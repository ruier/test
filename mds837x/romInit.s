/* romInit.s - Freescale mds837x ROM initialization module */

/*
 * Copyright (c) 2005, 2007, 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01c,15dec09,x_z  add support for booting from vxWorks bootrom and code clean.
01b,20oct07,x_s  modified for mds837x.
01a,19jan05,dtr  written from ads834x/romInit.s/01f
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

    /*defines */

    /* CS0 -FLASH */

    /* Port Size=2(16bit), DECC=0, WP=0, MSEL=0(GPCM), ATOM=0, V=1 */

#   define QUICC_BR0_VAL            \
        ((FLASH_BASE_ADRS & M83XX_BR_BA_MASK) | M83XX_BR_PS_16 | M83XX_BR_V)

    /*
     * AM=0xfe000000(32MB), BCTLD=0, CSNT=0, ACS=0, XACS=1, SCY=9, SETA=0,
     * TRLX=0, EHTR=1, EAD=1
     */

#   define QUICC_OR0_VAL            \
        (M83XX_OR_AM_32MB | M83XX_OR_GPCM_XACS | \
         ((9 << M83XX_OR_GPCM_SCY_SHIFT) & M83XX_OR_GPCM_SCY) | \
         M83XX_OR_GPCM_EHTR | M83XX_OR_GPCM_EAD)

    /* CS1 - BCSR */

    /* Port Size=1(8bit), DECC=0, WP=0, MSEL=0(GPCM), ATOM=0, V=1  */

#   define QUICC_BR1_VAL            \
        ((BCSR_BASE_ADRS & M83XX_BR_BA_MASK) | M83XX_BR_PS_8 | M83XX_BR_V)

    /*
     * AM=0xffff8000(32KB), BCTLD=0, CSNT=1, ACS=0, XACS=1, SCY=15, SETA=0,
     * TRLX=1,EHTR=1,EAD=0
     */

#   define QUICC_OR1_VAL            \
        (M83XX_OR_AM_32KB | M83XX_OR_GPCM_CSNT | M83XX_OR_GPCM_XACS | \
         ((15 << M83XX_OR_GPCM_SCY_SHIFT) & M83XX_OR_GPCM_SCY) | \
         M83XX_OR_GPCM_TRLX | M83XX_OR_GPCM_EHTR | M83XX_OR_GPCM_EAD)

    /* memory configguration */

#   define CS0_BNDS_VAL             \
        (((LOCAL_MEM_LOCAL_ADRS >> CS_BNDS_SHIFT) & CS_BNDS_MASK) | \
          (((LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE) >> CS_BNDS_SHIFT) & \
            CS_BNDS_MASK))

#   define CS0_CONFIG_VAL           \
        (CS_CONFIG_EN | CS_CONFIG_ODT_WR_CFG_1 | CS_CONFIG_ROW_BIT_14 | \
         CS_CONFIG_COL_BIT_10)

#   define DDR_SDRAM_CLK_CNTRL_VAL \
        ((2 << DDR_SDRAM_CLK_CNTL_CLK_ADJUST_SHIFT) & \
         DDR_SDRAM_CLK_CNTL_CLK_ADJUST)

#   define TIMING_CFG_3_VAL         0

#   define TIMING_CFG_0_VAL         \
        (((0 << TIMING_CFG_0_RWT_SHIFT) & TIMING_CFG_0_RWT) | \
         ((0  << TIMING_CFG_0_WRT_SHIFT) & TIMING_CFG_0_WRT) | \
         ((0  << TIMING_CFG_0_RRT_SHIFT) & TIMING_CFG_0_RRT) | \
         ((0 << TIMING_CFG_0_WWT_SHIFT) & TIMING_CFG_0_WWT) | \
         ((6 << TIMING_CFG_0_ACT_PD_EXIT_SHIFT) & TIMING_CFG_0_ACT_PD_EXIT) | \
         ((2 << TIMING_CFG_0_PRE_PD_EXIT_SHIFT) & TIMING_CFG_0_PRE_PD_EXIT) | \
         ((8 << TIMING_CFG_0_ODT_PD_EXIT_SHIFT) & TIMING_CFG_0_ODT_PD_EXIT) | \
         (2 & TIMING_CFG_0_MRS_CYC))

#   define TIMING_CFG_1_VAL         \
        (((3 << TIMING_CFG_1_PRETOACT_SHIFT) & TIMING_CFG_1_PRETOACT) | \
         ((9 << TIMING_CFG_1_ACTTOPRE_SHIFT) & TIMING_CFG_1_ACTTOPRE_SHIFT) | \
         ((3 << TIMING_CFG_1_ACTTORW_SHIFT) & TIMING_CFG_1_ACTTORW) | \
         ((5 << TIMING_CFG_1_CASLAT_SHIFT) & TIMING_CFG_1_CASLAT) | \
         ((13 << TIMING_CFG_1_REFREC_SHIFT) & TIMING_CFG_1_REFREC) | \
         ((3 << TIMING_CFG_1_WRREC_SHIFT) & TIMING_CFG_1_WRREC) | \
         ((2<< TIMING_CFG_1_ACTTOACT_SHIFT) & TIMING_CFG_1_ACTTOACT) | \
         (2 & TIMING_CFG_1_WRTORD))

#   define TIMING_CFG_2_VAL         \
        (((1 << TIMING_CFG_2_ADD_LAT_SHIFT) & TIMING_CFG_2_ADD_LAT) | \
         ((6 << TIMING_CFG_2_CPO_SHIFT) & TIMING_CFG_2_CPO) | \
         ((2 << TIMING_CFG_2_WR_LAT_DELAY_SHIFT) & TIMING_CFG_2_WR_LAT_DELAY)|\
         ((4 << TIMING_CFG_2_RD_TO_PRE_SHIFT) & TIMING_CFG_2_RD_TO_PRE) | \
         ((2 << TIMING_CFG_2_WR_DATA_DELAY_SHIFT)& TIMING_CFG_2_WR_DATA_DELAY)|\
         ((3 << TIMING_CFG_2_CKE_PLS_SHIFT) & TIMING_CFG_2_CKE_PLS) | \
         (8 & TIMING_CFG_2_FOUR_ACT))

#   define DDR_SDRAM_CFG_VAL        \
        (SDRAM_CFG_SREN | SDRAM_CFG_SDRAM_TYPE_DDR2)

#   define DDR_SDRAM_CFG_2_VAL      \
        ((1 << SDRAM_CFG_2_NUM_PR_SHIFT) & SDRAM_CFG_2_NUM_PR)

#   define DDR_SDRAM_MODE_CFG_VAL   \
        ((0x0448 << DDR_SDRAM_MODE_ESD_SHIFT) & DDR_SDRAM_MODE_ESD) | \
         (0x1432 & DDR_SDRAM_MODE_SD)

#   define DDR_SDRAM_MODE_CFG_2_VAL   \
        ((0 << DDR_SDRAM_MODE_ESD2_SHIFT) & DDR_SDRAM_MODE_ESD2) | \
         (0 & DDR_SDRAM_MODE_ESD3)

#   define DDR_SDRAM_INTERVAL_VAL   \
        (((0x03E0 << SDRAM_INTERVAL_REFINT_SHIFT) & SDRAM_INTERVAL_REFINT) | \
         (0x0100 & SDRAM_INTERVAL_BSTOPRE))

    /* register operations */

#   define WRITEADR(reg1,reg2,addr32,val)       \
        lis reg1, HI(addr32);                   \
        ori reg1, reg1, LO(addr32);             \
        lis reg2, HI(val);                      \
        ori reg2, reg2, LO(val);                \
        stw reg2, 0(reg1)

#   define WRITEOFFSET(regbase,reg2,offset,val) \
        lis reg2, HI(val);                      \
        ori reg2, reg2, LO(val);                \
        stw reg2, offset(regbase)

    /* internals */

    FUNC_EXPORT(_romInit)       /* start of system code */
    FUNC_EXPORT(romInit)        /* start of system code */

    /* externals */

    .extern romStart            /* system initialization routine */

    .text

    /*
     * Hard Reset Configuration Word (HRCW) See config.h/mds837x.h/target.ref
     * for more info.
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
* Entry point for VxWorks in ROM
*
* RETURNS: N/A
*/

_romInit:
romInit:

    bl  cold        /* jump to the cold boot initialization */
    nop
    bl  warm        /* jump to the warm boot initialization */

    /* copyright notice appears at beginning of ROM (in TEXT segment) */

    .ascii   "Copyright 1984-2007 Wind River Systems, Inc."
    .align 2

cold:
    li      r11, BOOT_COLD   /* set cold boot as start type */

    /*
     * initialize the IMMR register before any non-core registers
     * modification. The default IMMR base address was 0xFF400000,
     * as originally programmed in the Hard Reset Configuration Word.
     */

#ifndef BOOT_FROM_UBOOT
    lis     r4, HI (CCSBAR)
    ori     r4, r4, LO (CCSBAR)
    lis     r8, HIADJ(CCSBAR_INIT)  /* IMMR was at 0xff400000 */
    ori     r8, r8, LO(CCSBAR_INIT) /* IMMR now at CCSBAR     */
    stw     r4, 0(r8)
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

    WRITEADR(r6, r7, M83XX_LBLAWBARn (1), FLASH_BASE_ADRS)

    WRITEADR(r6, r7, M83XX_LBLAWARn (1), (LAWAR_ENABLE |  LAWAR_SIZE_32MB ))

    lis     r6, HI(M83XX_LBLAWARn (1))
    ori     r6, r6, LO(M83XX_LBLAWARn (1))
    lwz     r7, 0(r6)

    isync
#endif /* BOOT_FROM_UBOOT */

    /*
     * When the PowerPC 83xx is powered on, the processor fetches the
     * instructions located at the address 0x100. We need to jump
     * from the address 0x100 to the Flash space.
     */

    lis     r4, HIADJ(start)                /* load r4 with the address */
    addi    r4, r4, LO(start)               /* of start */

    lis     r5, HIADJ(romInit)              /* load r5 with the address */
    addi    r5, r5, LO(romInit)             /* of romInit() */

    lis     r6, HIADJ(ROM_TEXT_ADRS)        /* load r6 with the address */
    addi    r6, r6, LO(ROM_TEXT_ADRS)       /* of ROM_TEXT_ADRS */

    sub     r4, r4, r5
    add     r4, r4, r6

    mtspr   LR, r4                          /* save destination address */
                                            /* into LR register */
    blr                                     /* jump to flash mem address */

