#pragma once

#include "json.hpp"

#define VPRO_GENERAL_OPEN_SCENE "/general/openScene"
#define VPRO_DEFAULT_VIDEO_FILTER "/defaults/videoFilter"
#define VPRO_DEFAULT_AUDIO_FILTER "/defaults/audioFilter"
#define VPRO_DEFAULT_VIDEO_ENCODER "/defaults/videoEncoder"
#define VPRO_DEFAULT_AUDIO_ENCODER "/defaults/audioEncoder"
#define VPRO_DEFAULT_MUXER "/defaults/muxer"
#define VPRO_DEFAULT_OUTPUT "/defaults/output"
#define VPRO_DEFAULT_POSTPROC "/defaults/postproc"
#define VPRO_PROPERTIES_TECHNAMES "/properties/techNames"
#define VPRO_NEWS_LASTREAD "/news/lastRead"
#define VPRO_TEST_CONFIG "/test/config/0"

class Preferences
{
private:
    Preferences();

public:
    Preferences(const Preferences&) = delete;
    Preferences& operator=(const Preferences &) = delete;
    Preferences(Preferences &&) = delete;
    Preferences & operator=(Preferences &&) = delete;

    int load();
    int save();

    template <typename T>
    T get(const std::string& keyPath, T def = {})
    {
        nlohmann::ordered_json::json_pointer p(keyPath);
        return data.contains(p) ? data.at(p).get<T>() : def;
    }

    template <typename T>
    void set(const std::string& keyPath, T val)
    {
        nlohmann::ordered_json::json_pointer p(keyPath);
        data[p] = val;
    }

    static auto& instance()
    {
        static Preferences instance;
        return instance;
    }

private:
    nlohmann::ordered_json data;
    std::string filename;
};
