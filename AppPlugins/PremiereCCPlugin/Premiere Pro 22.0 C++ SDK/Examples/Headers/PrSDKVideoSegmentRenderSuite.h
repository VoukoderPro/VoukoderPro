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

//	This file is part of the public Premiere SDK.


#ifndef	PRVIDEOSEGMENTRENDERSUITE_H
#define PRVIDEOSEGMENTRENDERSUITE_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKMALERRORS_H
#include "PrSDKMALErrors.h"
#endif

#ifndef	PRSDKIMPORTERSHARED_H
#include "PrSDKImporterShared.h"
#endif

#ifndef	PRSDKSEQUENCERENDERSUITE_H
#include "PrSDKSequenceRenderSuite.h"
#endif

#ifndef PRSDKCAPTIONSTREAMFORMAT_H
#include "PrSDKCaptionStreamFormat.h"
#endif

#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif

#define kPrSDKVideoSegmentRenderSuite				"MediaCore Video Segment Render Suite"
#define kPrSDKVideoSegmentRenderSuiteVersion5		5	// 11.0
#define kPrSDKVideoSegmentRenderSuiteVersion6		6	// 13.0, color managed and captioning version of APIs
#define kPrSDKVideoSegmentRenderSuiteVersion		kPrSDKVideoSegmentRenderSuiteVersion6

typedef void (*PrSDKVideoSegmentAsyncRenderCompletionProc)(
	PPixHand inRenderedFrame,								// ppixSuite->Dispose() must be called on this buffer to release its memory
	csSDK_int64 inCompletionData,
	prSuiteError inResult);

typedef struct 
{
	/**
	** For a given node, in a given timeline, render the frame that that node would normally produce, given an overriding frame rect.
	** For any of the overrides, you can pass zero, implying that you don't want to override at all. This is useful when rendering the inputs to a MulticamNode
	** Note that the resulting frame may not match any of your requested pixel formats.
	** 
	** @param inTimelineID - the timeline identifier provided by the host
	** @param inNodeID - the node you would like to render. Effect nodes are not suitable for this call, but all other node types will work fine.
	**						In general, inputs will work, operators will not.
	** @param inSequenceTime - the time you want rendered, in the containing timeline. This is used for filters that request other media during their render
	** @param inSegmentTime - the time you want rendered, relative to the node's concept of time, not the containing timeline.
	** @param inFrameRateScale - the frame rate you want rendered. In general, this should be the framerate of the timeline, but any can be provided.
	** @param inFrameRateSampleSize - see above.
	** @param inSequenceWidth - the overridden sequence width
	** @param inSequenceHeight - the overridden sequence height
	** @param inSequencePixelAspectRatioNumerator - the overridden sequence PAR
	** @param inSequencePixelAspectRatioDenominator - the overridden sequence PAR
	** @param inRenderParams - the same set of render params used in the SequenceRenderSuite. The option to composite on black is ignored.
	** @param inCompletionProc - the callback that will be called when the render is complete.
	** @param inAsyncCompletionData - an extra param that will be provided to the completion routine.
	** @param outRequestID - an identifier that can be used to cancel this request. It isn't really useful for anything else since your 
	**						completion routine can be called before this one returns.
	*/
	prSuiteError (*ProduceFrameAsync)(
		PrTimelineID inTimelineID,
		csSDK_int32 inNodeID,
		PrTime inSequenceTime,
		PrTime inSegmentTime,
		PrTime inSequenceTicksPerFrame,
		csSDK_int32 inSequenceWidth,
		csSDK_int32 inSequenceHeight,
		csSDK_int32 inSequencePixelAspectRatioNumerator,
		csSDK_int32 inSequencePixelAspectRatioDenominator,
		const SequenceRender_ParamsRec* inRenderParams,
		PrSDKVideoSegmentAsyncRenderCompletionProc inCompletionProc,
		csSDK_int64 inAsyncCompletionData,
		csSDK_int32* outRequestID);

	/**
	**	A matching function for ProduceFrameAsync. This allows you to check the cache for this frame
	**	prior to requesting it to be rendered.
	*/
	prSuiteError (*GetIdentifierForProduceFrameAsync)(
		PrTimelineID inTimelineID,
		csSDK_int32 inNodeID,
		PrTime inSequenceTime,
		PrTime inSegmentTime,
		PrTime inSequenceTicksPerFrame,
		csSDK_int32 inSequenceWidth,
		csSDK_int32 inSequenceHeight,
		csSDK_int32 inSequencePixelAspectRatioNumerator,
		csSDK_int32 inSequencePixelAspectRatioDenominator,
		const SequenceRender_ParamsRec* inRenderParams,
		prPluginID* outIdentifier);

	/**
	** For a given operator, in a given timeline, with a provided input frame, render the effect asynchronously.
	** For any of the overrides, you can pass zero, implying that you don't want to override at all. This is useful when rendering the inputs to a MulticamNode
	** Note that the resulting frame may not match any of your requested pixel formats.
	** 
	** @param inTimelineID - the timeline identifier provided by the host
	** @param inClipNodeID - the Clip that contains the operator you want to render
	** @param inOperatorStartIndex - the zero-based index of the operator you want to start rendering at
	** @param inOperatorCount - the number of operators you want to apply
	** @param inSequenceTime - the time you want rendered, in the containing timeline. This is used for filters that request other media during their render
	** @param inSegmentTime - the time you want rendered, relative to the node's concept of time, not the containing timeline.
	** @param inFrameRateScale - the frame rate you want rendered. In general, this should be the framerate of the timeline, but any can be provided.
	** @param inFrameRateSampleSize - see above.
	** @param inSequenceWidth - the overridden sequence width
	** @param inSequenceHeight - the overridden sequence height
	** @param inSequencePixelAspectRatioNumerator - the overridden sequence PAR
	** @param inSequencePixelAspectRatioDenominator - the overridden sequence PAR
	** @param inInputFrame - can be NULL if inOperatorStartIndex is 0, implies that you want the host to provide the clip node's input frame
	** @param inRenderParams - the same set of render params used in the SequenceRenderSuite. The option to composite on black is ignored.
	** @param inCompletionProc - the callback that will be called when the render is complete.
	** @param inAsyncCompletionData - an extra param that will be provided to the completion routine.
	** @param outRequestID - an identifier that can be used to cancel this request. It isn't really useful for anything else since your 
	**						completion routine can be called before this one returns.
	*/
	prSuiteError (*ApplyOperatorsToFrameAsync)(
		PrTimelineID inTimelineID,
		csSDK_int32 inClipNodeID,
		csSDK_int32 inOperatorStartIndex,
		csSDK_int32 inOperatorCount,
		PrTime inSequenceTime,
		PrTime inSegmentTime,
		PrTime inSequenceTicksPerFrame,
		csSDK_int32 inSequenceWidth,
		csSDK_int32 inSequenceHeight,
		csSDK_int32 inSequencePixelAspectRatioNumerator,
		csSDK_int32 inSequencePixelAspectRatioDenominator,
		PPixHand inInputFrame,
		const SequenceRender_ParamsRec* inRenderParams,
		PrSDKVideoSegmentAsyncRenderCompletionProc inCompletionProc,
		csSDK_int64 inAsyncCompletionData,
		csSDK_int32* outRequestID);

	/**
	**	A matching function for ApplyOperatorsToFrameAsync. This allows you to check the cache for this frame
	**	prior to requesting it to be rendered.
	*/
	prSuiteError (*GetIdentifierForApplyOperatorsToFrameAsync)(
		PrTimelineID inTimelineID,
		csSDK_int32 inClipNodeID,
		csSDK_int32 inOperatorStartIndex,
		csSDK_int32 inOperatorCount,
		PrTime inSequenceTime,
		PrTime inSegmentTime,
		PrTime inSequenceTicksPerFrame,
		csSDK_int32 inSequenceWidth,
		csSDK_int32 inSequenceHeight,
		csSDK_int32 inSequencePixelAspectRatioNumerator,
		csSDK_int32 inSequencePixelAspectRatioDenominator,
		PPixHand inInputFrame,
		const SequenceRender_ParamsRec* inRenderParams,
		prPluginID* outIdentifier);

	/**
	** For a given transition, in a given timeline, with two (optional) provided input frames, render the transition.
	** Note that the resulting frame may not match any of your requested pixel formats.
	** 
	** @param inTimelineID - the timeline identifier provided by the host
	** @param inTransitionNodeID - Only transition nodes are acceptable here.
	** @param inSequenceTime - the time you want rendered, in the containing timeline. This is used for filters that request other media during their render
	** @param inSegmentTime - the time you want rendered, relative to the node's concept of time, not the containing timeline.
	** @param inFrameRateScale - the frame rate you want rendered. In general, this should be the framerate of the timeline, but any can be provided.
	** @param inFrameRateSampleSize - see above.
	** @param inOutgoingInputFrame  - if the PPixHand is NULL, then transparent black will be used
	** @param inIncomingInputFrame - if the PPixHand is NULL, then transparent black will be used
	** @param inRenderParams - the same set of render params used in the SequenceRenderSuite. The option to composite on black is ignored.
	** @param inCompletionProc - the callback that will be called when the render is complete.
	** @param inAsyncCompletionData - an extra param that will be provided to the completion routine.
	** @param outRequestID - an identifier that can be used to cancel this request. It isn't really useful for anything else since your 
	**						completion routine can be called before this one returns.
	*/
	prSuiteError (*ApplyTransitionToFrameAsync)(
		PrTimelineID inTimelineID,
		csSDK_int32 inTransitionNodeID,
		PrTime inSequenceTime,
		PrTime inSegmentTime,
		PrTime inSequenceTicksPerFrame,
		PPixHand inOutgoingInputFrame,
		PPixHand inIncomingInputFrame,
		const SequenceRender_ParamsRec* inRenderParams,
		PrSDKVideoSegmentAsyncRenderCompletionProc inCompletionProc,
		csSDK_int64 inAsyncCompletionData,
		csSDK_int32* outRequestID);

	/**
	**	A matching function for ApplyOperatorsToFrameAsync. This allows you to check the cache for this frame
	**	prior to requesting it to be rendered.
	*/
	prSuiteError (*GetIdentifierForApplyTransitionToFrameAsync)(
		PrTimelineID inTimelineID,
		csSDK_int32 inTransitionNodeID,
		PrTime inSequenceTime,
		PrTime inSegmentTime,
		PrTime inSequenceTicksPerFrame,
		PPixHand inOutgoingInputFrame,
		PPixHand inIncomingInputFrame,
		const SequenceRender_ParamsRec* inRenderParams,
		prPluginID* outIdentifier);

	/**
	** For a given clip (the ClipID is a property of a media node, if applicable), determine the best possible match for a given frame descriptor
	**
	** @param inClipID - a clip ID retrieved from a media node
	** @param inClipTime - it's possible for a source to vary it's answer over time
	** @param inDesiredClipFrameDescriptor - the size, par, quality and pixel format you're looking for. You can leave the size or par at 0 if you just want the native size returned.
	** @param outBestFrameDescriptor - the closest match available. It might not be all that close a match, but it's what we can get from the importer/source
	*/
	prSuiteError (*SelectClipFrameDescriptor)(
		PrClipID inClipID,
		PrTime inClipTime,
		const ClipFrameDescriptor* inDesiredClipFrameDescriptor,
		ClipFrameDescriptor* outBestFrameDescriptor);

	/**
	** For a given clip, at a given time, start a prefetch.
	**
	** @param inClipID - a clip ID retrieved from a media node
	** @param inRequestedFrameDescriptor - a frame descriptor which was returned from a call to SelectClipFrameDescriptor
	** @param inMediaTime - the time, in the media space, that you wish to request
	** @param inCompletionProc - the callback that will be called when the prefetch is complete.
	** @param inAsyncCompletionData - an extra param that will be provided to the completion routine.
	** @param outRequestID - an identifier that can be used to cancel this request. It isn't really useful for anything else since your 
	**						completion routine can be called before this one returns.
	*/
	prSuiteError (*InitiateClipPrefetch)(
		PrClipID inClipID,
		const ClipFrameDescriptor* inRequestedFrameDescriptor,
		PrTime inMediaTime,
		PrSDKVideoSegmentAsyncRenderCompletionProc inCompletionProc,
		csSDK_int64 inAsyncCompletionData,
		csSDK_int32* outRequestID);

	/**
	**	A matching function for InitiateClipPrefetch. This allows you to check the cache for this frame
	**	prior to requesting it to be rendered.
	*/
	prSuiteError (*GetIdentifierForInitiateClipPrefetch)(
		PrClipID inClipID,
		const ClipFrameDescriptor* inRequestedFrameDescriptor,
		PrTime inMediaTime,
		prPluginID* outIdentifier);
		
	/**
	**
	*/
	prSuiteError (*CancelAsyncRequest)(
		csSDK_int32 inRequestID);

	/**
	**
	*/
	prSuiteError (*SupportsInitiateClipPrefetch)(
		PrClipID inClipID,
		prBool* outSupported);

	/**
	** For a given node, in a given timeline, render the frame that that node would normally produce, given an overriding frame rect.
	** For any of the overrides, you can pass zero, implying that you don't want to override at all. This is useful when rendering the inputs to a MulticamNode
	** Note that the resulting frame may not match any of your requested pixel formats.
	** 
	** @param inTimelineID - the timeline identifier provided by the host
	** @param inNodeID - the node you would like to render. Effect nodes are not suitable for this call, but all other node types will work fine.
	**						In general, inputs will work, operators will not.
	** @param inSequenceTime - the time you want rendered, in the containing timeline. This is used for filters that request other media during their render
	** @param inSegmentTime - the time you want rendered, relative to the node's concept of time, not the containing timeline.
	** @param inFrameRateScale - the frame rate you want rendered. In general, this should be the framerate of the timeline, but any can be provided.
	** @param inFrameRateSampleSize - see above.
	** @param inSequenceWidth - the overridden sequence width
	** @param inSequenceHeight - the overridden sequence height
	** @param inSequencePixelAspectRatioNumerator - the overridden sequence PAR
	** @param inSequencePixelAspectRatioDenominator - the overridden sequence PAR
	** @param inRenderParams - the same set of render params used in the SequenceRenderSuite. The option to composite on black is ignored.
	** @param inRenderContext - the context in which the render is occurring
	** @param inCompletionProc - the callback that will be called when the render is complete.
	** @param inAsyncCompletionData - an extra param that will be provided to the completion routine.
	** @param outRequestID - an identifier that can be used to cancel this request. It isn't really useful for anything else since your 
	**						completion routine can be called before this one returns.
	*/
	prSuiteError (*ProduceFrameAsync2)(
		PrTimelineID inTimelineID,
		csSDK_int32 inNodeID,
		PrTime inSequenceTime,
		PrTime inSegmentTime,
		PrTime inSequenceTicksPerFrame,
		csSDK_int32 inSequenceWidth,
		csSDK_int32 inSequenceHeight,
		csSDK_int32 inSequencePixelAspectRatioNumerator,
		csSDK_int32 inSequencePixelAspectRatioDenominator,
		const SequenceRender_ParamsRec* inRenderParams,
		imRenderContext inRenderContext,
		PrSDKVideoSegmentAsyncRenderCompletionProc inCompletionProc,
		csSDK_int64 inAsyncCompletionData,
		csSDK_int32* outRequestID);

	/**
	** For a given clip, at a given time, start a prefetch.
	**
	** @param inClipID - a clip ID retrieved from a media node
	** @param inRequestedFrameDescriptor - a frame descriptor which was returned from a call to SelectClipFrameDescriptor
	** @param inMediaTime - the time, in the media space, that you wish to request
	** @param inRenderContext - the context in which the render is occurring
	** @param inCompletionProc - the callback that will be called when the prefetch is complete.
	** @param inAsyncCompletionData - an extra param that will be provided to the completion routine.
	** @param outRequestID - an identifier that can be used to cancel this request. It isn't really useful for anything else since your 
	**						completion routine can be called before this one returns.
	*/
	prSuiteError (*InitiateClipPrefetch2)(
		PrClipID inClipID,
		const ClipFrameDescriptor* inRequestedFrameDescriptor,
		PrTime inMediaTime,
		imRenderContext inRenderContext,
		PrSDKVideoSegmentAsyncRenderCompletionProc inCompletionProc,
		csSDK_int64 inAsyncCompletionData,
		csSDK_int32* outRequestID);

	/**
	** For a given operator, in a given timeline, with a provided input frame, render the effect asynchronously.
	** For any of the overrides, you can pass zero, implying that you don't want to override at all. This is useful when rendering the inputs to a MulticamNode
	** Note that the resulting frame may not match any of your requested pixel formats.
	** 
	** @param inTimelineID - the timeline identifier provided by the host
	** @param inClipNodeID - the Clip that contains the operator you want to render
	** @param inOperatorStartIndex - the zero-based index of the operator you want to start rendering at
	** @param inOperatorCount - the number of operators you want to apply
	** @param inSequenceTime - the time you want rendered, in the containing timeline. This is used for filters that request other media during their render
	** @param inSegmentTime - the time you want rendered, relative to the node's concept of time, not the containing timeline.
	** @param inFrameRateScale - the frame rate you want rendered. In general, this should be the framerate of the timeline, but any can be provided.
	** @param inFrameRateSampleSize - see above.
	** @param inSequenceWidth - the overridden sequence width
	** @param inSequenceHeight - the overridden sequence height
	** @param inSequencePixelAspectRatioNumerator - the overridden sequence PAR
	** @param inSequencePixelAspectRatioDenominator - the overridden sequence PAR
	** @param inInputFrame - can be NULL if inOperatorStartIndex is 0, implies that you want the host to provide the clip node's input frame
	** @param inRenderParams - the same set of render params used in the SequenceRenderSuite. The option to composite on black is ignored.
	** @param inRenderContext - the context in which the render is occurring
	** @param inCompletionProc - the callback that will be called when the render is complete.
	** @param inAsyncCompletionData - an extra param that will be provided to the completion routine.
	** @param outRequestID - an identifier that can be used to cancel this request. It isn't really useful for anything else since your 
	**						completion routine can be called before this one returns.
	*/
	prSuiteError (*ApplyOperatorsToFrameAsync2)(
		PrTimelineID inTimelineID,
		csSDK_int32 inClipNodeID,
		csSDK_int32 inOperatorStartIndex,
		csSDK_int32 inOperatorCount,
		PrTime inSequenceTime,
		PrTime inSegmentTime,
		PrTime inSequenceTicksPerFrame,
		csSDK_int32 inSequenceWidth,
		csSDK_int32 inSequenceHeight,
		csSDK_int32 inSequencePixelAspectRatioNumerator,
		csSDK_int32 inSequencePixelAspectRatioDenominator,
		PPixHand inInputFrame,
		const SequenceRender_ParamsRec* inRenderParams,
		imRenderContext inRenderContext,
		PrSDKVideoSegmentAsyncRenderCompletionProc inCompletionProc,
		csSDK_int64 inAsyncCompletionData,
		csSDK_int32* outRequestID);

	/**
	** For a given node, in a given timeline, render the frame that that node would normally produce, given an overriding frame rect.
	** For any of the overrides, you can pass zero, implying that you don't want to override at all. This is useful when rendering the inputs to a MulticamNode
	** Note that the resulting frame may not match any of your requested pixel formats.
	**
	** @param inTimelineID - the timeline identifier provided by the host
	** @param inNodeID - the node you would like to render. Effect nodes are not suitable for this call, but all other node types will work fine.
	**						In general, inputs will work, operators will not.
	** @param inSequenceTime - the time you want rendered, in the containing timeline. This is used for filters that request other media during their render
	** @param inSegmentTime - the time you want rendered, relative to the node's concept of time, not the containing timeline.
	** @param inFrameRateScale - the frame rate you want rendered. In general, this should be the framerate of the timeline, but any can be provided.
	** @param inFrameRateSampleSize - see above.
	** @param inSequenceWidth - the overridden sequence width
	** @param inSequenceHeight - the overridden sequence height
	** @param inSequencePixelAspectRatioNumerator - the overridden sequence PAR
	** @param inSequencePixelAspectRatioDenominator - the overridden sequence PAR
	** @param inRenderParams - the same set of render params used in the SequenceRenderSuite. The option to composite on black is ignored.
	** @param inRenderContext - the context in which the render is occurring
	** @param inCompletionProc - the callback that will be called when the render is complete.
	** @param inAsyncCompletionData - an extra param that will be provided to the completion routine.
	** @param inBypassEffects - if true, skip non-intrinsic video effects while rendering
	** @param outRequestID - an identifier that can be used to cancel this request. It isn't really useful for anything else since your
	**						completion routine can be called before this one returns.
	*/
	prSuiteError(*ProduceFrameAsync3)(
		PrTimelineID inTimelineID,
		csSDK_int32 inNodeID,
		PrTime inSequenceTime,
		PrTime inSegmentTime,
		PrTime inSequenceTicksPerFrame,
		csSDK_int32 inSequenceWidth,
		csSDK_int32 inSequenceHeight,
		csSDK_int32 inSequencePixelAspectRatioNumerator,
		csSDK_int32 inSequencePixelAspectRatioDenominator,
		const SequenceRender_ParamsRec* inRenderParams,
		imRenderContext inRenderContext,
		PrSDKVideoSegmentAsyncRenderCompletionProc inCompletionProc,
		csSDK_int64 inAsyncCompletionData,
		prBool inBypassEffects, 
		csSDK_int32* outRequestID);
		
	/**
	**	A matching function for ProduceFrameAsync. This allows you to check the cache for this frame
	**	prior to requesting it to be rendered.
	*/
	prSuiteError(*GetIdentifierForProduceFrameAsync2)(
		PrTimelineID inTimelineID,
		csSDK_int32 inNodeID,
		PrTime inSequenceTime,
		PrTime inSegmentTime,
		PrTime inSequenceTicksPerFrame,
		csSDK_int32 inSequenceWidth,
		csSDK_int32 inSequenceHeight,
		csSDK_int32 inSequencePixelAspectRatioNumerator,
		csSDK_int32 inSequencePixelAspectRatioDenominator,
		const SequenceRender_ParamsRec* inRenderParams,
		prBool inBypassEffects,
		prPluginID* outIdentifier);

	/**
	** For a given operator, in a given timeline, with a provided input frame, render the effect asynchronously.
	** For any of the overrides, you can pass zero, implying that you don't want to override at all. This is useful when rendering the inputs to a MulticamNode
	** Note that the resulting frame may not match any of your requested pixel formats.
	**
	** @param inTimelineID - the timeline identifier provided by the host
	** @param inClipNodeID - the Clip that contains the operator you want to render
	** @param inOperatorStartIndex - the zero-based index of the operator you want to start rendering at
	** @param inOperatorCount - the number of operators you want to apply
	** @param inSequenceTime - the time you want rendered, in the containing timeline. This is used for filters that request other media during their render
	** @param inSegmentTime - the time you want rendered, relative to the node's concept of time, not the containing timeline.
	** @param inFrameRateScale - the frame rate you want rendered. In general, this should be the framerate of the timeline, but any can be provided.
	** @param inFrameRateSampleSize - see above.
	** @param inSequenceWidth - the overridden sequence width
	** @param inSequenceHeight - the overridden sequence height
	** @param inSequencePixelAspectRatioNumerator - the overridden sequence PAR
	** @param inSequencePixelAspectRatioDenominator - the overridden sequence PAR
	** @param inInputFrame - can be NULL if inOperatorStartIndex is 0, implies that you want the host to provide the clip node's input frame
	** @param inRenderParams - the same set of render params used in the SequenceRenderSuite. The option to composite on black is ignored.
	** @param inRenderContext - the context in which the render is occurring
	** @param inCompletionProc - the callback that will be called when the render is complete.
	** @param inAsyncCompletionData - an extra param that will be provided to the completion routine.
	** @param inBypassEffects - if true, skip non-intrinsic video effects while rendering
	** @param outRequestID - an identifier that can be used to cancel this request. It isn't really useful for anything else since your
	**						completion routine can be called before this one returns.
	*/
	prSuiteError(*ApplyOperatorsToFrameAsync3)(
		PrTimelineID inTimelineID,
		csSDK_int32 inClipNodeID,
		csSDK_int32 inOperatorStartIndex,
		csSDK_int32 inOperatorCount,
		PrTime inSequenceTime,
		PrTime inSegmentTime,
		PrTime inSequenceTicksPerFrame,
		csSDK_int32 inSequenceWidth,
		csSDK_int32 inSequenceHeight,
		csSDK_int32 inSequencePixelAspectRatioNumerator,
		csSDK_int32 inSequencePixelAspectRatioDenominator,
		PPixHand inInputFrame,
		const SequenceRender_ParamsRec* inRenderParams,
		imRenderContext inRenderContext,
		PrSDKVideoSegmentAsyncRenderCompletionProc inCompletionProc,
		csSDK_int64 inAsyncCompletionData,
		prBool inBypassEffects,
		csSDK_int32* outRequestID);

	/**
	**	A matching function for ApplyOperatorsToFrameAsync. This allows you to check the cache for this frame
	**	prior to requesting it to be rendered.
	*/
	prSuiteError(*GetIdentifierForApplyOperatorsToFrameAsync2)(
		PrTimelineID inTimelineID,
		csSDK_int32 inClipNodeID,
		csSDK_int32 inOperatorStartIndex,
		csSDK_int32 inOperatorCount,
		PrTime inSequenceTime,
		PrTime inSegmentTime,
		PrTime inSequenceTicksPerFrame,
		csSDK_int32 inSequenceWidth,
		csSDK_int32 inSequenceHeight,
		csSDK_int32 inSequencePixelAspectRatioNumerator,
		csSDK_int32 inSequencePixelAspectRatioDenominator,
		PPixHand inInputFrame,
		const SequenceRender_ParamsRec* inRenderParams,
		prBool inBypassEffects,
		prPluginID* outIdentifier);

	/**
	** For a given transition, in a given timeline, with two (optional) provided input frames, render the transition.
	** Note that the resulting frame may not match any of your requested pixel formats.
	**
	** @param inTimelineID - the timeline identifier provided by the host
	** @param inTransitionNodeID - Only transition nodes are acceptable here.
	** @param inSequenceTime - the time you want rendered, in the containing timeline. This is used for filters that request other media during their render
	** @param inSegmentTime - the time you want rendered, relative to the node's concept of time, not the containing timeline.
	** @param inFrameRateScale - the frame rate you want rendered. In general, this should be the framerate of the timeline, but any can be provided.
	** @param inFrameRateSampleSize - see above.
	** @param inOutgoingInputFrame  - if the PPixHand is NULL, then transparent black will be used
	** @param inIncomingInputFrame - if the PPixHand is NULL, then transparent black will be used
	** @param inRenderParams - the same set of render params used in the SequenceRenderSuite. The option to composite on black is ignored.
	** @param inCompletionProc - the callback that will be called when the render is complete.
	** @param inAsyncCompletionData - an extra param that will be provided to the completion routine.
	** @param inBypassEffects - if true, skip non-intrinsic video effects while rendering
	** @param outRequestID - an identifier that can be used to cancel this request. It isn't really useful for anything else since your
	**						completion routine can be called before this one returns.
	*/
	prSuiteError(*ApplyTransitionToFrameAsync2)(
		PrTimelineID inTimelineID,
		csSDK_int32 inTransitionNodeID,
		PrTime inSequenceTime,
		PrTime inSegmentTime,
		PrTime inSequenceTicksPerFrame,
		PPixHand inOutgoingInputFrame,
		PPixHand inIncomingInputFrame,
		const SequenceRender_ParamsRec* inRenderParams,
		PrSDKVideoSegmentAsyncRenderCompletionProc inCompletionProc,
		csSDK_int64 inAsyncCompletionData,
		prBool inBypassEffects,
		csSDK_int32* outRequestID);
	
	/**
	** Color managed version of ProduceFrameAsync3
	** same params except that we pass SequenceRender_ParamsRecExt that includes
	** opaque id for the color space
	*/
	prSuiteError(*ProduceColorManagedFrameAsync4)(
		PrTimelineID inTimelineID,
		csSDK_int32 inNodeID,
		PrTime inSequenceTime,
		PrTime inSegmentTime,
		PrTime inSequenceTicksPerFrame,
		csSDK_int32 inSequenceWidth,
		csSDK_int32 inSequenceHeight,
		csSDK_int32 inSequencePixelAspectRatioNumerator,
		csSDK_int32 inSequencePixelAspectRatioDenominator,
		const SequenceRender_ParamsRecExt* inRenderParams,
		imRenderContext inRenderContext,
		PrSDKVideoSegmentAsyncRenderCompletionProc inCompletionProc,
		csSDK_int64 inAsyncCompletionData,
		prBool inBypassEffects, 
		PrRenderCaptionStreamFormat inRenderCaptioningStreamFormat,
		csSDK_int32* outRequestID);
	
	/**
	**	A matching function for ProduceColorManagedFrameAsync4. 
	**	This allows you to check the cache for this frame
	**	prior to requesting it to be rendered.
	*/
	prSuiteError(*GetIdentifierForProduceColorManagedFrameAsync3)(
		PrTimelineID inTimelineID,
		csSDK_int32 inNodeID,
		PrTime inSequenceTime,
		PrTime inSegmentTime,
		PrTime inSequenceTicksPerFrame,
		csSDK_int32 inSequenceWidth,
		csSDK_int32 inSequenceHeight,
		csSDK_int32 inSequencePixelAspectRatioNumerator,
		csSDK_int32 inSequencePixelAspectRatioDenominator,
		const SequenceRender_ParamsRecExt* inRenderParams,
		prBool inBypassEffects,
		PrRenderCaptionStreamFormat inRenderCaptionStreamFormat,
		prPluginID* outIdentifier);
	
	/**
	** Color managed version of ApplyOperatorsToFrameAsync3
	** Note that we pass SequenceRencer_ParamsRecExt
	*/
	prSuiteError(*ApplyOperatorsToColorManagedFrameAsync4)(
		PrTimelineID inTimelineID,
		csSDK_int32 inClipNodeID,
		csSDK_int32 inOperatorStartIndex,
		csSDK_int32 inOperatorCount,
		PrTime inSequenceTime,
		PrTime inSegmentTime,
		PrTime inSequenceTicksPerFrame,
		csSDK_int32 inSequenceWidth,
		csSDK_int32 inSequenceHeight,
		csSDK_int32 inSequencePixelAspectRatioNumerator,
		csSDK_int32 inSequencePixelAspectRatioDenominator,
		PPixHand inInputFrame,
		const SequenceRender_ParamsRecExt* inRenderParams,
		imRenderContext inRenderContext,
		PrSDKVideoSegmentAsyncRenderCompletionProc inCompletionProc,
		csSDK_int64 inAsyncCompletionData,
		prBool inBypassEffects,
		PrRenderCaptionStreamFormat inRenderCaptionStreamFormat,
		csSDK_int32* outRequestID);
	
	/**
	**	A matching function for ApplyOperatorsToColorManagedFrameAsync4.
	**	This allows you to check the cache for this frame
	**	prior to requesting it to be rendered.
	*/
	prSuiteError(*GetIdentifierForApplyOperatorsToColorManagedFrameAsync3)(
		PrTimelineID inTimelineID,
		csSDK_int32 inClipNodeID,
		csSDK_int32 inOperatorStartIndex,
		csSDK_int32 inOperatorCount,
		PrTime inSequenceTime,
		PrTime inSegmentTime,
		PrTime inSequenceTicksPerFrame,
		csSDK_int32 inSequenceWidth,
		csSDK_int32 inSequenceHeight,
		csSDK_int32 inSequencePixelAspectRatioNumerator,
		csSDK_int32 inSequencePixelAspectRatioDenominator,
		PPixHand inInputFrame,
		const SequenceRender_ParamsRecExt* inRenderParams,
		prBool inBypassEffects,
		PrRenderCaptionStreamFormat inRenderCaptionStreamFormat,
		prPluginID* outIdentifier);
	
	/**
	**	Color managed version of ApplyTransitionToFrameAsync2
	**	Note that we pass SequenceRencer_ParamsRecExt
	*/
	prSuiteError(*ApplyTransitionToColorManagedFrameAsync3)(
		PrTimelineID inTimelineID,
		csSDK_int32 inTransitionNodeID,
		PrTime inSequenceTime,
		PrTime inSegmentTime,
		PrTime inSequenceTicksPerFrame,
		PPixHand inOutgoingInputFrame,
		PPixHand inIncomingInputFrame,
		const SequenceRender_ParamsRecExt* inRenderParams,
		PrSDKVideoSegmentAsyncRenderCompletionProc inCompletionProc,
		csSDK_int64 inAsyncCompletionData,
		prBool inBypassEffects,
		PrRenderCaptionStreamFormat inRenderCaptionStreamFormat,
		csSDK_int32* outRequestID);
	
	/**
	**	A matching function for ApplyTransitionToColorManagedFrameAsync3.
	**	This allows you to check the cache for this frame
	**	prior to requesting it to be rendered.
	*/
	prSuiteError (*GetIdentifierForApplyTransitionToColorManagedFrameAsync2)(
		PrTimelineID inTimelineID,
		csSDK_int32 inTransitionNodeID,
		PrTime inSequenceTime,
		PrTime inSegmentTime,
		PrTime inSequenceTicksPerFrame,
		PPixHand inOutgoingInputFrame,
		PPixHand inIncomingInputFrame,
		const SequenceRender_ParamsRecExt* inRenderParams,
		PrRenderCaptionStreamFormat inRenderCaptionStreamFormat,
		prPluginID* outIdentifier);

} PrSDKVideoSegmentRenderSuite;


#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif

