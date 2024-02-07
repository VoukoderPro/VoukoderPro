#include <Windows.h>
#include "AfterEffectsPlugin.h"
#include "AEGP_SuiteHandler.cpp"
#include "MissingSuiteError.cpp"
#include "Common.h"
#include <tmmintrin.h>
#include <codecvt>

#include <boost/filesystem.hpp>
#include <boost/function.hpp>
#include <boost/dll.hpp>

extern boost::function<pluginapi_create_t> factory = VOUKODERPRO_CREATE_INSTANCE;
static std::shared_ptr<VoukoderPro::IClient> vkdrpro = NULL;

AEGP_PluginID S_mem_id = 0;

static A_Err AEIO_ConstructModuleInfo(AEIO_ModuleInfo *info)
{
	A_Err err = A_Err_NONE;

	if (info)
	{
		info->sig = 'VPRO';
		info->max_width = 8192;
		info->max_height = 4320;
		info->num_filetypes = 1;
		info->num_extensions = 1;
		info->num_clips = 0;
		info->num_aux_extensionsS = 0;

		info->flags = AEIO_MFlag_OUTPUT |
			AEIO_MFlag_FILE |
			AEIO_MFlag_VIDEO |
			AEIO_MFlag_AUDIO |
			AEIO_MFlag_NO_TIME;

		info->create_kind.type = 'VPRO';
		info->create_kind.creator = 'VOUK';

		// Set plugin name
		strcpy_s(info->name, PLUGIN_NAME);

		//
		info->read_kinds[0].mac.type = 'VPRO';
		info->read_kinds[0].mac.creator = AEIO_ANY_CREATOR;

		info->read_kinds[1].ext.pad = '.';

		const char ext[12] = "voukoderpro";
		for (int j = 0; j < strlen(ext); j++)
			info->read_kinds[1].ext.extension[j] = ext[j];
		info->create_ext = info->read_kinds[1].ext;
	}
	else
		err = A_Err_STRUCT;

	return err;
}

static A_Err My_InitOutputSpec(AEIO_BasicData *basic_dataP, AEIO_OutSpecH outH, A_Boolean *user_interacted)
{
	A_Err err = A_Err_NONE;

	AEGP_SuiteHandler suites(basic_dataP->pica_basicP);

	// Make new options handle
	ArbData* newArb = NULL;
	AEGP_MemSize newSize;
	AEIO_Handle newOptionsH = NULL;
	suites.MemorySuite1()->AEGP_NewMemHandle(S_mem_id, "ARB Data", sizeof(ArbData), AEGP_MemFlag_CLEAR, &newOptionsH);
	err = suites.MemorySuite1()->AEGP_GetMemHandleSize(newOptionsH, &newSize);
	err = suites.MemorySuite1()->AEGP_LockMemHandle(newOptionsH, (void**)& newArb);

	// Existing options handle
	AEIO_Handle optionsH = NULL;
	suites.IOOutSuite4()->AEGP_GetOutSpecOptionsHandle(outH, (void**)& optionsH);

	// Do we have saved data?
	if (optionsH)
	{
		AEGP_MemSize size = 0;
		ERR(suites.MemorySuite1()->AEGP_GetMemHandleSize(optionsH, &size));
		
		// Did the data structure increase in size?
		if (newSize > size)
		{
			suites.MemorySuite1()->AEGP_UnlockMemHandle(optionsH);
			suites.MemorySuite1()->AEGP_ResizeMemHandle("Old Handle Resize", size, optionsH);
		}

		// Get existing data
		ArbData* arb = NULL;
		suites.MemorySuite1()->AEGP_LockMemHandle(optionsH, (void**)& arb);

		// Copy existing data to new data
		memcpy((char*)newArb, (char*)arb, newSize);

		suites.MemorySuite1()->AEGP_UnlockMemHandle(optionsH);
	}
	else
	{
		// Set default values as initial values
		memset(newArb, 0, sizeof(ArbData));
		strcpy_s(newArb->selectedScene, "");
	}

	suites.MemorySuite1()->AEGP_UnlockMemHandle(newOptionsH);

	// Set the options handle
	AEIO_Handle oldOptionsH = NULL;
	suites.IOOutSuite4()->AEGP_SetOutSpecOptionsHandle(outH, (void*)newOptionsH, (void**)&oldOptionsH);
	if (oldOptionsH)
		suites.MemorySuite1()->AEGP_FreeMemHandle(oldOptionsH);

	return err;
}

// TODO: Check if it is necessary
static A_Err My_GetFlatOutputOptions(AEIO_BasicData	*basic_dataP, AEIO_OutSpecH outH, AEIO_Handle *new_optionsPH)
{
	A_Err err = A_Err_NONE;

	AEIO_Handle old_optionsH = NULL;
	ArbData	*new_optionsP = NULL,
		*old_optionsP = NULL;
	AEGP_SuiteHandler suites(basic_dataP->pica_basicP);

	ERR(suites.IOOutSuite4()->AEGP_GetOutSpecOptionsHandle(outH, reinterpret_cast<void**>(&old_optionsH)));

	if (!err && old_optionsH) {
		ERR(suites.MemorySuite1()->AEGP_NewMemHandle(S_mem_id,
			"flat optionsH",
			sizeof(ArbData),
			AEGP_MemFlag_CLEAR,
			new_optionsPH));
		if (!err && *new_optionsPH) {
			ERR(suites.MemorySuite1()->AEGP_LockMemHandle(*new_optionsPH, reinterpret_cast<void**>(&new_optionsP)));
			ERR(suites.MemorySuite1()->AEGP_LockMemHandle(old_optionsH, reinterpret_cast<void**>(&old_optionsP)));

			if (!err && new_optionsP && old_optionsP) {
				// Convert the old unflat structure into a separate flat structure for output
				// In this case, we just do a simple copy and set the isFlat flag
				memcpy(new_optionsP, old_optionsP, sizeof(ArbData));
				//new_optionsP->isFlat = TRUE;

				ERR(suites.MemorySuite1()->AEGP_UnlockMemHandle(*new_optionsPH));
				ERR(suites.MemorySuite1()->AEGP_UnlockMemHandle(old_optionsH));
			}
		}
	}
	return err;
}

static A_Err My_DisposeOutputOptions(AEIO_BasicData *basic_dataP, void *optionsPV)
{
	A_Err err = A_Err_NONE;

	AEGP_SuiteHandler suites(basic_dataP->pica_basicP);

	// Dispose options if we have any
	AEIO_Handle optionsH = reinterpret_cast<AEIO_Handle>(optionsPV);
	if (optionsH)
		ERR(suites.MemorySuite1()->AEGP_FreeMemHandle(optionsH));

	return err;
}

static A_Err My_UserOptionsDialog(AEIO_BasicData *basic_dataP, AEIO_OutSpecH outH, const PF_EffectWorld *sample0, A_Boolean *user_interacted0)
{
	A_Err err = A_Err_NONE;

	AEGP_SuiteHandler suites(basic_dataP->pica_basicP);

	// Get ARB data
	AEIO_Handle optionsH = NULL, old_optionsH = 0;
	ArbData *arbData = NULL;
	ERR(suites.IOOutSuite4()->AEGP_GetOutSpecOptionsHandle(outH, reinterpret_cast<void**>(&optionsH)));
	ERR(suites.MemorySuite1()->AEGP_LockMemHandle(optionsH, reinterpret_cast<void**>(&arbData)));
	ERR(suites.MemorySuite1()->AEGP_UnlockMemHandle(optionsH));

	if (arbData)
	{
		// Open voukoder dialog
		std::string sceneName = arbData->selectedScene;
		if (vkdrpro->sceneSelect(sceneName) == 0)
		{
			*user_interacted0 = true;

			strcpy_s(arbData->selectedScene, sceneName.c_str());

			// Store configuration
			ERR(suites.IOOutSuite4()->AEGP_SetOutSpecOptionsHandle(outH, optionsH, reinterpret_cast<void**>(&old_optionsH)));
		}
	}

	return err;
}

static A_Err My_GetOutputInfo(AEIO_BasicData *basic_dataP, AEIO_OutSpecH outH, AEIO_Verbiage *verbiageP)
{
	A_Err err = A_Err_NONE;
	
	AEGP_SuiteHandler suites(basic_dataP->pica_basicP);

	std::wstring name = GetFileNameW(basic_dataP, outH);
	wcstombs(verbiageP->name, name.c_str(), sizeof(verbiageP->name));
	suites.ANSICallbacksSuite1()->strcpy(verbiageP->type, PLUGIN_NAME);

	verbiageP->sub_type[0] = '\0';

	return err;
}

static A_Err My_OutputInfoChanged(AEIO_BasicData *basic_dataP, AEIO_OutSpecH outH)
{
	/*	This function is called when either the user
		or the plug-in has changed the output options info.
		You may want to update your plug-in's internal
		representation of the output at this time.
		We've exercised the likely getters below.
	*/

	A_Err err = A_Err_NONE;

	AEIO_AlphaLabel	alpha;
	AEFX_CLR_STRUCT(alpha);

	FIEL_Label		fields;
	AEFX_CLR_STRUCT(fields);

	A_short			depthS = 0;
	A_Time			durationT = { 0,1 };

	A_Fixed			native_fps = 0;
	A_Ratio			hsf = { 1,1 };
	A_Boolean		is_missingB = TRUE;

	AEGP_SuiteHandler	suites(basic_dataP->pica_basicP);

	ERR(suites.IOOutSuite4()->AEGP_GetOutSpecIsMissing(outH, &is_missingB));

	if (!is_missingB)
	{
		// Find out all the details of the output spec; update
		// your options data as necessary.
		ERR(suites.IOOutSuite4()->AEGP_GetOutSpecAlphaLabel(outH, &alpha));
		ERR(suites.IOOutSuite4()->AEGP_GetOutSpecDepth(outH, &depthS));
		ERR(suites.IOOutSuite4()->AEGP_GetOutSpecInterlaceLabel(outH, &fields));
		ERR(suites.IOOutSuite4()->AEGP_GetOutSpecDuration(outH, &durationT));
		ERR(suites.IOOutSuite4()->AEGP_GetOutSpecFPS(outH, &native_fps));
		ERR(suites.IOOutSuite4()->AEGP_GetOutSpecHSF(outH, &hsf));
	}

	return err;
}

static A_Err My_SetOutputFile(AEIO_BasicData *basic_dataP, AEIO_OutSpecH outH, const A_UTF16Char *file_pathZ)
{
	return AEIO_Err_USE_DFLT_CALLBACK;
}

static std::wstring GetFileNameW(AEIO_BasicData* basic_dataP, AEIO_OutSpecH outH)
{
	AEGP_SuiteHandler suites(basic_dataP->pica_basicP);

	AEGP_MemHandle file_pathH = NULL;
	A_Boolean file_reservedB = FALSE;
	A_UTF16Char* file_pathZ = NULL;
	suites.IOOutSuite4()->AEGP_GetOutSpecFilePath(outH, &file_pathH, &file_reservedB);
	suites.MemorySuite1()->AEGP_LockMemHandle(file_pathH, reinterpret_cast<void**>(&file_pathZ));
	suites.MemorySuite1()->AEGP_UnlockMemHandle(file_pathH);
	suites.MemorySuite1()->AEGP_FreeMemHandle(file_pathH);

	return std::wstring((wchar_t*)file_pathZ);
}

static std::string GetFileNameA(AEIO_BasicData* basic_dataP, AEIO_OutSpecH outH)
{
	std::wstring in = GetFileNameW(basic_dataP, outH);
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.to_bytes(in);
}

