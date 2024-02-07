/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2004 Adobe Systems Incorporated                       */
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

#ifndef PRSDKMARKERSUITE_H
#define PRSDKMARKERSUITE_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKMALERRORS_H
#include "PrSDKMALErrors.h"
#endif

#ifndef PRSDKTIMESUITE_H
#include "PrSDKTimeSuite.h"
#endif

#ifndef PRSDKFLASHCUEMARKERDATASUITE_H
#include "PrSDKFlashCueMarkerDataSuite.h"
#endif

#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif

#define kPrSDKMarkerSuite				"Marker Suite"
#define kPrSDKMarkerSuiteVersionV1		1
#define kPrSDKMarkerSuiteVersionV2		2
#define kPrSDKMarkerSuiteVersion		3

enum MarkerType
{
	kMarkerType_Timeline = 0,
	kMarkerType_DVD
};

enum DVDMarkerType
{
	kDVDMarkerType_Main = 0,
	kDVDMarkerType_Scene,
	kDVDMarkerType_Stop
};

typedef csSDK_int32 PrMarkerType;
const PrMarkerType kMarkerSuiteType_FlashVideoCue = 100;

typedef void*	prMarkerRef;
#define kInvalidMarkerRef prMarkerRef(-1)

// Obsoleted 20080514 in favor of PRT_MARKER_VALUE64. Use that instead
//#define PRT_MARKER_VALUE				0x00000001		//return value is a long timevalue

// Obsoleted 20080514 in favor of PRT_MARKER_DURATION64. Use that instead
//#define PRT_MARKER_DURATION				0x00000002		//return value is a long timevalue

#define PRT_MARKER_COMMENT				0x00000003		//return value is a pointer to a string
#define PRT_MARKER_CHAPTER				0x00000004		//return value is a pointer to a string
#define PRT_MARKER_HREF					0x00000005		//return value is a pointer to a string
#define PRT_MARKER_TARGET				0x00000006		//return value is a pointer to a string

#define PRT_MARKER_TYPE					0x00000007      //return value is an enum MarkerType
#define PRT_DVD_MARKER_TYPE				0x00000008      //return value is an enum DVDMarkerType

// Obsoleted 20080514 due to disuse and complication in making it work. It would not be hard to
// provide a PRT_DVDMARKER_THUMBNAIL_OFFSET64, but no current clients have been identified
//#define PRT_DVDMARKER_THUMBNAIL_OFFSET	0x00000009      //return value is a long timevalue

#define PRT_MARKER_SCALE				0x0000000A		//return value is a long scale (in the tdb sense)
#define PRT_MARKER_SAMPLESIZE			0x0000000B		//return value is a long sample size (in the tdb sense)
#define PRT_MARKER_NAME                 0x00000010      //return value is a pointer to a string

#define PRT_MARKER_VALUE64				0x00000011		//return value is a 64 bit timevalue
#define PRT_MARKER_DURATION64			0x00000012		//return value is a 64 bit timevalue

