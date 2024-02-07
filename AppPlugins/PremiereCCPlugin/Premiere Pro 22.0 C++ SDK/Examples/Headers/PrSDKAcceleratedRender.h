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

#ifndef PRSDKACCELERATEDRENDER_H
#define PRSDKACCELERATEDRENDER_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKENTRY_H
#include "PrSDKEntry.h"
#endif

#ifndef PRSDKPLAYMODULE_H
#include "PrSDKPlayModule.h"
#endif

#ifndef PRSDKPLUGSUITES_H
#include "PrSDKPlugSuites.h"
#endif

#ifndef PRSDKCAPTIONSTREAMFORMAT_H
#include "PrSDKCaptionStreamFormat.h"
#endif

#ifndef PRSDKQUALITY_H
#include "PrSDKQuality.h"
#endif

#ifndef PRSDKPIXELFORMAT_H
#include "PrSDKPixelFormat.h"
#endif

#ifndef PRSDKSTREAMLABEL_H
#include "PrSDKStreamLabel.h"
#endif

#ifndef PRSDKTIMESUITE_H
#include "PrSDKTimeSuite.h"
#endif

#ifndef	PRSDKIMPORTERSHARED_H
#include "PrSDKImporterShared.h"
#endif

#ifndef __SPBasic__
#include "SPBasic.h"
#endif

#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif

#define prAcceleratedRendererVersion100	1 // CS4
#define prAcceleratedRendererVersion200	2 // CS5
#define prAcceleratedRendererVersion300	3 // CS5.0.1
#define prAcceleratedRendererVersion400	4 // CS5.5
#define prAcceleratedRendererVersion500	5 // CS6
#define prAcceleratedRendererVersion600	6 // CS7
#define prAcceleratedRendererVersion700	7 // CC8
#define prAcceleratedRendererVersion800	8 // Pr 14.0
#define prAcceleratedRendererVersion900	9 // Added support of export LUT
#define prAcceleratedRendererVersion1000	10 // Introducing Render Session Start-Finish
#define prAcceleratedRendererVersion1100 11 // Added to support hardware resident frame
#define RENDERMOD_VERSION	prAcceleratedRendererVersion1100

#define prHideRendererInUI -100

typedef struct
{
	prPluginID outIdentifier;
	// 0 is the priority of the host renderer - a plugin generally wants to be higher
	// <0 implies the accelerator is not enabled by default
	// <= prHideRenderInUI is self explanatory
	int outPriority;
	prUTF16Char outDisplayName[256];
} arRendererInfo;

typedef struct
{
	int inInterfaceVer; //RENDERMOD_VERSION
	int inPluginID;
	int inIndex;
	void* ioPluginPrivateInstanceData; //Set during arSelector_Startup, available during all other selectors, and cleaned up during arSelector_Shutdown

	piSuites* (*inGetPluginInterfaceSuiteCallback)();
} arStdParms;

typedef struct
{
	PrTimelineID inTimelineID;
	int inExportFlags;
	PrTime inTimelineFrameRate;
	int inTimelineWidth;
	int inTimelineHeight;
	csSDK_uint32 inTimelinePARNum;
	csSDK_uint32 inTimelinePARDen;
	prFieldType inTimelineFieldType;

	void* ioPrivatePluginSequenceData; //Set during arSelector_SequenceSetup, cleaned up during arSelector_SequenceSetdown
	PrSDKStreamLabel inStreamLabel;
} arSequenceData;

typedef struct
{
	PrTime inStartTime; //provided by the host - will start at time zero and will iterate
	PrTime outEndTime; //Filled in by the plugin
	int outStatus; //see the arSegmentStatus enum
} arVideoSegmentInfo;

typedef struct
{
	arVideoSegmentInfo inSegmentInfo;
	int inPixelFormatIndex;
	PrPixelFormat outPixelFormat;
} arVideoSegmentPixelFormat;

enum
{
	arCompletion_Success = 0,
	arCompletion_Error = -1,
	arCompletion_Cancelled = -2
};

typedef void (*AcceleratedRendererCompletionCallback)(void* inCallbackData, csSDK_int32 inRequestID, PPixHand inPPix, int inCompletion);

typedef struct
{
	PrTime inTime; 
	int inWidth;
	int inHeight;
	csSDK_uint32 inPARNum;
	csSDK_uint32 inPARDen;
	PrPixelFormat inPixelFormat;
	PrRenderQuality inQuality;
	bool inPrefetchOnly;
	bool inCompositeOnBlack;

	AcceleratedRendererCompletionCallback inCompletionCallback;
	void* inCompletionCallbackData;
	csSDK_int32 inRequestID;//provided by the host, used to cancel the request

	bool inRenderFields;
	imRenderContext mRenderContext;
} arRenderRequest;