start:

    /* set the MSR register to a known state */

    xor r0, r0, r0      /* clear register R0 */
    isync               /* synchronize */
    mtmsr   r0          /* clear the MSR register */
    sync
    isync               /* synchronize */

    /* CS0 -FLASH */

    WRITEADR (r6, r7, QUICC_BR0 (CCSBAR), QUICC_BR0_VAL)
    WRITEADR (r6, r7, QUICC_OR0 (CCSBAR), QUICC_OR0_VAL)

    /* LAW BAR 0 */

    WRITEADR (r6, r7, M83XX_LBLAWBARn (0), FLASH_BASE_ADRS)
    WRITEADR (r6, r7, M83XX_LBLAWARn (0), (LAWAR_ENABLE |  LAWAR_SIZE_32MB))

    lis     r6, HI(M83XX_LBLAWARn (0))
    ori     r6, r6, LO(M83XX_LBLAWARn (0))
    lwz     r7, 0(r6)

    /* CS1 - BCSR */

    /* Port Size=1(8bit), DECC=0, WP=0, MSEL=0(GPCM), ATOM=0, V=1  */

    WRITEADR(r6,r7,QUICC_BR1 (CCSBAR), QUICC_BR1_VAL)

    /*
     * AM=0xffff8000(32KB), BCTLD=0, CSNT=1, ACS=0, XACS=1, SCY=15, SETA=1,
     * TRLX=1,EHTR=1,EAD=0
     */

    WRITEADR(r6, r7, QUICC_OR1 (CCSBAR), QUICC_OR1_VAL)

    /* LAW BAR 1 */

    WRITEADR(r6, r7, M83XX_LBLAWBARn (1), BCSR_BASE_ADRS)

    WRITEADR(r6, r7, M83XX_LBLAWARn (1), (LAWAR_ENABLE | LAWAR_SIZE_32KB ))

    lis     r6, HI(M83XX_LBLAWARn (1))
    ori     r6, r6, LO(M83XX_LBLAWARn (1))
    lwz     r7, 0(r6)

    isync

    /* disable clocks, peripheral driver or sysHwInit() will start  */

    WRITEADR(r6, r7, QUICC_LBCR (CCSBAR), 0)
    isync

    /* Initialise the Local Bus Controller */

    WRITEADR(r6, r7, QUICC_LCRR (CCSBAR), M83XX_LCRR_PBYP | M83XX_LCRR_CLKDIV_4)

    li      r6, 0x0020
    mtctr   r6
lbcdelay:
    nop
    bdnz lbcdelay
    nop
    isync
    sync

    mtspr   HID0, r0        /* HID0 */
    isync

    /* Zero-out registers: SPRGs */

    addis   r0, 0, 0
    isync                   /* synchronize */

    mtspr   SPRG0, r0
    mtspr   SPRG1, r0
    mtspr   SPRG2, r0
    mtspr   SPRG3, r0
    isync                   /* synchronize */

    /* zero-out the Segment registers */

    mtsr    0, r0
    isync
    mtsr    1, r0
    isync
    mtsr    2, r0
    isync
    mtsr    3, r0
    isync
    mtsr    4, r0
    isync
    mtsr    5, r0
    isync
    mtsr    6, r0
    isync
    mtsr    7, r0
    isync
    mtsr    8, r0
    isync
    mtsr    9, r0
    isync
    mtsr    10, r0
    isync
    mtsr    11, r0
    isync
    mtsr    12, r0
    isync
    mtsr    13, r0
    isync
    mtsr    14, r0
    isync
    mtsr    15, r0
    isync

    /* invalidate DBATs: clear VP and VS bits */

    mtspr   DBAT0U, r0
    isync
    mtspr   DBAT1U, r0
    isync
    mtspr   DBAT2U, r0
    isync
    mtspr   DBAT3U, r0
    isync

    mtspr   DBAT4U, r0
    isync
    mtspr   DBAT5U, r0
    isync
    mtspr   DBAT6U, r0
    isync
    mtspr   DBAT7U, r0
    isync

    /* invalidate IBATs: clear VP and VS bits */

    mtspr   IBAT0U, r0
    isync
    mtspr   IBAT1U, r0
    isync
    mtspr   IBAT2U, r0
    isync
    mtspr   IBAT3U, r0
    isync

    mtspr   IBAT4U, r0
    isync
    mtspr   IBAT5U, r0
    isync
    mtspr   IBAT6U, r0
    isync
    mtspr   IBAT7U, r0
    isync

    /* invalidate TLBs: loop on all TLB entries using r7 as an index */

    addi    r0, 0, 64
    mtspr   CTR, r0             /* Load CTR with TLB entry number */
    addi    r7, 0, 0            /* Use r7 as the tlb index */

tlb_write_loop:

    tlbie   r7                  /* invalidate the tlb entry */
    sync
    addi    r7, r7, 0x1000      /* increment the index */

    /* Decrement CTR, then branch if the decremented CTR is not equal to 0 */

    bdnz tlb_write_loop

    /* Turn off data and instruction cache control bits */

    mfspr   r7, HID0
    isync
    sync                        /* synchronize */
    andi.   r7, r7, 0x3FFF      /* Clear DCE and ICE bits */
    mtspr   HID0, r7
    isync
    sync                            /* synchronize */

