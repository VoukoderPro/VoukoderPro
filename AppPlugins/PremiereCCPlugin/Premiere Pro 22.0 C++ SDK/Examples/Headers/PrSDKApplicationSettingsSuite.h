/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2008 Adobe Systems Incorporated                       */
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

//	This file is part of the public MediaCore SDK

#ifndef PRSDKAPPLICATIONSETTINGSSUITE_H
#define PRSDKAPPLICATIONSETTINGSSUITE_H

#ifndef PRSDKMALERRORS_H
#include "PrSDKMALErrors.h"
#endif

#ifndef PRSDKSTRINGSUITE_H
#include "PrSDKStringSuite.h"
#endif

#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif

// Version info:
// Version 1 - Created for CS401
#define kPrSDKApplicationSettingsSuite				"MediaCore Application Settings Suite"
#define kPrSDKApplicationSettingsSuiteVersion		1

enum ScratchDiskType
{
	kScratchDiskType_CaptureVideo = 0,
	kScratchDiskType_CaptureAudio,
	kScratchDiskType_PreviewVideo,
	kScratchDiskType_PreviewAudio,
	kScratchDiskType_Max = 0xFFFFFFFF
};

typedef struct 
{
	/*
	**	Given a ScratchDiskType, return a PrSDKString* containing the data. 
	**	Here's an example of how to get the data out into a prUTF16Char
	**
	**	PrSDKString captureVideoPath;
	**	prSuiteError suiteError = applicationSettingsSuite->GetScratchDiskFolder(kScratchDiskType_CaptureVideo, &captureVideoPath);
	**	if (!suiteError)
	**	{
	**		prUTF16Char* utf16CharPath = NULL;
	**		unsigned long utf16BufferElements = 0;
	**		// the first call gets the buffer length to allocate
	**		stringSuite->CopyToUTF16String(&captureVideoPath, utf16CharPath, &utf16BufferElements);
	**		utf16CharPath = new prUTF16Char[utf16BufferElements];
	**		stringSuite->CopyToUTF16String(&captureVideoPath, utf16CharPath, &utf16BufferElements);
	**		stringSuite->DisposeString(&captureVideoPath);
	**	}	
	**
	**	@param	inScratchDiskType		the scratch disk (see enum ScratchDiskType)
	**	@param	outScratchDiskFolder	if succeeded, contains the PrSDKString with the path info.
	**	@return		suiteError_InvalidCall if it isn't available (due to no project being loaded or called
	**				from a helper application context like ImporterProcessServer).
	*/
	prSuiteError (*GetScratchDiskFolder)(
		unsigned int inScratchDiskType,
		PrSDKString* outScratchDiskFolder);

	/*
	** If there is context for getting a project path, and one is opened, 
	**	@param	outCurrentProjectPath	if succeeded, contains the PrSDKString with the path info.
	**	@return		suiteError_InvalidCall if it isn't available (due to no project being loaded or called
	**				from a helper application context like ImporterProcessServer).
	*/
	prSuiteError (*GetCurrentProjectPath)(
		PrSDKString* outCurrentProjectPath);

} PrSDKApplicationSettingsSuite;


#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif
