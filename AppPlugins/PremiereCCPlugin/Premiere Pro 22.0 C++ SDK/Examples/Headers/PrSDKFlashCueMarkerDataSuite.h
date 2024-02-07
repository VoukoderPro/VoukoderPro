/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2004 Adobe Systems Incorporated                       */
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

//	This file is part of the public Premiere SDK.
#ifndef PRSDKFLASHCUEMARKERDATASUITE_H
#define PRSDKFLASHCUEMARKERDATASUITE_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKMALERRORS_H
#include "PrSDKMALErrors.h"
#endif

#ifndef PRSDKTIMESUITE_H
#include "PrSDKTimeSuite.h"
#endif

#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif

#define kPrSDKFlashCueMarkerDataSuite				"FlashCueMarkerData Suite"
#define kPrSDKFlashCueMarkerDataSuiteVersion		1

// PrFlashCueMarkerID is acquired from the PrMarkerSuite, which also is used
// to dispose of it.
typedef csSDK_uint32 PrFlashCueMarkerID;

typedef struct 
{
	/*
	**	A cue point is either an Event or a Navigation cue, this is a boolean style
	**	query - if this is not a Navigation point, it is an Event cue.
	**
	**	@param inMarkerID				a marker ID that was retrieved using the PrMarkerSuite
	**									function, GetFlashCueMarkerID()
	**	@param	outIsNavigationCue		non-zero if this is a Navigation cue, zero if this is an Event cue.
	**
	*/
	prSuiteError (*GetIsNavigationCue)(
		PrFlashCueMarkerID inMarkerID,
		csSDK_uint32* outIsNavigationCue);

	/*
	**	Retrieve the number of key/value pairs (which are prUTF16Char character buffers) the flash cue has.
	**	May be zero if there are no key/value pairs.
	**
	**	@param inMarkerID					a marker ID that was retrieved using the PrMarkerSuite
	**										function, GetFlashCueMarkerID()
	**	@param	outNumberKeyValuePairs		the number of pairs, used for querying for individual pairs' data
	*/
	prSuiteError (*GetNumberKeyValuePairs)(
		PrFlashCueMarkerID inMarkerID,
		csSDK_uint32* outNumberKeyValuePairs);

	/*
	**	Retrieve the size of the buffers you allocate using prNewPtr prior to calling GetKeyValuePairData.
	**
	**	@param inMarkerID				a marker ID that was retrieved using the PrMarkerSuite
	**									function, GetFlashCueMarkerID()
	**	@param	inKeyValuePairIndex		the index of the particular pair you are querying for
	**	@param	outKeyStringLength		the size of the buffer you'll create using prNewPtr
	**	@param	outValueStringLength	the size of the buffer you'll create using prNewPtr
	*/
	prSuiteError (*GetKeyValuePairStringLength)(
		PrFlashCueMarkerID inMarkerID,
		csSDK_uint32 inKeyValuePairIndex,
		size_t* outKeyStringLength,
		size_t* outValueStringLength);

	/*
	**	Retrieve a specific key/value pair - the buffers will contain prUTF16Char characters.
	**
	**	@param inMarkerID				a marker ID that was retrieved using the PrMarkerSuite
	**									function, GetFlashCueMarkerID()
	**	@param	inKeyValuePairIndex		the index of the particular pair you are querying for
	**	@param	outKeyBuffer			the buffer you allocated using prNewPtr(keyStringLength).
	**	@param	outValueBuffer			the buffer you allocated using prNewPtr(valueStringLength).
	*/
	prSuiteError (*GetKeyValuePairData)(
		PrFlashCueMarkerID inMarkerID,
		csSDK_uint32 inKeyValuePairIndex,
		PrMemoryPtr* outKeyBuffer,
		PrMemoryPtr* outValueBuffer);

} PrSDKFlashCueMarkerDataSuite;

#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif