#pragma once

#include <functional>
#include <boost/function.hpp>
#include <boost/dll.hpp>

#include "Premiere Pro 22.0 C++ SDK/Examples/Headers/PrSDKExport.h"
#include "Premiere Pro 22.0 C++ SDK/Examples/Headers/PrSDKAppInfoSuite.h"
#include "suites.h"

#include "../../VoukoderPro/voukoderpro_api.h"

typedef std::shared_ptr<VoukoderPro::IClient>(pluginapi_create_t)();

#define ALIGN16(value) (((value + 15) >> 4) << 4)
#define ALIGN32(X) (((mfxU32)((X) + 31)) & (~(mfxU32)31))

const PrTime VideoFramerates[][2] = { {24000, 1001}, {24, 1}, {25, 1}, {30000, 1001}, {30, 1}, {50, 1}, {60000, 1001}, {60, 1} };
const csSDK_int32 VideoAspects[][2] = { { 1, 1 }, { 10, 11 }, { 40, 33 }, { 768, 702 }, { 1024, 702 }, { 2, 1 }, { 4, 3 }, { 3, 2 } };

#define VPROVideoCustomFPS "VPROVideoCustomFPS"
#define VPROEncodingSettingsGroup "VPROEncodingSettingsGroup"
#define VPROVideoColorSubsampling "VPROVideoColorSubsampling"
#define VPROVideoColorTransfer "VPROVideoColorTransfer"
#define VPROVoukoderProTabGroup "VPROVoukoderProTabGroup"
#define VPROVoukoderProConfigurationsGroup "VPROVoukoderProConfigurationsGroup"
#define VPROVoukoderProConfigurations "VPROVoukoderProConfigurations"
#define VPROVoukoderProConfigureButton "VPROVoukoderProConfigureButton"

#define SHOW_ERROR(SUBJECT, TEXT) { \
	vkdrpro->log(TEXT); \
	prUTF16Char subject[128]; copyWideStringIntoUTF16(L##SUBJECT, subject); \
	prUTF16Char text[128]; copyWideStringIntoUTF16(L##TEXT, text); \
	suites.errorSuite->SetEventStringUnicode(PrSDKErrorSuite::kEventTypeError, subject, text); }

enum VPROColorFormat
{
	YUV420 = '420 ',
	YUV422 = '422 ',
	YUV444 = '444 '
};

// UI helpers
#define prCreateIntParam(_identifier_, _groupName_, _paramflags_, _defaultValue_, _minValue_, _maxValue_, _disabled_, _hidden_) { \
	exParamValues val; \
	val.structVersion = 1; \
	val.value.intValue = _defaultValue_; \
	if (_minValue_ > -1) val.rangeMin.intValue = _minValue_; \
	if (_maxValue_ > -1) val.rangeMax.intValue = _maxValue_; \
	val.disabled = _disabled_; \
	val.hidden = _hidden_; \
	val.optionalParamEnabled = kPrFalse; \
	exNewParamInfo par; \
	par.structVersion = 1; \
	par.flags = _paramflags_; \
	par.paramType = exParamType_int; \
	par.paramValues = val; \
    strncpy_s(par.identifier, _identifier_, sizeof(_identifier_)); \
	suites.exportParamSuite->AddParam(pluginId, 0, _groupName_, &par); \
}

#define prCreateFloatParam(_identifier_, _groupName_, _paramflags_, _defaultValue_, _minValue_, _maxValue_, _disabled_, _hidden_) { \
	exParamValues val; \
	val.structVersion = 1; \
	val.value.floatValue = _defaultValue_; \
	if (_minValue_ > -1) val.rangeMin.floatValue = _minValue_; \
	if (_maxValue_ > -1) val.rangeMax.floatValue = _maxValue_; \
	val.disabled = _disabled_; \
	val.hidden = _hidden_; \
	val.optionalParamEnabled = kPrFalse; \
	exNewParamInfo par; \
	par.structVersion = 1; \
	par.flags = _paramflags_; \
	par.paramType = exParamType_float; \
	par.paramValues = val; \
    strncpy_s(par.identifier, _identifier_, sizeof(_identifier_)); \
	suites.exportParamSuite->AddParam(pluginId, 0, _groupName_, &par); \
}

#define prCreateBoolParam(_identifier_, _groupName_, _paramflags_, _defaultValue_, _disabled_, _hidden_) { \
	exParamValues val; \
	val.structVersion = 1; \
	val.value.intValue = _defaultValue_; \
	val.disabled = _disabled_; \
	val.hidden = _hidden_; \
	val.optionalParamEnabled = kPrFalse; \
	exNewParamInfo par; \
	par.structVersion = 1; \
	par.flags = _paramflags_; \
	par.paramType = exParamType_bool; \
	par.paramValues = val; \
    strncpy_s(par.identifier, _identifier_, sizeof(_identifier_)); \
	suites.exportParamSuite->AddParam(pluginId, 0, _groupName_, &par); \
}

#define prCreateRatioParam(_identifier_, _groupName_, _paramflags_, _defaultValueNum_, _defaultValueDen_, _disabled_, _hidden_) { \
	exParamValues val; \
	val.structVersion = 1; \
	val.value.ratioValue.numerator = _defaultValueNum_; \
	val.value.ratioValue.denominator = _defaultValueDen_; \
	val.disabled = _disabled_; \
	val.hidden = _hidden_; \
	val.optionalParamEnabled = kPrFalse; \
	exNewParamInfo par; \
	par.structVersion = 1; \
	par.flags = _paramflags_; \
	par.paramType = exParamType_ratio; \
	par.paramValues = val; \
    strncpy_s(par.identifier, _identifier_, sizeof(_identifier_)); \
	suites.exportParamSuite->AddParam(pluginId, 0, _groupName_, &par); \
}

#define prCreateTimeParam(_identifier_, _groupName_, _paramflags_, _defaultValue_, _disabled_, _hidden_) { \
	exParamValues val; \
	val.structVersion = 1; \
	val.value.timeValue = _defaultValue_; \
	val.disabled = _disabled_; \
	val.hidden = _hidden_; \
	val.optionalParamEnabled = kPrFalse; \
	exNewParamInfo par; \
	par.structVersion = 1; \
	par.flags = _paramflags_; \
	par.paramType = exParamType_ticksFrameRate; \
	par.paramValues = val; \
    strncpy_s(par.identifier, _identifier_, sizeof(_identifier_)); \
	suites.exportParamSuite->AddParam(pluginId, 0, _groupName_, &par); \
}

#define prCreateButtonParam(_identifier_, _groupName_, _paramflags_) { \
	exParamValues val; \
	val.structVersion = 1; \
	val.disabled = kPrFalse; \
	val.hidden = kPrFalse; \
	val.optionalParamEnabled = kPrFalse; \
	exNewParamInfo par; \
	par.structVersion = 1; \
	par.flags = _paramflags_; \
	par.paramType = exParamType_button; \
	par.paramValues = val; \
    strncpy_s(par.identifier, _identifier_, sizeof(_identifier_)); \
	suites.exportParamSuite->AddParam(pluginId, 0, _groupName_, &par); \
}

#define prCreateParamGroup(_groupName_, _parentGroupName_, _name_) { \
    prUTF16Char name[128]; \
    copyWideStringIntoUTF16(_name_, name); \
	params->AddParamGroup(pluginId, 0, _parentGroupName_, _groupName_, name, kPrFalse, kPrFalse, kPrFalse); \
}

#define prSetNameDescription(_identifier_, _name_) { \
    prUTF16Char name[128]; \
    copyWideStringIntoUTF16(_name_, name); \
    suites.exportParamSuite->SetParamName(pluginId, 0, _identifier_, name); \
}

template <typename T>
struct Callback;

template <typename Ret, typename... Params>
struct Callback<Ret(Params...)>
{
	template <typename... Args>
	static Ret callback(Args... args)
	{
		return func(args...);
	}
	static std::function<Ret(Params...)> func;
};

template <typename Ret, typename... Params>
std::function<Ret(Params...)> Callback<Ret(Params...)>::func;

static inline void copyWideStringIntoUTF16(const wchar_t* inputString, prUTF16Char* destination)
{
	wcscpy_s(destination, wcslen(inputString) + 1, inputString);
}

class CPremierePluginApp
{
	struct FrameBuffer
	{
		FrameBuffer(size_t size = 0):
			bufferSize(size)
		{
			if (size > 0)
				for (int i = 0; i < sizeof(buffer) / sizeof(buffer[0]); i++)
					buffer[i] = (uint8_t*)malloc(size);
		}

		~FrameBuffer()
		{
			if (bufferSize > 0)
				for (int i = 0; i < sizeof(buffer) / sizeof(buffer[0]); i++)
					if (buffer[i])
						free(buffer[i]);
		}

		int track = 0;
		uint8_t* buffer[8] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
		int linesize[8] = { 0,0,0,0,0,0,0,0 };
		int64_t pts = 0;
		int timebaseNum = 0;
		int timebaseDen = 0;

	private:
		size_t bufferSize = 0;
	};

public:
	CPremierePluginApp(csSDK_uint32 pluginId, std::shared_ptr<VoukoderPro::IClient> vkdrpro);
	prMALError beginInstance(SPBasicSuite* spBasic, exExporterInstanceRec* instanceRecP);
	prMALError endInstance();
	prMALError generateDefaultParams(exGenerateDefaultParamRec* paramRecP);
	prMALError postProcessParams(exPostProcessParamsRec* paramRecP);
	prMALError validateParamChanged(exParamChangedRec* paramRecP);
	prMALError getParamSummary(exParamSummaryRec* paramRecP);
	prMALError StartExport(exDoExportRec* exportRecP);
	prMALError queryExportFileExtension(exQueryExportFileExtensionRec* exportFileExtensionRecP);
	prMALError queryOutputSettings(exQueryOutputSettingsRec* outputSettingsRecP);
	prMALError buttonAction(exParamButtonRec* paramButtonRecP);

public:
	std::shared_ptr<VoukoderPro::IClient> vkdrpro = nullptr;

private:
	PrSuites suites;
	csSDK_uint32 pluginId = 0;
	csSDK_uint32 sampleCount = 0;
	csSDK_int64 totalSampleCount = 0;
	csSDK_uint32 audioRendererId = 0;
	VersionInfo appVersion;
	std::shared_ptr<FrameBuffer> video = nullptr;
	std::shared_ptr<FrameBuffer> audio = nullptr;
	std::map<int, std::shared_ptr<VoukoderPro::SceneInfo>> m_scenes;

private:
	bool getPixelFormat(PrPixelFormat& format, VoukoderPro::configType& colorFormat);
	bool getSourceTimecode(std::string& timecode, PrTime timebase);
	const std::string getFilename(csSDK_uint32 fileObject);
	prSuiteError frameCompleteCallback(const csSDK_uint32 pass, const csSDK_uint32 inFrameNumber, const csSDK_uint32 inFrameRepeatCount, PPixHand inRenderedFrame, void* inCallbackData);
};

// Main entry function
extern "C" DllExport PREMPLUGENTRY xSDKExport(csSDK_int32 selector, exportStdParms * stdParms, void* param1, void* param2);