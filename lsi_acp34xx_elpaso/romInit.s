/* romInit.s - LSI Acp34xx Board ROM bootcode */

/*
 * Copyright (c) 2010-2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01r,19jun12,x_s  added dynamic judgement for silicon Rev 2.0 & 2.1 Errata fix.
01q,15may12,x_s  removed workaround of erratum #38 for silicon Rev 2.1. 
01p,05oct11,d_c  removed code for PPC 476 errata 42 and 43.
01o,23aug11,x_z  fixed CCR0 value and enabled dead code for PLB6 initialization. 
01n,22mar11,x_z  added workaround for PPC476 REV2 erratum #38, #42 and #43;
                 replaced lwsync with msync for PPC476 REV2 erratum #41.
01m,08mar11,x_z  replaced INCLUDE_PCI with DRV_PCIBUS_LSI_PCIEX.
01l,10feb11,x_z  added isync for the store operations of the PID and CCRn register;
                 added missing PCIe TLB for warm reset;
                 added romTlbInit().
01k,22jan11,x_s  added TLB for PICe space.
01j,18jan11,x_z  removed redundant CCR initialization to fix uAMP issue.
01i,07jan11,x_z  added ici after initialization of I-Cache; added additional
                 PLB6 initialization.
01h,21dec10,x_z  added initialization for CCR registers.
01g,06dec10,x_z  added initialization of the I-Cache as recommended by IBM.
01f,22nov10,x_s  added PLB6 workaround.
01e,19nov10,x_z  removed L2 cache workaround for PPC476 V1.
01d,26oct10,x_z  added support for uAMP.
01c,19oct10,x_z  fixed warm reset issue.
01b,25may10,x_z  code clean.
01a,26jan10,x_s  written.
*/

#define _ASMLANGUAGE
#include <vxWorks.h>
#include <sysLib.h>
#include <asm.h>
#include <regs.h>
#include <arch/ppc/mmu476Lib.h>
#include "config.h"

/*
 * MSR register initialization
 *
 * This initial MSR value disables machine check and critical class
 * exceptions.
 */

#define MSR_VALUE   (0)

/*
 * Supervisor/user Search Priority (SSPCR/USPCR)
 *
 * If input PID != 0, the first search uses PID = 0. Then repeat the search
 * from 4K to 1G pages with input PID
 */

#define PRIORITY_ORDER_INIT             (0x9abcdef0)

/* Invalidate Search Priority (ISPCR) - from 4K to 1G pages with input PID */

#define PRIORITY_ORDER_INIT_ISPCR       (0x12345670)

/*
 * DBSR register initialization
 *
 * This is a write-1s-to-clear register. The value below clears all
 * debug event status bits but leaves the Most Recent Reset status bits
 * untouched.
 */

#define DBSR_INIT (_DBSR_IDE   | _DBSR_UDE   | \
           _DBSR_ICMP  | _DBSR_BRT   | _DBSR_IRPT  | _DBSR_TRAP  | \
           _DBSR_IAC1  | _DBSR_IAC2  | _DBSR_IAC3  | _DBSR_IAC4  | \
           _DBSR_DAC1R | _DBSR_DAC1W | _DBSR_DAC2R | _DBSR_DAC2W | \
           _DBSR_RET   | \
           _DBSR_IAC12ATS | _DBSR_IAC34ATS)

/*
 * MMUCR register initialization
 *
 * Specify TID field to be written to TLB entries.  TID = 0
 * Specify TS field to be used for TLB searches. TS = 0
 * Specify store miss alloc behavior.  On a cache miss, allocate a line
 * Disable transient cache mechanism
 * Disable cache locking exceptions
 */

#define MMUCR_INIT  0x00000000

/* TLB setup defines and functions */

#define TLB_WORD_SET(wordNum, ixReg, tempReg, value)         \
            lis   tempReg, HI(value); \
            ori   tempReg, tempReg, LO(value); \
            tlbwe tempReg, ixReg, wordNum;

