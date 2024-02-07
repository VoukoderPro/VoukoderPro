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

#include	<SDK_File.h>
#include	<PrSDKImport.h>
#include	<PrSDKImporterFileManagerSuite.h>
#include	<stdio.h>
#include 	<time.h>

// Constants
#define	IMPORTER_NAME		"SDK Custom Importer"
#define PIPL_NAME			"SDK Custom Importer"
#define PIPL_NAME_UTF		L"SDK Custom Importer"
#define FORMAT_NAME			"SDK Custom Importer Format"
#define	FORMAT_SHORT_NAME	"SDK File"
#define	DESCRIPTION_WIN		"Second GetPrefs, Creating new SDK file in C:\\Windows\\Temp.\nShow the background frame?"
#define DESCRIPTION_MAC		"Second GetPrefs, Creating new SDK file on Desktop.\nShow the background frame?"
#define EDITING_FILE		"Editing existing SDK File."
#define	PIXEL_FORMAT_SIZE	4

//------------------------------------------------
// Prototypes

// Declare plug-in entry point with C linkage
extern "C" {
PREMPLUGENTRY DllExport xImportEntry (
	csSDK_int32			selector, 
	imStdParms			*stdParms, 
	void				*param1, 
	void				*param2);
}

prMALError 
SDKInit(
	imStdParms			*stdParms, 
	imImportInfoRec		*importInfo);

prMALError 
SDKGetIndFormat(
	imStdParms			*stdparms, 
	csSDK_size_t		index, 
	imIndFormatRec		*SDKIndFormatRec);

prMALError 
SDKGetPrefs8 (
	imStdParms			*stdParms, 
	imFileAccessRec8	*SDKfileAccessRec8, 
	imGetPrefsRec		*SDKgetPrefsRec);

prMALError 
SDKGetInfo8(
	imStdParms			*stdparms, 
	imFileAccessRec8	*fileAccess8, 
	imFileInfoRec8		*imFileInfo8);

prMALError 
SDKOpenFile8(
	imStdParms			*stdParms, 
	imFileRef			*SDKfileRef, 
	imFileOpenRec8		*SDKfileOpenRec8);

prMALError	
SDKQuietFile(
	imStdParms			*stdParms, 
	imFileRef			*SDKfileRef, 
	void				*privateData);

prMALError	
SDKCloseFile(
	imStdParms			*stdParms, 
	imFileRef			*SDKfileRef,
	void				*privateData); 

prMALError 
SDKImportImage(
	imStdParms			*stdparms, 
	imFileRef			SDKfileRef, 
	imImportImageRec	*imageRec);

prMALError 
SDKImportAudio7(
	imStdParms			*stdParms, 
	imFileRef			SDKfileRef, 
	imImportAudioRec7	*SDKImportAudioRec7);

prMALError 
SDKGetIndPixelFormat(
	imStdParms			*stdParms, 
	csSDK_size_t		index, 
	imIndPixelFormatRec	*pixelRec);

void
CreateNewSDKFile(
	imStdParms		*stdParms,
	imGetPrefsRec	*prefsRec,
	imFileRef		refNum,
	PrTimelineID	timelineID);

void
DisplayImage(
	imStdParms		*stdParms,
	imGetPrefsRec	*prefsRec);
