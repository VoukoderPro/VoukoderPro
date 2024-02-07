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


#ifndef PRSDKOPAQUEEFFECTDATASUITE_H
#define PRSDKOPAQUEEFFECTDATASUITE_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef _H_AE_Effect
#include "AE_Effect.h"
#endif


#define kPrSDKOpaqueEffectDataSuite			"Opaque Effect Data Suite"
#define kPrSDKOpaqueEffectDataSuiteVersion2	2
#define kPrSDKOpaqueEffectDataSuiteVersion	kPrSDKOpaqueEffectDataSuiteVersion2

struct OpaqueEffectDataType;

/**
**	This suite provides effects a means to share unflattened sequence data between its instances.
**  The data is opaque to the host and effects are responsible for maintaining thread safety
**  of the shared data.  The host provides ref counting that the effect can use to manage
**  the lifetime of the shared data.
*/
typedef struct
{
	/**
	**	Acquire pointer to opaque effect data.  This is reference counted meaning that 
	**  AcquireOpaqueEffectData and ReleaseOpaqueEffectData should always be called in pairs.
	**  If no opaque effect was registered for the given effect_ref AcquireOpaqueEffectData
	**  will return 0 and the reference count remains 0.
	*/
	SPAPI PF_Err	(*AcquireOpaqueEffectData)(
		csSDK_int32				instanceID,						/* filter instance ID */
		OpaqueEffectDataType	**outOpaqueEffectDataPP);		/* returned EffectDataP to opaque effect data */

	/**
	**	Register opaque effect data.  If multiple threads invoke RegisterOpaqueEffectData
	**  only one will be successful.  The ioOpaqueEffectDataPP of the successful thread
	**  will be returned to all callers.  Calling threads are always responsible for
	**  managing the data they register.  This is the case whether or not threads are
	**  successful registering their data.
	**  Similarly, RegisterOpaqueEffectData always increments the internal reference count.

	//
	// Sample code showing how to use RegisterOpaqueEffectData.
	// Note: code is simplified (not exception-safe, etc.)
	//

	//
	// Try to acquire first, in case another thread registered the opaque effect data earlier
	//
	OpaqueEffectDataType * pData = 0;
	PF_Err err = opaqueEffectDataSuite->AcquireOpaqueEffectData(instanceID, &pData);
	assert(err == PF_Err_NONE);

	//
	// If acquire did not return a valid pointer, create a new object and register it
	// otherwise we are done
	//
	if (pData == 0)
	{
		OpaqueEffectDataType * pNewData(new OpaqueEffectDataType());
		pData = pNewData;
		err = opaqueEffectDataSuite->RegisterOpaqueEffectData(instanceID, &pData);
		assert(err == PF_Err_NONE);
			
		// now we check if this thread actually succeeded registering 
		// if the returned pData is unchanged it means that it was successful
		if (pData != pNewData)
		{
			delete pNewData;
		}
	}

	// pData now points to the right OpaqueEffectDataType object and we can start using it
	...

	**
	**
	**
	*/

	SPAPI PF_Err	(*RegisterOpaqueEffectData)(
		csSDK_int32				instanceID,						/* filter instance ID */
		OpaqueEffectDataType	**ioOpaqueEffectDataPP);		/* new EffectDataP to opaque effect data.  Pointer value might be changed in case another thread calls this function earlier */

	/**
	**	Release opaque effect data.  This decrements the internal reference count.
	**  If the internal reference count goes to 0 outDisposeOpaqueEffectDataPP is set
	**  to the managed data that should be deleted, otherwise it is set to NULL.
	**  If the internal reference count goes to 0 any calls made to AcquireOpaqueEffectData 
	**  will return 0 until new opaque effect data is registered via RegisterOpaqueEffectData.
	*/
	SPAPI PF_Err	(*ReleaseOpaqueEffectData)(
		csSDK_int32				instanceID,							/* filter instance ID */
		OpaqueEffectDataType	**outDisposeOpaqueEffectDataPP);		/* pointer to managed data - nonnull indicates effect should dispose of the data (data is no longer referenced) */
} PrSDKOpaqueEffectDataSuite;

#endif // PRSDKOPAQUEEFFECTDATASUITE_H

