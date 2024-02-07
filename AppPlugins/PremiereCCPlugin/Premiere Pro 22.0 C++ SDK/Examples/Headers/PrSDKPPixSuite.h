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

#ifndef	PRSDKPPIXSUITE_H
#define PRSDKPPIXSUITE_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKPIXELFORMAT_H
#include "PrSDKPixelFormat.h"
#endif 

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKMALERRORS_H
#include "PrSDKMALErrors.h"
#endif

#ifndef PRSDKQUALITY_H
#include "PrSDKQuality.h"
#endif

#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif

typedef enum 
{	
	PrPPixBufferAccess_ReadOnly = 0,
	PrPPixBufferAccess_WriteOnly = 1,
	PrPPixBufferAccess_ReadWrite = 2,
	PrPPixBufferAccess_ForceEnumSize = kPrForceEnumToIntValue
} PrPPixBufferAccess;

#define kPrSDKPPixSuite				"Premiere PPix Suite"
#define kPrSDKPPixSuiteVersion		1

typedef struct 
{
	/**
	**	This will free this ppix. The ppix is no longer valid after this function is called.
	**
	**	@param	inPPixHand				The ppix handle you want to dispose. 
	*/
	prSuiteError (*Dispose)(
		PPixHand			inPPixHand);

	/**
	**	This will return a pointer to the pixel buffer. 
	**
	**	@param	inPPixHand			The ppix handle you want to operate on. 
	**	@param	inRequestedAccess	Requested pixel access. Most PPixs do not support write access modes.
	**	@param	outPixelAddress		The output pixel buffer address. May be NULL if the requested pixel access is not supported.
	*/
	prSuiteError (*GetPixels)(
		PPixHand			inPPixHand,
		PrPPixBufferAccess	inRequestedAccess,
		char**				outPixelAddress);

	/**
	**	This will return the bounding rect. 
	**
	**	@param	inPPixHand			The ppix handle you want to operate on. 
	**	@param	outBoundingRect		The address of a bounding rect to be filled in.
	*/
	prSuiteError (*GetBounds)(
		PPixHand			inPPixHand,
		prRect*				inoutBoundingRect);

	/**
	**	This will return the row bytes of the ppix.
	**
	**	@param	inPPixHand			The ppix handle you want to operate on. 
	**	@param	outRowBytes			Returns how many bytes must be added to the pixel buffer address to get to the next line.
	**								May be negative.
	*/
	prSuiteError (*GetRowBytes)(
		PPixHand			inPPixHand,
		csSDK_int32*		outRowBytes);

	/**
	**	This will return the pixel aspect ratio of this ppix.
	**
	**	@param	inPPixHand						The ppix handle you want to operate on. 
	**	@param	outPixelAspectRatioNumerator	Returns the numerator of the pixel aspect ratio.
	**	@param	outPixelAspectRatioDenominator	Returns the denominator of the pixel aspect ratio.
	*/
	prSuiteError (*GetPixelAspectRatio)(
		PPixHand			inPPixHand,
		csSDK_uint32*		outPixelAspectRatioNumerator,		
		csSDK_uint32*		outPixelAspectRatioDenominator);

	/**
	**	This will return the pixel format of this ppix.
	**
	**	@param	inPPixHand			The ppix handle you want to operate on. 
	**	@param	outPixelFormat		Returns the pixel format of this ppix.
	*/
	prSuiteError (*GetPixelFormat)(
		PPixHand			inPPixHand,
		PrPixelFormat*		outPixelFormat);

	/**
	**	This will return the unique key for this ppix.
	**
	**	@param	inPPixHand			The ppix handle you want to operate on. 
	**	@param	outKeyBuffer		Returns the pixel format of this ppix.
	**
	**	[TODO] Fill in returned error codes.
	**	@returns Error if the buffer size is too small (call GetUniqueKeySize() to get the correct size).
	**	@returns Error if the key is not available.
	**	@returns Success if the key data was filled in.
	*/
	prSuiteError (*GetUniqueKey)(
		PPixHand			inPPixHand,
		unsigned char*		inoutKeyBuffer,
		size_t				inKeyBufferSize);

	/**
	**	This will return the unique key size. 
	**	This will not change for the entire run of the application.
	**
	**	@param	outKeyBufferSize	Returns the size of the ppix unique key.
	*/
	prSuiteError (*GetUniqueKeySize)(
		size_t*				outKeyBufferSize);

	/**
	**	This will return the render time for this ppix.
	**
	**	@param	inPPixHand				The ppix handle you want to operate on. 
	**	@param	outRenderMilliseconds	Returns the render time in milliseconds. If the frame was cached, this time will be 0.
	*/
	prSuiteError (*GetRenderTime)(
		PPixHand			inPPixHand,
		csSDK_int32*		outRenderMilliseconds);

} PrSDKPPixSuite;

#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif
