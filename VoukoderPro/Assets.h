#pragma once

#include <string>

#include "voukoderpro_api.h"
#include "../PluginInterface/plugin_api.h"
#include "Logger.h"

#include "boost/filesystem.hpp"
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/function.hpp>
#include <boost/dll.hpp>

namespace VoukoderPro
{
    typedef std::shared_ptr<plugin_api>(voukoderpro_plugin_api_create_t)();

    class Assets
    {
    private:
        Assets();

    public:
        Assets(const Assets&) = delete;
        Assets& operator=(const Assets&) = delete;
        Assets(Assets&&) = delete;
        Assets& operator=(Assets&&) = delete;

        void init();
        void list(std::vector<AssetInfo>& plugins);
        void list(std::vector<AssetInfo>& plugins, NodeInfoType type);
        std::shared_ptr<Asset> createAssetInstance(const std::string id, const NodeInfoType type);

        static auto& instance()
        {
            static Assets instance;
            instance.init();
            return instance;
        }

    private:
        std::vector<boost::function<voukoderpro_plugin_api_create_t>> factories;
        std::vector<std::shared_ptr<plugin_api>> plugins;
        boost::filesystem::path pluginPath;
    };
}