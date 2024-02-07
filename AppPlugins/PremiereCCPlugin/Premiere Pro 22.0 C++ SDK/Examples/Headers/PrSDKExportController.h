/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2001-2010 Adobe Systems Incorporated                  */
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

#ifndef PRSDKEXPORTCONTROLLER_H
#define PRSDKEXPORTCONTROLLER_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKENTRY_H
#include "PrSDKEntry.h"
#endif

#ifndef PRSDKPLUGSUITES_H
#include "PrSDKPlugSuites.h"
#endif

#ifndef PRSDKPLUGMEMORY_H
#include "PrSDKPlugMemory.h"
#endif

#ifndef __SPBasic__
#include "SPBasic.h"
#endif

#ifndef	kPrMaxPath
#define kPrMaxPath 260
#endif

// [NOTE] any negative value result will be considered an error by Premiere,
// return kErrorReportedInSuite if you have something to say about the error
// and premiere will extract the error string from the PrSDKErrorSuite and display
// it for you at an appropriate time.
typedef csSDK_int32 PrExportControllerResult;
const PrExportControllerResult			kExportControllerSuccess				= 0;	// success
const PrExportControllerResult			kExportControllerCancelSetup			= 1;	// returned if the user selects cancel during your setup
const PrExportControllerResult			kExportControllerSelectorUnsupported	= 3;	// This should be the default error return for unsupported selectors

const PrExportControllerResult			eExportControllerErrorReportedInSuite	= -1;	// if you have an error to report, return this result so we know to get the result string
const PrExportControllerResult			eExportControllerCannotInitialize		= -2;

// Selectors
typedef csSDK_uint32 PrExportControllerSelector;
enum
{
	exportController_Startup		= 0,
	exportController_ExportTimeline	= 1,
	exportController_Shutdown		= 7,
	exportController_ForceEnumSize	= kPrForceEnumToIntValue
};

typedef struct 
{
	prUTF16Char	mDisplayName[30];		// the display name for your plugin (as it will be shown under the menu Export - ...)
} ExportControllerInitRec;

typedef struct
{
	csSDK_int32	ecInterfaceVer;			// version # of the Export Controller interface
	piSuitesPtr	piSuites;
} ecStdParms;



typedef struct 
{
	void* mModuleID;					// this is private to Premiere, don't modify it. You must pass it in when using the 
										// PrSDKExportControllerSuite callbacks.
	PrTimelineID mTimelineID;			// ID of the currently active sequence.  Used in conjunction with PrSDKExportControllerSuite's ExportFile callback
} ExportControllerTimelineInfoRec;

/**
**	Export Controller plugin's entry point 
**
**	@param	inSelector		the selector sent by Premiere to the plugin
**	@param	inParam1		depending on the selector the contents of this parameter will change
*/
extern "C" 
{
typedef PrExportControllerResult (* ExportControllerEntryFunc)(PrExportControllerSelector inSelector,  ecStdParms* ecStdParms, void* ioParam1);
#define SDKExportControllerEntryPointName "xExportControllerEntry"
}

/**
**	General notes:
**	Any result code returned that is negative will be considered to be an error by Premiere.
**	Premiere will call the plugin back with kGetErrorTextSize, followed by kGetErrorText
**	to provide the plugin with the opportunity to display information about what failed.
**
**	Selectors:
**	exportController_Startup
**	param1 = ExportControllerInitRec*
**
**	Called once during application startup. 
**	The plugin tells Premiere whether or not it can initialize itself, 
**	and fills out the ExportControllerInitRec.
**
**
**
**	exportController_ExportTimeline
**	param1 = PrTimelineID
**
**	Called by Premiere when the user triggers an Export.
**
**	ExportController_Shutdown
**	param1 = empty (NULL)
**	
**	Called when Premiere is quitting, do any necessary cleanup.
*/	

#endif