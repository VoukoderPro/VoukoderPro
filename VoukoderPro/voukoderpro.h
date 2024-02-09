#pragma once

#include <functional>
#include <boost/shared_ptr.hpp>
#include <boost/dll/alias.hpp>
#include <boost/filesystem.hpp>
#include "voukoderpro_api.h"
#include "json.hpp"
#include "Router.h"
#include "Logger.h"
#include "Assets.h"
#include "SceneManager.h"
#include "PerformanceManager.h"

#include <boost/log/sinks/basic_sink_backend.hpp>
#include <boost/log/core/record_view.hpp>

namespace fs = boost::filesystem;
namespace logging = boost::log;

namespace VoukoderPro
{
	template<typename CharT>
	class callback_sink : public logging::sinks::basic_formatted_sink_backend<CharT, logging::sinks::synchronized_feeding>
	{
		typedef logging::sinks::basic_formatted_sink_backend<CharT, logging::sinks::synchronized_feeding> base_type;

	public:
		typedef typename base_type::string_type string_type;

		explicit callback_sink(std::function<void(std::string)> logCallback):
			logCallback(logCallback)
		{
		}

		void consume(logging::record_view const& rec, string_type const& formatted_message)
		{
			logCallback(formatted_message);
		}

	private:
		std::function<void(std::string)> logCallback;
	};

	struct Track
	{
		VoukoderPro::configType properties;
		std::shared_ptr<AVFrame> frame;
		std::shared_ptr<AVPacket> compressedFrame;
		std::shared_ptr<AVCodecContext> decoderCtx;
		int64_t nextPts = 0;
	};

	class Client : public IClient
	{
		typedef logging::sinks::synchronous_sink<callback_sink<char>> callback_sink_t;

		Client();

	public:
		~Client();
		void setScene(std::shared_ptr<SceneInfo>);
		int init(std::function<void(std::string)> logCallback);
		std::string extension();
		int open(config project);
		int close(const bool savePerformance = true);
		int configure(const std::string id = "");
		int sceneSelect(std::string& name);
		int writeAudioSamples(const int trackIdx, uint8_t** buffer, const int linesize);
		int writeVideoFrame(const int trackIdx, const int64_t frameIdx, uint8_t** buffer, const int* linesize);
		int writeCompressedVideoFrame(const int track, const int64_t frame, uint8_t* compressedBuffer, const int linesize);
		void log(const std::string msg);
		std::vector<AssetInfo> plugins();
		int event(const std::string& name = "", const std::map<std::string, std::string>& params = {});
		
		// Managers
		std::shared_ptr<ISceneManager> sceneManager() { return sceneMgr; }
		
		static std::shared_ptr<Client> createInstance()
		{
			return std::shared_ptr<Client>(new Client);
		}

	private:
		std::shared_ptr<SceneInfo> sceneInfo;
		nlohmann::json m_scenes;
		config project;
		std::unique_ptr<Router> router;
		std::stringstream logstream;
		std::vector<std::shared_ptr<Track>> tracks;

	private:
		fs::path homeDir;
		fs::path dataDir;
		std::vector<AssetInfo> assetList;
		boost::shared_ptr<callback_sink_t> callbackSink = nullptr;
		std::shared_ptr<SceneManager> sceneMgr = std::make_shared<SceneManager>();

	private:
		int savePerformanceLog();
	};

	BOOST_DLL_ALIAS(VoukoderPro::Client::createInstance, createInstance)
}