typedef struct  
{
	/*
	**	Used to retrieve the number of markers of a given type on the incoming inTimelineID, which
	**	is provided to the plugin as a "timelineData" member. For compilers this is in compileInfo
	**
	**	@param	inTimelineID			the timeline instance data used for all marker suite communication,
	**									it is passed into the compiler instance in the compDoCompileInfo8 struct
	**	@param	outNumberMarkers		the number of markers of the requested type. May be zero.
	*/
	prSuiteError (*GetMarkerCount)(
		PrTimelineID inTimelineID, 
		csSDK_uint32* outNumberMarkers);

	/*
	**	Call this prior to requesting GetMarkerName, and use the value populated in outMarkerNameLength
	**	to allocate memory for the host to copy data to using the stdParms->funcs->memoryFuncs->newPtr
	**	and pass in the allocated pointer to GetMarkerName.
	**	@param	inTimelineID			the timeline instance data used for all marker suite communication,
	**									it is passed into the compiler instance in the compDoCompileInfo8 struct
	**	@param	inMarkerType			the marker type to query for, must be a prMarkerType.
	**	@param	outMarkerNameSize		the size of the data caller should allocate with prNewPtr, and call back
	**									with GetMarkerName using the allocated buffer.
	*/
	prSuiteError (*GetMarkerNameSize)(
		PrTimelineID inTimelineID, 
		csSDK_uint32 inMarkerIndex, 
		size_t* outMarkerNameSize);

	/*
	**	Retrieve the marker name - the data is stored as prUTF16Char. See GetMarkerNameSize.
	**
	**	@param	inTimelineID			the timeline instance data used for all marker suite communication,
	**									it is passed into the compiler instance in the compDoCompileInfo8 struct
	**	@param	inMarkerType			the marker type to query for, must be a prMarkerType.
	**	@param	outMarkerName			populated with the marker name
	*/
	prSuiteError (*GetMarkerName)(
		PrTimelineID inTimelineID, 
		csSDK_uint32 inMarkerIndex, 
		PrMemoryPtr* outMarkerName);

	/*
	**	Retrieve the marker start time
	**
	**	@param	inTimelineID			the timeline instance data used for all marker suite communication,
	**									it is passed into the compiler instance in the compDoCompileInfo8 struct
	**	@param	inMarkerIndex			the zero-based index of the marker you're interested in. Must be within range.
	**	@param	outStartTime			the start time of the marker.
	*/
	prSuiteError (*GetMarkerStartTime)(
		PrTimelineID inTimelineID, 
		csSDK_uint32 inMarkerIndex,
		PrTime* outStartTime);

	/*
	**	Retrieve the marker duration - may be zero length.
	**
	**	@param	inTimelineID			the timeline instance data used for all marker suite communication,
	**									it is passed into the compiler instance in the compDoCompileInfo8 struct
	**	@param	inMarkerIndex			the zero-based index of the marker you're interested in. Must be within range.
	**	@param	outMarkerDuration		the duration of the marker. May be zero length if the marker has no duration.
	*/
	prSuiteError (*GetMarkerDuration)(
		PrTimelineID inTimelineID, 
		csSDK_uint32 inMarkerIndex,
		PrTime* outMarkerDuration);

	/*
	**	Retrieve a PrFlashCueMarkerID that can be used for retrieving flash cue marker data. It is legal
	**	for a marker to not be a flash cue marker, in which case ths outMarkerID will be zero. It is not necessary
	**	to call DisposeFlashCueMarkerID if the marker ID is zero, but it is required if it is non-zero.
	**
	**	See PrSDKFlashCueMarkerDataSuite for details.
	**
	**	@param	inTimelineID			the timeline instance data used for all marker suite communication,
	**									it is passed into the compiler instance in the compDoCompileInfo8 struct
	**	@param	inMarkerIndex			the zero-based index of the marker you're interested in. Must be within range.
	**	@param	outMarkerID				if this is a flash cue marker, outMarkerID will be non-zero
	*/
	prSuiteError (*GetFlashCueMarkerID)(
		PrTimelineID inTimelineID,
		csSDK_uint32 inMarkerIndex,
		PrFlashCueMarkerID* outMarkerID);

	/*
	**	Dispose of a PrFlashCueMarkerID when you no longer need it.
	**
	**	@param	inMarkerID				a flash cue marker ID.
	*/
	prSuiteError (*DisposeFlashCueMarkerID)(
		PrFlashCueMarkerID inMarkerID);

	/*
	*/
	prMarkerRef (*GetFirstMarker)(
		PrTimelineID inTimelineID);

	/*
	*/
	prMarkerRef (*GetNextMarker)(
		PrTimelineID timelineData, 
		prMarkerRef prev);

	/*
	*/
	prSuiteError (*GetMarkerData)(
		PrTimelineID timelineData, 
		prMarkerRef marker, 
		csSDK_uint32 prMarkerDataType, 
		void* data);

} PrSDKMarkerSuite;

#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif
