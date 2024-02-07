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
#include "adobesdk/controlsurface/plugin/ControlSurfaceSuite.h"
#include "adobesdk/controlsurface/plugin/ControlSurfaceTransportSuite.h"
#include "adobesdk/controlsurface/plugin/ControlSurfaceCommandSuite.h"
#include "adobesdk/controlsurface/plugin/ControlSurfaceMixerSuite.h"
#include "adobesdk/controlsurface/plugin/ControlSurfaceLumetriSuite.h"

// std
#include <iosfwd>
#include <iomanip>
#include <cmath>

namespace YourHardwareName
{
	namespace
	{
		enum
		{
			kCommandAction_TransportShuttleLeft		= 0x20003,
			kCommandAction_TransportShuttleStop		= 0x20002,
			kCommandAction_TransportShuttleRight	= 0x20001,
			kCommandAction_TransportPreviousFrame	= 0x20007,
			kCommandAction_TransportNextFrame		= 0x20006
		};
		
		//////////////////////////////////////////////////////////////////////////////
		///
		float GetClosestValue(float fval, float inStep)
		{
			float sign = fval >= 0.0f? 1.0f : -1.0f;
			return static_cast<float>(static_cast<int>((std::abs(fval) + inStep / 2) / inStep) * inStep * sign);
		}

		//////////////////////////////////////////////////////////////////////////////
		///
		std::string UnicodeToAscii(const ADOBESDK_UTF16Char* inUnicode)
		{
			std::string result;

			const ADOBESDK_UTF16Char* p = inUnicode;
			while (ADOBESDK_UTF16Char ch = *p++)
			{
				if (ch < 128)
				{
					result.push_back(static_cast<char>(ch));
				}
				else
				{
					result.push_back('_');
				}
			}
			return result;
		}
	} // namespace

	//////////////////////////////////////////////////////////////////////////////
	///
	YourHardwareControlSurface::YourHardwareControlSurface(ADOBESDK_ControlSurfaceHostID inHostIdentifier, SPBasicSuite* inSPBasic)
		:
		mHostIdentifier(inHostIdentifier),
		mSPBasic(inSPBasic),
		mControlSurfaceHostTransportSuite(0),
		mControlSurfaceHostCommandSuite(0),
		mControlSurfaceHostMixerSuite(0),
		mControlSurfaceHostLumetriSuite(0),
		mControlSurfaceHostTransportRef(0),
		mControlSurfaceHostCommandRef(0),
		mControlSurfaceHostMixerRef(0),
		mControlSurfaceHostLumetriRef(0),
		mControlSurfaceHostRef(0),
		mUpdateCounter(0)
//		mLastRewindPressedTime(0),
//		mLastForwardPressedTime(0)
	{
		// Add some default commands - @todo: should be populated from config files/XML
		if (mHostIdentifier == kADOBESDK_ControlSurfaceHost_PremierePro)
		{
/*			mButtonToCommandMap.insert(std::make_pair(dvacore::StdString("Mf (1) - 1"), DVA_STR("cmd.transport.step.back")));
			mButtonToCommandMap.insert(std::make_pair(dvacore::StdString("Mf (1) - 2"), DVA_STR("cmd.transport.step.forward")));
			mButtonToCommandMap.insert(std::make_pair(dvacore::StdString("Mf (1) - 3"), DVA_STR("cmd.transport.toggleplay")));
			mButtonToCommandMap.insert(std::make_pair(dvacore::StdString("Mf (1) - 4"), DVA_STR("cmd.transport.playintoout")));
			mButtonToCommandMap.insert(std::make_pair(dvacore::StdString("Mf (1) - 5"), DVA_STR("cmd.transport.playedit")));
			mButtonToCommandMap.insert(std::make_pair(dvacore::StdString("Mf (1) - 6"), DVA_STR("cmd.transport.take.av")));
			mButtonToCommandMap.insert(std::make_pair(dvacore::StdString("Mf (1) - 7"), DVA_STR("cmd.transport.take.audio")));
			mButtonToCommandMap.insert(std::make_pair(dvacore::StdString("Mf (1) - 8"), DVA_STR("cmd.transport.take.video")));
			mButtonToCommandMap.insert(std::make_pair(dvacore::StdString("Mf (1) - 9"), DVA_STR("cmd.transport.toggletakeav")));
			mButtonToCommandMap.insert(std::make_pair(dvacore::StdString("Mf (1) - 10"), DVA_STR("cmd.transport.shuttle.slow.left")));
			mButtonToCommandMap.insert(std::make_pair(dvacore::StdString("Mf (1) - 11"), DVA_STR("cmd.transport.shuttle.slow.right")));
			mButtonToCommandMap.insert(std::make_pair(dvacore::StdString("Mf (1) - 12"), DVA_STR("cmd.transport.shuttle.stop")));

			mButtonToCommandMap.insert(std::make_pair(dvacore::StdString("Bt (1) - 1"), DVA_STR("cmd.edit.undo")));
			mButtonToCommandMap.insert(std::make_pair(dvacore::StdString("Bt (1) - 2"), DVA_STR("cmd.edit.redo")));
			mButtonToCommandMap.insert(std::make_pair(dvacore::StdString("Bt (1) - 3"), DVA_STR("cmd.edit.cut")));
			mButtonToCommandMap.insert(std::make_pair(dvacore::StdString("Bt (1) - 4"), DVA_STR("cmd.edit.copy")));
			mButtonToCommandMap.insert(std::make_pair(dvacore::StdString("Bt (1) - 5"), DVA_STR("cmd.edit.paste")));
			mButtonToCommandMap.insert(std::make_pair(dvacore::StdString("Bt (1) - 6"), DVA_STR("cmd.edit.clear")));
			mButtonToCommandMap.insert(std::make_pair(dvacore::StdString("Bt (1) - 7"), DVA_STR("cmd.edit.rippledelete")));
			mButtonToCommandMap.insert(std::make_pair(dvacore::StdString("Bt (1) - 8"), DVA_STR("cmd.edit.duplicate")));
			mButtonToCommandMap.insert(std::make_pair(dvacore::StdString("Bt (1) - 9"), DVA_STR("cmd.edit.selectall")));
			mButtonToCommandMap.insert(std::make_pair(dvacore::StdString("Bt (1) - 10"), DVA_STR("cmd.edit.deselectall")));
			mButtonToCommandMap.insert(std::make_pair(dvacore::StdString("Bt (1) - 11"), DVA_STR("cmd.zoom.in")));
			mButtonToCommandMap.insert(std::make_pair(dvacore::StdString("Bt (1) - 12"), DVA_STR("cmd.zoom.out")));

			mButtonToCommandMap.insert(std::make_pair(dvacore::StdString("Mf (1) - 1 (Alt)"), DVA_STR("cmd.common.setin")));
			mButtonToCommandMap.insert(std::make_pair(dvacore::StdString("Mf (1) - 2 (Alt)"), DVA_STR("cmd.common.setout")));
			mButtonToCommandMap.insert(std::make_pair(dvacore::StdString("Mf (1) - 4 (Alt)"), DVA_STR("cmd.goto.in")));
			mButtonToCommandMap.insert(std::make_pair(dvacore::StdString("Mf (1) - 5 (Alt)"), DVA_STR("cmd.goto.out")));
			mButtonToCommandMap.insert(std::make_pair(dvacore::StdString("Mf (1) - 6 (Alt)"), DVA_STR("cmd.clear.in")));
			mButtonToCommandMap.insert(std::make_pair(dvacore::StdString("Mf (1) - 7 (Alt)"), DVA_STR("cmd.clear.out")));
			mButtonToCommandMap.insert(std::make_pair(dvacore::StdString("Mf (1) - 8 (Alt)"), DVA_STR("cmd.clear.inandout")));
			mButtonToCommandMap.insert(std::make_pair(dvacore::StdString("Mf (1) - 9 (Alt)"), DVA_STR("cmd.set.marker.unnumbered")));
			mButtonToCommandMap.insert(std::make_pair(dvacore::StdString("Mf (1) - 10 (Alt)"), DVA_STR("cmd.set.marker.numbered")));
			mButtonToCommandMap.insert(std::make_pair(dvacore::StdString("Mf (1) - 11 (Alt)"), DVA_STR("cmd.transport.play.ctitoout")));
			mButtonToCommandMap.insert(std::make_pair(dvacore::StdString("Mf (1) - 12 (Alt)"), DVA_STR("cmd.transport.play.fat")));
*/		}
		else if (mHostIdentifier == kADOBESDK_ControlSurfaceHost_Audition)
		{
//			mButtonToCommandMap.insert(std::make_pair(dvacore::StdString("Mf (1) - 1"), DVA_STR("Edit.Undo")));
//			mButtonToCommandMap.insert(std::make_pair(dvacore::StdString("Mf (1) - 2"), DVA_STR("Edit.Redo")));
		}

	}

