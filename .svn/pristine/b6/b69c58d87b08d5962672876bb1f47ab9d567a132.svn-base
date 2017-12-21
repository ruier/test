/* uicIntr.c - IBM Universal Interrupt Controller (UIC) library */

/*
****************************************************************************
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

   COPYRIGHT   I B M   CORPORATION 2003
   LICENSED MATERIAL  -  PROGRAM PROPERTY OF  I B M"

****************************************************************************
\NOMANUAL
*/

/*
 * Copyright (c) 2003-2005, 2008, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* modification history
-----------------------
01h,20may13,d_l  disable lower priority interrupts. (WIND00417545)
01g,02sep08,x_f  Replace sysDcrInLong with dcrInLong, 
                 sysDcrOutLong with dcrOutLong
01f,30mar05,dr   Removing warnings
01e,15dec04,dr   Updating function descriptions
01d,01dec04,dr   Fixes as per peer review
01c,08sep04,dr   Implement sysUicIntDisconnect().  Support nested interrupts.
01b,07sep04,mdo  documentation fixes for apigen
01a,08jul03,tr   port from ebony code version 01b
*/

/*
This library provides various interface routines to manipulate and
connect to external hardware interrupts of the IBM Universal Interrupt
Controller (UIC), as implemented on the IBM PPC440EP processor.
The PPC440EP contains two UIC units chained together.  The UIC itself is
part of the IBM Blue Logic ASIC core library.
*/

#include "private/eventP.h"

/* defines */

#define INT_LEVEL_MAX 63   /* the UIC has 64 interrupt sources */
#define INT_LEVEL_MIN 0    /* 0-31 in UIC0, 32-63 in UIC1      */

#define INT_LEVEL_UIC0_MIN   0    /* First interrupt level on UIC-0 */
#define INT_LEVEL_UIC0_MAX  31    /* Last interrupt level on UIC-0 */
#define INT_LEVEL_UIC1_MIN  32    /* First interrupt level on UIC-0 */
#define INT_LEVEL_UIC1_MAX  63    /* Last interrupt level on UIC-0 */

#define INT_IS_CRT      (TRUE)         /* Int. is critical (for handler) */
#define INT_IS_NOT_CRT  (!INT_IS_CRT)  /* Int. is not critical */


#define VEC_TO_BIT_SHIFT    31      /* max shift when setting bits */
#define VEC_TO_MSK_SHIFT    32      /* max shift when masking bits */

#define vecToUicBit(v) 		(0x00000001 << (VEC_TO_BIT_SHIFT - (v))) 
#define vecToUicMask(v) 	(0xffffffff << (VEC_TO_MSK_SHIFT - (v)))


/* externals */

IMPORT STATUS   (*_func_intConnectRtn) (VOIDFUNCPTR *, VOIDFUNCPTR, int);
IMPORT STATUS   (*_func_intDisconnectRtn) (VOIDFUNCPTR *, VOIDFUNCPTR, int);
IMPORT int      (*_func_intEnableRtn) (int);
IMPORT int      (*_func_intDisableRtn) (int);

/* globals */

void    sysUicIntHandler (void);
void    sysUicCrtIntHandler (void);

/* locals */

LOCAL void          sysUicIntHandlerCommon (BOOL intIsCritical);
LOCAL VOIDFUNCPTR   sysIntBlTbl [INT_LEVEL_MAX+1] = {(VOIDFUNCPTR) NULL};
LOCAL int           sysIntArg [INT_LEVEL_MAX+1]   = {0};
LOCAL UINT32 sysUicIntMask[32] = {
    0x80000000, 0xC0000000, 0xE0000000, 0xF0000000,
    0xF8000000, 0xFC000000, 0xFE000000, 0xFF000000,
    0xFF800000, 0xFFC00000, 0xFFE00000, 0xFFF00000,
    0xFFF80000, 0xFFFC0000, 0xFFFE0000, 0xFFFF0000,
    0xFFFF8000, 0xFFFFC000, 0xFFFFE000, 0xFFFFF000,
    0xFFFFF800, 0xFFFFFC00, 0xFFFFFE00, 0xFFFFFF00,
    0xFFFFFF80, 0xFFFFFFC0, 0xFFFFFFE0, 0xFFFFFFF0,
    0xFFFFFFF8, 0xFFFFFFFC, 0xFFFFFFFE, 0xFFFFFFFF,
};

