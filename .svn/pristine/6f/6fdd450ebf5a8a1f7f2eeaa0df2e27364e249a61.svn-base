/* romInit.s - ROM bootcode for QorIQ T4240 BSP */

/*
 * Copyright (c) 2012, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01q,17mar13,syt  setup a temp tlb map for DDR initialization C code.
                 added a workaround for the temporary internal SRAM for SIMICS.
01p,14mar13,syt  changed NOR Flash interface timing settings.
01o,11mar13,syt  moved the DDR initialization to the FSL common DDR
                 controller driver.
01n,26feb13,syt  enabled L1 D-cache when CPU come out from reset. 
01m,17feb13,syt  disable L2Cache after DDR controller initialization.
01l,08feb13,syt  add SPD function support.
01k,21jan13,wyt  updated code to accord with macros.
01j,16jan13,x_s  removed dead code.
01i,10jan13,x_z  removed INCLUDE_FALSH.
01h,11dec12,h_k  added workaround for DDR A-004934.
                 cleaned up unnecessary instructions.
01g,30nov12,x_s  cleaned macro name with T4 prefix instead of T4240.
01f,13nov12,h_k  updated for thread1.
                 fixed MMU attributes setting.
01e,05nov12,pcs  Updated the code to use _MMU_TLB_XXX macros instead of the
                 MMU_STATE_XXX macros.
01d,12nov12,syt  replaced macro MAIN_FLASH_SIZE with FLASH_SIZE.
01c,30oct12,h_k  updated DDR settings.
01b,09oct12,syt  code clean up.
01a,07may12,h_k  adapted from fsl_p4080_ds version 01w.
*/

#define _ASMLANGUAGE
#include <vxWorks.h>
#include <sysLib.h>
#include <asm.h>
#include <regs.h>
#include <arch/ppc/mmuE500Lib.h>
#include <arch/ppc/ppc85xx.h>
#include "config.h"

FUNC_EXPORT(romInit)
FUNC_EXPORT(_romInit)

FUNC_IMPORT(romStart)
FUNC_IMPORT(fslDdrCtlrInit)

#define WRITEADR(reg1,reg2,addr32,val)		\
	lis	reg2, HI(val);			\
	ori	reg2, reg2, LO(val);		\
	lis	reg1, HIADJ(addr32);		\
	stw	reg2, LO(addr32)(reg1);		\
	sync

#define WRITEOFFSET(regbase,reg2,offset,val)	\
	lis	reg2, HI(val);			\
	ori	reg2, reg2, LO(val);		\
	stw	reg2, offset(regbase);		\
	sync

        _WRS_TEXT_SEG_START

        .fill 0x100,1,0

FUNC_BEGIN(romInit)
FUNC_BEGIN(_romInit)
        bl    resetEntry
romWarm:
        bl     warm

        /* Useful to have recognizeable string */

        .ascii   "Wind River vxWorks bootrom"

        .balign 4

cold:
        li      r2, BOOT_CLEAR
        b       start

warm:
        mr      r2, r3

        mfspr   r6, PIR
        cmpwi   r6, 0
        beq     start

        /* running on secondary CPU */

        /* jump to sysInit for AMP - never get here for SMP */

        rlwinm  r6, r6, CPUn_SPACE_SIZE_SHIFT,0,31
                                    /* mult. cpunum by CPUn_SPACE_SIZE */
        lis     r3, HI(CPUn_SPACE_BASE_ADRS)
        ori     r3, r3, LO(CPUn_SPACE_BASE_ADRS)
        subf    r6, r6, r3          /* equiv. to CPUn_SPACE */

waitForStartSetWarm:
        lwz     r3, 8(r6)            /* CPUn_INIT_START_ADR */

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
                                        /* turn off exceptions   */
        mfmsr   r3                      /* r3 = msr              */
        INT_MASK (r3, r4)               /* mask EE and CE bit    */
        rlwinm  r4, r4, 0, 20, 18       /* turn off _PPC_MSR_ME  */
        mtmsr   r4                      /* msr = r4              */
        isync                           /* Per Table 3-2 e500mc Core RM Rev J note 2 */

        mfspr   r0, MCSR
        mtspr   MCSR, r0                /* Write set bits to clear */

        xor     r0, r0, r0
        addi    r1, r0, -1

        mtspr   DEC, r0
        mtspr   TBL, r0
        mtspr   TBU, r0
        mtspr   TSR, r1
        mtspr   TCR, r0
        mtspr   ESR, r0                 /* clear Exception Syndrome Reg */
        mtspr   XER, r0                 /*  clear Fixed-Point Exception Reg */
        lis     r1, 0xfff0
        ori     r1, r1, 0x0000
        mtspr   IVPR, r1

        /* TLB1 #0.  ROM - 0xff000000 -> 0xff000000., 16MB */

        addis   r4,  0,  0x1000         /* TLBSEL = TLB1(CAM) , ESEL = 0 */
        ori     r4,  r4, 0x0000
        addis   r5,  0,  HI(_MMU_TLB_VALID | _MMU_TLB_IPROT)
        ori     r5,  r5, _MMU_TLB_SZ_16M /* TS = 0, TSIZE = 16 MByte page size*/
        addis   r6,  0,  HI(FLASH_BASE_ADRS)   /* EPN */
        ori     r6,  r6, MMU_ROM_ACCESS
        addis   r7,  0,  HI(FLASH_BASE_ADRS)   /* RPN */
        ori     r7,  r7, 0x0015         /* Supervisor XWR*/

        bl      setTlbFn
        bl      initCacheFn

        /* enable timebase for core 0 */

        WRITEADR(r6, r7, T4_CTBENR, 1)

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

ddrDone:
        /*
         * Now that memory is stable we reset TLB entries for standard
         * operation.  Let the booted image itself map the 2nd DRAM's TLB.
         */

        /*
         * TLB1 #1.  Main SDRAM
         */

        li     r7, _LOCAL_MEM_ERPN           /* upper 8 bits of physical address */
        mtspr  MAS7, r7

        addis   r4, 0,  0x1001              /* TLBSEL = TLB1(CAM) , ESEL = 1 */
        addis   r5, 0,  HI(_MMU_TLB_VALID|_MMU_TLB_IPROT)
        ori     r5, r5, _MMU_TLB_SZ_1G      /* TS = 0, TSIZE = 1GByte page size*/
        addis   r6, 0,  HI(PHYS_MEM_START)  /* EPN */
        ori     r6, r6, MMU_RAM_ACCESS      /* WIMGE */
        addis   r7, 0,  HI(PHYS_MEM_START)  /* RPN */
        ori     r7, r7, 0x003f
        bl      setTlbFn

        /* TLB1 #3: DDR memory space: 0x40000000 -> 0x7FFFFFFF (1024MB) */

        li      r7, _LOCAL_MEM_ERPN          /* upper 8 bits of physical address */
        mtspr   MAS7,   r7

        addis   r4, 0,  0x1003              /* TLBSEL = TLB1(CAM) , ESEL = 3 */
        addis   r5, 0,  HI(_MMU_TLB_VALID|_MMU_TLB_IPROT)
        ori     r5, r5, _MMU_TLB_SZ_1G      /* TS = 0*/
        addis   r6, 0,  HI(PHYS_MEM_START+0x40000000)/* EPN */
        ori     r6, r6, MMU_RAM_ACCESS      /* WIMGE */
        addis   r7, 0,  HI(PHYS_MEM_START+0x40000000)/* RPN */
        ori     r7, r7, 0x003f
        bl      setTlbFn

        li      r7, 0                       /* clear upper 8 bits */
        mtspr   MAS7, r7
        isync

#if (PHYS_MEM_SIZE > 0x80000000)
#error "PHYS_MEM_SIZE > 0x80000000, currently unsupported"
#endif
        bl  cold

FUNC_END(_romInit)
FUNC_END(romInit)

l2Enable:
        lis     r3, HI(L2_CACHE_REG_BASE_ADDR + L2CACHE_REG_BASE)
        ori     r3, r3, LO(L2_CACHE_REG_BASE_ADDR + L2CACHE_REG_BASE)
        li      r4, 33  /* stash id */
        stw     r4, 4(r3)
        
        lis	    r4, HI(0x00200400)
        ori	    r4, r4, LO(0x00200400)
        sync
        stw	r4, 0(r3)	/* invalidate L2 */
        
1:      sync
        lwz     r0, 0(r3)
        twi     0, r0, 0
        isync
        and.    r1, r0, r4
        bne     1b
        
        lis     r4, HI(0x80000000)
        ori     r4, r4, LO(0x80000000)
        sync
        stw     r4, 0(r3)   /* enable L2 */
        
        li      r2, (32 + 0)
        mtspr   L1CSR2, r2
        blr

l2Disable:
        lis     r3, HI(L2_CACHE_REG_BASE_ADDR + L2CACHE_REG_BASE)
        ori     r3, r3, LO(L2_CACHE_REG_BASE_ADDR + L2CACHE_REG_BASE)

        /* Flush L2 cache */

        lwz     r4, L2CSR0_OFFSET(r3)
        ori     r4, r4, LO(0x00000800)
        stw     r4, L2CSR0_OFFSET(r3)

1:      lwz     r4, L2CSR0_OFFSET(r3)
        rlwinm. r4, r4, 0, L2CSR0_L2FL_BIT, L2CSR0_L2FL_BIT
        bne     1b

        lis     r4, HI(0x00000000)
        ori     r4, r4, LO(0x00000000)
        stw     r4, L2CSR0_OFFSET(r3)
        sync

        blr

/***************************************************************************
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
	/*
	 * Thread1 does not start from 0xfffffffc any more and should not come
	 * here but in case it comes, it will skip L1 cache initialization.
	 */

        mfspr   r0, _SPR_TIR
        cmpwi   r0, 0
        bne     skipInitCache

        bl     initCacheFn

skipInitCache:

        /* Clear SRR0, CSRR0, MCSRR0, SRR1, CSRR1 , MCSRR1, IVPR */

        mfspr   r0, MCSR
        mtspr   MCSR, r0

        xor     r0,r0,r0

        mtspr   SRR0,  r0
        mtspr   SRR1,  r0
        mtspr   CSRR0, r0
        mtspr   CSRR1, r0
        mtspr   MCSRR0,r0
        mtspr   MCSRR1,r0
        mtspr   ESR,   r0
        mtspr   DEAR,  r0

        mfmsr   r4
        andi.   r5, r4, _PPC_MSR_DE
        mtmsr   r5                 /* Per Table 3-4 e500mc Core RM Rev J note 1 */
        isync                      /* Per Table 3-3 e500mc Core RM Rev J row 1 */
        mtspr   DBCR0, r0
        mtspr   DBCR1, r0
        mtspr   DBCR2, r0
        mtspr   IAC1,  r0
        mtspr   IAC2,  r0
        mtspr   DAC1,  r0
        mtspr   DAC2,  r0
        mfspr   r1, DBSR
        mtspr   DBSR, r1
        isync                      /* Per Table 3-4 e500mc Core RM Rev J note 1 */
        mtmsr   r4                 /* Per Table 3-4 e500mc Core RM Rev J note 1 */
        isync                      /* Per Table 3-3 e500mc Core RM Rev J row 1 */

        isync                      /* Per Table 3-2 e500mc Core RM Rev J row 15 */
        mtspr   PID0, r0
        isync                      /* Per Table 3-3 e500mc Core RM Rev J row 15 */

        mtspr   TCR,  r0
        mtspr   BUCSR,r0

        mtspr   MAS4, r0
        mtspr   MAS5, r0
        mtspr   MAS6, r0
        mtspr   MAS7, r0
        mtspr   MAS8, r0
        isync                      /* Per Table 3-3 e500mc Core RM Rev J note 1 */

        mfspr   r6, PIR
        andi.   r4, r6, 1
        rlwinm  r6, r6, 30, 27, 30
        or      r6, r4, r6
        mtspr   PIR,r6

	/*
	 * Thread1 does not start from 0xfffffffc anymore and should not come
	 * here but in case it comes, it will skip MMU initialization.
	 */

        bne     skipMmuInit

        lis     r1, 0xfff0
        ori     r1, r1, 0x0000
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
        addi    r3, r0, 0x800
        mtspr   IVOR7, r3
        addi    r3, r0, 0x900
        mtspr   IVOR8, r3

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
        addi    r3, r0, 0x1400      /* perf mon is e500 specific */
        mtspr   IVOR35, r3

        li      r2, 0x06            /* L2TLB0_FI | L2TLB1_FI */
        mtspr   MMUCSR0, r2
        isync                       /* Per Table 3-4 e500mc Core RM Rev J row 10 */

        li      r3, 4
        li      r4, 0
        tlbivax r4, r3
        nop
        isync                       /* Per Table 3-3 e500mc Core RM Rev J notes 2,3 */

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

        /*
         * TLB1 #0.  ROM - 0xff000000 -> 0xffffffff.
         * 16MB
         * Attributes: SX/SW/SR **PROTECTED**
         */

        addis   r4, 0,  0x1000              /* TLBSEL = TLB1(CAM) , ESEL = 0 */
        addis   r5, 0,  HI(_MMU_TLB_VALID | _MMU_TLB_IPROT)
        ori     r5, r5, _MMU_TLB_SZ_16M     /* TS = 0, TSIZE = 16 MByte page size*/
        addis   r6, 0,  HI(FLASH_BASE_ADRS) /* EPN */
        ori     r6, r6, MMU_ROM_ACCESS
        addis   r7, 0,  HI(FLASH_BASE_ADRS) /* RPN */
        ori     r7, r7, 0x0015              /* Supervisor XWR*/

        bl      setTlbFn

        /*
         * TLB1 #1.  Main SDRAM - Cached
         *           PHYS_MEM_START -> +1 GB size
         * Attributes: UX/UW/UR/SX/SW/SR
         */

        li      r7, _LOCAL_MEM_ERPN          /* upper 8 bits of physical address */
        mtspr   MAS7, r7

        addis   r4, 0,  0x1001              /* TLBSEL = TLB1(CAM) , ESEL = 1 */
        addis   r5, 0,  HI(_MMU_TLB_VALID | _MMU_TLB_IPROT)
        ori     r5, r5, _MMU_TLB_SZ_1G      /* TS = 0, TSIZE = 1GByte page size*/
        addis   r6, 0,  HI(PHYS_MEM_START)  /* EPN = PHYS_MEM_START */
        ori     r6, r6, MMU_RAM_ACCESS      /* WIMGE */
        addis   r7, 0,  HI(PHYS_MEM_START)  /* RPN = PHYS_MEM_START */
        ori     r7, r7, 0x003f              /* User/Supervisor XWR*/

        bl      setTlbFn

        li      r7, 0                       /* clear upper 8 bits */
        mtspr   MAS7, r7

        /*
         * TLB1 #2.  CCSBAR
         * Attributes: UX/UW/UR/SX/SW/SR
         */

        addis   r4, 0,  0x1002              /* TLBSEL = TLB1(CAM) , ESEL = 2 */
        addis   r5, 0,  HI(_MMU_TLB_VALID | _MMU_TLB_IPROT)
        ori     r5, r5, _MMU_TLB_SZ_16M     /* TS = 0, TSIZE = 16 MByte page size*/
        addis   r6, 0,  HI(CCSBAR)          /* EPN = CCSBAR */
        ori     r6, r6, MMU_IO_ACCESS       /* WIMGE */
        addis   r7, 0,  HI(CCSBAR)          /* RPN = CCSBAR */
        ori     r7, r7, 0x0015              /* Supervisor XWR*/

        bl      setTlbFn

        /*
         * TLB1 #3.  On chip SRAM - Cached
         * Attributes: UX/UW/UR/SX/SW/SR
         */

        addis   r4, 0,  0x1003              /* TLBSEL = TLB1(CAM) , ESEL = 3 */
        addis   r5, 0,  HI(_MMU_TLB_VALID | _MMU_TLB_IPROT)
        ori     r5, r5, _MMU_TLB_SZ_16K     /* TS = 0, TSIZE = 16 KByte page size*/
        addis   r6, 0,  HI(ROM_INIT_RAM_BASE)  /* EPN = INTER_RAM_BASE */
        ori     r6, r6, 0                   /* WIMGE */

        /*
         * A half of the L1 D-cache lines are locked as a temporary internal
         * SRAM, it is used to store some global data and as the stack for
         * the DDR initialization in C code in the early boot stage.
         *
         * This function may not be simulated on Simics, that depends on its
         * specification. So use the FMAN 2 register space as a temporary
         * SRAM as its workaround.
         */

        addis   r7, 0,  HI(T4_FMAN2_BASE)   /* RPN = FMAN 2 base */
        ori     r7, r7, 0x0015              /* Supervisor XWR*/

        bl      setTlbFn

        /*
         * TLB1 #4.  QIXIS
         * Attributes: UX/UW/UR/SX/SW/SR
         */

        addis   r4, 0,  0x1004              /* TLBSEL = TLB1(CAM) , ESEL = 4 */
        addis   r5, 0,  HI(_MMU_TLB_VALID | _MMU_TLB_IPROT)
        ori     r5, r5, _MMU_TLB_SZ_4K      /* TS = 0, TSIZE = 4 KByte page size*/
        addis   r6, 0,  HI(QIXIS_BASE)      /* EPN = INTER_RAM_BASE */
        ori     r6, r6, MMU_IO_ACCESS       /* WIMGE */
        addis   r7, 0,  HI(QIXIS_BASE)      /* RPN = INTER_RAM_BASE */
        ori     r7, r7, 0x0015              /* Supervisor XWR*/

        bl      setTlbFn

        mfspr   r6, PIR
        cmpwi   r6, 0
        beq     doCcsrbar

skipMmuInit:
        /* running on CPU1-23 */
        /* set up initial stack pointer */

        rlwinm  r5, r6, CPUn_SPACE_SIZE_SHIFT, 0, 31
                                            /* mult. cpunum by CPUn_SPACE_SIZE */
        lis     r3, HI(CPUn_SPACE_BASE_ADRS)
        ori     r3, r3, LO(CPUn_SPACE_BASE_ADRS)
        subf    r5, r5, r3                  /* equiv. to CPUn_SPACE */

waitForStartSet:
        lwz     r3, 8(r5)                   /* CPUn_INIT_START_ADR */

        /*
         * Check that init start address is not zero before jumping
         * This allows cpu1 to reboot and wait for cpu0 to reload image
         */

        lis     r4, HI(MP_MAGIC_RBV)
        ori     r4, r4, LO(MP_MAGIC_RBV)
        cmpw    r3, r4
        beq     waitForStartSet

        lwz     sp, 4(r5)                   /* CPUn_STACK_START_ADR */

        /*
         * Thread1 should not come here from resetEntry
         * but in case it comes, it will skip MMU setting.
         */

	andi.	r6, r6, 1
	bne	branchToSysInit

        lwz     r9, 0x0c(r5)                /* CPUn_LOCAL_MEM_ERPN  */
        lwz     r8, 0x10(r5)                /* CPUn_LOCAL_MEM_RPN   */

        /*
         * The masking below indicates bits that must be cleared per spec.
         * However, if bits are set other than how the mask indicates then
         * wrload placed the image where it won't boot anyway.
         *
         * andi.    r9, r9, 0x00FF      -- low 8-bits are valid
         * andis.   r8, r8, IMAGE_MASK  -- For a IMAGE_BOUNDARY size TLB
         */

        /*
         * TLB1 #1. Execution space
         * Only map IMAGE_BOUNDARY to allow loading at any e.g. 256MB boundary.
         * The runtime kernel can later expand the range when
         * sysStaticTlbDesc becomes active in sysHwInit().
         */

        mtspr   MAS7,   r9                  /* Extended RPN */
        addis   r4, 0,  0x1001              /* TLB1 2nd entry (#1) */
        addis   r5, 0,  HI(_MMU_TLB_VALID|_MMU_TLB_IPROT)
        ori     r5, r5, IMAGE_BOUNDARY      /* TS = 0 */

        andis.  r6, r3, IMAGE_MASK          /* Mask Start addr for the IMAGE_BOUNDARY */
        andis.  r7, r8, IMAGE_MASK
        add     r7, r7, r6                  /* RPN(r8) = EPN(r4) + load bias(r8) */
        ori     r6, r6, MMU_RAM_ACCESS      /* WIMGE */
        ori     r7, r7, 0x003f              /* User/Supervisor XWR*/

        bl      setTlbFn

#if ((MIPC_SM_SYSTEM_POOL  > (PHYS_MEM_START + 0x100000)) || \
     (PHYS_MEM_START != 0))
#error "TLB3 mapping assumption broken"
#endif

	cmpwi	r9, 0
	bne	mapCPUn_SPACE

        andis.  r7, r3, IMAGE_MASK
	beq	branchToSysInit

mapCPUn_SPACE:
        /*
         * TLB1 #3.  For CPUn_SPACE (0x000e_0000,+0x1_0000) during boot.
         * Note: 1MB is below RAM_LOW_ADRS
         */

        li      r4, 0                       /* clear upper 8 bits */
        mtspr   MAS7, r4                    /* ERPN = 0 */

        addis   r4, 0,  0x1003              /* TLB1 4th entry (#3) */
        addis   r5, 0,  HI(_MMU_TLB_VALID|_MMU_TLB_IPROT)
        ori     r5, r5, _MMU_TLB_SZ_64K     /* TS = 0 */
        addis   r6, 0,  HI(CPUn_SPACE_BASE_ADRS - CPUn_SPACE_SIZE * MAX_CPUS)
        ori     r6, r6, MMU_RAM_ACCESS      /* WIMGE */
        addis   r7, 0,  HI(CPUn_SPACE_BASE_ADRS - CPUn_SPACE_SIZE * MAX_CPUS)
        ori     r7, r7, 0x003f              /* Supervisor XWR*/

        bl      setTlbFn

        /*
         * To support the wrload -loadbias option, pass a copy of ERPN/RPN to
         * secondary (same virtual, different physical address) so that
         * sysHwInit() can patch up sysStaticTlbDesc[] & sysPhysMemDesc[]
         *
         * stw    0x0c(r6) <-- CPUn_LOCAL_MEM_ERPN
         * stw    0x10(r6) <-- CPUn_LOCAL_MEM_RPN
         */

branchToSysInit:
        stwu    r0, -FRAMEBASESZ(sp)    /* save one stack frame (after TLB init) */
        mtctr   r3                      /* branch to CPUx_INIT_START_ADR */
        bctr                            /* sysInit sets BOOT start type */

doCcsrbar:

        /* Setup the memory mapped register address */

        WRITEADR(r6, r7, T4_LAWBARL8_RESET, CCSBAR)
        WRITEADR(r6, r7, T4_LAWAR8_RESET,
                            LAWAR_ENABLE | LAWAR_TGTIF_RESERVE | LAWAR_SIZE_16MB )

        sync    /* ensure LAWAR8 is set */

        lis     r7, HI(CCSBAR_RESET)
        ori     r7, r7, LO(CCSBAR_RESET)
        lwz     r4, (CCSRBARH_OFFSET)(r7)

        lwz     r4, (CCSRBARL_OFFSET)(r7)

        lis     r6, HI(CCSBAR)
        ori     r6, r6, LO(CCSBAR)

        xor     r5, r5, r5

        stw     r5, (CCSRBARH_OFFSET)(r7)
        stw     r6, (CCSRBARL_OFFSET)(r7)
        sync

        lis     r5, HI(CCSBAR_COMMIT)
        ori     r5, r5, LO(CCSBAR_COMMIT)
        stw     r5, (CCSRAR_OFFSET)(r7)
        sync

        WRITEADR(r6, r7, T4_LAWBARL8, 0x0)
        WRITEADR(r6, r7, T4_LAWAR8, 0x0)

        sync    /* ensure T4_LAWAR8 is set */

        li      r4, 0x200
        mtctr   r4

ccsrbarWait:
        nop
        bdnz    ccsrbarWait

        /* Initialize NOR Flash interface */

        WRITEADR(r6, r7, T4_LAWBARH0, 0x00)
        WRITEADR(r6, r7, T4_LAWBARL0, FLASH_BASE_ADRS)
        WRITEADR(r6, r7, T4_LAWAR0,
                         LAWAR_ENABLE | LAWAR_TGTIF_IFC | LAWAR_SIZE_16MB)
        sync

        WRITEADR(r6, r7, T4_IFC_FTIM0_CS0, 0x40050005)
        WRITEADR(r6, r7, T4_IFC_FTIM1_CS0, 0x35001a13)
        WRITEADR(r6, r7, T4_IFC_FTIM2_CS0, 0x0410381c)
        WRITEADR(r6, r7, T4_IFC_FTIM3_CS0, 0x00000000)
        WRITEADR(r6, r7, T4_IFC_CSOR0, 0x0000000c)
        WRITEADR(r6, r7, T4_IFC_CSPR0_EXT, 0x00000000)
        WRITEADR(r6, r7, T4_IFC_CSPR0, (FLASH_BASE_ADRS | 0x0101))
        WRITEADR(r6, r7, T4_IFC_AMASK0, ~(FLASH_SIZE - 1))
        sync

        WRITEADR(r6, r7, T4_LAWBARH4, 0x00)
        WRITEADR(r6, r7, T4_LAWBARL4, QIXIS_BASE)
        WRITEADR(r6, r7, T4_LAWAR4,
                         LAWAR_ENABLE | LAWAR_TGTIF_IFC | LAWAR_SIZE_4KB)

        sync

        /* Initialize QIXIS interface */

        WRITEADR(r6, r7, T4_IFC_FTIM0_CS3, 0xe00e000e)
        WRITEADR(r6, r7, T4_IFC_FTIM1_CS3, 0xff003f00)
        WRITEADR(r6, r7, T4_IFC_FTIM2_CS3, 0x0e00001f)
        WRITEADR(r6, r7, T4_IFC_FTIM3_CS3, 0x00000000)
        WRITEADR(r6, r7, T4_IFC_CSOR3, 0x00000000)
        WRITEADR(r6, r7, T4_IFC_AMASK3, ~(0x10000 - 1))
        WRITEADR(r6, r7, T4_IFC_CSPR3_EXT, 0x00000000)
        WRITEADR(r6, r7, T4_IFC_CSPR3, (QIXIS_BASE | 0x0085))

        sync

        /* Enable L2 Cache */

        bl  l2Enable

        /* lock the cache lines */
 
        lis     r3, HI(ROM_INIT_RAM_BASE)
        ori     r3, r3,  LO(ROM_INIT_RAM_BASE)
        mfspr   r2, L1CFG0
        andi.   r2, r2, 0x1ff
     
        /* lock cache size L1 32K/2 */
 
        slwi    r2, r2, (10 - 1 - L1_CACHE_SHIFT)
        mtctr   r2
        li      r0,0
 
cacheL1Lock:
 
        dcbz    r0, r3
        hwsync
        dcbtls  0, r0, r3
        hwsync    
        addi    r3, r3, L1_CACHE_LINE_SIZE
        bdnz    cacheL1Lock

        /*
         * Temporarily set up a mapping between RAM_LOW_ADRS and
         * ROM_BASE_ADRS. This allows our DDR initialization C code
         * to use its compiled-in addresses, which don't actually
         * coincide with the ROM/flash location from which it's
         * executing. This will be overwritten later when we
         * jump to ddrDone.
         */

        /*
         * Only the bootrom build from the BSP directory needs this
         * temporarily mapping.
         */

        lis r3, HI(wrs_kernel_text_start)
        ori r3, r3, LO(wrs_kernel_text_start)
        lis r4, HI(ROM_BASE_ADRS)
        ori r4, r4, LO(ROM_BASE_ADRS)
        cmplw   r3, r4
        beq     skipTempTlbSet

        li      r7, 0                       /* upper 8 bits of physical address */
        mtspr   MAS7, r7
        addis   r4, 0,  0x1001              /* TLBSEL = TLB1(CAM) , ESEL = 1 */
        addis   r5, 0,  HI(_MMU_TLB_VALID | _MMU_TLB_IPROT)
        ori     r5, r5, ROM_TLB_SZ          /* TS = 0, TSIZE = 1MByte page size*/
        addis   r6, 0,  HI(wrs_kernel_text_start)   /* EPN = wrs_kernel_text_start */
        ori     r6, r6, MMU_ROM_ACCESS      /* WIMGE */
        addis   r7, 0,  HI(ROM_BASE_ADRS)   /* RPN = ROM_BASE_ADRS */
        ori     r7, r7, 0x003f              /* User/Supervisor XWR*/
        bl      setTlbFn

skipTempTlbSet:

        lis     sp, HI(ROM_STACK_ADRS)
        ori     sp, sp, LO(ROM_STACK_ADRS)
        addi    sp, sp, -FRAMEBASESZ

        lis r3, HI(ROM_STACK_ADRS)
        ori r3, r3, LO(ROM_STACK_ADRS)
        
        bl  fslDdrCtlrInit

        /* unlock the cache lines */
 
        lis     r3, HI(ROM_INIT_RAM_BASE & ~(L1_CACHE_LINE_SIZE-1))
        ori     r3, r3, LO(ROM_INIT_RAM_BASE & ~(L1_CACHE_LINE_SIZE-1))
        mfspr   r4, L1CFG0
        andi.   r4, r4, 0x1ff
        slwi    r4, r4, (10 - 1 - L1_CACHE_SHIFT)
        mtctr   r4
 
cacheL1Unlock:
        dcbi    r0, r3
        addi    r3, r3, L1_CACHE_LINE_SIZE
        bdnz    cacheL1Unlock
        sync

        /* Disable L2 Cache */

        bl  l2Disable

        b  ddrDone

setTlbFn:
        mtspr   MAS0, r4
        mtspr   MAS1, r5
        mtspr   MAS2, r6
        mtspr   MAS3, r7
        isync
        tlbwe
        isync
        blr

initCacheFn:
        /* Beginning of e500mc Core RM Rev J section 11.5 code: */

        /* L1 data cache */

        xor   r4, r4, r4        /* set r4 to 0  */
        ori   r5, r4, 0x0102    /* set CFI and CLFC bit  */
        sync
        isync                   /* synchronize setting of L1CSR0  */
        mtspr L1CSR0, r5        /* flash invalidate L1 data cache */
        isync                   /* synchronize setting of L1CSR0  */

dloop:
        mfspr r4, L1CSR0        /* get current value */
        and.  r4, r4, r5        /* test CFI bit      */
        bne   dloop             /* check again if not complete    */
        isync                   /* discard prefetched instructions*/

        li    r5, 0x0001
        sync                    /* Per Table 3-2 e500mc Core RM Rev J row 10 */
        isync                   /* Per Table 3-2 e500mc Core RM Rev J row 10 */
        mtspr L1CSR0, r5        /* enable the L1 instruction cache */
        isync                   /* Per Table 3-2 e500mc Core RM Rev J row 10 */

        /* L1 instruction cache */
        xor   r4, r4, r4        /* set r4 to 0  */
        ori   r5, r4, 0x0002    /* set ICFI bit */
        sync
        isync                   /* synchronize setting of L1CSR1 */
        mtspr L1CSR1, r5        /* flash invalidate L1 instruction cache */
        isync                   /* synchronize setting of L1CSR1 */

iloop:
        mfspr r4, L1CSR1        /* get current value */
        and.  r4, r4, r5        /* test ICFI bit     */
        bne   iloop             /* check again if not complete     */
        isync                   /* discard prefetched instructions */

        /* End of e500mc Core RM Rev J section 11.5 code */

        /*
         * Currently causes problems if the data cache is turned on here:
         * fsl_p5020:  Works fine but reboot hangs.
         * wrSbcP4080: Boots & wrloads images from a start script then reboots.
         * This likely requires restructuring to initialize TLBs cleanly for
         * the reboot() case.
         */

        li    r5, 0x0001
        sync                    /* Per Table 3-2 e500mc Core RM Rev J row 10 */
        isync                   /* Per Table 3-2 e500mc Core RM Rev J row 10 */
        mtspr L1CSR1, r5        /* enable the L1 instruction cache */
        isync                   /* Per Table 3-2 e500mc Core RM Rev J row 10 */

        blr
FUNC_END(resetEntry)

#if   defined(_GNU_TOOL)
        .section .reset, "ax", @progbits
#elif defined(_DIAB_TOOL)
        .section .reset, 4, "rx"
#else
#error "Please add a correctly spelled .section directive for your toolchain."
#endif
FUNC_BEGIN(resetVector)
        b    resetEntry
FUNC_END(resetVector)

