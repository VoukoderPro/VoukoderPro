#include "plugin.h"

namespace VoukoderPro
{
    FFmpegMuxerPlugin::FFmpegMuxerPlugin()
    {
        registerAAC();
        registerAC3();
        registerAVI();
        registerFLV();
        registerGIF();
        registerMatroska();
        registerMOV();
        registerMP2();
        registerMP3();
        registerMP4();
        registerMPEG2Video();
        registerWAV();
        registerWavpack();
        registerWEBM();
    }

    void FFmpegMuxerPlugin::registerMatroska()
    {
        AssetInfo info;
        info.id = "matroska";
        info.name = "Matroska";
        info.category = std::make_pair("ffmpeg", "FFmpeg");
        info.description = "Matroska / MKV";
        info.type = NodeInfoType::muxer;
        info.mediaType = MediaType::mux;
        info.helpUrl = "https://ffmpeg.org/ffmpeg-formats.html#" + info.id;
        info.allowedInputGroups = {
            AV_CODEC_ID_H264, AV_CODEC_ID_HEVC, AV_CODEC_ID_VP8, AV_CODEC_ID_VP9, AV_CODEC_ID_AV1, AV_CODEC_ID_PRORES, AV_CODEC_ID_HAP, AV_CODEC_ID_FFV1, AV_CODEC_ID_UTVIDEO, AV_CODEC_ID_CFHD, AV_CODEC_ID_FFVHUFF, // Video
            AV_CODEC_ID_AAC, AV_CODEC_ID_AC3, AV_CODEC_ID_EAC3, AV_CODEC_ID_TRUEHD, AV_CODEC_ID_DTS, AV_CODEC_ID_ALAC, AV_CODEC_ID_FLAC, AV_CODEC_ID_MP2, AV_CODEC_ID_MP3, AV_CODEC_ID_OPUS, AV_CODEC_ID_VORBIS, AV_CODEC_ID_PCM_S16LE, AV_CODEC_ID_PCM_S24LE, AV_CODEC_ID_PCM_S32LE, AV_CODEC_ID_WAVPACK // Audio
        };

        auto& standard = info.group("standard", "Standard", ItemParamGroupType::Standard);

        standard.param<int>("reserve_index_space", "Reserve index space")
            .description("Reserve a given amount of space (in bytes) at the beginning of the file for the index (cues).")
            .minValue(0)
            .maxValue(INT_MAX)
            .defaultValue(0);

        standard.param<bool>("cues_to_front", "Cues to front")
            .description("Move Cues (the index) to the front by shifting data if necessary")
            .defaultValue(false);

        standard.param<int>("cluster_size_limit", "Cluster size limit")
            .description("Store at most the provided amount of bytes in a cluster.")
            .minValue(-1)
            .maxValue(INT_MAX)
            .defaultValue(-1);

        standard.param<int>("cluster_time_limit", "Cluster time limit")
            .description("Store at most the provided number of milliseconds in a cluster.")
            .minValue(-1)
            .maxValue(INT_MAX)
            .defaultValue(-1);

        standard.param<bool>("dash", "Dash")
            .description("Create a WebM file conforming to WebM DASH specification")
            .defaultValue(false);

        standard.param<int>("dash_track_number", "Track number for the DASH stream")
            .description("Store at most the provided number of milliseconds in a cluster.")
            .minValue(1)
            .maxValue(INT_MAX)
            .defaultValue(1);

        standard.param<bool>("live", "Live")
            .description("Write files assuming it is a live stream.")
            .defaultValue(false);

        standard.param<bool>("allow_raw_vfw", "Allow raw VfW")
            .description("allow RAW VFW mode")
            .defaultValue(false);

        standard.param<bool>("flipped_raw_rgb", "Flipped raw RGB")
            .description("Raw RGB bitmaps in VFW mode are stored bottom-up")
            .defaultValue(false);

        standard.param<bool>("write_crc32", "Write CRC32")
            .description("write a CRC32 element inside every Level 1 element")
            .defaultValue(true);

        standard.param<int>("default_mode", "Default mode")
            .description("Controls how a track's FlagDefault is inferred")
            .option("Infer", 0)
            .option("Infer no subs", 1)
            .option("Passthrough", 2)
            .defaultValue(2);

        registerAsset(info);
    }

    void FFmpegMuxerPlugin::registerMOV()
    {
        AssetInfo info;
        info.id = "mov";
        info.name = "MOV";
        info.category = std::make_pair("ffmpeg", "FFmpeg");
        info.description = "QuickTime / MOV";
        info.type = NodeInfoType::muxer;
        info.mediaType = MediaType::mux;
        info.helpUrl = "https://ffmpeg.org/ffmpeg-formats.html#mov_002c-mp4_002c-ismv";
        info.allowedInputGroups = {
            AV_CODEC_ID_PRORES, AV_CODEC_ID_H264, AV_CODEC_ID_HEVC, AV_CODEC_ID_HAP, AV_CODEC_ID_QTRLE, AV_CODEC_ID_CFHD, // Video
            AV_CODEC_ID_AAC, AV_CODEC_ID_ALAC, AV_CODEC_ID_PCM_S16LE, AV_CODEC_ID_PCM_S24LE, AV_CODEC_ID_PCM_S32LE // Audio
        };

        auto& standard = info.group("standard", "Standard", ItemParamGroupType::Standard);

        standard.param<flags>("movflags", "MOV flags")
            .description("MOV flags")
            .flag("rtphint")
            .flag("empty_moov")
            .flag("frag_keyframe")
            .flag("frag_every_frame")
            .flag("separate_moof")
            .flag("frag_custom")
            .flag("isml")
            .flag("faststart")
            .flag("omit_tfhd_offset")
            .flag("disable_chpl")
            .flag("default_base_moof")
            .flag("dash")
            .flag("cmaf")
            .flag("frag_discont")
            .flag("delay_moov")
            .flag("global_sidx")
            .flag("skip_sidx")
            .flag("write_colr")
            .flag("prefer_icc")
            .flag("write_gama")
            .flag("use_metadata_tags")
            .flag("skip_trailer")
            .flag("negative_cts_offsets")
            .defaultValue(0);

        standard.param<int>("moov_size", "Moov size")
            .description("maximum moov size so it can be placed at the begin")
            .minValue(0)
            .maxValue(INT_MAX)
            .defaultValue(0);

        standard.param<flags>("rtpflags", "RTP flags")
            .description("RTP flags")
            .flag("latm")
            .flag("rfc2190")
            .flag("skip_rtcp")
            .flag("h264_mode0")
            .flag("send_bye")
            .defaultValue(0);

        auto& advanced = info.group("advanced", "Advanced", ItemParamGroupType::Standard);

        advanced.param<bool>("skip_iods", "Skip IODs")
            .description("Skip writing iods atom.")
            .defaultValue(true);

        advanced.param<int>("iods_audio_profile", "IODs audio profile")
            .description("iods audio profile atom.")
            .minValue(-1)
            .maxValue(255)
            .defaultValue(-1);

        advanced.param<int>("iods_video_profile", "IODs video profile")
            .description("iods video profile atom.")
            .minValue(-1)
            .maxValue(255)
            .defaultValue(-1);

        advanced.param<int>("min_frag_duration", "Minimum fragment duration")
            .description("Minimum fragment duration")
            .minValue(0)
            .maxValue(INT_MAX)
            .defaultValue(0);

        advanced.param<int>("frag_duration", "Maximum fragment duration")
            .description("Maximum fragment duration")
            .minValue(0)
            .maxValue(INT_MAX)
            .defaultValue(0);

        advanced.param<int>("frag_size", "Maximum fragment size")
            .description("Maximum fragment size")
            .minValue(0)
            .maxValue(INT_MAX)
            .defaultValue(0);

        advanced.param<int>("ism_lookahead", "ISM lookahead")
            .description("Number of lookahead entries for ISM files")
            .minValue(0)
            .maxValue(255)
            .defaultValue(0);

        advanced.param<int>("video_track_timescale", "Video track timescale")
            .description("set timescale of all video tracks")
            .minValue(0)
            .maxValue(INT_MAX)
            .defaultValue(0);

        advanced.param<std::string>("brand", "Brand")
            .description("Override major brand")
            .defaultValue("");

        advanced.param<bool>("use_editlist", "Use editlist")
            .description("use edit list")
            .defaultValue(false);

        advanced.param<int>("fragment_index", "Fragment index")
            .description("Fragment number of the next fragment")
            .minValue(1)
            .maxValue(INT_MAX)
            .defaultValue(1);

        advanced.param<double>("mov_gamma", "MOV gamma")
            .description("gamma value for gama atom")
            .minValue(0.0)
            .maxValue(10.0)
            .defaultValue(0.0);

        advanced.param<int>("frag_interleave", "Fragment interleave")
            .description("Interleave samples within fragments (max number of consecutive samples, lower is tighter interleaving, but with more overhead)")
            .minValue(0)
            .maxValue(INT_MAX)
            .defaultValue(0);

        advanced.param<bool>("use_stream_ids_as_track_ids", "Use stream ids as track ids")
            .description("use stream ids as track ids")
            .defaultValue(false);

        advanced.param<bool>("write_btrt", "Write bitrate")
            .description("Force writing btrt atom")
            .defaultValue(false);

        advanced.param<bool>("write_tmcd", "Write timecode")
            .description("Force writing tmcd atom")
            .defaultValue(false);

        advanced.param<int>("write_prft", "Write producer ref. time")
            .description("Write producer reference time box with specified time source")
            .option("Automatic", 0)
            .option("Wallclock", 1)
            .option("PTS", 2)
            .defaultValue(0);

        advanced.param<bool>("empty_hdlr_name", "Empty HDLR name")
            .description("write zero-length name string in hdlr atoms within mdia and minf atoms")
            .defaultValue(false);

        advanced.param<int>("movie_timescale", "Movie timescale")
            .description("set movie timescale")
            .minValue(1)
            .maxValue(INT_MAX)
            .defaultValue(1000);

        registerAsset(info);
    }

    void FFmpegMuxerPlugin::registerMP4()
    {
        AssetInfo info;
        info.id = "mp4";
        info.name = "MP4";
        info.category = std::make_pair("ffmpeg", "FFmpeg");
        info.description = "MPEG-4 Part 14";
        info.type = NodeInfoType::muxer;
        info.mediaType = MediaType::mux;
        info.helpUrl = "https://ffmpeg.org/ffmpeg-formats.html#mov_002c-mp4_002c-ismv";
        info.allowedInputGroups = {
            AV_CODEC_ID_H264, AV_CODEC_ID_HEVC, AV_CODEC_ID_VP8, AV_CODEC_ID_VP9, AV_CODEC_ID_AV1, // Video
            AV_CODEC_ID_AAC, AV_CODEC_ID_AC3, AV_CODEC_ID_EAC3, AV_CODEC_ID_MP3, AV_CODEC_ID_OPUS, AV_CODEC_ID_VORBIS // Audio
        };

        auto& standard = info.group("standard", "Standard", ItemParamGroupType::Standard);

        standard.param<flags>("movflags", "MOV flags")
            .description("MOV flags")
            .flag("rtphint")
            .flag("empty_moov")
            .flag("frag_keyframe")
            .flag("frag_every_frame")
            .flag("separate_moof")
            .flag("frag_custom")
            .flag("isml")
            .flag("faststart")
            .flag("omit_tfhd_offset")
            .flag("disable_chpl")
            .flag("default_base_moof")
            .flag("dash")
            .flag("cmaf")
            .flag("frag_discont")
            .flag("delay_moov")
            .flag("global_sidx")
            .flag("skip_sidx")
            .flag("write_colr")
            .flag("prefer_icc")
            .flag("write_gama")
            .flag("use_metadata_tags")
            .flag("skip_trailer")
            .flag("negative_cts_offsets")
            .defaultValue(0);

        standard.param<int>("moov_size", "Moov size")
            .description("maximum moov size so it can be placed at the begin")
            .minValue(0)
            .maxValue(INT_MAX)
            .defaultValue(0);

        standard.param<flags>("rtpflags", "RTP flags")
            .description("RTP flags")
            .flag("latm")
            .flag("rfc2190")
            .flag("skip_rtcp")
            .flag("h264_mode0")
            .flag("send_bye")
            .defaultValue(0);

        auto& advanced = info.group("advanced", "Advanced", ItemParamGroupType::Standard);

        advanced.param<bool>("skip_iods", "Skip IODs")
            .description("Skip writing iods atom.")
            .defaultValue(true);

        advanced.param<int>("iods_audio_profile", "IODs audio profile")
            .description("iods audio profile atom.")
            .minValue(-1)
            .maxValue(255)
            .defaultValue(-1);

        advanced.param<int>("iods_video_profile", "IODs video profile")
            .description("iods video profile atom.")
            .minValue(-1)
            .maxValue(255)
            .defaultValue(-1);

        advanced.param<int>("min_frag_duration", "Minimum fragment duration")
            .description("Minimum fragment duration")
            .minValue(0)
            .maxValue(INT_MAX)
            .defaultValue(0);

        advanced.param<int>("frag_duration", "Maximum fragment duration")
            .description("Maximum fragment duration")
            .minValue(0)
            .maxValue(INT_MAX)
            .defaultValue(0);

        advanced.param<int>("frag_size", "Maximum fragment size")
            .description("Maximum fragment size")
            .minValue(0)
            .maxValue(INT_MAX)
            .defaultValue(0);

        advanced.param<int>("ism_lookahead", "ISM lookahead")
            .description("Number of lookahead entries for ISM files")
            .minValue(0)
            .maxValue(255)
            .defaultValue(0);

        advanced.param<int>("video_track_timescale", "Video track timescale")
            .description("set timescale of all video tracks")
            .minValue(0)
            .maxValue(INT_MAX)
            .defaultValue(0);

        advanced.param<std::string>("brand", "Brand")
            .description("Override major brand")
            .defaultValue("");

        advanced.param<bool>("use_editlist", "Use editlist")
            .description("use edit list")
            .defaultValue(false);

        advanced.param<int>("fragment_index", "Fragment index")
            .description("Fragment number of the next fragment")
            .minValue(1)
            .maxValue(INT_MAX)
            .defaultValue(1);

        advanced.param<double>("mov_gamma", "MOV gamma")
            .description("gamma value for gama atom")
            .minValue(0.0)
            .maxValue(10.0)
            .defaultValue(0.0);

        advanced.param<int>("frag_interleave", "Fragment interleave")
            .description("Interleave samples within fragments (max number of consecutive samples, lower is tighter interleaving, but with more overhead)")
            .minValue(0)
            .maxValue(INT_MAX)
            .defaultValue(0);

        advanced.param<bool>("use_stream_ids_as_track_ids", "Use stream ids as track ids")
            .description("use stream ids as track ids")
            .defaultValue(false);

        advanced.param<bool>("write_btrt", "Write bitrate")
            .description("Force writing btrt atom")
            .defaultValue(false);

        advanced.param<bool>("write_tmcd", "Write timecode")
            .description("Force writing tmcd atom")
            .defaultValue(false);

        advanced.param<int>("write_prft", "Write producer ref. time")
            .description("Write producer reference time box with specified time source")
            .option("Automatic", 0)
            .option("Wallclock", 1)
            .option("PTS", 2)
            .defaultValue(0);

        advanced.param<bool>("empty_hdlr_name", "Empty HDLR name")
            .description("write zero-length name string in hdlr atoms within mdia and minf atoms")
            .defaultValue(false);

        advanced.param<int>("movie_timescale", "Movie timescale")
            .description("set movie timescale")
            .minValue(1)
            .maxValue(INT_MAX)
            .defaultValue(1000);

        registerAsset(info);
    }