/* 
 * PPC476 REV2 has some errata, this below two macros are  
 * for these workarounds implementation.
 * PPC476 REV2.1 has corrected these errata, the workaround would not be 
 * done for REV2.1 silicon revision by dynamic distinguish silicon version.
 */

#define PPC476_REV2_FIX

#ifdef PPC476_REV2_FIX

/*  instructions for I-Cache initialization */

#define L1ICACHE_INIT_B_NEXT_LINE_INST  0x48000020  /* "b . + 32" */
#define L1ICACHE_INIT_BLR_INST          0x4E800020  /* "blr" */

#endif /* PPC476_REV2_FIX */

    /* internals */

    FUNC_EXPORT(romInit)    /* entry point for cold reboot */
    FUNC_EXPORT(_romInit)   /* entry point for cold reboot */

    /* externals */

    FUNC_IMPORT(romStart)   /* system initialization routine */

    _WRS_TEXT_SEG_START

/*******************************************************************************
*
* romInit - entry point for VxWorks in ROM
*
* Entry point for VxWorks in ROM
*
* RETURNS: N/A
*/

FUNC_BEGIN(romInit)     /* used by romStart() */
FUNC_LABEL(_romInit)    /* used by linker     */

    /* branch to resetEntry, which will initialize the tlb and return at cold */

    bl      resetEntry

FUNC_LABEL(romWarm)

    bl  warm

    /* copyright notice appears at beginning of ROM (in TEXT segment) */

    .ascii   "Copyright 1984-2010 Wind River Systems, Inc."
    .align 4

cold:

    /* resetEntry transfers execution here once TLB is initialized */

    li      r2, BOOT_COLD
    bl      start           /* skip over next instruction */

warm:
    /*
     * Note: r2 is used to store the reset type code. The data it
     * stores should not be changed until the jump to romStart
     * at the end of romInit. r2 makes a good candidate for storing
     * this information since by PowerPC convention it is reserved
     * for use by the linker as the Small Data Area pointer. Since
     * VxWorks does not make use of this feature, and since the
     * compiler does not generate code to make use of this feature,
     * it can be safely co-opted for this temporary use.
     */

    mr  r2, r3

    /* General MMU initialization */

    addis   r0, r0, HI(MMUCR_INIT)
    ori     r0, r0, LO(MMUCR_INIT)
    mtspr   MMUCR, r0
    isync

    /* initialize UTLB entries */

    bl      romTlbInit

    mfspr   r4, PIR
    cmpwi   r4, 0
    beq     start

    /* calculate CPUn_SPACE */

    rlwinm  r5, r4, CPU_SPACE_SIZE_SHIFT, 0, CPU_SPACE_SIZE_SHIFT - 1
    lis     r4, HI(RAM_LOW_ADRS_CPU0)
    ori     r4, r4, LO(RAM_LOW_ADRS_CPU0)
    subf    r4, r5, r4

    /* running on CPU 1 ~ 3 */

    /* jump to sysInit for AMP - never get here for SMP */

waitForStartSetWarm:
    lwz     r5, CPU_INIT_STATUS_ADR_OFFSET(r4)

    /*
     * Check cpu init status before jumping.
     * This allows cpu 1 ~ 3 to reboot and wait for cpu 0 to reload image.
     */

    cmpwi   r5, ACP34XX_MP_CPU_STATUS_READY
    bne     waitForStartSetWarm

    lwz     r5, CPU_INIT_START_ADR_OFFSET(r4)
    xor     r0, r0, r0
    lwz     sp, CPU_STACK_START_ADR_OFFSET(r4)  /* CPUn_STACK_START_ADR */
    stwu    r0, -FRAMEBASESZ(sp)                /* save one frame stack */

    /*
     * Setup for subsequent change in instruction fetch address
     */

    addis   r0, r0, HI(MSR_VALUE)
    ori     r0, r0, LO(MSR_VALUE)
    mtspr   SRR1, r0    /* Write the new MSR value into SRR1 */
    mtspr   SRR0, r5    /* Write addr from which to cont exec into SRR0 */

    /*
     * Context synchronize to invalidate shadow TLB contents and
     * cause new TLB  contents to take effect. Jump to main boot
     * code.
     */

    rfi

start:

    /* Initalize registers that need to be set to zero. */

    xor     r0, r0, r0
    addi    r1, r0, -1

#ifdef CLEAR_DEBUG_REGISTERS
    /*
     * Clear DBCR0 register (disable all debug events)
     * Reset all other debugging registers associated with DBCR0.
     * Although the PPC476 is defined to reset some of the debug event enables
     * during the reset operation, this is not required by the  architecture
     * and hence the initialization software should not assume this behavior.
     * Software should disable all debug events in order to  prevent
     * non-deterministic behavior on the trace interface to the core.
     */

    mtspr   DBCR0, r0
    mtspr   DBCR1, r0
    mtspr   DBCR2, r0
    mtspr   IAC1, r0
    mtspr   IAC2, r0
    mtspr   IAC3, r0
    mtspr   IAC4, r0
    mtspr   DAC1, r0
    mtspr   DAC2, r0
    mtspr   DVC1, r0
    mtspr   DVC2, r0

    /* Clear DBSR register (initialize all debug event status) */

    addis   r0, r0, HI(DBSR_INIT)
    ori     r0, r0, LO(DBSR_INIT)
    mtspr   DBSR, r0    /* write 1s to clear */
#endif /* CLEAR_DEBUG_REGISTERS */

    /*
     * Initialize timer facilities
     * a. Write DEC to 0 to prevent DEC exceptions
     * b. Write TBL and TBU to 0 to prevent FIT and WDT exceptions
     * c. Clear TSR to clear all timer exception status
     * d. Write TCR to configure and enable timers as desired
     */

    mtspr   DEC, r0
    mtspr   TBL, r0
    mtspr   TBU, r0
    mtspr   TSR, r1
    mtspr   TCR, r0

    /* Clear some misc registers */

    mtspr   ESR, r0                 /* clear Exception Syndrome Reg     */
    mtspr   XER, r0                 /* clear Fixed-Point Exception Reg  */

    /* Initialize the stack pointer (r1) */

    lis     sp, HIADJ(STACK_ADRS)
    addi    sp, sp, LO(STACK_ADRS)

    /* calculate C entry point: routine - entry point + ROM base */

    lis     r4, HIADJ(romStart)         /* r4 = romstart                    */
    addi    r4, r4, LO(romStart)

    lis     r5, HIADJ(romInit)          /* r5 = romInit                     */
    addi    r5, r5, LO(romInit)

    lis     r6, HIADJ(ROM_TEXT_ADRS)    /* r6 = ROM_TEXT_ADRS               */
    addi    r6, r6, LO(ROM_TEXT_ADRS)

    subf    r4, r5, r4              /* r4 = r4 - r5                     */
    add     r4, r4, r6              /* r4 = r4 + r6                     */
    mtlr    r4                      /* link register = C entry point    */

    mr      r3, r2                  /* r3 = startType                   */
    addi    sp, sp, -FRAMEBASESZ    /* get frame stack                  */

    blr             /* branch to link register */
FUNC_END(romInit)

/*******************************************************************************
*
* resetEntry - rom entry point
*
* resetEntry is the first function to be called by the PPC476 core after reset
* It is located at ROM address 0xfffff000. It is responsible for initializing
* key registers to a quiescent state, initializing the the TLB with a static
* memory mapping adequate for running the bootrom, and clearing the rest of the
* TLB. It sets up device access registers so the rest of the SRAM (or Boot
* Flash) can be correctly accessed. Finally, it sets up SRR0 and SRR1 with  a
* safe MSR value and the instruction address of the romInit cold start entry
* point, and 'returns from interrupt' to synchronize processor context and
* continue the boot process in romInit.
*
* Main Initialization Steps:
*      1. Set up and clear the DBCR0 register to disable all debug events.
*      2. Clear the DBSR register to initialize all debug event status.
*      3. Initialize CCR0.
*      4. Configure the memory management unit control registers
*         (MMUBE0, MMUBE1, SSPCR, USPCR,ISPCR) as appropriate.
*      5. Set TLB entries to at least cover initial program memory page
*      6. Call rfi to cause a context synchronization and jump to romInit
*/

#if   defined(_GNU_TOOL)
    .section .boot, "ax", @progbits
#elif defined(_DIAB_TOOL)
    .section .boot, 4, "rx"
#else
    #error "_GNU_TOOL or _DIAB_TOOL should be defined."
#endif

FUNC_BEGIN(resetEntry)

    /* Set r0 = 0 */

    xor     r0, r0, r0
    mtmsr   r0

    /* the core will jump to address 0x0 and then go here */

    mfspr   r9, PIR
    cmpwi   r9, 0
    beq     init

otherCores:

    /* calculate CPUn_SPACE */

    rlwinm  r10, r9, CPU_SPACE_SIZE_SHIFT, 0, CPU_SPACE_SIZE_SHIFT - 1
    lis     r3, HI(RAM_LOW_ADRS_CPU0)
    ori     r3, r3, LO(RAM_LOW_ADRS_CPU0)
    subf    r10, r10, r3

    /* inform core 0 by CPU status */

    li      r5, ACP34XX_MP_CPU_STATUS_BOOTING
    stw     r5, CPU_INIT_STATUS_ADR_OFFSET(r10)
    dcbf    0, r10
    msync
    isync

    /* wait here until core 0 release the core */

waitCore0:
    lwz     r5, CPU_INIT_STATUS_ADR_OFFSET(r10)
    cmpwi   r5, ACP34XX_MP_CPU_STATUS_REBOOT
    bne     waitCore0

init:

    /* Clear SRR0, CSRR0, SRR1, CSRR1 */

    mtmsr   r0
    mtspr   SRR0, r0
    mtspr   SRR1, r0
    mtspr   CSRR0, r0
    mtspr   CSRR1, r0

#ifdef CLEAR_DEBUG_REGISTERS
    /*
     * Clear DBCR0 register (disable all debug events)
     * Reset all other debugging registers associated with DBCR0.
     * Although the PPC476 is defined to reset some of the debug event enables
     * during the reset operation, this is not required by the  architecture
     * and hence the initialization software should not assume this behavior.
     * Software should disable all debug events in order to  prevent
     * non-deterministic behavior on the trace interface to the core.
     */

    mtspr   DBCR0, r0
    mtspr   DBCR1, r0
    mtspr   DBCR2, r0
    mtspr   IAC1, r0
    mtspr   IAC2, r0
    mtspr   IAC3, r0
    mtspr   IAC4, r0
    mtspr   DAC1, r0
    mtspr   DAC2, r0
    mtspr   DVC1, r0
    mtspr   DVC2, r0

    /* Clear DBSR register (initialize all debug event status) */

    addis   r0, r0, HI(DBSR_INIT)
    ori     r0, r0, LO(DBSR_INIT)
    mtspr   DBSR, r0    /* write 1s to clear */
#endif /* CLEAR_DEBUG_REGISTERS */

    /*
     * set up CCR registers
     *
     * icbi request size must be set to 128 bytes because dcbX instructions
     * work on L2 cache with 128 bytes.
     */

    lis    r0, _PPC_CCR0_ICS_U
    mtspr   CCR0, r0
    isync

    /* Reset the timebase. */

    xor     r0, r0, r0
    mtspr   TBL, r0
    mtspr   TBU, r0

    /* Priority registers configuration */

    lis     r0, HI(PRIORITY_ORDER_INIT)
    ori     r0, r0, LO(PRIORITY_ORDER_INIT)
    mtspr   SSPCR, r0
    mtspr   USPCR, r0
    lis     r0, HI(PRIORITY_ORDER_INIT_ISPCR)
    ori     r0, r0, LO(PRIORITY_ORDER_INIT_ISPCR)
    mtspr   ISPCR, r0

    /* General MMU initialization */

    addis   r0, r0, HI(MMUCR_INIT)
    ori     r0, r0, LO(MMUCR_INIT)
    mtspr   MMUCR, r0
    isync

    /*
     * For PPC476 REV2, software needs to implement the below Errata workaround.
     * For PPC476 REV2.1, the below Errata has been fixed by hardware itself.
     * So here read the PVR value to distinguish silicon version.
     */

    mfspr   r5, PVR
    rlwinm  r6, r5, PPC476_PVR_CORE_REV_LSB + 1, 24, 31
    cmpwi   r6, PPC476_PVR_CORE_REV_0
    bgt     noErrataFix

    /*
     * PPC476 REV2 Erratum 38: false I-Cache data parity errors might cause
     * machine check exceptions incorrect data.
     */

    /*  I-Cache initialization as recommended by IBM */

    /* need one cached region with EA = 0x40000000 and RA = 0x000f0000 */

    lis     r1, HI(MMUTLB_WRITE_RA_SWAY)
    TLB_WORD_SET(0, r1, r0, 0x40000000 | _MMU_TLB_V | _MMU_TLB_SZ_64K)
    TLB_WORD_SET(1, r1, r0, 0x000f0000)
    TLB_WORD_SET(2, r1, r0, _MMU_TLB_PERM_X | _MMU_TLB_PERM_W | _MMU_TLB_PERM_R)
    isync

    /* set up initialization code at 0x40000000 */

    lis     r3, 0x4000
    mtlr    r3
    li      r4, L1ICACHE_LINES - 1
    mtctr   r4

    /* fill up (L1ICACHE_LINES - 1) "b . + 32" instructions */

    addis   r4, 0, HI(L1ICACHE_INIT_B_NEXT_LINE_INST)
    ori     r4, r4, LO(L1ICACHE_INIT_B_NEXT_LINE_INST)

    addi    r3, r3, -L1ICACHE_LINE_SIZE
1:
    stwu    r4, L1ICACHE_LINE_SIZE(r3)
    bdnz    1b

    /* set up one "blr" instruction as the end */

    addis   r4, 0, HI(L1ICACHE_INIT_BLR_INST)
    ori     r4, r4, LO(L1ICACHE_INIT_BLR_INST)
    stwu    r4, L1ICACHE_LINE_SIZE(r3)

    /* execute the piece of code to initialize I-Cache */

    blrl

    /* remove temporary TLB entry */

    lis     r1, HI(MMUTLB_WRITE_RA_SWAY)
    TLB_WORD_SET(0, r1, r0, 0x40000000 | 0 | 0)
    TLB_WORD_SET(1, r1, r0, 0)
    TLB_WORD_SET(2, r1, r0, 0)
    isync

    /* invalidate the entire I-Cache */

    ici     0
    isync

noErrataFix:
    /* initialize UTLB entries */

    bl      romTlbInit

    /*
     * Setup for subsequent change in instruction fetch address
     */

    lis     r3, HI(MSR_VALUE)
    ori     r3, r3, LO(MSR_VALUE)
    mtspr   SRR1, r3          /* Write the new MSR value into SRR1 */

    /* PIR has been saved in r9, and CPUn_SPACE has been saved in r10 */

    cmpwi   r9, 0
    beq     skipLoop

    /* jump to sysCpu1Loop for SMP - sysInit for AMP */

waitForStartSet:
    lwz     r8, CPU_INIT_STATUS_ADR_OFFSET(r10)

    /*
     * Check cpu init status before jumping.
     * This allows cpu 1 ~ 3 to reboot and wait for cpu 0 to reload image.
     */

    cmpwi   r8, ACP34XX_MP_CPU_STATUS_READY
    bne     waitForStartSet

    lwz     r8, CPU_INIT_START_ADR_OFFSET(r10)
    xor     r0, r0, r0
    lwz     sp, CPU_STACK_START_ADR_OFFSET(r10)  /* CPUn_STACK_START_ADR */
    stwu    r0, -FRAMEBASESZ(sp)                /* save one frame stack */
    mtspr   SRR0, r8    /* Write addr from which to cont exec into SRR0 */

    /*
     * Context synchronize to invalidate shadow TLB contents and
     * cause new TLB  contents to take effect. Jump to main boot
     * code.
     */

    rfi

skipLoop:

    /* Set the PLB6 hang pulse count, the default value 0x1000 would be set */

    li      r5, PLB6BC_HCPP_REG_ADDR
    li      r4, PLB6BC_HCPP_REG_VALUE
    mtdcrx  r5, r4

    /* clear staus registers */

    li      r5, PLB6BC_BCHD_REG_ADDR
    lis     r4, HI(PLB6BC_BCHD_REG_VALUE)
    ori     r4, r4, LO(PLB6BC_BCHD_REG_VALUE)
    mtdcrx  r5, r4

    li      r5, PLB6BC_BCQD_REG_ADDR
    lis     r4, HI(PLB6BC_BCQD_REG_VALUE)
    ori     r4, r4, LO(PLB6BC_BCQD_REG_VALUE)
    mtdcrx  r5, r4

    li      r5, PLB6BC_BCSHD_REG_ADDR
    lis     r4, HI(PLB6BC_BCSHD_REG_VALUE)
    ori     r4, r4, LO(PLB6BC_BCSHD_REG_VALUE)
    mtdcrx  r5, r4

    /*
     * set up the clock count after the combined response until PLB6 removes an
    * entry from the PAAM buffer.
    */

    li      r5, PLB6BC_PAAM_REG_ADDR
    lis     r4, HI(PLB6BC_PAAM_REG_VALUE)
    ori     r4, r4, LO(PLB6BC_PAAM_REG_VALUE)
    mtdcrx  r5, r4

    /* calculate C entry point: routine - entry point + ROM base */

    lis     r4, HIADJ(cold)     /* r4 = cold */
    addi    r4, r4, LO(cold)

    lis     r5, HIADJ(romInit)  /* r5 = romInit */
    addi    r5, r5, LO(romInit)

    lis     r6, HIADJ(ROM_TEXT_ADRS) /* r6 = ROM_TEXT_ADRS */
    addi    r6, r6, LO(ROM_TEXT_ADRS)

    subf    r4, r5, r4      /* r4 = r4 - r5 */
    add     r4, r4, r6      /* r4 = r4 + r6 */

    mtspr   SRR0, r4      /* Write addr from which to cont exec into SRR0 */

    /*
     * Context synchronize to invalidate shadow TLB contents and
     * cause new TLB  contents to take effect. Jump to main boot
     * code.
     */

    rfi
FUNC_END(resetEntry)

/*******************************************************************************
*
* romTlbInit - initialize UTLB entires for bootrom
*
* This routine initializes UTLB entires for bootrom.
*
* RETURNS: N/A
*/

FUNC_BEGIN(romTlbInit)

    /* Just using 4k size, to get the hash table and clear all the tlb entry */

    addis   r3, 0, HI(MMUTLB_WRITE_RA_SWAY)
    addi    r4, 0, 0
    addi    r5, 0, 0
    b       invalid_utlb_entry

    /* align to speed up */

    .align  _CPU_CACHE_ALIGN_SHIFT

invalid_utlb_entry:

    tlbwe   r4, r3, 0
    tlbwe   r5, r3, 1
    tlbwe   r5, r3, 2

    addis   r3, r3, HI(MMUTLB_WRITE_RA_WAY1)
    cmpwi   r3, 0
    bne     invalid_utlb_entry
    addis   r3, 0, HI(MMUTLB_WRITE_RA_SWAY)
    addis   r4, r4, 0x100
    cmpwi   r4, 0
    bne     invalid_utlb_entry

    /*
     * Write TLB entry for initial program memory page
     *
     * - Specify EPN, RPN, ERPN, and SIZE as appropriate for system
     * - Set valid bit
     * - Specify TID=0
     * - Specify TS=0 or else MSR[IS,DS] must be set to correspond to TS=1
     * - Specify storage attributes (W, I, M, G, E, U0 - U3) as appropriate
     * - Enable supervisor mode fetch, read, and write access (SX, SR, SW)
     */

    /*
     * 0 : DDR SDRAM 0 - L2 cached and L1 cache inhibited.
     *
     * When cached memory is configured as memory coherent, PLB 6 will take care
     * of coherency for the other master, FEMAC, PCIe, other cores, etc. Note,
     * PLB 6 only interfaces to L2 cache. So L1 cache must be disabled to
     * emulate cache disabled mode for boot loader.
     */

    lis     r1, HI(MMUTLB_BOLT_ENTRYn (0))
    TLB_WORD_SET(0, r1, r0, DRAM_BASE_ADRS | _MMU_TLB_V | _MMU_TLB_SZ_1G)
    TLB_WORD_SET(1, r1, r0, DRAM_BASE_ADRS_PHYS_HI | DRAM_BASE_ADRS_PHYS_LO)
    TLB_WORD_SET(2, r1, r0, _MMU_TLB_ATTR_IL1D | _MMU_TLB_ATTR_IL1I | _MMU_TLB_ATTR_M | _MMU_TLB_PERM_X | _MMU_TLB_PERM_W | _MMU_TLB_PERM_R)

    /* 1-2 : Peripherals (now must be set to 2 entries) */

    lis     r1, HI(MMUTLB_BOLT_ENTRYn (1))
    TLB_WORD_SET(0, r1, r0, PER_BASE_ADRS | _MMU_TLB_V | _MMU_TLB_SZ_1M)
    TLB_WORD_SET(1, r1, r0, PER_BASE_ADRS_PHYS_HI | PER_BASE_ADRS_PHYS_LO)
    TLB_WORD_SET(2, r1, r0, _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W | _MMU_TLB_PERM_R)

    lis     r1, HI(MMUTLB_BOLT_ENTRYn (2))
    TLB_WORD_SET(0, r1, r0, (PER_BASE_ADRS + PER_SIZE_TLB) | _MMU_TLB_V | _MMU_TLB_SZ_1M)
    TLB_WORD_SET(1, r1, r0, PER_BASE_ADRS_PHYS_HI | (PER_BASE_ADRS_PHYS_LO + PER_SIZE_TLB))
    TLB_WORD_SET(2, r1, r0, _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W | _MMU_TLB_PERM_R)

    /* 3 : PCIe */

#ifdef DRV_PCIBUS_LSI_PCIEX
    lis     r1, HI(MMUTLB_BOLT_ENTRYn (3))
    TLB_WORD_SET(0, r1, r0, PCIE0_CFG_ADRS | _MMU_TLB_V | _MMU_TLB_SZ_1G)
    TLB_WORD_SET(1, r1, r0, PCIE_BASE_ADRS_PHYS_HI | PCIE_BASE_ADRS_PHYS_LO)
    TLB_WORD_SET(2, r1, r0, _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W | _MMU_TLB_PERM_R)
#endif /* DRV_PCIBUS_LSI_PCIEX */

    /*  Initialize PID to match TID field of TLB entry (unless using TID=0) */

    xor     r0, r0, r0
    mtspr   PID, r0
    isync

    blr
FUNC_END(romTlbInit)

