#include "plugin.h"

namespace VoukoderPro
{
    FFmpegMuxerPlugin::FFmpegMuxerPlugin()
    {
        void* opaque = NULL;
        const AVOutputFormat* format = NULL;
        while ((format = av_muxer_iterate(&opaque)))
        {
            // Has this muxer already been registered?
            if (std::find_if(std::begin(infos), std::end(infos), [&](AssetInfo info) -> bool
                {
                    return info.name == format->name;
                }) != infos.end())
                continue;

            AssetInfo info;
            info.id = format->name;
            info.name = format->name;
            info.description = format->long_name;
            info.type = NodeInfoType::muxer;
            info.mediaType = MediaType::mux;
            info.category = std::make_pair("ffmpeg", "FFmpeg");
            info.helpUrl = "https://ffmpeg.org/ffmpeg-formats.html#" + info.id;

            // Iterate over filter options
            addPrivateOptions(info, format->priv_class);

            // Find the allowed codecs for this muxer
            void* opaque2 = NULL;
            const AVCodec* codec = NULL;
            while ((codec = av_codec_iterate(&opaque2)))
            {
                if (codec->id != AV_CODEC_ID_NONE &&
                    std::find(info.allowedInputGroups.begin(), info.allowedInputGroups.end(), codec->id) == info.allowedInputGroups.end() && // Is codec already in list?
                    avformat_query_codec(format, codec->id, FF_COMPLIANCE_NORMAL) == 1)  // Is codec supported?
                    info.allowedInputGroups.push_back(codec->id);
            }

            registerAsset(info);
        }
    }
}
