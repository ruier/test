/* sysTsi108Mpic.c - Tundra TSI108 Interrupt Controller driver */

/*
 * Copyright (c) 2007-2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 2005 Tundra Semiconductor Corp. */

/*
modification history
--------------------
01e,17oct08,x_f  add PCI reset before initializing interrupt sources
01d,10jul07,y_w  add support for Hackberry EVB
01c,07dec05,avb  clean up to meet vxWorks coding style requirements
01b,26sep05,avb  changed to fix handling of nested interrupts
01a,02aug05,avb  written (from MPC107 EPIC code).
*/

/*
DESCRIPTION
This module implements the TSI108 MPIC interrupt controller driver.

INCLUDES: sysTsi108Mpic.h
*/


/* includes */

#include "vxWorks.h"
#include "config.h"
#include "sysTsi108Mpic.h"

#ifdef INCLUDE_WINDVIEW
#   include "private/eventP.h"
#endif /* INCLUDE_WINDVIEW */

/* defines */

#define INT_TABLE_SIZE	256

#ifndef CPU_INT_LOCK
#define CPU_INT_LOCK(x) (*x = intLock ())
#endif

#ifndef CPU_INT_UNLOCK
#define CPU_INT_UNLOCK(data) (intUnlock (data))
#endif

#define	TSI108_MPIC_DEBUG

#ifdef  TSI108_MPIC_DEBUG
#define TSI108_MPIC_DBG_OFF	0x0000
#define TSI108_MPIC_DBG_INIT	0x0001
#define TSI108_MPIC_DBG_INT	0x0002
#define TSI108_MPIC_DBG_ERR	0x0004

int tsi108MpicDbg =  TSI108_MPIC_DBG_ERR;

#define TSI108_MPIC_LOG(FLG, X0, X1, X2, X3, X4, X5, X6) \
    do  {					    \
	if (tsi108MpicDbg & FLG)		    \
	    logMsg (X0, X1, X2, X3, X4, X5, X6);    \
	} while ((0))

#else
#define TSI108_MPIC_LOG(FLG, X0, X1, X2, X3, X4, X5, X6)
#endif

/* globals */

IMPORT STATUS	(*_func_intConnectRtn) (VOIDFUNCPTR *, VOIDFUNCPTR, int);
IMPORT int	(*_func_intEnableRtn) (int);
IMPORT int	(*_func_intDisableRtn) (int);
IMPORT STATUS	excIntConnect (VOIDFUNCPTR *, VOIDFUNCPTR);
void		sysTsi108MpicIntHandler (void);

INT_HANDLER_DESC * sysIntTbl [INT_TABLE_SIZE]; /* system interrupt table */
int spurIntCnt;

/* locals */

LOCAL UINT32 outPinNum = 0; /* MPIC output pin delivering interrupt to CPU */
/*
 * The table below defines external and internal IRQ senses and polarities.
 */
LOCAL UINT32 tsi108IntSenses[] =
    {
    /*
     * First four entries of the table will be updated
     * by the MPIC initialization routine based on actual board design
     */
    (TSI108_IRQ_SENSE_LEVEL | TSI108_IRQ_ACTIVE_HIGH),  /* INT[0] */ 
    (TSI108_IRQ_SENSE_LEVEL | TSI108_IRQ_ACTIVE_HIGH),  /* INT[1] */
    (TSI108_IRQ_SENSE_LEVEL | TSI108_IRQ_ACTIVE_HIGH),  /* INT[2] */
    (TSI108_IRQ_SENSE_LEVEL | TSI108_IRQ_ACTIVE_HIGH),  /* INT[3] */

    /* All interrupt sources below are internal for Tsi108 */
    (TSI108_IRQ_SENSE_EDGE  | TSI108_IRQ_ACTIVE_HIGH),  /* Reserved */
    (TSI108_IRQ_SENSE_EDGE  | TSI108_IRQ_ACTIVE_HIGH),  /* Reserved */
    (TSI108_IRQ_SENSE_EDGE  | TSI108_IRQ_ACTIVE_HIGH),  /* Reserved */
    (TSI108_IRQ_SENSE_EDGE  | TSI108_IRQ_ACTIVE_HIGH),  /* Reserved */

    (TSI108_IRQ_SENSE_EDGE  | TSI108_IRQ_ACTIVE_HIGH),  /* DMA ch 0 */ 
    (TSI108_IRQ_SENSE_EDGE  | TSI108_IRQ_ACTIVE_HIGH),  /* DMA ch 1 */ 
    (TSI108_IRQ_SENSE_EDGE  | TSI108_IRQ_ACTIVE_HIGH),  /* DMA ch 2 */ 
    (TSI108_IRQ_SENSE_EDGE  | TSI108_IRQ_ACTIVE_HIGH),  /* DMA ch 3 */ 
    (TSI108_IRQ_SENSE_LEVEL | TSI108_IRQ_ACTIVE_HIGH),  /* UART0 */
    (TSI108_IRQ_SENSE_LEVEL | TSI108_IRQ_ACTIVE_HIGH),  /* UART1 */
    (TSI108_IRQ_SENSE_EDGE  | TSI108_IRQ_ACTIVE_HIGH),  /* I2C */
    (TSI108_IRQ_SENSE_EDGE  | TSI108_IRQ_ACTIVE_HIGH),  /* GPIO */

    (TSI108_IRQ_SENSE_LEVEL | TSI108_IRQ_ACTIVE_HIGH),  /* GIGE0 */
    (TSI108_IRQ_SENSE_LEVEL | TSI108_IRQ_ACTIVE_HIGH),  /* GIGE1 */
    (TSI108_IRQ_SENSE_EDGE  | TSI108_IRQ_ACTIVE_HIGH),  /* Reserved */
    (TSI108_IRQ_SENSE_EDGE  | TSI108_IRQ_ACTIVE_HIGH),  /* HLP */
    (TSI108_IRQ_SENSE_EDGE  | TSI108_IRQ_ACTIVE_HIGH),  /* SDRAM */
    (TSI108_IRQ_SENSE_EDGE  | TSI108_IRQ_ACTIVE_HIGH),  /* PI1 */
    (TSI108_IRQ_SENSE_EDGE  | TSI108_IRQ_ACTIVE_HIGH),  /* PI2 */
    (TSI108_IRQ_SENSE_LEVEL | TSI108_IRQ_ACTIVE_HIGH)   /* PCI block */
    };

/* forward declarations */

LOCAL STATUS sysTsi108MpicIntConnect (VOIDFUNCPTR * vector,
					VOIDFUNCPTR routine, int parameter);
LOCAL int    sysTsi108MpicIntUnmask(int);
LOCAL int    sysTsi108PciIntUnmask(int);
LOCAL int    sysTsi108MpicIntMask(int);
LOCAL int    sysTsi108PciIntMask(int);
LOCAL UINT32 mpicGetVectorOffset(UINT irq);
LOCAL int    mpicGetPciSource(void);


/*******************************************************************************
*
* sysTsi108MpicInit - initialize the MPIC in the TSI108	host bridge
*
* This function initializes the Multiprocessor Programmable 
* Interrupt Controller (MPIC) which is part of the TSI108 host bridge device.
*
* It first initializes the system vector table, connects the MPIC interrupt
* handler to the PPC external interrupt and attaches the local MPIC routines
* for interrupt connecting, enabling and disabling to the corresponding system
* routine pointers.
*
* It then initializes the MPIC registers and clears any pending MPIC interrupts.
*
* RETURNS: OK always
*
*/

STATUS sysTsi108MpicInit 
    (
    UINT32  cpuNum,	    /* MPIC output pin to map all interrupts (0 -3) */
    UINT32* pExtIntSenseMap /* Sensitivity map for four external interrupts */
    )
    {
    int	i;
    UINT32 temp;

    if (cpuNum > 3)
	return ERROR;

    outPinNum  = cpuNum; /* save MPIC output pin number defined on the board */

    /* Initialize the interrupt table */

    for (i = 0; i < INT_TABLE_SIZE; i++)
        {
        sysIntTbl[i] = NULL;
        }
 
    /* Connect the interrupt handler to the PowerPC external interrupt */

    excIntConnect ((VOIDFUNCPTR *) _EXC_OFF_INTR, sysTsi108MpicIntHandler);

    /*
     *  Set up the BSP specific routines
     *  Attach the local routines to the vxWorks system calls
     */

    _func_intConnectRtn = sysTsi108MpicIntConnect;
    _func_intEnableRtn  = sysTsi108MpicIntUnmask;
    _func_intDisableRtn = sysTsi108MpicIntMask;

    /* Reset MPIC */
	
    TSI108_REG_WR(BL_MPIC, MPIC_INT_GCR, MPIC_INT_GCR_R);
	
    /* Wait for reset completion */
	
    while(TSI108_REG_RD(BL_MPIC, MPIC_INT_GCR) & MPIC_INT_GCR_R)
        EIEIO_SYNC;

    /* Reset PCI block */

    temp = TSI108_REG_RD(BL_PCI, PCI_MISC_CSR);
    temp &= ~PCI_BLOCK_SOFT_RESET;
    TSI108_REG_WR(BL_PCI, PCI_MISC_CSR, temp);

    /* Wait for reset completion */

    while(TSI108_REG_RD(BL_PCI, PCI_MISC_CSR) & PCI_BLOCK_SOFT_RESET)
        EIEIO_SYNC;

    /*---------------------------------------------------------------------
     * Initialize all interrupt sources
     */

    /* Initialize timer interrupts */
    for (i = 0; i < MAX_MPIC_GT; i++)
	{
	/* Disabled, Priority 0 */
	temp = MPIC_INT_GTVPR_M |
		(DEFAULT_PRIO_LVL << 16) | (INT_VEC_TIMER0 + i);
	TSI108_REG_WR(BL_MPIC, MPIC_INT_GTVPR(i), temp);

	/* Not mapped to any processor at this time */
	TSI108_REG_WR(BL_MPIC, MPIC_INT_GTDR0(i), 0);
	}

    /*
     * Initialize interrupt sources
     */

     /* Initialize external interrupt sources (pins INT[0] - INT[3] */

    for (i = 0; i < 4; i++)
	{
	/* Save sensitivity configuration for an external source */
	tsi108IntSenses[i] = pExtIntSenseMap[i];
	temp = tsi108IntSenses[i] << 24;
	temp |= MPIC_INT_IVPR_M |
		(DEFAULT_PRIO_LVL << 16) | (TSI108_IRQ_BASE + i);
	TSI108_REG_WR(BL_MPIC, MPIC_INT_IVPR(i), temp);

	/* Map to specified output (CPU) */
	TSI108_REG_WR(BL_MPIC, MPIC_INT_IDR(i), (1 << cpuNum));
	}

    /* Remaining interrupt sources have predefined input sensitivity */

    for (; i < sizeof(tsi108IntSenses)/sizeof(UINT32); i++)
	{
	temp = tsi108IntSenses[i] << 24;
	temp |= MPIC_INT_IVPR_M |
		(DEFAULT_PRIO_LVL << 16) | (TSI108_IRQ_BASE + i);
	TSI108_REG_WR(BL_MPIC, MPIC_INT_IVPR(i), temp);

	/* Map to specified output (CPU) */
	TSI108_REG_WR(BL_MPIC, MPIC_INT_IDR(i), (1 << cpuNum));
	}

    /* Initialize the spurious interrupt */
    TSI108_REG_WR(BL_MPIC, MPIC_INT_SVR, INT_VEC_SPURIOUS);

    /* Set the current processor priority level */
    TSI108_REG_WR(BL_MPIC, MPIC_INT_TASKP(cpuNum), 0x00);

    /* Init interrupt sources from the PCI block */
    TSI108_REG_WR(BL_PCI, PCI_IRP_CFG_CTL, 0x0000ff00);
    TSI108_REG_WR(BL_PCI, PCI_IRP_ENABLE, 0x00400000);

    /* Configure MPIC outputs to CPU0 */
    TSI108_REG_WR(BL_MPIC, MPIC_INT_CSR(0),
		    (TSI108_IRQ_SENSE_EDGE | TSI108_IRQ_ACTIVE_LOW));
    TSI108_REG_WR(BL_MPIC, MPIC_INT_CSR(1),
		    (TSI108_IRQ_SENSE_EDGE | TSI108_IRQ_ACTIVE_LOW));
    TSI108_REG_WR(BL_MPIC, MPIC_INT_CSR(2),
		    (TSI108_IRQ_SENSE_EDGE | TSI108_IRQ_ACTIVE_LOW));
    TSI108_REG_WR(BL_MPIC, MPIC_INT_CSR(3),
		    (TSI108_IRQ_SENSE_EDGE | TSI108_IRQ_ACTIVE_LOW));

    /* Clear out all possible pending interrupts */
    while (
	(TSI108_REG_RD(BL_MPIC,MPIC_INT_VECTOR(cpuNum)) & VECTOR_MASK) != 
							INT_VEC_SPURIOUS
	)
        {
	TSI108_REG_WR(BL_MPIC, MPIC_INT_EOI(cpuNum), 0);
        }
    
    TSI108_REG_WR(BL_MPIC, MPIC_INT_EOI(cpuNum), 0);
    return OK;
    }


