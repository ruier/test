/* romInit.s - Freescale MPC8308_RDB ROM initialization module */

/*
 * Copyright (c) 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,16jun10,syt  derived from version 01b of fsl_rdb8313/romInit.s
*/

/*
DESCRIPTION
This module contains the entry code for the VxWorks bootrom.
The entry point romInit, is the first code executed on power-up.
It sets the BOOT_COLD parameter to be passed to the generic
romStart() routine.

The routine sysToMonitor() jumps to the location 8 bytes
past the beginning of romInit, to perform a "warm boot".
This entry point allows a parameter to be passed to romStart().

*/

#define _ASMLANGUAGE
#include <vxWorks.h>
#include <asm.h>
#include <cacheLib.h>
#include "config.h"
#include <regs.h>
#include <sysLib.h>

#define WRITEADR(reg1,reg2,addr32,val) \
    lis reg1, HI(addr32); \
    ori reg1, reg1, LO(addr32); \
    lis reg2, HI(val); \
    ori reg2, reg2, LO(val); \
    stw reg2, 0(reg1)

#define WRITEOFFSET(regbase,reg2,offset,val) \
    lis reg2, HI(val); \
    ori reg2, reg2, LO(val); \
    stw reg2, offset(regbase);

/* internals */

FUNC_EXPORT(_romInit)       /* start of system code */
FUNC_EXPORT(romInit)        /* start of system code */

    /* externals */

    .extern romStart        /* system initialization routine */

    .text
/*
 * Hard Reset Configuration Word (HRCW) See config.h/target.ref
 * for more info
 */
    .fill   8, 1, HRCW_LOW_BYTE0
    .fill   8, 1, HRCW_LOW_BYTE1
    .fill   8, 1, HRCW_LOW_BYTE2
    .fill   8, 1, HRCW_LOW_BYTE3
    .fill   8, 1, HRCW_HIGH_BYTE0
    .fill   8, 1, HRCW_HIGH_BYTE1
    .fill   8, 1, HRCW_HIGH_BYTE2
    .fill   8, 1, HRCW_HIGH_BYTE3

    .fill   192, 1, 0   /* The rest of the space are filled with zeros */

/*******************************************************************************
*
* romInit - entry point for VxWorks in ROM
*
*
* romInit
*     (
*     int startType /@ only used by 2nd entry point @/
*     )
*
*/

_romInit:
romInit:
    bl  cold            /* cold boot initialization */
    nop
    bl  warm            /* warm boot initialization */

    /* copyright notice appears at beginning of ROM (in TEXT segment) */

    .ascii   "Copyright 2010 Wind River Systems, Inc."
    .align 2

cold:
    li  r3, BOOT_COLD   /* set cold boot as start type */

    /*
     * initialize the IMMR register before any non-core registers
     * modification. The default IMMR base address was 0xFF400000,
     * as originally programmed in the Hard Reset Configuration Word.
     */

    lis r4, HI (IMMR_BASE)
    ori r4, r4, LO (IMMR_BASE)

    lis r8, HIADJ(IMMR_INIT)    /* IMMR was at 0xff400000 */
    ori r8, r8, LO(IMMR_INIT)   /* IMMR now at IMMR */
    stw r4, 0(r8)
    isync
    sync

    /*
     * Set HID2[10] to 1, SPCR[14] to 1 and ACR[10:11] to 01/10,
     * immediately after boot up depending on CSB/CPU clk ratio
     *
     * HID2 - 0x00200000
     * SPCR - 0x00020000
     * ACR  - 0x00100000
     */

    lis r5, 0x0004
    sync
    isync
    mtspr   1011, r5            /* HID2 */
    sync
    isync

    /* Remap Nor Flash bass address to 0xfe000000 */

    lis  r5, HIADJ(ELBC_OR0)
    addi r5, r5, LO(ELBC_OR0)
    lwz  r6, 0(r5)
    ori  r6, r6, 0x0ff7
    stw  r6, 0(r5)
    isync                       /* synchronize */

    WRITEADR(r6, r7, LBLAWBARn(1), ROM_BASE_ADRS)
    WRITEADR(r6, r7, LBLAWARn(1), (LAWAR_ENABLE | LAWAR_SIZE_8MB))

    /*
     * When the PowerPC 83xx is powered on, the processor fetches the
     * instructions located at the address 0x100. We need to jump
     * from the address 0x100 to the Flash space.
     */

    lis     r4, HIADJ(start)    /* load r4 with the address */
    addi    r4, r4, LO(start)   /* of start */

    lis     r5, HIADJ(romInit)  /* load r5 with the address */
    addi    r5, r5, LO(romInit) /* of romInit() */

    lis     r6, HIADJ(ROM_TEXT_ADRS)    /* load r6 with the address */
    addi    r6, r6, LO(ROM_TEXT_ADRS)   /* of ROM_TEXT_ADRS */

    sub     r4, r4, r5
    add     r4, r4, r6

    mtspr   LR, r4              /* save destination address*/
                                /* into LR register */
    blr                         /* jump to flash mem address */
start:

    /* set the MSR register to a known state */

    xor r0, r0, r0              /* clear register R0 */
    isync                       /* synchronize */
    mtmsr   r0                  /* clear the MSR register */
    sync
    isync                       /* synchronize */

    /* Configure Local Bus Local Access Windows */

    /* WINDOW 0 - NOR FLASH */

    WRITEADR(r6, r7, ELBC_BR0, (ROM_BASE_ADRS | 0x1001))
    WRITEADR(r6, r7, ELBC_OR0, (OR_SIZE_8M | 0x0ff7))

    WRITEADR(r6, r7, LBLAWBARn(0), ROM_BASE_ADRS)
    WRITEADR(r6, r7, LBLAWARn(0), (LAWAR_ENABLE | LAWAR_SIZE_8MB))

    /* WINDOW 1 - NAND FLASH */

    WRITEADR(r6, r7, LBLAWBARn(1), NAND_BUFFER_BASE)
    WRITEADR(r6, r7, LBLAWARn(1), (LAWAR_ENABLE | LAWAR_SIZE_32KB))

    WRITEADR(r6, r7, ELBC_BR1, (NAND_BUFFER_BASE | 0x0C21))
    WRITEADR(r6, r7, ELBC_OR1, (0xFFFF8396))

    /* WINDOW 2 - VSC7385 */

    WRITEADR(r6, r7, LBLAWBARn(2), VSC7385_BASE)
    WRITEADR(r6, r7, LBLAWARn(2), (LAWAR_ENABLE | LAWAR_SIZE_128KB))

    WRITEADR(r6, r7, ELBC_BR2, 0xF0000801)
    WRITEADR(r6, r7, ELBC_OR2, 0xFFFE09FF)

    isync
    sync

    /* Set the system clock to eLBC bus frequency ratio (2:1) */

    WRITEADR(r6, r7, ELBC_LCRR, 0x80000002)
    isync

    li      r6, 0x2000
    mtctr   r6
lbcdelay:
    nop
    bdnz    lbcdelay
    nop
    isync
    sync

    /* Zero-out HID0 register */

    mtspr   HID0, r0
    isync

    /* Zero-out registers: SPRGs */

    addis    r0, 0, 0
    isync                       /* synchronize */

    mtspr   272, r0
    mtspr   273, r0
    mtspr   274, r0
    mtspr   275, r0
    isync                       /* synchronize */

    /* zero-out the Segment registers */

    mtsr    0,r0
    isync
    mtsr    1,r0
    isync
    mtsr    2,r0
    isync
    mtsr    3,r0
    isync
    mtsr    4,r0
    isync
    mtsr    5,r0
    isync
    mtsr    6,r0
    isync
    mtsr    7,r0
    isync
    mtsr    8,r0
    isync
    mtsr    9,r0
    isync
    mtsr    10,r0
    isync
    mtsr    11,r0
    isync
    mtsr    12,r0
    isync
    mtsr    13,r0
    isync
    mtsr    14,r0
    isync
    mtsr    15,r0
    isync

    /* invalidate DBATs: clear VP and VS bits */

    mtspr   536,r0              /* Data bat register 0 upper */
    isync
    mtspr   538,r0              /* Data bat register 1 upper */
    isync
    mtspr   540,r0              /* Data bat register 2 upper */
    isync
    mtspr   542,r0              /* Data bat register 3 upper */
    isync
    mtspr   0x238,r0            /* Data bat register 4 upper */
    isync
    mtspr   0x23A,r0            /* Data bat register 5 upper */
    isync
    mtspr   0x23C,r0            /* Data bat register 6 upper */
    isync
    mtspr   0x23E,r0            /* Data bat register 7 upper */
    isync

    /* invalidate IBATs: clear VP and VS bits */

    mtspr   528,r0              /* Instruction bat register 0 upper */
    isync
    mtspr   530,r0              /* Instruction bat register 1 upper */
    isync
    mtspr   532,r0              /* Instruction bat register 2 upper */
    isync
    mtspr   534,r0              /* Instruction bat register 3 upper */
    isync
    mtspr   0x230,r0            /* Instruction bat register 4 upper */
    isync
    mtspr   0x232,r0            /* Instruction bat register 5 upper */
    isync
    mtspr   0x234,r0            /* Instruction bat register 6 upper */
    isync
    mtspr   0x236,r0            /* Instruction bat register 7 upper */
    isync

    /* invalidate TLBs: loop on all TLB entries using r7 as an index */

    addi     r0, 0, 64
    mtspr    9,  r0             /* Load CTR with 32 */
    addi     r7, 0, 0           /* Use r7 as the tlb index */

tlb_write_loop:

    tlbie    r7                 /* invalidate the tlb entry */
    sync
    addi     r7,r7,0x1000       /* increment the index */
    bdnz tlb_write_loop         /* Decrement CTR, then branch if the */
                                /* decremented CTR is not equal to 0 */

    /* Turn off data and instruction cache control bits */

    mfspr   r7, HID0
    isync
    sync                        /* synchronize */
    andi.   r7, r7, 0x3FFF      /* Clear DCE and ICE bits */
    mtspr   HID0,r7
    isync
    sync                        /* synchronize */

    /* Memory mapped region base address */

    WRITEADR(r6, r7, DDRLAWBARn(0), LOCAL_MEM_LOCAL_ADRS)
    WRITEADR(r6, r7, DDRLAWARn(0), (LAWAR_ENABLE | LAWAR_SIZE_128MB))
    WRITEADR(r6, r7, DDRCDR, 0x73000002)

    /* Initialize the DDR Memory controller */

    lis r6, HI(DDR_REG_BASE)
    ori r6, r6, LO(DDR_REG_BASE)       /* r6 = DDR memory controller base */

    WRITEOFFSET(r6, r7, (CS0_BNDS), 0x00000007)
    WRITEOFFSET(r6, r7, (CS0_CONFIG), 0x80840102)

    /* currently we use only one CS, so disable the other banks */

    WRITEOFFSET(r6, r7, (CS1_BNDS), 0x0)
    WRITEOFFSET(r6, r7, (CS1_CONFIG), 0x0)

    WRITEOFFSET(r6, r7, (DDR_SDRAM_CLK_CNTRL), 0x02000000)
    WRITEOFFSET(r6, r7, (TIMING_CFG_3), 0x00000000)
    WRITEOFFSET(r6, r7, (TIMING_CFG_0), 0x00220802)
    WRITEOFFSET(r6, r7, (TIMING_CFG_1), 0x3935d322)
    WRITEOFFSET(r6, r7, (TIMING_CFG_2), 0x0f9048ca)


    WRITEOFFSET(r6, r7, (DDR_SDRAM_CFG), 0x43080000)
    WRITEOFFSET(r6, r7, (DDR_SDRAM_CFG_2), 0x00401000)
    WRITEOFFSET(r6, r7, (DDR_SDRAM_MODE_CFG), 0x44400432)
    WRITEOFFSET(r6, r7, (DDR_SDRAM_MODE_CFG_2), 0x8000C000)
    WRITEOFFSET(r6, r7, (DDR_SDRAM_INTERVAL), 0x03200064)

    li  r5, 0x7000
    mtctr r5
delayddr1:
    nop
    nop
    bdnz delayddr1
    isync

    /* enable DDR controller */

    WRITEOFFSET(r6, r7, DDR_SDRAM_CFG, 0xC3080000)

    /* Local Bus setup lbcr and mrtpr */

    WRITEADR(r6, r7, ELBC_LBCR, 0x0000ff0f)

    WRITEADR(r6, r7, M83XX_OCCR, 0xffffffff)
    isync

warm:

    /* turn the instruction cache ON for faster FLASH ROM boots */

    mfspr   r4, HID0
    ori r4, r4, (_PPC_HID0_ICE | _PPC_HID0_ICFI)    /* set ICE & ICFI */
    rlwinm  r5, r4, 0, (_PPC_HID0_BIT_ICFI + 1), (_PPC_HID0_BIT_ICFI - 1)
                                                    /* clear the ICFI bit */
    isync

    /*
     * The setting of the instruction cache enable (ICE) bit must be
     * preceded by an isync instruction to prevent the cache from being
     * enabled or disabled while an instruction access is in progress.
     */

    mtspr   HID0, r4            /* Enable Instr Cache & Inval cache */
    sync
    mtspr   HID0, r5            /* using 2 consec instructions */
                                /* E300 core recommendation */
    isync

    /* initialize the stack pointer */

    lis sp, HIADJ(STACK_ADRS)
    addi sp, sp, LO(STACK_ADRS)

    /* go to C entry point */

    addi sp, sp, -FRAMEBASESZ        /* get frame stack */

    /*
     * calculate C entry point: routine - entry point + ROM base
     * routine  = romStart
     * entry point  = romInit   = R7
     * ROM base = ROM_TEXT_ADRS = R8
     * C entry point: romStart - R7 + R8
     */

    lis  r7, HIADJ(romInit)
    addi r7, r7, LO(romInit)

    lis  r8, HIADJ(ROM_TEXT_ADRS)
    addi r8, r8, LO(ROM_TEXT_ADRS)

    lis r6, HIADJ(romStart)
    addi r6, r6, LO(romStart)   /* load R6 with C entry point */

    sub r6, r6, r7              /* routine - entry point */
    add r6, r6, r8              /* + ROM base */

    mtspr   LR, r6              /* save destination address*/
                                /* into LR register */
    blr                         /* jump to the C entry point */
