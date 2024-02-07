#pragma once

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/opt.h>
#include <libavutil/pixdesc.h>
#include <libavutil/channel_layout.h>
#include <libavfilter/avfilter.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libavutil/hwcontext.h>
#include <libavutil/mastering_display_metadata.h>
}

#pragma comment(lib, "avformat-voukoderpro.lib")
#pragma comment(lib, "avcodec-voukoderpro.lib")
#pragma comment(lib, "avfilter-voukoderpro.lib")
#pragma comment(lib, "avutil-voukoderpro.lib")
#pragma comment(lib, "swscale-voukoderpro.lib")
#pragma comment(lib, "swresample-voukoderpro.lib")
