/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2002-2020 Adobe Systems Incorporated                  */
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


#ifndef PRSDKIMPORT_H
#define PRSDKIMPORT_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKSTRUCTS_H
#include "PrSDKStructs.h"
#endif

#ifndef PRSDKENTRY_H
#include "PrSDKEntry.h"
#endif

#ifndef PRSDKALPHATYPES_H
#include "PrSDKAlphaTypes.h"
#endif

#ifndef PRSDKCLASSDATA_H
#include "PrSDKClassData.h"
#endif

#ifndef PRSDKPLUGSUITES_H
#include "PrSDKPlugSuites.h"
#endif

#ifndef	PRSDKAUDIOSUITE_H
#include "PrSDKAudioSuite.h"
#endif

#ifndef	PRSDKROLLCRAWLSUITE_H
#include "PrSDKRollCrawlSuite.h"
#endif

#ifndef PRSDKASYNCIMPORTER_H
#include "PrSDKAsyncImporter.h"
#endif

#ifndef PRSDKDATASTREAMTYPES_H
#include "PrSDKDataStreamTypes.h"
#endif

#ifndef PRSDKIMPORTERSHARED_H
#include "PrSDKImporterShared.h"
#endif

#ifndef PRSDKSTRINGSUITE_H
#include "PrSDKStringSuite.h"
#endif

#ifndef	PRSDKIMMERSIVEVIDEOTYPES_H
#include "PrSDKImmersiveVideoTypes.h"
#endif

#ifndef PRSDKCOLORPROFILE_H
#include "PrSDKColorProfile.h"
#endif

#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif

#define IMPORTMOD_VERSION_1		1	// 5.0
#define IMPORTMOD_VERSION_2		2	// 5.0.1
#define IMPORTMOD_VERSION_3		3	// 6.0
#define IMPORTMOD_VERSION_4		4	// 7.0/Premiere Pro 1.0
#define IMPORTMOD_VERSION_5		5	// Premiere Pro 1.5
#define IMPORTMOD_VERSION_6		6	// Premiere Elements 1.0
#define IMPORTMOD_VERSION_7		7	// Premiere Pro 2.0
#define IMPORTMOD_VERSION_8		8	// CS3
#define IMPORTMOD_VERSION_9		9	// CS4
#define IMPORTMOD_VERSION_10	10	// CS5
#define IMPORTMOD_VERSION_11	11	// CS5.5
#define IMPORTMOD_VERSION_12	12	// CS6
#define IMPORTMOD_VERSION_13	13	// CS6.0x
#define IMPORTMOD_VERSION_14	14	// CS7
#define IMPORTMOD_VERSION_15	15	// 8.0
#define IMPORTMOD_VERSION_16	16	// 8.0.1
#define IMPORTMOD_VERSION_17	17	// 9.2.1
#define IMPORTMOD_VERSION_18	18	// 11.0
#define IMPORTMOD_VERSION_19	19	// 12.0.1
#define IMPORTMOD_VERSION_20	20	// 12.1
#define IMPORTMOD_VERSION_21	21	// 13.0
#define IMPORTMOD_VERSION_22	22	// 14.0.2
#define IMPORTMOD_VERSION_23	23	// 14.1
#define IMPORTMOD_VERSION		IMPORTMOD_VERSION_23

// Platform file reference
#ifdef PRWIN_ENV
#define imFileRef HANDLE
#define imInvalidHandleValue	(HANDLE)-1
#else
#define imFileRef void*
#define imInvalidHandleValue	(void*)-1
#endif


// progress callback for trimming/saving files
typedef csSDK_int32 (*importProgressFunc)(csSDK_int32 partDone, csSDK_int32 totalToDo, void* trimCallbackID);


// Utility functions passed in with the Standard Parms

typedef struct
{
	ClassDataFuncsPtr	classFuncs;
	csSDK_int32			unused1;			//	CS4: Removed the OpenFileFunc (which didn't appear to work anyway in CS3)
	csSDK_int32			unused2;			//	CS4: Removed the DeleteFileFunc (which didn't appear to work anyway in CS3)
} imCallbackFuncs;

// Standard parms passed into xImportEntry

typedef struct
{
	csSDK_int32		imInterfaceVer;		// version # of import interface (IMPORTMOD_VERSION)
	imCallbackFuncs	*funcs;
	piSuitesPtr		piSuites;
} imStdParms;

//--------------------------------------
// Flags for the xRef resource

#define xfCanOpen               0x8000
#define xfCanImport             0x4000
#define xfCanReplace			0x2000
#define xfUsesFiles             0x1000

#define xfCantBatchProcess		0x0800	// Used by importers to indicate the format isn't batch processable
#define xfCanWriteMetaData		0x0400	// Used by importers to save off the canWriteMetaData flag
#define xfCanWriteTimecode		0x0200	// Used by importers to save off the canWriteTimecode flag
#define xfIsTitle               0x0100


#define xfIsEffect              0x0080
#define xfIsAFilter             0x0040
#define xfIsVFilter             0x0020
#define xfIsStill               0x0010
#define xfIsMovie               0x0008
#define xfIsSound               0x0004
#define xfIsAnimation			0x0002
#define xfIsBackdrop			0x0001

#define xfTypes                 0x03FF


// struct filled in during imImportInit
typedef struct
{
	csSDK_uint32			importerType;		// identifier for the module
	csSDK_int32				unused1;			// CS4: Removed canOpen. All importers must now open their own files.
	csSDK_int32				canSave;			// importer handles "save as"
	csSDK_int32				canDelete;			// importer handles deleting its own files (probably because there are child files)
	csSDK_int32				canResize;			// importer can do resizing
	csSDK_int32				canDoSubsize;		// importer can rasterize a subset of an image at any size
	csSDK_int32				canDoContinuousTime;// importer can render frames at arbitrary times (interpolate or
												// generate synthetically). If set, the importer may be called
												// to generate fields.
	csSDK_int32				noFile;				// no file reference, the importer generates the image
	csSDK_int32				addToMenu;			// add this importer to a premiere menu (see enum)
	csSDK_int32				hasSetup;			// importer has a setup dialog
	csSDK_int32				dontCache;			// Frames should not be cached
	csSDK_int32				setupOnDblClk;		// If true, the setup dialog will be opened on a double click

	csSDK_int32				keepLoaded;			// If true, the importer plugin will never be unloaded.
												// Normally, importers are loaded and unloaded on an as needed basis
												// to minimize system load. With this flag set, an importer will never
												// be released until the app shuts down. Don't set this flag unless it's
												// really necessary.
	
	csSDK_int32				priority;			// New for 5.1RT LL981112
												// Used to determine priority levels for importers that handle the
												// same filetype.
												// Priority now accepts any positive value. Higher numbers will override
												// lower numbers. For overriding Premiere shipping plugins, use number
												// of 100 or greater.
												// NOTE: importers with priority>0 MUST also set canOpen.
	
	csSDK_int32             canAsync;           // 6.0 threading   [DEPRECATED]
	csSDK_int32				canCreate;			// 6.0 Set by 'custom' importers (synth importers which create files on disk)
	csSDK_int32				canCalcSizes;		// Pro 1.x Set by importers that want to calc trimmed and untrimmed file sizes (probably because there are child files)
	csSDK_int32				canTrim;			// Pro 1.x Set by importers that can trim media files
	csSDK_int32				avoidAudioConform;	// Elements 1.0, Pro 2.0 - Set by importers if they can support fast audio retrieval. 
												// If the file's audio rate is equal to or less than the current project audio rate no conforming will be attempted
												// unless there is compression in the audio data.
	prUTF16Char*			acceleratorFileExt;	// The file extension of accelerator files that this module creates and supports.
												// This pointer points to an array of size 256. It should only be filled in if the module
												// supports
	csSDK_int32				canCopy;			// Pro 2.0 Set by importers that can copy media files
	csSDK_int32				canSupplyMetadataClipName;	// Pro 3.0.2 Allows file based importer to set clip name from metadata (P2 supports this)
    csSDK_int32				canValidatePrefs;	// Pro 4.1. Used for RED color settings. In the future this needs to be paired with an imValidatePrefs call.
	csSDK_int32				canProvidePeakAudio;// Pro 5 If importer can supply peak audio data
	csSDK_int32				canProvideFileList;	// Pro 6.0 Importer can provide a list of all files for a copy operation
	csSDK_int32				canProvideClosedCaptions;	// Pro 7.0 Importer can provide closed captions
	prPluginID				fileInfoVersion;	// Pro 8.0 Importer file info version
	csSDK_int32				hasSourceSettingsEffect;	// Works with a master clip source settings effect
	csSDK_int32				hasPersistentData;	// PPro 9.2 Allows for getPrefs, but without any associated source settings dialog (similar to hasSetup).
	csSDK_int32				reserved[10];		// reserved; set to null by the host.
} imImportInfoRec;

typedef struct
{
	prDateStamp		creationDateStamp;
	csSDK_int32		reserved[40];				// reserved; set to null by the host.
} imFileAttributesRec;

// color record for alphaArbitrary matte color
typedef struct
{
	unsigned char	matteRed;
	unsigned char	matteBlue;
	unsigned char	matteGreen;
} matteColRec;


typedef PrSDKAlphaType ImporterAlphaType;

// values for noDuration field in imImageInfoRec
enum ImporterDurationType
{
	imNoDurationFalse  = 0,		// not a "No Duration" file
	imNoDurationNoDefault,		// Use supplied vid duration - don't use default
	imNoDurationStillDefault	// Use the "still" default duration
};


enum
{
	imMenuNone = 0,
	imMenuNew
};

#define imUncompressed		'RAW '				// Special subType for uncompressed files
#define imSubTypeUnknown	0xffffffff			// Special subType for files which can't report a subtype.

enum ImporterPulldownCadence
{
	importer_PulldownPhase_NO_PULLDOWN = 0,
	// 2:3 cadences
	importer_PulldownPhase_WSSWW = 1,
	importer_PulldownPhase_SSWWW,
	importer_PulldownPhase_SWWWS,
	importer_PulldownPhase_WWWSS,
	importer_PulldownPhase_WWSSW,

	// 24pa cadences
	importer_PulldownPhase_WWWSW,
	importer_PulldownPhase_WWSWW,
	importer_PulldownPhase_WSWWW,
	importer_PulldownPhase_SWWWW,
	importer_PulldownPhase_WWWWS
};

enum
{
	imColorProfileSupport_None = 0,
	imColorProfileSupport_Fixed // The importer has a list of possible profiles which can be selected, possibly of length 1. If the Importer is guessing, it should set the imEmbeddedColorProfileUncertain flag
};

// similar to imColorProfileSupport but designed to handle extensible set
// of media color spaces, not just ICC profiles or LUTs; added in Pr 13.0
enum
{
	imColorSpaceSupport_None = 0,
	imColorSpaceSupport_Fixed
};

#define	imPixelAspectRatioUncertain			0x1
#define	imFieldTypeUncertain				0x2
#define imAlphaInfoUncertain				0x4
#define imEmbeddedColorProfileUncertain		0x8
	
#define imColorSpaceUncertain				imEmbeddedColorProfileUncertain

typedef struct
{
// Image bounds info:
	csSDK_int32			imageWidth;		// frame width in pixels
	csSDK_int32			imageHeight;	// frame height in pixels
	csSDK_uint16		pixelAspectV1;	// aspect of pixels x:y, e.g. D1 == 648:720
										// intended to be 8.8 fixed, superseded by pixelAspectNum/Den below
	csSDK_int16			depth;			// bits per pixel - image buffers are 32bpp so this is informational only

	csSDK_int32			subType;		// The subType for the file.
										// This is generally the fourcc of the codec for the file
										// though not always. It's used to match files to fileType's and subTypes
										// that compile modules support.
										// For uncompressed files, this should be set to imUncompressed.
// Image format info:
	char			fieldType;			// field type -- see enum, note 'any' is not supported for importers
	char			fieldsStacked;		// fields present, not interlaced
	char			reserved_1;			// reserved; set to null by the host.
	char			reserved_2;			// reserved; set to null by the host.
	char			alphaType;			// alpha channel type -- see enum
	matteColRec		matteColor;			// color of alpha matte for alphaArbitrary type
	char			alphaInverted;		// alpha is inverted from normal
	char			isVectors;			// content has no inherent resolution

// Image draw info:
	char			drawsExternal;		// file draws only externally (NTSC/PAL out)
	char			canForceInternalDraw;	// file draws only externally BUT we'll respect
	char			dontObscure;		// file plays external and internal, don't obscure onscreen drawing (T2K)

// Image time info:
	char			isStill;			// File contains a single frame. Only one frame will be cached.
	char			noDuration;			// File has no intrinsic duration so it can be stretched. Usually used for synthetic images
										// please see enum above for values in this field.

	char			reserved_3;			// reserved; set to null by the host.

// pixel aspect ratio (new in 6.0)
	csSDK_uint32	pixelAspectNum;
	csSDK_uint32	pixelAspectDen;

	char			isRollCrawl;		// this is a title or other image that does roll/crawl and supports the
										// imGetRollCrawlInfo, imGetRollCrawlPage selectors
	char			reservedc[3];		// reserved; set to null by the host.
	csSDK_int32		importerID;			// [in] the ID for this importer instance, used in suite callbacks
	csSDK_int32		supportsAsyncIO;	// supports the Pro 2.0 imCreateAsyncImporter selector for this file
	csSDK_int32		supportsGetSourceVideo;// supports the Pro 2.0 imGetSourceVideo selector for this file
	csSDK_int32		hasPulldown;		// file has NTSC film 3:2 pulldown
	csSDK_int32		pulldownCadence;	// See enum, above
	csSDK_int32		posterFrame;		// poster frame number (not time!!)
	csSDK_int32		canTransform;		// Pro 3.0 Set by importers that can apply a transform matrix
	csSDK_int32		interpretationUncertain;	// Pro 3.0 Importers can specify if they are uncertain about their interpretation, see flags above.  0 means no certain.
	csSDK_int32		colorProfileSupport;// Added in CS5.5. Currently only meaningful in After Effects. See the imColorProfileSupport enum - obsolete as of Pr 13.0
	PrSDKString		codecDescription;	// Codec description string.
	csSDK_int32		colorSpaceSupport;	// see imColorSpaceSupport enum. To be used by importers capable of passing media color space definition to the host. Use it instead of colorProfileSupport.
	PrTime			frameRate;			// If non-zero, this value supercedes vidScale/vidSampleSize as the video timebase.  The frame period is in ticks relative to PrSDKTimeSuite::GetTicksPerSecond.
	prBool			hasEmbeddedLUT;		// To be used to specify if media contains embedded LUT.
	csSDK_int32		bitDepth;			// If non-zero, this data indicates number of significant bits per color component of the underlying media.
 	csSDK_int32		reserved[11];		// reserved; set to null by the host.
} imImageInfoRec;

// struct filled in at imFileInfo time for audio
typedef struct
{
	csSDK_int32				numChannels;
	float					sampleRate;		// In Hz.
	PrAudioSampleType		sampleType;		// This is for informational use only! All samples will be return as buffers of Float32's
} imAudioInfoRec7;

// timebase/format struct passed to/from importer
typedef struct
{
	csSDK_int32	timebase;			// timebase
	csSDK_int32	samplesize;			// size of one sample
	csSDK_int32	duration;			// duration in samples
} imTimebaseRec;

// Populated during imGetInfo
// contains attributes for the file being imported.

//	Access modes
const csSDK_int32 kRandomAccessImport				= 0; //This file can be read by random access (default)
const csSDK_int32 kSequentialAudioOnly				= 1; //When accessing audio, only sequential reads allowed (for
														 //variable bit rate files).
const csSDK_int32 kSequentialVideoOnly				= 2; //When accessing video, only sequential reads allowed.
const csSDK_int32 kSequentialOnly					= kSequentialAudioOnly + kSequentialVideoOnly;
const csSDK_int32 kSeparateSequentialAudio			= 4; //New for 7.5. Plugin supports the new SequentialAudio selectors

// Unicode version of imFileInfoRec
typedef struct
{
	char				hasVideo;			// The file contains video
	char				hasAudio;			// The file contains audio

	imImageInfoRec		vidInfo;			// Information about the video data
	csSDK_int32			vidScale;			// vidScale/vidSampleSize is the preferred video timebase
	csSDK_int32			vidSampleSize;		// vidScale/vidSampleSize is the preferred video timebase
	csSDK_int32			vidDuration;		// The duration of the video in the video timebase

	imAudioInfoRec7		audInfo;			// Information about the audio data
	PrAudioSample		audDuration;		// The duration of the audio sample frames

	csSDK_int32			accessModes;		// This should be one of the access modes listed above
	
	void*				privatedata;		// this is private instance data
	void*				prefs;				// this is persistent settings data
	char				hasDataRate;		// importer can generate datarate information for this file.

	csSDK_int32			streamIdx;			// IN: stream index number
	char				streamsAsComp;		// OUT: if multiple streams and this is stream zero, indicate
											// whether to import as a composition or multiple clips
	prUTF16Char			streamName[256];	// OUT: optional name of this stream if there are multiple streams
	csSDK_int32			sessionPluginID;	// ID for specific plugin instance, valid for this call only; new for 7.5/1.5
											// almost guaranteed NOT to be the same value as was passed into GetPrefs,
											// but it DOES identify the same object.
	char				alwaysUnquiet;		// If != 0, then Premiere will always unquiet this file immediately when the
											// application resumes.
	char				hasDataStreams;		// OUT: does the file instance have datastreams
	prUTF16Char			filePath[2048];		// Full absolute path to the media file for this stream
	char				canProvidePeakData;	// This is initialized to the value set in imImportInfoRec, but can be changed on a per file basis
	char				mayBeGrowing;		// This may be a partial file that will get longer. Host will check back at intervals by reloading the file
	char				ignoreGrowing;		// The host knows the file isn't growing
	prUTF16Char			sourceSettingsMatchName[256];	// OUT: If file supports the new source settings api, then set the match name of the effect to use.
	csSDK_int64			vidDurationInFrames;	// vidDuration will be ignored if this is set.

	PrIVProjectionType	ivProjectionType;			// File's immersive video projection type
	PrIVFrameLayout		ivFrameLayout;				// File's immersive video mono/stereoscopic frame layout
	csSDK_uint32		ivHorizontalCapturedView;	// Degrees of horizontal immersive view captured in the file's video, 0 - 360
	csSDK_uint32		ivVerticalCapturedView;		// Degrees of vertical immersive view captured in the file's video, 0 - 180
	
	char				mayHaveCaptions;			// The file may have caption data to scan

	char				reserved[2];
	// No new fields will be defined here.  imFileInfoRec9 will be used to extend the GetInfo interface.
} imFileInfoRec8;

// Newer, more easily extensible version of imFileInfoRec8.  See imGetInfo9.
typedef struct
{
	imFileInfoRec8		info;						// Info fields defined in the past.

	csSDK_uint32		systemStateFlagMask;		// Set bits to indicate which system state items affect this file.  See imGetCurrentSystemState.
	csSDK_uint32		systemStateFlagsCurrent;	// Represents the current system state.  See imGetCurrentSystemState.
	csSDK_uint32		systemStateSubtypeVersion;	// 0 or a cache invalidation version number for info.vidInfo.subType.  See imGetCurrentSystemState.

	csSDK_uint32		reserved[445];				// Reserved; set to null by the host.
} imFileInfoRec9;

// Unicode version of imFileAccessRec
typedef struct
{
	void*				importID;
	csSDK_int32			filetype;
	const prUTF16Char*	filepath;
	imFileRef			fileref;
	PrMemoryPtr			newfilename;	//	prUTF16Char buffer (imGetPrefs8 only)
} imFileAccessRec8;

// This the struct passed to an importer during imOpenFile, imQuiet
typedef enum 
{
	kPrOpenFileAccess_ReadOnly = 0,
	kPrOpenFileAccess_ReadWrite = 1,
	kPrOpenFileAccess_ForceEnumSize = kPrForceEnumToIntValue
} PrFileOpenAccess;

// Unicode version of imFileOpenRec
typedef struct
{
	imFileAccessRec8	fileinfo;
	void*				privatedata;
	csSDK_int32			delegatetype;			// internal use; do not use.
	PrFileOpenAccess	inReadWrite;			// New for Pro 1.0 - (only used for imOpenFile) uses the PrFileOpenAccess type to request read or read write access
	csSDK_int32			inImporterID;			// New for Pro 2.0 - the importer ID for use with the caching suite.
	csSDK_size_t		outExtraMemoryUsage;	// New for CS5. If the importer uses memory which cannot otherwise be registered in the cache just by being
												// open, put the size in bytes in this field.
	csSDK_int32			inStreamIdx;			// New for CS6. Which stream is this?
} imFileOpenRec8;

// flags passed during imImportImage
enum
{
	imDraftMode=8,				// Draw quickly if possible. This is passed when scrubbing and playing
								// from the timeline.
	imSamplesAreFields=16		// new for 7.0 - most importers will ignore as Premiere already
								// scales in/out/scale to account for fields, but if you need to know
								// that this has occurred (because maybe you measure something in 'frames'), check this flag.
								// Also, may we suggest considering measuring in seconds instead of frames?...
};

// struct passed during imImportImage message
typedef struct
{
	csSDK_int32			unused1;		// this field is not used
	csSDK_int32			dstWidth;		// destination width
	csSDK_int32			dstHeight;		// destination height
	csSDK_int32			dstOriginX;		// destination origin X (always 0 for offscreen)
	csSDK_int32			dstOriginY;		// destination origin Y (always 0 for offscreen)
	csSDK_int32			srcWidth;		// source width if subsampling
	csSDK_int32			srcHeight;		// source height if subsampling
	csSDK_int32			srcOriginX;		// source origin X if subsampling
	csSDK_int32			srcOriginY;		// source origin Y if subsampling
	csSDK_int32			unused2;		// this field is not used
	csSDK_int32			unused3;		// this field is not used
	csSDK_int32			rowbytes;		// rowbytes;
	char*				pix;			// destination bits
	csSDK_int32			pixsize;		// number of bytes in "pix"
	PrPixelFormat		pixformat;		// format of the pixels as fourcc, 0 = BGRA
	csSDK_int32			flags;			// special flags, see enum above
	prFieldType			fieldType;		// unused
	csSDK_int32			scale;			// scale/sampleSize is the timebase
	csSDK_int32			sampleSize;		// scale/sampleSize is the timebase

										// The following are based on the timebase given by scale and sampleSize
	csSDK_int32			in;				// in point 
	csSDK_int32			out;			// out point
	csSDK_int32			pos;			// importTime position

	void*				privatedata;	// instance data from imGetInfo
	void*				prefs;			// persistent data from imGetSettings
	prRect				alphaBounds;	// new for 7.5. This is the rect outside of which the alpha is always 0.
										// Simply do not alter this field if the alpha bounds match the dest bounds.
										// If set, the alpha bounds must be contained by the dest bounds.

	csSDK_int32			applyTransform;	// Pro 3.0 - the following matrix and rect are valid, use them.
	float				transform[3][3];// Pro 3.0 - source to dest matrix.
	prRect				destClipRect;	// Pro 3.0 - destination rect inside the bounds of the pix buffer.
} imImportImageRec;

// Extended struct passed to import modules with extra
// private data. The data is at the end of the public struct.
typedef struct
{
	imImportImageRec	iir;
	csSDK_int32			extra[16];
	PPixHand			thePix;
	PWorldID			thePort;
} imImportImageRecEx;

// Added to 6.0 to support multi-threading
// Deprecated in Pro 2.0
typedef struct
{
	void*	privatedata;			// instance data from imGetInfo
    csSDK_int32		startframe;     // frame to start from
    csSDK_int32		endframe;       // last frame to pre-buffer (inclusive)
    csSDK_int32		width;          // width we want it at
    csSDK_int32		height;         // height ditto
} imAsyncImportImageRec;

// struct passed during imImportAudio message
// the new audio import struct for version 7
typedef struct
{
	PrAudioSample	position;				// in point in sample frames. If it is < 0, then the request is sequential and
											// the importer should return contiguous samples from the last call.
	csSDK_uint32	size;					// the number of sample frames to import (1 sample frame = 1 sample on all channels)
	float**			buffer;					// an array of numChannels buffers of floats of size size
	void*			privateData;			// instance data from imGetInfo
	void*			prefs;					// persistent data from imGetSettings
} imImportAudioRec7;

// struct passed during imGetTimeInfo, filled in by plug-in
typedef struct
{
	void*			privatedata;			// instance data from imGetInfo
	void*			prefs;					// persistent data from imGetSettings
	char			orgtime[18];
	csSDK_int32		orgScale;
	csSDK_int32		orgSampleSize;
	char			alttime[18];
	csSDK_int32		altScale;
	csSDK_int32		altSampleSize;
	char			orgreel[40];
	char			altreel[40];
	char			logcomment[256];		// the comment from the capture
	csSDK_int32		dataType;				// 1 = SMPTE timecode
} imTimeInfoRec8;							

// struct passed during imGetMetaData, imSetMetaData
typedef struct
{
	void*			privatedata;	// instance data from imGetInfo
	void*			prefs;			// persistent data from imGetPrefs
	csSDK_int32		fourCC;			// fourcc code of the metadata chunk
	csSDK_uint32	buffersize;		// size of the buffer
	char*			buffer;			// metadata buffer
} imMetaDataRec;


// struct passed during imAnalysis, filled in by plug-in
typedef struct
{
	void*				privatedata;	// instance data from imGetInfo
	void*				prefs;			// persistent data from imGetPrefs
	csSDK_int32			buffersize;		// the size of the analysis buffer
	char*				buffer;			// analysis buffer to be filled with text
	csSDK_int32			timecodeFormat;	// preferred timecode format
} imAnalysisRec;


// struct passed during imGetPrefs, filled in by plug-in
typedef struct
{
	char*			prefs;				// Buffer to store preferences
	csSDK_int32		prefsLength;		// If "prefs"==0, fill this in with the
										// buffer size needed for preferences
	char			firstTime;			// If 1, then imGetPrefs is being called for the first
										// time. The importer should setup reasonable default values for prefs.
	PrTimelineID	timelineData;		// **6.0** timelineData handle to use with the getPreviewFrame 
										//   callback passed only to synthetic importers when invoked
										//   from the timeline, so they can generate a full preview over
										//   content.   Will be set to NULL in all other cases.
	csSDK_int32		unused;				//	As of CS4, you can no longer set private data during the prefs call, since
										//	it will be called in a different process.
	TDB_TimeRecord	tdbTimelineLocation;	//	For Pro 1.5 tdbTimelineLocation added so synthetic importers 
											//	can get the correct background image during imGetPrefs calls.
	csSDK_int32		sessionPluginID;	// ID for specific plugin instance, valid for this call only; new for 7.5/1.5
	csSDK_int32		imageWidth;			// frame width in pixels
	csSDK_int32		imageHeight;		// frame height in pixels
	csSDK_uint32	pixelAspectNum;
	csSDK_uint32	pixelAspectDen;
	csSDK_int32		vidScale;			// vidScale/vidSampleSize is the preferred video timebase
	csSDK_int32		vidSampleSize;		// vidScale/vidSampleSize is the preferred video timebase
	float			sampleRate;			// Audio, In Hz.
} imGetPrefsRec;
	
// struct passed during imGetInstancePrefs, filled in by plug-in
typedef struct {
	void			*privateData;
	imGetPrefsRec	prefsRec;
} imGetInstancePrefsRec;

#define kMaxAlternateTypes 50

// struct passed during imGetIndFormat
typedef struct {
	csSDK_int32	filetype;				// unique fourcc, e.g.: 'MooV'
	csSDK_int32	flags;					// defines file class (video, still, etc) and loading flags (can this file be in a project?, etc.)
	csSDK_int32	canWriteTimecode;		// Set to 1 if timecode can be written for this filetype.
	char		FormatName[256];		// csSDK_int32 descriptive name
	char		FormatShortName[32];	// short name
	char		PlatformExtension[256];	// file extension(s). 
										// If multiple extensions, separate with nulls, e.g.:
										// "bmp\0dib"

	// Added to support MacOS Navigation Services
	// --- Deprecated ---
	prBool	hasAlternateTypes;
	csSDK_int32	alternateTypes[kMaxAlternateTypes];

	csSDK_int32	canWriteMetaData;		// New in 6.0: Set to 1 if imSetMetaData is supported for the filetype
} imIndFormatRec;


// struct passed during imAnalysis, filled in by plug-in
typedef struct
{
	void*				privatedata;	// instance data from imGetInfo
	void*				prefs;			// persistent data from imGetPrefs
	csSDK_int32			buffersize;		// the size of the analysis buffer
	char*				buffer;			// analysis buffer to be filled with imDataSamples
	csSDK_int32			baserate;		// base data rate per second of the file MINUS the data samples. 
										// This is usually the audio data rate.
} imDataRateAnalysisRec;


// one sample entry for the imDataRateAnalysis message
typedef struct
{
	csSDK_uint32	sampledur;			// duration of one sample in vidTimebase.samplesize increments. Hi bit set if its a keyframe
	csSDK_uint32	samplesize;			// size of this sample in bytes. 
} imDataSample;

// Unicode version of imSaveFileRec
typedef struct
{
	void*					privatedata;		// instance data from imGetInfo
	csSDK_int32*			prefs;				// persistent data from imGetPrefs
	const prUTF16Char*		sourcePath;			// full path for the source file
	const prUTF16Char*		destPath;			// full path for the dest file
	char					move;				// true if it's a move operation
	importProgressFunc		progressCallback;	// function to call repeated with progress and to check for cancel
	void*					progressCallbackID;	// pass back to progressCallback;
} imSaveFileRec8;

// Unicode version of imDeleteFileRec
typedef struct
{
	csSDK_int32			filetype;		// instance data from imGetInfo
	const prUTF16Char*	deleteFilePath;	// full path for the file
} imDeleteFileRec8;

// struct passed during imGetFrameInfo and imGetCompressedFrame to tell the importer what frame
// is being checked
typedef struct
{
	void*			privatedata;			// instance data from imGetInfo
	PrPixelFormat	outPixelFormat;			// fourCC of pixel format
	const void*		prefs;
} imIndPixelFormatRec;

typedef ColorProfileRec imIndColorProfileRec;	// obsolete as of Pr 13.0
typedef ColorSpaceRec	imIndColorSpaceRec;		// color managed importers should use this
typedef EmbeddedLUTRec 	imIndEmbeddedLUTRec;

// struct passed during imCalcSize selector.
typedef struct	
{
	void*			privatedata;	// instance data
	void*			prefs;			// persistent data from imGetPrefs
	csSDK_int32		trimIn;			// requested trim in point 
	csSDK_int32		duration;		// requested duration (0 == current duration, no trim)
	prInt64			sizeInBytes;	// returned size
	csSDK_int32		scale;			// timebase
	csSDK_int32		sampleSize;		// size of one sample
} imCalcSizeRec;


// struct passed during imCheckTrim selector.
typedef struct
{
	void*			privatedata;	// instance data
	void*			prefs;			// persistent data from imGetPrefs
	csSDK_int32		trimIn;			// requested trim in point 
	csSDK_int32		duration;		// requested duration (0 == current duration, no trim)
	csSDK_int32		keepAudio;		// new trimmed file should get source audio
	csSDK_int32		keepVideo;		// new trimmed file should get source video
	csSDK_int32		newTrimIn;		// returned trimIn
	csSDK_int32		newDuration;	// returned duration
	csSDK_int32		scale;			// timebase
	csSDK_int32		sampleSize;		// size of one sample
} imCheckTrimRec;

// return values from progress callback
enum {
	imProgressAbort = 0,
	imProgressContinue = 1
};

// struct passed during imTrimFile selector.
// Unicode version of imTrimFileRec.
typedef struct
{
	void*					privatedata;		// instance data
	void*					prefs;				// persistent data from imGetPrefs
	csSDK_int32				trimIn;				// trim in point 
	csSDK_int32				duration;			// duration
	csSDK_int32				keepAudio;			// new trimmed file should get source audio
	csSDK_int32				keepVideo;			// new trimmed file should get source video
	const prUTF16Char*		destFilePath;		// destination file
	csSDK_int32				scale;				// timebase
	csSDK_int32				sampleSize;			// size of one sample
	importProgressFunc		progressCallback;	// function to call repeated with progress and to check for cancel
	void*					progressCallbackID;	// pass back to progressCallback;
} imTrimFileRec8;

// struct passed during imGetRollCrawlInfo call
typedef struct
{
	void*			privatedata;	// instance data
	void*			prefs;			// persistent data from imGetPrefs
	RollCrawlInfo	rollCrawlInfo;	// filled in by the importer.
} imRollCrawlInfoRec;

// struct passed during imRollCrawlRenderPage call
typedef struct
{
	void*			privatedata;	// instance data
	void*			prefs;			// persistent data from imGetPrefs
	csSDK_int32		page;
	csSDK_int32		width;			// page width
	csSDK_int32		height;			// page height
	csSDK_int32		rowBytes;		// rowbytes;
	char*			pix;			// destination bits
	PrPixelFormat	pixformat;		// format of the pixels as fourcc, 0 = BGRA
} imRollCrawlRenderRec;

// struct passed during imGetPreferredFrameSize call
typedef struct
{
	void*			inPrivateData;
	void*			inPrefs;			// persistent data from imGetSettings
	PrPixelFormat	inPixelFormat;		// format of the pixels as fourcc, 0 = BGRA
	csSDK_int32		inIndex;			// the frame rect index requested
	csSDK_int32		outWidth;			// the width of the preferred frame size. 0 means any width.
	csSDK_int32		outHeight;			// the height of the preferred frame size. 0 means any height.
} imPreferredFrameSizeRec;

// struct passed during imCreateAsyncImporter call
typedef struct
{
	void*					inPrivateData;
	void*					inPrefs;			// persistent data from imGetSettings
	AsyncImporterEntry		outAsyncEntry;		// the entry point for async selectors
	void*					outAsyncPrivateData;// the private data for the async importer
} imAsyncImporterCreationRec;

// struct passed during imDeferredProcessing call
typedef struct
{
	void*					inPrivateData;		// instance data
	float					outProgress;		// if > 0, then progress is reported (0.0 to 1.0);
	char					outInvalidateFile;	// the importer has updated information about the file.
	char					outCallAgain;		// if != 0, then the importer wants to be called immediately for more time.
} imDeferredProcessingRec;

// struct passed during imGetPeakAudio call
typedef struct
{
	void*			inPrivateData;		// instance data
	void*			inPrefs;			// persistent data from imGetPrefs
	PrAudioSample	inPosition;			// the start position of the peak data
	float			inSampleRate;		// The sample rate at which to generate the peak data
	csSDK_int32		inNumSampleFrames;	// The number of sample frames in each buffer
	float**			outMaxima;			// An array of arrays to be filled with the maximum sample values
	float**			outMinima;			// An array of arrays to be filled with the minimum sample values 
} imPeakAudioRec;

// struct passed during imRetargetAccelerator call
typedef struct
{
	const prUTF16Char*	inOriginalPath;		//	the path to the copied original media
	const prUTF16Char*	inAcceleratorPath;	//	the path to the copied accelerator
} imAcceleratorRec;

// struct passed during imCopyFile call
typedef struct
{
	void*				inPrivateData;			// instance data from imGetInfo
	csSDK_int32*		inPrefs;				// persistent data from imGetPrefs
	const prUTF16Char*	inSourcePath;			// full path for the source file
	const prUTF16Char*	inDestPath;				// full path for the dest file
	importProgressFunc	inProgressCallback;		// function to call repeated with progress and to check for cancel
	void*				inProgressCallbackID;	// pass back to progressCallback
} imCopyFileRec;

// struct passed during imQueryDestinationPath call
typedef struct
{
	void*				inPrivateData;			// instance data from imGetInfo
	void*				inPrefs;				// persistent data from imGetPrefs
	const prUTF16Char*	inSourcePath;
	const prUTF16Char*	inSuggestedDestinationPath;
	prUTF16Char*		outActualDestinationPath;
} imQueryDestinationPathRec;

// struct passed during imQueryContentState call
typedef struct  
{
	const prUTF16Char*	inSourcePath;
	prPluginID			outContentStateID;
} imQueryContentStateRec;

// struct passed during imGetSubTypeNames call
typedef struct
{
	csSDK_int32		subType;
	prUTF16Char		subTypeName[256];
} imSubTypeDescriptionRec;

// sent during imQueryInputFileList
// Calling sequence:
//	-- first call: 
//		The plug-in fills out 'numOutputFiles', returns.
//  -- second call: 
//		The host has allocated an array of PrSDKString references (all null).
//		The plug-in should use PrSDKStringSuite to fill that array with 
//		PrSDKStrings containing full file paths.

//	NOTE: the plug-in can fail exSelQueryOutputFileList by returning
//	imUnsupported. The input file is then assumed to be the the singular
//	file opened at imOpenFile time.
//	
typedef struct
{
	void*			inPrivateData;		// instance data from imGetInfo
	void*			inPrefs;			// persistent data from imGetPrefs
	PrSDKString		inBasePath;			// path that was passed during imOpenFile
	csSDK_int32		outNumFilePaths;	
	PrSDKString*	outFilePaths;
} imQueryInputFileListRec;

// sent during imQueryStreamLabel
// if any error is returned, we assume that there is no label for this stream. See some Adobe defined constants in PrSDKStreamLabel.h
typedef struct
{
	void*			inPrivateData;
	const void*		inPrefs;
	csSDK_int32		inStreamIdx;
	PrSDKString		outStreamLabel;
} imQueryStreamLabelRec;

typedef PREMPLUGENTRY (*ImportEntryFunc)(csSDK_int32 selector, imStdParms* stdparms, void* param1, void* param2);



// struct passed during imInitiateAsyncClosedCaptionScan
typedef struct
{
	void*					inPrivateData;					// instance data
	const void*				inPrefs;						// persistent data from imGetPrefs
	void* 					outAsyncCaptionScanPrivateData;	// instance data for this closed caption scan
	csSDK_int64				outScale;						// numerator of the frames per second (e.g. 30000)
	csSDK_int64				outSampleSize;					// denominator of the frames per second (e.g. 1001)
	csSDK_int64				outEstimatedDuration;			// The estimated duration of all the captions,
															//	measured in the above size / scale
	imImageInfoRec			inVidInfo;						// Information about the video data
} imInitiateAsyncClosedCaptionScanRec;

typedef enum 
{
	kPrClosedCaptionFormat_Undefined = 0,
	kPrClosedCaptionFormat_Australian,			// Plug-In is providing an Australian captioning Byte Stream
	kPrClosedCaptionFormat_CEA608 = 608,		// Plug-In is providing a CEA-608 Byte Stream
	kPrClosedCaptionFormat_CEA708 = 708,		// Plug-In is providing a CEA-708 Byte Stream (may contain 608 data wrapped in 708)
	kPrClosedCaptionFormat_TTML = 0x54544d4c	// 'TTML'...
												// Plug-In is providing a W3C TTML string that conforms to the
												//	W3C Timed Text Markup Language (TTML) 1.0: (http://www.w3.org/TR/ttaf1-dfxp/)
												//	or optionally conforming to SMPTE ST 2052-1:2010:
												//	(http://store.smpte.org/product-p/st%202052-1-2010.htm)
												//	or optionally conforming to EBU Tech 3350
												//	(http://tech.ebu.ch/webdav/site/tech/shared/tech/tech3350.pdf)
												// If the TTML string contains tunneled data (e.g. CEA-608 data),
												//	then it is preferred that the plug-in provide that through
												//	the appropriate byte stream format (e.g. kPrClosedCaptionFormat_CEA608)
} PrClosedCaptionFormat;

// struct passed during imGetNextClosedCaption, filled in by plug-in
const size_t kTTMLBufferSize = 512;
typedef struct
{	
	void*					inPrivateData;			// instance data
	const void*				inPrefs;				// persistent data from imGetPrefs
	void* 					inAsyncCaptionScanPrivateData;	// instance data from imInitiateAsyncClosedCaptionScan
	float					outProgress; 			//between 0.0 and 1.0

	csSDK_int64				outScale;				// numerator of the frames per second (e.g. 30000)
	csSDK_int64				outSampleSize;			// denominator of the frames per second (e.g. 1001)
	csSDK_int64				outPosition;			// the position of the closed caption
													// measured in the above size / scale

	PrClosedCaptionFormat	outClosedCaptionFormat;	// Format for this closed caption

	PrMemoryPtr				outCaptionData;			// Memory location to where the plug-in
													//	should write the closed caption bytes
													//	if providing CEA-608 or CEA-708
	
	prUTF8Char				outTTMLData[kTTMLBufferSize];	// UTF-8 String of valid W3C TTML, SMPTE-TT, or EBU-TT data
															// The entire string may be split into substrings
															// (e.g. line by line) and the host will concatenate
															// and decode them (only used when outCaptionData == kPrClosedCaptionFormat_TTML)

	csSDK_size_t			ioCaptionDataSize;		// Size of outCaptionData buffer (in bytes) allocated
													//	from the host.  The plug-in should set
													//	this variable to the actual number of
													//	bytes that were written to outCaptionData,
													//	or the length of the string (characters, not bytes)
													// pointed by outTTMLData.
} imGetNextClosedCaptionRec;

// struct passed during imCompleteAsyncClosedCaptionScan
typedef struct
{
	void*					inPrivateData;					// instance data
	const void*				inPrefs;						// persistent data from imGetPrefs
	void* 					inAsyncCaptionScanPrivateData;	// Will be invalid after this call
	prBool					inScanCompletedWithoutError;
} imCompleteAsyncClosedCaptionScanRec;

// struct passed during imGetAudioChannelLayout
typedef struct
{
	void*					inPrivateData;								// instance data
	PrAudioChannelLabel		outChannelLabels[kMaxAudioChannelCount];	// audio channel labels, a valid label should be assigned for each channel in the file
} imGetAudioChannelLayoutRec;

// struct passed during imSelectClipFrameDescriptor
typedef struct
{
	void*					inPrefs;						// persistent data from imGetPrefs
	ClipFrameDescriptor		inDesiredClipFrameDescriptor;	// requested frame properties
	ClipFrameDescriptor		outBestFrameDescriptor;			// frame properties to be produced, filled in with initial guesses
	void*					inPrivateData;					// new to 14.0.2: instance data
} imClipFrameDescriptorRec;

// struct for imPerformSourceSettingsCommand. The data can be anything as long as both the importer and the source settings effect both know what it is.
typedef struct
{
	char*					ioData;
	csSDK_int32				inDataSize;
	void*					inPrivateData;
} imSourceSettingsCommandRec;

// Note that this information may be serialized and stored with a project.  If the importer makes any change at all to the extent or interpretation of the format data, the UUID should be updated.
typedef struct
{
	void*					inPrivateData;			// instance data
	const void*				inPrefs;				// persistent data from imGetPrefs
	csSDK_int32				inStreamIdx;			// stream index number
	csSDK_int32				ioFlags;				// reserved for future use
	unsigned char			outFormatID[16];		// A UUID identifying the precise category of extended format data structure (type, version, etc) held by the buffer, to be filled in by the plug-in
	csSDK_int32				outFormatInfoSize;		// Size of the format info provided
	PrMemoryPtr				outFormatInfo;			// The extended format information that can be understood by an exporter via the exExtendedFormatInfoRec structure.  (Allocated by the plug-in using PlugMemoryFuncs)
} imExtendedFormatInfoRec;

// struct passed during imGetDataStreamsInfo call
typedef struct
{
	void*					inPrivateData;					// instance data
	const void*				inPrefs;						// persistent data from imGetPrefs
	csSDK_uint32			outNumDataStreams;				// the number of data streams present in the file
} imGetDataStreamsInfoRec;

// struct passed during imGetDataStream call
typedef struct
{
	void*					inPrivateData;					// instance data
	const void*				inPrefs;						// persistent data from imGetPrefs
	csSDK_uint32			inIndex;
	PrDSGroupChildType		outDataStreamType;
	PrSDKString				outMatchName;
	PrSDKString				outDisplayName;
} imGetDataStreamRec;

// struct passed during imGetDataSample call
typedef struct
{
	void*					inPrivateData;					// instance data
	const void*				inPrefs;						// persistent data from imGetPrefs
	PrSDKString				inMatchName;					// 
	
	csSDK_int64				inScale;						// numerator of the samples per second (e.g. 30000)
	csSDK_int64				inSampleSize;					// denominator of the samples per second (e.g. 1001)
	csSDK_int64				inPosition;						// the position of the desired sample value
															// measured in the above size / scale
	
	float					outValue;						// @TODO: Accommodate other data types
	
} imGetDataSampleRec;
	
// struct passed during imGetDataStreamChunk call
typedef struct
{
	void*					inPrivateData;					// instance data
	const void*				inPrefs;						// persistent data from imGetPrefs
	csSDK_uint32			inIndex;
	char*					ioBuffer;
	csSDK_size_t			inBufferSize;
	csSDK_size_t			outReadSize;
	csSDK_size_t			outRemainingSize;
} imGetDataStreamChunkRec;
	
// struct passed during imGetDataFileInfo call
typedef struct
{
	void*				inPrivateData;					// instance data
	const void*			inPrefs;						// persistent data from imGetPrefs
	PrRatioTime			outTotalStreamDuration;			// total duration of streams
	PrSDKString			outDataFileCreator;				// creator tool of data file.
	PrSDKString			outErrorMsg;					// Error message if failure occurs.
	int32_t				outErrorId;						// ErrorId of error message.
} imGetDataFileInfoRec;
	
// struct passed during imGetDataHierarchy call
typedef struct
{
	void*				inPrivateData;					// instance data
	const void*			inPrefs;						// persistent data from imGetPrefs
	PrMemoryPtr			inBaseHierarchyPath;			// path of stream whose hierarchy is required. Top level hierarchy is returned, if NULL
	PrMemoryPtr			outHierarchyPathBuffer;			// buffer contains path of streams under base hierarchy path.
	csSDK_uint32		outHierarchyPathBufferSize;		// size of hierarchy path buffer.
	void*				outPathLengthBuffer;			// buffer holding size of each stream path under base hierarchy path.
	csSDK_uint32		outPathCount;					// total paths under base hierarchy path.
	PrSDKString			outErrorMsg;					// Error message if failure occurs.
	int32_t				outErrorId;						// ErrorId of error message.
} imGetDataHierarchyRec;
	
// struct passed during imGetStreamInfo call
typedef struct
{
	void*				inPrivateData;						// instance data
	const void*			inPrefs;							// persistent data from imGetPrefs
	PrMemoryPtr			inStreamPath;						// path of stream whose info is required.
	PrSDKString			outDisplayName;						// display name of stream
	PrSDKString			outMatchName;						// match name of stream
	int32_t				outStreamType;						// stream type
	int32_t				outDSValueType;						// data type of values in stream
		
	//Data type properties related info.
	prBool				outIsEventMarker;					// holds true if string value is an event marker
	prBool 				outIsPoint;							// holds true if number array represents point.
	PrSDKString*		outDisplayNames;					// name used for elements of the number array.
	double				outLegalMin;						// minimum legal number value allowed.
	double				outLegalMax;						// maximum legal number value allowed.
	double				outOccuringMin;						// minimum occurred value in samples.
	double				outOccuringMax;						// maximum occurred value in samples.
	csSDK_uint64		outDecimalPrecision;				// decimal precision, if data type is number or numberArray
	
	//Static stream related info
	PrSDKString			outStringValue;						// hold string value if data type is string.
	double				outNumberValue;						// hold number value if data type is number.
	double* 			outNumberArrayValue;				// hold number array if data type is number array.
	csSDK_int8			outArrayItemCount;					// number of items in number array(max 3)
	prBool				outBooleanValue;					// hold boolean value if data type is boolean.
		
	//Dynamic stream related info
	PrSDKString			outSampleSetId;						// sample id of the dynamic stream.
	csSDK_uint64		outSampleCount;						// total samples present in dynamic stream.
	int32_t				outInterpolationType;				// interpolation info of dynamic stream.
	prBool				outHasExpectedSampleFrequencyB; 	// true means minimum number of samples is expected.
	PrRatioTime			outStartSampleTime;					// first temporal value of dynamic stream.
	PrRatioTime			outEndSampleTime;					// last temporal value of dynamic stream.
	PrSDKString			outExpectedMaxInterSampleDuration; 	// max expected duration allowed between adjacent samples
		
	PrSDKString			outErrorMsg;						// Error message if failure occurs.
	int32_t				outErrorId;							// ErrorId of error message.
} imGetDataStreamInfoRec;
	
// struct passed during imGetStreamTemporalSamples call
typedef struct
{
	void*				inPrivateData;						// instance data
	const void*			inPrefs;							// persistent data from imGetPrefs
	PrMemoryPtr			inSampleSetId;						// sample set id of dynamic stream, obtained from imGetDataStreamInfoRec::outSampleSetId
	PrRatioTime			inSampleStartTime;					// time from where samples are required.
	PrRatioTime			inSampleEndTime;					// time till samples are required.
	prBool				inAdjacentSampleRequired;			// true if adjacent samples are required, in this case only inSampleStartTime will be consider for adjacent samples.
	csSDK_uint64		outSampleCount;						// number of samples in between inStartTime and inEndTime.
	int32_t				outValueType;						// value type of samples.
	PrRatioTime*		outSampleTimeBuffer;				// buffer holds temporal info about samples.
	void*				outValueBuffer;						// buffer holds data values.
	size_t				outArrayItemCount;					// number of items in number array(max 3)
	PrSDKString			outErrorMsg;						// Error message if failure occurs.
	int32_t				outErrorId;							// ErrorId of error message.
} imGetStreamTemporalSamplesRec;
	
// struct passed during imGetDataStreamVideoSyncTime call
typedef struct
{
	void*					inPrivateData;					// instance data
	const void*				inPrefs;						// persistent data from imGetPrefs
	csSDK_uint32			inIndex;						// required index of sync list
	PrSDKString 			outVideoFileName;				// Video file name associated with the data stream.
	int32_t					outVideoSyncTimeType;			// time type of video sync - frame or time string
	PrSDKString				outTimeString;					// time string value if sync time type is sting.
	csSDK_int32				outFrameNumber;					// frame number if sync type is frame.
	PrSDKString				outErrorMsg;					// Error message if failure occurs.
	int32_t					outErrorId;						// ErrorId of error message.
}imGetDataStreamVideoSyncTimeRec;

// struct passed during imGetCurrentSystemState
typedef struct
{
	csSDK_uint32			inSystemStateMask;				// Mask of bits of interest to the host, e.g. 0x1 = bit 0 of the response should be valid, 0xC = bits 2 and 3 of the response should be valid.
	csSDK_uint32			outCommonStateMask;				// Mask reporting the bits that are of interest to all files opened by the importer.  If new outCommonStateMask bits are introduced, all previous cache entries for the importer will be flushed.
	csSDK_uint32			outCurrentSystemState;			// Bits representing the current system state.  outCurrentSystemState bits that are not requested by inSystemStateMask are ignored.
	csSDK_uint32			inSubtype;						// Codec subtype from imFileInfoRec8::vidInfo::subType for the current query.
	csSDK_uint32			outSubtypeVersion;				// Current version number specific to inSubtype.  If the cached version for the given subtype is different from this value, the cache for the media will be refreshed.
	csSDK_uint32			reserved[6];					// Reserved; set to null by the host.
} imCurrentSystemStateRec;

// importer selectors:

