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


#ifndef PRSDKGPUFILTER_H
#define PRSDKGPUFILTER_H


#include "PrSDKMALErrors.h"
#include "PrSDKPlayModule.h"
#include "PrSDKPlugSuites.h"
#include "PrSDKQuality.h"
#include "PrSDKTimeSuite.h"


#pragma pack(push, 1)
#ifdef __cplusplus
extern "C" {
#endif



/**
**	Interface version
*/
#define PrSDKGPUFilterInterfaceVersion1 1 // CS7
#define PrSDKGPUFilterInterfaceVersion2 2 // CC9.0
#define PrSDKGPUFilterInterfaceVersion PrSDKGPUFilterInterfaceVersion2



/**
**	Information returned from a plugin
*/
typedef struct
{
	csSDK_uint32 outInterfaceVersion;
	PrSDKString outMatchName; // outMatchName must be equal to a registered software filter, if null will default to the module's pipl.
} PrGPUFilterInfo;


/**
**	A filter instance represents an effect applied to a track item with a fixed set of parameters.
**	Changed parameters will create a new instance.
*/
typedef struct
{
	// Standard suites
	piSuitesPtr piSuites;

	// For use in conjunction with PrSDKGPUDeviceSuite
	csSDK_uint32 inDeviceIndex;

	// For use in conjunction with PrSDKVideoSegmentSuite
	PrTimelineID inTimelineID;
	csSDK_int32 inNodeID;

	// Used by a plugin to store instance data, never touched by the host
	void* ioPrivatePluginData;

	// Specify if the plugin is likely to play in realtime, used to determine segment coloring
	prBool outIsRealtime;

} PrGPUFilterInstance;


/**
**	Information about a frame render
*/
typedef struct
{
	// Time of the current render
	PrTime inClipTime;
	PrTime inSequenceTime;

	// Render properties
	PrRenderQuality inQuality;
	float inDownsampleFactorX;
	float inDownsampleFactorY;

	// Frame properties
	csSDK_uint32 inRenderWidth;
	csSDK_uint32 inRenderHeight;
	csSDK_uint32 inRenderPARNum;
	csSDK_uint32 inRenderPARDen;
	prFieldType inRenderFieldType;
	PrTime inRenderTicksPerFrame;

	// GPU rendering is always on full height progressive frames unless outNeedsFieldSeparation is false.
	//		inRenderField indicates which field is being rendered
	pmFieldDisplay inRenderField;

} PrGPUFilterRenderParams;


/**
**	Specify dependencies to satisfy a render
*/
typedef enum
{
	PrGPUDependency_InputFrame = 0x1,
	PrGPUDependency_Precompute = 0x2,
	PrGPUDependency_FieldSeparation = 0x4,
	PrGPUDependency_TransitionInputFrame = 0x8,
} PrGPUFilterFrameDependencyType;
typedef struct
{
	// The dependency type
	PrGPUFilterFrameDependencyType outDependencyType;

	// PrGPUDependency_InputFrame:
	// Dependence on other frame times
	csSDK_int32 outTrackID; // 0 for the current track
	PrTime outSequenceTime;

	// PrGPUDependency_Precompute:
	// Dependence on precomputation phase
	PrPixelFormat outPrecomputePixelFormat;
	csSDK_uint32 outPrecomputeFrameWidth;
	csSDK_uint32 outPrecomputeFrameHeight;
	csSDK_uint32 outPrecomputeFramePARNumerator;
	csSDK_uint32 outPrecomputeFramePARDenominator;
	prFieldType outPrecomputeFrameFieldType;
	csSDK_size_t outPrecomputeCustomDataSize; // only needed if outPrecomputePixelFormat is custom

	// PrGPUDependency_FieldSeparation:
	// Indicates if the plugin may operate on both fields simultaneously (eg non-spatial and non-time varying)
	prBool outNeedsFieldSeparation;

	// PrGPUDependency_TransitionInputFrame:
	// Dependence on other transition frame times. Uses outSequenceTime.
	// Which clip of the transition the input frame should be read from (false = left clip, true = right clip).
	// Whether a clip has an "incoming" or "outgoing" transition is from the perspective of the
	// clip, not the transition. A transition on the left edge of the clip is an "incoming" transition,
	// while a transition on the right edge of the clip is an "outgoing" transition. So from the perspective
	// of a transition at a cut point, the clip on the left of the transition is "outgoing" and the clip on
	// the right of the transition is "incoming".
	prBool outReadIncomingTransition;
} PrGPUFilterFrameDependency;



/**
**	GPU Filter callbacks. 
*/
typedef struct
{
	/**
	**	Creates a GPU filter instance representing an effect or transition on a
	**	track item. Returning an error from CreateInstance will cause this node
	**	to be rendered in software for the current set of parameters. Separate
	**	instances may be called concurrently.
	*/
	prSuiteError (*CreateInstance)(
		PrGPUFilterInstance* ioInstanceData);
	prSuiteError (*DisposeInstance)(
		PrGPUFilterInstance* ioInstanceData);

	/**
	**	Return dependency information about a render, or nothing if
	**	only the current frame is required.
	*/
	prSuiteError (*GetFrameDependencies)(
		PrGPUFilterInstance* inInstanceData,
		const PrGPUFilterRenderParams* inRenderParams,
		csSDK_int32* ioQueryIndex, // increment for additional dependencies
		PrGPUFilterFrameDependency* outFrameDependencies);

	/**
	**	Precompute a result into preallocated uninitialized host (pinned) memory.
	**	Will only be called if PrGPUDependency_Precompute was returned from GetFrameDependencies.
	**	Precomputation may be called ahead of render time. Results will be
	**	uploaded to the GPU by the host. If outPrecomputePixelFormat is not custom,
	**	frames will be converted to the GPU pixel format.
	*/
	prSuiteError (*Precompute)(
		PrGPUFilterInstance* inInstanceData,
		const PrGPUFilterRenderParams* inRenderParams,
		csSDK_int32 inIndex,
		PPixHand inFrame);

	/**
	**	Render into an allocated outFrame allocated with PrSDKGPUDeviceSuite or operate
	**	in place. Result must be in the same pixel format as the input. For effects, frame 0
	**	will always be the frame at the current time, other input frames will be in the same order as
	**	returned from GetFrameDependencies. For transitions frame 0 will be the incoming frame and
	**	frame 1 the outgoing frame. Transitions may not have other frame dependencies.
	*/
	prSuiteError (*Render)(
		PrGPUFilterInstance* inInstanceData,
		const PrGPUFilterRenderParams* inRenderParams,
		const PPixHand* inFrames,
		csSDK_size_t inFrameCount,
		PPixHand* outFrame);
		
} PrGPUFilter;



/**
**	Entry point.
*/
#define PrGPUFilterEntryPointName "xGPUFilterEntry"
typedef prSuiteError (*PrGPUFilterEntry)(
	csSDK_uint32 inHostInterfaceVersion, // current host version
	csSDK_int32* ioIndex, // increment if more filters
	prBool inStartup, // 1 for startup, 0 for shutdown
	piSuitesPtr piSuites,
	PrGPUFilter* outFilter,
	PrGPUFilterInfo* outFilterInfo);



#ifdef __cplusplus
}
#endif
#pragma pack(pop)



#endif