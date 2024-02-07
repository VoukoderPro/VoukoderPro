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

#ifndef PRSDKIMPORTERSHARED_H
#define PRSDKIMPORTERSHARED_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKPIXELFORMAT_H
#include "PrSDKPixelFormat.h"
#endif

#ifndef PRSDKQUALITY_H
#include "PrSDKQuality.h"
#endif

#ifndef PRSDKTIMESUITE_H
#include "PrSDKTimeSuite.h"
#endif

#ifndef PRSDKCOLORSPACES_H
#include "PrSDKColorSpaces.h"
#endif

#pragma pack(push, 1)

typedef enum
{
	imRenderIntent_Unknown = -1,
	imRenderIntent_Export = 0,
	imRenderIntent_Stopped,
	imRenderIntent_Scrubbing,
	imRenderIntent_Preroll,
	imRenderIntent_Playing,
	imRenderIntent_SpeculativePrefetch,
	imRenderIntent_Thumbnail,
	imRenderIntent_Analysis,
	imRenderIntent_ExportPreview,
	imRenderIntent_ExportProxies,
} imRenderIntent;
typedef struct 
{
	imRenderIntent inIntent;
	double inPlaybackRatio; // 1.0 means full framerate, lower values indicate deteriorating playback
	double inPlaybackRate; // 1.0 means 1x forward, -1.0 means 1x backward, etc.
} imRenderContext;
#define imPlaybackRatio_Normal 1.0
#define imPlaybackRate_Stopped 0.0
inline imRenderContext imDefaultRenderContext()
{
	imRenderContext context = {imRenderIntent_Unknown, imPlaybackRatio_Normal, imPlaybackRate_Stopped};
	return context;
}

/**
**	This struct defines a specific frame format that is being requested
**	fromt the importer. Any member can be 0, which means that any value
**	is an acceptable match. For instance, the host might ask for a specific
**	width and height, but pass 0 as the pixel format, meaning it can accept
**	any pixel format.
*/
typedef struct
{
	csSDK_int32			inFrameWidth;
	csSDK_int32			inFrameHeight;
	PrPixelFormat		inPixelFormat;
} imFrameFormat;

typedef struct
{
	void*				inPrivateData;
	csSDK_int32			currentStreamIdx;				//	Added in Premiere Pro 3.0 to maintain struct alignment, turned into streamIdx for CS6
	PrTime				inFrameTime;
	imFrameFormat*		inFrameFormats;					//	An array of requested frame format, in order of preference.
														//	If NULL, then any format is acceptable.
	csSDK_int32			inNumFrameFormats;				//	The number of frame formats in the array.
	bool				removePulldown;					//  pulldown should be removed if pixel format supports that.
	char				unused2[3];						//	Added in Premiere Pro 3.0 to maintain struct alignment
	PPixHand*			outFrame;						//	A pointer to the PPixHandle holding the requested frame.
	void				*prefs;							// persistent data from imGetSettings
	csSDK_int32			prefsSize;						//  size of persistent data from imGetSettings
	// As of Pr 13.0, this string may be a special descriptor for over-ranged 32f
	// full range Rec. 709 space (see PrSDKColorSpaces.h).
	// Color-managed importer should test for it and, if found, use this space
	// as a connection space between media and the host. It will be passed if
	// the host does not understand media native space previously reported
	// by the importer.
	PrSDKString			selectedColorProfileName;		//	Added CS5.5 The name of the selected color profile, if the importer supports color management; as of Pr 13, importer that supports full color management should check this string against kPrOverranged709 to know if it needs to do its own conversion from media space into OR709
	PrRenderQuality		inQuality;
	imRenderContext		inRenderContext;
	// new in Pr 13.0
	PrSDKColorSpaceID	opaqueColorSpaceIdentifier;		// opaque representation of frame's color spaces; to be passed back to the host through other suites requiring color space info by the importer if it supports full color management with color spaces
} imSourceVideoRec;

typedef struct 
{
	PrPixelFormat		inPixelFormat;
	csSDK_int32			inWidth;
	csSDK_int32			inHeight;
	csSDK_int32			inPixelAspectRatioNumerator;
	csSDK_int32			inPixelAspectRatioDenominator;
	prFieldType			inFieldType;
	PrRenderQuality		inQuality;
} ClipFrameDescriptor;

#pragma pack(pop)

#endif