static A_Err My_StartAdding(AEIO_BasicData *basic_dataP, AEIO_OutSpecH outH, A_long flags)
{
	A_Err err = A_Err_NONE;
	AEGP_SuiteHandler suites(basic_dataP->pica_basicP);
	
	// Get stored voukoder config
	AEIO_Handle optionsH = NULL;
	ArbData* arbData = NULL;
	ERR(suites.IOOutSuite4()->AEGP_GetOutSpecOptionsHandle(outH, reinterpret_cast<void**>(&optionsH)));
	ERR(suites.MemorySuite1()->AEGP_LockMemHandle(optionsH, reinterpret_cast<void**>(&arbData)));
	ERR(suites.MemorySuite1()->AEGP_UnlockMemHandle(optionsH));

	// Video dimensions
	A_long widthL = 0, heightL = 0;
	suites.IOOutSuite4()->AEGP_GetOutSpecDimensions(outH, &widthL, &heightL);

	// Get audio timebase
	A_FpLong soundRateF = 0.0;
	suites.IOOutSuite4()->AEGP_GetOutSpecSoundRate(outH, &soundRateF);

	// Configure voukoder
	const std::string sceneName = arbData->selectedScene;

	std::vector<std::shared_ptr<VoukoderPro::SceneInfo>> infos;
	vkdrpro->sceneManager()->load(infos);

	if (infos.size() == 0)
		return A_Err_PARAMETER;

	//
	bool found = false;
	for (const auto& info : infos)
	{
		if (info->name == sceneName)
		{
			found = true;
			vkdrpro->setScene(info);
			break;
		}
	}

	// Use first scene if the selected scene was not found
	if (!found)
		vkdrpro->setScene(infos.at(0));

	VoukoderPro::config project;
	project[VoukoderPro::pPropApplication] = "Adobe AfterEffects CC";
	project[VoukoderPro::pPropApplicationVersion] = "0.0.0.0";
	project[VoukoderPro::pPropFilename] = GetFileNameA(basic_dataP, outH);

	if (widthL > 0 && heightL > 0)
	{
		// Video timebase
		A_Fixed fps = 0;
		ERR(suites.IOOutSuite4()->AEGP_GetOutSpecFPS(outH, &fps));

		// Get field order
		FIEL_Label fields = { 0 };
		ERR(suites.IOOutSuite4()->AEGP_GetOutSpecInterlaceLabel(outH, &fields));

		// Get video depth
		A_short depthPS = 0;
		suites.IOOutSuite4()->AEGP_GetOutSpecDepth(outH, &depthPS);

		VoukoderPro::config track;
		track[VoukoderPro::pPropType] = "video";
		track[VoukoderPro::pPropWidth] = static_cast<int>(widthL);
		track[VoukoderPro::pPropHeight] = static_cast<int>(heightL);
		track[VoukoderPro::pPropTimebaseNum] = 65536;
		track[VoukoderPro::pPropTimebaseDen] = static_cast<int>(fps);
		track[VoukoderPro::pPropAspectNum] = 1;
		track[VoukoderPro::pPropAspectDen] = 1;

		if (depthPS == 48 | depthPS == 64)
			track[VoukoderPro::pPropFormat] = "bgra64le";
		else if (depthPS == 24 | depthPS == 32)
			track[VoukoderPro::pPropFormat] = "argb";
		else
		{
			vkdrpro->log("Unsupported color depth: " + std::to_string(depthPS));

			return A_Err_GENERIC;
		}

		// Field order
		if (fields.type == FIEL_Type_FRAME_RENDERED)
			track[VoukoderPro::pPropFieldOrder] = "progressive";
		else
			track[VoukoderPro::pPropFieldOrder] = fields.order == FIEL_Order_LOWER_FIRST ? "bff" : "tff";

		// Set the timecode
		//std::string timecode;
		//if (getSourceTimecode(timecode, ticksPerFrame))
		//	track[VoukoderPro::pPropTimecode] = timecode;

		// Add video track
		project.tracks.push_back(track);
	}

	if (soundRateF > 0)
	{
		// Audio channels
		AEIO_SndChannels channels;
		suites.IOOutSuite4()->AEGP_GetOutSpecSoundChannels(outH, &channels);

		// Get data encoding
		AEIO_SndEncoding encoding;
		suites.IOOutSuite4()->AEGP_GetOutSpecSoundEncoding(outH, &encoding);

		// Get sample size
		AEIO_SndSampleSize sampleSize = 0;
		suites.IOOutSuite4()->AEGP_GetOutSpecSoundSampleSize(outH, &sampleSize);

		// Set basic values
		VoukoderPro::config track;
		track[VoukoderPro::pPropType] = "audio";
		track[VoukoderPro::pPropSamplingRate] = static_cast<int>(soundRateF);
		track[VoukoderPro::pPropFormat] = "fltp";

		switch (channels)
		{
		case AEIO_SndChannels_MONO:
			track[VoukoderPro::pPropChannelLayout] = "mono";
			track[VoukoderPro::pPropChannelCount] = 1;
			break;
		case AEIO_SndChannels_STEREO:
		case 0:
			track[VoukoderPro::pPropChannelLayout] = "stereo";
			track[VoukoderPro::pPropChannelCount] = 2;
			break;
		//default:
			//vkdr->Log(CC("Channel layout with " << (int)channels << " channels is not supported. Audio track disabled."));
			//info.audio.enabled = false;
		}

		// Map sample format
		if (encoding == AEIO_E_UNSIGNED_PCM)
		{
			if (sampleSize == AEIO_SS_1)
				track[VoukoderPro::pPropFormat] = "u8";
			else
				return A_Err_PARAMETER; // Sample size not supported
		}
		else if (encoding == AEIO_E_SIGNED_PCM)
		{
			if (sampleSize == AEIO_SS_1)
				return A_Err_PARAMETER; // Sample size not supported
			else if (sampleSize == AEIO_SS_2)
				track[VoukoderPro::pPropFormat] = "s16";
			else if (sampleSize == AEIO_SS_4)
				track[VoukoderPro::pPropFormat] = "s32";
		}
		else if (encoding == AEIO_E_SIGNED_FLOAT)
		{
			if (sampleSize == AEIO_SS_4)
				track[VoukoderPro::pPropFormat] = "flt";
			else
				return A_Err_PARAMETER; // Sample size not supported
		}

		// Add audio track
		project.tracks.push_back(track);
	}
	
	// Open the encopder
	if (vkdrpro->open(project) < 0)
	{
		vkdrpro->log("Unable to open the VoukoderPro scene.");
		
		return A_Err_GENERIC;
	}

	vkdrpro->event("export", {
		{ "dimension1", "Adobe After Effects" },
		{ "dimension2", "Adobe After Effects " + GetAppVersionFromFile() }
	});

	return A_Err_NONE;
}

