/* usrCoreE6500.c - PowrPC QorIQ E6500 configuration */

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
01r,01mar13,h_k  decouple e6500 specific interrupt handler and SMT scheduler.
01q,07feb13,h_k  disabled L1 instruction cache before calling
                 cacheAimE500EnableI_Impl() to set consistent L1CSR1 value
                 between primary core.
01p,04feb13,pcs  Add inclusion of hearer file mmuE500Lib.h
01o,06feb13,wyt  removed Simics definition.
01n,06feb13,x_s  Restricted L2&L3 cache disable only for primary physical core 0 
                 reboot in AMP configuration.
01m,31jan13,h_k  replaced _wrs_kernel_data_start with _wrs_kernel_text_start
                 to set to the cachePpcReadOrigin.
01l,29jan13,h_k  added cachePpcReadOrigin setting to avoid memory access error
                 at data cache clear on the system CACHE_FLUSH_LOAD_ADRS
                 (0x10000) is not mapped.
                 fixed build and reboot failure on AMP in UP.
01k,21jan13,wyt  using I/O access APIs.
01j,23jan13,zl   changed coreE6500CpuReset() argument to bitset of EPIC IDs.
                 fixed timebase sync arrays indexing.
01i,23jan13,h_k  created a common errata workaround routine.
                 updated comment for mmuE500TlbStaticInit call.
01h,22jan13,h_k  fixed CPUn_LOCAL_MEM_RPN setting for non-primary AMP-SMP.
01g,17jan13,h_k  added missing CPUn_LOCAL_MEM_RPN setting in
                 coreE6500CpuEnable(). (WIND00398207)
                 added workaround for errata CPU A004792 and A004809.
01f,18jan13,zl   corrected thread 0 check in coreE6500CpuInit()
01e,10jan13,zl   added thread support (SMT) component
01d,06dec12,x_s  re-implemented timebase synchronization for SMP.
01c,04dec12,h_k  added workaround for errata CPU A004779, A004857, A005553.
01b,04dec12,h_k  fixed _WRS_CONFIG_SMP typo.
                 fixed coreE6500CpuLoop for threads on non-phys core0 and
                 non-primary AMP SMP.
01a,20nov12,x_s  written.
*/

/*
DESCRIPTION
This file is used to configure and initialize the PowrPC QorIQ E6500 cores based
on the required components or configuration. This file includes SMP & AMP 
multicore flag word configuration requried among cores, and inlcudes SMP time 
base synchronization.

This file is included by sysLib.c and could be used by SOC with PowrPC QorIQ 
E6500 core.

NOMANUAL
*/

/* includes */

#include "config.h"
#include <sysLib.h>
#include <arch/ppc/mmuE500Lib.h>

#ifdef INCLUDE_WRLOAD
#   include <wrload.h>
#endif /* INCLUDE_WRLOAD */

#ifdef _WRS_CONFIG_SMP
#   include <private/cpcLibP.h>
#   include <private/kernelLibP.h>
#   include <private/tickLibP.h>
#endif /* _WRS_CONFIG_SMP */

#ifdef INCLUDE_AMP
#   include <vxIpiLib.h>
#endif /* INCLUDE_AMP */

/*
 * can't enforce dependency in CDF, this informs users if configured 
 * incorrectly. 
 */

#if defined(INCLUDE_SMP_SCHED_SMT_POLICY) && !defined(INCLUDE_SMT)
# error INCLUDE_SMT must be added to support INCLUDE_SMP_SCHED_SMT_POLICY
#endif

/* __vxSmtSet is used by the SMT-optimzied SMP scheduler */

#ifdef	INCLUDE_SMP_SCHED_SMT_POLICY
cpuset_t __vxSmtSet[VX_SMP_NUM_CPUS];
#endif	/* INCLUDE_SMP_SCHED_SMT_POLICY */

#if defined(_WRS_CONFIG_SMP) && !defined(INCLUDE_SMT)
# define EPIC_ID_INTERVAL	E6500_THREAD_NUM
#endif	/* _WRS_CONFIG_SMP && !INCLUDE_SMT */

/*
 * a bitset of enabled CPUs in this VxWorks instance, identified by 
 * the EPIC IDs.
 */

LOCAL phys_cpuset_t epicIdEnabledSet;

/* externs */

IMPORT char _wrs_kernel_text_start[];
IMPORT char * cachePpcReadOrigin;
IMPORT void mmuE500TlbDynamicInvalidate ();
IMPORT void mmuE500TlbStaticInvalidate ();
IMPORT void mmuE500TlbStaticInit (int numDescs, TLB_ENTRY_DESC *pTlbDesc,
                                  BOOL cacheAllow);
IMPORT STATUS cacheAimE500DisableDFromMmu (void);
IMPORT STATUS cacheAimE500DisableIFromMmu (void);

IMPORT STATUS cacheAimE500FlushAllD (void);

#ifdef INCLUDE_LOGICAL_PARTITION
IMPORT void vxLpidSet (UINT32);
#endif /* INCLUDE_LOGICAL_PARTITION */

#ifdef INCLUDE_BRANCH_PREDICTION
IMPORT void vxEnableBP ();
IMPORT void vxDisableBP ();
#endif /* INCLUDE_BRANCH_PREDICTION */

#ifdef INCLUDE_CPU_LIGHT_PWR_MGR
IMPORT VOIDFUNCPTR _func_sysDoze;
IMPORT void coreE6500CpuDoze ();
#endif /* INCLUDE_CPU_LIGHT_PWR_MGR */

#if	(defined(T4_ERRATUM_CPU4786) && defined(FSL_ERRATA_CPU_A004786)) || \
	(defined(T4_ERRATUM_CPU4806) && defined(FSL_ERRATA_CPU_A004806))
IMPORT void vxSpr631BitSet (UINT32);
#endif	/* (T4_ERRATUM_CPU4786 && FSL_ERRATA_CPU_A004786) ||
	 * (T4_ERRATUM_CPU4806 && FSL_ERRATA_CPU_A004806)
	 */

#if	(defined(T4_ERRATUM_CPU4786) && !defined(FSL_ERRATA_CPU_A004786)) || \
	(defined(T4_ERRATUM_CPU4806) && !defined(FSL_ERRATA_CPU_A004806))
IMPORT void vxSpr631BitClear (UINT32);
#endif	/* (T4_ERRATUM_CPU4786 && !FSL_ERRATA_CPU_A004786) ||
	 * (T4_ERRATUM_CPU4806 && !FSL_ERRATA_CPU_A004806)
	 */

#if	defined(T4_ERRATUM_CPU4792) || defined(T4_ERRATUM_CPU4809)
# if	defined(FSL_ERRATA_CPU_A004792) || defined(FSL_ERRATA_CPU_A004809)
IMPORT void vxSpr976BitSet (UINT32);
# endif	/* FSL_ERRATA_CPU_A004792 || FSL_ERRATA_CPU_A004809 */

# if	!defined(FSL_ERRATA_CPU_A004792) || !defined(FSL_ERRATA_CPU_A004809)
IMPORT void vxSpr976BitCLear (UINT32);
# endif	/* !FSL_ERRATA_CPU_A004792 || !FSL_ERRATA_CPU_A004809 */
#endif	/* T4_ERRATUM_CPU4792 || T4_ERRATUM_CPU4809 */

#ifdef	T4_ERRATUM_CPU4779
# ifdef	FSL_ERRATA_CPU_A004779
IMPORT void vxSpr978BitSet (UINT32);
# endif	/* FSL_ERRATA_CPU_A004779 */

IMPORT void vxSpr978BitClear (UINT32);
#endif	/* T4_ERRATUM_CPU4779 */

#ifdef _WRS_CONFIG_SMP
IMPORT void vxSdaInit (void);
IMPORT UINT32 vxTenGet (void);
IMPORT void vxTenSet (UINT32);
IMPORT void vxTenClear (UINT32);
IMPORT void vxInia1Set (UINT32);
IMPORT void vxImsr1Set (UINT32);
IMPORT void vxPirSet (UINT32);
IMPORT void excIvorInit ();
IMPORT void mmuPpcPidSet (UINT32);
IMPORT void vxTimeBaseSet (UINT32 tbHI, UINT32 tbLO);
IMPORT void sysThreadReset (void);
#endif /* _WRS_CONFIG_SMP */

IMPORT int sysStaticTlbDescNumEnt;
IMPORT TLB_ENTRY_DESC sysStaticTlbDesc[];

/* forward declarations */

void coreE6500CpuPriInit (void);
void coreE6500CpuShutdown(int startType);
LOCAL void coreE6500CpuReset (phys_cpuset_t epicIdSet);
LOCAL void coreE6500ErrataWorkaround (void);

#ifdef _WRS_CONFIG_SMP
# ifdef	INCLUDE_SMT
LOCAL void coreE6500ThreadEnable (UINT32);
# endif	/* INCLUDE_SMT */

LOCAL void coreE6500CpuInit (UINT32);
LOCAL void coreE6500CpuNonPriShutdownLoop (void);
STATUS coreE6500CpuEnable (UINT32, WIND_CPU_STATE *);
# if (defined(DRV_TIMER_M85XX) && defined(INCLUDE_TIMESTAMP))

#  ifdef INCLUDE_SMT
#  define DOMAIN_COUNT	(VX_SMP_NUM_CPUS / E6500_THREAD_NUM)
#  else
#  define DOMAIN_COUNT	(VX_SMP_NUM_CPUS)
#  endif

/* upper 32 bit value of timebase */

volatile UINT32 tbHI[DOMAIN_COUNT];

/* lower 32 bit value of timebase */

volatile UINT32 tbLO[DOMAIN_COUNT];

/* flag for global TB value */

volatile UINT32 tbCpuReady[DOMAIN_COUNT];

/* flag for slave core booted up */

volatile UINT32 subCpuReady[DOMAIN_COUNT];
#   endif  /* DRV_TIMER_M85XX && INCLUDE_SYS_TIMESTAMP */
#endif /* _WRS_CONFIG_SMP */

#ifdef INCLUDE_WRLOAD
IMPORT UINT32 usrWrloadBiasHighGet(WRLOAD_ID id); /* from usrWrload.c */
IMPORT UINT32 usrWrloadBiasLowGet (WRLOAD_ID id); /* from usrWrload.c */
void coreE6500AmpCpuEnable (FUNCPTR, UINT32);
STATUS coreE6500AmpCpuPrep (UINT32, void *);
#endif /* INCLUDE_WRLOAD */

#ifdef	INCLUDE_L2_CACHE
# ifdef	T4_ERRATUM_CPU4857
IMPORT BOOL t4240ErratamCpu4857;
# endif	/* T4_ERRATUM_CPU4857 */

# ifdef	T4_ERRATUM_CPU5553
IMPORT BOOL t4240ErratamCpu5553;
# endif	/* T4_ERRATUM_CPU5553 */
#endif	/* INCLUDE_L2_CACHE */

#ifdef INCLUDE_AMP
IMPORT void cacheL2E6500LibInit (void);
IMPORT void cacheL3E6500LibInit (void);
#endif /* INCLUDE_AMP */

/*******************************************************************************
*
* coreE6500CpuPriInit - initialize primary core
*
* This routine initializes various features of the primary E6500 core. It sets 
* up HID register and branch prediction enablement based on the configuration and 
* TLBs.
*
* It is only called by sysHwInit for primary core initialization.
*
* RETURNS: NONE
*
* \NOMANUAL
*/

void coreE6500CpuPriInit (void)
    {
#if defined (_WRS_CONFIG_SMP)
    int i;
#endif /* _WRS_CONFIG_SMP */

    /* check and enable errata workarounds */

    coreE6500ErrataWorkaround();

    /* Disable L1 Icache */

    vxL1CSR1Set(vxL1CSR1Get() & ~_PPC_L1CSR_E);

    /*
     * Put cachePpcReadOrigin at the start of text section.
     * Assuming the text section is always mapped and cacheable.
     * It must not be in a dirty state when the region is used to clear entire
     * data cache.
     */

    cachePpcReadOrigin = (char *) &_wrs_kernel_text_start;

    /* Check for architecture support for 36 bit physical addressing */

    vxHid0Set(_PPC_HID0_MAS7EN | vxHid0Get());

    /* Enable machine check pin */

    vxHid0Set(HID0_MCP | vxHid0Get());

    /* enable time base for delay use before DEC interrupt is setup */

    vxDecSet(0xffffffff);
    vxDecarSet(0xffffffff);

    /*
     * Need to setup static TLB entries for bootrom or any non-MMU
     * enabled images
     */

    mmuE500TlbDynamicInvalidate();
    mmuE500TlbStaticInvalidate();

#ifdef INCLUDE_LOGICAL_PARTITION
    vxLpidSet(LOGICAL_PARTITION_ID);
#endif /* INCLUDE_LOGICAL_PARTITION */

    mmuE500TlbStaticInit(sysStaticTlbDescNumEnt, &sysStaticTlbDesc[0], TRUE);

    /* Machine check via RXFE for RIO */

#ifdef INCLUDE_BRANCH_PREDICTION
    vxEnableBP();
#endif /* INCLUDE_BRANCH_PREDICTION */

#ifdef INCLUDE_CPU_LIGHT_PWR_MGR
    _func_sysDoze = coreE6500CpuDoze;
#endif /* INCLUDE_CPU_LIGHT_PWR_MGR */


#ifdef _WRS_CONFIG_SMP
# ifdef INCLUDE_SMT
#  ifdef INCLUDE_SMP_SCHED_SMT_POLICY
    /* initialize SMT set for boot CPU */

    CPUSET_SET (__vxSmtSet[0], 0);
#  endif /* INCLUDE_SMP_SCHED_SMT_POLICY */
#else
    /* 
     * when thread support is not included, skip IDs corresponding to 
     * non-primary threads (see EPIC_ID_INTERVAL).
     */

    vxbSysEpicIdAssign (VX_SMP_NUM_CPUS, EPIC_ID_INTERVAL);
# endif	/* INCLUDE_SMT */

    /* set EPIC bit for boot CPU */

    PHYS_CPUSET_SET (epicIdEnabledSet, VXB_SYS_EPIC_ID_GET(CPU_LOGICAL_TO_PHYS(0)));


    /*
     * Here we initialize non-cpu0 init address so we don't wait until the
     * correct value is initialized
     */

    for (i = 1; i < VX_SMP_NUM_CPUS; i++)
        {
	UINT epicId = VXB_SYS_EPIC_ID_GET (CPU_LOGICAL_TO_PHYS(i));
	*(UINT32*)CPUn_INIT_START_ADR(epicId) = MP_MAGIC_RBV;
	}
#endif /* _WRS_CONFIG_SMP */
    }

