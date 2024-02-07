/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 1992-2008 Adobe Systems Incorporated                  */
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

#include "SDK_Exporter_Params.h"

prMALError exSDKGenerateDefaultParams(
	exportStdParms				*stdParms, 
	exGenerateDefaultParamRec	*generateDefaultParamRec)
{
	prMALError				result				= malNoError;
	ExportSettings			*lRec				= reinterpret_cast<ExportSettings*>(generateDefaultParamRec->privateData);
	PrSDKExportParamSuite	*exportParamSuite	= lRec->exportParamSuite;
	PrSDKExportInfoSuite	*exportInfoSuite	= lRec->exportInfoSuite;
	PrSDKTimeSuite			*timeSuite			= lRec->timeSuite;
	csSDK_int32				exporterPluginID	= generateDefaultParamRec->exporterPluginID,
							mgroupIndex			= 0;
	PrParam					hasVideo,
							hasAudio,
							seqWidth,
							seqHeight,
							seqPARNum,
							seqPARDen,
							seqFrameRate,
							seqFieldOrder,
							seqChannelType,
							seqSampleRate;
	prUTF16Char				tempString[256];

	if (exportInfoSuite)
	{
		exportInfoSuite->GetExportSourceInfo(	exporterPluginID,
												kExportInfo_SourceHasVideo,
												&hasVideo);
		exportInfoSuite->GetExportSourceInfo(	exporterPluginID,
												kExportInfo_SourceHasAudio,
												&hasAudio);
		exportInfoSuite->GetExportSourceInfo(	exporterPluginID,
												kExportInfo_VideoWidth,
												&seqWidth);
		exportInfoSuite->GetExportSourceInfo(	exporterPluginID,
												kExportInfo_VideoHeight,
												&seqHeight);
		// When AME first initializes the exporter, it does so with a dummy sequence of width and height 0
		// In that case, set width and height to reasonable defaults
		// Otherwise, these 0 values will be returned to AME when exSelQueryOutputSettings is called,
		// and the preview will be turned off (1882928)
		if (seqWidth.mInt32 == 0)
		{
			seqWidth.mInt32 = 720;
		}
		if (seqHeight.mInt32 == 0)
		{
			seqHeight.mInt32 = 480;
		}
		exportInfoSuite->GetExportSourceInfo(	exporterPluginID,
												kExportInfo_VideoFrameRate,
												&seqFrameRate);
		exportInfoSuite->GetExportSourceInfo( exporterPluginID,
												kExportInfo_PixelAspectNumerator,
												&seqPARNum);
		exportInfoSuite->GetExportSourceInfo( exporterPluginID,
												kExportInfo_PixelAspectDenominator,
												&seqPARDen);
		exportInfoSuite->GetExportSourceInfo( exporterPluginID,
												kExportInfo_VideoFieldType,
												&seqFieldOrder);
		exportInfoSuite->GetExportSourceInfo( exporterPluginID,
												kExportInfo_AudioChannelsType,
												&seqChannelType);
		exportInfoSuite->GetExportSourceInfo( exporterPluginID,
												kExportInfo_AudioSampleRate,
												&seqSampleRate);

		// Stash off the source field type into our private data.
		// This is the time to get that information, rather than during exSelExport
		lRec->sourceFieldType = seqFieldOrder.mInt32;
	}
	if(exportParamSuite)
	{
		exportParamSuite->AddMultiGroup(exporterPluginID, &mgroupIndex);
		// Video parameters
		// Even though the real parameter strings are provided during exSelPostProcessParams,
		// we still need to provide them here too, otherwise AddParamGroup won't work
		copyConvertStringLiteralIntoUTF16(TOP_VIDEO_PARAM_GROUP_NAME, tempString);
		exportParamSuite->AddParamGroup(	exporterPluginID,
											mgroupIndex,
											ADBETopParamGroup,
											ADBEVideoTabGroup,
											tempString,
											kPrFalse,
											kPrFalse,
											kPrFalse);
		// Codec Param Group
		copyConvertStringLiteralIntoUTF16(VIDEO_CODEC_PARAM_GROUP_NAME, tempString);
		exportParamSuite->AddParamGroup(	exporterPluginID,
											mgroupIndex,
											ADBEVideoTabGroup,
											ADBEVideoCodecGroup,
											tempString,
											kPrFalse,
											kPrFalse,
											kPrFalse);
		// Codec
		exNewParamInfo codecParam;
		exParamValues codecValues;
		safeStrCpy(codecParam.identifier, 256, ADBEVideoCodec);
		codecParam.paramType = exParamType_int;
		codecParam.flags = exParamFlag_none;
		codecValues.rangeMin.intValue = 0;
		codecValues.rangeMax.intValue = 0;
		codecValues.value.intValue = SDK_8_BIT_RGB;
		codecValues.disabled = kPrFalse;
		codecValues.hidden = kPrFalse;
		codecParam.paramValues = codecValues;
		exportParamSuite->AddParam(	exporterPluginID,
										mgroupIndex,
										ADBEVideoCodecGroup,
										&codecParam);
		// Codec button
		exNewParamInfo codecButtonParam;
		exParamValues codecButtonValues;
		safeStrCpy(codecButtonParam.identifier, 256, ADBEVideoCodecPrefsButton);
		codecButtonParam.paramType = exParamType_button;
		codecButtonParam.flags = exParamFlag_none;
		codecButtonValues.disabled = kPrFalse;
		codecButtonValues.hidden = kPrFalse;
		codecButtonValues.arbData = 0;
		codecButtonValues.arbDataSize = sizeof(CodecSettings);
		codecButtonParam.paramValues = codecButtonValues;
		exportParamSuite->AddParam(	exporterPluginID,
										mgroupIndex,
										ADBEVideoCodecGroup,
										&codecButtonParam);
		// Basic Video Param Group
		copyConvertStringLiteralIntoUTF16(BASIC_VIDEO_PARAM_GROUP_NAME, tempString);
		exportParamSuite->AddParamGroup(	exporterPluginID,
											mgroupIndex,
											ADBEVideoTabGroup,
											ADBEBasicVideoGroup,
											tempString,
											kPrFalse,
											kPrFalse,
											kPrFalse);
		// Width
		exNewParamInfo widthParam;
		exParamValues widthValues;
		safeStrCpy(widthParam.identifier, 256, ADBEVideoWidth);
		widthParam.paramType = exParamType_int;
		widthParam.flags = exParamFlag_none;
		widthValues.rangeMin.intValue = 16;
		widthValues.rangeMax.intValue = 4096;
		widthValues.value.intValue = seqWidth.mInt32;
		widthValues.disabled = kPrFalse;
		widthValues.hidden = kPrFalse;
		widthParam.paramValues = widthValues;
		exportParamSuite->AddParam(	exporterPluginID,
										mgroupIndex,
										ADBEBasicVideoGroup,
										&widthParam);
		// Height
		exNewParamInfo heightParam;
		exParamValues heightValues;
		safeStrCpy(heightParam.identifier, 256, ADBEVideoHeight);
		heightParam.paramType = exParamType_int;
		heightParam.flags = exParamFlag_none;
		heightValues.rangeMin.intValue = 16;
		heightValues.rangeMax.intValue = 4096;
		heightValues.value.intValue = seqHeight.mInt32;
		heightValues.disabled = kPrFalse;
		heightValues.hidden = kPrFalse;
		heightParam.paramValues = heightValues;
		exportParamSuite->AddParam(	exporterPluginID,
										mgroupIndex,
										ADBEBasicVideoGroup,
										&heightParam);
		// Pixel aspect ratio
		exNewParamInfo PARParam;
		exParamValues PARValues;
		safeStrCpy(PARParam.identifier, 256, ADBEVideoAspect);
		PARParam.paramType = exParamType_ratio;
		PARParam.flags = exParamFlag_none;
		PARValues.rangeMin.ratioValue.numerator = 10;
		PARValues.rangeMin.ratioValue.denominator = 11;
		PARValues.rangeMax.ratioValue.numerator = 2;
		PARValues.rangeMax.ratioValue.denominator = 1;
		PARValues.value.ratioValue.numerator = seqPARNum.mInt32;
		PARValues.value.ratioValue.denominator = seqPARDen.mInt32;
		PARValues.disabled = kPrFalse;
		PARValues.hidden = kPrFalse;
		PARParam.paramValues = PARValues;
		exportParamSuite->AddParam(	exporterPluginID,
										mgroupIndex,
										ADBEBasicVideoGroup,
										&PARParam);
		// Frame rate
		exNewParamInfo frameRateParam;
		exParamValues frameRateValues;
		safeStrCpy(frameRateParam.identifier, 256, ADBEVideoFPS);
		frameRateParam.paramType = exParamType_ticksFrameRate;
		frameRateParam.flags = exParamFlag_none;
		frameRateValues.rangeMin.timeValue = 1;
		timeSuite->GetTicksPerSecond (&frameRateValues.rangeMax.timeValue);
		frameRateValues.value.timeValue = seqFrameRate.mInt64;
		frameRateValues.disabled = kPrFalse;
		frameRateValues.hidden = kPrFalse;
		frameRateParam.paramValues = frameRateValues;
		exportParamSuite->AddParam(	exporterPluginID,
										mgroupIndex,
										ADBEBasicVideoGroup,
										&frameRateParam);
		// Field order
		exNewParamInfo fieldOrderParam;
		exParamValues fieldOrderValues;
		safeStrCpy(fieldOrderParam.identifier, 256, ADBEVideoFieldType);
		fieldOrderParam.paramType = exParamType_int;
		fieldOrderParam.flags = exParamFlag_none;
		frameRateValues.value.intValue = seqFieldOrder.mInt32;
		fieldOrderValues.disabled = kPrFalse;
		fieldOrderValues.hidden = kPrFalse;
		fieldOrderParam.paramValues = fieldOrderValues;
		exportParamSuite->AddParam(	exporterPluginID,
										mgroupIndex,
										ADBEBasicVideoGroup,
										&fieldOrderParam);

		// Video Bit Rate Encoding
		exNewParamInfo sliderParam;
		exParamValues sliderValues;
		safeStrCpy(sliderParam.identifier, 256, ID_SLIDER);
		sliderParam.paramType				= exParamType_float;
		sliderParam.flags					= exParamFlag_slider;
		sliderValues.value.floatValue		= 15.0; // Default Value
		sliderValues.disabled				= kPrFalse;
		sliderValues.hidden					= kPrFalse;
		sliderParam.paramValues				= sliderValues;
		exportParamSuite->AddParam(	exporterPluginID,
										mgroupIndex,
										ADBEBasicVideoGroup,
										&sliderParam);

		// Sample string
		exNewParamInfo stringParam;
		exParamValues stringValues;
		safeStrCpy(stringParam.identifier, 256, ID_STRING);
		stringParam.paramType				= exParamType_string;
		stringParam.flags					= exParamFlag_none;
		stringValues.value.intValue			= 0; // Default Value
		stringValues.disabled				= kPrFalse;
		stringValues.hidden					= kPrFalse;
		stringParam.paramValues				= stringValues;
		exportParamSuite->AddParam(	exporterPluginID,
										mgroupIndex,
										ADBEBasicVideoGroup,
										&stringParam);

		// Sample checkbox
		exNewParamInfo checkboxParam;
		exParamValues checkboxValues;
		safeStrCpy(checkboxParam.identifier, 256, ID_CHECKBOX);
		checkboxParam.paramType				= exParamType_bool;
		checkboxParam.flags					= exParamFlag_none;
		checkboxValues.value.intValue		= kPrFalse; // Default Value
		checkboxValues.disabled				= kPrFalse;
		checkboxValues.hidden				= kPrFalse;
		checkboxParam.paramValues			= checkboxValues;
		exportParamSuite->AddParam(	exporterPluginID,
										mgroupIndex,
										ADBEBasicVideoGroup,
										&checkboxParam);

		// Audio parameters
		copyConvertStringLiteralIntoUTF16(TOP_AUDIO_PARAM_GROUP_NAME, tempString);
		exportParamSuite->AddParamGroup(	exporterPluginID,
											mgroupIndex,
											ADBETopParamGroup,
											ADBEAudioTabGroup,
											tempString,
											kPrFalse,
											kPrFalse,
											kPrFalse);
		copyConvertStringLiteralIntoUTF16(BASIC_AUDIO_PARAM_GROUP_NAME, tempString);
		exportParamSuite->AddParamGroup(	exporterPluginID,
											mgroupIndex,
											ADBEAudioTabGroup,
											ADBEBasicAudioGroup,
											tempString,
											kPrFalse,
											kPrFalse,
											kPrFalse);
		// Sample rate
		exNewParamInfo sampleRateParam;
		exParamValues sampleRateValues;
		safeStrCpy(sampleRateParam.identifier, 256, ADBEAudioRatePerSecond);
		sampleRateParam.paramType = exParamType_float;
		sampleRateParam.flags = exParamFlag_none;
		sampleRateValues.value.floatValue = seqSampleRate.mFloat64;
		sampleRateValues.disabled = kPrFalse;
		sampleRateValues.hidden = kPrFalse;
		sampleRateParam.paramValues = sampleRateValues;
		exportParamSuite->AddParam(	exporterPluginID,
										mgroupIndex,
										ADBEBasicAudioGroup,
										&sampleRateParam);
		// Channel type
		exNewParamInfo channelTypeParam;
		exParamValues channelTypeValues;
		safeStrCpy(channelTypeParam.identifier, 256, ADBEAudioNumChannels);
		channelTypeParam.paramType = exParamType_int;
		channelTypeParam.flags = exParamFlag_none;
		channelTypeValues.value.intValue = seqChannelType.mInt32;
		channelTypeValues.disabled = kPrFalse;
		channelTypeValues.hidden = kPrFalse;
		channelTypeParam.paramValues = channelTypeValues;
		exportParamSuite->AddParam(	exporterPluginID,
										mgroupIndex,
										ADBEBasicAudioGroup,
										&channelTypeParam);

		// [TODO - ZL] Add more params: 8-bit vs 32-bit processing

		//PostProcessParams(exporterPluginID, exportParamSuite);
		exportParamSuite->SetParamsVersion(exporterPluginID, 2);//kMyParamsVersion);
	}

	return result;
}


// Need to give parameters, parameter groups, and constrained values their names here
prMALError exSDKPostProcessParams (
	exportStdParms			*stdParmsP, 
	exPostProcessParamsRec	*postProcessParamsRecP)
{
	prMALError		result	= malNoError;
	csSDK_uint32	exID	= postProcessParamsRecP->exporterPluginID;
	ExportSettings	*lRec	= reinterpret_cast<ExportSettings*>(postProcessParamsRecP->privateData);
	exOneParamValueRec		tempCodec;
	csSDK_int32				codecs[]			= {SDK_8_BIT_RGB, SDK_10_BIT_YUV};
	exOneParamValueRec		tempPAR;
	csSDK_int32				PARs[][2]			= {{1, 1}, {10, 11}, {40, 33}, {768, 702}, 
													{1024, 702}, {2, 1}, {4, 3}, {3, 2}};
	PrTime					ticksPerSecond		= 0;
	exOneParamValueRec		tempFrameRate;
	PrTime					frameRates[]		= {10, 15, 23,
													24, 25, 29,
													30, 50, 59,
													60};
	PrTime					frameRateNumDens[][2]= {{10, 1}, {15, 1}, {24000, 1001},
													{24, 1}, {25, 1}, {30000, 1001},
													{30, 1}, {50, 1}, {60000, 1001},
													{60, 1}};
	exOneParamValueRec		tempFieldOrder;
	csSDK_int32				fieldOrders[]		= {prFieldsUpperFirst, prFieldsLowerFirst,
													prFieldsNone};
	exOneParamValueRec		tempSampleRate;
	float					sampleRates[]		= {8000.0f, 16000.0f, 32000.0f, 44100.0f, 48000.0f, 96000.0f};
	exOneParamValueRec		tempChannelType;
	csSDK_int32				channelTypes[]		= {kPrAudioChannelType_Mono, kPrAudioChannelType_Stereo,
													kPrAudioChannelType_51};
	CodecSettings			codecSettings;
	csSDK_int32				codecSettingsSize	= static_cast<csSDK_int32>(sizeof(CodecSettings));

	// Initialize arrays of strings
	prUTF16Char				tempString[256];
	const wchar_t*			codecStrings[]		= {	SDK_8_BIT_RGB_NAME, SDK_10_BIT_YUV_NAME};
	const wchar_t*			PARStrings[]		= {	STR_PAR_10, STR_PAR_09, STR_PAR_12, STR_PAR_11, 
													STR_PAR_144, STR_PAR_20, STR_PAR_13, STR_PAR_15};
	const wchar_t*			frameRateStrings[]	= {	STR_FRAME_RATE_10, STR_FRAME_RATE_15, STR_FRAME_RATE_23976,
													STR_FRAME_RATE_24, STR_FRAME_RATE_25, STR_FRAME_RATE_2997,
													STR_FRAME_RATE_30, STR_FRAME_RATE_50, STR_FRAME_RATE_5994,
													STR_FRAME_RATE_60};
	const wchar_t*			fieldOrderStrings[]	= {	STR_FIELD_ORDER_UPPER, STR_FIELD_ORDER_LOWER,
													STR_FIELD_ORDER_NONE};
	const wchar_t*			sampleRateStrings[]	= {	STR_SAMPLE_RATE_8, STR_SAMPLE_RATE_16,
													STR_SAMPLE_RATE_32, STR_SAMPLE_RATE_441,
													STR_SAMPLE_RATE_48, STR_SAMPLE_RATE_96};
	const wchar_t*			channelTypeStrings[]= {	STR_CHANNEL_TYPE_MONO, STR_CHANNEL_TYPE_STEREO,
													STR_CHANNEL_TYPE_51};

	lRec->timeSuite->GetTicksPerSecond (&ticksPerSecond);
	for (csSDK_int32 i = 0; i < sizeof(frameRates) / sizeof (PrTime); i++)
	{
		frameRates[i] = ticksPerSecond / frameRateNumDens[i][0] * frameRateNumDens[i][1];
	}

	copyConvertStringLiteralIntoUTF16(VIDEO_CODEC_PARAM_GROUP_NAME, tempString);
	lRec->exportParamSuite->SetParamName(exID, 0, ADBEVideoCodecGroup, tempString);

	copyConvertStringLiteralIntoUTF16(STR_CODEC, tempString);
	lRec->exportParamSuite->SetParamName(exID, 0, ADBEVideoCodec, tempString);
	copyConvertStringLiteralIntoUTF16(STR_CODEC_TOOLTIP, tempString);
	lRec->exportParamSuite->SetParamDescription(exID, 0, ADBEVideoCodec, tempString);

	copyConvertStringLiteralIntoUTF16(STR_CODEC_BUTTON, tempString);
	lRec->exportParamSuite->SetParamName(exID, 0, ADBEVideoCodecPrefsButton, tempString);

	copyConvertStringLiteralIntoUTF16(BASIC_VIDEO_PARAM_GROUP_NAME, tempString);
	lRec->exportParamSuite->SetParamName(exID, 0, ADBEBasicVideoGroup, tempString);

	copyConvertStringLiteralIntoUTF16(STR_WIDTH, tempString);
	lRec->exportParamSuite->SetParamName(exID, 0, ADBEVideoWidth, tempString);

	copyConvertStringLiteralIntoUTF16(STR_HEIGHT, tempString);
	lRec->exportParamSuite->SetParamName(exID, 0, ADBEVideoHeight, tempString);

	copyConvertStringLiteralIntoUTF16(STR_PAR, tempString);
	lRec->exportParamSuite->SetParamName(exID, 0, ADBEVideoAspect, tempString);

	copyConvertStringLiteralIntoUTF16(STR_FRAME_RATE, tempString); 
	lRec->exportParamSuite->SetParamName(exID, 0, ADBEVideoFPS, tempString);

	copyConvertStringLiteralIntoUTF16(STR_FIELD_ORDER, tempString);
	lRec->exportParamSuite->SetParamName(exID, 0, ADBEVideoFieldType, tempString);

	copyConvertStringLiteralIntoUTF16(STR_SLIDER, tempString);
	lRec->exportParamSuite->SetParamName(exID, 0, ID_SLIDER, tempString);

	copyConvertStringLiteralIntoUTF16(STR_STRING, tempString);
	lRec->exportParamSuite->SetParamName(exID, 0, ID_STRING, tempString);

	copyConvertStringLiteralIntoUTF16(STR_CHECKBOX, tempString);
	lRec->exportParamSuite->SetParamName(exID, 0, ID_CHECKBOX, tempString);

	copyConvertStringLiteralIntoUTF16(BASIC_AUDIO_PARAM_GROUP_NAME, tempString);
	lRec->exportParamSuite->SetParamName(exID, 0, ADBEBasicAudioGroup, tempString);

	copyConvertStringLiteralIntoUTF16(STR_SAMPLE_RATE, tempString);
	lRec->exportParamSuite->SetParamName(exID, 0, ADBEAudioRatePerSecond, tempString);

	copyConvertStringLiteralIntoUTF16(STR_CHANNEL_TYPE, tempString);
	lRec->exportParamSuite->SetParamName(exID, 0, ADBEAudioNumChannels, tempString);

	lRec->exportParamSuite->ClearConstrainedValues(	exID,
													0,
													ADBEVideoCodec);
	for (csSDK_int32 i = 0; i < sizeof (codecs) / sizeof (csSDK_int32); i++)
	{
		tempCodec.intValue = codecs[i];
		copyConvertStringLiteralIntoUTF16(codecStrings[i], tempString);
		lRec->exportParamSuite->AddConstrainedValuePair(	exID,
															0,
															ADBEVideoCodec,
															&tempCodec,
															tempString);
	}
	lRec->exportParamSuite->ClearConstrainedValues(	exID,
													0,
													ADBEVideoAspect);
	for (csSDK_int32 i = 0; i < sizeof (PARs) / sizeof(PARs[0]); i++)
	{
		tempPAR.ratioValue.numerator = PARs[i][0];
		tempPAR.ratioValue.denominator = PARs[i][1];
		copyConvertStringLiteralIntoUTF16(PARStrings[i], tempString);
		lRec->exportParamSuite->AddConstrainedValuePair(	exID,
															0,
															ADBEVideoAspect,
															&tempPAR,
															tempString);
	}
	lRec->exportParamSuite->ClearConstrainedValues(	exID,
													0,
													ADBEVideoFPS);
	for (csSDK_int32 i = 0; i < sizeof(frameRates) / sizeof (PrTime); i++)
	{
		tempFrameRate.timeValue = frameRates[i];
		copyConvertStringLiteralIntoUTF16(frameRateStrings[i], tempString);
		lRec->exportParamSuite->AddConstrainedValuePair(	exID,
															0,
															ADBEVideoFPS,
															&tempFrameRate,
															tempString);
	}
	lRec->exportParamSuite->ClearConstrainedValues(	exID,
													0,
													ADBEVideoFieldType);
	for (csSDK_int32 i = 0; i < 3; i++)
	{
		tempFieldOrder.intValue = fieldOrders[i];
		copyConvertStringLiteralIntoUTF16(fieldOrderStrings[i], tempString);
		lRec->exportParamSuite->AddConstrainedValuePair(	exID,
															0,
															ADBEVideoFieldType,
															&tempFieldOrder,
															tempString);
	}
	lRec->exportParamSuite->ClearConstrainedValues(	exID,
													0,
													ADBEAudioRatePerSecond);
	for (csSDK_int32 i = 0; i < sizeof(sampleRates) / sizeof (float); i++)
	{
		tempSampleRate.floatValue = sampleRates[i];
		copyConvertStringLiteralIntoUTF16(sampleRateStrings[i], tempString);
		lRec->exportParamSuite->AddConstrainedValuePair(	exID,
															0,
															ADBEAudioRatePerSecond,
															&tempSampleRate,
															tempString);
	}
	lRec->exportParamSuite->ClearConstrainedValues(	exID,
													0,
													ADBEAudioNumChannels);
	for (csSDK_int32 i = 0; i < sizeof(channelTypes) / sizeof (csSDK_int32); i++)
	{
		tempChannelType.intValue = channelTypes[i];
		copyConvertStringLiteralIntoUTF16(channelTypeStrings[i], tempString);
		lRec->exportParamSuite->AddConstrainedValuePair(	exID,
															0,
															ADBEAudioNumChannels,
															&tempChannelType,
															tempString);
	}

	// Set the slider ranges, while preserving the current value
	exParamValues sliderValues;
	lRec->exportParamSuite->GetParamValue(exID, 0, ID_SLIDER, &sliderValues);
	sliderValues.rangeMin.floatValue	= 1.0;
	sliderValues.rangeMax.floatValue	= 50.0;
	sliderValues.disabled				= kPrFalse;
	sliderValues.hidden					= kPrFalse;
	lRec->exportParamSuite->ChangeParam(exID,
										0,
										ID_SLIDER,
										&sliderValues);

	lRec->exportParamSuite->GetArbData(	exID,
										0,
										ADBEVideoCodecPrefsButton,
										&codecSettingsSize,
										NULL);
	if (codecSettingsSize)
	{
		// Settings valid.  Let's get them.
		lRec->exportParamSuite->GetArbData(exID,
											0,
											ADBEVideoCodecPrefsButton,
											&codecSettingsSize,
											reinterpret_cast<void*>(&codecSettings));
	}
	else
	{
		// Settings invalid.  Let's set default ones.
		codecSettings.sampleSetting = kPrFalse;
		codecSettingsSize = static_cast<csSDK_int32>(sizeof(CodecSettings));
		lRec->exportParamSuite->SetArbData(	exID,
											0,
											ADBEVideoCodecPrefsButton,
											codecSettingsSize,
											reinterpret_cast<void*>(&codecSettings));
	}
	return result;
}