    void FFmpegMuxerPlugin::registerGIF()
    {
        AssetInfo info;
        info.id = "gif";
        info.name = "GIF";
        info.category = std::make_pair("ffmpeg", "FFmpeg");
        info.description = "CompuServe Graphics Interchange Format";
        info.type = NodeInfoType::muxer;
        info.mediaType = MediaType::mux;
        info.helpUrl = "https://ffmpeg.org/ffmpeg-formats.html#" + info.id;
        info.allowedInputGroups = {
            AV_CODEC_ID_GIF
        };

        auto& advanced = info.group("advanced", "Advanced", ItemParamGroupType::Standard);
        advanced.param<int>("loop", "Loop")
            .description("Number of times to loop the output: -1 - no loop, 0 - infinite loop")
            .minValue(-1)
            .maxValue(65535)
            .defaultValue(0);

        advanced.param<int>("final_delay", "Final delay")
            .description("Force delay (in centiseconds) after the last frame")
            .minValue(-1)
            .maxValue(65535)
            .defaultValue(-1);

        registerAsset(info);
    }

    void FFmpegMuxerPlugin::registerFLV()
    {
        AssetInfo info;
        info.id = "flv";
        info.name = "FLV";
        info.category = std::make_pair("ffmpeg", "FFmpeg");
        info.description = "Flash Video";
        info.type = NodeInfoType::muxer;
        info.mediaType = MediaType::mux;
        info.helpUrl = "https://ffmpeg.org/ffmpeg-formats.html#flv";
        info.allowedInputGroups = {
            AV_CODEC_ID_FLV1, // Video
            AV_CODEC_ID_MP3 // Audio
        };

        auto& standard = info.group("standard", "Standard", ItemParamGroupType::Standard);
        standard.param<flags>("flvflags", "FLV flags")
            .description("FLV flags")
            .flag("aac_seq_header_detect")
            .flag("no_sequence_end")
            .flag("no_metadata")
            .flag("no_duration_filesize")
            .flag("add_keyframe_index")
            .defaultValue(0);

        registerAsset(info);
    }

