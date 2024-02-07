#pragma once

#include "BaseNode.h"
#include "EncoderNode.h"
#include "../3rdparty/duktape-2.7.0/dist/duktape.h"

namespace VoukoderPro
{
	class OutputNode : public BaseNode
	{
    public:
        OutputNode(std::shared_ptr<NodeInfo> nodeInfo);
        int preinit(const int nleTrackIndex, std::shared_ptr<NodeData> data);
        int init();
        int open();
        int close();
        int registerStream(std::shared_ptr<AVCodecContext> codecCtx, const int index, BaseNode* pEncoderNode);
        int mux(std::shared_ptr<AVCodecContext> codecCtx, const int index, std::shared_ptr<AVPacket> packet);

    private:
        int handleSideData(AVStream* stream, std::shared_ptr<NodeInfo> nodeInfo);
        int injectStereoData(AVStream* stream, nlohmann::ordered_json& props);
        int injectSphericalData(AVStream* stream, nlohmann::ordered_json& props);
        int injectMasteringDisplayData(AVStream* stream, nlohmann::ordered_json& props);
        int injectContentLightLevels(AVStream* stream, nlohmann::ordered_json& props);

    private:
        std::shared_ptr<AVFormatContext> formatCtx = std::shared_ptr<AVFormatContext>(avformat_alloc_context(), avformat_free_context);
        std::map<AVStream*, std::pair<std::shared_ptr<AVCodecContext>, int>> streams;
	};
}
