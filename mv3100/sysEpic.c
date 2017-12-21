/* sysEpic.c - Embedded Programmable Interrupt Controller */

/*
 * Copyright (c) 2003-2006, 2009, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 2004-2005 Motorola Inc. All Rights Reserved */

/*
modification history
--------------------
01n,07mar13,d_l  Fixed prevent errors.
01m,21sep09,l_z  Remove the logMsg for spurious interrupt. (WIND00179032)
01l,12dec06,jch (WRS) Updated for VxWorks 6.x compatibility.
01k,02aug05,cak  Fixed a GNU compiler warning in sysEpicIntConnect().
01j,30jun05,cak  Totally overhauled the PIC driver.  Allow the configuration
		 of interrupts via a table in config.h.
01i,11apr05,cak  Modified sysEpicIntConnect() to set external interrupts
		 to level instead of edge sensitive - this is a temporary
		 solution until the epic code can be overhauled.
01h,10mar04,efb  Removed intUnlocking in main interrupt handler.
01g,17dec04,cak  Added show routine for system interrupt vector table.
01f,23aug04,scb  Starting point for "Big Easy".
01e,13oct03,mil  Added options for polarity and sense.
01d,12sep03,mil  Added support for critical interrupts.
01c,08jan03,mil  Created based on Sandpoint MPC107.
*/

/*
DESCRIPTION

This module implements the Embedded Programmable Interrupt Controller (EPIC)
driver for the MPC8540.

The EPIC is an integrated interrupt controller in the 8540 which
provides the following major capabilities:

  Support for connection of external interrupt controller device e.g. 8259
  like as implemented on a WinBond chip.
     
  12 external interrupt sources
  23 internal interrupt sources
  16 programmable interrupt priority levels
  Fully-nested interrupt delivery
  Spurious vector generation
  Route to critical interrupt destinations
  Route to external pin
  Configurable sense and polarity at initialization and runtime

This EPIC controller does not support:

 Runtime configuration of sense and polarity.
 Four Interprocessor interrupt channels, other than the ability to
  configure and connect an interrupt handler to the IPI interrupt sources.
 Four 32-bit messaging interrupt channels, other than the ability to
  configure and connect an interrupt handler to the messaging channel
  interrupt sources.

This driver allows a basic interface to the EPIC such as intializing it,
setting interrupt vectors, priorities, level/edge sense and interrupt
polarities, as well as enabling and disabling specific interrupts.

This driver implements a complete interrupt architecture system, complete
with vector table.

Since interrupt vectors can be shared, this driver does provide for
overloading of interrupt routines (i.e. there is a list of interrupt 
routines for each interrupt vector (level)).  To service a vector requires 
that all connected interrupt routines be called in order of their connection.

The following diagram shows an example of how interrupts can
be configured in this system.

    EPIC Vector table
0  |-------|  <-- external INT0 starts
   |-------|
   |-------|------------------------------+
    ....                                  |
   |-------|                              |
   |-------|                              |
12 |-------| <-- internal INT0 starts     |   (EPIC_MAX_EXT_IRQS) = 12
   |-------|                              |
   |-------|                              |
    ....                                  |
   |-------|                              |
   |-------|                              |
35 |-------| <-- global timer INT0 starts |   (EPIC_MAX_EXT_IRQS + \
   |-------|                              |    EPIC_MAX_IN_IRQS) = 35
   |-------|                              |
   |-------|                              |
39 |-------| <-- message INT0 starts      |   (EPIC_MAX_EXT_IRQS + \
   |-------|                              |    EPIC_MAX_IN_IRQS + \
   |-------|                              |    EPIC_MAX_GT_IRQS) = 39
   |-------|                              |
43 |-------| <-- IPI INT0 starts          |   (EPIC_MAX_EXT_IRQS + \
   |-------|                              |    EPIC_MAX_IN_IRQS + \
   |-------|                              |    EPIC_MAX_GT_IRQS + \
   |-------|                              |    EPIC_MAX_MSG_IRQS) = 43
47 |-------|
   |-------|
    ....
   |-------|
   |-------|
256|-------|

If there are other devices in the system capable of generating their own
vectors then we presume that an appropriate interrupt handler is created
and attached to the vector associated with the correct IRQ number.  That
interrupt handler would get a new vector directly from the device and then
call all of the handlers attached to that new vector.  Vector information is
stored in a linked list of INT_HANDLER_DESC structures. The sysIntTbl array
contains a pointer to the first entry for each vector.

.SH INITIALIZATION

This driver is initialized from the BSP, usually as part of sysHwInit().
The first routine to be called is sysEpicInit(). The routine resets the
global configuration register which resets the epic registers to default
values and then configures the interrupt sources based on the values in
the INTERRUPT_CONFIG_TABLE found in config.h.

The second routine to be called is sysEpicIntrInit().  This routine takes no
arguments. This routine allocates the vector table and connects the
top-level interrupt handlers to the system vector table.  All individual 
interrupt sources remain disabled.  Each has to be individually enabled by 
intEnable() before it will be unmasked and allowed to generate an interrupt.

.SH CRITICAL INTERRUPT

By default all interrupt sources are configured to utilize the int signal
for signalling an interrupt.  Some interrupt sources can, however, be 
configured, via the INTERRUPT_CONFIG_TABLE in config.h, to utilize the 
cint signal.

The critical interrupt handler uses information from the summary registers
CISR0 and CISR1.  The EPIC does not manage critical interrupts and hence
Iack or EOI do not apply.  
*/

/* includes */

#include "vxWorks.h"
#include "config.h"
#include "sysEpic.h"
#include "sysLib.h"
#include "stdio.h"
#include "string.h"

#ifdef INCLUDE_WINDVIEW
#include "private/eventP.h"
#endif

/* defines */

#ifndef CPU_INT_LOCK
#   define CPU_INT_LOCK(x) (*x = intLock ())
#endif

#ifndef CPU_INT_UNLOCK
#   define CPU_INT_UNLOCK(x) (intUnlock (x))
#endif

/* typedefs */

/* globals */

INT_HANDLER_DESC * sysIntTbl [INTERRUPT_TABLESIZE]; /* system interrupt tbl */
INTERRUPT_CONFIG_TABLE	/* instantiate interrupt config table from config.h */

/* externals */

IMPORT STATUS 	excIntConnect (VOIDFUNCPTR *, VOIDFUNCPTR);
IMPORT STATUS 	excIntCrtConnect (VOIDFUNCPTR *, VOIDFUNCPTR);

/* get the interrupt hook routines  prototypes*/

IMPORT STATUS	(*_func_intConnectRtn) (VOIDFUNCPTR *, VOIDFUNCPTR, int);
IMPORT int	(*_func_intEnableRtn)  (int);
IMPORT int	(*_func_intdisableRtn) (int);

/* locals */

/* Local xxxintEnable()/xxxintDisable() functions */

LOCAL INT_UTIL_DESC intUtil [] = {
                                     { INT_UTIL_MPC8540 }
                                       INT_UTIL_EXTRA
                                 };

/* forward declarations */

LOCAL void	sysEpicIntHandlerExec (int vector);
LOCAL void 	sysEpicIntHandler (void);
LOCAL void	sysEpicIntConfig (void);
LOCAL void 	sysEpicCrtIntHandler (void);

/*******************************************************************************
*
* sysEpicInit - initialize the epic controller
*
* This routine resets the global Configuration Register, thus it:
*  -  disables all interrupts
*  -  sets epic registers to reset values
*
* It then sets the EPIC operation mode to Mixed Mode (vs. Pass Through
* mode).
*
* RETURNS: N/A
*/    

void sysEpicInit
    (
    void
    )
    {
    ULONG 	gcrVal;

    /* Force the EPIC to be reset */

    gcrVal = sysEpicRegRead (CCSR_GLBL_GCR);
    gcrVal |= (EPIC_GCR_RESET);
    sysEpicRegWrite (CCSR_GLBL_GCR, gcrVal);

    /* wait for the reset sequence to be completed */
    
    while (sysEpicRegRead (CCSR_GLBL_GCR) & EPIC_GCR_RESET)
        {
        ; /* do nothing */
        }

    /* Configure EPIC for Mixed-Mode */

    gcrVal = sysEpicRegRead (CCSR_GLBL_GCR);
    gcrVal |= (EPIC_GCR_MODE_MIXED);
    sysEpicRegWrite (CCSR_GLBL_GCR, gcrVal);

    /* Clear all pending interrupt */

    while (((USHORT) epicIntAck()) != (USHORT) 0xffff)
        {
        epicEOI(); 
	}

    /* Configure, but do not unmask, the interrupt sources */

    sysEpicIntConfig();

    /*
     * Disable Interrupt Signals to the Processor
     *
     * Setting the task priority (TASKP) to 15 (0xF) will mask all
     * interrupts to the processor.
     */

    epicCurTaskPrioSet (EPIC_PRIORITY_MAX);
    }

/*******************************************************************************
*
* sysEpicIntrInit - initialize the interrupt table
*
* This function initializes the interrupt mechanism of the board.
*
* RETURNS: OK, if able to successfully connect interrupt handlers to 
* system vector table, ERROR otherwise. 
*/

STATUS  sysEpicIntrInit (void)
    {
    int vector;
    int rc;

    /* initialize the interrupt table */

    for (vector = 0; vector < INTERRUPT_TABLESIZE; vector++)
        {
	sysIntTbl [vector] = NULL;
        }

    /*
     * connect the interrupt demultiplexer to the PowerPC external 
     * interrupt exception vector.
     * i. e.  put the address of this interrupt handler in
     * the PowerPC's only external interrupt exception vector
     * which is  _EXC_OFF_INTR = 0x500
     * Also connect critical input pin handler _EXC_OFF_CRTL = 0x100.
     */

    rc = excIntConnect ((VOIDFUNCPTR *) _EXC_OFF_INTR, sysEpicIntHandler);
    rc = excIntCrtConnect ((VOIDFUNCPTR *) _EXC_OFF_CRTL, sysEpicCrtIntHandler);

    /*  
     * set up the BSP specific interrupt routines
     * Attach the local routines to the VxWorks system calls
     */

    _func_intConnectRtn  =  sysEpicIntConnect;
    _func_intEnableRtn   =  sysEpicIntEnable;
    _func_intDisableRtn  =  sysEpicIntDisable;

    /*
     * Enable Interrupt Signals to the Processor
     *
     * Setting the task priority (TASKP) to 0 will allow any interrupts
     * with a higher priority (than 0) to assert an interrupt to the 
     * core.
     */
 
    epicCurTaskPrioSet (EPIC_PRIORITY_MIN);
    
    return (rc);
    }

/*******************************************************************************
*
* sysEpicIntEnable - enable an EPIC interrupt source 
*
* This routine enables a specified EPIC interrupt source identified by
* its assigned vector.
*
* RETURNS: OK, ERROR if vector is outside of the range allowed by the EPIC.
*/

int sysEpicIntEnable
    (
    int vector
    )
    {
    UINT32 i;

    for (i = 0; ;)
        {
        if ( (vector >= intUtil[i].loIntNum) &&
             (vector <= intUtil[i].hiIntNum) )
            break;

        i++;
        if (i >= (sizeof(intUtil))/(sizeof(INT_UTIL_DESC)))
            return (ERROR);     /* No registered intEnable() function */
        }

    if (intUtil[i].intEnableFunc != NULL)
        return (intUtil[i].intEnableFunc (vector));
    else
        return (ERROR);
    }

/*******************************************************************************
*
* sysEpicIntDisable - disable an EPIC interrupt source 
*
* This routine disables a specified EPIC interrupt source identified by
* its assigned vector.
*
* RETURNS: OK, ERROR if vector is outside of the range allowed by the EPIC.
*/

int  sysEpicIntDisable
    (
    int vector
    )
    {
    UINT32 i;

    for (i = 0;;)
        {
        if ( (vector >= intUtil[i].loIntNum) &&
             (vector <= intUtil[i].hiIntNum) )
            break;

        i++;
        if (i >= (sizeof(intUtil))/(sizeof(INT_UTIL_DESC)))
            return (ERROR);     /* No registered intDisable() function */
        }

    if (intUtil[i].intDisableFunc != NULL)
        return (intUtil[i].intDisableFunc (vector));
    else
        return (ERROR);
    }

/*******************************************************************************
*
* sysEpicIntConnect - connect an interrupt handler to the system vector table
*
* This function connects an interrupt handler to the system vector table.
*
* RETURNS: OK or ERROR if vector is outside of the range supported by the
* vector table, or if unable to malloc space for a new interrupt handler 
* entry.
*/

STATUS sysEpicIntConnect
    (
    VOIDFUNCPTR * 	vector,		/* interrupt vector to attach */
    VOIDFUNCPTR		routine,	/* routine to be called */
    int			parameter	/* parameter to be passed to routine */
    )
    {
    INT_HANDLER_DESC *	pNewHandler;
    INT_HANDLER_DESC *	pCurrHandler;
    int			theVector =  (int)vector;
    int			intVal;
    UINT32		i;

    /* Check to see if intEnable() registered for this vector number */

    for (i = 0;;)
        {

        if ( (theVector >= intUtil[i].loIntNum) &&
             (theVector <= intUtil[i].hiIntNum) )
            break;              /* Registered - so continue */
        i++;
        if (i >= (sizeof(intUtil))/(sizeof(INT_UTIL_DESC)))
            return (ERROR);     /* No registered intEnable() function */
        }

    /*
     * We have intEnable/intDisable registered so we can safely connect
     * this interrupt.
     */

    if (((int)vector < 0)  || ((int) vector >= INTERRUPT_TABLESIZE)) 
	{
        return (ERROR);   /*  out of range  */
	}

    /* create a new interrupt handler */

    pNewHandler = malloc (sizeof (INT_HANDLER_DESC));

    /* check if the memory allocation succeed */

    if (pNewHandler == NULL)
	return (ERROR);

    /*  initialize the new handler  */

    pNewHandler->vec = routine;
    pNewHandler->arg = parameter;
    pNewHandler->next = NULL;

    /* install the handler in the system interrupt table  */

    intVal = intLock (); /* lock interrupts to prevent races */

    if (sysIntTbl [(int) vector] == NULL)
	{
        sysIntTbl [(int) vector] = pNewHandler;	/* single handler */
	}
    else
	{
        pCurrHandler = sysIntTbl[(int) vector];	/* multiple handlers */

        while (pCurrHandler->next != NULL)
            {
            pCurrHandler = pCurrHandler->next;
            }
        
        pCurrHandler->next = pNewHandler;
	}

    intUnlock (intVal);

    return (OK);
    }

/*******************************************************************************
*
* sysEpicIntHandlerExec  - execute the handlers for a given vector
* 
* This routine executes all the handlers chained to a given vector.
* If a vector has no handlers attached to it, a logMsg is generated.
*
* RETURNS: N/A
*/

LOCAL void sysEpicIntHandlerExec
    (
    int vector
    )
    {    
    INT_HANDLER_DESC * pCurrHandler;

    if ((pCurrHandler = sysIntTbl [vector]) == NULL)
        {
	logMsg ("uninitialized PIC interrupt vector 0x%x\r\n",
                vector, 0,0,0,0,0);
        }
    else
        {

	/* call Each respective chained interrupt handler  */

	while (pCurrHandler != NULL)
            {
  	    (*pCurrHandler->vec) (pCurrHandler->arg);
	    pCurrHandler = pCurrHandler->next;
            }
        }
    }

/*******************************************************************************
*
* sysEpicIntHandler - handles the (int) EPIC interrupts to the CPU
*
* This routine handles interrupts originating from the embedded interrupt
* controller on the MPC8540 PowerPC processor.  This handler is entered from 
* the 0x500 exception.
*
* Since the EPIC is the primary interrupt controller this driver
* first initiates an Epic acknowledge call and reads the vector
* put out by the EPIC. Subsequent vectors have to be obtained if
* an external interrupt controller is connected to one of the
* epic handlers. 
*
* This routine then processes the interrupt by calling all the interrupt
* service routines chained to the vector.
*
* Finally, this routine re-arms the interrupt at the PIC by performing an 
* PIC EOI.
*
* RETURNS:  N/A
*/

LOCAL void  sysEpicIntHandler (void)
    {
    int	epicIntVec;
    int spurious;
    int dontCare;

    /*
     * Acknowledge the Interrupt
     *
     * Reading this register acknowledges the pending highest priority
     * interrupt and returns its vector number.
     */

    epicIntVec = epicIntAck ();

    /* Ensure that this is not a spurious interrupt */

    spurious = sysEpicRegRead (CCSR_GLBL_SVR);
    WRS_ASM ("eieio");

    if (epicIntVec != spurious)
        {

        /*
         * Allow maskable interrupts to the CPU.  MPIC will hold off
         * lower and equal interrupts until EOI is performed.
         */

        CPU_INT_UNLOCK(_PPC_MSR_EE);

#ifdef INCLUDE_WINDVIEW
        WV_EVT_INT_ENT (epicIntVec);
#endif 

        /* Service the interrupt */

        sysEpicIntHandlerExec (epicIntVec);

        CPU_INT_LOCK (&dontCare);

        /* Signal End of Interrupt */

        WRS_ASM ("sync");
        epicEOI();
        WRS_ASM ("sync");
        }
    }

/*******************************************************************************
*
* sysEpicCrtIntHandler - handles the (cint) EPIC interrupts to the CPU
*
* This routine handles critical interrupts, or those that utilize cint
* to signal an interrupt.  This handler is entered from the 0x100 exception.
*/

LOCAL void  sysEpicCrtIntHandler (void)
    {
    volatile ULONG extSumm;
    volatile ULONG intSumm = 0;
    UINT32 vecRegOffset = 0x0;
    int vector;
    int i;

    while (((extSumm = (sysEpicRegRead(CCSR_GLBL_CISR0) & 0xfff)) != 0) ||
           ((intSumm = sysEpicRegRead(CCSR_GLBL_CISR1)) != 0))
        {

        /* service external & message interrupts first */

        for (i = 0; i < 16; i++)
            {
            if (((extSumm >> i) & 0x1) == 0x1)
                {
		switch (i)
		    {
		    case 0:
			vecRegOffset = CCSR_EIVPR11;
			break;
		    case 1:
			vecRegOffset = CCSR_EIVPR10;
			break;
		    case 2:
			vecRegOffset = CCSR_EIVPR9;
			break;
		    case 3:
			vecRegOffset = CCSR_EIVPR8;
			break;
		    case 4:
			vecRegOffset = CCSR_EIVPR7;
			break;
		    case 5:
			vecRegOffset = CCSR_EIVPR6;
			break;
		    case 6:
			vecRegOffset = CCSR_EIVPR5;
			break;
		    case 7:
			vecRegOffset = CCSR_EIVPR4;
			break;
		    case 8:
			vecRegOffset = CCSR_EIVPR3;
			break;
		    case 9:
			vecRegOffset = CCSR_EIVPR2;
			break;
		    case 10:
			vecRegOffset = CCSR_EIVPR1;
			break;
		    case 11:
			vecRegOffset = CCSR_EIVPR0;
			break;
		    case 12:
			vecRegOffset = CCSR_MIVPR3;
			break;
		    case 13:
			vecRegOffset = CCSR_MIVPR2;
			break;
		    case 14:
			vecRegOffset = CCSR_MIVPR1;
			break;
		    case 15:
			vecRegOffset = CCSR_MIVPR0;
			break;	
		    }	
		vector = (sysEpicRegRead(vecRegOffset) & EPIC_VECTOR_MSK);
		sysEpicIntHandlerExec (vector);
                }
            }

        /* service internal interrupts */

        for (i = 0; i < 32; i++)
            {
            if (((intSumm >> i) & 0x1) == 0x1)
                {
		switch (i)
		    {
		    case 3:
			vecRegOffset = CCSR_IIVPR28;
			break;
		    case 4:
			vecRegOffset = CCSR_IIVPR27;
			break;
		    case 5:
			vecRegOffset = CCSR_IIVPR26;
			break;
		    case 6:
			vecRegOffset = CCSR_IIVPR25;
			break;
		    case 7:
			vecRegOffset = CCSR_IIVPR24;
			break;
		    case 11:
			vecRegOffset = CCSR_IIVPR20;
			break;
		    case 12:
			vecRegOffset = CCSR_IIVPR19;
			break;
		    case 13:
			vecRegOffset = CCSR_IIVPR18;
			break;
		    case 17:
			vecRegOffset = CCSR_IIVPR14;
			break;
		    case 18:
			vecRegOffset = CCSR_IIVPR13;
			break;
		    case 19:
			vecRegOffset = CCSR_IIVPR12;
			break;
		    case 20:
			vecRegOffset = CCSR_IIVPR11;
			break;
		    case 21:
			vecRegOffset = CCSR_IIVPR10;
			break;
		    case 22:
			vecRegOffset = CCSR_IIVPR9;
			break;
		    case 23:
			vecRegOffset = CCSR_IIVPR8;
			break;
                    case 24:
                        vecRegOffset = CCSR_IIVPR7;
                        break;  
                    case 25:
                        vecRegOffset = CCSR_IIVPR6;
                        break;  
                    case 26:
                        vecRegOffset = CCSR_IIVPR5;
                        break;  
                    case 27:
                        vecRegOffset = CCSR_IIVPR4;
                        break;  
                    case 28:
                        vecRegOffset = CCSR_IIVPR3;
                        break;  
                    case 29:
                        vecRegOffset = CCSR_IIVPR2;
                        break;  
                    case 30:
                        vecRegOffset = CCSR_IIVPR1;
                        break;  
                    case 31:
                        vecRegOffset = CCSR_IIVPR0;
                        break;  
		    }	
		vector = (sysEpicRegRead(vecRegOffset) & EPIC_VECTOR_MSK);
                sysEpicIntHandlerExec (vector);
                }
            }
        }
    }

/*******************************************************************************
*
* epicCurTaskPrioSet - set the priority of the current task.
*
* This routine sets the priority of the Processor Current Task
* Priority register to the value of the prioNum parameter.  This function
* should be called after sysEpicInit() to lower the priority of the processor
* current task. Note that valid priority values are 0 through 15 (15 being
* the highest priority).
*
* RETURNS: previous priority of the task, ERROR if requested priority is
* out of the range supported by the MPC8540.
*/

int epicCurTaskPrioSet
    (
    int prioNum
    )
    {
    ULONG oldPrio;

    if ((prioNum < EPIC_PRIORITY_MIN) || (prioNum > EPIC_PRIORITY_MAX))
        {
        return (ERROR);
	}

    oldPrio = sysEpicRegRead (CCSR_GLBL_CTPR);

    sysEpicRegWrite (CCSR_GLBL_CTPR, prioNum);

    return (oldPrio);
    }

/*******************************************************************************
*
* epicIntEnable - enable an EPIC interrupt 
*
* This function clears the mask bit of an interrupt source, given its
* vector/priority register offset.
*
* <regOffset> is the address offset of the Vector Priority register  
* 
* RETURNS: OK
*/

int epicIntEnable
    (
    int vector	
    )
    {
    ULONG 	srcVal;
    int		regOffset;

    regOffset = epicGetVecRegAdrs(vector);
    srcVal = sysEpicRegRead (regOffset);
    srcVal &= ~(EPIC_INTR_EN);		/* clear the mask bit */

    sysEpicRegWrite (regOffset, srcVal);

    return OK;
    }

/*******************************************************************************
*
* epicIntDisable - disable an EPIC interrupt
*
* This function sets the mask bit of an interrupt source given its
* vector/priority register offset. 
*
* RETURNS: OK 
*/
               
int epicIntDisable
    (
    int vector	
    )
    {
    ULONG 	srcVal;
    int		regOffset;

    regOffset = epicGetVecRegAdrs(vector);
    srcVal = sysEpicRegRead (regOffset);
    srcVal |= EPIC_INTR_EN;		/* set the mask bit */

    sysEpicRegWrite (regOffset, srcVal);

    return OK;
    }

/*******************************************************************************
*
* epicIntAck - read the IACK register and return vector
* 
* This routine reads the Interrupt acknowledge register and returns
* the vector number of the highest pending interrupt.
*
* RETURNS: vector number of the highest priority pending interrupt
*/

int epicIntAck (void)
    {
    int eumbVal;

    eumbVal = sysEpicRegRead (CCSR_GLBL_IACK);
    WRS_ASM ("eieio");
    return eumbVal;
    }

/*******************************************************************************
*
* epicEOI - signal end of interrupt 
*
* This routine signals end of interrupt by writing 0x0 to the EOI register.
*
* RETURNS: N/A
*/

void epicEOI(void)
    {
    sysEpicRegWrite (CCSR_GLBL_EOI, 0x0);
    }

/*******************************************************************************
*
* epicGetVecRegAdrs - translate a vector to vector reg address
*
* This routine uses the INTERRUPT_CONFIG_TABLE in config.h to determine
* the register offset associated with a given vector.
*
* RETURNS: the vector register address of the corresponding vector, ERROR
* if the vector cannot be found.
*/

ULONG epicGetVecRegAdrs
    (
    int vector
    )
    {
    int index = 0;

    while (intConfig[index][0] != 0xFFFFFFFF)
	{
	if (intConfig[index][2] == (UINT32)vector)
	    return (intConfig[index][0]);
	index ++;
	}
    return ((ULONG) ERROR);	/* cannot find the specified vector */
    }

/*******************************************************************************
*
* sysEpicIntConfig - configure the vector/priority registers
*
* This routine uses the INTERRUPT_CONFIG_TABLE table defined in config.h 
* to configure the vector/priority registers for all of the interrupt 
* sources.  This routine does not enable the interrupts by clearing the 
* mask bit.
*
* RETURNS: N/A
*/

LOCAL void sysEpicIntConfig (void)
    {
    UINT32 oldVal = 0x0;
    UINT32 newVal = 0x0;
    int index = 0;

    while (intConfig[index][0] != 0xFFFFFFFF)
	{
	oldVal = sysEpicRegRead(intConfig[index][0]);

	/* 
	 * Make sure interrupt is not active which it should
	 * not be at this time.
	 * The interrupt is configured using the configuration
	 * values in config.h (INTERRUPT_CONFIG_TABLE).
	 */

	if ((oldVal & EPIC_INTR_ACTIVE) != EPIC_INTR_ACTIVE)
	    {
	    newVal = (EPIC_INTR_EN | intConfig[index][1] |
		      intConfig[index][2]);
	    sysEpicRegWrite(intConfig[index][0], newVal);
	    }

	/* critical interrupts */

	if (intConfig[index][3] != 0xFFFFFFFF)
	    {
	    sysEpicRegWrite(intConfig[index][0]+0x10, intConfig[index][3]);
	    }
	index ++;	
	}
    }

/*******************************************************************************
*
* sysEpicRegRead - read a register from the EPIC address space
*
* This function reads a register from the EPIC address space. 
*
* RETURNS:  the 32 bit little endian value of the register.
*/

UINT32 sysEpicRegRead
    (
    ULONG regNum
    )
    {
    UINT32 temp;

    WRS_ASM("eieio;sync");
    temp = CCSR_READ32(CCSBAR,regNum);
    return (temp);
    }

/*******************************************************************************
*
* sysEpicRegWrite -  write a register to the EPIC address space
*
* This function wrties a register to the EPIC address space.
*
* RETURNS: N/A
*/

void sysEpicRegWrite
    (
    ULONG regNum,
    UINT32 regVal
    )
    {
    CCSR_WRITE32(CCSBAR, regNum, regVal);
    WRS_ASM("eieio;sync");
    return ;
    }

#ifdef INCLUDE_SHOW_ROUTINES
/******************************************************************************
*
* intConnectShow - Show all intConnect()ions
*
* This routine scans the 256-entry sysIntTbl[] vector table.  For each
* vector which has been intConnect()ed, a line is displayed showing the
* vector number and handler(arg) associated with the previously executed
* intConnect().  If there is an interrupt chain then each item is the
* chain is shown on that line as well.
*
* RETURNS: NA
*/

void intConnectShow (void)
    {
    IMPORT INT_HANDLER_DESC * sysIntTbl [256];
    INT_HANDLER_DESC * handler;
    int i;

    printf ("\n   Vector :  Handler  (Arg)\n\n");
    for (i=0; i<=255; i++)
        {
        if ((handler = sysIntTbl[i]) != NULL)
            {
            printf ("%3d (0x%02x):  0x%x (0x%x)\n",
                     i,i, (UINT32)handler->vec, handler->arg);
            handler = handler->next;
            while (handler != NULL)
                {
                printf("             0x%x (0x%x) - chained\n",
                        (UINT32)handler->vec, handler->arg);
                handler = handler->next;
                }
            }
        }
    printf("\n");
    }
#endif /* INCLUDE_SHOW_ROUTINES */
