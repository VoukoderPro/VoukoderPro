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


#include "SDK_ProcAmp.h"
#include "PrGPUFilterModule.h"

#if _WIN32
    #include "SDK_ProcAmp.cl.h"
    #include <CL/cl.h>
    #define HAS_METAL 0
#else
    #include <OpenCL/cl.h>
    #define HAS_METAL 1
    #include <Metal/Metal.h>
#endif
#include <math.h>

/*
**The ProcAmp2Params structure mirrors that used by the metal kernel.
*/
typedef struct
{
	int mPitch;
	int m16f;
	int mWidth;
	int mHeight;
	float mBrightness;
	float mContrast;
	float mHueCosSaturation;
	float mHueSinSaturation;
} ProcAmp2Params;


#if HAS_METAL

prSuiteError CheckForMetalError(NSError *inError)
{
	if (inError)
	{
		//char const * errorDescription = [[inError localizedDescription] cStringUsingEncoding:NSASCIIStringEncoding];
		return suiteError_Fail;  //For debugging, uncomment above line and set breakpoint here
	}
	return suiteError_NoError;
}

size_t DivideRoundUp(
	size_t inValue,
	size_t inMultiple)
{
	return inValue ? (inValue + inMultiple - 1) / inMultiple: 0;
}
#endif //HAS_METAL

/*
**
*/
enum {kMaxDevices = 12};
static cl_kernel sKernelCache[kMaxDevices] = {};
#if HAS_METAL
static id<MTLComputePipelineState> sMetalPipelineStateCache[kMaxDevices] = {};
#endif
/*
**
*/
class ProcAmp2 :
	public PrGPUFilterBase
{
public:
	virtual prSuiteError Initialize(
		PrGPUFilterInstance* ioInstanceData)
	{
		PrGPUFilterBase::Initialize(ioInstanceData);

		if (mDeviceIndex > kMaxDevices)
		{
			// Exceeded max device count
			return suiteError_Fail;
		}

		// Load and compile the kernel - a real plugin would cache binaries to disk
		if (mDeviceInfo.outDeviceFramework == PrGPUDeviceFramework_OpenCL)
		{
            #if _WIN32
			mKernelOpenCL = sKernelCache[mDeviceIndex];
			if (!mKernelOpenCL)
			{
				cl_int result = CL_SUCCESS;
				//A real plugin would check to see if 16f access is actually supported.
				char const *k16fString = "#define GF_OPENCL_SUPPORTS_16F 1\n";
				size_t sizes[] = { strlen(k16fString), strlen(kSDK_ProcAmp_OpenCLString) };
				char const *strings[] = { k16fString, kSDK_ProcAmp_OpenCLString };
				cl_context context = (cl_context)mDeviceInfo.outContextHandle;
				cl_device_id device = (cl_device_id)mDeviceInfo.outDeviceHandle;
				cl_program program = clCreateProgramWithSource(context, 2, &strings[0], &sizes[0], &result);
				if (result != CL_SUCCESS)
				{
					return suiteError_Fail;
				}

				result = clBuildProgram(program, 1, &device, "-cl-single-precision-constant -cl-fast-relaxed-math", 0, 0);
				if (result != CL_SUCCESS)
				{
					return suiteError_Fail;
				}

				mKernelOpenCL = clCreateKernel(program, "ProcAmp2Kernel", &result);
				if (result != CL_SUCCESS)
				{
					return suiteError_Fail;
				}

				sKernelCache[mDeviceIndex] = mKernelOpenCL;
			}
			return suiteError_NoError;
            #else
            return suiteError_NotImplemented;
            #endif //#if _WIN32
		}
#if HAS_METAL
		else if (mDeviceInfo.outDeviceFramework == PrGPUDeviceFramework_Metal)
		{
			//A real plugin would check version here - the PremierePro 10.3.0 release has Metal, but does not support Metal plugins.
            if (mDeviceIndex > sizeof(sMetalPipelineStateCache) / sizeof(id<MTLComputePipelineState>))
            {
                return suiteError_Fail;        // Exceeded max device count
            }
            
			if (!sMetalPipelineStateCache[mDeviceIndex])
			@autoreleasepool{
				prSuiteError result = suiteError_NoError;

                NSString *pluginBundlePath = [[NSBundle bundleWithIdentifier:@"MyCompany.SDK-ProcAmp"] bundlePath];
                NSString *metalLibPath = [pluginBundlePath stringByAppendingPathComponent:@"Contents/Resources/MetalLib/SDK_ProcAmp.metalLib"];
                if(!(metalLibPath && [[NSFileManager defaultManager] fileExistsAtPath:metalLibPath]))
                {
                    // Metallib file missing in path
                    return suiteError_Fail;
                }
                
				//Create a library from source
				NSError *error = nil;
				id<MTLDevice> device = (id<MTLDevice>)mDeviceInfo.outDeviceHandle;
                id<MTLLibrary> library = [[device newLibraryWithFile:metalLibPath error:&error] autorelease];
				result = CheckForMetalError(error);

				//Create pipeline state from function extracted from library
				if (result == suiteError_NoError)
				{
					id<MTLFunction> function = nil;
					NSString *name = [NSString stringWithCString:"ProcAmp2Kernel" encoding:NSUTF8StringEncoding];
					function = [[library newFunctionWithName:name] autorelease];
					
                    sMetalPipelineStateCache[mDeviceIndex] = [device newComputePipelineStateWithFunction:function error:&error];
					result = CheckForMetalError(error);
				}
				
				return result;
			}
			return suiteError_NoError;
		}
#endif //HAS_METAL

		// Sample code is only accelerated with OpenCL & Metal
		return suiteError_Fail;
	}

	prSuiteError Render(
		const PrGPUFilterRenderParams* inRenderParams,
		const PPixHand* inFrames,
		csSDK_size_t inFrameCount,
		PPixHand* outFrame)
	{
		// read the parameters
		ProcAmp2Params params;
		params.mBrightness = GetParam(SDK_PROCAMP_BRIGHTNESS, inRenderParams->inClipTime).mFloat32 / 100.0f;
		params.mContrast = GetParam(SDK_PROCAMP_CONTRAST, inRenderParams->inClipTime).mFloat32 / 100.0f;
		
		float hue = GetParam(SDK_PROCAMP_HUE, inRenderParams->inClipTime).mFloat32;
		float saturation = GetParam(SDK_PROCAMP_SATURATION, inRenderParams->inClipTime).mFloat32 / 100.0f;
		float hueRadians = float(M_PI / 180) * hue;
		params.mHueCosSaturation = cos(hueRadians) * saturation;
		params.mHueSinSaturation = sin(hueRadians) * saturation;

		void* frameData = 0;
		mGPUDeviceSuite->GetGPUPPixData(*outFrame, &frameData);

		PrPixelFormat pixelFormat = PrPixelFormat_Invalid;
		mPPixSuite->GetPixelFormat(*outFrame, &pixelFormat);

		prRect bounds = {};
		mPPixSuite->GetBounds(*outFrame, &bounds);
		params.mWidth = bounds.right - bounds.left;
		params.mHeight = bounds.bottom - bounds.top;

		csSDK_int32 rowBytes = 0;
		mPPixSuite->GetRowBytes(*outFrame, &rowBytes);
		params.mPitch = rowBytes / GetGPUBytesPerPixel(pixelFormat);
		params.m16f = pixelFormat != PrPixelFormat_GPU_BGRA_4444_32f;

		if (mDeviceInfo.outDeviceFramework == PrGPUDeviceFramework_OpenCL)
		{
			cl_mem buffer = (cl_mem)frameData;
			
			// Set the arguments
			clSetKernelArg(mKernelOpenCL, 0, sizeof(cl_mem), &buffer);
			clSetKernelArg(mKernelOpenCL, 1, sizeof(int), &params.mPitch);
			clSetKernelArg(mKernelOpenCL, 2, sizeof(int), &params.m16f);
			clSetKernelArg(mKernelOpenCL, 3, sizeof(int), &params.mWidth);
			clSetKernelArg(mKernelOpenCL, 4, sizeof(int), &params.mHeight);
			clSetKernelArg(mKernelOpenCL, 5, sizeof(float), &params.mBrightness);
			clSetKernelArg(mKernelOpenCL, 6, sizeof(float), &params.mContrast);
			clSetKernelArg(mKernelOpenCL, 7, sizeof(float), &params.mHueCosSaturation);
			clSetKernelArg(mKernelOpenCL, 8, sizeof(float), &params.mHueSinSaturation);

			// Launch the kernel
			size_t threadBlock[2] = { 16, 16 };
			size_t grid[2] = { RoundUp(params.mWidth, threadBlock[0]), RoundUp(params.mHeight, threadBlock[1])};

			cl_int result = clEnqueueNDRangeKernel(
				(cl_command_queue)mDeviceInfo.outCommandQueueHandle,
				mKernelOpenCL,
				2,
				0,
				grid,
				threadBlock,
				0,
				0,
				0);
			return result == CL_SUCCESS ? suiteError_NoError : suiteError_Fail;
		}
#if HAS_METAL
		else if (mDeviceInfo.outDeviceFramework == PrGPUDeviceFramework_Metal)
		{
            @autoreleasepool {
                prSuiteError result = suiteError_NoError;
                
                //Set the arguments
                id<MTLDevice> device = (id<MTLDevice>)mDeviceInfo.outDeviceHandle;
                id<MTLBuffer> parameterBuffer = [[device newBufferWithBytes:&params
                    length:sizeof(ProcAmp2Params)
                    options:MTLResourceStorageModeManaged] autorelease];
                
                //Launch the command
                id<MTLCommandQueue> queue = (id<MTLCommandQueue>)mDeviceInfo.outCommandQueueHandle;
                id<MTLCommandBuffer> commandBuffer = [queue commandBuffer];
                id<MTLComputeCommandEncoder> computeEncoder = [commandBuffer computeCommandEncoder];
                id<MTLBuffer> ioBuffer = (id<MTLBuffer>)frameData;
                
                MTLSize threadsPerGroup = {[sMetalPipelineStateCache[mDeviceIndex] threadExecutionWidth], 16, 1};
                MTLSize numThreadgroups = {DivideRoundUp(params.mWidth, threadsPerGroup.width), DivideRoundUp(params.mHeight, threadsPerGroup.height), 1};
                
                [computeEncoder setComputePipelineState:sMetalPipelineStateCache[mDeviceIndex]];
                [computeEncoder setBuffer:ioBuffer offset:0 atIndex:0];
                [computeEncoder setBuffer:parameterBuffer offset:0 atIndex:1];
                [computeEncoder	dispatchThreadgroups:numThreadgroups threadsPerThreadgroup:threadsPerGroup];
                [computeEncoder endEncoding];
                [commandBuffer commit];
                // result = CheckForMetalError([commandBuffer error]);
                
                return result;
            }
		}
#endif //HAS_METAL
		return suiteError_Fail;
	}
	
	static prSuiteError Shutdown(
		piSuitesPtr piSuites,
		csSDK_int32 inIndex)
	{
#if HAS_METAL
        @autoreleasepool
        {
            if(sMetalPipelineStateCache[inIndex])
            {
                [sMetalPipelineStateCache[inIndex] release];
                sMetalPipelineStateCache[inIndex] = nil;
            }
        }
#endif
		return suiteError_NoError;
	}

private:
	cl_kernel mKernelOpenCL;
};


DECLARE_GPUFILTER_ENTRY(PrGPUFilterModule<ProcAmp2>)
