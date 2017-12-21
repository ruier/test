/* romInit.s - ROM bootcode for Freescale ADS5121e BSP */

/*
 * Copyright (c) 2007-2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01e,23feb09,x_f  add Elpida DDR support for rev 4.1
01d,30dec08,x_f  add bootrom support for PCB rev4 hardware.
01c,25aug08,b_m  support rev 3.x hardware.
01b,16oct07,b_m  add memory controller init code.
01a,07sep07,b_m  created.
*/

#define _ASMLANGUAGE

#include <vxWorks.h>
#include <sysLib.h>
#include <asm.h>
#include <regs.h>
#include <arch/ppc/mmu603Lib.h>
#include "config.h"

    /* defines */

    FUNC_EXPORT(romInit)    /* start of system code */
    FUNC_EXPORT(_romInit)   /* start of system code */

    FUNC_EXPORT(romInitSdram)

    /* externals */

    FUNC_IMPORT(romStart)   /* system initialization routine */

#define WRITEADR(reg1, reg2, addr, val) \
    lis     reg1, HI(addr);             \
    ori     reg1, reg1, LO(addr);       \
    lis     reg2, HI(val);              \
    ori     reg2, reg2, LO(val);        \
    stw     reg2, 0(reg1)

    _WRS_TEXT_SEG_START

    .fill   0x100, 1, 0

/*******************************************************************************
*
* romInit - entry point for VxWorks in ROM
*
* SYNOPSIS
* \ss
* romInit
*     (
*     int startType     /@ only used by 2nd entry point @/
*     )
* \se
*/

FUNC_BEGIN(romInit)
FUNC_BEGIN(_romInit)

    bl      cold
    nop
    bl      warm

    /* copyright notice appears at beginning of ROM (in TEXT segment) */

    .ascii  "Copyright (c) 2007-2008 Wind River Systems, Inc."
    .align  2

cold:

    li      r2, BOOT_CLEAR

#ifndef BOOT_FROM_UBOOT

    /* Save MSR contents */

    mfmsr   r5

    /* Set IMMR area to our preferred location */

    lis     r4, HI(IMMRBAR_RESET_VALUE)
    ori     r4, r4, LO(IMMRBAR_RESET_VALUE)
    lis     r3, HI(IMMRBAR)
    ori     r3, r3, LO(IMMRBAR)
    stw     r3, 0(r4)
    mtspr   MBAR, r3

    /* Initialize machine status */

    li      r3, 0x3002          /* Set ME and RI flags */
    rlwimi  r3, r5, 0, 25, 25   /* preserve IP bit */
    mtmsr   r3
    sync
    mtspr   SRR1, r3            /* Mirror current MSR state in SRR1 */

    /* Initialize the Hardware Implementation-Dependent Registers */

    xor     r3, r3, r3
    sync
    mtspr   HID0, r3

    lis     r3, HI(HID0_CONFIG)
    ori     r3, r3, LO(HID0_CONFIG)
    sync
    mtspr   HID0, r3

    lis     r3, HI(HID2_CONFIG)
    ori     r3, r3, LO(HID2_CONFIG)
    sync
    mtspr   HID2, r3
    sync

    /* Set configuration of the Boot/CS0 */

    lis     r3, HI(XLBMEN_LPCSAW0)
    ori     r3, r3, LO(XLBMEN_LPCSAW0)
    lis     r4, HI(CS0_START_ADRS)
    ori     r4, r4, HI(CS0_STOP_ADRS)
    stw     r4, 0(r3)
    isync

    WRITEADR(r3, r4, LPC_CSCFGR0, CS0_CONFIG)
    isync

    /* Set configuration of CS2 */

    lis     r3, HI(XLBMEN_LPCSAW2)
    ori     r3, r3, LO(XLBMEN_LPCSAW2)
    lis     r4, HI(CS2_START_ADRS)
    ori     r4, r4, HI(CS2_STOP_ADRS)
    stw     r4, 0(r3)
    isync

    WRITEADR(r3, r4, LPC_CSCFGR2, CS2_CONFIG)
    isync

    /* Master enable all CS's */

    WRITEADR(r3, r4, LPC_CSCTLR, CS_CTRL_ME)

    WRITEADR(r3, r4, RESET_RSR, 0x331f)

    /* Set IPS-CSB divider: IPS = 1/2 CSB */

    lis     r3, HI(CLOCK_SCFR1)
    ori     r3, r3, LO(CLOCK_SCFR1)
    lwz     r0, 0(r3)
    rlwinm  r0, r0, 0, 9, 5
    oris    r0, r0, 384
    stw     r0, 0(r3)

    /* Enable Time Base Unit */

    lis     r3, HI(XLBMEN_SPCR)
    ori     r3, r3, LO(XLBMEN_SPCR)
    lwz     r0, 0(r3)
    oris    r0, r0, 64
    stw     r0, 0(r3)

    bl      romInitSdram
#endif /* !BOOT_FROM_UBOOT */

    bl      start

warm:
    mr      r2, r3

start:

    xor     r3, r3, r3
    sync

    mtmsr   r3
    isync

    /* clear SPRGs */

    mtspr   SPRG0, r3
    mtspr   SPRG1, r3
    mtspr   SPRG2, r3
    mtspr   SPRG3, r3
    isync

    /* clear segment registers */

    mtsr    0, r3
    isync
    mtsr    1, r3
    isync
    mtsr    2, r3
    isync
    mtsr    3, r3
    isync
    mtsr    4, r3
    isync
    mtsr    5, r3
    isync
    mtsr    6, r3
    isync
    mtsr    7, r3
    isync
    mtsr    8, r3
    isync
    mtsr    9, r3
    isync
    mtsr    10, r3
    isync
    mtsr    11, r3
    isync
    mtsr    12, r3
    isync
    mtsr    13, r3
    isync
    mtsr    14, r3
    isync
    mtsr    15, r3
    isync

    /* initialize BAT registers */

    mtspr   IBAT0U, r3
    isync
    mtspr   IBAT1U, r3
    isync
    mtspr   IBAT2U, r3
    isync
    mtspr   IBAT3U, r3
    isync

    mtspr   DBAT0U, r3
    isync
    mtspr   DBAT1U, r3
    isync
    mtspr   DBAT2U, r3
    isync
    mtspr   DBAT3U, r3
    isync

    mtspr   IBAT0L, r3
    isync
    mtspr   IBAT1L, r3
    isync
    mtspr   IBAT2L, r3
    isync
    mtspr   IBAT3L, r3
    isync

    mtspr   DBAT0L, r3
    isync
    mtspr   DBAT1L, r3
    isync
    mtspr   DBAT2L, r3
    isync
    mtspr   DBAT3L, r3
    isync

    /* invalidate entries within both TLBs */

    li      r3, 32
    mtctr   r3              /* CTR = 32 */
    xor     r3, r3, r3
    isync                   /* context sync req'd before tlbie */
tlbloop:
    tlbie   r3
    addi    r3, r3, 0x1000  /* increment bits 15-19 */
    sync                    /* sync instr req'd after tlbie      */
    bdnz    tlbloop         /* decrement CTR, branch if CTR != 0 */
    isync

    /* initialize the FPU's registers */

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

    mflr    r4      /* save link register so we can use it for */
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

    /* Restore MPU/MSR to a known state */

    mtmsr  r3   /* restore saved value */
    isync

    /* turn off data and instruction cache control bits */

    mfspr   r3, HID0            /* r3 = HID0 */
    andi.   r3, r3, 0xcfff      /* make sure lock bits are clear */
    ori     r4, r3, (_PPC_HID0_ICE | _PPC_HID0_DCE | _PPC_HID0_ICFI | _PPC_HID0_DCFI)
                                /* r4 has ICE,DCE,ICI,DCI bits set */
    andi.   r3, r3, 0x03ff      /* r3 has enable bits cleared */
    sync
    mtspr   HID0, r4            /* HID0 = r4 */
    isync
    sync
    mtspr   HID0, r3            /* HID0 = r3 */
    isync

#ifdef INCLUDE_CACHE_SUPPORT
#ifdef USER_I_CACHE_ENABLE

    mfspr   r3, HID0
    ori     r3, r3, _PPC_HID0_ICE

    /*
     * The setting of the instruction cache enable (ICE) bit must be
     * preceded by an isync instruction to prevent the cache from being
     * enabled or disabled while an instruction access is in progress.
     */

    isync
    mtspr   HID0, r3

#endif /* USER_I_CACHE_ENABLE */
#endif /* INCLUDE_CACHE_SUPPORT */

    /* go to C entry point */

    lis     sp, HI(STACK_ADRS)
    ori     sp, sp, LO(STACK_ADRS)
    addi    sp, sp, -FRAMEBASESZ
    lis     r6, HI(romStart)
    ori     r6, r6, LO(romStart)
    lis     r7, HI(romInit)
    ori     r7, r7, LO(romInit)
    lis     r8, HI(ROM_TEXT_ADRS)
    ori     r8, r8, LO(ROM_TEXT_ADRS)
    sub     r6, r6, r7
    add     r6, r6, r8
    mtlr    r6
    mr      r3, r2
    blr

FUNC_END(_romInit)
FUNC_END(romInit)

#ifndef BOOT_FROM_UBOOT
/*******************************************************************************
*
* romInitSdram - MPC5121e specific multi-port DRAM controller initialization
*
* Initialize MPC5121e specific multi-port DRAM controller.
*
* SYNOPSIS
* \ss
* void romInitSdram
*     (
*     void
*     )
* \se
*
* RETURNS: N/A
*/

FUNC_BEGIN(romInitSdram)

    /* io port control for memory */

    WRITEADR(r6, r7, IOCTL_MEM, 0x00000036)

    /* Initialize DDR Local Window */ 

    WRITEADR(r6, r7, XLBMEN_DDRLAWBAR, LOCAL_MEM_LOCAL_ADRS & 0xFFFFF000)
    WRITEADR(r6, r7, XLBMEN_DDRLAWAR, 0x1C)
    isync

    /* enable multi-port DRAM controller */

    WRITEADR(r6, r7, MDDRC_DDR_SYS_CFG, DDR_SYS_CFG_EN)

    /* initialize priority manager */

    WRITEADR(r6, r7, MDDRC_PRIOMAN_CFG1, PRIOMAN_CFG1_VALUE)
    WRITEADR(r6, r7, MDDRC_PRIOMAN_CFG2, PRIOMAN_CFG2_VALUE)
    WRITEADR(r6, r7, MDDRC_HIPRIO_CFG, HIPRIO_CFG_VALUE)

    WRITEADR(r6, r7, MDDRC_LUT0_MAIN_UPPER, LUT0_MAIN_UPPER_VALUE)
    WRITEADR(r6, r7, MDDRC_LUT1_MAIN_UPPER, LUT1_MAIN_UPPER_VALUE)
    WRITEADR(r6, r7, MDDRC_LUT2_MAIN_UPPER, LUT2_MAIN_UPPER_VALUE)
    WRITEADR(r6, r7, MDDRC_LUT3_MAIN_UPPER, LUT3_MAIN_UPPER_VALUE)
    WRITEADR(r6, r7, MDDRC_LUT4_MAIN_UPPER, LUT4_MAIN_UPPER_VALUE)

    WRITEADR(r6, r7, MDDRC_LUT0_MAIN_LOWER, LUT0_MAIN_LOWER_VALUE)
    WRITEADR(r6, r7, MDDRC_LUT1_MAIN_LOWER, LUT1_MAIN_LOWER_VALUE)
    WRITEADR(r6, r7, MDDRC_LUT2_MAIN_LOWER, LUT2_MAIN_LOWER_VALUE)
    WRITEADR(r6, r7, MDDRC_LUT3_MAIN_LOWER, LUT3_MAIN_LOWER_VALUE)
    WRITEADR(r6, r7, MDDRC_LUT4_MAIN_LOWER, LUT4_MAIN_LOWER_VALUE)

    WRITEADR(r6, r7, MDDRC_LUT0_ALT_UPPER, LUT0_ALT_UPPER_VALUE)
    WRITEADR(r6, r7, MDDRC_LUT1_ALT_UPPER, LUT1_ALT_UPPER_VALUE)
    WRITEADR(r6, r7, MDDRC_LUT2_ALT_UPPER, LUT2_ALT_UPPER_VALUE)
    WRITEADR(r6, r7, MDDRC_LUT3_ALT_UPPER, LUT3_ALT_UPPER_VALUE)
    WRITEADR(r6, r7, MDDRC_LUT4_ALT_UPPER, LUT4_ALT_UPPER_VALUE)

    WRITEADR(r6, r7, MDDRC_LUT0_ALT_LOWER, LUT0_ALT_LOWER_VALUE)
    WRITEADR(r6, r7, MDDRC_LUT1_ALT_LOWER, LUT1_ALT_LOWER_VALUE)
    WRITEADR(r6, r7, MDDRC_LUT2_ALT_LOWER, LUT2_ALT_LOWER_VALUE)
    WRITEADR(r6, r7, MDDRC_LUT3_ALT_LOWER, LUT3_ALT_LOWER_VALUE)
    WRITEADR(r6, r7, MDDRC_LUT4_ALT_LOWER, LUT4_ALT_LOWER_VALUE)

    /* initialize multi-port DRAM controller */

    WRITEADR(r6, r7, MDDRC_DDR_SYS_CFG,   DDR_SYS_CFG_VALUE)
    WRITEADR(r6, r7, MDDRC_DDR_TIME_CFG0, DDR_TIME_CFG0_VALUE)
    WRITEADR(r6, r7, MDDRC_DDR_TIME_CFG1, DDR_TIME_CFG1_VALUE)
    WRITEADR(r6, r7, MDDRC_DDR_TIME_CFG2, DDR_TIME_CFG2_VALUE)

    /* issue 10 NOP commands */

    li      r3, 10
    mtctr   r3

ddr_init:
    WRITEADR(r6, r7, MDDRC_DDR_CMD, DDR_CMD_NOP)
    bdnz    ddr_init

    WRITEADR(r6, r7, MDDRC_DDR_CMD, DDR_CMD_PRECHG_ALL)
    WRITEADR(r6, r7, MDDRC_DDR_CMD, DDR_CMD_NOP)
    WRITEADR(r6, r7, MDDRC_DDR_CMD, DDR_CMD_REFRESH)
    WRITEADR(r6, r7, MDDRC_DDR_CMD, DDR_CMD_NOP)
    WRITEADR(r6, r7, MDDRC_DDR_CMD, DDR_CMD_REFRESH)
    WRITEADR(r6, r7, MDDRC_DDR_CMD, DDR_CMD_NOP)

#ifdef INCLUDE_DDR_MICRON
    WRITEADR(r6, r7, MDDRC_DDR_CMD, DDR_CMD_INIT_DEV_OP)
    WRITEADR(r6, r7, MDDRC_DDR_CMD, DDR_CMD_NOP)
    WRITEADR(r6, r7, MDDRC_DDR_CMD, DDR_CMD_EXMODE2)
    WRITEADR(r6, r7, MDDRC_DDR_CMD, DDR_CMD_NOP)
    WRITEADR(r6, r7, MDDRC_DDR_CMD, DDR_CMD_PRECHG_ALL)
    WRITEADR(r6, r7, MDDRC_DDR_CMD, DDR_CMD_EXMODE2)
    WRITEADR(r6, r7, MDDRC_DDR_CMD, DDR_CMD_EXMODE3)
    WRITEADR(r6, r7, MDDRC_DDR_CMD, DDR_CMD_EN_DLL)
    WRITEADR(r6, r7, MDDRC_DDR_CMD, DDR_CMD_INIT_DEV_OP)
    WRITEADR(r6, r7, MDDRC_DDR_CMD, DDR_CMD_PRECHG_ALL)
    WRITEADR(r6, r7, MDDRC_DDR_CMD, DDR_CMD_REFRESH)
    WRITEADR(r6, r7, MDDRC_DDR_CMD, DDR_CMD_INIT_DEV_OP)
    WRITEADR(r6, r7, MDDRC_DDR_CMD, DDR_CMD_OCD_DEFAULT)
    WRITEADR(r6, r7, MDDRC_DDR_CMD, DDR_CMD_PRECHG_ALL)
    WRITEADR(r6, r7, MDDRC_DDR_CMD, DDR_CMD_NOP)
#else  /* INCLUDE_DDR_MICRON */
    WRITEADR(r6, r7, MDDRC_DDR_CMD, DDR_CMD_EXMODE2)
    WRITEADR(r6, r7, MDDRC_DDR_CMD, DDR_CMD_EXMODE3)
    WRITEADR(r6, r7, MDDRC_DDR_CMD, DDR_CMD_EN_DLL)
    WRITEADR(r6, r7, MDDRC_DDR_CMD, DDR_CMD_RES_DLL)
    WRITEADR(r6, r7, MDDRC_DDR_CMD, DDR_CMD_PRECHG_ALL)
    WRITEADR(r6, r7, MDDRC_DDR_CMD, DDR_CMD_REFRESH)
    WRITEADR(r6, r7, MDDRC_DDR_CMD, DDR_CMD_REFRESH)
    WRITEADR(r6, r7, MDDRC_DDR_CMD, DDR_CMD_REFRESH)
    WRITEADR(r6, r7, MDDRC_DDR_CMD, DDR_CMD_INIT_DEV_OP)
#endif /* INCLUDE_DDR_MICRON */

    WRITEADR(r6, r7, MDDRC_DDR_CMD, DDR_CMD_OCD_DEFAULT)
    WRITEADR(r6, r7, MDDRC_DDR_CMD, DDR_CMD_OCD_EXIT)
    WRITEADR(r6, r7, MDDRC_DDR_CMD, DDR_CMD_NOP)

    /* issue 10 NOP commands */

    li      r3, 10
    mtctr   r3
loop:
    WRITEADR(r6, r7, MDDRC_DDR_CMD, DDR_CMD_NOP)
    bdnz    loop

    /* enable multi-port DRAM controller */

    WRITEADR(r6, r7, MDDRC_DDR_TIME_CFG0, DDR_TIME_CFG0_RUN)
    WRITEADR(r6, r7, MDDRC_DDR_SYS_CFG, DDR_SYS_CFG_RUN)

    blr

FUNC_END(romInitSdram)

#endif /* !BOOT_FROM_UBOOT */
