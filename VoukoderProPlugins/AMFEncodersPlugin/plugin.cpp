#include "plugin.h"

namespace VoukoderPro
{
    AMFEncodersPlugin::AMFEncodersPlugin() : EncoderPlugin()
    {
        registerH264();
        registerHEVC();
        registerAV1();
    }

    int AMFEncodersPlugin::registerH264()
    {
        AssetInfo info;
        info.id = "h264_amf";
        info.name = "AMF H.264";
        info.category = std::make_pair("amd", "AMD");
        info.description = "AMDs GPU accelerated h264 encoder.";
        info.type = NodeInfoType::encoder;
        info.mediaType = MediaType::video;
        info.helpUrl = "";

        info.format("YUV 420", "yuv420p");

        auto& basic = info.group("basic", "Basic", ItemParamGroupType::Forced);
        basic.param<std::string>("usage", "Usage")
            .description("Usage")
            .option("Transcoding", "transcoding")
            .option("Ultra low latency", "ultralowlatency")
            .option("Low latency", "lowlatency")
            .option("Webcam", "webcam")
            .defaultValue("transcoding");

        basic.param<std::string>("profile", "Profile")
            .description("Profile")
            .option("Main", "main")
            .option("High", "high")
            .option("Constrained Baseline", "constrained_baseline")
            .option("Constrained High", "constrained_high")
            .defaultValue("main");

        basic.param<std::string>("quality", "Quality")
            .description("Quality")
            .option("Speed", "speed")
            .option("Balanced", "balanced")
            .option("Quality", "quality")
            .defaultValue("speed");

        basic.param<std::string>("rc", "Strategy")
            .description("The encoding mode to use. Choose Constant Bitrate (CBR), Constant Quantizer (CQP) or Variable Bitrate (VBR) and if two encoding passes should be used (VBR only).")
            .option("Constant Quantizer (CQP)", "cqp", [](const ItemParamAction& action) {
                action.setVisible("basic.qp_i", true);
                action.setVisible("basic.qp_p", true);
                action.setVisible("basic.qp_b", true);
                action.setVisible("basic.b", false);
            })
            .option("Constant Bitrate (CBR)", "cbr", [](const ItemParamAction& action) {
                action.setVisible("basic.qp_i", false);
                action.setVisible("basic.qp_p", false);
                action.setVisible("basic.qp_b", false);
                action.setVisible("basic.b", true);
            })
            .option("Peak Contrained Variable Bitrate (VBR Peak)", "vbr_peak", [](const ItemParamAction& action) {
                action.setVisible("basic.qp_i", false);
                action.setVisible("basic.qp_p", false);
                action.setVisible("basic.qp_b", false);
                action.setVisible("basic.b", true);
            })
            .option("Latency Contrained Variable Bitrate (VBR Latency)", "vbr_latency", [](const ItemParamAction& action) {
                action.setVisible("basic.qp_i", false);
                action.setVisible("basic.qp_p", false);
                action.setVisible("basic.qp_b", false);
                action.setVisible("basic.b", true);
            })
            .defaultValue("cqp");

        basic.param<int>("qp_i", "Quant. parameter (I-Frame)", 1)
            .description("Quantization Parameter for I-Frame")
            .minValue(0)
            .maxValue(51)
            .defaultValue(22);

        basic.param<int>("qp_p", "Quant. parameter (P-Frame)", 1)
            .description("Quantization Parameter for P-Frame")
            .minValue(0)
            .maxValue(51)
            .defaultValue(22);

        basic.param<int>("qp_b", "Quant. parameter (B-Frame)", 1)
            .description("Quantization Parameter for B-Frame")
            .minValue(0)
            .maxValue(51)
            .defaultValue(22);

        basic.param<int>("b", "Bitrate [kbit]", 1)
            .description("The data rate allowed by the encoder.")
            .minValue(0)
            .maxValue(512000)
            .multiplierValue(1000)
            .defaultValue(15000);

        auto& frames = info.group("frames", "Frames", ItemParamGroupType::Standard);
        frames.param<int>("g", "Max. GOP Size")
            .description("Maximum Group of Pictures Structure size in which intra- and inter-frames are arranged. Smaller values increase the video quality.")
            .minValue(1)
            .maxValue(1200)
            .defaultValue(300);

        auto& quantizer = info.group("quantizer", "Quantizer", ItemParamGroupType::Standard);
        quantizer.param<int>("bf_delta_qp", "B-Picture Delta QP")
            .description("B-Picture Delta QP")
            .minValue(-10)
            .maxValue(10)
            .defaultValue(4);

        quantizer.param<bool>("bf_ref", "B-Frames Refs")
            .description("Enable Reference to B-Frames")
            .defaultValue(true);

        quantizer.param<bool>("vbaq", "Enable VBAQ")
            .description("Enable VBAQ")
            .defaultValue(false);

        auto& analysis = info.group("analysis", "Analysis", ItemParamGroupType::Standard);
        analysis.param<bool>("me_half_pel", "Enable ME Half Pixel")
            .description("Enable ME Half Pixel")
            .defaultValue(true);

        analysis.param<bool>("me_quarter_pel", "Enable ME Quarter Pixel")
            .description("Enable ME Quarter Pixel")
            .defaultValue(true);

        analysis.param<bool>("frame_skipping", "Rate Control Based Frame Skip")
            .description("Rate Control Based Frame Skip")
            .defaultValue(false);

        analysis.param<bool>("preanalysis", "Pre-Analysis Mode")
            .description("Pre-Analysis Mode")
            .defaultValue(false);

        analysis.param<int>("max_au_size", "Max. AU size")
            .description("Maximum Access Unit Size for rate control (in bits)")
            .minValue(0)
            .maxValue(9999)
            .defaultValue(0);

        analysis.param<int>("intra_refresh_mb", "Intra Refresh MB")
            .description("Intra Refresh MBs Number Per Slot in Macroblocks")
            .minValue(0)
            .maxValue(9999)
            .defaultValue(0);

        auto& misc = info.group("misc", "Miscellaneous", ItemParamGroupType::Standard);
        misc.param<std::string>("level", "Level")
            .description("Constrains encoding parameters such as bitrate range and maximum frame size.")
            .option("1", "1.0")
            .option("1.1", "1.1")
            .option("1.2", "1.2")
            .option("1.3", "1.3")
            .option("2", "2.0")
            .option("2.1", "2.1")
            .option("2.2", "2.2")
            .option("3", "3.0")
            .option("3.1", "3.1")
            .option("3.2", "3.2")
            .option("4", "4.0")
            .option("4.1", "4.1")
            .option("4.2", "4.2")
            .option("5", "5.0")
            .option("5.1", "5.1")
            .option("5.2", "5.2")
            .option("6", "6.0")
            .option("6.1", "6.1")
            .option("6.2", "6.2")
            .defaultValue("1.0");

        misc.param<bool>("filler_data", "Filler Data Enable")
            .description("Filler Data Enable")
            .defaultValue(false);

        misc.param<std::string>("coder", "Coding Type")
            .description("Coding Type")
            .option("CABAC", "cabac")
            .option("CAVLC", "cavlc")
            .defaultValue("cabac");

        misc.param<int>("header_spacing", "Header Insertion Spacing")
            .description("Header Insertion Spacing")
            .minValue(0)
            .maxValue(1000)
            .defaultValue(0);

        misc.param<bool>("aud", "AUD")
            .description("Inserts AU Delimiter NAL unit")
            .defaultValue(false);

        misc.param<bool>("enforce_hrd", "Enforce HRD")
            .description("Enforce HRD")
            .defaultValue(false);

        return registerAsset(info);
    }