/*******************************************************************************
*
* coreE6500ErrataWorkaround - apply errata workarounds
*
* This routine applies the erratum workarounds if necessary.
*
* RETURNS: NONE
*/

LOCAL void coreE6500ErrataWorkaround (void)
    {
#if defined(T4_ERRATUM_CPU4779) || defined(T4_ERRATUM_CPU4786) || \
    defined(T4_ERRATUM_CPU4792) || defined(T4_ERRATUM_CPU4806) || \
    defined(T4_ERRATUM_CPU4809) || \
    (defined(INCLUDE_L2_CACHE) && \
     (defined(T4_ERRATUM_CPU4857) && defined(FSL_ERRATA_CPU_A004857)) || \
     (defined(T4_ERRATUM_CPU5553) && defined(FSL_ERRATA_CPU_A005553)))
    /* All the erratum will be fixed on Rev 2.0 or later. */

    if ((T4_CSR_READ_32(T4_SVR) & 0x000000ff) < 0x20)
        {
#ifdef	INCLUDE_L2_CACHE
# ifdef	T4_ERRATUM_CPU4857
#  ifdef FSL_ERRATA_CPU_A004857
        t4240ErratamCpu4857 = TRUE;
#  else
        t4240ErratamCpu4857 = FALSE;
#  endif /* FSL_ERRATA_CPU_A004857 */
# endif	/* T4_ERRATUM_CPU4857 */

# ifdef	T4_ERRATUM_CPU5553
#  ifdef FSL_ERRATA_CPU_A005553
        t4240ErratamCpu5553 = TRUE;
#  else
        t4240ErratamCpu5553 = FALSE;
#  endif /* FSL_ERRATA_CPU_A005553 */
# endif	/* T4_ERRATUM_CPU5553 */
#endif	/* INCLUDE_L2_CACHE */

#ifdef	T4_ERRATUM_CPU4779
        /*
         * Disable the workaround that might have been set by the previous
         * image or bootrom on silicon Rev 1.x.
         */

        vxSpr978BitClear (0xff7f0000);

# ifdef	FSL_ERRATA_CPU_A004779
        /*
         * Workaround: Follow all of these steps.
         * 1. Set bit 32 of SPR978 to 1.
         * 2. Set bits 33:39 of SPR978 to 0b0000011.
         * 3. Set bits 41:47 of SPR978 to 0b0000011.
         */

        vxSpr978BitSet (0x83030000);
# endif	/* FSL_ERRATA_CPU_A004779 */
#endif	/* T4_ERRATUM_CPU4779 */

#ifdef	T4_ERRATUM_CPU4786
# ifdef	FSL_ERRATA_CPU_A004786
        /*
         * Workaround CPU_A004786:
         * Set bit 32 of SPR631 to 1.
         */

        vxSpr631BitSet (0x80000000);
# else
        /*
         * Disable the workaround that might have been set by the previous
         * image or bootrom on silicon Rev 1.x.
         */

	vxSpr631BitClear (0x80000000);
# endif	/* FSL_ERRATA_CPU_A004786 */
#endif	/* T4_ERRATUM_CPU4786 */

#ifdef	T4_ERRATUM_CPU4792
# ifdef	FSL_ERRATA_CPU_A004792
        /*
         * Workaround CPU_A004792.
         * Set bit 49 of SPR 976 to 1.
         */

        vxSpr976BitSet (0x00004000);
# else
        /*
         * Disable the workaround that might have been set by the previous
         * image or bootrom on silicon Rev 1.x.
         */

        vxSpr976BitClear (0x00004000);
# endif	/* FSL_ERRATA_CPU_A004792 */
#endif	/* T4_ERRATUM_CPU4792 */

#ifdef	T4_ERRATUM_CPU4806
# ifdef	FSL_ERRATA_CPU_A004806
        /*
         * Workaround CPU_A004806:
         * Set bit 34 of SPR631 to 1.
         */

        vxSpr631BitSet (0x20000000);
# else
        /*
         * Disable the workaround that might have been set by the previous
         * image or bootrom on silicon Rev 1.x.
         */

        vxSpr631BitClear (0x20000000);
# endif	/* FSL_ERRATA_CPU_A004806 */
#endif	/* T4_ERRATUM_CPU4806 */

#ifdef	T4_ERRATUM_CPU4809
# ifdef	FSL_ERRATA_CPU_A004809
        /*
         * Workaround: Follow all of these steps.
         * 1. Set bit 39 of SPR 976 to 1.
         * 2. Set bit 48 of SPR 976 to 1.
         */

        vxSpr976BitSet (0x01008000);
# else
        /*
         * Disable the workaround that might have been set by the previous
         * image or bootrom on silicon Rev 1.x.
         */

        vxSpr976BitClear (0x01008000);
# endif	/* FSL_ERRATA_CPU_A004809 */
#endif	/* T4_ERRATUM_CPU4809 */
        }
#endif	/* T4_ERRATUM_CPU4779 || T4_ERRATUM_CPU4786 || T4_ERRATUM_CPU4792 ||
         * T4_ERRATUM_CPU4806 || T4_ERRATUM_CPU4809 ||
         * (INCLUDE_L2_CACHE &&
         *  (T4_ERRATUM_CPU4857 && FSL_ERRATA_CPU_A004857) ||
         *  (T4_ERRATUM_CPU5553 && FSL_ERRATA_CPU_A005553))
         */
    }

#ifdef _WRS_CONFIG_SMP
#ifdef	INCLUDE_SMT
/*******************************************************************************
*
* coreE6500ThreadEnable - enable a thread
*
* This routine brings a thread out of disabled
*
* RETURNS: NONE
*/

LOCAL void coreE6500ThreadEnable
    (
    UINT32  threadBit   /* thread bit to enable */
    )
    {
    vxInia1Set ((UINT32) &sysThreadReset);

    vxTenSet (threadBit);
    }
#endif	/* INCLUDE_SMT */

/*******************************************************************************
*
* coreE6500CpuInit - initialize additional core
*
* This routine initializes additional cores, including TLBs,cache,and others
*
* It is called by coreE6500CpuLoop.
*
* RETURNS: NONE
*
*/

