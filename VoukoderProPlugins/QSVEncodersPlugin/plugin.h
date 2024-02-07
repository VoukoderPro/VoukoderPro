#pragma once

#include <functional>

#include "../../PluginInterface/plugin_api.h"

namespace VoukoderPro
{
	class QSVEncodersPlugin : public EncoderPlugin
	{
	public:
		QSVEncodersPlugin();

	public:
		static std::shared_ptr<QSVEncodersPlugin> CreateInstance()
		{
			return std::make_shared<QSVEncodersPlugin>();
		}

	private:
		int registerAV1();
		int registerH264();
		int registerHEVC();
		int registerVP9();
	};

	BOOST_DLL_ALIAS(VoukoderPro::QSVEncodersPlugin::CreateInstance, CreateInstance)
}
