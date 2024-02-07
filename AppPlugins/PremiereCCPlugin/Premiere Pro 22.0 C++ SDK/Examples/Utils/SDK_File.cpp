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
//		SDK_File.cpp - Used to read/write the SDK file format.
//		
//		Description: SDK Files can contain audio and video
//		In addition they can be RLE compressed
//
//		Purpose: This "artificial" file format is used as
//		a part of the SDK "Media Abstraction Layer".  By writing
//		an import and compile module, and optionally a
//		real-time playback module and assigning a Filetype, 
//		Subtype, and a unique ClassID, your plug-ins can 
//		work cooperatively with one another.
//
//		Created by Adobe Developer Technologies for 
//		Adobe Systems, Inc.
//		Part of the Adobe Premiere Pro SDK
//		_______________________________________
//		Version 1.0 - eks
//				1.1	- bbb -	(.c -> .cpp)
//				1.2 - zal - Fixed file write, work area export
//				1.3 - zal - Added support for arbitrary audio sample rates, multi-channel audio,
//							pixel aspect ratio, and fields
//				1.4 - zal - Support for 24-bit video (no alpha channel), versioning
//				2.0 - zal - Generic routines for rendering and writing video, audio, and markers
//				2.5 - zal - High-bit video support (v410)
//              3.0 - zal - MacOS support
//              3.1 - zal - Respect rowbytes
//              4.0 - zal - Porting to exporter API
//              4.1 - zal - Support async IO on Win
//              4.2 - zal - Refactor audio import code
//              4.3 - zal - Switch to more modern marker suite
//              4.4 - zal - Support various audio channel types
//              5.0 - zal - 64-bit support
//              5.1 - zal - When export cancelled, generate a well-formed file
//              5.2 - zal - Fix alpha channel round-tripping
//              5.3 - zal - Fix integer overflow problems


#include "SDK_File.h"

prMALError WriteSDK_FileHeader(exportStdParms *stdParms, exDoExportRec *exportInfoP, PrTime exportDuration)
{
	prMALError				result				= malNoError;
	csSDK_uint32			bytesToWriteLu		= 0;
	csSDK_uint32			exID				= exportInfoP->exporterPluginID;
	exParamValues			codecSubType,
							height,
							width,
							ticksPerFrame,
							fieldType,
							pixelAspectRatio,
							sampleRate,
							channelType;
	PrTime					ticksPerSample;
	ExportSettings			*mySettings;

	// Update the private data with the parameter settings
	mySettings = reinterpret_cast<ExportSettings *>(exportInfoP->privateData);

	#ifdef PRWIN_ENV
	strcpy_s(mySettings->SDKFileRec.name, sizeof(SDK_NAME), SDK_NAME);
	#else
	strcpy(mySettings->SDKFileRec.name, SDK_NAME);
	#endif
	mySettings->SDKFileRec.version = SDK_FILE_CURRENT_VERSION;

	if(exportInfoP->exportVideo)
	{
		mySettings->SDKFileRec.hasVideo = kPrTrue;
		mySettings->exportParamSuite->GetParamValue(exID, 0, ADBEVideoCodec, &codecSubType);
		mySettings->SDKFileRec.videoSubtype = codecSubType.value.intValue;

		mySettings->SDKFileRec.pixelFormat = (PrPixelFormat)0;
		switch(codecSubType.value.intValue)
		{
			case SDK_8_BIT_RGB:
				mySettings->SDKFileRec.depth = 32;
				break;

			case SDK_10_BIT_YUV:
				mySettings->SDKFileRec.depth = 24;
				break;
		}

		mySettings->exportParamSuite->GetParamValue(exID, 0, ADBEVideoWidth, &width);
		mySettings->SDKFileRec.width	= width.value.intValue;
		mySettings->exportParamSuite->GetParamValue(exID, 0, ADBEVideoHeight, &height);
		mySettings->SDKFileRec.height = height.value.intValue;
		mySettings->exportParamSuite->GetParamValue(exID, 0, ADBEVideoFPS, &ticksPerFrame);
		mySettings->SDKFileRec.numFrames		=
			static_cast<csSDK_uint32>(exportDuration / ticksPerFrame.value.timeValue);	// Zero based.
		mySettings->SDKFileRec.frameRate		= ticksPerFrame.value.timeValue;

		mySettings->exportParamSuite->GetParamValue(exID, 0, ADBEVideoFieldType, &fieldType);
		mySettings->SDKFileRec.fieldType		= fieldType.value.intValue;
		mySettings->exportParamSuite->GetParamValue(exID, 0, ADBEVideoAspect, &pixelAspectRatio);
		mySettings->SDKFileRec.pixelAspectNum	= pixelAspectRatio.value.ratioValue.numerator;
		mySettings->SDKFileRec.pixelAspectDen	= pixelAspectRatio.value.ratioValue.denominator;
	}
	else
	{
		mySettings->SDKFileRec.hasVideo = kPrFalse; // No video in file
	}

	if(exportInfoP->exportAudio)
	{
		mySettings->SDKFileRec.hasAudio = kPrTrue;
		mySettings->SDKFileRec.audioSubtype = 'RAW_'; //exportInfoP->outputRec.audCompression.subtype;

		// Calculate audio samples
		mySettings->exportParamSuite->GetParamValue(exID, 0, ADBEAudioRatePerSecond, &sampleRate);
		mySettings->SDKFileRec.sampleRate = sampleRate.value.floatValue;
		mySettings->timeSuite->GetTicksPerAudioSample ((float)sampleRate.value.floatValue, &ticksPerSample);
		mySettings->SDKFileRec.numSampleFrames = exportDuration / ticksPerSample;
		mySettings->exportParamSuite->GetParamValue(exID, 0, ADBEAudioNumChannels, &channelType);
		mySettings->SDKFileRec.channelType = channelType.value.intValue;
	}
	else
	{
		mySettings->SDKFileRec.hasAudio = kPrFalse;				// No audio in file
	}

	// Simple validation
	if (mySettings->SDKFileRec.hasVideo && (!mySettings->SDKFileRec.width || !mySettings->SDKFileRec.height))
	{
		result = exportReturn_ErrOther;
	}
	else
	{
		// Write out the header
		bytesToWriteLu = sizeof(SDK_File);

		exportInfoP->privateData = reinterpret_cast<void*>(mySettings);
		
		// Seek to beginning of file
		prInt64 newPosition;
		mySettings->exportFileSuite->Seek(exportInfoP->fileObject,
											0,
											newPosition,
											fileSeekMode_Begin);
		mySettings->exportFileSuite->Write(	exportInfoP->fileObject,
											reinterpret_cast<void*>(&mySettings->SDKFileRec),
											bytesToWriteLu);
	}

	return result;
}


//////////////////////////////////////////////////////////////////
//
//	prWriteFile - File write.
//

int 
prWriteFile (
	imFileRef		refNum, 
	const void 		*data, 
	csSDK_uint32	*bytes)
{
	csSDK_uint32		orig_bytes = *bytes;
	csSDK_int32		err;
	#ifdef PRWIN_ENV
	err = !WriteFile(refNum, data, (DWORD)*bytes, (LPDWORD)bytes, NULL);
	if (err)
	{
		err = GetLastError ();
	}
	#else
	err = FSWriteFork(	reinterpret_cast<intptr_t>(refNum),
						fsAtMark,
						0,
						orig_bytes,
						data,
						reinterpret_cast<ByteCount*>(bytes));
	#endif
	
	if (!err && (*bytes != orig_bytes)) {
		err = exportReturn_OutOfDiskSpace;
	}
	return err;
}

//////////////////////////////////////////////////////////////////
//
//	WriteRLE - Simple RLE function, frame by frame.
//
//	RLE Compress the data in the frame, then write to file
//
/*
void WriteRLE(long *src, compFileRef ref, long totalPix)
{
	long			totalNodes 	= 1;
	unsigned long	in_bytes = 0;		
	Node *  nodes = (Node*)malloc((sizeof(Node) * totalPix));	// Create Nodes
	register Node * N = nodes;
	register long * s1 = src;
	register long * s2 = src;
	
	++s2;				

	if (!N)				// Make sure I've got the memory...
		return;
	
	N->count = 1;		// initialize the first element in our node
	N->pixel = *src;	// get the value of the first pixel from src

	while (--totalPix)
	{	
		// Look at the source buffer, compare to next. pixel

		if (*s1++ == *s2++)
		{
			// If the current and next pixel values match,
			// increment the count of the already stored
			// pixel.
			++(N->count);
		}
		else
		{
			//	If the current and next pixel don't match
			//	Increment the node position, start a new node count
			//	assign the value of the current pixel and increment
			//	the total number of nodes created.
			++N;
			N->count = 1;
			N->pixel = *s1;
			++totalNodes;
		}
	}
	in_bytes = totalNodes * sizeof(Node);
	prWriteFile(ref,
				nodes,
				&in_bytes);

	free(nodes);
	
}
*/

//////////////////////////////////////////////////////////////////
//
//	ReadSDKFileAsync - Read theFrame from the indicated SDK file,
//	Returns a populated inFrameBuffer. 
//	Only for uncompressed SDK files
//

#ifdef PRWIN_ENV
unsigned char ReadSDKFileAsync(
	imFileRef	SDKfileRef, 
	csSDK_int32	frameBytes, 
	csSDK_int32	theFrame, 
	char		*inFrameBuffer,
	OVERLAPPED	*overlapped)
{
	prBool			didRead		= 0;
	DWORD			lastError	= 0;
	csSDK_uint64	offset		= sizeof(SDK_File);
	csSDK_uint32	bytesRead	= 0;

	offset += (theFrame * static_cast<csSDK_uint64>(frameBytes));
	if( theFrame > 0)
	{	
		// take the extra 4 bytes (\n\n\n\n) at the end of each frame into account
		offset += (4 * (theFrame));
	}

	overlapped->Offset = offset & 0xffffffff;
	overlapped->OffsetHigh = offset >> 32;
	didRead = ReadFile(	SDKfileRef,
						inFrameBuffer,
						frameBytes,
						NULL,
						overlapped);

	return imNoErr;
}
#endif


//////////////////////////////////////////////////////////////////
//
//	ReadSDK_File - Read theFrame from the indicated SDK file,
//	Returns a populated inFrameBuffer. 
//	Only for uncompressed SDK files
//

unsigned char ReadSDK_File(	imFileRef		SDKfileRef, 
							csSDK_int32		frameBytes, 
							csSDK_int32		theFrame, 
							char			*inFrameBuffer)
{
	csSDK_uint64	offset		= sizeof(SDK_File);
	csSDK_uint32	bytesRead	= 0;

	#ifdef PRWIN_ENV
	LARGE_INTEGER	offsetLI;
	char			didRead		= 0;
	#else
	OSErr			returnValue = 0;
	#endif

	offset += (theFrame * static_cast<csSDK_uint64>(frameBytes));
	if( theFrame > 0)
	{	
		// Take the extra bytes at the end of each frame into account
		offset += (PLUS_LINE_LENGTH * (theFrame));
	}
	
	#ifdef PRWIN_ENV
	offsetLI.QuadPart = offset;
	SetFilePointerEx(SDKfileRef, offsetLI, NULL, FILE_BEGIN);
	didRead = ReadFile(	SDKfileRef,
						inFrameBuffer,
						frameBytes,
						reinterpret_cast<LPDWORD>(&bytesRead),
						NULL);
	if(!didRead)
	{
		return imBadFile;
	}						
	#else
	returnValue = FSReadFork(	reinterpret_cast<intptr_t>(SDKfileRef),
								fsFromStart,
								offset,
								frameBytes,
								inFrameBuffer,
								reinterpret_cast<ByteCount*>(&bytesRead));
	if(returnValue)
	{
		return imBadFile;
	}							
	#endif

	return imNoErr;
}


//////////////////////////////////////////////////////////////////
//
//	ScaleAndBltFrame - Scaling Function
//		
//	Designed to work with SDK format files, modify for your own importer needs
//		

void ScaleAndBltFrame(imStdParms		*stdParms,
					  SDK_File			fileHeader,
					  csSDK_uint32		frameBytes,
					  char				*inFrameBuffer, 
					  imImportImageRec	*imageRec)
{
	// original source and dest in pixels
	csSDK_int32	srcWidth, srcHeight, dstWidth, dstHeight, dstCoorW,dstCoorH;
	float		ratioW, ratioH;
	char		paddingBytes = 0;
	
	char		*tempPix;
	csSDK_int32	*dstPix = (csSDK_int32*)imageRec->pix; 
	
	paddingBytes = (imageRec->rowbytes) - (imageRec->dstWidth * 4); 

	srcWidth	= fileHeader.width;;
	srcHeight	= fileHeader.height;
	dstWidth	= imageRec->dstWidth;
	dstHeight	= imageRec->dstHeight;

	// coordinate numbers, ratios
	
	ratioW		= (float)srcWidth / (float)dstWidth;
	ratioH		= (float)srcHeight / (float)dstHeight;
	
	// loop through the destination coordinate grid, find the "virtual" pixel in source grid

	for(dstCoorH = 0; dstCoorH < dstHeight; ++dstCoorH)
	{
		for(dstCoorW = 0; dstCoorW < dstWidth; ++dstCoorW)
		{
			*dstPix = GetSrcPix(inFrameBuffer, fileHeader,dstCoorW,dstCoorH,ratioW,ratioH);
			++dstPix;
		}
		// add the padding bytes to the dst after it's scaled
		tempPix = (char*)dstPix;
		tempPix += paddingBytes;
		dstPix = (csSDK_int32*)tempPix;
	}
	return;
}

//////////////////////////////////////////////////////////////////
//
//	GetSrcPix - Utility function used by the scaling functions
//	

csSDK_int32 GetSrcPix(char			*inFrameBuffer, 
			   SDK_File		fileHeader,
			   csSDK_uint32	dstCoorW, 
			   csSDK_uint32	dstCoorH, 
			   float		ratioW, 
			   float		ratioH)
{
	csSDK_uint32	w, h;
	csSDK_uint32	*thePixel = reinterpret_cast<csSDK_uint32*>(inFrameBuffer);

	// The translated coordinates

	w = static_cast<csSDK_uint32>(dstCoorW * ratioW);
	h = static_cast<csSDK_uint32>(dstCoorH * ratioH);
	
	thePixel += (h * fileHeader.width) + w; 

	return *thePixel;
}


// Source and destination frames may be the same
void RemoveRowPadding(	char		*srcFrame,
						char		*dstFrame, 
						csSDK_int32 rowBytes, 
						csSDK_int32 pixelSize,
						csSDK_int32 widthL, 
						csSDK_int32 heightL)
{
	csSDK_int32 widthBytes = widthL * pixelSize;

	if (widthBytes < rowBytes)
	{
		for(csSDK_int32 hL = 0; hL < heightL; ++hL)
		{
			memcpy (&dstFrame[hL * widthBytes], &srcFrame[hL * rowBytes], widthBytes);
		}
	}

	return;
}


// Source and destination frames may be the same
void AddRowPadding(	char			*srcFrame,
					char			*dstFrame, 
					csSDK_uint32	rowBytesL, 
					csSDK_uint32	pixelSize,
					csSDK_uint32	widthL, 
					csSDK_uint32	heightL)
{
	csSDK_uint32 widthBytes = widthL * pixelSize;

	if (widthBytes < rowBytesL)
	{
		// Expand rows starting from last row, so that we can handle an in-place operation
		for(csSDK_int32 hL = heightL - 1; hL >= 0; --hL)
		{
			memcpy (&dstFrame[hL * rowBytesL], &srcFrame[hL * widthBytes], widthBytes);
		}
	}

	return;
}


// Returns malNoError if successful, or comp_CompileAbort if user aborted
prMALError RenderAndWriteVideoFrame(
	const PrTime				videoTime,
	exDoExportRec				*exportInfoP)
{
	csSDK_int32					resultS					= malNoError;
	csSDK_uint32				exID					= exportInfoP->exporterPluginID;
	ExportSettings				*mySettings = reinterpret_cast<ExportSettings *>(exportInfoP->privateData);
	csSDK_int32					rowbytes				= 0;
	csSDK_int32					renderedPixelSize		= 0;
	exParamValues				width,
								height,
								pixelAspectRatio,
								fieldType,
								codecSubType;
	PrPixelFormat				renderedPixelFormat;
	csSDK_uint32				bytesToWriteLu			= 0;
	char						*frameBufferP			= NULL,
								*f32BufferP				= NULL,
								*frameNoPaddingP		= NULL,
								*v410Buffer				= NULL;
	SequenceRender_ParamsRec renderParms;
	PrPixelFormat pixelFormats[] = {PrPixelFormat_BGRA_4444_8u, PrPixelFormat_BGRA_4444_8u};

	renderParms.inRequestedPixelFormatArray = pixelFormats;
	renderParms.inRequestedPixelFormatArrayCount = 1;

	mySettings->exportParamSuite->GetParamValue(exID, 0, ADBEVideoWidth, &width);
	renderParms.inWidth = width.value.intValue;
	mySettings->exportParamSuite->GetParamValue(exID, 0, ADBEVideoHeight, &height);
	renderParms.inHeight = height.value.intValue;
	mySettings->exportParamSuite->GetParamValue(exID, 0, ADBEVideoAspect, &pixelAspectRatio);
	renderParms.inPixelAspectRatioNumerator = pixelAspectRatio.value.ratioValue.numerator;
	renderParms.inPixelAspectRatioDenominator = pixelAspectRatio.value.ratioValue.denominator;

	renderParms.inRenderQuality = kPrRenderQuality_High;
	mySettings->exportParamSuite->GetParamValue(exID, 0, ADBEVideoFieldType, &fieldType);
	renderParms.inFieldType = fieldType.value.intValue;
	// By setting this to false, we basically leave deinterlacing up to the host logic
	// We could set it to true if we wanted to force deinterlacing
	renderParms.inDeinterlace = kPrFalse;
	renderParms.inDeinterlaceQuality = kPrRenderQuality_High;

	mySettings->exportParamSuite->GetParamValue(exID, 0, ADBEVideoCodec, &codecSubType);

	switch(codecSubType.value.intValue)
	{
		case SDK_8_BIT_RGB:
			renderParms.inCompositeOnBlack = kPrFalse;
			break;

		case SDK_10_BIT_YUV:
			renderParms.inCompositeOnBlack = kPrTrue;
			renderParms.inRequestedPixelFormatArrayCount = 2;
			pixelFormats[0] = PrPixelFormat_VUYA_4444_32f;
			break;
	}

	SequenceRender_GetFrameReturnRec renderResult;

	resultS = mySettings->sequenceRenderSuite->RenderVideoFrame(mySettings->videoRenderID,
																videoTime,
																&renderParms,
																kRenderCacheType_None,	// [TODO] Try different settings
																&renderResult);
	mySettings->ppixSuite->GetPixels(	renderResult.outFrame,
										PrPPixBufferAccess_ReadOnly,
										&frameBufferP);
	mySettings->ppixSuite->GetRowBytes(renderResult.outFrame, &rowbytes);
	mySettings->ppixSuite->GetPixelFormat(renderResult.outFrame, &renderedPixelFormat);
	renderedPixelSize = GetPixelFormatSize(renderedPixelFormat);

	// If user hit cancel
	if (resultS == suiteError_CompilerCompileAbort)
	{
		// Just return the abort
	}
	else
	{
		// If there is extra row padding, trim it off.
		// Currently, rows are aligned to 128 bytes for optimizations.
		// So, for example a width of 320 * 4 bytes per pixel = 1280 = 128 * 10 = no padding
		// But a width of 720 * 4 bytes per pixel = 2880 = 128 * 22.5 = padding
		if (rowbytes != width.value.intValue * renderedPixelSize)
		{
	   		frameNoPaddingP = new char[width.value.intValue  * height.value.intValue  * renderedPixelSize];
			RemoveRowPadding(	frameBufferP, 
	  							frameNoPaddingP,
								rowbytes,
								renderedPixelSize,
					   			width.value.intValue, 
						   		height.value.intValue);
			rowbytes		= width.value.intValue * renderedPixelSize;
			frameBufferP	= frameNoPaddingP;
		}

		if (codecSubType.value.intValue == SDK_10_BIT_YUV)
		{
			// Check for returned pixel format and convert if necessary
			if (renderedPixelFormat == PrPixelFormat_BGRA_4444_8u)
			{
	   			f32BufferP = new char[width.value.intValue * height.value.intValue * GetPixelFormatSize(PrPixelFormat_VUYA_4444_32f)];
				ConvertFrom8uTo32f(	frameBufferP,
									f32BufferP,
									width.value.intValue,
									height.value.intValue);
				ConvertFromBGRA32fToVUYA32f(f32BufferP,
											width.value.intValue,
											height.value.intValue);
				frameBufferP = f32BufferP;
			}

			v410Buffer = new char[width.value.intValue * height.value.intValue * GetPixelFormatSize(codecSubType.value.intValue)];
			ConvertFrom32fToV410(	frameBufferP,
									v410Buffer,
									width.value.intValue,
									height.value.intValue);
			rowbytes		= width.value.intValue * GetPixelFormatSize(codecSubType.value.intValue);
			frameBufferP	= v410Buffer;
		}

		// Save the uncompressed frames to disk
		if (codecSubType.value.intValue != SDK_RLE)
		{
			bytesToWriteLu = rowbytes * height.value.intValue;
			mySettings->exportFileSuite->Write(exportInfoP->fileObject,
												frameBufferP,
												bytesToWriteLu);
		}
		/*
		else (codecSubType.value.intValue == SDK_RLE)
		{
			// Currently the SDK import cannot read RLE
			WriteRLE(	reinterpret_cast<long*>(frameBufferP),
						exportInfoP->outputFileRef, 
						(height.value.intValue * width.value.intValue));
		}
		*/
				
		// Write divider between frames
		bytesToWriteLu = PLUS_LINE_LENGTH;
		mySettings->exportFileSuite->Write(exportInfoP->fileObject,
											const_cast<char *>(PLUS_LINE),
											bytesToWriteLu);

		// Now that buffer is written to disk, we can dispose of memory
		mySettings->ppixSuite->Dispose(renderResult.outFrame);
		if (frameNoPaddingP)
		{
			delete(frameNoPaddingP);
			frameNoPaddingP = NULL;
		}
		if (f32BufferP)
		{
			delete(f32BufferP);
			f32BufferP = NULL;
		}
		if (v410Buffer)
		{
			delete(v410Buffer);
			v410Buffer = NULL;
		}
	}

	return resultS;
}


