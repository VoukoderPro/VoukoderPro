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


#ifndef	PRSDKAUDIOSUITE_H
#define PRSDKAUDIOSUITE_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKMALERRORS_H
#include "PrSDKMALErrors.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define kPrSDKAudioSuite				"Premiere Audio Suite"
#define kPrSDKAudioSuiteVersion			1

/**
**	Audio overview:
**
**	All audio calls to and from Premiere use arrays of buffers of 32-bit
**	floats to pass audio. Therefore all audio is not interleaved by channel.
**	So stereo audio looks like this:
**
**	float* audio[2];
**
**	where audio[0] is the address of a buffer N samples long and audio[1]
**	is the address of a second buffer N samples long. audio[0] contains
**	the left channel and audio[1] contains the right channel. N is called
**	the number of "sample frames" in the buffer. N will be the same no matter
**	how many channels of audio are present in the buffer.
**
**	Because we use 32-bit floats for each audio sample, we can represent
**	values above 0 dB during mixing. 0 dB corresponds to +/- 1.0 in the floating
**	point. For symmetry in the int <--> float conversions, it is recommended
**	you use the utility functions below.  For example if you want to convert
**	a single float to a 16-bit signed int, use ConvertAndInterleaveTo16BitInteger()
**	with 1 channel and one sample frame.
**
**	Since floats are the only audio format ever passed, there is no variable
**	sample type or bit depth. However, when going back and forth to storage
**	formats, it is necessary to give the user an indication of the sample
**	type in the storage. For this reason, the SDK includes AudioSampleType
**	which defines a variety of possible formats. Note that these are ONLY
**	USED FOR USER INTERFACE!
**
**	In general, any audio sample rate is supported. For this reason, we use
**	a floating point number to represent audio sample rates. However, not all
**	rates will evenly divide into our time ticks per second, so for more
**	unusual rates there will be quantization errors possible using PrTime.
**
**	The host currently supports 4 audio channels types: mono, stereo, 5.1 and 32 channels.
**	Stereo channel ordering is {kPrAudioChannelLabel_FrontLeft, kPrAudioChannelLabel_FrontRight}.
**	5.1 channels ordering is
**		{kPrAudioChannelLabel_FrontLeft,		// L
**		 kPrAudioChannelLabel_FrontRight,		// R
**		 kPrAudioChannelLabel_FrontCenter,		// C
**		 kPrAudioChannelLabel_LowFrequency,		// LFE
**		 kPrAudioChannelLabel_BackLeft,			// Ls
**		 kPrAudioChannelLabel_BackRight}.		// Rs

**	32 channel ordering is all {kPrAudioChannelLabel_Discrete}
*/

enum PrAudioSampleType
{
	kPrAudioSampleType_8BitInt = 1,
	kPrAudioSampleType_8BitTwosInt,			//	8 bit, twos complement
	kPrAudioSampleType_16BitInt,		
	kPrAudioSampleType_24BitInt,		
	kPrAudioSampleType_32BitInt,		
	kPrAudioSampleType_32BitFloat,
	kPrAudioSampleType_64BitFloat,
	kPrAudioSampleType_16BitIntBigEndian,
	kPrAudioSampleType_24BitIntBigEndian,
	kPrAudioSampleType_32BitIntBigEndian,
	kPrAudioSampleType_32BitFloatBigEndian,
	kPrAudioSampleType_Compressed,			//	any non-PCM format
	kPrAudioSampleType_Packed,				//	any PCM format with mixed sample types
	kPrAudioSampleType_Other,				//	A sample type not in this list
	kPrAudioSampleType_Any,					//	Any available sample type (used by Compilers)
	kPrAudioSampleType_AudioPeakData,
	kPrAudioSampleType_Max = 0xFFFFFFFF	
};

enum PrAudioChannelType
{
	kPrAudioChannelType_Mono = 1,
	kPrAudioChannelType_Stereo = 2,
	kPrAudioChannelType_51 = 3,
	kPrAudioChannelType_MaxChannel = 4,
	kPrAudioChannelType_Max = 0xFFFFFFFF
};
#define kMaxAudioChannelCount 32

/**
**	This is not the sample type of the underlying audio - this is the sample type
**	that is returned in the calls that support variable audio types. If the type is not
**	explicitly called out as variable, then it is Float32.
*/
enum PrAudioDataType
{
	kPrAudioDataType_Float32 = 1,
	kPrAudioDataType_Int32 = 2,
	kPrAudioDataType_PeakAudio = 3
};

typedef prInt64 PrAudioSample;