	//////////////////////////////////////////////////////////////////////////////
	///
	YourHardwareControlSurface::~YourHardwareControlSurface()
	{
	}

	//////////////////////////////////////////////////////////////////////////////
	///
	void YourHardwareControlSurface::SetControlSurfaceHost(ADOBESDK_ControlSurfaceHostRef inControlSurfaceHostRef)
	{
		if (mControlSurfaceHostSuite)
		{
			mSPBasic->ReleaseSuite(kADOBESDK_ControlSurfaceHostSuite, kADOBESDK_ControlSurfaceHostSuite_Version1);
			mControlSurfaceHostSuite = 0;
		}

		if (mControlSurfaceHostTransportSuite)
		{
			mControlSurfaceHostTransportRef = 0;
			mSPBasic->ReleaseSuite(kADOBESDK_ControlSurfaceHostTransportSuite, kADOBESDK_ControlSurfaceHostTransportSuite_Version1);
			mControlSurfaceHostTransportSuite = 0;
		}

		if (mControlSurfaceHostCommandSuite)
		{
			mControlSurfaceHostCommandRef = 0;
			mSPBasic->ReleaseSuite(kADOBESDK_ControlSurfaceHostCommandSuite, kADOBESDK_ControlSurfaceHostCommandSuite_Version1);
			mControlSurfaceHostCommandSuite = 0;
		}

		if (mControlSurfaceHostMixerSuite)
		{
			mControlSurfaceHostMixerRef = 0;
			mSPBasic->ReleaseSuite(kADOBESDK_ControlSurfaceHostMixerSuite, kADOBESDK_ControlSurfaceHostMixerSuite_Version1);
			mControlSurfaceHostMixerSuite = 0;
		}

		if (mControlSurfaceHostLumetriSuite)
		{
			mControlSurfaceHostLumetriRef = 0;
			mSPBasic->ReleaseSuite(kADOBESDK_ControlSurfaceHostLumetriSuite, kADOBESDK_ControlSurfaceHostLumetriSuite_Version1);
			mControlSurfaceHostLumetriSuite = 0;
		}

//		mYourHardwareIO.reset();
		
		mControlSurfaceHostRef = 0;

		if (inControlSurfaceHostRef)
		{
			ADOBESDK_ControlSurfaceHostSuite1* controlSurfaceHostSuite = 0;
			if (mSPBasic->AcquireSuite(
				kADOBESDK_ControlSurfaceHostSuite,
				kADOBESDK_ControlSurfaceHostSuite_Version1,
				(const void**)&controlSurfaceHostSuite) == kSPNoError && controlSurfaceHostSuite)
			{
				mControlSurfaceHostSuite = controlSurfaceHostSuite;
			}
			if (mControlSurfaceHostSuite)
			{
//				mYourHardwareIO.reset(new HardwareIO(DVA_STR("Your Hardware Driver"),
			
				ADOBESDK_ControlSurfaceHostTransportSuite1* controlSurfaceHostTransportSuite = 0;
				if (mSPBasic->AcquireSuite(
					kADOBESDK_ControlSurfaceHostTransportSuite,
					kADOBESDK_ControlSurfaceHostTransportSuite_Version1,
					(const void**)&controlSurfaceHostTransportSuite) == kSPNoError && controlSurfaceHostTransportSuite)
				{
					mControlSurfaceHostTransportSuite = controlSurfaceHostTransportSuite;
					if (mControlSurfaceHostSuite->GetTransportRef(inControlSurfaceHostRef, &mControlSurfaceHostTransportRef) != kSPNoError)
					{
						mSPBasic->ReleaseSuite(kADOBESDK_ControlSurfaceHostTransportSuite, kADOBESDK_ControlSurfaceHostTransportSuite_Version1);
						mControlSurfaceHostTransportSuite = 0;
					}
				}

				ADOBESDK_ControlSurfaceHostCommandSuite1* controlSurfaceHostCommandSuite = 0;
				if (mSPBasic->AcquireSuite(
					kADOBESDK_ControlSurfaceHostCommandSuite,
					kADOBESDK_ControlSurfaceHostCommandSuite_Version1,
					(const void**)&controlSurfaceHostCommandSuite) == kSPNoError && controlSurfaceHostCommandSuite)
				{
					mControlSurfaceHostCommandSuite = controlSurfaceHostCommandSuite;
					if (mControlSurfaceHostSuite->GetCommandRef(inControlSurfaceHostRef, &mControlSurfaceHostCommandRef) != kSPNoError)
					{
						mSPBasic->ReleaseSuite(kADOBESDK_ControlSurfaceHostCommandSuite, kADOBESDK_ControlSurfaceHostCommandSuite_Version1);
						mControlSurfaceHostCommandSuite = 0;
					}
				}

				ADOBESDK_ControlSurfaceHostMixerSuite1* controlSurfaceHostMixerSuite = 0;
				if (mSPBasic->AcquireSuite(
					kADOBESDK_ControlSurfaceHostMixerSuite,
					kADOBESDK_ControlSurfaceHostMixerSuite_Version1,
					(const void**)&controlSurfaceHostMixerSuite) == kSPNoError && controlSurfaceHostMixerSuite)
				{
					mControlSurfaceHostMixerSuite = controlSurfaceHostMixerSuite;
					if (mControlSurfaceHostSuite->GetMixerRef(inControlSurfaceHostRef, &mControlSurfaceHostMixerRef) != kSPNoError)
					{
						mSPBasic->ReleaseSuite(kADOBESDK_ControlSurfaceHostMixerSuite, kADOBESDK_ControlSurfaceHostMixerSuite_Version1);
						mControlSurfaceHostMixerSuite = 0;
					}
				}

				ADOBESDK_ControlSurfaceHostLumetriSuite1* controlSurfaceHostLumetriSuite = 0;
				if (mSPBasic->AcquireSuite(
					kADOBESDK_ControlSurfaceHostLumetriSuite,
					kADOBESDK_ControlSurfaceHostLumetriSuite_Version1,
					(const void**)&controlSurfaceHostLumetriSuite) == kSPNoError && controlSurfaceHostLumetriSuite)
				{
					mControlSurfaceHostLumetriSuite = controlSurfaceHostLumetriSuite;
					if (mControlSurfaceHostSuite->GetLumetriRef(inControlSurfaceHostRef, &mControlSurfaceHostLumetriRef) != kSPNoError)
					{
						mSPBasic->ReleaseSuite(kADOBESDK_ControlSurfaceHostLumetriSuite, kADOBESDK_ControlSurfaceHostLumetriSuite_Version1);
						mControlSurfaceHostLumetriSuite = 0;
					}
				}

				if (mControlSurfaceHostTransportSuite || mControlSurfaceHostCommandSuite)
				{
					mControlSurfaceHostRef = inControlSurfaceHostRef;
						
					if (mControlSurfaceHostCommandSuite)
					{
						uint32_t commandCount = 0;
						mControlSurfaceHostCommandSuite->GetCommandCount(mControlSurfaceHostCommandRef, &commandCount);
						for (uint32_t i = 0; i < commandCount; ++i)
						{
							/* [TODO]
							dvacore::UTF16String commandID;

							ADOBESDK_UTF16Char* commandName = 0;
							uint32_t commandNameLength = 0;

							if (mControlSurfaceHostCommandSuite->GetCommandName(
								mControlSurfaceHostCommandRef, i, 9, &commandName, &commandNameLength) == kSPNoError)
							{
								ADOBESDK_UTF16Char* commandID = 0;
								uint32_t commandIDLength = 0;

								if (mControlSurfaceHostCommandSuite->GetCommandID(
									mControlSurfaceHostCommandRef, i, 1024, &commandID, &commandIDLength) == kSPNoError)
								{
									// DVA_TRACE("YourHardwareControlSurface::SetControlSurfaceHost", 5, "CommandID: " << dvacore::UTF16String(commandID) << ", commandName: " << dvacore::UTF16String(commandName));
									mCommandIDToNameMap.insert(std::make_pair(dvacore::UTF16String(commandID), UnicodeToAscii(commandName)));
									
									// [TODO] mControlSurfaceHostMemorySuite->FreeString(commandID);
								}
								// [TODO] mControlSurfaceHostMemorySuite->FreeString(commandName);
							}
							*/
						}			
					}
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////////
	///
	SPErr YourHardwareControlSurface::SetTimeDisplay(uint32_t inTimeDisplayMode, const ADOBESDK_UTF16Char* inDisplayString)
	{
/*		const dvacore::UTF16Char* newTimeDisplayString = reinterpret_cast<const dvacore::UTF16Char*>(inDisplayString);
		if (mLastTimeDisplayString != newTimeDisplayString)
		{
		
			std::string displayString8 = UnicodeToAscii(inDisplayString);
			mYourHardwareIO->SendCurrentTimeString(displayString8);
		
			mLastTimeDisplayString = newTimeDisplayString;
		}
*/		return kSPNoError;
	}

	//////////////////////////////////////////////////////////////////////////////
	///
	SPErr YourHardwareControlSurface::Update()
	{
		SPErr err = 0;

		mUpdateCounter++;

		// Play with volume of master audio channel
		
		ADOBESDK_ControlSurfaceChannelID masterChannelID = 0;
		float volume = 0;
		err = mControlSurfaceHostMixerSuite->GetMasterChannelID(mControlSurfaceHostMixerRef, &masterChannelID);
		if (!err && masterChannelID > 0)
		{
			err = mControlSurfaceHostMixerSuite->GetChannelFaderValue(mControlSurfaceHostMixerRef, masterChannelID, &volume);
			volume -= 0.05f;
			if (volume < -30)
			{
				volume = 0;
			}

			// Getting is fine, but setting we can run into trouble when switching sequences.  Let us know if this is an issue for you.
			// err = mControlSurfaceHostMixerSuite->SetChannelFaderValue(mControlSurfaceHostMixerRef, masterChannelID, volume);
		}
		
		return kSPNoError;
	}

	//////////////////////////////////////////////////////////////////////////////
	///
	SPErr YourHardwareControlSurface::SetPlayState(bool inValue)
	{
		return kSPNoError;
	}

	//////////////////////////////////////////////////////////////////////////////
	///
	SPErr YourHardwareControlSurface::SetRecordState(bool inValue)
	{
		return kSPNoError;
	}

	//////////////////////////////////////////////////////////////////////////////
	///
	SPErr YourHardwareControlSurface::SetForwardState(bool inValue)
	{
		return kSPNoError;
	}

	//////////////////////////////////////////////////////////////////////////////
	///
	SPErr YourHardwareControlSurface::SetRewindState(bool inValue)
	{
		return kSPNoError;
	}

	//////////////////////////////////////////////////////////////////////////////
	///
	SPErr YourHardwareControlSurface::GetUserButtonCount(uint32_t* outButtonCount) const
	{
		if (outButtonCount)
		{
			return kSPNoError;
		}
		return kSPBadParameterError;
	}

	//////////////////////////////////////////////////////////////////////////////
	///
	SPErr YourHardwareControlSurface::GetUserButtonIDAtIndex(
		uint32_t inIndex,
		ADOBESDK_UTF16Char** outButtonID,
		uint32_t* outButtonIDLength) const
	{
		return kSPNoError;
	}

	//////////////////////////////////////////////////////////////////////////////
	///
	SPErr YourHardwareControlSurface::GetUserButtonDisplayName(
		const ADOBESDK_UTF16Char* inButtonID,
		ADOBESDK_UTF16Char** outDisplayName,
		uint32_t* outDisplayNameLength) const
	{
		if (outDisplayName && outDisplayNameLength)
		{
/*			if (mYourHardwareIO)
			{
				dvacore::StdString buttonID(UnicodeToAscii(inButtonID));
			
				const ButtonDescriptorVec& buttons = mYourHardwareIO->GetButtons();
				
				// Sequential search for now - @todo

				for (size_t i = 0; i < buttons.size(); ++i)
				{
					if (buttons[i].mUniqueButtonID == buttonID)
					{
						ADOBESDK_UTF16Char* result = 0;
						dvacore::UTF16String buttonDisplayName(dvacore::utility::AsciiToUTF16(buttons[i].mDisplayName.c_str()));
						if (buttonDisplayName.length())
						{
							//[TODO] if (mControlSurfaceHostMemorySuite->AllocateString(buttonDisplayName.length() + 1, &result) == kSPNoError && result != 0)
							//{
							//	memcpy(result, buttonDisplayName.c_str(), buttonDisplayName.length() * sizeof(ADOBESDK_UTF16Char));
							//	result[buttonDisplayName.length()] = 0;
							//	*outDisplayNameLength = static_cast<uint32_t>(buttonDisplayName.length());
							//}
						}
						*outDisplayName = result;
						return kSPNoError;
					}
				}
				
				return kSPBadParameterError;
			}
			return kSPLogicError;
*/		}
		return kSPBadParameterError;
	}

	//////////////////////////////////////////////////////////////////////////////
	///
	SPErr YourHardwareControlSurface::SetUserButtonCommandID(const ADOBESDK_UTF16Char* inButtonID, const ADOBESDK_UTF16Char* inCommandID)
	{
		if (inButtonID)
		{
/*			dvacore::StdString buttonID(UnicodeToAscii(inButtonID));
			
			ButtonToCommandMap::iterator iter = mButtonToCommandMap.find(buttonID);
			if (iter != mButtonToCommandMap.end())
			{
				mButtonToCommandMap.erase(iter);
			}
			if (inCommandID)
			{
				mButtonToCommandMap.insert(std::make_pair(
					buttonID,
					dvacore::UTF16String(reinterpret_cast<const dvacore::UTF16Char*>(inCommandID))));
			}
			UserButtonCommandIDChanged(buttonID);
*/			return kSPNoError;
		}
		
		return kSPBadParameterError;
	}
	
	//////////////////////////////////////////////////////////////////////////////
	///
	SPErr YourHardwareControlSurface::GetUserButtonCommandID(const ADOBESDK_UTF16Char* inButtonID, ADOBESDK_UTF16Char** outCommandID, uint32_t* outCommandIDLength) const
	{
		if (inButtonID && outCommandID && outCommandIDLength)
		{
/*			dvacore::StdString buttonID(UnicodeToAscii(inButtonID));
			
			ButtonToCommandMap::const_iterator iter = mButtonToCommandMap.find(buttonID);
			if (iter != mButtonToCommandMap.end())
			{
				if (mControlSurfaceHostMemorySuite->AllocateString(iter->second.length() + 1, outCommandID) == kSPNoError)
				{
					memcpy(*outCommandID, iter->second.c_str(), iter->second.length() * sizeof(ADOBESDK_UTF16Char));
					(*outCommandID)[iter->second.length()] = 0;
					*outCommandIDLength = static_cast<uint32_t>(iter->second.length());
				}
				else
				{
					return kSPOutOfMemoryError;
				}
			}
			else
			{
				*outCommandID = 0;
				*outCommandIDLength = 0;
			}
		
			return kSPNoError;
*/		}
		return kSPBadParameterError;
	}

	//////////////////////////////////////////////////////////////////////////////
	///
	SPErr YourHardwareControlSurface::ChangeMixerAutomationMode(ADOBESDK_ControlSurfaceChannelID inChannelID, ADOBESDK_ControlSurfaceAutomationMode inAutomationMode) const
	{
		ADOBESDK_ControlSurfaceChannelID masterChannelID = 0;
		mControlSurfaceHostMixerSuite->GetMasterChannelID(mControlSurfaceHostMixerRef, &masterChannelID);
		mControlSurfaceHostMixerSuite->SetAutomationMode(mControlSurfaceHostMixerRef, inChannelID, inAutomationMode);
		return kSPNoError;
	}

	//////////////////////////////////////////////////////////////////////////////
	///
	SPErr YourHardwareControlSurface::ChangeLumetriControls(void) const
	{
		uint32_t	parameterCount = 0,
					outNewMode = 0;
		mControlSurfaceHostLumetriSuite->GetParameterCount(mControlSurfaceHostLumetriRef, &parameterCount);
		mControlSurfaceHostLumetriSuite->SetMode(mControlSurfaceHostLumetriRef, kADOBESDK_ControlSurfaceLumetriPanelMode_Vignette, &outNewMode);
		return kSPNoError;
	}

	//////////////////////////////////////////////////////////////////////////////
	///
	void YourHardwareControlSurface::RegisterSuites(SPSuitesSuite* inSPSuitesSuite, SPSuiteListRef inSuiteList)
	{
		adobesdk::ControlSurfaceBase::RegisterSuite(inSPSuitesSuite, inSuiteList);
		adobesdk::ControlSurfaceCommandBase::RegisterSuite(inSPSuitesSuite, inSuiteList);
		adobesdk::ControlSurfaceTransportBase::RegisterSuite(inSPSuitesSuite, inSuiteList);
		adobesdk::ControlSurfaceMixerBase::RegisterSuite(inSPSuitesSuite, inSuiteList);
		adobesdk::ControlSurfaceLumetriBase::RegisterSuite(inSPSuitesSuite, inSuiteList);
	}

	//////////////////////////////////////////////////////////////////////////////
	///
	ADOBESDK_ControlSurfaceRef YourHardwareControlSurface::GetControlSurfaceRef()
	{
		return reinterpret_cast<ADOBESDK_ControlSurfaceRef>(static_cast<ControlSurfaceBase*>(this));
	}

	//////////////////////////////////////////////////////////////////////////////
	///
	SPErr YourHardwareControlSurface::GetConfigIdentifier(ADOBESDK_UTF16Char** outConfigIdentifier, uint32_t* outConfigIdentiferLength) const
	{
/*		static dvacore::UTF16String controlSurfaceIdentifier = DVA_STR("YourHardwareControlSurface");

		ADOBESDK_UTF16Char* result = 0;
		if (mControlSurfaceHostMemorySuite->AllocateString(controlSurfaceIdentifier.length() + 1, &result) == kSPNoError && result != 0)
		{
			memcpy(result, controlSurfaceIdentifier.c_str(), controlSurfaceIdentifier.length() * sizeof(ADOBESDK_UTF16Char));
			result[controlSurfaceIdentifier.length()] = 0;
			*outConfigIdentiferLength = static_cast<uint32_t>(controlSurfaceIdentifier.length());
		}
		*outConfigIdentifier = result;
*/		return kSPNoError;
	}

	//////////////////////////////////////////////////////////////////////////////
	///
	SPErr YourHardwareControlSurface::GetControlSurfaceFlags(uint32_t* outControlSurfaceFlags) const
	{
		*outControlSurfaceFlags = 0;
		return kSPNoError;
	}

	//////////////////////////////////////////////////////////////////////////////
	///
	SPErr YourHardwareControlSurface::GetTransportRef(ADOBESDK_ControlSurfaceTransportRef* outTransportRef)
	{
		*outTransportRef = reinterpret_cast<ADOBESDK_ControlSurfaceTransportRef>(static_cast<ControlSurfaceTransportBase*>(this));
		return kSPNoError;
	}

	//////////////////////////////////////////////////////////////////////////////
	///
	SPErr YourHardwareControlSurface::GetCommandRef(ADOBESDK_ControlSurfaceCommandRef* outCommandRef)
	{
		*outCommandRef = reinterpret_cast<ADOBESDK_ControlSurfaceCommandRef>(static_cast<ControlSurfaceCommandBase*>(this));
		return kSPNoError;
	}

	//////////////////////////////////////////////////////////////////////////////
	///
	SPErr YourHardwareControlSurface::GetMixerRef(ADOBESDK_ControlSurfaceMixerRef* outMixerRef)
	{
		*outMixerRef = reinterpret_cast<ADOBESDK_ControlSurfaceMixerRef>(static_cast<ControlSurfaceMixerBase*>(this));
		return kSPNoError;
	}

	//////////////////////////////////////////////////////////////////////////////
	///
	void YourHardwareControlSurface::OnHardwareCommandAction(uint32_t inActionID, bool inOn)
	{
/*		mYourHardwareIO->SendHighlight(inActionID, inOn ? 1 : 0);

		if (mControlSurfaceHostCommandSuite)
		{
			if (inOn)
			{
				const ButtonDescriptorVec& buttons = mYourHardwareIO->GetButtons();
				for (ButtonDescriptorVec::const_iterator iter = buttons.begin(); iter != buttons.end(); ++iter)
				{
					if (iter->mActionID == inActionID)
					{
						ButtonToCommandMap::const_iterator commandIter = mButtonToCommandMap.find(iter->mUniqueButtonID);
						if (commandIter != mButtonToCommandMap.end())
						{
							mControlSurfaceHostCommandSuite->ExecuteCommand(
								mControlSurfaceHostCommandRef,
								reinterpret_cast<ADOBESDK_UTF16Char*>(const_cast<UTF16Char*>(commandIter->second.c_str())),
								commandIter->second.length());
						}
						return;
					}
				}
			}
			
		}

		switch (inActionID)
		{
			case kCommandAction_TransportShuttleLeft:
			{
				if (inOn)
				{
					if (mControlSurfaceHostTransportSuite)
					{
						mControlSurfaceHostTransportSuite->ShuttleLeft(mControlSurfaceHostTransportRef);
					}
				}
				break;
			}

			case kCommandAction_TransportShuttleStop:
			{
				if (inOn)
				{
					if (mControlSurfaceHostTransportSuite)
					{
						mControlSurfaceHostTransportSuite->ShuttleStop(mControlSurfaceHostTransportRef);
					}
				}
				break;
			}
			
			case kCommandAction_TransportShuttleRight:
			{
				if (inOn)
				{
					if (mControlSurfaceHostTransportSuite)
					{
						mControlSurfaceHostTransportSuite->ShuttleRight(mControlSurfaceHostTransportRef);
					}
				}
				break;
			}

			case kCommandAction_TransportPreviousFrame:
			{
				if (inOn)
				{
					if (mControlSurfaceHostTransportSuite)
					{
						mControlSurfaceHostTransportSuite->GotoPrevious(mControlSurfaceHostTransportRef);
					}
				}
				break;
			}

			case kCommandAction_TransportNextFrame:
			{
				if (inOn)
				{
					if (mControlSurfaceHostTransportSuite)
					{
						mControlSurfaceHostTransportSuite->GotoNext(mControlSurfaceHostTransportRef);
					}
				}
				break;
			}
		}
*/		
	}

	//////////////////////////////////////////////////////////////////////////////
	///
	void YourHardwareControlSurface::OnHardwareTransportFunction(int32_t inJog, int32_t inShuttle)
	{
		if (inJog != 0)
		{
			mControlSurfaceHostTransportSuite->JogDial(mControlSurfaceHostTransportRef, static_cast<float>(inJog), 32.0f, kAdobesdk_False);
		}
		else if (inShuttle != 0)
		{
			mControlSurfaceHostTransportSuite->JogDial(mControlSurfaceHostTransportRef, static_cast<float>(inShuttle), 32.0f, kAdobesdk_True);
		}

	}

} // namespace YourHardwareName

