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
/* Adobe Premiere Device Control plug-in definitions			   */ 
/*																   */
/*******************************************************************/

//	This file is part of the public Premiere Pro SDK.


#ifndef PRSDKDEVICE_H
#define PRSDKDEVICE_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKPLUGSUITES_H
#include "PrSDKPlugSuites.h"
#endif

#ifndef PRSDKENTRY_H
#include "PrSDKEntry.h"
#endif

#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************
**	Version
*/
#define kDeviceControlAPIVersion75		750			// Premiere Pro 1.5
#define kDeviceControlAPIVersion8		800			// Pro 2.0
#define kDeviceControlAPIVersion85		850			// private
#define kDeviceControlAPIVersion9		900			// Pro 3.0
#define kDeviceControlAPIVersion10		1000		// CS5
#define kDeviceControlAPIVersion105		1050		// CS5.5
#define kDeviceControlAPIVersion11		1100		// CS6.0.1
#define kDeviceControlAPIVersion12		1200		// CS7.0
#define kDeviceControlAPIVersion13		1300		// CS7.0.1
#define kDeviceControlAPIVersion14		1400		// CS7.1
#define kDeviceControlAPIVersion		kDeviceControlAPIVersion14

/****************************************************************
**	Error Definitions
*/

enum prDevicemodError
{
	dmNoError =				0,
	dmDeviceNotFound =		1,		// The device is not available
	dmTimecodeNotFound =	2,		// The device cannot read the timecode from the media, or there isn't any to be read
	dmBadTimecode =			3,		// The device has timecode but it doesn't trust it
	dmCantRecord =			4,		// The device is unable to record to the media
	dmUserAborted =			5,		// The operation has stopped because the user cancelled
	dmLastErrorSet =		6,		// The plugin set the last error string for display through the UI using the SuitePea ErrorSuite.
	dmExportToTapeFinished = 7,		// The plugin is signalling the end of the export to tape operation
	dmTapeWriteProtected =	8,		// A record to tape failed because the tape was write protected
	dmNoTape =				9,		// A record to tape failed because there was no tape in the deck
	dmLastInfoSet =			10,		// The plugin set the info string for display through the UI using the SuitePea ErrorSuite.
	dmLastWarningSet =		11,		// The plugin set the warning string for display through the UI using the SuitePea ErrorSuite.
	dmHasNoOptions =		12,		// Pro 2.0: return this in response to the dsHasOptions selector. Return dmNoError to indicate device has options
	dmUnknownError =		99,		// A generic error that does not fit any above error is being returned.
	dmUnsupported =			-100,	// Unsupported selector
	dmGeneralError =		-1		// Unspecified error
};


/****************************************************************
**	Type Definitions
*/


/****************************************************************
**	Enumerations
*/
// Feature bits; these describe your device's capabilities to Premiere

enum
{
	fCanDelayMovieStart = 0x40000000,	// supports delay moves start on tape export			--- added in kDeviceControlAPIVersion13
	fCanPrintToTape	= 0x20000000,	// supports print to tape record mode.						--- added in kDeviceControlAPIVersion12
	fCanUseCC		= 0x10000000,	// Handles closed caption data.								--- added in kDeviceControlAPIVersion12
	fCanInsertNoUI	= 0x08000000,	// Supports kDeviceControlAPIVersion12 new record modes		--- added in kDeviceControlAPIVersion12
	fCanPreviewEdit	= 0x04000000,	// recorder supports preview								--- added in kDeviceControlAPIVersion12
	fCanAssembleEdit= 0x02000000,	// recorder supports assemble record mode					--- added in kDeviceControlAPIVersion12
	fRecordImmediate= 0x01000000,	// recorder records immedately after the controller seeks	--- NEW in 7.5
	fNoTransport	= 0x00800000,	// device supports no transport modes(play,stop,etc)		--- NEW in 7.0
	fCantLoop		= 0x00400000,	// device can't loop and it can't play to out				// obsolete
	fSyncStatus		= 0x00200000,	// driver guarantees sync status call						// obsolete Mac only
	fExportDialog	= 0x00100000,	// driver has its own export dialog							--- NEW in 5.0
	fCanInsertEdit	= 0x00080000,	// supports the InsertEdit command							--- NEW in 5.0
	fDrvrQuiet		= 0x00040000,	// driver supports a quiet mode								--- NEW in 4.2
	fHasJogMode		= 0x00020000,	// device has jog capabilities								--- NEW in 4.2
	fCanEject		= 0x00010000,	// can Eject media											--- NEW in 4.2
	fStepFwd		= 0x00008000,	// can step forward
	fStepBack		= 0x00004000,	// can step back
	fRecord			= 0x00002000,	// can record
	fPositionInfo	= 0x00001000,	// returns position info
	fGoto			= 0x00000800,	// can seek to a specific frame (fPositionInfo must also be set)
	f1_5			= 0x00000400,	// can play at 1/5 speed
	f1_10			= 0x00000200,	// can play at 1/10 speed									// obsolete, f1_5 now used for slow play
	fBasic			= 0x00000100,	// supports Stop,Play,Pause,FastFwd,Rewind
	fHasOptions		= 0x00000080,	// *** OBSOLETE ***
	fReversePlay	= 0x00000040,	// supports reverse play
	fCanLocate		= 0x00000020,	// can locate a specific timecode
	fStillFrame		= 0x00000010,	// device is frame addressable, like a laser disc			// obsolete
	fCanShuttle		= 0x00000008,	// supports the Shuttle command
	fCanJog			= 0x00000004,	// supports the JogTo command								// obsolete

