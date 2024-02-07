/* $Id: //premiere/main/Main/r/Mac/US/PiPL.r#10 $ */

//-------------------------------------------------------------------------------
//
//	File:
//		PiPL.r
//
//	Copyright 1991-2000, Adobe Systems Incorporated.
//	All Rights Reserved.
//
//	Distribution:
//		PUBLIC
//
//	Description:
//		This file contains the public resource templates
//		for all PiPL types for all graphic and dynamic
//		media products.
//
//	Use:
//		This file must be included to correctly compile any
//		resource (.r) files with PiPLs defined in them.
//
//	Version history:
//		Version 1.0.0	10/9/1997	Cnvtpipl 3.0.1		Ace		Initial compilation.
//			Compiled for Photoshop, After Effects, Premiere, Illustrator.
//
//		Version 1.0.1	3/9/1998	Cnvtpipl 3.0.37		Ace		Premiere/PICA updates.
//			PrImporter, PrCompile, PrRecord, PrPlay, AdapterVersion, InternalName.
//
//		Version 1.0.2	8/26/98		Cnvtpipl 3.0.39		BA		Premiere 5.1 updates.
//			Activated the version field in Pr_Effect_Info and added two new flags - 
//			exclusiveDialog (bit 6) and directCompData (bit 4). For backwards compatibility,
//			these are protected by #if/#endif blocks which are keyed off the PiPL version
//			number. PiPL version 2.2 turns on the new fields.
//
//-------------------------------------------------------------------------------

#ifndef __PIPL_r__		// Only include this once.
#define __PIPL_r__

//-------------------------------------------------------------------------------
//	Definitions -- Version and include information
//-------------------------------------------------------------------------------

// Version of Plug In Properties Template described in this file:
#ifndef kPIPropertiesVersion
	#define kPIPropertiesVersion 0
#endif

//-------------------------------------------------------------------------------
//	Don't include this file on MSWindows
//-------------------------------------------------------------------------------

#define qIncludeMe 1

#ifdef MSWindows
	#undef qIncludeMe
	#define qIncludeMe !MSWindows
#endif

#if qIncludeMe // Not MSWindows

//-------------------------------------------------------------------------------
//	Temporarily define PiPL types for all references:
//-------------------------------------------------------------------------------

#define PIPiPLTypes								\
	/* Photoshop plug-in types: */				\
	General = '8BPI',							\
	Filter = '8BFM',							\
	Parser = '8BYM',							\
	ImageFormat='8BIF',							\
	Extension = '8BXM',							\
	Acquire = '8BAM',							\
	Export = '8BEM',							\
	Selection = '8BSM',							\
	Picker = '8BCM',							\
	Actions = '8LIZ',							\
	Test = '8BTS',								\
	MSPUtility = '8SPU'							\
	PsModernFilter='8BFm'						\
	/* After Effects plug-in types: */			\
	AEEffect = 'eFKT', 							\
	AEImageFormat = 'FXIF', 					\
	AEAccelerator = 'eFST', 					\
	AEGeneral = 'AEgp',							\
	/* Premiere plug-in types: */				\
	PrEffect = 'SPFX', 							\
	PrVideoFilter = 'VFlt', 					\
	PrAudioFilter = 'AFlt', 					\
	PrEDLExport = 'ExpM', 						\
	PrDataExport = 'ExpD', 						\
	PrDevice = 'DevC', 							\
	PrImporter = 'IMPT'							\
	PrCompile = 'CMPM',							\
	PrRecord = 'RECM',							\
	PrPlay = 'PLYM',							\
	/* Illustrator/SweetPea plug-in types: */	\
	SweetPea = 'SPEA'							\
	AIGeneral = 'ARPI'

//-------------------------------------------------------------------------------
//	PiPL resource template
//-------------------------------------------------------------------------------

type 'PiPL'
	{
	longint = kPIPropertiesVersion;
	longint = $$CountOf(properties);
	array properties
		{
		switch
			{


			//-------------------------------------------------------------------
			// General PiPL properties
			//-------------------------------------------------------------------
			case Kind:
				longint = '8BIM';
				key longint = 'kind';
				longint = 0;
				longint = 4;
				literal longint PIPiPLTypes;

			case Version:
				longint = '8BIM';
				key longint = 'vers';
				longint = 0;
				longint = 4;
				longint;

			case Priority:
				longint = '8BIM';
				key longint = 'prty';
				longint = 0;
				longint = 4;
				longint;

			case Component:
				longint = '8BIM';
				key longint = 'cmpt';
				longint = 0;
				#if DeRez
					fill long;
				#else
					longint = (componentEnd[$$ArrayIndex(properties)] - componentStart[$$ArrayIndex(properties)]) / 8;
				#endif
			  componentStart:
				longint;				// Version.
				cstring;				// UUID.
			  componentEnd:
			  	align long;
			  	
			case RequiredHost:
				longint = '8BIM';
				key longint = 'host';
				longint = 0;
				longint = 4;
				literal longint;

			case Name:
				longint = '8BIM';
				key longint = 'name';
				longint = 0;
				#if DeRez
					fill long;
				#else
					longint = (nameEnd[$$ArrayIndex(properties)] - nameStart[$$ArrayIndex(properties)]) / 8;
				#endif
			  nameStart:
				pstring;
			  nameEnd:
				align long;

			case Category:
				longint = '8BIM';
				key longint = 'catg';
				longint = 0;
				#if DeRez
					fill long;
				#else
					longint = (catgEnd[$$ArrayIndex(properties)] - catgStart[$$ArrayIndex(properties)]) / 8;
				#endif
			  catgStart:
				pstring
					PSHelpMenu = "**Help**";	// Use this to place actions plug-ins in the help menu.
			  catgEnd:
				align long;

			// May no longer be supported in current versions, but kept
			// for backwards and PUBLIC compatibility:
			case Code68k:
				longint = '8BIM';
				key longint = 'm68k';
				longint = 0;
				longint = 6;
				literal longint PIPiPLTypes;
				integer;
				align long;

			case Code68kFPU:
				longint = '8BIM';
				key longint = '68fp';
				longint = 0;
				longint = 6;
				literal longint PIPiPLTypes;
				integer;
				align long;

			case CodePowerPC:
				longint = '8BIM';
				key longint = 'pwpc';
				longint = 0;
				#if DeRez
					fill long;
				#else
					longint = (pwpcEnd[$$ArrayIndex(properties)] - pwpcStart[$$ArrayIndex(properties)]) / 8;
				#endif
			  pwpcStart:
				longint;
				longint;
				pstring;
			  pwpcEnd:
				align long;

			// For documentation purposes, here's the Windows
			// code resource template:
			#ifdef MSWindows
				case CodeWin32X86:
					longint = '8BIM';
					key longint = 'wx86';
					longint = 0;
					longint = (win32x86End[$$ArrayIndex(properties)] - win32x86Start[$$ArrayIndex(properties)]) / 8;
				  win32x86Start:
					cstring;
				  win32x86End:
					align long;
			#endif

			case SupportedModes:
				longint = '8BIM';
				key longint = 'mode';
				longint = 0;
				#if DeRez
					fill long;
				#else
					longint = (modeEnd[$$ArrayIndex(properties)] - modeStart[$$ArrayIndex(properties)]) / 8;
				#endif
			  modeStart:
				boolean noBitmap, doesSupportBitmap;
				boolean noGrayScale, doesSupportGrayScale;
				boolean noIndexedColor, doesSupportIndexedColor;
				boolean noRGBColor, doesSupportRGBColor;
				boolean noCMYKColor, doesSupportCMYKColor;
				boolean noHSLColor, doesSupportHSLColor;
				boolean noHSBColor, doesSupportHSBColor;
				boolean noMultichannel, doesSupportMultichannel;
				boolean noDuotone, doesSupportDuotone;
				boolean noLABColor, doesSupportLABColor;
				fill bit[6];
			  modeEnd:
				align long;

			case EnableInfo:
				longint = '8BIM';
				key longint = 'enbl';
				longint = 0;
				#if DeRez
					fill long;
				#else
					longint = (EnableInfoEnd[$$ArrayIndex(properties)] - EnableInfoStart[$$ArrayIndex(properties)]) / 8;
				#endif
			  EnableInfoStart:
				cstring;
			  EnableInfoEnd:
				align long;

			//-------------------------------------------------------------------
			// Photoshop Filter PiPL properties
			//-------------------------------------------------------------------
			case FilterCaseInfo:
				longint = '8BIM';
				key longint = 'fici';
				longint = 0;
				longint = 28;
				array [7]
					{
					byte inCantFilter = 0,
						 inStraightData = 1,
						 inBlackMat = 2,
						 inGrayMat = 3,
						 inWhiteMat = 4,
						 inDefringe = 5,
						 inBlackZap = 6,
						 inGrayZap = 7,
						 inWhiteZap = 8,
						 inBackgroundZap = 10,
						 inForegroundZap = 11;
					byte outCantFilter = 0,
						 outStraightData = 1,
						 outBlackMat = 2,
						 outGrayMat = 3,
						 outWhiteMat = 4,
						 outFillMask = 9;
					fill bit [4];
					boolean doNotWriteOutsideSelection, writeOutsideSelection;
					boolean doesNotFilterLayerMasks, filtersLayerMasks;
					boolean doesNotWorkWithBlankData, worksWithBlankData;
					boolean copySourceToDestination, doNotCopySourceToDestination;
					fill byte;
					};

			//-------------------------------------------------------------------
			// Photoshop Export PiPL properties
			//-------------------------------------------------------------------
			case ExportFlags:
				longint = '8BIM';
				key longint = 'expf';
				longint = 0;
				#if DeRez
					fill long;
				#else
					longint = (expFlagsEnd[$$ArrayIndex(properties)] - expFlagsStart[$$ArrayIndex(properties)]) / 8;
				#endif
			 expFlagsStart:
				boolean expDoesNotSupportTransparency, expSupportsTransparency;
				fill bit[7];
			  expFlagsEnd:
				align long;

			//-------------------------------------------------------------------
			// Photoshop File Format PiPL properties
			//-------------------------------------------------------------------
			case FmtFileType:
				longint = '8BIM';
				key longint = 'fmTC';
				longint = 0;
				longint = 8;
				literal longint; // Default file type.
				literal longint; // Default file creator.

			// NOTE: If you specify you can READ type 'foo_', then you
			// will never be called with a FilterFile for type 'foo_'.
			case ReadTypes:
				longint = '8BIM';
				key longint = 'RdTy';
				longint = 0;
				longint = $$CountOf(ReadableTypes) * 8;
				wide array ReadableTypes { literal longint; literal longint; } ;

			case WriteTypes:
				longint = '8BIM';
				key longint = 'WrTy';
				longint = 0;
				longint = $$CountOf(WritableTypes) * 8;
				wide array WritableTypes { literal longint; literal longint; } ;

			// NOTE: If you specify you want to filter type 'foo_' AND you
			// specify you can read type 'foo_', you will never get
			// a filter call.
			case FilteredTypes:
				longint = '8BIM';
				key longint = 'fftT';
				longint = 0;
				longint = $$CountOf(FilteredTypes) * 8;
				wide array FilteredTypes { literal longint; literal longint; } ;

			// Macintosh plug-ins can use Windows file extensions
			// to determine read/write/parseability.
			//
			// NOTE: If you specify you READ extension '.foo' then you
			// won't be called to Filter that type.
			case ReadExtensions:
				longint = '8BIM';
				key longint = 'RdEx';
				longint = 0;
				longint = $$CountOf(ReadableExtensions) * 4;
				wide array ReadableExtensions { literal longint; } ;

			case WriteExtensions:
				longint = '8BIM';
				key longint = 'WrEx';
				longint = 0;
				longint = $$CountOf(WriteableExtensions) * 4;
				wide array WriteableExtensions { literal longint; } ;

			// NOTE: If you specify you want to filter extension '.foo'
			// AND you specify you can read extension '.foo', you will
			// never get a filter call.
			case FilteredExtensions:
				longint = '8BIM';
				key longint = 'fftE';
				longint = 0;
				longint = $$CountOf(FilteredExtensions) * 4;
				wide array FilteredExtensions { literal longint; } ;

			case FormatFlags:
				longint = '8BIM';
				key longint = 'fmtf';
				longint = 0;
				longint = (fmtFlagsEnd[$$ArrayIndex(properties)] - fmtFlagsStart[$$ArrayIndex(properties)]) / 8;
			 fmtFlagsStart:
				boolean = false; // Obsolete.
				boolean fmtDoesNotSaveImageResources, fmtSavesImageResources;
				boolean fmtCannotRead, fmtCanRead;
				boolean fmtCannotWrite, fmtCanWrite;
				boolean fmtWritesAll, fmtCanWriteIfRead;
				fill bit[3];
			  fmtFlagsEnd:
				align long;

			case FormatMaxSize:
				longint = '8BIM';
				key longint = 'mxsz';
				longint = 0;
				longint = 4;
				Point;

			case FormatMaxChannels:
				longint = '8BIM';
				key longint = 'mxch';
				longint = 0;
				longint = $$CountOf(ChannelsSupported) * 2;
				wide array ChannelsSupported { integer; } ;
				align long;

			//-------------------------------------------------------------------
			// Photoshop Parser PiPL properties
			//-------------------------------------------------------------------
			// NOTE: If you specify you want to filter type 'foo_' and you
			// specify you can parse type 'foo_', you will never get a
			// filter call.
			case ParsableTypes:
				longint = '8BIM';
				key longint = 'psTY';
				longint = 0;
				longint = $$CountOf(ParsableTypes) * 8;
				wide array ParsableTypes { literal longint; literal longint; } ;

			case ParsableClipboardTypes:
				longint = '8BIM';
				key longint = 'psCB';
				longint = 0;
				longint = $$CountOf(ParsableClipboardTypes) * 4;
				wide array ParsableClipboardTypes { literal longint; };

			// NOTE: If you want to filter type 'foo_' and you specify you
			// can parse type 'foo_', you will never get a filter call.
			case FilteredParsableTypes:
				longint = '8BIM';
				key longint = 'psTy';
				longint = 0;
				longint = $$CountOf(ParsableTypes) * 8;
				wide array ParsableTypes { literal longint; literal longint; } ;


			// Macintosh plug-ins can use Windows file extensions
			// to determine read/write/parseability.
			//
			// NOTE: If you want to filter extension '.foo' and you
			// specify you can parse extension '.foo', you will
			// never get a filter call.
			case ParsableExtensions:
				longint = '8BIM';
				key longint = 'psEX';
				longint = 0;
				longint = $$CountOf(ParsableExtensions) * 4;
				wide array ParsableExtensions { literal longint; };

			case FilteredParsableExtensions:
				longint = '8BIM';
				key longint = 'psEx';
				longint = 0;
				longint = $$CountOf(ParsableExtensions) * 4;
				wide array ParsableExtensions { literal longint; };

			//-------------------------------------------------------------------
			// Photoshop Parser PiPL properties
			//-------------------------------------------------------------------
			case PickerID:
				longint = '8BIM';
				key longint = 'pnme';
				longint = 0;
				#if DeRez
					fill long;
				#else
					longint = (PickerIDEnd[$$ArrayIndex(properties)] - PickerIDStart[$$ArrayIndex(properties)]) / 8;
				#endif
			  PickerIDStart:
				pstring;			// Unique ID string.
			  PickerIDEnd:
				align long;

			//-------------------------------------------------------------------
			// Photoshop Actions/Scripting PiPL properties
			// (Photoshop 4.0 and later)
			//-------------------------------------------------------------------
			case HasTerminology:
				longint = '8BIM';
				key longint = 'hstm';
				longint = 0;
				longint = (hasTermEnd[$$ArrayIndex(properties)] - hasTermStart[$$ArrayIndex(properties)]) / 8;
			hasTermStart:
				longint = 0;	// Version.
				longint;		// Class ID, always required.  Can be Suite ID.
				longint;		// Event ID, or typeNULL if not Filter/Color Picker/Selection.
				integer;		// Dictionary ('AETE') resource ID.
				cstring;		// Unique scope string.  Always required in Photoshop 5.0 and later.
			hasTermEnd:
				align long;

			// If this property is present, then its on.  No parameters
			// are required:
			case Persistent:
				longint = '8BIM';
				key longint = 'prst';
				longint = 0;	// Index.
				longint = 4; 	// Length.
				literal longint = 1;	// If specified, always on.

			//-------------------------------------------------------------------
			// After Effects and Premiere specific PiPL properties
			//-------------------------------------------------------------------
			case AE_PiPL_Version:
				longint = '8BIM';
				key longint = 'ePVR';
				longint = 0;
				longint = 4;
				integer;		// Minor version. *NOTE: May have these reversed.
				integer;		// Major version.
				
			case AE_Effect_Spec_Version:
				longint = '8BIM';
				key longint = 'eSVR';
				longint = 0;
				longint = 4;
				integer;		// Minor version. *NOTE: May have these reversed.
				integer;		// Major version.

			case AE_Effect_Version:
				longint = '8BIM';
				key longint = 'eVER';
				longint = 0;
				longint = 4;
				longint;
				
			case AE_Effect_Match_Name:
				longint = '8BIM';
				key longint = 'eMNA';
				longint = 0;
				#if DeRez
					fill long;
				#else
					longint = (matchNameEnd[$$ArrayIndex(properties)] - matchNameStart[$$ArrayIndex(properties)]) / 8;
				#endif
			  matchNameStart:
				pstring;
			  matchNameEnd:
				align long;

			//-------------------------------------------------------------------
			// After Effects effects PiPL properties
			//-------------------------------------------------------------------
			case AE_Effect_Info_Flags:
				longint = '8BIM';
				key longint = 'eINF';
				longint = 0;
				longint = 4; 	// NOTE CHANGE: Was 2, but everything must be 4 byte aligned.
				// integer;		// Was defined this way.
				// align long;	// Was defined this way.
				longint;		// This is consistent with Windows resource.

			case AE_Effect_Global_OutFlags:
				longint = '8BIM';
				key longint = 'eGLO';
				longint = 0;	// Index.
				longint = 4;	// Length.
				fill bit[6];	// Reserved.
				boolean=0; // PF_OutFlag_RESERVED4
				boolean=0; // PF_OutFlag_RESERVED3
				boolean=0; // PF_OutFlag_RESERVED2
				boolean=0; // PF_OutFlag_RESERVED1
				boolean notObsolete, obsolete; 							// PF_OutFlag_I_AM_OBSOLETE
				boolean dontUseAudio, useAudio; 						// PF_OutFlag_I_USE_AUDIO
				boolean dontUseShutterAngle, useShutterAngle;			// PF_OutFlag_I_USE_SHUTTER_ANGLE
				boolean noFrameNopRender, useFrameNopRender;			// PF_OutFlag_NOP_RENDER (frame)
				boolean noRefreshUIEvent, useRefreshUIEvent;			// PF_OutFlag_REFRESH_UI (event)
				boolean noCustomNTRP, haveCustomNTRP;					// PF_OutFlag_CUSTOM_NTRP
				boolean noCustomUI, haveCustomUI;						// PF_OutFlag_CUSTOM_UI
				boolean nonSquarePixOk, squarePixOnly;					// PF_OutFlag_SQUARE_PIX_ONLY
				boolean dontWorkInPlace, workInPlace;					// PF_OutFlag_WORKS_IN_PLACE
				boolean doesntShrinkBuffer, shrinksBuffer;				// PF_OutFlag_I_SHRINK_BUFFER
				boolean doesntWriteInputBuffer, writesInputBuffer;		// PF_OutFlag_I_WRITE_INPUT_BUFFER
				boolean pixDependent, pixIndependent;					// PF_OutFlag_PIX_INDEPENDENT
				boolean doesntExpandBuffer, expandsBuffer;				// PF_OutFlag_I_EXPAND_BUFFER
				boolean dontDisplayErrorMessage, displayErrorMessage;	// PF_OutFlag_DISPLAY_ERROR_MESSAGE
				boolean dontSendDoDialog, sendDoDialog;					// PF_OutFlag_SEND_DO_DIALOG (sequence)
				boolean dontUseOutputExtent, useOutputExtent;			// PF_OutFlag_USE_OUTPUT_EXTENT
				boolean doesntDoDialog, doesDialog;						// PF_OutFlag_I_DO_DIALOG
				boolean dontFlattenSequenceData, flattenSequenceData;	// PF_OutFlag_SEQUENCE_DATA_NEEDS_FLATTENING
				boolean dontSendParamsUpdate, sendParamsUpdate;			// PF_OutFlag_SEND_PARAMS_UPDATE
				boolean noNonParamVary, nonParamVary;					// PF_OutFlag_NON_PARAM_VARY
				boolean noWideTimeInput, wideTimeInput;					// PF_OutFlag_WIDE_TIME_INPUT
				boolean dontKeepResourceOpen, keepResourceOpen;			// PF_OutFlag_KEEP_RESOURCE_OPEN

			case AE_Reserved:
				longint = '8BIM';
				key longint = 'aeRD';
				longint = 0;
				longint = 4;
				longint;

			case AE_Reserved_Info:
				longint = '8BIM';
				key longint = 'aeFL';
				longint = 0;
				longint = 4;
				longint;

			//-------------------------------------------------------------------
			// After Effects Image Format Extension PiPL properties
			//-------------------------------------------------------------------
			case AE_ImageFormat_Extension_Info:
				longint = '8BIM';
				key longint = 'FXMF';
				longint = 0;
				longint = 16;
				integer;		// Major version.
				integer;		// Minor version.
				fill bit[21];
				boolean hasOptions, hasNoOptions;
				boolean sequentialOnly, nonSequentialOk;
				boolean noInteractRequired, mustInteract;
				boolean noInteractPut, hasInteractPut;
				boolean noInteractGet, hasInteractGet;
				boolean hasTime, hasNoTime;
				boolean noVideo, hasVideo;
				boolean noStill, still;
				boolean noFile, hasFile;
				boolean noOutput, output;
				boolean noInput, input;

				longint = 0;		// Reserved.
				literal longint;	// Signature.

			//-------------------------------------------------------------------
			// After Effects and Premiere ANIM PiPL properties
			//-------------------------------------------------------------------
			case ANIM_FilterInfo:
				longint = '8BIM';
				key longint = 'aFLT';
				longint = 0;	// Index.
				#if DeRez
					fill long;
				#else
					longint = (animFilterEnd[$$ArrayIndex(properties)] - animFilterStart[$$ArrayIndex(properties)]) / 8;
				#endif

			  animFilterStart:
			  	longint=1;		// spec_version_major (AE & PrMr)
			  	longint=1;		// spec_version_minor (AE & PrMr)
			  	longint;		// filter_params_version (AE only)

#ifdef PiPLVer2p3
				fill bit[14];
				boolean notUnityPixelAspectRatio, unityPixelAspectRatio; // ANIM_FF_UNITY_PAR
				boolean notAnyPixelAspectRatio, anyPixelAspectRatio; // ANIM_FF_ANY_PAR
				boolean reserved4False, reserved4True; 		// ANIM_FF_RESERVED4 (spare)
				boolean reserved3False, reserved3True; 		// ANIM_FF_RESERVED3 (spare)
				boolean reserved2False, reserved2True; 		// ANIM_FF_RESERVED2 (spare)
#else
				fill bit[19];
#endif
				boolean reserved1False, reserved1True; 		// ANIM_FF_RESERVED1 (AE only)
				boolean reserved0False, reserved0True; 		// ANIM_FF_RESERVED0 (AE only)
				boolean driveMe, dontDriveMe; 				// ANIM_FF_DONT_DRIVE_ME (AE only)
				boolean needsDialog, doesntNeedDialog;		// ANIM_FF_DOESNT_NEED_DLOG (AE only)
				boolean paramsNotPointer, paramsPointer;	// ANIM_FF_PARAMS_ARE PTR (AE only)
				boolean paramsNotHandle, paramsHandle;		// ANIM_FF_PARAMS_ARE_HANDLE (AE only)
				boolean paramsNotMacHandle,paramsMacHandle;	// ANIM_FF_PARAMS_ARE_MAC_HANDLE (AE only)
				boolean dialogNotInRender, dialogInRender;	// ANIM_FF_DIALOG_IN_RENDER (AE only)
				boolean paramsNotInGlobals,paramsInGlobals;	// ANIM_FF_PARAMS_IN_GLOBALS (AE only)
				boolean bgNotAnimatable, bgAnimatable;		// ANIM_FF_BG_ANIMATABLE (AE only)
				boolean fgNotAnimatable, fgAnimatable;		// ANIM_FF_FG_ANIMATABLE (AE only)
				boolean geometric, notGeometric;			// ANIM_FF_NON_GEOMETRIC (AE only)
				boolean noRandomness, randomness;			// ANIM_FF_HAS_RANDOMNESS (AE only)

				longint;		// number of parameters

				cstring[32];	// match name

			  	longint=0;		// Operates in place - not currently implemented
			  	longint=0;		// reserved
			  	longint=0;		// reserved
			  	longint=0;		// reserved
			  animFilterEnd:

			case ANIM_ParamAtom:
				longint = '8BIM';
				key longint = 'aPAR';
				longint;		// property id *NOTE: Breaks model -- MUST SPECIFY.
				#if DeRez
					fill long;
				#else
					longint = (animParamEnd[$$ArrayIndex(properties)] - animParamStart[$$ArrayIndex(properties)]) / 8;
				#endif

			  animParamStart:
				cstring[32];					// external name

			  	longint;						// match id
			  	
			  	longint ANIM_DT_OPAQUE, 		// obsolete, don't use OPAQUE with Premiere
			  			ANIM_DT_CHAR,
						ANIM_DT_SHORT,
						ANIM_DT_LONG,
						ANIM_DT_UNSIGNED_CHAR,
						ANIM_DT_UNSIGNED_SHORT,
						ANIM_DT_UNSIGNED_LONG,
						ANIM_DT_FIXED,
						ANIM_DT_UNSIGNED_FIXED,
						ANIM_DT_EXTENDED_96,
						ANIM_DT_DOUBLE_64,
						ANIM_DT_FLOAT_32,
						ANIM_DT_COLOR_RGB;

			  	longint ANIM_UI_NO_UI,			// UI types are only used by AE
			  			ANIM_UI_ANGLE,
						ANIM_UI_SLIDER,
						ANIM_UI_POINT,
						ANIM_UI_RECT,
						ANIM_UI_COLOR_RGB,
						ANIM_UI_COLOR_CMYK,
						ANIM_UI_COLOR_LAB;

				// These next four sets of longints are IEEE 64-bit doubles.  To store
				// them correctly, you must specify them as hexidecimal numbers.  To
				// find the correct hexidecimal number, you must convert your decimal
				// number to a double.
			  	hex longint;		// low long, valid_min (used for UI type slider - AE only)
			  	hex longint;		// high long, valid_min (64-bit double)
			  	
			  	hex longint;		// low long, valid_max (used for UI type slider - AE only)
			  	hex longint;		// high long, valid_max (64-bit double)

			  	hex longint;		// low long, ui_min (used for UI type slider - AE only)
			  	hex longint;		// high long, ui_min (64-bit double)

			  	hex longint;		// low long, ui_max (used for UI type slider - AE only)
			  	hex longint;		// high long, ui_max (64-bit double)

#ifdef PiPLVer2p3
				fill bit[27];		// ANIM_ParamFlags
				boolean dontScaleUIRange, scaleUIRange;		// ANIM_PF_SCALE_UI_RANGE (Premiere 6.0)
#else
			  	fill bit[28];		// ANIM_ParamFlags
#endif
			  	boolean dontAnimateParam, animateParam;		// ANIM_PR_DONT_ANIMATE (PrMr)
			  	boolean dontRestrictBounds, restrictBounds;	// ANIM_PF_RESTRICT_BOUNDS (AE only)
			  	boolean	spaceIsAbsolute, spaceIsRelative;	// ANIM_PF_SPACE_IS_RELATIVE (AE only)
			  	boolean resIndependent, resDependant;		// ANIM_PF_IS_RES_DEPENDENT (AE only)

			  	longint;			// size of property described in bytes (short = 2, long = 4, etc.)

			  	longint=0;			// reserved0
			  	longint=0;			// reserved1
			  	longint=0;			// reserved2
			  	longint=0;			// reserved3
			  animParamEnd:			  

			//-------------------------------------------------------------------
			// Premiere Transition Effect PiPL properties
			//-------------------------------------------------------------------
			case Pr_Effect_Info:		// Mirrors the old Premiere 'Fopt' resource
				longint = 'PrMr';		// Premiere host.
				key longint = 'pOPT';
				longint = 0;			// Index.
				longint = 16;			// Length.
#ifdef PiPLVer2p2
				longint;				// Version of this property
#else
				longint = 0;
#endif
				
				// Valid corners mask and initial corners (lsb to msb):
				// bitTop | bitRight | bitBottom | bitLeft | bitUpperRight |
				// bitLowerRight | bitLowerLeft | bitUpperLeft
				byte;					// Valid corners mask.
				byte;					// Initial corners.
#ifdef PiPLVer2p2
				boolean;										// Premiere 5.1
				boolean noExclusiveDialog, exclusiveDialog;		// Premiere 5.1
				boolean doesNotNeedCallbacksAtSetup, needsCallbacksAtSetup;
				boolean noDirectCompData, directCompData;		// Premiere 5.1
#else
				fill bit[2];
				boolean doesNotNeedCallbacksAtSetup, needsCallbacksAtSetup;
				boolean;		
#endif
				boolean wantInitialSetupCall, dontWantInitialSetupCall;	
				boolean treatAsTransition, treatAsTwoInputFilter;
				boolean noCustomDialog, hasCustomDialog;				
				boolean dontHighlightOppositeCorners, highlightOppositeCorners;

				// These should be changed to booleans:
				byte notExclusive = 0, exclusive = 1;		
				byte notReversible = 0, reversible = 1;
				byte doesNotHaveEdges = 0, haveEdges = 1;
				byte doesNotHaveStartPoint = 0, haveStartPoint = 1;
				byte doesNotHaveEndPoint = 0, haveEndPoint = 1;

#ifdef PiPLVer2p3
				longint;				// more flags - Premiere 6.0
#else
				longint = 0;			// Reserved.
#endif

			case Pr_Effect_Description:	// The text description of the transition.
				longint = 'PrMr';		// Premiere host.
				key longint = 'TEXT';	// This should be changed to 'pDES'.
				longint = 0;			// Index.
				#if DeRez
					fill long;
				#else
					longint = (descEnd[$$ArrayIndex(properties)] - descStart[$$ArrayIndex(properties)]) / 8;
				#endif
			  descStart:
				pstring;
			  descEnd:
				align long;
								
			//-------------------------------------------------------------------
			// Illustrator/SweetPea PiPL properties
			//-------------------------------------------------------------------
			case InterfaceVersion:
				longint = 'ADBE';		// SweetPea/Illustrator host.
				key longint = 'ivrs';
				longint = 0;			// Index.
				longint = 4;			// Length.
				longint;				// Version.
				
			case AdapterVersion:
				longint = 'ADBE';		// SweetPea/Illustrator host.
				key longint = 'adpt';
				longint = 0;			// Index.
				longint = 4;			// Length.
				longint;				// Version.
			
			case SP_STSP:
				longint = 'ADBE';		// SweetPea/Illustrator host.
				key longint = 'STSP';
				longint = 0;			// Index.
				longint = 4;			// Length.
				longint;
					
			case InternalName:
				longint = 'ADBE';		// SweetPea/Illustrator host.
				key longint = 'pinm';
				longint = 0;			// Index.
				#if DeRez
					fill long;
				#else
					longint = (plugInNameEnd[$$ArrayIndex(properties)] -
							   plugInNameStart[$$ArrayIndex(properties)]) / 8;
				#endif
				plugInNameStart:
					cstring;
				plugInNameEnd:
					align long;

			case Imports:
				longint = 'ADBE';		// SweetPea/Illustrator host.
				key longint = 'impt';
				longint = 0;			// Index.
				#if DeRez
					fill long;
				#else
					longint = (importsEnd[$$ArrayIndex(properties)] - 
							   importsStart[$$ArrayIndex(properties)]) / 8;
				#endif
				importsStart:
					longint = $$CountOf(ImportSuites);
					wide array ImportSuites
					{  
						isuitesStart:
							// Length (including this long):
							#if DeRez
								fill long;
							#else
								longint = ((isuitesEnd[$$ArrayIndex(properties), $$ArrayIndex(ImportSuites)] - 
											isuitesStart[$$ArrayIndex(properties), $$ArrayIndex(ImportSuites)]) / 8);
							#endif

							cstring;
							align long;
							longint;			// Suite version.
						isuitesEnd:
					};
				importsEnd:
								
			case Exports:
				longint = 'ADBE';		// SweetPea/Illustrator host.
				key longint = 'expt';
				longint = 0;			// Index.
				#if DeRez
					fill long;
				#else
					longint = (exportsEnd[$$ArrayIndex(properties)] - 
							   exportsStart[$$ArrayIndex(properties)]) / 8;
				#endif
				exportsStart:
					longint = $$CountOf(ExportSuites);
					wide array ExportSuites
					{  
						esuitesStart:
							// Length (including this long):
							#if DeRez
								fill long;
							#else
								longint = ((esuitesEnd[$$ArrayIndex(properties), $$ArrayIndex(ExportSuites)] - 
											esuitesStart[$$ArrayIndex(properties), $$ArrayIndex(ExportSuites)]) / 8);
							#endif

							cstring;
							align long;
							longint;			// Suite version.
						esuitesEnd:
					};
				exportsEnd:
			
			case Description:
				longint = 'ADBE';		// SweetPea/Illustrator host.
				key longint = 'desc';
				longint = 0;			// Index.
				#if DeRez
					fill long;
				#else
					longint = (descriptionEnd[$$ArrayIndex(properties)] -
							   descriptionStart[$$ArrayIndex(properties)]) / 8;
				#endif
				descriptionStart:
					cstring;
				descriptionEnd:
					align long;
					
			case Keywords:
				longint = 'ADBE';		// SweetPea/Illustrator host.
				key longint = 'keyw';
				longint = 0;			// Index.
				#if DeRez
					fill long;
				#else
					longint = (keywordsEnd[$$ArrayIndex(properties)] - 
							   keywordsStart[$$ArrayIndex(properties)]) / 8;
				#endif
				keywordsStart:
					longint = $$CountOf(KeywordsArray);
					wide array KeywordsArray
					{  
						keywordsArrayStart:
							// Length (including this long):
							#if DeRez
								fill long;
							#else
								longint = ((keywordsArrayEnd[$$ArrayIndex(properties), $$ArrayIndex(KeywordsArray)] - 
											keywordsArrayStart[$$ArrayIndex(properties), $$ArrayIndex(KeywordsArray)]) / 8);
							#endif

							cstring;
						keywordsArrayEnd:
					};
				keywordsEnd:
					align long;
				
			case Title:
				longint = 'ADBE';		// SweetPea/Illustrator host.
				key longint = 'titl';
				longint = 0;			// Index.
				#if DeRez
					fill long;
				#else
					longint = (titleEnd[$$ArrayIndex(properties)] -
							   titleStart[$$ArrayIndex(properties)]) / 8;
				#endif
				titleStart:
					cstring;
				titleEnd:
					align long;
					
			case Messages:
				longint = 'ADBE';		// SweetPea/Illustrator host
				key longint = 'AcpM';
				longint = 0;			// Index.
				longint = 4;			// Length.
				fill bit[28];			// Reserved.
				
				boolean startupRequired, noStartupRequired;
				boolean doesNotPurgeCache, purgeCache;
				boolean shutdownRequired, noShutdownRequired;	// Default is to give shutdown msg.
				boolean doNotAcceptProperty, acceptProperty;
				
			//-------------------------------------------------------------------
			// PhotoDeluxe PiPL properties
			//-------------------------------------------------------------------
			case ButtonIcon:
				longint = '8BIM';
				key longint = 'btni';
				longint = 0;		// pad
				#if DeRez
					fill long;
				#else
					longint = (buttonIconEnd[$$ArrayIndex(properties)] - buttonIconStart[$$ArrayIndex(properties)]) / 8; // length
				#endif
			buttonIconStart:
				longint = 0;		// version
				longint none = 0,
					 	cicn = 1;	// Macintosh icon type
				longint none = 0,
					 	ICON = 1;	// Windows icon type
				longint;			// Icon resource ID
				cstring;			// Button icon name
			buttonIconEnd:
				align long;

			//-------------------------------------------------------------------
			// PhotoDeluxe extension to Import plug-in PiPL properties
			//-------------------------------------------------------------------			
			case Class:
				longint = '8BIM';
				key longint = 'clas';
				longint = 0;	// pad
				longint = 8;	// length
				longint = 0;	// version
				longint none = 0,
						scanner = 1,
						camera = 2,
						video = 3,
						floppy = 4,
						cdrom = 5,
						internet = 6;

			case PreviewFile:
				longint = '8BIM';
				key longint = 'prvw';
				longint = 0;	// pad
				#if DeRez
					fill long;
				#else
					longint = (previewFileEnd[$$ArrayIndex(properties)] - previewFileStart[$$ArrayIndex(properties)]) / 8; // length
				#endif
			previewFileStart:
				longint = 0;	// version
				cstring;		// preview filename
			previewFileEnd:
				align long;
			};
		};
	};

#undef PIPiPLTypes

#ifndef bitNone
#define bitNone			0x00
#define bitTop			0x01
#define bitRight		0x02
#define	bitBottom		0x04
#define	bitLeft			0x08
#define	bitUpperRight	0x10
#define	bitLowerRight	0x20
#define	bitLowerLeft	0x40
#define	bitUpperLeft	0x80
#endif /* !defined(bitNone) */

#ifndef fxBitAnyPAR
#define	fxBitAnyPAR		0x10000
#define	fxBitUnityPAR	0x20000
#endif /* !defined(fxBitAnyPAR) */

//-------------------------------------------------------------------------------

#endif // qIncludeMe
#endif // __PIPL_r__
