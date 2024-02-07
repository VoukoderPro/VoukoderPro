/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 1992-2008, Adobe Systems Incorporated                 */
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
	10/15/99	eks - Created, version 1.0
	12/7/99		bbb - Edited, changed header #inclusion
	3/27/01		bbb - Rework (and fixed reel info handling, converted to MALError usage)
	9/18/01		zal - 2 minor changes to fix crash and bad graph in imDataRateAnalysis
	1/13/03		zal - Updated to build with Adobe Premiere Pro headers, .cpp shared library
	3/7/03		zal - Updated for Adobe Premiere Pro features
	8/11/03		zal - Fixed video duration analysis
	1/6/04		zal - Added audio support for Premiere Pro, arbitrary audio sample rates,
				multi-channel audio, pixel aspect ratio, and fields; code cleanup
	3/7/04		zal - Added trimming support for Premiere Pro 1.5, imOpenFile and imCloseFile
	1/25/05		zal - Support for 24-bit video (no alpha channel), versioning, imSaveFile,
				and imDeleteFile
	10/26/05	zal - Support for new Premiere Pro 2.0 selectors
	12/13/05	zal - Asynchronous import
	6/20/06		zal	- High-bit video support (v410)
	10/12/2017	zal - Turn on caching flag for ims file generation
*/

#include "SDK_File_Import.h"
#ifdef PRWIN_ENV
#include "SDK_Async_Import.h"
#endif

PREMPLUGENTRY DllExport xImportEntry (
	csSDK_int32		selector, 
	imStdParms		*stdParms, 
	void			*param1, 
	void			*param2)
{
	prMALError	result				= imUnsupported;

	switch (selector)
	{
		case imInit:
			result =	SDKInit(stdParms, 
								reinterpret_cast<imImportInfoRec*>(param1));
			break;

		// To be demonstrated
		// case imShutdown:

		case imGetPrefs8:
			result =	SDKGetPrefs8(	stdParms, 
										reinterpret_cast<imFileAccessRec8*>(param1),
										reinterpret_cast<imGetPrefsRec*>(param2));
			break;

		// To be demonstrated
		// case imSetPrefs:

		case imGetInfo8:
			result =	SDKGetInfo8(stdParms, 
									reinterpret_cast<imFileAccessRec8*>(param1), 
									reinterpret_cast<imFileInfoRec8*>(param2));
			break;

		case imImportAudio7:
			result =	SDKImportAudio7(stdParms, 
										reinterpret_cast<imFileRef>(param1),
										reinterpret_cast<imImportAudioRec7*>(param2));
			break;

		case imOpenFile8:
			result =	SDKOpenFile8(	stdParms, 
										reinterpret_cast<imFileRef*>(param1), 
										reinterpret_cast<imFileOpenRec8*>(param2));
			break;
		
		case imQuietFile:
			result =	SDKQuietFile(	stdParms, 
										reinterpret_cast<imFileRef*>(param1), 
										param2); 
			break;

		case imCloseFile:
			result =	SDKCloseFile(	stdParms, 
										reinterpret_cast<imFileRef*>(param1), 
										param2);
			break;

		case imGetTimeInfo8:
			result =	SDKGetTimeInfo8(stdParms, 
										reinterpret_cast<imFileRef>(param1),
										reinterpret_cast<imTimeInfoRec8*>(param2));
			break;

		case imSetTimeInfo8:
			result =	SDKSetTimeInfo(	stdParms, 
										reinterpret_cast<imFileRef>(param1),
										reinterpret_cast<imTimeInfoRec8*>(param2));
			break;

		case imAnalysis:
			result =	SDKAnalysis(	stdParms,
										reinterpret_cast<imFileRef>(param1),
										reinterpret_cast<imAnalysisRec*>(param2));
			break;

		case imDataRateAnalysis:
			result =	SDKDataRateAnalysis(stdParms,
											reinterpret_cast<imFileRef>(param1),
											reinterpret_cast<imDataRateAnalysisRec*>(param2));
			break;

		case imGetIndFormat:
			result =	SDKGetIndFormat(stdParms, 
										reinterpret_cast<csSDK_size_t>(param1),
										reinterpret_cast<imIndFormatRec*>(param2));
			break;

		case imSaveFile8:
			result =	SDKSaveFile8(	stdParms, 
										reinterpret_cast<imSaveFileRec8*>(param1));
			break;
			
		case imDeleteFile8:
			result =	SDKDeleteFile8(	stdParms, 
										reinterpret_cast<imDeleteFileRec8*>(param1));
			break;

		case imGetMetaData:
			result =	SDKGetMetaData(	stdParms, 
										reinterpret_cast<imFileRef>(param1),
										reinterpret_cast<imMetaDataRec*>(param2));
			break;

		case imSetMetaData:
			result =	SDKSetMetaData(	stdParms, 
										reinterpret_cast<imFileRef>(param1),
										reinterpret_cast<imMetaDataRec*>(param2));
			break;

		case imGetIndPixelFormat:
			result = SDKGetIndPixelFormat(	stdParms,
											reinterpret_cast<csSDK_size_t>(param1),
											reinterpret_cast<imIndPixelFormatRec*>(param2));
			break;

		// Importers that support the Premiere Pro 2.0 API must return malSupports8 for this selector
		case imGetSupports8:
			result = malSupports8;
			break;

		case imCheckTrim8:
			result =	SDKCheckTrim8(	stdParms,
										reinterpret_cast<imCheckTrimRec*>(param1),
										reinterpret_cast<imFileAccessRec8*>(param2));
			break;

		case imTrimFile8:
			result =	SDKTrimFile8(	stdParms,
										reinterpret_cast<imFileAccessRec8*>(param1),
										reinterpret_cast<imTrimFileRec8*>(param2));
			break;

		case imCalcSize8:
			result =	SDKCalcSize8(	stdParms,
										reinterpret_cast<imCalcSizeRec*>(param1),
										reinterpret_cast<imFileAccessRec8*>(param2));
			break;

		case imGetPreferredFrameSize:
			result =	SDKPreferredFrameSize(	stdParms,
												reinterpret_cast<imPreferredFrameSizeRec*>(param1));
			break;

		case imGetSourceVideo:
			result =	SDKGetSourceVideo(	stdParms,
											reinterpret_cast<imFileRef>(param1),
											reinterpret_cast<imSourceVideoRec*>(param2));
			break;

		case imCreateAsyncImporter:
			#ifdef PRWIN_ENV
			result =	SDKCreateAsyncImporter(	stdParms,
												reinterpret_cast<imAsyncImporterCreationRec*>(param1));
			#else
			result =	imUnsupported;
			#endif
			break;
			
			/*Message:	imPerformSourceSettingsCommand
			 Required:	no
			 Called to initialize the prefs, when the importer has an associated source settings effect
			 param1 : imFileAccessRec8*
			 param2 : imSourceSettingsCommandRec**/
			
		case imPerformSourceSettingsCommand:
			result = SDKPerformSourceSettings(stdParms,
											  reinterpret_cast<imFileAccessRec8*>(param1),
											  reinterpret_cast<imSourceSettingsCommandRec*>(param2));
		case imShutdown:
			int fail = 2;
			fail--;
			break;
			
		
			
			
	}

	return result;
}

static prMALError
SDKPerformSourceSettings(
	imStdParms*					stdParms,
	imFileAccessRec8* 			fileInfo8,
	imSourceSettingsCommandRec* sourceSettingsRec)
{
	prMALError err = malNoError;
	return err;
}

static prMALError 
SDKInit(
	imStdParms		*stdParms, 
	imImportInfoRec	*importInfo)
{
	importInfo->setupOnDblClk		= kPrFalse;		// If user dbl-clicks file you imported, pop your setup dialog
	importInfo->canSave				= kPrTrue;		// Can 'save as' files to disk, real file only
	
	// imDeleteFile8 is broken on MacOS when renaming a file using the Save Captured Files dialog
	// So it is not recommended to set this on MacOS yet (bug 1627325)
	#ifdef PRWIN_ENV
	importInfo->canDelete			= kPrTrue;		// File importers only, use if you only if you have child files
	#endif
	
	importInfo->hasSourceSettingsEffect = kPrTrue;
	
	importInfo->dontCache			= kPrFalse;		// Don't let Premiere cache these files
	importInfo->hasSetup			= kPrTrue;		// Set to kPrTrue if you have a setup dialog
	importInfo->keepLoaded			= kPrFalse;		// If you MUST stay loaded use, otherwise don't: play nice
	importInfo->priority			= 0;
	importInfo->canTrim				= kPrTrue;
	importInfo->canCalcSizes		= kPrTrue;
	if (stdParms->imInterfaceVer >= IMPORTMOD_VERSION_6)
	{
		importInfo->avoidAudioConform = kPrTrue;
	}							

	return imIsCacheable;
}


//-------------------------------------------------------------------
//-------------------------------------------------------------------
// Get the prefs for the importer

static prMALError 
SDKGetPrefs8(
	imStdParms			*stdParms, 
	imFileAccessRec8	*fileInfo8, 
	imGetPrefsRec		*prefsRec)
{
	ImporterLocalRec8	*ldata;

	//-----------------
	// The first time you are called (or if you've been Quieted or Closed)
	// you will get asked for prefs data.  First time called, return the
	// size of the buffer you want Premiere to store prefs for your plug-in.

	// Note: if canOpen is not set to kPrTrue, I'm not getting this selector. Why?
	// Answer: because this selector is associated directly with "hasSetup"

	if(prefsRec->prefsLength == 0)
	{
		prefsRec->prefsLength = sizeof(ImporterLocalRec8);
	}
	else
	{
		ldata = (ImporterLocalRec8 *)prefsRec->prefs;
		prUTF16CharCopy(ldata->fileName, fileInfo8->filepath);
	}
	return malNoError;
}


//-------------------------------------------------------------------
//-------------------------------------------------------------------
//	Open the file and return the pointer to the file.
//	NOTE: This selector will only be called if "canOpen" was set to kPrTrue in imInit  
//

prMALError 
SDKOpenFile8(
	imStdParms		*stdParms, 
	imFileRef		*SDKfileRef, 
	imFileOpenRec8	*SDKfileOpenRec8)
{
	prMALError			result = malNoError;
	ImporterLocalRec8H	localRecH;

	#ifdef PRWIN_ENV
	DWORD				shareMode;
	#else
	SInt8				filePermissions;	
	OSErr				returnValue;
	CFStringRef			filePathCFSR;
	CFURLRef			filePathURL;
	FSRef				fileRef;
	HFSUniStr255		dataForkName;	
	FSIORefNum			refNum;
	#endif
	
	if(SDKfileOpenRec8->privatedata)
	{
		localRecH = (ImporterLocalRec8H)SDKfileOpenRec8->privatedata;
	}
	else
	{
		localRecH = (ImporterLocalRec8H)stdParms->piSuites->memFuncs->newHandle(sizeof(ImporterLocalRec8));
		SDKfileOpenRec8->privatedata = (void*)localRecH;
	}	

	// Open file according to access mode specified in imFileOpenRec.inReadWrite
	// Sometimes we will be asked to open with write access, for example to write
	// timecode metadata after a capture
	#ifdef PRWIN_ENV
	if (SDKfileOpenRec8->inReadWrite == kPrOpenFileAccess_ReadOnly)
	{
		shareMode = GENERIC_READ;
	}
	else if (SDKfileOpenRec8->inReadWrite == kPrOpenFileAccess_ReadWrite)
	{
		shareMode = GENERIC_WRITE;
	}
	#else
	if (SDKfileOpenRec8->inReadWrite == kPrOpenFileAccess_ReadOnly)
	{
		filePermissions = fsRdPerm;
	}
	else if (SDKfileOpenRec8->inReadWrite == kPrOpenFileAccess_ReadWrite)
	{
		filePermissions = fsRdWrPerm;
	}
	#endif

	// Open the file
	#ifdef PRWIN_ENV
	(*localRecH)->fileRef = CreateFileW (	SDKfileOpenRec8->fileinfo.filepath,
											shareMode,
											FILE_SHARE_READ,
											NULL,
											OPEN_EXISTING,
											FILE_ATTRIBUTE_NORMAL,
											NULL);
	#else
	// Convert prUTF16Char filePath to FSRef for path
	filePathCFSR = CFStringCreateWithCharacters(NULL,
												SDKfileOpenRec8->fileinfo.filepath,
												prUTF16CharLength(SDKfileOpenRec8->fileinfo.filepath));
	filePathURL = CFURLCreateWithFileSystemPath(NULL, filePathCFSR, kCFURLPOSIXPathStyle, false);
	CFURLGetFSRef (filePathURL, &fileRef);
	FSGetDataForkName (&dataForkName);
	returnValue = FSOpenFork(	&fileRef,
								dataForkName.length,
								dataForkName.unicode,
								filePermissions,
								&refNum);
	*SDKfileRef = reinterpret_cast<void*>(refNum);
	(*localRecH)->fileRef = reinterpret_cast<void*>(refNum);
	#endif
	
	// Check to see if file is valid
	#ifdef PRWIN_ENV
	if ((*localRecH)->fileRef == imInvalidHandleValue)
	{
		result = GetLastError();

		// Make sure the file is closed if returning imBadFile.
		// Otherwise, a lower priority importer will not be able to open it.
		result = imBadFile;
	}
	else
	{
		*SDKfileRef = (*localRecH)->fileRef;
		SDKfileOpenRec8->fileinfo.fileref = (*localRecH)->fileRef;
		SDKfileOpenRec8->fileinfo.filetype = 'SDK_';
	}
	#else
	if (returnValue)
	{
		// Make sure the file is closed if returning imBadFile.
		// Otherwise, a lower priority importer will not be able to open it.
		result = imBadFile;
	}
	else
	{
		SDKfileOpenRec8->fileinfo.filetype = 'SDK_';
	}
	#endif

	return result;
}


//-------------------------------------------------------------------
//-------------------------------------------------------------------
//	"Quiet" the file (it's being closed, but you maintain your Private data).  
//	
//	NOTE:	If you don't set any privateData, you will not get an imCloseFile call
//			so close it up here.

static prMALError 
SDKQuietFile(
	imStdParms			*stdParms, 
	imFileRef			*SDKfileRef, 
	void				*privateData)
{
	// If file has not yet been closed
	#ifdef PRWIN_ENV
	if (SDKfileRef && *SDKfileRef != imInvalidHandleValue)
	{
		CloseHandle (*SDKfileRef);
		*SDKfileRef = imInvalidHandleValue;
	}
	#else
		FSCloseFork (reinterpret_cast<intptr_t>(*SDKfileRef));
		*SDKfileRef = imInvalidHandleValue;
	#endif

	return malNoError; 
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//	Close the file.  You MUST have allocated Private data in imGetPrefs or you will not
//	receive this call.

static prMALError 
SDKCloseFile(
	imStdParms			*stdParms, 
	imFileRef			*SDKfileRef,
	void				*privateData) 
{
	ImporterLocalRec8H ldataH	= reinterpret_cast<ImporterLocalRec8H>(privateData);

	// If file has not yet been closed
	if (SDKfileRef && *SDKfileRef != imInvalidHandleValue)
	{
		SDKQuietFile (stdParms, SDKfileRef, privateData);
	}

	// Remove the privateData handle.
	// CLEANUP - Destroy the handle we created to avoid memory leaks
	if (ldataH && *ldataH && (*ldataH)->BasicSuite)
	{
		(*ldataH)->BasicSuite->ReleaseSuite (kPrSDKPPixCreatorSuite, kPrSDKPPixCreatorSuiteVersion);
		(*ldataH)->BasicSuite->ReleaseSuite (kPrSDKPPixCacheSuite, kPrSDKPPixCacheSuiteVersion);
		(*ldataH)->BasicSuite->ReleaseSuite (kPrSDKPPixSuite, kPrSDKPPixSuiteVersion);
		(*ldataH)->BasicSuite->ReleaseSuite (kPrSDKTimeSuite, kPrSDKTimeSuiteVersion);
		stdParms->piSuites->memFuncs->disposeHandle(reinterpret_cast<char**>(ldataH));
	}

	return malNoError;
}


// Go ahead and overwrite any existing file. Premiere will have already checked and warned the user if file will be overwritten.
// Of course, if there are child files, you should check and return imSaveErr if appropriate
static prMALError 
SDKSaveFile8(
	imStdParms			*stdParms, 
	imSaveFileRec8		*SDKSaveFileRec8) 
{
	prMALError	result = malNoError;
	#ifdef PRMAC_ENV
	CFStringRef			sourceFilePathCFSR,
						destFilePathCFSR,
						destFolderCFSR,
						destFileNameCFSR;
	CFRange				destFileNameRange,
						destFolderRange;
	CFURLRef			sourceFilePathURL,
						destFolderURL;
	FSRef				sourceFileRef,
						destFolderRef;
												
	// Convert prUTF16Char filePaths to FSRefs for paths
	sourceFilePathCFSR = CFStringCreateWithCharacters(	kCFAllocatorDefault,
														SDKSaveFileRec8->sourcePath,
														prUTF16CharLength(SDKSaveFileRec8->sourcePath));
	destFilePathCFSR = CFStringCreateWithCharacters(	kCFAllocatorDefault,
														SDKSaveFileRec8->destPath,
														prUTF16CharLength(SDKSaveFileRec8->destPath));
														
	// Separate the folder path from the file name
	destFileNameRange = CFStringFind(	destFilePathCFSR,
										CFSTR("/"),
										kCFCompareBackwards);
	destFolderRange.location = 0;
	destFolderRange.length = destFileNameRange.location;
	destFileNameRange.location += destFileNameRange.length;
	destFileNameRange.length = CFStringGetLength(destFilePathCFSR) - destFileNameRange.location;
	destFolderCFSR = CFStringCreateWithSubstring(	kCFAllocatorDefault,
													destFilePathCFSR,
													destFolderRange);
	destFileNameCFSR = CFStringCreateWithSubstring(	kCFAllocatorDefault,
													destFilePathCFSR,
													destFileNameRange);
		
	// Make FSRefs
	sourceFilePathURL = CFURLCreateWithFileSystemPath(	kCFAllocatorDefault,
														sourceFilePathCFSR,
														kCFURLPOSIXPathStyle,
														false);
	destFolderURL = CFURLCreateWithFileSystemPath(	kCFAllocatorDefault,
													destFolderCFSR,
													kCFURLPOSIXPathStyle,
													true);
	CFURLGetFSRef (sourceFilePathURL, &sourceFileRef);
	CFURLGetFSRef (destFolderURL, &destFolderRef);						
	#endif
	
	if (SDKSaveFileRec8->move)
	{
		#ifdef PRWIN_ENV
		if (MoveFileW (SDKSaveFileRec8->sourcePath, SDKSaveFileRec8->destPath) == 0)
		{
			result = imSaveErr;
		}
		#else
		if (FSCopyObjectSync (	&sourceFileRef,
								&destFolderRef,
								destFileNameCFSR,
								NULL,
								kFSFileOperationOverwrite))
		{
			result = imSaveErr;
		}
		#endif
	}
	else
	{
		#ifdef PRWIN_ENV
		if (CopyFileW (SDKSaveFileRec8->sourcePath, SDKSaveFileRec8->destPath, kPrTrue) == 0)
		{
			result = imSaveErr;
		}
		#else
		if (FSMoveObjectSync (	&sourceFileRef,
								&destFolderRef,
								destFileNameCFSR,
								NULL,
								kFSFileOperationOverwrite))
		{
			result = imSaveErr;
		}
		#endif
	}
	return result;
}


static prMALError 
SDKDeleteFile8(
	imStdParms			*stdParms, 
	imDeleteFileRec8	*SDKDeleteFileRec8)
{
	prMALError	result = malNoError;

	#ifdef PRWIN_ENV
	if (DeleteFileW (SDKDeleteFileRec8->deleteFilePath))
	{
		result = imDeleteErr;
	}
	#else
	CFStringRef	filePathCFSR;
	CFURLRef	filePathURL;
	FSRef		fileRef;

	filePathCFSR = CFStringCreateWithCharacters(kCFAllocatorDefault,
												SDKDeleteFileRec8->deleteFilePath,
												prUTF16CharLength(SDKDeleteFileRec8->deleteFilePath));
	filePathURL = CFURLCreateWithFileSystemPath(kCFAllocatorDefault,
												filePathCFSR,
												kCFURLPOSIXPathStyle,
												false);
	CFURLGetFSRef(filePathURL, &fileRef);					
	if (FSDeleteObject(&fileRef))
	{
		result = imDeleteErr;
	}
	#endif
	
	return result;
}


static prMALError 
SDKGetMetaData(
	imStdParms			*stdParms, 
	imFileRef			SDKfileRef, 
	imMetaDataRec		*SDKMetaDataRec)
{
	ImporterLocalRec8H ldataH = reinterpret_cast<ImporterLocalRec8H>(SDKMetaDataRec->privatedata);
    #ifdef PRMAC_ENV
    #pragma unused (ldataH)
    #endif
	return malNoError;
}


static prMALError 
SDKSetMetaData(
	imStdParms			*stdParms, 
	imFileRef			SDKfileRef, 
	imMetaDataRec		*SDKMetaDataRec)
{
	ImporterLocalRec8H ldataH = reinterpret_cast<ImporterLocalRec8H>(SDKMetaDataRec->privatedata);
    #ifdef PRMAC_ENV
    #pragma unused (ldataH)
    #endif
	return malNoError;
}


static prMALError 
SDKGetIndPixelFormat(
	imStdParms			*stdParms,
	csSDK_size_t		idx,
	imIndPixelFormatRec	*SDKIndPixelFormatRec) 
{
	prMALError	result	= malNoError;
	ImporterLocalRec8H	ldataH	= reinterpret_cast<ImporterLocalRec8H>(SDKIndPixelFormatRec->privatedata);

	switch(idx)
	{
		case 0:
			if ((*ldataH)->theFile.videoSubtype == SDK_10_BIT_YUV)
			{
				SDKIndPixelFormatRec->outPixelFormat = PrPixelFormat_VUYA_4444_32f;
			}
			else
			{
				SDKIndPixelFormatRec->outPixelFormat = PrPixelFormat_BGRA_4444_8u;
			}
			break;
	
		default:
			result = imBadFormatIndex;
			break;
	}
	return result;	
}


//-------------------------------------------------------------------
//-------------------------------------------------------------------
//	GetTimeInfo8 - Read a file's timecode and pass it back to Premiere

static prMALError 
SDKGetTimeInfo8(
	imStdParms			*stdParms, 
	imFileRef			SDKfileRef, 
	imTimeInfoRec8		*SDKtimeInfoRec8)
{
	ImporterLocalRec8H ldataH = reinterpret_cast<ImporterLocalRec8H>(SDKtimeInfoRec8->privatedata);

	#ifdef PRWIN_ENV
	strcpy_s((SDKtimeInfoRec8)->altreel, 40, (*ldataH)->theFile.altreel);
	strcpy_s((SDKtimeInfoRec8)->alttime, 18, (*ldataH)->theFile.alttime);
	strcpy_s((SDKtimeInfoRec8)->orgreel, 40, (*ldataH)->theFile.orgreel);
	strcpy_s((SDKtimeInfoRec8)->orgtime, 18, (*ldataH)->theFile.orgtime);
	strcpy_s((SDKtimeInfoRec8)->logcomment, 256, (*ldataH)->theFile.logcomment);
	#else
	strcpy((SDKtimeInfoRec8)->altreel,(*ldataH)->theFile.altreel);
	strcpy((SDKtimeInfoRec8)->alttime,(*ldataH)->theFile.alttime);
	strcpy((SDKtimeInfoRec8)->orgreel,(*ldataH)->theFile.orgreel);
	strcpy((SDKtimeInfoRec8)->orgtime,(*ldataH)->theFile.orgtime);
	strcpy((SDKtimeInfoRec8)->logcomment,(*ldataH)->theFile.logcomment);
	#endif

	return malNoError;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//	SetTimeInfo - Premiere sets a file's timecode

static prMALError 	
SDKSetTimeInfo(
	imStdParms			*stdParms, 
	imFileRef			SDKfileRef, 
	imTimeInfoRec8		*SDKtimeInfoRec)
{
	prMALError	result					= malNoError;
	ImporterLocalRec8H	ldataH			= reinterpret_cast<ImporterLocalRec8H>(SDKtimeInfoRec->privatedata);
	csSDK_uint32		bytes_to_write	= sizeof(SDK_File);

	// Copy time info to our private data
	#ifdef PRWIN_ENV
	strcpy_s((*ldataH)->theFile.altreel, sizeof ((*ldataH)->theFile.altreel), SDKtimeInfoRec->altreel);
	strcpy_s((*ldataH)->theFile.alttime, sizeof ((*ldataH)->theFile.alttime), SDKtimeInfoRec->alttime);
	strcpy_s((*ldataH)->theFile.orgreel, sizeof ((*ldataH)->theFile.orgreel), SDKtimeInfoRec->orgreel);
	strcpy_s((*ldataH)->theFile.orgtime, sizeof ((*ldataH)->theFile.orgtime), SDKtimeInfoRec->orgtime);
	strcpy_s((*ldataH)->theFile.logcomment, sizeof ((*ldataH)->theFile.logcomment), SDKtimeInfoRec->logcomment);
	#else
	strcpy((*ldataH)->theFile.altreel, SDKtimeInfoRec->altreel);
	strcpy((*ldataH)->theFile.alttime, SDKtimeInfoRec->alttime);
	strcpy((*ldataH)->theFile.orgreel, SDKtimeInfoRec->orgreel);
	strcpy((*ldataH)->theFile.orgtime, SDKtimeInfoRec->orgtime);
	strcpy((*ldataH)->theFile.logcomment, SDKtimeInfoRec->logcomment);
	#endif
	
	// Write the time info to the file
	result	= prWriteFile(	SDKfileRef,
							(&(*ldataH)->theFile),
							&bytes_to_write);

	return result;
}


// File analysis - Supplies supplemental compression information to File Properties dialog
static prMALError 
SDKAnalysis(
	imStdParms		*stdParms,
	imFileRef		SDKfileRef,
	imAnalysisRec	*SDKAnalysisRec)
{
	ImporterLocalRec8H ldataH = (ImporterLocalRec8H)SDKAnalysisRec->privatedata;
	
	#ifdef PRWIN_ENV
	size_t stringLength = 0;
	#endif

	if((*ldataH)->theFile.videoSubtype == SDK_8_BIT_RGB)
	{
		// Put Unicode string in char buffer
		#ifdef PRWIN_ENV
		stringLength = wcslen(reinterpret_cast<const wchar_t*>(SDK_8_BIT_RGB_NAME));		
		wcstombs_s(	NULL,
					const_cast<char*>(SDKAnalysisRec->buffer),
					SDKAnalysisRec->buffersize,
					const_cast<wchar_t*>(SDK_8_BIT_RGB_NAME),
					stringLength);
		#elif defined PRMAC_ENV
		CFStringRef codecStringCFSR = CFStringCreateWithBytes(	kCFAllocatorDefault,
																reinterpret_cast<UInt8 *>(const_cast<wchar_t *>(SDK_8_BIT_RGB_NAME)),
																sizeof(SDK_8_BIT_RGB_NAME),
																kCFStringEncodingUTF32LE,
																kPrFalse);
		CFStringGetCString(	codecStringCFSR,
							SDKAnalysisRec->buffer,
							SDKAnalysisRec->buffersize,
							kCFStringEncodingMacRoman);
		CFRelease(codecStringCFSR);
		#endif
	}
	else if ((*ldataH)->theFile.videoSubtype == SDK_10_BIT_YUV)
	{
		#ifdef PRWIN_ENV
		stringLength = wcslen(reinterpret_cast<const wchar_t*>(SDK_10_BIT_YUV_NAME));
		wcstombs_s(	NULL,
					reinterpret_cast<char*>(SDKAnalysisRec->buffer),
					SDKAnalysisRec->buffersize,
					const_cast<wchar_t*>(SDK_10_BIT_YUV_NAME),
					stringLength);
		#elif defined PRMAC_ENV
		CFStringRef codecStringCFSR = CFStringCreateWithBytes(	kCFAllocatorDefault,
																reinterpret_cast<UInt8 *>(const_cast<wchar_t *>(SDK_10_BIT_YUV_NAME)),
																sizeof(SDK_10_BIT_YUV_NAME),
																kCFStringEncodingUTF32LE,
																kPrFalse);
		CFStringGetCString(	codecStringCFSR,
							SDKAnalysisRec->buffer,
							SDKAnalysisRec->buffersize,
							kCFStringEncodingMacRoman);
		CFRelease(codecStringCFSR);
		#endif
	}
	else
	{
		#ifdef PRWIN_ENV
		stringLength = wcslen(reinterpret_cast<const wchar_t*>(SDK_RLE_NAME));
		wcstombs_s(	NULL,
					reinterpret_cast<char*>(SDKAnalysisRec->buffer),
					SDKAnalysisRec->buffersize,
					const_cast<wchar_t*>(SDK_RLE_NAME),
					stringLength);
		#elif defined PRMAC_ENV
		CFStringRef codecStringCFSR = CFStringCreateWithBytes(	kCFAllocatorDefault,
																reinterpret_cast<UInt8 *>(const_cast<wchar_t *>(SDK_RLE_NAME)),
																sizeof(SDK_RLE_NAME),
																kCFStringEncodingUTF32LE,
																kPrFalse);
		CFStringGetCString(	codecStringCFSR,
							SDKAnalysisRec->buffer,
							SDKAnalysisRec->buffersize,
							kCFStringEncodingMacRoman);
		CFRelease(codecStringCFSR);
		#endif
	}

	return malNoError;
}


// Data rate analysis -	Supplies data rate and keyframe information to File Properties dialog
static prMALError 
SDKDataRateAnalysis(
	imStdParms				*stdParms,
	imFileRef				SDKfileRef,
	imDataRateAnalysisRec	*SDK_DRARec)
{
	prMALError			result		= malNoError;
	ImporterLocalRec8H	ldataH		= reinterpret_cast<ImporterLocalRec8H>(SDK_DRARec->privatedata);
	imDataSample		*vidSample	= NULL;
	csSDK_int32			scale		= 0,
						sampleSize	= 0;

	// Tell Premiere the size of the buffer to store, and return once to let Premiere allocate
	if(!SDK_DRARec->buffer)
	{
		// Need to get total number of frames worth of data samples that we'll populate
		SDK_DRARec->buffersize = (*ldataH)->theFile.numFrames * sizeof(imDataSample);
		result = malNoError;
	}
	else
	{
		vidSample = reinterpret_cast<imDataSample *>(SDK_DRARec->buffer);

		SDK_DRARec->baserate = 0;

		ConvertPrTimeToScaleSampleSize ((*ldataH)->TimeSuite, (*ldataH)->theFile.frameRate, &scale, &sampleSize);

		for(csSDK_uint32 i = 0; i < (*ldataH)->theFile.numFrames; i++)
		{
			// Calculate video data rate

			// This is a keyframe, so set the high bit
			vidSample->sampledur = sampleSize + 0x80000000;

			vidSample->samplesize = ((*ldataH)->theFile.width) * ((*ldataH)->theFile.height) *
				GetPixelFormatSize((*ldataH)->theFile.videoSubtype);

			// Calculate audio data rate (not demonstrated at this time)

			//  Go to next sample
			++vidSample;
		}
	}

	return result;
}


static prMALError 
SDKGetIndFormat(
	imStdParms		*stdParms, 
	csSDK_size_t	index, 
	imIndFormatRec	*SDKIndFormatRec)
{
	prMALError	result		= malNoError;
	char formatname[255]	= "SDK Importer Format";
	char shortname[32]		= "SDK File";
	char platformXten[256]	= "sdk\0\0";

	switch(index)
	{
		//	Add a case for each filetype.
		
	case 0:		
		
			SDKIndFormatRec->filetype			= 'SDK_';

			SDKIndFormatRec->canWriteTimecode	= kPrTrue;

			#ifdef PRWIN_ENV
			strcpy_s(SDKIndFormatRec->FormatName, sizeof (SDKIndFormatRec->FormatName), formatname);				// The long name of the importer
			strcpy_s(SDKIndFormatRec->FormatShortName, sizeof (SDKIndFormatRec->FormatShortName), shortname);		// The short (menu name) of the importer
			strcpy_s(SDKIndFormatRec->PlatformExtension, sizeof (SDKIndFormatRec->PlatformExtension), platformXten);	// The 3 letter extension
			#else
			strcpy(SDKIndFormatRec->FormatName, formatname);			// The Long name of the importer
			strcpy(SDKIndFormatRec->FormatShortName, shortname);		// The short (menu name) of the importer
			strcpy(SDKIndFormatRec->PlatformExtension, platformXten);	// The 3 letter extension
			#endif

			break;
		
/*		
	Support for a RLE-compressed import would look something like this:
	
	case 1:
			SDKIndFormatRec->filetype		=	'RLE_';			// The 'FCC' (Four Char Code) of our CODEC
			strcpy(SDKIndFormatRec->FormatName,formatname);				// The Long name of the importer
			strcpy(SDKIndFormatRec->FormatShortName, shortname);		// The short (menu name) of the importer
			strcpy(SDKIndFormatRec->PlatformExtension, platformXtenRLE);	// The 3 letter extension
		break;
*/

	default:
		result = imBadFormatIndex;
	}
	return result;
}


//-------------------------------------------------------------------
//-------------------------------------------------------------------
// ImportAudio7 - Gets audio chunks from the file 
// Standard file importers get the chunks from disk, 
//	a synthetic importer generates the chunks. 
// SDK format audio is always at 32-bit float
// NOTE: A chunk is an arbitrary length of data

static prMALError 
SDKImportAudio7(
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


void GetInfoVideo(
	ImporterLocalRec8H	ldataH,
	imFileInfoRec8		*SDKFileInfo8)
{
	if((**ldataH).theFile.hasVideo)
	{
		SDKFileInfo8->hasVideo				= kPrTrue;
		SDKFileInfo8->vidInfo.subType		= (**ldataH).theFile.videoSubtype;
		SDKFileInfo8->vidInfo.imageWidth	= (**ldataH).theFile.width;
		SDKFileInfo8->vidInfo.imageHeight	= (**ldataH).theFile.height;
		SDKFileInfo8->vidInfo.depth			= (**ldataH).theFile.depth;	// The bit depth of the video
		SDKFileInfo8->vidInfo.fieldType		= static_cast<char>((**ldataH).theFile.fieldType);

		if (SDKFileInfo8->vidInfo.depth == 32)
		{
			SDKFileInfo8->vidInfo.alphaType	= alphaStraight;
		}
		else
		{
			SDKFileInfo8->vidInfo.alphaType	= alphaNone;
		}

		SDKFileInfo8->vidInfo.pixelAspectNum	= (**ldataH).theFile.pixelAspectNum;
		SDKFileInfo8->vidInfo.pixelAspectDen	= (**ldataH).theFile.pixelAspectDen;

		ConvertPrTimeToScaleSampleSize(	(**ldataH).TimeSuite,
										(**ldataH).theFile.frameRate,
										&SDKFileInfo8->vidScale,
										&SDKFileInfo8->vidSampleSize);
		SDKFileInfo8->vidDuration				= (**ldataH).theFile.numFrames * SDKFileInfo8->vidSampleSize;
	}
	else
	{
		SDKFileInfo8->hasVideo = kPrFalse;
		SDKFileInfo8->vidInfo.imageWidth = 0;
		SDKFileInfo8->vidInfo.imageHeight = 0;
	}
}


prMALError MultiStreamAudioTesting(
	ImporterLocalRec8H	ldataH,
	imFileInfoRec8		*SDKFileInfo8)
{
	prMALError returnValue = malNoError;

	SDKFileInfo8->streamsAsComp = kPrFalse;

	if ((**ldataH).theFile.hasVideo)
	{
		if (((**ldataH).theFile.channelType == kPrAudioChannelType_Mono) && (SDKFileInfo8->streamIdx < 7))
		{
			if (SDKFileInfo8->streamIdx == 0)
			{
				SDKFileInfo8->hasVideo			= kPrTrue;
				SDKFileInfo8->hasAudio			= kPrFalse;
				returnValue = imIterateStreams;
			}
			else if (SDKFileInfo8->streamIdx < 6)
			{
				SDKFileInfo8->hasVideo			= kPrFalse;
				SDKFileInfo8->hasAudio			= kPrTrue;
				returnValue = imIterateStreams;
			}
			else
			{
				SDKFileInfo8->hasVideo			= kPrFalse;
				SDKFileInfo8->hasAudio			= kPrTrue;
				returnValue = imBadStreamIndex;
			}
		}

		else if (((**ldataH).theFile.channelType == kPrAudioChannelType_Stereo) && (SDKFileInfo8->streamIdx < 4))
		{
			if (SDKFileInfo8->streamIdx == 0)
			{
				SDKFileInfo8->hasVideo			= kPrTrue;
				SDKFileInfo8->hasAudio			= kPrTrue;
				returnValue = imIterateStreams;
			}
			else if (SDKFileInfo8->streamIdx < 3)
			{
				SDKFileInfo8->hasVideo			= kPrFalse;
				SDKFileInfo8->hasAudio			= kPrTrue;
				returnValue = imIterateStreams;
			}
			else
			{
				SDKFileInfo8->hasVideo			= kPrFalse;
				SDKFileInfo8->hasAudio			= kPrTrue;
				returnValue = imBadStreamIndex;
			}
		}
	}

	else // No video
	{
		if (((**ldataH).theFile.channelType == kPrAudioChannelType_Mono) && (SDKFileInfo8->streamIdx < 6))
		{
			if (SDKFileInfo8->streamIdx < 5)
			{
				SDKFileInfo8->hasVideo			= kPrFalse;
				SDKFileInfo8->hasAudio			= kPrTrue;
				returnValue = imIterateStreams;
			}
			else
			{
				SDKFileInfo8->hasVideo			= kPrFalse;
				SDKFileInfo8->hasAudio			= kPrTrue;
				returnValue = imBadStreamIndex;
			}
		}

		else if (((**ldataH).theFile.channelType == kPrAudioChannelType_Stereo) && (SDKFileInfo8->streamIdx < 4))
		{
			if (SDKFileInfo8->streamIdx < 3)
			{
				SDKFileInfo8->hasVideo			= kPrFalse;
				SDKFileInfo8->hasAudio			= kPrTrue;
				returnValue = imIterateStreams;
			}
			else
			{
				SDKFileInfo8->hasVideo			= kPrFalse;
				SDKFileInfo8->hasAudio			= kPrTrue;
				returnValue = imBadStreamIndex;
			}
		}
	}

	return returnValue;
}


prMALError
GetInfoAudio(
	ImporterLocalRec8H	ldataH,
	imFileInfoRec8		*SDKFileInfo8)
{
	prMALError returnValue = malNoError;

	if((**ldataH).theFile.hasAudio)
	{
		SDKFileInfo8->hasAudio				= kPrTrue;

		// Importer API doesn't use channel-type enum from compiler API - need to map them
		if ((**ldataH).theFile.channelType == kPrAudioChannelType_Mono)
		{
			SDKFileInfo8->audInfo.numChannels = 1;
		}
		else if ((**ldataH).theFile.channelType == kPrAudioChannelType_Stereo)
		{
			SDKFileInfo8->audInfo.numChannels = 2;
		}
		else if ((**ldataH).theFile.channelType == kPrAudioChannelType_51)
		{
			SDKFileInfo8->audInfo.numChannels = 6;
		}
		else
		{
			returnValue = imBadFile;
		}

		SDKFileInfo8->audInfo.sampleRate	= (float)(**ldataH).theFile.sampleRate;
		// 32 bit float only for now
		SDKFileInfo8->audInfo.sampleType	= kPrAudioSampleType_32BitFloat;
		SDKFileInfo8->audDuration			= (**ldataH).theFile.numSampleFrames;

		#ifdef MULTISTREAM_AUDIO_TESTING
		if (!returnValue)
		{
			returnValue = MultiStreamAudioTesting(ldataH, SDKFileInfo8);
		}
		#endif
	}
	else
	{
		SDKFileInfo8->hasAudio = kPrFalse;
	}
	return returnValue;
}


/* 
	Populate the imFileInfoRec8 structure describing this file instance
	to Premiere.  Check file validity, allocate any private instance data 
	to share between different calls.
*/
prMALError 
SDKGetInfo8(
	imStdParms			*stdParms, 
	imFileAccessRec8	*fileAccessInfo8, 
	imFileInfoRec8		*SDKFileInfo8)
{
	prMALError					result				= malNoError;
	ImporterLocalRec8H			ldataH				= NULL;

	// If Premiere Pro 2.0 / Elements 2.0 or later, specify sequential audio so we can avoid
	// audio conforming.  Otherwise, specify random access audio so that we are not sent
	// imResetSequentialAudio and imGetSequentialAudio (which are not implemented in this sample)
	if (stdParms->imInterfaceVer >= IMPORTMOD_VERSION_6)
	{
		SDKFileInfo8->accessModes = kSeparateSequentialAudio;
	}
	else
	{
		SDKFileInfo8->accessModes = kRandomAccessImport;
	}

	#ifdef PRWIN_ENV
	SDKFileInfo8->vidInfo.supportsAsyncIO			= kPrTrue;
	#elif defined PRMAC_ENV
	SDKFileInfo8->vidInfo.supportsAsyncIO			= kPrFalse;
	#endif
	SDKFileInfo8->vidInfo.supportsGetSourceVideo	= kPrTrue;
	SDKFileInfo8->vidInfo.hasPulldown				= kPrFalse;
	SDKFileInfo8->hasDataRate						= kPrTrue;

	// Get a handle to our private data.  If it doesn't exist, allocate one
	// so we can use it to store our file instance info
	if(SDKFileInfo8->privatedata)
	{
		ldataH = reinterpret_cast<ImporterLocalRec8H>(SDKFileInfo8->privatedata);
	}
	else
	{
		ldataH						= reinterpret_cast<ImporterLocalRec8H>(stdParms->piSuites->memFuncs->newHandle(sizeof(ImporterLocalRec8)));
		SDKFileInfo8->privatedata	= reinterpret_cast<void*>(ldataH);
	}

	// Either way, lock it in memory so it doesn't move while we modify it.
	stdParms->piSuites->memFuncs->lockHandle(reinterpret_cast<char**>(ldataH));

	// Always reset the file pointer to the beginning of the file
	// since plug-ins are unloaded only after 5 seconds, and the user
	// may have just opened the file, then hit "Get Properties".
	result = ReadSDKFileHeader(stdParms, fileAccessInfo8, *ldataH);

	// Check version, return error if bad version
	if ((*ldataH)->theFile.version < SDK_FILE_CURRENT_VERSION || (*ldataH)->theFile.version > SDK_FILE_CURRENT_VERSION)
	{
		return imBadFile;
	}

	// Acquire needed suites
	(*ldataH)->memFuncs = stdParms->piSuites->memFuncs;
	(*ldataH)->BasicSuite = stdParms->piSuites->utilFuncs->getSPBasicSuite();
	if ((*ldataH)->BasicSuite)
	{
		(*ldataH)->BasicSuite->AcquireSuite (kPrSDKPPixCreatorSuite, kPrSDKPPixCreatorSuiteVersion, (const void**)&(*ldataH)->PPixCreatorSuite);
		(*ldataH)->BasicSuite->AcquireSuite (kPrSDKPPixCacheSuite, kPrSDKPPixCacheSuiteVersion, (const void**)&(*ldataH)->PPixCacheSuite);
		(*ldataH)->BasicSuite->AcquireSuite (kPrSDKPPixSuite, kPrSDKPPixSuiteVersion, (const void**)&(*ldataH)->PPixSuite);
		(*ldataH)->BasicSuite->AcquireSuite (kPrSDKTimeSuite, kPrSDKTimeSuiteVersion, (const void**)&(*ldataH)->TimeSuite);
	}

	// Initialize persistent storage
	(*ldataH)->audioPosition = 0;

	// Get video info from header
	GetInfoVideo(ldataH, SDKFileInfo8);

	// Get audio info from header
	result = GetInfoAudio(ldataH, SDKFileInfo8);

	(*ldataH)->importerID = SDKFileInfo8->vidInfo.importerID;

	prUTF16CharCopy((*ldataH)->fileName, fileAccessInfo8->filepath);

	stdParms->piSuites->memFuncs->unlockHandle(reinterpret_cast<char**>(ldataH));

	return result;
}


static prMALError 
SDKCheckTrim8(
	imStdParms			*stdParms, 
	imCheckTrimRec		*checkTrimRec,
	imFileAccessRec8	*fileAccessInfo8)
{
	prMALError	result = malNoError;

	// This importer can trim at any boundary

	return result;
}

static prMALError 
SDKTrimFile8(
	imStdParms			*stdParms, 
	imFileAccessRec8	*fileAccessRec8,
	imTrimFileRec8		*trimFileRec8)
{
	prMALError			result			= malNoError;
	char				*frameBufferP,		// Buffer to store frame being copied
						*audioBufferP;		// Buffer to store audio samples being copied
	csSDK_int32			err				= 0,
						continueL		= 1,
						progressL		= 0,
						bytesPerFrameL	= 0,
						audioChannelsL	= 0;
	csSDK_uint32		bytesToWriteLu	= 0;
	ImporterLocalRec8H	ldataH			= reinterpret_cast<ImporterLocalRec8H>(trimFileRec8->privatedata);
	prBool				needsTrimB		= kPrTrue;
	
	PrAudioSample		audioBufferSize = 0,
						startingSample	= 0,
						endingSample	= 0,
						audioChannelSize = 0,
						samplesRemaining = 0,
						samplesToCopy	= 0;
	prInt64				audioStartLL	= 0,
						trimInBytesLL	= 0;
					
	#ifdef PRWIN_ENV
	HANDLE				newFileH		= 0;
	LARGE_INTEGER		distanceToMoveLI;
	csSDK_uint32		bytesWrittenLu,
						bytesReadLu;
	#endif

	// If the file doesn't need to be trimmed, then do a file copy,
	// which is simpler and faster than our buffer-by-buffer copy.
	if (((*ldataH)->theFile.numFrames * trimFileRec8->sampleSize > static_cast<csSDK_uint32>(trimFileRec8->duration)) ||
		(!trimFileRec8->keepVideo && (*ldataH)->theFile.hasVideo) ||
		(!trimFileRec8->keepAudio && (*ldataH)->theFile.hasAudio))
	{
		needsTrimB = kPrTrue;
	}
	// Sanity check the duration
	else if ((*ldataH)->theFile.numFrames * trimFileRec8->sampleSize < static_cast<csSDK_uint32>(trimFileRec8->duration))
	{
		result = imDiskErr;
	}
	else
	{
		needsTrimB = kPrFalse;

		// Copy the file
		#ifdef PRWIN_ENV
		if (!CopyFileW (fileAccessRec8->filepath, trimFileRec8->destFilePath, TRUE))
		{
			// If CopyFile fails, return disk error
			result = imDiskErr;
		}
		#else
		// [TODO]
		#endif
	}

	if (needsTrimB && continueL)
	{
		// Create the new file
		#ifdef PRWIN_ENV
		newFileH = CreateFileW (trimFileRec8->destFilePath,
								GENERIC_WRITE,
								FILE_SHARE_READ,
								NULL,
								CREATE_NEW,
								FILE_ATTRIBUTE_NORMAL,
								NULL);

		// If file already exists, error
		if (newFileH == INVALID_HANDLE_VALUE)
		{
			result = imDiskErr;
		}
		#else
		// [TODO]
		if (0)
		{}
		#endif
		// File creation successful
		else
		{
			// Update header info based on new contents of trimmed file
			SDK_File newHeader;
			memcpy (&newHeader, &(*ldataH)->theFile, sizeof (SDK_File));

			if (trimFileRec8->keepVideo && trimFileRec8->keepAudio)
			{
				newHeader.numFrames = trimFileRec8->duration / trimFileRec8->sampleSize;
				newHeader.numSampleFrames = trimFileRec8->duration * static_cast<PrAudioSample>((*ldataH)->theFile.sampleRate) / trimFileRec8->scale;
			}
			else if (trimFileRec8->keepVideo)
			{
				newHeader.hasAudio = kPrFalse;
				newHeader.numFrames = trimFileRec8->duration / trimFileRec8->sampleSize;
				newHeader.numSampleFrames = 0;
			}
			else if (trimFileRec8->keepAudio)
			{
				newHeader.hasVideo = kPrTrue;
				newHeader.numFrames = 0;
				newHeader.numSampleFrames = trimFileRec8->duration;
			}

			//	Write out the header
			bytesToWriteLu = sizeof (SDK_File);
			#ifdef PRWIN_ENV
			prWriteFile (newFileH, &newHeader, &bytesToWriteLu);
			#else
			// [TODO]
			#endif

			// Copy video
			if (trimFileRec8->keepVideo && !result)
			{
				bytesPerFrameL = (*ldataH)->theFile.width * (*ldataH)->theFile.height *
					GetPixelFormatSize((*ldataH)->theFile.videoSubtype);
				frameBufferP = stdParms->piSuites->memFuncs->newPtr(bytesPerFrameL);

				for (csSDK_int32 theFrameL = (trimFileRec8->trimIn / trimFileRec8->sampleSize);
					(theFrameL < ((trimFileRec8->trimIn + trimFileRec8->duration) / trimFileRec8->sampleSize)) && continueL;
					theFrameL++)
				{
					// Read video frame from source file
					err = ReadSDK_File(	fileAccessRec8->fileref, 
										bytesPerFrameL, 
										theFrameL, 
										frameBufferP);

					#ifdef PRWIN_ENV
					// Write video frame to dest file
					err = WriteFile(	newFileH,
										frameBufferP,
										bytesPerFrameL,
										reinterpret_cast<LPDWORD>(&bytesWrittenLu),
										NULL);

					// Write divider between frames
					err = WriteFile(	newFileH,
										PLUS_LINE,
										PLUS_LINE_LENGTH,
										reinterpret_cast<LPDWORD>(&bytesWrittenLu),
										NULL);
					#else
					// [TODO]
					#endif

					// Check for abort
					progressL = theFrameL * 50 / (trimFileRec8->duration / trimFileRec8->sampleSize);
					continueL = trimFileRec8->progressCallback (progressL, 100, trimFileRec8->progressCallbackID);
				}

				// Deallocate video buffer
				stdParms->piSuites->memFuncs->disposePtr(frameBufferP);
			} // Copy video

			// Copy audio
			if (trimFileRec8->keepAudio && !result && continueL)
			{
				audioBufferSize = static_cast<PrAudioSample>((*ldataH)->theFile.sampleRate) * AUDIO_SAMPLE_SIZE;
				// Allocate a second's amount of audio for a single channel
				audioBufferP = stdParms->piSuites->memFuncs->newPtr((csSDK_uint32)audioBufferSize);

				if (trimFileRec8->keepVideo)
				{
					startingSample = trimFileRec8->trimIn * static_cast<PrAudioSample>((*ldataH)->theFile.sampleRate) / trimFileRec8->scale;
					endingSample = (trimFileRec8->trimIn + trimFileRec8->duration) * static_cast<PrAudioSample>((*ldataH)->theFile.sampleRate) / trimFileRec8->scale;
				}
				else
				{
					startingSample = trimFileRec8->trimIn;
					endingSample = trimFileRec8->trimIn + trimFileRec8->duration;
				}

				audioChannelsL = GetNumberOfAudioChannels ((*ldataH)->theFile.channelType);

				// Calculate file position of beginning of audio in source file
				audioStartLL = sizeof(SDK_File) + (*ldataH)->theFile.numFrames * (bytesPerFrameL + 4);
				// Calculate file position of trimIn of audio in source file
				if (trimFileRec8->keepVideo)
				{
					// Must fall on a sample boundary, so we cast to __int64 before
					// multiplying by AUDIO_SAMPLE_SIZE
					trimInBytesLL =	(trimFileRec8->trimIn *
									(prInt64)(*ldataH)->theFile.sampleRate /
									trimFileRec8->scale) *
									AUDIO_SAMPLE_SIZE;
				}
				else
				{
					trimInBytesLL = trimFileRec8->trimIn * AUDIO_SAMPLE_SIZE;
				}
				audioStartLL += trimInBytesLL;

				// Calculate size of an entire audio channel
				audioChannelSize = (*ldataH)->theFile.numSampleFrames * AUDIO_SAMPLE_SIZE;

				#ifdef PRWIN_ENV
				distanceToMoveLI.QuadPart = audioStartLL;
				err =	SetFilePointerEx(	fileAccessRec8->fileref,
											distanceToMoveLI,
											NULL,
											FILE_BEGIN);
				#else
				// [TODO]
				#endif

				samplesRemaining = endingSample - startingSample;
		
				// Do for each channel
				for (csSDK_int32 theChannelL = 0; theChannelL < audioChannelsL; theChannelL++)
				{
					while (samplesRemaining && continueL)
					{
						if (audioBufferSize / AUDIO_SAMPLE_SIZE < samplesRemaining)
						{
							samplesToCopy = audioBufferSize / AUDIO_SAMPLE_SIZE;
						}
						else
						{
							samplesToCopy = samplesRemaining;
						}
						#ifdef PRWIN_ENV
						// Read audio from source file
						err = ReadFile (fileAccessRec8->fileref,
										audioBufferP,
										(csSDK_int32) samplesToCopy * AUDIO_SAMPLE_SIZE,
										reinterpret_cast<LPDWORD>(&bytesReadLu),
										NULL);
						// Write audio to dest file
						err = WriteFile(newFileH,
										audioBufferP,
										(csSDK_int32) samplesToCopy * AUDIO_SAMPLE_SIZE,
										reinterpret_cast<LPDWORD>(&bytesWrittenLu),
										NULL);
						#else
						// [TODO]
						#endif
						samplesRemaining -= samplesToCopy;

						// Check for abort
						continueL = trimFileRec8->progressCallback (50, 100, trimFileRec8->progressCallbackID);
					}

					// Move file pointer to trimIn of next channel in source file
					#ifdef PRWIN_ENV
					distanceToMoveLI.QuadPart = audioChannelSize - (endingSample - startingSample) * AUDIO_SAMPLE_SIZE;
					err = SetFilePointerEx (fileAccessRec8->fileref,
											distanceToMoveLI,
											NULL,
											FILE_CURRENT);
					#else
					// [TODO]
					#endif

					samplesRemaining = endingSample - startingSample;
				}

				// Deallocate audio buffer
				stdParms->piSuites->memFuncs->disposePtr(audioBufferP);
			} // Copy audio

			// Close file
			#ifdef PRWIN_ENV
			CloseHandle (newFileH);
			#else
			// [TODO]
			//FSCloseFork (newFileH);
			#endif
		} // File creation successful
	} // If (needsTrimB)
	return result;
}


static prMALError 
SDKCalcSize8(
	imStdParms			*stdParms, 
	imCalcSizeRec		*calcSizeRec,
	imFileAccessRec8	*fileAccessRec8)
{
	prMALError			result				= malNoError;
	ImporterLocalRec8H	ldataH				= reinterpret_cast<ImporterLocalRec8H>(calcSizeRec->privatedata);
    csSDK_int32			audioChannels		= 0;
	prInt64				bytesPerFrame		= 0,
						videoSizeInBytes	= 0,
						audioSampleFrames	= 0,
						audioSizeInBytes	= 0;
	#ifdef PRWIN_ENV
	csSDK_int32			errL				= 0;
	LARGE_INTEGER		fileSizeInBytes;
	#endif

	// If Premiere is asking for untrimmed size
	if (calcSizeRec->duration == 0)
	{
		#ifdef PRWIN_ENV
		errL = GetFileSizeEx (fileAccessRec8->fileref, &fileSizeInBytes);
		if (GetFileSizeEx (fileAccessRec8->fileref, &fileSizeInBytes))
		{
			calcSizeRec->sizeInBytes = fileSizeInBytes.QuadPart;	
		}
		else
		{
			errL = GetLastError ();
			result = imMemErr;
		}
		#else
		// [TODO]
		#endif
	}

	// Calculate trimmed size
	else
	{
		// Calculate video size in bytes
		if ((*ldataH)->theFile.hasVideo)
		{
			bytesPerFrame = (*ldataH)->theFile.height * (*ldataH)->theFile.width *
				GetPixelFormatSize((*ldataH)->theFile.videoSubtype);

			videoSizeInBytes = (bytesPerFrame + PLUS_LINE_LENGTH) * calcSizeRec->duration / calcSizeRec->sampleSize;
		}

		// Calculate audio size in bytes
		if ((*ldataH)->theFile.hasAudio)
		{
			// Calculate number of audio sample frames in trimmed file
			if ((*ldataH)->theFile.hasVideo)
			{
				audioSampleFrames = (prInt64)((*ldataH)->theFile.sampleRate * calcSizeRec->duration / calcSizeRec->scale);
			}
			else
			{
				audioSampleFrames = calcSizeRec->duration;
			}

			audioChannels = GetNumberOfAudioChannels ((*ldataH)->theFile.channelType);
			audioSizeInBytes = audioSampleFrames * audioChannels * AUDIO_SAMPLE_SIZE;
		}

		calcSizeRec->sizeInBytes = sizeof(SDK_File) + videoSizeInBytes + audioSizeInBytes;
	}

	return result;
}

static prMALError 
SDKPreferredFrameSize(
	imStdParms					*stdparms, 
	imPreferredFrameSizeRec		*preferredFrameSizeRec)
{
	prMALError			result	= malNoError;
	ImporterLocalRec8H	ldataH	= reinterpret_cast<ImporterLocalRec8H>(preferredFrameSizeRec->inPrivateData);

	// Enumerate formats in order of priority, starting from the most preferred format
	switch(preferredFrameSizeRec->inIndex)
	{
		case 0:
			preferredFrameSizeRec->outWidth = (*ldataH)->theFile.width;
			preferredFrameSizeRec->outHeight = (*ldataH)->theFile.height;
			// If we supported more formats, we'd return imIterateFrameSizes to request to be called again
			result = malNoError;
			break;
	
		default:
			// We shouldn't be called for anything other than the case above
			result = imOtherErr;
	}

	return result;
}

static prMALError 
SDKGetSourceVideo(
	imStdParms			*stdparms, 
	imFileRef			fileRef, 
	imSourceVideoRec	*sourceVideoRec)
{
	prMALError		result		= malNoError;
	csSDK_int32		theFrame	= 0,
					rowBytes	= 0;
	imFrameFormat	*frameFormat;
	char			*frameBuffer;

	// Get the privateData handle you stored in imGetInfo
	ImporterLocalRec8H ldataH = reinterpret_cast<ImporterLocalRec8H>(sourceVideoRec->inPrivateData);

	PrTime ticksPerSecond = 0;
	(*ldataH)->TimeSuite->GetTicksPerSecond(&ticksPerSecond);
	theFrame = static_cast<csSDK_int32>(sourceVideoRec->inFrameTime / (*ldataH)->theFile.frameRate);

	// Check to see if frame is already in cache
	result = (*ldataH)->PPixCacheSuite->GetFrameFromCache (	(*ldataH)->importerID,
															0,
															theFrame,
															1,
															sourceVideoRec->inFrameFormats,
															sourceVideoRec->outFrame,
															NULL,
															NULL);
	
	// If frame is not in the cache, read the frame and put it in the cache; otherwise, we're done
	if (result != suiteError_NoError)
	{
		// Get parameters for ReadFrameToBuffer()
		frameFormat = &sourceVideoRec->inFrameFormats[0];
		prRect theRect;
		if (frameFormat->inFrameWidth == 0 && frameFormat->inFrameHeight == 0)
		{
			frameFormat->inFrameWidth = (*ldataH)->theFile.width;
			frameFormat->inFrameHeight = (*ldataH)->theFile.height;
		}
		// Windows and MacOS have different definitions of Rects, so use the cross-platform prSetRect
		prSetRect (&theRect, 0, 0, frameFormat->inFrameWidth, frameFormat->inFrameHeight);
		(*ldataH)->PPixCreatorSuite->CreatePPix(sourceVideoRec->outFrame, PrPPixBufferAccess_ReadWrite, frameFormat->inPixelFormat, &theRect);
		(*ldataH)->PPixSuite->GetPixels(*sourceVideoRec->outFrame, PrPPixBufferAccess_ReadWrite, &frameBuffer);

		result = ReadFrameToBuffer(	stdparms,
									reinterpret_cast<ImporterLocalRec8H>(ldataH),
									theFrame,
									frameFormat,
									fileRef,
									frameBuffer);

		// If extra row padding is needed, add it
		(*ldataH)->PPixSuite->GetRowBytes (*sourceVideoRec->outFrame, &rowBytes);
		AddRowPadding (	frameBuffer,
						frameBuffer,
						rowBytes,
						GetPixelFormatSize (frameFormat->inPixelFormat),
						frameFormat->inFrameWidth,
						frameFormat->inFrameHeight);

		(*ldataH)->PPixCacheSuite->AddFrameToCache(	(*ldataH)->importerID,
													0,
													*sourceVideoRec->outFrame,
													theFrame,
													NULL,
													NULL);
	}

	return result;
}


// Returns the specified frame in the buffer
prMALError
ReadFrameToBuffer(
	imStdParms			*stdParmsP,
	ImporterLocalRec8H	ldataH,
	csSDK_int32			theFrame,
	imFrameFormat		*frameFormat,
	imFileRef			fileRef,
	char				*frameBuffer)
{
	prMALError		result		= malNoError;
	csSDK_int32		bytesPerFrame = frameFormat->inFrameWidth * frameFormat->inFrameHeight *
									GetPixelFormatSize((*ldataH)->theFile.videoSubtype);

	// If file is uncompressed
	if((*ldataH)->theFile.videoSubtype == SDK_8_BIT_RGB)
	{			 
		ReadSDK_File(	fileRef,
						bytesPerFrame, 
						theFrame,
						frameBuffer);
		result = malNoError;
	}
	else if((*ldataH)->theFile.videoSubtype == SDK_10_BIT_YUV)
	{
		char *v210buffer = stdParmsP->piSuites->memFuncs->newPtr(bytesPerFrame);
		ReadSDK_File(	fileRef,
						bytesPerFrame, 
						theFrame,
						v210buffer);
		ConvertFromV410To32f(v210buffer, frameBuffer, frameFormat->inFrameWidth, frameFormat->inFrameHeight);
		stdParmsP->piSuites->memFuncs->disposePtr(v210buffer);
		result = malNoError;
	}
	// If file is RLE compressed
	else if ((*ldataH)->theFile.videoSubtype == SDK_RLE)
	{
		// [TODO] Decompress the file, return a handle to a buffer
	}
	else // Anything else is an error
	{
		result = malUnknownError;
	}

	return result;
}


#ifdef PRWIN_ENV
static prMALError 
SDKCreateAsyncImporter(
	imStdParms					*stdparms, 
	imAsyncImporterCreationRec	*asyncImporterCreationRec)
{
	prMALError		result		= malNoError;

	// Set entry point for async importer
	asyncImporterCreationRec->outAsyncEntry = xAsyncImportEntry;

	// Create and initialize async importer
	// Deleted during aiClose
	SDKAsyncImporter *asyncImporter = new SDKAsyncImporter(stdparms, reinterpret_cast<ImporterLocalRec8H>(asyncImporterCreationRec->inPrivateData));

	// Store importer as private data
	asyncImporterCreationRec->outAsyncPrivateData = reinterpret_cast<void*>(asyncImporter);
	return result;
}
#endif


prMALError 
ReadSDKFileHeader(
	imStdParms			*stdParms,
	imFileAccessRec8	*fileAccessInfo, 
	ImporterLocalRec8	*ldata)
{
	prMALError		result		= malNoError;
	csSDK_uint32	bytesReadLu	= 0;

	#ifdef PRWIN_ENV
	csSDK_int32			didReadL	= 0;
	csSDK_uint32		err			= 0;
	
	err = SetFilePointer(fileAccessInfo->fileref,
					0,
					NULL,
					FILE_BEGIN);

	// Error checking
	if (err == INVALID_SET_FILE_POINTER)
	{
		err = GetLastError();
		result = imOtherErr;
	}
	else
	{
			
		didReadL = ReadFile(fileAccessInfo->fileref,
							&(ldata->theFile),
							sizeof(SDK_File),
							reinterpret_cast<LPDWORD>(&bytesReadLu),
							0);
		if (!didReadL)
		{
			result = malUnknownError;
		}
	}
	#else
	result = FSReadFork(reinterpret_cast<intptr_t>(ldata->fileRef),
						fsFromStart,
						0,
						sizeof(SDK_File),
						&(ldata->theFile),
						reinterpret_cast<ByteCount*>(&bytesReadLu));
	if (result)
	{
		result = malUnknownError;
	}
	#endif
	
	return result;
}
