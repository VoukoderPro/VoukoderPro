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


#ifndef PRSDKQUALITY_H
#define PRSDKQUALITY_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

/**
**	Render qualities
*/
typedef enum 
{
	kPrRenderQuality_Max = 4,
	kPrRenderQuality_High = 3,
	kPrRenderQuality_Medium = 2,
	kPrRenderQuality_Low = 1,
	kPrRenderQuality_Draft = 0,
	kPrRenderQuality_Invalid = 0xFFFFFFFF,
	kPrRenderQuality_ForceEnumSize = kPrForceEnumToIntValue
} PrRenderQuality;

/**
** Playback qualities
*/
typedef enum {
	kPrPlaybackQuality_Invalid = 4,
	kPrPlaybackQuality_High = 3,			
	kPrPlaybackQuality_Draft = 2,		
	kPrPlaybackQuality_Auto = 1,
	kPrPlaybackQuality_ForceEnumSize = kPrForceEnumToIntValue
} PrPlaybackQuality;

/**
** Playback Fractional Resolutions
*/
typedef enum {
	kPrPlaybackFractionalResolution_Invalid = 6,
	kPrPlaybackFractionalResolution_Sixteenth = 5,			
	kPrPlaybackFractionalResolution_Eighth = 4,		
	kPrPlaybackFractionalResolution_Quarter = 3,		
	kPrPlaybackFractionalResolution_Half = 2,
	kPrPlaybackFractionalResolution_Full = 1,
	kPrPlaybackFractionalResolution_ForceEnumSize = kPrForceEnumToIntValue
} PrPlaybackFractionalResolution;

#endif