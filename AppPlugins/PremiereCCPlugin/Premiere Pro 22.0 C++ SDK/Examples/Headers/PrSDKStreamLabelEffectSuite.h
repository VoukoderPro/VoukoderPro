/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2011 Adobe Systems Incorporated                       */
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


#ifndef PRSDKSTREAMLABELEFFECTSUITE_H
#define PRSDKSTREAMLABELEFFECTSUITE_H

#ifndef PRSDKSTREAMLABEL_H
#include "PrSDKStreamLabel.h"
#endif

#define kPrSDKStreamLabelEffectSuite			"Effect Stream Label Suite"
#define kPrSDKStreamLabelEffectSuiteVersion1	1
#define kPrSDKStreamLabelEffectSuiteVersion		kPrSDKStreamLabelEffectSuiteVersion1

#ifndef	_H_AE_Effect
#include "AE_Effect.h"
#endif

typedef struct
{
	// The first two functions are called during global setup, just like the AddSupportedPixelFormat pattern
	PF_Err (*PF_AddStreamLabel)(PF_ProgPtr effect_ref, PrSDKStreamLabel inLabel);
	PF_Err (*PF_ClearStreamLabels)(PF_ProgPtr effect_ref);

	// This function is called during a render, or frame setup. 
	PF_Err (*PF_CurrentRenderMatchesStreamLabel)(PF_ProgPtr effect_ref, PrSDKStreamLabel inLabel, PF_Boolean* outMatches);
} PrSDKStreamLabelEffectSuite;


#endif //PRSDKSTREAMLABELEFFECTSUITE_H