prMALError exSDKGetParamSummary (
	exportStdParms			*stdParmsP, 
	exParamSummaryRec		*summaryRecP)
{
	prMALError		result	= malNoError;
	wchar_t			videoSummary[256],
					audioSummary[256],
					bitrateSummary[256];
	exParamValues	width,
					height,
					frameRate,
					pixelAspectRatio,
					sampleRate,
					channelType;
	ExportSettings	*lRec	= reinterpret_cast<ExportSettings*>(summaryRecP->privateData);
	PrSDKExportParamSuite	*paramSuite	= lRec->exportParamSuite;
	PrSDKTimeSuite			*timeSuite	= lRec->timeSuite;
	PrTime			ticksPerSecond;
	csSDK_int32		mgroupIndex = 0,
					exporterPluginID	= summaryRecP->exporterPluginID;

	if (paramSuite)
	{
		paramSuite->GetParamValue(exporterPluginID, mgroupIndex, ADBEVideoWidth, &width);
		paramSuite->GetParamValue(exporterPluginID, mgroupIndex, ADBEVideoHeight, &height);
		paramSuite->GetParamValue(exporterPluginID, mgroupIndex, ADBEVideoFPS, &frameRate);
		paramSuite->GetParamValue(exporterPluginID, mgroupIndex, ADBEVideoAspect, &pixelAspectRatio);
		paramSuite->GetParamValue(exporterPluginID, mgroupIndex, ADBEAudioRatePerSecond, &sampleRate);
		paramSuite->GetParamValue(exporterPluginID, mgroupIndex, ADBEAudioNumChannels, &channelType);
		timeSuite->GetTicksPerSecond(&ticksPerSecond);
	
		swprintf(videoSummary, 256, L"%ix%i, %.2f fps, %.4f PAR",
					width.value.intValue,
					height.value.intValue,
					static_cast<float>(ticksPerSecond) / static_cast<float>(frameRate.value.timeValue),
					static_cast<float>(pixelAspectRatio.value.ratioValue.numerator) /
						static_cast<float>(pixelAspectRatio.value.ratioValue.denominator));
		copyConvertStringLiteralIntoUTF16(videoSummary, summaryRecP->videoSummary);
		swprintf(audioSummary, 20, L"%.0f Hz, ",
					sampleRate.value.floatValue);
		switch(channelType.value.intValue)
		{
		case kPrAudioChannelType_Mono:
			safeWcscat(audioSummary, 256, STR_CHANNEL_TYPE_MONO);
			break;
		case kPrAudioChannelType_Stereo:
			safeWcscat(audioSummary, 256, STR_CHANNEL_TYPE_STEREO);
			break;
		case kPrAudioChannelType_51:
			safeWcscat(audioSummary, 256, STR_CHANNEL_TYPE_51);
			break;
		default:
			safeWcscat(audioSummary, 256, L"Unknown channel type");
		}
		copyConvertStringLiteralIntoUTF16(audioSummary, summaryRecP->audioSummary);

		// Nothing to put in the bitrate summary
		swprintf(bitrateSummary, 256, L"Test bitrate summary");
		copyConvertStringLiteralIntoUTF16(bitrateSummary, summaryRecP->bitrateSummary);
	}
	return result;
}