LOCAL void coreE6500CpuInit
    (
    UINT32 physCpuIndex
    )
    {
    BOOL isThread0 = TRUE;
    UINT cpuId = CPU_PHYS_TO_LOGICAL(physCpuIndex);
#if defined(DRV_TIMER_M85XX) && defined(INCLUDE_TIMESTAMP)
    UINT32 domainIndex;
#endif /* DRV_TIMER_M85XX && INCLUDE_TIMESTAMP */    

    /* write logical CPU */

    vxPirSet (cpuId);

#ifdef INCLUDE_SMT
    if (E6500_THREAD(physCpuIndex) != 0)
	isThread0 = FALSE;
#endif

    if (isThread0)
        {                
        vxIvprSet ((UINT32)VEC_BASE_ADRS);
        excIvorInit();

        vxHid0Set(_PPC_HID0_MAS7EN | vxHid0Get());

        /* Enable machine check pin */

        vxHid0Set(HID0_MCP | vxHid0Get());

        mmuE500TlbDynamicInvalidate();
        mmuE500TlbStaticInvalidate();
        }

#ifdef INCLUDE_LOGICAL_PARTITION
    vxLpidSet(LOGICAL_PARTITION_ID);
#endif /* INCLUDE_LOGICAL_PARTITION */

    /*
     * The mmuE500TlbStaticInit () initializes tlb1EselPtr and some other
     * tables for all the logical cores and need to execute the routine
     * from thread1, as well.
     *
     * The static TLB entries set is done by thread0. The thread1 skips
     * the mapping and shares the TLB entries with the thread0.
     */

    mmuE500TlbStaticInit (sysStaticTlbDescNumEnt, &sysStaticTlbDesc[0], TRUE);

    /* check and enable errata workarounds */

    coreE6500ErrataWorkaround();

    if (isThread0)
        {
	/* Disable L1 Icache */

	vxL1CSR1Set(vxL1CSR1Get() & ~_PPC_L1CSR_E);

#ifdef USER_I_CACHE_ENABLE
        cacheAimE500EnableI_Impl ();    /* enable instruction cache */
#endif /* USER_I_CACHE_ENABLE */

#ifdef USER_D_CACHE_ENABLE
        cacheAimE500EnableD_Impl ();    /* enable data cache */
#endif /* USER_D_CACHE_ENABLE */

#ifdef INCLUDE_L1_CACHE_ERROR_RECOVERY
        cacheL1E6500RecoveryEnable();
#endif  /* INCLUDE_CACHE_ERROR_HANDLER */
        }
    
#ifdef INCLUDE_BRANCH_PREDICTION
    vxEnableBP();
#endif /* INCLUDE_BRANCH_PREDICTION */

    mmuPpcPidSet(MMU_ASID_GLOBAL);

#if (SW_MMU_ENABLE == FALSE)
    vxMsrSet(vxMsrGet() | _PPC_MSR_IS | _PPC_MSR_DS);
#endif /* SW_MMU_ENABLE = FALSE */

    if (isThread0)
        {
#if (defined(DRV_TIMER_M85XX) && defined(INCLUDE_TIMESTAMP))

#ifdef INCLUDE_SMT
        domainIndex = cpuId / E6500_THREAD_NUM;
#else
        domainIndex = cpuId;
#endif
 
        /* 
         * set time base ready flag indicating the current processor is ready to 
         * set time base value to register.
         */
                         
        subCpuReady[domainIndex] = 1;

        /* wait till the primary CPU to get global time base */
        
        while (!tbCpuReady[domainIndex])
	    ;
        
        /* Set the current timebase read from primary CPU. */

        vxTimeBaseSet (tbHI[domainIndex], tbLO[domainIndex]);                                   
#endif  /* DRV_TIMER_M85XX && INCLUDE_TIMESTAMP */
        }
    }

/*******************************************************************************
*
* coreE6500CpuLoop - CPU loop
*
* This routine is the CPU idle loop function which will invoke the startup
* function specified in the coreE6500CpuEnable function.
*
* RETURNS: NONE
*
* \NOMANUAL
*/

void coreE6500CpuLoop(void)
    {
    unsigned int a = 0;
    FUNCPTR p;
    UINT epicId;
    UINT physCpu;

    /*
     * The call to vxSdaInit() must be the first operation here.
     * This is because vxSdaInit() sets the r2 and r13 registers to the
     * SDA and SDA2 base registers. No C code must be placed before this
     * call.
     */

    _WRS_ASM ("");   /* code barrier to prevent compiler moving vxSdaInit() */
    vxSdaInit ();    /* this MUST be the first operation in coreE6500CpuLoop() */
    _WRS_ASM ("");   /* code barrier to prevent compiler moving vxSdaInit() */

    /*  
     * read the start address that was set for this CPU. CPUn_XXX() are 
     * indexed based on the unique EPIC ID; 
     */

    epicId = T4_CSR_READ_32(T4_EPICWHOAMI);
    p = (FUNCPTR)(*(UINT32 *)CPUn_FUNC_START_ADR (epicId));

    physCpu = *(UINT32 *)CPUn_PHYS_ID(epicId);

    coreE6500CpuInit (physCpu);

    if (p != NULL)
        {
        p ();
        }

    /* something went wrong, but we must not return from this routine */

    for (;;)
        ;
    }

/*******************************************************************************
*
* coreE6500CpuEnable - enable a multi core CPU
*
* This routine brings a CPU out of reset.
* It is called by sysCpuEnable.
*
* RETURNS: OK or ERROR
*
* \NOMANUAL
*/

