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
/*******************************************************************/

#ifndef	PRSDKROLLCRAWLSUITE_H
#define PRSDKROLLCRAWLSUITE_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKPIXELFORMAT_H
#include "PrSDKPixelFormat.h"
#endif 

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKMALERRORS_H
#include "PrSDKMALErrors.h"
#endif

#ifndef PRSDKQUALITY_H
#include "PrSDKQuality.h"
#endif

#ifndef PRSDKTIMESUITE_H
#include "PrSDKTimeSuite.h"
#endif

#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif

#define kPrSDKRollCrawlSuite			"Premiere RollCrawl Suite"
#define kPrSDKRollCrawlSuiteVersion		1

	
enum
{
	prcRoll              = csSDK_uint32(0x00000000),  // default
	prcCrawl             = csSDK_uint32(0x00000001),
	prcStartOffScreen    = csSDK_uint32(0x00000000),  // default - start the roll off the bottom of the screen
	prcStartOnScreen     = csSDK_uint32(0x00000002),
	prcEndOffScreen      = csSDK_uint32(0x00000000),  // default - end off the top of the screen
	prcEndOnScreen       = csSDK_uint32(0x00000004),
	prcForward           = csSDK_uint32(0x00000000),  // default
	prcReverse           = csSDK_uint32(0x00000008)   // ie crawl that moves left to right (ie reads right to left)
};

typedef struct
{
	unsigned int flags; // use above enum (roll/crawl, start/end on/off, etc etc etc)

	int length;		// # of scanlines for a roll, or width in pixels for a crawl.  
					// Note that this is NOT necessarily a multiple of the page size. (ie last page may not be full size)

	int easeIn;     // # of frames over which the roll/crawl accelerates to get up to speed
	int easeOut;	// # of frames to decellerate at the end

	int preRoll;	// # of frames to pause the first frame for 
					// (note that preRoll + tiStartOffScrren doesn't make much sense, because
					// you would just be holding a black frame, but oh well)
	int postRoll;   // # of frames to hold the last frame/page on screen (postRoll + !iEndOffScreen also a bit inconsistent)

	// [TODO]
	// not yet implemented, but should be for 7.5 - some sort of fading along the top/bottom (or left/right) of a roll/crawl
	// these are just place-holders for now - not yet sure what the real values will need to be (and whether 
	// they'll be percentages or scanlines values, etc)
/*	int fadeTop;
	int fadeTopWidth;
	int fadeBottom;
	int fadeBottomWidth;*/
} RollCrawlInfo;

typedef struct 
{
	// For a given clip that is a roll or crawl, return information about it. See above for
	// more information about the RollCrawlInfo that is returned.
	prSuiteError (*GetRollCrawlClipInfo)(
							PrClipID inClipData,								// clipData from a prtFileRec
							RollCrawlInfo* outRollCrawlInfo						// filled by the host if successful.
							);

	prSuiteError (*GetRollCrawlPage)(
							PrClipID		inClipData,							// clipData from a prtFileRec
							int				inPage,								// page to retrieve
							int				inWidth,							// width of pixels
							int				inHeight,							// height
							PrPixelFormat*	inRequestedPixelFormatArray,		// requested pixel formats. MUST include BGRA as the last element.
							csSDK_int32		inRequestedPixelFormatArrayCount,	// how many formats in the array.
							csSDK_uint32	inPixelAspectRatioNumerator,		// aspect ratio, e.g. numerator = 648, denominator = 720
							csSDK_uint32	inPixelAspectRatioDenominator,		// 
							PPixHand*		outRenderedFrame					// rendered frame from host if succeeded.
																				// Must be disposed of using Dispose in the PPix suite.
							);

} PrSDKRollCrawlSuite;

#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif // PRSDKROLLCRAWLSUITE_H
