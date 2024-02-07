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
/* Adobe Premiere Device Control plug-in definitions			   */
/*																   */
/*******************************************************************/

//	This file is part of the public Premiere SDK.


#ifndef PRSDKMALERRORS_H
#define PRSDKMALERRORS_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

/*	These constants are part of our efforts to unify
	the Media Abstraction Layer (MAL) interface.
*/

//Media abstraction layer errors and types

typedef csSDK_int32 prMALError;
typedef csSDK_int32 prSuiteError;

const prMALError malNoError = 0;						// no error
const prMALError malUnknownError = 99;					// unknown error
const prMALError malSupports7 = 100;					// returned by plugins if they support version 7 of the API
const prMALError malSupports8 = 101;					// returned by plugins if they support version 8 of the API
const prMALError malSupportsPerInstancePrefs = 102;		// returned by plugins if they support new per instance prefs APIs

/**
**	Errors returned by suite function calls.
*/

#define PrSuiteErrorFailed(suiteError)		(suiteError < 0)
#define PrSuiteErrorSucceeded(suiteError)	(suiteError >= 0)

/*	<private>
**	
**	Suite Errors are divided into the following bits:
**	bit	 31			Severity  1 for error, 0 for success (negative numbers are errors)
**	bits 30..16		Suite returning error. 0 is reserved for general errors.
**	bits 15..0		Error code.
**
**	</private>
*/

/*
**	Successful error results.
*/
const prSuiteError		suiteError_NoError			= 0x00000000;	// Method succeeded

/*
**	General error results.
*/
const prSuiteError		suiteError_Fail				= 0x80000000;	// Method failed
const prSuiteError		suiteError_InvalidParms		= 0x80000001;	// A parameter to this method is invalid
const prSuiteError		suiteError_OutOfMemory		= 0x80000002;	// There is not enough memory to complete this method
const prSuiteError		suiteError_InvalidCall		= 0x80000003;	// Usually this means this method call is not appropriate at this time
const prSuiteError		suiteError_NotImplemented	= 0x80000004;	// The requested action is not implemented
const prSuiteError		suiteError_IDNotValid		= 0x80000005;	// The passed in ID (pluginID, clipID...) is not valid


/*
**	RenderSuite results
*/

/*	<private>
**	RenderSuite ErrorCategory == 1
**	</private>
*/
const prSuiteError		suiteError_RenderPending				= 0x00010000;	// Render is pending
const prSuiteError		suiteError_RenderedFrameNotFound		= 0x00010001;	// A cached frame was not found.
const prSuiteError		suiteError_RenderedFrameCanceled		= 0x00010002;	// A render was canceled

const prSuiteError		suiteError_RenderInvalidPixelFormat		= 0x80010000;	// Render output pixel format list is invalid
const prSuiteError		suiteError_RenderCompletionProcNotSet	= 0x80010001;	// The render completion proc was not set for an async request

/*
**	TimeSuite results
*/

/*	<private>
**	TimeSuite ErrorCategory == 2
**	</private>
*/
const prSuiteError		suiteError_TimeRoundedAudioRate			= 0x00020000;	// Audio rate returned was rounded

/*
**	Compiler{Render,Audio,Settings}Suite results
**
**	NOTE: If this list is changed in any way, you must also
**	update:
**
**	1.) SuiteErrorToCompilerError() and CompilerErrorToSuiteError()
**		in \Plugins\MediaCommon\MediaUtils\Src\Compilers\CompilerErrorUtils.cpp
**	2.)	CompilerErrorToSuiteError() in \MediaLayer\Src\Compilers\CompilerModuleCallbacks.cpp
*/

/*	<private>
**	Compiler{Render,Audio,Settings}Suite ErrorCategory == 3
**	</private>
*/
const prSuiteError		suiteError_CompilerCompileAbort				= 0x00030001;	// User aborted the compile
const prSuiteError		suiteError_CompilerCompileDone				= 0x00030002;	// Compile finished normally
const prSuiteError		suiteError_CompilerOutputFormatAccept		= 0x00030004;	// The output format is valid
const prSuiteError		suiteError_CompilerOutputFormatDecline		= 0x00030005;	// The compile module cannot compile to the output format
const prSuiteError		suiteError_CompilerRebuildCutList			= 0x00030006;	// Return value from compGetFilePrefs used to force Premiere to bebuild its cutlist
const prSuiteError		suiteError_CompilerIterateCompiler			= 0x00030007;	// 6.0 Return value from compInit to request compiler iteration
const prSuiteError		suiteError_CompilerIterateCompilerDone		= 0x00030008;	// 6.0 Return value from compInit to indicate there are no more compilers
const prSuiteError		suiteError_CompilerInternalErrorSilent		= 0x00030009;	// 6.0 Silent error code; Premiere will not display an error message on screen.
																					// Compilers can return this error code from compDoCompile if they wish to
																					// put their own customized error message on screen just before returning 
																					// control to Premiere
const prSuiteError		suiteError_CompilerIterateCompilerCacheable = 0x0003000A;	// 7.0 Return value from compInit to request compiler iteration and indicating that this
																					// compiler is cacheable.