    int AMFEncodersPlugin::registerHEVC()
    {
        AssetInfo info;
        info.id = "hevc_amf";
        info.name = "AMF HEVC";
        info.category = std::make_pair("amd", "AMD");
        info.description = "AMDs GPU accelerated hevc encoder.";
        info.type = NodeInfoType::encoder;
        info.mediaType = MediaType::video;
        info.helpUrl = "";

        info.format("YUV 420", "yuv420p");

        auto& basic = info.group("basic", "Basic", ItemParamGroupType::Forced);
        basic.param<std::string>("usage", "Usage")
            .description("Usage")
            .option("Transcoding", "transcoding")
            .option("Ultra low latency", "ultralowlatency")
            .option("Low latency", "lowlatency")
            .option("Webcam", "webcam")
            .defaultValue("transcoding");

        basic.param<std::string>("profile", "Profile")
            .description("Profile")
            .option("Main", "main")
            .defaultValue("main");

        basic.param<std::string>("profile_tier", "Tier")
            .description("Tier")
            .option("Main", "main")
            .option("High", "high")
            .defaultValue("main");

        basic.param<std::string>("quality", "Quality")
            .description("Quality")
            .option("Speed", "speed")
            .option("Balanced", "balanced")
            .option("Quality", "quality")
            .defaultValue("speed");

        basic.param<std::string>("rc", "Strategy")
            .description("The encoding mode to use. Choose Constant Bitrate (CBR), Constant Quantizer (CQP) or Variable Bitrate (VBR) and if two encoding passes should be used (VBR only).")
            .option("Constant Quantizer (CQP)", "cqp", [](const ItemParamAction& action) {
                action.setVisible("basic.qp_i", true);
                action.setVisible("basic.qp_p", true);
                action.setVisible("basic.b", false);
            })
            .option("Constant Bitrate (CBR)", "cbr", [](const ItemParamAction& action) {
                action.setVisible("basic.qp_i", false);
                action.setVisible("basic.qp_p", false);
                action.setVisible("basic.b", true);
            })
            .option("Peak Contrained Variable Bitrate (VBR Peak)", "vbr_peak", [](const ItemParamAction& action) {
                action.setVisible("basic.qp_i", false);
                action.setVisible("basic.qp_p", false);
                action.setVisible("basic.b", true);
            })
            .option("Latency Contrained Variable Bitrate (VBR Latency)", "vbr_latency", [](const ItemParamAction& action) {
                action.setVisible("basic.qp_i", false);
                action.setVisible("basic.qp_p", false);
                action.setVisible("basic.b", true);
            })
            .defaultValue("cqp");

        basic.param<int>("qp_i", "Quant. parameter (I-Frame)", 1)
            .description("Quantization Parameter for I-Frame")
            .minValue(0)
            .maxValue(51)
            .defaultValue(22);

        basic.param<int>("qp_p", "Quant. parameter (P-Frame)", 1)
            .description("Quantization Parameter for P-Frame")
            .minValue(0)
            .maxValue(51)
            .defaultValue(22);

        basic.param<int>("b", "Bitrate [kbit]", 1)
            .description("The data rate allowed by the encoder.")
            .minValue(0)
            .maxValue(512000)
            .multiplierValue(1000)
            .defaultValue(15000);

        auto& frames = info.group("frames", "Frames", ItemParamGroupType::Standard);
        frames.param<int>("g", "Max. GOP Size")
            .description("Maximum Group of Pictures Structure size in which intra- and inter-frames are arranged. Smaller values increase the video quality.")
            .minValue(1)
            .maxValue(1200)
            .defaultValue(300);

        auto& quantizer = info.group("quantizer", "Quantizer", ItemParamGroupType::Standard);
        quantizer.param<int>("min_qp_i", "Min. QPI")
            .description("Minimum Quantization Parameter for I-Frame")
            .minValue(0)
            .maxValue(51)
            .defaultValue(0);

        quantizer.param<int>("max_qp_i", "Max. QPI")
            .description("Maximum Quantization Parameter for I-Frame")
            .minValue(0)
            .maxValue(51)
            .defaultValue(0);

        quantizer.param<int>("min_qp_p", "Min. QPP")
            .description("Minimum Quantization Parameter for P-Frame")
            .minValue(0)
            .maxValue(51)
            .defaultValue(0);

        quantizer.param<int>("max_qp_p", "Max. QPP")
            .description("Maximum Quantization Parameter for P-Frame")
            .minValue(0)
            .maxValue(51)
            .defaultValue(0);

        quantizer.param<bool>("vbaq", "Enable VBAQ")
            .description("Enable VBAQ")
            .defaultValue(false);

        auto& analysis = info.group("analysis", "Analysis", ItemParamGroupType::Standard);
        analysis.param<bool>("me_half_pel", "Enable ME Half Pixel")
            .description("Enable ME Half Pixel")
            .defaultValue(true);

        analysis.param<bool>("me_quarter_pel", "Enable ME Quarter Pixel")
            .description("Enable ME Quarter Pixel")
            .defaultValue(true);

        analysis.param<bool>("skip_frame", "Rate Control Based Frame Skip")
            .description("Rate Control Based Frame Skip")
            .defaultValue(false);

        analysis.param<bool>("preanalysis", "Pre-Analysis Mode")
            .description("Pre-Analysis Mode")
            .defaultValue(false);

        analysis.param<int>("max_au_size", "Max. AU size")
            .description("Maximum Access Unit Size for rate control (in bits)")
            .minValue(0)
            .maxValue(9999)
            .defaultValue(0);

        auto& misc = info.group("misc", "Miscellaneous", ItemParamGroupType::Standard);
        misc.param<std::string>("level", "Level")
            .description("Constrains encoding parameters such as bitrate range and maximum frame size.")
            .option("1", "1.0")
            .option("2", "2.0")
            .option("2.1", "2.1")
            .option("3", "3.0")
            .option("3.1", "3.1")
            .option("4", "4.0")
            .option("4.1", "4.1")
            .option("5", "5.0")
            .option("5.1", "5.1")
            .option("5.2", "5.2")
            .option("6", "6.0")
            .option("6.1", "6.1")
            .option("6.2", "6.2")
            .defaultValue("1.0");

        misc.param<std::string>("header_insertion_mode", "Header Insertion Mode")
            .description("Header Insertion Mode")
            .option("GOP", "gop")
            .option("IDR", "idr")
            .defaultValue("gop");

        misc.param<int>("gops_per_idr", "GOPs per IDR")
            .description("GOPs per IDR 0-no IDR will be inserted")
            .minValue(0)
            .maxValue(9999)
            .defaultValue(60);

        misc.param<bool>("filler_data", "Filler Data Enable")
            .description("Filler Data Enable")
            .defaultValue(false);

        misc.param<bool>("aud", "AUD")
            .description("Inserts AU Delimiter NAL unit")
            .defaultValue(false);

        misc.param<bool>("enforce_hrd", "Enforce HRD")
            .description("Enforce HRD")
            .defaultValue(false);

        return registerAsset(info);
    }

