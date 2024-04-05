#include "Telemetry.h"
#include "voukoderpro_api.h"
#include "Version.h"

#include <sstream>
#include <thread>
#include <random>
#include <chrono>
#include <ctime>
#include <boost/url.hpp>
#include <boost/compute/detail/sha1.hpp>

#ifdef _WIN32
extern "C" {
#include "../3rdparty/curl-8.2.1/include/curl/curl.h"
}
#else
extern "C" {
#include "curl/curl.h"
}
#endif


namespace VoukoderPro
{
    Telemetry::Telemetry()
    {}

    template <typename T>
    std::string Telemetry::calculateSHA1(const T& input)
    {
        boost::uuids::detail::sha1 sha1;
        sha1.process_bytes(input.data(), input.size());

        unsigned int hash[5];
        sha1.get_digest(hash);

        std::ostringstream oss;
        for (int i = 0; i < 5; ++i)
            oss << std::hex << std::setw(8) << std::setfill('0') << hash[i];

        return oss.str();
    }


    int Telemetry::event(const std::string& name, const std::map<std::string, std::string>& params)
    {
        const std::string id = (getUserIdentifier().empty() ? "<error>" : getUserIdentifier());

        std::thread worker([id,name,params]()
                           {
                               // Seed the random number generator
                               std::random_device rd;  // Used to obtain a seed for the random number engine
                               std::mt19937 gen(rd()); // Mersenne Twister engine with a random seed
                               std::uniform_int_distribution<int> distribution(0, INT_MAX);

                               // Get local time
                               std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
                               std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
                               std::tm localTime = *std::localtime(&currentTime);

                               //
                               std::string osVersion=getOsVersion();
                               std::string osPlatform=getOsPlatform();

                               // Build Matomo url
                               boost::urls::url url_builder("https://matomo.stankewitz.eu/matomo.php");
                               url_builder.params().set("idsite", "1");
                               url_builder.params().set("rec", "1");
                               url_builder.params().set("apiv", "1");
                               url_builder.params().set("rand", std::to_string(distribution(gen)));
                               url_builder.params().set("h", std::to_string(localTime.tm_hour));
                               url_builder.params().set("m", std::to_string(localTime.tm_min));
                               url_builder.params().set("s", std::to_string(localTime.tm_sec));
                               url_builder.params().set("uid", id);
                               url_builder.params().set("url", "https://www.voukoderpro.com/plugin?action=" + name);
                               url_builder.params().set("lang", "en-us");
                               url_builder.params().set("new_visit ", "1");
                               url_builder.params().set("action_name", name);
                               url_builder.params().set("send_image", "0");
                               url_builder.params().set("res", "1920x1080");
                               auto uadata = std::format(ua_format, APP_NAME, APP_VERSION, osPlatform, osVersion);
                               url_builder.params().set("uadata", uadata);

                               for (const auto& [pname, pvalue] : params)
                                   url_builder.params().set(pname, pvalue);

                               url_builder.params().set("dimension3", APP_VERSION);
                               url_builder.params().set("dimension4", osPlatform + " " + osVersion);

                               const std::string url = url_builder.c_str();

                               CURLcode r;
                               char errbuf[CURL_ERROR_SIZE];
                               errbuf[0] = 0;

                               CURL* curl = curl_easy_init();
                               if (curl)
                               {
                                   curl_easy_setopt(curl, CURLOPT_POST, 1L);
                                   curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
                                   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
                                   curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);

                                   r = curl_easy_perform(curl);
                                   if (r != CURLE_OK)
                                       return -r;

                                   curl_easy_cleanup(curl);
                               }

                               curl_global_cleanup();
                           });
        worker.detach();
        return 0;
    }
}

#ifdef _WIN32
#include "TelemetryWindows.cpp"
#else
#include "TelemetryLinux.cpp"
#endif