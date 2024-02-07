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

#ifndef ADOBESDK_CONTROLSURFACE_CONTROLSURFACEHOSTSUITE_H
#define ADOBESDK_CONTROLSURFACE_CONTROLSURFACEHOSTSUITE_H

// adobesdk
#include <adobesdk/config/PreConfig.h>
#include <adobesdk/controlsurface/ControlSurfaceTypes.h>

// SweetPea2
#include <SPTypes.h>

#define kADOBESDK_ControlSurfaceHostSuite					"ADOBESDK ControlSurfaceHost Suite"
#define kADOBESDK_ControlSurfaceHostSuite_Version1			1
#define kADOBESDK_ControlSurfaceHostSuite_Version2			2

#define kADOBESDK_ControlSurfaceHostSuite_VersionCurrent	kADOBESDK_ControlSurfaceHostSuite_Version2
#define	kADOBESDK_ControlSurfaceHostSuiteCurrent			ADOBESDK_ControlSurfaceHostSuite2

typedef struct
{
	// Returns the command handler on the host side in outCommandRef
	SPErr (*GetCommandRef)(ADOBESDK_ControlSurfaceHostRef inHostRef, ADOBESDK_ControlSurfaceHostCommandRef* outCommandRef);

	// Returns the marker handler on the host side in outMarkerRef
	SPErr (*GetMarkerRef)(ADOBESDK_ControlSurfaceHostRef inHostRef, ADOBESDK_ControlSurfaceHostMarkerRef* outMarkerRef);

	// Returns the mixer handler on the host side in outMixerRef
	SPErr (*GetMixerRef)(ADOBESDK_ControlSurfaceHostRef inHostRef, ADOBESDK_ControlSurfaceHostMixerRef* outMixerRef);

	// Returns the transport handler on the host side in outTransportRef
	SPErr (*GetTransportRef)(ADOBESDK_ControlSurfaceHostRef inHostRef, ADOBESDK_ControlSurfaceHostTransportRef* outTransportRef);

	// New in version 2
	// Returns the lumetri handler on the host side in outLumetriRef
	SPErr (*GetLumetriRef)(ADOBESDK_ControlSurfaceHostRef inHostRef, ADOBESDK_ControlSurfaceHostLumetriRef* outLumetriRef);

} ADOBESDK_ControlSurfaceHostSuite2;

typedef ADOBESDK_ControlSurfaceHostSuite2 ADOBESDK_ControlSurfaceHostSuite1;

#include <adobesdk/config/PostConfig.h>

#endif // ADOBESDK_CONTROLSURFACE_CONTROLSURFACEHOSTSUITE_H
