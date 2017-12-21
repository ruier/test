/* romInit.s - amcc_taishan ROM initialization module */

/*******************************************************************************
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

*******************************************************************************/

/*
 * Copyright (c) 2002-2003, 2007-2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01j,15oct08,x_s  Removed ROM_RESIDENT definition. (WIND00136189)
01i,01mar07,x_s  Modify external bus controller init to support amcc_taishan board
01h,27aug03,jtp  Note L2 data parity not supported in bootrom.
01g,06aug03,jtp  Formal code inspection changes: macro additions & moves
01f,19jun03,pch  rename AltVecBaseAdrs to VecBaseAltAdrs
01e,18jul03,jtp  Support PCI
01d,05jun03,jtp  Change memory map to optionally support recoverable machine
		 checks
01c,02may03,pch  add comment re dependency in sysToMonitor; fix build errors
01b,28may03,jtp  Merge IBM changes: update DDR_MEMCLK_ERRATA, only do if
		 cold boot, add TRACEB_EBMI_ERRATA workaround.
01a,15dec02,saw  Written based on 440GP romInit.s version 01m.
*/

/*
DESCRIPTION
This module contains the entry code for the VxWorks bootrom.  After a
cold reset, The IBM PowerPC 400 processors all begin executing at
address 0xfffffffc, the resetVector entry point. resetVector is
typically a branch instruction to the romInit() entry point near the
beginning of the bootrom memory at ROM_TEXT_ADRS.  The romInit() entry
point sets the BOOT_COLD parameter to be passed to the generic
romStart() routine.

The IBM PowerPC 440 changes this model slightly. After a cold boot, only
the top 4KB of the address space is mapped by the chip, so instead of a
direct branch to romInit(), the boot vector instead branches to the
resetEntry() function located in the top 4KB of the device.
resetEntry() stabilizes the processor and extends access to the
romInit() memory area.

The romWarm() entry point is used instead for software reboots.  The
routine sysToMonitor() jumps to the location 8 bytes past the beginning
of romInit, to perform a "warm boot".  This entry point allows a
parameter to be passed to romStart().

To understand the bootrom startup, follow the cold reset code path.  The
bootrom discovers settings, initializes buses, bridges and devices just
enough to complete the boot process. It leaves high performance
operation to the full vxWorks kernel.

The bus architecture of PowerPC 440 devices is as follows:

    CPU Core - the CPU Core supports the Device Control Register Bus.
    There are several sub-device control buses accessible via keyhole
    access (program a sub-register address into a DCR register, read or
    write data via a different DCR register).

    Processor Local Bus (PLB) - The high-performance bus for master
    devices. CPU I- and D-Cache, SRAM/L2 Cache, SDRAM, PCI-X, MAL, and
    the PLB->OPB Bridge all reside on this bus.

    Onchip Peripheral Bus (OPB) - A bus for non-busmaster onchip
    devices. EMAC, UART, IIC, etc. plus the OPB->EB bridge.

    External Bus (EB) - Offchip device bus, including Ocotea Large and
    Small Flash, Onboard SRAM, Dallas RTC/NVRAM Chip, FPGA, External PCI
    Arbiter, etc.

The buses run at various programmable clock ratios with respect to one
another.  There is also a timebase clock, which may be divided from an
internal or provided by an external source, which runs most of the
peripheral timers.

This is relocatable code.  The initial bootrom code is typically loaded
at a different address than it was linked. Initially, no memory facility
is available. romInit is responsible for mapping in the onchip SRAM, and
making it available to the SDRAM initialization code. Only then is SDRAM
available.  All code in this module must be position-independent, and
not use memory before it has been enabled.  Relative data and code
addresses are valid, but absolute addresses must be translated to the
actual loaded location before use.

*/

#define _ASMLANGUAGE
#include "vxWorks.h"
#include "sysLib.h"
#include "asm.h"
#include "config.h"
#include "regs.h"
#include "arch/ppc/excPpcLib.h"
#include "arch/ppc/mmu440Lib.h"

	/* defines */

/*
 * Location of temporary stack in onchip SRAM0, for use after SRAM0
 * access is available but before SDRAM0 controller is configured.
 */

#define ROM_STACK_ADRS	(SRAM0_BASE_ADDR+SRAM0_SIZE-16)

/*
 * SDRAM_RESET_DELAY_COUNT - number of instruction clock cycles needed
 * to ensure 200usec have passed since reset. Assume worst case that the
 * core is running 533Mhz:
 *
 *   533,333,333 (cycles/sec) X .000200 (sec) = 106667 cycles
 *   = 1A0AB
 */

#define SDRAM_RESET_DELAY_COUNT	0x0001A0AB

/*
 * IVPR register initialization. This is used only if
 * DEBUG_BOOTROM_EXCEPTIONS is configured.
 */

#define IVPR_BOOT_DEFAULT 0xfff90000

/*
 * MSR register initialization
 *
 * This initial MSR value allows machine check and critical class
 * exceptions.
 */

#define MSR_VALUE	(_PPC_MSR_CE|_PPC_MSR_ME)

/*
 * Definitions for the bank control registers, which control access
 * to offchip devices.
 */

#define EBC0_CFG_VALUE (                                          \
        EBC_CFG_LE_UNLOCK                                       | \
        EBC_CFG_PTD_EN                                          | \
        EBC_CFG_RTC_64       |                                    \
        EBC_CFG_ATC_PREVIOUS | EBC_CFG_DTC_PREVIOUS             | \
        EBC_CFG_CTC_PREVIOUS | EBC_CFG_EMC_NONDEFAULT           | \
        EBC_CFG_PME_DISABLE  | EBC_CFG_PR_32)

 
/*-------------------------------------------------------------------------+
+ 64MB FLASH. Initialize bank 0 with default values.
+-------------------------------------------------------------------------*/

#define BANK0_AP_VALUE ( \
	    EBC_BXAP_BCE_DISABLE| (15 << TWT_BITSTOSHIFT)|\
        EBC_BXAP_BCE_DISABLE|\
        (1 << 18)|(1 << 16)|\
        (1 << 14)|(1 << 12)|\
        (3 << 9)|EBC_BXAP_RE_DIS|\
        EBC_BXAP_BEM_W|\
        EBC_BXAP_NO_PAR)
      
#define BANK0_CR_VALUE ( CFG_FLASH_BASE |\
      EBC_BXCR_BS_64MB|EBC_BXCR_BU_RW|EBC_BXCR_BW_32)

/*-------------------------------------------------------------------------+
* FPGA. Initialize bank 1 with default values.
+-------------------------------------------------------------------------*/
#define BANK1_AP_VALUE ( EBC_BXAP_BCE_DISABLE|(5 << TWT_BITSTOSHIFT)|\
      EBC_BXAP_BCE_DISABLE|\
      (1 << 18)|(1 << 16)|\
      (1 << 14)|(1 << 12)|\
      (3 << 9)|EBC_BXAP_RE_DIS|\
      EBC_BXAP_BEM_W|\
      EBC_BXAP_NO_PAR)
      
#define BANK1_CR_VALUE ( 0x41000000 |\
      EBC_BXCR_BS_1MB|EBC_BXCR_BU_RW|EBC_BXCR_BW_8)

/*-------------------------------------------------------------------------+
* LCM. Initialize bank 2 with default values.
+-------------------------------------------------------------------------*/
#define BANK2_AP_VALUE ( EBC_BXAP_BCE_DISABLE|(64 << TWT_BITSTOSHIFT)|\
      EBC_BXAP_BCE_DISABLE|\
      (3 << 18)|(3 << 16)|\
      (3 << 14)|(3 << 12)|\
      (7 << 9)|EBC_BXAP_RE_DIS|\
      EBC_BXAP_BEM_W|\
      EBC_BXAP_NO_PAR)
      
#define BANK2_CR_VALUE ( 0x42000000|\
      EBC_BXCR_BS_1MB|EBC_BXCR_BU_RW|EBC_BXCR_BW_8)

/*-------------------------------------------------------------------------+
* TMP. Initialize bank 3 with default values.
+-------------------------------------------------------------------------*/
#define BANK3_AP_VALUE ( EBC_BXAP_BCE_DISABLE|(128 << TWT_BITSTOSHIFT)|\
      EBC_BXAP_BCE_DISABLE|\
      (3 << 18)|(3 << 16)|\
      (3 << 14)|(3 << 12)|\
      (7 << 9)|EBC_BXAP_RE_DIS|\
      EBC_BXAP_BEM_W|\
      EBC_BXAP_NO_PAR)
      
#define BANK3_CR_VALUE ( 0x48000000|\
      EBC_BXCR_BS_64MB|EBC_BXCR_BU_RW|EBC_BXCR_BW_32)

 

/* General System DCR configuration parameters */

#define SDR0_AMP_MASK (~( \
	SDR0_AMP_AICURP | SDR0_AMP_ADCURP | SDR0_AMP_ADCUWP | \
        SDR0_AMP_APCIXP | SDR0_AMP_AIMUP  | SDR0_AMP_AMALP  | \
        SDR0_AMP_ADMAP  | SDR0_AMP_AOPBP  | SDR0_AMP_ICURP  | \
        SDR0_AMP_DCURP  | SDR0_AMP_DCUWP  | SDR0_AMP_PCIXP  | \
        SDR0_AMP_IMUP   | SDR0_AMP_MALP   | SDR0_AMP_DMAP   | \
        SDR0_AMP_OPBP))

#define SDR0_AMP_VALUE  ( \
	SDR0_AMP_AICURP_0 | SDR0_AMP_ADCURP_0  | SDR0_AMP_ADCUWP_0 | \
        SDR0_AMP_APCIXP_0 | SDR0_AMP_AIMUP_0   | SDR0_AMP_AMALP_0  | \
        SDR0_AMP_ADMAP_0  | SDR0_AMP_AOPBP_0   | SDR0_AMP_ICURP_A  | \
        SDR0_AMP_DCURP_A  | SDR0_AMP_DCUWP_A   | SDR0_AMP_PCIXP_A  | \
        SDR0_AMP_IMUP_A   | SDR0_AMP_MALP_A    | SDR0_AMP_DMAP_A   | \
        SDR0_AMP_OPBP_A )

	/*
	 * Processor core instruction cache unit.
	 *   IRF. IcuRdFetch priority setting
	 *   IRT. IcuRdTouch priority setting
	 *   IRS. IcuRdSpec priority setting
	 */

	/*
	 * Processor core data cache read unit.
	 *   DRU. DcuRdUrgent priority setting
	 *   DRT. DcuRdTouch priority setting
	 *  DRNC. DcuRdNonCache priority
	 *  DRLC. DcuRdLdCache priority setting
	 */

	/*
	 * Processor core data cache write unit
	 *   DWF. DcuWrFlush priority setting
	 *   DWS. DcuWrStore priority setting
	 *   DWU. DcuWrUrgent priority setting
	 */

#define SDR0_CP440_MASK (~( \
	SDR0_CP440_DRU | SDR0_CP440_DRT | SDR0_CP440_DRNC | SDR0_CP440_DRLC | \
	SDR0_CP440_DWF | SDR0_CP440_DWS | SDR0_CP440_DWU  | SDR0_CP440_IRF  | \
	SDR0_CP440_IRT | SDR0_CP440_IRS))

#define SDR0_CP440_VALUE ( \
	SDR0_CP440_DRU_0  | SDR0_CP440_DRT   | SDR0_CP440_DRNC_0 | \
        SDR0_CP440_DRLC_0 | SDR0_CP440_DWF_0 | SDR0_CP440_DWS_0  | \
        SDR0_CP440_DWU_0  | SDR0_CP440_IRF_0 | SDR0_CP440_IRT_0  | \
        SDR0_CP440_IRS_0)

/* Level 2 Cache Controller setup */

#define L2C0_CFG_MASK (~( \
	L2C_CFG_L2M_MASK   | L2C_CFG_SS_MASK  |       \
	L2C_CFG_ICU_MASK   | L2C_CFG_DCU_MASK)) 

#define L2C0_CFG_VALUE ( \
	L2C_CFG_L2M_DISABLE | L2C_CFG_SS_256KB |       \
        L2C_CFG_ICU_DISABLE | L2C_CFG_DCU_DISABLE)

#define L2C0_SNP_MASK (~L2C_SNPX_ESR_MASK)

#define L2C0_SNP_VALUE (L2C_SNPX_ESR_DISABLE)

/* Onchip SRAM configuration */

/* Note: SRAM0 Data Parity not supported in bootrom */
#define SRAM0_DPC_MASK (~SRAM_DPC_DPC_MASK)

#define SRAM0_DPC_VALUE (SRAM_DPC_DPC_DISABLE)

#define SRAM0_PMEG_MASK (~SRAM_PMEG_MASK)

#define SRAM0_SBXCR_MASK (~SRAM_SBXCR_MASK)

#define SRAM0_SB0CR_VALUE ( \
	(SRAM0_BASE_PLB & SRAM_SBXCR_BAS_MASK)                 | \
	SRAM_SBXCR_BS_64KB | SRAM_SBXCR_BU_RW                  | \
        (SRAM0_BASE_PLB_UA & SRAM_SBXCR_UA_MASK))

#define SRAM0_SB1CR_VALUE ( \
        ((SRAM0_BASE_PLB + 0x00010000) &  SRAM_SBXCR_BAS_MASK) | \
        SRAM_SBXCR_BS_64KB | SRAM_SBXCR_BU_RW                  | \
        (SRAM0_BASE_PLB_UA & SRAM_SBXCR_UA_MASK))

#define SRAM0_SB2CR_VALUE ( \
	((SRAM0_BASE_PLB + 0x00020000) &  SRAM_SBXCR_BAS_MASK) | \
        SRAM_SBXCR_BS_64KB | SRAM_SBXCR_BU_RW                  | \
        (SRAM0_BASE_PLB_UA & SRAM_SBXCR_UA_MASK))

#define SRAM0_SB3CR_VALUE ( \
	((SRAM0_BASE_PLB + 0x00030000) &  SRAM_SBXCR_BAS_MASK) | \
        SRAM_SBXCR_BS_64KB | SRAM_SBXCR_BU_RW                  | \
        (SRAM0_BASE_PLB_UA & SRAM_SBXCR_UA_MASK))

/*
 * CCR0 register initialization (for resetEntry)
 *
 * a. Disable store gathering
 * b. Disable broadcast of instrs to aux processor (save power)
 * c. Enable broadcast of trace information
 * d. Icbt is guaranteed to fill a cache line even during a stall
 * e. Dcbt is guaranteed to fill a cache line even during a stall
 * f. Disable exceptions for misaligned data accesses
 * g. Instruction cache speculative line count = 0
 * h. Instruction cache speculative line threshold = 0
 */

#define CCR0_INIT (((_PPC_CCR0_DSTG_U | _PPC_CCR0_DAPUIB_U) << 16) | \
		  _PPC_CCR0_GICBT | _PPC_CCR0_GDCBT)

/*
 * CCR1 register initialization
 *
 * a. ICDPEI: Record even parity. Normal operation.
 * b. ICTPEI: Record even parity. Normal operation.
 * c. DCTPEI: Record even parity. Normal operation.
 * d. DCDPEI: Record even parity. Normal operation.
 * e. DCUPEI: Record even parity. Normal operation.
 * f. DCMPEI: Record even parity. Normal operation.
 * g. FCOM:   Normal operation
 * h. MMUPEI: Record even parity. Normal operation.
 * i. FFF:    Flush only as much data as necessary.
 * j. TCS:    Timebase increments from externally supplied clock
 */

#define CCR1_INIT	(_PPC_CCR1_TCS)

/*
 * DBSR register initialization
 *
 * This is a write-1s-to-clear register. The value below clears all
 * debug event status bits but leaves the Most Recent Reset status bits
 * untouched.
 */

#define DBSR_INIT (_DBSR_IDE   | _DBSR_UDE   | \
		   _DBSR_ICMP  | _DBSR_BRT   | _DBSR_IRPT  | _DBSR_TRAP  | \
		   _DBSR_IAC1  | _DBSR_IAC2  | _DBSR_IAC3  | _DBSR_IAC4  | \
		   _DBSR_DAC1R | _DBSR_DAC1W | _DBSR_DAC2R | _DBSR_DAC2W | \
		   _DBSR_RET   | \
		   _DBSR_IAC12ATS | _DBSR_IAC34ATS)

/*
 * IVLIM, DVLIM register initialization
 *
 * Sets the Transient & Normal ceiling and floor such that all cache
 * lines are for normal storage.  MMU library supports only normal
 * cache lines.  Note that DVLIM is also currently initialized to the
 * same value as IVLIM.
 */

#define IVLIM_INIT	0x0001f800

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

#define MMUCR_INIT	0x00000000

/* TLB setup defines and functions */

#define TLB_V	0x00000200	/* TLB Word 0, Valid Bit */
#define TLB_W	0x00000800	/* TLB Word 2, Writethrough Cache Bit */
#define TLB_I	0x00000400	/* TLB Word 2, Inhibit Caching Bit  */
#define TLB_M	0x00000200	/* TLB Word 2, Memory Coherency Cache Bit */
#define TLB_G	0x00000100	/* TLB Word 2, Guarded Cache Bit */
#define TLB_E	0x00000080	/* TLB Word 2, Endianness Bit */
#define TLB_UX	0x00000020	/* TLB Word 2, User Executable Bit */
#define TLB_UW	0x00000010	/* TLB Word 2, User Writeable Bit */
#define TLB_UR	0x00000008	/* TLB Word 2, User Readable Bit */
#define TLB_SX	0x00000004	/* TLB Word 2, Supervisor Executable Bit */
#define TLB_SW	0x00000002	/* TLB Word 2, Supervisor Writeable Bit */
#define TLB_SR	0x00000001	/* TLB Word 2, Supervisor Readable Bit */

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

#define _INHIBIT		TLB_I
#define _INHIBIT_AND_G		TLB_I | TLB_G
#ifdef NOCACHE
# define _WRITETHROUGH		TLB_I
# define _WRITETHROUGH_OR_G	TLB_I | TLB_G
# define _WRITETHROUGH_AND_G	TLB_I | TLB_G
#else /* NOCACHE */
# define _WRITETHROUGH		TLB_W | TLB_G
# define _WRITETHROUGH_OR_G	TLB_W
# define _WRITETHROUGH_AND_G	TLB_W | TLB_G
#endif /* NOCACHE */

/*
 * When the boot space is configured as writeable (example: when
 * jumpering it to SRAM for JTAG-based debugging), add the writeable
 * bit.
 */

#ifdef BOOT_SPACE_WRITEABLE
# define BOOT_SPACE_WRITE 	TLB_UW | TLB_SW
#else /* BOOT_SPACE_WRITEABLE */
# define BOOT_SPACE_WRITE 	0
#endif /* BOOT_SPACE_WRITEABLE */

/*
 * The TLB_NEXT and TLB_WORD_SET macros are used to set up each TLB
 * entry, which consists of three words. Using macros reduces the
 * opportunity for error.
 */

