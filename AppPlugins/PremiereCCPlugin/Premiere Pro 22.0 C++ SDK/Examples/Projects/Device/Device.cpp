/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 1992-2008 Adobe Systems Incorporated                  */
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
	10/20/93	Original version.			
	9/12/94		Updated for Adobe Premiere 4.0.							
	11/8/95		Updated for Adobe Premiere 4.2 and CW7 - Added dsQuiet,		- bsa
					cmdJog, and cmdEject, fDrvrQuiet, fHasJogMode,
					fHasEject.
	2/21/97		Updated for CW11.											- bsa
	6/3/97		Updated for CW12.											- bsa
	4/11/98		Updated for Adobe Premiere 5.0 and made cross platform.		- bsa
	12/13/99	Updated for Adobe Premiere 6.0								- bbb
	5/23/00		Added mention of changing playback speed for 				- bbb
					different modes.
	8/6/02		Updated to include cmdStatusSync and						- zal
					cmdGetDeviceDisplayName, and return real status
					in cmdStatus
	2/26/03		Moved to .NET												- zal
	11/7/03		Sample Error Suite usage, updated cmdGetDeviceDisplayName,	- zal
					sample cmdShuttle modes
	8/19/04		Simulate timecode reporting									- zal
*/

#include "Device.h"
	
	
PREMPLUGENTRY DllExport 
xDevice (short selector, DeviceHand theData)
{
	csSDK_int32		resultS	 	= dmUnsupported;
	int				returnValue	= 0;
	LocalHand		lDataH	 	= NULL;
	
	#ifdef PRMAC_ENV
	/*
	CFStringRef						deviceControllerName	= CFSTR (DEVICE_CONTROLLER_NAME),
									setupDescription		= CFSTR (SETUP_DESCRIPTION),
									exportDescription		= CFSTR (EXPORT_DESCRIPTION),
									yes						= CFSTR ("Yes"),
									no						= CFSTR ("No");
	DialogRef						alert;
	DialogItemIndex					outItemHit;									
	AlertStdCFStringAlertParamRec	inAlertParamSetup		= {	kStdCFStringAlertVersionOne,
																kPrTrue,
																kPrFalse,
																(const CFStringRef)kAlertDefaultOKText,
																NULL,
																NULL,
																kAlertStdAlertOKButton,
																NULL,
																kWindowDefaultPosition,
																NULL};
	AlertStdCFStringAlertParamRec	inAlertParamExport		= {	kStdCFStringAlertVersionOne,
																kPrTrue,
																kPrFalse,
																yes,
																no,
																NULL,
																kAlertStdAlertOKButton,
																kAlertStdAlertCancelButton,
																kWindowDefaultPosition,
																NULL};
	*/
	#endif

	switch (selector) 
	{
		case dsInit:
			resultS 	= dmNoError;

			// Allocate any local data here and return an error if could not complete.
			// Open any necessary drivers and connect to hardware.
			// This selector is only sent when there is no deviceData stored in the
			// preferences file, likely the first time the device controller is called
			// after a clean installation of Premiere.
			
			// Allocate our local data handle and stuff it into theData. This will persist
			// across calls.

			(*theData)->deviceData = (*theData)->piSuites->memFuncs->newHandleClear(sizeof(LocalRec));

			if ((*theData)->deviceData == 0)
			{
				resultS = dmGeneralError;
			}
			else
			{
				lDataH = reinterpret_cast<LocalHand>((*theData)->deviceData);
				(*lDataH)->structVersion = STRUCT_VERSION;

				// Attempt the default connection to the device
				resultS = CheckConnection(lDataH);
			}
			
			break;
			
		case dsRestart:
			
			// This message is the same as dsInit, except the data handle has already been
			// filled in with data saved in the preferences file. You
			// should verify the device connection, and open any needed drivers.
			// This selector is sent when there deviceData stored in the preferences file,
			// likely any time after the first time the device controller is called
			// after a clean installation of Premiere.
			
			lDataH	=	reinterpret_cast<LocalHand>((*theData)->deviceData);

			// Check if the data saved in the prefs is old.  If so, reallocate a new block of memory
			if (lDataH)
			{
				if ((*lDataH)->structVersion != STRUCT_VERSION)
				{
					(*theData)->piSuites->memFuncs->disposeHandle(reinterpret_cast<PrMemoryHandle>(lDataH));
					
					(*theData)->deviceData = (*theData)->piSuites->memFuncs->newHandleClear(sizeof(LocalRec));
					lDataH = reinterpret_cast<LocalHand>((*theData)->deviceData);
					(*lDataH)->structVersion = STRUCT_VERSION;
				}
			}

			resultS	=	CheckConnection(lDataH);
			
			if (resultS)
			{
				resultS = DisplayEvent(PrSDKErrorSuite2::kEventTypeInformational, theData);
			}
			break;
			
		case dsSetup:
			resultS 	= dmNoError;

			// This message is sent when the user clicks the "Options..." button in the
			// Device Control pane of the preferences dialog, or in the Device Control
			// area of the settings pane in the Capture Window. Return a non-zero value
			// if the new settings fail (for instance, if they specify a different device
			// and the CheckConnection fails for that new device, return a non-zero value). 
			            
			#ifdef PRWIN_ENV
			// Use GetForegroundWindow(), rather than getMainWnd()
			// so that MessageBox will be centered over Capture panel, or Preferences window
			// rather than application main frame
			returnValue = MessageBox(	(*theData)->parentWindow,
										SETUP_DESCRIPTION,
										DEVICE_CONTROLLER_NAME,
										MB_OK);
			#else
/*			[TODO] Will need to use Cocoa with NSAlertPanel
			returnValue = CreateStandardAlert (	kAlertNoteAlert,
												deviceControllerName,
												setupDescription,
												&inAlertParamSetup,
												&alert);
			returnValue = RunStandardAlert (alert, NULL, &outItemHit);
*/			#endif

			break;
								
		case dsQuiet:
			resultS 	= dmNoError;
			
			// Disconnect from the device, but don't dispose of allocated structures yet.
			// You'll get a dsCleanup for that.
			
			break;
			
		case dsCleanup:
			resultS 	= dmNoError;

			lDataH	=	reinterpret_cast<LocalHand>((*theData)->deviceData);
			
			// Here is where we dispose of anything we allocated and close drivers that we opened
			if (lDataH)
			{
				(*theData)->piSuites->memFuncs->disposeHandle(reinterpret_cast<PrMemoryHandle>(lDataH));
				(*theData)->deviceData = 0;
			}

			break;
			
		case dsExecute:
			resultS 	= dmNoError;
			
			// This message is where we perform actual device control and device inquiry
			// tasks. Cache up lDataH and then act based on the command.
			
			lDataH	=	reinterpret_cast<LocalHand>((*theData)->deviceData);

			switch ((*theData)->command) 
			{
				case cmdGetFeatures:
			
					// Here's where we tell Premiere about the features this device has.
					// We can OR as many of these together as apply.
					
					(*theData)->features =	fBasic			| 
											fStepFwd		|
											fStepBack		|
											f1_5			|
											fReversePlay	|
											fRecord			|
											fRecordImmediate |
											fCanShuttle		|	
											fPositionInfo	|
											fDrvrQuiet		|
											fHasJogMode		|
											fCanEject		|
											fExportDialog	|
											fCanInsertEdit  |
											fCanLocate		|
											fGoto			|
											fHasOptions		|
											fCanInsertNoUI;
					break;
					
				case cmdStatus:
					
					// Premiere wants to know the current status of the device we're con-
					// nected to. Often it is not possible to instantly know the status of
					// a device. Premiere does cmdStatus calls all the time, and it is
					// important for the device controller not to waste any time here. The way some
					// of the built-in Premiere modules handle this is by keeping a "last record-
					// ed mode/location" value in their local data, and they use asynch-
					// ronous calls to their drivers. Each time into cmdStatus, if the
					// last async status call you made to your driver is ready, you copy
					// the mode and location into your "last recorded" values. If there's
					// not a status call queued up, go ahead and queue one up to your
					// driver. Finally, at the end of each cmdStatus call, just return
					// your last-recorded mode and location.
					
					(*theData)->timeformat 	= 0;		// 0: non-drop, 1: drop-frame
					(*theData)->timerate 	= 60;		// 24, 25, 30, and 60 are valid

					(*theData)->mode		= (*lDataH)->mode;	// Current mode

					switch ((*lDataH)->mode)
					{
					case modeRewind:
						(*lDataH)->currentTimecode = (*lDataH)->currentTimecode - 4.5;
						break;
					case modePlayRewind:
						(*lDataH)->currentTimecode = (*lDataH)->currentTimecode - 1;
						break;
					case modePlayRev:
						(*lDataH)->currentTimecode = (*lDataH)->currentTimecode - .2;
						break;
					case modePlayRev1_5:
						(*lDataH)->currentTimecode = (*lDataH)->currentTimecode - .04;
						break;
					case modePlayRev1_10:
						(*lDataH)->currentTimecode = (*lDataH)->currentTimecode - .02;
						break;
					case modePlay1_10:
						(*lDataH)->currentTimecode = (*lDataH)->currentTimecode + .02;
						break;
					case modePlay1_5:
						(*lDataH)->currentTimecode = (*lDataH)->currentTimecode + .04;
						break;
					case modePlay:
						(*lDataH)->currentTimecode = (*lDataH)->currentTimecode + .2;
						break;
					case modePlayFastFwd:
						(*lDataH)->currentTimecode = (*lDataH)->currentTimecode + 1;
						break;
					case modeFastFwd:
						(*lDataH)->currentTimecode = (*lDataH)->currentTimecode + 4.5;
						break;
					case modeRecord:
						(*lDataH)->currentTimecode = (*lDataH)->currentTimecode + .2;
						break;
					}

					// If we are handling cmdGoto, we are seeking to a specific timecode
					// So continue to return modeGoto, unless we have reached or passed the timecode we're looking for
					if ((*theData)->mode == modeGoto)
					{
						if ((*lDataH)->gotoMode == modeRewind)
						{
							(*lDataH)->currentTimecode = (*lDataH)->currentTimecode - 4.5;
						}
						else if ((*lDataH)->gotoMode == modeFastFwd)
						{
							(*lDataH)->currentTimecode = (*lDataH)->currentTimecode + 4.5;
						}

						if (((*lDataH)->gotoMode == modeFastFwd && (*lDataH)->currentTimecode >= (*lDataH)->gotoTimecode) ||
							((*lDataH)->gotoMode == modeRewind  && (*lDataH)->currentTimecode <= (*lDataH)->gotoTimecode))
						{
							(*lDataH)->currentTimecode = (*lDataH)->gotoTimecode;
							(*lDataH)->gotoMode = 0;
							(*theData)->mode = (*lDataH)->mode = modePause;
						}
					}

					(*theData)->timecode 	= (csSDK_int32)(*lDataH)->currentTimecode;	// Current device location

					break;
				
				case cmdNewMode:

					// Here Premiere wants you to change the device into a new mode. If we
					// can't do it, the next cmdStatus call will reflect the old mode.
					// Normally you send the commands to the device here, even if they are
					// asynchronous.

					// Keep track of the mode in private data, so that it can be returned
					// during cmdStatus
					
					(*lDataH)->mode = (*theData)->mode;
					
					// Make changes to playback speed and direction based on the mode.
					switch ((*theData)->mode)
					{

					case modePlay:
						break;

					// After step, device should end in pause mode
					case modeStepFwd:
						(*lDataH)->currentTimecode++;
						(*lDataH)->mode = modePause;
						break;

					case modeStepBack:
						if ((csSDK_int32)(*lDataH)->currentTimecode == 0)
						{
							// Sample error if reverse step button pressed
							// when timecode is zero
							resultS = DisplayEvent(PrSDKErrorSuite2::kEventTypeError, theData);
						}
						else
						{
							(*lDataH)->currentTimecode--;
						}
						(*lDataH)->mode = modePause;
						break;

					case modeStop:
						(*lDataH)->mode = modeStop;
						break;

					case modeRecordInsert:
						(*lDataH)->mode = modeRecordInsert;
						break;
					}
					
					break;
					
				case cmdGoto:
					
					// Premiere is telling you to move the device to the timecode spec-
					// ified in theData. Usually you start the device searching and return
					// without waiting for it to finish. Then Premiere will hammer
					// cmdStatus until the time matches the requested time. When the deck
					// is searching, your cmdStatus should return the mode as "modeGoto",
					// which will result in the text "Seeking..." showing up in the
					// timecode display. This is the sorta lame, let-Premiere-do-the-work
					// way of doing timecode location -- if you can do cmdLocate, do it.
					(*theData)->mode = (*lDataH)->mode = modeGoto;
					(*lDataH)->gotoTimecode = (*theData)->timecode;

					// Begin moving the device to (*theData)->timecode
					if ((csSDK_int32)(*lDataH)->currentTimecode > (*lDataH)->gotoTimecode)
					{
						(*lDataH)->gotoMode = modeRewind;
					}
					else if ((csSDK_int32)(*lDataH)->currentTimecode < (*lDataH)->gotoTimecode)
					{
						(*lDataH)->gotoMode = modeFastFwd;
					}
					else // ((csSDK_int32)(*lDataH)->currentTimecode == (*lDataH)->gotoTimecode)
					{
						if ((*lDataH)->mode != modeStop)
						{
							(*lDataH)->mode = modePause;
						}
					}
					
					break;
					
				case cmdLocate:
					
					// This is the synchronous way of doing timecode location. Premiere
					// wants you to move the deck to the specified timecode and return
					// with the deck in play mode rolling over timecode. You should use
					// the most accurate positioning you can here. Remember that you can
					// send yourself cmdStatus messages to find out what time your over.
					// Often you'll use some "rough seek" command to get your self within
					// a second or two before the requested code, then send yourself a
					// cmdNewMode to go into play mode. Then you sit in a tight loop
					// sending yourself cmdStatus messages until the time is equal to the
					// requested timecode, then you return as fast as possible. Don't try
					// to compensate for the latency between when you return and when
					// video actually gets captured. This is not really within your con-
					// trol, and Premiere has a calibration feature to take care of that
					// for you (by providing a preroll value).

					// Go to the timecode requested
					(*lDataH)->currentTimecode = (*theData)->timecode;
					(*theData)->TimecodeUpdateProc (static_cast<csSDK_int32>((*lDataH)->currentTimecode), (*theData)->classID);

					// Return in play mode
					(*theData)->mode = modePlay;
					
					break;

				case cmdShuttle:
					
					// Here Premiere is telling you to shuttle the deck either forward
					// or backward at a certain speed (the value is in (*theData)->mode).
					// -100 means 2x backwards playback, -50 means 1x backwards playback,
					// 0 means pause, 50 means 1x forward playback, and 100 means 2x
					// forwards playback. You may have to tweak this mapping to get the
					// right feel.
					
					if ((*theData)->mode < -80)
					{
						// Reverse play fast
						(*lDataH)->mode = modePlayRewind;
					}
					else if ((*theData)->mode < -40)
					{
						// Reverse play
						(*lDataH)->mode = modePlayRev;
					}
					else if ((*theData)->mode < -20)
					{
						// Reverse play slowly
						(*lDataH)->mode = modePlayRev1_5;
					}
					else if ((*theData)->mode < 0)
					{
						// Reverse play very slowly
						(*lDataH)->mode = modePlayRev1_10;
					}
					else if ((*theData)->mode == 0)
					{
						// Pause
						(*lDataH)->mode = modePause;
					}
					else if ((*theData)->mode <= 20)
					{
						// Play very slowly
						(*lDataH)->mode = modePlay1_10;
					}
					else if ((*theData)->mode <= 40)
					{
						// Play slowly
						(*lDataH)->mode = modePlay1_5;
					}
					else if ((*theData)->mode <= 80)
					{
						// Play
						(*lDataH)->mode = modePlay;
					}
					else // else if ((*theData)->mode <= 100)
					{
						// Play fast
						(*lDataH)->mode = modePlayFastFwd;
					}
					
					break;

				case cmdEject:
					
					// Here Premiere tells you to eject the media.
					
					break;

				case cmdInsertEdit:
					
					// For insert edit, used when the plug-in is controlling the edit.
					#ifdef PRWIN_ENV
					returnValue = MessageBox(	(*theData)->parentWindow,
												EXPORT_DESCRIPTION,
												DEVICE_CONTROLLER_NAME,
												MB_OKCANCEL);
					returnValue = (returnValue == IDOK? 1: 0);
					#else
/*					[TODO] Will need to use Cocoa with NSAlertPanel
					returnValue = CreateStandardAlert (	kAlertNoteAlert,
														deviceControllerName,
														exportDescription,
														&inAlertParamExport,
														&alert);
					returnValue = RunStandardAlert (alert, NULL, &outItemHit);
					returnValue = (outItemHit == kAlertStdAlertOKButton? 1: 0);
*/					#endif												

					if (returnValue)
					{
						(*theData)->PrintProc =(Print2TapeProcPtr)ExportToTape;

						// Save some private data we can access from our PrintProc
						(*lDataH)->numCallsToIdle = 0;
					}
					else
					{
						resultS = dmUserAborted;
					}

					break;

				case cmdGetDeviceDisplayName:

					// Fill in the displayName field
					#ifdef PRWIN_ENV
					strcpy_s (	(*theData)->displayName,
								sizeof(DEVICE_NAME),
								DEVICE_NAME);
					#else
					strcpy ((*theData)->displayName,
							DEVICE_NAME);
					#endif
					
					break;
			}
			break;
	}
	return(short)(resultS);
}

//========================================================================================
// Handle export to tape
//========================================================================================

static csSDK_int32 ExportToTape (PrMemoryHandle deviceHand, long selector)
{
	DeviceHand		theData = (DeviceHand)deviceHand;
	LocalHand		lDataH = reinterpret_cast<LocalHand>((*theData)->deviceData);

	switch(selector)
	{
		case setupWaitProc:
				// Wait for camera to stabilize
				return dmNoError;
			break;
			
		case idle:
			if ((*lDataH)->numCallsToIdle++ > 300)
			{
				return dmExportToTapeFinished;
			}
			break;
		
		case complete:
			return dmExportToTapeFinished;
			break;
	}
	
	return dmNoError;
}

static csSDK_int32 
DisplayEvent(csSDK_int32 eventType, DeviceHand theData)
{
	SPBasicSuite		*SPBasic	= NULL;
	PrSDKErrorSuite2	*ErrorSuite = NULL;
	csSDK_int32			result		= dmNoError;

	// Sample error message
	SPBasic = (*theData)->piSuites->utilFuncs->getSPBasicSuite ();
	if (SPBasic)
	{
		SPBasic->AcquireSuite (kPrSDKErrorSuite, kPrSDKErrorSuiteVersion2, (const void**)&ErrorSuite);
		if (ErrorSuite)
		{
			// Queue a sample error message using error suite
			switch(eventType)
			{
				case PrSDKErrorSuite2::kEventTypeInformational:
					ErrorSuite->SetEventString(eventType, INFO_TITLE, EVENT_DESCRIPTION);
					result = dmLastInfoSet;
					break;

				case PrSDKErrorSuite2::kEventTypeWarning:
					ErrorSuite->SetEventString(eventType, WARNING_TITLE, EVENT_DESCRIPTION);
					result = dmLastWarningSet;
					break;

				case PrSDKErrorSuite2::kEventTypeError:
					ErrorSuite->SetEventString(eventType, ERROR_TITLE, EVENT_DESCRIPTION);
					result = dmLastErrorSet;
					break;
			}

			// Clean up
			SPBasic->ReleaseSuite (kPrSDKErrorSuite, kPrSDKErrorSuiteVersion2);		
		}
	}

	return result;
}

static csSDK_int32 
CheckConnection(LocalHand lDataH)
{
	// Check the device specified in 'lDataH' and return dmNoError if connection is OK,
	// or other appropriate error code otherwise
	
	return dmNoError;								
}
