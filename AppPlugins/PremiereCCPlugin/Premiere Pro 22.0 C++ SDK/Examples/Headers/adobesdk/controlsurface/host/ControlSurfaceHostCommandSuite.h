/**************************************************************************
*
* ADOBE CONFIDENTIAL
* ___________________
*
*  Copyright 2011 Adobe Systems Incorporated
*  All Rights Reserved.
*
* NOTICE:  All information contained herein is, and remains the property of
* Adobe Systems Incorporated  and its suppliers,  if any.  The intellectual
* and technical concepts contained herein are proprietary to  Adobe Systems
* Incorporated  and its suppliers  and may be  covered by U.S.  and Foreign
* Patents,patents in process,and are protected by trade secret or copyright
* law.  Dissemination of this  information or reproduction of this material
* is strictly  forbidden  unless prior written permission is  obtained from
* Adobe Systems Incorporated.
**************************************************************************/

#ifndef ADOBESDK_CONTROLSURFACE_CONTROLSURFACEHOSTCOMMANDSUITE_H
#define ADOBESDK_CONTROLSURFACE_CONTROLSURFACEHOSTCOMMANDSUITE_H

// adobesdk
#include <adobesdk/config/PreConfig.h>
#include <adobesdk/controlsurface/ControlSurfaceTypes.h>

// SweetPea2
#include <SPTypes.h>

#define kADOBESDK_ControlSurfaceHostCommandSuite					"ADOBESDK ControlSurfaceHost Command Suite"
#define kADOBESDK_ControlSurfaceHostCommandSuite_Version1			1

#define kADOBESDK_ControlSurfaceHostCommandSuite_VersionCurrent	kADOBESDK_ControlSurfaceHostCommandSuite_Version1
#define	kADOBESDK_ControlSurfaceHostCommandSuiteCurrent			ADOBESDK_ControlSurfaceHostCommandSuite1


typedef struct
{
	/**
	**	Navigate by delta through the host's editor. Usually called in response of cursor keys on a control surface.
	**
	**	@param	inControlSurfaceHostCommandRef	The handle to the host command interface
	**	@param	inKeyDown						If true, this function is called as a response to a key down event, else it was a key up event.
	**	@param	inDeltaX						Amount to navigate horizontally
	**	@param	inDeltaY						Amount to navigate vertically
	**	@param	inZoom							If inZoom is true, this indicates that the host should zoom the content rather than move the edit focus.
	**
	**	@return kSPBadParameterError		one of the params is invalid
	*/
	SPErr (*Navigate)(
		ADOBESDK_ControlSurfaceHostCommandRef	inControlSurfaceHostCommandRef,
		ADOBESDK_Boolean						inKeyDown,
		int32_t									inDeltaX,
		int32_t									inDeltaY,
		ADOBESDK_Boolean						inZoom);
		
	/**
	**	Retrieve the number of commands.
	**
	**	@param	inControlSurfaceHostCommandRef	The handle to the host command interface
	**	@param	outCommandCount				Receives the command count
	**
	**	@return kSPBadParameterError		one of the params is invalid
	*/
	SPErr (*GetCommandCount)(
		ADOBESDK_ControlSurfaceHostCommandRef	inControlSurfaceHandler,
		uint32_t*								outCommandCount);

	/**
	**	Retrieve the name, ID and context of a command
	**
	**	@param	inControlSurfaceHostCommandRef	The handle to the host command interface
	**	@param	inCommandIndex					The index of the command.
	**	@param	outContextID					Receives the command's context ID.
	**	@param	outContextName					Receives the command's context UI name.
	**	@param	outCommandID					Receives the command's unique ID
	**	@param	outCommandName					Receives the command's UI name
	**
	**	@return kSPBadParameterError		one of the params is invalid
	*/
	SPErr (*GetCommand)(
		ADOBESDK_ControlSurfaceHostCommandRef	inControlSurfaceHostCommandRefRef,
		uint32_t								inCommandIndex,
		ADOBESDK_String*						outContextID,
		ADOBESDK_String*						outContextName,
		ADOBESDK_String*						outCommandID,
		ADOBESDK_String*						outCommandName);

	/**
	**	Execute a command with the given ID
	**
	**	@param	inControlSurfaceHostCommandRef	The handle to the host command interface
	**	@param	inContextID						The context in which the command shall be executed.
	**											May be NULL which implies the golbal context
	**  @param	inCommandID						the ID of the command to be executed
	**	@return kSPBadParameterError			one of the params is invalid
	*/
	SPErr (*ExecuteCommand)(
		ADOBESDK_ControlSurfaceHostCommandRef	inControlSurfaceHostCommandRefRef,
		const ADOBESDK_String*					inContextID,
		const ADOBESDK_String*					inCommandID);

} ADOBESDK_ControlSurfaceHostCommandSuite1;


#include <adobesdk/config/PostConfig.h>

#endif // ADOBESDK_CONTROLSURFACE_CONTROLSURFACEHOSTCOMMANDSUITE_H
