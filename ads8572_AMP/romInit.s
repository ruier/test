/* romInit.s - ROM bootcode for ads8572 BSP */
	
/* Copyright (c) 2007-2008 Wind River Systems, Inc. 
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind RIver License agreement.
 */
		
/*
modification history
--------------------
01e,27aug08,dtr  Fix compiler error.
01d,15aug08,dtr  Modify AMP boot approach.
01c,28jan08,dtr  Update DDR settings.
01b,19dec07,dtr  Add Support for AMP.
01a,03oct07,dtr  Created from ads8544/01b.
*/

#define	_ASMLANGUAGE
#include <vxWorks.h>
#include <sysLib.h>
#include <asm.h>
#include "config.h"
#include <regs.h>
#include <arch/ppc/mmuE500Lib.h>

FUNC_EXPORT(romInit)
FUNC_EXPORT(_romInit)
	
FUNC_IMPORT(romStart)

/*
 * LOADVAR initializes a register with the contents of a specified memory
 * address. The difference being that the value loaded is the contents of
 * the memory location and not just the address of it. [REMOVE WHEN NO LONGER
 * NEEDED].
 * This is tracked at WRS under SPR#20104.
 */

#undef LOADVAR
#define	LOADVAR(reg, addr32) \
	  addis reg, r0, HIADJ(addr32); \
	  lwz reg, LO(addr32)(reg)

#define WRITEADR(reg1,reg2,addr32,val) \
	lis	reg1, HI(addr32); \
	ori	reg1, reg1, LO(addr32); \
	lis	reg2, HI(val); \
	ori	reg2, reg2, LO(val); \
	stw	reg2, 0(reg1)

#define WRITEOFFSET(regbase,reg2,offset,val) \
        lis	reg2, HI(val); \
        ori	reg2, reg2, LO(val); \
	stw	reg2, offset(regbase);
	
	_WRS_TEXT_SEG_START
	
	.fill 0x100,1,0
	
FUNC_BEGIN(romInit)	
FUNC_BEGIN(_romInit)
	bl     resetEntry

romWarm:
	bl     warm
	/* Useful to have recognizeable string */
        .ascii   "Wind River vxWorks bootrom"

        .balign 4

platpllTable:	
	.long 0x00000000
	.long 0x04050600
	.long 0x08000a00
	.long 0x0c000000
	.long 0x00000000
	.long 0x00000000

cold:
	li    r2, BOOT_COLD
	b     start
	/* defines for memory initialization */
warm:
	mr     r2,r3
		
	mfspr	r6, HID1
	andi.   r6, r6, 1
	cmpwi   r6, 1	
	bne     start		
	/* running on CPU1 */
	/* jump to sysInit for AMP - never get here for SMP */
waitForStartSetWarm:	
	LOADVAR (r3, CPU1_INIT_START_ADR)
	/* Check that init start address is not zero before jumping 
	 * This allows cpu1 to reboot and wait for cpu0 to reload image */
	lis     r4,HI(MP_MAGIC_RBV)
	ori     r4, r4, LO(MP_MAGIC_RBV)
	cmpw    r3, r4
	beq     waitForStartSetWarm
	mtctr	r3
	bctr

start:	
        /* turn off exceptions */
        mfmsr   r3                      /* r3 = msr              */
        INT_MASK (r3, r4)               /* mask EE and CE bit    */
        rlwinm  r4, r4, 0, 20, 18       /* turn off _PPC_MSR_ME  */
        mtmsr   r4                      /* msr = r4              */
        isync

        xor     r0, r0, r0
        addi    r1, r0, -1

        mtspr   DEC, r0
        mtspr   TBL, r0
        mtspr   TBU, r0
        mtspr   TSR, r1
        mtspr   TCR, r0
        mtspr   ESR, r0                 /* clear Exception Syndrome Reg */
        mtspr   XER, r0                 /*  clear Fixed-Point Exception Reg */
#ifdef AMP_CORE1
	lis   r1,0xffc0
	ori   r1,r1,0x0000
        mtspr IVPR, r1
#endif
        /*
         * TLB1 #0.  ROM - writethrough cached 0xff000000 -> 0xff000000.  
	 * 16MB
         * Attributes: SX/SW/SR **PROTECTED**
         */

        addis  r4,0,0x1000           /* TLBSEL = TLB1(CAM) , ESEL = 0 */ 
        ori    r4,r4,0x0000
        mtspr  MAS0, r4
        addis  r5,0,0xc000           /* V = 1, IPROT = 1, TID = 0*/
        ori    r5,r5,_MMU_TLB_SZ_16M  /* TS = 0, TSIZE = 64 MByte page size*/
        mtspr  MAS1, r5
        addis  r6,0,0xff00           /* EPN = 0xff000000*/
        ori    r6,r6,0x0016          /* WIMGE = 10110 */
        mtspr  MAS2, r6
        addis  r7,0,0xff00           /* RPN = 0xff000000*/
        ori    r7,r7,0x0015          /* Supervisor XWR*/
        mtspr  MAS3, r7
        tlbwe         
        tlbsync                     

	xor   r6, r6, r6
	msync
	isync
	mtspr L1CSR0, r6		/* Disable the Data cache */
        li   r6, 0x0002              
	msync
	isync
	mtspr L1CSR0, r6		/* Invalidate the Data cache */
        li    r6, 0x0000              
        msync
	isync
	mtspr L1CSR1, r6	 /* Disable the Instrunction cache */
        li   r6, 0x0002              
        msync
	isync
	mtspr L1CSR1, r6	/* Invalidate the Instruction cache */
        isync
        li   r6, 0x0000              
	msync
	isync
	mtspr L1CSR1, r6        /* temp disabled Enable the Instruction cache*/

        isync

        li	r7, 0x0001
	msync
	isync
	mtspr	L1CSR1, r7		/* enable the instruction cache */

		
	msync
	isync

		
	xor     r6,r6,r6
	xor     r7,r7,r7
	mullw   r7,r7,r6
        lis     sp, HI(STACK_ADRS)
        ori     sp, sp, LO(STACK_ADRS)
        addi    sp, sp, -FRAMEBASESZ
        lis     r6, HI(romInit)
        ori     r6, r6, LO(romInit)
        lis     r7, HI(romStart)
        ori     r7, r7, LO(romStart)
        lis     r8, HI(ROM_TEXT_ADRS)
        ori     r8, r8, LO(ROM_TEXT_ADRS)
	sub	r6, r7, r6		/* routine - entry point */
	add	r6, r6, r8 		/* + ROM base */
        mtspr   LR, r6

        mr      r3, r2
        blr 

FUNC_END(_romInit)
FUNC_END(romInit)


/***************************************************************************
*
* resetEntry - rom entry point
*
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
FUNC_LABEL(_resetEntry)

#if REV1
	lis  r0,0x2000
	mtspr 977,r0
	lis r2,0x1000
	mtspr MAS0,r2
	tlbre
	tlbwe
	isync
#endif	

	xor   r6, r6, r6
	msync
	isync
	mtspr L1CSR0, r6		/* Disable the Data cache */
        li   r6, 0x0002              
	msync
	isync
	mtspr L1CSR0, r6		/* Invalidate the Data cache */
        li    r6, 0x0000              
        msync
	isync
	mtspr L1CSR1, r6	 /* Disable the Instrunction cache */
        li   r6, 0x0002          
        msync
	isync
	mtspr L1CSR1, r6	/* Invalidate the Instruction cache */
        isync
        li   r6, 0x0000              
	msync
	isync
	mtspr L1CSR1, r6        /* temp disabled Enable the Instruction cache*/


        isync
        li	r7, 0x0000
	msync
	isync
	mtspr	L1CSR1, r7		/* enable the instruction cache */


	msync
	isync


        /* Clear SRR0, CSRR0, MCSRR0, SRR1, CSRR1 , MCSRR1, IVPR */
	
	xor   r0,r0,r0
	
        mtspr SRR0, r0
        mtspr SRR1, r0
        mtspr CSRR0, r0
        mtspr CSRR1, r0
        mtspr MCSRR0, r0
        mtspr MCSRR1, r0
        mtspr ESR, r0
        mtspr MCSR, r0
        mtspr DEAR, r0
        mtspr DBCR0, r0
        mtspr DBCR1, r0        
        mtspr DBCR2, r0
        mtspr IAC1, r0
        mtspr IAC2, r0
        mtspr DAC1, r0
        mtspr DAC2, r0
        
        mfspr r1, DBSR
        mtspr DBSR, r1
        
        mtspr PID0, r0
        mtspr PID1, r0
        mtspr PID2, r0
        mtspr TCR, r0
        mtspr 1013, r0

	mtspr MAS4, r0
	mtspr MAS6, r0
	isync

	lis   r1,0xfff0
	ori   r1,r1,0x0000
        mtspr IVPR, r1

        /* Set up vector offsets */
        addi  r3, r0, 0x100
        mtspr IVOR0, r3
        addi  r3, r0, 0x200
        mtspr IVOR1, r3
        addi  r3, r0, 0x300
        mtspr IVOR2, r3
        addi  r3, r0, 0x400
        mtspr IVOR3, r3
        addi  r3, r0, 0x500
        mtspr IVOR4, r3
        addi  r3, r0, 0x600
        mtspr IVOR5, r3
        addi  r3, r0, 0x700
        mtspr IVOR6, r3
        /* skipping IVOR7 0x800: no FPU on e500 */
        addi  r3, r0, 0x900
        mtspr IVOR8, r3
        /* skipping IVOR9 0xa00: no aux processor on e500 */
        addi  r3, r0, 0xb00
        mtspr IVOR10, r3
        addi  r3, r0, 0xc00
        mtspr IVOR11, r3
        addi  r3, r0, 0xd00
        mtspr IVOR12, r3
        addi  r3, r0, 0xe00
        mtspr IVOR13, r3
        addi  r3, r0, 0xf00
        mtspr IVOR14, r3
        addi  r3, r0, 0x1000
        mtspr IVOR15, r3
        addi  r3, r0, 0x1100       /* SPU is e500 specific */
        mtspr IVOR32, r3
        addi  r3, r0, 0x1200       /* FP data is e500 specific */
        mtspr IVOR33, r3
        addi  r3, r0, 0x1300       /* FP round is e500 specific */
        mtspr IVOR34, r3
        addi  r3, r0, 0x1400       /* perf mon is e500 specific */
        mtspr IVOR35, r3

	li    r2,0x1e
	mtspr MMUCSR0, r2
	isync
		
	li    r3,4
	li    r4,0
	tlbivax r4,r3
	nop

	
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
         * TLB1 #0.  ROM - non-cached 0xf0000000 -> 0xf00000000.  
	 * 256MB
         * Attributes: SX/SW/SR **PROTECTED**
         */

        addis  r4,0,0x1000           /* TLBSEL = TLB1(CAM) , ESEL = 0 */ 
        ori    r4,r4,0x0000
        mtspr  MAS0, r4
        addis  r5,0,0xC000           /* V = 1, IPROT = 1, TID = 0*/
        ori    r5,r5,_MMU_TLB_SZ_256M  /* TS = 0, TSIZE = 256 MByte page size*/
        mtspr  MAS1, r5
        addis  r6,0,0xf000           /* EPN = 0xf0000000*/
        ori    r6,r6,0x000a          /* WIMGE = 01010 */
        mtspr  MAS2, r6
        addis  r7,0,0xf000           /* RPN = 0xf0000000*/
        ori    r7,r7,0x0015          /* Supervisor XWR*/
        mtspr  MAS3, r7
        isync
        msync
        tlbwe         
        tlbsync                     

        /*
         * TLB1 #1.  Main SDRAM - Not Cached
	 *           LOCAL_MEM_LOCAL_ADRS -> LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE
         * Attributes: UX/UW/UR/SX/SW/SR
         */

        addis  r4,0,0x1001           /* TLBSEL = TLB1(CAM) , ESEL = 1*/
        ori    r4,r4,0x0000
        mtspr  MAS0, r4
        addis  r5,0,0xC000           /* V = 1, IPROT = 1, TID = 0*/
        ori    r5,r5,_MMU_TLB_SZ_1G  /* TS = 0, TSIZE = 256 MByte page size*/
        mtspr  MAS1, r5
        addis  r6,0,HI(PHYS_MEM_START) /* EPN = LOCAL_MEM_LOCAL_ADRS */
        ori    r6,r6,0x000a          /* WIMGE = 01010*/
        mtspr  MAS2, r6
        addis  r7,0,HI(PHYS_MEM_START)  /* RPN = LOCAL_MEM_LOCAL_ADRS */
        ori    r7,r7,0x003f          /* User/Supervisor XWR*/
        mtspr  MAS3, r7
        isync
        msync
        tlbwe                       
        tlbsync                      


	/*
         * TLB1 #3.  CCSBAR 
         * Attributes: UX/UW/UR/SX/SW/SR
         */

        addis  r4,0,0x1002          /* TLBSEL = TLB1(CAM) , ESEL = 2*/
        ori    r4,r4,0x0000
        mtspr  MAS0, r4
        addis  r5,0,0xC000           /* V = 1, IPROT = 0, TID = 0*/
        ori    r5,r5,_MMU_TLB_SZ_64M /* TS = 0, TSIZE = 64 MByte page size*/
        mtspr  MAS1, r5
        addis  r6,0,HI(CCSBAR) /* EPN = CCSBAR */
        ori    r6,r6,0x000a          /* WIMGE = 01010 */
        mtspr  MAS2, r6

        addis  r7,0,HI(CCSBAR) /* RPN = CCSBAR */
        ori    r7,r7,0x0015          /* Supervisor XWR*/
        mtspr  MAS3, r7
        isync
        msync
        tlbwe                       
        tlbsync                      

	mfspr	r6, HID1
	andi.   r6, r6, 1
	cmpwi   r6, 1	
	bne     doCcsrbar

	/* running on CPU1 */
	/* set up initial stack pointer */

	LOADVAR (sp, CPU1_STACK_START_ADR)
	stwu	r0, -FRAMEBASESZ(sp)		/* save one frame stack */

	/* jump to sysCpu1Loop for SMP - sysInit for AMP */
waitForStartSet:
	LOADVAR (r3, CPU1_INIT_START_ADR)
	/* Check that init start address is not zero before jumping 
	 * This allows cpu1 to reboot and wait for cpu0 to reload image */
	lis     r4,HI(MP_MAGIC_RBV)
	ori     r4, r4, LO(MP_MAGIC_RBV)
	cmpw    r3, r4
	beq     waitForStartSet
	mtctr	r3
	bctr

doCcsrbar:	
	/* Setup the memory mapped register address */

	isync
	sync

				
	lis     r6,HI(CCSBAR)
	ori     r6,r6, LO(CCSBAR)
	isync
		
	srwi    r5, r6, 12
	lis     r7,HI(CCSBAR_RESET)
	ori     r7,r7,LO(CCSBAR_RESET)
	lwz     r4, 0(r7)

	isync
	sync
	mbar 0
		
	stw     r5,0(r7)
	
	sync
	isync
	mbar 0

	lis    r5,0xffff
	ori    r5, r5,0xf800
	lwz    r4, 0 (r5)
	isync
			
	li     r4,0x2000	
	mtctr   r4
ccsrbarWait:
	nop
	bdnz    ccsrbarWait

			
	WRITEADR(r6,r7,M85XX_LAWBAR0(CCSBAR),0xf0000)
	 
	WRITEADR(r6,r7,M85XX_LAWAR0(CCSBAR), 
		 LAWAR_ENABLE | LAWAR_TGTIF_LBC | LAWAR_SIZE_256MB )
	 
	/* load BR0 */
	WRITEADR(r6,r7,M85XX_BR0 (CCSBAR),0xf8001001)
	/* load OR0 */
	WRITEADR(r6,r7,M85XX_OR0 (CCSBAR),0xf8000FF7)
	isync
	sync
	mbar 0
	
	/* Initialise the Local Bus Controller */
	li      r4,0x2000	
	mtctr   r4
	WRITEADR(r6,r7,M85XX_LCRR(CCSBAR),0x80000008) 
	
	isync
	sync
	mbar 0
dllDelay4:
	nop	
	bdnz    dllDelay4
#ifdef INCLUDE_LBC_CS3 /* PIXIS */

	/* PIXIS - registers */
	 
	/* load OR3 */

	WRITEADR(r6,r7,M85XX_OR3 (CCSBAR),(LBC_CS3_LOCAL_SIZE_MASK & 0xffff0000) | 0x0ff7)

	/* load BR3 */
	WRITEADR(r6,r7,M85XX_BR3 (CCSBAR), \
		 (LBC_CS3_LOCAL_ADRS & 0xffff0000) | \
		 0x0801)

        mbar 0

#endif /* INCLUDE_LBC_CS3 */
	
	
	
				
#ifdef INCLUDE_DDR_SDRAM
	/* Memory mapped region base address */
	
	WRITEADR(r6,r7,M85XX_LAWBAR1(CCSBAR), 
		 DDR_SDRAM_LOCAL_ADRS >> LAWBAR_ADRS_SHIFT)
	 
	WRITEADR(r6,r7,M85XX_LAWAR1(CCSBAR), 
		 LAWAR_ENABLE | LAWAR_TGTIF_DDRSDRAM | LAWAR_SIZE_512MB ) 

	WRITEADR(r6,r7,M85XX_LAWBAR2(CCSBAR), 
		 DDR_SDRAM_LOCAL_ADRS2 >> LAWBAR_ADRS_SHIFT)
	 
	WRITEADR(r6,r7,M85XX_LAWAR2(CCSBAR), 
		 LAWAR_ENABLE | LAWAR_TGTIF_DDRSDRAM2 | LAWAR_SIZE_512MB ) 

	isync

	lis	r7, HI(M85XX_PORPLLSR(CCSBAR))
	ori     r7, r7, LO(M85XX_PORPLLSR(CCSBAR))
	lwz     r7, 0(r7)
	andi.   r7, r7, 0x3e
	srwi    r7, r7, 1
	
	/* Get multiplier from table */
	lis     r8, HI(ROM_TEXT_ADRS)
	ori     r8, r8, LO(platpllTable)
	add     r8, r8, r7
	lbz     r8, 0(r8)

	cmpwi   r8,0 /* Test for unsupported freq */
	beq     checkStop  /* Jump to 0 */
	

	/* Initialize the DDR Memory controller */
        lis	r6, HI(DDRBA)
        ori	r6, r6, LO(DDRBA)		/* r6 = DDR base */
	
	WRITEOFFSET(r6,r7,(DDR_SDRAM_CLK_CTRL), 0x03800000)
	
	WRITEOFFSET(r6,r7,(CS0_BNDS), 0x0000001f)
	WRITEOFFSET(r6,r7,(CS1_BNDS), 0x00000000)
	WRITEOFFSET(r6,r7,(CS2_BNDS), 0x00000000)
	WRITEOFFSET(r6,r7,(CS3_BNDS), 0x00000000)
	WRITEOFFSET(r6,r7,(CS0_CONFIG), 0x80010202)
	WRITEOFFSET(r6,r7,(CS1_CONFIG), 0x00000000)
	WRITEOFFSET(r6,r7,(CS2_CONFIG), 0x00000000)
	WRITEOFFSET(r6,r7,(CS3_CONFIG), 0x00000000)
	WRITEOFFSET(r6,r7,(DDR_SDRAM_CLK_CTRL), 0x00800000)
	WRITEOFFSET(r6,r7,(DDR_DATA_INIT), 0x0000000)
	WRITEOFFSET(r6,r7,(DDR_SDRAM_MODE_CFG_2), 0x00000000)
	WRITEOFFSET(r6,r7,(DDR_SDRAM_MD_CNTL), 0x0)


	lis     r7, HIADJ(PIXIS_OSC(PIXIS_BASE))
	lbz     r9, LO(PIXIS_OSC(PIXIS_BASE))(r7)
	andi.   r9, r9, 7
	
	/* If value not that configured by default use slowest DDR settings */
	cmpwi   r9, PIXIS_OSC_33_MHZ
	beq     useBuildOscFreq
	cmpwi   r9, PIXIS_OSC_66_MHZ
	/* Unsupported oscillator clock assume worst case DDR settings reqd*/ 
	bne     defaultDDRInit 
	slwi    r8,r8,1 /* Adjust multiplier for definitions */
useBuildOscFreq:

		/* comparison assumes 66Mhz clk */
	cmpwi   r8, PLAT_RATIO_533_MHZ
	beq     defaultDDRInit
	cmpwi   r8, PLAT_RATIO_800_MHZ
	beq     defaultDDRInit

defaultDDRInit:	
		
	WRITEOFFSET(r6,r7,(EXTENDED_REF_REC), 0x00020000)
	WRITEOFFSET(r6,r7,(TIMING_CFG_0), 0x00260802)	
	WRITEOFFSET(r6,r7,(TIMING_CFG_1), 0x626B2634)
	WRITEOFFSET(r6,r7,(TIMING_CFG_2), 0x062874CF)
#ifdef INCLUDE_DDR_ECC
	WRITEOFFSET(r6,r7,(DDR_SDRAM_CFG), 0x63000008)
#else
	WRITEOFFSET(r6,r7,(DDR_SDRAM_CFG), 0x43000008)
#endif
	WRITEOFFSET(r6,r7,(DDR_SDRAM_CFG_2), 0x24400000)
	WRITEOFFSET(r6,r7,(DDR_SDRAM_MODE_CFG), 0x00440462)
	WRITEOFFSET(r6,r7,(DDR_SDRAM_INTERVAL), 0x0c300100)
			  
  	isync
	
	li      r4,0x2000	
	mtctr   r4
ddrDelay:
	nop
	bdnz    ddrDelay

#ifdef INCLUDE_DDR_ECC
	WRITEOFFSET(r6,r7,(DDR_SDRAM_CFG), 0xE3000008)
#else /* INCLUDE_DDR_ECC */
	WRITEOFFSET(r6,r7,(DDR_SDRAM_CFG), 0xC3000008)
#endif /* INCLUDE_DDR_ECC */
	
	isync
  	isync
	
	li      r4,0x2000	
	mtctr   r4
ddrDelay2:
	nop
	bdnz    ddrDelay2
	
	/* Memory mapped region base address */
	

	isync

	lis	r7, HI(M85XX_PORPLLSR(CCSBAR))
	ori     r7, r7, LO(M85XX_PORPLLSR(CCSBAR))
	lwz     r7, 0(r7)
	andi.   r7, r7, 0x3e
	srwi    r7, r7, 1
	
	/* Get multiplier from table */
	lis     r8, HI(ROM_TEXT_ADRS)
	ori     r8, r8, LO(platpllTable)
	add     r8, r8, r7
	lbz     r8, 0(r8)

	cmpwi   r8,0 /* Test for unsupported freq */
	beq     checkStop  /* Jump to 0 */
	

	/* Initialize the DDR Memory controller */
        lis	r6, HI(DDRBA2)
        ori	r6, r6, LO(DDRBA2)		/* r6 = DDR base */
	
	
	WRITEOFFSET(r6,r7,(CS0_BNDS), 0x0020003f)
	WRITEOFFSET(r6,r7,(CS1_BNDS), 0x00000000)
	WRITEOFFSET(r6,r7,(CS2_BNDS), 0x00000000)
	WRITEOFFSET(r6,r7,(CS3_BNDS), 0x00000000)
	WRITEOFFSET(r6,r7,(CS0_CONFIG), 0x80010202)
	WRITEOFFSET(r6,r7,(CS1_CONFIG), 0x00000000)
	WRITEOFFSET(r6,r7,(CS2_CONFIG), 0x00000000)
	WRITEOFFSET(r6,r7,(CS3_CONFIG), 0x00000000)
	WRITEOFFSET(r6,r7,(DDR_SDRAM_CLK_CTRL), 0x00800000)
	WRITEOFFSET(r6,r7,(DDR_DATA_INIT), 0x0000000)
	WRITEOFFSET(r6,r7,(DDR_SDRAM_MODE_CFG_2), 0x00000000)
	WRITEOFFSET(r6,r7,(DDR_SDRAM_MD_CNTL), 0x0)


	lis     r7, HIADJ(PIXIS_OSC(PIXIS_BASE))
	lbz     r9, LO(PIXIS_OSC(PIXIS_BASE))(r7)
	andi.   r9, r9, 7
	
	/* If value not that configured by default use slowest DDR settings */
	cmpwi   r9, PIXIS_OSC_33_MHZ
	beq     useBuildOscFreq
	cmpwi   r9, PIXIS_OSC_66_MHZ
	/* Unsupported oscillator clock assume worst case DDR settings reqd*/ 
	bne     defaultDDR2Init /* Probably 133Mhz */
	slwi    r8,r8,1 /* Adjust multiplier for definitions */
useBuildOscFreq2:

		/* comparison assumes 66Mhz clk */
	cmpwi   r8, PLAT_RATIO_533_MHZ
	beq     defaultDDR2Init
	cmpwi   r8, PLAT_RATIO_800_MHZ
	beq     defaultDDR2Init
defaultDDR2Init:	
		
	WRITEOFFSET(r6,r7,(EXTENDED_REF_REC), 0x00020000)
	WRITEOFFSET(r6,r7,(TIMING_CFG_0), 0x00260802)	
	WRITEOFFSET(r6,r7,(TIMING_CFG_1), 0x626B2634)
	WRITEOFFSET(r6,r7,(TIMING_CFG_2), 0x06a878CF)
#ifdef INCLUDE_DDR_ECC
	WRITEOFFSET(r6,r7,(DDR_SDRAM_CFG), 0x63000008)
#else
	WRITEOFFSET(r6,r7,(DDR_SDRAM_CFG), 0x43000008)
#endif
	WRITEOFFSET(r6,r7,(DDR_SDRAM_CFG_2), 0x24400000)
	WRITEOFFSET(r6,r7,(DDR_SDRAM_MODE_CFG), 0x00440462)
	WRITEOFFSET(r6,r7,(DDR_SDRAM_INTERVAL), 0x0c300100)

  	isync
	
	li      r4,0x2000	
	mtctr   r4
ddr2Delay:
	nop
	bdnz    ddr2Delay


#ifdef INCLUDE_DDR_ECC
	WRITEOFFSET(r6,r7,(DDR_SDRAM_CFG), 0xE3000008)
#else /* INCLUDE_DDR_ECC */
	WRITEOFFSET(r6,r7,(DDR_SDRAM_CFG), 0xC3000008)
#endif /* INCLUDE_DDR_ECC */
	
	isync
  	isync
	
	li      r4,0x2000	
	mtctr   r4
ddr2Delay2:
	nop
	bdnz    ddr2Delay2
	
#endif /* INCLUDE_DDR_SDRAM */

	/* Now that memory is stable we reset TLB entries for standard 
	 * operation 
	 */
        /*
         * TLB1 #0.  ROM - writethrough cached 0xff000000 -> 0xff000000.  
	 * 16MB
         * Attributes: SX/SW/SR **PROTECTED**
         */

        addis  r4,0,0x1000           /* TLBSEL = TLB1(CAM) , ESEL = 0 */ 
        ori    r4,r4,0x0000
        mtspr  MAS0, r4
        addis  r5,0,0xc000           /* V = 1, IPROT = 1, TID = 0*/
        ori    r5,r5,_MMU_TLB_SZ_16M  /* TS = 0, TSIZE = 64 MByte page size*/
        mtspr  MAS1, r5
        addis  r6,0,0xff00           /* EPN = 0xff000000*/
        ori    r6,r6,0x0016          /* WIMGE = 10110 */
        mtspr  MAS2, r6
        addis  r7,0,0xff00           /* RPN = 0xff000000*/
        ori    r7,r7,0x0015          /* Supervisor XWR*/
        mtspr  MAS3, r7
        tlbwe         
        tlbsync                     

        /*
         * TLB1 #1.  Main SDRAM - Cached with Coherence
	 *           LOCAL_MEM_LOCAL_ADRS -> LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE
         * Attributes: UX/UW/UR/SX/SW/SR
         */

        addis  r4,0,0x1001           /* TLBSEL = TLB1(CAM) , ESEL = 1*/
        ori    r4,r4,0x0000
        mtspr  MAS0, r4
        addis  r5,0,0xc000           /* V = 1, IPROT = 1, TID = 0*/
        ori    r5,r5,_MMU_TLB_SZ_1G  /* TS = 0, TSIZE = !GByte page size*/
        mtspr  MAS1, r5
        addis  r6,0,HI(PHYS_MEM_START) /* EPN = LOCAL_MEM_LOCAL_ADRS */
        ori    r6,r6,0x0000          /* WIMGE = 00000 */
        mtspr  MAS2, r6
        addis  r7,0,HI(PHYS_MEM_START)  /* RPN = LOCAL_MEM_LOCAL_ADRS */
        ori    r7,r7,0x003f          /* User/Supervisor XWR*/
        mtspr  MAS3, r7
        tlbwe                       
        tlbsync                              


        /*
         * TLB1 #2.  CCSRBAR - guarded 0xe0000000 -> 0xe0000000.  
	 * 16MB
         * Attributes: SX/SW/SR
         */

        addis  r4,0,0x1002           /* TLBSEL = TLB1(CAM) , ESEL = 0 */ 
        ori    r4,r4,0x0000
        mtspr  MAS0, r4
        addis  r5,0,0xc000           /* V = 1, IPROT = 0, TID = 0*/
        ori    r5,r5,_MMU_TLB_SZ_64M  /* TS = 0, TSIZE = 16 MByte page size*/
        mtspr  MAS1, r5
        addis  r6,0,HI(CCSBAR)           /* EPN = 0xe0000000*/
        ori    r6,r6,0x000a          /* WIMGE = 10110 */
        mtspr  MAS2, r6
        addis  r7,0,HI(CCSBAR)           /* RPN = 0xe0000000*/
        ori    r7,r7,0x003f          /* Supervisor XWR*/
        mtspr  MAS3, r7
        tlbwe         
        tlbsync            
	               
        b  cold
checkStop:	
	ba 0x0				
FUNC_END(resetEntry)

	
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

