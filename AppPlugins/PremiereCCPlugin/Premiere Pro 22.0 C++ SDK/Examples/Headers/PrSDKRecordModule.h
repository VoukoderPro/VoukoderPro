/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2000 Adobe Systems Incorporated                       */
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


// This header is part of the public Premiere Pro SDK.



//===================================================================
//
//  
//	PrSDKRecordModule.h
//
//	This header will be used by all recorder plug-ins.
//
//===================================================================

#ifndef	PRSDKRECORDMODULE_H
#define PRSDKRECORDMODULE_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKCLASSDATA_H
#include "PrSDKClassData.h"
#endif

#ifndef PRSDKPLUGMEMORY_H
#include "PrSDKPlugMemory.h"
#endif

#ifndef PRSDKPLUGSUITES_H
#include "PrSDKPlugSuites.h"
#endif

#ifndef PRSDKMALERRORS_H
#include "PrSDKMALErrors.h"
#endif

#ifndef PRSDKSTRUCTS_H
#include "PrSDKStructs.h"
#endif

#ifndef	PRSDKAUDIOSUITE_H
#include "PrSDKAudioSuite.h"
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
#define RECMOD_VERSION_1		1	// 5.0
#define RECMOD_VERSION_2		2	// 5.0.1
#define RECMOD_VERSION_3		3	// 6.0
#define RECMOD_VERSION_4		4	// 7.0/Premiere Pro 1.0
#define RECMOD_VERSION_5		5	// Premiere Pro 1.5
#define RECMOD_VERSION_6		6	// Premiere Pro 2.0
#define RECMOD_VERSION_7		7	// CS3
#define RECMOD_VERSION_8		8	// Premiere Elements 3.0 added Stop motion
#define RECMOD_VERSION_9		9	// CS4
#define RECMOD_VERSION_10		10	// CS5
#define RECMOD_VERSION_11		11	// CS5.5
#define RECMOD_VERSION_12		12	// CS6
#define RECMOD_VERSION			RECMOD_VERSION_12


/****************************************************************
**	Error Definitions
*/
enum prRecordmodError
{
	rmNoErr =					0,
	rmUnsupported = 			1,		// unsupported entry selector
	rmAudioRecordError =		2,		// general audio recording error
	rmVideoRecordError =		3,		// general video recording error
	rmVideoDataError =			4,		// data rate too high to record (return this if too many frames get dropped)
	rmDriverError =				5,		// general driver error
	rmMemoryError =				6,		// memory error occurred
	rmDiskFullError =			7,		// disk full when recording
	rmDriverNotFound =			8,		// can't connect to capture driver
	rmStatusCaptureDone =		9,		// returned from recmod_StartRecord if cap module completes recording and no error occured
	rmCaptureLimitReached =		10,		// returned from recmod_ServiceRecord if the cap module is limiting capture timing itself and has hit the recordlimit time.
	rmBadFormatIndex =			11,		// Invalid format index - used to stop rmGetIndAudFormat queries
	rmFormatAccept =			12,		// The output format is valid
	rmFormatDecline =			13,		// The module cannot capture to this format
	rmPrerollAbort =			14,		// the preroll function aborted.
	rmUserAbort =				15,		// error returned from recmodStartRecord if cap completes and user aborted.
	rmFileSizeLimit =			16,		// error returned from recmod_ServiceRecord if file size limit was reached 
	rmFramesDropped =			17,		// error returned from dropped frames
	rmDeviceRemoved =			18,		// The capture device was removed during capture (capture was successful until the device was removed)
	rmDeviceNotFound =			19,		// The capture device is not available
	rmCapturedNoFrames =		20,		// Captured zero frames
	rmEndOfScene =				21,		// If detecting scenes, and record module senses the end of a scene
	rmNoFrameTimeout =			22,		// Haven't seen any frames in a while, maybe the tape stopped or hit blank part of tape?
	rmCantDetectScenesError =	23,		// Returned if the record module can't find the information it needs to properly judge scene bounds
	rmCantFindRecordInPoint =	24,		// Returned if capturing in to out and the recorder can't find the in point timecode
	rmLastErrorSet =			25,		// The plugin set the last error string for UI display using the SuitePea ErrorSuite.
	rmLastWarningSet =			26,		// The plugin set the last warning string for UI display using the SuitePea ErrorSuite.
	rmLastInfoSet =				27,		// The plugin set the last info string for UI display using the SuitePea ErrorSuite.
	rmIllegalAudioFormatChange = 28,	// The audio format changed during capture, and the plugin says that's bad.  Plugin should save what was captured to that point.
	rmRequiresCustomPrefsError = 29,	// New in CS3, no capture prefs found, need to generate custom prefs
	rmRequiresRoyaltyContent   = 30,	// New in CC 8.0. Royalty codec not licensed.
	rmBadFile =					-1,

	rmIsCacheable =				400		// Returned from recmod_startup, non-zero if the plugin can be lazy-inited,
										// zero if the plugin must be loaded from disk on startup
};


/****************************************************************
**	Enumerations
*/
enum
{
	PR60RECMODAPI		= 6				// Premiere 6 Recmod for better user experience
};

//	Audio format support
enum
{
	aud8Mono =		0x000000001,
	aud8Stereo =	0x000000002,
	aud16Mono =		0x000000004,
	aud16Stereo =	0x000000008,
	audAllFormats =	(aud8Mono | aud8Stereo | aud16Mono | aud16Stereo),
	audBadFormat =	0xffffffff
};

const csSDK_int32	audUncompressed = 'RAW ';

const csSDK_int32	kRecMaxSetups = 4;
const csSDK_int32	kRecMaxConnectedDevices = 10;

/*  NEW in 7.0.  The scene searching algorithm happens in two passes.
    The first pass is at a play fast forward.  In this mode, the plugin
	is expected to pass back a valid range of timecodes the scene break
	may occur.  After premiere gets a good range, it will call stopSceneSearch
	and start a new slowScan scene search in the opposite direction.  The
	plugin is expected to report back the 2 timecodes that lie on either edge
	of the scene break.
*/
enum 
{
	sceneSearch_FastScan,
	sceneSearch_SlowScan
};

enum
{
	kMALFieldsNone =				0,	// Full frame
	kMALFieldsUpperFirst =			1,	// topmost line is dominant
	kMALFieldsLowerFirst =			2,	// topmost+1 line is dominant
	kMALFieldsUnknown =				3,	// has fields, unknown dominance
	kMALFieldsInvalid =				4,	// unknown if fields, thus invalid

	kMALFields_Size =				kPrForceEnumToIntValue
};

// capture info, passed in recmod_QueryInfo
// since record module may not be able to determine all of its current settings,
// there are "kMALInvalid" values for each field.
const csSDK_int32	kMALInvalidCompressorType =		'XxXx';
const csSDK_int32	kMALInvalidSize	 =				0;
const csSDK_int32	kMALInvalidDepth =				0;
const csSDK_int32	kMALInvalidPixelAspectRatio =	0x7FFF0001;
const csSDK_int32	kMALInvalidQuality =			-1;
const csSDK_int32	kMALInvalidFieldType =			kMALFieldsInvalid;
const csSDK_int32	kMALInvalidRate	 =				0;
const csSDK_int32	kMALInvalidAudFormat =			-1;

const csSDK_int32	kRecCapInfoRecVersion =			1;


/****************************************************************
**	Callback Functions
*/

// Status function
// The module should call this function while capturing to return status.
// If the module controls the capture, not returning from a recmodStartRecord
// call until capture is complete, it can determine if capturing should be
// aborted by checking the return value from this function; The host returns TRUE 
// if capture should be halted.
typedef int (*StatusDispFunc)(void* callbackID, char *stattext, int framenum);

// Preroll function for capturing with device control
// This function MUST be called just before beginning a capture.
// Host returns a prDevicemodError to inform why the preroll failed
// recmodID is the value passed in recCapParmsRec
typedef csSDK_int32 (*PrerollFunc)(void* callbackID);

// Obsolete in favor of SceneCapturedFunc (defined below)
// Callback to return the scene detected by the record module
// The 'inSceneEdgeTimecode' parameter marks the timecode of the scene edge, if it can be determined exactly.
// If it cannot, it marks the approximated timecode of the edge, and the 'inEarliestSceneEdgeTimecode' and
// 'inGreatestSceneEdgeTimecode' parameters mark the earliest and latest possible timecodes that the scene
// would fall in between.  If the scene transition can be determined exactly, all three return parameters
// will be set to the same value.
typedef void (*ReportSceneFunc)(void*	callbackID, 
								csSDK_uint32	inSceneEdgeTimecode, 
								csSDK_uint32	inEarliestSceneEdgeTimecode, 
								csSDK_uint32	inGreatestSceneEdgeTimecode);


// NEW in 7.0
// Callback to return the format of the media stream by the record module so that the 
// UI may display the correct aspect ratio.
typedef void (*FormatChangedFunc)(void*		callbackID,			
								  unsigned int		inPixelAspectRatioNum, 
								  unsigned int		inPixelAspectRatioDen,
								  unsigned int		inMaxFrameWidth,
								  unsigned int		inMaxFrameHeight,
								  TDB_TimeRecord	inFramerate,
								  int				isDropFrame);

typedef void (*GetDeviceTimecodeFunc)(	void* inCallbackID,
										csSDK_uint32 *outTimecode,
										TDB_TimeRecord *outFrameRate,
										int *outIsDropFrame);
										
typedef struct {
	float shortAmplitude;					// current volume value at the time of calling
	float longAmplitude;					// averaged peak volume value since the last call
	bool  hasClipped;						// has clipping occurred since the last status call?
} AudioPeakChannelData;

const csSDK_uint32	kRecMaxAudioPeakData = 16;

typedef struct {
	csSDK_uint32		 numOfUsedChannels;				// indicates how many channels are in use
	AudioPeakChannelData data[kRecMaxAudioPeakData];	// supports up to 16 channels
} recAudioPeakData;

//
//	NEW in CS5
//	Callback used to return audio metering data to the host.
//	The shortAmplitude is the current (instant) audio level
//
typedef void (*AudioPeakDataFunc)(void* callbackID,
								  recAudioPeakData* inAudioPeakData);
	
// Audio settings record used by the audio recording services:

typedef struct {
	csSDK_int32		capAudRate;				// 0:11 khz, 1:22 khz, 2:44 khz 
	short			capAudStereo;			// 0:mono, 1:stereo
	short			capAud16;				// 0:8 bit samples, 1:16 bit samples
	char			AudCompRec[512];		// compressed audio record (WAVEFORMATEX)
} AudioRec;

// Utility functions passed in with the Standard Parms
typedef struct
{
	ClassDataFuncsPtr	classFuncs;				// class functions
	PlugMemoryFuncsPtr	memoryFuncs;			// memory functions
} recCallbackFuncs;

// Standard parms
typedef struct
{
	int					rmInterfaceVer;		// version # of record interface (RECMOD_VERSION)
	recCallbackFuncs	*funcs;
	piSuitesPtr			piSuites;
} rmStdParms;


/****************************************************************
**	Structures
*/

// struct for setup item info
typedef struct 
{
	char	name[256];
} recSetupItem;

// struct for setup item info
typedef struct 
{
	prUTF16Char	name[256];
} recSetupItem8;

// struct filled in by recmod_Startup message
// These are the capabilities/info for the record module 
typedef struct 
{
	csSDK_int32		recmodID;				// Runtime ID for the module - don't change this!
	csSDK_int32		fileType;				// The file type (AVI, MOOV etc)
											// On windows, how is this matched up to extensions?
	csSDK_int32		classID;				// The classID for the module
											// This is no longer used for identification in Pro 2.0. However, if you
											// want pre-2.0 projects to map to this recorder, fill in the classID that
											// was used in the older projects and Premiere will update the projects
											// accordingly
											// different plugin types (i.e. play and record modules).
	int				canVideoCap;			// can capture video
	int				canAudioCap;			// can capture audio (*and* audio is available!)
	int				canStepCap;				// can capture async frames to a file on command
	int				canStillCap;			// can capture a still and return as a buffer
	int				canRecordLimit;			// accepts recording time limits
	int				acceptsTimebase;		// can capture to an arbitrary timebase
	int				acceptsBounds;			// can capture to an arbitrary size
	int				multipleFiles;			// may capture to multiple files or external drives
	int				canSeparateVidAud;		// can capture video & audio to different system drives
	int				canPreview;				// can display continuous preview frames
	int				wantsEvents;			// capture module wants to process messages
	int				wantsMenuInactivate;	// capture module wants to an inactivation when a menu goes down
	int				acceptsAudioSettings;	// can accept audio settings from the host. If this is false, record module does its own settings.
	int				canCountFrames;			// can count frames and quit when count is reached
	int				canAbortDropped;		// can abort when frames are dropped
	int				requestedAPIVersion;	// expected API version (allows for improved user experience)
	int				canGetTimecode;			// can get timecode from the capture stream
	int				reserved[16];			// reserved capabilities
											// capture bounds limits; ignored if acceptsBounds != true 
	int				activeDuringSetup;		// don't deactivate the record module when before a recmodSetup selector.
	csSDK_int32		prefTimescale;			// preferred timebase to capture to (if acceptsTimebase was true)
	csSDK_int32		prefSamplesize;			// preferred dividend of timebase (if acceptsTimebase was true)
	csSDK_int32		minWidth;				// Minimum width (set min = max to allow only one size)
	csSDK_int32		minHeight;				// Mininum height
	csSDK_int32		maxWidth;				// Maximum width
	csSDK_int32		maxHeight;				// Maximum height
	int				prefAspect;				// 16.16: pixel aspect ratio of source video (e.g., 648.720)
	csSDK_int32		prefPreviewWidth;		// onscreen width we prefer to display at
	csSDK_int32		prefPreviewHeight;		// onscreen width we prefer to display at
	char			recmodName[256];		// The displayable name for this module
	csSDK_int32		audioOnlyFileType;		// file type to create for audio-only captures. If 0, then type will be same as video type
	int				canSearchScenes;		// NEW in 7.0.  Record module can detect a scene transition for searching purposes.
	int				canCaptureScenes;		// NEW in 7.0.  Record module can return a value when it has hit the end of a scene.
	prPluginID		outRecorderID;			// NEW in Pro 2.0. A GUID identifier is now required for all recorders.
} recInfoRec, *recInfoPtr;

// struct filled in by recmod_Startup8 message (Unicode)
// These are the capabilities/info for the record module 
typedef struct 
{
	csSDK_int32		recmodID;				// Runtime ID for the module - don't change this!
	csSDK_int32		fileType;				// The file type (AVI, MOOV etc)
											// On windows, how is this matched up to extensions?
	csSDK_int32		classID;				// The classID for the module
											// This is no longer used for identification in Pro 2.0. However, if you
											// want pre-2.0 projects to map to this recorder, fill in the classID that
											// was used in the older projects and Premiere will update the projects
											// accordingly
											// different plugin types (i.e. play and record modules).
	int				canVideoCap;			// can capture video
	int				canAudioCap;			// can capture audio (*and* audio is available!)
	int				canStepCap;				// can capture async frames to a file on command
	int				canStillCap;			// can capture a still
	int				canRecordLimit;			// accepts recording time limits
	int				acceptsTimebase;		// can capture to an arbitrary timebase
	int				acceptsBounds;			// can capture to an arbitrary size
	int				multipleFiles;			// may capture to multiple files or external drives
	int				canSeparateVidAud;		// can capture video & audio to different system drives
	int				canPreview;				// can display continuous preview frames
	int				wantsEvents;			// capture module wants to process messages
	int				wantsMenuInactivate;	// capture module wants to an inactivation when a menu goes down
	int				acceptsAudioSettings;	// can accept audio settings from the host. If this is false, record module does its own settings.
	int				canCountFrames;			// can count frames and quit when count is reached
	int				canAbortDropped;		// can abort when frames are dropped
	int				requestedAPIVersion;	// expected API version (allows for improved user experience)
	int				canGetTimecode;			// can get timecode from the capture stream
	int				reserved[16];			// reserved capabilities
											// capture bounds limits; ignored if acceptsBounds != true 
	int				activeDuringSetup;		// don't deactivate the record module when before a recmodSetup selector.
	csSDK_int32		prefTimescale;			// preferred timebase to capture to (if acceptsTimebase was true)
	csSDK_int32		prefSamplesize;			// preferred dividend of timebase (if acceptsTimebase was true)
	csSDK_int32		minWidth;				// Minimum width (set min = max to allow only one size)
	csSDK_int32		minHeight;				// Mininum height
	csSDK_int32		maxWidth;				// Maximum width
	csSDK_int32		maxHeight;				// Maximum height
	int				prefAspect;				// 16.16: pixel aspect ratio of source video (e.g., 648.720)
	csSDK_int32		prefPreviewWidth;		// onscreen width we prefer to display at
	csSDK_int32		prefPreviewHeight;		// onscreen width we prefer to display at
	prUTF16Char		recmodName[256];		// The displayable name for this module
	csSDK_int32		audioOnlyFileType;		// file type to create for audio-only captures. If 0, then type will be same as video type
	int				canSearchScenes;		// NEW in 7.0.  Record module can detect a scene transition for searching purposes.
	int				canCaptureScenes;		// NEW in 7.0.  Record module can return a value when it has hit the end of a scene.
	prPluginID		outRecorderID;			// NEW in Pro 2.0. A GUID identifier is now required for all recorders.
} recInfoRec8, *recInfo8Ptr;

typedef struct 
{
	int				customSetups;			// number of custom setup items ( < kRecMaxSetups)
	csSDK_int32		enableflags;			// flags for which setups are available (flag = 1 << setupnum)
	recSetupItem	setups[kRecMaxSetups];
} recCapSetups;

typedef struct 
{
	int				customSetups;			// number of custom setup items ( < kRecMaxSetups)
	csSDK_int32		enableflags;			// flags for which setups are available (flag = 1 << setupnum)
	recSetupItem8	setups[kRecMaxSetups];
} recCapSetups8;

//  audio format support
//	Structure for the audio formats an audio compressor supports
//	ie 16bit mono @ 44100
typedef struct
{
	int				audioDepths;			// audio format support bit field (use audio format support #defines)
	csSDK_int32		audioRate;				//	Audio rates the compressor supports
} recAudioFormat, *recAudioFormatPtr;

typedef struct
{
	csSDK_int32			recmodID;			// ID for the module - don't change this!
	csSDK_int32			subtype;			// File subtype supported. Generally a compressionID, but not always.
											// subtype==compUncompressed is a special case for "no compression"
											// subtype==compBadFormat is an invalid format
	char				name[256];			// The displayable name for this subtype
	recAudioFormatPtr	audioFormats;		// A ptr to a list of audio formats that the compressor supports
											// if this param is nil, it supports any rates and bits
} recAudioInfoRec;

typedef struct
{
	PrAudioSampleType	sampleType;					//A bit depth from the AudioBitDepth enum.		
	float				sampleRate;					//A sample rate in Hz. Use 0 for any sample rate.
	PrAudioChannelType	channelType;				//A channel type from the AudioChannelType enum.
} recAudioFormat7, *recAudioFormat7Ptr;

typedef struct
{
	csSDK_int32			recmodID;			// ID for the module - don't change this!
	csSDK_int32			subtype;			// File subtype supported. Generally this will be a compressionID,
											// but not always.
											// subtype==compUncompressed is a special case for "no compression"
											// subtype==compBadFormat is an invalid format

	char				name[256];			// The displayable name for this subtype

	csSDK_int32			numFormats;			// The number of formats in audioFormats
	
	recAudioFormat7Ptr	audioFormats;		// A ptr to a list of audio formats that the compressor supports
											// if this param is nil, it supports any rates and bits
} recAudioInfoRec7;

// struct that describes playback position on screen
typedef struct 
{
	prWnd		wind;		// window/embedded view to display into (HWND on PC, NSView* on Mac)
	int			originTop;	// offset in pixels from top of window to display to
	int			originLeft;	// offset in pixels from top of window to display to
	int			dispWidth;	// width of display area
	int			dispHeight;	// height of display area
	int			mustresize;	// the display must fit into these bounds; see note in recmod_SetDisp
} recDisplayPos;

// recmod_Open struct
typedef struct 
{
	recDisplayPos		disp;				// the display area
	void*				callbackID;			// instance of this open recording session. Used for callback functions
	char				*setup;				// private record module settings from previous session (or nil if none)
	FormatChangedFunc	formatFunc;			// NEW for 7.0 callback to inform Premiere of our aspect ratio.
	short				isDropFrame;		// NEW for Pro 2.0: The current segment of tape timecode is drop frame encoded
	AudioPeakDataFunc	audioPeakDataFunc;	// NEW for CS5: Callback function to send audio metering data to the host.	
} recOpenParms;

// recmod_StartRecord struct
typedef struct 
{
	unsigned int	pixelAspectRatioNum;	// The numerator part of the pixel aspect ratio of the captured file
	unsigned int	pixelAspectRatioDen;	// The denominator part of the pixel aspect ratio of the captured file	
	char			timeCode[31];			// The timecode of the the captured file, uses ; as separaters if drop frame, : of non-drop
	TDB_TimeRecord	tdb;					// NEW in 7.0.  The timebase of the captured file
	char			date[31];				// NEW in PRE 7.0 (2007-2008) The date of the the captured file, format "d/m/y" or "d/m/y h:m" or "d/m/y h:m:s"
} recCapturedFileInfo;

// file spec for open
typedef struct
{
	short		volID;						// used on Mac only
	csSDK_int32	parID;						// used on Mac only
	char		name[256];					// file name on Mac, full path elsewhere
} recFileSpec;

typedef struct
{
	short			volID;						// used on Mac only
	csSDK_int32		parID;						// used on Mac only
	prUTF16Char		name[kPrMaxPath];			// file name on Mac, full path elsewhere
} recFileSpec8;

typedef struct 
{
	prParentWnd	parentwind;					// (modal) parent window to build the setup window on (HWND on Win, NSWindow* on Mac)
	int			setupnum;
	char		*setup;
} recSetupParms;

//TODO: Yogita: These standards are defined at various places in the code and should be ideally at one location
enum recFrameRate
{
	kRecUnknownStandard	= 0,
	kRecNTSCStandard	= 1,
	kRecPALStandard 	= 2,
	kHD720p24			= 3,
	kHD720p23976		= 4,
	kHD720p2997			= 5,
	kHD1920x1080_2997	= 6
};

typedef struct
{
	recFrameRate frameRate;
}recVideoStandard;

// Callback to notify Premiere that a scene has been captured.
// Premiere returns the recFileSpec to designate a filename for the next scene to 
// capture and reserves memory for and returns recCapturedFileInfo for the next capture.
typedef void (*SceneCapturedFunc8)(	void*			inCallbackID,
									prUTF16Char			*inFileCaptured, 
									recFileSpec8			*outNextSceneFilename,
									recCapturedFileInfo	**outFileInfo);

