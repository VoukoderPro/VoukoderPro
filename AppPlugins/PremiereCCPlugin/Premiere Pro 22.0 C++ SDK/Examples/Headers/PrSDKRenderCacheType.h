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

#ifndef	PRSDKRENDERCACHETYPE_H
#define PRSDKRENDERCACHETYPE_H

#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif

typedef csSDK_int32 PrRenderCacheType;
const PrRenderCacheType kRenderCacheType_None =						0;
const PrRenderCacheType kRenderCacheType_ImportedFrames =			1 << 0;
const PrRenderCacheType kRenderCacheType_ImportedStillFrames =		1 << 1;
const PrRenderCacheType kRenderCacheType_IntermediateFrames =		1 << 2;
const PrRenderCacheType kRenderCacheType_RenderedFrame =			1 << 3;
const PrRenderCacheType kRenderCacheType_RenderedStillFrames =		1 << 4;		// Still frames with static effects - New in PPro 2.0
const PrRenderCacheType kRenderCacheType_PersistentPrefetch =		1 << 5;
const PrRenderCacheType kRenderCacheType_AllFrames =				kRenderCacheType_ImportedFrames | kRenderCacheType_ImportedStillFrames | kRenderCacheType_IntermediateFrames | kRenderCacheType_RenderedFrame | kRenderCacheType_RenderedStillFrames;

#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif
