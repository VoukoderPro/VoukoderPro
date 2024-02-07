#pragma once

#include <Windows.h>
#include <sstream>

inline static std::string GetAppVersionFromFile()
{
	std::stringstream version;

	DWORD currentProcessId = GetCurrentProcessId();
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, currentProcessId);
	if (hProcess) {
		TCHAR exePath[MAX_PATH];
		DWORD pathLength = GetModuleFileName(NULL, exePath, MAX_PATH);
		if (pathLength > 0) {
			DWORD versionInfoSize = GetFileVersionInfoSize(exePath, NULL);
			if (versionInfoSize > 0)
			{
				std::unique_ptr<BYTE[]> versionInfoBuffer(new BYTE[versionInfoSize]);
				if (GetFileVersionInfo(exePath, 0, versionInfoSize, versionInfoBuffer.get()))
				{
					VS_FIXEDFILEINFO* fileInfo;
					UINT fileInfoSize;

					if (VerQueryValue(versionInfoBuffer.get(), L"\\", (LPVOID*)&fileInfo, &fileInfoSize))
					{
						WORD major = HIWORD(fileInfo->dwFileVersionMS);
						WORD minor = LOWORD(fileInfo->dwFileVersionMS);
						WORD revision = HIWORD(fileInfo->dwFileVersionLS);
						WORD build = LOWORD(fileInfo->dwFileVersionLS);

						version << major << "." << minor << "." << revision << "." << build;
					}
				}
			}
		}

		CloseHandle(hProcess);
	}

	return version.str();
}