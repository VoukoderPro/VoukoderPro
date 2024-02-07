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

//	This file is part of the public Premiere SDK.


#ifndef	PRSDKPIXELFORMATSUITE_H
#define PRSDKPIXELFORMATSUITE_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKPIXELFORMAT_H
#include "PrSDKPixelFormat.h"
#endif 

#ifndef PRSDKMALERRORS_H
#include "PrSDKMALErrors.h"
#endif

#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif

#define kPrSDKPixelFormatSuite				"Premiere Pixel Format Suite"
#define kPrSDKPixelFormatSuiteVersion		1

typedef struct 
{
	/**
	**	Retrieves the minimum i.e. "black" value for a give pixel type.
	**
	**	[NOTE]	pixel types like YUY2, YUYV actually contain a group of two pixels to specify a color
	**			completely, so the data size returned in this case will be 4 bytes (rather than 2)
	**
	**	@param		pixelFormat				the Premiere pixel format whose black level you want
	**	@param		pixelData				a void pointer to data large enough to hold the pixel value (see note above)
	**
	**	@returns	suiteError_NoError : successful retreival of black value
	**	@returns	suiteError_Fail : 
	*/

	prSuiteError (*GetBlackForPixelFormat)(
					PrPixelFormat		pixelFormat,
					void*				pixelData);

	/**
	**	Retrieves the maximum i.e. "white" value for a give pixel type.
	**
	**	[NOTE]	pixel types like YUY2, YUYV actually contain a group of two pixels to specify a color
	**			completely, so the data size returned in this case will be 4 bytes (rather than 2)
	**
	**	@param		pixelFormat				the Premiere pixel format whose white level you want
	**	@param		pixelData				a void pointer to data large enough to hold the pixel value (see note above)
	**
	**	@returns	suiteError_NoError : successful retreival of white value
	**	@returns	suiteError_Fail : 
	*/

	prSuiteError (*GetWhiteForPixelFormat)(
					PrPixelFormat		pixelFormat,
					void*				pixelData);

	/**
	**	Converts an alpha, red, green, blue specification into a pixel value for a give pixel type.
	**
	**	[NOTE]	pixel types like YUY2, YUYV actually contain a group of two pixels to specify a color
	**			completely, so the data size returned in this case will be 4 bytes (rather than 2)
	**
	**	@param		pixelFormat				the Premiere pixel format whose white level you want
	**	@param		alpha					alpha value (0.0 - 1.0)
	**	@param		red						red value (0.0 - 1.0)
	**	@param		green					green value (0.0 - 1.0)
	**	@param		blue					blue value (0.0 - 1.0)
	**	@param		pixelData				a void pointer to data large enough to hold the pixel value (see note above)
	**
	**	@returns	suiteError_NoError : successful conversion of color value
	**	@returns	suiteError_Fail : 
	*/

	prSuiteError (*ConvertColorToPixelFormattedData)(
					PrPixelFormat		pixelFormat,
					float				alpha,
					float				red,
					float				green,
					float				blue,
					void*				pixelData);

} PrSDKPixelFormatSuite;

#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif /* PRSDKPIXELFORMATSUITE_H */

