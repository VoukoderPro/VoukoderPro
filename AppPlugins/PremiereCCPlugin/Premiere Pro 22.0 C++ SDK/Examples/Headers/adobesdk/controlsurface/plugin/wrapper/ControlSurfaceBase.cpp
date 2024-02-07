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


#include "ControlSurfaceBase.h"

// adobesdk
#include "adobesdk/controlsurface/plugin/ControlSurfaceMarkerSuite.h"

namespace adobesdk
{
	namespace
	{
		//////////////////////////////////////////////////////////////////////////////
		///
		SPErr ControlSurface_GetConfigIdentifier(ADOBESDK_ControlSurfaceRef inControlSurfaceRef, ADOBESDK_String* outIdentifier)
		{
			return reinterpret_cast<const ControlSurfaceBase*>(inControlSurfaceRef)->GetConfigIdentifier(outIdentifier);
		}

		//////////////////////////////////////////////////////////////////////////////
		///
		SPErr ControlSurface_GetControlSurfaceFlags(ADOBESDK_ControlSurfaceRef inControlSurfaceRef, uint32_t* outFlags)
		{
			return reinterpret_cast<const ControlSurfaceBase*>(inControlSurfaceRef)->GetControlSurfaceFlags(outFlags);
		}

		//////////////////////////////////////////////////////////////////////////////
		///
		SPErr  ControlSurface_GetTransportRef(
			ADOBESDK_ControlSurfaceRef				inControlSurfaceRef,
			ADOBESDK_ControlSurfaceTransportRef*	outTransportRef)
		{
			return reinterpret_cast<ControlSurfaceBase*>(inControlSurfaceRef)->GetTransportRef(outTransportRef);
		}
		//////////////////////////////////////////////////////////////////////////////
		///
		SPErr  ControlSurface_GetMixerRef(
			ADOBESDK_ControlSurfaceRef			inControlSurfaceRef,
			ADOBESDK_ControlSurfaceMixerRef*	outMixerRef)
		{
			return reinterpret_cast<ControlSurfaceBase*>(inControlSurfaceRef)->GetMixerRef(outMixerRef);
		}

		//////////////////////////////////////////////////////////////////////////////
		///
		SPErr  ControlSurface_GetMarkerRef(
			ADOBESDK_ControlSurfaceRef			inControlSurfaceRef,
			ADOBESDK_ControlSurfaceMarkerRef*	outMarkerRef)
		{
			return reinterpret_cast<ControlSurfaceBase*>(inControlSurfaceRef)->GetMarkerRef(outMarkerRef);
		}

		//////////////////////////////////////////////////////////////////////////////
		///
		SPErr  ControlSurface_GetCommandRef(
			ADOBESDK_ControlSurfaceRef			inControlSurfaceRef,
			ADOBESDK_ControlSurfaceCommandRef*	outCommandRef)
		{
			return reinterpret_cast<ControlSurfaceBase*>(inControlSurfaceRef)->GetCommandRef(outCommandRef);
		}
		
		//////////////////////////////////////////////////////////////////////////////
		///
		SPErr  ControlSurface_GetLumetriRef(
			ADOBESDK_ControlSurfaceRef			inControlSurfaceRef,
			ADOBESDK_ControlSurfaceLumetriRef*	outLumetriRef)
		{
			return reinterpret_cast<ControlSurfaceBase*>(inControlSurfaceRef)->GetLumetriRef(outLumetriRef);
		}

		//////////////////////////////////////////////////////////////////////////////
		///
		SPErr ControlSurface_Update(ADOBESDK_ControlSurfaceRef inControlSurfaceRef)
		{
			return reinterpret_cast<ControlSurfaceBase*>(inControlSurfaceRef)->Update();
		}

		//////////////////////////////////////////////////////////////////////////////
		///
		ADOBESDK_ControlSurfaceSuite2 sControlSurfaceSuite =
		{
			ControlSurface_GetConfigIdentifier,
			ControlSurface_GetControlSurfaceFlags,
			ControlSurface_GetTransportRef,
			ControlSurface_GetMixerRef,	
			ControlSurface_GetMarkerRef,
			ControlSurface_GetCommandRef,
			ControlSurface_Update,
			ControlSurface_GetLumetriRef
		};
	
	} // namespace

		
	//////////////////////////////////////////////////////////////////////////////
	///
	ControlSurfaceBase::ControlSurfaceBase()
	{
	}
	
	//////////////////////////////////////////////////////////////////////////////
	///
	ControlSurfaceBase::~ControlSurfaceBase()
	{
	}

	//////////////////////////////////////////////////////////////////////////////
	///
	SPErr ControlSurfaceBase::GetConfigIdentifier(ADOBESDK_String* outCongigIdentifier) const
	{
		return kSPUnimplementedError;
	}

	//////////////////////////////////////////////////////////////////////////////
	///
	SPErr ControlSurfaceBase::GetControlSurfaceFlags(uint32_t* outControlSurfaceFlags) const
	{
		*outControlSurfaceFlags = 0;
		return kSPNoError;
	}

	//////////////////////////////////////////////////////////////////////////////
	///
	SPErr ControlSurfaceBase::GetTransportRef(ADOBESDK_ControlSurfaceTransportRef* outTransportRef)
	{
		*outTransportRef = 0;
		return kSPNoError;
	}

	//////////////////////////////////////////////////////////////////////////////
	///
	SPErr ControlSurfaceBase::GetMixerRef(ADOBESDK_ControlSurfaceMixerRef* outMixerRef)
	{
		*outMixerRef = 0;
		return kSPNoError;
	}

	//////////////////////////////////////////////////////////////////////////////
	///
	SPErr ControlSurfaceBase::GetMarkerRef(ADOBESDK_ControlSurfaceMarkerRef* outMarkerRef)
	{
		*outMarkerRef = 0;
		return kSPNoError;
	}

	//////////////////////////////////////////////////////////////////////////////
	///
	SPErr ControlSurfaceBase::GetCommandRef(ADOBESDK_ControlSurfaceCommandRef* outCommandRef)
	{
		*outCommandRef = 0;
		return kSPNoError;
	}

	//////////////////////////////////////////////////////////////////////////////
	///
	SPErr ControlSurfaceBase::GetLumetriRef(ADOBESDK_ControlSurfaceLumetriRef* outLumetriRef)
	{
		*outLumetriRef = 0;
		return kSPNoError;
	}

	//////////////////////////////////////////////////////////////////////////////
	///
	SPErr ControlSurfaceBase::Update()
	{
		return kSPUnimplementedError;
	}

	//////////////////////////////////////////////////////////////////////////////
	///
	void ControlSurfaceBase::RegisterSuite(SPSuitesSuite* inSPSuitesSuite, SPSuiteListRef inSuiteList)
	{
		SPSuiteRef controlSurfaceSuiteRef = 0;
		inSPSuitesSuite->AddSuite(
			inSuiteList,
			NULL,
			kADOBESDK_ControlSurfaceSuite,
			kADOBESDK_ControlSurfaceSuite_Version1,
			kADOBESDK_ControlSurfaceSuite_Version1,
			&sControlSurfaceSuite,
			&controlSurfaceSuiteRef);
		inSPSuitesSuite->AddSuite(
			inSuiteList,
			NULL,
			kADOBESDK_ControlSurfaceSuite,
			kADOBESDK_ControlSurfaceSuite_Version2,
			kADOBESDK_ControlSurfaceSuite_Version2,
			&sControlSurfaceSuite,
			&controlSurfaceSuiteRef);
	}

} // namespace adobesdk

