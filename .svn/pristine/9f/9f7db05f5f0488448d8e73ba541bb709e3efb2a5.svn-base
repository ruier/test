/* sysALib.s - IBM WALNUT 405GP system-dependent assembly routines */

/*
********************************************************************************
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
   LICENSED MATERIAL  -  PROGRAM PROPERTY OF  I B M

********************************************************************************
\NOMANUAL
*/

/*
 * Copyright (c) 1999-2004, 2008,2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01p,12feb09,syt  Save and restore startType at begin and end of sysInit().
                 (WIND00132564)
01o,02sep08,x_f  Remove sysDcr.s include
01n,03sep04,mdo  Documentation fixes for apigen
01m,20jun03,nwd  Deliver of Tornado 2.2 Cumulative Patch 1
01l,11jul02,pch  SPR 77845: fix name collision with PCI drivers
01k,18jun02,mcg  add support for 405GPr
01j,30nov01,kab  Update for T2.2
01i,05sep01,kab  grouped all sys[In|Out]* funcs here
01h,10may01,mcg  Support for Emac driver data cache & multi-channel changes
01g,08may01,pch  Add assembler abstractions (FUNC_EXPORT, FUNC_BEGIN, etc.)
01f,04dec00,s_m  used cache size defines from ppc405GP.h
01e,23oct00,s_m  changes for new kernel cache support
01d,25aug00,mcg  disable caches upon entry
01c,15may00,mcg  register name updates to match 405GP User Manual
01b,28mar00,mcg  change register init to reflect changes to romInit
                 added cache initialization
01a,31aug99,mcg  created from evb403 version 01r.
*/

/*
DESCRIPTION
This module contains system-dependent routines written in assembly
language.

This module must be the first specified in the \f3ld\f1 command used to
build the system.  The sysInit() routine is the system start-up code.
*/

#define _ASMLANGUAGE
#include "vxWorks.h"
#include "sysLib.h"
#include "config.h"
#include "asm.h"
#include "regs.h"

        /* globals */

        FUNC_EXPORT(_sysInit)                  /* start of system code */
        FUNC_EXPORT(sysPciInByte)
        FUNC_EXPORT(sysPciOutByte)
        FUNC_EXPORT(sysPciInWord)
        FUNC_EXPORT(sysPciOutWord)
        FUNC_EXPORT(sysPciInLong)
        FUNC_EXPORT(sysPciOutLong)
        FUNC_EXPORT(sysInByte)
        FUNC_EXPORT(sysOutByte)
        FUNC_EXPORT(sysInWord)
        FUNC_EXPORT(sysOutWord)
        FUNC_EXPORT(sysInLong)
        FUNC_EXPORT(sysOutLong)
        FUNC_EXPORT(sysPlbInByte)
        FUNC_EXPORT(sysPlbOutByte)
        FUNC_EXPORT(sysPlbInWord)
        FUNC_EXPORT(sysPlbOutWord)
        FUNC_EXPORT(sysPlbInLong)
        FUNC_EXPORT(sysPlbOutLong)
        FUNC_EXPORT(sysSpin)

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
* The initial stack is set to grow down from the address of sysInit().  This
* stack is used only by usrInit() and is never used again.  Memory for the
* stack must be accounted for when determining the system load address.
*
* NOTE: This routine should not be called by the user.
*
* RETURNS: N/A
* SYNOPSIS
* \ss
* sysInit (void)        /@ THIS IS NOT A CALLABLE ROUTINE @/
* \se
*/

FUNC_BEGIN(_sysInit)

        mr      r2, r3  /* Save away startType */ 
        isync           /* Make sure it is saved away */

        /*
         * Repeat initialization done in romInit.s except SDRAM and EBC
         * Initalize registers that need to be set to zero.
         */

        addi    r4,r0,0x0000
        mtmsr   r4                      /* disable all interrupts        */
        isync
        mtspr   ICCR,r4                 /* turn off I-cache              */
        mtspr   DCCR,r4                 /* turn off D-cache              */
        mtspr   DCWR,r4                 /* all memory regions write-back */
        mtspr   ESR, r4                 /* clear error status            */
        mtspr   TCR, r4                 /* disable all timers            */
        mtspr   PIT, r4                 /* clear the PIT timer           */
        mtdcr   UIC_ER, r4              /* disable ints at the UIC       */
        mtspr   XER, r4                 /* clear integer exception reg   */
        mtdcr   DMA_CR0, r4             /* stop DMA channels             */
        mtdcr   DMA_CR1, r4
        mtdcr   DMA_CR2, r4
        mtdcr   DMA_CR3, r4

        /*
         * Initalize registers that need to be cleared with 0xFFFFFFFF
         */

        addis   r4,r0,0xffff
        ori     r4,r4,0xffff
        mtspr   TSR, r4                 /* clear timer status            */
        mtspr   DBSR, r4                /* clear debug status reg        */
        mtdcr   UIC_SR, r4              /* clear all pending UIC ints    */
        mtdcr   DMA_SR, r4              /* clear DMA status reg          */

        /*
         * Set storage guarded attribute
         *     Unguarded: 0x00000000 - 0x7FFFFFFF  SDRAM
         *       Guarded: 0x80000000 - 0xF7FFFFFF  PCI Memory, I/O, config,
         *                                         405GP internal peripherals,
         *                                         Walnut external peripherals
         *     Unguarded: 0xF8000000 - 0xFFFFFFFF  Bootrom
         */

        addi    r4,r0,0x0000
        ori     r4,r4,0xfffe
        mtspr   SGR,r4

        /*
         * Invalidate the entire instruction cache. This can be done
         * with a single iccci instruction in the 405 processor core.
         */

        iccci   r0, r0

        /*
         * Invalidate the entire data cache.
         * The 405 processor core in the 405GP has 128 congruence classes.
         * The 405 processor core in the 405GPr has 256 congruence classes.
         * Each cache line in the 405 processor is 32 bytes.
         * Using 256 in the loop below will cover both 405GP and 405GPR.
         */

        li      r3, 0                     /* clear r3 */
        li      r4, _DCACHE_LINE_NUM_405GPR  /* load number of cache lines */
        mtctr   r4

rom403Dcci:
        dccci   r0, r3
        addi    r3, r3, _CACHE_ALIGN_SIZE       /* bump to next line */
        bdnz    rom403Dcci                      /* go to invalidate */

        /*
         * Clear the CPU reservation bit
         */

        li      r0, 0
        lwarx   r3, r0, r0
        stwcx.  r3, r0, r0

        /*
         * Initialize the stack pointer
         */

        lis     sp, HIADJ(_sysInit)
        addi    sp, sp, LO(_sysInit)

#if     FALSE                           /* SDA not supported */
        /* initialize r2 and r13 according to EABI standard */

        lis     r2, HIADJ(_SDA2_BASE_)
        addi    r2, r2, LO(_SDA2_BASE_)
        lis     r13, HIADJ(_SDA_BASE_)
        addi    r13, r13, LO(_SDA_BASE_)
#endif

        mfmsr   r3                      /* read msr */
        INT_MASK(r3, r3)                /* mask ee bit and ce bit */
        mtmsr   r3                      /* DISABLE INTERRUPT */

        addi    sp, sp, -FRAMEBASESZ    /* car frame stack */
        
        mr      r3, r2                  /* restore startType */
        isync                           /* make sure it is restored */        

        bl      FUNC(usrInit)
FUNC_END(_sysInit)

/*****************************************************************************
*
* sysPciInByte - reads a byte from PCI Config Space.
*
* This function reads a byte from a specified PCI Config Space address.
*
* SYNOPSIS
* \ss
* UCHAR sysPciInByte
*     (
*     ULONG       *addr  /@ address of data @/
*     )
* \se
*
* RETURNS: byte from address.
*/

FUNC_BEGIN(sysPciInByte)
FUNC_LABEL(sysInByte)

        lbzx    r3,r0,r3        /* Read byte from PCI space */
#ifndef PPC405GP_REVB           /* Required for 405GP rev B errata #6 */
        sync                    /* Sync I/O operation */
#endif
        bclr    20,0            /* Return to caller */
FUNC_END(sysPciInByte)

/*****************************************************************************
*
* sysPciInWord - reads a word (16-bit big-endian) from PCI Config Space.
*
* This function reads a word from a specified PCI Config Space (little-endian)
* address.  It uses the load halfword byte-reversed instruction.
*
* SYNOPSIS
* \ss
* UINT16 sysPciInWord
*     (
*     UCHAR  *addr	/@ address to read data from @/
*     )
* \se
*
* RETURNS: word (16-bit big-endian) from address.
*/

FUNC_BEGIN(sysPciInWord)
FUNC_LABEL(sysInWord)

        lhbrx   r3,r0,r3        /* Read and swap from PCI space */
#ifndef PPC405GP_REVB           /* Required for 405GP rev B errata #6 */
        sync                    /* Sync I/O operation */
#endif
        bclr    20,0            /* Return to caller */
FUNC_END(sysPciInWord)

/*****************************************************************************
*
* sysPciInLong - reads a long (32-bit big-endian) from PCI Config Space.
*
* This function reads a long from a specified PCI Config Space (little-endian)
* address.  It uses the load word byte-reversed instruction.
*
* SYNOPSIS
* \ss
* UINT32 sysPciInLong
*     (
*     UCHAR  *addr	/@ address to read data from @/
*     )
* \se
*
* RETURNS: long (32-bit big-endian) from address.
*/

FUNC_BEGIN(sysPciInLong)
FUNC_LABEL(sysInLong)

        lwbrx   r3,r0,r3        /* Read and swap from PCI space */
#ifndef PPC405GP_REVB           /* Required for 405GP rev B errata #6 */
        sync                    /* Sync I/O operation */
#endif
        bclr    20,0            /* Return to caller */
FUNC_END(sysPciInLong)

/******************************************************************************
*
* sysPciOutByte - writes a byte to PCI Config Space.
*
* This function writes a byte to a specified PCI Config Space address.
*
* SYNOPSIS
* \ss
* void sysPciOutByte
*     (
*     UCHAR  *addr,    /@ PCI config space address to write @/
*     UCHAR   data     /@ byte (8-bit data) to write @/
*     )
* \se
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysPciOutByte)
FUNC_LABEL(sysOutByte)

        stbx    r4,r0,r3        /* Write a byte to PCI space */
#ifndef PPC405GP_REVB           /* Required for 405GP rev B errata #6 */
        sync                    /* Sync I/O operation */
#endif
        bclr    20,0            /* Return to caller */
FUNC_END(sysPciOutByte)

/******************************************************************************
*
* sysPciOutWord - writes a word (16-bit big-endian) to PCI Config Space.
*
* This function writes a word to a specified PCI Config Space (little-endian)
* address.  It uses the store halfword byte-reversed instruction.
*
* SYNOPSIS
* \ss
* void sysPciOutWord
*     (
*     UCHAR  *addr,    /@ PCI config space address to write @/
*     UINT16  data     /@ word (16-bit big-endian) to write @/
*     )
* \se
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysPciOutWord)
FUNC_LABEL(sysOutWord)

        sthbrx  r4,r0,r3        /* Write with swap to PCI space */
#ifndef PPC405GP_REVB           /* Required for 405GP rev B errata #6 */
        sync                    /* Sync I/O operation */
#endif
        bclr    20,0            /* Return to caller */
FUNC_END(sysPciOutWord)

/******************************************************************************
*
* sysPciOutLong - writes a long (32-bit big-endian) to PCI Config Space.
*
* This function writes a long to a specified PCI Config Space (little-endian)
* address.  It uses the store word byte-reversed instruction.
*
* SYNOPSIS
* \ss
* void sysPciOutLong
*     (
*     UCHAR  *addr,    /@ PCI config space address to write @/
*     UINT32  data     /@ long (32-bit big-endian) to write @/
*     )
* \se
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysPciOutLong)
FUNC_LABEL(sysOutLong)

        stwbrx  r4,r0,r3        /* Write big-endian long to little-endian */
#ifndef PPC405GP_REVB           /* Required for 405GP rev B errata #6 */
        sync                    /* Sync I/O operation */
#endif
        bclr    20,0            /* Return to caller */
FUNC_END(sysPciOutLong)