#define TLB_NEXT(ixReg) \
        addi  ixReg, ixReg, 0x0001 /* Increment to the next TLB entry */

#define TLB_WORD_SET(wordNum, ixReg, tempReg, value) 		  \
	lis   tempReg, HI(value)				; \
	ori   tempReg, tempReg, LO(value)			; \
        tlbwe tempReg, ixReg, wordNum /* Write TLB Word wordNum */


	/* internals */

	FUNC_EXPORT(romInit)	/* entry point for cold reboot */
	FUNC_EXPORT(_romInit)	/* entry point for cold reboot */
	FUNC_EXPORT(romWarm)	/* entry point for warm reboot */
	FUNC_EXPORT(resetEntry)	/* hard-reset entry to set up TLB */
	FUNC_EXPORT(sdramDcrGet)/* get an SDRAM DCR */
	FUNC_EXPORT(sdramDcrSet)/* set an SDRAM DCR */
	FUNC_EXPORT(sdramResetDelay)/* allow SDRAM to warm up */
	FUNC_EXPORT(sdramTestPatterns)/* SDRAM test patterns */
	FUNC_EXPORT(sdramDcbf)	/* Data cache block flush */
	FUNC_EXPORT(sdramDflush)/* Data cache flush */
	FUNC_EXPORT(timeBaseGet) /* get TBL register value */
	FUNC_EXPORT(sdrDcrGet)
	FUNC_EXPORT(sdrDcrSet)
	

	/* externals */

	FUNC_IMPORT(romStart)	 /* system initialization routine */

	FUNC_IMPORT(romSdramInit) /* DDR SDRAM controller initialization */

	_WRS_TEXT_SEG_START

/*******************************************************************************
*
* romInit - entry point for VxWorks in ROM
*

* romInit
*     (
*     int startType	/@ only used by sysToMonitor @/
*     )

*/

FUNC_BEGIN(romInit)		/* used by romStart() */
FUNC_LABEL(_romInit)		/* used by linker */
	/*
	 * branch to resetEntry, which will initialize the tlb and
	 * return at cold
	 */

	b	resetEntry

FUNC_LABEL(romWarm)
	b	warm

	/*
	 * Copyright notice appears at beginning of ROM (in TEXT segment).
	 * Note that the warm-boot code in sysLib.c:sysToMonitor() depends
	 * on this being at offset 8 from the base of the flash device.
	 */

	.ascii	 "Copyright 1984-2003 Wind River Systems, Inc."
	.align 4

cold:
	/* resetEntry transfers execution here once TLB is initialized */

	li	r2, BOOT_COLD
	b	start			/* skip over next instruction */

warm:
	mr	r2, r3			/* startType to r2 */

start:
	/*
	 * Note: r2 is used to store the reset type code. The data it
	 * stores should not be changed until the jump to romStart
	 * at the end of romInit. r2 makes a good candidate for storing
	 * this information since by PowerPC convention it is reserved
	 * for use by the linker as the Small Data Area pointer. Since
	 * VxWorks does not make use of this feature, and since the
	 * compiler does not generate code to make use of this feature,
	 * it can be safely co-opted for this temporary use.
	 */

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

        xor     r0, r0, r0
        addi    r1, r0, -1

#ifdef CLEAR_DEBUG_REGISTERS
        /*
         * Configure debug facilities
         * a. Write DBCR1 and DBCR2 to specify IAC, DAC and DVC event
         *    conditions.
         * b. Clear DBSR to initialize IAC auto-toggle status
         * c. Initialize IAC1 - IAC4, DAC1 - DAC2, DVC1 - DVC2 registers
         * d. Write MSR[DWE] to enable Debug Wait mode (if desired)
         * e. Write DBCR0 to enable desired debug mode(s) and event(s)
         * f. Context synchronize to establish new debug facility context
         *    (isync)
         * Note: Debug facilities are already disabled at this point.
         *       Uncomment the code if they need to be enabled.
         */

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

        /*
         * Configure timer facilities
         * a. Write DEC to 0 to prevent DEC exceptions
         * b. Write TBL and TBU to 0 to prevent FIT and WDT exceptions
         * c. Clear TSR to clear all timer exception status
         * d. Write TCR to configure and enable timers as desired
         * e. Initialize TBU value as desired
         * f. Initialize TBL value as desired
         * g. Initialize DECAR if enabling the auto-reload function
         * h. Initialize DEC to desired value
         */

        mtspr   DEC, r0
        mtspr   TBL, r0
        mtspr   TBU, r0
        mtspr   TSR, r1
        mtspr   TCR, r0

        /*
         * Clear some misc registers.
         */

        mtspr   ESR, r0                 /* clear Exception Syndrome Reg */
        mtspr   XER, r0                 /*  clear Fixed-Point Exception Reg */

        /*
         * Initialize facilities outside the processor core which are
         * possible  sources of asynchronous interrupt requests
         * (including DCRs and/or  other memory-mapped resources). This
         * must be done prior to enabling  asynchronous interrupts in
         * the MSR.
         */

        /* assume caches off, all cached memory regions set to writeback */

        mtdcr   UIC0_ER, r0             /* disable ints at the UIC       */
        mtdcr   UIC1_ER, r0             /* likewise for the other UICs   */
        mtdcr   UIC2_ER, r0
        mtdcr   UICB_ER, r0
        mtdcr   DMA0_CR0, r0            /* stop DMA channels             */
        mtdcr   DMA0_CR1, r0
        mtdcr   DMA0_CR2, r0
        mtdcr   DMA0_CR3, r0

        /*
         * Initalize registers that need to be cleared with 0xFFFFFFFF
         */

        mtdcr   DMA0_SR, r1             /* clear DMA status reg          */
        mtdcr   UIC0_SR, r1             /* clear all pending UIC ints    */
        mtdcr   UIC1_SR, r1             /* likewise for the second UIC   */
        mtdcr   UIC2_SR, r1             /* clear all pending UIC ints    */
        mtdcr   UICB_SR, r1             /* likewise for the other UICs   */

#ifdef DEBUG_BOOTROM_EXCEPTIONS
	/*
	 * Normally, interrupt vector registers are set up when the
	 * excLib is initialized. For debugging exceptions in the
	 * romInit.s code, it can be helpful to include the following
	 * code.
	 */

	lis	r4, HIADJ(IVPR_BOOT_DEFAULT)
	addi	r4, r4, LO(IVPR_BOOT_DEFAULT)
	mtspr	IVPR, r4	/* Base of interrupt table	*/
	li	r4, IVOR0_VAL
	mtspr	IVOR0, r4	/* Critical Interrupt		*/
	li	r4, IVOR1_VAL
	mtspr	IVOR1, r4	/* Machine Check Interrupt	*/
	li	r4, IVOR2_VAL
	mtspr	IVOR2, r4	/* Data Storage Interrupt	*/
	li	r4, IVOR3_VAL
	mtspr	IVOR3, r4	/* Instruction Storage Interrupt */
	li	r4, IVOR4_VAL
	mtspr	IVOR4, r4	/* External Interrupt		*/
	li	r4, IVOR5_VAL
	mtspr	IVOR5, r4	/* Alignment Interrupt		*/
	li	r4, IVOR6_VAL
	mtspr	IVOR6, r4	/* Program Interrupt		*/
	li	r4, IVOR7_VAL
	mtspr	IVOR7, r4	/* Floating Point Unavailable Interrupt */
	li	r4, IVOR8_VAL
	mtspr	IVOR8, r4	/* System Call Interrupt	*/
	li	r4, IVOR9_VAL
	mtspr	IVOR9, r4	/* Aux Proc Unavailable Interrupt */
	li	r4, IVOR10_VAL
	mtspr	IVOR10, r4	/* Decrementer Interrupt	*/
	li	r4, IVOR11_VAL
	mtspr	IVOR11, r4	/* Fixed Interval Timer Interrupt */
	li	r4, IVOR12_VAL
	mtspr	IVOR12, r4	/* Watchdog Timer Interrupt	*/
	li	r4, IVOR13_VAL
	mtspr	IVOR13, r4	/* Data TLB Error Interrupt	*/
	li	r4, IVOR14_VAL
	mtspr	IVOR14, r4	/* Instruction TLB Error Interrupt */
	li	r4, IVOR15_VAL
	mtspr	IVOR15, r4	/* Debug Interrupt		*/
#endif /* DEBUG_BOOTROM_EXCEPTIONS */

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

#ifdef DDR_RESET_ERRATA
        /* 
         * Workaround for errata DDR_1.
         * Reset the SDRAM controller only if this is a cold boot.
         */

        li      r3, BOOT_COLD
        and.    r3, r3, r2          /* r2 is saved at the entry of romInit */
        beq     skip_ddr_reset

        /* 
         * DDR reset errata workaround 
         */ 

	lis	r3, HIADJ(SDR0_SRST_DMC)	/* DDR SDRAM soft reset bit */
	addis	r3, r3, LO(SDR0_SRST_DMC)
        addi    r4, r0, SDR0_SRST
        mtdcr   SDR0_CFGADDR, r4
        mtdcr   SDR0_CFGDATA, r3
        addis   r3, r0, 0x0000
        mtdcr   SDR0_CFGDATA, r3

skip_ddr_reset:
#endif

#ifdef TRACEB_EBMI_ERRATA
        /* 
         * Workaround for PLB timeouts caused by invalid EBMI cycle w/
         * Trace B enabled
         */
        lis     r3, 0x1000
        ori     r3, r3, 0x0000
        lis     r4, HI(EBM0_UAR)
        ori     r4, r4, LO(EBM0_UAR)
        mtdcr   EBM0_CFGADDR, r4
        mtdcr   EBM0_CFGDATA, r3
#endif

        /*
	 * Turn off 440GP compatibility mode; put chip in 440GX mode
         */

        addis   r3, r0, HI(~SDR0_MFR_PCM)
        ori     r3, r3, LO(~SDR0_MFR_PCM)
        addi    r4, r0, SDR0_MFR
        mtdcr   SDR0_CFGADDR, r4
        mfdcr   r4, SDR0_CFGDATA
        and     r4, r4, r3
        mtdcr   SDR0_CFGDATA, r4

        /*
         * L2/SRAM init.  Enable the onchip SRAM
         */

        bl      l2ConfigSram

        /*
         * STEP 0: External Bus Controller initialization.
         *
         * Initialize the External Bus Controller for external
         * peripherals. This is simple and we do it first to get it out
         * of the way.
         */

        bl      ext_bus_cntlr_init

        /*
         * STEP 1: Set up the PLB priorities
         */

        bl      priority_init

        /*
         * Configure the SDRAM controller only if this is a cold boot.
         * If the SDRAM controller is reinitialized on a warm boot, the
         * boot line will get wiped out because of the ECC SDRAM memory
         * initialization.
         */

        li      r3, BOOT_COLD
        and.    r3, r3, r2          /* r2 is saved at the entry of romInit */
        beq     skip

	/*
	 * Initialize stack pointer to point at onchip SRAM.
	 * It is used by C initialization code for IIC and SDRAM.
	 */

	lis	sp, HI(ROM_STACK_ADRS)
	ori	sp, sp, LO(ROM_STACK_ADRS)

    /*
     * Perform IIC and SDRAM initialization
     */ 

     bl      romSdramInit

skip:

	/*
	 * Clear the CPU reservation bit
	 */

	li	r0, 0
	lwarx	r3, r0, r0
	stwcx.	r3, r0, r0

	/* Initialize the stack pointer (r1) */

	lis	sp, HIADJ(STACK_ADRS)
	addi	sp, sp, LO(STACK_ADRS)

	/* small data area support would go here if it were supported */

	/* calculate C entry point: routine - entry point + ROM base */

	lis	r4, HIADJ(romStart)	/* r4 = romstart */
	addi	r4, r4, LO(romStart)

	lis	r5, HIADJ(romInit)	/* r5 = romInit */
	addi	r5, r5, LO(romInit)

	lis	r6, HIADJ(ROM_TEXT_ADRS) /* r6 = ROM_TEXT_ADRS */
	addi	r6, r6, LO(ROM_TEXT_ADRS)

	subf	r4, r5, r4		/* r4 = r4 - r5 */
	add	r4, r4, r6		/* r4 = r4 + r6 */
	mtlr	r4			/* link register = C entry point */

	mr	r3, r2			/* r3 = startType */
	addi	sp, sp, -FRAMEBASESZ	/* get frame stack */

	blr				/* branch to link register */
FUNC_END(romInit)


/***************************************************************************
*
* ext_bus_cntlr_init - init the Ext Bus Controller for external peripherals
*
* Bank 0 - Boot Flash/SRAM
* Bank 1 - NVRAM/RTC
* Bank 2 - Large Flash (one 4MB part)
* Bank 3 - 
* Bank 4 - Pinned out to Expansion connector
* Bank 5 - Pinned out to Expansion connector
* Bank 6 - Pinned out to Expansion connector
* Bank 7 - FPGA regs
*
* RETURNS: N/A
*
***************************************************************************/
FUNC_BEGIN(ext_bus_cntlr_init)

     /* 
      * Initalize EBC Config 
      */

        addi    r4, 0, EBC0_CFG
        mtdcr   EBC0_CFGADDR, r4
        lis     r4, HIADJ(EBC0_CFG_VALUE)     
        addi    r4, r4, LO(EBC0_CFG_VALUE)
        mtdcr   EBC0_CFGDATA, r4

        /*
         * Memory Bank 0 (64 MEG Flash) initialization
        */

        addi    r4, 0, EBC0_B0AP
        mtdcr   EBC0_CFGADDR, r4
        lis     r4, HI(BANK0_AP_VALUE)
        ori     r4, r4, LO(BANK0_AP_VALUE)
        mtdcr   EBC0_CFGDATA, r4

        addi    r4, 0, EBC0_B0CR
        mtdcr   EBC0_CFGADDR, r4
        lis     r4, HI(BANK0_CR_VALUE)
        ori     r4, r4, LO(BANK0_CR_VALUE)
        mtdcr   EBC0_CFGDATA, r4

        /*
         * Memory Bank 1 (1 M CPLD) initialization
         */

        addi    r4,0,EBC0_B1AP
        mtdcr   EBC0_CFGADDR, r4
        lis     r4, HI(BANK1_AP_VALUE)
        ori     r4, r4, LO(BANK1_AP_VALUE)
        mtdcr   EBC0_CFGDATA, r4

        addi    r4, 0, EBC0_B1CR
        mtdcr   EBC0_CFGADDR, r4
        lis     r4, HI(BANK1_CR_VALUE)
        ori     r4, r4, LO(BANK1_CR_VALUE)
        mtdcr   EBC0_CFGDATA, r4

        /*
         * Memory Bank 2 (1 M LCM) initialization
        */

        addi    r4, 0, EBC0_B2AP
        mtdcr   EBC0_CFGADDR, r4
        lis     r4, HI(BANK2_AP_VALUE)
        ori     r4, r4, LO(BANK2_AP_VALUE)
        mtdcr   EBC0_CFGDATA, r4

        addi    r4, 0, EBC0_B2CR
        mtdcr   EBC0_CFGADDR, r4
        lis     r4, HI(BANK2_CR_VALUE)
        ori     r4, r4, LO(BANK2_CR_VALUE)
        mtdcr   EBC0_CFGDATA, r4

        /*
         * Memory Bank 3 (128 M EBC connector) initialization
         */

        addi    r4, 0, EBC0_B3AP
        mtdcr   EBC0_CFGADDR, r4
        lis     r4, HI(BANK3_AP_VALUE)
        ori     r4, r4, LO(BANK3_AP_VALUE)
        mtdcr   EBC0_CFGDATA, r4

        addi    r4, 0, EBC0_B3CR
        mtdcr   EBC0_CFGADDR, r4
        lis     r4, HI(BANK3_CR_VALUE)
        ori     r4, r4, LO(BANK3_CR_VALUE)
        mtdcr   EBC0_CFGDATA, r4

        blr
 
FUNC_END(ext_bus_cntlr_init)


/***************************************************************************
*
* priority_init - Init the PLB Priorities
*
* RETURNS: N/A
*
***************************************************************************/

FUNC_BEGIN(priority_init)

	/* Set the PLB priority mode to fair */

	mfdcr	r5, PLB0_ACR
	lis	r6, HIADJ(~PLB_ACR_PPM_MASK)
	addi	r6, r6, LO(~PLB_ACR_PPM_MASK)
	and	r5, r5, r6
	lis	r6, HIADJ(PLB_ACR_PPM_FAIR)
	addi	r6, r6, LO(PLB_ACR_PPM_FAIR)
	or	r5, r5, r6
	mtdcr	PLB0_ACR, r5

	/* Set up alternate PLB master priority settings */

        addi  r4, 0, SDR0_AMP  
	mtdcr	SDR0_CFGADDR, r4
	mfdcr	r5, SDR0_CFGDATA      /* read SDR0_AMP value */
	lis	r6, HIADJ(SDR0_AMP_MASK)
	addi	r6, r6, LO(SDR0_AMP_MASK)
	and	r5, r5, r6
	lis	r6, HIADJ(SDR0_AMP_VALUE)
	addi	r6, r6, LO(SDR0_AMP_VALUE)
	or	r5, r5, r6
	mtdcr	SDR0_CFGDATA, r5        /* write new value */ 

	/* Set priorities for I-cache and D-cache */

        addi    r4, 0, SDR0_CP440
	mtdcr   SDR0_CFGADDR, r4  
	mfdcr   r5, SDR0_CFGDATA      /* read SDR0_CP440 value */
        lis     r6, HIADJ(SDR0_CP440_MASK)
        addi    r6, r6, LO(SDR0_CP440_MASK)
        and     r5, r5, r6
        lis     r6, HIADJ(SDR0_CP440_VALUE)
        addi    r6, r6, LO(SDR0_CP440_VALUE)
        or      r5, r5, r6
        mtdcr   SDR0_CFGDATA, r5      /* write new value */ 

        /* Peripheral component interconnect bridge controller */

        lis     r7, HIADJ(PCIX0_BRDGOPT1)
        addi    r7, r7, LO(PCIX0_BRDGOPT1)
        lwzu    r5, 0x0(r7)
        lis     r6, HIADJ(~BRDGOPT1_PLREQ)
        addi    r6, r6, LO(~BRDGOPT1_PLREQ)
        and     r5, r5, r6
        lis     r6, HIADJ(BRDGOPT1_PLREQ_0)
        addi    r6, r6, LO(BRDGOPT1_PLREQ_0)
        or      r5, r5, r6
        stw     r5, 0x0(r7)

        /* Memory access layer (MAL) */

        mfdcr   r5, (MAL0_DCR_BASE+MAL_CFG)
        lis     r6, HIADJ(~MAL_CR_PLBP_MASK)
        addi    r6, r6, LO(~MAL_CR_PLBP_MASK)
        and     r5, r5, r6
        lis     r6, HIADJ(MAL_CR_PLBP_0)
        addi    r6, r6, LO(MAL_CR_PLBP_0)
        or      r5, r5, r6
        mtdcr   (MAL0_DCR_BASE+MAL_CFG), r5

        /* Set DMA memory accesses to a specific priority */

        mfdcr   r5, DMA0_CR0
        lis     r6, HIADJ(~DMA_CR_CP)
        addi    r6, r6, LO(~DMA_CR_CP)
        and     r5, r5, r6
        lis     r6, HIADJ(DMA_CR_CP_0)
        addi    r6, r6, LO(DMA_CR_CP_0)
        or      r5, r5, r6
        mtdcr   DMA0_CR0, r5

        mfdcr   r5, DMA0_CR1
        lis     r6, HIADJ(~DMA_CR_CP)
        addi    r6, r6, LO(~DMA_CR_CP)
        and     r5, r5, r6
        lis     r6, HIADJ(DMA_CR_CP_0)
        addi    r6, r6, LO(DMA_CR_CP_0)
        or      r5, r5, r6
        mtdcr   DMA0_CR1, r5

        mfdcr   r5, DMA0_CR2
        lis     r6, HIADJ(~DMA_CR_CP)
        addi    r6, r6, LO(~DMA_CR_CP)
        and     r5, r5, r6
        lis     r6, HIADJ(DMA_CR_CP_0)
        addi    r6, r6, LO(DMA_CR_CP_0)
        or      r5, r5, r6
        mtdcr   DMA0_CR2, r5

        mfdcr   r5, DMA0_CR3
        lis     r6, HIADJ(~DMA_CR_CP)
        addi    r6, r6, LO(~DMA_CR_CP)
        and     r5, r5, r6
        lis     r6, HIADJ(DMA_CR_CP_0)
        addi    r6, r6, LO(DMA_CR_CP_0)
        or      r5, r5, r6
        mtdcr   DMA0_CR3, r5

        /* OPB to PLB bridge controller */

        mfdcr   r5, OPB0_CTRL
        lis     r6, HIADJ(~OPB_PRI_MASK)
        addi    r6, r6, LO(~OPB_PRI_MASK)
        and     r5, r5, r6
        lis     r6, HIADJ(OPB_PRI_0)
        addi    r6, r6, LO(OPB_PRI_0)
        or      r5, r5, r6
        mtdcr   OPB0_CTRL, r5

        blr

FUNC_END(priority_init)

/***************************************************************************
*
* sdramDcrGet - retrieve the contents of an indirect SDRAM0 register
*
* RETURNS: UINT32
*

* UINT32 sdramDcrGet
*	(
*	int registerAdrs		/@ sdram register address @/
*	)

*/

FUNC_BEGIN(sdramDcrGet)
	mtdcr	SDRAM0_CFGADDR, r3
	mfdcr	r3, SDRAM0_CFGDATA
	blr
FUNC_END(sdramDcrGet)

/***************************************************************************
*
* sdramDcrSet - store a value in an indirect SDRAM0 register
*
* RETURNS: N/A
*

* void sdramDcrSet
*	(
*	int registerAdrs,		/@ sdram register address @/
*	UINT32 data			/@ data to store in register @/
*	)

*/
FUNC_BEGIN(sdramDcrSet)
	mtdcr	SDRAM0_CFGADDR, r3
	mtdcr	SDRAM0_CFGDATA, r4
	blr
FUNC_END(sdramDcrSet)

/*Added drc set & get*/
FUNC_BEGIN(sdrDcrGet)
	mtdcr	SDR0_CFGADDR, r3
	mfdcr	r3, SDR0_CFGDATA
	blr
FUNC_END(sdrDcrGet)


FUNC_BEGIN(sdrDcrSet)
	mtdcr	SDR0_CFGADDR, r3
	mtdcr	SDR0_CFGDATA, r4
	blr
FUNC_END(sdrDcrSet)

/***************************************************************************
*
* sdramResetDelay - Delay to ensure 200usec have elapsed since reset.
*
*
* RETURNS: N/A
*

* void sdramResetDelay(void)

*/

FUNC_BEGIN(sdramResetDelay)
        addis   r3,r0,HI(SDRAM_RESET_DELAY_COUNT)
        ori     r3,r3,LO(SDRAM_RESET_DELAY_COUNT)
        mtctr   r3
resetLoop:
	bdnz	resetLoop	    /* spin loop */
	blr
FUNC_END(sdramResetDelay)

/***************************************************************************
*
* sdramTestPatterns - returns a pointer to sdram patterns
*
* The patterns are stored in an array of 8 test patterns, each with 8
* UINT32 values.  These sizes must be coordinated with NUMMEMTESTS and
* NUMMEMWORDS in romSdramInit.s
*
* RETURNS: UINT32 *
*
 
* UINT32 *sdramTestPatterns(void)
 
*/
FUNC_BEGIN(sdramTestPatterns)
	/* calculate test pattern point: routine - entry point + ROM base */

	lis	r4, HIADJ(testPatterns)	/* r4 = testPatterns */
	addi	r4, r4, LO(testPatterns)

	lis	r5, HIADJ(romInit)	/* r5 = romInit */
	addi	r5, r5, LO(romInit)

	lis	r6, HIADJ(ROM_TEXT_ADRS) /* r6 = ROM_TEXT_ADRS */
	addi	r6, r6, LO(ROM_TEXT_ADRS)

	subf	r4, r5, r4		/* r4 = r4 - r5 */
	add	r3, r4, r6		/* r3 = r4 + r6 */
	blr				/* done */
FUNC_END(sdramTestPatterns)

	/*
	 * store testPatterns in text section so they are correctly
	 * relocated.  Please coordinate these with NUMMEMTESTS and
	 * NUMMEMWORDS in romSdramInit.c
	 */
testPatterns:
	/* pattern 0 */
	.long	0x00000000, 0x00000000, 0xffffffff, 0xffffffff
	.long	0x00000000, 0x00000000, 0xffffffff, 0xffffffff

	/* pattern 1 */
	.long	0xffffffff, 0xffffffff, 0x00000000, 0x00000000
	.long	0xffffffff, 0xffffffff, 0x00000000, 0x00000000

	/* pattern 2 */
	.long	0xaaaaaaaa, 0xaaaaaaaa, 0x55555555, 0x55555555
	.long	0xaaaaaaaa, 0xaaaaaaaa, 0x55555555, 0x55555555

	/* pattern 3 */
	.long	0x55555555, 0x55555555, 0xaaaaaaaa, 0xaaaaaaaa
	.long	0x55555555, 0x55555555, 0xaaaaaaaa, 0xaaaaaaaa

	/* pattern 4 */
	.long	0xa5a5a5a5, 0xa5a5a5a5, 0x5a5a5a5a, 0x5a5a5a5a
	.long	0xa5a5a5a5, 0xa5a5a5a5, 0x5a5a5a5a, 0x5a5a5a5a

	/* pattern 5 */
	.long	0x5a5a5a5a, 0x5a5a5a5a, 0xa5a5a5a5, 0xa5a5a5a5
	.long	0x5a5a5a5a, 0x5a5a5a5a, 0xa5a5a5a5, 0xa5a5a5a5

	/* pattern 6 */
	.long	0xaa55aa55, 0xaa55aa55, 0x55aa55aa, 0x55aa55aa
	.long	0xaa55aa55, 0xaa55aa55, 0x55aa55aa, 0x55aa55aa

	/* pattern 7 */
	.long	0x55aa55aa, 0x55aa55aa, 0xaa55aa55, 0xaa55aa55
	.long	0x55aa55aa, 0x55aa55aa, 0xaa55aa55, 0xaa55aa55

/***************************************************************************
*
* sdramDcbf - perform data cache block flush operation
*
* RETURNS: N/A
*

* void sdramDcbf
*    (
*    void * blockAdrs			/@ Address to flush from D-cache @/
*    )

*/
FUNC_BEGIN(sdramDcbf)
	dcbf	0, r3
	blr
FUNC_END(sdramDcbf)

/***************************************************************************
*
* sdramDflush - perform entire data cache flush operation
*
* In order to flush the entire data cache, read a cacheful of data from
* a known address space, then flush all cache lines matching addresses
* in that address space.
*
* RETURNS: N/A
*

* void sdramDflush(void)

*/
FUNC_BEGIN(sdramDflush)
	lis	r3, HIADJ(ROM_TEXT_ADRS)	/* r3 = cached boot device */
	addi	r3, r3, LO(ROM_TEXT_ADRS)

	mfspr	r6, CTR
	msync

	lis	r4, HIADJ(_DCACHE_LINE_NUM_440GX * _DCACHE_NUM_WAYS)
	addi	r4, r4, LO(_DCACHE_LINE_NUM_440GX * _DCACHE_NUM_WAYS)
	mtspr	CTR, r4
flush1:
	lwz	r4, 0(r3)
	dcbf	0, r3
	addi	r3, r3, _CACHE_ALIGN_SIZE
	bdnz	flush1

	msync
	mtspr	CTR, r6
	blr
FUNC_END(sdramDflush)

/***************************************************************************
*
* l2ConfigSram - configure L2 data arrays as SRAM
*
* RETURNS: N/A
*

* void l2ConfigSram(void)

*/
FUNC_BEGIN(l2ConfigSram)

      msync

      /* 
       * Init L2 Cache Configuration register for SRAM
       */

      mfdcr   r5, L2C0_CFG
      lis     r6, HIADJ(L2C0_CFG_MASK)
      addi    r6, r6, LO(L2C0_CFG_MASK)
      and     r5, r5, r6
      lis     r6, HIADJ(L2C0_CFG_VALUE)
      addi    r6, r6, LO(L2C0_CFG_VALUE)
      or      r5, r5, r6
      mtdcr   L2C0_CFG, r5

	msync

      /* 
       * Disable snoop regions
       */

      mfdcr   r5, L2C0_SNP0
      lis     r6, HIADJ(L2C0_SNP_MASK)
      addi    r6, r6, LO(L2C0_SNP_MASK)
      and     r5, r5, r6
      lis     r6, HIADJ(L2C0_SNP_VALUE)
      addi    r6, r6, LO(L2C0_SNP_VALUE)
      or      r5, r5, r6
      mtdcr   L2C0_SNP0, r5

      mfdcr   r5, L2C0_SNP1
      lis     r6, HIADJ(L2C0_SNP_MASK)
      addi    r6, r6, LO(L2C0_SNP_MASK)
      and     r5, r5, r6
      lis     r6, HIADJ(L2C0_SNP_VALUE)
      addi    r6, r6, LO(L2C0_SNP_VALUE)
      or      r5, r5, r6
      mtdcr   L2C0_SNP1, r5

      /* 
       * Enable SRAM 
       */

      /* Clear Bus Error Status Registers */
      xor     r0, r0, r0          
      mtdcr   SRAM0_BESR0, r0
      mtdcr   SRAM0_BESR0, r0

      mfdcr   r5, SRAM0_DPC
      lis     r6, HIADJ(SRAM0_DPC_MASK)
      addi    r6, r6, LO(SRAM0_DPC_MASK)
      and     r5, r5, r6
      lis     r6, HIADJ(SRAM0_DPC_VALUE)
      addi    r6, r6, LO(SRAM0_DPC_VALUE)
      or      r5, r5, r6
      mtdcr   SRAM0_DPC, r5

      /* Clear Power Management Register */

      mfdcr   r5, SRAM0_PMEG
      lis     r6, HIADJ(SRAM0_PMEG_MASK)
      addi    r6, r6, LO(SRAM0_PMEG_MASK)
      and     r5, r5, r6
      mtdcr   SRAM0_PMEG, r5

      /* Memory configuration Registers */

      mfdcr   r5, SRAM0_SB0CR
      lis     r6, HIADJ(SRAM0_SBXCR_MASK)
      addi    r6, r6, LO(SRAM0_SBXCR_MASK)
      and     r5, r5, r6
      lis     r6, HIADJ(SRAM0_SB0CR_VALUE)
      addi    r6, r6, LO(SRAM0_SB0CR_VALUE)
      or      r5, r5, r6
      mtdcr   SRAM0_SB0CR, r5

      mfdcr   r5, SRAM0_SB1CR
      lis     r6, HIADJ(SRAM0_SBXCR_MASK)
      addi    r6, r6, LO(SRAM0_SBXCR_MASK)
      and     r5, r5, r6
      lis     r6, HIADJ(SRAM0_SB1CR_VALUE)
      addi    r6, r6, LO(SRAM0_SB1CR_VALUE)
      or      r5, r5, r6
      mtdcr   SRAM0_SB1CR, r5

      mfdcr   r5, SRAM0_SB2CR
      lis     r6, HIADJ(SRAM0_SBXCR_MASK)
      addi    r6, r6, LO(SRAM0_SBXCR_MASK)
      and     r5, r5, r6
      lis     r6, HIADJ(SRAM0_SB2CR_VALUE)
      addi    r6, r6, LO(SRAM0_SB2CR_VALUE)
      or      r5, r5, r6
      mtdcr   SRAM0_SB2CR, r5

      mfdcr   r5, SRAM0_SB3CR
      lis     r6, HIADJ(SRAM0_SBXCR_MASK)
      addi    r6, r6, LO(SRAM0_SBXCR_MASK)
      and     r5, r5, r6
      lis     r6, HIADJ(SRAM0_SB3CR_VALUE)
      addi    r6, r6, LO(SRAM0_SB3CR_VALUE)
      or      r5, r5, r6
      mtdcr   SRAM0_SB3CR, r5

FUNC_END(l2ConfigSram)

/***************************************************************************
*
* romPvrGet - get Processor Version Register contents
*
* RETURNS: UINT32
*
* UINT32 romPvrGet(void)
*/
FUNC_BEGIN(romPvrGet)
	mfspr	r3, PVR
	blr
FUNC_END(romPvrGet)

/***************************************************************************
*
* timeBaseGet - Get the free running timebase
*
* RETURNS: UINT32
*

* UINT32 timeBaseGet(void)

*/
FUNC_BEGIN(timeBaseGet)
	mfspr	r3, TBL_R
	blr
FUNC_END(timeBaseGet)

/*****************************************************************************
*
* ppcDcbzArea - this function performs a dcbz across an area of memory
*
* ARGUMENTS:
*	r3 = start address  
*	r4 = area size
*
* RETURNS:  N/A 
*/

FUNC_BEGIN(ppcDcbzArea)
	rlwinm.  r5,r4,0,27,31
	rlwinm   r5,r4,27,5,31
	beq      ..d_ran
	addi     r5,r5,0x0001
..d_ran:
	mtctr	   r5
..d_ag: 
	dcbz     r0,r3
	addi     r3,r3,32
	bdnz     ..d_ag
	blr
FUNC_END(ppcDcbzArea)

/***************************************************************************
*
* The following section is based on the IBM PIBS BIOS
* init_core.s, init_core().
*
***************************************************************************/

/* -------------------------------------------------------------------------
*
*
*	This source code has been made available to you by IBM on an AS-IS
*	basis.	Anyone receiving this source is licensed under IBM
*	copyrights to use it in any way he or she deems fit, including
*	copying it, modifying it, compiling it, and redistributing it either
*	with or without modifications.	No license under IBM patents or
*	patent applications is to be implied by the copyright license.
*
*	Any user of this software should understand that IBM cannot provide
*	technical support for this software and will not be responsible for
*	any consequences resulting from the use of this software.
*
*	Any person who transfers this source code or any derivative work
*	must include the IBM copyright notice, this paragraph, and the
*	preceding two paragraphs in the transferred software.
*
*	COPYRIGHT   I B M   CORPORATION 1995
*	LICENSED MATERIAL  -  PROGRAM PROPERTY OF I B M
* --------------------------------------------------------------------------
*
* File: init_core.s
* Purpose: Basic PPC440 core initialization.
*
* --------------------------------------------------------------------------
*/

/***************************************************************************
*
* resetEntry - rom entry point
*
* resetEntry is the first function to be called by the PPC440 core after
* reset. It is located at ROM address 0xfffff000. It is responsible for
* initializing key registers to a quiescent state, initializing the the
* TLB with a static memory mapping adequate for running the bootrom, and
* clearing the rest of the TLB. It sets up device access registers so
* the rest of the SRAM (or Boot Flash) can be correctly accessed.
* Finally, it sets up SRR0 and SRR1 with a safe MSR value and the
* instruction address of the romInit cold start entry point, and
* 'returns from interrupt' to synchronize processor context and continue
* the boot process in romInit.
*
* Main Initialization Steps:
*      1. Invalidate the instruction cache (iccci)
*      2. Invalidate the data cache (dccci)
*      3. Synchronize memory accesses (msync)
*      4. Clear DBCR0 register (disable all debug events)
*      5. Clear DBSR register (initialize all debug event status)
*      6. Initialize CCR0 register
*      7. Configure instruction and data cache regions
*      8. Set TLB entries to at least cover initial program memory page
*      9. Call rfi to cause a context synchronization and jump to romInit
*
* IMPORTANT NOTE: At chip reset, the TLB contents are unknown and there
*	will be only one entry in the ITLB to cover a region from
*	0xFFFFF000 to 0xFFFFFFFF. Therefore the resetEntry routine must
*	be  positioned to fall within this region.
*
*	Since the 440 only initializes an architecturally-invisible
*	shadow	TLB entry during the reset operation, and since all
*	shadow TLB entries  are invalidated upon any context
*	synchronization, special care must be  taken during the
*	initialization sequence to prevent any such context
*	synchronizing operations (such as interrupts and the isync
*	instruction)  until after this step is completed, and an
*	architected TLB entry has  been established in the TLB.
*
*/
#if   defined(_GNU_TOOL)
	.section .boot, "ax", @progbits
#elif defined(_DIAB_TOOL)
        .section .boot, 4, "rx"
#else
#error "Please add a correctly spelled .section directive for your toolchain."
#endif

FUNC_BEGIN(resetEntry)

	/* Invalidate the instruction cache (iccci) */

	iccci r0, r0

        /* Invalidate the data cache (dccci) */

        dccci r0, r0

        /*
         * Synchronize memory accesses (msync)
         *
         * This step forces any data PLB operations that may have been
         * in  progress prior to the reset operation to complete,
         * thereby allowing  subsequent data accesses to be initiated
         * and completed properly.
         */
        msync

        xor   r0, r0, r0          /* r0 = 0 */

        /* Clear SRR0, CSRR0, SRR1, CSRR1, MCSRR0, MCSRR1 */

        mtspr SRR0, r0
        mtspr SRR1, r0
        mtspr CSRR0, r0
        mtspr CSRR1, r0
        mtspr MCSRR0, r0
        mtspr MCSRR1, r0

#ifdef CLEAR_DEBUG_REGISTERS
        /*
         * Clear DBCR0 register (disable all debug events)
         * Reset all other debugging registers associated with DBCR0.
         * Although the 440 is defined to reset some of the debug
         * event  enables during the reset operation, this is not
         * required by the  architecture and hence the initialization
         * software should not assume  this behavior. Software should
         * disable all debug events in order to  prevent
         * non-deterministic behavior on the trace interface to the
         * core.
         */

        mtspr DBCR0, r0
        mtspr DBCR1, r0
        mtspr DBCR2, r0
        mtspr IAC1, r0
        mtspr IAC2, r0
        mtspr IAC3, r0
        mtspr IAC4, r0
        mtspr DAC1, r0
        mtspr DAC2, r0
        mtspr DVC1, r0
        mtspr DVC2, r0

        addis r0,r0,HI(DBSR_INIT)
        ori   r0,r0,LO(DBSR_INIT)
        mtspr DBSR, r0			/* write 1s to clear */
#endif /* CLEAR_DEBUG_REGISTERS */

	/* Initialize general load/store and cache operations */

        addis r0, r0, HI(CCR0_INIT)
        ori   r0, r0, LO(CCR0_INIT)
        mtspr CCR0, r0			/* Load/Store/General cache config */

        addis r0, r0, HI(CCR1_INIT)
        ori   r0, r0, LO(CCR1_INIT)
        mtspr CCR1, r0			/* Cache parity/Flush/Timebase config */

        /*
         * Reset the timebase
         * The previous write to CCR1 sets the timebase source
         */
        xor  r0, r0, r0
        mtspr TBL, r0
        mtspr TBU, r0

        /*
         * Configure instruction and data cache regions
         * These steps must be performed prior to enabling the caches
         * a. Clear the instruction and data cache normal victim index
         *    registers
         * b. Clear the instruction and data cache transient victim index
         *    regs
         * c. Set the instruction and data cache victim limit registers
         */

        mtspr INV0, r0
        mtspr INV1, r0
        mtspr INV2, r0
        mtspr INV3, r0
        mtspr DNV0, r0
        mtspr DNV1, r0
        mtspr DNV2, r0
        mtspr DNV3, r0
        mtspr ITV0, r0
        mtspr ITV1, r0
        mtspr ITV2, r0
        mtspr ITV3, r0
        mtspr DTV0, r0
        mtspr DTV1, r0
        mtspr DTV2, r0
        mtspr DTV3, r0
        addis r0, r0, HI(IVLIM_INIT)
        ori   r0, r0, LO(IVLIM_INIT)
        mtspr IVLIM, r0
        mtspr DVLIM, r0		/* note: DVLIM configured same as IVLIM */

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

        xor  r1, r1, r1	     /* start with TLB #0 */

#ifdef INCLUDE_440X5_TLB_RECOVERY_MAX
        /*
         * Tlb #0.  DDR SDRAM Machine Check Handler - Uncached
	 *     VEC_BASE_ADRS + 0x4000 -> 0x000004000.  16KB
         * Attributes: X/W/R
         */
	 
	TLB_WORD_SET(0, r1, r0, (VEC_BASE_ADRS + 0x4000) /* EPN */ \
		     | TLB_V | _MMU_TLB_SZ_16K)
	TLB_WORD_SET(1, r1, r0, 0x0 | 0x000004000)	 /* RPN */
	TLB_WORD_SET(2, r1, r0, _INHIBIT_AND_G \
		     | TLB_UX | TLB_UW | TLB_UR | TLB_SX | TLB_SW | TLB_SR)

	TLB_NEXT(r1)					/* next TLB */
#endif /* INCLUDE_440X5_TLB_RECOVERY_MAX */

        /*
         * Tlb #0 (or 1).  DDR SDRAM 0 - Cached. This includes the Machine
	 * Check Handler when not using the INCLUDE_440X5_TLB_RECOVERY_MAX
	 * option.
	 *     VEC_BASE_ALT_ADRS -> 0x000000000.  256MB
         * Attributes: X/W/R
         */

	TLB_WORD_SET(0, r1, r0, VEC_BASE_ALT_ADRS	/* EPN */ \
		     | TLB_V | _MMU_TLB_SZ_256M)
	TLB_WORD_SET(1, r1, r0, 0x0 | 0x000000000)	/* RPN */
	TLB_WORD_SET(2, r1, r0, _WRITETHROUGH_AND_G \
		     | TLB_UX | TLB_UW | TLB_UR | TLB_SX | TLB_SW | TLB_SR)

#ifndef INCLUDE_440X5_TLB_RECOVERY_MAX
        /*
         * Tlb #1 unless INCLUDE_440X5_TLB_RECOVERY_MAX.
	 * DDR SDRAM Machine Check Handler - Uncached
	 *     0x80000000 -> 0x000000000.  256MB
	 * TLB_NUM_SDRAM_0 must be coordinated with this setting
         * Attributes: I/W/R
         */

	TLB_NEXT(r1)					/* next TLB */
	TLB_WORD_SET(0, r1, r0, 0x80000000 		/* EPN */ \
		     | TLB_V | _MMU_TLB_SZ_256M)
	TLB_WORD_SET(1, r1, r0, 0x0 | 0x000000000)	/* RPN */
	TLB_WORD_SET(2, r1, r0, _INHIBIT \
		     |          TLB_UW | TLB_UR |          TLB_SW | TLB_SR)
#endif /* INCLUDE_440X5_TLB_RECOVERY_MAX */

        /*
         * Tlb #2.  Peripherals - 0xE0000000 -> 0x140000000.  256MB
         * Attributes: I/G/W/R
         */

	TLB_NEXT(r1)					/* next TLB */
	TLB_WORD_SET(0, r1, r0, 0xe0000000 		/* EPN */ \
		     | TLB_V | _MMU_TLB_SZ_256M)
	TLB_WORD_SET(1, r1, r0, 0x1 | 0x40000000)	/* RPN */
	TLB_WORD_SET(2, r1, r0, _INHIBIT_AND_G \
		     |          TLB_UW | TLB_UR |          TLB_SW | TLB_SR)

        /*
         * Tlb #3.  Boot Space - 0xF0000000 -> 0x1F0000000.  256MB
	 * TLB_NUM_BOOT_SPACE must be coordinated with this setting
         * Attributes: W/X/R
         */

	TLB_NEXT(r1)					/* next TLB */
	TLB_WORD_SET(0, r1, r0, 0xf0000000 		/* EPN */ \
		     | TLB_V | _MMU_TLB_SZ_256M)
	TLB_WORD_SET(1, r1, r0, 0x1 | 0xf0000000)	/* RPN */
	TLB_WORD_SET(2, r1, r0, _WRITETHROUGH | BOOT_SPACE_WRITE \
		     | TLB_UX |          TLB_UR | TLB_SX          | TLB_SR)

        /*
         * Tlb #4.  PCI Memory - 0x90000000 -> 0x300000000.  256MB
         * Attributes: I/G/X/W/R
         * Used for PCI memory that does not allow prefetching.
         */

	TLB_NEXT(r1)					/* next TLB */
	TLB_WORD_SET(0, r1, r0, 0x90000000		/* EPN */ \
		     | TLB_V | _MMU_TLB_SZ_256M)
	TLB_WORD_SET(1, r1, r0, 0x3 | 0x000000000)	/* RPN */
	TLB_WORD_SET(2, r1, r0, _INHIBIT_AND_G \
		     | TLB_UX | TLB_UW | TLB_UR | TLB_SX | TLB_SW | TLB_SR)

        /*
         * Tlb #5.  PCI Memory - 0xa0000000 -> 0x310000000.  256MB
         * Attributes: X/W/R
         */

	TLB_NEXT(r1)					/* next TLB */
	TLB_WORD_SET(0, r1, r0, 0xa0000000		/* EPN */ \
		     | TLB_V | _MMU_TLB_SZ_256M)
	TLB_WORD_SET(1, r1, r0, 0x3 | 0x10000000)	/* RPN */
	TLB_WORD_SET(2, r1, r0, _INHIBIT \
		     | TLB_UX | TLB_UW | TLB_UR | TLB_SX | TLB_SW | TLB_SR)

        /*
         * Tlb #6.  I2O Memory - 0xb0000000 -> 0x0f0000000.  256MB
         * Attributes: I/G/W/R
         */

	TLB_NEXT(r1)					/* next TLB */
	TLB_WORD_SET(0, r1, r0, 0xb0000000		/* EPN */ \
		     | TLB_V | _MMU_TLB_SZ_256M)
	TLB_WORD_SET(1, r1, r0, 0x0 | 0xf0000000)	/* RPN */
	TLB_WORD_SET(2, r1, r0, _INHIBIT_AND_G \
		     |          TLB_UW | TLB_UR |          TLB_SW | TLB_SR)

        /*
         * Tlb #7.  Int SRAM - 0xC0000000 -> 0x080000000.  256KB
	 * Note the size is just large enough for SRAM, and a hole
	 * follows where a machine machine check handler mapping can be
	 * placed.
         * Attributes: X/W/R
         */

	TLB_NEXT(r1)					/* next TLB */
	TLB_WORD_SET(0, r1, r0, 0xc0000000		/* EPN */ \
		     | TLB_V | _MMU_TLB_SZ_256K)
	TLB_WORD_SET(1, r1, r0, 0x0 | 0x80000000)	/* RPN */
	TLB_WORD_SET(2, r1, r0, _WRITETHROUGH_OR_G \
		     | TLB_UX | TLB_UW | TLB_UR | TLB_SX | TLB_SW | TLB_SR)

        /*
         * Tlb #8.  PCI - 0xD0000000 -> 0x200000000.  256MB
         * Attributes: I/G/W/R
         */

	TLB_NEXT(r1)					/* next TLB */
	TLB_WORD_SET(0, r1, r0, 0xd0000000		/* EPN */ \
		     | TLB_V | _MMU_TLB_SZ_256M)
	TLB_WORD_SET(1, r1, r0, 0x2 | 0x00000000)	/* RPN */
	TLB_WORD_SET(2, r1, r0, _INHIBIT_AND_G \
		     |          TLB_UW | TLB_UR |          TLB_SW | TLB_SR)

        /*
         * Tlb #9.  DDR SDRAM - Cached 0x10000000 -> 0x010000000.  256MB
         * Attributes: X/W/R
         */

	TLB_NEXT(r1)					/* next TLB */
	TLB_WORD_SET(0, r1, r0, 0x10000000 		/* EPN */ \
		     | TLB_V | _MMU_TLB_SZ_256M)
	TLB_WORD_SET(1, r1, r0, 0x0 | 0x10000000)	/* RPN */
	TLB_WORD_SET(2, r1, r0, _WRITETHROUGH_AND_G \
		     | TLB_UX | TLB_UW | TLB_UR | TLB_SX | TLB_SW | TLB_SR)

        /*
         * Tlb #10.  DDR SDRAM - Cached 0x20000000 -> 0x020000000.  256MB
         * Attributes: X/W/R
         */

	TLB_NEXT(r1)					/* next TLB */
	TLB_WORD_SET(0, r1, r0, 0x20000000 		/* EPN */ \
		     | TLB_V | _MMU_TLB_SZ_256M)
	TLB_WORD_SET(1, r1, r0, 0x0 | 0x20000000)	/* RPN */
	TLB_WORD_SET(2, r1, r0, _WRITETHROUGH_AND_G \
		     | TLB_UX | TLB_UW | TLB_UR | TLB_SX | TLB_SW | TLB_SR)

        /*
         * Tlb #11.  DDR SDRAM - Cached 0x30000000 -> 0x030000000.  256MB
         * Attributes: X/W/R
         */

	TLB_NEXT(r1)					/* next TLB */
	TLB_WORD_SET(0, r1, r0, 0x30000000 		/* EPN */ \
		     | TLB_V | _MMU_TLB_SZ_256M)
	TLB_WORD_SET(1, r1, r0, 0x0 | 0x30000000)	/* RPN */
	TLB_WORD_SET(2, r1, r0, _WRITETHROUGH_AND_G \
		     | TLB_UX | TLB_UW | TLB_UR | TLB_SX | TLB_SW | TLB_SR)

        /*
         * Tlb #12.  DDR SDRAM - cached 0x40000000 -> 0x040000000.  256MB
         * Attributes: X/W/R
         */

	TLB_NEXT(r1)					/* next TLB */
	TLB_WORD_SET(0, r1, r0, 0x40000000 		/* EPN */ \
		     | TLB_V | _MMU_TLB_SZ_256M)
	TLB_WORD_SET(1, r1, r0, 0x0 | 0x40000000)	/* RPN */
	TLB_WORD_SET(2, r1, r0, _WRITETHROUGH_AND_G \
		     | TLB_UX | TLB_UW | TLB_UR | TLB_SX | TLB_SW | TLB_SR)

        /*
         * Tlb #13.  DDR SDRAM - Cached 0x50000000 -> 0x050000000.  256MB
         * Attributes: X/W/R
         */

	TLB_NEXT(r1)					/* next TLB */
	TLB_WORD_SET(0, r1, r0, 0x50000000 		/* EPN */ \
		     | TLB_V | _MMU_TLB_SZ_256M)
	TLB_WORD_SET(1, r1, r0, 0x0 | 0x50000000)	/* RPN */
	TLB_WORD_SET(2, r1, r0, _WRITETHROUGH_AND_G \
		     | TLB_UX | TLB_UW | TLB_UR | TLB_SX | TLB_SW | TLB_SR)

        /*
         * Tlb #14.  DDR SDRAM - Cached 0x60000000 -> 0x060000000.  256MB
         * Attributes: X/W/R
         */

	TLB_NEXT(r1)					/* next TLB */
	TLB_WORD_SET(0, r1, r0, 0x60000000 		/* EPN */ \
		     | TLB_V | _MMU_TLB_SZ_256M)
	TLB_WORD_SET(1, r1, r0, 0x0 | 0x60000000)	/* RPN */
	TLB_WORD_SET(2, r1, r0, _WRITETHROUGH_AND_G \
		     | TLB_UX | TLB_UW | TLB_UR | TLB_SX | TLB_SW | TLB_SR)

        /*
         * Tlb #15.  DDR SDRAM - Cached 0x70000000 -> 0x070000000.  256MB
         * Attributes: X/W/R
         */

	TLB_NEXT(r1)					/* next TLB */
	TLB_WORD_SET(0, r1, r0, 0x70000000 		/* EPN */ \
		     | TLB_V | _MMU_TLB_SZ_256M)
	TLB_WORD_SET(1, r1, r0, 0x0 | 0x70000000)	/* RPN */
	TLB_WORD_SET(2, r1, r0, _WRITETHROUGH_AND_G \
		     | TLB_UX | TLB_UW | TLB_UR | TLB_SX | TLB_SW | TLB_SR)

#ifdef INCLUDE_440X5_TLB_RECOVERY_MAX
        /*
         * Tlb #16.  DDR SDRAM 0 - Uncached
	 * For use when INCLUDE_440X5_TLB_RECOVERY_MAX is configured
	 *     0x80000000 -> 0x000000000.  256MB
         * Attributes: I/W/R
         */

	TLB_NEXT(r1)					/* next TLB */
	TLB_WORD_SET(0, r1, r0, 0x80000000 		/* EPN */ \
		     | TLB_V | _MMU_TLB_SZ_256M)
	TLB_WORD_SET(1, r1, r0, 0x0 | 0x00000000)	/* RPN */
	TLB_WORD_SET(2, r1, r0, _INHIBIT_AND_G \
		     | TLB_UX | TLB_UW | TLB_UR | TLB_SX | TLB_SW | TLB_SR)

        /*
         * Tlb #17.  DDR SDRAM 0 - Cached
	 * Vectors other than Machine Check (cached, execute-only)
	 * For use when INCLUDE_440X5_TLB_RECOVERY_MAX is configured
	 *     VEC_BASE_ADRS -> 0x000000000.  16KB
         * Attributes: X/R
         */

	TLB_NEXT(r1)					/* next TLB */
	TLB_WORD_SET(0, r1, r0, VEC_BASE_ADRS 		/* EPN */ \
		     | TLB_V | _MMU_TLB_SZ_16K)
	TLB_WORD_SET(1, r1, r0, 0x0 | 0x00000000)	/* RPN */
	TLB_WORD_SET(2, r1, r0, _WRITETHROUGH_AND_G \
		     | TLB_UX |          TLB_UR | TLB_SX |          TLB_SR)
#endif /* INCLUDE_440X5_TLB_RECOVERY_MAX */

        /*
         * Invalidate the rest of the TLB entries
         */

        subfic r0, r1, (N_TLB_ENTRIES-1)    /* N_TLB_ENTRIES - 1 - Num tlb */
					    /* entries already set up.     */
        mtspr CTR, r0         	/* Set the counter for the branch */
        addis r0, r0, 0x0000	/* EPN = 0x00000000, Not Valid. */
TLB_invalidate_loop:
	addi  r1, r1, 0x0001	/* Increment to the next TLB entry */
	tlbwe r0, r1, 0x0000	/* Write TLB entry word 0 */
	tlbwe r0, r1, 0x0001	/* Write TLB entry word 1 */
	tlbwe r0, r1, 0x0002 	/* Write TLB entry word 2 */
	bdnz  TLB_invalidate_loop

        /* Context switch */
        isync   

	/*
	 * Initialize PID reg to match TID field of TLB entry (unless
	 * using TID=0)
	 */

	xor   r0, r0, r0
	mtspr PID, r0

	/*
	 * Setup for subsequent change in instruction fetch address
	 */

	lis   r0, HI(MSR_VALUE)
	ori   r0, r0, LO(MSR_VALUE)
	mtspr SRR1, r0	      /* Write the new MSR value into SRR1 */

	/* calculate C entry point: routine - entry point + ROM base */

	lis	r4, HIADJ(cold) 	/* r4 = cold */
	addi	r4, r4, LO(cold)

	lis	r5, HIADJ(romInit)	/* r5 = romInit */
	addi	r5, r5, LO(romInit)

	lis	r6, HIADJ(ROM_TEXT_ADRS) /* r6 = ROM_TEXT_ADRS */
	addi	r6, r6, LO(ROM_TEXT_ADRS)

	subf	r4, r5, r4		/* r4 = r4 - r5 */
	add	r4, r4, r6		/* r4 = r4 + r6 */

	mtspr SRR0, r4	    /* Write addr from which to cont exec into SRR0 */

	/*
	 * Context synchronize to invalidate shadow TLB contents and
	 * cause new TLB  contents to take effect. Jump to main boot
	 * code.
	 */

	rfi
FUNC_END(resetEntry)

/***************************************************************************
*
* resetVector - hardware reset vector
*
* At power-on, the PPC440 starts executing at ROM address 0xfffffffc -- the
* top of the address space -- which must be a jump to the reset entry point.
* This is defined as a separate "section" to assist the linker in locating
* it properly.
*/
#if   defined(_GNU_TOOL)
	.section .reset, "ax", @progbits
#elif defined(_DIAB_TOOL)
	.section .reset, 4, "rx"
#else
#error "Please add a correctly spelled .section directive for your toolchain."
#endif
FUNC_BEGIN(resetVector)
	b	resetEntry
FUNC_END(resetVector)
