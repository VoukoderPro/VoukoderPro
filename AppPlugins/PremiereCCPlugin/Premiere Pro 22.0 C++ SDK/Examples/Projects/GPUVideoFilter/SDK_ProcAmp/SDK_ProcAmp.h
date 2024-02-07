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


#ifndef SDK_PROCAMP_H
#define SDK_PROCAMP_H

#include "AEConfig.h"

#include "PrSDKTypes.h"
#include "AE_Effect.h"
#include "A.h"
#include "AE_Macros.h"
#include "AEFX_SuiteHandlerTemplate.h"
#include "Param_Utils.h"

#include "PrSDKAESupport.h"

#include <math.h>


/*
**
*/
enum
{
	SDK_PROCAMP_INPUT = 0,
	SDK_PROCAMP_BRIGHTNESS,
	SDK_PROCAMP_CONTRAST,
	SDK_PROCAMP_HUE,
	SDK_PROCAMP_SATURATION,
	SDK_PROCAMP_NUM_PARAMS
};

/*
**
*/
#define	BRIGHTNESS_MIN_VALUE		-100
#define	BRIGHTNESS_MAX_VALUE		100
#define	BRIGHTNESS_MIN_SLIDER		-100
#define	BRIGHTNESS_MAX_SLIDER		100
#define	BRIGHTNESS_DFLT				0

#define	CONTRAST_MIN_VALUE			0
#define	CONTRAST_MAX_VALUE			200
#define	CONTRAST_MIN_SLIDER			0
#define	CONTRAST_MAX_SLIDER			200
#define	CONTRAST_DFLT				100

#define	HUE_DFLT					0

#define	SATURATION_MIN_VALUE		0
#define	SATURATION_MAX_VALUE		200
#define	SATURATION_MIN_SLIDER		0
#define	SATURATION_MAX_SLIDER		200
#define	SATURATION_DFLT				100

/*
**
*/
#define	MAJOR_VERSION		1
#define	MINOR_VERSION		0
#define	BUG_VERSION			0
#define	STAGE_VERSION		PF_Stage_DEVELOP
#define	BUILD_VERSION		0


#endif