enum PrImporterSelector
{
	imInit = 0,
	imShutdown = 1,
	imImportImage = 5,
	imQuietFile = 8,
	imCloseFile = 9,
	imAnalysis = 12,
	imDataRateAnalysis = 13,
	imGetIndFormat = 14,
	imGetMetaData = 23,						// 6.0
	imSetMetaData = 24,						// 6.0
	imImportAudio7 = 25,					// 7.0 new audio import call
	imGetIndPixelFormat = 27,				// 7.0 
	imGetSupports7 = 28,					// 7.0 plugin must return malSupports7
	imResetSequentialAudio = 29,
	imGetSequentialAudio = 30,
	imGetRollCrawlInfo = 32,				// 7.x
	imRollCrawlRenderPage = 33,				// 7.x
	imGetSupports8 = 36,					// Pro 2.0
	imGetInfo8 = 37,						// Pro 2.0
	imGetPrefs8 = 38,						// Pro 2.0
	imOpenFile8 = 39,						// Pro 2.0
	imSaveFile8 = 40,						// Pro 2.0
	imDeleteFile8 = 41,						// Pro 2.0
	imCalcSize8 = 42,						// Pro 2.0
	imCheckTrim8 = 43,						// Pro 2.0
	imTrimFile8 = 44,						// Pro 2.0
	imGetPreferredFrameSize = 45,			// Pro 2.0
	imCreateAsyncImporter = 46,				// Pro 2.0
	imGetSourceVideo = 47,					// Pro 2.0
	imDeferredProcessing = 48,				// Pro 2.0
	imGetPeakAudio = 49,					// Pro 2.0
	imRetargetAccelerator = 50,				// Pro 2.0
	imCopyFile = 51,						// Pro 2.0
	imGetTimeInfo8 = 52,					// Pro 2.0
	imSetTimeInfo8 = 53,					// Pro 2.0
	imGetSubTypeNames = 54,					// Pro 3.0
	imGetFileAttributes = 55,				// Pro 3.0
	imQueryDestinationPath = 56,			// Pro 5.0
	imQueryContentState = 57,				// Pro 5.0
	imGetIndColorProfile = 58,				// Pro 5.5
	imQueryInputFileList = 59,				// Pro 6.0
	imQueryStreamLabel = 60,				// Pro 6.0
	imInitiateAsyncClosedCaptionScan = 61,	// Pro 7.0
	imGetNextClosedCaption = 62,			// Pro 7.0
	imCompleteAsyncClosedCaptionScan = 63,	// Pro 7.0
	imGetAudioChannelLayout = 64, 			// Pro 7.0
	imSelectClipFrameDescriptor = 65,		// Pro 8.0
	imPerformSourceSettingsCommand = 66,	// Pro 9.0
	imGetExtendedFormatInfo = 67,			// Pro 12.0.1
	imGetDataStreamsInfo = 68,				// Pro 12.1
	imGetDataStream = 69,					// Pro 12.1
	imGetDataSample = 70,					// Pro 12.1
	imGetSupportsPerInstancePrefs = 71,		// Pro 12.1; indicates whether imGetInstancePrefs is supported
	imGetInstancePrefs = 72,				// Pro 12.1; same as imGetPrefs8 but instance-based, not static
	imGetIndColorSpace = 73,				// Pro 13.0; used to get media color space definition
	imGetDataStreamChunk = 74,				// Pro 13.0; used to get data stream mgjson in chunks
	imGetDataFileInfo = 75,
	imGetDataHierarchy = 76,
	imGetStreamInfo = 77,
	imGetStreamTemporalSamples = 78,
	imGetDataStreamVideoSyncTime = 79,
	imGetCurrentSystemState = 80,			// Pro 14.0.2
	imGetInfo9 = 81,						// Pro 14.1; Moving new GetInfo items to a structure with reserved padding for the future
	imGetEmbeddedLUT = 82,
};

// Error Return codes

enum PrImporterReturnValue {
	imNoErr = 0,				// No Error
	imTooWide = 1,				// File dimensions to large
	imBadFile = 2,				// Bad file format
	imUnsupported = 3,			// Unsupported selector
	imMemErr = 4,				// Memory Error
	imOtherErr = 5,				// Unknown Error
	imNoContent = 6,			// No audio or video
	imBadRate = 7,				// Bad Audio rate
	imBadCompression = 8,		// Bad compression
	imBadCodec = 9,				// Codec not found
	imNotFlat = 10,				// Unflattened QuickTime movie
	imBadSndComp = 11,			// Bad sound compression
	imNoTimecode = 12,			// Timecode supported, but not found
	imMissingComponent = 13,	// Missing component needed to open the file
	imSaveErr = 14,				// error saving file
	imDeleteErr = 15,			// error deleting file
	imNotFoundErr = 16,			// metadata chunk not found
	imSetFile = 17,				// returned by 'custom' importers only in imGetPrefs after file creation
	imIterateStreams = 18,		// returned from GetInfo to indicate there are multiple input streams
	imBadStreamIndex = 19,		// returned from GetInfo to indicate there are no more streams.
	imCantTrim = 20,			// returned from imCheckTrim if the file should not be trimmed.
	imDiskFull = 21,			// disk full trimming file
	imDiskErr = 22,				// other disk or I/O error trimming file
	imFileShareViolation = 23,	// File cannot be opened due to another process having exclusive read access
	imIterateFrameSizes = 24,	// Returned from imGetPreferredFrameSize when there are more frame sizes for a particular
								// pixel format.
	imMediaPending = 25,		// The importer is still working on the file.
	imDRMControlled = 26,		// The file cannot be opened because it is under rights management.
    imActivationFailed = 27,    // Activation of a component failed (mostly due to user cancellation)
	imFrameNotFound = 28,		// An importer could not find the requested frame (typically used with async importers)
	imBadHeader = 29,			// The file cannot be opened because of a header error.
	imUnsupportedCompression = 30,	// The file has a compression type that we do not support
	imFileOpenFailed = 31,		// We were unable to open the file on disk
	imFileHasNoImportableStreams = 32,	// The file has no audio or video streams
	imFileReadFailed = 33,		// A read from an open file failed
	imUnsupportedAudioFormat = 34,	// Something in the audio format is not importable
	imUnsupportedVideoBitDepth = 35,	// The video bit depth of this file is unsupported
	imDecompressionError = 36,	//	There was an error decompressing audio or video
	imInvalidPreferences = 37,	//	Invalid prefs data was passed to the plugin
	inFileNotAvailable = 38,    // return value from imQueryContentState if the file/stream is no longer available. e.g. deleted or offline
	imRequiresProtectedContent = 39,
	imNoCaptions = 40,			//	return value for imInitiateAsyncClosedCaptionScan and imGetNextClosedCaption which will signal to stop calling imGetNextClosedCaption
	imMultipleDescription = 41, // Video contains multiple image description
	imComplexEditList = 42,		// Video contains complex edit list
	imDelegate = 255			// internal: delegate this file to another importer
};

// Other Return codes

#define imCancel			300				// Returned from imGetPrefs
#define imBadFormatIndex	301				// Returned from imGetIndFormat
#define imIsCacheable		400				// Returned from imInit, non-zero if the plugin can be lazy-inited,
											// zero if the plugin must be loaded from disk on startup

/* 
importer messages:

Message:	imInit
Required:	yes
Purpose:	Called once at application startup. Importer may need certain 
			drivers or hardware to be present and initialized.
			If any error is returned, importer and file types will not be used.
			If the plugin does not have to be loaded immediately upon application startup,
			then it should return imIsCacheable instead of imNoError.
			param1 : imImportInfoRec*

Message:	imShutdown
Required:	yes
			Called once at application shutdown. Allows importer to release drivers or other resources.

Message:	imGetIndFormat
Required:	yes
			Repeatedly called with an ordinal to request all the import file formats that
			this importer can handle. Importer should return imBadFormatIndex after
			all formats have been enumerated.
			parm 1 : format to return, 0-n
			parm 2 : imIndFormatRec*

Message:	imGetInfo8
Required:	yes (one of imGetInfo8 or imGetInfo9 must be supported)
			Called when a specific file is instantiated. Importer checks file validity,
			Optionally allocates file instance data, and fills in an imFileInfoRec8.
			parm1 : imFileAccessRec8*
			parm2 : imFileInfoRec8*

Message:	imGetFrameInfo
Required:	no
			Called to get information about a specific frame in a file.
			The file will already be opened. The importer reads information about the
			frame requested and fills in the imFrameInfoRec with information about the
			frame.
			param1 : imGetFrameInfoRec*
			param2 : imFrameInfoRec*
			
Message:	imDisposeFrameInfo
Required:	yes, if imGetFrameInfo returns data that needs to be disposed
			param1 : csSDK_int32
			
Message:	imGetCompressedFrame
Required:	no
			param1 : imGetFrameInfoRec*
			param2 : csSDK_int32*
			
Message:	imDisposeCompressedFrame
Required:
			param1 : csSDK_int32
			
Message:	imImportImage
Required:	yes, if there's image data
			Called to rasterize or draw an image in the file.
			param1 : imFileRef
			param2 : imImportImageRec*
			
Message:	imImportAudio:
Required:	yes, if there's audio data
			Called to import audio
			param1 : imFileRef
			param2 : imImportAudioRec*
			
Message:	imOpenFile
Required:	yes, if canOpen is true
			If importer sets canOpen, this message will be sent to
			open the file. imFileRef* should be filled in with the file reference
			param1 : imFileOpenRec*
			param2 : imFileRef*
			
Message:	imOpenFile8
Required:	yes, if canOpen is true
			If importer sets canOpen, this message will be sent to
			open the file. imFileRef* should be filled in with the file reference
			param1 : imFileOpenRec8*
			param2 : imFileRef*

Message:	imQuietFile
Required:	yes, if canOpen is true
			If importer requests specialOpen status, this message will be sent to
			allow the importer to release memory and drivers
			param1 : imFileRef
			param2 : privatedata

Message		imCloseFile
Required:	yes
			Called when the file will no longer be used by the application.
			File instance data is disposed of during this call.
			If importer set the canOpen flag, the file should also be closed.
			param1 : imFileRef*
			param2 : private data from imFileInfo
			
Message		imGetPrefs
Required:	no
			Called to get any settings that the importer wants to store.
			If the file is synthetic (imNoFile), then the importer can change the name
			field in the imFileAccessRec parameter to give the file a displayable name.
			The importer should return imNoErr if OK.
			The importer can also return imCancel.
			param1 : imFileAccessRec*
			param2 : imGetPrefsRec*
			
Message		imGetPrefs8
Required:	no
			Called to get any settings that the importer wants to store.
			If the file is synthetic (imNoFile), then the importer can use the newfilename
			field in the imFileAccessRec8 parameter to give the file a displayable name.
			The importer should return imNoErr if OK.
			The importer can also return imCancel.
			param1 : imFileAccessRec8*
			param2 : imGetPrefsRec*

Message		imSetPrefs
Required:	no
			Called to give the importer any settings it saved previously.
			This will always be followed by a call to imFileInfo to allow the importer to
			change any fields in imFileInfoRec that depend on saved preferences.
			param1 : char*			Preferences buffer
			
Message		imGetTimeInfo
Required:	no
			Called to get timecode info from a file. 
			param1 : imFileRef
			param2 : imTimeInfoRec*
			
Message:	imSetTimeInfo
Required:	no
			Called to set the timecode in a file. 
			param1 : imFileRef
			param2 : imTimeInfoRec*

Message:	imAnalysis
Required:	no
			Called to get a text description of the file.
			If the importer doesn't handle this, a default analysis will be done (based on
			the info returned from imGetInfo.
			If the buffersize passed in imAnalysisRec is not big enough for the analysis, the
			importer can change this field to request a larger size.
			param1 : imFileRef
			param2 : imAnalysisRec*
			
Message:	imDataRateAnalysis
Required:	no
			Called to get a data rate analysis of the file
			This will be called once with imDataRateAnalysisRec.buffer == nil, the importer should
			fill in imDataRateAnalysisRec.buffersize to the size needed hold its data rate analysis.
			When this is called with imDataRateAnalysisRec.buffer != nil, the importer should fill
			in the buffer with its data rate analysis
			param1 : imFileRef
			param1 : imDataRateAnalysisRec*

Message:	imSaveFile
Required:	no, unless "canSave" is set
			Called to save a file to the fsspec given in param2.
			param1 : imSaveFileRec*	

Message:	imSaveFile8
Required:	no, unless "canSave" is set
			Called to save a file to the fsspec given in param2.
			param1 : imSaveFileRec8*	

Message:	imDeleteFile
Required:	no, unless "canDelete" is set
			Called to delete an importer file, usually because the file
			has associated child files that the host can't know about.
			param1 : imDeleteFileRec*	

Message:	imDeleteFile8
Required:	no, unless "canDelete" is set
			Called to delete an importer file, usually because the file
			has associated child files that the host can't know about.
			param1 : imDeleteFileRec8*	

Message:	imGetMetaData
Required:   no
			Called to get a metadata chunk specified by a fourcc code
			If the imMetaDataRec::buffer pointer is nil, the plug-in
			should set the buffersize field to the needed buffer size,
			and return imNoErr.  Premiere will call again with the 
			appropriate buffer already allocated.
			param1 : imFileRef
			param2 : imMetaDataRec*
			Return values: imNoErr, imNotFoundErr, imUnsupported

Message:	imSetMetaData
Required:   no
			Called to add a metadata chunk specified by a fourcc code.
			param1 : imFileRef
			param2 : imMetaDataRec*
			Return values: imNoErr, imNotFoundErr, imUnsupported
			
Message:	imImportAudio7:
Required:	yes, if there's audio data
			Called to import audio using the new audio format
			param1 : imFileRef
			param2 : imImportAudioRec7*

Message:	imGetIndPixelFormat
Required:	no
			Called to enumerated the pixel formats available for a specific file.
			This message will be sent repeatedly until all formats have been 
			returned. Pixels formats should be returned in the preferred order
			that the importer supports. The host will attempt to always talk
			to the importer in the preferred pixel format if possible, but
			all importers must support BGRA pixel format as well. The Importer 
			should return imBadFormatIndex after all formats have been enumerated. 
			imUnsupported should be returned on the first call if only classic 
			BGRA format is supported.
			parm 1 : format to return, 0-n
			parm 2 : imIndPixelFormatRec*

Message:	imResetSequentialAudio:
Required:	no
			If the plugin supports imGetSequentialAudio, then this selector will be
			called to reset the file pointer.
			param1 : imFileRef
			param2 : imImportAudioRec7*

Message:	imGetSequentialAudio:
Required:	no
			If the plugin can support sequential audio calls interleaved with random access
			calls, it should add the flag kSeparateSequentialAudio to accessModes in 
			imFileInfoRec. If it does this, then sequential calls during conforming will
			use this selector to get audio instead of imImportAudio7.
			param1 : imFileRef
			param2 : imImportAudioRec7*

Message:	imCalcSize
Required:	no
			This selector is called if the importer sets canCalcSizes to non-zero in 
			imImportInfoRec. An importer should support this call if it uses a tree
			of files represented as one top-level file to Premiere.
			The importer should calculate either the trimmed or current size of the
			file and return it. See further comments in the imCalcSizeRec struct
			param1: imCalcSizeRec*
			param2: imFileAccessRec*

Message:	imCalcSize8
Required:	no
			This selector is called if the importer sets canCalcSizes to non-zero in 
			imImportInfoRec. An importer should support this call if it uses a tree
			of files represented as one top-level file to Premiere.
			The importer should calculate either the trimmed or current size of the
			file and return it. See further comments in the imCalcSizeRec struct
			param1: imCalcSizeRec*
			param2: imFileAccessRec8*

Message		imCheckTrim
Required:	no
			This selector is called when the host wants to trim a file. The importer 
			examines the proposed trimmed size of the file, and can change the requested
			in point and duration to new values if the file can only be trimmed at
			certain locations (GOP boundaries in MPEG files would be an obvious
			example of this type of file). If the importer changes the in and duration,
			the new values must include all the material requested in the original
			trim request. If an importer does not need to change the in and duration,
			it may either return imUnsupported, or imNoErr and simply not change the
			in/duration fields. If the importer does not want the file trimmed (perhaps
			because the audio or video would be degraded if recompressed?) it can return
			imCantTrim and the trim operation will fail and the file will be copied instead.
			param1: imCheckTrimRec*
			param2: imFileAccessRec*

Message		imCheckTrim8
Required:	no
			This selector is called when the host wants to trim a file. The importer 
			examines the proposed trimmed size of the file, and can change the requested
			in point and duration to new values if the file can only be trimmed at
			certain locations (GOP boundaries in MPEG files would be an obvious
			example of this type of file). If the importer changes the in and duration,
			the new values must include all the material requested in the original
			trim request. If an importer does not need to change the in and duration,
			it may either return imUnsupported, or imNoErr and simply not change the
			in/duration fields. If the importer does not want the file trimmed (perhaps
			because the audio or video would be degraded if recompressed?) it can return
			imCantTrim and the trim operation will fail and the file will be copied instead.
			param1: imCheckTrimRec*
			param2: imFileAccessRec8*

Message		imTrimFile
Required:	no
			param1: imFileAccessRec*
			param1: imTrimFileRec*

Message		imTrimFile8
Required:	no
			param1: imFileAccessRec8*
			param1: imTrimFileRec8*

Message:	imGetRollCrawlInfo
Required:	no
			This selector is called if the importer sets isRollCrawl to non-zero in 
			imImportInfoRec. The importer should fill in a RollCrawlInfo struct.
			param1: imRollCrawlInfoRec*
			param2: unused

Message:	imRollCrawlRenderPage
Required:	no
			This selector is called if the importer sets isRollCrawl to non-zero in 
			imImportInfoRec. The importer render the requested page of the roll/crawl.
			param1: imRollCrawlRenderRec*
			param2: unused

Message:	imGetPreferredFrameSize
Required:	no
			This selector is used by Premiere determine if the plugin has one or more preferred
			frames sizes for a given pixel format. A preferred frame size is one which the importer
			feels it can scale its output to faster than the host can. The native frame size is
			usually a preferred size, but there may be others, such as 1/4 native frame. If
			the importer can naturally render to any frame size efficiently, then 0 should be
			returned as the width and height.

			param1: imPreferredFrameSizeRec*
			param2: unused

Message:	imCreateAsyncImporter
Required:	only if importer returns supportsAsyncIO != 0 in imImageInfoRec
			This selector creates a new AsyncImporter and returns an entry point to it, along with
			private data for it. See PrSDKAsyncImporter.h for further information.
			
			param1:	imAsyncImporterCreationRec*
			param2: unused

Message:	imGetSourceVideo
Required:	only if importer returns supportsGetSourceVideo != 0 in imImageInfoRec
			This selector is a different way for the host to get a video frame from the importer.
			Instead of pushing down a frame buffer and format, this call requests a frame that
			is allocated by the importer. The importer is free to match any of the requested
			formats. If it cannot match the requested formats, it is free to fail.
			
			param1:	mFileHandle
			param2: imSourceVideoRec*
	
Message:	imDeferredProcessing
Required:	no.
			This selector is a way for the importer plugin to request some processing time.
			param1:	imDeferredProcessingRec*	
			param2: unused

Message:	imGetPeakAudio:
Required:	no
			This selector is used by Premiere to obtain peak data used to generate waveforms.
			The peak data is returned in two arrays of buffers, one for the maximum values and
			one for the minimum values. In Pro 2.0 and later, this will only be called if the importer is
			synthetic and its duration is longer than a few minutes (including infinite).
			
			param1 : imFileRef
			param2 : imPeakAudioRec*

Message:	imRetargetAccelerator
Required:	no
			This selector is static (no instance data). It is called when the host has copied
			an original with a registered accelerator matching the file extension reported in
			imInit. The host passes the path to the new original and the new accelerator, and
			the plugin should update the accelerator to match the original, however necessary.
			
			param1 : imAcceleratorRec*
			param2 : unused

Message:	imCopyFile
Required:	no, unless "canCopy" is set
			This selector is called to copy the file represented by the inSourcePath to the
			inDestPath. After the copy operation is complete the importer should still be
			referring to the original file and not the copied file.
			param1 : imCopyFileRec*
			param2 : unused

Message		imGetTimeInfo8
Required:	no
			Called to get timecode info from a file. 
			param1 : imFileRef
			param2 : imTimeInfoRec8*
			
Message		imGetSubTypeNames
Required	no
			Called for each fileType returned by imGetIndFormat.
			The plug-in should return an array of imSubTypeDescriptionRec

			param1:	in:		csSDK_int32 fileType as returned from imGetIndFormat
			param2:	out:	imSubTypeDescriptionRec*, allocated by the plugNewPtrFunc provided by 
							the host. This array will be released by the host.


Message:	imGetIndColorProfile - obsolete as of Pr 13.0
Required:	Yes, if the Importer reported that it supports color profiles
			Called to enumerate the supported color profiles.
			If the support is _Fixed, we will iterate until we reach an error return. We will default to profile 0.

			The first time this is called, the inDestinationBuffer will be NULL. Fill in the required size for the buffer, and we'll call back with enough memory.
			param 1: format to return, 0-n
			param 2: imIndColorProfileRec*

Message:	imInitiateAsyncClosedCaptionScan
Required:	no
			Called to get closed captions embedded in the file.
			param1 : imFileRef
			param2 : imInitiateAsyncClosedCaptionScanRec*
			
Message:	imGetNextClosedCaption
Required:	no
			Called to get closed captions embedded in the file.
			param1 : imFileRef
			param2 : imGetNextClosedCaptionRec*
			
Message:	imCompleteAsyncClosedCaptionScan
Required:	no
			Called to get closed captions embedded in the file.
			param1 : imFileRef
			param2 : imCompleteAsyncClosedCaptionScanRec*

Message:	imGetAudioChannelLayout
Required:	no
			Called to get the audio channel layout in the file.
			param1 : imFileRef
			param2 : imGetAudioChannelLayoutRec*

Message:	imSelectClipFrameDescriptor
Required:	no
			Called to negotiate the clip frame descriptor for render.
			param1 : imFileRef
			param2 : imClipFrameDescriptorRec*

Message:	imPerformSourceSettingsCommand
Required:	no
			Called to initialize the prefs, when the importer has an associated source settings effect
			param1 : imFileAccessRec8*
			param2 : imSourceSettingsCommandRec*
			
Message:	imGetExtendedFormatInfo
Required:	no
			Called to obtain extended details of the file format for use by an associated exporter for match source and/or smart rendering.
			param1 : imFileRef
			param2 : imExtendedFormatInfoRec*

Message:	imGetDataStreamsInfo
Required:	no
			Called to get information about the data stream(s) contained in the file
			param1 : imFileRef*
			param2 : imGetDataStreamsInfoRec*
			
Message:	imGetDataStream
Required:	no
			Called to get information about a particular data stream
			param1 : imFileRef*
			param2 : imGetDataStreamRec*
			
Message:	imGetDataSample
Required:	no
			Called to get a data sample for a particular data stream
			param1 : imFileRef*
			param2 : imGetDataSampleRec*
 
 Message	imGetInstancePrefs
 Required:	no
			Called when user wants to modify existing importer prefs.
			Unlike old imGetPrefs8 it is instance-based and not static.
			This allows importers to cache source settings as recorded
			in source when processing OnGetInfo and use them when handling
			user changes to source settings allowing user to get to "as shot"
			settings. Please note that the static version of imGetPrefs8 is
			still needed as importer host can call importer prior to Open but
			all the processing of user changes should shift to new per instance
			API.
			The importer should return imNoErr if OK.
			The importer can also return imCancel.
			param1 : imFileAccessRec8*
			param2 : imGetInstancePrefsRec*
 
 Message:	imGetIndColorSpace - as of Pr 13.0
 Required:	Yes, if the Importer reported that it can provide media color space 
 			definition to the host.
			Called to enumerate the media color spaces.
			If the support is _Fixed, we will iterate until we reach an error return. 
			We will default to space at index 0.
			At present, importer is expected to report a single media color space 
			but we kept the enumerated set-up that was used for imGetIndColorProfile to 
			preserve flexibility.
 
			We maintain old imIndColorProfile but color managed importers should use 
			new selector.
 
			imInColorSpaceRec contains all the substructures to describe media space
			based on different color space types. Importer should report color space for
			its media in outPrSDKColorSpaceType and fill in the relevant substructure.
			If media space is embedded ICC profile or LUT, importer will be called twice
			similarly to imGetIndColorProfile.
			The first time it is called, the inDestinationBuffer will be NULL. 
			Fill in the required size for the buffer, set the correct space type, 
			and we'll call back with enough memory. 
			For other types importer will be called once since no dynamic allocation 
			is required.
			param 1: color space to return, 0-n
			param 2: imIndColorSpaceRec*
 
 Message:	imGetDataStreamChunk
 Required:	no
			Called to get mgjson stream in chunks of a particular data stream
			param1 : imFileRef*
			param2 : imGetDataStreamChunkRec*
 
 Message:	imGetDataFileInfo
 Required:	no
 			Called to get basic information about the data file.
 			If some failure occurs, outErrorMsg will hold error message and outErrorId will hold error id.
 			param1 : imFileRef*
 			param2 : imGetDataFileInfoRec*
 
 Message:	imGetDataHierarchy
 Required:	no
 			Called to get path of data hierarchy. If inBaseHierarchyPath is null, paths under
 			root/base hierarchy will return, otherwise paths under inBaseHierarchyPath will return.
 			Each stream in identified by these unique paths.
 			These paths will be used for getting further information about data streams.
 			If some failure occurs, outErrorMsg will hold error message and outErrorId will hold error id.
 			param1 : imFileRef*
 			param2 : imGetDataHierarchyRec*
 
 Message:	imGetStreamInfo
 Required:	no
 			Called to get additional information about data streams.
 			inStreamPath holds the path of stream whose information is required.
 			Streams can be of type - Group, Static and Dynamic.
 			Group stream contains streams of other type.
 			Static streams holds non temporal data.
 			Dynamic streams holds temporal data. Sample id is associated with each dynamic stream.
 			This id will be used to fetch temporal samples of that dynamic stream.
 			Properties about stream data also attached with each stream.
 			If some failure occurs, outErrorMsg will hold error message and outErrorId will hold error id.
 			param1 : imFileRef*
 			param2 : imGetDataStreamInfoRec*
 
 Message:	imGetStreamTemporalSamples
 Required:	no
 			Called to get temporal samples of dynamic stream with sample id represented by inSampleSetId.
 			Temporal samples will be fetched in between inStartTime and inEndTime.
 			if inAdjacentSampleRequired is set to true, adjacent samples will be fetched using only
 			inStartTime.
 			outSampleTimeBuffer will hold time values and outValueBuffer will hold sample values associated with
 			corresponding outSampleTimeBuffer index.
 			type of sample values can be obtained using outValueType and total fetched samples can be obtained
 			using outSampleCount.
 			If some failure occurs, outErrorMsg will hold error message and outErrorId will hold error id.
 			param1 : imFileRef*
 			param2 : imGetStreamTemporalSamplesRec*
 
 Message:	imGetDataStreamVideoSyncTime
 Required:	no
 			Called to get syncing information of data file with the video file.
 			If some failure occurs, outErrorMsg will hold error message and outErrorId will hold error id.
 			param1 : imFileRef*
			param2 : imGetDataStreamVideoSyncTimeRec*

Message:	imGetCurrentSystemState
Required:	no
			This selector is static (no instance data). It is called to obtain the state of the computer 
			system for the current runtime session. The values returned in response to imGetIndPixelFormat, 
			imGetInfo9, etc. may depend on the current state of the system. For example, a minimum 
			operating system version may be required to enable some variation of behavior, or a codec or 
			hardware acceleration option may need to be installed or configured in a particular way for a 
			pixel format to be supported. For each media file, the host will usually cache the importerﾒs 
			responses to imGetInfo9 and other format queries and use those values across runtime sessions.
			But if the hardware configuration changes between runtime sessions, the host needs to know when 
			to discard the cached values and query the importer for updated information.  The system state 
			flags can be used to do this.  The assignment and meaning of the flags is determined solely by 
			the importer.  For example, suppose media file A has behavior that depends on whether a minimum 
			operating system version is installed, and media file B has behavior that depends on both the 
			operating system version and the presence of a hardware accelerator, while media file C behaves 
			the same in all cases. The importer can assign flag bit 0 the meaning that the minimum operating 
			system version is installed (value 1 if installed), and assign flag bit 1 the meaning that the 
			hardware accelerator is installed (value 1 if installed). So a system state value of 0x2 means 
			the hardware accelerator is installed but the OS is of an older version, while a system state 
			value of 0x3 means the features requiring both the OS version and hardware acceleration are 
			enabled. When imGetInfo9 is called for file A, the importer should set imFileInfoRec9::
			systemStateFlagMask to 0x1 (the OS version is a dependency), for file B, imFileInfoRec9::
			systemStateFlagMask should be set to 0x3 (OS and hardware accelerator are both dependencies), 
			and for file C 0x0 (no system state dependency). The imFileInfoRec9::systemStateFlagsCurrent 
			should always be set to the value that represents the state of the computer system at the time 
			imGetInfo9 is called, although any bits in systemStateFlagsCurrent will be ignored by the host 
			unless there is a 1 in the corresponding bit of systemStateFlagMask.  This means, for example, 
			that systemStateFlagsCurrent will be entirely ignored for file C, because its mask value is 0 
			(no dependency on system state at all). When the host is validating the cache state for a 
			particular media file, it will call the importer using imGetCurrentSystemState, passing the 
			media fileﾒs flag mask in imCurrentSystemStateRec::inSystemStateMask, and it will compare the 
			corresponding bits from imCurrentSystemStateRec::outCurrentSystemState against the cached flags 
			previously reported in imFileInfoRec9::systemStateFlagsCurrent. If there is a mismatch, the 
			file will be reopened for a fresh query using imGetInfo9, etc. The importer may choose to ignore 
			inSystemStateMask and always respond with the complete system state value in systemStateFlagsCurrent, 
			but if it is expensive to check the state of some bits, the importer will wish to examine the 
			systemStateFlagMask bits in which the host is interested and only initiate a query of the 
			associated system state when necessary (for example, checking for the presence of hardware
			acceleration only when or if required). In all cases, the importer should cache and not 
			continually recompute the system state because the host could call imCurrentSystemStateRec 
			many times per second as different media files are accessed. The system state value should 
			never change during one runtime session, but only between different runtime sessions.

			If an importer wants to invalidate the cache for only certain codec subtypes, it can simply 
			increment the version number reported in imCurrentSystemStateRec::outSubtypeVersion for 
			that subtype and of course set imFileInfoRec9::systemStateSubtypeVersion to that same value 
			for newly opened media of that subtype (imFileInfoRec9::info::vidInfo::subType).  Be sure that 
			the same version number is set both in imFileInfoRec9::systemStateSubtypeVersion and 
			imCurrentSystemStateRec::outSubtypeVersion or caching will never be enabled for media using
			that codec subtype, causing a severe performance penalty.

			Note that the systemState and SubtypeVersion values set in imFileInfoRec9 are only effective 
			when imFileInfoRec9::info::streamIdx == 0.  Values provided for any additional streams are ignored.

			param1 : imCurrentSystemStateRec*
			param2 : unused
 
Message:	imGetInfo9
Required:	yes (one of imGetInfo8 or imGetInfo9 must be supported)
			Called when a specific file is instantiated. Importer checks file validity,
			Optionally allocates file instance data, and fills in an imFileInfoRec9.
			parm1 : imFileAccessRec8*
			parm2 : imFileInfoRec9*

Message:	imGetEmbeddedLUT
Required:	Yes, if the Importer reported that it has embedded LUT.
			imIndEmbeddedLUTRec contains all the substructures to describe embedded LUT input color space,
			embedded LUT output color space and embedded LUT blob.
			The first time it is called, the inDestinationBuffer will be NULL.
			Fill in the required size for the buffer, set the correct space type, and we'll call back with enough memory.
			param 1: Embedded LUT profile to return, 0-n
			param 2: imIndEmbeddedLUTRec*
*/

#ifdef __cplusplus
}

// Returns kPrTrue if an importer return value actually indicates an error
inline prBool PrImporterReturnValueIsError(PrImporterReturnValue inValue)
{
	switch ((int)inValue)
	{
	case imNoErr:
	case imIterateFrameSizes:
	case imMediaPending:
	case imNoCaptions:
	case imDelegate:
	case imCancel:
	case imSetFile:
	case imNoTimecode:
	case imIterateStreams:
	case imBadStreamIndex:
	case imBadFormatIndex:
	case imIsCacheable:
	case imUnsupported:
	case imFrameNotFound:
	case imFileOpenFailed:
	case malSupports7:
	case malSupports8:
	case imCantTrim:
	case imSaveErr:
	case imDeleteErr:
	case imNotFoundErr:
	case malSupportsPerInstancePrefs:
		return kPrFalse;
	default:
		return kPrTrue;
	}
}
#endif

#pragma pack(pop)

#endif /* PRSDKIMPORT_H */
