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

#include "PrSDKImport.h"
#include "PrSDKPixelFormat.h"

// Constants
#define	IMPORTER_NAME	"Synthetic SDK File Options Dialog"
#define EDITING_FILE	"Provide any user options here"
#define SynthImporterClassID  'SyIm'

typedef struct LocalRec
{
	csSDK_int32	getPrefsCounter;
} LocalRec, **LocalHand;

prMALError colrGetInfo8 (imStdParms *stdParms, imFileAccessRec8 *fileAccessRec8, imFileInfoRec8 *imFileInfo8);
prMALError colrImportImage (imStdParms *stdParms, imFileRef ref, imImportImageRec *importImageRec);
prMALError colrInit (imStdParms *stdParms, imImportInfoRec *importInfoRec);
prMALError colrGetPrefs8 (imStdParms *stdParms, imFileAccessRec8 *fileAccessRec8, imGetPrefsRec *getPrefsRec);
prMALError colrGetIndFormat(imStdParms *stdParms, csSDK_size_t index, imIndFormatRec *indFormatRec);
prMALError colrGetIndPixelFormat (imStdParms *stdParms, csSDK_size_t index, imIndPixelFormatRec *pixelRec);
prMALError colrShutdown (imStdParms *stdParms);

// Declare plug-in entry point with C linkage
#ifdef __cplusplus
extern "C" {
#endif

PREMPLUGENTRY DllExport xImportEntry (csSDK_int32 selector, imStdParms *stdParms, void *param1, void *param2);

#ifdef __cplusplus
}
#endif