#ifndef BOOT_FROM_UBOOT

    /* Memory mapped region base address */

    WRITEADR(r6, r7, M83XX_DDRLAWBARn (0), LOCAL_MEM_LOCAL_ADRS)
    WRITEADR(r6, r7, M83XX_DDRLAWARn (0), LAWAR_ENABLE | LAWAR_SIZE_512MB)

    /* Initialize the DDR Memory controller */

    /* ODT 150ohm on SoC */

    WRITEADR(r6, r7, (M83XX_DDRCDR), 0x80080001)

    lis     r6, HI(DDR_REG_BA)
    ori     r6, r6, LO(DDR_REG_BA)

    WRITEOFFSET(r6, r7, CS0_BNDS, CS0_BNDS_VAL)
    WRITEOFFSET(r6, r7, (CS0_CONFIG), CS0_CONFIG_VAL)

    /* currently we use only one CS, so disable the other banks */

    WRITEOFFSET(r6, r7, (CS1_BNDS), 0)
    WRITEOFFSET(r6, r7, (CS1_CONFIG), 0)
    WRITEOFFSET(r6, r7, (CS2_BNDS), 0)
    WRITEOFFSET(r6, r7, (CS2_CONFIG), 0)
    WRITEOFFSET(r6, r7, (CS3_BNDS), 0)
    WRITEOFFSET(r6, r7, (CS3_CONFIG), 0)

    WRITEOFFSET(r6, r7, (DDR_SDRAM_CLK_CNTRL), DDR_SDRAM_CLK_CNTRL_VAL)

    WRITEOFFSET(r6, r7, (TIMING_CFG_3), TIMING_CFG_3_VAL)
    WRITEOFFSET(r6, r7, (TIMING_CFG_0), TIMING_CFG_0_VAL)
    WRITEOFFSET(r6, r7, (TIMING_CFG_1), TIMING_CFG_1_VAL)
    WRITEOFFSET(r6, r7, (TIMING_CFG_2), TIMING_CFG_2_VAL)

    WRITEOFFSET(r6, r7, (DDR_SDRAM_CFG), DDR_SDRAM_CFG_VAL)
    WRITEOFFSET(r6, r7, (DDR_SDRAM_CFG_2), DDR_SDRAM_CFG_2_VAL)
    WRITEOFFSET(r6, r7, (DDR_SDRAM_MODE_CFG), DDR_SDRAM_MODE_CFG_VAL)
    WRITEOFFSET(r6, r7, (DDR_SDRAM_MODE_CFG_2), DDR_SDRAM_MODE_CFG_2_VAL)
    WRITEOFFSET(r6, r7, (DDR_SDRAM_INTERVAL), DDR_SDRAM_INTERVAL_VAL)

    li      r5, 0x7000
    mtctr   r5
delayddr1:
    nop
    nop
    bdnz delayddr1

    isync

    /* enable DDR controller */

    WRITEOFFSET(r6,r7,(DDR_SDRAM_CFG), (DDR_SDRAM_CFG_VAL | SDRAM_CFG_MEM_EN))

#endif /* BOOT_FROM_UBOOT */

    isync

    /* save current value of MSR */

    mfmsr   r3

    /* Turn on FP */

    li      r4, _PPC_MSR_FP     /* turn on FP so we can init it */
    sync
    mtmsr   r4
    isync

    /* Init the floating point control/status register */

    mtfsfi  7, 0
    mtfsfi  6, 0
    mtfsfi  5, 0
    mtfsfi  4, 0
    mtfsfi  3, 0
    mtfsfi  2, 0
    mtfsfi  1, 0
    mtfsfi  0, 0
    isync

    /*
     * Initialize the floating point data registers to a known state.
     * First save the link register.  By doing a bl to ifpdrValue,
     * the link register has the address of the IEEE value for 1.0
     * The lr can now be moved to a register for use as a pointer for
     * the IEEE value of 1.0
     */

    mflr    r4          /* save link register so we can use it for */
                        /* initializing the fp registers with a good number */

    bl      ifpdrValue  /* link register now has pointer to IEEE value */
    .long   0x3f800000  /* IEEE single-precision value of 1.0 */

ifpdrValue:
    mflr    r5          /* move current lr to r3 for use as pointer */
    mtlr    r4          /* restore lr to previous value */
    lfs     f0, 0(r5)   /* load all 32 FP regs with 1.0 */
    lfs     f1, 0(r5)
    lfs     f2, 0(r5)
    lfs     f3, 0(r5)
    lfs     f4, 0(r5)
    lfs     f5, 0(r5)
    lfs     f6, 0(r5)
    lfs     f7, 0(r5)
    lfs     f8, 0(r5)
    lfs     f9, 0(r5)
    lfs     f10, 0(r5)
    lfs     f11, 0(r5)
    lfs     f12, 0(r5)
    lfs     f13, 0(r5)
    lfs     f14, 0(r5)
    lfs     f15, 0(r5)
    lfs     f16, 0(r5)
    lfs     f17, 0(r5)
    lfs     f18, 0(r5)
    lfs     f19, 0(r5)
    lfs     f20, 0(r5)
    lfs     f21, 0(r5)
    lfs     f22, 0(r5)
    lfs     f23, 0(r5)
    lfs     f24, 0(r5)
    lfs     f25, 0(r5)
    lfs     f26, 0(r5)
    lfs     f27, 0(r5)
    lfs     f28, 0(r5)
    lfs     f29, 0(r5)
    lfs     f30, 0(r5)
    lfs     f31, 0(r5)
    sync

    /*  Restore MPU/MSR to a known state  */

    mtmsr   r3  /* restore saved value */
    isync

warm:
    /* turn the instruction cache ON for faster FLASH ROM boots */

    mfspr   r4, HID0

    /* set ICE & ICFI */

    ori     r4, r4, _PPC_HID0_ICE | _PPC_HID0_ICFI

    /* clear the ICFI bit */

    rlwinm  r5, r4, 0, _PPC_HID0_BIT_ICFI + 1, _PPC_HID0_BIT_ICFI - 1

    isync

    /*
     * The setting of the instruction cache enable (ICE) bit must be
     * preceded by an isync instruction to prevent the cache from being
     * enabled or disabled while an instruction access is in progress.
     */

    mtspr   HID0, r4    /* Enable Instr Cache & Inval cache */
    sync
    mtspr   HID0, r5    /* using 2 consec instructions      */
                        /* E300 core recommendation         */
    isync

    /* initialize the stack pointer */

    lis     sp, HIADJ(STACK_ADRS)
    addi    sp, sp, LO(STACK_ADRS)

    /* go to C entry point */

    addi    sp, sp, -FRAMEBASESZ        /* get frame stack */

    /*
     * calculate C entry point: routine - entry point + ROM base
     * routine  = romStart
     * entry point  = romInit   = R7
     * ROM base = ROM_TEXT_ADRS = R8
     * C entry point: romStart - R7 + R8
     */

    lis     r7, HIADJ(romInit)
    addi    r7, r7, LO(romInit)

    lis     r8, HIADJ(ROM_TEXT_ADRS)
    addi    r8, r8, LO(ROM_TEXT_ADRS)

    lis     r6, HIADJ(romStart)
    addi    r6, r6, LO(romStart)    /* load R6 with C entry point */

    sub     r6, r6, r7              /* routine - entry point */
    add     r6, r6, r8              /* + ROM base */
    mr      r3, r11
    mtspr   LR, r6                  /* save destination address*/
                                    /* into LR register */
    blr                             /* jump to the C entry point */
