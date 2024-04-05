#include <Windows.h>
#include <sddl.h>

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
    std::string Telemetry::getUserIdentifier()
    {
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
    }

    std::string Telemetry::getOsPlatform() {
        return "Windows";
    }

    std::string Telemetry::getOsVersion() {
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

        return osVersion;
    }
}