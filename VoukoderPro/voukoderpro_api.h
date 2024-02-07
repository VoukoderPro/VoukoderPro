#pragma once

#include "../VoukoderPro/types.h"
#include "../PluginInterface/properties.h"

#include <boost/date_time/gregorian/gregorian.hpp>

#ifdef _DEBUG
#define VOUKODERPRO_HOME \
    []()->std::string { const char* c = std::getenv("VOUKODERPRO_DEBUG_HOME"); return c ? std::string(c) : ""; }()
#else
#define VOUKODERPRO_HOME \
    []()->std::string { const char* c = std::getenv("VOUKODERPRO_HOME"); return c ? std::string(c) : ""; }()
#endif

#define VOUKODERPRO_DATA \
    boost::filesystem::path(std::getenv("LOCALAPPDATA")) / "VoukoderPro"

#define VOUKODERPRO_CREATE_INSTANCE \
	boost::dll::import_alias<pluginapi_create_t>( \
    []() { std::string c = VOUKODERPRO_HOME; return !c.empty() ? boost::filesystem::path(c) / "voukoderpro" : ""; }(), \
	"createInstance", boost::dll::load_mode::append_decorations)

namespace VoukoderPro
{
    struct config : public configType
    {
        std::vector<configType> tracks;
    };

    class BOOST_SYMBOL_VISIBLE ISceneManager
    {
    public:
        virtual ~ISceneManager() {}
        virtual int load(std::vector<std::shared_ptr<SceneInfo>>&) = 0;
        virtual int save(const std::vector<std::shared_ptr<SceneInfo>>) = 0;
        virtual int save(const std::shared_ptr<SceneInfo>) = 0;
        virtual int importScene(std::shared_ptr<VoukoderPro::SceneInfo>, const std::string) = 0;
        virtual int exportScene(std::shared_ptr<VoukoderPro::SceneInfo>, const std::string) = 0;
    };

    class BOOST_SYMBOL_VISIBLE IPerformanceManager
    {
    public:
        ~IPerformanceManager() {};
        virtual int create(const std::string key, const std::string document) = 0;
        virtual int list(std::vector<std::string>&) = 0;
    };

    class BOOST_SYMBOL_VISIBLE IClient
    {
    public:
        virtual ~IClient() {}
        virtual int init(std::function<void(std::string)> callback = nullptr) = 0;
        virtual void setScene(std::shared_ptr<SceneInfo>) = 0;
        virtual std::string extension() = 0;
        virtual int open(config properties) = 0;
        virtual int close(const bool savePerformance = true) = 0;
        virtual int configure(const std::string id = "") = 0;
        virtual int sceneSelect(std::string& name) = 0;
        virtual int writeAudioSamples(const int track, uint8_t** buffer, const int linesize) = 0;
        virtual int writeVideoFrame(const int track, const int64_t frame, uint8_t** buffer, const int* linesize) = 0;
        virtual int writeCompressedVideoFrame(const int track, int64_t frame, uint8_t* compressedBuffer, const int linesize) = 0;
        virtual void log(const std::string msg) = 0;
        virtual std::vector<AssetInfo> plugins() = 0;
        virtual int event(const std::string& name = "", const std::map<std::string, std::string>& params = {}) = 0;

        // Managers
        virtual std::shared_ptr<ISceneManager> sceneManager() = 0;
        //virtual std::shared_ptr<IPerformanceManager> performanceManager() = 0;
    };
}