// Obsolete. Use SceneCapturedFunc8
typedef void (*SceneCapturedFunc)(void*			inCallbackID,
								  char					*inFileCaptured, 
								  recFileSpec			*outNextSceneFilename,
								  recCapturedFileInfo	**outFileInfo);


// capture params, passed in recmod_PrepRecord
typedef struct 
{
	void*				callbackID;				// must be passed back with statFunc, prerollFunc callbacks
	int					stepcapture;			// capture is a step capture (0 = streaming capture)
	int					capVideo;				// capture video
	int					capAudio;				// capture audio 
	int					width;					// width to capture (if acceptsBounds was true)
	int					height;					// height to capture (ditto)
												// timebase: timescale/samplesize == fps, e.g 2997/100
	csSDK_int32			timescale;				// timebase to capture to (if acceptsTimebase was true)
	csSDK_int32			samplesize;				// dividend of timebase (if acceptsTimebase was true)
	csSDK_int32			audSubtype;				// compressed audio format to capture (or audUncompressed)
	csSDK_uint32		audrate;				// The audio rate in samples per second.
	int					audsamplesize;			// The audio samplesize, 0 == 8bit, 1 == 16bit
	int					stereo;					// If 1, audio is stereo, othewise it's mono
	char				*setup;					// pointer to setup storage created by setup calls
	int					abortondrops;			// stop capturing if any frames get dropped
	int					recordlimit;			// recording limit in seconds (if canRecordLimit was true)
	recFileSpec8		thefile;				// file to capture to (valid on recmod_PrepRecord only)
	StatusDispFunc		statFunc;				// function pointer used to return capture status (streaming only)
	PrerollFunc			prerollFunc;			// function to call _just_ before capture begins; used for device control preroll (streaming only)
	csSDK_int32			frameCount;				// Version 2 only! - if module sets canCountFrames, frameCount is # of frames to capture and no device polling is done
	char				reportDrops;			// Version 2 only! - If true, report dropped frames
	short				currate;				// Version 2 only! - the fps of the deck, 30, 25, or 24
	short				timeFormat;				// Version 3 only - 0=non-drop frame, 1=drop frame timecode
	csSDK_int32			timeCode;				// Version 3 only - timecode for in-point of capture (-1=ignore)
	csSDK_int32			inHandleAmount;			// Version 3 only - # frames of "handle" before in-point of capture
	ReportSceneFunc		reportSceneFunc;		// NEW in 7.0.  Use this to report the scenes
	int					captureScenes;			// NEW in 7.0.  True if user requests capture of scene.
	SceneCapturedFunc8	sceneCapturedFunc;		// NEW in 7.5.  Use this to report scene captured
	bool				recordImmediate;		// NEW in 7.5.  If this is set, record immediately after device control returns from seek for preroll, don't wait for a timecode
	GetDeviceTimecodeFunc getDeviceTimecodeFunc; // CS3. Added so the Mac could get device timecode in the HDV recorder for HDV1.
} recCapParmsRec8;

// Obsolete. Use recCapParmsRec8
typedef struct 
{
	void*				callbackID;				// must be passed back with statFunc, prerollFunc callbacks
	int					stepcapture;			// capture is a step capture (0 = streaming capture)
	int					capVideo;				// capture video
	int					capAudio;				// capture audio 
	int					width;					// width to capture (if acceptsBounds was true)
	int					height;					// height to capture (ditto)
												// timebase: timescale/samplesize == fps, e.g 2997/100
	csSDK_int32			timescale;				// timebase to capture to (if acceptsTimebase was true)
	csSDK_int32			samplesize;				// dividend of timebase (if acceptsTimebase was true)
	csSDK_int32			audSubtype;				// compressed audio format to capture (or audUncompressed)
	csSDK_uint32		audrate;				// The audio rate in samples per second.
	int					audsamplesize;			// The audio samplesize, 0 == 8bit, 1 == 16bit
	int					stereo;					// If 1, audio is stereo, othewise it's mono
	char				*setup;					// pointer to setup storage created by setup calls
	int					abortondrops;			// stop capturing if any frames get dropped
	int					recordlimit;			// recording limit in seconds (if canRecordLimit was true)
	recFileSpec			thefile;				// file to capture to (valid on recmod_PrepRecord only)
	StatusDispFunc		statFunc;				// function pointer used to return capture status (streaming only)
	PrerollFunc			prerollFunc;			// function to call _just_ before capture begins; used for device control preroll (streaming only)
	csSDK_int32			frameCount;				// Version 2 only! - if module sets canCountFrames, frameCount is # of frames to capture and no device polling is done
	char				reportDrops;			// Version 2 only! - If true, report dropped frames
	short				currate;				// Version 2 only! - the fps of the deck, 30, 25, or 24
	short				timeFormat;				// Version 3 only - 0=non-drop frame, 1=drop frame timecode
	csSDK_int32			timeCode;				// Version 3 only - timecode for in-point of capture (-1=ignore)
	csSDK_int32			inHandleAmount;			// Version 3 only - # frames of "handle" before in-point of capture
	ReportSceneFunc		reportSceneFunc;		// NEW in 7.0.  Use this to report the scenes
	int					captureScenes;			// NEW in 7.0.  True if user requests capture of scene.
	SceneCapturedFunc	sceneCapturedFunc;		// NEW in 7.5.  Use this to report scene captured
	bool				recordImmediate;		// NEW in 7.5.  If this is set, record immediately after device control returns from seek for preroll, don't wait for a timecode
} recCapParmsRec;

