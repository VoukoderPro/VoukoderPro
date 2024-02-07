#pragma once

#include <functional>

#include "../../PluginInterface/plugin_api.h"

namespace VoukoderPro
{
	class AMFEncodersPlugin : public EncoderPlugin
	{
	public:
		AMFEncodersPlugin();

	public:
		static std::shared_ptr<AMFEncodersPlugin> CreateInstance()
		{
			return std::make_shared<AMFEncodersPlugin>();
		}

	private:
		int registerAV1();
		int registerH264();
		int registerHEVC();
	};

	BOOST_DLL_ALIAS(VoukoderPro::AMFEncodersPlugin::CreateInstance, CreateInstance)
}
