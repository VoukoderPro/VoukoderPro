/*************************************************************************
*
* ADOBE CONFIDENTIAL
* ___________________
*
*  Copyright 2010 Adobe Systems Incorporated
*  All Rights Reserved.
*
* NOTICE:  All information contained herein is, and remains
* the property of Adobe Systems Incorporated and its suppliers,
* if any.  The intellectual and technical concepts contained
* herein are proprietary to Adobe Systems Incorporated and its
* suppliers and are protected by trade secret or copyright law.
* Dissemination of this information or reproduction of this material
* is strictly forbidden unless prior written permission is obtained
* from Adobe Systems Incorporated.
**************************************************************************/

//	This file is part of the public Premiere SDK.

#ifndef PRSDKCAPTIONINGSUITE_H
#define PRSDKCAPTIONINGSUITE_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKMALERRORS_H
#include "PrSDKMALErrors.h"
#endif

#ifndef PRSDKTIMESUITE_H
#include "PrSDKTimeSuite.h"
#endif

#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif

#define kPrSDKCaptioningSuite				"Captioning Suite"
#define kPrSDKCaptioningSuiteVersion		3

/*******************************************************************************************
**	This suite provides callbacks to support the injecting of close caption data into native
**	SDI, HD-SDI, and DV streams.  
**
**	The first block of calls is for the CEA-708 (and CEA-608) handling, the second block handles
**	Australian Captions (OP-47)
**
**  1. Block (CEA-708 handling):
**	The callbacks can provide both CEA-608 and CEA-708 data, simultaneously.  For example, if the 
**	user has imported one format such as CEA-608 from a SCC file to a Timeline, both CEA-608 and 
**	CEA-708 callbacks will function as Premiere converts from CEA-608 to CEA-708 data on the fly.
**	If the user has attached an MCC (CEA-708) file, Premiere will first attempt to extract the CEA-608
**	data from within the ANC packet and return that data, before doing the conversion from CEA-708
**	to CEA-608.
**
**	The callbacks are documented below, but can be grouped as such:
**
**	HasCaptionData()
**		Specifies if caption data is present.
**
**	Get608CaptionDataMaxSize(), GetNext608CaptionData(), GetPrevious608CaptionData()
**		The data returned is of the format specified in CEA-608-E, and is either 0, 2 or 4 
**		bytes long.  The data returned could be written into the VAUX Closed Caption pack (0x65) of 
**		a DV frame, or other appropriate stream.
**
**	Get708CaptionDataMaxSize(), GetNext708CaptionData(), GetPrevious708CaptionData()
**		The data returned is an ANC packet as defined in SMPTE 291M-1998.  The packet will usually
**		be a CDP (SMPTE 334-2) packet.  The CDP could contain CEA-608 and CEA-708 data, as well as
**		other types of ANC data, such as AFD.
**
**	GetCaptionInfo()
**		Deprecated after CS6
**
**	The data returned is mapped 1 to 1 with the timecode returned.  The data is appropriate for injecting
**	into a stream playing at 1x speed.  No adjustment or mapping is made for other playback speeds.  If it's 
**	desirable to display a caption while the playback is paused, it's the responsability of the client
**	to cache (or fetch) the previous data, and clock it out appropriately.
**
**	Definitions:
**		SCC file:	A file with file extension *.scc, also known as a Scenarist Caption file or DVD Caption 
**					file.  The file contains a start timecode on each line and is followed by CEA-608 data.
**		MCC file:	A file with file extension *.mcc, also known as a MacCaption VANC file.  Each line of
**					the file starts with a timecode, and then a ANC (SMPTE 291M) packet in a form that has
**					been compressed with LZW compression, as described in comments at the head of the file.
**
**
**  2. Block (OP-47 handling):
**	HasCaptionXDCAMEmbedableData()
**		It checks, if the given timeline has embedable captions. It fills two bools: "has CEA-708" captions
**		(This includes also CEA-608 which can be encoded in CEA 708) and "has OP47 captions".
**
**	GetOP47MaxPayloadSize(), GetNextOP47CaptionData
**		The data returned is an ANC packet as defined in in SMPTE 291M-1998. The packet will usually be
**		a SMPTE RDD-8 (OP-47 packet). The GetOP47MaxPayloadSize returns the size that is needed to get such
**		a data packet. To get the content for OP-47 right, you have to call the GetNextOP47CaptionData
**		two times for each frame (first packet and second packet), because for OP-47 we fill the info into
**		two packets (e.g. for line 19 and 582). 
**
**	3. Block (Respect encoding border):
**	HasCaptionXDCAMEmbedableDataInRange()
**		Same as HasCaptionXDCAMEmbedableData but with additional params to set the encoding in and out points
**
*******************************************************************************************/


typedef struct  
{
	/*
	**	Used to determine if the timeline currently has caption data.  The value could change
	**	at any time due to the user attaching caption data.
	**
	**	@param	inTimelineID			the timeline instance ID used for all captioning suite communication,
	**									it is passed into the player instance in the pmNewListParms struct.
	**	@param	outHasCaptionData		true if the user has attached caption data to the timeline.
	*/
	prSuiteError (*HasCaptionData)(
		PrTimelineID inTimelineID,
		bool* outHasCaptionData);
	
	/*
	**	Obtain the maximum size of the buffer to allocate to hold the data obtained when
	**	calling the GetNext608CaptionData and GetPrevious608CaptionData callbacks.
	**
	**	@param	inTimelineID			the timeline instance ID used for all captioning suite communication,
	**									it is passed into the player instance in the pmNewListParms struct.
	**	@param outCaptionDataSize		is the maximum size needed to hold the data.
	*/
	prSuiteError (*Get608CaptionDataMaxSize)(
		PrTimelineID inTimelineID,
		size_t* outCaptionDataSize);

	/*
	** 	Obtain the next closed caption data using the time provided via inOutCaptionTime as a starting point.  The
	**	actual time of the data will be returned in the inOutCaptionTime parameter.
	**
	**	@param	inTimelineID			the timeline instance ID used for all captioning suite communication,
	**									it is passed into the player instance in the pmNewListParms struct.
	**	@param	inOutCaptionTime		the starting time to search from.  The returned time is the time
	**									of the caption data.  The time returned will be no earlier than the time provided.
	**	@param	outCaptionData			the caption data to be written to line 21, or injected into a DV frame.
	**									the buffer is allocated/disposed by the client of size returned by Get608CaptionDataMaxSize().
	**	@param	outCaptionDataSize		the actual number of bytes of data written to outCaptionData.
	**
	**	@return suiteError_NoMoreData if no more caption data
	*/
	prSuiteError (*GetNext608CaptionData)(
		PrTimelineID inTimelineID,
		PrTime* inOutCaptionTime,
		char* outCaptionData,
		size_t* outCaptionDataSize);

	/*
	** 	Obtain the previous closed caption data using the time provided via inOutCaptionTime as a starting point.  The
	**	actual time of the data will be returned in the inOutCaptionTime parameter.
	**
	**	@param	inTimelineID			the timeline instance ID used for all captioning suite communication,
	**									it is passed into the player instance in the pmNewListParms struct.
	**	@param	inOutCaptionTime		the starting time to search from.  The returned time is the time
	**									of the caption data.  The time returned will be no later than the time provided.
	**	@param	outCaptionData			the caption data to be written to line 21, or injected into a DV frame.
	**									the buffer is allocated/disposed by the client of size returned by Get608CaptionDataMaxSize().
	**	@param	outCaptionDataSize		the actual number of bytes of data written to outCaptionData.
	**
	**	@return suiteError_NoMoreData if no more caption data
	*/
	prSuiteError (*GetPrevious608CaptionData)(
		PrTimelineID inTimelineID,
		PrTime* inOutCaptionTime,
		char* outCaptionData,
		size_t* outCaptionDataSize);

	/*
	**	Obtain the maximum size of the buffer to allocate to hold the data obtained when
	**	calling the GetNext708CaptionData and GetPrevious708CaptionData callbacks.
	**
	**	@param	inTimelineID			the timeline instance ID used for all captioning suite communication,
	**									it is passed into the player instance in the pmNewListParms struct.
	**	@param outCaptionDataSize		is the maximum size needed to hold the data.
	*/
	prSuiteError (*Get708CaptionDataMaxSize)(
		PrTimelineID inTimelineID,
		size_t* outCaptionDataSize);

	/*
	** 	Obtain the next closed caption data using the time provided via inOutCaptionTime as a starting point.  The
	**	actual time of the data will be returned in the inOutCaptionTime parameter.
	**
	**	@param	inTimelineID			the timeline instance ID used for all captioning suite communication,
	**									it is passed into the player instance in the pmNewListParms struct.
	**	@param	inOutCaptionTime		the starting time to search from.  The returned time is the time
	**									of the caption data.  The time returned will be no earlier than the time provided.
	**	@param	outCaptionData			the caption data to be written to the VANC section of SDI or HD-SDI.
	**									the buffer is allocated/disposed by the client of size returned by Get708CaptionDataMaxSize().
	**	@param	outCaptionDataSize		the actual number of bytes of data written to outCaptionData.
	**
	**	@return suiteError_NoMoreData if no more caption data
	*/
	prSuiteError (*GetNext708CaptionData)(
		PrTimelineID inTimelineID,
		PrTime* inOutCaptionTime,
		char* outCaptionData,
		size_t* outCaptionDataSize);

	/*
	** 	Obtain the previous closed caption data using the time provided actual time of the data as a starting point.  The
	**	actual time of the data will be returned in the inOutCaptionTime parameter.
	**
	**	@param	inTimelineID			the timeline instance ID used for all captioning suite communication,
	**									it is passed into the player instance in the pmNewListParms struct.
	**	@param	inOutCaptionTime		the starting time to search from.  The returned time is the time
	**									of the caption data.  The time returned will be no later than the time provided.
	**	@param	outCaptionData			the caption data to be written to the VANC section of SDI or HD-SDI.
	**									the buffer is allocated/disposed by the client of size returned by Get708CaptionDataMaxSize().
	**	@param	outCaptionDataSize		the actual number of bytes of data written to outCaptionData.
	**
	**	@return suiteError_NoMoreData if no more caption data
	*/
	prSuiteError (*GetPrevious708CaptionData)(
		PrTimelineID inTimelineID,
		PrTime* inOutCaptionTime,
		char* outCaptionData,
		size_t* outCaptionDataSize);

	/*
	**	DEPRECATED after CS6
	*/
	prSuiteError (*GetCaptionInfo)(
		PrTimelineID inTimelineID,
		PrSDKString* outFileFormat,
		PrSDKString* outCaptionUUID,
		PrSDKString* outCreationProgram,
		PrSDKString* outCreationDate,
		PrSDKString* outCreationTime);

	/*
	**	Used to determine if the timeline currently has caption data that can be embedded to XDCAMHD.
	**
	**	@param	inTimelineID			the timeline instance ID used for all captioning suite communication,
	**									it is passed into the player instance in the pmNewListParms struct.
	**	@param	outHas708Data			Indicates if we have any CEA-708 compatible (CEA-708 or CEA-608) caption in timeline.
	**	@param	outHasOP47Data			Indicates if we have OP-47 captions in timeline.
	*/
	prSuiteError (*HasCaptionXDCAMEmbedableData)(
		PrTimelineID inTimelineID,
		bool* outHas708Data,
		bool* outHasOP47Data);
	
	/*
	**	Obtain the maximum size of the payload to allocate to hold the data obtained when
	**	calling the GetNextOP47CaptionData callbacks.
	**
	**	@param	outCaptionDataSize		is the maximum size needed to hold the data.
	*/
	prSuiteError (*GetOP47MaxPayloadSize)(
		size_t* outCaptionDataSize);
	
	/*
	** 	Obtain the next closed caption data using the time provided via inOutCaptionTime as a starting point.  The
	**	actual time of the data will be returned in the inOutCaptionTime parameter.
	**
	**	@param	inTimelineID			the timeline instance ID used for all captioning suite communication,
	**									it is passed into the player instance in the pmNewListParms struct.
	**	@param	inStartEncodeTime		Is currently used to determine the index of the frame data.
	**	@param	inOutCaptionTime		For this time, the data is get (OP-47 fills every frame with its data) 
	**									If you call it with option inSecondPacket, one frame is added to it.
	**	@param	inSecondPacket			indicates if we want the first or the second packet info. (OP-47 fills
	**									two packets for each frame, the data have a running ID (so the data in
	**									first packet has always an even and the second packet the next following
	**									odd number.
	**	@param	outCaptionData			the caption data to be written to the VANC section of SDI or HD-SDI.
	**									the buffer is allocated/disposed by the client of size returned by Get708CaptionDataMaxSize().
	**	@param	outCaptionDataSize		the actual number of bytes of data written to outCaptionData.
	**
	**	@return suiteError_NoMoreData if no more caption data
	*/
	prSuiteError (*GetNextOP47CaptionData)(
		PrTimelineID inTimelineID,
		PrTime inStartEncodeTime,
		PrTime* inOutCaptionTime,
		bool inSecondPacket,
		char* outCaptionData,
		size_t* outCaptionDataSize);

	/*
	**	Used to determine if the timeline currently has caption data that can be embedded to XDCAMHD.
	**
	**	@param	inTimelineID			the timeline instance ID used for all captioning suite communication,
	**									it is passed into the player instance in the pmNewListParms struct.
	**	@param	inStartEncodeTime		Start of area to encode.
	**	@param	inEndEncodeTime			End of area to encode.
	**	@param	outHas708Data			Indicates if we have any CEA-708 compatible (CEA-708 or CEA-608) caption in timeline.
	**	@param	outHasOP47Data			Indicates if we have OP-47 captions in timeline.
	*/
	prSuiteError(*HasCaptionXDCAMEmbedableDataInRange)(
		PrTimelineID inTimelineID,
		PrTime inStartEncodeTime,
		PrTime inEndEncodeTime,
		bool* outHas708Data,
		bool* outHasOP47Data);

} PrSDKCaptioningSuite;

#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif
