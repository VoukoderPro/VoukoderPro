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

#ifndef ADOBESDK_ADOBESDKSTRINGSUITE_H
#define ADOBESDK_ADOBESDKSTRINGSUITE_H

// adobesdk
#include <adobesdk/config/PreConfig.h>
#include <adobesdk/config/AdobesdkTypes.h>

// SweetPea2
#include <SPTypes.h>

#define kADOBESDK_StringSuite			"ADOBESDK StringSuite"
#define kADOBESDK_StringSuite_Version1	1

#define kADOBESDK_StringSuite_VersionCurrent	kADOBESDK_StringSuite_Version1
#define	kADOBESDK_StringSuiteCurrent			ADOBESDK_StringSuite1

#define kADOBESDK_Error_StringBufferTooSmall		'sbts'
#define kADOBESDK_Error_StringNotFound				'stnf'

typedef struct 
{
	/**
	**	This will dispose of an SDK string. It is OK to pass in an empty string.
	**
	**	@param	inSDKString					the string to dispose of
	**
	**	@return kSPBadParameterError		one of the params is invalid
	*/
	SPErr (*DisposeString)(
		const ADOBESDK_String*		inSDKString);			

	/**
	**	This will allocate an SDKString from a passed in null terminated string.
	**
	**	@param	inUTF8String			null terminated UTF8 string to copy into the SDK string
	**	@param	ioSDKString				this passed in SDK string will receive the new allocated string 
	**										- you must dispose of this sdk string
	**
	**	@return kSPBadParameterError		one of the params is invalid
	*/
	SPErr (*AllocateFromUTF8)(
		const ADOBESDK_UTF8Char*	inUTF8String,			
		ADOBESDK_String*			outSDKString);		

	/**
	**	This will copy an SDKString into a UTF8 null terminated string if ioUTF8StringBufferByteCount reports enough bytes in 
	**		the string buffer to copy. If there are not enough bytes - kADOBESDK_Error_StringBufferTooSmall will be returned
	**		and ioUTF8StringBufferByteCount will contain the number of bytes needed to hold the string.
	**
	**	@param	outUTF8StringBuffer						Buffer where UTF8 string will be stored
	**	@param	ioUTF8StringBufferSizeInElements		Pass in the size, in elements, of outUTF8StringStorage. Returns the number of elements copied.  Includes terminating 0.
	**
	**	@return kSPBadParameterError					One of the params is invalid
	**	@return kADOBESDK_Error_StringBufferTooSmall	The passed in string buffer is too small. ioUTF8StringBufferByteCount will contain the number of bytes needed to copy this string.
	**	@return kSPNoError								The string has been copied. ioUTF8StringBufferByteCount contains the number of bytes copied.
	*/
	SPErr (*CopyToUTF8String)(
		const ADOBESDK_String*		inSDKString,		
		ADOBESDK_UTF8Char*			outUTF8StringBuffer,			
		uint32_t*					ioUTF8StringBufferSizeInElements);

	/**
	**	This will allocate an SDKString from a passed in null terminated string.
	**
	**	@param	inUTF16String			null terminated UTF16 string to copy into the SDK string
	**	@param	ioSDKString				this passed in SDK string will receive the new allocated string 
	**										- you must dispose of this sdk string
	**
	**	@return kSPBadParameterError		one of the params is invalid
	*/
	SPErr (*AllocateFromUTF16)(
		const ADOBESDK_UTF16Char*	inUTF16String,			
		ADOBESDK_String*			outSDKString);		

	/**
	**	This will copy an SDKString into a UTF16 null terminated string if ioUTF16StringBufferByteCount reports enough bytes in 
	**		the string buffer to copy. If there are not enough bytes - kADOBESDK_Error_StringBufferTooSmall will be returned
	**		and ioUTF16StringBufferByteCount will contain the number of bytes needed to hold the string.
	**
	**	@param	outUTF16StringBuffer				Buffer where UTF16 string will be stored
	**	@param	ioUTF16StringBufferSizeInElements	Pass in the size, in elements, of outUTF16StringStorage. Returns the number of elements copied.  Includes terminating 0.
	**
	**	@return kSPBadParameterError					One of the params is invalid
	**	@return kADOBESDK_Error_StringBufferTooSmall	The passed in string buffer is too small. ioUTF16StringBufferByteCount will contain the number of bytes needed to copy this string.
	**	@return kSPNoError								The string has been copied. ioUTF16StringBufferByteCount contains the number of bytes copied.
	*/
	SPErr (*CopyToUTF16String)(
		const ADOBESDK_String*					inSDKString,		
		ADOBESDK_UTF16Char*						outUTF16StringBuffer,			
		uint32_t*								ioUTF16StringBufferSizeInElements);

} ADOBESDK_StringSuite1;

#include <adobesdk/config/PostConfig.h>

#endif // ADOBESDK_ADOBESDKSTRINGSUITE_H
