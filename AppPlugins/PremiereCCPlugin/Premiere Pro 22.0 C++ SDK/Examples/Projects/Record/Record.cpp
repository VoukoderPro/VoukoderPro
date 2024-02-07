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
	Record.cpp

	Sample code for a record module. 
	Part of the .sdk file format Media Abstraction Layer.

	Notes									Engineer		Date
	================================================================
	Revised for Adobe Premiere 6.5 SDK		bbb				10/01/01
	Fixed crash after filename prompt		zal				10/21/02
	Major clean up, enable audio settings	zal				10/24/03
	Sample use of recorder settings			zal				03/18/04

*/

#include "Record.h"

static csSDK_int32 G_Timecode = 10000;

DllExport PREMPLUGENTRY RecEntry(
	csSDK_int32		selector, 
	rmStdParms		*stdParms, 
	void			*param1,
	void			*param2)
{
	prMALError	result = rmUnsupported;

	switch(selector)
	{
		case recmod_Startup8:
			result = recStartup8(	stdParms, 
									reinterpret_cast<recInfo8Ptr>(param1));
			break;

		case recmod_Open:
			result = recOpen(	stdParms, 
								reinterpret_cast<CapPrivH>(param1), 
								reinterpret_cast<recOpenParms*>(param2));
			break;

		case recmod_ShowOptions:
			result = recShowOptions(stdParms,
									reinterpret_cast<CapPrivP>(param1), 
									reinterpret_cast<recSetupParms*>(param2));
			break;

		case recmod_GetSetupInfo:
			result = recGetSetupInfo(	reinterpret_cast<CapPrivP>(param1), 
										reinterpret_cast<recCapSetups*>(param2));
			break;

		case recmod_Close:
			result = recClose(	stdParms,
								reinterpret_cast<CapPrivP>(param1));
			break;

		case recmod_PrepRecord8:
			result = recPrepRecord8(reinterpret_cast<CapPrivP>(param1), 
									reinterpret_cast<recCapParmsRec8*>(param2));
			break;

		case recmod_StartRecord:
			result = recStartRecord(reinterpret_cast<CapPrivP>(param1), 
									reinterpret_cast<recCapturedFileInfo*>(param2));
			break;

		case recmod_ServiceRecord:
			result = recServiceRecord(reinterpret_cast<CapPrivP>(param1));
			break;

		case recmod_StopRecord:
			result = recStopRecord(reinterpret_cast<CapPrivP>(param1));
			break;

		case recmod_Shutdown:
			result = recShutdown(stdParms);
			break;

		case recmod_CloseRecord:
			result = recCloseRecord(reinterpret_cast<CapPrivP>(param1));
			break;

		case recmod_SetActive:
			result = recSetActive(	reinterpret_cast<CapPrivP>(param1), 
									param2 != 0);
			break;

		case recmod_SetDisp:
			result = recSetDisp(reinterpret_cast<CapPrivP>(param1), 
								reinterpret_cast<recDisplayPos*>(param2));
			break;

		case recmod_Idle:
			result = recIdle(	stdParms,
								reinterpret_cast<CapPrivP>(param1), 
								reinterpret_cast<recGetTimecodeRec*>(param2));
			break;

		case recmod_QueryInfo:
			result = recQueryInfo(stdParms,
								reinterpret_cast<CapPrivP>(param1),
								reinterpret_cast<recCapInfoRec*>(param2));
			break;
	}
	return result;
}

static prMALError
recStartup8(
	rmStdParms		*stdparms, 
	recInfoRec8		*info8)
{
	prMALError result			=	rmIsCacheable;

	info8->requestedAPIVersion	=	RECMOD_VERSION;

	// fileType and classID are same as SDK_File_Compiler and SDK_File_Importer
	// For example, if you were writing a recorder that captured AVI files, fileType would be 'AVIV'
	info8->fileType				=	'SDK_';
	info8->classID 				=	'DTEK';

	info8->canVideoCap 			=	kPrTrue;
	info8->canAudioCap			=	kPrTrue;
	info8->audioOnlyFileType 	=	0;

	// Scene detection support new for Premiere Pro 1.0
	info8->canSearchScenes		=	kPrFalse;
	info8->canCaptureScenes		=	kPrFalse;

	// Step and still capture, and record limit not supported in Premiere Pro 1.0
	info8->canStepCap 			=	kPrFalse;
	info8->canStillCap 			=	kPrFalse;
	info8->canRecordLimit		=	kPrFalse;

	info8->canCountFrames		=	kPrFalse;		
	info8->canAbortDropped		=	kPrTrue;		
	info8->acceptsTimebase		=	kPrFalse;
	info8->acceptsBounds 		=	kPrFalse;
	info8->multipleFiles 		=	kPrFalse;
	info8->canSeparateVidAud	=	kPrFalse;

	// canPreview must be kPrTrue - we won't load it otherwise
	info8->canPreview 			=	kPrTrue;

	// wantsEvents not used in Premiere Pro 1.0
	info8->wantsEvents 			=	kPrFalse;

	info8->minWidth 			=	360;
	info8->minHeight 			=	240;
	info8->maxWidth 			=	1920;
	info8->maxHeight 			=	1080;
	info8->activeDuringSetup	=	kPrFalse;

	// prefPreviewWidth/Height not used in Premiere Pro 1.0
	info8->prefPreviewWidth 	=	360;
	info8->prefPreviewHeight 	=	240;

	// Aspect ratio used on startup; can be changed later using formatChangedFunc
	info8->prefAspect			=	648 << 16 | 720;

	info8->wantsMenuInactivate	=	kPrTrue;
	info8->canGetTimecode		=	kPrTrue;

	#ifdef PRWIN_ENV
	wcscpy_s (	info8->recmodName,
				sizeof (info8->recmodName) / sizeof (wchar_t),
				L"SDK Record");
	#else
	CFRange range = {0, 256};
	CFStringGetCharacters (CFSTR ("SDK Record"), range, info8->recmodName);
	#endif

	info8->audioOnlyFileType 	=	0;
	info8->canSearchScenes		=	kPrFalse;
	info8->canCaptureScenes		=	kPrFalse;
	memcpy (info8->outRecorderID.mGUID, "4be363d3-9dc2-4a95-a135-10f80d86d057", 37);

	// rmIsCacheable is not supported prior to Premiere Pro CS5.5 on MacOS
	#ifdef PRMAC_ENV
	if (stdparms->rmInterfaceVer < RECMOD_VERSION_11)
	{
		result = malNoError;
	}
	#endif

	return result;
}


static prMALError
recClose(
	rmStdParms	*stdParms,
	CapPriv		*capPtr)
{
	prMALError result = malNoError;

	// Close any open drivers here
	// Dispose of our private storage

	SPBasicSuite	*SPBasic = NULL;

	SPBasic = stdParms->piSuites->utilFuncs->getSPBasicSuite ();

	if (SPBasic && &(capPtr->errorSuite))
	{
		// Clean up
		SPBasic->ReleaseSuite (kPrSDKErrorSuite, kPrSDKErrorSuiteVersion3);
	}

	if (capPtr && (RECMOD_MAGIC == capPtr->magic))
	{
		stdParms->funcs->memoryFuncs->disposePtr(reinterpret_cast<PrMemoryPtr>(capPtr->setupP));
		stdParms->funcs->memoryFuncs->disposePtr(reinterpret_cast<PrMemoryPtr>(capPtr));
	}
	
	return result;
}


/*	Fill in the setups structure. You can have up to four settings dialogs. Here we tell Premiere
	how many we have and give it labels to put on the buttons which will appear in the capture 
	settings windows. 
*/

static prMALError
recGetSetupInfo(
	CapPriv			*capPtr, 
	recCapSetups	*setups)
{
	// In Premiere Pro 1.5, there is a bug that breaks using more than one setup dialog
	#ifdef PRWIN_ENV
	strcpy_s(setups->setups[0].name, sizeof (setups->setups[0].name), "Setup 1");
	strcpy_s(setups->setups[1].name, sizeof (setups->setups[1].name), "Setup 2");
	#else
	strcpy(setups->setups[0].name, "Setup 1");
	strcpy(setups->setups[1].name, "Setup 2");
	#endif

	// We want two setup buttons
	setups->customSetups = 2;
	// Lowest two bits enable first two buttons
	setups->enableflags = 0x3;

	return malNoError;
}


/*	Allocate storage for our private stuff and make sure our 'hardware' is present. */

static prMALError
recOpen(
	rmStdParms		*stdparms, 
	CapPriv			**cappriv, 
	recOpenParms	*parms)
{
	prMALError		result			= malNoError;
	CapPrivP		tempCapPrivP	= NULL;
	CapSetupP		tempSetupP		= NULL;
	SPBasicSuite	*SPBasic		= NULL;

	// Reset global timecode counter (demo purposes only)
	G_Timecode = 10000;

	// Initialize private instance data
	tempCapPrivP = (CapPrivP)stdparms->funcs->memoryFuncs->newPtr(sizeof(CapPriv));
	if (tempCapPrivP)
	{
		*cappriv = tempCapPrivP;

		tempCapPrivP->APIVersion = stdparms->rmInterfaceVer;

		tempCapPrivP->setupP = (CapSetupP)stdparms->funcs->memoryFuncs->newPtr(sizeof(CapSetup));
		if (!tempCapPrivP->setupP)
		{
			result = rmMemoryError;
		}

		tempCapPrivP->magic = RECMOD_MAGIC;
		tempCapPrivP->currentWidth = 720;
		tempCapPrivP->currentHeight = 480;
		tempCapPrivP->currentPAR = (12<<16) | 10;
	}

	// If private data allocation was successful
	if (!result)
	{
		tempSetupP = reinterpret_cast<CapSetupP>(parms->setup);
		// If setup data is present, copy it over.  Otherwise, use default values
		if (tempSetupP)
		{
			tempCapPrivP->setupP->setup1_yes = tempSetupP->setup1_yes;
			tempCapPrivP->setupP->setup1_no = tempSetupP->setup1_no;
			tempCapPrivP->setupP->setup2_yes = tempSetupP->setup2_yes;
			tempCapPrivP->setupP->setup2_no = tempSetupP->setup2_no;
		}
		else
		{
			tempCapPrivP->setupP->setup1_yes = 0;
			tempCapPrivP->setupP->setup1_no = 0;
			tempCapPrivP->setupP->setup2_yes = 0;
			tempCapPrivP->setupP->setup2_no = 0;
		}

		tempCapPrivP->capWnd = parms->disp.wind;
		tempCapPrivP->callbackID = parms->callbackID;
		tempCapPrivP->formatChangedFunc = parms->formatFunc;

	TDB_TimeRecord inFramerate;
	// inFramerate.value is ignored by formatChangedFunc
	inFramerate.scale = 2997;
	inFramerate.sampleSize = 100;

	parms->formatFunc (parms->callbackID, 120, 100, 720, 480, inFramerate, 0);

		SPBasic = stdparms->piSuites->utilFuncs->getSPBasicSuite ();
		if (SPBasic)
		{
			SPBasic->AcquireSuite (kPrSDKErrorSuite, kPrSDKErrorSuiteVersion3, (const void**)&(tempCapPrivP->errorSuite));
		}

		// Explicitly call formatChangedFunc, unless you want initial settings to be project settings
		// These happen to be NTSC settings: .9 pixel aspect ratio, 720x480 resolution, 29.97 drop frame
		// tempCapPrivP->formatChangedFunc (tempCapPrivP->callbackID, 90, 100, 720, 480, inFramerate, 1);

		// You should now open and connect to your audio and video devices, storing any
		// private settings data in the CapPriv structure
	}
	else
	{
		result = rmMemoryError;
	}
	return result;
}


static prMALError 
recShutdown (
	rmStdParms		*stdparms)
{
	return malNoError;
}


//--------------------------------------------
// Show a capture options dialog.  The dialog is opened by the user from the
// Capture Settings.  You can put up a custom dialog here in which the user can
// specify any parameters.  You can have up to four settings dialogs.
// The dialog requested is indicated by setupnum in the recSetupParms structure.
// In this example, we have two dialogs where the user can choose 'Yes', 'No', or 'Cancel'.
// Each time 'Yes' or 'No' is hit, the corresponding value is incremented.
// These values are displayed in the dialog, and saved a single CapSetup structure,
// which is automatically saved by Premiere in the project after the Project Settings dialog
// is accepted, and passed with each subsequent recmod_Open in recOpenParms->setup

static prMALError recShowOptions (
	rmStdParms		*stdParms,
	CapPrivP		capPriv,
	recSetupParms	*setupParms)
{
	prMALError	result		= malNoError;
	char 		str[256]	= "Sample recorder options\n";
	char		str2[256];
	CapSetupP	tempSetupP	= 0;
	
	#ifdef PRWIN_ENV
	int			returnValue = 0;
	#else
	/*
	CFStringRef				sampleSetup1		= CFSTR (SAMPLE_SETUP1),
							sampleSetup2		= CFSTR (SAMPLE_SETUP2),
							yes					= CFSTR ("Yes"),
							no					= CFSTR ("No");
	DialogRef				alert;
	DialogItemIndex			outItemHit;
	AlertStdCFStringAlertParamRec inAlertParamSetup1 = {	kStdCFStringAlertVersionOne,
															kPrTrue,
															kPrFalse,
															yes,
															no,
															(const CFStringRef)kAlertDefaultCancelText,
															kAlertStdAlertOKButton,
															kAlertStdAlertOtherButton,
															kWindowDefaultPosition,
															NULL};	
	AlertStdCFStringAlertParamRec inAlertParamSetup2 = {	kStdCFStringAlertVersionOne,
															kPrTrue,
															kPrFalse,
															yes,
															no,
															(const CFStringRef)kAlertDefaultCancelText,
															kAlertStdAlertOKButton,
															kAlertStdAlertOtherButton,
															kWindowDefaultPosition,
															NULL};
	 */
	
	#endif

	// If this is the first time showing options,
	// allocate memory to save settings in and initialize default values.
	// These settings will persist with the project.
	if (setupParms->setup == NULL)
	{
		setupParms->setup = stdParms->piSuites->memFuncs->newPtr (sizeof (CapSetup));
		tempSetupP = reinterpret_cast<CapSetupP>(setupParms->setup);
		tempSetupP->setup1_yes = 0;
		tempSetupP->setup1_no = 0;
		tempSetupP->setup2_yes = 0;
		tempSetupP->setup2_no = 0;
	}

	tempSetupP = reinterpret_cast<CapSetupP>(setupParms->setup);

	// If user hit first button
	if (setupParms->setupnum == 0)
	{
		#ifdef PRWIN_ENV
		sprintf_s (str2, sizeof (str2), "%d yes, %d no", tempSetupP->setup1_yes, tempSetupP->setup1_no);
		strcat_s(str, sizeof (str), str2);
		returnValue = MessageBox(	reinterpret_cast<HWND>(setupParms->parentwind),
									str, 
									SAMPLE_SETUP1, 
									MB_YESNOCANCEL);
		if (returnValue == IDYES)
		{
			tempSetupP->setup1_yes++;
		}
		else if (returnValue == IDNO)
		{
			tempSetupP->setup1_no++;
		}
		
		#else
		sprintf (str2, "%d yes, %d no", tempSetupP->setup1_yes, tempSetupP->setup1_no);
		strcat(str, str2);
/*		CFStringRef	setupStr = CFStringCreateWithCString (kCFAllocatorDefault, str, kCFStringEncodingMacRoman);
		[TODO] Will need to use Cocoa with NSAlertPanel
		returnValue = CreateStandardAlert (	kAlertNoteAlert,
											sampleSetup1,
											setupStr,
											&inAlertParamSetup1,
											&alert);
		returnValue = RunStandardAlert (alert, NULL, &outItemHit);
		CFRelease (setupStr);
		if (outItemHit == kAlertStdAlertOKButton)
		{
			tempSetupP->setup1_yes++;
		}
		else if (outItemHit == kAlertStdAlertCancelButton)
		{
			tempSetupP->setup1_no++;
		}
*/		#endif
	}

	// If user hit second button
	else
	{
		#ifdef PRWIN_ENV
		sprintf_s (str2, sizeof (str2), "%d yes, %d no", tempSetupP->setup2_yes, tempSetupP->setup2_no);
		strcat_s(str, sizeof (str), str2);
		returnValue = MessageBox(	reinterpret_cast<HWND>(setupParms->parentwind),
									str, 
									SAMPLE_SETUP2, 
									MB_YESNOCANCEL);
		if (returnValue == IDYES)
		{
			tempSetupP->setup2_yes++;
		}
		else if (returnValue == IDNO)
		{
			tempSetupP->setup2_no++;
		}
		
		#else
		sprintf (str2, "%d yes, %d no", tempSetupP->setup2_yes, tempSetupP->setup2_no);
		strcat(str, str2);
/*		CFStringRef	setupStr = CFStringCreateWithCString (kCFAllocatorDefault, str, kCFStringEncodingMacRoman);
		[TODO] Will need to use Cocoa with NSAlertPanel
		returnValue = CreateStandardAlert (	kAlertNoteAlert,
											sampleSetup2,
											setupStr,
											&inAlertParamSetup2,
											&alert);
		returnValue = RunStandardAlert (alert, NULL, &outItemHit);
		CFRelease (setupStr);
		if (outItemHit == kAlertStdAlertOKButton)
		{
			tempSetupP->setup2_yes++;
		}
		else if (outItemHit == kAlertStdAlertCancelButton)
		{
			tempSetupP->setup2_no++;
		}
*/		#endif
	}
	
	/*
	#ifdef PRMAC_ENV
	CFRelease (sampleSetup1);
	CFRelease (sampleSetup2);
	#endif
	*/

	return result;
}


/*	Do everything you need prior to capturing here. 
	The next selector will be StartRecord, and 
	you'll need to be ready to start recording right away.
*/


static prMALError			  
recPrepRecord8 (
	CapPrivP		capPtr, 
	recCapParmsRec8	*capRec8)
{
	prMALError result = malNoError;
	#ifdef PRMAC_ENV
	OSErr			returnValue;
	CFStringRef		sourceFileNameCFSR = CFSTR("premiere.sdk");
	CFRange			range = {0, 255};
	CFURLRef		destFolderURL;
	FSRef			sourceFolderRef,
					sourceFileRef,
					destFolderRef,
					destFileRef;
	UniChar			sourceFileNameUC[255];
	CFStringRef		destFileCFSR,
					destFolderCFSR,
					destFileNameCFSR;
	#endif

	// [TODO] synthesize media to disk
	// For now we will pretend to capture c:\premiere.sdk, so make sure to generate a file using the SDK exporter and put it there
	#ifdef PRWIN_ENV
	wchar_t cheater[255];
	wcscpy_s (cheater, sizeof (cheater) / sizeof (wchar_t),  L"c:\\premiere.sdk");

	if (0 == CopyFileW(cheater, capRec8->thefile.name, FALSE))
	{
		result = malUnknownError;
	}
	#else
	returnValue = FSFindFolder(kOnSystemDisk, kDesktopFolderType, kDontCreateFolder, &sourceFolderRef);
	range.length = CFStringGetLength(sourceFileNameCFSR);
	CFStringGetCharacters(sourceFileNameCFSR, range, sourceFileNameUC);
	FSMakeFSRefUnicode(	&sourceFolderRef,
						CFStringGetLength(sourceFileNameCFSR),
						sourceFileNameUC,
						kTextEncodingUnknown,
						&sourceFileRef);
	
	// Get separate CFStringRefs for the file path and file name
	destFileCFSR = CFStringCreateWithCharacters(kCFAllocatorDefault, capRec8->thefile.name, prUTF16CharLength(capRec8->thefile.name));
	range = CFStringFind(destFileCFSR, CFSTR("/"), kCFCompareBackwards);
	range.length = range.location;
	range.location = 0;
	destFolderCFSR = CFStringCreateWithSubstring(kCFAllocatorDefault, destFileCFSR, range);
	range.location = range.length + 1;
	range.length = CFStringGetLength(destFileCFSR) - range.location;
	destFileNameCFSR = CFStringCreateWithSubstring(kCFAllocatorDefault, destFileCFSR, range);
	
	destFolderURL = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, destFolderCFSR, kCFURLPOSIXPathStyle, true);
	returnValue = CFURLGetFSRef (destFolderURL, &destFolderRef);
	
	FSCopyObjectSync (&sourceFileRef, &destFolderRef, destFileNameCFSR, &destFileRef, kFSFileOperationDefaultOptions);
	#endif

	// Save the capture flags
	capPtr->capVideo			= capRec8->capVideo;
	capPtr->capAudio			= capRec8->capAudio;
	
	capPtr->prerollFunc 		= capRec8->prerollFunc;
	capPtr->statusFunc			= capRec8->statFunc;
	capPtr->currentFrame		= capRec8->timeCode;
	capPtr->framesToCapture		= capRec8->frameCount;
	capPtr->framesCaptured		= 0;

	// Prepare to capture!
	if(!result && capPtr->capAudio)
	{
		// Prep the audio hardware
	}
	if (!result && capPtr->capVideo)
	{
		// Prep the video hardware
	}
	return result;
}


