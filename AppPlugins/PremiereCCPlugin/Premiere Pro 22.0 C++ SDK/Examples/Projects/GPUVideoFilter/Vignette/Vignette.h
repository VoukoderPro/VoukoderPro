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

/* Vignette.h */

#pragma once

#define PF_TABLE_BITS	12
#define PF_TABLE_SZ_16	4096

#define PF_DEEP_COLOR_AWARE 1	// make sure we get 16bpc pixels; 
								// AE_Effect.h checks for this.

#include "AEConfig.h"

#ifdef AE_OS_WIN
	typedef unsigned short PixelType;
	#include <Windows.h>
#endif

#include "PrSDKTypes.h"
#include "entry.h"
#include "AE_Effect.h"
#include "AE_EffectCB.h"
#include "AE_EffectCBSuites.h"
#include "PrSDKAESupport.h"
#include "AE_Macros.h"
#include "Param_Utils.h"
#include "Smart_Utils.h"
#include "AEFX_SuiteHelper.h"
#include "AEGP_SuiteHandler.h"

#include <string>

	const std::string
		strName{ "Quick Vignette" },
		strDescription{ "Quick Vignette for fxphd SYS204 class." },
		strCenter_ParamName{ "Center point" },
		strAmount_ParamName{ "Amount" },
		strFeather_ParamName{ "Feather" },
		strRoundness_ParamName{ "Roundness" },
		strSize_ParamName{ "Size" };


/* Versioning information */

#define	PLUGIN_MAJOR_VERSION	1
#define	PLUGIN_MINOR_VERSION	0
#define	PLUGIN_BUG_VERSION		0
#define	PLUGIN_STAGE_VERSION	PF_Stage_DEVELOP
#define	PLUGIN_BUILD_VERSION	1


/* Parameter defaults */
	const long int
		VIG_CENTER_DFLT = 50;

	const PF_FpShort
		VIG_AMOUNT_MIN = -100.0,
		VIG_AMOUNT_MAX = 100.0,
		VIG_AMOUNT_DFLT = -30.0,
		VIG_SIZE_RANGE_MIN = 0.0,
		VIG_SIZE_RANGE_MAX = 250.0,
		VIG_SIZE_MIN = 50.0,
		VIG_SIZE_MAX = 150.0,
		VIG_SIZE_DFLT = 100.0,
		VIG_ROUNDNESS_MIN = 0.0,
		VIG_ROUNDNESS_MAX = 100.0,
		VIG_ROUNDNESS_DFLT = 0.0,
		VIG_FEATHER_MIN = 0.0,
		VIG_FEATHER_MAX = 100.0,
		VIG_FEATHER_DFLT = 30.0;

enum {
	VIG_INPUT = 0,
	VIG_CENTER,
	VIG_AMOUNT,
	VIG_SIZE,
	VIG_ROUNDNESS,
	VIG_FEATHER,
	VIG_NUM_PARAMS
};

enum {
	AMOUNT_DISK_ID = 1,
	SIZE_DISK_ID,
	ROUNDNESS_DISK_ID,
	FEATHER_DISK_ID,
	CENTER_DISK_ID
};

typedef struct VigInfo{
	PF_FpLong	amountF;
	PF_FpLong	outerEllipse_aaF;
	PF_FpLong	outerEllipse_bbF;
	PF_FpLong	outerEllipse_aabbF;
	PF_FpLong	outerEllipse_abF;
	PF_FpLong	innerEllipse_aaF;
	PF_FpLong	innerEllipse_bbF;
	PF_FpLong	innerEllipse_aabbF;
	PF_FpLong	innerEllipse_abF;
	A_long		x_t;
	A_long		y_t;
	A_long		innerRect_aL;
	A_long		innerRect_bL;
	A_long		outerRect_aL;
	A_long		outerRect_bL;
	PF_Boolean	outerCheck;
} VigInfo;

typedef struct {
	A_u_char	blue, green, red, alpha;
} PF_Pixel_BGRA_8u;

typedef struct {
	A_u_char	Pr, Pb, luma, alpha;
} PF_Pixel_VUYA_8u;

typedef struct {
	PF_FpShort	blue, green, red, alpha;
} PF_Pixel_BGRA_32f;

typedef struct {
	PF_FpShort	Pr, Pb, luma, alpha;
} PF_Pixel_VUYA_32f;

#ifdef __cplusplus
	extern "C" {
#endif
	
DllExport	PF_Err 
EntryPointFunc(	
	PF_Cmd			cmd,
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output,
	void			*extra) ;

#ifdef __cplusplus
}
#endif
