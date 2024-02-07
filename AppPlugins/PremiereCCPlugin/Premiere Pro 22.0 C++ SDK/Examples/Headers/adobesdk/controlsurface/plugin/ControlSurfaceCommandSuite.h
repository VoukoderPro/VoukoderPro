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
#ifndef ADOBESDK_CONTROLSURFACE_CONTROLSURFACECOMMANDSUITE_H
#define ADOBESDK_CONTROLSURFACE_CONTROLSURFACECOMMANDSUITE_H

// adobesdk
#include <adobesdk/config/PreConfig.h>
#include <adobesdk/controlsurface/ControlSurfaceTypes.h>

// SweetPea2
#include <SPTypes.h>

#define kADOBESDK_ControlSurfaceCommandSuite					"ADOBESDK ControlSurface Command Suite"
#define kADOBESDK_ControlSurfaceCommandSuite_Version1			1

#define kADOBESDK_ControlSurfaceCommandSuite_VersionCurrent		kADOBESDK_ControlSurfaceCommandSuite_Version1
#define	kADOBESDK_ControlSurfaceCommandSuiteCurrent				ADOBESDK_ControlSurfaceCommandSuite1

typedef struct
{

	// Returns the number of user assignable buttons.
	SPErr (*GetUserButtonCount)(
		ADOBESDK_ControlSurfaceCommandRef	inControlSurfaceRef,
		uint32_t*							outButtonCount);

	// Returns the ID for the button at index <inIndex>.
	SPErr (*GetUserButtonIDAtIndex)(
		ADOBESDK_ControlSurfaceCommandRef	inControlSurfaceRef,
		uint32_t							inIndex,
		ADOBESDK_String*					outButtonID);

	// Retrieves the display name of a command button. This must be unique ).
	// The returned string has to be allocated using ADOBESDK_ControlSurfaceHostMemorySuite1::AllocateString and freed using ADOBESDK_ControlSurfaceHostMemorySuite1::FreeString
	SPErr (*GetUserButtonDisplayName)(
		ADOBESDK_ControlSurfaceCommandRef	inControlSurfaceRef,
		const ADOBESDK_String*				inButtonID,
		ADOBESDK_String*					outDisplayName);

	// Assigns a command ID to the specified button
	SPErr (*SetUserButtonCommand)(
		ADOBESDK_ControlSurfaceCommandRef	inControlSurfaceRef,
		const ADOBESDK_String*				inButtonID,
		const ADOBESDK_String*				inContextID,
		const ADOBESDK_String*				inCommandID);

	// Retrieves the assigned command ID from a button. 
	// The returned string has to be allocated using ADOBESDK_ControlSurfaceHostMemorySuite1::AllocateString and freed using ADOBESDK_ControlSurfaceHostMemorySuite1::FreeString
	SPErr (*GetUserButtonCommand)(
		ADOBESDK_ControlSurfaceCommandRef	inControlSurfaceRef,
		const ADOBESDK_String*				inButtonID,
		ADOBESDK_String*					outContextID,			///< May be empty or NULL, indicating that there is no context ID assigne to the specified button.
		ADOBESDK_String*					outCommandID);			///< May be empty or NULL, indicating that there is no command ID assigne to the specified button.

} ADOBESDK_ControlSurfaceCommandSuite1;

#include <adobesdk/config/PostConfig.h>

#endif //ADOBESDK_CONTROLSURFACE_CONTROLSURFACECOMMANDSUITE_H
