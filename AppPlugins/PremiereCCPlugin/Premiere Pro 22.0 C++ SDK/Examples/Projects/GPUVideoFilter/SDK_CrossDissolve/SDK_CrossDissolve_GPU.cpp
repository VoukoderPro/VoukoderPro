/*
** ADOBE CONFIDENTIAL
**
** Copyright 2013 Adobe
** All Rights Reserved.
**
** NOTICE: Adobe permits you to use, modify, and distribute this file in
** accordance with the terms of the Adobe license agreement accompanying
** it. If you have received this file from a source other than Adobe,
** then your use, modification, or distribution of it requires the prior
** written permission of Adobe.
*/

/*
	SDK_CrossDissolve_GPU.cpp
	
	Revision History
		
	Version		Change													Engineer	Date
	=======		======													========	======
	1.0 		Created	with OpenCL render path							shoeg		8/5/2013
	1.1			Work around a crasher in CC								zlam		9/10/2013
	1.5			Fix SDK sample to handle 16f, added in 7.2				shoeg		4/23/2014
	2.0			Integrating CUDA render path generously provided by		zlam		1/20/2015
				Rama Hoetzlein from nVidia
    2.0.1       Fixed custom build steps for CUDA on Windows            zlam        5/6/2017
*/

#include "SDK_CrossDissolve.h"
#include "PrGPUFilterModule.h"
#include "PrSDKVideoSegmentProperties.h"

#if _WIN32
    #include <CL/cl.h>
    #include "SDK_CrossDissolve.cl.h"
    #include <cuda_runtime.h>

    #define HAS_OPENCL  1
    #define HAS_CUDA    1
    #define HAS_METAL   0
#else
    // macOS
    #include <Metal/Metal.h>
    #define HAS_OPENCL  0
    #define HAS_CUDA    0
    #define HAS_METAL   1
#endif

#include <math.h>

#if HAS_METAL
enum {kMaxDevices = 12};
static id<MTLComputePipelineState> sMetalPipelineStateCache[kMaxDevices] = {};

prSuiteError CheckForMetalError(NSError *inError)
{
    if (inError)
    {
        //char const * errorDescription = [[inError localizedDescription] cStringUsingEncoding:NSASCIIStringEncoding];
        return suiteError_Fail;  //For debugging, uncomment above line and set breakpoint here
    }
    return suiteError_NoError;
}

typedef struct
{
    unsigned int outPitch;
    unsigned int inPitch;
    unsigned int destPitch;
    int    is16f;
    unsigned int width;
    unsigned int height;
    float progress;
    int flip;
} CrossDissolveParams;

size_t DivideRoundUp(
    size_t inValue,
    size_t inMultiple)
{
    return inValue ? (inValue + inMultiple - 1) / inMultiple: 0;
}

#endif // HAS_METAL

//  CUDA KERNEL 
//  * See SDK_CrossDissolve.cu
extern void CrossDissolve_CUDA ( float const *outBuf, float const *inBuf , float *destBuf, unsigned int outPitch, unsigned int inPitch, unsigned int destPitch, int is16f, unsigned int width, unsigned int height, float progress, int flip );

