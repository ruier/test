/* romInit.s - ROM bootcode for fsl_p1010_rdb BSP */

/*
 * Copyright (c) 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
01b,29dec11,mpc  Initialized MMUCSR0 with value 0x06. (WIND00319727)
01a,22oct11,fao  Created from fsl_p2020_rdb/01b.
*/

#define _ASMLANGUAGE
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
 * MSR register initialization
 *
 * This initial MSR value allows machine check and critical class
 * exceptions.
 */

#define MSR_VALUE       (_PPC_MSR_CE|_PPC_MSR_ME)

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

#ifdef BSP_BOOT_CACHE_SYNC
FUNC_EXPORT(sysBspCacheSync)
FUNC_EXPORT(_sysBspCacheSync)
#endif /* BSP_BOOT_CACHE_SYNC */

    _WRS_TEXT_SEG_START

FUNC_BEGIN(romInit)
FUNC_BEGIN(_romInit)
    bl     resetEntry

romWarm:
    bl     warm

    /* Useful to have recognizeable string */

    .ascii   "Wind River vxWorks bootrom"

    .balign 4

cold:
    li  r2, BOOT_COLD
    b   start

warm:
    mr  r2, r3

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

/*
 * Add this function for Instruction Cache is set Enable.
 * If I-Cache is enable, we must Invalidate I-cache before
 * unzip bootrom
 */

#ifdef BSP_BOOT_CACHE_SYNC
FUNC_BEGIN(sysBspCacheSync)
FUNC_LABEL(_sysBspCacheSync)

    li    r6,    0x0000
    mtspr L1CSR1,    r6    /* Disable the Instruction cache */
    msync
    isync

    li    r6,    0x0002
    mtspr L1CSR1,    r6    /* Invalidate the Instruction cache */
    msync
    isync

    li    r6,    0x0001
    mtspr L1CSR1,    r6    /* Enable the Instruction cache */
    isync

    blr

FUNC_END(sysBspCacheSync)
#endif /* BSP_BOOT_CACHE_SYNC */

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

    mfspr   r1, DBSR  /* clear all bits */
    mtspr   DBSR, r1

    mtspr   PID0, r0
    mtspr   PID1, r0
    mtspr   PID2, r0
    mtspr   TCR, r0
    mtspr   1013, r0

    mtspr   MAS4, r0
    mtspr   MAS6, r0
    isync

    li      r2, 0x06		 /* L2TLB0_FI | L2TLB1_FI */
    mtspr   MMUCSR0, r2
    isync

    li      r3, 4
    li      r4, 0
    tlbivax r4, r3
    nop

    /* TLB1 #0. Flash space: 0xEE000000 -> 0xEEFFFFFF (16MB) */

    addis   r4, 0, 0x1001               /* TLB1 entry#0 */
    ori     r4, r4, 0x0000
    mtspr   MAS0, r4
    addis   r5, 0, 0xc000               /* V = 1, IPROT = 1, TID = 0 */
    ori     r5, r5, _MMU_TLB_SZ_16M     /* TS = 0, TSIZE = 16 MByte */
    mtspr   MAS1, r5
    addis   r6, 0, HI(FLASH_BASE_ADRS)  /* EPN */
    ori     r6, r6, 0x00a               /* WIMGE = 01010 */
    mtspr   MAS2, r6
    addis   r7, 0, HI(FLASH_BASE_ADRS)  /* RPN */
    ori     r7, r7, 0x003f              /* Supervisor XWR */
    mtspr   MAS3, r7
    tlbwe
    tlbsync

    /* TLB1 #0. Flash space: 0xEF000000 -> 0xEFFFFFFF (16MB) */

    addis   r4, 0, 0x1002               /* TLB1 entry#0 */
    ori     r4, r4, 0x0000
    mtspr   MAS0, r4
    addis   r5, 0, 0xc000               /* V = 1, IPROT = 1, TID = 0 */
    ori     r5, r5, _MMU_TLB_SZ_16M     /* TS = 0, TSIZE = 16 MByte */
    mtspr   MAS1, r5
    addis   r6, 0, HI(FLASH_BASE_ADRS + 0x1000000)  /* EPN */
    ori     r6, r6, 0x002               /* WIMGE = 00010 */
    mtspr   MAS2, r6
    addis   r7, 0, HI(FLASH_BASE_ADRS + 0x1000000)  /* RPN */
    ori     r7, r7, 0x003f              /* Supervisor XWR */
    mtspr   MAS3, r7
    tlbwe
    tlbsync

    /*
     * TLB1 #1. Memeroy space: 0x0000000 -> 0x3FFFFFFF (1G)
     * Attributes: UX/UW/UR/SX/SW/SR
     */

    addis   r4, 0, 0x1003               /* TLB1 entry#1 */
    ori     r4, r4, 0x0000
    mtspr   MAS0, r4
    addis   r5, 0, 0xc000               /* V = 1, IPROT = 1, TID = 0 */
    ori     r5, r5, _MMU_TLB_SZ_1G      /* TS = 0, TSIZE = 1G Byte */
    mtspr   MAS1, r5
    addis   r6, 0, HI(LOCAL_MEM_LOCAL_ADRS)               /* EPN */
    ori     r6, r6, 0x00000             /* WIMGE = 00000 */
    mtspr   MAS2, r6
    addis   r7, 0, HI(LOCAL_MEM_LOCAL_ADRS) /* RPN */
    ori     r7, r7, 0x003f              /* Supervisor XWR*/
    mtspr   MAS3, r7
    tlbwe
    tlbsync

    /* TLB1 #2: CCSR space 0xFFE00000 -> 0xFFEFFFFF (1MB) */

    addis   r4, 0,  0x1004              /* TLBSEL = TLB1(CAM) , ESEL = 3*/
    ori     r4, r4, 0x0000
    mtspr   MAS0,   r4
    addis   r5, 0,  0xC000              /* V = 1, IPROT = 0, TID = 0*/
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

    /* TLB1 #2: CCSR space 0xFF700000 -> 0xFF7FFFFF (1MB) */

    addis   r4, 0,  0x1005              /* TLBSEL = TLB1(CAM) , ESEL = 3*/
    ori     r4, r4, 0x0000
    mtspr   MAS0,   r4
    addis   r5, 0,  0xC000              /* V = 1, IPROT = 0, TID = 0*/
    ori     r5, r5, _MMU_TLB_SZ_1M      /* TS = 0, TSIZE = 1 MByte page size*/
    mtspr   MAS1,   r5
    addis   r6, 0,  HI(CCSBAR_RESET)    /* EPN = CCSBAR */
    ori     r6, r6, 0x000a              /* WIMGE = 01010 */
    mtspr   MAS2,   r6
    addis   r7, 0,  HI(CCSBAR_RESET)    /* RPN = CCSBAR */
    ori     r7, r7, 0x003f              /* Supervisor XWR*/
    mtspr   MAS3,   r7
    isync
    msync
    tlbwe
    tlbsync

    /* Setup the memory mapped register address */
 
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

    lis     r5, HI(0xfffffffc)
    ori     r5, r5,LO(0xfffffffc)
    lwz     r4, 0(r5)
    isync

    li      r4, 0x4000
    mtctr   r4