enum PrAudioChannelLabel
{
	// Normally, a channel label corresponds (roughly) to a speaker position.
	// Other more generic channel labels may be defined in the future.
	kPrAudioChannelLabel_Discrete				= 0,	// No particular speaker: (e.g. the channel is a discrete mono channel)

	// Adobe											   WAVEFORMATEXTENSIBLE				Apple CoreAudio								VST 3.6
	// ----------------------------------------------	   -----------------------------	---------------------------------------		------------------
	kPrAudioChannelLabel_FrontLeft				= 100,	// SPEAKER_FRONT_LEFT				kAudioChannelLabel_Left						kSpeakerL
	kPrAudioChannelLabel_FrontRight				= 101,	// SPEAKER_FRONT_RIGHT				kAudioChannelLabel_Right					kSpeakerR
	kPrAudioChannelLabel_FrontCenter			= 102,	// SPEAKER_FRONT_CENTER				kAudioChannelLabel_Center					kSpeakerC
	kPrAudioChannelLabel_LowFrequency			= 103,	// SPEAKER_LOW_FREQUENCY			kAudioChannelLabel_LFEScreen				kSpeakerLfe
	kPrAudioChannelLabel_BackLeft				= 104,	// SPEAKER_BACK_LEFT				kAudioChannelLabel_LeftSurround				kSpeakerLs
	kPrAudioChannelLabel_BackRight				= 105,	// SPEAKER_BACK_RIGHT				kAudioChannelLabel_RightSurround			kSpeakerRs		
	kPrAudioChannelLabel_BackCenter				= 106,	// SPEAKER_BACK_CENTER				kAudioChannelLabel_CenterSurround			kSpeakerCs = kSpeakerS
	kPrAudioChannelLabel_FrontLeftOfCenter		= 107,	// SPEAKER_FRONT_LEFT_OF_CENTER		kAudioChannelLabel_LeftCenter				kSpeakerLc
	kPrAudioChannelLabel_FrontRightOfCenter		= 108,	// SPEAKER_FRONT_RIGHT_OF_CENTER	kAudioChannelLabel_RightCenter				kSpeakerRc
	kPrAudioChannelLabel_SideLeft				= 109,	// SPEAKER_SIDE_LEFT				kAudioChannelLabel_LeftSurroundDirect		kSpeakerSl
	kPrAudioChannelLabel_SideRight				= 110,	// SPEAKER_SIDE_RIGHT				kAudioChannelLabel_RightSurroundDirect		kSpeakerSr
	kPrAudioChannelLabel_TopCenter				= 111,	// SPEAKER_TOP_CENTER				kAudioChannelLabel_TopCenterSurround		kSpeakerTc
	kPrAudioChannelLabel_TopFrontLeft			= 112,	// SPEAKER_TOP_FRONT_LEFT			kAudioChannelLabel_VerticalHeightLeft		kSpeakerTfl
	kPrAudioChannelLabel_TopFrontRight			= 113,	// SPEAKER_TOP_FRONT_RIGHT			kAudioChannelLabel_VerticalHeightRight		kSpeakerTfr
	kPrAudioChannelLabel_TopFrontCenter			= 114,	// SPEAKER_TOP_FRONT_CENTER			kAudioChannelLabel_VerticalHeightCenter		kSpeakerTfc
	kPrAudioChannelLabel_TopBackLeft			= 115,	// SPEAKER_TOP_BACK_LEFT			kAudioChannelLabel_TopBackLeft				kSpeakerTrl
	kPrAudioChannelLabel_TopBackRight			= 116,	// SPEAKER_TOP_BACK_RIGHT			kAudioChannelLabel_TopBackRight				kSpeakerTrr
	kPrAudioChannelLabel_TopBackCenter			= 117,	// SPEAKER_TOP_BACK_CENTER			kAudioChannelLabel_TopBackCenter			kSpeakerTrc
	kPrAudioChannelLabel_FrontLeftWide			= 118,	//									kAudioChannelLabel_LeftWide 
	kPrAudioChannelLabel_FrontRightWide			= 119,	//									kAudioChannelLabel_RightWide

