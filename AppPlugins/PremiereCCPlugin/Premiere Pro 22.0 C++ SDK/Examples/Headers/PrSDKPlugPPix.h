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


#ifndef PRSDKPLUGPPIX_H
#define PRSDKPLUGPPIX_H

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

typedef char *(*plugppixGetPixelsFunc)(PPixHand pix);
typedef void (*plugppixGetBoundsFunc)(PPixHand pix, prRect *bounds);
typedef int (*plugppixGetRowbytesFunc)(PPixHand pix);
typedef PPixHand (*plugppixNewFunc)(prRect *bounds);
typedef void (*plugppixDisposeFunc)(PPixHand ppix);
typedef void (*plugppixLockFunc)(PPixHand pix);
typedef void (*plugppixUnlockFunc)(PPixHand pix);
typedef int (*plugppixGetPixelAspectRatioFunc)(PPixHand pix, csSDK_uint32 *num, csSDK_uint32 *den);
typedef void (*plugppixGetAlphaBounds)(PPixHand pix, prRect *alphaBounds);

typedef struct
{
	plugppixGetPixelsFunc				ppixGetPixels;
	plugppixGetBoundsFunc				ppixGetBounds;
	plugppixGetRowbytesFunc				ppixGetRowbytes;
	plugppixNewFunc						ppixNew;						/* PR_PISUITES_VERSION_2 */
	plugppixDisposeFunc					ppixDispose;					/* PR_PISUITES_VERSION_2 */
	plugppixLockFunc					ppixLockPixels;					/* PR_PISUITES_VERSION_2 */
	plugppixUnlockFunc					ppixUnlockPixels;				/* PR_PISUITES_VERSION_2 */
	plugppixGetPixelAspectRatioFunc		ppixGetPixelAspectRatio; 		/* PR_PISUITES_VERSION_3 */
	plugppixGetAlphaBounds				ppixGetAlphaBounds;				/* PR_PISUITES_VERSION_6 */
} PlugppixFuncs, *PlugppixFuncsPtr;

#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif /* PRSDKPLUGPPIX_H */
