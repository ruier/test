/* romInit.s - Motorola ads827x ROM initialization module */

/*
 * Copyright (c) 2003, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,23aug12,l_z  remove copyright_wind_river. (WIND00371949)
01a,23dec03,dtr  create from ads826x/romInit.s
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

#define	_ASMLANGUAGE
#include "vxWorks.h"
#include "asm.h"
#include "cacheLib.h"
#include "config.h"
#include "regs.h"	
#include "sysLib.h"
#include "drv/timer/m8260Clock.h"
#include "drv/mem/m8260Siu.h"
#include "drv/mem/m8260Memc.h"

	/* internals */

	FUNC_EXPORT(_romInit)		/* start of system code */
	FUNC_EXPORT(romInit)		/* start of system code */

	/* externals */

	FUNC_IMPORT(romStart)	/* system initialization routine */

/*
* A flash based Hard Reset Config Word can be used to boot the board by
* changing jumper JP3.  However the reset word must be located at the
* base of flash memory.  The directives below configure a hard reset
* config word at ROM_BASE_ADRS, which is usually 0xFFF00000.
*
* The following defines what the hard reset config word should look like,
* but the table is located at the wrong address.
* The following must be programmed into the first 32 bytes of flash
* to use it as a hard reset configuration word.
*/
	.text                       /* byte 0 (MSByte) of the configuration master's */
	.fill   1,1,HRCW_BYTE_0     /* Hard Reset Configuration Word */
	.fill   7,1,0
	.fill   1,1,HRCW_BYTE_1
	.fill   7,1,0
	.fill   1,1,HRCW_BYTE_2
	.fill   7,1,0
	.fill   1,1,HRCW_BYTE_3		/* This is the LSByte */
	.fill   231,1,0     		/* The rest of the space are filled with zeros */

	.align 2

/******************************************************************************
*
* romInit - entry point for VxWorks in ROM
*
*
* romInit
*		(
*		int startType	/@ only used by 2nd entry point @/
*		)

*/

FUNC_BEGIN(_romInit)
FUNC_BEGIN(romInit)

	b	  	cold		/* jump to the cold boot initialization */
	nop
	mr		r31, r3		/* save	passed parameter in r31 */
	b	  	warm		/* jump to the warm boot initialization */
						/* r3 should have BOOT_WARM value */

	/* copyright notice appears at beginning of ROM (in TEXT segment) */

	.ascii   "Copyright 1984-2003 Wind River Systems, Inc."
	.align 2

cold:
	li		r31, BOOT_COLD	/* set cold boot as start type */
							/* saved in r31 to later pass as */
							/* argument to romStart() */
warm:
	li		r1, BOOT_COLD	/* need to see if a warm or cold */
	xor.	r1, r1, r31		/* start to jump around IMMR stuff */
	bne		start 			/* jump if not BOOT_COLD */

	/*
	 * initialize the IMMR register before any non-core register
	 * modifications. The default IMMR base address was 0x0F000000,
	 * as originally programmed in the Hard Reset Configuration Word.
	 * ONLY IF A COLD BOOT
	 */

	/* put desired internal memory map base address in r4 */
	lis		r4, HIADJ (INTERNAL_MEM_MAP_ADDR)
	addi	r4, r4, LO (INTERNAL_MEM_MAP_ADDR)

	/* get the reset value of IMM base address */
	lis		r8, HIADJ (RST_INTERNAL_MEM_MAP_ADDR + PQII_REG_BASE_OFF)
	addi	r8, r8, LO (RST_INTERNAL_MEM_MAP_ADDR + PQII_REG_BASE_OFF)
	stw		r4, IMMR_OFFSET(r8)		/* IMMR now at bsp specified value */
	isync

	/* SYPCR - turn off the system protection stuff */

	lis		r5, HIADJ (M8260_SYPCR_SWTC | M8260_SYPCR_BMT | \
                       M8260_SYPCR_PBME | M8260_SYPCR_LBME | M8260_SYPCR_SWP)
	addi	r5, r5, LO (M8260_SYPCR_SWTC | M8260_SYPCR_BMT | \
                        M8260_SYPCR_PBME | M8260_SYPCR_LBME | M8260_SYPCR_SWP)
	lis 	r6, HIADJ (M8260_SYPCR (INTERNAL_MEM_MAP_ADDR))
	addi	r6, r6, LO (M8260_SYPCR (INTERNAL_MEM_MAP_ADDR))
	stw 	r5, 0(r6)

start:
	/* now that IMMR is set, use dual ported ram for a temp stack */
	lis		sp, HIADJ (INTERNAL_MEM_MAP_ADDR + DPRAM1_SIZE)
	addi	sp,	sp, LO (INTERNAL_MEM_MAP_ADDR + DPRAM1_SIZE)

	/*
	 * Map the bank 0 to the flash area - On the ADS board at reset time
	 * the bank 0 is already used to map the flash but has the base at
	 * 0xFE000000.  This is determined by the hard reset config word.
	 */

	/* load the base register 0 */
	lis		r5, HIADJ ((FLASH_BASE_ADRS & M8260_BR_BA_MSK) | \
					   M8260_BR_PS_32 | M8260_BR_V)
	addi	r5,	r5, LO ((FLASH_BASE_ADRS & M8260_BR_BA_MSK) | \
						M8260_BR_PS_32 | M8260_BR_V)
	lis 	r6, HIADJ (M8260_BR0 (INTERNAL_MEM_MAP_ADDR))
	addi	r6,	r6, LO (M8260_BR0 (INTERNAL_MEM_MAP_ADDR))

	/* load the option register 0 */
	stw 	r5, 0(r6)
	lis	r5, HIADJ(	(FLASH_BASE_ADRS & M8260_OR_AM_MSK) | \
				M8260_OR_EHTR_8 | M8260_OR_CSNT_EARLY | \
				M8260_OR_SCY_7_CLK)
	addi	r5,	r5, LO ((FLASH_BASE_ADRS & M8260_OR_AM_MSK) | \
				M8260_OR_EHTR_8 | M8260_OR_CSNT_EARLY | \
				M8260_OR_SCY_7_CLK)
	lis 	r6, HIADJ (M8260_OR0 (INTERNAL_MEM_MAP_ADDR))
	addi	r6,	r6, LO (M8260_OR0 (INTERNAL_MEM_MAP_ADDR))
	stw 	r5, 0 (r6)

	/* map the bank 1 to the BCSRs */

	lis 	r5, HIADJ (	(BCSRS_MASK & M8260_OR_AM_MSK) \
				| M8260_OR_SCY_1_CLK)
	addi	r5, r5, LO (	(BCSRS_MASK & M8260_OR_AM_MSK) \
				| M8260_OR_SCY_1_CLK)
	lis 	r6, HIADJ (M8260_OR1 (INTERNAL_MEM_MAP_ADDR))
	addi	r6,	r6, LO (M8260_OR1 (INTERNAL_MEM_MAP_ADDR))
	stw 	r5, 0(r6)

	/* write the proper value to the base register */

	lis 	r5, HIADJ (BCSR0 | M8260_BR_PS_32 | M8260_BR_V)
	addi	r5,	r5, LO (BCSR0 | M8260_BR_PS_32 | M8260_BR_V)
	lis 	r6, HIADJ (M8260_BR1 (INTERNAL_MEM_MAP_ADDR))
	addi	r6,	r6, LO (M8260_BR1 (INTERNAL_MEM_MAP_ADDR))
	stw 	r5, 0(r6)

	/*
	 * Initialize all necessary core registers before continuing
	 *
	 * 1. Clear MSR to disable all exceptions, no mmu, no fpu etc
	 * 2. Clear out the SPRGs
	 * 3. Clear out the Segment Registers
	 * 4. Clear Instruction BAT registers
	 * 5. Clear Data BAT registers
	 * 6. Initialize FPU registers
	 *
	 */
	bl		sysClearMSR
	bl		sysClearSPRGs
	bl		sysClearSRs
	bl		sysClearIBATs
	bl		sysClearDBATs
	bl		sysClearFPRegs


	/* program the SCCR: normal operation */

	lis 	r5, HIADJ (M8260_SCCR_DFBRG_16)
	addi	r5, r5, LO (M8260_SCCR_DFBRG_16)

	lis 	r6, HIADJ (M8260_SCCR (INTERNAL_MEM_MAP_ADDR))
	addi	r6, r6, LO (M8260_SCCR (INTERNAL_MEM_MAP_ADDR))
	stw 	r5, 0(r6)

#if DEBUG
	li		r3,1
	bl		flashRedLed
#endif
	/* program the BCR */

	lis     r5, 0x100c
	lis 	r6, HIADJ (M8260_BCR (INTERNAL_MEM_MAP_ADDR))
	addi	r6, r6, LO (M8260_BCR (INTERNAL_MEM_MAP_ADDR))
	stw 	r5, 0 (r6)

	/* program the PPC_ACR */

	addi	r5, 0, 0x02
	lis 	r6, HIADJ (M8260_PPC_ACR (INTERNAL_MEM_MAP_ADDR))
	addi	r6, r6, LO (M8260_PPC_ACR (INTERNAL_MEM_MAP_ADDR))
	stb 	r5, 0(r6)

	/* program the PPC_ALRH */
#ifdef INCLUDE_PCI
	addis   r5, 0, 0x3012
	ori		r5, r5, 0x6745
#else
	addis   r5, 0, 0x0126
	ori		r5, r5, 0x7893
#endif /*INCLUDE_PCI*/
	lis 	r6, HIADJ (M8260_PPC_ALRH (INTERNAL_MEM_MAP_ADDR))
	addi	r6, r6, LO (M8260_PPC_ALRH (INTERNAL_MEM_MAP_ADDR))
	stw 	r5, 0(r6)

	/* program the TESCR1 */

	addis   r5, 0, 0x0000
	ori	r5, r5, 0x4000
	lis 	r6, HIADJ (M8260_TESCR1 (INTERNAL_MEM_MAP_ADDR))	
	addi	r6, r6, LO (M8260_TESCR1 (INTERNAL_MEM_MAP_ADDR))
	stw 	r5, 0(r6)

	/* program the LTESCR1 */

	addis   r5, 0, 0x0000
	ori	r5, r5, 0x0000
	lis 	r6, HIADJ (M8260_LTESCR1 (INTERNAL_MEM_MAP_ADDR))	
	addi	r6, r6, LO (M8260_LTESCR1 (INTERNAL_MEM_MAP_ADDR))
	stw 	r5, 0(r6)
	
SdramInit:

	/* program the MPTPR */

	addi    r5,0,0x2800	 /* MPTPR[PTP] */
        lis     r6, HIADJ (M8260_MPTPR (INTERNAL_MEM_MAP_ADDR))
        addi    r6, r6, LO (M8260_MPTPR (INTERNAL_MEM_MAP_ADDR))
	sth     r5, 0x0 (r6)      /* store upper half-word */

	/* program the PSRT */

	addi    r5,0,0x13
        lis     r6, HIADJ (M8260_PSRT (INTERNAL_MEM_MAP_ADDR))
        addi    r6, r6, LO (M8260_PSRT (INTERNAL_MEM_MAP_ADDR))
	stb     r5, 0x0 (r6)      /* store byte - bits[24-31] */

	/* load OR2 */

        lis     r5, HIADJ (0xfc000000 | \
                           M8260_OR_SDRAM_BPD_4 | \
	                   M8260_OR_SDRAM_ROWST_9| \
	                   M8260_OR_SDRAM_NUMR_12 )
        addi    r5, r5, LO (0xfc000000 | \
                           M8260_OR_SDRAM_BPD_4 | \
	                   M8260_OR_SDRAM_ROWST_9| \
	                   M8260_OR_SDRAM_NUMR_12 )  
        lis     r6, HIADJ (M8260_OR2 (INTERNAL_MEM_MAP_ADDR))
        addi    r6, r6, LO (M8260_OR2 (INTERNAL_MEM_MAP_ADDR))
	stw	r5, 0 (r6)

	/* load BR2 */

	lis	r5, HIADJ (M8260_BR_MS_SDRAM_60X | M8260_BR_V) 
	addi	r5, r5, LO (M8260_BR_MS_SDRAM_60X | M8260_BR_V)
        lis     r6, HIADJ (M8260_BR2 (INTERNAL_MEM_MAP_ADDR))
        addi    r6, r6, LO (M8260_BR2 (INTERNAL_MEM_MAP_ADDR))
	stw	r5, 0 (r6)

	/* The following describes the bit settings of the PSDMR register */
	/* On board SDRAM supporting page base interleaving.
	 * Refresh services are off for now
	 * SDAM = b011
	 * BSMA selects A13-A15 as bank select lines 
	 * A7 is selected as control pin for SDA10 on SDRAM (PBI)
	 * 8-clock refresh recovery time
	 * precharge-to-activate interval is 3-clock time
	 * activate-to-read/write interval is 3-clock time
	 * Burst lenght is 4
	 * last data out to precharge is 2 clock
	 * write recovery time is 2 clock
	 * no external address multiplexing
	 * normal timing for the control lines
	 * CAS latency is 3
	 */

	addis   r5,0,0x834f  
	ori     r5,r5,0x36a3 
        lis     r6, HIADJ (M8260_PSDMR (INTERNAL_MEM_MAP_ADDR))
        addi    r6, r6, LO (M8260_PSDMR (INTERNAL_MEM_MAP_ADDR))
	stw	r5, 0 (r6)

	/* OP selects the Precharge all banks command */

	addis   r5,0,0xab4f
	ori     r5,r5,0x36a3
	stw	r5, 0 (r6)
	addis   r0,0,0
 
	/* do a single write to an arbitrary location */

	addi    r5,0,0x00FF      /* Load 0x000000FF into r5 */
	stb     r5,0(r0)         /* Write 0xFF to address 0 - bits [24-31] */

	/* issue a "CBR Refresh" command to SDRAM */
	
	addis   r5,0,0x8b4f
	ori     r5,r5,0x36a3
	stw	r5, 0 (r6)
 
	addi	r7,0,0x0008
	mtspr	9,r7             /* Load spr CTR with 8 */
	addi 	r8,0,0x00FF      /* Load 0x000000FF into r8 */