STATUS coreE6500CpuEnable
    (
    UINT32 physCpuIndex, /* physical cpu ID */
    WIND_CPU_STATE * pCpuState
    )
    {
    STATUS status = OK;
    UINT32        physPrimaryThread;
    UINT32        physDomainset;
    UINT32        epicId;
    int		  cpuId;
#ifdef INCLUDE_SMT
    _Vx_ticks64_t tickStart;
    cpuset_t      cpuset;
    phys_cpuset_t physCpuset;
# ifdef INCLUDE_SMP_SCHED_SMT_POLICY
    int           primCpuId;
# endif /* INCLUDE_SMP_SCHED_SMT_POLICY */
    int           cpuIx;
#endif
    int           key;
    UINT32        oldMsr;
#if defined(DRV_TIMER_M85XX) && defined(INCLUDE_TIMESTAMP)
    UINT32        domainIndex;
    _Vx_ticks64_t timeoutTicks = VX_ENABLE_CPU_TIMEOUT * sysClkRateGet ();
    _Vx_ticks64_t timeoutStart, currentTickCount;    
#endif /* DRV_TIMER_M85XX && INCLUDE_TIMESTAMP */

    /* make sure physCpuIndex is requesting an available CPU other than 0 */

    cpuId = CPU_PHYS_TO_LOGICAL (physCpuIndex);

    if ((physCpuIndex == 0) || (cpuId >= vxCpuConfiguredGet()))
        return ERROR;

    epicId = VXB_SYS_EPIC_ID_GET (physCpuIndex);
    PHYS_CPUSET_SET (epicIdEnabledSet, epicId);

    /*
     * if loaded above the 32-bit boundary, we need to write the bootstrap
     * words a second time for after the new CPU update its TLB1 entries with
     * its ERPN.  To accomplish this, temporarily switch to TS0 (static TLBs)
     * so that the translation will result in the physical address with
     * ERPN != 0.
     */

    key = intCpuLock ();

    vxMsrSet ((oldMsr = vxMsrGet()) & ~_PPC_MSR_DS);

    *((int *)CPUn_PHYS_ID(epicId)) = (int)physCpuIndex;
    *((int *)CPUn_FUNC_START_ADR(epicId)) = (int)pCpuState->regs.pc;
    *((int *)CPUn_STACK_START_ADR(epicId)) = (int)pCpuState->regs.spReg;
    *((int *)CPUn_LOCAL_MEM_ERPN(epicId)) = (int)_LOCAL_MEM_ERPN;
#ifdef	WRLOAD_IMAGE_BUILD_PHYS_BIAS
    *((int *)CPUn_LOCAL_MEM_RPN(epicId)) = (int)WRLOAD_IMAGE_BUILD_PHYS_BIAS;
#else
    *((int *)CPUn_LOCAL_MEM_RPN(epicId)) = 0;
#endif	/* WRLOAD_IMAGE_BUILD_PHYS_BIAS */
    *((int *)CPUn_INIT_START_ADR(epicId)) = (int)coreE6500CpuLoop;

#ifdef  INCLUDE_CACHE_SUPPORT
    /*
     * Flush Data cache so as to make sure other cores have updated cache.
     * Since cacheFlush is no-op in SMP, we use cacheTextUpdate as its
     * replacement.
     */

    cacheTextUpdate ((char *) CPUn_FUNC_START_ADR(epicId),CPUn_SPACE_SIZE);
#endif  /* INCLUDE_CACHE_SUPPORT */

    vxMsrSet (oldMsr);

    intCpuUnlock (key);

#ifdef INCLUDE_SMT

    physPrimaryThread = physCpuIndex & ~(E6500_THREAD_NUM - 1);

    if (physCpuIndex == physPrimaryThread)
	{
# ifdef INCLUDE_SMP_SCHED_SMT_POLICY
	/*
	 * primary thread; this is enabled implicitly with the core reset,
	 * we only need to do the book-keeping part here for __vxSmtSet.
	 * 
	 * The primary thread is alway the first one in the group.
	 */

	CPUSET_SET (__vxSmtSet[cpuId], cpuId);
# endif /* INCLUDE_SMP_SCHED_SMT_POLICY */
	}
    else
	{
	/* prevent migration */

	taskCpuLock();

	if (vxCpuPhysIndexGet() == physPrimaryThread)
	    {
	    /* currently running the associated primary thread */

	    coreE6500ThreadEnable (E6500_THREAD_BIT(physCpuIndex));
	    }
	else
	    {
	    /* 
	     * not running on associated primary thread, must send CPC;
	     * we may have to wait for the primary thread to be ready for
	     * accepting CPCs.
	     */

	    tickStart = vxAbsTicksGet();
	    while (TRUE)
		{
	        cpusetLogicalToPhys (vxCpuEnabledGet(), &physCpuset);

		if (PHYS_CPUSET_ISSET(physCpuset, physPrimaryThread))
		    break;

		if ((vxAbsTicksGet() - tickStart) > 2)
        	    {
		    taskCpuUnlock();

        	    if (_func_logMsg != NULL)
        		{
        		_func_logMsg (
				"Error: CPU%d must be enabled before CPU%d\n",
                        	       CPU_PHYS_TO_LOGICAL(physPrimaryThread),
				       cpuId, 0, 0, 0, 0);
        		}

		    return (ERROR);
		    }
		}

	    CPUSET_ZERO (cpuset);
	    CPUSET_SET  (cpuset, CPU_PHYS_TO_LOGICAL(physPrimaryThread));

            cpcInvoke (cpuset, (CPC_FUNC) coreE6500ThreadEnable,
                       (void *)E6500_THREAD_BIT(physCpuIndex),
                       sizeof(int), VX_CPC_SYNC);
	    }

	taskCpuUnlock();

# ifdef INCLUDE_SMP_SCHED_SMT_POLICY
	/* update __vxSmtSet[] for primary thread */

	primCpuId = CPU_PHYS_TO_LOGICAL (physPrimaryThread);
	CPUSET_SET (__vxSmtSet[primCpuId], cpuId);

	/* 
	 * copy __vxSmtSet[] for the rest of the CPUs in the set.
	 * "cpuset &= cpuset - 1" is used to clear the least significant 
	 * bit set.
	 */

	cpuset = __vxSmtSet[primCpuId];
	CPUSET_CLR (cpuset, primCpuId);

	for ( ; (cpuIx = CPUSET_FIRST_INDEX (cpuset)) != NONE; 
	     cpuset &= cpuset - 1)
	    {
	    CPUSET_ADD (__vxSmtSet[cpuIx], __vxSmtSet[primCpuId]);
	    }
# endif /* INCLUDE_SMP_SCHED_SMT_POLICY */

	/* for non-primary threads nothing else to do */

	return (OK);
	}

#endif /* INCLUDE_SMT */

#if defined(DRV_TIMER_M85XX) && defined(INCLUDE_TIMESTAMP)
#ifdef INCLUDE_SMT
    domainIndex = cpuId / E6500_THREAD_NUM;
#else
    domainIndex = cpuId;
#endif
    tbCpuReady[domainIndex] = 0;
    subCpuReady[domainIndex] = 0;
#endif /* DRV_TIMER_M85XX && INCLUDE_TIMESTAMP */

    coreE6500CpuReset (1 << epicId);

    /* for referencing BRR, CTBEN bits, use the EPIC ID */

    physDomainset = 1 << (epicId / E6500_THREAD_NUM);

    /* Set BRR to make core released for booting */
    
    T4_CSR_WRITE_32(T4_BRR, T4_CSR_READ_32(T4_BRR) | physDomainset);

#if defined(DRV_TIMER_M85XX) && defined(INCLUDE_TIMESTAMP)
    /* set time base enablement for corresponding cores */

    T4_CSR_WRITE_32(T4_CTBENR, T4_CSR_READ_32(T4_CTBENR) | physDomainset);  

    /* wait till the corresponding subordinate CPU ready to set time base */
    
    timeoutStart = vxAbsTicksGet ();
    
    while (!subCpuReady[domainIndex])
        {
        currentTickCount = vxAbsTicksGet ();
        if ((currentTickCount - timeoutStart) > timeoutTicks)
            return (ERROR);
        }

    /* lock interrupts for a critical section */

    key = intCpuLock ();
    
    /* get the global time base value */
    
    vxTimeBaseGet (&tbHI[domainIndex], &tbLO[domainIndex]);    
    
    /* 
     * set time base ready flag indicating the primary CPU has gotten global  
     * time base value, it's ready to slave core to set time base value.
     */

    tbCpuReady[domainIndex] = 1;
    
    intCpuUnlock (key);    
#endif /* DRV_TIMER_M85XX && INCLUDE_SYS_TIMESTAMP */

    return status;
    }

/*******************************************************************************
*
* coreE6500CpuNonPriShutdownLoop - parking loop for CPU != 0
*
* This routine is launched onto secondary cores during system reboot. It locks
* interrupts, flushes d-cache, and puts the core into a tight loop so that it
* doesn't interfere with the sysToMonitor() routine.
*
* RETURNS: N/A
*/

