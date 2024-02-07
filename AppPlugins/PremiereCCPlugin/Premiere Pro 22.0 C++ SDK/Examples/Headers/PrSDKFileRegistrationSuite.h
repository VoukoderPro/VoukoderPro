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

#ifndef	PRSDKFILEREGISTRATIONSUITE_H
#define PRSDKFILEREGISTRATIONSUITE_H

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

#define kPrSDKFileRegistrationSuite			"Premiere File Registration Suite"
#define kPrSDKFileRegistrationSuiteVersion	1

typedef struct 
{
	prSuiteError (*RegisterFileForHostManagement)(
		csSDK_uint32 inPluginID,
		char const* inFileDescTag,
		char const* inFilePath,
		char const* const* inExtensions,
		int inNumExtensions,
		csSDK_uint32 inFlags);

	prSuiteError (*GetRegisteredFilePath)(
		csSDK_uint32 inPluginID,
		char const* inFileDescTag,
		char* outFilePath,
		csSDK_uint32 inFilePathLength);

	prSuiteError (*UnregisterHostManagedFile)(
		csSDK_uint32 inPluginID,
		char const* inFileDescTag);

} PrSDKFileRegistrationSuite;

#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif // PRSDKFILEREGISTRATIONSUITE_H

