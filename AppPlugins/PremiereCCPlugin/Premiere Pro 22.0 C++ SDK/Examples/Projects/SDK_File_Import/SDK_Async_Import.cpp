/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 1992-2008, Adobe Systems Incorporated                 */
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
	Revision History
		
	Version		Change										Engineer	Date
	=======		======										========	======
	1.0			Async code created							zal			12/13/2005
	1.01		Async code moved to separate source file	zal			03/21/2006
	1.5			Moving to STL list, made list access 		zal			01/28/2011
				 thread-safe, more debug info
*/

#include "SDK_Async_Import.h"


PREMPLUGENTRY xAsyncImportEntry(
	int		inSelector,
	void	*inParam)
{
	csSDK_int32			result			= aiUnsupported;
	SDKAsyncImporter	*asyncImporter	= 0;

	switch (inSelector)
	{
		case aiInitiateAsyncRead:
		{
			aiAsyncRequest* asyncRequest(reinterpret_cast<aiAsyncRequest*>(inParam));
			asyncImporter = reinterpret_cast<SDKAsyncImporter*>(asyncRequest->inPrivateData);
			result = asyncImporter->OnInitiateAsyncRead(asyncRequest->inSourceRec);
			break;
		}
		case aiCancelAsyncRead:
		{
			aiAsyncRequest* asyncRequest(reinterpret_cast<aiAsyncRequest*>(inParam));
			asyncImporter = reinterpret_cast<SDKAsyncImporter*>(asyncRequest->inPrivateData);
			result = asyncImporter->OnCancelAsyncRead(asyncRequest->inSourceRec);
			break;
		}
		case aiFlush:
		{
			asyncImporter = reinterpret_cast<SDKAsyncImporter*>(inParam);
			result = asyncImporter->OnFlush();
			break;
		}
		case aiGetFrame:
		{
			imSourceVideoRec* getFrameRec(reinterpret_cast<imSourceVideoRec*>(inParam));
			asyncImporter = reinterpret_cast<SDKAsyncImporter*>(getFrameRec->inPrivateData);
			result = asyncImporter->OnGetFrame(getFrameRec);
			break;
		}
		case aiClose:
		{
			asyncImporter = reinterpret_cast<SDKAsyncImporter*>(inParam);
			delete asyncImporter;
			result = aiNoError;
			break;
		}
	}
	
	return result;
}


SDKAsyncImporter::SDKAsyncImporter(imStdParms *stdParms, ImporterLocalRec8H inRecH)
{
	localRecH = (ImporterLocalRec8H) stdParms->piSuites->memFuncs->newHandle (sizeof (ImporterLocalRec8));
	memcpy ((*localRecH), (*inRecH), sizeof (ImporterLocalRec8));
	(*localRecH)->fileRef = CreateFileW ((*inRecH)->fileName,
						GENERIC_READ,
						FILE_SHARE_READ,
						NULL,
						OPEN_EXISTING,
						FILE_FLAG_OVERLAPPED, // Overlapped necessary for async reads
						NULL);
#ifdef PRWIN_ENV
	InitializeCriticalSection(&listLock);
#endif
}


SDKAsyncImporter::~SDKAsyncImporter()
{
	// Release acquired suites
	(*localRecH)->BasicSuite->ReleaseSuite(kPrSDKPPixCreatorSuite, kPrSDKPPixCreatorSuiteVersion);
	(*localRecH)->BasicSuite->ReleaseSuite(kPrSDKPPixCacheSuite, kPrSDKPPixCacheSuiteVersion);
	(*localRecH)->BasicSuite->ReleaseSuite(kPrSDKPPixSuite, kPrSDKPPixSuiteVersion);
	(*localRecH)->BasicSuite->ReleaseSuite(kPrSDKTimeSuite, kPrSDKTimeSuiteVersion);

	// If file has not yet been closed
	if ((*localRecH)->fileRef)
	{
		CloseHandle ((*localRecH)->fileRef);
	}

	(*localRecH)->memFuncs->disposeHandle(reinterpret_cast<char **>(localRecH));

#ifdef PRWIN_ENV
	// Try out the critical section before deleting it, just in case flush is still using it
	EnterCriticalSection(&listLock);
	LeaveCriticalSection(&listLock);
	DeleteCriticalSection(&listLock);
#endif
}


int SDKAsyncImporter::OnInitiateAsyncRead(imSourceVideoRec& inSourceRec)
{
	prMALError		result = aiFrameNotFound,
					returnValue = 0;
	PPixHand		tempPPH;

	imFrameFormat	*frameFormat = &inSourceRec.inFrameFormats[0];
	csSDK_int32		frameBytesL = frameFormat->inFrameWidth * frameFormat->inFrameHeight *
								GetPixelFormatSize((*localRecH)->theFile.videoSubtype);
	// Buffer to hold the raw format from the file, i.e. either BGRA_8u or v410
	char			*frameBufferFromFile;

	PrTime			ticksPerSecond = 0;
	(*localRecH)->TimeSuite->GetTicksPerSecond(&ticksPerSecond);

	csSDK_int32		theFrame = static_cast<csSDK_int32>(inSourceRec.inFrameTime / (*localRecH)->theFile.frameRate);

	#ifdef ASYNC_IMPORT_DEBUG_INFO
	char tempString[255];

	if (frameFormat->inPixelFormat == PrPixelFormat_BGRA_4444_8u)
		sprintf (tempString, "%i - Frame %i (BGRA 8u) initiated via InitiateAsyncRead\n", GetCurrentThreadId(), theFrame);
	else if (frameFormat->inPixelFormat == PrPixelFormat_VUYA_4444_8u)
		sprintf (tempString, "%i - Frame %i (VUYA 8u) initiated via InitiateAsyncRead\n", GetCurrentThreadId(), theFrame);
	else if (frameFormat->inPixelFormat == PrPixelFormat_VUYA_4444_32f)
		sprintf (tempString, "%i - Frame %i (VUYA 32f) initiated via InitiateAsyncRead\n", GetCurrentThreadId(), theFrame);
	else
		sprintf (tempString, "%i - Frame %i (other format) initiated via InitiateAsyncRead\n", GetCurrentThreadId(), theFrame);

	OutputDebugString(tempString);

#endif

	if ((*localRecH)->theFile.videoSubtype == SDK_8_BIT_RGB ||
		(*localRecH)->theFile.videoSubtype == SDK_10_BIT_YUV)
	{
		prRect theRect = {0, 0, frameFormat->inFrameWidth, frameFormat->inFrameHeight};

		// Check to see if frame is already in cache
		result = (*localRecH)->PPixCacheSuite->GetFrameFromCache (	(*localRecH)->importerID,
																	0,
																	theFrame,
																	1,
																	inSourceRec.inFrameFormats,
																	&tempPPH,
																	NULL,
																	NULL);
		if (result == suiteError_NoError)
		{
			#ifdef ASYNC_IMPORT_DEBUG_INFO
			sprintf (tempString, "%i - Frame %i already in cache\n", GetCurrentThreadId(), theFrame);
			OutputDebugString(tempString);
			#endif

			// If so, we're done.  There's no need to pass the frame back.
			result = aiNoError;

			// Dispose of PPH
			(*localRecH)->PPixSuite->Dispose (tempPPH);
		}
		else
		{
			FrameRequest *tempFrameRequest;
			tempFrameRequest = (FrameRequest*)(*localRecH)->memFuncs->newPtrClear(sizeof (FrameRequest));

			(*localRecH)->PPixCreatorSuite->CreatePPix(&tempPPH, PrPPixBufferAccess_WriteOnly, frameFormat->inPixelFormat, &theRect);
			// We create a special buffer for v410 to store the format contained in the file
			// because v410 -> VUYA_32f requires an extra conversion
			// Our 8-bit RGB format requires no such conversion to BGRA_8u
			if ((*localRecH)->theFile.videoSubtype == SDK_10_BIT_YUV)
			{
				frameBufferFromFile = (*localRecH)->memFuncs->newPtr(frameBytesL);
			}
			else if ((*localRecH)->theFile.videoSubtype == SDK_8_BIT_RGB)
			{
				(*localRecH)->PPixSuite->GetPixels(tempPPH, PrPPixBufferAccess_WriteOnly, &frameBufferFromFile);
			}

			// Create the request struct to add to list
			tempFrameRequest->frameNum = theFrame;
			tempFrameRequest->frameBufferFromFile = frameBufferFromFile;
			tempFrameRequest->thePPH = tempPPH;
			ZeroMemory(&(tempFrameRequest->overlapped), sizeof(OVERLAPPED));
			tempFrameRequest->overlapped.hEvent = (HANDLE)NULL;

			// Start loading it in memory asynchronously
			returnValue = ReadSDKFileAsync(	(*localRecH)->fileRef, 
											frameBytesL, 
											theFrame, 
											frameBufferFromFile,
											&(tempFrameRequest->overlapped));

			// Add the frame info to our list.  Access to the list must be synchronized since we use the same list on multiple threads.
#ifdef PRWIN_ENV
			EnterCriticalSection(&listLock);
			listOfOutstandingRequests.push_back(tempFrameRequest);
			LeaveCriticalSection(&listLock);
#endif

			#ifdef ASYNC_IMPORT_DEBUG_INFO
			sprintf (tempString, "%i - There are now %i outstanding requests\n", GetCurrentThreadId(), listOfOutstandingRequests.size());
			OutputDebugString(tempString);
			#endif

			result = aiNoError;
		}
	}

	return result;
}


int SDKAsyncImporter::OnCancelAsyncRead(imSourceVideoRec& inSourceRec)
{
	return aiUnsupported;
}


int SDKAsyncImporter::OnFlush()
{
	// Wait for outstanding requests to complete
	prMALError		returnValue = 0;
	DWORD			bytesTransferred;

	#ifdef ASYNC_IMPORT_DEBUG_INFO
	char tempString[255];
	if (listOfOutstandingRequests.empty())
	{
		sprintf (tempString, "%i - Flush called.  No outstanding requests\n", GetCurrentThreadId());
		OutputDebugString(tempString);
	}
	else
	{
		sprintf (tempString, "%i - Flush called.  There are currently %i outstanding requests\n", GetCurrentThreadId(), listOfOutstandingRequests.size());
		OutputDebugString(tempString);
	}
	#endif

	// Access to the list must be synchronized since we use the same list on multiple threads.
#ifdef PRWIN_ENV
	EnterCriticalSection(&listLock);
#endif

	std::list<FrameRequest *>::iterator	listIt = listOfOutstandingRequests.begin();

	while (!listOfOutstandingRequests.empty() && listIt != listOfOutstandingRequests.end())
	{
		returnValue = GetOverlappedResult((*localRecH)->fileRef, &((*listIt)->overlapped), &bytesTransferred, kPrFalse);

		// If error reading
		if (returnValue == 0)
		{
			returnValue = GetLastError();
			if (returnValue == ERROR_IO_INCOMPLETE || returnValue == ERROR_IO_PENDING)
			{
				// [TODO] Cancel the read if pending
				listIt++;

				#ifdef ASYNC_IMPORT_DEBUG_INFO
				sprintf (tempString, "%i - Frame %i returned IO_INCOMPLETE or PENDING error.\n", GetCurrentThreadId(), (*listIt)->frameNum);
				OutputDebugString(tempString);
				#endif
			}
			else
			{
				(*localRecH)->PPixSuite->Dispose ((*listIt)->thePPH);
				if ((*localRecH)->theFile.videoSubtype == SDK_10_BIT_YUV)
				{
					(*localRecH)->memFuncs->disposePtr((*listIt)->frameBufferFromFile);
				}
				(*localRecH)->memFuncs->disposePtr(reinterpret_cast<char *>((*listIt)));

				listIt = listOfOutstandingRequests.erase(listIt);

				#ifdef ASYNC_IMPORT_DEBUG_INFO
				sprintf (tempString, "%i - Frame %i returned unknown error.  Cancelled\n", GetCurrentThreadId(), (*listIt)->frameNum);
				OutputDebugString(tempString);
				#endif
			}
		}
		else
		{
			(*localRecH)->PPixSuite->Dispose ((*listIt)->thePPH);
			if ((*localRecH)->theFile.videoSubtype == SDK_10_BIT_YUV)
			{
				(*localRecH)->memFuncs->disposePtr((*listIt)->frameBufferFromFile);
			}
			(*localRecH)->memFuncs->disposePtr(reinterpret_cast<char *>((*listIt)));

			listIt = listOfOutstandingRequests.erase(listIt);
		}
	}

#ifdef PRWIN_ENV
	LeaveCriticalSection(&listLock);
#endif

	return aiNoError;
}


int SDKAsyncImporter::OnGetFrame(imSourceVideoRec* inSourceRec)
{
	prMALError	result				= aiFrameNotFound,
				cacheResult			= suiteError_NoError;
	csSDK_int32	rowBytes			= 0;
	prBool		foundFrame			= kPrFalse;
	char		*frameBuffer		= NULL;

	PrTime ticksPerSecond = 0;
	(*localRecH)->TimeSuite->GetTicksPerSecond(&ticksPerSecond);
	csSDK_int32 theFrame = static_cast<csSDK_int32>(inSourceRec->inFrameTime / (*localRecH)->theFile.frameRate);

	#ifdef ASYNC_IMPORT_DEBUG_INFO
	char tempString[255];

	if (inSourceRec->inFrameFormats[0].inPixelFormat == PrPixelFormat_BGRA_4444_8u)
		sprintf (tempString, "%i - Frame %i (BGRA 8u) initiated via GetFrame\n", GetCurrentThreadId(), theFrame);
	else if (inSourceRec->inFrameFormats[0].inPixelFormat == PrPixelFormat_VUYA_4444_8u)
		sprintf (tempString, "%i - Frame %i (VUYA 8u) initiated via GetFrame\n", GetCurrentThreadId(), theFrame);
	else if (inSourceRec->inFrameFormats[0].inPixelFormat == PrPixelFormat_VUYA_4444_32f)
		sprintf (tempString, "%i - Frame %i (VUYA 32f) initiated via GetFrame\n", GetCurrentThreadId(), theFrame);
	else
		sprintf (tempString, "%i - Frame %i (other format) initiated via GetFrame\n", GetCurrentThreadId(), theFrame);

	OutputDebugString(tempString);

	#endif

	// Check to see if frame is already in cache
	PPixHand tempPPH;
	result = (*localRecH)->PPixCacheSuite->GetFrameFromCache (	(*localRecH)->importerID,
																0,
																theFrame,
																1,
																inSourceRec->inFrameFormats,
																&tempPPH,
																NULL,
																NULL);

	if (result == suiteError_NoError)
	{
		#ifdef ASYNC_IMPORT_DEBUG_INFO
		sprintf (tempString, "%i - Frame %i already in cache\n", GetCurrentThreadId(), theFrame);
		OutputDebugString(tempString);
		#endif

		// If so, we're done.  Just pass the frame back.
		foundFrame = kPrTrue;
		*(inSourceRec->outFrame) = tempPPH;
	}
	else
	{
		prBool returnValue = kPrFalse;
		DWORD bytesTransferred;

		// Access to the list must be synchronized since we use the same list on multiple threads.
#ifdef PRWIN_ENV
		EnterCriticalSection(&listLock);
#endif

		std::list<FrameRequest *>::iterator	listIt = listOfOutstandingRequests.begin();

		while (!listOfOutstandingRequests.empty() && listIt != listOfOutstandingRequests.end())
		{
			returnValue = GetOverlappedResult((*localRecH)->fileRef, &((*listIt)->overlapped), &bytesTransferred, kPrFalse);

			// If error reading
			if (returnValue == 0)
			{
				returnValue = GetLastError();
				if (returnValue == ERROR_IO_INCOMPLETE || returnValue == ERROR_IO_PENDING)
				{
					listIt++;

					#ifdef ASYNC_IMPORT_DEBUG_INFO
					sprintf (tempString, "%i - Frame %i returned IO_INCOMPLETE or PENDING error.\n", GetCurrentThreadId(), (*listIt)->frameNum);
					OutputDebugString(tempString);
					#endif
				}
				else
				{
					(*localRecH)->PPixSuite->Dispose ((*listIt)->thePPH);
					if ((*localRecH)->theFile.videoSubtype == SDK_10_BIT_YUV)
					{
						(*localRecH)->memFuncs->disposePtr((*listIt)->frameBufferFromFile);
					}
					(*localRecH)->memFuncs->disposePtr(reinterpret_cast<char *>((*listIt)));

					listIt = listOfOutstandingRequests.erase(listIt);

					#ifdef ASYNC_IMPORT_DEBUG_INFO
					sprintf (tempString, "%i - Frame %i returned unknown error.  Cancelled\n", GetCurrentThreadId(), (*listIt)->frameNum);
					OutputDebugString(tempString);
					#endif
				}
			}
			else
			{
				returnValue = GetLastError();

				(*localRecH)->PPixSuite->GetPixels((*listIt)->thePPH, PrPPixBufferAccess_WriteOnly, &frameBuffer);

				// Do further processing on v410 frames
				// 8-bit RGB frames are already in the BGRA_8u format needed
				if ((*localRecH)->theFile.videoSubtype == SDK_10_BIT_YUV)
				{
					// Copy convert the v410 buffer into a VUYA_32f buffer
					ConvertFromV410To32f(	(*listIt)->frameBufferFromFile,
											frameBuffer,
											(*localRecH)->theFile.width,
											(*localRecH)->theFile.height);
				}

				// If extra row padding is needed, add it
				(*localRecH)->PPixSuite->GetRowBytes ((*listIt)->thePPH, &rowBytes);
				AddRowPadding (	frameBuffer,
								frameBuffer,
								rowBytes,
								GetPixelFormatSize ((*localRecH)->theFile.videoSubtype),
								(*localRecH)->theFile.width,
								(*localRecH)->theFile.height);
													
				// Add to cache
				cacheResult = (*localRecH)->PPixCacheSuite->AddFrameToCache (	(*localRecH)->importerID,
																				0,
																				(*listIt)->thePPH,
																				(*listIt)->frameNum,
																				NULL,
																				NULL);

				#ifdef ASYNC_IMPORT_DEBUG_INFO
				sprintf (tempString, "%i - Frame %i completed\n", GetCurrentThreadId(), (*listIt)->frameNum);
				OutputDebugString(tempString);
				#endif

				// Does this match the frame we were asked for?
				if ((*listIt)->frameNum == theFrame)
				{
					#ifdef ASYNC_IMPORT_DEBUG_INFO
					sprintf (tempString, "%i - This frame matches the request!\n", GetCurrentThreadId());
					OutputDebugString(tempString);
					#endif

					*(inSourceRec->outFrame) = (*listIt)->thePPH;
					foundFrame = kPrTrue;

					if ((*localRecH)->theFile.videoSubtype == SDK_10_BIT_YUV)
					{
						(*localRecH)->memFuncs->disposePtr((*listIt)->frameBufferFromFile);
					}
					(*localRecH)->memFuncs->disposePtr(reinterpret_cast<char *>((*listIt)));

					listIt = listOfOutstandingRequests.erase(listIt);

					result = aiNoError;
					break;
				}
				else
				{
					// Dispose of PPH, though it will stay in the cache
					(*localRecH)->PPixSuite->Dispose ((*listIt)->thePPH);
					if ((*localRecH)->theFile.videoSubtype == SDK_10_BIT_YUV)
					{
						(*localRecH)->memFuncs->disposePtr((*listIt)->frameBufferFromFile);
					}
					(*localRecH)->memFuncs->disposePtr(reinterpret_cast<char *>((*listIt)));

					listIt = listOfOutstandingRequests.erase(listIt);
				}
			}
		}
#ifdef PRWIN_ENV
		LeaveCriticalSection(&listLock);
#endif
		#ifdef ASYNC_IMPORT_DEBUG_INFO
		if (listOfOutstandingRequests.empty())
		{
			sprintf (tempString, "%i - No outstanding requests\n", GetCurrentThreadId());
			OutputDebugString(tempString);
		}
		else
		{
			sprintf (tempString, "%i - There are now %i outstanding requests\n", GetCurrentThreadId(), listOfOutstandingRequests.size());
			OutputDebugString(tempString);
		}
		#endif
	}

	if (!foundFrame)
	{
		*(inSourceRec->outFrame) = NULL;
	}

	return result;
}