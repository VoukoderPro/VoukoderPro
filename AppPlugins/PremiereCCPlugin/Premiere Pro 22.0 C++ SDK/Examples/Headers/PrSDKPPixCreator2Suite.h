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

#ifndef	PRSDKPPIXCREATOR2SUITE_H
#define PRSDKPPIXCREATOR2SUITE_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKPIXELFORMAT_H
#include "PrSDKPixelFormat.h"
#endif 

#ifndef PRSDKMALERRORS_H
#include "PrSDKMALErrors.h"
#endif

#ifndef PRSDKPPIXSUITE_H
#include "PrSDKPPixSuite.h"
#endif 

#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif

#define kPrSDKPPixCreator2Suite				"Premiere PPix Creator 2 Suite"
#define kPrSDKPPixCreator2SuiteVersion1		1
#define kPrSDKPPixCreator2SuiteVersion2		2
#define kPrSDKPPixCreator2SuiteVersion3		3
#define kPrSDKPPixCreator2SuiteVersion4		4	// Pr 14.0; color managed extensions
#define kPrSDKPPixCreator2SuiteVersion		kPrSDKPPixCreator2SuiteVersion4

typedef struct 
{
	/**
	**	This will create a new ppix. 
	**
	**	@param	outPPixHand						The new ppix handle if the create was successfull. NULL otherwise. 
	**	@param	inRequestedAccess				Requested pixel access. ReadOnly is not allowed (doesn't make sense).
	**	@param	inPixelFormat					The pixel format of this ppix.
	**	@param	inWidth							The horizontal size of the ppix.
	**	@param	inHeight						The vertical size of the ppix.
	**	@param	inUseFields						If true, the ppix is a field.
	**	@param	inFieldNumber					If inUseFields is true, this specifies first field (0) or second (1)
	**	@param	inPARNumerator					The pixel aspect ratio numerator for the ppix.
	**	@param	inPARDenominator				The pixel aspect ratio denominator for the ppix.
	*/
	prSuiteError (*CreatePPix)(
		PPixHand*			outPPixHand,
		PrPPixBufferAccess	inRequestedAccess,
		PrPixelFormat		inPixelFormat,
		int					inWidth,
		int					inHeight,
		bool				inUseFields,
		int					inFieldNumber,
		int					inPARNumerator,
		int					inPARDenominator);
	
	/**
	**	This will create a new raw ppix.
	**
	**	@param	outPPixHand						The new ppix handle if the create was successfull. NULL otherwise. 
	**	@param	inRequestedAccess				Requested pixel access. ReadOnly is not allowed (doesn't make sense).
	**	@param	inSize							The size of the ppix in bytes.
	**	@param	inAlignment						The alignment of the beginning of the ppix in bytes.
	*/
	prSuiteError (*CreateRawPPix)(
		PPixHand*			outPPixHand,
		PrPPixBufferAccess	inRequestedAccess,
		int					inSize,
		int					inAlignment);


	/**
	**	This will create a new ppix in a custom pixel format. 
	**
	**	@param	outPPixHand						The new ppix handle if the create was successfull. NULL otherwise. 
	**	@param	inRequestedAccess				Requested pixel access. ReadOnly is not allowed (doesn't make sense).
	**	@param	inPixelFormat					The pixel format of this ppix.
	**	@param	inWidth							The horizontal size of the ppix.
	**	@param	inHeight						The vertical size of the ppix.
	**	@param	inPARNumerator					The pixel aspect ratio numerator for the ppix.
	**	@param	inPARDenominator				The pixel aspect ratio denominator for the ppix.
	**	@param	inDataBufferSize				The number of bytes requested, not including the header.
	*/
	prSuiteError (*CreateCustomPPix)(
		PPixHand*			outPPixHand,
		PrPPixBufferAccess	inRequestedAccess,
		PrPixelFormat		inPixelFormat,
		int					inWidth,
		int					inHeight,
		int					inPARNumerator,
		int					inPARDenominator,
		int					inDataBufferSize);
	
	/**
	**	This will create a new ppix properly aligned for reading from disk. 
	**
	**	@param	outPPixHand									The new ppix handle if the create was successfull. NULL otherwise. 
	**	@param	inPixelFormat								The pixel format of this ppix.
	**	@param	inWidth										The horizontal size of the ppix.
	**	@param	inHeight									The vertical size of the ppix.
	**	@param	inPARNumerator								The pixel aspect ratio numerator for the ppix.
	**	@param	inPARDenominator							The pixel aspect ratio denominator for the ppix.
	**	@param	inMemoryAlignment							The alignment of memory required for the start of the first sector.
	**														If this value is zero, then we will align to the sector size. 
	**														This value must be a multiple of the pixel size or bad things will happen.
	**	@param	inSectorSize								The size of a sector on disk. The required amount of memory will be padded
	**														up to a multiple of the sector size. If this is zero, then there will be no padding.
	**	@param	inOffsetToPixelDataFromFirstSectorStart		The number of bytes between the start of the first sector and the start
	**														of the pixel data. The resulting pixel start location must be aligned to
	**														the pixel size.
	*/
	prSuiteError (*CreateDiskAlignedPPix)(
		PPixHand*			outPPixHand,
		PrPixelFormat		inPixelFormat,
		int					inWidth,
		int					inHeight,
		int					inPARNumerator,
		int					inPARDenominator,
		int					inMemoryAlignment,
		int					inSectorSize,
		int					inOffsetToPixelDataFromFirstSectorStart);
	
	// ------------------------------------- color managed versions ----------------------------------
	
	/**
	 **	This will create a new ppix.
	 **
	 **	@param	outPPixHand						The new ppix handle if the create was successfull. NULL otherwise.
	 **	@param	inRequestedAccess				Requested pixel access. ReadOnly is not allowed (doesn't make sense).
	 **	@param	inPixelFormat					The pixel format of this ppix.
	 **	@param	inWidth							The horizontal size of the ppix.
	 **	@param	inHeight						The vertical size of the ppix.
	 **	@param	inUseFields						If true, the ppix is a field.
	 **	@param	inFieldNumber					If inUseFields is true, this specifies first field (0) or second (1)
	 **	@param	inPARNumerator					The pixel aspect ratio numerator for the ppix.
	 **	@param	inPARDenominator				The pixel aspect ratio denominator for the ppix.
	 **	@param 	inColorSpaceID					opaque color space identifier; new video frame will be tagged with this color space
	 */
	prSuiteError (*CreateColorManagedPPix)(
		PPixHand*			outPPixHand,
		PrPPixBufferAccess	inRequestedAccess,
		PrPixelFormat		inPixelFormat,
		int					inWidth,
		int					inHeight,
		bool					inUseFields,
		int					inFieldNumber,
		int					inPARNumerator,
		int					inPARDenominator,
		PrSDKColorSpaceID	inColorSpaceID);
	
	
	/**
	 **	This will create a new ppix in a custom pixel format.
	 **
	 **	@param	outPPixHand						The new ppix handle if the create was successfull. NULL otherwise.
	 **	@param	inRequestedAccess				Requested pixel access. ReadOnly is not allowed (doesn't make sense).
	 **	@param	inPixelFormat					The pixel format of this ppix.
	 **	@param	inWidth							The horizontal size of the ppix.
	 **	@param	inHeight						The vertical size of the ppix.
	 **	@param	inPARNumerator					The pixel aspect ratio numerator for the ppix.
	 **	@param	inPARDenominator				The pixel aspect ratio denominator for the ppix.
	 **	@param	inDataBufferSize				The number of bytes requested, not including the header.
	 **	@param 	inColorSpaceID					opaque color space identifier; new video frame will be tagged with this color space
	 */
	prSuiteError (*CreateColorManagedCustomPPix)(
		PPixHand*			outPPixHand,
		PrPPixBufferAccess	inRequestedAccess,
		PrPixelFormat		inPixelFormat,
		int					inWidth,
		int					inHeight,
		int					inPARNumerator,
		int					inPARDenominator,
		int					inDataBufferSize,
		PrSDKColorSpaceID	inColorSpaceID);
	
	/**
	 **	This will create a new ppix properly aligned for reading from disk.
	 **
	 **	@param	outPPixHand									The new ppix handle if the create was successfull. NULL otherwise.
	 **	@param	inPixelFormat								The pixel format of this ppix.
	 **	@param	inWidth										The horizontal size of the ppix.
	 **	@param	inHeight									The vertical size of the ppix.
	 **	@param	inPARNumerator								The pixel aspect ratio numerator for the ppix.
	 **	@param	inPARDenominator							The pixel aspect ratio denominator for the ppix.
	 **	@param	inMemoryAlignment							The alignment of memory required for the start of the first sector.
	 **														If this value is zero, then we will align to the sector size.
	 **														This value must be a multiple of the pixel size or bad things will happen.
	 **	@param	inSectorSize								The size of a sector on disk. The required amount of memory will be padded
	 **														up to a multiple of the sector size. If this is zero, then there will be no padding.
	 **	@param	inOffsetToPixelDataFromFirstSectorStart		The number of bytes between the start of the first sector and the start
	 **														of the pixel data. The resulting pixel start location must be aligned to
	 **														the pixel size.
	 **	@param 	inColorSpaceID								opaque color space identifier; new video frame will be tagged with this color space
	 */
	prSuiteError (*CreateColorManagedDiskAlignedPPix)(
		PPixHand*			outPPixHand,
		PrPixelFormat		inPixelFormat,
		int					inWidth,
		int					inHeight,
		int					inPARNumerator,
		int					inPARDenominator,
		int					inMemoryAlignment,
		int					inSectorSize,
		int					inOffsetToPixelDataFromFirstSectorStart,
		PrSDKColorSpaceID	inColorSpaceID);

} PrSDKPPixCreator2Suite;

#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif
