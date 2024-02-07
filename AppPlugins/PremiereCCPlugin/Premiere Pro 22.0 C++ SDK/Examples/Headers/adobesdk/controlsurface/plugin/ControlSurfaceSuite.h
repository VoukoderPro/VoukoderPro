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

#ifndef ADOBE_SDK_CONTROLSURFACE_CONTROLSURFACESUITE_H
#define ADOBE_SDK_CONTROLSURFACE_CONTROLSURFACESUITE_H

// adobesdk
#include <adobesdk/config/PreConfig.h>
#include <adobesdk/controlsurface/ControlSurfaceTypes.h>

// SweetPea2
#include <SPTypes.h>

#define kADOBESDK_ControlSurfaceSuite					"ADOBESDK ControlSurface Suite"
#define kADOBESDK_ControlSurfaceSuite_Version1			1
#define kADOBESDK_ControlSurfaceSuite_Version2			2

#define kADOBESDK_ControlSurfaceSuite_VersionCurrent	kADOBESDK_ControlSurfaceSuite_Version2
#define	kADOBESDK_ControlSurfaceSuiteCurrent			ADOBESDK_ControlSurfaceSuite2

enum
{
	// If set, the control surface is capable of displaying Unicode.
	// If not set, the control surface will display ASCII only.
	kADOBESDK_ControlSurfaceFlag_CanDisplayUnicode = 1
};

typedef struct
{
	SPErr (*GetConfigIdentifier)(
		ADOBESDK_ControlSurfaceRef	inControlSurfaceRef,
		ADOBESDK_String*			outCongigIdentifier);

	SPErr (*GetControlSurfaceFlags)(
		ADOBESDK_ControlSurfaceRef	inControlSurfaceRef,
		uint32_t*					outControlSurfaceFlags);

	SPErr (*GetTransportRef)(
		ADOBESDK_ControlSurfaceRef				inControlSurfaceRef,
		ADOBESDK_ControlSurfaceTransportRef*	outTransportRef);

	SPErr (*GetMixerRef)(
		ADOBESDK_ControlSurfaceRef			inControlSurfaceRef,
		ADOBESDK_ControlSurfaceMixerRef*	outMixerRef);

	SPErr (*GetMarkerRef)(
		ADOBESDK_ControlSurfaceRef			inControlSurfaceRef,
		ADOBESDK_ControlSurfaceMarkerRef*	outMarkerRef);

	SPErr (*GetCommandRef)(
		ADOBESDK_ControlSurfaceRef			inControlSurfaceRef,
		ADOBESDK_ControlSurfaceCommandRef*	outCommandRef);
	
	// Called to let the control surface check for changes and update itself if needed.
	SPErr (*Update)(ADOBESDK_ControlSurfaceRef inControlSurfaceRef);

	// New in version 2
	SPErr (*GetLumetriRef)(
		ADOBESDK_ControlSurfaceRef			inControlSurfaceRef,
		ADOBESDK_ControlSurfaceLumetriRef*	outLumetriRef);

} ADOBESDK_ControlSurfaceSuite2;

typedef ADOBESDK_ControlSurfaceSuite2 ADOBESDK_ControlSurfaceSuite1;

#include <adobesdk/config/PostConfig.h>

#endif // ADOBE_SDK_CONTROLSURFACE_CONTROLSURFACESUITE_H