static prMALError
audStartRecord(
	CapPriv		*capPtr)
{
	return malNoError;
}


static prMALError
vidStartRecord(
	CapPriv		*capPtr)
{
	return malNoError;
}


static prMALError
recStartRecord(
	CapPriv				*capPtr, 
	recCapturedFileInfo	*fileInfo)
{
	prMALError result = malNoError;

	if(capPtr->prerollFunc)
	{
		if(capPtr->prerollFunc(capPtr->callbackID))
		{
			result = rmPrerollAbort;
		}
	}
	if(!result && capPtr->capVideo)
	{
		result = vidStartRecord(capPtr);
	}
	if (!result && capPtr->capAudio)
	{
		result = audStartRecord(capPtr);
	}
	fileInfo->pixelAspectRatioNum = 1;
	fileInfo->pixelAspectRatioDen = 1;

	// The Premiere Pro 1.0 API requires a valid timecode for the clip to be added to
	// the project after capture, so we will use time zero.
	// This is fixed in 1.5
	if (capPtr->APIVersion == RECMOD_VERSION_4)
	{
		#ifdef PRWIN_ENV
		strcpy_s (fileInfo->timeCode, sizeof (fileInfo->timeCode), "00;00;00;00");
		#endif
	}

	return result;
}


static prMALError
audServiceRecord(
	CapPriv		*capPtr)
{
	return malNoError;
}


static prMALError
vidServiceRecord(
	CapPriv		*capPtr)
{
	return malNoError;
}


static prMALError
recServiceRecord(
	CapPriv		*capPtr)
{
	prMALError result = malNoError;
	if (capPtr->capAudio)
	{
		result = audServiceRecord(capPtr);
	}

	if (!result && capPtr->capVideo)
	{
		result = vidServiceRecord(capPtr);
	}

	if (!result)
	{
		// If this callback ever comes back non-zero, you should abort the capture
		result = (capPtr->statusFunc)(capPtr->callbackID, const_cast<char*>("Pretending to capture"), capPtr->framesCaptured++);
	}

	if (capPtr->framesCaptured == capPtr->framesToCapture)
	{
		result = rmStatusCaptureDone;
	}

	return result;
}


static prMALError
audStopRecord(
	CapPriv		*capPtr)
{
	return malNoError;
}


static prMALError
vidStopRecord(
	CapPriv		*capPtr)
{
	return malNoError;
}


static prMALError
recStopRecord(
	CapPriv		*capPtr)
{
	prMALError	auderr	= malNoError, 
				viderr	= malNoError, 
				result	= malNoError;

	if (capPtr->capAudio)
	{
		auderr = audStopRecord(capPtr);
	}
	if (capPtr->capVideo)
	{
		viderr = vidStopRecord(capPtr);
	}
	if (viderr) 
	{
		result = viderr;
	}
	if (auderr && !viderr) 
	{
		result = auderr;
	}

	// Blank the status display area
	if (!result)
	{
		result = (capPtr->statusFunc)(capPtr->callbackID, NULL, 0);

		// If you want backwards-compatibility with 1.0, you would use this call instead
		// result = (capPtr->statusFunc)(capPtr->callbackID, "                                       ", 0);
	}

	return result;
}


static prMALError
audCloseRecord(
	CapPriv		*capPtr)
{
	return malNoError;
}


static prMALError
vidCloseRecord(
	CapPriv		*capPtr)
{
	return malNoError;
}


static prMALError
recCloseRecord(
	CapPriv		*capPtr)
{
	prMALError result = malNoError;

	if (capPtr->capAudio)
	{
		result = audCloseRecord(capPtr);
	}
	if(capPtr->capVideo) 
	{
		result = vidCloseRecord(capPtr);
	}

	return result;
}


// Toggle activation state
static prMALError
recSetActive(
	CapPriv	*capPtr, 
	prBool	param)
{
	return malNoError;
}


// Update the size/position of the capture window
static prMALError
recSetDisp(
	CapPriv			*capPtr, 
	recDisplayPos	*disp)
{
	capPtr->disp = *disp;
	return malNoError;
}


static prMALError
recIdle(
	rmStdParms			*stdParms,
	CapPriv				*capPtr, 
	recGetTimecodeRec	*timecode)
{
	prMALError	result		=	malNoError;
	csSDK_int32	scale		=	0,
				sampleSize	=	0;

	// Until we provide some UI for the user to choose the capture frame rate,
	// just hardcode to 29.97 for now
	scale = 2997;
	sampleSize = 100;

	// Demonstration: when the counter reaches 10200 frames, change the pixel aspect ratio to 1.2,
	// the frame size to 720x480, and the time display format to 29.97 non-drop frame
	if (G_Timecode == 10200)
	{
		TDB_TimeRecord inFramerate;
		// inFramerate.value is ignored by formatChangedFunc
		inFramerate.scale = 2997;
		inFramerate.sampleSize = 100;

		capPtr->formatChangedFunc (capPtr->callbackID, 120, 100, 720, 480, inFramerate, 0);
	}

	// Special-case 29.97 and 23.976
	if (scale && sampleSize && (29.0 < (float) scale / sampleSize) && ((float) scale / sampleSize) < 30.0)
	{
		timecode->currate		= 30;
		timecode->timeFormat	= 1;

	}
	else if (scale && sampleSize && (23.0 < (float) scale / sampleSize) && ((float) scale / sampleSize) < 24.0)
	{
		timecode->currate		= 24;
		timecode->timeFormat	= 1;
	}
	else
	{
		timecode->currate		= static_cast<short>(scale);
		timecode->timeFormat	= 0;
	}

	timecode->status = 0;

	// [TODO] Get rid of static variable
	timecode->timeCode		=	G_Timecode++;

	return result;
}


static prMALError 
recQueryInfo(
	rmStdParms			*stdParms,
	CapPriv				*capPtr, 
	recCapInfoRec		*capInfo)
{
	prMALError result = malNoError;

	// These are needed for logged clips to have the correct width and height.
	// Otherwise, the maxWidth/maxHeight will be used, and sample rate will be 32000.
	capInfo->width = capPtr->currentWidth;
	capInfo->height = capPtr->currentHeight;
	capInfo->pixelAspectRatio = capPtr->currentPAR;
	capInfo->audRate = 48000;

	return result;
}