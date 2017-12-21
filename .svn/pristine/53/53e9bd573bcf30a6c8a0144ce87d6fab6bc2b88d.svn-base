/* uicIntr.c - IBM Universal Interrupt Controller (UIC) library */

/*
********************************************************************************
   This source and object code has been made available to you by IBM on an
   AS-IS basis.

   IT IS PROVIDED WITHOUT WARRANTY OF ANY KIND, INCLUDING THE WARRANTIES OF
   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE OR OF NONINFRINGEMENT
   OF THIRD PARTY RIGHTS.  IN NO EVENT SHALL IBM OR ITS LICENSORS BE LIABLE
   FOR INCIDENTAL, CONSEQUENTIAL OR PUNITIVE DAMAGES.  IBM'S OR ITS LICENSOR'S
   DAMAGES FOR ANY CAUSE OF ACTION, WHETHER IN CONTRACT OR IN TORT, AT LAW OR
   AT EQUITY, SHALL BE LIMITED TO A MAXIMUM OF $1,000 PER LICENSE.  No license
   under IBM patents or patent applications is to be implied by the copyright
   license.

   Any user of this software should understand that neither IBM nor its
   licensors will be responsible for any consequences resulting from the use
   of this software.

   Any person who transfers this source code or any derivative work must
   include the IBM copyright notice, this paragraph, and the preceding two
   paragraphs in the transferred software.

   Any person who transfers this object code or any derivative work must
   include the IBM copyright notice in the transferred software.

   COPYRIGHT   I B M   CORPORATION 2000
   LICENSED MATERIAL  -  PROGRAM PROPERTY OF  I B M"

********************************************************************************
\NOMANUAL
*/

/* Copyright 1984-2004 Wind River Systems, Inc. */

/* modification history
-----------------------
01h,30aug04,mdo  Documentation fixes for apigen
01g,20nov03,to   added intDisconnect() functionality
01f,11jan02,pch  fix Diab warnings
01e,24jul01,aeg  re-enabled CE & EE exceptions in sysUicIntHandler (SPR #69542).
01d,15may00,mcg  register name updates to match 405GP User Manual
01c,17mar00,mcg  Set initial trigger, level, and priority with macros, and
                 simplified sysUicIntHandler
01b,23jan00,mcg  bug fix in sysUicIntDisable
01a,19sep99,mcg  created from Spruce CPC700Intr.c version 01a. Uses DCR regs.
*/

/*
This library provides various interface routines to manipulate and connect
to external hardware interrupts of the IBM Universal Interrupt Controller (UIC).
This controller is in the IBM Blue Logic ASIC core library.
*/

#include "private/eventP.h"

/* defines */

#define INT_LEVEL_MAX   31             /* the UIC has 32 interrupt sources */
#define INT_LEVEL_MIN   0

/* externals */

/* globals */

void sysUicIntHandler (void);

/* locals */

LOCAL VOIDFUNCPTR       sysIntBlTbl [32] = {(VOIDFUNCPTR) NULL};
LOCAL int               sysIntArg [32]   = {0};

/* forward LOCAL functions declarations */

LOCAL STATUS    sysUicIntConnect (VOIDFUNCPTR * vector, VOIDFUNCPTR routine,
                                  int parameter);
LOCAL STATUS    sysUicIntDisconnect (VOIDFUNCPTR * vector, VOIDFUNCPTR routine,
                                     int parameter);
LOCAL STATUS    sysUicIntEnable  (int intLevel);
LOCAL STATUS    sysUicIntDisable (int intLevel);

/*******************************************************************************
*
* sysUicIntrInit - initialize the UIC interrupt facility
*
* This routine initializes the interrupt facility for the UIC interrupt
* controller.
*
* RETURNS: OK, always.
* 
* ERRNO
*/
STATUS sysUicIntrInit
    (
    void
    )
    {
    /* set the BSP driver specific Interrupt Handler and intConnect routines */

    _func_intConnectRtn = sysUicIntConnect;
    _func_intDisconnectRtn = sysUicIntDisconnect;
    _func_intEnableRtn = sysUicIntEnable;
    _func_intDisableRtn = sysUicIntDisable;

    /*
     * Initial Interrupt controller setup
     */

    sysDcrUicsrClear(0xFFFFFFFF);        /* clear all ints                    */
    sysDcrUicerSet(0x00000000);          /* disable all ints                  */
    sysDcrUiccrSet(UIC_INTR_CRITICAL);   /* All ints except FPGA SMI non-crit */
    sysDcrUicprSet(UIC_INTR_POLARITY);   /* set int polarities                */
    sysDcrUictrSet(UIC_INTR_TRIGGER);    /* set int trigger levels all level  */
    sysDcrUicvcrSet(0x00000001);         /* INT0 has highest priority         */
    sysDcrUicsrClear(0xFFFFFFFF);        /* clear all ints again              */

    return (OK);
    }

/*******************************************************************************
*
* sysUicIntConnect - connect a C routine to a hardware interrupt
*
* This routine connects a specified C routine to a specified
* interrupt vector.  It is called by intConnect().
*
* The UIC does not provide vectors for external interrupts. Instead, vectors
* are assigned according to the bits in the UIC's Enable Register (UIC_ER).
*
* RETURNS: OK, or ERROR if <vector> is out of range.
*
* ERRNO
*
* SEE ALSO: intConnect(), sysUicIntHandler()
*/
LOCAL STATUS sysUicIntConnect
    (
    VOIDFUNCPTR * vector,       /* interrupt vector to attach to     */
    VOIDFUNCPTR   routine,      /* routine to be called              */
    int           parameter     /* parameter to be passed to routine */
    )
    {
    if (((int)vector > INT_LEVEL_MAX) || ((int)vector < INT_LEVEL_MIN))
        return (ERROR);
    else
        {
        sysIntBlTbl[(int)vector] = routine;
        sysIntArg[(int)vector]   = parameter;
        return (OK);
        }
    }

/*******************************************************************************
*
* sysUicIntDisconnect - disconnect a C routine from a hardware interrupt
*
* This routine disconnects a specified C routine from a specified
* interrupt vector.  It is called by intDisconnect().
*
* RETURNS: OK, or ERROR if <vector> is out of range.
*
* ERRNO
*
* SEE ALSO: intDisconnect(), sysUicIntHandler()
*/
LOCAL STATUS sysUicIntDisconnect
    (
    VOIDFUNCPTR * vector,       /* interrupt vector to detach from   */
    VOIDFUNCPTR   routine,      /* routine to be disconnected        */
    int           parameter     /* parameter to be matched           */
    )
    {
    if (((int)vector > INT_LEVEL_MAX) || ((int)vector < INT_LEVEL_MIN))
        return (ERROR);
    else
        {
        sysIntBlTbl[(int)vector] = (VOIDFUNCPTR) NULL;
        sysIntArg[(int)vector]   = 0;
        return (OK);
        }
    }

/*******************************************************************************
*
* sysUicIntHandler - UIC external interrupt handler
*
* This is the external interrupt handler for the UIC
*
* RETURNS: N/A
*
* ERRNO
*/

void sysUicIntHandler
    (
    void
    )
    {
    int vector;
    UINT32 uicmsr;               /* contents of Masked Status Register */
    UINT32 uicer;		 /* contents of the Enable Register */
    UINT32 uiccr;		 /* contents of the Critical Register */

    /*
     * Get contents of UIC_MSR.  This register is read-only
     * and reflects the value of UIC_SR ANDed with UIC_ER.
     */

    uicmsr = sysDcrUicmsrGet();

    /*
     * Determine the interrupt source.
     */

    vector = vxFirstBit(uicmsr);

#ifdef INCLUDE_WINDVIEW
    WV_EVT_INT_ENT(vector)
#endif

    if (vector <= INT_LEVEL_MAX)
        {

	/*
	 * The current interrupt and all lower priority interrupts are
	 * masked out.  The initialization performed in sysUicIntrInit()
	 * configures INT0 (MSB of the UIC enable register) as the highest
	 * priority interrupt.  Note that the UIC enable register does
	 * NOT affect the watchdog, PIT, or FIT interrupts.
	 */

	uicer = sysDcrUicerGet ();
	sysDcrUicerSet (uicer & (0xFFFFFFFF << (INT_LEVEL_MAX + 1 - vector)));


	/*
	 * To enable interrupt nesting, an intUnlock() is performed to
	 * set the EE and/or CE bits in the MSR.  The critical register
	 * (UIC_CR) is read to determine if the current vector has been
	 * configured as a critical exception.  If the current interrupt
	 * is critical, only the CE bit is set in the MSR permitting the
	 * nesting of only critical interrupts.  Otherwise, both the CE
	 * and EE bits are set permitting the nesting of critical and
	 * non-critical interrupts.
	 */

        uiccr = sysDcrUiccrGet ();

	if ((uiccr & (0x80000000 >> vector)) == 0)
	    intUnlock (_PPC_MSR_EE | _PPC_MSR_CE);
	else
	    intUnlock (_PPC_MSR_CE);

        /*
         * Call the appropriate handler with the appropriate argument
         */

        if (sysIntBlTbl [vector] != (VOIDFUNCPTR) NULL)
            (*sysIntBlTbl [vector]) (sysIntArg[vector]);
        else
            logMsg ("uninitialized interrupt vector %d\n", vector, 0,0,0,0,0);


	/*
	 * Clear the appropriate interrupt status bit in the UIC_SR.  This
	 * is performed after the handler is invoked to ensure the hardware
	 * device has been acknowledged prior to clearing the status bit.
	 * This is important for interrupts configured as level sensitive.
	 */

        sysDcrUicsrClear (1 << (INT_LEVEL_MAX - vector));

        /* re-enable the current and lower priority interrupts */

        sysDcrUicerSet (uicer);
        }
    }

/*******************************************************************************
*
* sysUicIntDisable - disable an external interrupt level
*
* This routine disables a specified external interrupt for the UIC
*
* RETURNS: OK, or ERROR if <intLevel> is not in the range 0 - 31.
*
* ERRNO
*
* SEE ALSO: sysUicIntEnable()
*/
LOCAL STATUS sysUicIntDisable
    (
    int intLevel        /* interrupt level to disable */
    )
    {
    UINT32 intMask;

    if (intLevel > INT_LEVEL_MAX || intLevel < INT_LEVEL_MIN)
        return (ERROR);

    intMask = 1 << (INT_LEVEL_MAX - intLevel);

    /*
     * disable the interrupt level
     */
    sysDcrUicerSet((~intMask) & sysDcrUicerGet());

    sysDcrUicsrClear(intMask);                    /* clear pending interrupts */

    return (OK);
    }

/*******************************************************************************
*
* sysUicIntEnable - enable an external interrupt level
*
* This routine enables a specified external interrupt for the UIC
*
* RETURNS: OK, or ERROR if <intLevel> is not in the range 0 - 31.
*
* ERRNO
*
* SEE ALSO: sysUicIntDisable()
*/
LOCAL STATUS sysUicIntEnable
    (
    int intLevel        /* interrupt level to enable */
    )
    {
    UINT32 intMask;

    if (intLevel > INT_LEVEL_MAX || intLevel < INT_LEVEL_MIN)
        return (ERROR);

    intMask = 1 << (INT_LEVEL_MAX - intLevel);

    sysDcrUicsrClear(intMask);                    /* clear pending interrupts */

    /*
     * enable the interrupt level
     */
    sysDcrUicerSet(intMask | sysDcrUicerGet());

    return (OK);
    }

