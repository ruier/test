/* romInit.s - ROM bootcode for fsl_p2020_rdb BSP */

/*
 * Copyright (c) 2009-2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
01g,12apr12,wqi  Fixed SMP image booting issue. (WIND00314083)
01f,29dec11,mpc  Initialized MMUCSR0 with value 0x06. (WIND00319727)
01e,22aug11,ers  CQ253685:Fix memory mapping bugs and isolate the cores'
                 memory from each other.
01d,18jan11,agf  CQ234239: change DDR timing to support rev D hardware
01c,16sep10,r_z  Modify the OR0 value to improve the flash performance.
01b,20jan10,syt  Add AMP support.
01a,05oct09,syt  Created from fsl_p2020_ds/01a.
*/

#define _ASMLANGUAGE
#include <vxWorks.h>
#include <sysLib.h>
#include <asm.h>
#include <regs.h>
#include <arch/ppc/mmuE500Lib.h>
#include "config.h"

#ifndef MMU_STATE_IPROT
#define MMU_STATE_IPROT 0x40000000
#endif

#define MMU_ROM_ACCESS (MMU_STATE_CACHEABLE_WRITETHROUGH | MMU_STATE_CACHEABLE | \
                        MMU_STATE_MEM_COHERENCY          | MMU_STATE_GUARDED)

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
#define LOADVAR(reg, addr32)            \
    addis reg, r0, HIADJ(addr32);       \
    lwz reg, LO(addr32)(reg)

#define WRITEADR(reg1,reg2,addr32,val)  \
    lis reg1, HI(addr32);               \
    ori reg1, reg1, LO(addr32);         \
    lis reg2, HI(val);                  \
    ori reg2, reg2, LO(val);            \
    stw reg2, 0(reg1)

#define WRITE_BYTE(reg1,reg2,addr32,val)\
    lis reg1, HI(addr32);               \
    ori reg1, reg1, LO(addr32);         \
    lis reg2, 0x00;                     \
    ori reg2, reg2, LO(val);            \
    stb reg2, 0(reg1)

#define WRITEOFFSET(regbase,reg2,offset,val) \
    lis reg2, HI(val);                  \
    ori reg2, reg2, LO(val);            \
    stw reg2, offset(regbase);

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

    li  r2, BOOT_COLD
    b   start

warm:
    mr  r2, r3

    mfspr   r6, HID1
    andi.   r6, r6, 1
    cmpwi   r6, 1
    bne     start

    /*
     * running on CPU1
     * jump to sysInit for AMP - never get here for SMP
     */

waitForStartSetWarm:
    LOADVAR (r3, CPU1_INIT_START_ADR)

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
    mtspr   XER, r0                 /* clear Fixed-Point Exception Reg */

    /*
     * TLB1 #0.  ROM - writethrough cached 0xff000000 -> 0xff000000.
     * 16MB
     * Attributes: SX/SW/SR **PROTECTED**
     */

    addis   r4, 0, 0x1000           /* TLB1 entry#0 */
    ori     r4, r4, 0x0000
    mtspr   MAS0, r4
    addis   r5, 0, HI(MMU_STATE_VALID|MMU_STATE_IPROT)
    ori     r5, r5, _MMU_TLB_SZ_16M /* TS = 0, TSIZE = 16 MByte */
    mtspr   MAS1, r5
    addis   r6, 0, HI(FLASH_BASE_ADRS)   /* EPN */
    ori     r6, r6, MMU_ROM_ACCESS
    mtspr   MAS2, r6
    addis   r7, 0, HI(FLASH_BASE_ADRS)   /* RPN */
    ori     r7, r7, 0x0015          /* Supervisor XWR */
    mtspr   MAS3, r7
    tlbwe
    tlbsync

    xor     r6, r6, r6
    msync
    isync
    mtspr   L1CSR0, r6              /* Invalidate the Data cache */
    li      r6, 0x0000
    msync
    isync
    mtspr   L1CSR0, r6              /* Disable the Data cache */
    li      r6, 0x0002
    msync
    isync
    mtspr   L1CSR1, r6              /* Disable the Instrunction cache */
    li      r6, 0x0002
    msync
    isync
    mtspr   L1CSR1, r6              /* Invalidate the Instruction cache */
    isync
    li      r6, 0x0000
    msync
    isync
    mtspr   L1CSR1, r6              /* Temp disable Instruction cache*/

    isync

    li      r7, 0x0001
    msync
    isync
    mtspr   L1CSR1, r7              /* enable the instruction cache */


    msync
    isync

    xor     r6, r6, r6
    xor     r7, r7, r7
    mullw   r7, r7, r6
    lis     sp, HI(STACK_ADRS)
    ori     sp, sp, LO(STACK_ADRS)
    addi    sp, sp, -FRAMEBASESZ
    lis     r6, HI(romInit)
    ori     r6, r6, LO(romInit)
    lis     r7, HI(romStart)
    ori     r7, r7, LO(romStart)
    lis     r8, HI(ROM_TEXT_ADRS)
    ori     r8, r8, LO(ROM_TEXT_ADRS)
    sub     r6, r7, r6              /* routine - entry point */
    add     r6, r6, r8              /* + ROM base */
    mtspr   LR, r6
    mr      r3, r2
    blr

FUNC_END(_romInit)
FUNC_END(romInit)


/*******************************************************************************
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

    xor     r6, r6, r6
    msync
    isync
    mtspr   L1CSR0, r6      /* Invalidate the Data cache */
    li  r6, 0x0000
    msync
    isync
    mtspr   L1CSR0, r6      /* Disable the Data cache */
    li  r6, 0x0002
    msync
    isync
    mtspr   L1CSR1, r6      /* Disable the Instrunction cache */
    li  r6, 0x0002
    msync
    isync
    mtspr   L1CSR1, r6      /* Invalidate the Instruction cache */
    isync
    li  r6, 0x0000
    msync
    isync
    mtspr   L1CSR1, r6      /* temp disabled Enable the Instruction cache*/

    isync
    li  r7, 0x0000
    msync
    isync
    mtspr   L1CSR1, r7      /* enable the instruction cache */

    msync
    isync

    /* Clear SRR0, CSRR0, MCSRR0, SRR1, CSRR1 , MCSRR1, IVPR */

    xor     r0,r0,r0

    mtspr   SRR0, r0
    mtspr   SRR1, r0
    mtspr   CSRR0, r0
    mtspr   CSRR1, r0
    mtspr   MCSRR0, r0
    mtspr   MCSRR1, r0
    mtspr   ESR, r0
    mtspr   MCSR, r0
    mtspr   DEAR, r0
    mtspr   DBCR0, r0
    mtspr   DBCR1, r0
    mtspr   DBCR2, r0
    mtspr   IAC1, r0
    mtspr   IAC2, r0
    mtspr   DAC1, r0
    mtspr   DAC2, r0

    mfspr   r1, DBSR
    mtspr   DBSR, r1

    mtspr   PID0, r0
    mtspr   PID1, r0
    mtspr   PID2, r0
    mtspr   TCR, r0
    mtspr   1013, r0

    mtspr   MAS4, r0
    mtspr   MAS6, r0
    isync

    lis     r1,0xfff0
    ori     r1,r1,0x0000
    mtspr   IVPR, r1

    /* Set up vector offsets */

    addi    r3, r0, 0x100
    mtspr   IVOR0, r3
    addi    r3, r0, 0x200
    mtspr   IVOR1, r3
    addi    r3, r0, 0x300
    mtspr   IVOR2, r3
    addi    r3, r0, 0x400
    mtspr   IVOR3, r3
    addi    r3, r0, 0x500
    mtspr   IVOR4, r3
    addi    r3, r0, 0x600
    mtspr   IVOR5, r3
    addi    r3, r0, 0x700
    mtspr   IVOR6, r3
    /* skipping IVOR7 0x800: no FPU on e500 */
    addi    r3, r0, 0x900
    mtspr IVOR8, r3
    /* skipping IVOR9 0xa00: no aux processor on e500 */
    addi    r3, r0, 0xb00
    mtspr   IVOR10, r3
    addi    r3, r0, 0xc00
    mtspr   IVOR11, r3
    addi    r3, r0, 0xd00
    mtspr   IVOR12, r3
    addi    r3, r0, 0xe00
    mtspr   IVOR13, r3
    addi    r3, r0, 0xf00
    mtspr   IVOR14, r3
    addi    r3, r0, 0x1000
    mtspr   IVOR15, r3
    addi    r3, r0, 0x1100       /* SPU is e500 specific */
    mtspr   IVOR32, r3
    addi    r3, r0, 0x1200       /* FP data is e500 specific */
    mtspr   IVOR33, r3
    addi    r3, r0, 0x1300       /* FP round is e500 specific */
    mtspr   IVOR34, r3
    addi    r3, r0, 0x1400       /* perf mon is e500 specific */
    mtspr   IVOR35, r3

    li      r2, 0x06		 /* L2TLB0_FI | L2TLB1_FI */
    mtspr   MMUCSR0, r2
    isync

    li      r3, 4
    li      r4, 0
    tlbivax r4, r3
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


    /* TLB1 #0. Flash space: 0xFF000000 -> 0xFFFFFFFF (16MB) */

    addis   r4, 0, 0x1000               /* TLB1 entry#0 */
    ori     r4, r4, 0x0000
    mtspr   MAS0, r4
    addis   r5, 0, HI(MMU_STATE_VALID|MMU_STATE_IPROT)
    ori     r5, r5, _MMU_TLB_SZ_16M     /* TS = 0, TSIZE = 16 MByte */
    mtspr   MAS1, r5
    addis   r6, 0, HI(FLASH_BASE_ADRS)  /* EPN */
    ori     r6, r6, MMU_ROM_ACCESS
    mtspr   MAS2, r6
    addis   r7, 0, HI(FLASH_BASE_ADRS)  /* RPN */
    ori     r7, r7, 0x0015              /* Supervisor XWR */
    mtspr   MAS3, r7
    tlbwe
    tlbsync

    /*
     * TLB1 #1. Memory space: 0x0000000 -> 0x3FFFFFFF (1GB)
     * Attributes: UX/UW/UR/SX/SW/SR
     */

    addis   r4, 0, 0x1001               /* TLB1 entry#1 */
    ori     r4, r4, 0x0000
    mtspr   MAS0, r4
    addis   r5, 0, HI(MMU_STATE_VALID|MMU_STATE_IPROT)
    ori     r5, r5, _MMU_TLB_SZ_1G      /* TS = 0, TSIZE = 1G Byte */
    mtspr   MAS1, r5
    addis   r6, 0, HI(PHYS_MEM_START)   /* EPN: 0 */
    ori     r6, r6, 0x000a              /* WIMGE = 01010 */
    mtspr   MAS2, r6
    addis   r7, 0, HI(PHYS_MEM_START)   /* RPN: 0 */
    ori     r7, r7, 0x003f              /* Supervisor XWR*/
    mtspr   MAS3, r7
    tlbwe
    tlbsync

    /* TLB1 #2: CCSR space 0xF3000000 -> 0xF30FFFFF (1MB) */

    addis   r4, 0,  0x1002              /* TLBSEL = TLB1(CAM) , ESEL = 3*/
    ori     r4, r4, 0x0000
    mtspr   MAS0,   r4
    addis   r5, 0,  HI(MMU_STATE_VALID|MMU_STATE_IPROT)
    ori     r5, r5, _MMU_TLB_SZ_1M      /* TS = 0, TSIZE = 1 MByte page size*/
    mtspr   MAS1,   r5
    addis   r6, 0,  HI(CCSBAR)          /* EPN = CCSBAR */
    ori     r6, r6, 0x000a              /* WIMGE = 01010 */
    mtspr   MAS2,   r6
    addis   r7, 0,  HI(CCSBAR)          /* RPN = CCSBAR */
    ori     r7, r7, 0x003f              /* Supervisor XWR*/
    mtspr   MAS3,   r7
    isync
    msync
    tlbwe
    tlbsync

    mfspr  r6, HID1
    andi.  r6, r6, 1
    cmpwi  r6, 1
    bne    doCcsrbar

