/* usrCoprocColdfire.c - Coldfire coprocessor discovery & initialization */

/*
 * Copyright (c) 2005-2007, 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */


/*
modification history
01e,17sep09,cww  Use fully qualified function pointers
01d,10jul07,rec  include fppColdfireLib.h
01c,17jul06,rec  add MAC show prototoypes
01b,09nov05,scm  correct improper function reference...
01a,23may05,rec  written
*/

/*
DESCRIPTION

This module implements the process of discovery and initialization for Coldfire
family coprocessors.

*/

/* includes */

#include <vxWorks.h>
#include <private/coprocLibP.h>
#include <taskLib.h>
#include <arch/coldfire/macColdfireLib.h>

/* externals */

#ifdef INCLUDE_HW_FP
#include <arch/coldfire/fppColdfireLib.h>

IMPORT void   fppSave (FP_CONTEXT *);
IMPORT void   fppRestore (FP_CONTEXT *);

IMPORT void   fppEnable (void);
IMPORT void   fppDisable (void);

IMPORT void   fppCtxShow (FP_CONTEXT *);
IMPORT STATUS fppCtxDelete (FP_CONTEXT *);
IMPORT FP_CONTEXT * fppCtxCreate (int tid);

IMPORT void   fppRegListShow (void);
IMPORT STATUS fppMRegs (int, FP_CONTEXT *, char *, BOOL *);

IMPORT void   aimFppLibInit (void);
#endif /* INCLUDE_HW_FP */

#ifdef INCLUDE_MAC
IMPORT void macSave (MAC_CONTEXT *  pMacContext);
IMPORT void macRestore(MAC_CONTEXT *  pMacContext);
#ifdef INCLUDE_MAC_SHOW
IMPORT void macCtxShow (MAC_CONTEXT *  pMacContext);
IMPORT void macRegListShow (MAC_CONTEXT *  pMacContext);
#endif /* INCLUDE_MAC_SHOW */

#endif /* INCLUDE_MAC */


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
#ifdef INCLUDE_MAC
    if (macProbe () == OK)
        {
        *pMasks++ = VX_MAC_TASK;

        }

#endif /* INCLUDE_MAC */

    /* Add any additional coprocessor here */

    }


/******************************************************************************
*
* coprocDescriptorsInit - Initialize all selected coprocessor descriptors 
*
* This routine initializes all selected Coldfire coprocessor descriptors.
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
#ifdef INCLUDE_MAC
            case VX_MAC_TASK:
                macArchInit ();         /* Initialize MAC coprocessor */

                pDesc->ctxSize        = sizeof (FP_CONTEXT);
                pDesc->ctxAlign       = _CACHE_ALIGN_SIZE;
                pDesc->pCtxCreateRtn  = (COPROC_CTX_CREATE_RTN) macCtxCreate;
                pDesc->pCtxDeleteRtn  = (COPROC_CTX_DELETE_RTN) macCtxDelete;
                pDesc->pCtxSaveRtn    = (COPROC_CTX_SAVE_RTN) macSave;
                pDesc->pCtxRestoreRtn = (COPROC_CTX_RESTORE_RTN) macRestore;
#ifdef INCLUDE_MAC_SHOW
                pDesc->pShowRtn       = (COPROC_CTX_SHOW_RTN) macCtxShow;
                pDesc->pRegListShowRtn = 
				(COPROC_REG_LIST_SHOW_RTN) macRegListShow;
#endif /* INCLUDE_MAC_SHOW */
                break;
#endif /* INCLUDE_MAC */

#ifdef INCLUDE_HW_FP
            case VX_FP_TASK:
                fppArchInit ();         /* Initialize FP coprocessor */

                pDesc->ctxSize        = sizeof (FP_CONTEXT);
                pDesc->ctxAlign       = _CACHE_ALIGN_SIZE;
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

            default:
                break;
            }
        }
    }

