/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2002 Adobe Systems Incorporated                       */
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

/*		
	PrSDKSetEnv.h

	Part of the Adobe Premiere SDK.
*/

#ifndef PRSDKSETENV_H
#define PRSDKSETENV_H

#if defined(__APPLE__)
	#include "TargetConditionals.h"	// defines the nesting of TARGET_OS_*
	#if TARGET_OS_MAC
		#if TARGET_OS_IPHONE
			// Older SDKs ( before TV, WATCH ) do not have TARGET_OS_IOS defined.
			#if TARGET_OS_IOS || !defined(TARGET_OS_IOS)
				#undef PRMAC_ENV
				#undef PRWIN_ENV
				#undef PRANDROID_ENV
				#define PRIOS_ENV 1
			#endif
		#else
			 	#define PRMAC_ENV 1
				#undef PRWIN_ENV
				#undef PRIOS_ENV
				#undef PRANDROID_ENV
		#endif // TARGET_OS_IPHONE
	#endif // TARGET_OS_MAC
#endif

#ifndef PRWIN_ENV
	#if defined(_WIN32) || defined(_WIN64)
		#define PRWIN_ENV
		#undef PRMAC_ENV
		#undef PRIOS_ENV
		#undef PRANDROID_ENV
	#endif
#endif

#ifndef PRANDROID_ENV
	#if defined(__ANDROID__)
		#define PRANDROID_ENV 1
		#undef PRWIN_ENV
		#undef PRIOS_ENV
		#undef PRMAC_ENV
	#endif
#endif

#if defined(PRMAC_ENV) || defined(PRIOS_ENV)
#define PRAPPLE_ENV 1
#endif

#ifndef CSSDK_TARGET_ARCH_X64
	#define CSSDK_TARGET_ARCH_X64	0
	#if defined(__x86_64__) || defined(_M_X64) || defined(__arm64__)  || defined(__aarch64__)
		#undef CSSDK_TARGET_ARCH_X64
		#define CSSDK_TARGET_ARCH_X64		1
	#endif
#endif


#endif // PRSDKSETENV_H