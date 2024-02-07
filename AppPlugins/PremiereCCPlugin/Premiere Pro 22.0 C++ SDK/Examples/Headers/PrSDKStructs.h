/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2000 Adobe Systems Incorporated                       */
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

/* prSDKStructs.h

Defines structures used by the various Premiere interfaces - PPix, TDB, BottleRec

*/
#ifndef PRSDKSTRUCTS_H
#define PRSDKSTRUCTS_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#pragma pack(push, 1)

typedef csSDK_int32	TDB_Time;
typedef csSDK_int32	TDB_SampSize;
typedef csSDK_uint32 TDB_TimeScale;

typedef struct {
	TDB_Time		value;
	TDB_TimeScale	scale;
	TDB_SampSize	sampleSize;
} TDB_TimeRecord;

typedef struct {
		csSDK_int32    x;
		csSDK_int32    y;
} LongPoint;	// change name???


typedef struct
{
	PrMemoryPtr		srcPtr;					// Pointer to total source block
	csSDK_int32		srcSampleOffset;		// Offset to important part of source (in samps)
	csSDK_int32		srcTotalSampleCount;	// Total samples in source block
	csSDK_int32		srcSampleCount;			// Len of this part of source block (in samples)
	short			srcFlags;				// Format of source block
	PrMemoryPtr		dstPtr;					// Pointer to destination block
	csSDK_int32		dstSampleCount;			// Len of destination block (in samples)
	short			dstFlags;				// Format of destination block

	csSDK_int32		srcRate;
	csSDK_int32		dstRate;
	csSDK_int32		prevDstSamples;
	csSDK_int32		prevSrcSamples;
} ResampleRec;

typedef struct
{
	PrMemoryPtr		srcPtr;
	csSDK_int32		srcSampleCount;
	short			srcFlags;

	PrMemoryPtr		dstPtr;
	csSDK_int32		dstSampleCount;
	short			dstFlags;

	csSDK_int32		convertOffset;
	csSDK_int32		step;
} AudioMungeRec;

typedef void (*StretchBitsPtr)(	PPixHand	srcPix, 
								PPixHand	dstPix, 
								prRect		*srcRect,
								prRect		*dstRect, 
								int			mode, 
								prRgn		rgn);	// Unused on MacOS

typedef void (*AudStretchPtr)(	PrMemoryPtr		src, 
								csSDK_int32		srclen, 
								PrMemoryPtr		dest, 
								csSDK_int32		destlen, 
								unsigned int	flags);

typedef void (*AudMixPtr)(		PrMemoryPtr		buf1, 
								csSDK_int32		v1, 
								PrMemoryPtr		buf2, 
								csSDK_int32		v2, 
								PrMemoryPtr		buf3, 
								csSDK_int32		v3,
								csSDK_int32		width, 
								PrMemoryPtr		dest, 
								unsigned int	flags);

typedef void (*AudLimitPtr)(	PrMemoryPtr		src, 
								PrMemoryPtr		dest, 
								csSDK_int32		width, 
								unsigned int	flags, 
								csSDK_int32		total);

typedef void (*AudSumPtr)(		PrMemoryPtr		src, 
								PrMemoryPtr		dest, 
								csSDK_int32		width, 
								csSDK_int32		scale, 
								unsigned int	flags, 
								csSDK_int32		part, 
								csSDK_int32		total);

typedef void (*DistortPolygonPtr)(	PPixHand		src, 
									PPixHand		dest, 
									prRect			*srcbox, 
									prPoint			*dstpts);

typedef void (*DistortFixedPolygonPtr)(	PPixHand		src, 
										PPixHand		dest, 
										prRect			*srcbox, 
										LongPoint		*dstpts);

typedef void (*PolyToPolyPtr)(	PPixHand		src, 
								PPixHand		dest, 
								prPoint			*srcpts, 
								prPoint			*dstpts);

typedef void (*FixedToFixedPtr)(PPixHand		src, 
								PPixHand		dest, 
								LongPoint		*srcpts, 
								LongPoint		*dstpts);
typedef void (*ImageKeyPtr)(PPixHand src, PPixHand dest, PPixHand work,	PrMemoryPtr workbuf, unsigned short level, void* therec);
typedef void (*ResamplePtr)(ResampleRec *theRec);
typedef void (*AudioMungePtr)(AudioMungeRec *theRec);
typedef void (*ConvolvePtr)(unsigned char *src, unsigned char *dst, short *matrix, int row, int width, int height);
typedef void (*IndexMapPtr)(char *src, char *dst, int row, int pixwidth, int height,char *lookup1, char *lookup2, char *lookup3);


typedef struct
{
	short                                   count;                                  // number of routines
	short                                   reserved[14];

	StretchBitsPtr                          StretchBits;
	DistortPolygonPtr                       DistortPolygon;
	PolyToPolyPtr                           MapPolygon;
	AudStretchPtr                           AudioStretch;
	AudMixPtr                               AudioMix;
	AudSumPtr                               AudioSum;
	AudLimitPtr                             AudioLimit;
	DistortFixedPolygonPtr         			DistortFixed;
	FixedToFixedPtr                         FixedToFixed;
	ImageKeyPtr                             ImageKey;
	ResamplePtr                             Resample;
	AudioMungePtr                           AudioMunge;
	IndexMapPtr                             DoIndexMap;
	ConvolvePtr                             DoConvolve;     
	csSDK_int32                             unused[1];
} BottleRec;

/* 
	These values are used to determing the Project Settings Time Format
	used by the plug-in suites utilites callback.
	
	long timeSettings =  getSettings(kSettingsTimeFormat);

	if timeSettings == 131102 == 0x2001e
	The framerate is x1e = 30 and the format is 0x20000 = tfSMPTE.

	The low 16-bits has the framerate and the hi 16-bits has the format.
*/
enum {
	tfFrameCount = 0x00010000,		// Frames/Samples
	tfSMPTE = 0x00020000,			// HH:MM:SS:FF   where FF is in the frame rate base
	tfSMPTEDrop = 0x00040000,		// HH;MM;SS;FF   in 29.97 base
	tfFeetFrame = 0x00080000		// ffff.FF  -- frame rate is either 16 (35mm) or 40 (16mm)

};

#pragma pack(pop)

#endif	/* PRSDKSTRUCTS_H */