waitForStartSet:
    LOADVAR (r3, CPU1_INIT_START_ADR)

    /*
     * Check that init start address is not zero before jumping
     * This allows cpu1 to reboot and wait for cpu0 to reload image
     */

    lis     r4, HI(MP_MAGIC_RBV)
    ori     r4, r4, LO(MP_MAGIC_RBV)
    cmpw    r3, r4
    beq     waitForStartSet

    LOADVAR (sp, CPU1_STACK_START_ADR)
    stwu    r0, -FRAMEBASESZ(sp)        /* save one frame stack */

    mtctr   r3                           /* branch to CPU1_INIT_START_ADR */
    li      r3, BOOT_WARM_AUTOBOOT
    bctr

doCcsrbar:
    /* Setup the memory mapped register address */

    isync
    sync

    lis     r6, HI(CCSBAR)
    ori     r6, r6, LO(CCSBAR)
    isync

    srwi    r5, r6, 12
    lis     r7, HI(CCSBAR_RESET)
    ori     r7, r7, LO(CCSBAR_RESET)
    lwz     r4, 0(r7)

    isync
    sync
    mbar 0

    stw     r5, 0(r7)

    sync
    isync
    mbar 0

    /* Perform a load of flash address */

    lis     r5, 0xffff
    ori     r5, r5,0xffc
    lwz     r4, 0(r5)
    isync

    li      r4, 0x2000
    mtctr   r4
