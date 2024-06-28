#include "plugin.h"

#include <codecvt>
#include <sstream>
#include <iomanip>
#include <immintrin.h>

#include <boost/filesystem.hpp>
#include <boost/dll.hpp>

boost::function<pluginapi_create_t> factory = VoukoderProCreateInstance();

static inline int hash(const char* str)
{
	int h = 0;
	while (*str)
		h = h << 1 ^ *str++;
	return h;
}

template <typename T>
static inline T GCD(T a, T b)
{
	if (a == 0)
		return b;
	return GCD(b % a, a);
}

static inline __m256i load_and_scale256(const float* src, __m256 mul, __m256 add)
{
	__m256 vuya = _mm256_loadu_ps(src);
	__m256 ayuv = _mm256_permute_ps(vuya, 0b00011011);
	return _mm256_cvttps_epi32(_mm256_fmadd_ps(ayuv, mul, add));
}

DllExport PREMPLUGENTRY xSDKExport(csSDK_int32 selector, exportStdParms* stdParmsP, void* param1, void* param2)
{
	switch (selector)
	{
	case exSelStartup:
	{
		// We're not supporting older plugins
		if (stdParmsP->interfaceVer < prExportVersion500)
			return exportReturn_Unsupported;

		// Register the plugin
		exExporterInfoRec* infoRecP = reinterpret_cast<exExporterInfoRec*>(param1);
		infoRecP->fileType = 'VPRO';
		copyWideStringIntoUTF16(L"VoukoderPro", infoRecP->fileTypeName);
		copyWideStringIntoUTF16(L"voukoderpro", infoRecP->fileTypeDefaultExtension);
		infoRecP->classID = 'VPRO';
		infoRecP->wantsNoProgressBar = kPrFalse;
		infoRecP->hideInUI = kPrFalse;
		infoRecP->doesNotSupportAudioOnly = kPrFalse;
		infoRecP->interfaceVersion = EXPORTMOD_VERSION;
		infoRecP->isCacheable = kPrFalse;
		infoRecP->canExportVideo = kPrTrue;
		infoRecP->canExportAudio = kPrTrue;
		infoRecP->singleFrameOnly = kPrFalse;
		infoRecP->canConformToMatchParams = kPrTrue;
		infoRecP->canEmbedColorProfile = kPrTrue;
		infoRecP->canEmbedCaptions = kPrFalse;
		infoRecP->supportsColorManagement = kPrTrue;
		infoRecP->flags = kExInfoRecFlag_PostEncodePublishNotSupported;

		return exportReturn_ErrNone;
	}

	case exSelBeginInstance:
	{
		exExporterInstanceRec* instanceRecP = reinterpret_cast<exExporterInstanceRec*>(param1);

        // Create voukoderpro class factory
		std::shared_ptr<VoukoderPro::IClient> instance = nullptr;

		try
		{
			instance = factory();
			if (!instance)
				return exportReturn_ErrOther;
		}
		catch (boost::system::system_error e)
		{
			//TODO: Copy FFMPEG to System32 or anything!!!
			return exportReturn_ErrOther;
		}

		if (instance->init() < 0)
			return exportReturn_ErrOther;

		CPremierePluginApp* connector = new CPremierePluginApp(instanceRecP->exporterPluginID, instance);
		prMALError error = connector->beginInstance(stdParmsP->getSPBasicSuite(), instanceRecP);
		if (error == malNoError)
			instanceRecP->privateData = reinterpret_cast<void*>(connector);

		return error;
	}

	case exSelEndInstance:
	{
		const exExporterInstanceRec* instanceRecP = reinterpret_cast<exExporterInstanceRec*>(param1);

		prMALError error = malNoError;

		CPremierePluginApp* connector = reinterpret_cast<CPremierePluginApp*>(instanceRecP->privateData);
		if (connector)
		{
			error = connector->endInstance();
			if (error == malNoError)
				delete(connector);
		}

		return error;
	}

	case exSelGenerateDefaultParams:
	{
		exGenerateDefaultParamRec* instanceRecP = reinterpret_cast<exGenerateDefaultParamRec*>(param1);
		return (reinterpret_cast<CPremierePluginApp*>(instanceRecP->privateData))->generateDefaultParams(instanceRecP);
	}
	case exSelPostProcessParams:
	{
		exPostProcessParamsRec* instanceRecP = reinterpret_cast<exPostProcessParamsRec*>(param1);
		return (reinterpret_cast<CPremierePluginApp*>(instanceRecP->privateData))->postProcessParams(instanceRecP);
	}
	case exSelValidateParamChanged:
	{
		exParamChangedRec* instanceRecP = reinterpret_cast<exParamChangedRec*>(param1);
		return (reinterpret_cast<CPremierePluginApp*>(instanceRecP->privateData))->validateParamChanged(instanceRecP);
	}
	case exSelGetParamSummary:
	{
		exParamSummaryRec* instanceRecP = reinterpret_cast<exParamSummaryRec*>(param1);
		return (reinterpret_cast<CPremierePluginApp*>(instanceRecP->privateData))->getParamSummary(instanceRecP);
	}
	case exSelExport:
	{
		exDoExportRec* instanceRecP = reinterpret_cast<exDoExportRec*>(param1);
		return (reinterpret_cast<CPremierePluginApp*>(instanceRecP->privateData))->StartExport(instanceRecP);
	}
	case exSelQueryExportFileExtension:
	{
		exQueryExportFileExtensionRec* instanceRecP = reinterpret_cast<exQueryExportFileExtensionRec*>(param1);
		return (reinterpret_cast<CPremierePluginApp*>(instanceRecP->privateData))->queryExportFileExtension(instanceRecP);
	}
	case exSelQueryOutputSettings:
	{
		exQueryOutputSettingsRec* instanceRecP = reinterpret_cast<exQueryOutputSettingsRec*>(param1);
		return (reinterpret_cast<CPremierePluginApp*>(instanceRecP->privateData))->queryOutputSettings(instanceRecP);
	}
	case exSelParamButton:
	{
		exParamButtonRec* instanceRecP = reinterpret_cast<exParamButtonRec*>(param1);
		return (reinterpret_cast<CPremierePluginApp*>(instanceRecP->privateData))->buttonAction(instanceRecP);
	}
	}

	return exportReturn_Unsupported;
}

CPremierePluginApp::CPremierePluginApp(csSDK_uint32 pluginId, std::shared_ptr<VoukoderPro::IClient> vkdrpro):
	pluginId(pluginId), vkdrpro(vkdrpro), appVersion({}), suites({})
{}

prMALError CPremierePluginApp::beginInstance(SPBasicSuite* spBasicSuite, exExporterInstanceRec* instanceRecP)
{
	suites.basicSuite = spBasicSuite;

	prMALError ret;

	ret = spBasicSuite->AcquireSuite(kPrSDKMemoryManagerSuite, kPrSDKMemoryManagerSuiteVersion, const_cast<const void**>(reinterpret_cast<void**>(&suites.memorySuite)));
	ret = spBasicSuite->AcquireSuite(kPrSDKTimeSuite, kPrSDKTimeSuiteVersion, const_cast<const void**>(reinterpret_cast<void**>(&suites.timeSuite)));
	ret = spBasicSuite->AcquireSuite(kPrSDKExportParamSuite, kPrSDKExportParamSuiteVersion, const_cast<const void**>(reinterpret_cast<void**>(&suites.exportParamSuite)));
	ret = spBasicSuite->AcquireSuite(kPrSDKExportInfoSuite, kPrSDKExportInfoSuiteVersion, const_cast<const void**>(reinterpret_cast<void**>(&suites.exportInfoSuite)));
	ret = spBasicSuite->AcquireSuite(kPrSDKExportFileSuite, kPrSDKExportFileSuiteVersion, const_cast<const void**>(reinterpret_cast<void**>(&suites.exportFileSuite)));
	ret = spBasicSuite->AcquireSuite(kPrSDKPPixSuite, kPrSDKPPixSuiteVersion, const_cast<const void**>(reinterpret_cast<void**>(&suites.ppixSuite)));
	ret = spBasicSuite->AcquireSuite(kPrSDKPPix2Suite, kPrSDKPPix2SuiteVersion, const_cast<const void**>(reinterpret_cast<void**>(&suites.ppix2Suite)));
	ret = spBasicSuite->AcquireSuite(kPrSDKExporterUtilitySuite, kPrSDKExporterUtilitySuiteVersion, const_cast<const void**>(reinterpret_cast<void**>(&suites.exporterUtilitySuite)));
	ret = spBasicSuite->AcquireSuite(kPrSDKSequenceAudioSuite, kPrSDKSequenceAudioSuiteVersion, const_cast<const void**>(reinterpret_cast<void**>(&suites.sequenceAudioSuite)));
	ret = spBasicSuite->AcquireSuite(kPrSDKAppInfoSuite, kPrSDKAppInfoSuiteVersion, const_cast<const void**>(reinterpret_cast<void**>(&suites.appInfoSuite)));
	ret = spBasicSuite->AcquireSuite(kPrSDKErrorSuite, kPrSDKErrorSuiteVersion, const_cast<const void**>(reinterpret_cast<void**>(&suites.errorSuite)));

	suites.appInfoSuite->GetAppInfo(PrSDKAppInfoSuite::kAppInfo_Version, (void*)&appVersion);

	vkdrpro->event("startup", {
		{ "dimension1", "Adobe Premiere Pro" },
		{ "dimension2", "Adobe Premiere Pro " + std::to_string(appVersion.major) + "." + std::to_string(appVersion.minor) + "." + std::to_string(appVersion.patch) }
	});

	return ret;
}

prMALError CPremierePluginApp::endInstance()
{
	prMALError ret;

	ret = suites.basicSuite->ReleaseSuite(kPrSDKTimeSuite, kPrSDKTimeSuiteVersion);
	ret = suites.basicSuite->ReleaseSuite(kPrSDKExportParamSuite, kPrSDKExportParamSuiteVersion);
	ret = suites.basicSuite->ReleaseSuite(kPrSDKExportInfoSuite, kPrSDKExportInfoSuiteVersion);
	ret = suites.basicSuite->ReleaseSuite(kPrSDKExportFileSuite, kPrSDKExportFileSuiteVersion);
	ret = suites.basicSuite->ReleaseSuite(kPrSDKPPixSuite, kPrSDKPPixSuiteVersion);
	ret = suites.basicSuite->ReleaseSuite(kPrSDKPPix2Suite, kPrSDKPPix2SuiteVersion);
	ret = suites.basicSuite->ReleaseSuite(kPrSDKExporterUtilitySuite, kPrSDKExporterUtilitySuiteVersion);
	ret = suites.basicSuite->ReleaseSuite(kPrSDKSequenceAudioSuite, kPrSDKSequenceAudioSuiteVersion);
	ret = suites.basicSuite->ReleaseSuite(kPrSDKAppInfoSuite, kPrSDKAppInfoSuiteVersion);
	ret = suites.basicSuite->ReleaseSuite(kPrSDKErrorSuite, kPrSDKErrorSuiteVersion);

	return ret;
}

prMALError CPremierePluginApp::generateDefaultParams(exGenerateDefaultParamRec* instanceRecP)
{
	const PrSDKExportParamSuite* params = suites.exportParamSuite;

	// Add parameter groups
	csSDK_int32 groupId;
	params->AddMultiGroup(pluginId, &groupId);

	prCreateParamGroup(ADBEVideoTabGroup, ADBETopParamGroup, L"Video");
	prCreateParamGroup(ADBEBasicVideoGroup, ADBEVideoTabGroup, L"Basic Video Settings");
	prCreateParamGroup(ADBEAudioTabGroup, ADBETopParamGroup, L"Audio");
	prCreateParamGroup(ADBEBasicAudioGroup, ADBEAudioTabGroup, L"Basic Audio Settings");
	prCreateParamGroup(VPROVoukoderProTabGroup, ADBETopParamGroup, L"VoukoderPro");
	prCreateParamGroup(VPROVoukoderProConfigurationsGroup, VPROVoukoderProTabGroup, L"Configurations");
	prCreateParamGroup(VPROEncodingSettingsGroup, VPROVoukoderProTabGroup, L"Color Format Input");

	// Get sequence values
	PrParam seqWidth, seqHeight, seqFrameRate, seqSampleRate, seqNumChannels;
	suites.exportInfoSuite->GetExportSourceInfo(pluginId, kExportInfo_VideoWidth, &seqWidth);
	suites.exportInfoSuite->GetExportSourceInfo(pluginId, kExportInfo_VideoHeight, &seqHeight);
	suites.exportInfoSuite->GetExportSourceInfo(pluginId, kExportInfo_VideoFrameRate, &seqFrameRate);
	suites.exportInfoSuite->GetExportSourceInfo(pluginId, kExportInfo_AudioSampleRate, &seqSampleRate);
	suites.exportInfoSuite->GetExportSourceInfo(pluginId, kExportInfo_NumAudioChannels, &seqNumChannels);

	// Req'd for AME
	if (seqWidth.mInt32 == 0)
		seqWidth.mInt32 = 1920;

	if (seqHeight.mInt32 == 0)
		seqHeight.mInt32 = 1080;

	// Get ticks per second
	PrTime ticksPerSecond;
	suites.timeSuite->GetTicksPerSecond(&ticksPerSecond);

	// Get frame rates
	csSDK_int64 ticks = 1;
	csSDK_int32 fps = static_cast<csSDK_int32>(ticksPerSecond / seqFrameRate.mInt64);
	for (csSDK_int32 i = 0; i < sizeof(VideoFramerates) / sizeof(VideoFramerates[0]); i++)
	{
		PrTime time = ticksPerSecond / VideoFramerates[i][0] * VideoFramerates[i][1];
		if (seqFrameRate.mInt64 == time)
		{
			ticks = time;
			break;
		}
	}

	// Basic video settings
	prCreateIntParam(ADBEVideoCodec, ADBEBasicVideoGroup, exParamFlag_none, 0, -1, -1, kPrFalse, kPrTrue);
	prCreateIntParam(ADBEVideoWidth, ADBEBasicVideoGroup, exParamFlag_none, seqWidth.mInt32, 16, 8192, kPrFalse, kPrFalse);
	prCreateIntParam(ADBEVideoHeight, ADBEBasicVideoGroup, exParamFlag_none, seqHeight.mInt32, 16, 8192, kPrFalse, kPrFalse);
	prCreateTimeParam(ADBEVideoFPS, ADBEBasicVideoGroup, exParamFlag_none, ticks, kPrFalse, kPrFalse);
	prCreateIntParam(VPROVideoCustomFPS, ADBEBasicVideoGroup, exParamFlag_none, fps, 1, 999, kPrFalse, kPrTrue);
	prCreateRatioParam(ADBEVideoAspect, ADBEBasicVideoGroup, exParamFlag_none, 1, 1, kPrFalse, kPrFalse);
	prCreateIntParam(ADBEVideoFieldType, ADBEBasicVideoGroup, exParamFlag_none, prFieldsNone, -1, -1, kPrTrue, kPrFalse);

	// Basic audio settings
	prCreateIntParam(ADBEAudioCodec, ADBEBasicAudioGroup, exParamFlag_none, 0, NULL, NULL, kPrFalse, kPrTrue);
	prCreateFloatParam(ADBEAudioRatePerSecond, ADBEBasicAudioGroup, exParamFlag_none, seqSampleRate.mFloat64, NULL, NULL, kPrFalse, kPrFalse);
	prCreateIntParam(ADBEAudioNumChannels, ADBEBasicAudioGroup, exParamFlag_none, seqNumChannels.mInt32, NULL, NULL, kPrFalse, kPrFalse);
	
	// VoukoderPro
	prCreateIntParam(VPROVoukoderProConfigurations, VPROVoukoderProConfigurationsGroup, exParamFlag_none, 0, NULL, NULL, kPrFalse, kPrFalse);
	prCreateButtonParam(VPROVoukoderProConfigureButton, VPROVoukoderProConfigurationsGroup, exParamFlag_independant);

	// Color format input
	prCreateIntParam(VPROVideoColorSubsampling, VPROEncodingSettingsGroup, exParamFlag_none, VPROColorFormat::YUV420, -1, -1, kPrFalse, kPrFalse);
	prCreateIntParam(ADBEVideoBitDepth, VPROEncodingSettingsGroup, exParamFlag_none, 8, -1, -1, kPrFalse, kPrFalse);
	prCreateIntParam(VPROVideoColorTransfer, VPROEncodingSettingsGroup, exParamFlag_none, 1, -1, -1, kPrFalse, kPrTrue);

	return exportReturn_ErrNone;
}

prMALError CPremierePluginApp::postProcessParams(exPostProcessParamsRec* instanceRecP)
{
	exOneParamValueRec intParam, floatParam;

	// ### VIDEO ###

	prSetNameDescription(ADBEVideoTabGroup, L"Video");
	prSetNameDescription(ADBEBasicVideoGroup, L"Basic Video Settings");
	prSetNameDescription(ADBEVideoWidth, L"Width");
	prSetNameDescription(ADBEVideoHeight, L"Height");

	// Get ticks per second
	PrTime ticksPerSecond;
	suites.timeSuite->GetTicksPerSecond(&ticksPerSecond);

	// Frame rate
	prSetNameDescription(ADBEVideoFPS, L"Frame Rate");
	prSetNameDescription(VPROVideoCustomFPS, L"Custom Frame Rate");
	suites.exportParamSuite->ClearConstrainedValues(pluginId, 0, ADBEVideoFPS);
	exOneParamValueRec paramFps;
	for (csSDK_int32 i = 0; i < sizeof(VideoFramerates) / sizeof(VideoFramerates[0]); i++)
	{
		std::wstringstream ws;
		ws << std::fixed;
		ws.precision(2);

		float fps = static_cast<float>(VideoFramerates[i][0]) / static_cast<float>(VideoFramerates[i][1]);
		if (fps == (int)fps)
			ws << (int)fps;
		else
			ws << fps;

		paramFps.timeValue = ticksPerSecond / VideoFramerates[i][0] * VideoFramerates[i][1];

		prUTF16Char name[128];
		copyWideStringIntoUTF16(ws.str().c_str(), name);

		suites.exportParamSuite->AddConstrainedValuePair(pluginId, 0, ADBEVideoFPS, &paramFps, name);
	}
	paramFps.timeValue = 1;
	prUTF16Char name[128];
	copyWideStringIntoUTF16(L"Custom value", name);
	suites.exportParamSuite->AddConstrainedValuePair(pluginId, 0, ADBEVideoFPS, &paramFps, name);

	// Field order
	prSetNameDescription(ADBEVideoFieldType, L"Field Order");
	suites.exportParamSuite->ClearConstrainedValues(pluginId, 0, ADBEVideoFieldType);
	intParam.intValue = prFieldsNone;
	suites.exportParamSuite->AddConstrainedValuePair(pluginId, 0, ADBEVideoFieldType, &intParam, L"Progressive");
	intParam.intValue = prFieldsUpperFirst;
	suites.exportParamSuite->AddConstrainedValuePair(pluginId, 0, ADBEVideoFieldType, &intParam, L"Upper First");
	intParam.intValue = prFieldsLowerFirst;
	suites.exportParamSuite->AddConstrainedValuePair(pluginId, 0, ADBEVideoFieldType, &intParam, L"Lower First");

	// Video aspect
	prSetNameDescription(ADBEVideoAspect, L"Aspect");
	suites.exportParamSuite->ClearConstrainedValues(pluginId, 0, ADBEVideoAspect);
	exOneParamValueRec paramPar;
	for (csSDK_int32 i = 0; i < sizeof(VideoAspects) / sizeof(VideoAspects[0]); i++)
	{
		std::wstringstream ws;
		ws << std::fixed;
		ws.precision(2);

		float aspect = static_cast<float>(VideoAspects[i][0]) / static_cast<float>(VideoAspects[i][1]);
		if (aspect == (int)aspect)
			ws << (int)aspect;
		else
			ws << aspect;

		paramPar.ratioValue.numerator = VideoAspects[i][0];
		paramPar.ratioValue.denominator = VideoAspects[i][1];

		prUTF16Char name[128];
		copyWideStringIntoUTF16(ws.str().c_str(), name);

		suites.exportParamSuite->AddConstrainedValuePair(pluginId, 0, ADBEVideoAspect, &paramPar, name);
	}

	// ### AUDIO ###

	prSetNameDescription(ADBEAudioTabGroup, L"Audio");
	prSetNameDescription(ADBEBasicAudioGroup, L"Basic Audio Settings");

	// Audio rate
	prSetNameDescription(ADBEAudioRatePerSecond, L"Sample Rate");
	std::vector<csSDK_int32> audioSamplingRates = { 16000, 22500, 24000, 32000, 44100, 48000, 96000 };
	suites.exportParamSuite->ClearConstrainedValues(pluginId, 0, ADBEAudioRatePerSecond);
	for (const csSDK_int32 audioSamplingRate : audioSamplingRates)
	{
		floatParam.floatValue = audioSamplingRate;

		std::wstring label = std::to_wstring(audioSamplingRate) + L" Hz";
		suites.exportParamSuite->AddConstrainedValuePair(pluginId, 0, ADBEAudioRatePerSecond, &floatParam, label.c_str());
	}

	// Audio channels
	prSetNameDescription(ADBEAudioNumChannels, L"Channels");
	suites.exportParamSuite->ClearConstrainedValues(pluginId, 0, ADBEAudioNumChannels);
	intParam.intValue = kPrAudioChannelType_Mono;
	suites.exportParamSuite->AddConstrainedValuePair(pluginId, 0, ADBEAudioNumChannels, &intParam, L"Mono");
	intParam.intValue = kPrAudioChannelType_Stereo;
	suites.exportParamSuite->AddConstrainedValuePair(pluginId, 0, ADBEAudioNumChannels, &intParam, L"Stereo");
	intParam.intValue = kPrAudioChannelType_51;
	suites.exportParamSuite->AddConstrainedValuePair(pluginId, 0, ADBEAudioNumChannels, &intParam, L"5.1 Surround");

	// ### VOUKODER PRO ###

	prSetNameDescription(VPROVoukoderProTabGroup, L"VoukoderPro");

	prSetNameDescription(VPROVoukoderProConfigurationsGroup, L"Scenes");
	
	prSetNameDescription(VPROVoukoderProConfigurations, L"Scene");
	suites.exportParamSuite->ClearConstrainedValues(pluginId, 0, VPROVoukoderProConfigurations);

	prSetNameDescription(VPROVoukoderProConfigureButton, L"Open Scene Designer ...");

	prSetNameDescription(VPROEncodingSettingsGroup, L"Input Color Format");

	prSetNameDescription(VPROVideoColorSubsampling, L"Color Subsampling");
	suites.exportParamSuite->ClearConstrainedValues(pluginId, 0, VPROVideoColorSubsampling);
	intParam.intValue = VPROColorFormat::YUV420;
	suites.exportParamSuite->AddConstrainedValuePair(pluginId, 0, VPROVideoColorSubsampling, &intParam, L"YUV 4:2:0");
	intParam.intValue = VPROColorFormat::YUV422;
	suites.exportParamSuite->AddConstrainedValuePair(pluginId, 0, VPROVideoColorSubsampling, &intParam, L"YUV 4:2:2");
	intParam.intValue = VPROColorFormat::YUV444;
	suites.exportParamSuite->AddConstrainedValuePair(pluginId, 0, VPROVideoColorSubsampling, &intParam, L"YUV 4:4:4 (incl. Alpha)");

	prSetNameDescription(ADBEVideoBitDepth, L"Max. Bit Depth");
	suites.exportParamSuite->ClearConstrainedValues(pluginId, 0, ADBEVideoBitDepth);
	//intParam.intValue = 8;
	//suites.exportParamSuite->AddConstrainedValuePair(pluginId, 0, ADBEVideoBitDepth, &intParam, L"8 bit");
	//intParam.intValue = 10;
	//suites.exportParamSuite->AddConstrainedValuePair(pluginId, 0, ADBEVideoBitDepth, &intParam, L"10 bit");

	prSetNameDescription(VPROVideoColorTransfer, L"Color Space");
	suites.exportParamSuite->ClearConstrainedValues(pluginId, 0, VPROVideoColorTransfer);
	intParam.intValue = 1;
	suites.exportParamSuite->AddConstrainedValuePair(pluginId, 0, VPROVideoColorTransfer, &intParam, L"Rec.709");
	//intParam.intValue = 16;
	//suites.exportParamSuite->AddConstrainedValuePair(pluginId, 0, VPROVideoColorTransfer, &intParam, L"Rec.2020 PQ");
	//intParam.intValue = 18;
	//suites.exportParamSuite->AddConstrainedValuePair(pluginId, 0, VPROVideoColorTransfer, &intParam, L"Rec.2020 HLG");

	exParamChangedRec paramchanged;
	strcpy_s(paramchanged.changedParamIdentifier, VPROVoukoderProConfigurations);
	validateParamChanged(&paramchanged);

	return exportReturn_ErrNone;
}

prMALError CPremierePluginApp::queryOutputSettings(exQueryOutputSettingsRec* outputSettingsRecP)
{
	if (outputSettingsRecP->inExportVideo)
	{
		PrTime ticksPerSecond;
		suites.timeSuite->GetTicksPerSecond(&ticksPerSecond);

		// Width
		exParamValues videoWidth;
		suites.exportParamSuite->GetParamValue(pluginId, outputSettingsRecP->inMultiGroupIndex, ADBEVideoWidth, &videoWidth);
		outputSettingsRecP->outVideoWidth = videoWidth.value.intValue;

		// Height
		exParamValues videoHeight;
		suites.exportParamSuite->GetParamValue(pluginId, outputSettingsRecP->inMultiGroupIndex, ADBEVideoHeight, &videoHeight);
		outputSettingsRecP->outVideoHeight = videoHeight.value.intValue;

		// Frame rate
		exParamValues videoFps;
		suites.exportParamSuite->GetParamValue(pluginId, outputSettingsRecP->inMultiGroupIndex, ADBEVideoFPS, &videoFps);
		if (videoFps.value.timeValue > 1)
			outputSettingsRecP->outVideoFrameRate = videoFps.value.timeValue;
		else
		{
			exParamValues customFps;
			suites.exportParamSuite->GetParamValue(pluginId, outputSettingsRecP->inMultiGroupIndex, VPROVideoCustomFPS, &customFps);
			outputSettingsRecP->outVideoFrameRate = ticksPerSecond / customFps.value.intValue;
		}

		// Aspect
		exParamValues videoAspect;
		suites.exportParamSuite->GetParamValue(pluginId, outputSettingsRecP->inMultiGroupIndex, ADBEVideoAspect, &videoAspect);
		outputSettingsRecP->outVideoAspectNum = videoAspect.value.ratioValue.numerator;
		outputSettingsRecP->outVideoAspectDen = videoAspect.value.ratioValue.denominator;

		// Field type
		exParamValues videoFieldType;
		suites.exportParamSuite->GetParamValue(pluginId, outputSettingsRecP->inMultiGroupIndex, ADBEVideoFieldType, &videoFieldType);
		outputSettingsRecP->outVideoFieldType = videoFieldType.value.intValue;
	}

	// Audio
	if (outputSettingsRecP->inExportAudio)
	{
		// Sample rate
		exParamValues audioRatePerSecond;
		suites.exportParamSuite->GetParamValue(pluginId, outputSettingsRecP->inMultiGroupIndex, ADBEAudioRatePerSecond, &audioRatePerSecond);
		outputSettingsRecP->outAudioSampleRate = audioRatePerSecond.value.floatValue;

		// Channel type
		exParamValues audioChannelType;
		suites.exportParamSuite->GetParamValue(pluginId, outputSettingsRecP->inMultiGroupIndex, ADBEAudioNumChannels, &audioChannelType);
		outputSettingsRecP->outAudioChannelType = static_cast<PrAudioChannelType>(audioChannelType.value.intValue);

		// Sample type
		outputSettingsRecP->outAudioSampleType = static_cast<PrAudioSampleType>(kPrAudioSampleType_32BitFloat);
	}

	// Important: This makes it faster!
	outputSettingsRecP->outUseMaximumRenderPrecision = kPrFalse;

	// Don't display an estimation!
	outputSettingsRecP->outBitratePerSecond = 0;

	return malNoError;
}

prMALError CPremierePluginApp::validateParamChanged(exParamChangedRec* paramRecP)
{
	exParamValues customFrameRate, videoFPS, colorFormat, bitDepth, colorTransfer, settingsGroup, config;
	suites.exportParamSuite->GetParamValue(pluginId, 0, VPROVideoCustomFPS, &customFrameRate);
	suites.exportParamSuite->GetParamValue(pluginId, 0, ADBEVideoFPS, &videoFPS);
	suites.exportParamSuite->GetParamValue(pluginId, 0, VPROVideoColorSubsampling, &colorFormat);
	suites.exportParamSuite->GetParamValue(pluginId, 0, ADBEVideoBitDepth, &bitDepth);
	suites.exportParamSuite->GetParamValue(pluginId, 0, VPROVideoColorTransfer, &colorTransfer);
	suites.exportParamSuite->GetParamValue(pluginId, 0, VPROEncodingSettingsGroup, &settingsGroup);
	suites.exportParamSuite->GetParamValue(pluginId, 0, VPROVoukoderProConfigurations, &config);

	// Clear the bit depth options
	suites.exportParamSuite->ClearConstrainedValues(pluginId, 0, ADBEVideoBitDepth);

	exOneParamValueRec intParam;
	if (colorFormat.value.intValue == VPROColorFormat::YUV444)
	{
		// With 444 we only have a 16 bit mode available
		intParam.intValue = 0;
		suites.exportParamSuite->AddConstrainedValuePair(pluginId, 0, ADBEVideoBitDepth, &intParam, L"You shuldn't see this! ;)"); // Just to make 16 bit look nice
		intParam.intValue = 16;
		suites.exportParamSuite->AddConstrainedValuePair(pluginId, 0, ADBEVideoBitDepth, &intParam, L"16 bit");

		// Bit depth is 16 bit only
		bitDepth.disabled = kPrTrue;
		bitDepth.value.intValue = 16;

		// Color transfer is bt709 only
		colorTransfer.disabled = kPrTrue;
		colorTransfer.value.intValue = 1;
	}
	else
	{
		// In non-444 modes we have 8 and 10 bit modes available
		intParam.intValue = 8;
		suites.exportParamSuite->AddConstrainedValuePair(pluginId, 0, ADBEVideoBitDepth, &intParam, L"8 bit");
		intParam.intValue = 10;
		suites.exportParamSuite->AddConstrainedValuePair(pluginId, 0, ADBEVideoBitDepth, &intParam, L"10 bit");
	}

	if (colorFormat.value.intValue == VPROColorFormat::YUV422)
	{
		// Bit depth is 10 bit only
		bitDepth.disabled = kPrTrue;
		bitDepth.value.intValue = 10;

		// All Color transfers are available
		colorTransfer.disabled = kPrTrue;
		colorTransfer.value.intValue = 1;
	}

	if (colorFormat.value.intValue == VPROColorFormat::YUV420)
	{
		// Bit depth
		bitDepth.disabled = kPrFalse;
		if (bitDepth.value.intValue == 16)
			bitDepth.value.intValue = 10;

		//
		if (bitDepth.value.intValue == 10)
		{
			colorTransfer.disabled = kPrFalse;
		}
		else
		{
			colorTransfer.disabled = kPrTrue;
			colorTransfer.value.intValue = 1;
		}
	}

	// Disable video input settings if we don't export video
	settingsGroup.hidden = paramRecP->exportVideo ? kPrFalse : kPrTrue;

	customFrameRate.hidden = videoFPS.value.timeValue > 1;

	// Apply changes
	suites.exportParamSuite->ChangeParam(pluginId, 0, VPROVideoCustomFPS, &customFrameRate);
	suites.exportParamSuite->ChangeParam(pluginId, 0, VPROEncodingSettingsGroup, &settingsGroup);
	suites.exportParamSuite->ChangeParam(pluginId, 0, ADBEVideoBitDepth, &bitDepth);
	suites.exportParamSuite->ChangeParam(pluginId, 0, VPROVideoColorTransfer, &colorTransfer);

	// Configs
	std::vector<std::shared_ptr<VoukoderPro::SceneInfo>> scenes;
	vkdrpro->sceneManager()->load(scenes);

	// Populate configs
	suites.exportParamSuite->ClearConstrainedValues(pluginId, 0, VPROVoukoderProConfigurations);
	m_scenes.clear();

	csSDK_int32 hashSelected = -1;
	exOneParamValueRec entry;
	for (auto const& sceneInfo : scenes)
	{
		// Convert UTF8 to UTF16
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> converter;

		// Create a hash key first
		entry.intValue = hash(sceneInfo->name.c_str());
		suites.exportParamSuite->AddConstrainedValuePair(pluginId, 0, VPROVoukoderProConfigurations, &entry, converter.from_bytes(sceneInfo->name).c_str());

		// Map the hash key to the config id
		m_scenes.insert(std::make_pair(entry.intValue, sceneInfo));

		// Store the first hash that matches
		if (hashSelected == -1 && entry.intValue == config.value.intValue)
		{
			hashSelected = entry.intValue;

			if (strcmp(paramRecP->changedParamIdentifier, VPROVoukoderProConfigurations) == 0)
				vkdrpro->setScene(sceneInfo);
		}
	}

	// If there are no scenes configured ...
	if (m_scenes.size() <= 0)
	{
		// Add a dummy entry
		entry.intValue = 0;
		suites.exportParamSuite->AddConstrainedValuePair(pluginId, 0, VPROVoukoderProConfigurations, &entry, L"Please create a scene first");

		// Select that entry and disable the dropdown
		config.value.intValue = 0;
		config.disabled = kPrTrue;
	}
	else
	{
		config.disabled = kPrFalse;

		// If none is selected select the first entry
		if (hashSelected == -1 && config.value.intValue == 0)
			config.value.intValue = hash(scenes.front()->name.c_str());
	}

	suites.exportParamSuite->ChangeParam(pluginId, 0, VPROVoukoderProConfigurations, &config);

	return malNoError;
}

prMALError CPremierePluginApp::buttonAction(exParamButtonRec* paramButtonRecP)
{
	if (strcmp(paramButtonRecP->buttonParamIdentifier, VPROVoukoderProConfigureButton) == 0)
	{
		// Get currently active scene
		exParamValues scene;
		suites.exportParamSuite->GetParamValue(pluginId, 0, VPROVoukoderProConfigurations, &scene);

		std::string name = "";

		// Configs
		std::vector<std::shared_ptr<VoukoderPro::SceneInfo>> scenes;
		vkdrpro->sceneManager()->load(scenes);

		for (auto const& sceneInfo : scenes)
		{
			if (hash(sceneInfo->name.c_str()) == scene.value.intValue)
			{
				name = sceneInfo->name;
				break;
			}
		}

		// Open the scene designer
		if (vkdrpro->configure(name) == 0)
		{
			// Rebuild scene selector
			exParamChangedRec paramchanged;
			validateParamChanged(&paramchanged);
		}
	}

	return exportReturn_ErrNone;
}

/**
* Set the currently selected file extension. This is usually based on the currently
* associated muxer associated with the "OutputFilename" property.
* 
* If this property is not used the extension is .voukoderpro - but it is not used
* anyway.
*/
prMALError CPremierePluginApp::queryExportFileExtension(exQueryExportFileExtensionRec* exportFileExtensionRecP)
{
	exParamValues scene;
	suites.exportParamSuite->GetParamValue(pluginId, 0, VPROVoukoderProConfigurations, &scene);

	const std::string extension = vkdrpro->extension();
	const std::wstring wextension = std::wstring(extension.begin(), extension.end());
	copyWideStringIntoUTF16(wextension.c_str(), exportFileExtensionRecP->outFileExtension);

	return malNoError;
}

prMALError CPremierePluginApp::getParamSummary(exParamSummaryRec* summaryRecP)
{
	// This should always be true
	if (summaryRecP->exportVideo)
	{
		exParamValues width, height, frameRate;
		suites.exportParamSuite->GetParamValue(pluginId, 0, ADBEVideoWidth, &width);
		suites.exportParamSuite->GetParamValue(pluginId, 0, ADBEVideoHeight, &height);
		suites.exportParamSuite->GetParamValue(pluginId, 0, ADBEVideoFPS, &frameRate);

		std::wstringstream summary;
		summary << std::fixed;
		summary.precision(2);

		// Frame size
		summary << width.value.intValue << "x" << height.value.intValue << ", ";

		// FPS
		if (frameRate.value.timeValue > 1)
		{
			PrTime ticksPerSecond;
			suites.timeSuite->GetTicksPerSecond(&ticksPerSecond);

			const float fps = static_cast<float>(ticksPerSecond) / static_cast<float>(frameRate.value.timeValue);

			summary << fps << " fps";
		}
		else
		{
			exParamValues customTicksPerFrame;
			suites.exportParamSuite->GetParamValue(pluginId, 0, VPROVideoCustomFPS, &customTicksPerFrame);

			summary << customTicksPerFrame.value.intValue << " fps";
		}

		prUTF16CharCopy(summaryRecP->videoSummary, summary.str().c_str());
	}

	if (summaryRecP->exportAudio)
	{
		exParamValues sampleRate, channelType;
		suites.exportParamSuite->GetParamValue(pluginId, 0, ADBEAudioRatePerSecond, &sampleRate);
		suites.exportParamSuite->GetParamValue(pluginId, 0, ADBEAudioNumChannels, &channelType);

		std::wstringstream summary;
		summary << std::fixed;
		summary.precision(0);
		summary << sampleRate.value.floatValue << " Hz, ";

		switch (channelType.value.intValue)
		{
		case 1:
			summary << "Mono";
			break;
		case 2:
			summary << "Stereo";
			break;
		case 6:
			summary << "5.1 Surround";
			break;
		default:
			summary << "Unknown";
		}

		prUTF16CharCopy(summaryRecP->audioSummary, summary.str().c_str());
	}

	return malNoError;
}

const std::string CPremierePluginApp::getFilename(csSDK_uint32 fileObject)
{
	prUTF16Char prFilename[kPrMaxPath];
	csSDK_int32 prFilenameLength = kPrMaxPath;
	suites.exportFileSuite->GetPlatformPath(fileObject, &prFilenameLength, prFilename);

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;

	return convert.to_bytes(prFilename);
}

