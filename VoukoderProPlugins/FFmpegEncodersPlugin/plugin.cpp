#include "plugin.h"

#include <boost/algorithm/string/predicate.hpp>

namespace VoukoderPro
{
    FFmpegEncodersPlugin::FFmpegEncodersPlugin() : EncoderPlugin()
    {
        const AVCodec* codec = NULL;
        void* opaque = NULL;

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

            if (codec->type == AVMEDIA_TYPE_VIDEO && codec->pix_fmts)
            {
                info.mediaType = MediaType::video;

                const enum AVPixelFormat* p;
                for (p = codec->pix_fmts; *p != -1; p++) {
                    const char* pix_fmt_name = av_get_pix_fmt_name(*p);
                    if (pix_fmt_name) {
                        info.format(pix_fmt_name, pix_fmt_name);
                    }
                }
            }
            else if (codec->type == AVMEDIA_TYPE_AUDIO && codec->sample_fmts)
            {
                info.mediaType = MediaType::audio;

                const enum AVSampleFormat* p;
                for (p = codec->sample_fmts; *p != -1; p++) {
                    const char* sample_fmt_name = av_get_sample_fmt_name(*p);
                    if (sample_fmt_name) {
                        info.format(sample_fmt_name, sample_fmt_name);
                    }
                }
            }
            else
                continue;

            // Iterate over filter options
            createFFmpegParameters(info, codec->priv_class);

            registerAsset(info);
        }
    }

    int FFmpegEncodersPlugin::registerAAC()
    {
        AssetInfo info;
        info.id = "aac";
        info.name = "AAC";
        info.category = std::make_pair("ffmpeg", "FFmpeg");
        info.description = "FFmpegs built-in AAC encoder";
        info.type = NodeInfoType::encoder;
        info.mediaType = MediaType::audio;
        info.helpUrl = "https://ffmpeg.org/ffmpeg-codecs.html#" + info.id;

        info.format("Floating Point", "fltp");

        auto& basic = info.group("basic", "Basic", ItemParamGroupType::Forced);
        basic.param<std::string>("rc", "Rate control").ignore(true)
            .description("The selected rate control mechanism.")
            .option("Constant bit rate", "cbr", [](const ItemParamAction& action) {
                action.setVisible("basic.b", true);
                action.setVisible("basic.q", false);
            })
            .option("Variable bit rate", "vbr", [](const ItemParamAction& action) {
                action.setVisible("basic.b", false);
                action.setVisible("basic.q", true);
            })
            .defaultValue("cbr");

        basic.param<int>("b", "Bit rate", 1)
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
            .defaultValue(128000);

        basic.param<int>("global_quality", "Quality", 1)
            .description("The quality as a variable bit rate in kbit/s per channel.")
            .option("20-32 kbit/s per channel", 1)
            .option("32-40 kbit/s per channel", 2)
            .option("48-56 kbit/s per channel", 3)
            .option("64-72 kbit/s per channel", 4)
            .option("96-112kbit/s per channel", 5)
            .defaultValue(1);

        basic.param<std::string>("profile", "Profile")
            .description("Sets the encoding profile")
            .option("Low-complexity", "aac_low")
            .option("MPEG2 Low-complexity", "mpeg2_aac_low")
            .option("Long term prediction", "aac_ltp")
            .option("Main", "aac_main")
            .defaultValue("aac_low");

        auto& advanced = info.group("advanced", "Advanced", ItemParamGroupType::Standard);
        advanced.param<int>("cutoff", "Cutoff frequency [in Hz]")
            .description("Set cutoff frequency. If unspecified will allow the encoder to dynamically adjust the cutoff to improve clarity on low bitrates.")
            .minValue(0)
            .maxValue(20000)
            .defaultValue(0);

        advanced.param<std::string>("aac_coder", "Coding algorithm")
            .description("Coding algorithm")
            .option("ANMR", "anmr")
            .option("Two Loop", "twoloop")
            .option("Fast search", "fast")
            .defaultValue("twoloop");

        advanced.param<bool>("aac_ms", "Force M/S stereo coding")
            .description("Force M/S stereo coding")
            .defaultValue(false);

        advanced.param<bool>("aac_is", "Intensity stereo coding")
            .description("Intensity stereo coding")
            .defaultValue(true);

        advanced.param<bool>("aac_pns", "Force M/S stereo coding")
            .description("Perceptual noise substitution")
            .defaultValue(true);

        advanced.param<bool>("aac_tns", "Temporal noise shaping")
            .description("Temporal noise shaping")
            .defaultValue(true);

        advanced.param<bool>("aac_ltp", "Long term prediction")
            .description("Long term prediction")
            .defaultValue(false);

        advanced.param<bool>("aac_pred", "AAC-Main prediction")
            .description("AAC-Main prediction")
            .defaultValue(false);

        advanced.param<bool>("aac_pce", "Forces the use of PCEs")
            .description("Forces the use of PCEs")
            .defaultValue(false);

        return registerAsset(info);
    }

    int FFmpegEncodersPlugin::registerAC3()
    {
        AssetInfo info;
        info.id = "ac3";
        info.name = "AC3";
        info.category = std::make_pair("ffmpeg", "FFmpeg");
        info.description = "FFmpegs built-in AC3 encoder";
        info.type = NodeInfoType::encoder;
        info.mediaType = MediaType::audio;
        info.helpUrl = "https://ffmpeg.org/ffmpeg-codecs.html#" + info.id;
        
        info.format("Floating Point", "fltp");
        
        auto& basic = info.group("basic", "Basic", ItemParamGroupType::Forced);
        basic.param<int>("b", "Bitrate")
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

        auto& metadata = info.group("metadata", "Metadata", ItemParamGroupType::Standard);
        metadata.param<bool>("copyright", "Copyright Indicator")
            .description("Specifies whether a copyright exists for this audio.")
            .defaultValue(false);

        metadata.param<std::string>("dsur_mode", "Dolby Surround Mode")
            .description("Specifies whether the stereo signal uses Dolby Surround (Pro Logic). This field will only be written to the bitstream if the audio stream is stereo. Using this option does NOT mean the encoder will actually apply Dolby Surround processing.")
            .option("Not indicated", "notindicated")
            .option("Enabled", "on")
            .option("Disbaled", "off")
            .defaultValue("notindicated");

        metadata.param<bool>("original", "Original Bit Stream Indicator")
            .description("Specifies whether this audio is from the original source and not a copy.")
            .defaultValue(true);

        metadata.param<int>("dialnorm", "Dialogue Normalization")
            .description("Indicates how far the average dialogue level of the program is below digital 100% full scale (0 dBFS). This parameter determines a level shift during audio reproduction that sets the average volume of the dialogue to a preset level. The goal is to match volume level between program sources. A value of -31dB will result in no volume level change, relative to the source volume, during audio reproduction. Valid values are whole numbers in the range -31 to -1, with -31 being the default.")
            .minValue(-31)
            .maxValue(-1)
            .defaultValue(-31);

        auto& bitstream = info.group("bitstream", "Bitstream Information", ItemParamGroupType::Standard);
        bitstream.param<std::string>("dmix_mode", "Preferred Stereo Downmix Mode")
            .description("Allows the user to select either Lt/Rt (Dolby Surround) or Lo/Ro (normal stereo) as the preferred stereo downmix mode.")
            .option("Not indicated", "notindicated")
            .option("Lt/Rt Downmix preferred", "ltrt")
            .option("Lo/Ro Downmix preferred", "loro")
            .option("Dolby Pro Logic II Downmix preferred", "dplii")
            .defaultValue("notindicated");

        bitstream.param<double>("ltrt_cmixlev", "Lt/Rt Center Mix Level")
            .description("The amount of gain the decoder should apply to the center channel when downmixing to stereo in Lt/Rt mode.")
            .option("+3dB gain", 1.414)
            .option("+1.5dB gain", 1.189)
            .option("0dB gain", 1.000)
            .option("-1.5dB gain", 0.841)
            .option("-3.0dB gain", 0.707)
            .option("-4.5dB gain", 0.595)
            .option("-6.0dB gain", 0.500)
            .option("Silence center channel", 0.000)
            .defaultValue(0.595);

        bitstream.param<double>("ltrt_surmixlev", "Lt/Rt Surround Mix Level")
            .description("The amount of gain the decoder should apply to the surround channel(s) when downmixing to stereo in Lt/Rt mode.")
            .option("-1.5dB gain", 0.841)
            .option("-3.0dB gain", 0.707)
            .option("-4.5dB gain", 0.595)
            .option("-6.0dB gain", 0.500)
            .option("Silence surround channel(s)", 0.000)
            .defaultValue(0.500);

        bitstream.param<double>("loro_cmixlev", "Lo/Ro Center Mix Level")
            .description("The amount of gain the decoder should apply to the center channel when downmixing to stereo in Lo/Ro mode.")
            .option("+3dB gain", 1.414)
            .option("+1.5dB gain", 1.189)
            .option("0dB gain", 1.000)
            .option("-1.5dB gain", 0.841)
            .option("-3.0dB gain", 0.707)
            .option("-4.5dB gain", 0.595)
            .option("-6.0dB gain", 0.500)
            .option("Silence center channel", 0.000)
            .defaultValue(0.595);

        bitstream.param<double>("loro_surmixlev", "Lo/Ro Surround Mix Level")
            .description("The amount of gain the decoder should apply to the surround channel(s) when downmixing to stereo in Lo/Ro mode.")
            .option("-1.5dB gain", 0.841)
            .option("-3.0dB gain", 0.707)
            .option("-4.5dB gain", 0.595)
            .option("-6.0dB gain", 0.500)
            .option("Silence surround channel(s)", 0.000)
            .defaultValue(0.500);

        bitstream.param<std::string>("dsurex_mode", "Dolby Surround EX Mode")
            .description("Indicates whether the stream uses Dolby Surround EX (7.1 matrixed to 5.1). Using this option does NOT mean the encoder will actually apply Dolby Surround EX processing.")
            .option("Not indicated", "notindicated")
            .option("Enabled", "on")
            .option("Disabled", "off")
            .option("Dolby Pro Logic IIz", "dpliiz")
            .defaultValue("notindicated");

        bitstream.param<std::string>("dheadphone_mode", "Dolby Headphone Mode")
            .description("Indicates whether the stream uses Dolby Headphone encoding (multi-channel matrixed to 2.0 for use with headphones). Using this option does NOT mean the encoder will actually apply Dolby Headphone processing.")
            .option("Not indicated", "notindicated")
            .option("Enabled", "on")
            .option("Disabled", "off")
            .defaultValue("notindicated");

        bitstream.param<std::string>("ad_conv_type", "A/D Converter Type")
            .description("Indicates whether the audio has passed through HDCD A/D conversion.")
            .option("Standard", "standard")
            .option("HDCD", "hdcd")
            .defaultValue("standard");

        auto& misc = info.group("misc", "Miscellaneous", ItemParamGroupType::Standard);
        misc.param<double>("center_mixlev", "Center Mix Level")
            .description("The amount of gain the decoder should apply to the center channel when downmixing to stereo. This field will only be written to the bitstream if a center channel is present.")
            .option("-3dB gain", 0.707)
            .option("-4.5dB gain", 0.595)
            .option("-6dB gain", 0.500)
            .defaultValue(0.595);

        misc.param<double>("surround_mixlev", "Surround Mix Level")
            .description("The amount of gain the decoder should apply to the surround channel(s) when downmixing to stereo. This field will only be written to the bitstream if one or more surround channels are present.")
            .option("-3dB gain", 0.707)
            .option("-6dB gain", 0.500)
            .option("Silence surround channel(s)", 0.000)
            .defaultValue(0.500);

        misc.param<std::string>("room_type", "Room Type")
            .description("Describes the equalization used during the final mixing session at the studio or on the dubbing stage. A large room is a dubbing stage with the industry standard X-curve equalization; a small room has flat equalization. This field will not be written to the bitstream if both the mixing_level option and the room_type option have the default values.")
            .option("Not Indicated", "notindicated")
            .option("Large", "large")
            .option("Small", "small")
            .defaultValue("notindicated");

        misc.param<bool>("stereo_rematrixing", "Stereo Rematrixing")
            .description("Enables/Disables use of rematrixing for stereo input. This is an optional AC-3 feature that increases quality by selectively encoding the left/right channels as mid/side. This option is enabled by default, and it is highly recommended that it be left as enabled except for testing purposes.")
            .defaultValue(true);

        misc.param<bool>("channel_coupling", "Channel Coupling")
            .description("Enables/Disables use of channel coupling, which is an optional AC-3 feature that increases quality by combining high frequency information from multiple channels into a single channel. The per-channel high frequency information is sent with less accuracy in both the frequency and time domains. This allows more bits to be used for lower frequencies while preserving enough information to reconstruct the high frequencies. This option is enabled by default for the floating-point encoder and should generally be left as enabled except for testing purposes or to increase encoding speed.")
            .defaultValue(true);

        misc.param<int>("mixing_level", "Mixing Level")
            .description("Specifies peak sound pressure level (SPL) in the production environment when the mix was mastered. Valid values are 80 to 111, or -1 for unknown or not indicated. The default value is -1, but that value cannot be used if the Audio Production Information is written to the bitstream. Therefore, if the room_type option is not the default value, the mixing_level option must not be -1.")
            .minValue(80)
            .maxValue(111)
            .defaultValue(80);

        misc.param<int>("cpl_start_band", "Coupling Start Band")
            .description("Sets the channel coupling start band, from 1 to 15. If a value higher than the bandwidth is used, it will be reduced to 1 less than the coupling end band. If auto is used, the start band will be determined by the encoder based on the bit rate, sample rate, and channel layout. This option has no effect if channel coupling is disabled.")
            .minValue(1)
            .maxValue(15)
            .defaultValue(1);

        return registerAsset(info);
    }

    int FFmpegEncodersPlugin::registerALAC()
    {
        AssetInfo info;
        info.id = "alac";
        info.name = "ALAC";
        info.category = std::make_pair("ffmpeg", "FFmpeg");
        info.description = "FFmpegs built-in ALAC encoder";
        info.type = NodeInfoType::encoder;
        info.mediaType = MediaType::audio;
        info.helpUrl = "https://ffmpeg.org/ffmpeg-codecs.html#" + info.id;

        info.format("16 bit", "s16p")
            .format("32 bit", "s32p");

        auto& prediction_order = info.group("prediction_order", "Prediction Order", ItemParamGroupType::Standard);
        prediction_order.param<int>("min_prediction_order", "Minimum")
            .description("Minimum amount of prediction order.")
            .minValue(1)
            .maxValue(30)
            .defaultValue(4);

        prediction_order.param<int>("max_prediction_order", "Maximum")
            .description("Maximum amount of prediction order.")
            .minValue(1)
            .maxValue(30)
            .defaultValue(6);

        return registerAsset(info);
    }

    int FFmpegEncodersPlugin::registerCFHD()
    {
        AssetInfo info;
        info.id = "cfhd";
        info.name = "CineForm HD";
        info.category = std::make_pair("ffmpeg", "FFmpeg");
        info.description = "FFmpegs very own GoPro CineForm HD encoder";
        info.type = NodeInfoType::encoder;
        info.mediaType = MediaType::video;
        info.helpUrl = "https://ffmpeg.org/ffmpeg-codecs.html#" + info.id;

        info.format("YUV 422 (10 bit)", "yuv422p10le");

        auto& basic = info.group("basic", "Basic", ItemParamGroupType::Forced);
        basic.param<std::string>("quality", "Quality")
            .description("Set quality")
            .option("Film 3+", "film3+")
            .option("Film 3", "film3")
            .option("Film 2+", "film2+")
            .option("Film 2", "film2")
            .option("Film 1.5", "film1.5")
            .option("Film 1+", "film1+")
            .option("Film 1", "film1")
            .option("High +", "high+")
            .option("High", "high")
            .option("Medium +", "medium+")
            .option("Medium", "medium")
            .option("Low +", "low+")
            .option("Low", "low")
            .defaultValue("film3+");

        return registerAsset(info);
    }

    int FFmpegEncodersPlugin::registerDCA()
    {
        AssetInfo info;
        info.id = "dca";
        info.name = "DTS/DCA";
        info.category = std::make_pair("ffmpeg", "FFmpeg");
        info.description = "FFmpegs built-in DTS/DCA encoder";
        info.type = NodeInfoType::encoder;
        info.mediaType = MediaType::audio;
        info.helpUrl = "https://ffmpeg.org/ffmpeg-codecs.html#" + info.id;

        info.format("32 bit", "s32");

        auto& standard = info.group("standard", "Standard", ItemParamGroupType::Forced);
        standard.param<int>("b", "Bitrate")
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

        auto& misc = info.group("misc", "Miscellaneous", ItemParamGroupType::Standard);
        misc.param<bool>("dca_adpcm", "ADPCM")
            .description("Use ADPCM encoding")
            .defaultValue(false);

        return registerAsset(info);
    }

    int FFmpegEncodersPlugin::registerEAC3()
    {
        AssetInfo info;
        info.id = "eac3";
        info.name = "EAC3";
        info.category = std::make_pair("ffmpeg", "FFmpeg");
        info.description = "FFmpegs built-in EAC3 encoder";
        info.type = NodeInfoType::encoder;
        info.mediaType = MediaType::audio;
        info.helpUrl = "https://ffmpeg.org/ffmpeg-codecs.html#" + info.id;

        info.format("Floating Point", "fltp");

        auto& standard = info.group("standard", "Standard", ItemParamGroupType::Forced);
        standard.param<int>("b", "Bitrate")
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

        auto& metadata = info.group("metadata", "Metadata", ItemParamGroupType::Standard);
        metadata.param<bool>("copyright", "Copyright Indicator")
            .description("Specifies whether a copyright exists for this audio.")
            .defaultValue(false);

        metadata.param<std::string>("dsur_mode", "Dolby Surround Mode")
            .description("Specifies whether the stereo signal uses Dolby Surround (Pro Logic). This field will only be written to the bitstream if the audio stream is stereo. Using this option does NOT mean the encoder will actually apply Dolby Surround processing.")
            .option("Not Indicated (default)", "notindicated")
            .option("Not Dolby Surround Encoded", "off")
            .option("Dolby Surround Encoded", "on")
            .defaultValue("notindicated");

        metadata.param<bool>("original", "Original Bit Stream Indicator")
            .description("Specifies whether this audio is from the original source and not a copy.")
            .defaultValue(true);

        metadata.param<int>("dialnorm", "Dialogue Normalization")
            .description("Indicates how far the average dialogue level of the program is below digital 100% full scale (0 dBFS). This parameter determines a level shift during audio reproduction that sets the average volume of the dialogue to a preset level. The goal is to match volume level between program sources. A value of -31dB will result in no volume level change, relative to the source volume, during audio reproduction. Valid values are whole numbers in the range -31 to -1, with -31 being the default.")
            .minValue(-31)
            .maxValue(-1)
            .defaultValue(-31);

        auto& bitstream = info.group("bitstream", "Bitstream Information", ItemParamGroupType::Standard);
        bitstream.param<std::string>("dmix_mode", "Preferred Stereo Downmix Mode")
            .description("Allows the user to select either Lt/Rt (Dolby Surround) or Lo/Ro (normal stereo) as the preferred stereo downmix mode.")
            .option("Not Indicated (default)", "notindicated")
            .option("Lt/Rt Downmix Preferred", "ltrt")
            .option("Lo/Ro Downmix Preferred", "loro")
            .option("Dolby Pro Logic II Downmix Preferred", "dplii")
            .defaultValue("notindicated");

        bitstream.param<double>("ltrt_cmixlev", "Lt/Rt Center Mix Level")
            .description("The amount of gain the decoder should apply to the center channel when downmixing to stereo in Lt/Rt mode.")
            .option("+3dB gain", 1.414)
            .option("+1.5dB gain", 1.189)
            .option("0dB gain", 1.000)
            .option("-1.5dB gain", 0.841)
            .option("-3.0dB gain", 0.707)
            .option("-4.5dB gain (default)", 0.595)
            .option("-6.0dB gain", 0.500)
            .option("Silence Center Channel", 0.000)
            .defaultValue(0.595);

        bitstream.param<double>("ltrt_surmixlev", "Lt/Rt Surround Mix Level")
            .description("The amount of gain the decoder should apply to the surround channel(s) when downmixing to stereo in Lt/Rt mode.")
            .option("-1.5dB gain", 0.841)
            .option("-3.0dB gain", 0.707)
            .option("-4.5dB gain", 0.595)
            .option("-6.0dB gain (default)", 0.500)
            .option("Silence Surround Channel(s)", 0.000)
            .defaultValue(0.500);

        bitstream.param<double>("loro_cmixlev", "Lo/Ro Center Mix Level")
            .description("The amount of gain the decoder should apply to the center channel when downmixing to stereo in Lo/Ro mode.")
            .option("+3dB gain", 1.414)
            .option("+1.5dB gain", 1.189)
            .option("0dB gain", 1.000)
            .option("-1.5dB gain", 0.841)
            .option("-3.0dB gain", 0.707)
            .option("-4.5dB gain (default)", 0.595)
            .option("-6.0dB gain", 0.500)
            .option("Silence Center Channel", 0.000)
            .defaultValue(0.595);

        bitstream.param<double>("loro_surmixlev", "Lo/Ro Surround Mix Level")
            .description("The amount of gain the decoder should apply to the surround channel(s) when downmixing to stereo in Lo/Ro mode.")
            .option("-1.5dB gain", 0.841)
            .option("-3.0dB gain", 0.707)
            .option("-4.5dB gain", 0.595)
            .option("-6.0dB gain (default)", 0.500)
            .option("Silence Surround Channel(s)", 0.000)
            .defaultValue(0.500);

        bitstream.param<std::string>("dsurex_mode", "Dolby Surround EX Mode")
            .description("Indicates whether the stream uses Dolby Surround EX (7.1 matrixed to 5.1). Using this option does NOT mean the encoder will actually apply Dolby Surround EX processing.")
            .option("Not Indicated (default)", "notindicated")
            .option("Dolby Surround EX on", "on")
            .option("Dolby Surround EX off", "off")
            .option("Dolby Pro Logic IIz-encoded", "dpliiz")
            .defaultValue("notindicated");

        bitstream.param<std::string>("dheadphone_mode", "Dolby Headphone Mode")
            .description("Indicates whether the stream uses Dolby Headphone encoding (multi-channel matrixed to 2.0 for use with headphones). Using this option does NOT mean the encoder will actually apply Dolby Headphone processing.")
            .option("Not Indicated (default)", "notindicated")
            .option("Dolby Headphone Encoded", "on")
            .option("Not Dolby Headphone Encoded", "off")
            .defaultValue("notindicated");

        bitstream.param<std::string>("ad_conv_type", "A/D Converter Type")
            .description("Indicates whether the audio has passed through HDCD A/D conversion.")
            .option("Standard A/D Converter (default)", "standard")
            .option("HDCD A/D Converter", "hdcd")
            .defaultValue("standard");


        auto& misc = info.group("misc", "Miscellaneous", ItemParamGroupType::Standard);
        misc.param<std::string>("room_type", "Room Type")
            .description("Describes the equalization used during the final mixing session at the studio or on the dubbing stage. A large room is a dubbing stage with the industry standard X-curve equalization; a small room has flat equalization. This field will not be written to the bitstream if both the mixing_level option and the room_type option have the default values.")
            .option("Not Indicated (default)", "notindicated")
            .option("Large Room", "large")
            .option("Small Room", "small")
            .defaultValue("notindicated");

        misc.param<bool>("stereo_rematrixing", "Stereo Rematrixing")
            .description("Enables/Disables use of rematrixing for stereo input. This is an optional AC-3 feature that increases quality by selectively encoding the left/right channels as mid/side. This option is enabled by default, and it is highly recommended that it be left as enabled except for testing purposes.")
            .defaultValue(true);

        misc.param<bool>("channel_coupling", "Channel Coupling")
            .description("Enables/Disables use of channel coupling, which is an optional AC-3 feature that increases quality by combining high frequency information from multiple channels into a single channel. The per-channel high frequency information is sent with less accuracy in both the frequency and time domains. This allows more bits to be used for lower frequencies while preserving enough information to reconstruct the high frequencies. This option is enabled by default for the floating-point encoder and should generally be left as enabled except for testing purposes or to increase encoding speed.")
            .defaultValue(true);

        misc.param<int>("mixing_level", "Mixing Level")
            .description("Specifies peak sound pressure level (SPL) in the production environment when the mix was mastered. Valid values are 80 to 111, or -1 for unknown or not indicated. The default value is -1, but that value cannot be used if the Audio Production Information is written to the bitstream. Therefore, if the room_type option is not the default value, the mixing_level option must not be -1.")
            .minValue(80)
            .maxValue(111)
            .defaultValue(80);

        misc.param<int>("cpl_start_band", "Coupling Start Band")
            .description("Sets the channel coupling start band, from 1 to 15. If a value higher than the bandwidth is used, it will be reduced to 1 less than the coupling end band. If auto is used, the start band will be determined by the encoder based on the bit rate, sample rate, and channel layout. This option has no effect if channel coupling is disabled.")
            .minValue(1)
            .maxValue(15)
            .defaultValue(1);

        return registerAsset(info);
    }

    int FFmpegEncodersPlugin::registerFFv1()
    {
        AssetInfo info;
        info.id = "ffv1";
        info.name = "FFv1";
        info.category = std::make_pair("ffmpeg", "FFmpeg");
        info.description = "FFmpegs very own FFv1 encoder";
        info.type = NodeInfoType::encoder;
        info.mediaType = MediaType::video;
        info.helpUrl = "https://ffmpeg.org/ffmpeg-codecs.html#" + info.id;

        info.format("YUV 420", "yuv420p")
            .format("YUV 422", "yuv422p")
            .format("YUV 444", "yuv444p")
            .format("YUV 420 (10 bit)", "yuv420p10le")
            .format("YUV 422 (10 bit)", "yuv422p10le")
            .format("YUV 444 (10 bit)", "yuv444p10le")
            .format("YUV 420 (12 bit)", "yuv420p12le")
            .format("YUV 422 (12 bit)", "yuv422p12le")
            .format("YUV 444 (12 bit)", "yuv444p12le")
            .format("YUV 420 (14 bit)", "yuv420p14le")
            .format("YUV 422 (14 bit)", "yuv422p14le")
            .format("YUV 444 (14 bit)", "yuv444p14le")
            .format("YUV 420 (16 bit)", "yuv420p16le")
            .format("YUV 422 (16 bit)", "yuv422p16le")
            .format("YUV 444 (16 bit)", "yuv444p16le")
            .format("RGBA", "bgra")
            .format("RGBA (16 bit)", "rgba64le");

        auto& frames = info.group("frames", "Frames", ItemParamGroupType::Standard);
        frames.param<int>("g", "GOP-size")
            .description("For archival use, GOP-size should be '1'.")
            .minValue(0)
            .maxValue(600)
            .defaultValue(250);

        auto& slices = info.group("slices", "Slices", ItemParamGroupType::Standard);
        slices.param<int>("slices", "Slices")
            .description("Each frame is split into this number of slices. This affects multithreading performance, as well as filesize: Increasing the number of slices might speed up performance, but also increases the filesize.")
            .option("4", 4)
            .option("6", 6)
            .option("9", 9)
            .option("12", 12)
            .option("16", 16)
            .option("24", 24)
            .option("30", 30)
            .defaultValue(12);

        slices.param<bool>("slicecrc", "Create a checksum for each slice.")
            .description("Enabling this option adds CRC information to each slice. This makes it possible for a decoder to detect errors in the bitstream, rather than blindly decoding a broken slice.")
            .defaultValue(false);

        auto& misc = info.group("misc", "Miscellaneous", ItemParamGroupType::Standard);
        misc.param<int>("coder", "Coder")
            .description("Coder")
            .option("Golomb-Rice", 0)
            .option("Range Coder", 1)
            .option("Range Coder (with custom state transition table)", 2)
            .defaultValue(0);

        misc.param<int>("context", "Context")
            .description("Context")
            .option("Small", 0)
            .option("Large", 1)
            .defaultValue(0);

        return registerAsset(info);
    }

    int FFmpegEncodersPlugin::registerFFvhuff()
    {
        AssetInfo info;
        info.id = "ffvhuff";
        info.name = "Huffyuv";
        info.category = std::make_pair("ffmpeg", "FFmpeg");
        info.description = "FFmpegs extended Huffyuv encoder";
        info.type = NodeInfoType::encoder;
        info.mediaType = MediaType::video;
        info.helpUrl = "https://ffmpeg.org/ffmpeg-codecs.html#" + info.id;

        info.format("YUV 420", "yuv420p")
            .format("YUV 422", "yuv422p")
            .format("YUV 444", "yuv444p")
            .format("YUV 420 (Alpha)", "yuva420p")
            .format("YUV 422 (Alpha)", "yuva422p")
            .format("YUV 444 (Alpha)", "yuva444p")
            .format("YUV 420 (10 bit)", "yuv420p10le")
            .format("YUV 422 (10 bit)", "yuv422p10le")
            .format("YUV 444 (10 bit)", "yuv444p10le")
            .format("YUV 420 (10 bit, Alpha)", "yuva420p10le")
            .format("YUV 422 (10 bit, Alpha)", "yuva422p10le")
            .format("YUV 444 (10 bit, Alpha)", "yuva444p10le")
            .format("YUV 420 (12 bit)", "yuv420p12le")
            .format("YUV 422 (12 bit)", "yuv422p12le")
            .format("YUV 444 (12 bit)", "yuv444p12le")
            .format("YUV 420 (14 bit)", "yuv420p14le")
            .format("YUV 422 (14 bit)", "yuv422p14le")
            .format("YUV 444 (14 bit)", "yuv444p14le")
            .format("YUV 420 (16 bit)", "yuv420p16le")
            .format("YUV 422 (16 bit)", "yuv422p16le")
            .format("YUV 444 (16 bit)", "yuv444p16le")
            .format("YUV 420 (16 bit, Alpha)", "yuva420p16le")
            .format("YUV 422 (16 bit, Alpha)", "yuva422p16le")
            .format("YUV 444 (16 bit, Alpha)", "yuva444p16le")
            .format("RGB", "rgb24")
            .format("RGB (Alpha)", "bgra");

        auto& advanced = info.group("advanced", "Advanced", ItemParamGroupType::Standard);
        advanced.param<bool>("non_deterministic", "Non deterministic")
            .description("Allow multithreading for e.g. context=1 at the expense of determinism")
            .defaultValue(false);

        advanced.param<int>("pred", "Prediction method")
            .description("Prediction method")
            .option("Left", 0)
            .option("Plane", 1)
            .option("Median", 2)
            .defaultValue(0);

        advanced.param<int>("context", "Context")
            .description("Set per-frame huffman tables")
            .minValue(0)
            .maxValue(1)
            .defaultValue(0);

        return registerAsset(info);
    }

    int FFmpegEncodersPlugin::registerFLAC()
    {
        AssetInfo info;
        info.id = "flac";
        info.name = "FLAC";
        info.category = std::make_pair("ffmpeg", "FFmpeg");
        info.description = "FFmpegs built-in FLAC encoder";
        info.type = NodeInfoType::encoder;
        info.mediaType = MediaType::audio;
        info.helpUrl = "https://ffmpeg.org/ffmpeg-codecs.html#" + info.id;

        info.format("16 bit", "s16")
            .format("32 bit", "s32");

        auto& basic = info.group("basic", "Basic", ItemParamGroupType::Forced);
        basic.param<int>("compression_level", "Compression")
            .description("The constant bit rate in kbit/s for all channels.")
            .minValue(0)
            .maxValue(12)
            .defaultValue(5);

        auto& advanced = info.group("advanced", "Advanced", ItemParamGroupType::Standard);
        advanced.param<int>("lpc_coeff_precision", "LPC coefficient precision")
            .description("LPC coefficient precision")
            .minValue(0)
            .maxValue(15)
            .defaultValue(15);

        advanced.param<int>("lpc_type", "LPC algorithm")
            .description("LPC type/algorithm")
            .option("(Disabled)", -1)
            .option("None", 0)
            .option("Fixed", 1)
            .option("Levinson", 2)
            .option("Cholesky", 3)
            .defaultValue(-1);

        advanced.param<int>("lpc_passes", "LPC passes")
            .description("Number of passes to use for Cholesky factorization during LPC analysis")
            .minValue(0)
            .maxValue(INT_MAX)
            .defaultValue(2);

        advanced.param<int>("min_partition_order", "Min. partition order")
            .description("Min. partition order")
            .minValue(-1)
            .maxValue(8)
            .defaultValue(-1);

        advanced.param<int>("max_partition_order", "Max. partition order")
            .description("Max. partition order")
            .minValue(-1)
            .maxValue(8)
            .defaultValue(-1);

        advanced.param<int>("prediction_order_method", "Prediction order method")
            .description("Search method for selecting prediction order")
            .option("(Disabled)", -1)
            .option("Estimation", 0)
            .option("2-level", 1)
            .option("4-level", 2)
            .option("8-level", 3)
            .option("Full search", 4)
            .option("Log search", 5)
            .defaultValue(-1);

        advanced.param<int>("ch_mode", "Channel mode")
            .description("Stereo decorrelation mode")
            .option("Automatic", -1)
            .option("Independent", 0)
            .option("Left side", 1)
            .option("Right side", 2)
            .option("Mid side", 3)
            .defaultValue(-1);

        advanced.param<bool>("exact_rice_parameters", "Exact rice params")
            .description("Calculate rice parameters exactly")
            .defaultValue(false);

        advanced.param<bool>("multi_dim_quant", "Multi-dimensional quantization")
            .description("Multi-dimensional quantization")
            .defaultValue(false);

        advanced.param<int>("min_prediction_order", "Min. prediction order")
            .description("Min. prediction order")
            .minValue(-1)
            .maxValue(32)
            .defaultValue(-1);

        advanced.param<int>("max_prediction_order", "Max. prediction order")
            .description("Max. prediction order")
            .minValue(-1)
            .maxValue(32)
            .defaultValue(-1);

        return registerAsset(info);
    }

    int FFmpegEncodersPlugin::registerFLV1()
    {
        AssetInfo info;
        info.id = "flv";
        info.name = "FLV";
        info.category = std::make_pair("ffmpeg", "FFmpeg");
        info.description = "Sorenson Spark / Sorenson H.263 (Flash Video)";
        info.type = NodeInfoType::encoder;
        info.mediaType = MediaType::video;
        info.helpUrl = "https://ffmpeg.org/ffmpeg-codecs.html#" + info.id;

        info.format("YUV 420", "yuv420p");

        // -mpv_flags         <flags>      E..V.......Flags common for all mpegvideo - based encoders. (default 0)
        //     skip_rd                      E..V.......RD optimal MB level residual skipping
        //     strict_gop                   E..V.......Strictly enforce gop size
        //     qp_rd                        E..V.......Use rate distortion optimization for qp selection
        //     cbp_rd                       E..V.......use rate distortion optimization for CBP
        //     naq                          E..V.......normalize adaptive quantization
        //     mv0                          E..V.......always try a mb with mv = <0, 0>
        // -luma_elim_threshold <int>        E..V.......single coefficient elimination threshold for luminance(negative values also consider dc coefficient) (from INT_MIN to INT_MAX) (default 0)
        // - chroma_elim_threshold <int>        E..V.......single coefficient elimination threshold for chrominance(negative values also consider dc coefficient) (from INT_MIN to INT_MAX) (default 0)
        // - quantizer_noise_shaping <int>        E..V....... (from 0 to INT_MAX) (default 0)
        // - error_rate        <int>        E..V.......Simulate errors in the bitstream to test error concealment. (from 0 to INT_MAX) (default 0)
        // - qsquish           <float>      E..V.......how to keep quantizer between qmin and qmax(0 = clip, 1 = use differentiable function) (from 0 to 99) (default 0)
        // - rc_qmod_amp       <float>      E..V.......experimental quantizer modulation(from - FLT_MAX to FLT_MAX) (default 0)
        // - rc_qmod_freq      <int>        E..V.......experimental quantizer modulation(from INT_MIN to INT_MAX) (default 0)
        // - rc_eq             <string>     E..V.......Set rate control equation.When computing the expression, besides the standard functions defined in the section 'Expression Evaluation', the following functions are available : bits2qp(bits), qp2bits(qp).Also the following constants are available : iTex pTex tex mv fCode iCount mcVar var isI isP isB avgQP qComp avgIITex avgPITex avgPPTex avgBPTex avgTex.
        // - rc_init_cplx      <float>      E..V.......initial complexity for 1 - pass encoding(from - FLT_MAX to FLT_MAX) (default 0)
        // - rc_buf_aggressivity <float>      E..V.......currently useless(from - FLT_MAX to FLT_MAX) (default 1)
        // - border_mask       <float>      E..V.......increase the quantizer for macroblocks close to borders(from - FLT_MAX to FLT_MAX) (default 0)
        // - lmin              <int>        E..V.......minimum Lagrange factor(VBR) (from 0 to INT_MAX) (default 236)
        // - lmax              <int>        E..V.......maximum Lagrange factor(VBR) (from 0 to INT_MAX) (default 3658)
        // - skip_threshold    <int>        E..V.......Frame skip threshold(from INT_MIN to INT_MAX) (default 0)
        // - skip_factor       <int>        E..V.......Frame skip factor(from INT_MIN to INT_MAX) (default 0)
        // - skip_exp          <int>        E..V.......Frame skip exponent(from INT_MIN to INT_MAX) (default 0)
        // - skip_cmp          <int>        E..V.......Frame skip compare function(from INT_MIN to INT_MAX) (default dctmax)
        //     sad             0            E..V.......Sum of absolute differences, fast
        //     sse             1            E..V.......Sum of squared errors
        //     satd            2            E..V.......Sum of absolute Hadamard transformed differences
        //     dct             3            E..V.......Sum of absolute DCT transformed differences
        //     psnr            4            E..V.......Sum of squared quantization errors, low quality
        //     bit             5            E..V.......Number of bits needed for the block
        //     rd              6            E..V.......Rate distortion optimal, slow
        //     zero            7            E..V.......Zero
        //     vsad            8            E..V.......Sum of absolute vertical differences
        //     vsse            9            E..V.......Sum of squared vertical differences
        //     nsse            10           E..V.......Noise preserving sum of squared differences
        //     dct264          14           E..V.......
        //     dctmax          13           E..V.......
        //     chroma          256          E..V.......
        //     msad            15           E..V.......Sum of absolute differences, median predicted
        // - sc_threshold      <int>        E..V.......Scene change threshold(from INT_MIN to INT_MAX) (default 0)
        // - noise_reduction   <int>        E..V.......Noise reduction(from INT_MIN to INT_MAX) (default 0)
        // - ps                <int>        E..V.......RTP payload size in bytes(from INT_MIN to INT_MAX) (default 0)
        // - motion_est        <int>        E..V.......motion estimation algorithm(from 0 to 2) (default epzs)
        //     zero            0            E..V.......
        //     epzs            1            E..V.......
        //     xone            2            E..V.......
        // - mepc              <int>        E..V.......Motion estimation bitrate penalty compensation(1.0 = 256) (from INT_MIN to INT_MAX) (default 256)
        // - mepre             <int>        E..V.......pre motion estimation(from INT_MIN to INT_MAX) (default 0)
        // - intra_penalty     <int>        E..V.......Penalty for intra blocks in block decision(from 0 to 1.07374e+09) (default 0)

        return registerAsset(info);
    }

    int FFmpegEncodersPlugin::registerGIF()
    {
        AssetInfo info;
        info.id = "gif";
        info.name = "Animated GIF";
        info.category = std::make_pair("ffmpeg", "FFmpeg");
        info.description = "FFmpegs built-in GIF encoder";
        info.type = NodeInfoType::encoder;
        info.mediaType = MediaType::video;
        info.helpUrl = "https://ffmpeg.org/ffmpeg-codecs.html#" + info.id;

        info.format("RGB", "rgb8");

        auto& advanced = info.group("advanced", "Advanced", ItemParamGroupType::Standard);
        advanced.param<flags>("gifflags", "GIF flags")
            .description("GIF flags")
            .flag("offsetting")
            .flag("transdiff")
            .defaultValue(3);

        advanced.param<bool>("gifimage", "Image frames only")
            .description("Enable encoding only images per frame")
            .defaultValue(false);

        advanced.param<bool>("global_palette", "Global palette")
            .description("Write a palette to the global gif header where feasible")
            .defaultValue(true);

        return registerAsset(info);
    }

    int FFmpegEncodersPlugin::registerHAP()
    {
        AssetInfo info;
        info.id = "hap";
        info.name = "HAP";
        info.category = std::make_pair("ffmpeg", "FFmpeg");
        info.description = "FFmpegs built-in HAP encoder";
        info.type = NodeInfoType::encoder;
        info.mediaType = MediaType::video;
        info.helpUrl = "https://ffmpeg.org/ffmpeg-codecs.html#" + info.id;

        info.format("RGBA", "rgba");

        auto& standard = info.group("standard", "Standard", ItemParamGroupType::Forced);
        standard.param<std::string>("format", "Format")
            .description("Specifies the Hap format to encode.")
            .option("HAP", "hap")
            .option("HAP Alpha", "hap_alpha")
            .option("HAP Q", "hap_q")
            .defaultValue("hap");

        standard.param<int>("chunks", "Chunks")
            .description("Specifies the number of chunks to split frames into, between 1 and 64. This permits multithreaded decoding of large frames, potentially at the cost of data-rate. The encoder may modify this value to divide frames evenly.")
            .minValue(1)
            .maxValue(64)
            .defaultValue(1);

        standard.param<std::string>("compressor", "Compressor")
            .description("Specifies the second-stage compressor to use. If set to none, chunks will be limited to 1, as chunked uncompressed frames offer no benefit.")
            .option("None", "none")
            .option("Snappy", "snappy")
            .defaultValue("snappy");

        return registerAsset(info);
    }

    int FFmpegEncodersPlugin::registerMP2()
    {
        AssetInfo info;
        info.id = "mp2";
        info.name = "MP2";
        info.category = std::make_pair("ffmpeg", "FFmpeg");
        info.description = "FFmpegs built-in MP2 encoder";
        info.type = NodeInfoType::encoder;
        info.mediaType = MediaType::audio;
        info.helpUrl = "https://ffmpeg.org/ffmpeg-codecs.html#" + info.id;

        info.format("16 bit", "s16");

        auto& basic = info.group("basic", "Basic", ItemParamGroupType::Forced);
        basic.param<int>("b", "Bit rate", 1)
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

        return registerAsset(info);
    }

    int FFmpegEncodersPlugin::registerMPEG2Video()
    {
        AssetInfo info;
        info.id = "mpeg2video";
        info.name = "MPEG-2 Video";
        info.category = std::make_pair("ffmpeg", "FFmpeg");
        info.description = "FFmpegs built-in MPEG-2 video encoder";
        info.type = NodeInfoType::encoder;
        info.mediaType = MediaType::video;
        info.helpUrl = "https://ffmpeg.org/ffmpeg-codecs.html#" + info.id;

        info.format("YUV 420", "yuv420p")
            .format("YUV 422", "yuv422p");

        // Find encoder by name
        const AVCodec* codec = avcodec_find_encoder_by_name(info.id.c_str());
        if (codec)
            createFFmpegParameters(info, codec->priv_class);

        return registerAsset(info);
    }

    int FFmpegEncodersPlugin::registerPCM16le()
    {
        AssetInfo info;
        info.id = "pcm_s16le";
        info.name = "PCM (16 bit)";
        info.category = std::make_pair("ffmpeg", "FFmpeg");
        info.description = "FFmpegs built-in 16 bit PCM encoder (little endian)";
        info.type = NodeInfoType::encoder;
        info.mediaType = MediaType::audio;
        info.helpUrl = "https://ffmpeg.org/ffmpeg-codecs.html#" + info.id;

        info.format("16 bit", "s16");

        return registerAsset(info);
    }

    int FFmpegEncodersPlugin::registerPCM24le()
    {
        AssetInfo info;
        info.id = "pcm_s24le";
        info.name = "PCM (24 bit)";
        info.category = std::make_pair("ffmpeg", "FFmpeg");
        info.description = "FFmpegs built-in PCM encoder";
        info.type = NodeInfoType::encoder;
        info.mediaType = MediaType::audio;
        info.helpUrl = "https://ffmpeg.org/ffmpeg-codecs.html#" + info.id;

        info.format("24 bit", "s32");

        return registerAsset(info);
    }

    int FFmpegEncodersPlugin::registerPCM32le()
    {
        AssetInfo info;
        info.id = "pcm_s32le";
        info.name = "PCM (32 bit)";
        info.category = std::make_pair("ffmpeg", "FFmpeg");
        info.description = "FFmpegs built-in PCM encoder";
        info.type = NodeInfoType::encoder;
        info.mediaType = MediaType::audio;
        info.helpUrl = "https://ffmpeg.org/ffmpeg-codecs.html#" + info.id;

        info.format("32 bit", "s32");

        return registerAsset(info);
    }

    int FFmpegEncodersPlugin::registerProResKS()
    {
        AssetInfo info;
        info.id = "prores_ks";
        info.name = "ProRes KS";
        info.category = std::make_pair("ffmpeg", "FFmpeg");
        info.description = "FFmpegs built-in ProRes KS encoder";
        info.type = NodeInfoType::encoder;
        info.mediaType = MediaType::video;
        info.helpUrl = "https://ffmpeg.org/ffmpeg-codecs.html#" + info.id;

        info.format("YUV 422 (10 bit)", "yuv422p10le")
            .format("YUV 444 (10 bit)", "yuv444p10le")
            .format("YUVA 444 (10 bit)", "yuva444p10le");

        auto& basic = info.group("basic", "Basic", ItemParamGroupType::Forced);
        basic.param<std::string>("profile", "Profile")
            .description("Constraints the bitrate range and controls other properties such as compression algorithm and chroma format.")
            .option("Auto", "auto")
            .option("Proxy", "proxy")
            .option("LT", "lt")
            .option("Standard", "standard")
            .option("High Quality", "hq")
            .option("4444", "4444")
            .option("4444 XQ", "4444xq")
            .defaultValue("auto");

        basic.param<std::string>("quant_mat", "Quantizer Matrix")
            .description("Quantizer Matrices for different purposes.")
            .option("Auto", "auto")
            .option("Proxy", "proxy")
            .option("LT", "lt")
            .option("Standard", "standard")
            .option("High Quality", "hq")
            .option("Default", "default")
            .defaultValue("auto");

        auto& frametype = info.group("frametype", "Frame type", ItemParamGroupType::Standard);
        frametype.param<int>("mbs_per_slice", "Macroblocks per Slice")
            .description("Amount of Macroblocks per slice. Higher value increases quality.")
            .option("1", 1)
            .option("2", 2)
            .option("4", 4)
            .option("8", 8)
            .defaultValue(8);

        frametype.param<int>("bits_per_mb", "Bits per Macroblock")
            .description("Amount of bits per Macroblock.")
            .minValue(0)
            .maxValue(8192)
            .defaultValue(0);

        auto& misc = info.group("misc", "Miscellaneous", ItemParamGroupType::Standard);
        misc.param<int>("alpha_bits", "Alpha channel size")
            .description("Amount of bits for the alpha channel.")
            .option("1 bit", 1)
            .option("8 bits", 8)
            .option("16 bits", 16)
            .defaultValue(16);

        misc.param<std::string>("vendor", "Vendor")
            .description("FFMpeg usually identifies as 'Lavc'.")
            .defaultValue("apl0");

        return registerAsset(info);
    }

    int FFmpegEncodersPlugin::registerTrueHD()
    {
        AssetInfo info;
        info.id = "truehd";
        info.name = "TrueHD";
        info.category = std::make_pair("ffmpeg", "FFmpeg");
        info.description = "FFmpegs built-in TrueHD encoder";
        info.type = NodeInfoType::encoder;
        info.mediaType = MediaType::audio;
        info.helpUrl = "https://ffmpeg.org/ffmpeg-codecs.html#" + info.id;

        info.format("16 bit", "s16")
            .format("32 bit", "s32");

        return registerAsset(info);
    }

    int FFmpegEncodersPlugin::registerUTVideo()
    {
        AssetInfo info;
        info.id = "utvideo";
        info.name = "UT Video";
        info.category = std::make_pair("ffmpeg", "FFmpeg");
        info.description = "FFmpegs very own UTVideo encoder";
        info.type = NodeInfoType::encoder;
        info.mediaType = MediaType::video;
        info.helpUrl = "https://ffmpeg.org/ffmpeg-codecs.html#" + info.id;

        info.format("YUV 420", "yuv420p")
            .format("YUV 422", "yuv422p")
            .format("YUV 444", "yuv444p");

        auto& basic = info.group("basic", "Basic", ItemParamGroupType::Forced);
        basic.param<std::string>("pred", "Prediction method")
            .description("Prediction method")
            .option("Left", "left")
            .option("Gradient", "gradient")
            .option("Median", "median")
            .defaultValue("left");

        return registerAsset(info);
    }

    int FFmpegEncodersPlugin::registerQuickTimeRLE()
    {
        AssetInfo info;
        info.id = "qtrle";
        info.name = "QT RLE";
        info.category = std::make_pair("ffmpeg", "FFmpeg");
        info.description = "FFmpegs very own QuickTime Animation (RLE) encoder";
        info.type = NodeInfoType::encoder;
        info.mediaType = MediaType::video;
        info.helpUrl = "https://ffmpeg.org/ffmpeg-codecs.html#" + info.id;

        info.format("RGBA", "argb");

        return registerAsset(info);
    }

    int FFmpegEncodersPlugin::registerWavPack()
    {
        AssetInfo info;
        info.id = "wavpack";
        info.name = "WavPack";
        info.category = std::make_pair("ffmpeg", "FFmpeg");
        info.description = "FFmpegs built-in WavPack encoder";
        info.type = NodeInfoType::encoder;
        info.mediaType = MediaType::audio;
        info.helpUrl = "https://ffmpeg.org/ffmpeg-codecs.html#" + info.id;

        info.format("Floating Point", "fltp")
            .format("8 bit", "u8p")
            .format("16 bit", "s16p")
            .format("32 bit", "s32p");

        auto& basic = info.group("basic", "Basic", ItemParamGroupType::Forced);
        basic.param<int>("compression_level", "Compression level")
            .description("Compression level")
            .minValue(0)
            .maxValue(8)
            .defaultValue(1);

        auto& advanced = info.group("advanced", "Advanced", ItemParamGroupType::Standard);
        advanced.param<double>("frame_size", "Frame size")
            .description("For this encoder, the range for this option is between 128 and 131072. Default is automatically decided based on sample rate and number of channel.")
            .minValue(128)
            .maxValue(131072)
            .defaultValue(0);

        advanced.param<bool>("joint_stereo", "Intensity stereo coding")
            .description("Intensity stereo coding")
            .defaultValue(true);

        advanced.param<bool>("optimize_mono", "Optimize mono")
            .description("Optimize mono")
            .defaultValue(false);

        return registerAsset(info);
    }

    int FFmpegEncodersPlugin::registerVP9()
    {
        AssetInfo info;
        info.id = "libvpx-vp9";
        info.name = "VP9";
        info.category = std::make_pair("libvpx", "LibVPX");
        info.description = "The LibVPX VP9 encoder";
        info.type = NodeInfoType::encoder;
        info.mediaType = MediaType::video;
        info.helpUrl = "https://ffmpeg.org/ffmpeg-codecs.html#" + info.id;

        info.format("YUV 420", "yuv420p")
            .format("YUVA 420 (with Alpha)", "yuva420p")
            .format("YUV 422", "yuv422p")
            .format("YUV 444", "yuv444p")
            .format("YUV 420 (10 bit)", "yuv420p10le")
            .format("YUV 422 (10 bit)", "yuv422p10le")
            .format("YUV 444 (10 bit)", "yuv444p10le")
            .format("YUV 420 (12 bit)", "yuv420p12le")
            .format("YUV 422 (12 bit)", "yuv422p12le")
            .format("YUV 444 (12 bit)", "yuv444p12le");

        auto& basic = info.group("basic", "Basic", ItemParamGroupType::Forced);
        basic.param<std::string>("_strategy", "Strategy")
            .description("The encoding strategy to use.")
            .option("Average Bitrate (ABR)", "", [](const ItemParamAction& action) {
                action.setVisible("basic.b", true);
                action.setVisible("basic.crf", false);
            })
            .option("Constant Quality (CRF)", "crf", [](const ItemParamAction& action) {
                action.setVisible("basic.b", false);
                action.setVisible("basic.crf", true);
            })
            .option("Constrained Quality (CQ)", "bitrate_crf", [](const ItemParamAction& action) {
                action.setVisible("basic.b", true);
                action.setVisible("basic.crf", true);
            })
            .defaultValue("crf");

        basic.param<int>("b", "Average Bitrate [kbit]", 1)
            .description("The average data rate allowed by the encoder.")
            .minValue(0)
            .maxValue(288000)
            .defaultValue(15000);

        basic.param<int>("crf", "Constant Rate Factor", 1)
            .description("Lower values mean better quality but increase the processing time.")
            .minValue(4)
            .maxValue(63)
            .defaultValue(10);

        basic.param<std::string>("quality", "Quality")
            .description("Good is the default and recommended for most applications. Best is recommended if you have lots of time and want the best compression efficiency. Realtime is recommended for live / fast encoding.")
            .option("Good", "good")
            .option("Best", "best")
            .option("Realtime", "realtime")
            .defaultValue("good");

        basic.param<int>("cpu-used", "CPU used")
            .description("Quality/Speed ratio modifier")
            .minValue(0)
            .maxValue(8)
            .defaultValue(1);

        auto& frame = info.group("frame", "Frame Type", ItemParamGroupType::Standard);
        frame.param<int>("g", "Max. GOP Size")
            .description("Maximum keyframe interval (frames)")
            .minValue(0)
            .maxValue(600)
            .defaultValue(250);

        frame.param<bool>("auto-alt-ref", "Alternate reference frames")
            .description("Enable use of alternate reference frames (2-pass only)")
            .defaultValue(false);

        frame.param<int>("threads", "Threads")
            .description("Number of max. threads")
            .minValue(1)
            .maxValue(24)
            .defaultValue(8);

        frame.param<bool>("frame-parallel", "Frame Parallel")
            .description("Enable frame parallel decodability features.")
            .defaultValue(false);

        frame.param<int>("lag-in-frames", "Look ahead frames")
            .description("Number of frames to look ahead for when encoding, 0 no limit")
            .minValue(0)
            .maxValue(25)
            .defaultValue(0);

        frame.param<int>("tile-columns", "Tile Columns")
            .description("Number of tile columns to use, log2")
            .minValue(-1)
            .maxValue(6)
            .defaultValue(6);

        frame.param<int>("tile-rows", "Tile Rows")
            .description("Number of tile rows to use, log2")
            .minValue(-1)
            .maxValue(2)
            .defaultValue(2);

        auto& misc = info.group("misc", "Miscellaneous", ItemParamGroupType::Standard);
        misc.param<int>("sharpness", "Sharpness")
            .description("Anything above 0 weakens the deblocking effect of the loop filter.")
            .minValue(0)
            .maxValue(7)
            .defaultValue(0);

        return registerAsset(info);
    }
}
