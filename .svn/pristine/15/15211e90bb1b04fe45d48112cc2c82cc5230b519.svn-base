/* romInit.s - Freescale ROM initialization module for HPC2-8641 */

/*
 * Copyright (c) 2008-2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind RIver License agreement.
 */

/*
modification history
--------------------
01c,28aug09,mdg  Changed flag that tracks if I2C should use locking from
                 global variable to UART scratch register, fixing issue 
                 with bootrom_uncmp. (WIND00179587)
01b,06jan09,e_d  change rominit for 6.7 SMP
01a,17mar08,mdg  written from hpc8641D and mv3100
*/

/*
DESCRIPTION
This module contains the entry code for VxWorks images that start
running from ROM, such as 'bootrom' and 'vxWorks_rom'.
The entry point, romInit(), is the first code executed on power-up.
It performs the minimal setup needed to call
the generic C routine romStart() with parameter BOOT_COLD.

RomInit() typically masks interrupts in the processor, sets the initial
stack pointer (to STACK_ADRS which is defined in configAll.h), and
readies system memory by configuring the DRAM controller if necessary.
Other hardware and device initialization is performed later in the
BSP's sysHwInit() routine.

A second entry point in romInit.s is called romInitWarm(). It is called
by sysToMonitor() in sysLib.c to perform a warm boot.
The warm-start entry point must be written to allow a parameter on
the stack to be passed to romStart().

WARNING:
This code must be Position Independent Code (PIC).  This means that it
should not contain any absolute address references.  If an absolute address
must be used, it must be relocated by the macro ROM_ADRS(x).  This macro
will convert the absolute reference to the appropriate address within
ROM space no matter how the boot code was linked. (For PPC, ROM_ADRS does
not work.  You must subtract _romInit and add ROM_TEXT_ADRS to each
absolute address). (NOTE: ROM_ADRS(x) macro does not work for current
PPC compiler).

This code should not call out to subroutines declared in other modules,
specifically sysLib.o, and sysALib.o.  If an outside module is absolutely
necessary, it can be linked into the system by adding the module
to the makefile variable BOOT_EXTRA.  If the same module is referenced by
other BSP code, then that module must be added to MACH_EXTRA as well.
Note that some C compilers can generate code with absolute addresses.
Such code should not be called from this module.  If absolute addresses
cannot be avoided, then only ROM resident code can be generated from this
module.  Compressed and uncompressed bootroms or VxWorks images will not
work if absolute addresses are not processed by the macro ROM_ADRS.

WARNING:
The most common mistake in BSP development is to attempt to do too much
in romInit.s.  This is not the main hardware initialization routine.
Only do enough device initialization to get memory functioning.  All other
device setup should be done in sysLib.c, as part of sysHwInit().

Unlike other RTOS systems, VxWorks does not use a single linear device
initialization phase.  It is common for inexperienced BSP writers to take
a BSP from another RTOS, extract the assembly language hardware setup
code and try to paste it into this file.  Because VxWorks provides 3
different memory configurations, compressed, uncompressed, and rom-resident,
this strategy will usually not work successfully.

WARNING:
The second most common mistake made by BSP writers is to assume that
hardware or CPU setup functions done by romInit.o do not need to be
repeated in sysALib.s or sysLib.c.  A vxWorks image needs only the following
from a boot program: The startType code, and the boot parameters string
in memory.  Each VxWorks image will completely reset the CPU and all
hardware upon startup.  The image should not rely on the boot program to
initialize any part of the system (it may assume that the memory controller
is initialized).

This means that all initialization done by romInit.s must be repeated in
either sysALib.s or sysLib.c.  The only exception here could be the
memory controller.  However, in most cases even that can be
reinitialized without harm.

Failure to follow this rule may require users to rebuild bootrom's for
minor changes in configuration.  It is WRS policy that bootroms and vxWorks
images should not be linked in this manner.

INCLUDE FILES:
*/


/* includes */

#define _ASMLANGUAGE
#include <vxWorks.h>
#include <sysLib.h>
#include <asm.h>
#include <regs.h>
#include "config.h"
#include "arch/ppc/mmu603Lib.h"

/* defines */

/*
 * Some releases of h/arch/ppc/toolPpc.h had a bad definition of
 * LOADPTR. So we will define it correctly. [REMOVE WHEN NO LONGER NEEDED].
 * This is tracked at WRS under SPR#20104.
 * LOADPTR initializes a register with a 32 bit constant, presumably the
 * address of something.
 */

#undef LOADPTR
#define LOADPTR(reg,const32) \
        addis reg,r0,HIADJ(const32); addi reg,reg,LO(const32)

/*
 * LOADVAR initializes a register with the contents of a specified memory
 * address. The difference being that the value loaded is the contents of
 * the memory location and not just the address of it. [REMOVE WHEN NO LONGER
 * NEEDED].
 * This is tracked at WRS under SPR#20104.
 */

#undef LOADVAR
#define LOADVAR(reg,addr32) \
        addis reg,r0,HIADJ(addr32); \
        lwz reg,LO(addr32)(reg)

#define WRITEADR(reg1,reg2,addr32,val) \
        lis     reg1, HI(addr32); \
        ori     reg1, reg1, LO(addr32); \
        lis     reg2, HI(val); \
        ori     reg2, reg2, LO(val); \
        stw     reg2, 0(reg1)

#define WRITEOFFSET(regbase,reg2,offset,val) \
        lis     reg2, HI(val); \
        ori     reg2, reg2, LO(val); \
        stw     reg2, offset(regbase);

#define DSSALL .long 0x7E00066C          /* explicit encoding for DSSALL instruction */
#define L2CR                1017         /* L2 Cache Control Register */
#define PPC_L2CR_L2IO       0x00100000   /* L2 Instruction-Only Mode */

#define CACHE_STACK_ADRS    (0x00003000) /* No-RAM cache initial stack top */
#define CACHE_STACK_SIZE    (8*1024)

/*
 * NOTE: Cannot use ROM_ADRS macro with HIADJ and LO macro
 * functions, for PPC
 */

        /* Exported internal functions */

        .data
        FUNC_EXPORT(_romInit)           /* start of system code */
        FUNC_EXPORT(romInit)            /* start of system code */
        FUNC_EXPORT(_romInitWarm)       /* start of system code */
        FUNC_EXPORT(romInitWarm)        /* start of system code */

        /* externals */

        FUNC_IMPORT(romStart)           /* system initialization routine */
        FUNC_IMPORT(sysMpc8641SmcInitialize) /* DDR controller init */
        FUNC_IMPORT(sysCpu1Loop)

        _WRS_TEXT_SEG_START

/*******************************************************************************
*
* romInit - entry point for VxWorks in ROM
*

* romInit
*     (
*     int startType     /@ only used by 2nd entry point @/
*     )

*/

        /*
         * After reset, the processor fetches from ROM_BASE_ADRS + 0x100.
         * We offset the _romInit entry by 0x100 bytes to compensate.
         * In other WRS PPC BSP's, this offset was accounted for in the
         * elfHex stage of building the rom image by setting HEX_FLAGS
         * to "-a 100".  Explictly adding the space here seems more
         * straitforward, and it also helps when not using elfToHex
         * such as in a flash programmed binary image.   Therefore
         * the Yellowknife BSP Makefile uses HEX_FLAGS of "-a 0".
         * This also means that ROM_TEXT_ADRS must be defined
         * equivalent to ROM_BASE_ADRS + 0x100 in config.h and Makefile.
         */

        .space (0x100)


FUNC_LABEL(_romInit)
FUNC_BEGIN(romInit)

        /* This is the cold boot entry (ROM_TEXT_ADRS) */

        /*
         * According to a comment in the DINK32 startup code, a 7400
         * erratum requires an isync to be the first instruction executed
         * here.  The isync plus the branch amount to 8 bytes, so omit the
         * nop which would otherwise follow the branch.
         */
        isync
        sync
        bl      cold

        /*
         * This warm boot entry point is defined as ROM_WARM_ADRS in config.h.
         * It is defined as an offset from ROM_TEXT_ADRS.  Please make sure
         * that the offset from _romInit to romInitWarm matches that specified
         * in config.h.  (WRS is standardizing this offset to 8 bytes.)
         */

_romInitWarm:
romInitWarm:

        /*
         * See above comment regarding a 7400 erratum.  Since DINK32
         * uses a common entry point for both cold and warm boot, it's
         * not clear which one requires the isync.  isync is harmless,
         * so do it both places to be safe.
         */

        isync
        sync
        bl      warm

        /*
         * copyright notice appears at beginning of ROM (in TEXT segment)
         * This is also useful for debugging images.
         */

        .ascii  "Copyright 1984-2008 Wind River Systems, Inc."
        .balign 4

platpllTable:
        .long 0x00000203
        .long 0x04050607
        .long 0x08090a0b
        .long 0xc0d00000
        .long 0x00000000
        .long 0x00000000

cold:
        li   r11, BOOT_COLD
        li   r12, 0
        bl   start           /* skip over next instruction */

warm:
        or   r11, r3, r3     /* startType to r11 */
        li   r12, 1
start:

        /*
         * DINK32 does a sync after the isync.  It may or may not
         * be necessary, but should at least be harmless.
         */

        sync

        /* Zero-out the CPU registers */

        xor     r0,r0,0         /* DINK32 uses xor instead of addis */
        mtspr   SPRG0,r0
        mtspr   SPRG1,r0
        mtspr   SPRG2,r0
        mtspr   SPRG3,r0

        /* initialize the stack pointer */

        LOADPTR (sp, STACK_ADRS)

        /* Set MPU/MSR to a known state. Turn on FP */

        LOADPTR (r3, _PPC_MSR_FP)
        sync
        mtmsr   r3
        isync
        sync            /* DINK32 does both isync and sync after mtmsr */

        /* Init the floating point control/status register */

        mtfsfi  7,0x0
        mtfsfi  6,0x0
        mtfsfi  5,0x0
        mtfsfi  4,0x0
        mtfsfi  3,0x0
        mtfsfi  2,0x0
        mtfsfi  1,0x0
        mtfsfi  0,0x0
        isync

        /* Initialize the floating point data regsiters to a known state */

        bl      ifpdr_value
        .long   0x3f800000      /* 1.0 */

ifpdr_value:
        mfspr   r3,LR
        lfs     f0,0(r3)
        lfs     f1,0(r3)
        lfs     f2,0(r3)
        lfs     f3,0(r3)
        lfs     f4,0(r3)
        lfs     f5,0(r3)
        lfs     f6,0(r3)
        lfs     f7,0(r3)
        lfs     f8,0(r3)
        lfs     f9,0(r3)
        lfs     f10,0(r3)
        lfs     f11,0(r3)
        lfs     f12,0(r3)
        lfs     f13,0(r3)
        lfs     f14,0(r3)
        lfs     f15,0(r3)
        lfs     f16,0(r3)
        lfs     f17,0(r3)
        lfs     f18,0(r3)
        lfs     f19,0(r3)
        lfs     f20,0(r3)
        lfs     f21,0(r3)
        lfs     f22,0(r3)
        lfs     f23,0(r3)
        lfs     f24,0(r3)
        lfs     f25,0(r3)
        lfs     f26,0(r3)
        lfs     f27,0(r3)
        lfs     f28,0(r3)
        lfs     f29,0(r3)
        lfs     f30,0(r3)
        lfs     f31,0(r3)
        sync

        /* Set MPU/MSR to a known state. Turn off FP */

        andi.   r3, r3, 0
        sync
        mtmsr   r3
        isync
        sync            /* DINK32 does both isync and sync after mtmsr */

        /* Init the Segment registers */

        andi.   r3, r3, 0
        isync
        mtsr    0,r3
        isync
        mtsr    1,r3
        isync
        mtsr    2,r3
        isync
        mtsr    3,r3
        isync
        mtsr    4,r3
        isync
        mtsr    5,r3
        isync
        mtsr    6,r3
        isync
        mtsr    7,r3
        isync
        mtsr    8,r3
        isync
        mtsr    9,r3
        isync
        mtsr    10,r3
        isync
        mtsr    11,r3
        isync
        mtsr    12,r3
        isync
        mtsr    13,r3
        isync
        mtsr    14,r3
        isync
        mtsr    15,r3
        isync

        /* Turn off data and instruction cache control bits */

        mfspr   r3, HID0
        isync
        rlwinm  r4, r3, 0, _PPC_HID0_BIT_DCE + 1, _PPC_HID0_BIT_ICE - 1
        sync
        isync
        mtspr   HID0, r4                /* HID0 = r4 */
        isync

        /* invalidate the MPU's data/instruction caches */

        mfspr   r3, HID0
        sync
        oris    r3, r3, 0x0400            # Set TBEN
        oris    r3, r3, 0x0010            # Set DPM
        mtspr   HID0, r3
        sync

        addis   r4,r0,0x0000    /* Clear r4 */
        ori     r4,r4,0x8800    /* Setup bit pattern for ICE/ICFI */
        or      r3,r4,r3
        isync
        mtspr   HID0,r3         /* set ICE/ICFI */
        isync

        addis   r4,r0,0x0000    /* Clear r4 */
        ori     r4,r4,0x0800    /* Setup bit pattern for ICFI */
        andc    r3,r3,r4
        isync
        mtspr   HID0,r3         /* clear IFCI (bit 16) */
        isync

        addis   r4,r0,0x0000    /* Clear r4 */
        ori     r4,r4,0x2000    /* Setup bit pattern for ILOCK */
        andc    r3,r3,r4
        isync
        mtspr   HID0,r3         /* clear ILOCK (bit 18) */
        isync
        sync

        /* NOTE: This code is for vxWorks ROMable image, not for bootrom */

        /* initialize PIR with cpu index */

        mfspr   r3, MSSCR0
        rlwinm. r3, r3, _PPC_MSSCR0_BIT_ID + 1, 31, 31
        mtspr   PIR, r3
        beq     1f                              /* branch if CPU0 */

        /* running on CPU1 */

        /* set up initial stack pointer */

        LOADVAR (sp, CPU1_STACK_START_ADR)
        stwu    r0, -FRAMEBASESZ(sp)            /* save one frame stack */

    /* jump to sysCpu1Loop or sysInit */

waitForInitStartSet:
        LOADVAR (r3, CPU1_INIT_START_ADR)

        /*
         * Check that init start address is not zero before jumping
         * This allows cpu1 to reboot and wait for cpu0 to reload image
         */

        lis     r4,HI(MP_MAGIC_RBV)
        ori     r4, r4, LO(MP_MAGIC_RBV)
        cmpw    r3, r4
        beq     waitForInitStartSet
        mtctr   r3
        bctr

1:
        cmpwi   r12,1         /* Check to see if reboot or power on */
        beq     startCSetup   /* If reboot don't touch ccsrbar or DDR etc*/

        /* Configure MPX/CCB arbiter */

        lis     r6,HIADJ(CCSR_ADDR(0xff700000,CCSR_MCM_DBCR))
        addi    r6,r6,LO(CCSR_ADDR(0xff700000,CCSR_MCM_DBCR))
        lis     r7,HIADJ(0x40000000)
        addi    r7,r7,LO(0x40000000)
        stw     r7,0x0(r6)
        isync
        eieio

        /*
         * Errata form revI of 8641d errata
         * MCM8 - use round robin
         * MCM7 - stream cnt should be 3
         */

        lis     r6,HIADJ(CCSR_ADDR(0xff700000,CCSR_MCM_ABCR))
        addi    r6,r6,LO(CCSR_ADDR(0xff700000,CCSR_MCM_ABCR))
        lis     r7,HIADJ(0x00001003)
        addi    r7,r7,LO(0x00001003)
        stw     r7,0x0(r6)
        isync
        eieio

        /* Setup the memory mapped register address */

        isync
        sync

        lis     r6,HI(CCSBAR)
        ori     r6,r6, LO(CCSBAR)
        isync

        srwi    r5, r6, 12
        lis     r7,HI(0xff700000)
        ori     r7,r7,LO(0xff700000)
        lwz     r4, 0(r7)

        isync
        sync
        eieio

        stw     r5,0(r7)

        sync
        isync
        eieio

        lis    r5,0xffff
        ori    r5, r5,0xf800
        lwz    r4, 0 (r5)
        isync

        li     r4,0x2000
        mtctr   r4
