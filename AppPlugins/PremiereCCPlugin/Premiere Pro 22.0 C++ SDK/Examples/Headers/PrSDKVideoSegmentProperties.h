/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2008 Adobe Systems Incorporated                       */
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

#ifndef PRSDKVIDEOSEGMENTPROPERTIES_H
#define PRSDKVIDEOSEGMENTPROPERTIES_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

/**
**	This file contains all of the string definitions of properties that may be found in video segments.
**	Each one is list by the node type it may show up in, and hs a description of its value type.
**
**	No property is ever guaranteed to be present on a node.
*/

//	MediaNode, int32
#define kVideoSegmentProperty_Media_ClipID "MediaNode::ClipID"

//	MediaNode, int32
#define kVideoSegmentProperty_Media_ProxyClipID "MediaNode::ProxyClipID"

//	MediaNode, string
#define kVideoSegmentProperty_Media_InstanceString "MediaNode::MediaInstanceString"

//	MediaNode, string
#define kVideoSegmentProperty_Media_ProxyInstanceString "MediaNode::ProxyMediaInstanceString"

//	MediaNode, string
#define kVideoSegmentProperty_Media_ImplementationID "MediaNode::ImplementationID"

//	MediaNode, size_t
#define kVideoSegmentProperty_Media_StreamGroup "MediaNode::MediaStreamGroup"

//	MediaNode, string
#define kVideoSegmentProperty_Media_StreamLabel "MediaNode::MediaStreamLabel"

//	MediaNode, bool
#define kVideoSegmentProperty_Media_IsDraft "MediaNode::MediaIsDraft"

//	MediaNode, binary data
#define kVideoSegmentProperty_Media_ModState "MediaNode::MediaModState"

//	MediaNode, bool
#define kVideoSegmentProperty_Media_IsOffline "MediaNode::MediaIsOffline"

//	MediaNode, bool
#define kVideoSegmentProperty_Media_IsPending "MediaNode::MediaIsPending"

//	MediaNode, stringl
#define kVideoSegmentProperty_Media_CaptioningID "MediaNode::MediaCaptioningID"

//	MediaNode, PrTime
#define kVideoSegmentProperty_Media_StreamFrameRate "MediaNode::StreamFrameRate"

//	MediaNode, int32
#define kVideoSegmentProperty_Media_StreamAlphaType "MediaNode::StreamAlphaType"

//	MediaNode, bool
#define kVideoSegmentProperty_Media_StreamIgnoreAlpha "MediaNode::StreamIgnoreAlpha"

//	MediaNode, bool
#define kVideoSegmentProperty_Media_StreamInvertAlpha "MediaNode::StreamInvertAlpha"

//	MediaNode, int32
#define kVideoSegmentProperty_Media_StreamAlphaMatteColor "MediaNode::StreamAlphaMatteColor"

//	MediaNode, bool
#define kVideoSegmentProperty_Media_StreamRemovePulldown "MediaNode::StreamRemovePulldown"

//	MediaNode, int32
#define kVideoSegmentProperty_Media_StreamPixelAspectRatioNum "MediaNode::StreamPixelAspectRatioNum"

//	MediaNode, int32
#define kVideoSegmentProperty_Media_StreamFrameWidth "MediaNode::StreamFrameWidth"

//	MediaNode, int32
#define kVideoSegmentProperty_Media_StreamFrameHeight "MediaNode::StreamFrameHeight"

//	MediaNode, int32
#define kVideoSegmentProperty_Media_StreamPixelAspectRatioDen "MediaNode::StreamPixelAspectRatioDen"

//	MediaNode, int32
#define kVideoSegmentProperty_Media_StreamFieldType "MediaNode::StreamFieldType"

//	MediaNode, binary data
#define kVideoSegmentProperty_Media_StreamOpaqueData "MediaNode::StreamOpaqueData"

//	MediaNode, binary data
#define kVideoSegmentProperty_Media_ProxyStreamOpaqueData "MediaNode::ProxyStreamOpaqueData"


//	MediaNode, int32
#define kVideoSegmentProperty_Media_StreamPullDownCadence "MediaNode::StreamPullDownCadence"

//	MediaNode, int32
#define kVideoSegmentProperty_Media_StreamFrameVidSubType "MediaNode::StreamFrameVidSubType"

//	MediaNode, bool
#define kVideoSegmentProperty_Media_StreamIsContinuousTime "MediaNode::kVideoSegmentProperty_Media_StreamIsContinuousTime"

//	MediaNode, bool
#define kVideoSegmentProperty_Media_StreamIsRollCrawl "MediaNode::StreamIsRollCrawl"

//	MediaNode, int32
#define kVideoSegmentProperty_Media_RollCrawlDuration "MediaNode::RollCrawlDuration"

//	MediaNode, PrTime
#define kVideoSegmentProperty_Media_ContentStart "MediaNode::ContentStart"

//	MediaNode, PrTime
#define kVideoSegmentProperty_Media_ContentEnd "MediaNode::ContentEnd"

//	MediaNode, PrTime
#define kVideoSegmentProperty_Media_StartTimecodeOffset "MediaNode::StartTimecodeOffset"
#define kVideoSegmentProperty_Media_ProxyStartTimecodeOffset "MediaNode::ProxyStartTimecodeOffset"

//	MediaNode, string
#define kVideoSegmentProperty_Media_NestedSequenceHash "MediaNode::NestedSequenceHash"

//	MediaNode, int32
#define kVideoSegmentProperty_Media_SelectedMulticamTrackFromNestedSequence "MediaNode::SelectedMulticamTrack"

//  MediaNode string
#define kVideoSegmentProperty_Media_MulticamCameraOrderFromNestedSequence "MediaNode::MulticamCameraOrder"

//  MediaNode string
#define kVideoSegmentProperty_Media_MulticamCamerasPerPage "MediaNode::MulticamCamerasPerPage"

//  MediaNode string
#define kVideoSegmentProperty_Media_MulticamCurrentPage "MediaNode::MulticamCurrentPage"

//	MediaNode, int32
#define kVideoSegmentProperty_Media_SelectedMulticamTrackTimelineID "MediaNode::SelectedMulticamTrackTimelineID"

//	MediaNode, int32
#define kVideoSegmentProperty_Media_NestedSequenceTimelineID "MediaNode::NestedSequenceTimelineID"

//	MediaNode, bool
#define kVideoSegmentProperty_Media_TrackItemIsMuted "MediaNode::TrackItemIsMuted"

//	MediaNode, float64
#define kVideoSegmentProperty_Media_ClipSpeed "MediaNode::ClipSpeed"

//	MediaNode, bool
#define kVideoSegmentProperty_Media_ClipBackwards "MediaNode::ClipBackwards"

//	MediaNode, bool
#define kVideoSegmentProperty_Media_StreamFrameBlend "MediaNode::StreamFrameBlend"

//	MediaNode, uint32 - containing dvamediatypes::TimeInterpolationType
#define kVideoSegmentProperty_Media_StreamTimeInterpolationType "MediaNode::StreamTimeInterpolationType"

//	MediaNode, bool
#define kVideoSegmentProperty_Media_ClipScaleToFrameSize "MediaNode::ClipScaleToFrameSize"

//	MediaNode, int, optional see PrNodeScalePolicy
#define kVideoSegmentProperty_Media_ClipScaleToFramePolicy "MediaNode::ClipScaleToFramePolicy"

//	MediaNode, bool
#define kVideoSegmentProperty_Media_StreamReverseFieldDominance "MediaNode::StreamReverseFieldDominance"

//	MediaNode, bool
#define kVideoSegmentProperty_Media_DeinterlaceAlways "MediaNode::DeinterlaceAlways"

//	MediaNode, bool
#define kVideoSegmentProperty_Media_RemoveFlicker "MediaNode::RemoveFlicker"

//	MediaNode, bool
#define kVideoSegmentProperty_Media_InterlaceConsecutiveFrames "MediaNode::InterlaceConsecutiveFrames"

//	MediaNode, string
#define kVideoSegmentProperty_Media_SequenceColorSpace "MediaNode::SequenceColorSpace"

//	MediaNode, string
#define kVideoSegmentProperty_Media_StreamColorSpace "MediaNode::StreamColorSpace"

//	MediaNode, string
#define kVideoSegmentProperty_Media_StreamInputLUTID "MediaNode::StreamInputLUTID"

//	MediaNode, int32 - positive values mean shift up, negative means shift down
#define kVideoSegmentProperty_Media_ScanlineOffsetToImproveVerticalCentering "MediaNode::ScanlineOffsetToImproveVerticalCentering"

//	MediaNode, int64 - media in point in units of ticks in media time
#define kVideoSegmentProperty_Media_InPointMediaTimeAsTicks "MediaNode::InPointMediaTimeAsTicks"

//	MediaNode, int64 - media out point in units of ticks in media time
#define kVideoSegmentProperty_Media_OutPointMediaTimeAsTicks "MediaNode::OutPointMediaTimeAsTicks"

//	MediaNode, int32 - containing sequence field type
#define kVideoSegmentProperty_Media_SequenceFieldType "MediaNode::SequenceFieldType"

//	MediaNode, int32 - containing sequence frame rate
#define kVideoSegmentProperty_Media_SequenceFrameRate "MediaNode::SequenceFrameRate"

//	MediaNode, int32 - containing sequence width
#define kVideoSegmentProperty_Media_SequenceWidth "MediaNode::SequenceWidth"

//	MediaNode, int32 - containing sequence height
#define kVideoSegmentProperty_Media_SequenceHeight "MediaNode::SequenceHeight"

//	MediaNode, int32 - containing sequence pixel aspect ratio num
#define kVideoSegmentProperty_Media_SequencePixelAspectRatioNum "MediaNode::SequencePixelAspectRatioNum"

//	MediaNode, int32 - containing sequence pixel aspect ratio den
#define kVideoSegmentProperty_Media_SequencePixelAspectRatioDen "MediaNode::SequencePixelAspectRatioDen"

// Orientation for primary source
#define kVideoSegmentProperty_Media_OrientationType "MediaNode::OrientationType"

// Orientation for Proxy
#define kVideoSegmentProperty_Media_OrientationTypeProxy "MediaNode::OrientationTypeProxy"

//	ClipNode, float64
#define kVideoSegmentProperty_Clip_ClipSpeed "ClipNode::ClipSpeed"

//	ClipNode, bool
#define kVideoSegmentProperty_Clip_ClipBackwards "ClipNode::ClipBackwards"

//	ClipNode, keyframe data
#define kVideoSegmentProperty_Clip_TimeRemapping "ClipNode::TimeRemapping"

//	ClipNode, PrTime
#define kVideoSegmentProperty_Clip_FrameHoldAtTime "ClipNode::FrameHoldAtTime"

//	ClipNode, bool
#define kVideoSegmentProperty_Clip_FrameHoldFilters "ClipNode::FrameHoldFilters"

//	ClipNode, PrTime
#define kVideoSegmentProperty_Clip_GrowingDuration "ClipNode::GrowingDuration"

//	ClipNode, bool
#define kVideoSegmentProperty_Clip_FrameHoldDeinterlace "ClipNode::FrameHoldDeinterlace"

//	ClipNode, int32
#define kVideoSegmentProperty_Clip_TrackID "ClipNode::TrackID"

//	ClipNode, int64 - the start of the track item in units of ticks in sequence time
#define kVideoSegmentProperty_Clip_TrackItemStartAsTicks "ClipNode::TrackItemStartAsTicks"

//	ClipNode, int64 - the end of the track item in units of ticks in sequence time
#define kVideoSegmentProperty_Clip_TrackItemEndAsTicks "ClipNode::TrackItemEndAsTicks"

//	ClipNode, int64 - the "effective" start of the track item, adjusted for transitions at the head, in units of ticks in sequence time
#define kVideoSegmentProperty_Clip_EffectiveTrackItemStartAsTicks "ClipNode::EffectiveTrackItemStartAsTicks"

//	ClipNode, int64 - the "effective" start of the track item, adjusted for transitions at the tail, in units of ticks in sequence time
#define kVideoSegmentProperty_Clip_EffectiveTrackItemEndAsTicks "ClipNode::EffectiveTrackItemEndAsTicks"

//	ClipNode, bool - only set if false
#define kVideoSegmentProperty_Clip_AllowLinearCompositing "ClipNode::AllowLinearCompositing"

//	ClipNode, bool - only set if true
#define kVideoSegmentProperty_Clip_HasCaptions "ClipNode::HasCaptions"

//	ClipNode, int64 - the untrimmed duration of the clip in ticks
#define kVideoSegmentProperty_Clip_UntrimmedDuration "ClipNode::UntrimmedDuration"

//	MulticamNode, int32
#define kVideoSegmentProperty_Multicam_SelectedTrack "MulticamNode::SelectedTrack"

//	MulticamNode, string
#define kVideoSegmentProperty_Multicam_CameraOrder "MulticamNode::CameraOrder"

//	MulticamNode, string
#define kVideoSegmentProperty_Multicam_CamerasPerPage "MulticamNode::CamerasPerPage"

//	MulticamNode, string
#define kVideoSegmentProperty_Multicam_CurrentPage "MulticamNode::CurrentPage"

//	MulticamNode, bool
#define kVideoSegmentProperty_Multicam_ShowProgram "MulticamNode::ShowProgram"

//	MulticamNode, bool
#define kVideoSegmentProperty_Multicam_Recording "MulticamNode::Recording"

