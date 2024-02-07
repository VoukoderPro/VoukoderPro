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

#ifndef ADOBESDK_CONSTROLSURFACE_PLUGIN_CONTROLSURFACEMIXERBASE_H
#define ADOBESDK_CONSTROLSURFACE_PLUGIN_CONTROLSURFACEMIXERBASE_H

#if defined(__APPLE_CC__)
#include <Cocoa/Cocoa.h> // Required by SweetPea
#endif

// adobesdk
#include "adobesdk/controlsurface/plugin/ControlSurfaceMixerSuite.h"

// SPSuites
#include <SPBasic.h>
#include <SPSuites.h>

// std
#include <string>

namespace adobesdk
{
	class ControlSurfaceMixerBase
	{
	public:
		ControlSurfaceMixerBase();
		virtual ~ControlSurfaceMixerBase();

		virtual SPErr GetMaxChannelStripCount(uint32_t* outMaxChannelStripCount);
		virtual SPErr ChannelConfigChanged(uint32_t inChannelID, uint32_t inSelector);
		virtual SPErr SelectionChanged();
		virtual SPErr SetChannelState(uint32_t inChannelID, uint32_t inChannelStateIndex, bool inState);
		virtual SPErr SetAutomationMode(uint32_t inChannelID, uint32_t inAutomationMode);
		virtual SPErr SetChannelOffset(uint32_t inChannelOffset);
		virtual SPErr SetChannelCount(uint32_t inChannelCount);
		virtual SPErr SetRudeSoloState(bool inValue);
		virtual SPErr SetComponentBypass(uint32_t inChannelID, uint32_t inComponentIndex, bool inBypass);
		virtual SPErr SetComponentParameter(uint32_t inChannelID, uint32_t inComponentIndex, uint32_t inParameterIndex, float inValue);
		virtual SPErr AddMasterFaderAsRegularChannel(bool* outValue);

		static void RegisterSuite(
			SPSuitesSuite*	inSPSuitesSuite,
			SPSuiteListRef	inSuiteList);

	}; // ControlSurfaceMixerBase

} // namespace ControlSurfaceMixerBase


#endif // ADOBESDK_CONSTROLSURFACE_PLUGIN_CONTROLSURFACEMIXERBASE_H
