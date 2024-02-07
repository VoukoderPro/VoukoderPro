/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2006 Adobe Systems Incorporated                       */
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

#ifndef PRSDKIMPORTERFILEMANAGERSUITE_H
#define PRSDKIMPORTERFILEMANAGERSUITE_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKMALERRORS_H
#include "PrSDKMALErrors.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif


#define kPrSDKImporterFileManagerSuite				"Importer File Manager Suite"
#define kPrSDKImporterFileManagerSuiteVersion		4

typedef struct 
{
	/*
	**	@param inPluginID, the importerID provided to the plugin
	**	@param inStreamIndex, the stream held by the importer to modify
	**	@param inFileCount, the number of files that the stream holds open between imOpenFile and imQuietFile.
	**	If more files are opened, but on a temporary basis, we don't actually want to know.
	*/
	prSuiteError (*SetImporterStreamFileCount)(csSDK_uint32 inPluginID, csSDK_int32 inStreamIndex, csSDK_int32 inFileCount);

	/*
	**	Asynchronously refreshes any media associated with inFilePath
	**
	**	@param inFilePath, path to file on disk
	*/
	prSuiteError (*RefreshFileAsync)(const prUTF16Char* inFilePath);

	/**
	**	Get the refresh interval for growing files. 0 means no automatic refresh.
	*/
	prSuiteError (*GetGrowingFileRefreshInterval)(csSDK_int32* outIntervalInSeconds);

	/*
	**	@param inImportID, the importID provided to the plugin
	**	@param inFileCount, the number of files that the stream holds open between imOpenFile and imQuietFile.
	**	If more files are opened, but on a temporary basis, we don't actually want to know.
	*/
	prSuiteError (*SetImporterInstanceStreamFileCount)(void* inImportID, csSDK_int32 inFileCount);

} PrSDKImporterFileManagerSuite;

// </PREMIERE_PRIVATE>

#ifdef __cplusplus
}
#endif

#endif // PRSDKIMPORTERFILEMANAGERSUITE_H