#pragma once

#include "BaseNode.h"

namespace VoukoderPro
{
    class PostProcNode : public BaseNode
    {
    public:
        PostProcNode(std::shared_ptr<NodeInfo> nodeInfo);
        int preinit(const int nleTrackIndex, std::shared_ptr<NodeData> nleData);
        int init();
        int open();
        int close();

    private:
        configType params;
    };
}
