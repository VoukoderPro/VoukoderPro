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

#ifndef ADOBESDK_CONTROLSURFACE_CONTROLSURFACETYPES_H
#define ADOBESDK_CONTROLSURFACE_CONTROLSURFACETYPES_H

#include <adobesdk/config/PreConfig.h>
#include <adobesdk/config/AdobesdkTypes.h>

typedef struct ADOBESDK_ControlSurfaceHost* ADOBESDK_ControlSurfaceHostRef;
typedef struct ADOBESDK_ControlSurfacePlugin* ADOBESDK_ControlSurfacePluginRef;
typedef struct ADOBESDK_ControlSurface* ADOBESDK_ControlSurfaceRef;

typedef struct ADOBESDK_ControlSurfaceHostCommand* ADOBESDK_ControlSurfaceHostCommandRef;
typedef struct ADOBESDK_ControlSurfaceHostMarker* ADOBESDK_ControlSurfaceHostMarkerRef;
typedef struct ADOBESDK_ControlSurfaceHostMixer* ADOBESDK_ControlSurfaceHostMixerRef;
typedef struct ADOBESDK_ControlSurfaceHostTransport* ADOBESDK_ControlSurfaceHostTransportRef;
typedef struct ADOBESDK_ControlSurfaceHostLumetri* ADOBESDK_ControlSurfaceHostLumetriRef;

typedef struct ADOBESDK_ControlSurfaceTransport* ADOBESDK_ControlSurfaceTransportRef;
typedef struct ADOBESDK_ControlSurfaceMarker* ADOBESDK_ControlSurfaceMarkerRef;
typedef struct ADOBESDK_ControlSurfaceCommand* ADOBESDK_ControlSurfaceCommandRef;
typedef struct ADOBESDK_ControlSurfaceMixer* ADOBESDK_ControlSurfaceMixerRef;
typedef struct ADOBESDK_ControlSurfaceLumetri* ADOBESDK_ControlSurfaceLumetriRef;


typedef uint32_t ADOBESDK_ControlSurfaceHostID;

enum
{
	kADOBESDK_ControlSurfaceHost_Unknown = -1,
	kADOBESDK_ControlSurfaceHost_Audition = 0,
	kADOBESDK_ControlSurfaceHost_PremierePro = 1
};

enum
{
	kADOBESDK_ControlSurfaceTrackControlsMode_IO = 0,
	kADOBESDK_ControlSurfaceTrackControlsMode_FX = 1,
	kADOBESDK_ControlSurfaceTrackControlsMode_Sends = 2,
	kADOBESDK_ControlSurfaceTrackControlsMode_EQ = 3
};

enum
{
	kADOBESDK_ControlSurfaceTimeDisplayMode_None = 0,
	kADOBESDK_ControlSurfaceTimeDisplayMode_Decimal = 1,		///< h:m:s.f or m:s.f
	kADOBESDK_ControlSurfaceTimeDisplayMode_SMPTE = 2,			///< hh:mm:ss:ff or hh;mm;ss;ff
	kADOBESDK_ControlSurfaceTimeDisplayMode_Samples = 3,
	kADOBESDK_ControlSurfaceTimeDisplayMode_BarsAndBeats = 4	///< bar:beat.subdivision
};

typedef uint32_t ADOBESDK_ControlSurfaceAutomationMode;

enum
{
	kADOBESDK_ControlSurfaceAutomationMode_Off = 0,
	kADOBESDK_ControlSurfaceAutomationMode_Read = 1,
	kADOBESDK_ControlSurfaceAutomationMode_Latch = 2,
	kADOBESDK_ControlSurfaceAutomationMode_Touch = 3,
	kADOBESDK_ControlSurfaceAutomationMode_Write = 4
};


enum
{
	kADOBESDK_ControlSurfaceComponentIndex_Fader = 0,
	kADOBESDK_ControlSurfaceComponentIndex_Pan = 1,
	kADOBESDK_ControlSurfaceComponentIndex_EQ = 2,

	kADOBESDK_ControlSurfaceComponentIndex_InsertRack = 100,
	kADOBESDK_ControlSurfaceComponentIndex_InsertFirst = 101,
	kADOBESDK_ControlSurfaceComponentIndex_Insert1 = kADOBESDK_ControlSurfaceComponentIndex_InsertFirst,
	kADOBESDK_ControlSurfaceComponentIndex_Insert2,
	kADOBESDK_ControlSurfaceComponentIndex_Insert3,
	kADOBESDK_ControlSurfaceComponentIndex_Insert4,
	kADOBESDK_ControlSurfaceComponentIndex_Insert5,
	kADOBESDK_ControlSurfaceComponentIndex_Insert6,
	kADOBESDK_ControlSurfaceComponentIndex_Insert7,
	kADOBESDK_ControlSurfaceComponentIndex_Insert8,
	kADOBESDK_ControlSurfaceComponentIndex_Insert9,
	kADOBESDK_ControlSurfaceComponentIndex_Insert10,
	kADOBESDK_ControlSurfaceComponentIndex_Insert11,
	kADOBESDK_ControlSurfaceComponentIndex_Insert12,
	kADOBESDK_ControlSurfaceComponentIndex_Insert13,
	kADOBESDK_ControlSurfaceComponentIndex_Insert14,
	kADOBESDK_ControlSurfaceComponentIndex_Insert15,
	kADOBESDK_ControlSurfaceComponentIndex_Insert16,
	kADOBESDK_ControlSurfaceComponentIndex_InsertLast = kADOBESDK_ControlSurfaceComponentIndex_Insert16,
	
	kADOBESDK_ControlSurfaceComponentIndex_SendRack = 200,
	kADOBESDK_ControlSurfaceComponentIndex_SendFirst = 201,
	kADOBESDK_ControlSurfaceComponentIndex_Send1 = kADOBESDK_ControlSurfaceComponentIndex_SendFirst,
	kADOBESDK_ControlSurfaceComponentIndex_Send2,
	kADOBESDK_ControlSurfaceComponentIndex_Send3,
	kADOBESDK_ControlSurfaceComponentIndex_Send4,
	kADOBESDK_ControlSurfaceComponentIndex_Send5,
	kADOBESDK_ControlSurfaceComponentIndex_Send6,
	kADOBESDK_ControlSurfaceComponentIndex_Send7,
	kADOBESDK_ControlSurfaceComponentIndex_Send8,
	kADOBESDK_ControlSurfaceComponentIndex_Send9,
	kADOBESDK_ControlSurfaceComponentIndex_Send10,
	kADOBESDK_ControlSurfaceComponentIndex_Send11,
	kADOBESDK_ControlSurfaceComponentIndex_Send12,
	kADOBESDK_ControlSurfaceComponentIndex_Send13,
	kADOBESDK_ControlSurfaceComponentIndex_Send14,
	kADOBESDK_ControlSurfaceComponentIndex_Send15,
	kADOBESDK_ControlSurfaceComponentIndex_Send16,
	kADOBESDK_ControlSurfaceComponentIndex_SendLast = kADOBESDK_ControlSurfaceComponentIndex_Send16,

	kADOBESDK_ControlSurfaceComponentIndex_SendPanFirst = 300,
	kADOBESDK_ControlSurfaceComponentIndex_SendPan1 = kADOBESDK_ControlSurfaceComponentIndex_SendPanFirst,
	kADOBESDK_ControlSurfaceComponentIndex_SendPan2,
	kADOBESDK_ControlSurfaceComponentIndex_SendPan3,
	kADOBESDK_ControlSurfaceComponentIndex_SendPan4,
	kADOBESDK_ControlSurfaceComponentIndex_SendPan5,
	kADOBESDK_ControlSurfaceComponentIndex_SendPan6,
	kADOBESDK_ControlSurfaceComponentIndex_SendPan7,
	kADOBESDK_ControlSurfaceComponentIndex_SendPan8,
	kADOBESDK_ControlSurfaceComponentIndex_SendPan9,
	kADOBESDK_ControlSurfaceComponentIndex_SendPan10,
	kADOBESDK_ControlSurfaceComponentIndex_SendPan11,
	kADOBESDK_ControlSurfaceComponentIndex_SendPan12,
	kADOBESDK_ControlSurfaceComponentIndex_SendPan13,
	kADOBESDK_ControlSurfaceComponentIndex_SendPan14,
	kADOBESDK_ControlSurfaceComponentIndex_SendPan15,
	kADOBESDK_ControlSurfaceComponentIndex_SendPan16,
	kADOBESDK_ControlSurfaceComponentIndex_SendPanLast = kADOBESDK_ControlSurfaceComponentIndex_SendPan16
	
};

enum
{
	kADOBESDK_ControlSurfaceChannelStateIndex_Mute = 0,
	kADOBESDK_ControlSurfaceChannelStateIndex_Solo,
	kADOBESDK_ControlSurfaceChannelStateIndex_Record,
	kADOBESDK_ControlSurfaceChannelStateIndex_InputMonitor,
	kADOBESDK_ControlSurfaceChannelStateIndex_InvertPhase,
	kADOBESDK_ControlSurfaceChannelStateIndex_PreRender
};

typedef uint32_t ADOBESDK_ControlSurfaceChannelID;

enum
{
	kADOBESDK_ControlSurfaceChannelID_None = 0
};

enum
{
	kADOBESDK_ControlSurfaceFaderParameter_Gain = 0
};

enum
{
	kADOBESDK_ControlSurfacePanType_None = 0,
	kADOBESDK_ControlSurfacePanType_Stereo = 1,
	kADOBESDK_ControlSurfacePanType_MonoTo51 = 2,
	kADOBESDK_ControlSurfacePanType_StereoTo51 = 3
};

enum
{
	kADOBESDK_ControlSurfaceStereoPanParameter_Pan = 0
};

enum
{
	kADOBESDK_ControlSurfaceMonoTo51PanParameter_RadAngle = 0,
	kADOBESDK_ControlSurfaceMonoTo51PanParameter_Radius = 1,
	kADOBESDK_ControlSurfaceMonoTo51PanParameter_CenterLevel = 2,
	kADOBESDK_ControlSurfaceMonoTo51PanParameter_LFELevel = 3,
};

enum
{
	kADOBESDK_ControlSurfaceStereoTo51PanParameter_RadAngle = 0,
	kADOBESDK_ControlSurfaceStereoTo51PanParameter_Radius = 1,
	kADOBESDK_ControlSurfaceStereoTo51PanParameter_CenterLevel = 2,
	kADOBESDK_ControlSurfaceStereoTo51PanParameter_LFELevel = 3,
	kADOBESDK_ControlSurfaceStereoTo51PanParameter_Spread = 4
};

enum
{
	kADOBESDK_ControlSurfacePremiere51PanParameter_LeftRight = 0,
	kADOBESDK_ControlSurfacePremiere51PanParameter_FrontRear = 1,
	kADOBESDK_ControlSurfacePremiere51PanParameter_Center = 2,
	kADOBESDK_ControlSurfacePremiere51PanParameter_LFELevel = 3,
};

enum
{
	kADOBESDK_ControlSurfaceSendParameter_Level = 0
};

enum
{
	kADOBESDK_ControlSurfaceChannelFlag_HasRecord		= 1 << 0,
	kADOBESDK_ControlSurfaceChannelFlag_HasSolo			= 1 << 1,
	kADOBESDK_ControlSurfaceChannelFlag_HasMute			= 1 << 2,
	kADOBESDK_ControlSurfaceChannelFlag_HasInputMonitor = 1 << 3,
	kADOBESDK_ControlSurfaceChannelFlag_HasInvertPhase	= 1 << 4,
	kADOBESDK_ControlSurfaceChannelFlag_HasPreRender	= 1 << 5,

	kADOBESDK_ControlSurfaceChannelFlag_HasInserts		= 1 << 6,
	kADOBESDK_ControlSurfaceChannelFlag_HasSends		= 1 << 7,
	kADOBESDK_ControlSurfaceChannelFlag_HasPan			= 1 << 8,

	kADOBESDK_ControlSurfaceChannelFlag_IsAudioClip		= 1 << 16,
	kADOBESDK_ControlSurfaceChannelFlag_IsBus			= 1 << 17,
	kADOBESDK_ControlSurfaceChannelFlag_IsMetronome		= 1 << 18,
	kADOBESDK_ControlSurfaceChannelFlag_IsMaster		= 1 << 19

};

enum
{
	kADOBESDK_ControlSurfaceLumetriPanelMode_Hidden				= 0,
	kADOBESDK_ControlSurfaceLumetriPanelMode_BasicCorrection	= 1,
	kADOBESDK_ControlSurfaceLumetriPanelMode_Creative			= 2,
	kADOBESDK_ControlSurfaceLumetriPanelMode_Curves				= 3,
	kADOBESDK_ControlSurfaceLumetriPanelMode_ColorWheels		= 4,
	kADOBESDK_ControlSurfaceLumetriPanelMode_HSLSecondary		= 5,
	kADOBESDK_ControlSurfaceLumetriPanelMode_Vignette			= 6
};

#include <adobesdk/config/PostConfig.h>

#endif // ADOBESDK_CONTROLSURFACE_CONTROLSURFACETYPES_H