/*******************************************************************************
*
* sysTsi108MpicIntConnect - connect an interrupt handler to the system
*			    vector table
*
* This function connects an interrupt handler to the system vector table.
*
* RETURNS: OK/ERROR.
*
*/
 
LOCAL STATUS sysTsi108MpicIntConnect
    (
     VOIDFUNCPTR *      vector,         /* interrupt vector to attach */
     VOIDFUNCPTR        routine,        /* routine to be called */
     int                parameter       /* parameter to be passed to routine */
    )
    {
    INT_HANDLER_DESC*	newHandler;
    INT_HANDLER_DESC*	currHandler;
    int			intVal;

    if (((int)vector < 0) || ((int)vector > 0xff))      /* Out of Range? */
        return (ERROR);
 
    /* create a new interrupt handler */

    if ((newHandler = (INT_HANDLER_DESC *)calloc (1, sizeof (INT_HANDLER_DESC)))
	 == NULL)
        return (ERROR);
 
    /* initialize the new handler */

    newHandler->vec  = routine;
    newHandler->arg  = parameter;
    newHandler->next = NULL;

    /* install the handler in the system interrupt table */

    CPU_INT_LOCK(&intVal); /* lock interrupts to prevent races */

    if (sysIntTbl[(int) vector] == NULL)
        sysIntTbl [(int ) vector] = newHandler; /* single int. handler case */
    else
        {
        currHandler = sysIntTbl[(int) vector]; /* multiple int. handler case */
        while (currHandler->next != NULL)
            {
            currHandler = currHandler->next;
            }
        currHandler->next = newHandler;
        }

    CPU_INT_UNLOCK(intVal);

    return (OK);
    }


/*******************************************************************************
*
* sysTsi108MpicIntUnmask - enable (unmask) an MPIC interrupt source
*
* This routine enables a specified MPIC interrupt source.
*
* RETURNS: OK or ERROR if interrupt souce not supported
*
*/
 
LOCAL int sysTsi108MpicIntUnmask
    (
    int intLevel        /* interrupt level to enable */
    )
    {
    UINT32 offset;
    UINT32 reg_val;


    if ((intLevel >= IRQ_PCI_INTAD_BASE) && (intLevel <= IRQ_PCI_INTD))
	return (sysTsi108PciIntUnmask(intLevel)); /* enable PCI sources */

    /*
     * Ensure that the int. level is a valid IRQ 
     * If not supported, just return with ERROR.
     */

    offset = mpicGetVectorOffset(intLevel);

    if (offset != 0)
	{

	/* Clear the interrupt mask bit */
	reg_val = TSI108_REG_RD(BL_MPIC, offset);

	/*
	 * Restore level sensing for level signalled service request
	 * at corresponding MPIC input (opposite to disable function).
	 * See design note in UM on MPIC "Input Edge/Level Sensitivity" 
	 */

	if (
	    (intLevel < INT_VEC_PCI) &&
	    (tsi108IntSenses[intLevel] & TSI108_IRQ_SENSE_LEVEL)
	    )
	    reg_val |= (1 << 25); /* restore level sense */

	TSI108_REG_WR(BL_MPIC, offset, reg_val & ~MPIC_INT_IVPR_M);
	return (OK);
	}
    else    /* Undefined interrupt number */
	{
        TSI108_MPIC_LOG(TSI108_MPIC_DBG_ERR,
		        ("MPIC IntUnmask: Undefineded interrupt number %d\r\n"),
			intLevel, 2,3,4,5,6);
	return (ERROR);
	}
    }


/*******************************************************************************
*
* sysTsi108PciIntUnmask - enable (unmask) an PCI interrupt request line
*
* This routine enables a specified PCI interrupt  request line in PCI block.
*
* RETURNS: OK or ERROR if interrupt souce not supported
*
*/
 
LOCAL int sysTsi108PciIntUnmask
    (
    int intLevel        /* interrupt level to enable */
    )
    {
    UINT32 offset;
    UINT32 reg_val;

    /*
     * Ensure that the int. level is a valid IRQ 
     * If not supported, just return with ERROR.
     */

    if ((intLevel < IRQ_PCI_INTAD_BASE) || (intLevel > IRQ_PCI_INTD))
	return (ERROR);
    else    /* PCI INTA#-INTD# IRQ */
	{
	offset = (intLevel - IRQ_PCI_INTAD_BASE);
	reg_val = TSI108_REG_RD(BL_PCI, PCI_IRP_CFG_CTL);
	reg_val &= ~(1 << offset);
	reg_val |= (3 << (8 + (offset * 2)));
	TSI108_REG_WR(BL_PCI, PCI_IRP_CFG_CTL, reg_val);
	return (OK);
	}
    }


/*******************************************************************************
*
* sysTsi108MpicIntMask - mask (disable) an Mpic interrupt source
*
* This routine disables a specified Mpic interrupt source.
*
* RETURNS: OK or ERROR if interrupt level not supported
*
*/
 
LOCAL int sysTsi108MpicIntMask
    (
    int intLevel        /* interrupt level to disable */
    )
    {
    UINT32 offset;
    UINT32 reg_val;

    if ((intLevel >= IRQ_PCI_INTAD_BASE) && (intLevel <= IRQ_PCI_INTD))
	return (sysTsi108PciIntMask(intLevel)); /* disable PCI sources */

    /*
     * Ensure that the int. level is a valid IRQ 
     * If not supported, just return with ERROR.
     */

    offset = mpicGetVectorOffset(intLevel);

    if (offset != 0)
	{
	/* Set the interrupt mask bit */
	reg_val = TSI108_REG_RD(BL_MPIC, offset);

	/*
	 * Switch from level to edge sensing mode to remove
	 * service request at corresponding MPIC input.
	 * See design note in UM on MPIC "Input Edge/Level Sensitivity" 
	 */

	if (
	    (intLevel < INT_VEC_PCI) &&
	    (tsi108IntSenses[intLevel] & TSI108_IRQ_SENSE_LEVEL)
	    )
	    reg_val &= ~(1 << 25); /* clear level sense */

	TSI108_REG_WR(BL_MPIC, offset, reg_val | MPIC_INT_IVPR_M);
	WRS_ASM ("sync");
	/* read back to ensure that register is set before continue */
	reg_val = TSI108_REG_RD(BL_MPIC, offset);

	return (OK);
	}
    else    /* invalid IRQ */
	{
        TSI108_MPIC_LOG(TSI108_MPIC_DBG_ERR,
			("MPIC IntMask : Undefineded interrupt number %d\r\n"),
			intLevel, 2,3,4,5,6);
	return (ERROR);
	}
    }


/*******************************************************************************
*
* sysTsi108PciIntMask - mask (disable) a PCI interrupt request line
*                       (in PCI block)
*
* This routine disables a PCI interrupt line in the PCI block.
*
* RETURNS: OK or ERROR if interrupt level not supported
*
*/
 
LOCAL int sysTsi108PciIntMask
    (
    int intLevel        /* interrupt level to disable */
    )
    {
    UINT32 offset;
    UINT32 reg_val;

    /*
     * Ensure that the int. level is a valid IRQ 
     * If not supported, just return with ERROR.
     */

    if ((intLevel < IRQ_PCI_INTAD_BASE) || (intLevel > IRQ_PCI_INTD))
	return (ERROR);
    else    /* PCI INTA#-INTD# IRQ */
	{
	offset = (intLevel - IRQ_PCI_INTAD_BASE);
	reg_val = TSI108_REG_RD(BL_PCI, PCI_IRP_CFG_CTL);
	reg_val |= (1 << offset); /* INTx_DIR = output */
	reg_val &= ~(3 << (8 + (offset * 2))); /* INTx_TYPE = unused */
	TSI108_REG_WR(BL_PCI, PCI_IRP_CFG_CTL, reg_val);
	WRS_ASM ("sync");
	/* read back to ensure that register is set before continue */
	reg_val = TSI108_REG_RD(BL_PCI, PCI_IRP_CFG_CTL);
	return (OK);
	}
    }


/*******************************************************************************
*
* sysTsi108MpicIntHandler - handles an interrupt requests received 
* by the MPIC.
* 
* This routine processes interrupts received from MPIC interrupt sources.
*
* RETURNS: N/A
*
*/

void sysTsi108MpicIntHandler 
    (
    void
    )
    { 
    int                dontCare; 
    INT_HANDLER_DESC * currHandler;
    UINT32	       vecNum;
    int	irq;		

    /* Get the vector from the MPIC INT_VECTOR register */
    vecNum = TSI108_REG_RD(BL_MPIC, MPIC_INT_VECTOR(outPinNum));
    EIEIO;
    vecNum &= VECTOR_MASK;

#ifdef INCLUDE_WINDVIEW
    WV_EVT_INT_ENT (vecNum);
#endif /* INCLUDE_WINDVIEW */

    /* Ignore spurious interrupts */
    
    if (vecNum == INT_VEC_SPURIOUS) spurIntCnt++ ;

    while (vecNum != INT_VEC_SPURIOUS)
        {

	if (vecNum == INT_VEC_PCI)
	    {
	    /* Individual PCI INTA# - INTD# lines require 
	     * additional demultiplexing.
	     */
	    irq = mpicGetPciSource();

	    /* Disable interrupts from PCI INTx# line */
	    sysTsi108PciIntMask(irq);
	    }
	else
	    {
	    irq = vecNum;
	    }

        /* issue an end-of-interrupt to the MPIC for non-level interrupts */
	if ((tsi108IntSenses[vecNum] & TSI108_IRQ_SENSE_LEVEL) == 0)
	    {
	    TSI108_REG_WR(BL_MPIC, MPIC_INT_EOI(outPinNum), 0);
	    }

        /*
         * Allow maskable interrupts to the CPU.
         */

        CPU_INT_UNLOCK(_PPC_MSR_EE);

        /* call the necessary interrupt handlers */

        if ((currHandler = sysIntTbl [irq]) == NULL)
            {
            TSI108_MPIC_LOG(TSI108_MPIC_DBG_ERR,
			    ("Uninitialized TSI108 MPIC interrupt %d\r\n"),
			    irq, 2,3,4,5,6);
            }
        else
            {
            /* Call EACH respective chained interrupt handler */

            while (currHandler != NULL)
                {
                currHandler->vec (currHandler->arg);
                currHandler = currHandler->next;
                }
            }

	/* 
	 * Disable CPU External Interrupts
	 * External Interrupts will be re-enabled in the kernel's wrapper
	 * of this Interrupt.
	 */
	
        CPU_INT_LOCK (&dontCare);
        WRS_ASM ("sync");

	if (vecNum == INT_VEC_PCI)
	    {
	    sysTsi108PciIntUnmask(irq);
	    }

        /* issue an end-of-interrupt to the MPIC for level interrupts */
	if ((tsi108IntSenses[vecNum] & TSI108_IRQ_SENSE_LEVEL) != 0)
	    {
	    TSI108_REG_WR(BL_MPIC, MPIC_INT_EOI(outPinNum), 0);
	    }

        /* get the next vector from the MPIC IACK reg. */
       
	vecNum = TSI108_REG_RD(BL_MPIC, MPIC_INT_VECTOR(outPinNum));
	EIEIO;
	vecNum &= VECTOR_MASK;
 
        }

    /*
     * Issue an end-of-interrupt to the MPIC.
     * Tsi108 requires EOI signalled after spurious interrupt vector read. 
     */
    TSI108_REG_WR(BL_MPIC, MPIC_INT_EOI(outPinNum), 0);

    return;
    }


/*******************************************************************************
*
* mpicGetVectorOffset - helper routine used by other MPIC routines
* 
* This routine will return interrupt vector register offset for specified
* interrupt vector number.
*
* RETURNS: non-zero - offset for matching VPR register, 0 - if error.
*
*/

LOCAL UINT32 mpicGetVectorOffset(UINT irq)
    {
    if ((irq < (TSI108_IRQ_BASE)) || (irq >= (IRQ_PCI_INTAD_BASE)))
	    return 0;

	if (irq < INT_VEC_MBOX0)
	    return (MPIC_INT_IVPR(irq - TSI108_IRQ_BASE));
	else if (irq < INT_VEC_DBELL0) 
	    return (MPIC_INT_MBVPR(irq - INT_VEC_MBOX0));
	else if (irq < INT_VEC_TIMER0)
	    return (MPIC_INT_DVPR(irq - INT_VEC_DBELL0));
	else
	    return (MPIC_INT_GTVPR(irq - INT_VEC_TIMER0));
    }


/*******************************************************************************
*
* mpicGetPciSource - PCI interrupt request mapping routine
* 
* This routine remaps PCI interrupt request on INTA# - INTD# lines into
* corresponding interrupt request number assigned to each line.
* This routine implements simple round-robin mechanism to keep
* PCI interrupt request lines from starvation. 
*
* RETURNS: non-zero - IRQ number, (-1) - if error.
*
*/

LOCAL int mpicGetPciSource(void)
    {
    UINT32 temp = 0;
    int irq = -1;
    int i;
    UINT32 pci_irp_stat;
    static int mask = 0;

    /* Read PCI/X block interrupt status register */
    pci_irp_stat = TSI108_REG_RD(BL_PCI, PCI_IRP_STAT);

    if (pci_irp_stat & PCI_IRP_STAT_P_INT)
	{
	/* Process Interrupt from PCI bus INTA# - INTD# lines */
	temp = TSI108_REG_RD(BL_PCI, PCI_IRP_INTAD) & 0xf;
	for (i = 0; i < 4; i++, mask++)
	    {
	    if (temp & (1 << mask%4))
		{
		irq = IRQ_PCI_INTA + mask%4;
		mask++;
		break;
		}
	    }
	}
    else
	{
#ifdef DBG_TSI108_INTERRUPT
	TSI108_MPIC_LOG(TSI108_MPIC_DBG_ERR,
			("TSI108_PIC: error in TSI108_PCI_IRP_STAT\r\n"),
			0,0,0,0,0,0);
	pci_irp_stat = TSI108_REG_RD(BL_PCI, PCI_IRP_STAT);
	temp = TSI108_REG_RD(BL_PCI, PCI_IRP_INTAD);
	TSI108_MPIC_LOG(TSI108_MPIC_DBG_ERR,
			(">> stat=0x%08x intad=0x%08x "),
			pci_irp_stat,temp,0,0,0,0);
	temp = TSI108_REG_RD(BL_PCI, PCI_IRP_CFG_CTL);
	TSI108_MPIC_LOG(TSI108_MPIC_DBG_ERR, ("cfg_ctl=0x%08x "),
			temp,0,0,0,0,0);
	temp = TSI108_REG_RD(BL_PCI, PCI_IRP_ENABLE);
	TSI108_MPIC_LOG(TSI108_MPIC_DBG_ERR, ("irp_enable=0x%08x\r\n"),
			temp,0,0,0,0,0);
#endif /* DBG_TSI108_INTERRUPT */
	}

    return irq;
    }
