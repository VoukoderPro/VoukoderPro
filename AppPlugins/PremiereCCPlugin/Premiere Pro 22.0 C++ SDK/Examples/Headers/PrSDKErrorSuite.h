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


#ifndef	PRSDKERRORSUITE_H
#define PRSDKERRORSUITE_H

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

#define kPrSDKErrorSuite				"Premiere Error Suite"

#define kPrSDKErrorSuiteVersion1		1	/* Frozen in Premiere Pro 1.0 */
/**
**	Error overview:
**
**	Plugins are allowed to send errors, warning, and information to 3 different
**	callbacks which will be handled by Premiere. For each, the plugin passes a string
**	and a context ID from their plugin type (for instance, a CompilerID from the a 
**	compiler). Then the plugin returns an error code defined in the appropriate
**	plugin type header which will tell Premiere that there is an error with description
**	waiting. Currently, Premiere concatenates all strings that it receives of each type
**	until they can be displayed.
*/
typedef struct 
{
	/**
	**	Set the plugin error string for Premiere.
	**
	**	@param	inErrorString	A NULL terminated string for the error. Premiere will
	**							copy this string, so the caller can delete it after
	**							the function returns.
	**	@param	inContextID		The context ID passed to the plugin.
	*/
	prSuiteError (*SetErrorString)(
		const char*				inErrorString,
		csSDK_uint32			inContextID);

	/**
	**	Set the plugin error string for Premiere.
	**
	**	@param	inWarningString	A NULL terminated string for the warning. Premiere will
	**							copy this string, so the caller can delete it after
	**							the function returns.
	**	@param	inContextID		The context ID passed to the plugin.
	*/
	prSuiteError (*SetWarningString)(
		const char*				inWarningString,
		csSDK_uint32			inContextID);

	/**
	**	Set the plugin error string for Premiere.
	**
	**	@param	inInfoString	A NULL terminated string for the info. Premiere will
	**							copy this string, so the caller can delete it after
	**							the function returns.
	**	@param	inContextID		The context ID passed to the plugin.
	*/
	prSuiteError (*SetInfoString)(
		const char*				inInfoString,
		csSDK_uint32			inContextID);
	
} PrSDKErrorSuite1;



#define kPrSDKErrorSuiteVersion2		2 /* New for Premiere Pro 1.5 */
/**
**	Version 2 changes:
**	The suite has been streamlined for this release.  In addition, the
**	string has been broken into two parts, a title and a description.  This
**	separation works well with the new Events Window.
*/
typedef struct 
{
enum{
	kEventTypeInformational = 1,
	kEventTypeWarning,
	kEventTypeError
};
	prSuiteError (*SetEventString)(
		csSDK_uint32			eventType,
		const char*				eventTitle,
		const char*				eventDescription);

} PrSDKErrorSuite2;


#define kPrSDKErrorSuiteVersion3		3 /* New CS4 */
/**
**	Version 3 changes:
**	Add UTF16 string support
*/
typedef struct 
{
enum{
	kEventTypeInformational = 1,
	kEventTypeWarning,
	kEventTypeError,

	kEventType_Mask = 0xFF,					// A mask to find the event type.  Higher bits are reserved for flags.

	//
	// The following flags can be bitwise or'ed with the base event type:
	//
	kEventFlag_DecodeError = 0x100,			// This event pertains to a decode error.
	kEventFlag_SubstitutedFrame = 0x200,	// This event pertains to a decode error in which a nearby frame was substituted.  Meaning when set with kEventFlag_DecodeError is currently undefined.
	kEventFlag_ImportOperation = 0x400,		// This event pertains to an import operation and contains additional details of errors or warnings encountered.  The message is formatted with a file name in quotes as the last portion of the description.
};
	prSuiteError (*SetEventStringUnicode)(
		csSDK_uint32			eventType,
		prUTF16Char*			eventTitle,
		prUTF16Char*			eventDescription);

} PrSDKErrorSuite3;

#define kPrSDKErrorSuiteVersion kPrSDKErrorSuiteVersion3
typedef PrSDKErrorSuite3 PrSDKErrorSuite;

#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif /* PRSDKERRORSUITE_H */

