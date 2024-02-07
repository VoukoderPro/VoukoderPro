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
	prSDKEntry.h
	
	Part of the Adobe After Effects and Premiere SDKs.
	Copyright 2000 Adobe Systems Incorporated.
	All Rights Reserved.

	Revision History
		1.0				created						eks			10/1/99
		1.1				added PREMPLUGENTRY			bbb			12/13/99
		
*/

#ifndef PRSDKENTRY_H
#define PRSDKENTRY_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef DllExport
#if defined(__GNUC__)
#define DllExport		__attribute__((visibility("default")))
#elif	MAC_ENV // Note: not appropriate for Xcode/GCC on OS X.
#define	DllExport
#else
#define DllExport   __declspec( dllexport )
#endif
#endif

#ifndef PREFFECTENTRY
#define PREFFECTENTRY csSDK_int16
#endif
#ifndef PRMEXPORT
#define PRMEXPORT __cdecl
#endif
#ifndef PRDEVICEENTRY
#define PRDEVICEENTRY csSDK_int16
#endif
#ifndef PREMPLUGENTRY
#define PREMPLUGENTRY csSDK_int32
#endif

#endif // PRSDKENTRY_H
