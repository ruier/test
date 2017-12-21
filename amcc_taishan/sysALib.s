/* sysALib.s - ocotea system-dependent assembly routines */

/***************************************************************************
   This source and object code has been made available to you by IBM on an
   AS-IS basis.

   IT IS PROVIDED WITHOUT WARRANTY OF ANY KIND, INCLUDING THE WARRANTIES OF
   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE OR OF NONINFRINGEMENT
   OF THIRD PARTY RIGHTS.  IN NO EVENT SHALL IBM OR ITS LICENSORS BE LIABLE
   FOR INCIDENTAL, CONSEQUENTIAL OR PUNITIVE DAMAGES.  IBM'S OR ITS LICENSOR'S
   DAMAGES FOR ANY CAUSE OF ACTION, WHETHER IN CONTRACT OR IN TORT, AT LAW OR
   AT EQUITY, SHALL BE LIMITED TO A MAXIMUM OF $1,000 PER LICENSE.  No license
   under IBM patents or patent applications is to be implied by the copyright
   license.

   Any user of this software should understand that neither IBM nor its
   licensors will be responsible for any consequences resulting from the use
   of this software.

   Any person who transfers this source code or any derivative work must
   include the IBM copyright notice, this paragraph, and the preceding two
   paragraphs in the transferred software.

   Any person who transfers this object code or any derivative work must
   include the IBM copyright notice in the transferred software.

   COPYRIGHT   I B M   CORPORATION 2000
   LICENSED MATERIAL  -  PROGRAM PROPERTY OF  I B M"

***************************************************************************/

/*
 * Copyright (c) 2002-2003, 2008-2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01i,20sep09,syt  Re-initialize TLB entry.(WIND00128808)
01h,02sep08,x_f  Remove sysDcr.s include
01g,17nov03,pch  fix typos in UIC clearing
01f,12aug03,jtp  Formal code inspection changes. vx routines moved to arch
                 lib; removed unnecessary initialization, added UIC2 and
                 UICB init.
01e,18jul03,jtp  Remove unneeded code
01d,05jun03,jtp  Remove TLB invalidation section; let bootrom handle it
01c,29may03,pch  Add PPC440_x5 architecture support;
                 partitioning to enable compressed bootrom build
01b,28may03,jtp  Replace @h with HI() and @l with LO()
01a,10dec02,saw  based on 440GP v01f 
*/

/*
DESCRIPTION
This module contains system-dependent routines written in assembly
language.

This module must be the first specified in the \f3ld\f1 command used to
build the system. The sysInit() routine is the system start-up code.
*/

#define _ASMLANGUAGE
#include "vxWorks.h"
#include "sysLib.h"
#include "config.h"
#include "asm.h"
#include "regs.h"
#include "arch/ppc/excPpcLib.h"
#include "arch/ppc/mmu440Lib.h"

/*
 * MMUCR register initialization
 *
 * Specify TID field to be written to TLB entries.  TID = 0
 * Specify TS field to be used for TLB searches. TS = 0
 * Specify store miss alloc behavior.  On a cache miss, allocate a
 *     line
 * Disable transient cache mechanism
 * Disable cache locking exceptions
 */

#define MMUCR_INIT  0x00000000

/* TLB setup defines and functions */

#define TLB_V   0x00000200  /* TLB Word 0, Valid Bit */
#define TLB_W   0x00000800  /* TLB Word 2, Writethrough Cache Bit */
#define TLB_I   0x00000400  /* TLB Word 2, Inhibit Caching Bit  */
#define TLB_M   0x00000200  /* TLB Word 2, Memory Coherency Cache Bit */
#define TLB_G   0x00000100  /* TLB Word 2, Guarded Cache Bit */
#define TLB_E   0x00000080  /* TLB Word 2, Endianness Bit */
#define TLB_UX  0x00000020  /* TLB Word 2, User Executable Bit */
#define TLB_UW  0x00000010  /* TLB Word 2, User Writeable Bit */
#define TLB_UR  0x00000008  /* TLB Word 2, User Readable Bit */
#define TLB_SX  0x00000004  /* TLB Word 2, Supervisor Executable Bit */
#define TLB_SW  0x00000002  /* TLB Word 2, Supervisor Writeable Bit */
#define TLB_SR  0x00000001  /* TLB Word 2, Supervisor Readable Bit */

