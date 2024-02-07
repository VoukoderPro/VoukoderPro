#include "plugin.h"

#include <boost/process.hpp>

namespace VoukoderPro
{
    ExecutePostProcPlugin::ExecutePostProcPlugin()
    {
        AssetInfo info;
        info.id = "exec";
        info.name = "Execute";
        info.category = std::make_pair("voukoderpro", "VoukoderPro");
        info.description = "Executes a command line";
        info.type = NodeInfoType::postproc;
        info.mediaType = MediaType::out;
        info.helpUrl = "";

        auto& basic = info.group("basic", "Basic", ItemParamGroupType::Forced);
        basic.param<std::string>("exec", "Command line")
            .description("Command line to execute.\n\nSupported variables: $(" + pVarFilePath + "), $(" + pVarFileName + "), $(" + pVarFileExtension + "), $(" + pVarFileFullname + ")");

        basic.param<bool>("wait", "Wait for termination")
            .description("Wait until the command line finishes.")
            .defaultValue(true);

        registerAsset(info);
    }

    std::shared_ptr<Asset> ExecutePostProcPlugin::createAsset(const AssetInfo& info)
    {
        return std::make_shared<ExecutePostProcAsset>(info);
    }

    // ----

    ExecutePostProcAsset::ExecutePostProcAsset(const AssetInfo& info):
        Asset(info)
    {}

    int ExecutePostProcAsset::open(nlohmann::ordered_json& params)
    {
        this->params = params;

        return 0;
    }

    int ExecutePostProcAsset::close()
    {
        int ret = ERR_OK;

        if (params.contains("exec"))
        {
            const std::string exec = params["exec"].get<std::string>();

            // Should we wait for program termination
            bool wait = true;
            if (params.contains("wait"))
                wait = params["wait"].get<bool>();

            // Execute command line
            if (wait)
                ret = boost::process::system(exec);
            else
                boost::process::spawn(exec);
        }

        return ret;
    }
}
