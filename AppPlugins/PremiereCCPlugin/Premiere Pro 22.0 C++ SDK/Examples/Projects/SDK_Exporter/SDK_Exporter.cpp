/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 1992-2008 Adobe Systems Incorporated                  */
/* All Rights Reserved.                                            */
/*                                                                 */
/* NOTICE:  All information contained herein is, and remains the   */
/* property of Adobe Systems Incorporated and its suppliers, if    */
/* any.  The intellectual and technical concepts contained         */
/* herein are proprietary to Adobe Systems Incorporated and its    */
/* suppliers and may be covered by U.S. and Foreign Patents,       */
/* patents in process, and are protected by trade secret or        */
/* copyright law.  Dissemination of this information or            */
/* reproduction of this material is strictly forbidden unless      */
/* prior written permission is obtained from Adobe Systems         */
/* Incorporated.                                                   */
/*                                                                 */
/*******************************************************************/
/*
	Revision History
		
	Version		Change										Engineer	Date
	=======		======										========	======
	1.0			created										eks			10/11/1999
	1.1			Added note on supporting multiple			bbb			6/14/2000
				audioRate in compGetAudioIndFormat
	1.2			Converted to C++, imposed coding guidelines	bbb			10/22/2001
				fixed a supervision logic bug or two...
	1.3			Updated for Adobe Premiere 6.5				bbb			5/21/2002
	1.4			Fixed work area export						zal			1/20/2003
	1.5			Updated for Adobe Premiere Pro 1.0			zal			2/28/2003
	1.6			Fixed row padding problem					zal			8/11/2003
	2.0			Added audio support for Premiere Pro,		zal			1/6/2004
				arbitrary audio sample rates,
				multi-channel audio, pixel aspect ratio,
				and fields; code cleanup
	2.5			Updated for Adobe Premiere Pro 2.0,			zal			3/10/2006
				code cleanup
	3.0			High-bit video support (v410)				zal			6/20/2006
	4.0			Ported to new export API					zal			3/3/2008
*/

#include "SDK_Exporter.h"
#include "SDK_Exporter_Params.h"

DllExport PREMPLUGENTRY xSDKExport (
	csSDK_int32		selector, 
	exportStdParms	*stdParmsP, 
	void			*param1, 
	void			*param2)
{
	prMALError result = exportReturn_Unsupported;
	
	switch (selector)
	{
		case exSelStartup:
			result = exSDKStartup(	stdParmsP, 
									reinterpret_cast<exExporterInfoRec*>(param1));
			break;

		case exSelBeginInstance:
			result = exSDKBeginInstance(stdParmsP,
										reinterpret_cast<exExporterInstanceRec*>(param1));
			break;

		case exSelEndInstance:
			result = exSDKEndInstance(	stdParmsP,
										reinterpret_cast<exExporterInstanceRec*>(param1));
			break;

		case exSelGenerateDefaultParams:
			result = exSDKGenerateDefaultParams(stdParmsP,
												reinterpret_cast<exGenerateDefaultParamRec*>(param1));
			break;

		case exSelPostProcessParams:
			result = exSDKPostProcessParams(stdParmsP,
											reinterpret_cast<exPostProcessParamsRec*>(param1));
			break;

		case exSelGetParamSummary:
			result = exSDKGetParamSummary(	stdParmsP,
											reinterpret_cast<exParamSummaryRec*>(param1));
			break;

		case exSelQueryOutputSettings:
			result = exSDKQueryOutputSettings(	stdParmsP,
												reinterpret_cast<exQueryOutputSettingsRec*>(param1));
			break;

		case exSelQueryExportFileExtension:
			result = exSDKFileExtension(stdParmsP,
										reinterpret_cast<exQueryExportFileExtensionRec*>(param1));
			break;

		case exSelParamButton:
			result = exSDKParamButton(	stdParmsP, 
										reinterpret_cast<exParamButtonRec*>(param1));
			break;

		case exSelValidateParamChanged:
			result = exSDKValidateParamChanged(	stdParmsP,
												reinterpret_cast<exParamChangedRec*>(param1));
			break;

		case exSelValidateOutputSettings:
			result = malNoError;
			break;

		case exSelExport:
			result = exSDKExport(	stdParmsP,
									reinterpret_cast<exDoExportRec*>(param1));
			break;
	}
	return result;
}


