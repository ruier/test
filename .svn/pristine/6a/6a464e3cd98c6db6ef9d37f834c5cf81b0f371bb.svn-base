/* sysALib.s - Assembly support file and init routines */

/*
 * Copyright (c) 2010, 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01c,29dec11,mpc  initialized MMUCSR0 with value 0x06. (WIND00319727)
01b,19dec11,agf  Moved L1 parity handling to arch lib. (WIND00309426)
01a,17Dec10,y_y  derived from fsl_p1020_rdb/sysALib.s.
*/

#define _ASMLANGUAGE
#include <vxWorks.h>
#include <vsbConfig.h>
#include <asm.h>
#include "config.h"
#include <sysLib.h>
#include <sysL2Cache.h>
#include <arch/ppc/mmuE500Lib.h>

FUNC_EXPORT(sysInit)
FUNC_EXPORT(_sysInit)
FUNC_EXPORT(sysIvprSet)
FUNC_EXPORT(vxL2CTLSet)
FUNC_EXPORT(vxL2CTLGet)
FUNC_EXPORT(sysCacheFlush)
FUNC_EXPORT(sysCacheEnable)
FUNC_IMPORT(usrInit)
FUNC_EXPORT(sysL1Csr1Set)
FUNC_EXPORT(sysTimeBaseLGet)
FUNC_EXPORT(sysCpu1Start)
FUNC_EXPORT(sysCpu1Stop)

#if defined(_WRS_CONFIG_SMP)
FUNC_EXPORT(sysSmpTimeBaseSet)
#endif  /* _WRS_CONFIG_SMP */

#define WRITEADR(reg1, reg2, addr32, val) \
    lis    reg1, HI(addr32); \
    ori    reg1, reg1, LO(addr32); \
    lis    reg2, HI(val); \
    ori    reg2, reg2, LO(val); \
    stw    reg2, 0(reg1)

FUNC_IMPORT(sysCpu1Loop)

    _WRS_TEXT_SEG_START
        DATA_IMPORT(inFullVxWorksImage)

#define CACHE_ALIGN_SHIFT   5   /* Cache line size == 2**5 */

FUNC_LABEL(_sysInit)
FUNC_BEGIN(sysInit)

#ifdef INCLUDE_WRLOAD_IMAGE_BUILD

    xor     r6, r6, r6
    msync
    isync
    
    li      r6, 0x0002
    mtspr   L1CSR0, r6              /* Invalidate the Data cache */
    mtspr   L1CSR1, r6              /* Invalidate the Instruction cache */
    isync
    msync
    
    li      r6, 0x0000
    mtspr   L1CSR1, r6              /* Disable the Instrunction cache */
    isync
    mtspr   L1CSR0, r6              /* Disable the Data cache */
    isync
    msync
    
    li      r7, 0x0001
    mtspr   L1CSR1, r7              /* enable the instruction cache */
    isync
    msync
    
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
    addi  r3, r0, 0x1100        /* SPU is e500 specific */
    mtspr IVOR32, r3
    addi  r3, r0, 0x1200        /* FP data is e500 specific */
    mtspr IVOR33, r3
    addi  r3, r0, 0x1300        /* FP round is e500 specific */
    mtspr IVOR34, r3
    addi  r3, r0, 0x1400        /* perf mon is e500 specific */
    mtspr IVOR35, r3

    li    r2,0x06               /* L2TLB0_FI | L2TLB1_FI */
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
     * TLB1 #0.  Initial TLB for reset address
     * Attributes: UX/UW/UR/SX/SW/SR
     */
     
    addis   r4, 0, 0x1000               /* TLB1 entry#0 */
    ori     r4, r4, 0x0000
    mtspr   MAS0, r4
    addis   r5, 0, 0xc000               /* V = 1, IPROT = 1, TID = 0 */
    ori     r5, r5, _MMU_TLB_SZ_1M     /* TS = 0, TSIZE = 16 MByte */
    mtspr   MAS1, r5
    addis   r6, 0, HI(0xfff00000)  /* EPN */
    ori     r6, r6, 0x0008               /* WIMGE = 01010 */
    mtspr   MAS2, r6
    addis   r7, 0, HI(0xfff00000)  /* RPN */
    ori     r7, r7, 0x0015              /* Supervisor XWR */
    mtspr   MAS3, r7
    isync
    tlbwe
    msync
    isync 
	
  	/* TLB1 #1. 	CCSBAR space  */

    addis   r4, 0, 0x1001               /* TLB1 entry#1 */
    ori     r4, r4, 0x0000
    mtspr   MAS0, r4
    addis   r5, 0, 0xc000               /* V = 1, IPROT = 1, TID = 0 */
    ori     r5, r5, _MMU_TLB_SZ_1M     /* TS = 0, TSIZE = 16 MByte */
    mtspr   MAS1, r5
    addis   r6, 0, HI(CCSBAR)  /* EPN */
    ori     r6, r6, 0x000a               /* WIMGE = 01010 */
    mtspr   MAS2, r6
    addis   r7, 0, HI(CCSBAR)  /* RPN */
    ori     r7, r7, 0x0015              /* Supervisor XWR */
    mtspr   MAS3, r7
    isync
    tlbwe
    msync
    isync 

    /* TLB#2: DDR    0x00000000 -> 0x3FFFFFFF 1GB */

    addis   r4, 0, 0x1002            /* TLBSEL = TLB1(CAM) , ESEL = 1*/
    ori     r4, r4, 0x0000
    mtspr   MAS0, r4
    addis   r5, 0, 0xc000            /* V = 1, IPROT = 1, TID = 0*/
    ori     r5, r5, _MMU_TLB_SZ_1G /* TS = 0, TSIZE = 256 MByte page size*/
    mtspr   MAS1, r5
    addis   r6, 0, HI(0)  /* EPN = LOCAL_MEM_LOCAL_ADRS */
    ori     r6, r6, 0x0000          /* WIMGE = 00000 */
    mtspr   MAS2, r6
    addis   r7, 0, HI(0)  /* RPN = LOCAL_MEM_LOCAL_ADRS */
    ori     r7, r7, 0x0015           /* User/Supervisor XWR*/
    mtspr   MAS3, r7
    isync
    msync
    tlbwe
    tlbsync

    /* TLB1 #4. BCSR/PMC0/PMC1 space: 0xf8000000 -> 0xf8040000 (256kBB) */

    addis   r4, 0, 0x1004              /* TLB1 entry#4 */
    ori     r4, r4, 0x0000
    mtspr   MAS0, r4
    addis   r5, 0, 0xc000               /* V = 1, IPROT = 1, TID = 0 */
    ori     r5, r5, _MMU_TLB_SZ_256K     /* TS = 0, TSIZE = 16 MByte */
    mtspr   MAS1, r5
    addis   r6, 0, HI(0xf8000000)  /* EPN */
    ori     r6, r6, 0x000a               /* WIMGE = 01010 */
    mtspr   MAS2, r6
    addis   r7, 0, HI(0xf8000000)  /* RPN */
    ori     r7, r7, 0x0015              /* Supervisor XWR */
    mtspr   MAS3, r7
    isync
    tlbwe
    msync
    isync 
    
    /* Setup stack pointer */

    lis     sp, HIADJ(RAM_LOW_ADRS)
    addi    sp, sp, LO(RAM_LOW_ADRS)
    addi    sp, sp, -FRAMEBASESZ    /* get frame stack */

    
    /* Memory etc already setup jump to alternate boot image */
    lis     r6, HI(usrInit)
    ori     r6, r6, LO(usrInit)
    mtctr   r6
    bctr
    
_sysInitEnd:
    .space (0xffc - (_sysInitEnd-_sysInit))
_resetVector:

    b sysInit
    
#else  /* INCLUDE_WRLOAD_IMAGE_BUILD */

    mr      r8, r3

    xor     p0, p0, p0
    mtspr   TCR, p0

    li      r6, 0x0002
    mtspr   L1CSR0, r6              /* Invalidate the Data cache */
    mtspr   L1CSR1, r6              /* Invalidate the Instruction cache */
    isync
    msync
    
    li      r6, 0x0000
    mtspr   L1CSR1, r6              /* Disable the Instrunction cache */
    isync
    mtspr   L1CSR0, r6              /* Disable the Data cache */
    isync
    msync
    
    li      r7, 0x0001
    mtspr   L1CSR1, r7              /* enable the instruction cache */
    isync
    msync
   
    mbar 0
    
    lis    r6, HIADJ(inFullVxWorksImage)
    addi   r6, r6,LO(inFullVxWorksImage)
    li     r7, TRUE
    stw    r7, 0(r6)

    /* initialize the stack pointer */

doCpu0Stack:

    /* insert protection from decrementer exceptions */

    xor    p0, p0, p0
    LOADPTR (p1, 0x4c000064)        /* load rfi (0x4c000064) to p1  */
    stw    p1, 0x900(r0)            /* store rfi at 0x00000900  */

    lis    sp, HIADJ(RAM_LOW_ADRS)
    addi   sp, sp, LO(RAM_LOW_ADRS)
    
doneCpu0Stack:

    addi   sp, sp, -FRAMEBASESZ     /* get frame stack */

bootUsrInit:
    mr     r3, r8

#endif  /* INCLUDE_WRLOAD_IMAGE_BUILD */

    b      usrInit
FUNC_END(sysInit)

/*******************************************************************************
*
* sysCacheEnable - Enable the L1 cache (D-catch and I-catch)
*
* This routine returns the value written.
*
* SYNOPSIS
* \ss
* VOID sysCacheEnable(void)
* \se
*
* RETURNS: N/A.
*/
FUNC_BEGIN(sysCacheEnable)

    /* Invalidate the Data cache */

    li     r3, 0x0002
    msync
    isync
    mtspr  L1CSR0, r3
    msync
    isync

    /* enable the data cache */

    li     r3, 0x0001
    msync
    isync
    mtspr  L1CSR0, r3
    msync
    isync

    /*
     * Write to 0 and then write to 1 to flash clear the valid bit of all
     * entries in the instruction cache
     */

    li     r3, 0x0000
    msync
    isync
    mtspr  L1CSR1, r3
    msync
    isync
    li     r3, 0x0002
    msync
    isync
    mtspr  L1CSR1, r3
    msync
    isync

    /* enable the instruction cache */

    li     r3, 0x0001
    msync
    isync
    mtspr   L1CSR1, r3
    msync
    isync

    blr
FUNC_END(sysCacheEnable)

    .balign 32
/*******************************************************************************
*
* vxL2CTLSet - Set the value of the L@ cache control register
*
* This routine returns the value written.
*
* SYNOPSIS
* \ss
* UINT32 sysL2CTLSet
*     (
*     UINT32 value,
*     UINT32 *addr
*     )
* \se
*
* RETURNS: This routine returns the value in the L2CTL reg.
*/

FUNC_BEGIN(vxL2CTLSet)
    mbar   0
    isync
    stw    p0, 0(p1)
    lwz    p0, 0(p1)
    mbar    0
    isync
    blr
FUNC_END(vxL2CTLSet)

    .balign 32

/*******************************************************************************
*
* vxL2CTLGet - Get the value of the L2 cache control register
*
* SYNOPSIS
* \ss
* UINT32 sysL2CTLGet
*     (
*     UINT32 *addr
*     )
* \se
*
* RETURNS: This routine returns the value in the L2CTL reg.
*/

FUNC_BEGIN(vxL2CTLGet)
    mbar    0
    isync
    lwz    p1, 0x0(p0)
    addi   p0, p1, 0x0
    mbar   0
    isync
    blr
FUNC_END(vxL2CTLGet)

/*******************************************************************************
*
* sysCacheFlush - just flushes cache - assume int lock
*
* p0 - cache line num
* p1 - buffer origin
* p2 - cache align size
*
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

    srwi   p3, p0, (MMU_RPN_SHIFT - CACHE_ALIGN_SHIFT)
    mtspr  CTR, p3
    addi   p6, p1,0
    li     p5, MMU_PAGE_SIZE
    subf   p3, p5, p1

    /*
     * There might be a page boundary between here and the end of
     * the function, so make sure both pages are in the I-TLB.
     */

    b      cacheL2DisableLoadItlb
cacheL2DisableLoadDtlb:
    add    p3, p3, p5
    lbzu   p4, 0(p3)
    bdnz   cacheL2DisableLoadDtlb
    mtctr  p0               /* Load counter with number of cache lines */
    subf   p1, p2, p1       /* buffer points to text  - cache line size */
l2DisableFlush:
    add    p1, p2, p1       /* +  cache line size */
    lbzu   p3, 0x0(p1)      /* flush the data cache block */
    bdnz   l2DisableFlush   /* loop till cache ctr is zero */
    sync
    isync
    mtctr  p0               /* Load counter with number of cache lines */
    addi   p1, p6, 0
    subf   p1, p2, p1       /* buffer points to text  - cache line size */
l2DisableClear:
    add    p1, p2, p1       /* point to next cache line */
    dcbf   0, p1            /* flush newly-loaded line */
    bdnz   l2DisableClear   /* repeat for all sets and ways */
    sync
    isync
    blr
cacheL2DisableLoadItlb:
    b      cacheL2DisableLoadDtlb
FUNC_END(sysCacheFlush)

/*******************************************************************************
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
    mfspr  r3, 268
    bclr   20, 0            /* Return to caller */
FUNC_END(sysTimeBaseLGet)

/*******************************************************************************
*
* sysL1Csr1Set - Set the value of L1CSR1
*
* SYNOPSIS
* \ss
* void sysL1Csr1Set
*     (
*     UINT32
*     )
* \se
*
* RETURNS: none
*/

FUNC_BEGIN(sysL1Csr1Set)
    msync
    isync
    mtspr  L1CSR1, r3
    msync
    isync
    blr
FUNC_END(sysL1Csr1Set)

FUNC_BEGIN(sysIvprSet)
    mtspr  IVPR, p0
    blr
FUNC_END(sysIvprSet)

#define WRITEADR(reg1, reg2, addr32, val) \
    lis    reg1, HI(addr32); \
    ori    reg1, reg1, LO(addr32); \
    lis    reg2, HI(val); \
    ori    reg2, reg2, LO(val); \
    stw    reg2, 0(reg1)


/*******************************************************************************
*
* sysCpu1Start - this routine starts CPU 1.
*
* RETURN: None.
*/

FUNC_BEGIN(sysCpu1Start)
    WRITEADR(r4, r5, EEB_PCR(CCSBAR), 0x03000000)
    isync
    sync
    eieio
    blr
FUNC_END(sysCpu1Start)


/*******************************************************************************
*
* sysCpu1Stop - this routine stops CPU 1.
*
* RETURN: None.
*/

FUNC_BEGIN(sysCpu1Stop)
    WRITEADR(r4, r5, EEB_PCR(CCSBAR), 0x01000000)
    isync
    sync
    eieio
    blr
FUNC_END(sysCpu1Stop)

#if defined(_WRS_CONFIG_SMP)
/*******************************************************************************
*
* sysSmpTimeBaseSet - This routine sets the timebase, and ensures
* that it is enabled in HID0.  It is intended to be called only by
* sysSmpTimeBaseInit, as part of the initial timebase synchronization
* process.  (vxTimeBaseSet cannot be used for this purpose because,
* in VxWorks SMP, vxTimeBaseSet is a no-op to prevent loss of
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
    mttbu   p0
    mfspr   p0, HID0
    oris    p0, p0, _PPC_HID0_TBEN
    mtspr   HID0, p0
    isync       /* Context sync req'd after setting HID0[TBEN] */
    blr
FUNC_END(sysSmpTimeBaseSet)
#endif  /* _WRS_CONFIG_SMP */

