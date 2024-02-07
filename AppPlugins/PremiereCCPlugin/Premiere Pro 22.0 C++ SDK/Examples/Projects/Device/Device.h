/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 1993-2008 Adobe Systems Incorporated                  */
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
/* 
	Device.h

	Pulled headers and prototypes out of .c file, fixed header inclusion
	problems. 

	12/13/99	- bbb

*/

#include "PrSDKDevice.h"
#include "PrSDKErrorSuite.h"

#define STRUCT_VERSION			2
#define DEVICE_CONTROLLER_NAME	"dsSetup sent."
#define SETUP_DESCRIPTION		"dsSetup sent. Respond with a setup dialog."
#define EXPORT_DESCRIPTION		"cmdInsertEdit sent. Respond with an Export To Tape dialog.\nThis is also sent after Export To Tape completes, so dialog\ncan be presented to show results of Export To Tape to user."
#define DEVICE_NAME				"SDK Virtual Device"
#define INFO_TITLE				"Sample info using Error Suite!"
#define WARNING_TITLE			"Sample warning using Error Suite!"
#define	ERROR_TITLE				"Sample error using Error Suite!"
#define EVENT_DESCRIPTION		"Sample description\nSecond line."

typedef struct {
	csSDK_int32	structVersion;
	csSDK_int32	mode;				// The last reported mode our device was in
	double		currentTimecode;	// The timecode of the simulated device, in frames
									// We use a float so we can simulate 1/10 speed, etc.
	csSDK_int32	gotoMode;			// If in gotoMode, which direction are we seeking in? (either modeFastFwd or modeRewind)
	double		gotoTimecode;		// The timecode to seek to, if in gotoMode
	csSDK_int32	numCallsToIdle;		// The number of times the PrintProc idle was called
} LocalRec, **LocalHand;

//	Prototypes

// Declare plug-in entry point with C linkage
extern "C" {
PREMPLUGENTRY DllExport xDevice (short selector, DeviceHand theData);
}

static csSDK_int32 DisplayEvent(csSDK_int32 eventType, DeviceHand theData);
static csSDK_int32 CheckConnection(LocalHand ldata);
static csSDK_int32 ExportToTape(PrMemoryHandle deviceHand, long selector);

