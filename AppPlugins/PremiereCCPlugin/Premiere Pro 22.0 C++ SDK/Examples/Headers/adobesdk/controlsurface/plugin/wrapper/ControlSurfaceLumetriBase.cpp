/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2016 Adobe Systems Incorporated                       */
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


#include "ControlSurfaceLumetriBase.h"

// adobesdk
#include "adobesdk/controlsurface/plugin/ControlSurfaceLumetriSuite.h"

namespace adobesdk
{
	namespace
	{
		//////////////////////////////////////////////////////////////////////////////
		///
		SPErr ControlSurface_ModeChanged(
			ADOBESDK_ControlSurfaceLumetriRef inLumetriRef,
			uint32_t inMode)
		{
			return reinterpret_cast<ControlSurfaceLumetriBase*>(inLumetriRef)->ModeChanged(inMode);
		}

		//////////////////////////////////////////////////////////////////////////////
		///
		SPErr ControlSurface_ParameterValueChanged(
			ADOBESDK_ControlSurfaceLumetriRef inLumetriRef,
			const ADOBESDK_String* inParameterID,
			float inParameterValue)
		{
			return reinterpret_cast<ControlSurfaceLumetriBase*>(inLumetriRef)->ParameterValueChanged(inParameterID, inParameterValue);
		}

		//////////////////////////////////////////////////////////////////////////////
		///
		SPErr ControlSurface_MenuValueChanged(
			ADOBESDK_ControlSurfaceLumetriRef inLumetriRef,
			const ADOBESDK_String* inMenuID,
			const ADOBESDK_String* inDisplayString)
		{
			return reinterpret_cast<ControlSurfaceLumetriBase*>(inLumetriRef)->MenuValueChanged(inMenuID, inDisplayString);
		}

		//////////////////////////////////////////////////////////////////////////////
		///
		SPErr ControlSurface_AllChanged(
			ADOBESDK_ControlSurfaceLumetriRef inLumetriRef)
		{
			return reinterpret_cast<ControlSurfaceLumetriBase*>(inLumetriRef)->AllChanged();
		}

		//////////////////////////////////////////////////////////////////////////////
		///
		ADOBESDK_ControlSurfaceLumetriSuite1 sControlSurfaceLumetriSuite =
		{
			ControlSurface_ModeChanged,
			ControlSurface_ParameterValueChanged,
			ControlSurface_MenuValueChanged,
			ControlSurface_AllChanged
		};
	
	} // namespace

		
	//////////////////////////////////////////////////////////////////////////////
	///
	ControlSurfaceLumetriBase::ControlSurfaceLumetriBase()
	{
	}
	
	//////////////////////////////////////////////////////////////////////////////
	///
	ControlSurfaceLumetriBase::~ControlSurfaceLumetriBase()
	{
	}

	//////////////////////////////////////////////////////////////////////////////
	///
	SPErr ControlSurfaceLumetriBase::ModeChanged(uint32_t /* inMode */)
	{
		return kSPUnimplementedError;
	}
	
	//////////////////////////////////////////////////////////////////////////////
	///
	SPErr ControlSurfaceLumetriBase::ParameterValueChanged(const ADOBESDK_String* /* inParameterID */, float /* inParameterValue */)
	{
		return kSPUnimplementedError;
	}
	
	//////////////////////////////////////////////////////////////////////////////
	///
	SPErr ControlSurfaceLumetriBase::MenuValueChanged(const ADOBESDK_String* /* inMenuID */, const ADOBESDK_String* /* inDisplayString */)
	{
		return kSPUnimplementedError;
	}

	//////////////////////////////////////////////////////////////////////////////
	///
	SPErr ControlSurfaceLumetriBase::AllChanged()
	{
		return kSPUnimplementedError;
	}

	//////////////////////////////////////////////////////////////////////////////
	///
	void ControlSurfaceLumetriBase::RegisterSuite(SPSuitesSuite* inSPSuitesSuite, SPSuiteListRef inSuiteList)
	{
		SPSuiteRef controlSurfaceLumetriSuiteRef = 0;
		inSPSuitesSuite->AddSuite(
			inSuiteList,
			NULL,
			kADOBESDK_ControlSurfaceLumetriSuite,
			kADOBESDK_ControlSurfaceLumetriSuite_Version1,
			kADOBESDK_ControlSurfaceLumetriSuite_Version1,
			&sControlSurfaceLumetriSuite,
			&controlSurfaceLumetriSuiteRef);
	}

} // namespace adobesdk

