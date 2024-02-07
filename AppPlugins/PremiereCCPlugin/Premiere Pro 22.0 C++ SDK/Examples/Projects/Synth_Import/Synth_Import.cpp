/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 1992-2008 Adobe Systems Incorporated                 */
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
	This plug-in demonstrates the Synthetic Importer interface.
	It generates frames with horizontal lines of random colors.

	10/30/2002 - Moved to .NET and C++ - ZAL
	05/16/2003 - Updated for Premiere Pro and added VUYA32 support - ZAL
	11/11/2003 - Sample prefs allocation and setup dialog, improved ClassData example - ZAL
	09/25/2008 - Updated for CS4, and version 8 selectors
*/

#include "Synth_Import.h"

PREMPLUGENTRY DllExport xImportEntry (csSDK_int32 selector, imStdParms *stdParms, void *param1, void *param2)
{
	csSDK_int32 result = imUnsupported;
	
	switch (selector)
	{
		case imInit:
			result = colrInit(stdParms, reinterpret_cast<imImportInfoRec *>(param1));
			break;
		
		case imGetPrefs8:
			result = colrGetPrefs8(stdParms, reinterpret_cast<imFileAccessRec8 *>(param1), reinterpret_cast<imGetPrefsRec *>(param2));
			break;

		case imGetInfo8:
			result = colrGetInfo8(stdParms, reinterpret_cast<imFileAccessRec8 *>(param1), reinterpret_cast<imFileInfoRec8 *>(param2));
			break;

		case imImportImage:
			result = colrImportImage(stdParms, reinterpret_cast<imFileRef>(param1), reinterpret_cast<imImportImageRec *>(param2));
			break;
			
		case imGetIndFormat:
			result = colrGetIndFormat(stdParms, reinterpret_cast<csSDK_size_t>(param1), reinterpret_cast<imIndFormatRec *>(param2));
			break;

		case imGetIndPixelFormat:
			result = colrGetIndPixelFormat(stdParms, reinterpret_cast<csSDK_size_t>(param1), reinterpret_cast<imIndPixelFormatRec *>(param2));
			break;

		case imShutdown:
			result = colrShutdown(stdParms);
			break;

		case imGetSupports8:
			result = malSupports8;
			break;
	}

	return result;
}

prMALError colrInit (imStdParms *stdParms, imImportInfoRec *importInfo)
{
	importInfo->noFile				= kPrTrue;
	importInfo->addToMenu			= kPrTrue;
	importInfo->hasSetup			= kPrTrue;
	importInfo->setupOnDblClk		= kPrTrue;
	importInfo->canDoContinuousTime	= kPrTrue;

	return imNoErr;
}

prMALError colrGetInfo8 (imStdParms *stdparms, imFileAccessRec8 *fileAccessRec8, imFileInfoRec8 *imFileInfo8)
{
	int randomDurationInSeconds		= 1 + rand() % 10;

	imFileInfo8->hasVideo 			= kPrTrue;
	imFileInfo8->hasAudio			= kPrFalse;

	imFileInfo8->vidInfo.depth 		= 24;
	imFileInfo8->vidInfo.isStill 	= kPrFalse;
	imFileInfo8->vidInfo.noDuration	= imNoDurationNoDefault;

	// Leave dimensions in imFileInfoRec8->vidInfo alone,
	// and they will automatically match the user-specified settings in the New Synthetic setup dialog

	// Set a random duration between 1 and 10 seconds
	// Extra work for non-integer frame rates
	if (imFileInfo8->vidSampleSize == 1001)
	{
		imFileInfo8->vidDuration = 30030 * randomDurationInSeconds;
	}
	else if(imFileInfo8->vidSampleSize == 100)
	{
		imFileInfo8->vidDuration = 3000 * randomDurationInSeconds;
	}
	else
	{ 
		imFileInfo8->vidDuration = imFileInfo8->vidScale * randomDurationInSeconds;
	}
	return imNoErr;
}

