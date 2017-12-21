/* romInit.s - ROM initialization module */

/*
 * Copyright (c) 1984-2004, 2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01o,04apr07,zl   fixed TLB size for ROM mapping
01n,13oct04,dtr  Mod to default to 256 MB DDR.
01m,10sep04,mil  Added romInitWarm and padded to offset 0x8 from romInit.
01l,03aug04,dtr  Add workaround for LBC errata #11.
01k,29jul04,dtr  Reset TLB to not guarded after DDR/LBC init.
	         Implement workaround for errata DDR#11.  
01j,22jul04,dtr  Improve boot time by enabling ICACHE.
01i,03feb04,dtr  Mod for DDR issue.
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

FUNC_EXPORT(romInit)
FUNC_EXPORT(_romInit)
	
FUNC_IMPORT(romStart)

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
FUNC_LABEL(_romInit)
	bl     resetEntry
	nop

FUNC_LABEL(_romInitWarm)
FUNC_LABEL(romInitWarm)
	bl     warm

        .ascii   "Copyright 1984-2002 Wind River Systems, Inc."
        .balign 4

cold:
	li    r2, BOOT_COLD
	b     start
	/* defines for memory initialization */
warm:
	mr     r2,r3

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
#if defined(INCLUDE_CACHE_SUPPORT) && defined(USER_I_CACHE_ENABLE)
        isync
        li	r7, 0x0001
	msync
	isync
	mtspr	L1CSR1, r7		/* enable the instruction cache */
#endif  /* INCLUDE_CACHE_SUPPORT && USER_I_CACHE_ENABLE */
	msync
	isync
#ifdef INCLUDE_PCI
        /* Set the pciAutoconfig check to FALSE */
	xor     r5,r5,r5                /* Zero r5 */
	lis     r6,HIADJ(PCI_AUTO_CONFIG_ADRS)                   
	addi	r6,r6,LO(PCI_AUTO_CONFIG_ADRS)
	stw     r5,0(r6)
#endif /* INCLUDE_PCI */
	
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

		
        /* Clear SRR0, CSRR0, MCSRR0, SRR1, CSRR1 , MCSRR1, IVPR */

	xor   r0,r0,r0
        mtspr SRR0, r0
        mtspr SRR1, r0
        mtspr CSRR0, r0
        mtspr CSRR1, r0
        mtspr MCSRR0, r0
        mtspr MCSRR1, r0
        mtspr IVPR, r0

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

        /* Initialize PID reg to match TID field of TLB entry (unless using TID=0) */

	xor   r0,r0,r0
	mtspr PID0 , r0
	mtspr PID1 , r0
	/*mtspr PID2 , r0 comment out for now due to suspected errata*/

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
         * TLB1 #0.  ROM/CCSBAR - non-cached 0xf0000000 -> 0xf00000000.  
	 * 256MB
         * Attributes: SX/SW/SR **PROTECTED**
         */

        addis  r4,0,0x1000           /* TLBSEL = TLB1(CAM) , ESEL = 0 */ 
        ori    r4,r4,0x0000
        mtspr  MAS0, r4
        addis  r5,0,0xc000           /* V = 1, IPROT = 1, TID = 0*/
        ori    r5,r5,_MMU_TLB_SZ_256M  /* TS = 0, TSIZE = 256 MByte page size*/
        mtspr  MAS1, r5
        addis  r6,0,0xf000           /* EPN = 0xf0000000*/
        ori    r6,r6,0x000a          /* WIMGE = 01010 */
        mtspr  MAS2, r6
        addis  r7,0,0xf000           /* RPN = 0xf0000000*/
        ori    r7,r7,0x0015          /* Supervisor XWR*/
        mtspr  MAS3, r7
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
        addis  r5,0,0xc000           /* V = 1, IPROT = 1, TID = 0*/
        ori    r5,r5,_MMU_TLB_SZ_256M  /* TS = 0, TSIZE = 256 MByte page size*/
        mtspr  MAS1, r5
        addis  r6,0,HI(LOCAL_MEM_LOCAL_ADRS) /* EPN = LOCAL_MEM_LOCAL_ADRS */
        ori    r6,r6,0x000a          /* WIMGE = 01010*/
        mtspr  MAS2, r6
        addis  r7,0,HI(LOCAL_MEM_LOCAL_ADRS)  /* RPN = LOCAL_MEM_LOCAL_ADRS */
        ori    r7,r7,0x003f          /* User/Supervisor XWR*/
        mtspr  MAS3, r7
        tlbwe                       
        tlbsync                      
	
        /*
         * TLB1 #2.  Additional SDRAM - not cached 
	 *           LOCAL_MEM_LOCAL_ADRS2 -> LOCAL_MEM_LOCAL_ADRS2 + LOCAL_MEM_SIZE
         * Attributes: UX/UW/UR/SX/SW/SR
         */

        addis  r4,0,0x1002          /* TLBSEL = TLB1(CAM) , ESEL = 2*/
        ori    r4,r4,0x0000
        mtspr  MAS0, r4
        addis  r5,0,0x8000           /* V = 1, IPROT = 0, TID = 0*/
        ori    r5,r5,_MMU_TLB_SZ_64M /* TS = 0, TSIZE = 64 MByte page size*/
        mtspr  MAS1, r5
        addis  r6,0,HI(LOCAL_MEM_LOCAL_ADRS2) /* EPN = LOCAL_MEM_LOCAL_ADRS2 */
        ori    r6,r6,0x000a          /* WIMGE = 01010 */
        mtspr  MAS2, r6
        addis  r7,0,HI(LOCAL_MEM_LOCAL_ADRS2) /* RPN = LOCAL_MEM_LOCAL_ADRS2 */
        ori    r7,r7,0x0015          /* Supervisor XWR*/
        mtspr  MAS3, r7
        tlbwe                       
        tlbsync                      
	
	/* Setup the memory mapped register address */
	WRITEADR(r6,r7,0xFF700000,(CCSBAR >> 12)) 
	sync
	isync
	
	/* Initialise the Local Bus Controller */

	WRITEADR(r6,r7,M85XX_LCRR(CCSBAR),0x10000004) 
	WRITEADR(r6,r7,M85XX_LCRR(CCSBAR),0x00030004)
	
	/* Set override for DDR */ 
	WRITEADR(r6,r7,M85XX_DDRDLLCR(CCSBAR),0x81000000) 

/* Store DLL setting and override  so as to avoid glitches in local bus 
   due to errata. Start with delay to allow DLL to stabilize ~ 200us 
   for 1GHz */
	lis     r6, 0x3
	mtctr   r6
delayLoop:
	bdnz    delayLoop

	/* Read LBC DLL and get tap reading */	
        lis     r6, HIADJ(M85XX_LBCDLLSR(CCSBAR))
        addi    r6, r6, LO(M85XX_LBCDLLSR(CCSBAR))
        lwz     r3, 0(r6)
	andi.   r3, r3, 0x7ff

	/* Start loop which terminates after ten cycles */
	li      r4, 11
retestLbcDll:
	addi    r4, r4, -1
	cmpwi   r4, 0

	/* terminate loop and generate check stop */
	beq     checkStop   
	/* Delay not more than 2500 CCB clock cycles */ 
	li      r5, 2000
	mtctr   r5
lbcDllDelay:
	bdnz    lbcDllDelay

	/* Read latest tap value */
        lwz     r8, 0(r6)
	andi.   r5, r8, 0x7ff

	/* diff between previous value */
	subf    r7, r5, r3
	/* Move latest value to previous */
	mr      r3, r5
	
	/* Check difference is not greater than 10 */	
	cmpwi   r7, 10
	bgt     retestLbcDll
	cmpwi   r7, -10
	blt     retestLbcDll

	/* Re-read dll reg and lock */
	lwz     r3, 0(r6)
        slwi    r3,r3,16
	lis     r5,0x8000
	or      r3,r3,r5
	stw     r3, 0(r6)

        isync
	sync
	mbar 0
	li      r4, 0x1
retestDll:	
	/* Read DDR DLL and test for override complete */	
        lis     r6, HI(M85XX_DDRDLLCR(CCSBAR))
        ori     r6, r6, LO(M85XX_DDRDLLCR(CCSBAR))
        lwz     r3, 0(r6)
	lis     r7, HI(0x81000100)
	ori     r7, r7 ,LO(0x81000100)
	cmpw    r7, r3
	beq     dllDone
	/* if override not complete reset DDR device and try again */
	lis     r6, HI(M85XX_DEVDISR(CCSBAR))
	ori     r6, r6, LO(M85XX_DEVDISR(CCSBAR))
	lwz     r3, 0(r6)
	oris    r3, r3, (M85XX_DEVDISR_DDR >> 16)
	stw     r3, 0(r6)
	mtctr   r4
dllDelay:
	nop
	bdnz    dllDelay
	lis     r6, HI(M85XX_DEVDISR(CCSBAR))
	ori     r6, r6, LO(M85XX_DEVDISR(CCSBAR))
	lwz     r7, 0(r6)
	lis	r8, HI(~M85XX_DEVDISR_DDR)
	ori	r8, r8, LO(~M85XX_DEVDISR_DDR)
	and     r7, r7, r8
	stw     r7, 0(r6)
	addi    r4, r4, 1
	b retestDll
dllDone:	
#ifdef INCLUDE_DDR_SDRAM
	/* Memory mapped region base address */
	
	WRITEADR(r6,r7,M85XX_LAWBAR0(CCSBAR), 
		 DDR_SDRAM_LOCAL_ADRS >> LAWBAR_ADRS_SHIFT)
	 
	WRITEADR(r6,r7,M85XX_LAWAR0(CCSBAR), 
		 LAWAR_ENABLE | LAWAR_TGTIF_DDRSDRAM | LAWAR_SIZE_256MB ) 

        mbar 0
		
	/* Initialize the DDR Memory controller */
        lis	r6, HI(DDRBA)
        ori	r6, r6, LO(DDRBA)		/* r6 = DDR base */
	WRITEOFFSET(r6,r7,(CS0_BNDS), 0x0000000f)
	WRITEOFFSET(r6,r7,(CS1_BNDS), 0x00000000)
	WRITEOFFSET(r6,r7,(CS2_BNDS), 0x00000000)
	WRITEOFFSET(r6,r7,(CS3_BNDS), 0x00000000)
	WRITEOFFSET(r6,r7,(CS0_CONFIG), 0x80000102)
	WRITEOFFSET(r6,r7,(CS1_CONFIG), 0x00000000)
	WRITEOFFSET(r6,r7,(CS2_CONFIG), 0x00000000)
	WRITEOFFSET(r6,r7,(CS3_CONFIG), 0x00000000)
	
	WRITEOFFSET(r6,r7,(TIMING_CFG_1), 0x36343321)
	WRITEOFFSET(r6,r7,(TIMING_CFG_2), 0x00000800)
	WRITEOFFSET(r6,r7,(DDR_SDRAM_CFG), 0x42000000)
	WRITEOFFSET(r6,r7,(DDR_SDRAM_MODE_CFG), 0x00000062)
	WRITEOFFSET(r6,r7,(DDR_SDRAM_INTERVAL), 0x045b0100)
	WRITEOFFSET(r6,r7,(DDR_SDRAM_CFG), 0xc2000000)

#ifdef INCLUDE_ECC
	WRITEOFFSET(r6,r7,(DDR_ERR_DISABLE), 0x00000000)
	WRITEOFFSET(r6,r7,(DDR_ERR_SBE), 0x00ff0000)
	isync
	sync
	mbar 0
	lis     r7, 0x1
	mtspr   9,  r7
eccDelayLoop:
	nop
	bdnz    eccDelayLoop
	WRITEOFFSET(r6,r7,(DDR_SDRAM_CFG), 0xe2000000)
#endif
	isync
	sync
	mbar 0
	
#endif /* INCLUDE_DDR_SDRAM */
	
	
#ifdef ADS_PROTOTYPE
	/* 16 bit flash */
	WRITEADR(r6,r7,M85XX_BR0(CCSBAR),FLASH_BASE_ADRS|0x01001 ) 
#else /* ADS_PROTOTYPE */
	/* 2 * 16 bit */
	WRITEADR(r6,r7,M85XX_BR0(CCSBAR),FLASH_BASE_ADRS|0x01801 )
#endif /* ADS_PROTOTYPE */

	WRITEADR(r6,r7,M85XX_OR0(CCSBAR),FLASH_ADRS_MASK|0x6e61 )

#ifdef INCLUDE_LBC_SDRAM
	/* Initialise SDRAM */
SdramInit:
	/* Memory mapped region base address */
	
	WRITEADR(r6,r7,M85XX_LAWBAR1(CCSBAR), \
	         (LBC_SDRAM_LOCAL_ADRS >> LAWBAR_ADRS_SHIFT) )

	WRITEADR(r6,r7,M85XX_LAWAR1(CCSBAR),
		 (LAWAR_ENABLE | \
	          LAWAR_TGTIF_LBC | \
		  LAWAR_SIZE_64MB ))
	
        mbar 0
 
	/* load OR2 */

	WRITEADR(r6,r7,M85XX_OR2 (CCSBAR),(LBC_SDRAM_LOCAL_SIZE_MASK & 0xffff0000) | 0x6901)

	/* load BR2 */
	WRITEADR(r6,r7,M85XX_BR2 (CCSBAR), \
		 (LBC_SDRAM_LOCAL_ADRS & 0xffff0000) | \
		 0x1861)

	/* Pre-charge all banks */
	WRITEADR(r6,r7,M85XX_LSDMR(CCSBAR),0x2863B723 )
 
	lis     r9,HIADJ(LBC_SDRAM_LOCAL_ADRS)
        addi    r9, r9, LO(LBC_SDRAM_LOCAL_ADRS)
 
	/* do a single write to an arbitrary location */

	addi    r5,0,0x00FF      /* Load 0x000000FF into r5 */
	stb     r5,0(r9)         /* Write 0xFF to SDRAM address - bits [24-31] */
	mbar 0
	/* issue a "Auto Refresh" command to SDRAM */

	WRITEADR(r6,r7,M85XX_LSDMR(CCSBAR),0x0863B723)
 
	/* do a single write to an arbitrary location */

	addi    r5,0,0x00FF      /* Load 0x000000FF into r5 */
	stb     r5,0(r9)         /* Write 0xFF to SDRAM address - bits [24-31] */
	mbar 0
	/* issue a "Auto Refresh" command to SDRAM */

	WRITEADR(r6,r7,M85XX_LSDMR(CCSBAR),0x0863B723)
 
	/* do a single write to an arbitrary location */

	addi    r8,0,0x00FF      /* Load 0x000000FF into r8 */

	stb     r8,0(r9)         /* Write 0xFF to address R9  */
	stb     r8,1(r9)         /* Write 0xFF to address R9  */
	stb     r8,2(r9)         /* Write 0xFF to address R9  */
	stb     r8,3(r9)         /* Write 0xFF to address R9  */
	stb     r8,4(r9)         /* Write 0xFF to address R9  */
	stb     r8,5(r9)         /* Write 0xFF to address R9  */
	stb     r8,6(r9)         /* Write 0xFF to address R9  */
	stb     r8,7(r9)         /* Write 0xFF to address R9  */
	
	/* issue a "Mode Register Write" command to SDRAM */

	WRITEADR(r6,r7,M85XX_LSDMR(CCSBAR),0x1863B723)
 
	/* do a single write to an arbitrary location */

	addi    r8,0,0x00FF      /* Load 0x000000FF into r8 */
	stb     r8,0(r9)         /* Write 0xFF to address R9 - bits [24-31] */

	/* enable refresh services and put SDRAM into normal operation  */
	
	WRITEADR(r6,r7,M85XX_LSDMR(CCSBAR),0x4063B723)

	/* program the MRTPR */

	addi    r5,0,TPR	 /* MRTPR[TPR] */
        lis     r6, HIADJ (M85XX_MRTPR (CCSBAR))
        addi    r6, r6, LO (M85XX_MRTPR (CCSBAR))
	sth     r5, 0x0 (r6)      /* store upper half-word */

	/* program the LSRT */

	addi    r5,0,LSRT_VALUE
        lis     r6, HIADJ (M85XX_LSRT (CCSBAR))
        addi    r6, r6, LO (M85XX_LSRT (CCSBAR))
	stb     r5, 0x0 (r6)      /* store byte - bits[24-31] */

	mbar 0

	lis     r9, HI(LBC_SDRAM_LOCAL_ADRS)
	ori     r9,r9, LO(LBC_SDRAM_LOCAL_ADRS)

	lis	r7, HIADJ(0x100)      /* Loop 256 times */
	addi    r7, r7, LO(0x100)
	mtspr	9,r7             /* Load spr CTR with 8 */
	lis 	r8,0x5555      /* Load 0x000000FF into r8 */
        ori     r8,r8,0x5555

SdramWrLoop2:
	
	stw  	r8,0(r9)        	/* Write 0xFF to address R9 */
	addi    r9,r9,4                 /* Move R9 to next byte */
	addi    r8,r8,1                 /* Add 1 to r8 */
	bc   	16,0,SdramWrLoop2	/* Decrement CTR, and possibly branch */
#endif  /* INCLUDE_LBC_SDRAM */

	/* Now that memory is stable we reset TLB entries for standard 
	 * operation 
	 */
	
        /*
         * TLB1 #0.  ROM - cached writethrough 0xff000000 -> 0xffffffff.
	 * 16MB
         * Attributes: SX/SW/SR **PROTECTED**
         */

        addis  r4,0,0x1000           /* TLBSEL = TLB1(CAM) , ESEL = 0 */ 
        ori    r4,r4,0x0000
        mtspr  MAS0, r4
        addis  r5,0,0xc000           /* V = 1, IPROT = 1, TID = 0*/
        ori    r5,r5,_MMU_TLB_SZ_16M  /* TS = 0, TSIZE = 16 MByte page size*/
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
         * TLB1 #1.  Main SDRAM - Cached
	 *           LOCAL_MEM_LOCAL_ADRS -> LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE
         * Attributes: UX/UW/UR/SX/SW/SR
         */

        addis  r4,0,0x1001           /* TLBSEL = TLB1(CAM) , ESEL = 1*/
        ori    r4,r4,0x0000
        mtspr  MAS0, r4
        addis  r5,0,0xc000           /* V = 1, IPROT = 1, TID = 0*/
        ori    r5,r5,_MMU_TLB_SZ_256M  /* TS = 0, TSIZE = 256 MByte page size*/
        mtspr  MAS1, r5
        addis  r6,0,HI(LOCAL_MEM_LOCAL_ADRS) /* EPN = LOCAL_MEM_LOCAL_ADRS */
        ori    r6,r6,0x0000          /* WIMGE = 00000 */
        mtspr  MAS2, r6
        addis  r7,0,HI(LOCAL_MEM_LOCAL_ADRS)  /* RPN = LOCAL_MEM_LOCAL_ADRS */
        ori    r7,r7,0x003f          /* User/Supervisor XWR*/
        mtspr  MAS3, r7
        tlbwe                       
        tlbsync                              

        /*
         * TLB1 #2.  CCSRBAR - guarded 0xfe000000 -> 0xfeffffff.
	 * 16MB
         * Attributes: SX/SW/SR
         */

        addis  r4,0,0x1002           /* TLBSEL = TLB1(CAM) , ESEL = 0 */ 
        ori    r4,r4,0x0000
        mtspr  MAS0, r4
        addis  r5,0,0x8000           /* V = 1, IPROT = 0, TID = 0*/
        ori    r5,r5,_MMU_TLB_SZ_16M  /* TS = 0, TSIZE = 16 MByte page size*/
        mtspr  MAS1, r5
        addis  r6,0,HI(CCSBAR)           /* EPN = 0xfe000000*/
        ori    r6,r6,0x0016          /* WIMGE = 10110 */
        mtspr  MAS2, r6
        addis  r7,0,HI(CCSBAR)           /* RPN = 0xfe000000*/
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

