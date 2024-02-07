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


#ifndef	PRSDKPLAYMODULEDEVICECONTROLSUITE_H
#define PRSDKPLAYMODULEDEVICECONTROLSUITE_H

#ifndef PRSDKTYPES_H
#include "PrSDKTypes.h"
#endif

#ifndef PRSDKMALERRORS_H
#include "PrSDKMALErrors.h"
#endif

#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif

#define kPrSDKPlayModuleDeviceControlSuite				"Premiere Playmod Device Control Suite"
#define kPrSDKPlayModuleDeviceControlSuiteVersion		1

typedef csSDK_int32 PlayModuleDeviceID;

const prSuiteError	kPrDeviceControlResult_Success = 0;
const prSuiteError	kPrDeviceControlResult_GeneralError = -1;
const prSuiteError	kPrDeviceControlResult_IllegalCallSequence = -2;

/**
**	Playmod Device Control overview:
**
**	This suite is provided so that play modules can control a hardware
**	device during "Export To Tape." The necessary DeviceControlID will only
**	be passed down when a transmit is requested. The player must then call
**	all of the methods in this suite, in the order in which they are
**	listed.
*/

typedef struct 
{
	/**
	**	Tell the device to seek to the appropriate location.
	**
	**	@return					kPrDeviceControlResult_Success or kPrDeviceControlResult_IllegalCallSequence.
	*/
	prSuiteError (*Seek)(
		PlayModuleDeviceID					inDeviceID);
	
	/**
	**	Tell the device to prepare to record.
	**
	**	@return					kPrDeviceControlResult_Success or kPrDeviceControlResult_IllegalCallSequence.
	*/
	prSuiteError (*Arm)(
		PlayModuleDeviceID					inDeviceID);
	
	/**
	**	Tell the device to start recording.
	**
	**	@return					kPrDeviceControlResult_Success or kPrDeviceControlResult_IllegalCallSequence.
	*/
	prSuiteError (*Record)(
		PlayModuleDeviceID					inDeviceID);
	
	/**
	**	Tell the device to stop recording. This also cleans up the device control
	**	object, so it is not available after this call.
	**
	**	@return					kPrDeviceControlResult_Success or kPrDeviceControlResult_IllegalCallSequence.
	*/
	prSuiteError (*Stop)(
		PlayModuleDeviceID					inDeviceID);
	

} PrSDKPlayModuleDeviceControlSuite;

#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif /* PRSDKPLAYMODULEDEVICECONTROLSUITE_H */

