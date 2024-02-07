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

#ifndef	PRSDKIMAGEPROCESSINGSUITE_H
#define PRSDKIMAGEPROCESSINGSUITE_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKPIXELFORMAT_H
#include "PrSDKPixelFormat.h"
#endif 

#ifndef PRSDKQUALITY_H
#include "PrSDKQuality.h"
#endif

#ifndef PRSDKMALERRORS_H
#include "PrSDKMALErrors.h"
#endif

#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif

#define kPrSDKImageProcessingSuite			"Premiere Image Processing Suite"

#define kPrSDKImageProcessingSuiteVersion1	1
#define kPrSDKImageProcessingSuiteVersion2	2

// Current
#define kPrSDKImageProcessingSuiteVersion		kPrSDKImageProcessingSuiteVersion2

typedef struct 
{
	/*
	**	@param inPixelFormat - the format in question
	**	@param outIsCompressed - a bool that says if the format is considered a compressed format or not
	*/
	prSuiteError (*FormatIsCompressed)(
		PrPixelFormat inPixelFormat,
		prBool*	outIsCompressed
	);

	/*
	**	@param inPixelFormat - the format in question
	**	@param outHasAlpha - a bool that says if the format has a valid alpha channel or not
	*/
	prSuiteError (*FormatHasAlpha)(
		PrPixelFormat inPixelFormat,
		prBool*	outHasAlpha		
	);

	/*
	**	@param inPixelFormat - the format in question
	**	@param outIsTopLeft - a bool that says if the format should be considered origin top left, or origin bottom left
	**	At the moment this is pretty hard wired in the host - 4444 formats are bottom left, all others are top left, but in future releases this might change
	*/
	prSuiteError (*FormatIsOriginTopLeft)(
		PrPixelFormat inPixelFormat,
		prBool*	outIsTopLeft		
	);

	/*
	**	@param inPixelFormat - the format in question
	**	@param inWidth
	**	@param inHeight - the width and height of the desired buffer
	**	@param outSizeToAllocate - the size of buffer needed to store that pixel format at that size, in a buffer. 
	**	If you would like to know the desired rowbytes for the host app for that format, pass in a height of 1.
	*/
	prSuiteError (*GetSizeForPixelBuffer)(
		PrPixelFormat inPixelFormat,
		csSDK_int32 inWidth,
		csSDK_int32 inHeight,
		csSDK_int32* outSizeToAllocate
	);

	/*
	*/
	prSuiteError (*ScaleConvert)(
		PrPixelFormat inSrcPixelFormat,
		csSDK_int32 inSrcWidth,
		csSDK_int32 inSrcHeight,
		csSDK_int32 inSrcRowBytes,
		prFieldType inSrcFieldType,
		void* inSrcData,
		
		PrPixelFormat inDestPixelFormat,
		csSDK_int32 inDestWidth,
		csSDK_int32 inDestHeight,
		csSDK_int32 inDestRowBytes,
		prFieldType inDestFieldType,
		void* inDestData,
		
		PrRenderQuality inQuality
	);

	/*
	*/
	prSuiteError (*CompressDV100720p)(
		PrPixelFormat inSrcPixelFormat,
		csSDK_int32 inSrcWidth,
		csSDK_int32 inSrcHeight,
		csSDK_int32 inSrcRowBytes,
		void * inSrcData,
		
		PrPixelFormat inDestPixelFormat,
		csSDK_int32 inDestBufferLength,
		void* inDestData,

		csSDK_int32 inCompressedChannelOffset, /*either 0 for even frames or 2 for odd frames - if this doesn't mean anything to you, you should probably not be using this function*/
		PrRenderQuality inQuality);

	/*
	*/
	prSuiteError (*StampDVFrameAspect)(
		PrPixelFormat inPixelFormat,
		void* inData,
		csSDK_int32 inDataSize,
		prBool inStampAs16x9);
		
} PrSDKImageProcessingSuite;

#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif //PRSDKIMAGEPROCESSINGSUITE_H
