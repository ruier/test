/* usrCoprocArm.c - ARM coprocessor discovery & initialization */

/*
 * Copyright (c) 2004-2006, 2009-2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute or otherwise make use of this software
 * may be licensed only pursuant to the terms of an applicable Wind River
 * license agreement.
 */

/*
modification history
--------------------
01e,27sep10,jdw  Remove obsolete INCLUDE_VFP_SHOW
01d,17sep09,cww  Use fully qualified function pointers
01c,11jan06,scm  Add VFP support
01b,26apr05,dr   SPR 108845, SPR 108100. Call aimFppLibInit().
01a,13feb04,yvp  written
*/

/*
DESCRIPTION

This module implements the process of discovery and initialization for ARM
family coprocessors.

*/

/* includes */

#include <vxWorks.h>
#include <private/coprocLibP.h>
#include <taskLib.h>

/* externals */

#ifdef INCLUDE_HW_FP
IMPORT void   aimFppLibInit (void);
#endif /* INCLUDE_HW_FP */

#ifdef INCLUDE_VFP
#include <arch/arm/vfpArmLib.h>
#endif /* INCLUDE_VFP */

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
    aimFppLibInit ();     /* call to dummy routine to pull in aimFppLib.o */
#endif /* INCLUDE_HW_FP */

#ifdef INCLUDE_VFP
    if (vfpProbe () == OK)      /* can we talk to VFP ? */
      {
       *pMasks++ = VX_VFP_TASK;
       vfpArchInit();           /* initialize VFP support */
      }
#endif /* INCLUDE_VFP */
    }


/******************************************************************************
*
* coprocDescriptorsInit - Initialize all selected coprocessor descriptors
*
* This routine initializes all selected ARM/XSCALE coprocessor descriptors.
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
#ifdef INCLUDE_VFP
            case VX_VFP_TASK:
                pDesc->ctxSize        = sizeof (VFP_CONTEXT);
                pDesc->ctxAlign       = _VFP_ALIGN_SIZE;
                pDesc->pCtxCreateRtn  = (COPROC_CTX_CREATE_RTN) vfpCtxCreate;
                pDesc->pCtxDeleteRtn  = (COPROC_CTX_DELETE_RTN) vfpCtxDelete;
                pDesc->pEnableRtn     = (COPROC_ENABLE_RTN) vfpEnable;
                pDesc->pDisableRtn    = (COPROC_DISABLE_RTN) vfpDisable;
                pDesc->pCtxSaveRtn    = (COPROC_CTX_SAVE_RTN) vfpSave;
                pDesc->pCtxRestoreRtn = (COPROC_CTX_RESTORE_RTN) vfpRestore;
                pDesc->pShowRtn       = (COPROC_CTX_SHOW_RTN) vfpCtxShow;
                break;
#endif /* INCLUDE_VFP */
            default:
                break;
            }
        }
    }

