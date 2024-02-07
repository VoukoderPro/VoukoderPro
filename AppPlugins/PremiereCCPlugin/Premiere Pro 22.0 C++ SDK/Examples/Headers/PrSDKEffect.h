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

#ifndef PRSDKEFFECT_H
#define PRSDKEFFECT_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKPLUGSUITES_H
#include "PrSDKPlugSuites.h"
#endif

#ifndef PRSDKSTRUCTS_H
#include "PrSDKStructs.h"
#endif

#ifndef PRSDKENTRY_H
#include "PrSDKEntry.h"
#endif

#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************
**	Version
*/

#define VIDEO_FILTER_VERSION_8	8	// Pro 1.5 version
#define VIDEO_FILTER_VERSION_9	9	// Pro 2.0 version
#define VIDEO_FILTER_VERSION_10	10	// Pro 3.0 version
#define VIDEO_FILTER_VERSION_11	11	// CS5 version
#define VIDEO_FILTER_VERSION_12	12	// CS5.5 version
#define	kVideoFilterVersion		VIDEO_FILTER_VERSION_12

#define TRANSITION_VERSION_8	8	// Pro 1.5 version
#define TRANSITION_VERSION_9	9	// Pro 2.0 version
#define TRANSITION_VERSION_10	10	// Pro 3.0 version
#define TRANSITION_VERSION_11	11	// CS5 version
#define TRANSITION_VERSION_12	12	// CS5 version
#define TRANSITION_VERSION_13	12	// CS7 version
#define	kEffectVersion			TRANSITION_VERSION_13


/****************************************************************
**	Error Definitions
*/

// Effect error Return codes

enum PrTransitionReturnValue {
	esNoErr = 0,				// No Error
	esBadFormatIndex = 1,
	esDoNotCacheOnLoad = 2,
	esUnsupported    = -100,	// Unsupported selector

	esErr_Size			=	kPrForceEnumToIntValue,
};

// Filter error Return codes

enum PrFilterReturnValue {
	fsNoErr = 0,				// No Error
	fsBadFormatIndex = 1,
	fsDoNotCacheOnLoad = 2,
	fsHasNoSetupDialog = 3,		// Premiere Pro 3.0: possible return value for fsHasSetupDialog (otherwise fsNoErr or fsUnsupported)
	fsUnsupported    = -100,	// Unsupported selector

	fsErr_Size			=	kPrForceEnumToIntValue
};


/****************************************************************
**	Type Definitions
*/

#define PRFILTERENTRY short 


/****************************************************************
**	Enumerations
*/

/**
**	These flags are passed in the flags field of the Effect and Video recs.
*/
enum
{
	kEffectFlags_DraftQuality	= 0x00000001,				// If set, the render is for draft quality
	kEffectFlags_TransitionHasIncomingClip = 0x00000002,	// If set, the transition has an incoming clip
	kEffectFlags_TransitionHasOutgoingClip = 0x00000004,	// If set, the transition has an outgoing clip
	kEffectFlags_ForceEnumSize	= kPrForceEnumToIntValue
};

// The following effect corner bits are also defined
// in PIResDefines.h, but as macros rather than as an enum.
// If that file is included first, bad things happen.
#ifndef bitTop

// Effect Corner Bits
enum {
	bitTop =				0x01,
	bitRight =				0x02,
	bitBottom =				0x04,
	bitLeft =				0x08,
	bitUpperRight =			0x10,
	bitLowerRight =			0x20,
	bitLowerLeft =			0x40,
	bitUpperLeft =			0x80,
	
	bit_Size =				kPrForceEnumToIntValue
};

#endif // bitTop

/*	Important! 

	When responding to esCanHandlePAR or fsCanHandlePAR,
	return an OR'd-together combination of the following
	flags (from prSDKEffect.h).
	
	prEffectCanHandlePAR	0x4000
	prEffectUnityPARSetup	0x0001
	prEffectUnityPARExecute	0x0002

*/
// return bits for esCanHandlePAR and fsCanHandlePAR selectors
enum
{
	prEffectCanHandlePAR = 0x4000,	/* selector is implemented */
	prEffectUnityPARSetup = 1,		/* requires unity PAR for setup */
	prEffectUnityPARExecute = 2,		/* requires unity PAR for execute */

	prEffect_Size =				kPrForceEnumToIntValue
};

enum
{
	gvFieldsFirst   =       0x1000,         // TRUE if we're passing the dominant field to this filter. 4.2 changes -- njs.
	gvFieldsOdd     =       0x0200,
	gvFieldsEven    =       0x0100,
	
	prEffectGetVideo_Size =				kPrForceEnumToIntValue
};


/****************************************************************
**	Callback Functions
*/

// Transition callback prototype

typedef short (*FXCallBackProcPtr)(	csSDK_int32	frame, 
									short		track, 
									PPixHand	thePort, 
									prRect		*theBox, 
									PrMemoryHandle		privateData);
// Filter callback prototype

typedef short (CALLBACK *VFilterCallBackProcPtr)(	csSDK_int32	frame, 
													PPixHand	thePort,
													prRect		*theBox, 
													PrMemoryHandle		privateData);


/****************************************************************
**	Structures
*/

// Transition Data Structure

typedef struct {
	PrMemoryHandle			specsHandle;
	PPixHand				source1;		// source pixels 1
	PPixHand				source2;		// source pixels 2
	PPixHand				destination;	// Destination pixels
	csSDK_int32				part;			// part / total = % complete
	csSDK_int32				total;
	char					previewing;		// in preview mode?
	unsigned char			arrowFlags;		// flags for direction arrows
	char					reverse;		// is effect being reversed?
	char					source;			// are sources swapped?
	prPoint					start;			// starting point for effect
	prPoint					end;			// ending point for effect
	prPoint					center;			// the reference center point
	void					*privateData;	// Editor private data handle
	FXCallBackProcPtr		callBack;		// callback, not valid if null
	BottleRec				*bottleNecks;	// botleneck callback routines
	short					version;		// version of this record (kEffectVersion)
	short					sizeFlags;
	csSDK_int32				flags;			// Effect flags (see above)
	TDB_TimeRecord			*tdb;
	piSuitesPtr				piSuites;
	PrTimelineID			timelineData;
	PrMemoryHandle			instanceData;
	char					altName[MAX_FXALIAS];	// alternate filter name - new for 6.0
	PrPixelFormat			pixelFormatSupported;	// return fourCC of pixel type supported - new for 7.0
	csSDK_int32				pixelFormatIndex;		// index of query of fourCC of pixel type supported - new for 7.0
	csSDK_uint32			instanceID;				// The runtime instance id - new for 7.0
	TDB_TimeRecord			tdbTimelineLocation;	// On fsSetup only, this specifies the location
													// in the timeline of the transition - new for 7.0
	csSDK_int32				sessionPluginID;		// ID for specific plugin instance, valid for session; new for 7.5/1.5
} EffectRecord, **EffectHandle;

// Video Filter Data Structure

typedef struct {
	PrMemoryHandle				specsHandle;
	PPixHand					source;
	PPixHand					destination;
	csSDK_int32					part;
	csSDK_int32					total;
	char						previewing;
	void						*privateData;
	VFilterCallBackProcPtr		callBack;
	BottleRec					*bottleNecks;
	short						version;					// Version of this record (kVideoFilterVersion)
	short						sizeFlags;
	csSDK_int32					flags;						// Effect flags (see above)
	TDB_TimeRecord			   *tdb;
	PrMemoryHandle				instanceData;
	piSuitesPtr					piSuites;
	PrTimelineID				timelineData;
	char						altName[MAX_FXALIAS];		// Alternate Filter name - new for 6.0
	PrPixelFormat				pixelFormatSupported;		// return fourCC of pixel type supported - new for 7.0
	csSDK_int32					pixelFormatIndex;			// index of query of fourCC of pixel type supported - new for 7.0
	csSDK_uint32				instanceID;					// The runtime instance id - new for 7.0
	TDB_TimeRecord				tdbTimelineLocation;		// On fsSetup only, this specifies the location of the clip in the timeline
															// that the filter is applied to. - new for 7.0
	csSDK_int32					sessionPluginID;			// ID for specific plugin instance, valid for session; new for 7.5/1.5
} VideoRecord, **VideoHandle;


/****************************************************************
**	Entry Point
*/
typedef PRFILTERENTRY (*FilterProcPtr)(short selector, PrMemoryHandle theData);
typedef PRFILTERENTRY (*PRVideoFilterEntry)(
	csSDK_int32 inID,
	short selector,
	PrMemoryHandle theData);
typedef PRFILTERENTRY (*EffectProcPtr)(short selector, EffectHandle theData);
typedef PRFILTERENTRY (*PRVideoTransitionEntry)(
	csSDK_int32 inID,
	short inSelector, 
	EffectHandle inData);

/****************************************************************
**	Selectors
*/

// Selector messages
enum PrTransitionSelector {
	esExecute = 0,
	esSetup,
	esUnused1,					// Not supported
	esUnused2,					// Not supported
	esUnused3,					// Not supported
	esDisposeData,
	esCanHandlePAR,				// Premiere 6.0
	esGetPixelFormatsSupported,	// 7.0 - used to get pixel formats supported, return an array of int32s (i.e. fourCC's), Premiere will dispose of array
	esCacheOnLoad,				// If the effect returns esDoNotCacheOnLoad, then Premiere will load the dll from disk on startup
								// every time. Any other return value will maintain the Pro 1.5 behavior of lazy loading of dlls.
								// inData will always be NULL for this call.

	esSelector_Size =				kPrForceEnumToIntValue
};

// Selector messages
enum PrFilterSelector {
	fsExecute = 0,
	fsSetup,
	fsAbout,					// Not supported
	fsDisposeData,
	fsCanHandlePAR,		
	fsInitSpec,					// 6.0 - Init default spec - no UI
	fsGetPixelFormatsSupported,	// 7.0 -- used to get pixel formats supported
	fsCacheOnLoad,				// If the filter returns fsDoNotCacheOnLoad, then Premiere will load the dll from disk on startup
								// every time. Any other return value will maintain the Pro 1.5 behavior of lazy loading of dlls.
								// inData will always be NULL for this call.
	fsHasSetupDialog,			// Premiere Pro 3.0 - used to definitely state whether or not a plugin has a setup dialog to display

	fsSelector_Size =				kPrForceEnumToIntValue
};


#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif /* PRSDKEFFECT_H */