SdramWrLoop:
	/* Loop 8 times, writing 0xFF to address 0 */

	stb  	r8,0(r0)        	/* Write 0xFF to address 0 */
	bc   	16,0,SdramWrLoop	/* Decrement CTR, and possibly branch */
 
	/* issue a "Mode Register Write" command to SDRAM */

	addis   r5,0,0x9b4f
	ori     r5,r5,0x36a3
	stw	r5, 0 (r6)
 
	/* do a single write to an arbitrary location */

	addi    r8,0,0x00FF      /* Load 0x000000FF into r8 */
	stb     r8,0(r0)         /* Write 0xFF to address 0 - bits [24-31] */

	/* enable refresh services and put SDRAM into normal operation */
	
	addis   r5,0,0xc34f
	ori     r5,r5,0x36a3
	stw	r5, 0 (r6)

#if DEBUG
	li		r3, 1
	bl		flashGreenLed
#endif

	/* invalidate TLBs: loop on all TLB entries using r7 as an index */

	addi	r0, 0, 0x0020
	mtspr	9, r0			/* Load CTR with 32 */
	addi	r7, 0, 0		/* Use r7 as the tlb index */

tlb_write_loop:

	tlbie	r7					/* invalidate the tlb entry */
	sync
	addi   	r7, r7, 0x1000			/* increment the index */
	bc		16, 0, tlb_write_loop	/* Decrement CTR, then branch if the */
						/* decremented CTR is not equal to 0 */

		/* Turn off data and instruction cache control bits */

	mfspr   r7, HID0
	isync
	sync				/* synchronize */
	andi.   r7, r7, 0x3FFF		/* Clear DCE and ICE bits */
	mtspr   HID0, r7
	isync
	sync				/* synchronize */

#ifdef INCLUDE_PCI
		/* Set the pciAutoconfig check to FALSE */
	xor		r5, r5, r5				/* Zero r5 */
	lis		r6, HIADJ (PCI_AUTO_CONFIG_ADRS)
	addi		r6, r6, LO (PCI_AUTO_CONFIG_ADRS)
	stw		r5, 0(r6)
	stw		r5, 0(r6)
#endif /* INCLUDE_PCI */

#ifdef INCLUDE_SECURITY_ENGINE

        lis     r5, HIADJ(SEC_ENG_SIZE_MASK) 
        addi    r5, r5, LO(SEC_ENG_SIZE_MASK)
	lis 	r6, HIADJ (M82XX_SECMR (INTERNAL_MEM_MAP_ADDR))
	addi	r6, r6, LO (M82XX_SECMR (INTERNAL_MEM_MAP_ADDR))
	stw 	r5, 0(r6)

        lis     r5, HIADJ(SEC_ENG_BASE_ADRS | 0x1) 
        addi    r5, r5, LO(SEC_ENG_BASE_ADRS | 0x1) 
	lis 	r6, HIADJ (M82XX_SECBR (INTERNAL_MEM_MAP_ADDR))
	addi	r6, r6, LO (M82XX_SECBR (INTERNAL_MEM_MAP_ADDR))
	stw 	r5, 0(r6)

#endif /* INCLUDE_SECURITY_ENGINE */
	/* Get the board revision number but do nothing for now */


	/* initialize the stack pointer */

	lis 	sp, HIADJ (STACK_ADRS)
	addi	sp, sp, LO (STACK_ADRS)

	/* go to C entry point */

	addi	sp, sp, -FRAMEBASESZ		/* get frame stack */

	/*
	 * restore startType from r31
	 * calculate C entry point: routine - entry point + ROM base
	 * routine	= romStart
	 * entry point	= romInit	= R7
	 * ROM base	= ROM_TEXT_ADRS = R8
	 * C entry point: romStart - R7 + R8
	 */

#if DEBUG
	li		r3, 5
	bl		flashGreenLed
#endif

	mr		r3,r31		/* load startType to parameter for 'C' func */

	lis		r7, HIADJ (romInit)
	addi	r7, r7, LO (romInit)

	lis		r8, HIADJ (ROM_TEXT_ADRS)
	addi	r8, r8, LO (ROM_TEXT_ADRS)

	lis 	r6, HIADJ (romStart)
	addi	r6, r6, LO (romStart)	/* load R6 with C entry point */

	sub 	r6, r6, r7		/* routine - entry point */
	add 	r6, r6, r8		/* + ROM base */

	mtspr   LR, r6			/* save destination address*/
							/* into LR register */
	blr						/* jump to the C entry point */