	fFeature_Size	= kPrForceEnumToIntValue
};

// Modes; the device is always in one of the following modes. 
enum 
{
	modeStop = 0,
	modePlay,
	modePlay1_5,
	modePlay1_10,
	modePause,
	modeFastFwd,
	modeRewind,
	modeRecord,
	modeGoto,
	modeStepFwd,
	modeStepBack,
	modePlayRev,
	modePlayRev1_5,
	modePlayRev1_10,
	modeTapeOut,			// 5.0 -- no media in device
	modeLocal,				// 5.0 -- VTR in local not remote mode
	modeRecordPause,		// NEW 7.0
	modePlayFastFwd,		// NEW 7.0
	modePlayRewind,			// NEW 7.0
	modeRecordAssemble,		// added in kDeviceControlAPIVersion12
	modeRecordInsert,		// added in kDeviceControlAPIVersion12

	mode_Size	= kPrForceEnumToIntValue
};

enum {
	handlerCapture = 1,
	handlerEditToTape,

	handler_Size	= kPrForceEnumToIntValue
}; // set in DeviceRec.mode when the command is cmdSetDeviceHandler

// Commands; sent with dsExecute
enum 
{
	cmdGetFeatures = 0,			// return feature bits
	cmdStatus,					// fill in current mode and timecode, gets called repeatedly
	cmdNewMode,					// change to the mode in 'mode'
	cmdGoto,					// goto the timecode specified in 'timecode'
	cmdLocate,					// find the timecode in 'timecode' and then return (with deck in play)
	cmdShuttle,					// shuttle at rate specified in 'mode', from -100 to +100
	cmdJogTo,					// Obsolete, do not use!
	cmdJog,						// Obsolete, do not use!
	cmdEject,					// 4.2	eject media
	cmdLocateInsertAsm,			// Obsolete
	cmdInsertEdit,				// 5.0  used when driver is controlling the edit
	cmdStatusSync,				// Obsolete, do not use!
	cmdGetDeviceDisplayName,	// 6.0	fill in the displayName field
	cmdSetDropness,				// 2.0 Pro:  Premiere tells device control what the dropness is based on data from DV stream
	cmdGetCurrentDeviceIdentifier,	// 2.0 Pro: Optional. Return a unique (non-localized) string for the currently connected camera/deck.
	cmdSetDeviceHandler,			// CC 7.1 Optional. Sent when the current device handler changes (capture or edit to tape, from the enum above)
									
	cmd_Size	= kPrForceEnumToIntValue
};

// edit enable flags for cmdLocateInsertAsm command
enum
{
	insertVideo			= 0x0001,
	insertAudio1		= 0x0002,
	insertAudio2		= 0x0004,
	insertTimeCode		= 0x0008,
	insertAssemble		= 0x0080,
	insertPreview		= 0x1000,

	insert_Size	= kPrForceEnumToIntValue
};

// used when device driver is handling the edit
enum
{
	setupWaitProc = 1,
	idle,
	complete,

	edit_Size	= kPrForceEnumToIntValue
};

// Constants
const int kInvalidTimecode = -1;					// Used as an init value or when the device reports it can't read the timecode


/****************************************************************
**	Callback Functions
*/

//	Prototypes for functions used to communicate with Premiere
typedef void (*PauseProcPtr)(void);
typedef void (*ResumeProcPtr)(void);
typedef csSDK_int32 (*Print2TapeProcPtr)(PrMemoryHandle deviceHand, long selector);
typedef csSDK_int32 (*CallBackPtr)(void);
typedef void (*TimecodeUpdatePtr)(csSDK_int32 outTimecode, void* inClassID);
typedef csSDK_int32 (*DroppedFrameCountPtr)(void* inClassID);


