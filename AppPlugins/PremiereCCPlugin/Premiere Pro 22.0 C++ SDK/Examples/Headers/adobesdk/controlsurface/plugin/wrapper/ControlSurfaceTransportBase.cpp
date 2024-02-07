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


#include "ControlSurfaceTransportBase.h"

// adobesdk
#include "adobesdk/controlsurface/plugin/ControlSurfaceTransportSuite.h"

namespace adobesdk
{
	namespace
	{
		//////////////////////////////////////////////////////////////////////////////
		///
		SPErr ControlSurface_SetStopState(ADOBESDK_ControlSurfaceTransportRef inControlSurfaceTransportRef, ADOBESDK_Boolean inState)
		{
			return reinterpret_cast<ControlSurfaceTransportBase*>(inControlSurfaceTransportRef)->SetStopState(!!inState);
		}

		//////////////////////////////////////////////////////////////////////////////
		///
		SPErr ControlSurface_SetPlayState(ADOBESDK_ControlSurfaceTransportRef inControlSurfaceTransportRef, ADOBESDK_Boolean inState)
		{
			return reinterpret_cast<ControlSurfaceTransportBase*>(inControlSurfaceTransportRef)->SetPlayState(!!inState);
		}

		//////////////////////////////////////////////////////////////////////////////
		///
		SPErr ControlSurface_SetRecordState(ADOBESDK_ControlSurfaceTransportRef inControlSurfaceTransportRef, ADOBESDK_Boolean inState)
		{
			return reinterpret_cast<ControlSurfaceTransportBase*>(inControlSurfaceTransportRef)->SetRecordState(!!inState);
		}

		//////////////////////////////////////////////////////////////////////////////
		///
		SPErr ControlSurface_SetRewindState(ADOBESDK_ControlSurfaceTransportRef inControlSurfaceTransportRef, ADOBESDK_Boolean inState)
		{
			return reinterpret_cast<ControlSurfaceTransportBase*>(inControlSurfaceTransportRef)->SetRewindState(!!inState);
		}

		//////////////////////////////////////////////////////////////////////////////
		///
		SPErr ControlSurface_SetForwardState(ADOBESDK_ControlSurfaceTransportRef inControlSurfaceTransportRef, ADOBESDK_Boolean inState)
		{
			return reinterpret_cast<ControlSurfaceTransportBase*>(inControlSurfaceTransportRef)->SetForwardState(!!inState);
		}

		//////////////////////////////////////////////////////////////////////////////
		///
		SPErr ControlSurface_SetCycleState(ADOBESDK_ControlSurfaceTransportRef inControlSurfaceTransportRef, ADOBESDK_Boolean inState)
		{
			return reinterpret_cast<ControlSurfaceTransportBase*>(inControlSurfaceTransportRef)->SetCycleState(!!inState);
		}

		//////////////////////////////////////////////////////////////////////////////
		///
		SPErr ControlSurface_SetPauseState(ADOBESDK_ControlSurfaceTransportRef inControlSurfaceTransportRef, ADOBESDK_Boolean inState)
		{
			return reinterpret_cast<ControlSurfaceTransportBase*>(inControlSurfaceTransportRef)->SetPauseState(!!inState);
		}

		//////////////////////////////////////////////////////////////////////////////
		///
		SPErr ControlSurface_SetTimeDisplay(
			ADOBESDK_ControlSurfaceTransportRef	inControlSurfaceTransportRef,
			uint32_t							inTimeDisplayMode,
			const ADOBESDK_String*				inDisplayString)
		{
			return reinterpret_cast<ControlSurfaceTransportBase*>(inControlSurfaceTransportRef)->SetTimeDisplay(
				inTimeDisplayMode,
				inDisplayString);
		}

		//////////////////////////////////////////////////////////////////////////////
		///
		ADOBESDK_ControlSurfaceTransportSuite1 sControlSurfaceTransportSuite =
		{
			ControlSurface_SetStopState,
			ControlSurface_SetPlayState,
			ControlSurface_SetRecordState,
			ControlSurface_SetRewindState,
			ControlSurface_SetForwardState,
			ControlSurface_SetCycleState,
			ControlSurface_SetPauseState,
			ControlSurface_SetTimeDisplay
		};
	
	} // namespace
		
	//////////////////////////////////////////////////////////////////////////////
	///
	ControlSurfaceTransportBase::ControlSurfaceTransportBase()
	{
	}
	
	//////////////////////////////////////////////////////////////////////////////
	///
	ControlSurfaceTransportBase::~ControlSurfaceTransportBase()
	{
	}

	//////////////////////////////////////////////////////////////////////////////
	///
	SPErr ControlSurfaceTransportBase::SetStopState(bool /* inState */)
	{
		return kSPUnimplementedError;
	}
	
	//////////////////////////////////////////////////////////////////////////////
	///
	SPErr ControlSurfaceTransportBase::SetPlayState(bool /* inState */)
	{
		return kSPUnimplementedError;
	}

	//////////////////////////////////////////////////////////////////////////////
	///
	SPErr ControlSurfaceTransportBase::SetRecordState(bool /* inState */)
	{
		return kSPUnimplementedError;
	}

	//////////////////////////////////////////////////////////////////////////////
	///
	SPErr ControlSurfaceTransportBase::SetRewindState(bool /* inState */)
	{
		return kSPUnimplementedError;
	}

	//////////////////////////////////////////////////////////////////////////////
	///
	SPErr ControlSurfaceTransportBase::SetForwardState(bool /* inState */)
	{
		return kSPUnimplementedError;
	}

	//////////////////////////////////////////////////////////////////////////////
	///
	SPErr ControlSurfaceTransportBase::SetCycleState(bool /* inState */)
	{
		return kSPUnimplementedError;
	}

	//////////////////////////////////////////////////////////////////////////////
	///
	SPErr ControlSurfaceTransportBase::SetPauseState(bool /* inState */)
	{
		return kSPUnimplementedError;
	}

	//////////////////////////////////////////////////////////////////////////////
	///
	SPErr ControlSurfaceTransportBase::SetTimeDisplay(uint32_t /* inTimeDisplayMode */, const ADOBESDK_String* /* inDisplayString */)
	{
		return kSPUnimplementedError;
	}

	//////////////////////////////////////////////////////////////////////////////
	///
	void ControlSurfaceTransportBase::RegisterSuite(SPSuitesSuite* inSPSuitesSuite, SPSuiteListRef inSuiteList)
	{
		SPSuiteRef controlSurfaceTransportSuiteRef = 0;
		inSPSuitesSuite->AddSuite(
			inSuiteList,
			NULL,
			kADOBESDK_ControlSurfaceTransportSuite,
			kADOBESDK_ControlSurfaceTransportSuite_Version1,
			kADOBESDK_ControlSurfaceTransportSuite_Version1,
			&sControlSurfaceTransportSuite,
			&controlSurfaceTransportSuiteRef);
	}

} // namespace adobesdk