LOCAL void coreE6500CpuNonPriShutdownLoop (void)
    {
    UINT32 epicId;
    UINT32 oldMsr;

    (void) intCpuLock();

    /*
     * Flush L1 data cache first.
     * This must be done before flushing L2 cache unless L2 cache is
     * flushed and disabled.
     *
     * The cacheFlush API is a no-op in an SMP system.
     * Instead, call the AIM primitive directly.
     */

    cacheAimE500FlushAllD ();

    vxMsrSet ((oldMsr = vxMsrGet()) & ~_PPC_MSR_DS);

    /* Report to CPU0 this CPU's cache flush is complete */

    epicId = T4_CSR_READ_32(T4_EPICWHOAMI);
    *(UINT32*)CPUn_INIT_START_ADR(epicId) = MP_MAGIC_CACHE_FLUSHED;

    vxMsrSet (oldMsr);

    /* make sure the magic num is visible on CPU0 immediately */

    _WRS_ASM("sync");

    /*
     * Do not over write any persistent data, which will be accessed by
     * bootrom from, now on.  Data cache is still enabled and the cached data
     * will not be visible from bootrom.
     *
     * So long as core0 hard-resets the non-primary cores and bootrom
     * and VxWorks correctly invalidate and disable the L1 and L2
     * caches, it is fine to leave the data cache enabled here.
     *
     * Note: If you want to disable data cache, the stack used within this
     * function must be flushed unless this routine is written in assmbly
     * and free from stack usage.  There is no easy way to find from
     * where to where need to be flushed but if you want to flush entire
     * data cache, be careful not to copy-back the data to physical memory,
     * where Core0 might already modify directly with data cache off.
     */

    /* Finally idle this CPU */

#ifdef INCLUDE_CPU_LIGHT_PWR_MGR
    coreE6500CpuDoze();
#endif  /* INCLUDE_CPU_LIGHT_PWR_MGR */

    /* expect to lock in DOZE mode until HRESET */

    FOREVER;
    }
#endif /* _WRS_CONFIG_SMP */

/*******************************************************************************
*
* coreE6500CpuReset - reset a set of CPUs
*
* This routine resets a set of CPUs. It is called when:
*  1. enabling an SMP CPU (sysCpuEnable->coreE6500CpuEnable)
*      - a single CPU, always primary thread (Thread 0).
*  2. starting AMP (wrloadCpuPrep->...->sysAmpCpuPrep->coreE6500AmpCpuPrep)
*      - a single CPU, always the first CPU of the AMP instance. This is 
*        always a primary thread (Thread 0)
*  3. rebooting (sysToMonitor->coreE6500CpuShutdown)
*  3.1. rebooting from AMP0
*      - reset all CPUs in the system, except self (caller ensured)
*      - called by CPU 0
*  3.2. rebooting from SMP of AMPn (n != 0)
*      - reset all enabled CPUs in SMP instance, except self (caller ensured)
*      - called by logical CPU 0
*  4. reset a secondary AMP (TBD, not yet implemented in wrload)
*      - reset all CPUs of a secondary AMP
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void coreE6500CpuReset
    (
    phys_cpuset_t epicIdset
    )
    {
    int i, j;
    phys_cpuset_t physDomainset;
    UINT currentEpicId;
    UINT32 brr;
    int key;
    UINT thread0EpicId;
    UINT32 threadOthers;

    PHYS_CPUSET_ZERO(physDomainset);

    key = intCpuLock ();

    /*
     * don't attempt to reset any of CPUs on cores that have not been 
     * released for booting (per BRR register). The BRR bits are
     * per domain (i.e. for all threads of a physical core).
     *
     * domain 0 is never disabled, so we can start the verification
     * with the first thread of domain 1. 
     */

    brr = T4_CSR_READ_32(T4_BRR);

    for (i = E6500_THREAD_NUM; i < MAX_CPUS; i++)
        {
        if (PHYS_CPUSET_ISSET (epicIdset, i))
            {
            /* don't attempt to reset non-running CPUs */

            if (PHYS_CPUSET_ISSET(brr, (i / E6500_THREAD_NUM)))
                {
                PHYS_CPUSET_SET(physDomainset, (i / E6500_THREAD_NUM));
                }
            else
                {
                PHYS_CPUSET_CLR(epicIdset, i);
                }
            }
        }

    /* mask off and end all interrupts */

    for (i = 1; i < MAX_CPUS; i++)
        {
        if (PHYS_CPUSET_ISSET(epicIdset, i))
            {
            /*
             * writing 15 to current task priority register effectively masks
             * all interrupts
             */

            T4_CSR_WRITE_32(T4_EPICCTPRn(i), EPIC_CTPR_MAX);

            /* send sufficient number of EOIs to end all in-service interrupts */

            for (j = 0; j < RESET_EOI_NUM; j++)
                T4_CSR_WRITE_32(T4_EPICEOIn(i), 0);
            }
        }

    currentEpicId = VXB_SYS_EPIC_ID_GET (vxCpuPhysIndexGet());

    /* disable other threads on the current core */

    thread0EpicId = currentEpicId - E6500_THREAD(currentEpicId);

    threadOthers = epicIdset & (E6500_THREAD_ALL << thread0EpicId);

    if (threadOthers != 0)
        {
#ifdef	_WRS_CONFIG_SMP
        vxTenClear (threadOthers >> thread0EpicId);

        /* initialize init  next inst address and MSR on thread1 */

        vxInia1Set (0xfffffffc);
        vxImsr1Set (0);
#endif	/* _WRS_CONFIG_SMP */

        /*
         * Don't attempt to reset PIR for other threads on the current
         * processor.
         */

        epicIdset &= ~threadOthers;
        }

    /* do not stop timebase and do not clear BRR for current domain */

    PHYS_CPUSET_CLR (physDomainset, currentEpicId / E6500_THREAD_NUM);

    /* stop timebase */

    T4_CSR_WRITE_32(T4_CTBENR, T4_CSR_READ_32(T4_CTBENR) & ~physDomainset);

    /* put threads on other processors into P10 state */

    T4_CSR_WRITE_32(T4_TPH10SETR0, epicIdset);

    /* reset cores using PIR */

    T4_CSR_WRITE_32(T4_EPICPIR, T4_CSR_READ_32(T4_EPICPIR) | epicIdset);

    /* read the PIR, this pushes the previous write. */

    T4_CSR_READ_32(T4_EPICPIR);

    /* clear boot release */

    T4_CSR_WRITE_32(T4_BRR, T4_CSR_READ_32(T4_BRR) & (~physDomainset));

    /* clear PIR */

    T4_CSR_WRITE_32(T4_EPICPIR, T4_CSR_READ_32(T4_EPICPIR) & (~epicIdset));

    /*
     * clear P10 state; we do this for the threads on the current core as well,
     * in case they were in that state due to power management.
     */

    T4_CSR_WRITE_32(T4_TPH10CLRR0, epicIdset | threadOthers);

    intCpuUnlock (key);
    }

#ifdef  INCLUDE_CPU_LIGHT_PWR_MGR

/*******************************************************************************
*
* coreE6500CpuDoze - set thread into doze state 
*
* this routine puts the current thread into PH10 state.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void coreE6500CpuDoze (void)
    {
    UINT32 phyCpuIndex;
    UINT32 epicId;
    
    phyCpuIndex = vxCpuPhysIndexGet();
    epicId = VXB_SYS_EPIC_ID_GET (phyCpuIndex);

    T4_CSR_WRITE_32(T4_TPH10SETR0, 1 << epicId);
    }
#endif /* INCLUDE_CPU_LIGHT_PWR_MGR */

/*******************************************************************************
*
* coreE6500CpuShutdown - shut down primary or slave cores
*
* This routine shuts down the primary or slave cores, including disable caches 
* and invalid TLBs and others.
*
* It is only called by sysToMonitor.
*
* RETURNS: NONE
*
* SMP Considerations
*
* When rebooting an SMP system, it is likely that all CPUs' data caches,
* and especially the L2 caches if enabled, contain modified lines which
* have not been written out to SDRAM (although, due to hardware-enforced
* coherency, at most one CPU's cache contains a modified copy of any one
* cache line).  To ensure that critical diagnostic data -- such as kernel
* stacks, the ED&R buffer, and the coredump buffer -- are preserved, we
* must ensure that all CPUs flush their caches before the system reboots.
* This requires a CPC (cross processor call). Depending on the nature of
* the cause for shutdown, the system may be unstable such that the CPC's
* will be unsuccessful. As an added precaution to save as much state as
* possible before resetting, we perform a cache flush prior to sending
* any CPC.
*
* \cs
*      sysHwInit
* +--------------------------------+
* |  clear flush "DONE" magic num  |
* +--------------------------------+
*
*                                   +- - - - - - - - - - - - - - - - - - - - -+
* sysToMonitor()                    |                                         |
*   starts on any CPU               v
* +-----------------------------------------------------------------------+   |
* |                             current CPU ?                             |
* |                  0                  |                n                |   |
* +-------------------------------------+---------------------------------+
* |CPC to CPU n:cpuNonPriShutdownLoop   |   CPC to CPU 0:  sysToMonitor   |- -+
* +--------------------------|----------+---------------------------------+
*   |
*   |                        |
*   |                        + - - - - - - - +
*   |                                        |  coreE6500CpuNonPriShutdownLoop()
*   v    always on CPU 0                     v         always on CPU n's
* +---------------------------------+   +---------------------------------+
* |          intCpuLock ()          |   |          intCpuLock ()          |
* +---------------------------------+   +---------------------------------+
* |  wait for CPUn flush or timeout |   |          flush d-cache          |
* +---------------------------------+   +---------------------------------+
* |       disable D & I cache       |   |   write flush "DONE" magic num  |
* +---------------------------------+   +---------------------------------+
* |   misc resetting of processor   |   |               nap               |
* |      registers and state        |   +---------------------------------+
* +---------------------------------+
* |   put CPUn's into hard reset    |
* +---------------------------------+
* |         jump to bootrom         |
* +---------------------------------+
* \ce
*
* \NOMANUAL
*/

void coreE6500CpuShutdown
    (
    int startType   /* parameter passed to ROM to tell it how to shut down */
    )
    {
    UINT32 epicId;
#if (defined (INCLUDE_AMP) || defined (_WRS_CONFIG_SMP))
    cpuset_t cpuset;
    phys_cpuset_t epicIdSet;
#endif  /* INCLUDE_AMP || _WRS_CONFIG_SMP */
#ifdef  _WRS_CONFIG_SMP
    int i;
    int thisPhysCpu;
    int countSmpCpus;
    int countCacheFlushed;
    UINT32 oldMsr;
#endif  /* _WRS_CONFIG_SMP */

#ifdef _WRS_CONFIG_SMP

    /*
     * Make sure we cannot migrate, but without locking
     * interrupts -- see cpcInvoke() requirements.
     */

    if (!INT_CONTEXT() && !KERNEL_ENTERED_ME())
        (void) taskCpuLock();

    /*
     * In SMP mode, the last part of sysToMonitor() must run on core 0
     * because it will jump to the bootrom.
     *
     * If we happen to already be on core 0, we first cause CPU 1 to n to clear
     * their caches and enter nap mode (or a parking loop) after which CPU 0
     * can proceed with shutdown.  If we are on a core n, we will proceed
     * to the coreE6500CpuNonPriShutdownLoop after arranging for sysToMonitor() 
     * to be reinvoked on core 0.
     */

    if (vxCpuIndexGet() == 0)
        {
        CPUSET_ZERO (cpuset);        /* implicitly sends CPC to all except self */

        cpcInvoke (cpuset, (CPC_FUNC)coreE6500CpuNonPriShutdownLoop, NULL, 0,
                   VX_CPC_ASYNC);
        }
    else
        {
        CPUSET_ZERO (cpuset);
        CPUSET_SET  (cpuset, 0);     /* send CPC to CPU 0 only */

        cpcInvoke (cpuset, (CPC_FUNC)sysToMonitor, (void *)startType,
                   sizeof(startType), VX_CPC_ASYNC);

        FOREVER;
        }

    (void) intCpuLock ();      /* CPC done, now safe to disable interrupts */

    /*
     * Only CPU 0 gets here,
     * but not sure if it is physical core 0 on an AMP system
     * or merely logical core 0 on an SMP M-N system
     */

    thisPhysCpu = vxCpuPhysIndexGet();

    i = 50;            /* 0.5sec (50 * 0.01) max wait for CPUn */
    do  {
        UINT32 initStartAdr;
        sysMsDelay(10);

        countSmpCpus = VX_SMP_NUM_CPUS - 1;
        countCacheFlushed = 0;

        vxMsrSet ((oldMsr = vxMsrGet()) & ~_PPC_MSR_DS);

        do  
            {
            epicId = VXB_SYS_EPIC_ID_GET (thisPhysCpu + countSmpCpus);
	    initStartAdr = *(volatile UINT32 *) CPUn_INIT_START_ADR(epicId);

	    if ((initStartAdr == MP_MAGIC_CACHE_FLUSHED)
#if (ENABLE_ALL_CPUS == FALSE)
               || (initStartAdr == MP_MAGIC_RBV)
#endif  /* ENABLE_ALL_CPUS == FALSE */
                )
                {
                countCacheFlushed++;
                }
            } while (--countSmpCpus > 0);

        vxMsrSet (oldMsr);

        if (countCacheFlushed == (VX_SMP_NUM_CPUS - 1))
            {
            break;
            }

        } while (--i > 0);
#else   /* _WRS_CONFIG_SMP */

    /* lock interrupts in this entire routine */

    (void) intCpuLock();
#endif  /* _WRS_CONFIG_SMP */

#ifdef  INCLUDE_CACHE_SUPPORT
   
    /*
     * for AMP, L2 & L3 shared cache register hook have been removed
     * to avoid conflict among AMP cores, here need to re-register the 
     * hook to disable cache when reboot.
     */ 

#   if defined(INCLUDE_AMP) && defined(INCLUDE_AMP_CPU_00)
#      ifdef INCLUDE_L2_CACHE
       cacheL2E6500LibInit();
#      endif /* INCLUDE_L2_CACHE */

#      ifdef INCLUDE_L3_CACHE
       cacheL3E6500LibInit();
#      endif /* INCLUDE_L3_CACHE */
#   endif /* INCLUDE_AMP */

    /*
     * The cacheDisable() API is not supported in SMP, so call the AIM
     * flush primitives directly.
     */

    cacheAimE500DisableDFromMmu();
    cacheAimE500DisableIFromMmu();

#endif  /* INCLUDE_CACHE_SUPPORT */

#ifdef INCLUDE_BRANCH_PREDICTION
    vxDisableBP();
#endif /* INCLUDE_BRANCH_PREDICTION */

    /*
     * set MSR to its power-up default value
     * disables ALL interrupts and exceptions, including debugging breakpoints
     */

    vxMsrSet(0);

    /* Clear unnecessary TLBs */

    mmuE500TlbDynamicInvalidate();
    mmuE500TlbStaticInvalidate();

#if (defined(_WRS_CONFIG_SMP) || defined(INCLUDE_AMP_CPU_00))

    /*
     * Reset the secondary CPUs. If this is AMP0 then reset all
     * CPUs, otherwise reset the CPUs enabled in the current VxWorks
     * instance.
     */

# if defined (INCLUDE_AMP_CPU_00)
    epicIdSet = ~(0xfffffffe << (MAX_CPUS - 1));
# else
    epicIdSet = epicIdEnabledSet;
# endif /* INCLUDE_AMP_CPU_00 */

    /* clear self */

    epicId = VXB_SYS_EPIC_ID_GET (CPU_LOGICAL_TO_PHYS(0));
    PHYS_CPUSET_CLR (epicIdSet, epicId);

    coreE6500CpuReset (epicIdSet);

#endif /* _WRS_CONFIG_SMP || INCLUDE_AMP_CPU_00 */

#if defined(INCLUDE_AMP) && !defined(INCLUDE_AMP_CPU_00)

    /* slave AMP must wait for wrload to boot up */

    *((volatile int *)CPUn_INIT_START_ADR(CPU_LOGICAL_TO_PHYS(0))) = (int)MP_MAGIC_RBV;
    _WRS_ASM("sync");

    /* set physical CPU ID to PIR prior to entering into bootrom */

    vxPirSet (CPU_LOGICAL_TO_PHYS(0));
#endif /* INCLUDE_AMP && !INCLUDE_AMP_CPU_00 */

    return;
    }

