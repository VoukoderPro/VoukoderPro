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

#ifndef ADOBESDK_CONTROLSURFACE_CONTROLSURFACEHOSTTRANSPORTSUITE_H
#define ADOBESDK_CONTROLSURFACE_CONTROLSURFACEHOSTTRANSPORTSUITE_H

// adobesdk
#include <adobesdk/config/PreConfig.h>
#include <adobesdk/controlsurface/ControlSurfaceTypes.h>

// SweetPea2
#include <SPTypes.h>

#define kADOBESDK_ControlSurfaceHostTransportSuite					"ADOBESDK ControlSurfaceHost Transport Suite"
#define kADOBESDK_ControlSurfaceHostTransportSuite_Version1			1

#define kADOBESDK_ControlSurfaceHostTransportSuite_VersionCurrent	kADOBESDK_ControlSurfaceHostTransportSuite_Version1
#define	kADOBESDK_ControlSurfaceHostTransportSuiteCurrent			ADOBESDK_ControlSurfaceHostTransportSuite1

typedef struct
{
	// Starts rewinding (key is pressed)
	SPErr (*BeginRewind)(
		ADOBESDK_ControlSurfaceHostTransportRef inControlSurfaceHostTransportRef);
	
	// Stops rewinding (key is released)
	SPErr (*EndRewind)(
		ADOBESDK_ControlSurfaceHostTransportRef inControlSurfaceHostTransportRef);

	// Starts fast forward (key is pressed)
	SPErr (*BeginForward)(
		ADOBESDK_ControlSurfaceHostTransportRef inControlSurfaceHostTransportRef);

	// Stops fast forward (key is released)
	SPErr (*EndForward)(
		ADOBESDK_ControlSurfaceHostTransportRef inControlSurfaceHostTransportRef);

	// Stops the playback
	SPErr (*Stop)(
		ADOBESDK_ControlSurfaceHostTransportRef inControlSurfaceHostTransportRef);

	// Starts the playback
	SPErr (*Play)(
		ADOBESDK_ControlSurfaceHostTransportRef inControlSurfaceHostTransportRef);

	// Starts recording
	SPErr (*Record)(
		ADOBESDK_ControlSurfaceHostTransportRef inControlSurfaceHostTransportRef);

	// Pauses the playback
	SPErr (*Pause)(
		ADOBESDK_ControlSurfaceHostTransportRef inControlSurfaceHostTransportRef);

	// Toggles cycling of the playback
	SPErr (*ToggleCycle)(
		ADOBESDK_ControlSurfaceHostTransportRef inControlSurfaceHostTransportRef);

	// Jumps to the previous point of interest, like a marker
	SPErr (*GotoPrevious)(
		ADOBESDK_ControlSurfaceHostTransportRef inControlSurfaceHostTransportRef);

	// Jumps to the next point of interest, like a marker
	SPErr (*GotoNext)(
		ADOBESDK_ControlSurfaceHostTransportRef inControlSurfaceHostTransportRef);

	// Jumps to the start of the sequence
	SPErr (*GotoStart)(
		ADOBESDK_ControlSurfaceHostTransportRef inControlSurfaceHostTransportRef);

	// J
	SPErr (*ShuttleLeft)(
		ADOBESDK_ControlSurfaceHostTransportRef inControlSurfaceHostTransportRef);

	// K
	SPErr (*ShuttleStop)(
		ADOBESDK_ControlSurfaceHostTransportRef inControlSurfaceHostTransportRef);

	// L
	SPErr (*ShuttleRight)(
		ADOBESDK_ControlSurfaceHostTransportRef inControlSurfaceHostTransportRef);

	// Indicates that the jog dial has been turned
	SPErr (*JogDial)(
		ADOBESDK_ControlSurfaceHostTransportRef	inControlSurfaceHostTransportRef,
		float									inTicksMoved,			// Number of ticks moved - this can be negative
		float									inTicksPerRevolution,	// Number of ticks for a full rotation
		ADOBESDK_Boolean						inShuttle);				// If true, the host should shuttle, else it should move the CTI

	// Returns the current time as a string as well as the current display mode
	// See kADOBESDK_ControlSurfaceTimeDisplayMode_* for possible display modes
	SPErr (*GetTimeDisplayString)(
		ADOBESDK_ControlSurfaceHostTransportRef	inControlSurfaceHostTransportRef,
		uint32_t*								outTimeDisplayMode,
		uint32_t								inMaxLength,
		ADOBESDK_String*						outTimeDisplayString);

	// Returns true if the host is currently recording, i.e. if the record LED should be lit
	SPErr (*GetRecordState)(
		ADOBESDK_ControlSurfaceHostTransportRef	inControlSurfaceHostTransportRef,
		ADOBESDK_Boolean*						outState);

	// Returns true if the host is currently in stopped state, i.e. if the stop LED should be lit
	SPErr (*GetStopState)(
		ADOBESDK_ControlSurfaceHostTransportRef	inControlSurfaceHostTransportRef,
		ADOBESDK_Boolean*						outState);

	// Returns true if the host is currently in play state, i.e. if the play LED should be lit
	SPErr (*GetPlayState)(
		ADOBESDK_ControlSurfaceHostTransportRef	inControlSurfaceHostTransportRef,
		ADOBESDK_Boolean*						outState);

	// Returns true if the host has a soloed channel, i.e. if a rude solo LED should be lit / blinking
	SPErr (*GetSoloState)(
		ADOBESDK_ControlSurfaceHostTransportRef	inControlSurfaceHostTransportRef,
		ADOBESDK_Boolean*						outState);

} ADOBESDK_ControlSurfaceHostTransportSuite1;


#include <adobesdk/config/PostConfig.h>

#endif // ADOBESDK_CONTROLSURFACE_CONTROLSURFACEHOSTTRANSPORTSUITE_H