/*
 * define NOCACHE to keep cache off when initializing TLBs.
 * This must be defined, it is presently the only supported mode.
 */

#define NOCACHE

/*
 * Define ideal caching modes for setting up the cacheability of each
 * address region mapped by a TLB entry.  If caching is disabled with
 * NOCACHE, all cached modes will revert to cache-inhibited.
 */

#define _INHIBIT                TLB_I
#define _INHIBIT_AND_G          TLB_I | TLB_G
#ifdef NOCACHE
#   define _WRITETHROUGH        TLB_I
#   define _WRITETHROUGH_OR_G   TLB_I | TLB_G
#   define _WRITETHROUGH_AND_G  TLB_I | TLB_G
#else /* NOCACHE */
#   define _WRITETHROUGH        TLB_W | TLB_G
#   define _WRITETHROUGH_OR_G   TLB_W
#   define _WRITETHROUGH_AND_G  TLB_W | TLB_G
#endif /* NOCACHE */

/*
 * When the boot space is configured as writeable (example: when
 * jumpering it to SRAM for JTAG-based debugging), add the writeable
 * bit.
 */

#ifdef BOOT_SPACE_WRITEABLE
#   define BOOT_SPACE_WRITE     TLB_UW | TLB_SW
#else /* BOOT_SPACE_WRITEABLE */
#   define BOOT_SPACE_WRITE     0
#endif /* BOOT_SPACE_WRITEABLE */

/*
 * The TLB_NEXT and TLB_WORD_SET macros are used to set up each TLB
 * entry, which consists of three words. Using macros reduces the
 * opportunity for error.
 */

#define TLB_NEXT(ixReg) \
        addi  ixReg, ixReg, 0x0001 /* Increment to the next TLB entry */

#define TLB_WORD_SET(wordNum, ixReg, tempReg, value)  \
        lis   tempReg, HI(value)                    ; \
        ori   tempReg, tempReg, LO(value)           ; \
        tlbwe tempReg, ixReg, wordNum   /* Write TLB Word wordNum */

    /* globals */

    FUNC_EXPORT(_sysInit)           /* start of system code */
#ifdef INCLUDE_PCI
    FUNC_EXPORT(sysPciInByte)
    FUNC_EXPORT(sysPciOutByte)
    FUNC_EXPORT(sysPciInWord)
    FUNC_EXPORT(sysPciOutWord)
    FUNC_EXPORT(sysPciInLong)
    FUNC_EXPORT(sysPciOutLong)
    FUNC_EXPORT(sysInWord)
    FUNC_EXPORT(sysOutWord)
    FUNC_EXPORT(sysInLong)
    FUNC_EXPORT(sysOutLong)
#endif /* INCLUDE_PCI */
    FUNC_EXPORT(sysPlbInLong)
    FUNC_EXPORT(sysPlbOutLong)

    /* externals */

    FUNC_IMPORT(usrInit)


    _WRS_TEXT_SEG_START

/*******************************************************************************
*
* sysInit - start after boot
*
* This routine is the system start-up entry point for VxWorks in RAM, the
* first code executed after booting.  It disables interrupts, sets up the
* stack, and jumps to the C routine usrInit() in usrConfig.c.
*
* The initial stack is set to grow down from the address of sysInit().
* This stack is used only by usrInit() and is never used again. Memory
* for the stack must be accounted for when determining the system load
* address.
*
* NOTE: This routine should not be called by the user.
*
* RETURNS: N/A

* sysInit (void)    /@ THIS IS NOT A CALLABLE ROUTINE @/

*/

FUNC_BEGIN(_sysInit)
    mr      r8, r3
    xor r0, r0, r0
    addi    r1, r0, -1

    /*
     * PPC BSPs normally invalidate & clear all TLB entries here,
     * but Book E processors always have some TLB entries that must
     * be valid.  We cannot be sure how many and which ones have
     * been set up by romInit, so this BSP skips the step of
     * clearing them.
     */

    /* Disable all interrupts */

    mtmsr   r0
    isync
    
    /* General MMU initialization */

    addis r0, r0, HI(MMUCR_INIT)
    ori   r0, r0, LO(MMUCR_INIT)
    mtspr MMUCR, r0
    
    /*
     * Write TLB entry for initial program memory page
     *
     * - Specify EPN, RPN, ERPN, and SIZE as appropriate for system
     * - Set valid bit
     * - Specify TID=0
     * - Specify TS=0 or else MSR[IS,DS] must be set to correspond to TS=1
     * - Specify storage attributes (W, I, M, G, E, U0 - U3) as appropriate
     * - Enable supervisor mode fetch, read, and write access (SX, SR, SW)
     */
    
    xor  r1, r1, r1      /* start with TLB #0 */

#ifdef INCLUDE_440X5_TLB_RECOVERY_MAX
    /*
     * Tlb #0.  DDR SDRAM Machine Check Handler - Uncached
     *     VEC_BASE_ADRS + 0x4000 -> 0x000004000.  16KB
     * Attributes: X/W/R
     */
    
    TLB_WORD_SET(0, r1, r0, (VEC_BASE_ADRS + 0x4000) | TLB_V | _MMU_TLB_SZ_16K)
    TLB_WORD_SET(1, r1, r0, 0x0 | 0x000004000)
    TLB_WORD_SET(2, r1, r0, _INHIBIT_AND_G | TLB_UX | TLB_UW | TLB_UR | TLB_SX | TLB_SW | TLB_SR)
    TLB_NEXT(r1)
#endif /* INCLUDE_440X5_TLB_RECOVERY_MAX */

    /*
     * Tlb #0 (or 1).  DDR SDRAM 0 - Cached. This includes the Machine
     * Check Handler when not using the INCLUDE_440X5_TLB_RECOVERY_MAX
     * option.
     *     VEC_BASE_ALT_ADRS -> 0x000000000.  256MB
     * Attributes: X/W/R
     */

    TLB_WORD_SET(0, r1, r0, VEC_BASE_ALT_ADRS | TLB_V | _MMU_TLB_SZ_256M)
    TLB_WORD_SET(1, r1, r0, 0x0 | 0x000000000)
    TLB_WORD_SET(2, r1, r0, _WRITETHROUGH_AND_G | TLB_UX | TLB_UW | TLB_UR | TLB_SX | TLB_SW | TLB_SR)

#ifndef INCLUDE_440X5_TLB_RECOVERY_MAX
    /*
     * Tlb #1 unless INCLUDE_440X5_TLB_RECOVERY_MAX.
     * DDR SDRAM Machine Check Handler - Uncached
     *     0x80000000 -> 0x000000000.  256MB
     * TLB_NUM_SDRAM_0 must be coordinated with this setting
     * Attributes: I/W/R
     */

    TLB_NEXT(r1)
    TLB_WORD_SET(0, r1, r0, 0x80000000 | TLB_V | _MMU_TLB_SZ_256M)
    TLB_WORD_SET(1, r1, r0, 0x0 | 0x000000000)  /* RPN */
    TLB_WORD_SET(2, r1, r0, _INHIBIT | TLB_UW | TLB_UR | TLB_SW | TLB_SR)
#endif /* INCLUDE_440X5_TLB_RECOVERY_MAX */

    /*
     * Tlb #2.  Peripherals - 0xE0000000 -> 0x140000000.  256MB
     * Attributes: I/G/W/R
     */

    TLB_NEXT(r1)
    TLB_WORD_SET(0, r1, r0, 0xe0000000  | TLB_V | _MMU_TLB_SZ_256M)
    TLB_WORD_SET(1, r1, r0, 0x1 | 0x40000000)
    TLB_WORD_SET(2, r1, r0, _INHIBIT_AND_G | TLB_UW | TLB_UR | TLB_SW | TLB_SR)

    /*
     * Tlb #3.  Boot Space - 0xF0000000 -> 0x1F0000000.  256MB
     * TLB_NUM_BOOT_SPACE must be coordinated with this setting
     * Attributes: W/X/R
     */

    TLB_NEXT(r1)                    /* next TLB */
    TLB_WORD_SET(0, r1, r0, 0xf0000000      /* EPN */ \
             | TLB_V | _MMU_TLB_SZ_256M)
    TLB_WORD_SET(1, r1, r0, 0x1 | 0xf0000000)   /* RPN */
    TLB_WORD_SET(2, r1, r0, _WRITETHROUGH | BOOT_SPACE_WRITE \
             | TLB_UX |          TLB_UR | TLB_SX          | TLB_SR)

    /*
     * Tlb #4.  PCI Memory - 0x90000000 -> 0x300000000.  256MB
     * Attributes: I/G/X/W/R
     * Used for PCI memory that does not allow prefetching.
     */

    TLB_NEXT(r1)
    TLB_WORD_SET(0, r1, r0, 0x90000000 | TLB_V | _MMU_TLB_SZ_256M)
    TLB_WORD_SET(1, r1, r0, 0x3 | 0x000000000)
    TLB_WORD_SET(2, r1, r0, _INHIBIT_AND_G | TLB_UX | TLB_UW | TLB_UR | TLB_SX | TLB_SW | TLB_SR)

    /*
     * Tlb #5.  PCI Memory - 0xa0000000 -> 0x310000000.  256MB
     * Attributes: X/W/R
     */

    TLB_NEXT(r1)
    TLB_WORD_SET(0, r1, r0, 0xa0000000 | TLB_V | _MMU_TLB_SZ_256M)
    TLB_WORD_SET(1, r1, r0, 0x3 | 0x10000000)
    TLB_WORD_SET(2, r1, r0, _INHIBIT | TLB_UX | TLB_UW | TLB_UR | TLB_SX | TLB_SW | TLB_SR)

    /*
     * Tlb #6.  I2O Memory - 0xb0000000 -> 0x0f0000000.  256MB
     * Attributes: I/G/W/R
     */

    TLB_NEXT(r1)
    TLB_WORD_SET(0, r1, r0, 0xb0000000 | TLB_V | _MMU_TLB_SZ_256M)
    TLB_WORD_SET(1, r1, r0, 0x0 | 0xf0000000)
    TLB_WORD_SET(2, r1, r0, _INHIBIT_AND_G | TLB_UW | TLB_UR | TLB_SW | TLB_SR)

    /*
     * Tlb #7.  Int SRAM - 0xC0000000 -> 0x080000000.  256KB
     * Note the size is just large enough for SRAM, and a hole
     * follows where a machine machine check handler mapping can be
     * placed.
     * Attributes: X/W/R
     */

    TLB_NEXT(r1)
    TLB_WORD_SET(0, r1, r0, 0xc0000000 | TLB_V | _MMU_TLB_SZ_256K)
    TLB_WORD_SET(1, r1, r0, 0x0 | 0x80000000)
    TLB_WORD_SET(2, r1, r0, _WRITETHROUGH_OR_G | TLB_UX | TLB_UW | TLB_UR | TLB_SX | TLB_SW | TLB_SR)

    /*
     * Tlb #8.  PCI - 0xD0000000 -> 0x200000000.  256MB
     * Attributes: I/G/W/R
     */

    TLB_NEXT(r1)
    TLB_WORD_SET(0, r1, r0, 0xd0000000 | TLB_V | _MMU_TLB_SZ_256M)
    TLB_WORD_SET(1, r1, r0, 0x2 | 0x00000000)   /* RPN */
    TLB_WORD_SET(2, r1, r0, _INHIBIT_AND_G | TLB_UW | TLB_UR | TLB_SW | TLB_SR)

    /*
     * Tlb #9.  DDR SDRAM - Cached 0x10000000 -> 0x010000000.  256MB
     * Attributes: X/W/R
     */

    TLB_NEXT(r1)
    TLB_WORD_SET(0, r1, r0, 0x10000000 | TLB_V | _MMU_TLB_SZ_256M)
    TLB_WORD_SET(1, r1, r0, 0x0 | 0x10000000)
    TLB_WORD_SET(2, r1, r0, _WRITETHROUGH_AND_G | TLB_UX | TLB_UW | TLB_UR | TLB_SX | TLB_SW | TLB_SR)

    /*
     * Tlb #10.  DDR SDRAM - Cached 0x20000000 -> 0x020000000.  256MB
     * Attributes: X/W/R
     */

    TLB_NEXT(r1)
    TLB_WORD_SET(0, r1, r0, 0x20000000 | TLB_V | _MMU_TLB_SZ_256M)
    TLB_WORD_SET(1, r1, r0, 0x0 | 0x20000000)
    TLB_WORD_SET(2, r1, r0, _WRITETHROUGH_AND_G | TLB_UX | TLB_UW | TLB_UR | TLB_SX | TLB_SW | TLB_SR)

    /*
     * Tlb #11.  DDR SDRAM - Cached 0x30000000 -> 0x030000000.  256MB
     * Attributes: X/W/R
     */

    TLB_NEXT(r1)
    TLB_WORD_SET(0, r1, r0, 0x30000000 | TLB_V | _MMU_TLB_SZ_256M)
    TLB_WORD_SET(1, r1, r0, 0x0 | 0x30000000)
    TLB_WORD_SET(2, r1, r0, _WRITETHROUGH_AND_G | TLB_UX | TLB_UW | TLB_UR | TLB_SX | TLB_SW | TLB_SR)

    /*
     * Tlb #12.  DDR SDRAM - cached 0x40000000 -> 0x040000000.  256MB
     * Attributes: X/W/R
     */

    TLB_NEXT(r1)
    TLB_WORD_SET(0, r1, r0, 0x40000000 | TLB_V | _MMU_TLB_SZ_256M)
    TLB_WORD_SET(1, r1, r0, 0x0 | 0x40000000)
    TLB_WORD_SET(2, r1, r0, _WRITETHROUGH_AND_G | TLB_UX | TLB_UW | TLB_UR | TLB_SX | TLB_SW | TLB_SR)

    /*
     * Tlb #13.  DDR SDRAM - Cached 0x50000000 -> 0x050000000.  256MB
     * Attributes: X/W/R
     */

    TLB_NEXT(r1)
    TLB_WORD_SET(0, r1, r0, 0x50000000 | TLB_V | _MMU_TLB_SZ_256M)
    TLB_WORD_SET(1, r1, r0, 0x0 | 0x50000000)
    TLB_WORD_SET(2, r1, r0, _WRITETHROUGH_AND_G | TLB_UX | TLB_UW | TLB_UR | TLB_SX | TLB_SW | TLB_SR)

    /*
     * Tlb #14.  DDR SDRAM - Cached 0x60000000 -> 0x060000000.  256MB
     * Attributes: X/W/R
     */

    TLB_NEXT(r1)
    TLB_WORD_SET(0, r1, r0, 0x60000000 | TLB_V | _MMU_TLB_SZ_256M)
    TLB_WORD_SET(1, r1, r0, 0x0 | 0x60000000)
    TLB_WORD_SET(2, r1, r0, _WRITETHROUGH_AND_G | TLB_UX | TLB_UW | TLB_UR | TLB_SX | TLB_SW | TLB_SR)

    /*
     * Tlb #15.  DDR SDRAM - Cached 0x70000000 -> 0x070000000.  256MB
     * Attributes: X/W/R
     */

    TLB_NEXT(r1)
    TLB_WORD_SET(0, r1, r0, 0x70000000 | TLB_V | _MMU_TLB_SZ_256M)
    TLB_WORD_SET(1, r1, r0, 0x0 | 0x70000000)
    TLB_WORD_SET(2, r1, r0, _WRITETHROUGH_AND_G | TLB_UX | TLB_UW | TLB_UR | TLB_SX | TLB_SW | TLB_SR)

#ifdef INCLUDE_440X5_TLB_RECOVERY_MAX
    /*
     * Tlb #16.  DDR SDRAM 0 - Uncached
     * For use when INCLUDE_440X5_TLB_RECOVERY_MAX is configured
     *     0x80000000 -> 0x000000000.  256MB
     * Attributes: I/W/R
     */

    TLB_NEXT(r1)
    TLB_WORD_SET(0, r1, r0, 0x80000000 | TLB_V | _MMU_TLB_SZ_256M)
    TLB_WORD_SET(1, r1, r0, 0x0 | 0x00000000)
    TLB_WORD_SET(2, r1, r0, _INHIBIT_AND_G | TLB_UX | TLB_UW | TLB_UR | TLB_SX | TLB_SW | TLB_SR)

    /*
     * Tlb #17.  DDR SDRAM 0 - Cached
     * Vectors other than Machine Check (cached, execute-only)
     * For use when INCLUDE_440X5_TLB_RECOVERY_MAX is configured
     *     VEC_BASE_ADRS -> 0x000000000.  16KB
     * Attributes: X/R
     */

    TLB_NEXT(r1)
    TLB_WORD_SET(0, r1, r0, VEC_BASE_ADRS | TLB_V | _MMU_TLB_SZ_16K)
    TLB_WORD_SET(1, r1, r0, 0x0 | 0x00000000)
    TLB_WORD_SET(2, r1, r0, _WRITETHROUGH_AND_G | TLB_UX | TLB_UR | TLB_SX | TLB_SR)
#endif /* INCLUDE_440X5_TLB_RECOVERY_MAX */

    /* Invalidate the rest of the TLB entries */

    subfic r0, r1, (N_TLB_ENTRIES-1)    /* N_TLB_ENTRIES - 1 - Num tlb */
                                        /* entries already set up.     */
    mtspr CTR, r0                       /* Set the counter for the branch */
    addis r0, r0, 0x0000                /* EPN = 0x00000000, Not Valid. */
TLB_invalidate_loop:                    
    addi  r1, r1, 0x0001                /* Increment to the next TLB entry */
    tlbwe r0, r1, 0x0000                /* Write TLB entry word 0 */
    tlbwe r0, r1, 0x0001                /* Write TLB entry word 1 */
    tlbwe r0, r1, 0x0002                /* Write TLB entry word 2 */
    bdnz  TLB_invalidate_loop

    /* Context switch */
    
    isync 

#ifdef CLEAR_DEBUG_REGISTERS
    /* Configure debug facilities */

    mtspr   DBCR1, r0
    mtspr   DBCR2, r0
    mtspr   DBSR, r1
    mtspr   IAC1, r0
    mtspr   IAC2, r0
    mtspr   IAC3, r0
    mtspr   IAC4, r0
    mtspr   DAC1, r0
    mtspr   DAC2, r0
    mtspr   DVC1, r0
    mtspr   DVC2, r0
    mtspr   DBCR0, r0
    isync
#endif /* CLEAR_DEBUG_REGISTERS */

    /* Configure timer facilities */

    mtspr   DEC, r0
    mtspr   TBL, r0
    mtspr   TBU, r0
    mtspr   TSR, r1
    mtspr   TCR, r0

    /* Clear some misc registers */

    mtspr   ESR, r0         /* clear Exception Syndrome Reg */
    mtspr   XER, r0         /*  clear Fixed-Point Exception Reg */

    /* clear interrupt sources, stop dma */

    mtdcr   UIC0_ER, r0     /* disable ints at the UIC   */
    mtdcr   UIC1_ER, r0     /* likewise for the second UIC   */
    mtdcr   UIC2_ER, r0     /* and the third UIC         */
    mtdcr   UICB_ER, r0     /* and the base UIC     */
    mtdcr   DMA0_CR0, r0        /* stop DMA channels         */
    mtdcr   DMA0_CR1, r0
    mtdcr   DMA0_CR2, r0
    mtdcr   DMA0_CR3, r0

    /*
     * Initalize registers that need to be cleared with 0xFFFFFFFF
     */

    mtdcr   DMA0_SR, r1     /* clear DMA status reg      */
    mtdcr   UIC0_SR, r1     /* clear all pending UIC ints    */
    mtdcr   UIC1_SR, r1     /* likewise for the second UIC   */
    mtdcr   UIC2_SR, r1     /* and the third UIC         */
    mtdcr   UICB_SR, r1     /* and the base UIC     */

    /*
     * Invalidate the entire instruction cache. This can be done
     * with a single iccci instruction in the 440 processor core.
     */

    iccci   r0, r0

    /*
     * Invalidate the entire data cache. This can be done with a
     * single dccci instruction in the 440 processor core.
     */

    dccci   r0, r0

    /*
     * Clear the CPU reservation bit
     */

    li  r0, 0
    lwarx   r3, r0, r0
    stwcx.  r3, r0, r0

    /*
     * Initialize the stack pointer
     */

    lis sp, HIADJ(_sysInit)
    addi    sp, sp, LO(_sysInit)

#if FALSE               /* SDA not supported */
    /* initialize r2 and r13 according to EABI standard */

    lis r2, HIADJ(_SDA2_BASE_)
    addi    r2, r2, LO(_SDA2_BASE_)
    lis r13, HIADJ(_SDA_BASE_)
    addi    r13, r13, LO(_SDA_BASE_)
#endif /* FALSE */

    mfmsr   r3          /* read msr */
    INT_MASK(r3, r3)        /* mask ee bit and ce bit */
    mtmsr   r3          /* DISABLE INTERRUPT */

    addi    sp, sp, -FRAMEBASESZ    /* carve stack frame */
    mr  r3, r8                      
    bl  FUNC(usrInit)
FUNC_END(_sysInit)

#ifdef INCLUDE_PCI
/*****************************************************************************
*
* sysPciInByte - reads a byte from PCI Config Space.
*
* This function reads a byte from a specified PCI Config Space address.
*
* ARGUMENTS:
*   r3  = Config Space address
*
* RETURNS:
*   r3  = byte from address.
*/

FUNC_BEGIN(sysPciInByte)
    lbzx    r3,r0,r3    /* Read byte from PCI space */
    blr
FUNC_END(sysPciInByte)

/*****************************************************************************
*
* sysPciInWord - reads a word (16-bit big-endian) from PCI Config Space.
*
* This function reads a word from a specified PCI Config Space (little-endian)
* address.  It uses the load halfword byte-reversed instruction.
*
* ARGUMENTS:
*   r3  = Config Space address
*
* RETURNS:
*   r3  = word (16-bit big-endian) from address.
*/

FUNC_BEGIN(sysPciInWord)
FUNC_LABEL(sysInWord)
    lhbrx   r3,r0,r3    /* Read and swap from PCI space */
    blr
FUNC_END(sysPciInWord)

/*****************************************************************************
*
* sysPciInLong - reads a long (32-bit big-endian) from PCI Config Space.
*
* This function reads a long from a specified PCI Config Space (little-endian)
* address.  It uses the load word byte-reversed instruction.
*
* ARGUMENTS:
*   r3  = Config Space address
*
* RETURNS:
*   r3  = long (32-bit big-endian) from address.
*/

FUNC_BEGIN(sysPciInLong)
FUNC_LABEL(sysInLong)
    lwbrx   r3,r0,r3    /* Read and swap from PCI space */
    blr
FUNC_END(sysPciInLong)

/******************************************************************************
*
* sysPciOutByte - writes a byte to PCI Config Space.
*
* This function writes a byte to a specified PCI Config Space address.
*
* ARGUMENTS:
*   r3  = Config Space address
*   r4  = byte to write
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysPciOutByte)
    stbx    r4,r0,r3    /* Write a byte to PCI space */
    blr
FUNC_END(sysPciOutByte)

/******************************************************************************
*
* sysPciOutWord - writes a word (16-bit big-endian) to PCI Config Space.
*
* This function writes a word to a specified PCI Config Space (little-endian)
* address.  It uses the store halfword byte-reversed instruction.
*
* ARGUMENTS:
*   r3  = Config Space address
*   r4  = word (16-bit big-endian) to write
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysPciOutWord)
FUNC_LABEL(sysOutWord)
    sthbrx  r4,r0,r3    /* Write with swap to PCI space */
    blr
FUNC_END(sysPciOutWord)

/******************************************************************************
*
* sysPciOutLong - writes a long (32-bit big-endian) to PCI Config Space.
*
* This function writes a long to a specified PCI Config Space (little-endian)
* address.  It uses the store word byte-reversed instruction.
*
* ARGUMENTS:
*   r3  = Config Space address
*   r4  = long (32-bit big-endian) to write
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysPciOutLong)
FUNC_LABEL(sysOutLong)
    stwbrx  r4,r0,r3    /* Write big-endian long to little-endian */
    blr
FUNC_END(sysPciOutLong)

#endif /* INCLUDE_PCI */


/*****************************************************************************
*
* sysPlbInLong - reads a word from an io address.
*
* This function reads a 32 bit word from a specified io address.
*
* INPUTS:
*   r3  = address to read data from
* RETURNS: word from address.
*/

FUNC_BEGIN(sysPlbInLong)
    lwzx    r3,r0,r3    /* Read word from an address */
    sync                /* Sync I/O operation */
    blr
FUNC_END(sysPlbInLong)

/******************************************************************************
*
* sysPlbOutLong - writes a word to an io address.
*
* This function writes a 32 bit word to a specified io address.
*
*   r3  = address to write to
*   r4  = data to be written
* RETURNS: N/A
*/

FUNC_BEGIN(sysPlbOutLong)
    stwx    r4,r0,r3    /* Write a word to an address */
    sync                /* Sync I/O operation */
    blr
FUNC_END(sysPlbOutLong)

/*
 * Include other .s files for chip-specific DCR functions
 */

#include "440gxBusErr.s"