#if HAS_OPENCL
static cl_kernel sKernelCache[4];
#endif
/*
**
*/
class SDK_CrossDissolve :
	public PrGPUFilterBase
{
public:
	prSuiteError InitializeCUDA ()
	{
		// Nothing to do here. CUDA Kernel statically linked
        #if HAS_CUDA
        return suiteError_NoError;
        #else
        return suiteError_Fail;
        #endif //#if HAS_CUDA
    }

	prSuiteError InitializeOpenCL ()
    {
        #if HAS_OPENCL
		if (mDeviceIndex > sizeof(sKernelCache) / sizeof(cl_kernel))  	{			
			return suiteError_Fail;		// Exceeded max device count
		}
        if (!mTransitionSuite)
        {
            // Running in PPro 7.0 in GPU mode, there is a crasher related to GetFrameDependencies
            // (error message: VideoFrameFactory.cpp-510)
            // So only use GPU rendering path in 7.1 and later (when TransitionSuite exists)
			return suiteError_Fail;
        }

		mCommandQueue = (cl_command_queue)mDeviceInfo.outCommandQueueHandle;

		// Load and compile the kernel - a real plugin would cache binaries to disk
		mKernel = sKernelCache[mDeviceIndex];
		if (!mKernel)
		{
			cl_int result = CL_SUCCESS;
			size_t size = strlen(kSDK_CrossDissolve_OpenCLString);
			char const* kKernelStrings = &kSDK_CrossDissolve_OpenCLString[0];
			cl_context context = (cl_context)mDeviceInfo.outContextHandle;
			cl_device_id device = (cl_device_id)mDeviceInfo.outDeviceHandle;
			cl_program program = clCreateProgramWithSource(context, 1, &kKernelStrings, &size, &result);
			if (result != CL_SUCCESS)
			{
				return suiteError_Fail;
			}

			result = clBuildProgram(program, 1, &device, "-cl-single-precision-constant -cl-fast-relaxed-math", 0, 0);
			if (result != CL_SUCCESS)
			{
				return suiteError_Fail;
			}

			mKernel = clCreateKernel(program, "CrossDissolveKernel", &result);
			if (result != CL_SUCCESS)
			{
				return suiteError_Fail;
			}

			sKernelCache[mDeviceIndex] = mKernel;
		}
        return suiteError_NoError;
        #else
        return suiteError_Fail;
        #endif //#if HAS_OPENCL
	}
    
    prSuiteError InitializeMetal ()
    {
        #if HAS_METAL
        if (mDeviceIndex > sizeof(sMetalPipelineStateCache) / sizeof(id<MTLComputePipelineState>))
        {
            return suiteError_Fail;        // Exceeded max device count
        }
        
        if (!sMetalPipelineStateCache[mDeviceIndex])
        {
            @autoreleasepool
            {
                prSuiteError result = suiteError_NoError;
                
                NSString *pluginBundlePath = [[NSBundle bundleWithIdentifier:@"MyCompany.SDK-CrossDissolve"] bundlePath];
                NSString *metalLibPath = [pluginBundlePath stringByAppendingPathComponent:@"Contents/Resources/MetalLib/SDK_CrossDissolve.metalLib"];
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
                    NSString *name = [NSString stringWithCString:"kCrossDissolveCUDA" encoding:NSUTF8StringEncoding];
                    function = [[library newFunctionWithName:name] autorelease];
                    
                    sMetalPipelineStateCache[mDeviceIndex] = [device newComputePipelineStateWithFunction:function error:&error];
                    result = CheckForMetalError(error);
                }
                return result;
            }   // autorelease
        }
        return suiteError_NoError;
        #else
        return suiteError_Fail;
        #endif  // HAS_METAL
    }

	virtual prSuiteError Initialize( PrGPUFilterInstance* ioInstanceData )
	{
		PrGPUFilterBase::Initialize(ioInstanceData);

		if (mDeviceInfo.outDeviceFramework == PrGPUDeviceFramework_CUDA)	
			return InitializeCUDA();			

		if (mDeviceInfo.outDeviceFramework == PrGPUDeviceFramework_OpenCL)
			return InitializeOpenCL();
        
        if (mDeviceInfo.outDeviceFramework == PrGPUDeviceFramework_Metal)
            return InitializeMetal();

		return suiteError_Fail;			// GPUDeviceFramework unknown
	}

	prSuiteError GetFrameDependencies(
		const PrGPUFilterRenderParams* inRenderParams,
		csSDK_int32* ioQueryIndex,
		PrGPUFilterFrameDependency* outFrameRequirements)
	{
		if (!mTransitionSuite)
		{
			outFrameRequirements->outDependencyType = PrGPUDependency_InputFrame;
			outFrameRequirements->outTrackID = GetParam(SDK_CROSSDISSOLVE_TRANSITIONINPUT, inRenderParams->inClipTime).mInt32;
			outFrameRequirements->outSequenceTime = inRenderParams->inSequenceTime;
			return suiteError_NoError;
		}
		else
		{
			return suiteError_NotImplemented;
		}
	}


	prSuiteError Render(
		const PrGPUFilterRenderParams* inRenderParams,
		const PPixHand* inFrames,
		csSDK_size_t inFrameCount,
		PPixHand* outFrame)
	{
		float progress;

		// Initial steps are independent of CUDA and OpenCL

		if (inFrameCount < 2 || (!inFrames[0] && !inFrames[1]))
		{
			return suiteError_Fail;
		}

		// read the parameters
		int flip = GetParam(SDK_CROSSDISSOLVE_FLIP, inRenderParams->inClipTime).mBool;

		if (mTransitionSuite)
		{
			csSDK_int64 duration = 0;
			GetProperty(kVideoSegmentProperty_Transition_TransitionDuration, duration);
			progress = (float)inRenderParams->inClipTime / (float)duration;
		}
		else
		{
			csSDK_int64 duration = 0;
			GetProperty(kVideoSegmentProperty_Effect_EffectDuration, duration);
			progress = (float)inRenderParams->inClipTime / (float)duration;
		}

		PPixHand properties = inFrames[0] ? inFrames[0] : inFrames[1];

		csSDK_uint32 index = 0;
		mGPUDeviceSuite->GetGPUPPixDeviceIndex(properties, &index);

		// Get pixel format
		PrPixelFormat pixelFormat = PrPixelFormat_Invalid;
		mPPixSuite->GetPixelFormat(properties, &pixelFormat);
		int is16f = pixelFormat != PrPixelFormat_GPU_BGRA_4444_32f;

		// Get width & height
		prRect bounds = {};
		mPPixSuite->GetBounds(properties, &bounds);
		int width = bounds.right - bounds.left;
		int height = bounds.bottom - bounds.top;

		csSDK_uint32 parNumerator = 0;
		csSDK_uint32 parDenominator = 0;
		mPPixSuite->GetPixelAspectRatio(properties, &parNumerator, &parDenominator);
		
		prFieldType fieldType = 0;
		mPPix2Suite->GetFieldOrder(properties, &fieldType);

		// Create a frame to process output
		mGPUDeviceSuite->CreateGPUPPix(
			index,
			pixelFormat,
			width,
			height,
			parNumerator,
			parDenominator,
			fieldType,
			outFrame);

		if (!outFrame)
		{
			return suiteError_Fail;
		}

		// Get outgoing data
		void* outgoingFrameData = 0;
		csSDK_int32 outgoingRowBytes = 0;
		if (inFrames[0])
		{
			mGPUDeviceSuite->GetGPUPPixData(inFrames[0], &outgoingFrameData);
			mPPixSuite->GetRowBytes(inFrames[0], &outgoingRowBytes);
		}
		int outgoingPitch = outgoingRowBytes / GetGPUBytesPerPixel(pixelFormat);

		// Get incoming data
		void* incomingFrameData = 0;
		csSDK_int32 incomingRowBytes = 0;
		if (inFrames[1])
		{
			mGPUDeviceSuite->GetGPUPPixData(inFrames[1], &incomingFrameData);
			mPPixSuite->GetRowBytes(inFrames[1], &incomingRowBytes);
		}

		// Get dest data
		int incomingPitch = incomingRowBytes / GetGPUBytesPerPixel(pixelFormat);
		void* destFrameData = 0;
		csSDK_int32 destRowBytes = 0;
		mGPUDeviceSuite->GetGPUPPixData(*outFrame, &destFrameData);
		mPPixSuite->GetRowBytes(*outFrame, &destRowBytes);
		int destPitch = destRowBytes / GetGPUBytesPerPixel(pixelFormat);

		if (!outgoingFrameData || !incomingFrameData)
		{
			return suiteError_Fail;
		}

		// Start CUDA or OpenCL specific code

		if (mDeviceInfo.outDeviceFramework == PrGPUDeviceFramework_CUDA) {
            #if HAS_CUDA
			// CUDA device pointers
			float* outgoingBuffer = (float*) outgoingFrameData;
			float* incomingBuffer = (float* )incomingFrameData;	
			float* destBuffer = (float*) destFrameData;	

			// Launch CUDA kernel
			CrossDissolve_CUDA ( outgoingBuffer, 
								incomingBuffer, 
								destBuffer, 
								outgoingPitch, 
								incomingPitch, 
								destPitch,
								is16f, 
								width, 
								height, 
								progress, 
								flip );
	
			if ( cudaPeekAtLastError() != cudaSuccess) 			
			{
				return suiteError_Fail;
			}
            #else
            return suiteError_NotImplemented;
            #endif // #if _WIN32
		}
        else if (mDeviceInfo.outDeviceFramework == PrGPUDeviceFramework_OpenCL) {
            #if HAS_OPENCL
			// OpenCL device pointers
			cl_mem outgoingBuffer = (cl_mem)outgoingFrameData;
			cl_mem incomingBuffer = (cl_mem)incomingFrameData;
			cl_mem destBuffer = (cl_mem)destFrameData;

			// Set the arguments
			clSetKernelArg(mKernel, 0, sizeof(cl_mem), &outgoingBuffer);
			clSetKernelArg(mKernel, 1, sizeof(cl_mem), &incomingBuffer);
			clSetKernelArg(mKernel, 2, sizeof(cl_mem), &destBuffer);
			clSetKernelArg(mKernel, 3, sizeof(unsigned int), &outgoingPitch);
			clSetKernelArg(mKernel, 4, sizeof(unsigned int), &incomingPitch);
			clSetKernelArg(mKernel, 5, sizeof(unsigned int), &destPitch);
			clSetKernelArg(mKernel, 6, sizeof(int), &is16f);
			clSetKernelArg(mKernel, 7, sizeof(unsigned int), &width);
			clSetKernelArg(mKernel, 8, sizeof(unsigned int), &height);
			clSetKernelArg(mKernel, 9, sizeof(float), &progress);
			clSetKernelArg(mKernel, 10, sizeof(int), &flip);

			// Launch the kernel
			size_t threadBlock[2] = { 16, 16 };
			size_t grid[2] = { RoundUp(width, threadBlock[0]), RoundUp(height, threadBlock[1] )};

			cl_int result = clEnqueueNDRangeKernel(
				mCommandQueue,
				mKernel,
				2,
				0,
				grid,
				threadBlock,
				0,
				0,
				0);

			if ( result != CL_SUCCESS )	
				return suiteError_Fail;
            #else
            return suiteError_NotImplemented;
            #endif // HAS_OPENCL
		}
        else if (mDeviceInfo.outDeviceFramework == PrGPUDeviceFramework_Metal) {
            #if HAS_METAL
            
            @autoreleasepool{
                // Set params
                CrossDissolveParams params;
                params.outPitch = outgoingPitch;
                params.inPitch = incomingPitch;
                params.destPitch = destPitch;
                params.is16f = is16f;
                params.width = width;
                params.height = height;
                params.progress = progress;
                params.flip = flip;
                
                // framedata
                id<MTLBuffer> outgoingFrameBuffer = (id<MTLBuffer>)outgoingFrameData;
                id<MTLBuffer> incomingFrameBuffer = (id<MTLBuffer>)incomingFrameData;
                id<MTLBuffer> destBuffer = (id<MTLBuffer>)destFrameData;
                
                id<MTLDevice> device = (id<MTLDevice>)mDeviceInfo.outDeviceHandle;
                id<MTLBuffer> parameterBuffer = [[device newBufferWithBytes:&params
                    length:sizeof(CrossDissolveParams)
                    options:MTLResourceStorageModeManaged] autorelease];
                
                id<MTLCommandQueue> queue = (id<MTLCommandQueue>)mDeviceInfo.outCommandQueueHandle;
                id<MTLCommandBuffer> commandBuffer = [queue commandBuffer];
                id<MTLComputeCommandEncoder> computeEncoder = [commandBuffer computeCommandEncoder];
                
                MTLSize threadsPerGroup = {[sMetalPipelineStateCache[mDeviceIndex] threadExecutionWidth], 16, 1};
                MTLSize numThreadgroups = {DivideRoundUp(params.width, threadsPerGroup.width), DivideRoundUp(params.height, threadsPerGroup.height), 1};
                
                // set buffers
                [computeEncoder setComputePipelineState:sMetalPipelineStateCache[mDeviceIndex]];
                [computeEncoder setBuffer:outgoingFrameBuffer offset:0 atIndex:0];
                [computeEncoder setBuffer:incomingFrameBuffer offset:0 atIndex:1];
                [computeEncoder setBuffer:destBuffer offset:0 atIndex:2];
                [computeEncoder setBuffer:parameterBuffer offset:0 atIndex:3];
                
                [computeEncoder dispatchThreadgroups:numThreadgroups threadsPerThreadgroup:threadsPerGroup];
                [computeEncoder endEncoding];
                [commandBuffer commit];
            }   // autoreleasepool
            #else
            return suiteError_NotImplemented;
            #endif
        }
		return suiteError_NoError;
	}

    /*
    **
    */
    static prSuiteError Shutdown(
        piSuitesPtr piSuites,
        csSDK_int32 inIndex)
    {
        #if HAS_METAL
        @autoreleasepool{
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
	// CUDA


	// OpenCL
    #if HAS_OPENCL
	cl_command_queue mCommandQueue;
	cl_kernel mKernel;
    #endif
};


DECLARE_GPUFILTER_ENTRY(PrGPUFilterModule<SDK_CrossDissolve>)
