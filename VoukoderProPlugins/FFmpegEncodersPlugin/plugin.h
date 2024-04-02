#pragma once

#include <functional>

#include "../../PluginInterface/plugin_api.h"

namespace VoukoderPro
{
	class FFmpegEncodersPlugin : public EncoderPlugin
	{
	public:
		FFmpegEncodersPlugin();

	private:
		void addGlobalOptions(AssetInfo& info, AVCodecContext* codecCtx, int flags);

	public:
		static std::shared_ptr<FFmpegEncodersPlugin> CreateInstance()
		{
			return std::make_shared<FFmpegEncodersPlugin>();
		}
	};

	BOOST_DLL_ALIAS(VoukoderPro::FFmpegEncodersPlugin::CreateInstance, CreateInstance)
}
