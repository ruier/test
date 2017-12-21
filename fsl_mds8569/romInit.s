/* romInit.s - ROM bootcode for Freescale MDS8569 BSP */

/*
 * Copyright (c) 2009, 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,07jul11,y_c  Support VxWorks native bootrom. (WIND00240156)
01a,10jul09,x_z  derived from version 01c of fsl_mds8568/romInit.s.
*/

/*
DESCRIPTION
This module contains the entry code for the VxWorks bootrom.
The entry point romInit, is the first code executed on power-up.
It sets the BOOT_COLD parameter to be passed to the generic romStart() routine.

The routine sysToMonitor() jumps to the location 4 bytes past the beginning of
romInit, to perform a "warm boot".
This entry point allows a parameter to be passed to romStart().

INCLUDE FILES: sysLib.h asm.h regs.h mmuE500Lib.h

SEE ALSO:
\tb "PowerPC e500 Core Family Reference Manual",
\tb "MPC8569E PowerQUICC III Integrated Processor Reference Manual",
\tb "MPC8569E-MDS-PB HW User Guide"
*/

#define _ASMLANGUAGE

#include <vxWorks.h>
#include <sysLib.h>
#include <asm.h>
#include <regs.h>
#include <arch/ppc/mmuE500Lib.h>
#include "config.h"

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

#define WRITE_BYTE(reg1,reg2,addr32,val)        \
    lis reg1, HI(addr32);                       \
    ori reg1, reg1, LO(addr32);                 \
    lis reg2, 0x00;                             \
    ori reg2, reg2, LO(val);                    \
    stb reg2, 0(reg1)

FUNC_EXPORT(romInit)
FUNC_EXPORT(_romInit)

FUNC_IMPORT(romStart)

    _WRS_TEXT_SEG_START

    .fill   0x100, 1, 0

/*******************************************************************************
*
* romInit - entry point for VxWorks in ROM
*
* This routine is the entry point for VxWorks in ROM.
*
* RETURNS: N/A
*
* ERRNO
*/

FUNC_BEGIN(romInit)
FUNC_BEGIN(_romInit)

    b       resetEntry
    b       warm

    .ascii  "Copyright (c) 2009 Wind River Systems, Inc."
    .balign 4

cold:
    li      r2, BOOT_CLEAR
    b       start

warm:
    mr      r2, r3

start:
    xor     r0, r0, r0
    addi    r1, r0, -1

    /* initialize the timer */

    mtspr   DEC, r0
    mtspr   TBL, r0
    mtspr   TBU, r0
    mtspr   TSR, r1
    mtspr   TCR, r0
    mtspr   ESR, r0
    mtspr   XER, r0
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
* This routine is the first function to be called by the Book E core after
* reset. It is located at ROM address 0xfffff000. It is responsible for
* initializing key registers to a quiescent state, initializing the the
* TLB with a static memory mapping adequate for running the bootrom, and
* clearing the rest of the TLB. It sets up device access registers so
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

    xor     r0, r0, r0
    mtmsr   r0
    isync

    mtspr   L1CSR0, r0          /* Disable the Data cache           */
    li      r6, _PPC_L1CSR_FI
    msync
    isync

    mtspr   L1CSR0, r6          /* Invalidate the Data cache        */
    li      r6, _PPC_L1CSR_E
    msync
    isync

    mtspr   L1CSR0, r6          /* Enable the Data cache            */
    li      r6, 0x0000
    msync
    isync

    mtspr   L1CSR1, r6          /* Disable the Instruction cache    */
    li      r6, _PPC_L1CSR_FI
    msync
    isync

    mtspr   L1CSR1, r6          /* Invalidate the Instruction cache */
    isync

    li      r6, _PPC_L1CSR_E
    msync
    isync
    mtspr   L1CSR1, r6          /* Enable the Instruction cache     */
    msync
    isync

    mbar 0

    /* clear core registers */

    mtspr   SRR0,   r0
    mtspr   SRR1,   r0
    mtspr   CSRR0,  r0
    mtspr   CSRR1,  r0
    mtspr   MCSRR0, r0
    mtspr   MCSRR1, r0
    mtspr   ESR,    r0
    mtspr   MCSR,   r0
    mtspr   DEAR,   r0
    mtspr   DBCR0,  r0
    mtspr   DBCR1,  r0
    mtspr   DBCR2,  r0
    mtspr   IAC1,   r0
    mtspr   IAC2,   r0
    mtspr   DAC1,   r0
    mtspr   DAC2,   r0

    mfspr   r1,     DBSR
    mtspr   DBSR,   r1

    mtspr   PID0,   r0
    mtspr   PID1,   r0
    mtspr   PID2,   r0
    mtspr   TCR,    r0
    mtspr   BUCSR,  r0

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

    /* TLB#1: CCSBAR_RESET 0xFF700000 -> 0xFF800000 (1MB) */

    addis   r4, 0, 0x1001                    /* TLB1 entry#1 */
    ori     r4, r4, 0x0000
    mtspr   MAS0, r4
    addis   r5, 0, 0xc000                    /* V = 1, IPROT = 1, TID = 0 */
    ori     r5, r5, _MMU_TLB_SZ_1M           /* TS = 0, TSIZE = 1 MByte */
    mtspr   MAS1, r5
    addis   r6, 0, HI(CCSBAR_RESET)          /* EPN */
    ori     r6, r6, 0x000a                   /* WIMGE = 01010 */
    mtspr   MAS2, r6
    addis   r7, 0, HI(CCSBAR_RESET)          /* RPN */
    ori     r7, r7, 0x0015                   /* Supervisor XWR */
    mtspr   MAS3, r7
    tlbwe
    tlbsync

    /* TLB#2: 0xE0000000 -> 0xE0100000 1MB */

    addis   r4, 0, 0x1002                   /* TLB1 entry#2 */
    ori     r4, r4, 0x0000
    mtspr   MAS0, r4
    addis   r5, 0, 0xc000                   /* V = 1, IPROT = 0, TID = 0     */
    ori     r5, r5, _MMU_TLB_SZ_1M          /* TS = 0, TSIZE = 1 MByte       */
    mtspr   MAS1, r5
    addis   r6, 0, HI(CCSBAR)               /* EPN = CCSBAR                  */
    ori     r6, r6, 0x000a                  /* WIMGE = 01010                 */
    mtspr   MAS2, r6
    addis   r7, 0, HI(CCSBAR)               /* RPN = CCSBAR                  */
    ori     r7, r7, 0x003f                  /* Supervisor XWR                */
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

    /* LAW0 - DDR SDRAM */

    WRITEADR(r4, r5, M85XX_LAWBARn(CCSBAR, 0), 0)
    WRITEADR(r6, r7, M85XX_LAWARn(CCSBAR, 0), LAWAR_ENABLE | LAWAR_TGTIF_DDRSDRAM | LAWAR_SIZE_1GB)
    isync

    /* LAW1 - NOR Flash */

    WRITEADR(r4, r5, M85XX_LAWBARn(CCSBAR, 1), FLASH_BASE_ADRS_LAW)
    WRITEADR(r6, r7, M85XX_LAWARn(CCSBAR, 1), LAWAR_ENABLE | LAWAR_TGTIF_LBC | LAWAR_SIZE_32MB)
    isync

    /* LAW2 - BCSR */

    WRITEADR(r4, r5, M85XX_LAWBARn(CCSBAR, 2), BCSR_BASE_ADRS_LAW)
    WRITEADR(r6, r7, M85XX_LAWARn(CCSBAR, 2), LAWAR_ENABLE | LAWAR_TGTIF_LBC | LAWAR_SIZE_32KB)
    isync

    /* setup LBC CSs */

    WRITEADR(r6, r7, M85XX_BR0(CCSBAR), 0xfe000801)
    WRITEADR(r6, r7, M85XX_OR0(CCSBAR), 0xfe000ff7)
    isync

    WRITEADR(r6, r7, M85XX_BR1(CCSBAR), 0xf8000801)
    WRITEADR(r6, r7, M85XX_OR1(CCSBAR), 0xffff89f7)
    isync

    WRITEADR(r6, r7, M85XX_BR3(CCSBAR), 0xfc000c21)
    WRITEADR(r6, r7, M85XX_OR3(CCSBAR), 0xfffc0396)
    isync
    
    WRITEADR(r6, r7, M85XX_BR4(CCSBAR), 0xf8008801)
    WRITEADR(r6, r7, M85XX_OR4(CCSBAR), 0xffff89f7)
    isync

    WRITEADR(r6, r7, M85XX_BR5(CCSBAR), 0xf8010801)
    WRITEADR(r6, r7, M85XX_OR5(CCSBAR), 0xffff89f7)
    
    isync
    msync
    mbar    0

    /* Initialise the Local Bus Controller */

    WRITEADR(r6, r7, M85XX_LBCR(CCSBAR), 0x40040000)
    WRITEADR(r6, r7, M85XX_LCRR(CCSBAR), 0x80030008)

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

    /* Initialize the DDR Memory controller */

    lis     r6, HI(DDRBA)
    ori     r6, r6, LO(DDRBA)       /* r6 = DDR base */

    WRITEOFFSET(r6, r7, (CS0_BNDS), 0x0000003f)
    WRITEOFFSET(r6, r7, (CS1_BNDS), 0x00000000)
    WRITEOFFSET(r6, r7, (CS2_BNDS), 0x00000000)
    WRITEOFFSET(r6, r7, (CS3_BNDS), 0x00000000)
    WRITEOFFSET(r6, r7, (CS0_CONFIG), 0x80014202)
    WRITEOFFSET(r6, r7, (CS1_CONFIG), 0x00000000)
    WRITEOFFSET(r6, r7, (CS2_CONFIG), 0x00000000)
    WRITEOFFSET(r6, r7, (CS3_CONFIG), 0x00000000)

    WRITEOFFSET(r6, r7, (DDR_SDRAM_CLK_CTRL), 0x02000000)
    WRITEOFFSET(r6, r7, (DDR_DATA_INIT), 0x00000000)
    WRITEOFFSET(r6, r7, (DDR_SDRAM_MODE_CFG_2), 0x00000000)
    WRITEOFFSET(r6, r7, (DDR_SDRAM_MD_CNTL), 0x00000000)

    WRITEOFFSET(r6, r7, (EXTENDED_REF_REC), 0x00010000)
    WRITEOFFSET(r6, r7, (TIMING_CFG_0), 0x00660804)
    WRITEOFFSET(r6, r7, (TIMING_CFG_1), 0x5d5bd746)
    WRITEOFFSET(r6, r7, (TIMING_CFG_2), 0x0fa8c8cd)
    WRITEOFFSET(r6, r7, (TIMING_CFG_4), 0x00440001)
    WRITEOFFSET(r6, r7, (TIMING_CFG_5), 0x03402400)

    WRITEOFFSET(r6, r7, (DDR_DDR_ZQ_CNTL), 0x00000000)
    WRITEOFFSET(r6, r7, (DDR_DDR_WRLVL_CNTL), 0x8675f508)
   
    WRITEOFFSET(r6, r7, (DDR_SDRAM_CFG), 0x47000000)
    WRITEOFFSET(r6, r7, (DDR_SDRAM_CFG_2), 0x24401040)

    WRITEOFFSET(r6, r7, (DDR_SDRAM_MODE_CFG), 0x00021221) 
    WRITEOFFSET(r6, r7, (DDR_SDRAM_INTERVAL), 0x0a280100)
    
    isync

    li      r4, 0x2000
    mtctr   r4
ddrDelay:
    nop
    bdnz    ddrDelay

    WRITEOFFSET(r6, r7, (DDR_SDRAM_CFG), 0xC7000000)

    isync

    /* now that memory is stable we reset TLB entries for standard operation */

    /* TLB#0: 0xFC000000 -> 0xFFFFFFFF 64MB */

    addis   r4, 0, 0x1000                   /* TLB1 entry#0 */
    ori     r4, r4, 0x0000
    mtspr   MAS0, r4
    addis   r5, 0, 0xc000                   /* V = 1, IPROT = 1, TID = 0     */
    ori     r5, r5, _MMU_TLB_SZ_64M         /* TS = 0, TSIZE = 64 MByte      */
    mtspr   MAS1, r5
    addis   r6, 0, HI(FLASH_BASE_ADRS_TLB)  /* EPN = 0xfc000000              */
    ori     r6, r6, 0x0016                  /* WIMGE = 10110                 */
    mtspr   MAS2, r6
    addis   r7, 0, 0xfc00                   /* RPN = 0xfc000000              */
    ori     r7, r7, 0x0015                  /* Supervisor XWR                */
    mtspr   MAS3, r7
    tlbwe
    tlbsync

    /* TLB#1: 0x00000000 -> 0x3FFFFFFF 1GB */

    addis   r4, 0, 0x1001                   /* TLB1 entry#1 */
    ori     r4, r4, 0x0000
    mtspr   MAS0, r4
    addis   r5, 0, 0xc000                   /* V = 1, IPROT = 1, TID = 0     */
    ori     r5, r5, _MMU_TLB_SZ_1G          /* TS = 0, TSIZE = 1GByte        */
    mtspr   MAS1, r5
    addis   r6, 0, HI(LOCAL_MEM_LOCAL_ADRS) /* EPN = LOCAL_MEM_LOCAL_ADRS    */
    ori     r6, r6, 0x0004                  /* WIMGE = 00100                 */
    mtspr   MAS2, r6
    addis   r7, 0, HI(LOCAL_MEM_LOCAL_ADRS) /* RPN = LOCAL_MEM_LOCAL_ADRS    */
    ori     r7, r7, 0x003f                  /* User/Supervisor XWR           */
    mtspr   MAS3, r7
    tlbwe
    tlbsync

    /* TLB#2: 0xF8000000 -> 0xF8010000 64K */

    addis   r4, 0, 0x1002                   /* TLB1 entry#2 */
    ori     r4, r4, 0x0000
    mtspr   MAS0, r4
    addis   r5, 0, 0xc000                   /* V = 1, IPROT = 0, TID = 0     */
    ori     r5, r5, _MMU_TLB_SZ_64K         /* TS = 0, TSIZE = 64 KByte      */
    mtspr   MAS1, r5
    addis   r6, 0, HI(BCSR_BASE_ADRS)       /* EPN = BCSR_BASE_ADRS          */
    ori     r6, r6, 0x000a                  /* WIMGE = 01010                 */
    mtspr   MAS2, r6
    addis   r7, 0, HI(BCSR_BASE_ADRS)       /* RPN = BCSR_BASE_ADRS          */
    ori     r7, r7, 0x003f                  /* Supervisor XWR                */
    mtspr   MAS3, r7
    tlbwe
    tlbsync

    /* TLB#3: 0xE0000000 -> 0xE0100000 1MB */

    addis   r4, 0, 0x1003                   /* TLB1 entry#3 */
    ori     r4, r4, 0x0000
    mtspr   MAS0, r4
    addis   r5, 0, 0xc000                   /* V = 1, IPROT = 0, TID = 0     */
    ori     r5, r5, _MMU_TLB_SZ_1M          /* TS = 0, TSIZE = 1 MByte       */
    mtspr   MAS1, r5
    addis   r6, 0, HI(CCSBAR)               /* EPN = CCSBAR                  */
    ori     r6, r6, 0x000a                  /* WIMGE = 01010                 */
    mtspr   MAS2, r6
    addis   r7, 0, HI(CCSBAR)               /* RPN = CCSBAR                  */
    ori     r7, r7, 0x003f                  /* Supervisor XWR                */
    mtspr   MAS3, r7
    tlbwe
    tlbsync

	/* invalidate the rest of the TLB entries */

    li      r5,   12                        /* 4 of all 16 entries are used  */
    mtspr   CTR,  r5
    lis     r4,   0x1003                    /* TLBSEL = TLB1(CAM) , ESEL = 0 */
tlbInvalidateLoop:
    addis   r4,   r4, 0x0001                /* increment the TLB entry index */
    mtspr   MAS0, r4
    mtspr   MAS1, r0                        /* V = 0, IPROT = 0, TID = 0     */
    mtspr   MAS2, r0
    mtspr   MAS3, r0
    tlbwe
    tlbsync
    bdnz    tlbInvalidateLoop

    /* jump to rom cold entry */

    b       cold

checkStop:
    ba      0
FUNC_END(resetEntry)

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
