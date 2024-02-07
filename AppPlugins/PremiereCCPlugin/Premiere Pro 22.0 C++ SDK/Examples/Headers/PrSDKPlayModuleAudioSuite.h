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


#ifndef	PRSDKPLAYMODULEAUDIOSUITE_H
#define PRSDKPLAYMODULEAUDIOSUITE_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKTIMESUITE_H
#include "PrSDKTimeSuite.h"
#endif

#ifndef	PRSDKAUDIOSUITE_H
#include "PrSDKAudioSuite.h"
#endif

#ifndef PRSDKMALERRORS_H
#include "PrSDKMALErrors.h"
#endif

#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif

#define kPrSDKPlayModuleAudioSuite				"Premiere Playmod Audio Suite"
#define kPrSDKPlayModuleAudioSuiteVersion		4

/**
**	Audio playback overview:
**
**	A playmod has two choices for playing audio: it can ask the host (Premiere) to play
**	the audio through the audio device selected by the user, or it can handle its own
**	playback of audio. A single playmod could use both states for different play events.
**
**	For host-based audio, the following sequence of calls should happen:
**
**	- Plugin receives a preroll selector.
**	- Plugin calls initHostAudio.
**	- Plugin returns from preroll selector.
**	- Plugin receives a start playback selector.
**	- Plugin calls startAudio.
**	- Plugin plays.
**	- Plugin receives stop playback selector.
**	- Plugin calls stopAudio.
**
**	During playback, the plugin can get the current play position from the host using
**	getCurrentTime, which will be clocked from the audio device.
**
**	For plugin-based audio, the following sequence should happen.
**
**	- Plugin receives a preroll selector.
**	- Plugin calls initPluginAudio, passing a AudioPlaybackSettings struct holding
**		the current audio playback settings.
**	- Plugin returns from preroll selector.
**	- Plugin receives a start playback selector.
**	- Plugin calls startAudio.
**	- Plugin plays. Whenever the plugin needs another buffer of audio, it calls getNextAudioBuffer.
**		Note that these buffers are always sequential.
**	- Plugin receives stop playback selector.
**	- Plugin calls stopAudio.
**
**	Note that the initAudio and startAudio calls can fail by returning an error code.
**	If this happens, then the player must abort preroll or playback start.
**
**	Premiere will return a negative countdown during its own audio preroll, which will
**	last at least as long as the requested minimum preroll time in the StartAudio call.
*/

typedef struct AudioPlaybackSettings
{
	PrAudioChannelType	outputChannelType;		//	The output channel type (see enum above)
	float				sampleRate;				//	The sample rate the playmod would like the audio at
	csSDK_uint32		maxBufferSize;			//	The largest buffer (in sample frames) that the
												//	playmod will ever ask for.
	PrTime				inputLatency;			//	The input latency of audio provided.
	PrTime				outputLatency;			//	The time between the request for audio
												//	and is transmission.
} AudioPlaybackSettings;

typedef struct AudioPlaybackSettings2
{
	float				sampleRate;				//	The sample rate the playmod would like the audio at
	csSDK_uint32		maxBufferSize;			//	The largest buffer (in sample frames) that the
												//	playmod will ever ask for.
	PrTime				inputLatency;			//	The input latency of audio provided.
	PrTime				outputLatency;			//	The time between the request for audio
												//	and is transmission.
	csSDK_uint32		outNumInputChannels;	//	The number of input channels activated. Filled in on return.
	csSDK_uint32		outNumOutputChannels;	//	The number of output channels activated. Filled in on return.
} AudioPlaybackSettings2;

typedef struct AudioPlaybackSettings3
{
	csSDK_uint32		numInputChannels;		//	The number of input channels supported by the playmod
	csSDK_uint32		numOutputChannels;		//	The number of output channels supported by the playmod
	float				sampleRate;				//	The sample rate the playmod would like the audio at
	csSDK_uint32		maxBufferSize;			//	The largest buffer (in sample frames) that the
												//	playmod will ever ask for.
	PrTime				inputLatency;			//	The input latency of audio provided.
	PrTime				outputLatency;			//	The time between the request for audio
												//	and is transmission.
	PrTime				delayCompensation;		//  How early to deliver audio
} AudioPlaybackSettings3;

