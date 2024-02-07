#pragma once

#include "voukoderpro_api.h"
#include "BaseNode.h"
#include <boost/describe/enum.hpp>
#include <boost/describe/enumerators.hpp>
#include <boost/describe/enum_to_string.hpp>
#include <boost/describe/enum_from_string.hpp>
#include "json.hpp"

namespace fs = boost::filesystem;

namespace VoukoderPro
{
	typedef std::vector<std::vector<std::string>> pinId;

	struct Scene
	{
		SceneInfo info;
		std::map<std::string, std::shared_ptr<BaseNode>> nodes;
		std::map<std::string, std::pair<std::shared_ptr<BaseNode>, std::shared_ptr<BaseNode>>> links;
	};

	class SceneManager : public ISceneManager
	{
	public:
		SceneManager();
		std::shared_ptr<Scene> createScene(std::shared_ptr<SceneInfo>);

		// ISceneManager
		int load(std::vector<std::shared_ptr<SceneInfo>>&);
		int save(const std::vector<std::shared_ptr<SceneInfo>>);
		int save(const std::shared_ptr<SceneInfo>);
		int importScene(std::shared_ptr<VoukoderPro::SceneInfo>, const std::string);
		int exportScene(std::shared_ptr<VoukoderPro::SceneInfo>, const std::string);

	private:
		std::shared_ptr<BaseNode> createNode(std::shared_ptr<NodeInfo>);

	private:
		fs::path sceneFile;
		SceneInfo root;
	};
}
