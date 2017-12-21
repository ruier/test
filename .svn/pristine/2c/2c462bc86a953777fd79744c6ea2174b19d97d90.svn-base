/* romInit.s - Motorola Big Easy ROM initialization module */

/*
 * Copyright (c) 2002-2005, 2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 2004-2005 Motorola Inc. All Rights Reserved */

#include "copyright_wrs.h"

/*
modification history
--------------------
01g,20apr07,y_w  Fix reboot(2) problem. (WIND00087536)
01t,15jul05,cak  Added support for booting via the firmware.
01s,06jul05,cak  Added call to suite of memory tests.
01r,12apr05,cak  Set the 'M' (WIMGE) bit in the MMU entries for the DRAM.
01q,21feb05,cak  Fixed the DRAM scrubbing.
01p,07feb05,cak  L2 Cache/SRAM support. 
01o,20jan05,cak  Added #define for HID0.
01n,06jan05,cak  Removed memory controller hard-coding, changed FLASH
                 bank MMU mapping to cover all FLASH banks and added
                 enabling of the time base.
01m,13dec04,cak  Removed old INCLUDE_PCI and associated code. 
01l,18nov04,cak  Support for dynamically calculating memory controller 
                 values.
01k,09nov04,scb  Clean up while doing I2c support.
01j,11oct04,scb  Move flash banks to 0xf0000000.
01i,23aug04,scb  Starting point for "Big Easy".
01h,28oct03,dtr  Change OR value for ADS_PROTOTYPE.
01g,08oct03,dtr  Errata related to local bus DLL.
01f,12sep03,dtr  Removing CAM entry for PCI.
01e,02sep03,dtr  Remove zero of PID2 due to errata.
01d,05aug03,dtr  Adding SDRAM support.
01c,29jul03,dtr  Remove some magic numbers CCSBAR.
01b,22jul03,mil  Saved boot type and allow cold boot.
01a,03oct02,dtr  Created.
*/
	.data
#define	_ASMLANGUAGE
#include "vxWorks.h"
#include "sysLib.h"
#include "asm.h"
#include "config.h"
#include "regs.h"
#include "arch/ppc/mmuE500Lib.h"		
#include "mpc8540.h"

#define HID0	1008

FUNC_EXPORT(romInit)
FUNC_EXPORT(_romInit)
	
FUNC_IMPORT(romStart)

FUNC_IMPORT(sysMpc8540SdramInit)
FUNC_IMPORT(sysMemoryTest)

	_WRS_TEXT_SEG_START
	
	.fill	0x100,1,0
	
/***************************************************************************
*
* romInit - 
* This function (romInit) is linked to start ROM_TEXT_ADRS as a result
* of link instructions which reside at "h/tool/diab/ldscripts/link.DOTBOOTRAM"
*/

FUNC_BEGIN(romInit)	
FUNC_BEGIN(_romInit)
	bl	cold	

romWarm:
	bl	warm

	.ascii	"Copyright 1984-2002 Wind River Systems, Inc."
	.balign	4

cold:
	li	r2,BOOT_COLD
	b	tlbExpand	

warm:
	/* Workaround for reboot(2) causing system to hang*/
	cmpwi   r3,BOOT_COLD	/* check for reboot with clear */

        bne warm_resume
        bl coldresetstart	/* if so, cold reset */ 
warm_resume:
	mr	r2,r3
	b	resetEntry	

start:	

	/* turn off exceptions */

	mfmsr	r3                       /* r3 = msr              */
	INT_MASK (r3,r4)               	 /* mask EE and CE bit    */
	rlwinm	r4,r4,0,20,18            /* turn off _PPC_MSR_ME  */
	mtmsr	r4                       /* msr = r4              */
	isync

	xor	r0,r0,r0
	addi	r1,r0,-1

	mtspr	DEC,r0
	mtspr	TBL,r0
	mtspr	TBU,r0
	mtspr	TSR,r1
	mtspr	TCR,r0
	mtspr	ESR,r0		/* Clear Exception Syndrome Reg */
	mtspr	XER,r0		/* Clear Fixed-Point Exception Reg */

	xor	r6,r6,r6
	msync
	isync
	mtspr	L1CSR0,r6	/* Disable the Data cache */
	li	r6,0x0002
	msync
	isync
	mtspr	L1CSR0,r6	/* Invalidate the Data cache */
	li	r6,0x0000
	msync
	isync
	mtspr	L1CSR1,r6	/* Disable the Instruction cache */
	li	r6,0x0002
	msync
	isync
	mtspr	L1CSR1,r6	/* Invalidate the Instruction cache */
	isync
	li	r6,0x0000
	msync
	isync
	mtspr	L1CSR1,r6	/* disable Instruction & Data cache */
	msync
	isync

	xor	r6,r6,r6
	xor	r7,r7,r7
	mullw	r7,r7,r6
	lis	sp,HI(STACK_ADRS)
	ori	sp,sp,LO(STACK_ADRS)
	addi	sp,sp,-FRAMEBASESZ
	lis	r6,HI(romInit)
	ori	r6,r6,LO(romInit)
	lis	r7,HI(romStart)
	ori	r7,r7,LO(romStart)
	lis	r8,HI(ROM_TEXT_ADRS)
	ori	r8,r8,LO(ROM_TEXT_ADRS)
	sub	r6,r7,r6		/* routine - entry point */
	add	r6,r6,r8 		/* + ROM base */
	mtspr	LR,r6

	/* Dump LR to 0x4 */

	xor	r7,r7,r7
	stw	r6,4(r7)

	mr	r3,r2

	blr

FUNC_END(_romInit)
FUNC_END(romInit)

/***************************************************************************
*
* sdramInit - memory controller initialization
*/

FUNC_BEGIN(sdramInit)
FUNC_LABEL(_sdramInit)

        cmpwi   r2,BOOT_COLD	/* check for warm boot */
        bne     start		/* if warm boot, skip memory config */


        /* turn on the COM1 up-left LED*/
/*
        lis     r7,HIADJ (BRD_SYSTEM_STATUS_INDICATOR_REG)
        addi    r7,r7,LO (BRD_SYSTEM_STATUS_INDICATOR_REG)
        li      r6,0x0
        addi    r6,r6,0x02
        stb     r6,0(r7)
        mbar    0
*/

        /*
         * DDR Memory Controller Initialization
         *
         * In order for the memory controller initialization code
         * to be written in 'C' it is necessary to use the MPC8540's
         * L2 SRAM for the stack.  To do this we need an entry for
         * the SRAM in the MMU and we need to configure the L2 as
         * SRAM and enable it.
         */

        /* TLB1 #7. L2 SRAM */

        addis   r4,0,0x1007             /* TLBSEL = TLB1, ESEL = 7 */
        ori     r4,r4,0x0000
        mtspr   MAS0,r4
        addis   r5,0,0xc000             /* V = 1,IPROT = 1,TID = 0 */
        ori     r5,r5,_MMU_TLB_SZ_256K  /* TS = 0, TSIZE */
        mtspr   MAS1,r5
        addis   r6,0,HIADJ(MPC8540_L2_SRAM_BASE)         /* EPN */
        ori     r6,r6,0x000a            /* WIMGE = 01010 */
        mtspr   MAS2,r6
        addis   r7,0,HIADJ(MPC8540_L2_SRAM_BASE)         /* RPN */
        ori     r7,r7,0x0015            /* Supervisor XWR */
        mtspr   MAS3,r7
        tlbwe
        tlbsync

        /* enable L2 as SRAM */

        lis     r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_L2SRBAR0))
        addi    r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_L2SRBAR0))
        lis     r7,HIADJ(MPC8540_L2_SRAM_BASE)
        addi    r7,r7,LO(MPC8540_L2_SRAM_BASE)
        stw     r7,0x0(r6)
        lwz     r3,0(r6)
        lis     r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_L2ERRDIS))
        addi    r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_L2ERRDIS))
        lis     r7,HIADJ(0x0000001D)    /* disable all L2 error detection */
        addi    r7,r7,LO(0x0000001D)
        stw     r7,0x0(r6)
        lwz     r3,0(r6)
        mbar    0
        isync
        lis     r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_L2CTL))
        addi    r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_L2CTL))
        lis     r7,HIADJ(0x08010000)    /* all 256 Kbytes as SRAM */
        addi    r7,r7,LO(0x08010000)
        stw     r7,0x0(r6)
        lwz     r3,0(r6)
        mbar    0
        isync
        lis     r7,HIADJ(0x88010000)    /* set the L2E bit */
        addi    r7,r7,LO(0x88010000)
        stw     r7,0x0(r6)
        lwz     r3,0(r6)
        mbar    0

        /* set the stack pointer */

        lis     sp,HI((MPC8540_L2_SRAM_BASE+0x00040000)-0x10)
        ori     sp,sp,LO((MPC8540_L2_SRAM_BASE+0x00040000)-0x10)

        /* call DDR memory controller initialization routine */

        bl      sysMpc8540SdramInit
        or      r14,r3,r3                   /* save memory size */

        /*
         * When ECC is enabled it is necessary to scrub memory
         * to clear the ECC syndrome bits.
         *
         * We will turn on the data cache for fast scrubbing,
         * and disable it when we are finished scrubbing.
         */

        /* Invalidate Data Cache */

        addis   r6,r0,0x0000    /* add DCFI */
        ori     r6,r6,0x0002    /* add DCFI */
        sync                    /* synchronize */
        mfspr   r7,1010         /* load spr1010/L1CSR0 register */
        sync                    /* synchronize */
        or      r7,r7,r6        /* set bit (invalidate cache) */
        isync                   /* synchronize */
        sync                    /* synchronize */
        mtspr   1010,r7         /* store spr1010/L1CSR0 register */
        andc    r7,r7,r6        /* clear bit (invalidate cache) */
        sync                    /* synchronize */
        mtspr   1010,r7         /* store spr1010/L1CSR0 register */
        sync                    /* synchronize */

        /* Enable the Data and Instruction Caches */

        addis   r6,r0,0x0000    /* add DCE */
        ori     r6,r6,0x0001    /* add DCE */
        sync                    /* synchronize */
        isync                   /* synchronize */
        mfspr   r7,1010         /* load spr1010/L1CSR0 register */
        sync                    /* synchronize */
        or      r7,r7,r6        /* set bit (enable cache) */
        sync                    /* synchronize */
        isync                   /* synchronize */
        mtspr   1010,r7         /* store spr1010/L1CSR0 register */
        sync                    /* synchronize */
        mfspr   r7,1011         /* load spr1011/L1CSR1 register */
        sync                    /* synchronize */
        or      r7,r7,r6        /* set bit (enable cache) */
        sync                    /* synchronize */
        isync                   /* synchronize */
        mtspr   1011,r7         /* store spr1011/L1CSR1 register */
        sync                    /* synchronize */

#ifdef INCLUDE_ECC

        /* scrub */

        xor     r0,r0,r0                /* clear r0 */
        or      r15,r14,r14             /* memory size */
        rlwinm  r15,r15,27,0,31         /* calculate number of doubles */
        lis     r16,HIADJ(LOCAL_MEM_LOCAL_ADRS) /* starting address */
        addi    r16,r16,LO(LOCAL_MEM_LOCAL_ADRS)
        mtspr   CTR,r15                 /* load number of doubles/words */
scrubLoop:
        dcbz    r0,r16
        dcbf    r0,r16
        addi    r16,r16,0x20
        bc      16,0,scrubLoop          /* branch until counter == 0 */

        /* Flush the Data Cache */

        li      r3,0            /* start at addr 0 */
        li      r4,0x4000       /* flush 16384 blocks */
        mtctr   r4              /* load counter with initial value */
flushLoop:
        lwz     r5,0(r3)        /* reads at 32-byte boundaries */
        addi    r3,r3,32        /* increment for next read/flush */
        bdnz    flushLoop       /* ctr--, loop if ctr != 0 */
        sync                    /* synchronize */

        /* Clear ECC Errors generated during scrubbing */

        lis     r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_ERR_SBE))
        addi    r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_ERR_SBE))
        xor     r0,r0,r0                /* clear r0 */
        stw     r0,0x0(r6)
        sync
        lis     r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_ERR_DETECT))
        addi    r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_ERR_DETECT))
        lis     r7,HIADJ(0x8000000D)	/* write 1 to each error bit */
        addi    r7,r7,LO(0x8000000D)	/* to clear the error */
        stw     r7,0x0(r6)
        sync
#endif

	/* Execute Suite of Memory Tests */

        or      r3,r14,r14
        bl      sysMemoryTest
	
        /* Invalidate Data & Instruction Caches */

        addis   r6,r0,0x0000    /* add DCFI */
        ori     r6,r6,0x0002    /* add DCFI */
        sync                    /* synchronize */
        mfspr   r7,1010         /* load spr1010/L1CSR0 register */
        sync                    /* synchronize */
        or      r7,r7,r6        /* set bit (invalidate cache) */
        isync                   /* synchronize */
        sync                    /* synchronize */
        mtspr   1010,r7         /* store spr1010/L1CSR0 register */
        andc    r7,r7,r6        /* clear bit (invalidate cache) */
        sync                    /* synchronize */
        mtspr   1010,r7         /* store spr1010/L1CSR0 register */
        sync                    /* synchronize */
        mfspr   r7,1011         /* load spr1011/L1CSR1 register */
        sync                    /* synchronize */
        or      r7,r7,r6        /* set bit (invalidate cache) */
        isync                   /* synchronize */
        sync                    /* synchronize */
        mtspr   1011,r7         /* store spr1011/L1CSR1 register */
        andc    r7,r7,r6        /* clear bit (invalidate cache) */
        sync                    /* synchronize */
        mtspr   1011,r7         /* store spr1011/L1CSR1 register */
        sync                    /* synchronize */

        /* Disable the Data & Instruction Caches */

        addis   r6,r0,0x0000    /* add DCE */
        ori     r6,r6,0x0001    /* add DCE */
        isync                   /* synchronize */
        sync                    /* synchronize */
        mfspr   r7,1010         /* load spr1010/L1CSR0 register */
        sync                    /* synchronize */
        andc    r7,r7,r6        /* clear bit (disable cache) */
        sync                    /* synchronize */
        mtspr   1010,r7         /* store spr1010/L1CSR0 register */
        sync                    /* synchronize */
        mfspr   r7,1011         /* load spr1011/L1CSR1 register */
        sync                    /* synchronize */
        andc    r7,r7,r6        /* clear bit (disable cache) */
        sync                    /* synchronize */
        mtspr   1011,r7         /* store spr1011/L1CSR1 register */
        sync                    /* synchronize */

        /*
         * now that the memory controller has been initialized
         * we can disable the L2 SRAM
         */

        mbar    0
        isync
        lis     r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_L2CTL))
        addi    r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_L2CTL))
        lis     r7,HIADJ(0x00000000)
        addi    r7,r7,LO(0x00000000)
        stw     r7,0x0(r6)
        lwz     r3,0(r6)
        mbar    0

        /* disable TLB1 entry 7 (L2 SRAM) */

        addis   r4,0,0x1007            /* TLBSEL = TLB1(CAM), ESEL = 7 */
        ori     r4,r4,0x0000
        mtspr   MAS0,r4
        addis   r5,0,0x0000
        ori     r5,r5,0x0000
        mtspr   MAS1,r5
        addis   r6,0,0x0000
        ori     r6,r6,0x0000
        mtspr   MAS2,r6
        addis   r7,0,0x0000
        ori     r7,r7,0x0000
        mtspr   MAS3,r7
        tlbwe
        tlbsync

	b	start

FUNC_END(sdramInit)

/***************************************************************************
*
* resetEntry - ROM entry point, section '.boot'.
* Check linker command file: h/tool/diab/ldscripts/link.DOTBOOTRAM
* this is linked to start at 0x0fffff800 (4GB - 0x800)
*/

#if   defined(_GNU_TOOL)
	.section .boot,"ax",@progbits
#elif defined(_DIAB_TOOL)
	.section .boot,4,"rx"
#else
#error "Please add a correctly spelled .section directive for your toolchain."
#endif

FUNC_BEGIN(resetEntry)
FUNC_LABEL(_resetEntry)

	/* Clear SRR0, CSRR0, MCSRR0, SRR1, CSRR1, MCSRR1, IVPR */

	xor	r0,r0,r0
	mtspr 	SRR0,r0
	mtspr	SRR1,r0
	mtspr	CSRR0,r0
	mtspr	CSRR1,r0
	mtspr	MCSRR0,r0
	mtspr	MCSRR1,r0
	mtspr	IVPR,r0

	/* Enable Time Base */

	lis	r4,HIADJ(0x00004000)
	addi	r4,r4,LO(0x00004000)
	mtspr	HID0,r4

	/* Set [ABE] in HID1 for L2 Cache Management */

        addis   r4,r0,0x0000    /* add ABE */
        ori     r4,r4,0x1000    /* add ABE */
        sync                    /* synchronize */
        mfspr   r7,1009         /* load spr1009/HID1 register */
        sync                    /* synchronize */
        or      r7,r7,r4        /* set bit */
        isync                   /* synchronize */
        sync                    /* synchronize */
        mtspr   1009,r7         /* store spr1009/HID1 register */

	/* Set up vector offsets */

	addi	r3,r0,0x100
	mtspr	IVOR0,r3
	addi	r3,r0,0x200
	mtspr	IVOR1,r3
	addi	r3,r0,0x300
	mtspr	IVOR2,r3
	addi	r3,r0,0x400
	mtspr	IVOR3,r3
	addi	r3,r0,0x500
	mtspr	IVOR4,r3
	addi	r3,r0,0x600
	mtspr	IVOR5,r3
	addi	r3,r0,0x700
	mtspr	IVOR6,r3

	/* skipping IVOR7 0x800: no FPU on e500 */

	addi	r3,r0,0x900
	mtspr	IVOR8,r3

	/* skipping IVOR9 0xa00: no aux processor on e500 */

	addi	r3,r0,0xb00
	mtspr	IVOR10,r3
	addi	r3,r0,0xc00
	mtspr	IVOR11,r3
	addi	r3,r0,0xd00
	mtspr	IVOR12,r3
	addi	r3,r0,0xe00
	mtspr	IVOR13,r3
	addi	r3,r0,0xf00
	mtspr	IVOR14,r3
	addi	r3,r0,0x1000
	mtspr	IVOR15,r3
	addi	r3,r0,0x1100       /* SPU is e500 specific */
	mtspr	IVOR32,r3
	addi	r3,r0,0x1200       /* FP data is e500 specific */
	mtspr	IVOR33,r3
	addi	r3,r0,0x1300       /* FP round is e500 specific */
	mtspr	IVOR34,r3
	addi	r3,r0,0x1400       /* perf mon is e500 specific */
	mtspr	IVOR35,r3

	/* 
	 * Initialize PID reg to match TID field of TLB entry 
	 * (unless using TID=0) 
	 */

	xor	r0,r0,r0
	mtspr	PID0,r0
	mtspr	PID1,r0

	/* mtspr PID2, r0 comment out for now due to suspected errata */

	/*
	 * flash invalidate all level 1 TLBs and TLB0 entries
	 * and unprotected TLB1 entries for warmStart to prevent overlap
	 */

	li	r4,0x1c                /* IL1MMU_FI | DL1MMU_FI | L2TLB0_FI */
	mtspr	MMUCSR0,r4
tstTlbClear0:
	mfspr	r4,MMUCSR0
	cmpwi	r4,0
	bne	tstTlbClear0
	li	r4,0x2                 /* L2TLB1_FI */
	mtspr	MMUCSR0,r4
tstTlbClear1:
	mfspr	r4,MMUCSR0
	cmpwi	r4,0
	bne	tstTlbClear1
	tlbsync

	/*
	 * Clear TLB1, ESEL 6
	 * 
	 * We need to disable ESEL 6 in case we are booting via the
	 * firmware.  This is necessary to avoid a conflict in address
	 * mapping.
	 */

        addis   r4,0,0x1006            /* TLBSEL = TLB1(CAM), ESEL = 6 */
        ori     r4,r4,0x0000
        mtspr   MAS0,r4
        addis   r5,0,0x0000
        ori     r5,r5,0x0000
        mtspr   MAS1,r5
        addis   r6,0,0x0000
        ori     r6,r6,0x0000
        mtspr   MAS2,r6
        addis   r7,0,0x0000
        ori     r7,r7,0x0000
        mtspr   MAS3,r7
        tlbwe
        tlbsync

	/*
	 * TLB1 #1.  CCSBAR space
	 *           0xe1000000 -> 0xe1ffffff
	 * Attributes: UX/UW/UR/SX/SW/SR
	 */

	addis	r4,0,0x1001            /* TLBSEL = TLB1(CAM), ESEL = 1 */
	ori	r4,r4,0x0000
	mtspr	MAS0,r4
	addis	r5,0,0xc000            /* V = 1,IPROT = 1,TID = 0 */
	ori	r5,r5,_MMU_TLB_SZ_1M   /* TS = 0,TSIZE = 1 MByte page size */
	mtspr	MAS1,r5
	addis	r6,0,HIADJ(CCSBAR)     /* EPN = CCSBAR base */
	ori	r6,r6,0x000a           /* WIMGE = 01010 */
	mtspr	MAS2,r6
	addis	r7,0,HIADJ(CCSBAR)     /* RPN = CCSBAR base */
	ori	r7,r7,0x003f           /* User/Supervisor XWR */
	mtspr	MAS3,r7
	tlbwe                       
	tlbsync                      

	/*
	 * TLB1 #2.  Local board access
	 * Attributes: UX/UW/UR/SX/SW/SR
	 */

	addis	r4,0,0x1002            /* TLBSEL = TLB1(CAM), ESEL = 2 */
	ori	r4,r4,0x0000
	mtspr	MAS0,r4
	addis	r5,0,0xc000            /* V = 1,IPROT = 1,TID = 0 */
	ori	r5,r5,LOCAL_BOARD_ACCESS_MMU_TBL_SZ    /* TS = 0, TSIZE */
	mtspr	MAS1,r5
	addis	r6,0,HIADJ(LOCAL_BOARD_ACCESS_ADRS) /* EPN */
	ori	r6,r6,0x000a           /* WIMGE = 01010 */
	mtspr	MAS2,r6
	addis	r7,0,HIADJ(LOCAL_BOARD_ACCESS_ADRS) /* RPN */
	ori	r7,r7,0x003f           /* User/Supervisor XWR */
	mtspr	MAS3,r7
	tlbwe                       
	tlbsync 
	
	/*
	 * Write TLB entry for initial program memory page
	 *
	 * - Specify EPN, RPN, and TSIZE as appropriate for system
	 * - Set valid bit
	 * - Specify TID=0
	 * - Specify TS=0 or else MSR[IS,DS] must be set to correspond to TS=1
	 * - Specify storage attributes (W, I, M, G, E, U0 - U3) as appropriate
	 * - Enable supervisor mode fetch, read, and write access (SX, SR, SW)
	 */

	/*
	 * TLB1 #3.  Main SDRAM 1/4 - Not Cached
	 *           0x00000000 -> 0x0FFFFFFF
	 * Attributes: UX/UW/UR/SX/SW/SR
	 */

	addis	r4,0,0x1003            /* TLBSEL = TLB1(CAM), ESEL = 3 */
	ori	r4,r4,0x0000
	mtspr	MAS0,r4
	addis	r5,0,0xc000            /* V = 1,IPROT = 1,TID = 0 */
	ori	r5,r5,_MMU_TLB_SZ_256M /* TS = 0,TSIZE = 256 MByte page size */
	mtspr	MAS1,r5
	addis	r6,0,0x0000            /* EPN */
	ori	r6,r6,0x0006           /* WIMGE = 00110 */
	mtspr	MAS2,r6
	addis	r7,0,0x0000            /* RPN */
	ori	r7,r7,0x003f           /* User/Supervisor XWR */
	mtspr	MAS3,r7
	tlbwe                       
	tlbsync                      
	
	/*
	 * TLB1 #4.  Main SDRAM 2/4 - Not Cached
	 *           0x10000000 -> 0x1FFFFFFF
	 * Attributes: UX/UW/UR/SX/SW/SR
	 */

	addis	r4,0,0x1004            /* TLBSEL = TLB1(CAM), ESEL = 4 */
	ori	r4,r4,0x0000
	mtspr	MAS0,r4
	addis	r5,0,0x8000            /* V = 1,IPROT = 0,TID = 0 */
	ori	r5,r5,_MMU_TLB_SZ_256M /* TS = 0,TSIZE = 256 MByte page size */
	mtspr	MAS1,r5
	addis	r6,0,0x1000            /* EPN */
	ori	r6,r6,0x0006           /* WIMGE = 00110 */
	mtspr	MAS2,r6
	addis	r7,0,0x1000            /* RPN */
	ori	r7,r7,0x003f           /* User/Supervisor XWR */
	mtspr	MAS3,r7
	tlbwe                       
	tlbsync                      
	
	/*
	 * TLB1 #5.  Main SDRAM 3/4 - Not Cached
	 *           0x20000000 -> 0x2FFFFFFF
	 * Attributes: UX/UW/UR/SX/SW/SR
	 */

	addis	r4,0,0x1005            /* TLBSEL = TLB1(CAM), ESEL = 5 */
	ori	r4,r4,0x0000
	mtspr	MAS0,r4
	addis	r5,0,0x8000            /* V = 1,IPROT = 0,TID = 0 */
	ori	r5,r5,_MMU_TLB_SZ_256M /* TS = 0,TSIZE = 256 MByte page size */
	mtspr	MAS1,r5
	addis	r6,0,0x2000            /* EPN */
	ori	r6,r6,0x0006           /* WIMGE = 00110 */
	mtspr	MAS2,r6
	addis	r7,0,0x2000            /* RPN */
	ori	r7,r7,0x003f           /* User/Supervisor XWR */
	mtspr	MAS3,r7
	tlbwe                       
	tlbsync                      
	
	/*
	 * TLB1 #6.  Main SDRAM 4/4 - Not Cached
	 *           0x30000000 -> 0x3FFFFFFF
	 * Attributes: UX/UW/UR/SX/SW/SR
	 */

	addis	r4,0,0x1006            /* TLBSEL = TLB1(CAM), ESEL = 6 */
	ori	r4,r4,0x0000
	mtspr	MAS0,r4
	addis	r5,0,0x8000            /* V = 1,IPROT = 0,TID = 0 */
	ori	r5,r5,_MMU_TLB_SZ_256M /* TS = 0,TSIZE = 256 MByte page size */
	mtspr	MAS1,r5
	addis	r6,0,0x3000            /* EPN */
	ori	r6,r6,0x0006           /* WIMGE = 00110 */
	mtspr	MAS2,r6
	addis	r7,0,0x3000            /* RPN */
	ori	r7,r7,0x003f           /* User/Supervisor XWR */
	mtspr	MAS3,r7
	tlbwe                       
	tlbsync                      
	
	/*
	 * Setup the memory mapped register address
	 * XXX - needed only for cold boot
	 */

	lis	r6,HIADJ(CCSBAR >> 12)
	addi	r6,r6,LO(CCSBAR >> 12)
	lis	r7,HIADJ(0xFF700000)
	addi	r7,r7,LO(0xFF700000)
	stw	r6,0(r7)
	sync
	isync

	/* Set 0 - FLASH */

	lis	r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_LAWBAR0))
	addi	r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_LAWBAR0))

	lis	r7,HIADJ(CCSR_LAWBAR0_PROG)
	addi	r7,r7,LO(CCSR_LAWBAR0_PROG)
	stw	r7,0x0(r6)

	lis	r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_LAWAR0))
	addi	r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_LAWAR0))

	lis	r7,HIADJ (CCSR_LAWAR0_PROG)
	addi	r7,r7,LO (CCSR_LAWAR0_PROG)
	stw	r7,0x0(r6)
	mbar	0

	/* Set 1 - DRAM */

	lis	r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_LAWBAR1))
	addi	r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_LAWBAR1))

	lis	r7,HIADJ(CCSR_LAWBAR1_PROG)
	addi	r7,r7,LO(CCSR_LAWBAR1_PROG)
	stw	r7,0x0(r6)

	lis	r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_LAWAR1))
	addi	r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_LAWAR1))

	lis	r7,HIADJ (CCSR_LAWAR1_PROG)
	addi	r7,r7,LO (CCSR_LAWAR1_PROG)
	stw	r7,0x0(r6)
	mbar	0

	/* Set 2 */

	lis	r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_LAWBAR2))
	addi	r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_LAWBAR2))

	lis	r7,HIADJ(CCSR_LAWBAR2_PROG)
	addi	r7,r7,LO(CCSR_LAWBAR2_PROG)
	stw	r7,0x0(r6)

	lis	r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_LAWAR2))
	addi	r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_LAWAR2))

	lis	r7,HIADJ (CCSR_LAWAR2_PROG)
	addi	r7,r7,LO (CCSR_LAWAR2_PROG)
	stw	r7,0x0(r6)
	mbar	0

	/* Set 3 */

	lis	r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_LAWBAR3))
	addi	r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_LAWBAR3))

	lis	r7,HIADJ(CCSR_LAWBAR3_PROG)
	addi	r7,r7,LO(CCSR_LAWBAR3_PROG)
	stw	r7,0x0(r6)

	lis	r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_LAWAR3))
	addi	r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_LAWAR3))

	lis	r7,HIADJ (CCSR_LAWAR3_PROG)
	addi	r7,r7,LO (CCSR_LAWAR3_PROG)
	stw	r7,0x0(r6)
	mbar	0

	/* Set 4 */

	lis	r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_LAWBAR4))
	addi	r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_LAWBAR4))

	lis	r7,HIADJ(CCSR_LAWBAR4_PROG)
	addi	r7,r7,LO(CCSR_LAWBAR4_PROG)
	stw	r7,0x0(r6)

	lis	r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_LAWAR4))
	addi	r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_LAWAR4))

	lis	r7,HIADJ (CCSR_LAWAR4_PROG)
	addi	r7,r7,LO (CCSR_LAWAR4_PROG)
	stw	r7,0x0(r6)
	mbar	0

	/* Set 5 */

	lis	r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_LAWBAR5))
	addi	r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_LAWBAR5))

	lis	r7,HIADJ(CCSR_LAWBAR5_PROG)
	addi	r7,r7,LO(CCSR_LAWBAR5_PROG)
	stw	r7,0x0(r6)

	lis	r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_LAWAR5))
	addi	r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_LAWAR5))

	lis	r7,HIADJ (CCSR_LAWAR5_PROG)
	addi	r7,r7,LO (CCSR_LAWAR5_PROG)
	stw	r7,0x0(r6)
	mbar	0

	/* Set 6 */

	lis	r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_LAWBAR6))
	addi	r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_LAWBAR6))

	lis	r7,HIADJ(CCSR_LAWBAR6_PROG)
	addi	r7,r7,LO(CCSR_LAWBAR6_PROG)
	stw	r7,0x0(r6)

	lis	r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_LAWAR6))
	addi	r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_LAWAR6))

	lis	r7,HIADJ (CCSR_LAWAR6_PROG)
	addi	r7,r7,LO (CCSR_LAWAR6_PROG)
	stw	r7,0x0(r6)
	mbar	0

	/* Set 7 */

	lis	r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_LAWBAR7))
	addi	r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_LAWBAR7))

	lis	r7,HIADJ(CCSR_LAWBAR7_PROG)
	addi	r7,r7,LO(CCSR_LAWBAR7_PROG)
	stw	r7,0x0(r6)

	lis	r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_LAWAR7))
	addi	r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_LAWAR7))

	lis	r7,HIADJ (CCSR_LAWAR7_PROG)
	addi	r7,r7,LO (CCSR_LAWAR7_PROG)
	stw	r7,0x0(r6)
	mbar	0

	/*
	 * Initialize the Local Bus Controller
	 * Start with clock ratio register, set as follows:
	 *
	 *   bit 3:1     = 1    -> CLKDIV: ratio CCB/memBus clock = 4
	 *   bit 17:16   = 11   -> EADC: 3 cycles for assertion of LALE.
	 *   bit 25:24   = 00   -> ECL:
	 *   bit 29:28   = 00   -> BUFCMDC:
	 *   bit 31      = 0    -> DBYP: DLL is enabled.
	 *
	 */

	lis	r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_LCRR))
	addi	r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_LCRR))
	lis	r7,HIADJ(0x00030004)
	addi	r7,r7,LO(0x00030004)
	stw	r7,0x0(r6)
	
	/* Now program chip select 0 (BR0/OR0) */

	lis	r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_BR0))
	addi	r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_BR0))
	lis	r7,HIADJ(CCSR_BR0_PROG)
	addi	r7,r7,LO(CCSR_BR0_PROG)
	stw	r7,0x0(r6)
	
	lis	r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_OR0))
	addi	r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_OR0))
	lis	r7,HIADJ(CCSR_OR0_PROG)
	addi	r7,r7,LO(CCSR_OR0_PROG)
	stw	r7,0x0(r6)

	/* Now for chip select 1 (BR1/OR1) */

	lis	r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_BR1))
	addi	r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_BR1))
	lis	r7,HIADJ(CCSR_BR1_PROG)
	addi	r7,r7,LO(CCSR_BR1_PROG)
	stw	r7,0x0(r6)
	
	lis	r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_OR1))
	addi	r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_OR1))
	lis	r7,HIADJ(CCSR_OR1_PROG)
	addi	r7,r7,LO(CCSR_OR1_PROG)
	stw	r7,0x0(r6)

	/* Now for chip select 2 (BR2/OR2) */

	lis	r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_BR2))
	addi	r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_BR2))
	lis     r7,HIADJ(CCSR_BR2_PROG)
	addi    r7,r7,LO(CCSR_BR2_PROG)
	stw	r7,0x0(r6)
	
	lis	r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_OR2))
	addi	r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_OR2))
	lis	r7,HIADJ(CCSR_OR2_PROG)
	addi	r7,r7,LO(CCSR_OR2_PROG)
	stw	r7,0x0(r6)

	/* Now for chip select 3 (BR3/OR3) */

	lis	r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_BR3))
	addi	r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_BR3))
	lis     r7,HIADJ(CCSR_BR3_PROG)
	addi    r7,r7,LO(CCSR_BR3_PROG)
	stw	r7,0x0(r6)
	
	lis	r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_OR3))
	addi	r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_OR3))
	lis	r7,HIADJ(CCSR_OR3_PROG)
	addi	r7,r7,LO(CCSR_OR3_PROG)
	stw	r7,0x0(r6)

	/* Now for chip select 4 (BR4/OR4) */

	lis	r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_BR4))
	addi	r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_BR4))
	lis     r7,HIADJ(CCSR_BR4_PROG)
	addi    r7,r7,LO(CCSR_BR4_PROG)
	stw	r7,0x0(r6)
	
	lis	r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_OR4))
	addi	r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_OR4))
	lis	r7,HIADJ(CCSR_OR4_PROG)
	addi	r7,r7,LO(CCSR_OR4_PROG)
	stw	r7,0x0(r6)

	/* Now for chip select 5 (BR5/OR5) */

	lis	r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_BR5))
	addi	r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_BR5))
	lis     r7,HIADJ(CCSR_BR5_PROG)
	addi    r7,r7,LO(CCSR_BR5_PROG)
	stw	r7,0x0(r6)
	
	lis	r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_OR5))
	addi	r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_OR5))
	lis	r7,HIADJ(CCSR_OR5_PROG)
	addi	r7,r7,LO(CCSR_OR5_PROG)
	stw	r7,0x0(r6)

	/* Now for chip select 6 (BR6/OR6) */

	lis	r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_BR6))
	addi	r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_BR6))
	lis     r7,HIADJ(CCSR_BR6_PROG)
	addi    r7,r7,LO(CCSR_BR6_PROG)
	stw	r7,0x0(r6)
	
	lis	r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_OR6))
	addi	r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_OR6))
	lis	r7,HIADJ(CCSR_OR6_PROG)
	addi	r7,r7,LO(CCSR_OR6_PROG)
	stw	r7,0x0(r6)

	/* Now for chip select 7 (BR7/OR7) */

	lis	r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_BR7))
	addi	r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_BR7))
	lis     r7,HIADJ(CCSR_BR7_PROG)
	addi    r7,r7,LO(CCSR_BR7_PROG)
	stw	r7,0x0(r6)
	
	lis	r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_OR7))
	addi	r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_OR7))
	lis	r7,HIADJ(CCSR_OR7_PROG)
	addi	r7,r7,LO(CCSR_OR7_PROG)
	stw	r7,0x0(r6)

/*
 * Store DLL setting and override so as to avoid glitches in local bus 
 * due to errata. Start with delay to allow DLL to stabilize
 */

	lis	r6,0x1
	mtctr	r6
delayLoop:
	nop
	bdnz	delayLoop

/* Read LBC DLL and shift in to override bits and set override bit */	

	lis	r6,HIADJ(CCSR_ADDR(CCSBAR,CCSR_LBDLLCR))
	addi	r6,r6,LO(CCSR_ADDR(CCSBAR,CCSR_LBDLLCR))
	lwz	r3,0(r6)
	slwi	r3,r3,16
	lis	r5,0x8000
	or	r3,r3,r5
	stw	r3,0(r6)

#ifndef FIRMWARE_BOOT
	b	sdramInit	/* memory controller initialization */	
#else
	b	start		/* skip memory controller initialization */
#endif /* FIRMWARE_BOOT */

coldresetstart:
 	lis r10,HIADJ(BRD_SYSTEM_CONTROL_REG)
 	ori r10,r10,LO(BRD_SYSTEM_CONTROL_REG)
 	lbz r11,0(r10)
 	ori r11,r11,BRD_SYSTEM_CONTROL_RESET_VAL
 	stb r11,0(r10)
 	bl   warm_resume 

FUNC_END(resetEntry)

/*
 * Construct in MAS0 thru MAS3, information which will map
 * the flash boot bank into the translation
 * lookaside buffer.  This address range includes the ROM
 * from which we are currently executing.  Once MAS0 thru
 * MAS3 have been initialized, the information contained
 * therin is transfered to the translation lookaside buffer
 * (TLB) via the tlbwe (translation lookaside buffer write
 * entry) instruction.
 */

FUNC_BEGIN(tlbExpand)

	/*
	 * MAS0 - program it as follows:
	 * - Select TLB1  via TBLSEL (bit 35 (3))
	 *     Note that TLB1 allows variable length page size, we
	 *     are not forced to a 4K page size as we would be with
	 *     TLB0.
	 * - Select entry number 0 in the array via ESEL (bits 44-47(12-15))
	 */  

	addis	r4,0,0x1000             /* TLBSEL = TLB1(CAM) ,ESEL = 0 */ 
	ori	r4,r4,0x0000
	mtspr	MAS0,r4

	/*
	 * MSA1 - program it as follows:
	 * - Entry is valid via V (bit 32(0))
	 * - Do not protect from invalidation via IPROT (bit 33(1))
	 * - Translation ID of 0 via TID (bits 40-47(8-15))
	 * - Translation space of 0 via TS (bit 51(19)) note that this
	 *     must coordinate with IS field of MSR register
	 * - Translation size = FLASH_MMU_TLB_SZ via TSIZE (bits 52-55(20-25))
	 */

	addis	r5,0,0xc000            /* V = 1,IPROT = 1,TID = 0 */
	ori	r5,r5,FLASH_MMU_TLB_SZ /* TS = 0,TSIZE */
	mtspr	MAS1,r5

	/*
	 * MSA2 - program it as follows:
	 * - Effective page number via EPN (bits 32-51(0-19)).
	 * - Cacheing attributes = write back (W=0), 
	 *     cacheing allowed (I=0), coherence not required (M=0),
	 *     guarded (G=1), Endian = bit (E=0).  (WIMGE bits are
	 *     bits 59-63(27-31)).
	 */

	lis	r6,HIADJ(FLASH_BASE_ADRS)
	ori	r6,r6,0x0002            /* WIMGE = 00010 */
	mtspr	MAS2,r6

	/*
	 * MSA3 - program it as follows:
	 * - Real Page Number via RPN (bits 32-51(0-19)).  Note that
	 *     in combination with MAS2 (EPN), this makes the virtual to
	 *     physical mapping an identity (virtual address = physical
	 *     address).
	 * - Permissions are: User no execute (UX = 0), Supervisor
	 *     execute (SX = 1), User no write (UW = 0), Supervisor 
	 *     write (SW = 1), User no read (UR = 0), Supervisor Read
	 *     (SR = 0).  (UX_SX_UW_SW_UR_SR bits are 58-63(24-31)).
	 *
	 */

	lis	r7,HIADJ(FLASH_BASE_ADRS)
	ori	r7,r7,0x0015            /* Supervisor XWR */
	mtspr	MAS3,r7

	/*
	 * Write the entry and sync the TLB.
	 */

	tlbwe         
	tlbsync
	b	resetEntry                     

FUNC_END(tlbExpand)

FUNC_BEGIN(coldStart)

        li      r2,BOOT_COLD
	b	tlbExpand

FUNC_END(coldStart)

#if   defined(_GNU_TOOL)
	.section .reset,"ax",@progbits
#elif defined(_DIAB_TOOL)
	.section .reset,4,"rx"
#else
#error "Please add a correctly spelled .section directive for your toolchain."
#endif
FUNC_BEGIN(resetVector)

/*
 * Here is where execution begins coming out of reset:
 *
 * When the e500 core comes out of reset, its MMU has one 4-Kbyte page
 * defined at 0xFFFF_Fnnn.  The first instruction executed by the e500
 * core is always address 0xFFFF_FFFC, which must be a branch to an
 * address within the 4-Kbyte boot page.  The instruction below appears
 * at 0xfffffffc in the build as a result of being in the ".reset"
 * section.  The linker command file h/tool/diab/ldscripts/link.DOTBOOTRAM
 * makes sure of this.
 */

	b	coldStart	

FUNC_END(resetVector)
