#pragma once

#include <functional>

#include "../../PluginInterface/plugin_api.h"

namespace VoukoderPro
{
	class LameEncodersPlugin: public EncoderPlugin
	{
	public:
		LameEncodersPlugin();

	public:
		static std::shared_ptr<LameEncodersPlugin> CreateInstance()
		{
			return std::make_shared<LameEncodersPlugin>();
		}
	};

	BOOST_DLL_ALIAS(VoukoderPro::LameEncodersPlugin::CreateInstance, CreateInstance)
}
