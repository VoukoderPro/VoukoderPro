R"===(
	__kernel void kVignetteCL (
		__global float4* destImg,
		int destPitch,
		int in16f,
		int inWidth,
		int inHeight,
		float	amountF,
		float	outer_aaF,
		float	outer_bbF,
		float	outer_abF,
		float	inner_aaF,
		float	inner_bbF,
		float	inner_aabbF,
		float	inner_abF,
		float	x_t,
		float	y_t
		)	
	{
		float4 dest;

		int x = get_global_id(0);
		int y = get_global_id(1);

		if ( x >= inWidth || y >= inHeight ) return;

		dest = in16f ? vload_half4(y * destPitch + x, (const __global half*) destImg) : destImg[y *  destPitch + x];
		
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

		dest.x = max(0.0f, dest.x + f);
		dest.y = max(0.0f, dest.y + f);
		dest.z = max(0.0f, dest.z + f);

		if ( in16f ) {
			vstore_half4_rtz(dest, y * destPitch + x, (__global half*) destImg);
		} else {
			destImg[y * destPitch + x] = dest;
		}
	}
)==="