#pragma once

#include <chrono>
#include <boost/describe/enum.hpp>
#include <boost/describe/enum_to_string.hpp>

extern "C" {
#include <libavfilter/avfilter.h>
#include <libavcodec/avcodec.h>
}
#include "../PluginInterface/plugin_api.h"
#include "voukoderpro_api.h"
#include "Logger.h"
#include "Errors.h"
#include "Performance.h"

#pragma warning(disable : 4996)

namespace VoukoderPro
{
	BOOST_DEFINE_ENUM_CLASS(NodeState, Uninitialized, Preinitialized, Initialized, Open, Closed)

	struct NodeData
	{
		std::string filterConfig;                                 // per NLE tack
		std::shared_ptr<AVFilterGraph> filterGraph;               // per NLE track
		std::unique_ptr<Performance> performance = nullptr;       // ignored
		configType properties;                                    //
		configType trackProperties;                               //
	};
	
	class BaseNode : public std::enable_shared_from_this<BaseNode>
	{
	public:
		BaseNode(std::shared_ptr<NodeInfo> nodeInfo):
			nodeInfo(nodeInfo)
		{
			// Get the node id
			if (nodeInfo->data.contains("id") && nodeInfo->data["id"].is_string())
				nodeId = nodeInfo->data.at("id").get<std::string>();

			// Get the node params
			if (nodeInfo->data.contains("params") && nodeInfo->data["params"].is_object())
				nodeParams = nodeInfo->data["params"];
		}

		virtual int preinit(const int nleTrackIndex, std::shared_ptr<NodeData> nleData)
		{
			int ret = ERR_OK;

			// Dispatch preinit
			for (auto& output : outputs)
				if ((ret = output.lock()->preinit(nleTrackIndex, nleData)) < ERR_OK)
					break;

			if (nleTrackIndex == data.size() - 1)
				state = NodeState::Preinitialized;

			return ret;
		}

		virtual int init()
		{
			int ret = ERR_OK;

			state = NodeState::Initialized;

			// Dispatch init
			for (auto& output : outputs)
			{
				auto node = output.lock();

				// Initialize
				if (node->state < NodeState::Initialized)
				{
					// Copy the node data
					node->data = data;

					if ((ret = node->init()) < ERR_OK)
						break;
				}
			}

			return ret;
		}

		virtual int preopen()
		{
			int ret = ERR_OK;

			// Dispatch preinit
			for (auto& output : outputs)
				if ((ret = output.lock()->preopen()) < ERR_OK)
					break;

			return ret;
		}

		virtual int open()
		{
			int ret = ERR_OK;

			state = NodeState::Open;

			// Dispatch to following nodes
			for (auto& output : outputs)
			{
				auto node = output.lock();
				
				// Initialize
				if (node->state == NodeState::Initialized)
					if (allInputsHaveState(node, NodeState::Open))
						if ((ret = node->open()) < ERR_OK)
							break;
			}

			return ret;
		}

		virtual int preclose()
		{
			int ret = ERR_OK;

			// Dispatch preinit
			for (auto& output : outputs)
				if ((ret = output.lock()->preclose()) < ERR_OK)
					break;

			return ret;
		}

		virtual int close()
		{
			int ret = ERR_OK;

			state = NodeState::Closed;

			// Dispatch to following nodes
			for (auto& output : outputs)
			{
				auto node = output.lock();

				if (node->state == NodeState::Open)
				{
					// Initialize
					if (allInputsHaveState(node, NodeState::Closed))
                    {
                        if ((ret = node->close()) < ERR_OK)
							break;
                        else
							node->state = NodeState::Closed;
                    }
				}
			}

			return ret;
		}

		virtual std::shared_ptr<NodeData> nodeData(const int nleTrackIndex)
		{
			return data.find(nleTrackIndex) == data.end() ? nullptr : data.at(nleTrackIndex);
		}

		virtual int checkFrame(const int nleTrackIndex, bool flush = false)
		{
			int ret = ERR_OK;

			for (auto& output : outputs)
				if ((ret = output.lock()->checkFrame(nleTrackIndex, flush)) < ERR_OK)
					break;

			return ret;
		}

		virtual int registerStream(std::shared_ptr<AVCodecContext> codecCtx, const int index, BaseNode* pEncoderNode)
		{
			int ret = ERR_OK;

			for (auto& output : outputs)
				if ((ret = output.lock()->registerStream(codecCtx, index, pEncoderNode)) < ERR_OK)
					break;

			return ret;
		}

		virtual int mux(std::shared_ptr<AVCodecContext> codecCtx, const int index, std::shared_ptr<AVPacket> packet)
		{
			int ret = ERR_OK;

			for (auto& output : outputs)
				if ((ret = output.lock()->mux(codecCtx, index, packet)) < ERR_OK)
					break;

			return ret;
		}

		std::string getFilterId()
		{
			std::string filterId = nodeInfo->id;
			filterId.erase(std::remove(filterId.begin(), filterId.end(), '-'), filterId.end());

			return filterId;
		}

		std::string getSplitName()
		{
			if (nodeInfo->mediaType == MediaType::video)
				return "split";
			else if (nodeInfo->mediaType == MediaType::audio)
				return "asplit";
			
			return "";
		}

	public:
		std::shared_ptr<NodeInfo> nodeInfo;
		std::string nodeId;
		nlohmann::ordered_json nodeParams;
		NodeState state = NodeState::Uninitialized;
		std::vector<std::weak_ptr<BaseNode>> inputs;
		std::vector<std::weak_ptr<BaseNode>> outputs;
		std::map<int, std::shared_ptr<Asset>> plugins;
		std::map<int, std::shared_ptr<NodeData>> data;

	private:
		bool allInputsHaveState(std::shared_ptr<BaseNode> target, NodeState state)
		{
			// Check all inputs
			for (auto& input : target->inputs)
			{
				auto node = input.lock();

				if (node->state != NodeState::Uninitialized && node->state < state)
					return false;
			}

			return true;
		}

	protected:
		int indent = 0;
	};
}
