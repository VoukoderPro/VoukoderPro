#pragma once

#include <string>
#include <map>

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
    };
}