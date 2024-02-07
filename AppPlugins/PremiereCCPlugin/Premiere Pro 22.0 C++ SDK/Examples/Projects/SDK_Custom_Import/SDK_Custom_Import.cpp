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

#include	"SDK_Custom_Import.h"

PREMPLUGENTRY 
DllExport 
xImportEntry (
	csSDK_int32			selector, 
	imStdParms			*stdParms, 
	void				*param1, 
	void				*param2)
{
	prMALError result = imUnsupported;
	switch (selector)
	{
		case imInit:
			result =	SDKInit	(stdParms, reinterpret_cast<imImportInfoRec *>(param1));
			break;

		case imGetIndFormat:
			result =	SDKGetIndFormat	(stdParms, reinterpret_cast<csSDK_size_t>(param1), reinterpret_cast<imIndFormatRec *>(param2));
			break;

		case imGetPrefs8:
			result =	SDKGetPrefs8 (stdParms, reinterpret_cast<imFileAccessRec8 *>(param1), reinterpret_cast<imGetPrefsRec *>(param2));
			break;

		case imGetInfo8:
			result =	SDKGetInfo8	(stdParms, reinterpret_cast<imFileAccessRec8 *>(param1), reinterpret_cast<imFileInfoRec8 *>(param2));
			break;		

		case imOpenFile8:
			result =	SDKOpenFile8 (stdParms, reinterpret_cast<imFileRef*>(param1), reinterpret_cast<imFileOpenRec8*>(param2));
			break;

		case imQuietFile:
			result =	SDKQuietFile (stdParms, reinterpret_cast<imFileRef*>(param1), param2); 
			break;

		case imCloseFile:
			result =	SDKCloseFile (stdParms, reinterpret_cast<imFileRef*>(param1), param2);
			break;

		case imImportImage:
			result =	SDKImportImage	(stdParms, reinterpret_cast<imFileRef>(param1), reinterpret_cast<imImportImageRec *>(param2));
			break;

		case imImportAudio7:
			result =	SDKImportAudio7	(stdParms, reinterpret_cast<imFileRef>(param1), reinterpret_cast<imImportAudioRec7 *>(param2));
			break;

		case imGetIndPixelFormat:
			result =	SDKGetIndPixelFormat(stdParms, reinterpret_cast<csSDK_size_t>(param1), reinterpret_cast<imIndPixelFormatRec *>(param2));
			break;

		// Importers that support the Premiere Pro 2.0 API must return malSupports8 for this selector
		case imGetSupports8:
			result = malSupports8;
			break;

		// Importers that support the Premiere Pro API must return malSupports7 for this selector
		case imGetSupports7:
			result = malSupports7;
			break;
	}

	return result;
}


prMALError
SDKInit(
	imStdParms			*stdParms, 
	imImportInfoRec		*importInfo)
{
	importInfo->addToMenu					= imMenuNew;	
	importInfo->noFile						= kPrTrue;
	importInfo->canCreate					= kPrTrue;
	importInfo->hasSetup					= kPrTrue;		
	importInfo->setupOnDblClk				= kPrTrue;	
	importInfo->canSupplyMetadataClipName	= kPrTrue;

	return imIsCacheable;
}


prMALError 
SDKGetIndFormat(
	imStdParms			*stdparms, 
	csSDK_size_t		index, 
	imIndFormatRec		*SDKIndFormatRec)

{
	prMALError	result 				= 0;
	char 		platformXten[256]	= "sdkc\0\0";

	// Add a case for each filetype.
	switch(index)
	{
	case 0:		
			SDKIndFormatRec->filetype	=	'SDKC';
		
			#ifdef PRWIN_ENV
			strcpy_s(	SDKIndFormatRec->FormatName,
						sizeof (FORMAT_NAME),
						FORMAT_NAME);				// The long name of the importer
			strcpy_s(SDKIndFormatRec->FormatShortName,
						sizeof (FORMAT_SHORT_NAME),
						FORMAT_SHORT_NAME);	// The short name of the importer (not currently used)
			#else
			strcpy(SDKIndFormatRec->FormatName, FORMAT_NAME);				// The long name of the importer
			strcpy(SDKIndFormatRec->FormatShortName, FORMAT_SHORT_NAME);	// The short of the importer (not currently used)
			#endif
			memcpy(SDKIndFormatRec->PlatformExtension, platformXten, sizeof(platformXten));	// The filename extension
			break;
	default:
			result = imBadFormatIndex;
	}
	return result;
}


/*
	GetPrefs8 is where the magic happens - display your setup dialog box, where the user can modify the clip.

	getPreviewFrameEx - Use to retrieve rendered frames for use in a modal preview window.
	timelineData - allows synthetic and custom importers  access to the timeline. 
		The timeline handle provide the importer with the ability to parse the timeline.
*/
prMALError
SDKGetPrefs8	(
	imStdParms			*stdParms, 
	imFileAccessRec8	*SDKfileAccessRec8, 
	imGetPrefsRec		*prefsRec)
{
	prMALError				returnValue				= malNoError,
							err						= 0;
	csSDK_int32				compareStringResult		= 0;
	time_t					returnedTime			= 0;
	imFileRef				openFileRef				= 0;

	#ifdef PRWIN_ENV
	prUTF16Char				*importerPiPLName		= PIPL_NAME_UTF,
							filepath[kPrMaxPath],
							filename[kPrMaxPath];
	csSDK_int32				showBackgroundFrame		= 0;
	#else
	CFStringRef				importerPiPLNameCFSR	= CFSTR (PIPL_NAME),
							importerName			= CFSTR (IMPORTER_NAME),
							importerDescription		= CFSTR (DESCRIPTION_MAC),
							filenameCFSR,
							filepathCFSR,
							editText				= CFSTR (EDITING_FILE);
	//						yes						= CFSTR ("Yes"),
	//						no						= CFSTR ("No");
	CFMutableStringRef		filepathCFMutable;
	char					filenameCString[kPrMaxPath];
	prUTF16Char				importerPiPLName[sizeof(PIPL_NAME) + 1];
	UniChar					filenameUC[kPrMaxPath];
	FSRef					fileRef,
							folderRef;
	CFURLRef				filepathURL,
							folderURL;
	CFRange					range = {0, kPrMaxPath};
	HFSUniStr255			dataForkName;	
	range.length = CFStringGetLength (importerPiPLNameCFSR);
	CFStringGetBytes (	importerPiPLNameCFSR,
						range,
						kCFStringEncodingUTF16,
						0,
						kPrFalse,
						reinterpret_cast<UInt8 *>(importerPiPLName),
						(sizeof (PIPL_NAME) + 1) * (sizeof (prUTF16Char)),
						NULL);
/*	DialogRef				alert;
	DialogItemIndex			outItemHit;
	AlertStdCFStringAlertParamRec inAlertParamNewFile = {	kStdCFStringAlertVersionOne,
															kPrTrue,
															kPrFalse,
															yes,
															no,
															NULL,
															kAlertStdAlertOKButton,
															kAlertStdAlertCancelButton,
															kWindowDefaultPosition,
															NULL};
	AlertStdCFStringAlertParamRec inAlertParamEditFile = {	kStdCFStringAlertVersionOne,
															kPrTrue,
															kPrFalse,
															(const CFStringRef)kAlertDefaultOKText,
															NULL,
															NULL,
															kAlertStdAlertOKButton,
															NULL,
															kWindowDefaultPosition,
															NULL};
*/	#endif

	// The first time you are called (or if you've been quieted or closed),
	// return the size of the buffer you want Premiere to allocate for your importer prefs
  	if(prefsRec->prefs == NULL)
   	{
		// We currently don't have any special options, so we don't need anything allocated here
		// But we ask Premiere Pro to allocate something so that prefs won't be NULL next time
  		prefsRec->prefsLength = 1;
   	}
	else
	{
		// If the passed in filename is the same as your importer name
		// (as found in the PiPL) then you are being asked to create 
		// the file for the first time.
		// On Mac, the PiPL name in SDKfileAccessRec8->filepath is prepended with a '/'
		// So need to chop it off for the comparison
		#ifdef PRWIN_ENV
		compareStringResult = prUTF16CharCompare(importerPiPLName, SDKfileAccessRec8->filepath);
		#elif defined PRMAC_ENV
		compareStringResult = prUTF16CharCompare(importerPiPLName, SDKfileAccessRec8->filepath + 1);
		#endif

		// The file is being created and the prefs have been allocated.
		// Now create the new file.
		if(compareStringResult == 0)
		{
			// Generate the filename, choose an appropriate path, and save it to SDKfileAccessRec8->newfilename
			// This is where you could display your setup dialog for the first time.  We pop a sample MessageBox / StandardAlert.
			// We use the current time for the filename, and choose C:\Windows\Temp on Windows, or the Desktop on Mac.
			// [TODO] Use the Application Settings Suite to create the file at the path returned by kScratchDiskType_CaptureVideo
			time(&returnedTime);
			
			#ifdef PRWIN_ENV
			swprintf_s(filename, sizeof(filename) / sizeof (prUTF16Char), L"%ld", static_cast<long>(returnedTime));
			wcscat_s(filename, sizeof(filename) / sizeof (prUTF16Char), L".sdkc");
			wcscpy_s(filepath, sizeof(filepath) / sizeof (prUTF16Char), L"C:\\Windows\\Temp\\");
			wcscat_s(filepath, sizeof(filepath) / sizeof (prUTF16Char), filename);
			SDKfileAccessRec8->newfilename = stdParms->piSuites->memFuncs->newPtrClear ((csSDK_uint32)(wcslen (filepath) + 1) * sizeof (prUTF16Char));
			wcscpy_s(	reinterpret_cast<wchar_t *>(SDKfileAccessRec8->newfilename),
						wcslen (filepath) + 1,
						filepath);
			
			showBackgroundFrame = MessageBox(	(HWND)stdParms->piSuites->windFuncs->getMainWnd (),
												DESCRIPTION_WIN,
												IMPORTER_NAME,
												MB_YESNO | MB_DEFBUTTON2);
			if (showBackgroundFrame == IDYES)
			{
				DisplayImage (stdParms, prefsRec);
			}
			#else
			sprintf (filenameCString, "%ld", returnedTime);
			strcat (filenameCString, ".sdkc");
			filenameCFSR = CFStringCreateWithCString (	kCFAllocatorDefault,
														filenameCString,
														kCFStringEncodingMacRoman);
			err = FSFindFolder(kOnSystemDisk, kDesktopFolderType, kDontCreateFolder, &folderRef);
			folderURL = CFURLCreateFromFSRef (NULL, &folderRef);
			filepathCFSR = CFURLCopyFileSystemPath (folderURL, kCFURLPOSIXPathStyle);
			filepathCFMutable = CFStringCreateMutableCopy (	kCFAllocatorDefault,
															kPrMaxPath,
															filepathCFSR);
			CFStringAppend (filepathCFMutable, CFSTR("/"));
			CFStringAppend (filepathCFMutable, filenameCFSR);
			range.length = CFStringGetLength (filepathCFMutable);
			SDKfileAccessRec8->newfilename = stdParms->piSuites->memFuncs->newPtrClear ((static_cast<SInt32>(range.length) + 1) * sizeof (prUTF16Char));
			CFStringGetCharacters (	filepathCFMutable,
									range,
									reinterpret_cast<UniChar *>(SDKfileAccessRec8->newfilename));
			CFRelease (filepathCFMutable);
			
/*	[TODO] Will need to use Cocoa with NSAlertPanel
			err = CreateStandardAlert (	kAlertNoteAlert,
										importerName,
										importerDescription,
										&inAlertParamNewFile,
										&alert);
			err = RunStandardAlert (alert, NULL, &outItemHit);
*/			
			// [TODO] Display background frame on MacOS
			#endif
			
			// Create and open new file
			#ifdef PRWIN_ENV
			// Need to create the file in a non-protected location on Vista
			openFileRef	= CreateFileW (	filepath,
										GENERIC_WRITE | GENERIC_READ,
										FILE_SHARE_WRITE | FILE_SHARE_READ,
										NULL,
										OPEN_ALWAYS,
										FILE_ATTRIBUTE_NORMAL,
										NULL);
			#else
			CFStringGetCharacters (filenameCFSR, range, filenameUC);
			err = FSCreateFileUnicode(	&folderRef,
										CFStringGetLength (filenameCFSR),
										filenameUC,
										kFSCatInfoNone,
										NULL,
										&fileRef,
										NULL);
			// If file is already present
			if (err == dupFNErr)
			{
				FSMakeFSRefUnicode(	&folderRef,
									CFStringGetLength (filenameCFSR),
									filenameUC,
									kTextEncodingUnknown,
									&fileRef);
				FSDeleteObject(&fileRef);
				err = FSCreateFileUnicode(	&folderRef,
											CFStringGetLength (filenameCFSR),
											filenameUC,
											kFSCatInfoNone,
											NULL,
											&fileRef,
											NULL);
			}
			CFRelease (filepathCFSR);
			CFRelease (filenameCFSR);
			CFRelease (folderURL);
			
			FSGetDataForkName (&dataForkName);
			err = FSOpenFork(	&fileRef,
								dataForkName.length,
								dataForkName.unicode,
								fsWrPerm,
								reinterpret_cast<FSIORefNum*>(&openFileRef));
			#endif

			// Initialize opened file with meaning full properties and video data
			CreateNewSDKFile(stdParms, prefsRec, openFileRef, prefsRec->timelineData);

			#ifdef PRWIN_ENV
			CloseHandle(openFileRef);
			#else
			FSCloseFork(reinterpret_cast<intptr_t>(openFileRef));
			#endif
			
			// Custom importers MUST return imSetFile after file creation/modification
			returnValue = imSetFile;
		}

		// The file is being modified because the user has double-clicked on the clip
		else if (!prefsRec->firstTime)
		{
			SPBasicSuite					*BasicSuite = stdParms->piSuites->utilFuncs->getSPBasicSuite();
			PrSDKImporterFileManagerSuite	*FileImporterSuite;

			#ifdef PRWIN_ENV
			MessageBox(	(HWND)stdParms->piSuites->windFuncs->getMainWnd (),
						EDITING_FILE,
						IMPORTER_NAME,
						MB_OK);
						
			// This is where you would display your setup dialog when an
			// existing file is being modified
			DisplayImage (stdParms, prefsRec);					
			#else
/*	[TODO] Will need to use Cocoa with NSAlertPanel
			err = CreateStandardAlert (	kAlertNoteAlert,
										importerName,
										editText,
										&inAlertParamEditFile,
										&alert);
			err = RunStandardAlert (alert, NULL, &outItemHit);	
*/			
			// [TODO] Display background frame on MacOS
			#endif
			
			// [TRICKY] This close seems to help the refresh on Mac, but causes problems on Win,
			// because later the file handle is explicitly closed with a call to imQuietFile,
			// and if XMP opens a file and the file handle ID is reused, that file will get closed instead.
			// However on Mac, closing the file on imQuietFile seems to return an error, so do it here.
			#ifdef PRMAC_ENV
			SDKQuietFile (stdParms, &SDKfileAccessRec8->fileref, 0);
			#endif

			#ifdef PRWIN_ENV
			openFileRef = CreateFileW (	SDKfileAccessRec8->filepath,
										GENERIC_WRITE | GENERIC_READ,
										FILE_SHARE_WRITE | FILE_SHARE_READ,
										NULL,
										CREATE_ALWAYS,
										FILE_ATTRIBUTE_NORMAL,
										NULL);
			#else
			filepathCFSR = CFStringCreateWithCharacters (kCFAllocatorDefault,
														 SDKfileAccessRec8->filepath,
														 prUTF16CharLength (SDKfileAccessRec8->filepath));
			filepathURL = CFURLCreateWithFileSystemPath (kCFAllocatorDefault,
														 filepathCFSR,
														 kCFURLPOSIXPathStyle,
														 false);
			CFURLGetFSRef (filepathURL, &fileRef);
			FSDeleteObject(&fileRef);
			
			folderURL = CFURLCreateCopyDeletingLastPathComponent(kCFAllocatorDefault,
																 filepathURL);
			CFURLGetFSRef (folderURL, &folderRef);
			
			filenameCFSR = CFURLCopyLastPathComponent(filepathURL);
																		
			range.length = CFStringGetLength (filenameCFSR);
			CFStringGetCharacters (filenameCFSR, range, filenameUC);
			
			err = FSCreateFileUnicode(	&folderRef,
										range.length,
										filenameUC,
										kFSCatInfoNone,
										NULL,
										&fileRef,
										NULL);

			CFRelease (filepathCFSR);
			CFRelease (filenameCFSR);
			CFRelease (filepathURL);
			CFRelease (folderURL);

			FSGetDataForkName (&dataForkName);
			err = FSOpenFork(	&fileRef,
								dataForkName.length,
								dataForkName.unicode,
								fsWrPerm,
								reinterpret_cast<FSIORefNum*>(&openFileRef));
										
			// [TODO] Display background frame on MacOS											
			#endif

			if (BasicSuite)
			{
				BasicSuite->AcquireSuite(kPrSDKImporterFileManagerSuite, kPrSDKImporterFileManagerSuiteVersion, (const void**)&FileImporterSuite);
			}

			CreateNewSDKFile(stdParms, prefsRec, openFileRef, prefsRec->timelineData);
			
			#ifdef PRWIN_ENV
			CloseHandle(openFileRef);
			#else
			FSCloseFork(reinterpret_cast<intptr_t>(openFileRef));
			#endif

			// Starting in CS5, custom importers must manually refresh the file after file creation/modification
			// Bug 2303475 - In CS5.0.x, RefreshFileAsync() doesn't correctly parse the \\?\ prefix on Win, so we chop off the first 4 characters
			#ifdef PRWIN_ENV
			if (stdParms->imInterfaceVer == IMPORTMOD_VERSION_10)
			{
				FileImporterSuite->RefreshFileAsync(SDKfileAccessRec8->filepath + 4);
			}
			else
			{
				FileImporterSuite->RefreshFileAsync(SDKfileAccessRec8->filepath);
			}
			#else
			FileImporterSuite->RefreshFileAsync(SDKfileAccessRec8->filepath);
			#endif

			if (BasicSuite && FileImporterSuite)
			{
				BasicSuite->ReleaseSuite(kPrSDKImporterFileManagerSuite, kPrSDKImporterFileManagerSuiteVersion);
			}
		}
		
		#ifdef PRMAC_ENV
		CFRelease (importerPiPLNameCFSR);
		CFRelease (importerName);
		CFRelease (importerDescription);
		CFRelease (editText);
		#endif
	}

	return returnValue;
}


prMALError
SDKGetInfo8(
	imStdParms			*stdParms, 
	imFileAccessRec8	*fileAccess8, 
	imFileInfoRec8		*fileInfo8)
{
	ImporterLocalRec8H	localData8H	= 0;
	
	#ifdef PRWIN_ENV
	char				didRead		= 0;	
	csSDK_uint32		bytesRead	= 0;
	#else
	OSErr				returnValue	= 0;
	ByteCount			bytesRead;
	#endif
	
	if(fileInfo8->privatedata)
	{
		localData8H = reinterpret_cast<ImporterLocalRec8H>(fileInfo8->privatedata);
	}
	else
	{
		localData8H = reinterpret_cast<ImporterLocalRec8H>(stdParms->piSuites->memFuncs->newHandle(sizeof(ImporterLocalRec8)));
		(*localData8H)->BasicSuite = stdParms->piSuites->utilFuncs->getSPBasicSuite();
		if ((*localData8H)->BasicSuite)
		{
			(*localData8H)->BasicSuite->AcquireSuite (kPrSDKTimeSuite, kPrSDKTimeSuiteVersion, (const void**)&(*localData8H)->TimeSuite);
		}
		fileInfo8->privatedata = reinterpret_cast<void*>(localData8H);
	}

	#ifdef PRWIN_ENV
	// Apply a sample name to the clip
	wcscpy_s (fileInfo8->streamName, wcslen (L"Custom Importer clip") + 1, L"Custom Importer clip");

	// Reset the file pointer to the beginning of the file
	SetFilePointer((*localData8H)->fileRef, 0, NULL, FILE_BEGIN);
	didRead = ReadFile(	(*localData8H)->fileRef,
						&(*localData8H)->theFile,
						sizeof(SDK_File),
						(LPDWORD)&bytesRead,
						0);
	if(!didRead)
	{
		return imBadFile;
	}						

	#else
	returnValue = FSReadFork(	reinterpret_cast<intptr_t>((*localData8H)->fileRef),
								fsFromStart,
								0,
								sizeof(SDK_File),
								&(*localData8H)->theFile,
								&bytesRead);
	if(returnValue)
	{
		return imBadFile;
	}							

	#endif
	prUTF16CharCopy((*localData8H)->fileName, (const prUTF16Char*)fileAccess8->filepath);
	if((*localData8H)->theFile.hasVideo)
	{
		fileInfo8->hasVideo				= kPrTrue;
		fileInfo8->vidInfo.subType		= (*localData8H)->theFile.videoSubtype;
		fileInfo8->vidInfo.depth		= 32;		// bits per pixel
		fileInfo8->vidInfo.noDuration	= imNoDurationFalse;		// A file with 'no duration' can be stretched in the timeline
		fileInfo8->vidInfo.isStill		= kPrFalse;	// A synth importer or still file importer might set this kPrTrue	
		fileInfo8->vidInfo.isRollCrawl	= kPrFalse;

		fileInfo8->vidInfo.imageWidth	= (*localData8H)->theFile.width;
		fileInfo8->vidInfo.imageHeight	= (*localData8H)->theFile.height;

		ConvertPrTimeToScaleSampleSize(	(*localData8H)->TimeSuite,
										(*localData8H)->theFile.frameRate,
										&fileInfo8->vidScale,
										&fileInfo8->vidSampleSize);
		fileInfo8->vidDuration			= (*localData8H)->theFile.numFrames * fileInfo8->vidSampleSize;

		fileInfo8->vidInfo.alphaType	= alphaStraight;
	}

	if((*localData8H)->theFile.hasAudio)
	{
		fileInfo8->hasAudio				= kPrTrue;
		fileInfo8->audDuration			= (*localData8H)->theFile.numFrames;

		// Importer API doesn't use channel-type enum from compiler API - need to map them
		if ((*localData8H)->theFile.channelType == kPrAudioChannelType_Mono)
		{
			fileInfo8->audInfo.numChannels = 1;
		}
		else if ((*localData8H)->theFile.channelType == kPrAudioChannelType_Stereo)
		{
			fileInfo8->audInfo.numChannels = 2;
		}
		else if ((*localData8H)->theFile.channelType == kPrAudioChannelType_51)
		{
			fileInfo8->audInfo.numChannels = 6;
		}
		else
		{
			// [TODO] Use error suite to return critical error
		}

		fileInfo8->audInfo.sampleRate = (float) (*localData8H)->theFile.sampleRate;
		fileInfo8->audInfo.sampleType = kPrAudioSampleType_32BitFloat;
	}

	return imNoErr;
}


// Custom importers must do their own file handling!
prMALError
SDKOpenFile8(
	imStdParms			*stdParms, 
	imFileRef			*SDKfileRef, 
	imFileOpenRec8		*SDKfileOpenRec8)
{
	csSDK_int32		err			= imNoErr;
	ImporterLocalRec8H	localRec8H;
	
	#ifdef PRMAC_ENV
	OSErr			returnValue;
	FSRef			fileRef;					
	CFStringRef		filepathCFSR;
	CFURLRef		filepathURL;
	HFSUniStr255	dataForkName;
	#endif
	
	if(SDKfileOpenRec8->privatedata)
	{
		localRec8H = reinterpret_cast<ImporterLocalRec8H>(SDKfileOpenRec8->privatedata);
	}
	else
	{
		localRec8H = reinterpret_cast<ImporterLocalRec8H>(stdParms->piSuites->memFuncs->newHandle(sizeof(ImporterLocalRec8)));
		(*localRec8H)->BasicSuite = stdParms->piSuites->utilFuncs->getSPBasicSuite();
		if ((*localRec8H)->BasicSuite)
		{
			(*localRec8H)->BasicSuite->AcquireSuite (kPrSDKTimeSuite, kPrSDKTimeSuiteVersion, (const void**)&(*localRec8H)->TimeSuite);
		}
		SDKfileOpenRec8->privatedata = reinterpret_cast<void*>(localRec8H);
	}	
	
	#ifdef PRWIN_ENV
	(*localRec8H)->fileRef = CreateFileW(	SDKfileOpenRec8->fileinfo.filepath,
											GENERIC_WRITE | GENERIC_READ,
											FILE_SHARE_WRITE | FILE_SHARE_READ,
											NULL,
											OPEN_EXISTING,
											FILE_ATTRIBUTE_NORMAL,
											NULL);
	*SDKfileRef = (*localRec8H)->fileRef;
	if((*localRec8H)->fileRef == imInvalidHandleValue)
	{
		err = imBadFile;
	}	
											
	#else
	filepathCFSR = CFStringCreateWithCharacters (kCFAllocatorDefault,
												 SDKfileOpenRec8->fileinfo.filepath,
												 prUTF16CharLength (SDKfileOpenRec8->fileinfo.filepath));
	filepathURL = CFURLCreateWithFileSystemPath (kCFAllocatorDefault,
											 filepathCFSR,
											 kCFURLPOSIXPathStyle,
											 false),
	CFURLGetFSRef (filepathURL, &fileRef);
	CFRelease (filepathCFSR);
	CFRelease (filepathURL);
	FSGetDataForkName (&dataForkName);
	returnValue = FSOpenFork(	&fileRef,
								dataForkName.length,
								dataForkName.unicode,
								fsWrPerm,
								reinterpret_cast<FSIORefNum*>(&(*localRec8H)->fileRef));
	*SDKfileRef = (*localRec8H)->fileRef;
	#endif
	
	return err;
}


