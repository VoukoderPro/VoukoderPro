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

#ifndef	PRSDKPPIXCACHESUITE_H
#define PRSDKPPIXCACHESUITE_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKPIXELFORMAT_H
#include "PrSDKPixelFormat.h"
#endif 

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKMALERRORS_H
#include "PrSDKMALErrors.h"
#endif

#ifndef PRSDKIMPORTERSHARED_H
#include "PrSDKImporterShared.h"
#endif

#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif

#define kPrSDKPPixCacheSuite			"Premiere PPix Cache Suite"
#define kPrSDKPPixCacheSuiteVersion1	1
#define kPrSDKPPixCacheSuiteVersion2	2
#define kPrSDKPPixCacheSuiteVersion3	3
#define kPrSDKPPixCacheSuiteVersion4	4
#define kPrSDKPPixCacheSuiteVersion5	5
#define kPrSDKPPixCacheSuiteVersion6	6
#define kPrSDKPPixCacheSuiteVersion7	7	// Pr 13.0 - ColorSpace APIs
#define kPrSDKPPixCacheSuiteVersion8	8
#define kPrSDKPPixCacheSuiteVersion		kPrSDKPPixCacheSuiteVersion8

typedef struct 
{
	/**
	**	This will add the ppix to the frame cache for this importer instance and stream. 
	**
	**	@param	inImporterID		The ID of this importer instance (passed down in imGetInfo8).
	**	@param	inStreamIndex		The index of this stream
	**	@param	inPPixHand			The ppix handle you want to add. 
	**	@param	inFrameNumber		The frame number of the ppix.
	**	@param	inPreferences		Current importer preferences, can be NULL for no preferences
	**	@param	inPreferencesLength	Current importer preferences length
	*/
	prSuiteError (*AddFrameToCache)(
		csSDK_uint32		inImporterID,
		csSDK_int32			inStreamIndex,
		PPixHand			inPPixHand,
		csSDK_int32			inFrameNumber,
		void*				inPreferences,		
		csSDK_int32			inPreferencesLength);

	/**
	**	This will retrieve a ppix from the frame cache for this importer instance and stream. 
	**
	**	@param	inImporterID		The ID of this importer instance (passed down in imGetInfo8).
	**	@param	inStreamIndex		The index of this stream
	**	@param	inFrameNumber		The frame number of the ppix requested.
	**	@param	outPPixHand			A pointer to a ppix handle to hold the retrieved frame. 
	**	@param	inPreferences		Current importer preferences, can be NULL for no preferences
	**	@param	inPreferencesLength	Current importer preferences length
	*/
	prSuiteError (*GetFrameFromCache)(
		csSDK_uint32		inImporterID,
		csSDK_int32			inStreamIndex,
		csSDK_int32			inFrameNumber,
		csSDK_int32			inNumFormats,
		imFrameFormat*		inFormats,
		PPixHand*			outPPixHand,
		void*				inPreferences,
		csSDK_int32			inPreferencesLength);

	/**
	**	This will add the ppix to the raw cache for this importer instance. 
	**
	**	@param	inImporterID		The ID of this importer instance (passed down in imGetInfo8).
	**	@param	inPPixHand			The raw ppix handle you want to add. 
	**	@param	inKey				The key to use for this raw ppix. If an entry with this key already
	**								exists, this call will fail.
	*/
	prSuiteError (*AddRawPPixToCache)(
		csSDK_uint32		inImporterID,
		PPixHand			inPPixHand,
		csSDK_int32			inKey);

	/**
	**	This will retrieve a ppix from the frame cache for this importer instance. 
	**
	**	@param	inImporterID		The ID of this importer instance (passed down in imGetInfo8).
	**	@param	inKey				The key to identify this raw ppix.
	**	@param	outPPixHand			A pointer to a ppix handle to hold the retrieved frame. 
	*/
	prSuiteError (*GetRawPPixFromCache)(
		csSDK_uint32		inImporterID,
		csSDK_int32			inKey,
		PPixHand*			outPPixHand);

	/**
	**	This function allows you to cache a PPix using a GUID as an identifier.
	**
	**	@param	inPPixIdentifier	The ID of the PPix, used to identify this particular set of pixels. 
	**								The GUID can be anything, but must be unique.
	**	@param	inPPix				The Pixels to put in the cache. If a frame with this id is already in the cache,
	**								then this PPix will be ignored. You still have ownership over the PPix.
	**								You may want to get the PPix from the cache after this, in case your frame is redundant.
	*/
	prSuiteError (*AddNamedPPixToCache)(
		const prPluginID*	inPPixIdentifier,
		PPixHand			inPPix);

	/**
	**	This function retrieves a named PPix from the cache.
	**
	**	@param	inPPixIdentifier	The ID of the PPix.
	**	@param	outPPixHand			The PPix, if it's available. If it's not in the cache, this will be NULL.
	**								You are responsible for calling PPixDispose on this PPix, like any other.
	*/
	prSuiteError (*GetNamedPPixFromCache)(
		const prPluginID*	inPPixIdentifier,
		PPixHand*			outPPixHand);

	/**
	**	This function registers a dependency on a PPix. If a frame with that identifier is in the cache now,
	**	or ends up in the cache soon, then it will not be flushed from the cache while there are outstanding
	**	dependencies. You are responsible for call the Unregister function - if you don't then this is the 
	**	same as a memory leak of a PPix.
	**
	**	@param	inPPixIdentifier	The ID of the PPix you want held in the cache.
	*/
	prSuiteError (*RegisterDependencyOnNamedPPix)(
		const prPluginID*	inPPixIdentifier);

	/**
	**	This function unregisters a dependency on a named PPix. This must be called for every successful call of 
	**	RegisterDependencyOnNamedPPix
	**
	**	@param inPPixIdentifier		The ID of the PPix you no longer need in the cache.
	*/
	prSuiteError (*UnregisterDependencyOnNamedPPix)(
		const prPluginID* inPPixIdentifier);

	/**
	**  This function attempts to flush a frame from the cache. If the frame has a registered dependency, then the 
	**  frame will remain available until the dependency count hits zero.
	**
	**	@param inPPixIdentifier		The ID of the PPix you want forcibly removed from the cache.
	*/
	prSuiteError (*ExpireNamedPPixFromCache)(
		const prPluginID* inPPixIdentifier);
		
	/**
	**  This function attempts to flush all ppixes from the cache.
	*/
	prSuiteError (*ExpireAllPPixesFromCache)();

	/**
	**	This will add the ppix to the frame cache for this importer instance and stream. 
	**
	**	@param	inImporterID		The ID of this importer instance (passed down in imGetInfo8).
	**	@param	inStreamIndex		The index of this stream
	**	@param	inPPixHand			The ppix handle you want to add. 
	**	@param	inFrameNumber		The frame number of the ppix.
	**	@param	inPreferences		Current importer preferences, can be NULL for no preferences
	**	@param	inPreferencesLength	Current importer preferences length
	**	@param	inColorProfileName	The name of the selected color profile. This is only worth providing if your 
	**								Importer supports color management, and has more than one possible profile.
	**								Otherwise we will default to the single profile provided.
	**	@param	inColorProfileData	This is completely optional. We will use the color profile data provided 
	**								through the imGetIndColorProfile selector. In some future version we will
	**								allow Importers to support arbitrary profiles, at which point this param will
	**								be useful.
	*/
	prSuiteError (*AddFrameToCacheWithColorProfile)(
		csSDK_uint32		inImporterID,
		csSDK_int32			inStreamIndex,
		PPixHand			inPPixHand,
		csSDK_int32			inFrameNumber,
		void*				inPreferences,		
		csSDK_int32			inPreferencesLength,
		PrSDKString*		inColorProfileName,
		void*				inColorProfileData,
		csSDK_int32			inColorProfileDataLength);

	/**
	**	This will retrieve a ppix from the frame cache for this importer instance and stream. 
	**
	**	@param	inImporterID		The ID of this importer instance (passed down in imGetInfo8).
	**	@param	inStreamIndex		The index of this stream
	**	@param	inFrameNumber		The frame number of the ppix requested.
	**	@param	outPPixHand			A pointer to a ppix handle to hold the retrieved frame. 
	**	@param	inPreferences		Current importer preferences, can be NULL for no preferences
	**	@param	inPreferencesLength	Current importer preferences length
	**	@param	inColorProfileName	The name of the selected color profile. This is only worth providing if your 
	**								Importer supports color management, and has more than one possible profile.
	**								Otherwise we will default to the single profile provided.
	**	@param	inColorProfileData	This is completely optional. We will use the color profile data provided 
	**								through the imGetIndColorProfile selector. In some future version we will
	**								allow Importers to support arbitrary profiles, at which point this param will
	**								be useful.
	*/
	prSuiteError (*GetFrameFromCacheWithColorProfile)(
		csSDK_uint32		inImporterID,
		csSDK_int32			inStreamIndex,
		csSDK_int32			inFrameNumber,
		csSDK_int32			inNumFormats,
		imFrameFormat*		inFormats,
		PPixHand*			outPPixHand,
		void*				inPreferences,
		csSDK_int32			inPreferencesLength,
		PrSDKString*		inColorProfileName,
		void*				inColorProfileData,
		csSDK_int32			inColorProfileDataSize);
		
	/**
	**	This will add the ppix to the frame cache for this importer instance and stream. 
	**
	**	@param	inImporterID		The ID of this importer instance (passed down in imGetInfo8).
	**	@param	inStreamIndex		The index of this stream
	**	@param	inPPixHand			The ppix handle you want to add. 
	**	@param	inFrameNumber		The frame number of the ppix.
	**	@param	inPreferences		Current importer preferences, can be NULL for no preferences
	**	@param	inPreferencesLength	Current importer preferences length
	**	@param	inColorProfileName	The name of the selected color profile. This is only worth providing if your 
	**								Importer supports color management, and has more than one possible profile.
	**								Otherwise we will default to the single profile provided.
	**	@param	inColorProfileData	This is completely optional. We will use the color profile data provided 
	**								through the imGetIndColorProfile selector. In some future version we will
	**								allow Importers to support arbitrary profiles, at which point this param will
	**								be useful.
	*/
	prSuiteError (*AddFrameToCacheWithColorProfile2)(
		csSDK_uint32		inImporterID,
		csSDK_int32			inStreamIndex,
		PPixHand			inPPixHand,
		csSDK_int32			inFrameNumber,
		PrRenderQuality		inQuality,
		void*				inPreferences,		
		csSDK_int32			inPreferencesLength,
		PrSDKString*		inColorProfileName,
		void*				inColorProfileData,
		csSDK_int32			inColorProfileDataLength);

	/**
	**	This will retrieve a ppix from the frame cache for this importer instance and stream. 
	**
	**	@param	inImporterID		The ID of this importer instance (passed down in imGetInfo8).
	**	@param	inStreamIndex		The index of this stream
	**	@param	inFrameNumber		The frame number of the ppix requested.
	**	@param	outPPixHand			A pointer to a ppix handle to hold the retrieved frame. 
	**	@param	inPreferences		Current importer preferences, can be NULL for no preferences
	**	@param	inPreferencesLength	Current importer preferences length
	**	@param	inColorProfileName	The name of the selected color profile. This is only worth providing if your 
	**								Importer supports color management, and has more than one possible profile.
	**								Otherwise we will default to the single profile provided.
	**	@param	inColorProfileData	This is completely optional. We will use the color profile data provided 
	**								through the imGetIndColorProfile selector. In some future version we will
	**								allow Importers to support arbitrary profiles, at which point this param will
	**								be useful.
	*/
	prSuiteError (*GetFrameFromCacheWithColorProfile2)(
		csSDK_uint32		inImporterID,
		csSDK_int32			inStreamIndex,
		csSDK_int32			inFrameNumber,
		csSDK_int32			inNumFormats,
		imFrameFormat*		inFormats,
		PPixHand*			outPPixHand,
		PrRenderQuality		inQuality,
		void*				inPreferences,
		csSDK_int32			inPreferencesLength,
		PrSDKString*		inColorProfileName,
		void*				inColorProfileData,
		csSDK_int32			inColorProfileDataSize);
	
	// new in Pr 13.0
	
	/**
	 **	This will add the ppix to the frame cache for this importer instance and stream.
	 **
	 **	@param	inImporterID		The ID of this importer instance (passed down in imGetInfo8).
	 **	@param	inStreamIndex		The index of this stream
	 **	@param	inPPixHand			The ppix handle you want to add.
	 **	@param	inFrameNumber		The frame number of the ppix.
	 **	@param	inQuality			The quality level with which to render the frame
	 **	@param	inPreferences		Current importer preferences, can be NULL for no preferences
	 **	@param	inPreferencesLength	Current importer preferences length
	 **	@param	inPrColorSpaceID	color space for the frame to be cached; this is an opaque token passed to the importer by the host that importer is expected to use in color management APIs
	 */
	prSuiteError (*AddFrameToCacheWithColorSpace)(
		csSDK_uint32		inImporterID,
		csSDK_int32			inStreamIndex,
		PPixHand			inPPixHand,
		csSDK_int32			inFrameNumber,
		PrRenderQuality		inQuality,
		void*				inPreferences,
		csSDK_int32			inPreferencesLength,
		PrSDKColorSpaceID*	inPrColorSpaceID);
	
	/**
	 **	This will retrieve a ppix from the frame cache for this importer instance and stream.
	 **
	 **	@param	inImporterID		The ID of this importer instance (passed down in imGetInfo8).
	 **	@param	inStreamIndex		The index of this stream
	 **	@param	inFrameNumber		The frame number of the ppix requested.
	 ** @param	inNumFormats		The number of frame formats
	 **	@param	inFormats			The array of frame formats
	 **	@param	outPPixHand			A pointer to a ppix handle to hold the retrieved frame.
	 **	@param	inQuality			The quality level with which to render the frame
	 **	@param	inPreferences		Current importer preferences, can be NULL for no preferences
	 **	@param	inPreferencesLength	Current importer preferences length
	 **	@param	inPrColorSpaceID	color space to be used as part of identifier to retrieve a frame from cache; this is an opaque token passed to the importer by the host that importer is expected to use in color management APIs
	 */
	prSuiteError (*GetFrameFromCacheWithColorSpace)(
		csSDK_uint32	inImporterID,
		csSDK_int32		inStreamIndex,
		csSDK_int32		inFrameNumber,
		csSDK_int32		inNumFormats,
		imFrameFormat*	inFormats,
		PPixHand*		outPPixHand,
		PrRenderQuality	inQuality,
		void*			inPreferences,
		csSDK_int32		inPreferencesLength,
		PrSDKColorSpaceID*	inPrColorSpaceID);

	/**
	**	This function registers a dependency on a PPix. If a frame with these settings is in the cache now,
	**	or ends up in the cache soon, then it will not be flushed from the cache while there are outstanding
	**	dependencies. You are responsible for calling the UnregisterDependencyOnNamedPPix function with the
	**  returned identifier - if you don't then this is the same as a memory leak of a PPix.
	**
	**	@param	inImporterID		The ID of this importer instance (passed down in imGetInfo8).
	**	@param	inStreamIndex		The index of this stream
	**	@param	inFrameNumber		The frame number of the ppix.
	**	@param	outPPixIdentifier	The pointer to the ppix Identifer obtained for the frame
	**	@param	inPreferences		Current importer preferences, can be NULL for no preferences
	**	@param	inPreferencesLength	Current importer preferences length
	*/
	prSuiteError (*RegisterDependencyOnFrame)(
		csSDK_uint32		inImporterID,
		csSDK_int32			inStreamIndex,
		csSDK_int32			inFrameNumber,
		csSDK_int32			inNumFormats,
		imFrameFormat*		inFormats,
        prPluginID*	        outPPixIdentifier,
		void*				inPreferences,
		csSDK_int32			inPreferencesLength);

	/**
	**	This function registers a dependency on a PPix. If a frame with these settings is in the cache now,
	**	or ends up in the cache soon, then it will not be flushed from the cache while there are outstanding
	**	dependencies. You are responsible for calling the UnregisterDependencyOnNamedPPix function with the
	**  returned identifier - if you don't then this is the same as a memory leak of a PPix.
	**
	**	@param	inImporterID		The ID of this importer instance (passed down in imGetInfo8).
	**	@param	inStreamIndex		The index of this stream
	**	@param	inFrameNumber		The frame number of the ppix.
	**	@param	outPPixIdentifier	The pointer to the ppix Identifer obtained for the frame
	**	@param	inPreferences		Current importer preferences, can be NULL for no preferences
	**	@param	inPreferencesLength	Current importer preferences length
	**	@param	inColorProfileName	The name of the selected color profile. This is only worth providing if your
	**								Importer supports color management, and has more than one possible profile.
	**								Otherwise we will default to the single profile provided.
	**	@param	inColorProfileData	This is completely optional. We will use the color profile data provided
	**								through the imGetIndColorProfile selector. In some future version we will
	**								allow Importers to support arbitrary profiles, at which point this param will
	**								be useful.
	*/
	prSuiteError (*RegisterDependencyOnFrameWithColorProfile)(
		csSDK_uint32		inImporterID,
		csSDK_int32			inStreamIndex,
		csSDK_int32			inFrameNumber,
		csSDK_int32			inNumFormats,
		imFrameFormat*		inFormats,
        prPluginID*	        outPPixIdentifier,
		void*				inPreferences,
		csSDK_int32			inPreferencesLength,
		PrSDKString*		inColorProfileName,
		void*				inColorProfileData,
		csSDK_int32			inColorProfileDataSize);

	/**
	**	This function registers a dependency on a PPix. If a frame with these settings is in the cache now,
	**	or ends up in the cache soon, then it will not be flushed from the cache while there are outstanding
	**	dependencies. You are responsible for calling the UnregisterDependencyOnNamedPPix function with the
	**  returned identifier - if you don't then this is the same as a memory leak of a PPix.
	**
	**	@param	inImporterID		The ID of this importer instance (passed down in imGetInfo8).
	**	@param	inStreamIndex		The index of this stream
	**	@param	inFrameNumber		The frame number of the ppix.
	**	@param	outPPixIdentifier	The pointer to the ppix Identifer obtained for the frame
	**	@param	inPreferences		Current importer preferences, can be NULL for no preferences
	**	@param	inPreferencesLength	Current importer preferences length
	**	@param	inColorProfileName	The name of the selected color profile. This is only worth providing if your
	**								Importer supports color management, and has more than one possible profile.
	**								Otherwise we will default to the single profile provided.
	**	@param	inColorProfileData	This is completely optional. We will use the color profile data provided
	**								through the imGetIndColorProfile selector. In some future version we will
	**								allow Importers to support arbitrary profiles, at which point this param will
	**								be useful.
	*/
	prSuiteError (*RegisterDependencyOnFrameWithColorProfile2)(
		csSDK_uint32		inImporterID,
		csSDK_int32			inStreamIndex,
		csSDK_int32			inFrameNumber,
		csSDK_int32			inNumFormats,
		imFrameFormat*		inFormats,
		PrRenderQuality		inQuality,
        prPluginID*	        outPPixIdentifier,
		void*				inPreferences,
		csSDK_int32			inPreferencesLength,
		PrSDKString*		inColorProfileName,
		void*				inColorProfileData,
		csSDK_int32			inColorProfileDataSize);

	/**
	**	This function registers a dependency on a PPix. If a frame with these settings is in the cache now,
	**	or ends up in the cache soon, then it will not be flushed from the cache while there are outstanding
	**	dependencies. You are responsible for calling the UnregisterDependencyOnNamedPPix function with the
	**  returned identifier - if you don't then this is the same as a memory leak of a PPix.
	**
	**	@param	inImporterID		The ID of this importer instance (passed down in imGetInfo8).
	**	@param	inStreamIndex		The index of this stream
	**	@param	inFrameNumber		The frame number of the ppix.
	** 	@param	inNumFormats		The number of frame formats
	**	@param	inFormats			The array of frame formats
	**	@param	inQuality			The quality level with which to render the frame
	**	@param	outPPixIdentifier	The pointer to the ppix Identifer obtained for the frame
	**	@param	inPreferences		Current importer preferences, can be NULL for no preferences
	**	@param	inPreferencesLength	Current importer preferences length
	**	@param	inPrColorSpaceID	color space to be used as part of identifier to retrieve a frame from cache; this is an opaque token passed to the importer by the host that importer is expected to use in color management APIs
	*/
	prSuiteError (*RegisterDependencyOnFrameWithColorSpace)(
		csSDK_uint32		inImporterID,
		csSDK_int32			inStreamIndex,
		csSDK_int32			inFrameNumber,
		csSDK_int32			inNumFormats,
		imFrameFormat*		inFormats,
		PrRenderQuality		inQuality,
        prPluginID*	        outPPixIdentifier,
		void*				inPreferences,
		csSDK_int32			inPreferencesLength,
		PrSDKColorSpaceID*	inPrColorSpaceID);
} PrSDKPPixCacheSuite;


/*
*	v1 of suite; please use version 2
*/ 
typedef struct 
{
	/**
	**	This will add the ppix to the frame cache for this importer instance and stream. 
	**
	**	@param	inImporterID		The ID of this importer instance (passed down in imGetInfo8).
	**	@param	inStreamIndex		The index of this stream
	**	@param	inPPixHand			The ppix handle you want to add. 
	**	@param	inFrameNumber		The frame number of the ppix.
	*/
	prSuiteError (*AddFrameToCache)(
		csSDK_uint32		inImporterID,
		csSDK_int32			inStreamIndex,
		PPixHand			inPPixHand,
		csSDK_int32			inFrameNumber);

	/**
	**	This will retrieve a ppix from the frame cache for this importer instance and stream. 
	**
	**	@param	inImporterID		The ID of this importer instance (passed down in imGetInfo8).
	**	@param	inStreamIndex		The index of this stream
	**	@param	inFrameNumber		The frame number of the ppix requested.
	**	@param	outPPixHand			A pointer to a ppix handle to hold the retrieved frame. 
	*/
	prSuiteError (*GetFrameFromCache)(
		csSDK_uint32		inImporterID,
		csSDK_int32			inStreamIndex,
		csSDK_int32			inFrameNumber,
		csSDK_int32			inNumFormats,
		imFrameFormat*		inFormats,
		PPixHand*			outPPixHand);

	/**
	**	This will add the ppix to the raw cache for this importer instance. 
	**
	**	@param	inImporterID		The ID of this importer instance (passed down in imGetInfo8).
	**	@param	inPPixHand			The raw ppix handle you want to add. 
	**	@param	inKey				The key to use for this raw ppix. If an entry with this key already
	**								exists, this call will fail.
	*/
	prSuiteError (*AddRawPPixToCache)(
		csSDK_uint32		inImporterID,
		PPixHand			inPPixHand,
		csSDK_int32			inKey);

	/**
	**	This will retrieve a ppix from the frame cache for this importer instance. 
	**
	**	@param	inImporterID		The ID of this importer instance (passed down in imGetInfo8).
	**	@param	inKey				The key to identify this raw ppix.
	**	@param	outPPixHand			A pointer to a ppix handle to hold the retrieved frame. 
	*/
	prSuiteError (*GetRawPPixFromCache)(
		csSDK_uint32		inImporterID,
		csSDK_int32			inKey,
		PPixHand*			outPPixHand);

} PrSDKPPixCacheSuite1;

#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif
