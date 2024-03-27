#pragma once

#include <chrono>
#include <string>
#include "AEConfig.h"
#include "entry.h"
#include <windows.h>
#include "AE_IO.h"
#include "AE_Macros.h"
#include "AEGP_SuiteHandler.h"
#include "../../VoukoderPro/voukoderpro_api.h"

#define ARB_VERSION 1

#ifdef __cplusplus
extern "C" {
#endif
	typedef struct 
	{
		char selectedScene[256];
	} ArbData;

	DllExport A_Err GPMain_IO( struct SPBasicSuite *pica_basicP, A_long major_versionL, A_long minor_versionL, AEGP_PluginID aegp_plugin_id, void *global_refconPV);
#ifdef __cplusplus
}
#endif

static std::wstring GetFileNameW(AEIO_BasicData* basic_dataP, AEIO_OutSpecH outH);
static std::string GetFileNameA(AEIO_BasicData* basic_dataP, AEIO_OutSpecH outH);
