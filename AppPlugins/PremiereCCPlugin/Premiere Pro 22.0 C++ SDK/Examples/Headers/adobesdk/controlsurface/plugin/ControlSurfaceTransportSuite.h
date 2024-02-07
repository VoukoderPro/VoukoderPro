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

#ifndef ADOBE_SDK_CONTROLSURFACE_CONTROLSURFACETRANSPORTSUITE_H
#define ADOBE_SDK_CONTROLSURFACE_CONTROLSURFACETRANSPORTSUITE_H

// adobesdk
#include <adobesdk/config/PreConfig.h>
#include <adobesdk/controlsurface/ControlSurfaceTypes.h>

// SweetPea2
#include <SPTypes.h>

#define kADOBESDK_ControlSurfaceTransportSuite					"ADOBESDK ControlSurface Transport Suite"
#define kADOBESDK_ControlSurfaceTransportSuite_Version1			1

#define kADOBESDK_ControlSurfaceTransportSuite_VersionCurrent	kADOBESDK_ControlSurfaceTransportSuite_Version1
#define	kADOBESDK_ControlSurfaceTransportSuiteCurrent			ADOBESDK_ControlSurfaceTransportSuite1

typedef struct
{
	SPErr (*SetStopState)(
		ADOBESDK_ControlSurfaceTransportRef	inControlSurfaceTransportRef,
		ADOBESDK_Boolean					inState);

	SPErr (*SetPlayState)(
		ADOBESDK_ControlSurfaceTransportRef	inControlSurfaceTransportRef,
		ADOBESDK_Boolean					inState);

	SPErr (*SetRecordState)(
		ADOBESDK_ControlSurfaceTransportRef	inControlSurfaceTransportRef,
		ADOBESDK_Boolean					inState);

	SPErr (*SetRewindState)(
		ADOBESDK_ControlSurfaceTransportRef	inControlSurfaceTransportRef,
		ADOBESDK_Boolean					inState);

	SPErr (*SetForwardState)(
		ADOBESDK_ControlSurfaceTransportRef	inControlSurfaceTransportRef,
		ADOBESDK_Boolean					inState);

	SPErr (*SetCycleState)(
		ADOBESDK_ControlSurfaceTransportRef	inControlSurfaceTransportRef,
		ADOBESDK_Boolean					inState);

	SPErr (*SetPauseState)(
		ADOBESDK_ControlSurfaceTransportRef	inControlSurfaceTransportRef,
		ADOBESDK_Boolean					inState);

	SPErr (*SetTimeDisplay)(
		ADOBESDK_ControlSurfaceTransportRef	inControlSurfaceTransportRef,
		uint32_t							inTimeDisplayMode,
		const ADOBESDK_String*				inDisplayString);

} ADOBESDK_ControlSurfaceTransportSuite1;


#include <adobesdk/config/PostConfig.h>

#endif // ADOBE_SDK_CONTROLSURFACE_CONTROLSURFACETRANSPORTSUITE_H
