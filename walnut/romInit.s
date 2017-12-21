/* romInit.s - IBM Walnut/Sycamore PPC405GP/PPC405GPr ROM initialization */

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

/* Copyright 1984-2004 Wind River Systems, Inc. */
        .data	     

/*
modification history
--------------------
01q,03sep04,mdo  Documentation fixes for apigen
01p,20aug02,pch  405GP / 405GPr interoperability
01o,18jun02,mcg  add support for 405GPr, fix IIC divisor setting for 405GP
01n,30apr02,pch  SPR 75652: add ".space 0x100" to set proper origin since we
                 no longer use HEX_FLAGS for the purpose
01m,04apr02,pch  SPR 74348: Machine Check cleanup
01l,09jan02,pch  Change EBC references to EBC0 for consistency with ebony
01k,10dec01,tpw  Diab-ize .reset section and rlwinm mask arguments
01j,04dec01,pch  Add .reset section
01j,30nov01,kab  Update for T2.2
01i,19nov01,pch  cleanup
01h,08may01,pch  Add assembler abstractions (FUNC_EXPORT, FUNC_BEGIN, etc.)
01g,05dec00,s_m  changed to use defines from ppc405GP.h for cache size
01f,23oct00,s_m  changes to use new kernel cache lib
01e,25aug00,mcg  disable caches upon entry
01d,15may00,mcg  register name updates to match 405GP User Manual
01c,27mar00,mcg  simplified register initialization, do not call sdramInit
                 for warm reboots (ECC init was clobbering boot line)
01b,22jan00,mcg  added SDRAM auto-configuration support
01a,08sep99,mcg  created from evb403 romInit.s version 01l.
*/

/*
DESCRIPTION
This module contains the entry code for the VxWorks bootrom.
The entry point romInit, is the first code executed on power-up.
It sets the BOOT_COLD parameter to be passed to the generic
romStart() routine.

The routine sysToMonitor() jumps to the location 4 bytes
past the beginning of romInit, to perform a "warm boot".
This entry point allows a parameter to be passed to romStart().

*/

#define _ASMLANGUAGE
#include "vxWorks.h"
#include "sysLib.h"
#include "asm.h"
#include "config.h"
#include "regs.h"

        /* internals */

        FUNC_EXPORT(romInit)         /* start of system code */
        FUNC_EXPORT(_romInit)        /* start of system code */

        /* externals */

        FUNC_IMPORT(romStart)        /* system initialization routine */

        _WRS_TEXT_SEG_START
	.space (0x100)

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
*
* RETURNS: N/A
*/

FUNC_BEGIN(romInit)
FUNC_LABEL(_romInit)
        bl      cold
        bl      warm

        /* copyright notice appears at beginning of ROM (in TEXT segment) */

        .ascii   "Copyright 1984-2002 Wind River Systems, Inc."
        .align 4

cold:
        li      r8, BOOT_COLD
        bl      start                   /* skip over next instruction */

warm:
        or      r8, r3, r3              /* startType to r8 */

start:
        /*
         * Disable external interrupts by turning off CE and EE bits in MSR
	 * Also turn off Machine Check until its vector is set up.
         */

        mfmsr   r3                      /* r3 = msr              */
        INT_MASK (r3, r4)               /* mask EE and CE bit    */
	rlwinm	r4, r4, 0, 20, 18	/* turn off _PPC_MSR_ME  */
        mtmsr   r4                      /* msr = r4              */
        isync

        /*
         * Initalize registers that need to be set to zero.
         */

        addi    r4,r0,0x0000
        mtspr   ICCR,r4                 /* turn off I-cache (warm)       */
        mtspr   DCCR,r4                 /* turn off D-cache (warm)       */
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
        mtdcr   DMA_SR, r4              /* clear DMA status reg          */
        mtdcr   UIC_SR, r4              /* clear all pending UIC ints    */

        /*
         * Set storage guarded attribute
         *     Unguarded: 0x00000000 - 0x7FFFFFFF  SDRAM
         *       Guarded: 0x80000000 - 0xF7FFFFFF  PCI Memory, I/O, config,
         *                                         405GP internal peripherals,
         *                                         board external peripherals
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
         *
         */

        li      r3, 0                     /* clear r3 */
        li      r4, _DCACHE_LINE_NUM_405GPR   /* load number of cache lines */
        mtctr   r4

rom403Dcci:
        dccci   r0, r3
        addi    r3, r3, _CACHE_ALIGN_SIZE       /* bump to next line */
        bdnz    rom403Dcci                      /* go to invalidate */

        /*
         * Turn the instruction cache on for faster boot-up.
         * Also, the icache is needed to help initialize Bank 0
         * of the EBC to speed up accesses to flash.
         * address space 0x00000000-0x07ffffff is cached
         * address space 0xf8000000-0xffffffff is cached
         */

        lis     r3, HIADJ(_PPC405_ICCR_DEFAULT_VAL)
        addi    r3, r3, LO(_PPC405_ICCR_DEFAULT_VAL)
        mtspr   ICCR, r3
        isync

	/*
	 * Initialize External Bus Controller (EBC) for off-chip peripherals.
	 * Several settings depend on the processor type, 405GP vs 405GPr.
	 */

	mfspr	r3, PVR
	srwi	r3, r3, 16
	cmpwi	r3, PVR_405GP_HI
	beq	isGP
	bl	sycExtBusCntlrInit
	b	ebcDone
isGP:
	bl	walExtBusCntlrInit
ebcDone:

        /*
         * Now that the EBC Bank 0 has been set up, turn the I-cache off if
         * i-cache was not specified in config.h.  It is also invalidated
         * again.
         */

#ifndef USER_I_CACHE_ENABLE
        li      r3, 0                     /* clear r3 */
        mtspr   ICCR, r3                  /* turn off i-cache */
        isync
        iccci   r0, r0                    /* invalidate the I-cache again */
#endif

        /*
         * Initialize IIC0 for use in automatic SDRAM configuration
         */

#ifdef LOCAL_MEM_AUTOSIZE
        bl      iic0Init
#endif

        /*
         * Configure the SDRAM controller only if this is a cold boot.
         * If the SDRAM controller is reinitialized on a warm boot, the
         * boot line will get wiped out because of the ECC SDRAM memory
         * initialization.
         */

        li      r3, BOOT_COLD
        and.    r3, r3, r8             /* r8 is saved at the entry of romInit */
        beq     skip

        bl      sdramInit

skip:

        /*
         * Clear the CPU reservation bit
         */

        li      r0, 0
        lwarx   r3, r0, r0
        stwcx.  r3, r0, r0

        /* Initialize the stack pointer (r1) */

        lis     sp, HIADJ(STACK_ADRS)
        addi    sp, sp, LO(STACK_ADRS)

#if     FALSE                           /* SDA not supported  */
        /* initialize r2 and r13 according to EABI standard */

        lis     r2, HIADJ(_SDA2_BASE_)
        addi    r2, r2, LO(_SDA2_BASE_)
        lis     r13, HIADJ(_SDA_BASE_)
        addi    r13, r13, LO(_SDA_BASE_)
#endif

        /* calculate C entry point: routine - entry point + ROM base */

        lis     r4, HIADJ(romStart)     /* r4 = romstart */
        addi    r4, r4, LO(romStart)

        lis     r5, HIADJ(romInit)      /* r5 = romInit */
        addi    r5, r5, LO(romInit)

        lis     r6, HIADJ(ROM_TEXT_ADRS) /* r6 = ROM_TEXT_ADRS */
        addi    r6, r6, LO(ROM_TEXT_ADRS)

        subf    r4, r5, r4              /* r4 = r4 - r5 */
        add     r4, r4, r6              /* r4 = r4 + r6 */
        mtlr    r4                      /* link register = C entry point */

        or      r3, r8, r8              /* r3 = startType */
        addi    sp, sp, -FRAMEBASESZ    /* get frame stack */

        blr                             /* branch to link register */
FUNC_END(romInit)

/*******************************************************************************
*
* walExtBusCntlrInit - Initializes the 405GP External Bus Controller peripheral
*		    bank registers for the Walnut board.
*
*		    IMPORTANT: This code must run from the I-cache since
*		    the peripheral bank timing register EBC0_B0AP cannot be
*		    reliably changed while code is being executed from bank 0.
*
*			Bank 0 - Flash/SRAM
*			Bank 1 - NVRAM/RTC
*			Bank 2 - KYBD/Mouse Controller
*			Bank 3 - IRDA
*			Bank 4 - Pinned out on Expansion connector
*			Bank 5 - Pinned out on Expansion connector
*			Bank 6 - Pinned out on Expansion connector
*			Bank 7 - FPGA regs

* NOMANUAL
*/

FUNC_BEGIN(walExtBusCntlrInit)

/* Build this fcn only if including 405GP support */
#ifdef PPC405GP
        mflr    r4                     /* save link register                  */
        bl      ..getAddrW
..getAddrW:
        mflr    r3                     /* get address of ..getAddrW           */
        mtlr    r4                     /* restore link register value         */
        addi    r4,0,10                /* set ctr to 10; used to prefetch     */
        mtctr   r4                     /* 10 cache lines to fit this function */
                                       /* in cache (8x10=80 instructions)     */
..ebcloopW:
        icbt    r0,r3                  /* prefetch cache line for addr in r3  */
        addi    r3,r3,32               /* move to next cache line             */
        bdnz    ..ebcloopW             /* continue for 10 cache lines         */

        /*
         * Delay to ensure all accesses to ROM are complete before changing
         * bank 0 timings. 200usec should be enough.
         *  200,000,000 (cycles/sec) X .000200 (sec) = 0x9C40 cycles
         */
        addis   r3,0,0x0
        ori     r3,r3,0xA000          /* wait 200usec have passed since reset */
        mtctr   r3
..spinlpW:
        bdnz    ..spinlpW             /* spin loop */

        /*
         * Memory Bank 0 (Flash/SRAM) initialization
         */
        addi    r4,0,EBC0_B0AP
        mtdcr   EBC0_CFGADDR,r4
        addis   r4,0,0x9B01
        ori     r4,r4,0x5480
        mtdcr   EBC0_CFGDATA,r4

        addi    r4,0,EBC0_B0CR
        mtdcr   EBC0_CFGADDR,r4
        addis   r4,0,0xFFF1           /* BAS=0xFFF,BS=0x0(1MB),BU=0x3(R/W),   */
        ori     r4,r4,0x8000          /* BW=0x0(8 bits)                       */
        mtdcr   EBC0_CFGDATA,r4

        /*
         * Memory Bank 1 (NVRAM/RTC) initialization
         */
        addi    r4,0,EBC0_B1AP
        mtdcr   EBC0_CFGADDR,r4
        addis   r4,0,0x0281
        ori     r4,r4,0x5480
        mtdcr   EBC0_CFGDATA,r4

        addi    r4,0,EBC0_B1CR
        mtdcr   EBC0_CFGADDR,r4
        addis   r4,0,0xF001           /* BAS=0xF00,BS=0x0(1MB),BU=0x3(R/W),   */
        ori     r4,r4,0x8000          /* BW=0x0(8 bits)                       */
        mtdcr   EBC0_CFGDATA,r4

        /*
         * Memory Bank 2 (KYBD/Mouse) initialization
         */
        addi    r4,0,EBC0_B2AP
        mtdcr   EBC0_CFGADDR,r4
        addis   r4,0,0x0481
        ori     r4,r4,0x5A80
        mtdcr   EBC0_CFGDATA,r4

        addi    r4,0,EBC0_B2CR
        mtdcr   EBC0_CFGADDR,r4
        addis   r4,0,0xF011           /* BAS=0xF01,BS=0x0(1MB),BU=0x3(R/W),   */
        ori     r4,r4,0x8000          /* BW=0x0(8 bits)                       */
        mtdcr   EBC0_CFGDATA,r4

        /*
         * Memory Bank 3 (IRDA) initialization
         */
        addi    r4,0,EBC0_B3AP
        mtdcr   EBC0_CFGADDR,r4
        addis   r4,0,0x0181
        ori     r4,r4,0x5280
        mtdcr   EBC0_CFGDATA,r4

        addi    r4,0,EBC0_B3CR
        mtdcr   EBC0_CFGADDR,r4
        addis   r4,0,0xF021           /* BAS=0xF02,BS=0x0(1MB),BU=0x3(R/W),   */
        ori     r4,r4,0x8000          /* BW=0x0(8 bits)                       */
        mtdcr   EBC0_CFGDATA,r4

        /*
         * Memory Bank 7 (FPGA regs) initialization
         */
        addi    r4,0,EBC0_B7AP
        mtdcr   EBC0_CFGADDR,r4
        addis   r4,0,0x0101
        ori     r4,r4,0x5280
        mtdcr   EBC0_CFGDATA,r4

        addi    r4,0,EBC0_B7CR
        mtdcr   EBC0_CFGADDR,r4
        addis   r4,0,0xF031           /* BAS=0xF03,BS=0x0(1MB),BU=0x3(R/W),   */
        ori     r4,r4,0x8000          /* BW=0x0(8 bits)                       */
        mtdcr   EBC0_CFGDATA,r4
#endif	/* PPC405GP */
        blr
FUNC_END(walExtBusCntlrInit)

/*******************************************************************************
*
* sycExtBusCntlrInit - Initializes the 405GP External Bus Controller peripheral
*		    bank registers for the Walnut board.
*
*		    IMPORTANT: This code must run from the I-cache since
*		    the peripheral bank timing register EBC0_B0AP cannot be
*		    reliably changed while code is being executed from bank 0.
*
*			Bank 0 - Flash/SRAM
*			Bank 1 - NVRAM/RTC
*			Bank 2 - KYBD/Mouse Controller
*			Bank 3 - IRDA
*			Bank 4 - Pinned out on Expansion connector
*			Bank 5 - Pinned out on Expansion connector
*			Bank 6 - Pinned out on Expansion connector
*			Bank 7 - FPGA regs

* NOMANUAL
*/

FUNC_BEGIN(sycExtBusCntlrInit)

/* Build this fcn only if including 405GPr support */
#ifdef PPC405GPR
        mflr    r4                     /* save link register                  */
        bl      ..getAddrS
..getAddrS:
        mflr    r3                     /* get address of ..getAddrS           */
        mtlr    r4                     /* restore link register value         */
        addi    r4,0,10                /* set ctr to 10; used to prefetch     */
        mtctr   r4                     /* 10 cache lines to fit this function */
                                       /* in cache (8x10=80 instructions)     */
..ebcloopS:
        icbt    r0,r3                  /* prefetch cache line for addr in r3  */
        addi    r3,r3,32               /* move to next cache line             */
        bdnz    ..ebcloopS             /* continue for 10 cache lines         */

        /*
         * Delay to ensure all accesses to ROM are complete before changing
         * bank 0 timings. 200usec should be enough.
         *  200,000,000 (cycles/sec) X .000200 (sec) = 0x9C40 cycles
         */
        addis   r3,0,0x0
        ori     r3,r3,0xA000          /* wait 200usec have passed since reset */
        mtctr   r3
..spinlpS:
        bdnz    ..spinlpS             /* spin loop */

        /*
         * Memory Bank 0 (Flash/SRAM) initialization
         */
        addi    r4,0,EBC0_B0AP
        mtdcr   EBC0_CFGADDR,r4
        addis   r4,0,0x0400
        ori     r4,r4,0x6000
        mtdcr   EBC0_CFGDATA,r4

        addi    r4,0,EBC0_B0CR
        mtdcr   EBC0_CFGADDR,r4
        addis   r4,0,0xFFF1           /* BAS=0xFFF,BS=0x0(1MB),BU=0x3(R/W),   */
        ori     r4,r4,0x8000          /* BW=0x0(8 bits)                       */
        mtdcr   EBC0_CFGDATA,r4

        /*
         * Memory Bank 1 (NVRAM/RTC) initialization
         */
        addi    r4,0,EBC0_B1AP
        mtdcr   EBC0_CFGADDR,r4
        addis   r4,0,0x0404
        ori     r4,r4,0x1200
        mtdcr   EBC0_CFGDATA,r4

        addi    r4,0,EBC0_B1CR
        mtdcr   EBC0_CFGADDR,r4
        addis   r4,0,0xF001           /* BAS=0xF00,BS=0x0(1MB),BU=0x3(R/W),   */
        ori     r4,r4,0x8000          /* BW=0x0(8 bits)                       */
        mtdcr   EBC0_CFGDATA,r4

        /*
         * Memory Bank 2 (KYBD/Mouse) initialization
         */
        addi    r4,0,EBC0_B2AP
        mtdcr   EBC0_CFGADDR,r4
        addis   r4,0,0x0481
        ori     r4,r4,0x5A00
        mtdcr   EBC0_CFGDATA,r4

        addi    r4,0,EBC0_B2CR
        mtdcr   EBC0_CFGADDR,r4
        addis   r4,0,0xF011           /* BAS=0xF01,BS=0x0(1MB),BU=0x3(R/W),   */
        ori     r4,r4,0x8000          /* BW=0x0(8 bits)                       */
        mtdcr   EBC0_CFGDATA,r4

        /*
         * Memory Bank 3 (IRDA) initialization
         */
        addi    r4,0,EBC0_B3AP
        mtdcr   EBC0_CFGADDR,r4
        addis   r4,0,0x0201
        ori     r4,r4,0x5000
        mtdcr   EBC0_CFGDATA,r4

        addi    r4,0,EBC0_B3CR
        mtdcr   EBC0_CFGADDR,r4
        addis   r4,0,0xF021           /* BAS=0xF02,BS=0x0(1MB),BU=0x3(R/W),   */
        ori     r4,r4,0x8000          /* BW=0x0(8 bits)                       */
        mtdcr   EBC0_CFGDATA,r4

        /*
         * Memory Bank 7 (FPGA regs) initialization
         */
        addi    r4,0,EBC0_B7AP
        mtdcr   EBC0_CFGADDR,r4
        addis   r4,0,0x0181
        ori     r4,r4,0x5000
        mtdcr   EBC0_CFGDATA,r4

        addi    r4,0,EBC0_B7CR
        mtdcr   EBC0_CFGADDR,r4
        addis   r4,0,0xF031           /* BAS=0xF03,BS=0x0(1MB),BU=0x3(R/W),   */
        ori     r4,r4,0x8000          /* BW=0x0(8 bits)                       */
        mtdcr   EBC0_CFGDATA,r4
#endif  /* PPC405GPR */
        blr
FUNC_END(sycExtBusCntlrInit)

/*******************************************************************************
*
* sdramInit -       Initializes the 405GP SDRAM controller
*                   Configures SDRAM memory banks.
*                   Auto Memory Configuration option reads the SDRAM EEPROM
*                   via the IIC bus and then configures the SDRAM memory
*                   banks appropriately.

* NOMANUAL
*/

FUNC_BEGIN(sdramInit)

        mflr    r31

#ifndef  LOCAL_MEM_AUTOSIZE

        /*
         * Fixed 32MB SDRAM configuration.
         */

        /*
         * Set SDRAM_B0CR for bank 0. (0-16MB) Address Mode 4
         */
        addi    r4,0,SDRAM_B0CR
        mtdcr   SDRAM_CFGADDR,r4
        addis   r4,0,0x0004
        ori     r4,r4,0x6001
        mtdcr   SDRAM_CFGDATA,r4

        /*
         * Set SDRAM_B2CR for bank 2. (16MB-32MB) Address Mode 4
         */
        addi    r4,0,SDRAM_B2CR
        mtdcr   SDRAM_CFGADDR,r4
        addis   r4,0,0x0104
        ori     r4,r4,0x6001
        mtdcr   SDRAM_CFGDATA,r4

        /*
         * Take defaults for SDRAM Timing reg (SDRAM_TR)
         */

        /*
         * Delay to ensure 200usec have elapsed since reset. Assume worst
         * case that the core is running 200Mhz:
         *   200,000,000 (cycles/sec) X .000200 (sec) = 0x9C40 cycles
         */
        addis   r3,0,0x0000
        ori     r3,r3,0xA000        /* ensure 200usec have passed since reset */
        mtctr   r3
..spinlp2:
        bdnz    ..spinlp2           /* spin loop */

        /*
         * Set memory controller options reg, SDRAM_CFG.
         * Set DC_EN to '1' and BRD_PRF to '01' for 16 byte PLB Burst
         * read/prefetch.
         */
        addi    r4,0,SDRAM_CFG
        mtdcr   SDRAM_CFGADDR,r4
        addis   r4,0,0x8080         /* set DC_EN=1 */
        ori     r4,r4,0000
        mtdcr   SDRAM_CFGDATA,r4

        /*
         * Delay to ensure 10msec have elapsed since reset. This is
         * required for the MPC952 to stabilize. Assume worst
         * case that the core is running 200Mhz:
         * 200,000,000 (cycles/sec) X .010 (sec) = 0x1E8480 cycles
         * This delay should occur before accessing SDRAM.
         */
        addis   r3,0,0x001E
        ori     r3,r3,0x8480        /* ensure 10msec have passed since reset  */
        mtctr   r3
..spinlp3:
        bdnz    ..spinlp3           /* spin loop */

#else	/* LOCAL_MEM_AUTOSIZE */

        /*
         * Memory Auto Configuration. Reads EEPROM on the SDRAM DIMM via IIC
         * and sets the SDRAM controller appropriately. Handles ECC as well.
         */


        /*
         * Read bytes of interest on SDRAM EEPROM via iic_read subroutine.
         * These values are required to configure the SDRAM controller
         * properly. Data is stored in regs 4, 5, 6, 7, 16, and 10.
         */

        addi    r4,0,0x00
        addi    r5,0,0x00
        addi    r3,0,3                       /* set byte to read (# ROWS)    */
        bl      iic0Read
        ori     r4,r3,0x00                   /* store value in R4            */
        addi    r3,0,4                       /* set byte to read (# COLS)    */
        bl      iic0Read
        rlwinm  r4,r4,8,0,31                 /* make room for byte           */
        or      r4,r3,r4                     /* store value in R4            */
        addi    r3,0,17                      /* set byte to read(# INT BANKS */
        bl      iic0Read
        rlwinm  r4,r4,8,0,31                 /* make room for byte           */
        or      r4,r3,r4                     /* store value in R4            */
        addi    r3,0,5                       /* set byte to read (# BANKS)   */
        bl      iic0Read
        ori     r5,r3,0x00                   /* store value in R5            */
        addi    r3,0,6                       /* set byte to read (DATA WIDTH)*/
        bl      iic0Read
        ori     r6,r3,0x00                   /* store value in R6            */
        addi    r3,0,31                      /* set byte to read (BANK SIZE) */
        bl      iic0Read
        ori     r7,r3,0x00                   /* store value in R7            */
        addi    r3,0,23                      /* set byte to read (CASL)      */
        bl      iic0Read
        ori     r16,r3,0x00                  /* store value in R16           */
        addi    r3,0,21                      /* set byte to read (MODULE     */
        bl      iic0Read                     /* ATTRIBUTES)                  */
        ori     r10,r3,0x00                  /* store value in R10           */

        /*
         * If SDRAM_ECC_ENABLE is not defined, force the width of the DIMM
         * to be 64 bits.  This will allow ECC SDRAM DIMMs to be used as
         * regular SDRAM DIMMs.
         */

#ifndef SDRAM_ECC_ENABLE
        addi    r6,0,0x40
#endif

        /*
         * Now we have all the data from the SDRAM EEPROM needed in regs 4, 5
         * 6 and 7. Reg 4 has # rows, # cols, and # internal banks in 3 LSBytes.
         * Reg 5 has # banks, Reg 6 has data width, and Reg 7 bank size. No more
         * iic reads are needed. Now the memory controller needs to be set
         * up properly based on the data from the SDRAM EEPROM.
         */

        /*
         * Check r4 values to determine mode. Mode is determined by the
         * # rows, # cols, and # of internal banks. All three of these
         * parameters are stored in r4. Mode should be set as follows:
         *               rows x cols (internal banks)
         *   mode 1 - 11x9(2), 11X10(2)
         *   mode 2 - 12x9(4), 12x10(4)
         *   mode 3 - 13x9(4), 13x10(4), 13x11(4)
         *   mode 4 - 12x8(2), 12x8(4)
         *   mode 5 - 11x8(2), 11x8(4)
         *   mode 6 - 13x8(2), 13x8(4)
         *   mode 7 - 13x9(2), 13x10(2)
         * The mode must be programmed in each of the Memory Configuration
         * regs (MBxCF) for each enabled bank of SDRAM. Since we only have
         * one DIMM on the board, the mode will be the same for
         * each enabled SDRAM bank.
         */

        addis   r14,0,0x0B
        ori     r14,r14,0x0902
        cmp     0,0,r4,r14                   /* 11x9(2)  */
        beq     ..mode1
        addis   r14,0,0x0B
        ori     r14,r14,0x0A02
        cmp     0,0,r4,r14                   /* 11x10(2) */
        beq     ..mode1
        addis   r14,0,0x0C
        ori     r14,r14,0x0904
        cmp     0,0,r4,r14                   /* 12x9(4)  */
        beq     ..mode2
        addis   r14,0,0x0C
        ori     r14,r14,0x0A04
        cmp     0,0,r4,r14                   /* 12x10(4) */
        beq     ..mode2
        addis   r14,0,0x0D
        ori     r14,r14,0x0904
        cmp     0,0,r4,r14                   /* 13x9(4)  */
        beq     ..mode3
        addis   r14,0,0x0D
        ori     r14,r14,0x0A04
        cmp     0,0,r4,r14                   /* 13x10(4) */
        beq     ..mode3
        addis   r14,0,0x0D
        ori     r14,r14,0x0B04
        cmp     0,0,r4,r14                   /* 13x11(4) */
        beq     ..mode3
        addis   r14,0,0x0C
        ori     r14,r14,0x0804
        cmp     0,0,r4,r14                   /* 12x8(4)  */
        beq     ..mode4
        addis   r14,0,0x0C
        ori     r14,r14,0x0802
        cmp     0,0,r4,r14                   /* 12x8(2)  */
        beq     ..mode4
        addis   r14,0,0x0B
        ori     r14,r14,0x0802
        cmp     0,0,r4,r14                   /* 11x8(2)  */
        beq     ..mode5
        addis   r14,0,0x0B
        ori     r14,r14,0x0804
        cmp     0,0,r4,r14                   /* 11x8(4)  */
        beq     ..mode5
        addis   r14,0,0x0D
        ori     r14,r14,0x0802
        cmp     0,0,r4,r14                   /* 13x8(2)  */
        beq     ..mode6
        addis   r14,0,0x0D
        ori     r14,r14,0x0804
        cmp     0,0,r4,r14                   /* 13x8(4)  */
        beq     ..mode6
        addis   r14,0,0x0D
        ori     r14,r14,0x0902
        cmp     0,0,r4,r14                   /* 13x9(2)  */
        beq     ..mode7
        addis   r14,0,0x0D
        ori     r14,r14,0x0A02
        cmp     0,0,r4,r14                   /* 13x10(2) */
        beq     ..mode7

        /*
         * If we get here, then mode bytes read are invalid.
         * r4 contains the byte read.
         */

..invalid_dimm_mode_spin:
        b       ..invalid_dimm_mode_spin     /* halt - invalid dimm mode   */

        /*
         * Store mode bit values in r4. We will use this value when
         * setting the SDx_AM field (bits 16:18 of the MBxCF regs)
         * for each of the SDRAM banks.
         */
..mode7:
        addi    r4,0,0x6
        b       ..mode_set
..mode6:
        addi    r4,0,0x5
        b       ..mode_set
..mode5:
        addi    r4,0,0x4
        b       ..mode_set
..mode4:
        addi    r4,0,0x3
        b       ..mode_set
..mode3:
        addi    r4,0,0x2
        b       ..mode_set
..mode2:
        addi    r4,0,0x1
        b       ..mode_set
..mode1:
        addi    r4,0,0x0

..mode_set:
        /*
         * Determine the size of each bank by using the Module Bank Density
         * value stored in r7 as read from the SDRAM DIMM EEPROM. Since
         * the value currently in r7 contains the size per 64-bit
         * bank on the DIMM, we divide this value by 2 (shift right 1 bit)
         * to determine the size per 32-bit bank.
         */
        srwi    r7,r7,1                /* calc size per 32 bit bank           */
        cmpi    0,0,r7,0x02            /* check r7 for bank size              */
        beq     ..meg8                 /* and use value to calc bank end addr */
        cmpi    0,0,r7,0x04
        beq     ..meg16
        cmpi    0,0,r7,0x08
        beq     ..meg32
        cmpi    0,0,r7,0x10
        beq     ..meg64
        cmpi    0,0,r7,0x20
        beq     ..meg128
        addi    r7,0,0x100            /* must be 256M                         */
        addi    r15,0,0x6             /* set size bits (SDx_SZ) for MBxCF reg */
        b       ..set_mb
..meg8:
        addi    r7,0,0x08
        addi    r15,0,0x1             /* set size bits (SDx_SZ) for MBxCF reg */
        b       ..set_mb
..meg16:
        addi    r7,0,0x10
        addi    r15,0,0x2             /* set size bits (SDx_SZ) for MBxCF reg */
        b       ..set_mb
..meg32:
        addi    r7,0,0x20
        addi    r15,0,0x3             /* set size bits (SDx_SZ) for MBxCF reg */
        b       ..set_mb
..meg64:
        addi    r7,0,0x40
        addi    r15,0,0x4             /* set size bits (SDx_SZ) for MBxCF reg */
        b       ..set_mb
..meg128:
        addi    r7,0,0x80
        addi    r15,0,0x5             /* set size bits (SDx_SZ) for MBxCF reg */

..set_mb:

        /*
         * Set the SDRAM memory configuration regs (MBxCF). To do this we must
         * first determine the number of 32-bit banks to be configured. Note
         * that the number of banks in r5 is the number of 64-bit banks as
         * read from the SDRAM DIMM EEPROM. Since the 405GP only supports
         * 32-bit memory, each 64-bit bank must be configured as 2 separate
         * 32-bit banks. We also use the address mode stored in r4, the
         * size per 32-bit bank stored in r7, and the bank size bits stored
         * in r15 to configure the SDRAM memory bank regsisters (MBxCF).
         * Note that Memory starts at address 0x00000000 and
         * continues contiguously.
         */
        rlwinm  r4,r4,13,0,31           /* set SDx_AM (addr mode field bits   */
                                        /* 16:18) to be used when setting     */
                                        /* MBxCF regs                         */
        rlwinm  r15,r15,17,0,31         /* set SDx_SZ (size field bits        */
                                        /* 12:14) to be used when setting     */
                                        /* MBxCF regs                         */
        or      r4,r4,r15               /* set SDx_AM & SDx_SZ fields - these */
                                        /* settings are the same for all      */
                                        /* SDRAM banks since only 1 DIMM      */
        ori     r4,r4,0x1               /* set SDx_BE bit (bank enable)       */
        rlwinm  r7,r7,20,0,31           /* set r7 (size per bank) to be used  */
                                        /* for calculating the SDx_BA field   */
                                        /* (base addr field bits 0:9)         */
                                        /* when setting the MBxCF regs        */


        /*
         * Set SDRAM_B0CR for bank 0. No need to add r7 for bank 0 since base
         * address for SDRAM_B0CR is 0x00000000.
         */

        addi    r12,0,SDRAM_B0CR
        mtdcr   SDRAM_CFGADDR,r12
        mtdcr   SDRAM_CFGDATA,r4

        /*
         * Set B2CR for bank 2 only if DIMM is non-ECC.  We can not configure
         * bank 2 if an ECC DIMM is plugged in, since bank 2 can only
         * be used as non-ECC and 405GP does NOT support mixing of ECC and
         * non-ECC memory banks.
         */

        cmpi    0,0,r6,0x48            /* Check bank width, if 0x48 then      */
        beq     ..ecc_dimm1            /* its a 72 bit ECC DIMM               */

        addi    r12,0,SDRAM_B2CR
        mtdcr   SDRAM_CFGADDR,r12
        add     r4,r4,r7               /* set base address for this bank      */
                                       /* by adding bank size to SDx_SZ field */
        mtdcr   SDRAM_CFGDATA,r4

..ecc_dimm1:
        /*
         * Check r5 which contains the # of 64-bit banks as read from the
         * SDRAM DIMM EEPROM, to determine if SDRAM banks 1 and 3 need
         * to be configured as well.
         */

        cmpi    0,0,r5,0x2                 /* check # of 64-bit banks in r5   */
                                           /* if 2 then banks B1CR and B3CR   */
                                           /* also need to be configured      */
        bne     ..mb_set

..banks1_3:
        /*
         * Set SDRAM_B1CR for bank 1.
         */

        addi    r12,0,SDRAM_B1CR
        mtdcr   SDRAM_CFGADDR,r12
        add     r4,r4,r7               /* set base address for this bank      */
                                       /* by adding bank size to SDx_SZ field */
        mtdcr   SDRAM_CFGDATA,r4

        /*
         * Set B3CR for bank 3 only if DIMM is non-ECC.  We can not configure
         * bank 3 if an ECC DIMM is plugged in, since bank 3 can only
         * be used as non-ECC and 405GP does NOT support mixing of ECC and
         * non-ECC memory banks.
         */
        cmpi    0,0,r6,0x48
        beq     ..ecc_dimm2

        addi    r12,0,SDRAM_B3CR
        mtdcr   SDRAM_CFGADDR,r12
        add     r4,r4,r7               /* set base address for this bank      */
                                       /* by adding bank size to SDx_SZ field */
       mtdcr   SDRAM_CFGDATA,r4
..ecc_dimm2:

..mb_set:

        /*
         * Walnut (405GP) uses defaults for SDRAM timing and refresh timer regs
         */

#ifdef PPC405GPR
	mfspr	r3, PVR
	srwi	r3, r3, 16
	cmpwi	r3, PVR_405GP_HI
	beq	skipSD0
        /*
	 * For Sycamore, set the SDRAM timing register, SDRAM0_TR and the
	 * refresh timer register SDRAM0_RTR.  Here we assume 133MHz
	 * operation and set the timings as such for the PC133 DIMM shipped
	 * with the board.  These timings are on the conservative side and
	 * should work with most non-ECC, non-registered PC133 and PC100
	 * DIMMs.  These settings should be reviewed if optimal timings are
	 * required, especially if operating SDRAM below 133MHz.  Refer to the
	 * DIMM datasheet and the chip user manual or additional information.
         */

        /*
         * Set SDRAM0_RTR value for 133MHz (7.5ns period).
         * Refresh rate approx = 15.24us/7.5ns = 2032 = 0x7F0.
         */
        addis   r3,r0,0x07F0
        addi    r12,0,SDRAM_RTR
        mtdcr   SDRAM_CFGADDR,r12
        mtdcr   SDRAM_CFGDATA,r3

        /*
         * Set SDRAM0_TR for 133MHz.  Look at CAS latency value in R16 from
         * the SEEPROM to determine its proper setting in the TR register.
         */
        cmpwi   r16,0x75                     /* compare to 7.5ns             */
        bgt     ..cas13
        addis   r3,r0,0x008B                 /* CASL = 0x1, PTA=0x2, CTP=0x3 */
        ori     r3,r3,0x4016                 /* LDF=0x1, RFTA=0x5, RCD=0x2   */
        b       ..set_tr
..cas13:
        addis   r3,r0,0x010B                 /* CASL = 0x2, PTA=0x2, CTP=0x3 */
        ori     r3,r3,0x4016                 /* LDF=0x1, RFTA=0x5, RCD=0x2   */
..set_tr:
        addi    r12,0,SDRAM_TR
        mtdcr   SDRAM_CFGADDR,r12
        mtdcr   SDRAM_CFGDATA,r3

skipSD0:
#endif   /* PPC405GPR */

        /*
         * Check bank width value in r6 to determine if ECC is possible
         * with the SDRAM being used. The board provides up to four
         * 32-bit banks of SDRAM, of which only bank 0 and
         * bank 1 can support ECC since a 64-bit DIMM is being used.
         * Banks 2 and 3 can still be used as non-ECC memory provided
         * ECC correction is disabled for those banks.
         *
         * NOTE: If ECC SDRAM is being used, ECC correction should be enabled
         * only after the portion of the ECC SDRAM memory to be used has been
         * initialized with ECC enabled (SDRAM_CFG MEMCHK bits). That portion of
         * memory must be initialized with ECC enabled but with correction
         * disabled to ensure the correct ECC checkbits get stored properly.
         * Since ECC does read-modify-writes, if correction was enabled
         * before initilaizing memory with ECC enabled, the checkbits returned
         * on the read would be garbage and would trigger an ECC error.
         */
        cmpi    0,0,r6,0x40            /* Check bank width, if 0x40 then      */
        beq     ..no_ecc               /* 64 bit and no ECC, else assume ECC  */
        addis   r9,0,0x9080            /* set value to program the SDRAM_CFG  */
        ori     r9,r9,0x0000           /* reg; DC_EN=1, MEMCHK='10' to        */
        b       ..cont1                /* enable ECC, and BRD_PRF to '01'     */
                                       /* for 16 byte PLB burst read/prefetch */
..no_ecc:
        addis   r9,0,0x8080            /* set value to program the SDRAM_CFG  */
        ori     r9,r9,0x0000           /* reg; DC_EN=1, MEMCHK='00' to        */
                                       /* disable ECC, and BRD_PRF to '01'    */
                                       /* for 16 byte PLB burst read/prefetch */
..cont1:
        cmpi    0,0,r10,0x16           /* Check DIMM MODEL ATTRIBUTES save in */
                                       /* r10 to see if Registered memory;    */
        bne     ..cont2                /* if registered (0x16) then           */
        addis   r10,0,0x0800           /* update SDRAM_CFG reg value. Set     */
        or      r9,r9,r10              /* R_DIMM_EN to enable registered mem  */

..cont2:
        /*
         * Delay to ensure 200usec have elapsed since reset. Assume worst
         * case that the core is running 200Mhz:
         *   200,000,000 (cycles/sec) X .000200 (sec) = 0x9C40 cycles
         */

        addis   r3,0,0x0000
        ori     r3,r3,0xA000        /* ensure 200usec have passed since reset */
        mtctr   r3
..spinlp2:
        bdnz    ..spinlp2           /* spin loop                              */

        /*
         * Set memory controller options reg, SDRAM_CFG with value in r9.
         * This enables the SDRAM controller.
         */

        addi    r12,0,SDRAM_CFG
        mtdcr   SDRAM_CFGADDR,r12
        mtdcr   SDRAM_CFGDATA,r9

        /*
         * Delay to ensure 10msec have elapsed since reset. This is
         * required for the MPC952 to stabalize. Assume worst
         * case that the core is running 200Mhz:
         *   200,000,000 (cycles/sec) X .010 (sec) = 0x1E8480 cycles
         * This delay should occur before accessing SDRAM.
         */

        addis   r3,0,0x001E
        ori     r3,r3,0x8480         /* ensure 10msec have passed since reset */
        mtctr   r3
..spinlp3:
        bdnz    ..spinlp3            /* spin loop */


        /*
         * Check value in r9 to see if ECC was enabled in SDRAM_CFG reg..
         * Bit 3 of the SDRAM_CFG MEMCHK field can be used to determine if
         * ECC enabled.
         */
        rlwinm. r9,r9,0,3,3          /* Check MEMCHK bit 3 of SDRAM_CFG */
        beq     ..ecc_disabled

