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

#ifndef ADOBESDK_CONSTROLSURFACE_PLUGIN_CONTROLSURFACECOMMANDBASE_H
#define ADOBESDK_CONSTROLSURFACE_PLUGIN_CONTROLSURFACECOMMANDBASE_H

#if defined(__APPLE_CC__)
#include <Cocoa/Cocoa.h> // Required by SweetPea
#endif

// adobesdk
#include "adobesdk/controlsurface/plugin/ControlSurfaceCommandSuite.h"

// SPSuites
#include <SPBasic.h>
#include <SPSuites.h>

// std
#include <string>

namespace adobesdk
{
	class ControlSurfaceCommandBase
	{
	public:
		ControlSurfaceCommandBase();
		virtual ~ControlSurfaceCommandBase();

		virtual SPErr GetUserButtonCount(
			uint32_t* outButtonCount) const;
		
		virtual SPErr GetUserButtonIDAtIndex(
			uint32_t inIndex,
			ADOBESDK_String* outButtonID) const;

		virtual SPErr GetUserButtonDisplayName(
			const ADOBESDK_String* inButtonID,
			ADOBESDK_String* outDisplayName) const;

		virtual SPErr SetUserButtonCommandID(
			const ADOBESDK_String* inButtonID,
			const ADOBESDK_String* inContextID,
			const ADOBESDK_String* inCommandID);

		virtual SPErr GetUserButtonCommandID(
			const ADOBESDK_String* inButtonID,
			ADOBESDK_String* outContextID,
			ADOBESDK_String* outCommandID) const;

		static void RegisterSuite(
			SPSuitesSuite*	inSPSuitesSuite,
			SPSuiteListRef	inSuiteList);

	}; // ControlSurfaceCommandBase

} // namespace ControlSurfaceCommandBase


#endif // ADOBESDK_CONSTROLSURFACE_PLUGIN_CONTROLSURFACECOMMANDBASE_H
