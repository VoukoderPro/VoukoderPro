#include "voukoderpro.h"

#include <stdio.h>
#include <iostream>
#include <string>
#include <boost/process.hpp> 
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/log/sinks/debug_output_backend.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "json.hpp"
#include "System.h"
#include "Telemetry.h"
#include "Version.h"

namespace attrs = boost::log::attributes;
namespace expr = boost::log::expressions;

#include "JavaScript.h"

namespace VoukoderPro
{
	static std::string logbuffer;

	Client::Client() :
		homeDir(VOUKODERPRO_HOME), dataDir(VOUKODERPRO_DATA), sceneInfo({})
	{
		// Make sure all dirs exists where data should be stored
		if (!boost::filesystem::is_directory(dataDir))
			boost::filesystem::create_directories(dataDir);
	}

	/**
    * Shuts down all logging (and steam integration).
    */
	Client::~Client()
	{
		if (callbackSink)
			logging::core::get()->remove_sink(callbackSink);
	}

	/**
	* Updates the current scene.
	*/
	void Client::setScene(std::shared_ptr<SceneInfo> sceneInfo)
	{
		this->sceneInfo = sceneInfo;
	}

	/**
	* Sets up (steam integration,) logging and plugins.
	*/
	int Client::init(std::function<void(std::string)> logCallback)
	{
		if (logCallback && !callbackSink)
		{
			//
			boost::shared_ptr<callback_sink<char>> backend(new callback_sink<char>(logCallback));
			callbackSink = boost::shared_ptr< callback_sink_t >(new callback_sink_t(backend));
			callbackSink->set_formatter(
				expr::stream << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S")
				<< " (" << expr::attr<boost::log::trivial::severity_level >("Severity") << ")\t"
				//<< "[" << boost::log::expressions::attr<std::string>("Source") << ":" << boost::log::expressions::attr<int>("Line") << "] "
				<< expr::smessage
			);

			logging::core::get()->add_sink(callbackSink);
		}

		// Load all plugins
		assetList.clear();
		Assets::instance().list(assetList);

		return ERR_OK;
	}

	std::string Client::extension()
	{
		std::string outputId;

		// Is there an output using the NLE output path
		if (sceneInfo)
		{
			for (const auto& nodeInfo : sceneInfo->nodes)
			{
				if (nodeInfo->type == NodeInfoType::output &&
					nodeInfo->data.contains("url") && nodeInfo->data["url"].get<std::string>().find("$(OutputFilename)") != std::string::npos &&
					nodeInfo->inputs.size() == 1 && nodeInfo->inputs[0].size() == 1)
				{
					outputId = nodeInfo->inputs[0][0];
					break;
				}
			}

			if (!outputId.empty())
			{
				for (const auto& nodeInfo : sceneInfo->nodes)
				{
					// Skip all non-muxer nodes
					if (nodeInfo->type != NodeInfoType::muxer)
						continue;

					// Find the correct output
					for (const auto& output : nodeInfo->outputs)
					{
						if (std::find(output.begin(), output.end(), outputId) != output.end() &&
							nodeInfo->data.contains("id"))
						{
							std::string id = nodeInfo->data["id"].get<std::string>();
							const AVOutputFormat* format = av_guess_format(id.c_str(), NULL, NULL);

							return format->extensions;
						}
					}
				}
			}
		}

		return "voukoderpro";
	}

	int Client::open(config project)
	{
		if (!sceneInfo)
		{
			BLOG(error) << "No scene has been set!";
			return -1;
		}

		this->project = project;

		auto scene = sceneMgr->createScene(sceneInfo);
		if (!scene)
			return -1;

		// Prepare AVFrames for each track
		for (auto& track : project.tracks)
		{
            // Track type
            if (track.find(pPropType) == track.end())
            {
                BLOG(warning) << "Skipping track - No track type has been specified.";
                continue;
            }

            //
			const std::string type = std::get<std::string>(track.at(pPropType));
			if (type == "video")
			{
				if (track.find(pPropWidth) == track.end() ||
					track.find(pPropHeight) == track.end() ||
					track.find(pPropFormat) == track.end() ||
					track.find(pPropTimebaseNum) == track.end() ||
					track.find(pPropTimebaseDen) == track.end() ||
					track.find(pPropAspectNum) == track.end() ||
					track.find(pPropAspectDen) == track.end() ||
					track.find(pPropFieldOrder) == track.end())
				{
					BLOG(warning) << "Skipping video track - Not required properties are set.";
					continue;
				}

				// Create a new track
				std::shared_ptr<Track> vkdrTrack = std::make_shared<Track>();

				// Create a frame structure for this track
				vkdrTrack->frame = std::shared_ptr<AVFrame>(av_frame_alloc(), [](AVFrame* ptr) { av_frame_free(&ptr); });
				vkdrTrack->frame->pts = 0;
				vkdrTrack->frame->width = std::get<int>(track.at(pPropWidth));
				vkdrTrack->frame->height = std::get<int>(track.at(pPropHeight));

				if (track.find(pPropColorRange) != track.end())
					vkdrTrack->frame->color_range = (AVColorRange)av_color_range_from_name(std::get<std::string>(track.at(pPropColorRange)).c_str());

				if (track.find(pPropColorSpace) != track.end())
					vkdrTrack->frame->colorspace = (AVColorSpace)av_color_space_from_name(std::get<std::string>(track.at(pPropColorSpace)).c_str());

				if (track.find(pPropColorPrimaries) != track.end())
					vkdrTrack->frame->color_primaries = (AVColorPrimaries)av_color_primaries_from_name(std::get<std::string>(track.at(pPropColorPrimaries)).c_str());

				if (track.find(pPropColorTransfer) != track.end())
					vkdrTrack->frame->color_trc = (AVColorTransferCharacteristic)av_color_transfer_from_name(std::get<std::string>(track.at(pPropColorTransfer)).c_str());

				// For compressed frames create a decoder context
				const std::string format = std::get<std::string>(track.at(pPropFormat));
				const AVCodec* decoder = avcodec_find_decoder_by_name(format.c_str());
				if (decoder && format == "v210") // TODO
				{
					vkdrTrack->decoderCtx = std::shared_ptr<AVCodecContext>(avcodec_alloc_context3(decoder), [](AVCodecContext* ptr) { avcodec_free_context(&ptr); });
					vkdrTrack->decoderCtx->thread_type = FF_THREAD_FRAME;
					vkdrTrack->decoderCtx->thread_count = av_cpu_count();
					vkdrTrack->decoderCtx->width = vkdrTrack->frame->width;
					vkdrTrack->decoderCtx->height = vkdrTrack->frame->height;
					vkdrTrack->decoderCtx->pix_fmt = AV_PIX_FMT_YUV422P10LE; // TODO
					vkdrTrack->decoderCtx->time_base.num = std::get<int>(track.at(pPropTimebaseNum));
					vkdrTrack->decoderCtx->time_base.den = std::get<int>(track.at(pPropTimebaseDen));
					vkdrTrack->decoderCtx->framerate = av_inv_q(vkdrTrack->decoderCtx->time_base);
					vkdrTrack->decoderCtx->sample_aspect_ratio.num = std::get<int>(track.at(pPropAspectNum));
					vkdrTrack->decoderCtx->sample_aspect_ratio.den = std::get<int>(track.at(pPropAspectDen));
					std::string fieldOrder = std::get<std::string>(track.at(VoukoderPro::pPropFieldOrder));
					if (fieldOrder == "tff")
						vkdrTrack->decoderCtx->field_order = AV_FIELD_TT;
					else if (fieldOrder == "bff")
						vkdrTrack->decoderCtx->field_order = AV_FIELD_BB;
					else
						vkdrTrack->decoderCtx->field_order = AV_FIELD_PROGRESSIVE;
					vkdrTrack->decoderCtx->field_order = AVFieldOrder::AV_FIELD_PROGRESSIVE;

					if (avcodec_open2(vkdrTrack->decoderCtx.get(), decoder, NULL) < 0)
						return -1;

					vkdrTrack->compressedFrame = std::shared_ptr<AVPacket>(av_packet_alloc(), [](AVPacket* ptr) { av_packet_free(&ptr); });
					vkdrTrack->frame->format = vkdrTrack->decoderCtx->pix_fmt;
					track[pPropFormat] = std::string(av_get_pix_fmt_name(vkdrTrack->decoderCtx->pix_fmt));
				}
				else
				{
					vkdrTrack->decoderCtx = nullptr;
					vkdrTrack->compressedFrame = nullptr;
					vkdrTrack->frame->format = av_get_pix_fmt(format.c_str());
				}
				vkdrTrack->properties = track;

				tracks.push_back(vkdrTrack);
			}
			else if(type == "audio")
			{
				if (track.find(pPropSamplingRate) == track.end() ||
					track.find(pPropChannelLayout) == track.end() ||
					track.find(pPropFormat) == track.end())
				{
					BLOG(warning) << "Skipping audio track - Not required properties are set.";
					continue;
				}

				// Create a new track
				std::shared_ptr<Track> vkdrTrack = std::make_shared<Track>();
				vkdrTrack->nextPts = 0;
				vkdrTrack->decoderCtx = nullptr;
				vkdrTrack->compressedFrame = nullptr;
				vkdrTrack->properties = track;

				// Create a frame structure for this track
				vkdrTrack->frame = std::shared_ptr<AVFrame>(av_frame_alloc(), [](AVFrame* ptr) { av_frame_free(&ptr); });
				vkdrTrack->frame->sample_rate = std::get<int>(track.at(pPropSamplingRate));
				av_channel_layout_from_string(&vkdrTrack->frame->ch_layout, std::get<std::string>(track.at(pPropChannelLayout)).c_str());
				vkdrTrack->frame->format = av_get_sample_fmt(std::get<std::string>(track.at(pPropFormat)).c_str());

				tracks.push_back(vkdrTrack);
			}
		}

		// Create router instance
		router = std::make_unique<Router>(scene, project);
		
		// Init
		int ret = 0;
		if ((ret = router->init()) < 0)
			return ret;

		return router->open();
	}

	/**
	* Closes the encoder pipeline and writes the performance log.
	*/
	int Client::close(const bool savePerformance)
	{
		if (router)
			router->close();

		tracks.clear();

		int ret = ERR_OK;

		if (savePerformance)
			ret = savePerformanceLog();

		return ret;
	}

	/**
	* Opens the VoukoderPro Scene Designer tool.
	*/
	int Client::configure(const std::string id)
	{
		int ret = 0;

		if (id.empty())
			ret = boost::process::system(homeDir / "Designer.exe");
		else
			ret = boost::process::system(homeDir / "Designer.exe", id);

		// Handle potential errors
		if (ret < 0)
			BLOG(boost::log::trivial::error) << "Opening the Scene Manager tool failed. (Error code: " << error << ")";

		return ret;
	}

	/**
	* Opens the scene selection dialog.
	*/
	int Client::sceneSelect(std::string& name)
	{
		boost::process::ipstream pipe;
		boost::process::child c((homeDir / "Designer.exe").string(), boost::process::args({ "/sceneselect", name }), boost::process::std_out > pipe);
		
		std::getline(pipe, name);
		c.wait();

		// On windows strip the CR
		name.erase(std::remove(name.begin(), name.end(), '\r'), name.end());

		return c.exit_code();
	}

	/**
	* Sends a raw video frame buffer to VoukoderPro.
	*/
	int Client::writeVideoFrame(const int trackIdx, const int64_t frameIdx, uint8_t** buffer, const int* lineSize)
	{
		if (trackIdx >= tracks.size())
			return -1;

		// Get the track
		std::shared_ptr<Track> track = tracks.at(trackIdx);

		track->frame->pts = frameIdx;

		// Fill the frame with data
		const int planes = av_pix_fmt_count_planes((AVPixelFormat)track->frame->format);
		for (int i = 0; i < planes; i++)
		{
			track->frame->data[i] = buffer[i];
			track->frame->linesize[i] = lineSize[i];
		}

		// Send the frame to the router
		return router->sendFrame(trackIdx, track->frame);
	}

	/**
    * Sends a compressed video frame buffer to VoukoderPro.
    */
	int Client::writeCompressedVideoFrame(const int trackIdx, const int64_t frameIdx, uint8_t* compressedBuffer, const int lineSize)
	{
		if (trackIdx >= tracks.size())
			return -1;

		// Get the track
		std::shared_ptr<Track> track = tracks.at(trackIdx);

		// Assign compressed data
		track->compressedFrame->data = compressedBuffer;
		track->compressedFrame->size = lineSize * track->frame->height;
		track->compressedFrame->pts = frameIdx;

		int ret = avcodec_send_packet(track->decoderCtx.get(), track->compressedFrame.get());

		std::shared_ptr<AVFrame> tmp_frame = std::shared_ptr<AVFrame>(av_frame_alloc(), [](AVFrame* ptr) { av_frame_free(&ptr); });

		while (ret >= 0)
		{
			ret = avcodec_receive_frame(track->decoderCtx.get(), tmp_frame.get());

			if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
				return 0;
			else if (ret < 0)
				return ret;

			// Apply color format settings
			if (track->properties.find(VoukoderPro::pPropColorRange) != track->properties.end())
				track->frame->color_range = (AVColorRange)av_color_range_from_name(std::get<std::string>(track->properties.at(VoukoderPro::pPropColorRange)).c_str());
			if (track->properties.find(VoukoderPro::pPropColorSpace) != track->properties.end())
				track->frame->colorspace = (AVColorSpace)av_color_space_from_name(std::get<std::string>(track->properties.at(VoukoderPro::pPropColorSpace)).c_str());
			if (track->properties.find(VoukoderPro::pPropColorPrimaries) != track->properties.end())
				track->frame->color_primaries = (AVColorPrimaries)av_color_primaries_from_name(std::get<std::string>(track->properties.at(VoukoderPro::pPropColorPrimaries)).c_str());
			if (track->properties.find(VoukoderPro::pPropColorTransfer) != track->properties.end())
				track->frame->color_trc = (AVColorTransferCharacteristic)av_color_transfer_from_name(std::get<std::string>(track->properties.at(VoukoderPro::pPropColorTransfer)).c_str());

			// Send the frame to the router
			ret = router->sendFrame(trackIdx, tmp_frame);
		}

		return ret;
	}

	/**
	* Sends a raw audio sample buffer to VoukoderPro.
	*/
	int Client::writeAudioSamples(const int trackIdx, uint8_t** buffer, const int linesize)
	{
		if (trackIdx >= tracks.size())
			return -1;

		// Get the track
		std::shared_ptr<Track> track = tracks.at(trackIdx);

		track->frame->pts = track->nextPts;

		const AVSampleFormat format = (AVSampleFormat)track->frame->format;

		// Perform a s24 to s32 conversion because FFmpeg doesn't know s24
		if (format == AV_SAMPLE_FMT_S32 &&
			track->properties.find(VoukoderPro::pPropOriginalFormat) != track->properties.end() &&
			std::get<std::string>(track->properties.at(VoukoderPro::pPropOriginalFormat)) == "s24")
		{
			// How many samples do we have?
			track->frame->nb_samples = linesize / 3 / track->frame->ch_layout.nb_channels;

			// Reserve buffers
			if (track->frame->pts == 0)
				av_frame_get_buffer(track->frame.get(), 0);

			// Conversion
			int p = 0;
			for (int i = 0; i < linesize; i += 3)
			{
				track->frame->data[0][p] = 0x00;
				track->frame->data[0][p + 1] = buffer[0][i];
				track->frame->data[0][p + 2] = buffer[0][i + 1];
				track->frame->data[0][p + 3] = buffer[0][i + 2];
				p += 4;
			}

			track->frame->linesize[0] = p;
		}
		else
		{
			const int bytes = av_get_bytes_per_sample(format);

			// Fill the frame with data
			if (av_sample_fmt_is_planar(format))
			{
				for (int i = 0; i < track->frame->ch_layout.nb_channels; i++)
				{
					track->frame->data[i] = buffer[i];
					track->frame->linesize[i] = linesize;
				}

				track->frame->nb_samples = linesize / bytes;
			}
			else
			{
				track->frame->nb_samples = linesize / bytes / track->frame->ch_layout.nb_channels;
				track->frame->data[0] = buffer[0];
				track->frame->linesize[0] = linesize;
			}
		}

		// Send the frame to the router
		int ret = router->sendFrame(trackIdx, track->frame);

		track->nextPts += track->frame->nb_samples;

		return ret;
	}

	void Client::log(const std::string msg)
	{
		BLOG(info) << msg;
	}

	int Client::event(const std::string& name, const std::map<std::string, std::string>& params)
	{
		return Telemetry::instance().event(name, params);
	}

	std::vector<AssetInfo> Client::plugins()
	{
		return this->assetList;
	}

	int Client::savePerformanceLog()
	{
		nlohmann::ordered_json system;

		const auto quantities = iware::cpu::quantities();
		system["processor"] = {
			{ "Architecture", architecture_name(iware::cpu::architecture()) },
			{ "Vendor ID", iware::cpu::vendor_id() },
			{ "Model name", iware::cpu::model_name() },
			{ "Frequency", iware::cpu::frequency() },
			{ "CPU packages", quantities.packages },
			{ "Physical CPUs", quantities.physical },
			{ "Logical CPUs", quantities.logical },
		};

		for (auto&& set : { std::make_pair("3D-now!", iware::cpu::instruction_set_t::s3d_now),
							  std::make_pair("MMX", iware::cpu::instruction_set_t::mmx),
							  std::make_pair("SSE", iware::cpu::instruction_set_t::sse),
							  std::make_pair("SSE2", iware::cpu::instruction_set_t::sse2),
							  std::make_pair("SSE3", iware::cpu::instruction_set_t::sse3),
							  std::make_pair("AVX", iware::cpu::instruction_set_t::avx) })
			system["processor"][set.first] = iware::cpu::instruction_set_supported(set.second);

		const auto memory = iware::system::memory();
		system["memory"] = {
			{ "Physical", {
				{ "Total", memory.physical_total },
				{ "Available", memory.physical_available },
				}
			},
			{ "Virtual", {
				{ "Total", memory.virtual_total },
				{ "Available", memory.virtual_available },
				}
			}
		};

		const auto OS_info = iware::system::OS_info();
		system["os"] = {
			{ "Name", OS_info.name },
			{ "Full name", OS_info.full_name },
			{ "Version", std::to_string(OS_info.major) + "." + std::to_string(OS_info.minor) + "." + std::to_string(OS_info.patch) + "." + std::to_string(OS_info.build_number) }
		};

		const auto kernel_info = iware::system::kernel_info();
		system["kernel"] = {
			{ "Variant", kernel_variant_name(kernel_info.variant) },
			{ "Version", std::to_string(kernel_info.major) + "." + std::to_string(kernel_info.minor) + "." + std::to_string(kernel_info.patch) + "." + std::to_string(kernel_info.build_number) }
		};

		const auto device_properties = iware::gpu::device_properties();
		system["devices"] = nlohmann::ordered_json::array();
		for (auto i = 0u; i < device_properties.size(); ++i) {
			const auto& properties_of_device = device_properties[i];
			system["devices"].push_back(
				{
					{ "Vendor", vendor_name(properties_of_device.vendor) },
					{ "Name", properties_of_device.name },
					{ "RAM size", properties_of_device.memory_size },
					{ "Cache size", properties_of_device.cache_size }
				});
		}

		// Create the performance log
		nlohmann::ordered_json perfLog;
		perfLog["system"] = system;

		/*
		perfLog["scene"] = {

		};
		*/

		int ret = ERR_OK;
		if ((ret = router->getPerformanceLog(perfLog)) < 0)
			return ret;

		boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
		boost::posix_time::time_facet* facet = new boost::posix_time::time_facet("%Y%m%d-%H%M%S");
		std::stringstream key;
		key << now;

		// Store performance data
		return PerformanceManager::instance((dataDir / "performance").string()).create(key.str(), perfLog.dump());
	}

	BOOST_LOG_GLOBAL_LOGGER_INIT(lg, logger_t)
	{
		logger_t lg;
		
		// Logging
		logging::add_common_attributes();

        fs::path dataDir(VOUKODERPRO_DATA);

		logging::add_file_log(
            boost::log::keywords::file_name = (dataDir / "logs" / "voukoderpro-%N.log").c_str(),
			boost::log::keywords::open_mode = std::ios_base::app,
            boost::log::keywords::rotation_size = 10 * 1024 * 1024,
			boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0),
			boost::log::keywords::format = (
				expr::stream << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S")
                << " (" << expr::attr<boost::log::trivial::severity_level >("Severity") << ")\t"
				<< "[" << boost::log::expressions::attr<std::string>("Source") << ":" << boost::log::expressions::attr<int>("Line") << "] "
				<< expr::smessage
				)
		)->locked_backend()->auto_flush();

		logging::core::get()->set_filter
		(
			logging::trivial::severity >= logging::trivial::trace
		);

		av_log_set_level(AV_LOG_INFO);
		av_log_set_callback([](void*, int level, const char* szFmt, va_list varg) // TODO: Optimize this
			{
				int len = vsnprintf(nullptr, 0, szFmt, varg);

				std::string msg;
				msg.resize(len);
				vsnprintf(&msg[0], len + 1, szFmt, varg);

				logbuffer += msg;

				size_t pos = 0;
				if ((pos = logbuffer.find_first_of('\n', 0)) != std::string::npos)
				{
					BOOST_LOG_SEV(lg::get(), trace) << boost::log::add_value("Source", "FFmpeg") << boost::log::add_value<int>("Line", 0) << logbuffer.substr(0, pos);

					logbuffer = logbuffer.substr(pos + 1);
				}
			});

		return lg;
	}
}