..ecc_enabled:
        /*
         * If ECC enabled, initialize the entire SDRAM memory space
         * by writing it with zeros. This is necessary when
         * using ECC SDRAM since the memory must be initialized with ECC
         * enabled to ensure the checkbits get initialized correctly.
         * This must occur BEFORE enabling ECC correction. Otherwise, since
         * ECC performs read-modify writes, the checkbits returned on the
         * read would be garbage and an ECC error flagged. ECC correction
         * should only be enabled after initializing the ECC memory space
         * with ECC enabled. Initialization of the ECC memory space should
         * occur with the data cache enabled to ensure 64 bit writes and
         * proper checkbit generation.
         */
        add     r4,r4,r7              /* use last MBxCF reg setting still     */
                                      /* in r4 and the bank size still in r7  */
                                      /* to calculate the total size of       */
                                      /* SDRAM memory; here we add the bank   */
                                      /* size to the last bank base addr      */
                                      /* to determine the amount of SDRAM     */
        rlwinm   r4,r4,0,0,9          /* mask off all but the base addr field */
                                      /* and this value is the final SDRAM    */
                                      /* address                              */
        addis   r3,r0,0xFFFF          /* set r3 to start address (used        */
        ori     r3,r3,0xFFF8          /* to start at addr 0x00000000)         */
        addis   r6,r0,0xFFFF          /* set r6 to start address (used        */
        ori     r6,r6,0xFFFC          /* to start at addr 0x00000004)         */
        addis   r9,r0,0x0000          /* set r9 to test value                 */
                                      /* r4 contains the end address          */
        rlwinm  r4,r4,29,0,31         /* r4 contains the end address          */
                                      /* divide by 8 to get loop count        */
        mtctr   r4

        /*
         * Now we enable the data cache for the lower 128MB region in the
         * DCCR reg and initialize memory.  When complete we flush the data
         * cache and then disable it.
         */

        addis  r10,0,0x8000           /* put 0x80000000 in r10                */
        mtspr  DCCR,r10               /* enable D cache for the lowest 128MB  */

..loopz:
        stwu    r9,0x8(r3)            /* store test value */
        stwu    r9,0x8(r6)            /* store test value */
        bdnz    ..loopz

        /*
         * The last portion of data is stuck in the dcache now.
         * Start at the beginning and do a series of loads that will cause
         * dcache line fills that will flush the last bit of data out.
         * The data cache will be fully invalidated and disabled when complete.
         * Using the number of data cache lines for the 405GPr will also work
         * for the 405GP.
         */

        addi    r4,r0,_DCACHE_LINE_NUM_405GPR
        mtctr   r4

        addis   r3,r0,0xFFFF          /* set r3 to start address (used        */
        ori     r3,r3,0xFFE0          /* to start at addr 0x00000000)         */
        addi    r4,r0,0x0FE0          /* set r4 to start address (used        */
                                      /* to start at addr 0x00001000)         */
..flush:
        lwzu    r9,0x20(r3)           /* load in a line to force another out  */
        lwzu    r9,0x20(r4)           /* load the same line other side        */
        dcbi    r0,r3                 /* invalidate the line just loaded      */
        dcbi    r0,r4                 /* invalidate the line just loaded      */
        bdnz    ..flush

        addis   r10,0,0x0000
        mtspr   DCCR,r10              /* disable D cache                      */

        sync                          /* ensures stores complete before       */
                                      /* enabling ECC correction              */

        /*
         * With memory now initialized and ECC enabled, enable ECC
         * correction. Enable in bank 0 and check # of banks in r5
         * to determine if bank 1 should be enabled as well.
         * ECC correction is enabled on a per bank basis via the ECCCFG reg
         * bits 8:15.
         */

        addis   r9,0,0x0080                 /* set bank 0 correction bit      */
        cmpi    0,0,r5,0x2                  /* check # of 64-bit banks in r5  */
        bne     ..set_ecccf
        oris    r9,r9,0x0040                /* set bank 1 correction bit      */

..set_ecccf:
        /*
         * Set SDRAM_ECCCFG reg to enable correction.
         */

        addi    r12,0,SDRAM_ECCCFG
        mtdcr   SDRAM_CFGADDR,r12
        mtdcr   SDRAM_CFGDATA,r9
        sync

        /*
         * May call ..whups if we fail. ..whups expects failing addr in r3,
         * expected value in r4, and actual value in r7. Since we need the
         * value in r4 for the end addr, move it to r10 before setting r4
         * to the expected value.
         */
        mr      r10,r4                 /* set r10 to end address (used below) */
        addis   r3,r0,0xFFFF           /* set r3 to start address (used       */
        ori     r3,r3,0xFFFC           /* to start at addr 0x00000000)        */
        addis   r4,0,0x0               /* set r4 to value expected            */
        rlwinm  r10,r10,30,0,31        /* r10 contains the end address        */
                                       /* divide by 4 to get loop count       */
        mtctr   r10
..chk0loop:
        lwzu    r7,0x4(r3)             /* load test value                     */
        cmpl    cr0,r7,r4              /* compare read value to written value */
        bne     ..whups                /* branch to error if not the same     */
        bdnz    ..chk0loop

..ecc_disabled:

#endif	/* LOCAL_MEM_AUTOSIZE */

        mtlr    r31
        blr

        /*
         * R3 = Failing addrress
         * R4 = Expected value
         * R7 = Actual value
         */
..whups:
        b       ..whups
FUNC_END(sdramInit)


#ifdef  LOCAL_MEM_AUTOSIZE

/*******************************************************************************
*
* iic0Init -        Initializes the 405GP IIC0 for use during SDRAM
*                   Automatic Configuration.

* NOMANUAL
*/

FUNC_BEGIN(iic0Init)

        /*
         * Initialize IIC0 controller.
         */

        addis   r3,0,HI(IIC0_BASE)           /* base adrs of IIC0 cntlr       */
        ori     r3,r3,LO(IIC0_BASE)

        addi    r4,0,0x00
        stb     r4,IIC_LMADR(r3)             /* clear lo master address       */
        eieio
        stb     r4,IIC_HMADR(r3)             /* clear hi master address       */
        eieio
        stb     r4,IIC_LSADR(r3)             /* clear lo slave address        */
        eieio
        stb     r4,IIC_HSADR(r3)             /* clear hi slave address        */
        eieio
        addi    r4,0,0x08
        stb     r4,IIC_STS(r3)               /* clear status                  */
        eieio
        addi    r4,0,0x8F
        stb     r4,IIC_EXTSTS(r3)            /* clear extended sattus         */
        eieio

        /*
         * Set the IIC clock divisor relative to the OPB.
         *    if  30MHz < OPB speed <= 40MHz divisor should be 0x3
         *    if  40MHz < OPB speed <= 50MHz divisor should be 0x4
         *    if  50MHz < OPB speed <= 60MHz divisor should be 0x5
         *    if  60MHz < OPB speed <= 70MHz divisor should be 0x6
         */
        addi    r4,0,0x04                    /* Assume 50MHz OPB for 405GP    */
#ifdef PPC405GPR
	mfspr	r5, PVR
	srwi	r5, r5, 16
	cmpwi	r5, PVR_405GP_HI
	beq	use50
        addi    r4,0,0x06                    /* Assume 66MHz OPB for 405GPr   */
use50:
#endif
        stb     r4,IIC_CLKDIV(r3)
        eieio

        addi    r4,0,0x00
        stb     r4,IIC_INTRMSK(r3)           /* no interrupts                 */
        eieio
        stb     r4,IIC_XFRCNT(r3)            /* clear transfer count          */
        eieio
        addi    r4,0,0xF0
        stb     r4,IIC_XTCNTLSS(r3)          /* clear extended control & stat */
        eieio
        addi    r4,0,0x43                    /* set mode control - flush      */
        stb     r4,IIC_MDCNTL(r3)            /* master data buf, enable hold  */
        eieio                                /* SCL, exit unknown state       */
        addi    r4,0,0x00
        stb     r4,IIC_CNTL(r3)              /* clear control reg             */
        eieio

        blr
FUNC_END(iic0Init)


/*******************************************************************************
*
* iic0Read -        Performs a one byte read from the SDRAM EEPROM on the
*                   IIC0 bus.

* UCHAR iic0Read
*       (
*       UCHAR byteNum
*       )

* NOMANUAL
*/

        /*
         * iic0Read - does 1 byte read from SDRAM EEPROM on IIC0 bus.
         * iic combined format read requires a write of the byte we want to
         * read, before doing the read.
         */

FUNC_BEGIN(iic0Read)
        addis   r10,0,HI(IIC0_BASE)          /* r10 <-base addr of IIC0 cntlr */
        ori     r10,r10,LO(IIC0_BASE)

        addi    r11,0,0x08                   /* clear status                  */
        stb     r11,IIC_STS(r10)
        eieio

        addis   r12,r0,0x0001                /* set up counter for timeout    */
        mtctr   r12
..chk_sts1:
        lbz     r11,IIC_STS(r10)             /* read status                   */
        eieio
        andi.   r11,r11,0x01                 /* check for pending transfer    */
        beq     ..sts_ok1                    /* if 0, OK                      */
        bdnz    ..chk_sts1

        /*
         * If we get here, we timed out on clear status
         */

..iic0_timeout_fail_spin:
        b       ..iic0_timeout_fail_spin     /* halt - can not clear status   */

..sts_ok1:
        lbz     r11,IIC_MDCNTL(r10)          /* read mode control             */
        eieio
        ori     r11,r11,0x40                 /* flush master data buffer      */
        stb     r11,IIC_MDCNTL(r10)
        eieio
        addi    r11,0,0xA0                   /* set device address for        */
        stb     r11,IIC_LMADR(r10)           /* SDRAM EEPROM write (0xA0)     */
        eieio
        stb     r3,IIC_MDBUF(r10)            /* set device sub address        */
                                             /* which is the byte # to read   */
        addi    r11,0,0x01                   /* start write of 1 byte         */
        stb     r11,IIC_CNTL(r10)
        eieio

        addis   r12,r0,0x0001                /* set up counter for timeout    */
        mtctr   r12
..chk_sts2:
        lbz     r11,IIC_STS(r10)             /* read status                   */
        eieio
        andi.   r11,r11,0x01                 /* check for pending transfer    */
        beq     ..write_ok                   /* if 0, OK                      */
        bdnz    ..chk_sts2

        /*
         * If we get here, we timed out on writing the SDRAM EEPROM
         * Could be No DIMM. Halt since memory can not be configured
         * properly without the EEPROM data.
         */

..iic0Read_fail_spin:
        b       ..iic0Read_fail_spin         /* halt - read failed            */

..write_ok:
        addi    r11,0,0x08                   /* clear status                  */
        stb     r11,IIC_STS(r10)
        eieio

        addis   r12,r0,0x0001                /* set up counter for timeout    */
        mtctr   r12
..chk_sts3:
        lbz     r11,IIC_STS(r10)             /* read status                   */
        eieio
        andi.   r11,r11,0x01                 /* check for pending transfer    */
        beq     ..sts_ok2                    /* if 0, OK                      */
        bdnz    ..chk_sts3

        /*
         * If we get here, we timed out on clear status
         */

..iic0_timeout_fail_spin2:
        b       ..iic0_timeout_fail_spin2    /* halt - can not clear status   */

..sts_ok2:
        lbz     r11,IIC_MDCNTL(r10)          /* read mode control             */
        eieio
        ori     r11,r11,0x40                 /* flush master data buffer      */
        stb     r11,IIC_MDCNTL(r10)
        eieio
        addi    r11,0,0xA1                   /* set device address for        */
        stb     r11,IIC_LMADR(r10)           /* SDRAM EEPROM (0xA1)           */
        eieio
        addi    r11,0,0x03                   /* read of 1 byte                */
        stb     r11,IIC_CNTL(r10)
        eieio

        addis   r12,r0,0x0001                /* set up counter for timeout    */
        mtctr   r12
..chk_sts4:
        lbz     r11,IIC_STS(r10)             /* read status                   */
        eieio
        andi.   r11,r11,0x01                 /* check for pending transfer    */
        beq     ..read_ok                    /* if 0, OK                      */
        bdnz    ..chk_sts4

        /*
         * If we get here, we timed out on reading the SDRAM EEPROM
         * Could be No DIMM. Halt since memory can not be configured
         * properly without the EEPROM data.
         */

..iic0Read_fail_spin2:
        b       ..iic0Read_fail_spin2      /* halt - read failed            */

..read_ok:
        lbz     r3,IIC_MDBUF(r10)          /* read byte returned from EEPROM*/
        eieio

        blr                                /* return with data in R3        */
FUNC_END(iic0Read)


#endif /* LOCAL_MEM_AUTOSIZE */

/***************************************************************************
*
* resetVector - hardware reset vector
*
* At power-on, the PPC405 starts executing at ROM address 0xfffffffc -- the
* top of the address space -- which must be a jump to the reset entry point.
* This jump instruction is defined as a separate .section to assist the
* linker in placing it properly in the ROM.
*
*/
#if   defined(_GNU_TOOL)
	.section .reset, "ax", @progbits
#elif defined(_DIAB_TOOL)
	.section .reset, 4, "rx"
#else
#error "Please add a correctly spelled .section directive for your toolchain."
#endif
FUNC_BEGIN(resetVector)
	b	romInit
FUNC_END(resetVector)