	kPrAudioChannelLabel_LeftHeight				= 127,	//																				kSpeakerTsl
	kPrAudioChannelLabel_RightHeight			= 128,	//																				kSpeakerTsr
	kPrAudioChannelLabel_RearSurroundLeft 		= 129,	//									kAudioChannelLabel_RearSurroundLeft			kSpeakerLcs
	kPrAudioChannelLabel_RearSurroundRight		= 130, 	//									kAudioChannelLabel_RearSurroundRight		kSpeakerRcs
	kPrAudioChannelLabel_LFE2					= 131,	//									kAudioChannelLabel_LFE2						kSpeakerLfe2
	kPrAudioChannelLabel_LeftTotal				= 132,	//									kAudioChannelLabel_LeftTotal 
	kPrAudioChannelLabel_RightTotal				= 133,	//									kAudioChannelLabel_RightTotal 
	kPrAudioChannelLabel_Ambisonic_W			= 134,	//									kAudioChannelLabel_Ambisonic_W				kSpeakerW
	kPrAudioChannelLabel_Ambisonic_X			= 135,	//									kAudioChannelLabel_Ambisonic_X				kSpeakerX
	kPrAudioChannelLabel_Ambisonic_Y			= 136,	//									kAudioChannelLabel_Ambisonic_Y				kSpeakerY
	kPrAudioChannelLabel_Ambisonic_Z			= 137,	//									kAudioChannelLabel_Ambisonic_Z				kSpeakerZ
	kPrAudioChannelLabel_BottomFrontLeft		= 138,	//																				kSpeakerBfl
	kPrAudioChannelLabel_BottomFrontCenter		= 139,	//																				kSpeakerBfc
	kPrAudioChannelLabel_BottomFrontRight		= 140,	//																				kSpeakerBfr
	kPrAudioChannelLabel_ProximityLeft			= 141,	//																				kSpeakerPl
	kPrAudioChannelLabel_ProximityRight			= 142,	//																				kSpeakerPr
};

typedef struct 
{
	/**
	**	These are a variety of audio buffer conversion routines to convert an array
	**	of floating point buffers into interleaved audio of a specific type.
	**
	**	@param	inSource		An array of float buffers holding audio.
	**	@param	inDestination	A single buffer to hold the converted, interleaved audio.
	**							inDestination must be at least inNumChannels * sizeof(destinationFormat) *
	**							inNumSampleFrames bytes long.
	**	@param	inNumChannels	The number of channels in inSource.
	**	@param	inNumSampleFrames	The number of samples in each buffer in inSource.
	*/
	prSuiteError (*ConvertAndInterleaveTo8BitInteger)(
		float**				inSource,
		char*				inDestination,
		unsigned int		inNumChannels,
		unsigned int		inNumSampleFrames);

	prSuiteError (*ConvertAndInterleaveTo16BitInteger)(
		float**				inSource,
		short*				inDestination,
		unsigned int		inNumChannels,
		unsigned int		inNumSampleFrames);

	prSuiteError (*ConvertAndInterleaveTo24BitInteger)(
		float**				inSource,
		char*				inDestination,
		unsigned int		inNumChannels,
		unsigned int		inNumSampleFrames);

	prSuiteError (*ConvertAndInterleaveTo32BitInteger)(
		float**				inSource,
		csSDK_int32*		inDestination,
		unsigned int		inNumChannels,
		unsigned int		inNumSampleFrames);
	
	/**
	**	These routines provide the reverse of the routines above, converting and
	**	uninterleaving a single buffer in a specified type.
	**
	**	@param	inSource		A single buffer holding the orignal, interleaved audio.
	**	@param	inDestination	An array of float buffers holding audio. There must be inNumChannels
	**							buffers allocated in the array, and each must be at least
	**							inNumSampleFrames * sizeof(float) bytes long.
	**	@param	inNumChannels	The number of channels in inSource.
	**	@param	inNumSampleFrames	The number of samples in each buffer in inSource.
	*/
	prSuiteError (*UninterleaveAndConvertFrom8BitInteger)(
		char*				inSource,
		float**				inDestination,
		unsigned int		inNumChannels,
		unsigned int		inNumSampleFrames);

	prSuiteError (*UninterleaveAndConvertFrom16BitInteger)(
		short*				inSource,
		float**				inDestination,
		unsigned int		inNumChannels,
		unsigned int		inNumSampleFrames);

	prSuiteError (*UninterleaveAndConvertFrom24BitInteger)(
		char*				inSource,
		float**				inDestination,
		unsigned int		inNumChannels,
		unsigned int		inNumSampleFrames);

	prSuiteError (*UninterleaveAndConvertFrom32BitInteger)(
		csSDK_int32*		inSource,
		float**				inDestination,
		unsigned int		inNumChannels,
		unsigned int		inNumSampleFrames);


} PrSDKAudioSuite;

#ifdef __cplusplus
}
#endif

#endif /* PRSDKAUDIOSUITE_H */