prMALError exSDKStartup (
	exportStdParms		*stdParmsP, 
	exExporterInfoRec	*infoRecP)
{
	prMALError result = malNoError;

	infoRecP->fileType			= SDK_FILETYPE;	// The filetype FCC (Four Character Code)
	copyConvertStringLiteralIntoUTF16(SDK_FILE_NAME, infoRecP->fileTypeName);
	// fileTypeDefaultExtension is used for extension when generating preview files
	copyConvertStringLiteralIntoUTF16(SDK_FILE_EXTENSION, infoRecP->fileTypeDefaultExtension);

	infoRecP->classID			= SDK_CLSS;		// Class ID of the MAL (media abstraction layer)
	infoRecP->exportReqIndex	= 0;			// Set this to exportReturn_IterateExporter to support multiple file types
	infoRecP->wantsNoProgressBar = kPrFalse;	// Let Premiere provide the progress bar
	infoRecP->hideInUI			= kPrFalse;
	infoRecP->doesNotSupportAudioOnly = kPrFalse; // Sure we support audio-only
	infoRecP->canConformToMatchParams = kPrTrue; 
	infoRecP->canExportVideo	= kPrTrue;		// Can compile Video, enables the Video checkbox in File > Export > Movie
	infoRecP->canExportAudio	= kPrTrue;		// Can compile Audio, enables the Audio checkbox in File > Export > Movie		

	// Tell Premiere which headers the exporter was compiled with
	infoRecP->interfaceVersion	= EXPORTMOD_VERSION;

	return result;
}



prMALError exSDKBeginInstance (
	exportStdParms			*stdParmsP, 
	exExporterInstanceRec	*instanceRecP)
{
	prMALError				result				= malNoError;
	SPErr					spError				= kSPNoError;
	ExportSettings			*mySettings;
	PrSDKMemoryManagerSuite	*memorySuite;
	csSDK_int32				exportSettingsSize	= sizeof(ExportSettings);
	SPBasicSuite			*spBasic			= stdParmsP->getSPBasicSuite();
	if (spBasic != NULL)
	{
		spError = spBasic->AcquireSuite(
			kPrSDKMemoryManagerSuite,
			kPrSDKMemoryManagerSuiteVersion,
			const_cast<const void**>(reinterpret_cast<void**>(&memorySuite)));
		mySettings = reinterpret_cast<ExportSettings *>(memorySuite->NewPtrClear(exportSettingsSize));

		if (mySettings)
		{
			mySettings->spBasic		= spBasic;
			mySettings->memorySuite	= memorySuite;
			spError = spBasic->AcquireSuite(
				kPrSDKExportParamSuite,
				kPrSDKExportParamSuiteVersion,
				const_cast<const void**>(reinterpret_cast<void**>(&(mySettings->exportParamSuite))));
			spError = spBasic->AcquireSuite (
				kPrSDKExportProgressSuite,
				kPrSDKExportProgressSuiteVersion,
				const_cast<const void**>(reinterpret_cast<void**>(&(mySettings->exportProgressSuite))));
			spError = spBasic->AcquireSuite(
				kPrSDKExportFileSuite,
				kPrSDKExportFileSuiteVersion,
				const_cast<const void**>(reinterpret_cast<void**>(&(mySettings->exportFileSuite))));
			spError = spBasic->AcquireSuite(
				kPrSDKExportInfoSuite,
				kPrSDKExportInfoSuiteVersion,
				const_cast<const void**>(reinterpret_cast<void**>(&(mySettings->exportInfoSuite))));
			spError = spBasic->AcquireSuite(
				kPrSDKErrorSuite,
				kPrSDKErrorSuiteVersion3,
				const_cast<const void**>(reinterpret_cast<void**>(&(mySettings->errorSuite))));
			spError = spBasic->AcquireSuite(
				kPrSDKClipRenderSuite,
				kPrSDKClipRenderSuiteVersion,
				const_cast<const void**>(reinterpret_cast<void**>(&(mySettings->clipRenderSuite))));
			spError = spBasic->AcquireSuite(
				kPrSDKMarkerSuite,
				kPrSDKMarkerSuiteVersion,
				const_cast<const void**>(reinterpret_cast<void**>(&(mySettings->markerSuite))));
			spError = spBasic->AcquireSuite(
				kPrSDKPPixSuite,
				kPrSDKPPixSuiteVersion,
				const_cast<const void**>(reinterpret_cast<void**>(&(mySettings->ppixSuite))));
			spError = spBasic->AcquireSuite(
				kPrSDKSequenceAudioSuite,
				kPrSDKSequenceAudioSuiteVersion1,
				const_cast<const void**>(reinterpret_cast<void**>(&(mySettings->sequenceAudioSuite))));
			spError = spBasic->AcquireSuite(
				kPrSDKSequenceRenderSuite,
				kPrSDKSequenceRenderSuiteVersion,
				const_cast<const void**>(reinterpret_cast<void**>(&(mySettings->sequenceRenderSuite))));
			spError = spBasic->AcquireSuite(
				kPrSDKTimeSuite,
				kPrSDKTimeSuiteVersion,
				const_cast<const void**>(reinterpret_cast<void**>(&(mySettings->timeSuite))));
			spError = spBasic->AcquireSuite(
				kPrSDKWindowSuite,
				kPrSDKWindowSuiteVersion,
				const_cast<const void**>(reinterpret_cast<void**>(&(mySettings->windowSuite))));
			spError = spBasic->AcquireSuite(
				kPrSDKApplicationSettingsSuite,
				kPrSDKApplicationSettingsSuiteVersion,
				const_cast<const void**>(reinterpret_cast<void**>(&(mySettings->appSettingsSuite))));
		}

		mySettings->SDKFileRec.width = 0;
		mySettings->SDKFileRec.height = 0;

		instanceRecP->privateData = reinterpret_cast<void*>(mySettings);
	}
	else
	{
		result = exportReturn_ErrMemory;
	}
	return result;
}


prMALError exSDKEndInstance (
	exportStdParms			*stdParmsP, 
	exExporterInstanceRec	*instanceRecP)
{
	prMALError				result		= malNoError;
	ExportSettings			*lRec		= reinterpret_cast<ExportSettings*>(instanceRecP->privateData);
	SPBasicSuite			*spBasic	= stdParmsP->getSPBasicSuite();
	PrSDKMemoryManagerSuite	*memorySuite;
	if(spBasic != NULL && lRec != NULL)
	{
		if (lRec->exportParamSuite)
		{
			result = spBasic->ReleaseSuite(kPrSDKExportParamSuite, kPrSDKExportParamSuiteVersion);
		}
		if (lRec->exportProgressSuite)
		{
			result = spBasic->ReleaseSuite(kPrSDKExportProgressSuite, kPrSDKExportProgressSuiteVersion);
		}
		if (lRec->exportFileSuite)
		{
			result = spBasic->ReleaseSuite(kPrSDKExportFileSuite, kPrSDKExportFileSuiteVersion);
		}
		if (lRec->exportInfoSuite)
		{
			result = spBasic->ReleaseSuite(kPrSDKExportInfoSuite, kPrSDKExportInfoSuiteVersion);
		}
		if (lRec->errorSuite)
		{
			result = spBasic->ReleaseSuite(kPrSDKErrorSuite, kPrSDKErrorSuiteVersion3);
		}
		if (lRec->clipRenderSuite)
		{
			result = spBasic->ReleaseSuite(kPrSDKClipRenderSuite, kPrSDKClipRenderSuiteVersion);
		}
		if (lRec->markerSuite)
		{
			result = spBasic->ReleaseSuite(kPrSDKMarkerSuite, kPrSDKMarkerSuiteVersion);
		}
		if (lRec->ppixSuite)
		{
			result = spBasic->ReleaseSuite(kPrSDKPPixSuite, kPrSDKPPixSuiteVersion);
		}
		if (lRec->sequenceAudioSuite)
		{
			result = spBasic->ReleaseSuite(kPrSDKSequenceAudioSuite, kPrSDKSequenceAudioSuiteVersion1);
		}
		if (lRec->sequenceRenderSuite)
		{
			result = spBasic->ReleaseSuite(kPrSDKSequenceRenderSuite, kPrSDKSequenceRenderSuiteVersion);
		}
		if (lRec->timeSuite)
		{
			result = spBasic->ReleaseSuite(kPrSDKTimeSuite, kPrSDKTimeSuiteVersion);
		}
		if (lRec->windowSuite)
		{
			result = spBasic->ReleaseSuite(kPrSDKWindowSuite, kPrSDKWindowSuiteVersion);
		}
		if (lRec->appSettingsSuite)
		{
			result = spBasic->ReleaseSuite(kPrSDKApplicationSettingsSuite, kPrSDKApplicationSettingsSuiteVersion);
		}
		if (lRec->memorySuite)
		{
			memorySuite = lRec->memorySuite;
			memorySuite->PrDisposePtr(reinterpret_cast<PrMemoryPtr>(lRec));
			result = spBasic->ReleaseSuite(kPrSDKMemoryManagerSuite, kPrSDKMemoryManagerSuiteVersion);
		}
	}

	return result;
}


// This selector is necessary so that the AME UI can provide a preview
// The bitrate value is used to provide the Estimated File Size
prMALError exSDKQueryOutputSettings(
	exportStdParms				*stdParmsP,
	exQueryOutputSettingsRec	*outputSettingsP)
{
	prMALError					result			= malNoError;
	csSDK_uint32				exID			= outputSettingsP->exporterPluginID;
	exParamValues				width,
								height,
								frameRate,
								pixelAspectRatio,
								fieldType,
								codec,
								sampleRate,
								channelType;
	ExportSettings				*privateData	= reinterpret_cast<ExportSettings*>(outputSettingsP->privateData);
	PrSDKExportParamSuite		*paramSuite		= privateData->exportParamSuite;
	csSDK_int32					mgroupIndex		= 0;
	float						fps				= 0.0f;
	
	if (outputSettingsP->inExportVideo)
	{
		paramSuite->GetParamValue(exID, mgroupIndex, ADBEVideoWidth, &width);
		outputSettingsP->outVideoWidth = width.value.intValue;
		paramSuite->GetParamValue(exID, mgroupIndex, ADBEVideoHeight, &height);
		outputSettingsP->outVideoHeight = height.value.intValue;
		paramSuite->GetParamValue(exID, mgroupIndex, ADBEVideoFPS, &frameRate);
		outputSettingsP->outVideoFrameRate = frameRate.value.timeValue;
		paramSuite->GetParamValue(exID, mgroupIndex, ADBEVideoAspect, &pixelAspectRatio);
		outputSettingsP->outVideoAspectNum = pixelAspectRatio.value.ratioValue.numerator;
		outputSettingsP->outVideoAspectDen = pixelAspectRatio.value.ratioValue.denominator;
		paramSuite->GetParamValue(exID, mgroupIndex, ADBEVideoFieldType, &fieldType);
		outputSettingsP->outVideoFieldType = fieldType.value.intValue;
	}
	if (outputSettingsP->inExportAudio)
	{
		paramSuite->GetParamValue(exID, mgroupIndex, ADBEAudioRatePerSecond, &sampleRate);
		outputSettingsP->outAudioSampleRate = sampleRate.value.floatValue;
		outputSettingsP->outAudioSampleType = kPrAudioSampleType_32BitFloat;
		paramSuite->GetParamValue(exID, mgroupIndex, ADBEAudioNumChannels, &channelType);
		outputSettingsP->outAudioChannelType = (PrAudioChannelType)channelType.value.intValue;
	}

	// Calculate bitrate
	PrTime			ticksPerSecond	= 0;
	csSDK_uint32	videoBitrate	= 0,
					audioBitrate	= 0;
	if (outputSettingsP->inExportVideo)
	{
		privateData->timeSuite->GetTicksPerSecond(&ticksPerSecond);
		fps = static_cast<float>(ticksPerSecond) / frameRate.value.timeValue;
		paramSuite->GetParamValue(exID, mgroupIndex, ADBEVideoCodec, &codec);
		videoBitrate = static_cast<csSDK_uint32>(width.value.intValue * height.value.intValue *
												GetPixelFormatSize(codec.value.intValue) * fps);
	}
	if (outputSettingsP->inExportAudio)
	{
		audioBitrate = static_cast<csSDK_uint32>(sampleRate.value.floatValue * 4 *
												GetNumberOfAudioChannels(outputSettingsP->outAudioChannelType));
	}
	outputSettingsP->outBitratePerSecond = videoBitrate + audioBitrate;

	// New in CS5 - return outBitratePerSecond in kbps
	outputSettingsP->outBitratePerSecond = outputSettingsP->outBitratePerSecond * 8 / 1000;

	return result;
}


// If an exporter supports various file extensions, it would specify which one to use here
prMALError exSDKFileExtension (
	exportStdParms					*stdParmsP, 
	exQueryExportFileExtensionRec	*exportFileExtensionRecP)
{
	prMALError		result	= malNoError;
	copyConvertStringLiteralIntoUTF16(SDK_FILE_EXTENSION, exportFileExtensionRecP->outFileExtension);
	return result;
}


prMALError RenderAndWriteAllVideo(
	exDoExportRec	*exportInfoP,
	float			progress,
	float			videoProgress,
	PrTime			*exportDuration)
{
	prMALError		result			= malNoError;
	csSDK_uint32	exID			= exportInfoP->exporterPluginID;
	ExportSettings	*mySettings		= reinterpret_cast<ExportSettings*>(exportInfoP->privateData);
	exParamValues	ticksPerFrame,
					width,
					height,
					pixelAspectRatio;

	mySettings->exportParamSuite->GetParamValue (exID, 0, ADBEVideoFPS, &ticksPerFrame);
	mySettings->sequenceRenderSuite->MakeVideoRenderer(	exID,
														&mySettings->videoRenderID,
														ticksPerFrame.value.timeValue);

	// The following code is in progress to test the new custom pixel format support
	mySettings->exportParamSuite->GetParamValue(exID, 0, ADBEVideoWidth, &width);
	mySettings->exportParamSuite->GetParamValue(exID, 0, ADBEVideoHeight, &height);
	mySettings->exportParamSuite->GetParamValue(exID, 0, ADBEVideoAspect, &pixelAspectRatio);
	mySettings->videoSequenceParser = new VideoSequenceParser(mySettings->spBasic,
										width.value.intValue,
										height.value.intValue,
										pixelAspectRatio.value.ratioValue.numerator,
										pixelAspectRatio.value.ratioValue.denominator);
	mySettings->videoSequenceParser->ParseSequence(exportInfoP->timelineData);

	// Since the endTime can fall in between frames, make sure to not include any fractional trailing frames
	for (	PrTime videoTime = exportInfoP->startTime;
			videoTime <= (exportInfoP->endTime - ticksPerFrame.value.timeValue);
			videoTime += ticksPerFrame.value.timeValue)
	{
		result = RenderAndWriteVideoFrame(videoTime, exportInfoP);

		progress = static_cast<float>(videoTime - exportInfoP->startTime) / static_cast<float>(*exportDuration) * videoProgress;
		result = mySettings->exportProgressSuite->UpdateProgressPercent(exID, progress);
		if (result == suiteError_ExporterSuspended)
		{
			mySettings->exportProgressSuite->WaitForResume(exID);
		}
		else if (result == exportReturn_Abort)
		{
			// Pass back the actual length exported so far
			// Since the endTime can fall in between frames, we go with the lower of the two values
			*exportDuration = videoTime + ticksPerFrame.value.timeValue - exportInfoP->startTime < *exportDuration ?
								videoTime + ticksPerFrame.value.timeValue - exportInfoP->startTime : *exportDuration;
			break;
		}
	}

	mySettings->sequenceRenderSuite->ReleaseVideoRenderer(exID, mySettings->videoRenderID);
	return result;
}


// Export markers and return warning
void HandleOptionalExportSetting(
	exportStdParms	*stdParmsP,
	exDoExportRec	*exportInfoP,
	ExportSettings	*mySettings,
	prMALError		*result)
{
	CodecSettings	codecSettings;
	csSDK_int32		codecSettingsSize	= static_cast<csSDK_int32>(sizeof(CodecSettings));

	mySettings->exportParamSuite->GetArbData(	exportInfoP->exporterPluginID,
												0,
												ADBEVideoCodecPrefsButton,
												&codecSettingsSize,
												NULL);
	if (codecSettingsSize)
	{
		// Settings valid.  Let's get them.
		mySettings->exportParamSuite->GetArbData(	exportInfoP->exporterPluginID,
													0,
													ADBEVideoCodecPrefsButton,
													&codecSettingsSize,
													reinterpret_cast<void*>(&codecSettings));
	}

	if (codecSettings.sampleSetting == kPrTrue)
	{
		// Example: here is the string value set in the UI, in stringValue.paramString
		exParamValues stringValue;
		mySettings->exportParamSuite->GetParamValue(exportInfoP->exporterPluginID, 0, ID_STRING, &stringValue);

		// Write another file which contains the marker data (if any)
		if (mySettings->markerSuite)
		{
			WriteMarkerAndProjectDataToFile(stdParmsP, exportInfoP);
		}

		// Fake a warning and a info
		prUTF16Char title[256],
					description[256];
		copyConvertStringLiteralIntoUTF16(WARNING_TITLE, title);
		copyConvertStringLiteralIntoUTF16(WARNING_DESC, description);
		mySettings->errorSuite->SetEventStringUnicode(PrSDKErrorSuite3::kEventTypeWarning, title, description);
		copyConvertStringLiteralIntoUTF16(INFO_TITLE, title);
		copyConvertStringLiteralIntoUTF16((const wchar_t *)stringValue.paramString, description);
		mySettings->errorSuite->SetEventStringUnicode(PrSDKErrorSuite3::kEventTypeInformational, title, description);
		if (*result == 0)
		{
			*result = exportReturn_ErrLastWarningSet;
		}
	}
}


// The main export function
prMALError exSDKExport(
	exportStdParms	*stdParmsP,
	exDoExportRec	*exportInfoP)
{
	prMALError					result					= malNoError;
	PrTime						exportDuration			= exportInfoP->endTime - exportInfoP->startTime;
	float						progress				= 0.0,
								videoProgress,
								audioProgress;
	ExportSettings				*mySettings				= reinterpret_cast<ExportSettings*>(exportInfoP->privateData);

	
	PrSDKString					projPath;
	
	prMALError test = mySettings->appSettingsSuite->GetCurrentProjectPath(&projPath);
	

	

	// Write the header to disk
	mySettings->exportFileSuite->Open(exportInfoP->fileObject);
	result = WriteSDK_FileHeader(stdParmsP, exportInfoP, exportDuration);

	// For progress meter, calculate how much video and audio should contribute to total progress
	if (exportInfoP->exportVideo && exportInfoP->exportAudio)
	{
		videoProgress = 0.9f;
		audioProgress = 0.1f;
	}
	else if (exportInfoP->exportVideo && !exportInfoP->exportAudio)
	{
		videoProgress = 1.0;
		audioProgress = 0.0;
	}
	else if (!exportInfoP->exportVideo && exportInfoP->exportAudio)
	{
		videoProgress = 0.0;
		audioProgress = 1.0;
	}

	if (exportInfoP->exportVideo && !result)			
	{
		result = RenderAndWriteAllVideo(exportInfoP, progress, videoProgress, &exportDuration);
	}
	// Even if user aborted export during video rendering, we'll just finish the audio to that point since it is really fast
	// and will make the export complete. How your exporter handles an abort, of course, is up to your implementation
	if (exportInfoP->exportAudio)
	{
		RenderAndWriteAllAudio(exportInfoP, exportDuration);
	}

	// If export was aborted midway by user, rewrite the header to reflect the shorter file
	if (result == exportReturn_Abort)
	{
		result = WriteSDK_FileHeader(stdParmsP, exportInfoP, exportDuration);
	}

	if (result != suiteError_CompilerCompileAbort)
	{
		HandleOptionalExportSetting(stdParmsP, exportInfoP, mySettings, &result);
	}

	mySettings->exportFileSuite->Close(exportInfoP->fileObject);

	return result;
}
