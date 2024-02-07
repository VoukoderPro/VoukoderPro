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

#ifndef	PRSDKMEDIAACCELERATORSUITE_H
#define PRSDKMEDIAACCELERATORSUITE_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKPIXELFORMAT_H
#include "PrSDKPixelFormat.h"
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

#define kPrSDKMediaAcceleratorSuite			"Premiere Media Accelerator Suite"
#define kPrSDKMediaAcceleratorSuiteVersion3		3
#define kPrSDKMediaAcceleratorSuiteVersion4		4
#define kPrSDKMediaAcceleratorSuiteVersion		kPrSDKMediaAcceleratorSuiteVersion4

typedef struct 
{
	/**
	**	Get the current directory in which to place media accelerator files.
	**
	**	@param	inPathBufferSize	The max size of outPath.
	**	@param	outPath				A buffer to hold the directory path.
	*/
	prSuiteError (*GetMediaAcceleratorDirectoryPath)(
		const prUTF16Char* inOriginalPath,
		int inPathBufferSize,
		prUTF16Char* outPath);

	/**
	**	For a given original path, get the Document ID and Content State GUID
	**	which are needed for the register and find calls.
	**
	**	@param	inOriginalPath		The path to the original file
	**	@param	inWriteIDToFileIfAbsent	If non-0, then if there is no XMP doc ID, one will be written to the file.
	**								only use this option in a compile or capture situation where it is always appropriate
	**								to write to the file. (BTW, even if you set this to false, if the app preference
	**	@param	outDocumentID		The Document ID GUID as a string
	**	@param	outContentState		The Content State as a GUID.
	*/
	prSuiteError (*GetDocumentIDAndContentState)(
		const prUTF16Char* inOriginalPath,
		int inWriteIDToFileIfAbsent,
		prPluginID* outDocumentID,
		prPluginID* outContentState);

	/**
	**	Register a path in the media accelerator database.
	**
	**	@param	inDocumentID		the GUID document ID
	**	@param	inContentState		the GUID content state
	**	@param	inPath				the path to register.
	**	@param	inKey				the string key under which to register the path.
	**	@param	inOriginalMediaPath	the path to the original media for this accelerator.
	*/
	prSuiteError (*RegisterPathInDatabase)(
		const prPluginID* inDocumentID,
		const prPluginID* inContentState,
		const prUTF16Char* inPath,
		const prUTF16Char* inKey,
		const prUTF16Char* inOriginalMediaPath);

	/**
	**	Find if a path matching the requested key and original media exists in the database. 
	**
	**	@param	inPathBufferSize	The max size of outPath.
	**	@param	outPath				the path from the database, if found.
	**	@param	inKey				the string key under which to register the path.
	**	@param	inDocumentID		the GUID document ID
	**	@param	inContentState		the GUID content state
	*/
	prSuiteError (*FindPathInDatabase)(
		int inPathBufferSize,
		prUTF16Char* outPath,
		const prPluginID* inDocumentID,
		const prPluginID* inContentState,
		const prUTF16Char* inKey);

	/**
	**	Find if a path matching the requested key and original media exists in the database. 
	**
	**	@param	inPathBufferSize	The max size of outPath.
	**	@param	outPath				the path from the database, if found.
	**	@param	inKey				the string key under which to register the path.
	**	@param	inDocumentID		the GUID document ID
	**	@param	inContentState		the GUID content state
	*/
	prSuiteError (*FindPathInDatabaseAndValidateContentState)(
		int inPathBufferSize,
		prUTF16Char* outPath,
		const prPluginID* inDocumentID,
		const prPluginID* inContentState,
		const prUTF16Char* inKey);
} PrSDKMediaAcceleratorSuite;

#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif
