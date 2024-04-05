#include "boost/filesystem/fstream.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "format"

namespace VoukoderPro
{
    std::pair<std::string, std::string> linuxTelemetryParseInfo(const boost::filesystem::path &p);

    std::string Telemetry::getUserIdentifier() {
        auto user_id_file = VoukoderProData() / "user_id";
        std::string user_id="0";
        if (exists(user_id_file)) {
            if (is_regular_file(user_id_file)) {
                boost::filesystem::ifstream id_in{user_id_file};
                id_in>>user_id;
            }
        }else{
            user_id=to_string(boost::uuids::random_generator()());
            boost::filesystem::ofstream id_out{user_id_file};
            id_out<<user_id;
        }

        std::string sha1 = calculateSHA1(user_id);
        return sha1;
    }

    std::string Telemetry::getOsPlatform() {
        boost::filesystem::path info_file{"/etc/os-release"};
        if (!is_regular_file(info_file)){
            info_file="/usr/lib/os-release";
            if (!is_regular_file(info_file)){
                return "Unknown Linux";
            }
        }

        return linuxTelemetryParseInfo(info_file).first;
    }

    std::string Telemetry::getOsVersion() {
        boost::filesystem::path info_file{"/etc/os-release"};
        if (!is_regular_file(info_file)){
            info_file="/usr/lib/os-release";
            if (!is_regular_file(info_file)){
                return "Unknown Version";
            }
        }

        return linuxTelemetryParseInfo(info_file).second;
    }

    // Returns: Name, Version
    std::pair<std::string, std::string> linuxTelemetryParseInfo(const boost::filesystem::path &p) {
        // Get distro name and version, based on the spec:
        // https://www.freedesktop.org/software/systemd/man/latest/os-release.html
        boost::filesystem::ifstream in_file{p};
        std::string line{};
        std::map<std::string, std::string> info;

        while (std::getline(in_file, line)) {
            // TODO: According to the spec,
            //  ''Shell special characters ("$", quotes, backslash, backtick) must be escaped with backslashes.''
            //  But leave this for now, it doesn't affect telemetry
            if ((!line.empty() && line[0] == '#') || line.empty())
                continue;
            auto key = line.substr(0, line.find('='));
            auto value = line.substr(line.find('=') + 1);
            if (value.size() >= 2 && (value[0] == '"' && value.back() == '"') ||
                (value[0] == '\'' && value.back() == '\'')) {
                value = value.substr(1, value.size() - 2);
            }
            info[key] = value;
        }

        std::string version = "Unknown Version";
        for (auto version_key: {"VERSION", "VERSION_ID", "VERSION_CODENAME", "BUILD_ID", "IMAGE_ID", "IMAGE_VERSION"}) {
            if (info.contains(version_key)) {
                version = info[version_key];
                break;
            }
        }

        std::string name = "Unknown Linux";
        if (info.contains("PRETTY_NAME")) {
            name = info["PRETTY_NAME"];
        }
        return {name, version};
    }
}