void calculateAudioRequest(
	imImportAudioRec7	*audioRec7,
	const PrAudioSample	totalSampleFrames,
	PrAudioSample		*savedAudioPosition,
	PrAudioSample		*startAudioPosition,
	PrAudioSample		*numAudioFrames)
{
	// If audioRec7->position is less than zero, this means Premiere Pro is requesting
	// contiguous samples from the last call
	if (audioRec7->position < 0)
	{
		*startAudioPosition = *savedAudioPosition;
	}
	else
	{
		*startAudioPosition = audioRec7->position;
	}

	// If amount requested is more than amount left in file
	if (*startAudioPosition + audioRec7->size > totalSampleFrames)
	{
		// Update number of actual audio frames to read
		*numAudioFrames = totalSampleFrames - *startAudioPosition;

        // Save off next audio position, in case another sequential call is made
		*savedAudioPosition = totalSampleFrames;
	}
	else
	{
		// Update number of actual audio frames to read
		*numAudioFrames = audioRec7->size;

        // Save off next audio position, in case another sequential call is made
		*savedAudioPosition = *startAudioPosition + audioRec7->size;
	}

	return;
}


void setPointerToAudioStart(
	ImporterLocalRec8H		ldataH,
	const PrAudioSample		startAudioPosition,
	imFileRef				SDKfileRef)
{
	csSDK_uint32		totalVideoFramesLu	= 0;
	csSDK_int64			bytesPerFrameLL		= 0,
						videoOffsetLL		= 0;
	PrAudioSample		audioOffset			= 0;

	#ifdef PRWIN_ENV
	csSDK_int32			tempErrorS			= 0;
	LARGE_INTEGER		distanceToMoveLI;
	#else
	SInt64				distanceToMove;
	#endif

	totalVideoFramesLu	= (*ldataH)->theFile.numFrames;
	if ((*ldataH)->theFile.hasVideo && totalVideoFramesLu > 0)
	{	
		bytesPerFrameLL		= (*ldataH)->theFile.width * (*ldataH)->theFile.height *
			GetPixelFormatSize((*ldataH)->theFile.videoSubtype);

		videoOffsetLL += totalVideoFramesLu * bytesPerFrameLL;

		// Take the extra bytes at the end of each frame into account
		videoOffsetLL += (PLUS_LINE_LENGTH * (totalVideoFramesLu));
	}

	audioOffset = startAudioPosition * AUDIO_SAMPLE_SIZE;

	#ifdef PRWIN_ENV
	distanceToMoveLI.QuadPart = sizeof(SDK_File) + videoOffsetLL + audioOffset;
	tempErrorS = SetFilePointerEx(SDKfileRef,
								distanceToMoveLI,
								NULL,
								FILE_BEGIN);

	if (tempErrorS == INVALID_SET_FILE_POINTER)
	{
		GetLastError ();
	}
	#else
	distanceToMove = sizeof(SDK_File) + videoOffsetLL + audioOffset;
	FSSetForkPosition (	reinterpret_cast<intptr_t>(SDKfileRef),
						fsFromStart,
						distanceToMove);
	#endif
}