    int AMFEncodersPlugin::registerAV1()
    {
        AssetInfo info;
        info.id = "av1_amf";
        info.name = "AMF AV1";
        info.category = std::make_pair("amd", "AMD");
        info.description = "AMDs GPU accelerated AV1 encoder.";
        info.type = NodeInfoType::encoder;
        info.mediaType = MediaType::video;
        info.helpUrl = "";

        info.format("YUV 420", "yuv420p");

        auto& basic = info.group("basic", "Basic", ItemParamGroupType::Forced);
        basic.param<std::string>("usage", "Usage")
            .description("Usage")
            .option("Transcoding", "transcoding")
            .option("Low latency", "lowlatency")
            .defaultValue("transcoding");

        basic.param<std::string>("profile", "Profile")
            .description("Profile")
            .option("Main", "main")
            .defaultValue("main");

        basic.param<std::string>("quality", "Quality")
            .description("Quality")
            .option("Speed", "speed")
            .option("Balanced", "balanced")
            .option("Quality", "quality")
            .option("High Quality", "high_quality")
            .defaultValue("speed");

        basic.param<std::string>("rc", "Strategy")
            .description("The encoding mode to use. Choose Constant Bitrate (CBR), Constant Quantizer (CQP) or Variable Bitrate (VBR) and if two encoding passes should be used (VBR only).")
            .option("Constant Quantization Parameter", "cqp", [](const ItemParamAction& action)
                {
                    action.setVisible("basic.qp_i", true);
                    action.setVisible("basic.qp_p", true);
                    action.setVisible("basic.b", false);
                })
            .option("Constant Bitrate", "cbr", [](const ItemParamAction& action)
                {
                    action.setVisible("basic.qp_i", false);
                    action.setVisible("basic.qp_p", false);
                    action.setVisible("basic.b", true);
                })
            .option("Peak Constrained Variable Bitrate", "vbr_peak", [](const ItemParamAction& action)
                {
                    action.setVisible("basic.qp_i", false);
                    action.setVisible("basic.qp_p", false);
                    action.setVisible("basic.b", true);
                })
            .option("Latency Constrained Variable Bitrate", "vbr_latency", [](const ItemParamAction& action)
                {
                    action.setVisible("basic.qp_i", false);
                    action.setVisible("basic.qp_p", false);
                    action.setVisible("basic.b", true);
                })
            .defaultValue("cqp");

        basic.param<int>("qp_i", "Quant. parameter (I-Frame)", 1)
            .description("Quantization Parameter for I-Frame")
            .minValue(0)
            .maxValue(255)
            .defaultValue(110);

        basic.param<int>("qp_p", "Quant. parameter (P-Frame)", 1)
            .description("Quantization Parameter for P-Frame")
            .minValue(0)
            .maxValue(255)
            .defaultValue(110);

        basic.param<int>("b", "Bitrate [kbit]", 1)
            .description("The data rate allowed by the encoder.")
            .minValue(0)
            .maxValue(512000)
            .multiplierValue(1000)
            .defaultValue(15000);

        auto& frames = info.group("frames", "Frames", ItemParamGroupType::Standard);
        frames.param<int>("g", "Max. GOP Size")
            .description("Maximum Group of Pictures Structure size in which intra- and inter-frames are arranged. Smaller values increase the video quality.")
            .minValue(1)
            .maxValue(1200)
            .defaultValue(300);

        auto& quantizer = info.group("quantizer", "Quantizer", ItemParamGroupType::Standard);
        quantizer.param<int>("min_qp_i", "Min. QPI")
            .description("Minimum Quantization Parameter for I-Frame")
            .minValue(0)
            .maxValue(255)
            .defaultValue(0);

        quantizer.param<int>("max_qp_i", "Max. QPI")
            .description("Maximum Quantization Parameter for I-Frame")
            .minValue(0)
            .maxValue(255)
            .defaultValue(0);

        quantizer.param<int>("min_qp_p", "Min. QPP")
            .description("Minimum Quantization Parameter for P-Frame")
            .minValue(0)
            .maxValue(255)
            .defaultValue(0);

        quantizer.param<int>("max_qp_p", "Max. QPP")
            .description("Maximum Quantization Parameter for P-Frame")
            .minValue(0)
            .maxValue(255)
            .defaultValue(0);

        auto& analysis = info.group("analysis", "Analysis", ItemParamGroupType::Standard);
        analysis.param<bool>("skip_frame", "Rate Control Based Frame Skip")
            .description("Rate Control Based Frame Skip")
            .defaultValue(false);

        analysis.param<bool>("preanalysis", "Pre-Analysis Mode")
            .description("Pre-Analysis Mode")
            .defaultValue(false);

        auto& misc = info.group("misc", "Miscellaneous", ItemParamGroupType::Standard);
        misc.param<std::string>("level", "Level")
            .description("Constrains encoding parameters such as bitrate range and maximum frame size.")
            .option("2.0", "2.0")
            .option("2.1", "2.1")
            .option("2.2", "2.2")
            .option("2.3", "2.3")
            .option("3.0", "3.0")
            .option("3.1", "3.1")
            .option("3.2", "3.2")
            .option("3.3", "3.3")
            .option("4.0", "4.0")
            .option("4.1", "4.1")
            .option("4.2", "4.2")
            .option("4.3", "4.3")
            .option("5.0", "5.0")
            .option("5.1", "5.1")
            .option("5.2", "5.2")
            .option("5.3", "5.3")
            .option("6.0", "6.0")
            .option("6.1", "6.1")
            .option("6.2", "6.2")
            .option("6.3", "6.3")
            .option("7.0", "7.0")
            .option("7.1", "7.1")
            .option("7.2", "7.2")
            .option("7.3", "7.3")
            .defaultValue("2.0");

        misc.param<bool>("filler_data", "Filler Data Enable")
            .description("Filler Data Enable")
            .defaultValue(false);

        misc.param<bool>("enforce_hrd", "Enforce HRD")
            .description("Enforce HRD")
            .defaultValue(false);

        misc.param<std::string>("align", "Align")
            .description("Alignment mode")
            .option("64x16", "64x16")
            .option("1080p", "1080p")
            .option("none", "none")
            .defaultValue("none");

        misc.param<std::string>("header_insertion_mode", "Header insertion mode")
            .description("Set header insertion mode")
            .option("none", "none")
            .option("Gop", "gop")
            .option("Frame", "frame")
            .defaultValue("none");

        return registerAsset(info);
    }
}