typedef struct
{
	PrTime inTime; 
	int inWidth;
	int inHeight;
	csSDK_uint32 inPARNum;
	csSDK_uint32 inPARDen;
	PrPixelFormat inPixelFormat;
	PrRenderQuality inQuality;
	bool inPrefetchOnly;
	bool inCompositeOnBlack;

	AcceleratedRendererCompletionCallback inCompletionCallback;
	void* inCompletionCallbackData;
	csSDK_int32 inRequestID;//provided by the host, used to cancel the request

	bool inRenderFields;
	imRenderContext mRenderContext;
	PrRenderCaptionStreamFormat inCaptionStreamFormatToDisplay;
} arRenderRequest2;


typedef struct
{
    PrTime inTime; 
    int inWidth;
    int inHeight;
    csSDK_uint32 inPARNum;
    csSDK_uint32 inPARDen;
    PrPixelFormat inPixelFormat;
    PrRenderQuality inQuality;
    bool inPrefetchOnly;
    bool inCompositeOnBlack;

    AcceleratedRendererCompletionCallback inCompletionCallback;
    void* inCompletionCallbackData;
    csSDK_int32 inRequestID;//provided by the host, used to cancel the request

    bool inRenderFields;
    imRenderContext mRenderContext;
    PrRenderCaptionStreamFormat inCaptionStreamFormatToDisplay;
    
	PrPixelFormat inFinalRenderPixelFormat;
} arRenderRequest3;

typedef struct
{
	PrTime inTime; 
	int inWidth;
	int inHeight;
	csSDK_uint32 inPARNum;
	csSDK_uint32 inPARDen;
	PrPixelFormat inPixelFormat;
	PrRenderQuality inQuality;
	bool inPrefetchOnly;
	bool inCompositeOnBlack;
		
	AcceleratedRendererCompletionCallback inCompletionCallback;
	void* inCompletionCallbackData;
	csSDK_int32 inRequestID;//provided by the host, used to cancel the request
	
	bool inRenderFields;
	imRenderContext mRenderContext;
	PrRenderCaptionStreamFormat inCaptionStreamFormatToDisplay;
		
	PrPixelFormat inFinalRenderPixelFormat;
	PrSDKColorSpaceID inPrSDKFinalRenderColorSpaceID; // new in Pr 14.0
} arRenderRequest4;
	
typedef struct
{
	PrTime inTime; 
	int inWidth;
	int inHeight;
	csSDK_uint32 inPARNum;
	csSDK_uint32 inPARDen;
	PrPixelFormat inPixelFormat;
	PrRenderQuality inQuality;
	bool inPrefetchOnly;
	bool inCompositeOnBlack;
		
	AcceleratedRendererCompletionCallback inCompletionCallback;
	void* inCompletionCallbackData;
	csSDK_int32 inRequestID;//provided by the host, used to cancel the request
	
	bool inRenderFields;
	imRenderContext mRenderContext;
	PrRenderCaptionStreamFormat inCaptionStreamFormatToDisplay;
		
	PrPixelFormat inFinalRenderPixelFormat;
	PrSDKColorSpaceID inPrSDKFinalRenderColorSpaceID; // new in Pr 14.0
	PrSDKLUTID inPrSDKFinalRenderLUTID;	// Added to support export LUT
} arRenderRequest5;

typedef struct
{
	PrTime inTime; 
	int inWidth;
	int inHeight;
	csSDK_uint32 inPARNum;
	csSDK_uint32 inPARDen;
	PrPixelFormat inPixelFormat;
	PrRenderQuality inQuality;
	bool inPrefetchOnly;
	bool inCompositeOnBlack;
		
	AcceleratedRendererCompletionCallback inCompletionCallback;
	void* inCompletionCallbackData;
	csSDK_int32 inRequestID;//provided by the host, used to cancel the request
	
	bool inRenderFields;
	imRenderContext mRenderContext;
	PrRenderCaptionStreamFormat inCaptionStreamFormatToDisplay;
		
	PrPixelFormat inFinalRenderPixelFormat;
	PrSDKColorSpaceID inPrSDKFinalRenderColorSpaceID; // new in Pr 14.0
	PrSDKLUTID inPrSDKFinalRenderLUTID;	// Added to support export LUT
	bool inHardwareResidentFrameOutputSupported; // Added to support hardware resident frame
} arRenderRequest6;
	
/*
**
*/
static arRenderRequest2 ConvertToArRenderRequest2(arRenderRequest& arRenderRequest)
{
	arRenderRequest2 newRenderRequest;

	newRenderRequest.inTime = arRenderRequest.inTime;
	newRenderRequest.inWidth = arRenderRequest.inWidth;
	newRenderRequest.inHeight = arRenderRequest.inHeight;
	newRenderRequest.inPARNum = arRenderRequest.inPARNum;
	newRenderRequest.inPARDen = arRenderRequest.inPARDen;
	newRenderRequest.inPixelFormat = arRenderRequest.inPixelFormat;
	newRenderRequest.inQuality = arRenderRequest.inQuality;
	newRenderRequest.inPrefetchOnly = arRenderRequest.inPrefetchOnly;
	newRenderRequest.inCompositeOnBlack = arRenderRequest.inCompositeOnBlack;
	newRenderRequest.inCompletionCallback = arRenderRequest.inCompletionCallback;
	newRenderRequest.inCompletionCallbackData = arRenderRequest.inCompletionCallbackData;
	newRenderRequest.inRequestID = arRenderRequest.inRequestID;
	newRenderRequest.inRenderFields = arRenderRequest.inRenderFields;
	newRenderRequest.mRenderContext = arRenderRequest.mRenderContext;
	newRenderRequest.inCaptionStreamFormatToDisplay = kPrRenderCaptionStreamFormat_Undefined;	// Default

    return newRenderRequest;
}

