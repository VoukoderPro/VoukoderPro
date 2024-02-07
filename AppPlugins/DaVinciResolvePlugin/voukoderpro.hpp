#pragma once

#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/dll.hpp>
#include <boost/process.hpp>

#include "json.hpp"

namespace bp = boost::process;
namespace fs = boost::filesystem;
using json = nlohmann::json;

#include "../../VoukoderPro/voukoderpro_api.h"

typedef boost::shared_ptr<VoukoderPro::IClient>(pluginapi_create_t)();

class VoukoderBase
{
public:
    VoukoderBase()
    {}

    int voukoderpro_create()
    {
        int ret = 0;

        try
        {
            // Create voukoderpro class factory
            factory = VOUKODERPRO_CREATE_INSTANCE;

            // Create an instance
            vkdrpro = factory();
        }
        catch (boost::system::system_error e)
        {
            return -3;
        }

        return 0;
    }

    static int voukoderpro_config(std::string& config)
    {
        int ret = 0;

        return MessageBoxW(NULL, L"Configure me!", L"Voukoder Plugin", MB_OKCANCEL | MB_ICONEXCLAMATION);

        //// Find out the home directory
        //fs::path voukoderPro;
        //if ((ret = GetVoukoderLocation(voukoderPro)) < 0)
        //    return -1;

        //auto path = voukoderPro.parent_path() / "VoukoderProConfig.exe /Select";

        //bp::ipstream is;
        //ret = bp::system(path.string(), bp::std_out > is);
        //if (ret == 0)
        //{
        //    std::stringstream ss;
        //    
        //    std::string line;
        //    while (std::getline(is, line) && !line.empty())
        //        ss << line;

        //    is.pipe().close();

        //    config = ss.str();

        //    return 0;
        //}

        //return ret;
    }

protected:
    boost::function<pluginapi_create_t> factory;
    boost::shared_ptr<VoukoderPro::IClient> vkdrpro = nullptr;
};