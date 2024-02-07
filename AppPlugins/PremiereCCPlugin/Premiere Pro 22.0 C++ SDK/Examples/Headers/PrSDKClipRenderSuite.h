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

#ifndef	PRSDKCLIPRENDERSUITE_H
#define PRSDKCLIPRENDERSUITE_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKPLUGTIMELINE_H
#include "PrSDKPlugTimeline.h"
#endif

#ifndef PRSDKPIXELFORMAT_H
#include "PrSDKPixelFormat.h"
#endif 

#ifndef PRSDKMALERRORS_H
#include "PrSDKMALErrors.h"
#endif

#ifndef PRSDKQUALITY_H
#include "PrSDKQuality.h"
#endif

#ifndef PRSDKTIMESUITE_H
#include "PrSDKTimeSuite.h"
#endif

#ifndef PRSDKRENDERCACHETYPE_H
#include "PrSDKRenderCacheType.h"
#endif

#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif

#define kPrSDKClipRenderSuite				"Premiere Clip Render Suite"
#define kPrSDKClipRenderSuiteVersion		3

typedef struct 
{
	PrPixelFormat		inPixelFormat;
	csSDK_int32			inWidth;
	csSDK_int32			inHeight;
} ClipFrameFormat;

typedef struct 
{
	/**
	**	Does this clip support the clip render suite functions?
	**
	**	@param	inClipID							The ClipID of the clip.
	**	@param	outSupported						If true, the clip supports the ClipRenderSuite.
	**	@param	outAsyncIOSupported					If true, the clip supports asynchronous IO.
	**												If false, the clip does not directly support asynchronous IO,
	**												however it is still legal to call InitiateAsyncRead.
	**												Pass nil if you do not need this information.
	*/
	prSuiteError (*SupportsClipRenderSuite)(
		PrClipID				inClipID,
		prBool*					outSupported,
		prBool*					outAsyncIOSupported);

	/**
	**	Given a specific clip, find out the number of pixel formats natively supported
	**	by this clip.
	**
	**	@param	inClipID							The ClipID of the clip.
	**	@param	outNumPixelFormats					The number of pixel formats.
	*/
	prSuiteError (*GetNumPixelFormats)(
		PrClipID				inClipID,
		csSDK_int32*			outNumPixelFormats);
	
	/**
	**	Given a specific clip get a native pixel format. The pixel formats will be returned
	**	in index order based on preference. The maximum index can be determined by using
	**	GetNumPixelFormats().
	**
	**	@param	inClipID							The ClipID of the clip.
	**	@param	inIndex								The pixel format index.
	**	@param	outPixelFormat						The pixel format.
	*/
	prSuiteError (*GetPixelFormat)(
		PrClipID				inClipID,
		csSDK_int32				inIndex,
		PrPixelFormat*			outPixelFormat);

	/**
	**	Given a specific clip and pixel format, find out the number of preferred
	**	frame sizes for this clip. A preferred frame size is a size which represents
	**	a "native" size of the media.
	**
	**	@param	inClipID							The ClipID of the clip.
	**	@param	inPixelFormat						The pixel format to query for preferrred sizes.
	**	@param	outNumPreferredFrameSizes			The number of sizes for this clip and pixel format.
	*/
	prSuiteError (*GetNumPreferredFrameSizes)(
		PrClipID				inClipID,
		PrPixelFormat			inPixelFormat,
		csSDK_int32*			outNumPreferredFrameSizes);
	
	/**
	**	Given a specific clip and pixel format, get a preferred size. The sizes will be returned
	**	in index order based on preference. The maximum index can be determined by using
	**	GetNumPreferredFrameSizes().
	**
	**	@param	inClipID							The ClipID of the clip.
	**	@param	inPixelFormat						The pixel format to query for preferrred sizes.
	**	@param	inIndex								The preferred size index.
	**	@param	outWidth							The width of the preferred size. 0 means any width.
	**	@param	outHeight							The height of the preferred size. 0 means any height.
	*/
	prSuiteError (*GetPreferredFrameSize)(
		PrClipID				inClipID,
		PrPixelFormat			inPixelFormat,
		csSDK_int32				inIndex,
		csSDK_int32*			outWidth,
		csSDK_int32*			outHeight);
	
	/**
	**	Initiate an asynchronous read for the specified frame. Note that you cannot specify
	**	a specific frame format here. The clip will prefetch whatever data possible to
	**	facilitate a faster decode, and return.
	**
	**	@param	inClipID							The ClipID of the clip.
	**	@param	inFrameTime							The time of the video frame number.
	**	@param	inFormat							The preferred format for the frame.
	*/
	prSuiteError (*InitiateAsyncRead)(
		PrClipID				inClipID,
		const PrTime*			inFrameTime,
		ClipFrameFormat*		inFormat);
	
	/**
	**	Cancel an asynchronous read for the specified frame. Note that you cannot specify
	**	a specific frame format here. 
	**
	**	@param	inClipID							The ClipID of the clip.
	**	@param	inFrameTime							The time of the video frame number.
	**	@param	inFormat							The preferred format for the frame.
	*/
	prSuiteError (*CancelAsyncRead)(
		PrClipID				inClipID,
		const PrTime*			inFrameTime,
		ClipFrameFormat*		inFormat);

	/**
	**	Find a frame of a specific format in the cache. This function will first look
	**	for a match in the supplied list of formats. If it does not find one, it will
	**	ask the importer to decode one out of cached raw data. It will not access the
	**	disk for new raw data, so it will fail if the required data is not in the cache.
	**
	**	@param  inClipID							The ClipID of the clip.
	**	@param	inFrameTime							The time of the video frame number.
	**	@param	inFormats							An array of formats that would be acceptable.
	**												Each format must be a combination of
	**												supported pixel formats and preferred frame
	**												sizes. May be NULL, in which case in format
	**												may be returned.
	**	@param	inNumFormats						The number of items in inFormats. May be 0.
	**	@param	inSynchronous						If true, the function will render the file from disk if necessary
	**	@param	outFrame							The found frame. May be NULL if it was not found.
	*/	
	prSuiteError (*FindFrame)(
		PrClipID				inClipID,
		const PrTime*			inFrameTime,
		ClipFrameFormat*		inFormats,
		csSDK_int32				inNumFormats,
		bool					inSynchronous,
		PPixHand*				outFrame);

	/**
	**	What is the field type of this clip? Added in version 2.
	**
	**	@param	inClipID							The ClipID of the clip.
	**	@param	outFieldType						See prFieldType in PrSDKTypes.h
	*/
	prSuiteError (*GetClipFieldType)(
		PrClipID				inClipID,
		prFieldType*			outFieldType);

	/**
	**	Given a specific clip, find out the number of custom pixel formats natively supported
	**	by this clip.
	**
	**	@param	inClipID							The ClipID of the clip.
	**	@param	outNumPixelFormats					The number of pixel formats.
	*/
	prSuiteError (*GetNumCustomPixelFormats)(
		PrClipID				inClipID,
		csSDK_int32*			outNumPixelFormats);
	
	/**
	**	Given a specific clip get a native pixel format. The pixel formats will be returned
	**	in index order based on preference. The maximum index can be determined by using
	**	GetNumPixelFormats().
	**
	**	@param	inClipID							The ClipID of the clip.
	**	@param	inIndex								The pixel format index.
	**	@param	outPixelFormat						The pixel format.
	*/
	prSuiteError (*GetCustomPixelFormat)(
		PrClipID				inClipID,
		csSDK_int32				inIndex,
		PrPixelFormat*			outPixelFormat);
} PrSDKClipRenderSuite;

#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif
