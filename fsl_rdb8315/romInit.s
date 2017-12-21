/* romInit.s - Freescale MPC8315ERDB ROM initialization module */

/*
 * Copyright (c) 2008-2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,11dec09,x_z  Code clean.
01a,09may08,z_l  Created, routines are derived from version 01b
                 of fsl_rdb8313/romInit.s
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

#define WRITEADR(reg1,reg2,addr32,val)     \
            lis    reg1, HI(addr32);       \
            ori    reg1, reg1, LO(addr32); \
            lis    reg2, HI(val);          \
            ori    reg2, reg2, LO(val);    \
            stw    reg2, 0(reg1)

#define WRITEOFFSET(regbase,reg2,offset,val) \
            lis    reg2, HI(val);            \
            ori    reg2, reg2, LO(val);      \
            stw    reg2, offset(regbase);

    /* internals */

    FUNC_EXPORT(_romInit)       /* start of system code */
    FUNC_EXPORT(romInit)        /* start of system code */

    /* externals */

    .extern romStart            /* system initialization routine */

    .text

    /*
     * Hard Reset Configuration Word (HRCW) See config.h/target.ref
     * for more info
     */

    .fill   8,1,HRCW_LOW_BYTE0
    .fill   8,1,HRCW_LOW_BYTE1
    .fill   8,1,HRCW_LOW_BYTE2
    .fill   8,1,HRCW_LOW_BYTE3
    .fill   8,1,HRCW_HIGH_BYTE0
    .fill   8,1,HRCW_HIGH_BYTE1
    .fill   8,1,HRCW_HIGH_BYTE2
    .fill   8,1,HRCW_HIGH_BYTE3

    .fill   192,1,0     /* The rest of the space are filled with zeros */

/*******************************************************************************
*
* romInit - entry point for VxWorks in ROM
*
*
* romInit
*     (
*     int startType    /@ only used by 2nd entry point @/
*     )

*/

_romInit:
romInit:

    bl      cold        /* jump to the cold boot initialization */
    nop
    bl      warm        /* jump to the warm boot initialization */

    /* copyright notice appears at beginning of ROM (in TEXT segment) */

    .ascii   "Copyright 2008 Wind River Systems, Inc."
    .align 2

cold:
    li      r3, BOOT_COLD   /* set cold boot as start type */

    /*
     * initialize the IMMR register before any non-core registers
     * modification. The default IMMR base address was 0xFF400000,
     * as originally programmed in the Hard Reset Configuration Word.
     */

#ifndef BOOT_FROM_UBOOT
    lis     r4, HI (CCSBAR)
    ori     r4, r4, LO (CCSBAR)

    lis     r8,HIADJ(CCSBAR_INIT)    /* IMMR was at 0xff400000 */
    ori     r8, r8, LO(CCSBAR_INIT)  /* IMMR now at CCSBAR */
    stw     r4,0(r8)
    isync
    sync
#endif /* BOOT_FROM_UBOOT */

    /* enable high BATs */

    lis     r5, 0x0004
    sync
    isync

    mtspr   HID2, r5
    sync
    isync

#ifndef BOOT_FROM_UBOOT

    /* configure LAW Bar 1 for FLASH and leave LAW Bar 0 reserved */

    WRITEADR(r6,r7,M83XX_LBLAWBARn(1), FLASH_BASE_ADRS)

    WRITEADR(r6,r7,M83XX_LBLAWARn(1), (LAWAR_ENABLE | LAWAR_SIZE_8MB))

    lis     r6, HI(M83XX_LBLAWARn(1))
    ori     r6, r6, LO(M83XX_LBLAWARn(1))
    lwz     r7, 0(r6)

    isync

#endif /* BOOT_FROM_UBOOT */

    /*
     * When the PowerPC 83xx is powered on, the processor fetches the
     * instructions located at the address 0x100. We need to jump
     * from the address 0x100 to the Flash space.
     */

    /* load r4 with the address of start */

    lis     r4, HIADJ(start)
    addi    r4, r4, LO(start)

    /* load r5 with the address of romInit() */

    lis     r5, HIADJ(romInit)
    addi    r5, r5, LO(romInit)

    /* load r6 with the address of ROM_TEXT_ADRS */

    lis     r6, HIADJ(ROM_TEXT_ADRS)
    addi    r6, r6, LO(ROM_TEXT_ADRS)

    sub     r4, r4, r5
    add     r4, r4, r6

    /* save destination address into LR register*/

    mtspr   LR, r4

    /* jump to flash mem address */

    blr
start:

    /* set the MSR register to a known state */

    xor     r0, r0, r0    /* clear register R0 */
    isync                /* synchronize */
    mtmsr   r0            /* clear the MSR register */
    sync
    isync                /* synchronize */

    /* BR0 - Port Size=2(16bit),DECC=0,WP=0,MSEL=0(GPCM),ATOM=0,V=1  */

    WRITEADR(r6,r7,QUICC_BR0(CCSBAR),FLASH_BASE_ADRS | 0x1001)

    /* OR0 - BCTLD=0,CSNT=0,ACS=0,XACS=1,SCY=9,SETA=0,TRLX=0,EHTR=1,EAD=1 */

    WRITEADR(r6,r7,QUICC_OR0(CCSBAR),FLASH_BASE_ADRS | 0x0193)

    /* LAW Bar 0 for FLASH */

    WRITEADR(r6,r7,M83XX_LBLAWBARn(0), FLASH_BASE_ADRS)
    WRITEADR(r6,r7,M83XX_LBLAWARn(0), (LAWAR_ENABLE | LAWAR_SIZE_8MB))

    lis     r6, HI(M83XX_LBLAWARn(0))
    ori     r6, r6,LO(M83XX_LBLAWARn(0))
    lwz     r7, 0(r6)

    /* invalidate LAW Bar 1 */

    WRITEADR(r6,r7,M83XX_LBLAWBARn(1), 0)

    WRITEADR(r6,r7,M83XX_LBLAWARn(1), 0)

    lis     r6, HI(M83XX_LBLAWARn(1))
    ori     r6, r6, LO(M83XX_LBLAWARn(1))
    lwz     r7, 0(r6)

    /* configure System I/O */

    WRITEADR(r6,r7, M83XX_SICRL,0x00000000)
    isync
    WRITEADR(r6,r7, M83XX_SICRH,0x00000000)
    isync

    /*
     * initialise the Local Bus Controller
     *
     * PBYP = 0 (PLL enabled)
     * EADC = 1 (number of cycles for the assertion of LALE)
     * CLKDIV = 2 (ratio between the system clock and the local bus clock )
     */

    WRITEADR(r6,r7,QUICC_LCRR(CCSBAR),0x00010002)

    isync

    li      r6, 0x2000
    mtctr   r6
lbcdelay:
    nop
    bdnz    lbcdelay
    nop
    isync
    sync

    mtspr   HID0, r0
    isync

    /* Zero-out registers: SPRGs */

    addis    r0,0,0
    isync                /* synchronize */

    mtspr       SPRG0, r0
    mtspr       SPRG1, r0
    mtspr       SPRG2, r0
    mtspr       SPRG3, r0
    isync                /* synchronize */

    /* zero-out the Segment registers */

    mtsr        0, r0
    isync
    mtsr        1, r0
    isync
    mtsr        2, r0
    isync
    mtsr        3, r0
    isync
    mtsr        4, r0
    isync
    mtsr        5, r0
    isync
    mtsr        6, r0
    isync
    mtsr        7, r0
    isync
    mtsr        8, r0
    isync
    mtsr        9, r0
    isync
    mtsr        10, r0
    isync
    mtsr        11, r0
    isync
    mtsr        12, r0
    isync
    mtsr        13, r0
    isync
    mtsr        14, r0
    isync
    mtsr        15, r0
    isync

    /* invalidate DBATs: clear VP and VS bits */

    mtspr       DBAT0U, r0
    isync
    mtspr       DBAT1U, r0
    isync
    mtspr       DBAT2U, r0
    isync
    mtspr       DBAT3U, r0
    isync

    mtspr       DBAT4U, r0
    isync
    mtspr       DBAT5U, r0
    isync
    mtspr       DBAT6U, r0
    isync
    mtspr       DBAT7U, r0
    isync

    /* invalidate IBATs: clear VP and VS bits */

    mtspr       IBAT0U, r0
    isync
    mtspr       IBAT1U, r0
    isync
    mtspr       IBAT2U, r0
    isync
    mtspr       IBAT3U, r0
    isync

    mtspr       IBAT4U, r0
    isync
    mtspr       IBAT5U, r0
    isync
    mtspr       IBAT6U, r0
    isync
    mtspr       IBAT7U, r0
    isync

    /* invalidate TLBs: loop on all TLB entries using r7 as an index */

    addi        r0, 0, 64
    mtspr       CTR, r0             /* Load CTR with TLB entry number */
    addi        r7, 0, 0            /* Use r7 as the tlb index */

tlb_write_loop:

    tlbie       r7                  /* invalidate the tlb entry */
    sync
    addi        r7, r7, 0x1000      /* increment the index */

    /* Decrement CTR, then branch if the decremented CTR is not equal to 0 */

    bdnz tlb_write_loop

    /* Turn off data and instruction cache control bits */

    mfspr       r7, HID0
    isync
    sync                            /* synchronize */
    andi.       r7, r7, 0x3FFF      /* Clear DCE and ICE bits */
    mtspr       HID0, r7
    isync
    sync                            /* synchronize */

#ifndef  BOOT_FROM_UBOOT

    /* Memory mapped region base address */

    WRITEADR(r6,r7,M83XX_DDRLAWBARn(0), LOCAL_MEM_LOCAL_ADRS)

    WRITEADR(r6,r7,M83XX_DDRLAWARn(0), (LAWAR_ENABLE | LAWAR_SIZE_128MB))

    WRITEADR(r6,r7,M83XX_DDRCDR, 0x7b880001)

    /* Initialize the DDR Memory controller */

    lis    r6, HI(DDR_REG_BA)
    ori    r6, r6, LO(DDR_REG_BA)

    WRITEOFFSET(r6,r7,(CS0_BNDS), 0x00000007)
    WRITEOFFSET(r6,r7,(CS0_CONFIG), 0x80010102)

    /* currently we use only one CS, so disable the other banks */

    WRITEOFFSET(r6,r7,(CS1_BNDS), 0x0)
    WRITEOFFSET(r6,r7,(CS1_CONFIG), 0x0)

    WRITEOFFSET(r6,r7,(DDR_SDRAM_CLK_CNTRL),  0x02000000)

    WRITEOFFSET(r6,r7,(TIMING_CFG_3),         0x00000000)
    WRITEOFFSET(r6,r7,(TIMING_CFG_0),         0x00220802)
    WRITEOFFSET(r6,r7,(TIMING_CFG_1),         0x27256222)
    WRITEOFFSET(r6,r7,(TIMING_CFG_2),         0x121048c5)

    WRITEOFFSET(r6,r7,(DDR_SDRAM_CFG),        0x43080000)
    WRITEOFFSET(r6,r7,(DDR_SDRAM_CFG_2),      0x00401000)
    WRITEOFFSET(r6,r7,(DDR_SDRAM_MODE_CFG),   0x04480232)
    WRITEOFFSET(r6,r7,(DDR_SDRAM_MODE_CFG_2), 0x00000000)
    WRITEOFFSET(r6,r7,(DDR_SDRAM_INTERVAL),   0x03600100)

    li      r5, 0x7000
    mtctr   r5
delayddr1:
    nop
    nop
    bdnz delayddr1

    isync

    /* enable DDR controller */

    WRITEOFFSET(r6,r7,(DDR_SDRAM_CFG), 0xC3080000)

#endif /* BOOT_FROM_UBOOT */

    /* Initialize the Local Bus Controller */

    lis    r6, HI(CCSBAR + LBC_REG_BA)
    ori    r6, r6, LO(CCSBAR + LBC_REG_BA)

    /* Local Bus setup lbcr and mrtpr */

    WRITEOFFSET(r6,r7,(0x0D0), 0x0004ff0f)
    WRITEOFFSET(r6,r7,(0x084), 0x20000000)

    WRITEADR(r6,r7,M83XX_OCCR,0xffffffff)
    isync

warm:

    /* turn the instruction cache ON for faster FLASH ROM boots */

    mfspr   r4, HID0
    ori     r4, r4, (_PPC_HID0_ICE | _PPC_HID0_ICFI)
    rlwinm  r5, r4, 0, (_PPC_HID0_BIT_ICFI + 1), (_PPC_HID0_BIT_ICFI - 1)
    isync

    /*
     * The setting of the instruction cache enable (ICE) bit must be
     * preceded by an isync instruction to prevent the cache from being
     * enabled or disabled while an instruction access is in progress.
     */

    mtspr    HID0, r4    /* Enable Instr Cache & Inval cache */
    sync

    /* using 2 consec instructions, E300 core recommendation */

    mtspr    HID0, r5
    isync

    /* initialize the stack pointer */

    lis     sp, HIADJ(STACK_ADRS)
    addi    sp, sp, LO(STACK_ADRS)

    /* go to C entry point */

    addi    sp, sp, -FRAMEBASESZ        /* get frame stack */

    /*
     * calculate C entry point: routine - entry point + ROM base
     * routine    = romStart
     * entry point    = romInit    = R7
     * ROM base    = ROM_TEXT_ADRS = R8
     * C entry point: romStart - R7 + R8
     */

    lis     r7, HIADJ(romInit)
    addi    r7, r7, LO(romInit)

    lis     r8, HIADJ(ROM_TEXT_ADRS)
    addi    r8, r8, LO(ROM_TEXT_ADRS)

    lis     r6, HIADJ(romStart)
    addi    r6, r6, LO(romStart) /* load R6 with C entry point */

    sub     r6, r6, r7          /* routine - entry point */
    add     r6, r6, r8          /* + ROM base */

    mtspr   LR, r6              /* save destination address into LR register */
    blr                         /* jump to the C entry point */

