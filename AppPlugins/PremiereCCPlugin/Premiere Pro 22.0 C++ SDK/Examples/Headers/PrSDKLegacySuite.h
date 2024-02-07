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


#ifndef	PRSDKLEGACYSUITE_H
#define PRSDKLEGACYSUITE_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKTIMESUITE_H
#include "PrSDKTimeSuite.h"
#endif

#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif

#define kPrSDKLegacySuite				"Premiere Legacy Suite"
#define kPrSDKLegacySuiteVersion		1
#define kPrSDKLegacySuiteVersion2		2

/**
**	Legacy overview:
**
**	The functions in this suite provide a bridge between old calls and
**	structures and their new counterparts.
*/

typedef struct 
{
	/**
	**	This function will convert a scale and sample size from a timebase into
	**	a ticks per frame. An error will return if the scale and sample rate
	**	do not match a valid video frame rate.
	**
	**	@param	inScale				the scale value of the timebase in question.
	**	@param	inSampleSize		the sample size of the timebase in question.
	**	@param	outTicksPerFrame	on return, the number of time ticks per second.
	**	@return						kLegacyResult_Success or kLegacyResult_InvalidVideoRate
	*/
	prSuiteError (*GetTicksPerTimebaseFrame)(
		csSDK_uint32			inScale,
		csSDK_uint32			inSampleSize,
		PrTime*					outTicksPerFrame);

} PrSDKLegacySuite;


typedef struct 
{
	/**
	**	This function will convert a scale and sample size from a timebase into
	**	a ticks per frame. An error will return if the scale and sample rate
	**	do not match a valid video frame rate.
	**
	**	@param	inScale				the scale value of the timebase in question.
	**	@param	inSampleSize		the sample size of the timebase in question.
	**	@param	outTicksPerFrame	on return, the number of time ticks per second.
	**	@return						kLegacyResult_Success or kLegacyResult_InvalidVideoRate
	*/
	prSuiteError (*GetTicksPerTimebaseFrame)(
		csSDK_uint32			inScale,
		csSDK_uint32			inSampleSize,
		PrTime*					outTicksPerFrame);

	prSuiteError (*GetTimebaseFrameFromTicks)(
		PrTime					inTicksPerFrame,
		csSDK_uint32*			outScale,
		csSDK_uint32*			outSampleSize);
	

} PrSDKLegacySuite2;

#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif

