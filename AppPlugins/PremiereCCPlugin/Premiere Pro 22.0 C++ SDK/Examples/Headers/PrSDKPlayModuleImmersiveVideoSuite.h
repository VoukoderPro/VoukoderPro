/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2016 Adobe Systems Incorporated                       */
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
#ifndef PRSDKPLAYMODULEIMMERSIVEVIDEOSUITE_H
#define PRSDKPLAYMODULEIMMERSIVEVIDEOSUITE_H

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

#define kPrSDKPlayModuleImmersiveVideoSuite				"MediaCore Playmod Immersive Video Suite"
#define kPrSDKPlayModuleImmersiveVideoSuiteVersion		2

/**
**	If supported, set the player capability PMCapSupportsImmersiveVideo
*/
typedef struct
{
	/**
	**	Retrieves the current immersive video configuration for a specified player module.
	**		If immersive video playback is not used, kPrIVProjection_None and kkPrIVFrameLayout_Monoscopic will be returned
	**		along with 0 degrees of horizontal and vertical captured view.
	**
	**	@param	inPlayID					Player module identifier.
	**	@param	outProjection				The type of projection the current player module is using.
	**	@param	outFrameLayout				The type of frame layout the current player module is using.
	**	@param	outHorizontalCapturedFOV	How many degrees of horizontal view is captured in the video stream (up to 360).
	**	@param	outVerticalCapturedFOV		How many degrees of vertical view is captured in the video stream (up to 180).
	*/
	prSuiteError(*GetPlayerConfiguration)(
		PrPlayID inPlayID,
		PrIVProjectionType* outProjection,
		PrIVFrameLayout* outFrameLayout,
		csSDK_uint32* outHorizontalCapturedFOV,
		csSDK_uint32* outVerticalCapturedFOV);

	/**
	**	Tell the player module that head tracking is or is not supported.
	**
	**	@param	inPlayID					Player module identifier.
	**	@param	inTrackingSupported			kPrTrue = head tracking is supported, kPrFalse = head tracking is not supported.
	*/
	prSuiteError(*SupportsHeadTracking)(
		PrPlayID inPlayID,
		prBool inTrackingSupported);

	/**
	**	Notify the player module of the head-mounted display's current direction.
	**
	**	@param	inPlayID					Player module identifier.
	**	@param	inPan						The angle, in degrees, of the horizontal pan. -180 to +180, with negative turning left, and positive turning right.
	**	@param	inTilt						The angle, in degrees, of the vertical tilt. -90 to +90, with negative tilting down, and positive tilting up.
	**	@param	inRoll						The angle, in degrees, of the counter/clockwise roll. -180 to +180, with negative turning counter-clockwise, and positive turning clockwise.
	*/
	prSuiteError(*NotifyDirection)(
		PrPlayID inPlayID,
		float inPan,
		float inTilt,
		float inRoll);

	/**
	**	Retrieves the current immersive video configuration for a specified player module.
	**		If immersive video playback is not used, kPrIVProjection_None and kkPrIVFrameLayout_Monoscopic will be returned
	**		along with 0 degrees of horizontal and vertical captured view.
	**
	**	@param	inPlayID					Player module identifier.
	**	@param	outProjection				The type of projection the current player module is using.
	**	@param	outFrameLayout				The type of frame layout the current player module is using.
	**	@param	outHorizontalCapturedFOV	How many degrees of horizontal view is captured in the video stream (up to 360).
	**	@param	outVerticalCapturedFOV		How many degrees of vertical view is captured in the video stream (up to 180).
	**	@param	outAmbisonicsMonitoring		Returns how if ambisonics monitoring is enabled and if so, how it is configured.
	*/
	prSuiteError(*GetPlayerConfiguration2)(
		PrPlayID inPlayID,
		PrIVProjectionType* outProjection,
		PrIVFrameLayout* outFrameLayout,
		csSDK_uint32* outHorizontalCapturedFOV,
		csSDK_uint32* outVerticalCapturedFOV,
		PrIVAmbisonicsMonitoringType* outAmbisonicsMonitoring);

	/**
	**	Returns if the player is currently viewing immersive video and if so, what its current status is.
	**
	**	@param	inPlayID					Player module identifier.
	**	@param	outViewingImmersiveVideo	Returns kPrTrue if currently viewing immersive/360/VR video.
	**	@param	outMonitoringAmbisonics		Returns kPrTrue if currently monitoring ambisonics audio.
	**	@param	outTrackingHMD				Returns kPrTrue if the player is currently tracking the head-mounted display's direction.
	**	@param	outPan						Returns the angle, in degrees, of the horizontal pan. -180 to +180, with negative turning left, and positive turning right.
	**	@param	outTilt						Returns the angle, in degrees, of the vertical tilt. -90 to +90, with negative tilting down, and positive tilting up.
	**	@param	outRoll						Returns the angle, in degrees, of the counter/clockwise roll. -180 to +180, with negative turning counter-clockwise, and positive turning clockwise.
	*/
	prSuiteError(*GetPlayerStatus)(
		PrPlayID inPlayID,
		prBool* outViewingImmersiveVideo,
		prBool* outMonitoringAmbisonics,
		prBool* outTrackingHMD,
		float* outPan,
		float* outTilt,
		float* outRoll);
} PrSDKPlayModuleImmersiveVideoSuite;

#ifdef __cplusplus
}
#endif

#pragma pack(pop)


#endif	