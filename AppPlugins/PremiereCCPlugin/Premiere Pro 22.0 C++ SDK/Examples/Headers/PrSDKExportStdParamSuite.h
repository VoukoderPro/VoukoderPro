/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2007 Adobe Systems Incorporated                       */
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

#ifndef PRSDKEXPORTSTDPARAMSUITE_H
#define PRSDKEXPORTSTDPARAMSUITE_H

#define kPrExportStdParamSuite				"Premiere Export Standard Param Suite"
#define kPrExportStdParamSuiteVersion		1

#define ADBEStillSequence	"ADBEStillSequence"

enum PrSDKStdParamType
{
	SDKStdParams_Video,
	SDKStdParams_Audio,
	SDKStdParams_Still,
	SDKStdParams_Reserved1,			// do not use
	SDKStdParams_VideoBitrateGroup,
	SDKStdParams_Video_NoRenderMax,
	SDKStdParams_Video_AddRenderMax,
	SDKStdParams_AudioTabOnly,
	SDKStdParams_AudioBitrateGroup,
	SDKStdParams_VideoWithSizePopup,
	SDKStdParams_AudioChannelConfigurationGroup,
	SDKStdParams_AudioAndChannelConfigurationGroup
};

enum PrSDKStdParamGroup
{
	SDKStdParamGroup_VideoBitrate
};

typedef struct 
{
	void (*AddStandardParams)(
		csSDK_uint32 exporterPluginID,
		PrSDKStdParamType inSDKStdParamType);

	void (*PostProcessParamNames)(
		csSDK_uint32 exporterPluginID,
		PrAudioChannelType inSourceAudioChannelType);

	void (*QueryOutputSettings)(
		csSDK_uint32 exporterPluginID,
		exQueryOutputSettingsRec* outOutputSettings);

	void (*MakeParamSummary)(
		csSDK_uint32 exporterPluginID,
		prBool inDoVideo,
		prBool inDoAudio,
        prBool inIsDiscreteAudio,
		prUTF16Char* outVideoDescription,	// 256
		prUTF16Char* outAudioDescription);	// 256

} PrSDKExportStdParamSuite;

#endif