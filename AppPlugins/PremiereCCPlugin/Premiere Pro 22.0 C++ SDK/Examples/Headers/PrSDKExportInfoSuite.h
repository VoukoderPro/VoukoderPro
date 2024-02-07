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


#ifndef	PRSDKEXPORTINFOSUITE_H
#define PRSDKEXPORTINFOSUITE_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKMALERRORS_H
#include "PrSDKMALErrors.h"
#endif

#ifndef PRSDKPIXELFORMAT_H
#include "PrSDKPixelFormat.h"
#endif

#ifndef PRSDKEXPORT_H
#include "PrSDKExport.h"
#endif

#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif

#define kPrSDKExportInfoSuite				"MediaCore Export Info Suite"
#define kPrSDKExportInfoSuiteVersion		1

enum PrExportSourceInfoSelector 
{
	kExportInfo_VideoWidth,					// PrParam holds a kPrParam_Int32
	kExportInfo_VideoHeight,				// PrParam holds a kPrParam_Int32
	kExportInfo_VideoFrameRate,				// PrParam holds a kPrTime
	kExportInfo_VideoFieldType,				// PrParam holds a kPrParamType_Int32 - treat as PrFieldType
	kExportInfo_VideoDuration,				// PrParam holds a kPrParamType_Int64 - treat as PrTime
	kExportInfo_PixelAspectNumerator,		// PrParam holds a kPrParam_Int32
	kExportInfo_PixelAspectDenominator,		// PrParam holds a kPrParam_Int32
	kExportInfo_AudioDuration,				// PrParam holds a kPrParamType_Int64 - treat as PrTime
	kExportInfo_AudioChannelsType,			// PrParam holds a kPrParamType_Int32 - treat as PrAudioChannelType
	kExportInfo_AudioSampleRate,			// PrParam holds a kPrParamType_Float64
	kExportInfo_Unused1,					// N/A
	kExportInfo_Unused2,					// N/A
	kExportInfo_Unused3,					// N/A
	kExportInfo_SourceHasAudio,				// PrParam holds a kPrParamType_Bool
	kExportInfo_SourceHasVideo,				// PrParam holds a kPrParamType_Bool
	kExportInfo_RenderAsPreview,			// PrParam holds a kPrParamType_Bool
	kExportInfo_SequenceGUID,				// PrParam holds a kPrParamType_Guid. Treat as prPluginID.
	kExportInfo_SessionFilePath,			// PrParam holds a kPrParamType_PrMemoryPtr, which is holding a prUTF16Char array.
											// note, the plugin is responsible to release the pointer using PrMemory suites.
	kExportInfo_VideoPosterFrameTickTime,	// PrParam holds a kPrParamType_Int64 - treat as PrTime - added for CS5, used internally for AE
	kExportInfo_SourceTimecode,				// CS5.0.X PrParam holds a kPrParamType_PrMemoryPtr to a ExporterTimecodeRec. Deallocate w PrMemory suites
	kExportInfo_UsePreviewFiles,			// PrParam holds a kPrParamType_Bool
	kExportInfo_NumAudioChannels,			// PrParam holds a kPrParam_Int32
	kExportInfo_SourcePixelFormat,			// PrParam holds a kPrParamType_PrMemoryPtr to a ExporterSourcePixelFormatRec. Deallocate w PrMemory suites (including member pointers)
	kExportInfo_SourceBitrate,				// PrParam holds a kPrParamType_Int64. This is the source's bitrate in kbps, not available for all source types.
	kExportInfo_SourceExtendedFormat,		// PrParam is supplied with a kPrParamType_PrMemoryPtr to an ExportInfoExtendedFormatQueryRec which also contains ExporterSourceExtendedFormatRec structure with the result.  Deallocate w PrMemory suites (including member pointers)

	kExportInfo_MaxSize = kPrForceEnumToIntValue
};


typedef struct
{
	csSDK_int64	mTimecodeTicks;
	csSDK_int64	mTicksPerFrame;
	bool		mTimecodeStartPrefersDropFrame;
} ExporterTimecodeRec;

typedef struct
{
	PrPixelFormat*		mPixelFormatArray;
	csSDK_int32			mPixelFormatArrayCount;
	PrPixelFormat*		mPixelCustomFormatArray;
	csSDK_int32			mPixelCustomFormatArrayCount;
} ExporterSourcePixelFormatRec;

typedef struct
{
	ExporterSourcePixelFormatRec	mPixelFormats;
	exExtendedFormatInfoRec			mExtendedFormat;
} ExporterSourceExtendedFormatRec;

//typedef struct : result error C7626: unnamed class used in typedef name cannot declare members other than non-static data members, member enumerations, or member classes 
typedef struct NameIt 
{
	enum
	{
		kValidationResponse_NoComment = 0,
		kValidationResponse_NotAcceptable = 1,	// The format as described is not acceptable to the Exporter.
		kValidationResponse_Acceptable = 2,		// The format as described is acceptable to the Exporter.
	};
	// The callback is used to check if a proposed format is acceptable to the Exporter.
	typedef csSDK_int32 (*ValidationCallback)(void* inInstance, const ExporterSourceExtendedFormatRec* inFormat);

	ValidationCallback				inValidationCallback;			// Callback to check which formats are acceptable to the requesting Exporter
	void*							inValidationCallbackInstance;	// An instance to be supplied when the ValidationCallback is invoked
	ExporterSourceExtendedFormatRec	outResultFormat;				// Final resulting format information returned from the query
} ExporterSourceExtendedFormatQueryRec;


typedef struct 
{
	/*
	**	GetExportSourceInfo - given an exporter pluginID, get some specific
	**	information about the source that is being exported.
	**
	**	If the outSourceInfo holds a PrMemoryPtr on return, it will have been allocated
	**	by the host, and it is then the plugin's responsibility to free it using the PrMemory suite.
	**
	**	@param	inExporterPluginID		the passed in plugin ID 
	**	@param	inSelector				the requested value to retrieve about the source being exported
	**	@param	outSourceInfo			the populated PrParam holding the data (if no error was returned).
	*/
	prSuiteError (*GetExportSourceInfo)(
		csSDK_uint32 inExporterPluginID, 
		PrExportSourceInfoSelector inSelector, 
		PrParam* outSourceInfo);

} PrSDKExportInfoSuite;

#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif /* PRSDKEXPORTINFOSUITE_H */

