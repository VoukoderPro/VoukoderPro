/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 1992-2008 Adobe Systems Incorporated                  */
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
/* Record.h */

#include "PrSDKRecordModule.h"
#include "PrSDKErrorSuite.h"
#include "SDK_File.h"

#define RECMOD_MAGIC	131313
#define RECORDER_NAME	"SDK Recorder"
#define SAMPLE_SETUP1	"Sample Setup Dialog 1"
#define SAMPLE_SETUP2	"Sample Setup Dialog 2"

// This is the setup struct defined by the plug-in.
// It is passed with recmod_Open and recmod_ShowOptions.
// It is filled out by the plug-in based on user input during recmod_ShowOptions.  It is
// saved with the project and passed to the plug-in on subsequent recmod_Open's.
typedef struct {
	csSDK_int32		setup1_yes;
	csSDK_int32		setup1_no;
	csSDK_int32		setup2_yes;
	csSDK_int32		setup2_no;
} CapSetup, *CapSetupP;

// This is the private data struct defined by the plug-in.
// It is passed with almost every recmod selector.
typedef struct {
	int					APIVersion;
	prWnd				capWnd;
	int					capVideo;
	int					capAudio;
	StatusDispFunc		statusFunc;
	PrerollFunc			prerollFunc;
	FormatChangedFunc	formatChangedFunc;
	PrSDKErrorSuite3	*errorSuite;
	void				*callbackID;
	recDisplayPos		disp;
	CapSetupP			setupP;
	int					currentWidth;
	int					currentHeight;
	csSDK_int32			currentPAR;
	csSDK_int32			currentFrame;	// Frame number detected through the video stream
	csSDK_int32			framesToCapture;
	csSDK_int32			framesCaptured;
	csSDK_int32			magic;
} CapPriv, *CapPrivP, **CapPrivH;


//========================================================================================
// Windows Entry point
//========================================================================================
#ifdef PRWIN_ENV
HINSTANCE	gInst;

BOOL WINAPI DllMain (HINSTANCE hDLL, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
		case DLL_PROCESS_ATTACH:
			gInst = hDLL;
			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
	}
	return(kPrTrue);
}
#endif


//========================================================================================
// Prototypes
//========================================================================================

// Declare plug-in entry point with C linkage
extern "C" {
DllExport PREMPLUGENTRY RecEntry(
	csSDK_int32		selector, 
	rmStdParms		*stdparms, 
	void			*param1,
	void			*param2);
}

static prMALError 
recStartup8(
	rmStdParms		*stdparms, 
	recInfoRec8		*info8);

static prMALError 
recStopRecord(
	CapPriv		*capPtr);

static prMALError
recClose(
	rmStdParms		*stdparms,
	CapPriv			*capPtr);

static prMALError 
recGetSetupInfo(
	CapPriv			*capPtr, 
	recCapSetups	*setups);

static prMALError 
recOpen(
	rmStdParms		*stdparms, 
	CapPriv			**cappriv, 
	recOpenParms	*parms);

static prMALError 
recShutdown(
	rmStdParms		*stdparms);

static prMALError
recShowOptions(
	rmStdParms		*stdParms,
	CapPriv			*capPriv, 
	recSetupParms	*setupParms);

static prMALError 
audPrepRecord(
	CapPriv			*capPtr, 
	recCapParmsRec	*capRec);

static prMALError 
vidPrepRecord(
	CapPriv			*capPtr, 
	recCapParmsRec	*capRec);

static prMALError 
recPrepRecord8(
	CapPriv			*capPtr, 
	recCapParmsRec8	*capRec8);

static prMALError 
audStartRecord(
	CapPriv		*capPtr);

static prMALError 
vidStartRecord(
	CapPriv		*capPtr);

static prMALError
recStartRecord(
	CapPriv				*capPtr, 
	recCapturedFileInfo	*fileInfo);

static prMALError 
audServiceRecord(
	CapPriv		*capPtr);

static prMALError 
vidServiceRecord(
	CapPriv		*capPtr);

static prMALError 
recServiceRecord(
	CapPriv		*capPtr);

static prMALError 
audStopRecord(
	CapPriv		*capPtr);

static prMALError 
vidStopRecord(
	CapPriv		*capPtr);

static prMALError	
recStopRecord(
	CapPriv		*capPtr);

static prMALError 
audCloseRecord(
	CapPriv		*capPtr);

static prMALError 
vidCloseRecord(
	CapPriv		*capPtr);

static prMALError
recCloseRecord(
	CapPriv		*capPtr);

static prMALError 
recSetActive(
	CapPriv	*capPtr, 
	prBool	param);

static prMALError 
recSetDisp(
	CapPriv			*capPtr, 
	recDisplayPos	*disp);

static prMALError 
recIdle(
	rmStdParms			*stdParms,
	CapPriv				*capPtr, 
	recGetTimecodeRec	*timecode);

static prMALError 
recQueryInfo(
	rmStdParms			*stdParms,
	CapPriv				*capPtr, 
	recCapInfoRec		*capInfo);