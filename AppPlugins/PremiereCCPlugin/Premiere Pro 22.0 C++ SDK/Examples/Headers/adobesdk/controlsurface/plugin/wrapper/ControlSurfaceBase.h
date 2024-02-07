/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2011 Adobe Systems Incorporated                       */
/* All Rights Reserved.                                            */
/*                                                                 */
/* NOTICE:  All information contained herein is, and remains the   */
/* property of Adobe Systems Incorporated and its suppliers, if    */
/* any.  The intellectual and technical concepts contained         */
/* herein are proprietary to Adobe Systems Incorporated and its    */
/* suppliers and may be covered by U.S. and Foreign Patents,       */
/* patents in process, and are protected by trade secret or        */
/* copyright law.  Dissemination of this information or            */
/* reproduction of this material is strictly forbidden unless      */
/* prior written permission is obtained from Adobe Systems         */
/* Incorporated.                                                   */
/*                                                                 */
/*******************************************************************/

#ifndef ADOBESDK_CONSTROLSURFACE_PLUGIN_CONTROLSURFACEBASE_H
#define ADOBESDK_CONSTROLSURFACE_PLUGIN_CONTROLSURFACEBASE_H

#if defined(__APPLE_CC__)
#include <Cocoa/Cocoa.h> // Required by SweetPea
#endif

// adobesdk
#include "adobesdk/controlsurface/plugin/ControlSurfaceSuite.h"

// SPSuites
#include <SPBasic.h>
#include <SPSuites.h>

// std
#include <string>

namespace adobesdk
{
	class ControlSurfaceBase
	{
	public:
		ControlSurfaceBase();
		virtual ~ControlSurfaceBase();
		
		virtual SPErr GetConfigIdentifier(ADOBESDK_String* outConfigIdentifier) const;
		virtual SPErr GetControlSurfaceFlags(uint32_t* outControlSurfaceFlags) const;

		virtual SPErr GetTransportRef(ADOBESDK_ControlSurfaceTransportRef* outTransportRef);
		virtual SPErr GetMixerRef(ADOBESDK_ControlSurfaceMixerRef* outMixerRef);
		virtual SPErr GetMarkerRef(ADOBESDK_ControlSurfaceMarkerRef* outMarkerRef);
		virtual SPErr GetCommandRef(ADOBESDK_ControlSurfaceCommandRef* outCommandRef);
		virtual SPErr GetLumetriRef(ADOBESDK_ControlSurfaceLumetriRef* outLumetriRef);
		
		virtual SPErr Update();

		static void RegisterSuite(
			SPSuitesSuite*	inSPSuitesSuite,
			SPSuiteListRef	inSuiteList);

	}; // ControlSurfaceBase

} // namespace adobesdk


#endif // ADOBESDK_CONSTROLSURFACE_PLUGIN_CONTROLSURFACEBASE_H
