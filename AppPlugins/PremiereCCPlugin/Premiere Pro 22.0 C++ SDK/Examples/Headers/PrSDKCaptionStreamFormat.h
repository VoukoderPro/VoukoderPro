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


#ifndef PRSDKCAPTIONSTREAMFORMAT_H
#define PRSDKCAPTIONSTREAMFORMAT_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

/**
**	Defines the Caption Stream format to render
*/
typedef enum 
{
	kPrRenderCaptionStreamFormat_Undefined = 0,	// No captions rendered

	kPrRenderCaptionStreamFormat_Open,			// Open Captions (Free format)
	// Austalian Captions
	kPrRenderCaptionStreamFormat_OP_42 = 10,	// OP-42
	kPrRenderCaptionStreamFormat_OP_47,			// OP-47
	// CEA-608
	kPrRenderCaptionStreamFormat_608_CC1 = 20,	// Primary Synchronous Caption Service
	kPrRenderCaptionStreamFormat_608_CC2,		// Special Non-Synchronous Use Captions
	kPrRenderCaptionStreamFormat_608_CC3,		// Secondary Synchronous Caption Service
	kPrRenderCaptionStreamFormat_608_CC4,		// Special Non-Synchronous Use Captions
	kPrRenderCaptionStreamFormat_608_Text1,		// First Text Service
	kPrRenderCaptionStreamFormat_608_Text2,		// Second Text Service
	kPrRenderCaptionStreamFormat_608_Text3,		// Third Text Service
	kPrRenderCaptionStreamFormat_608_Text4,		// Fourth Text Service
	kPrRenderCaptionStreamFormat_608_XDS,		// XDS (Extended Data Service)
	// CEA-708
	kPrRenderCaptionStreamFormat_708_Service1 = 40,	// Primary Caption Service
	kPrRenderCaptionStreamFormat_708_Service2,		// Secondary Caption Service
	kPrRenderCaptionStreamFormat_708_Service3,
	kPrRenderCaptionStreamFormat_708_Service4,
	kPrRenderCaptionStreamFormat_708_Service5,
	kPrRenderCaptionStreamFormat_708_Service6,
	// Teletext (EBU N-19)
	kPrRenderCaptionStreamFormat_Teletext_Level1 = 50,
	kPrRenderCaptionStreamFormat_Teletext_Level2,
	kPrRenderCaptionStreamFormat_Open_Subtitling
} PrRenderCaptionStreamFormat;

#endif