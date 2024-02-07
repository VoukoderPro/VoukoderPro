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

#ifndef PRSDKASYNCOPERATIONSUITE_H
#define PRSDKASYNCOPERATIONSUITE_H

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

typedef csSDK_int64 PrSDKAsyncOperationID;
#define kPrSDKAsyncOperationSuite				"MediaCore Async Operation Suite"
#define kPrSDKAsyncOperationSuiteVersion		1

/*
@return true if you want the host to automatically queue more work with the same param.
*/
typedef prBool (*AsyncOperationWorkerFunction)(PrSDKAsyncOperationID inID, void* inParam1, void* inParam2, void* inParam3);

enum aoWorkerFlags
{
	aoWorkerFlag_OperationIsSingleThreaded = 1
};

typedef struct 
{
	prSuiteError (*CreateSubOperation)(
		PrSDKAsyncOperationID inParentOperation,
		PrSDKAsyncOperationID* outNewOperationID,
		PrSDKString* inSubOperationName,
		PrSDKString* inInitialStatusString);

	prSuiteError (*SetSubOperationProgress)(
		PrSDKAsyncOperationID inID,
		csSDK_int32 inCurrentPart,
		csSDK_int32 inTotalParts,
		PrSDKString* inStatusString);

	prSuiteError (*CompleteSubOperation)(
		PrSDKAsyncOperationID inID,
		PrSDKString* inFinalStatusString,
		PrSDKString* inFinalErrorString);

	prSuiteError (*RequestThreadedWorkForOperation)(
		PrSDKAsyncOperationID inID,
		AsyncOperationWorkerFunction inWorkerFunction,
		void* inWorkerParam1,
		void* inWorkerParam2,
		void* inWorkerParam3,
		csSDK_int32 inWorkerFlags);

} PrSDKAsyncOperationSuite;

#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif //PRSDKASYNCOPERATIONSUITE_H