/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2013 Adobe Systems Incorporated                       */
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


#include "SDK_CrossDissolve.h"



/*
**
*/
static PF_Err GlobalSetup(
	PF_InData* in_data,
	PF_OutData* out_data,
	PF_ParamDef* params[],
	PF_LayerDef* output)
{
	out_data->my_version = PF_VERSION(PLUGIN_MAJOR_VERSION, PLUGIN_MINOR_VERSION, PLUGIN_BUG_VERSION, PLUGIN_STAGE_VERSION, PLUGIN_BUILD_VERSION);

    if (in_data->appl_id == 'PrMr')
    {
        AEFX_SuiteScoper<PF_PixelFormatSuite1> pixelFormatSuite(in_data, kPFPixelFormatSuite, kPFPixelFormatSuiteVersion1, out_data);
        (*pixelFormatSuite->ClearSupportedPixelFormats)(in_data->effect_ref);
        (*pixelFormatSuite->AddSupportedPixelFormat)(in_data->effect_ref, PrPixelFormat_BGRA_4444_32f);

        AEFX_SuiteScoper<PF_UtilitySuite4> utilitySuite(in_data, kPFUtilitySuite, kPFUtilitySuiteVersion4, out_data);
        utilitySuite->EffectWantsCheckedOutFramesToMatchRenderPixelFormat(in_data->effect_ref);
    }
	
	out_data->out_flags |= PF_OutFlag_USE_OUTPUT_EXTENT | PF_OutFlag_NON_PARAM_VARY;
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

	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(
		"Flip",
		"",
		0,
		0,
		SDK_CROSSDISSOLVE_FLIP);

	// Here's where we register to behave as a transition in Premiere
	if (in_data->appl_id == 'PrMr')
	{
		try {
			AEFX_SuiteScoper<PF_TransitionSuite> transitionSuite(in_data, kPFTransitionSuite, kPFTransitionSuiteVersion, out_data);
			(*transitionSuite->RegisterTransitionInputParam)(in_data->effect_ref, SDK_CROSSDISSOLVE_TRANSITIONINPUT);
		} catch (A_long) {
			// If the suite is not present (prior to Premiere Pro 7.1), then the transition will appear as an effect
		}
	}

	AEFX_CLR_STRUCT(def);
	PF_ADD_LAYER(
		"Input",
		PF_LayerDefault_NONE,
		SDK_CROSSDISSOLVE_TRANSITIONINPUT);

	out_data->num_params = SDK_CROSSDISSOLVE_NUM_PARAMS;
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
	PF_ParamDef param;
	PF_CHECKOUT_PARAM(in_data, SDK_CROSSDISSOLVE_TRANSITIONINPUT, in_data->current_time, in_data->time_step, in_data->time_scale, &param);

	float progress = (float)in_data->current_time / (float)in_data->total_time;

	PF_LayerDef* outgoing = &params[0]->u.ld;
	PF_LayerDef* incoming = &param.u.ld;
	PF_LayerDef* dest = output;

	const char* outgoingData = (const char*)outgoing->data;
	const char* incomingData = (const char*)incoming->data;
	char* destData = (char*)dest->data;

	if (params[SDK_CROSSDISSOLVE_FLIP]->u.bd.value)
	{
		destData += (output->height - 1) * dest->rowbytes;
		dest->rowbytes = -dest->rowbytes;
	}

	for (int y = 0; y < output->height;
		++y, outgoingData += outgoing->rowbytes, incomingData += incoming->rowbytes, destData += dest->rowbytes)
	{
		for (int x = 0; x < output->width; ++x)
		{
			float outgoingB = outgoingData ? ((const float*)outgoingData)[x * 4 + 0] : 0.0f;
			float outgoingG = outgoingData ? ((const float*)outgoingData)[x * 4 + 1] : 0.0f;
			float outgoingR = outgoingData ? ((const float*)outgoingData)[x * 4 + 2] : 0.0f;
			float outgoingA = outgoingData ? ((const float*)outgoingData)[x * 4 + 3] : 0.0f;

			float incomingB = incomingData ? ((const float*)incomingData)[x * 4 + 0] : 0.0f;
			float incomingG = incomingData ? ((const float*)incomingData)[x * 4 + 1] : 0.0f;
			float incomingR = incomingData ? ((const float*)incomingData)[x * 4 + 2] : 0.0f;
			float incomingA = incomingData ? ((const float*)incomingData)[x * 4 + 3] : 0.0f;

			float outgoingAlphaWeighted = outgoingA * (1.0f - progress);
			float incomingAlphaWeighted = incomingA * progress;
			float newAlpha = outgoingAlphaWeighted + incomingAlphaWeighted;
			float recipNewAlpha = newAlpha != 0.0f ? 1.0f / newAlpha : 0.0f;
			
			((float*)destData)[x * 4 + 0] = (outgoingB * outgoingAlphaWeighted + incomingB * incomingAlphaWeighted) * recipNewAlpha;
			((float*)destData)[x * 4 + 1] = (outgoingG * outgoingAlphaWeighted + incomingG * incomingAlphaWeighted) * recipNewAlpha;
			((float*)destData)[x * 4 + 2] = (outgoingR * outgoingAlphaWeighted + incomingR * incomingAlphaWeighted) * recipNewAlpha;
			((float*)destData)[x * 4 + 3] = newAlpha;
		}
	}

	PF_CHECKIN_PARAM(in_data, &param);

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