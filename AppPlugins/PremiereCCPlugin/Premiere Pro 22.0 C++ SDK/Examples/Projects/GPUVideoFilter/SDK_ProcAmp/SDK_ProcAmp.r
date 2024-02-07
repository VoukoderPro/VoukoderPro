/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2012 Adobe Systems Incorporated                       */
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

#include "AEConfig.h"
#include "AE_EffectVers.h"
#include "AE_General.r"


resource 'PiPL' (16000) {
	{	/* array properties: 11 elements */
		/* [1] */
		Kind {
			AEEffect
		},
		/* [2] */
		Name {
			"SDK ProcAmp"
		},
		/* [3] */
		Category {
			"SDK"
		},

		/* [4] */
#ifdef AE_OS_WIN
		CodeWin64X86 {"EffectMain"},
#else
		CodeMacIntel64 {"EffectMain"},
#endif

		/* [5] */
		AE_PiPL_Version {
			2,
			0
		},
		/* [6] */
		AE_Effect_Spec_Version {
			PF_PLUG_IN_VERSION,
			PF_PLUG_IN_SUBVERS
		},
		/* [7] */
		AE_Effect_Version {
			524288 
		},
		/* [8] */
		AE_Effect_Info_Flags {
			0
		},
		/* [9] */
		AE_Effect_Global_OutFlags {
			0x40
		},
		AE_Effect_Global_OutFlags_2 {
			0x100
		},
		/* [10] */
		AE_Effect_Match_Name {
			"SDK ProcAmp"
		},
		/* [11] */
		AE_Reserved_Info {
			8
		}
	}
};
