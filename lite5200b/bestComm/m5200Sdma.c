/* m5200Sdma.c - m5200 SDMA driver support */

/*
 * Copyright (c) 2003-2006, 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01g,04jan10,wqi Omit the OR conjunction.(WIND00141923)
01f,21feb06,pkr  fix reentrancy problem in bestcomm api (bit register access)
01e,18mar05,j_b  code cleanup (apigen)
01d,24feb05,j_b add function header blocks
01c,27dec04,k_p fixed diab compilation error, changed the macro
                #define MBAR_VALUE (vxMBarGet()) to
                #define MBAR_VALUE ((unsigned long)vxMBarGet()).
01b,05mar04,bjn Modified to build as driver source rather than from sysLib.c
01a,17Jul03,pkr written
*/

/*
DESCRIPTION
Provide support for SDMA Bestcomm API.
*/

/*
change log details
------------------
01b * Building m5200Sdma.c from the BSP file sysLib.c caused the Tornado
      project creation to fail when building dependencies for the Diab
      toolchain. Since this file does not depend a great deal on the BSP
      configuration files, it can be built as a standalone driver and the
      BSP just links agains the object code.
    * Variable: MBarGlobal also defined in bestcomm_api.c - removed duplicate
      from this file.
    * Variable: sysSDMA - moved this global to sysLib.c.
*/

#include "vxWorks.h"
#include "logLib.h"
#include "iv.h"
#include "intLib.h"
#include "drv/intrCtl/m5200Intr.h"
#include "drv/dma/m5200Sdma.h"
#include "bestcomm_api.h"
#include "task_api/bestcomm_cntrl.h"
#include "task_api/bestcomm_api_mem.h"
#include "intLib.h"

extern FUNCPTR _func_sdmaIntEnableRtn;
extern FUNCPTR _func_sdmaIntDisableRtn;
extern FUNCPTR _func_sdmaIntSetLevelRtn;
extern void m5200SdmaIntTrigger(int intNum);

extern void * vxMBarGet (void);

#define MBAR_VALUE ((unsigned long)vxMBarGet())

#define SDMA_DECLARE_CRITICAL int level;
#define SDMA_ENTER_CRITICAL level = intLock();
#define SDMA_LEAVE_CRITICAL intUnlock(level);

/*******************************************************************************
*
* sdmaIntSetLevel - set SDMA interrupt level
*
* This routine sets the SDMA interrupt level.
*
* RETURNS: OK always
*/

LOCAL STATUS sdmaIntSetLevel
    (
    int intNum,
    int level
    )
    {
    /* TBD */
    return OK;
    }


/*******************************************************************************
*
* sdmaDemuxInt - handle SDMA interrupt
*
* This routine determines and calls the proper SDMA interrupt handler.
*
* RETURNS: N/A
*/

LOCAL void sdmaDemuxInt
    (
    int arg
    )
    {
    int pend = sysSDMA->IntPend & ~(sysSDMA->IntMask);
    int intNum = -1;
    int i, msk;


    if (pend & 0x10000000)
        intNum = INUM_SDMA_TEA;
    else
        for (i = INUM_SDMA_TASK0, msk=1; i <= INUM_SDMA_TASK15; i++, msk<<=1)
            {
            if ((pend & msk) != 0)
                {
                sysSDMA->IntPend = msk; /* clear pending int */
                intNum = i;
                break;
                }
            }

    if (intNum >= 0)
        m5200SdmaIntTrigger(intNum);
    }


/*******************************************************************************
*
* sdmaTeaInt - handle TEA interrupt
*
* This routine clears and logs the a TEA interrupt.
*
* RETURNS: N/A
*/

LOCAL void sdmaTeaInt
    (
    int arg
    )
    {
    int taskNum = ((sysSDMA->IntPend)>>24)&0xF;

    sysSDMA->IntPend |= 0x10000000; /* clear the event */

    logMsg("SDMA: TEA detected in task %d\n",taskNum,2,3,4,5,6);
    }


LOCAL STATUS sdmaIntEnable(int intNum);
LOCAL STATUS sdmaIntDisable(int intNum);


/*******************************************************************************
*
* sysSdmaInit - initialize BestComm SDMA support
*
* This routine sets up the SmartDMA module and BestComm API.
*
* RETURNS: OK always
*/

STATUS sysSdmaInit
    (
    )
    {
    sysSDMA->taskBar = (unsigned long)SRAM_BASE_ADRS;

    /* set up SDMA interrupt support */
    sysSDMA->IntMask = 0xffffffff;
    sysSDMA->IntPend = 0xffffffff;

    _func_sdmaIntEnableRtn = sdmaIntEnable;
    _func_sdmaIntDisableRtn = sdmaIntDisable;
    _func_sdmaIntSetLevelRtn = sdmaIntSetLevel;
    intConnect (IV_BESTCOMM, sdmaDemuxInt, 0);
    intEnable(INUM_BESTCOMM);
    intConnect (IV_SDMA_TEA, sdmaTeaInt, 0);
    intEnable(INUM_SDMA_TEA);

    /* set up BestComm support */
    TasksInitAPI ((void *)MBAR_VALUE);
    TasksLoadImage (sysSDMA);

    sysSDMA->IPR[0] = 7;

#ifdef UN_USED_CODE
    /*
     * Set core and BestComm XLB priority the same.
     */
    *XLB_MPER |= 0x5;
    *XLB_MPR  = 0x77777171;
#endif /* UN_USED_CODE */

    /*
     * Turn off COMM bus prefetch. This affects all data movements on
     * the COMM bus. (Yes, _PE -- prefetch enable -- should probably be
     * named _PD.)
     */

    sysSDMA->PtdCntrl |= SDMA_PTDCNTRL_PE;

    return OK;

    }


/*******************************************************************************
*
* sdmaIntEnable - enable SDMA interrupt
*
* This routine enables the given SDMA interrupt.
*
* RETURNS: OK always
*/

LOCAL STATUS sdmaIntEnable
    (
    int intNum
    )
    {
    SDMA_DECLARE_CRITICAL

    SDMA_ENTER_CRITICAL
    /* range is checked in caller */
    if (intNum == INUM_SDMA_TEA)
        sysSDMA->IntMask &= 0xEFFFFFFF;
    else
        SDMA_INT_ENABLE( SDMA_INT_MASK, (intNum-INUM_SDMA_TASK0));
    SDMA_LEAVE_CRITICAL
    return OK;
    }


/*******************************************************************************
*
* sdmaIntDisable - disable SDMA interrupt
*
* This routine disables the given SDMA interrupt.
*
* RETURNS: OK always
*/

LOCAL STATUS sdmaIntDisable
    (
    int intNum
    )
    {
    SDMA_DECLARE_CRITICAL

    SDMA_ENTER_CRITICAL
    /* range is checked in caller */
    if (intNum == INUM_SDMA_TEA)
        sysSDMA->IntMask |= 0x10000000;
    else
        SDMA_INT_DISABLE( SDMA_INT_MASK, (intNum-INUM_SDMA_TASK0));
    SDMA_LEAVE_CRITICAL
    return OK;
    }
