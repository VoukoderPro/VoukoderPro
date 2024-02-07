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



#ifndef PRSDKPIPLVER_H
#define PRSDKPIPLVER_H

#ifndef	MAC_ENV
#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif
#endif

/*
 * This file should be included by all plug-in PiPLs.
 * It defines a global plug-in version number scheme
 * plus a few other useful types.
 */

/* PiPL Build Stage used in the version string */

#define PiPLDevelop			0
#define PiPLAlpha			1
#define PiPLBeta			2
#define PiPLRelease			3

/* PiPL version number stuff */

#define PiPLMajorVersion	6
#define PiPLMinorVersion	0
#define PiPLStage			PiPLDevelop
#define PiPLBuildNum		0

/* PiPL version number, this is the version of the PiPL resource itself */

/*	This is being left at 2.1 for 5.1 compatibility; plug-ins which use
	6.0-specific PiPL implementations must update PiPLVerMinor to 3.	*/


#ifndef PiPLVerMajor
#define PiPLVerMajor		2
#endif
#ifndef PiPLVerMinor
#define PiPLVerMinor		1
#endif

/* true for PiPL version 2.2 or higher (Premiere 5.1) */

#define	PiPLVer2p2		(PiPLVerMajor > 2 || (PiPLVerMajor == 2 && PiPLVerMinor > 1))

/* true for PiPL version 2.3 or higher (Premiere 6.0) */

#define	PiPLVer2p3		(PiPLVerMajor > 2 || (PiPLVerMajor == 2 && PiPLVerMinor > 2))

/*
 * These are used by transition PiPLs, and the RT transition record.
 * In the SDK, they are defined in PiPL.r
 */

#define bitNone			0x00
#define bitTop			0x01
#define bitRight		0x02
#define	bitBottom		0x04
#define	bitLeft			0x08
#define	bitUpperRight	0x10
#define	bitLowerRight	0x20
#define	bitLowerLeft	0x40
#define	bitUpperLeft	0x80

#define	fxBitAnyPAR		0x10000
#define	fxBitUnityPAR	0x20000

#endif /* PRSDKPIPLVER_H */