ccsrbarWait:
    nop
    bdnz    ccsrbarWait

    /* read new CCSRBAR value */

    lis     r5, HI(CCSBAR)
    ori     r5, r5, LO(CCSBAR)
    lwz     r4, 0(r5)
    isync

    /* Local access window 0 (Nor Flash) initialize */

    WRITEADR(r6, r7, M85XX_LAWBAR0(CCSBAR), 0x000ff000)
    WRITEADR(r6, r7, M85XX_LAWAR0(CCSBAR), 0x80400017)

    WRITEADR(r6, r7, M85XX_BR0(CCSBAR), 0xff001001)
    WRITEADR(r6, r7, M85XX_OR0(CCSBAR), 0xff000030)

    isync
    sync
    mbar 0

    /* Initialise the Local Bus Clock Ratio Register */

    li      r4, 0x2000
    mtctr   r4
    WRITEADR(r6, r7, M85XX_LCRR(CCSBAR), 0x80000008)

    isync
    sync
    mbar 0

dllDelay4:
    nop
    bdnz    dllDelay4

    /* VSC7385 interface initialize */

    WRITEADR(r6, r7, M85XX_LAWBAR5(CCSBAR), 0x000f1000)
    WRITEADR(r6, r7, M85XX_LAWAR5(CCSBAR), 0x80400010)

    WRITEADR(r6, r7, M85XX_BR2(CCSBAR), 0xf1000801)
    WRITEADR(r6, r7, M85XX_OR2(CCSBAR), 0xfffe09ff)

    isync
    sync
    mbar 0

    /* NAND Flash interface initialize */

    WRITEADR(r6, r7, M85XX_LAWBAR6(CCSBAR), 0x000f0000)
    WRITEADR(r6, r7, M85XX_LAWAR6(CCSBAR), 0x80400013)

    WRITEADR(r6, r7, M85XX_BR1(CCSBAR), 0xf0000c21)
    WRITEADR(r6, r7, M85XX_OR1(CCSBAR), 0xfff80396)

    isync
    sync
    mbar 0

    /* Memory mapped region base address */

    WRITEADR(r6, r7, M85XX_LAWBAR11(CCSBAR), 0x00000000)
    WRITEADR(r6, r7, M85XX_LAWAR11(CCSBAR), 0x80f0001d)

    isync

    lis     r7, HI(M85XX_PORPLLSR(CCSBAR))
    ori     r7, r7, LO(M85XX_PORPLLSR(CCSBAR))
    lwz     r7, 0(r7)
    andi.   r7, r7, 0x3e
    srwi    r7, r7, 1

    /* Get multiplier from table */

    lis     r8, HI(ROM_TEXT_ADRS)
    ori     r8, r8, LO(platpllTable)
    add     r8, r8, r7
    lbz     r8, 0(r8)

    cmpwi   r8,0                /* Test for unsupported freq */
    beq     checkStop           /* Jump to 0 */

    /* DDR Controller Initialization */

    lis r6, HI(DDRBA)
    ori r6, r6, LO(DDRBA)       /* r6 = DDR base */

#ifdef INCLUDE_DDR_ECC
    WRITEOFFSET(r6, r7, (DDR_SDRAM_CFG),        0x63000000)
#else  /* INCLUDE_DDR_ECC */
    WRITEOFFSET(r6, r7, (DDR_SDRAM_CFG),        0x43000000)
#endif  /* INCLUDE_DDR_ECC */

    WRITEOFFSET(r6, r7, (CS0_BNDS),             0x0000003F)
    WRITEOFFSET(r6, r7, (CS0_CONFIG),           0x80014202)
    WRITEOFFSET(r6, r7, (CS0_CONFIG_2),         0x00000000)
    WRITEOFFSET(r6, r7, (TIMING_CFG_3),         0x00030000)
    WRITEOFFSET(r6, r7, (TIMING_CFG_0),         0x55770802)
    WRITEOFFSET(r6, r7, (TIMING_CFG_1),         0x5f599543)
    WRITEOFFSET(r6, r7, (TIMING_CFG_2),         0x0fa074d1)
    WRITEOFFSET(r6, r7, (TIMING_CFG_4),         0x00000000)
    WRITEOFFSET(r6, r7, (TIMING_CFG_5),         0x00000000)
    WRITEOFFSET(r6, r7, (DDR_SDRAM_CFG_2),      0x24401000)
    WRITEOFFSET(r6, r7, (DDR_SDRAM_MODE_CFG),   0x00040852)
    WRITEOFFSET(r6, r7, (DDR_SDRAM_MODE_CFG_2), 0x00000000)
    WRITEOFFSET(r6, r7, (DDR_SDRAM_MD_CNTL),    0x00000000)
    WRITEOFFSET(r6, r7, (DDR_SDRAM_INTERVAL),   0x0a280100)
    WRITEOFFSET(r6, r7, (DDR_DATA_INIT),        0xdeadbeef)
    WRITEOFFSET(r6, r7, (DDR_SDRAM_CLK_CTRL),   0x03000000)
    WRITEOFFSET(r6, r7, (DDR_DDR_ZQ_CNTL),      0x00000000)
    WRITEOFFSET(r6, r7, (DDR_DDR_WRLVL_CNTL),   0x00000000)
    WRITEOFFSET(r6, r7, (ERR_INT_EN),           0x00000000)
    WRITEOFFSET(r6, r7, (ERR_SBE),              0x00000000)
    WRITEOFFSET(r6, r7, (DDR_DDRCDR_1),         0x00000000)
    WRITEOFFSET(r6, r7, (DDR_DDRCDR_2),         0x00000000)

    /* wait before enable */

    addi    r5, 0, 0x0fff
    mtspr   CTR,r5
ddrLoop_1:
    bc      16, 0, ddrLoop_1

#ifdef INCLUDE_DDR_ECC
    WRITEOFFSET(r6, r7, (DDR_SDRAM_CFG),        0xe3000000)
#else  /* INCLUDE_DDR_ECC */
    WRITEOFFSET(r6, r7, (DDR_SDRAM_CFG),        0xc3000000)
#endif  /* INCLUDE_DDR_ECC */

    /* wait for DRAM data initialization */

    addi    r5, 0, 0x0fff
    mtspr   CTR,r5
ddrLoop_2:
    bc      16, 0, ddrLoop_2

    /* Memory mapped region base address */

    isync

    lis     r7, HI(M85XX_PORPLLSR(CCSBAR))
    ori     r7, r7, LO(M85XX_PORPLLSR(CCSBAR))
    lwz     r7, 0(r7)
    andi.   r7, r7, 0x3e
    srwi    r7, r7, 1

    /* Get multiplier from table */

    lis     r8, HI(ROM_TEXT_ADRS)
    ori     r8, r8, LO(platpllTable)
    add     r8, r8, r7
    lbz     r8, 0(r8)

    cmpwi   r8, 0 /* Test for unsupported freq */

    beq     checkStop  /* Jump to 0 */

    /* Now that memory is stable we reset TLB entries for standard operation */

    /* TLB1 #0: NorFlash space 0xf0000000 -> 0xffffffff (16MB) */

    addis   r4, 0, 0x1000               /* TLB1 entry#0 */
    ori     r4, r4, 0x0000
    mtspr   MAS0, r4
    addis   r5, 0, HI(MMU_STATE_VALID|MMU_STATE_IPROT)
    ori     r5, r5, _MMU_TLB_SZ_16M     /* TS = 0, TSIZE = 16 MByte */
    mtspr   MAS1, r5
    addis   r6, 0, HI(FLASH_BASE_ADRS)  /* EPN */
    ori     r6, r6, MMU_ROM_ACCESS
    mtspr   MAS2, r6
    addis   r7, 0, HI(FLASH_BASE_ADRS)  /* RPN */
    ori     r7, r7, 0x0015              /* Supervisor XWR */
    mtspr   MAS3, r7
    tlbwe
    tlbsync

    /*
     * TLB1 #1.  Main SDRAM - Not Cached
     *           LOCAL_MEM_LOCAL_ADRS -> LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE
     * Attributes: UX/UW/UR/SX/SW/SR
     *
     */

    addis   r4, 0, 0x1001               /* TLB1 entry#1 */
    ori     r4, r4, 0x0000
    mtspr   MAS0, r4
    addis   r5, 0, HI(MMU_STATE_VALID|MMU_STATE_IPROT)
    ori     r5, r5, _MMU_TLB_SZ_1G      /* TS = 0, TSIZE = 1 GByte */
    mtspr   MAS1, r5
    addis   r6, 0, HI(LOCAL_MEM_LOCAL_ADRS) /* EPN */
    ori     r6, r6, 0x0000              /* WIMGE = 00000 */
    mtspr   MAS2, r6
    addis   r7, 0, HI(LOCAL_MEM_LOCAL_ADRS) /* RPN */
    ori     r7, r7, 0x003f              /* Supervisor XWR*/
    mtspr   MAS3, r7
    tlbwe
    tlbsync

    /* TLB1 #2: CCSR space 0xE0000000 -> 0xE00FFFFF (1MB) */

    addis   r4, 0,  0x1002              /* TLBSEL = TLB1(CAM) , ESEL = 3*/
    ori     r4, r4, 0x0000
    mtspr   MAS0,   r4
    addis   r5, 0,  HI(MMU_STATE_VALID|MMU_STATE_IPROT)
    ori     r5, r5, _MMU_TLB_SZ_1M      /* TS = 0, TSIZE = 1 MByte page size*/
    mtspr   MAS1,   r5
    addis   r6, 0,  HI(CCSBAR)          /* EPN = CCSBAR */
    ori     r6, r6, 0x000a              /* WIMGE = 01010 */
    mtspr   MAS2,   r6
    addis   r7, 0,  HI(CCSBAR)          /* RPN = CCSBAR */
    ori     r7, r7, 0x003f              /* Supervisor XWR*/
    mtspr   MAS3,   r7
    isync
    msync
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
    b   resetEntry
FUNC_END(resetVector)
