/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2009 Adobe Systems Incorporated                       */
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

#ifndef PRSDKASYNCFILEREADERSUITE_H
#define PRSDKASYNCFILEREADERSUITE_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKSTRINGSUITE_H
#include "PrSDKStringSuite.h"
#endif

#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif

#define kPrSDKAsyncFileReaderSuite				"MediaCore Async File Reader Suite"
#define kPrSDKAsyncFileReaderSuiteVersion		1

typedef csSDK_int64 PrSDKAsyncFileHandle;
typedef csSDK_int32 PrSDKAsyncReadResult;
typedef void (*AsyncReadCompletionFunction)(PrSDKAsyncFileHandle, PrSDKAsyncReadResult inResult, csSDK_int64 inFileOffset, csSDK_int32 inReadSize, csSDK_int32 inBytesRead, void* inBuffer, void* inParam);

enum SDK_AsyncOpenFlags
{
	SDK_AsyncOpenFlag_CompletionIsSingleThreaded = 1
};

enum SDK_AsyncReadResults
{
	SDK_AsyncReadResult_Success = 0,
	SDK_AsyncReadResult_Unknown_Error = -1,
	SDK_AsyncReadResult_FileIsStale = -2, //This means that the file has changed on disk and must be re-opened
};

typedef struct 
{
	prSuiteError (*OpenAsyncFileReader)(
		PrSDKString* inFilePath,
		csSDK_int32 inOpenFlags,
		PrSDKAsyncFileHandle* outFileHandle);

	prSuiteError (*CloseAsyncFileReader)(
		PrSDKAsyncFileHandle inFileHandle);
		
	prSuiteError (*InitiateAsyncRead)(
		PrSDKAsyncFileHandle inFileHandle,
		csSDK_int64 inFileOffset,
		csSDK_int32 inReadSize,
		void* inBuffer,
		AsyncReadCompletionFunction inCompletion,
		void* inCompletionParam);
		
	prSuiteError (*CancelAsyncRead)(
		PrSDKAsyncFileHandle inFileHandle,
		csSDK_int64 inFileOffset,
		csSDK_int32 inReadSize,
		void* inBuffer);
		
	prSuiteError (*CancelAllAsyncReads)(
		PrSDKAsyncFileHandle inFileHandle);
	
} PrSDKAsyncFileReaderSuite;

#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif //PRSDKASYNCFILEREADERSUITE_H