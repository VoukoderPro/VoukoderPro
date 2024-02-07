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


#ifndef PRSDKALPHATYPES_H
#define PRSDKALPHATYPES_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

// alpha channel types for alphaType field:

typedef csSDK_uint32 PrSDKAlphaType;		
const PrSDKAlphaType alphaUnknown		= 0;	// Unsupported
const PrSDKAlphaType alphaStraight		= 1;	// straight alpha channel
const PrSDKAlphaType alphaBlackMatte	= 2;	// Unsupported
const PrSDKAlphaType alphaWhiteMatte	= 3;	// Unsupported
const PrSDKAlphaType alphaArbitrary		= 4;	// Unsupported
const PrSDKAlphaType alphaNone			= 5;	// no alpha channel
const PrSDKAlphaType alphaOpaque		= 6;	// has alpha channel prefilled to opaque
const PrSDKAlphaType alphaIgnore		= 7;	// has alpha channel but ignore it

#endif