/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2000 Adobe Systems Incorporated                       */
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



//	This file is part of the public Premiere SDK.


#ifndef PRSDKPLUGSUITES_H
#define PRSDKPLUGSUITES_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKPLUGMEMORY_H
#include "PrSDKPlugMemory.h"
#endif

#ifndef PRSDKPLUGWINDOW_H
#include "PrSDKPlugWindow.h"
#endif

#ifndef PRSDKPLUGPPIX_H
#include "PrSDKPlugPPix.h"
#endif

#ifndef PRSDKPLUGUTILITIES_H
#include "PrSDKPlugUtilities.h"
#endif

#ifndef PRSDKPLUGTIMELINE_H
#include "PrSDKPlugTimeline.h"
#endif

#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif

// Premiere plug-in suites

#define PR_PISUITES_VERSION_1		1	// 5.0
#define PR_PISUITES_VERSION_2		2	// 5.1
#define PR_PISUITES_VERSION_3		3	// 6.0
#define PR_PISUITES_VERSION_4		4	// 7.0
#define PR_PISUITES_VERSION_5		5	// Pro 1.5
#define PR_PISUITES_VERSION_6		6	// Pro 1.5.1
#define PR_PISUITES_VERSION_7		7	// Pro 2.0
#define PR_PISUITES_VERSION_8		8	// Pro 3.0
#define PR_PISUITES_VERSION_9		9	// Pro 4.0
#define PR_PISUITES_VERSION			PR_PISUITES_VERSION_9

typedef struct
{
	int						piInterfaceVer;		// version of plug-in interface (PR_PISUITES_VERSION)
	PlugMemoryFuncsPtr		memFuncs;
	PlugWindowFuncsPtr		windFuncs;
	PlugppixFuncsPtr		ppixFuncs;
	PlugUtilFuncsPtr		utilFuncs;
	PlugTimelineFuncsPtr	timelineFuncs;
} piSuites, *piSuitesPtr;

#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif /* PRSDKPLUGSUITES_H */


