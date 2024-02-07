/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2012 Adobe Systems Incorporated                       */
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


#ifndef PRSDKGPUDEVICESUITE_H
#define PRSDKGPUDEVICESUITE_H


#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKMALERRORS_H
#include "PrSDKMALErrors.h"
#endif

#ifndef PRSDKPIXELFORMAT_H
#include "PrSDKPixelFormat.h"
#endif



#pragma pack(push, 1)
#ifdef __cplusplus
extern "C" {
#endif



#define kPrSDKGPUDeviceSuite				"MediaCore GPU Device Suite"
#define kPrSDKGPUDeviceSuiteVersion			2


/**
**
*/
#if defined PRMAC_ENV
#define PrCUDAVersion						9020
#else
#define PrCUDAVersion						10010
#endif
#define PrOpenCLVersion						1000


/**
**	Custom pixel to represent an on device BGRA 16f/32f GPU frame in global memory.
**	All GPU frames may have outstanding asynchronous operations on the compute stream.
**	GPU Frames always have origin top left.
*/
#define PrPixelFormat_GPU_BGRA_4444_16f	MAKE_ADOBE_PRIVATE_PIXEL_FORMAT_FOURCC('C', 'D', 'a')
#define PrPixelFormat_GPU_BGRA_4444_32f	MAKE_ADOBE_PRIVATE_PIXEL_FORMAT_FOURCC('C', 'D', 'A')


/**
**
*/
typedef enum
{
	PrGPUDeviceFramework_CUDA,
	PrGPUDeviceFramework_OpenCL,
	PrGPUDeviceFramework_Metal,
} PrGPUDeviceFramework;

typedef struct
{
	PrGPUDeviceFramework outDeviceFramework;
	prBool outMeetsMinimumRequirementsForAcceleration;
	
	void* outPlatformHandle; // cl_platform_id
	void* outDeviceHandle; // CUdevice or cl_device_id
	void* outContextHandle; // CUcontext or cl_context
	void* outCommandQueueHandle; // CUstream or cl_command_queue
	void* outOffscreenOpenGLContextHandle; // CGLContextObj or HGLRC - only available on the primary device
	void* outOffscreenOpenGLDeviceHandle; // HDC - only available on the primary device

} PrGPUDeviceInfo;

/**
**	Access CUDA or OpenCL devices.
**		All CUDA usage is through the CUDA device API.
*/
typedef struct
{
	/**
	**	Device enumeration
	*/
	prSuiteError (*GetDeviceCount)(
		csSDK_uint32* outDeviceCount);
	prSuiteError (*GetDeviceInfo)(
		csSDK_uint32 inSuiteVersion,
		csSDK_uint32 inDeviceIndex,
		PrGPUDeviceInfo* outDeviceInfo);

	/**
	**	Acquire/release exclusive access to inDeviceIndex. All calls below this point generally require access be held.
	**	For full GPU plugins (those that use a separate entry point for GPU rendering) exclusive access is always held.
	**	These calls do not need to be made in that case.
	**		For CUDA calls cuCtxPushCurrent/cuCtxPopCurrent on the current thread to manage the devices context.
	*/
	prSuiteError (*AcquireExclusiveDeviceAccess)(
		csSDK_uint32 inDeviceIndex);
	prSuiteError (*ReleaseExclusiveDeviceAccess)(
		csSDK_uint32 inDeviceIndex);

	/**
	**	All device memory must be allocated through this suite.
	**		Purge should be called only in emergency situations when working with GPU memory
	**			that cannot be allocated through this suite (eg OpenGL memory).
	**		Returned pointer value represents memory allocated through cuMemAlloc or clCreateBuffer.
	*/
	prSuiteError (*AllocateDeviceMemory)(
		csSDK_uint32 inDeviceIndex,
		size_t inSizeInBytes,
		void** outMemory);
	prSuiteError (*FreeDeviceMemory)(
		csSDK_uint32 inDeviceIndex,
		void* inMemory);
	prSuiteError (*PurgeDeviceMemory)(
		csSDK_uint32 inDeviceIndex,
		size_t inRequestedBytesToPurge,
		size_t* outBytesPurged);

	/**
	**	All host (pinned) memory must be allocated through this suite.
	**		Purge should be called only in emergency situations when working with GPU memory
	**			that cannot be allocated through this suite (eg OpenGL memory).
	**		Returned pointer value represents memory allocated through cuMemHostAlloc or malloc.
	*/
	prSuiteError (*AllocateHostMemory)(
		csSDK_uint32 inDeviceIndex,
		size_t inSizeInBytes,
		void** outMemory);
	prSuiteError (*FreeHostMemory)(
		csSDK_uint32 inDeviceIndex,
		void* inMemory);
	prSuiteError (*PurgeHostMemory)(
		csSDK_uint32 inDeviceIndex,
		size_t inRequestedBytesToPurge,
		size_t* outBytesPurged);

	/**
	**	Information on a GPU ppix. The following ppix functions may also be used:
	**		-PrSDKPPixSuite::Dispose
	**		-PrSDKPPixSuite::GetBounds
	**		-PrSDKPPixSuite::GetRowBytes
	**		-PrSDKPPixSuite::GetPixelAspectRatio
	**		-PrSDKPPixSuite::GetPixelFormat
	**		-PrSDKPPix2Suite::GetFieldOrder
	*/
	prSuiteError (*CreateGPUPPix)(
		csSDK_uint32 inDeviceIndex,
		PrPixelFormat inPixelFormat,
		int inWidth,
		int inHeight,
		int inPARNumerator,
		int inPARDenominator,
		prFieldType inFieldType,
		PPixHand* outPPixHand);
	prSuiteError (*GetGPUPPixData)(
		PPixHand inPPixHand,
		void** outData);
	prSuiteError (*GetGPUPPixDeviceIndex)(
		PPixHand inPPixHand,
		csSDK_uint32* outDeviceIndex);
	prSuiteError (*GetGPUPPixSize)(
		PPixHand inPPixHand,
		size_t* outSize);

} PrSDKGPUDeviceSuite;



#ifdef __cplusplus
}
#endif

#pragma pack(pop)


#endif
