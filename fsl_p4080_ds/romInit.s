/* romInit.s - ROM bootcode for QorIQ BSPs */

/*
 * Copyright (c) 2008-2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01z,11jan13,ylu  Update for fsl_p4080_ds rev 3 silicon.
01y,25jun12,c_l  Used low priority LAWs for DDR. (WIND00357618)
01x,04jun12,c_l  Added Errata implementation A-004510(CCF) and A-004511(CCF).
                 (WIND00352077) (WIND00349563)
01w,06feb12,x_s  Added two workarounds raised in P4080 Errata notification,
                 A-003474(DDR) and A-003999(FPU). (WIND00331187)
01v,27jan12,agf  replace SYSTEM_GLOBALS with RESERVED_ATOMICS (WIND00329621)
01u,29dec11,mpc  Initialized MMUCSR0 with value 0x06. (WIND00319727)
01t,24nov11,y_y  Wrap MMU_RAM_ACCESS in ifndef to allow config.h or
                 <boardname>.h to override.
01s,10oct11,ers  Changed RAM cache mode for compressed bootrom.(WIND00310376)
01r,20sep11,ers  Added SYSTEM_GLOBALS for SMP, made common across QorIQ targets,
                 save space on the 4K boot page by using setTlbFn().
01q,17aug11,ers  Replaced DYNAMIC_WRLOAD with generic wrload support.
01p,10feb11,e_d  add DDR TLB map and remove ROM TLB map in resetEntry for SMP mode.
01o,21jan11,e_d  Fix CCSRBAR register value set issue.(WIND00247171)
01n,14jan11,agf  use macro for CPUn_SPACE calc instead of hard-coded number
01m,22dec10,pgh  Fix address for magic numbers
01l,09mar10,pmr  support rev B board
01k,10feb10,pmr  clear MAS5,7,8
01j,09nov09,pmr  support split RAM, half below and half above 32-bit boundary
01i,08oct09,pmr  remove tlbsync
01h,28sep09,pmr  remove USE_36_BIT
01g,31aug09,pmr  DDR setup for P4080DS
01f,19jun09,pmr  36-bit addressing
01e,11nov08,pmr  cleanup
01d,09oct08,pmr  use cmpw instead of cmp
01c,20aug08,pmr  modifications for AMP
01b,30may08,pmr  mods for AMP
01a,24apr08,pmr  adapted from ads8572 version 01e.
*/

#define    _ASMLANGUAGE
#include <vxWorks.h>
#include <sysLib.h>
#include <asm.h>
#include <regs.h>
#include <arch/ppc/mmuE500Lib.h>
#include "config.h"

#ifndef MMU_STATE_IPROT
#define MMU_STATE_IPROT 0x40000000
#endif

#define MMU_ROM_ACCESS (MMU_STATE_CACHEABLE_WRITETHROUGH | \
                        MMU_STATE_MEM_COHERENCY      | MMU_STATE_GUARDED)

#define MMU_UNCACHED   (MMU_STATE_CACHEABLE_NOT      | MMU_STATE_GUARDED)
#define MMU_CACHED     (MMU_STATE_CACHEABLE_COPYBACK | MMU_STATE_MEM_COHERENCY )

#define MMU_IO_ACCESS  MMU_UNCACHED  /* WIMGE bits for IO */

#ifndef MMU_RAM_ACCESS
#define MMU_RAM_ACCESS MMU_UNCACHED  /* WIMGE bits for RAM */
#endif

/*
 * These defines supports secondary images on a specific boundary.  Only map 256MB
 * to allow loading at any 256MB boundary.  The runtime kernel can later expand the
 * range when sysStaticTlbDesc becomes active in sysHwInit().  At the time of this
 * writing the SMP image for the P5020 BSP limits this to a 256M boundary.  The
 * P4080 BSPs worked fine with a 64MB boundary.
 */
#define IMAGE_MASK       0xF000          /* 0xF000 - 256MB, 0xFC00 - 64MB, etc. */
#define IMAGE_BOUNDARY  _MMU_TLB_SZ_256M

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
#define    LOADVAR(reg, addr32) \
      addis reg, r0, HIADJ(addr32); \
      lwz reg, LO(addr32)(reg)

#define WRITEADR(reg1,reg2,addr32,val) \
        lis reg1, HI(addr32); \
        ori reg1, reg1, LO(addr32); \
        lis reg2, HI(val); \
        ori reg2, reg2, LO(val); \
        stw reg2, 0(reg1)

#define READADR(reg1,reg2,addr32) \
        lis reg1, HI(addr32); \
        ori reg1, reg1, LO(addr32); \
        lwz reg2, 0(reg1)

#define WRITEOFFSET(regbase,reg2,offset,val) \
        lis reg2, HI(val); \
        ori reg2, reg2, LO(val); \
        stw reg2, offset(regbase);

#define CCFREGISTERSET(regbase,offset,val) \
        lis r3, HI(regbase + offset); \
        ori r3, r3,LO(regbase + offset); \
        lis r4, HI(val); \
        ori r4, r4,LO(val); \
        bl  setCCfRegister;

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
    .long 0x00050607
    .long 0x08090a0b
    .long 0x0c000000
    .long 0x00000000
    .long 0x00000000

cold:
    li    r2, BOOT_COLD
    b     start

warm:
    mr     r2,r3

    mfspr    r6, PIR
    rlwinm    r6, r6, 27, 29, 31
    cmpwi   r6, 0
    beq     skipCcfRecfg

    /* running on secondary CPU */

    /* jump to sysInit for AMP - never get here for SMP */

    rlwinm  r6, r6, CPUn_SPACE_SIZE_SHIFT,0,31
                 /* mult. cpunum by CPUn_SPACE_SIZE */
    lis    r3, HI(CPUn_SPACE_BASE_ADRS)
    ori     r3, r3, LO(CPUn_SPACE_BASE_ADRS)
    subf    r6, r6, r3            /* equiv. to CPUn_SPACE */

waitForStartSetWarm:
    lwz     r3, 8(r6)            /* CPUn_INIT_START_ADR */

    /*
     * Check that init start address is not zero before jumping
     * This allows cpu1 to reboot and wait for cpu0 to reload image
     */

    lis     r4,HI(MP_MAGIC_RBV)
    ori     r4, r4, LO(MP_MAGIC_RBV)
    cmpw    r3, r4
    beq     waitForStartSetWarm
    mtctr   r3
    bctr

start:
#if defined(FSL_ERRATA_CCF_A004510) || defined(FSL_ERRATA_CCF_A004511)

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

    /* This workaround needs to be applied only on rev 2.0 silicon. */

    mfspr   r3,SVR    /* get the system version from the SVR */
    rlwinm  r4,r3,28,28,31
    cmpwi   r4,2
    bne     skipCcfRecfg

#ifdef FSL_ERRATA_CCF_A004510  /* Fix A-004510 */

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

#ifdef FSL_ERRATA_CCF_A004510  /* Fix A-004510 or both A-004510&A-004511*/

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

    /* The I-cache must be flushed/unlocked after the workaround has been
     * completed. The flushing/unlocking of the I-cache afterwards avoids
     * problems from any potential cache aliasing between inhibited and
     * non-inhibited mappings so that there are no problematic TLB aliases
     * between cacheable and noncacheable.
     */

    lis r6,HI(_PPC_L1CSR_FI|_PPC_L1CSR_FLR)
    ori r6,r6,LO(_PPC_L1CSR_FI|_PPC_L1CSR_FLR)
    sync
    isync
    mtspr   L1CSR1,r6
    isync

cacheSetDone1:
    sync
    mfspr   r7,L1CSR1
    and.    r7,r7,r6
    bne cacheSetDone1

    lis r6,HI(_PPC_L1CSR_CPE|_PPC_L1CSR_E)
    ori r6,r6,LO(_PPC_L1CSR_CPE|_PPC_L1CSR_E)
    sync
    isync
    mtspr   L1CSR1,r6
    isync

cacheSetDone2:
    sync
    mfspr   r7,L1CSR1
    and.    r7,r7,r6
    beq cacheSetDone2

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

#endif /* FSL_ERRATA_CCF_A004510 || FSL_ERRATA_CCF_A004511 */

skipCcfRecfg:

                                    /* turn off exceptions   */
    mfmsr   r3                      /* r3 = msr              */
    INT_MASK (r3, r4)               /* mask EE and CE bit    */
    rlwinm  r4, r4, 0, 20, 18       /* turn off _PPC_MSR_ME  */
    mtmsr   r4                      /* msr = r4              */
    isync                           /* Per Table 3-2 e500mc Core RM Rev J note 2 */

    mfspr   r0, MCSR
    mtspr   MCSR, r0                /* Write set bits to clear */

    xor     r0, r0, r0
    addi    r1, r0, -1

    mtspr   DEC, r0
    mtspr   TBL, r0
    mtspr   TBU, r0
    mtspr   TSR, r1
    mtspr   TCR, r0
    mtspr   ESR, r0                 /* clear Exception Syndrome Reg */
    mtspr   XER, r0                 /*  clear Fixed-Point Exception Reg */
    lis   r1,0xfff0
    ori   r1,r1,0x0000
    mtspr IVPR, r1

    /*
     * TLB1 #0.  ROM - 0xff000000 -> 0xff000000., 16MB
     */

    addis  r4,0,0x1000           /* TLBSEL = TLB1(CAM) , ESEL = 0 */
    ori    r4,r4,0x0000
    addis  r5, 0, HI(MMU_STATE_VALID|MMU_STATE_IPROT)
    ori    r5,r5,_MMU_TLB_SZ_16M  /* TS = 0, TSIZE = 16 MByte page size*/
    addis  r6, 0, HI(FLASH_BASE_ADRS)   /* EPN */
    ori    r6,r6,MMU_ROM_ACCESS
    addis  r7, 0, HI(FLASH_BASE_ADRS)   /* RPN */
    ori    r7,r7,0x0015          /* Supervisor XWR*/

    bl     setTlbFn
    bl     initCacheFn

/* enable timebase for core 1 */
    mfspr   r6, PIR
    andi.   r6, r6, 0xf0
    cmpwi   r6, 0
    bne     prepareRomStart
    WRITEADR(r6,r7,P4080_CTBENR(CCSBAR),1)

prepareRomStart:
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
    sub     r6, r7, r6        /* routine - entry point */
    add     r6, r6, r8         /* + ROM base */
    mtspr   LR, r6
    mr      r3, r2
    blr

#if defined(FSL_ERRATA_CCF_A004510) || defined(FSL_ERRATA_CCF_A004511)

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
     * The region between and including A and B must be locked in the I-cache.
     * This ensures that no transaction,other than the write to the register,
     * is generated into the system while the core is executing the sequence.
     * The region between and including A and B fill two cache lines,therefore,
     * lock two cache lines into I-cache.
     */

    /* Backup lr to r8 */

    mflr    r8
    bl  currentAdr
currentAdr:
    mflr    r6

    /* calculate address which will be locked in the I-cache. */

    addi    r7, r6, lockInICache - currentAdr

    sync
    mfspr   r6, L1CSR1
    rlwinm  r6, r6, 0, 22, 20
    sync
    isync
    mtspr   L1CSR1, r6 /* I-cache enable to lock */
    isync

    icbtls  0, 0, r7
    sync
    mfspr   r6, L1CSR1
lockDone1:
    andi.   r6, r6, _PPC_L1CSR_UL
    bne lockDone1

    addi    r9, r7, 64  /* calculate the subsequent cacheline address */
    icbtls  0, 0, r9
    sync
    mfspr   r6, L1CSR1
lockDone2:
    andi.   r6, r6, _PPC_L1CSR_UL
    bne lockDone2

    b lockInICache

    .balign 64

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

    /* Unlock above two cache lines in I-Cache */

    icblc  0, 0, r7
    icblc  0, 0, r9

    mtlr    r8
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

#endif /* FSL_ERRATA_CCF_A004510 || FSL_ERRATA_CCF_A004511 */

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

    bl     initCacheFn

    /* Clear SRR0, CSRR0, MCSRR0, SRR1, CSRR1 , MCSRR1, IVPR */

    mfspr   r0, MCSR
    mtspr   MCSR, r0

    xor   r0,r0,r0

    mtspr SRR0,  r0
    mtspr SRR1,  r0
    mtspr CSRR0, r0
    mtspr CSRR1, r0
    mtspr MCSRR0,r0
    mtspr MCSRR1,r0
    mtspr ESR,   r0
    mtspr DEAR,  r0

    mfmsr   r4
    andi.   r5, r4, _PPC_MSR_DE
    mtmsr   r5                 /* Per Table 3-4 e500mc Core RM Rev J note 1 */
    isync                      /* Per Table 3-3 e500mc Core RM Rev J row 1 */
    mtspr DBCR0, r0
    mtspr DBCR1, r0
    mtspr DBCR2, r0
    mtspr IAC1,  r0
    mtspr IAC2,  r0
    mtspr DAC1,  r0
    mtspr DAC2,  r0
    mfspr r1, DBSR
    mtspr DBSR, r1
    isync                      /* Per Table 3-4 e500mc Core RM Rev J note 1 */
    mtmsr   r4                 /* Per Table 3-4 e500mc Core RM Rev J note 1 */
    isync                      /* Per Table 3-3 e500mc Core RM Rev J row 1 */

    isync                      /* Per Table 3-2 e500mc Core RM Rev J row 15 */
    mtspr PID0, r0
    isync                      /* Per Table 3-3 e500mc Core RM Rev J row 15 */

    mtspr TCR,  r0
    mtspr 1013, r0

    mtspr MAS4, r0
    mtspr MAS5, r0
    mtspr MAS6, r0
    mtspr MAS7, r0
    mtspr MAS8, r0
    isync                      /* Per Table 3-3 e500mc Core RM Rev J note 1 */

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

#ifdef FSL_ERRATA_FPU_A003999
    /*
     * Anticipated behavior of the A-003999 (Running Floating Point instructions
     * requires special initialization) errata:
     * Any floating point arithmetic operation may result in an incorrect result when
     * it occurs in combination with a rare sequence of additional internal core
     * events.  Floating point load/store instructions are not affected and will not
     * corrupt memory.
     *
     * To avoid the behavior Freescale recommends the following workaround:
     *
     * Perform a read-modify-write to set bit 7 to a one in SPR977 before executing
     * any floating point arithmetic operation.  The bit can be set when setting MSR
     * [FP] and can be cleared when clearing MSR[FP].  Alternatively, the bit can be
     * set once at boot time and never cleared.  There will be no performance
     * degradation due to setting this bit.
     */

    mfspr r3, 977
    oris  r3, r3, 0x0100
    mtspr 977, r3