ccsrbarWait:
        nop
        bdnz    ccsrbarWait

        /* Set 0 - FLASH */

        lis     r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_LAWBAR0))
        addi    r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_LAWBAR0))

        lis     r7,HIADJ(CCSR_LAWBAR0_PROG)
        addi    r7,r7,LO(CCSR_LAWBAR0_PROG)
        stw     r7,0x0(r6)

        lis     r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_LAWAR0))
        addi    r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_LAWAR0))

        lis     r7,HIADJ (CCSR_LAWAR0_PROG)
        addi    r7,r7,LO (CCSR_LAWAR0_PROG)
        stw     r7,0x0(r6)
        isync
        sync
        eieio

        /* Set 1 - DRAM */

        lis     r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_LAWBAR1))
        addi    r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_LAWBAR1))

        lis     r7,HIADJ(CCSR_LAWBAR1_PROG)
        addi    r7,r7,LO(CCSR_LAWBAR1_PROG)
        stw     r7,0x0(r6)

        lis     r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_LAWAR1))
        addi    r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_LAWAR1))

        lis     r7,HIADJ (CCSR_LAWAR1_PROG)
        addi    r7,r7,LO (CCSR_LAWAR1_PROG)
        stw     r7,0x0(r6)
        isync
        sync
        eieio

        /* Set 2 */

        lis     r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_LAWBAR2))
        addi    r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_LAWBAR2))

        lis     r7,HIADJ(CCSR_LAWBAR2_PROG)
        addi    r7,r7,LO(CCSR_LAWBAR2_PROG)
        stw     r7,0x0(r6)

        lis     r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_LAWAR2))
        addi    r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_LAWAR2))

        lis     r7,HIADJ (CCSR_LAWAR2_PROG)
        addi    r7,r7,LO (CCSR_LAWAR2_PROG)
        stw     r7,0x0(r6)
        isync
        sync
        eieio

        /* Set 3 */

        lis     r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_LAWBAR3))
        addi    r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_LAWBAR3))

        lis     r7,HIADJ(CCSR_LAWBAR3_PROG)
        addi    r7,r7,LO(CCSR_LAWBAR3_PROG)
        stw     r7,0x0(r6)

        lis     r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_LAWAR3))
        addi    r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_LAWAR3))

        lis     r7,HIADJ (CCSR_LAWAR3_PROG)
        addi    r7,r7,LO (CCSR_LAWAR3_PROG)
        stw     r7,0x0(r6)
        isync
        sync
        eieio

        /* Set 4 */

        lis     r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_LAWBAR4))
        addi    r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_LAWBAR4))

        lis     r7,HIADJ(CCSR_LAWBAR4_PROG)
        addi    r7,r7,LO(CCSR_LAWBAR4_PROG)
        stw     r7,0x0(r6)

        lis     r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_LAWAR4))
        addi    r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_LAWAR4))

        lis     r7,HIADJ (CCSR_LAWAR4_PROG)
        addi    r7,r7,LO (CCSR_LAWAR4_PROG)
        stw     r7,0x0(r6)
        isync
        sync
        eieio

        /* Test for the XMC Span */

        lis     r6,HIADJ(BRD_PRESENCE_DETECT_REG)
        addi    r6,r6,LO(BRD_PRESENCE_DETECT_REG)
        lbz     r7,0x0(r6)
        andi.   r7,r7,BRD_PRESENCE_DETECT_PMC_SPAN_MASK
        beq     no_xmcspan

        /* Set 5 */

        lis     r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_LAWBAR5))
        addi    r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_LAWBAR5))

        lis     r7,HIADJ(CCSR_LAWBAR5_PROG)
        addi    r7,r7,LO(CCSR_LAWBAR5_PROG)
        stw     r7,0x0(r6)

        lis     r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_LAWAR5))
        addi    r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_LAWAR5))

        lis     r7,HIADJ (CCSR_LAWAR5_PROG)
        addi    r7,r7,LO (CCSR_LAWAR5_PROG)
        stw     r7,0x0(r6)
        isync
        sync
        eieio

        /* Set 6 */

        lis     r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_LAWBAR6))
        addi    r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_LAWBAR6))

        lis     r7,HIADJ(CCSR_LAWBAR6_PROG)
        addi    r7,r7,LO(CCSR_LAWBAR6_PROG)
        stw     r7,0x0(r6)

        lis     r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_LAWAR6))
        addi    r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_LAWAR6))

        lis     r7,HIADJ (CCSR_LAWAR6_PROG)
        addi    r7,r7,LO (CCSR_LAWAR6_PROG)
        stw     r7,0x0(r6)
        isync
        sync
        eieio
        b       do_lawbar7

no_xmcspan:

        /* Set 5 */

        lis     r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_LAWBAR5))
        addi    r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_LAWBAR5))

        lis     r7,HIADJ(CCSR_LAWBARn_INVALID)
        addi    r7,r7,LO(CCSR_LAWBARn_INVALID)
        stw     r7,0x0(r6)

        lis     r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_LAWAR5))
        addi    r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_LAWAR5))

        lis     r7,HIADJ (CCSR_LAWARn_INVALID)
        addi    r7,r7,LO (CCSR_LAWARn_INVALID)
        stw     r7,0x0(r6)
        isync
        sync
        eieio

        /* Set 6 */

        lis     r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_LAWBAR6))
        addi    r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_LAWBAR6))

        lis     r7,HIADJ(CCSR_LAWBARn_INVALID)
        addi    r7,r7,LO(CCSR_LAWBARn_INVALID)
        stw     r7,0x0(r6)

        lis     r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_LAWAR6))
        addi    r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_LAWAR6))

        lis     r7,HIADJ (CCSR_LAWARn_INVALID)
        addi    r7,r7,LO (CCSR_LAWARn_INVALID)
        stw     r7,0x0(r6)
        isync
        sync
        eieio

