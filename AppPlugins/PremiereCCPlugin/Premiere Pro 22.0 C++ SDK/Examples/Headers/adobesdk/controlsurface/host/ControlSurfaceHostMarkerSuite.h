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

#ifndef ADOBESDK_CONTROLSURFACE_CONTROLSURFACEHOSTMARKERSUITE_H
#define ADOBESDK_CONTROLSURFACE_CONTROLSURFACEHOSTMARKERSUITE_H

// adobesdk
#include <adobesdk/config/PreConfig.h>
#include <adobesdk/controlsurface/ControlSurfaceTypes.h>

// SweetPea2
#include <SPTypes.h>

#define kADOBESDK_ControlSurfaceHostMarkerSuite					"ADOBESDK ControlSurfaceHost Marker Suite"
#define kADOBESDK_ControlSurfaceHostMarkerSuite_Version1			1

#define kADOBESDK_ControlSurfaceHostMarkerSuite_VersionCurrent	kADOBESDK_ControlSurfaceHostMarkerSuite_Version1
#define	kADOBESDK_ControlSurfaceHostMarkerSuiteCurrent			ADOBESDK_ControlSurfaceHostMarkerSuite1

typedef struct
{
	// Returns the number of markers in the current document.
	SPErr (*GetMarkerCount)(
		ADOBESDK_ControlSurfaceHostMarkerRef	inControlSurfaceHandler,
		uint32_t*								outMarkerCount);

	// <outMarkerName> receives the marker name as a zero terminated UTF16 string. It must be freed via ADOBESDK_ControlSurfaceHostMemorySuite1.FreeString().
	SPErr (*GetMarkerName)(
		ADOBESDK_ControlSurfaceHostMarkerRef	inControlSurfaceHostMarkerRef,
		uint32_t								inMarkerIndex,				///< The index of the marker.
		uint32_t								inMaxLength,				///< Maximum number of characters. String will be shortened accordingly.
		ADOBESDK_String*						outMarkerName);

	// Jumps to the marker with the given index.
	SPErr (*GotoMarker)(
		ADOBESDK_ControlSurfaceHostMarkerRef	inControlSurfaceHostMarkerRef,
		uint32_t								inMarkerIndex);

} ADOBESDK_ControlSurfaceHostMarkerSuite1;


#include <adobesdk/config/PostConfig.h>

#endif // ADOBESDK_CONTROLSURFACE_CONTROLSURFACEHOSTMARKERSUITE_H
