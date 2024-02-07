#include "plugin.h"
#include <boost/dll/import.hpp>
#include <boost/dll/shared_library.hpp>
#include <boost/function.hpp>

namespace VoukoderPro
{
    SVTEncodersPlugin::SVTEncodersPlugin() : EncoderPlugin()
    {
        registerAV1();
    }

    int SVTEncodersPlugin::registerAV1()
    {
        AssetInfo info;
        info.id = "libsvtav1";
        info.name = "SVT AV1";
        info.category = std::make_pair("svt", "Scalable Vector Technology");
        info.description = "Scalable Vector Technology for AV1";
        info.type = NodeInfoType::encoder;
        info.mediaType = MediaType::video;
        info.helpUrl = "https://gitlab.com/AOMediaCodec/SVT-AV1/-/blob/master/Docs/svt-av1_encoder_user_guide.md";

        info.format("YUV 420", "yuv420p")
            .format("YUV 420 (10 bit)", "yuv420p10le");

        auto& basic = info.group("basic", "Basic", ItemParamGroupType::Forced);
        basic.param<int>("rc", "Strategy")
            .description("Bit rate control mode")
            .option("Constant Rate Factor", 0, [](const ItemParamAction& action)
                {
                    action.setVisible("basic.crf", true);
                    action.setVisible("basic.qp", true);
                    action.setVisible("basic.tbr", false);
                    action.setVisible("basic.mbr", false);
                })
            .option("Variable Bitrate", 1, [](const ItemParamAction& action)
                {
                    action.setVisible("basic.crf", false);
                    action.setVisible("basic.qp", false);
                    action.setVisible("basic.tbr", true);
                    action.setVisible("basic.mbr", true);
                })
            .option("Constant Bitrate", 2, [](const ItemParamAction& action)
                {
                    action.setVisible("basic.crf", false);
                    action.setVisible("basic.qp", false);
                    action.setVisible("basic.tbr", true);
                    action.setVisible("basic.mbr", false);
                })
            .defaultValue(0);

        basic.param<int>("crf", "Constant Rate Factor", 1)
            .description("Lower values mean better quality but increase the processing time.")
            .minValue(1)
            .maxValue(63)
            .defaultValue(35);

        basic.param<int>("qp", "Initial QP level value", 1)
            .description("Initial QP level value")
            .minValue(1)
            .maxValue(63)
            .defaultValue(35);

        basic.param<int>("tbr", "Bitrate [kbit]", 1)
            .description("The data rate allowed by the encoder.")
            .minValue(1)
            .maxValue(100000)
            .defaultValue(2000);

        basic.param<int>("mbr", "Max. Bitrate [kbit]", 1)
            .description("Higher value can improve maximum quality, but increases decoder requirements.")
            .minValue(1)
            .maxValue(100000)
            .defaultValue(2000);

        basic.param<int>("preset", "Preset")
            .description("Encoding preset")
            .option("Ultra quality", 0)
            .option("Best quality", 1)
            .option("Better quality", 2)
            .option("Good quality", 3)
            .option("Medium", 4)
            .option("Fast", 5)
            .option("Faster", 6)
            .option("Very fast", 7)
            .option("Ultra fast", 8)
            .option("Extremly fast", 9)
            .option("Warp 9.975", 10)
            .option("Faster than light", 11)
            .option("Ridiculous speed", 12)
            .option("Ludicrous speed", 13)
            .defaultValue(10);

        basic.param<int>("tune", "Tune")
            .description("Specifies whether to use PSNR or VQ as the tuning metric")
            .option("VQ", 0)
            .option("PSNR", 1)
            .defaultValue(0);

        basic.param<std::string>("tier", "Tier")
            .description("Set the operating point tier.")
            .option("main", "main")
            .option("high", "high")
            .defaultValue("main");

        basic.param<std::string>("profile", "Profile")
            .description("Bitstream profile to use")
            .option("Main", "main")
            .option("High", "high")
            .option("Professional", "professional")
            .defaultValue("main");

        auto& advanced = info.group("advanced", "Advanced", ItemParamGroupType::Standard);
        advanced.param<std::string>("level", "Level")
            .description("Set level (level_idc)")
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

        advanced.param<int>("qmin", "Qmin")
            .description("Minimum (best) quantizer[1-62]")
            .minValue(1)
            .maxValue(62)
            .defaultValue(1);

        advanced.param<int>("qmax", "Qmax")
            .description("Maximum (worst) quantizer[1-63]")
            .minValue(1)
            .maxValue(63)
            .defaultValue(63);

        advanced.param<int>("hierarchical-levels", "Hierarchical prediction levels")
            .description("Hierarchical prediction levels setting")
            .minValue(3)
            .maxValue(5)
            .defaultValue(4);

        advanced.param<bool>("fast-decode", "Fast decode")
            .description("Tune settings to output bitstreams that can be decoded faster.")
            .defaultValue(false);

        advanced.param<bool>("sc_detection", "Scene change detection")
            .description("Scene change detection")
            .defaultValue(false);

        advanced.param<int>("film-grain", "Film grain")
            .description("Enable film grain.")
            .minValue(1)
            .maxValue(50)
            .defaultValue(1);

        advanced.param<int>("keyint", "GOP size (frames)")
            .description("[-2: ~5 seconds, -1=infinite only for CRF, 0: == -1]")
            .minValue(-2)
            .maxValue(2147483647)
            .defaultValue(-2);

        advanced.param<int>("irefresh-type", "Intra refresh")
            .description("Intra refresh type [FWD Frame (Open GOP), KEY Frame (Closed GOP)]")
            .option("FWD Frame", 1)
            .option("KEY Frame", 2)
            .defaultValue(2);

        advanced.param<bool>("enable-hdr", "HDR Metadata")
            .description("Enable writing of HDR metadata in the bitstream")
            .defaultValue(false);

        advanced.param<int>("lp", "CPUs")
            .description("Target (best effort) number of logical cores to be used. 0 means all.")
            .minValue(0)
            .maxValue(65535)
            .defaultValue(0);

        advanced.param<bool>("pin", "Pinned execution")
            .description("Pin the execution to the first --lp cores. Overwritten to 0 when --ss is set.")
            .defaultValue(false);

        advanced.param<int>("ss", "Target socket")
            .description("Specifies which socket to run on, assumes a max of two sockets.")
            .minValue(-1)
            .maxValue(1)
            .defaultValue(-1);

        advanced.param<int>("la_depth", "Look ahead depth")
            .description("Look ahead distance [0, 120]")
            .minValue(-1)
            .maxValue(120)
            .defaultValue(-1);

        advanced.param<int>("pred-struct", "Prediction structure")
            .description("Set prediction structure.")
            .option("Low delay", 1)
            .option("Random access", 2)
            .defaultValue(2);

        advanced.param<int>("aq-mode", "Adaptive QP level")
            .description("Set adaptive QP level.")
            .option("Off", 0)
            .option("Variance based", 1)
            .option("Deltaq pred. efficiency", 2)
            .defaultValue(2);

        advanced.param<int>("use-fixed-qindex-offsets", "Fixed QIndex offset")
            .description("Overwrite the encoder default hierarchical layer based QP assignment and use fixed Q index offsets.")
            .minValue(0)
            .maxValue(2)
            .defaultValue(0);

        advanced.param<int>("key-frame-qindex-offset", "Key frame QIndex offset")
            .description("Overwrite the encoder default keyframe Q index assignment.")
            .minValue(-256)
            .maxValue(255)
            .defaultValue(0);

        advanced.param<int>("key-frame-chroma-qindex-offset", "Key frame chroma QIndex offset")
            .description("Overwrite the encoder default chroma keyframe Q index assignment.")
            .minValue(-256)
            .maxValue(255)
            .defaultValue(0);

        advanced.param<int>("undershoot-pct", "Datarate undershoot")
            .description("Allowable datarate undershoot (min) target (%), default depends on the rate control mode.")
            .minValue(0)
            .maxValue(100)
            .defaultValue(25);

        advanced.param<int>("overshoot-pct", "Datarate overshoot")
            .description("Allowable datarate overshoot (max) target (%), default depends on the rate control mode.")
            .minValue(0)
            .maxValue(100)
            .defaultValue(25);

        advanced.param<int>("mbr-overshoot-pct", "MBR datarate overshoot")
            .description("Allowable datarate overshoot (max) target (%), Only applicable for Capped CRF.")
            .minValue(0)
            .maxValue(100)
            .defaultValue(50);

        advanced.param<int>("buf-sz", "Client maximum buffer size")
            .description("Client maximum buffer size (ms), only applicable for CBR.")
            .minValue(20)
            .maxValue(10000)
            .defaultValue(1000);

        advanced.param<int>("buf-initial-sz", "Client initial buffer size")
            .description("Client initial buffer size (ms), only applicable for CBR.")
            .minValue(20)
            .maxValue(10000)
            .defaultValue(600);

        advanced.param<int>("buf-optimal-sz", "Client optimal buffer size")
            .description("Client optimal buffer size (ms), only applicable for CBR.")
            .minValue(20)
            .maxValue(10000)
            .defaultValue(600);

        advanced.param<int>("recode-loop", "Recode loop level")
            .description("Recode loop level")
            .option("Off", 0)
            .option("Allow recode for KF and exceeding maximum frame bandwidth", 1)
            .option("Allow recode only for key frames, alternate reference frames, and Golden frames", 2)
            .option("Allow recode for all frame types based on bitrate constraints", 3)
            .option("Preset based decision", 4)
            .defaultValue(4);

        advanced.param<int>("bias-pct", "CBR/VBR bias")
            .description("CBR/VBR bias [0: CBR-like, 100: VBR-like].")
            .minValue(0)
            .maxValue(100)
            .defaultValue(100);

        advanced.param<int>("minsection-pct", "GOP min bitrate")
            .description("GOP min bitrate (expressed as a percentage of the target rate).")
            .minValue(0)
            .maxValue(100)
            .defaultValue(0);

        advanced.param<int>("maxsection-pct", "GOP max bitrate")
            .description("GOP max bitrate (expressed as a percentage of the target rate).")
            .minValue(0)
            .maxValue(10000)
            .defaultValue(2000);

        advanced.param<bool>("gop-constraint-rc", "GOP constraint rate control")
            .description("Constrains the rate control to match the target rate for each GoP.")
            .defaultValue(false);

        advanced.param<bool>("enable-qm", "Quantisation matrices")
            .description("Enable quantisation matrices.")
            .defaultValue(false);

        advanced.param<int>("qm-min", "Min quant matrix flatness")
            .description("Min quant matrix flatness")
            .minValue(0)
            .maxValue(15)
            .defaultValue(8);

        advanced.param<int>("qm-max", "Max quant matrix flatness")
            .description("Max quant matrix flatness")
            .minValue(0)
            .maxValue(15)
            .defaultValue(15);

        auto& av1_specific = info.group("av1_specific", "AV1 specific", ItemParamGroupType::Standard);
        av1_specific.param<int>("tile_rows", "Tile rows")
            .description("Number of tile rows to use, TileRow == log2(x), default changes per resolution.")
            .minValue(0)
            .maxValue(6)
            .defaultValue(0);

        av1_specific.param<int>("tile_columns", "Tile columns")
            .description("Number of tile columns to use, TileCol == log2(x), default changes per resolution.")
            .minValue(0)
            .maxValue(4)
            .defaultValue(0);

        av1_specific.param<bool>("enable-dlf", "Loop filter")
            .description("Deblocking loop filter control.")
            .defaultValue(true);

        av1_specific.param<bool>("enable-cdef", "Constrained Directional Enhancement")
            .description("Enable Constrained Directional Enhancement Filter.")
            .defaultValue(true);

        av1_specific.param<bool>("enable-restoration", "Loop restoration filter")
            .description("Enable loop restoration filter.")
            .defaultValue(true);

        av1_specific.param<bool>("enable-tpl-la", "Temporal Dependency model control")
            .description("Temporal Dependency model control, currently forced on library side, only applicable for CRF/CQP.")
            .defaultValue(true);

        av1_specific.param<bool>("enable-tf", "ALT-REF")
            .description("Enable ALT-REF (temporally filtered) frames.")
            .defaultValue(true);

        av1_specific.param<bool>("enable-overlays", "Overlayer pictures")
            .description("Enable the insertion of overlayer pictures which will be used as an additional reference frame for the base layer picture.")
            .defaultValue(false);

        av1_specific.param<int>("scm", "Screen content detection")
            .description("Set screen content detection level.")
            .option("Off", 0)
            .option("On", 1)
            .option("Content adaptive", 2)
            .defaultValue(2);

        av1_specific.param<bool>("rmv", "Restricted motion vector")
            .description("Restrict motion vectors from reaching outside the picture boundary.")
            .defaultValue(false);

        av1_specific.param<bool>("film-grain-denoise", "Film grain denoise")
            .description("Apply denoising when film grain is ON, default is 1 [0: no denoising, film grain data sent in frame header, 1: level of denoising is set by the film-grain parameter].")
            .defaultValue(true);

        av1_specific.param<int>("superres-mode", "Super-resolution mode")
            .description("Enable super-resolution mode.")
            .option("None", 0)
            .option("8/denom", 1)
            .option("Random scale", 2)
            .option("Q_index based", 3)
            .option("Automatic", 4)
            .defaultValue(2);

        av1_specific.param<int>("superres-denom", "Super-resolution denominator")
            .description("Super-resolution denominator, only applicable for mode == 1 [8: no scaling, 16: half-scaling].")
            .minValue(8)
            .maxValue(16)
            .defaultValue(8);

        av1_specific.param<int>("superres-kf-denom", "Super-resolution denominator for key frames")
            .description("Super-resolution denominator for key frames, only applicable for mode == 1 [8: no scaling, 16: half-scaling].")
            .minValue(8)
            .maxValue(16)
            .defaultValue(8);

        av1_specific.param<int>("superres-qthres", "Super-resolution q-threshold")
            .description("Super-resolution q-threshold, only applicable for mode == 3.")
            .minValue(0)
            .maxValue(63)
            .defaultValue(43);

        av1_specific.param<int>("superres-kf-qthres", "Super-resolution q-threshold for key frames")
            .description("Super-resolution q-threshold for key frames, only applicable for mode == 3.")
            .minValue(0)
            .maxValue(63)
            .defaultValue(43);

        av1_specific.param<int>("sframe-dist", "S-Frame interva")
            .description("S-Frame interval (frames) [0: OFF, > 0: ON].")
            .minValue(0)
            .maxValue(2147483647)
            .defaultValue(0);

        av1_specific.param<int>("sframe-mode", "S-Frame insertion mode")
            .description("S-Frame insertion mode [1: the considered frame will be made into an S-Frame only if it is an altref frame, 2: the next altref frame will be made into an S-Frame].")
            .minValue(1)
            .maxValue(2)
            .defaultValue(2);

        av1_specific.param<int>("resize-mode", "Resize mode")
            .description("Enable reference scaling mode.")
            .minValue(0)
            .maxValue(4)
            .defaultValue(0);

        av1_specific.param<int>("resize-denom", "Reference scaling denominator")
            .description("Reference scaling denominator, only applicable for mode == 1 [8: no scaling, 16: half-scaling].")
            .minValue(8)
            .maxValue(16)
            .defaultValue(8);

        av1_specific.param<int>("resize-kf-denom", "Reference scaling denominator for key frames")
            .description("Reference scaling denominator for key frames, only applicable for mode == 1 [8: no scaling, 16: half-scaling].")
            .minValue(8)
            .maxValue(16)
            .defaultValue(8);

        auto& color = info.group("color", "Color", ItemParamGroupType::Standard);
        color.param<int>("color-primaries", "Color primaries")
            .description("Color primaries")
            .option("BT.709", 1)
            .option("Unspecified", 2)
            .option("BT.470M", 4)
            .option("BT.470BG", 5)
            .option("BT.601", 6)
            .option("SMPTE240", 7)
            .option("Film", 8)
            .option("BT.2020", 9)
            .option("SMPTE428", 10)
            .option("SMPTE431", 11)
            .option("SMPTE432", 12)
            .option("EBU3213", 22)
            .defaultValue(2);

        color.param<int>("transfer-characteristics", "Transfer characteristics")
            .description("Transfer characteristics")
            .option("BT.709", 1)
            .option("Unspecified", 2)
            .option("BT.470M", 4)
            .option("BT.470BG", 5)
            .option("BT.601", 6)
            .option("SMPTE240", 7)
            .option("Linear", 8)
            .option("Log100", 9)
            .option("Log100-Sqrt10", 10)
            .option("IEC61966", 11)
            .option("BT.1361", 12)
            .option("SRGB", 13)
            .option("BT.2020-10", 14)
            .option("BT.2020-12", 15)
            .option("SMPTE2084", 16)
            .option("SMPTE428", 17)
            .option("BT.2100 HLG", 18)
            .defaultValue(2);

        color.param<int>("matrix-coefficients", "Matrix coefficients")
            .description("Matrix coefficients")
            .option("Identity", 0)
            .option("BT.709", 1)
            .option("Unspecified", 2)
            .option("FCC", 4)
            .option("BT.470BG", 5)
            .option("BT.601", 6)
            .option("SMPTE240", 7)
            .option("YCgCo", 8)
            .option("BT.2020-NCL", 9)
            .option("BT.2020-CL", 10)
            .option("SMPTE2085", 11)
            .option("Chroma-NCL", 12)
            .option("Chroma-CL", 13)
            .option("BT.2100 ICtCp", 14)
            .defaultValue(2);

        color.param<int>("color-range", "Color range")
            .description("Color range")
            .option("Studio", 0)
            .option("Full", 1)
            .defaultValue(0);

        color.param<int>("chroma-sample-position", "Chroma sample position")
            .description("Chroma sample position")
            .option("Unknown", 0)
            .option("vertical/left", 1)
            .option("colocated/topleft", 2)
            .defaultValue(0);

        return registerAsset(info);
    }
}