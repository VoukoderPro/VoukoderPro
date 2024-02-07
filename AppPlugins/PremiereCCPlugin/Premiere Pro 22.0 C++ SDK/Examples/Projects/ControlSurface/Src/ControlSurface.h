/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2012 Adobe Systems Incorporated                       */
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

#ifndef YOUR_HARDWARE_CONTROL_SURFACE_PLUGIN_H
#define YOUR_HARDWARE_CONTROL_SURFACE_PLUGIN_H

#if defined(__APPLE_CC__)
#include <Cocoa/Cocoa.h> // Required by SweetPea
#endif

// adobesdk
#include "adobesdk/AdobesdkStringSuite.h"
#include "adobesdk/controlsurface/host/ControlSurfaceHostSuite.h"
#include "adobesdk/controlsurface/host/ControlSurfaceHostTransportSuite.h"
#include "adobesdk/controlsurface/host/ControlSurfaceHostCommandSuite.h"
#include "adobesdk/controlsurface/host/ControlSurfaceHostMixerSuite.h"
#include "adobesdk/controlsurface/host/ControlSurfaceHostLumetriSuite.h"
#include "adobesdk/controlsurface/plugin/wrapper/ControlSurfaceBase.h"
#include "adobesdk/controlsurface/plugin/wrapper/ControlSurfaceTransportBase.h"
#include "adobesdk/controlsurface/plugin/wrapper/ControlSurfaceCommandBase.h"
#include "adobesdk/controlsurface/plugin/wrapper/ControlSurfaceMixerBase.h"
#include "adobesdk/controlsurface/plugin/wrapper/ControlSurfaceLumetriBase.h"

// SPSuites
#include <SPBasic.h>
#include <SPSuites.h>

// std
#include <string>

namespace YourHardwareName
{
	class YourHardwareControlSurface
		:
		adobesdk::ControlSurfaceBase,
		adobesdk::ControlSurfaceTransportBase,
		adobesdk::ControlSurfaceCommandBase,
		adobesdk::ControlSurfaceMixerBase
	{
	public:
		YourHardwareControlSurface(ADOBESDK_ControlSurfaceHostID inHostIdentifier, SPBasicSuite* inSPBasic);
		virtual ~YourHardwareControlSurface();

		void SetControlSurfaceHost(ADOBESDK_ControlSurfaceHostRef inControlSurfaceHostRef);

		ADOBESDK_ControlSurfaceRef GetControlSurfaceRef();		

		// ControlSurface
		virtual SPErr GetConfigIdentifier(ADOBESDK_UTF16Char** outCongigIdentifier, uint32_t* outConfigIdentiferLength) const;
		virtual SPErr GetControlSurfaceFlags(uint32_t* outControlSurfaceFlags) const;
		virtual SPErr GetTransportRef(ADOBESDK_ControlSurfaceTransportRef* outTransportRef);
		virtual SPErr GetCommandRef(ADOBESDK_ControlSurfaceCommandRef* outCommandRef);
		virtual SPErr GetMixerRef(ADOBESDK_ControlSurfaceMixerRef* outMixerRef);
		virtual SPErr Update();

		// Transport
		virtual SPErr SetPlayState(bool inState);
		virtual SPErr SetRecordState(bool inState);
		virtual SPErr SetRewindState(bool inState);
		virtual SPErr SetForwardState(bool inState);
		virtual SPErr SetTimeDisplay(uint32_t inTimeDisplayMode, const ADOBESDK_UTF16Char* inDisplayString);

		// Command
		virtual SPErr GetUserButtonCount(uint32_t* outButtonCount) const;
		virtual SPErr GetUserButtonIDAtIndex(uint32_t inIndex, ADOBESDK_UTF16Char** outButtonID, uint32_t* outButtonIDLength) const;
		virtual SPErr GetUserButtonDisplayName(const ADOBESDK_UTF16Char* inButtonID, ADOBESDK_UTF16Char** outDisplayName, uint32_t* outDisplayNameLength) const;
		virtual SPErr SetUserButtonCommandID(const ADOBESDK_UTF16Char* inButtonID, const ADOBESDK_UTF16Char* inCommandID);
		virtual SPErr GetUserButtonCommandID(const ADOBESDK_UTF16Char* inButtonID, ADOBESDK_UTF16Char** outCommandID, uint32_t* outCommandIDLength) const;

		// Custom
		virtual SPErr ChangeMixerAutomationMode(ADOBESDK_ControlSurfaceChannelID inChannelID, ADOBESDK_ControlSurfaceAutomationMode inAutomationMode) const;
		virtual SPErr ChangeLumetriControls(void) const;

		static void RegisterSuites(
			SPSuitesSuite*	inSPSuitesSuite,
			SPSuiteListRef	inSuiteList);


	private:
		virtual void OnHardwareCommandAction(uint32_t inAction, bool inOn);
		virtual void OnHardwareTransportFunction(int32_t inJog, int32_t inShuttle);

		//void OnDeviceBecameOnline();

	private:
		ADOBESDK_ControlSurfaceHostID 				mHostIdentifier;
//		boost::shared_ptr<YourHardwareIO>			mYourHardwareIO;
		SPBasicSuite*								mSPBasic;
		ADOBESDK_ControlSurfaceHostSuite1*			mControlSurfaceHostSuite;
		ADOBESDK_ControlSurfaceHostTransportSuite1*	mControlSurfaceHostTransportSuite;
		ADOBESDK_ControlSurfaceHostCommandSuite1*	mControlSurfaceHostCommandSuite;
		ADOBESDK_ControlSurfaceHostMixerSuite1*		mControlSurfaceHostMixerSuite;
		ADOBESDK_ControlSurfaceHostLumetriSuite1*	mControlSurfaceHostLumetriSuite;
		ADOBESDK_ControlSurfaceHostRef				mControlSurfaceHostRef;
		ADOBESDK_ControlSurfaceHostTransportRef		mControlSurfaceHostTransportRef;
		ADOBESDK_ControlSurfaceHostCommandRef		mControlSurfaceHostCommandRef;
		ADOBESDK_ControlSurfaceHostMixerRef			mControlSurfaceHostMixerRef;
		ADOBESDK_ControlSurfaceHostLumetriRef		mControlSurfaceHostLumetriRef;
		uint32_t									mUpdateCounter;
//		dvacore::utility::TimeStamp					mLastRewindPressedTime;
//		dvacore::utility::TimeStamp					mLastForwardPressedTime;
		
//		dvacore::UTF16String						mLastTimeDisplayString;

//		typedef std::map<dvacore::StdString, dvacore::UTF16String> ButtonToCommandMap;
//		ButtonToCommandMap							mButtonToCommandMap;

//		typedef std::map<dvacore::UTF16String, dvacore::StdString> CommandIDToNameMap;
//		CommandIDToNameMap							mCommandIDToNameMap;

	}; // YourHardwareControlSurface

} // namespace YourHardwareName


#endif // YOUR_HARDWARE_CONTROL_SURFACE_PLUGIN_H
