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
/*
	12/24/08	ZL	Split segment-parsing code out to separate utility file
*/

#include <SDK_Segment_Utils.h>


// This is the information we keep on each segment in the cutlist
// We initialize each segment with this default information
segmentInfo::segmentInfo()
{
	segmentHash.mGUID[0] = '\0';
	description = "Empty";
	startTime = -kPrSDKEndOfTimeline;
	endTime = kPrSDKEndOfTimeline;
	clipID = -1;
}


VideoSequenceParser::VideoSequenceParser (
	SPBasicSuite	*spBasic,
	int				width,
	int				height,
	csSDK_uint32	PARNum,
	csSDK_uint32	PARDen)
{
	mSPBasic = spBasic;
	mWidth = width;
	mHeight = height;
	mPARNum = PARNum;
	mPARDen = PARDen;
	mCutlist = new std::list<segmentInfo>(1);	// Create cutlist with one empty segment

	// Acquire suites
	if (mSPBasic->AcquireSuite(kPrSDKVideoSegmentSuite, kPrSDKVideoSegmentSuiteVersion,
							  reinterpret_cast<const void**>((const void **)&mSegmentSuite)) != kSPNoError)
	{
		// This would be a problem
	}
	if (mSPBasic->AcquireSuite(kPrSDKMemoryManagerSuite, kPrSDKMemoryManagerSuiteVersion,
							  reinterpret_cast<const void**>((const void **)&mMemorySuite)) != kSPNoError)
	{
		// This would be a problem
	}
	if (mSPBasic->AcquireSuite(kPrSDKMemoryManagerSuite, kPrSDKMemoryManagerSuiteVersion,
							  reinterpret_cast<const void**>((const void **)&mMemorySuite)) != kSPNoError)
	{
		// This would be a problem
	}
	if (mSPBasic->AcquireSuite(kPrSDKPPixSuite, kPrSDKPPixSuiteVersion,
							  reinterpret_cast<const void**>((const void **)&mPPixSuite)) != kSPNoError)
	{
		// This would be a problem
	}
	if (spBasic->AcquireSuite (	kPrSDKMediaAcceleratorSuite,
								kPrSDKMediaAcceleratorSuiteVersion,
								const_cast<const void**>(reinterpret_cast<void**>(&mMediaAccelSuite))) != kSPNoError)
	{
		// This would be a problem
	}
	if (spBasic->AcquireSuite(	kPrSDKRollCrawlSuite,
								kPrSDKRollCrawlSuiteVersion,
								reinterpret_cast<const void**>((const void **)&mRollCrawlSuite)) != kSPNoError)
	{
		// This would be a problem;
	}	

}


VideoSequenceParser::~VideoSequenceParser ()
{
	// Cleanup the cutlist
	mCutlist->clear();
	delete mCutlist;
}


prMALError VideoSequenceParser::ParseSequence (PrTimelineID mSequenceID, csSDK_int32 indentation)
{
	prMALError	result					= malNoError,
				returnValue				= malNoError;
	std::list<segmentInfo>::iterator segmentIt, segmentEnd;
	csSDK_int32	segmentsID				= 0,
				nodeID					= 0,
				numSegments				= 0,
				nodeFlags				= 0;
	PrTime		startTime				= 0,
				endTime					= 0,
				segmentOffset			= 0;
	prPluginID	segmentHash,
				nodeHash;
	char		tempText[255],
				nodeType[kMaxNodeTypeStringSize];

	mSegmentSuite->AcquireVideoSegmentsWithPreviewsID(mSequenceID, &segmentsID);
	mSegmentSuite->GetSegmentCount(segmentsID, &numSegments);
	segmentIt = mCutlist->begin();

	if (static_cast<csSDK_uint32>(numSegments) > mCutlist->size())
	{
		mCutlist->resize(numSegments);
	}

	for (int i = 0; i < numSegments; i++, segmentIt++)
	{
		mSegmentSuite->GetSegmentInfo(	segmentsID,
										i,
										&startTime,
										&endTime,
										&segmentOffset,
										&segmentHash);
		mSegmentSuite->AcquireNodeID(	segmentsID,
										&segmentHash,
										&nodeID);
		segmentIt->nodeID = nodeID;
		// Even if the segment hash doesn't change, the start and end times may be changed
		segmentIt->startTime = startTime;
		segmentIt->endTime = endTime;
		if (strcmp (segmentIt->segmentHash.mGUID, segmentHash.mGUID))
		{
			segmentIt->description.clear();
			segmentIt->description.append("Segment Start: ");
		}
		#ifdef PRWIN_ENV
		sprintf_s(tempText, 255, "%*.2f sec  ", 7, startTime / 254016000000.0);
		#elif defined PRMAC_ENV
		sprintf(tempText, "%*.2f sec  ", 7, startTime / 254016000000.0);
		#endif
		segmentIt->description.replace(15, 13, tempText);

		segmentIt->description.replace(28, 6, " End: ");
		#ifdef PRWIN_ENV
		sprintf_s(tempText, 255, "%*.2f sec  ", 7, endTime / 254016000000.0);
		#elif defined PRMAC_ENV
		sprintf(tempText, "%*.2f sec  ", 7, endTime / 254016000000.0);
		#endif
		segmentIt->description.replace(34, 13, tempText);

		segmentIt->description.replace(47, 9, " Offset: ");
		#ifdef PRWIN_ENV
		sprintf_s(tempText, 255, "%*.2f sec\n", 8, segmentOffset / 254016000000.0);
		#elif defined PRMAC_ENV
		sprintf(tempText, "%*.2f sec\n", 8, endTime / 254016000000.0);
		#endif
		segmentIt->description.replace(56, 13, tempText);

		if (strcmp (segmentIt->segmentHash.mGUID, segmentHash.mGUID))
		{
			#ifdef PRWIN_ENV
			strcpy_s(segmentIt->segmentHash.mGUID, 37, segmentHash.mGUID);
			#elif defined PRMAC_ENV
			strcpy(segmentIt->segmentHash.mGUID, segmentHash.mGUID);
			#endif

			mSegmentSuite->GetNodeInfo(	nodeID,
										nodeType,
										&nodeHash,
										&nodeFlags);
			if (strcmp(nodeType, kVideoSegment_NodeType_Compositor) == 0)
			{
				returnValue = parseCompositor(segmentIt, nodeID, indentation + 1);
			}
			else if (strcmp(nodeType, kVideoSegment_NodeType_Multicam) == 0)
			{
				returnValue = parseMulticam(segmentIt, nodeID, indentation + 1);
			}
			else if (strcmp(nodeType, kVideoSegment_NodeType_Preview) == 0)
			{
				segmentIt->description.append("Preview ");

				returnValue = parseMedia(segmentIt, nodeID, indentation + 1);
			}
			else
			{
				segmentIt->description.append("Unknown Node Type\n");
			}
			mSegmentSuite->ReleaseVideoNodeID(nodeID);
		}
	}

	// If there are no segments, make sure there is a long empty segment
	if (!numSegments)
	{
		mCutlist->clear();
		mCutlist->resize(1);
	}
	// Otherwise, make sure the segment at the end is a long empty segment
	else
	{
		// Make sure there is one more segment
		segmentEnd = mCutlist->end();
		if (segmentIt == segmentEnd)
		{
			mCutlist->resize(mCutlist->size() + 1);
			segmentIt--;
		}

		// Create the long empty segment
		segmentEnd = segmentIt;
		segmentIt--;
		segmentEnd->segmentHash.mGUID[0] = '\0';
		segmentEnd->description.clear();
		segmentEnd->startTime = segmentIt->endTime;
		segmentEnd->endTime = kPrSDKEndOfTimeline;

		// Delete rest of segments, if any
		segmentEnd = mCutlist->end();
		segmentEnd--;
		segmentIt++;
		if (segmentIt != segmentEnd)
		{
			mCutlist->erase(++segmentIt, mCutlist->end());
		}
	}

	mSegmentSuite->ReleaseVideoSegmentsID(segmentsID);

	return result;
}


prMALError VideoSequenceParser::parseCompositor (
	std::list<segmentInfo>::iterator	segmentIt,
	csSDK_int32							nodeID,
	csSDK_int32							indentation)
{
	prMALError	result					= malNoError,
				returnValue				= malNoError;

	csSDK_int32	nodeInputs				= 0,
				layerNodeID				= 0,
				layerNodeFlags			= 0;
	PrTime		layerNodeOffset			= 0;
	prPluginID	layerNodeHash;
	char		layerNodeType[kMaxNodeTypeStringSize],
				tempText[255];

	for (int i = 0; i < indentation; i++)
	{
		segmentIt->description.append(" ");
	}
	segmentIt->description.append("Compositor Node\n");

	// How many layers are in this compositor node?
	mSegmentSuite->GetNodeInputCount(nodeID, &nodeInputs);
	// Iterate over the layers
	for (int nodeInput = 0; nodeInput < nodeInputs; nodeInput++)
	{
		mSegmentSuite->AcquireInputNodeID(	nodeID,
											nodeInput,
											&layerNodeOffset,
											&layerNodeID);

		for (int i = 0; i < indentation; i++)
		{
			segmentIt->description.append(" ");
		}
		segmentIt->description.append(" Layer offset: ");
		#ifdef PRWIN_ENV
		sprintf_s(tempText, 255, "%*.2f sec\n", 7, layerNodeOffset / 254016000000.0);
		#elif defined PRMAC_ENV
		sprintf(tempText, "%*.2f sec\n", 7, layerNodeOffset / 254016000000.0);
		#endif
		segmentIt->description.append(tempText);

		mSegmentSuite->GetNodeInfo(	layerNodeID,
									layerNodeType,
									&layerNodeHash,
									&layerNodeFlags);
		if (strcmp (layerNodeType, kVideoSegment_NodeType_Clip) == 0)
		{
			returnValue = parseClips(segmentIt, layerNodeID, indentation + 1);
		}
		else if (strcmp (layerNodeType, kVideoSegment_NodeType_Transition) == 0)
		{
			returnValue = parseTransition(segmentIt, layerNodeID, indentation + 1);
		}
		else
		{
			// Return an error in the text overlay
		}
		mSegmentSuite->ReleaseVideoNodeID(layerNodeID);
	}

	return result;
}


prMALError VideoSequenceParser::parseMulticam (
	std::list<segmentInfo>::iterator	segmentIt,
	csSDK_int32							nodeID,
	csSDK_int32							indentation)
{
	prMALError	result					= malNoError,
				returnValue				= malNoError;

	char		*selectedTrackID;
	csSDK_int32	nodeInputs				= 0,
				layerNodeID				= 0,
				layerNodeFlags			= 0;
	PrTime		layerNodeOffset			= 0;
	prPluginID	layerNodeHash;
	char		layerNodeType[kMaxNodeTypeStringSize];

	for (int i = 0; i < indentation; i++)
	{
		segmentIt->description.append(" ");
	}
	segmentIt->description.append("Multicam Node\n");
	returnValue = mSegmentSuite->GetNodeProperty(
									nodeID,
									kVideoSegmentProperty_Multicam_SelectedTrack,
									&selectedTrackID);
	if (returnValue)
	{
		// As of CS5, there is a bug (2551735) where nested sequences masquerade as multicam sequences
		// If the selected track isn't returned, then we know this was really a nested sequence
		ParseSequence(nodeID, indentation);
	}
	else
	{
		mMemorySuite->PrDisposePtr(selectedTrackID);

		// How many layers are in this multicam node?
		mSegmentSuite->GetNodeInputCount(nodeID, &nodeInputs);
		// Iterate over the layers
		for (int nodeInput = 0; nodeInput < nodeInputs; nodeInput++)
		{
			mSegmentSuite->AcquireInputNodeID(	nodeID,
												nodeInput,
												&layerNodeOffset,
												&layerNodeID);
			mSegmentSuite->GetNodeInfo(	layerNodeID,
										layerNodeType,
										&layerNodeHash,
										&layerNodeFlags);
			if (strcmp (layerNodeType, kVideoSegment_NodeType_Clip) == 0)
			{
				returnValue = parseClips(segmentIt, layerNodeID, indentation + 1);
			}
			else if (strcmp (layerNodeType, kVideoSegment_NodeType_Transition) == 0)
			{
				returnValue = parseTransition(segmentIt, layerNodeID, indentation + 1);
			}
			else
			{
				// Return an error in the text overlay
			}
			mSegmentSuite->ReleaseVideoNodeID(layerNodeID);
		}
	}

	return result;
}


prMALError VideoSequenceParser::parseTransition (
	std::list<segmentInfo>::iterator	segmentIt,
	csSDK_int32							transitionNodeID,
	csSDK_int32							indentation)
{
	prMALError	result					= malNoError,
				returnValue				= malNoError;

	PrMemoryPtr	transitionName,
				transitionDurationString,
				transitionOpaqueData,
				transitionInstanceID;
	csSDK_int32	transitionNodeInputs				= 0,
				transitionInputNodeID				= 0,
				transitionInputNodeFlags			= 0,
				transitionInputNodeInputs			= 0;
	PrTime		transitionDuration					= 0,
				transitionInputNodeOffset			= 0;
	prPluginID	transitionInputNodeHash;
	char		transitionInputNodeType[kMaxNodeTypeStringSize];

	for (int i = 0; i < indentation; i++)
	{
		segmentIt->description.append(" ");
	}
	segmentIt->description.append("Transition Node: ");
	mSegmentSuite->GetNodeProperty(	transitionNodeID,
									kVideoSegmentProperty_Transition_TransitionMatchName,
									&transitionName);
	mSegmentSuite->GetNodeProperty(	transitionNodeID,
									kVideoSegmentProperty_Transition_TransitionDuration,
									&transitionDurationString);
	returnValue = mSegmentSuite->GetNodeProperty(transitionNodeID,
									kVideoSegmentProperty_Transition_TransitionOpaqueData,
									&transitionOpaqueData);
	mSegmentSuite->GetNodeProperty(transitionNodeID,
									"TransitionNode::RuntimeInstanceID",
									&transitionInstanceID);
	transitionDuration = atoi(transitionDurationString);
	segmentIt->description.append(transitionName);
	segmentIt->description.append("\n");
	mMemorySuite->PrDisposePtr(transitionName);
	mMemorySuite->PrDisposePtr(transitionDurationString);
	if (!returnValue)
	{
		mMemorySuite->PrDisposePtr(transitionOpaqueData);
	}
	mMemorySuite->PrDisposePtr(transitionInstanceID);

	// How many clip items are in this transition node?
	mSegmentSuite->GetNodeInputCount(transitionNodeID, &transitionNodeInputs);
	// Iterate over the clips
	for (int transitionNodeIndex = 0; transitionNodeIndex < transitionNodeInputs; transitionNodeIndex++)
	{
		for (int i = 0; i < indentation; i++)
		{
			segmentIt->description.append(" ");
		}
		if (transitionNodeIndex == 0)
		{
			segmentIt->description.append("Input 1 (outgoing clip):\n");
		}
		else if (transitionNodeIndex == 1)
		{
			segmentIt->description.append("Input 2 (incoming clip):\n");
		}
		returnValue = mSegmentSuite->AcquireInputNodeID(transitionNodeID,
														transitionNodeIndex,
														&transitionInputNodeOffset,
														&transitionInputNodeID);
		if (!returnValue && transitionInputNodeID)
		{
			// This call will return malUnknownError if this transitionNodeInput is empty
			returnValue = mSegmentSuite->GetNodeInfo(	transitionInputNodeID,
														transitionInputNodeType,
														&transitionInputNodeHash,
														&transitionInputNodeFlags);
			if (strcmp(transitionInputNodeType, kVideoSegment_NodeType_Clip) == 0)
			{
				returnValue = parseClips(segmentIt, transitionInputNodeID, indentation + 1);
			}
			else
			{
				for (int i = 0; i < indentation + 1; i++)
				{
					segmentIt->description.append(" ");
				}
				segmentIt->description.append("Empty (transparent black)\n");
			}

			// How many inputs are there for this node?  Should be none.
			mSegmentSuite->GetNodeInputCount(transitionInputNodeID, &transitionInputNodeInputs);
			mSegmentSuite->ReleaseVideoNodeID(transitionInputNodeID);
		}
		else
		{
			for (int i = 0; i < indentation + 1; i++)
			{
				segmentIt->description.append(" ");
			}
			segmentIt->description.append("Empty (transparent black)\n");
		}
	}

	return result;
}


prMALError VideoSequenceParser::parseClips (
	std::list<segmentInfo>::iterator	segmentIt,
	csSDK_int32							clipNodeID,
	csSDK_int32							indentation)
{
	prMALError	result					= malNoError,
				returnValue				= malNoError;

	csSDK_int32	clipNodeInputs			= 0,
				mediaNodeID				= 0,
				mediaNodeFlags			= 0,
				mediaNodeInputs			= 0;
	PrTime		mediaNodeOffset			= 0;
	prPluginID	mediaNodeHash;
	char		mediaNodeType[kMaxNodeTypeStringSize],
				tempText[255];

	for (int i = 0; i < indentation; i++)
	{
		segmentIt->description.append(" ");
	}
	segmentIt->description.append("Clip Node\n");
	// How many media items are in this clip node?  Should be just one.
	mSegmentSuite->GetNodeInputCount(clipNodeID, &clipNodeInputs);
	// Iterate over the media
	for (int mediaNodeIndex = 0; mediaNodeIndex < clipNodeInputs; mediaNodeIndex++)
	{
		mSegmentSuite->AcquireInputNodeID(	clipNodeID,
											mediaNodeIndex,
											&mediaNodeOffset,
											&mediaNodeID);

		for (int i = 0; i < indentation; i++)
		{
			segmentIt->description.append(" ");
		}
		segmentIt->description.append(" Media offset: ");
		#ifdef PRWIN_ENV
		sprintf_s(tempText, 255, "%*.2f sec\n", 7, mediaNodeOffset / 254016000000.0);
		#elif defined PRMAC_ENV
		sprintf(tempText, "%*.2f sec\n", 7, mediaNodeOffset / 254016000000.0);
		#endif
		segmentIt->description.append(tempText);

		mSegmentSuite->GetNodeInfo(	mediaNodeID,
									mediaNodeType,
									&mediaNodeHash,
									&mediaNodeFlags);
		if (strcmp(mediaNodeType, kVideoSegment_NodeType_Media) == 0)
		{
			returnValue = parseMedia(segmentIt, mediaNodeID, indentation + 1);
		}

		// How many inputs are there for this media node?  Should be none.
		mSegmentSuite->GetNodeInputCount(mediaNodeID, &mediaNodeInputs);
		mSegmentSuite->ReleaseVideoNodeID(mediaNodeID);
	}

	returnValue = parseClipOperators(segmentIt, clipNodeID, indentation + 1);

	return result;
}


prMALError VideoSequenceParser::parseMedia (
	std::list<segmentInfo>::iterator	segmentIt,
	csSDK_int32							mediaNodeID,
	csSDK_int32							indentation)
{
	prMALError	result					= malNoError,
				returnValue				= malNoError;
	char		*nestedSequenceIDString	= 0,
				*multicamSequenceIDString = 0,
				*mediaSubtype			= 0,
				*mediaWidthString		= 0,
				*mediaHeightString		= 0,
				*mediaPARNumString		= 0,
				*mediaPARDenString		= 0,
				*mediaFrameRateString	= 0,
				*mediaClipSpeedString	= 0,
				*mediaPath				= 0,
				*mediaClipIDString		= 0,
				*mediaAlphaTypeString	= 0,
				*isOffline				= 0;
	int			nestedSequenceID		= 0,
				multicamSequenceID		= 0,
				mediaWidth				= 0,
				mediaHeight				= 0,
				mediaPARNum				= 0,
				mediaPARDen				= 0,
				mediaClipID				= 0,
				mediaAlphaType			= 0;
	double		mediaClipSpeed			= 0.0;
	PrTime		mediaFrameRate			= 0;
	PrMemoryPtr	isRollCrawl				= 0;

	for (int i = 0; i < indentation; i++)
	{
		segmentIt->description.append(" ");
	}
	segmentIt->description.append("Media Node:\n");

	returnValue = mSegmentSuite->GetNodeProperty(
									mediaNodeID,
									kVideoSegmentProperty_Media_NestedSequenceTimelineID,
									&nestedSequenceIDString);
	if (!returnValue)
	{
		// This "media" must be a nested sequence
		nestedSequenceID = atoi(nestedSequenceIDString);
		returnValue = ParseSequence(nestedSequenceID, indentation + 1);
		mMemorySuite->PrDisposePtr(nestedSequenceIDString);
	}
	else
	{
		returnValue = mSegmentSuite->GetNodeProperty(
										mediaNodeID,
										kVideoSegmentProperty_Media_SelectedMulticamTrackTimelineID,
										&multicamSequenceIDString);
		if (!returnValue)
		{
			// This "media" must be a multicam sequence
			multicamSequenceID = atoi(multicamSequenceIDString);
			returnValue = parseMulticam(segmentIt, multicamSequenceID, indentation + 1);
		}
		else
		{
			returnValue = mSegmentSuite->GetNodeProperty(
											mediaNodeID,
											kVideoSegmentProperty_Media_StreamFrameWidth,
											&mediaWidthString);
			mediaWidth = atoi(mediaWidthString);
			returnValue = mSegmentSuite->GetNodeProperty(
											mediaNodeID,
											kVideoSegmentProperty_Media_StreamFrameHeight,
											&mediaHeightString);
			mediaHeight = atoi(mediaHeightString);
			returnValue = mSegmentSuite->GetNodeProperty(
											mediaNodeID,
											kVideoSegmentProperty_Media_StreamPixelAspectRatioNum,
											&mediaPARNumString);
			mediaPARNum = atoi(mediaPARNumString);
			returnValue = mSegmentSuite->GetNodeProperty(
											mediaNodeID,
											kVideoSegmentProperty_Media_StreamPixelAspectRatioDen,
											&mediaPARDenString);
			mediaPARDen = atoi(mediaPARDenString);
			returnValue = mSegmentSuite->GetNodeProperty(
											mediaNodeID,
											kVideoSegmentProperty_Media_StreamFrameRate,
											&mediaFrameRateString);
			mediaFrameRate = atoi(mediaFrameRateString);
			returnValue = mSegmentSuite->GetNodeProperty(
											mediaNodeID,
											kVideoSegmentProperty_Media_ClipSpeed,
											&mediaClipSpeedString);
			mediaClipSpeed = atof(mediaClipSpeedString);
			returnValue = mSegmentSuite->GetNodeProperty(
											mediaNodeID,
											kVideoSegmentProperty_Media_StreamFrameVidSubType,
											&mediaSubtype);
			returnValue = mSegmentSuite->GetNodeProperty(
											mediaNodeID,
											kVideoSegmentProperty_Media_InstanceString,
											&mediaPath);
			returnValue = mSegmentSuite->GetNodeProperty(
											mediaNodeID,
											kVideoSegmentProperty_Media_ClipID,
											&mediaClipIDString);
			returnValue = mSegmentSuite->GetNodeProperty(
											mediaNodeID,
											kVideoSegmentProperty_Media_StreamAlphaType,
											&mediaAlphaTypeString);
			if (returnValue == 0)
			{
				mediaAlphaType = atoi(mediaAlphaTypeString);
			}
			if (returnValue == 0)
			{
				mediaClipID = atoi(mediaClipIDString);
				segmentIt->clipID = mediaClipID;
			}
			returnValue = mSegmentSuite->GetNodeProperty(
											mediaNodeID,
											kVideoSegmentProperty_Media_IsOffline,
											&isOffline);

			// If speed is not 100%
			if (mediaClipSpeed < 0.99 ||
				mediaClipSpeed > 1.01)
			{
				for (int i = 0; i < indentation + 1; i++)
				{
					segmentIt->description.append(" ");
				}
				segmentIt->description.append("Media clip speed changed\n");
			}

			for (int i = 0; i < indentation + 1; i++)
			{
				segmentIt->description.append(" ");
			}
			if (mediaClipIDString)
			{
				segmentIt->description.append("Media ClipID: ");
				segmentIt->description.append(mediaClipIDString);
				segmentIt->description.append("\n");
			}

			// If media is online
			if (!returnValue && (atoi (isOffline) == 0))
			{
				for (int i = 0; i < indentation + 1; i++)
				{
					segmentIt->description.append(" ");
				}
				segmentIt->description.append(mediaPath);

				#ifdef PRWIN_ENV
				// Sample code to check mpegindex media accelerator for HDV files
				if (strcmp (mediaSubtype, "1297106247") == 0)
				{
					#define ACCEL_KEY L"mpgindex"
					prUTF16Char path[255], mediaPathW[255];
					prPluginID documentID, contentState;
					MultiByteToWideChar (CP_UTF8, 0, mediaPath, -1, mediaPathW, 255);
					mMediaAccelSuite->GetDocumentIDAndContentState (mediaPathW, kPrFalse, &documentID, &contentState);
					mMediaAccelSuite->FindPathInDatabase (255, path, &documentID, &contentState, ACCEL_KEY);
				}
				#endif

				mMemorySuite->PrDisposePtr(mediaSubtype);
				mMemorySuite->PrDisposePtr(mediaWidthString);
				mMemorySuite->PrDisposePtr(mediaHeightString);
				mMemorySuite->PrDisposePtr(mediaPARNumString);
				mMemorySuite->PrDisposePtr(mediaPARDenString);
				mMemorySuite->PrDisposePtr(mediaFrameRateString);
				mMemorySuite->PrDisposePtr(mediaClipSpeedString);
				mMemorySuite->PrDisposePtr(mediaPath);
				mMemorySuite->PrDisposePtr(mediaClipIDString);
				mMemorySuite->PrDisposePtr(mediaAlphaTypeString);
				mMemorySuite->PrDisposePtr(isOffline);

				returnValue = mSegmentSuite->GetNodeProperty(
					mediaNodeID,
					kVideoSegmentProperty_Media_StreamIsRollCrawl,
					&isRollCrawl);
				// Test code for RollCrawl suite
				if (!returnValue && atoi(isRollCrawl) != 0)
				{
					PrPixelFormat			pixelFormatArray[]	= {PrPixelFormat_BGRA_4444_8u};
					prSuiteError			returnValue			= suiteError_NoError;
					RollCrawlInfo			rollCrawlInfo;
					PPixHand				rollCrawlPPH;

					returnValue = mRollCrawlSuite->GetRollCrawlClipInfo(mediaClipID,
																		&rollCrawlInfo);
					if (!returnValue)
					{
						prBool isCrawl = !!(rollCrawlInfo.flags & prcCrawl);
						if (isCrawl)
						{
							segmentIt->description.append("\nCrawling title");
						}
						prBool isStartOnscreen = !!(rollCrawlInfo.flags & prcStartOnScreen);
						if (isStartOnscreen)
						{
							segmentIt->description.append("\nStarts onscreen");
						}
						prBool isEndOnscreen = !!(rollCrawlInfo.flags & prcEndOnScreen);
						if (isEndOnscreen)
						{
							segmentIt->description.append("\nEnds onscreen");
						}
						prBool isReverse = !!(rollCrawlInfo.flags & prcReverse);					
						if (isReverse)
						{
							segmentIt->description.append("\nDirection reversed");
						}
					}
					returnValue = mRollCrawlSuite->GetRollCrawlPage(mediaClipID,
																	0, // page
																	mediaWidth,
																	mediaHeight,
																	pixelFormatArray,
																	1,
																	mediaPARNum,
																	mediaPARDen,
																	&rollCrawlPPH);
					if (returnValue == suiteError_NoError)
					{
						mPPixSuite->Dispose(rollCrawlPPH);
					}
					mMemorySuite->PrDisposePtr(reinterpret_cast<char *>(isRollCrawl));
				}	
				segmentIt->description.append("\n");
			}
			// If media is offline
			else
			{
				segmentIt->description.append("Media Offline\n");
			}
		}
	}

	return result;
}


prMALError VideoSequenceParser::parseClipOperators (
	std::list<segmentInfo>::iterator	segmentIt,
	csSDK_int32							clipNodeID,
	csSDK_int32							indentation)
{
	prMALError	result					= malNoError,
				returnValue				= malNoError;
	csSDK_int32	clipNodeOperators		= 0,
				operatorNodeID			= 0,
				operatorNodeFlags		= 0,
				filterParamCount		= 0;
	char		operatorNodeType[kMaxNodeTypeStringSize];
	prPluginID	operatorNodeHash;
	PrTime		startTime				= 0,
				nextKeyframeTime		= 0;
	PrMemoryPtr	effectName, effectInstanceID, filterParams;
	PrParam		motionPositionParam, opacityLevelParam;

	mSegmentSuite->GetNodeOperatorCount(clipNodeID, &clipNodeOperators);
	// Iterate over the clip operators
	for (int operatorNodeIndex = 0; operatorNodeIndex < clipNodeOperators; operatorNodeIndex++)
	{
		for (int i = 0; i < indentation; i++)
		{
			segmentIt->description.append(" ");
		}
		segmentIt->description.append("Clip Operator: ");

		mSegmentSuite->AcquireOperatorNodeID(	clipNodeID,
												operatorNodeIndex,
												&operatorNodeID);
		mSegmentSuite->GetNodeInfo(	operatorNodeID,
									operatorNodeType,
									&operatorNodeHash,
									&operatorNodeFlags);
		if (strcmp(operatorNodeType, kVideoSegment_NodeType_Effect) == 0)
		{
			mSegmentSuite->GetNodeProperty(	operatorNodeID,
											kVideoSegmentProperty_Effect_FilterMatchName,
											&effectName);
			segmentIt->description.append(effectName);
			segmentIt->description.append("\n");
			mSegmentSuite->GetNodeProperty(operatorNodeID,
											kVideoSegmentProperty_Effect_RuntimeInstanceID,
											&effectInstanceID);
			mMemorySuite->PrDisposePtr(effectInstanceID);

			if (strcmp(effectName, "AE.ADBE Motion") == 0)
			{
				// Look at param values
				mSegmentSuite->GetNodeProperty(	operatorNodeID,
												kVideoSegmentProperty_Effect_FilterParams,
												&filterParams);
				mSegmentSuite->GetParamCount(operatorNodeID, &filterParamCount);
				if (filterParamCount >= 1)
				{
					mSegmentSuite->GetParam(operatorNodeID, 0, 0, &motionPositionParam);
					csSDK_int32 keyframeInterpMode = -1;
					returnValue = mSegmentSuite->GetNextKeyframeTime(operatorNodeID, 0, startTime, &nextKeyframeTime, &keyframeInterpMode);
				
					csSDK_int32 nextKeyframeInterpolationMode = -1;
					returnValue = mSegmentSuite->GetNextKeyframeTime(operatorNodeID, 0, nextKeyframeTime, &nextKeyframeTime, &nextKeyframeInterpolationMode);
				}
				mMemorySuite->PrDisposePtr(filterParams);
			}
			else if (strcmp(effectName, "AE.ADBE Opacity") == 0)
			{
				// Look at param values
				mSegmentSuite->GetNodeProperty(	operatorNodeID,
												kVideoSegmentProperty_Effect_FilterParams,
												&filterParams);
				mSegmentSuite->GetParamCount(operatorNodeID, &filterParamCount);
				if (filterParamCount >= 1)
				{
					mSegmentSuite->GetParam(operatorNodeID, 0, 0, &opacityLevelParam);
				}
				mMemorySuite->PrDisposePtr(filterParams);
			}
			mMemorySuite->PrDisposePtr(effectName);
		}

		mSegmentSuite->ReleaseVideoNodeID(operatorNodeID);
	}

	return result;
}


// Searches for the segment that covers the time requested
// If found, returns kFindSegmentSuccess and iterator points to segment requested
// If segment doesn't exist but there are other segments,
//  returns kFindSegmentMissing and iterator points to next segment
// If segment is past end of cutlist, returns kFindSegmentEnd
findSegmentReturnCode VideoSequenceParser::CutlistFindSegmentAtTime(
	PrTime								position,
	std::list<segmentInfo>::iterator	*segmentIt)
{  
	prBool found = kPrFalse;
	
	if(!mCutlist->empty())
	{
		// Search from beginning to end
		for (	*segmentIt = mCutlist->begin();
				found == kPrFalse && *segmentIt != mCutlist->end();
				(*segmentIt)++)
		{
			if (position < (*segmentIt)->endTime && position >= (*segmentIt)->startTime)
			{
				return kFindSegmentSuccess;
			}
			// Is the position in a gap between segments?
			else if (position < (*segmentIt)->startTime && *segmentIt != mCutlist->begin())
			{
				return kFindSegmentMissing;
			}
		}
	}
	return kFindSegmentEnd;
}


PrClipID VideoSequenceParser::FindClipIDAtTime(PrTime position)
{
	PrClipID							clipID = -1;
	std::list<segmentInfo>::iterator	segmentIt;
	findSegmentReturnCode				findSegmentResult;

	findSegmentResult = CutlistFindSegmentAtTime(position, &segmentIt);
	if (findSegmentResult == kFindSegmentSuccess)
	{
		clipID = segmentIt->clipID;
	}
	return clipID;
}
