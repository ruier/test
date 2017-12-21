/* usrCoprocSh.c - Renesas SuperH coprocessor discovery & initialization */

/*
 * Copyright (c) 2004-2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01k,23mar07,h_k  added dspMRegs.
01j,26apr05,dr   Add extern void aimFppLibInit(void)
01i,22apr05,dr   Add call to aimFppLibInit to pull in aimFppLib.o (SPR108100)
01h,09mar05,asa  SPR 105886: changed fppMRegs to fppArchMRegs, and only 
                 use fppArchMRegs if INCLUDE_SHELL_HW_FP is defined 
                 moved extern function definitions from fppShLib.h
                 removed all pointer assigns equal NULL as pDesc is initialized 
                 as NULL.  updated copyright to 2005.
01g,12nov04,h_k  changed alignment for FPU coproc. (SPR #102473)
                 removed externs and included header files instead.
01f,26oct04,bpn  Removed shell references.
01e,11oct04,yvp  Enabled mRegs support.
01d,02oct04,yvp  Added mRegs support.
01c,24aug04,rlg  fix for SPR 100461 bounded the Show function initialization
                 with if defs to keep from populating if component is not 
                 defined.
01b,21apr04,h_k  added coprocessor support.
01a,13feb04,yvp  written
*/

/*
DESCRIPTION

This module implements the process of discovery and initialization for SH
family coprocessors.

*/

/* includes */

#include <vxWorks.h>
#include <private/coprocLibP.h>
#include <taskLib.h>

#ifdef INCLUDE_HW_FP

extern void     fppSave (FP_CONTEXT *);
extern void     fppRestore (FP_CONTEXT *);
extern STATUS   fppEnable (void);
extern STATUS   fppDisable (void);
extern void     fppCtxShow (FP_CONTEXT *);
extern STATUS   fppCtxDelete (FP_CONTEXT *);
extern FP_CONTEXT * fppCtxCreate (int);
extern void     fppRegListShow (void);
extern STATUS   fppArchMRegs (int, FP_CONTEXT *, char *, BOOL *);
extern void     aimFppLibInit (void);

#endif /* INCLUDE_HW_FP */

#ifdef INCLUDE_DSP
extern void     dspSave (DSP_CONTEXT *);
extern void     dspRestore (DSP_CONTEXT *);
extern STATUS   dspEnable (void);
extern STATUS   dspDisable (void);
extern void     dspCtxShow (DSP_CONTEXT *);
extern STATUS   dspCtxDelete (DSP_CONTEXT *);
extern DSP_CONTEXT * dspCtxCreate (int);
extern void     dspRegListShow (void);
extern STATUS   dspMRegs (int, DSP_CONTEXT *, char *, BOOL *);
extern void     dspArchLibInit (void);
#endif /* INCLUDE_DSP */


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
	}
#endif	/* INCLUDE_HW_FP */

#ifdef INCLUDE_DSP
    /* Initialize arch specific DSP Lib before dspProbe call */

    dspArchInit ();			/* Initialize DSP coprocessor */

    if (dspProbe () == OK)
	{
	*pMasks++ = VX_DSP_TASK;
	}
#endif	/* INCLUDE_DSP */
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
		fppArchInit ();		/* Initialize FP coprocessor */

		pDesc->ctxSize        = sizeof (FP_CONTEXT);
		pDesc->ctxAlign       = _FP_ALIGN_SIZE;
		pDesc->pCtxCreateRtn  = (FUNCPTR) fppCtxCreate;
		pDesc->pCtxDeleteRtn  = (FUNCPTR) fppCtxDelete;
		pDesc->pEnableRtn     = (FUNCPTR) fppEnable;
		pDesc->pDisableRtn    = (FUNCPTR) fppDisable;
		pDesc->pCtxSaveRtn    = (FUNCPTR) fppSave;
		pDesc->pCtxRestoreRtn = (FUNCPTR) fppRestore;
#ifdef INCLUDE_HW_FP_SHOW
		pDesc->pShowRtn       = (FUNCPTR) fppCtxShow;
		pDesc->pRegListShowRtn = (FUNCPTR) fppRegListShow;
#ifdef INCLUDE_HW_FP_MREGS

                /* fppRegsArchModify is the SH specific fppMRegs routine */

		pDesc->pMRegsRtn      = (FUNCPTR) fppArchMRegs;
#endif /* INCLUDE_HW_FP_MREGS */
#endif /* INCLUDE_HW_FP_SHOW */
		break;
#endif	/* INCLUDE_HW_FP */

#ifdef INCLUDE_DSP
	    case VX_DSP_TASK:
		pDesc->ctxSize        = sizeof (DSP_CONTEXT);
		pDesc->ctxAlign       = _ALLOC_ALIGN_SIZE;
		pDesc->pCtxCreateRtn  = (FUNCPTR) dspCtxCreate;
		pDesc->pCtxDeleteRtn  = (FUNCPTR) dspCtxDelete;
		pDesc->pEnableRtn     = (FUNCPTR) dspEnable;
		pDesc->pDisableRtn    = (FUNCPTR) dspDisable;
		pDesc->pCtxSaveRtn    = (FUNCPTR) dspSave;
		pDesc->pCtxRestoreRtn = (FUNCPTR) dspRestore;
#ifdef INCLUDE_DSP_SHOW
		pDesc->pShowRtn       = (FUNCPTR) dspCtxShow;
		pDesc->pRegListShowRtn = (FUNCPTR) dspRegListShow;

#ifdef INCLUDE_DSP_MREGS
		pDesc->pMRegsRtn      = (FUNCPTR) dspMRegs;
#endif /* INCLUDE_DSP_MREGS */
#endif /* INCLUDE_DSP_SHOW */
		break;
#endif	/* INCLUDE_DSP */

            default:
                break;
            }
	}
    }

