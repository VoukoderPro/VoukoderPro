#pragma once

#include <functional>

#include "../../PluginInterface/plugin_api.h"

namespace VoukoderPro
{	
	// Types from nvcuda.dll
	typedef int(*__cuInit)(int* flags);
	typedef int(*__cuDeviceGetCount)(int* count);
	typedef int(*__cuDeviceGetName)(char* name, int len, int dev);

	class FFmpegFiltersPlugin : public FilterPlugin
	{
	public:
		FFmpegFiltersPlugin();

	public:
		static std::shared_ptr<FFmpegFiltersPlugin> CreateInstance()
		{
			return std::make_shared<FFmpegFiltersPlugin>();
		}

	private:
		void createFilter(const std::string id, const std::string name, const MediaType type);
		void createZScaleFilter();
		void createScaleFilter();
		void createHWUploadCudaFilter();

	private:
		std::vector<std::string> devices;
	};

	BOOST_DLL_ALIAS(VoukoderPro::FFmpegFiltersPlugin::CreateInstance, CreateInstance)
}