static A_Err My_AddFrame(AEIO_BasicData *basic_dataP, AEIO_OutSpecH outH, A_long frame_index, A_long frames, const PF_EffectWorld *wP, const A_LPoint *origin0, A_Boolean was_compressedB, AEIO_InterruptFuncs *inter0)
{
	AEGP_SuiteHandler suites(basic_dataP->pica_basicP);

	// Get video depth
	A_short depthPS = 0;
	suites.IOOutSuite4()->AEGP_GetOutSpecDepth(outH, &depthPS);

	// Fill in 8 or 16 bpc data
	if (PF_WORLD_IS_DEEP(wP) && (depthPS == 48 || depthPS == 64))
	{

		// Video dimensions
		A_long widthL = 0, heightL = 0;
		suites.IOOutSuite4()->AEGP_GetOutSpecDimensions(outH, &widthL, &heightL);

		const auto mask = _mm_set_epi8(9, 8, 11, 10, 13, 12, 15, 14, 1, 0, 3, 2, 5, 4, 7, 6);
		__m128i* buffer = (__m128i*)(wP->data);
		const size_t bufSize = (widthL * heightL) / 2;
		for (int i = 0; i < bufSize; i++)
		{
			auto bgra64_15bit = _mm_shuffle_epi8(_mm_loadu_si128(buffer), mask);
			auto bgra64 = _mm_adds_epu16(bgra64_15bit, bgra64_15bit);
			memcpy(buffer, bgra64.m128i_u8, 16);

			buffer++;
		}
	}

	// Prepare buffer
	uint8_t** buffer = new uint8_t * [1];
	buffer[0] = reinterpret_cast<uint8_t*>(wP->data);

	// How many bytes per row do we have?
	int* rowbytes = new int[1];
	rowbytes[0] = wP->rowbytes;

	// Send to voukoder
	if (vkdrpro->writeVideoFrame(0, frame_index, buffer, rowbytes) < 0)
	{
		vkdrpro->log("Unable to write video frame #" + std::to_string(frame_index) + ".");

		return A_Err_GENERIC;
	}

	return A_Err_NONE;
}

static A_Err My_EndAdding(AEIO_BasicData *basic_dataP, AEIO_OutSpecH outH, A_long flags)
{
	vkdrpro->close();

	return A_Err_NONE;
}

static A_Err My_WriteLabels(AEIO_BasicData *basic_dataP, AEIO_OutSpecH outH, AEIO_LabelFlags *written)
{
	return AEIO_Err_USE_DFLT_CALLBACK;
}

static A_Err My_GetSizes(AEIO_BasicData *basic_dataP, AEIO_OutSpecH outH, A_u_longlong *free_space, A_u_longlong *file_size)
{
	return AEIO_Err_USE_DFLT_CALLBACK;
}

static A_Err My_Flush(AEIO_BasicData *basic_dataP, AEIO_OutSpecH outH)
{
	return A_Err_NONE;
}

static A_Err My_AddSoundChunk(AEIO_BasicData *basic_dataP, AEIO_OutSpecH outH, const A_Time *start, A_u_long num_samplesLu, const void *dataPV)
{
	AEGP_SuiteHandler suites(basic_dataP->pica_basicP);

	// Get sample size
	AEIO_SndSampleSize sampleSize = 0;
	suites.IOOutSuite4()->AEGP_GetOutSpecSoundSampleSize(outH, &sampleSize);

	// Get channels
	AEIO_SndChannels channels;
	suites.IOOutSuite4()->AEGP_GetOutSpecSoundChannels(outH, &channels);
	
	// Package the buffer
	uint8_t** buffer = new uint8_t*[1];
	buffer[0] = (uint8_t*)dataPV;

	// How many bytes are we sending?
	const int bufferSize = num_samplesLu * sampleSize * channels;

	// Send to voukoder
	if (vkdrpro->writeAudioSamples(1, buffer, bufferSize) < 0)
	{
		vkdrpro->log("Unable to write audio buffer.");

		return A_Err_GENERIC;
	}

	return A_Err_NONE;
};

