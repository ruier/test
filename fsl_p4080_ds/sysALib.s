/* sysALib.s - Assembly support file and init routines */

/*
 * Copyright (c) 2008-2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01y,21aug12,l_z  Better implementation for sysPCGet. (WIND00076160)
01x,06jun12,c_l  add function sysFslErrataCcfA004510AndA004511.
                 (WIND00352077) (WIND00349563)
01w,11mar12,syt  updated sysSmpTimeBaseSet() to remove the TBEN bit
                 operation for HID0. (WIND00282419)
01v,15feb12,x_s  correct comments for cache operation. (WIND00324119)
01u,07feb12,x_s  added function sysFslErrataFpuA003999. (WIND00331187)
01t,04nov11,ers  WIND00297094: cache fix for vmPageLock/Unlock.
01s,13sep11,ers  Replaced DYNAMIC_WRLOAD with generic wrload support.
01r,03mar11,pch  CQ247720: remove unused sysIvprSet reference
01q,27mar10,pmr  add L2/L3 error register routines
01p,08mar10,pmr  add sysBoardReset()
01o,01mar10,pmr  WIND00201922: fix TLB clear
01n,05jan10,pmr  SMP M-N; virtual SM region for 36-bit
01m,22oct09,pmr  temporarily hardcode wait so sfdiab can build
01l,16oct09,pmr  doze with wait, correct TLB setup for UBOOT, TLB clear
01k,08oct09,pmr  remove tlbsync, sysCpu[Set]Start/Stop
01j,28sep09,pmr  remove USE_36_BIT
01i,23sep09,pmr  doze with wait instead of DOZ, remove extra EXPORT
01h,12aug09,pmr  remove PIXIS TLB, add vxL2CFG0Get
01g,19jun09,pmr  36-bit addressing
01f,19jan09,pmr  gcc compatibility
01e,01dec08,pmr  L2 cache, external proxy register
01d,03sep08,pmr  modifications for AMP, hard float
01c,29aug08,kab  Change _WRS_VX_SMP to _WRS_CONFIG_SMP
01b,30may08,pmr  mods for AMP
01a,24apr08,pmr  adapted from ads8572 version 01e.
*/

#define _ASMLANGUAGE
#include <vxWorks.h>
#include <vsbConfig.h>
#include <asm.h>
#include <config.h>
#include <sysLib.h>
#include <sysCache.h>
#include <arch/ppc/mmuE500Lib.h>

	FUNC_EXPORT(sysInit)
	FUNC_EXPORT(_sysInit)
	FUNC_EXPORT(vxL2CSR0Set)
	FUNC_EXPORT(vxL2CSR0Get)
	FUNC_EXPORT(vxL2CFG0Get)
	FUNC_EXPORT(vxL2ERRINJCTLSet)
	FUNC_EXPORT(vxL2ERRINJCTLGet)
	FUNC_EXPORT(vxL2ERRINTENSet)
	FUNC_EXPORT(vxL2ERRINTENGet)
	FUNC_EXPORT(vxL2ERRDETGet)
	FUNC_EXPORT(vxL2ERRDETSet)
	FUNC_EXPORT(vxL2ERRDISGet)
	FUNC_EXPORT(vxL2ERRDISSet)
	FUNC_EXPORT(vxL2ERRCTLGet)
	FUNC_EXPORT(vxL2ERRCTLSet)
	FUNC_EXPORT(sysICacheLock)
	FUNC_EXPORT(sysICacheUnlock)
	FUNC_EXPORT(sysCacheFlush)
	FUNC_EXPORT(sysCacheEnable)
	FUNC_IMPORT(usrInit)
	FUNC_EXPORT(sysInWord)
	FUNC_EXPORT(sysOutWord)
	FUNC_EXPORT(sysInLong)
	FUNC_EXPORT(sysOutLong)
    FUNC_EXPORT(sysInByte)
	FUNC_EXPORT(sysOutByte)
	FUNC_EXPORT(sysPciRead32)
	FUNC_EXPORT(sysPciWrite32)
	FUNC_EXPORT(sysPciInByte)
	FUNC_EXPORT(sysPciOutByte)
	FUNC_EXPORT(sysPciInWord)
	FUNC_EXPORT(sysPciOutWord)
	FUNC_EXPORT(sysPciInLong)
	FUNC_EXPORT(sysPciOutLong)
    FUNC_EXPORT(sysPCGet)   /* get the value of the PC register */
    FUNC_EXPORT(sysTimeBaseLGet)
	FUNC_EXPORT(disableBranchPrediction)
	FUNC_EXPORT(sysTas)

	FUNC_EXPORT(sysEprGet)
	FUNC_EXPORT(sysDoze)
	FUNC_EXPORT(sysDozeCpu)

#if defined(_WRS_CONFIG_SMP)
	FUNC_EXPORT(sysSmpTimeBaseSet)
#endif	/* _WRS_CONFIG_SMP */

	FUNC_EXPORT(sysCpcRegGet)
	FUNC_EXPORT(sysCpcRegSet)

	FUNC_EXPORT(sysCpuIdGet)

	FUNC_IMPORT(sysCpuLoop)

	FUNC_EXPORT(sysLpidSet)
	FUNC_EXPORT(sysSelfReset)
	FUNC_EXPORT(sysBoardReset)

#ifdef FSL_ERRATA_FPU_A003999
	FUNC_EXPORT(sysFslErrataFpuA003999)
#endif /* FSL_ERRATA_FPU_A003999 */

#if defined(FSL_ERRATA_CCF_A004510) || defined(FSL_ERRATA_CCF_A004511)
    FUNC_EXPORT(sysFslErrataCcfA004510AndA004511)
#endif /* FSL_ERRATA_CCF_A004510 || FSL_ERRATA_CCF_A004511 */

#ifdef INCLUDE_VIRTUAL_SM
        FUNC_EXPORT(sysVirtualSmTlb)
#endif

#define CPU_ID_GET(reg) \
	addis	reg, 0, HI(P4080_EPICWHOAMI(CCSBAR)) ; \
	lwz 	reg, LO(P4080_EPICWHOAMI(CCSBAR))(reg) ;

#define TLB_WRITE(esel,reg,size,epn,wimge,rpn,perm,erpn) 	\
        mtspr  MAS0, esel ;					\
        addis  reg,0,0xC000 ; /* valid and iprot */		\
        ori    reg,reg,size ;					\
        mtspr  MAS1,reg ;					\
        addis  reg,0,HI(epn) ;					\
        ori    reg,reg,wimge ;					\
        mtspr  MAS2,reg ; 					\
        addis  reg,0,HI(rpn) ;					\
        ori    reg,reg,perm ;					\
        mtspr  MAS3,reg ;					\
	li     reg,erpn ;					\
        mtspr  MAS7,reg ;					\
        isync ;							\
        msync ;							\
        tlbwe ;							\
        isync ;							\
        msync ;							\

	_WRS_TEXT_SEG_START
        DATA_IMPORT(inFullVxWorksImage)

#define	CACHE_ALIGN_SHIFT	6	/* Cache line size == 2**5 */

FUNC_LABEL(_sysInit)
FUNC_BEGIN(sysInit)

	mr  r8, r3

        /* turn off exceptions */
        mfmsr   r3                      /* r3 = msr              */
        INT_MASK (r3, r4)               /* mask EE and CE bit    */
        rlwinm  r4, r4, 0, 20, 18       /* turn off _PPC_MSR_ME  */
        mtmsr   r4                      /* msr = r4              */
        isync

/*
 * Per e500 core RM Table 2-42 : msync,isync; mtspr l1csr0; isync
 *                                            mtspr l1csr1; isync
 * Code must to wait until D-cache invalidate is done before enabling it.
 * Lock bits should be cleared during invalidation.
 */

#if defined(INCLUDE_WRLOAD_IMAGE_BUILD) || defined(UBOOT)
	xor   r6, r6, r6

	msync
	isync
	mtspr L1CSR0, r6	/* Disable the Data cache */
	isync                   /* Per e500 core RM Table 2-42 */

        li   r6, 0x0002

	msync
	isync
	mtspr L1CSR0, r6	/* Invalidate the Data cache */
	isync                   /* Per e500 core RM Table 2-42 */

        li    r6, 0x0000
	mtspr L1CSR1, r6	/* Disable the Instrunction cache */
	isync                   /* Per e500 core RM Table 2-42 */

        li   r6, 0x0002
	mtspr L1CSR1, r6	/* Invalidate the Instruction cache */
	isync                   /* Per e500 core RM Table 2-42 */

        li   r6, 0x0001
	mtspr L1CSR1, r6        /* Enable the Instruction cache*/
	isync                   /* Per e500 core RM Table 2-42 */

#ifdef INCLUDE_HW_FP

	/*
	 * Set MPU/MSR to a known state
	 * Turn on FP
	 */

	andi.	r3, r3, 0
	ori	r3, r3, 0x2000
	sync
	mtmsr	r3
	isync
	sync

	/* Init the floating point control/status register */

	mtfsfi	7, 0x0
	mtfsfi	6, 0x0
	mtfsfi	5, 0x0
	mtfsfi	4, 0x0
	mtfsfi	3, 0x0
	mtfsfi	2, 0x0
	mtfsfi	1, 0x0
	mtfsfi	0, 0x0
	isync

	/* Initialize the floating point data registers to a known state */

	bl	ifpdrValue
	.long	0x3f800000	/* 1.0 */
ifpdrValue:
	mfspr	r3, LR
	lfs	0, 0(r3)
	lfs	1, 0(r3)
	lfs	2, 0(r3)
	lfs	3, 0(r3)
	lfs	4, 0(r3)
	lfs	5, 0(r3)
	lfs	6, 0(r3)
	lfs	7, 0(r3)
	lfs	8, 0(r3)
	lfs	9, 0(r3)
	lfs	10, 0(r3)
	lfs	11, 0(r3)
	lfs	12, 0(r3)
	lfs	13, 0(r3)
	lfs	14, 0(r3)
	lfs	15, 0(r3)
	lfs	16, 0(r3)
	lfs	17, 0(r3)
	lfs	18, 0(r3)
	lfs	19, 0(r3)
	lfs	20, 0(r3)
	lfs	21, 0(r3)
	lfs	22, 0(r3)
	lfs	23, 0(r3)
	lfs	24, 0(r3)
	lfs	25, 0(r3)
	lfs	26, 0(r3)
	lfs	27, 0(r3)
	lfs	28, 0(r3)
	lfs	29, 0(r3)
	lfs	30, 0(r3)
	lfs	31, 0(r3)
	sync

	/*
	 * Set MPU/MSR to a known state
	 * Turn off FP
	 */

	andi.	r3, r3, 0
	sync
	mtmsr	r3
	isync
	sync

#endif /* INCLUDE_HW_FP */

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
#if 0
        mtspr PID1, r0
        mtspr PID2, r0
#endif
        mtspr TCR, r0
        mtspr 1013, r0

	mtspr MAS4, r0
	mtspr MAS6, r0
	isync

	lis   r1,HI(VEC_BASE_ADRS)
	ori   r1,r1,LO(VEC_BASE_ADRS)
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
        addi  r3, r0, 0x800
        mtspr IVOR7, r3
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
        addi  r3, r0, 0x1400       /* perf mon is e500 specific */
        mtspr IVOR35, r3

	li    r2,6
	mtspr MMUCSR0, r2
	isync

	li    r3,4
	li    r4,0
        isync                /* Per E500 CoreRM Rev J tbl 3-2 */
	tlbivax r4,r3
        msync                /* Per E500 CoreRM Rev J tbl 3-2 */
        isync                /* Per E500 CoreRM Rev J tbl 3-2 */
	nop

/*
 * If we're building for U-Boot, start with CAM1.
 * The VxWorks bootrom sets up the TLBS when not using U-BOOT
 */

#ifdef UBOOT
        addis  r4,0,0x1000           /* TLBSEL = TLB1(CAM) , ESEL = 0*/
        ori    r4,r4,0x0000

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
         * TLB1 #0/1.  Main SDRAM - Not Cached
	 *           PHYS_MEM_START -> PHYS_MEM_START + 1 GB
         * Attributes: UX/UW/UR/SX/SW/SR
         */

        TLB_WRITE(r4,r5,_MMU_TLB_SZ_1G,PHYS_MEM_START,0x000a,PHYS_MEM_START,0x003f,LOCAL_MEM_ERPN)

	/*
         * TLB1 #1/2.  CCSBAR
         * Attributes: UX/UW/UR/SX/SW/SR
         */

	addis  r4, r4, 1
        TLB_WRITE(r4,r5,_MMU_TLB_SZ_1M,CCSBAR,0x000a,CCSBAR,0x0015,0)

#ifdef INCLUDE_VIRTUAL_SM
	/*
         * TLB1 #2/3.  virtual SM
         * Attributes: UX/UW/UR/SX/SW/SR
         */

	addis  r4, r4, 1
        TLB_WRITE(r4,r5,_MMU_TLB_SZ_16M,VIRTUAL_SM_BASE,0x000a,VIRTUAL_SM_BASE_PHYS,0x003f,0)

#endif /* INCLUDE_VIRTUAL_SM */

/* calculate r7 based upon value of r4 and # of TLB CAM entries */

	rlwinm r7, r4, 16, 26, 31	/* r7 = last CAM index */

        neg    r7, r7
	addi   r7, r7, 63		/* 64 entries - (last used index + 1) */

	mtctr  r7
clearTLBs:
        addis  r4,r4,0x0001
        mtspr  MAS0, r4
        lis    r5,0x0000           /* V = 1, IPROT = 1, TID = 0*/
        mtspr  MAS1, r5
	isync
        msync
        tlbwe
        isync
        msync

	bdnz    clearTLBs
#endif /* UBOOT */

	mr      r3, r8

        /* write physical CPU index */
	CPU_ID_GET(r6)
	mtspr	PIR, r6

	/* Memory etc already setup jump to alternate boot image */
        lis     r6, HI(usrInit)
        ori     r6, r6, LO(usrInit)
	mtctr	r6

	lis     sp, HIADJ(RAM_LOW_ADRS)
	addi    sp, sp, LO(RAM_LOW_ADRS)
	addi    sp, sp, -FRAMEBASESZ    /* get frame stack */
	bctr
_sysInitEnd:
	.space (0xffc - (_sysInitEnd-_sysInit))
_resetVector:
	b sysInit
#else   /* INCLUDE_WRLOAD_IMAGE_BUILD || UBOOT */

        xor p0,p0,p0
	mtspr TCR,p0
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
	mtspr L1CSR1, r6        /* Disable the Instruction cache*/
	msync
	isync

        mbar 0

#ifdef INCLUDE_HW_FP

	/*
	 * Set MPU/MSR to a known state
	 * Turn on FP
	 */

	andi.	r3, r3, 0
	ori	r3, r3, 0x2000
	sync
	mtmsr	r3
	isync
	sync

	/* Init the floating point control/status register */

	mtfsfi	7, 0x0
	mtfsfi	6, 0x0
	mtfsfi	5, 0x0
	mtfsfi	4, 0x0
	mtfsfi	3, 0x0
	mtfsfi	2, 0x0
	mtfsfi	1, 0x0
	mtfsfi	0, 0x0
	isync

	/* Initialize the floating point data registers to a known state */

	bl	ifpdrValue
	.long	0x3f800000	/* 1.0 */
ifpdrValue:
	mfspr	r3, LR
	lfs	0, 0(r3)
	lfs	1, 0(r3)
	lfs	2, 0(r3)
	lfs	3, 0(r3)
	lfs	4, 0(r3)
	lfs	5, 0(r3)
	lfs	6, 0(r3)
	lfs	7, 0(r3)
	lfs	8, 0(r3)
	lfs	9, 0(r3)
	lfs	10, 0(r3)
	lfs	11, 0(r3)
	lfs	12, 0(r3)
	lfs	13, 0(r3)
	lfs	14, 0(r3)
	lfs	15, 0(r3)
	lfs	16, 0(r3)
	lfs	17, 0(r3)
	lfs	18, 0(r3)
	lfs	19, 0(r3)
	lfs	20, 0(r3)
	lfs	21, 0(r3)
	lfs	22, 0(r3)
	lfs	23, 0(r3)
	lfs	24, 0(r3)
	lfs	25, 0(r3)
	lfs	26, 0(r3)
	lfs	27, 0(r3)
	lfs	28, 0(r3)
	lfs	29, 0(r3)
	lfs	30, 0(r3)
	lfs	31, 0(r3)
	sync

	/*
	 * Set MPU/MSR to a known state
	 * Turn off FP
	 */

	andi.	r3, r3, 0
	sync
	mtmsr	r3
	isync
	sync

#endif /* INCLUDE_HW_FP */

	lis  r6, HIADJ(inFullVxWorksImage)
	addi r6, r6,LO(inFullVxWorksImage)
	li   r7, TRUE
	stw  r7, 0(r6)

	/* initialize the stack pointer */
	/* insert protection from decrementer exceptions */

	xor	p0, p0, p0
	LOADPTR	(p1, 0x4c000064)	/* load rfi (0x4c000064) to p1	*/
	stw	p1, 0x900(r0)		/* store rfi at 0x00000900	*/

	lis     sp, HIADJ(RAM_LOW_ADRS)
	addi    sp, sp, LO(RAM_LOW_ADRS)
doneCpu0Stack:

	addi    sp, sp, -FRAMEBASESZ    /* get frame stack */

        /* write physical CPU index */
	CPU_ID_GET(r6)
	mtspr	PIR, r6

#ifdef INCLUDE_VIRTUAL_SM
	/*
         * TLB1 #2/3.  virtual SM
         * Attributes: UX/UW/UR/SX/SW/SR
         */

        addis  r4,0,0x1003           /* TLBSEL = TLB1(CAM) , ESEL = 3*/
	ori    r4, r4, 0
        TLB_WRITE(r4,r5,_MMU_TLB_SZ_16M,VIRTUAL_SM_BASE,0x000a,VIRTUAL_SM_BASE_PHYS,0x003f,0)

#endif /* INCLUDE_VIRTUAL_SM */

	mr      r3, r8

	b usrInit
#endif /* INCLUDE_WRLOAD_IMAGE_BUILD || UBOOT */

FUNC_END(sysInit)

#ifdef INCLUDE_VIRTUAL_SM
FUNC_BEGIN(sysVirtualSmTlb)
    addis  r3,0,0x1003           /* TLBSEL = TLB1(CAM) , ESEL = 3*/
	ori    r3, r3, 0
    TLB_WRITE(r3,r4,_MMU_TLB_SZ_16M,VIRTUAL_SM_BASE,0x000a,VIRTUAL_SM_BASE_PHYS,0x003f,0)
    blr
FUNC_END(sysVirtualSmTlb)
#endif /* INCLUDE_VIRTUAL_SM */

FUNC_BEGIN(sysCacheEnable)
    li      r3, 0x0002
	msync
	isync
	mtspr   L1CSR0, r3		/* Invalidate the Data cache */
    li		r3, 0x0001
	msync
	isync
	mtspr	L1CSR0, r3		/* Enable the Data cache */

    li      r3, 0x0002
	msync
	isync
	mtspr   L1CSR1, r3		/* Invalidate the Instruction cache */
	msync
	isync
    li		r3, 0x0001
	msync
	isync
	mtspr	L1CSR1, r3		/* Enable the Instruction cache */

	msync
	isync
	blr
FUNC_END(sysCacheEnable)

/******************************************************************************
*
* sysICacheLock - lock one line in the instruction cache
*
* SYNOPSIS
* \ss
* void sysICacheLock
*     (
*     UINT32 addr
*     )
* \se
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysICacheLock)
        icbtls  0, 0, p0
        blr
FUNC_END(sysICacheLock)

/******************************************************************************
*
* sysICacheUnlock - unlock one line in the instruction cache
*
* SYNOPSIS
* \ss
* void sysICacheUnlock
*     (
*     UINT32 addr
*     )
* \se
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysICacheUnlock)
        icblc   0, 0, p0
        blr
FUNC_END(sysICacheUnlock)

	.balign 32
/******************************************************************************
*
* vxL2CSR0Set - Set the value of the L@ cache control register
*
* This routine returns the value written.
*
* SYNOPSIS
* \ss
* UINT32 vxL2CSR0Set
*     (
*     UINT32 value
*     )
* \se
*
* RETURNS: This routine returns the value in the L2CSR0 reg.
*/

FUNC_BEGIN(vxL2CSR0Set)
	sync
	isync
	mtspr	1017, p0
	isync
	blr
FUNC_END(vxL2CSR0Set)

	.balign 32
/******************************************************************************
*
* vxL2CSR0Get - Get the value of the L2 cache control register
*
* SYNOPSIS
* \ss
* UINT32 vxL2CSR0Get
*     (
*     void
*     )
* \se
*
* RETURNS: This routine returns the value in the L2CSR0 reg.
*/
FUNC_BEGIN(vxL2CSR0Get)
        mfspr  p0, 1017
	isync
	blr
FUNC_END(vxL2CSR0Get)

	.balign 32
/******************************************************************************
*
* vxL2CFG0Get - Get the value of the L2 cache config register
*
* SYNOPSIS
* \ss
* UINT32 vxL2CFG0Get
*     (
*     void
*     )
* \se
*
* RETURNS: This routine returns the value in the L2CFG0 reg.
*/
FUNC_BEGIN(vxL2CFG0Get)
        mfspr  p0, 519
	isync
	blr
FUNC_END(vxL2CFG0Get)

/******************************************************************************
*
* vxL2ERRINJCTLGet - Get the value of the L2 cache error injection control
* register
*
* SYNOPSIS
* \ss
* UINT32 vxL2ERRINJCTLGet
*     (
*     void
*     )
* \se
*
* RETURNS: This routine returns the value in the L2ERRINJCTL reg.
*/
FUNC_BEGIN(vxL2ERRINJCTLGet)
        mfspr  p0, L2ERRINJCTL
	blr
FUNC_END(vxL2ERRINJCTLGet)

/******************************************************************************
*
* vxL2ERRINJCTLSet - Set the value of the L2 cache error injection control
* register
*
* SYNOPSIS
* \ss
* void vxL2ERRINJCTLSet
*     (
*     UINT32
*     )
* \se
*
* RETURNS: N/A
*/
FUNC_BEGIN(vxL2ERRINJCTLSet)
	msync
	isync
        mtspr  L2ERRINJCTL, p0
	isync
	blr
FUNC_END(vxL2ERRINJCTLSet)

/******************************************************************************
*
* vxL2ERRDETGet - Get the value of the L2 cache error detect register
*
* SYNOPSIS
* \ss
* UINT32 vxL2ERRDETGet
*     (
*     void
*     )
* \se
*
* RETURNS: This routine returns the value in the L2ERRDET reg.
*/
FUNC_BEGIN(vxL2ERRDETGet)
        mfspr  p0, L2ERRDET
	blr
FUNC_END(vxL2ERRDETGet)

/******************************************************************************
*
* vxL2ERRDETSet - Set the value of the L2 cache error detect register
*
* SYNOPSIS
* \ss
* void vxL2ERRDETSet
*     (
*     UINT32
*     )
* \se
*
* RETURNS: N/A
*/
FUNC_BEGIN(vxL2ERRDETSet)
	msync
	isync
        mtspr  L2ERRDET, p0
	isync
	blr
FUNC_END(vxL2ERRDETSet)

/******************************************************************************
*
* vxL2ERRDISGet - Get the value of the L2 cache error disable register
*
* SYNOPSIS
* \ss
* UINT32 vxL2ERRDISGet
*     (
*     void
*     )
* \se
*
* RETURNS: This routine returns the value in the L2ERRDIS reg.
*/
FUNC_BEGIN(vxL2ERRDISGet)
        mfspr  p0, L2ERRDIS
	blr
FUNC_END(vxL2ERRDISGet)

/******************************************************************************
*
* vxL2ERRDISSet - Set the value of the L2 cache error disable register
*
* SYNOPSIS
* \ss
* void vxL2ERRDISSet
*     (
*     UINT32
*     )
* \se
*
* RETURNS: N/A
*/
FUNC_BEGIN(vxL2ERRDISSet)
	msync
	isync
        mtspr  L2ERRDIS, p0
	isync
	blr
FUNC_END(vxL2ERRDISSet)

/******************************************************************************
*
* vxL2ERRCTLGet - Get the value of the L2 cache error control register
*
* SYNOPSIS
* \ss
* UINT32 vxL2ERRCTLGet
*     (
*     void
*     )
* \se
*
* RETURNS: This routine returns the value in the L2ERRCTL reg.
*/
FUNC_BEGIN(vxL2ERRCTLGet)
        mfspr  p0, L2ERRCTL
	blr
FUNC_END(vxL2ERRCTLGet)

/******************************************************************************
*
* vxL2ERRCTLSet - Set the value of the L2 cache error control register
*
* SYNOPSIS
* \ss
* void vxL2ERRCTLSet
*     (
*     UINT32
*     )
* \se
*
* RETURNS: N/A
*/
FUNC_BEGIN(vxL2ERRCTLSet)
	msync
	isync
        mtspr  L2ERRCTL, p0
	isync
	blr
FUNC_END(vxL2ERRCTLSet)

/******************************************************************************
*
* vxL2ERRINTENGet - Get the value of the L2 cache error interrupt enable
* register
*
* SYNOPSIS
* \ss
* UINT32 vxL2ERRINTENGet
*     (
*     void
*     )
* \se
*
* RETURNS: This routine returns the value in the L2ERRINTEN reg.
*/
FUNC_BEGIN(vxL2ERRINTENGet)
        mfspr  p0, L2ERRINTEN
	blr
FUNC_END(vxL2ERRINTENGet)

/******************************************************************************
*
* vxL2ERRINTENSet - Set the value of the L2 cache error interrupt enable
* register
*
* SYNOPSIS
* \ss
* void vxL2ERRINTENSet
*     (
*     UINT32
*     )
* \se
*
* RETURNS: N/A
*/
FUNC_BEGIN(vxL2ERRINTENSet)
	msync
	isync
        mtspr  L2ERRINTEN, p0
	isync
	blr
FUNC_END(vxL2ERRINTENSet)

/******************************************************************************
* disableBranchPrediction - disables branch prediction
*
* SYNOPSIS
* \ss
* void disableBranchPrediction
*     (
*     void
*     )
* \se
*
* RETURNS: NONE
*/
FUNC_BEGIN(disableBranchPrediction)
        mfspr p0, 1013
        andi. p1, p0,LO(~_PPC_BUCSR_E)
        isync
        mtspr 1013,p1
        isync
        li    p0, _PPC_BUCSR_FI
        mtspr 1013,p0
        isync
        blr
FUNC_END(disableBranchPrediction)


/****************************************************
 * sysCacheFlush just flushes cache - assume int lock
 * p0 - cache line num
 * p1 - buffer origin
 * p2 - cache align size
 */

FUNC_BEGIN(sysCacheFlush)
	/*
	 * p3 contains the count of cache lines to be fetched & flushed.
	 * Convert to a count of pages covered, and fetch a word from
	 * each page to ensure that all addresses involved are in
	 * the TLB so that reloads do not disrupt the flush loop.
	 * A simple shift without round-up is sufficient because
	 * the p3 value is always a multiple of the shift count.
	 */
	srwi	p3, p0, MMU_RPN_SHIFT - CACHE_ALIGN_SHIFT
	mtspr	CTR, p3
        addi    p6,p1,0
        li      p5,MMU_PAGE_SIZE
	subf    p3,p5,p1

	/*
	 * There might be a page boundary between here and the end of
	 * the function, so make sure both pages are in the I-TLB.
	 */
	b	cacheL2DisableLoadItlb
cacheL2DisableLoadDtlb:
	add     p3,p3,p5
	lbzu	p4,0(p3)
	bdnz	cacheL2DisableLoadDtlb
	mtctr   p0         /* Load counter with number of cache lines */
	subf	p1, p2, p1 /* buffer points to text  - cache line size */
l2DisableFlush:
        add	p1, p2, p1		  /* +  cache line size */
	lbzu	p3, 0x0(p1)	       	  /* flush the data cache block */
        bdnz    l2DisableFlush     /* loop till cache ctr is zero */
	sync
	isync
	mtctr   p0         /* Load counter with number of cache lines */
        addi    p1, p6, 0
	subf	p1, p2, p1 /* buffer points to text  - cache line size */
l2DisableClear:
	add	p1, p2, p1  /* point to next cache line */
	dcbf	0,p1			    /* flush newly-loaded line */
	bdnz	l2DisableClear	    /* repeat for all sets and ways */
	sync
	isync
	blr
cacheL2DisableLoadItlb:
	b	cacheL2DisableLoadDtlb
FUNC_END(sysCacheFlush)

/*****************************************************************************
*
* sysInByte - reads a byte from an io address.
*
* This function reads a byte from a specified io address.
*
* RETURNS: byte from address.

* UCHAR sysInByte
*     (
*     UCHAR *  pAddr		/@ Virtual I/O addr to read from @/
*     )

*/

FUNC_BEGIN(sysInByte)
	eieio			/* Sync I/O operation */
	sync
	lbzx	p0,r0,p0	/* Read byte from I/O space */
	bclr	20,0		/* Return to caller */
FUNC_END(sysInByte)

/******************************************************************************
*
* sysOutByte - writes a byte to an io address.
*
* This function writes a byte to a specified io address.
*
* RETURNS: N/A

* VOID sysOutByte
*     (
*     UCHAR *  pAddr,		/@ Virtual I/O addr to write to @/
*     UCHAR    data		/@ data to be written @/
*     )

*/

FUNC_BEGIN(sysOutByte)
	stbx	p1,r0,p0	/* Write a byte to PCI space */
	eieio			/* Sync I/O operation */
	sync
	bclr	20,0		/* Return to caller */
FUNC_END(sysOutByte)

/*****************************************************************************
*
* sysInWord - reads a word from an address, swapping the bytes.
*
* This function reads a swapped word from a specified
* address.
*
* RETURNS:
* Returns swapped 16 bit data from the specified address.

* USHORT sysInWord
*     (
*     ULONG  address,		/@ addr to read from @/
*     )

*/

FUNC_BEGIN(sysInWord)
	eieio			/* Sync I/O operation */
	sync
        lhbrx   p0,r0,p0	/* Read and swap */
        bclr    20,0		/* Return to caller */
FUNC_END(sysInWord)


/*****************************************************************************
*
* sysOutWord - writes a word to an address swapping the bytes.
*
* This function writes a swapped word to a specified
* address.
*
* RETURNS: N/A

* VOID sysOutWord
*     (
*     ULONG address,		/@ Virtual addr to write to @/
*     UINT16  data		/@ Data to be written       @/
*     )

*/

FUNC_BEGIN(sysOutWord)
        sthbrx  p1,r0,p0	/* Write with swap to address */
	eieio			/* Sync I/O operation */
	sync
        bclr    20,0		/* Return to caller */
FUNC_END(sysOutWord)

/*****************************************************************************
*
* sysInLong - reads a long from an address.
*
* This function reads a long from a specified PCI Config Space (little-endian)
* address.
*
* RETURNS:
* Returns 32 bit data from the specified register.  Note that for PCI systems
* if no target responds, the data returned to the CPU will be 0xffffffff.

* ULONG sysInLong
*     (
*     ULONG  address,		/@ Virtual addr to read from @/
*     )

*/

FUNC_BEGIN(sysInLong)
	eieio			/* Sync I/O operation */
	sync
        lwbrx   p0,r0,p0	/* Read and swap from address */
        bclr    20,0		/* Return to caller */
FUNC_END(sysInLong)


/******************************************************************************
*
* sysOutLong - write a swapped long to address.
*
* This routine will store a 32-bit data item (input as big-endian)
* into an address in little-endian mode.
*
* RETURNS: N/A

* VOID sysOutLong
*     (
*     ULONG   address,		/@ Virtual addr to write to @/
*     ULONG   data		/@ Data to be written @/
*     )

*/

FUNC_BEGIN(sysOutLong)
        stwbrx  p1,r0,p0	/* store data as little-endian */
	eieio			/* Sync I/O operation */
	sync
        bclr    20,0
FUNC_END(sysOutLong)

/******************************************************************************
*
* sysPciRead32 - read 32 bit PCI data
*
* This routine will read a 32-bit data item from PCI (I/O or
* memory) space.
*
* RETURNS: N/A

* VOID sysPciRead32
*     (
*     ULONG *  pAddr,		/@ Virtual addr to read from @/
*     ULONG *  pResult		/@ location to receive data @/
*     )

*/

FUNC_BEGIN(sysPciRead32)
	eieio			/* Sync I/O operation */
        lwbrx   p0,r0,p0	/* get the data and swap the bytes */
        stw     p0,0(p1)	/* store into address ptd. to by p1 */
        bclr    20,0
FUNC_END(sysPciRead32)

/******************************************************************************
*
* sysPciWrite32 - write a 32 bit data item to PCI space
*
* This routine will store a 32-bit data item (input as big-endian)
* into PCI (I/O or memory) space in little-endian mode.
*
* RETURNS: N/A

* VOID sysPciWrite32
*     (
*     ULONG *  pAddr,		/@ Virtual addr to write to @/
*     ULONG   data		/@ Data to be written @/
*     )

*/

FUNC_BEGIN(sysPciWrite32)
        stwbrx  p1,r0,p0	/* store data as little-endian */
        bclr    20,0
FUNC_END(sysPciWrite32)

/*****************************************************************************
*
* sysPciInByte - reads a byte from PCI Config Space.
*
* This function reads a byte from a specified PCI Config Space address.
*
* RETURNS:
* Returns 8 bit data from the specified register.  Note that for PCI systems
* if no target responds, the data returned to the CPU will be 0xff.

* UINT8 sysPciInByte
*     (
*     UINT8 *  pAddr,		/@ Virtual addr to read from @/
*     )

*/

FUNC_BEGIN(sysPciInByte)
	eieio			/* Sync I/O operation */
        lbzx    p0,r0,p0	/* Read byte from PCI space */
        bclr    20,0		/* Return to caller */
FUNC_END(sysPciInByte)

/*****************************************************************************
*
* sysPciInWord - reads a word (16-bit big-endian) from PCI Config Space.
*
* This function reads a word from a specified PCI Config Space (little-endian)
* address.
*
* RETURNS:
* Returns 16 bit data from the specified register.  Note that for PCI systems
* if no target responds, the data returned to the CPU will be 0xffff.

* USHORT sysPciInWord
*     (
*     USHORT *  pAddr,		/@ Virtual addr to read from @/
*     )

*/

FUNC_BEGIN(sysPciInWord)
	eieio			/* Sync I/O operation */
        lhbrx   p0,r0,p0	/* Read and swap from PCI space */
        bclr    20,0		/* Return to caller */
FUNC_END(sysPciInWord)

/*****************************************************************************
*
* sysPciInLong - reads a long (32-bit big-endian) from PCI Config Space.
*
* This function reads a long from a specified PCI Config Space (little-endian)
* address.
*
* RETURNS:
* Returns 32 bit data from the specified register.  Note that for PCI systems
* if no target responds, the data returned to the CPU will be 0xffffffff.

* ULONG sysPciInLong
*     (
*     ULONG *  pAddr,		/@ Virtual addr to read from @/
*     )

*/

FUNC_BEGIN(sysPciInLong)
	eieio			/* Sync I/O operation */
        lwbrx   p0,r0,p0	/* Read and swap from PCI space */
        bclr    20,0		/* Return to caller */
FUNC_END(sysPciInLong)

/******************************************************************************
*
* sysPciOutByte - writes a byte to PCI Config Space.
*
* This function writes a byte to a specified PCI Config Space address.
*
* RETURNS: N/A

* VOID sysPciOutByte
*     (
*     UINT8 *  pAddr,		/@ Virtual addr to write to @/
*     UINT8  data		/@ Data to be written       @/
*     )

*/

FUNC_BEGIN(sysPciOutByte)
        stbx    p1,r0,p0	/* Write a byte to PCI space */
        bclr    20,0		/* Return to caller */
FUNC_END(sysPciOutByte)

/******************************************************************************
*
* sysPciOutWord - writes a word (16-bit big-endian) to PCI Config Space.
*
* This function writes a word to a specified PCI Config Space (little-endian)
* address.
*
* RETURNS: N/A

* VOID sysPciOutWord
*     (
*     USHORT *  pAddr,		/@ Virtual addr to write to @/
*     USHORT  data		/@ Data to be written       @/
*     )

*/

FUNC_BEGIN(sysPciOutWord)
        sthbrx  p1,r0,p0	/* Write with swap to PCI space */
        bclr    20,0		/* Return to caller */
FUNC_END(sysPciOutWord)

/******************************************************************************
*
* sysPciOutLong - writes a long (32-bit big-endian) to PCI Config Space.
*
* This function writes a long to a specified PCI Config Space (little-endian)
* address.
*
* RETURNS: N/A

* VOID sysPciOutLong
*     (
*     ULONG *  pAddr,		/@ Virtual addr to write to @/
*     ULONG  data		/@ Data to be written       @/
*     )

*/

FUNC_BEGIN(sysPciOutLong)
        stwbrx  p1,r0,p0	/* Write big-endian long to little-endian */
        mr      p0,p1		/* PCI space */
        bclr    20,0		/* Return to caller */
FUNC_END(sysPciOutLong)

/******************************************************************************
*
* sysPCGet - Get the value of the PC (Program Counter)
*
* This routine returns the value of the PC.
*
* SYNOPSIS
* \ss
* UINT32 sysPCGet
*     (
*     void
*     )
* \se
*
* RETURNS: the Program Counter Register (PC) value.
*/

FUNC_BEGIN(sysPCGet)
        mflr    r3
        blr
FUNC_END(sysPCGet)

/******************************************************************************
*
* sysTimeBaseLGet - Get lower half of Time Base Register
*
* SYNOPSIS
* \ss
* UINT32 sysTimeBaseLGet(void)
* \se
*
* This routine will read the contents the lower half of the Time
* Base Register (TBL - TBR 268).
*
* RETURNS: value of TBR 268 (in r3)
*/

FUNC_BEGIN(sysTimeBaseLGet)
    mfspr       r3, 268
    bclr        20,0                    /* Return to caller */
FUNC_END(sysTimeBaseLGet)

/*******************************************************************************
*
* vxTas - this routine performs the atomic test and set for the PowerPC arch.
*
* RETURN: None.
*/

FUNC_BEGIN(sysTas)
	lis	r4, 0x8000	/* set the upper bit of r4 */
	ori     r4, r4, 0x0000
	eieio			/* simple ordered store using eieio */

	dcbf    0, r3           /* Per errata CPU 22 */
	lwarx	r5, 0, r3	/* load and reserve */
	cmpwi	r5, 0		/* done if word */
	bne	sysTasEnd	/* not equal to 0 */

	stwcx.	r4, 0, r3	/* try to store non-zero */
	eieio			/* preserve load/store order */
	bne-	sysTas
	li	r3, 0x01
	blr
sysTasEnd:
	li	r3, 0
	blr
FUNC_END(sysTas)


#define WRITEADR(reg1, reg2, addr32, val) \
	lis	reg1, HI(addr32); \
	ori	reg1, reg1, LO(addr32); \
	lis	reg2, HI(val); \
	ori	reg2, reg2, LO(val); \
	stw	reg2, 0(reg1)

/*******************************************************************************
*
* sysEprGet - this routine returns the vector value in EPR.
*
* RETURN: Value in External Proxy Register (EPR).
*/

FUNC_BEGIN(sysEprGet)
        mfspr  r3, EPR
        isync
        blr
FUNC_END(sysEprGet)

/*******************************************************************************
*
* sysDoze - this routine puts the current CPU into doze state
*
* RETURN: None
*/

FUNC_BEGIN(sysDoze)
	CPU_ID_GET(r3)
	li	r4, 1
	slw	r3, r4, r3
FUNC_BEGIN(sysDozeCpu)
#if 0 /* XXX dozing appears to cause problems with dtsec */
	lis	r4, HI(P4080_CDOZCRL(CCSBAR))
	ori	r4, r4, LO(P4080_CDOZCRL(CCSBAR))
	stw	r3, 0(r4)
#else	/* XXX so we'll wait instead */
# if 0 /* XXX 'wait' is not understood by sf compiler :( */
	wait
# else /* XXX so we'll use hand-assembled instruction instead */
	.long 0x7c00007c
# endif
#endif
	blr
FUNC_END(sysDozeCpu)
FUNC_END(sysDoze)

#if defined(_WRS_CONFIG_SMP)
/*******************************************************************************
*
* sysSmpTimeBaseSet - This routine sets the timebase. It is intended
* to be called only by sysSmpTimeBaseInit, as part of the initial timebase
* synchronization process.  (vxTimeBaseSet cannot be used for this purpose
* because, in VxWorks SMP, vxTimeBaseSet is a no-op to prevent loss of
* synchronization due to applications setting the timebase.)
*
* RETURN: None.
*/

FUNC_BEGIN(sysSmpTimeBaseSet)
	/*
	 * No need to clear TBL first, since the TB
	 * is not running when this function is called.
	 */
	mttbl   p1
	mttbu	p0
	isync		/* Context sync req'd after setting time base registers */
	blr
FUNC_END(sysSmpTimeBaseSet)
#endif	/* _WRS_CONFIG_SMP */

/******************************************************************************
*
* sysCpcRegGet - get value of CPC register
*
* This routine reads a CPC register and returns the value.
*
* RETURNS: value of CPC register at addr
*
* UINT32 sysCpcRegGet
*     (
*     UINT32 addr		/@ address of register @/
*     )
*/

/******************************************************************************
*
* sysCpcRegSet - set value of CPC register
*
* This routine sets a CPC register.
*
* RETURNS: value of CPC register at addr
*
* UINT32 sysCpcRegSet
*     (
*     UINT32 addr		/@ address of register @/
*     UINT32 value		/@ value to write @/
*     )
*/

FUNC_BEGIN(sysCpcRegSet)
      mbar
      isync
      stw	p1, 0(p0)
FUNC_BEGIN(sysCpcRegGet)
      lwz	p0, 0(p0)
      mbar
      blr
FUNC_END(sysCpcRegGet)
FUNC_END(sysCpcRegSet)

/******************************************************************************
*
* sysCpuIdGet - get value of EPIC WHOAMI register
*
* This routine gets the CPU ID as reported by the EPIC
*
* RETURNS: value of EPIC WHOAMI
*
* UINT32 sysCpuIdGet (void)
*
*/
FUNC_BEGIN(sysCpuIdGet)
      CPU_ID_GET(r3)
      blr
FUNC_END(sysCpuIdGet)

/******************************************************************************
*
* sysLpidSet - set logical partition ID
*
* This routine sets the logical partition ID in LPIDR, MAS5, and MAS8
*
* RETURNS: N/A
*
* void sysLpidSet
*     (
*     UINT32 id			/@ logical partition ID @/
*     )
*/

FUNC_BEGIN(sysLpidSet)
	/* MAS8[TLPID] */
	mfspr	p1, MAS8
	rlwinm  p1, p1, 0, 0, 27	/* mask off TLPID */
	or	p1, p1, p0
	mtspr	MAS8, p1
	isync
	/* MAS5[SLPID] */
	mfspr	p1, MAS5
	rlwinm  p1, p1, 0, 0, 27	/* mask off SLPID */
	or	p1, p1, p0
	mtspr	MAS5, p1
	isync
	/* LPIDR */
	mtspr	LPIDR, p0
	isync
	blr
FUNC_END(sysLpidSet)

/******************************************************************************
*
* sysSelfReset - this routine resets the current CPU
*
* RETURNS: N/A
*
* void sysSelfReset ()
*/

FUNC_BEGIN(sysSelfReset)
	mfmsr	p0
	ori	p0, p0, LO(_PPC_MSR_DE)
	sync
	mtmsr	p0
	isync
	sync
        lis	p0, HI(0x40000000)
        ori	p0, p0, LO(0x40000000)
	mtspr	DBCR0, p0
	msync
	isync
        lis	p0, HI(0x60000000)
        ori	p0, p0, LO(0x60000000)
	mtspr	DBCR0, p0
	msync
	isync
	/* should not return, but just in case */
	blr
FUNC_END(sysSelfReset)

/******************************************************************************
*
* sysBoardReset - this routine resets the board
*
* RETURNS: N/A
*
* void sysBoardReset ()
*/

FUNC_BEGIN(sysBoardReset)
#define P4080_RSTCR(base)            (CAST(VUINT32 *)((base) + 0xE00B0))
	li	r3, 2
	lis	r4, HI(P4080_RSTCR(CCSBAR))
	ori	r4, r4, LO(P4080_RSTCR(CCSBAR))
	stw	r3, 0(r4)
	blr
FUNC_END(sysBoardReset)

#ifdef FSL_ERRATA_FPU_A003999
/******************************************************************************
*
* sysFslErrataFpuA003999 - this routine implements workaround to Errata A-003999
*
* void sysFslErrataFpuA003999 (void)
*
* This routine sets a bit 7 to a one to SPR977.
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysFslErrataFpuA003999)
    mfspr   p0, 977
    oris    p0, p0, 0x0100
	mtspr	977, p0
	blr
FUNC_END(sysFslErrataFpuA003999)
#endif /* FSL_ERRATA_FPU_A003999 */

#if defined(FSL_ERRATA_CCF_A004510) || defined(FSL_ERRATA_CCF_A004511)
/******************************************************************************
*
* sysFslErrataCcfA004510AndA004511 - This routine implements workaround
* to Errata A-004510 and A-004511.
*
* This routine reconfigure the CoreNet coherency fabric for A-004510
* and A-004511.
*
* RETURNS: N/A
*
* void sysFslErrataCcfA004510AndA004511 (void)
*/

FUNC_BEGIN(sysFslErrataCcfA004510AndA004511)

    /*
     * Errata: A-004510 (Transactions can be lost in the CoreNet coherency
     * fabric). This has been fixed in Rev 3.0 silicon.
     *
     * In Rev 2.0 silicon, to avoid the behavior Freescale recommends the
     * following software workaround:
     *
     * Workaround: Specific settings in the CCF, CPC and the core are required
     * to reconfigure the CoreNet coherency fabric.
     */

    /*
     * Errata: A-004511 (Transactions may be incorrectly dropped from the wait
     * queue in the CoreNet coherency fabric). This has been fixed in Rev 3.0
     * silicon.
     *
     * In Rev 2.0 silicon, to avoid the behavior Freescale recommends the
     * following software workaround:
     *
     * Workaround: Disable the Wait Queue logic by performing a read-modify-write
     * to clear bit 7 of the register at DCSRBAR + 0x000B_0008.
     */
#ifndef WRITEADR
#define WRITEADR(reg1,reg2,addr32,val) \
            lis reg1, HI(addr32); \
            ori reg1, reg1, LO(addr32); \
            lis reg2, HI(val); \
            ori reg2, reg2, LO(val); \
            stw reg2, 0(reg1)
#endif

#ifndef CCFREGISTERSET
#define CCFREGISTERSET(regbase,offset,val) \
            lis r3, HI(regbase + offset); \
            ori r3, r3,LO(regbase + offset); \
            lis r4, HI(val); \
            ori r4, r4,LO(val); \
            bl  setCCfRegister;
#endif

#ifndef MMU_UNCACHED
#define MMU_UNCACHED   (MMU_STATE_CACHEABLE_NOT      | MMU_STATE_GUARDED)
#endif

#ifndef MMU_IO_ACCESS
#define MMU_IO_ACCESS  MMU_UNCACHED  /* WIMGE bits for IO */
#endif

    mflr    r9

    /* This workaround needs to be applied only on rev 2.0 silicon. */

    mfspr   r3,SVR    /* get the system version from the SVR */
    rlwinm  r4,r3,28,28,31
    cmpwi   r4,2
    bne     skipCcfRecfg

#ifdef FSL_ERRATA_CCF_A004510 /* Fix A-004510 */

    /* Set core register SPR976 for A-004510 */

    bl  setCoreRegister

#endif /* FSL_ERRATA_CCF_A004510 */

    /*
     * While the other cores are held in boot hold-off mode (DCFG_BRR), one core
     * must be used to perform the write once the system has reached the ready
     * state of the reset process.So placed this fix workaround at here,
     * and core 0 used to perform the write.
     */

    mfspr   r6, PIR
    andi.   r6, r6, 0xf0
    bne     skipCcfRecfg

    /* Setup the memory mapped register address for DCSR registers */

    WRITEADR(r6,r7,P4080_LAWBARL29(CCSBAR),DCSR_LAW_BASE)
    sync
    WRITEADR(r6,r7,P4080_LAWAR29(CCSBAR),
                 LAWAR_ENABLE | LAWAR_TGTIF_DCSR | LAWAR_SIZE_4MB )
    LOADVAR(r7, P4080_LAWAR29(CCSBAR))    /* ensure LAWAR29 is set */
    isync

    li r7, 0                    /* clear upper 4 bits */
    mtspr MAS7, r7

    /*
     * TLB1 #10. DCSRBAR
     * Attributes: UX/UW/UR/SX/SW/SR
     */

    addis  r4,0,0x100A          /* TLBSEL = TLB1(CAM) , ESEL = 10*/
    addis  r5,0,HI(MMU_STATE_VALID)
    ori    r5,r5,_MMU_TLB_SZ_4M /* TS = 0, TSIZE = 4 MByte page size*/
    addis  r6,0,HI(DCSR_LAW_BASE) /* EPN = DCSRBAR */
    ori    r6,r6,MMU_IO_ACCESS   /* WIMGE */
    addis  r7,0,HI(DCSR_LAW_BASE) /* RPN = DCSRBAR */
    ori    r7,r7,0x0015          /* Supervisor XWR*/

    bl     setTlbFn

#ifdef FSL_ERRATA_CCF_A004510 /* Fix A-004510 or both A-004510&A-004511*/

    /* Set CCF register CCSRBASE + 0x0001_8600 for A-004510 */

    CCFREGISTERSET(CCSBAR,CCSR_REG_18600,CCSR_REG_18600_A004510)

    /* Set CCF debug register DCSRBASE + 0x000B_0E08 for A-004510 */

    CCFREGISTERSET(DCSR_LAW_BASE,DCSR_REG_B0E08,DCSR_REG_B0E08_A004510)

    /* Set CCF debug register DCSRBASE + 0x000B_0E18 for A-004510 */

    CCFREGISTERSET(DCSR_LAW_BASE,DCSR_REG_B0E18,DCSR_REG_B0E18_A004510)

    /* Set CCF debug register DCSRBASE + 0x000B_0E38 for A-004510 */

    CCFREGISTERSET(DCSR_LAW_BASE,DCSR_REG_B0E38,DCSR_REG_B0E38_A004510)

    /* Set CCF debug register DCSRBASE + 0x000B_0E40 for A-004510 */

    CCFREGISTERSET(DCSR_LAW_BASE,DCSR_REG_B0E40,DCSR_REG_B0E40_A004510)

    /* Set CPC registers CPCn_CPCHDBCR0(for each CPC) for A-004510 */

    CCFREGISTERSET(CCSBAR,CPC_REG_CPC1,CPC_REG_CPC1_A004510)
    CCFREGISTERSET(CCSBAR,CPC_REG_CPC2,CPC_REG_CPC2_A004510)

    /*
     * Set CCF debug register DCSRBASE + 0x000B_0008 for A-004510
     * Clear bit 7 of regsiter DCSRBASE + 0x000B_0008. This app note has to
     * cover multiple devices. Some devices are affected by both A-004510
     * and A-004511. Other devices are only affected by A-004511. If a device
     * is affected by both errata it must set the value to 0x0090_0000. If a
     * device is affected by only A-004511, it must set the value to
     * 0x0290_0000.
     */

    CCFREGISTERSET(DCSR_LAW_BASE,DCSR_REG_B0008,DCSR_REG_B0008_CFG)

#else  /* Fix A-004511 only */

    /* Set CCF debug register DCSRBASE + 0x000B_0008 for only A-004511 */

    CCFREGISTERSET(DCSR_LAW_BASE,DCSR_REG_B0008,DCSR_REG_B0008_A004511)

#endif
    /*
     * Note:
     * The I-cache must be flushed/unlocked after the workaround has been
     * completed. The flushing/unlocking of the I-cache afterwards avoids
     * problems from any potential cache aliasing between inhibited and
     * non-inhibited mappings so that there are no problematic TLB aliases
     * between cacheable and noncacheable.
     *
     * The I-cache is disabled at this moment, So, do nothing.
     */

    /* Invalidate TLB1 #10. */

    lis    r6, HI(DCSR_LAW_BASE)
    ori    r6, r6, LO(DCSR_LAW_BASE)
    li     r7, 0

   /*
    * tlbivax invalidates globally.  Another core's OS (linux) may happen to use
    * this same virtual address, so use tlbilx (local invalidate) for multi-core.
    */

    tlbilx  3, r7, r6  /* same as tlbilxva r7, p6 */
    isync

    WRITEADR(r6,r7,P4080_LAWBARL29(CCSBAR),0x0)
    sync
    WRITEADR(r6,r7,P4080_LAWAR29(CCSBAR),0x0)
    LOADVAR(r7, P4080_LAWAR29(CCSBAR))
    isync

skipCcfRecfg:
    mtlr    r9
    blr

    /*
     *
     * setCCfRegister - Writing to the CCF and CPC registers
     * r3 - CCF register address
     * r4 - value
     * The following sequence must be used to update the CCF and CPC registers.
     *   A: sync
     *      wait loop
     *      write register
     *      wait loop
     *      sync
     *      nop
     *      nop
     *      :
     *      :
     *      B: nop
     */

setCCfRegister:
    li      r5,0x200

    /*
     * Note:
     * The region between and including A and B must be locked in the I-cache.
     * This ensures that no transaction,other than the write to the register,
     * is generated into the system while the core is executing the sequence.
     * The region between and including A and B fill two cache lines,therefore,
     * lock two cache lines into I-cache.
     *
     * The I-cache is disabled at this moment, So, do nothing.
     */

lockInICache:
    sync

    /*
     * The wait loop that occurs before the write allows all previous
     * transactions to complete. The wait loop must not call into any
     * external code, or perform any loads or stores.
     */

    mtctr   r5
nopDelay1:
    nop
    bdnz    nopDelay1

    stw r4, 0(r3)

    /*
     * The wait loop that occurs after the write ensures that the write
     * transaction completes before moving on. The wait loop must not call
     * into any external code, or perform any loads or stores.
     */

    mtctr   r5
nopDelay2:
    nop
    bdnz    nopDelay2

    sync

    /*
     * Enough nop instructions need to be added to fill the current cacheline
     * and the subsequent cacheline. The nop instructions are used so that a
     * branch does not cause a fetch of another cacheline.
     */

fillNop:
    .fill 23,4,0x60000000

    blr

#ifdef FSL_ERRATA_CCF_A004510

    /* A-004510 required core register SPR976 set bits 55:60 to 001001 */

setCoreRegister:
    msync
    isync
    mfspr r3,976
    li r4,0x48
    rlwimi r3,r4,0,23,28
    mtspr 976,r3
    isync
    blr

#endif /* FSL_ERRATA_CCF_A004510 */

setTlbFn:
    mtspr  MAS0, r4
    mtspr  MAS1, r5
    mtspr  MAS2, r6
    mtspr  MAS3, r7
    isync           /* Per Table 3-3 e500mc Core RM Rev J note 1 */
    tlbwe
    isync           /* Per Table 3-3 e500mc Core RM Rev J note 2 */
    blr

FUNC_END(sysFslErrataCcfA004510AndA004511)
#endif /* FSL_ERRATA_CCF_A004510 || FSL_ERRATA_CCF_A004511 */

#if defined(INCLUDE_CACHE_ERROR_HANDLER)
#   include "sysCacheErrorALib.s"
#endif  /* INCLUDE_CACHE_ERROR_HANDLER */
