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


#include "ControlSurfaceCommandBase.h"

// adobesdk
#include "adobesdk/controlsurface/plugin/ControlSurfaceCommandSuite.h"

namespace adobesdk
{
	namespace
	{
		//////////////////////////////////////////////////////////////////////////////
		///
		SPErr ControlSurface_GetUserButtonCount(
			ADOBESDK_ControlSurfaceCommandRef inCommandRef,
			uint32_t* outUserButtonCount)
		{
			return reinterpret_cast<ControlSurfaceCommandBase*>(inCommandRef)->GetUserButtonCount(outUserButtonCount);
		}

		//////////////////////////////////////////////////////////////////////////////
		///
		SPErr ControlSurface_GetUserButtonIDAt(
			ADOBESDK_ControlSurfaceCommandRef inCommandRef,
			uint32_t inIndex,
			ADOBESDK_String* outButtonID)
		{
			return reinterpret_cast<ControlSurfaceCommandBase*>(inCommandRef)->GetUserButtonIDAtIndex(inIndex, outButtonID);
		}

		//////////////////////////////////////////////////////////////////////////////
		///
		SPErr ControlSurface_GetUserButtonDisplayName(
			ADOBESDK_ControlSurfaceCommandRef inCommandRef,
			const ADOBESDK_String* inButtonID,
			ADOBESDK_String* outDisplayName)
		{
			return reinterpret_cast<ControlSurfaceCommandBase*>(inCommandRef)->GetUserButtonDisplayName(inButtonID, outDisplayName);
		}

		//////////////////////////////////////////////////////////////////////////////
		///
		SPErr ControlSurface_SetUserButtonCommandID(
			ADOBESDK_ControlSurfaceCommandRef inCommandRef,
			const ADOBESDK_String* inButtonID,
			const ADOBESDK_String* inContextID,
			const ADOBESDK_String* inCommandID)
		{
			return reinterpret_cast<ControlSurfaceCommandBase*>(inCommandRef)->SetUserButtonCommandID(inButtonID, inContextID, inCommandID);
		}

		//////////////////////////////////////////////////////////////////////////////
		///
		SPErr ControlSurface_GetUserButtonCommandID(
			ADOBESDK_ControlSurfaceCommandRef inCommandRef,
			const ADOBESDK_String* inButtonID,
			ADOBESDK_String* outContextID,
			ADOBESDK_String* outCommandID)
		{
			return reinterpret_cast<ControlSurfaceCommandBase*>(inCommandRef)->GetUserButtonCommandID(inButtonID, outContextID, outCommandID);
		}

		//////////////////////////////////////////////////////////////////////////////
		///
		ADOBESDK_ControlSurfaceCommandSuite1 sControlSurfaceCommandSuite =
		{
			ControlSurface_GetUserButtonCount,
			ControlSurface_GetUserButtonIDAt,
			ControlSurface_GetUserButtonDisplayName,
			ControlSurface_SetUserButtonCommandID,
			ControlSurface_GetUserButtonCommandID
		};
	
	} // namespace

		
	//////////////////////////////////////////////////////////////////////////////
	///
	ControlSurfaceCommandBase::ControlSurfaceCommandBase()
	{
	}
	
	//////////////////////////////////////////////////////////////////////////////
	///
	ControlSurfaceCommandBase::~ControlSurfaceCommandBase()
	{
	}

	//////////////////////////////////////////////////////////////////////////////
	///
	SPErr ControlSurfaceCommandBase::GetUserButtonCount(uint32_t* /* outButtonCount */) const
	{
		return kSPUnimplementedError;
	}
	
	//////////////////////////////////////////////////////////////////////////////
	///
	SPErr ControlSurfaceCommandBase::GetUserButtonIDAtIndex(uint32_t /* inIndex */, ADOBESDK_String* /* outButtonID*/) const
	{
		return kSPUnimplementedError;
	}
	
	//////////////////////////////////////////////////////////////////////////////
	///
	SPErr ControlSurfaceCommandBase::GetUserButtonDisplayName(const ADOBESDK_String* /* inButtonID */, ADOBESDK_String* /* outDisplayName */) const
	{
		return kSPUnimplementedError;
	}

	//////////////////////////////////////////////////////////////////////////////
	///
	SPErr ControlSurfaceCommandBase::SetUserButtonCommandID(const ADOBESDK_String* /* inButtonID */, const ADOBESDK_String* /* inContextID */, const ADOBESDK_String* /* inCommandID */)
	{
		return kSPUnimplementedError;
	}
	
	//////////////////////////////////////////////////////////////////////////////
	///
	SPErr ControlSurfaceCommandBase::GetUserButtonCommandID(const ADOBESDK_String* /* inButtonID */, ADOBESDK_String* /* outContextID */, ADOBESDK_String* /* outCommandID */) const
	{
		return kSPUnimplementedError;
	}

	//////////////////////////////////////////////////////////////////////////////
	///
	void ControlSurfaceCommandBase::RegisterSuite(SPSuitesSuite* inSPSuitesSuite, SPSuiteListRef inSuiteList)
	{
		SPSuiteRef controlSurfaceCommandSuiteRef = 0;
		inSPSuitesSuite->AddSuite(
			inSuiteList,
			NULL,
			kADOBESDK_ControlSurfaceCommandSuite,
			kADOBESDK_ControlSurfaceCommandSuite_Version1,
			kADOBESDK_ControlSurfaceCommandSuite_Version1,
			&sControlSurfaceCommandSuite,
			&controlSurfaceCommandSuiteRef);
	}

} // namespace adobesdk

