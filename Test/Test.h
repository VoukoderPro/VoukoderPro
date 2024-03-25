#pragma once

#include <boost/function.hpp>
#include <boost/dll.hpp>

#include "../VoukoderPro/voukoderpro_api.h"

typedef boost::shared_ptr<VoukoderPro::IClient>(pluginapi_create_t)();