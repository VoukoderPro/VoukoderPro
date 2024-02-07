/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2017 Adobe Systems Incorporated                       */
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
/*																   */
/*******************************************************************/

#ifndef	PRSDKDATASTREAMTYPES_H
#define PRSDKDATASTREAMTYPES_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif

/**
**	Types used to describe different aspects of data stream types
*/

/**
**	Defines data stream types
*/
typedef enum {

	//!Indicates Data Group
	kPrDSGroupType_GROUP = 0,

	//!Indicates Dynamic Group
	kPrDSGroupType_DYNAMIC_DATA,

	//!Indicates Static Group
	kPrDSGroupType_STATIC_DATA,

	//!Indicates invalid entry
	kPrDSGroupType_INVALID
} PrDSGroupChildType;


#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif /* PRSDKDATASTREAMTYPES_H */
