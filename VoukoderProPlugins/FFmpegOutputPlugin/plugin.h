#pragma once

#include <functional>

#include "../../PluginInterface/plugin_api.h"

namespace VoukoderPro
{
	class FFmpegOutputsPlugin : public OutputPlugin
	{
	public:
		FFmpegOutputsPlugin();

	public:
		static std::shared_ptr<FFmpegOutputsPlugin> CreateInstance()
		{
			return std::make_shared<FFmpegOutputsPlugin>();
		}
	};

	BOOST_DLL_ALIAS(VoukoderPro::FFmpegOutputsPlugin::CreateInstance, CreateInstance)
}
