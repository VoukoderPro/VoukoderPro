#include "SceneManager.h"

#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <boost/describe/enumerators.hpp>
#include <boost/describe/enum_from_string.hpp>
#include <boost/describe/enum_to_string.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/compute/detail/sha1.hpp>

#include "InputNode.h"
#include "FilterNode.h"
#include "EncoderNode.h"
#include "MuxerNode.h"
#include "OutputNode.h"
#include "PostProcNode.h"

#include "../3rdparty/nlohmann/include/json.hpp"
#include "../3rdparty/nlohmann/include/json-schema.hpp"
#include "json-root-schema.h"

#if _DEBUG
#pragma comment(lib, "../3rdparty/nlohmann/lib/nlohmann_json_schema_validatord.lib")
#else
#pragma comment(lib, "../3rdparty/nlohmann/lib/nlohmann_json_schema_validator.lib")
#endif

namespace VoukoderPro
{
	/**
    * NodeInfo > ordered_json
    */
	void to_json(nlohmann::ordered_json& j, const NodeInfo& p)
	{
		j = nlohmann::ordered_json{
			{ "id", p.id },
            { "type", boost::describe::enum_to_string<NodeInfoType>(p.type, "") },
			{ "mediaType", boost::describe::enum_to_string<MediaType>(p.mediaType, "") },
			{ "pos", {
				{ "x", p.posX },
				{ "y", p.posY }
			}},
			{ "data", p.data },
			{ "inputs", p.inputs },
			{ "outputs", p.outputs }
		};
	}

	/**
	* ordered_json > NodeInfo
	*/
	void from_json(const nlohmann::ordered_json& j, NodeInfo& p)
	{
		j.at("id").get_to(p.id);
		boost::describe::enum_from_string<NodeInfoType>(j["type"].get<std::string>().c_str(), p.type);
		boost::describe::enum_from_string<MediaType>(j["mediaType"].get<std::string>().c_str(), p.mediaType);
		j.at("pos").at("x").get_to(p.posX);
		j.at("pos").at("y").get_to(p.posY);
		j.at("inputs").get_to(p.inputs);
		j.at("outputs").get_to(p.outputs);
        j.at("data").get_to(p.data);
	}

	/**
	* SceneInfo > ordered_json
	*/
	void to_json(nlohmann::ordered_json& j, const SceneInfo& p)
	{
		j = nlohmann::ordered_json {
			{ "name", p.name },
			{ "nodes", nlohmann::ordered_json::array() }
		};

		for (const auto& node : p.nodes)
			j["nodes"].push_back(*node);
	}

	/**
	* ordered_json > SceneInfo
	*/
	void from_json(const nlohmann::ordered_json& j, SceneInfo& p)
	{
		j.at("name").get_to(p.name);

		for (nlohmann::ordered_json j : j.at("nodes"))
		{
			std::shared_ptr<NodeInfo> nodeInfo = std::make_shared<NodeInfo>();
			j.at("id").get_to(nodeInfo->id);
			boost::describe::enum_from_string<NodeInfoType>(j["type"].get<std::string>().c_str(), nodeInfo->type);
			boost::describe::enum_from_string<MediaType>(j["mediaType"].get<std::string>().c_str(), nodeInfo->mediaType);
			j.at("pos").at("x").get_to(nodeInfo->posX);
			j.at("pos").at("y").get_to(nodeInfo->posY);
			j.at("inputs").get_to(nodeInfo->inputs);
			j.at("outputs").get_to(nodeInfo->outputs);
			j.at("data").get_to(nodeInfo->data);

			p.nodes.push_back(nodeInfo);
		}
	}

	SceneManager::SceneManager():
		sceneFile(fs::path(VoukoderProData()) / "scenes.json")
	{}

	int SceneManager::load(std::vector<std::shared_ptr<SceneInfo>>& sceneInfos)
	{
		sceneInfos.clear();

		// If the path doesn't exists return
		if (!fs::exists(sceneFile))
			return 0;

		nlohmann::ordered_json json;

		// Load scene file
		std::ifstream inputStream(sceneFile.string());
		if (inputStream.fail())
		{
			BLOG(error) << "Unable to load config file: " << sceneFile;

			return -1;
		}

		// Parse JSON
		try
		{
			json = nlohmann::ordered_json::parse(inputStream);
		}
		catch (nlohmann::json::parse_error p)
		{
			BLOG(error) << "Unable to parse JSON file " << sceneFile << " - reason: " << p.what();

			return -1;
		}

		// Validate the loaded json file
		try
		{
			nlohmann::json_schema::json_validator validator;
			validator.set_root_schema(voukoderpro_schema);
			validator.validate(json);
		}
		catch (const std::exception& e)
		{
			BLOG(error) << "Unable to validate scene file against schema - reason: " << e.what();

			return -1;
		}

		// Read in all scenes
		for (const auto& [id, scene] : json.items())
			sceneInfos.push_back(std::make_shared<SceneInfo>(scene));

		// Sort the configurations
		std::sort(sceneInfos.begin(), sceneInfos.end(), [](std::shared_ptr<SceneInfo> const& a, std::shared_ptr<SceneInfo> const& b)
			{
				return a->name < b->name;
			});

		return 0;
	}

	int SceneManager::save(const std::vector<std::shared_ptr<SceneInfo>> sceneInfos)
	{
		nlohmann::ordered_json json = nlohmann::ordered_json::object();

		// Create the array of scenes
		for (const auto& sceneInfo : sceneInfos)
			json[sceneInfo->name] = *sceneInfo;

		// Validate the generated json file
		try
		{
			nlohmann::json_schema::json_validator validator;
			validator.set_root_schema(voukoderpro_schema);
			validator.validate(json);
		}
		catch (const std::exception& e)
		{
			BLOG(error) << "Unable to validate scene file against schema - reason: " << e.what();

			return -1;
		}

		// Try to open output file
		std::ofstream out(sceneFile.string());
		if (out.fail())
			return -1;

		// Write contents
		out << json.dump(2);

		return 0;
	}

	int SceneManager::save(const std::shared_ptr<SceneInfo> sceneInfo)
	{
		nlohmann::ordered_json json = nlohmann::ordered_json::object();

		std::ifstream inputStream(sceneFile.string());
		if (inputStream.good())
		{
			// Parse json file
			try
			{
				json = nlohmann::ordered_json::parse(inputStream);
			}
			catch (nlohmann::json::parse_error p)
			{
				BLOG(error) << "Unable to parse JSON file " << sceneFile << " - reason: " << p.what();

				return -1;
			}

			// Validate the loaded json file
			try
			{
				nlohmann::json_schema::json_validator validator;
				validator.set_root_schema(voukoderpro_schema);
				validator.validate(json);
			}
			catch (const std::exception& e)
			{
				BLOG(error) << "Unable to validate scene file against schema - reason: " << e.what();

				return -1;
			}
		}

		// Add or overwrite scene info
		json[sceneInfo->name] = *sceneInfo;

		// Try to open output file
		std::ofstream out(sceneFile.string());
		if (out.fail())
			return -1;

		// Write contents
		out << json.dump(2);

		return 0;
	}

	int SceneManager::importScene(std::shared_ptr<VoukoderPro::SceneInfo> sceneInfo, const std::string filename)
	{
		try
		{
			// Load and parse JSON
			std::ifstream inputStream(filename);
			nlohmann::ordered_json json = nlohmann::ordered_json::parse(inputStream);

			// Validate the loaded json file
			nlohmann::json_schema::json_validator validator;
			validator.set_root_schema(voukoderpro_schema);
			validator.validate(json);

			// Parse JSON to SceneInfos
			*sceneInfo = json;
		}
		catch (boost::system::system_error e)
		{
			BLOG(error) << "Unable to load config file: " << filename;

			return -1;
		}
		catch (nlohmann::json::parse_error p)
		{
			BLOG(error) << "Unable to parse JSON file " << filename << " - reason: " << p.what();

			return -1;
		}
		catch (const std::exception& e)
		{
			BLOG(error) << "Unable to validate file against schema " << filename << " - reason: " << e.what();

			return -1;
		}

		return 0;
	}

	int SceneManager::exportScene(std::shared_ptr<VoukoderPro::SceneInfo> sceneInfo, const std::string filename)
	{
		if (filename.empty())
			return -1;

		// Convert SceneInfo to ojson
		nlohmann::ordered_json json(*sceneInfo);

		// Try to open output file
		std::ofstream out(filename);
		if (out.fail())
			return -1;

		// Write contents
		out << json.dump(2);

		return 0;
	}

	std::shared_ptr<Scene> SceneManager::createScene(std::shared_ptr<SceneInfo> sceneItem)
	{
		std::shared_ptr<Scene> scene = std::make_shared<Scene>();

		const auto flatten = [](std::vector<std::vector<std::string>>& pins)
		{
			std::vector<std::string> list;
			for (auto& pin : pins)
			{
				for (std::string id : pin)
					list.push_back(id);
			}
			return list;
		};

		// Instantiate the node chain of this scene
		for (auto& nodeInfo : sceneItem->nodes)
		{
			// Create each node
			auto node = createNode(nodeInfo);
			if (!node)
				return nullptr;

			// Store it in a id->node lookup map
			scene->nodes.insert(std::make_pair(node->nodeInfo->id, node));

			// Map the link sources first
			const auto outputs = flatten(nodeInfo->outputs);
			for (auto& linkId : outputs)
			{
				if (scene->links.find(linkId) != scene->links.end())
				{
					BLOG(warning) << "One link (Id: " << linkId << ") must have exactly one source and one sink point only!";

					return nullptr;
				}
	
				scene->links.insert(std::make_pair(linkId, std::make_pair(node, nullptr)));
			}
		}

		// Map the link sinks to its sources
		for (auto& nodeInfo : sceneItem->nodes)
		{
			const auto inputs = flatten(nodeInfo->inputs);
			for (auto& linkId : inputs)
			{
				if (scene->links.find(linkId) == scene->links.end() || !scene->links[linkId].first)
				{
					BLOG(warning) << "The link sink (Id: " << linkId << ") does not have a source point!";

					return nullptr;
				}
				else if (scene->links[linkId].second)
				{
					BLOG(warning) << "The link sink (Id: " << linkId << ") has multiple source points! Only one is allowed!";

					return nullptr;
				}

				scene->links[linkId].second = scene->nodes[nodeInfo->id];
			}
		}

		// Connect all nodes of this scene
		for (auto& [id, nodes] : scene->links)
		{
			nodes.first->outputs.push_back(nodes.second);
			nodes.second->inputs.push_back(nodes.first);
		}

		return scene;
	}

	std::shared_ptr<BaseNode> SceneManager::createNode(std::shared_ptr<NodeInfo> nodeInfo)
	{
		std::shared_ptr<BaseNode> node = nullptr;

		switch (nodeInfo->type)
		{
		case NodeInfoType::input:
		{
			node = std::make_shared<InputNode>(nodeInfo); 
			break;
		}
		case NodeInfoType::filter:
		{
			node = std::make_shared<FilterNode>(nodeInfo); 
			break;
		}
		case NodeInfoType::encoder:
		{
			node = std::make_shared<EncoderNode>(nodeInfo);
			break;
		}
		case NodeInfoType::muxer:
		{
			node = std::make_shared<MuxerNode>(nodeInfo);
			break;
		}
		case NodeInfoType::output:
		{
			node = std::make_shared<OutputNode>(nodeInfo);
			break;
		}
		case NodeInfoType::postproc:
		{
			node = std::make_shared<PostProcNode>(nodeInfo);
			break;
		}
		default:
		{
			BLOG(error) << "Unsupported node type";

			return nullptr;
		}
		}

		//// Do we have a nodeId given?
		//if (nodeInfo->data.find("id") != nodeInfo->data.end())
		//{
  //          const std::string id = nodeInfo->data["id"].get<std::string>();

		//	// Create plugin instance
		//	node->plugin = Plugins::instance().createAssetInstance(id, nodeInfo->type);
		//	if (!node->plugin)
		//	{
		//		BLOG(warning) << boost::describe::enum_to_string<NodeInfoType>(nodeInfo->type, 0) << " node (Id: " << nodeInfo->id << ") does not have a plugin id associated which is required for this type!";

		//		return nullptr;
		//	}
		//}

		return node;
	}
}
