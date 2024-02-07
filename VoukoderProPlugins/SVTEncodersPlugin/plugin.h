#pragma once

#include <functional>

#include "../../PluginInterface/plugin_api.h"

namespace VoukoderPro
{
	class SVTEncodersPlugin : public EncoderPlugin
	{
	public:
		SVTEncodersPlugin();

	public:
		static std::shared_ptr<SVTEncodersPlugin> CreateInstance()
		{
			return std::make_shared<SVTEncodersPlugin>();
		}

	private:
		int registerAV1();
	};

	BOOST_DLL_ALIAS(VoukoderPro::SVTEncodersPlugin::CreateInstance, CreateInstance)
}
