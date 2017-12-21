/* romInit.s - Freescale rdb8323 ROM initialization module */

/*
 * Copyright (c) 2007, 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,02jun09,e_d  code cleanup
01a,10jan07,pmr  written from mds8360
*/

/*
DESCRIPTION
This module contains the entry code for the VxWorks bootrom.
The entry point romInit, is the first code executed on power-up.
It sets the BOOT_COLD parameter to be passed to the generic
romStart() routine.

The routine sysToMonitor() jumps to the location 8 bytes
past the beginning of romInit, to perform a "warm boot".
This entry point allows a parameter to be passed to romStart().
*/

#define _ASMLANGUAGE
#include <vxWorks.h>
#include <asm.h>
#include <cacheLib.h>
#include <config.h>
#include <regs.h>
#include <sysLib.h>

#define WRITEADR(reg1,reg2,addr32,val) \
    lis reg1, HI(addr32); \
    ori reg1, reg1, LO(addr32); \
    lis reg2, HI(val); \
    ori reg2, reg2, LO(val); \
    stw reg2, 0(reg1)

#define WRITEOFFSET(regbase,reg2,offset,val) \
    lis reg2, HI(val); \
    ori reg2, reg2, LO(val); \
    stw reg2, offset(regbase);

    /* internals */

    FUNC_EXPORT(_romInit)       /* start of system code */
    FUNC_EXPORT(romInit)        /* start of system code */

    /* externals */

    .extern romStart    /* system initialization routine */

    .text

/*
 * Hard Reset Configuration Word (HRCW) See config.h/ads834x.h/target.ref for
 * more info
 */

    .fill   8,1,HRCW_LOW_BYTE0
    .fill   8,1,HRCW_LOW_BYTE1
    .fill   8,1,HRCW_LOW_BYTE2
    .fill   8,1,HRCW_LOW_BYTE3
    .fill   8,1,HRCW_HIGH_BYTE0
    .fill   8,1,HRCW_HIGH_BYTE1
    .fill   8,1,HRCW_HIGH_BYTE2
    .fill   8,1,HRCW_HIGH_BYTE3
    .fill   192,1,0     /* The rest of the space are filled with zeros */

/*******************************************************************************
*
* romInit - entry point for VxWorks in ROM
*
*
* romInit
*     (
*     int startType /@ only used by 2nd entry point @/
*     )

*/

_romInit:
romInit:
    bl  cold        /* jump to the cold boot initialization */
    nop
    bl  warm        /* jump to the warm boot initialization */

    /* copyright notice appears at beginning of ROM (in TEXT segment) */

    .ascii   "Boot Image Wind River Systems, Inc."
    .align 2

cold:
    li      r3, BOOT_COLD   /* set cold boot as start type */

    /*
     * initialize the IMMR register before any non-core registers
     * modification. The default IMMR base address was 0xFF400000,
     * as originally programmed in the Hard Reset Configuration Word.
     */

    lis r4, HI (CCSBAR)
    ori r4, r4, LO (CCSBAR)

    lis     r8,HIADJ(CCSBAR_INIT)   /* IMMR was at 0xff400000 */
    ori     r8, r8, LO(CCSBAR_INIT) /* IMMR now at CCSBAR */
    stw     r4,0(r8)
    isync
    sync

    sync
    isync

    lis  r5, HIADJ(QUICC_OR0(CCSBAR))
    addi r5, r5, LO(QUICC_OR0(CCSBAR))

    lwz  r6, 0(r5)
    ori  r6, r6, 0xff7
    stw  r6, 0(r5)
    isync           /* synchronize */

    WRITEADR(r6,r7,M83XX_LBLAWBARn(CCSBAR,1), 0x80000000)

    WRITEADR(r6,r7,M83XX_LBLAWARn(CCSBAR,1), \
             (LAWAR_ENABLE |  LAWAR_SIZE_2GB ) )

    lis  r6,HI(M83XX_LBLAWARn(CCSBAR,1))
    ori  r6,r6,LO(M83XX_LBLAWARn(CCSBAR,1))
    lwz  r7,0(r6)

    WRITEADR(r6,r7,M83XX_LBLAWBARn(CCSBAR,0), 0x00000000)

    WRITEADR(r6,r7,M83XX_LBLAWARn(CCSBAR,0), \
             (LAWAR_ENABLE |  LAWAR_SIZE_2GB ))

    lis  r6,HI(M83XX_LBLAWARn(CCSBAR,0))
    ori  r6,r6,LO(M83XX_LBLAWARn(CCSBAR,0))
    lwz  r7,0(r6)

    isync

    /*
     * When the PowerPC 83xx is powered on, the processor fetches the
     * instructions located at the address 0x100. We need to jump
     * from the address 0x100 to the Flash space.
     */

    lis     r4, HIADJ(start)                /* load r4 with the address */
    addi    r4, r4, LO(start)               /* of start */

    lis     r5, HIADJ(romInit)              /* load r5 with the address */
    addi    r5, r5, LO(romInit)             /* of romInit() */

    lis     r6, HIADJ(ROM_TEXT_ADRS)        /* load r6 with the address */
    addi    r6, r6, LO(ROM_TEXT_ADRS)       /* of ROM_TEXT_ADRS */

    sub     r4, r4, r5                      /*  */
    add     r4, r4, r6

    mtspr   LR, r4                          /* save destination address*/
                                            /* into LR register */
    blr                                     /* jump to flash mem address */
start:

    /* set the MSR register to a known state */

    xor r0, r0, r0      /* clear register R0 */
    isync               /* synchronize */
    mtmsr   r0          /* clear the MSR register */
    sync
    isync               /* synchronize */

    WRITEADR(r6,r7,QUICC_BR0(CCSBAR),ROM_BASE_ADRS|0x1001)

    WRITEADR(r6,r7,QUICC_OR0(CCSBAR),0xFE006FF7)

    WRITEADR(r6,r7,M83XX_LBLAWBARn(CCSBAR,0), ROM_BASE_ADRS)

    WRITEADR(r6,r7,M83XX_LBLAWARn(CCSBAR,0), \
             (LAWAR_ENABLE | LAWAR_SIZE_16MB ))

    WRITEADR(r6,r7,M83XX_LBLAWARn(CCSBAR,1), 0x00000000)
    WRITEADR(r6,r7,M83XX_LBLAWBARn(CCSBAR,1), 0x10000000)

    WRITEADR(r6,r7, M83XX_SICRL(CCSBAR),0x00000000)
    isync
    WRITEADR(r6,r7, M83XX_DDRCDR(CCSBAR),0x73000002)
    isync

    WRITEADR(r6,r7, M83XX_RMR(CCSBAR),0x00000001)
    isync
    WRITEADR(r6,r7, M83XX_RSR(CCSBAR),0xffffffff)
    isync

    /* disable clocks, peripheral driver or sysHwInit() will start  */

    WRITEADR(r6,r7, QUICC_LBCR(CCSBAR),0)
    isync


    /* Initialise the Local Bus Controller */

    WRITEADR(r6,r7,QUICC_LCRR(CCSBAR),0x80000002)
    li  r6,0x0020
    mtctr r6
lbcdelay:
    nop
    bdnz lbcdelay
    nop
    isync
    sync

    mtspr   1008, r0        /* HID0 */
    isync

    /* Zero-out registers: SPRGs */

    addis    r0,0,0
    isync               /* synchronize */

    mtspr   272,r0
    mtspr   273,r0
    mtspr   274,r0
    mtspr   275,r0
    isync               /* synchronize */

    /* zero-out the Segment registers */

    mtsr    0,r0
    isync
    mtsr    1,r0
    isync
    mtsr    2,r0
    isync
    mtsr    3,r0
    isync
    mtsr    4,r0
    isync
    mtsr    5,r0
    isync
    mtsr    6,r0
    isync
    mtsr    7,r0
    isync
    mtsr    8,r0
    isync
    mtsr    9,r0
    isync
    mtsr    10,r0
    isync
    mtsr    11,r0
    isync
    mtsr    12,r0
    isync
    mtsr    13,r0
    isync
    mtsr    14,r0
    isync
    mtsr    15,r0
    isync

    /* invalidate DBATs: clear VP and VS bits */

    mtspr   536,r0   /* Data bat register 0 upper */
    isync
    mtspr   538,r0   /* Data bat register 1 upper */
    isync
    mtspr   540,r0   /* Data bat register 2 upper */
    isync
    mtspr   542,r0   /* Data bat register 3 upper */
    isync

    /* invalidate IBATs: clear VP and VS bits */

    mtspr   528,r0   /* Instruction bat register 0 upper */
    isync
    mtspr   530,r0   /* Instruction bat register 1 upper */
    isync
    mtspr   532,r0   /* Instruction bat register 2 upper */
    isync
    mtspr   534,r0   /* Instruction bat register 3 upper */
    isync

    /* invalidate TLBs: loop on all TLB entries using r7 as an index */

    addi     r0,0,0x0020
    mtspr    9,r0           /* Load CTR with 32 */
    addi     r7,0,0             /* Use r7 as the tlb index */

tlb_write_loop:

    tlbie    r7                 /* invalidate the tlb entry */
    sync
    addi     r7,r7,0x1000           /* increment the index */
    bc       16,0,tlb_write_loop    /* Decrement CTR, then branch if the */
                                    /* decremented CTR is not equal to 0 */

    /* Turn off data and instruction cache control bits */

    mfspr   r7, HID0
    isync
    sync                        /* synchronize */
    andi.   r7,r7,0x3FFF        /* Clear DCE and ICE bits */
    mtspr   HID0,r7
    isync
    sync                /* synchronize */

    WRITEADR(r6,r7,QUICC_OR1 (CCSBAR), 0x0)

    WRITEADR(r6,r7,QUICC_BR1 (CCSBAR), 0)

    isync
    sync

#ifdef INCLUDE_DDR_SDRAM

    /* Memory mapped region base address */

    WRITEADR(r6,r7,M83XX_DDRLAWBARn(CCSBAR,0), \
             DDR_SDRAM_LOCAL_ADRS)

    WRITEADR(r6,r7,M83XX_DDRLAWARn(CCSBAR,0), \
             LAWAR_ENABLE  | LAWAR_SIZE_64MB )

    nop
    nop
    nop
    nop
    isync

/*
DDRMEMC                         register Group. (DDRMEMC)
CS0_BNDS                       = 00000007
CS0_CONFIG                     = 80840102
TIMING_CFG3                    = 00000000
TIMING_CFG0                    = 00220802
TIMING_CFG1                    = 26256222
TIMING_CFG2                    = 0F9028C7
DDR_SDRAM_CFG                  = C3080000
DDR_SDRAM_CFG2                 = 00401000
DDR_SDRAM_MODE                 = 44400232
DDR_SDRAM_MODE2                = 8000C000
DDR_SDRAM_MDCNTL               = 00000000
DDR_SDRAM_INT                  = 03200064
DDR_DATA_INIT                  = 00000000
DDR_SDRAM_CLK                  = 02000000
DDR_INIT_ADDR                  = 00000000
DDR_IP_REV1                    = 00020200
DDR_IP_REV2                    = 00000000
DDR_SDRAM_CFG3                 = C3080000
DDR_SDRAM_CFG4                 = C3080000
DDR_SDRAM_CFG5                 = C3080000
>BKM>
*/

    /* Initialize the DDR Memory controller */

    lis r6, HI(DDRBA)
    ori r6, r6, LO(DDRBA)       /* r6 = DDR base */
    WRITEOFFSET(r6,r7,(CS0_BNDS), 0x00000003)
    WRITEOFFSET(r6,r7,(CS0_CONFIG), 0x80840101)
    WRITEOFFSET(r6,r7,(TIMING_CFG_3), 0x00000000)
    WRITEOFFSET(r6,r7,(TIMING_CFG_0), 0x00220802)
    WRITEOFFSET(r6,r7,(TIMING_CFG_1), 0x3935D322)
    WRITEOFFSET(r6,r7,(TIMING_CFG_2), 0x0F9048CA)
    WRITEOFFSET(r6,r7,(DDR_SDRAM_CFG), 0x43080000)
    WRITEOFFSET(r6,r7,(DDR_SDRAM_CFG_2), 0x00401000)
    WRITEOFFSET(r6,r7,(DDR_SDRAM_MODE_CFG), 0x44400232)
    WRITEOFFSET(r6,r7,(DDR_SDRAM_MODE_CFG_2), 0x8000C000)
    WRITEOFFSET(r6,r7,(DDR_DATA_INIT), 0x00000000)
    WRITEOFFSET(r6,r7,(DDR_SDRAM_INTERVAL), 0x03200064)
    WRITEOFFSET(r6,r7,(DDR_SDRAM_CLK_CNTRL), 0x02000000)

    lis  r7,0x0004
    mtctr r7
