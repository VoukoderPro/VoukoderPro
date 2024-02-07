/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2011 Adobe Systems Incorporated                       */
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

#ifndef	PRSDKEXPORTERUTILITYSUITE_H
#define PRSDKEXPORTERUTILITYSUITE_H

#ifndef PRSDKEXPORT_H
#include "PrSDKExport.h"
#endif

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKMALERRORS_H
#include "PrSDKMALErrors.h"
#endif

#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif

#define kPrSDKExporterUtilitySuite				"MediaCore Exporter Utility Suite"
#define kPrSDKExporterUtilitySuiteVersion1		1
#define kPrSDKExporterUtilitySuiteVersion		kPrSDKExporterUtilitySuiteVersion1

typedef struct
{
	// TODO - do we need more, or is this it?
	csSDK_int32			inRenderParamsSize;
	csSDK_int32			inRenderParamsVersion;
	PrPixelFormat		inFinalPixelFormat;
	PrTime				inStartTime;
	PrTime				inEndTime;
	float				inReservedProgressPreRender; //Defaults to zero, represents the amount of progress already done before the render loop
	float				inReservedProgressPostRender; //Defaults to zero, represents the amount of progress left after the render loop
	bool				inHardwareResidentFrameOutputSupported;
} ExportLoopRenderParams;

// Callbacks used for driving the Export loop.
// The call-ee is not responsible for freeing the PPixHand, though if you need it to survive longer than the life of the function call,
// then you need to addref and dispose it.
// TODO - we need to add a function to the PPixSuite for adding a ref to a PPixHand.
typedef prSuiteError (*PrSDKMultipassExportLoopFrameCompletionFunction)(csSDK_uint32 inWhichPass, csSDK_uint32 inFrameNumber, csSDK_uint32 inFrameRepeatCount, PPixHand inRenderedFrame, void* inCallbackData);

typedef struct
{
	/*
	 * This function assumes that your exporter supports exSelQueryOutputSettings, which will be called.
	 */
	prSuiteError (*DoMultiPassExportLoop)(
		csSDK_uint32 inExporterID,
		const ExportLoopRenderParams* inRenderParams,
		csSDK_uint32 inNumberOfPasses,
		PrSDKMultipassExportLoopFrameCompletionFunction inCompletionFunction,
		void* inCompletionParam);

	/*
	 * This function only makes sense when called during your callback from DoMultiPassExportLoop.
	 */
	prSuiteError (*ReportIntermediateProgressForRepeatedVideoFrame)(
		csSDK_uint32 inExporterID,
		csSDK_uint32 inRepetitionsProcessedSinceLastUpdate);

	/*
	 * Use this function to report an event from an exporter.
	 */
	prSuiteError (*ReportEvent)(
		csSDK_uint32 inExporterID,
		csSDK_uint32 inEventType,
		const prUTF16Char*	inEventTitle,
		const prUTF16Char*	inEventDescription);
} PrSDKExporterUtilitySuite;

} // extern "C"

#pragma pack(pop)

#endif //PRSDKEXPORTERUTILITYSUITE_H
