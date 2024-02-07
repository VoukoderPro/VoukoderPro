/**************************************************************************
*
* ADOBE CONFIDENTIAL
* ___________________
*
*  Copyright 2016 Adobe Systems Incorporated
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

#ifndef ADOBE_SDK_CONTROLSURFACE_CONTROLSURFACELUMETRISUITE_H
#define ADOBE_SDK_CONTROLSURFACE_CONTROLSURFACELUMETRISUITE_H

// adobesdk
#include <adobesdk/config/PreConfig.h>
#include <adobesdk/controlsurface/ControlSurfaceTypes.h>

// SweetPea2
#include <SPTypes.h>

#define kADOBESDK_ControlSurfaceLumetriSuite					"ADOBESDK ControlSurface Lumetri Suite"
#define kADOBESDK_ControlSurfaceLumetriSuite_Version1			1

#define kADOBESDK_ControlSurfaceLumetriSuite_VersionCurrent		kADOBESDK_ControlSurfaceLumetriSuite_Version1
#define	kADOBESDK_ControlSurfaceLumetriSuiteCurrent				ADOBESDK_ControlSurfaceLumetriSuite1

typedef struct
{
	SPErr (*ModeChanged)(
		ADOBESDK_ControlSurfaceLumetriRef	inControlSurfaceLumetriRef,
		uint32_t							inMode);

	SPErr (*ParameterValueChanged)(
		ADOBESDK_ControlSurfaceLumetriRef	inControlSurfaceLumetriRef,
		const ADOBESDK_String*				inParameterID,
		float								inParameterValue);

	SPErr (*MenuValueChanged)(
		ADOBESDK_ControlSurfaceLumetriRef	inControlSurfaceLumetriRef,
		const ADOBESDK_String*				inMenuID,
		const ADOBESDK_String*				inDisplayString);

	SPErr (*AllChanged)(
		ADOBESDK_ControlSurfaceLumetriRef	inControlSurfaceLumetriRef);

} ADOBESDK_ControlSurfaceLumetriSuite1;


#include <adobesdk/config/PostConfig.h>

#endif // ADOBE_SDK_CONTROLSURFACE_CONTROLSURFACELUMETRISUITE_H
