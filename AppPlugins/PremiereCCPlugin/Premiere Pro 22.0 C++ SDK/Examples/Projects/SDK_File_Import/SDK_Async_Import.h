/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 1999-2008, Adobe Systems Incorporated                 */
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

#ifndef SDKFILEIMPORT_H
#include "SDK_File_Import.h"
#endif

// For asynchronous import testing
// Defining will write debug information to MSVC++ Output Pane
//#define ASYNC_IMPORT_DEBUG_INFO

// We use a list of these to keep track of the asynchronous disk reads we've started
typedef struct
{
	csSDK_int32	frameNum;
	char		*frameBufferFromFile;
	PPixHand	thePPH;
	#ifdef PRWIN_ENV
	OVERLAPPED	overlapped;
	#endif
} FrameRequest;

PREMPLUGENTRY xAsyncImportEntry(int inSelector,	void *inParam);

// This object is created by an importer during imCreateAsyncImporter
class SDKAsyncImporter
{
public:
	SDKAsyncImporter(imStdParms *stdParms, ImporterLocalRec8H inRecH);
	~SDKAsyncImporter();
	int OnInitiateAsyncRead(imSourceVideoRec& inSourceRec);
	int OnCancelAsyncRead(imSourceVideoRec& inSourceRec);
	int OnFlush();
	int OnGetFrame(imSourceVideoRec* inFrameRec);
private:
	ImporterLocalRec8H localRecH;
	#ifdef PRWIN_ENV
	std::list<FrameRequest *>	listOfOutstandingRequests;
	CRITICAL_SECTION			listLock;
	#endif
};