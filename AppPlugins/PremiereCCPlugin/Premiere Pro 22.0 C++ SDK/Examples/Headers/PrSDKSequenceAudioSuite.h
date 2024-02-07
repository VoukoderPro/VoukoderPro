/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2007 Adobe Systems Incorporated                       */
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


#ifndef	PRSDKSEQUENCEAUDIOSUITE_H
#define PRSDKSEQUENCEAUDIOSUITE_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKMALERRORS_H
#include "PrSDKMALErrors.h"
#endif

#include "PrSDKAudioSuite.h"

#include "PrSDKTimeSuite.h"

#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif

#define kPrSDKSequenceAudioSuite				"MediaCore Sequence Audio Suite"
#define kPrSDKSequenceAudioSuiteVersion1		1	// started for CS4
#define kPrSDKSequenceAudioSuiteVersion2		2	// started for 9.0, MakeAudioRenderer with channel layout

#define kPrSDKSequenceAudioSuiteVersion			kPrSDKSequenceAudioSuiteVersion2


// return value from MakeAudioRenderer
enum 
{
	sequenceAudioSuite_ErrIncompatibleChannelType = 'chan',	// this also means incompatible channel layout
	sequenceAudioSuite_ErrUnknown = 255
};

typedef struct 
{
	//
	prSuiteError (*MakeAudioRenderer)(
		csSDK_uint32 inPluginID,
		PrTime inStartTime,
		PrAudioChannelType inChannelType,
		PrAudioSampleType inSampleType,
		float inSampleRate,
		csSDK_uint32 *outAudioRenderID);

	//
	prSuiteError (*ReleaseAudioRenderer)(
		csSDK_uint32 inPluginID,
		csSDK_uint32 inAudioRenderID);

	// Audio is returned as arrays of uninterleaved floats. InBuffer must point to n buffers of floats
	// inFrameCount long, where n is the number of channels. This always returns the next contiguous
	// chunk of audio sample frames.
	prSuiteError (*GetAudio)(
		csSDK_uint32 inAudioRenderID, 
		csSDK_uint32 inFrameCount, 
		float** inBuffer, 
		char inClipAudio);

	// This call will reset the position on the audio generation to time zero. 
	// This can be used for multi-pass encoding.
	prSuiteError (*ResetAudioToBeginning)(
		csSDK_uint32 inAudioRenderID);

	// GetMaxBlip returns the maximum size of a blip
	prSuiteError (*GetMaxBlip)(
		csSDK_uint32 inAudioRenderID, 
		PrTime inTicksPerFrame, 
		csSDK_int32 *maxBlipSize);

} PrSDKSequenceAudioSuite1;


// Deprecate the usage of PrAudioChannelType in favor of a Channel Label based solution for MakeAudioRenderer
typedef struct
{
	//
	prSuiteError (*MakeAudioRenderer)(
		csSDK_uint32 inPluginID,
		PrTime inStartTime,
		csSDK_uint32 inNumChannels,
		PrAudioChannelLabel* inChannelLabels,   // array with size of at least inNumChannels
		PrAudioSampleType inSampleType,
		float inSampleRate,
		csSDK_uint32 *outAudioRenderID);

	//
	prSuiteError (*ReleaseAudioRenderer)(
		csSDK_uint32 inPluginID,
		csSDK_uint32 inAudioRenderID);

	// Audio is returned as arrays of uninterleaved floats. InBuffer must point to n buffers of floats
	// inFrameCount long, where n is the number of channels. This always returns the next contiguous
	// chunk of audio sample frames.
	prSuiteError (*GetAudio)(
		csSDK_uint32 inAudioRenderID, 
		csSDK_uint32 inFrameCount, 
		float** inBuffer, 
		char inClipAudio);

	// This call will reset the position on the audio generation to time zero. 
	// This can be used for multi-pass encoding.
	prSuiteError (*ResetAudioToBeginning)(
		csSDK_uint32 inAudioRenderID);

	// GetMaxBlip returns the maximum size of a blip
	prSuiteError (*GetMaxBlip)(
		csSDK_uint32 inAudioRenderID, 
		PrTime inTicksPerFrame, 
		csSDK_int32 *maxBlipSize);

} PrSDKSequenceAudioSuite2;


typedef PrSDKSequenceAudioSuite2 PrSDKSequenceAudioSuite;



#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif

