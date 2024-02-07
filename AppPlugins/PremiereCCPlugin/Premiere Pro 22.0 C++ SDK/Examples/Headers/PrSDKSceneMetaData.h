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

/*		
	Part of the Adobe Premiere SDK.
	Defines the scene meta data for attachment to movies.
*/

#define kSceneMetaDataFourCC	'ADsc'

#pragma pack(push, 1)

typedef struct
{
	csSDK_uint32 scale;		// e.g. 2997
	csSDK_int32	sampleSize;	// e.g. 100
} PrSceneTimebaseRec;

// this is one scene entry
typedef struct
{
	csSDK_int32	startTime;	// in SceneTimebase samples
	csSDK_int32	duration;	// in SceneTimebase samples
} PrSceneRecord;

// this is the definition of what the data in the scene
// meta data will look like: one PrSceneTimebaseRec
// followed by 'n' PrSceneRecords.
typedef struct
{
	PrSceneTimebaseRec	timeBase;
	PrSceneRecord		scenes[1];
} PrSceneMetaDataRec;

#pragma pack(pop)
