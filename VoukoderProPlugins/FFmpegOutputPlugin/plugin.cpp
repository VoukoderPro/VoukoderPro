#include "plugin.h"

namespace VoukoderPro
{
    FFmpegOutputsPlugin::FFmpegOutputsPlugin(): OutputPlugin()
    {
        void* opaque = NULL;
        const char* protocol = NULL;
        while ((protocol = avio_enum_protocols(&opaque, 1)))
        {
            // Has this encoder already been registered?
            if (std::find_if(std::begin(infos), std::end(infos), [&](AssetInfo info) -> bool
                {
                    return info.id == protocol;
                }) != infos.end())
                continue;

            AssetInfo info;
            info.id = protocol;
            info.name = boost::to_upper_copy(info.id);
            info.category = std::make_pair("ffmpeg", "FFmpeg");
            info.description = protocol;
            info.type = NodeInfoType::output;
            info.mediaType = MediaType::mux;
            info.helpUrl = "https://ffmpeg.org/ffmpeg-protocols.html#" + info.id;

            ItemParamGroup& global = info.group("global", "Global", ItemParamGroupType::Forced);
            global.param<std::string>("_host", "Host / IP").ignore(true)
                .description("Defines the basic host information")
                .defaultValue("127.0.0.1");

            global.param<int>("_port", "Port").ignore(true)
                .description("Defines the basic port information")
                .minValue(0)
                .maxValue(65535)
                .defaultValue(1234);

            // Iterate over encoder options
            const AVClass* cls = avio_protocol_get_class(protocol);
            if (cls != nullptr)
            {
                // Render private options
                addPrivateOptions(info, cls);

                // Register non-blacklisted protocols
                std::vector<std::string> blacklisted = { "crypto", "fd", "ffrtmpcrypt", "ffrtmphttp", "ftp", "gopher", "gophers", "httpproxy", "md5", "pipe", "tee", "tls", "udplite" };
                if (std::find(blacklisted.begin(), blacklisted.end(), info.id) == blacklisted.end())
                    registerAsset(info);
            }
        }
    }
}
