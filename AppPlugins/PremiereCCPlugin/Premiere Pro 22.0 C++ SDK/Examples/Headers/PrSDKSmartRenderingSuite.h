/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2012 Adobe Systems Incorporated                       */
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

#ifndef PRSDKSMARTRENDERINGSUITE_H
#define PRSDKSMARTRENDERINGSUITE_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKMALERRORS_H
#include "PrSDKMALErrors.h"
#endif

#ifndef PRSDKPIXELFORMAT_H
#include "PrSDKPixelFormat.h"
#endif

#ifndef PRSDKTIMESUITE_H
#include "PrSDKTimeSuite.h"
#endif

#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif

#define kPrSDKSmartRenderingSuite				"MediaCore Smart Rendering Suite"
#define kPrSDKSmartRenderingSuiteVersion1		1
#define kPrSDKSmartRenderingSuiteVersion2       2
#define kPrSDKSmartRenderingSuiteVersion3       3
#define kPrSDKSmartRenderingSuiteVersion        kPrSDKSmartRenderingSuiteVersion3

typedef struct
{
	csSDK_int32 mClipID;
	csSDK_int64 mSegmentStartTime;
	csSDK_int64 mSegmentEndTime;
	csSDK_int64 mSegmentOffset;

	csSDK_int64 mClipStartTime;
	csSDK_int64 mClipEndTime;

	PrSDKString mClipPath; // this member will be disposed after the callback function in SDKSuite. 
						   // So if you want to copy this struct and use this member, 
						   // mClipPath need be re-allocated in Plugin side
	csSDK_int64 mMediaStartTime;
	csSDK_int64 mMediaEndTime;
} PrClipSegmentInfo;

typedef void (*SegmentInfoCallback)(void* inCallbackData, PrClipSegmentInfo* inClipSegmentInfo);

typedef struct
{
	prSuiteError (*BuildSmartRenderSegmentList)(SegmentInfoCallback inCallbackFunc, void* inCallbackData, csSDK_int32 inSegmentsID, PrTime inTimeBase, PrPixelFormat inPixelFormat);
    prSuiteError (*BuildSmartRenderSegmentListNoPreviewFiles)(SegmentInfoCallback inCallbackFunc, void* inCallbackData, csSDK_int32 inSegmentsID, PrTime inTimeBase, PrPixelFormat inPixelFormat);
	prSuiteError (*BuildAncillaryDataSegmentMap)(SegmentInfoCallback inCallbackFunc, void* inCallbackData, csSDK_int32 inVideoSegmentsID, PrTime inTimeBase, PrPixelFormat inPixelFormat);
	
	// ---------------------------------- color managed version ------------------------------------------- //
	prSuiteError (*BuildColorManagedSmartRenderSegmentList)(SegmentInfoCallback inCallbackFunc, void* inCallbackData, csSDK_int32 inSegmentsID, PrTime inTimeBase, PrPixelFormat inPixelFormat, PrSDKColorSpaceID	inColorSpaceID);
	prSuiteError (*BuildColorManagedSmartRenderSegmentListNoPreviewFiles)(SegmentInfoCallback inCallbackFunc, void* inCallbackData, csSDK_int32 inSegmentsID, PrTime inTimeBase, PrPixelFormat inPixelFormat, PrSDKColorSpaceID	inColorSpaceID);
} PrSDKSmartRenderingSuite;

#ifdef __cplusplus
}
#endif

#pragma pack(pop)


#endif //PRSDKSMARTRENDERINGSUITE_H
