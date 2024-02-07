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

//	This file is part of the public Premiere SDK.


#ifndef	PRVIDEOSEGMENTSUITE_H
#define PRVIDEOSEGMENTSUITE_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKMALERRORS_H
#include "PrSDKMALErrors.h"
#endif

#ifndef PRSDKTIMESUITE_H
#include "PrSDKTimeSuite.h"
#endif

#ifndef PRSDKSTREAMLABEL_H
#include "PrSDKStreamLabel.h"
#endif

#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif

#define kPrSDKVideoSegmentSuite				"MediaCore Video Segment Suite"
#define kPrSDKVideoSegmentSuiteVersion4		4
#define kPrSDKVideoSegmentSuiteVersion5		5 //Added StreamLabels
#define kPrSDKVideoSegmentSuiteVersion6		6 //Added AcquireFirstNodeInTimeRange
#define kPrSDKVideoSegmentSuiteVersion7		7 //Added Graphics Transforms
#define kPrSDKVideoSegmentSuiteVersion8		8 // added color managed version of GetVideoSegmentsProperties
#define kPrSDKVideoSegmentSuiteVersion9		9 // added GetNodeTimeScale

#define kPrSDKVideoSegmentSuiteVersion		kPrSDKVideoSegmentSuiteVersion9

enum PrNodeInfoFlag
{
	kPrNodeInfoFlag_IsCompletelyOpaque					= 1 << 0,
	kPrNodeInfoFlag_IsCompletelyTransparent				= 1 << 1,
	kPrNodeInfoFlag_DoesNotDependOnSequenceTime			= 1 << 2,
	kPrNodeInfoFlag_NodeDoesNotDependOnSegmentTime		= 1 << 3,
	kPrNodeInfoFlag_NodeDoesNotDependOnClipInOutTime	= 1 << 4,
	kPrNodeInfoFlag_IsNOP								= 1 << 5,
	kPrNodeInfoFlag_NodeDoesNotDependOnTrackInputs		= 1 << 6,
	kPrNodeInfoFlag_IsAdjustment						= 1 << 7,
	kPrNodeInfoFlag_MaxSize								= 0xFFFFFFFF
};

const int kMaxNodeTypeStringSize = 256;

enum PrKeyframeInterpolationModeFlag
{
	kPrInterpolationModeFlag_Linear =				0,
	kPrInterpolationModeFlag_Hold =					4,
	kPrInterpolationModeFlag_Bezier =				5, 
	kPrInterpolationModeFlag_Time =					6,
	kPrInterpolationModeFlag_TimeTransitionStart =	7,
	kPrInterpolationModeFlag_TimeTransitionEnd =	8,
	kPrInterpolationModeFlag_MaxSize = 0xFFFFFFFF
};

enum PrNodeScalePolicy
{
	kPrNodeScalePolicy_None =					0,
	kPrNodeScalePolicy_ScaleToFrame =			1, //Scale to fit in the frame, no distortion, letter or pillar box
	kPrNodeScalePolicy_ScaleToFillCrop =		2, //Scale to fit with overlap, no distortion, but some cropping
	kPrNodeScalePolicy_ScaleToFillDistort =		3, //Scale to fit, with distortion, no padding
};

/**
**	This callback function is used by the property query methods. Iteration can be cancelled at
**	any time by returning a result other than malNoError.
**
**	@param	inPluginObject			A value (such as "this") passed by the plugin in the original call
**									and passed back in the callback. May be NULL.
**	@param	inKey					The key for the property
**	@param	inValue					The value for the property
*/
typedef prSuiteError (*SegmentNodePropertyCallback)(
	csSDK_int32 inPluginObject,
	const char* inKey,
	const prUTF8Char* inValue);

/**
**	This is the list of strings that identify the different possible node types
*/
#define kVideoSegment_NodeType_Clip "RenderableNodeClipImpl"
#define kVideoSegment_NodeType_Compositor "RenderableNodeCompositorImpl"
#define kVideoSegment_NodeType_Disabled "RenderableNodeDisabledImpl"
#define kVideoSegment_NodeType_Effect "RenderableNodeEffectImpl"
#define kVideoSegment_NodeType_Transition "RenderableNodeTransitionImpl"
#define kVideoSegment_NodeType_Media "RenderableNodeMediaImpl"
#define kVideoSegment_NodeType_Preview "RenderableNodePreviewImpl"
#define kVideoSegment_NodeType_SolidColor "RenderableNodeSolidColorImpl"
#define kVideoSegment_NodeType_Multicam "RenderableNodeMulticamImpl"
#define kVideoSegment_NodeType_Adjustment "RenderableNode_AdjustmentImpl"
#define kVideoSegment_NodeType_AdjustmentEffect "RenderableNode_AdjustmentEffectImpl"

typedef struct 
{
	
	/**
	**	From a sequence, get an ID to its video segments ID. This is a ref-counted
	**	object, and must be released when no longer needed.
	**
	**	@param	inTimelineID		The plugin timeline ID for the sequence
	**	@param	outVideoSegmentsID	Receives the ID for the Video Segments
	*/
	prSuiteError (*AcquireVideoSegmentsID)(
		PrTimelineID inTimelineData,
		csSDK_int32* outVideoSegmentsID);

	/**
	**	From a sequence, get an ID to its video segments ID with preview files substituted. This is a ref-counted
	**	object, and must be released when no longer needed.
	**
	**	@param	inTimelineID		The plugin timeline ID for the sequence
	**	@param	outVideoSegmentsID	Receives the ID for the Video Segments with Previews.
	*/
	prSuiteError (*AcquireVideoSegmentsWithPreviewsID)(
		PrTimelineID inTimelineData,
		csSDK_int32* outVideoSegmentsID);

	/**
	**	From a sequence, get an ID to its video segments ID with preview files substituted, but only previews
	**	for sections that are opaque. This is appropriate for use when using previews for nested sequences. This is a ref-counted
	**	object, and must be released when no longer needed.
	**
	**	@param	inTimelineID		The plugin timeline ID for the sequence
	**	@param	outVideoSegmentsID	Recevies the ID for the Video Segments with Previews.
	*/
	prSuiteError (*AcquireVideoSegmentsWithOpaquePreviewsID)(
		PrTimelineID inTimelineData,
		csSDK_int32* outVideoSegmentsID);
	
	/**
	**	Release a Video Segments ID
	**
	**	@param	inVideoSegmentsID	The Video Segments ID to release
	*/
	prSuiteError (*ReleaseVideoSegmentsID)(
		csSDK_int32 inVideoSegmentsID);
		
	/**
	**	Get the hash of a Video Segments object
	**
	**	@param	inVideoSegmentsID	The Video Segments ID
	**	@param	outHash				The GUID hash of the segments
	*/
	prSuiteError (*GetHash)(
		csSDK_int32 inVideoSegmentsID,
		prPluginID* outHash);
	
	/**
	**	Get the number of segments in the segments object
	**
	**	@param	inVideoSegmentsID	The Video Segments ID
	**	@param	outNumSegments		The number of segments
	*/
	prSuiteError (*GetSegmentCount)(
		csSDK_int32 inVideoSegmentsID,
		csSDK_int32* outNumSegments);
	
	/**
	**	Get the details of the Nth Node.
	**
	**	@param	inVideoSegmentsID	The Video Segments ID
	**	@param	inIndex				Which segment?
	**	@param	outStartTime		The start time of the segment
	**	@param	outEndTime			The end time of the segment
	**	@param	outSegmentOffset	The offset value for the segment
	**	@param	outHash				The hash for the segment
	*/
	prSuiteError (*GetSegmentInfo)(
		csSDK_int32 inVideoSegmentsID,
		csSDK_int32 inIndex,
		PrTime* outStartTime,
		PrTime* outEndTime,
		PrTime* outSegmentOffset,
		prPluginID* outHash);
	
	/**
	**	Get a segment node. This object is ref-counted and must be released.
	**
	**	@param	inVideoSegmentsID	The Video Segments ID
	**	@param	inHash				The hash for the segment
	**	@param	outVideoNodeID		The video node ID.
	*/
	prSuiteError (*AcquireNodeID)(
		csSDK_int32 inVideoSegmentsID,
		prPluginID* inHash,
		csSDK_int32* outVideoNodeID);
	
	/**
	**	Release a Video Node ID
	**
	**	@param	inVideoNodeID		The Video Node ID to release
	*/
	prSuiteError (*ReleaseVideoNodeID)(
		csSDK_int32 inVideoNodeID);
	
	/**
	**	Get details about a node.
	**
	**	@param	inVideoNodeID		The Video Node ID
	**	@param	outNodeType			A string of size kMaxNodeTypeStringSize holding the node type
	**	@param	outHash				The hash for the node (may be different than the hash used to get the node)
	**	@param	outInfoFlags		The flags for this node (see enum above)
	*/
	prSuiteError (*GetNodeInfo)(
		csSDK_int32 inVideoNodeID,
		char* outNodeType,
		prPluginID* outHash,
		csSDK_int32* outFlags);
		
	/**
	**	Get the number of inputs on the node object
	**
	**	@param	inVideoNodeID		The Video Node ID
	**	@param	outNumInputs		The number of inputs
	*/
	prSuiteError (*GetNodeInputCount)(
		csSDK_int32 inVideoNodeID,
		csSDK_int32* outNumInputs);
	
	/**
	**	Get a segment node that is an input to another node. This object is ref-counted and must be released.
	**
	**	@param	inVideoNodeID		The Video Node ID
	**	@param	inIndex				The index of the input
	**	@param	outOffset			The time offset relative to it's parent node
	**	@param	outInputVideoNodeID	The video node ID of the input node.
	*/
	prSuiteError (*AcquireInputNodeID)(
		csSDK_int32 inVideoNodeID,
		csSDK_int32 inIndex,
		PrTime* outOffset,
		csSDK_int32* outInputVideoNodeID);
		
	/**
	**	Get the number of inputs on the node object
	**
	**	@param	inVideoNodeID		The Video Node ID
	**	@param	outNumOperators		The number of operators
	*/
	prSuiteError (*GetNodeOperatorCount)(
		csSDK_int32 inVideoNodeID,
		csSDK_int32* outNumOperators);
	
	/**
	**	Get a segment node that is an operator on another node. This object is ref-counted and must be released.
	**
	**	@param	inVideoNodeID		The Video Node ID
	**	@param	inIndex				The index of the operator
	**	@param	outOperatorVideoNodeID	The video node ID of the input node.
	*/
	prSuiteError (*AcquireOperatorNodeID)(
		csSDK_int32 inVideoNodeID,
		csSDK_int32 inIndex,
		csSDK_int32* outOperatorVideoNodeID);
	
	/**
	**	Iterate all of the properties on a node.
	**
	**	@param	inVideoNodeID		The Video Node ID
	**	@param	inCallback			The callback function to return the properties
	**	@param	inPluginObject		The plugin object returned in the callback.
	*/
	prSuiteError (*IterateNodeProperties)(
		csSDK_int32 inVideoNodeID,
		SegmentNodePropertyCallback inCallback,
		csSDK_int32 inPluginObject);
	
	/**
	**	Get the value of a single property on a node
	**
	**	@param	inVideoNodeID		The Video Node ID
	**	@param	inKey				The key of the property
	**	@param	outValue			A string holding the value. This UTF8 string is allocated using PrNewPtr,
	**								and ownership is transferred to the plugin and must be disposed
	**								by the plugin.
	*/
	prSuiteError (*GetNodeProperty)(
		csSDK_int32 inVideoNodeID,
		const char* inKey,
		PrMemoryPtr* outValue);
	
	/**
	**	Get the number of params
	**
	**	@param	inVideoNodeID		The Video Node ID
	**	@param	outParamCount		The number of params
	*/
	prSuiteError (*GetParamCount)(
		csSDK_int32 inVideoNodeID,
		csSDK_int32* outParamCount);
	
	/**
	**	Get a specific param value at a specific time
	**
	**	@param	inVideoNodeID		The Video Node ID
	**	@param	inIndex				The index of the param
	**	@param	inTime				The time requested (in Media time)
	**	@param	outParam			The param
	*/
	prSuiteError (*GetParam)(
		csSDK_int32 inVideoNodeID,
		csSDK_int32 inIndex,
		PrTime inTime,
		PrParam* outParam);
	
	/**
	**	Get the next keyframe time after the specified time.
	**
	**	Example: Keyframes at 0 and 10
	**	inTime = -1, outKeyframeTime = 0
	**	inTime = 0, outKeyframeTime = 10
	**	inTime = 9, outKeyframeTime = 10
	**	inTime = 10, returns suiteError_NoKeyframeAfterInTime
	**
	**	@param	inVideoNodeID		The Video Node ID
	**	@param	inIndex				The index of the param
	**	@param	inTime				The lower bound time
	**	@param	outKeyframeTime		The time of the next keyframe > inTime
	**	@param	outKeyframeInterpolationMode	The temporal interpolation mode of the keyframe, see
	**											the enum for PrKeyframeInterpolationModeFlag above
	*/
	prSuiteError (*GetNextKeyframeTime)(
		csSDK_int32 inVideoNodeID,
		csSDK_int32 inIndex,
		PrTime inTime,
		PrTime* outKeyframeTime,
		csSDK_int32* outKeyframeInterpolationMode);
	
	/**
	**	Transform a node local time into a time appropriate for node inputs and 
	**	operators. When used on the clip node, for instance, this will take into
	**	account speed change, reverse, time remapping and return a time value
	**	which can be used in the Media and Effect nodes.
	**	If the node does not have a time transform, function will not fail but
	**	will return in input time in the output.
	*/
	prSuiteError (*TransformNodeTime)(
		csSDK_int32 inVideoNodeID,
		PrTime inTime,
		PrTime* outTime);

	/**
	 **	Retrieve general properties of a sequence (video segments). I.e. width/height, par and framerate.
	 **
	 **	@param	inTimelineID					The plugin timeline ID for the sequence
	 **	@param	outWidth						Receives width of the sequence
	 **	@param	outHeight						Receives height of the sequence
	 **	@param	outPixelAspectRatioNumerator	Receives the pixel aspect ratio numerator of the sequence
	 **	@param	outPixelAspectRatioDenominator	Receives the pixel aspect ratio denominator of the sequence
	 **	@param	outFrameRateNumerator			Receives the frame rate numerator of the sequence
	 **	@param	outFrameRateDenominator			Receives the frame rate denominator of the sequence
	 */
	prSuiteError (*GetVideoSegmentsProperties)(
		PrTimelineID			inTimelineData,
		prRect*					outBounds,
		csSDK_int32*			outPixelAspectRatioNumerator,
		csSDK_int32*			outPixelAspectRatioDenominator,
		PrTime*					outFrameRate,
		prFieldType*			outFieldType);

	/**
	**	From a sequence, get a segment node for a requested time. This is a ref-counted
	**	object, and must be released when no longer needed.
	**
	**	@param	inVideoSegmentsID	The Video Segments ID
	**	@param	inTime				Requested segment time
	**	@param	outVideoNodeID		The video node ID
	**	@param	outSegmentOffset	Offset of retrieved segment
	*/
	prSuiteError (*AcquireNodeForTime)(
		csSDK_int32 inVideoSegmentsID,
		PrTime inTime,
		csSDK_int32* outVideoNodeID,
		PrTime* outSegmentOffset);

	/**
	**	From a sequence, get an ID to its video segments ID. This is a ref-counted
	**	object, and must be released when no longer needed.
	**
	**	@param	inTimelineID		The plugin timeline ID for the sequence
	**	@param	outVideoSegmentsID	Receives the ID for the Video Segments
	*/
	prSuiteError (*AcquireVideoSegmentsIDWithStreamLabel)(
		PrTimelineID inTimelineData,
		PrSDKStreamLabel inStreamLabel,
		csSDK_int32* outVideoSegmentsID);

	/**
	**	From a sequence, get an ID to its video segments ID with preview files substituted. This is a ref-counted
	**	object, and must be released when no longer needed.
	**
	**	@param	inTimelineID		The plugin timeline ID for the sequence
	**	@param	outVideoSegmentsID	Receives the ID for the Video Segments with Previews.
	*/
	prSuiteError (*AcquireVideoSegmentsWithPreviewsIDWithStreamLabel)(
		PrTimelineID inTimelineData,
		PrSDKStreamLabel inStreamLabel,
		csSDK_int32* outVideoSegmentsID);

	/**
	**	From a sequence, get an ID to its video segments ID with preview files substituted, but only previews
	**	for sections that are opaque. This is appropriate for use when using previews for nested sequences. This is a ref-counted
	**	object, and must be released when no longer needed.
	**
	**	@param	inTimelineID		The plugin timeline ID for the sequence
	**	@param	outVideoSegmentsID	Recevies the ID for the Video Segments with Previews.
	*/
	prSuiteError (*AcquireVideoSegmentsWithOpaquePreviewsIDWithStreamLabel)(
		PrTimelineID inTimelineData,
		PrSDKStreamLabel inStreamLabel,
		csSDK_int32* outVideoSegmentsID);
	
	/**
	**	From a sequence, get the first segment node that intersects with a range of times.
	**  This is a ref-counted object, and must be released when no longer needed.
	**
	**	@param	inVideoSegmentsID	The Video Segments ID
	**	@param	inStartTime			The start of the requested segment time range
	**	@param	inEndTime			The end of the requested segment time range
	**	@param	outVideoNodeID		The video node ID
	**	@param	outSegmentOffset	Offset of retrieved segment
	*/
	prSuiteError (*AcquireFirstNodeInTimeRange)(
		csSDK_int32 inVideoSegmentsID,
		PrTime inStartTime,
		PrTime inEndTime,
		csSDK_int32* outVideoNodeID,
		PrTime* outSegmentOffset);

	/**
	**	Acquire the node owning an operator
	**
	**	@param	inVideoNodeID		The operator
	**	@param	outOwnerNodeID		The owner
	*/
	prSuiteError (*AcquireOperatorOwnerNodeID)(
		csSDK_int32 inVideoNodeID,
		csSDK_int32* outOwnerNodeID);
	
	/**
	 **	Get graphic layer params at a specific time
	 **
	 **	@param	inVideoNodeID		The Video Node ID
	 **	@param	inTime				The time requested (in Media time)
	 */
	prSuiteError (*GetGraphicsTransformedParams)(
		csSDK_int32 inVideoNodeID,
		PrTime inTime,
		prFPoint64* outPosition,
		prFPoint64* outAnchor,
		prFPoint64* outScale,
		float* outRotation);
	
	/**
	 **	Get graphic layer group ID
	 **
	 **	@param	inVideoNodeID		The Video Node ID
	 */
	prSuiteError (*HasGraphicsGroup)(
		csSDK_int32 inVideoNodeID,
		bool* outHasGraphicGroup);
	
	/**
	 **	Get graphic layer group ID
	 **
	 **	@param	inVideoNodeID		The Video Node ID
	 */
	prSuiteError (*GetGraphicsGroupID)(
		csSDK_int32 inVideoNodeID,
		csSDK_int32* outGroupID);
	
	/**
	 ** Color managed version of GetVideoSegmentsProperties
	 **	Retrieve general properties of a sequence (video segments). I.e. width/height, par and framerate
	 ** and color space
	 **
	 **	@param	inTimelineID					The plugin timeline ID for the sequence
	 **	@param	outWidth						Receives width of the sequence
	 **	@param	outHeight						Receives height of the sequence
	 **	@param	outPixelAspectRatioNumerator	Receives the pixel aspect ratio numerator of the sequence
	 **	@param	outPixelAspectRatioDenominator	Receives the pixel aspect ratio denominator of the sequence
	 **	@param	outFrameRateNumerator			Receives the frame rate numerator of the sequence
	 **	@param	outFrameRateDenominator			Receives the frame rate denominator of the sequence
	 ** @param	outColorSpaceID					Receives the opaque ID of the sequence's working color space
	 */
	prSuiteError (*GetVideoSegmentsPropertiesExt)(
		PrTimelineID			inTimelineData,
		prRect*					outBounds,
		csSDK_int32*			outPixelAspectRatioNumerator,
		csSDK_int32*			outPixelAspectRatioDenominator,
		PrTime*					outFrameRate,
		prFieldType*			outFieldType,
		PrSDKColorSpaceID*		outColorSpaceID);

	/**
	**	From a sequence, get the first segment node that intersects with a range of times.
	**  This is a ref-counted object, and must be released when no longer needed.
	**
	**	@param	inVideoSegmentsID	The Video Segments ID
	**	@param	inStartTime			The start of the requested segment time range
	**	@param	inEndTime			The end of the requested segment time range
	**	@param	outVideoNodeID		The video node ID
	**	@param	outSegmentStartTime	Start time of retrieved segment
	**	@param	outSegmentEndTime	End time of retrieved segment
	**	@param	outSegmentOffset	Offset of retrieved segment
	*/
	prSuiteError(*AcquireFirstNodeInTimeRangeExt)(
		csSDK_int32 inVideoSegmentsID,
		PrTime inStartTime,
		PrTime inEndTime,
		csSDK_int32* outVideoNodeID,
		PrTime* outSegmentStartTime,
		PrTime* outSegmentEndTime,
		PrTime* outSegmentOffset);

	
	/**
	**	Returns the relative time rate of a node at a given point in time.
	**  Node time rate varies with e.g. time remapping but not the playback speed of the sequence
	**  Can be thought of as the instantaneous rate of change of TransformNodeTime()
	**	@param	inVideoNodeID		The Video Node ID
	**	@param	inTime				The time requested (in Media time - untransformed)
	**	@param	outRate				The node rate relative to the containing sequence
	*/
	prSuiteError(*GetNodeTimeScale)(
		csSDK_int32 inVideoNodeID,
		PrTime inTime,
		double* outRate);
	
} PrSDKVideoSegmentSuite;



#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif

