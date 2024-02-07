#pragma once

#include "Premiere Pro 22.0 C++ SDK/Examples/Headers/PrSDKMemoryManagerSuite.h"
#include "Premiere Pro 22.0 C++ SDK/Examples/Headers/PrSDKExportParamSuite.h"
#include "Premiere Pro 22.0 C++ SDK/Examples/Headers/PrSDKExportInfoSuite.h"
#include "Premiere Pro 22.0 C++ SDK/Examples/Headers/PrSDKExportFileSuite.h"
#include "Premiere Pro 22.0 C++ SDK/Examples/Headers/PrSDKPPixSuite.h"
#include "Premiere Pro 22.0 C++ SDK/Examples/Headers/PrSDKPPix2Suite.h"
#include "Premiere Pro 22.0 C++ SDK/Examples/Headers/PrSDKExporterUtilitySuite.h"
#include "Premiere Pro 22.0 C++ SDK/Examples/Headers/PrSDKSequenceAudioSuite.h"
#include "Premiere Pro 22.0 C++ SDK/Examples/Headers/PrSDKErrorSuite.h"

typedef struct PrSuites
{
	SPBasicSuite* basicSuite;
	PrSDKMemoryManagerSuite* memorySuite;
	PrSDKTimeSuite* timeSuite;
	PrSDKExportParamSuite* exportParamSuite;
	PrSDKExportInfoSuite* exportInfoSuite;
	PrSDKExportFileSuite* exportFileSuite;
	PrSDKPPixSuite* ppixSuite;
	PrSDKPPix2Suite* ppix2Suite;
	PrSDKExporterUtilitySuite* exporterUtilitySuite;
	PrSDKSequenceAudioSuite* sequenceAudioSuite;
	PrSDKAppInfoSuite* appInfoSuite;
	PrSDKErrorSuite* errorSuite;
} PrSuites;