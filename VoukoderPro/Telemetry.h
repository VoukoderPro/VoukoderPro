#pragma once

#ifndef VP_TELEMETRY
#define VP_TELEMETRY

#include <string>
#include <map>
#include "utility"

namespace VoukoderPro
{
    class Telemetry
    {
    private:
        Telemetry();

    public:
        Telemetry(const Telemetry&) = delete;
        Telemetry& operator=(const Telemetry&) = delete;
        Telemetry(Telemetry&&) = delete;
        Telemetry& operator=(Telemetry&&) = delete;

        int event(const std::string& name = "", const std::map<std::string, std::string>& params = {});

        static auto& instance()
        {
            static Telemetry instance;
            return instance;
        }

    private:
        std::string getUserIdentifier();
        template <typename T> std::string calculateSHA1(const T& input);

        static std::string getOsPlatform();
        static std::string getOsVersion();

        constexpr static std::string_view ua_format{R"-({{"fullVersionList": [{{"brand": "{0}", "version": "{1}"}}], "mobile": false, "model": "PC", "platform": "{2}", "platformVersion": "{3}"}})-"};
    };
}

#endif