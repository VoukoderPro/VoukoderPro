/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2002 Adobe Systems Incorporated                       */
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
#ifndef PRSDKACTIVATIONEVENT_H
#define PRSDKACTIVATIONEVENT_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

/**
**	PrActivationEvent - Describes the event that caused activation/deactivation to occur
*/
typedef enum 
{
	PrActivationEvent_Unspecified,
	PrActivationEvent_RecorderActivated,
	PrActivationEvent_PlayerActivated,
	PrActivationEvent_ApplicationLostFocus,

	PrActivationEvent_ForceSize = kPrForceEnumToIntValue
} PrActivationEvent;

#endif 