#endif /* FSL_ERRATA_FPU_A003999 */

    li    r2,0x06	      /* L2TLB0_FI | L2TLB1_FI */
    mtspr MMUCSR0, r2
    isync                     /* Per Table 3-4 e500mc Core RM Rev J row 10 */

    li    r3,4
    li    r4,0
    tlbivax r4,r3
    nop
    isync                     /* Per Table 3-3 e500mc Core RM Rev J notes 2,3 */


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
     * TLB1 #0.  ROM - 0xff000000 -> 0xffffffff.
     * 16MB
     * Attributes: SX/SW/SR **PROTECTED**
     */

    addis  r4,0,0x1000           /* TLBSEL = TLB1(CAM) , ESEL = 0 */
    addis  r5, 0, HI(MMU_STATE_VALID|MMU_STATE_IPROT)
    ori    r5,r5,_MMU_TLB_SZ_16M  /* TS = 0, TSIZE = 16 MByte page size*/
    addis  r6, 0, HI(FLASH_BASE_ADRS)  /* EPN */
    ori    r6,r6, MMU_ROM_ACCESS
    addis  r7, 0, HI(FLASH_BASE_ADRS)  /* RPN */
    ori    r7,r7,0x0015          /* Supervisor XWR*/

    bl     setTlbFn

    /*
     * TLB1 #1.  Main SDRAM - Not Cached
     *           PHYS_MEM_START -> +1 GB size
     * Attributes: UX/UW/UR/SX/SW/SR
     */

    li     r7, LOCAL_MEM_ERPN      /* upper 4 bits of physical address */
    mtspr  MAS7, r7

    addis  r4,0,0x1001             /* TLBSEL = TLB1(CAM) , ESEL = 1*/
    addis   r5, 0, HI(MMU_STATE_VALID|MMU_STATE_IPROT)
    ori    r5,r5,_MMU_TLB_SZ_1G    /* TS = 0, TSIZE = 1GByte page size*/
    addis  r6,0,HI(PHYS_MEM_START) /* EPN = PHYS_MEM_START */
    ori    r6,r6,MMU_RAM_ACCESS    /* WIMGE */
    addis  r7,0,HI(PHYS_MEM_START) /* RPN = PHYS_MEM_START */
    ori    r7,r7,0x003f            /* User/Supervisor XWR*/

    bl     setTlbFn

    li     r7, 0                 /* clear upper 4 bits */
    mtspr  MAS7, r7

    /*
     * TLB1 #2.  CCSBAR
     * Attributes: UX/UW/UR/SX/SW/SR
     */

    addis  r4,0,0x1002          /* TLBSEL = TLB1(CAM) , ESEL = 2*/
    addis   r5, 0,  HI(MMU_STATE_VALID|MMU_STATE_IPROT)
    ori    r5,r5,_MMU_TLB_SZ_16M /* TS = 0, TSIZE = 16 MByte page size*/
    addis  r6,0,HI(CCSBAR) /* EPN = CCSBAR */
    ori    r6,r6,MMU_IO_ACCESS   /* WIMGE */
    addis  r7,0,HI(CCSBAR) /* RPN = CCSBAR */
    ori    r7,r7,0x0015          /* Supervisor XWR*/

    bl     setTlbFn

    mfspr    r6, PIR
    rlwinm     r6, r6, 27, 29, 31
    cmpwi   r6, 0
    beq     doCcsrbar

    /* running on CPU1-7 */
    /* set up initial stack pointer */

    rlwinm  r6, r6, CPUn_SPACE_SIZE_SHIFT,0,31
                 /* mult. cpunum by CPUn_SPACE_SIZE */
    lis    r3, HI(CPUn_SPACE_BASE_ADRS)
    ori     r3, r3, LO(CPUn_SPACE_BASE_ADRS)
    subf    r6, r6, r3            /* equiv. to CPUn_SPACE */

waitForStartSet:
    lwz     r3, 8(r6)            /* CPUn_INIT_START_ADR */

    /*
     * Check that init start address is not zero before jumping
     * This allows cpu1 to reboot and wait for cpu0 to reload image
     */

    lis     r4,HI(MP_MAGIC_RBV)
    ori     r4, r4, LO(MP_MAGIC_RBV)
    cmpw    r3, r4
    beq     waitForStartSet

    lwz     sp, 4(r6)            /* CPUn_STACK_START_ADR */
    lwz     r7, 0x0c(r6)         /* CPUn_LOCAL_MEM_ERPN  */
    lwz     r8, 0x10(r6)         /* CPUn_LOCAL_MEM_RPN   */
    /*
     * The masking below indicates bits that must be cleared per spec.
     * However, if bits are set other than how the mask indicates then
     * wrload placed the image where it won't boot anyway.
     *
     * andi.   r7, r7, 0x000F                -- low 4-bits are valid
     * andis.  r8, r8, IMAGE_MASK            -- For a IMAGE_BOUNDARY size TLB
     */

    /*
     * TLB1 #1. Execution space
     * Only map IMAGE_BOUNDARY to allow loading at any e.g. 256MB boundary.
     * The runtime kernel can later expand the range when
     * sysStaticTlbDesc becomes active in sysHwInit().
     */

    mtspr   MAS7, r7                    /* Extended RPN */

    addis   r4, 0, 0x1001               /* TLB1 2nd entry (#1) */

    addis   r5, 0, HI(MMU_STATE_VALID|MMU_STATE_IPROT)
    ori     r5, r5, IMAGE_BOUNDARY    /* TS = 0 */

    andis.  r6, r3, IMAGE_MASK   /* Mask Start addr for the IMAGE_BOUNDARY */
    andis.  r7, r8, IMAGE_MASK
    add     r7, r7, r6           /* RPN(r8) = EPN(r4) + load bias(r8) */
    ori     r6,r6,MMU_RAM_ACCESS /* WIMGE */
    ori     r7,r7,0x003f         /* User/Supervisor XWR*/

    bl      setTlbFn

#if (    (MIPC_SM_SYSTEM_POOL  > (PHYS_MEM_START + 0x100000)) \
      || (PHYS_MEM_START != 0) )
#error "TLB3 mapping assumption broken"
#endif

    /*
     * TLB1 #3.  For RESERVED_ATOMICS_ADRS (0x0000_5000,+0x0_1000) during boot.
     * Note: 1MB is below RAM_LOW_ADRS
     */
    li      r4, 0                /* clear upper 4 bits (for the next guy)*/
    mtspr   MAS7, r4                    /* ERPN = 0 */

    addis   r4, 0, 0x1003               /* TLB1 4th entry (#3) */
    addis   r5, 0, HI(MMU_STATE_VALID|MMU_STATE_IPROT)
    ori     r5, r5, _MMU_TLB_SZ_1M      /* TS = 0 */
    addis   r6, 0, HI(0x00000000)
    ori     r6, r6, MMU_RAM_ACCESS      /* WIMGE */
    addis   r7, 0, HI(0x00000000)
    ori     r7, r7, 0x003f              /* Supervisor XWR*/

    bl      setTlbFn

    /*
     * To support the wrload -loadbias option, pass a copy of ERPN/RPN to
     * secondary (same virtual, different physical address) so that
     * sysHwInit() can patch up sysStaticTlbDesc[] & sysPhysMemDesc[]
     *
     * stw    0x0c(r6) <-- CPUn_LOCAL_MEM_ERPN
     * stw    0x10(r6) <-- CPUn_LOCAL_MEM_RPN
     */

branchToSysInit:
    stwu    r0, -FRAMEBASESZ(sp)    /* save one stack frame (after TLB init) */
    mtctr   r3                      /* branch to CPU1_INIT_START_ADR */
    bctr                            /* sysInit sets BOOT start type */

doCcsrbar:

    /* Setup the memory mapped register address */

    WRITEADR(r6,r7,P4080_LAWBARL8(CCSBAR_RESET),CCSBAR)
    sync
    WRITEADR(r6,r7,P4080_LAWAR8(CCSBAR_RESET),
             LAWAR_ENABLE | LAWAR_TGTIF_RESERVE | LAWAR_SIZE_16MB )
    LOADVAR(r7, P4080_LAWAR8(CCSBAR_RESET))
    isync    /* ensure LAWAR8 is set */

    lis     r7,HI(CCSBAR_RESET)
    ori     r7,r7,LO(CCSBAR_RESET)
    lwz     r4, (CCSRBARH_OFFSET)(r7)

    lis     r7,HI(CCSBAR_RESET)
    ori     r7,r7,LO(CCSBAR_RESET)
    lwz     r4, (CCSRBARL_OFFSET)(r7)

    lis     r6,HI(CCSBAR)
    ori     r6,r6, LO(CCSBAR)

    xor     r5,r5,r5
    lis     r7,HI(CCSBAR_RESET)
    ori     r7,r7,LO(CCSBAR_RESET)

    stw     r5,(CCSRBARH_OFFSET)(r7)

    stw     r6,(CCSRBARL_OFFSET)(r7)

    sync

    lis     r5,HI(CCSBAR_COMMIT)
    ori     r5,r5, LO(CCSBAR_COMMIT)

    stw     r5,(CCSRAR_OFFSET)(r7)

    sync

    WRITEADR(r6,r7,P4080_LAWBARL8(CCSBAR),0x0)
    sync
    WRITEADR(r6,r7,P4080_LAWAR8(CCSBAR),0x0)
    LOADVAR(r7, P4080_LAWAR8(CCSBAR))
    isync    /* ensure LAWAR8 is set */

    lis     r5,0xffff
    ori     r5, r5,0xf800
    lwz     r4, 0 (r5)

    li      r4,0x200
    mtctr   r4

ccsrbarWait:
    nop
    bdnz    ccsrbarWait


    WRITEADR(r6,r7,P4080_LAWBARL0(CCSBAR),FLASH_BASE_ADRS)
    sync
    WRITEADR(r6,r7,P4080_LAWAR0(CCSBAR),
         LAWAR_ENABLE | LAWAR_TGTIF_LBC | LAWAR_SIZE_16MB )
    LOADVAR(r7, P4080_LAWAR0(CCSBAR))
    isync    /* ensure LAWAR0 is set */

    /* load BR0 */
    WRITEADR(r6,r7,P4080_BR0 (CCSBAR),(FLASH_BASE_ADRS | 0x1001))

    /* load OR0 */
    WRITEADR(r6,r7,P4080_OR0 (CCSBAR),(FLASH_ADRS_MASK | FLASH_OR0_VALUE))

    sync    /* ensure BR0 and OR0 are set */

    /* Initialise the Local Bus Controller */
    li      r4,0x200
    mtctr   r4
    WRITEADR(r6,r7,P4080_LBCR(CCSBAR),0x40000000)
    WRITEADR(r6,r7,P4080_LCRR(CCSBAR),0x80000000 | LCRR_CLOCK_DIV )

    sync    /* ensure LBCR/LCRR are set */

dllDelay4:
    nop
    bdnz    dllDelay4

#ifdef INCLUDE_LBC_CS3 /* PIXIS */

    WRITEADR(r6,r7,P4080_LAWBARL3(CCSBAR),LBC_CS3_LOCAL_ADRS)
    sync
    WRITEADR(r6,r7,P4080_LAWAR3(CCSBAR),
         LAWAR_ENABLE | LAWAR_TGTIF_LBC | LAWAR_SIZE_4KB )
    LOADVAR(r7, P4080_LAWAR3(CCSBAR))
    isync

    /* PIXIS - registers */

    /* load OR3 */

    WRITEADR(r6,r7,P4080_OR3 (CCSBAR),(LBC_CS3_LOCAL_SIZE_MASK & 0xffff0000) | 0x0ff7)

    /* load BR3 */
    WRITEADR(r6,r7,P4080_BR3 (CCSBAR),(LBC_CS3_LOCAL_ADRS & 0xffff0000) | \
         0x0801)

    sync    /* ensure BR3 and OR3 are set */

#endif /* INCLUDE_LBC_CS3 */

    /* Local Access Window for first half of RAM */

    WRITEADR(r6,r7,P4080_LAWBARL30(CCSBAR),
         DDR_SDRAM_LOCAL_ADRS >> LAWBAR_ADRS_SHIFT)
    
    WRITEADR(r6,r7,P4080_LAWBARH30(CCSBAR),
         DDR_SDRAM_LOCAL_ADRS_H)
    sync
    WRITEADR(r6,r7,P4080_LAWAR30(CCSBAR),
         LAWAR_ENABLE | DDR_SDRAM_LAWAR_TGTIF | DDR_SDRAM_LAWAR_SIZE)
    LOADVAR(r7, P4080_LAWAR30(CCSBAR))
    isync    /* ensure LAWAR30 is set */

    /* Local Access Window for second half of RAM */

#ifdef DDR_SDRAM_LAWAR_TGTIF2
    WRITEADR(r6,r7,P4080_LAWBARL31(CCSBAR), LAWBARL2_VALUE);

    WRITEADR(r6,r7,P4080_LAWBARH31(CCSBAR),
         DDR_SDRAM_LOCAL_ADRS2_H)
    sync
    WRITEADR(r6,r7,P4080_LAWAR31(CCSBAR),
         LAWAR_ENABLE | DDR_SDRAM_LAWAR_TGTIF2 | DDR_SDRAM_LAWAR_SIZE)
    LOADVAR(r7, P4080_LAWAR31(CCSBAR))
    isync    /* ensure LAWAR31 is set */

#endif /* DDR_SDRAM_LAWAR_TGTIF2 */

    /* Initialize the 1st DDR Memory controller */
        lis    r6, HI(DDRBA)
        ori    r6, r6, LO(DDRBA)        /* r6 = DDR base */

    WRITEOFFSET(r6,r7,(CS0_BNDS), CS0_BNDS_VALUE)
    WRITEOFFSET(r6,r7,(CS1_BNDS), CS1_BNDS_VALUE)
    WRITEOFFSET(r6,r7,(CS2_BNDS), CS2_BNDS_VALUE)
    WRITEOFFSET(r6,r7,(CS3_BNDS), CS3_BNDS_VALUE)
    WRITEOFFSET(r6,r7,(CS0_CONFIG), CS0_CONFIG_VALUE)
    WRITEOFFSET(r6,r7,(CS1_CONFIG), CS1_CONFIG_VALUE)
    WRITEOFFSET(r6,r7,(CS2_CONFIG), CS2_CONFIG_VALUE)
    WRITEOFFSET(r6,r7,(CS3_CONFIG), CS3_CONFIG_VALUE)

    b     ddrCommon

ddr2:
    /* Initialize the 2nd DDR Memory controller */

    WRITEOFFSET(r6,r7,(CS0_BNDS), CS0_BNDS_2DDR_VALUE)
    WRITEOFFSET(r6,r7,(CS1_BNDS), CS1_BNDS_2DDR_VALUE)
    WRITEOFFSET(r6,r7,(CS2_BNDS), CS2_BNDS_2DDR_VALUE)
    WRITEOFFSET(r6,r7,(CS3_BNDS), CS3_BNDS_2DDR_VALUE)
    WRITEOFFSET(r6,r7,(CS0_CONFIG), CS0_CONFIG_2DDR_VALUE)
    WRITEOFFSET(r6,r7,(CS1_CONFIG), CS1_CONFIG_2DDR_VALUE)
    WRITEOFFSET(r6,r7,(CS2_CONFIG), CS2_CONFIG_2DDR_VALUE)
    WRITEOFFSET(r6,r7,(CS3_CONFIG), CS3_CONFIG_2DDR_VALUE)

ddrCommon:
    WRITEOFFSET(r6,r7,(DDR_SDRAM_CLK_CTRL), DDR_SDRAM_CLK_CTRL_VALUE)

    WRITEOFFSET(r6,r7,(DDR_DATA_INIT), DDR_DATA_INIT_VALUE)
    WRITEOFFSET(r6,r7,(DDR_SDRAM_MODE_CFG_2), DDR_SDRAM_MODE_CFG_2_VALUE)
    WRITEOFFSET(r6,r7,(DDR_SDRAM_MD_CNTL), DDR_SDRAM_MD_CNTL_VALUE)

    WRITEOFFSET(r6,r7,(TIMING_CFG_3), TIMING_CFG_3_VALUE)
    WRITEOFFSET(r6,r7,(TIMING_CFG_0), TIMING_CFG_0_VALUE)
    WRITEOFFSET(r6,r7,(TIMING_CFG_1), TIMING_CFG_1_VALUE)
    WRITEOFFSET(r6,r7,(TIMING_CFG_2), TIMING_CFG_2_VALUE)
    WRITEOFFSET(r6,r7,(TIMING_CFG_4), TIMING_CFG_4_VALUE)
    WRITEOFFSET(r6,r7,(TIMING_CFG_5), TIMING_CFG_5_VALUE)
    WRITEOFFSET(r6,r7,(DDR_ZQ_CNTL), DDR_ZQ_CNTL_VALUE)
    WRITEOFFSET(r6,r7,(DDR_WRLVL_CNTL), DDR_WRLVL_CNTL_VALUE)
    WRITEOFFSET(r6,r7,(DDR_SDRAM_CFG), DDR_SDRAM_CFG_VALUE1)
    WRITEOFFSET(r6,r7,(DDR_SDRAM_CFG_2), DDR_SDRAM_CFG_2_VALUE1)
    WRITEOFFSET(r6,r7,(DDR_SDRAM_MODE_CFG), DDR_SDRAM_MODE_CFG_VALUE)
    WRITEOFFSET(r6,r7,(DDR_SDRAM_INTERVAL), DDR_SDRAM_INTERVAL_VALUE)

    /* This workaround needs to be applied only on rev 2.0 silicon. */
    
    mfspr   r4,SVR    /* get the system version from the SVR */
    rlwinm  r5,r4,28,28,31
    cmpwi   r5,2
    bne     skip3474

    /*
     * Anticipated behavior of the A-003474 (Internal DDR calibration circuit is not
     * supported) errata:
     * The DDR controller has several internal automatic calibration circuits enabled
     * by default.  To ensure consistent timing results, Freescale recommends this
     * feature be disabled in future customer products.  There is no anticipated
     * impact to parts that are already operating in the field.
     *
     * To avoid the behavior Freescale recommends the following workaround:
     *
     * Prior to setting DDR_SDRAM_CFG[MEM_EN] = 1, disable auto-calibration
     * by doing the following:
     *
     * .Write a value of 0x0000_0015 to the register at offset CCSRBAR + DDR
     *  OFFSET + 0xf30.
     * .Write a value of 0x2400_0000 to the register at offset CCSRBAR + DDR
     *  OFFSET + 0xf54.
     */

    WRITEOFFSET(r6,r7,(0xF30), 0x00000015)
    WRITEOFFSET(r6,r7,(0xF54), 0x24000000)

skip3474:
    WRITEOFFSET(r6,r7,(DDR_SDRAM_CFG), DDR_SDRAM_CFG_VALUE2)

#ifdef DDR_SDRAM_LAWAR_TGTIF2

    lis    r7, HI(DDRBA2)
    ori    r7, r7, LO(DDRBA2)
    cmpw    r6, r7
    mr    r6, r7
    bne    ddr2

    /* Initialize the 2nd DDR Memory controller */
    WRITEOFFSET(r6,r7,(DDR_SDRAM_CFG_2), DDR_SDRAM_CFG_2_VALUE2)

2:  lwz    r7, DDR_SDRAM_CFG_2(r6)
    rlwinm. r7, r7, 0, 27, 27
    sync
    bne     2b
#endif /* DDR_SDRAM_LAWAR_TGTIF2 */

    lis    r6, HI(DDRBA)
    ori    r6, r6, LO(DDRBA)        /* r6 = DDR base */

    /* Initialize the 1st DDR Memory controller */
    WRITEOFFSET(r6,r7,(DDR_SDRAM_CFG_2), DDR_SDRAM_CFG_2_VALUE2)

    /* wait here until the DRAM init is complete (see ROMSTART_BOOT_CLEAR) */

1:  lwz    r7, DDR_SDRAM_CFG_2(r6)
    rlwinm. r7, r7, 0, 27, 27      /* D_INIT bit clears when done */
    bne     1b


    /*
     * Now that memory is stable we reset TLB entries for standard
     * operation.  Let the booted image itself map the 2nd DRAM's TLB.
     */
    /*
     * TLB1 #1.  Main SDRAM
     */
    li     r7, LOCAL_MEM_ERPN    /* upper 4 bits of physical address */
    mtspr  MAS7, r7

    addis  r4,0,0x1001           /* TLBSEL = TLB1(CAM) , ESEL = 1*/
    addis  r5, 0, HI(MMU_STATE_VALID|MMU_STATE_IPROT)
    ori    r5,r5,_MMU_TLB_SZ_1G  /* TS = 0, TSIZE = 1GByte page size*/
    addis  r6,0,HI(PHYS_MEM_START) /* EPN */
    ori    r6,r6,MMU_RAM_ACCESS    /* WIMGE */
    addis  r7,0,HI(PHYS_MEM_START) /* RPN */
    ori    r7,r7,0x003f
    bl     setTlbFn

    li     r7, 0             /* clear upper 4 bits */
    mtspr  MAS7, r7
    isync           /* Per Table 3-3 e500mc Core RM Rev J note 2 */

    /* TLB1 #4: DDR memory space: 0x40000000 -> 0x7FFFFFFF (1024MB) */

#if (PHYS_MEM_SIZE > 0x40000000)
    li     r7, LOCAL_MEM_ERPN    /* upper 4 bits of physical address */
    mtspr  MAS7, r7

    addis  r4,0,0x1004                    /* TLBSEL = TLB1(CAM) , ESEL = 1*/
    addis  r5, 0, HI(MMU_STATE_VALID|MMU_STATE_IPROT)
    ori    r5,r5,_MMU_TLB_SZ_1G              /* TS = 0*/
    addis  r6,0,HI(PHYS_MEM_START+0x40000000)/* EPN */
    ori    r6,r6,MMU_RAM_ACCESS              /* WIMGE */
    addis  r7,0,HI(PHYS_MEM_START+0x40000000)/* RPN */
    ori    r7,r7,0x003f
    bl     setTlbFn

    li     r7, 0             /* clear upper 4 bits */
    mtspr  MAS7, r7
    isync           /* Per Table 3-3 e500mc Core RM Rev J note 2 */
#endif

#if (PHYS_MEM_SIZE > 0x80000000)
#error "PHYS_MEM_SIZE > 0x80000000, currently unsupported"
#endif
    b  cold

setTlbFn:
    mtspr  MAS0, r4
    mtspr  MAS1, r5
    mtspr  MAS2, r6
    mtspr  MAS3, r7
    isync           /* Per Table 3-3 e500mc Core RM Rev J note 1 */
    tlbwe
    isync           /* Per Table 3-3 e500mc Core RM Rev J note 2 */
    blr

initCacheFn:
    /* Beginning of e500mc Core RM Rev J section 11.5 code: */

    /* L1 data cache */
    xor   r4, r4, r4        /* set r4 to 0  */
    ori   r5, r4, 0x0002    /* set CFI bit  */
    sync
    isync                   /* synchronize setting of L1CSR0  */
    mtspr L1CSR0, r5        /* flash invalidate L1 data cache */
    isync                   /* synchronize setting of L1CSR0  */

dloop:
    mfspr r4, L1CSR0        /* get current value */
    and.  r4, r4, r5        /* test CFI bit      */
    bne   dloop             /* check again if not complete    */
    isync                   /* discard prefetched instructions*/

    /* L1 instruction cache */
    xor   r4, r4, r4        /* set r4 to 0  */
    ori   r5, r4, 0x0002    /* set ICFI bit */
    sync
    isync                   /* synchronize setting of L1CSR1 */
    mtspr L1CSR1, r5        /* flash invalidate L1 instruction cache */
    isync                   /* synchronize setting of L1CSR1 */

iloop:
    mfspr r4, L1CSR1        /* get current value */
    and.  r4, r4, r5        /* test ICFI bit     */
    bne   iloop             /* check again if not complete     */
    isync                   /* discard prefetched instructions */

    /* End of e500mc Core RM Rev J section 11.5 code */

    /*
     * Currently causes problems if the data cache is turned on here:
     * fsl_p5020:  Works fine but reboot hangs.
     * wrSbcP4080: Boots & wrloads images from a start script then reboots.
     * This likely requires restructuring to initialize TLBs cleanly for
     * the reboot() case.
     */
#if 0
    li    r5, 0x0001
    sync                    /* Per Table 3-2 e500mc Core RM Rev J row 10 */
    isync                   /* Per Table 3-2 e500mc Core RM Rev J row 10 */
    mtspr L1CSR0, r5        /* enable the L1 data cache */
    isync                   /* Per Table 3-2 e500mc Core RM Rev J row 10 */
#endif

    li    r5, 0x0001
    sync                    /* Per Table 3-2 e500mc Core RM Rev J row 10 */
    isync                   /* Per Table 3-2 e500mc Core RM Rev J row 10 */
    mtspr L1CSR1, r5        /* enable the L1 instruction cache */
    isync                   /* Per Table 3-2 e500mc Core RM Rev J row 10 */

    blr

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
    b    resetEntry
FUNC_END(resetVector)

