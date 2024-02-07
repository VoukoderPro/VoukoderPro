#include "plugin.h"
#include <Windows.h>


namespace VoukoderPro
{
    NVENCEncodersPlugin::NVENCEncodersPlugin(): EncoderPlugin()
    {
        // Load cuda
        HMODULE cuda = LoadLibraryW(L"nvcuda.dll");
        if (cuda)
        {
            // Functions from nvcuda.dll
            __cuInit cuInit = (__cuInit)GetProcAddress(cuda, "cuInit");
            __cuDeviceGetCount cuDeviceGetCount = (__cuDeviceGetCount)GetProcAddress(cuda, "cuDeviceGetCount");
            __cuDeviceGetName cuDeviceGetName = (__cuDeviceGetName)GetProcAddress(cuda, "cuDeviceGetName");

            cuInit(0);

            // Get number of installed cuda devices
            int deviceCount = 0;
            cuDeviceGetCount(&deviceCount);

            char name[100];
            int len = sizeof(name);

            // Iterate cuda devices and get names and compute capability
            for (int i = 0; i < deviceCount; i++)
            {
                // Get the device name and versions
                if (cuDeviceGetName(name, len, i) == 0)
                    devices.push_back(name);
            }

            // Unload cuda
            FreeLibrary(cuda);
        }

        registerH264();
        registerHEVC();
        registerAV1();
    }

    int NVENCEncodersPlugin::registerH264()
    {
        AssetInfo info;
        info.id = "h264_nvenc";
        info.name = "NVENC H.264";
        info.category = std::make_pair("nvidia", "Nvidia");
        info.description = "NVIDIAs GPU accelerated h264 encoder.";
        info.type = NodeInfoType::encoder;
        info.mediaType = MediaType::video;
        info.helpUrl = "";

        info.preset("Good quality", {
            { "rc", "constq" },
            { "qp", "15" },
            { "preset", "medium" },
            { "profle", "high" }
        });

        info.format("YUV 420", "yuv420p")
            .format("YUV 444", "yuv444p")
            .format("YUV 420 (10 bit)", "p010le")
            .format("CUDA", "cuda");
        
        auto& basic = info.group("basic", "Basic", ItemParamGroupType::Forced);
        addDeviceProperty(basic);

        basic.param<std::string>("preset", "Preset")
            .description("Processing time of each frame. Slower values increase the overall video quality but take more time to process.")
            .option("Fastest (lowest quality)", "p1")
            .option("Faster (lower quality)", "p2")
            .option("Fast (low quality)", "p3")
            .option("Medium", "p4")
            .option("Slow (good quality)", "p5")
            .option("Slower (better quality)", "p6")
            .option("Slowest (best quality)", "p7")
            .defaultValue("p4");

        basic.param<std::string>("profile", "Profile")
            .description("Constraints the bitrate range and controls other properties such as compression algorithm and chroma format.")
            .option("Baseline", "baseline")
            .option("Main", "main")
            .option("High", "high")
            .option("High 4:4:4", "high444p")
            .defaultValue("main");

        basic.param<std::string>("rc", "Strategy")
            .description("")
            .option("Constant QP mode", "constqp", [](const ItemParamAction& action) {
                action.setVisible("basic.qp", true);
                action.setVisible("basic.b", false);
                action.setVisible("basic.maxrate", false);
                action.setVisible("basic.bufsize", false);
                action.setVisible("basic.cq", false);
            })
            .option("Constant bitrate mode", "cbr", [](const ItemParamAction& action) {
                action.setVisible("basic.qp", false);
                action.setVisible("basic.b", true);
                action.setVisible("basic.maxrate", false);
                action.setVisible("basic.bufsize", true);
                action.setVisible("basic.cq", false);
            })
            .option("Variable bitrate mode", "vbr", [](const ItemParamAction& action) {
                action.setVisible("basic.qp", false);
                action.setVisible("basic.b", true);
                action.setVisible("basic.maxrate", true);
                action.setVisible("basic.bufsize", true);
                action.setVisible("basic.cq", true);
            })
            .defaultValue("constqp");

        basic.param<int>("qp", "Quantizer", 1)
            .description("Quality level of the video. Lower values mean better quality but increase file size and processing time.")
            .minValue(0)
            .maxValue(51)
            .defaultValue(23);

        basic.param<int>("b", "Bitrate [kbit/s]", 1)
            .description("The data rate allowed by the encoder.")
            .minValue(0)
            .maxValue(512000)
            .multiplierValue(1000)
            .defaultValue(15000);

        basic.param<int>("maxrate", "Max. Bitrate [kbit/s]", 1)
            .description("Higher value can improve maximum quality, but increases decoder requirements.")
            .minValue(0)
            .maxValue(512000)
            .multiplierValue(1000)
            .defaultValue(15000);

        basic.param<int>("bufsize", "Buffer Size [kbit/s]", 1)
            .description("The encoder bitstream buffer size.")
            .minValue(0)
            .maxValue(512000)
            .multiplierValue(1000)
            .defaultValue(15000);

        basic.param<int>("cq", "Constant quality", 1)
            .description("Set target quality level (0 to 51, 0 means automatic).")
            .minValue(0)
            .maxValue(51)
            .defaultValue(0);

        basic.param<std::string>("multipass", "Multipass")
            .description("Enable multipass encoding.")
            .option("Disabled", "disabled")
            .option("Quarter Resolution", "qres")
            .option("Full Resolution", "fullres")
            .defaultValue("disabled");

        basic.param<std::string>("tune", "Tune")
            .description("Set the encoding tuning info")
            .option("High Quality", "hq")
            .option("Low Latency", "ll")
            .option("Ultra Low Latency", "ull")
            .option("Lossless", "lossless")
            .defaultValue("hq");

        auto& frames = info.group("frames", "Frames", ItemParamGroupType::Standard);
        frames.param<int>("rc-lookahead", "RC Lookahead")
            .description("Sets the number of frames to use for ratecontrol and lookahead. Higher value means better quality and increased precision.")
            .minValue(0)
            .maxValue(1024)
            .defaultValue(0);

        frames.param<bool>("b_adapt", "B Adapt")
            .description("When lookahead is enabled, set this to 0 to disable adaptive B-frame decision")
            .defaultValue(true);

        frames.param<int>("g", "Max. GOP Size")
            .description("Maximum Group of Pictures Structure size in which intra- and inter-frames are arranged. Smaller values increase the video quality.")
            .minValue(1)
            .maxValue(1200)
            .defaultValue(300);

        frames.param<bool>("strict-gop", "Strict GOP")
            .description("Minimizes GOP-to-GOP rate fluctuations.")
            .defaultValue(false);

        frames.param<int>("bf", "Number of B-Frames")
            .description("Set max number of B-frames between non-B-frames. 0 means that Bipredective coded picture frames are disabled. If a value of -1 is used, it will choose an automatic value depending on the encoder.")
            .minValue(0)
            .maxValue(4)
            .defaultValue(2);

        frames.param<int>("refs", "Number of reference frames")
            .description("This parameter lets one specify how many references can be used, through a maximum of 16. Increasing the number of refs increases the DPB (Decoded Picture Buffer) requirement, which means hardware playback devices will often have strict limits to the number of refs they can handle. In live-action sources, more reference have limited use beyond 4-8, but in cartoon sources up to the maximum value of 16 is often useful. More reference frames require more processing power because every frame is searched by the motion search (except when an early skip decision is made). The slowdown is especially apparent with slower motion estimation methods.")
            .minValue(1)
            .maxValue(16)
            .defaultValue(3);

        frames.param<std::string>("b_ref_mode", "Use B frames as references")
            .description("Use B frames as references.")
            .option("Disabled", "disabled")
            .option("Each", "each")
            .option("Middle", "middle")
            .defaultValue("disabled");

        frames.param<bool>("no-scenecut", "Disable scene cuts")
            .description("When lookahead is enabled, set this to 1 to disable adaptive I-frame insertion at scene cuts.")
            .defaultValue(false);

        frames.param<bool>("forced-idr", "Force keyframes as IDR")
            .description("If forcing keyframes, force them as IDR frames.")
            .defaultValue(false);

        frames.param<bool>("nonref_p", "AI P-Frames")
            .description("Enables automatic insertion of non-reference P-frames.")
            .defaultValue(false);

        auto& quantizer = info.group("quantizer", "Quantizer", ItemParamGroupType::Standard);
        quantizer.param<int>("qmin", "Min. Quantizer")
            .description("Lowest allowed quantizer")
            .minValue(0)
            .maxValue(51)
            .defaultValue(0);

        quantizer.param<int>("qmax", "Max. Quantizer")
            .description("Highest allowed quantizer")
            .minValue(0)
            .maxValue(51)
            .defaultValue(0);

        quantizer.param<int>("init_qpI", "QP Value for I-Frames")
            .description("Enable to change quantization parameter for I-frames.")
            .minValue(0)
            .maxValue(51)
            .defaultValue(0);

        quantizer.param<int>("init_qpP", "QP Value for P-Frames")
            .description("Enable to change quantization parameter for P-frames.")
            .minValue(0)
            .maxValue(51)
            .defaultValue(0);

        quantizer.param<int>("init_qpB", "QP Value for B-Frames")
            .description("Enable to change quantization parameter for B-frames.")
            .minValue(0)
            .maxValue(51)
            .defaultValue(0);

        quantizer.param<bool>("spatial-aq", "Spatial AQ")
            .description("Choose AQ Mode how available bits should be distributed between all macroblocks in the video.")
            .defaultValue(false);

        quantizer.param<int>("aq-strength", "Strength")
            .description("AQ strength scale from 1 (low) - 15 (aggressive).")
            .minValue(1)
            .maxValue(15)
            .defaultValue(8);

        quantizer.param<bool>("temporal-aq", "Temporal AQ")
            .description("Choose AQ Mode how available bits should be distributed between all macroblocks in the video.")
            .defaultValue(false);

        auto& misc = info.group("misc", "Miscellaneous", ItemParamGroupType::Standard);
        misc.param<std::string>("level", "Level")
            .description("Constrains encoding parameters such as bitrate range and maximum frame size.")
            .option("1", "1")
            .option("1b", "1b")
            .option("1.1", "1.1")
            .option("1.2", "1.2")
            .option("1.3", "1.3")
            .option("2", "2")
            .option("2.1", "2.1")
            .option("2.2", "2.2")
            .option("3", "3")
            .option("3.1", "3.1")
            .option("3.2", "3.2")
            .option("4", "4")
            .option("4.1", "4.1")
            .option("4.2", "4.2")
            .option("5", "5")
            .option("5.1", "5.1")
            .option("6", "6.0")
            .option("6.1", "6.1")
            .option("6.2", "6.2")
            .defaultValue("1");

        misc.param<std::string>("coder", "Coder")
            .description("Coder type")
            .option("CABAC", "cabac")
            .option("CAVLC", "cavlc")
            .option("AC", "ac")
            .option("VLC", "vlc")
            .defaultValue("cabac");

        misc.param<int>("surfaces", "Surfaces")
            .description("Blockcount of memory allocated for bitmap rendering.")
            .minValue(0)
            .maxValue(64)
            .defaultValue(0);

        misc.param<bool>("zerolatency", "Zero Latency")
            .description("Indicate zero latency operation (no reordering delay).")
            .defaultValue(false);

        misc.param<bool>("weighted_pred", "Weighted Prediction")
            .description("Specifies the use of a scaling and offset, when performing motion compensation. This includes implicit weighted prediction for B-frames, and explicit weighted prediction for P-frames.")
            .defaultValue(false);

        misc.param<bool>("bluray-compat", "Blu-Ray Compatibility")
            .description("Create video files that will be compatible for Blu-Ray discs.")
            .defaultValue(false);

        misc.param<bool>("aud", "Use AUD")
            .description("Access unit delimiter can be used for signaling about start of video frame.")
            .defaultValue(false);

        return registerAsset(info);
    }

