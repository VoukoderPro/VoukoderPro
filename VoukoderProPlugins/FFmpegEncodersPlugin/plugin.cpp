#include "plugin.h"

#include <boost/algorithm/string/predicate.hpp>

namespace VoukoderPro
{
    FFmpegEncodersPlugin::FFmpegEncodersPlugin() : EncoderPlugin()
    {
        void* opaque = NULL;

        const AVCodec* codec = NULL;
        while ((codec = av_codec_iterate(&opaque)))
        {
            // Has this encoder already been registered?
            if (std::find_if(std::begin(infos), std::end(infos), [&](AssetInfo info) -> bool
                {
                    return info.name == codec->name;
                }) != infos.end())
                continue;

            // Do we want to skip this codec?
            if (codec->id == AV_CODEC_ID_NONE || codec->id == AV_CODEC_ID_ANULL || codec->id == AV_CODEC_ID_VNULL ||
                !av_codec_is_encoder(codec))
                continue;

            // Hide (double/planar) audio encoders
            if (boost::algorithm::ends_with(codec->name, "_planar"))
                continue;

            AssetInfo info;
            info.id = codec->name;
            info.name = codec->name;
            info.description = codec->long_name;
            info.type = NodeInfoType::encoder;
            info.helpUrl = "https://ffmpeg.org/ffmpeg-codecs.html#" + info.id;

            // Set the right category
            if (boost::algorithm::ends_with(codec->name, "_amf"))
            {
                info.category = std::make_pair("amd", "AMD");
                boost::algorithm::replace_all(info.name, "_amf", "");
            }
            else if (boost::algorithm::ends_with(codec->name, "_nvenc"))
            {
                info.category = std::make_pair("nvidia", "Nvidia");
                boost::algorithm::replace_all(info.name, "_nvenc", "");
            }
            else if (boost::algorithm::ends_with(codec->name, "_qsv"))
            {
                info.category = std::make_pair("intel", "Intel");
                boost::algorithm::replace_all(info.name, "_qsv", "");
            }
            else if (boost::algorithm::ends_with(codec->name, "_mf"))
            {
                info.category = std::make_pair("msmf", "Microsoft Media Foundation");
                boost::algorithm::replace_all(info.name, "_mf", "");
            }
            else if (boost::algorithm::ends_with(codec->name, "_vaapi"))
            {
                info.category = std::make_pair("vaapi", "VA-API");
                boost::algorithm::replace_all(info.name, "_vaapi", "");
            }
            else if (boost::algorithm::starts_with(codec->name, "pcm_"))
            {
                info.category = std::make_pair("pcm", "PCM (Uncompressed)");
                boost::algorithm::replace_all(info.name, "pcm_", "");
            }
            else if (boost::algorithm::starts_with(codec->name, "adpcm_"))
            {
                info.category = std::make_pair("adpcm", "ADPCM (Adaptive Differencial PCM)");
                boost::algorithm::replace_all(info.name, "adpcm_", "");
            }
            else
                info.category = std::make_pair("ffmpeg", "General");

            boost::to_upper(info.name);

            // Mark experimental encoders as such
            if (codec->capabilities & AV_CODEC_CAP_EXPERIMENTAL)
                info.name += " (Experimental)";

            AVCodecContext* codecCtx = avcodec_alloc_context3(codec);

            // Set media type specific data
            if (codec->type == AVMEDIA_TYPE_VIDEO && codec->pix_fmts)
            {
                info.mediaType = MediaType::video;

                const enum AVPixelFormat* p;
                for (p = codec->pix_fmts; *p != -1; p++)
                {
                    const char* pix_fmt_name = av_get_pix_fmt_name(*p);
                    if (pix_fmt_name)
                        info.format(pix_fmt_name, pix_fmt_name);
                }

                // Add the global options
                addGlobalOptions(info, codecCtx, AV_OPT_FLAG_VIDEO_PARAM);
            }
            else if (codec->type == AVMEDIA_TYPE_AUDIO && codec->sample_fmts)
            {
                info.mediaType = MediaType::audio;

                const enum AVSampleFormat* p;
                for (p = codec->sample_fmts; *p != -1; p++)
                {
                    const char* sample_fmt_name = av_get_sample_fmt_name(*p);
                    if (sample_fmt_name)
                        info.format(sample_fmt_name, sample_fmt_name);
                }

                // Add the global options
                addGlobalOptions(info, codecCtx, AV_OPT_FLAG_AUDIO_PARAM);
            }
            else
                continue;

            avcodec_free_context(&codecCtx);
                
            // Iterate over encoder options
            addPrivateOptions(info, codec->priv_class);

            registerAsset(info);
        }
    }

    void FFmpegEncodersPlugin::addGlobalOptions(AssetInfo& info, AVCodecContext* codecCtx, int flags)
    {
        flags |= AV_OPT_FLAG_ENCODING_PARAM;

        ItemParamGroup& global = info.group("global", "Global", ItemParamGroupType::Forced);

        //const AVOption* option = NULL;
        //while ((option = av_opt_next(codecCtx, option)) != NULL)
        //{
        //    // Only use flags that are relevant to us
        //    if ((option->flags & flags) != flags)
        //        continue;

        //    const std::string name = option->name;

        if (boost::algorithm::ends_with(info.id, "_amf"))
        {
            const std::string name = info.id;

            global.param<std::string>("rc", "Strategy")
                .description("The encoding mode to use. Choose Constant Bitrate (CBR), Constant Quantizer (CQP) or Variable Bitrate (VBR) and if two encoding passes should be used (VBR only).")
                .option("Constant Quantizer (CQP)", "cqp", [name](const ItemParamAction& action) {
                    action.setVisible("global.qp_i", true);
                    action.setVisible("global.qp_p", true);
                    action.setVisible("global.qp_b", boost::algorithm::starts_with(name, "h264_"));
                    action.setVisible("global.b", false);
                })
                .option("Constant Bitrate (CBR)", "cbr", [](const ItemParamAction& action) {
                    action.setVisible("global.qp_i", false);
                    action.setVisible("global.qp_p", false);
                    action.setVisible("global.qp_b", false);
                    action.setVisible("global.b", true);
                })
                .option("Peak Contrained Variable Bitrate (VBR Peak)", "vbr_peak", [](const ItemParamAction& action) {
                    action.setVisible("global.qp_i", false);
                    action.setVisible("global.qp_p", false);
                    action.setVisible("global.qp_b", false);
                    action.setVisible("global.b", true);
                })
                .option("Latency Contrained Variable Bitrate (VBR Latency)", "vbr_latency", [](const ItemParamAction& action) {
                    action.setVisible("global.qp_i", false);
                    action.setVisible("global.qp_p", false);
                    action.setVisible("global.qp_b", false);
                    action.setVisible("global.b", true);
                })
                .defaultValue("cqp");

            global.param<int>("qp_i", "Quant. parameter (I-Frame)", 1)
                .description("Quantization Parameter for I-Frame")
                .minValue(0)
                .maxValue(51)
                .defaultValue(22);

            global.param<int>("qp_p", "Quant. parameter (P-Frame)", 1)
                .description("Quantization Parameter for P-Frame")
                .minValue(0)
                .maxValue(51)
                .defaultValue(22);

            global.param<int>("qp_b", "Quant. parameter (B-Frame)", 1)
                .description("Quantization Parameter for B-Frame")
                .minValue(0)
                .maxValue(51)
                .defaultValue(22);

            global.param<int>("b", "Bitrate [kbit]", 1)
                .description("The data rate allowed by the encoder.")
                .minValue(0)
                .maxValue(512000)
                .multiplierValue(1000)
                .defaultValue(15000);

            global.param<int>("g", "Keyframe interval")
                .description("A keyframe is inserted at least every x frames, sometimes sooner.")
                .minValue(1)
                .maxValue(INT_MAX)
                .defaultValue(60);
        }
        else if (boost::algorithm::ends_with(info.id, "_nvenc"))
        {
            global.param<std::string>("rc", "Strategy")
                .description("")
                .option("Constant QP mode", "constqp", [](const ItemParamAction& action) {
                    action.setVisible("global.qp", true);
                    action.setVisible("global.b", false);
                    action.setVisible("global.maxrate", false);
                    action.setVisible("global.bufsize", false);
                    action.setVisible("global.cq", false);
                })
                .option("Constant bitrate mode", "cbr", [](const ItemParamAction& action) {
                    action.setVisible("global.qp", false);
                    action.setVisible("global.b", true);
                    action.setVisible("global.maxrate", false);
                    action.setVisible("global.bufsize", true);
                    action.setVisible("global.cq", false);
                })
                .option("Variable bitrate mode", "vbr", [](const ItemParamAction& action) {
                    action.setVisible("global.qp", false);
                    action.setVisible("global.b", true);
                    action.setVisible("global.maxrate", true);
                    action.setVisible("global.bufsize", true);
                    action.setVisible("global.cq", true);
                })
                .defaultValue("constqp");

            global.param<int>("qp", "Quantizer", 1)
                .description("Quality level of the video. Lower values mean better quality but increase file size and processing time.")
                .minValue(0)
                .maxValue(51)
                .defaultValue(23);

            global.param<int>("b", "Bitrate [kbit/s]", 1)
                .description("The data rate allowed by the encoder.")
                .minValue(0)
                .maxValue(512000)
                .multiplierValue(1000)
                .defaultValue(15000);

            global.param<int>("maxrate", "Max. Bitrate [kbit/s]", 1)
                .description("Higher value can improve maximum quality, but increases decoder requirements.")
                .minValue(0)
                .maxValue(512000)
                .multiplierValue(1000)
                .defaultValue(15000);

            global.param<int>("bufsize", "Buffer Size [kbit/s]", 1)
                .description("The encoder bitstream buffer size.")
                .minValue(0)
                .maxValue(512000)
                .multiplierValue(1000)
                .defaultValue(15000);

            global.param<int>("cq", "Constant quality", 1)
                .description("Set target quality level (0 to 51, 0 means automatic).")
                .minValue(0)
                .maxValue(51)
                .defaultValue(0);

            global.param<int>("g", "Keyframe interval")
                .description("A keyframe is inserted at least every x frames, sometimes sooner.")
                .minValue(1)
                .maxValue(INT_MAX)
                .defaultValue(60);
        }
        else if (boost::algorithm::ends_with(info.id, "_qsv"))
        {
            global.param<std::string>("_strategy", "Strategy")
                .ignore(true)
                .description("The rate control method.")
                .option("Constant Quantizer", "cqp", [](const ItemParamAction& action) {
                    action.setVisible("global.global_quality", true);
                    action.setVisible("global.b", false);
                    action.setVisible("global.maxrate", false);
                    action.setVisible("global.avbr_accuracy", false);
                    action.setVisible("global.avbr_convergence", false);
                    action.setProperty("qscale", "1");
                    action.setProperty("look_ahead", "");
                })
                .option("Intelligent Constant Quality (w. lookahead)", "la_cqp", [](const ItemParamAction& action) {
                    action.setVisible("global.global_quality", true);
                    action.setVisible("global.b", false);
                    action.setVisible("global.maxrate", false);
                    action.setVisible("global.avbr_accuracy", false);
                    action.setVisible("global.avbr_convergence", false);
                    action.setProperty("qscale", "1");
                    action.setProperty("look_ahead", "1");
                })
                .option("Intelligent Constant Quality", "icq", [](const ItemParamAction& action) {
                    action.setVisible("global.global_quality", true);
                    action.setVisible("global.b", false);
                    action.setVisible("global.maxrate", false);
                    action.setVisible("global.avbr_accuracy", false);
                    action.setVisible("global.avbr_convergence", false);
                    action.setProperty("qscale", "");
                    action.setProperty("look_ahead", "");
                })
                .option("VBR with lookahead", "la", [](const ItemParamAction& action) {
                    action.setVisible("global.global_quality", false);
                    action.setVisible("global.b", true);
                    action.setVisible("global.maxrate", true);
                    action.setVisible("global.avbr_accuracy", false);
                    action.setVisible("global.avbr_convergence", false);
                    action.setProperty("qscale", "");
                    action.setProperty("look_ahead", "1");
                })
                .option("CBR / VBR", "cbr/vbr", [](const ItemParamAction& action) {
                    action.setVisible("global.global_quality", false);
                    action.setVisible("global.b", true);
                    action.setVisible("global.maxrate", true);
                    action.setVisible("global.avbr_accuracy", false);
                    action.setVisible("global.avbr_convergence", false);
                    action.setProperty("qscale", "");
                    action.setProperty("look_ahead", "");
                })
                .option("Average VBR", "avbr", [](const ItemParamAction& action) {
                    action.setVisible("global.global_quality", false);
                    action.setVisible("global.b", true);
                    action.setVisible("global.maxrate", false);
                    action.setVisible("global.avbr_accuracy", true);
                    action.setVisible("global.avbr_convergence", true);
                    action.setProperty("qscale", "");
                    action.setProperty("look_ahead", "");
                })
                .defaultValue("la_cqp");

            global.param<int>("global_quality", "Quality", 1)
                .description("Quality setting (1-51).")
                .minValue(1)
                .maxValue(51)
                .defaultValue(25);

            global.param<int>("b", "Bitrate [kbit]", 1)
                .description("The data rate allowed by the encoder.")
                .minValue(100)
                .maxValue(512000)
                .multiplierValue(1000)
                .defaultValue(15000);

            global.param<int>("maxrate", "Max. Bitrate [kbit]", 1)
                .description("Higher value can improve maximum quality, but increases decoder requirements.")
                .minValue(100)
                .maxValue(512000)
                .multiplierValue(1000)
                .defaultValue(15000);

            global.param<int>("avbr_accuracy", "AVBR Accuracy", 1)
                .description("Accuracy of the AVBR ratecontrol")
                .minValue(0)
                .maxValue(1000)
                .defaultValue(0);

            global.param<int>("avbr_convergence", "AVBR Convergence", 1)
                .description("Convergence of the AVBR ratecontrol")
                .minValue(0)
                .maxValue(99999)
                .defaultValue(0);

            global.param<int>("g", "Keyframe interval")
                .description("A keyframe is inserted at least every x frames, sometimes sooner.")
                .minValue(1)
                .maxValue(INT_MAX)
                .defaultValue(60);
        }
        else if (info.id == "mpeg1video" || info.id == "mpeg2video")
        {
            global.param<std::string>("rc", "Strategy")
                .ignore(true)
                .description("The encoding mode to use. Choose Constant Bitrate (CBR), Constant Quantizer (CQP) or Variable Bitrate (VBR) and if two encoding passes should be used (VBR only).")
                .option("Average Bitrate", "avr", [](const ItemParamAction& action) {
                    action.setVisible("global.minrate", false);
                    action.setVisible("global.maxrate", false);
                    action.setVisible("global.bufsize", false);
                })
                .option("Constant / Variable Bitrate", "cvbr", [](const ItemParamAction& action) {
                    action.setVisible("global.minrate", true);
                    action.setVisible("global.maxrate", true);
                    action.setVisible("global.bufsize", true);
                })
                .defaultValue("avr");

            global.param<int>("b", "Bitrate [kbit/s]", 1)
                .description("The data rate allowed by the encoder.")
                .minValue(0)
                .maxValue(512000)
                .multiplierValue(1024)
                .defaultValue(15000);

            global.param<int>("minrate", "Min. Bitrate [kbit/s]", 1)
                .description("Higher value can improve maximum quality, but increases decoder requirements.")
                .minValue(0)
                .maxValue(512000)
                .multiplierValue(1024)
                .defaultValue(15000);

            global.param<int>("maxrate", "Max. Bitrate [kbit/s]", 1)
                .description("Higher value can improve maximum quality, but increases decoder requirements.")
                .minValue(0)
                .maxValue(512000)
                .multiplierValue(1024)
                .defaultValue(15000);

            global.param<int>("bufsize", "Buffer Size [kbit/s]", 1)
                .description("The encoder bitstream buffer size.")
                .minValue(0)
                .maxValue(512000)
                .multiplierValue(1024)
                .defaultValue(15000);

            global.param<int>("g", "Keyframe interval")
                .description("A keyframe is inserted at least every x frames, sometimes sooner.")
                .minValue(1)
                .maxValue(INT_MAX)
                .defaultValue(60);
        }
        else if (info.id == "libvpx" || info.id == "libvpx-vp9")
        {
            global.param<std::string>("_strategy", "Strategy")
                .description("The encoding strategy to use.")
                .option("Average Bitrate (ABR)", "", [](const ItemParamAction& action) {
                    action.setVisible("global.b", true);
                    action.setVisible("global.crf", false);
                })
                .option("Constant Quality (CRF)", "crf", [](const ItemParamAction& action) {
                    action.setVisible("global.b", false);
                    action.setVisible("global.crf", true);
                })
                .option("Constrained Quality (CQ)", "bitrate_crf", [](const ItemParamAction& action) {
                    action.setVisible("global.b", true);
                    action.setVisible("global.crf", true);
                })
                .defaultValue("crf");

            global.param<int>("b", "Average Bitrate [kbit/s]", 1)
                .description("The average data rate allowed by the encoder.")
                .multiplierValue(1024)
                .minValue(0)
                .maxValue(288000)
                .defaultValue(15000);

            global.param<int>("crf", "Constant Rate Factor", 1)
                .description("Lower values mean better quality but increase the processing time.")
                .minValue(4)
                .maxValue(63)
                .defaultValue(10);
        }
        else if (info.id == "libsvtav1")
        {
            global.param<int>("rc", "Strategy")
                .description("Bit rate control mode")
                .option("Constant Rate Factor", 0, [](const ItemParamAction& action)
                    {
                        action.setVisible("global.crf", true);
                        action.setVisible("global.qp", true);
                        action.setVisible("global.tbr", false);
                        action.setVisible("global.mbr", false);
                    })
                .option("Variable Bitrate", 1, [](const ItemParamAction& action)
                    {
                        action.setVisible("global.crf", false);
                        action.setVisible("global.qp", false);
                        action.setVisible("global.tbr", true);
                        action.setVisible("global.mbr", true);
                    })
                .option("Constant Bitrate", 2, [](const ItemParamAction& action)
                    {
                        action.setVisible("global.crf", false);
                        action.setVisible("global.qp", false);
                        action.setVisible("global.tbr", true);
                        action.setVisible("global.mbr", false);
                    })
                .defaultValue(0);

            global.param<int>("crf", "Constant Rate Factor", 1)
                .description("Lower values mean better quality but increase the processing time.")
                .minValue(1)
                .maxValue(63)
                .defaultValue(35);

            global.param<int>("qp", "Initial QP level value", 1)
                .description("Initial QP level value")
                .minValue(1)
                .maxValue(63)
                .defaultValue(35);

            global.param<int>("tbr", "Bitrate [kbit]", 1)
                .description("The data rate allowed by the encoder.")
                .minValue(1)
                .maxValue(100000)
                .defaultValue(2000);

            global.param<int>("mbr", "Max. Bitrate [kbit]", 1)
                .description("Higher value can improve maximum quality, but increases decoder requirements.")
                .minValue(1)
                .maxValue(100000)
                .defaultValue(2000);
        }
        else if (info.id == "libx264" || info.id == "libx265")
        {
            global.param<int>("rc", "Strategy")
                .description("Bit rate control mode")
                .option("Constant Rate Factor", 0, [](const ItemParamAction& action)
                    {
                        action.setVisible("global.crf", true);
                        action.setVisible("global.qp", false);
                        action.setVisible("global.b", false);
                        action.setVisible("global.minrate", false);
                        action.setVisible("global.maxrate", false);
                    })
                .option("Constant Quantizer Parameter", 1, [](const ItemParamAction& action)
                    {
                        action.setVisible("global.crf", false);
                        action.setVisible("global.qp", true);
                        action.setVisible("global.b", false);
                        action.setVisible("global.minrate", false);
                        action.setVisible("global.maxrate", false);
                    })
                .option("Constant Bitrate", 2, [](const ItemParamAction& action)
                    {
                        action.setVisible("global.crf", false);
                        action.setVisible("global.qp", false);
                        action.setVisible("global.b", true);
                        action.setVisible("global.minrate", true);
                        action.setVisible("global.maxrate", true);
                    })
                .defaultValue(0);

            global.param<int>("crf", "Constant Rate Factor", 1)
                .description("Lower values mean better quality but increase the processing time.")
                .minValue(1)
                .maxValue(51)
                .defaultValue(info.id == "libx264" ? 23 : 28);

            global.param<int>("qp", "Initial QP level value", 1)
                .description("Initial QP level value")
                .minValue(1)
                .maxValue(51)
                .defaultValue(info.id == "libx264" ? 23 : 28);

            global.param<int>("b", "Bitrate [kbit/s]", 1)
                .description("The data rate allowed by the encoder.")
                .minValue(0)
                .maxValue(512000)
                .multiplierValue(1024)
                .defaultValue(15000);

            global.param<int>("minrate", "Min. Bitrate [kbit/s]", 1)
                .description("Higher value can improve maximum quality, but increases decoder requirements.")
                .minValue(0)
                .maxValue(512000)
                .multiplierValue(1024)
                .defaultValue(15000);

            global.param<int>("maxrate", "Max. Bitrate [kbit/s]", 1)
                .description("Higher value can improve maximum quality, but increases decoder requirements.")
                .minValue(0)
                .maxValue(512000)
                .multiplierValue(1024)
                .defaultValue(15000);
        }
        else if (info.id == "vc2")
        {
            global.param<int>("b", "Average Bitrate [kbit/s]", 1)
                .description("The average data rate allowed by the encoder.")
                .multiplierValue(1024)
                .minValue(0)
                .maxValue(288000)
                .defaultValue(15000);
        }
        else if (info.id == "prores_aw")
        {
            global.param<int>("profile", "Profile")
                .description("The average data rate allowed by the encoder.")
                .option("422 Proxy (APCO)", 0)
                .option("422 LT (APCS)", 1)
                .option("422 SD (APCN)", 2)
                .option("422 HQ (APCH)", 3)
                .defaultValue(1);
        }
        else if (info.id == "aac")
        {
            global.param<std::string>("rc", "Strategy").ignore(true)
                .description("The encoding mode to use. Choose Constant Bitrate (CBR), Constant Quantizer (CQP) or Variable Bitrate (VBR) and if two encoding passes should be used (VBR only).")
                .option("Constant Bitrate", "cbr", [&](const ItemParamAction& action)
                    {
                        action.setVisible("global.b", true);
                        action.setVisible("global.global_quality", false);
                    })
                .option("Variable Bitrate", "vbr", [&](const ItemParamAction& action)
                    {
                        action.setVisible("global.b", false);
                        action.setVisible("global.global_quality", true);
                    });

            global.param<int>("b", "Bit rate", 1)
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

            global.param<int>("global_quality", "Quality", 1)
                .description("The quality as a variable bit rate in kbit/s per channel.")
                .option("20-32 kbit/s per channel", 1)
                .option("32-40 kbit/s per channel", 2)
                .option("48-56 kbit/s per channel", 3)
                .option("64-72 kbit/s per channel", 4)
                .option("96-112kbit/s per channel", 5)
                .defaultValue(1);

            global.param<std::string>("profile", "Profile")
                .description("Sets the encoding profile")
                .option("Low-complexity", "aac_low")
                .option("MPEG2 Low-complexity", "mpeg2_aac_low")
                .option("Long term prediction", "aac_ltp")
                .option("Main", "aac_main")
                .defaultValue("aac_low");

            global.param<int>("cutoff", "Cutoff frequency [in Hz]")
                .description("Set cutoff frequency. If unspecified will allow the encoder to dynamically adjust the cutoff to improve clarity on low bitrates.")
                .minValue(0)
                .maxValue(20000)
                .defaultValue(17000);
        }
        else if (info.id == "ac3" || info.id == "ac3_fixed")
        {
            global.param<int>("b", "Bitrate")
                .description("The bitrate used for encoding audio. Higher bitrate implies better quality.")
                .option("96 kbit/s", 96000)
                .option("128 kbit/s", 128000)
                .option("192 kbit/s", 192000)
                .option("256 kbit/s", 256000)
                .option("320 kbit/s", 320000)
                .option("384 kbit/s", 384000)
                .option("448 kbit/s", 448000)
                .option("512 kbit/s", 512000)
                .option("576 kbit/s", 576000)
                .option("640 kbit/s", 640000)
                .defaultValue(192000);
        }
        else if (info.id == "cinepak")
        {
            global.param<int>("global_quality", "Quality factor")
                .description("Quality factor. Lower is better. Higher gives lower bitrate.")
                .minValue(1)
                .maxValue(1000)
                .defaultValue(10);

            global.param<int>("g", "Keyframe interval")
                .description("A keyframe is inserted at least every x frames, sometimes sooner.")
                .minValue(1)
                .maxValue(9999)
                .defaultValue(60);
        }
        else if (info.id == "dca")
        {
            global.param<int>("b", "Bitrate")
                .description("The bitrate used for encoding audio. Higher bitrate implies better quality.")
                .option("672 kbit/s", 672000)
                .option("896 kbit/s", 896000)
                .option("1024 kbit/s", 1024000)
                .option("1344 kbit/s", 1344000)
                .option("1536 kbit/s", 1536000)
                .option("1920 kbit/s", 1920000)
                .option("3072 kbit/s", 3072000)
                .option("3840 kbit/s", 3840000)
                .defaultValue(896000);
        }
        else if (info.id == "eac3")
        {
            global.param<int>("b", "Bitrate")
                .description("The bitrate used for encoding audio. Higher bitrate implies better quality.")
                .option("96 kbit/s", 96000)
                .option("128 kbit/s", 128000)
                .option("192 kbit/s", 192000)
                .option("256 kbit/s", 256000)
                .option("320 kbit/s", 320000)
                .option("384 kbit/s", 384000)
                .option("448 kbit/s", 448000)
                .option("512 kbit/s", 512000)
                .option("576 kbit/s", 576000)
                .option("640 kbit/s", 640000)
                .option("786 kbit/s", 768000)
                .option("896 kbit/s", 896000)
                .option("1152 kbit/s", 1152000)
                .option("1408 kbit/s", 1408000)
                .option("1920 kbit/s", 1920000)
                .option("2432 kbit/s", 2432000)
                .option("3456 kbit/s", 3456000)
                .option("4480 kbit/s", 4480000)
                .option("6144 kbit/s", 6144000)
                .defaultValue(640000);
        }
        else if (info.id == "flac")
        {
            global.param<int>("compression_level", "Compression")
                .description("The constant bit rate in kbit/s for all channels.")
                .minValue(0)
                .maxValue(12)
                .defaultValue(5);
        }
        else if (info.id == "libmp3lame")
        {
            global.param<int>("b", "Bit rate")
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
        }
        else if (info.id == "libtheora")
        {
            global.param<int>("b", "Bitrate [kbit/s]", 1)
                .description("The data rate allowed by the encoder.")
                .minValue(0)
                .maxValue(512000)
                .multiplierValue(1024)
                .defaultValue(15000);
        }
        else if (info.id == "libtwolame")
        {
            global.param<int>("b", "Bit rate")
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
                }
        else if (info.id == "libopencore-amrnb")
        {
            global.param<int>("b", "Bit rate")
                .description("The constant bit rate in bit/s for all channels.")
                .option("4750 bit/s", 4750)
                .option("5150 bit/s", 5150)
                .option("5900 bit/s", 5900)
                .option("6700 bit/s", 6700)
                .option("7400 bit/s", 7400)
                .option("7950 bit/s", 7950)
                .option("10200 bit/s", 10200)
                .option("12200 bit/s", 12200)
                .defaultValue(7950);
        }
        else if (info.id == "libopus" || info.id == "opus")
        {
            global.param<int>("b", "Bitrate")
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
        }
        else if (info.id == "libvorbis" || info.id == "vorbis")
        {
            global.param<std::string>("rc", "Rate control").ignore(true)
                .description("The selected rate control mechanism.")
                .option("Constant bit rate", "cbr", [](const ItemParamAction& action) {
                    action.setVisible("global.b", true);
                    action.setVisible("global.qp", false);
                    })
                .option("Variable bit rate", "vbr", [](const ItemParamAction& action) {
                    action.setVisible("global.b", false);
                    action.setVisible("global.qp", true);
                })
                .defaultValue("qp");

            global.param<int>("b", "Constant bit rate", 1)
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

            global.param<int>("qp", "Variable bit rate", 1)
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
        }
        else if (info.id == "mp2" || info.id == "mp2fixed")
        {
            global.param<int>("b", "Bit rate", 1)
                .description("The constant bit rate in kbit/s for all channels.")
                .option("32 kbit/s", 32000)
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
                .option("384 kbit/s", 384000)
                .defaultValue(128000);
        }
        else if (info.id == "wavpack")
        {
            global.param<int>("compression_level", "Compression level")
                .description("Compression level")
                .minValue(0)
                .maxValue(8)
                .defaultValue(1);
        }
    }
}
