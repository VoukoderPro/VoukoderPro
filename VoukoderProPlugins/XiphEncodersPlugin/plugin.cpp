#include "plugin.h"

namespace VoukoderPro
{
    XiphEncoderPlugin::XiphEncoderPlugin(): EncoderPlugin()
    {
        registerOpus();
        registerVorbis();
    }

    int XiphEncoderPlugin::registerOpus()
    {
        AssetInfo info;
        info.id = "libopus";
        info.name = "Opus";
        info.category = std::make_pair("xiph", "Xiph.org");
        info.description = "The xiph.org opus encoder";
        info.type = NodeInfoType::encoder;
        info.mediaType = MediaType::audio;
        info.helpUrl = "https://ffmpeg.org/ffmpeg-codecs.html#" + info.id;

        info.format("16 bit", "s16");

        auto& standard = info.group("standard", "Standard", ItemParamGroupType::Forced);
        standard.param<std::string>("vbr", "Strategy")
            .description("The encoding strategy to use.")
            .option("Variable Bitrate (VBR)", "on")
            .option("Constant Bitrate (CBR)", "off")
            .option("Constrained Quality (CQ)", "constrained")
            .defaultValue("on");

        standard.param<int>("b", "Bitrate")
            .description("In VBR mode the bitrate may go up and down freely depending on the content to achieve more consistent quality.")
            .option("16 kbit/s", 16000)
            .option("32 kbit/s", 32000)
            .option("64 kbit/s", 64000)
            .option("96 kbit/s", 96000)
            .option("128 kbit/s", 128000)
            .option("192 kbit/s", 192000)
            .option("256 kbit/s", 256000)
            .option("320 kbit/s", 320000)
            .option("384 kbit/s", 384000)
            .option("448 kbit/s", 448000)
            .option("512 kbit/s", 512000)
            .defaultValue(96000);

        return registerAsset(info);
    }

    int XiphEncoderPlugin::registerVorbis()
    {
        AssetInfo info;
        info.id = "libvorbis";
        info.name = "Vorbis";
        info.category = std::make_pair("xiph", "Xiph.org");
        info.description = "The xiph.org vorbis encoder";
        info.type = NodeInfoType::encoder;
        info.mediaType = MediaType::audio;
        info.helpUrl = "https://ffmpeg.org/ffmpeg-codecs.html#" + info.id;

        info.format("Floating Point", "fltp");

        auto& basic = info.group("basic", "Basic", ItemParamGroupType::Forced);
        basic.param<std::string>("rc", "Rate control").ignore(true)
            .description("The selected rate control mechanism.")
            .option("Constant bit rate", "cbr", [](const ItemParamAction& action) {
                action.setVisible("basic.b", true);
                action.setVisible("basic.qp", false);
            })
            .option("Variable bit rate", "vbr", [](const ItemParamAction& action) {
                action.setVisible("basic.b", false);
                action.setVisible("basic.qp", true);
            })
            .defaultValue("qp");

        basic.param<int>("b", "Constant bit rate", 1)
            .description("The constant bit rate in kbit/s for all channels.")
            .option("64 kbit/s", 64000)
            .option("96 kbit/s", 96000)
            .option("128 kbit/s", 128000)
            .option("192 kbit/s", 192000)
            .option("256 kbit/s", 256000)
            .option("320 kbit/s", 320000)
            .option("384 kbit/s", 384000)
            .option("448 kbit/s", 448000)
            .option("512 kbit/s", 512000)
            .defaultValue(96000);

        basic.param<int>("qp", "Variable bit rate", 1)
            .description("The constant bit rate in kbit/s for all channels.")
            .option("ca. 64 kbit/s", 0)
            .option("ca. 80 kbit/s", 1)
            .option("ca. 96 kbit/s", 2)
            .option("ca. 112 kbit/s", 3)
            .option("ca. 128 kbit/s", 4)
            .option("ca. 160 kbit/s", 5)
            .option("ca. 192 kbit/s", 6)
            .option("ca. 224 kbit/s", 7)
            .option("ca. 256 kbit/s", 8)
            .option("ca. 320 kbit/s", 9)
            .option("ca. 500 kbit/s", 10)
            .defaultValue(2);

        return registerAsset(info);
    }
}