    int NVENCEncodersPlugin::registerHEVC()
    {
        AssetInfo info;
        info.id = "hevc_nvenc";
        info.name = "NVENC HEVC";
        info.category = std::make_pair("nvidia", "Nvidia");
        info.description = "NVIDIAs GPU accelerated HEVC encoder.";
        info.type = NodeInfoType::encoder;
        info.mediaType = MediaType::video;
        info.helpUrl = "";

        info.preset("Good quality", {
            { "rc", "constq" },
            { "qp", "15" },
            { "preset", "medium" },
            { "profle", "high" }
        });

        info.format("YUV 420", "yuv420p", [](ItemParamAction& action) {
            action.setVisible("basic.profile.main10", false);
        })
        .format("YUV 444", "yuv444p", [](ItemParamAction& action) {
            action.setVisible("basic.profile.main10", false);
        })
        .format("YUV 420 (10 bit)", "p010le", [](ItemParamAction& action) {
            action.setVisible("basic.profile.main10", true);
        })
        .format("CUDA", "cuda", [](ItemParamAction& action) {
            action.setVisible("basic.profile.main10", false);
        });

        auto& basic = info.group("basic", "Basic", ItemParamGroupType::Forced);
        addDeviceProperty(basic);

        basic.param<std::string>("preset", "Preset")
            .description("Processing time of each frame. Slower values increase the overall video quality but take more time to process.")
            .option("Fastest (lowest quality)", "p1")
            .option("Faster (lower quality)", "p2")
            .option("Fast (low quality)", "p3")
            .option("Medium", "p4")
            .option("Slow (good quality)", "p5")
            .option("Slower (better quality)", "p6")
            .option("Slowest (best quality)", "p7")
            .defaultValue("p4");

        basic.param<std::string>("profile", "Profile")
            .description("Constraints the bitrate range and controls other properties such as compression algorithm and chroma format.")
            .option("Main", "main")
            .option("Main (10 bit)", "main10")
            .option("REXT", "rext")
            .defaultValue("main");

        basic.param<std::string>("tier", "Tier")
            .description("Encoding tier.")
            .option("Main", "main")
            .option("High", "high")
            .defaultValue("main");

        basic.param<std::string>("rc", "Strategy")
            .description("The encoding mode to use. Choose Constant Bitrate (CBR), Constant Quantizer (CQP) or Variable Bitrate (VBR) and if two encoding passes should be used (VBR only).")
            .option("Constant Quantizer (QP)", "constqp", [](const ItemParamAction& action) {
                action.setVisible("basic.qp", true);
                action.setVisible("basic.b", false);
                action.setVisible("basic.maxrate", false);
                action.setVisible("basic.bufsize", false);
                action.setVisible("basic.cq", false);
            })
            .option("Constant Bitrate (CBR)", "cbr", [](const ItemParamAction& action) {
                action.setVisible("basic.qp", false);
                action.setVisible("basic.b", true);
                action.setVisible("basic.maxrate", false);
                action.setVisible("basic.bufsize", true);
                action.setVisible("basic.cq", false);
            })
            .option("Variable Bitrate (VBR)", "vbr", [](const ItemParamAction& action) {
                action.setVisible("basic.qp", false);
                action.setVisible("basic.b", true);
                action.setVisible("basic.maxrate", true);
                action.setVisible("basic.bufsize", true);
                action.setVisible("basic.cq", true);
            })
            .defaultValue("constqp");

        basic.param<int>("qp", "Quantizer", 1)
            .description("Quality level of the video. Lower values mean better quality but increase file size and processing time.")
            .minValue(0)
            .maxValue(51)
            .defaultValue(23);

        basic.param<int>("b", "Bitrate [kbit/s]", 1)
            .description("The data rate allowed by the encoder.")
            .minValue(0)
            .maxValue(512000)
            .multiplierValue(1000)
            .defaultValue(15000);

        basic.param<int>("maxrate", "Max. Bitrate [kbit/s]", 1)
            .description("Higher value can improve maximum quality, but increases decoder requirements.")
            .minValue(0)
            .maxValue(512000)
            .multiplierValue(1000)
            .defaultValue(15000);

        basic.param<int>("bufsize", "Buffer Size [kbit/s]", 1)
            .description("The encoder bitstream buffer size.")
            .minValue(0)
            .maxValue(512000)
            .multiplierValue(1000)
            .defaultValue(15000);

        basic.param<int>("cq", "Constant quality", 1)
            .description("Set target quality level (0 to 51, 0 means automatic).")
            .minValue(0)
            .maxValue(51)
            .defaultValue(0);

        basic.param<std::string>("multipass", "Multipass")
            .description("Enable multipass encoding.")
            .option("Disabled", "disabled")
            .option("Quarter Resolution", "qres")
            .option("Full Resolution", "fullres")
            .defaultValue("disabled");

        basic.param<std::string>("tune", "Tune")
            .description("Set the encoding tuning info")
            .option("High Quality", "hq")
            .option("Low Latency", "ll")
            .option("Ultra Low Latency", "ull")
            .option("Lossless", "lossless")
            .defaultValue("hq");

        auto& frames = info.group("frames", "Frames", ItemParamGroupType::Standard);
        frames.param<int>("rc-lookahead", "RC Lookahead")
            .description("Sets the number of frames to use for ratecontrol and lookahead. Higher value means better quality and increased precision.")
            .minValue(0)
            .maxValue(1024)
            .defaultValue(0);

        frames.param<int>("g", "Max. GOP Size")
            .description("Maximum Group of Pictures Structure size in which intra- and inter-frames are arranged. Smaller values increase the video quality.")
            .minValue(1)
            .maxValue(1200)
            .defaultValue(300);

        frames.param<bool>("strict-gop", "Strict GOP")
            .description("Minimizes GOP-to-GOP rate fluctuations.")
            .defaultValue(false);

        frames.param<int>("bf", "Number of B-Frames")
            .description("Set max number of B-frames between non-B-frames. 0 means that Bipredective coded picture frames are disabled. If a value of -1 is used, it will choose an automatic value depending on the encoder.")
            .minValue(0)
            .maxValue(4)
            .defaultValue(2);

        frames.param<int>("refs", "Number of reference frames")
            .description("This parameter lets one specify how many references can be used, through a maximum of 16. Increasing the number of refs increases the DPB (Decoded Picture Buffer) requirement, which means hardware playback devices will often have strict limits to the number of refs they can handle. In live-action sources, more reference have limited use beyond 4-8, but in cartoon sources up to the maximum value of 16 is often useful. More reference frames require more processing power because every frame is searched by the motion search (except when an early skip decision is made). The slowdown is especially apparent with slower motion estimation methods.")
            .minValue(1)
            .maxValue(16)
            .defaultValue(3);

        frames.param<bool>("no-scenecut", "Disable scene cuts")
            .description("When lookahead is enabled, set this to 1 to disable adaptive I-frame insertion at scene cuts.")
            .defaultValue(false);

        frames.param<bool>("forced-idr", "Force keyframes as IDR")
            .description("If forcing keyframes, force them as IDR frames.")
            .defaultValue(false);

        frames.param<bool>("nonref_p", "AI P-Frames")
            .description("Enables automatic insertion of non-reference P-frames.")
            .defaultValue(false);

        auto& quantizer = info.group("quantizer", "Quantizer", ItemParamGroupType::Standard);
        quantizer.param<int>("qmin", "Min. Quantizer")
            .description("Lowest allowed quantizer")
            .minValue(0)
            .maxValue(51)
            .defaultValue(0);

        quantizer.param<int>("qmax", "Max. Quantizer")
            .description("Highest allowed quantizer")
            .minValue(0)
            .maxValue(51)
            .defaultValue(0);

        quantizer.param<int>("init_qpI", "QP Value for I-Frames")
            .description("Enable to change quantization parameter for I-frames.")
            .minValue(0)
            .maxValue(51)
            .defaultValue(0);

        quantizer.param<int>("init_qpP", "QP Value for P-Frames")
            .description("Enable to change quantization parameter for P-frames.")
            .minValue(0)
            .maxValue(51)
            .defaultValue(0);

        quantizer.param<int>("init_qpB", "QP Value for B-Frames")
            .description("Enable to change quantization parameter for B-frames.")
            .minValue(0)
            .maxValue(51)
            .defaultValue(0);

        quantizer.param<bool>("spatial-aq", "Spatial AQ")
            .description("Choose AQ Mode how available bits should be distributed between all macroblocks in the video.")
            .defaultValue(false);

        quantizer.param<int>("aq-strength", "Strength")
            .description("AQ strength scale from 1 (low) - 15 (aggressive).")
            .minValue(1)
            .maxValue(15)
            .defaultValue(8);

        quantizer.param<bool>("temporal-aq", "Temporal AQ")
            .description("Choose AQ Mode how available bits should be distributed between all macroblocks in the video.")
            .defaultValue(false);

        auto& misc = info.group("misc", "Miscellaneous", ItemParamGroupType::Standard);
        misc.param<std::string>("level", "Level")
            .description("Constrains encoding parameters such as bitrate range and maximum frame size.")
            .option("1", "1")
            .option("2", "2")
            .option("2.1", "2.1")
            .option("3", "3")
            .option("3.1", "3.1")
            .option("4", "4")
            .option("4.1", "4.1")
            .option("5", "5")
            .option("5.1", "5.1")
            .option("5.2", "5.2")
            .option("6", "6")
            .option("6.1", "6.1")
            .option("6.2", "6.2")
            .defaultValue("1");

        misc.param<int>("surfaces", "Surfaces")
            .description("Blockcount of memory allocated for bitmap rendering.")
            .minValue(0)
            .maxValue(64)
            .defaultValue(0);

        misc.param<bool>("zerolatency", "Zero Latency")
            .description("Indicate zero latency operation (no reordering delay).")
            .defaultValue(false);

        misc.param<bool>("weighted_pred", "Weighted Prediction")
            .description("Specifies the use of a scaling and offset, when performing motion compensation. This includes implicit weighted prediction for B-frames, and explicit weighted prediction for P-frames.")
            .defaultValue(false);

        misc.param<bool>("bluray-compat", "Blu-Ray Compatibility")
            .description("Create video files that will be compatible for Blu-Ray discs.")
            .defaultValue(false);

        misc.param<bool>("aud", "Use AUD")
            .description("Access unit delimiter can be used for signaling about start of video frame.")
            .defaultValue(false);

        return registerAsset(info);
    }

