/* sysALib.s - Assembly support file and init routines */

/*
 * Copyright (c) 2012, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01n,25jan13,wyt  updated code to accord with macros.
01m,22jan13,h_k  cleaned up UBOOT macros.
01l,09jan13,zl   renamed CPU_ID_GET to EPIC_ID_GET
01k,23nov12,x_s  removed e6500 core related functions to vxALib.s.
01j,21nov12,h_k  added sysSpr631BitSet.
01i,09nov12,h_k  added sysThreadReset.
01h,06nov12,h_k  corrected sysDoze.
01g,02nov12,x_s  Remove L2/L3 cache code to ARCH.
01f,01nov12,h_k  removed dec timer exception stub setting to avoid un-mapped
                 memory access on non-primary AMPs.
01e,15oct12,wap  Disable branch prediction early during sysInit() when booting
                 with U-Boot
01d,09oct12,syt  code clean up.
01c,02oct12,h_k  added UBOOT config.
01b,05sep12,h_k  removed unsupported L2 cache routines.
01a,07may12,h_k  adapted from fsl_p5020_ds version 01d.
*/

#define _ASMLANGUAGE
#include <vxWorks.h>
#include <vsbConfig.h>
#include <asm.h>
#include <config.h>
#include <sysLib.h>
#include <arch/ppc/mmuE500Lib.h>

    FUNC_EXPORT(sysInit)
    FUNC_EXPORT(_sysInit)
    FUNC_IMPORT(usrInit)     
    FUNC_EXPORT(sysBoardReset)
#ifdef INCLUDE_VIRTUAL_SM
    FUNC_EXPORT(sysVirtualSmTlb)
#endif
#ifdef _WRS_CONFIG_SMP
    FUNC_IMPORT(coreE6500CpuLoop)
    FUNC_EXPORT(sysThreadReset)
#endif

#define EPIC_ID_GET(reg) \
    addis   reg, 0, HI(T4_EPICWHOAMI) ; \
    lwz     reg, LO(T4_EPICWHOAMI)(reg) ;

#define TLB_WRITE(esel,reg,size,epn,wimge,rpn,perm,erpn)    \
    mtspr  MAS0, esel;              \
    addis  reg,0,0xC000; /* valid and iprot */     \
    ori    reg,reg,size;            \
    mtspr  MAS1,reg;                \
    addis  reg,0,HI(epn);           \
    ori    reg,reg,wimge;           \
    mtspr  MAS2,reg;                \
    addis  reg,0,HI(rpn) ;          \
    ori    reg,reg,perm ;           \
    mtspr  MAS3,reg ;               \
    li     reg,erpn ;               \
    mtspr  MAS7,reg ;               \
    isync ;                         \
    msync ;                         \
    tlbwe ;                         \
    isync ;                         \
    msync ;                         \

    _WRS_TEXT_SEG_START

#define CACHE_ALIGN_SHIFT   6   /* Cache line size == 2**5 */

FUNC_LABEL(_sysInit)
FUNC_BEGIN(sysInit)

#if defined(INCLUDE_AMP) && !defined(INCLUDE_AMP_CPU_00)
    li  r8, BOOT_NORMAL
#else
    mr  r8, r3
#endif	/* INCLUDE_AMP && !INCLUDE_AMP_CPU_00 */

    mfmsr    r3             /* load msr to parm0 */
    INT_MASK(r3, r4)        /* mask EE bit */
    mtmsr    r4             /* LOCK INTERRUPT */

    xor p0, p0, p0
    mtspr TCR, p0
    xor r6, r6, r6
    msync
    isync
    mtspr L1CSR0, r6        /* Disable the Data cache */
    li   r6, 0x0002
    msync
    isync
    mtspr L1CSR0, r6        /* Invalidate the Data cache */
    li    r6, 0x0000
    msync
    isync
    mtspr L1CSR1, r6        /* Disable the Instrunction cache */
    li   r6, 0x0002
    msync
    isync
    mtspr L1CSR1, r6        /* Invalidate the Instruction cache */
    isync
    li   r6, 0x0000
    msync
    isync
    mtspr L1CSR1, r6        /* Disable the Instruction cache*/
    msync
    isync

    mbar 0

#ifdef INCLUDE_HW_FP

     /* Set MPU/MSR to a known state, turn on FP */

    andi.   r3, r3, 0
    ori     r3, r3, 0x2000
    sync
    mtmsr   r3
    isync
    sync

    /* Init the floating point control/status register */

    mtfsfi  7, 0x0
    mtfsfi  6, 0x0
    mtfsfi  5, 0x0
    mtfsfi  4, 0x0
    mtfsfi  3, 0x0
    mtfsfi  2, 0x0
    mtfsfi  1, 0x0
    mtfsfi  0, 0x0
    isync

    /* Initialize the floating point data registers to a known state */

    bl  ifpdrValue
    .long   0x3f800000  /* 1.0 */
ifpdrValue:
    mfspr   r3, LR
    lfs 0, 0(r3)
    lfs 1, 0(r3)
    lfs 2, 0(r3)
    lfs 3, 0(r3)
    lfs 4, 0(r3)
    lfs 5, 0(r3)
    lfs 6, 0(r3)
    lfs 7, 0(r3)
    lfs 8, 0(r3)
    lfs 9, 0(r3)
    lfs 10, 0(r3)
    lfs 11, 0(r3)
    lfs 12, 0(r3)
    lfs 13, 0(r3)
    lfs 14, 0(r3)
    lfs 15, 0(r3)
    lfs 16, 0(r3)
    lfs 17, 0(r3)
    lfs 18, 0(r3)
    lfs 19, 0(r3)
    lfs 20, 0(r3)
    lfs 21, 0(r3)
    lfs 22, 0(r3)
    lfs 23, 0(r3)
    lfs 24, 0(r3)
    lfs 25, 0(r3)
    lfs 26, 0(r3)
    lfs 27, 0(r3)
    lfs 28, 0(r3)
    lfs 29, 0(r3)
    lfs 30, 0(r3)
    lfs 31, 0(r3)
    sync

    /*
     * Set MPU/MSR to a known state
     * Turn off FP
     */

    andi.   r3, r3, 0
    sync
    mtmsr   r3
    isync
    sync

#endif /* INCLUDE_HW_FP */

    /* initialize the stack pointer */

    lis     sp, HIADJ(RAM_LOW_ADRS - FRAMEBASESZ)
    addi    sp, sp, LO(RAM_LOW_ADRS - FRAMEBASESZ)	/* get frame stack */

    /*
     * save physical CPU index in PIR; vxCpuLibInit() for PPC expects PIR to 
     * be set to the AMP base CPU. It will save the PIR value as 
     * vxBaseCpuPhysIndex, then clear PIR. After that point PIR is storing 
     * the logical index, and the physical CPU ID is obtained with 
     * CPU_LOGICAL_TO_PHYS().
     */

    EPIC_ID_GET(r6)

    mtspr   PIR, r6

#ifdef INCLUDE_VIRTUAL_SM
    /*
     * TLB1 #2/3.  virtual SM
     * Attributes: UX/UW/UR/SX/SW/SR
     */

    addis  r4,0,0x1003              /* TLBSEL = TLB1(CAM) , ESEL = 3*/
    ori    r4, r4, 0
    TLB_WRITE(r4,r5,_MMU_TLB_SZ_16M,VIRTUAL_SM_BASE,0x000a,VIRTUAL_SM_BASE_PHYS,0x003f,0)
#endif /* INCLUDE_VIRTUAL_SM */

    mr      r3, r8

    b usrInit

FUNC_END(sysInit)

#ifdef INCLUDE_VIRTUAL_SM
FUNC_BEGIN(sysVirtualSmTlb)
    addis  r3,0,0x1003              /* TLBSEL = TLB1(CAM) , ESEL = 3*/
    ori    r3, r3, 0
    TLB_WRITE(r3, r4, _MMU_TLB_SZ_16M, VIRTUAL_SM_BASE, 0x000a, VIRTUAL_SM_BASE_PHYS, 0x003f, 0)
    blr
FUNC_END(sysVirtualSmTlb)
#endif /* INCLUDE_VIRTUAL_SM */

/*******************************************************************************
* sysBoardReset - this routine resets the board
*
* RETURNS: N/A
*
* void sysBoardReset (void)
*/

FUNC_BEGIN(sysBoardReset)
    li  r3, 2
    lis r4, HI(T4_RSTCR)
    ori r4, r4, LO(T4_RSTCR)
    stw r3, 0(r4)
    blr
FUNC_END(sysBoardReset)

#ifdef _WRS_CONFIG_SMP
/*******************************************************************************
*
* sysThreadReset - prepare thread1 to start
*
* This routine initializes system registers, set stack pointer and braches to
* coreE6500CpuLoop() function to start thread1.
*
* void sysThreadReset (void)
*/

FUNC_BEGIN(sysThreadReset)
        /* Clear SRR0, CSRR0, MCSRR0, SRR1, CSRR1 , MCSRR1, IVPR */

        mfspr   r0, MCSR
        mtspr   MCSR, r0

        xor     r0,r0,r0

        mtspr   SRR0,  r0
        mtspr   SRR1,  r0
        mtspr   CSRR0, r0
        mtspr   CSRR1, r0
        mtspr   MCSRR0,r0
        mtspr   MCSRR1,r0
        mtspr   ESR,   r0
        mtspr   DEAR,  r0

        mfmsr   r4
        andi.   r5, r4, _PPC_MSR_DE
        mtmsr   r5	/* Per Table 3-4 e500mc Core RM Rev J note 1 */
        isync		/* Per Table 3-3 e500mc Core RM Rev J row 1 */
        mtspr   DBCR0, r0
        mtspr   DBCR1, r0
        mtspr   DBCR2, r0
        mtspr   IAC1,  r0
        mtspr   IAC2,  r0
        mtspr   DAC1,  r0
        mtspr   DAC2,  r0
        mfspr   r1, DBSR
        mtspr   DBSR, r1
        isync		/* Per Table 3-4 e500mc Core RM Rev J note 1 */
        mtmsr   r4	/* Per Table 3-4 e500mc Core RM Rev J note 1 */
        isync		/* Per Table 3-3 e500mc Core RM Rev J row 1 */

        isync		/* Per Table 3-2 e500mc Core RM Rev J row 15 */
        mtspr   PID0, r0
        isync		/* Per Table 3-3 e500mc Core RM Rev J row 15 */

        mtspr   TCR,  r0
        mtspr   BUCSR,r0

        mtspr   MAS4, r0
        mtspr   MAS5, r0
        mtspr   MAS6, r0
        mtspr   MAS7, r0
        mtspr   MAS8, r0
        isync		/* Per Table 3-3 e500mc Core RM Rev J note 1 */

        EPIC_ID_GET(r3)
        rlwinm  r4, r3, CPUn_SPACE_SIZE_SHIFT, 0, 31
        lis     r3, HI(CPUn_SPACE_BASE_ADRS)
        ori     r3, r3, LO(CPUn_SPACE_BASE_ADRS)
        subf    r3, r4, r3		/* equiv. to CPUn_SPACE */
        lwz     sp, 4(r3)		/* CPUn_STACK_START_ADR */
        b       coreE6500CpuLoop
FUNC_END(sysThreadReset)
#endif /* _WRS_CONFIG_SMP */
