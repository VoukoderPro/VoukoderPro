#pragma once

#include "BaseNode.h"

namespace VoukoderPro
{
	class InputNode : public BaseNode
	{
	public:
		InputNode(std::shared_ptr<NodeInfo> nodeInfo);
		int preinit();
		int init();
		int preclose();
		int close();
		int sendFrame(const int nleTrackIndex, std::shared_ptr<AVFrame> frame);
		void setNodeData(const int nleTrackIndex, std::shared_ptr<NodeData> nodeData);

	private:
		std::map<int, AVFilterContext*> inputCtxs;
		std::map<int, std::shared_ptr<AVFilterGraph>> filterGraphs;
	};
}