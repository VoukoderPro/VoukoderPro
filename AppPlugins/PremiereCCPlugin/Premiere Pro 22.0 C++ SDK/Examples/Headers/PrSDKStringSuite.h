/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2001 Adobe Systems Incorporated                       */
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

#ifndef PRSDKSTRINGSUITE_H
#define PRSDKSTRINGSUITE_H

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

#define kPrSDKStringSuite			"MediaCore StringSuite"
#define kPrSDKStringSuiteVersion	1

typedef struct 
{
	/**
	**	This will dispose of an SDKString. It is OK to pass in an empty string.
	**
	**	@param	inSDKString				the string to dispose of
	**
	**	@return suiteError_StringNotFound	this string has not been allocated, or may have already been disposed
	**	@return suiteError_InvalidParms		one of the params is invalid
	*/
	prSuiteError (*DisposeString)(
		const PrSDKString*					inSDKString);			

	/**
	**	This will allocate an SDKString from a passed in null terminated string.
	**
	**	@param	inUTF8String			null terminated UTF8 string to copy into the SDK string
	**	@param	ioSDKString				this passed in SDK string will receive the new allocated string 
	**										- you must dispose of this sdk string
	**
	**	@return suiteError_StringNotFound	this string has not been allocated, or may have already been disposed
	**	@return suiteError_InvalidParms		one of the params is invalid
	*/
	prSuiteError (*AllocateFromUTF8)(
		const prUTF8Char*					inUTF8String,			
		PrSDKString*						outSDKString);		

	/**
	**	This will copy an SDKString into a UTF8 null terminated string if ioUTF8StringBufferByteCount reports enough bytes in 
	**		the string buffer to copy. If there are not enough bytes - suiteError_StringBufferTooSmall will be returned
	**		and ioUTF8StringBufferByteCount will contain the number of bytes needed to hold the string.
	**
	**	@param	outUTF8StringBuffer					Buffer where UTF8 string will be stored
	**	@param	ioUTF8StringBufferSizeInElements	Pass in the size, in elements, of outUTF8StringStorage. Returns the number of elements copied.  Includes terminating 0.
	**
	**	@return suiteError_InvalidParms			One of the params is invalid
	**	@return suiteError_StringBufferTooSmall	The passed in string buffer is too small. ioUTF8StringBufferByteCount will contain the number of bytes needed to copy this string.
	**	@return suiteError_NoError				The string has been copied. ioUTF8StringBufferByteCount contains the number of bytes copied.
	*/
	prSuiteError (*CopyToUTF8String)(
		const PrSDKString*					inSDKString,		
		prUTF8Char*							outUTF8StringBuffer,			
		csSDK_uint32*						ioUTF8StringBufferSizeInElements);

	/**
	**	This will allocate an SDKString from a passed in null terminated string.
	**
	**	@param	inUTF16String			null terminated UTF16 string to copy into the SDK string
	**	@param	ioSDKString				this passed in SDK string will receive the new allocated string 
	**										- you must dispose of this sdk string
	**
	**	@return suiteError_StringNotFound	this string has not been allocated, or may have already been disposed
	**	@return suiteError_InvalidParms		one of the params is invalid
	*/
	prSuiteError (*AllocateFromUTF16)(
		const prUTF16Char*					inUTF16String,			
		PrSDKString*						outSDKString);		

	/**
	**	This will copy an SDKString into a UTF16 null terminated string if ioUTF16StringBufferByteCount reports enough bytes in 
	**		the string buffer to copy. If there are not enough bytes - suiteError_StringBufferTooSmall will be returned
	**		and ioUTF16StringBufferByteCount will contain the number of bytes needed to hold the string.
	**
	**	@param	outUTF16StringBuffer				Buffer where UTF16 string will be stored
	**	@param	ioUTF16StringBufferSizeInElements	Pass in the size, in elements, of outUTF16StringStorage. Returns the number of elements copied.  Includes terminating 0.
	**
	**	@return suiteError_InvalidParms			One of the params is invalid
	**	@return suiteError_StringBufferTooSmall	The passed in string buffer is too small. ioUTF16StringBufferByteCount will contain the number of bytes needed to copy this string.
	**	@return suiteError_NoError				The string has been copied. ioUTF16StringBufferByteCount contains the number of bytes copied.
	*/
	prSuiteError (*CopyToUTF16String)(
		const PrSDKString*					inSDKString,		
		prUTF16Char*						outUTF16StringBuffer,			
		csSDK_uint32*						ioUTF16StringBufferSizeInElements);



} PrSDKStringSuite;

#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif 
