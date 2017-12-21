/* romInit.s - ROM bootcode for fsl_p1021_mds BSP */

/*
 * Copyright (c) 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,17Doct10,y_y  Created
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
\tb "PowerPC e500v2 Core Family Reference Manual",
\tb "P1021 Reference Manual (Rev.2)",
\tb "P1021-MDS_PB HW User Guide"
\tb "User_Manual_P1021MDS_PB"
*/

#define _ASMLANGUAGE

#include <vxWorks.h>
#include <sysLib.h>
#include <asm.h>
#include <regs.h>
#include <arch/ppc/mmuE500Lib.h>
#include "config.h"

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

#ifdef BSP_BOOT_CACHE_SYNC
FUNC_EXPORT(sysBspCacheSync)
FUNC_EXPORT(_sysBspCacheSync)
#endif /* BSP_BOOT_CACHE_SYNC */

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
romWarm:
    bl     warm

    /* Useful to have recognizeable string */

    .ascii   "Wind River vxWorks bootrom"
    .balign 4

cold:
    li  r2, BOOT_COLD
    b   start

warm:

    mr      r2, r3
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
    mtspr   XER, r0                 /*  clear Fixed-Point Exception Reg */

    
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
    add     r6, r6, r8              /* ROM base */
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

FUNC_BEGIN(resetEntry)
FUNC_LABEL(_resetEntry)

    xor     r0, r0, r0
    mtmsr   r0
    isync
    mbar 0

    /* clear core registers */

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
    mtspr   PID2,   r0
    mtspr   TCR,    r0
    mtspr   BUCSR,  r0

    /* Clear and set up some registers. */
    
    li      r0,0x0000
    lis     r1,0xffff
    mtspr	DEC,r0          /* prevent dec exceptions */
    mttbl	r0              /* prevent fit & wdt exceptions */
    mttbu	r0
    mtspr	TSR,r1          /* clear all timer exception status */
    mtspr	TCR,r0          /* disable all */
    mtspr	ESR,r0          /* clear exception syndrome register */
    mtspr	MCSR,r0         /* machine check syndrome register */
    mtxer	r0              /* clear integer exception register */

    /* Enable Time Base and Select Time Base Clock */

    lis  r0,HID0_EMCP@h     /* Enable machine check */
    ori  r0,r0,HID0_ENMAS7@l /* Enable MAS7 */
    mtspr HID0,r0
    
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

    addis   r4, 0, 0x1000                   /* TLBSEL = TLB1(CAM) , ESEL = 0 */
    ori     r4, r4, 0x0000
    mtspr   MAS0, r4
    addis   r5, 0, 0xc000                   /* V = 1, IPROT = 1, TID = 0     */
    ori     r5, r5, _MMU_TLB_SZ_256M        /* TS = 0, TSIZE = 64 MByte      */
    mtspr   MAS1, r5
    addis   r6, 0, HI(0xf0000000)           /* EPN = 0xf0000000              */
    ori     r6, r6, 0x000a                  /* WIMGE = 01010                 */
    mtspr   MAS2, r6
    addis   r7, 0, 0xf000                   /* RPN = 0xf0000000              */
    ori     r7, r7, 0x003F                  /* Supervisor XWR                */
    mtspr   MAS3, r7
    tlbwe
    tlbsync

    /* TLB#1: 0x00000000 -> 0x0FFFFFFF 256MB */

    addis   r4, 0, 0x1001                   /* TLBSEL = TLB1(CAM) , ESEL = 1 */
    ori     r4, r4, 0x0000
    mtspr   MAS0, r4
    addis   r5, 0, 0xc000                   /* V = 1, IPROT = 1, TID = 0     */
    ori     r5, r5, _MMU_TLB_SZ_256M        /* TS = 0, TSIZE = 256MByte      */
    mtspr   MAS1, r5
    addis   r6, 0, HI(0)                    /* EPN = LOCAL_MEM_LOCAL_ADRS    */
    ori     r6, r6, 0x0004                  /* WIMGE = 00100                 */
    mtspr   MAS2, r6
    addis   r7, 0, HI(0)                    /* RPN = LOCAL_MEM_LOCAL_ADRS    */
    ori     r7, r7, 0x003f                  /* Supervisor XWR                */
    mtspr   MAS3, r7
    tlbwe
    tlbsync
    
    /* TLB#2: 0x10000000 -> 0x1FFFFFFF 256MB */

    addis   r4, 0, 0x1002                   /* TLBSEL = TLB1(CAM) , ESEL = 1 */
    ori     r4, r4, 0x0000
    mtspr   MAS0, r4
    addis   r5, 0, 0xc000                   /* V = 1, IPROT = 1, TID = 0     */
    ori     r5, r5, _MMU_TLB_SZ_256M        /* TS = 0, TSIZE = 256MByte      */
    mtspr   MAS1, r5
    addis   r6, 0, HI(0x10000000)           /* EPN = LOCAL_MEM_LOCAL_ADRS    */
    ori     r6, r6, 0x0004                  /* WIMGE = 00100                 */
    mtspr   MAS2, r6
    addis   r7, 0, HI(0x10000000)           /* RPN = LOCAL_MEM_LOCAL_ADRS    */
    ori     r7, r7, 0x003f                  /* User/Supervisor XWR           */
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

FUNC_END(resetEntry)

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
