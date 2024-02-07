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


#include "ControlSurfaceMarkerBase.h"

// adobesdk
#include "adobesdk/controlsurface/plugin/ControlSurfaceMarkerSuite.h"

namespace adobesdk
{
	namespace
	{
		//////////////////////////////////////////////////////////////////////////////
		///
		SPErr ControlSurface_MarkerChanged(ADOBESDK_ControlSurfaceMarkerRef inMarkerRef, uint32_t inMarkerIndex)
		{
			reinterpret_cast<ControlSurfaceMarkerBase*>(inMarkerRef)->MarkerChanged(inMarkerIndex);
			return kSPNoError;
		}

		//////////////////////////////////////////////////////////////////////////////
		///
		SPErr ControlSurface_RebuildMarkers(ADOBESDK_ControlSurfaceMarkerRef inMarkerRef)
		{
			reinterpret_cast<ControlSurfaceMarkerBase*>(inMarkerRef)->RebuildMarkers();
			return kSPNoError;
		}

		//////////////////////////////////////////////////////////////////////////////
		///
		ADOBESDK_ControlSurfaceMarkerSuite1 sControlSurfaceMarkerSuite =
		{
			ControlSurface_MarkerChanged,
			ControlSurface_RebuildMarkers
		};
	
	} // namespace

		
	//////////////////////////////////////////////////////////////////////////////
	///
	ControlSurfaceMarkerBase::ControlSurfaceMarkerBase()
	{
	}
	
	//////////////////////////////////////////////////////////////////////////////
	///
	ControlSurfaceMarkerBase::~ControlSurfaceMarkerBase()
	{
	}

	//////////////////////////////////////////////////////////////////////////////
	///
	SPErr ControlSurfaceMarkerBase::MarkerChanged(uint32_t inMarkerIndex)
	{
		return kSPUnimplementedError;
	}
	
	//////////////////////////////////////////////////////////////////////////////
	///
	SPErr ControlSurfaceMarkerBase::RebuildMarkers()
	{
		return kSPUnimplementedError;
	}

	//////////////////////////////////////////////////////////////////////////////
	///
	void ControlSurfaceMarkerBase::RegisterSuite(SPSuitesSuite* inSPSuitesSuite, SPSuiteListRef inSuiteList)
	{
		SPSuiteRef controlSurfaceMarkerSuiteRef = 0;
		inSPSuitesSuite->AddSuite(
			inSuiteList,
			NULL,
			kADOBESDK_ControlSurfaceMarkerSuite,
			kADOBESDK_ControlSurfaceMarkerSuite_Version1,
			kADOBESDK_ControlSurfaceMarkerSuite_Version1,
			&sControlSurfaceMarkerSuite,
			&controlSurfaceMarkerSuiteRef);

	}

} // namespace adobesdk

