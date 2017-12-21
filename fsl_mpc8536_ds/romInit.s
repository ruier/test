/* romInit.s - ROM bootcode for Freescale MPC8536DS BSP */

/*
 * Copyright (c) 2008, 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,02apr11,y_c  Support VxWorks native bootrom. (WIND00240155)
01a,20oct08,b_m  Derived from version 01c of fsl_mds8568/romInit.s.
*/

#define _ASMLANGUAGE

#include <vxWorks.h>
#include <sysLib.h>
#include <asm.h>
#include <regs.h>
#include <arch/ppc/mmuE500Lib.h>
#include "config.h"

FUNC_EXPORT(romInit)
FUNC_EXPORT(_romInit)

#ifdef BSP_BOOT_CACHE_SYNC
FUNC_EXPORT(sysBspBootSync)
#endif /* BSP_BOOT_CACHE_SYNC */

FUNC_IMPORT(romStart)

#define WRITEADR(reg1, reg2, addr32, val)       \
    lis reg1, HI(addr32);                       \
    ori reg1, reg1, LO(addr32);                 \
    lis reg2, HI(val);                          \
    ori reg2, reg2, LO(val);                    \
    stw reg2, 0(reg1)

#define WRITEOFFSET(reg1, reg2, offset, val)    \
    lis reg2, HI(val);                          \
    ori reg2, reg2, LO(val);                    \
    stw reg2, offset(reg1);

    _WRS_TEXT_SEG_START

    .fill   0x100, 1, 0

FUNC_BEGIN(romInit)
FUNC_BEGIN(_romInit)

    b       resetEntry
    b       warm

    .ascii  "Copyright (c) 2008-2011 Wind River Systems, Inc."
    .balign 4

cold:
    li      r2, BOOT_CLEAR
    b       start

warm:
    mr      r2, r3

start:
    xor     r0, r0, r0
    addi    r1, r0, -1

    mtspr   DEC, r0
    mtspr   TBL, r0
    mtspr   TBU, r0
    mtspr   TCR, r0
    mtspr   TSR, r1
    mtspr   ESR, r0
    mtspr   XER, r0

    mfmsr   r3                      /* r3 = msr              */
    INT_MASK (r3, r4)               /* mask EE and CE bit    */
    
    /* turn off _PPC_MSR_ME  */

    rlwinm  r4, r4, 0, _PPC_MSR_BIT_ME + 1, _PPC_MSR_BIT_ME - 1
    mtmsr   r4                      /* msr = r4              */
    isync

    xor     r6, r6, r6
    msync
    isync
    mtspr   L1CSR0, r6      /* disable the Data cache */
    li  r6, 0x0002
    isync

    mtspr   L1CSR0, r6      /* invalidate the Data cache */
    li  r6, 0x0000
    isync

    mtspr   L1CSR1, r6      /* disable the Instrunction cache */
    li  r6, 0x0002
    isync

    mtspr   L1CSR1, r6      /* invalidate the Instruction cache */
    isync
    li  r6, 0x0000
    isync

    mtspr   L1CSR1, r6      /* disabled the Instruction cache temporarily */
    isync
    li  r7, 0x0001
    isync
    mtspr   L1CSR1, r7      /* enable the instruction cache */
    isync

    lis     sp, HI(STACK_ADRS)
    ori     sp, sp, LO(STACK_ADRS)
    addi    sp, sp, -FRAMEBASESZ
    lis     r6, HI(romInit)
    ori     r6, r6, LO(romInit)
    lis     r7, HI(romStart)
    ori     r7, r7, LO(romStart)
    lis     r8, HI(ROM_TEXT_ADRS)
    ori     r8, r8, LO(ROM_TEXT_ADRS)
    sub     r6, r7, r6
    add     r6, r6, r8
    mtspr   LR, r6
    mr      r3, r2
    blr

FUNC_END(_romInit)
FUNC_END(romInit)

#ifdef BSP_BOOT_CACHE_SYNC
FUNC_BEGIN(sysBspBootSync)

    li      r6, 0x0000
    mtspr   L1CSR1, r6    /* Disable the Instrunction cache */
    msync
    isync

    li      r6, 0x0002
    mtspr   L1CSR1, r6    /* Invalidate the Instruction cache */
    msync
    isync

    li      r6, 0x0001
    mtspr   L1CSR1, r6    /* Enable the Instruction cache */
    isync

    blr

FUNC_END(sysBspBootSync)
#endif /* BSP_BOOT_CACHE_SYNC */

/*******************************************************************************
*
* resetEntry - rom entry point
*
* resetEntry is the first function to be called by the Book E core after
* reset.  It is located at ROM address 0xfffff000. It is responsible for
* initializing key registers to a quiescent state, initializing the the
* TLB with a static memory mapping adequate for running the bootrom, and
* clearing the rest of the TLB.  It sets up device access registers so
* the rest of the SRAM (or Boot Flash) can be correctly accessed.
*
* NOMANUAL
*
*/

#if defined(_GNU_TOOL)
    .section .boot, "ax", @progbits
#elif defined(_DIAB_TOOL)
    .section .boot, 4, "rx"
#else
#error "Please add a correctly spelled .section directive for your toolchain."
#endif

FUNC_BEGIN(resetEntry)
FUNC_LABEL(_resetEntry)

	xor     r4, r4, r4      /* clear the MSR */
	mtmsr   r4

    xor     r6, r6, r6
    msync
    isync
    mtspr   L1CSR0, r6      /* disable the Data cache */
    li  r6, 0x0002
    isync
    
    mtspr   L1CSR0, r6      /* invalidate the Data cache */
    li  r6, 0x0000
    isync
    
    mtspr   L1CSR1, r6      /* disable the Instrunction cache */
    li  r6, 0x0002
    isync
    
    mtspr   L1CSR1, r6      /* invalidate the Instruction cache */
    isync
    li  r6, 0x0000
    isync
    
    mtspr   L1CSR1, r6      /* disabled the Instruction cache temporarily */
    isync
    li  r7, 0x0000
    isync
    mtspr   L1CSR1, r7      /* enable the instruction cache */
    isync

    /* clear core registers */

    xor     r0, r0, r0

    mtspr   SRR0, r0
    mtspr   SRR1, r0
    mtspr   CSRR0, r0
    mtspr   CSRR1, r0
    mtspr   MCSRR0, r0
    mtspr   MCSRR1, r0
    mtspr   ESR, r0
    mtspr   MCSR, r0
    mtspr   MCAR, r0
    mtspr   DEAR, r0

    mtspr   DBCR0, r0
    isync
    mtspr   DBCR1, r0
    isync
    mtspr   DBCR2, r0
    isync
    mtspr   IAC1, r0
    mtspr   IAC2, r0
    mtspr   DAC1, r0
    mtspr   DAC2, r0
    mfspr   r1, DBSR
    mtspr   DBSR, r1

    mtspr   BUCSR, r0
    isync

    /*
     * write TLB entry for initial program memory page
     *
     * - specify EPN, RPN, and TSIZE as appropriate for system
     * - set valid bit
     * - specify TID = 0
     * - specify TS = 0
     * - specify storage attributes (W, I, M, G, E, U0 - U3) as appropriate
     * - enable supervisor mode fetch, read, and write access (SX, SR, SW)
     */

    /* TLB#0: CCSBAR_RESET 0xFF700000 -> 0xFF800000 (1MB) */

    addis   r4, 0, 0x1001                       /* TLB1 entry#1 */
    ori     r4, r4, 0x0000
    mtspr   MAS0, r4
    addis   r5, 0, 0xc000                       /* V = 1, IPROT = 1, TID = 0 */
    ori     r5, r5, _MMU_TLB_SZ_1M              /* TS = 0, TSIZE = 1 MByte */
    mtspr   MAS1, r5
    addis   r6, 0, HI(CCSBAR_RESET)                /* EPN */
    ori     r6, r6, 0x000a                      /* WIMGE = 01010 */
    mtspr   MAS2, r6
    addis   r7, 0, HI(CCSBAR_RESET)                /* RPN */
    ori     r7, r7, 0x0015                      /* Supervisor XWR */
    mtspr   MAS3, r7
    tlbwe
    tlbsync

    /* TLB#1: CCSBAR 0xE0000000 -> 0xE0100000 (1MB) */

    addis   r4, 0, 0x1002                       /* TLB1 entry#2 */
    ori     r4, r4, 0x0000
    mtspr   MAS0, r4
    addis   r5, 0, 0xc000                       /* V = 1, IPROT = 1, TID = 0 */
    ori     r5, r5, _MMU_TLB_SZ_1M              /* TS = 0, TSIZE = 1 MByte */
    mtspr   MAS1, r5
    addis   r6, 0, HI(CCSR_BASE)                /* EPN */
    ori     r6, r6, 0x000a                      /* WIMGE = 01010 */
    mtspr   MAS2, r6
    addis   r7, 0, HI(CCSR_BASE)                /* RPN */
    ori     r7, r7, 0x0015                      /* Supervisor XWR */
    mtspr   MAS3, r7
    tlbwe
    tlbsync

    isync
    msync
    lis     r6, HI(CCSBAR)
    ori     r6, r6, LO(CCSBAR)
    isync

    srwi    r5, r6, 12
    lis     r7, HI(CCSBAR_RESET)
    ori     r7, r7, LO(CCSBAR_RESET)

    isync
    msync

    stw     r5, 0(r7)

    isync
    mbar    0

    lis     r5, 0xffff
    ori     r5, r5, 0xf800
    lwz     r4, 0(r5)
    isync

    /* wait for ccsrbar */
    li      r4, 0x2000
    mtctr   r4
ccsrbarWait:
    nop
    bdnz    ccsrbarWait

    /* initialise the Local Bus Controller */

    WRITEADR(r6, r7, M85XX_LCRR(CCSBAR), 0x00000002)

    isync
    msync
    mbar    0

    /* setup necessary LAWs */

    WRITEADR(r6, r7, M85XX_LAWBAR0(CCSBAR), 0x00000000)
    WRITEADR(r6, r7, M85XX_LAWAR0(CCSBAR), LAWAR_ENABLE | LAWAR_TGTIF_DDRSDRAM | LAWAR_SIZE_512MB)

    WRITEADR(r6, r7, M85XX_LAWBAR1(CCSBAR), 0x000f0000)
    WRITEADR(r6, r7, M85XX_LAWAR1(CCSBAR), LAWAR_ENABLE | LAWAR_TGTIF_LBC | LAWAR_SIZE_256MB)

    WRITEADR(r6, r7, M85XX_LAWBAR2(CCSBAR), 0x000efdf0)
    WRITEADR(r6, r7, M85XX_LAWAR2(CCSBAR), LAWAR_ENABLE | LAWAR_TGTIF_LBC | LAWAR_SIZE_32KB)
    isync

    /* setup LBC CSs */

    WRITEADR(r6, r7, M85XX_BR0(CCSBAR), 0xf8001001)
    WRITEADR(r6, r7, M85XX_OR0(CCSBAR), 0xf8000ff7)

    WRITEADR(r6, r7, M85XX_BR1(CCSBAR), 0xf0001001)
    WRITEADR(r6, r7, M85XX_OR1(CCSBAR), 0xf8000ff7)

    WRITEADR(r6, r7, M85XX_BR3(CCSBAR), 0xefdf0801)
    WRITEADR(r6, r7, M85XX_OR3(CCSBAR), 0xffff8ff7)

    isync
    mbar    0

    /* initialize the DDR Memory controller */

    lis     r6, HI(DDRBA)
    ori     r6, r6, LO(DDRBA)       /* r6 = DDR base */

    WRITEOFFSET(r6, r7, (DDR_SDRAM_CFG), 0x43000000)

    WRITEOFFSET(r6, r7, (CS0_BNDS), 0x0000001F)
    WRITEOFFSET(r6, r7, (CS0_CONFIG), 0x80010202)

    WRITEOFFSET(r6, r7, (EXTENDED_REF_REC), 0x00010000)
    WRITEOFFSET(r6, r7, (TIMING_CFG_0), 0x00260802)
    WRITEOFFSET(r6, r7, (TIMING_CFG_1), 0x4C473422)
    WRITEOFFSET(r6, r7, (TIMING_CFG_2), 0x05184CCA)

    WRITEOFFSET(r6, r7, (DDR_DATA_INIT), 0x00000000)

    WRITEOFFSET(r6, r7, (DDR_SDRAM_MODE_CFG), 0x00440642)
    WRITEOFFSET(r6, r7, (DDR_SDRAM_MODE_CFG_2), 0x00000000)
    WRITEOFFSET(r6, r7, (DDR_SDRAM_MD_CNTL), 0x00000000)
    WRITEOFFSET(r6, r7, (DDR_SDRAM_INTERVAL), 0x079E0100)
    WRITEOFFSET(r6, r7, (DDR_SDRAM_CLK_CTRL), 0x03800000)
    WRITEOFFSET(r6, r7, (DDR_SDRAM_CFG_2), 0x24401000)

    isync

    li      r4, 0x2000
    mtctr   r4
ddrDelay:
    nop
    bdnz    ddrDelay

    WRITEOFFSET(r6, r7, (DDR_SDRAM_CFG), 0xC3000000)

    isync

    /* now that memory is stable we reset TLB entries for standard operation */

    /* TLB#0: Flash 0xF0000000 -> 0xFFFFFFFF (256M but only use 128MB) */

    addis   r4, 0, 0x1000                       /* TLB1 entry#0 */
    ori     r4, r4, 0x0000
    mtspr   MAS0, r4
    addis   r5, 0, 0xc000                       /* V = 1, IPROT = 1, TID = 0 */
    ori     r5, r5, _MMU_TLB_SZ_256M            /* TS = 0, TSIZE = 256 MByte */
    mtspr   MAS1, r5
    addis   r6, 0, HI(0xF0000000)               /* EPN */
    ori     r6, r6, 0x0016                      /* WIMGE = 10110 */
    mtspr   MAS2, r6
    addis   r7, 0, HI(0xF0000000)               /* RPN */
    ori     r7, r7, 0x0015                      /* Supervisor XWR */
    mtspr   MAS3, r7
    tlbwe
    tlbsync

    /* TLB#1: DDR part1 0x00000000 -> 0x0FFFFFFF (256MB) */

    addis   r4, 0, 0x1001                       /* TLB1 entry#1 */
    ori     r4, r4, 0x0000
    mtspr   MAS0, r4
    addis   r5, 0, 0xc000                       /* V = 1, IPROT = 1, TID = 0 */
    ori     r5, r5, _MMU_TLB_SZ_256M            /* TS = 0, TSIZE = 256 MByte */
    mtspr   MAS1, r5
    addis   r6, 0, HI(LOCAL_MEM_LOCAL_ADRS)     /* EPN = LOCAL_MEM_LOCAL_ADRS */
    ori     r6, r6, 0x0004                      /* WIMGE = 00100 */
    mtspr   MAS2, r6
    addis   r7, 0, HI(LOCAL_MEM_LOCAL_ADRS)     /* RPN = LOCAL_MEM_LOCAL_ADRS */
    ori     r7, r7, 0x003f                      /* User/Supervisor XWR*/
    mtspr   MAS3, r7
    tlbwe
    tlbsync

    /* TLB#2: DDR part2 0x10000000 -> 0x1FFFFFFF (256MB) */

    addis   r4, 0, 0x1002                       /* TLB1 entry#2 */
    ori     r4, r4, 0x0000
    mtspr   MAS0, r4
    addis   r5, 0, 0xc000                       /* V = 1, IPROT = 1, TID = 0 */
    ori     r5, r5, _MMU_TLB_SZ_256M            /* TS = 0, TSIZE = 256 MByte */
    mtspr   MAS1, r5
    addis   r6, 0, HI(0x10000000)               /* EPN = 0x10000000 */
    ori     r6, r6, 0x0004                      /* WIMGE = 00100 */
    mtspr   MAS2, r6
    addis   r7, 0, HI(0x10000000)               /* RPN = 0x10000000 */
    ori     r7, r7, 0x003f                      /* User/Supervisor XWR*/
    mtspr   MAS3, r7
    tlbwe
    tlbsync

    /* TLB#3: CCSR 0xE0000000 -> 0xE0100000 (1MB) */

    addis   r4, 0, 0x1003                       /* TLB1 entry#3 */
    ori     r4, r4, 0x0000
    mtspr   MAS0, r4
    addis   r5, 0, 0xc000                       /* V = 1, IPROT = 1, TID = 0 */
    ori     r5, r5, _MMU_TLB_SZ_1M              /* TS = 0, TSIZE = 1 MByte */
    mtspr   MAS1, r5
    addis   r6, 0, HI(CCSR_BASE)                /* EPN */
    ori     r6, r6, 0x000a                      /* WIMGE = 01010 */
    mtspr   MAS2, r6
    addis   r7, 0, HI(CCSR_BASE)                /* RPN */
    ori     r7, r7, 0x0015                      /* Supervisor XWR */
    mtspr   MAS3, r7
    tlbwe
    tlbsync

    /* TLB#4: PIXIS 0xEFDF0000 -> 0xEFDF0FFF (4KB) */

    addis   r4, 0, 0x1004                       /* TLB1 entry#4 */
    ori     r4, r4, 0x0000
    mtspr   MAS0, r4
    addis   r5, 0, 0xc000                       /* V = 1, IPROT = 1, TID = 0 */
    ori     r5, r5, _MMU_TLB_SZ_4K              /* TS = 0, TSIZE = 4 KByte */
    mtspr   MAS1, r5
    addis   r6, 0, HI(PIXIS_BASE)               /* EPN */
    ori     r6, r6, 0x000a                      /* WIMGE = 01010 */
    mtspr   MAS2, r6
    addis   r7, 0, HI(PIXIS_BASE)               /* RPN */
    ori     r7, r7, 0x003f                      /* User/Supervisor XWR */
    mtspr   MAS3, r7
    tlbwe
    tlbsync

    /* jump to rom cold entry */

    b       cold

checkStop:
    ba      0

FUNC_END(resetEntry)

/*******************************************************************************
*
* resetVector - hardware reset vector
*
* At power-on, the processor starts executing at ROM address 0xfffffffc -- the
* top of the address space -- which must be a jump to resetEntry.  This is
* defined as a separate "section" to assist the linker in locating it properly.
*
* NOMANUAL
*
*/

#if defined(_GNU_TOOL)
    .section .reset, "ax", @progbits
#elif defined(_DIAB_TOOL)
    .section .reset, 4, "rx"
#else
#error "Please add a correctly spelled .section directive for your toolchain."
#endif

FUNC_BEGIN(resetVector)
    b       resetEntry
FUNC_END(resetVector)
