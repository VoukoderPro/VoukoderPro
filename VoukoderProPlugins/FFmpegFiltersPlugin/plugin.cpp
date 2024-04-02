#include "plugin.h"

#include <Windows.h>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace VoukoderPro
{
    FFmpegFiltersPlugin::FFmpegFiltersPlugin()
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

		//AVFilterGraph* filterGraph = avfilter_graph_alloc();

		//void* opaque = NULL;
		//const AVFilter* filter = NULL;
		//while ((filter = av_filter_iterate(&opaque)))
		//{
		//	AVFilterContext* filterCtx = avfilter_graph_alloc_filter(filterGraph, filter, filter->name);

		//	AssetInfo info;
		//	info.id = filter->name;
		//	info.name = filter->name;
		//	info.description = filter->description;
		//	info.type = NodeInfoType::filter;
		//	//info.mediaType = M
		//	info.helpUrl = "https://ffmpeg.org/ffmpeg-filters.html#" + info.id;


		//	for (unsigned j = 0; j < filterCtx->nb_inputs; j++)
		//	{
		//		filterCtx->inputs[j]->

		//			// Type (Audio / video, 
		//	}
		//	av_opt_next()


		//	for (int i = 0; i < filterCtx->nb_inputs; i++) {
		//		AVFilterPad* pad = filterCtx->input_pads[i];
		//		if (!pad)
		//			continue;
		//		printf("  Input Pad: %s\n", pad->name);
		//	}

		//	// Select the right category
		//	if (boost::algorithm::ends_with(id, "_cuda") || id == "hwdownload")
		//		info.category = std::make_pair("cuda", "CUDA");
		//	else if (boost::algorithm::ends_with(id, "_qsv"))
		//		info.category = std::make_pair("qsv", "QSV");
		//	else
		//		info.category = std::make_pair("ffmpeg", "FFmpeg");

		//	// Iterate over filter options
		//	createFFmpegParameters(info, filter->priv_class);

		//	registerAsset(info);
		//}

		std::map<std::string, std::string> audioFilters = {
			{ "acompressor", "Compressor" },
			{ "acontrast", "Contrast" },
			{ "acrusher", "Crusher" },
			{ "adeclick", "Declick" },
			{ "adeclip", "Declip" },
			{ "adecorrelate", "Decorrelate" },
			{ "adelay", "Delay" },
			{ "adenorm", "Denormalize" },
			{ "adrc", "Dynanmic Range Control" },
			{ "adynamicequalizer", "Dynamic Equalizer" },
			{ "adynamicsmooth", "Dynamic Smooting" },
			{ "aecho", "Echo" },
			{ "aemphasis", "Emphasis" },
			{ "aexciter", "Exciter" },
			{ "afade", "Fade" },
			{ "afftdn", "FFT Denoise" },
			{ "afftfilt", "FFT Filter" },
			{ "afreqshift", "Frequency Shift" },
			{ "afwtdn", "Wavelet Denoise" },
			{ "agate", "Gate" },
			{ "alimiter", "Limiter" },
			{ "allpass", "All-Pass" },
			{ "aloop", "Loop" },
			{ "anlmdn", "Non-Local Means Denoise" },
			{ "apad", "Pad" },
			{ "aphaser", "Phaser" },
			{ "aphaseshift", "Phase Shift" },
			{ "apsyclip", "Psychoacoustic Clipper" },
			{ "apulsator", "Pulsator" },
			{ "arealtime", "Realtime" },
			{ "aresample", "Resample" },
			{ "areverse", "Reverse" },
			{ "asoftclip", "Soft Clip" },
			{ "asubboost", "Sub Boost" },
			{ "asubcut", "Sub Cut" },
			{ "asupercut", "Super Cut" },
			{ "asuperpass", "Super Pass" },
			{ "asuperstop", "Super Stop" },
			{ "atempo", "Tempo" },
			{ "atilt", "Tilt" },
			{ "atrim", "Trim" },
			{ "bandpass", "Band Pass" },
			{ "bandreject", "Band Reject" },
			{ "bass", "Bass" },
			{ "biquad", "Bi-Quad" },
			{ "compand", "Compand" },
			{ "compensationdelay", "Compensation Delay" },
			{ "crossfeed", "Crossfeed" },
			{ "crystalizer", "Crystalizer" },
			{ "dcshift", "DC Shift" },
			{ "deesser", "De-Esser" },
			{ "dialoguenhance", "Dialogue Enhancer" },
			{ "dynaudnorm", "Dynamic Normalizer" },
			{ "earwax", "Ear wax" },
			{ "equalizer", "Equalizer" },
			{ "extrastereo", "Extra Stereo" },
			{ "firequalizer", "FIR Equalizer" },
			{ "flanger", "Flanger" },
			{ "haas", "Haas" },
			{ "highpass", "High Pass" },
			{ "highshelf", "High Shelf" },
			{ "lowpass", "Low Pass" },
			{ "lowshelf", "Low Shelf" },
			{ "mcompand", "Multipand Compressor" },
			{ "replaygain", "Replay Gain" },
			{ "silencedetect", "Silence Detect" },
			{ "silenceremove", "Silence Remove" },
			{ "speechnorm", "Speech Normalizer" },
			{ "stereotools", "Stereo Tools" },
			{ "stereowiden", "Stereo Widen" },
			{ "superequalizer", "Super Equalizer" },
			{ "surround", "Surround" },
			{ "tiltshelf", "Tilt Shelf" },
			{ "treble", "Treble" },
			{ "tremolo", "Tremolo" },
			{ "vibrato", "Vibrato" },
			{ "virtualbass", "Virtual Bass" },
			{ "volume", "Volume" },
		};

		// Add white-listed audio filters
		for (const auto& [id, name] : audioFilters)
			createFilter(id, name, MediaType::audio);

		std::map<std::string, std::string> videoFilters = {
			{ "addroi", "Add Range of Interest" },
			{ "amplify", "Amplify" },
			{ "atadenoise", "Adaptive Temporal Average Denoise" },
			{ "avgblur", "Blur (Average)" },
			{ "backgroundkey", "Background Key" },
			{ "bbox", "Bounding Box" },
			{ "bilateral", "Bilateral" },
			{ "bitplanenoise", "Bitplane Noise" },
			{ "blackdetect", "Black Detect" },
			{ "blockdetect", "Block Detect" },
			{ "blurdetect", "Blur Detect" },
			{ "bwdif", "Deinterlace" },
			{ "cas", "Contrast Adaptive Sharpen" },
			{ "chromahold", "Chroma Hold" },
			{ "chromakey", "Chroma Key" },
			{ "chromanr", "Chroma Noise Reduction" },
			{ "chromashift", "Chroma Shift" },
			{ "ciescope", "CIE Scope" },
			{ "codecview", "Codec View" },
			{ "colorbalance", "Color Balance" },
			{ "colorchannelmixer", "Color Channel Mixer" },
			{ "colorcontrast", "Color Contrast" },
			{ "colorcorrect", "Color Correct" },
			{ "colorize", "Colorize" },
			{ "colorkey", "Color Key" },
			{ "colorhold", "Color Hold" },
			{ "colorlevels", "Color Levels" },
			//{ "colorspace", "Color Space" },
			{ "colortemperature", "Color Temperature" },
			{ "convolution", "Convolution" },
			{ "crop", "Crop" },
			{ "cue", "Cue" },
			{ "curves", "Curves" },
			{ "datascope", "Datascope" },
			{ "dblur", "Directional Blur" },
			{ "dctdnoiz", "Denoise (2D DCT)" },
			{ "deband", "Deband" },
			{ "deblock", "Deblock" },
			{ "dedot", "Dedot" },
			{ "deflate", "Deflate" },
			{ "deflicker", "Deflicker" },
			{ "deshake", "Deshake" },
			{ "despill", "Despill" },
			{ "dilation", "Dilation" },
			{ "doubleweave", "Double Weave" },
			{ "drawbox", "Draw Box" },
			{ "drawgraph", "Draw Graph" },
			{ "drawgrid", "Draw Grid" },
			//{ "edgedetect", "Edge Detect" },
			{ "elbg", "Posterize (ELBG)" },
			{ "entropy", "Entropy" },
			{ "erosion", "Erosion" },
			{ "estdif", "Deinterlace (Edge Slope)" },
			{ "exposure", "Exposure" },
			{ "fade", "Fade" },
			{ "fftdnoiz", "FFT Denoise" },
			{ "fftfilt", "FFT Filter" },
			{ "field", "Field" },
			{ "fieldorder", "Field Order" },
			{ "fillborders", "Fill Borders" },
			{ "floodfill", "Flood Fill" },
			{ "fps", "FPS" },
			{ "framerate", "Framerate" },
			{ "framestep", "Frame Step" },
			{ "freezedetect", "Freeze Detect" },
			{ "gblur", "Gaussian Blur" },
			{ "gradfun", "Gradient Fun" },
			{ "graphmonitor", "Graph Monitor" },
			{ "grayworld", "Gray World" },
			{ "greyedge", "Gray Edge" },
			{ "hflip", "Horizontal Flip" },
			{ "hsvhold", "HSV Hold" },
			{ "hsvkey", "HSV Key" },
			{ "hue", "Hue" },
			{ "huesaturation", "Hue & Saturation" },
			{ "idet", "Interlace Detect" },
			{ "il", "Interleave" },
			{ "inflate", "Inflate" },
			{ "kirsch", "Kirsch" },
			{ "lagfun", "Lag Fun" },
			{ "latency", "Latency" },
			{ "lenscorrection", "Lens Correction" },
			{ "limiter", "Limiter" },
			{ "loop", "Loop" },
			{ "lumakey", "Luma Key" },
			{ "lut", "LUT" },
			{ "lut1d", "LUT 1D" },
			{ "lut3d", "LUT 3D" },
			{ "lutrgb", "LUT RGB" },
			{ "lutyuv", "LUT YUV" },
			{ "maskfun", "Mask Fun" },
			{ "median", "Median" },
			{ "mestimate", "Motion Estimate" },
			{ "minterpolate", "Motion Interpolate" },
			{ "monochrome", "Monochrome" },
			{ "negate", "Negate" },
			{ "nlmeans", "Non-Local Means Denoiser" },
			{ "noise", "Noise" },
			{ "normalize", "Normalize" },
			{ "oscilloscope", "Oscilloscope" },
			{ "pad", "Pad" },
			{ "photosensitivity", "Photo Sensitivity" },
			{ "pixelize", "Pixelize" },
			{ "pixscope", "Pixel Scope" },
			{ "prewitt", "Prewitt" },
			{ "pseudocolor", "Pseudo Color" },
			{ "qp", "Quantization Parameters" },
			{ "random", "Random" },
			{ "realtime", "Realtime" },
			{ "removegrain", "Remove Grain" },
			{ "reverse", "Reverse" },
			{ "rgbashift", "RGBA Shift" },
			{ "roberts", "Roberts" },
			{ "rotate", "Rotate" },
			//{ "scale", "Scale" },
			{ "scdet", "Scene Change Detect" },
			{ "scharr", "Scharr" },
			{ "scroll", "Scholl" },
			{ "selectivecolor", "Selective Color" },
			{ "separatefields", "Separate Fields" },
			{ "setdar", "Set DAR" },
			{ "setfield", "Set Field" },
			{ "setparams", "Set Params" },
			{ "setpts", "Set PTS" },
			{ "setrange", "Set Range" },
			{ "setsar", "Set SAR" },
			{ "settb", "Set Timebase" },
			{ "shear", "Shear" },
			{ "showinfo", "Show Info" },
			{ "showpalette", "Show Palette" },
			{ "shuffleframes", "Shuffle Frames" },
			{ "shufflepixels", "Shuffle Pixels" },
			{ "shuffleplanes", "Shuffle Planes" },
			{ "signalstats", "Signal Stats" },
			{ "siti", "SITI" },
			{ "sobel", "Sobel" },
			{ "swaprect", "Swap Rect" },
			{ "swapuv", "Swap UV" },
			{ "tblend", "Temporal Blend" },
			{ "thumbnail", "Thumbnail" },
			{ "tlut2", "Temporal LUT" },
			{ "tmedian", "Temporal Median" },
			{ "tmidequalizer", "Temporal Midway EQ" },
			{ "tmix", "Temporal Mix" },
			{ "tonemap", "Tonemap" },
			{ "tpad", "Temporal Pad" },
			{ "transpose", "Transpose" },
			{ "trim", "Trim" },
			{ "unsharp", "Unsharp" },
			{ "v360", "Convert 360 Projection" },
			{ "vflip", "Vertical Flip" },
			{ "vfrdet", "VFR Detect" },
			{ "vibrance", "Vibrance" },
			{ "vignette", "Vignette" },
			{ "w3fdif", "Martin Weston Deinterlace" },
			{ "weave", "Weave" },
			{ "yadif", "Yadif" },
			{ "yaepblur", "YAEP Blur" },
			{ "zoompan", "Zoom & Pan" },
			//{ "zscale", "ZScale" },

			// CUDA
			{ "bilateral_cuda", "Bilateral" },
			{ "chromakey_cuda", "Chroma Key" },
			{ "colorspace_cuda", "Color Space" },
			//{ "hwupload_cuda", "Upload" },
			{ "hwdownload", "Download" },
			{ "overlay_cuda", "Overlay" },
			{ "scale_cuda", "Scale" },
			{ "thumbnail_cuda", "Thumbnail" },
			{ "yadif_cuda", "Yadif" },

			// QSV
			//{ "deinterlace_qsv", "Deinterlace" },
			//{ "overlay_qsv", "Overlay" },
			//{ "scale_qsv", "Scale" },
			//{ "vpp_qsv", "VPP" },
			//{ "hstack_qsv", "HStack" },
			//{ "vstack_qsv", "VStack" },
			//{ "xstack_qsv", "XStack" },
			//{ "hwupload", "Upload" },
		};

		// Add white-listed video filters
		for (const auto& [id, name] : videoFilters)
			createFilter(id, name, MediaType::video);

		createScaleFilter();
		createZScaleFilter();
		createHWUploadCudaFilter();
	}

    void FFmpegFiltersPlugin::createFilter(const std::string id, const std::string name, const MediaType type)
    {
        const AVFilter* filter = avfilter_get_by_name(id.c_str());
        if (!filter)
            return;

        AssetInfo info;
        info.id = filter->name;
        info.name = name;
        info.description = filter->description;
        info.type = NodeInfoType::filter;
        info.mediaType = type;
        info.helpUrl = "https://ffmpeg.org/ffmpeg-filters.html#" + info.id;

		// Select the right category
		if (boost::algorithm::ends_with(id, "_cuda") || id == "hwdownload")
			info.category = std::make_pair("cuda", "CUDA");
		else if (boost::algorithm::ends_with(id, "_qsv"))
			info.category = std::make_pair("qsv", "QSV");
		else
			info.category = std::make_pair("ffmpeg", "FFmpeg");

        // Iterate over filter options
		addPrivateOptions(info, filter->priv_class);

        registerAsset(info);
    }

	void FFmpegFiltersPlugin::createScaleFilter()
	{
		AssetInfo info;
		info.id = "scale";
		info.name = "Scale";
		info.description = "";
		info.type = NodeInfoType::filter;
		info.mediaType = MediaType::video;
		info.category = std::make_pair("ffmpeg", "FFmpeg");
		info.helpUrl = "https://ffmpeg.org/ffmpeg-filters.html#" + info.id;

		auto& basic = info.group("basic", "Basic", ItemParamGroupType::Forced);
		basic.param<int>("width", "Width")
			.description("Output video width")
			.minValue(320)
			.maxValue(16384)
			.defaultValue(0);

		basic.param<int>("height", "Height")
			.description("Output video height")
			.minValue(240)
			.maxValue(8640)
			.defaultValue(0);

		basic.param<std::string>("flags", "Quality")
			.description("The quality as a variable bit rate in kbit/s per channel.")
			.option("Fast bilinear", "fast_bilinear")
			.option("Bilinear", "bilinear")
			.option("Bicubic", "bicubic")
			.option("Nearest neighbor", "neighbor")
			.option("Averaging area", "area")
			.option("Luma bicubic, Chroma bilinear", "bicublin")
			.option("Gaussian", "gauss")
			.option("Sinc", "sinc")
			.option("Lanczos", "lanczos")
			.option("Spline", "spline")
			.defaultValue("bicubic");

		registerAsset(info);
	}

	void FFmpegFiltersPlugin::createZScaleFilter()
	{
		AssetInfo info;
		info.id = "zscale";
		info.name = "ZScale";
		info.description = "";
		info.type = NodeInfoType::filter;
		info.mediaType = MediaType::video;
		info.category = std::make_pair("ffmpeg", "FFmpeg");
		info.helpUrl = "https://ffmpeg.org/ffmpeg-filters.html#" + info.id;

		auto& basic = info.group("basic", "Basic", ItemParamGroupType::Forced);
		basic.param<int>("width", "Width")
			.description("Output video width")
			.minValue(320)
			.maxValue(16384)
			.defaultValue(0);

		basic.param<int>("height", "Height")
			.description("Output video height")
			.minValue(240)
			.maxValue(8640)
			.defaultValue(0);

		basic.param<std::string>("dither", "Dither")
			.description("Set dither type.")
			.option("None", "none")
			.option("Ordered", "ordered")
			.option("Random", "random")
			.option("Error diffusion", "error_diffusion")
			.defaultValue("none");

		basic.param<std::string>("filter", "Filter")
			.description("Set filter type.")
			.option("Point", "point")
			.option("Bilinear", "bilinear")
			.option("Bicubic", "bicubic")
			.option("Spline 16", "spline16")
			.option("Spline 36", "spline36")
			.option("Lanczos", "lanczos")
			.defaultValue("bilinear");

		registerAsset(info);
	}

	void FFmpegFiltersPlugin::createHWUploadCudaFilter()
	{
		AssetInfo info;
		info.id = "hwupload_cuda";
		info.name = "Upload";
		info.description = "";
		info.type = NodeInfoType::filter;
		info.mediaType = MediaType::video;
		info.category = std::make_pair("cuda", "CUDA");
		info.helpUrl = "https://ffmpeg.org/ffmpeg-filters.html#" + info.id;

		auto& basic = info.group("basic", "Basic", ItemParamGroupType::Forced);
		auto& device = basic.param<int>("device", "Device")
			.description("CUDA device")
			.defaultValue(0);

		if (devices.size() > 0)
		{
			for (int i = 0; i < devices.size(); i++)
				device.option(std::to_string(i) + ": " + devices.at(i), i);
		}
		else if (devices.size() == 0)
			device.option("Auto", 0);

		registerAsset(info);
	}
}