#include "PostProcNode.h"

#include <filesystem>
#include <boost/algorithm/string.hpp>

namespace VoukoderPro
{
	PostProcNode::PostProcNode(std::shared_ptr<NodeInfo> nodeInfo):
		BaseNode(nodeInfo)
	{}

	int PostProcNode::init()
	{
		// Check for plugin instance
		if (plugins.size() < 1)
		{
			BLOG(error) << "The post processing node requires a plugin instance.";
			return ERR_PLUGIN_NOT_SET;
		}

		int ret = ERR_OK;

		// Initialize plugin
		for (const auto& it : plugins)
		{
			if ((ret = it.second->init(data.begin()->second->trackProperties)) < ERR_OK)
			{
				BLOG(error) << "Initializing the post processing plugin failed with error code: " << ret;

				return ret;
			}
		}

		return BaseNode::init();
	}

	int PostProcNode::open()
	{
		int ret = ERR_OK;

		const auto& nleData = data.begin()->second;

		// Generate variables
		std::map<std::string, std::string> variables;
		if (nleData->properties.find(pPropFilename) != nleData->properties.end())
		{
			std::string filename = std::get<std::string>(nleData->properties["filename"]);

			// Fill variable values
			std::filesystem::path fileinfo(filename);
			variables[pVarFilePath] = fileinfo.parent_path().string();
			variables[pVarFileName] = fileinfo.stem().string();
			variables[pVarFileExtension] = fileinfo.extension().string();
			variables[pVarFileFullname] = filename;
		}

		// Generate plugin params
		nlohmann::ordered_json params;
		if (nodeInfo->data.contains("params") && nodeInfo->data.count("params") > 0)
			params = nodeInfo->data.at("params");

		for (auto& [key, param] : params.items())
		{
			if (!param.is_string())
				continue;

			std::string value = param.get<std::string>();

			// Replace variables
			for (const auto& [varName, varValue] : variables)
				boost::algorithm::replace_all(value, "$(" + varName + ")", varValue);

			params[key] = value;
		}

		// Open plugin
		for (const auto& it : plugins)
		{
			if ((ret = it.second->open(params)) < ERR_OK)
			{
				BLOG(error) << "Opening the post processing plugin failed with error code: " << ret;
				return ret;
			}
		}

		return BaseNode::open();
	}

	int PostProcNode::close()
	{
		BLOG(severity_level::info) << "Starting post processing action ...";

		int ret = ERR_OK;

		// Closing plugin
		for (const auto& [nleTrackIndex, plugin] : plugins)
		{
			if ((ret = plugin->close()) < ERR_OK)
			{
				BLOG(error) << "Post processing action for NLE track #" << std::to_string(nleTrackIndex) << "failed with error code: " << ret;
				return ret;
			}
		}

		return BaseNode::close();
	}
}
