#pragma once

#include "BaseNode.h"

namespace VoukoderPro
{
    class MuxerNode : public BaseNode
    {
    public:
        MuxerNode(std::shared_ptr<NodeInfo> nodeInfo);
        int preinit(const int nleTrackIndex, std::shared_ptr<NodeData> nleData);
    };
}