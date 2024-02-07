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

#ifndef ADOBESDK_CONSTROLSURFACE_PLUGIN_CONTROLSURFACEMARKERBASE_H
#define ADOBESDK_CONSTROLSURFACE_PLUGIN_CONTROLSURFACEMARKERBASE_H

#if defined(__APPLE_CC__)
#include <Cocoa/Cocoa.h> // Required by SweetPea
#endif

// adobesdk
#include "adobesdk/controlsurface/plugin/ControlSurfaceMarkerSuite.h"

// SPSuites
#include <SPBasic.h>
#include <SPSuites.h>

// std
#include <string>

namespace adobesdk
{
	class ControlSurfaceMarkerBase
	{
	public:
		ControlSurfaceMarkerBase();
		virtual ~ControlSurfaceMarkerBase();
		
		virtual SPErr MarkerChanged(uint32_t inMarkerIndex);
		virtual SPErr RebuildMarkers();

		static void RegisterSuite(
			SPSuitesSuite*	inSPSuitesSuite,
			SPSuiteListRef	inSuiteList);

	}; // ControlSurfaceMarkerBase

} // namespace adobesdk


#endif // ADOBESDK_CONSTROLSURFACE_PLUGIN_CONTROLSURFACEMARKERBASE_H
