#pragma once

#include <set>

#include "json.hpp"
#include "ffmpeg.h"
#include "InputNode.h"
#include "SceneManager.h"
#include "System.h"

namespace VoukoderPro
{
	class Router
	{
	public:
		Router(std::shared_ptr<Scene> scene, config project);

		int init();
		int open();
		int close();
		int sendFrame(const int nleTrackIndex, std::shared_ptr<AVFrame> frame);
		int getPerformanceLog(nlohmann::ordered_json& log);

	private:
		config project;
		std::shared_ptr<Scene> scene;
		std::set<std::shared_ptr<InputNode>> inputNodes;
		std::map<int, std::set<std::shared_ptr<InputNode>>> nleTrackToInputNodes;

	private:
		int translateTrackIndex(const int search, MediaType mediaType);
		int findInputNodes(std::set<std::shared_ptr<InputNode>>& inputNodes, const int nleTrackIndex, const MediaType mediaType);
		void sysinfo();
	};
}

