/* romInit.s - ROM bootcode for Freescale MDS8568 BSP */

/*
 * Copyright (c) 2007-2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01c,14aug08,b_m  add workaround for errata DDR11.
01b,16may07,b_m  add U-BOOT support.
01a,10may07,b_m  derived from version 01b of wrSbc8548/romInit.s.
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

    .ascii  "Copyright (c) 2007-2008 Wind River Systems, Inc."
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
    mtspr   TSR, r1
    mtspr   TCR, r0
    mtspr   ESR, r0
    mtspr   XER, r0

#if defined(INCLUDE_CACHE_SUPPORT) && defined(USER_I_CACHE_ENABLE)
    li      r6, 0x0002
    mtspr   L1CSR1, r6          /* invalidate the instruction cache */
    isync

    li      r6, 0x0001
    mtspr   L1CSR1, r6          /* enable the instruction cache */
    isync
#endif

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
    sub     r6, r7, r6
    add     r6, r6, r8
    mtspr   LR, r6
    mr      r3, r2
    blr

FUNC_END(_romInit)
FUNC_END(romInit)

/*******************************************************************************
*
* resetEntry - rom entry point
*
* resetEntry is the first function to be called by the Book E core after
* reset. It is located at ROM address 0xfffff000. It is responsible for
* initializing key registers to a quiescent state, initializing the the
* TLB with a static memory mapping adequate for running the bootrom, and
* clearing the rest of the TLB. It sets up device access registers so
* the rest of the SRAM (or Boot Flash) can be correctly accessed.
*
* NOMANUAL
*
*/

#ifndef BOOT_FROM_UBOOT
#if defined(_GNU_TOOL)
    .section .boot, "ax", @progbits
#elif defined(_DIAB_TOOL)
    .section .boot, 4, "rx"
#else
#error "Please add a correctly spelled .section directive for your toolchain."
#endif
#endif

FUNC_BEGIN(resetEntry)
FUNC_LABEL(_resetEntry)

    xor     r0, r0, r0
    mtmsr   r0
    isync

    msync
    isync
    mtspr   L1CSR0, r6      /* disable the data cache */
    li      r6, 0x0002

    msync
    isync
    mtspr   L1CSR0, r6      /* invalidate the data cache */
    li      r6, 0x0000

    msync
    isync
    mtspr   L1CSR1, r6      /* disable the instruction cache */
    li      r6, 0x0002

    msync
    isync
    mtspr   L1CSR1, r6      /* invalidate the instruction cache */
    isync

    li      r6, 0x0000
    msync
    isync
    mtspr   L1CSR1, r6      /* temp disable the instruction cache */
    isync

    li      r7, 0x0000
    msync
    isync
    mtspr   L1CSR1, r7      /* enable the instruction cache */
    msync
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

    b       postTable

platpllTable:
    .long   0x00000203
    .long   0x04050600
    .long   0x08091000
    .long   0x12000000
    .long   0x16000000
    .long   0x20000000

postTable:

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

    /* TLB#0: 0xF0000000 -> 0xFFFFFFFF 256MB */

    addis   r4, 0, 0x1000           /* TLBSEL = TLB1(CAM), ESEL = 0 */
    ori     r4, r4, 0x0000
    mtspr   MAS0, r4
    addis   r5, 0, 0xc000           /* V = 1, IPROT = 1, TID = 0 */
    ori     r5, r5, _MMU_TLB_SZ_256M    /* TS = 0, TSIZE = 256 MByte page size */
    mtspr   MAS1, r5
    addis   r6, 0, 0xf000           /* EPN = 0xf0000000 */
    ori     r6, r6, 0x000a          /* WIMGE = 01010 */
    mtspr   MAS2, r6
    addis   r7, 0, 0xf000           /* RPN = 0xf0000000 */
    ori     r7, r7, 0x0015          /* Supervisor XWR */
    mtspr   MAS3, r7
    isync
    msync
    tlbwe
    tlbsync

    /* TLB#1: 0x00000000 -> 0x0FFFFFFF 256MB */

    addis   r4, 0, 0x1001           /* TLBSEL = TLB1(CAM), ESEL = 1 */
    ori     r4, r4, 0x0000
    mtspr   MAS0, r4
    addis   r5, 0, 0xc000           /* V = 1, IPROT = 1, TID = 0 */
    ori     r5, r5,_MMU_TLB_SZ_256M /* TS = 0, TSIZE = 256 MByte page size*/
    mtspr   MAS1, r5
    addis   r6, 0, HI(LOCAL_MEM_LOCAL_ADRS) /* EPN = LOCAL_MEM_LOCAL_ADRS */
    ori     r6, r6, 0x000a          /* WIMGE = 01010 */
    mtspr   MAS2, r6
    addis   r7, 0, HI(LOCAL_MEM_LOCAL_ADRS) /* RPN = LOCAL_MEM_LOCAL_ADRS */
    ori     r7, r7, 0x003f          /* User/Supervisor XWR */
    mtspr   MAS3, r7
    isync
    msync
    tlbwe
    tlbsync

    /* TLB#2: 0x10000000 -> 0x1FFFFFFF 256MB */

    addis   r4, 0, 0x1002           /* TLBSEL = TLB1(CAM), ESEL = 1 */
    ori     r4, r4, 0x0000
    mtspr   MAS0, r4
    addis   r5, 0, 0xc000           /* V = 1, IPROT = 1, TID = 0 */
    ori     r5, r5,_MMU_TLB_SZ_256M /* TS = 0, TSIZE = 256 MByte page size*/
    mtspr   MAS1, r5
    addis   r6, 0, HI(0x10000000)   /* EPN = LOCAL_MEM_LOCAL_ADRS */
    ori     r6, r6, 0x000a          /* WIMGE = 01010 */
    mtspr   MAS2, r6
    addis   r7, 0, HI(0x10000000)   /* RPN = LOCAL_MEM_LOCAL_ADRS */
    ori     r7, r7, 0x003f          /* User/Supervisor XWR */
    mtspr   MAS3, r7
    isync
    msync
    tlbwe
    tlbsync

    /* TLB#3: 0xE0000000 -> 0xE0100000 1MB */

    addis   r4, 0, 0x1003            /* TLBSEL = TLB1(CAM) , ESEL = 2*/
    ori     r4, r4, 0x0000
    mtspr   MAS0, r4
    addis   r5, 0, 0x8000            /* V = 1, IPROT = 0, TID = 0*/
    ori     r5, r5, _MMU_TLB_SZ_1M   /* TS = 0, TSIZE = 64 MByte page size*/
    mtspr   MAS1, r5
    addis   r6, 0, HI(CCSBAR)        /* EPN = CCSBAR */
    ori     r6, r6, 0x000a           /* WIMGE = 01010 */
    mtspr   MAS2, r6
    addis   r7, 0, HI(CCSBAR)        /* RPN = CCSBAR */
    ori     r7, r7, 0x0015           /* Supervisor XWR*/
    mtspr   MAS3, r7
    isync
    msync
    tlbwe
    tlbsync

#ifndef BOOT_FROM_UBOOT

    isync
    msync

    lis     r6, HI(CCSBAR)
    ori     r6, r6, LO(CCSBAR)
    isync

    srwi    r5, r6, 12
    lis     r7, HI(0xff700000)
    ori     r7, r7, LO(0xff700000)
    lwz     r4, 0(r7)

    isync
    msync
    mbar    0

    stw     r5, 0(r7)

    msync
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

    /* setup necessary LAWs */

    WRITEADR(r6, r7, M85XX_LAWBAR0(CCSBAR), 0x000f0000)
    WRITEADR(r6, r7, M85XX_LAWAR0(CCSBAR), LAWAR_ENABLE | LAWAR_TGTIF_LBC | LAWAR_SIZE_256MB)
    isync

    WRITEADR(r6, r7, M85XX_LAWBAR1(CCSBAR), 0x00000000)
    WRITEADR(r6, r7, M85XX_LAWAR1(CCSBAR), LAWAR_ENABLE | LAWAR_TGTIF_DDRSDRAM | LAWAR_SIZE_2GB)
    isync

    /* setup LBC CSs */

    WRITEADR(r6, r7, M85XX_BR0(CCSBAR), 0xfe001001)
    WRITEADR(r6, r7, M85XX_OR0(CCSBAR), 0xfe006ff7)
    isync

    WRITEADR(r6, r7, M85XX_BR1(CCSBAR), 0xf8000801)
    WRITEADR(r6, r7, M85XX_OR1(CCSBAR), 0xffffe9f7)
    isync

    isync
    msync
    mbar    0

    /* Initialise the Local Bus Controller */

    WRITEADR(r6, r7, M85XX_LCRR(CCSBAR), 0x00000002)

    isync
    msync
    mbar    0

    li      r4, 0x2000
    mtctr   r4

dllDelay4:
    nop
    bdnz    dllDelay4

    lis     r7, HI(M85XX_PORPLLSR(CCSBAR))
    ori     r7, r7, LO(M85XX_PORPLLSR(CCSBAR))
    lwz     r7, 0(r7)
    andi.   r7, r7, 0x3e
    srwi    r7, r7, 1

    /* Get multiplier from table */

    lis     r8, HI(0xffffffff)
    ori     r8, r8, LO(platpllTable)
    add     r8, r8, r7
    lbz     r8, 0(r8)

    cmpwi   r8, 0       /* Test for unsupported freq */
    beq     checkStop   /* Jump to 0 */

    /* workaround for errata DDR11 */

    WRITEADR(r6, r7, M85XX_DDRDLLCR(CCSBAR), 0x81000000)

    sync
    isync
    msync

    /* Initialize the DDR Memory controller */

    lis     r6, HI(DDRBA)
    ori     r6, r6, LO(DDRBA)       /* r6 = DDR base */

    WRITEOFFSET(r6, r7, (CS0_BNDS), 0x0000001F)
    WRITEOFFSET(r6, r7, (CS1_BNDS), 0x00000000)
    WRITEOFFSET(r6, r7, (CS2_BNDS), 0x00000000)
    WRITEOFFSET(r6, r7, (CS3_BNDS), 0x00000000)
    WRITEOFFSET(r6, r7, (CS0_CONFIG), 0x80010202)
    WRITEOFFSET(r6, r7, (CS1_CONFIG), 0x00000000)
    WRITEOFFSET(r6, r7, (CS2_CONFIG), 0x00000000)
    WRITEOFFSET(r6, r7, (CS3_CONFIG), 0x00000000)

    WRITEOFFSET(r6, r7, (EXTENDED_REF_REC), 0x00000000)
    WRITEOFFSET(r6, r7, (TIMING_CFG_0), 0x00260802)
    WRITEOFFSET(r6, r7, (TIMING_CFG_1), 0x3935D322)
    WRITEOFFSET(r6, r7, (TIMING_CFG_2), 0x14904CC8)

    WRITEOFFSET(r6, r7, (DDR_DATA_INIT), 0x00000000)

    WRITEOFFSET(r6, r7, (DDR_SDRAM_CFG), 0x63000000)
    WRITEOFFSET(r6, r7, (DDR_SDRAM_CFG_2), 0x04400010)

    WRITEOFFSET(r6, r7, (DDR_SDRAM_MODE_CFG), 0x00480432)
    WRITEOFFSET(r6, r7, (DDR_SDRAM_MODE_CFG_2), 0x00000000)
    WRITEOFFSET(r6, r7, (DDR_SDRAM_MD_CNTL), 0x00000000)
    WRITEOFFSET(r6, r7, (DDR_SDRAM_INTERVAL), 0x06090100)
    WRITEOFFSET(r6, r7, (DDR_SDRAM_CLK_CTRL), 0x03000000)

    isync

    li      r4, 0x2000
    mtctr   r4
ddrDelay:
    nop
    bdnz    ddrDelay

    WRITEOFFSET(r6, r7, (DDR_SDRAM_CFG), 0xC3000000)

    isync

#ifdef INCLUDE_LBC_SDRAM

    /* setup CS2 */

    WRITEADR(r6, r7, M85XX_BR2(CCSBAR), 0xf0001861)
    WRITEADR(r6, r7, M85XX_OR2(CCSBAR), 0xfc006901)
    isync

    WRITEADR(r6, r7, M85XX_LBCR(CCSBAR), 0x00000000)
    isync

SdramInit:

    /* Precharge All Banks */

    WRITEADR(r6, r7, M85XX_LSDMR(CCSBAR), M85XX_LSDMR_COMMON | M85XX_LSDMR_OP_PREALL)

    lis     r9, HIADJ(LBC_SDRAM_LOCAL_ADRS)
    addi    r9, r9, LO(LBC_SDRAM_LOCAL_ADRS)

    /* do a single write to an arbitrary location */

    addi    r5, 0, 0x00FF       /* Load 0x000000FF into r5 */
    stb     r5, 0(r9)           /* Write 0xFF to SDRAM address - bits [24-31] */
    mbar    0

    /* issue a "Auto Refresh" command to SDRAM */

    WRITEADR(r6, r7, M85XX_LSDMR(CCSBAR), M85XX_LSDMR_COMMON | M85XX_LSDMR_OP_AUTOREF)

    /* do a single write to an arbitrary location */

    addi    r5, 0, 0x00FF       /* Load 0x000000FF into r5 */
    stb     r5, 0(r9)           /* Write 0xFF to SDRAM address - bits [24-31] */
    mbar    0

    /* issue a "Auto Refresh" command to SDRAM */

    WRITEADR(r6, r7, M85XX_LSDMR(CCSBAR), M85XX_LSDMR_COMMON | M85XX_LSDMR_OP_AUTOREF)

    /* do a single write to an arbitrary location */

    addi    r8, 0, 0x00FF       /* Load 0x000000FF into r8 */
    stb     r8, 0(r9)           /* Write 0xFF to address R9  */
    stb     r8, 1(r9)           /* Write 0xFF to address R9  */
    stb     r8, 2(r9)           /* Write 0xFF to address R9  */
    stb     r8, 3(r9)           /* Write 0xFF to address R9  */
    stb     r8, 4(r9)           /* Write 0xFF to address R9  */
    stb     r8, 5(r9)           /* Write 0xFF to address R9  */
    stb     r8, 6(r9)           /* Write 0xFF to address R9  */
    stb     r8, 7(r9)           /* Write 0xFF to address R9  */

    /* issue a "Mode Register Write" command to SDRAM */

    WRITEADR(r6, r7, M85XX_LSDMR(CCSBAR), M85XX_LSDMR_COMMON | M85XX_LSDMR_OP_MRWRITE)

    /* do a single write to an arbitrary location */

    addi    r8, 0, 0x00FF       /* Load 0x000000FF into r8 */
    stb     r8, 0(r9)           /* Write 0xFF to address R9 - bits [24-31] */

    /* enable refresh services and put SDRAM into normal operation  */

    WRITEADR(r6, r7, M85XX_LSDMR(CCSBAR), M85XX_LSDMR_COMMON | M85XX_LSDMR_OP_NORMAL)

    /* program the MRTPR */

    addi    r5, 0, TPR          /* MRTPR[TPR] */
    lis     r6, HIADJ(M85XX_MRTPR(CCSBAR))
    addi    r6, r6, LO(M85XX_MRTPR(CCSBAR))
    sth     r5, 0(r6)           /* store upper half-word */

    /* program the LSRT */

    addi    r5, 0, LSRT_VALUE
    lis     r6, HIADJ(M85XX_LSRT(CCSBAR))
    addi    r6, r6, LO(M85XX_LSRT(CCSBAR))
    stb     r5, 0(r6)           /* store byte - bits[0-7] */

    mbar    0

    lis     r9, HI(LBC_SDRAM_LOCAL_ADRS)
    ori     r9, r9, LO(LBC_SDRAM_LOCAL_ADRS)

    lis     r7, HIADJ(0x100)    /* Loop 256 times */
    addi    r7, r7, LO(0x100)
    mtspr   9, r7               /* Load spr CTR with 8 */
    lis     r8, 0x5555          /* Load 0x000000FF into r8 */
    ori     r8, r8, 0x5555

SdramWrLoop2:
    stw     r8, 0(r9)           /* Write 0xFF to address r9 */
    addi    r9, r9, 4           /* Move r9 to next byte */
    addi    r8, r8, 1           /* Add 1 to r8 */
    bc      16, 0, SdramWrLoop2 /* Decrement CTR, and possibly branch */

#endif /* INCLUDE_LBC_SDRAM */

#endif  /* !BOOT_FROM_UBOOT */

    /* Now that memory is stable we reset TLB entries for standard operation */

    /* TLB#0: 0xF0000000 -> 0xFFFFFFFF 256MB */

    addis   r4, 0, 0x1000           /* TLBSEL = TLB1(CAM) , ESEL = 0 */
    ori     r4, r4, 0x0000
    mtspr   MAS0, r4
    addis   r5, 0, 0xc000       /* V = 1, IPROT = 1, TID = 0*/
    ori     r5, r5, _MMU_TLB_SZ_256M    /* TS = 0, TSIZE = 16 MByte page size*/
    mtspr   MAS1, r5
    addis   r6, 0, 0xf000           /* EPN = 0xff000000*/
    ori     r6, r6, 0x0016          /* WIMGE = 10110 */
    mtspr   MAS2, r6
    addis   r7, 0, 0xf000           /* RPN = 0xff000000*/
    ori     r7, r7, 0x0015          /* Supervisor XWR*/
    mtspr   MAS3, r7
    tlbwe
    tlbsync

    /* TLB#1: 0x00000000 -> 0x0FFFFFFF 256MB */

    addis   r4, 0, 0x1001            /* TLBSEL = TLB1(CAM) , ESEL = 1*/
    ori     r4, r4, 0x0000
    mtspr   MAS0, r4
    addis   r5, 0, 0xc000            /* V = 1, IPROT = 1, TID = 0*/
    ori     r5, r5, _MMU_TLB_SZ_256M /* TS = 0, TSIZE = 256 MByte page size*/
    mtspr   MAS1, r5
    addis   r6, 0, HI(LOCAL_MEM_LOCAL_ADRS)  /* EPN = LOCAL_MEM_LOCAL_ADRS */
    ori     r6, r6, 0x0004           /* WIMGE = 00000 */
    mtspr   MAS2, r6
    addis   r7, 0, HI(LOCAL_MEM_LOCAL_ADRS)  /* RPN = LOCAL_MEM_LOCAL_ADRS */
    ori     r7, r7, 0x003f           /* User/Supervisor XWR*/
    mtspr   MAS3, r7
    tlbwe
    tlbsync

    /* TLB#2: 0x10000000 -> 0x1FFFFFFF 256MB */

    addis   r4, 0, 0x1002            /* TLBSEL = TLB1(CAM) , ESEL = 1*/
    ori     r4, r4, 0x0000
    mtspr   MAS0, r4
    addis   r5, 0, 0xc000            /* V = 1, IPROT = 1, TID = 0*/
    ori     r5, r5, _MMU_TLB_SZ_256M /* TS = 0, TSIZE = 256 MByte page size*/
    mtspr   MAS1, r5
    addis   r6, 0, HI(0x10000000)    /* EPN = LOCAL_MEM_LOCAL_ADRS */
    ori     r6, r6, 0x0004           /* WIMGE = 00000 */
    mtspr   MAS2, r6
    addis   r7, 0, HI(0x10000000)    /* RPN = LOCAL_MEM_LOCAL_ADRS */
    ori     r7, r7, 0x003f           /* User/Supervisor XWR*/
    mtspr   MAS3, r7
    tlbwe
    tlbsync

    /* TLB#3: 0xE0000000 -> 0xE0100000 1MB */

    addis   r4, 0, 0x1003            /* TLBSEL = TLB1(CAM) , ESEL = 0 */
    ori     r4, r4, 0x0000
    mtspr   MAS0, r4
    addis   r5, 0, 0xc000            /* V = 1, IPROT = 0, TID = 0*/
    ori     r5, r5, _MMU_TLB_SZ_1M   /* TS = 0, TSIZE = 16 MByte page size*/
    mtspr   MAS1, r5
    addis   r6, 0, HI(CCSBAR)        /* EPN = 0xe0000000*/
    ori     r6, r6, 0x000a           /* WIMGE = 10110 */
    mtspr   MAS2, r6
    addis   r7, 0, HI(CCSBAR)        /* RPN = 0xe0000000*/
    ori     r7, r7, 0x003f           /* Supervisor XWR*/
    mtspr   MAS3, r7
    tlbwe
    tlbsync

    /* jump to rom cold entry */
    b       cold

checkStop:
    ba      0

FUNC_END(resetEntry)


#ifndef BOOT_FROM_UBOOT
/*******************************************************************************
*
* resetVector - hardware reset vector
*
* At power-on, the processor starts executing at ROM address 0xfffffffc --
* the top of the address space -- which must be a jump to resetEntry.
* This is defined as a separate "section" to assist the linker in locating
* it properly.
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
#endif
