/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2000 Adobe Systems Incorporated                       */
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


#ifndef PRSDKPLUGTIMELINE_H
#define PRSDKPLUGTIMELINE_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKPIXELFORMAT_H
#include "PrSDKPixelFormat.h"
#endif

#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif

// flags for getClipVideo callback
#define kGCVFlag_UseFilePixelAspectRatio	1

// utility functions for plugins that want to access timeline information
typedef csSDK_int32 (*plugGetClipVideoFunc)(csSDK_int32 frame, PPixHand thePort, prRect *bounds, csSDK_int32 flags, PrClipID clipData);
typedef csSDK_int32 (*plugGetWorkAreaFunc)(PrTimelineID timelineData, csSDK_int32 *workAreaStart, csSDK_int32 *workAreaEnd);
typedef void (*plugGetCurrentTimebaseFunc)(PrTimelineID timelineData, csSDK_uint32 *scale, csSDK_int32 *sampleSize);
typedef csSDK_int32 (*plugGetCurrentPosFunc)(PrTimelineID timelineData);
typedef csSDK_int32 (*plugGetPreviewFrameExFunc)(PrTimelineID inTimelineData, csSDK_int32 inFrame,
											PPixHand*		outRenderedFrame,					
											const prRect*	inFrameRect,						
											PrPixelFormat*	inRequestedPixelFormatArray,
											csSDK_int32		inRequestedPixelFormatArrayCount,
											csSDK_uint32		inPixelAspectRatioNumerator,		
											csSDK_uint32		inPixelAspectRatioDenominator,
											bool			inAlwaysRender);
typedef csSDK_int32 (*plugGetClipVideoBoundsFunc)(PrClipID inClipData, 
											prRect *outBounds, 
											csSDK_uint32 *outPixelAspectRatioNumerator, 
											csSDK_uint32 *outPixelAspectRatioDenominator);
typedef csSDK_int32 (*plugGetClipVideoExFunc)(csSDK_int32 inFrame, 
										PPixHand* outRenderedFrame, 
										const prRect*	inFrameRect, 
										const PrPixelFormat* inRequestedPixelFormatArray,
									    csSDK_int32		inRequestedPixelFormatArrayCount,
										csSDK_uint32		inPixelAspectRatioNumerator,		
										csSDK_uint32		inPixelAspectRatioDenominator,
										PrClipID inClipData);

typedef struct
{
   void*					unused2;
   void*					unused3;
   plugGetClipVideoFunc				getClipVideo;
   void*					unused1;			// Not Supported
   void*					unused8;
   void*					unused9;
   void*					unused10;
   plugGetWorkAreaFunc				getWorkArea;
   void*					unused7;
   void*					unused4;
   plugGetCurrentTimebaseFunc		getCurrentTimebase;
   plugGetCurrentPosFunc			getCurrentPos;
   void*					unused5;
   void*					unused6;
   plugGetPreviewFrameExFunc		getPreviewFrameEx;
   plugGetClipVideoBoundsFunc		getClipVideoBounds;
   plugGetClipVideoExFunc			getClipVideoEx;
} PlugTimelineFuncs, *PlugTimelineFuncsPtr;

#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif /* PRSDKPLUGTIMELINE_H */

/**
************************ Notes for Version 7 *************************
**
**	Implementation of these callbacks have changed in version 7. It is no
**	longer acceptable to pass NULL as the timelineData Handle to any of these
**	functions. All calls MUST pass the timelineData Handle that was passed
**	to the plugin. All calls with NULL passed as the timelineData will return
**	errors and invalid data.
**
**	If a particular plugin type does not have a timelineData value available to
**	them (e.g. Importers), then the timeline suite is unusable by them.
**
**	The functions that have been deprecated and are not supported are:
**
**	getClipAudioSecond
**	getCurrentClipPos
**	invalCutList
**
**********************************************************************
*/


