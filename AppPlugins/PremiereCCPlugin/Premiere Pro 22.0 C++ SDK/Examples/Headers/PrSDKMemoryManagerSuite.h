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

#ifndef	PRSDKMEMORYMANAGERSUITE_H
#define PRSDKMEMORYMANAGERSUITE_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKPIXELFORMAT_H
#include "PrSDKPixelFormat.h"
#endif 

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

#define kPrSDKMemoryManagerSuite			"Premiere Memory Manager Suite"
#define kPrSDKMemoryManagerSuiteVersion1		1
#define kPrSDKMemoryManagerSuiteVersion2		2
#define kPrSDKMemoryManagerSuiteVersion3		3
#define kPrSDKMemoryManagerSuiteVersion4		4
#define kPrSDKMemoryManagerSuiteVersion			kPrSDKMemoryManagerSuiteVersion4

typedef struct 
{
	/**
	**	Set the memory reserve size in bytes for the plugin with the specified ID.
	**
	**	@param	inPluginID			The ID of the plugin. 
	**	@param	inSize				The size in bytes to reserve.
	*/
	prSuiteError (*ReserveMemory)(
		csSDK_uint32 inPluginID,
		csSDK_uint32 inSize);

} PrSDKMemoryManagerSuiteV1;


typedef struct 
{
	/**
	**	Set the memory reserve size in bytes for the plugin with the specified ID.
	**
	**	@param	inPluginID			The ID of the plugin. 
	**	@param	inSize				The size in bytes to reserve.
	*/
	prSuiteError (*ReserveMemory)(
		csSDK_uint32 inPluginID,
		csSDK_uint32 inSize);

	PrMemoryPtr (*NewPtrClear)(
		csSDK_uint32 byteCount);

	PrMemoryPtr (*NewPtr)(
		csSDK_uint32 byteCount);

	csSDK_uint32 (*GetPtrSize)(
		PrMemoryPtr p);

	void (*SetPtrSize)(
		PrMemoryPtr *p, 
		csSDK_uint32 newSize);

	PrMemoryHandle (*NewHandle)(
		csSDK_uint32 byteCount);

	PrMemoryHandle (*NewHandleClear)(
		csSDK_uint32 byteCount);

	void (*PrDisposePtr)(
		PrMemoryPtr p);

	void (*DisposeHandle)(
		PrMemoryHandle h);

	short (*SetHandleSize)(
		PrMemoryHandle h, 
		csSDK_uint32 newSize);

	csSDK_uint32 (*GetHandleSize)(
		PrMemoryHandle h);

	void (*HUnlock)(
		PrMemoryHandle h);

	void (*HLock)(
		PrMemoryHandle h);

} PrSDKMemoryManagerSuiteV2;

typedef void (*PrSDKMemoryManagerSuite_PurgeMemoryFunction)(
	void* inPurgeMemoryData,
	csSDK_uint32 inMemoryID);

typedef struct 
{
	/**
	**	Set the memory reserve size in bytes for the plugin with the specified ID.
	**
	**	@param	inPluginID			The ID of the plugin. 
	**	@param	inSize				The size in bytes to reserve.
	*/
	prSuiteError (*ReserveMemory)(
		csSDK_uint32 inPluginID,
		csSDK_uint32 inSize);
	
	/**
	**	Get the current size of the media cache in this process.
	**
	**	@param	outMemoryManagerSize	The size of the memory manager in bytes.
	*/
	prSuiteError (*GetMemoryManagerSize)(
		csSDK_uint64* outMemoryManagerSize);
	
	/**
	**	Add a block of memory to management. This block should not be something entered
	**	in any of the suite cache, and it must be purgeable. The purge function you pass in
	**	may be called on any thread.
	**
	**	@param	inSize				The size in bytes of the item in question.
	**	@param	inPurgeFunction		The function pointer that will be called to purge the item.
	**	@param	inPurgeMemoryData	The data object passed back in the purge callback.
	**	@param	outID				The id the host will use for this item.
	*/
	prSuiteError (*AddBlock)(
		csSDK_size_t inSize,
		PrSDKMemoryManagerSuite_PurgeMemoryFunction inPurgeFunction,
		void* inPurgeMemoryData,
		csSDK_uint32* outID);
	
	/**
	**	Each time you use a block of memory, you should call this function. This pushes its 
	**	priority up in the cache, making a purge less likely.
	**
	**	@param	inID				The id of the block to touch.
	*/
	prSuiteError (*TouchBlock)(
		csSDK_uint32 inID);
	
	/**
	**	You can manually expire an item from the cache with this function. Note that the purge function
	**	on the item will not be called.
	**
	**	@param	inID				The id of the block to touch.
	*/
	prSuiteError (*RemoveBlock)(
		csSDK_uint32 inID);

	PrMemoryPtr (*NewPtrClear)(
		csSDK_uint32 byteCount);

	PrMemoryPtr (*NewPtr)(
		csSDK_uint32 byteCount);

	csSDK_uint32 (*GetPtrSize)(
		PrMemoryPtr p);

	void (*SetPtrSize)(
		PrMemoryPtr *p, 
		csSDK_uint32 newSize);

	PrMemoryHandle (*NewHandle)(
		csSDK_uint32 byteCount);

	PrMemoryHandle (*NewHandleClear)(
		csSDK_uint32 byteCount);

	void (*PrDisposePtr)(
		PrMemoryPtr p);

	void (*DisposeHandle)(
		PrMemoryHandle h);

	short (*SetHandleSize)(
		PrMemoryHandle h, 
		csSDK_uint32 newSize);

	csSDK_uint32 (*GetHandleSize)(
		PrMemoryHandle h);

	prSuiteError (*AdjustReservedMemorySize)(
		csSDK_uint32 inPluginID,
		csSDK_int64 inSize);

} PrSDKMemoryManagerSuite;

#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif
