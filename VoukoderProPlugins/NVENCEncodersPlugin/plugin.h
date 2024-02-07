#pragma once

#include <functional>

#include "../../PluginInterface/plugin_api.h"

namespace VoukoderPro
{
	// Types from nvcuda.dll
	typedef int(*__cuInit)(int* flags);
	typedef int(*__cuDeviceGetCount)(int* count);
	typedef int(*__cuDeviceGetName)(char* name, int len, int dev);

	class NVENCEncodersPlugin : public EncoderPlugin
	{
	public:
		NVENCEncodersPlugin();

	public:
		static std::shared_ptr<NVENCEncodersPlugin> CreateInstance()
		{
			return std::make_shared<NVENCEncodersPlugin>();
		}

	private:
		int registerAV1();
		int registerH264();
		int registerHEVC();
		void addDeviceProperty(ItemParamGroup& group);

	private:
		std::vector<std::string> devices;
	};

	BOOST_DLL_ALIAS(VoukoderPro::NVENCEncodersPlugin::CreateInstance, CreateInstance)
}