//	MulticamNode, bool
#define kVideoSegmentProperty_Multicam_IsNonMultiCam "MulticamNode::IsNonMultiCam"

//	MulticamNode, bool
#define kVideoSegmentProperty_Multicam_UseTimeRemapping "MulticamNode::UseTimeRemapping"

//	SolidColorNode, int32
#define kVideoSegmentProperty_SolidColor_Color "SolidColorNode::Color"

//	EffectNode, string
#define kVideoSegmentProperty_Effect_FilterMatchName "EffectNode::FilterMatchName"

//	EffectNode, binary data
#define kVideoSegmentProperty_Effect_FilterOpaqueData "EffectNode::FilterOpaqueData"

//	EffectNode, keyframe data
#define kVideoSegmentProperty_Effect_FilterParams "EffectNode::FilterParams"

//	EffectNode, PrTime
#define kVideoSegmentProperty_Effect_EffectDuration "EffectNode::EffectDuration"

//	EffectNode, uint32
#define kVideoSegmentProperty_Effect_RuntimeInstanceID "EffectNode::RuntimeInstanceID"

//	EffectNode, string
#define kVideoSegmentProperty_Effect_LayerInputHashes "EffectNode::LayerInputHashes"

//	EffectNode, string
#define kVideoSegmentProperty_Effect_StreamLabel "EffectNode::StreamLabel"

// EffectNode, string
#define kVideoSegmentProperty_Effect_ClipName "EffectNode::ClipName"

// EffectNode, string
#define kVideoSegmentProperty_Effect_MasterClipName "EffectNode::MasterClipName"

// EffectNode, string
#define kVideoSegmentProperty_Effect_FileName "EffectNode::FileName"

// EffectNode, string
#define kVideoSegmentProperty_Effect_SourceTrackClipNameHashes "EffectNode::SourceTrackClipNameHashes"

//	TransitionNode, string
#define kVideoSegmentProperty_Transition_TransitionMatchName "TransitionNode::TransitionMatchName"

//	TransitionNode, binary data
#define kVideoSegmentProperty_Transition_TransitionOpaqueData "TransitionNode::TransitionOpaqueData"

//	TransitionNode, fpoint32
#define kVideoSegmentProperty_Transition_TransitionStartPosition "TransitionNode::TransitionStartPosition"

//	TransitionNode, fpoint32
#define kVideoSegmentProperty_Transition_TransitionEndPosition "TransitionNode::TransitionEndPosition"

//	TransitionNode, fpoint32
#define kVideoSegmentProperty_Transition_TransitionCenterPosition "TransitionNode::TransitionCenterPosition"

//	TransitionNode, float32
#define kVideoSegmentProperty_Transition_TransitionStartPercent "TransitionNode::TransitionStartPercent"

//	TransitionNode, float32
#define kVideoSegmentProperty_Transition_TransitionEndPercent "TransitionNode::TransitionEndPercent"

//	TransitionNode, float32
#define kVideoSegmentProperty_Transition_TransitionBorderWidth "TransitionNode::TransitionBorderWidth"

//	TransitionNode, int32
#define kVideoSegmentProperty_Transition_TransitionBorderColor "TransitionNode::TransitionBorderColor"

//	TransitionNode, bool
#define kVideoSegmentProperty_Transition_TransitionSwitchSources "TransitionNode::TransitionSwitchSources"

//	TransitionNode, bool
#define kVideoSegmentProperty_Transition_TransitionReverse "TransitionNode::TransitionReverse"

//	TransitionNode, int32
#define kVideoSegmentProperty_Transition_TransitionDirection "TransitionNode::TransitionDirection"

//	TransitionNode, int32
#define kVideoSegmentProperty_Transition_TransitionAntiAliasQuality "TransitionNode::TransitionAntiAliasQuality"

//	TransitionNode, PrTime
#define kVideoSegmentProperty_Transition_TransitionDuration "TransitionNode::TransitionDuration"

//	TransitionNode, keyframe data
#define kVideoSegmentProperty_Transition_TransitionParams "TransitionNode::TransitionParams"

//	TransitionNode, uint32
#define kVideoSegmentProperty_Transition_RuntimeInstanceID "TransitionNode::RuntimeInstanceID"

//	AdjustmentNode, bool
#define kVideoSegmentProperty_Adjustment_AdjustmentMediaIsOpaque "AdjustmentNode::AdjustmentMediaIsOpaque"

//	AdjustmentNode, bool
#define kVideoSegmentProperty_Adjustment_InvertAlpha "AdjustmentNode::InvertAlpha"

#endif
