/* romInit.s - Wind River Lite5200B ROM initialization module */

/* 
 * Copyright (c) 2003-2004, 2006-2007 Wind River Systems, Inc. 
 * 
 * The right to copy, distribute, modify, or otherwise make use 
 * of this software may be licensed only pursuant to the terms 
 * of an applicable Wind River license agreement. 
 */

/*
modification history
--------------------
01l,07aug07,agf  remove vxDecGet, it is part of arch lib
01k,10jan07,jw   modified from Lite5200 to work on Lite5200B, add SDELAY
01j,25may06,pkr	 support boot timing
01i,03feb06,pkr  adapt to different frequency jumpers
01h,03feb06,pkr  add U_BOOT support
01g,24jan06,pkr  Remove CP2, merge two RAM blocks into one
01f,30dec04,k_p  Copyright info update in the routine romInit()
01e,08mar04,bjn  Remove BAT register init
01d,04feb04,bjn  support IS_ICECUBE_OLD
01c,13nov03,e_c  Enable read/write access to the Flash in DBAT to use
                 nvram driver
01c,24oct03,pdr  fixed BATs settings (bad behavior during PCI
                 autoconfiguration)
01b,17sep03,pkr  adapted for Scorpion CP2 board.
            pdr  moved MBAR to 0x01000000.
01a,17jul03,pkr  written
*/

/*
DESCRIPTION
This module contains the entry code for the VxWorks bootrom.
The entry point romInit, is the first code executed on power-up.
The startType parameter is set (cold boot) or passed (warm boot)
to the generic romStart() routine.

The routine sysToMonitor() jumps to the location 4 bytes
past the beginning of romInit, to perform a "warm boot".

Hardware that requires special register setting or memory
mapping to be done immediately, may do so here.
*/

#define _ASMLANGUAGE
#include "vxWorks.h"
#include "sysLib.h"
#include "asm.h"
#include "config.h"
#include "regs.h"
#include "arch/ppc/mmu603Lib.h"

        /* defines */
#if (SYS_PLL_FREQ == SYS_FREQ*16 ||SYS_PLL_FREQ == SYS_FREQ*32)
#define CDM_DIV_VALUE CDM_DIV_VALUE_16
#else
#define CDM_DIV_VALUE CDM_DIV_VALUE_12
#endif

    /* Exported internal functions */

    FUNC_EXPORT(_romInit)   /* start of system code */
    FUNC_EXPORT(romInit)    /* start of system code */

    /* externals */

    FUNC_IMPORT(romStart)   /* system initialization routine */

    _WRS_TEXT_SEG_START

/***************************************************************************
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

FUNC_BEGIN(_romInit)
FUNC_BEGIN(romInit)

    bl  cold
        nop                     /* pad to 8 byte boundary */
    bl  warm

    /* copyright notice appears at beginning of ROM (in TEXT segment) */

    .ascii   "Copyright 1984-2007 Wind River Systems, Inc."
    .align 2

    /* useful when used with an emulator to detect interrupts (when vector is low) */
    /* .space    0x3000 */

cold:
#ifndef BOOT_FROM_UBOOT
    /* Setup MBAR mirror register. MBAR is at 0x80000000 after reset. */
    lis r3,HI(MBAR_RESET_VALUE)
    ori r3,r3,LO(MBAR_RESET_VALUE)
    mtspr   MBAR,r3

    /* store new MBAR value to MBAR */
    lis     r4,LO(MBAR_VALUE)     /* this is to load the high address part into the LSB */
    ori     r4,r4,HI(MBAR_VALUE)
    stw     r4,MBAR_IPBI(r3)
    eieio

    /* setup MBAR */
    lis r3,HI(MBAR_VALUE)
    ori r3,r3,LO(MBAR_VALUE)
    mtspr   MBAR,r3

#endif /* BOOT_FROM_UBOOT */

    /* Setup GPS to something meaningful */

    lis r4,HI(GPS_INIT_VALUE)
    ori r4,r4,LO(GPS_INIT_VALUE)
    stw r4,MBAR_GPIO_STD+GPIO_STD_PORT_CONFIG(r3)
    eieio

    /* Setup CDM */

    lis r4,HI(CDM_CFG_VALUE)
    ori r4,r4,LO(CDM_CFG_VALUE)
    stw r4,MBAR_CDM+CDM_CFG_OFF(r3)
    eieio

    lis r4,HI(CDM_DIV_VALUE)
    ori r4,r4,LO(CDM_DIV_VALUE)
    stw r4,MBAR_CDM+CDM_DIVIDER_OFF(r3)
    eieio

    lis r4,HI(CDM_ENA_VALUE)
    ori r4,r4,LO(CDM_ENA_VALUE)
    stw r4,MBAR_CDM+CDM_CLK_ENABLE_OFF(r3)
    eieio

    /* Set SDELAY, must be set to 4 */
    li  r4,4
    stw r4,MBAR_SDRAM+SDRAM_SDELAY_OFF(r3)
    eieio


#ifndef BOOT_FROM_UBOOT
    /* Initialize SDRAM controller. */
    bl  romInitSdram

    /* Switch to execution from CS0 */
    bl  romInitCS01
#else
	/* 
	 * if all four values at 0x000FFF00 are
	 * equal we assume this is the startType 
	 */
	lis r4,0xF
	ori r4,r4,0xFF00
	lwz  r3,0(r4)
	lwz  r5,4(r4)
	cmplw r3,r5
	bne ultracold
	lwz  r5,8(r4)
	cmplw r3,r5
	bne ultracold
	lwz  r5,12(r4)
	cmplw r3,r5
	beq warm
ultracold:	
#endif /* BOOT_FROM_UBOOT */
    li  r24, BOOT_COLD
    bl  start           /* skip over next instruction */


warm:
    or  r24, r3, r3     /* startType to r24 */

start:

#ifdef BOOT_FROM_UBOOT

	/* clear MSR (set to 0xb002 from U-Boot */

	andi.   r3, r3, 0
	sync
	mtmsr   r3
	isync
	
#endif /* BOOT_FROM_UBOOT */

#ifdef BTT_DATA_ADDR
	li		r3,-1
	sync
	mtspr	DEC,r3
	isync
#endif
	
    /* Zero-out registers: r0 & SPRGs */

    xor     r0,r0,r0
    mtspr   272,r0
    mtspr   273,r0
    mtspr   274,r0
    mtspr   275,r0
    mtspr   276,r0
    mtspr   277,r0
    mtspr   278,r0
    mtspr   279,r0

    /* initialize the stack pointer */

    lis     sp, HI(STACK_ADRS)
    ori     sp, sp, LO(STACK_ADRS)

    /*
     *  Set MPU/MSR to a known state
     *  Turn on FP
     */

    andi.   r3, r3, 0
    ori r3, r3, _PPC_MSR_FP
    sync
    mtmsr   r3
    isync

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

    /* Initialize the floating point data registers to a known state */

    bl  ifpdr_value
    .long   0x3f800000  /* 1.0 */
ifpdr_value:
    mfspr   r3,8
    lfs f0,0(r3)
    lfs f1,0(r3)
    lfs f2,0(r3)
    lfs f3,0(r3)
    lfs f4,0(r3)
    lfs f5,0(r3)
    lfs f6,0(r3)
    lfs f7,0(r3)
    lfs f8,0(r3)
    lfs f9,0(r3)
    lfs f10,0(r3)
    lfs f11,0(r3)
    lfs f12,0(r3)
    lfs f13,0(r3)
    lfs f14,0(r3)
    lfs f15,0(r3)
    lfs f16,0(r3)
    lfs f17,0(r3)
    lfs f18,0(r3)
    lfs f19,0(r3)
    lfs f20,0(r3)
    lfs f21,0(r3)
    lfs f22,0(r3)
    lfs f23,0(r3)
    lfs f24,0(r3)
    lfs f25,0(r3)
    lfs f26,0(r3)
    lfs f27,0(r3)
    lfs f28,0(r3)
    lfs f29,0(r3)
    lfs f30,0(r3)
    lfs f31,0(r3)
    sync

    /*
     *  Set MPU/MSR to a known state
     *  Turn off FP
     */

    andi.   r3, r3, 0
    sync
    mtmsr   r3
    isync

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

    mfspr   r3,HID0         /* r3 = HID0 */
    andi.   r3,r3,0xcfff        /* make sure lock bits are clear */
    ori r4,r3,(_PPC_HID0_ICE | _PPC_HID0_DCE | _PPC_HID0_ICFI | _PPC_HID0_DCFI)
                                        /* r4 has ICE,DCE,ICI,DCI bits set */
    andi.   r3,r3,0x03ff        /* r3 has enable bits cleared */
    sync
    mtspr   HID0,r4         /* HIDO = r4 */
    isync
    sync
    mtspr   HID0,r3         /* HIDO = r3 */
    isync

    bl  romClearBATs            /* clear BATs before initialise them */
    bl  romInvalidateTLBs

#ifdef INCLUDE_CACHE_SUPPORT
    /* turn the Instruction cache ON for faster FLASH ROM boots */

#   ifdef USER_I_CACHE_ENABLE
    mfspr   r3,HID0         /* r3 = HID0 */
    ori r3, r3, 0x8000      /* set ICE bit */

    /*
     * The setting of the instruction cache enable (ICE) bit must be
     * preceded by an isync instruction to prevent the cache from being
     * enabled or disabled while an instruction access is in progress.
     */
    isync
    mtspr   HID0, r3
#   endif /* USER_I_CACHE_ENABLE */
#endif /* INCLUDE_CACHE_SUPPORT */

    /* go to C entry point */

    or      r3, r24, r24            /* Restore startType */
    addi    sp, sp, -FRAMEBASESZ    /* get frame stack */

#ifdef BOOT_FROM_UBOOT
#   if (UBOOT_BOOT_OPTION == TFTP_BOOT)
	bl	romStart
#   endif /* UBOOT_BOOT_OPTION */
#endif /* BOOT_FROM_UBOOT */

    lis     r6, HI(romStart)
    ori     r6, r6, LO(romStart)
    lis     r7, HI(romInit)
    ori     r7, r7, LO(romInit)
    lis     r8, HI(ROM_TEXT_ADRS)
    ori     r8, r8, LO(ROM_TEXT_ADRS)
    sub     r6, r6, r7
    add     r6, r6, r8
    mtlr    r6
    blr
	
FUNC_END(romInit)
FUNC_END(_romInit)

/***************************************************************************
*
* romInitCS01 - Set Chip Select 0,1 / Boot Configuration
*
* Initialize CS0/CS1 and switch to execution from there.
*
* SYNOPSIS
* \ss
* void romInitCS0
*     (
*     void
*     )
* \se
*
* RETURNS: N/A
*/

FUNC_BEGIN(romInitCS01)

    /* Save LR to R31. */
    mflr    r31

    /* R3 points to internal peripheral space. */
    mfspr   r3,MBAR

    /* Set Chip Select 0 / Boot ROM Configuration Register */

    lis r4,HI(LPC_CS0_BOOT_CFG_VALUE)
    ori r4,r4,LO(LPC_CS0_BOOT_CFG_VALUE)
    stw r4,MBAR_LPC+LPC_CS0_BOOT_CFG_OFF(r3)
    eieio

    /* Set Chip Select 1 Configuration Register */

    lis r4,HI(LPC_CS1_CFG_VALUE)
    ori r4,r4,LO(LPC_CS1_CFG_VALUE)
    stw r4,MBAR_LPC+LPC_CS1_CFG_OFF(r3)
    eieio

    /* Set Chip Select Deadcycle Control Register */

    lis r4,HI(LPC_CS_DEAD_CTRL_VALUE)
    ori r4,r4,LO(LPC_CS_DEAD_CTRL_VALUE)
    stw r4,MBAR_LPC+LPC_CS_DEAD_CTRL_OFF(r3)
    eieio

    /* Disable Burst for Flashes */
    /* (Write to Chip Select Burst Control Register) */
    /* (No Burst is used) */

    lis r4,0
    stw r4,MBAR_LPC+LPC_CS_BURST_CTRL_OFF(r3)
    eieio

    /* Enable CS module and external bus error signalling. */
    li r4,HI(LPC_CS_CTRL_EBEE|LPC_CS_CTRL_ME)
    addi r4,0,LO(LPC_CS_CTRL_EBEE|LPC_CS_CTRL_ME)
    sth r4,MBAR_LPC+LPC_CS_CTRL_OFF(r3)

    eieio

#define CS0_FLASH_STOP (CS0_FLASH_BASE_ADRS+CS0_FLASH_SIZE-1)

    /* Set CS0 start and stop address. */
    lis r4,HI(CS0_FLASH_BASE_ADRS>>16)
    ori r4,r4,LO(CS0_FLASH_BASE_ADRS>>16)
    stw r4,MBAR_IPBI+IPBI_CS0_START_OFF(r3)
    eieio

    lis r4,HI(CS0_FLASH_STOP>>16)
    ori r4,r4,LO(CS0_FLASH_STOP>>16)
    stw r4,MBAR_IPBI+IPBI_CS0_STOP_OFF(r3)
    eieio

#if defined(CS1_FLASH_BASE_ADRS)
#define CS1_FLASH_STOP (CS1_FLASH_BASE_ADRS+CS1_FLASH_SIZE-1)

    /* Set CS1 start and stop address. */
    lis r4,HI(CS1_FLASH_BASE_ADRS>>16)
    ori r4,r4,LO(CS1_FLASH_BASE_ADRS>>16)
    stw r4,MBAR_IPBI+IPBI_CS1_START_OFF(r3)
    eieio

    lis r4,HI(CS1_FLASH_STOP>>16)
    ori r4,r4,LO(CS1_FLASH_STOP>>16)
    stw r4,MBAR_IPBI+IPBI_CS1_STOP_OFF(r3)
    eieio
#endif

    /* Set optimized CS settings. */
    /* TODO */

    /* Check for boot low or boot high (IP bit in MSR). */
    sync
    mfmsr   r4
    sync
    andi.   r4,r4,0x0040
    bgt boot_high

boot_low:
    /* CSBoot on, CS0, CS1 on */
    lis r4,HI(IPBI_CTRL_WSE | IPBI_CTRL_CSBOOT | IPBI_CTRL_CS0 | IPBI_CTRL_CS1)
    ori r4,r4,LO(IPBI_CTRL_WSE | IPBI_CTRL_CSBOOT | IPBI_CTRL_CS0 | IPBI_CTRL_CS1)
    sync
    stw r4,MBAR_IPBI+IPBI_CTRL_OFF(r3)
    isync
    eieio

    /* New start address of Flash */
    lis r4,HI(CS0_FLASH_BASE_ADRS)
    ori r4,r4,LO(CS0_FLASH_BASE_ADRS)

    /* Adjust return address */
    add r31,r31,r4

    /* Switch from executing in CSBoot space in CS0 space with */
    /* an absolute jump. */
    lis r5,HI(boot_high)
    ori r5,r5,LO(boot_high)
    lis     r7, HI(romInit)
    ori r7, r7, LO(romInit)
    lis     r8, HI(ROM_TEXT_ADRS)
    ori r8, r8, LO(ROM_TEXT_ADRS)
    sub r5, r5, r7
    add r5, r5, r8
    mtlr    r5
    blr

boot_high:
    /* CSBoot off, CS0/1 on */
    lis r4,HI(IPBI_CTRL_WSE | IPBI_CTRL_CS0 | IPBI_CTRL_CS1)
    ori r4,r4,LO(IPBI_CTRL_WSE | IPBI_CTRL_CS0 | IPBI_CTRL_CS1)
    sync
    stw r4,MBAR_IPBI+IPBI_CTRL_OFF(r3)
    isync
    eieio

    /* return */
    mtlr    r31
    blr

FUNC_END(romInitCS01)

/***************************************************************************
*
* romInitSdram - MPC5200 specific initialation - set SDRAM controller
*
* Initialize SDRAM controller.
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

    /* R3 points to internal peripheral space. */
    mfspr   r3,MBAR

    /* RAM devided onto two CS */

#define CS0_RAM_ADDR LOCAL_MEM_LOCAL_ADRS 
#define CS1_RAM_ADDR (LOCAL_MEM_LOCAL_ADRS+(LOCAL_MEM_SIZE/2))

    /* Set SDRAM CS0 Configuration Register. */
    lis r4,HI((CS0_RAM_ADDR & 0xFFF00000) | (DRAM_BITS-1))
    ori r4,r4,LO((CS0_RAM_ADDR & 0xFFF00000) | (DRAM_BITS-1))
    stw r4,MBAR_IPBI+IPBI_SDRAM_CONF0_OFF(r3)
    eieio

    /* Set SDRAM CS1 Configuration Register. */
    lis r4,HI((CS1_RAM_ADDR & 0xFFF00000) | (DRAM_BITS-1))
    ori r4,r4,LO((CS1_RAM_ADDR & 0xFFF00000) | (DRAM_BITS-1))
    stw r4,MBAR_IPBI+IPBI_SDRAM_CONF1_OFF(r3)
    eieio


    /* hi_addr = 0 */

    /*
     * Wait for 200 us (JEDEC Specification)
     * (Required for proper DDR-SDRAM Initialisation)
     *
     *     CPU Slice Timer 0 is used
     */


    /*
     * Reset Slice Timer 0
     * (Write to Slice Timer 0 Control Register)
     * (Note: Interrupts are disabled)
     */

    li  r4,0
    stw r4,MBAR_SLT+SLT0_CTRL_OFF(r3)

    /*
     * Reset Slice Timer 0 Status Bit
     * (Write to Slice Timer 0 Status Register)
     */

    lis r4,HI(SLT_STATUS_RESET)
    ori r4,r4,LO(SLT_STATUS_RESET)
    stw r4,MBAR_SLT+SLT0_STATUS_OFF(r3)

    /*
     * Load Counter Value
     * (Write to Slice Timer 0 Terminal Count Register)
     * (T = 7.75ns, 200us = 26400 x T)
     */

        li  r4,0x00006720
        stw r4,MBAR_SLT+SLT0_TCNT_OFF(r3)

    /* Start Slice Timer 0 (Set Timer Enable Bit) */

    lis r4,HI(SLT_CTRL_ENABLE)
    ori r4,r4,LO(SLT_CTRL_ENABLE)
    stw r4,MBAR_SLT+SLT0_CTRL_OFF(r3)

    /*
     * Wait until Slice Timer 0 is finished
     * (Check for Status Bit in Slice Timer 0 Status Register)
     */

    lis r5,HI(SLT_STATUS_RESET)
    ori r5,r5,LO(SLT_STATUS_RESET)

1:
    lwz r4,MBAR_SLT+SLT0_STATUS_OFF(r3)
    and r4,r4,r5
    cmplw   r4,r5
    bne 1b

        /*
         * Finally Stop Slice Timer 0
         * (Write to Slice Timer 0 Control Register)
         */

    li  r4,0
    stw r4,MBAR_SLT+SLT0_CTRL_OFF(r3)
    eieio


    /* Write Config 1. */
    lis r4,HI(SDRAM_CONFIG1)
    ori r4,r4,LO(SDRAM_CONFIG1)
    stw r4,MBAR_SDRAM+SDRAM_CONF1_OFF(r3)
    eieio


    /* Write Config 2. */
    lis r4,HI(SDRAM_CONFIG2)
    ori r4,r4,LO(SDRAM_CONFIG2)
    stw r4,MBAR_SDRAM+SDRAM_CONF2_OFF(r3)
    eieio


    /* Write Control */
    lis r4,HI(SDRAM_CONTROL | 0x80000000)
    ori r4,r4,LO(SDRAM_CONTROL | 0x80000000)
    stw r4,MBAR_SDRAM+SDRAM_CTRL_OFF(r3)
    eieio

    /* Issue Precharge All command */
    lis r4,HI(SDRAM_CONTROL | 0x80000002)
    ori r4,r4,LO(SDRAM_CONTROL | 0x80000002)
    stw r4,MBAR_SDRAM+SDRAM_CTRL_OFF(r3)
    eieio

#if (DDR == 1)
    /* Write Extended Mode to enable DLL */
    li  r4,SDRAM_EXTD_MODE
    sth r4,MBAR_SDRAM+SDRAM_MODE_OFF(r3)
    eieio
#endif

    /* Configure DDR ICs by resetting DLL */
    li  r4,(SDRAM_MODE | 0x0400)
    sth r4,MBAR_SDRAM+SDRAM_MODE_OFF(r3)
    eieio

    /*
     * Wait for 200 us (JEDEC Specification)
     * (Required for proper DDR-SDRAM Initialisation)
     * CPU Slice Timer 0 is used
     */


    /*
     * Reset Slice Timer 0
     * (Write to Slice Timer 0 Control Register)
     * (Note: Interrupts are disabled)
     */

    li  r4,0
    stw r4,MBAR_SLT+SLT0_CTRL_OFF(r3)

    /*
     * Reset Slice Timer 0 Status Bit
     * (Write to Slice Timer 0 Status Register)
     */

    lis r4,HI(SLT_STATUS_RESET)
    ori r4,r4,LO(SLT_STATUS_RESET)
    stw r4,MBAR_SLT+SLT0_STATUS_OFF(r3)

    /*
     * Load Counter Value
     * (Write to Slice Timer 0 Terminal Count Register)
     * (T = 7.75ns, 200us = 26400 x T)
     */

    li  r4,0x00006720
    stw r4,MBAR_SLT+SLT0_TCNT_OFF(r3)

    /* Start Slice Timer 0 (Set Timer Enable Bit) */

    lis r4,HI(SLT_CTRL_ENABLE)
    ori r4,r4,LO(SLT_CTRL_ENABLE)
    stw r4,MBAR_SLT+SLT0_CTRL_OFF(r3)

    /*
     * Wait until Slice Timer 0 is finished
     * (Check for Status Bit in Slice Timer 0 Status Register)
     */

    lis r5,HI(SLT_STATUS_RESET)
    ori r5,r5,LO(SLT_STATUS_RESET)
