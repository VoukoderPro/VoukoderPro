/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2000-05 Adobe Systems Incorporated                    */
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
/* Adobe Premiere Utility Callback definitions					   */
/*																   */
/*******************************************************************/

//	This file is part of the public Premiere SDK.

#ifndef PRSDKPLUGUTILITIES_H
#define PRSDKPLUGUTILITIES_H

#pragma once

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifdef PRIOS_ENV
 	#include <CoreFoundation/CoreFoundation.h>
#endif

#include "SPBasic.h"

#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif

// timebase struct for getFileTimebase
typedef struct {
	csSDK_int32	duration;
	csSDK_int32	scale;
	csSDK_int32	sampleSize;
} prTimebaseRec;


// file spec for getFileVideo, getFileAudioSecond and getFileTimebase
typedef struct {
	int		volID;			// used on Mac only
	int		parID;			// used on Mac only
	char	name[256];		// file name on Mac, full path elsewhere
} prFileSpec;

// utility functions for plugins

typedef void (*plugGetSerialNumberFunc)(char *result);   // buffer must be at least 40 bytes long
typedef csSDK_int32 (*plugGetFileTimebaseFunc)(prFileSpec *filespec, csSDK_int32 audioOnly, prTimebaseRec *result);
typedef csSDK_int32 (*plugGetFileVideoFunc)(prFileSpec *filespec, csSDK_int32 frame, PPixHand thePort, prRect *bounds, csSDK_int32 flags);
typedef csSDK_int32 (*plugGetFileAudioSecondFunc)(prFileSpec *filespec, csSDK_int32 num, char *buffer, short flags, csSDK_int32 audrate, short speed);
typedef csSDK_int32 (*plugGetFileVideoBoundsFunc)(prFileSpec *filespec, prRect *bounds);
typedef SPBasicSuite* (*plugGetSPBasicSuiteFunc)();
typedef csSDK_int32 (*plugGetFileExtStringFunc)(csSDK_uint32 fileTypes, char *inBuffer, csSDK_uint32 inBufferSize);

typedef struct
{
	void*						unused1;
	plugGetSerialNumberFunc		getSerialNumber;
	plugGetFileTimebaseFunc		getFileTimebase;
	plugGetFileVideoFunc		getFileVideo;
	plugGetFileAudioSecondFunc	getFileAudioSecond;
	void*						unused2;
	plugGetFileVideoBoundsFunc	getFileVideoBounds;
	plugGetSPBasicSuiteFunc		getSPBasicSuite;
	plugGetFileExtStringFunc	getFileExtString;
} PlugUtilFuncs, *PlugUtilFuncsPtr;


// file 'class' constants for getFileExtString callback
const csSDK_uint32 kFileTypes_Still = 1;
const csSDK_uint32 kFileTypes_AudioOnly = 2;
const csSDK_uint32 kFileTypes_AudioVideo = 3;
const csSDK_uint32 kFileTypes_AllNoIntrinsics = 4; // All importable media types, via importers; no ppj/prproj

#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif /* PRSDKPLUGUTILITIES_H */
