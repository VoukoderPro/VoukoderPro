/**************************************************************************
*
* ADOBE CONFIDENTIAL
* ___________________
*
*  Copyright 2016 Adobe Systems Incorporated
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

#ifndef ADOBESDK_CONTROLSURFACE_CONTROLSURFACEHOSTLUMETRISUITE_H
#define ADOBESDK_CONTROLSURFACE_CONTROLSURFACEHOSTLUMETRISUITE_H

// adobesdk
#include <adobesdk/config/PreConfig.h>
#include <adobesdk/controlsurface/ControlSurfaceTypes.h>

// SweetPea2
#include <SPTypes.h>

#define kADOBESDK_ControlSurfaceHostLumetriSuite					"ADOBESDK ControlSurfaceHost Lumetri Suite"
#define kADOBESDK_ControlSurfaceHostLumetriSuite_Version1			1

#define kADOBESDK_ControlSurfaceHostLumetriSuite_VersionCurrent		kADOBESDK_ControlSurfaceHostLumetriSuite_Version1
#define	kADOBESDK_ControlSurfaceHostLumetriSuiteCurrent				ADOBESDK_ControlSurfaceHostLumetriSuite1


typedef struct
{
	/**
	**	Retrieve the number of parameters.
	**
	**	@param	inControlSurfaceHostLumetriRef	The handle to the host Lumetri interface
	**	@param	outParameterCount				Receives the parameter count
	**
	**	@return kSPBadParameterError			one of the params is invalid
	*/
	SPErr (*GetParameterCount)(
		ADOBESDK_ControlSurfaceHostLumetriRef	inControlSurfaceHostLumetriRef,
		uint32_t*								outParameterCount);

	/**
	**	Retrieve the name, and ID of a parameter
	**
	**	@param	inControlSurfaceHostLumetriRef	The handle to the host Lumetri interface
	**	@param	inParameterIndex				The index of the parameter.
	**	@param	outParameterID					Receives the parameter's unique ID
	**	@param	outParameterName				Receives the parameter's UI name
	**	@param	outMinimumValue					Receives the smallest value the parameter can have
	**	@param	outMaximumValue					Receives the largest value the parameter can have
	**
	**	@return kSPBadParameterError			one of the params is invalid
	*/
	SPErr (*GetParameter)(
		ADOBESDK_ControlSurfaceHostLumetriRef	inControlSurfaceHostLumetriRef,
		uint32_t								inParameterIndex,
		ADOBESDK_String*						outParameterID,
		ADOBESDK_String*						outParameterName,
		float*									outMinimumValue,
		float*									outMaximumValue);

	/**
	**	Get the value of the parameter with the given ID
	**
	**	@param	inControlSurfaceHostLumetriRef	The handle to the host Lumetri interface
	**  @param	inParameterID					the ID of the parameter to be set
	**	@param	outParameterValue				the current value of the parameter
	**
	**	@return kSPBadParameterError			one of the params is invalid
	*/
	SPErr (*GetParameterValue)(
		ADOBESDK_ControlSurfaceHostLumetriRef	inControlSurfaceHostLumetriRef,
		const ADOBESDK_String*					inParameterID,
		float*									outParameterValue);

	/**
	**	Change the value of the parameter with the given ID
	**
	**	@param	inControlSurfaceHostLumetriRef	The handle to the host Lumetri interface
	**  @param	inParameterID					the ID of the parameter
	**	@param	inDelta							the amount by which the value should be changed
	**	@param	outNewValue						the new value of the parameter after the change
	**
	**	@return kSPBadParameterError			one of the params is invalid
	*/
	SPErr (*ChangeParameterValue)(
		ADOBESDK_ControlSurfaceHostLumetriRef	inControlSurfaceHostLumetriRef,
		const ADOBESDK_String*					inParameterID,
		float									inDelta,
		float*									outNewValue);

	/**
	**	Reset the value of the parameter with the given ID to its default value
	**
	**	@param	inControlSurfaceHostLumetriRef	The handle to the host Lumetri interface
	**  @param	inParameterID					the ID of the parameter
	**	@param	outNewValue						the new value of the parameter after the change
	**
	**	@return kSPBadParameterError			one of the params is invalid
	*/
	SPErr (*ResetParameter)(
		ADOBESDK_ControlSurfaceHostLumetriRef	inControlSurfaceHostLumetriRef,
		const ADOBESDK_String*					inParameterID,
		float*									outNewValue);

	/**
	**	Retrieve the number of menus.
	**
	**	@param	inControlSurfaceHostLumetriRef	The handle to the host Lumetri interface
	**	@param	outMenuCount					Receives the menu count
	**
	**	@return kSPBadParameterError			one of the params is invalid
	*/
	SPErr (*GetMenuCount)(
		ADOBESDK_ControlSurfaceHostLumetriRef	inControlSurfaceHostLumetriRef,
		uint32_t*								outMenuCount);

	/**
	**	Retrieve the name, and ID of a menu
	**
	**	@param	inControlSurfaceHostLumetriRef	The handle to the host Lumetri interface
	**	@param	inMenuIndex						The index of the menu.
	**	@param	outMenuID						Receives the menu's unique ID
	**	@param	outMenuName						Receives the menu's UI name
	**
	**	@return kSPBadParameterError			one of the params is invalid
	*/
	SPErr (*GetMenu)(
		ADOBESDK_ControlSurfaceHostLumetriRef	inControlSurfaceHostLumetriRef,
		uint32_t								inMenuIndex,
		ADOBESDK_String*						outMenuID,
		ADOBESDK_String*						outMenuName);

	/**
	**	Get the current value of the menu with the given ID
	**
	**	@param	inControlSurfaceHostLumetriRef	The handle to the host Lumetri interface
	**  @param	inMenuID						the ID of the menu
	**	@param	outMenuValue					the current value of the menu
	**
	**	@return kSPBadParameterError			one of the params is invalid
	*/
	SPErr (*GetMenuValue)(
		ADOBESDK_ControlSurfaceHostLumetriRef	inControlSurfaceHostLumetriRef,
		const ADOBESDK_String*					inMenuID,
		ADOBESDK_String*						outMenuValue);

	/**
	**	Modify the value of the menu with the given ID
	**
	**	@param	inControlSurfaceHostLumetriRef	The handle to the host Lumetri interface
	**  @param	inMenuID						the ID of the menu to be set
	**	@param	inDelta							the number of entries (positive or negative) to add to the current value
	**	@param	outNewValue						the new value of the menu after change
	**
	**	@return kSPBadParameterError			one of the params is invalid
	*/
	SPErr (*ChangeMenuValue)(
		ADOBESDK_ControlSurfaceHostLumetriRef	inControlSurfaceHostLumetriRef,
		const ADOBESDK_String*					inMenuID,
		int32_t									inDelta,
		ADOBESDK_String*						outNewValue);

	/**
	**	Reset the menu with the given ID to its default value
	**
	**	@param	inControlSurfaceHostLumetriRef	The handle to the host Lumetri interface
	**  @param	inMenuID						the ID of the menu to be set
	**	@param	outNewValue						the new value of the menu after reset
	**
	**	@return kSPBadParameterError			one of the params is invalid
	*/
	SPErr (*ResetMenu)(
		ADOBESDK_ControlSurfaceHostLumetriRef	inControlSurfaceHostLumetriRef,
		const ADOBESDK_String*					inMenuID,
		ADOBESDK_String*						outNewValue);

	/**
	**	Set the Lumetri panel to a specific mode
	**
	**	@param	inControlSurfaceHostLumetriRef	The handle to the host Lumetri interface
	**  @param	inDesiredMode					the desired mode (modes are defined in ControlSurfaceTypes.h)
	**	@param	outNewMode						the new mode of the Lumetri panel (usually inDesiredMode, but not guaranteed)
	**
	**	@return kSPBadParameterError			one of the params is invalid
	*/
	SPErr (*SetMode)(
		ADOBESDK_ControlSurfaceHostLumetriRef	inControlSurfaceHostLumetriRef,
		uint32_t								inDesiredMode,
		uint32_t*								outNewMode);

} ADOBESDK_ControlSurfaceHostLumetriSuite1;


#include <adobesdk/config/PostConfig.h>

#endif // ADOBESDK_CONTROLSURFACE_CONTROLSURFACEHOSTLUMETRISUITE_H
