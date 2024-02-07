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

//	This file is part of the public Premiere SDK.


#ifndef	PRSDKPALETTESUITE_H
#define PRSDKPALETTESUITE_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKMALERRORS_H
#include "PrSDKMALErrors.h"
#endif

#ifdef PRWIN_ENV // The Palette Suite is Windows Only

#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif


#define kPrSDKPaletteSuite				"Premiere Palette Suite"
#define kPrSDKPaletteSuiteVersion		1

#ifdef PRWIN_ENV
	//#define compFileRef HANDLE
#define compPalette	HPALETTE
#endif
#ifdef PRMAC_ENV
	//#define compFileRef short
#define compPalette PaletteHandle
#endif
typedef unsigned long *compHistogramPtr;
typedef struct
{
	int				width;			// width of image
	int				height;			// height of image
	short			rowBytes;		// # of bytes in a row
	char			*pixels;		// ptr to the pixels
} compHistogramFrameInfoRec;

#ifdef PRWIN_ENV
typedef struct
{
	LPBITMAPINFOHEADER	lpbiIn;		//	dib format
	LPBYTE				pbIn;		//	input pixels
	LPBITMAPINFOHEADER	lpbiOut;	//	output dib format
	LPBYTE				pbOut;		//	output pixels
	HPALETTE			hpal;		//	palette to use
} compPixelInfoRec;
#endif

#ifdef PRMAC_ENV
typedef struct
{
	CTabHandle			hpal;		//	palette to use
} compPixelInfoRec;
#endif

typedef struct 
{
	compHistogramPtr (*HistogramInit)();

	void (*HistogramFree)(
				compHistogramPtr histogram);

	int (*HistogramProcessPixels)(
				compHistogramFrameInfoRec* histogramInfo, 
				compHistogramPtr histogram);

	compPalette (*HistogramToPalette)(
				compHistogramPtr histogram, 
				char *lp16to8, int nColors);

	void (*HistogramDisposePalette)(
				compPalette palette); 

	void (*HistogramReducePixels)(	LPBITMAPINFOHEADER	lpbiIn,		//	dib format
									LPBYTE				pbIn,		//	input pixels
									LPBITMAPINFOHEADER	lpbiOut,	//	output dib format
									LPBYTE				pbOut,		//	output pixels
									int					srcRowBytes,//	source bytes/line
									int					dstRowBytes,//	dest bytes/line
									compPalette			palette,	// HPALETTE on win
									char*				lp16to8);	// lookup table from HistogramToPalette

} PrSDKPaletteSuite;

#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif
#endif /* PRSDKPALETTESUITE_H */