    void FFmpegMuxerPlugin::registerAAC()
    {
        AssetInfo info;
        info.id = "adts";
        info.name = "AAC";
        info.category = std::make_pair("ffmpeg", "FFmpeg");
        info.description = "ADTS AAC (Advanced Audio Coding)";
        info.type = NodeInfoType::muxer;
        info.mediaType = MediaType::mux;
        info.helpUrl = "https://ffmpeg.org/ffmpeg-formats.html#" + info.id;
        info.allowedInputGroups = {
            AV_CODEC_ID_AAC
        };

        const AVOutputFormat* format = av_guess_format(info.id.c_str(), NULL, NULL);
        if (format)
            createFFmpegParameters(info, format->priv_class);

        registerAsset(info);
    }

    void FFmpegMuxerPlugin::registerAC3()
    {
        AssetInfo info;
        info.id = "ac3";
        info.name = "AC3";
        info.category = std::make_pair("ffmpeg", "FFmpeg");
        info.description = "Audio Codec 3";
        info.type = NodeInfoType::muxer;
        info.mediaType = MediaType::mux;
        info.helpUrl = "https://ffmpeg.org/ffmpeg-formats.html#" + info.id;
        info.allowedInputGroups = {
            AV_CODEC_ID_AC3
        };

        const AVOutputFormat* format = av_guess_format(info.id.c_str(), NULL, NULL);
        if (format)
            createFFmpegParameters(info, format->priv_class);

        registerAsset(info);
    }

    void FFmpegMuxerPlugin::registerAVI()
    {
        AssetInfo info;
        info.id = "avi";
        info.name = "AVI";
        info.category = std::make_pair("ffmpeg", "FFmpeg");
        info.description = "Audio Video Interleave";
        info.type = NodeInfoType::muxer;
        info.mediaType = MediaType::mux;
        info.helpUrl = "https://ffmpeg.org/ffmpeg-formats.html#" + info.id;
        info.allowedInputGroups = {
            AV_CODEC_ID_FFV1, AV_CODEC_ID_H264, AV_CODEC_ID_HEVC, AV_CODEC_ID_VP8, AV_CODEC_ID_VP9, AV_CODEC_ID_HAP, AV_CODEC_ID_MPEG2VIDEO, AV_CODEC_ID_AV1, AV_CODEC_ID_UTVIDEO, AV_CODEC_ID_CFHD,
            AV_CODEC_ID_AAC, AV_CODEC_ID_AC3, AV_CODEC_ID_EAC3, AV_CODEC_ID_FLAC, AV_CODEC_ID_MP3, AV_CODEC_ID_VORBIS, AV_CODEC_ID_PCM_S16LE, AV_CODEC_ID_PCM_S24LE, AV_CODEC_ID_PCM_S32LE
        };

        const AVOutputFormat* format = av_guess_format(info.id.c_str(), NULL, NULL);
        if (format)
            createFFmpegParameters(info, format->priv_class);

        registerAsset(info);
    }

    void FFmpegMuxerPlugin::registerMP2()
    {
        AssetInfo info;
        info.id = "mp2";
        info.name = "MP2";
        info.category = std::make_pair("ffmpeg", "FFmpeg");
        info.description = "MPEG 1 Layer 2";
        info.type = NodeInfoType::muxer;
        info.mediaType = MediaType::mux;
        info.helpUrl = "https://ffmpeg.org/ffmpeg-formats.html#" + info.id;
        info.allowedInputGroups = {
            AV_CODEC_ID_MP2
        };

        const AVOutputFormat* format = av_guess_format(info.id.c_str(), NULL, NULL);
        if (format)
            createFFmpegParameters(info, format->priv_class);

        registerAsset(info);
    }

    void FFmpegMuxerPlugin::registerMP3()
    {
        AssetInfo info;
        info.id = "mp3";
        info.name = "MP3";
        info.category = std::make_pair("ffmpeg", "FFmpeg");
        info.description = "MPEG 1 Layer 3";
        info.type = NodeInfoType::muxer;
        info.mediaType = MediaType::mux;
        info.helpUrl = "https://ffmpeg.org/ffmpeg-formats.html#" + info.id;
        info.allowedInputGroups = {
            AV_CODEC_ID_MP3
        };

        const AVOutputFormat* format = av_guess_format(info.id.c_str(), NULL, NULL);
        if (format)
            createFFmpegParameters(info, format->priv_class);

        registerAsset(info);
    }

    void FFmpegMuxerPlugin::registerMPEG2Video()
    {
        AssetInfo info;
        info.id = "mpeg2video";
        info.name = "MPEG-2";
        info.category = std::make_pair("ffmpeg", "FFmpeg");
        info.description = "MPEG-2 Video";
        info.type = NodeInfoType::muxer;
        info.mediaType = MediaType::mux;
        info.helpUrl = "https://ffmpeg.org/ffmpeg-formats.html#" + info.id;
        info.allowedInputGroups = {
            AV_CODEC_ID_MPEG2VIDEO
        };

        registerAsset(info);
    }

    void FFmpegMuxerPlugin::registerWAV()
    {
        AssetInfo info;
        info.id = "wav";
        info.name = "WAV";
        info.category = std::make_pair("ffmpeg", "FFmpeg");
        info.description = "WAV";
        info.type = NodeInfoType::muxer;
        info.mediaType = MediaType::mux;
        info.helpUrl = "https://ffmpeg.org/ffmpeg-formats.html#" + info.id;
        info.allowedInputGroups = {
            AV_CODEC_ID_PCM_S16LE, AV_CODEC_ID_PCM_S24LE, AV_CODEC_ID_PCM_S32LE
        };

        const AVOutputFormat* format = av_guess_format(info.id.c_str(), NULL, NULL);
        if (format)
            createFFmpegParameters(info, format->priv_class);

        registerAsset(info);
    }

    void FFmpegMuxerPlugin::registerWavpack()
    {
        AssetInfo info;
        info.id = "wv";
        info.name = "Wavpack";
        info.category = std::make_pair("ffmpeg", "FFmpeg");
        info.description = "Wavpack";
        info.type = NodeInfoType::muxer;
        info.mediaType = MediaType::mux;
        info.helpUrl = "https://ffmpeg.org/ffmpeg-formats.html#" + info.id;
        info.allowedInputGroups = {
            AV_CODEC_ID_WAVPACK
        };

        const AVOutputFormat* format = av_guess_format(info.id.c_str(), NULL, NULL);
        if (format)
            createFFmpegParameters(info, format->priv_class);

        registerAsset(info);
    }

    void FFmpegMuxerPlugin::registerWEBM()
    {
        AssetInfo info;
        info.id = "webm";
        info.name = "WebM";
        info.category = std::make_pair("ffmpeg", "FFmpeg");
        info.description = "WebM";
        info.type = NodeInfoType::muxer;
        info.mediaType = MediaType::mux;
        info.helpUrl = "https://ffmpeg.org/ffmpeg-formats.html#" + info.id;
        info.allowedInputGroups = {
            AV_CODEC_ID_VP9, AV_CODEC_ID_AV1,
            AV_CODEC_ID_OPUS, AV_CODEC_ID_VORBIS
        };

        const AVOutputFormat* format = av_guess_format(info.id.c_str(), NULL, NULL);
        if (format)
            createFFmpegParameters(info, format->priv_class);

        registerAsset(info);
    }

}