prMALError CPremierePluginApp::StartExport(exDoExportRec* exportRecP)
{
	PrTime ticksPerFrame;

	exParamValues configuration, videoFPS;
	suites.exportParamSuite->GetParamValue(pluginId, 0, VPROVoukoderProConfigurations, &configuration);
	suites.exportParamSuite->GetParamValue(pluginId, 0, ADBEVideoFPS, &videoFPS);

	// Do we use a standard frame rate or a custom one
	int tbNum = 0;
	int tbDen = 0;

	if (videoFPS.value.timeValue == 1)
	{
		PrTime ticksPerSecond;
		exParamValues customTicksPerFrame;
		suites.timeSuite->GetTicksPerSecond(&ticksPerSecond);
		suites.exportParamSuite->GetParamValue(pluginId, 0, VPROVideoCustomFPS, &customTicksPerFrame);

		// Calculate ticks
		ticksPerFrame = ticksPerSecond / customTicksPerFrame.value.intValue;

		// Set a fraction FFmpeg understands
		tbNum = 1;
		tbDen = customTicksPerFrame.value.intValue;
	}
	else
	{
		// Get ticks from Premiere
		ticksPerFrame = videoFPS.value.timeValue;

		// Convert PrTime to a fraction FFmpeg understands
		const PrTime timebase = GCD(254016000000, ticksPerFrame);
		tbNum = static_cast<int>(ticksPerFrame / timebase);
		tbDen = static_cast<int>(254016000000 / timebase);
	}

	// Get active Scene
	if (m_scenes.find(configuration.value.intValue) != m_scenes.end())
	{
		// Found the right config
		vkdrpro->setScene(m_scenes.at(configuration.value.intValue));
	}
	else if (m_scenes.size() > 0)
	{
		// Fallback to the first config
		vkdrpro->setScene(m_scenes.begin()->second);
	}
	else
	{
		SHOW_ERROR("No export scene set!", "No export scene was set. Maybe you want to click 'Open Scene Designer' to set up a scene first?");

		return exportReturn_ErrLastErrorSet;
	}

	// Project structure
	VoukoderPro::config project;
	project[VoukoderPro::pPropApplication] = "Adobe Premiere Pro CC";
	project[VoukoderPro::pPropApplicationVersion] = std::to_string(appVersion.major) + "." + std::to_string(appVersion.minor) + "." + std::to_string(appVersion.patch);
	project[VoukoderPro::pPropFilename] = getFilename(exportRecP->fileObject);

	// Find the right pixel format
	PrPixelFormat pixelFormat;
	VoukoderPro::configType colorFormat;
	if (!getPixelFormat(pixelFormat, colorFormat))
	{
		SHOW_ERROR("PixelFormat not found!", "Unable to find an appropiate PixelFormat.");

		return exportReturn_ErrInvalidPixelFormat;
	}

	prSuiteError err = suiteError_NoError;

	// Video track
	if (exportRecP->exportVideo == kPrTrue)
	{
		exParamValues width, height, aspect, fieldType;
		suites.exportParamSuite->GetParamValue(pluginId, 0, ADBEVideoWidth, &width);
		suites.exportParamSuite->GetParamValue(pluginId, 0, ADBEVideoHeight, &height);
		suites.exportParamSuite->GetParamValue(pluginId, 0, ADBEVideoAspect, &aspect);
		suites.exportParamSuite->GetParamValue(pluginId, 0, ADBEVideoFieldType, &fieldType);

		// Set basic values
		VoukoderPro::config track;
		track[VoukoderPro::pPropType] = "video";
		track[VoukoderPro::pPropWidth] = width.value.intValue;
		track[VoukoderPro::pPropHeight] = height.value.intValue;
		track[VoukoderPro::pPropTimebaseNum] = tbNum;
		track[VoukoderPro::pPropTimebaseDen] = tbDen;
		track[VoukoderPro::pPropAspectNum] = aspect.value.ratioValue.numerator;
		track[VoukoderPro::pPropAspectDen] = aspect.value.ratioValue.denominator;

		// Field order
		if (fieldType.value.intValue == prFieldsUpperFirst)
			track[VoukoderPro::pPropFieldOrder] = "tff";
		else if (fieldType.value.intValue == prFieldsLowerFirst)
			track[VoukoderPro::pPropFieldOrder] = "bff";
		else if (fieldType.value.intValue == prFieldsNone)
			track[VoukoderPro::pPropFieldOrder] = "progressive";
		else
		{
			SHOW_ERROR("Invalid field format!", "Supplied field format is not supported.");

			return exportReturn_ErrVideoEncoderConfiguration;
		}

		// Insert color format settings
		track.insert(colorFormat.begin(), colorFormat.end());

		// Set the timecode
		std::string timecode;
		if (getSourceTimecode(timecode, ticksPerFrame))
			track[VoukoderPro::pPropTimecode] = timecode;

		// Add video track
		project.tracks.push_back(track);

		// Create buffer
		video = std::make_shared<FrameBuffer>();
		video->track = static_cast<int>(project.tracks.size() - 1);
		video->timebaseNum = tbNum;
		video->timebaseDen = tbDen;
	}

	// Audio track
	if (exportRecP->exportAudio == kPrTrue)
	{
		exParamValues sampleRate, numChannels;
		suites.exportParamSuite->GetParamValue(pluginId, 0, ADBEAudioRatePerSecond, &sampleRate);
		suites.exportParamSuite->GetParamValue(pluginId, 0, ADBEAudioNumChannels, &numChannels);

		// Set basic values
		VoukoderPro::config track;
		track[VoukoderPro::pPropType] = "audio";
		track[VoukoderPro::pPropChannelCount] = numChannels.value.intValue;
		track[VoukoderPro::pPropSamplingRate] = static_cast<int>(sampleRate.value.floatValue);
		track[VoukoderPro::pPropFormat] = "fltp";

		// Channel layout
		PrAudioChannelLabel labels[6] = {};
		switch (numChannels.value.intValue)
		{
		case 6:
			labels[0] = kPrAudioChannelLabel_FrontLeft;
			labels[1] = kPrAudioChannelLabel_FrontRight;
			labels[2] = kPrAudioChannelLabel_FrontCenter;
			labels[3] = kPrAudioChannelLabel_LowFrequency;
			labels[4] = kPrAudioChannelLabel_BackLeft;
			labels[5] = kPrAudioChannelLabel_BackRight;
			track[VoukoderPro::pPropChannelLayout] = "5.1";
			break;

		case 2:
			labels[0] = kPrAudioChannelLabel_FrontLeft;
			labels[1] = kPrAudioChannelLabel_FrontRight;
			track[VoukoderPro::pPropChannelLayout] = "stereo";
			break;

		case 1:
			labels[0] = kPrAudioChannelLabel_Discrete;
			track[VoukoderPro::pPropChannelLayout] = "mono";
			break;

		default:
			return suiteError_CompilerErrCodecBadInput;
		}

		// Calculate total sample count
		PrTime ticksPerSample;
		suites.timeSuite->GetTicksPerAudioSample(static_cast<float>(sampleRate.value.floatValue), &ticksPerSample);
		totalSampleCount = 1 + ((exportRecP->endTime - exportRecP->startTime - 1) / ticksPerSample);
		sampleCount = INT_MAX;

		// How many audio samples fit into one video frame?
		if (video)
			sampleCount = (static_cast<csSDK_uint32>(video->timebaseNum) * static_cast<csSDK_uint32>(sampleRate.value.floatValue)) / static_cast<csSDK_uint32>(video->timebaseDen);

		// Create audio renderer
		err = suites.sequenceAudioSuite->MakeAudioRenderer(pluginId, exportRecP->startTime, numChannels.value.intValue, labels, PrAudioSampleType::kPrAudioSampleType_32BitFloat, static_cast<float>(sampleRate.value.floatValue), &audioRendererId);
		if (PrSuiteErrorFailed(err))
			return err;

		// Get max. chunk size
		csSDK_int32 maxSampleCount = 0;
		err = suites.sequenceAudioSuite->GetMaxBlip(audioRendererId, ticksPerFrame, &maxSampleCount);
		if (PrSuiteErrorFailed(err))
			return err;

		// Calculate total sample count
		if (sampleCount > (csSDK_uint32)maxSampleCount)
			sampleCount = maxSampleCount;

		// Add audio track
		project.tracks.push_back(track);

		// Create buffer
		audio = std::make_shared<FrameBuffer>(sampleCount * sizeof(float));
		audio->track = static_cast<int>(project.tracks.size() - 1);
		audio->timebaseNum = 1;
		audio->timebaseDen = static_cast<int>(sampleRate.value.floatValue);
	}

	// Open
	if (vkdrpro->open(project) < 0)
	{
		prUTF16Char name[128];
		copyWideStringIntoUTF16(L"VoukoderPro", name);

		prUTF16Char text[128];
		copyWideStringIntoUTF16(L"Error", text);

		suites.exporterUtilitySuite->ReportEvent(exportRecP->exporterPluginID, PrSDKErrorSuite3::kEventTypeError, name, text);

		return exportReturn_ErrLastErrorSet;
	}

	vkdrpro->event("export", {
		{ "dimension1", "Adobe Premiere Pro" },
		{ "dimension2", "Adobe Premiere Pro " + std::to_string(appVersion.major) + "." + std::to_string(appVersion.minor) + "." + std::to_string(appVersion.patch) }
	});

	// Start rendering
	ExportLoopRenderParams renderParams;
	renderParams.inRenderParamsSize = sizeof(ExportLoopRenderParams);
	renderParams.inRenderParamsVersion = 1;
	renderParams.inStartTime = exportRecP->startTime;
	renderParams.inEndTime = exportRecP->endTime;
	renderParams.inReservedProgressPreRender = 0;
	renderParams.inReservedProgressPostRender = 0;
	renderParams.inFinalPixelFormat = pixelFormat;

	// Create C conform callback
	Callback<prSuiteError(csSDK_uint32, csSDK_uint32, csSDK_uint32, PPixHand, void*)>::func = std::bind(&CPremierePluginApp::frameCompleteCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	PrSDKMultipassExportLoopFrameCompletionFunction callback = static_cast<PrSDKMultipassExportLoopFrameCompletionFunction>(Callback<prSuiteError(csSDK_uint32, csSDK_uint32, csSDK_uint32, PPixHand, void*)>::callback);

	// Start a single pass export
	err = suites.exporterUtilitySuite->DoMultiPassExportLoop(pluginId, &renderParams, 1, callback, nullptr);

	vkdrpro->close();

	// Release audio renderer if needed
	if (audioRendererId > 0)
		suites.sequenceAudioSuite->ReleaseAudioRenderer(pluginId, audioRendererId);

	return err;
}

prSuiteError CPremierePluginApp::frameCompleteCallback(const csSDK_uint32 pass, const csSDK_uint32 frameNumber, const csSDK_uint32 frameRepeatCount, PPixHand renderedFrame, void* callbackData)
{
	prSuiteError err = suiteError_NoError;

	// Export video
	auto renderVideo = [&](csSDK_uint32 frameNo, const bool compressed)
	{
		// Write video frame
		int ret;
		if (compressed)
			ret = vkdrpro->writeCompressedVideoFrame(video->track, frameNo, video->buffer[0], video->linesize[0]);
		else
			ret = vkdrpro->writeVideoFrame(video->track, frameNo, video->buffer, video->linesize);

		video->pts++;

		return (ret < 0) ? suiteError_CompilerInternalError : suiteError_NoError;
	};

	// Export audio
	auto renderAudio = [&]()
	{
		// Fetch next audio chunk
		err = suites.sequenceAudioSuite->GetAudio(audioRendererId, sampleCount, (float**)audio->buffer, kPrFalse);
		if (PrSuiteErrorFailed(err))
			return err;

		// Write audio samples
		if (vkdrpro->writeAudioSamples(audio->track, audio->buffer, static_cast<int>(sampleCount * 4)) < 0)
		{
			err = suiteError_CompilerInternalError;
			return err;
		}

		totalSampleCount -= sampleCount;
		audio->pts += sampleCount;
		
		return suiteError_NoError;
	};

	if (video)
	{
		// Get actual pixel format
		PrPixelFormat format;
		suites.ppixSuite->GetPixelFormat(renderedFrame, &format);

		// Check supported pixel formats
		if (format == PrPixelFormat_YUV_420_MPEG4_FRAME_PICTURE_BIPLANAR_8u_709 ||
			format == PrPixelFormat_YUV_420_MPEG4_FRAME_PICTURE_BIPLANAR_10u_as16u_709 ||
			format == PrPixelFormat_YUV_420_MPEG4_FRAME_PICTURE_BIPLANAR_10u_as16u_2020_HDR ||
			format == PrPixelFormat_YUV_420_MPEG4_FRAME_PICTURE_BIPLANAR_10u_as16u_2020_HDR_HLG)
		{
			// Get plane buffers
			suites.ppix2Suite->GetYUV420PlanarBuffers(renderedFrame, PrPPixBufferAccess_ReadOnly,
				(char**)&video->buffer[0], (csSDK_uint32*)&video->linesize[0],   // Y
				(char**)&video->buffer[1], (csSDK_uint32*)&video->linesize[1],   // U (or UV interleaved)
				(char**)&video->buffer[2], (csSDK_uint32*)&video->linesize[2]);  // V (or none)
		}
		else if (format == PrPixelFormat_V210_422_10u_709)
		{
			// Get stride
			csSDK_int32 rowBytes;
			suites.ppixSuite->GetRowBytes(renderedFrame, &rowBytes);

			video->linesize[0] = rowBytes;

			// Get the raw pixel buffer
			prSuiteError err = suites.ppixSuite->GetPixels(renderedFrame, PrPPixBufferAccess_ReadOnly, (char**)&(video->buffer[0])); // packed YUV
		}
		else if (format == PrPixelFormat_VUYA_4444_32f_709)
		{
			// Get stride
			csSDK_int32 rowBytes;
			suites.ppixSuite->GetRowBytes(renderedFrame, &rowBytes);

			// Get the raw pixel buffer
			float* pixels;
			prSuiteError err = suites.ppixSuite->GetPixels(renderedFrame, PrPPixBufferAccess_ReadOnly, (char**)&pixels); // packed YUVA

			// Frame bounds
			prRect bounds;
			err = suites.ppixSuite->GetBounds(renderedFrame, &bounds);
			const int width = bounds.right - bounds.left;
			const int height = bounds.bottom - bounds.top;

			video->buffer[0] = (uint8_t*)malloc(rowBytes * height);
			video->linesize[0] = width * 8;

			// Values for FMA
			const __m256 mul = _mm256_setr_ps(65535.0f, 56283.17216f, 57342.98164f, 57342.98164f, 65535.0f, 56283.17216f, 57342.98164f, 57342.98164f);
			const __m256 add = _mm256_setr_ps(0.0f, 4112.04855f, 32767.0f, 32767.0f, 0.0f, 4112.04855f, 32767.0f, 32767.0f);

			int q = 0;
			for (int r = height - 1; r >= 0; r--)
			{
				const float* p = &pixels[r * width * 4];

				for (int c = 0; c < (int)width * 4; c += 16)
				{
					__m256i ayuv1 = load_and_scale256(p + c, mul, add);
					__m256i ayuv2 = load_and_scale256(p + c + 8, mul, add);
					__m256i ayuvi = _mm256_packus_epi32(ayuv1, ayuv2);
					__m256i ayuv = _mm256_permute4x64_epi64(ayuvi, 0b11011000); // AVX2 required!
					_mm256_storeu_si256((__m256i*)(video->buffer[0] + q), ayuv);

					q += 32;
				}
			}
		}
		else
			return suiteError_RenderInvalidPixelFormat;

		for (csSDK_uint32 repeat = 0; repeat < frameRepeatCount; repeat++)
		{
			// Encode video
			err = renderVideo(frameNumber + repeat, format == PrPixelFormat_V210_422_10u_709);
			if (PrSuiteErrorFailed(err))
				break;

			// If "video" has more progress than "audio" -> continue with an "audio" frame
			while (audio && totalSampleCount > 0 && err == suiteError_NoError &&
				(video->pts * video->timebaseNum * audio->timebaseDen - video->timebaseDen * audio->timebaseNum * audio->pts) > 0)
				err = renderAudio();

			if (PrSuiteErrorFailed(err))
				break;

			// Increase progress bar by one frame
			err = suites.exporterUtilitySuite->ReportIntermediateProgressForRepeatedVideoFrame(pluginId, 1);
			if (PrSuiteErrorFailed(err))
				break;
		}

		// Free the conversion buffer
		if (format == PrPixelFormat_VUYA_4444_32f_709 && video->buffer[0])
		{
			free(video->buffer[0]);
			video->buffer[0] = nullptr;
		}
	}
	else
	{
		// Process audio stream only
		while (audio && totalSampleCount > 0 && err == suiteError_NoError)
			err = renderAudio();

		// Increase progress bar by all frames
		err = suites.exporterUtilitySuite->ReportIntermediateProgressForRepeatedVideoFrame(pluginId, frameRepeatCount);
	}

	return err;
}

bool CPremierePluginApp::getSourceTimecode(std::string& out, PrTime timebase)
{
	prSuiteError err = suiteError_NoError;

	// Get sorce timecode information
	PrParam sourceTimecode;
	err = suites.exportInfoSuite->GetExportSourceInfo(pluginId, kExportInfo_SourceTimecode, &sourceTimecode);
	if (PrSuiteErrorFailed(err))
		return false;

	ExporterTimecodeRec* tc = reinterpret_cast<ExporterTimecodeRec*>(sourceTimecode.mMemoryPtr);
	csSDK_int64 ticks = tc->mTimecodeTicks;

	// We don't need that pointer anymore
	suites.memorySuite->PrDisposePtr(sourceTimecode.mMemoryPtr);

	// Constant values
	const csSDK_int64 secs = 254016000000;
	const csSDK_int64 mins = secs * 60;
	const csSDK_int64 hours = mins * 60;
	const csSDK_int64 frames = tc->mTicksPerFrame;

	std::stringstream timecode;
	
	// HRS
	{
		const csSDK_int64 sceneInfo = static_cast<csSDK_int64>(ticks / hours);
		ticks -= sceneInfo * hours;
		timecode << std::setw(2) << std::setfill('0') << sceneInfo << ':';
	}

	// MIN
	{
		const csSDK_int64 sceneInfo = static_cast<csSDK_int64>(ticks / mins);
		ticks -= sceneInfo * mins;
		timecode << std::setw(2) << std::setfill('0') << sceneInfo << ':';
	}

	// SEC
	{
		const csSDK_int64 sceneInfo = static_cast<csSDK_int64>(ticks / secs);
		ticks -= sceneInfo * secs;
		timecode << std::setw(2) << std::setfill('0') << sceneInfo << ':';
	}

	// FRM
	{
		const csSDK_int64 sceneInfo = static_cast<csSDK_int64>(ticks / frames);
		timecode << std::setw(2) << std::setfill('0') << sceneInfo;
	}

	out = timecode.str();

	return true;
}

bool CPremierePluginApp::getPixelFormat(PrPixelFormat& premiereFormat, VoukoderPro::configType& colorFormat)
{
	// Get current settings
	exParamValues bitDepth, subsampling, colorTransfer, fieldOrder;
	suites.exportParamSuite->GetParamValue(pluginId, 0, ADBEVideoBitDepth, &bitDepth);
	suites.exportParamSuite->GetParamValue(pluginId, 0, VPROVideoColorSubsampling, &subsampling);
	suites.exportParamSuite->GetParamValue(pluginId, 0, VPROVideoColorTransfer, &colorTransfer);
	suites.exportParamSuite->GetParamValue(pluginId, 0, ADBEVideoFieldType, &fieldOrder);

	if (subsampling.value.intValue == VPROColorFormat::YUV420)
	{
		if (bitDepth.value.intValue == 8)
		{
			premiereFormat = fieldOrder.value.intValue == prFieldsNone ? PrPixelFormat_YUV_420_MPEG4_FRAME_PICTURE_BIPLANAR_8u_709 : PrPixelFormat_YUV_420_MPEG4_FIELD_PICTURE_BIPLANAR_8u_709;
			colorFormat[VoukoderPro::pPropFormat] = "nv12";
			colorFormat[VoukoderPro::pPropColorRange] = "tv";
			colorFormat[VoukoderPro::pPropColorSpace] = "bt709";
			colorFormat[VoukoderPro::pPropColorPrimaries] = "bt709";
			colorFormat[VoukoderPro::pPropColorTransfer] = "bt709";
		}
		else if (bitDepth.value.intValue == 10)
		{
			if (colorTransfer.value.intValue == 1)
			{
				premiereFormat = fieldOrder.value.intValue == prFieldsNone ? PrPixelFormat_YUV_420_MPEG4_FRAME_PICTURE_BIPLANAR_10u_as16u_709 : PrPixelFormat_YUV_420_MPEG4_FIELD_PICTURE_BIPLANAR_10u_as16u_709;
				colorFormat[VoukoderPro::pPropFormat] = "p010";
				colorFormat[VoukoderPro::pPropColorRange] = "tv";
				colorFormat[VoukoderPro::pPropColorSpace] = "bt709";
				colorFormat[VoukoderPro::pPropColorPrimaries] = "bt709";
				colorFormat[VoukoderPro::pPropColorTransfer] = "bt709";
			}
			else if (colorTransfer.value.intValue == 16) // Todo: Premiere always returns an error when trying to use this pixfmt
			{
				premiereFormat = fieldOrder.value.intValue == prFieldsNone ? PrPixelFormat_YUV_420_MPEG4_FRAME_PICTURE_BIPLANAR_10u_as16u_2020_HDR : PrPixelFormat_YUV_420_MPEG4_FIELD_PICTURE_BIPLANAR_10u_as16u_2020_HDR;
				colorFormat[VoukoderPro::pPropFormat] = "p010";
				colorFormat[VoukoderPro::pPropColorRange] = "tv";
				colorFormat[VoukoderPro::pPropColorSpace] = "bt2020ncl";
				colorFormat[VoukoderPro::pPropColorPrimaries] = "bt2020";
				colorFormat[VoukoderPro::pPropColorTransfer] = "smpte2084";
			}
			else if (colorTransfer.value.intValue == 18) // Todo: Premiere always returns an error when trying to use this pixfmt
			{
				premiereFormat = fieldOrder.value.intValue == prFieldsNone ? PrPixelFormat_YUV_420_MPEG4_FRAME_PICTURE_BIPLANAR_10u_as16u_2020_HDR_HLG : PrPixelFormat_YUV_420_MPEG4_FIELD_PICTURE_BIPLANAR_10u_as16u_2020_HDR_HLG;
				colorFormat[VoukoderPro::pPropFormat] = "p010";
				colorFormat[VoukoderPro::pPropColorRange] = "tv";
				colorFormat[VoukoderPro::pPropColorSpace] = "bt2020ncl";
				colorFormat[VoukoderPro::pPropColorPrimaries] = "bt2020";
				colorFormat[VoukoderPro::pPropColorTransfer] = "arib-std-b67";
			}
		}
	}
	else if (subsampling.value.intValue == VPROColorFormat::YUV422)
	{
		premiereFormat = PrPixelFormat_V210_422_10u_709;
		colorFormat[VoukoderPro::pPropFormat] = "v210";
		colorFormat[VoukoderPro::pPropColorRange] = "tv";
		colorFormat[VoukoderPro::pPropColorSpace] = "bt709";
		colorFormat[VoukoderPro::pPropColorPrimaries] = "bt709";
		colorFormat[VoukoderPro::pPropColorTransfer] = "bt709";
	}
	else if (subsampling.value.intValue == VPROColorFormat::YUV444)
	{
		premiereFormat = PrPixelFormat_VUYA_4444_32f_709;
		colorFormat[VoukoderPro::pPropFormat] = "ayuv64le";
		colorFormat[VoukoderPro::pPropColorRange] = "tv";
		colorFormat[VoukoderPro::pPropColorSpace] = "bt709";
		colorFormat[VoukoderPro::pPropColorPrimaries] = "bt709";
		colorFormat[VoukoderPro::pPropColorTransfer] = "bt709";
	}
	else
		return false;

	return true;
}