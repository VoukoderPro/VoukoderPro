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



#ifndef PRSDKGPUIMAGEPROCESSINGSUITE_H
#define PRSDKGPUIMAGEPROCESSINGSUITE_H


#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKMALERRORS_H
#include "PrSDKMALErrors.h"
#endif

#ifndef PRSDKPIXELFORMAT_H
#include "PrSDKPixelFormat.h"
#endif

#ifndef PRSDKQUALITY_H
#include "PrSDKQuality.h"
#endif



#pragma pack(push, 1)
#ifdef __cplusplus
extern "C" {
#endif



#define kPrSDKGPUImageProcessingSuite				"MediaCore GPU Image Processing Suite"
#define kPrSDKGPUImageProcessingSuiteVersion		1



/**
**	Access to common GPU image processing algorithms
*/
typedef struct
{
	/**
	**	Convert between formats on the GPU
	**		One of inSrcPixelFormat or inDestPixelFormat must be a host format,
	**	the other must be either PrPixelFormat_GPU_BGRA_4444_16f or PrPixelFormat_GPU_BGRA_4444_32f
	*/
	prSuiteError (*PixelFormatConvert)(
		csSDK_uint32 inDeviceIndex,
		const void* inSrc,
		csSDK_int32 inSrcRowBytes,
		PrPixelFormat inSrcPixelFormat,
		void* inDest,
		csSDK_int32 inDestRowBytes,
		PrPixelFormat inDestPixelFormat,
		csSDK_uint32 inWidth,
		csSDK_uint32 inHeight,
		PrRenderQuality inQuality);

	/**
	**	Scale a frame on the GPU
	**		inPixelFormat must be PrPixelFormat_GPU_BGRA_4444_16f or PrPixelFormat_GPU_BGRA_4444_32f
	*/
	prSuiteError (*Scale)(
		csSDK_uint32 inDeviceIndex,
		const void* inSrc,
		csSDK_int32 inSrcRowBytes,
		csSDK_uint32 inSrcWidth,
		csSDK_uint32 inSrcHeight,
		void* inDest,
		csSDK_int32 inDestRowBytes,
		csSDK_uint32 inDestWidth,
		csSDK_uint32 inDestHeight,
		PrPixelFormat inPixelFormat,
		float inScaleX,
		float inScaleY,
		PrRenderQuality inQuality);

	/**
	**	Gaussian blur on the GPU
	**		inPixelFormat must be PrPixelFormat_GPU_BGRA_4444_16f or PrPixelFormat_GPU_BGRA_4444_32f
	*/
	prSuiteError (*GaussianBlur)(
		csSDK_uint32 inDeviceIndex,
		const void* inSrc,
		csSDK_int32 inSrcRowBytes,
		csSDK_uint32 inSrcWidth,
		csSDK_uint32 inSrcHeight,
		void* inDest,
		csSDK_int32 inDestRowBytes,
		csSDK_uint32 inDestWidth,
		csSDK_uint32 inDestHeight,
		PrPixelFormat inPixelFormat,
		float inSigmaX,
		float inSigmaY,
		prBool inRepeatEdgePixels,
		prBool inBlurHorizontally,
		prBool inBlurVertically,
		PrRenderQuality inQuality);

} PrSDKGPUImageProcessingSuite;



#ifdef __cplusplus
}
#endif

#pragma pack(pop)


#endif
