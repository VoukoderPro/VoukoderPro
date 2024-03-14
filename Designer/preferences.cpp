#include "preferences.h"

#include <fstream>
#include <boost/filesystem.hpp>

#include "../VoukoderPro/voukoderpro_api.h"

Preferences::Preferences():
    filename((VOUKODERPRO_DATA / "preferences.json").string())
{
    // Provide a default configuration
    data[nlohmann::ordered_json::json_pointer(VPRO_GENERAL_OPEN_SCENE)] = "simple";
    data[nlohmann::ordered_json::json_pointer(VPRO_DEFAULT_VIDEO_FILTER)] = "zscale";
    data[nlohmann::ordered_json::json_pointer(VPRO_DEFAULT_AUDIO_FILTER)] = "volume";
    data[nlohmann::ordered_json::json_pointer(VPRO_DEFAULT_VIDEO_ENCODER)] = "prores_ks";
    data[nlohmann::ordered_json::json_pointer(VPRO_DEFAULT_AUDIO_ENCODER)] = "alac";
    data[nlohmann::ordered_json::json_pointer(VPRO_DEFAULT_MUXER)] = "mov";
    data[nlohmann::ordered_json::json_pointer(VPRO_DEFAULT_OUTPUT)] = "file";
    data[nlohmann::ordered_json::json_pointer(VPRO_DEFAULT_POSTPROC)] = "exec";
    data[nlohmann::ordered_json::json_pointer(VPRO_PROPERTIES_TECHNAMES)] = false;
    data[nlohmann::ordered_json::json_pointer(VPRO_NEWS_LASTREAD)] = 0;
    data[nlohmann::ordered_json::json_pointer(VPRO_TEST_CONFIG)] = nlohmann::ordered_json::object();
}

int Preferences::load()
{
    try
    {
        // Load the config file.
        // Stick with defaults if it does not exist yet.
        std::ifstream in(filename);
        if (in.fail())
            return 0;

        data = nlohmann::ordered_json::parse(in);
    }
    catch (nlohmann::json::parse_error p)
    {
        return -1;
    }

    return 0;
}

int Preferences::save()
{
    // Try to open output file
    std::ofstream out(filename);
    if (out.fail())
        return -1;

    // Write contents
    out << data.dump(2);

    return 0;
}
