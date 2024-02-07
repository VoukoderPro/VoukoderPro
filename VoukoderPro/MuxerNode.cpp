#include "MuxerNode.h"

namespace VoukoderPro
{
	MuxerNode::MuxerNode(std::shared_ptr<NodeInfo> nodeInfo):
		BaseNode(nodeInfo)
	{}

	int MuxerNode::preinit(const int nleTrackIndex, std::shared_ptr<NodeData> nleData)
	{
		int ret = ERR_OK;

		state = NodeState::Preinitialized;

		// Dispatch init
		for (auto& output : outputs)
		{
			auto node = output.lock();

			// Initialize
			if (node->state == NodeState::Uninitialized)
			{
				// Copy the node data
				node->data = data;

				if ((ret = node->preinit(nleTrackIndex, nleData)) < ERR_OK)
					break;
			}
		}

		return ret;
	}
}