// Pop any custom settings UI, since the user has just clicked on the Configure button
// in the Sequence Settings dialog, or the Codec Settings button in the Export Movie dialog.
// Save the user's choices for use during export
prMALError exSDKParamButton(
		exportStdParms		*stdParmsP, 
		exParamButtonRec	*getFilePrefsRecP)
{
	prMALError		result				= malNoError;
	ExportSettings	*lRec				= reinterpret_cast<ExportSettings*>(getFilePrefsRecP->privateData);
	CodecSettings	codecSettings;
	csSDK_int32		codecSettingsSize	= static_cast<csSDK_int32>(sizeof(CodecSettings));
	#ifdef PRWIN_ENV
	csSDK_int32		returnValue			= 0;
	#endif

	lRec->exportParamSuite->GetArbData(getFilePrefsRecP->exporterPluginID,
											getFilePrefsRecP->multiGroupIndex,
											getFilePrefsRecP->buttonParamIdentifier,
											&codecSettingsSize,
											NULL);
	if (codecSettingsSize)
	{
		// Settings valid.  Let's get them.
		lRec->exportParamSuite->GetArbData(getFilePrefsRecP->exporterPluginID,
											getFilePrefsRecP->multiGroupIndex,
											getFilePrefsRecP->buttonParamIdentifier,
											&codecSettingsSize,
											reinterpret_cast<void*>(&codecSettings));
	}
	else
	{
		codecSettings.sampleSetting = kPrFalse;
		codecSettingsSize = static_cast<csSDK_int32>(sizeof(CodecSettings));
	}

	#ifdef PRMAC_ENV
	/*
	CFStringRef				exporterName		= CFSTR (EXPORTER_NAME),
							settingString		= CFSTR (SETTING_STRING),
							yesString			= CFSTR ("Yes"),
							noString			= CFSTR ("No");
	DialogRef				alert;
	*/
	DialogItemIndex			outItemHit;
	/*
	AlertStdCFStringAlertParamRec inAlertParam = {	kStdCFStringAlertVersionOne,
													kPrTrue,
													kPrFalse,
													yesString,
													noString,
													NULL,
													kAlertStdAlertOKButton,
													kAlertStdAlertCancelButton,
													kWindowDefaultPosition,
													NULL};
	 */
	#endif

	// Show sample dialog and modify export settings
	#ifdef PRWIN_ENV
	HWND mainWnd = lRec->windowSuite->GetMainWindow();
	returnValue = MessageBox(	GetLastActivePopup(mainWnd),
								SETTING_STRING,
								EXPORTER_NAME,
								MB_YESNOCANCEL);
	#else
	
/*	[TODO] Will need to use Cocoa with NSAlertPanel
	returnValue = CreateStandardAlert (	kAlertNoteAlert,
										exporterName,
										settingString,
										&inAlertParam,
										&alert);
	returnValue = RunStandardAlert (alert, NULL, &outItemHit);
	CFRelease (exporterName);
	CFRelease (settingString);
*/	#endif
	
	#ifdef PRWIN_ENV
	if (returnValue == IDYES)
	#else
	if (outItemHit == kAlertStdAlertOKButton)
	#endif
	{
		codecSettings.sampleSetting = kPrTrue;
	}
	#ifdef PRWIN_ENV
	else if (returnValue == IDNO)
	#else
	else if (outItemHit == kAlertStdAlertOtherButton)
	#endif
	{
		codecSettings.sampleSetting = kPrFalse;
	}
	else
	{
		// If user cancelled, return this value so that host knows nothing changed
		// If the user has a preset selected, hits Codec Settings, and cancels,
		// this ensures that the preset remains selected rather than changing to Custom.
		result = exportReturn_ParamButtonCancel;
	}

	lRec->exportParamSuite->SetArbData(getFilePrefsRecP->exporterPluginID,
											getFilePrefsRecP->multiGroupIndex,
											getFilePrefsRecP->buttonParamIdentifier,
											codecSettingsSize,
											reinterpret_cast<void*>(&codecSettings));
	return result;
}


// An exporter can monitor parameter changes, and modify other parameters based on the changes
// Currently this code doesn't do any of that
prMALError exSDKValidateParamChanged (
	exportStdParms		*stdParmsP, 
	exParamChangedRec	*validateParamChangedRecP)
{
	prMALError				result			= malNoError;
	csSDK_uint32			exID			= validateParamChangedRecP->exporterPluginID;
	ExportSettings			*lRec			= reinterpret_cast<ExportSettings*>(validateParamChangedRecP->privateData);
	exParamValues			changedValue;

	if (lRec->exportParamSuite)
	{
		if (strcmp(validateParamChangedRecP->changedParamIdentifier, ADBEVideoWidth) == 0)
		{
			lRec->exportParamSuite->GetParamValue(exID,
													validateParamChangedRecP->multiGroupIndex,
													validateParamChangedRecP->changedParamIdentifier,
													&changedValue);
			lRec->SDKFileRec.width = changedValue.value.intValue;
		}
		else if (strcmp(validateParamChangedRecP->changedParamIdentifier, ADBEVideoHeight) == 0)
		{
			lRec->exportParamSuite->GetParamValue(exID,
													validateParamChangedRecP->multiGroupIndex,
													validateParamChangedRecP->changedParamIdentifier,
													&changedValue);
			lRec->SDKFileRec.height = changedValue.value.intValue;
		}
		else if (strcmp(validateParamChangedRecP->changedParamIdentifier, ADBEVideoAspect) == 0)
		{
			lRec->exportParamSuite->GetParamValue(exID,
													validateParamChangedRecP->multiGroupIndex,
													validateParamChangedRecP->changedParamIdentifier,
													&changedValue);
			lRec->SDKFileRec.pixelAspectNum = changedValue.value.ratioValue.numerator;
			lRec->SDKFileRec.pixelAspectDen = changedValue.value.ratioValue.denominator;
		}
		else if (strcmp(validateParamChangedRecP->changedParamIdentifier, ADBEVideoFPS) == 0)
		{
			lRec->exportParamSuite->GetParamValue(exID,
													validateParamChangedRecP->multiGroupIndex,
													validateParamChangedRecP->changedParamIdentifier,
													&changedValue);
			/* [TODO - ZL] Need to convert ticks to frame rate
			lRec->SDKFileRec.value = changedValue.value.timeValue;
			lRec->SDKFileRec.sampleSize = changedValue.value.timeValue;
			*/
		}
		else if (strcmp(validateParamChangedRecP->changedParamIdentifier, ID_SLIDER) == 0)
		{
			lRec->exportParamSuite->GetParamValue(exID,
													validateParamChangedRecP->multiGroupIndex,
													validateParamChangedRecP->changedParamIdentifier,
													&changedValue);
    }
		else if (strcmp(validateParamChangedRecP->changedParamIdentifier, ADBEAudioRatePerSecond) == 0)
		{
			lRec->exportParamSuite->GetParamValue(exID,
													validateParamChangedRecP->multiGroupIndex,
													validateParamChangedRecP->changedParamIdentifier,
													&changedValue);
			lRec->SDKFileRec.sampleRate = changedValue.value.floatValue;
		}
	}
	else
	{
		result = exportReturn_ErrMemory;
	}

	return result;
}
