#include "InputNode.h"

namespace VoukoderPro
{
	InputNode::InputNode(std::shared_ptr<NodeInfo> nodeInfo):
		BaseNode(nodeInfo)
	{}

	/**
	* Builds the filter chain for this input > filter > encoder chain.
	*/
	int InputNode::preinit()
	{
		int ret = ERR_OK;

		for (const auto& [nleTrackIndex, nleData] : data)
		{
			std::stringstream filterConfigBuffer;

			if (nodeInfo->mediaType == MediaType::video)
			{
				filterConfigBuffer << "buffer@" << getFilterId() << "=";
				filterConfigBuffer << "width=" << std::get<int>(nleData->trackProperties.at(pPropWidth)) << ":";
				filterConfigBuffer << "height=" << std::get<int>(nleData->trackProperties.at(pPropHeight)) << ":";
				filterConfigBuffer << "pix_fmt=" << std::get<std::string>(nleData->trackProperties.at(pPropFormat)) << ":";
				filterConfigBuffer << "time_base=" << std::get<int>(nleData->trackProperties.at(pPropTimebaseNum)) << "/" << std::get<int>(nleData->trackProperties.at(pPropTimebaseDen)) << ":";
				filterConfigBuffer << "pixel_aspect=" << std::get<int>(nleData->trackProperties.at(pPropAspectNum)) << "/" << std::get<int>(nleData->trackProperties.at(pPropAspectDen));
			}
			else if (nodeInfo->mediaType == MediaType::audio)
			{
				filterConfigBuffer << "abuffer@" << getFilterId() << "=";
				filterConfigBuffer << "sample_rate=" << std::get<int>(nleData->trackProperties.at(pPropSamplingRate)) << ":";
				filterConfigBuffer << "sample_fmt=" << std::get<std::string>(nleData->trackProperties.at(pPropFormat)) << ":";
				filterConfigBuffer << "channel_layout=" << std::get<std::string>(nleData->trackProperties.at(pPropChannelLayout));
			}
			else
				return ERR_MEDIA_TYPE_NOT_SUPPORTED;

			nleData->filterConfig += filterConfigBuffer.str();

			// Insert splits if necessary
			if (outputs.size() > 1)
			{
				nleData->filterConfig += "[" + getFilterId() + "];[" + getFilterId() + "]" + getSplitName() + "=" + std::to_string(outputs.size());

				for (auto& output : outputs)
					nleData->filterConfig += "[split_" + output.lock()->getFilterId() + "]";

				nleData->filterConfig += ";";
			}
			else
				nleData->filterConfig += ",";

			// Do we have a color range set?
			std::string scale = "";
			if (nleData->trackProperties.find(pPropColorRange) != nleData->trackProperties.end())
			{
				const std::string range = std::get<std::string>(nleData->trackProperties.at(pPropColorRange));
				scale += "scale=in_range=" + range + ":out_range=" + range + ","; // TODO: Don't know why this is necessary to keep the color range as it is
			}

			// Process the output nodes
			for (auto& output : outputs)
			{
				auto node = output.lock();

				// Handle splits if present
				if (outputs.size() > 1)
					nleData->filterConfig += "[split_" + node->getFilterId() + "]" + scale;

				// Call subnodes
				if ((ret = node->preinit(nleTrackIndex, nleData)) < ERR_OK)
					break;
			}

			// Remove the trailing ;
			nleData->filterConfig.resize(nleData->filterConfig.size() - 1);

			BLOG(debug) << "Filter config: " << nleData->filterConfig;
		}

		return ret;
	}

	/**
	* Initialize the filter graph from the filter chain string.
	*/
	int InputNode::init()
	{
		int ret = ERR_OK;

		// Build filter id
		std::string filterId;
		if (nodeInfo->mediaType == MediaType::video)
			filterId = "buffer";
		else if (nodeInfo->mediaType == MediaType::audio)
			filterId = "abuffer";
		else
		{
			BLOG(error) << "Unsupported media type.";

			return ERR_FFMPEG(err);
		}
		filterId += "@" + getFilterId();

		for (const auto& [nleTrackIndex, nleData] : data)
		{
			std::shared_ptr<AVFilterGraph> filterGraph;

			// Allocate filter graph
			if (!(filterGraph = std::shared_ptr<AVFilterGraph>(avfilter_graph_alloc(), [](AVFilterGraph* ptr) { avfilter_graph_free(&ptr); })))
			{
				BLOG(error) << "Unable to allocate filter graph.";

				return -1;
			}

			//avfilter_graph_set_auto_convert(data->filterGraph.get(), AVFILTER_AUTO_CONVERT_NONE);

			// Parse the filter graph
			AVFilterInOut* filterInputs, * filterOutputs;
			if ((ret = avfilter_graph_parse2(filterGraph.get(), nleData->filterConfig.c_str(), &filterInputs, &filterOutputs)) < 0)
			{
				BLOG(error) << "Unable to parse filter graph.";

				return ERR_FFMPEG(err);
			}

			// Configure filter graph
			if ((ret = avfilter_graph_config(filterGraph.get(), NULL)) < 0)
			{
				BLOG(error) << "Unable to configure filter graph.";

				return ERR_FFMPEG(err);
			}

			// Get the input buffer filter context
			AVFilterContext* inputCtx;
			if (!(inputCtx = avfilter_graph_get_filter(filterGraph.get(), filterId.c_str())))
			{
				BLOG(error) << "Unable to find filter by name '" << filterId << "'.";

				return ERR_FILTER_NOT_FOUND;
			}
			inputCtxs.insert(std::make_pair(nleTrackIndex, inputCtx));

			nleData->filterGraph = filterGraph;

			BLOG(debug) << "Dumping filter graphfor NLE Track #" << nleTrackIndex << ":\n" << avfilter_graph_dump(nleData->filterGraph.get(), NULL);
		}

        return BaseNode::init();
	}

	/**
	* Flush all encoders before closing them.
	*/
	int InputNode::preclose()
	{
		BLOG(severity_level::info) << "Flushing " << boost::describe::enum_to_string<MediaType>(nodeInfo->mediaType, 0) << " encoders ...";

		int ret = ERR_OK;

		// Flush the encoder(s)
		for (const auto& it : data)
			if ((ret = sendFrame(it.first, nullptr)) < 0)
				return ret;

		return BaseNode::preclose();
	}

	/**
	* Close all encoders.
	*/
	int InputNode::close()
	{
		BLOG(severity_level::info) << "Closing " << boost::describe::enum_to_string<MediaType>(nodeInfo->mediaType, 0) << " encoders ...";

		return BaseNode::close();
	}

	/**
	* Accepts a frame an sends it to the filter chain.
	*/
	int InputNode::sendFrame(const int nleTrackIndex, std::shared_ptr<AVFrame> frame)
	{
		// Apply color format settings
		if (frame && nodeInfo->mediaType == MediaType::video &&
			nodeInfo->data.contains("color"))
		{
			const auto& color = nodeInfo->data["color"];

			if (color.contains("range"))
			{
				const std::string colorRange = color["range"].get<std::string>();
				if (colorRange != "auto")
					frame->color_range = (AVColorRange)av_color_range_from_name(colorRange.c_str());
			}
			if (color.contains("matrix"))
			{
				const std::string colorSpace = color["matrix"].get<std::string>();
				if (colorSpace != "auto")
					frame->colorspace = (AVColorSpace)av_color_space_from_name(colorSpace.c_str());
			}
			if (color.contains("primaries"))
			{
				const std::string colorPrimaries = color["primaries"].get<std::string>();
				if (colorPrimaries != "auto")
					frame->color_primaries = (AVColorPrimaries)av_color_primaries_from_name(colorPrimaries.c_str());
			}
			if (color.contains("transfer"))
			{
				const std::string colorTransfer = color["transfer"].get<std::string>();
				if (colorTransfer != "auto")
					frame->color_trc = (AVColorTransferCharacteristic)av_color_transfer_from_name(colorTransfer.c_str());
			}
		}

		// If it's just about flushing the encoder take a shortcut
		if (!frame)
			return BaseNode::checkFrame(nleTrackIndex, true);

		//data->performance->start("filterIn");

		// Send frame to filters
		if (av_buffersrc_write_frame(inputCtxs.at(nleTrackIndex), frame.get()) < 0)
		{
			//data->performance->end();

			return ERR_FAIL;
		}

		// Workaround/Hack: av_buffersrc_write_frame sets the old channel layout to active
		if (frame)
			frame->channel_layout = 0;

		int ret = BaseNode::checkFrame(nleTrackIndex);

		//data->performance->end();

		return ret;
	}

	/**
	* Sets the node data for this input.
	*/
	void InputNode::setNodeData(const int nleTrackIndex, std::shared_ptr<NodeData> nleData)
	{
		data.insert(std::make_pair(nleTrackIndex, nleData));
	}
}
