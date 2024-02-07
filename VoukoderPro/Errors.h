#pragma once

#define ERR_OK 0
#define ERR_FAIL -1
#define ERR_PARAMETER_MISSING -2
#define ERR_PARAMETER_VALUE_NOT_SUPPORTED -3
#define ERR_MEDIA_TYPE_NOT_SUPPORTED -4
#define ERR_PLUGIN_NOT_SET -5
#define ERR_FILTER_NOT_FOUND -6
#define ERR_BUFFER_SINK -7
#define ERR_INPUT_NODE_REQUIRED -8
#define ERR_MUXER_NOT_FOUND -9
#define ERR_STREAM_NOT_FOUND -10
#define ERR_FFMPEG(err) -11
#define ERR_FILENAME_IN_USE -12
#define ERR_DATABASE -13
#define ERR_NO_LICENSE -99998
#define ERR_EXPIRED -99999

static std::string Id2Error(int id)
{
	switch (id)
	{
	case ERR_OK: return "No error.";
	case ERR_FAIL: return "General failure.";
	case ERR_PARAMETER_MISSING: return "A parameter is missing.";
	case ERR_PARAMETER_VALUE_NOT_SUPPORTED: return "A parameter value is not supported.";
	case ERR_MEDIA_TYPE_NOT_SUPPORTED: return "This media type is not supported.";
	case ERR_PLUGIN_NOT_SET: return "Plugin instance is not set.";
	case ERR_FILTER_NOT_FOUND: return "The filter was not found.";
	case ERR_BUFFER_SINK: return "Buffer sink error.";
	case ERR_INPUT_NODE_REQUIRED: return "An input node is required.";
	case ERR_MUXER_NOT_FOUND: return "The muxer was not found.";
	case ERR_STREAM_NOT_FOUND: return "The stream was not found.";
	case ERR_FFMPEG(0): return "FFmpeg error.";
	case ERR_FILENAME_IN_USE: return "The filename is already in use by another output node.";
	case ERR_DATABASE: return "Database error.";
	case ERR_NO_LICENSE: return "No license!";
	case ERR_EXPIRED: return "The VoukoderPro license is expired!";
	default:
		return "Undefined error!";
	}
}


