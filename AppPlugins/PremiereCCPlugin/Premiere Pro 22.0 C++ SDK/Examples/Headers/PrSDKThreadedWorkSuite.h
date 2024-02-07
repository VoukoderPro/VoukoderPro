/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2006 Adobe Systems Incorporated                       */
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

#ifndef	PRSDKTHREADEDWORKSUITE_H
#define PRSDKTHREADEDWORKSUITE_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKMALERRORS_H
#include "PrSDKMALErrors.h"
#endif

#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif

#define kPrSDKThreadedWorkSuite				"Premiere Threaded Work Suite"
#define kPrSDKThreadedWorkSuiteVersion2		2
#define kPrSDKThreadedWorkSuiteVersion3		3

typedef void (*ThreadedWorkCallback)(void* inInstanceData);

/*
**	For V3, the callback gets the plugin ID and a status value. If the status
**	value is suiteError_InstanceDestroyed, the instance has already been destroyed
**	and the plugin should only perform cleanup related to the queued function (if any),
**	not perform any actual processing.
*/
typedef void (*ThreadedWorkCallbackVersion3)(void* inInstanceData, csSDK_int32 inPluginID, prSuiteError inStatus);
typedef void* ThreadedWorkRegistration;

typedef struct 
{
	/*
	**	Register a callback for future threaded work.
	**
	**	@param	inCallback		The callback function to be called by the host.
	**	@param	inInstanceData	The instance data which is passed back in the callback.
	**	@param	outRegistrationData	A token for identifying this registration.
	**
	**	@return					suiteError_NoError
	*/
	prSuiteError (*RegisterForThreadedWork)(
		ThreadedWorkCallback inCallback,
		void* inInstanceData,
		ThreadedWorkRegistration* outRegistrationData);
	
	/*
	**	Queue the registered threaded work callback for processing on a render thread.
	**	If you queue multiple times, it is possible for multiple threads to call your callback.
	**	If this is a problem, you'll need to handle this on your end.
	**
	**	@param	inRegistrationData	The token returned by RegisterForThreadedWork
	*/
	prSuiteError (*QueueThreadedWork)(
		ThreadedWorkRegistration inRegistrationData);

	/*
	**	Remove a registration for a threaded work function.
	**	Any queued callbacks will still occur, even after the unregistration.
	**	
	**	@param	inRegistrationData	The token returned by RegisterForThreadedWork
	*/
	prSuiteError (*UnregisterForThreadedWork)(
		ThreadedWorkRegistration inRegistrationData);
		
	/*
	**	Register a callback for future threaded work, with the addition that only one thread 
	**  will call the callback at a time. The same Unregister call is used.
	**
	**	@param	inCallback		The callback function to be called by the host.
	**	@param	inInstanceData	The instance data which is passed back in the callback.
	**	@param	outRegistrationData	A token for identifying this registration.
	**
	**	@return					suiteError_NoError
	*/
	prSuiteError (*RegisterForSingleThreadedWork)(
		ThreadedWorkCallback inCallback,
		void* inInstanceData,
		ThreadedWorkRegistration* outRegistrationData);
	
} PrSDKThreadedWorkSuiteVersion2;

/**
**	The following typedef is provided for short-term backwards compatibility only.
**	Clients should prefer the explicit versioned struct name.
*/
typedef PrSDKThreadedWorkSuiteVersion2 PrSDKThreadedWorkSuite;

typedef struct 
{
	/*
	**	Register a callback for future threaded work.
	**
	**	@param	inCallback		The callback function to be called by the host.
	**	@param	inInstanceData	The instance data which is passed back in the callback.
	**	@param	outRegistrationData	A token for identifying this registration.
	**
	**	@return					suiteError_NoError
	*/
	prSuiteError (*RegisterForThreadedWork)(
		ThreadedWorkCallbackVersion3 inCallback,
		void* inInstanceData,
		ThreadedWorkRegistration* outRegistrationData);
	
	/*
	**	Queue the registered threaded work callback for processing on a render thread.
	**	If you queue multiple times, it is possible for multiple threads to call your callback.
	**	If this is a problem, you'll need to handle this on your end.
	**
	**	[IMPORTANT]
	**	In a change from previous versions of this suite, your callback will *NOT* be called
	**	once your plugin ID is invalid. So, for instance, if you are an importer, once your importer
	**	is closed all remaining queued callbacks will not execute. 
	**
	**	@param	inRegistrationData	The token returned by RegisterForThreadedWork
	**	@param	inPluginID			The plugin ID for the calling plugin instance.
	**								If non-zero, a stable instance of the plugin
	**								is guaranteed for the full duration of the callback.
	**								If zero, a stable instance of the plugin is not
	**								guaranteed for the full duration of the callback.
	*/
	prSuiteError (*QueueThreadedWork)(
		ThreadedWorkRegistration inRegistrationData,
		csSDK_int32 inPluginID);

	/*
	**	Remove a registration for a threaded work function.
	**	Any queued callbacks will still occur, even after the unregistration.
	**	
	**	@param	inRegistrationData	The token returned by RegisterForThreadedWork
	*/
	prSuiteError (*UnregisterForThreadedWork)(
		ThreadedWorkRegistration inRegistrationData);
		
	/*
	**	Register a callback for future threaded work, with the addition that only one thread 
	**  will call the callback at a time. The same Unregister call is used.
	**
	**	@param	inCallback		The callback function to be called by the host.
	**	@param	inInstanceData	The instance data which is passed back in the callback.
	**	@param	outRegistrationData	A token for identifying this registration.
	**
	**	@return					suiteError_NoError
	*/
	prSuiteError (*RegisterForSingleThreadedWork)(
		ThreadedWorkCallbackVersion3 inCallback,
		void* inInstanceData,
		ThreadedWorkRegistration* outRegistrationData);
	
} PrSDKThreadedWorkSuiteVersion3;

#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif /* PRSDKTHREADEDWORKSUITE_H */

