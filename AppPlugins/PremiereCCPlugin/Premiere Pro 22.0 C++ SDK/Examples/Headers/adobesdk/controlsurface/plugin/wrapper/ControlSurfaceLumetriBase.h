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

#ifndef ADOBESDK_CONSTROLSURFACE_PLUGIN_CONTROLSURFACELUMETRIBASE_H
#define ADOBESDK_CONSTROLSURFACE_PLUGIN_CONTROLSURFACELUMETRIBASE_H

#if defined(__APPLE_CC__)
#include <Cocoa/Cocoa.h> // Required by SweetPea
#endif

// adobesdk
#include "adobesdk/controlsurface/plugin/ControlSurfaceLumetriSuite.h"

// SPSuites
#include <SPBasic.h>
#include <SPSuites.h>

// std
#include <string>

namespace adobesdk
{
	class ControlSurfaceLumetriBase
	{
	public:
		ControlSurfaceLumetriBase();
		virtual ~ControlSurfaceLumetriBase();

		virtual SPErr ModeChanged(uint32_t inMode);
		virtual SPErr ParameterValueChanged(
			const ADOBESDK_String* inParameterID,
			float inParameterValue);
		virtual SPErr MenuValueChanged(
			const ADOBESDK_String* inMenuID,
			const ADOBESDK_String* inDisplayString);
		virtual SPErr AllChanged();

		static void RegisterSuite(
			SPSuitesSuite*	inSPSuitesSuite,
			SPSuiteListRef	inSuiteList);
	}; // ControlSurfaceLumetriBase

} // namespace ControlSurfaceLumetriBase


#endif // ADOBESDK_CONSTROLSURFACE_PLUGIN_CONTROLSURFACELUMETRIBASE_H
