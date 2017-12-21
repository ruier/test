/* i8259.h - Intel 8259a PIC (Programmable Interrupt Controller) */

/* Copyright 2003 Wind River Systems, Inc. */

/*
modification history
--------------------
01b,17jun03,j_b  add POLL word bit masks
01a,22apr03,scm  based off of (01h,23apr02,hdn) i8259.h -only we do not support slave
*/

#ifndef	__INCi8259h
#define	__INCi8259h

#ifdef __cplusplus
extern "C" {
#endif

/*
 * The macro PIC_REG_ADDR_INTERVAL must be defined
 * when including this header.
 */

/* default definitions */

/* ICW1 */
#define PIC_ICW4_NEEDED         0x01
#define PIC_N0_ICW4_NEEDED      0x00
#define PIC_SINGLE_MODE         0x02
#define PIC_CASCADE_MODE        0x00
#define PIC_INTERVAL_4          0x04
#define PIC_INTERVAL_8          0x00
#define PIC_LEVEL_TRIGGER       0x08
#define PIC_EDGE_TRIGGER        0x00
#define PIC_ICW1                0x10

/* ICW4 */
#define PIC_8086_MODE           0x01
#define PIC_8080_MODE           0x00
#define PIC_AUTO_EOI            0x02
#define PIC_NORMAL_EOI          0x00
#define PIC_NON_BUFFERED        0x04
#define PIC_BUFFERED_SLAVE      0x08
#define PIC_BUFFERED_MASTER     0x0C
#define PIC_SPECIAL_NESTED      0x10
#define PIC_NOT_SPECIAL_NESTED  0x00

/* Operational Command Word 2 */
#define PIC_MASK_ALL_INTR       0xFF

/* Operational Command Word 2 */
#define PIC_NONSPECIFIC_EOI     0x20
#define PIC_SPECIFIC_EOI        0x60
#define PIC_ROT_NONSPEC_EOI     0xA0
#define PIC_ROT_AUT_EOI_SET     0x80
#define PIC_ROT_AUT_EOI_CLR     0x00
#define PIC_ROT_SPEC_AUT_EOI    0xE0
#define PIC_SET_PRIORTY_CMD     0xC0
#define PIC_OCW2_NOP            0x40

/* Operational Command Word 3 */
#define PIC_RESET_SPECIAL_MASK  0x40
#define PIC_SET_SPECIAL_MASK    0x60
#define PIC_OCW3                0x08
#define PIC_POLL_CMD            0x04
#define PIC_NO_POLL_CMD         0x00
#define PIC_READ_IR_REG         0x02
#define PIC_READ_IS_REG         0x03
#define PIC_DISABLE_SMM         0x08
#define PIC_ENABLE_SMM          0x68

#define PIC_ADRS(base,reg)  (CAST (base+(reg*PIC_REG_ADDR_INTERVAL)))

/* register definitions */

#define PIC_port1(base)    PIC_ADRS(base,0x00) /* port 1. */
#define PIC_port2(base)    PIC_ADRS(base,0x01) /* port 2. */

/* alias */

#define PIC_IMASK(base)    PIC_port2(base) /* Interrupt mask. */
#define PIC_IACK(base)     PIC_port1(base) /* Interrupt acknowledge. */

#define PIC_ISR_MASK(base) PIC_port1(base) /* in-service register mask */
#define PIC_IRR_MASK(base) PIC_port1(base) /* interrupt request reg */

#define OCW3_DEF           0x08            /* 3rd default control word */
#define OCW3_PCB           0x04            /* Polling Control Bit */
#define OCW3_ISR           0x03            /* Read in-service reg */
#define OCW3_IRR           0x02            /* Read inter request reg */

#define I8259_EOI          0x20            /* EOI bit in OCW2 */

/* Poll word fields */
#define PIC_POLL_INT_STATUS     0x80
#define PIC_POLL_INT_LVL_MASK   0x7F

#ifndef _ASMLANGUAGE

#if defined(__STDC__) || defined(__cplusplus)

IMPORT VOID i8259Init         (void);
IMPORT int  sysIntLevel       (void);
IMPORT VOID sysIntLock        (void);
IMPORT VOID sysIntUnlock      (void);
IMPORT VOID i8259IntEoi       (void);
IMPORT int  i8259IntLevel     (void);

#else

IMPORT VOID i8259Init         ();
IMPORT int  sysIntLevel       ();
IMPORT VOID sysIntLock        ();
IMPORT VOID sysIntUnlock      ();
IMPORT VOID i8259IntEoi       ();
IMPORT int  i8259IntLevel     ();

#endif  /* __STDC__ */

#endif	/* _ASMLANGUAGE */

#ifdef __cplusplus
}
#endif

#endif	/* __INCi8259h */
