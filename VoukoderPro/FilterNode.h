#pragma once

#include "BaseNode.h"

namespace VoukoderPro
{
    class FilterNode : public BaseNode
    {
    public:
        FilterNode(std::shared_ptr<NodeInfo> nodeInfo);
        int preinit(const int nleTrackIndex, std::shared_ptr<NodeData> nleData);
    };
}