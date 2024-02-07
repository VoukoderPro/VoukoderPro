/************************************************************************************/
/* The MIT License (MIT)															*/
/* Copyright (c) 2016 Bartlomiej Walczak											*/
/*																					*/
/* Permission is hereby granted, free of charge, to any person obtaining a copy		*/
/* of this software and associated documentation files (the "Software"), to deal	*/
/* in the Software without restriction, including without limitation the rights		*/
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell		*/
/* copies of the Software, and to permit persons to whom the Software is			*/
/* furnished to do so, subject to the following conditions:							*/
/*																					*/
/* The above copyright notice and this permission notice shall be included			*/
/* in all copies or substantial portions of the Software.							*/
/*																					*/
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS			*/
/* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,		*/
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE		*/
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER			*/
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING			*/
/* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS		*/
/* IN THE SOFTWARE.																	*/
/*																					*/
/************************************************************************************/

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
			"Quick Vignette"
		},
		/* [3] */
		Category {
			"SDK"
		},

		/* [4] */
#ifdef AE_OS_WIN
		CodeWin64X86 {"EntryPointFunc"},
#else
		CodeMacIntel64 {"EntryPointFunc"},
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
			524289 
		},
		/* [8] */
		AE_Effect_Info_Flags {
			0
		},
		/* [9] */
		AE_Effect_Global_OutFlags {
			0x00000440 
		},
		AE_Effect_Global_OutFlags_2 {
			0x00001440
		},
		/* [10] */
		AE_Effect_Match_Name {
			"FXPHD Quick Vignette"
		},
		/* [11] */
		AE_Reserved_Info {
			8
		}
	}
};
