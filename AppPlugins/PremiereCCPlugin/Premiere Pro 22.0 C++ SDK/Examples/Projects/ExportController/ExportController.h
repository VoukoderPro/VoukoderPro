/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2010 Adobe Systems Incorporated					   */
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

#include <PrSDKExport.h>
#include <PrSDKExportControllerSuite.h>
#include <PrSDKErrorSuite.h>
#include <PrSDKSequenceInfoSuite.h>
#include <SDK_File.h>
#include <fstream>

#ifdef PRWIN_ENV
#include <Shlobj.h>
#endif

#ifdef PRMAC_ENV
#include <Carbon/Carbon.h>
#endif

#define	PLUGIN_DISPLAY_NAME	L"SDK Export Controller"

// This sample makes a lot of assumptions about preset locations,
// so that we can reuse existing presets.
// You will no doubt want to provide your own presets.
// On Win we export a DV NTSC AVI, on MacOS a DV NTSC MOV, to the Desktop
#ifdef PRWIN_ENV
#define PRESET_PATH			L"C:\\Program Files\\Adobe\\Adobe Media Encoder CC 2018\\MediaIO\\systempresets\\3F3F3F3F_41564956\\NTSC DV Widescreen.epr"
#define OUTPUT_FILENAME		L"\\ExportControllerOutput.avi"
#elif defined PRMAC_ENV
#define PRESET_PATH			L"/Applications/Adobe Media Encoder CC 2018/Adobe Media Encoder CC 2018.app/MediaIO/systempresets/3F3F3F3F_4D6F6F56/NTSC DV Widescreen.epr"
#define OUTPUT_FILENAME		L"/ExportControllerOutput.mov"
#endif

#define	ERROR_TITLE			L"SDK Export Controller error"
#define	ERROR_PRESET_DESC	L"No preset found.  Export cancelled."

//----------------------------------------------------------
// Prototypes

// Declare plug-in entry point with C linkage
extern "C" {
DllExport
PREMPLUGENTRY xExportControllerEntry(
	int			selector, 
	ecStdParms	*stdParms, 
	void		*param1);
}

prMALError ecStartup(
	ecStdParms *stdParms, 
	ExportControllerInitRec	*initRec);

prMALError ecExportTimeline(
	ecStdParms *stdParms,
	ExportControllerTimelineInfoRec *timelineInfo);

prMALError ecShutdown(ecStdParms *stdParms);

bool fileExists(const prUTF16Char *filePath);
int setDestinationPathToDesktop(prUTF16Char *destinationPath);
