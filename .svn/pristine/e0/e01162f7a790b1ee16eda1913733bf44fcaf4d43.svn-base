/* sysTsi108Mpic.h - Tundra TSI108 MPIC definitions */

/*
 * Copyright (c) 2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 2005 Tundra Semiconductor Corp. */

/*
modification history
--------------------
01c,12nov07,y_w  Clear out apigen errors/warnings
01b,06dec05,avb  clean up to meet vxWorks coding style requirements
01a,02aug05,avb  written based on Tsi108 specification
*/

/*
Description:

Contains definitions for the Tundra Tsi108 MPIC
*/


#ifndef	__INCsysTsi108Mpich
#define	__INCsysTsi108Mpich

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

/* structures */
#ifndef	_ASMLANGUAGE

typedef struct intHandlerDesc	    /* interrupt handler desciption */
    {
    VOIDFUNCPTR		    vec;    /* interrupt vector */
    int			    arg;    /* interrupt handler argument */
    struct intHandlerDesc*  next;   /* next interrupt handler & argument */
    } INT_HANDLER_DESC;

#endif	/* _ASMLANGUAGE */

/* macros */
#define VECTOR_MASK		0x000000ff

/*
 * Tsi108 interrupts
 */
#ifndef TSI108_IRQ_BASE
#define TSI108_IRQ_BASE 	0
#endif

#define TSI108_IRQ(x)		(TSI108_IRQ_BASE + (x))

#define TSI108_MAX_VECTORS	(36 + 4) /* 36 sources + PCI INTAD demux */
#define MAX_TASK_PRIO		0xF

#define DEFAULT_PRIO_LVL	10 /* default priority level for all sources */

/*
 * Interrupt vectors assignment to external and internal 
 * sources of requests.
 */

#define INT_VEC_SPURIOUS	0xFF 


/* EXTERNAL INTERRUPT SOURCES */

#define INT_VEC_EXT_INT0	TSI108_IRQ(0)	/* Source at INT[0] line */
#define INT_VEC_EXT_INT1	TSI108_IRQ(1)	/* Source at INT[1] line */
#define INT_VEC_EXT_INT2	TSI108_IRQ(2)	/* Source at INT[2] line */
#define INT_VEC_EXT_INT3	TSI108_IRQ(3)	/* Source at INT[3] line */

/* INTERNAL INTERRUPT SOURCES */

#define INT_VEC_RESERVED0	TSI108_IRQ(4)	/* Reserved IRQ */
#define INT_VEC_RESERVED1	TSI108_IRQ(5)	/* Reserved IRQ */
#define INT_VEC_RESERVED2	TSI108_IRQ(6)	/* Reserved IRQ */
#define INT_VEC_RESERVED3	TSI108_IRQ(7)	/* Reserved IRQ */
#define INT_VEC_DMA0		TSI108_IRQ(8)	/* DMA0 */
#define INT_VEC_DMA1		TSI108_IRQ(9)	/* DMA1 */
#define INT_VEC_DMA2		TSI108_IRQ(10)	/* DMA2 */
#define INT_VEC_DMA3		TSI108_IRQ(11)	/* DMA3 */
#define INT_VEC_UART0		TSI108_IRQ(12)	/* UART0 */
#define INT_VEC_UART1		TSI108_IRQ(13)	/* UART1 */
#define INT_VEC_I2C		TSI108_IRQ(14)	/* I2C */
#define INT_VEC_GPIO		TSI108_IRQ(15)	/* GPIO */
#define INT_VEC_GIGE0		TSI108_IRQ(16)	/* GIGE Port 0 */
#define INT_VEC_GIGE1		TSI108_IRQ(17)	/* GIGE Port 1 */
#define INT_VEC_RESERVED4	TSI108_IRQ(18)	/* Reserved IRQ */
#define INT_VEC_HLP		TSI108_IRQ(19)	/* HLP block */
#define INT_VEC_SDRAM		TSI108_IRQ(20)	/* SDC block */
#define INT_VEC_PROC_IF		TSI108_IRQ(21)	/* Processor IF block */
#define INT_VEC_RESERVED5	TSI108_IRQ(22)	/* Reserved IRQ */
#define INT_VEC_PCI		TSI108_IRQ(23)	/* PCI/X block */

#define INT_VEC_MBOX0		TSI108_IRQ(24)	/* Mailbox 0 */
#define INT_VEC_MBOX1		TSI108_IRQ(25)	/* Mailbox 1 */
#define INT_VEC_MBOX2		TSI108_IRQ(26)	/* Mailbox 2 */
#define INT_VEC_MBOX3		TSI108_IRQ(27)	/* Mailbox 3 */

#define INT_VEC_DBELL0		TSI108_IRQ(28)	/* Doorbell 0 */
#define INT_VEC_DBELL1		TSI108_IRQ(29)	/* Doorbell 1 */
#define INT_VEC_DBELL2		TSI108_IRQ(30)	/* Doorbell 2 */
#define INT_VEC_DBELL3		TSI108_IRQ(31)	/* Doorbell 3 */

#define INT_VEC_TIMER0		TSI108_IRQ(32)	/* Global Timer 0 */
#define INT_VEC_TIMER1		TSI108_IRQ(33)	/* Global Timer 1 */
#define INT_VEC_TIMER2		TSI108_IRQ(34)	/* Global Timer 2 */
#define INT_VEC_TIMER3		TSI108_IRQ(35)	/* Global Timer 3 */

/*
 * PCI bus INTA# - INTD# lines demultiplexor
 */
#define IRQ_PCI_INTAD_BASE	TSI108_IRQ(36)
#define IRQ_PCI_INTA		(IRQ_PCI_INTAD_BASE + 0)
#define IRQ_PCI_INTB		(IRQ_PCI_INTAD_BASE + 1)
#define IRQ_PCI_INTC		(IRQ_PCI_INTAD_BASE + 2)
#define IRQ_PCI_INTD		(IRQ_PCI_INTAD_BASE + 3)
#define NUM_PCI_IRQS		(4)

/* number of entries in vector dispatch table */
#define IRQ_TSI108_TAB_SIZE	(TSI108_MAX_VECTORS + 1)

/* Mapping of MPIC outputs to processors' interrupt pins */

#define IDIR_INT_OUT0   0x1
#define IDIR_INT_OUT1   0x2
#define IDIR_INT_OUT2   0x4
#define IDIR_INT_OUT3   0x8

/* IRQ line configuration parameters */

#define TSI108_IRQ_SENSE_EDGE	0   /* Edge Sensitive */
#define TSI108_IRQ_SENSE_LEVEL	2   /* Level Sensitive */

#define TSI108_IRQ_ACTIVE_LOW	0   /* Active Low (level) / Negative Edge */
#define TSI108_IRQ_ACTIVE_HIGH	1   /* Active High (level) / Positive Edge */

/* Interrupt delivery modes */
#define	TSI108_IRQ_DIRECTED	0
#define TSI108_IRQ_DISTRIBUTED	1

/*
 * MPIC Registers (BL_MPIC)
 */

/*
 * Registers offsets from the block base
 */

/* Global interrupt registers */
#define MPIC_INT_FRR		(0x00000000)
#define MPIC_INT_GCR		(0x00000004)
#define MPIC_INT_ICR		(0x00000008)
#define MPIC_INT_MVI		(0x0000000c)
#define MPIC_INT_SVR		(0x00000010)
#define MPIC_INT_TFRR		(0x00000014)
#define MPIC_INT_SOFT_SET	(0x00000020)
#define MPIC_INT_SOFT_ENABLE	(0x00000024)

/* Timer registers */
#define MAX_MPIC_GT		4
#define MPIC_INT_GTCCR(x)	(0x30+0x10*(x))
#define MPIC_INT_GTBCR(x)	(0x34+0x10*(x))
#define MPIC_INT_GTVPR(x)	(0x38+0x10*(x))
#define MPIC_INT_GTDR0(x)	(0x3c+0x10*(x))

/* Interrupt controller input pins */
#define MAX_MPIC_INT_INPUTS	24
#define MPIC_INT_IVPR(x)	(0x100 + 0x8*(x))
#define MPIC_INT_IDR(x)		(0x104 + 0x8*(x))

/* Doorbell registers */
#define MPIC_INT_DAR		(0x00000200)

#define MAX_MPIC_DOORS		4
#define MPIC_INT_DVPR(x)	(0x204 + 0xc*(x))
#define MPIC_INT_DDR(x)		(0x208 + 0xc*(x))
#define MPIC_INT_DMR(x)		(0x20c + 0xc*(x))

/* Mailbox registers */
#define MAX_MPX_MAILBOXES	4
#define MPIC_INT_MBR(x)	(0x280 + 0x10*(x))
#define MPIC_INT_MBVPR(x)	(0x284 + 0x10*(x))
#define MPIC_INT_MBDR(x)	(0x288 + 0x10*(x))

/* Interrupt controller output pins */
#define MAX_MPIC_INT_OUTPUTS	4

#define MPIC_INT_TASKP(x)	(0x300+ 0x40*(x))
#define MPIC_INT_VECTOR(x)	(0x304+ 0x40*(x))
#define MPIC_INT_EOI(x)		(0x308+ 0x40*(x))
#define MPIC_INT_CSR(x)		(0x30c+ 0x40*(x))

/*
 * Register bits and fields
 */

#define MPIC_INT_FRR_VID	(0x000000ff)
#define MPIC_INT_FRR_NCPU	(0x00001f00)
#define MPIC_INT_FRR_NITM	(0x0000e000)
#define MPIC_INT_FRR_NIRQ	(0x07ff0000)
#define MPIC_INT_FRR_NIDOOR	(0xe0000000)

#define MPIC_INT_GCR_R		(0x80000000)

#define MPIC_INT_ICR_R		(0x0000000f)

#define MPIC_INT_MVI_VID	(0x000000ff)
#define MPIC_INT_MVI_DID	(0x0000ff00)
#define MPIC_INT_MVI_STEP	(0x00ff0000)

#define MPIC_INT_SVR_VECTOR	(0x000000ff)

#define MPIC_INT_SOFT_SET_S	(0x00ffffff)
#define MPIC_INT_SOFT_ENABLE_EN		(0x00ffffff)

#define MPIC_INT_GTCCR_COUNT	(0x7fffffff)
#define MPIC_INT_GTCCR_T	(0x80000000)

#define MPIC_INT_GTBCR_B_COUNT	(0x7fffffff)
#define MPIC_INT_GTBCR_CI	(0x80000000)

#define MPIC_INT_GTVPR_VECTOR		(0x000000ff)
#define MPIC_INT_GTVPR_PRIORITY		(0x000f0000)
#define MPIC_INT_GTVPR_PRESCALE		(0x00f00000)
#define MPIC_INT_GTVPR_A		(0x40000000)
#define MPIC_INT_GTVPR_M		(0x80000000)

#define MPIC_INT_GTDR_SEL_OUT		(0x0000000f)

#define MPIC_INT_IVPR_VECTOR		(0x000000ff)
#define MPIC_INT_IVPR_PRIORITY		(0x000f0000)
#define MPIC_INT_IVPR_P			(0x01000000)
#define MPIC_INT_IVPR_S			(0x02000000)
#define MPIC_INT_IVPR_MODE		(0x20000000)
#define MPIC_INT_IVPR_A			(0x40000000)
#define MPIC_INT_IVPR_M			(0x80000000)

#define MPIC_INT_IDR_SEL_OUT		(0x0000000f)

#define MPIC_INT_DAR_A			(0x0000000f)

#define MPIC_INT_DVPR_VECTOR		(0x000000ff)
#define MPIC_INT_DVPR_PRIORITY		(0x000f0000)
#define MPIC_INT_DVPR_A			(0x40000000)
#define MPIC_INT_DVPR_M			(0x80000000)

#define MPIC_INT_DDR_SEL_OUT		(0x0000000f)

#define MPIC_INT_MBVPR_VECTOR		(0x000000ff)
#define MPIC_INT_MBVPR_PRIORITY		(0x000f0000)
#define MPIC_INT_MBVPR_A		(0x40000000)
#define MPIC_INT_MBVPR_M		(0x80000000)

#define MPIC_INT_MBDR_SEL_OUT		(0x0000000f)

#define MPIC_INT_TASKP_TASKP		(0x0000000f)

#define MPIC_INT_VECTOR_VECTOR		(0x000000ff)
#define MPIC_INT_VECTOR_LS_VECTOR	(0xff000000)

#define MPIC_INT_EOI_EOI		(0x000000ff)

#define MPIC_INT_CSR_P			(0x00000001)
#define MPIC_INT_CSR_S			(0x00000002)

#ifdef __cplusplus
}
#endif

#endif	/* __INCsysTsi108Mpich */

