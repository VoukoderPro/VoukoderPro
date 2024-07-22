#include "EncoderNode.h"

extern "C" {
#include <libavutil/stereo3D.h>
#include <libavutil/spherical.h>
#include <libavutil/mastering_display_metadata.h>
}

#include "Assets.h"

namespace VoukoderPro
{
	static enum AVPixelFormat get_cuda_format(AVCodecContext*, const enum AVPixelFormat* pix_fmts)
	{
		const enum AVPixelFormat* p;
		for (p = pix_fmts; *p != AV_PIX_FMT_NONE; p++) {
			if (*p == AV_PIX_FMT_CUDA)
				return *p;
		}

		return AV_PIX_FMT_NONE;
	}

	static enum AVPixelFormat get_qsv_format(AVCodecContext*, const enum AVPixelFormat* pix_fmts)
	{
		const enum AVPixelFormat* p;
		for (p = pix_fmts; *p != AV_PIX_FMT_NONE; p++) {
			if (*p == AV_PIX_FMT_QSV)
				return *p;
		}

		return AV_PIX_FMT_NONE;
	}

	EncoderNode::EncoderNode(std::shared_ptr<NodeInfo> nodeInfo):
		BaseNode(nodeInfo)
	{}

	/**
	* Complete the filter config string.
	*/
	int EncoderNode::preinit(const int nleTrackIndex, std::shared_ptr<NodeData> nleData)
	{
		// Do we have a nodeId given?
		if (nodeInfo->data.find("id") != nodeInfo->data.end())
		{
			const std::string id = nodeInfo->data["id"].get<std::string>();

			auto instance = Assets::instance().createAssetInstance(id, nodeInfo->type);
			if (!instance)
			{
				BLOG(warning) << boost::describe::enum_to_string<NodeInfoType>(nodeInfo->type, 0) << " node (Id: " << nodeInfo->id << ") does not have a plugin id associated which is required for this type!";
				return ERR_FAIL;
			}

			// Get the input format
			if (nleData->trackProperties.find(pPropFormat) == nleData->trackProperties.end())
			{
				BLOG(error) << "The encoder has no input format set!";
				return ERR_PARAMETER_MISSING;
			}

			plugins.insert(std::make_pair(nleTrackIndex, instance));
		}

        // Get the encoder format. Is one specified?
        std::string format;
		if (nodeInfo->data.contains(pPropFormat))
			format = nodeInfo->data[pPropFormat].get<std::string>();
		else
		{
			const auto& plugin = plugins.begin()->second;

			// Get the first supported format
			if (plugin->info.id == nodeId && plugin->info.formats.size() > 0)
				format = plugin->info.formats.front()->id();
			else
				return ERR_PARAMETER_VALUE_NOT_SUPPORTED;
		}

		// End with (a)buffersink (we have to do a format conversion to the target format 'cos filters might change the pixel format)
		if (nodeInfo->mediaType == MediaType::video)
		{
			nleData->filterConfig += "format=pix_fmts=" + format + ",";
			nleData->filterConfig += "buffersink@" + getFilterId() + ";";
		}
		else if (nodeInfo->mediaType == MediaType::audio)
		{
			nleData->filterConfig += "aformat=sample_fmts=" + format + ",abuffersink@" + getFilterId() + ";";
		}
		else
		{
			BLOG(error) << "Media type '" << boost::describe::enum_to_string<MediaType>(nodeInfo->mediaType, 0) << "' is not supported as buffer sink.";
			
			return ERR_MEDIA_TYPE_NOT_SUPPORTED;
		}

		return BaseNode::preinit(nleTrackIndex, nleData);
	}

	/**
	* Perform the plugin/encoder initialization.
	*/
	int EncoderNode::init()
	{
		BLOG(severity_level::info) << "Initializing " << boost::describe::enum_to_string<MediaType>(nodeInfo->mediaType, 0) << " encoder: " << nodeInfo->data["id"].get<std::string>();

        // Check for plugin instance
		if (plugins.size() < 1)
		{
			BLOG(error) << "The encoder node requires a plugin instance.";
			return ERR_PLUGIN_NOT_SET;
		}

		int ret = ERR_OK;

		// Build filter id
		std::string filterId;
		if (nodeInfo->mediaType == MediaType::video)
			filterId = "buffersink";
		else if (nodeInfo->mediaType == MediaType::audio)
			filterId = "abuffersink";
		else
		{
			BLOG(error) << "Unsupported media type.";

			return ERR_FFMPEG(err);
		}
		filterId += "@" + getFilterId();

		// Initialize plugin
		for (const auto& it : plugins)
		{
			const auto& nleData = data.at(it.first);

			if ((ret = it.second->init(nleData->trackProperties)) < ERR_OK)
			{
				BLOG(error) << "Initializing the encoder plugin failed with error code: " << ret;

				return ret;
			}

			// Get the output filter context
			AVFilterContext* outputCtx;
			if (!(outputCtx = avfilter_graph_get_filter(nleData->filterGraph.get(), filterId.c_str())))
			{
				BLOG(error) << "Unable to find filter by name '" << filterId << "'.";
				return ERR_FILTER_NOT_FOUND;
			}
			outputCtxs.insert(std::make_pair(it.first, outputCtx));

			auto codecCtx = std::static_pointer_cast<EncoderAsset>(it.second)->getCodecContext();

			//codecCtx = encoderAsset->getCodecContext();

			// Update codec context
			switch (nodeInfo->mediaType)
			{
			case MediaType::video:
			{
				// Update codec context (from the filter chain output)
				codecCtx->width = av_buffersink_get_w(outputCtx);
				codecCtx->height = av_buffersink_get_h(outputCtx);
				codecCtx->time_base = av_buffersink_get_time_base(outputCtx);
				codecCtx->framerate = av_inv_q(codecCtx->time_base);
				codecCtx->sample_aspect_ratio = av_buffersink_get_sample_aspect_ratio(outputCtx);
				codecCtx->field_order = AV_FIELD_PROGRESSIVE; // TODO
				codecCtx->pix_fmt = (AVPixelFormat)av_buffersink_get_format(outputCtx);
				//    codecCtx->color_range = (AVColorRange)av_color_range_from_name(std::get<std::string>(properties.at(VoukoderPro::pPropColorRange)).c_str());
				//    codecCtx->colorspace = (AVColorSpace)av_color_space_from_name(std::get<std::string>(properties.at(VoukoderPro::pPropColorSpace)).c_str());
				//    codecCtx->color_primaries = (AVColorPrimaries)av_color_primaries_from_name(std::get<std::string>(properties.at(VoukoderPro::pPropColorPrimaries)).c_str());
				//    codecCtx->color_trc = (AVColorTransferCharacteristic)av_color_transfer_from_name(std::get<std::string>(properties.at(VoukoderPro::pPropColorTransfer)).c_str());

				// Patch track data
				nleData->trackProperties[pPropWidth] = codecCtx->width;
				nleData->trackProperties[pPropHeight] = codecCtx->height;
				nleData->trackProperties[pPropTimebaseNum] = codecCtx->time_base.num;
				nleData->trackProperties[pPropTimebaseDen] = codecCtx->time_base.den;
				nleData->trackProperties[pPropAspectNum] = codecCtx->sample_aspect_ratio.num;
				nleData->trackProperties[pPropAspectDen] = codecCtx->sample_aspect_ratio.den;
				nleData->trackProperties[pPropFormat] = av_get_pix_fmt_name(codecCtx->pix_fmt);
				//if (properties.find(VoukoderPro::pPropColorRange) != properties.end())
				//if (properties.find(VoukoderPro::pPropColorSpace) != properties.end())
				//if (properties.find(VoukoderPro::pPropColorPrimaries) != properties.end())
				//if (properties.find(VoukoderPro::pPropColorTransfer) != properties.end())

				if (codecCtx->pix_fmt == AV_PIX_FMT_CUDA)
				{
					codecCtx->get_format = get_cuda_format;

					av_hwdevice_ctx_create(&hwDeviceCtx, AV_HWDEVICE_TYPE_CUDA, NULL, NULL, 0);

					codecCtx->hw_device_ctx = nullptr;
					codecCtx->hw_frames_ctx = av_buffer_ref(av_buffersink_get_hw_frames_ctx(outputCtx));
				}
				else if (codecCtx->pix_fmt == AV_PIX_FMT_QSV)
				{
					codecCtx->get_format = get_qsv_format;

					av_hwdevice_ctx_create(&hwDeviceCtx, AV_HWDEVICE_TYPE_QSV, NULL, NULL, 0);

					codecCtx->hw_device_ctx = nullptr;
					codecCtx->hw_frames_ctx = av_buffer_ref(av_buffersink_get_hw_frames_ctx(outputCtx));
				}
				break;
			}
			case MediaType::audio:
			{
				// Update codec context (from the filter chain output)
				codecCtx->sample_rate = av_buffersink_get_sample_rate(outputCtx);
				codecCtx->time_base = av_buffersink_get_time_base(outputCtx);
				av_buffersink_get_ch_layout(outputCtx, &codecCtx->ch_layout);
				codecCtx->sample_fmt = (AVSampleFormat)av_buffersink_get_format(outputCtx);

				if (codecCtx->frame_size == 0)
					codecCtx->frame_size = 1024; //TODO
				
				// Patch track data
				nleData->trackProperties[pPropSamplingRate] = codecCtx->sample_rate;
				nleData->trackProperties[pPropTimebaseNum] = codecCtx->time_base.num;
				nleData->trackProperties[pPropTimebaseDen] = codecCtx->time_base.den;
				nleData->trackProperties[pPropFormat] = av_get_sample_fmt_name(codecCtx->sample_fmt);

				char layout[1024];
				av_channel_layout_describe(&codecCtx->ch_layout, layout, sizeof(layout));
				nleData->trackProperties[pPropChannelLayout] = layout;
				nleData->trackProperties[pPropChannelCount] = codecCtx->ch_layout.nb_channels;
				break;
			}
			default:
				BLOG(error) << "Media type '" << boost::describe::enum_to_string<MediaType>(nodeInfo->mediaType, 0) << "' is not supported for filters.";
				return ERR_MEDIA_TYPE_NOT_SUPPORTED;
			}
		}

		frame = std::shared_ptr<AVFrame>(av_frame_alloc(), [](AVFrame* ptr) { av_frame_free(&ptr); });

		return BaseNode::init();
	}

	/**
	* Before opening the encoder register the streams at the muxer.
	*/
	int EncoderNode::preopen()
	{
		int ret = ERR_OK;

		BLOG(severity_level::info) << "Registering " << plugins.size() << " " << boost::describe::enum_to_string<MediaType>(nodeInfo->mediaType, 0) << " stream(s)";

		// Register all streams
		for (const auto& it : plugins)
		{
			auto codecCtx = std::static_pointer_cast<EncoderAsset>(it.second)->getCodecContext();

			for (int i = 0; i < outputs.size(); i++)
			{
				// Mux the packet
				auto muxer = outputs.at(i).lock();
				muxer->registerStream(codecCtx, i, this);
			}
		}

		return BaseNode::preopen();
	}

	/**
	* Finally open the encoder.
	*/
	int EncoderNode::open()
	{
        BLOG(severity_level::info) << "Opening " << boost::describe::enum_to_string<MediaType>(nodeInfo->mediaType, 0) << " encoder plugin '" << nodeInfo->data["id"].get<std::string>() + "' with params '" + nodeParams.dump() + "'";

		int ret = ERR_OK;

		// Open plugin
		for (const auto& it : plugins)
		{
			if ((ret = it.second->open(nodeParams)) < ERR_OK)
			{
				BLOG(error) << "Opening the encoder plugin failed with error code: " << ret;
				return ret;
			}

			// If we have a fixed frame size set it to the buffer sink
			auto codecCtx = std::static_pointer_cast<EncoderAsset>(it.second)->getCodecContext();
			if (codecCtx->codec->type == AVMediaType::AVMEDIA_TYPE_AUDIO && !(codecCtx->codec->capabilities & AV_CODEC_CAP_VARIABLE_FRAME_SIZE))
				av_buffersink_set_frame_size(outputCtxs.at(it.first), codecCtx->frame_size);
		}

		return BaseNode::open();
	}

	/**
	* Close the encoder/plugin again.
	*/
	int EncoderNode::close()
	{
		for (auto& muxer : outputs)
			BLOG(severity_level::info) << "Closing " << boost::describe::enum_to_string<MediaType>(nodeInfo->mediaType, 0) << " encoder '" << nodeInfo->data["id"].get<std::string>() << "'";

		int ret = ERR_OK;

		// Closing plugin
		for (const auto& it : plugins)
		{
			if ((ret = it.second->close()) < ERR_OK)
			{
				BLOG(error) << "Closing the encoder failed with error code: " << ret;
				return ret;
			}
		}

		if (hwDeviceCtx)
			av_buffer_unref(&hwDeviceCtx);
	
		return BaseNode::close();
	}

	/**
	* Checks the filterchain buffersink for new frames. If there are
	* new frames send them to the encoder, followed by the muxer.
	*/
	int EncoderNode::checkFrame(const int nleTrackIndex, bool flush)
	{
		// Handle encoded packets
		const std::function<bool(std::shared_ptr<AVCodecContext>, std::shared_ptr<AVPacket>)> callback = [&](std::shared_ptr<AVCodecContext> codecCtx, std::shared_ptr<AVPacket> packet)
		{
			// We don't need the frame anymore
			av_frame_unref(frame.get());

			// Dispatch to following nodes
			for (int i = 0; i < outputs.size(); i++)
				if (outputs.at(i).lock()->mux(codecCtx, i, packet) < 0)
					break;

			return true;
		};

		// Just flush the encoder
		if (flush)
		{
			for (const auto& it : plugins)
			{
				auto encoderAsset = std::static_pointer_cast<EncoderAsset>(it.second);
				encoderAsset->encode(nullptr, callback);
			}
			return 0;
		}

		int ret = ERR_OK;

		// Find pending frames in the buffersink
		while (true)
		{
			ret = av_buffersink_get_frame(outputCtxs.at(nleTrackIndex), frame.get());

			// Get frame
			if (ret >= 0)
			{
				auto encoderAsset = std::static_pointer_cast<EncoderAsset>(plugins.at(nleTrackIndex));
				if ((ret = encoderAsset->encode(frame, callback)) != ERR_OK)
					break; // TODO?

				// Encode the frame
				if (ret < ERR_OK)
					return ret;
			}
			else if (ret == AVERROR(EAGAIN))
			{
				// Need more input data
				return ERR_OK;
			}
			else
				break;
		}

		return ERR_BUFFER_SINK;
	}
}
