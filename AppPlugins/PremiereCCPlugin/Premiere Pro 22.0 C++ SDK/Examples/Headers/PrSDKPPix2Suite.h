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

#ifndef	PRSDKPPIX2SUITE_H
#define PRSDKPPIX2SUITE_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKPIXELFORMAT_H
#include "PrSDKPixelFormat.h"
#endif 

#ifndef PRSDKPPIXSUITE_H
#include "PrSDKPPixSuite.h"
#endif

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKMALERRORS_H
#include "PrSDKMALErrors.h"
#endif

#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif

#define kPrSDKPPix2Suite			"Premiere PPix 2 Suite"
// Version 1 added GetSize
#define kPrSDKPPix2SuiteVersion1	1
// Version 2 adds GetYUV420PlanarBuffers
#define kPrSDKPPix2SuiteVersion2	2
// Version 3 adds GetOrigin and GetFieldType
#define kPrSDKPPix2SuiteVersion3	3

// Current
#define kPrSDKPPix2SuiteVersion		kPrSDKPPix2SuiteVersion3

typedef struct 
{
	/**
	**	This will return the total size of the ppix in bytes. 
	**
	**	@param	inPPixHand			The ppix handle you want to operate on. 
	**	@param	outSize				The size of the ppix in bytes.
	*/
	prSuiteError (*GetSize)(
		PPixHand			inPPixHand,
		size_t*				outSize);

	/**
	**	[Added in CS4]
	**	This will return the planar buffers and rowbytes for a PPixHand
	**	if the contained pixels are in a planar format, such as
	**	PrPixelFormat_YUV_420_MPEG2_FRAME_PICTURE_PLANAR_8u_601 
	**	PrPixelFormat_YUV_420_MPEG2_FIELD_PICTURE_PLANAR_8u_601
	**	PrPixelFormat_YUV_420_MPEG2_FRAME_PICTURE_PLANAR_8u_709
	**	PrPixelFormat_YUV_420_MPEG2_FIELD_PICTURE_PLANAR_8u_709
	**
	**	@param	inPPixHand			The ppix handle you want to operate on. 
	**	@param	inRequestedAccess	Will return an error if the source is read-only and the request is for write or read/write.
	**
	**	@param	out_xxx_PixelAddress	The output (Y, U, or V) pixel buffer address. May be NULL if the requested access is not supported.
	**	@param	out_xxx_RowBytes		Returns how many bytes must be added to the pixel buffer address to get to the next line.
	**									May be negative.
	*/
	prSuiteError (*GetYUV420PlanarBuffers)(
		PPixHand			inPPixHand,
		PrPPixBufferAccess	inRequestedAccess,
		char**				out_Y_PixelAddress,
		csSDK_uint32*		out_Y_RowBytes,
		char**				out_U_PixelAddress,
		csSDK_uint32*		out_U_RowBytes,
		char**				out_V_PixelAddress,
		csSDK_uint32*		out_V_RowBytes);
		
	/*
	**	[Added in CS5]
	**	This concept of the origin is primarily useful when using the VideoSegmentRenderSuite. In order to avoid unnecessary work this suite will not normalize to the intended output
	**	rectangle and thus you will need to handle this yourself. For example, if you are rendering a Sequence which is 720x480 and you import an image which is 110x100, it will have 
	**	an origin of -1, -1, implying that it should be centered (or left alone if it matches the desired output size.
	**	If you apply a Gaussian Blur to an image, then the origin will be a positive number that represents the soft edge padding size after the blur.
	**	If the origin is 0, 0, this implies that it has been set and the top-left corner should align with the logical top-left corner. A good test case for this is a Corner_Pin which
	**	leaves the top-left corner alone but modifies the bottom right corner.
	**
	**	@param inPPixHand		The ppix handle you want to know the origin of
	**	@param outOriginX		The origin of the ppix is a non-negative number which describes how much of the actual pixel buffer you need to skip to find the top-left origin of image. 
	**	@param outOriginY		If either of the X or Y origin is negative, that implies that the origin has never been set and the image is intended to be centered in the output window.
	**
	*/
	prSuiteError (*GetOrigin)(
		PPixHand			inPPixHand,
		csSDK_int32*		outOriginX,
		csSDK_int32*		outOriginY);
		
	/*
	**	[Added in CS5]
	**
	**	Almost all of the time, the field order of a PPix is clear - if it came from a clip, it will match the clip's. If it came from a render, it will match the Sequence.
	**	However, there are edge cases where a PPix will be unexpectedly progressive:
	**	- if you put a still on a timeline with no time-varying effects, the frame may be progressive.
	**	- if you render at quarter res and we do a quarter-res decompress of an interlaced format, the result is often progressive
	**  - if a time-remapping operation includes a hold frame, both fields may be the same time, and thus the result is progressive
	**  There may be other cases, but you get the idea.
	**
	**	@param inPPixHand		The ppix handle you want to know the field type of.
	**	@param outFieldType		The field type of the ppix.
	*/
	prSuiteError (*GetFieldOrder)(
		PPixHand inPPixHand,
		prFieldType* outFieldType);

} PrSDKPPix2Suite;

#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif
