#include "plugin.h"

namespace VoukoderPro
{
    LameEncodersPlugin::LameEncodersPlugin(): EncoderPlugin()
    {
        AssetInfo info;
        info.id = "libmp3lame";
        info.name = "MP3";
        info.category = std::make_pair("lame", "LAME");
        info.description = "The LAME MP3 encoder";
        info.type = NodeInfoType::encoder;
        info.mediaType = MediaType::audio;
        info.helpUrl = "https://ffmpeg.org/ffmpeg-codecs.html#" + info.id;

        info.format("16 bit", "s16p")
            .format("32 bit", "s32p")
            .format("Float", "fltp");

        auto& basic = info.group("basic", "Basic", ItemParamGroupType::Forced);
        basic.param<int>("b", "Bit rate")
            .description("The constant bit rate in kbit/s for all channels.")
            .option("32 kbit/s", 32000)
            .option("40 kbit/s", 40000)
            .option("48 kbit/s", 48000)
            .option("64 kbit/s", 64000)
            .option("80 kbit/s", 80000)
            .option("96 kbit/s", 96000)
            .option("112 kbit/s", 112000)
            .option("128 kbit/s", 128000)
            .option("160 kbit/s", 160000)
            .option("192 kbit/s", 192000)
            .option("224 kbit/s", 224000)
            .option("256 kbit/s", 256000)
            .option("320 kbit/s", 320000)
            .defaultValue(96000);

        basic.param<bool>("abr", "Force M/S stereo coding")
            .description("Force M/S stereo coding")
            .defaultValue(false);

        auto& advanced = info.group("advanced", "Advanced", ItemParamGroupType::Standard);
        advanced.param<bool>("reservoir", "Use bit reservoir")
            .description("Use bit reservoir")
            .defaultValue(true);

        advanced.param<bool>("joint_stereo", "Use joint stereo")
            .description("Use joint stereo")
            .defaultValue(true);

        registerAsset(info);
    }
}
