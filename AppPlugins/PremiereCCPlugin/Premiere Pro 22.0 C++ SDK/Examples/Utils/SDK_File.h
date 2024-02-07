/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 1999-2008 Adobe Systems Incorporated                  */
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
//
//		SDK_File.h - Used to read/write a custom file format
//		.sdk files are simple media files consisting of a header,
//		followed by video, then audio, both optional.

#ifndef SDKFILE_H
#define SDKFILE_H

#include	"PrSDKStructs.h"
#include	"PrSDKImport.h"
#include	"PrSDKExport.h"
#include	"PrSDKExportFileSuite.h"
#include	"PrSDKExportInfoSuite.h"
#include	"PrSDKExportParamSuite.h"
#include	"PrSDKExportProgressSuite.h"
#include	"PrSDKErrorSuite.h"
#include	"PrSDKMALErrors.h"
#include	"PrSDKMarkerSuite.h"
#include	"PrSDKSequenceRenderSuite.h"
#include	"PrSDKSequenceAudioSuite.h"
#include	"PrSDKClipRenderSuite.h"
#include	"PrSDKPPixCreatorSuite.h"
#include	"PrSDKPPixCacheSuite.h"
#include	"PrSDKMemoryManagerSuite.h"
#include	"PrSDKWindowSuite.h"
#include    "PrSDKApplicationSettingsSuite.h"
#include	"SDK_Segment_Utils.h"
#ifdef		PRMAC_ENV
    #include	<wchar.h>
    #include    <Carbon/Carbon.h>
#endif

#include	<stdio.h>
#include	<stdlib.h>

#define SDK_FILE_CURRENT_VERSION	5			// The current file version number. When making a change
												// to the file structure, increment this value.
#define SDK_FILETYPE				'SDK_'		// The four character code for our filetype
#define SDK_FILE_EXTENSION			L"sdk"

// Display strings for menus
#define SDK_FILE_NAME				L"SDK File"
#define SDK_8_BIT_RGB_NAME			L"Uncompressed 8-bit RGB"
#define SDK_10_BIT_YUV_NAME			L"Uncompressed 10-bit YUV (v410)"
#define	SDK_RLE_NAME				L"RLE Compressed 8-bit RGB"

#define	SDK_NAME					"PREMIERE SDK_FILE "	// This string is used in the file header
#define	SDK_CLSS					'DTEK'		// ClassID four character code, used in the Editing Mode XML

// Codec (subtype) fourCCs
#define SDK_8_BIT_RGB				'RAW '
#define SDK_10_BIT_YUV				'10yu'
#define SDK_RLE						'RLE_'

#define	AUDIO_SAMPLE_SIZE			4			// Currently, only 32-bit float is supported
#define	PLUS_LINE					"++++"		// Buffer for padding between video frames
#define	PLUS_LINE_LENGTH			4


///////////////////////////////////////////////////////////////////////////////
// SDK header structure
#pragma pack(1)
typedef struct {
	char				name[18];		// SDK_NAME (see above)
	csSDK_int32			version;		// File version #
	prBool				hasAudio;
	PrFourCC			audioSubtype;
	prBool				hasVideo;
	PrFourCC			videoSubtype;	// SDK_8_BIT_RGB, SDK_10_BIT_YUV, or SDK_RLE (not implemented)
										// NOTE: for demo purposes - the compiler does NOT compress audio
	PrPixelFormat		pixelFormat;
	csSDK_int32			depth;			// Bit depth of video
	csSDK_int32			width;
	csSDK_int32			height;
	csSDK_uint32		numFrames;		// Number of video frames
	PrAudioSample		numSampleFrames;// Number of audio sample frames. Note that audio samples may extend
										// past the last video frame.
	csSDK_int32			channelType;	// Audio channel type; uses same enum as PrSDKAudioSuite.h
										// - kAudioChannelType_Mono, kAudioChannelType_Stereo, or kAudioChannelType_51
	PrTime				frameRate;
	double				sampleRate;		// Can be any sample rate supported by Premiere
	csSDK_int32			fieldType;		// Uses same enum as compiler API
										// - compFieldsNone, compFieldsUpperFirst, or compFieldsLowerFirst
	csSDK_uint32		pixelAspectNum;	// Numerator of pixel aspect ratio
	csSDK_uint32		pixelAspectDen; // Denominator of pixel aspect ratio
	char				orgtime[18];	// These fields map directly to those in imTimeInfoRec.
	char				alttime[18];
	char				orgreel[40];
	char				altreel[40];
	char				logcomment[256];
	csSDK_int32			magic;
	csSDK_int32			unused[32];		// For future expansion
} SDK_File, *SDK_FileP, **SDK_FileH;
#pragma pack()


///////////////////////////////////////////////////////////////////////////////
// Importer local data structure, defined here for convenience,
// and shared between the various importer SDK samples

typedef struct
{	
	SDK_File				theFile;
	PrAudioSample			audioPosition;
	// fileName is used as an example of saving a filename in private data,
	// and could be used if a clip has child files that Premiere doesn't know about
	prUTF16Char				fileName[256];
	imFileRef				fileRef;
	#ifdef PRWIN_ENV
	HANDLE					ioCompletionPort;
	#endif
	PlugMemoryFuncsPtr		memFuncs;
	csSDK_int32				importerID;
	SPBasicSuite			*BasicSuite;
	PrSDKPPixCreatorSuite	*PPixCreatorSuite;
	PrSDKPPixCacheSuite		*PPixCacheSuite;
	PrSDKPPixSuite			*PPixSuite;
	PrSDKTimeSuite			*TimeSuite;
} ImporterLocalRec8, *ImporterLocalRec8Ptr, **ImporterLocalRec8H;


///////////////////////////////////////////////////////////////////////////////
// Exporter local data structure, defined here for convenience
typedef struct ExportSettings
{
	SDK_File					  SDKFileRec;	// The struct of the file header
	VideoSequenceParser			  *videoSequenceParser;
	SPBasicSuite				  *spBasic;
	PrSDKExportParamSuite		  *exportParamSuite;
	PrSDKExportProgressSuite	  *exportProgressSuite;
	PrSDKExportInfoSuite		  *exportInfoSuite;
	PrSDKExportFileSuite		  *exportFileSuite;
	PrSDKErrorSuite3			  *errorSuite;
	PrSDKClipRenderSuite		  *clipRenderSuite;
	PrSDKMarkerSuite			  *markerSuite;
	PrSDKPPixSuite				  *ppixSuite;
	PrSDKTimeSuite				  *timeSuite;
	PrSDKMemoryManagerSuite		  *memorySuite;
	PrSDKSequenceAudioSuite1	  *sequenceAudioSuite;
	PrSDKSequenceRenderSuite	  *sequenceRenderSuite;
	PrSDKWindowSuite			  *windowSuite;
	csSDK_uint32				  videoRenderID;
	prFieldType					  sourceFieldType;
	PrSDKApplicationSettingsSuite *appSettingsSuite;
} ExportSettings;


// Set during exSelParamButton, and passed during exSelExport
typedef struct CodecSettings
{
	prBool		sampleSetting;	// Sample setting to demonstrate how to set and get custom settings
} CodecSettings;


///////////////////////////////////////////////////////////////////////////////
// Video import-related calls
#ifdef PRWIN_ENV
unsigned char ReadSDKFileAsync(	imFileRef	SDKfileRef, 
								csSDK_int32	frameBytes, 
								csSDK_int32	theFrame, 
								char		*inFrameBuffer,
								OVERLAPPED	*overlapped);
#endif

unsigned char ReadSDK_File(	imFileRef	SDKfileRef, 
							csSDK_int32	frameBytes, 
							csSDK_int32	theFrame, 
							char		*inFrameBuffer);

void AddRowPadding(	char			*srcFrameZ,
					char			*dstFrameZ, 
					csSDK_uint32	rowBytesL, 
					csSDK_uint32	pixelSize,
					csSDK_uint32	widthL, 
					csSDK_uint32	heightL);

void ScaleAndBltFrame(imStdParms		*stdParms,
					  SDK_File			fileHeader,
					  csSDK_uint32		frameBytes,
					  char				*inFrameBuffer, 
					  imImportImageRec	*imageRec);

csSDK_int32 GetSrcPix(	char			*inFrameBuffer,
						SDK_File		fileHeader,
						csSDK_uint32	dstCoorW, 
						csSDK_uint32	dstCoorH, 
						float		ratioW, 
						float		ratioH);

int prWriteFile (imFileRef			refNum, 
				 const void			*data, 
				 csSDK_uint32		*bytes);
/*
void WriteRLE	(long			*src, 
				 compFileRef	ref, 
				 long			totalPix);
*/


///////////////////////////////////////////////////////////////////////////////
// Audio import-related calls
void calculateAudioRequest(
	imImportAudioRec7	*audioRec7,
	const PrAudioSample	totalSampleFrames,
	PrAudioSample		*savedAudioPosition,
	PrAudioSample		*startAudioPosition,
	PrAudioSample		*numAudioFrames);

void setPointerToAudioStart(
	ImporterLocalRec8H		ldataH,
	const PrAudioSample		startAudioPosition,
	imFileRef				SDKfileRef);

prMALError readAudioToBuffer (	const PrAudioSample	numAudioFrames,
								const PrAudioSample	totalSampleFrames,
								const csSDK_int32	numAudioChannels,
								imFileRef			SDKfileRef,
								float **			audioBuffer);


///////////////////////////////////////////////////////////////////////////////
// Export-related calls
prMALError WriteSDK_FileHeader(	exportStdParms	*stdParms,
								exDoExportRec	*exportInfoP, 
								PrTime			exportDuration);

prMALError RenderAndWriteVideoFrame(
	const PrTime				videoTime,
	exDoExportRec				*exportInfoP);

void RemoveRowPadding(	char		*srcFrameZ,
						char		*dstFrameZ, 
						csSDK_int32 rowBytesL,
						csSDK_int32 pixelSize,
						csSDK_int32 widthL, 
						csSDK_int32 heightL);

prMALError RenderAndWriteAllAudio(
	exDoExportRec				*exportInfoP,
	PrTime						exportDuration);

void WriteMarkerAndProjectDataToFile(
	exportStdParms		*stdParms, 
	exDoExportRec		*exportInfoP);


///////////////////////////////////////////////////////////////////////////////
// Miscellaneous helper funcs
csSDK_int32 GetNumberOfAudioChannels(csSDK_int32 audioChannelType);

csSDK_int32 GetPixelFormatSize(PrFourCC subtype);
csSDK_int32 GetPixelFormatSize(PrPixelFormat pixelFormat);

void ConvertFrom8uTo32f(char *buffer8u, char *buffer32f, csSDK_int32 width, csSDK_int32 height);
void ConvertFromBGRA32fToVUYA32f(char *buffer32f, csSDK_int32 width, csSDK_int32 height);
void ConvertFrom32fToV410(char *buffer32f, char *bufferV210, csSDK_int32 width, csSDK_int32 height);
void ConvertFromV410To32f(char *bufferV210, char *buffer32f, csSDK_int32 width, csSDK_int32 height);

void ConvertPrTimeToScaleSampleSize(
	PrSDKTimeSuite	*timeSuite,
	PrTime			prTime,
	csSDK_int32		*scale,
	csSDK_int32		*sampleSize);

void ConvertScaleSampleSizeToPrTime(
	PrSDKTimeSuite	*timeSuite,
	csSDK_int32		*scale,
	csSDK_int32		*sampleSize,
	PrTime			*prTime);
	
void copyConvertStringLiteralIntoUTF16(const wchar_t* inputString, prUTF16Char* destination);

void safeStrCpy (char *destStr, int size, const char *srcStr);
void safeWcscat (wchar_t *destStr, int size, const wchar_t *srcStr);

//	This format does not support audio interleaving or "smart" RLE encoding.

//	Frames end with \n\n\n\n, audio "blips" start with ++++
//	So, read until you get to the first \n, that will be the total amount
//	of bytes for the first frame.  Likewise for a "frame" or "blip" of audio.

//	RLE Compression is done on a pixel repeat per frame basis.  
//		So if a row has all one color such as 
//		[fa,fb,fc,00], it would look like 
//		(num_repeat)(fafbfc00)
//		where num_repeat is the number of pixels from 1 to totalPixels (pixWidth * pixHeight)

// Node struct, used in RLE compression 
typedef struct Node{
	csSDK_int32	count;
	csSDK_int32	pixel;
} Node;

#endif
