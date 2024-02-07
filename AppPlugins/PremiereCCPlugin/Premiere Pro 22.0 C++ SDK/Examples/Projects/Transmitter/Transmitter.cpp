/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2012 Adobe Systems Incorporated					   */
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

#include "TransmitterPlugin.h"

using namespace SDK;

/* The TransmitModule class is the main class.  Function pointers to its static methods are
** provided directly to the transmit host.  The TransmitModule, in turn, makes calls
** into the TransmitPlugin or TransmitInstance, as appropriate.
** A pointer to the TransmitPlugin is saved in ioStdParms->ioPrivatePluginData, and
** pointers to TransmitInstances are saved in ioPrivateInstanceData.
*/
class TransmitModule 
{
public:
	/**
	**
	*/
	static tmResult Startup(
		tmStdParms* ioStdParms,
		tmPluginInfo* outPluginInfo)
	{
		ioStdParms->ioPrivatePluginData = new TransmitPlugin(ioStdParms, outPluginInfo);
		return tmResult_Success;
	}

	/**
	**
	*/
	static tmResult Shutdown(
		tmStdParms* ioStdParms)
	{
		delete (TransmitPlugin*)ioStdParms->ioPrivatePluginData;
		ioStdParms->ioPrivatePluginData = 0;
		return tmResult_Success;
	}
	
	/**
	**
	*/
	static tmResult SetupDialog(
		tmStdParms* ioStdParms,
		prParentWnd inParentWnd)
	{
		return ((TransmitPlugin*)ioStdParms->ioPrivatePluginData)->SetupDialog(ioStdParms, inParentWnd);
	}

	/**
	**
	*/
	static tmResult NeedsReset(
		const tmStdParms* inStdParms,
		prBool* outResetModule)
	{
		return ((TransmitPlugin*)inStdParms->ioPrivatePluginData)->NeedsReset(inStdParms, outResetModule);
	}

	/**
	**
	*/
	static tmResult CreateInstance(
		const tmStdParms* inStdParms,
		tmInstance* ioInstance)
	{
		ioInstance->ioPrivateInstanceData = ((TransmitPlugin*)inStdParms->ioPrivatePluginData)->CreateInstance(inStdParms, ioInstance);
		return ioInstance->ioPrivateInstanceData != 0 ? tmResult_Success : tmResult_ErrorUnknown;
	}

	/**
	**
	*/
	static tmResult DisposeInstance(
		const tmStdParms* inStdParms,
		tmInstance* ioInstance)
	{
		((TransmitPlugin*)inStdParms->ioPrivatePluginData)->DisposeInstance(inStdParms, ioInstance);
		ioInstance->ioPrivateInstanceData = 0;
		return tmResult_Success;
	}

	/**
	**
	*/
	static tmResult QueryAudioMode(
		const tmStdParms* inStdParms,
		const tmInstance* inInstance,
		csSDK_int32 inQueryIterationIndex,
		tmAudioMode* outAudioMode)
	{
		return ((TransmitInstance*)inInstance->ioPrivateInstanceData)->QueryAudioMode(inStdParms, inInstance, inQueryIterationIndex, outAudioMode);
	}

	/**
	**
	*/
	static tmResult QueryVideoMode(
		const tmStdParms* inStdParms,
		const tmInstance* inInstance,
		csSDK_int32 inQueryIterationIndex,
		tmVideoMode* outVideoMode)
	{
		return ((TransmitInstance*)inInstance->ioPrivateInstanceData)->QueryVideoMode(inStdParms, inInstance, inQueryIterationIndex, outVideoMode);
	}

	/**
	**
	*/
	static tmResult ActivateDeactivate(
		const tmStdParms* inStdParms,
		const tmInstance* inInstance,
		PrActivationEvent inActivationEvent,
		prBool inAudioActive,
		prBool inVideoActive)
	{
		return ((TransmitInstance*)inInstance->ioPrivateInstanceData)->ActivateDeactivate(inStdParms, inInstance, inActivationEvent, inAudioActive, inVideoActive);
	}

	/**
	**
	*/
	static tmResult StartPlaybackClock(
		const tmStdParms* inStdParms,
		const tmInstance* inInstance,
		const tmPlaybackClock* inClock)
	{
		return ((TransmitInstance*)inInstance->ioPrivateInstanceData)->StartPlaybackClock(inStdParms, inInstance, inClock);
	}

	/**
	**
	*/
	static tmResult StopPlaybackClock(
		const tmStdParms* inStdParms,
		const tmInstance* inInstance)
	{
		return ((TransmitInstance*)inInstance->ioPrivateInstanceData)->StopPlaybackClock(inStdParms, inInstance);
	}

	/**
	**
	*/
	static tmResult PushVideo(
		const tmStdParms* inStdParms,
		const tmInstance* inInstance,
		const tmPushVideo* inPushVideo)
	{
		return ((TransmitInstance*)inInstance->ioPrivateInstanceData)->PushVideo(inStdParms, inInstance, inPushVideo);
	}
};


extern "C" {
DllExport PREMPLUGENTRY xTransmitEntry(
	csSDK_int32	inInterfaceVersion, 
	prBool		inLoadModule, 
	piSuitesPtr	piSuites,
	tmModule*	outModule)
{
	tmResult result = tmResult_Success;

	if (inLoadModule)
	{
		outModule->Startup = TransmitModule::Startup;
		outModule->Shutdown = TransmitModule::Shutdown;
		outModule->SetupDialog = TransmitModule::SetupDialog;
		outModule->NeedsReset = TransmitModule::NeedsReset;
		outModule->CreateInstance = TransmitModule::CreateInstance;
		outModule->DisposeInstance = TransmitModule::DisposeInstance;
		outModule->QueryAudioMode = TransmitModule::QueryAudioMode;
		outModule->QueryVideoMode = TransmitModule::QueryVideoMode;
		outModule->ActivateDeactivate = TransmitModule::ActivateDeactivate;
		outModule->StartPlaybackClock = TransmitModule::StartPlaybackClock;
		outModule->StopPlaybackClock = TransmitModule::StopPlaybackClock;
		outModule->PushVideo = TransmitModule::PushVideo;
	}
	else
	{
		// The module is being unloaded. Nothing to do here in our implementation.
	}
	return result;
}
} // extern "C"