do_lawbar7:

        /* Set 7 */

        lis     r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_LAWBAR7))
        addi    r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_LAWBAR7))

        lis     r7,HIADJ(CCSR_LAWBAR7_PROG)
        addi    r7,r7,LO(CCSR_LAWBAR7_PROG)
        stw     r7,0x0(r6)

        lis     r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_LAWAR7))
        addi    r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_LAWAR7))

        lis     r7,HIADJ (CCSR_LAWAR7_PROG)
        addi    r7,r7,LO (CCSR_LAWAR7_PROG)
        stw     r7,0x0(r6)
        isync
        sync
        eieio

        /* Now program chip select 0 (BR0/OR0) */

        lis     r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_BR0))
        addi    r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_BR0))
        lis     r7,HIADJ(CCSR_BR0_PROG)
        addi    r7,r7,LO(CCSR_BR0_PROG)
        stw     r7,0x0(r6)

        lis     r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_OR0))
        addi    r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_OR0))
        lis     r7,HIADJ(CCSR_OR0_PROG)
        addi    r7,r7,LO(CCSR_OR0_PROG)
        stw     r7,0x0(r6)

        /* Now for chip select 1 (BR1/OR1) */

        lis     r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_BR1))
        addi    r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_BR1))
        lis     r7,HIADJ(CCSR_BR1_PROG)
        addi    r7,r7,LO(CCSR_BR1_PROG)
        stw     r7,0x0(r6)

        lis     r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_OR1))
        addi    r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_OR1))
        lis     r7,HIADJ(CCSR_OR1_PROG)
        addi    r7,r7,LO(CCSR_OR1_PROG)
        stw     r7,0x0(r6)

        /* Now for chip select 2 (BR2/OR2) */

        lis     r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_BR2))
        addi    r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_BR2))
        lis     r7,HIADJ(CCSR_BR2_PROG)
        addi    r7,r7,LO(CCSR_BR2_PROG)
        stw     r7,0x0(r6)

        lis     r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_OR2))
        addi    r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_OR2))
        lis     r7,HIADJ(CCSR_OR2_PROG)
        addi    r7,r7,LO(CCSR_OR2_PROG)
        stw     r7,0x0(r6)

        /* Now for chip select 3 (BR3/OR3) */

        lis     r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_BR3))
        addi    r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_BR3))
        lis     r7,HIADJ(CCSR_BR3_PROG)
        addi    r7,r7,LO(CCSR_BR3_PROG)
        stw     r7,0x0(r6)

        lis     r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_OR3))
        addi    r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_OR3))
        lis     r7,HIADJ(CCSR_OR3_PROG)
        addi    r7,r7,LO(CCSR_OR3_PROG)
        stw     r7,0x0(r6)

        /* Now for chip select 4 (BR4/OR4) */

        lis     r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_BR4))
        addi    r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_BR4))
        lis     r7,HIADJ(CCSR_BR4_PROG)
        addi    r7,r7,LO(CCSR_BR4_PROG)
        stw     r7,0x0(r6)

        lis     r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_OR4))
        addi    r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_OR4))
        lis     r7,HIADJ(CCSR_OR4_PROG)
        addi    r7,r7,LO(CCSR_OR4_PROG)
        stw     r7,0x0(r6)

        /* Now for chip select 5 (BR5/OR5) */

        lis     r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_BR5))
        addi    r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_BR5))
        lis     r7,HIADJ(CCSR_BR5_PROG)
        addi    r7,r7,LO(CCSR_BR5_PROG)
        stw     r7,0x0(r6)

        lis     r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_OR5))
        addi    r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_OR5))
        lis     r7,HIADJ(CCSR_OR5_PROG)
        addi    r7,r7,LO(CCSR_OR5_PROG)
        stw     r7,0x0(r6)

        /* Now for chip select 6 (BR6/OR6) */

        lis     r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_BR6))
        addi    r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_BR6))
        lis     r7,HIADJ(CCSR_BR6_PROG)
        addi    r7,r7,LO(CCSR_BR6_PROG)
        stw     r7,0x0(r6)

        lis     r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_OR6))
        addi    r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_OR6))
        lis     r7,HIADJ(CCSR_OR6_PROG)
        addi    r7,r7,LO(CCSR_OR6_PROG)
        stw     r7,0x0(r6)

        /* Now for chip select 7 (BR7/OR7) */

        lis     r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_BR7))
        addi    r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_BR7))
        lis     r7,HIADJ(CCSR_BR7_PROG)
        addi    r7,r7,LO(CCSR_BR7_PROG)
        stw     r7,0x0(r6)

        lis     r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_OR7))
        addi    r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_OR7))
        lis     r7,HIADJ(CCSR_OR7_PROG)
        addi    r7,r7,LO(CCSR_OR7_PROG)
        stw     r7,0x0(r6)

        /* Initialise the Local Bus Controller */

        li      r4,0x2000
        mtctr   r4

        lis     r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_LCRR))
        addi    r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_LCRR))
        lis     r7,HIADJ(0x80000008)
        addi    r7,r7,LO(0x80000008)
        stw     r7,0x0(r6)

        isync
        sync
        eieio
dllDelay4:
        nop
        bdnz    dllDelay4

#ifdef INCLUDE_DDR_SDRAM

        /*
        * In order to program the memory controller we need to have
        * access to some sort of memory in order for our C routine,
        * to compute and then store our memory controller parameters.
        * Since the memory controller controls DRAM, and since we are
        * initializing the controller, we cannot use DRAM for this, we
        * must use some other memory.  The memory that we will use is
        * the L1 cache in write-back mode.  All references within this
        * space will not be flushed out to DRAM.  We accomplish this in
        * the following manner:
        *
        * 1. Program the data BATs for memory management.  We
        *    must have some type of memory management operating
        *    if we are going to use data cache in copyback mode.
        *    Use the DBAT0 to access the first 256MB of system
        *    memory (cacheing allowed) and DBAT1 to access the MV64260
        *    registers and FLASH bank A (cache inhibited).
        *    All other DBATs are disabled (2 through 7).
        *    This is necessary in order to configure the controller.  Once
        *    the controller is programmed we no longer need the DBATs.
        *
        * 2. Invalidate the data cache, instruction cache, and TLB entries.
        *
        * 4. Enable the data MMU.
        *
        * 5. Turn on the data cache and instruction cache.
        *
        * 6. Zero out some of the L1 data cache in the address range
        *    occupied by the stack so that the cache tag entires are
        *    present and all future stack access hit the L1 cache and
        *    don't go out to DRAM.
        */

        bl      setDbats

        /* DBAT contents arranged DBAT0L, DBAT0H, DBAT1L, ... */

        /* first 256 MB of RAM starting at 0x00000000 */

        .long   ((0x00000000    & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |\
                _MMU_LBAT_GUARDED)
        .long   ((0x00000000    &_MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_256M |\
                _MMU_UBAT_VS | _MMU_UBAT_VP)

        /* 256 MB FLASH  */

        .long   ((0xF0000000    & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |\
                _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED)
        .long   ((0xF0000000    & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_256M |\
                _MMU_UBAT_VS | _MMU_UBAT_VP)

        /* 256 MB 8641 Internal regs */

        .long   ((0xE0000000    & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |\
                _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED)
        .long   ((0xE0000000    & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_256M |\
                _MMU_UBAT_VS | _MMU_UBAT_VP)

        /* Disable DBAT3 */

        .long   (0x00000000)
        .long   (0x00000000)

        /*
         * DBATs 4 - 7
         *
         * It is not necessary to explicitly disable DBATs 4 through 7.  At
         * power-up these DBATs are disabled - HID0[HIGH_BAT_EN] = 0 by
         * default.
         */

        /*
         * load DBATs from table above
         *
         * Synchronization requirements: A DSSALL and sync must precede the
         * mtspr and then a sync and isync must follow.
         */

setDbats:
        DSSALL
        sync
        mfspr   r4,LR
        addi    r4,r4,-4
        lwzu    r3,4(r4)
        mtspr   DBAT0L,r3
        lwzu    r3,4(r4)
        mtspr   DBAT0U,r3
        lwzu    r3,4(r4)
        mtspr   DBAT1L,r3
        lwzu    r3,4(r4)
        mtspr   DBAT1U,r3
        lwzu    r3,4(r4)
        mtspr   DBAT2L,r3
        lwzu    r3,4(r4)
        mtspr   DBAT2U,r3
        lwzu    r3,4(r4)
        mtspr   DBAT3L,r3
        lwzu    r3,4(r4)
        mtspr   DBAT3U,r3
        sync
        isync

        /*
         * Zero out the SDR1 register.  We do this since we do not wish to
         * use the "page table" part of the MMU, but rather the DBATs.
         *
         * Synchronization requirements: A DSSALL and sync must precede a
         * mtspr and then a sync and isync must follow.
         */

        xor     r0,r0,r0
        DSSALL
        sync
        mtspr   SDR1,r0       /* SDR1 - Address of Page Table */
        sync
        isync

        /* invalidate entries within both Translation Lookaside Buffers */

        bl      tlbInval

        /*
         * Now that the MMU has been configured we can enable the data
         * MMU by turning on data address translation.
         *
         * Synchronization requirements: A DSSALL and sync must precede
         * a mtmsr and then a sync and isync must follow.
         */

        mfmsr   r3
        lis     r4,HI(1<<(31-_PPC_MSR_BIT_DR))     /* MSR[DR] = 1 */
        ori     r4,r4,LO(1<<(31-_PPC_MSR_BIT_DR))
        or      r4,r3,r4

        /* DSSALL */
        sync

        mtmsr   r4
        sync
        isync

        /* Turn on data cache */

        bl      dCacheOn

        /* Turn on Instruction Cache */

        bl      iCacheOn

        /* initialize the stack pointer */

        lis     sp, HI(CACHE_STACK_ADRS)
        ori     sp, sp, LO(CACHE_STACK_ADRS)

        /* Initialize some memory in the cache stack */

        lis     r3,HI(CACHE_STACK_SIZE)
        ori     r3,r3,LO(CACHE_STACK_SIZE)
        lis     r4,HI(CACHE_STACK_ADRS+_CACHE_ALIGN_SIZE)
        ori     r4,r4,LO(CACHE_STACK_ADRS+_CACHE_ALIGN_SIZE)
        subf    r3,r3,r4

fillLoop:
        dcbz    r0,r3
        addi    r3,r3,_CACHE_ALIGN_SIZE
        cmplw   r3,r4
        blt     fillLoop

        xor     r0,r0,r0                     /* insure r0 is zero */

        /*
         * The second UART scratch register is used to track if the
         * I2C driver should use locking or not.  It should be clear
         * to start for no locking.
         */

        lis     r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_USCR1))
        addi    r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_USCR1))
        stb     r0,0x0(r6)

        /*
         *Store the start type off at the end of the space we reserved
         * for the stack.  Use one of the DUART scratch registers as
         * temporary storage for it
         */

        lis     r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_USCR0))
        addi    r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_USCR0))
        stb     r11,0x0(r6)

        /* Program the MV64260 System Memory Controller */

        bl      sysMpc8641SmcInitialize
        or      r15,r3,r3                   /* save memory size */

        /* Retrieve the start type */

        lis     r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_USCR0))
        addi    r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_USCR0))
        lbz     r11,0x0(r6)

        /*
         * We have been using DBATs up until now.  We will disable the
         * DBATs by turning off address translation.  First we must disable
         * the data cache.  Once the data MMU is disabled we will enable the
         * data cache once again.  The instruction cache is still enabled.  Then
         * we will scrub the DRAM before disabling both the data and instruction
         * caches - the caches will be disabled when we leave romInit.
         */

        bl      dCacheOff     /* Disable the data cache */

        /* Disable the data MMU */

        mfmsr   r3
        rlwinm  r3,r3,0,_PPC_MSR_BIT_DR+1,_PPC_MSR_BIT_DR-1  /* MSR[DR] = 0 */
        DSSALL
        sync
        mtmsr   r3
        sync
        isync

#ifdef INCLUDE_ECC

        /*
         * Turn the data cache back on for the DRAM scrub
         *
         * This will reduce the time to scrub DRAM.
         */

        bl      dCacheOn

        /*
         * Scrub memory so that it is in a known state for error
         * correction/detection.
         */

        xor     r3,r3,r3                    /* memory starting address = 0 */
        add     r4,r3,r15                   /* memory ending address + 1 */
        bl      memoryScrub                 /* scrub system memory */

        bl      dCacheOff

#endif /* INCLUDE_ECC */

#endif /* INCLUDE_DDR_SDRAM */

#ifdef  RBV_ADRS

        /*
         * Check for the sentinel value from sysToMonitor, which
         * if found indicates that we are really doing a warm boot
         * even though we had to reset the board -- and therefore
         * reinitialize the hardware -- to get here.  This has to be
         * done even in non-SMP builds, because a non-SMP bootrom
         * should work with an SMP vxWorks.  The unrelocated absolute
         * address is correct, because it is not related to where
         * this code will be located.
         */

        lis     r6, HIADJ(RBV_ADRS)
        addi    r6, r6, LO(RBV_ADRS)
        lwz     r7, OFF_RBV_MAGIC1(r6)
        lis     r3, HI(_WRS_BOOT_MAGIC_1)
        ori     r3, r3, LO(_WRS_BOOT_MAGIC_1)
        cmpw    r3, r7
        bne     clrSA
        lwz     r7, OFF_RBV_MAGIC2(r6)
        lis     r3, HI(_WRS_BOOT_MAGIC_2)
        ori     r3, r3, LO(_WRS_BOOT_MAGIC_2)
        cmpw    r3, r7
        bne     clrSA
        lwz     r7, OFF_RBV_MAGIC3(r6)
        lis     r3, HI(_WRS_BOOT_MAGIC_3)
        ori     r3, r3, LO(_WRS_BOOT_MAGIC_3)
        cmpw    r3, r7
        bne     clrSA
        lwz     r11, OFF_RBV_STARTTYPE(r6)

clrSA:

        /* Clear the sentinel area */

        stw     r0, OFF_RBV_STARTTYPE(r6)
        stw     r0, OFF_RBV_MAGIC1(r6)
        stw     r0, OFF_RBV_MAGIC2(r6)
        stw     r0, OFF_RBV_MAGIC3(r6)
#endif  /* RBV_ADRS */

        /* Turn off the LED  */

        lis     r6,HIADJ(0xf2000002)
        addi    r6,r6,LO(0xf2000002)
        lis     r7,HIADJ(0x00000000)
        addi    r7,r7,LO(0x00000000)
        stb     r7,0x0(r6)

startCSetup:

        /* go to C entry point */

        or      r3, r11, r11            /* put startType in r3 (p0) */
        addi    sp, sp, -FRAMEBASESZ    /* save one frame stack */

        LOADPTR (r6, romStart)
        LOADPTR (r7, romInit)
        LOADPTR (r8, ROM_TEXT_ADRS)

        sub     r6, r6, r7
        add     r6, r6, r8

        mtlr    r6              /* romStart - romInit + ROM_TEXT_ADRS */
        blr
FUNC_END(romInit)


/*******************************************************************************
*
* tlbInval - Invalidate the Translation Lookaside Buffers
*
* The MPC7455 implements separate 128-entry data and instruction TLBs to
* maximize performance.  Each TLB contains 128 entries organized as a two-way
* set-associative array with 64 sets (64 indexes).  Each tlbie instruction
* invalidates four entries, two for ITLB and two for DTLB.  Since there are
* 64 indexes, we must execute the tlbie instruction 64 times, incrementing
* the index value by one each time, in order to invalidate both TLBs.
* According to page 2-92 of the MPC7450 RISC Microprocessor Family User's
* Manual, a tlbsync instruction must follow the sequence of 64 tlbie
* instructions.
*
* RETURNS: N/A.
*/

FUNC_BEGIN(tlbInval)
        li      r4,128
        xor     r3,r3,r3           /* p0 = 0 */
        mtctr   r4                 /* CTR = 64 */

        isync                      /* context sync req'd before tlbie */
tlbLoop:
        tlbie   r3
        sync                       /* sync instr req'd after tlbie */
        addi    r3,r3,0x1000       /* increment bits 14-19 */
        bdnz    tlbLoop            /* decrement CTR, branch if CTR != 0 */
        tlbsync

        bclr    0x14,0x0           /* return to caller */
FUNC_END(tlbInval)

/*******************************************************************************
*
* iCacheOn - Turn Instruction Cache On
*
* This routine enables the instruction cache by setting the Instruction
* Cache Enable (ICE) bit in HID0.  It also flash invalidates the instruction
* cache, via the Instruction Cache Flash Invalidate (ICFI) bit in HID0.
*
* RETURNS: N/A.
*/

FUNC_BEGIN(_iCacheOn)
FUNC_BEGIN(iCacheOn)
        mfspr   r4, HID0                             /* r4 = HID0 */
        ori     r4,r4,(_PPC_HID0_ICE+_PPC_HID0_ICFI) /* Set ICE & ICFI */
        isync
        mtspr   HID0, r4                             /* Enable Instr Cache & Inval cache */
        isync

        rlwinm  r3,r4,0,(_PPC_HID0_BIT_ICFI+1),(_PPC_HID0_BIT_ICFI-1) /* Clear */
                                                                      /* ICFI */
        mtspr   HID0, r3
        isync

        bclr    0x14,0x0               /* return to caller */
FUNC_END(_iCacheOn)
FUNC_END(iCacheOn)

/*******************************************************************************
*
* dCacheOn - Turn Data Cache On
*
* This routine enables the data cache by setting the Data Cache Enable (DCE)
* bit in HID0.  It also flash invalidates the data cache, via the Data Cache
* Flash Invalidate (DCFI) bit in HID0.
*
* RETURNS: N/A.
*/

FUNC_BEGIN(_dCacheOn)
FUNC_BEGIN(dCacheOn)
        mfspr   r4,HID0                /* r4 = HID0 */
        ori     r4,r4,(_PPC_HID0_DCE + _PPC_HID0_DCFI) /* Set DCE & DCFI */
        DSSALL                         /* required before changing DCE or DCFI */
        sync
        mtspr   HID0,r4                /* Enable data cache & invalidate cache */
        sync                           /* required after changing DCE or DCFI */
        isync

        rlwinm  r3,r4,0,(_PPC_HID0_BIT_DCFI+1),(_PPC_HID0_BIT_DCFI-1) /* Clear */
                                                                      /* DCFI */
        DSSALL                         /* required before changing DCE or DCFI */
        sync
        mtspr   HID0, r3               /* Clear data cache invalidate bit */
        sync                           /* required after changing DCE or DCFI */
        isync

        bclr    0x14,0x0               /* return to caller */
FUNC_END(_dCacheOn)
FUNC_END(dCacheOn)

/*******************************************************************************
*
* dCacheOff - Turn Data Cache Off
*
* This routine disables the data cache by unsetting the Data Cache Enable (DCE)
* bit in HID0.  It also flash invalidates the data cache, via the Data Cache
* Flash Invalidate (DCFI) bit in HID0, prior to disabling the data cache.
*
*
* RETURNS: N/A.
*/

FUNC_BEGIN(_dCacheOff)
FUNC_BEGIN(dCacheOff)
        mfspr   r4,HID0                /* r4 = HID0 */
        ori     r4,r4,(_PPC_HID0_DCE + _PPC_HID0_DCFI) /* Set DCE & DCFI */
        DSSALL                         /* required before changing DCE or DCFI */
        sync
        mtspr   HID0,r4                /* Enable data cache & invalidate cache */
        sync                           /* required after changing DCE or DCFI */
        isync

        rlwinm  r4,r4,0,_PPC_HID0_BIT_DCE+1,_PPC_HID0_BIT_DCE-1    /* clear DCE */
        rlwinm  r3,r4,0,(_PPC_HID0_BIT_DCFI+1),(_PPC_HID0_BIT_DCFI-1) /* clear */
                                                                      /* DCFI */
        DSSALL                         /* required before changing DCE or DCFI */
        sync
        mtspr   HID0,r4                /* Disable data cache, clear DCFI */
        sync                           /* required after changing DCE or DCFI */
        isync

        addis   r5,r0,0xFFFF           /* r5 = mask for MSSCR0[L2PFE] bits */
        ori     r5,r5,0xFFFC
        mfspr   r4,MSSCR0              /* r4 = MSSCR0 */
        and     r4,r4,r5               /* r4 = MSSCR0 with L2PFE bits cleared */
        DSSALL                         /* required before changing L2PFE bits */
        sync
        mtspr   MSSCR0,r4              /* clear L2PFE bits */
        sync                           /* required after changing L2PFE bits */
        isync

        bclr    0x14,0x0               /* return to caller */
FUNC_END(_dCacheOff)
FUNC_END(dCacheOff)

/*******************************************************************************
*
* memoryScrub - DRAM initialization.
*
* This routine's purpose is to initialize (i.e., scrub) DRAM, the MV64260
* protects DRAM by utilizing ECC, so the scrub insures that the entire
* DRAM array's check bits are initialized to a known state.
*
* ARGUMENTS:
* r3 = starting address of DRAM
* r4 = ending address of DRAM (plus 1)
*
* RETURNS: N/A
*/

FUNC_BEGIN(memoryScrub)
        mfspr   r16,LR                  /* save return instruction pointer */
        or      r13,r3,r3               /* save argument #1, start address */
        or      r14,r4,r4               /* save argument #2, end address */
        xor     r0,r0,r0                /* clear r0 */

        /*
         * Enable floating point support in the processor so that
         * we can scrub memory using 64 bit reads and writes.
         */

        mfmsr   r3
        lis     r4,HI(1<<(31-_PPC_MSR_BIT_FP))
        ori     r4,r4,LO(1<<(31-_PPC_MSR_BIT_FP))
        or      r4,r3,r4
        DSSALL
        sync
        mtmsr   r4
        sync
        isync

        bl      loadScrubData

        .long   0x00000000,0x00000000

loadScrubData:

        mfspr   r4,LR                   /* address of the above table of 0's */
        lfd     0,0(r4)                 /* load the 0's into fpr0 */


        /* Setup scrub loop specifics */

        subf    r18,r13,r14             /* calculate number of bytes */
        rlwinm  r18,r18,29,3,31         /* calculate number of doubles */
        addi    r17,r13,-8              /* starting address munged */
        mtspr   CTR,r18                 /* load number of doubles/words */

        /* Loop through the entire DRAM array, initialize memory */

scrubLoop:

        stfdu   0,8(r17)                /* write contents of fpr0 to memory */
        bc      16,0,scrubLoop          /* branch until counter == 0 */

        sync

        lis     r18,HI(0x8000/_CACHE_ALIGN_SIZE)    /* load number of cache */
        ori     r18,r18,(0x8000/_CACHE_ALIGN_SIZE)  /* lines in 32KB */
        mtspr   CTR,r18                             /* move to counter */

readLoop:

        /*
         * Flush L1 data cache to ensure that all data has been flushed
         * from the cache and has been written to memory.
         */

        dcbf    r0,r17                       /* flush line */
        addi    r17,r17,-(_CACHE_ALIGN_SIZE) /* next (previous) line */
        bc      16,0,readLoop                /* branch until counter == 0 */

scrubExit:

        /* Disable floating point support in the processor before exiting */

        mfmsr   r3
        rlwinm  r3,r3,0,_PPC_MSR_BIT_FP+1,_PPC_MSR_BIT_FP-1
        DSSALL
        sync
        mtmsr   r3
        sync
        isync

        mtspr   LR,r16                  /* restore return instruction pointer */
        bclr    0x14,0x0                /* return to caller */
FUNC_END(memoryScrub)

