#include "Telemetry.h"

#ifdef __WIN32

#include <Windows.h>
#include <sstream>
#include <sddl.h>
#include <thread>
#include <random>
#include <chrono>
#include <ctime>
#include <boost/url.hpp>
#include <boost/compute/detail/sha1.hpp>

#include "Version.h"

extern "C" {
#include "../3rdparty/curl-8.2.1/include/curl/curl.h"
}

#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Wldap32.lib")
#pragma comment(lib, "Normaliz.lib")
#pragma comment(lib, "../3rdparty/openssl-3.1.2/lib/libcrypto.lib")
#pragma comment(lib, "../3rdparty/openssl-3.1.2/lib/libssl.lib")
#pragma comment(lib, "../3rdparty/curl-8.2.1/lib/libcurl_a.lib")

/* Instructions
* 
* Guide: https://developers.refinitiv.com/en/article-catalog/article/how-to-build-openssl--zlib--and-curl-libraries-on-windows
* 
* 1. $ perl Configure VC-WIN64A no-shared no-module --prefix=C:\Users\daniel\source\repos\VoukoderPro\3rdparty\openssl-3.1.2
* 2. (Change /MT to /MD and -DOPENSSL_USE_APPLINK in "makefile")
* 3. $ nmake
* 4. $ nmake install
* 
* 5. nmake /f Makefile.vc mode=static MACHINE=x64 WITH_SSL=static SSL_PATH=C:\Users\daniel\source\repos\VoukoderPro\3rdparty\openssl-3.1.2
*/
namespace VoukoderPro
{
    Telemetry::Telemetry()
    {}

    int Telemetry::event(const std::string& name, const std::map<std::string, std::string>& params)
    {
        const std::string id = (getUserIdentifier() == "" ? "<error>" : getUserIdentifier());

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
                std::string osVersion;
                NTSTATUS(WINAPI * RtlGetVersion)(LPOSVERSIONINFOEXW);
                OSVERSIONINFOEXW osInfo;

                *(FARPROC*)&RtlGetVersion = GetProcAddress(GetModuleHandleA("ntdll"), "RtlGetVersion");

                if (NULL != RtlGetVersion)
                {
                    osInfo.dwOSVersionInfoSize = sizeof(osInfo);
                    RtlGetVersion(&osInfo);
                    osVersion = std::to_string(osInfo.dwMajorVersion) + "." + std::to_string(osInfo.dwMinorVersion) + "." + std::to_string(osInfo.dwBuildNumber);
                }
                else
                    osVersion = "0.0.0";

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
                //url_builder.params().set("ua", std::string(APP_NAME) + "/" + std::string(APP_VERSION) + " (Windows NT 11.0; Win64; x64)");
                url_builder.params().set("action_name", name);
                url_builder.params().set("send_image", "0");
                url_builder.params().set("res", "1920x1080");
                url_builder.params().set("uadata", "{\"fullVersionList\":[{\"brand\":\"" + std::string(APP_NAME) + "\",\"version\":\"" + std::string(APP_VERSION) + "\"}],\"mobile\":false,\"model\":\"PC\",\"platform\":\"Windows\",\"platformVersion\":\"" + osVersion + "\"}");

                for (const auto& [name, value] : params)
                    url_builder.params().set(name, value);

                url_builder.params().set("dimension3", APP_VERSION);
                url_builder.params().set("dimension4", "Windows " + osVersion);

                const std::string url = url_builder.c_str();

                CURLcode r = CURLE_OK;
                char errbuf[CURL_ERROR_SIZE];
                errbuf[0] = 0;

                CURL* curl = curl_easy_init();
                if (curl)
                {
                    curl_easy_setopt(curl, CURLOPT_POST, 1L);
                    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
                    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, FALSE);
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

    std::string Telemetry::getUserIdentifier()
    {
#ifdef WIN32
        HANDLE hToken;
        if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
            return "";

        DWORD bufferSize = 0;
        GetTokenInformation(hToken, TokenUser, NULL, 0, &bufferSize);
        if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
            CloseHandle(hToken);
            return "";
        }

        TOKEN_USER* pTokenUser = reinterpret_cast<TOKEN_USER*>(new BYTE[bufferSize]);
        if (!pTokenUser) {
            CloseHandle(hToken);
            return "";
        }

        if (!GetTokenInformation(hToken, TokenUser, pTokenUser, bufferSize, &bufferSize)) {
            CloseHandle(hToken);
            delete[] pTokenUser;
            return "";
        }

        CloseHandle(hToken);

        WCHAR* pStringSid;
        if (!ConvertSidToStringSid(pTokenUser->User.Sid, &pStringSid)) {
            delete[] pTokenUser;
            return "";
        }

        std::string sha1 = calculateSHA1(std::wstring(pStringSid));

        LocalFree(pStringSid);
        delete[] pTokenUser;

        return sha1;
#endif
    }

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
}

#else
#endif