/*****************************************************************************
*
* sysPlbInWord - reads a half word from an io address.
*
*  This function reads a 16 bit half word from a specified io address.
*
* SYNOPSIS
* \ss
* UINT16 sysPlbInWord
*     (
*     UCHAR  *addr	/@ address to read data from @/
*     )
* \se
*
* RETURNS: word (16-bit big-endian) from address.
*/

FUNC_BEGIN(sysPlbInWord)
    lhzx    r3,r0,r3        /* Read word from an address */
    sync                    /* Sync I/O operation */
    bclr    20,0            /* Return to caller */
FUNC_END(sysPlbInWord)

/*****************************************************************************
*
* sysPlbInByte - reads a byte from an io address.
*
*  This function reads a 8 bit byte from a specified io address.
*
* SYNOPSIS
* \ss
* UCHAR sysPlbInByte
*     (
*     UCHAR  *addr	/@ address to read data from @/
*     )
* \se
*
* RETURNS: byte from address.
*/

FUNC_BEGIN(sysPlbInByte)
    sync
    lbz     r3, 0(r3)
    sync
    bclr    20,0
FUNC_END(sysPlbInByte)

/******************************************************************************
*
* sysPlbOutByte - write a byte to an io address.
*
*  This function write a 8 bit byte to a specified io address.
*
* SYNOPSIS
* \ss
* void sysPlbOutByte
*             (
*             ULONG      *addr  /@ address to write data to @/
*             UCHAR       data  /@ 8-bit data @/
*             )
* \se
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysPlbOutByte)
    sync
    stb     r4,0(r3)
    sync
    bclr    20,0
FUNC_END(sysPlbOutByte)

/******************************************************************************
*
* sysPlbOutWord - write a half word to an io address.
*
*  This function write a 16 bit half word to a specified io address.
*
* SYNOPSIS
* \ss
* void sysPlbOutWord
*             (
*             ULONG      *addr  /@ address to write data to @/
*             UINT16      data  /@ 16-bit data @/
*             )
* \se
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysPlbOutWord)
    sthx    r4,r0,r3        /* Write a word to an address */
    sync                    /* Sync I/O operation */
    bclr    20,0            /* Return to caller */
FUNC_END(sysPlbOutWord)

/*****************************************************************************
*
* sysPlbInLong - reads a 32-bit word from an io address.
*
* This function reads a 32 bit word from a specified io address.
*
* SYNOPSIS
* \ss
* UINT32 sysPlbInLong
*     (
*     UCHAR  *addr	/@ address to read data from @/
*     )
* \se
*
* RETURNS: 32-bit data from address.
*/

FUNC_BEGIN(sysPlbInLong)
        lwzx    r3,r0,r3        /* Read 32-bit data from an address */
        sync                    /* Sync I/O operation */
        bclr    20,0            /* Return to caller */
FUNC_END(sysPlbInLong)

/******************************************************************************
*
* sysPlbOutLong - writes a word to an io address.
*
* This function writes a 32 bit word to a specified io address.
*
* SYNOPSIS
* \ss
* void sysPlbOutLong
*     (
*     UCHAR  *addr,    /@ PCI config space address to write @/
*     UINT32  data     /@ long (32-bit big-endian) to write @/
*     )
* \se
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysPlbOutLong)
        stwx    r4,r0,r3        /* Write a word to an address */
        sync                    /* Sync I/O operation */
        bclr    20,0            /* Return to caller */
FUNC_END(sysPlbOutLong)

/*****************************************************************************
*
* sysSpin - performs a branch hang, used for debug purposes.
*
* SYNOPSIS
* \ss
* void sysSpin(void)
* \se
*
* RETURNS: Never returns - spin loop
*/

FUNC_BEGIN(sysSpin)
        b       sysSpin
FUNC_END(sysSpin)


/*
 *  Include other .s files for chip specific DCR functions
 */

#include "405gpDcr.s"
#include "uicDcr.s"
#include "sdramDcr.s"
#include "ebcDcr.s"
#include "dmaDcr.s"
#if defined(EMAC_CACHE_FLUSH) || defined(EMAC_CACHE_INVALIDATE)
#include "405cache.s"
#endif

#ifdef PPC405GP_REVB
  #include "patchtblb.s"
#endif

#ifdef PPC405GP_REVC
  #include "patchtblc.s"
#endif