const prSuiteError		suiteError_CompilerBadFormatIndex			= 0x80030000;	// Invalid format index - used to stop compGetIndFormat queries
const prSuiteError		suiteError_CompilerInternalError			= 0x80030001;	// 
const prSuiteError		suiteError_CompilerOutOfDiskSpace			= 0x80030002;	// Out of disk space error
const prSuiteError		suiteError_CompilerBufferFull				= 0x80030003;	// The offset into the audio buffer would overflow it
const prSuiteError		suiteError_CompilerErrOther					= 0x80030004;	// Someone set gCompileErr
const prSuiteError		suiteError_CompilerErrMemory				= 0x80030005;	// Ran out of memory
const prSuiteError		suiteError_CompilerErrFileNotFound			= 0x80030006;	// File not found
const prSuiteError		suiteError_CompilerErrTooManyOpenFiles		= 0x80030007;	// Too many open files
const prSuiteError		suiteError_CompilerErrPermErr				= 0x80030008;	// Permission violation
const prSuiteError		suiteError_CompilerErrOpenErr				= 0x80030009;	// Unable to open the file
const prSuiteError		suiteError_CompilerErrInvalidDrive			= 0x8003000A;	// Drive isn't valid.
const prSuiteError		suiteError_CompilerErrDupFile				= 0x8003000B;	// Duplicate Filename
const prSuiteError		suiteError_CompilerErrIo					= 0x8003000C;	// File io error
const prSuiteError		suiteError_CompilerErrInUse					= 0x8003000D;	// File is in use
const prSuiteError		suiteError_CompilerErrCodecBadInput			= 0x8003000E;	// A video codec refused the input format
const prSuiteError		suiteError_ExporterSuspended				= 0x8003000F;	// The host has suspended the export
const prSuiteError		suiteError_ExporterNoMoreFrames			= 0x80030010;	// Halt export early skipping all remaining frames including this one. AE uses

/*
**	FileSuite results
*/

/*	<private>
**	FileSuite ErrorCategory == 4
**	</private>
*/
const prSuiteError		suiteError_FileBufferTooSmall			= 0x80040000;
const prSuiteError		suiteError_FileNotImportableFileType	= 0x80040001;	// Not an importable file type

/*
**	LegacySuite results
*/

/*	<private>
**	LegacySuite ErrorCategory == 5
**	</private>
*/
const prSuiteError		suiteError_LegacyInvalidVideoRate		= 0x80050000;	// Invalid video rate (scale and sample rate don't match a valid rate)

/*
**	PlayModuleAudioSuite results
*/

/*	<private>
**	PlayModuleAudioSuite ErrorCategory == 6
**	</private>
*/
const prSuiteError		suiteError_PlayModuleAudioInitFailure			= 0x80060000;
const prSuiteError		suiteError_PlayModuleAudioIllegalPlaySetting	= 0x80060001;
const prSuiteError		suiteError_PlayModuleAudioNotInitialized		= 0x80060002;
const prSuiteError		suiteError_PlayModuleAudioNotStarted			= 0x80060003;
const prSuiteError		suiteError_PlayModuleAudioIllegalAction			= 0x80060004;

/*
**	PlayModuleDeviceControlSuite
*/

/*	<private>
**	PlayModuleDeviceControlSuite ErrorCategory == 7
**	</private>
*/
const prSuiteError		suiteError_PlayModuleDeviceControlSuiteIllegalCallSequence	= 0x80070000;

/*
**	MediaAcceleratorSuite ErrorCategory == 8
*/
const prSuiteError		suiteError_MediaAcceleratorSuitePathNotFound	= 0x80080000;
const prSuiteError		suiteError_MediaAcceleratorSuiteRegisterFailure	= 0x80080001;


/*
**	Royalty Activation ErrorCategory == 9
*/
const prSuiteError		suiteError_RepositoryReadFailed					= 0x80090000;
const prSuiteError		suiteError_RepositoryWriteFailed				= 0x80090001;
const prSuiteError		suiteError_NotActivated							= 0x80090002;
const prSuiteError		suiteError_DataNotPresent						= 0x80090003;
const prSuiteError		suiteError_ServerCommunicationFailed			= 0x80090004;
const prSuiteError		suiteError_Internal								= 0x80090005;

/*
**	PrSDKStringSuite ErrorCategory == A
*/
const prSuiteError		suiteError_StringNotFound						= 0x800A0000;
const prSuiteError		suiteError_StringBufferTooSmall					= 0x800A0001;


/*
**	PrSDKVideoSegmentSuite ErrorCategory == B
*/
const prSuiteError		suiteError_NoKeyframeAfterInTime				= 0x800B0000;

/*
**	PrSDKCaptioningSuite ErrorCategory == C
*/
const prSuiteError		suiteError_NoMoreData							= 0x000C0000;

/*
**	PrSDKThreadedWorkSuite ErrorCategory == D
*/
const prSuiteError		suiteError_InstanceDestroyed					= 0x800D0000;

/**
**	Control Surface HRESULTs
*/

#define CS_E_UNKNOWN_ERROR		_HRESULT_TYPEDEF_(0xE0010001);
#define CS_E_INVALID_ID			_HRESULT_TYPEDEF_(0xE0010002);

#endif 
