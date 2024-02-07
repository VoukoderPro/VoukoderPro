/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2003 Adobe Systems Incorporated                       */
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

#ifndef	PRSDKDEFERREDPROCESSINGSUITE_H
#define PRSDKDEFERREDPROCESSINGSUITE_H

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

#define kPrSDKDeferredProcessingSuite				"Premiere Deferred Processing Suite"
#define kPrSDKDeferredProcessingSuiteVersion		1

typedef struct 
{
	/*
	**	Tell Premiere that you would like to schedule some processing time.
	**	@param	inPluginID		The ID of the plugin requesting some processing time.
	**	@param	inStreamIndex	The stream index. This is only used by importers so that the correct
	**							importer plugin gets called.
	**	@param	inDelay			The minimum amount of time in milliseconds that Premiere should wait,
	**							before calling the plugin.
	**
	**	@return					suiteError_NoError
	*/
	prSuiteError (*QueueDeferredProcessing)(
		csSDK_uint32 inPluginID,
		csSDK_int32 inStreamIndex,
		csSDK_int32 inDelay);
	
	/**
	**	An importer calls this function to set whether or not it is currently in a pending state.
	**
	**	@param	inPluginID		The ID of the plugin.
	**	@param	inIsPending		If true, the importer is pending.
	*/	
	prSuiteError (*SetPendingState)(
		csSDK_uint32 inPluginID,
		bool inIsPending);
		
} PrSDKDeferredProcessingSuite;

#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif /* PRSDKDEFERREDPROCESSINGSUITE_H */