ccsrbarWait:
    nop
    bdnz    ccsrbarWait

    /* read new CCSRBAR value */ 
 
    lis     r6, HI(CCSBAR)
    ori     r6, r6, LO(CCSBAR)
    lwz     r4, 0(r6) 
    isync

    /* IFC init */

    lis r6, HI(IFC)
    ori r6, r6, LO(IFC)       /* r6 = DDR base */

    /* NOR */

    WRITEOFFSET(r6, r7, (IFC_CSPR0),         0x00000105)
    WRITEOFFSET(r6, r7, (IFC_FTIM0_CS0),     0x40050005)
    WRITEOFFSET(r6, r7, (IFC_FTIM1_CS0),     0x1e000f00)
    WRITEOFFSET(r6, r7, (IFC_FTIM2_CS0),     0x0410001c)
    WRITEOFFSET(r6, r7, (IFC_FTIM3_CS0),     0x00000000)

    WRITEOFFSET(r6, r7, (IFC_CSPR2),         0x00000000)
    WRITEOFFSET(r6, r7, (IFC_AMASK2),        0x00000000)
    WRITEOFFSET(r6, r7, (IFC_CSOR2),         0x0000000C)
    WRITEOFFSET(r6, r7, (IFC_FTIM0_CS2),     0x00000000)
    WRITEOFFSET(r6, r7, (IFC_FTIM1_CS2),     0x00000000)
    WRITEOFFSET(r6, r7, (IFC_FTIM2_CS2),     0x00000000)
    WRITEOFFSET(r6, r7, (IFC_FTIM3_CS2),     0x00000000)

    WRITEOFFSET(r6, r7, (IFC_CSPR3),         0xFFB00085)
    WRITEOFFSET(r6, r7, (IFC_AMASK3),        0xFFFF0000)
    WRITEOFFSET(r6, r7, (IFC_CSOR3),         0x00000000)
    WRITEOFFSET(r6, r7, (IFC_FTIM0_CS3),     0xE00E000E)
    WRITEOFFSET(r6, r7, (IFC_FTIM1_CS3),     0x0E001F00)
    WRITEOFFSET(r6, r7, (IFC_FTIM2_CS3),     0x0E00001F)
    WRITEOFFSET(r6, r7, (IFC_FTIM3_CS3),     0x00000000)

    WRITEOFFSET(r6, r7, (IFC_GCR),           0x00000800)

    /* Local access window 0 (Nor Flash) initialize */

    WRITEADR(r6, r7, M85XX_LAWBAR0(CCSBAR), 0x000ee000)
    WRITEADR(r6, r7, M85XX_LAWAR0(CCSBAR),  0x80400018)

    isync

    /* Memory mapped region base address */

    WRITEADR(r6, r7, M85XX_LAWBAR1(CCSBAR), 0x00000000)
    WRITEADR(r6, r7, M85XX_LAWAR1(CCSBAR),  0x80f0001d)

    isync

    /* PCIE memory region base address */

    WRITEADR(r6, r7, M85XX_LAWBAR2(CCSBAR), 0x000a0000)
    WRITEADR(r6, r7, M85XX_LAWAR2(CCSBAR),  0x8010001c)

    isync

    /* CPLD memory region base address */

    WRITEADR(r6, r7, M85XX_LAWBAR3(CCSBAR), 0x000ffb00)
    WRITEADR(r6, r7, M85XX_LAWAR3(CCSBAR),  0x80400010)

    isync

    /* DDR Controller Initialization */

    lis r6, HI(DDRBA)
    ori r6, r6, LO(DDRBA)       /* r6 = DDR base */

    WRITEOFFSET(r6, r7, (DDR_SDRAM_CFG),        0x470c0000)

    WRITEOFFSET(r6, r7, (CS0_BNDS),             0x0000003F)
    WRITEOFFSET(r6, r7, (CS0_CONFIG),           0x80014302)
    WRITEOFFSET(r6, r7, (CS0_CONFIG_2),         0x00000000)
    WRITEOFFSET(r6, r7, (TIMING_CFG_3),         0x00010000)
    WRITEOFFSET(r6, r7, (TIMING_CFG_0),         0x00110004)
    WRITEOFFSET(r6, r7, (TIMING_CFG_1),         0x5d59e544)
    WRITEOFFSET(r6, r7, (TIMING_CFG_2),         0x0fa890cd)

    WRITEOFFSET(r6, r7, (DDR_SDRAM_CFG_2),      0x04401010)
    WRITEOFFSET(r6, r7, (DDR_SDRAM_MODE_CFG),   0x00441210)
    WRITEOFFSET(r6, r7, (DDR_SDRAM_MODE_CFG_2), 0x00000000)

    WRITEOFFSET(r6, r7, (DDR_SDRAM_MD_CNTL),    0x00000000)
    WRITEOFFSET(r6, r7, (DDR_SDRAM_INTERVAL),   0x0a280000)
    WRITEOFFSET(r6, r7, (DDR_DATA_INIT),        0xdeadbeef)
    WRITEOFFSET(r6, r7, (DDR_SDRAM_CLK_CTRL),   0x03000000)

    WRITEOFFSET(r6, r7, (TIMING_CFG_4),         0x00000001)
    WRITEOFFSET(r6, r7, (TIMING_CFG_5),         0x03402400)
    WRITEOFFSET(r6, r7, (DDR_DDR_ZQ_CNTL),      0x89080600)
    WRITEOFFSET(r6, r7, (DDR_DDR_WRLVL_CNTL),   0x8675f608)
    WRITEOFFSET(r6, r7, (ERR_INT_EN),           0x00000000)
    WRITEOFFSET(r6, r7, (ERR_SBE),              0x00000000)
    WRITEOFFSET(r6, r7, (DDR_DDRCDR_1),         0x00000000)
    WRITEOFFSET(r6, r7, (DDR_DDRCDR_2),         0x00000000)

    sync
    isync

    /* wait before enable */

    addi    r5, 0, 0x0fff
    mtspr   CTR,r5
