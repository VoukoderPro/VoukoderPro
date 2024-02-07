#pragma once

#include <functional>

#include "../../PluginInterface/plugin_api.h"

namespace VoukoderPro
{
	class ExecutePostProcAsset : public Asset
	{
	public:
		ExecutePostProcAsset(const AssetInfo& info);
		int open(nlohmann::ordered_json& params);
		int close();

	private:
		nlohmann::ordered_json params;
	};

	class ExecutePostProcPlugin : public PostProcPlugin
	{
	public:
		ExecutePostProcPlugin();
		std::shared_ptr<Asset> createAsset(const AssetInfo& info);

	public:
		static std::shared_ptr<ExecutePostProcPlugin> CreateInstance()
		{
			return std::make_shared<ExecutePostProcPlugin>();
		}
	};

	BOOST_DLL_ALIAS(VoukoderPro::ExecutePostProcPlugin::CreateInstance, CreateInstance)
}