/**
**	This callback definition is for the host to tell the plugin what the current audio
**	time is while it is playing the audio. It will be called on a very high priority
**	thread and it is important that the plugin not block this thread or perform any
**	lengthy processing during this call.
**
**	@param	inCurrentTime		The current time in PrTime ticks.
*/
typedef void (*AudioTimeCallback)(void* inInstanceData, PrTime inCurrentTime);

typedef struct AudioPositions
{
	PrTime				inPosition;				//	The in point of the playback;
												//	Must be less than the playback start and out points. 
	PrTime				outPosition;			//	The out point of playback;
												//	Must be greater than the playback in and start points. 
	PrTime				currentPosition;		//	the current playback time. 
												//	Must be between the in and out points.
	prBool				looping;				//	If TRUE, then playback should loop at the boundaries.
} AudioPositions;

typedef struct 
{
	/**
	**	Tell the host to initialize the audio hardware for the host to play the
	**	audio. GetNextAudioBuffer will be unavailable using this initialization.
	**
	**	@param	inTimerCallback	This is an optional parameter. If NULL, then the plugin will
	**							not be called by the audio hardware thread. If non-NULL, it must
	**							point to a function which can handle being called on a high-priority
	**							thread at a regular interval.
	**	@param	inInstanceData	If inTimerCallback is non-NULL, then the plugin can pass a pointer
	**							to instance data that it wishes returned when the timer callback is
	**							called.
	**	@param	inIsScrubbing	If non-zero, Premiere will treat this as a scrubbing init.
	**	@param	outClockInterval	If inTimerCallback is non-NULL and this parameter is non-NULL,
	**							then on return it will contain the expected calling interval of
	**							inTimerCallback. This should be used as a guide only, and may
	**							vary depending on the audio hardware.
	**	@return					kPrAudioResult_Success or kPrAudioResult_InitializationFailure.
	*/
	prSuiteError (*InitHostAudio)(
		csSDK_int32					inPlayID,
		AudioTimeCallback			inTimerCallback,
		void*						inInstanceData,
		int							inIsScrubbing,
		PrTime*						outClockInterval);
	
	/**
	**	Tell the host to initialize the audio render chain for the plugin to
	**	play the audio. GetNextAudioBuffer will be available using this initialization.
	**
	**	@param	inSettings		the settings for the requested playback.
	**	@param	inIsScrubbing	If non-zero, Premiere will treat this as a scrubbing init.
	**	@return					kPrAudioResult_Success or kPrAudioResult_InitializationFailure.
	*/
	prSuiteError (*InitPluginAudio)(
		csSDK_int32						inPlayID,
		int								inIsScrubbing,
		const AudioPlaybackSettings*	inSettings);		//	The settings struct for the current playback.
	
	/**
	**	Tell the host to start the audio playback. If InitHostAudio() was used, then the
	**	playback clock will start running on another thread, possibly before this
	**	call returns. If InitPluginAudio() was used, then GetNextAudioBuffer() is now
	**	available.
	**
	**	@param	inPlayPosition	the playback position structure.
	**	@param	inPlaybackSpeed	the requested playback speed. Must be non-zero.
	**	@param	inMinimumPreroll	the minimum time that the player needs to preroll video. Premiere
	**							guarantees that the auido clock will start at least this long before
	**							the start point.
	**	@return					kPrAudioResult_Success, kPrAudioResult_NotInited, or kPrAudioResult_IllegalPlaySetting.
	*/
	prSuiteError (*StartAudio)(
		csSDK_int32				inPlayID,
		const AudioPositions*	inPlayPosition,
		float					inPlaybackSpeed,
		PrTime					inMinimumPreroll);
	
	/**
	**	Retrieve the next N samples of audio from the host. This call is only available if (
	**	InitPluginAudio(0 was used.
	**
	**	@param	inInBuffers		a pointer to an array of buffers holding inNumSampleFrames input audio
	**							in each buffer, corresponding to the total number of available
	**							input channels.
	**	@param	outOutBuffers	a pointer to an array of buffers inNumSampleFrames long into which
	**							the host will write the output audio. There must be N buffers,
	**							where N is the number of output channels for the output channel
	**							type specified in InitPluginAudio.
	**	@param	inNumSampleFrames	the size of each of the buffers in the array in both inInBuffers
	**								and outOutBuffers.
	**	@return					kPrAudioResult_Success, kPrAudioResult_NotInited, or kPrAudioResult_NotStarted.
	*/
	prSuiteError (*GetNextAudioBuffer)(
		csSDK_int32				inPlayID,
		float**					inInBuffers,
		float**					outOutBuffers,
		unsigned int			inNumSampleFrames);
	
	/**
	**	Change the current audio playback position. If it is outside the in or out points,
	**	then the appropriate action will be taken to set the position.
	**
	**	@param	inRequestedPosition		points to the requested playback position.
	**	@param	outActualPosition		on return, will contain the actual position after the change (can be NULL).
	**	@return							kPrAudioResult_Success, kPrAudioResult_NotInited, or kPrAudioResult_NotStarted.
	*/
	prSuiteError (*SetPosition)(
		csSDK_int32				inPlayID,
		const PrTime*			inRequestedPosition,
		PrTime*					outActualPosition);
	
	/**
	**	This function will return the time of the audio buffer currently being played by the audio hardware.
	**	It is only valid to call after InitHostAudio().
	**
	**	@param	outPosition		on return, the time of the audio buffer currently being played by the audio hardware.
	**	@return					kPrAudioResult_Success, kPrAudioResult_NotInited, or kPrAudioResult_NotStarted.
	*/
	prSuiteError (*GetPosition)(
		csSDK_int32				inPlayID,
		PrTime*					outPosition);
	
	/**
	**	Change the current audio in and out points, and the looping state. If the current position
	**	field contains a valid position (i.e. it is between the in and out points), then the playback
	**	position will be moved to this position. If it contains an illegal value, then the playback
	**	position will not be moved except as constrained by the changed in and out points.
	**
	**	So, to simply change the in and out points or looping state, here is an example of the
	**	AudioPositions struct on calling:
	**
	**	AudioPositions position(0, 1000000, -1, TRUE);
	**
	**	On return, the currentPosition value will be filled in with the current position.
	**
	**	If instead the following struct is used:
	**
	**	AudioPositions position(0, 1000000, 25000, TRUE);
	**
	**	then the current position will be moved to 25000.
	**
	**	@param	inRequestedPosition		The requested state for the in, out, and loop positions,
	**									and, if valid, current position.
	**	@param	outActualPosition		On return, this will be the actual position that will be used even if an invalid position
	**									was sent in. Can be NULL.
	**
	**	@return					kPrAudioResult_Success, kPrAudioResult_IllegalPlaySetting, kPrAudioResult_NotInited.
	*/
	prSuiteError (*SetRange)(
		csSDK_int32				inPlayID,
		const AudioPositions*	inRequestedPosition,
		AudioPositions*			outActualPosition);
	
	/**
	**	Set the audio playback speed. This value must be non-zero, but can be any positive
	**	or negative value. Once the magnitude reaches a certain point, Premiere will
	**	generate silence.
	**
	**	@param	inSpeed			the requested playback speed. Must be non-zero.
	**	@return					kPrAudioResult_Success, kPrAudioResult_IllegalPlaySetting, or
	**							kPrAudioResult_NotInited.
	*/
	prSuiteError (*SetPlaybackSpeed)(
		csSDK_int32				inPlayID,
		float					inSpeed);
	
	/**
	**	StopAudio will cease audio playback and deinitialize the audio system. After calling StopAudio,
	**	you must call one of the InitAudio routines before calling Start again.
	**
	**	@return					kPrAudioResult_Success, kPrAudioResult_NotInited, or kPrAudioResult_NotStarted.
	*/
	prSuiteError (*StopAudio)(
		csSDK_int32				inPlayID);

} PrSDKPlayModuleAudioSuiteVersion1;

typedef struct 
{
	/**
	**	Tell the host to initialize the audio hardware for the host to play the
	**	audio. GetNextAudioBuffer will be unavailable using this initialization.
	**
	**	@param	inTimerCallback	This is an optional parameter. If NULL, then the plugin will
	**							not be called by the audio hardware thread. If non-NULL, it must
	**							point to a function which can handle being called on a high-priority
	**							thread at a regular interval.
	**	@param	inInstanceData	If inTimerCallback is non-NULL, then the plugin can pass a pointer
	**							to instance data that it wishes returned when the timer callback is
	**							called.
	**	@param	inIsScrubbing	If non-zero, Premiere will treat this as a scrubbing init.
	**	@param	outClockInterval	If inTimerCallback is non-NULL and this parameter is non-NULL,
	**							then on return it will contain the expected calling interval of
	**							inTimerCallback. This should be used as a guide only, and may
	**							vary depending on the audio hardware.
	**	@return					kPrAudioResult_Success or kPrAudioResult_InitializationFailure.
	*/
	prSuiteError (*InitHostAudio)(
		csSDK_int32					inPlayID,
		AudioTimeCallback			inTimerCallback,
		void*						inInstanceData,
		int							inIsScrubbing,
		PrTime*						outClockInterval);
	
	/**
	**	Tell the host to initialize the audio render chain for the plugin to
	**	play the audio. GetNextAudioBuffer will be available using this initialization.
	**
	**	@param	inSettings		the settings for the requested playback.
	**	@param	inIsScrubbing	If non-zero, Premiere will treat this as a scrubbing init.
	**	@return					kPrAudioResult_Success or kPrAudioResult_InitializationFailure.
	*/
	prSuiteError (*InitPluginAudio)(
		csSDK_int32						inPlayID,
		int								inIsScrubbing,
		AudioPlaybackSettings2*			ioSettings);		//	The settings struct for the current playback.
	
	/**
	**	Tell the host to start the audio playback. If InitHostAudio() was used, then the
	**	playback clock will start running on another thread, possibly before this
	**	call returns. If InitPluginAudio() was used, then GetNextAudioBuffer() is now
	**	available.
	**
	**	@param	inPlayPosition	the playback position structure.
	**	@param	inPlaybackSpeed	the requested playback speed. Must be non-zero.
	**	@param	inMinimumPreroll	the minimum time that the player needs to preroll video. Premiere
	**							guarantees that the auido clock will start at least this long before
	**							the start point.
	**	@return					kPrAudioResult_Success, kPrAudioResult_NotInited, or kPrAudioResult_IllegalPlaySetting.
	*/
	prSuiteError (*StartAudio)(
		csSDK_int32				inPlayID,
		const AudioPositions*	inPlayPosition,
		float					inPlaybackSpeed,
		PrTime					inMinimumPreroll);
	
	/**
	**	Retrieve the next N samples of audio from the host. This call is only available if (
	**	InitPluginAudio(0 was used.
	**
	**	@param	inInBuffers		a pointer to an array of buffers holding inNumSampleFrames input audio
	**							in each buffer, corresponding to the total number of available
	**							input channels.
	**	@param	outOutBuffers	a pointer to an array of buffers inNumSampleFrames long into which
	**							the host will write the output audio. There must be N buffers,
	**							where N is the number of output channels for the output channel
	**							type specified in InitPluginAudio.
	**	@param	inNumSampleFrames	the size of each of the buffers in the array in both inInBuffers
	**								and outOutBuffers.
	**	@return					kPrAudioResult_Success, kPrAudioResult_NotInited, or kPrAudioResult_NotStarted.
	*/
	prSuiteError (*GetNextAudioBuffer)(
		csSDK_int32				inPlayID,
		float**					inInBuffers,
		float**					outOutBuffers,
		unsigned int			inNumSampleFrames);
	
	/**
	**	Change the current audio playback position. If it is outside the in or out points,
	**	then the appropriate action will be taken to set the position.
	**
	**	@param	inRequestedPosition		points to the requested playback position.
	**	@param	outActualPosition		on return, will contain the actual position after the change (can be NULL).
	**	@return							kPrAudioResult_Success, kPrAudioResult_NotInited, or kPrAudioResult_NotStarted.
	*/
	prSuiteError (*SetPosition)(
		csSDK_int32				inPlayID,
		const PrTime*			inRequestedPosition,
		PrTime*					outActualPosition);
	
	/**
	**	This function will return the time of the audio buffer currently being played by the audio hardware.
	**	It is only valid to call after InitHostAudio().
	**
	**	@param	outPosition		on return, the time of the audio buffer currently being played by the audio hardware.
	**	@return					kPrAudioResult_Success, kPrAudioResult_NotInited, or kPrAudioResult_NotStarted.
	*/
	prSuiteError (*GetPosition)(
		csSDK_int32				inPlayID,
		PrTime*					outPosition);
	
	/**
	**	Change the current audio in and out points, and the looping state. If the current position
	**	field contains a valid position (i.e. it is between the in and out points), then the playback
	**	position will be moved to this position. If it contains an illegal value, then the playback
	**	position will not be moved except as constrained by the changed in and out points.
	**
	**	So, to simply change the in and out points or looping state, here is an example of the
	**	AudioPositions struct on calling:
	**
	**	AudioPositions position(0, 1000000, -1, TRUE);
	**
	**	On return, the currentPosition value will be filled in with the current position.
	**
	**	If instead the following struct is used:
	**
	**	AudioPositions position(0, 1000000, 25000, TRUE);
	**
	**	then the current position will be moved to 25000.
	**
	**	@param	inRequestedPosition		The requested state for the in, out, and loop positions,
	**									and, if valid, current position.
	**	@param	outActualPosition		On return, this will be the actual position that will be used even if an invalid position
	**									was sent in. Can be NULL.
	**
	**	@return					kPrAudioResult_Success, kPrAudioResult_IllegalPlaySetting, kPrAudioResult_NotInited.
	*/
	prSuiteError (*SetRange)(
		csSDK_int32				inPlayID,
		const AudioPositions*	inRequestedPosition,
		AudioPositions*			outActualPosition);
	
	/**
	**	Set the audio playback speed. This value must be non-zero, but can be any positive
	**	or negative value. Once the magnitude reaches a certain point, Premiere will
	**	generate silence.
	**
	**	@param	inSpeed			the requested playback speed. Must be non-zero.
	**	@return					kPrAudioResult_Success, kPrAudioResult_IllegalPlaySetting, or
	**							kPrAudioResult_NotInited.
	*/
	prSuiteError (*SetPlaybackSpeed)(
		csSDK_int32				inPlayID,
		float					inSpeed);
	
	/**
	**	StopAudio will cease audio playback and deinitialize the audio system. After calling StopAudio,
	**	you must call one of the InitAudio routines before calling Start again.
	**
	**	@return					kPrAudioResult_Success, kPrAudioResult_NotInited, or kPrAudioResult_NotStarted.
	*/
	prSuiteError (*StopAudio)(
		csSDK_int32				inPlayID);

	/**
	**	Tell the host to initialize the audio render chain for the plugin to
	**	play the audio. GetNextAudioBuffer will be available using this initialization.
	**
	**	@param	inSettings		the settings for the requested playback.
	**	@param	inIsScrubbing	If non-zero, Premiere will treat this as a scrubbing init.
	**	@return					kPrAudioResult_Success or kPrAudioResult_InitializationFailure.
	*/
	prSuiteError (*InitPluginAudio3)(
		csSDK_int32						inPlayID,
		int								inIsScrubbing,
		const AudioPlaybackSettings3*	inSettings);		//	The settings struct for the current playback.

	/**
	**	Retrieve the next N samples of audio from the host. This call is only available if (
	**	InitPluginAudio(0 was used.
	**
	**	@param	inInBuffers		a pointer to an array of buffers holding inNumSampleFrames input audio
	**							in each buffer, corresponding to the total number of available
	**							input channels.
	**	@param	outOutBuffers	a pointer to an array of buffers inNumSampleFrames long into which
	**							the host will write the output audio. There must be N buffers,
	**							where N is the number of output channels for the output channel
	**							type specified in InitPluginAudio.
	**	@param	inNumSampleFrames	the size of each of the buffers in the array in both inInBuffers
	**								and outOutBuffers.
	**	@param	outBufferTime	is the time at the start of the buffer returned.
	**	@return					kPrAudioResult_Success, kPrAudioResult_NotInited, or kPrAudioResult_NotStarted.
	*/
	prSuiteError (*GetNextAudioBuffer2)(
		csSDK_int32				inPlayID,
		float**					inInBuffers,
		float**					outOutBuffers,
		unsigned int			inNumSampleFrames,
		PrTime*					outBufferTime);
} PrSDKPlayModuleAudioSuite;

#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif /* PRSDKPLAYMODULEAUDIOSUITE_H */

