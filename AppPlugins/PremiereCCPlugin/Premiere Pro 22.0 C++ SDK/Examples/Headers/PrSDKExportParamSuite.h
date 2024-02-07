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

//	This file is part of the public Premiere SDK.


#ifndef	PRSDKEXPORTPARAMSUITE_H
#define PRSDKEXPORTPARAMSUITE_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKEXPORT_H
#include "PrSDKExport.h"
#endif

#ifndef PRSDKMALERRORS_H
#include "PrSDKMALErrors.h"
#endif

#ifndef PRSDKTIMESUITE_H
#include "PrSDKTimeSuite.h"
#endif

#ifndef PRSDKSTRINGSUITE_H
#include "PrSDKStringSuite.h"
#endif

#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif

/**
**	Version 2 has the same functions as version one, except with different
**	host handling of the flags field in AddParam
**
**	Version 3 adds:
	SetConstrainedListIsOptional
	RestoreStandardParamToInitialState
	GetConstrainedValueDisplayName
** Version 4 adds:
	SetParamDescription
** Version 5 adds:
	MoveParamToGroup
** Version 7 adds:
	RemoveParam
*/

#define kPrSDKExportParamSuite				"MediaCore Export Param Suite"
#define kPrSDKExportParamSuiteVersion1		1
#define kPrSDKExportParamSuiteVersion2		2
#define kPrSDKExportParamSuiteVersion3		3
#define kPrSDKExportParamSuiteVersion4		4
#define kPrSDKExportParamSuiteVersion5		5
#define kPrSDKExportParamSuiteVersion6		6
#define kPrSDKExportParamSuiteVersion7		7
#define kPrSDKExportParamSuiteVersion		kPrSDKExportParamSuiteVersion7


//------------------------
// params
//------------------------

// params are grouped by
// Multigroup (always top-level, and not user visible. See note on Multigroups, below)
//	group	  (there's always at least one top-level group)
//		group
//			param
//
//	e.g.
//	Multigroup
//		group = video
//			group = "basic video settings"
//				param:	quality	int|Slider	1-10000
//				param:	width	int			32-4000




/* 
* Common Params identifiers.
* Please use these for identifiers that the application can override at setup or export time.
* 
* note: these params can be hidden if a different interface is needed (e.g. enum for FPS) in plug-in;
* the exporter plug-in will need to 'shadow' these values into their UI if they change, or 
* replace Common Param values that are incompatible with the closest equivalent for the export
*
* [FIX ME] Check for existance of some of these when params are instantiated from plug-in
*/


// top level groups
#define ADBEVideoTabGroup					"ADBEVideoTabGroup"
#define ADBEAudioTabGroup					"ADBEAudioTabGroup"

// video groups
#define ADBEBasicVideoGroup					"ADBEBasicVideoGroup"
#define ADBEVideoCodecGroup					"ADBEVideoCodecGroup"

// audio basic group
#define ADBEBasicAudioGroup					"ADBEBasicAudioGroup"
#define ADBEAudioCodecGroup					"ADBEAudioCodecGroup"
#define ADBEAudioBitrateGroup				"ADBEAudioBitrateGroup"
#define ADBEAudioChannelConfigurationGroup	"ADBEAudioChannelConfigurationGroup"
#define ADBEAudioAdvancedSettingsGroup		"ADBEAudioAdvancedSettingsGroup"

// video bitrate group
#define ADBEVideoBitrateGroup			"ADBEVideoBitrateGroup"

#define ADBEVideoBitDepth				"ADBEVideoBitDepth"			// int - 0 = 8bit, 1=16bit, 2=24bit, 3=32bit
#define ADBEVideoBounds					"ADBEVideoBounds"			// int enum, shadows ADBEVideoWidth, ADBEVideoHeight, which can still be automated directly.
#define ADBEVideoBitDepth8BitGray		"ADBEVideoBitDepth8BitGray" // 0 or 1 (disabled / enabled) if a codec supports it
#define ADBEVideoAlphaType				"ADBEVideoAlphaType"		// 0 is straight 1 is premul
#define ADBEVideoAlphaMatteColor		"ADBEVideoAlphaMatteColor"	// a prcolor in the int field
#define ADBEVideoAlphaInvert			"ADBEVideoAlphaInvert"		// 0 is normal 1 is invert
#define ADBEVideoWidth					"ADBEVideoWidth"
#define ADBEVideoHeight					"ADBEVideoHeight"
#define	ADBEVideoQuality				"ADBEVideoQuality"
#define ADBEVideoFieldType				"ADBEVideoFieldType"		// progressive=0, upperFirst=1, lowerFirst=2
#define ADBEVideoDeinterlaceType		"ADBEVideoDeinterlaceType"	// off, lowQualityDeinterlace=1, hiQualityDeinterlace=2
#define ADBEVideoCodec					"ADBEVideoCodec"
#define ADBEVideoCodec_Unabridged		"ADBEVideoCodec_Unabridged"	// constrained list of codecs including sequence preview preset codecs that do not appear in the ADBEVideoCodec list
#define	ADBEVideoFPS					"ADBEVideoFPS"
#define	ADBEVideoAspect					"ADBEVideoAspect"
#define ADBERenderDeepColor				"ADBERenderDeepColor"
#define ADBEVideoCodecPrefsButton		"ADBEVideoCodecPrefsButton"
#define ADBEVideoCodecPrefsData			"ADBEVideoCodecPrefsData"
#define ADBEVideoResolution				"ADBEVideoResolution"

#define ADBEAudioRatePerSecond			"ADBEAudioRatePerSecond"
#define	ADBEAudioNumChannels			"ADBEAudioNumChannels"		// Usage not recommended, please use ADBEAudioChannelConfiguration
#define ADBEAudioSampleType				"ADBEAudioSampleType"
#define ADBEAudioCodec					"ADBEAudioCodec"
#define ADBEAudioCodecPrefsButton		"ADBEAudioCodecPrefsButton"
#define ADBEAudioCodecPrefsData			"ADBEAudioCodecPrefsData"
#define ADBEAudioBitrate				"ADBEAudioBitrate"
#define ADBEAudioChannelConfiguration	"ADBEAudioChannelConfiguration"

#define ADBEVideoBitrateEncoding		"ADBEVideoBitrateEncoding" // see enum
#define ADBEVideoBitrateLevel			"ADBEVideoBitrateLevel"
#define ADBEVideoMinBitrate				"ADBEVideoMinBitrate"
#define ADBEVideoTargetBitrate			"ADBEVideoTargetBitrate"
#define ADBEVideoMaxBitrate				"ADBEVideoMaxBitrate"

#define ADBEExportColorSpace			"ADBEExportColorSpace"
#define ADBEVideoGraphicsWhiteLuminance	"ADBEVideoGraphicsWhiteLuminance"
#define ADBEVideoExportLUT				"ADBEVideoExportLUT"

// VideoBitrateEncoding values
enum 
{
	prVideoBitrateEncoding_CBR1Pass = 0,
	prVideoBitrateEncoding_CBR2Pass,
	prVideoBitrateEncoding_VBR1Pass,
	prVideoBitrateEncoding_VBR2Pass,
	prVideoBitrateEncoding_VBR2PassConstrained,
	prVideoBitrateEncoding_VBR2PassUnconstrained,
};
	
// VideoBitrateLevel values
enum 
{
	prVideoBitrateLevel_Low = 0,
	prVideoBitrateLevel_Medium,
	prVideoBitrateLevel_High,
	prVideoBitrateLevel_Custom
};

// Topmost container group. 
// Use this to identify top-level params when calling AddParamGroup
#define ADBETopParamGroup			"ADBETopParamGroup"

/* Multigroup params:

	exParamType_multiGroup is a top-level param that only contains other parameters.
	The multiGroup is used to create multiple, top-level "audiences", e.g. groups
	of params that are for one output target (Audience). 
	
	An output target can be either one movie or many movies. Every param grouped
	under a multiGroup should be:
	
	a) An identical tree to the parameters under any other multiGroup
	b) Be echoed across all multigroups if the parameters are intended to
	   be not-unique to that target/audience.

    Simple movies that contain only one set of params would only have one multiGroup,
	with the top-level groups contained with it.

	Movies that can contain multiple targets/audiences would contain multiple, top-
	level multigroups that each contain a set of params for each target.

	Parameters that are marked with exParamFlag_shared are mirrored across all groups,
	but have a common value that is mirrored into all instances of the parameter.

	Example, simple settings:

	paramContainer
		multiGroup("mgroup0")
			Group ("video")
				Group ("basic video settings")
					param:	quality	intValue	exParamFlag_slider	range: 1-10000
					param:	width	int								range: 32-4000
					param:	height	int								range: 32-4000
			Group ("audio")
				Group ("basic audio settings")
					param:	rate	int								range: 11250-192000
				
	Example, multiple targets

	paramContainer
		multiGroup("mgroup0")
			Group ("video")
				Group ("basic video settings")
					param:	quality	intValue	exParamFlag_slider | exParamFlag_shared	range: 1-10000
					param:	width	int													range: 32-4000
					param:	height	int													range: 32-4000
			Group ("audio")
				Group ("basic audio settings")
					param:	rate	int								range: 11250-192000

		multiGroup("mgroup1")
			Group ("video")
				Group ("basic video settings")
					param:	rate	int								range: 11250-192000
					param:	quality	intValue	exParamFlag_slider | exParamFlag_shared	range: 1-10000
					param:	width	int													range: 32-4000
					param:	height	int													range: 32-4000
			Group ("audio")
				Group ("basic audio settings")
					param:	rate	int								range: 11250-192000

	In the second param set, there are two audiences, all the same parameters, and one parameter (quality),
	that is shared across groups, and has the same value in each audience.

	NOTE: it is the responsibility of the exporter plug-in to synchronize shared parameters across groups
	NOTE: at least one top-level multiGroup is required for a parameter container.

*/

// paramType
typedef enum
{
	exParamType_bool=1,
	exParamType_int,
	exParamType_float,		// 'double' float (i.e. float64)
	exParamType_ticksFrameRate,	// prTime int64 'ticks', used for framerates (second/ticks), durations, etc.
							// UI may display as a float, or shadow with a popup ? TBD ?
							// This allows for more accurate times/framerates than float.
	exParamType_ticksTime,	// prTime int64 'ticks', used for Time display.
							// UI may display as a timecode or similar
	exParamType_string,		// 
	exParamType_button,		// stores opaque binary data with custom button for setup
	exParamType_arbitrary,	// opaque binary data with no UI. Usage is up to exporter plug-in.
	exParamType_group,
	exParamType_multiGroup,	// see comment above
	exParamType_ratio,		// param is two int32's. May also hold a constrained list. used for aspect or 2D point.
	exParamType_thumbnail,
	exParamType_undefined=-1
} exParamType;



typedef enum
{
	exParamFlag_none = 0,
	exParamFlag_slider = 0x00000001,		// for exParamType_int or exParamType_float
	exParamFlag_independant = 0x00000002,	// no validate needed
	exParamFlag_shared = 0x00000004,		// this param may be shared across multiple multiGroups
	exParamFlag_multiLine = 0x00000008,		// string only -- multiline vs single line text
	exParamFlag_password = 0x00000010,		// string only -- don't display string
	exParamFlag_filePath = 0x00000020,		// string only -- this control contains a path and has a Browse button
	exParamFlag_optional = 0x00000040,		// This control has a checkbox attached. The param name is the checkbox string.
											// Checkbox is unchecked by default, so the param attached to it is disabled.
	exParamFlag_verticalAlignment = 0x00000080,	// This flag is used to determine if the property name and value controls are displayed side by side or vertically.
	exParamFlag_nonlinear = 0x00000100,		// When used with exParamFlag_slider, the slider range is mapped using polynomial nonlinearity (less nonlinear than logarithmic).  Useful when the range is very large.
} exParamFlags;


typedef struct
{
	csSDK_int32 numerator;
	csSDK_int32 denominator;
} exRatioValue;

// single param value
typedef struct
{
	union
	{
		csSDK_int32		intValue;
		double			floatValue;
		PrTime			timeValue;
		exRatioValue	ratioValue;
		csSDK_int32		unused[32];
	};
} exOneParamValueRec;

#define EXPORT_PARAM_STRING_LENGTH 256

// param value
typedef struct
{
	csSDK_int32			structVersion;	// 1
	exOneParamValueRec	rangeMin;
	exOneParamValueRec	rangeMax;
	exOneParamValueRec	value;

	prUTF16Char	paramString[EXPORT_PARAM_STRING_LENGTH];	// unicode
	csSDK_int32		disabled;
	csSDK_int32		hidden;
	void*			arbData;
	csSDK_int32		arbDataSize;
	csSDK_int32		optionalParamEnabled;
} exParamValues;

// param
typedef struct
{
	csSDK_int32			structVersion;	// 1
	exParamIdentifier	identifier;
	prUTF16Char			name[EXPORT_PARAM_STRING_LENGTH];		// unicode
	exParamType			paramType;
	csSDK_int32			flags;	// slider, independant, etc
	exParamValues		paramValues;
} exNewParamInfo;


typedef struct 
{
	void (*SetParamsVersion)(
		csSDK_uint32 exporterPluginID,
		csSDK_int32 inVersion);

	csSDK_int32 (*GetParamsVersion)(
		csSDK_uint32 exporterPluginID);

	prSuiteError (*AddMultiGroup)(
		csSDK_uint32 exporterPluginID,
		csSDK_int32* outMultiGroupIndex);

	prSuiteError (*AddParam)(
		csSDK_uint32 exporterPluginID,
		csSDK_int32 inMultiGroupIndex,
		const exParamIdentifier inParentGroupIdentifier,
		const exNewParamInfo *inParamInfo);

	prSuiteError (*AddParamGroup)(
		csSDK_uint32 exporterPluginID,
		csSDK_int32 inMultiGroupIndex,
		const exParamIdentifier inParentGroupIdentifier,		// parent group. 
														// use NULL to put the group into the multigroup
		const exParamIdentifier inParamGroupIdentifier,
		const prUTF16Char *inGroupName,
		csSDK_int32 inHidden,
		csSDK_int32 inDisabled,
		csSDK_int32 inIsParamPairGroup);			// inIsParamPairGroup = true means this is a group that 
											// displays 2 params side by side. Group box is not shown.

	prSuiteError (*ChangeParam)(
		csSDK_uint32 exporterPluginID,
		csSDK_int32 inMultiGroupIndex,
		const exParamIdentifier inParamIdentifier,	// param or group (to hide/dim a group)
		const exParamValues *inParamValue);

	prSuiteError (*GetParamValue)(
		csSDK_uint32 exporterPluginID,
		csSDK_int32 inMultiGroupIndex,
		const exParamIdentifier inParamIdentifier,
		exParamValues *outParamValue);

	prSuiteError (*GetParamType)(
		csSDK_uint32 exporterPluginID,
		csSDK_int32 inMultiGroupIndex,
		const exParamIdentifier inParamIdentifier,
		exParamType *outParamType);

	prSuiteError (*ClearParamGroupChildren)(
		csSDK_uint32 exporterPluginID,
		csSDK_int32 inMultiGroupIndex,						// inMultiGroupIndex = -1 == clear every multigroup
		const exParamIdentifier inParamGroupIdentifier);	// inParamGroupIdentifier == everything in this group

	prSuiteError (*SetParamName)(
		csSDK_uint32 inExporterPluginID,
		csSDK_int32 inMultiGroupIndex,
		const exParamIdentifier inParamIdentifier,
		const prUTF16Char* inParamName);

	prSuiteError (*AddConstrainedValuePair)(
		csSDK_uint32 inExporterPluginID,
		csSDK_int32 inMultiGroupIndex,
		const exParamIdentifier inParamIdentifier,
		const exOneParamValueRec* inParamValue,
		const prUTF16Char *inEnumParamName);

	prSuiteError (*ClearConstrainedValues)(
		csSDK_uint32 inExporterPluginID,
		csSDK_int32 inMultiGroupIndex,
		const exParamIdentifier inParamIdentifier);

	csSDK_int32 (*GetConstrainedValueCount)(
		csSDK_uint32 inExporterPluginID,
		csSDK_int32 inMultiGroupIndex,
		const exParamIdentifier inParamIdentifier);

	prSuiteError (*GetConstrainedValue)(
		csSDK_uint32 inExporterPluginID,
		csSDK_int32 inMultiGroupIndex,
		const exParamIdentifier inParamIdentifier,
		csSDK_int32 inContrainedValueIndex,
		exOneParamValueRec* outConstrainedValue);

	// calling sequence:
	// Call the first time with NULL for outArbData, to retrieve
	// the data size.
	// Call the second time with an allocated pointer to 
	// receive the arbitrary data
	prSuiteError (*GetArbData)(
		csSDK_uint32 inExporterPluginID,
		csSDK_int32 inMultiGroupIndex,
		const exParamIdentifier inParamIdentifier,
		csSDK_int32* outArbDataSize,
		void* outArbData);

	prSuiteError (*SetArbData)(
		csSDK_uint32 inExporterPluginID,
		csSDK_int32 inMultiGroupIndex,
		const exParamIdentifier inParamIdentifier,
		csSDK_int32 inArbDataSize,
		void* inArbData);

	csSDK_int32 (*CountMultiGroups)(
		csSDK_uint32 inExporterPluginID);

	prSuiteError (*SetConstrainedListIsOptional)(
		csSDK_uint32 inExporterPluginID,
		csSDK_int32 inMultiGroupIndex,
		const exParamIdentifier inParamIdentifier,
		csSDK_int32 inIsOptional);

	prSuiteError (*GetConstrainedValueDisplayName)(
		csSDK_uint32 inExporterPluginID,
		csSDK_int32 inMultiGroupIndex,
		const exParamIdentifier inParamIdentifier,
		csSDK_int32 inContrainedValueIndex,
		PrSDKString* outConstrainedValueDisplayName);

	prSuiteError (*SetParamDescription)(
		csSDK_uint32 inExporterPluginID,
		csSDK_int32 inMultiGroupIndex,
		const exParamIdentifier inParamIdentifier,
		const prUTF16Char* inParamDescription);

	prSuiteError (*MoveParam)(
		csSDK_uint32 exporterPluginID,
		csSDK_int32 inMultiGroupIndex,
		const exParamIdentifier inParamIdentifier,				// param to move
		const exParamIdentifier inIdentifierToBePlacedBefore);	// param to be placed before

	prSuiteError(*MoveParamToGroup)(
		csSDK_uint32 exporterPluginID,
		csSDK_int32 inMultiGroupIndex,
		const exParamIdentifier inParamIdentifier,				// param to move
		const exParamIdentifier inOldGroup,						// original location
		const exParamIdentifier inNewGroup);					// place at the end of the new group

	prSuiteError(*RemoveParam)(
		csSDK_uint32 exporterPluginID,
		csSDK_int32 inMultiGroupIndex,
		const exParamIdentifier inParamIdentifier,				// param to remove
		bool inRecursive);
	
	prSuiteError (*SetParamPlaceHolderText)(
		csSDK_uint32 inExporterPluginID,
		csSDK_int32 inMultiGroupIndex,
		const exParamIdentifier inParamIdentifier,
		const prUTF16Char* inParamPlaceHolderText);
} PrSDKExportParamSuite;

#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif /* PRSDKEXPORTPARAMSUITE_H */