delayddr1:
    nop
    bdnz delayddr1

    WRITEOFFSET(r6,r7,(DDR_SDRAM_CFG), 0xc3080000)

    sync
    isync

#ifdef INCLUDE_ECC
    WRITEOFFSET(r6,r7,(DDR_ERR_DISABLE), 0x00000000)
    WRITEOFFSET(r6,r7,(DDR_ERR_SBE), 0x00ff0000)
    isync
    sync
    lis     r7, 0x1
    mtspr   9,  r7
eccDelayLoop:
    nop
    bdnz    eccDelayLoop
    WRITEOFFSET(r6,r7,(DDR_SDRAM_CFG), 0xe2000000)
#endif /* INCLUDE_ECC */
    isync

#endif /* INCLUDE_DDR_SDRAM */

#ifdef INCLUDE_LBC_SDRAM

    /* Initialise SDRAM */

SdramInit:

    /* Memory mapped region base address */

    WRITEADR(r6,r7,M83XX_LBLAWBARn(CCSBAR,1), (LBC_SDRAM_LOCAL_ADRS))

    WRITEADR(r6,r7,M83XX_LBLAWARn(CCSBAR,1), \
             (LAWAR_ENABLE |  LAWAR_SIZE_64MB ))

    lis  r6,HI(M83XX_LBLAWARn(CCSBAR,1))
    ori  r6,r6,LO(M83XX_LBLAWARn(CCSBAR,1))
    lwz  r7,0(r6)
    isync

    /* load OR2 */

    WRITEADR(r6,r7,QUICC_OR2 (CCSBAR), \
             (LBC_SDRAM_LOCAL_SIZE_MASK & 0xffff0000) | 0x6901)

    /* load BR2 */

    WRITEADR(r6,r7,QUICC_BR2 (CCSBAR), \
             (LBC_SDRAM_LOCAL_ADRS & 0xffff0000) | \
             0x1861)

    /* Pre-charge all banks */

    WRITEADR(r6,r7,QUICC_LSDMR(CCSBAR),0x2862D733 )

    lis     r9,HIADJ(LBC_SDRAM_LOCAL_ADRS)
    addi    r9, r9, LO(LBC_SDRAM_LOCAL_ADRS)

    /* do a single write to an arbitrary location */

    addi    r5,0,0x00FF      /* Load 0x000000FF into r5 */
    stb     r5,0(r9)         /* Write 0xFF to SDRAM address - bits [24-31] */
    isync

    /* issue a "Auto Refresh" command to SDRAM */

    WRITEADR(r6,r7,QUICC_LSDMR(CCSBAR),0x0862D733)

    /* do a single write to an arbitrary location */

    addi    r5,0,0x00FF      /* Load 0x000000FF into r5 */
    stb     r5,0(r9)         /* Write 0xFF to SDRAM address - bits [24-31] */
    isync

    /* issue a "Auto Refresh" command to SDRAM */

    WRITEADR(r6,r7,QUICC_LSDMR(CCSBAR),0x0862D733)

    /* do a single write to an arbitrary location */

    addi    r8,0,0x00FF      /* Load 0x000000FF into r8 */

    stb     r8,0(r9)         /* Write 0xFF to address R9  */
    stb     r8,1(r9)         /* Write 0xFF to address R9  */
    stb     r8,2(r9)         /* Write 0xFF to address R9  */
    stb     r8,3(r9)         /* Write 0xFF to address R9  */
    stb     r8,4(r9)         /* Write 0xFF to address R9  */
    stb     r8,5(r9)         /* Write 0xFF to address R9  */
    stb     r8,6(r9)         /* Write 0xFF to address R9  */
    stb     r8,7(r9)         /* Write 0xFF to address R9  */

    /* issue a "Mode Register Write" command to SDRAM */

    WRITEADR(r6,r7,QUICC_LSDMR(CCSBAR),0x1862D733)

    /* do a single write to an arbitrary location */

    addi    r8,0,0x00FF      /* Load 0x000000FF into r8 */
    stb     r8,0(r9)         /* Write 0xFF to address R9 - bits [24-31] */

    /* enable refresh services and put SDRAM into normal operation  */

    WRITEADR(r6,r7,QUICC_LSDMR(CCSBAR),0x4062D733)

    /* program the MRTPR */

    addi    r5,0,TPR  /* MRTPR[TPR] */
    lis     r6, HIADJ (QUICC_MRTPR (CCSBAR))
    addi    r6, r6, LO (QUICC_MRTPR (CCSBAR))
    sth     r5, 0x0 (r6)      /* store upper half-word */

    /* program the LSRT */

    addi    r5,0,LSRT_VALUE
    lis     r6, HIADJ (QUICC_LSRT (CCSBAR))
    addi    r6, r6, LO (QUICC_LSRT (CCSBAR))
    stb     r5, 0x0 (r6)      /* store byte - bits[24-31] */

    isync

    lis     r9, HI(LBC_SDRAM_LOCAL_ADRS)
    ori     r9,r9, LO(LBC_SDRAM_LOCAL_ADRS)

    lis r7, HIADJ(0x100)      /* Loop 256 times */
    addi    r7, r7, LO(0x100)
    mtspr   9,r7             /* Load spr CTR with 8 */
    lis     r8,0x5555      /* Load 0x000000FF into r8 */
    ori     r8,r8,0x5555

SdramWrLoop2:

    stw     r8,0(r9)            /* Write 0xFF to address R9 */
    addi    r9,r9,4             /* Move R9 to next byte */
    addi    r8,r8,1             /* Add 1 to r8 */
    bc      16,0,SdramWrLoop2   /* Decrement CTR, and possibly branch */
#endif  /* INCLUDE_LBC_SDRAM */

    WRITEADR(r6,r7, M83XX_PCILAWBARn(CCSBAR,0),0x80000000)
    WRITEADR(r6,r7, M83XX_PCILAWBARn(CCSBAR,1),0xD0000000)
    WRITEADR(r6,r7, M83XX_PCILAWARn(CCSBAR,0),0x8000001D)
    WRITEADR(r6,r7, M83XX_PCILAWARn(CCSBAR,1),0x80000019)

warm:

    /* turn the instruction cache ON for faster FLASH ROM boots */

    mfspr   r4, HID0
    ori r4, r4, _PPC_HID0_ICE | _PPC_HID0_ICFI  /* set ICE & ICFI */
    rlwinm  r5, r4, 0, _PPC_HID0_BIT_ICFI + 1, _PPC_HID0_BIT_ICFI - 1

    /* clear the ICFI bit */

    isync

    /*
     * The setting of the instruction cache enable (ICE) bit must be
     * preceded by an isync instruction to prevent the cache from being
     * enabled or disabled while an instruction access is in progress.
     */

    mtspr   HID0, r4    /* Enable Instr Cache & Inval cache */
    isync
    sync
    mtspr   HID0, r5    /* using 2 consec instructions */
                        /* E300 core recommendation */
    isync

    /*
    Modifying ACR to increase pipeline depth, must retain
    rsvd bits values. Need to allow for Errata ARB3
    */

    li      r5, 0x100
    mtctr   r5
    lis     r4, HI(CCSBAR)
    ori     r4, r4, LO(CCSBAR)
    lwz     r5, M83XX_ACR(0)(r4)
    lis     r6, 0x0003
    ori     r6, r6, 0x0300
    or      r5, r5, r6
    b       iCacheLineLoad /* instruction cache line load */
    .balign 32
loadACR:
    stw     r5, M83XX_ACR(0)(r4)
    isync
delay:  /* loop idle in cache while ACR update completes */
    nop
    bdnz  delay
    b continueBoot
iCacheLineLoad:
    b loadACR

continueBoot:

    /* initialize the stack pointer */

    lis sp, HIADJ(STACK_ADRS)
    addi    sp, sp, LO(STACK_ADRS)

    /* go to C entry point */

    addi    sp, sp, -FRAMEBASESZ        /* get frame stack */

    /*
     * calculate C entry point: routine - entry point + ROM base
     * routine  = romStart
     * entry point  = romInit   = R7
     * ROM base = ROM_TEXT_ADRS = R8
     * C entry point: romStart - R7 + R8
     */

    lis     r7, HIADJ(romInit)
    addi    r7, r7, LO(romInit)

    lis     r8, HIADJ(ROM_TEXT_ADRS)
    addi    r8, r8, LO(ROM_TEXT_ADRS)

    lis r6, HIADJ(romStart)
    addi    r6, r6, LO(romStart)    /* load R6 with C entry point */

    sub r6, r6, r7      /* routine - entry point */
    add r6, r6, r8      /* + ROM base */

    mtspr   LR, r6      /* save destination address*/
                        /* into LR register */
    blr                 /* jump to the C entry point */
