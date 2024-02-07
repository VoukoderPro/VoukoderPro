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

#ifndef ADOBESDK_CONTROLSURFACE_CONTROLSURFACEPLUGINSUITE_H
#define ADOBESDK_CONTROLSURFACE_CONTROLSURFACEPLUGINSUITE_H

// adobesdk
#include <adobesdk/config/PreConfig.h>

#include <adobesdk/controlsurface/ControlSurfaceTypes.h>
#include <adobesdk/controlsurface/plugin/ControlSurfaceSuite.h>

// SweetPea2
#include <SPTypes.h>
#include <SPBasic.h>
#include <SPSuites.h>

#define kADOBESDK_ControlSurfacePluginSuite					"ADOBESDK ControlSurfacePlugin Suite"
#define kADOBESDK_ControlSurfacePluginSuite_Version1			1

#define kADOBESDK_ControlSurfacePluginSuite_VersionCurrent	kADOBESDK_ControlSurfacePluginSuite_Version1
#define	kADOBESDK_ControlSurfacePluginSuiteCurrent			ADOBESDK_ControlSurfacePluginSuite1

typedef struct
{
	// Connects the control surface with the handler
	SPErr (*Connect)(
		ADOBESDK_ControlSurfacePluginRef	inPluginRef,
		ADOBESDK_ControlSurfaceHostRef		inHostRef,
		ADOBESDK_ControlSurfaceRef*			outControlSurfaceRef);

	// Disconnects the control surface from the handler
	SPErr (*Disconnect)(ADOBESDK_ControlSurfacePluginRef inPluginRef);

	SPErr (*GetPluginID)(
		ADOBESDK_ControlSurfacePluginRef	inPluginRef,
		ADOBESDK_String*					outPluginID);

	SPErr (*GetPluginDisplayString)(
		ADOBESDK_ControlSurfacePluginRef	inPluginRef,
		ADOBESDK_String*					outDisplayString);

	SPErr (*GetPluginSettings)(
		ADOBESDK_ControlSurfacePluginRef	inPluginRef,
		ADOBESDK_String*					outSettingsString);

	// Sets the settings to the plug-in after they have been read from the preferences
	SPErr (*SetPluginSettings)(
		ADOBESDK_ControlSurfacePluginRef	inPluginRef,
		const ADOBESDK_String*				inSettings);

	// Return true if the plug-in has a configuration dialog.
	SPErr (*HasSettingsDialog)(
		ADOBESDK_ControlSurfacePluginRef	inPluginRef,
		ADOBESDK_Boolean*					outHasSettings);

	// Run the configuration dialog. Return true if the settings were changed.
	// inParentWindow represents NSWindow* on MacOS and HWND on Windows.
	SPErr (*RunSettingsDialog)(
		ADOBESDK_ControlSurfacePluginRef	inPluginRef,
		void*								inParentWindow,
		ADOBESDK_Boolean*					outSettingsChanged);

	// Called by the host when the application loses focus
	SPErr (*Suspend)(
		ADOBESDK_ControlSurfacePluginRef	inPluginRef);

	// Called by the host when the application gains focus again after being suspended
	SPErr (*Resume)(
		ADOBESDK_ControlSurfacePluginRef	inPluginRef);


} ADOBESDK_ControlSurfacePluginSuite1;

typedef struct
{
	// One time startup function for the plug-in instance
	SPErr (*Startup)();

	// One time shutdown function for the plug-in instance
	SPErr (*Shutdown)();

	SPErr (*CreatePluginInstance)(ADOBESDK_ControlSurfacePluginRef* outPluginInstanceRef);

	SPErr (*DeletePluginInstance)(ADOBESDK_ControlSurfacePluginRef inPluginInstanceRef);

	// Returns the list of supported suites
	SPErr (*GetSuiteList)(SPSuiteListRef* outSuiteListRef);

} ADOBESDK_ControlSurfacePluginFuncs;


#if defined(MAC_ENV)
#	define ADOBE_CONTROLSURFACE_API __attribute__((visibility("default")))
#endif

#if defined(WIN_ENV)
#	define ADOBE_CONTROLSURFACE_API __declspec(dllexport)
#endif

/*
ADOBE_CONTROLSURFACE_API SPErr EntryPoint(
	struct SPBasicSuite*					inSPBasic,
	uint32_t								inMajorVersion,
	uint32_t								inMinorVersion,
	ADOBESDK_ControlSurfaceHostID			inHostIdentifier,
	ADOBESDK_ControlSurfacePluginFuncs*		outPluginFuncs);
*/
typedef SPErr (*ADOBESDK_ControlSurfaceEntryPoint)(
	struct SPBasicSuite*					inSPBasic,
	uint32_t								inMajorVersion,
	uint32_t								inMinorVersion,
	ADOBESDK_ControlSurfaceHostID			inHostIdentifier,
	ADOBESDK_ControlSurfacePluginFuncs*		outPluginFuncs);


#include <adobesdk/config/PostConfig.h>

#endif // ADOBESDK_CONTROLSURFACE_CONTROLSURFACEPLUGINSUITE_H
