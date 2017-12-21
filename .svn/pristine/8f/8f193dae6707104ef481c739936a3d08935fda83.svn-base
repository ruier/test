/* usrCoprocSimsolaris.c - Solaris simulator coprocessor discovery & initialization */

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
01j,17sep09,cww  Use fully qualified function pointers
01i,26apr05,dr   Add extern void aimFppLibInit(void)
01h,22apr05,dr   Add call to aimFppLibInit to pull in aimFppLib.o (SPR108100)
01g,09mar05,asa  Only use fppMRegs if INCLUDE_HW_FP_MREGS is defined (SPR 105886)
                 Removed all pointer assigns equal NULL as pDesc is initialized 
                 as NULL.  updated copyright to 2005
01e,18jan05,dbt  Added #ifdef INCUDE_HW_FP around floating point routines use.
01d,08oct04,jmp  completed mRegs support (SPR# 102390).
01c,02oct04,yvp  Added mRegs support.
01b,27aug04,rlg  fix for SPR 100461 bounded the Show function initialization
                 with if defs to keep from populating if component is not
                 defined.

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

#ifdef INCLUDE_HW_FP_SHOW
IMPORT void   fppRegListShow (void);
IMPORT STATUS fppMRegs (int, FP_CONTEXT *, char *, BOOL *);
#endif /* INCLUDE_HW_FP_SHOW */

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

#ifdef	INCLUDE_HW_FP
	    case VX_FP_TASK:
		pDesc->ctxSize        = sizeof (FP_CONTEXT);
		pDesc->ctxAlign       = _ALLOC_ALIGN_SIZE;
		pDesc->pCtxCreateRtn  = (COPROC_CTX_CREATE_RTN) fppCtxCreate;
		pDesc->pCtxDeleteRtn  = (COPROC_CTX_DELETE_RTN) fppCtxDelete;
		pDesc->pEnableRtn     = (COPROC_ENABLE_RTN) fppEnable;
		pDesc->pDisableRtn    = (COPROC_DISABLE_RTN) fppDisable;
		pDesc->pCtxSaveRtn    = (COPROC_CTX_SAVE_RTN) fppSave;
		pDesc->pCtxRestoreRtn = (COPROC_CTX_RESTORE_RTN) fppRestore;
#ifdef INCLUDE_HW_FP_SHOW	 /* change for spr 100461 */
		pDesc->pShowRtn       = (COPROC_CTX_SHOW_RTN) fppCtxShow;
		pDesc->pRegListShowRtn = 
				(COPROC_REG_LIST_SHOW_RTN) fppRegListShow;;
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