/*
FUNCTION
getVideoSegment
DESCRIPTION
This function constructs the segment information tree that corresponds
to the specified timeline position and data.
PARAMETERS
frame: The position of the frame reltive to the start of the 
       edited/executed section of the timeline.  
       Zero represents the start of the section that is currently
       being edited/executed.  Once you get the first segment
       at frame zero, inside the segment the inListPos tells
       the real position of the editing operation.
timelineData: A handle to the privately-defined timeline data,
       as provided in the EffectRecord, VideoRecord, compDoCompileInfo 
	   and playMod newList and Open recs.
RETURN VALUE
A pointer to the newly created segment information, or NULL if
an error occured (or if the value of "frame" is invalid).
NOTE
Don't forget to destroy the segment with the "disposeSegment"
function when finished, or else there will be memory leaks.

FUNCTION
disposeSegment
DESCRIPTION
This function frees up a segment tree that was allocated during a call
to the "getVideoSegment" callback
PARAMETERS
segment: A pointer to the segment.
RETURN VALUE
None

FUNCTION
getClipVideo
DESCRIPTION
This function gets the video frame data associated with a specific
clip.  That clip is part of a segment tree that is either generated
by a call to getVideoSegment, or passed to the playback module through
the standard "playmod_ListAddRTVideoSegment" call.
PARAMETERS
frame: The frame number of the image to get, relative to the start and 
       timebase of the file
thePort: A pointer to an allocated PPixHand image.
bounds: The bounding rectangle of the image
flags: GetVideo flags: (ex.: gv24Bit, gv32Bit, etc....)
clipData: The handle that can be found in the "clipData" field of
          the "prtFileRec" structure in the segment tree.
RETURN VALUE:
0 if successful, -1 if an error occured

FUNCTION
getClipAudioSecond
DESCRIPTION
Not supported in version 7
RETURN VALUE:
-1

FUNCTION
getFirstMarker
DESCRIPTION
This function returns the first timeline (by time value) marker or kInvalidMarkerRef if there are none.  prtMarkerRef is opaque,
use the accesors to access it
PARAMETERS
none
RETURN VALUE:
0 if there are no markers, markerRef is successful

FUNCTION
getNextMarker
DESCRIPTION
This function returns the next timeline (by time value) marker or kInvalidMarkerRef if there are no more.  prtMarkerRef is opaque,
use the accesors to access it
PARAMETERS
prev: The marker returned by either getFirstMarker or the last call to get next marker
RETURN VALUE:
0 if there are no markers, markerRef is successful

FUNCTION
getMarkerData
DESCRIPTION
This function returns a member of an opaque markerRef.
PARAMETERS
marker:  the marker you want data from
prtMarkerDataType: the data type you want, see prSDKRealTime.h
data: a pointer to a long that will return the data
RETURN VALUE:

FUNCTION
getWorkArea
DESCRIPTION
This function returns the start and end of the current timeline work area
PARAMETERS
Handle timelineData: timeline data handle.
workAreaStart: a pointer to a long that will receive the starting frame number of the timeline work area
workAreaEnd: a pointer to a long that will receive the ending frame number of the work area (exclusive)
RETURN VALUE:
0 if successful

FUNCTION
invalRTRanges
DESCRIPTION
This function invalidate the current RT ranges.  A new round of playmod_GetPlayableRTRange calls will
be sent to the playmod so that the RT ranges can be refreshed on the timeline UI.
NOTE: If you call this function from a playback module, be prepared to be called recursively,
since some invalidations will generate immediate RT ranges updates to be query from the playmod.   
This callback should be used only by playmods using the RT interface (playmod_ListAddRTVideoSegment), 
or compilers with the same class as RT play modules.
PARAMETERS
Handle timelineData: timeline data handle.
invalStart: first frame of the range to invalidate
invalEnd: last frame of the range to invalidate (exclusive)
			Pass -1 to invalidate the whole timeline
RETURN VALUE:
none

FUNCTION
getCurrentTimebase
DESCRIPTION
Retrieves the current timebase of the timeline (scale + sampleSize)
PARAMETERS
Handle timelineData: timeline data handle.
long *scale:  a pointer to the variable that will receive the timebase scale
long *sampleSize: a pointer to the var that will receive the sample size
RETURN VALUE:
none

FUNCTION
getCurrentPos
DESCRIPTION
Retrieves the current position of the timeline (the position bar set by the user)
PARAMETERS
Handle timelineData: (unused) timeline data handle.  For future expansion only.
RETURN VALUE:
the current frame position, specified in the timebase returned by getCurrentTimebase.  
If -1 is returned, it means that no current position is set a the present, and the position
bar is invisible.

FUNCTION
getPreviewFrame
Gets a fully rendered frame from the timeline (all layers).  Used for preview purposes only 
(not to be used by compilers that are rendering final movies)
PARAMETERS
Handle timelineData: timeline data handle. 
long frame: The frame to get, specified in the current timebase.
PPixHand thePort: the destination buffer.  Has to be allocated prior to this call by the plug-in,
             using the PPix callbacks suite.  Also needs to be freed by the caller when done.
long reserved: unused
RETURN VALUE:
0: success rendering the frame, the resulting image is put in "thePort" parameter
negative value: an error has occured

FUNCTION
getCurrentClipPos
DESCRIPTION
Not supported in version 7
RETURN VALUE:
-1

*/