#ifdef INCLUDE_WRLOAD
/*****************************************************************************
*
* coreE6500AmpCpuEnable - Starts core executing code at entryPt
*
* This routine performs the hardware specific code to start the secondary
* core. It does so using two methods - using boot page translation register
* when first called. After this it uses CPU1_INIT_START_ADR to inform the
* second core where to jump to. The second core if reboot will wait until
* CPU1_INIT_START_ADR is set to something other than MP_MAGIC_RBV which is
* what this routines is doing. The success or failure of this routine cannot
* be determined here.
* This function is required for wrload.
*
* It is only called by sysAmpCpuEnable.
*
* RETURNS: NONE
*
* \NOMANUAL
*/

void coreE6500AmpCpuEnable
    (
    FUNCPTR entryPt,
    UINT32 cpu
    )
    {
    int    key;
    UINT32 oldMsr;
    UINT32 domainBit = CPU_BIT(cpu/2);
    UINT32 cpuBit = CPU_BIT(cpu);
    
    /*
     * if loaded above the 32-bit boundary, we need to write the bootstrap
     * words a second time for after the new CPU update its TLB1 entries with
     * its ERPN.  To accomplish this, temporarily switch to TS0 (static TLBs)
     * so that the translation will result in the physical address with
     * ERPN != 0.
     */

    key = intCpuLock ();

    vxMsrSet ((oldMsr = vxMsrGet()) & ~_PPC_MSR_DS);

    *((volatile int *)CPUn_FUNC_START_ADR(cpu)) = (int)0; /* SMP specific */

    /* SMP specific not used just loaded */

    *((volatile int *)CPUn_STACK_START_ADR(cpu)) = ((int)entryPt - FRAMEBASESZ);

    /*
     * The physical CPU ID stored in PIR on the first core on an AMP system 
     * always equals the EPIC ID 
     */

    *((volatile int *)CPUn_PHYS_ID(cpu)) = cpu;

    /* This is used if core waiting after core1 did asynchronous reboot */

    *((volatile int *)CPUn_INIT_START_ADR(cpu)) = (int)entryPt;

    *((volatile int *)CPUn_LOCAL_MEM_ERPN(cpu)) = usrWrloadBiasHighGet(cpu);
    *((volatile int *)CPUn_LOCAL_MEM_RPN(cpu))  = usrWrloadBiasLowGet (cpu);

#ifdef  INCLUDE_CACHE_SUPPORT
# ifdef _WRS_CONFIG_SMP
    /*
     * Flush Data cache so as to make sure other cores have updated cache.
     * Since cacheFlush is no-op in SMP, we use cacheTextUpdate as its
     * replacement.
     */

    cacheTextUpdate ((char *) CPUn_FUNC_START_ADR(cpu), CPUn_SPACE_SIZE);
# else
    cacheFlush (DATA_CACHE, (char *) CPUn_FUNC_START_ADR(cpu),
                CPUn_SPACE_SIZE);
# endif /* _WRS_CONFIG_SMP */
#endif /* INCLUDE_CACHE_SUPPORT */

    vxMsrSet (oldMsr);
    intCpuUnlock (key);

    /* If core is not already running then enable it */

    if ((T4_CSR_READ_32(T4_BRR) & domainBit) == 0)
        {
        T4_CSR_WRITE_32(T4_CTBENR, T4_CSR_READ_32(T4_CTBENR) | domainBit);
        T4_CSR_WRITE_32(T4_BRR, T4_CSR_READ_32(T4_BRR) | domainBit);
        }
    }

/*****************************************************************************
*
* coreE6500AmpCpuPrep - Prep cpuId for wrload download and start of AMP image
*
* This function is used by wrload to make a core ready for wrload to
* download and/or start a new image.  The state of the target cpu
* after this call is not specified, and is OS/arch/CPU specific.  If
* return is OK, then wrload should expect to succeed; if ERROR, then
* wrload will abort.
*
* flags argument currently unused, expected to provide finer grain control
* in the future.
*
* It is only called by sysAmpCpuPrep.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*
* \NOMANUAL
*/

STATUS coreE6500AmpCpuPrep
    (
    UINT32  physCpuIndex,
    void *  flags
    )
    {
    int    key;
    UINT32 oldMsr;
    int    basePhysCpu = CPU_LOGICAL_TO_PHYS(0);

    /* cannot prep core 0 or any of the cores assigned to current image */

    if ((physCpuIndex == 0) || (physCpuIndex == basePhysCpu))
        return ERROR;

#ifdef _WRS_CONFIG_SMP
    if ((physCpuIndex > basePhysCpu) &&
        (physCpuIndex < basePhysCpu + VX_SMP_NUM_CPUS))
	return ERROR;
#endif /* _WRS_CONFIG_SMP */

    /* shutdown ints */

    vxbDevMethodRun (DEVMETHOD_CALL(vxbIntCtlrCpuDisable),
                     (void *)physCpuIndex);

    key = intCpuLock ();

    vxMsrSet ((oldMsr = vxMsrGet()) & ~_PPC_MSR_DS);

    *((volatile int *)CPUn_FUNC_START_ADR(physCpuIndex)) =
                     (int)0; /* SMP specific */

    /* SMP specific not used just loaded */

    *((volatile int *)CPUn_STACK_START_ADR(physCpuIndex)) =
                     (int)RAM_LOW_ADRS - FRAMEBASESZ;

    /* This is used if waiting after CPU did asynchronous reboot */

    *((volatile int *)CPUn_INIT_START_ADR(physCpuIndex)) =
                     (int)MP_MAGIC_RBV;

    *((volatile int *)CPUn_LOCAL_MEM_ERPN(physCpuIndex)) = (int)0;

    vxMsrSet (oldMsr);
    intCpuUnlock (key);

    /* For AMP boot CPU, the EPIC ID is same as the physical CPU ID */

    coreE6500CpuReset (1 << physCpuIndex);

    return OK;
    }
#endif /* INCLUDE_WRLOAD */
