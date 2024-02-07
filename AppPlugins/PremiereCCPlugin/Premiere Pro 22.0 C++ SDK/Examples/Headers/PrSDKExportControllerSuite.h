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

#ifndef PRSDKEXPORTCONTROLLERSUITE_H
#define PRSDKEXPORTCONTROLLERSUITE_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKEXPORTCONTROLLER_H
#include "PrSDKExportController.h"
#endif

#ifndef PRSDKMALERRORS_H
#include "PrSDKMALErrors.h"
#endif

#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif

#define kPrSDKExportControllerSuite				"Premiere ExportController Suite"
#define kPrSDKExportControllerSuiteVersion		1


typedef struct 
{
	const prUTF16Char*			mDestinationPath;			// the path to the destination file that is to be created by Premiere's render operation
	const prUTF16Char*			mPresetPath;				// the Adobe Media Encoder preset file to be used for generating the output file
} ExportControllerExportRec;

const size_t kMosIDStringLength = 128;
typedef prUTF16Char prMosIDString[kMosIDStringLength];


typedef struct
{
	/**
	**	Use this callback to retrieve the MOS ID associated with a given TimelineID (which is passed to the plugin
	**	when the user triggers an export operation).
	**	
	**	@param	inTimelineID			the value indicating which timeline (ie sequence) the data is being requested from
	**
	**	@param	inModuleID				a value passed into the plugin by Premiere to identify
	**									the module instance. Do not modify this value.
	*/
	prSuiteError (*RetrieveMosID)(
		void*			inModuleID,
		PrTimelineID	inTimelineID,
		prMosIDString*	outMosID);

	/**
	**	This callback triggers Premiere to render out a file based on a timeline ID as a source.
	**	The ExportControllerExportRec specifies which render preset to use & what destination path to render to.
	**	
	**	@param inTimelineID				a value passed to the plugin when the user triggers the render operation 
	**									
	**
	**	@param	inModuleID				a value passed into the plugin by Premiere to identify
	**									the module instance. Do not modify this value.
	*/
	prSuiteError (*ExportFile)(
		void*			inModuleID,
		PrTimelineID	inTimelineID,
		ExportControllerExportRec* inExportInfo);

	
} PrSDKExportControllerSuite; 

#ifdef __cplusplus
}
#endif

#pragma pack(pop)
#endif /* PRSDKEXPORTCONTROLLERSUITE_H */