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

protected:
    boost::function<pluginapi_create_t> factory;
    std::shared_ptr<VoukoderPro::IClient> vkdrpro = nullptr;
};