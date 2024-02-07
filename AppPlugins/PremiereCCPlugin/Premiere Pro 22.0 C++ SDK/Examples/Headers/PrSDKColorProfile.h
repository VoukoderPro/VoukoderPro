/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2017 Adobe Systems Incorporated                       */
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


#ifndef PRSDKCOLORPROFILE_H
#define PRSDKCOLORPROFILE_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKCOLORSEICODES_H
#include "PrSDKColorSEICodes.h"
#endif

enum
{
	kPrSDKColorSpaceType_Undefined,
	kPrSDKColorSpaceType_ICC,
	kPrSDKColorSpaceType_LUT,  // DO NOT USE, deprecated and not supported anymore
	kPrSDKColorSpaceType_SEITags,
	kPrSDKColorSpaceType_MXFTags,
	kPrSDKColorSpaceType_Predefined	// see PrSDKColorSpaces.h for predefined color spaces
};
typedef csSDK_int32 PrSDKColorSpaceType;

typedef struct
{
	void*			inPrivateData;
	csSDK_int32		ioBufferSize;
	void*			inDestinationBuffer;
	PrSDKString		outName;
} ColorProfileRec;

typedef struct
{
	csSDK_int32		ioBufferSize;
	void*			inDestinationBuffer;
	PrSDKString		outName;
} RawColorProfileRec;

// only one of the components is expected to be filled
// based on outColorSpaceType
typedef struct
{
	void*					inPrivateData;
	PrSDKColorSpaceType		outColorSpaceType;
	RawColorProfileRec		ioProfileRec;	// for ICC, LUTs and Predefined Color Spaces
	prSEIColorCodesRec		outSEICodesRec;	// H-265 codes for HEVC, AVC, ProRes
} ColorSpaceRec;

typedef struct
{
	PrSDKColorSpaceType		colorSpaceType;
	RawColorProfileRec		profileRec;	    // for ICC, LUTs and Predefined Color Spaces
	prSEIColorCodesRec		seiCodesRec;	// H-265 codes for HEVC, AVC, ProRes
} RawColorSpaceRec;

// Embedded LUT profile
typedef struct
{
	void*					privateData;
	RawColorProfileRec 		lutBlobRec;
	RawColorSpaceRec 		lutInputColorSpaceRec;
	RawColorSpaceRec 		lutOutputColorSpaceRec;
} EmbeddedLUTRec;

#endif // PRSDKCOLORPROFILE_H
