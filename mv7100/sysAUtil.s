/* sysAUtil.s - system-dependent assembly routines */

/*
 * Copyright (c) 1984-1999, 2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 2004-2005 Motorola, Inc. All Rights Reserved */

/*
modification history
--------------------
01g,17mar08,mdg Updated for mv7100.
01f,02aug05,efb Added function to clear both timebase registers.
01e,12apr05,cak Removed the sysCacheFlush() routine.
01d,02feb05,cak Added sysCacheErrorTest routine for testing.
01c,09nov04,scb Added time base register access support.
01b,11oct04,scb Tab formatting.
01a,22sep04,scb Created, moved some former sysALib.s routines over.
*/

/*
DESCRIPTION
This module contains system-dependent routines written in assembly
language.
*/

#define _ASMLANGUAGE

/* includes */

#include "vxWorks.h"
#include "sysLib.h"
#include "config.h"
#include "regs.h"
#include "asm.h"
#include "mpc8641.h"

/* defines */

#define CACHE_ALIGN_SHIFT       5       /* Cache line size == 2**5 */

/* globals */

        FUNC_EXPORT(sysPCGet)           /* get the value of the PC register */
        FUNC_EXPORT(sysTimeBaseLPut)
        FUNC_EXPORT(sysTimeBaseLGet)
        FUNC_EXPORT(sysTimeBaseUPut)
        FUNC_EXPORT(sysTimeBaseUGet)
        FUNC_EXPORT(sysTimeBaseClr)
        FUNC_EXPORT(sysTblGet)

/* externals */

        _WRS_TEXT_SEG_START

/*******************************************************************************
*
* sysPCGet - Get the value of the PC (Program Counter)
*
* This routine returns the value of the PC.
*
* UINT32 sysPCGet (void)
*
* RETURNS: the Program Counter Register (PC) value.
*/

FUNC_BEGIN(sysPCGet)
        mflr    r4      /* Save LR value */
        bl  Next        /* Set PC */
Next:
        mflr    r3      /* Get PC */
        mtlr    r4      /* Restore LR value */
        blr
FUNC_END(sysPCGet)

/*******************************************************************************
*
* sysTimeBaseLPut - write to lower half of Time Base register
*
* This routine will write the contents of r3 to the lower half of the time
* base register.
*
* From a C point of view, the routine is defined as follows:
*
* void sysTimeBaseLPut
* (
* ULONG       value to write
* )
*
* RETURNS: NA
*/

FUNC_BEGIN(sysTimeBaseLPut)
        sync
        mtspr   TBLR_W,r3
        isync
        bclr    20,0
FUNC_END(sysTimeBaseLPut)

/*******************************************************************************
*
* sysTimeBaseLGet - Get lower half of Time Base Register
*
* This routine will read the contents the lower half of the Time
* Base Register.
*
* From a C point of view, the routine is defined as follows:
*
* UINT32 sysTimeBaseLGet(void)
*
* RETURNS: value of lower half of time base register (in r3)
*/

FUNC_BEGIN(sysTimeBaseLGet)
        mfspr   r3,TBLR_R
        bclr    20,0
FUNC_END(sysTimeBaseLGet)

/*******************************************************************************
*
* sysTimeBaseUPut - write to upper half of Time Base register
*
* This routine will write the contents of r3 to the upper half of the Time
* Base register.
*
* From a C point of view, the routine is defined as follows:
*
* void sysTimeBaseUPut
* (
* ULONG       value to write
* )
*
* RETURNS: NA
*/

FUNC_BEGIN(sysTimeBaseUPut)
        sync
        mtspr   TBUR_W,r3
        isync
        bclr    20,0
FUNC_END(sysTimeBaseUPut)

/*******************************************************************************
*
* sysTimeBaseUGet - Get upper half of Time Base Register
*
* This routine will read the contents of the upper half of the Time
* Base Register.
*
* From a C point of view, the routine is defined as follows:
*
* UINT32 sysTimeBaseUGet(void)
*
* RETURNS: value of upper half of time base register (in r3)
*/

FUNC_BEGIN(sysTimeBaseUGet)
        mfspr   r3,TBUR_R
        bclr    20,0
FUNC_END(sysTimeBaseUGet)

/*******************************************************************************
*
* sysTimeBaseClr - Clear the entire Time Base Register to zero
*
* This routine will zero the contents of the lower and upper halves of
* the Time Base Register.
*
* From a C point of view, the routine is defined as follows:
*
* void sysTimeBaseClr(void)
*
* RETURNS: NA
*/

FUNC_BEGIN(sysTimeBaseClr)
        sync
        xor     r3,r3,r3
        mtspr   TBLR_W,r3
        mtspr   TBUR_W,r3
        isync
        bclr    20,0
FUNC_END(sysTimeBaseClr)


/*******************************************************************************
*
* sysTblGet - this routine gets the lower 32 bits of the Time Base Register.
*
* RETURN: None.
*/
FUNC_BEGIN(sysTblGet)
        mftb r3
        blr
FUNC_END(sysTblGet)

