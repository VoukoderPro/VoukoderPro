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

#ifndef ADOBE_SDK_CONTROLSURFACE_CONTROLSURFACEMIXERSUITE_H
#define ADOBE_SDK_CONTROLSURFACE_CONTROLSURFACEMIXERSUITE_H

// adobesdk
#include <adobesdk/config/PreConfig.h>
#include <adobesdk/controlsurface/ControlSurfaceTypes.h>

// SweetPea2
#include <SPTypes.h>

#define kADOBESDK_ControlSurfaceMixerSuite					"ADOBESDK ControlSurface Mixer Suite"
#define kADOBESDK_ControlSurfaceMixerSuite_Version1			1
#define kADOBESDK_ControlSurfaceMixerSuite_Version2			2

#define kADOBESDK_ControlSurfaceMixerSuite_VersionCurrent	kADOBESDK_ControlSurfaceMixerSuite_Version2
#define	kADOBESDK_ControlSurfaceMixerSuiteCurrent			ADOBESDK_ControlSurfaceMixerSuite2

enum
{
	kADOBESDK_ControlSurfaceChannelConfig_Name = 0,
	kADOBESDK_ControlSurfaceChannelConfig_InsertRack = 1,
	kADOBESDK_ControlSurfaceChannelConfig_SendRack = 2,
	kADOBESDK_ControlSurfaceChannelConfig_TrackInput = 3,
	kADOBESDK_ControlSurfaceChannelConfig_TrackOutput = 4
};

static const uint32_t kADOBESDK_ControlSurfaceChannelChannelCount_Indeterminate = 0xffffffffUL;
static const uint32_t kADOBESDK_ControlSurfaceChannelChannelCount_None = 0UL;

typedef struct
{
	// Returns the number of channel strips supported by this surface.
	// May return kADOBESDK_ControlSurfaceChannelChannelCount_Indeterminate, which indicates
	// the number of channel strips is nopt exposed and managed by the plug-in.
	SPErr (*GetMaxChannelStripCount)(
		ADOBESDK_ControlSurfaceMixerRef	inControlSurfaceRef,
		uint32_t* outMaxChannelStripCount);

	// Called when a channel's configuration or name changed. See kADOBESDK_ControlSurfaceChannelConfig_* for possible values
	SPErr (*ChannelConfigChanged)(
		ADOBESDK_ControlSurfaceMixerRef	inControlSurfaceRef,
		ADOBESDK_ControlSurfaceChannelID inChannelID,
		uint32_t inSelector);

	// Called when the selection changed
	SPErr (*SelectionChanged)(
		ADOBESDK_ControlSurfaceMixerRef	inControlSurfaceRef);

	// Sets a channel's state to te specified value (like mute/solo/record).
	// See kADOBESDK_ControlSurfaceChannelStateIndex_* for supported states.
	SPErr (*SetChannelState)(
		ADOBESDK_ControlSurfaceMixerRef	inControlSurfaceRef,
		ADOBESDK_ControlSurfaceChannelID inChannelID,
		uint32_t inChannelStateIndex,
		ADOBESDK_Boolean inState);

	// Sets the automation mode of the specified channel
	SPErr (*SetAutomationMode)(
		ADOBESDK_ControlSurfaceMixerRef	inControlSurfaceRef,
		ADOBESDK_ControlSurfaceChannelID inChannelID,
		ADOBESDK_ControlSurfaceAutomationMode inAutomationMode);

	// Sets the channel offset of the mixer. A value of 1 would result in the assignment of the second
	// application channel to the first device channel.
	SPErr (*SetChannelOffset)(
		ADOBESDK_ControlSurfaceMixerRef	inControlSurfaceRef,
		uint32_t inChannelOffset);

	// Notifies the plug-in of the total number of application channels
	SPErr (*SetChannelCount)(
		ADOBESDK_ControlSurfaceMixerRef	inControlSurfaceRef,
		uint32_t inChannelCount);

	// Sets the rude solo state. This is used to indicate that at least one channel has been soloed.
	SPErr (*SetRudeSoloState)(
		ADOBESDK_ControlSurfaceMixerRef	inControlSurfaceRef,
		ADOBESDK_Boolean inValue);

	// Called when the component's bypass state changed.
	SPErr (*SetComponentBypass)(
		ADOBESDK_ControlSurfaceMixerRef	inControlSurfaceRef,
		ADOBESDK_ControlSurfaceChannelID inChannelID,
		uint32_t inComponentIndex,
		ADOBESDK_Boolean inBypass);

	// Sets the component parameter value from the effect to the control surface.
	SPErr (*SetComponentParameter)(
		ADOBESDK_ControlSurfaceMixerRef	inControlSurfaceRef,
		ADOBESDK_ControlSurfaceChannelID inChannelID,
		uint32_t inComponentIndex,
		uint32_t inParameterIndex,
		float inValue);

	// Added in version 2
	// Returns true if the control surface does not have a master master fader and the host should include
	// it in the set of regular faders.
	SPErr (*AddMasterFaderAsRegularChannel)(
		ADOBESDK_ControlSurfaceMixerRef	inControlSurfaceRef,
		ADOBESDK_Boolean* outValue);

} ADOBESDK_ControlSurfaceMixerSuite2;

typedef ADOBESDK_ControlSurfaceMixerSuite2 ADOBESDK_ControlSurfaceMixerSuite1;

#include <adobesdk/config/PostConfig.h>

#endif // ADOBE_SDK_CONTROLSURFACE_CONTROLSURFACEMIXERSUITE_H