/****************************************************************
**	Structures
*/
typedef struct {
	PrMemoryHandle			deviceData;				// private data which the plug-in creates
	short					command;				// command to perform
	short					mode;					// New mode (in) or current mode (out)
	csSDK_int32				timecode;				// New timecode (in) or current timecode (out); -1 = N/A, -2 = blank
	short					timeformat;				// Format: 0=non-drop, 1=drop-frame
	short					timerate;				// fps for the timecode above
	csSDK_int32				features;				// feature bits (out) for the features command
	short					error;					// error code (out) from any routine
	short					preroll;				// pre-roll time (frames) for cmdLocate
	CallBackPtr				callback;				// callback for cmdLocate, returns non-zero to stop
	PauseProcPtr			PauseProc;				// callback to pause current operations
	ResumeProcPtr			ResumeProc;				// callback to restart current operations
	long					xtimecode;				// either outpoint or duration for editing ops
	long					keycode;				// for film ops -- from the userbits
	short					editmode;				// for editing ops
	short					exteditmode;			// for extended editing ops.. DVW, etc.
	Print2TapeProcPtr		PrintProc;				// callback when driver is performing the edit
	prWnd					parentWindow;			// parent HWND for setup dialog, only valid during dsSetup
	piSuitesPtr				piSuites;				// pointer to plug-in suites functions
	char*					displayName;			// pointer to the device name (max 255) that is displayed in the Capture Window
	TimecodeUpdatePtr		TimecodeUpdateProc;		// During cmdLocate, this will report timecode to Premiere
	void*					classID;				// ClassID for reporting back timecode
	long					version;				// version of this API (kDeviceControlAPIVersion)
	short					videoStreamIsDrop;		// Filled in by Premiere when recorder determines video stream switched from/to drop/non drop... used during cmdSetDropness... see SDK for notes on this feature
	short					autoDetectDropness;		// Signal Premiere to determine dropness through recorder and let us know
	char*					currentDeviceIDStr;
	long					preferredScale;			// New in 3.0
	unsigned long			preferredSampleSize;	// New in 3.0
	DroppedFrameCountPtr	DroppedFrameProc;		// New in CS6.0.1, returns the count of dropped frames during an insert edit
	csSDK_uint32			exportAudioChannels;	// new in CS7.0.0, audio channels active on export. On record (in), the set bits correspond to 
													// audio channels to export when the device supports audio channel selection; A1 == bit 0, A2 == bit 1,
													// etc. On return from the plugin (out, using cmdGetFeatures) the bits set should correspond to the audio
													// channels available on the device.
	csSDK_uint16			exportFlags;			// new in CS7.0, see enum below
	csSDK_int32				delayFrames;			// new in SDK version 13, movie start delay in frames.
	char					reserved[18];			// reserved;
} DeviceRec, *DevicePtr, **DeviceHand;

/****************************************************************
**	additional record flags
*/

enum {
	exportVideo			= 0x01,						// When set, record video to tape (used with insert record mode)
	processCCdata		= 0x02,						// Read and record CC data from transmit stream
	previewEdit			= 0x04,

	export_Size	= kPrForceEnumToIntValue
}; // used in exportFlags, above

/****************************************************************
**	Entry Point
*/
typedef PRDEVICEENTRY(*DeviceModEntryFunc)(short selector, DeviceHand theData);


/****************************************************************
**	Selectors
*/

enum PrDeviceSelector {
	dsInit = 0,					// Create any structure(s), pick an operating mode, no dialogs here
	dsSetup,					// prompt for any user dialogs
	dsExecute,					// perform command
	dsCleanup,					// dispose any allocated structure(s)
	dsRestart,					// re-start any features, used at program startup to reconnect device;  if restart fails (maybe it rejected deviceData as being and old version), dsInit will immediately be called
	dsQuiet,					// disconnect from device but DON'T dispose allocated structures
	dsHasOptions,				// Pro 2.0: alternate way to determine if a device plugin has options. Added for lazy init devices that do not have options, so they can tell us that. 
								// For combatibility with old device plugins, if a device returns dmUnsupported or dmNoError to this selector, it is assume the device *does* have options. 
								// If a device returns dmHasNoOptions, the host knows there are no options.
	dsSelector_Size = kPrForceEnumToIntValue
};

#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif /* PRSDKDEVICE_H */
