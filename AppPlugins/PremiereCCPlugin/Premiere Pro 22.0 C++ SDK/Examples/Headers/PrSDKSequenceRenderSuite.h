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

//	This file is part of the public MediaCore SDK.


#ifndef	PRSDKSEQUENCERENDERSUITE_H
#define PRSDKSEQUENCERENDERSUITE_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKPIXELFORMAT_H
#include "PrSDKPixelFormat.h"
#endif

#ifndef PRSDKMALERRORS_H
#include "PrSDKMALErrors.h"
#endif

#ifndef	PRSDKRENDERCACHETYPE_H
#include "PrSDKRenderCacheType.h"
#endif

#ifndef PRSDKQUALITY_H
#include "PrSDKQuality.h"
#endif

#ifndef PRSDKSTREAMLABEL_H
#include "PrSDKStreamLabel.h"
#endif
#ifndef PRSDKTIMESUITE_H
#include "PrSDKTimeSuite.h"
#endif

#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif

// Version History
// version 1 - started for CS4, no longer supported
// version 2 - removed an argument from RenderVideoFrame in version 1, as it was unused
#define kPrSDKSequenceRenderSuite				"MediaCore Sequence Render Suite"
#define kPrSDKSequenceRenderSuiteVersion1		1
#define kPrSDKSequenceRenderSuiteVersion2		2
#define kPrSDKSequenceRenderSuiteVersion3		3
#define kPrSDKSequenceRenderSuiteVersion4		4
#define kPrSDKSequenceRenderSuiteVersion5		5
#define kPrSDKSequenceRenderSuiteVersion6		6	// Added support of export LUT
#define kPrSDKSequenceRenderSuiteVersion		kPrSDKSequenceRenderSuiteVersion6


typedef struct
{
	csSDK_int32		repeatCount;	// The number of contiguous frames from this file.
} SequenceRender_FrameInfoRec;

// This is info passed back in response to calling GetFrame or AsyncGetFrame

typedef struct
{
	void*		asyncCompletionData;// data passed to QueueAsyncVideoFrameRender
	csSDK_int32	returnVal;			// SequenceRenderReturn_ErrNone - no error
										// SequenceRenderReturn_Abort	- user aborted operation
										// SequenceRenderReturn_Done	- operation finished
										// ... or an error code
	csSDK_int32	repeatCount;		// The frame should be repeated for repeatCount frames
										// in the output files (write null frames, change the frame
										// duration or whatever is appropriate)
	csSDK_int32	onMarker;			// There is a marker on this frame.
	PPixHand	outFrame;			// filled during RenderVideoFrame, 
} SequenceRender_GetFrameReturnRec;


typedef struct
{
	const PrPixelFormat*	inRequestedPixelFormatArray;																	
	csSDK_int32			inRequestedPixelFormatArrayCount;
	csSDK_int32			inWidth;
	csSDK_int32			inHeight;
	csSDK_int32			inPixelAspectRatioNumerator;
	csSDK_int32			inPixelAspectRatioDenominator;
	PrRenderQuality		inRenderQuality;
	prFieldType			inFieldType;
	csSDK_int32			inDeinterlace;
	PrRenderQuality		inDeinterlaceQuality;
	csSDK_int32			inCompositeOnBlack;
} SequenceRender_ParamsRec;
	
typedef struct
{
	const PrPixelFormat*	inRequestedPixelFormatArray;
	csSDK_int32			inRequestedPixelFormatArrayCount;
	csSDK_int32			inWidth;
	csSDK_int32			inHeight;
	csSDK_int32			inPixelAspectRatioNumerator;
	csSDK_int32			inPixelAspectRatioDenominator;
	PrRenderQuality		inRenderQuality;
	prFieldType			inFieldType;
	csSDK_int32			inDeinterlace;
	PrRenderQuality		inDeinterlaceQuality;
	csSDK_int32			inCompositeOnBlack;
	PrSDKColorSpaceID	inPrSDKColorSpaceID;	// new in Pr 13.0
} SequenceRender_ParamsRecExt;

typedef struct
{
	const PrPixelFormat*	inRequestedPixelFormatArray;
	csSDK_int32			inRequestedPixelFormatArrayCount;
	csSDK_int32			inWidth;
	csSDK_int32			inHeight;
	csSDK_int32			inPixelAspectRatioNumerator;
	csSDK_int32			inPixelAspectRatioDenominator;
	PrRenderQuality		inRenderQuality;
	prFieldType			inFieldType;
	csSDK_int32			inDeinterlace;
	PrRenderQuality		inDeinterlaceQuality;
	csSDK_int32			inCompositeOnBlack;
	PrSDKColorSpaceID	inPrSDKColorSpaceID;	// new in Pr 13.0
	PrSDKLUTID			inPrSDKLUTID;			// Added to support export LUT
} SequenceRender_ParamsRecExt2;


// Callback used for async frame notification, passed to RegisterAsyncGetFrameCallback
// Error status (error or abort) is returned in getFrameReturn
typedef void (*PrSDKSequenceAsyncRenderCompletionProc)(
	csSDK_uint32 inVideoRenderID,
	void* inCallbackRef, 
	PrTime inTime, 
	PPixHand inRenderedFrame,								// ppixSuite->Dispose() must be called on this buffer
															// to release its memory
	SequenceRender_GetFrameReturnRec *inGetFrameReturn);

// suite
typedef struct 
{
	//
	prSuiteError (*MakeVideoRenderer)(
		csSDK_uint32 pluginID,	// id from exporter or player
		csSDK_uint32 *outVideoRenderID,
		PrTime inFrameRate);

	//
	prSuiteError (*ReleaseVideoRenderer)(
		csSDK_uint32 pluginID,
		csSDK_uint32 inVideoRenderID);


	// GetFrame returns:
	//		export_ErrNone - continue compiling
	//		export_Abort - user aborted the compile
	//		export_Done - finished the compile
	//		... or an error code

	prSuiteError (*RenderVideoFrame)(
				csSDK_uint32				inVideoRenderID,
				PrTime						inTime, 
				SequenceRender_ParamsRec*	inRenderParams,
				PrRenderCacheType			inCacheFlags,
				SequenceRender_GetFrameReturnRec*	getFrameReturn);


	prSuiteError (*QueueAsyncVideoFrameRender)(
				csSDK_uint32				inVideoRenderID,
				PrTime						inTime, 
				csSDK_uint32*				outRequestID,
				SequenceRender_ParamsRec*	inRenderParams,
				PrRenderCacheType			inCacheFlags,
				void*						inAsyncCompletionData);


	// Register a notification callback for retrieving async frames
	prSuiteError (*SetAsyncRenderCompletionProc)(
		csSDK_uint32							inVideoRenderID,
		PrSDKSequenceAsyncRenderCompletionProc	asyncGetFrameCallback, 
		void*									callbackRef);

	// GetFrameInfo - Get information about a given frame
	prSuiteError (*GetFrameInfo)(
		csSDK_uint32					inVideoRenderID,
		PrTime							inTime, 
		SequenceRender_FrameInfoRec*	outFrameInfo);

	/**
	**	Pre-fetches the media needed to render this frame. 
	**	This is a hint to the importers to begin reading media needed to render this video frame.
	**
	**	@param	inFrame								The video frame number.
	*/
	prSuiteError (*PrefetchMedia)(
		csSDK_uint32	inVideoRenderID,
		PrTime			inFrame);

	/**
	**	Pre-fetches the media needed to render this frame, using all of the parameters used to render the frame. 
	**	This is a hint to the importers to begin reading media needed to render this video frame.
	**
	**	@param	inFrameTime							The time of the video frame number.
	**
	**	@param	inRequestedPixelFormatArray			An array of PrPixelFormats that list your format preferences in order.
	**												This list must end with PrPixelFormat_BGRA_4444_8u.
	**	@param	inRequestedPixelFormatArrayCount	Number of formats in the format array
	**	@param	inFrameRect							Video frame size
	**	@param	inPixelAspectRatioNumerator			The numerator for the pixel aspect ratio.
	**	@param	inPixelAspectRatioDenominator		The denominator for the pixel aspect ratio.										 
	**	@param	inRenderQuality						The render quality of this frame.
	*/
	prSuiteError (*PrefetchMediaWithRenderParameters)(
		csSDK_uint32				inVideoRenderID,
		PrTime						inTime,
		SequenceRender_ParamsRec*	inRenderParams);

	/**
	**	This will cancel all media pre-fetches that are still outstanding. 
	*/
	prSuiteError (*CancelAllOutstandingMediaPrefetches)(
		csSDK_uint32			inVideoRenderID);

	/**
	**	Is all the prefetched media ready?
	*/
	prSuiteError (*IsPrefetchedMediaReady)(
		csSDK_uint32			inVideoRenderID,
		PrTime					inTime,
		prBool*					outMediaReady);

	/**
	**
	*/
	prSuiteError (*MakeVideoRendererForTimeline)(
		PrTimelineID inTimeline,
		csSDK_uint32* outVideoRendererID);

	/**
	**
	*/
	prSuiteError (*MakeVideoRendererForTimelineWithFrameRate)(
		PrTimelineID inTimeline,
		PrTime inFrameRate,
		csSDK_uint32* outVideoRendererID);
		
	/**
	**
	*/
	prSuiteError (*ReleaseVideoRendererForTimeline)(
		csSDK_uint32 inVideoRendererID);

	/**
	**
	*/
	prSuiteError (*RenderVideoFrameAndConformToPixelFormat)(
				csSDK_uint32				inVideoRenderID,
				PrTime						inTime, 
				SequenceRender_ParamsRec*	inRenderParams,
				PrRenderCacheType			inCacheFlags,
				PrPixelFormat				inConformToFormat,
				SequenceRender_GetFrameReturnRec*	getFrameReturn);
	
	/**
	**
	*/
	prSuiteError (*MakeVideoRendererForTimelineWithStreamLabel)(
		PrTimelineID inTimeline,
		PrSDKStreamLabel inStreamLabel,
		csSDK_uint32* outVideoRendererID);
	
	/*
	**
	*/
	prSuiteError (*RenderColorManagedVideoFrame)(
		csSDK_uint32					inVideoRenderID,
		PrTime							inTime,
		SequenceRender_ParamsRecExt*	inRenderParamsExt,
		PrRenderCacheType				inCacheFlags,
		SequenceRender_GetFrameReturnRec*	getFrameReturn);
	
	/*
	**
	*/
	prSuiteError (*QueueAsyncColorManagedVideoFrameRender)(
		csSDK_uint32					inVideoRenderID,
		PrTime							inTime,
		csSDK_uint32*					outRequestID,
		SequenceRender_ParamsRecExt*	inRenderParamsExt,
		PrRenderCacheType				inCacheFlags,
		void*							inAsyncCompletionData);
	
	/*
	**
	*/
	prSuiteError (*PrefetchColorManagedMedia)(
		csSDK_uint32		inVideoRenderID,
		PrTime				inFrame,
		PrSDKColorSpaceID inPrSDKColorSpaceID);
	
	/*
	**
	*/
	prSuiteError (*PrefetchColorManagedMediaWithRenderParameters)(
		csSDK_uint32					inVideoRenderID,
		PrTime							inTime,
		SequenceRender_ParamsRecExt*	inRenderParamsExt);
	
	/*
	**
	*/
	prSuiteError (*RenderColorManagedVideoFrameAndConformToPixelFormat)(
		csSDK_uint32					inVideoRenderID,
		PrTime							inTime,
		SequenceRender_ParamsRecExt*	inRenderParamsExt,
		PrRenderCacheType				inCacheFlags,
		PrPixelFormat					inConformToFormat,
		SequenceRender_GetFrameReturnRec*	getFrameReturn);

	/*
	**
	*/
	prSuiteError (*RenderColorManagedVideoFrame2)(
		csSDK_uint32                    inVideoRenderID,
		PrTime                          inTime,
		SequenceRender_ParamsRecExt2*   inRenderParamsExt2,
		PrRenderCacheType               inCacheFlags,
		SequenceRender_GetFrameReturnRec*   outGetFrameReturn);
	
	/*
	**
	*/
	prSuiteError (*QueueAsyncColorManagedVideoFrameRender2)(
		csSDK_uint32                    inVideoRenderID,
		PrTime                          inTime,
		csSDK_uint32*                   outRequestID,
		SequenceRender_ParamsRecExt2*   inRenderParamsExt2,
		PrRenderCacheType               inCacheFlags,
		void*                           inAsyncCompletionData);
	
	/*
	**
	*/
	prSuiteError(*PrefetchColorManagedMediaWithRenderParameters2)(
		csSDK_uint32                    inVideoRenderID,
		PrTime                          inTime,
		SequenceRender_ParamsRecExt2*   inRenderParamsExt2);

	/*
	**
	*/
	prSuiteError (*RenderColorManagedVideoFrameAndConformToPixelFormat2)(
		csSDK_uint32                    inVideoRenderID,
		PrTime                          inTime,
		SequenceRender_ParamsRecExt2*   inRenderParamsExt2,
		PrRenderCacheType               inCacheFlags,
		PrPixelFormat                   inConformToFormat,
		SequenceRender_GetFrameReturnRec*   outGetFrameReturn);

} PrSDKSequenceRenderSuite;

#ifdef __cplusplus
}
#endif // __cplusplus

#pragma pack(pop)

#endif // PRSDKSEQUENCERENDERSUITE_H