    int NVENCEncodersPlugin::registerAV1()
    {
        AssetInfo info;
        info.id = "av1_nvenc";
        info.name = "NVENC AV1";
        info.category = std::make_pair("nvidia", "Nvidia");
        info.description = "NVIDIAs GPU accelerated AV1 encoder.";
        info.type = NodeInfoType::encoder;
        info.mediaType = MediaType::video;
        info.helpUrl = "";

        info.preset("Good quality", {
            { "rc", "constq" },
            { "qp", "115" },
            { "preset", "medium" },
            { "profle", "high" }
        });

        info.format("YUV 420", "yuv420p")
            .format("YUV 444", "yuv444p")
            .format("YUV 420 (10 bit)", "p010le")
            .format("CUDA", "cuda");

        auto& basic = info.group("basic", "Basic", ItemParamGroupType::Forced);
        addDeviceProperty(basic);

        basic.param<std::string>("preset", "Preset")
            .description("Processing time of each frame. Slower values increase the overall video quality but take more time to process.")
            .option("Fastest (lowest quality)", "p1")
            .option("Faster (lower quality)", "p2")
            .option("Fast (low quality)", "p3")
            .option("Medium", "p4")
            .option("Slow (good quality)", "p5")
            .option("Slower (better quality)", "p6")
            .option("Slowest (best quality)", "p7")
            .defaultValue("p2");

        basic.param<std::string>("rc", "Strategy")
            .description("The encoding mode to use. Choose Constant Bitrate (CBR), Constant Quantizer (CQP) or Variable Bitrate (VBR) and if two encoding passes should be used (VBR only).")
            .option("Constant Quantizer (QP)", "constqp", [](const ItemParamAction& action) {
                action.setVisible("basic.qp", true);
                action.setVisible("basic.b", false);
                action.setVisible("basic.maxrate", false);
                action.setVisible("basic.bufsize", false);
                action.setVisible("basic.cq", false);
            })
            .option("Constant Bitrate (CBR)", "cbr", [](const ItemParamAction& action) {
                action.setVisible("basic.qp", true);
                action.setVisible("basic.b", false);
                action.setVisible("basic.maxrate", false);
                action.setVisible("basic.bufsize", false);
                action.setVisible("basic.cq", false);
            })
            .option("Variable Bitrate (VBR)", "vbr", [](const ItemParamAction& action) {
                action.setVisible("basic.qp", true);
                action.setVisible("basic.b", false);
                action.setVisible("basic.maxrate", false);
                action.setVisible("basic.bufsize", false);
                action.setVisible("basic.cq", false);
            })
            .defaultValue("constqp");

        basic.param<int>("qp", "Quantizer", 1)
            .description("Quality level of the video. Lower values mean better quality but increase file size and processing time.")
            .minValue(0)
            .maxValue(255)
            .defaultValue(23);

        basic.param<int>("b", "Bitrate [kbit/s]", 1)
            .description("The data rate allowed by the encoder.")
            .minValue(0)
            .maxValue(512000)
            .multiplierValue(1000)
            .defaultValue(15000);

        basic.param<int>("maxrate", "Max. Bitrate [kbit/s]", 1)
            .description("Higher value can improve maximum quality, but increases decoder requirements.")
            .minValue(0)
            .maxValue(512000)
            .multiplierValue(1000)
            .defaultValue(15000);

        basic.param<int>("bufsize", "Buffer Size [kbit/s]", 1)
            .description("The encoder bitstream buffer size.")
            .minValue(0)
            .maxValue(512000)
            .multiplierValue(1000)
            .defaultValue(15000);

        basic.param<int>("cq", "Constant quality", 1)
            .description("Set target quality level (0 to 51, 0 means automatic).")
            .minValue(0)
            .maxValue(51)
            .defaultValue(0);

        basic.param<std::string>("multipass", "Multipass")
            .description("Enable multipass encoding.")
            .option("Disabled", "disabled")
            .option("Quarter Resolution", "qres")
            .option("Full Resolution", "fullres")
            .defaultValue("disabled");

        basic.param<std::string>("tune", "Tune")
            .description("Set the encoding tuning info")
            .option("High Quality", "hq")
            .option("Low Latency", "ll")
            .option("Ultra Low Latency", "ull")
            .defaultValue("hq");

        auto& frames = info.group("frames", "Frames", ItemParamGroupType::Standard);
        frames.param<int>("rc-lookahead", "RC Lookahead")
            .description("Sets the number of frames to use for ratecontrol and lookahead. Higher value means better quality and increased precision.")
            .minValue(0)
            .maxValue(1024)
            .defaultValue(0);

        frames.param<bool>("strict_gop", "Strict GOP")
            .description("Minimizes GOP-to-GOP rate fluctuations.")
            .defaultValue(false);

        frames.param<bool>("no-scenecut", "Disable scene cuts")
            .description("When lookahead is enabled, set this to 1 to disable adaptive I-frame insertion at scene cuts.")
            .defaultValue(false);

        frames.param<bool>("forced-idr", "Force keyframes as IDR")
            .description("If forcing keyframes, force them as IDR frames.")
            .defaultValue(false);

        frames.param<bool>("nonref_p", "AI P-Frames")
            .description("Enables automatic insertion of non-reference P-frames.")
            .defaultValue(false);


        auto& quantizer = info.group("quantizer", "Quantizer", ItemParamGroupType::Standard);
        quantizer.param<int>("init_qpP", "QP Value for P-Frames")
            .description("Enable to change quantization parameter for P-frames.")
            .minValue(0)
            .maxValue(255)
            .defaultValue(0);

        quantizer.param<int>("init_qpB", "QP Value for B-Frames")
            .description("Enable to change quantization parameter for B-frames.")
            .minValue(0)
            .maxValue(255)
            .defaultValue(0);

        quantizer.param<int>("init_qpI", "QP Value for I-Frames")
            .description("Enable to change quantization parameter for I-frames.")
            .minValue(0)
            .maxValue(255)
            .defaultValue(0);

        quantizer.param<int>("qp_cb_offset", "CB Offset")
            .description("Quantization parameter offset for cb channel (from -12 to 12) (default 0)")
            .minValue(-12)
            .maxValue(12)
            .defaultValue(0);

        quantizer.param<int>("qp_cr_offset", "CR Offset")
            .description("Quantization parameter offset for cr channel (from -12 to 12) (default 0)")
            .minValue(-12)
            .maxValue(12)
            .defaultValue(0);

        quantizer.param<bool>("spatial-aq", "Spatial AQ")
            .description("Choose AQ Mode how available bits should be distributed between all macroblocks in the video.")
            .defaultValue(false);

        quantizer.param<int>("aq-strength", "Strength")
            .description("AQ strength scale from 1 (low) - 15 (aggressive).")
            .minValue(1)
            .maxValue(15)
            .defaultValue(8);

        quantizer.param<bool>("temporal-aq", "Temporal AQ")
            .description("Choose AQ Mode how available bits should be distributed between all macroblocks in the video.")
            .defaultValue(false);

        auto& misc = info.group("misc", "Miscellaneous", ItemParamGroupType::Standard);
        misc.param<std::string>("level", "Level")
            .description("Constrains encoding parameters such as bitrate range and maximum frame size.")
            .option("2", "2")
            .option("2.1", "2.1")
            .option("3", "3")
            .option("3.1", "3.1")
            .option("4", "4")
            .option("4.1", "4.1")
            .option("5", "5")
            .option("5.1", "5.1")
            .option("5.2", "5.2")
            .option("5.3", "5.3")
            .option("6", "6")
            .option("6.1", "6.1")
            .option("6.2", "6.2")
            .option("6.3", "6.3")
            .defaultValue("2");

        misc.param<int>("surfaces", "Surfaces")
            .description("Blockcount of memory allocated for bitmap rendering.")
            .minValue(0)
            .maxValue(64)
            .defaultValue(0);

        misc.param<bool>("zerolatency", "Zero Latency")
            .description("Indicate zero latency operation (no reordering delay).")
            .defaultValue(false);

        return registerAsset(info);
    }

    void NVENCEncodersPlugin::addDeviceProperty(ItemParamGroup& group)
    {
        auto& gpu = group.param<int>("gpu", "GPU")
            .description("CUDA device")
            .defaultValue(0);

        if (devices.size() > 0)
        {
            for (int i = 0; i < devices.size(); i++)
                gpu.option(std::to_string(i) + ": " + devices.at(i), i);
        }
        else if (devices.size() == 0)
            gpu.option("Auto", 0);
    }
}
