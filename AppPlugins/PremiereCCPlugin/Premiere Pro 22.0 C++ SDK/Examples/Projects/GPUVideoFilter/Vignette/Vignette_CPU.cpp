/************************************************************************************/
/* The MIT License (MIT)															*/
/* Copyright (c) 2016 Bartlomiej Walczak											*/
/*																					*/
/* Permission is hereby granted, free of charge, to any person obtaining a copy		*/
/* of this software and associated documentation files (the "Software"), to deal	*/
/* in the Software without restriction, including without limitation the rights		*/
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell		*/
/* copies of the Software, and to permit persons to whom the Software is			*/
/* furnished to do so, subject to the following conditions:							*/
/*																					*/
/* The above copyright notice and this permission notice shall be included			*/
/* in all copies or substantial portions of the Software.							*/
/*																					*/
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS			*/
/* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,		*/
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE		*/
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER			*/
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING			*/
/* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS		*/
/* IN THE SOFTWARE.																	*/
/*																					*/
/************************************************************************************/

/*

	Quick Vignette is an effect plugin supporting all typical pixel formats
	and bit depths in After Effects and Premiere Pro, including CUDA/OpenCL 
	acceleration.
	
	For more information see fxphd course 
		SYS204 - Plugin development for Premiere Pro and After Effects
		https://www.fxphd.com/details/?idCourse=526
		
	Revision history:
		date			developer	version		notes
		2016 Apr 11		bwal		1.0			version to be included in the SDK

*/
		
/* Vignette_CPU.h */

#include "Vignette.h"

static PF_FpShort ClipSum32(const PF_FpShort in, const PF_FpLong modF) {
	return static_cast<PF_FpShort>(MAX(0, modF + in));
}

static A_u_char ClipSum8(const A_u_char in, const PF_FpLong modF) {
	return static_cast<A_u_char>(MAX(0, MIN(modF + in, PF_MAX_CHAN8)));
}

static PF_Err 
About (	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	AEGP_SuiteHandler suites(in_data->pica_basicP);
	
	suites.ANSICallbacksSuite1()->sprintf(	out_data->return_msg,
											"%s v%d.%d\r%s",
                                            strName.c_str(),
											PLUGIN_MAJOR_VERSION,
											PLUGIN_MINOR_VERSION,
											strDescription.c_str());
	return PF_Err_NONE;
}

static PF_Err 
GlobalSetup (	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	out_data->my_version = PF_VERSION(	PLUGIN_MAJOR_VERSION, 
										PLUGIN_MINOR_VERSION,
										PLUGIN_BUG_VERSION,
										PLUGIN_STAGE_VERSION,
										PLUGIN_BUILD_VERSION);

	out_data->out_flags |=	PF_OutFlag_PIX_INDEPENDENT |
							PF_OutFlag_USE_OUTPUT_EXTENT;

	out_data->out_flags2 =	PF_OutFlag2_SUPPORTS_SMART_RENDER |
							PF_OutFlag2_FLOAT_COLOR_AWARE |
							PF_OutFlag2_DOESNT_NEED_EMPTY_PIXELS;

	// For Premiere - declare supported pixel formats
	if (in_data->appl_id == 'PrMr') {

		auto pixelFormatSuite =
			AEFX_SuiteScoper<PF_PixelFormatSuite1>(in_data,
												   kPFPixelFormatSuite,
												   kPFPixelFormatSuiteVersion1,
												   out_data);

		//	Add the pixel formats we support in order of preference.
		(*pixelFormatSuite->ClearSupportedPixelFormats)(in_data->effect_ref);
		(*pixelFormatSuite->AddSupportedPixelFormat)(
			in_data->effect_ref,
			PrPixelFormat_VUYA_4444_32f);
		(*pixelFormatSuite->AddSupportedPixelFormat)(
			in_data->effect_ref,
			PrPixelFormat_BGRA_4444_32f);
		(*pixelFormatSuite->AddSupportedPixelFormat)(
			in_data->effect_ref,
			PrPixelFormat_VUYA_4444_8u);
		(*pixelFormatSuite->AddSupportedPixelFormat)(
			in_data->effect_ref,
			PrPixelFormat_BGRA_4444_8u);
	}
	
	return PF_Err_NONE;
}

static PF_Err 
ParamsSetup (	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	PF_Err		err		= PF_Err_NONE;
	PF_ParamDef	def;	

	AEFX_CLR_STRUCT(def);

	PF_ADD_POINT(strCenter_ParamName.c_str(),
				 VIG_CENTER_DFLT,
				 VIG_CENTER_DFLT,
				 FALSE,
				 CENTER_DISK_ID);

	AEFX_CLR_STRUCT(def);

	PF_ADD_FLOAT_SLIDERX(	strAmount_ParamName.c_str(), 
							VIG_AMOUNT_MIN,
							VIG_AMOUNT_MAX,
							VIG_AMOUNT_MIN,
							VIG_AMOUNT_MAX,
							VIG_AMOUNT_DFLT,
							PF_Precision_TENTHS,
							0,
							0,
							AMOUNT_DISK_ID);

	AEFX_CLR_STRUCT(def);

	PF_ADD_FLOAT_SLIDERX(strSize_ParamName.c_str(),
						 VIG_SIZE_RANGE_MIN,
						 VIG_SIZE_RANGE_MAX,
						 VIG_SIZE_MIN,
						 VIG_SIZE_MAX,
						 VIG_SIZE_DFLT,
						 PF_Precision_TENTHS,
						 0,
						 0,
						 SIZE_DISK_ID);

	AEFX_CLR_STRUCT(def);

	PF_ADD_FLOAT_SLIDERX(strRoundness_ParamName.c_str(),
						 VIG_ROUNDNESS_MIN,
						 VIG_ROUNDNESS_MAX,
						 VIG_ROUNDNESS_MIN,
						 VIG_ROUNDNESS_MAX,
						 VIG_ROUNDNESS_DFLT,
						 PF_Precision_TENTHS,
						 0,
						 0,
						 ROUNDNESS_DISK_ID);

	AEFX_CLR_STRUCT(def);

	PF_ADD_FLOAT_SLIDERX(strFeather_ParamName.c_str(),
						 VIG_FEATHER_MIN,
						 VIG_FEATHER_MAX,
						 VIG_FEATHER_MIN,
						 VIG_FEATHER_MAX,
						 VIG_FEATHER_DFLT,
						 PF_Precision_TENTHS,
						 0,
						 0,
						 FEATHER_DISK_ID);

	out_data->num_params = VIG_NUM_PARAMS;

	return err;
}

static PF_FpLong
CalculateBlendingAmount(
	void		*refcon,
	A_long		xL,
	A_long		yL)
{
	A_long	x_tL, y_tL;

	VigInfo	*viP = reinterpret_cast<VigInfo*>(refcon);

	if (viP) {
		x_tL = xL - viP->x_t;
		y_tL = yL - viP->y_t;

		if (x_tL == 0 && y_tL == 0)
		{
			return 0.0;
		}

		if (abs(x_tL) < viP->innerRect_aL &&
			abs(y_tL) < viP->innerRect_bL)
		{
			return 0.0;
		}

		if (viP->outerCheck &&
			(y_tL > viP->outerRect_bL ||
			y_tL < -viP->outerRect_bL ||
			x_tL > viP->outerRect_aL ||
			x_tL < -viP->outerRect_aL))
		{
			return 1.0;
		}

		PF_FpLong xx = x_tL * x_tL;
		PF_FpLong yy = y_tL * y_tL;

		if (xx*viP->innerEllipse_bbF + yy * viP->innerEllipse_aaF < viP->innerEllipse_aabbF) {
			return 0.0;
		}
		else {
			if (xx*viP->outerEllipse_bbF + yy * viP->outerEllipse_aaF >= viP->outerEllipse_aabbF) {
				return 1.0;
			}
			else {
				PF_FpLong R = sqrt(xx + yy),
					r_i = viP->innerEllipse_abF / sqrt(viP->innerEllipse_bbF * xx + viP->innerEllipse_aaF * yy) * R,
					r_o = viP->outerEllipse_abF / sqrt(viP->outerEllipse_bbF * xx + viP->outerEllipse_aaF * yy) * R;
				return (R - r_i) / (r_o - r_i);
			}
		}
	}

	return 0.0;
}

static PF_Err
DrawVignette32(
	void		*refcon,
	A_long		xL,
	A_long		yL,
	PF_PixelFloat	*inP,
	PF_PixelFloat	*outP)
{
	PF_Err		err = PF_Err_NONE;

	VigInfo	*viP = reinterpret_cast<VigInfo*>(refcon);

	if (viP) {
		PF_FpLong amountF = viP->amountF * CalculateBlendingAmount(refcon, xL, yL);

		outP->alpha = inP->alpha;
		outP->red = ClipSum32(inP->red, amountF);
		outP->green = ClipSum32(inP->green, amountF);
		outP->blue = ClipSum32(inP->blue, amountF);
	}

	return err;
}


static PF_Err
DrawVignetteBGRA_32f(
	void			*refcon,
	A_long			xL,
	A_long			yL,
	PF_PixelFloat	*inP,
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;

	PF_Pixel_BGRA_32f *inBGRA_32fP, *outBGRA_32fP;
	inBGRA_32fP = reinterpret_cast<PF_Pixel_BGRA_32f*>(inP);
	outBGRA_32fP = reinterpret_cast<PF_Pixel_BGRA_32f*>(outP);

	VigInfo	*viP = reinterpret_cast<VigInfo*>(refcon);

	if (viP) {
		PF_FpLong amountF = viP->amountF * CalculateBlendingAmount(refcon, xL, yL);

		outBGRA_32fP->alpha = inBGRA_32fP->alpha;
		outBGRA_32fP->red = ClipSum32(inBGRA_32fP->red, amountF);
		outBGRA_32fP->green = ClipSum32(inBGRA_32fP->green, amountF);
		outBGRA_32fP->blue = ClipSum32(inBGRA_32fP->blue, amountF);
	}
	return err;
}

static PF_Err
DrawVignetteVUYA_32f(
	void			*refcon,
	A_long			xL,
	A_long			yL,
	PF_PixelFloat	*inP,
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;

	PF_Pixel_VUYA_32f *inVUYA_32fP, *outVUYA_32fP;
	inVUYA_32fP = reinterpret_cast<PF_Pixel_VUYA_32f*>(inP);
	outVUYA_32fP = reinterpret_cast<PF_Pixel_VUYA_32f*>(outP);

	VigInfo	*viP = reinterpret_cast<VigInfo*>(refcon);

	if (viP) {
		PF_FpLong amountF = viP->amountF * CalculateBlendingAmount(refcon, xL, yL);

		outVUYA_32fP->alpha = inVUYA_32fP->alpha;
		outVUYA_32fP->luma = ClipSum32(inVUYA_32fP->luma, amountF);
		outVUYA_32fP->Pb = inVUYA_32fP->Pb;
		outVUYA_32fP->Pr = inVUYA_32fP->Pr;
	}
	return err;
}

static PF_Err
DrawVignette16(
	void		*refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16	*inP,
	PF_Pixel16	*outP)
{
	PF_Err		err = PF_Err_NONE;

	VigInfo	*viP = reinterpret_cast<VigInfo*>(refcon);

	auto ClipSum16 = [](const A_u_short in, const PF_FpLong modF) {
		return static_cast<A_u_short>(MAX(0, MIN(modF + in, PF_MAX_CHAN16)));
	};

	if (viP) {
		PF_FpLong amountF = viP->amountF * CalculateBlendingAmount(refcon, xL, yL);

		outP->alpha = inP->alpha;
		outP->red = ClipSum16(inP->red, amountF);
		outP->green = ClipSum16(inP->green, amountF);
		outP->blue = ClipSum16(inP->blue, amountF);
	}

	return err;
}

static PF_Err
DrawVignette8 (
	void		*refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err		err = PF_Err_NONE;

	VigInfo	*viP	= reinterpret_cast<VigInfo*>(refcon);

	if (viP){
		PF_FpLong amountF = viP->amountF * CalculateBlendingAmount(refcon, xL, yL);

		outP->alpha		= inP->alpha;
		outP->red		= ClipSum8(inP->red, amountF);
		outP->green		= ClipSum8(inP->green, amountF);
		outP->blue		= ClipSum8(inP->blue, amountF);
	}

	return err;
}


static PF_Err
DrawVignetteBGRA_8u(
	void		*refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel8	*inP,
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;

	PF_Pixel_BGRA_8u *inBGRA_8uP, *outBGRA_8uP;
	inBGRA_8uP = reinterpret_cast<PF_Pixel_BGRA_8u*>(inP);
	outBGRA_8uP = reinterpret_cast<PF_Pixel_BGRA_8u*>(outP);

	VigInfo	*viP = reinterpret_cast<VigInfo*>(refcon);

	if (viP) {
		PF_FpLong amountF = viP->amountF * CalculateBlendingAmount(refcon, xL, yL);

		outBGRA_8uP->alpha = inBGRA_8uP->alpha;
		outBGRA_8uP->red = ClipSum8(inBGRA_8uP->red, amountF);
		outBGRA_8uP->green = ClipSum8(inBGRA_8uP->green, amountF);
		outBGRA_8uP->blue = ClipSum8(inBGRA_8uP->blue, amountF);
	}
	return err;
}

static PF_Err
DrawVignetteVUYA_8u(
	void		*refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel8	*inP,
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;

	PF_Pixel_VUYA_8u *inVUYA_8uP, *outVUYA_8uP;
	inVUYA_8uP = reinterpret_cast<PF_Pixel_VUYA_8u*>(inP);
	outVUYA_8uP = reinterpret_cast<PF_Pixel_VUYA_8u*>(outP);

	VigInfo	*viP = reinterpret_cast<VigInfo*>(refcon);

	if (viP) {
		PF_FpLong amountF = viP->amountF * CalculateBlendingAmount(refcon, xL, yL);

		outVUYA_8uP->alpha = inVUYA_8uP->alpha;
		outVUYA_8uP->luma = ClipSum8(inVUYA_8uP->luma, amountF);
		outVUYA_8uP->Pb = inVUYA_8uP->Pb;
		outVUYA_8uP->Pr = inVUYA_8uP->Pr;
	}
	return err;
}


static PF_Err
IterateFloat(
	PF_InData			*in_data,
	A_long				progress_base,
	A_long				progress_final,
	PF_EffectWorld		*src,
	void				*refcon,
	PF_Err(*pix_fn)(void *refcon, A_long x, A_long y, PF_PixelFloat *in, PF_PixelFloat *out),
	PF_EffectWorld		*dst)
{
	PF_Err	err = PF_Err_NONE;
	char	*localSrc, *localDst;
	localSrc = reinterpret_cast<char*>(src->data);
	localDst = reinterpret_cast<char*>(dst->data);

	for (int y = progress_base; y < progress_final; y++)
	{
		for (int x = 0; x < in_data->width; x++)
		{
			pix_fn(refcon,
				   static_cast<A_long> (x),
				   static_cast<A_long> (y),
				   reinterpret_cast<PF_PixelFloat*>(localSrc),
				   reinterpret_cast<PF_PixelFloat*>(localDst));
			localSrc += 16;
			localDst += 16;
		}
		localSrc += (src->rowbytes - in_data->width * 16);
		localDst += (dst->rowbytes - in_data->width * 16);
	}

	return err;
}

static PF_Err 
Render (
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	PF_Err				err		= PF_Err_NONE;
	AEGP_SuiteHandler	suites(in_data->pica_basicP);

	VigInfo				viP;
	AEFX_CLR_STRUCT(viP);
	A_long				linesL	= 0;

	linesL 		= output->extent_hint.bottom - output->extent_hint.top;

	if (params[VIG_AMOUNT]->u.fs_d.value == 0 ||
		params[VIG_SIZE]->u.fs_d.value == 0)
	{
		ERR(PF_COPY(&params[VIG_INPUT]->u.ld, output, NULL, NULL ));
	}
	else {
		// setup VigInfo

		PF_FpLong featherF = params[VIG_FEATHER]->u.fs_d.value / 100.0;
		featherF = (featherF == 1.0) ? 0.99999 : featherF;


		viP.amountF = params[VIG_AMOUNT]->u.fs_d.value / 100.0;
		PF_FpLong ellipse_aaF = params[VIG_SIZE]->u.fs_d.value * in_data->width / 200.0;
		PF_FpLong ellipse_bbF = params[VIG_SIZE]->u.fs_d.value * in_data->height / 200.0;
		ellipse_bbF = ellipse_aaF * params[VIG_ROUNDNESS]->u.fs_d.value / 100.0 +
			ellipse_bbF * (100.0 - params[VIG_ROUNDNESS]->u.fs_d.value) / 100.0;

		PF_FpLong scale_x = in_data->downsample_x.num / (float)in_data->downsample_x.den,
				  scale_y = in_data->downsample_y.num / (float)in_data->downsample_y.den;

		ellipse_aaF *= scale_x;
		ellipse_bbF *= scale_y;

		viP.outerEllipse_aaF = ellipse_aaF * (1 + featherF);
		viP.outerEllipse_bbF = ellipse_bbF * (1 + featherF);

		viP.innerEllipse_aaF = ellipse_aaF * (1 - featherF);
		viP.innerEllipse_bbF = ellipse_bbF * (1 - featherF);

		viP.innerRect_aL = static_cast<A_u_long> (floor(viP.innerEllipse_aaF / PF_SQRT2));
		viP.innerRect_bL = static_cast<A_u_long> (floor(viP.innerEllipse_bbF / PF_SQRT2));
		viP.outerRect_aL = static_cast<A_u_long> (ceil(viP.outerEllipse_aaF));
		viP.outerRect_bL = static_cast<A_u_long> (ceil(viP.outerEllipse_bbF));

		if (params[VIG_SIZE]->u.fs_d.value >= 100.0)
		{
			viP.outerCheck = false;
		}
		else
		{
			viP.outerCheck = true;
		}

		viP.outerEllipse_abF = viP.outerEllipse_aaF * viP.outerEllipse_bbF;
		viP.outerEllipse_aaF *= viP.outerEllipse_aaF;
		viP.outerEllipse_bbF *= viP.outerEllipse_bbF;
		viP.outerEllipse_aabbF = viP.outerEllipse_aaF * viP.outerEllipse_bbF;

		viP.innerEllipse_abF = viP.innerEllipse_aaF * viP.innerEllipse_bbF;
		viP.innerEllipse_aaF *= viP.innerEllipse_aaF;
		viP.innerEllipse_bbF *= viP.innerEllipse_bbF;
		viP.innerEllipse_aabbF = viP.innerEllipse_aaF * viP.innerEllipse_bbF;

		viP.x_t = static_cast<A_long>(round(FIX_2_FLOAT(params[VIG_CENTER]->u.td.x_value)));
		viP.y_t = static_cast<A_long>(round(FIX_2_FLOAT(params[VIG_CENTER]->u.td.y_value)));

		// Do high-bit depth rendering in Premiere Pro
		if (in_data->appl_id == 'PrMr') {

			// Get the Premiere pixel format suite
			AEFX_SuiteScoper<PF_PixelFormatSuite1> pixelFormatSuite =
				AEFX_SuiteScoper<PF_PixelFormatSuite1>(in_data,
													   kPFPixelFormatSuite,
													   kPFPixelFormatSuiteVersion1,
													   out_data);

			PrPixelFormat destinationPixelFormat = PrPixelFormat_BGRA_4444_8u;

			pixelFormatSuite->GetPixelFormat(output, &destinationPixelFormat);

			AEFX_SuiteScoper<PF_Iterate8Suite1> iterate8Suite =
				AEFX_SuiteScoper<PF_Iterate8Suite1>(in_data,
													kPFIterate8Suite,
													kPFIterate8SuiteVersion1,
													out_data);
			switch (destinationPixelFormat)
			{

			case PrPixelFormat_BGRA_4444_8u:
				viP.amountF *= PF_MAX_CHAN8;
				iterate8Suite->iterate(in_data,
									   0,								// progress base
									   linesL,							// progress final
									   &params[VIG_INPUT]->u.ld,		// src 
									   NULL,							// area - null for all pixels
									   (void*)&viP,					// refcon - your custom data pointer
									   DrawVignetteBGRA_8u,				// pixel function pointer
									   output);

				break;
			case PrPixelFormat_VUYA_4444_8u:
				viP.amountF *= PF_MAX_CHAN8;
				iterate8Suite->iterate(in_data,
									   0,								// progress base
									   linesL,							// progress final
									   &params[VIG_INPUT]->u.ld,		// src 
									   NULL,							// area - null for all pixels
									   (void*)&viP,					// refcon - your custom data pointer
									   DrawVignetteVUYA_8u,				// pixel function pointer
									   output);

				break;
			case PrPixelFormat_BGRA_4444_32f:
				IterateFloat(in_data,
							 0,								// progress base
							 linesL,							// progress final
							 &params[VIG_INPUT]->u.ld,		// src 
							 (void*)&viP,					// refcon - your custom data pointer
							 DrawVignetteBGRA_32f,			// pixel function pointer
							 output);

				break;
			case PrPixelFormat_VUYA_4444_32f:
				IterateFloat(in_data,
							 0,								// progress base
							 linesL,							// progress final
							 &params[VIG_INPUT]->u.ld,		// src 
							 (void*)&viP,					// refcon - your custom data pointer
							 DrawVignetteVUYA_32f,			// pixel function pointer
							 output);
				break;
			default:
				//	Return error, because we don't know how to handle the specified pixel type
				return PF_Err_UNRECOGNIZED_PARAM_TYPE;
			}
		}
	}
	return err;
}

static PF_Err
PreRender(
	PF_InData				*in_data,
	PF_OutData				*out_data,
	PF_PreRenderExtra		*extra)
{
	PF_Err err = PF_Err_NONE;
	PF_RenderRequest req = extra->input->output_request;
	PF_CheckoutResult in_result;

	req.channel_mask = PF_ChannelMask_RED | PF_ChannelMask_GREEN | PF_ChannelMask_BLUE;

	req.preserve_rgb_of_zero_alpha = FALSE;	//	Hey, we don't care.

	ERR(extra->cb->checkout_layer(in_data->effect_ref,
								  VIG_INPUT,
								  VIG_INPUT,
								  &req,
								  in_data->current_time,
								  in_data->time_step,
								  in_data->time_scale,
								  &in_result));

	UnionLRect(&in_result.result_rect, &extra->output->result_rect);
	UnionLRect(&in_result.max_result_rect, &extra->output->max_result_rect);

	return err;
}


static PF_Err
SmartRenderProper(
	PF_InData		*in_data,
	PF_EffectWorld 	*input,
	PF_ParamDef		*params[],
	PF_OutData		*out_data,
	PF_EffectWorld	*output)
{
	PF_Err				err = PF_Err_NONE,
						err2 = PF_Err_NONE;
	VigInfo				viP;
	PF_Point			origin;
	PF_Rect				areaR;
	PF_PixelFormat		format = PF_PixelFormat_INVALID;
	PF_WorldSuite2		*wsP = NULL;

	AEGP_SuiteHandler suites(in_data->pica_basicP);

	ERR(AEFX_AcquireSuite(in_data,
						  out_data,
						  kPFWorldSuite,
						  kPFWorldSuiteVersion2,
						  "Couldn't load suite.",
						  (void**)&wsP));

	if (!err) {

		if (params[VIG_AMOUNT]->u.fs_d.value == 0 ||
			params[VIG_SIZE]->u.fs_d.value == 0)
		{
			areaR.left = -in_data->output_origin_x;
			areaR.top = -in_data->output_origin_y;
			areaR.bottom = areaR.top + output->height;
			areaR.right = areaR.left + output->width;

			err = PF_COPY(input, output, &areaR, NULL);
		}
		else {
			// setup VigInfo

			PF_FpLong featherF = params[VIG_FEATHER]->u.fs_d.value / 100.0;
			featherF = (featherF == 1.0) ? 0.99999 : featherF;


			viP.amountF = params[VIG_AMOUNT]->u.fs_d.value / 100.0;
			PF_FpLong ellipse_aaF = params[VIG_SIZE]->u.fs_d.value * in_data->width / 200.0;
			PF_FpLong ellipse_bbF = params[VIG_SIZE]->u.fs_d.value * in_data->height / 200.0;
			ellipse_bbF = ellipse_aaF * params[VIG_ROUNDNESS]->u.fs_d.value / 100.0 +
				ellipse_bbF * (100.0 - params[VIG_ROUNDNESS]->u.fs_d.value) / 100.0;

			PF_FpLong scale_x = in_data->downsample_x.num / (float)in_data->downsample_x.den,
				scale_y = in_data->downsample_y.num / (float)in_data->downsample_y.den;

			ellipse_aaF *= scale_x;
			ellipse_bbF *= scale_y;

			viP.outerEllipse_aaF = ellipse_aaF * (1 + featherF);
			viP.outerEllipse_bbF = ellipse_bbF * (1 + featherF);

			viP.innerEllipse_aaF = ellipse_aaF * (1 - featherF);
			viP.innerEllipse_bbF = ellipse_bbF * (1 - featherF);

			viP.innerRect_aL = static_cast<A_u_long> (floor(viP.innerEllipse_aaF / PF_SQRT2));
			viP.innerRect_bL = static_cast<A_u_long> (floor(viP.innerEllipse_bbF / PF_SQRT2));
			viP.outerRect_aL = static_cast<A_u_long> (ceil(viP.outerEllipse_aaF));
			viP.outerRect_bL = static_cast<A_u_long> (ceil(viP.outerEllipse_bbF));

			if (params[VIG_SIZE]->u.fs_d.value >= 100.0)
			{
				viP.outerCheck = false;
			}
			else
			{
				viP.outerCheck = true;
			}

			viP.outerEllipse_abF = viP.outerEllipse_aaF * viP.outerEllipse_bbF;
			viP.outerEllipse_aaF *= viP.outerEllipse_aaF;
			viP.outerEllipse_bbF *= viP.outerEllipse_bbF;
			viP.outerEllipse_aabbF = viP.outerEllipse_aaF * viP.outerEllipse_bbF;

			viP.innerEllipse_abF = viP.innerEllipse_aaF * viP.innerEllipse_bbF;
			viP.innerEllipse_aaF *= viP.innerEllipse_aaF;
			viP.innerEllipse_bbF *= viP.innerEllipse_bbF;
			viP.innerEllipse_aabbF = viP.innerEllipse_aaF * viP.innerEllipse_bbF;

			viP.x_t = static_cast<A_long>(round(FIX_2_FLOAT(params[VIG_CENTER]->u.td.x_value) - input->origin_x));
			viP.y_t = static_cast<A_long>(round(FIX_2_FLOAT(params[VIG_CENTER]->u.td.y_value) - input->origin_y));

			origin.h = in_data->output_origin_x;
			origin.v = in_data->output_origin_y;

			areaR.top =
				areaR.left = 0;
			areaR.right = output->width;
			areaR.bottom = output->height;

			ERR(wsP->PF_GetPixelFormat(input, &format));

			switch (format) {

			case PF_PixelFormat_ARGB128:

				ERR(suites.IterateFloatSuite1()->iterate_origin(in_data,
																0,
																output->height,
																input,
																&areaR,
																&origin,
																(void*)(&viP),
																DrawVignette32,
																output));
				break;

			case PF_PixelFormat_ARGB64:
				viP.amountF *= PF_MAX_CHAN16;
				ERR(suites.Iterate16Suite1()->iterate_origin(in_data,
															 0,
															 output->height,
															 input,
															 &areaR,
															 &origin,
															 (void*)(&viP),
															 DrawVignette16,
															 output));
				break;

			case PF_PixelFormat_ARGB32:
				viP.amountF *= PF_MAX_CHAN8;

				ERR(suites.Iterate8Suite1()->iterate_origin(in_data,
															0,
															output->height,
															input,
															&areaR,
															&origin,
															(void*)(&viP),
															DrawVignette8,
															output));
				break;

			default:
				err = PF_Err_BAD_CALLBACK_PARAM;
				break;
			}
		}
	}
	ERR2(AEFX_ReleaseSuite(in_data,
						   out_data,
						   kPFWorldSuite,
						   kPFWorldSuiteVersion2,
						   "Couldn't release suite."));
	return err;
}


static PF_Err
SmartRender(
	PF_InData				*in_data,
	PF_OutData				*out_data,
	PF_SmartRenderExtra		*extra)

{

	PF_Err			err = PF_Err_NONE,
					err2 = PF_Err_NONE;

	PF_EffectWorld	*input_worldP = NULL,
		*output_worldP = NULL;

	PF_ParamDef params[VIG_NUM_PARAMS];
	PF_ParamDef *paramsP[VIG_NUM_PARAMS];

	AEFX_CLR_STRUCT(params);

	for (int i = 0; i < VIG_NUM_PARAMS; i++)
	{
		paramsP[i] = &params[i];
	}

	// checkout input & output buffers.
	ERR((extra->cb->checkout_layer_pixels(in_data->effect_ref, VIG_INPUT, &input_worldP)));

	ERR(extra->cb->checkout_output(in_data->effect_ref, &output_worldP));

	// checkout the required params
	ERR(PF_CHECKOUT_PARAM(in_data,
						  VIG_CENTER,
						  in_data->current_time,
						  in_data->time_step,
						  in_data->time_scale,
						  &params[VIG_CENTER]));
	ERR(PF_CHECKOUT_PARAM(in_data,
						  VIG_AMOUNT,
						  in_data->current_time,
						  in_data->time_step,
						  in_data->time_scale,
						  &params[VIG_AMOUNT]));
	ERR(PF_CHECKOUT_PARAM(in_data,
						  VIG_FEATHER,
						  in_data->current_time,
						  in_data->time_step,
						  in_data->time_scale,
						  &params[VIG_FEATHER]));
	ERR(PF_CHECKOUT_PARAM(in_data,
						  VIG_ROUNDNESS,
						  in_data->current_time,
						  in_data->time_step,
						  in_data->time_scale,
						  &params[VIG_ROUNDNESS]));
	ERR(PF_CHECKOUT_PARAM(in_data,
						  VIG_SIZE,
						  in_data->current_time,
						  in_data->time_step,
						  in_data->time_scale,
						  &params[VIG_SIZE]));


	ERR(SmartRenderProper(in_data,
					   input_worldP,
					   paramsP,
					   out_data,
					   output_worldP));

	// Always check in, no matter what the error condition!

	ERR2(PF_CHECKIN_PARAM(in_data, &params[VIG_CENTER]));
	ERR2(PF_CHECKIN_PARAM(in_data, &params[VIG_SIZE]));
	ERR2(PF_CHECKIN_PARAM(in_data, &params[VIG_ROUNDNESS]));
	ERR2(PF_CHECKIN_PARAM(in_data, &params[VIG_FEATHER]));
	ERR2(PF_CHECKIN_PARAM(in_data, &params[VIG_AMOUNT]));

	return err;
}

DllExport	
PF_Err 
EntryPointFunc (
	PF_Cmd			cmd,
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output,
	void			*extra)
{
	PF_Err		err = PF_Err_NONE;
	
	try {
		switch (cmd) {
			case PF_Cmd_ABOUT:

				err = About(in_data,
							out_data,
							params,
							output);
				break;
				
			case PF_Cmd_GLOBAL_SETUP:

				err = GlobalSetup(	in_data,
									out_data,
									params,
									output);
				break;
				
			case PF_Cmd_PARAMS_SETUP:

				err = ParamsSetup(	in_data,
									out_data,
									params,
									output);
				break;
				
			case PF_Cmd_RENDER:

				err = Render(	in_data,
								out_data,
								params,
								output);
				break;
			case PF_Cmd_SMART_PRE_RENDER:
				err = PreRender(in_data, out_data, (PF_PreRenderExtra*)extra);
				break;
			case PF_Cmd_SMART_RENDER:
				err = SmartRender(in_data, out_data, (PF_SmartRenderExtra*)extra);
				break;
		}
	}
	catch(PF_Err &thrown_err){
		err = thrown_err;
	}
	return err;
}
