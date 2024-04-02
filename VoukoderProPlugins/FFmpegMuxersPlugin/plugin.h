#pragma once

#include <functional>

#include "../../PluginInterface/plugin_api.h"

namespace VoukoderPro
{
	class FFmpegMuxerPlugin: public MuxerPlugin
	{
	public:
		FFmpegMuxerPlugin();

	public:
		static std::shared_ptr<FFmpegMuxerPlugin> CreateInstance()
		{
			return std::make_shared<FFmpegMuxerPlugin>();
		}
	};

	BOOST_DLL_ALIAS(VoukoderPro::FFmpegMuxerPlugin::CreateInstance, CreateInstance)
}
