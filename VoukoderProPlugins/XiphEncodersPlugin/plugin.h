#pragma once

#include <functional>

#include "../../PluginInterface/plugin_api.h"

namespace VoukoderPro
{
	class XiphEncoderPlugin: public EncoderPlugin
	{
	public:
		XiphEncoderPlugin();

	public:
		static std::shared_ptr<XiphEncoderPlugin> CreateInstance()
		{
			return std::make_shared<XiphEncoderPlugin>();
		}

	private:
		int registerOpus();
		int registerVorbis();
	};

	BOOST_DLL_ALIAS(VoukoderPro::XiphEncoderPlugin::CreateInstance, CreateInstance)
}