prMALError colrImportImage (imStdParms *stdparms, imFileRef ref, imImportImageRec *imageRec)
{
	char*			src 	= imageRec->pix;
	csSDK_int32		high 	= imageRec->dstHeight;
	csSDK_int32		skip 	= imageRec->rowbytes - (imageRec->dstWidth * 4);
	csSDK_int32		wide	= 0;
	prColor			pixel;
	unsigned char	val1 = 0, val2 = 0, val3 = 0;

	// We do not have to check which colorspace we are executing on because our logic is the same
	// for both VUYA and BGRA.  However, not all synthetic importers will be this easy!
	// You may have to switch on the format returned from GetPixelFormat in the PPix Suite
	while(high)
	{
		wide 	= imageRec->dstWidth;
		val1 	= rand() % 255;
		val2	= rand() % 255;
		val3 	= rand() % 255;
		pixel = val1 | (val2 << 8) | (val3 << 16) | 0xff000000;		

		while(wide)
		{
			*(csSDK_int32 *)src 	= pixel;
			src 			+= 4;
			wide--;
		}
		src += skip;
		high--;
	}
	return imNoErr;
}

prMALError colrGetPrefs8 (imStdParms *stdParms, imFileAccessRec8 *fileAccessRec8, imGetPrefsRec *getPrefsRec)
{
	prMALError		result = imNoErr;
	csSDK_int32		**classData;
	LocalHand		recHandle;
	
/*	#ifdef PRMAC_ENV
	csSDK_int32			returnValue		= 0;
	CFStringRef			importerName	= CFSTR (IMPORTER_NAME),
						editText		= CFSTR (EDITING_FILE);
	DialogRef			alert;
	DialogItemIndex		outItemHit;
						
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
	#endif
*/
	// Here we demonstrate two types of storage:
	// * classData is shared by all clips created by this synthetic importer
	// * prefs are clip-specific

	// This example of classID usage counts how many times imGetPrefs is called
	// for all clips created by this synthetic importer
	classData = (csSDK_int32 **) stdParms->funcs->classFuncs->getClassData(SynthImporterClassID);
	if (classData == NULL || *classData == NULL)
	{
        classData = (csSDK_int32 **)stdParms->piSuites->memFuncs->newHandle(sizeof(csSDK_int32));
		stdParms->funcs->classFuncs->setClassData(SynthImporterClassID, (void*)classData);
		**classData = 0;
	}
	else
	{
		**classData += 1; // != **classData++;
	}

	// Allocate prefs if they are unallocated
	if (getPrefsRec->prefs == NULL)
	{
		getPrefsRec->prefsLength = sizeof (LocalRec);
	}
	else
	{
		// Here's where you'd pop your setup dialog and save off the settings if changed
		#ifdef PRWIN_ENV
		MessageBox(	(HWND)stdParms->piSuites->windFuncs->getMainWnd (),
					EDITING_FILE,
					IMPORTER_NAME, 
					MB_OK);
		#else
/*		[TODO] Will need to use Cocoa with NSAlertPanel
		returnValue = CreateStandardAlert (	kAlertNoteAlert,
											importerName,
											editText,
											&inAlertParamEditFile,
											&alert);
		returnValue = RunStandardAlert (alert, NULL, &outItemHit);
*/		#endif

		recHandle = (LocalHand) &(getPrefsRec->prefs);

		// If preferences are allocated but not initialized
		if (getPrefsRec->firstTime)
		{
			(*recHandle)->getPrefsCounter = 0;
		}
		// If prefs are allocated and its time to modify them
		// This will happen when you double-click on the synthetic clip
		// in the project window or timeline
		else
		{
			(*recHandle)->getPrefsCounter += 1;
		}
	}
	return result;
}

prMALError colrGetIndFormat(imStdParms *stdparms, csSDK_size_t index, imIndFormatRec *ifrec)
{
	prMALError result = imNoErr;

	switch(index)
	{
	case 0:
		// Arbitrary filetype, should be unique
		ifrec->filetype = 'SNTH';
		break;
	default:
		result = imBadFormatIndex;
	}
	return result;
}

prMALError colrShutdown (imStdParms *stdParms)
{
	// Dispose of any lingering data here
	return imNoErr;
}

// imGetIndPixelFormat:
// New selector for Premiere Pro: successively called to get pixel formats
// supported.  Set pixelFormatSupported to a PrPixelFormat (i.e. fourCC's)
// defined in PrSDKPixelFormat.h
prMALError colrGetIndPixelFormat (imStdParms *stdParms, csSDK_size_t index, imIndPixelFormatRec *pixelRec)
{
	prMALError resultS = imNoErr;

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