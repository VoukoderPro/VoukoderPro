#ifndef VIGNETTE_CU
    #define VIGNETTE_CU

    #include "VignetteGPU.h"

    #if __CUDACC_VER_MAJOR__ >= 9
        #include <cuda_fp16.h>
    #endif

	struct Pixel16 {
		unsigned short x;
		unsigned short y;
		unsigned short z;
		unsigned short w;
	};
	inline __device__ float4 HalfToFloat4 ( Pixel16 in )
	{
		return make_float4 ( __half2float(in.x), __half2float(in.y), __half2float(in.z), __half2float(in.w ) );
	}
	inline __device__ Pixel16 FloatToHalf4 ( float4 in )
	{
		Pixel16 v;
		v.x = __float2half_rn( in.x ); v.y = __float2half_rn( in.y ); v.z = __float2half_rn( in.z ); v.w = __float2half_rn( in.w );
		return v;
	}

	// Vignette CUDA kernel

	__global__ void kVignetteCUDA (
		float4* destImg,
		const int destPitch,
		const int in16f,
		const int inWidth,
		const int inHeight,
		const float	amountF,
		const float	outer_aaF,
		const float	outer_bbF,
		const float	outer_abF,
		const float	inner_aaF,
		const float	inner_bbF,
		const float	inner_aabbF,
		const float	inner_abF,
		float	x_t,
		float	y_t
		)	
	{
		float4 dest;

		int x = blockIdx.x*blockDim.x + threadIdx.x;
		int y = blockIdx.y*blockDim.y + threadIdx.y;

		if ( x >= inWidth || y >= inHeight ) return;

		if ( in16f ) {
			Pixel16*  in16 = (Pixel16*)  destImg;			
			dest =  HalfToFloat4 (  in16[y *  destPitch + x] );
		} else {
			dest =  destImg[y *  destPitch + x];
		}
		
		x_t = x - x_t;
		y_t = y - y_t;

		float f = 0.0f;

		if (x_t != 0 || y_t != 0)
		{

			float xx = x_t * x_t;
			float yy = y_t * y_t;

			if (inner_aaF == outer_aaF)
			{
				f = xx*inner_bbF + yy * inner_aaF < inner_aabbF ? 0.0f : 1.0f;
			}
			else {
				float R = sqrt(xx + yy),
					r_i = inner_abF / sqrt(inner_bbF * xx + inner_aaF * yy) * R,
					r_o = outer_abF / sqrt(outer_bbF * xx + outer_aaF * yy) * R;
				f = min(1.0f, max(0.0f, (R - r_i) / (r_o - r_i)));
			}
		}

		f *= amountF;

		dest.x = max(0.0f,dest.x + f);
		dest.y = max(0.0f, dest.y + f);
		dest.z = max(0.0f, dest.z + f);

		if ( in16f ) {
			Pixel16*  dest16 = (Pixel16*) destImg;
			dest16[y * destPitch + x] = FloatToHalf4 ( dest );
		} else {
			destImg[y * destPitch + x] = dest;
		}
	}
	
	void Vignette_CUDA (
		float *destBuf,
		int destPitch,
		int	is16f,
		int width,
		int height,
		VigInfoGPU *viP )
	{
		dim3 blockDim (16, 16, 1);
		dim3 gridDim ( (width + blockDim.x - 1)/ blockDim.x, (height + blockDim.y - 1) / blockDim.y, 1 );		

		kVignetteCUDA <<< gridDim, blockDim, 0 >>> ( (float4*) destBuf, destPitch, is16f, width, height,  
			viP->amountF,
			viP->outer_aaF,
			viP->outer_bbF,
			viP->outer_abF,
			viP->inner_aaF,
			viP->inner_bbF,
			viP->inner_aabbF,
			viP->inner_abF,
			viP->x_t,
			viP->y_t);

		cudaDeviceSynchronize();
	}

#endif 
