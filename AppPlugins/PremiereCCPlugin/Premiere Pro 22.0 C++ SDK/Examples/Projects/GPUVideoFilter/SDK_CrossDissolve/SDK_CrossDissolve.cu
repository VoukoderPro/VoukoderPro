
#ifndef SDK_CROSS_DISSOLVE
	#define SDK_CROSS_DISSOLVE

    #if __CUDACC_VER_MAJOR__ >= 9
        #include <cuda_fp16.h>
    #endif
	#include "PrGPU/KernelSupport/KernelCore.h" //includes KernelWrapper.h
	#include "PrGPU/KernelSupport/KernelMemory.h"

	#if GF_DEVICE_TARGET_DEVICE
		GF_KERNEL_FUNCTION(kCrossDissolveCUDA,
			((GF_PTR(float4 const))(outImg))		
			((GF_PTR(float4 const))(inImg))
			((GF_PTR(float4))(destImg)),
			((unsigned int)(outPitch))
			((unsigned int)(inPitch))
			((unsigned int)(destPitch))
			((int)(in16f))
			((unsigned int)(inWidth))
			((unsigned int)(inHeight))
			((float)(inProgress))
			((int)(inFlip)),
			((uint2)(inXY)(KERNEL_XY)))
		{
			float4 outgoing, incoming, dest;

			if ( inXY.x >= inWidth || inXY.y >= inHeight ) return;

			outgoing = ReadFloat4(outImg, inXY.y * outPitch + inXY.x, !!in16f);
			incoming = ReadFloat4(inImg, inXY.y * inPitch + inXY.x, !!in16f);
		
			float outgoingAlphaWeighted = outgoing.w * (1.0f - inProgress);
			float incomingAlphaWeighted  = incoming.w * inProgress; 
			float newAlpha = outgoingAlphaWeighted  + incomingAlphaWeighted ; 
			float recipNewAlpha = newAlpha != 0.0f ? 1.0f / newAlpha : 0.0f;

			dest.x = (outgoing.x * outgoingAlphaWeighted + incoming.x * incomingAlphaWeighted) * recipNewAlpha; 
			dest.y = (outgoing.y * outgoingAlphaWeighted + incoming.y * incomingAlphaWeighted) * recipNewAlpha; 
			dest.z = (outgoing.z * outgoingAlphaWeighted + incoming.z * incomingAlphaWeighted) * recipNewAlpha; 
			dest.w = newAlpha;

			WriteFloat4(dest, destImg, inXY.y * outPitch + inXY.x, !!in16f);	
		}
	#endif

	#if __NVCC__
		void CrossDissolve_CUDA (
			float const *outBuf,		
			float const *inBuf,
			float *destBuf,
			unsigned int outPitch,
			unsigned int inPitch,
			unsigned int destPitch,
			int	is16f,
			unsigned int width,
			unsigned int height,
			float progress,
			int flip )
		{
			dim3 blockDim (16, 16, 1);
			dim3 gridDim ( (width + blockDim.x - 1)/ blockDim.x, (height + blockDim.y - 1) / blockDim.y, 1 );		

			kCrossDissolveCUDA <<< gridDim, blockDim, 0 >>> ( (float4 const*) outBuf, (float4 const*) inBuf, (float4*) destBuf, outPitch, inPitch, destPitch, is16f, width, height, progress, flip );

			cudaDeviceSynchronize();
		}
	#endif //GF_DEVICE_TARGET_HOST

#endif //SDK_CROSS_DISSOLVE
