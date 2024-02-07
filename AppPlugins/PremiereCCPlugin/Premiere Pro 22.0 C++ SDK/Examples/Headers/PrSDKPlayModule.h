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
/* Adobe Premiere Play Module plug-in definitions				   */
/*																   */
/*******************************************************************/

#ifndef	PRSDKPLAYMODULE_H
#define PRSDKPLAYMODULE_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKCLASSDATA_H
#include "PrSDKClassData.h"
#endif

#ifndef PRSDKPLUGSUITES_H
#include "PrSDKPlugSuites.h"
#endif

#ifndef PRSDKENTRY_H
#include "PrSDKEntry.h"
#endif

#ifndef PRSDKTIMESUITE_H
#include "PrSDKTimeSuite.h"
#endif

#ifndef PRSDKAUDIOSUITE_H
#include "PrSDKAudioSuite.h"
#endif

#ifndef PRSDKPIXELFORMAT_H
#include "PrSDKPixelFormat.h"
#endif

#ifndef PRSDKACTIVATIONEVENT_H
#include "PrSDKActivationEvent.h"
#endif

#ifndef PRSDKCAPTIONSTREAMFORMAT_H
#include "PrSDKCaptionStreamFormat.h"
#endif

#ifndef PRSDKQUALITY_H
#include "PrSDKQuality.h"
#endif

#ifndef PRSDKPLAYMODULEDEVICECONTROLSUITE_H
#include "PrSDKPlayModuleDeviceControlSuite.h"
#endif

#ifndef	PRSDKIMMERSIVEVIDEOTYPES_H
#include "PrSDKImmersiveVideoTypes.h"
#endif

#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************
**	Version
*/
#define PLAYMOD_VERSION_70		700	// 7.0/Premiere Pro 1.0
#define PLAYMOD_VERSION_75		750	// Premiere Pro 1.5
#define PLAYMOD_VERSION_80		800	// Premiere Pro 2.0
#define PLAYMOD_VERSION_85		850	// private
#define PLAYMOD_VERSION_90		900	// CS3
#define PLAYMOD_VERSION_100		1000 // CS4
#define PLAYMOD_VERSION_110		1100 // CS5
#define PLAYMOD_VERSION_115		1150 // CS5.5
#define PLAYMOD_VERSION_120		1200 // CS6
#define PLAYMOD_VERSION_130		1300 // CS7
#define PLAYMOD_VERSION			PLAYMOD_VERSION_130

/****************************************************************
**	Error Definitions
*/
typedef enum PrPlaymodReturnValue
{
	playmod_ErrNone = 0,			// no error
	playmod_ErrBadFile = 1,			// file is corrupt or unreadable
	playmod_ErrDriver = 2,			// driver error (hardware failure)
	playmod_ErrNotPreferred = 3,	// incorrect file subtype
	playmod_BadFormatIndex = 4,		// format index invalid (for GetIndFormat message)
	playmod_DeclinePlay = 5,		// I decline to play this back; find another module 
	playmod_ListWrongType = 6,		// can't play back this file -- wrong type; please render it.
	playmod_ListBadSpeed = 7,		// can't play back this file at this speed;
	playmod_CantAddSegment = 8,		// the cutlist can't add a segment
	playmod_Unsupported	= 9,		// unsupported call
	playmod_AudioOverload = 10,		// audio took more time to process than available
	playmod_OutOfRange = 11,		// callback param out of range (pmEnumAudioFileFunc)
	playmod_CannotRender = 12,		// cannot render frame in real-time
	playmod_RebuildCutlist = 13,	// (Not an error) return value used in playmod_GetFilePrefs
	playmod_CannotShiftLayer = 14,	// can't shift this layer
	//DO NOT USE, deprecated in CS5! playmod_OverlayDone	= 15,		// (Not an error) return value used in playmod_PutFrame
	playmod_UnsupportedPlaybackSpeed = 16,	// The requested speed is not supported.
	playmod_BroadcastPrefs = 17,	// Valid during playmod_GetFilePrefs. Tells premiere to call playmod_SetFilePrefs on all open play modules using the returned prefs.
	playmod_CannotRecord = 18,		// Returned if recording is requested and the playmod cannot support it.
	playmod_RenderAndPutFrame = 19, // Put segment could not render this segment, Premiere should render it and return it in a putframe.

	pmIsCacheable = 400,			// Returned from playmod_Startup, non-zero if the plugin can be lazy-inited,
									// zero if the plugin must be loaded from disk on startup

	playmod_ErrorForceSize = kPrForceEnumToIntValue
} prPlaymodError;


/****************************************************************
**	Type Definitions
*/
typedef csSDK_int32	PrPlayID;			// PlayID used in callbacks to identify this playmodule.


/****************************************************************
**	Enumerations
*/

/**
**	Mode types for pmPosRec
*/
typedef enum
{
	playmode_Stopped = 0,
	playmode_Playing = 1,
	
	//these playmodes are only supported in the context of SetDisplayStateProperties, not in the GetPos call.
	playmode_Scrubbing = 2,
	
	playmode_ForceSize = kPrForceEnumToIntValue
} pmPlayMode;

/**
**	Play module flags. 
**
**	capPlayLists - This playmodule can accept segments and play sequences.
**
*/
enum 
{
	pmFlag_canPlayLists = 4,				// can play lists of files
	
	pmFlag_ForceSize = kPrForceEnumToIntValue
};

/**
**	capability flags
*/
enum 
{
	PMCapPutFrame							= 0x00000080,	// Supports the playmod_PutFrame (VOut) function 
	PMCapWillReportDroppedFrames			= 0x00000800,	// This playmod will always report dropped frames during 1x playback
	PMCapCanLoopPlayback					= 0x00001000,	// This playmod can loop playback
	PMCapCanShuttlePlayback					= 0x00002000,	// This playmod can playback speeds between -4x and 4x
	PMCapCanZoom							= 0x00004000,	// This playmod can scale its display
	PMCapCanSafeMargin						= 0x00008000,	// This playmod can display safe margins (supports playmod_AdornSafeAreas).
	PMCapCanExport							= 0x00010000,	// If not set, the Export to Tape menu entry will be greyed out.
	PMCapPutSegment							= 0x00020000,	// DEPRECATED in CS4: This playmod can display RTSegments on the hardware (PutFrame functionality with RTSegments)
	PMCapSingleClipPlayerWantsRTSegments	= 0x00040000,	// This playmod wants segments for single clip players.
	PMCapCanDoFractionalResolution			= 0x00080000,	// This playmod can handle fractions of the display resolution for faster playback (called with playmod_SetFractionalResolution)
	PMCapCanPutTemporaryTimeline			= 0x00100000,	// This playmod can handle putting a temporary timeline. This is a replacement to the old PutSegment functionality
	PMCapSupportsDisplayStateProperties		= 0x00200000,	// This playmod supports playmod_SetDisplayStateProperties
	PMCapSupportsOverlayDrawing				= 0x00400000,	// This playmod supports drawing overlays through PrSDKPlayModuleOverlaySuite
	PMCapSupportsImmersiveVideo				= 0x00800000,	// This playmod supports immersive video through PrSDKImmersiveVideoSuite
	PMCap_ForceSize							= kPrForceEnumToIntValue
};

/**
**	PutFrame destinations
*/
typedef enum
{
	pmPutFrameDestination_VideoDesktop = 1,
	pmPutFrameDestination_VideoHardware = 2,

	pmPutFrameDestination_ForceEnumSize = kPrForceEnumToIntValue
} pmPutFrameDestination;

/**
**	Version enumeration
*/
typedef enum
{
	PR70RTAPI				= PLAYMOD_VERSION_70,	// Plugin supports version 7.0 selectors 

	PRRTAPI_ForceEnumSize	= kPrForceEnumToIntValue
} pmRTAPIVersion;

/**
**	The possible display modes for the player window.
*/
typedef enum 
{
	pmPlayerDisplay_Off = 0,
	pmPlayerDisplay_Composite = 1,
	pmPlayerDisplay_Alpha = 2,
	pmPlayerDisplay_DirectManipulation = 4,
	pmPlayerDisplay_AudioOnly = 5,					//	An audio-only clip is playing. Times should not be rounded to video frames,
													//	and no video display should be drawn.

	pmPlayerDisplay_ForceEnumSize = kPrForceEnumToIntValue 
} pmDisplayMode;


/****************************************************************
**	Structures
*/

//	Passed in startup to be filled in by the plugin
typedef struct
{
	prPluginID		outPlayerID;
	prUTF16Char		outDisplayName[256];
} pmStartupRec;

// file spec for open
typedef struct
{
	int		volID;			// used on Mac only
	int		parID;			// used on Mac only
	char	name[256];		// file name on Mac, full path elsewhere
} pmFileSpec;

typedef struct 
{
	prBool		enableSafeTitle;			// kPrTrue = overlay safe title, kPrFalse = disable overlay of safe title
	float		safeTitlePercentWidth;		// how far (percent-wise) to width inset the overlayed rectangle displaying safe title
											// i.e. for a 720x480 display, "0.1" would mean to inset the rectangle by 72x48 pixels
	float		safeTitlePercentHeight;		// how far (percent-wise) to height inset the overlayed rectangle displaying safe title
											// i.e. for a 720x480 display, "0.1" would mean to inset the rectangle by 72x48 pixels
	prBool		enableSafeAction;			// kPrTrue = overlay safe action, kPrFalse = disable overlay of safe action
	float		safeActionPercentWidth;		// how far (percent-wise) to width inset the overlayed rectangle displaying safe action
											// i.e. for a 720x480 display, "0.1" would mean to inset the rectangle by 72x48 pixels
	float		safeActionPercentHeight;	// how far (percent-wise) to height inset the overlayed rectangle displaying safe action
											// i.e. for a 720x480 display, "0.1" would mean to inset the rectangle by 72x48 pixels
	prBool		enable4to3;					// kPrTrue = overlay 4:3 safe margins, kPrFalse = disable overlay of 4:3 safe margins
} pmAdornSafeAreasParams;

// struct that describes playback position on screen
typedef struct 
{
	prWnd		wind;			// window to display into (HWND on PC, WindowPtr on Mac)
	int			originTop;		// offset in pixels from top of window to display to
	int			originLeft;		// offset in pixels from top of window to display to
	int			dispWidth;		// width of display area
	int			dispHeight;		// height of display area
} pmDisplayPos;

// struct that describes playback position on screen
typedef struct 
{
	prOffScreen	offscreen;		// offscreen to display into (HDC on PC, CGrafPtr on Mac)
	int			originTop;		// offset in pixels from top of window to display to
	int			originLeft;		// offset in pixels from top of window to display to
	int			dispWidth;		// width of display area
	int			dispHeight;		// height of display area
} pmOffscreenDisplayPos;

// playmod_Play struct
typedef struct
{
	PrTime				inTime;			// in point -- only used for looping
	PrTime				outTime;		// out point -- stop playing here or loop when this frame is reached
	PrTime				startTime;		// start playing from here
	prBool				loop;			// if kPrTrue, loop until playmod_Stop is sent
	float				speed;			// positive means forward, negative means backwards, zero will not be sent
	prBool				exportVideo;	// if kPrTrue, the playmod should play to the device (print to video and export to tape).
	PlayModuleDeviceID	deviceID;		// if transmit is true, then this ID will be non-zero to allow use of the DeviceControl suite.
	prBool				audioRecord;	// non-zero if audio is requested to be recorded. If the player calls InitHostAudio,
										// then no special action is required. If the player calls InitPluginAudio, then playmod_CannotRecord
										// must be returned to the host in this situation. Once plugin-based audio recording is
										// supported (after 7.0), this requirement will change.
} pmPlayParms;

/**
**	playmod_Activate data
*/
typedef struct 
{
	prBool				activate;			// kPrTrue or kPrFalse - whether to activate or not
	PrActivationEvent	activationEvent;	// Type of event causing the activation/deactivation
	PrFourCC			pluginClassID;
	PrFourCC			pluginFileType;
} pmActivateRec;

// playmod_GetPos struct
typedef struct 
{
	PrTime				position;			// current position
	pmPlayMode			mode;				// current play mode (see enum)
} pmGetPosRec;

typedef struct 
{
	PrTime				stepDistance;		// how far to step (including the direction)
} pmStepRec;

// playmod_Startup struct
typedef struct _pmCallbackFuncs pmCallbackFuncs;
typedef struct 
{
	int					pmInterfaceVer;		// version # of playmodule interface (PLAYMOD_VERSION)
	pmCallbackFuncs		*funcs;
	piSuitesPtr			piSuites;
} pmStdParms;

// playmod_Open struct
typedef struct 
{
	pmDisplayPos		disp;				// the display area
	pmFileSpec			theFile;			// the file spec
	pmPlayTimebase		timebase;			// the timebase and duration of the file
	PrFourCC			filetype;			// file type of this file
	PrFourCC			videoSubType;		// the video subtype of this file
	PrPlayID			playID;				// ID of this play instance. Used for callback functions
	PrTimelineID		timelineData;		// An identifer to be passed back to calls in the timeline suite.
	PrAudioChannelType	audioChannelType;	// The audio channel type of the file 
	PrMemoryPtr			playmodPrefs;		// A pointer to this playmodules preference data if this playmodule matches the editing mode (PrMemory)
	int					width;				// 'native' file width
	int					height;				// 'native' file height
	csSDK_uint32		pixelAspectNum;		// The numerator of the file's pixel aspect ratio
	csSDK_uint32		pixelAspectDen;		// The denominator of the file's pixel aspect ratio
	prFieldType			fieldType;			// field dominance of the file. prFieldsNone, prFieldsUpperFirst, prFieldsLowerFirst, prFieldsUnknown
	float				audioSampleRate;	// New in Pro 3.0
} pmPlayOpenParms;

// FILE info record filled in by playmod_GetInfo
typedef struct 
{
	int				width;					// -> 'native' file width
	int				height;					// -> 'native' file height
	prBool			hasVideo;				// <-> file has video
	prBool			hasAudio;				// <-> file has audio
	csSDK_int32		prefPreviewWidth;		// ->onscreen width we prefer to display at
	csSDK_int32		prefPreviewHeight;		// ->onscreen height we prefer to display at
	csSDK_uint32	pixelAspectNum;			// -> The numerator of the file or list's pixel aspect ratio
	csSDK_uint32	pixelAspectDen;			// -> The denominator of the file or list's pixel aspect ratio
} pmPlayInfoRec;

// Info rec filled in by module during playmod_GetIndFormat call
typedef struct 
{
	PrFourCC		filetype;				// file type supported
	PrFourCC		subtype;				// subfile type or zero
	PrFourCC		classID;				// The classID for the module
											// This is used to differentiate between compile modules that
											// support the same fileType and to cross reference between
											// different plugin types (i.e. play and record modules).
	csSDK_int32		playflags;				// see pmFlag enum above

// play list info
	int				hasSetup;				// If non-zero, the playback module has a private setup dialog
	int				capabilityFlags;		// see PMCap enum above
	int				requestedAPIVersion;	// Allows the playmod to request a version of API

	// future expansion
	int				reserved[32];			 // <- leave all the reserved fields to zero
} pmModuleInfoRec;

// structure passed during playmod_Newlist
typedef struct 
{
	pmPlayTimebase		listTimebase;
	pmDisplayPos		unused;				// This is deprecated. Wait for a playmod_SetDisp to get display info.
	PrPlayID			playID;				// ID of this play instance. Used for callback functions
	PrTimelineID		timelineData;		// An identifer to be passed back to calls in the timeline suite.
	PrMemoryPtr			playmodPrefs;		// A pointer to this playmodules preference data if this playmodule matches the editing mode (PrMemory)
	int					width;				// 'native' file width
	int					height;				// 'native' file height
	csSDK_uint32		pixelAspectNum;		// The numerator of the file's pixel aspect ratio
	csSDK_uint32		pixelAspectDen;		// The denominator of the file's pixel aspect ratio
	prFieldType			fieldType;			// New in Pro 3.0. Field dominance. prFieldsNone, prFieldsUpperFirst, prFieldsLowerFirst, prFieldsUnknown
	float				audioSampleRate;	// New in Pro 3.0
	csSDK_uint32		videoSubType;		// New in Pro 3.0
	csSDK_uint32		maxAudioOutputNum;	// New in CS7
	csSDK_uint32		numAudioChannels;	// New in CC 8.0
	PrAudioChannelLabel audioChannelLabels[kMaxAudioChannelCount];	// New in CC 8.0
} pmNewListParms;

// New in Pro 3.0. This is the struct sent with playmod_ListStartEdit
typedef struct
{
	csSDK_uint32		videoSubType;
} pmListStartEditParms;

// This is the struct sent with playmod_GetFilePrefs
typedef struct
{
	PrFourCC			filetype;			// the requested filetype preferences
	PrFourCC			subtype;			// the subtype
	PrFourCC			classID;			// The classID for the module
											// This is used to differentiate between compile modules that
											// support the same fileType and to cross reference between
											// different plugin types (i.e. play and record modules).
	PrMemoryPtr			playmodPrefs;		// Buffer to store preferences
											// If null, a buffer of the desired size must be allocated using the memory suite
											// Otherwise, it's a previously allocated buffer that can be reused.
	prBool				projectOpen;		// if false, the call is being made when no active project is open;
											// if true, the call is being made while the project is open.
	pmPlayTimebase		videoTimebase;		// New in Pro 3.0
} pmGetFilePrefsRec;

// This is the struct sent with playmod_ActivateFile
typedef struct
{
	csSDK_int32			activate;			// If 0, the file should be put in an inactivate state (closed)
	pmFileSpec			spec;				// The filespec.
} pmActivateFileRec;	

// Unicode version of pmActivateFileRec
typedef struct
{
	csSDK_int32			activate;			// If 0, the file should be put in an inactivate state (closed)
	const prUTF16Char*	filepath;			// Full path to the file.
} pmActivateFileRec8;	

/**
**	This is the struct that gets sent with the playmod_PutFrame selector.
**	Playmods that support some kind of secondary output (ie.: external VOut)
**	can use this call to synchronize VOut with Premiere's monitor window.
**	This is used in conjunction with the PMCapPutFrame flag.
*/
typedef struct 
{
	csSDK_int32				size;			// size of the entire struct
	csSDK_int32				version;		// current version (== PLAYMOD_VERSION)
	PPixHand				theFrame;		// the image
	pmPutFrameDestination	destination;	// The location this put frame should be displayed
} pmPutFrameRec;

/**
**	This is the struct that gets sent with the playmod_PutFrameRequest selector.
**	Playmods that support some kind of secondary output (ie.: external VOut)
**	can use this call to specify the render format that Premiere should 
**	use for the playmod_PutFrame operation.
**	This is used in conjunction with the PMCapPutFrame flag.
*/
typedef struct 
{
	// these are filled by Premiere
	csSDK_int32				size;					// size of the entire struct
	csSDK_int32				version;				// current version (== PLAYMOD_VERSION)
	pmPutFrameDestination	destination;			// The location this put frame should be displayed

	// these are returned by the plug-in
	csSDK_int32				width;					// (returned by the plug-in) the prefered frame width (pixels)
	csSDK_int32				height;					// (returned by the plug-in) the prefered frame height (pixels)
	PrPixelFormat			pixelFormats[64];		// array of pixel format prefs 
	csSDK_int32				pixelFormatCount;		// number of pixel formats
	PrRenderQuality			quality;				// render quality flag
} pmPutFrameRequestRec;

/**
**	This is the struct that gets sent along with the playmod_PutTemporaryTimeline selector.
**	The timeline ID is valid for a short period of time, ending on the next playmod_Update call.
*/
typedef struct
{
	csSDK_int32				size;					// size of the entire struct
	csSDK_int32				version;				// current version (== PLAYMOD_VERSION)
	PrTimelineID			timelineData;			// An identifer to be passed back to calls in the timeline suite.
	PrTime					position;				// The time to display, temporarily
} pmPutTemporaryTimelineRec;

typedef struct
{
	PrMemoryPtr				inPrefs;
	unsigned int			inPrefsSize;
	unsigned int			outNumInputChannels;	//	The number of input channels for the player (currently unsupported).
	unsigned int			outNumOutputChannels;	//	The number of output channels for the player.
	int						outWillUsePluginAudio;	//	If 0, then the player will never call InitPluginAudio (and the call will
													//	fail if the player does call it.
	prUTF16Char				outAudioDisplayName[256]; //The name of the current audio hardware to be displayed in the UI.
} pmAudioInfo;

typedef struct
{
	PrMemoryPtr				inPrefs;
	unsigned int			inPrefsSize;
	unsigned int			inChannelIndex;			//	The index of the channel who's info is being requested.
	int						inIsInput;				//	If != 0, then the request is for an input channel. Otherwise, output.
	prUTF16Char				outChannelName[256];	//	A string name for the channel, filled in by the plugin.
} pmAudioChannelInfo;

typedef struct
{
	int						useMaximumRenderPrecision;		// If true, the player should attempt to render at a higher bit depth
	int						mSuppressTransmit;				// If !=0, then the player should only provide desktop display of video
	int						mPrimaryDisplayFullScreen;		// If !=0, then the player should display its primary window in fullscreen
	int 					mDrawTransparencyGrid;			// If !=0, then the player should draw a transparency grid instead of black behind video
	int						mUseEndOfSequenceIndicator;		// If !=0, the player should draw indication when we are on the last frame
	int						mBypassEffects;					// If !=0, the player should skip rendering of all non-intrinsic video effects
	int						mEnableDisplayColorManagement;	// If !=0, the player should enable the display color management
} pmPlayerSettings;

//////////////////////////////////////////////////
// The following struct defines a playable RT region
//    A playable region defines a region that
//    can either be played live or not by the playback
//    module.  It is used during the 
//    playmod_GetPlayableRTRange and playmod_GetPlayableAudioRange function calls.
//  NOTE: If the queried frame falls past the end of
//        the current cut-list, the playback module
//        should return the PRT_END_OF_TIMELINE value
//        in the "outListPos" field.  In this case, 
//        the other fields are considered irrelevant.

/*
** A segment that has the following value in the "end" field is
**  usually a NULL segment that signifies the end of the timeline
**  (no more material to the right of the "inListPos" field of the 
**  segment).  The segment does not have to be kept by the playback 
**  module (used to be called PRT_END_OF_TIMELINE).
*/
#define kPrSDKEndOfTimeline			91445760000000000LL

// this enum is returned by the playmodule to Premiere
//		in order to tell what segments can be played fully
//		in real-time.
typedef enum 
{
	PRT_PLAYCODE_REALTIME = 0,
	PRT_PLAYCODE_NON_REALTIME_UNSPECIFIED = 1,
	PRT_PLAYCODE_REALTIME_WITH_MISMATCH = 2,
	PRT_PLAYCODE_REALTIME_CACHED = 3
} prtPlaycode;

// Version numbers
#define PRT_VERSION_PLAYABLERANGEREC_PREM6		1
#define PRT_VERSION_PLAYABLERANGEREC_PREM7		700
#define PRT_VERSION_PLAYABLERANGEREC_PREM10		1000
#define PRT_VERSION_PLAYABLERANGEREC_CURRENT	PRT_VERSION_PLAYABLERANGEREC_PREM7

typedef struct
{
	// the following members are filled by Premiere before the call
	csSDK_int32		size;			// size of this structure
	csSDK_int32		version;		// the version of the struct
	PrTime			inStartTime;	// frame being tested

	// the following members are filled by the playback module
	PrTime			outEndTime;
	prtPlaycode		playcode;		// see enum above
} prtPlayableRangeRec, *prtPlayableRangePtr;

