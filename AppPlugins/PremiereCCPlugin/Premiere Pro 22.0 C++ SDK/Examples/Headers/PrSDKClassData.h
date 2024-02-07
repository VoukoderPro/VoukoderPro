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

//	This file is part of the Adobe Premiere SDK.

#ifndef PRSDKCLASSDATA_H
#define PRSDKCLASSDATA_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif

// ClassData functions (availabe to Media Abstraction Layer plug-ins)

typedef void* (* GetClassDataFunc)(unsigned int theclass);
typedef int (* SetClassDataFunc)(unsigned int theclass, void* info);

typedef struct {
	SetClassDataFunc	setClassData;
	GetClassDataFunc	getClassData;
} ClassDataFuncs, *ClassDataFuncsPtr;

#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif /* PRSDKCLASSDATA_H */
