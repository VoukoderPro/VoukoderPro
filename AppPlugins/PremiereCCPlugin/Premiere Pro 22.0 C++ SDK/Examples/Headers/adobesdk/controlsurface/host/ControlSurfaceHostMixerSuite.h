/**************************************************************************
*
* ADOBE CONFIDENTIAL
* ___________________
*
*  Copyright 2011 Adobe Systems Incorporated
*  All Rights Reserved.
*
* NOTICE:  All information contained herein is, and remains the property of
* Adobe Systems Incorporated  and its suppliers,  if any.  The intellectual
* and technical concepts contained herein are proprietary to  Adobe Systems
* Incorporated  and its suppliers  and may be  covered by U.S.  and Foreign
* Patents,patents in process,and are protected by trade secret or copyright
* law.  Dissemination of this  information or reproduction of this material
* is strictly  forbidden  unless prior written permission is  obtained from
* Adobe Systems Incorporated.
**************************************************************************/

#ifndef ADOBESDK_CONTROLSURFACE_CONTROLSURFACEHOSTMIXERSUITE_H
#define ADOBESDK_CONTROLSURFACE_CONTROLSURFACEHOSTMIXERSUITE_H

// adobesdk
#include <adobesdk/config/PreConfig.h>
#include <adobesdk/controlsurface/ControlSurfaceTypes.h>

// SweetPea2
#include <SPTypes.h>

#define kADOBESDK_ControlSurfaceHostMixerSuite					"ADOBESDK ControlSurfaceHost Mixer Suite"
#define kADOBESDK_ControlSurfaceHostMixerSuite_Version1			1

#define kADOBESDK_ControlSurfaceHostMixerSuite_VersionCurrent	kADOBESDK_ControlSurfaceHostMixerSuite_Version1
#define	kADOBESDK_ControlSurfaceHostMixerSuiteCurrent			ADOBESDK_ControlSurfaceHostMixerSuite1

typedef struct
{
	// Sets the clip mixer mode. If true, the host uses the control surface
	// to control the clip mixer. If false, the track mixer is controlled.
	// Only supported by Premiere Pro.
	SPErr (*SetClipMixerMode)(
		ADOBESDK_ControlSurfaceHostMixerRef	inControlSurfaceHostMixerRef,
		ADOBESDK_Boolean inClipMixerMode);

	// Returns the current clip mixer mode (see SetClipMixerMode).
	// Only supported by Premiere Pro.
	SPErr (*GetClipMixerMode)(
		ADOBESDK_ControlSurfaceHostMixerRef	inControlSurfaceHostMixerRef,
		ADOBESDK_Boolean* outClipMixerMode);

	// Returns the number of channel strips excluding the master channel
	SPErr (*GetChannelCount)(
		ADOBESDK_ControlSurfaceHostMixerRef	inControlSurfaceHostMixerRef,
		uint32_t*							outChannelCount);

	// Channels are accessed by ID rather than the index. GetChannelForIndex() returns the ID for a given channel index.
	SPErr (*GetChannelIDForIndex)(
		ADOBESDK_ControlSurfaceHostMixerRef	inControlSurfaceHostMixerRef,
		uint32_t inChannelIndex,
		ADOBESDK_ControlSurfaceChannelID* outChannelID);

	// Channels are accessed by ID rather than the index, but sometimes the index is needed e.g. for displaying the track number.
	// Can sometimes be an expensive operation.
	SPErr (*GetChannelIndexForID)(
		ADOBESDK_ControlSurfaceHostMixerRef	inControlSurfaceHostMixerRef,
		ADOBESDK_ControlSurfaceChannelID inChannelID,
		uint32_t* outChannelIndex);

	// Returns the ID for the master channel, or 
	SPErr (*GetMasterChannelID)(
		ADOBESDK_ControlSurfaceHostMixerRef	inControlSurfaceHostMixerRef,
		ADOBESDK_ControlSurfaceChannelID* outMasterChannelID);

	// Sets a channel's state to te specified value (like mute/solo/record).
	// See kADOBESDK_ControlSurfaceChannelStateIndex_* for supported states.
	SPErr (*SetChannelState)(
		ADOBESDK_ControlSurfaceHostMixerRef	inControlSurfaceHostMixerRef,
		ADOBESDK_ControlSurfaceChannelID inChannelID,
		uint32_t inChannelStateIndex,	///< kADOBESDK_ControlSurfaceChannelStateIndex_*
		ADOBESDK_Boolean inState);

	// Returns a channel's state (like mute/solo/record).
	// See kADOBESDK_ControlSurfaceChannelStateIndex_* for supported states.
	SPErr (*GetChannelState)(
		ADOBESDK_ControlSurfaceHostMixerRef	inControlSurfaceHostMixerRef,
		ADOBESDK_ControlSurfaceChannelID inChannelID,
		uint32_t inChannelStateIndex,	///< kADOBESDK_ControlSurfaceChannelStateIndex_*
		ADOBESDK_Boolean* outState);

	// Returns a channel's flags.
	// See kADOBESDK_ControlSurfaceChannelFlag_* for supported flags.
	SPErr (*GetChannelFlags)(
		ADOBESDK_ControlSurfaceHostMixerRef	inControlSurfaceHostMixerRef,
		ADOBESDK_ControlSurfaceChannelID inChannelID,
		uint32_t* outFlags);

	// Selects a channel. This will deselect all other channels.
	SPErr (*SetSelectedChannelID)(
		ADOBESDK_ControlSurfaceHostMixerRef	inControlSurfaceHostMixerRef,
		ADOBESDK_ControlSurfaceChannelID inChannelID);

	// Returns the selected channel
	SPErr (*GetSelectedChannelID)(
		ADOBESDK_ControlSurfaceHostMixerRef	inControlSurfaceHostMixerRef,
		ADOBESDK_ControlSurfaceChannelID* outSelectedChannel);

	// Selects the next channel. This will deselect all other channels.
	SPErr (*SelectNextChannel)(
		ADOBESDK_ControlSurfaceHostMixerRef	inControlSurfaceHostMixerRef);

	// Selects the previous channel. This will deselect all other channels.
	SPErr (*SelectPreviousChannel)(
		ADOBESDK_ControlSurfaceHostMixerRef	inControlSurfaceHostMixerRef);

	// Adds a channel offset for the control surface to set its start channel
	SPErr (*AddChannelOffset)(
		ADOBESDK_ControlSurfaceHostMixerRef	inControlSurfaceHostMixerRef,
		int32_t								inChannelOffset);	///< inChannelOffset may be negative. Used for bank & channel buttons.

	// Sets the fader value for a channel.
	SPErr (*SetChannelFaderValue)(
		ADOBESDK_ControlSurfaceHostMixerRef	inControlSurfaceHostMixerRef,
		ADOBESDK_ControlSurfaceChannelID inChannelID,
		float inVolumeInDB);

	// Returns the fader value for a channel.
	SPErr (*GetChannelFaderValue)(
		ADOBESDK_ControlSurfaceHostMixerRef	inControlSurfaceHostMixerRef,
		ADOBESDK_ControlSurfaceChannelID inChannelID,
		float* outVolumeInDB);

	// Returns the number of audio channels for the channel strip (e.g. 6 for 5.1)
	SPErr (*GetChannelNumAudioChannels)(
		ADOBESDK_ControlSurfaceHostMixerRef	inControlSurfaceHostMixerRef,
		ADOBESDK_ControlSurfaceChannelID inChannelID,
		uint32_t* outNumAudioChannels);

	// Returns the current meter values for the channel. The <outValues> buffer must be large enough
	// to receive <inNumAudioChannels> values.
	SPErr (*GetChannelMeterValues)(
		ADOBESDK_ControlSurfaceHostMixerRef	inControlSurfaceHostMixerRef,
		ADOBESDK_ControlSurfaceChannelID inChannelID,
		uint32_t inNumAudioChannels,
		float* outValues);

	// Returns the channel pan type.
	// May be unsupported by master channel
	SPErr (*GetChannelPanType)(
		ADOBESDK_ControlSurfaceHostMixerRef	inControlSurfaceHostMixerRef,
		ADOBESDK_ControlSurfaceChannelID inChannelID,
		uint32_t* outPanType);	// See kADOBESDK_ControlSurfacePanType_*

	SPErr (*GetChannelName)(
		ADOBESDK_ControlSurfaceHostMixerRef	inControlSurfaceHostMixerRef,
		ADOBESDK_ControlSurfaceChannelID inChannelID,
		uint32_t inMaxLength,
		ADOBESDK_String* outChannelName);

	// See kADOBESDK_ControlSurfaceComponentIndex_* for possible index values.

	// Returns if the component is currently available (e.g. insert or send effect slot in use)
	SPErr (*HasComponent)(
		ADOBESDK_ControlSurfaceHostMixerRef	inControlSurfaceHostMixerRef,
		ADOBESDK_ControlSurfaceChannelID inChannelID,
		uint32_t inComponentIndex,
		ADOBESDK_Boolean* outHasComponent);

	// Returns the name of a component
	SPErr (*GetComponentName)(
		ADOBESDK_ControlSurfaceHostMixerRef	inControlSurfaceHostMixerRef,
		ADOBESDK_ControlSurfaceChannelID inChannelID,
		uint32_t inComponentIndex,
		uint32_t inMaxLength,
		ADOBESDK_String* outName);

	// Sets the bypass state of a component
	SPErr (*SetComponentBypass)(
		ADOBESDK_ControlSurfaceHostMixerRef	inControlSurfaceHostMixerRef,
		ADOBESDK_ControlSurfaceChannelID inChannelID,
		uint32_t inComponentIndex,
		ADOBESDK_Boolean inBypass);

	// Returns if the component is bypassed
	SPErr (*GetComponentBypass)(
		ADOBESDK_ControlSurfaceHostMixerRef	inControlSurfaceHostMixerRef,
		ADOBESDK_ControlSurfaceChannelID inChannelID,
		uint32_t inComponentIndex,
		ADOBESDK_Boolean* outBypass);

	// Returns the number of parameters for a given component index.
	SPErr (*GetComponentParameterCount)(
		ADOBESDK_ControlSurfaceHostMixerRef	inControlSurfaceHostMixerRef,
		ADOBESDK_ControlSurfaceChannelID inChannelID,
		uint32_t inComponentIndex,
		uint32_t* outParameterCount);

	// Returns the name of a component's parameter
	SPErr (*GetComponentParameterName)(
		ADOBESDK_ControlSurfaceHostMixerRef	inControlSurfaceHostMixerRef,
		ADOBESDK_ControlSurfaceChannelID inChannelID,
		uint32_t inComponentIndex,
		uint32_t inParameterIndex,
		uint32_t inMaxLength,
		ADOBESDK_String* outParameterName);

	SPErr (*GetComponentParameterDisplay)(
		ADOBESDK_ControlSurfaceHostMixerRef	inControlSurfaceHostMixerRef,
		ADOBESDK_ControlSurfaceChannelID inChannelID,
		uint32_t inComponentIndex,
		uint32_t inParameterIndex,
		uint32_t inMaxLength,
		ADOBESDK_String* outParameterDisplay);

	SPErr (*SetComponentParameterValue)(
		ADOBESDK_ControlSurfaceHostMixerRef	inControlSurfaceHostMixerRef,
		ADOBESDK_ControlSurfaceChannelID inChannelID,
		uint32_t inComponentIndex,
		uint32_t inParameterIndex,
		float inValue); // Normalized 0..1

	SPErr (*GetComponentParameterValue)(
		ADOBESDK_ControlSurfaceHostMixerRef	inControlSurfaceHostMixerRef,
		ADOBESDK_ControlSurfaceChannelID inChannelID,
		uint32_t inComponentIndex,
		uint32_t inParameterIndex,
		float* outParameterValue);
		
	SPErr (*ComponentParameterTouched)(
		ADOBESDK_ControlSurfaceHostMixerRef	inControlSurfaceHostMixerRef,
		ADOBESDK_ControlSurfaceChannelID inChannelID,
		uint32_t inComponentIndex,
		uint32_t inParameterIndex,
		ADOBESDK_Boolean inDownState);
		

	// Enables or disables being notified for parameter value changes of the specified component
	SPErr (*ListenToComponentParameterChanges)(
		ADOBESDK_ControlSurfaceHostMixerRef	inControlSurfaceHostMixerRef,
		ADOBESDK_ControlSurfaceChannelID inChannelID,
		uint32_t inComponentIndex,
		ADOBESDK_Boolean inListen);	// Set to true if changes to parameter values should be sent

	// Changes the component parameter value according to <inAmount>.
	// This value represents a percental change of a control like a rotary knob.
	SPErr (*SpinComponentParameterValue)(
		ADOBESDK_ControlSurfaceHostMixerRef	inControlSurfaceHostMixerRef,
		ADOBESDK_ControlSurfaceChannelID inChannelID,
		uint32_t inComponentIndex,
		uint32_t inParameterIndex,
		float inAmount);

	// Brings up the component UI, like tre channel's EQ
	SPErr (*ShowComponentUI)(
		ADOBESDK_ControlSurfaceHostMixerRef	inControlSurfaceHostMixerRef,
		ADOBESDK_ControlSurfaceChannelID inChannelID,
		uint32_t inComponentIndex,
		ADOBESDK_Boolean inShow);

	// Returns if the component UI is currently visible
	SPErr (*IsComponentUIShown)(
		ADOBESDK_ControlSurfaceHostMixerRef	inControlSurfaceHostMixerRef,
		ADOBESDK_ControlSurfaceChannelID inChannelID,
		uint32_t inComponentIndex,
		ADOBESDK_Boolean* outIsShown);

	// Sets the automation mode. for possible values, see kADOBESDK_ControlSurfaceAutomationMode_*
	SPErr (*SetAutomationMode)(
		ADOBESDK_ControlSurfaceHostMixerRef	inControlSurfaceHostMixerRef,
		ADOBESDK_ControlSurfaceChannelID inChannelID,
		ADOBESDK_ControlSurfaceAutomationMode inAutomationMode);

	// Returns the automation mode for a given channel.
	SPErr (*GetAutomationMode)(
		ADOBESDK_ControlSurfaceHostMixerRef	inControlSurfaceHostMixerRef,
		ADOBESDK_ControlSurfaceChannelID inChannelID,
		ADOBESDK_ControlSurfaceAutomationMode* outAutomationMode);


	// Switches the track UI between IO/FX/SENDS/EQ (if applicable, may be unsupported)
	SPErr (*SetTrackControlsMode)(
		ADOBESDK_ControlSurfaceHostMixerRef	inControlSurfaceHostMixerRef,
		uint32_t inTrackControlsMode);	///< kADOBESDK_ControlSurfaceTrackControlsMode_*

	// Returns the current track controls mode
	SPErr (*GetTrackControlsMode)(
		ADOBESDK_ControlSurfaceHostMixerRef	inControlSurfaceHostMixerRef,
		uint32_t* outTrackControlsMode);	///< kADOBESDK_ControlSurfaceTrackControlsMode_*

} ADOBESDK_ControlSurfaceHostMixerSuite1;


#include <adobesdk/config/PostConfig.h>

#endif // ADOBESDK_CONTROLSURFACE_CONTROLSURFACEHOSTMIXERSUITE_H
