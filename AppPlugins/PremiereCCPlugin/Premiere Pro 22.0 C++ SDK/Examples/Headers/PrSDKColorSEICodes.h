/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2018 Adobe Systems Incorporated                       */
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

#ifndef PRSDKCOLORSEICODES_H
#define PRSDKCOLORSEICODES_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#pragma pack(push, 1)

// video color space encoding
struct prSEIColorCodesRec
{
	// first 3 are based on SEI codes from Rec. H-265
	csSDK_int32	colorPrimariesCode;
	csSDK_int32	transferCharacteristicCode;
	csSDK_int32	matrixEquationsCode;
	csSDK_int32	bitDepth;
	prBool		isFullRange;	// full/narrow range
	prBool		isRGB;			// RGB/YUV
	prBool		isSceneReferred;
	// default init to Rec. 709
	prSEIColorCodesRec() :
		colorPrimariesCode(1),			// 709 color primaries
		transferCharacteristicCode(1),	// 709 transfer curve
		matrixEquationsCode(1),			// 709 matrix coefficients
		bitDepth(8),
		isFullRange(false),
		isRGB(false),
		isSceneReferred(false)	// default is display-referred
	{}
	prSEIColorCodesRec(
		csSDK_int32 inColorPrimariesCode,
		csSDK_int32 inTransferCharacteristicCode,
		csSDK_int32 inMatrixEquationsCode,
		csSDK_int32 inBitDepth,
		prBool		inIsFullRange,
		prBool		inIsRGB,
		prBool		inIsSceneReferred) :
			colorPrimariesCode(inColorPrimariesCode),
			matrixEquationsCode(inMatrixEquationsCode),
			transferCharacteristicCode(inTransferCharacteristicCode),
			bitDepth(inBitDepth),
			isFullRange(inIsFullRange),
			isRGB(inIsRGB),
			isSceneReferred(inIsSceneReferred)
	{}
};

#pragma pack(pop)

#endif	// PRSDKCOLORSEICODES_H
