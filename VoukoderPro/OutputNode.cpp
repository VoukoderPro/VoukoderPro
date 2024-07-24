#include "OutputNode.h"

extern "C" {
#include <libavformat/avformat.h>
#include <libavutil/stereo3D.h>
#include <libavutil/spherical.h>
#include <libavutil/mastering_display_metadata.h>
}

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/algorithm/string.hpp>
#include <regex>

#include "JavaScript.h"
#include "Version.h"

namespace VoukoderPro
{
	OutputNode::OutputNode(std::shared_ptr<NodeInfo> nodeInfo):
		BaseNode(nodeInfo)
	{}

	int OutputNode::preinit(const int nleTrackIndex, std::shared_ptr<NodeData> nleData)
	{
		// Muxer format
		const std::string formatId = inputs.front().lock()->nodeInfo->data["id"].get<std::string>();
		const AVOutputFormat* outputFormat = av_guess_format(formatId.c_str(), NULL, NULL);
		if (outputFormat)
		{
			// Do we have a filename supplied by the NLE?
			if (nleData->properties.find(pPropFilename) != nleData->properties.end())
			{
				// Get the first file extension of this format 
				std::string extension = outputFormat->extensions;
				size_t pos = extension.find(',');
				if (pos != std::string::npos)
					extension = extension.substr(0, pos);

				// The filename delivered by the NLE
				std::string filename = std::get<std::string>(nleData->properties.at(pPropFilename));

				// Replace a possible .voukoderpro suffix by the real extension
				filename = std::regex_replace(filename, std::regex("\\.voukoderpro$"), "." + extension);
				nleData->properties[pPropFilename] = filename;
			}
		}
		else
		{
			BLOG(error) << "Format " << nodeId << " is not known!";
			return ERR_MUXER_NOT_FOUND;
		}

		return BaseNode::preinit(nleTrackIndex, nleData);
	}

	/**
	* Initialize and configure the output format.
	*/
	int OutputNode::init()
	{
		// Input required
		if (inputs.size() != 1)
			return ERR_INPUT_NODE_REQUIRED;

		// Output url
		if (!nodeInfo->data.contains(pPropOutputUrl))
		{
			BLOG(warning) << "No output url has been specified.";

			return ERR_PARAMETER_MISSING;
		}
		std::string url = nodeInfo->data[pPropOutputUrl].get<std::string>();

		// Muxer format
        const std::string formatId = inputs.front().lock()->nodeInfo->data["id"].get<std::string>();
		formatCtx->oformat = av_guess_format(formatId.c_str(), NULL, NULL);
		if (!formatCtx->oformat)
		{
			BLOG(error) << "Format " << formatId << " is not known!";
			return ERR_MUXER_NOT_FOUND;
		}

		// Do we have a filename supplied by the NLE?
		if (data.size() == 0)
		{
			BLOG(error) << "No data available in output node.";
			return ERR_FAIL;
		}

		const auto& nleData = data.begin()->second;

		// Do we have a real file name?
		const std::string sfilename = std::get<std::string>(nleData->properties[pPropFilename]);
		if (sfilename == "NUL")
		{
			formatCtx->url = av_strdup("NUL");
		}
		else
		{
			JavaScript js;

			// Correct output filename
			if (nleData->properties.find(pPropFilename) != nleData->properties.end())
			{
				boost::filesystem::path filename(sfilename);

				// Fill in the filename data
				std::stringstream javascript;
				javascript << "var OutputFile = { Absolute: '";
				javascript << boost::replace_all_copy(sfilename, "\\", "\\\\");
				javascript << "', Path: '";
				javascript << boost::replace_all_copy(filename.parent_path().string(), "\\", "\\\\");
				javascript << "', Name: '";
				javascript << boost::replace_all_copy(filename.stem().string(), "\\", "\\\\");
				javascript << "', Extension: '";
				javascript << boost::replace_all_copy(filename.extension().string(), "\\", "\\\\");
				javascript << "' };";

				// Evaluate
				js.eval(javascript.str());
			}

			js.replaceJavaScript(url);

			// Setup the format context
			formatCtx->url = av_strdup(url.c_str());
		}

		// Assign meta information
		if (nodeInfo->data.contains("meta") && nodeInfo->data.count("meta") > 0)
			for (auto& [key, val] : nodeInfo->data["meta"].items())
				av_dict_set(&formatCtx->metadata, key.c_str(), val.get<std::string>().c_str(), 0);

		// Mark myself as encoding tool in the mp4/mov container
		std::stringstream encoder;
		encoder << APP_NAME << " " << VKDRPRO_VERSION_MAJOR << "." << VKDRPRO_VERSION_MINOR << "." << VKDRPRO_VERSION_PATCH;
		av_dict_set(&formatCtx->metadata, "encoding_tool", encoder.str().c_str(), 0);

		// Chapters
		if (nleData->trackProperties.find(pPropChapters) != nleData->trackProperties.end() &&
			nleData->trackProperties.find(pPropTimebaseNum) != nleData->trackProperties.end() &&
			nleData->trackProperties.find(pPropTimebaseDen) != nleData->trackProperties.end())
		{
			const std::string chaptersData = std::get<std::string>(nleData->trackProperties[pPropChapters]);
			const int timeBaseNum = std::get<int>(nleData->trackProperties[pPropTimebaseNum]);
			const int timeBaseDen = std::get<int>(nleData->trackProperties[pPropTimebaseDen]);

			std::vector<std::string> chapters;
			boost::split(chapters, chaptersData, boost::is_any_of(","));

			formatCtx->chapters = (AVChapter**)av_realloc_f(formatCtx->chapters, chapters.size(), sizeof(*formatCtx->chapters));

			std::vector<AVChapter*> tmp;

			// Process all chapter data
			for (const auto& chapter : chapters)
			{
				std::vector<std::string> field;
				boost::split(field, chapter, boost::is_any_of(";"));

				if (field.size() < 2)
				{
					BLOG(warning) << "Insufficient chapter information: " << chapter;
					continue;
				}

				// Create the chapter info
				AVChapter* pChapter = (AVChapter*)av_mallocz(sizeof(AVChapter));
				pChapter->id = tmp.size();
				pChapter->time_base = { timeBaseNum, timeBaseDen };
				pChapter->start = std::stoll(field[1].c_str());
				av_dict_set(&pChapter->metadata, "title", field[0].c_str(), 0);

				// Do we have an end frame number?
				if (field.size() == 3)
					pChapter->end = std::stoll(field[2].c_str());
				else
					pChapter->end = -1;
				tmp.push_back(pChapter);
			}

			// Assign the chapters (and interpolate possible end frame number data)
			size_t i;
			for (i = 0; i < tmp.size(); i++)
			{
				AVChapter* t = tmp.at(i);
				if (t->end == -1 && i < tmp.size() - 1)
					t->end = tmp.at(i + 1)->start - 1;

				formatCtx->chapters[formatCtx->nb_chapters++] = t;
			}
		}

		BLOG(severity_level::info) << "Successfully initialized the output format.";

		return BaseNode::init();
	}

	/**
	* Opens the output if all streams have been set up.
	*/
	int OutputNode::open()
	{
		int ret = ERR_OK;

		// Are all streams present?
		for (unsigned int i = 0; i < formatCtx->nb_streams; i++)
		{
			AVStream* stream = formatCtx->streams[i];

			// Is this stream in the stream -> codecCtx map?
			if (streams.find(stream) == streams.end())
			{
				BLOG(error) << "Unable to find stream " << i << "in stream list.";
				return ERR_STREAM_NOT_FOUND;
			}

			// Copy codec params to stream
			if ((ret = avcodec_parameters_from_context(stream->codecpar, streams.at(stream).first.get())) < 0)
			{
				BLOG(error) << "Unable to copy codec params to stream " << i;
				return ERR_FFMPEG(err);
			}
		}

		// Open output
		if ((ret = avio_open(&formatCtx->pb, formatCtx->url, AVIO_FLAG_WRITE)) < 0)
		{
			BLOG(error) << "Unable to open output: " << formatCtx->url;
			return ERR_FFMPEG(err);
		}

		// Dump format settings
		av_dump_format(formatCtx.get(), 0, formatCtx->url, 1);

		// Get options
		AVDictionary* opts = nullptr;
		if (nodeInfo->data.contains("params")  && nodeInfo->data.count("params") > 0)
			for (auto& [key, val] : nodeInfo->data["params"].items())
				av_dict_set(&opts, key.c_str(), val.get<std::string>().c_str(), 0);

		// Write header
		if ((ret = avformat_write_header(formatCtx.get(), &opts)) < 0)
		{
			BLOG(error) << "Unable to write format header.";
			return ERR_FFMPEG(err);
		}

		BLOG(severity_level::info) << "Successfully opened the output format.";

		return BaseNode::open();
	}

	/**
	* Writes the trailer and closes the output.
	*/
	int OutputNode::close()
	{
		int ret = ERR_OK;

		// Send format trailer
		if ((ret = av_write_trailer(formatCtx.get())) < 0)
		{
			BLOG(error) << "Unable to write format trailer.";
			return ERR_FFMPEG(err);
		}

		// Close output
		if ((ret = avio_close(formatCtx->pb)) < 0)
		{
			BLOG(error) << "Unable to close output url.";
			return ERR_FFMPEG(err);
		}

		BLOG(severity_level::info) << "Successfully closed output.";

		return BaseNode::close();
	}

	/**
	* Creates, configures and assignes a stream to the muxer.
	*/
	int OutputNode::registerStream(std::shared_ptr<AVCodecContext> codecCtx, const int index, BaseNode* pEncoderNode)
	{
		// Create output stream
		AVStream* stream = avformat_new_stream(formatCtx.get(), codecCtx->codec);
		stream->id = formatCtx->nb_streams - 1;
		stream->time_base = codecCtx->time_base;
		stream->avg_frame_rate = av_inv_q(stream->time_base);

		// Remember codecCtx<->stream association
		streams.insert(std::make_pair(stream, std::make_pair(codecCtx, index)));

		int ret = ERR_OK;

		// Deal with possible side data
		if ((ret = handleSideData(stream, pEncoderNode->nodeInfo)) < 0)
			return ret;

		// Timecode?
		const auto& nleData = data.begin()->second;
		if (nleData->trackProperties.find(pPropTimecode) != nleData->trackProperties.end())
		{
			std::string timecode = std::get<std::string>(nleData->trackProperties.at(pPropTimecode));
			av_dict_set(&stream->metadata, "timecode", timecode.c_str(), 0);
		}

		avcodec_parameters_from_context(stream->codecpar, codecCtx.get());

		// Global headers
		if (formatCtx->oformat->flags & AVFMT_GLOBALHEADER)
			codecCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

		BLOG(debug) << "Registered stream " << pEncoderNode->nodeInfo->id << ":" << stream->id;

		return BaseNode::registerStream(codecCtx, index, pEncoderNode);
	}

	/**
	* Accepts one frame and feeds it to the muxer.
	*/
	int OutputNode::mux(std::shared_ptr<AVCodecContext> codecCtx, const int index, std::shared_ptr<AVPacket> packet)
	{
		// Find the right stream
		AVStream* stream = nullptr;
		for (const auto& [otherStream, otherCodecCtx] : streams)
		{
			if (otherCodecCtx.first == codecCtx && otherCodecCtx.second == index)
			{
				stream = otherStream;
				break;
			}
		}

		// Abort if we couldn't find the right stream
		if (stream == nullptr)
		{
			BLOG(error) << "Was not able to find the right stream.";

			return ERR_STREAM_NOT_FOUND;
		}

		packet->stream_index = stream->id;

		// Clone the packet to keep it for potentially other muxers
		AVPacket* clonedPacket = av_packet_clone(packet.get());

		BLOG(debug) << "Muxing packet with pts " << clonedPacket->pts;

		// Force constant frame rate (necessary, but why???)
		if (stream->codecpar->codec_type == AVMediaType::AVMEDIA_TYPE_VIDEO && clonedPacket->duration == 0)
			clonedPacket->duration = 1;

		//data->performance.start("output[stream=" + std::to_string(index) + ":"  + std::to_string(stream->id) + ":pts=" + std::to_string(packet->pts) + "]");

		// Rescale time base
		av_packet_rescale_ts(clonedPacket, codecCtx->time_base, stream->time_base);

		int err = ERR_OK;
		if ((err = av_interleaved_write_frame(formatCtx.get(), clonedPacket)) < 0)
		{
			//data->performance->end();

			return ERR_FFMPEG(err);
		}

		//data->performance.end();

		return err;
	}

	/**
	* Handles potential stream side data.
	*/
	int OutputNode::handleSideData(AVStream* stream, std::shared_ptr<NodeInfo> nodeInfo)
	{
		int ret = ERR_OK;

		// Is there any sidedata?
		if (!nodeInfo->data.contains("sidedata"))
            return ret;

		// Sidedata
		auto sidedata = nodeInfo->data["sidedata"];

		// Is there any meta data?
		if (sidedata.contains("meta"))
		{
			// Metadata
            const auto meta = sidedata["meta"];

			// Language?
			if (meta.contains(pPropLanguage))
			{
				std::string language = meta[pPropLanguage].get<std::string>();
				av_dict_set(&stream->metadata, "language", language.c_str(), 0);
			}
		}

		// Stereo 3D
		// Src: https://github.com/FFmpeg/FFmpeg/blob/master/libavutil/stereo3d.c
		if (sidedata.contains("stereo3d"))
			ret = injectStereoData(stream, sidedata["stereo3d"]);

		// Spherical
		// Src: https://github.com/FFmpeg/FFmpeg/blob/master/libavutil/spherical.c
		if (sidedata.contains("spherical") && ret == ERR_OK)
			ret = injectSphericalData(stream, sidedata["spherical"]);

		// Mastering Display Data
		// Src: https://github.com/FFmpeg/FFmpeg/blob/master/libavutil/mastering_display_metadata->c
		if (sidedata.contains("mdd") && ret == ERR_OK)
			ret = injectMasteringDisplayData(stream, sidedata["mdd"]);

		// Content Light Levels
		// Src: https://github.com/FFmpeg/FFmpeg/blob/master/libavutil/mastering_display_metadata->c
		if (sidedata.contains("cll") && ret == ERR_OK)
			ret = injectContentLightLevels(stream, sidedata["cll"]);

		return ret;
	}

	/**
	* Injects stereoscopic side data to the stream.
	*/
	int OutputNode::injectStereoData(AVStream* stream, nlohmann::ordered_json& props)
	{
		// Add basic data
		AVStereo3D* stereo_3d = av_stereo3d_alloc();

		// Invert?
		if (props.find("invert") != props.end())
			stereo_3d->flags = props["invert"].get<bool>() ? 1 : 0;

		// Convert
		if (props.find("type") != props.end())
		{
			std::string type = props["type"].get<std::string>();
			int value = av_stereo3d_from_name(type.c_str());
			if (value < 0)
			{
				BLOG(error) << "Unknown 3D stereo type: " << type;

				return ERR_PARAMETER_VALUE_NOT_SUPPORTED;
			}
			stereo_3d->type = static_cast<AVStereo3DType>(value);
		}

		// Add views
		if (props.find("view") != props.end())
		{
			std::string view = props["view"].get<std::string>();
			if (view == "2d")
				stereo_3d->view = AV_STEREO3D_VIEW_PACKED;
			else if (view == "left")
				stereo_3d->view = AV_STEREO3D_VIEW_LEFT;
			else if (view == "right")
				stereo_3d->view = AV_STEREO3D_VIEW_RIGHT;
			else
			{
				BLOG(error) << "Unknown 3D stereo view: " << view;

				return ERR_PARAMETER_VALUE_NOT_SUPPORTED;
			}
		}

		int err = ERR_OK;
		if ((err = av_stream_add_side_data(stream, AV_PKT_DATA_STEREO3D, (uint8_t*)stereo_3d, sizeof(*stereo_3d))) < 0)
			return ERR_FFMPEG(err);

		return err;
	}

	/**
	* Injects spherical side data to the stream.
	*/
	int OutputNode::injectSphericalData(AVStream* stream, nlohmann::ordered_json& props)
	{
		// Add basic data
		size_t size;
		AVSphericalMapping* spherical = av_spherical_alloc(&size);
		if (props.find("yaw") != props.end())
			spherical->yaw = props["yaw"].get<int>();
		if (props.find("pitch") != props.end())
			spherical->pitch = props["pitch"].get<int>();
		if (props.find("roll") != props.end())
			spherical->roll = props["roll"].get<int>();

		// Convert
		if (props.find("projection") != props.end())
		{
			std::string projection = props["projection"].get<std::string>();
			int value = av_spherical_from_name(projection.c_str());
			if (value < 0)
			{
				BLOG(error) << "Unknown spherical projection: " << projection;

				return ERR_PARAMETER_VALUE_NOT_SUPPORTED;
			}

			spherical->projection = static_cast<AVSphericalProjection>(value);
		}

		// Parameters
		switch (spherical->projection)
		{
		case AV_SPHERICAL_CUBEMAP:
			if (props.find("padding") != props.end())
				spherical->padding = props["padding"].get<int>();
			break;
		case AV_SPHERICAL_EQUIRECTANGULAR:
			break;
		case AV_SPHERICAL_EQUIRECTANGULAR_TILE:
			if (props.find("bound_top") != props.end())
				spherical->bound_top = props["bound_top"].get<int>();
			if (props.find("bound_left") != props.end())
				spherical->bound_left = props["bound_left"].get<int>();
			if (props.find("bound_right") != props.end())
				spherical->bound_right = props["bound_right"].get<int>();
			if (props.find("bound_bottom") != props.end())
				spherical->bound_bottom = props["bound_bottom"].get<int>();
			break;
		default:
			BLOG(error) << "Projection not supported.";
			return ERR_PARAMETER_VALUE_NOT_SUPPORTED;
		}

		int err = ERR_OK;
		if ((err = av_stream_add_side_data(stream, AV_PKT_DATA_SPHERICAL, (uint8_t*)spherical, size)) < 0)
			return ERR_FFMPEG(err);

		return err;
	}

	/**
	* Injects MDD side data to the stream.
	*/
	int OutputNode::injectMasteringDisplayData(AVStream* stream, nlohmann::ordered_json& props)
	{
		AVMasteringDisplayMetadata* mdcv = av_mastering_display_metadata_alloc();

		// Do we have mdcv data?
		if (props.find("primaries") != props.end())
		{
			std::string name = props["primaries"].get<std::string>();
			int value = av_color_primaries_from_name(name.c_str());
			if (value < 0)
			{
				BLOG(error) << "Unknown color primary: " << name;

				return ERR_PARAMETER_VALUE_NOT_SUPPORTED;
			}

			AVColorPrimaries primary = static_cast<AVColorPrimaries>(value);

			mdcv->has_primaries = 1;

			if (primary == AVCOL_PRI_BT709)
			{
				// Rec.709 : --master - display G(15000, 30000)B(7500, 3000)R(32000, 16500)WP(15635, 16450)L(10000000, 1) --max - cll 1000, 1
				// RGB : G(x = 0.30, y = 0.60), B(x = 0.150, y = 0.060), R(x = 0.640, y = 0.330), WP(x = 0.3127, y = 0.329), L(max = 1000, min = 0.0000)
				mdcv->display_primaries[0][0] = av_d2q(0.64, INT_MAX); // Rx
				mdcv->display_primaries[0][1] = av_d2q(0.33, INT_MAX); // Ry
				mdcv->display_primaries[1][0] = av_d2q(0.3, INT_MAX);  // Gx
				mdcv->display_primaries[1][1] = av_d2q(0.6, INT_MAX);  // Gy
				mdcv->display_primaries[2][0] = av_d2q(0.15, INT_MAX); // Bx
				mdcv->display_primaries[2][1] = av_d2q(0.06, INT_MAX); // By
			}
			else if (primary == AVCOL_PRI_SMPTE432)
			{
				// DCI - P3: --master - display G(13250, 34500)B(7500, 3000)R(34000, 16000)WP(15635, 16450)L(10000000, 1) --max - cll 1000, 1
				// RGB : G(x = 0.265, y = 0.690), B(x = 0.150, y = 0.060), R(x = 0.680, y = 0.320), WP(x = 0.3127, y = 0.329), L(max = 1000, min = 0.0000)
				mdcv->display_primaries[0][0] = av_d2q(0.68, INT_MAX);  // Rx
				mdcv->display_primaries[0][1] = av_d2q(0.32, INT_MAX);  // Ry
				mdcv->display_primaries[1][0] = av_d2q(0.265, INT_MAX); // Gx
				mdcv->display_primaries[1][1] = av_d2q(0.69, INT_MAX);  // Gy
				mdcv->display_primaries[2][0] = av_d2q(0.15, INT_MAX);  // Bx
				mdcv->display_primaries[2][1] = av_d2q(0.06, INT_MAX);  // By
			}
			else if (primary == AVCOL_PRI_BT2020)
			{
				// Rec.2020 : --master - display G(8500, 39850)B(6550, 2300)R(35400, 14600)WP(15635, 16450)L(10000000, 1) --max - cll 1000, 1
				// RGB : G(x = 0.170, y = 0.797), B(x = 0.131, y = 0.046), R(x = 0.708, y = 0.292), WP(x = 0.3127, y = 0.329), L(max = 1000, min = 0.0000)
				mdcv->display_primaries[0][0] = av_d2q(0.708, INT_MAX); // Rx
				mdcv->display_primaries[0][1] = av_d2q(0.292, INT_MAX); // Ry
				mdcv->display_primaries[1][0] = av_d2q(0.17, INT_MAX);  // Gx
				mdcv->display_primaries[1][1] = av_d2q(0.797, INT_MAX); // Gy
				mdcv->display_primaries[2][0] = av_d2q(0.131, INT_MAX); // Bx
				mdcv->display_primaries[2][1] = av_d2q(0.046, INT_MAX); // By
			}
			else
			{
				BLOG(error) << "Color primary not (yet) supported: " << name;

				return ERR_PARAMETER_VALUE_NOT_SUPPORTED;
			}

			mdcv->white_point[0] = av_d2q(0.3127, INT_MAX); // WPx
			mdcv->white_point[1] = av_d2q(0.329, INT_MAX);  // WPy
		}

		// Luminance
		if (props.find("luminance") != props.end())
		{
			mdcv->has_luminance = 1;

			auto luminance = props["luminance"];

			if (luminance.find("min") != luminance.end())
			{
				const int precision = 10000;
				mdcv->min_luminance.num = static_cast<int>(luminance["min"].get<float>() * (float)precision);
				mdcv->min_luminance.den = precision;
			}

			if (luminance.find("max") != luminance.end())
			{
				mdcv->max_luminance.num = static_cast<int>(luminance["max"].get<int>());
				mdcv->max_luminance.den = 1;
			}
		}

		int err = ERR_OK;
		if ((err = av_stream_add_side_data(stream, AV_PKT_DATA_MASTERING_DISPLAY_METADATA, (uint8_t*)mdcv, sizeof(*mdcv))) < 0)
			return ERR_FFMPEG(err);

		return err;
	}

	/**
	* Injects CLL side data to the stream.
	*/
	int OutputNode::injectContentLightLevels(AVStream* stream, nlohmann::ordered_json& props)
	{
		size_t size;
		AVContentLightMetadata* cll = av_content_light_metadata_alloc(&size);
		if (props.find("max_cll") != props.end())
			cll->MaxCLL = props["max_cll"].get<unsigned>();
		if (props.find("max_fall") != props.end())
			cll->MaxFALL = props["max_fall"].get<unsigned>();

		int err = ERR_OK;
		if ((err = av_stream_add_side_data(stream, AV_PKT_DATA_CONTENT_LIGHT_LEVEL, (uint8_t*)cll, size)) < 0)
			return ERR_FFMPEG(err);

		return err;
	}
}
