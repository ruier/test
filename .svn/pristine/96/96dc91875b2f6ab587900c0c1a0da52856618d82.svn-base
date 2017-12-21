/* sysEpic.h - Embedded Programmable Interrupt Controller Header File */

/*
 * Copyright (c) 2003-2005, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 2004-2005 Motorola Inc. All Rights Reserved */

/*
modification history
--------------------
01i,07may13,d_l  Fix build warnings.
01h,28jun05,cak  Overhauled with the driver as well. 
01g,14dec04,cak  Added definitions for external interrupt vector numbers.
01f,23aug04,scb  Starting point for "Big Easy".
01e,13oct03,mil  Added options for polarity and sense.
01d,29jul03,dtr  Set EUMBARVAL to CCSBAR.
01c,08jan03,mil  Created based on Sandpoint MPC107.
*/

#ifndef	__INCsysEpich
#define	__INCsysEpich

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Template for the 256 entry interrupt handler table.  An entry is
 * filled in for each intConnect().  Index into table is vector
 * number which is numerically equivalent to interrupt number.
 */
    
typedef struct intHandlerDesc     
    {
    VOIDFUNCPTR			vec;	/* interrupt vector */
    int				arg;	/* interrupt handler argument */
    struct  intHandlerDesc *	next;	/* pointer to the next handler */
    } INT_HANDLER_DESC;

#define	INTERRUPT_TABLESIZE	256

/*
 * Entry which defines interrupt number range and associated intEnable
 * and intDisable functions which cover the interrupt numbers within
 * the specified range.
 */

typedef struct intUtilDesc
    {
    int			loIntNum;		/* lowest interrupt number */
    int			hiIntNum;		/* highest interrupt number */
    int			(*intEnableFunc)(int);	/* assoc. intEnable fcn. */
    int			(*intDisableFunc)(int);	/* assoc. intDisable fnc. */
    } INT_UTIL_DESC;

/* Utility function #defines for intUtil [] */

#define INT_UTIL_MPC8540 EPIC_INT_VEC_MIN,EPIC_MAX_ALL_IRQS, \
                         epicIntEnable,epicIntDisable

/* bits 31(MSB)...0(LSB) enable only 1 bit at bit x */

#define BIT(x)		(1 << (x))

/* bits 31(MSB)...0(LSB) enable from bit x to bit y where y>x */

#define BITS_M2N(x, y)	(( 1 << ((y)-(x)+1) ) - 1) << (x)

/* bits 31(MSB)...0(LSB) from bit x towards LSB enable y bits */          

#define BITS(x, y)	BITS_M2N(x-y+1, x)

/* Global Configuration (GCR) Register */

#define EPIC_GCR_RESET			((UINT32)BIT(31))
#define EPIC_GCR_MODE_MIXED		BIT(29)

/* Bit and Mask Definitions for Destination Registers */

#define EPIC_DR_EP_INT		((UINT32)BIT(31))
#define EPIC_DR_CI_INT		BIT(30)

#define EPIC_CRIT_INT_TRUE	EPIC_DR_CI_INT
#define EPIC_CRIT_INT_FALSE	0x00000000
#define EPIC_CRIT_INT_NONE	0xFFFFFFFF

/* Bit and Mask Definitions for Vector/Priority Registers */

#define EPIC_INTR_EN		((UINT32)BIT(31))
#define EPIC_INTR_ACTIVE	BIT(30)
#define EPIC_INTR_POLARITY	BIT(23)
#define EPIC_INTR_SENSE		BIT(22)
#define EPIC_PRIORITY_MSK	(BIT(19) | BIT(18) | BIT(17) | BIT(16))
#define EPIC_PRIORITY(p)	(((p) << 16) & EPIC_EIVPR_PRIORITY_MSK)
#define EPIC_VECTOR_MSK		(0xffff)
#define EPIC_VECTOR(vec)	((vec) & EPIC_VECTOR_MSK)
#define EPIC_INT_POL_BIT	23
#define EPIC_INT_ACT_LOW	(0 << EPIC_INT_POL_BIT)
#define EPIC_INT_ACT_HIGH	(1 << EPIC_INT_POL_BIT)
#define EPIC_INT_EDG_NEG	(0 << EPIC_INT_POL_BIT)
#define EPIC_INT_EDG_POS	(1 << EPIC_INT_POL_BIT)
#define EPIC_INT_SENSE_BIT	22
#define EPIC_SENSE_LVL		(1 << EPIC_INT_SENSE_BIT)
#define EPIC_SENSE_EDG		(0 << EPIC_INT_SENSE_BIT)

/* Interrupt Vector Definitions */

#define EPIC_MAX_EXT_IRQS	12
#define EPIC_MAX_IN_IRQS	23
#define EPIC_MAX_GT_IRQS	4
#define EPIC_MAX_MSG_IRQS	4
#define EPIC_MAX_IPI_IRQS	4

#define EPIC_INT_VEC_MIN	0

/* 12 External Interrupts: 0 through 11 */

#define EPIC_VEC_EXT_IRQ0       0
#define EPIC_VEC_EXT_IRQ1       1
#define EPIC_VEC_EXT_IRQ2       2
#define EPIC_VEC_EXT_IRQ3       3
#define EPIC_VEC_EXT_IRQ4       4
#define EPIC_VEC_EXT_IRQ5       5
#define EPIC_VEC_EXT_IRQ6       6
#define EPIC_VEC_EXT_IRQ7       7
#define EPIC_VEC_EXT_IRQ8       8
#define EPIC_VEC_EXT_IRQ9       9
#define EPIC_VEC_EXT_IRQ10      10
#define EPIC_VEC_EXT_IRQ11      11

/* 23 Internal Interrupts: 12 through 34 */

#define EPIC_VEC_IN_IRQ0        (EPIC_VEC_EXT_IRQ0 + EPIC_MAX_EXT_IRQS)
#define EPIC_L2CACHE_INT_VEC	(EPIC_VEC_IN_IRQ0 + 0)
#define EPIC_ECM_INT_VEC	(EPIC_VEC_IN_IRQ0 + 1)
#define EPIC_DDR_INT_VEC	(EPIC_VEC_IN_IRQ0 + 2)
#define EPIC_LBC_INT_VEC	(EPIC_VEC_IN_IRQ0 + 3)
#define EPIC_DMA0_INT_VEC	(EPIC_VEC_IN_IRQ0 + 4)
#define EPIC_DMA1_INT_VEC	(EPIC_VEC_IN_IRQ0 + 5)
#define EPIC_DMA2_INT_VEC	(EPIC_VEC_IN_IRQ0 + 6)
#define EPIC_DMA3_INT_VEC	(EPIC_VEC_IN_IRQ0 + 7)
#define EPIC_PCI_INT_VEC	(EPIC_VEC_IN_IRQ0 + 8)
#define EPIC_RIOWRERR_INT_VEC	(EPIC_VEC_IN_IRQ0 + 9)
#define EPIC_RIODBELL_INT_VEC	(EPIC_VEC_IN_IRQ0 + 10)
#define EPIC_RIOMSGOUT_INT_VEC	(EPIC_VEC_IN_IRQ0 + 11)
#define EPIC_RIOMSGIN_INT_VEC	(EPIC_VEC_IN_IRQ0 + 12)
#define EPIC_TSEC1TX_INT_VEC	(EPIC_VEC_IN_IRQ0 + 13)
#define EPIC_TSEC1RX_INT_VEC	(EPIC_VEC_IN_IRQ0 + 14)
#define EPIC_TSEC1ERR_INT_VEC	(EPIC_VEC_IN_IRQ0 + 15)
#define EPIC_TSEC2TX_INT_VEC	(EPIC_VEC_IN_IRQ0 + 16)
#define EPIC_TSEC2RX_INT_VEC	(EPIC_VEC_IN_IRQ0 + 17)
#define EPIC_TSEC2ERR_INT_VEC	(EPIC_VEC_IN_IRQ0 + 18)
#define EPIC_FEC_INT_VEC	(EPIC_VEC_IN_IRQ0 + 19)
#define EPIC_DUART_INT_VEC	(EPIC_VEC_IN_IRQ0 + 20)
#define EPIC_DUART_INT_NUM	(EPIC_DUART_INT_VEC)
#define EPIC_I2C_INT_VEC	(EPIC_VEC_IN_IRQ0 + 21)
#define EPIC_PERFMON_INT_VEC	(EPIC_VEC_IN_IRQ0 + 22)

/* Global Timer Interrupts: 35 through 38 */

#define EPIC_VEC_GT_IRQ0        (EPIC_VEC_IN_IRQ0 + EPIC_MAX_IN_IRQS)
#define EPIC_VEC_GT_IRQ1	(EPIC_VEC_GT_IRQ0 + 1)
#define EPIC_VEC_GT_IRQ2	(EPIC_VEC_GT_IRQ0 + 2)
#define EPIC_VEC_GT_IRQ3	(EPIC_VEC_GT_IRQ0 + 3)

/* Message Register Interrupts: 39 through 42 */

#define EPIC_VEC_MSG_IRQ0	(EPIC_VEC_GT_IRQ0 + EPIC_MAX_GT_IRQS)
#define EPIC_VEC_MSG_IRQ1	(EPIC_VEC_MSG_IRQ0 + 1)
#define EPIC_VEC_MSG_IRQ2	(EPIC_VEC_MSG_IRQ0 + 2)
#define EPIC_VEC_MSG_IRQ3	(EPIC_VEC_MSG_IRQ0 + 3)

/* Inter-Processor (IPI) Interrupts: 43 through 46 */

#define EPIC_VEC_IPI_IRQ0       (EPIC_VEC_MSG_IRQ0 + EPIC_MAX_MSG_IRQS)
#define EPIC_VEC_IPI_IRQ1	(EPIC_VEC_IPI_IRQ0 + 1)
#define EPIC_VEC_IPI_IRQ2	(EPIC_VEC_IPI_IRQ0 + 2)
#define EPIC_VEC_IPI_IRQ3	(EPIC_VEC_IPI_IRQ0 + 3)

#define EPIC_MAX_ALL_IRQS	(EPIC_VEC_IPI_IRQ3)  

/* Interrupt Priority Definitions */

#define EPIC_PRIORITY_BIT	16
#define EPIC_PRIORITY_0		(0 << EPIC_PRIORITY_BIT)
#define EPIC_PRIORITY_1		(1 << EPIC_PRIORITY_BIT)
#define EPIC_PRIORITY_2		(2 << EPIC_PRIORITY_BIT)
#define EPIC_PRIORITY_3		(3 << EPIC_PRIORITY_BIT)
#define EPIC_PRIORITY_4		(4 << EPIC_PRIORITY_BIT)
#define EPIC_PRIORITY_5		(5 << EPIC_PRIORITY_BIT)
#define EPIC_PRIORITY_6		(6 << EPIC_PRIORITY_BIT)
#define EPIC_PRIORITY_7		(7 << EPIC_PRIORITY_BIT)
#define EPIC_PRIORITY_8		(8 << EPIC_PRIORITY_BIT)
#define EPIC_PRIORITY_9		(9 << EPIC_PRIORITY_BIT)
#define EPIC_PRIORITY_10	(10 << EPIC_PRIORITY_BIT)
#define EPIC_PRIORITY_11	(11 << EPIC_PRIORITY_BIT)
#define EPIC_PRIORITY_12	(12 << EPIC_PRIORITY_BIT)
#define EPIC_PRIORITY_13	(13 << EPIC_PRIORITY_BIT)
#define EPIC_PRIORITY_14	(14 << EPIC_PRIORITY_BIT)
#define EPIC_PRIORITY_15	(15 << EPIC_PRIORITY_BIT)
#define EPIC_PRIORITY_MIN	EPIC_PRIORITY_0	
#define EPIC_PRIORITY_MAX	EPIC_PRIORITY_15
#define EPIC_PRIORITY_DEFAULT	EPIC_PRIORITY_3

IMPORT STATUS	sysEpicIntConnect (VOIDFUNCPTR * vector, VOIDFUNCPTR routine,
                                   int parameter);
IMPORT int	sysEpicIntEnable (int vector);
IMPORT int	sysEpicIntDisable (int vector);
IMPORT void   	sysEpicInit (void);
IMPORT STATUS 	sysEpicIntrInit (void);
IMPORT int  	epicCurTaskPrioSet (int prioNum);
IMPORT int    	epicIntAck (void);
IMPORT void   	epicEOI (void);
IMPORT int      epicIntEnable (int vector);
IMPORT int      epicIntDisable (int vector);
IMPORT ULONG	epicGetVecRegAdrs (int vector);
IMPORT UINT32	sysEpicRegRead (ULONG regNum);
IMPORT void	sysEpicRegWrite (ULONG regNum, UINT32 regVal);

#ifdef __cplusplus
}
#endif

#endif	/* __INCsysEpich */