/* forward LOCAL functions declarations */

LOCAL STATUS  sysUicIntConnect (VOIDFUNCPTR * vector, VOIDFUNCPTR routine,
          int parameter);
LOCAL STATUS  sysUicIntDisconnect (VOIDFUNCPTR * vector, VOIDFUNCPTR routine,
          int parameter);
LOCAL STATUS  sysUicIntEnable  (int intLevel);
LOCAL STATUS  sysUicIntDisable (int intLevel);

/***************************************************************************
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

    dcrOutLong(UIC0_SR, 0xFFFFFFFF);          /* clear all ints          */
    dcrOutLong(UIC1_SR, 0xFFFFFFFF);          /* clear all ints          */
    dcrOutLong(UIC0_ER, 0x00000000);          /* disable all ints        */
    dcrOutLong(UIC1_ER, 0x00000000);          /* disable all ints        */
    dcrOutLong(UIC0_CR, UIC0_INTR_CRITICAL);  /* set critical ints       */
    dcrOutLong(UIC1_CR, UIC1_INTR_CRITICAL);  /* set critical ints       */
    dcrOutLong(UIC0_PR, UIC0_INTR_POLARITY);  /* set intr polarity       */
    dcrOutLong(UIC1_PR, UIC1_INTR_POLARITY);  /* set intr polarity       */
    dcrOutLong(UIC0_TR, UIC0_INTR_TRIGGER);   /* set level/edge trigger  */
    dcrOutLong(UIC1_TR, UIC1_INTR_TRIGGER);   /* set level/edge trigger  */
    dcrOutLong(UIC0_VCR, 0x00000001);         /* set highest priority    */
    dcrOutLong(UIC1_VCR, 0x00000001);         /* set highest priority    */
    dcrOutLong(UIC0_SR, 0xFFFFFFFF);          /* clear all ints again    */
    dcrOutLong(UIC1_SR, 0xFFFFFFFF);          /* clear all ints again    */

    return (OK);
    }

/***************************************************************************
*
* sysUicIntConnect - connect a C routine to a hardware interrupt
*
* This routine connects a specified C routine to a specified
* interrupt vector.  It is called by intConnect().
*
* The UIC does not provide vectors for external interrupts. Instead,
* vectors are assigned according to the bits in the UIC's Enable
* Register (UIC_ER).
*
* RETURNS: OK, or ERROR if <vector> is out of range.
*
* SEE ALSO: intConnect(), sysUicIntHandler()
* 
* ERRNO
*/
LOCAL STATUS sysUicIntConnect
    (
    VOIDFUNCPTR * vector,   /* interrupt vector to attach to     */
    VOIDFUNCPTR   routine,  /* routine to be called        */
    int           parameter /* parameter to be passed to routine */
    )
    {
    if (((int)vector > INT_LEVEL_MAX) || ((int)vector < INT_LEVEL_MIN))
        return (ERROR);


    sysIntBlTbl[(int)vector] = routine;
    sysIntArg[(int)vector]   = parameter;
    return (OK);
    }

/***************************************************************************
*
* sysUicIntDisconnect - disconnect a C routine from a hardware interrupt
*
* This routine disconnects a specified C routine from a specified
* interrupt vector.  It is called by intDisconnect().
*
* RETURNS: OK, or ERROR if <vector> is out of range.
*
* SEE ALSO: intDisconnect(), sysUicIntHandler()
* 
* ERRNO
*/
LOCAL STATUS sysUicIntDisconnect
    (
    VOIDFUNCPTR * vector,	/* interrupt vector to detach from   */
    VOIDFUNCPTR   routine,	/* routine to be disconnected        */
    int           parameter	/* parameter to be matched           */
    )
    {
    if (((int)vector > INT_LEVEL_MAX) || ((int)vector < INT_LEVEL_MIN))
        return (ERROR);

    sysIntBlTbl[(int)vector] = (VOIDFUNCPTR) NULL;
    sysIntArg[(int)vector]   = 0;
    return (OK);
    }

/***************************************************************************
*
* sysUicIntHandler - UIC non-critical external interrupt handler 
*
* Non-critical interrupt handler entry point. 
* 
* This routine gets connected to the _EXC_OFF_INTR vector in the CPU's
* exception table by sysHwInit() during system hardware initialization.
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
    /* call common handler code with NON CRITICAL flag */
    sysUicIntHandlerCommon( INT_IS_NOT_CRT);
    }

/***************************************************************************
*
* sysUicCrtIntHandler - UIC critical external interrupt handler
*
* Critical interrupt handler entry point. 
* 
* This routine gets connected to the _EXC_OFF_CRTL vector in the CPU's
* exception table by sysHwInit() during system hardware initialization.
*
*
* RETURNS: N/A
* 
* ERRNO
*/
void sysUicCrtIntHandler
    (
    void
    )
    {
    /* call common handler code with CRITICAL flag */
    sysUicIntHandlerCommon( INT_IS_CRT ); 
    }

/***************************************************************************
*
* sysUicIntHandlerCommon - UIC external interrupt handler common code
*
* This is the external interrupt handler for the UIC. It must be called
* from either entry interrupt handler entry points (sysUicIntHandlerEnt and
* sysUicCrtIntHandlerEnt).
*
* RETURNS: N/A
* 
* ERRNO
* /NOMANUAL
*/
LOCAL void sysUicIntHandlerCommon
    (
    BOOL  intIsCritical
    )
    {
    int   vector;
    UINT32  uicmsr;     /* contents of Masked Status Register */
    UINT32  uic0Cr;     /* UIC-0 CR */
    UINT32  uic1Cr;     /* UIC-1 CR */
    UINT32  uic0Er = 0; /* UIC-0 ER */
    UINT32  uic1Er = 0; /* UIC-1 ER */
    UINT32  newUicXEr;  /* temporary variable for readability */

    /*
     * Get contents of UIC0_MSR.  This register is read-only
     * and relects the value of UIC0_SR ANDed with UIC0_ER.
     */

    uicmsr = dcrInLong(UIC0_MSR);

    /*
     * Determine the interrupt source.
     */

    vector = vxFirstBit(uicmsr);

    if ((vector == 30)|| (vector == 31)) /* dchain interrupt */
      {
      uicmsr = dcrInLong(UIC1_MSR);
      vector = vxFirstBit(uicmsr);
      vector+= 32;
      }

#ifdef INCLUDE_WINDVIEW
    WV_EVT_INT_ENT(vector)
#endif

    if ( vector <= INT_LEVEL_MAX )
        {

        /*
         * The vector is valid. Is it in UIC 0 or UIC 1?
         */

        if (vector > INT_LEVEL_UIC0_MAX)
            {

            /* In UIC-1 */

            /* 
             * Disable all lower-priority non-critical interrupts. 
             * 
             * If we are being called from a critical interrupt, 
             * we must also disable lower-priority critical interrupts.
             */

            if (intIsCritical) 
                {

                /* Critical interrupt, disable all lower priority interrupts */

                uic1Er = dcrInLong(UIC1_ER);
                newUicXEr = uic1Er & sysUicIntMask[vector - INT_VEC_UIC1_BASE];
                }
            else
                {

                /* 
                 * Non-critical interrupt, disable only non-critical, 
                 * lower priority interrupts.
                 */

                uic1Cr = dcrInLong(UIC1_CR);
                uic1Er = dcrInLong(UIC1_ER);
                newUicXEr = uic1Er & 
                            (sysUicIntMask[vector - INT_VEC_UIC1_BASE] |
                             (uic1Cr & ~sysUicIntMask[vector - INT_VEC_UIC1_BASE]) );
                }

            /* 
             * Disable this interrupt, critical or not, as we don't want to
             * be stuck having to constantly reservice this interrupt.
             */

            newUicXEr &= ~(1 << (INT_LEVEL_MAX - vector));

            /* Update the Enable register */

            dcrOutLong(UIC1_ER, newUicXEr);

            /* Clear the Status registers */

            dcrOutLong(UIC1_SR, (1 << (INT_LEVEL_MAX - vector)));
            dcrOutLong(UIC0_SR, 0x00000003);    /* clear daisychain */
            }
        else
            {

            /* In UIC-0 */

            /* 
             * Disable all lower-priority interrupts. 
             * 
             * If we are being called from a critical interrupt, 
             * we must also disable lower-priority critical interrupts.
             */

            if (intIsCritical) 
                {

                /* Critical interrupt, disable all lower priority interrupts */

                uic0Er = dcrInLong(UIC0_ER);
                newUicXEr = uic0Er & sysUicIntMask[vector - INT_VEC_UIC0_BASE];
                }
            else
                {
                /* 
                 * Non-critical interrupt, disable only non-critical, lower
                 * priority interrupts 
                 */

                uic0Cr = dcrInLong(UIC0_CR);
                uic0Er = dcrInLong(UIC0_ER);
                newUicXEr = uic0Er & 
                            (sysUicIntMask[vector - INT_VEC_UIC0_BASE] |
                             (uic0Cr & ~sysUicIntMask[vector - INT_VEC_UIC0_BASE]) );
                }


            /* 
             * Disable this interrupt, critical or not, as we don't want to
             * be stuck having to constantly reservice this interrupt.
             */

            newUicXEr &= ~(1 << (INT_LEVEL_MAX - vector - 32));

            /* Update the register */
            dcrOutLong(UIC0_ER, newUicXEr);

            /* Clear the Status register. */
            dcrOutLong(UIC0_SR,(1 << (INT_LEVEL_MAX - vector - 32)));
            }

        }
    else
        {

		/* This should never happen. Bail out. */

        if (intContext()) 
            { 
            logMsg("uicIntr: Invalid vector. Called from interrupt level.\n",1,2,3,4,5,6);
            }
        else 
            {
            logMsg("uicIntr: Invalid vector. Called from task level.\n",1,2,3,4,5,6);
            }

		return;

        } 

    /* 
     * Enable interrupt nesting. At this point all higher priority
     * interrupts and, in the case of servicing a non-critical 
     * interrupt, all lower priority critical interrupts are enabled.
     */

    if (intIsCritical)
        intUnlock(_PPC_MSR_CE);
    else
        intUnlock(_PPC_MSR_EE);

    /*
     * Call the appropriate handler with the appropriate argument
     */

    if (sysIntBlTbl [vector] != (VOIDFUNCPTR) NULL)
        (*sysIntBlTbl [vector]) (sysIntArg[vector]);
    else
        logMsg ("uninitialized interrupt vector %d\n", vector, 0,0,0,0,0);

    /*
     * Clear this interrupt level to ensure that this interrupt level is 
     * disabled before re-enabling interrupts. 
     */

    if (intIsCritical)
     vxMsrSet(vxMsrGet() & ~(_PPC_MSR_CE));
    else
	    vxMsrSet(vxMsrGet() & ~(_PPC_MSR_EE));

    /*
     * Reenable the interrupts as they were went we got called.
     */

    if (vector > INT_LEVEL_UIC0_MAX)
        {
        dcrOutLong(UIC1_ER, uic1Er );
        }
    else
        {
        dcrOutLong(UIC0_ER, uic0Er );
        }

    }

/***************************************************************************
*
* sysUicIntDisable - disable an external interrupt level
*
* This routine disables a specified external interrupt for the UIC
*
* RETURNS: OK, or ERROR if <intLevel> is out of range.
*
* SEE ALSO: sysUicIntEnable()
* 
* ERRNO
*/
LOCAL STATUS sysUicIntDisable
    (
    int   intLevel  /* interrupt level to disable */
    )
    {
    UINT32  intMask;
    int     key;        

    if (intLevel > INT_LEVEL_MAX || intLevel < INT_LEVEL_MIN)
        return (ERROR);

    if (intLevel > INT_LEVEL_UIC0_MAX)        /* For UIC1 */
        {
        intLevel-=32;
        intMask = 1 << (INT_LEVEL_MAX - intLevel - 32);

        /*
         * disable the interrupt level.
         * We must lock out interrupts to disable the hardware
         * as the handler may crush what we just did in UIC_ER.
         */

        key = intLock();                        /* lock interrupts */

        /* really disable interrupt */
        dcrOutLong(UIC1_ER, (~intMask) & dcrInLong(UIC1_ER));

        intUnlock(key);                         /* re-enable interrupts */

        dcrOutLong(UIC1_SR, intMask);       /* clear pending interrupts */
        dcrOutLong(UIC0_SR, 0x00000003);      /* clear dchained UIC1 ints */
        }
    else
        {
        intMask = 1 << (INT_LEVEL_MAX - intLevel - 32);

        /*
         * disable the interrupt level
         * We must lock out interrupts to disable the hardware
         * as the handler may crush what we just did in UIC_ER.
         */

        key = intLock();                        /* lock interrupts */

        /* really disable interrupt */
        dcrOutLong(UIC0_ER, (~intMask) & dcrInLong(UIC0_ER));

        intUnlock(key);                         /* re-enable interrupts */

        dcrOutLong(UIC0_SR, intMask);   /* clear pending interrupts */
        }

    return (OK);
    }

/***************************************************************************
*
* sysUicIntEnable - enable an external interrupt level
*
* This routine enables a specified external interrupt for the UIC
*
* RETURNS: OK, or ERROR if <intLevel> is out of range.
*
* SEE ALSO: sysUicIntDisable()
* 
* ERRNO
*/
LOCAL STATUS sysUicIntEnable
    (
    int   intLevel  /* interrupt level to enable */
    )
    {
    UINT32  intMask;
    int     key;        

    if (intLevel > INT_LEVEL_MAX || intLevel < INT_LEVEL_MIN)
        return (ERROR);


    if (intLevel > INT_LEVEL_UIC0_MAX)        /* For UIC1 */
        {
        intLevel -= 32;
        intMask = 1 << (INT_LEVEL_MAX - intLevel - 32);
        dcrOutLong(UIC1_SR, intMask);        /* clear pending interrupts */
        dcrOutLong(UIC0_SR, 0x00000003);     /* clear pending dchain     */

        /*
         * enable the interrupt level
         * We must lock out interrupts to enable the hardware
         * as the handler may crush what we just did in UIC_ER.
         */

        while (intContext() ) 
            {
            /* wait until we are out of interrupt context */
            }

        key = intLock();                        /* lock interrupts */

        dcrOutLong(UIC1_ER, intMask | dcrInLong(UIC1_ER));

        /* Enable dchain*/
        dcrOutLong(UIC0_ER, 0x00000003 | dcrInLong(UIC0_ER));

        intUnlock(key);                         /* re-enable interrupts */

        }
    else
        {
        intMask = 1 << (INT_LEVEL_MAX - intLevel - 32);
        dcrOutLong(UIC0_SR, intMask);        /* clear pending interrupts */

        /*
         * enable the interrupt level
         * We must lock out interrupts to enable the hardware
         * as the handler may crush what we just did in UIC_ER.
         */

        while (intContext() ) 
            {
            /* wait until we are out of interrupt context */
            }

        key = intLock();                        /* lock interrupts */

        dcrOutLong(UIC0_ER, intMask | dcrInLong(UIC0_ER));

        intUnlock(key);                         /* re-enable interrupts */

        }

    return (OK);
    }
