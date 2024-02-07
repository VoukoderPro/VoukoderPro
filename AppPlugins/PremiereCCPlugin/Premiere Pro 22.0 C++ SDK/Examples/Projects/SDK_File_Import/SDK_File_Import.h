/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 1999-2008, Adobe Systems Incorporated                 */
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

#ifndef SDKFILEIMPORT_H
#define SDKFILEIMPORT_H

#include	"SDK_File.h"
#include	<stdio.h>
#include	<time.h>

// For performance testing
// Defining will cause duration of imImportImage calls to be
// written to C:\Premiere SDK logs\ImportImage_times.tab
//#define PERFORMANCE_TESTING

#ifdef PERFORMANCE_TESTING
#include <time.h>
#include <sys\timeb.h>
#include <direct.h>
#endif

// For testing multi-stream audio support
// Defining will cause the importer to import an SDK clip with mono audio
// as having 6 identical audio streams (6 mono channels),
// and an SDK clip with stereo audio
// as having 4 stereo streams (8 channels in stereo pairs)
//#define MULTISTREAM_AUDIO_TESTING

// Constants
#define		IMPORTER_NAME			"SDK Importer"


// Declare plug-in entry point with C linkage
extern "C" {
PREMPLUGENTRY DllExport xImportEntry (csSDK_int32	selector, 
									  imStdParms	*stdParms, 
									  void			*param1, 
									  void			*param2);

}

static prMALError 
SDKInit(
	imStdParms		*stdParms, 
	imImportInfoRec	*importInfo);

static prMALError 
SDKGetPrefs8(
	imStdParms			*stdParms, 
	imFileAccessRec8	*SDKfileAccessRec8, 
	imGetPrefsRec		*prefsRec);

prMALError 
SDKOpenFile8(
	imStdParms			*stdParms, 
	imFileRef			*SDKfileRef, 
	imFileOpenRec8		*SDKfileOpenRec8);

static prMALError	
SDKQuietFile(
	imStdParms			*stdParms, 
	imFileRef			*SDKfileRef, 
	void				*privateData);

static prMALError	
SDKCloseFile(
	imStdParms			*stdParms, 
	imFileRef			*SDKfileRef,
	void				*privateData);

static prMALError	
SDKSaveFile8(
	imStdParms			*stdParms, 
	imSaveFileRec8		*SDKSaveFileRec8);
	
static prMALError
SDKDeleteFile8(
	imStdParms			*stdParms, 
	imDeleteFileRec8	*SDKDeleteFileRec8);

static prMALError	
SDKGetMetaData(
	imStdParms			*stdParms, 
	imFileRef			SDKfileRef, 
	imMetaDataRec		*SDKMetaDataRec);

static prMALError	
SDKSetMetaData(
	imStdParms			*stdParms, 
	imFileRef			SDKfileRef, 
	imMetaDataRec		*SDKMetaDataRec);

static prMALError
SDKGetIndPixelFormat(
	imStdParms			*stdParms,
	csSDK_size_t		idx,
	imIndPixelFormatRec	*SDKIndPixelFormatRec);

static prMALError	
SDKGetTimeInfo8(
	imStdParms			*stdParms, 
	imFileRef			SDKfileRef, 
	imTimeInfoRec8		*SDKtimeInfoRec8);

static prMALError	
SDKSetTimeInfo(
	imStdParms			*stdParms, 
	imFileRef			SDKfileRef, 
	imTimeInfoRec8		*SDKtimeInfoRec);

static prMALError 
SDKAnalysis(
	imStdParms		*stdParms,
	imFileRef		SDKfileRef,
	imAnalysisRec	*SDKAnalysisRec);

static prMALError 
SDKDataRateAnalysis(
	imStdParms				*stdParms,
	imFileRef				SDKfileRef,
	imDataRateAnalysisRec	*SDK_DRARec);

static prMALError 
SDKGetIndFormat(
	imStdParms			*stdparms, 
	csSDK_size_t		index, 
	imIndFormatRec		*SDKIndFormatRec);

static prMALError 
SDKImportAudio7(
	imStdParms			*stdParms, 
	imFileRef			SDKfileRef, 
	imImportAudioRec7	*SDKImportAudioRec7);

prMALError 
SDKGetInfo8(
	imStdParms			*stdparms, 
	imFileAccessRec8	*fileAccessInfo8, 
	imFileInfoRec8		*SDKFileInfo8);

static prMALError 
SDKCheckTrim8(
	imStdParms			*stdparms, 
	imCheckTrimRec		*checkTrimRec, 
	imFileAccessRec8	*fileAccessRec8);

static prMALError 
SDKTrimFile8(
	imStdParms			*stdparms, 
	imFileAccessRec8	*fileAccessRec8,
	imTrimFileRec8		*trimFileRec8);

static prMALError 
SDKCalcSize8(
	imStdParms			*stdparms, 
	imCalcSizeRec		*calcSizeRec,
	imFileAccessRec8	*fileAccessRec8);

static prMALError 
SDKPreferredFrameSize(
	imStdParms					*stdparms, 
	imPreferredFrameSizeRec		*preferredFrameSizeRec);

static prMALError 
SDKGetSourceVideo(
	imStdParms			*stdparms,
	imFileRef			fileRef, 
	imSourceVideoRec	*sourceVideoRec);

prMALError
ReadFrameToBuffer(
	imStdParms			*stdParmsP,
	ImporterLocalRec8H	localRecH,
	csSDK_int32			theFrame,
	imFrameFormat		*frameFormat,
	imFileRef			fileRef,
	char				*frameBuffer);

static prMALError
SDKPerformSourceSettings(
	 imStdParms*					stdParms,
	 imFileAccessRec8* 				fileInfo8,
	 imSourceSettingsCommandRec* 	sourceSettingsRec);

#ifdef PRWIN_ENV
static prMALError 
SDKCreateAsyncImporter(
	imStdParms					*stdparms, 
	imAsyncImporterCreationRec	*asyncImporterCreationRec);
#endif

prMALError 
ReadSDKFileHeader(
	imStdParms			*stdParms,
	imFileAccessRec8	*fileAccessInfo, 
	ImporterLocalRec8	*ldata);

#endif
