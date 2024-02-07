#ifndef SDK_PROC_AMP
#	define SDK_PROC_AMP

#	include "PrGPU/KernelSupport/KernelCore.h" //includes KernelWrapper.h
#	include "PrGPU/KernelSupport/KernelMemory.h"

#	if GF_DEVICE_TARGET_DEVICE
		GF_KERNEL_FUNCTION(ProcAmp2Kernel,
			((GF_PTR(float4))(ioImage)),
			((int)(inPitch))
			((int)(in16f))
			((unsigned int)(inWidth))
			((unsigned int)(inHeight))
			((float)(inBrightness))
			((float)(inContrast))
			((float)(inHueCosSaturation))
			((float)(inHueSinSaturation)),
			((uint2)(inXY)(KERNEL_XY)))
		{
			if (inXY.x < inWidth && inXY.y < inHeight)
			{
				/* We'll operate in-place on 16f/32f data */
				float4 pixel = ReadFloat4(ioImage, inXY.y * inPitch + inXY.x, !!in16f);
		
				/* RGB -> YUV */
				float srcY = pixel.z * 0.299f + pixel.y * 0.587f + pixel.x * 0.114f;
				float srcU = pixel.z * -0.168736f + pixel.y * -0.331264f + pixel.x * 0.5f;
				float srcV = pixel.z * 0.5f + pixel.y * -0.418688f + pixel.x * -0.081312f;
		
				/* Render ProcAmp */
				float destY = (inContrast * srcY) + inBrightness;
				float destU = (srcU * inHueCosSaturation) + (srcV * -inHueSinSaturation);
				float destV = (srcV * inHueCosSaturation) + (srcU *  inHueSinSaturation);
		
				/* YUV -> RGB */
				pixel.z = destY * 1.0f + destU * 0.0f + destV * 1.402f;
				pixel.y = destY * 1.0f + destU * -0.344136f + destV * -0.714136f;
				pixel.x = destY * 1.0f + destU * 1.772f + destV * 0.0f;

				WriteFloat4(pixel, ioImage, inXY.y * inPitch + inXY.x, !!in16f);	
			}
		}
#	endif
#endif