//-------------------------------------------------------------------
//-------------------------------------------------------------------
//	"Quiet" the file (it's being closed, but you maintain your Private data).  
//	
//	NOTE:	If you don't set any privateData, you will not get an imCloseFile call
//			so close it up here.  Also, since this call is made elsewhere in this code
//			with privateData == 0, we should check before doing anything with it

prMALError
SDKQuietFile(
	imStdParms			*stdParms, 
	imFileRef			*SDKfileRef, 
	void				*privateData)
{
	// If file has not yet been closed
	if (SDKfileRef && *SDKfileRef != imInvalidHandleValue)
	{
		#ifdef PRWIN_ENV
		CloseHandle (*SDKfileRef);

		#elif defined PRMAC_ENV
		FSCloseFork (reinterpret_cast<intptr_t>(*SDKfileRef));
		#endif

		*SDKfileRef = imInvalidHandleValue;
	}
		
	return imNoErr; 
}


prMALError
SDKCloseFile(
	imStdParms			*stdParms, 
	imFileRef			*SDKfileRef,
	void				*privateData) 
{
	/*	Note:
		Close the file and destroy any allocation in Private Data.  
		You MUST have allocated Private data in imGetPrefs or you will not
		receive this call.
	*/
	
	SDKQuietFile(stdParms, SDKfileRef, privateData);
	
	// Remove the privateData handle.
	// CLEANUP - Destroy the handle we created to avoid memory leaks
	if (privateData)
	{
		ImporterLocalRec8H	ldataHdl = reinterpret_cast<ImporterLocalRec8H>(privateData);
		stdParms->piSuites->memFuncs->disposeHandle(reinterpret_cast<char **>(ldataHdl));
	}

	return imNoErr;
}


prMALError
SDKImportImage(
	imStdParms			*stdParms, 
	imFileRef			SDKfileRef, 
	imImportImageRec	*imageRec)
{
	/*
		Note: return a frame of video.
		If you are a synthetic importer, generate the frame,
		otherwise, get the frame from disk.
	*/

	ImporterLocalRec8H	ldataHdl;
	SDK_File			fileHeader;
	csSDK_int32			theFrame = 0;		// The requested frame number
	char				*inFrameBuffer;		// Buffer to store read bytes
	csSDK_uint32		frameBytes;			// Number of bytes per frame

	// Get the privateData handle you stored in imGetInfo
	ldataHdl = reinterpret_cast<ImporterLocalRec8H>(imageRec->privatedata);

	/*	Note:
		Allocate memory for inFrameBuffer using Premiere's memory functions
		
		Note: 
		This is the size of the file's height * rowbytes, not the size of the frame
		that Premiere is requesting.
		(example: our stored frame is 640x480, Premiere requests a 320x240 frame)
	*/
	frameBytes = (((*ldataHdl)->theFile.width) * 4) * ((*ldataHdl)->theFile.height);
	inFrameBuffer = stdParms->piSuites->memFuncs->newPtr(frameBytes);

	
	/*	Note:
		1.	Determine the frame Premiere wants.
		2.	Determine if the file is compressed or not.
		3.	If uncompressed, open using a readfile, populate a buffer.
		4.	If compressed, open and uncompress using RLE_ReadFile, populate a buffer.
		5.	Use the buffer from either case to copy to the screen.

	*/

	// 1.	The frame requested by Premiere; don't forget to take the timebase into account!
	theFrame = imageRec->pos / imageRec->sampleSize;

	// 2.	Is the file compressed?

	// 3.	File is uncompressed
	if( (*ldataHdl)->theFile.videoSubtype == SDK_8_BIT_RGB)
	{			 
		ReadSDK_File((*ldataHdl)->fileRef, 
					 frameBytes, 
					 theFrame, 
					 inFrameBuffer);
	}

	// 4.	File is compressed
	else if ( (*ldataHdl)->theFile.videoSubtype == SDK_RLE)
	{
		// TODO: Decompress the file, return a handle to a buffer
	}

	else
	{
		// Since we should have either SDK_UNCOMPRESSED or SDK_RLE, anything else is an error
		return imMemErr;
	}

	// 5.	Populate the Premiere buffer.

	if( inFrameBuffer != 0)
	{
		fileHeader = (*ldataHdl)->theFile;
		
		ScaleAndBltFrame(stdParms,
  					     fileHeader,
					     frameBytes,
					     inFrameBuffer,
					     imageRec);

		stdParms->piSuites->memFuncs->disposePtr(inFrameBuffer);

		return imNoErr;
	}

	else
		return imMemErr;

	stdParms->piSuites->memFuncs->disposePtr(inFrameBuffer);

	return imNoErr;
}


prMALError
SDKImportAudio7 (
	imStdParms			*stdParms, 
	imFileRef			SDKfileRef, 
	imImportAudioRec7	*audioRec7)
{
	prMALError			result				= malNoError;
	ImporterLocalRec8H	ldataH				= reinterpret_cast<ImporterLocalRec8H>(audioRec7->privateData);
	PrAudioSample		startAudioPosition	= 0,
						numAudioFrames		= 0;
	
	calculateAudioRequest(	audioRec7,
							(*ldataH)->theFile.numSampleFrames,
							&((*ldataH)->audioPosition),
							&startAudioPosition,
							&numAudioFrames);

	setPointerToAudioStart(	ldataH,
							startAudioPosition,
							SDKfileRef);

	readAudioToBuffer(	numAudioFrames,
						(*ldataH)->theFile.numSampleFrames,
						GetNumberOfAudioChannels ((*ldataH)->theFile.channelType),
						SDKfileRef,
						audioRec7->buffer);

	return result;
}


// imGetIndPixelFormat:
// New selector for Premiere Pro: successively called to get pixel formats
// supported.  Set pixelFormatSupported to a PrPixelFormat (i.e. fourCC's)
// defined in PrSDKPixelFormat.h
prMALError
SDKGetIndPixelFormat (
	imStdParms			*stdParms,
	csSDK_size_t		index,
	imIndPixelFormatRec	*pixelRec)
{
	int resultS = imNoErr;

	switch(index)
	{
		case 0:
			pixelRec->outPixelFormat = PrPixelFormat_VUYA_4444_8u;
			break;

		case 1:
			pixelRec->outPixelFormat = PrPixelFormat_BGRA_4444_8u;
			break;
	
		default:
			resultS = imBadFormatIndex;
			break;
	}
	return resultS;
}


void
CreateNewSDKFile(
	imStdParms		*stdParms, 
	imGetPrefsRec	*prefsRec,
	imFileRef		refNum,
	PrTimelineID	timelineID)
{
	int 							newWidth 	= 0,
									newHeight 	= 0,
									high 		= 0,
									skip 		= 0,
									wide;
	char 							*src 		= NULL,
									*srcStart 	= NULL;
	prColor 						pixel;
	csSDK_uint32 					bytes		= 0,
									red,
									green,
									blue;
	SDK_File						SDK_FileRec;

	SPBasicSuite					*BasicSuite = stdParms->piSuites->utilFuncs->getSPBasicSuite();
	PrSDKTimeSuite					*TimeSuite;
	prRect							sequenceRect;
	csSDK_int32						sequencePARNum	= 10,
									sequencePARDen	= 11,
									scale			= 30000,
									sampleSize		= 1001;
	PrTime							frameRate;

	// If CS5 or later, the clip info is passed in
	if (stdParms->imInterfaceVer >= IMPORTMOD_VERSION_10)
	{
		prSetRect(&sequenceRect, 0, 0, prefsRec->imageWidth, prefsRec->imageHeight);
		sequencePARNum = prefsRec->pixelAspectNum;
		sequencePARDen = prefsRec->pixelAspectDen;
		scale = prefsRec->vidScale;
		sampleSize = prefsRec->vidSampleSize;
	}
	else
	{
		prSetRect(&sequenceRect, 0, 0, 720, 480);
	}

	if (BasicSuite)
	{
		BasicSuite->AcquireSuite (kPrSDKTimeSuite, kPrSDKTimeSuiteVersion, (const void**)&TimeSuite);
		if (TimeSuite)
		{
			ConvertScaleSampleSizeToPrTime(TimeSuite, &scale, &sampleSize, &frameRate);
			BasicSuite->ReleaseSuite(kPrSDKTimeSuite, kPrSDKTimeSuiteVersion);
		}
	}

	srand( (unsigned)time( NULL ) );

	newWidth = 32 + rand() % (sequenceRect.right - 31); // Generate a random width between 32 and the sequence width
	newHeight = 32 + rand() % (sequenceRect.bottom - 31); // Generate a random width between 32 and the sequence height
	SDK_FileRec.numFrames = 2 + rand() % 29; // Generate a random duration between 2 and 30 frames

	// Write out the new header
	#ifdef PRWIN_ENV
	strcpy_s(SDK_FileRec.name, sizeof (SDK_NAME), SDK_NAME);
	#else
	strcpy(SDK_FileRec.name, SDK_NAME);
	#endif

	SDK_FileRec.version = SDK_FILE_CURRENT_VERSION;
	SDK_FileRec.hasAudio = kPrFalse;
	SDK_FileRec.hasVideo = kPrTrue;
	SDK_FileRec.videoSubtype = SDK_8_BIT_RGB;
	SDK_FileRec.depth = 32;
	SDK_FileRec.height = newHeight;
	SDK_FileRec.width = newWidth;
	SDK_FileRec.frameRate = frameRate;
	SDK_FileRec.pixelAspectNum = sequencePARNum;
	SDK_FileRec.pixelAspectDen = sequencePARDen;

	wide = newWidth;
	high = newHeight;
	skip = 0;

	bytes = sizeof(SDK_File);
	prWriteFile(refNum,
				&SDK_FileRec,
				&bytes);

	// Buffer reused for each frame
	src = stdParms->piSuites->memFuncs->newPtr(SDK_FileRec.width * SDK_FileRec.height * PIXEL_FORMAT_SIZE + PLUS_LINE_LENGTH);

	srcStart = src;

	for (csSDK_int32 frameNum = 0; frameNum < (csSDK_int32) SDK_FileRec.numFrames; frameNum++)
	{
		for (csSDK_int32 rowNum = 0; rowNum < high; rowNum++)
		{	
			red = rand() % 255;
			green = rand() % 255;
			blue = rand() % 255;
			//pixel = RGB(red, green, blue) | 0xff000000;
			pixel = blue | (green << 8) | (red << 16) | 0xff000000;
			
			for (csSDK_int32 pixelNum = 0; pixelNum < wide; pixelNum++)
			{
				*(csSDK_uint32 *)src = pixel;
				src += PIXEL_FORMAT_SIZE;
			}
			src += skip;
		}

		// Write frame
		bytes = newWidth * PIXEL_FORMAT_SIZE * newHeight + PLUS_LINE_LENGTH;
		prWriteFile(refNum,
					srcStart,
					&bytes);

		// Reset pointer to first pixel in allocated buffer
		src = srcStart;
	}
}


void
DisplayImage (
	imStdParms		*stdParms,
	imGetPrefsRec	*prefsRec)
{
#ifdef PRWIN_ENV
	SPBasicSuite			*BasicSuite = stdParms->piSuites->utilFuncs->getSPBasicSuite();
	prRect					sequenceRect;
	csSDK_int32				sequencePARNum	= 10,
							sequencePARDen	= 11;
	PrTime					frameRate		= 25401600000; // 10 fps, a safe default

	// If CS5 or later, the clip info is passed in
	if (kPrTrue)//(stdParms->imInterfaceVer >= IMPORTMOD_VERSION_10)
	{
		prSetRect(&sequenceRect, 0, 0, prefsRec->imageWidth, prefsRec->imageHeight);
		sequencePARNum = prefsRec->pixelAspectNum;
		sequencePARDen = prefsRec->pixelAspectDen;
	}
	else
	{
		prSetRect(&sequenceRect, 0, 0, 720, 480);
	}

	TDB_TimeRecord localTDB = prefsRec->tdbTimelineLocation;
	PrTimelineID localTimelineID = prefsRec->timelineData;
	PrPixelFormat localPixelFormat = PrPixelFormat_BGRA_4444_8u;
	csSDK_int32 localFrame = localTDB.value;

	char* frameBuffer;
	csSDK_int32 tempRowBytes;

	PPixHand tempPPixH;

	HDC tempDC = GetDC (NULL);

	if (localTimelineID)
	{
			csSDK_int32 error = stdParms->piSuites->timelineFuncs->getPreviewFrameEx(localTimelineID,
											localFrame,
											&tempPPixH,
											&sequenceRect,
											&localPixelFormat,
											1,
											sequencePARNum,
											sequencePARDen,
											kPrFalse);

			// Get a buffer containing the bitmap bits for StretchDIBits
			frameBuffer = stdParms->piSuites->ppixFuncs->ppixGetPixels(tempPPixH);
			tempRowBytes = stdParms->piSuites->ppixFuncs->ppixGetRowbytes(tempPPixH);

			// Create a structure containing the image format for StretchDIBits
			BITMAPINFOHEADER bmiHeader = {	sizeof (BITMAPINFOHEADER),
											tempRowBytes / 4,
											sequenceRect.bottom,
											1,
											32,
											BI_RGB,
											0, 0, 0, 0,	0};
			BITMAPINFO bmi;
			bmi.bmiHeader = bmiHeader;

			// Default BltMode is ugly, HALFTONE is better
			SetStretchBltMode (tempDC, HALFTONE);

			// Send it to the screen, and adjust for pixel aspect ratio
			StretchDIBits (	tempDC,
							0,
							0,
							sequenceRect.right * sequencePARNum / sequencePARDen,
							sequenceRect.bottom,
							sequenceRect.left,
							sequenceRect.top,
							sequenceRect.right,
							sequenceRect.bottom,
							frameBuffer,
							&bmi,
							0,
							SRCCOPY);

			ReleaseDC (NULL, tempDC);

			stdParms->piSuites->ppixFuncs->ppixDispose (tempPPixH);
	}
	#endif
	//[TODO] Needs MacOS implementation
}
