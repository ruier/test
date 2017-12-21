/* usrCoprocPpc.c - PowerPC coprocessor discovery & initialization */

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
01i,17sep09,cww  Use fully qualified function pointers
01h,26apr05,dr   Add extern void aimFppLibInit(void)
01g,22apr05,dr   Add call to aimFppLibInit to pull in aimFppLib.o (SPR108100)
01f,09mar05,asa  Only use fppMRegs if INCLUDE_HW_FP_MREGS is defined (SPR 105886).  
                 Added altivecRegListShow and altivecMRegs and only use
                 altivecMRegs if INCLUDE_ALTIVEC_MREGS and INCLUDE_ALTIVEC_SHOW
                 are defined. removed all pointer assignments of NULL as pDesc 
                 initialized as NULL (SPR 102393)
                 updated copyright to 2005.
01e,02oct04,yvp  Added mRegs support.
01d,15sep04,dtr  Add init routines in for altivec and SPE.
01c,24aug04,rlg  fix for SPR 100461 bounded the Show function initialization
                 with if defs to keep from populating if component is not 
                 defined.
01b,01mar04,pcs  Added #ifdef INCLUDE_ALTIVEC around altivec fn. prototypes
                 and INCLUDE_HW_FP around FP fn. prototypes.
01b,01mar04,mil  Added support of SPE for PPC85XX.
01a,13feb04,yvp  written
*/

/*
DESCRIPTION

This module implements the process of discovery and initialization for PowerPC
family coprocessors (i.e. Hardware Floating-point units, Altivec etc). 

*/

/* includes */

#include <vxWorks.h>
#include <private/coprocLibP.h>
#include <taskLib.h>

/* externals */

#ifdef INCLUDE_HW_FP

IMPORT STATUS fppRegsGet (FPREG_SET *, FPREG_SET *);
IMPORT STATUS fppRegsSet (FPREG_SET *, FPREG_SET *);
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

#ifdef INCLUDE_ALTIVEC
IMPORT void altivecInit();
IMPORT STATUS altivecTaskRegsGet (int, ALTIVECREG_SET *);
IMPORT STATUS altivecTaskRegsSet (int, ALTIVECREG_SET *);
IMPORT ALTIVEC_CONTEXT * altivecCtxCreate (void);
IMPORT STATUS altivecCtxDelete (ALTIVEC_CONTEXT *);
IMPORT void altivecSave (ALTIVEC_CONTEXT *);
IMPORT void altivecRestore (ALTIVEC_CONTEXT *);
IMPORT void altivecEnable (void);
IMPORT void altivecDisable (void);
IMPORT void altivecCtxShow (ALTIVEC_CONTEXT *);
IMPORT void   altivecRegListShow (void);
IMPORT STATUS altivecMRegs (int, ALTIVEC_CONTEXT *, char *, BOOL *);
#endif /* INCLUDE_ALTIVEC */

#ifdef INCLUDE_SPE
IMPORT void speInit();
IMPORT STATUS speRegsGet (SPEREG_SET *, SPEREG_SET *);
IMPORT STATUS speRegsSet (SPEREG_SET *, SPEREG_SET *);
IMPORT SPE_CONTEXT * speCtxCreate (void);
IMPORT STATUS speCtxDelete (SPE_CONTEXT *);
IMPORT void speSave (SPE_CONTEXT *);
IMPORT void speRestore (SPE_CONTEXT *);
IMPORT void speEnable (void);
IMPORT void speDisable (void);
IMPORT void speCtxShow (SPE_CONTEXT *);
#endif /* INCLUDE_SPE */


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
#endif /* INCLUDE_HW_FP */

#ifdef INCLUDE_ALTIVEC

    altivecInit();

    if (altivecProbe () == OK)
	{
	*pMasks++ = VX_ALTIVEC_TASK;
	}
#endif /* INCLUDE_ALTIVEC */

#ifdef INCLUDE_SPE

    speInit();

    if (speProbe () == OK)
	{
	*pMasks++ = VX_SPE_TASK;
	}

#endif /* INCLUDE_SPE */
    }


/******************************************************************************
*
* coprocDescriptorsInit - Initialize all selected coprocessor descriptors 
*
* This routine initializes all selected PowerPC coprocessor descriptors.
* Project component selections such as INCLUDE_HW_FP and INCLUDE_ALTIVEC
* determine what coprocessors are configured into the system. This routine
* initializes the selected descriptors to point to their respective library 
* functions.
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
		pDesc->pDisableRtn    = (COPROC_DISABLE_RTN) fppDisable;
		pDesc->pCtxSaveRtn    = (COPROC_CTX_SAVE_RTN) fppSave;
		pDesc->pCtxRestoreRtn = (COPROC_CTX_RESTORE_RTN) fppRestore;
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

#ifdef INCLUDE_ALTIVEC
	    case VX_ALTIVEC_TASK:
		pDesc->ctxSize        = sizeof (ALTIVEC_CONTEXT);
		pDesc->ctxAlign       = 16;
		pDesc->pCtxCreateRtn  = (COPROC_CTX_CREATE_RTN)altivecCtxCreate;
		pDesc->pCtxDeleteRtn  = (COPROC_CTX_DELETE_RTN)altivecCtxDelete;
		pDesc->pEnableRtn     = (COPROC_ENABLE_RTN) altivecEnable;
		pDesc->pDisableRtn    = (COPROC_DISABLE_RTN) altivecDisable;
		pDesc->pCtxSaveRtn    = (COPROC_CTX_SAVE_RTN) altivecSave;
		pDesc->pCtxRestoreRtn = (COPROC_CTX_RESTORE_RTN) altivecRestore;
#ifdef INCLUDE_ALTIVEC_SHOW
		pDesc->pShowRtn       = (COPROC_CTX_SHOW_RTN) altivecCtxShow;
		pDesc->pRegListShowRtn =
				(COPROC_REG_LIST_SHOW_RTN) altivecRegListShow;
#ifdef INCLUDE_ALTIVEC_MREGS
		pDesc->pMRegsRtn      = (COPROC_MREGS_RTN) altivecMRegs;
#endif /* INCLUDE_ALTIVEC_MREGS */
#endif /* INCLUDE_ALTIVEC_SHOW */
		break;
#endif /* INCLUDE_ALTIVEC */

#ifdef INCLUDE_SPE
            case VX_SPE_TASK:
                pDesc->ctxSize        = sizeof (SPE_CONTEXT);
		pDesc->ctxAlign       = 16;
                pDesc->pCtxCreateRtn  = (COPROC_CTX_CREATE_RTN) speCtxCreate;
                pDesc->pCtxDeleteRtn  = (COPROC_CTX_DELETE_RTN) speCtxDelete;
                pDesc->pEnableRtn     = (COPROC_ENABLE_RTN) speEnable;
                pDesc->pDisableRtn    = (COPROC_DISABLE_RTN) speDisable;
                pDesc->pCtxSaveRtn    = (COPROC_CTX_SAVE_RTN) speSave;
                pDesc->pCtxRestoreRtn = (COPROC_CTX_RESTORE_RTN) speRestore;
#ifdef INCLUDE_SPE_SHOW
                pDesc->pShowRtn       = (COPROC_CTX_SHOW_RTN) speCtxShow;
#endif /* INCLUDE_SPE_SHOW */
                break;
#endif /* INCLUDE_SPE */

	    default:
		break;
	    }
	}
    }