typedef enum
{
	pmFieldDisplay_ShowFirstField = 0, // ignored if the player is showing progressive content
	pmFieldDisplay_ShowSecondField = 1, // ignored if the player is showing progressive content
	pmFieldDisplay_ShowBothFields = 2,
	
	pmFieldDisplay_ForceSize = kPrForceEnumToIntValue
} pmFieldDisplay;

typedef struct
{
	pmPlayMode inPlayMode;
	PrRenderQuality	inRenderQuality;
	PrRenderQuality inDeinterlaceQuality;
	pmFieldDisplay inFieldDisplay;
	csSDK_int32	inDownsampleFactor;
} pmDisplayStateProperties;

typedef struct
{
	pmPlayMode inPlayMode;
	PrRenderQuality	inRenderQuality;
	PrRenderQuality inDeinterlaceQuality;
	pmFieldDisplay inFieldDisplay;
	csSDK_int32	inDownsampleFactor;
	PrRenderCaptionStreamFormat inCaptionStreamFormatToDisplay;
} pmDisplayStateProperties2;

/*
**
*/
static pmDisplayStateProperties2 ConvertToPmDisplayStateProperties2(pmDisplayStateProperties& inPmDisplayStateProperties)
{
	pmDisplayStateProperties2 newDisplayStateProperties;

	newDisplayStateProperties.inPlayMode = inPmDisplayStateProperties.inPlayMode;
	newDisplayStateProperties.inRenderQuality = inPmDisplayStateProperties.inRenderQuality;
	newDisplayStateProperties.inDeinterlaceQuality = inPmDisplayStateProperties.inDeinterlaceQuality;
	newDisplayStateProperties.inFieldDisplay = inPmDisplayStateProperties.inFieldDisplay;
	newDisplayStateProperties.inDownsampleFactor = inPmDisplayStateProperties.inDownsampleFactor;
	newDisplayStateProperties.inCaptionStreamFormatToDisplay = kPrRenderCaptionStreamFormat_Undefined;

	return newDisplayStateProperties;
}

/**
**	Types to configure VR view support
*/

/**
**	Defines the configuration of a stream of VR video frames.
*/
typedef struct 
{
	PrIVProjectionType				projectionType;				// How the VR image is projected into a 2D frame.
	csSDK_uint32					capturedHorizontalView;		// How many degrees of view are stored in the frame, horizontally.
	csSDK_uint32					capturedVerticalView;		// How many degrees of view are stored in the frame, vertically.
	csSDK_uint32					fieldOfHorizontalView;		// How many degrees of view to display, horizontally, from the entirety of the frame using reverse-projection
	csSDK_uint32					fieldOfVerticalView;		// How many degrees of view to display, vertically, from the entirety of the frame using reverse-projection
	PrIVFrameLayout					stereoscopicType;			// If the VR image contains a stereo pair, and if so, what type of pair.
	PrIVStereoscopicEye				stereoscopicEye;			// Which eye to view when given a VR image with a stereoscopic pair.
	PrIVAmbisonicsMonitoringType	ambisonicsMonitoringType;	// How we are monitoring ambisonics audio
	PrIVAmbisonicsHRIR				ambisonicsHRIR;				// The ID for a head-related impulse response if using PrIVAmbisonicsMonitoring_Binaural
} pmVRConfiguration;

/**
**	Defines the direction the viewer should face within the VR image, taking
**	field of capture and view into account.
**  All angles are measured in degrees, with 0 being the center of the entire
**  VR image.
*/
typedef struct
{
	float			pan;						// Horizontal angle of view
	float			tilt;						// Vertical angle of view
	float			roll;						// Rotation angle of view
} pmVRView;

/**
**	Used to calculate the dimensions of the displayed view taking into
**  account the aspect ratio of the horizontal and vertical field of view.
*/
typedef struct
{
	csSDK_uint32		width;				// In  - Maximum width to display the view into the VR image
											// Out - Actual width to display the view, taking into account Field of View.
	csSDK_uint32		height;				// In  - Maximum height to display the view into the VR image
											// Out - Actual height to display the view, taking into account Field of View.
} pmVRDisplayDimensions;

/****************************************************************
**	Callback Functions
*/
// video functions
typedef void	(*pmShowFileFrameFunc)(PrPlayID playID, csSDK_int32 frametime, pmDisplayPos *disp, prFloatRect *view);
typedef void	(*pmShowFileFrameProxyFunc)(PrPlayID playID, csSDK_int32 frametime, int proxywidth, int proxyheight, int dispwidth, int dispheight);
typedef void	(*pmShowNeedsRenderXFunc)(PrPlayID playID, prWnd theWindow, prRect* theXRect);
typedef void	(*pmShowFileFrameOffscreenFunc)(PrPlayID playID, csSDK_int32 frametime, pmOffscreenDisplayPos *disp);
typedef int		(*pmGetCurrentTime)(PrPlayID playID, double *inCurrentTime);
typedef void	(*pmFrameDropped)(PrPlayID playID, csSDK_int32 inNumberFramesDropped);
typedef void	(*pmShowFileFrameWithSafeAreasFunc)(PrPlayID playID, csSDK_int32 frametime, pmDisplayPos *disp, prFloatRect *view, pmAdornSafeAreasParams* adornSafeAreaParams);
typedef void	(*pmShowFileFrameRenderSettings)(PrPlayID playID, const int inWidth, const int inHeight, const PrRenderQuality inRenderQuality, const prBool inRenderFields);

// file functions
typedef csSDK_int32	(*pmOpenFileFunc)(PrPlayID playID);
typedef void	(*pmReleaseFileFunc)(csSDK_int32 fileref);
typedef void	(*pmSetDebugParameterFunc)(PrPlayID playID, csSDK_int32 selector, csSDK_int32 value);
typedef int		(*pmGetPixelAspectRatioFunc)(PrPlayID playID, csSDK_uint32 *num, csSDK_uint32 *den);

typedef struct _FileFuncs
{
	pmOpenFileFunc				openFile;
	pmReleaseFileFunc			releaseFile;
	pmSetDebugParameterFunc		setDebugParameter;
	pmGetPixelAspectRatioFunc	getPixelAspectRatio;
} FileFuncs, *FileFuncsPtr;

typedef struct _VideoFuncs
{
	pmShowFileFrameFunc					showFileFrame;
	pmShowFileFrameProxyFunc			showFileFrameProxy;
	pmShowNeedsRenderXFunc				showNeedsRenderX;
	pmShowFileFrameOffscreenFunc		showFileFrameOffscreen;
	pmGetCurrentTime					getCurrentTime;
	pmFrameDropped						frameDropped;
	pmShowFileFrameWithSafeAreasFunc	showFileFrameWithSafeAreas;
	pmShowFileFrameRenderSettings		showFileFrameRenderSettings;
} VideoFuncs, *VideoFuncsPtr;

// Utility functions passed during playmod_Startup
typedef struct _pmCallbackFuncs
{
	ClassDataFuncsPtr	classFuncs;
	FileFuncsPtr		fileFuncs;
	VideoFuncsPtr		videoFuncs;
} pmCallbackFuncs;


/****************************************************************
**	Entry Point
*/
typedef PREMPLUGENTRY(* PlayModEntryFunc)(int selector, pmStdParms *parms, void* param1, void* param2);


/****************************************************************
**	Selectors
*/
enum PrPlaymodSelector {
	playmod_Startup = 1,
	playmod_Shutdown = 2,
	playmod_Open = 3,
	playmod_GetInfo = 4,
	playmod_SetDisp = 5,
	playmod_Update = 6,
	playmod_Stop = 10,
	playmod_PlayIdle = 11,
	playmod_Close = 12,
	playmod_GetIndFormat = 14,
	playmod_NewList = 21,
	playmod_GetFilePrefs = 34,
	playmod_UpdateMarkers = 45,
	playmod_SetFilePrefs = 46,				// the param value is a void* NOT the pmSetFilePrefsRec* in the SDK docs.
	playmod_PutFrame = 49,
	playmod_SetQuality = 53,
	playmod_SetPlaybackSpeed = 54,
	playmod_Play = 55,
	playmod_SetPos = 56,
	playmod_GetPos = 57,
	playmod_Step = 59,
	playmod_Preroll = 60,
	playmod_AdornSafeAreas = 61,
	playmod_Activate = 62,
	playmod_EnterScrub = 63,
	playmod_LeaveScrub = 64,	
	playmod_SetView = 65,
	playmod_SetDisplayMode = 66,			// Obsolete (use playmod_SetVideoDisplayType selector instead)
	playmod_PutFrameRequest = 67,
	playmod_SetVideoDisplayType = 69,  
	playmod_DisplayMoving = 70,
	playmod_DisplayChanged = 71,
	playmod_GetAudioInfo = 74,
	playmod_GetAudioChannelInfo = 75,
	playmod_PushPlayerSettings = 76,		// the param value is a pointer to a pmProjectSettings
	playmod_EnableDynamicPlayback = 77, 

	// new to CS4
	playmod_AllowSetPositionDuringPlayback = 78,
	playmod_VideoSequenceHasChanged = 79,
	playmod_GetRTStatusForTime = 80,
	playmod_SetUseFractionalResolution = 81,// CS 4.1 only - deprecated in CS5
	playmod_SetFractionalResolution = 82,	// CS 4.1 only - deprecated in CS5

	// new to CS5
	playmod_PutTemporaryTimeline = 83,
	playmod_SetDisplayStateProperties = 84, //deprecates PlayerQuality and FractionalResolution. Param is a const pmDisplayStateProperties*

	// new to CS7
	playmod_AudioOutputMappingUpdate = 85,

	// new to CC7.1
	playmod_SetDest = 86,
	playmod_SetBackgroundColor = 87,

	// new to CC10.0
	playmod_GetVRSupported = 88,
	playmod_SetVRConfiguration = 89,
	playmod_GetVRConfiguration = 90,
	playmod_SetVREnabled = 91,
	playmod_GetVREnabled = 92,
	playmod_SetVRView = 93,
	playmod_GetVRView = 94,
	playmod_CalculateVRDisplayDimensions = 95,

	// new to CC13.0 (TODO: Do we neede this?)
	playmod_SetDisplayStateProperties2 = 96, //deprecates PlayerQuality and FractionalResolution. Param is a const pmDisplayStateProperties2*

	playmod_SelectorsForceSize = kPrForceEnumToIntValue
};

/* Playback module messages

Message:	playmod_Startup
Purpose:	called at application startup. The play module determines if it has the
			necessary software/drivers to play back a file. If the call returns any error,
			Premiere will not call the module for services.
			If the plugin does not have to be loaded immediately upon application startup,
			then it should return pmIsCacheable instead of playmod_ErrNone
			The player must fill in a GUID into the pmStartupStruct or else the player
			will not be loaded
Params:		pmStartupRec*
Returns		non-zero on error


Message:	playmod_GetIndFormat
Purpose:	Return file support info for file format 'n'
			Fills in the pmModuleInfoRec passed in or returns an error if no more formats
Params:		index
			pmModuleInfoRec	*


Message:	playmod_Open	
Purpose:	Instantiate a file, create the play reference
			(Note: file MUST be opened for sharing; other modules may access it simultaneously)
Returns:	playmod_ErrNone, 
			playmod_ErrBadFile		// can't open it
			playmod_ErrNotPreferred	// good file, but subtype is incorrect (e.g. 'I only play MJPG')
			playmod_ErrDriver		// hardware failure or other unrecoverable error
Params:
	pmPlayRef		*playref
	pmOpenParams	*openparms

Message: playmod_SetDisp	
Purpose: Change the display area
Params:
	pmPlayRef		playref;
	pmDisplayPos	*disp;

Message: playmod_Update
Purpose: playback area was obscured, redraw current frame
Params:
	pmPlayRef		playref;

Message: playmod_Stop
Purpose: Stops playback
Params:
	prPlayRef		playref;

Message: playmod_PlayIdle
Purpose: sent repeatedly during playback so play module can service playback
Params:
	prPlayRef		playref;

Message: playmod_Close
Purpose: Close the playback module, release files, shut down, fini.
Params:
	prPlayRef		playref;

Message: playmod_NewList
Purpose: Called to create a new cut list
Params: (pmNewListParms structure, or pmNewListParmsV2 if audio playlist) 
	pmPlayTimebase	listTimebase;		// timebase for playlist
	pmDisplayPos	disp;				// the display area
	csSDK_int32			playID;				// ID of this play instance. Used for callback functions


Message:	playmod_GetPlayableRTRange
Purpose:	Sent to get information about a region of the current cut-list
         regarding if it can be played in real-time or not.
Params   (prtPlayableRange structure)
// the following members are filled by Premiere before the call
   csSDK_int32 size;              // size of this structure
   csSDK_int32 version;           // the version of the struct (set to 1)
   csSDK_int32 frame;             // frame being tested
   prtTimebase timebase;   // timebase of "frame"

// the following members are filled by the playback module
   csSDK_int32 inListPos;         // in point in the play list
   csSDK_int32 outListPos;        // out point in the plat list (inclusive)
   csSDK_int32 playcode;          // see enum above
   csSDK_int32 layerIndex;        // (optional) index of affected layer


Message:	playmod_GetFilePrefs
Purpose:	Sent to make the playback display itw own dialog box for settings filetype
         preferences.
Params   (pmGetFilePrefsRec structure)
	csSDK_int32    filetype;        // the requested filetype preferences
	csSDK_int32	classID;		 // The classID for the module
							 // This is used to differentiate between compile modules that
							 // support the same fileType and to cross reference between
							 // different plugin types (i.e. play and record modules).
	Ptr		playmodPrefs;	 // Buffer to store preferences
							 // If null, a buffer of the desired size must be allocated using the memory suite
							 // Otherwise, it's a previously allocated buffer that can be reused.
                             // (Same concept as in compile module with "compilePrefs"
Return values:
   playmod_ErrNone:         No action
   playmod_RebuildCutlist:  Invalidate the current cut-list (playmod will receive update). This will also call playmod_SetFilePrefs 
								on all open play modules using the returned prefs.
   playmod_BroadcastPrefs:	Tells premiere to call playmod_SetFilePrefs on all open play modules using the returned prefs.


Message:		playmod_ActivateFile
Purpose:		If "activate" is 0, the playmodule should close the file (so Premiere can write to it).
				If "activate" is 1, the file should be reopened
Params:			pmActivateFileRec
Return values:	none

Message:		playmod_ActivateFile8
Purpose:		If "activate" is 0, the playmodule should close the file (so Premiere can write to it).
				If "activate" is 1, the file should be reopened
Params:			pmActivateFileRec8
Return values:	none

Message:		playmod_UpdateMarkers
Purpose:		Tell the playmod to update its use of markers

Params:			none, use the callbacks in the PISuites to get at the markers
Return Values:	none

Message:		playmod_SetFilePrefs
Purpose:		Notify all instances of playmods that their prefs have changed
Params:			playmod Prefs

Message:		playmod_ProjectSettingsChanged
Purpose:		Notify all instances of playmods that the project settings have changed
Params:			none

Message:		playmod_PutFrame
Purpose:		Sent to the playmod to let it draw a rendered frame to its output (VOut).
				NOTE: This selector is called only if the PMCapPutFrame flag is set for the playmod.
Params:			A pmPutFrameRec struct, containing a PPixHand and the put frame destination. 
				If a playmod_PutFrameRequest is sent first, the size of the frame that is sent will
				match the put frame requested size. If this selector is not sent, any size image
				can be sent to putframe.
Return values: 
				playmod_ErrNone: success


Message:		playmod_PutFrameRequest
Purpose:		Sent to the playmod to query for the frame size and pixel format it would like to receive
				in a subsequent call to playmod_PutFrame. This is not always sent before a playmod_PutFrame selector.
				NOTE: This selector is called only if the PMCapPutFrame flag is set for the playmod
Params:			A pmPutFrameRequestRec struct, containing information that is to be filled by the playmod.
Return values:  playmod_ErrNone: the rectangle values were succesfully returned by the playmod.
				Any other error code: The request is denied by the plug-in.  In this case, 
									  Premiere will not call the playmod_PutFrame selector for this
									  particular frame, but rather do the update only in the monitor window.  
									  NOTE: returning an error code in this case does NOT prevent the 
											playmod from receiving subsequent calls to the 
											PutFrameRequest / PutFrame selectors in the next user action.

Message:		playmod_PutTemporaryTimline
Purpose:		Sent to the playmod to let it draw a timeline to its output (VOut).
				NOTE: This selector is called only if the PMCapCanPutTemporaryTimeline flag is set for the playmod.
Params:			A pmPutTemporaryTimelineRec struct
Return values: 
				playmod_ErrNone: success
				playmod_RenderAndPutFrame: the plugin cannot render the put segment. The Premiere will call PutFrameRequest,
						render the requested segment, and call PutFrame to display it.

Message:	playmod_SetQuality:
Purpose:	change quality during playback. Only called while file is not playing back,
			this state needs to be stored and used on a per playmodule instance.
Params:
			PrPlaybackQuality	playback quality 


Message:	playmod_SetPlaybackSpeed:
Purpose:	Request that the playmod adopt the requested playback speed. The speed value is a
			float, with positive meaning forward play and negative meaning backward play. Zero
			is an illegal speed an will never be passed down.
			The playmod can refuse to play at a requested speed, in which case the playmod
			should change the float value to the speed it is going to play at and return the
			playmod_UnsupportedPlaybackSpeed error code
Params:
			prPlayRef		playref;
			float*			inoutPlaybackSpeed.  // On input, the requested speed. On output, the actual speed. 


Message:	playmod_Play
Purpose:	Starts playback
			Position to start at, in/out point, loop flag, and speed are passed in
Params:
			prPlayRef		playref;
			pmPlayParms		playparms

Message:	playmod_SetPos
Purpose:	tells module to change its logical position to pos. If playing, playback should
			reset to the new position. If not playing, then nothing should happen.
Params:
			prPlayRef		playref;
			PrTime*			pos;			

Message:	playmod_GetPos
Purpose:	asks the module what its current position and mode is
			Module fills in pos.position and current mode (playing, stopped)
Params:
			prPlayRef		playref;
			pmPosRec7		*pos;

Message:	playmod_Step
Purpose:	Move the current position by the requested time. If playing, playback should stop
			after the step is completed. If not playing, the frame at the new position should
			be displayed. If the playAudio parameter is non-zero, then audio associated with
			the destination frame should be played.
Params:
			prPlayRef		playref;
			pmStepRec7		step settings.

Message:	playmod_Preroll
Purpose:	Readies movie for playback
			Position to start at is passed in
Params:
			prPlayRef		playref;
			pmPlayParms		playparms

Message:	playmod_AdornSafeAreas
Purpose:	Overlays safe title and safe action graphics on top of play module display
Params:
			prPlayRef			playref;
			pmAdornSafeAreas	safe area params

Message:	playmod_Activate
Purpose:	Activate/deactivate playback module.
			On deactivate, release hardware resources, sound channels, memory so other modules
				can play. A module can still recieve playmod_Update, playmod_SetPos, etc. messages,
				but will not recieve 'play' messages unless active.
			The activation/deactivation reason is sent in to the player. This allows the player to 
				know why it is being deactivated. 

			On activate, start all driver/play hardware, do not update current frame until
			playmod_Update message is sent.
Params:
			prPlayRef			playref;
			pmActivateRec*		activationRec

Message:	playmod_EnterScrub
Purpose:	The user is dragging the time control. The playmod should initialize the audio
			system and keep it inialized until LeaveScrub is called. On SetPosition and Step7 calls,
			it should move to the new position and play a blip of audio.
Params:
			prPlayRef		playref;
			prBool			If true, then any audio from Premiere will be silent and there is no need to
							intialize the audio system.

Message:	playmod_LeaveScrub
Purpose:	The user has stopped dragging the time control. The playmod should uninitialize the audio
			system.

Message:	playmod_SetView
Purpose:	Change the viewing rectangle within the player to be displayed in the display area
Params:
			pmPlayRef		playref;
			pmFloatRec		*view;

Message:	playmod_SetDest
Purpose:	Change the destination rectangle within which the player will be displayed
Params:
			pmPlayRef		playref;
			pmFloatRec		*dest;

Message:	playmod_SetBackgroundColor
Purpose:	Change the color that is used to fill the background when the rectangle set by playmod_SetDest is something other than unity.
Params:
			pmPlayRef		playref;
			prColor			*backgroundColor;

Message:	playmod_SetDisplayMode	
Purpose:	Set the current display mode of the player.
Params:
			pmPlayRef		playref;
			pmDisplayMode	newMode;

Message:	playmod_GetAudioInfo	
Purpose:	Gets information about the audio capabilities of the player. If the player wants to call
			InitPluginAudio in version 2 or higher of the PlayModuleAudioSuite, it must support this
			selector. Note that this call is static - a play module instance is not provided.
Params:
			pmAudioInfo		audioInfo;
			param2 unused

Message:	playmod_GetAudioChannelInfo	
Purpose:	Gets information about a specific audio channel of the player. If the player wants to call
			InitPluginAudio in version 2 or higher of the PlayModuleAudioSuite, it must support this
			selector. Note that this call is static - a play module instance is not provided.
Params:
			pmAudioChannelInfo	audioChannelInfo;
			param2 unused

Message:	playmod_EnableDynamicPlayback
Purpose:	If this is enabled, a player should expect that effect parameters could change during playback, 
			so the rendering of the effects should occur at the last possible moment. 
Params:		
			prBool			enabled;
			param2			unused;

Message:	playmod_GetVRSupported
Purpose:	Determine if the player supports viewing VR content.  The boolean pointer will be filled with
			kPrTrue if VR is supported, otherwise with kPrFalse if unsupported.
Params:
			pmPlayRef		playref;
			prBool*			outSupported;

Message:	playmod_SetVRConfiguration
Purpose:	Configures the player to display a stream of VR video.
Params:
			pmPlayRef			playref;
			pmVRConfiguration	*configuration;

Message:	playmod_GetVRConfiguration
Purpose:	Fills in a pmVRConfiguration structure with the current VR viewing configuration.
Params:
			pmPlayRef			playref;
			pmVRConfiguration	*configuration;

Message:	playmod_SetVREnabled
Purpose:	Enables (kPrTrue) or disables (kPrFalse) VR viewing on the player.
Params:
			pmPlayRef			playref;
			prBool				enabled;

Message:	playmod_GetVREnabled
Purpose:	Fills in a boolean pointer with pKRTrue if VR viewing is currently enabled, otherwise
			it is filled with pKRFalse.
Params:
			pmPlayRef			playref;
			prBool				*outEnabled;

Message:	playmod_SetVRView
Purpose:	Configures the player to look at a particular region within a VR video stream.
Params:
			pmPlayRef			playref;
			pmVRView			*view;

Message:	playmod_GetVRView
Purpose:	Fills in a pmVRView structure with the current VR viewing angles.
Params:
			pmPlayRef			playref;
			pmVRView			*outView;

Message:	playmod_CalculateVRDisplayDimensions
Purpose:	Determines the appropriate display dimensions for a video stream taking into
			account the current pmVRVew configuration's aspect ratio.
Params:
			pmPlayRef				playref;
			pmVRDisplayDimensions	*displayDimensions;
*/

#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif /* PRSDKPLAYMODULE_H */