static arRenderRequest3 ConvertToArRenderRequest2ToArRenderRequest3(arRenderRequest2& arRenderRequest2)
{
    arRenderRequest3 newRenderRequest;

    newRenderRequest.inTime = arRenderRequest2.inTime;
    newRenderRequest.inWidth = arRenderRequest2.inWidth;
    newRenderRequest.inHeight = arRenderRequest2.inHeight;
    newRenderRequest.inPARNum = arRenderRequest2.inPARNum;
    newRenderRequest.inPARDen = arRenderRequest2.inPARDen;
    newRenderRequest.inPixelFormat = arRenderRequest2.inPixelFormat;
    newRenderRequest.inQuality = arRenderRequest2.inQuality;
    newRenderRequest.inPrefetchOnly = arRenderRequest2.inPrefetchOnly;
    newRenderRequest.inCompositeOnBlack = arRenderRequest2.inCompositeOnBlack;
    newRenderRequest.inCompletionCallback = arRenderRequest2.inCompletionCallback;
    newRenderRequest.inCompletionCallbackData = arRenderRequest2.inCompletionCallbackData;
    newRenderRequest.inRequestID = arRenderRequest2.inRequestID;
    newRenderRequest.inRenderFields = arRenderRequest2.inRenderFields;
    newRenderRequest.mRenderContext = arRenderRequest2.mRenderContext;
    newRenderRequest.inCaptionStreamFormatToDisplay = arRenderRequest2.inCaptionStreamFormatToDisplay;    // Default
    newRenderRequest.inFinalRenderPixelFormat = PrPixelFormat_Invalid;
    return newRenderRequest;
}

static arRenderRequest3 ConvertToArRenderRequest1ToArRenderRequest3(arRenderRequest& arRenderRequest)
{
	arRenderRequest3 newRenderRequest3;

	newRenderRequest3.inTime = arRenderRequest.inTime;
	newRenderRequest3.inWidth = arRenderRequest.inWidth;
	newRenderRequest3.inHeight = arRenderRequest.inHeight;
	newRenderRequest3.inPARNum = arRenderRequest.inPARNum;
	newRenderRequest3.inPARDen = arRenderRequest.inPARDen;
	newRenderRequest3.inPixelFormat = arRenderRequest.inPixelFormat;
	newRenderRequest3.inQuality = arRenderRequest.inQuality;
	newRenderRequest3.inPrefetchOnly = arRenderRequest.inPrefetchOnly;
	newRenderRequest3.inCompositeOnBlack = arRenderRequest.inCompositeOnBlack;
	newRenderRequest3.inCompletionCallback = arRenderRequest.inCompletionCallback;
	newRenderRequest3.inCompletionCallbackData = arRenderRequest.inCompletionCallbackData;
	newRenderRequest3.inRequestID = arRenderRequest.inRequestID;
	newRenderRequest3.inRenderFields = arRenderRequest.inRenderFields;
	newRenderRequest3.mRenderContext = arRenderRequest.mRenderContext;
	newRenderRequest3.inCaptionStreamFormatToDisplay = kPrRenderCaptionStreamFormat_Undefined;
	newRenderRequest3.inFinalRenderPixelFormat = PrPixelFormat_Invalid;
    return newRenderRequest3;
}

static arRenderRequest4 ConvertToArRenderRequest1ToArRenderRequest4(arRenderRequest& arRenderRequest)
{
	arRenderRequest4 newRenderRequest4;
	
	newRenderRequest4.inTime = arRenderRequest.inTime;
	newRenderRequest4.inWidth = arRenderRequest.inWidth;
	newRenderRequest4.inHeight = arRenderRequest.inHeight;
	newRenderRequest4.inPARNum = arRenderRequest.inPARNum;
	newRenderRequest4.inPARDen = arRenderRequest.inPARDen;
	newRenderRequest4.inPixelFormat = arRenderRequest.inPixelFormat;
	newRenderRequest4.inQuality = arRenderRequest.inQuality;
	newRenderRequest4.inPrefetchOnly = arRenderRequest.inPrefetchOnly;
	newRenderRequest4.inCompositeOnBlack = arRenderRequest.inCompositeOnBlack;
	newRenderRequest4.inCompletionCallback = arRenderRequest.inCompletionCallback;
	newRenderRequest4.inCompletionCallbackData = arRenderRequest.inCompletionCallbackData;
	newRenderRequest4.inRequestID = arRenderRequest.inRequestID;
	newRenderRequest4.inRenderFields = arRenderRequest.inRenderFields;
	newRenderRequest4.mRenderContext = arRenderRequest.mRenderContext;
	newRenderRequest4.inCaptionStreamFormatToDisplay = kPrRenderCaptionStreamFormat_Undefined;
	newRenderRequest4.inFinalRenderPixelFormat = PrPixelFormat_Invalid;
	newRenderRequest4.inPrSDKFinalRenderColorSpaceID = kPrSDKColorSpaceID_Invalid;
	return newRenderRequest4;
}
	
static void ConvertToArRenderRequest1ToArRenderRequest5(
	const arRenderRequest& inarRenderRequest,
	arRenderRequest5& outarRenderRequest5)
{
	outarRenderRequest5.inTime = inarRenderRequest.inTime;
	outarRenderRequest5.inWidth = inarRenderRequest.inWidth;
	outarRenderRequest5.inHeight = inarRenderRequest.inHeight;
	outarRenderRequest5.inPARNum = inarRenderRequest.inPARNum;
	outarRenderRequest5.inPARDen = inarRenderRequest.inPARDen;
	outarRenderRequest5.inPixelFormat = inarRenderRequest.inPixelFormat;
	outarRenderRequest5.inQuality = inarRenderRequest.inQuality;
	outarRenderRequest5.inPrefetchOnly = inarRenderRequest.inPrefetchOnly;
	outarRenderRequest5.inCompositeOnBlack = inarRenderRequest.inCompositeOnBlack;
	outarRenderRequest5.inCompletionCallback = inarRenderRequest.inCompletionCallback;
	outarRenderRequest5.inCompletionCallbackData = inarRenderRequest.inCompletionCallbackData;
	outarRenderRequest5.inRequestID = inarRenderRequest.inRequestID;
	outarRenderRequest5.inRenderFields = inarRenderRequest.inRenderFields;
	outarRenderRequest5.mRenderContext = inarRenderRequest.mRenderContext;
	outarRenderRequest5.inCaptionStreamFormatToDisplay = kPrRenderCaptionStreamFormat_Undefined;
	outarRenderRequest5.inFinalRenderPixelFormat = PrPixelFormat_Invalid;
	outarRenderRequest5.inPrSDKFinalRenderColorSpaceID = kPrSDKColorSpaceID_Invalid;
	outarRenderRequest5.inPrSDKFinalRenderLUTID = kPrSDKLUTID_Invalid;
}

static void ConvertToArRenderRequest1ToArRenderRequest6(
	const arRenderRequest& inarRenderRequest,
	arRenderRequest6& outarRenderRequest6)
{
	outarRenderRequest6.inTime = inarRenderRequest.inTime;
	outarRenderRequest6.inWidth = inarRenderRequest.inWidth;
	outarRenderRequest6.inHeight = inarRenderRequest.inHeight;
	outarRenderRequest6.inPARNum = inarRenderRequest.inPARNum;
	outarRenderRequest6.inPARDen = inarRenderRequest.inPARDen;
	outarRenderRequest6.inPixelFormat = inarRenderRequest.inPixelFormat;
	outarRenderRequest6.inQuality = inarRenderRequest.inQuality;
	outarRenderRequest6.inPrefetchOnly = inarRenderRequest.inPrefetchOnly;
	outarRenderRequest6.inCompositeOnBlack = inarRenderRequest.inCompositeOnBlack;
	outarRenderRequest6.inCompletionCallback = inarRenderRequest.inCompletionCallback;
	outarRenderRequest6.inCompletionCallbackData = inarRenderRequest.inCompletionCallbackData;
	outarRenderRequest6.inRequestID = inarRenderRequest.inRequestID;
	outarRenderRequest6.inRenderFields = inarRenderRequest.inRenderFields;
	outarRenderRequest6.mRenderContext = inarRenderRequest.mRenderContext;
	outarRenderRequest6.inCaptionStreamFormatToDisplay = kPrRenderCaptionStreamFormat_Undefined;
	outarRenderRequest6.inFinalRenderPixelFormat = PrPixelFormat_Invalid;
	outarRenderRequest6.inPrSDKFinalRenderColorSpaceID = kPrSDKColorSpaceID_Invalid;
	outarRenderRequest6.inPrSDKFinalRenderLUTID = kPrSDKLUTID_Invalid;
	outarRenderRequest6.inHardwareResidentFrameOutputSupported = false;
}
	
static arRenderRequest4 ConvertToArRenderRequest2ToArRenderRequest4(arRenderRequest2& arRenderRequest)
{
	arRenderRequest4 newRenderRequest4;
		
	newRenderRequest4.inTime = arRenderRequest.inTime;
	newRenderRequest4.inWidth = arRenderRequest.inWidth;
	newRenderRequest4.inHeight = arRenderRequest.inHeight;
	newRenderRequest4.inPARNum = arRenderRequest.inPARNum;
	newRenderRequest4.inPARDen = arRenderRequest.inPARDen;
	newRenderRequest4.inPixelFormat = arRenderRequest.inPixelFormat;
	newRenderRequest4.inQuality = arRenderRequest.inQuality;
	newRenderRequest4.inPrefetchOnly = arRenderRequest.inPrefetchOnly;
	newRenderRequest4.inCompositeOnBlack = arRenderRequest.inCompositeOnBlack;
	newRenderRequest4.inCompletionCallback = arRenderRequest.inCompletionCallback;
	newRenderRequest4.inCompletionCallbackData = arRenderRequest.inCompletionCallbackData;
	newRenderRequest4.inRequestID = arRenderRequest.inRequestID;
	newRenderRequest4.inRenderFields = arRenderRequest.inRenderFields;
	newRenderRequest4.mRenderContext = arRenderRequest.mRenderContext;
	newRenderRequest4.inCaptionStreamFormatToDisplay = arRenderRequest.inCaptionStreamFormatToDisplay;
	newRenderRequest4.inFinalRenderPixelFormat = PrPixelFormat_Invalid;
	newRenderRequest4.inPrSDKFinalRenderColorSpaceID = kPrSDKColorSpaceID_Invalid;
	return newRenderRequest4;
}

static void ConvertToArRenderRequest2ToArRenderRequest5(
	const arRenderRequest2& inarRenderRequest,
	arRenderRequest5& outarRenderRequest5)
{
	outarRenderRequest5.inTime = inarRenderRequest.inTime;
	outarRenderRequest5.inWidth = inarRenderRequest.inWidth;
	outarRenderRequest5.inHeight = inarRenderRequest.inHeight;
	outarRenderRequest5.inPARNum = inarRenderRequest.inPARNum;
	outarRenderRequest5.inPARDen = inarRenderRequest.inPARDen;
	outarRenderRequest5.inPixelFormat = inarRenderRequest.inPixelFormat;
	outarRenderRequest5.inQuality = inarRenderRequest.inQuality;
	outarRenderRequest5.inPrefetchOnly = inarRenderRequest.inPrefetchOnly;
	outarRenderRequest5.inCompositeOnBlack = inarRenderRequest.inCompositeOnBlack;
	outarRenderRequest5.inCompletionCallback = inarRenderRequest.inCompletionCallback;
	outarRenderRequest5.inCompletionCallbackData = inarRenderRequest.inCompletionCallbackData;
	outarRenderRequest5.inRequestID = inarRenderRequest.inRequestID;
	outarRenderRequest5.inRenderFields = inarRenderRequest.inRenderFields;
	outarRenderRequest5.mRenderContext = inarRenderRequest.mRenderContext;
	outarRenderRequest5.inCaptionStreamFormatToDisplay = inarRenderRequest.inCaptionStreamFormatToDisplay;;
	outarRenderRequest5.inFinalRenderPixelFormat = PrPixelFormat_Invalid;
	outarRenderRequest5.inPrSDKFinalRenderColorSpaceID = kPrSDKColorSpaceID_Invalid;
	outarRenderRequest5.inPrSDKFinalRenderLUTID = kPrSDKLUTID_Invalid;
}

static void ConvertToArRenderRequest2ToArRenderRequest6(
	const arRenderRequest2& inarRenderRequest,
	arRenderRequest6& outarRenderRequest6)
{
	outarRenderRequest6.inTime = inarRenderRequest.inTime;
	outarRenderRequest6.inWidth = inarRenderRequest.inWidth;
	outarRenderRequest6.inHeight = inarRenderRequest.inHeight;
	outarRenderRequest6.inPARNum = inarRenderRequest.inPARNum;
	outarRenderRequest6.inPARDen = inarRenderRequest.inPARDen;
	outarRenderRequest6.inPixelFormat = inarRenderRequest.inPixelFormat;
	outarRenderRequest6.inQuality = inarRenderRequest.inQuality;
	outarRenderRequest6.inPrefetchOnly = inarRenderRequest.inPrefetchOnly;
	outarRenderRequest6.inCompositeOnBlack = inarRenderRequest.inCompositeOnBlack;
	outarRenderRequest6.inCompletionCallback = inarRenderRequest.inCompletionCallback;
	outarRenderRequest6.inCompletionCallbackData = inarRenderRequest.inCompletionCallbackData;
	outarRenderRequest6.inRequestID = inarRenderRequest.inRequestID;
	outarRenderRequest6.inRenderFields = inarRenderRequest.inRenderFields;
	outarRenderRequest6.mRenderContext = inarRenderRequest.mRenderContext;
	outarRenderRequest6.inCaptionStreamFormatToDisplay = inarRenderRequest.inCaptionStreamFormatToDisplay;;
	outarRenderRequest6.inFinalRenderPixelFormat = PrPixelFormat_Invalid;
	outarRenderRequest6.inPrSDKFinalRenderColorSpaceID = kPrSDKColorSpaceID_Invalid;
	outarRenderRequest6.inPrSDKFinalRenderLUTID = kPrSDKLUTID_Invalid;
	outarRenderRequest6.inHardwareResidentFrameOutputSupported = false;
}

static arRenderRequest4 ConvertToArRenderRequest3ToArRenderRequest4(arRenderRequest3& arRenderRequest)
{
	arRenderRequest4 newRenderRequest4;
		
	newRenderRequest4.inTime = arRenderRequest.inTime;
	newRenderRequest4.inWidth = arRenderRequest.inWidth;
	newRenderRequest4.inHeight = arRenderRequest.inHeight;
	newRenderRequest4.inPARNum = arRenderRequest.inPARNum;
	newRenderRequest4.inPARDen = arRenderRequest.inPARDen;
	newRenderRequest4.inPixelFormat = arRenderRequest.inPixelFormat;
	newRenderRequest4.inQuality = arRenderRequest.inQuality;
	newRenderRequest4.inPrefetchOnly = arRenderRequest.inPrefetchOnly;
	newRenderRequest4.inCompositeOnBlack = arRenderRequest.inCompositeOnBlack;
	newRenderRequest4.inCompletionCallback = arRenderRequest.inCompletionCallback;
	newRenderRequest4.inCompletionCallbackData = arRenderRequest.inCompletionCallbackData;
	newRenderRequest4.inRequestID = arRenderRequest.inRequestID;
	newRenderRequest4.inRenderFields = arRenderRequest.inRenderFields;
	newRenderRequest4.mRenderContext = arRenderRequest.mRenderContext;
	newRenderRequest4.inCaptionStreamFormatToDisplay = arRenderRequest.inCaptionStreamFormatToDisplay;
	newRenderRequest4.inFinalRenderPixelFormat = arRenderRequest.inFinalRenderPixelFormat;
	newRenderRequest4.inPrSDKFinalRenderColorSpaceID = kPrSDKColorSpaceID_Invalid;
	return newRenderRequest4;
}
	
static void ConvertToArRenderRequest3ToArRenderRequest5(
	const arRenderRequest3& inarRenderRequest,
	arRenderRequest5& outarRenderRequest5)
{
	outarRenderRequest5.inTime = inarRenderRequest.inTime;
	outarRenderRequest5.inWidth = inarRenderRequest.inWidth;
	outarRenderRequest5.inHeight = inarRenderRequest.inHeight;
	outarRenderRequest5.inPARNum = inarRenderRequest.inPARNum;
	outarRenderRequest5.inPARDen = inarRenderRequest.inPARDen;
	outarRenderRequest5.inPixelFormat = inarRenderRequest.inPixelFormat;
	outarRenderRequest5.inQuality = inarRenderRequest.inQuality;
	outarRenderRequest5.inPrefetchOnly = inarRenderRequest.inPrefetchOnly;
	outarRenderRequest5.inCompositeOnBlack = inarRenderRequest.inCompositeOnBlack;
	outarRenderRequest5.inCompletionCallback = inarRenderRequest.inCompletionCallback;
	outarRenderRequest5.inCompletionCallbackData = inarRenderRequest.inCompletionCallbackData;
	outarRenderRequest5.inRequestID = inarRenderRequest.inRequestID;
	outarRenderRequest5.inRenderFields = inarRenderRequest.inRenderFields;
	outarRenderRequest5.mRenderContext = inarRenderRequest.mRenderContext;
	outarRenderRequest5.inCaptionStreamFormatToDisplay = inarRenderRequest.inCaptionStreamFormatToDisplay;;
	outarRenderRequest5.inFinalRenderPixelFormat = inarRenderRequest.inFinalRenderPixelFormat;;
	outarRenderRequest5.inPrSDKFinalRenderColorSpaceID = kPrSDKColorSpaceID_Invalid;
	outarRenderRequest5.inPrSDKFinalRenderLUTID = kPrSDKLUTID_Invalid;
}

static void ConvertToArRenderRequest3ToArRenderRequest6(
	const arRenderRequest3& inarRenderRequest,
	arRenderRequest6& outarRenderRequest6)
{
	outarRenderRequest6.inTime = inarRenderRequest.inTime;
	outarRenderRequest6.inWidth = inarRenderRequest.inWidth;
	outarRenderRequest6.inHeight = inarRenderRequest.inHeight;
	outarRenderRequest6.inPARNum = inarRenderRequest.inPARNum;
	outarRenderRequest6.inPARDen = inarRenderRequest.inPARDen;
	outarRenderRequest6.inPixelFormat = inarRenderRequest.inPixelFormat;
	outarRenderRequest6.inQuality = inarRenderRequest.inQuality;
	outarRenderRequest6.inPrefetchOnly = inarRenderRequest.inPrefetchOnly;
	outarRenderRequest6.inCompositeOnBlack = inarRenderRequest.inCompositeOnBlack;
	outarRenderRequest6.inCompletionCallback = inarRenderRequest.inCompletionCallback;
	outarRenderRequest6.inCompletionCallbackData = inarRenderRequest.inCompletionCallbackData;
	outarRenderRequest6.inRequestID = inarRenderRequest.inRequestID;
	outarRenderRequest6.inRenderFields = inarRenderRequest.inRenderFields;
	outarRenderRequest6.mRenderContext = inarRenderRequest.mRenderContext;
	outarRenderRequest6.inCaptionStreamFormatToDisplay = inarRenderRequest.inCaptionStreamFormatToDisplay;;
	outarRenderRequest6.inFinalRenderPixelFormat = inarRenderRequest.inFinalRenderPixelFormat;;
	outarRenderRequest6.inPrSDKFinalRenderColorSpaceID = kPrSDKColorSpaceID_Invalid;
	outarRenderRequest6.inPrSDKFinalRenderLUTID = kPrSDKLUTID_Invalid;
	outarRenderRequest6.inHardwareResidentFrameOutputSupported = false;
}

static void ConvertToArRenderRequest4ToArRenderRequest5(
	const arRenderRequest4& inarRenderRequest,
	arRenderRequest5& outarRenderRequest5)
{
	outarRenderRequest5.inTime = inarRenderRequest.inTime;
	outarRenderRequest5.inWidth = inarRenderRequest.inWidth;
	outarRenderRequest5.inHeight = inarRenderRequest.inHeight;
	outarRenderRequest5.inPARNum = inarRenderRequest.inPARNum;
	outarRenderRequest5.inPARDen = inarRenderRequest.inPARDen;
	outarRenderRequest5.inPixelFormat = inarRenderRequest.inPixelFormat;
	outarRenderRequest5.inQuality = inarRenderRequest.inQuality;
	outarRenderRequest5.inPrefetchOnly = inarRenderRequest.inPrefetchOnly;
	outarRenderRequest5.inCompositeOnBlack = inarRenderRequest.inCompositeOnBlack;
	outarRenderRequest5.inCompletionCallback = inarRenderRequest.inCompletionCallback;
	outarRenderRequest5.inCompletionCallbackData = inarRenderRequest.inCompletionCallbackData;
	outarRenderRequest5.inRequestID = inarRenderRequest.inRequestID;
	outarRenderRequest5.inRenderFields = inarRenderRequest.inRenderFields;
	outarRenderRequest5.mRenderContext = inarRenderRequest.mRenderContext;
	outarRenderRequest5.inCaptionStreamFormatToDisplay = inarRenderRequest.inCaptionStreamFormatToDisplay;;
	outarRenderRequest5.inFinalRenderPixelFormat = inarRenderRequest.inFinalRenderPixelFormat;;
	outarRenderRequest5.inPrSDKFinalRenderColorSpaceID = inarRenderRequest.inPrSDKFinalRenderColorSpaceID;
	outarRenderRequest5.inPrSDKFinalRenderLUTID = kPrSDKLUTID_Invalid;
}

static void ConvertToArRenderRequest4ToArRenderRequest6(
	const arRenderRequest4& inarRenderRequest,
	arRenderRequest6& outarRenderRequest6)
{
	outarRenderRequest6.inTime = inarRenderRequest.inTime;
	outarRenderRequest6.inWidth = inarRenderRequest.inWidth;
	outarRenderRequest6.inHeight = inarRenderRequest.inHeight;
	outarRenderRequest6.inPARNum = inarRenderRequest.inPARNum;
	outarRenderRequest6.inPARDen = inarRenderRequest.inPARDen;
	outarRenderRequest6.inPixelFormat = inarRenderRequest.inPixelFormat;
	outarRenderRequest6.inQuality = inarRenderRequest.inQuality;
	outarRenderRequest6.inPrefetchOnly = inarRenderRequest.inPrefetchOnly;
	outarRenderRequest6.inCompositeOnBlack = inarRenderRequest.inCompositeOnBlack;
	outarRenderRequest6.inCompletionCallback = inarRenderRequest.inCompletionCallback;
	outarRenderRequest6.inCompletionCallbackData = inarRenderRequest.inCompletionCallbackData;
	outarRenderRequest6.inRequestID = inarRenderRequest.inRequestID;
	outarRenderRequest6.inRenderFields = inarRenderRequest.inRenderFields;
	outarRenderRequest6.mRenderContext = inarRenderRequest.mRenderContext;
	outarRenderRequest6.inCaptionStreamFormatToDisplay = inarRenderRequest.inCaptionStreamFormatToDisplay;;
	outarRenderRequest6.inFinalRenderPixelFormat = inarRenderRequest.inFinalRenderPixelFormat;;
	outarRenderRequest6.inPrSDKFinalRenderColorSpaceID = inarRenderRequest.inPrSDKFinalRenderColorSpaceID;
	outarRenderRequest6.inPrSDKFinalRenderLUTID = kPrSDKLUTID_Invalid;
	outarRenderRequest6.inHardwareResidentFrameOutputSupported = false;
}

static void ConvertToArRenderRequest5ToArRenderRequest6(
	const arRenderRequest5& inarRenderRequest,
	arRenderRequest6& outarRenderRequest6)
{
	outarRenderRequest6.inTime = inarRenderRequest.inTime;
	outarRenderRequest6.inWidth = inarRenderRequest.inWidth;
	outarRenderRequest6.inHeight = inarRenderRequest.inHeight;
	outarRenderRequest6.inPARNum = inarRenderRequest.inPARNum;
	outarRenderRequest6.inPARDen = inarRenderRequest.inPARDen;
	outarRenderRequest6.inPixelFormat = inarRenderRequest.inPixelFormat;
	outarRenderRequest6.inQuality = inarRenderRequest.inQuality;
	outarRenderRequest6.inPrefetchOnly = inarRenderRequest.inPrefetchOnly;
	outarRenderRequest6.inCompositeOnBlack = inarRenderRequest.inCompositeOnBlack;
	outarRenderRequest6.inCompletionCallback = inarRenderRequest.inCompletionCallback;
	outarRenderRequest6.inCompletionCallbackData = inarRenderRequest.inCompletionCallbackData;
	outarRenderRequest6.inRequestID = inarRenderRequest.inRequestID;
	outarRenderRequest6.inRenderFields = inarRenderRequest.inRenderFields;
	outarRenderRequest6.mRenderContext = inarRenderRequest.mRenderContext;
	outarRenderRequest6.inCaptionStreamFormatToDisplay = inarRenderRequest.inCaptionStreamFormatToDisplay;;
	outarRenderRequest6.inFinalRenderPixelFormat = inarRenderRequest.inFinalRenderPixelFormat;;
	outarRenderRequest6.inPrSDKFinalRenderColorSpaceID = inarRenderRequest.inPrSDKFinalRenderColorSpaceID;
	outarRenderRequest6.inPrSDKFinalRenderLUTID = inarRenderRequest.inPrSDKFinalRenderLUTID;
	outarRenderRequest6.inHardwareResidentFrameOutputSupported = false;
}
	
#define SDKAcceleratedRendererEntryPointName		"xRendererEntry"
typedef PREMPLUGENTRY (* AcceleratedRendererEntryFunc)(int selector, arStdParms* stdParms, void* param1, void* param2);

typedef enum
{
	arSegmentStatus_CannotRender = 0,
	arSegmentStatus_CanRender_NonRealTimeUnspecified = 1,
	arSegmentStatus_CanRender_RealTime = 2,
	arSegmentStatus_CanRender_RealTimeWithMismatch = 3,
	arSegmentStatus_CanRender_RealTimeCached = 4,

	arSegmentStatus_CanRender = arSegmentStatus_CanRender_NonRealTimeUnspecified
} arSegmentStatus;

enum
{
	arTimelineFlag_UseExistingPreviewFiles = 0x1 //A sequence will be setup twice, once with this flag and once without
};

// Selectors
enum PrRendererSelector
{
	arSelector_Startup = 0, //arRendererInfo, unused
	arSelector_Shutdown = 1,//unused, unused
	
	arSelector_SequenceSetup = 2, //arSequenceData, unused
	arSelector_SequenceSetdown = 3,//arSequenceData, unused
	
	arSelector_IterateVideoSegments = 4,//arSequenceData, arVideoSegmentInfo
	arSelector_IterateVideoSegmentPixelFormats = 5,//arSequenceData, arVideoSegmentPixelFormat
	
	arSelector_InitiateFrameRender = 6,//arSequenceData, arRenderRequest
	arSelector_CancelFrameRender = 7,//arSequenceData, renderRequestID
	arSelector_CancelAllFrameRenders = 8,//arSequenceData, unused

	arSelector_SetDisplayStateProperties = 9,//arSequenceData, const pmDisplayStateProperties*

	arSelector_InitiateFrameRender2 = 10,//arSequenceData, arRenderRequest2 (with Caption format to render)
	arSelector_SetDisplayStateProperties2 = 11,//arSequenceData, const pmDisplayStateProperties2* (with Caption format to render)
	arSelector_InitiateFrameRender3 = 12, //arRenderRequest3 (with final pixel format to render to )
	arSelector_InitiateFrameRender4 = 13, //arRenderRequest4 (with ColorSpace to render to ) new in Pr 14.0
	arSelector_InitiateFrameRender5 = 14, //arRenderRequest5 with export LUT ID support
	arSelector_StartRenderSession = 15, //unused, unused (this is an optional selector)
	arSelector_FinishRenderSession = 16, //unused, unused (this is an optional selector)
	arSelector_InitiateFrameRender6 = 17 // Added to support hardware resident frame
};

// Return codes
enum PrRendererReturn
{
	arReturn_Success = 0,
	
	arReturn_ContinueIterateRenderers = 1, //used during Startup to support multiple renderers in a single plugin
	arReturn_ContinueIterateVideoSegments = 2,
	arReturn_ContinueIteratePixelFormats = 5,
	
	arReturn_ErrorUnknown = -1,
	arReturn_ErrorMemory = -2,
	arReturn_ErrorHost = -3,
	arReturn_ErrorUnsupported = -4,
	arReturn_ErrorSelectorUnimplemented = -5
};

#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif //PRSDKACCELERATEDRENDER_H
