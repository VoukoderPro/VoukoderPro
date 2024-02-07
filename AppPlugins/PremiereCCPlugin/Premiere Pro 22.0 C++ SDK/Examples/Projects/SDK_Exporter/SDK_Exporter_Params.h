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

#include	<SDK_File.h>

#define		EXPORTER_NAME					"SDK Exporter"
#define		SETTING_STRING					"Turn on marker export and warnings? (Don't turn this on for rendering preview files)"
#define		ID_SLIDER						"SliderTest"
#define		ID_STRING						"StringTest"
#define		ID_CHECKBOX						"CheckboxTest"

// Parameter strings
#define		TOP_VIDEO_PARAM_GROUP_NAME		L"Sample top-level video param group"
#define		TOP_AUDIO_PARAM_GROUP_NAME		L"Sample top-level audio param group"
#define		VIDEO_CODEC_PARAM_GROUP_NAME	L"Video codec group"
#define		BASIC_VIDEO_PARAM_GROUP_NAME	L"Sample basic video param group"
#define		BASIC_AUDIO_PARAM_GROUP_NAME	L"Sample basic audio param group"
#define		STR_CODEC						L"Codec"
#define		STR_CODEC_TOOLTIP				L"The video codec to be used for encode."
#define		STR_CODEC_BUTTON				L"Codec Settings"
#define		STR_WIDTH						L"Width"
#define		STR_HEIGHT						L"Height"
#define		STR_FRAME_RATE					L"Frame Rate (fps)"
#define		STR_FRAME_RATE_10				L"10"
#define		STR_FRAME_RATE_15				L"15"
#define		STR_FRAME_RATE_23976			L"23.976"
#define		STR_FRAME_RATE_24				L"24"
#define		STR_FRAME_RATE_25				L"25 (PAL)"
#define		STR_FRAME_RATE_2997				L"29.97 (NTSC)"
#define		STR_FRAME_RATE_30				L"30"
#define		STR_FRAME_RATE_50				L"50"
#define		STR_FRAME_RATE_5994				L"59.94"
#define		STR_FRAME_RATE_60				L"60"
#define		STR_PAR							L"Pixel Aspect Ratio"
#define		STR_PAR_09						L"D1/DV NTSC (0.9091)"
#define		STR_PAR_10						L"Square pixels (1.0)"
#define		STR_PAR_11						L"D1/DV PAL (1.0940)"
#define		STR_PAR_12						L"D1/DV NTSC Widescreen 16:9 (1.2121)"
#define		STR_PAR_13						L"HD Anamorphic 1080 (1.3333)"
#define		STR_PAR_144						L"D1/DV PAL Widescreen 16:9 (1.4587)"
#define		STR_PAR_15						L"DVCPRO HD (1.5)"
#define		STR_PAR_20						L"Anamorphic 2:1 (2.0)"
#define		STR_FIELD_ORDER					L"Field Type"
#define		STR_FIELD_ORDER_UPPER			L"Upper First"
#define		STR_FIELD_ORDER_LOWER			L"Lower First"
#define		STR_FIELD_ORDER_NONE			L"None"
#define		STR_SLIDER						L"Slider"
#define		STR_STRING						L"String"
#define		STR_CHECKBOX					L"Checkbox"
#define		STR_SAMPLE_RATE					L"Sample Rate"
#define		STR_SAMPLE_RATE_8				L"8000 Hz"
#define		STR_SAMPLE_RATE_16				L"16000 Hz"
#define		STR_SAMPLE_RATE_32				L"32000 Hz"
#define		STR_SAMPLE_RATE_441				L"44100 Hz"
#define		STR_SAMPLE_RATE_48				L"48000 Hz"
#define		STR_SAMPLE_RATE_96				L"96000 Hz"
#define		STR_CHANNEL_TYPE				L"Channels"
#define		STR_CHANNEL_TYPE_MONO			L"Mono"
#define		STR_CHANNEL_TYPE_STEREO			L"Stereo"
#define		STR_CHANNEL_TYPE_51				L"5.1"


prMALError exSDKGenerateDefaultParams(
	exportStdParms				*stdParms, 
	exGenerateDefaultParamRec	*generateDefaultParamRec);

prMALError exSDKPostProcessParams (
	exportStdParms			*stdParmsP, 
	exPostProcessParamsRec	*postProcessParamsRecP);

prMALError exSDKGetParamSummary (
	exportStdParms			*stdParmsP, 
	exParamSummaryRec		*summaryRecP);

prMALError exSDKParamButton(
		exportStdParms		*stdParmsP, 
		exParamButtonRec	*getFilePrefsRecP);

prMALError exSDKValidateParamChanged (
	exportStdParms		*stdParmsP, 
	exParamChangedRec	*validateParamChangedRecP);