2:
    lwz r4,MBAR_SLT+SLT0_STATUS_OFF(r3)
    and r4,r4,r5
    cmplw   r4,r5
    bne 2b

    /*
     * Finally Stop Slice Timer 0
     * (Write to Slice Timer 0 Control Register)
     */

    li  r4,0
    stw r4,MBAR_SLT+SLT0_CTRL_OFF(r3)
    eieio

    /* Issue Precharge All followed by a refresh. */
    lis r4,HI(SDRAM_CONTROL | 0x80000002)
    ori r4,r4,LO(SDRAM_CONTROL | 0x80000002)
    stw r4,MBAR_SDRAM+SDRAM_CTRL_OFF(r3)
    eieio

    /* SDRAM is now in idle state */

    /* Do 1x Auto Refresh */
    /* (Write to MC SDRAM Control Register) */

    lis r4,HI(SDRAM_CONTROL | 0x80000004)
    ori r4,r4,LO(SDRAM_CONTROL | 0x80000004)
    stw r4,MBAR_SDRAM+SDRAM_CTRL_OFF(r3)
    eieio


    /* Set SDRAM to Normal Operation */
    li  r4,SDRAM_MODE
    sth r4,MBAR_SDRAM+SDRAM_MODE_OFF(r3)
    eieio

    /* Write Control, lock Mode Register. */
    lis r4,HI(SDRAM_CONTROL)
    ori r4,r4,LO(SDRAM_CONTROL)
    stw r4,MBAR_SDRAM+SDRAM_CTRL_OFF(r3)
    eieio

    /* repeat with hi-addr */

    /* Write Control */
    lis r4,HI(SDRAM_CONTROL | 0x81000000)
    ori r4,r4,LO(SDRAM_CONTROL | 0x81000000)
    stw r4,MBAR_SDRAM+SDRAM_CTRL_OFF(r3)
    eieio

    /* Issue Precharge All command */
    lis r4,HI(SDRAM_CONTROL | 0x81000002)
    ori r4,r4,LO(SDRAM_CONTROL | 0x81000002)
    stw r4,MBAR_SDRAM+SDRAM_CTRL_OFF(r3)
    eieio

    /* Write Extended Mode to enable DLL */
    li  r4,SDRAM_EXTD_MODE
    sth r4,MBAR_SDRAM+SDRAM_MODE_OFF(r3)
    eieio

    /* Configure DDR ICs by resetting DLL */
    li  r4,(SDRAM_MODE | 0x0400)
    sth r4,MBAR_SDRAM+SDRAM_MODE_OFF(r3)
    eieio

    /*
     * Wait for 200 us (JEDEC Specification)
     * (Required for proper DDR-SDRAM Initialisation)
     * CPU Slice Timer 0 is used
     */


    /*
     * Reset Slice Timer 0
     * (Write to Slice Timer 0 Control Register)
     * (Note: Interrupts are disabled)
     */

    li  r4,0
    stw r4,MBAR_SLT+SLT0_CTRL_OFF(r3)

    /*
     * Reset Slice Timer 0 Status Bit
     * (Write to Slice Timer 0 Status Register)
     */

    lis r4,HI(SLT_STATUS_RESET)
    ori r4,r4,LO(SLT_STATUS_RESET)
    stw r4,MBAR_SLT+SLT0_STATUS_OFF(r3)

    /*
     * Load Counter Value
     * (Write to Slice Timer 0 Terminal Count Register)
     * (T = 7.75ns, 200us = 26400 x T)
     */

    li  r4,0x00006720
    stw r4,MBAR_SLT+SLT0_TCNT_OFF(r3)

    /* Start Slice Timer 0 (Set Timer Enable Bit) */

    lis r4,HI(SLT_CTRL_ENABLE)
    ori r4,r4,LO(SLT_CTRL_ENABLE)
    stw r4,MBAR_SLT+SLT0_CTRL_OFF(r3)

    /*
     * Wait until Slice Timer 0 is finished
     * (Check for Status Bit in Slice Timer 0 Status Register)
     */

    lis r5,HI(SLT_STATUS_RESET)
    ori r5,r5,LO(SLT_STATUS_RESET)
3:
    lwz r4,MBAR_SLT+SLT0_STATUS_OFF(r3)
    and r4,r4,r5
    cmplw   r4,r5
    bne 3b

    /*
     * Finally Stop Slice Timer 0
     * (Write to Slice Timer 0 Control Register)
     */

    li  r4,0
    stw r4,MBAR_SLT+SLT0_CTRL_OFF(r3)
    eieio

    /* Issue Precharge All followed by a refresh. */
    lis r4,HI(SDRAM_CONTROL | 0x81000002)
    ori r4,r4,LO(SDRAM_CONTROL | 0x81000002)
    stw r4,MBAR_SDRAM+SDRAM_CTRL_OFF(r3)
    eieio

    /* SDRAM is now in idle state */

    /* Do 1x Auto Refresh */
    /* (Write to MC SDRAM Control Register) */

    lis r4,HI(SDRAM_CONTROL | 0x81000004)
    ori r4,r4,LO(SDRAM_CONTROL | 0x81000004)
    stw r4,MBAR_SDRAM+SDRAM_CTRL_OFF(r3)
    eieio

    /* Set SDRAM to Normal Operation */
    li  r4,SDRAM_MODE
    sth r4,MBAR_SDRAM+SDRAM_MODE_OFF(r3)
    eieio

    /* Write Control, lock Mode Register. */
    lis r4,HI(SDRAM_CONTROL | 0x01000000)
    ori r4,r4,LO(SDRAM_CONTROL | 0x01000000)
    stw r4,MBAR_SDRAM+SDRAM_CTRL_OFF(r3)
    eieio

    /* Write Tap Delay. */
    li  r4,SDRAM_TAP_DEL
    stb r4,MBAR_CDM+CDM_PORSTCFG_OFF(r3)
    eieio

    /* return */
    blr

FUNC_END(romInitSdram)

/*******************************************************************************
* romInvalidateTLBs - invalidate the TLB's.
*
* This routine will invalidate the TLB's.
*
* SYNOPSIS
* \ss
* void romInvalidateTLBs
*     (
*     void
*     )
* \se
*
* SEE ALSO: romClearBATs(), romMinimumBATsInit()
*
* RETURNS: N/A
*/

FUNC_BEGIN(romInvalidateTLBs)
    isync
    /* invalidate entries within both TLBs */
    li      r3,128
    mtctr   r3       /* CTR = 32  */
    xor     r3,r3,r3 /* r3 = 0    */
    isync            /* context sync req'd before tlbie */
tlbloop:
    tlbie   r3
    addi    r3,r3,0x1000 /* increment bits 15-19 */
    sync                 /* sync instr req'd after tlbie      */
    bdnz    tlbloop      /* decrement CTR, branch if CTR != 0 */
    blr
FUNC_END(romInvalidateTLBs)

/***************************************************************************
*
* romClearBATs - clearing all the BAT's register.
*
* This routine will zero the BAT's register.
*
* SYNOPSIS
* \ss
* void romClearBATs
*     (
*     void
*     )
* \se
*
* SEE ALSO: romInvalidateTLBs(), romMinimumBATsInit()
*
* RETURNS: N/A
*/

FUNC_BEGIN(romClearBATs)
        /* zero out the BAT registers */
    xor r3,r3,r3
    isync
    mtspr   IBAT0U,r3   /* clear all upper BATS first */
    mtspr   IBAT1U,r3
    mtspr   IBAT2U,r3
    mtspr   IBAT3U,r3
    mtspr   IBAT4U,r3
    mtspr   IBAT5U,r3
    mtspr   IBAT6U,r3
    mtspr   IBAT7U,r3

    mtspr   DBAT0U,r3
    mtspr   DBAT1U,r3
    mtspr   DBAT2U,r3
    mtspr   DBAT3U,r3
    mtspr   DBAT4U,r3
    mtspr   DBAT5U,r3
    mtspr   DBAT6U,r3
    mtspr   DBAT7U,r3

    mtspr   IBAT0L,r3   /* then clear lower BATS */
    mtspr   IBAT1L,r3
    mtspr   IBAT2L,r3
    mtspr   IBAT3L,r3
    mtspr   IBAT4L,r3
    mtspr   IBAT5L,r3
    mtspr   IBAT6L,r3
    mtspr   IBAT7L,r3

    mtspr   DBAT0L,r3
    mtspr   DBAT1L,r3
    mtspr   DBAT2L,r3
    mtspr   DBAT3L,r3
    mtspr   DBAT4L,r3
    mtspr   DBAT5L,r3
    mtspr   DBAT6L,r3
    mtspr   DBAT7L,r3

    isync

    /* disable BAT4-7 for now */
    mfspr   r3, HID2        /* HID2 */
    lis     r4, HI(0xFFFBFFFF)
    ori     r4, r4, LO(0xFFFBFFFF)
    and     r3, r3, r4      /* clear HBE */
    sync
    mtspr   HID2, r3        /* HID2 */
    isync

    blr
FUNC_END(romClearBATs)

