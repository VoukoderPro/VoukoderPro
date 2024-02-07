/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2010 Adobe Systems Incorporated					   */
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

#include "ExportController.h"

static PrSDKExportControllerSuite	*sExportControllerSuitePtr = NULL;
static PrSDKErrorSuite3				*sErrorSuitePtr = NULL;
static PrSDKSequenceInfoSuite		*sSequenceInfoSuitePtr = NULL;
static PrSDKTimeSuite				*sTimeSuitePtr = NULL;

DllExport PREMPLUGENTRY xExportControllerEntry (
	int				selector, 
	ecStdParms		*stdParms, 
	void			*param1)
{
	prMALError result = exportReturn_Unsupported;
	
	switch (selector)
	{
		case exportController_Startup:
			result = ecStartup(stdParms, reinterpret_cast<ExportControllerInitRec*>(param1));
			break;

		case exportController_ExportTimeline:
			result = ecExportTimeline(stdParms, reinterpret_cast<ExportControllerTimelineInfoRec*>(param1));
			break;

		case exportController_Shutdown:
			result = ecShutdown(stdParms);

	}
	return result;
}

prMALError ecStartup(
	ecStdParms *stdParms, 
	ExportControllerInitRec	*initRec)
{
	prMALError result = malNoError;

	std::wstring pluginName(PLUGIN_DISPLAY_NAME);
	std::copy(pluginName.begin(), pluginName.end(), initRec->mDisplayName);
	
	SPErr spError = kSPNoError;
	SPBasicSuite *spBasic = stdParms->piSuites->utilFuncs->getSPBasicSuite();
	if (spBasic)
	{
		spError = spBasic->AcquireSuite(
			kPrSDKExportControllerSuite,
			kPrSDKExportControllerSuiteVersion,
			const_cast<const void**>(reinterpret_cast<void**>(&sExportControllerSuitePtr)));

		spError = spBasic->AcquireSuite(
			kPrSDKErrorSuite,
			kPrSDKErrorSuiteVersion3,
			const_cast<const void**>(reinterpret_cast<void**>(&sErrorSuitePtr)));

		spError = spBasic->AcquireSuite(
			kPrSDKSequenceInfoSuite,
			kPrSDKSequenceInfoSuiteVersion,
			const_cast<const void**>(reinterpret_cast<void**>(&sSequenceInfoSuitePtr)));

		spError = spBasic->AcquireSuite(
			kPrSDKTimeSuite,
			kPrSDKTimeSuiteVersion,
			const_cast<const void**>(reinterpret_cast<void**>(&sTimeSuitePtr)));
	}

	return result;
}

prMALError ecExportTimeline(
	ecStdParms *stdParms,
	ExportControllerTimelineInfoRec *timelineInfo)
{
	prMALError result = malNoError;
	prRect			frameRect;
	csSDK_uint32	PARNumerator, PARDenominator;
	PrTime			ticksPerFrame;
	csSDK_int32		scale, sampleSize;

	// Callback example:  procuring the MOS ID using the timelineID
	prMosIDString mosID;
	if (sExportControllerSuitePtr)
	{
		sExportControllerSuitePtr->RetrieveMosID(timelineInfo->mModuleID, timelineInfo->mTimelineID, &mosID);
	}

	// Get info on the current sequence. This can be used to more intelligently decide which presets to show or use as a default
	if (sSequenceInfoSuitePtr)
	{
		sSequenceInfoSuitePtr->GetFrameRect(timelineInfo->mTimelineID, &frameRect);
		sSequenceInfoSuitePtr->GetPixelAspectRatio(timelineInfo->mTimelineID, &PARNumerator, &PARDenominator);
		sSequenceInfoSuitePtr->GetFrameRate(timelineInfo->mTimelineID, &ticksPerFrame);
		ConvertPrTimeToScaleSampleSize(sTimeSuitePtr, ticksPerFrame, &scale, &sampleSize);
	}

	// Do your UI (if required)
	#ifdef PRWIN_ENV
	MessageBox((HWND)stdParms->piSuites->windFuncs->getMainWnd(),
				"Display your UI here.",
				"SDK Export Controller",
				MB_OK);
	#endif

	// Callback example: trigger a render in Premiere

	if (sExportControllerSuitePtr)
	{
		prUTF16Char presetPath[255], destinationPath[255];
		ExportControllerExportRec ecExportRec = {destinationPath, presetPath};
		
		// Initialize preset and destination path parameters
		copyConvertStringLiteralIntoUTF16(PRESET_PATH, const_cast<prUTF16Char*>(ecExportRec.mPresetPath));
		copyConvertStringLiteralIntoUTF16(OUTPUT_FILENAME, const_cast<prUTF16Char*>(ecExportRec.mDestinationPath));
		result = setDestinationPathToDesktop(const_cast<prUTF16Char*>(ecExportRec.mDestinationPath));

		// Confirm there is actually a preset at the path
		bool	doExport = fileExists(ecExportRec.mPresetPath);
		
		if (!result && doExport)
		{
			result = sExportControllerSuitePtr->ExportFile(timelineInfo->mModuleID, timelineInfo->mTimelineID, &ecExportRec);
		}
		else
		{
			std::wstring errorTitle(ERROR_TITLE);
			std::wstring errorDesc(ERROR_PRESET_DESC);
			sErrorSuitePtr->SetEventStringUnicode(	PrSDKErrorSuite3::kEventTypeError,
													reinterpret_cast<prUTF16Char*>(const_cast<wchar_t*>(errorTitle.c_str())),
													reinterpret_cast<prUTF16Char*>(const_cast<wchar_t*>(errorDesc.c_str())));
			result = eExportControllerErrorReportedInSuite;
		}
	}

	return result;
}

prMALError ecShutdown(ecStdParms *stdParms)
{
	SPBasicSuite *spBasic = stdParms->piSuites->utilFuncs->getSPBasicSuite();
	if (spBasic)
	{
		if (sExportControllerSuitePtr)
		{
			spBasic->ReleaseSuite(kPrSDKExportControllerSuite, kPrSDKExportControllerSuiteVersion);
			sExportControllerSuitePtr = NULL;
		}
		if (sErrorSuitePtr)
		{
			spBasic->ReleaseSuite(kPrSDKErrorSuite, kPrSDKErrorSuiteVersion3);
			sErrorSuitePtr = NULL;
		}
		if (sSequenceInfoSuitePtr)
		{
			spBasic->ReleaseSuite(kPrSDKSequenceInfoSuite, kPrSDKSequenceInfoSuiteVersion);
			sSequenceInfoSuitePtr = NULL;
		}
		if (sTimeSuitePtr)
		{
			spBasic->ReleaseSuite(kPrSDKTimeSuite, kPrSDKTimeSuiteVersion);
			sTimeSuitePtr = NULL;
		}
	}

	return malNoError;
}

bool fileExists(const prUTF16Char *filePath)
{
	bool returnValue;
	
#ifdef PRWIN_ENV
	
	std::wstring fileName(filePath);
	std::fstream fin;
	fin.open(fileName.c_str(),std::ios::in);
	if( fin.is_open() )
	{
		fin.close();
		returnValue = true;
	}
	else
	{
		returnValue = false;
	}
	
#elif defined PRMAC_ENV
	
	CFStringRef filePathCFSR = CFStringCreateWithCharacters(kCFAllocatorDefault,
													  filePath,
													  prUTF16CharLength(filePath));
	CFURLRef filePathURL = CFURLCreateWithFileSystemPath(kCFAllocatorDefault,
														 filePathCFSR,
														 kCFURLPOSIXPathStyle,
														 false);
	if (filePathURL)
	{
		CFErrorRef error;
		returnValue = CFURLResourceIsReachable(filePathURL, &error);
		CFRelease(filePathURL);
	}
	CFRelease(filePathCFSR);
	
#endif
	
	return returnValue;
}

#ifdef PRWIN_ENV

int setDestinationPathToDesktop(prUTF16Char *destinationPath)
{
	int					result = 0;
	HRESULT				hresult = 0;
	PWSTR				desktopPath;
	prUTF16Char			tempFilename[255];
	
	// Copy the filename to a temp buffer
	prUTF16CharCopy (tempFilename, destinationPath);

	hresult = SHGetKnownFolderPath(FOLDERID_Desktop, 0, NULL, &desktopPath);
	prUTF16CharCopy(destinationPath, desktopPath);

	int length = prUTF16CharLength (destinationPath);
	
	// Check for buffer overrun
	if (length + prUTF16CharLength (tempFilename) < 255)
	{
		prUTF16CharCopy (destinationPath + length, tempFilename);
	}
	else
	{
		result = -1;
	}

	if (hresult == S_OK)
	{
		CoTaskMemFree(desktopPath);
	}
	
	return result;
}

#elif defined PRMAC_ENV

int setDestinationPathToDesktop(prUTF16Char *destinationPath)
{
	int					result = 0;
	CFStringRef			filepathCFSR;
	CFRange				range = {0, kPrMaxPath};
	FSRef				folderRef;
	CFURLRef			folderURL;
	prUTF16Char			tempFilename[255];
	
	// Copy the filename to a temp buffer
	prUTF16CharCopy (tempFilename, destinationPath);

	result = FSFindFolder(kOnSystemDisk, kDesktopFolderType, kDontCreateFolder, &folderRef);
	folderURL = CFURLCreateFromFSRef (NULL, &folderRef);
	filepathCFSR = CFURLCopyFileSystemPath (folderURL, kCFURLPOSIXPathStyle);
	range.length = CFStringGetLength (filepathCFSR) + 1;	// Include terminating NULL
	
	// Get UTF-16 data for Desktop path
	CFStringGetCharacters (	filepathCFSR,
						   range,
						   reinterpret_cast<UniChar *>(destinationPath));
	int length = prUTF16CharLength (destinationPath);
	
	// Check for buffer overrun
	if (length + prUTF16CharLength (tempFilename) < 255)
	{
		prUTF16CharCopy (destinationPath + length, tempFilename);
	}
	else
	{
		result = -1;
	}
	CFRelease (filepathCFSR);
	
	return result;
}

#endif