typedef struct 
{
	int					alphaLevel;				// alpha of the image to be blended
	recFileSpec8		thefile;				// name of file which has to be blended.

}recStillCapChangeParamsRec;

typedef struct 
{
	int					width;					// width to capture
	int					height;					// height to capture
	int					depth;					// returned depth, 24 or 32bpp allowed
	char				*pixels;				// pixels captured at depth reported, buffer allocated with memfuncs
	int					rowbytes;				// rowbytes of captured data
	recFileSpec8		thefile;				// file to capture to (valid on recmod_StillImageCpature only)
} recStillCapParmsRec;


// NEW in 7.0.  Use this to implement goto next/previous scene functionality.
// Selector will only be passed down if recInfoRec's 'canDetectScenes' field is 
// set to true.  'searchingForward' is provided as a hint as the state of the device, and
// the 'reportSceneFunc' should be used to notify Premiere of a scene change.
typedef struct 
{
	void*				callbackID;				// must be passed in to the callbacks
	ReportSceneFunc		reportSceneFunc;		// Use this to report the scenes
	int					searchingForward;		// True if the tape is playing forward
	int					searchMode;				// Either FastScan or SlowScan
	short				isDropFrame;				// Version 3 only - 0=non-drop frame, 1=drop frame timecode
	csSDK_int32			earliestTimecode;		// Only set for SlowScan, inPoint for range to report scene edge
	csSDK_int32			greatestTimecode;		// Only set for SlowScan, outPoint for range to report scene edge
} recSceneDetectionParmsRec;

typedef struct 
{
	csSDK_int32			version;				// version of this struct (kRecCapInfoRecVersion)
	int					timeScale;				// timebase to capture (timeScale/sampleSize == FPS, eg. 2997/100)
	int					sampleSize;				// dividend of timebase
	csSDK_int32			vidSubType;				// video compressor type
	int					width;					// frame width to capture
	int					height;					// frame height to capture
	int					depth;					// # bits per pixel (8/16/24/32)
	int					fieldType;				// type of fields (kMALFields)
	int					quality;				// video compression quality 0-100
	csSDK_int32			pixelAspectRatio;		// (16.16) pixel aspect ratio (eg. 0.9 is (9<<16) | 10)
	csSDK_int32			audSubType;				// audio compressor type
	int					audRate;				// The audio rate in samples per second.
	int					audSampleSize;			// The audio samplesize, 0 == 8bit, 1 == 16bit
	int					audStereo;				// 0=mono, 1=stereo
	int					reserved[10];
	char				*setup;					// pointer to setup storage created by setup calls
} recCapInfoRec;

// timecode params, passed in recmod_Idle
typedef struct 
{
	csSDK_int32			status;					// 0=OK, 1=unknown/stale, 2=error
	short				currate;				// the fps of the timecode, 30, 25, or 24
	short				timeFormat;				// 0=non-drop frame, 1=drop frame timecode
	csSDK_int32			timeCode;				// current timecode
	short				autoDetectDropness;		// set in device control, relinquishes chore of determining dropness to recorder
} recGetTimecodeRec;

typedef struct 
{
	prUTF16Char			name[246];
}recDeviceInfoRec;

typedef struct 
{
	recDeviceInfoRec	devices[kRecMaxConnectedDevices];
	int					deviceCount;
}recConnectedDeviceListRec;

/****************************************************************
**	Entry Point
*/
typedef PREMPLUGENTRY (* RecordEntryFunc)(csSDK_int32 selector, rmStdParms *stdparms, void* param1, void* param2);


/****************************************************************
**	Selectors
*/
enum PrRecmodSelector {
	recmod_Startup,
	recmod_Shutdown,
	recmod_Open,
	recmod_Close,
	recmod_PrepRecord,
	recmod_StartRecord,
	recmod_ServiceRecord,
	recmod_StopRecord,
	recmod_CloseRecord,
	recmod_StepRecord,
	recmod_StillRecord,
	recmod_ShowOptions,
	recmod_SetActive,
	recmod_GetAudioIndFormat,
	recmod_Idle,
	recmod_SetDisp,
	recmod_DisplayFrame,				// Unused
	recmod_QueryFormat,					// Unused
	recmod_QueryDisplayPos,				// Unused
	recmod_GetSetupInfo,
	recmod_ProcessEvent,				// Unused
	recmod_QueryInfo,
	recmod_GetAudioIndFormat7,			// Reserved, do not use
	recmod_StartSceneSearch,			// NEW in 7.0
	recmod_StopSceneSearch,				// NEW in 7.0
	recmod_ServiceSceneSearch,			// NEW in 7.0
	recmod_Startup8,					// New in PPro 2.0
	recmod_GetSetupInfo8,				// New in PPro 2.0
	recmod_PrepRecord8,					// New in PPro 2.0
	recmod_DeviceStatusChanged,			// New in PPro 2.0
	recmod_StartStopMotionCapture,		// New in Pre 3.0
	recmod_StopStopMotionCapture,		// New in Pre 3.0
	recmod_ChangeStopMotionSettings,	// New in Pre 3.0
	recmod_GetConnectedDeviceNames,		// New in Pre 7.0
	recmod_SelectCaptureDevice,			// New in Pre 7.0
	recmod_GetConnectedAudioDeviceNames,// New in Pre 7.0
	recmod_SelectAudioCaptureDevice,		// New in Pre 7.0
	recmod_GetSelectedDeviceVideoStandard// New in Pre 7.0
};

/*
Record Module Messages:

Message:	recmod_Startup8
			Called once at application launch.
			The module should fill in the recInfoRec with it's attributes
			If the plugin does not have to be loaded immediately upon application startup,
			then it should return rmIsCacheable instead of rmNoErr.
			param1 : recInfoRec8 *

Message:	recmod_Startup (deprecated)
			Called once at application launch.
			The module should fill in the recInfoRec with it's attributes
			If the plugin does not have to be loaded immediately upon application startup,
			then it should return rmIsCacheable instead of rmNoErr.
			param1 : recInfoRec *
			
Message:	recmod_Shutdown
			Called once at application shutdown.
			
Message:	recmod_GetAudioIndFormat
			Called repeatedly to query for audio compressors that the module
			supports. Query continues as long as rmNoErr is returned.
			param1 : recAudioInfoRec *
			param2 : index
			
Message:	recmod_Open
			Called to open the record module
			param1 : private module storage char **
			param2 : recOpenParms *

Message:	recmod_SetDisp
			Called to modify the display position for previews
			Sent if a window moves or is resized.
			The module can modify the recDisplayPos record if the display position bounds
			cannot be accepted. If the record is modified, the module may be called back
			again with the modified bounds but at a new left/top position.
			If the 'mustresize' flag is true, then the module must fit the preview within
			the specified bounds; if this cannot be done, then the module should display
			a black or gray frame for a preview. The 'mustresize' flag will be set to
			true when the record settings dialog in Premiere is displayed.
			param1 : private module storage
			param2 : recDisplayPos *

Message:	recmod_ShowOptions
			Called to display the customs when the user requests it.
			A recSetupParms record is passed to the module, which contains
			the setup number requested and a char *. The char * will be nil
			if no setups have ever been requested, or will be a pointer to memory
			containing the last setup used. 
			Notes:
			- The memory must be allocated using the PlugMemoryFuncs.
			- All setups share the same memory, and only one memory record
			will be preserved. If there are several different setup record,
			they must all be fit within one flattened memory allocation.
			param1 : private module storage
			param2: recSetupParms *

Message:	recmod_StartRecord
			Called to tell a record module to do the capture.
			There are two models for doing the capture:
			1) Do all the capture in recmod_StartRecord. The capture module can control the capture
			and should return rmStatusCaptureDone when the capture is finished.
			2) Do processing during recmod_ServiceRecord. The capture module can return malNoError
			on recmod_StartRecord and Premiere will continually call recmod_ServiceRecord to give
			the record module time.
			param1 : private module storage
			param2 : ptr to recCapturedFileInfo, the capture module can fill this in with info
					about the file that was captured.
					For capture modules that do work on recmod_ServiceRecord, the ptr is valid until
					the recording finishes (recmod_StopRecord).
						
Message:	recmod_QueryInfo
			Called to give the recmod a chance to respond to updated settings.
			param1 : private module storage
			param2 : ptr to recCapInfoRec containing current settings.

Message:	recmod_GetAudioIndFormat7 (currently unused)
			Called repeatedly to query for audio compressors that the module
			supports. Query continues as long as rmNoErr is returned.
			param1 : recAudioInfoRec7 *
			param2 : index
*/

#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif // PRSDKRECORDMODULE_H