static A_Err My_GetDepths(AEIO_BasicData *basic_dataP, AEIO_OutSpecH outH, AEIO_SupportedDepthFlags *which)
{
	*which = AEIO_SupportedDepthFlags_DEPTH_24 | AEIO_SupportedDepthFlags_DEPTH_32;// | AEIO_SupportedDepthFlags_DEPTH_48 | AEIO_SupportedDepthFlags_DEPTH_64;

	return A_Err_NONE;
}

static A_Err My_GetOutputSuffix(AEIO_BasicData *basic_dataP, AEIO_OutSpecH outH, A_char *suffix)
{
	A_Err err = A_Err_NONE;

	AEGP_SuiteHandler suites(basic_dataP->pica_basicP);

	suites.ANSICallbacksSuite1()->strcpy(suffix, ".voukoderpro");

	return A_Err_NONE;
}

static A_Err My_CloseSourceFiles(AEIO_BasicData	*basic_dataP, AEIO_InSpecH seqH)
{
	return A_Err_NONE;
}

static A_Err My_SetUserData(AEIO_BasicData *basic_dataP, AEIO_OutSpecH outH, A_u_long typeLu, A_u_long indexLu, const AEIO_Handle dataH)
{
	return A_Err_NONE;
}

static A_Err AEIO_ConstructFunctionBlock(AEIO_FunctionBlock4 *funcs)
{
	if (funcs)
	{
		funcs->AEIO_AddFrame = My_AddFrame;
		funcs->AEIO_AddSoundChunk = My_AddSoundChunk;
		funcs->AEIO_DisposeOutputOptions = My_DisposeOutputOptions;
		funcs->AEIO_EndAdding = My_EndAdding;
		funcs->AEIO_Flush = My_Flush;
		funcs->AEIO_GetDepths = My_GetDepths;
		funcs->AEIO_GetOutputInfo = My_GetOutputInfo;
		funcs->AEIO_GetOutputSuffix = My_GetOutputSuffix;
		funcs->AEIO_GetSizes = My_GetSizes;
		funcs->AEIO_SetOutputFile = My_SetOutputFile;
		funcs->AEIO_SetUserData = My_SetUserData;
		funcs->AEIO_StartAdding = My_StartAdding;
		funcs->AEIO_UserOptionsDialog = My_UserOptionsDialog;
		funcs->AEIO_WriteLabels = My_WriteLabels;
		funcs->AEIO_InitOutputSpec = My_InitOutputSpec;
		funcs->AEIO_GetFlatOutputOptions = My_GetFlatOutputOptions;
		funcs->AEIO_OutputInfoChanged = My_OutputInfoChanged;

		return A_Err_NONE;
	}
	else
		return A_Err_STRUCT;
}

A_Err GPMain_IO(struct SPBasicSuite *pica_basicP, A_long major_versionL, A_long minor_versionL,	AEGP_PluginID aegp_plugin_id, void *global_refconPV)
{
	A_Err err = A_Err_NONE;
	AEIO_ModuleInfo info = { 0 };
	AEIO_FunctionBlock4	funcs = { 0 };
	AEGP_SuiteHandler suites(pica_basicP);

	ERR(AEIO_ConstructModuleInfo(&info));
	ERR(AEIO_ConstructFunctionBlock(&funcs));
	ERR(suites.RegisterSuite5()->AEGP_RegisterIO(aegp_plugin_id, 0, &info, &funcs));
	ERR(suites.UtilitySuite3()->AEGP_RegisterWithAEGP(NULL, "VOUKODERPRO", &S_mem_id));

	vkdrpro = factory();

	// Create the voukoder instance
	if (!vkdrpro)
	{
		MessageBox(GetActiveWindow(), L"Unable to create VoukoderPro instance.", L"VoukoderPro", MB_OK);
		err = A_Err_GENERIC;
	}

	// Initialize
	if (vkdrpro->init() < 0)
	{
		MessageBox(GetActiveWindow(), L"VoukoderPro Plugin :: Initialization failed!", L"VoukoderPro", MB_OK);
		err = A_Err_GENERIC;
	}

	vkdrpro->event("startup", {
		{ "dimension1", "Adobe After Effects" },
		{ "dimension2", "Adobe After Effects " + GetAppVersionFromFile() }
	});

	return err;
}