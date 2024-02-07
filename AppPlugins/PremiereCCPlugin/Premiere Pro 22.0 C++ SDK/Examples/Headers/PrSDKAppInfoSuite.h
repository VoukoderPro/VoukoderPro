/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2007 Adobe Systems Incorporated                       */
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

//	This file is part of the public Premiere and After Effects SDKs.


#ifndef	PRSDKAPPINFOSUITE_H
#define PRSDKAPPINFOSUITE_H

#ifndef PRSDKMALERRORS_H
#include "PrSDKMALErrors.h"
#endif

#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif

#define kPrSDKAppInfoSuite				"MediaCore App Info Suite"
#define kPrSDKAppInfoSuiteVersion		3


// Used for kAppInfo_AppFourCC
#define kAppPremierePro			'PPro'
#define kAppPremiereElements	'PrEl'
#define	kAppAfterEffects		'FXTC'
#define kAppMediaEncoder		'AME '
#define kAppCottonwood			'COWO'
#define kAppPrelude				'PRLD'
#define kAppSpeedGrade			'SPGD'
#define kAppAudition			'AdAu'
#define kAppCharacterAnimator	'Anml'
	
// Used for kAppInfo_Version
typedef struct
{
	unsigned int major;
	unsigned int minor;
	unsigned int patch;
} VersionInfo;

// Used for kAppInfo_Language
// A NULL terminated string identifying the Locale used in the host application.
// For ex: "en_US", "ja_JP", "zh_CN". 
typedef struct
{
	char languageID[6];
} LanguageInfo;

typedef struct 
{
// Used for settingsSelector
	enum {
		kAppInfo_AppFourCC, // csSDK_uint32
		kAppInfo_Version, // VersionInfo
		kAppInfo_Build, // csSDK_uint32 - Added in version 2
		kAppInfo_Language, // LanguageInfo - Added in version 3
	};

// GetAppInfo - Get information on the host application.
//		Plug-ins such as importers and compilers are supported in different
//		host applications.  This suite can be used to distinguish between hosts.

	prSuiteError (*GetAppInfo)(
		int settingsSelector,
		void *appInfo);

} PrSDKAppInfoSuite;

#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif /* PRSDKAPPINFOSUITE_H */