prMALError readAudioToBuffer (	const PrAudioSample	numAudioFrames,
								const PrAudioSample	totalSampleFrames,
								const csSDK_int32	numAudioChannels,
								imFileRef			SDKfileRef,
								float **			audioBuffer)
{
	prMALError			result				= malNoError;
	csSDK_uint32		bytesReadLu			= 0;

	#ifdef PRWIN_ENV
	csSDK_int32			didReadL			= 0;
	csSDK_int32			tempErrorS			= 0;
	LARGE_INTEGER		distanceToMoveLI;
	#else
	SInt64				distanceToMove;
	#endif

	// Read all channels into their respective buffers
	for (csSDK_int32 bufferIndexL = 0; bufferIndexL < numAudioChannels; bufferIndexL++)
	{
		#ifdef PRWIN_ENV
		didReadL = ReadFile(SDKfileRef,
							audioBuffer[bufferIndexL],
							static_cast<csSDK_int32>(numAudioFrames) * AUDIO_SAMPLE_SIZE,
							reinterpret_cast<LPDWORD>(&bytesReadLu),
							NULL);
		if (!didReadL)
		{
			return imBadFile;
		}

		#else
		result = FSReadFork(reinterpret_cast<intptr_t>(SDKfileRef),
							fsAtMark,
							0,
							numAudioFrames * AUDIO_SAMPLE_SIZE,
							audioBuffer[bufferIndexL],
							reinterpret_cast<ByteCount*>(&bytesReadLu));
		if (result)
		{
			return imBadFile;
		}
		#endif

		// Move file pointer to next audio channel
		#ifdef PRWIN_ENV
		distanceToMoveLI.QuadPart = (totalSampleFrames - numAudioFrames) * AUDIO_SAMPLE_SIZE;
		tempErrorS = SetFilePointerEx(	SDKfileRef,
										distanceToMoveLI,
										NULL,
										FILE_CURRENT);

		if (tempErrorS == INVALID_SET_FILE_POINTER)
		{
			GetLastError ();
		}
		#else
		distanceToMove = (totalSampleFrames - numAudioFrames) * AUDIO_SAMPLE_SIZE;
		result = FSSetForkPosition(	reinterpret_cast<intptr_t>(SDKfileRef),
									fsFromMark,
									distanceToMove);
		#endif
	}

	return result;
}


// Returns malNoError if successful
prMALError RenderAndWriteAllAudio(
	exDoExportRec				*exportInfoP,
	PrTime						exportDuration)
{
	csSDK_int32					resultS					= malNoError;
	csSDK_uint32				exID					= exportInfoP->exporterPluginID;
	ExportSettings				*mySettings				= reinterpret_cast<ExportSettings*>(exportInfoP->privateData);
	exParamValues				ticksPerFrame,
								sampleRate,
								channelType;
	csSDK_int64					distanceToMove;
	prInt64						filePtrLocation;
	PrAudioSample				totalAudioSamples		= 0,
								samplesRemaining		= 0;
	csSDK_uint32				audioRenderID			= 0;
	csSDK_int32					maxBlip					= 0;
	csSDK_int32					audioBufferSizeL		= 0,
								samplesRequestedL		= 0,
								audioChannelsL			= 0;
	float *						audioBuffer[6]			= {NULL, NULL, NULL, NULL, NULL, NULL};
	csSDK_uint32				bytesToWriteLu			= 0;

	PrSDKMemoryManagerSuite	*memorySuite	= mySettings->memorySuite;
	PrSDKTimeSuite			*timeSuite		= mySettings->timeSuite;
	PrTime					ticksPerSample	= 0;

	PrSDKExportParamSuite	*paramSuite	= mySettings->exportParamSuite;
	paramSuite->GetParamValue(exID, 0, ADBEVideoFPS, &ticksPerFrame);
	paramSuite->GetParamValue(exID, 0, ADBEAudioRatePerSecond, &sampleRate);
	paramSuite->GetParamValue(exID, 0, ADBEAudioNumChannels, &channelType);
	audioChannelsL = GetNumberOfAudioChannels (channelType.value.intValue);

	timeSuite->GetTicksPerAudioSample ((float)sampleRate.value.floatValue, &ticksPerSample);
	
	mySettings->sequenceAudioSuite->MakeAudioRenderer(	exID,
														exportInfoP->startTime,
														(PrAudioChannelType)channelType.value.intValue,
														kPrAudioSampleType_32BitFloat,
														(float)sampleRate.value.floatValue,
														&audioRenderID);

	totalAudioSamples = exportDuration / ticksPerSample;
	samplesRemaining = totalAudioSamples;

	// Find size of blip to ask for
	// The lesser of the value returned from GetMaxBlip and number of samples remaining
	mySettings->sequenceAudioSuite->GetMaxBlip (audioRenderID, ticksPerFrame.value.timeValue, &maxBlip);
	if (maxBlip < samplesRemaining)
	{
		samplesRequestedL = maxBlip;
	}
	else
	{
		samplesRequestedL = (csSDK_int32) samplesRemaining;
	}

	// Set temporary audio buffer size (measured in samples)
	// to be size of first blip requested
	audioBufferSizeL = samplesRequestedL;

	// Allocate audio buffers
	for (csSDK_int32 bufferIndexL = 0; bufferIndexL < audioChannelsL; bufferIndexL++)
	{
		audioBuffer[bufferIndexL] = (float *) memorySuite->NewPtr (audioBufferSizeL * AUDIO_SAMPLE_SIZE);
	}

	while(samplesRemaining && (resultS == malNoError))
	{
		// Fill the buffer with audio
		resultS = mySettings->sequenceAudioSuite->GetAudio(	audioRenderID,
															(csSDK_uint32) samplesRequestedL,
															audioBuffer,
															kPrFalse);

		if (resultS == malNoError)
		{
			bytesToWriteLu = samplesRequestedL * AUDIO_SAMPLE_SIZE;

			// Write out the buffer of audio retrieved
			for (csSDK_int32 bufferIndexL = 0; bufferIndexL < audioChannelsL; bufferIndexL++)
			{
				resultS = mySettings->exportFileSuite->Write(	exportInfoP->fileObject,
																reinterpret_cast<void*>(audioBuffer[bufferIndexL]),
																(csSDK_int32) bytesToWriteLu);

				if (bufferIndexL < audioChannelsL - 1)
				{
					// Move file pointer to next audio buffer
					distanceToMove = totalAudioSamples * AUDIO_SAMPLE_SIZE - bytesToWriteLu;
					resultS = mySettings->exportFileSuite->Seek(exportInfoP->fileObject,
																distanceToMove,
																filePtrLocation,
																fileSeekMode_Current);
				}
				else
				{
					// Move file pointer back from last to first audio buffer
					distanceToMove = -totalAudioSamples * AUDIO_SAMPLE_SIZE * (audioChannelsL - 1);
					resultS = mySettings->exportFileSuite->Seek(exportInfoP->fileObject,
																distanceToMove,
																filePtrLocation,
																fileSeekMode_Current);
				}
			}

			// Calculate remaining audio
			samplesRemaining -= samplesRequestedL;

			// Find size of next blip to ask for
			mySettings->sequenceAudioSuite->GetMaxBlip (audioRenderID, ticksPerFrame.value.timeValue, &maxBlip);
			if (maxBlip < samplesRemaining)
			{
				samplesRequestedL = maxBlip;
			}
			else
			{
				samplesRequestedL = (csSDK_int32) samplesRemaining;
			}
			if (audioBufferSizeL < samplesRequestedL)
			{
				samplesRequestedL = audioBufferSizeL;
			}
		}
	}

	// Free up audioBuffer
	for (csSDK_int32 bufferIndexL = 0; bufferIndexL < audioChannelsL; bufferIndexL++)
	{
		memorySuite->PrDisposePtr ((char *) audioBuffer[bufferIndexL]);
	}
	
	mySettings->sequenceAudioSuite->ReleaseAudioRenderer(	exID,
															audioRenderID);

	return resultS;
}


//	Writes an HTML File that contains the marker info in the same folder as the output 
void WriteMarkerAndProjectDataToFile(
	exportStdParms		*stdParmsP, 
	exDoExportRec		*exportInfoP)
{
	#ifdef PRWIN_ENV
	FILE			*fileP					= NULL;
	prMarkerRef		marker					= 0;
	char			*nameZ					= NULL,
					*commentZ				= NULL,
					*chapterZ				= NULL,
					*hrefZ					= NULL,
					*targetZ				= NULL;
	wchar_t			htmlOutputFilePath[256]	= {'\0'};
	char			settingsA[256]			= {'\0'};
	prBool			firstMarker				= kPrTrue;
	ExportSettings	*mySettings				= reinterpret_cast<ExportSettings*>(exportInfoP->privateData);

	char			HTML_begin[]			= "<html>\n<head>\n<title>SDK Exporter - Sequence Marker Data Output\n</title>\n</head>\n",
					HTML_body[]				= "<body>",
					HTML_end[]				= "</body>\n</html>",
					noMarkers[]				= "<center>There were no markers found in the Adobe Premiere Pro Sequence";
	csSDK_int32		filepathLen				= 255;
	csSDK_uint32	markerType				= 0,
					DVDmarkerType			= 0,
					numMarkers				= 0;
	PrTime			ticksPerSecond			= 0,
					markerTime				= 0,
					markerDuration			= 0;
	float			markerTimeFloat			= 0.0,
					markerDurationFloat		= 0.0;

	mySettings->exportFileSuite->GetPlatformPath(exportInfoP->fileObject, &filepathLen, htmlOutputFilePath);
	mySettings->timeSuite->GetTicksPerSecond (&ticksPerSecond);

	#ifdef PRWIN_ENV
	wcscat_s(htmlOutputFilePath, sizeof (htmlOutputFilePath) / sizeof (wchar_t), L".html");
	_wfopen_s(&fileP, htmlOutputFilePath, L"w");
	#else
	wcscat(htmlOutputFilePath, L".html");
	fileP = _wfopen(htmlOutputFilePath, L"w");
	#endif

	mySettings->markerSuite->GetMarkerCount(exportInfoP->timelineData, &numMarkers);
	marker = mySettings->markerSuite->GetFirstMarker(exportInfoP->timelineData);
	
	// If no markers in the timeline, create default "no markers" 
	if (numMarkers == 0)
	{
		fprintf(fileP, HTML_begin);
		fprintf(fileP, HTML_body);
		fprintf(fileP, settingsA);
		fprintf(fileP, "%s", &noMarkers);
		fprintf(fileP, HTML_end);
		fclose(fileP);
		
		// Exit the function, nothing else to do
		return;
	}
	
	while (marker != kInvalidMarkerRef)
	{
		mySettings->markerSuite->GetMarkerData(exportInfoP->timelineData, marker, PRT_MARKER_VALUE64, &markerTime);
		mySettings->markerSuite->GetMarkerData(exportInfoP->timelineData, marker, PRT_MARKER_DURATION64, &markerDuration);
		mySettings->markerSuite->GetMarkerData(exportInfoP->timelineData, marker, PRT_MARKER_NAME, reinterpret_cast<void*>(&nameZ));
		mySettings->markerSuite->GetMarkerData(exportInfoP->timelineData, marker, PRT_MARKER_COMMENT, reinterpret_cast<void*>(&commentZ));
		mySettings->markerSuite->GetMarkerData(exportInfoP->timelineData, marker, PRT_MARKER_CHAPTER, reinterpret_cast<void*>(&chapterZ));
		mySettings->markerSuite->GetMarkerData(exportInfoP->timelineData, marker, PRT_MARKER_HREF,	reinterpret_cast<void*>(&hrefZ));
		mySettings->markerSuite->GetMarkerData(exportInfoP->timelineData, marker, PRT_MARKER_TARGET, reinterpret_cast<void*>(&targetZ));
		mySettings->markerSuite->GetMarkerData(exportInfoP->timelineData, marker, PRT_MARKER_TYPE, reinterpret_cast<void*>(&markerType));
		mySettings->markerSuite->GetMarkerData(exportInfoP->timelineData, marker, PRT_DVD_MARKER_TYPE, reinterpret_cast<void*>(&DVDmarkerType));

		// Create an HTML table of marker information, make links active
		if (firstMarker)
		{
			fprintf(fileP, HTML_begin);
			fprintf(fileP, HTML_body);
			fprintf(fileP, "<center>\nSequence Marker Data Output<p>\n");
			fprintf(fileP, "<table border=\"4\" cellpadding=\"0\" cellspacing=\"2\" width=\"350\">\n");
			firstMarker = false;
		}

		markerTimeFloat = static_cast<float>(markerTime) / static_cast<float>(ticksPerSecond);
		fprintf(fileP, "<tr><td>Time</td><td>%.2f sec</td></tr>", markerTimeFloat);
		markerDurationFloat = static_cast<float>(markerDuration) / static_cast<float>(ticksPerSecond);
		fprintf(fileP, "<tr><td>Duration</td><td>%.2f sec</td></tr>\n", markerDurationFloat);
		fprintf(fileP, "<tr><td>Name</td><td>%s</td></tr>\n", nameZ);
		fprintf(fileP, "<tr><td>Comment</td><td>%s</td></tr>\n", commentZ);
		fprintf(fileP, "<tr><td>Chapter</td><td>%s</td></tr>\n", chapterZ);
		fprintf(fileP, "<tr><td>HREF</td><td><a href=\"%s\">%s</a></td></tr>\n", hrefZ, hrefZ);
		fprintf(fileP, "<tr><td>Frame Target</td><td>%s</td></tr>\n", targetZ);
		if (markerType == kMarkerType_Timeline)
		{
			fprintf(fileP, "<tr><td>Marker Type</td><td>Timeline Marker</td></tr>\n<tr><td>----------</td><td>----------</td>\n");
		}
		else if (markerType == kMarkerType_DVD)
		{
			if (DVDmarkerType == kDVDMarkerType_Main)
			{
				fprintf(fileP, "<tr><td>Marker Type</td><td>DVD Chapter Marker</td></tr>\n<tr><td>----------</td><td>----------</td>\n");
			}
			else if (DVDmarkerType == kDVDMarkerType_Scene)
			{
				fprintf(fileP, "<tr><td>Marker Type</td><td>DVD Scene Marker</td></tr>\n<tr><td>----------</td><td>----------</td>\n");
			}
			else if (DVDmarkerType == kDVDMarkerType_Stop)
			{
				fprintf(fileP, "<tr><td>Marker Type</td><td>DVD Stop Marker</td></tr>\n<tr><td>----------</td><td>----------</td>\n");
			}
			else
			{
				fprintf(fileP, "<tr><td>Marker Type</td><td>Unknown DVD Marker</td></tr>\n<tr><td>----------</td><td>----------</td>\n");
			}
		}
		else
		{
			fprintf(fileP, "<tr><td>Marker Type</td><td>Unknown Marker Type</td></tr>\n<tr><td>----------</td><td>----------</td>\n");
		}

		marker = mySettings->markerSuite->GetNextMarker(exportInfoP->timelineData, marker);
	}

	fprintf(fileP, "</table>\n</center>\n</body>\n</html>");
	fclose(fileP);
	
	#endif

	return;
}


