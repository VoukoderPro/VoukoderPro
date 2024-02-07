/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2009 Adobe Systems Incorporated                       */
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


#ifndef PRSDKPLAYMODULEOVERLAYSUITESUITE_H
#define PRSDKPLAYMODULEOVERLAYSUITESUITE_H


#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKMALERRORS_H
#include "PrSDKMALErrors.h"
#endif

#ifndef PRSDKPLAYMODULE_H
#include "PrSDKPlayModule.h"
#endif

#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif


#define kPrSDKPlayModuleOverlaySuite			"MediaCore Playmod Overlay Suite"
#define kPrSDKPlayModuleOverlaySuiteVersion3	3
#define kPrSDKPlayModuleOverlaySuiteVersion4	4
#define kPrSDKPlayModuleOverlaySuiteVersion		kPrSDKPlayModuleOverlaySuiteVersion4


/**
**	If supported, set the player capability PMCapSupportsOverlayDrawing
*/
typedef struct
{
	/**
	**	Renders an overlay into the provided PPix.
	**		May only draw into opaque pixels unless inClearToTransparentBlack is specified.
	**		If a PPix is created, it will be cached under the key retrieved from GetIdentifier.
	**
	**	@param	inPlayID					Player module identifier
	**	@param	inTime						Requested overlay time
	**	@param	inLogicalRegion				Region of video segments being rendered
	**	@param	inDisplayWidth				Size of display
	**	@param	inDisplayHeight				Size of display
	**	@param	inScaleFactor				HIDPI scaling
	**	@param	inClearToTransparentBlack	if kPrTrue the frame will first be cleared to transparent black before render
	**											This is useful if the composite will be of the whole frame instead of just
	**											the returned visible regions.
	**	@param	ioPPix						The frame into which to draw the overlay
	**											If NULL the host will allocate the PPix
	**											If provided PPix must be BGRA square pixel and sized to inDisplayWidth & inDisplayHeight
	*/
	prSuiteError (*RenderImage)(
		PrPlayID inPlayID,
		PrTime inTime,
		const prRect* inLogicalRegion,
		int inDisplayWidth,
		int inDisplayHeight,
		float inScaleFactor,
		prBool inClearToTransparentBlack,
		PPixHand* ioPPix);

	/**
	**	Returns a unique identifier for the frame that would be produced with 
	**	the provided overlay parameters.
	**
	**	@param	inPlayID					Player module identifier
	**	@param	inTime						Requested overlay time
	**	@param	inLogicalRegion				Region of video segments being rendered
	**	@param	inDisplayWidth				Size of display
	**	@param	inDisplayHeight				Size of display
	**	@param	inScaleFactor				HIDPI scaling
	**	@param	inClearToTransparentBlack	if kPrTrue the frame will first be cleared to transparent black before render
	**	@param	outIdentifier				Unique identifier for requested params
	*/
	prSuiteError (*GetIdentifier)(
		PrPlayID inPlayID,
		PrTime inTime,
		const prRect* inLogicalRegion,
		int inDisplayWidth,
		int inDisplayHeight,
		float inScaleFactor,
		prBool inClearToTransparentBlack,
		prPluginID* outIdentifier);

	/**
	**	Returns if there are any regions visible for the current parameters.
	**
	**	@param	inPlayID					Player module identifier
	**	@param	inTime						Requested overlay time
	**	@param	inLogicalRegion				Region of video segments being rendered
	**	@param	inDisplayWidth				Size of display
	**	@param	inDisplayHeight				Size of display
	**	@param	inScaleFactor				HIDPI scaling
	**	@param	outHasVisibleRegions		kPrTrue if the overlay varies is visible
	*/
	prSuiteError (*HasVisibleRegions)(
		PrPlayID inPlayID,
		PrTime inTime,
		const prRect* inLogicalRegion,
		int inDisplayWidth,
		int inDisplayHeight,
		float inScaleFactor,
		prBool* outHasVisibleRegions);

	/**
	**	Returns if the current overlay will vary over time
	**
	**	@param	inPlayID					Player module identifier
	**	@param	outVariesOverTime			kPrTrue if the overlay varies over time
	*/
	prSuiteError (*VariesOverTime)(
		PrPlayID inPlayID,
		prBool* outVariesOverTime);

	/**
	**	Returns if there are any regions visible for the current parameters.
	**
	**	@param	inPlayID							Player module identifier
	**	@param	outHasCaptionsToDraw				kPrTrue if there are captions to draw
	**	@param	outCaptionStreamFormatToDisplay		returns the Caption stream format to display in Overview (if none: kPrRenderCaptionStreamFormat_Undefined)
	*/
	prSuiteError(*IsCaptionsOverlay)(
		PrPlayID inPlayID,
		prBool* outHasCaptionsToDraw,
		PrRenderCaptionStreamFormat* outCaptionStreamFormatToDisplay);

	/**
	**	Renders an overlay into the provided PPix.
	**		May only draw into opaque pixels unless inClearToTransparentBlack is specified.
	**		If a PPix is created, it will be cached under the key retrieved from GetIdentifier.
	**
	**	@param	inPlayID					Player module identifier
	**	@param	inTime						Requested overlay time
	**	@param	inLogicalRegion				Region of video segments being rendered
	**	@param	inDisplayWidth				Size of display
	**	@param	inDisplayHeight				Size of display
	**	@param	inScaleFactor				HIDPI scaling
	**	@param	inClearToTransparentBlack	if kPrTrue the frame will first be cleared to transparent black before render
	**											This is useful if the composite will be of the whole frame instead of just
	**											the returned visible regions.
	**	@param	inIsTranmit					Indicates to draw the overlay for Transmit
	**	@param	ioPPix						The frame into which to draw the overlay
	**											If NULL the host will allocate the PPix
	**											If provided PPix must be BGRA square pixel and sized to inDisplayWidth & inDisplayHeight
	*/
	prSuiteError(*RenderImage2)(
		PrPlayID inPlayID,
		PrTime inTime,
		const prRect* inLogicalRegion,
		int inDisplayWidth,
		int inDisplayHeight,
		float inScaleFactor,
		prBool inClearToTransparentBlack,
		prBool inIsTransmit,
		PPixHand* ioPPix);

	/**
	**	Returns a unique identifier for the frame that would be produced with
	**	the provided overlay parameters.
	**
	**	@param	inPlayID					Player module identifier
	**	@param	inTime						Requested overlay time
	**	@param	inLogicalRegion				Region of video segments being rendered
	**	@param	inDisplayWidth				Size of display
	**	@param	inDisplayHeight				Size of display
	**	@param	inScaleFactor				HIDPI scaling
	**	@param	inClearToTransparentBlack	if kPrTrue the frame will first be cleared to transparent black before render
	**	@param	inIsTranmit					Indicates to draw the overlay for Transmit
	**	@param	outIdentifier				Unique identifier for requested params
	*/
	prSuiteError(*GetIdentifier2)(
		PrPlayID inPlayID,
		PrTime inTime,
		const prRect* inLogicalRegion,
		int inDisplayWidth,
		int inDisplayHeight,
		float inScaleFactor,
		prBool inClearToTransparentBlack,
		prBool inIsTransmit,
		prPluginID* outIdentifier);

	/**
	**	Returns if there are any regions visible for the current parameters.
	**
	**	@param	inPlayID					Player module identifier
	**	@param	inTime						Requested overlay time
	**	@param	inLogicalRegion				Region of video segments being rendered
	**	@param	inDisplayWidth				Size of display
	**	@param	inDisplayHeight				Size of display
	**	@param	inScaleFactor				HIDPI scaling
	**	@param	inIsTranmit					Indicates to draw the overlay for Transmit
	**	@param	outHasVisibleRegions		kPrTrue if the overlay varies is visible
	*/
	prSuiteError(*HasVisibleRegions2)(
		PrPlayID inPlayID,
		PrTime inTime,
		const prRect* inLogicalRegion,
		int inDisplayWidth,
		int inDisplayHeight,
		float inScaleFactor,
		prBool inIsTransmit,
		prBool* outHasVisibleRegions);


} PrSDKPlayModuleOverlaySuite;



#ifdef __cplusplus
}
#endif

#pragma pack(pop)


#endif