ddrLoop_1:
    bc      16, 0, ddrLoop_1

    WRITEOFFSET(r6, r7, (DDR_SDRAM_CFG),        0xc70c0000)

    /* wait for DRAM data initialization */

    addi    r5, 0, 0x0fff
    mtspr   CTR,r5
ddrLoop_2:
    bc      16, 0, ddrLoop_2

    /*
     * Setup for subsequent change in instruction fetch address
     */

    lis   r0, HI(MSR_VALUE)
    ori   r0, r0, LO(MSR_VALUE)
    mtspr SRR1, r0        /* Write the new MSR value into SRR1 */

    /* calculate C entry point: routine - entry point + ROM base */

    lis     r4, HIADJ(cold)         /* r4 = cold */
    addi    r4, r4, LO(cold)

    lis     r5, HIADJ(romInit)      /* r5 = romInit */
    addi    r5, r5, LO(romInit)

    lis     r6, HIADJ(ROM_TEXT_ADRS) /* r6 = ROM_TEXT_ADRS */
    addi    r6, r6, LO(ROM_TEXT_ADRS)

    subf    r4, r5, r4              /* r4 = r4 - r5 */
    add     r4, r4, r6              /* r4 = r4 + r6 */

    mtspr SRR0, r4      /* Write addr from which to cont exec into SRR0 */

    /*
     * Context synchronize to invalidate shadow TLB contents and
     * cause new TLB  contents to take effect. Jump to main boot
     * code.
     */

    rfi

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