csSDK_int32 GetNumberOfAudioChannels(csSDK_int32 audioChannelType)
{
	csSDK_int32 numberOfChannels = -1;

	if (audioChannelType == kPrAudioChannelType_Mono)
	{
		numberOfChannels = 1;
	}
	else if (audioChannelType == kPrAudioChannelType_Stereo)
	{
		numberOfChannels = 2;
	}
	else if (audioChannelType == kPrAudioChannelType_51)
	{
		numberOfChannels = 6;
	}
	return numberOfChannels;
}


csSDK_int32 GetPixelFormatSize(PrFourCC subtype)
{
	csSDK_int32 formatSize = 4; // Default to size of 8-bit pixel formats

	if (subtype == SDK_10_BIT_YUV)
	{
		formatSize = 4;
	}
	return formatSize;
}


csSDK_int32 GetPixelFormatSize(PrPixelFormat pixelFormat)
{
	csSDK_int32 formatSize = 4; // Default to size of 8-bit pixel formats

	if (pixelFormat == PrPixelFormat_VUYA_4444_32f)
	{
		formatSize = 16;
	}
	return formatSize;
}


float max(float a, float b)
{
	return (a > b ? a : b);
}


float min(float a, float b)
{
	return (a < b ? a : b);
}


void ConvertFrom8uTo32f(
  	char		*buffer8u,
	char		*buffer32f,
	csSDK_int32 width,
	csSDK_int32 height)
{
	csSDK_uint32 *tempSrcBuffer = (csSDK_uint32 *)buffer8u;
	float *tempDestBuffer = (float *)buffer32f;
	csSDK_uint32 X, Y, Z, A;
	for (csSDK_int32 row = 0; row < height; row++)
	{
		for (csSDK_int32 col = 0; col < width; col++)
		{
			Z = ((*tempSrcBuffer) << 24) >> 24;
			Y = ((*tempSrcBuffer) << 16) >> 24;
			X = ((*tempSrcBuffer) << 8) >> 24;
			A = (*tempSrcBuffer) >> 24;
			tempDestBuffer[0] = (float)Z / 255.0f;
			tempDestBuffer[1] = (float)Y / 255.0f;
			tempDestBuffer[2] = (float)X / 255.0f;
			tempDestBuffer[3] = (float)A / 255.0f;
			tempSrcBuffer++;
			tempDestBuffer += 4;
		}
	}
}


// This uses ITU-R Recommendation BT.601
void ConvertFromBGRA32fToVUYA32f(
  	char		*buffer32f,
	csSDK_int32	width,
	csSDK_int32	height)
{
	float *tempBuffer		= (float *)buffer32f;
	float Y, Cb, Cr;
	// The luma component float range is 0.0 = black to 1.0 = white
	float Y_RGBtoYCbCr[3]	= { 0.299f, 0.587f, 0.114f};
	// The Cb and Cr float range is -0.5 to 0.5
	float Cb_RGBtoYCbCr[3]	= { -0.168736f, -0.331264f, 0.5f}; 
	float Cr_RGBtoYCbCr[3]	= { 0.5f, -0.418688f, -0.081312f};
	for (csSDK_int32 row = 0; row < height; row++)
	{
		for (csSDK_int32 col = 0; col < width; col++)
		{
			// BGR -> VUY
			Y =		Y_RGBtoYCbCr[0] * tempBuffer[2] +	// Red
					Y_RGBtoYCbCr[1] * tempBuffer[1] +	// Green
					Y_RGBtoYCbCr[2] * tempBuffer[0];	// Blue
			Cb =	Cb_RGBtoYCbCr[0] * tempBuffer[2] +
					Cb_RGBtoYCbCr[1] * tempBuffer[1] +
					Cb_RGBtoYCbCr[2] * tempBuffer[0];
			Cr =	Cr_RGBtoYCbCr[0] * tempBuffer[2] +
					Cr_RGBtoYCbCr[1] * tempBuffer[1] +
					Cr_RGBtoYCbCr[2] * tempBuffer[0];

			tempBuffer[0] = Cr;
			tempBuffer[1] = Cb;
			tempBuffer[2] = Y;

			tempBuffer += 4;
		}
	}
}


// Converts a 32f VUYA buffer to the v410 format described at
// http://developer.apple.com/quicktime/icefloe/dispatch019.html#v410
void ConvertFrom32fToV410(
	char *buffer32f,
	char *bufferV410,
	csSDK_int32 width,
	csSDK_int32 height)
{
	float *tempSrcBuffer = (float *)buffer32f;
	csSDK_int32 *tempDestBuffer = (csSDK_int32 *)bufferV410;
	float fY, fCr, fCb;
	csSDK_uint32 Y, Cr, Cb;
	for (csSDK_int32 row = 0; row < height; row++)
	{
		for (csSDK_int32 col = 0; col < width; col++)
		{
			fCr = (*(tempSrcBuffer + 1) * 896.0f + 512.5f);
			Cr = (csSDK_uint32)max(64, min(960, fCr));
			fY = (*(tempSrcBuffer + 2) * 876.0f + 64.5f);
			Y = (csSDK_uint32)max(64, min(940, fY));
			fCb = (*tempSrcBuffer * 896.0f + 512.5f);
			Cb = (csSDK_uint32)max(64, min(960, fCb));
			*tempDestBuffer = (Cr << 22) + (Y << 12) + (Cb << 2);
			tempSrcBuffer += 4;
			tempDestBuffer++;
		}
	}
}


// Converts to a 32f VUYA buffer from the v410 format described at
// http://developer.apple.com/quicktime/icefloe/dispatch019.html#v410
void ConvertFromV410To32f(
  	char *bufferV410,
	char *buffer32f,
	csSDK_int32 width,
	csSDK_int32 height)
{
	csSDK_uint32 *tempSrcBuffer = (csSDK_uint32 *)bufferV410;
	float *tempDestBuffer = (float *)buffer32f;
	csSDK_uint32 Y, Cr, Cb; // Y != y
	for (csSDK_int32 row = 0; row < height; row++)
	{
		for (csSDK_int32 col = 0; col < width; col++)
		{
			Cr = (*tempSrcBuffer) >> 22;
			Y = ((*tempSrcBuffer) << 10) >> 22;
			Cb = ((*tempSrcBuffer) << 20) >> 22;
			tempDestBuffer[0] = ((float)Cb - 512.0f) / 896.0f;
			tempDestBuffer[1] = ((float)Cr - 512.0f) / 896.0f;
			tempDestBuffer[2] = ((float)Y - 64.0f) / 876.0f;
			tempDestBuffer[3] = 1.0f;
			tempSrcBuffer++;
			tempDestBuffer += 4;
		}
	}
}


// Assumes that prTime is a framerate < ticksPerSecond
void ConvertPrTimeToScaleSampleSize(
	PrSDKTimeSuite	*timeSuite,
	PrTime			prTime,
	csSDK_int32		*scale,
	csSDK_int32		*sampleSize)
{
	PrTime	ticksPerSecond = 0,
			tempFrameRate = 0;
	timeSuite->GetTicksPerSecond(&ticksPerSecond);
	if (ticksPerSecond % prTime == 0) // a nice round frame rate
	{
		*scale = static_cast<csSDK_int32>(ticksPerSecond / prTime);
		*sampleSize = 1;
	}
	else
	{
		timeSuite->GetTicksPerVideoFrame(kVideoFrameRate_NTSC, &tempFrameRate);
		if (tempFrameRate == prTime)
		{
			*scale = 30000;
			*sampleSize = 1001;
		}
		timeSuite->GetTicksPerVideoFrame(kVideoFrameRate_NTSC_HD, &tempFrameRate);
		if (tempFrameRate == prTime)
		{
			*scale = 60000;
			*sampleSize = 1001;
		}
		timeSuite->GetTicksPerVideoFrame(kVideoFrameRate_24Drop, &tempFrameRate);
		if (tempFrameRate == prTime)
		{
			*scale = 24000;
			*sampleSize = 1001;
		}
	}
}


void ConvertScaleSampleSizeToPrTime(
	PrSDKTimeSuite	*timeSuite,
	csSDK_int32		*scale,
	csSDK_int32		*sampleSize,
	PrTime			*prTime)
{
	if ((*scale == 24000 && *sampleSize == 1001) ||
			(*scale == 23976 && *sampleSize == 1000) ||
			(*scale == 2397 && *sampleSize == 100))
	{
		timeSuite->GetTicksPerVideoFrame(kVideoFrameRate_24Drop, prTime);
	}
	else if (*scale == 24)
	{
		timeSuite->GetTicksPerVideoFrame(kVideoFrameRate_24, prTime);
	}
	else if (*scale == 25)
	{
		timeSuite->GetTicksPerVideoFrame(kVideoFrameRate_PAL, prTime);
	}
	else if ((*scale == 30000 && *sampleSize == 1001) ||
		(*scale == 29970 && *sampleSize == 1000) ||
		(*scale == 2997 && *sampleSize == 100))
	{
		timeSuite->GetTicksPerVideoFrame(kVideoFrameRate_NTSC, prTime);
	}
	else if (*scale == 30)
	{
		timeSuite->GetTicksPerVideoFrame(kVideoFrameRate_30, prTime);
	}
	else if (*scale == 50)
	{
		timeSuite->GetTicksPerVideoFrame(kVideoFrameRate_PAL_HD, prTime);
	}
	else if ((*scale == 60000 && *sampleSize == 1001) ||
			(*scale == 5994 && *sampleSize == 100))
	{
		timeSuite->GetTicksPerVideoFrame(kVideoFrameRate_NTSC_HD, prTime);
	}
	else if (*scale == 60)
	{
		timeSuite->GetTicksPerVideoFrame(kVideoFrameRate_60, prTime);
	}
}


// Function to convert and copy string literals to the format expected by the exporter API.
// On Win: Pass the input directly to the output
// On Mac: All conversion happens through the CFString format
void copyConvertStringLiteralIntoUTF16(const wchar_t* inputString, prUTF16Char* destination)
{
#ifdef PRMAC_ENV
	int length = wcslen(inputString);
	CFRange	range = {0, kPrMaxPath};
	range.length = length;
	CFStringRef inputStringCFSR = CFStringCreateWithBytes(	kCFAllocatorDefault,
															reinterpret_cast<const UInt8 *>(inputString),
															length * sizeof(wchar_t),
															kCFStringEncodingUTF32LE,
															kPrFalse);
	CFStringGetBytes(	inputStringCFSR,
						range,
						kCFStringEncodingUTF16,
						0,
						kPrFalse,
						reinterpret_cast<UInt8 *>(destination),
						length * (sizeof (prUTF16Char)),
						NULL);
	destination[length] = 0; // Set NULL-terminator, since CFString calls don't set it, and MediaCore hosts expect it
	CFRelease(inputStringCFSR);
#elif defined PRWIN_ENV
	size_t length = wcslen(inputString);
	wcscpy_s(destination, length + 1, inputString);
#endif
}


// Utility function to merge strcpy_s on Win and strcpy on Mac into one call
void safeStrCpy (char *destStr, int size, const char *srcStr)
{
#ifdef PRWIN_ENV
	strcpy_s (destStr, size, srcStr);
#elif defined PRMAC_ENV
	strcpy (destStr, srcStr);
#endif
}


// Utility function to merge wcscat_s on Win and wcscat on Mac into one call
void safeWcscat (wchar_t *destStr, int size, const wchar_t *srcStr)
{
#ifdef PRWIN_ENV
	wcscat_s (destStr, size, srcStr);
#elif defined PRMAC_ENV
	wcscat (destStr, srcStr);
#endif
}
