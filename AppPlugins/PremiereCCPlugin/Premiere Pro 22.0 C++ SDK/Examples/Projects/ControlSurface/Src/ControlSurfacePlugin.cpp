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

#include "ControlSurface.h"

// adobesdk
#include "adobesdk/controlsurface/plugin/ControlSurfacePluginSuite.h"
#include "adobesdk/controlsurface/plugin/ControlSurfaceSuite.h"

namespace
{
	ADOBESDK_ControlSurfaceHostID sHostIdentifier = kADOBESDK_ControlSurfaceHost_PremierePro;
	SPBasicSuite*		sSPBasicSuite = 0;
	SPSuitesSuite*		sSPSuitesSuite = 0;
	SPSuiteListRef		sSuiteList	= 0;
	ADOBESDK_StringSuite1*	sStringSuite = 0;

	ADOBESDK_UTF16Char kPluginIDString[] = { 'A', 'd', 'o', 'b', 'e', '.', 'C', 'o', 'n', 't', 'r', 'o', 'l', 'S', 'u', 'r', 'f', 'a', 'c', 'e', '.', 'S', 'a', 'm', 'p', 'l', 'e', 0 };
	ADOBESDK_UTF16Char kPluginDisplayString[] = { 'S', 'D', 'K', ' ', 'C', 'o', 'n', 't', 'r', 'o', 'l', ' ', 'S', 'u', 'r', 'f', 'a', 'c', 'e', ' ', 'S', 'a', 'm', 'p', 'l', 'e', 0 };

	SPErr GetString(const ADOBESDK_UTF16Char* inString, ADOBESDK_String* outBufferAsUnicode)
	{
		return sStringSuite->AllocateFromUTF16(inString, outBufferAsUnicode);
	}

} // namespace

namespace YourHardwareName
{


	class YourHardwareControlSurfacePlugin
	{
	public:
		YourHardwareControlSurfacePlugin()
			:
			mControlSurface(0)
		{

		}

		~YourHardwareControlSurfacePlugin()
		{
			Disconnect();
		}

		ADOBESDK_ControlSurfaceRef Connect(ADOBESDK_ControlSurfaceHostRef inHandlerRef)
		{
			mControlSurface = new YourHardwareControlSurface(sHostIdentifier, sSPBasicSuite);
			mControlSurface->SetControlSurfaceHost(inHandlerRef);
			return mControlSurface->GetControlSurfaceRef();
		}

		void Disconnect()
		{
			if (mControlSurface)
			{
				mControlSurface->SetControlSurfaceHost(0);
				delete mControlSurface;
				mControlSurface = 0;
			}
		}

		void ChangeMixerAutomationMode(ADOBESDK_ControlSurfaceChannelID inChannelID, ADOBESDK_ControlSurfaceAutomationMode inAutomationMode)
		{
			if (mControlSurface)
			{
				mControlSurface->ChangeMixerAutomationMode(inChannelID, inAutomationMode);
				mControlSurface->ChangeLumetriControls();
			}
		}

		void ChangeLumetriControls(void)
		{
			if (mControlSurface)
			{
				mControlSurface->ChangeLumetriControls();
			}
		}

	private:
		YourHardwareControlSurface*		mControlSurface;
	}; // YourHardwareControlSurfacePlugin
} // namespace YourHardwareName

namespace
{
	/////////////////////////////////////////////////////////////////////////////////////
	///
	SPErr ControlSurfacePlugin_Connect(ADOBESDK_ControlSurfacePluginRef inPluginRef, ADOBESDK_ControlSurfaceHostRef inHandlerRef, ADOBESDK_ControlSurfaceRef* outControlSurfaceRef)
	{
		*outControlSurfaceRef = reinterpret_cast<YourHardwareName::YourHardwareControlSurfacePlugin*>(inPluginRef)->Connect(inHandlerRef);
		return kSPNoError;
	}

	/////////////////////////////////////////////////////////////////////////////////////
	///
	SPErr ControlSurfacePlugin_Disconnect(ADOBESDK_ControlSurfacePluginRef inPluginRef)
	{
		reinterpret_cast<YourHardwareName::YourHardwareControlSurfacePlugin*>(inPluginRef)->Disconnect();
		return kSPNoError;
	}

	/////////////////////////////////////////////////////////////////////////////////////
	///
	SPErr ControlSurfacePlugin_GetPluginID(ADOBESDK_ControlSurfacePluginRef inPluginRef, ADOBESDK_String* outBufferAsUnicode)
	{
		return GetString(kPluginIDString, outBufferAsUnicode);
	}
	
	/////////////////////////////////////////////////////////////////////////////////////
	///
	SPErr ControlSurfacePlugin_GetPluginDisplayString(ADOBESDK_ControlSurfacePluginRef inPluginRef, ADOBESDK_String* outBufferAsUnicode)
	{
		return GetString(kPluginDisplayString, outBufferAsUnicode);
	}

	/////////////////////////////////////////////////////////////////////////////////////
	///
	SPErr ControlSurfacePlugin_GetPluginSettings(ADOBESDK_ControlSurfacePluginRef inPluginRef, ADOBESDK_String* outBufferAsUnicode)
	{
		/* [TODO]
		return GetString(
			reinterpret_cast<YourHardwareName::YourHardwareControlSurfacePlugin*>(inPluginRef)->GetSettings(),
			outBufferAsUnicode);
			*/
		return kSPUnimplementedError;
	}

	/////////////////////////////////////////////////////////////////////////////////////
	///
	SPErr ControlSurfacePlugin_SetPluginSettings(ADOBESDK_ControlSurfacePluginRef inPluginRef, const ADOBESDK_String* inSettings)
	{
		/* [TODO]
		if (inSettings)
		{		
			dvacore::UTF16String settings(static_cast<const dvacore::UTF16Char*>(inSettings));
			reinterpret_cast<YourHardwareName::YourHardwareControlSurfacePlugin*>(inPluginRef)->SetSettings(settings);
			return kSPNoError;
		}
		return kSPBadParameterError;
		*/
		return kSPUnimplementedError;
	}

	/////////////////////////////////////////////////////////////////////////////////////
	///
	SPErr ControlSurfacePlugin_HasConfigurationDialog(ADOBESDK_ControlSurfacePluginRef inPluginRef, ADOBESDK_Boolean* outHasDialog)
	{
		*outHasDialog = kAdobesdk_False;
		return kSPUnimplementedError;
	}

	/////////////////////////////////////////////////////////////////////////////////////
	///
	SPErr ControlSurfacePlugin_RunConfigurationDialog(ADOBESDK_ControlSurfacePluginRef inPluginRef, void* inParentWindow, ADOBESDK_Boolean* outSettingsChanged)
	{
		return kSPUnimplementedError;
	}

	/////////////////////////////////////////////////////////////////////////////////////
	///
	SPErr ControlSurfacePlugin_Suspend(ADOBESDK_ControlSurfacePluginRef inPluginRef)
	{
		return kSPUnimplementedError;
	}

	/////////////////////////////////////////////////////////////////////////////////////
	///
	SPErr ControlSurfacePlugin_Resume(ADOBESDK_ControlSurfacePluginRef inPluginRef)
	{
		// Do something interesting
		YourHardwareName::YourHardwareControlSurfacePlugin* yourPlugin = reinterpret_cast<YourHardwareName::YourHardwareControlSurfacePlugin*>(inPluginRef);
		// Call into the host mixer suite to SetAutomationMode() on a few channels
		yourPlugin->ChangeMixerAutomationMode(1, kADOBESDK_ControlSurfaceAutomationMode_Touch);
		yourPlugin->ChangeMixerAutomationMode(2, kADOBESDK_ControlSurfaceAutomationMode_Touch);
		yourPlugin->ChangeLumetriControls();

		return kSPUnimplementedError;
	}

	/////////////////////////////////////////////////////////////////////////////////////
	///
	ADOBESDK_ControlSurfacePluginSuite1 sPluginSuite =
	{
		ControlSurfacePlugin_Connect,
		ControlSurfacePlugin_Disconnect,
		ControlSurfacePlugin_GetPluginID,
		ControlSurfacePlugin_GetPluginDisplayString,
		ControlSurfacePlugin_GetPluginSettings,
		ControlSurfacePlugin_SetPluginSettings,
		ControlSurfacePlugin_HasConfigurationDialog,
		ControlSurfacePlugin_RunConfigurationDialog,
		ControlSurfacePlugin_Suspend,
		ControlSurfacePlugin_Resume
	};

	/////////////////////////////////////////////////////////////////////////////////////
	///
	SPErr ControlSurfacePlugin_Startup()
	{
		ADOBESDK_StringSuite1* memorySuite = 0;

		SPErr result = sSPBasicSuite->AcquireSuite(
			kADOBESDK_StringSuite,
			kADOBESDK_StringSuite_Version1,
			(const void**)&memorySuite);

		if (result == kSPNoError)
		{
			sStringSuite = memorySuite;

			result = sSPBasicSuite->AcquireSuite(kSPSuitesSuite, kSPSuitesSuiteVersion, (const void**)&sSPSuitesSuite);
			if (result == kSPNoError)
			{
				SPSuiteListRef suiteList = 0;
				if (sSPSuitesSuite->AllocateSuiteList(kSPRuntimeStringPool, kSPRuntimePluginList, &suiteList) == kSPNoError && suiteList != 0)
				{
					sSuiteList = suiteList;

					SPSuiteRef pluginSuiteRef = 0;
					sSPSuitesSuite->AddSuite(
						sSuiteList,
						NULL,
						kADOBESDK_ControlSurfacePluginSuite,
						kADOBESDK_ControlSurfacePluginSuite_Version1,
						kADOBESDK_ControlSurfacePluginSuite_Version1,
						&sPluginSuite,
						&pluginSuiteRef);

				}

				YourHardwareName::YourHardwareControlSurface::RegisterSuites(sSPSuitesSuite, sSuiteList);

			}
		}
		return result;
	}

	/////////////////////////////////////////////////////////////////////////////////////
	///
	SPErr ControlSurfacePlugin_Shutdown()
	{
		if (sSuiteList)
		{
			(void)sSPSuitesSuite->FreeSuiteList(sSuiteList);
			sSuiteList = 0;
		}

		if (sStringSuite)
		{
			sSPBasicSuite->ReleaseSuite(kADOBESDK_StringSuite, kADOBESDK_StringSuite_Version1);
			sStringSuite = 0;
		}

		return kSPNoError;
	}

	/////////////////////////////////////////////////////////////////////////////////////
	///
	SPErr ControlSurfacePlugin_CreatePluginInstance(ADOBESDK_ControlSurfacePluginRef* outControlSurfacePluginRef)
	{
		*outControlSurfacePluginRef = reinterpret_cast<ADOBESDK_ControlSurfacePluginRef>(new YourHardwareName::YourHardwareControlSurfacePlugin());
		return kSPNoError;
	}

	/////////////////////////////////////////////////////////////////////////////////////
	///
	SPErr ControlSurfacePlugin_DeletePluginInstance(ADOBESDK_ControlSurfacePluginRef inPluginRef)
	{
		delete reinterpret_cast<YourHardwareName::YourHardwareControlSurfacePlugin*>(inPluginRef);
		return kSPNoError;
	}

	/////////////////////////////////////////////////////////////////////////////////////
	///
	SPErr ControlSurfacePlugin_GetSuiteList(SPSuiteListRef* outSuiteListRef)
	{
		*outSuiteListRef = sSuiteList;
		return kSPNoError;
	}

	/////////////////////////////////////////////////////////////////////////////////////
	///
	ADOBESDK_ControlSurfacePluginFuncs sControlSurfacePluginFuncs =
	{
		ControlSurfacePlugin_Startup,
		ControlSurfacePlugin_Shutdown,
		ControlSurfacePlugin_CreatePluginInstance,
		ControlSurfacePlugin_DeletePluginInstance,
		ControlSurfacePlugin_GetSuiteList
	};
} // namespace

/////////////////////////////////////////////////////////////////////////////////////
///
extern "C" ADOBE_CONTROLSURFACE_API SPErr EntryPoint(
	struct SPBasicSuite*					inSPBasic,
	uint32_t								inMajorVersion,
	uint32_t								inMinorVersion,
	ADOBESDK_ControlSurfaceHostID			inHostIdentifier,
	ADOBESDK_ControlSurfacePluginFuncs*		outPluginFuncs)
{
	sHostIdentifier = inHostIdentifier;
	sSPBasicSuite = inSPBasic;

	if (sSPBasicSuite)
	{
		*outPluginFuncs = sControlSurfacePluginFuncs;
	}

	return sSPBasicSuite ? kSPNoError : kSPBadParameterError;
}
