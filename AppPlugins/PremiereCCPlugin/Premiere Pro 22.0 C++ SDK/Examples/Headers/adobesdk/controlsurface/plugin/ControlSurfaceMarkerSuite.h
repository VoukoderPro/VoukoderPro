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

#ifndef ADOBE_SDK_CONTROLSURFACE_CONTROLSURFACEMARKERSUITE_H
#define ADOBE_SDK_CONTROLSURFACE_CONTROLSURFACEMARKERSUITE_H

// adobesdk
#include <adobesdk/config/PreConfig.h>
#include <adobesdk/controlsurface/ControlSurfaceTypes.h>

// SweetPea2
#include <SPTypes.h>

#define kADOBESDK_ControlSurfaceMarkerSuite					"ADOBESDK ControlSurface Marker Suite"
#define kADOBESDK_ControlSurfaceMarkerSuite_Version1			1

#define kADOBESDK_ControlSurfaceMarkerSuite_VersionCurrent	kADOBESDK_ControlSurfaceMarkerSuite_Version1
#define	kADOBESDK_ControlSurfaceMarkerSuiteCurrent			ADOBESDK_ControlSurfaceMarkerSuite1

typedef struct
{
	SPErr (*MarkerChanged)(ADOBESDK_ControlSurfaceMarkerRef inControlSurfaceRef, uint32_t inMarkerIndex);
	SPErr (*RebuildMarkers)(ADOBESDK_ControlSurfaceMarkerRef inControlSurfaceRef);

} ADOBESDK_ControlSurfaceMarkerSuite1;


#include <adobesdk/config/PostConfig.h>

#endif // ADOBE_SDK_CONTROLSURFACE_CONTROLSURFACEMARKERSUITE_H
