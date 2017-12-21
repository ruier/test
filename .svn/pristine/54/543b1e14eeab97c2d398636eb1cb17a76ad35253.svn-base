/* romInit.s - AMCC440EP PPC440EP ROM initialization module */

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

   COPYRIGHT   I B M   CORPORATION 2003
   LICENSED MATERIAL  -  PROGRAM PROPERTY OF  I B M"

********************************************************************************
\NOMANUAL
*/

/* Copyright 1984-2005 Wind River Systems, Inc. */


/*
*****************************************************************************
   COPYRIGHT   Applied Micro Circuits Corporation (AMCC) 2004
   LICENSED MATERIAL  -  PROGRAM PROPERTY OF AMCC
   All rights reserved
   AMCC Proprietary

********************************************************************************
\NOMANUAL
*/



	.data

/*
modification history
--------------------
01a,09mar06,mmi mods history updated, TLB's updated, replaced MACRO's with
                hard coded values
01a,09mar06,mmi Written based on bamboo romInit.s version 01d.
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
routine sysToMonitor() jumps to the location 4 bytes past the beginning
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

    2 Processor Local Busses (The high-performance bus for master devices):
    Processor Local Bus 4 (PLB4) -  CPU I- and D-Cache, SDRAM, DMA,
    PLB4->PLB3 Bridge and the PLB4->OPB3 Bridge all reside on this bus.

    Processor Local Bus 3 (PLB3)(through PLB4->PLB3 Bridge) - PCI, EBC,
    DMA, MAL, PLB3->OPB3 Bridge

    2 Onchip Peripheral Busses (A bus for non-busmaster onchip devices):
    Onchip Peripheral Bus (OPB0) (through PLB4->OPB3 Bridge) -
    USB2.0 Device controller
    Onchip Peripheral Bus (OPB1) (through PLB3->OPB3 Bridge) -
    EMAC, UART, SPI, IIC, etc. plus the OPB->EB bridge.

    External Bus (EB) - Offchip device bus

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
 * Location of temporary stack in Data Cache, for use
 * before SDRAM0 controller is configured.
 */

#define ROM_STACK_ADRS	(0xF0004000)
#define ROM_STACK_SIZE	(0x00004000)

#define IVPR_BOOT_DEFAULT 0xfff90000

/*
 * EBC0 configuration registers and access parameters registers
 */
#define EBC_B0AP_VALUE   (0x03017300) 
#define EBC_B0CR_VALUE   (0xFE0BA000) 

#define EBC_B1AP_VALUE   (0x0) 
#define EBC_B1CR_VALUE   (0x0) 
#define EBC_B2AP_VALUE   (0x04814500) 
#define EBC_B2CR_VALUE   (0x80018000) 
#define EBC_B3AP_VALUE   (0x0) 
#define EBC_B3CR_VALUE   (0x0) 
#define EBC_B4AP_VALUE   (0x0) 
#define EBC_B4CR_VALUE   (0x0) 
#define EBC_B5AP_VALUE   (0x0) 
#define EBC_B5CR_VALUE   (0x0) 

/*
 * MSR register initialization
 *
 * This initial MSR value allows machine check and critical class
 * exceptions.
 */

#define MSR_VALUE	(_PPC_MSR_CE|_PPC_MSR_ME)

/* General System DCR configuration parameters */

#define SDR0_AMPX_MASK  (~(SDR0_AMPX_APM0_MASK | SDR0_AMPX_APM1_MASK | SDR0_AMPX_APM2_MASK | \
		SDR0_AMPX_APM3_MASK | SDR0_AMPX_APM5_MASK | SDR0_AMPX_APM6_MASK | \
		SDR0_AMPX_APM7_MASK | SDR0_AMPX_M0PS_MASK | SDR0_AMPX_M1PS_MASK | \
		SDR0_AMPX_M2PS_MASK | SDR0_AMPX_M3PS_MASK | SDR0_AMPX_M5PS_MASK | \
		SDR0_AMPX_M6PS_MASK | SDR0_AMPX_M7PS_MASK ))

#define SDR0_AMPX_VALUE  (SDR0_AMPX_APM0_0    | SDR0_AMPX_APM1_0    | SDR0_AMPX_APM2_0    | \
		SDR0_AMPX_APM3_0    | SDR0_AMPX_APM5_0    | SDR0_AMPX_APM6_0    | \
		SDR0_AMPX_APM7_0    | SDR0_AMPX_M0PS_ALT  | SDR0_AMPX_M1PS_ALT  | \
		SDR0_AMPX_M2PS_ALT  | SDR0_AMPX_M3PS_ALT  | SDR0_AMPX_M5PS_ALT  | \
		SDR0_AMPX_M6PS_ALT  | SDR0_AMPX_M7PS_ALT )

#define PLB_ACR_VALUE  ( \
        PLB_ACR_PPM_FAIR | PLB_ACR_HBU_ENABLED | PLB_ACR_RDP_4DEEP |  \
        PLB_ACR_WRP_2DEEP \
        )
		
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

#define SDR0_CPU440_MASK (~(SDR0_CPU440_DRU_MASK | SDR0_CPU440_DRT_MASK | SDR0_CPU440_DRNC_MASK | SDR0_CPU440_DRLC_MASK | \
		SDR0_CPU440_DWF_MASK | SDR0_CPU440_DWS_MASK | SDR0_CPU440_DWU_MASK  | SDR0_CPU440_IRF_MASK  | \
		SDR0_CPU440_IRT_MASK  | SDR0_CPU440_IRS_MASK))
		
#define SDR0_CPU440_VALUE (SDR0_CPU440_DRU_0 | SDR0_CPU440_DRT_0 | SDR0_CPU440_DRNC_0 | SDR0_CPU440_DRLC_0 | SDR0_CPU440_DWF_0 | \
	       SDR0_CPU440_DWS_0 | SDR0_CPU440_DWU_0 | SDR0_CPU440_IRF_0  | SDR0_CPU440_IRT_0  | SDR0_CPU440_IRS_0)

/*
 * CCR0 register initialization (for resetEntry)
 *
 * a. Disable store gathering
 * b. Enable broadcast of instructions to auxilary processors
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
	FUNC_EXPORT(romPvrGet)	/* get PVR SPR value */
	FUNC_EXPORT(romCprDcrGet) /* get CPR DCR value */
	FUNC_EXPORT(romDcrGet) /* get DCR value */
	FUNC_EXPORT(romDcrSet) /* set DCR value */
	FUNC_EXPORT(out8) /* get SDR value */
	FUNC_EXPORT(in8) /* set SDR value */
	FUNC_EXPORT(romSdrGet) /* get SDR value */
	FUNC_EXPORT(romSdrSet) /* set SDR value */
	FUNC_EXPORT(mulDiv64)   /* multiply and divide w/64-bit intermediate */
	FUNC_EXPORT(div64)      /* divide 64-bit by 32-bit yielding 32-bit */

	/* externals */

	FUNC_IMPORT(romStart)	/* system initialization routine */

/*
 * SPR 79925.  The qualification on ROM_RESIDENT below permits a
 * rom-resident image type to be linked by the project facility. The
 * image will not work, because it does not initialize the SDRAM
 * controller.  For this reason, rom resident images are not supported.
 */

#ifndef ROM_RESIDENT
	FUNC_IMPORT(romI2cInit)		/* i2c controller initialization */
	FUNC_IMPORT(romSdramInit) 	/* DDR SDRAM controller initialization */
	FUNC_IMPORT(pinSharingInit) 	/* Chip and Board pin sharing and wiring initialization */
#endif /* ROM_RESIDENT */

	_WRS_TEXT_SEG_START

/*******************************************************************************
*
* romInit - entry point for VxWorks in ROM
*
* Entry point for VxWorks in ROM
*
* 
* RETURNS: N/A
*
* ERRNO
*/

FUNC_BEGIN(romInit)		/* used by romStart() */
FUNC_LABEL(_romInit)		/* used by linker */
	/*
	 * branch to resetEntry, which will initialize the tlb and
	 * return at cold
	 */
	
	bl	resetEntry

FUNC_LABEL(romWarm)

	bl	warm

	/* copyright notice appears at beginning of ROM (in TEXT segment) */

	.ascii	 "Copyright 2006 Wind River Systems, Inc."
	.align 4

cold:

	/* resetEntry transfers execution here once TLB is initialized */

	li	r2, BOOT_COLD
	bl	start			/* skip over next instruction */

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
        mtdcr   UIC1_ER, r0             /* likewise for the second UIC   */
        mtdcr   DMA0_CR0, r0            /* stop DMA channels             */
        mtdcr   DMA0_CR1, r0
        mtdcr   DMA0_CR2, r0
        mtdcr   DMA0_CR3, r0
        mtdcr   DMA1_CR0, r0            
        mtdcr   DMA1_CR1, r0
        mtdcr   DMA1_CR2, r0
        mtdcr   DMA1_CR3, r0
        /*
         * Initalize registers that need to be cleared with 0xFFFFFFFF
         */

        mtdcr   DMA0_SR, r1             /* clear DMA status reg          */
        mtdcr   DMA1_SR, r1             /* clear DMA status reg          */        
        mtdcr   UIC0_SR, r1             /* clear all pending UIC ints    */
        mtdcr   UIC1_SR, r1             /* likewise for the second UIC   */

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

#ifndef ROM_RESIDENT	/* see note above about ROM_RESIDENT */
	
        /*
         * Tlb #3.  Boot Space - 0xF0000000 -> 0x0F0000000.  256MB
         * Attributes: W/X/R
         */

        li r1,3					
	TLB_WORD_SET(0, r1, r0, 0xf0000000 		/* EPN */ \
		     | TLB_V | _MMU_TLB_SZ_256M)
	TLB_WORD_SET(1, r1, r0, 0x0 | 0xf0000000)	/* RPN */
	TLB_WORD_SET(2, r1, r0, TLB_UW | TLB_UX | TLB_UR | TLB_SW | TLB_SX | TLB_SR)	
	isync
	
	/* Clear the data cache area */
        lis	r3,HI(ROM_STACK_ADRS-ROM_STACK_SIZE)
        ori	r3, r3, LO(ROM_STACK_ADRS-ROM_STACK_SIZE)
        lis	r4,HI(ROM_STACK_SIZE)
        ori	r4, r4, LO(ROM_STACK_SIZE)
        bl 	areaDcbz

	/*
	 * Initialize stack pointer to point in Data cache.
	 * It is used by C initialization code for IIC and SDRAM.
	 */

	lis	sp, HI(ROM_STACK_ADRS)
	ori	sp, sp, LO(ROM_STACK_ADRS)
        addi    sp, sp, -FRAMEBASESZ    /* get frame stack */
        /* bl      romI2cInit */ 
        bl      romSdramInit
	

      /* Restore initial setting for Boot space TLB */
      /*
       * Tlb #3.  Boot Space - 0xF0000000 -> 0x0F0000000.  256MB
       * Attributes: W/X/R
       */

	li r1,3	
	TLB_WORD_SET(0, r1, r0, 0xf0000000 		/* EPN */ \
		     | TLB_V | _MMU_TLB_SZ_256M)
	TLB_WORD_SET(1, r1, r0, 0x0 | 0xf0000000)	/* RPN */
	TLB_WORD_SET(2, r1, r0, _WRITETHROUGH | BOOT_SPACE_WRITE \
		     | TLB_UX |          TLB_UR | TLB_SX          | TLB_SR)
	
       isync

#endif /* ROM_RESIDENT */
        /*
         * Invalidate the entire both caches.
         */
        iccci  r0, r0
        dccci  r0, r0
skip:


	/*
	 * Clear the CPU reservation bit
	 */

	li	r0, 0
	lwarx	r3, r0, r0
	stwcx.	r3, r0, r0
	
#ifndef ROM_RESIDENT	/* see note above about ROM_RESIDENT */
        lis     sp, HIADJ(STACK_ADRS)
        addi    sp, sp, LO(STACK_ADRS)
        addi    sp, sp, -FRAMEBASESZ    /* get frame stack */

        /* init chip and board wiring sharing */
	bl	pinSharingInit
#endif /* ROM_RESIDENT */

	/* Initialize the stack pointer (r1) */

	lis	sp, HIADJ(STACK_ADRS)
	addi	sp, sp, LO(STACK_ADRS)

#if	FALSE				/* SDA not supported  */
	/* initialize r2 and r13 according to EABI standard */

	lis	r2, HIADJ(_SDA2_BASE_)
	addi	r2, r2, LO(_SDA2_BASE_)
	lis	r13, HIADJ(_SDA_BASE_)
	addi	r13, r13, LO(_SDA_BASE_)
#endif	/* FALSE */

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
* Init the Ext Bus Controller for external peripherals
*
* RETURNS: N/A
*
* ERRNO
*
***************************************************************************/
FUNC_BEGIN(ext_bus_cntlr_init)

        mflr    r7                     /* save link register                  */

       /*
        * before doing anything with EBC enable EBC addresses and CS to go out
        * by selecting correct configuration in GPIO registers
        */

        /* Select EBC adresses and CS from ALT1 source */
	lis     r4, HIADJ(GPIO0_OSL)
	addi    r4, r4, LO(GPIO0_OSL)
	lis	r5, HIADJ(OSL0_EBC_VAL)
	addi	r5, r5, LO(OSL0_EBC_VAL)	
        stw     r5, 0x0(r4)

        /* Disable GPIO three state and select ALT1 three state control */
        lis     r4, HIADJ(GPIO0_TSL)
	addi    r4, r4, LO(GPIO0_TSL)
	lis	r5, HIADJ(TSL0_EBC_VAL)
	addi	r5, r5, LO(TSL0_EBC_VAL)	
        stw     r5, 0x0(r4)

       /*
        * Init EBC Configuration and access parameter registers
        */

        addi    r4, 0, EBC0_CFG
        mtdcr   EBC0_CFGADDR, r4
        lis     r5, HIADJ(EBC_CFG_INIT_VALUE)
        addi    r5, r5, LO(EBC_CFG_INIT_VALUE)
        mfdcr   r4, EBC0_CFGDATA
        or      r4, r4, r5
        mtdcr   EBC0_CFGDATA, r4

        addi    r4, 0, EBC0_B0AP
	mtdcr   EBC0_CFGADDR, r4
	lis     r4, HI(EBC_B0AP_VALUE)
	ori     r4, r4, LO(EBC_B0AP_VALUE)
	mtdcr   EBC0_CFGDATA, r4

	addi    r4, 0, EBC0_B0CR
	mtdcr   EBC0_CFGADDR, r4
	lis     r4, HI(EBC_B0CR_VALUE)
	ori     r4, r4, LO(EBC_B0CR_VALUE)
        mtdcr   EBC0_CFGDATA, r4

        addi    r4, 0, EBC0_B1AP
	mtdcr   EBC0_CFGADDR, r4
	lis     r4, HI(EBC_B1AP_VALUE)
	ori     r4, r4, LO(EBC_B1AP_VALUE)
	mtdcr   EBC0_CFGDATA, r4

	addi    r4, 0, EBC0_B1CR
	mtdcr   EBC0_CFGADDR, r4
	lis     r4, HI(EBC_B1CR_VALUE)
	ori     r4, r4, LO(EBC_B1CR_VALUE)
        mtdcr   EBC0_CFGDATA, r4

        addi    r4, 0, EBC0_B2AP
	mtdcr   EBC0_CFGADDR, r4
	lis     r4, HI(EBC_B2AP_VALUE)
	ori     r4, r4, LO(EBC_B2AP_VALUE)
	mtdcr   EBC0_CFGDATA, r4

	addi    r4, 0, EBC0_B2CR
	mtdcr   EBC0_CFGADDR, r4
	lis     r4, HI(EBC_B2CR_VALUE)
	ori     r4, r4, LO(EBC_B2CR_VALUE)
        mtdcr   EBC0_CFGDATA, r4

        addi    r4, 0, EBC0_B4AP
	mtdcr   EBC0_CFGADDR, r4
	lis     r4, HI(EBC_B4AP_VALUE)
	ori     r4, r4, LO(EBC_B4AP_VALUE)
	mtdcr   EBC0_CFGDATA, r4

	addi    r4, 0, EBC0_B4CR
	mtdcr   EBC0_CFGADDR, r4
	lis     r4, HI(EBC_B4CR_VALUE)
	ori     r4, r4, LO(EBC_B4CR_VALUE)
        mtdcr   EBC0_CFGDATA, r4

        addi    r4, 0, EBC0_B5AP
	mtdcr   EBC0_CFGADDR, r4
	lis     r4, HI(EBC_B5AP_VALUE)
	ori     r4, r4, LO(EBC_B5AP_VALUE)
	mtdcr   EBC0_CFGDATA, r4

	addi    r4, 0, EBC0_B5CR
	mtdcr   EBC0_CFGADDR, r4
	lis     r4, HI(EBC_B5CR_VALUE)
	ori     r4, r4, LO(EBC_B5CR_VALUE)
        mtdcr   EBC0_CFGDATA, r4

        mtlr    r7                     /* restore link register value         */
        blr
FUNC_END(ext_bus_cntlr_init)


/***************************************************************************
*
* priority_init - Init the PLB Priorities
*
* Init the PLB Priorities
*
* RETURNS: N/A
*
* ERRNO
***************************************************************************/

FUNC_BEGIN(priority_init)

	/* Set up alternate PLB3 master priority settings */
	
        addi    r6, 0, SDR0_AMP1
	mtdcr   SDR0_CFGADDR, r6
	mfdcr	r5, SDR0_CFGDATA
	lis	r6, HIADJ(SDR0_AMPX_MASK)
	addi	r6, r6, LO(SDR0_AMPX_MASK)
	and	r5, r5, r6
	lis	r6, HIADJ(SDR0_AMPX_VALUE)
	addi	r6, r6, LO(SDR0_AMPX_VALUE)
	or	r5, r5, r6
	mtdcr	SDR0_CFGDATA, r5
	/* Set the PLB3 priority mode to fair */

	mfdcr	r5, PLB3_ACR
	lis	r6, HIADJ(~PLB_ACR_PPM_MASK)
	addi	r6, r6, LO(~PLB_ACR_PPM_MASK)
	and	r5, r5, r6
	lis	r6, HIADJ(PLB_ACR_PPM_FAIR)
	addi	r6, r6, LO(PLB_ACR_PPM_FAIR)
	or	r5, r5, r6
	mtdcr	PLB3_ACR, r5
	

	/* Set up alternate PLB4 master priority settings */

        addi    r6, 0, SDR0_AMP0
	mtdcr   SDR0_CFGADDR, r6
	mfdcr	r5, SDR0_CFGDATA
	lis	r6, HIADJ(SDR0_AMPX_MASK)
	addi	r6, r6, LO(SDR0_AMPX_MASK)
	and	r5, r5, r6
	lis	r6, HIADJ(SDR0_AMPX_VALUE)
	addi	r6, r6, LO(SDR0_AMPX_VALUE)
	or	r5, r5, r6
	mtdcr	SDR0_CFGDATA, r5
	
        /* Set the PLB4 priority mode to fair */
	lis	r6, HIADJ(PLB_ACR_VALUE)
	addi	r6, r6, LO(PLB_ACR_VALUE)
	mtdcr	PLB0_ACR, r6
	mtdcr	PLB1_ACR, r6

/*
 * Set up PLB4 master priority settings
 */

        /* Set Processor core priority */


        addi    r6, 0, SDR0_CPU440
	mtdcr   SDR0_CFGADDR, r6
        mfdcr   r5, SDR0_CFGDATA
        lis     r6, HIADJ(SDR0_CPU440_MASK)
        addi    r6, r6, LO(SDR0_CPU440_MASK)
        and     r5, r5, r6
        lis     r6, HIADJ(SDR0_CPU440_VALUE)
        addi    r6, r6, LO(SDR0_CPU440_VALUE)
        or      r5, r5, r6
        mtdcr   SDR0_CFGDATA, r5

        /* Set DMA to PLB4 memory accesses to a specific priority */

        mfdcr   r5, DMA1_CR0
        lis     r6, HIADJ(~DMA1_CR_CP)
        addi    r6, r6, LO(~DMA1_CR_CP)
        and     r5, r5, r6
        lis     r6, HIADJ(DMA1_CR_CP_0)
        addi    r6, r6, LO(DMA1_CR_CP_0)
        or      r5, r5, r6
        mtdcr   DMA1_CR0, r5

        mfdcr   r5, DMA1_CR1
        lis     r6, HIADJ(~DMA1_CR_CP)
        addi    r6, r6, LO(~DMA1_CR_CP)
        and     r5, r5, r6
        lis     r6, HIADJ(DMA1_CR_CP_0)
        addi    r6, r6, LO(DMA1_CR_CP_0)
        or      r5, r5, r6
        mtdcr   DMA1_CR1, r5

        mfdcr   r5, DMA1_CR2
        lis     r6, HIADJ(~DMA1_CR_CP)
        addi    r6, r6, LO(~DMA1_CR_CP)
        and     r5, r5, r6
        lis     r6, HIADJ(DMA1_CR_CP_0)
        addi    r6, r6, LO(DMA1_CR_CP_0)
        or      r5, r5, r6
        mtdcr   DMA1_CR2, r5

        mfdcr   r5, DMA1_CR3
        lis     r6, HIADJ(~DMA1_CR_CP)
        addi    r6, r6, LO(~DMA1_CR_CP)
        and     r5, r5, r6
        lis     r6, HIADJ(DMA1_CR_CP_0)
        addi    r6, r6, LO(DMA1_CR_CP_0)
        or      r5, r5, r6
        mtdcr   DMA1_CR3, r5


        /* Set PLB3 to PLB4 Bridge priority */
        mfdcr   r5, P3P4_CONFG
        lis     r6, HIADJ(~P3P4_CONFG_PRI_MASK)
        addi    r6, r6, LO(~P3P4_CONFG_PRI_MASK)
        and     r5, r5, r6
        lis     r6, HIADJ(P3P4_CONFG_PRI_0)
        addi    r6, r6, LO(P3P4_CONFG_PRI_0)
        or      r5, r5, r6
        mtdcr   P3P4_CONFG, r5
/*
 * Set up PLB3 master priority settings
 */


        /* Peripheral component interconnect bridge controller */

        /* Write in PCI_CFGADDR to access PCI_CFG_BOPT1PK in PCI bridge */
        lis     r7, HIADJ(PCI_CFGADDR)
	addi    r7, r7, LO(PCI_CFGADDR)
	lis     r6, HIADJ(PCI_CFG_BOPT1PK)
	addi    r6, r6, LO(PCI_CFG_BOPT1PK)
        stw     r6, 0x0(r7)

        /* Read the PCI_CFG_BRDGOPT1 contents and mask it with desired value  */
        /* read in PCI0_CFGDATA the corresponding value */
        lis     r7, HIADJ(PCI_CFGDATA)
	addi    r7, r7, LO(PCI_CFGDATA)
	msync
        lwzu    r5, 0x0(r7)
        li      r6, ~BRDGOPT1_PLREQ
        and     r5, r5, r6
        li      r6, BRDGOPT1_PLREQ_0
        or      r5, r5, r6

        /* Write in PCI_CFGADDR to access PCI_CFG_BRDGOPT1 */
        lis     r7, HIADJ(PCI_CFGADDR)
	addi    r7, r7, LO(PCI_CFGADDR)
	lis     r6, HIADJ(PCI_CFG_BOPT1PK)
	addi    r6, r6, LO(PCI_CFG_BOPT1PK)
        stw     r6, 0x0(r7)

        /* Write needed value in the PCI_CFGDATA reg to indirectly write to PCI_CFG_BRDGOPT1 */
        lis     r7, HIADJ(PCI_CFGDATA)
        addi    r7, r7, LO(PCI_CFGDATA)
        msync
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

        /* Set PLB3 DMA memory accesses to a specific priority */

        mfdcr   r5, DMA0_CR0
        lis     r6, HIADJ(~DMA0_CR_CP)
        addi    r6, r6, LO(~DMA0_CR_CP)
        and     r5, r5, r6
        lis     r6, HIADJ(DMA0_CR_CP_LOW)
        addi    r6, r6, LO(DMA0_CR_CP_LOW)
        or      r5, r5, r6
        mtdcr   DMA0_CR0, r5

        mfdcr   r5, DMA0_CR1
        lis     r6, HIADJ(~DMA0_CR_CP)
        addi    r6, r6, LO(~DMA0_CR_CP)
        and     r5, r5, r6
        lis     r6, HIADJ(DMA0_CR_CP_LOW)
        addi    r6, r6, LO(DMA0_CR_CP_LOW)
        or      r5, r5, r6
        mtdcr   DMA0_CR1, r5

        mfdcr   r5, DMA0_CR2
        lis     r6, HIADJ(~DMA0_CR_CP)
        addi    r6, r6, LO(~DMA0_CR_CP)
        and     r5, r5, r6
        lis     r6, HIADJ(DMA0_CR_CP_LOW)
        addi    r6, r6, LO(DMA0_CR_CP_LOW)
        or      r5, r5, r6
        mtdcr   DMA0_CR2, r5

        mfdcr   r5, DMA0_CR3
        lis     r6, HIADJ(~DMA0_CR_CP)
        addi    r6, r6, LO(~DMA0_CR_CP)
        and     r5, r5, r6
        lis     r6, HIADJ(DMA0_CR_CP_LOW)
        addi    r6, r6, LO(DMA0_CR_CP_LOW)
        or      r5, r5, r6
        mtdcr   DMA0_CR3, r5

        /* OPB to PLB3 bridge controller */

        mfdcr   r5, OPB3_CTRL
        lis     r6, HIADJ(~OPB_PRI_MASK)
        addi    r6, r6, LO(~OPB_PRI_MASK)
        and     r5, r5, r6
        lis     r6, HIADJ(OPB_PRI_0)
        addi    r6, r6, LO(OPB_PRI_0)
        or      r5, r5, r6
        mtdcr   OPB3_CTRL, r5


        blr
FUNC_END(priority_init)

/***************************************************************************
*
* sdramDcrGet - retrieve the contents of an indirect SDRAM0 register
*
* UINT32 sdramDcrGet
*	(
*	int registerAdrs		/@ sdram register address @/
*	)
*
*
* RETURNS: UINT32
*
* ERRNO
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
*
* void sdramDcrSet
*	(
*	int registerAdrs,		/@ sdram register address @/
*	UINT32 data			/@ data to store in register @/
*	)
*
* RETURNS: N/A
*
* ERRNO
*/
FUNC_BEGIN(sdramDcrSet)
	mtdcr	SDRAM0_CFGADDR, r3
	mtdcr	SDRAM0_CFGDATA, r4
	blr
FUNC_END(sdramDcrSet)

/***************************************************************************
*
* sdramResetDelay - Delay to ensure 200usec have elapsed since reset.
*
* Assume worst case that the core is running 533Mhz:
*   533,333,333 (cycles/sec) X .000200 (sec) = 106667 cycles
*   = 1A0AB
*
* RETURNS: N/A
*

* void sdramResetDelay(void)

*/

FUNC_BEGIN(sdramResetDelay)
        addis   r3,0,0x0001
        ori     r3,r3,0xA0AB        /* ensure 200usec have passed since reset */
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
*
* UINT32 *sdramTestPatterns(void)
*
* RETURNS: UINT32 *
*
* ERRNO
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
*
* void sdramDcbf
*    (
*    void * blockAdrs			/@ Address to flush from D-cache @/
*    )
*
* RETURNS: N/A
*
* ERRNO
*/
FUNC_BEGIN(sdramDcbf)
	dcbf	0, r3
	blr
FUNC_END(sdramDcbf)


/***************************************************************************
*
* areaDcbz - perform area data cache block clear operation
*
* void areaDcbf
*    (
*    void * blockAdrs			/@ area start Address to flush from D-cache @/
*    long   size                        /@ area size
*    )
*
* RETURNS: N/A
* ERRNO
*/
FUNC_BEGIN(areaDcbz)
	rlwinm.	r5,r4,0,27,31
	rlwinm	r5,r4,27,5,31
	beq	d_ran
	addi	r5,r5,0x0001
d_ran:	mtctr	r5
d_ag: 	dcbz	r0,r3
	addi	r3,r3,32
	bdnz	d_ag
	blr
FUNC_END(areaDcbz)


/***************************************************************************
*
* sdramDflush - perform entire data cache flush operation
*
* void sdramDflush(void)
*
* RETURNS: N/A
*
* ERRNO
*/
FUNC_BEGIN(sdramDflush)
	lis	r3, HIADJ(ROM_TEXT_ADRS)	/* r3 = cached boot device */
	addi	r3, r3, LO(ROM_TEXT_ADRS)

	mfspr	r6, CTR
	msync

	lis	r4, HIADJ(_NUM_DCACHE_LINES)
	addi	r4, r4, LO(_NUM_DCACHE_LINES)
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
* romPvrGet - get Processor Version Register contents
*
* UINT32 romPvrGet(void)
*
* RETURNS: UINT32
* 
* ERRNO
*/

FUNC_BEGIN(romPvrGet)
	mfspr	r3, PVR
	blr
FUNC_END(romPvrGet)


/***************************************************************************
*
* romCprDcrGet - retrieve the contents of an indirect CPR register
*
* RETURNS: UINT32     /@ CPR register data @/
*

* UINT32 romCprDcrGet
* (
*	int registerAdrs	/@ CPR register address @/
* )

*/
FUNC_BEGIN(romCprDcrGet)
        mtdcr	CPR0_CFGADDR, r3
	mfdcr	r3, CPR0_CFGDATA
	blr
FUNC_END(romCprDcrGet)

/***************************************************************************
*
* romSdrGet - retrieve the contents of an indirect SDR register
*
* RETURNS: UINT32     /@ SDR register data @/
*

* UINT32 romSdrGet
* (
*	int registerAdrs	/@ CPR register address @/
* )

*/
FUNC_BEGIN(romSdrGet)
        mtdcr	SDR0_CFGADDR, r3
	mfdcr	r3, SDR0_CFGDATA
	blr
FUNC_END(romSdrGet)

/***************************************************************************
*
* romSdrSet - store a value in an indirect SDR register
*
* RETURNS: N/A
*

* void romSdrSet
*	(
*	int registerAdrs,		/@ sdr register address @/
*	UINT32 data			/@ data to store in register @/
*	)

*/
FUNC_BEGIN(romSdrSet)
	mtdcr	SDR0_CFGADDR, r3
	mtdcr	SDR0_CFGDATA, r4
	blr
FUNC_END(romSdrSet)

FUNC_BEGIN(out8)
	stb	r4, 0x0000(r3)
	blr
FUNC_END(out8)

FUNC_BEGIN(in8)
	lbz     r3, 0x0000(r3)
	blr
FUNC_END(in8)

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

/***************************************************************************
*
* mulDiv64 - multiply two 32-bit values to 64-bit, divide by a 32-bit value
*
* Two 32-bit values are multiplied into a 64-bit intermediate value,
* then divided by a third 32-bit value to yield a 32-bit result.
*
* Author: Daniel Goehring
*
* RETURNS: UINT32
*

* UINT32 mulDiv64
*	(
*	UINT32	numerator0,	/@ numerator 0 @/
*	UINT32	numerator1,	/@ numerator 1 @/
*	UINT32	denominator	/@ denominator @/
*	);

*/
FUNC_BEGIN(mulDiv64)
        mullw   r6, r3, r4      /* R6 = prod[32:63]  low word */
        mulhw   r7, r3, r4      /* R7 = prod[00:31]  high word */

	/*
	 * finish with 64-bit divide, below
	 */

	mr	r3, r7
	mr	r4, r6
	b	div64		/* finish with 64-bit divide, below */
FUNC_END(mulDiv64)


/***************************************************************************
*
* div64 - divide a 64-bit value by a 32-bit value
*
* A 64-bit unsigned value is passed in with two 32-bit parameters, then
* divided by a 32 value, yielding a 32 bit result.
*
* Author: Daniel Goehring
*
* RETURNS: UINT32
*

* UINT32 div64
*	(
*	UINT32	numeratorHi,	/@ numerator most significant word @/
*	UINT32	numeratorLo,	/@ numerator least signficant word @/
*	UINT32	denominator	/@ denominator @/
*	);

*/
FUNC_BEGIN(div64)
        mr     r6, r4   	/* R6 = prod[32:63]  low word */
        mr     r7, r3   	/* R7 = prod[00:31]  high word */

	/* Now, r6 = r4 = low word, r7 = r3 = hi word */

        mfspr   r8, CTR
        addi    r3, r0, 0x20    	/* Loop 32 times. */
        mtspr   CTR, r3
        xor     r3, r3, r3      	/* R3 = result of the divide */
        xor     r4, r4, r4      	/* R4 = 0 */

dLoop:
        rlwinm  r3, r3, 0x1, 0, 30	/* Shift left 1 bit */
        subf    r7, r4, r7
        rlwinm  r7, r7, 0x1, 0, 30	/* Shift left 1 bit */
        rlwinm  r4, r6, 0x1, 31, 31	/* Grab the lower bit  */
        rlwinm  r6, r6, 0x1, 0, 30	/* Shift left 1 bit */
        or      r7, r7, r4		/* Or the two together */
        divw    r4, r7, r5
        add     r3, r3, r4
        mullw   r4, r4, r5
        bdnz    dLoop

        mtspr   CTR, r8
        blr
FUNC_END(div64)


/***************************************************************************
*
* The following section is based from the Ebony OpenBios 1.0.8:
*
***************************************************************************/

/* -------------------------------------------------------------------------
*
*  mainLib/entry1.s, openbios_ebony, ebony_bios 8/9/00 19:51:30
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
*  File Name:	entry1.s
*
*  Function:	Entry code for the 440 ROM Monitor (OpenBIOS) running
*		on the Ebony board.
*
*  Author:	Daniel Goehring
*
*  Change Activity-
*
*  Date        Description of Change					BY
*  ---------   ---------------------					---
*  06-Nov-00   Created							DWG
*
* --------------------------------------------------------------------------
*/

#define NOCACHE


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
*	Since the PPC440EP only initializes an architecturally-invisible
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

        /* Set r0 = 0 */

        xor   r0, r0, r0          /* r0 = 0 */

        /* Clear SRR0, CSRR0, SRR1, CSRR1 */

        mtspr SRR0, r0
        mtspr SRR1, r0
        mtspr CSRR0, r0
        mtspr CSRR1, r0

#ifdef CLEAR_DEBUG_REGISTERS
        /*
         * Clear DBCR0 register (disable all debug events)
         * Reset all other debugging registers associated with DBCR0.
         * Although the PPC440EP is defined to reset some of the debug
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

        /* Clear DBSR register (initialize all debug event status) */

        addis r0,r0,0xCFFF
        ori   r0,r0,0xFFFF         /* r0 = 0xCFFFFFFF Keep the DBSR[MRR] bits */
        mtspr DBSR, r0
#endif /* CLEAR_DEBUG_REGISTERS */

	/* Initialize general load/store and cache operations */

        addis r0, r0, HI(CCR0_INIT)
        ori   r0, r0, LO(CCR0_INIT)
        mtspr CCR0, r0			/* Load/Store/General cache config */

        addis r0, r0, HI(CCR1_INIT)
        ori   r0, r0, LO(CCR1_INIT)
        mtspr CCR1, r0			/* Cache parity/Flush/Timebase config */

       /*
	* Reset the timebase.
        * The previous write to CCR1 sets the timebase source.
        */
        xor   r0, r0, r0
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
	 *     0x40000000 -> 0x000000000.  256MB
	 * TLB_NUM_SDRAM_0 must be coordinated with this setting
         * Attributes: I/W/R
         */

	TLB_NEXT(r1)					/* next TLB */
	TLB_WORD_SET(0, r1, r0, 0x40000000 		/* EPN */ \
		     | TLB_V | _MMU_TLB_SZ_256M)
	TLB_WORD_SET(1, r1, r0, 0x0 | 0x00000000)	/* RPN */
	TLB_WORD_SET(2, r1, r0, _INHIBIT \
		     |          TLB_UW | TLB_UR |          TLB_SW | TLB_SR)
#endif /* INCLUDE_440X5_TLB_RECOVERY_MAX */

        /*
         * Tlb #2.  Peripherals - 0xC0600000 -> 0x0EF600000.  16KB
         * Attributes: I/G/W/R
         */

	TLB_NEXT(r1)					/* next TLB */
	TLB_WORD_SET(0, r1, r0,  0xC0600000		/* EPN */ \
		     | TLB_V | _MMU_TLB_SZ_16K)
	TLB_WORD_SET(1, r1, r0, 0x0 | 0xEF600000)	/* RPN */
	TLB_WORD_SET(2, r1, r0, _INHIBIT_AND_G \
		     |          TLB_UW | TLB_UR |          TLB_SW | TLB_SR)

        /*
         * Tlb #3.  Boot Space - 0xF0000000 -> 0x0F0000000.  256MB
         * Attributes: W/X/R
         */

	TLB_NEXT(r1)					/* next TLB */
	TLB_WORD_SET(0, r1, r0, 0xf0000000 		/* EPN */ \
		     | TLB_V | _MMU_TLB_SZ_256M)
	TLB_WORD_SET(1, r1, r0, 0x0 | 0xf0000000)	/* RPN */
	TLB_WORD_SET(2, r1, r0, _WRITETHROUGH | BOOT_SPACE_WRITE \
		     | TLB_UX |          TLB_UR | TLB_SX          | TLB_SR)

        /*
         * Tlb #4.  External Peripherals part1 - 0xD0000000 -> 0x080000000.  256M
         * Attributes: I/G/W/R
         */

	TLB_NEXT(r1)					/* next TLB */
	TLB_WORD_SET(0, r1, r0, 0xD0000000 		/* EPN */ \
		     | TLB_V | _MMU_TLB_SZ_256M)
	TLB_WORD_SET(1, r1, r0, 0x0 | 0x80000000)	/* RPN */
	TLB_WORD_SET(2, r1, r0, _INHIBIT_AND_G \
		     |          TLB_UW | TLB_UR |          TLB_SW | TLB_SR)

        /*
         * Tlb #5.  PCI Memory - 0x80000000 -> 0x0A0000000.  256MB
         * Attributes: I/G/X/W/R
         * Used for PCI memory that does not allow prefetching.
         */

	TLB_NEXT(r1)					/* next TLB */
	TLB_WORD_SET(0, r1, r0, 0x80000000		/* EPN */ \
		     | TLB_V | _MMU_TLB_SZ_256M)
	TLB_WORD_SET(1, r1, r0, 0x0 | 0xA0000000)	/* RPN */
	TLB_WORD_SET(2, r1, r0, _INHIBIT_AND_G \
		     | TLB_UX | TLB_UW | TLB_UR | TLB_SX | TLB_SW | TLB_SR)

        /*
         * Tlb #6.  PCI Memory - 0x90000000 -> 0x0B0000000.  256MB
         * Attributes: X/W/R
         */

	TLB_NEXT(r1)					/* next TLB */
       TLB_WORD_SET(0, r1, r0, 0x90000000		/* EPN */ \
		     | TLB_V | _MMU_TLB_SZ_256M)
	TLB_WORD_SET(1, r1, r0, 0x0 | 0xB0000000)	/* RPN */
	TLB_WORD_SET(2, r1, r0, _INHIBIT \
		     | TLB_UX | TLB_UW | TLB_UR | TLB_SX | TLB_SW | TLB_SR)
		
         /*
          * Tlb #7.  PCI Memory - 0xA0000000 -> 0x0C0000000.  256MB
          * Attributes: I/G/X/W/R
          * Used for PCI memory that does not allow prefetching.
          */

 	TLB_NEXT(r1)					/* next TLB */
 	TLB_WORD_SET(0, r1, r0, 0xA0000000		/* EPN */ \
 		     | TLB_V | _MMU_TLB_SZ_256M)
 	TLB_WORD_SET(1, r1, r0, 0x0 | 0xC0000000)	/* RPN */
 	TLB_WORD_SET(2, r1, r0, _INHIBIT_AND_G \
		     | TLB_UX | TLB_UW | TLB_UR | TLB_SX | TLB_SW | TLB_SR)
		
         /*
          * Tlb #8.  PCI Memory - 0xB0000000 -> 0x0D0000000.  256MB
          * Attributes: I/G/X/W/R
          * Used for PCI memory that does not allow prefetching.
          */

 	TLB_NEXT(r1)					/* next TLB */
 	TLB_WORD_SET(0, r1, r0, 0xB0000000		/* EPN */ \
 		     | TLB_V | _MMU_TLB_SZ_256M)
 	TLB_WORD_SET(1, r1, r0, 0x0 | 0xD0000000)	/* RPN */
 	TLB_WORD_SET(2, r1, r0, _INHIBIT_AND_G \
		     | TLB_UX | TLB_UW | TLB_UR | TLB_SX | TLB_SW | TLB_SR)		

        /*
         * Tlb #9.  USB Peripherals - 0xC9000000 -> 0x050000000.  4KB
         * Attributes: I/G/W/R
         */

	TLB_NEXT(r1)					/* next TLB */
	TLB_WORD_SET(0, r1, r0, 0xC9000000 		/* EPN */ \
		     | TLB_V | _MMU_TLB_SZ_4K)
	TLB_WORD_SET(1, r1, r0, 0x0 | 0x50000000)	/* RPN */
	TLB_WORD_SET(2, r1, r0, _INHIBIT_AND_G \
		     |          TLB_UW | TLB_UR |          TLB_SW | TLB_SR)
		
        /*
         * Tlb #10  PCI I/O - 0xC1000000 -> 0x0E8000000.  64KB
         * Attributes: I/G/W/R
         */

	TLB_NEXT(r1)					/* next TLB */
	TLB_WORD_SET(0, r1, r0, 0xC1000000 		/* EPN */ \
		     | TLB_V | _MMU_TLB_SZ_64K)
	TLB_WORD_SET(1, r1, r0, 0x0 | 0xE8000000)	/* RPN */
	TLB_WORD_SET(2, r1, r0, _INHIBIT_AND_G \
		     |          TLB_UW | TLB_UR |          TLB_SW | TLB_SR)

        /*
         * Tlb #11  PCI I/O - 0xC2800000 -> 0x0E8800000.  16MB
         * Attributes: I/G/W/R
         */

	TLB_NEXT(r1)					/* next TLB */
	TLB_WORD_SET(0, r1, r0, 0xC2800000 		/* EPN */ \
		     | TLB_V | _MMU_TLB_SZ_16M)
	TLB_WORD_SET(1, r1, r0, 0x0 | 0xE8800000)	/* RPN */
	TLB_WORD_SET(2, r1, r0, _INHIBIT_AND_G \
		     |          TLB_UW | TLB_UR |          TLB_SW | TLB_SR)

        /*
         * Tlb #12  PCI I/O - 0xC3800000 -> 0x0E9800000.  16MB
         * Attributes: I/G/W/R
         */

	TLB_NEXT(r1)					/* next TLB */
	TLB_WORD_SET(0, r1, r0, 0xC3800000 		/* EPN */ \
		     | TLB_V | _MMU_TLB_SZ_16M)
	TLB_WORD_SET(1, r1, r0, 0x0 | 0xE9800000)	/* RPN */
	TLB_WORD_SET(2, r1, r0, _INHIBIT_AND_G \
		     |          TLB_UW | TLB_UR |          TLB_SW | TLB_SR)


        /*
         * Tlb #13  PCI I/O - 0xC4800000 -> 0x0EA800000.  16MB
         * Attributes: I/G/W/R
         */

	TLB_NEXT(r1)					/* next TLB */
	TLB_WORD_SET(0, r1, r0, 0xC4800000 		/* EPN */ \
		     | TLB_V | _MMU_TLB_SZ_16M)
	TLB_WORD_SET(1, r1, r0, 0x0 | 0xEA800000)	/* RPN */
	TLB_WORD_SET(2, r1, r0, _INHIBIT_AND_G \
		     |          TLB_UW | TLB_UR |          TLB_SW | TLB_SR)


        /*
         * Tlb #14  PCI I/O - 0xC5800000 -> 0x0EB800000.  16MB
         * Attributes: I/G/W/R
         */

	TLB_NEXT(r1)					/* next TLB */
	TLB_WORD_SET(0, r1, r0, 0xC5800000 		/* EPN */ \
		     | TLB_V | _MMU_TLB_SZ_16M)
	TLB_WORD_SET(1, r1, r0, 0x0 | 0xEB800000)	/* RPN */
	TLB_WORD_SET(2, r1, r0, _INHIBIT_AND_G \
		     |          TLB_UW | TLB_UR |          TLB_SW | TLB_SR)


        /*
         * Tlb #15  PCI Config Regs - 0xC6C00000 -> 0x0EEC00000.  4KB
         * Attributes: I/G/W/R
         */

	TLB_NEXT(r1)					/* next TLB */
	TLB_WORD_SET(0, r1, r0, 0xC6C00000 		/* EPN */ \
		     | TLB_V | _MMU_TLB_SZ_4K)
	TLB_WORD_SET(1, r1, r0, 0x0 | 0xEEC00000)	/* RPN */
	TLB_WORD_SET(2, r1, r0, _INHIBIT_AND_G \
		     |          TLB_UW | TLB_UR |          TLB_SW | TLB_SR)


        /*
         * Tlb #16  PCI Interrupt Ack/special cycle - 0xC7D00000 -> 0x0EED00000.  4KB
         * Attributes: I/G/W/R
         */

	TLB_NEXT(r1)					/* next TLB */
	TLB_WORD_SET(0, r1, r0, 0xC7D00000 		/* EPN */ \
		     | TLB_V | _MMU_TLB_SZ_4K)
	TLB_WORD_SET(1, r1, r0, 0x0 | 0xEED00000)	/* RPN */
	TLB_WORD_SET(2, r1, r0, _INHIBIT_AND_G \
		     |          TLB_UW | TLB_UR |          TLB_SW | TLB_SR)


        /*
         * Tlb #17  PCI Local Config Regs - 0xC8400000 -> 0x0EF400000.  4KB
         * Attributes: I/G/W/R
         */

	TLB_NEXT(r1)					/* next TLB */
	TLB_WORD_SET(0, r1, r0, 0xC8400000 		/* EPN */ \
		     | TLB_V | _MMU_TLB_SZ_4K)
	TLB_WORD_SET(1, r1, r0, 0x0 | 0xEF400000)	/* RPN */
	TLB_WORD_SET(2, r1, r0, _INHIBIT_AND_G \
		     |          TLB_UW | TLB_UR |          TLB_SW | TLB_SR)



#ifdef INCLUDE_440X5_TLB_RECOVERY_MAX
        /*
         * Tlb #18.  DDR SDRAM 0 - Uncached
	 * For use when INCLUDE_440X5_TLB_RECOVERY_MAX is configured
	 *     0x40000000 -> 0x000000000.  256MB
         * Attributes: I/W/R
         */

	TLB_NEXT(r1)					/* next TLB */
	TLB_WORD_SET(0, r1, r0, 0x40000000 		/* EPN */ \
		     | TLB_V | _MMU_TLB_SZ_256M)
	TLB_WORD_SET(1, r1, r0, 0x0 | 0x00000000)	/* RPN */
	TLB_WORD_SET(2, r1, r0, _INHIBIT_AND_G \
		     | TLB_UX | TLB_UW | TLB_UR | TLB_SX | TLB_SW | TLB_SR)

        /*
         * Tlb #19.  DDR SDRAM 0 - Cached
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

        subfic r0, r1, (N_TLB_ENTRIES-1)    /* N_TLB_ENTRIES - 1 - Num tlb entries already set up. */

        mtspr CTR, r0         /* Set the counter for the branch */
        addis r0, r0, 0x0000 /* EPN = 0x00000000, Not Valid. */
TLB_invalidate_loop:
	addi  r1, r1, 0x0001	/* Increment to the next TLB entry */
	tlbwe r0, r1, 0x0000	/* Write TLB entry word 0 */
	tlbwe r0, r1, 0x0001	/* Write TLB entry word 1 */
	tlbwe r0, r1, 0x0002 	/* Write TLB entry word 2 */
	bdnz  TLB_invalidate_loop


	/*
	 * Initialize PID reg to match TID field of TLB entry (unless
	 * using TID=0)
	 */

	xor   r0, r0, r0
	mtspr PID, r0

	/*
	 * Setup for subsequent change in instruction fetch address
	 */

	addis r0, r0, MSR_VALUE@h
	ori   r0, r0, MSR_VALUE@l
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
