#include "plugin.h"

#include <boost/algorithm/string/predicate.hpp>

namespace VoukoderPro
{
    FFmpegEncodersPlugin::FFmpegEncodersPlugin() : EncoderPlugin()
    {
        const AVCodec* c = avcodec_find_encoder_by_name("prores_ks");
        AVCodecContext* codecContext = avcodec_alloc_context3(c);
        AVDictionary* options = NULL;
        const AVOption* opt = NULL;
        void* obj = NULL;

        while ((opt = av_opt_next(codecContext, opt))) {
            if (opt->type != AV_OPT_TYPE_CONST && opt->flags & AV_OPT_FLAG_ENCODING_PARAM)
                printf("Option: %s\n", opt->name);
        }




        void* opaque = NULL;

        const AVCodec* codec = NULL;
        while ((codec = av_codec_iterate(&opaque)))
        {
            // Has this encoder already been registered?
            if (codec->id == AV_CODEC_ID_NONE || 
                std::find_if(std::begin(infos), std::end(infos), [&](AssetInfo info) -> bool
                {
                    return info.name == codec->name;
                }) != infos.end())
                continue;

            AssetInfo info;
            info.id = codec->name;
            info.name = codec->name;
            info.description = codec->long_name;
            info.type = NodeInfoType::encoder;
            info.helpUrl = "https://ffmpeg.org/ffmpeg-encoders.html#" + info.id;

            // Set the right category
            if (boost::algorithm::ends_with(codec->name, "_amf"))
                info.category = std::make_pair("amd", "AMD");
            else if (boost::algorithm::ends_with(codec->name, "_nvenc"))
                info.category = std::make_pair("nvidia", "Nvidia");
            else if (boost::algorithm::ends_with(codec->name, "_qsv"))
                info.category = std::make_pair("intel", "Intel");
            else
                info.category = std::make_pair("ffmpeg", "FFmpeg");

            // Add some global codec options
            auto& basic = info.group("basic", "Basic", ItemParamGroupType::Forced);
            auto& rcm = basic.param<std::string>("_strategy", "Rate control mode").ignore(true)
                .description("");

            basic.param<double>("qcomp", "Value", 1)
                .description("Set video quantizer scale compression (VBR). It is used as a constant in the ratecontrol equation.")
                .minValue(0)
                .maxValue(1)
                .defaultValue(0.3);

            basic.param<int>("qmin", "Minimum", 1)
                .description("Set min video quantizer scale (VBR).")
                .minValue(-1)
                .maxValue(69)
                .defaultValue(2);

            basic.param<int>("qmax", "Maximum", 1)
                .description("Set max video quantizer scale (VBR).")
                .minValue(-1)
                .maxValue(1024)
                .defaultValue(31);

            basic.param<int>("b", "Value [kbit/s]", 1)
                .description("The data rate allowed by the encoder.")
                .minValue(0)
                .maxValue(INT_MAX)
                .multiplierValue(1000)
                .defaultValue(15000);

            basic.param<int>("minrate", "Minimum [kbit/s]", 1)
                .description("Higher value can improve maximum quality, but increases decoder requirements.")
                .minValue(0)
                .maxValue(INT_MAX)
                .multiplierValue(1000)
                .defaultValue(15000);

            basic.param<int>("maxrate", "Maximum [kbit/s]", 1)
                .description("Higher value can improve maximum quality, but increases decoder requirements.")
                .minValue(0)
                .maxValue(INT_MAX)
                .multiplierValue(1000)
                .defaultValue(15000);

            basic.param<int>("bufsize", "Buffer Size [kbits]", 1)
                .description("The encoder bitstream buffer size.")
                .minValue(0)
                .maxValue(INT_MAX)
                .multiplierValue(1000)
                .defaultValue(15000);

            // Set media type specific data
            if (codec->type == AVMEDIA_TYPE_VIDEO && codec->pix_fmts)
            {
                info.mediaType = MediaType::video;

                const enum AVPixelFormat* p;
                for (p = codec->pix_fmts; *p != -1; p++)
                {
                    const char* pix_fmt_name = av_get_pix_fmt_name(*p);
                    if (pix_fmt_name) {
                        info.format(pix_fmt_name, pix_fmt_name);
                    }
                }

                rcm.option("Quantizer", "quantizer", [](const ItemParamAction& action) {
                    action.setVisible("basic.qcomp", true);
                    action.setVisible("basic.qmin", true);
                    action.setVisible("basic.qmax", true);
                    action.setVisible("basic.b", false);
                    action.setVisible("basic.minrate", false);
                    action.setVisible("basic.maxrate", false);
                    action.setVisible("basic.bufsize", false);
                })
                .option("Bitrate", "bitrate", [](const ItemParamAction& action) {
                    action.setVisible("basic.qcomp", false);
                    action.setVisible("basic.qmin", false);
                    action.setVisible("basic.qmax", false);
                    action.setVisible("basic.b", true);
                    action.setVisible("basic.minrate", true);
                    action.setVisible("basic.maxrate", true);
                    action.setVisible("basic.bufsize", true);
                })
                .defaultValue("constqp");
            }
            else if (codec->type == AVMEDIA_TYPE_AUDIO && codec->sample_fmts)
            {
                info.mediaType = MediaType::audio;

                const enum AVSampleFormat* p;
                for (p = codec->sample_fmts; *p != -1; p++)
                {
                    const char* sample_fmt_name = av_get_sample_fmt_name(*p);
                    if (sample_fmt_name) {
                        info.format(sample_fmt_name, sample_fmt_name);
                    }
                }

                rcm.option("Bitrate", "bitrate", [](const ItemParamAction& action) {
                    action.setVisible("basic.qcomp", false);
                    action.setVisible("basic.qmin", false);
                    action.setVisible("basic.qmax", false);
                    action.setVisible("basic.b", true);
                    action.setVisible("basic.minrate", true);
                    action.setVisible("basic.maxrate", true);
                    action.setVisible("basic.bufsize", true);
                })
                .defaultValue("bitrate");
            }
            else
                continue;

            // Iterate over filter options
            createFFmpegParameters(info, codec->priv_class);

            registerAsset(info);
        }
    }
}
