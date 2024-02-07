/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2007 Adobe Systems Incorporated                       */
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
//	This file is part of the public MediaCore SDK.


#ifndef	PRSDKEXPORTPROGRESSSUITE_H
#define PRSDKEXPORTPROGRESSSUITE_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKMALERRORS_H
#include "PrSDKMALErrors.h"
#endif


#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif

#define kPrSDKExportProgressSuite				"MediaCore Export Progress Suite"
#define kPrSDKExportProgressSuiteVersion		2	// new version for CS5


// suite
typedef struct 
{
	/**
	**	Set the string for the progress bar. Pass nil to reset it to the default string.
	*/
	prSuiteError (*SetProgressString)(
		csSDK_uint32 inExportID, 
		prUTF16Char *inProgressString);

	/**
	**	Update the progress bar by percent.
	**	inPercent is 0 to 1.0
	**	returns comp_ErrNone if successful, exportReturn_Abort, suiteError_ExporterSuspended, or error
	*/
	prSuiteError (*UpdateProgressPercent)(
		csSDK_uint32 inExportID,
		float inPercent);

	/**
	**	When UpdateProgressPercent returns suiteError_ExporterSuspended,
	**	the exporter should minimize its memory usage and then call this function.
	**	When this function returns, the exporter should continue.
	*/
	prSuiteError (*WaitForResume)(
		csSDK_uint32 inExportID);

} PrSDKExportProgressSuite1;

// suite
typedef struct 
{
	/**
	**	Set the string for the progress bar. Pass nil to reset it to the default string.
	*/
	prSuiteError (*SetProgressString)(
		csSDK_uint32 inExportID, 
		prUTF16Char *inProgressString);

	/**
	**	Update the progress bar by percent.
	**	inPercent is 0 to 1.0
	**	returns comp_ErrNone if successful, exportReturn_Abort, suiteError_ExporterSuspended, or error
	*/
	prSuiteError (*UpdateProgressPercent)(
		csSDK_uint32 inExportID,
		float inPercent);

	/**
	**	Update the progress bar by percent, also setting the last video frame to be displayed in preview.
	**	Only use this function if you do not call the sequence render suite.
	**	inPercent is 0 to 1.0
	**	returns comp_ErrNone if successful, exportReturn_Abort, suiteError_ExporterSuspended, or error
	*/
	prSuiteError (*UpdateProgressPercentWithFrame)(
		csSDK_uint32 inExportID,
		float inPercent,
		PPixHand inPPix);

	/**
	**	When UpdateProgressPercent returns suiteError_ExporterSuspended,
	**	the exporter should minimize its memory usage and then call this function.
	**	When this function returns, the exporter should continue.
	*/
	prSuiteError (*WaitForResume)(
		csSDK_uint32 inExportID);

} PrSDKExportProgressSuite;

#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif /* PRSDKEXPORTPROGRESSSUITE_H */
