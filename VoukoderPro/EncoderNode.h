#pragma once

#include "BaseNode.h"

namespace VoukoderPro
{
    class EncoderNode : public BaseNode
    {
    public:
        EncoderNode(std::shared_ptr<NodeInfo> nodeInfo);
        int preinit(const int nleTrackIndex, std::shared_ptr<NodeData> nleData);
        int init();
        int preopen();
        int open();
        int close();
        int checkFrame(const int nleTrackIndex, bool flush);

    private:
        AVBufferRef* hwDeviceCtx = nullptr;
        std::shared_ptr<AVFrame> frame = nullptr;
        std::map<int, AVFilterContext*> outputCtxs;
    };
}