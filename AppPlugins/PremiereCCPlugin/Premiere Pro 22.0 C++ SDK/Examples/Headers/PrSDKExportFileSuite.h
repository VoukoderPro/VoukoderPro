/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2002 Adobe Systems Incorporated                       */
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

//	This file is part of the public Premiere SDK.


#ifndef	PRSDKEXPORTFILESUITE_H
#define PRSDKEXPORTFILESUITE_H

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

#define kPrSDKExportFileSuite				"MediaCore Export File Suite"
#define kPrSDKExportFileSuiteVersion1		1
#define kPrSDKExportFileSuiteVersion2		2
#define kPrSDKExportFileSuiteVersion		kPrSDKExportFileSuiteVersion2

typedef enum
{
	fileSeekMode_Begin=0,
	fileSeekMode_End,
	fileSeekMode_Current,
} ExFileSuite_SeekMode;

typedef struct 
{
	prSuiteError (*Open)(
		csSDK_uint32 inFileObject);

	prSuiteError (*Write)(
		csSDK_uint32 inFileObject, 
		void* inBytes, 
		csSDK_int32 inNumBytes);

	// Version 1: Bug: fileSeekMode_End handled as fileSeekMode_Current and visa versa
	// Version 2: Fixed
	prSuiteError (*Seek)(
		csSDK_uint32 inFileObject, 
		prInt64 inPosition, 
		prInt64& outNewPosition,
		ExFileSuite_SeekMode inSeekMode);

	prSuiteError (*Close)(
		csSDK_uint32 inFileObject);

	prSuiteError (*GetPlatformPath)(
		csSDK_uint32 inFileObject,
		csSDK_int32 *outPathLength,
		prUTF16Char *outPlatformPath);		// if outPlatformPath is NULL, only outPathLength is returned;

} PrSDKExportFileSuite;

#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif /* PRSDKEXPORTFILESUITE_H */

