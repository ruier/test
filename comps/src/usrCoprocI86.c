/* usrCoprocI86.c - I80X86 coprocessor discovery & initialization */

/*
 * Copyright (c) 2004-2006, 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */


/*
modification history
--------------------
01l,22mar11,ggm  WIND00262870 - Added XSAVE/XRSTOR to the coprocDescriptorsInit routine.
01k,17sep09,cww  Use fully qualified function pointers
01j,08aug06,jb   Fix for Defect WIND00057651 - fppTaskRegsGet uses wrong type
01i,26apr05,dr   Add extern void aimFppLibInit(void) 
01h,22apr05,dr   Add call to aimFppLibInit to pull in aimFppLib.o (SPR108100)
01g,09mar05,asa  Only use fppMRegs if INCLUDE_HW_FP_MREGS is defined (SPR 105886)
                 Removed all pointer assigns equal NULL as pDesc is initialized 
                 as NULL.  updated copyright to 2005.
01f,11oct04,yvp  Enabled mRegs support.
01e,02oct04,yvp  Added mRegs support.
01d,24aug04,rlg  fix for SPR 100461 bounded the Show function initialization
                 with if defs to keep from populating if component is not
                 defined.
01c,09jun04,zmm  Add missing math pointers initialization. Call
                 mathHardFuncInit().
01b,13apr04,zmm  Add coprocessor support for Pentium.
01a,13feb04,yvp  written
*/

/*
DESCRIPTION

This module implements the process of discovery and initialization for I80X86
family coprocessors.

*/

/* includes */

#include <vxWorks.h>
#include <private/coprocLibP.h>
#include <taskLib.h>
#include "hwif/cpu/arch/i86/vxCpuIdLib.h"   /* For VX_CPUID_XSAVE */

/* externals */

#ifdef INCLUDE_HW_FP

IMPORT CPUID    sysCpuId;

IMPORT void   fppSave (FP_CONTEXT *);
IMPORT void   fppRestore (FP_CONTEXT *);
IMPORT void   fppEnable (void);
IMPORT void   fppDisable (void);
IMPORT void   fppCtxShow (FP_CONTEXT *);
IMPORT void   fppXctxToRegs(FP_CONTEXT *, FPREG_SET *);
IMPORT void   fppCtxToRegs(FP_CONTEXT *, FPREG_SET *);
IMPORT void   fppXregsToCtx(FPREG_SET *, FP_CONTEXT *);
IMPORT void   fppRegsToCtx(FPREG_SET *, FP_CONTEXT *);
IMPORT void   fppExtXctxToRegs (FP_CONTEXT *pFpContext, FPREG_SET *pFpRegSet);
IMPORT void   fppExtXregsToCtx (FPREG_SET *pFpRegSet, FP_CONTEXT *pFpContext);
IMPORT void   mathHardFuncInit (void); 
IMPORT STATUS fppCtxDelete (FP_CONTEXT *);
IMPORT FP_CONTEXT * fppCtxCreate (void);
IMPORT void   fppRegListShow (void);
IMPORT STATUS fppMRegs (TASK_ID, FP_CONTEXT *, char *, BOOL *);
IMPORT void   aimFppLibInit (void);
#endif /* INCLUDE_HW_FP */



/******************************************************************************
*
* coprocsDiscover - discover the available and configured coprocessors 
*
* This routine detects which coprocessors are present and configured in
* the system. For each one found, a bitmask is placed into the pMasks array.
* coprocLibInit() will then allocate descriptor structures for each 
* coprocessor used in the system.
*
* RETURNS: N/A.
*
* ERRNO: N/A.
*
* SEE ALSO: 
* .pG "Basic OS"
*
* VARARGS5
*/

void coprocsDiscover
    (
    UINT32 * pMasks
    )
    {
#ifdef INCLUDE_HW_FP
    if (fppProbe () == OK)
	{
	*pMasks++ = VX_FP_TASK;

        aimFppLibInit ();     /* call to dummy routine to pull in aimFppLib.o */

	mathHardFuncInit ();
	}
#endif /* INCLUDE_HW_FP */

    /* Add any additional coprocessor here */

    }


/******************************************************************************
*
* coprocDescriptorsInit - Initialize all selected coprocessor descriptors 
*
* This routine initializes all selected PowerPC coprocessor descriptors.
* Project component selections such as INCLUDE_HW_FP etc determine what
* coprocessors are configured into the system. This routine initializes the
* selected descriptors to point to their respective library functions.
*
* RETURNS: N/A.
*
* ERRNO: N/A.
*
* SEE ALSO: 
* .pG "Basic OS"
*
* VARARGS5
*/

void coprocDescriptorsInit
    (
    COPROC_DESC * pDescList
    )
    {
    COPROC_DESC * pDesc;

    for (pDesc = pDescList; pDesc != NULL; pDesc = pDesc->next)
        {
        switch (pDesc->mask)
            {

#ifdef INCLUDE_HW_FP
	    case VX_FP_TASK:
                fppArchInit ();         /* Initialize FP coprocessor */

		pDesc->ctxSize        = sizeof (FP_CONTEXT);
		pDesc->ctxAlign       = _CACHE_ALIGN_SIZE;
		pDesc->pCtxCreateRtn  = (COPROC_CTX_CREATE_RTN) fppCtxCreate;
		pDesc->pCtxDeleteRtn  = (COPROC_CTX_DELETE_RTN) fppCtxDelete;
		pDesc->pEnableRtn     = (COPROC_ENABLE_RTN) fppEnable;
		pDesc->pDisableRtn    = (COPROC_DISABLE_RTN) fppDisable;
		pDesc->pCtxSaveRtn    = (COPROC_CTX_SAVE_RTN) _func_fppSaveRtn;
		pDesc->pCtxRestoreRtn = (COPROC_CTX_RESTORE_RTN) _func_fppRestoreRtn;
		if((sysCpuId.featuresEcx & VX_CPUID_XSAVE) && 
		   (sysCpuId.featuresEcx & VX_CPUID_AVX))
		    {
                    pDesc->pCtxGetRtn = (COPROC_CTX_GET_RTN) fppExtXctxToRegs;
                    pDesc->pCtxSetRtn = (COPROC_CTX_SET_RTN) fppExtXregsToCtx;
		    }
                else if (sysCpuId.featuresEdx & CPUID_FXSR)
                    {
                    pDesc->pCtxGetRtn = (COPROC_CTX_GET_RTN) fppXctxToRegs;
                    pDesc->pCtxSetRtn = (COPROC_CTX_SET_RTN) fppXregsToCtx;
                    }
                else
                    {
                    pDesc->pCtxGetRtn = (COPROC_CTX_GET_RTN) fppCtxToRegs;
                    pDesc->pCtxSetRtn = (COPROC_CTX_SET_RTN) fppRegsToCtx;
                    }
#ifdef INCLUDE_HW_FP_SHOW
		pDesc->pShowRtn       = (COPROC_CTX_SHOW_RTN) fppCtxShow;
		pDesc->pRegListShowRtn = 
				(COPROC_REG_LIST_SHOW_RTN) fppRegListShow;
#ifdef INCLUDE_HW_FP_MREGS
		pDesc->pMRegsRtn      = (COPROC_MREGS_RTN) fppMRegs;
#endif /* INCLUDE_HW_FP_MREGS */
#endif /* INCLUDE_HW_FP_SHOW */
		break;
#endif /* INCLUDE_HW_FP */

            default:
                break;
            }
	}
    }

