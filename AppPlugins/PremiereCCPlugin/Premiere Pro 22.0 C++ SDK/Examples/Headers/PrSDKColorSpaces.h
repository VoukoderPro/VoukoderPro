/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2018 Adobe Systems Incorporated                       */
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

#ifndef	PRSDKCOLORSPACES_H
#define PRSDKCOLORSPACES_H

/****************** Predefined standard color spaces ***************/
/*															       */
/* Predefined descriptors for supported standard color spaces.     */
/* To use them, when queried for color space, the plug-in should   */
/* set the type of ColorSpaceRec to kPrColorSpaceType_Predefined   */
/* and pass one of these descriptors as the outName in			   */
/* ioProfileRec (see kPrSDKColorProfile.h)						   */
/*																   */
/*******************************************************************/

// Descriptor for over-ranged 32f full range Rec. 709
// This space is used as a connection space between the host
// and a plug-in if the host does not understand plug-in's
// native color space. All importers and exporters should
// support conversion between their native media space
// and this connection space.


#define kPrSRGBColorSpace "sRGB"								// full range, RGB, 8-bit, Display

#define kPrRec601525ColorSpace "BT.601 (NTSC)"					// narrow range, YCC, 8-bit, Display
#define kPrRec601625ColorSpace "BT.601 (PAL)"					// narrow range, YCC, 8-bit, Display

#define kPrRec709 "BT.709"										// narrow range, YCC, 8-bit, Display
#define kPrRec709Scene "BT.709 (Scene)"							// narrow range, YCC, 8-bit, Scene
#define kPrRec709RGB "BT.709 RGB"								// full range, RGB, 8-bit, Display
#define kPrRec709RGBScene "BT.709 RGB (Scene)"					// full range, RGB, 8-bit, Scene
#define kPrOverranged709 "BT.709 RGB Full"						// full range, RGB, 32f, Scene/Display
#define kPrOverranged709Scene "BT.709 RGB Full (Scene)"			// full range, RGB, 32f, Scene
#define kPrOverranged709Display "BT.709 RGB Full (Display)"		// full range, RGB, 32f, Display

#define kPrRec2020 "BT.2020"									// narrow range, YCC, NCL, 10-bit, Display
#define kPrRec2020Scene "BT.2020 (Scene)"						// narrow range, YCC, NCL, 10-bit, Scene
#define kPrRec2020RGB "BT.2020 RGB"								// full range, RGB, 10-bit, Display
#define kPrRec2020RGBScene "BT.2020 RGB (Scene)"				// full range, RGB, 10-bit, Scene
#define kPrOverranged2020 "BT.2020 RGB Full"    				// full range, RGB, 32f, Scene/Display
#define kPrOverranged2020Scene "BT.2020 RGB Full (Scene)"  		// full range, RGB, 32f, Scene
#define kPrOverranged2020Display "BT.2020 RGB Full (Display)"	// full range, RGB, 32f, Display

#define kPrRec2100HLG "BT.2100 HLG" 							// narrow range, YCC, 10-bit, Display
#define kPrRec2100HLGScene "BT.2100 HLG (Scene)" 				// narrow range, YCC, 10-bit, Scene
#define kPrRec2100HLGRGB "BT.2100 HLG RGB" 						// full range, RGB, 10-bit, Display
#define kPrRec2100HLGRGBScene "BT.2100 HLG RGB (Scene)"			// full range, RGB, 10-bit, Scene
#define kPrOverranged2100HLG "BT.2100 HLG RGB Full"    			// full range, RGB, 32f, Scene/Display
#define kPrOverranged2100HLGScene "BT.2100 HLG RGB Full (Scene)"// full range, RGB, 32f, Scene
#define kPrOverranged2100HLGDisplay "BT.2100 HLG RGB Full (Display)"	// full range, RGB, 32f, Display

#define kPrRec2100PQ "BT.2100 PQ" 								// narrow range, YCC, 10-bit, Display
#define kPrRec2100PQScene "BT.2100 PQ (Scene)" 					// narrow range, YCC, 10-bit, Scene
#define kPrRec2100PQRGB "BT.2100 PQ RGB" 						// full range, RGB, 10-bit, Display
#define kPrRec2100PQRGBScene "BT.2100 PQ RGB (Scene)" 			// full range, RGB, 10-bit, Scene
#define kPrOverranged2100PQ "BT.2100 PQ RGB Full"				// full range, RGB, 32f, Scene/Display
#define kPrOverranged2100PQScene "BT.2100 PQ RGB Full (Scene)"	// full range, RGB, 32f, Scene
#define kPrOverranged2100PQDisplay "BT.2100 PQ RGB Full (Display)"	// full range, RGB, 32f, Display

#define kPrDCDMXYZ "DCDM X'Y'Z'"								// full range, RGB, 12-bit, Display

// [TODO] mbakshi 10.06.2019
// Implement SLog color space creation in MFTaggedColorSpaces from these strings. Tracking with Jira DVAPR-4217312
// Implemnent SLog color space automation tests. Tracking with Jira DVAPR-4217120
#define kPrSonySGamutSLog2 "Sony SGamut/SLog2"
#define kPrSony2020SLog3 "BT.2020/SLog3"
#define kPrSonySGamut3CineSLog3 "Sony SGamut3.Cine/SLog3"
#define kPrSonySGamut3SLog3 "Sony SGamut3/SLog3"


// this is a convenient way to get current working color space.
// if plug-in passes it back to the host when queried for color
// space, the host will use current working color space when
// rendering a frame.
// please note that since Pr defines working color space on a
// sequence level, it may not necessarily be one of the color
// spaces you see in the pop-up menu in sequence settings.
// For instance, for some sequences (when you export media directly),
// the working color space may be the media space of the source.
// if you don't need to explicitly specify your color space, you can
// use this token to set the rendering color space.
// also, please note that you can't use this token in the importer.
// the importer needs to explicitly identify media color space to
// the host.
#define kPrWorkingColorSpace "Working Color Space"

#endif // PRSDKCOLORSPACES_H
