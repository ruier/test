/* romInit_nand.s - ROM bootcode for fsl_p1021mds BSP */

/*
 * Copyright (c) 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
01a,05oct10, y_y  Created.
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
    
#define WRITEOFFSET(regbase,reg2,offset,val) \
    lis reg2, HI(val);                  \
    ori reg2, reg2, LO(val);            \
    stw reg2, offset(regbase);

#define READADR(reg1,reg2,addr32,offset) \
    lis reg1, HI(addr32); \
    ori reg1, reg1, LO(addr32); \
    lwz reg2, offset(reg1)

 _WRS_TEXT_SEG_START
  .fill 0x100,1,0
    
FUNC_BEGIN(romInit)
FUNC_BEGIN(_romInit)

    /* Useful to have recognizeable string */
    .ascii   "Wind River vxWorks bootrom"
    .balign 4
    
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

    li      r2,0x1e
    mtspr   MMUCSR0, r2
    isync

    li      r3,4
    li      r4,0
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
 
    /* TLB1 #0    Covers boot page(1MB) */

    addis   r4, 0,  0x1000                  /* TLB1 entry#0 */
    ori     r4, r4, 0x0000
    mtspr   MAS0, r4
    addis   r5, 0,  0xc000                 /* V = 1, IPROT = 1, TID = 0 */
    ori     r5, r5, _MMU_TLB_SZ_1M          /* TS = 0, TSIZE = 1 MByte */
    mtspr   MAS1, r5
    addis   r6, 0, HI(NAND_BUFFER_BASE)     /* EPN */
    ori     r6, r6, 0x0008                 /* WIMGE = 01000 */
    mtspr   MAS2, r6
    addis   r7, 0, HI(NAND_BUFFER_BASE)     /* RPN */
    ori     r7, r7, 0x0015                 /* Supervisor XWR */
    mtspr   MAS3, r7
    isync
    tlbwe
    msync
    isync

    /* TLB1 #1    CCSBAR space(1MB)  */

    addis   r4, 0,  0x1001              /* TLB1 entry#1 */
    ori     r4, r4, 0x0000
    mtspr   MAS0, r4
    addis   r5, 0, 0xc000               /* V = 1, IPROT = 1, TID = 0 */
    ori     r5, r5, _MMU_TLB_SZ_1M      /* TS = 0, TSIZE = 1 MByte */
    mtspr   MAS1, r5
    addis   r6, 0, HI(CCSBAR)           /* EPN */
    ori     r6, r6, 0x000a              /* WIMGE = 01010 */
    mtspr   MAS2, r6
    addis   r7, 0, HI(CCSBAR)            /* RPN */
    ori     r7, r7, 0x0015              /* Supervisor XWR */
    mtspr   MAS3, r7
    isync
    tlbwe
    msync
    isync

    /* TLB1 #2    BCSR/PMC0/PMC1 space: 0xf8000000 -> 0xf8040000 (256kBB) */

    addis   r4, 0,  0x1002              /* TLB1 entry#2 */
    ori     r4, r4, 0x0000
    mtspr   MAS0, r4
    addis   r5, 0,  0xc000              /* V = 1, IPROT = 1, TID = 0 */
    ori     r5, r5, _MMU_TLB_SZ_256K    /* TS = 0, TSIZE = 256K Byte */
    mtspr   MAS1, r5
    addis   r6, 0, HI(0xf8000000)       /* EPN */
    ori     r6, r6, 0x000a              /* WIMGE = 01010 */
    mtspr   MAS2, r6
    addis   r7, 0, HI(0xf8000000)       /* RPN */
    ori     r7, r7, 0x0015              /* Supervisor XWR */
    mtspr   MAS3, r7
    isync
    tlbwe
    msync
    isync

    /* TLB#4    DDR 0x00000000 -> 0x3FFFFFFF 1GB */

    addis   r4, 0,  0x1004              /* TLBSEL = TLB1(CAM) , ESEL = 1*/
    ori     r4, r4, 0x0000
    mtspr   MAS0, r4
    addis   r5, 0, 0xc000               /* V = 1, IPROT = 1, TID = 0*/
    ori     r5, r5, _MMU_TLB_SZ_1G      /* TS = 0, TSIZE = 1G Byte */
    mtspr   MAS1, r5
    addis   r6, 0, HI(0)                /* EPN = LOCAL_MEM_LOCAL_ADRS */
    ori     r6, r6, 0x000a              /* WIMGE = 01010 */
    mtspr   MAS2, r6
    addis   r7, 0, HI(0)                /* RPN = LOCAL_MEM_LOCAL_ADRS */
    ori     r7, r7, 0x0015              /* User/Supervisor XWR*/
    mtspr   MAS3, r7
    isync
    msync
    tlbwe
    tlbsync

    /* TLB1 #5. CCSBAR_INIT space: 0xff700000 -> 0xff7fffff (1MB) */

    addis   r4, 0, 0x1005              /* TLB1 entry#5 */
    ori     r4, r4, 0x0000
    mtspr   MAS0, r4
    addis   r5, 0, 0xc000               /* V = 1, IPROT = 1, TID = 0 */
    ori     r5, r5, _MMU_TLB_SZ_1M      /* TS = 0, TSIZE = 1 MByte */
    mtspr   MAS1, r5
    addis   r6, 0, HI(CCSBAR_INIT)      /* EPN */
    ori     r6, r6, 0x000a              /* WIMGE = 01010 */
    mtspr   MAS2, r6
    addis   r7, 0, HI(CCSBAR_INIT)      /* RPN */
    ori     r7, r7, 0x0015              /* Supervisor XWR */
    mtspr   MAS3, r7
    isync
    msync
    tlbwe
    tlbsync

    mfspr  r6, HID1
    andi.  r6, r6, 1
    cmpwi  r6, 1
    bne    doCcsrbar

    /* running on CPU1 set up initial stack pointer */

    LOADVAR (sp, CPU1_STACK_START_ADR)
    stwu	r0, -FRAMEBASESZ(sp)		/* save one frame stack */
   
    /* jump to sysCpu1Loop - sysInit for AMP */

waitForInitStartSet:	

    LOADVAR (r3, CPU1_INIT_START_ADR)
    
    /* Check that init start address is not zero before jumping 
     * This allows cpu1 to reboot and wait for cpu0 to reload image 
     */
    lis     r4,HI(MP_MAGIC_RBV)
    ori     r4, r4, LO(MP_MAGIC_RBV)
    cmpw    r3, r4
    beq     waitForInitStartSet
    mtctr	r3
    bctr
    
doCcsrbar:
    /* Setup the memory mapped register address */

    isync
    sync

    lis     r6, HI(CCSBAR)
    ori     r6, r6, LO(CCSBAR)
    isync

    srwi    r5, r6, 12
    lis     r7, HI(CCSBAR_INIT)
    ori     r7, r7, LO(CCSBAR_INIT)
    lwz     r4, 0(r7)

    isync
    sync
    mbar 0

    stw     r5, 0(r7)

    sync
    isync
    mbar 0

    li      r4, 0x0fff
    mtctr   r4
ccsrbarWait:
    nop
    bdnz    ccsrbarWait

    /* remap the nand buffer base at 0xfff0_0000 */

    WRITEADR(r6, r7, M85XX_LAWBAR4(CCSBAR), ((NAND_BUFFER_BASE>>12) & \
                                   LAWBAR_MASK))
    
    WRITEADR(r6, r7, M85XX_LAWAR4(CCSBAR),  LAWAR_ENABLE | \
            (LAWAR_TRGT_ELBC<< LAWAR_TRGT_ELBC_SHIFT)|LAWAR_SIZE_1MB ) 
    
    /* Read back so that we sync the writes */
    
    READADR(r5,r4,CCSBAR,M85XX_LAWAR4_OFFSET)
    isync 
     
    /* Local access window 5 (CONFIG_SYS_BCSR_BASE) initialize */

    WRITEADR(r6, r7, M85XX_LAWBAR5(CCSBAR), ((BCSR_BASE_ADRS>>12) & \
                                   LAWBAR_MASK))
                                   
    WRITEADR(r6, r7, M85XX_LAWAR5(CCSBAR),  LAWAR_ENABLE | \
            (LAWAR_TRGT_ELBC<<LAWAR_TRGT_ELBC_SHIFT) | LAWAR_SIZE_256KB)
     
    /* Read back so that we sync the writes */
    
    READADR(r5,r4,CCSBAR,M85XX_LAWAR5_OFFSET)
    isync
   
    /* Local access window 6 (DDR) initialize */

    WRITEADR(r6, r7, M85XX_LAWBAR6(CCSBAR), ((LOCAL_MEM_LOCAL_ADRS>>12) & \
                                   LAWBAR_MASK))
                                   
    WRITEADR(r6, r7, M85XX_LAWAR6(CCSBAR),   LAWAR_ENABLE | \
            ( LAWAR_TRGT_DDR<< LAWAR_TRGT_ELBC_SHIFT) | LAWAR_SIZE_512MB)
            
    /* Read back so that we sync the writes */
    
    READADR(r5,r4,CCSBAR,M85XX_LAWAR6_OFFSET)
    isync

    /* Initialise the Local Bus Clock Ratio Register */
    
    WRITEADR(r6, r7, M85XX_LCRR(CCSBAR), LCRR_DBYP | LCRR_CLKDIV_8)
    
   /* BR0 OR0*/
   
    WRITEADR(r6, r7, M85XX_BR0(CCSBAR), NAND_BUFFER_BASE | ELBC_BR_MASK | \
            ELBC_BR_Valid| ELBC_BR_MSEL_FCM|ELBC_BR_DECC_HW|ELBC_BR_PS_8)

    WRITEADR(r6, r7, M85XX_OR0(CCSBAR),         0xfff00010)

    /* BR1 OR1*/
    
    WRITEADR(r6, r7, M85XX_BR1(CCSBAR),         0xf8000801)
    WRITEADR(r6, r7, M85XX_OR1(CCSBAR),         0xffff8910)

    /* BR2 OR2*/
    
    WRITEADR(r6, r7, M85XX_BR2(CCSBAR),         0xf8010801)
    WRITEADR(r6, r7, M85XX_OR2(CCSBAR),         0xffff09f7)
    
    /* BR3 OR3*/
   
    WRITEADR(r6, r7, M85XX_BR3(CCSBAR),         0xf8020801)
    WRITEADR(r6, r7, M85XX_OR1(CCSBAR),         0xffff09f7)


    /* Initialize the DDR Memory controller */
   
    lis    r6, HI(CCSBAR+0x2000)
    ori    r6, r6, LO(CCSBAR+0x2000)    /* r6 = DDR base */

    WRITEOFFSET(r6, r7, (CS0_BNDS),             0x0000001F)
     
    WRITEOFFSET(r6, r7, (CS0_CONFIG),           0x80014202)
 
    WRITEOFFSET(r6, r7, (CS0_CONFIG_2),         0x00000000)
     
    WRITEOFFSET(r6, r7, (TIMING_CFG_3),         0x00010000)
  
    WRITEOFFSET(r6, r7, (TIMING_CFG_0),         0x00330004)
   
    WRITEOFFSET(r6, r7, (TIMING_CFG_1),         0x5d5bd746)
   
    WRITEOFFSET(r6, r7, (TIMING_CFG_2),         0x0fa8c8cd)
    
    WRITEOFFSET(r6, r7, (DDR_SDRAM_CFG_2),      0x04401050)
    
    WRITEOFFSET(r6, r7, (DDR_SDRAM_MODE),       0x40461320)

    WRITEOFFSET(r6, r7, (DDR_SDRAM_MODE_2),     0x8000c000)
    
    WRITEOFFSET(r6, r7, (DDR_SDRAM_INTERVAL),   0x0a280000)

    WRITEOFFSET(r6, r7, (DDR_DATA_INIT),        0x00000000)
      
    WRITEOFFSET(r6, r7, (DDR_SDRAM_CLK_CNTL),   0x03000000)
    
    WRITEOFFSET(r6, r7, (TIMING_CFG_4),         0x00220001)

    WRITEOFFSET(r6, r7, (TIMING_CFG_5),         0x02402400)
    
    WRITEOFFSET(r6, r7, (DDR_ZQ_CNTL),          0x89080600)
    
    WRITEOFFSET(r6, r7, (DDR_WRLVL_CNTL),       0x86559608)
    
    WRITEOFFSET(r6, r7, (DDR_DDRCDR_1),         0x000eaa00)
  
    WRITEOFFSET(r6, r7, (DDR_DDRCDR_2),         0x00000000)

    /* wait before enable */

    addi    r5, 0, 0x0fff
    mtspr   CTR,r5
ddrLoop_1:
    bc      16, 0, ddrLoop_1
                      
    /* enable DDR controller */
   
    WRITEOFFSET(r6,r7,(DDR_SDRAM_CFG),          0xc70c0000)
    
   /* initialize the stack pointer */
   
    lis  sp, HIADJ(STACK_ADRS)
    addi sp, sp, LO(STACK_ADRS)
    
    /* go to C entry point */

    addi  sp, sp, -FRAMEBASESZ          /* get frame stack */
    lis   r6, HIADJ(copyBufferToRam)
    addi  r6, r6, LO(copyBufferToRam)   /* load R6 with C entry point */
    mtspr LR, r6                        /* save destination address*/
    blr                                 /* jump to the C entry point */

    isync
    nop
    nop
    nop
    nop
   
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
