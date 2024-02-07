/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2015 Adobe Systems Incorporated                       */
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

#ifndef PRSDKEXPORTAUDIOPARAMSUITE_H
#define PRSDKEXPORTAUDIOPARAMSUITE_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKMALERRORS_H
#include "PrSDKMALErrors.h"
#endif

#ifndef PRSDKAUDIOSUITE_H
#include "PrSDKAudioSuite.h"
#endif

#ifndef PRSDKEXPORT_H
#include "PrSDKExport.h"
#endif

#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif

#define kPrSDKExportAudioParamSuite "Premiere Export Audio Param Suite"
#define kPrSDKExportAudioParamSuiteVersion 1

typedef struct 
{
	/**
	**
	*/
	prSuiteError (*GetNumAudioStreams)(
		csSDK_uint32 inExporterPluginID,
		csSDK_uint32* outNumAudioStreams);

	/**
	**
	*/
	prSuiteError (*SetNumAudioStreams)(
		csSDK_uint32 inExporterPluginID,
		csSDK_uint32 inNumAudioStreams);

	/**
	**
	*/
	prSuiteError (*GetTotalNumAudioChannels)(
		csSDK_uint32 inExporterPluginID,
		csSDK_uint32* outNumAudioChannels);
	

	/**
	**
	*/
	prSuiteError (*SetMaxNumAudioChannelsPerStream)(
		csSDK_uint32 inExporterPluginID,
		csSDK_uint32 inNumAudioStreams);

	/**
	**
	*/
	prSuiteError (*SetMaxNumAudioStreams)(
		csSDK_uint32 inExporterPluginID,
		csSDK_uint32 inNumAudioStreams);

	/**
	**
	*/
	prSuiteError (*SetMaxNumAudioChannels)(
		csSDK_uint32 inExporterPluginID,
		csSDK_uint32 inNumAudioChannels);

	/**
	**
	*/
	prSuiteError (*GetAudioChannelLayout)(
		csSDK_uint32 inExporterPluginID,
		csSDK_uint32 inAudioStreamIndex,
		csSDK_uint32* ioNumChannels,
		PrAudioChannelLabel* outChannelLabels);

	/**
	**
	*/
	prSuiteError (*SetAudioChannelLayout)(
		csSDK_uint32 inExporterPluginID,
		csSDK_uint32 inAudioStreamIndex,
		csSDK_uint32 inNumChannels,
		const PrAudioChannelLabel* inChannelLabels);

	/**
	**
	*/
	prSuiteError (*SetAllowedChannelLayouts)(
		csSDK_uint32 inExporterPluginID,
		csSDK_uint32 inNumStreams,
		csSDK_uint32* inNumChannels,
		const PrAudioChannelLabel* inChannelLabels);

	/**
	**
	*/
	prSuiteError (*GetDescription)(
		csSDK_uint32 inExporterPluginID,
		PrSDKString* outDescriptionString);

	/**
	**
	*/
	prSuiteError (*RemoveAllStreams)(
		csSDK_uint32 inExporterPluginID);

} PrSDKExportAudioParamSuite;

#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif // PRSDKEXPORTAUDIOPARAMSUITE_H
