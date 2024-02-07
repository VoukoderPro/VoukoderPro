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


#ifndef	PRSDKTIMESUITE_H
#define PRSDKTIMESUITE_H

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

#define kPrSDKTimeSuite				"Premiere Time Suite"
#define kPrSDKTimeSuiteVersion		1

/**
**	Time overview:
**
**	Premiere uses a tick-based time counter that is stored in a signed
**	64-bit integer. The current number of ticks per second must be retrieved
**	using the callback in this suite, but is guaranteed to be constant for
**	the duration of runtime.
*/

typedef prInt64 PrTime;

/**
**	RatioTime is used where value that are not representable by ticks are needed.
*/
struct PrRatioTime
{
	prInt64		mValue;
	prInt64		mScale;
};

// [TODO] [SFMOD] clean this up (PrFrameRate VS. PrVideoFrameRates).  Maybe extra helpers?
//#ifdef _MSC_VER
//	typedef double PrFrameRate;
//#else
//	typedef signed long long PrFrameRate;
//#endif


// [SFMOD] [TODO] this normally belongs in PrSDKPlayModule.h
//		but now it's also used by PrSDKRealTime.h" (TBD)
// timebase struct
// Example: scale = 2997, sampleSize = 100, fileDurations = numframes * 100
typedef struct {
	csSDK_uint32 scale;					// rate of the timebase
	csSDK_int32	sampleSize;				// size of one sample
	csSDK_int32	fileDuration;			// number of samples in file
} pmPlayTimebase;


enum PrVideoFrameRates
{
	kVideoFrameRate_24Drop = 1,			//	24000 / 1001
	kVideoFrameRate_24,					//	24
	kVideoFrameRate_PAL,				//	25
	kVideoFrameRate_NTSC,				//	30000 / 1001
	kVideoFrameRate_30,					//	30
	kVideoFrameRate_PAL_HD,				//	50
	kVideoFrameRate_NTSC_HD,			//	60000 / 1001
	kVideoFrameRate_60,					//	60
	kVideoFrameRate_Max = 0xFFFFFFFF
};

typedef struct 
{
	/**
	**	Get the current ticks per second. This is guaranteed to be constant for
	**	the duration of runtime.
	**
	**	@param	outTickPerSec	on return, the number of time ticks per second.
	*/
	prSuiteError (*GetTicksPerSecond)(
		PrTime*					outTicksPerSec);
	
	/**
	**	Get the number of ticks in a video frame rate.
	**
	**	@param	inFrameRate		an enum value for a video frame rate.
	**	@param	outTicksPerFrame	on return, the number of time ticks per frame.
	*/
	prSuiteError (*GetTicksPerVideoFrame)(
		PrVideoFrameRates		inVideoFrameRate,
		PrTime*					outTicksPerFrame);

	/**
	**	Get the number of ticks in an audio sample rate.
	**
	**	@param	inSampleRate	the audio sample rate as a float.
	**	@param	outTicksPerSample	on return, the number of time ticks per sample.
	**	@return					kPrTimeSuite_RoundedAudioRate is returned if the requested
	**							audio sample rate is not an even divisor of the base tick
	**							count and therefore times in this rate will not be exact.
	*/
	prSuiteError (*GetTicksPerAudioSample)(
		float					inSampleRate,
		PrTime*					outTicksPerSample);

} PrSDKTimeSuite;

#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif /* PRSDKTIMESUITE_H */

