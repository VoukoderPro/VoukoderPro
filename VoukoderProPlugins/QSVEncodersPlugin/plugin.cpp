#include "plugin.h"

namespace VoukoderPro
{
    QSVEncodersPlugin::QSVEncodersPlugin(): EncoderPlugin()
    {
        registerH264();
        registerHEVC();
        registerVP9();
        registerAV1();
    }

    int QSVEncodersPlugin::registerH264()
    {
        AssetInfo info;
        info.id = "h264_qsv";
        info.name = "QSV H.264";
        info.category = std::make_pair("intel", "Intel");
        info.description = "Intels GPU accelerated h264 encoder.";
        info.type = NodeInfoType::encoder;
        info.mediaType = MediaType::video;
        info.helpUrl = "";

        info.format("YUV 420", "nv12")
            .format("YUV 422 (10 bit)", "y210le");

        auto& basic = info.group("basic", "Basic", ItemParamGroupType::Forced);
        basic.param<std::string>("_strategy", "Strategy")
            .ignore(true)
            .description("The rate control method.")
            .option("Constant Quantizer", "cqp", [](const ItemParamAction& action) {
                action.setVisible("basic.global_quality", true);
                action.setVisible("basic.b", false);
                action.setVisible("basic.maxrate", false);
                action.setVisible("basic.avbr_accuracy", false);
                action.setVisible("basic.avbr_convergence", false);
                action.setProperty("qscale", "1");
                action.setProperty("look_ahead", "");
            })
            .option("Intelligent Constant Quality (w. lookahead)", "la_cqp", [](const ItemParamAction& action) {
                action.setVisible("basic.global_quality", true);
                action.setVisible("basic.b", false);
                action.setVisible("basic.maxrate", false);
                action.setVisible("basic.avbr_accuracy", false);
                action.setVisible("basic.avbr_convergence", false);
                action.setProperty("qscale", "1");
                action.setProperty("look_ahead", "1");
            })
            .option("Intelligent Constant Quality", "icq", [](const ItemParamAction& action) {
                action.setVisible("basic.global_quality", true);
                action.setVisible("basic.b", false);
                action.setVisible("basic.maxrate", false);
                action.setVisible("basic.avbr_accuracy", false);
                action.setVisible("basic.avbr_convergence", false);
                action.setProperty("qscale", "");
                action.setProperty("look_ahead", "");
            })
            .option("VBR with lookahead", "la", [](const ItemParamAction& action) {
                action.setVisible("basic.global_quality", false);
                action.setVisible("basic.b", true);
                action.setVisible("basic.maxrate", true);
                action.setVisible("basic.avbr_accuracy", false);
                action.setVisible("basic.avbr_convergence", false);
                action.setProperty("qscale", "");
                action.setProperty("look_ahead", "1");
            })
            .option("CBR / VBR", "cbr/vbr", [](const ItemParamAction& action) {
                action.setVisible("basic.global_quality", false);
                action.setVisible("basic.b", true);
                action.setVisible("basic.maxrate", true);
                action.setVisible("basic.avbr_accuracy", false);
                action.setVisible("basic.avbr_convergence", false);
                action.setProperty("qscale", "");
                action.setProperty("look_ahead", "");
            })
            .option("Average VBR", "avbr", [](const ItemParamAction& action) {
                action.setVisible("basic.global_quality", false);
                action.setVisible("basic.b", true);
                action.setVisible("basic.maxrate", false);
                action.setVisible("basic.avbr_accuracy", true);
                action.setVisible("basic.avbr_convergence", true);
                action.setProperty("qscale", "");
                action.setProperty("look_ahead", "");
            })
            .defaultValue("la_cqp");

        basic.param<int>("global_quality", "Quality", 1)
            .description("Quality setting (1-51).")
            .minValue(1)
            .maxValue(51)
            .defaultValue(25);

        basic.param<int>("b", "Bitrate [kbit]", 1)
            .description("The data rate allowed by the encoder.")
            .minValue(100)
            .maxValue(512000)
            .multiplierValue(1000)
            .defaultValue(15000);

        basic.param<int>("maxrate", "Max. Bitrate [kbit]", 1)
            .description("Higher value can improve maximum quality, but increases decoder requirements.")
            .minValue(100)
            .maxValue(512000)
            .multiplierValue(1000)
            .defaultValue(15000);

        basic.param<int>("avbr_accuracy", "AVBR Accuracy", 1)
            .description("Accuracy of the AVBR ratecontrol")
            .minValue(0)
            .maxValue(1000)
            .defaultValue(0);

        basic.param<int>("avbr_convergence", "AVBR Convergence", 1)
            .description("Convergence of the AVBR ratecontrol")
            .minValue(0)
            .maxValue(99999)
            .defaultValue(0);

        basic.param<std::string>("preset", "Preset")
            .description("Preset")
            .option("Very fast", "veryfast")
            .option("Faster", "faster")
            .option("Fast", "fast")
            .option("Medium", "medium")
            .option("Slow", "slow")
            .option("Slower", "slower")
            .option("Very slow", "veryslow")
            .defaultValue("medium");

        basic.param<std::string>("profile", "Profile")
            .description("Profile")
            .option("None", "unknown")
            .option("Baseline", "baseline")
            .option("Main", "main")
            .option("High", "high")
            .defaultValue("unknown");

        auto& advanced = info.group("advanced", "Advanced", ItemParamGroupType::Standard);
        advanced.param<int>("look_ahead_depth", "Lookahead depth")
            .description("Depth of look ahead in number frames")
            .minValue(0)
            .maxValue(100)
            .defaultValue(0);

        advanced.param<std::string>("look_ahead_downsampling", "Lookahead downsampling")
            .description("Downscaling factor for the frames saved for the lookahead analysis")
            .option("auto", "auto")
            .option("off", "off")
            .option("2x", "2x")
            .option("4x", "4x")
            .defaultValue("auto");

        advanced.param<int>("async_depth", "Async depth")
            .description("Maximum processing parallelism")
            .minValue(1)
            .maxValue(2147483647)
            .defaultValue(1);

        advanced.param<bool>("rdo", "RDO")
            .description("Enable rate distortion optimization")
            .defaultValue(true);

        advanced.param<int>("max_frame_size", "Max. frame size")
            .description("Maximum encoded frame size in bytes")
            .minValue(-1)
            .maxValue(65535)
            .defaultValue(-1);

        advanced.param<int>("max_slice_size", "Max. slice size")
            .description("Maximum encoded slice size in bytes")
            .minValue(-1)
            .maxValue(65535)
            .defaultValue(-1);

        advanced.param<bool>("bitrate_limit", "Bitrate limit")
            .description("Toggle bitrate limitations")
            .defaultValue(true);

        advanced.param<bool>("mbbrc", "MB BRC")
            .description("MB level bitrate control")
            .defaultValue(true);

        advanced.param<bool>("extbrc", "Extended BRC")
            .description("Extended bitrate control")
            .defaultValue(true);

        advanced.param<bool>("adaptive_i", "Adaptive I")
            .description("Adaptive I-frame placement")
            .defaultValue(true);

        advanced.param<bool>("adaptive_b", "Adaptive B")
            .description("Adaptive B-frame placement")
            .defaultValue(true);

        advanced.param<bool>("b_strategy", "B Strategy")
            .description("Strategy to choose between I/P/B-frames")
            .defaultValue(true);

        advanced.param<bool>("forced_idr", "Forced IDR")
            .description("Forcing I frames as IDR frames")
            .defaultValue(true);

        advanced.param<bool>("low_power", "Low power")
            .description("enable low power mode(experimental: many limitations by mfx version, BRC modes, etc.)")
            .defaultValue(true);

        advanced.param<bool>("cavlc", "CAVLC")
            .description("Enable CAVLC")
            .defaultValue(true);

        advanced.param<int>("idr_interval", "IDR interval")
            .description("Distance (in I-frames) between IDR frames")
            .minValue(0)
            .maxValue(2147483647)
            .defaultValue(0);

        advanced.param<bool>("pic_timing_sei", "PIC timing SEI")
            .description("Insert picture timing SEI with pic_struct_syntax element")
            .defaultValue(true);

        advanced.param<bool>("single_sei_nal_unit", "Single SEI NAL unit")
            .description("Put all the SEI messages into one NALU")
            .defaultValue(true);

        advanced.param<int>("max_dec_frame_buffering", "Max. decoded frame buffering")
            .description("Maximum number of frames buffered in the DPB")
            .minValue(0)
            .maxValue(65535)
            .defaultValue(0);

        advanced.param<std::string>("int_ref_type", "Intra refresh type")
            .description("Intra refresh type")
            .option("vertical", "vertical")
            .defaultValue("vertical");

        advanced.param<int>("int_ref_cycle_size", "Intra refresh cycle size")
            .description("Number of frames in the intra refresh cycle")
            .minValue(0)
            .maxValue(65535)
            .defaultValue(0);

        advanced.param<int>("int_ref_qp_delta", "Intra refresh QP delta")
            .description("QP difference for the refresh MBs")
            .minValue(-32768)
            .maxValue(32767)
            .defaultValue(-32768);

        advanced.param<bool>("recovery_point_sei", "Recovery point sei")
            .description("Insert recovery point SEI messages")
            .defaultValue(true);

        advanced.param<bool>("a53cc", "A53CC")
            .description("Use A53 Closed Captions (if available)")
            .defaultValue(true);

        advanced.param<bool>("aud", "AUS")
            .description("Insert the Access Unit Delimiter NAL")
            .defaultValue(true);

        advanced.param<bool>("repeat_pps", "Repeat pps")
            .description("repeat pps for every frame")
            .defaultValue(true);

        return registerAsset(info);
    }

    int QSVEncodersPlugin::registerHEVC()
    {
        AssetInfo info;
        info.id = "hevc_qsv";
        info.name = "QSV HEVC";
        info.category = std::make_pair("intel", "Intel");
        info.description = "Intels GPU accelerated HEVC encoder.";
        info.type = NodeInfoType::encoder;
        info.mediaType = MediaType::video;
        info.helpUrl = "";

        info.format("YUV 420", "nv12")
            .format("YUV 420 (10 bit)", "p010le");

        auto& basic = info.group("basic", "Basic", ItemParamGroupType::Forced);
        basic.param<std::string>("_strategy", "Strategy").ignore(true)
            .description("The rate control method.")
            .option("Constant Quantizer", "cqp", [](const ItemParamAction& action) {
                action.setVisible("basic.global_quality", true);
                action.setVisible("basic.b", false);
                action.setVisible("basic.maxrate", false);
                action.setVisible("basic.avbr_accuracy", false);
                action.setVisible("basic.avbr_convergence", false);
                action.setProperty("qscale", "1");
                action.setProperty("look_ahead", "");
            })
            .option("Intelligent Constant Quality (w. lookahead)", "la_cqp", [](const ItemParamAction& action) {
                action.setVisible("basic.global_quality", true);
                action.setVisible("basic.b", false);
                action.setVisible("basic.maxrate", false);
                action.setVisible("basic.avbr_accuracy", false);
                action.setVisible("basic.avbr_convergence", false);
                action.setProperty("qscale", "1");
                action.setProperty("look_ahead", "1");
            })
            .option("Intelligent Constant Quality", "icq", [](const ItemParamAction& action) {
                action.setVisible("basic.global_quality", true);
                action.setVisible("basic.b", false);
                action.setVisible("basic.maxrate", false);
                action.setVisible("basic.avbr_accuracy", false);
                action.setVisible("basic.avbr_convergence", false);
                action.setProperty("qscale", "");
                action.setProperty("look_ahead", "");
            })
            .option("VBR with lookahead", "la", [](const ItemParamAction& action) {
                action.setVisible("basic.global_quality", false);
                action.setVisible("basic.b", true);
                action.setVisible("basic.maxrate", true);
                action.setVisible("basic.avbr_accuracy", false);
                action.setVisible("basic.avbr_convergence", false);
                action.setProperty("qscale", "");
                action.setProperty("look_ahead", "1");
            })
            .option("CBR / VBR", "cbr/vbr", [](const ItemParamAction& action) {
                action.setVisible("basic.global_quality", false);
                action.setVisible("basic.b", true);
                action.setVisible("basic.maxrate", true);
                action.setVisible("basic.avbr_accuracy", false);
                action.setVisible("basic.avbr_convergence", false);
                action.setProperty("qscale", "");
                action.setProperty("look_ahead", "");
            })
            .option("Average VBR", "avbr", [](const ItemParamAction& action) {
                action.setVisible("basic.global_quality", false);
                action.setVisible("basic.b", true);
                action.setVisible("basic.maxrate", false);
                action.setVisible("basic.avbr_accuracy", true);
                action.setVisible("basic.avbr_convergence", true);
                action.setProperty("qscale", "");
                action.setProperty("look_ahead", "");
            })
            .defaultValue("la_cqp");

        basic.param<int>("global_quality", "Quality", 1)
            .description("Quality setting (1-51).")
            .minValue(1)
            .maxValue(51)
            .defaultValue(25);

        basic.param<int>("b", "Bitrate [kbit]", 1)
            .description("The data rate allowed by the encoder.")
            .minValue(100)
            .maxValue(512000)
            .multiplierValue(1000)
            .defaultValue(15000);

        basic.param<int>("maxrate", "Max. Bitrate [kbit]", 1)
            .description("Higher value can improve maximum quality, but increases decoder requirements.")
            .minValue(100)
            .maxValue(512000)
            .multiplierValue(1000)
            .defaultValue(15000);

        basic.param<int>("avbr_accuracy", "AVBR Accuracy", 1)
            .description("Accuracy of the AVBR ratecontrol")
            .minValue(0)
            .maxValue(1000)
            .defaultValue(0);

        basic.param<int>("avbr_convergence", "AVBR Convergence", 1)
            .description("Convergence of the AVBR ratecontrol")
            .minValue(0)
            .maxValue(99999)
            .defaultValue(0);

        basic.param<std::string>("preset", "Preset")
            .description("Preset")
            .option("Very fast", "veryfast")
            .option("Faster", "faster")
            .option("Fast", "fast")
            .option("Medium", "medium")
            .option("Slow", "slow")
            .option("Slower", "slower")
            .option("Very slow", "veryslow")
            .defaultValue("medium");

        basic.param<std::string>("profile", "Profile")
            .description("Profile")
            .option("None", "unknown")
            .option("Main", "main")
            .option("Main 10", "main10")
            .option("Main SP", "mainsp")
            .defaultValue("unknown");

        auto& advanced = info.group("advanced", "Advanced", ItemParamGroupType::Standard);
        advanced.param<int>("async_depth", "Async depth")
            .description("Maximum processing parallelism")
            .minValue(1)
            .maxValue(2147483647)
            .defaultValue(1);

        advanced.param<bool>("rdo", "RDO")
            .description("Enable rate distortion optimization")
            .defaultValue(true);

        advanced.param<int>("max_frame_size", "Max. frame size")
            .description("Maximum encoded frame size in bytes")
            .minValue(-1)
            .maxValue(65535)
            .defaultValue(-1);

        advanced.param<int>("max_slice_size", "Max. slice size")
            .description("Maximum encoded slice size in bytes")
            .minValue(-1)
            .maxValue(65535)
            .defaultValue(-1);

        advanced.param<bool>("bitrate_limit", "Bitrate limit")
            .description("Toggle bitrate limitations")
            .defaultValue(true);

        advanced.param<bool>("mbbrc", "MB BRC")
            .description("MB level bitrate control")
            .defaultValue(true);

        advanced.param<bool>("extbrc", "Extended BRC")
            .description("Extended bitrate control")
            .defaultValue(true);

        advanced.param<bool>("adaptive_i", "Adaptive I")
            .description("Adaptive I-frame placement")
            .defaultValue(true);

        advanced.param<bool>("adaptive_b", "Adaptive B")
            .description("Adaptive B-frame placement")
            .defaultValue(true);

        advanced.param<bool>("b_strategy", "B Strategy")
            .description("Strategy to choose between I/P/B-frames")
            .defaultValue(true);

        advanced.param<bool>("forced_idr", "Forced IDR")
            .description("Forcing I frames as IDR frames")
            .defaultValue(true);

        advanced.param<bool>("low_power", "Low power")
            .description("enable low power mode(experimental: many limitations by mfx version, BRC modes, etc.)")
            .defaultValue(true);

        advanced.param<int>("idr_interval", "IDR interval")
            .description("Distance (in I-frames) between IDR frames")
            .minValue(0)
            .maxValue(2147483647)
            .defaultValue(0);

        advanced.param<int>("gpb", "GPB")
            .description("1: GPB (generalized P/B frame); 0: regular P frame (default 1)")
            .option("Regular P frame", 0)
            .option("GPB (generalized P/B frame)", 1)
            .defaultValue(1);

        return registerAsset(info);
    }

    int QSVEncodersPlugin::registerAV1()
    {
        AssetInfo info;
        info.id = "av1_qsv";
        info.name = "QSV AV1";
        info.category = std::make_pair("intel", "Intel");
        info.description = "Intels GPU accelerated AV1 encoder.";
        info.type = NodeInfoType::encoder;
        info.mediaType = MediaType::video;
        info.helpUrl = "";

        info.format("YUV 420", "nv12")
            .format("YUV 420 (10 bit)", "p010le");

        auto& basic = info.group("basic", "Basic", ItemParamGroupType::Forced);
        basic.param<std::string>("strategy", "Strategy")
            .ignore(true)
            .description("The rate control method.")
            .option("Constant Quantizer", "cqp", [](const ItemParamAction& action)
                {
                    action.setVisible("basic.global_quality", true);
                    action.setVisible("basic.b", false);
                    action.setVisible("basic.maxrate", false);
                    action.setVisible("basic.avbr_accuracy", false);
                    action.setVisible("basic.avbr_convergence", false);
                    action.setProperty("qscale", "1");
                    action.setProperty("look_ahead", "");
                })
            .option("Intelligent Constant Quality (w. lookahead)", "la_cqp", [](const ItemParamAction& action)
                {
                    action.setVisible("basic.global_quality", true);
                    action.setVisible("basic.b", false);
                    action.setVisible("basic.maxrate", false);
                    action.setVisible("basic.avbr_accuracy", false);
                    action.setVisible("basic.avbr_convergence", false);
                    action.setProperty("qscale", "1");
                    action.setProperty("look_ahead", "1");
                })
            .option("Intelligent Constant Quality", "icq", [](const ItemParamAction& action)
                {
                    action.setVisible("basic.global_quality", true);
                    action.setVisible("basic.b", false);
                    action.setVisible("basic.maxrate", false);
                    action.setVisible("basic.avbr_accuracy", false);
                    action.setVisible("basic.avbr_convergence", false);
                    action.setProperty("qscale", "");
                    action.setProperty("look_ahead", "");
                })
            .option("VBR with lookahead", "la", [](const ItemParamAction& action)
                {
                    action.setVisible("basic.global_quality", false);
                    action.setVisible("basic.b", true);
                    action.setVisible("basic.maxrate", true);
                    action.setVisible("basic.avbr_accuracy", false);
                    action.setVisible("basic.avbr_convergence", false);
                    action.setProperty("qscale", "");
                    action.setProperty("look_ahead", "1");
                })
            .option("CBR / VBR", "cbr/vbr", [](const ItemParamAction& action)
                {
                    action.setVisible("basic.global_quality", false);
                    action.setVisible("basic.b", true);
                    action.setVisible("basic.maxrate", true);
                    action.setVisible("basic.avbr_accuracy", false);
                    action.setVisible("basic.avbr_convergence", false);
                    action.setProperty("qscale", "");
                    action.setProperty("look_ahead", "");
                })
            .option("Average VBR", "avbr", [](const ItemParamAction& action)
                {
                    action.setVisible("basic.global_quality", false);
                    action.setVisible("basic.b", true);
                    action.setVisible("basic.maxrate", false);
                    action.setVisible("basic.avbr_accuracy", true);
                    action.setVisible("basic.avbr_convergence", true);
                    action.setProperty("qscale", "");
                    action.setProperty("look_ahead", "");
                })
            .defaultValue("la_cqp");

        basic.param<int>("global_quality", "Quality", 1)
            .description("Quality setting (1-51).")
            .minValue(1)
            .maxValue(51)
            .defaultValue(25);

        basic.param<int>("b", "Bitrate [kbit]", 1)
            .description("The data rate allowed by the encoder.")
            .minValue(100)
            .maxValue(512000)
            .multiplierValue(1000)
            .defaultValue(15000);

        basic.param<int>("maxrate", "Max. Bitrate [kbit]", 1)
            .description("Higher value can improve maximum quality, but increases decoder requirements.")
            .minValue(100)
            .maxValue(512000)
            .multiplierValue(1000)
            .defaultValue(15000);

        basic.param<int>("avbr_accuracy", "AVBR Accuracy", 1)
            .description("Accuracy of the AVBR ratecontrol")
            .minValue(1)
            .maxValue(65535)
            .defaultValue(1);

        basic.param<int>("avbr_convergence", "AVBR Convergence", 1)
            .description("Convergence of the AVBR ratecontrol")
            .minValue(1)
            .maxValue(65535)
            .defaultValue(1);

        basic.param<std::string>("preset", "Preset")
            .description("Preset")
            .option("Very fast", "veryfast")
            .option("Faster", "faster")
            .option("Fast", "fast")
            .option("Medium", "medium")
            .option("Slow", "slow")
            .option("Slower", "slower")
            .option("Very slow", "veryslow")
            .defaultValue("veryfast");

        basic.param<std::string>("profile", "Profile")
            .description("Profile")
            .option("Unknown", "unknown")
            .option("Main", "main")
            .defaultValue("unknown");

        auto& advanced = info.group("advanced", "Advanced", ItemParamGroupType::Standard);
        advanced.param<int>("async_depth", "Async depth")
            .description("Maximum processing parallelism")
            .minValue(1)
            .maxValue(2147483647)
            .defaultValue(1);

        advanced.param<bool>("rdo", "RDO")
            .description("Enable rate distortion optimization")
            .defaultValue(true);

        advanced.param<int>("max_frame_size", "Max. frame size")
            .description("Maximum encoded frame size in bytes")
            .minValue(-1)
            .maxValue(65535)
            .defaultValue(-1);

        advanced.param<int>("max_slice_size", "Max. slice size")
            .description("Maximum encoded slice size in bytes")
            .minValue(-1)
            .maxValue(65535)
            .defaultValue(-1);

        advanced.param<bool>("bitrate_limit", "Bitrate limit")
            .description("Toggle bitrate limitations")
            .defaultValue(true);

        advanced.param<bool>("mbbrc", "MB BRC")
            .description("MB level bitrate control")
            .defaultValue(true);

        advanced.param<bool>("extbrc", "Extended BRC")
            .description("Extended bitrate control")
            .defaultValue(true);

        advanced.param<bool>("adaptive_i", "Adaptive I")
            .description("Adaptive I-frame placement")
            .defaultValue(true);

        advanced.param<bool>("adaptive_b", "Adaptive B")
            .description("Adaptive B-frame placement")
            .defaultValue(true);

        advanced.param<int>("p_strategy", "P Strategy")
            .description("Enable P-pyramid")
            .option("Default", 0)
            .option("Simple", 1)
            .option("Pyramid", 2)
            .defaultValue(0);

        advanced.param<bool>("b_strategy", "B Strategy")
            .description("Strategy to choose between I/P/B-frames")
            .defaultValue(true);

        advanced.param<bool>("forced_idr", "Forced IDR")
            .description("Forcing I frames as IDR frames")
            .defaultValue(true);

        advanced.param<bool>("low_power", "Low power")
            .description("enable low power mode(experimental: many limitations by mfx version, BRC modes, etc.)")
            .defaultValue(true);

        return registerAsset(info);
    }

    int QSVEncodersPlugin::registerVP9()
    {
        AssetInfo info;
        info.id = "vp9_qsv";
        info.name = "QSV VP9";
        info.category = std::make_pair("intel", "Intel");
        info.description = "Intels GPU accelerated VP9 encoder.";
        info.type = NodeInfoType::encoder;
        info.mediaType = MediaType::video;
        info.helpUrl = "";

        info.format("YUV 420", "nv12")
            .format("YUV 420 (10 bit)", "p010le");

        auto& basic = info.group("basic", "Basic", ItemParamGroupType::Forced);
        basic.param<std::string>("strategy", "Strategy")
            .ignore(true)
            .description("The rate control method.")
            .option("Constant Quantizer", "cqp", [](const ItemParamAction& action)
                {
                    action.setVisible("basic.global_quality", true);
                    action.setVisible("basic.b", false);
                    action.setVisible("basic.maxrate", false);
                    action.setProperty("qscale", "1");
                })
            .option("Intelligent Constant Quality", "icq", [](const ItemParamAction& action)
                {
                    action.setVisible("basic.global_quality", true);
                    action.setVisible("basic.b", false);
                    action.setVisible("basic.maxrate", false);
                    action.setProperty("qscale", "");
                })
            .option("CBR / VBR", "cbr/vbr", [](const ItemParamAction& action)
                {
                    action.setVisible("basic.global_quality", false);
                    action.setVisible("basic.b", true);
                    action.setVisible("basic.maxrate", true);
                    action.setProperty("qscale", "");
                })
            .option("Average VBR", "avbr", [](const ItemParamAction& action)
                {
                    action.setVisible("basic.global_quality", false);
                    action.setVisible("basic.b", true);
                    action.setVisible("basic.maxrate", false);
                    action.setProperty("qscale", "");
                })
            .defaultValue("cqp");

        basic.param<int>("global_quality", "Quality", 1)
            .description("Quality setting")
            .minValue(1)
            .maxValue(63)
            .defaultValue(31);

        basic.param<int>("b", "Bitrate [kbit]", 1)
            .description("The data rate allowed by the encoder.")
            .minValue(100)
            .maxValue(512000)
            .multiplierValue(1000)
            .defaultValue(15000);

        basic.param<int>("maxrate", "Max. Bitrate [kbit]", 1)
            .description("Higher value can improve maximum quality, but increases decoder requirements.")
            .minValue(100)
            .maxValue(512000)
            .multiplierValue(1000)
            .defaultValue(15000);

        basic.param<std::string>("preset", "Preset")
            .description("Preset")
            .option("Very fast", "veryfast")
            .option("Faster", "faster")
            .option("Fast", "fast")
            .option("Medium", "medium")
            .option("Slow", "slow")
            .option("Slower", "slower")
            .option("Very slow", "veryslow")
            .defaultValue("veryfast");

        basic.param<std::string>("profile", "Profile")
            .description("Profile")
            .option("Unknown", "unknown")
            .option("Profile 0", "profile0")
            .option("Profile 1", "profile1")
            .option("Profile 2", "profile2")
            .option("Profile 3", "profile3")
            .defaultValue("unknown");

        auto& advanced = info.group("advanced", "Advanced", ItemParamGroupType::Standard);
        advanced.param<int>("async_depth", "Async depth")
            .description("Maximum processing parallelism")
            .minValue(1)
            .maxValue(2147483647)
            .defaultValue(1);

        advanced.param<bool>("forced_idr", "Forced IDR")
            .description("Forcing I frames as IDR frames")
            .defaultValue(true);

        advanced.param<bool>("low_power", "Low power")
            .description("enable low power mode(experimental: many limitations by mfx version, BRC modes, etc.)")
            .defaultValue(true);

        advanced.param<int>("tile_cols", "Tile columns")
            .description("Number of columns for tiled encoding")
            .minValue(0)
            .maxValue(32)
            .defaultValue(0);

        advanced.param<int>("tile_rows", "Tile rows")
            .description("Number of rows for tiled encoding")
            .minValue(0)
            .maxValue(4)
            .defaultValue(0);

        return registerAsset(info);
    }
}