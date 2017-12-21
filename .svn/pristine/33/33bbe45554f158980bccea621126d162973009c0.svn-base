/* usrCoprocMips.c - MIPS coprocessor discovery & initialization */

/*
 * Copyright (c) 2004-2005, 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */


/*
modification history
--------------------
01m,17sep09,cww  Use fully qualified function pointers
01l,27jul05,pes  Add support for DSP coprocessor.
01k,06jun05,pes  Correction of fix for SPR #109840
01j,02jun05,pes  Add initialization of FPE exception vector. (SPR #109840)
01i,26apr05,dr   Add extern void aimFppLibInit(void)
01h,22apr05,dr   Add call to aimFppLibInit to pull in aimFppLib.o (SPR108100)
01g,09mar05,asa  Only use fppMRegs if INCLUDE_HW_FP_MREGS is defined (SPR 105886)
                 Removed all pointer assigns equal NULL as pDesc is initialized 
                 as NULL.  updated copyright to 2005
01f,11oct04,yvp  Enabled mRegs support.
01e,02oct04,yvp  Added mRegs support.
01d,24aug04,rlg  fix for SPR 100461 bounded the Show function initialization
                 with if defs to keep from populating if component is not 
                 defined.
01c,31mar04,pes  Fix MIPS32 build problem.
01b,19mar04,pes  Update coprocessor abstraction support.
01a,13feb04,yvp  written
*/

/*
DESCRIPTION

This module implements the process of discovery and initialization for MIPS
family coprocessors.

*/

/* includes */

#include <vxWorks.h>
#include <private/coprocLibP.h>
#include <taskLib.h>

#ifdef INCLUDE_MIPS_DSP
#include <arch/mips/dspMipsLib.h>
#endif /* INCLUDE_MIPS_DSP */

/* externals */

#ifdef INCLUDE_HW_FP

IMPORT void   fppSave (FP_CONTEXT *);
IMPORT void   fppRestore (FP_CONTEXT *);
IMPORT void   fppEnable (void);
IMPORT void   fppDisable (void);
IMPORT void   fppCtxShow (FP_CONTEXT *);
IMPORT STATUS fppCtxDelete (FP_CONTEXT *);
IMPORT FP_CONTEXT * fppCtxCreate (void);
IMPORT void   fppRegListShow (void);
IMPORT STATUS fppMRegs (int, FP_CONTEXT *, char *, BOOL *);
IMPORT void   aimFppLibInit (void);
#endif /* INCLUDE_HW_FP */

#ifdef INCLUDE_MIPS_DSP
IMPORT STATUS	dspProbe (void);
IMPORT void	dspSave (DSP_CONTEXT *);
IMPORT void	dspRestore (DSP_CONTEXT *);
IMPORT STATUS	dspMRegs (int, DSP_CONTEXT *, char *, BOOL *);
#ifdef INCLUDE_MIPS_DSP_SHOW
IMPORT void	dspCtxShow (DSP_CONTEXT *);
IMPORT void	dspRegListShow (void);
#endif /* INCLUDE_MIPS_DSP_SHOW */
#endif /* INCLUDE_MIPS_DSP */


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
    *pMasks++ = VX_FP_TASK;
    aimFppLibInit ();     /* call to dummy routine to pull in aimFppLib.o */
    fppArchUnitInit();
#endif /* INCLUDE_HW_FP */
#ifdef INCLUDE_MIPS_DSP
    *pMasks++ = VX_DSP_TASK;
    dspArchUnitInit();
#endif /* INCLUDE_MIPS_DSP */
    }


/******************************************************************************
*
* coprocDescriptorsInit - Initialize all selected coprocessor descriptors 
*
* This routine initializes all selected MIPS coprocessor descriptors.
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
		pDesc->ctxSize        = sizeof (FP_CONTEXT);
		pDesc->ctxAlign       = _ALLOC_ALIGN_SIZE;
		pDesc->pCtxCreateRtn  = (COPROC_CTX_CREATE_RTN) fppCtxCreate;
		pDesc->pCtxDeleteRtn  = (COPROC_CTX_DELETE_RTN) fppCtxDelete;
		pDesc->pEnableRtn     = (COPROC_ENABLE_RTN) fppEnable;
		pDesc->pDisableRtn    = (COPROC_DISABLE_RTN)fppDisable;
		if (fppProbe () == OK)
		    {
		    pDesc->pCtxSaveRtn    = (COPROC_CTX_SAVE_RTN) fppSave;
		    pDesc->pCtxRestoreRtn = (COPROC_CTX_RESTORE_RTN) fppRestore;
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

#ifdef INCLUDE_MIPS_DSP
	    case VX_DSP_TASK:
		if (dspProbe() == OK)
		    {
		    pDesc->ctxSize	 = sizeof (DSP_CONTEXT);
		    pDesc->ctxAlign	 = _ALLOC_ALIGN_SIZE;
		    pDesc->pCtxCreateRtn = (COPROC_CTX_CREATE_RTN) dspCtxCreate;
		    pDesc->pCtxDeleteRtn = (COPROC_CTX_DELETE_RTN) dspCtxDelete;
		    pDesc->pCtxSaveRtn	 = (COPROC_CTX_SAVE_RTN) dspSave;
		    pDesc->pCtxRestoreRtn = (COPROC_CTX_RESTORE_RTN) dspRestore;
		    pDesc->pEnableRtn	 = (COPROC_ENABLE_RTN) dspEnable;
		    pDesc->pDisableRtn	 = (COPROC_DISABLE_RTN) dspDisable;
#ifdef INCLUDE_MIPS_DSP_SHOW
		    pDesc->pShowRtn	    = (COPROC_CTX_SHOW_RTN) dspCtxShow;
		    pDesc->pRegListShowRtn  = 
				(COPROC_REG_LIST_SHOW_RTN) dspRegListShow;
#ifdef INCLUDE_MIPS_DSP_MREGS
		    pDesc->pMRegsRtn	 = (COPROC_MREGS_RTN) dspMRegs;
#endif /* INCLUDE_MIPS_DSP_MREGS */
#endif /* INCLUDE_MIPS_DSP_SHOW */
		    }
		break;
#endif /* INCLUDE_MIPS_DSP */

            default:
                break;
            }
	}
    }

