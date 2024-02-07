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

#include "SPBasic.h"
#include "PrSDKVideoSegmentSuite.h"
#include "PrSDKVideoSegmentProperties.h"
#include "PrSDKMemoryManagerSuite.h"
#include "PrSDKPPixSuite.h"
#include "PrSDKMediaAcceleratorSuite.h"
#include "PrSDKRollCrawlSuite.h"

#include <string>
#include <list>

/*
** A segment that has the following value in the "end" field is
**  usually a NULL segment that signifies the end of the timeline
**  (no more material to the right of the "inListPos" field of the
**  segment).
*/
#define kPrSDKEndOfTimeline			91445760000000000LL

typedef enum 
{
	kFindSegmentMissing = 0,
	kFindSegmentSuccess,
	kFindSegmentEnd
} findSegmentReturnCode;


class segmentInfo
{
public:
	prPluginID		segmentHash;	// hash value
	std::string		description;	// our actual segment info (just text in our case)
	PrTime			startTime;		// the segment's trim in
	PrTime			endTime;		// the segment's trim out
	csSDK_int32		nodeID;			// the nodeID of the segment
	PrClipID		clipID;			// the clipID of the media, if only one piece of media in the segment

	segmentInfo();
};


class VideoSequenceParser
{
public:
	VideoSequenceParser (
		SPBasicSuite	*spBasic,
		int				width,
		int				height,
		csSDK_uint32	PARNum,
		csSDK_uint32	PARDen);

	~VideoSequenceParser ();

	prMALError ParseSequence (PrTimelineID timelineID, csSDK_int32 indentation = 0);

	findSegmentReturnCode CutlistFindSegmentAtTime(
		PrTime								position,
		std::list<segmentInfo>::iterator	*segmentIt);
	
	PrClipID FindClipIDAtTime(PrTime position);

private:
		std::list<segmentInfo>		*mCutlist;
		int							mWidth;
		int							mHeight;
		csSDK_uint32				mPARNum;
		csSDK_uint32				mPARDen;
		SPBasicSuite				*mSPBasic;
		PrSDKVideoSegmentSuite		*mSegmentSuite;
		PrSDKMemoryManagerSuite		*mMemorySuite;
		PrSDKMediaAcceleratorSuite	*mMediaAccelSuite;
		PrSDKPPixSuite				*mPPixSuite;
		PrSDKRollCrawlSuite			*mRollCrawlSuite;

	prMALError parseCompositor (
		std::list<segmentInfo>::iterator	segmentIt,
		csSDK_int32							nodeID,
		csSDK_int32							indentation);

	prMALError parseMulticam (
		std::list<segmentInfo>::iterator	segmentIt,
		csSDK_int32							nodeID,
		csSDK_int32							indentation);

	prMALError parseTransition (
		std::list<segmentInfo>::iterator	segmentIt,
		csSDK_int32							transitionNodeID,
		csSDK_int32							indentation);

	prMALError parseClips (
		std::list<segmentInfo>::iterator	segmentIt,
		csSDK_int32							clipNodeID,
		csSDK_int32							indentation);

	prMALError parseMedia (
		std::list<segmentInfo>::iterator	segmentIt,
		csSDK_int32							mediaNodeID,
		csSDK_int32							indentation);

	prMALError parseClipOperators (
		std::list<segmentInfo>::iterator	segmentIt,
		csSDK_int32							clipNodeID,
		csSDK_int32							indentation);
};
