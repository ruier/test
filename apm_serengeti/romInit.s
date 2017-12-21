/* romInit.s - ROM bootcode for APM Serengeti board */

/*
 * Copyright (c) 2011, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
01g,02mar12,x_s  added a workaround for warm boot with DDR as cache inhibit.
01f,26dec11,x_s  added register config for warm boot.
01e,07sep11,x_s  added SMP support.
01d,29aug11,x_s  added L2 cache support.
01c,06aug11,syt  enable cache on NOR Flash and DDR SRAM in boot
                 stage, add DCR access functions.
01b,18jul11,b_m  only release OCM reset before jumping to SRAM.
01a,05oct09,syt  initial creation.
*/

#define _ASMLANGUAGE
#include <vxWorks.h>
#include <sysLib.h>
#include <asm.h>
#include "config.h"
#include <regs.h>
#include <arch/ppc/excPpcLib.h>
#include <arch/ppc/mmu440Lib.h>
#include <arch/ppc/ppc440.h>

/* defines */

/*
 * Location of temporary stack in on-chip SRAM, for use after SRAM
 * access is available but before SDRAM controller is configured.
 */

#define ROM_STACK_ADRS  (APM_OCM_BASE + APM_OCM_SIZE - 16)

/*
 * DBSR register initialization
 *
 * This is a write-1s-to-clear register. The value below clears all
 * debug event status bits but leaves the Most Recent Reset status bits
 * untouched.
 */

#define DBSR_INIT   (_DBSR_IDE   | _DBSR_UDE   | _DBSR_ICMP  | _DBSR_BRT   | \
                     _DBSR_IRPT  | _DBSR_TRAP  | _DBSR_IAC1  | _DBSR_IAC2  | \
                     _DBSR_IAC3  | _DBSR_IAC4  | _DBSR_DAC1R | _DBSR_DAC1W | \
                     _DBSR_DAC2R | _DBSR_DAC2W | _DBSR_RET   | \
                     _DBSR_IAC12ATS | _DBSR_IAC34ATS)

/*
 * CCR0 register initialization (for resetEntry)
 *
 * a. Disable store gathering
 * b. Enable broadcast of instructions to auxilary processors
 * c. Enable broadcast of trace information
 * d. Icbt is guaranteed to fill a cache line even during a stall
 * e. Dcbt is guaranteed to fill a cache line even during a stall
 * f. Disable exceptions for misaligned data accesses
 * g. Instruction cache speculative line count = 0
 * h. Instruction cache speculative line threshold = 0
 */

#define CCR0_INIT   (((_PPC_CCR0_DSTG_U | _PPC_CCR0_DAPUIB_U) << 16) | \
                       _PPC_CCR0_GICBT  | _PPC_CCR0_GDCBT)

/*
 * CCR1 register initialization
 *
 * a. ICDPEI: Record even parity. Normal operation.
 * b. ICTPEI: Record even parity. Normal operation.
 * c. DCTPEI: Record even parity. Normal operation.
 * d. DCDPEI: Record even parity. Normal operation.
 * e. DCUPEI: Record even parity. Normal operation.
 * f. DCMPEI: Record even parity. Normal operation.
 * g. FCOM:   Normal operation
 * h. MMUPEI: Record even parity. Normal operation.
 * i. FFF:    Flush only as much data as necessary.
 * j. TCS:    Timebase increments from externally supplied clock
 */

#define CCR1_INIT   (0)

/*
 * MSR register initialization
 *
 * This initial MSR value allows machine check and critical class
 * exceptions.
 */

#define MSR_VALUE   (0)

/*
 * IVLIM, DVLIM register initialization
 *
 * Sets the Transient & Normal ceiling and floor such that all cache
 * lines are for normal storage.  MMU library supports only normal
 * cache lines.  Note that DVLIM is also currently initialized to the
 * same value as IVLIM.
 */

#define IVLIM_INIT  0x0001f800

/*
 * MMUCR register initialization
 *
 * Specify TID field to be written to TLB entries.  TID = 0
 * Specify TS field to be used for TLB searches. TS = 0
 * Specify store miss alloc behavior.  On a cache miss, allocate a
 *     line
 * Disable transient cache mechanism
 * Disable cache locking exceptions
 */

#define MMUCR_INIT  0x00000000

/* TLB setup defines and functions */

#define TLB_V       0x00000200  /* TLB Word 0, Valid Bit */

/*
 * The TLB_NEXT and TLB_WORD_SET macros are used to set up each TLB
 * entry, which consists of three words. Using macros reduces the
 * opportunity for error.
 */

#define TLB_NEXT(ixReg) \
            addi  ixReg, ixReg, 0x0001 /* Increment to the next TLB entry */

#define TLB_WORD_SET(wordNum, ixReg, tempReg, value) \
            lis   tempReg, HI(value); \
            ori   tempReg, tempReg, LO(value); \
            tlbwe tempReg, ixReg, wordNum;

    FUNC_EXPORT(romRead32)
    FUNC_EXPORT(romWrite32)
    FUNC_EXPORT(romDcrOutLong)
    FUNC_EXPORT(romDcrInLong)
    FUNC_EXPORT(romInit)
    FUNC_EXPORT(_romInit)

    /* externals */

    FUNC_IMPORT(romStart)
    FUNC_IMPORT(romBoardEarlyInit)

    _WRS_TEXT_SEG_START

/*******************************************************************************
*
* romInit - entry point for VxWorks in ROM
*
*   romInit
*   (
*   int startType   /@ only used by sysToMonitor @/
*   )
*/

FUNC_BEGIN(romInit)     /* used by romStart() */
FUNC_BEGIN(_romInit)    /* used by linker */

    /*
     * branch to resetEntry, which will initialize the tlb and
     * return at cold
     */

    b       resetEntry

romWarm:

    b       warm

    /* Useful to have recognizeable string */

    .ascii  "Wind River VxWorks bootrom"

    .balign 4

cold:
    /* resetEntry transfers execution here once TLB is initialized */

    li      r2, BOOT_COLD
    b       start           /* skip over next instruction */

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

    mr      r2, r3          /* startType to r2 */

start:
    /*
     * Disable external interrupts by turning off CE and EE bits in MSR
     * Also turn off Machine Check until its vector is set up.
     */

    mfmsr   r3                  /* r3 = msr */
    INT_MASK (r3, r4)           /* mask EE and CE bit */
    rlwinm  r4, r4, 0, 20, 18   /* turn off _PPC_MSR_ME */
    mtmsr   r4                  /* msr = r4 */
    isync

    /* Clear SRR0, CSRR0, SRR1, CSRR1, MCSRR0, MCSRR1, MCSR, ESR */

    xor     r0, r0, r0          /* r0 = 0 */
    mtspr   SRR0, r0
    mtspr   SRR1, r0
    mtspr   CSRR0, r0
    mtspr   CSRR1, r0
    mtspr   MCSRR0, r0
    mtspr   MCSRR1, r0
    mtspr   ESR, r0

    mfspr   r1, MCSR
    mtspr   MCSR, r1

    li      r3, BOOT_COLD
    cmplw   r3, r2
    beq     skipWarmInit

#ifdef CLEAR_DEBUG_REGISTERS

    /*
     * Clear DBCR0 register (disable all debug events)
     * Reset all other debugging registers associated with DBCR0.
     * Although the 465 is defined to reset some of the debug
     * event  enables during the reset operation, this is not
     * required by the  architecture and hence the initialization
     * software should not assume  this behavior. Software should
     * disable all debug events in order to  prevent
     * non-deterministic behavior on the trace interface to the
     * core.
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

    /* Clear DBSR register, write 1s to clear */

    addis   r0, r0, HI(DBSR_INIT)
    ori     r0, r0, LO(DBSR_INIT)
    mtspr   DBSR, r0

#endif /* CLEAR_DEBUG_REGISTERS */

    /* Initialize general load/store and cache operations */

    addis   r0, r0, HI(CCR0_INIT)
    ori     r0, r0, LO(CCR0_INIT)
    mtspr   CCR0, r0

    addis   r0, r0, HI(CCR1_INIT)
    ori     r0, r0, LO(CCR1_INIT)
    mtspr   CCR1, r0

    /*
     * Reset the timebase
     * The previous write to CCR1 sets the timebase source
     */

    xor     r0, r0, r0
    mtspr   TBL, r0
    mtspr   TBU, r0

    /*
     * Configure instruction and data cache regions
     * These steps must be performed prior to enabling the caches
     * a. Clear the instruction and data cache normal victim index
     *    registers
     * b. Clear the instruction and data cache transient victim index
     *    regs
     * c. Set the instruction and data cache victim limit registers
     */

    mtspr   INV0, r0
    mtspr   INV1, r0
    mtspr   INV2, r0
    mtspr   INV3, r0
    mtspr   DNV0, r0
    mtspr   DNV1, r0
    mtspr   DNV2, r0
    mtspr   DNV3, r0
    mtspr   ITV0, r0
    mtspr   ITV1, r0
    mtspr   ITV2, r0
    mtspr   ITV3, r0
    mtspr   DTV0, r0
    mtspr   DTV1, r0
    mtspr   DTV2, r0
    mtspr   DTV3, r0
    addis   r0, r0, HI(IVLIM_INIT)
    ori     r0, r0, LO(IVLIM_INIT)
    mtspr   IVLIM, r0
    mtspr   DVLIM, r0     /* note: DVLIM configured same as IVLIM */

    /* Invalidate the L1 Instruction and Data cache */

    iccci   r0, r0
    dccci   r0, r0
    msync

    /* initialize TLB entries */   

    bl      romTlbInit

    /* 
     * Workaround: re-write TLB1 #1: Main DDR RAM as cache inhibit.
     * Since unknown hardware silicon issue, if Main DDR RAM as cache 
     * opened for warm boot, the system would be unstabitily. 
     */

    li      r1, 1
    TLB_WORD_SET(0, r1, r0, PHY_MEM_BASE | TLB_V | _MMU_TLB_SZ_1G)
    TLB_WORD_SET(1, r1, r0, PHY_MEM_BASE | PHY_MEM_BASE_HI)
    TLB_WORD_SET(2, r1, r0, _MMU_TLB_ATTR_I | MMU_STATE_SUP_RWX_USR_RWX)
    isync
       
    mfspr   r4, PIR
    cmpwi   r4, 0
    beq     skipWarmInit

    /* calculate CPUn_SPACE */

    rlwinm  r5, r4, CPU_SPACE_SIZE_SHIFT, 0, CPU_SPACE_SIZE_SHIFT - 1
    lis     r4, HI(RAM_LOW_ADRS_CPU0)
    ori     r4, r4, LO(RAM_LOW_ADRS_CPU0)
    subf    r4, r5, r4

    /* jump to sysInit for AMP Core 1 or SMP */

waitForStartSetWarm:
    lwz     r5, CPU_INIT_STATUS_ADR_OFFSET(r4)

    /*
     * Check cpu init status before jumping.
     * This allows cpu 1 to reboot and wait for cpu 0 to reload image.
     */

    cmpwi   r5, SERENGETI_MP_CPU_STATUS_READY
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

skipWarmInit:

    /* Initialize the stack pointer (r1) */

    lis     sp, HIADJ(STACK_ADRS)
    addi    sp, sp, LO(STACK_ADRS)

    /* small data area support would go here if it were supported */

    /* calculate C entry point: routine - entry point + ROM base */

    lis     r4, HIADJ(romStart)     /* r4 = romstart */
    addi    r4, r4, LO(romStart)

    lis     r5, HIADJ(romInit)      /* r5 = romInit */
    addi    r5, r5, LO(romInit)

    lis     r6, HIADJ(ROM_TEXT_ADRS)    /* r6 = ROM_TEXT_ADRS */
    addi    r6, r6, LO(ROM_TEXT_ADRS)

    subf    r4, r5, r4              /* r4 = r4 - r5 */
    add     r4, r4, r6              /* r4 = r4 + r6 */
    mtlr    r4                      /* link register = C entry point */

    mr      r3, r2                  /* r3 = startType */
    addi    sp, sp, -FRAMEBASESZ    /* get frame stack */

    blr                             /* branch to link register */

FUNC_END(romInit)

/*******************************************************************************
*
* l2CacheInvalidate - Invalidate the L2 Cache Congruence Class
*
* This routine invalidates the L2 Cache Congruence Class
*
* void l2CacheInvalidate (void)
*
* RETURNS: N/A
*
* ERRNO: N/A
*
*/

FUNC_BEGIN(l2CacheInvalidate)

    /*
     * invalidate L2 cache
     * before invalidate L2 cahce, need to check DDR related configuration.
     */

    lis     r3, HI(SCU_BASE_ADDR)
    ori     r3, r3, LO(SCU_BASE_ADDR)

    /* check register SCU_SRST[17] MEMC */

    lwz     r4, 0xc(r3)
    andi.   r5, r4, 0x4000

    beq     shipDdrConfig

    /* set MEMC&DDR_PHY for SCU_SRST */

    xori    r4, r4, 0x5000
    stw     r4, 0xc(r3)

    /* read register SCU_CLKEN */

    lwz     r4, 0x10(r3)

    /* set register SCU_CLKEN bits MEMC_AXI&MEMC_PLB&MEMC_DDR */

    ori     r4, r4, 0x7000
    stw     r4, 0x10(r3)

    /* set register SCU_CSR_SRST[17] MEMC */

    lwz     r4, 0xe0(r3)
    ori     r4, r4, 0x4000
    stw     r4, 0xe0(r3)

shipDdrConfig:

    lis     r6, HI(L2CR0_TAI)
    ori     r6, r6, LO(L2CR0_TAI)
    li      r3, L2CR0
    mtdcr   L2DCDCRAI, r3

    lis     r4, HI(L2CR0_AS_256K | 3 << L2CR0_TAA_SHIFT | 3 << L2CR0_DAA_SHIFT )
    ori     r4, r4, LO(L2CR0_TAI)

    mtdcr   L2DCDCRDI, r4
    isync

l2InvalidCheck:
    mfdcr   r4, L2DCDCRDI
    and     r4, r4, r6
    cmpwi   r4, 0
    bne     l2InvalidCheck

    blr
FUNC_END(l2CacheInvalidate)

/*******************************************************************************
*
* l2CacheDisable - disable L2 Cache
*
* This routine disables PPC465's L2 Cache arrays
*
* RETURNS: N/A
*
*/

FUNC_BEGIN(l2CacheDisable)

    /* Disable L2 cache if L2 cache enabled */

    li      r3, L2CR0
    mtdcr   L2DCDCRAI, r3
    mfdcr   r4, L2DCDCRDI
    lis     r3, HI(L2CR0_AS_256K)
    ori     r3, r3, LO(L2CR0_AS_256K)
    and     r4, r4, r3
    cmpw    r4, r3
    beq     disableL2Cache
    blr

    /* Disable L2 Array */
disableL2Cache:
    li      r3, L2CR0
    mtdcr   L2DCDCRAI, r3
    mfdcr   r4, L2DCDCRDI
    lis     r3, HI(L2CR0_AS_MASK)
    ori     r3, r3, LO(L2CR0_AS_MASK)
    andc    r4, r4, r3
    mtdcr   L2DCDCRDI, r4

    blr
FUNC_END(l2CacheDisable)

/*******************************************************************************
*
* l2CacheEnable - enable L2 Cache
*
* This routine enables PPC465's L2 Cache arrays
*
* RETURNS: N/A
*
*/

FUNC_BEGIN(l2CacheEnable)

    li      r3, L2ERAPR
    mtdcr   L2DCDCRAI, r3
    li      r4, 0
    mtdcr   L2DCDCRDI, r4

    li      r3, L2SLVERAPR
    mtdcr   L2DCDCRAI, r3
    li      r4, 0
    mtdcr   L2DCDCRDI, r4
    
    li      r3, L2CR0
    mtdcr   L2DCDCRAI, r3
    li      r4, 0
    lis     r4, HI(L2CR0_AS_256K | 1 << L2CR0_TAA_SHIFT | 1 << L2CR0_DAA_SHIFT)
    ori     r4, r4, 0
    
    mtdcr   L2DCDCRDI, r4

    li      r3, L2CR1
    mtdcr   L2DCDCRAI, r3
    li      r4, 0
    mtdcr   L2DCDCRDI, r4

    li      r3, L2CR2
    mtdcr   L2DCDCRAI, r3
    li      r4, 0
    lis     r4, HI(L2CR2_DEFAULT_VAL)
    ori     r4, r4, LO(L2CR2_DEFAULT_VAL)
    mtdcr   L2DCDCRDI, r4

    li      r3, L2CR3
    mtdcr   L2DCDCRAI, r3
    li      r4, 0
    lis     r4, HI(L2CR3_MSTPD)
    mtdcr   L2DCDCRDI, r4

    li      r3, L2FAMAR
    mtdcr   L2DCDCRAI, r3
    li      r4, 0
    mtdcr   L2DCDCRDI, r4

    li      r3, L2FER0
    mtdcr   L2DCDCRAI, r3
    li      r4, 0
    mtdcr   L2DCDCRDI, r4

    li      r3, L2FER1
    mtdcr   L2DCDCRAI, r3
    li      r4, 0
    mtdcr   L2DCDCRDI, r4

    li      r3, L2MCSR
    mtdcr   L2DCDCRAI, r3
    li      r4, -1
    mtdcr   L2DCDCRDI, r4

    li      r3, L2MCRER
    mtdcr   L2DCDCRAI, r3
    li      r4, -1
    mtdcr   L2DCDCRDI, r4

    li      r3, L2DBCR
    mtdcr   L2DCDCRAI, r3
    li      r4, 0
    mtdcr   L2DCDCRDI, r4

    li      r3, L2DBSR
    mtdcr   L2DCDCRAI, r3
    li      r3, 0
    mtdcr   L2DCDCRDI, r4

    li      r3, L2SLVAC0
    mtdcr   L2DCDCRAI, r3
    li      r4, 0
    mtdcr   L2DCDCRDI, r4

    li      r3, L2SNPAC0
    mtdcr   L2DCDCRAI, r3
    li      r4, 0
    mtdcr   L2DCDCRDI, r4

    li      r3, L2SNPAC1
    mtdcr   L2DCDCRAI, r3
    li      r4, 0
    mtdcr   L2DCDCRDI, r4
    
    mfspr   r3, CCR1
    ori     r3, r3, _PPC_CCR1_L2COBE
    mtspr   CCR1, r3
    
    isync

    blr
FUNC_END(l2CacheEnable)

/*******************************************************************************
*
* romRead32 - read a 32 bit register
*
* This function reads a 32 bit register at <offset> and returns the value.
* The <offset> could be a memory space or IO space address.
*
* UINT32 romRead32 (UINT32 * address)
*
* RETURNS: the value read from the register
*
* ERRNO: N/A
*/

FUNC_BEGIN(romRead32)
    lwzx    r3, 0, r3   /* Read 32 bits from an address */
    eieio               /* Sync I/O operation */
    blr                 /* Return to caller */
FUNC_END(romRead32)

/*******************************************************************************
*
* romWrite32 - write a 32 bit register
*
* This function writes a value to a 32-bit register at <address>.
* The <address> could be a memory space or IO space address.
*
* void romWrite32 (UINT32 * address, UINT32 value)
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

FUNC_BEGIN(romWrite32)
    stwx    r4, 0, r3   /* Write 32 bits to an address */
    eieio               /* Sync I/O operation */
    blr                 /* Return to caller */
FUNC_END(romWrite32)

/*****************************************************************************
*
* romDcrInLong - read from an aribtrary Device Control Register
*
* Read from an aribtrary Device Control Register
*
*
* ARGUMENTS:
*       r3 = dcr to read
*
* RETURNS:
*       r3 = dcr value read 
*
* UINT32 romDcrInLong(UINT32 dcr)
*       r3 = dcr
*
*/

FUNC_BEGIN(romDcrInLong)
    .long   0x7c630206
    isync
    blr
FUNC_END(romDcrInLong)

/*****************************************************************************
*
* romDcrOutLong - write to an aribtrary Device Control Register
*
* Write to an aribtrary Device Control Register
*
* ARGUMENTS:
*       r3 = dcr
*       r4 = value
*
* RETURNS:
*       none
*
* void romDcrOutLong(UINT32 dcr, UINT32 value)
*       r3 = dcr
*       r4 = value
*/

FUNC_BEGIN(romDcrOutLong)
    .long   0x7c830306
    isync
    blr
FUNC_END(romDcrOutLong)

/*******************************************************************************
*
* resetEntry - rom entry point
*
* resetEntry is the first function to be called by the PPC465 core after
* reset. It is located at ROM address 0xfffff000. It is responsible for
* initializing key registers to a quiescent state, initializing the TLB
* with a static memory mapping adequate for running the bootrom, and
* clearing the rest of the TLB. It sets up device access registers so
* the rest of the SRAM (or Boot Flash) can be correctly accessed.
* Finally, it sets up SRR0 and SRR1 with a safe MSR value and the
* instruction address of the romInit cold start entry point, and
* 'returns from interrupt' to synchronize processor context and continue
* the boot process in romInit.
*
* Main Initialization Steps:
*      1. Invalidate the instruction cache (iccci)
*      2. Invalidate the data cache (dccci)
*      3. Synchronize memory accesses (msync)
*      4. Clear DBCR0 register (disable all debug events)
*      5. Clear DBSR register (initialize all debug event status)
*      6. Initialize CCR0 register
*      7. Configure instruction and data cache regions
*      8. Set TLB entries to at least cover initial program memory page
*      9. Call rfi to cause a context synchronization and jump to romInit
*
* IMPORTANT NOTE: At chip reset, the TLB contents are unknown and there
*   will be only one entry in the ITLB to cover a region from
*   0xFFFFF000 to 0xFFFFFFFF. Therefore the resetEntry routine must
*   be  positioned to fall within this region.
*
*   Since the PPC465 only initializes an architecturally-invisible
*   shadow  TLB entry during the reset operation, and since all
*   shadow TLB entries  are invalidated upon any context
*   synchronization, special care must be  taken during the
*   initialization sequence to prevent any such context
*   synchronizing operations (such as interrupts and the isync
*   instruction)  until after this step is completed, and an
*   architected TLB entry has  been established in the TLB.
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

    /* Clear the ESR */

    xor     r0, r0, r0
    mtspr   ESR, r0

    /* Invalidate the L1 Instruction and Data cache */

    iccci   r0, r0
    dccci   r0, r0
    msync

    /* Clear SRR0, CSRR0, SRR1, CSRR1, MCSRR0, MCSRR1, MCSR */

    xor     r0, r0, r0          /* r0 = 0 */
    mtspr   SRR0, r0
    mtspr   SRR1, r0
    mtspr   CSRR0, r0
    mtspr   CSRR1, r0
    mtspr   MCSRR0, r0
    mtspr   MCSRR1, r0

    mfspr   r1, MCSR
    mtspr   MCSR, r1

#ifdef CLEAR_DEBUG_REGISTERS

    /*
     * Clear DBCR0 register (disable all debug events)
     * Reset all other debugging registers associated with DBCR0.
     * Although the 465 is defined to reset some of the debug
     * event  enables during the reset operation, this is not
     * required by the  architecture and hence the initialization
     * software should not assume  this behavior. Software should
     * disable all debug events in order to  prevent
     * non-deterministic behavior on the trace interface to the
     * core.
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

    /* Clear DBSR register, write 1s to clear */

    addis   r0, r0, HI(DBSR_INIT)
    ori     r0, r0, LO(DBSR_INIT)
    mtspr   DBSR, r0

#endif /* CLEAR_DEBUG_REGISTERS */

    /* Initialize general load/store and cache operations */

    addis   r0, r0, HI(CCR0_INIT)
    ori     r0, r0, LO(CCR0_INIT)
    mtspr   CCR0, r0

    addis   r0, r0, HI(CCR1_INIT)
    ori     r0, r0, LO(CCR1_INIT)
    mtspr   CCR1, r0

    /*
     * Reset the timebase
     * The previous write to CCR1 sets the timebase source
     */

    xor     r0, r0, r0
    mtspr   TBL, r0
    mtspr   TBU, r0

    /*
     * Configure instruction and data cache regions
     * These steps must be performed prior to enabling the caches
     * a. Clear the instruction and data cache normal victim index
     *    registers
     * b. Clear the instruction and data cache transient victim index
     *    regs
     * c. Set the instruction and data cache victim limit registers
     */

    mtspr   INV0, r0
    mtspr   INV1, r0
    mtspr   INV2, r0
    mtspr   INV3, r0
    mtspr   DNV0, r0
    mtspr   DNV1, r0
    mtspr   DNV2, r0
    mtspr   DNV3, r0
    mtspr   ITV0, r0
    mtspr   ITV1, r0
    mtspr   ITV2, r0
    mtspr   ITV3, r0
    mtspr   DTV0, r0
    mtspr   DTV1, r0
    mtspr   DTV2, r0
    mtspr   DTV3, r0
    addis   r0, r0, HI(IVLIM_INIT)
    ori     r0, r0, LO(IVLIM_INIT)
    mtspr   IVLIM, r0
    mtspr   DVLIM, r0     /* note: DVLIM configured same as IVLIM */

    /* initialize TLB entries */

    bl      romTlbInit

    /* Clear and set up some registers. */

    li      r0, 0x0000
    lis     r1, 0xffff
    mtspr   DEC, r0         /* prevent dec exceptions */
    mtspr   TBL, r0         /* prevent fit & wdt exceptions */
    mtspr   TBU, r0
    mtspr   TSR, r1         /* clear all timer exception status */
    mtspr   TCR, r0         /* disable all */
    mtspr   ESR, r0         /* clear exception syndrome register */
    mtxer   r0              /* clear integer exception register */

#ifdef INCLUDE_L2_CACHE
    /* invalidate L2 cache */

    bl      l2CacheInvalidate

    /* enable L2 cache */

    bl      l2CacheEnable
#endif /* INCLUDE_L2_CACHE */

    /* the core will jump to address 0x0 and then go here */

    mfspr   r9, PIR
    cmpwi   r9, 0
    beq     skipLoop
    
    /*
     * Setup for subsequent change in instruction fetch address
     */

    lis     r3, HI(MSR_VALUE)
    ori     r3, r3, LO(MSR_VALUE)
    mtspr   SRR1, r3          /* Write the new MSR value into SRR1 */
    
    /* jump to sysCpu1Loop for SMP - sysInit for AMP */

    /* calculate CPUn_SPACE */

    rlwinm  r10, r9, CPU_SPACE_SIZE_SHIFT, 0, CPU_SPACE_SIZE_SHIFT - 1
    lis     r3, HI(RAM_LOW_ADRS_CPU0)
    ori     r3, r3, LO(RAM_LOW_ADRS_CPU0)
    subf    r10, r10, r3
    
waitForStartSet:
    lwz     r8, CPU_INIT_STATUS_ADR_OFFSET(r10)
        
    /*
     * Check cpu init status before jumping.
     * This allows cpu 1 ~ 3 to reboot and wait for cpu 0 to reload image.
     */

    cmpwi   r8, SERENGETI_MP_CPU_STATUS_READY
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

    /* enable clock to OCM and release its reset */

    lis     r4, HI(SCU_CLKEN)
    ori     r4, r4, LO(SCU_CLKEN)
    lwz     r5, 0(r4)
    ori     r5, r5, LO(SCU_CLKEN_OCM)
    stw     r5, 0(r4)

    lis     r4, HI(SCU_SRST)
    ori     r4, r4, LO(SCU_SRST)
    lwz     r5, 0(r4)
    rlwinm  r5, r5, 0, 21, 19
    stw     r5, 0(r4)

    lis     r4, HI(SCU_CSR_SRST)
    ori     r4, r4, LO(SCU_CSR_SRST)
    lwz     r5, 0(r4)
    rlwinm  r5, r5, 0, 21, 19
    stw     r5, 0(r4)

    lis     r4, HI(SCU_MRDY)
    ori     r4, r4, LO(SCU_MRDY)
redayCheck:
    lwz     r5, 0(r4)

    lis     r6, HI(OCM_RDY_MASK)
    ori     r6, r6, LO(OCM_RDY_MASK)
    and     r5, r5, r6
    cmplw   r5, r6
    bne     redayCheck

    /*
     * Initialize stack pointer to point at on chip SRAM.
     * It is used by C initialization code for APM Serengeti
     * board early initialization.
     */

    lis     sp, HI(ROM_STACK_ADRS)
    ori     sp, sp, LO(ROM_STACK_ADRS)

    /* Perform APM Serengeti board early initialization */

    bl      romBoardEarlyInit

    /* calculate C entry point: routine - entry point + ROM base */

    lis     r4, HIADJ(cold)
    addi    r4, r4, LO(cold)

    lis     r5, HIADJ(romInit)
    addi    r5, r5, LO(romInit)

    lis     r6, HIADJ(ROM_TEXT_ADRS)
    addi    r6, r6, LO(ROM_TEXT_ADRS)

    subf    r4, r5, r4      /* r4 = r4 - r5 */
    add     r4, r4, r6      /* r4 = r4 + r6 */

    mtspr   SRR0, r4        /* Write addr from which to cont exec into SRR0 */

    /*
     * Context synchronize to invalidate shadow TLB contents and
     * cause new TLB  contents to take effect. Jump to main boot
     * code.
     */

    rfi

    /* jump to rom cold entry */

FUNC_END(resetEntry)

/*******************************************************************************
*
* romTlbInit - initialize TLB entires for bootrom
*
* This routine initializes TLB entires for bootrom.
*
* RETURNS: N/A
*/

FUNC_BEGIN(romTlbInit)

    /* General MMU initialization */ 

    addis   r0, r0, HI(MMUCR_INIT)
    ori     r0, r0, LO(MMUCR_INIT)
    mtspr   MMUCR, r0

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
    
    xor     r1, r1, r1

    /* TLB1 #0: Flash Rom, 256M */

    TLB_WORD_SET(0, r1, r0, 0xf0000000 | TLB_V | _MMU_TLB_SZ_256M)
    TLB_WORD_SET(1, r1, r0, 0xf0000000 | FLASH_BASE_HI)
    TLB_WORD_SET(2, r1, r0, MMU_STATE_SUP_RWX_USR_RWX)
    TLB_NEXT(r1)

    /* TLB1 #1: Main DDR RAM, first 1GB */

    TLB_WORD_SET(0, r1, r0, PHY_MEM_BASE | TLB_V | _MMU_TLB_SZ_1G)
    TLB_WORD_SET(1, r1, r0, PHY_MEM_BASE | PHY_MEM_BASE_HI)
    TLB_WORD_SET(2, r1, r0, _MMU_TLB_ATTR_WL1 | _MMU_TLB_ATTR_PHY_M | MMU_STATE_SUP_RWX_USR_RWX)
    
    TLB_NEXT(r1)
    
    /* TLB1 #2: APM86x90 Freepool Mailboxes and DDR controller map space */
    
    TLB_WORD_SET(0, r1, r0, APM_MEMC_BASE | TLB_V | _MMU_TLB_SZ_16K)
    TLB_WORD_SET(1, r1, r0, APM_MEMC_PHY_BASE | APM_MEMC_PHY_BASE_HI)
    TLB_WORD_SET(2, r1, r0, _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | MMU_STATE_SUP_RW_USR_RW)
    TLB_NEXT(r1)
    
    /* TLB1 #3: APM86x90 CSR registers */
    
    TLB_WORD_SET(0, r1, r0, APM_CSR_BASE | TLB_V | _MMU_TLB_SZ_1M)
    TLB_WORD_SET(1, r1, r0, APM_CSR_BASE | APM_CSR_BASE_HI)
    TLB_WORD_SET(2, r1, r0, _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | MMU_STATE_SUP_RW_USR_RW)
    TLB_NEXT(r1)

    /* TLB1 #4: APM86x90 MPIC registers map space */

    TLB_WORD_SET(0, r1, r0, APM_MPIC_BASE | TLB_V | _MMU_TLB_SZ_256K)
    TLB_WORD_SET(1, r1, r0, APM_MPIC_BASE | APM_MPIC_BASE_HI)
    TLB_WORD_SET(2, r1, r0, _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | MMU_STATE_SUP_RW_USR_RW)
    TLB_NEXT(r1)

    /* TLB1 #5,6,7: APM86x90 peripherals registers map space */

    TLB_WORD_SET(0, r1, r0, APM_PERIPHERAL_BASE | TLB_V | _MMU_TLB_SZ_1M)
    TLB_WORD_SET(1, r1, r0, APM_PERIPHERAL1_PHY_BASE | APM_PERIPHERAL1_PHY_BASE_HI)
    TLB_WORD_SET(2, r1, r0, _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | MMU_STATE_SUP_RW_USR_RW)
    TLB_NEXT(r1)

    TLB_WORD_SET(0, r1, r0, (APM_PERIPHERAL_BASE + 0x100000) | TLB_V | _MMU_TLB_SZ_1M)
    TLB_WORD_SET(1, r1, r0, (APM_PERIPHERAL1_PHY_BASE + 0x100000) | APM_PERIPHERAL1_PHY_BASE_HI)
    TLB_WORD_SET(2, r1, r0, _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | MMU_STATE_SUP_RW_USR_RW)
    TLB_NEXT(r1)

    TLB_WORD_SET(0, r1, r0, (APM_PERIPHERAL_BASE + 0x200000) | TLB_V | _MMU_TLB_SZ_1M)
    TLB_WORD_SET(1, r1, r0, APM_PERIPHERAL2_PHY_BASE | APM_PERIPHERAL2_PHY_BASE_HI)
    TLB_WORD_SET(2, r1, r0, _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | MMU_STATE_SUP_RW_USR_RW)
    TLB_NEXT(r1)

    /* TLB1 #8,9: APM86x90 on chip memory */

    TLB_WORD_SET(0, r1, r0, APM_OCM_BASE | TLB_V | _MMU_TLB_SZ_16K)
    TLB_WORD_SET(1, r1, r0, APM_OCM_BASE_PHY_BASE | APM_OCM_BASE_PHY_BASE_HI)
    TLB_WORD_SET(2, r1, r0, _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | MMU_STATE_SUP_RW_USR_RW)
    TLB_NEXT(r1)

    TLB_WORD_SET(0, r1, r0, (APM_OCM_BASE + 0x4000) | TLB_V | _MMU_TLB_SZ_16K)
    TLB_WORD_SET(1, r1, r0, (APM_OCM_BASE_PHY_BASE + 0x4000) | APM_OCM_BASE_PHY_BASE_HI)
    TLB_WORD_SET(2, r1, r0, _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | MMU_STATE_SUP_RW_USR_RW)

    /* Clean the rest of the TLB entries */

    subfic  r0, r1, (N_TLB_ENTRIES - 1)
    mtspr   CTR, r0             /* Set the counter for the branch */
    addis   r0, r0, 0x0000      /* EPN = 0x00000000, Not Valid. */
TLB_clean_loop:
    addi    r1, r1, 0x0001      /* Increment to the next TLB entry */
    tlbwe   r0, r1, 0x0000      /* Write TLB entry word 0 */
    tlbwe   r0, r1, 0x0001      /* Write TLB entry word 1 */
    tlbwe   r0, r1, 0x0002      /* Write TLB entry word 2 */
    bdnz    TLB_clean_loop

    xor     r0, r0, r0
    mtspr   PID, r0

    isync

    blr
FUNC_END(romTlbInit)

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
    b   resetEntry
FUNC_END(resetVector)
