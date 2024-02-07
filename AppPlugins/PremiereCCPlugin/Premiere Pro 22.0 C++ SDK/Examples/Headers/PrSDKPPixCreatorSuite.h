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

#ifndef	PRSDKPPIXCREATORSUITE_H
#define PRSDKPPIXCREATORSUITE_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKPIXELFORMAT_H
#include "PrSDKPixelFormat.h"
#endif 

#ifndef PRSDKMALERRORS_H
#include "PrSDKMALErrors.h"
#endif

#ifndef PRSDKPPIXSUITE_H
#include "PrSDKPPixSuite.h"
#endif 

#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif

#define kPrSDKPPixCreatorSuite				"Premiere PPix Creator Suite"
#define kPrSDKPPixCreatorSuiteVersion		1

typedef struct 
{
	/**
	**	This will create a new ppix. 
	**
	**	@param	outPPixHand						The new ppix handle if the create was successfull. NULL otherwise. 
	**	@param	inRequestedAccess				Requested pixel access. ReadOnly is not allowed (doesn't make sense).
	**	@param	inPixelFormat					The pixel format of this ppix.
	*/
	prSuiteError (*CreatePPix)(
		PPixHand*			outPPixHand,
		PrPPixBufferAccess	inRequestedAccess,
		PrPixelFormat		inPixelFormat,
		const prRect*		inBoundingRect);

	/**
	**	This will clone an existing ppix. It will ref-count the ppix if only read-access is requested and 
	**		the ppix to copy from is read-only as well, otherwise it will create a new one and copy.
	**
	**	@param	inPPixToClone					The ppix to clone from.
	**	@param	outPPixHand						The new ppix handle if the create was successfull. NULL otherwise. 
	**	@param	inRequestedAccess				Requested pixel access on the new ppix. (Only PrPPixBufferAccess_ReadOnly is allowed right now).
	*/
	prSuiteError (*ClonePPix)(
		PPixHand			inPPixToClone,
		PPixHand*			outPPixHand,
		PrPPixBufferAccess	inRequestedAccess);

} PrSDKPPixCreatorSuite;

#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif
