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


#ifndef PRSDKPLUGMEMORY_H
#define PRSDKPLUGMEMORY_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif

// memory functions

typedef char *(*plugNewPtrFunc)(csSDK_uint32 size);
typedef void (*plugSetPtrSizeFunc)(PrMemoryPtr *ptr, csSDK_uint32 newsize);
typedef csSDK_int32 (*plugGetPtrSizeFunc)(char *ptr);
typedef void (*plugDisposePtrFunc)(char *ptr);
typedef char **(*plugNewHandleFunc)(csSDK_uint32 size);
typedef csSDK_int16 (*plugSetHandleSizeFunc)(PrMemoryHandle h, csSDK_uint32 newsize);
typedef csSDK_int32 (*plugGetHandleSizeFunc)(PrMemoryHandle h);
typedef void (*plugDisposeHandleFunc)(PrMemoryHandle h);
typedef char *(*plugNewPtrClearFunc)(csSDK_uint32 size);
typedef char **(*plugNewHandleClearFunc)(csSDK_uint32 size);
typedef void (*plugLockHandleFunc)(PrMemoryHandle h);
typedef void (*plugUnlockHandleFunc)(PrMemoryHandle h);


typedef struct
{
	plugNewPtrFunc				newPtr;
	plugSetPtrSizeFunc			setPtrSize;
	plugGetPtrSizeFunc			getPtrSize;
	plugDisposePtrFunc			disposePtr;
	plugNewHandleFunc			newHandle;
	plugSetHandleSizeFunc		setHandleSize;
	plugGetHandleSizeFunc		getHandleSize;
	plugDisposeHandleFunc		disposeHandle;
	plugNewPtrClearFunc			newPtrClear;
	plugNewHandleClearFunc		newHandleClear;
	plugLockHandleFunc			lockHandle;
	plugUnlockHandleFunc		unlockHandle;
} PlugMemoryFuncs, *PlugMemoryFuncsPtr;

#ifdef __cplusplus
}
#endif
#pragma pack(pop)

#endif /* PRSDKPLUGMEMORY_H */


