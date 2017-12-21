/* i8259Intr.c - Intel 8259A PIC (Programmable Interrupt Controller) driver */

/* Copyright 2004 Wind River Systems, Inc. */
#include "copyright_wrs.h"

/*
modification history
--------------------
01c,23sep04,mdo  Documentation fixes for apigen
01b,16jun03,j_b  replace hardcoded values; cleanup
01a,22apr03,scm  based on (01p,16apr02,hdn) i8259Intr.c --we only have a single 8259A
*/

/*
DESCRIPTION
This module is a driver for the Intel 8259A PIC (Programmable Interrupt
Controller).  The Intel 8259A handles up to 8 vectored priority interrupts
for the CPU.  It is cascade-able for up to 64 vectored priority interrupts,
though this driver assumes a single 8259A.  It is initialized for
"Fully Nested Mode", "Non-Specific EOI" mode.

Fully Nested Mode:
In this mode, interrupt requests are ordered in priority
from 0 through 7 (0 is the highest priority).  When an interrupt is
acknowledged, the highest priority request is determined and its vector is
placed on the bus.  Additionally, a bit of the Interrupt Service (IS)
register is set.  This bit remains set until the microprocessor issues an
EOI command immediately before returning from the service routine.  While
the IS bit is set, all further interrupts of the same or lower priority
are inhibited, while higher level interrupts are allowed.  The
i8259IntEoiSlaveNfnm() routine is used to issue the EOI command.  The PICs
in a PC typically operate in this mode (normal nested mode).  In this mode,
while the slave PIC is being serviced by the master PIC, the slave PIC blocks
all higher priority interrupt requests.  Alternatively, to allow interrupts of
a higher priority, enable the Special Fully Nested Mode.

Special Fully Nested Mode: I8259_SPECIAL_FULLY_NESTED_MODE.
This mode is similar to the Fully Nested Mode with the following exceptions:

1) When an interrupt request from a slave PIC is in service, the slave is
not locked out from the master's priority logic and further interrupt
requests from higher priority IRs within the slave are recognized by
the master and will initiate interrupts to the processor.  
2) When exiting the interrupt service routine, the software must check 
whether or not the interrupt serviced was the only interrupt request from the 
slave.  If it was the only interrupt request, a non-specific EOI is sent to 
the master. If not, no EOI is sent.  This is implemented by the 
i8259EoiSlaveSfnm() routine.

Non-Specific EOI: When the 8259A is operated in the Fully Nested Mode, it
can determine which IS bit to reset on EOI.  When a non-specific EOI
command is issued, the 8259A automatically resets the highest IS bit of
those that are set, since in the fully nested mode the highest IS level is
the last level acknowledged and serviced.

Spurious/Phantom Interrupt: The IR inputs must remain high until after the
falling edge of the first INTA.  If the IR input goes low before this time,
a DEFAULT(Spurious/Phantom) IR7 occurs when the CPU acknowledges the
interrupt.  The interrupt handler should simply return without sending
an EOI command.

The PIC(8259A) IRQ0 is hard wired to the PIT(8253) channel 0 in a PC
motherboard.  IRQ0 is the highest priority in the 8259A interrupt
controller.  Thus, the system clock interrupt handler blocks all lower
level interrupts.  This may cause a delay of the lower level interrupts in
some situations even though the system clock interrupt handler finishes its
job without any delay.  This is quite natural from the hardware point of
view, but may not be ideal from the application software standpoint.  The
following modes are supplied to mitigate this situation by providing
corresponding configuration macros in the BSP.  The three modes are mutually
exclusive.

Early EOI Issue in IRQ0 ISR:
In this mode, the EOI command is issued before the IRQ0 system clock interrupt
service routine starts the kernel work.  This lowers the IRQ0 ISR blocking
level to the next lower level.  If no IRQs are in service, the next lower
level is the lowest level.  If IRQn is in service, the next lower level
corresponds to the next lower priority.  As a result, the kernel work in the
system clock interrupt service routine can be interrupted by an interrupt
with a higher priority than the blocking level.  The i8259IntBoiEem() routine
is called before the interrupt service routine, and no EOI is sent after
the interrupt service routine.

Special Mask Mode in IRQ0 ISR:
In this mode, the Special Mask Mode is used in the IRQ0 system clock
service routine.  This lowers the blocking level to the specified level
(currently hard coded to the lowest level).  The i8259IntBoiSmm() routine
is called before the interrupt service routine, and the i8259IntEoiSmm()
routine is called after the interrupt service routine.

Automatic EOI Mode: I8259_AUTO_EOI:
This mode provides no nested multi-level interrupt structure in PIC1.  The
EOI command is automatically sent to the master PIC at the end of the
interrupt acknowledge cycle.  Thus, no software intervention is needed.  The
i8259IntBoi() routine is called before the IRQ7 and IRQ15 interrupt service
routines.  Either the i8259IntEoiSlaveNfnm() routine or the
i8259IntEoiSlaveSfnm() routine is called after the slave PIC's interrupt
service routine.

SEE ALSO: target.ref
*/

#ifdef INCLUDE_i8259A

#include "./i8259.h"

/* externs */

/* defines */

/* globals */

/* local */

LOCAL INT8	i8259IntMask1;		/* interrupt mask for PIC1 */

/* forward static functions */

/*******************************************************************************
*
* i8259Init - initialize the PIC
*
* This routine initializes the PIC.
*
* RETURNS:
*
* ERRNO
*/

VOID i8259Init (void)
    {
    FAST INT32 oldLevel;
    UINT8 icw4;

    /* initialize the PIC (Programmable Interrupt Controller) */

    /* LOCK INTERRUPTS */
    oldLevel = intLock ();

    /* ICW1 */
    sysOutByte (PIC_port1 (PIC1_BASE_ADR), (PIC_ICW4_NEEDED | PIC_SINGLE_MODE | 
                    PIC_INTERVAL_4 | PIC_LEVEL_TRIGGER | PIC_ICW1));

    /* ICW2 */
    sysOutByte (PIC_port2 (PIC1_BASE_ADR), PIC1_IRQ_BASE);

    /* ICW3 not used if PIC_SINGLE_MODE in ICW1 */

    icw4 = (PIC_8086_MODE | PIC_NORMAL_EOI | PIC_NON_BUFFERED | PIC_NOT_SPECIAL_NESTED);
    sysOutByte (PIC_port2 (PIC1_BASE_ADR), icw4);

    /* disable interrupts */
    /* OCW1: mask all interrupts */
    sysOutByte (PIC_port2 (PIC1_BASE_ADR), PIC_MASK_ALL_INTR);

    /* OCW2: NOP */
    sysOutByte (PIC_port1 (PIC1_BASE_ADR), PIC_OCW2_NOP);

    /* OCW3: reset special mask mode */
    sysOutByte (PIC_port1 (PIC1_BASE_ADR), (PIC_RESET_SPECIAL_MASK | PIC_OCW3 | PIC_NO_POLL_CMD));

    /* UNLOCK INTERRUPTS */
    intUnlock (oldLevel);
    }

/*******************************************************************************
*
* i8259IntEoi - send EOI(end of interrupt) signal to the PIC.
*
* This routine is called at the end of the interrupt handler.
*
* RETURNS:
*
* ERRNO
*/

VOID i8259IntEoi(void)
    {
    FAST INT32 oldLevel = intLock ();  /* LOCK INTERRUPTS */

    sysOutByte (PIC_IACK (PIC1_BASE_ADR), I8259_EOI); /* non-specific EOI */

    intUnlock (oldLevel);             /* UNLOCK INTERRUPTS */
    }

/*******************************************************************************
*
* i8259IntLevel - Get interrupt level by reading Interrupt Service Register.
*
* This routine is called to get an interrupt level in service.
*
* RETURNS: interrupt level.
*
* ERRNO
*/

int i8259IntLevel(void)
    {
    int iv;
    FAST ULONG statusReg;
    UINT8 icw4;

    /* LOCK INTERRUPTS */
    statusReg = intLock ();

    sysOutByte (PIC_port1 (PIC1_BASE_ADR), (PIC_RESET_SPECIAL_MASK | PIC_OCW3 | PIC_POLL_CMD));

    iv = sysInByte (PIC_port1 (PIC1_BASE_ADR));

    icw4 = (PIC_8086_MODE | PIC_NORMAL_EOI | PIC_NON_BUFFERED | PIC_NOT_SPECIAL_NESTED);
    sysOutByte (PIC_port1 (PIC1_BASE_ADR), icw4);

    /* UNLOCK INTERRUPTS */
    intUnlock(statusReg);

    /* check for interrupt and return priority level */
    if (iv & PIC_POLL_INT_STATUS)
        {
        iv = iv & PIC_POLL_INT_LVL_MASK;
        }
    else
        {
        iv = ERROR;  /* no interrupt */
        }
    return iv;
    }

/*******************************************************************************
*
* i8259IntDisable - disable a specified PIC interrupt input line
*
* This routine disables a specified PIC interrupt input line.
*
* RETURNS: OK, always.
*
* ERRNO
*
* SEE ALSO: i8259IntEnable()
*
*/

LOCAL STATUS i8259IntDisable
    (
    INT32 irqNo        /* IRQ number to disable */
    )
    {
    FAST INT32 oldLevel;

    if (irqNo < NUMBER_OF_PIC_IRQS)
        {
        oldLevel = intLock ();              /* LOCK INTERRUPTS */
        sysOutByte (PIC_IMASK (PIC1_BASE_ADR),
                    sysInByte (PIC_IMASK (PIC1_BASE_ADR)) | (1 << irqNo));
        intUnlock (oldLevel);               /* UNLOCK INTERRUPTS */
        }

    return (OK);
    }

/*******************************************************************************
*
* i8259IntEnable - enable a specified PIC interrupt input line
*
* This routine enables a specified PIC interrupt input line.
*
* RETURNS: OK, always.
*
* ERRNO
*
* SEE ALSO: i8259IntDisable()
*
*/

LOCAL STATUS i8259IntEnable
    (
    INT32 irqNo        /* IRQ number to enable */
    )
    {
    FAST INT32 oldLevel;

    if (irqNo < NUMBER_OF_PIC_IRQS)
        {
        oldLevel = intLock ();              /* LOCK INTERRUPTS */
        sysOutByte (PIC_IMASK (PIC1_BASE_ADR),
                    sysInByte (PIC_IMASK (PIC1_BASE_ADR)) & ~(1 << irqNo));
        intUnlock (oldLevel);               /* UNLOCK INTERRUPTS */
        }

    return (OK);
    }

/*******************************************************************************
*
* i8259IntLock - lock out all PIC interrupts
*
* This routine saves the mask and locks out all PIC interrupts.
* It should be called in the interrupt disable state(IF bit is 0).
*
* RETURNS:
*
* ERRNO
*
* SEE ALSO: i8259IntUnlock()
*
*/

LOCAL VOID i8259IntLock (void)
    {
    i8259IntMask1 = sysInByte (PIC_IMASK (PIC1_BASE_ADR));
    sysOutByte (PIC_IMASK (PIC1_BASE_ADR), PIC_MASK_ALL_INTR);
    }

/*******************************************************************************
*
* i8259IntUnlock - unlock the PIC interrupts
*
* This routine restores the mask and unlocks the PIC interrupts
* It should be called in the interrupt disable state(IF bit is 0).
*
* RETURNS:
*
* ERRNO
*
* SEE ALSO: i8259IntLock()
*
*/

LOCAL VOID i8259IntUnlock (void)
    {
    sysOutByte (PIC_IMASK (PIC1_BASE_ADR), i8259IntMask1);
    }
#endif  /* INCLUDE_i8259A */

