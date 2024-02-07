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



/*
**
*/
static PF_Err GlobalSetup(
	PF_InData* in_data,
	PF_OutData* out_data,
	PF_ParamDef* params[],
	PF_LayerDef* output)
{
	out_data->my_version	= PF_VERSION(MAJOR_VERSION, MINOR_VERSION, BUG_VERSION, STAGE_VERSION, BUILD_VERSION);

	if (in_data->appl_id == 'PrMr')
	{
		AEFX_SuiteScoper<PF_PixelFormatSuite1> pixelFormatSuite(in_data, kPFPixelFormatSuite, kPFPixelFormatSuiteVersion1, out_data);
		(*pixelFormatSuite->ClearSupportedPixelFormats)(in_data->effect_ref);
		(*pixelFormatSuite->AddSupportedPixelFormat)(in_data->effect_ref, PrPixelFormat_VUYA_4444_32f);
	}

	out_data->out_flags |= PF_OutFlag_USE_OUTPUT_EXTENT;
	out_data->out_flags2 |= PF_OutFlag2_PRESERVES_FULLY_OPAQUE_PIXELS;

	return PF_Err_NONE;
}
/*
**
*/
static PF_Err GlobalSetdown(
	PF_InData* in_data,
	PF_OutData* out_data,
	PF_ParamDef* params[],
	PF_LayerDef* output)
{
	return PF_Err_NONE;
}

/*
**
*/
static PF_Err ParamsSetup(
	PF_InData* in_data,
	PF_OutData* out_data,
	PF_ParamDef* params[],
	PF_LayerDef* output)
{
	PF_ParamDef	def;

	// Brightness
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(
		"Brightness",
		BRIGHTNESS_MIN_VALUE, 
		BRIGHTNESS_MAX_VALUE, 
		BRIGHTNESS_MIN_SLIDER, 
		BRIGHTNESS_MAX_SLIDER, 
		BRIGHTNESS_DFLT, 
		1, 
		PF_ValueDisplayFlag_NONE,
		0,
		SDK_PROCAMP_BRIGHTNESS);

	// Contrast
	AEFX_CLR_STRUCT(def);	
	PF_ADD_FIXED(
		"Contrast",
		CONTRAST_MIN_VALUE, 
		CONTRAST_MAX_VALUE, 
		CONTRAST_MIN_SLIDER, 
		CONTRAST_MAX_SLIDER, 
		CONTRAST_DFLT, 
		1, 
		PF_ValueDisplayFlag_NONE,
		0,
		SDK_PROCAMP_CONTRAST);

	// Hue
	AEFX_CLR_STRUCT(def);
	PF_ADD_ANGLE(
		"Hue",
		HUE_DFLT,
		SDK_PROCAMP_HUE);

	// Saturation
	AEFX_CLR_STRUCT(def);	
	PF_ADD_FIXED(
		"Saturation",
		SATURATION_MIN_VALUE, 
		SATURATION_MAX_VALUE, 
		SATURATION_MIN_SLIDER, 
		SATURATION_MAX_SLIDER, 
		SATURATION_DFLT, 
		1, 
		PF_ValueDisplayFlag_NONE,
		0,
		SDK_PROCAMP_SATURATION);

	out_data->num_params = SDK_PROCAMP_NUM_PARAMS;
	return PF_Err_NONE;
}

/*
**
*/
static PF_Err Render(
	PF_InData* in_data,
	PF_OutData* out_data,
	PF_ParamDef* params[],
	PF_LayerDef* output)
{

	if (in_data->appl_id == 'PrMr')
	{
		PF_LayerDef* src = &params[0]->u.ld;
		PF_LayerDef* dest = output;

		const char* srcData = (const char*)src->data;
		char* destData = (char*)dest->data;

		float brightness = (float)FIX_2_FLOAT(params[SDK_PROCAMP_BRIGHTNESS]->u.fd.value) / 100.0f;
		float contrast = (float)FIX_2_FLOAT(params[SDK_PROCAMP_CONTRAST]->u.fd.value) / 100.0f;
		float hue = (float)FIX_2_FLOAT(params[SDK_PROCAMP_HUE]->u.ad.value);
		float saturation = (float)FIX_2_FLOAT(params[SDK_PROCAMP_SATURATION]->u.fd.value) / 100.0f;

		float hueRadians = (float)(M_PI / 180) * hue;
		float hueCosSaturation = (float)cos(hueRadians) * saturation;
		float hueSinSaturation = (float)sin(hueRadians) * saturation;

		for (int y = 0; y < output->height; ++y, srcData += src->rowbytes, destData += dest->rowbytes)
		{
			for (int x = 0; x < output->width; ++x)
			{
				float v = ((const float*)srcData)[x * 4 + 0];
				float u = ((const float*)srcData)[x * 4 + 1];
				float y = ((const float*)srcData)[x * 4 + 2];
				float a = ((const float*)srcData)[x * 4 + 3];

				((float*)destData)[x * 4 + 0] = (v * hueCosSaturation) + (u *  hueSinSaturation);
				((float*)destData)[x * 4 + 1] = (u * hueCosSaturation) + (v * -hueSinSaturation);
				((float*)destData)[x * 4 + 2] = (contrast * y) + brightness;
				((float*)destData)[x * 4 + 3] = a;
			}
		}
	}

	return PF_Err_NONE;
}

/*
**
*/
#ifdef MSWindows
#define DllExport   __declspec( dllexport )
#else
#define DllExport	__attribute__((visibility("default")))
#endif
extern "C" DllExport PF_Err EffectMain(
	PF_Cmd inCmd,
	PF_InData* in_data,
	PF_OutData* out_data,
	PF_ParamDef* params[],
	PF_LayerDef* inOutput,
	void* extra)
{
	PF_Err err = PF_Err_NONE;
	switch (inCmd)
	{
	case PF_Cmd_GLOBAL_SETUP:
		err = GlobalSetup(in_data, out_data, params, inOutput);
		break;
	case PF_Cmd_GLOBAL_SETDOWN:
		err = GlobalSetdown(in_data, out_data, params, inOutput);
		break;
	case PF_Cmd_PARAMS_SETUP:
		err = ParamsSetup(in_data, out_data, params, inOutput);
		break;
	case PF_Cmd_RENDER:
		err = Render(in_data, out_data, params, inOutput);
		break;
	}
	return err;
}