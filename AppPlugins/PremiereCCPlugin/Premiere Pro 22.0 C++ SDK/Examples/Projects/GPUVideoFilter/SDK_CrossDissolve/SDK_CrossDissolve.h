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


#ifndef SDK_CROSSDISSOLVE_H
#define SDK_CROSSDISSOLVE_H

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
	SDK_CROSSDISSOLVE_INPUT = 0,
	SDK_CROSSDISSOLVE_FLIP,
	SDK_CROSSDISSOLVE_TRANSITIONINPUT,
	SDK_CROSSDISSOLVE_NUM_PARAMS
};

/*
**
*/
#define	PROGRESS_MIN_VALUE		0.0f
#define	PROGRESS_MAX_VALUE		100.0f
#define	PROGRESS_MIN_SLIDER		0.0f
#define	PROGRESS_MAX_SLIDER		100.0f
#define	PROGRESS_DFLT			0.0f

/*
**
*/
#define	PLUGIN_MAJOR_VERSION		1
#define	PLUGIN_MINOR_VERSION		0
#define	PLUGIN_BUG_VERSION			0
#define	PLUGIN_STAGE_VERSION		PF_Stage_DEVELOP
#define	PLUGIN_BUILD_VERSION		0


#endif