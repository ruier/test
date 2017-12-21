/* usrMmuInit.c - memory management unit initialization */

/*
 * Copyright (c) 1992-1996, 1998-2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute or otherwise make use of this software
 * may be licensed only pursuant to the terms of an applicable Wind River
 * license agreement.
 */

/*
modification history
--------------------
04h,04may11,syt  add support for PPC465.
04g,01oct10,sem  Add new IA cpus. Remove pentium2/3
04f,08sep10,pcs  Updated to pass the configuration parameter
                 GLOBAL_VIRT_MEM_REGION_SIZE to adrSpaceLibInit.
04e,20sep10,jpb  Replaced private header file, aimMmuLibP.h with extern because
                 aimMmuLibP.h was causing a redefinition of a struct when 
                 building images.
04d,09sep10,jpb  Added header file for _func_aimMmuLockRegionsImport.
04c,20aug10,x_z  add support for PPC476.
04b,02jul10,zl   set page optimization flag for 64-bit
04a,29jun10,slk  updates for parameter passing to mmuMipsLibInit
03z,09mar10,pad  Removed settings of the RTP region for VxWorks 64-bit.
03y,10feb10,zl   64-bit VxWorks require true MMU support
03x,01nov09,pcs  Pass rtpRgnBase and rtpRgnSize to adrSpaceLibInit
                 unconditionally, since the component INCLIDE_ADR_SPACE_LIB is
                 included by default in a 64bit system.
03w,26oct09,pcs  Updated to pass the RTP private region info to
                 adrSpaceLibInit for LP64.
03v,04sep09,zl   removed usage of sysPhysMemDesc[] for 64-bit.
03u,31jul09,jb   Adding 64bit MMU support
03t,24jun09,zl   updated address space initialization
03s,30apr09,zl   removed unused vmBaseLibInit() argument
03r,25aug08,jpb  Renamed _WRS_VX_SMP to _WRS_CONFIG_SMP.
03q,23jun08,pad  Now pass the RTP_OVERLAPPED_ADDRESS_SPACE parameter to
                 adrSpaceLibInit().
03p,07may08,pad  Now passes RTP_CODE_REGION_SIZE and RTP_CODE_REGION_START
		 parameters to adrSpaceLibInit().
03o,15oct07,zl   SMP kernel requires MMU support included.
03n,21feb07,pes  Add support for new MIPS lib structure.
03m,31mar06,pch  SPR 119828: SW_MMU_ENABLE must disable D-cache on PPC
03l,19jan06,rec  change mmu548xLib to mmuV4eLib
03k,28oct05,rec  add Coldfire MMU support
03j,23aug05,rec  leave SW_MMU_ENABLE=FALSE for Coldfire
03i,31oct05,zl   added INCLUDE_VM_INVTBL.
03h,09aug05,zl   make global map support separate component. Fix warnings
                 from #else (SPR#111044).
03g,19aug05,md   fixed parameters being passed to ED&R macro
03f,13jul05,h_k  added ifdef INCLUDE_EDR_STUB.
03e,24feb05,pch  adjust PPC970 support
03d,22apr04,to   PPC970 support
03c,13dec04,h_k  renamed mmuSh77xxLibInit to mmuShLibInit.
03b,22nov04,zl   test KERNEL_HEAP_SIZE parameter against sysMemTop().
03a,16nov04,zl   fixed warning for mmuSoftLibInit() parameter
02z,10nov04,dtr  SPR #103997.
02y,10nov04,scm  remove osm init for i86 to comply with new API...
02x,02nov04,to   SPR#102918 enable I&D MMU for PPC if adrSpaceLib is included.
02w,28oct04,pes  Added conditional definition of SW_MMU_ENABLE for MIPS.
02v,12oct04,kk   updated kernelBaseLib.h to private/kernelBaseLibP.h
02u,12oct04,pcs  Replace vmBaseGlobalMapInit with vmGlobalMapInit.
02t,03oct04,tam  commented out call to sysOSMTaskGateInit (PENTIUM Specific)
02s,29sep04,tam  replaced EDR_KH by PROTECT_TASK_STACK
02r,29sep04,pcs  Add 2 more parameters to mmuPpcLibInit.
02q,20sep04,zl   avoid using printExc(); fixed MMU-less init without RTPs.
02p,15sep04,zl   replaced adrSpaceLib.h with private version. Removed 
                 obsolete sections.
02o,10aug04,kk   renamed rtpBaseLib kernelBaseLib
02o,08aug04,sru  Added call to _func_aimMmuLockRegionsImport.
02n,19jul04,tam  moved vmContextUnionInit() to pgMgrLibInit()
02m,02jun04,mem  remove arguments to MIPS MMU lib init.
02l,09jun04,zl   replaced INCLUDE_MMU_FULL with INCLUDE_VM_RTP.
02k,18may04,kk   replaced rtpIdSelf() with MY_CTX_ID()
02j,29apr04,gls  added vmInvTblLibInit()
02i,08apr04,zl   added software MMU simulation support.
02h,05apr04,yp   reworking adrSpaceLibInit call
02g,24feb04,jtp  SPR 92581 detect config error if RTP support enabled
                 with I- or D-MMU disabled.
02f,03dec03,jmp  added VxSim MMU header files.
02m,18dec03,jtp  SPR 92700 guard against cache disabled cases
02l,18dec03,jtp  SPR 92700 vmBaseLib must honor USER_D_CACHE_MODE
02k,09dec03,jmt  Change call for MIPS MMU init
02j,01dec03,yp   fixing compiler warning resulting from address type change
02i,24nov03,zmm  Revome i86 specific vmArchLibInit calls.
02h,19nov03,tam  added kernel ctx union initialization
02g,17nov03,jtp  Add variable minimum page size to PPC405 & 440 MMU.
02f,10nov03,jmt  Updated as part of MIPS AIM MMU rewrite
02e,23oct03,tam  simplified call to adrSpaceLibInit()
02d,17oct03,gls  added adrSpaceLibInit()
02c,08oct03,tam  fixed dependencies between MMU_BASIC and MMU_FULL
02b,30sep03,jmp  added support for VxSim.
02a,19sep03,pcs  Change the paramer to vmBaseLib from pageSize to
                 cacheDefault.
01z,04sep03,pcs  Add support for MMU_FULL.
01z,03sep03,jmt  Add MIPS R4k Library init
01y,30jun03,tam  initialized vmShow with BASIC MMU support instead of FULL
02a,07jan03,dtr  Adding support for static TLB entries for E500.
01z,25nov02,dtr  Modify for E500.
01y,02aug02,pcs  Add support for PPC85XX and make it the same as PPC603 for
                 the present.
01x,05jun02,hdn  added 36bit MMU support for P6 and P7
01w,17apr02,jtp  support PPC440 MMU
01v,27aug01,hdn  renamed mmuLibInit to mmuI86LibInit for PENTIUM2/3/4
01u,14aug01,hdn  added PENTIUM2/3/4 support
01t,26apr01,scm  remove xscale debug LED code...
01q,20jan99,cdp  prevent MMU initialisation when ARM_HAS_NO_MMU defined;
		 removed support for old ARM libraries.
01p,16nov98,cdp  added BSP selection of MMU for ARM. Added INCLUDE_MMU_MPU.
01n,10nov00,s_m  PPC405 support
01n,02mar00,zl   merged SH support into T2
01m,24aug98,tpr  added PowerPC EC 603 support.
01n,09apr98,hdn  added support for PentiumPro.
01m,28nov96,cdp  added ARM support.
01l,17jun96,tpr  added PowerPC 860 support.
01k,19sep95,tpr  added PowerPC support.
01j,06nov94,tmk  added MC68LC040 support.
01i,16jun94,tpr  added MC68060 cpu support.
01i,13nov93,hdn  added support for I80X86.
01i,02dec93,pme  added Am29K family support.
01h,30jul93,jwt  changed CPU to CPU_FAMILY for SPARC; include sun4.h to BSP.
01g,27feb93,rdc  removed check for both BASIC and FULL (done in usrDepend.c).
01f,13feb93,jcf  changed SPARC mmu initialization to utilize sysMmuLibInit.
01e,16oct92,jwt  included sun4.h to pull in prototype for mmuSun4LibInit().
01d,08oct92,rdc  ensured that both BASIC and FULL aren't set simultaneously.
01c,29sep92,ccc  renamed mmu1eLibInit() to mmuSun4LibInit().
01b,22sep92,rdc  cleanup, added support for sun1e.
01a,18sep92,jcf  written.
*/

/*
DESCRIPTION
This file is used to initialize the memory management unit.  This file is
included by usrConfig.c.

SEE ALSO: usrExtra.c

NOMANUAL
*/

#ifndef  __INCusrMmuInitc
#define  __INCusrMmuInitc

#include <vsbConfig.h>

/* if not defined, default SW_MMU_ENABLE to FALSE (except 32-bit UP MIPS) */

#ifndef SW_MMU_ENABLE
#if (CPU_FAMILY != MIPS) || defined (_WRS_CONFIG_SMP) || \
    defined (_WRS_CONFIG_LP64)
#define SW_MMU_ENABLE	FALSE
#else /* (CPU_FAMILY == MIPS) */
#define SW_MMU_ENABLE	TRUE
#endif /* (CPU_FAMILY != MIPS) */
#endif

#if (SW_MMU_ENABLE == FALSE)


#if	(CPU == PPC85XX)
#include <arch/ppc/mmuE500Lib.h>
#elif	((CPU==PPC601) || (CPU==PPC603) || (CPU==PPCEC603) || (CPU==PPC604) || \
	 (CPU==PPC970))
#include <arch/ppc/mmuPpcLib.h>
#elif	(CPU == PPC860)
#include <arch/ppc/mmu800Lib.h>
#elif 	((CPU == PPC405) || (CPU == PPC405F))
#include <arch/ppc/mmu405Lib.h>
#elif 	((CPU == PPC440) || (CPU == PPC465))
#include <arch/ppc/mmu440Lib.h>
#elif 	((CPU == PPC476))
#include <arch/ppc/mmu476Lib.h>
#elif   (CPU == MCF5400)
#include <arch/coldfire/mmuV4eLib.h>
#elif   (CPU_FAMILY == MIPS)
#include <arch/mips/mmuMipsLib.h>
#elif	(CPU == SIMSPARCSOLARIS)
#include <arch/simsolaris/mmuArchLib.h>
#elif	(CPU == SIMNT)
#include <arch/simnt/mmuArchLib.h>
#elif	(CPU == SIMLINUX)
#include <arch/simlinux/mmuArchLib.h>
#endif	/* CPU */

#else /* SW_MMU_ENABLE */

#if defined (_WRS_CONFIG_SMP) || defined (_WRS_CONFIG_LP64)

/*
 * For VxWorks SMP and 64-bit VxWorks, SW_MMU_ENABLE should never 
 * be defined to TRUE. The following protects against unexpectedly 
 * being defined so.
 */

# error   VxWorks SMP and 64-bit VxWorks require MMU support. Make \
    sure "SW_MMU_ENABLE" is undefined, or defined "FALSE".
#endif /* _WRS_CONFIG_SMP | _WRS_CONFIG_LP64 */

#include <private/mmuSoftLibP.h>

#endif /* SW_MMU_ENABLE */

#if (defined INCLUDE_ADR_SPACE_LIB)
#include <private/adrSpaceLibP.h>
#include <private/kernelBaseLibP.h>
#include <private/rtpLibP.h>
#endif

#if (defined INCLUDE_VM_INVTBL)
#include <private/vmInvTblLibP.h>
#endif

/*
 * Placed extern here instead of including the private file aimMmuLibP.h,
 * which contains the declaration of _func_aimMmuLockRegionsImport, because
 * it includes the header file aimMmuLib.h which will pull in 
 * a redefinition of 'struct mmuTransTblStruct'
 */

extern STATUS (*_func_aimMmuLockRegionsImport)(void);

/* defines */

/*
 * arrange for default cache mode to track USER_D_CACHE_MODE if
 * cache is supported, enabled, and USER_D_CACHE_MODE exists.
 * Otherwise, the VM will leave cache disabled by default.
 */

#if (defined(INCLUDE_CACHE_SUPPORT) && defined(USER_D_CACHE_ENABLE) && \
     defined(USER_D_CACHE_MODE))
# define MMU_DEFAULT_CACHE_MODE		USER_D_CACHE_MODE
#else
# define MMU_DEFAULT_CACHE_MODE		CACHE_DISABLED
#endif

/*
 * For PowerPC CPUs only, check to see if MMU support exists (one of
 * _WRS_TLB_MISS_CLASS_* is defined).
 */

#if ((CPU_FAMILY == PPC) && \
    (defined(_WRS_TLB_MISS_CLASS_SW) || defined(_WRS_TLB_MISS_CLASS_HW)))

 /*
  * MMU support is available for this CPU. Set PPC_MMU_TYPE to identify
  * the CPU-specific code expected by mmuXxxLibInit corresponding to
  * USER_I_MMU_ENABLE and USER_D_MMU_ENABLE.
  * Also INCLUDE_ADR_SPACE_LIB requires both I&D MMU enabled.
  */

# if ((defined(USER_I_MMU_ENABLE) && defined(USER_D_MMU_ENABLE)) || \
      defined(INCLUDE_ADR_SPACE_LIB))
#  define PPC_MMU_TYPE	(MMU_INST | MMU_DATA)
# elif (defined(USER_I_MMU_ENABLE))
#  define PPC_MMU_TYPE	(MMU_INST)
# elif (defined(USER_D_MMU_ENABLE))
#  define PPC_MMU_TYPE	(MMU_DATA)
# else
#  define PPC_MMU_TYPE	(0)
# endif /* USER_I_MMU_ENABLE, USER_D_MMU_ENABLE */

#endif /* CPU_FAMILY == PPC && _WRS_TLB_MISS_CLASS_* */

/* externals */

IMPORT FUNCPTR sysMmuLibInit;		/* board-specific MMU library */
IMPORT UINT32 sysBatDesc [];
IMPORT PHYS_MEM_DESC * pAsMemDesc;
IMPORT int asMemDescNumEnt;
IMPORT char * memPoolEndAdrs;

#if (SW_MMU_ENABLE == FALSE)
IMPORT PHYS_MEM_DESC sysPhysMemDesc [];
IMPORT int sysPhysMemDescNumEnt;
#define MEM_DESC		sysPhysMemDesc
#define MEM_DESC_NUM_ENT	sysPhysMemDescNumEnt
#else
IMPORT PHYS_MEM_DESC mmuSoftMemDesc [];
IMPORT int mmuSoftMemDescNumEnt;
#define MEM_DESC		mmuSoftMemDesc
#define MEM_DESC_NUM_ENT	mmuSoftMemDescNumEnt
#endif

#ifdef INCLUDE_ADR_SPACE_LIB
LOCAL ADR_SPACE_INIT_PARAMS asInitParam;
#endif

#if ((CPU == PPC440) || (CPU==PPC85XX) || (CPU == PPC476) || (CPU == PPC465))
#if (SW_MMU_ENABLE == FALSE)
IMPORT TLB_ENTRY_DESC sysStaticTlbDesc [];
IMPORT int sysStaticTlbDescNumEnt;
#endif
#endif

/*******************************************************************************
*
* usrMmuInit - initialize the memory management unit
*
* NOMANUAL
*/

void usrMmuInit 
    (
    VIRT_ADDR 		memPoolStartAdrs,
    unsigned		memPoolSize
    )
    {
    UINT          err;
    UINT          ix;
    char *        pMsg;
    static char * pErrorMsg = 
		    "usrRoot: MMU configuration failed, errno = 0x00000000\n";
    static char   digits[] = "0123456789abcdef";
    

#if (SW_MMU_ENABLE == FALSE)

#if (CPU_FAMILY == SPARC)

    /* BSP selection of cache library */

    if ((sysMmuLibInit == NULL) || (((* sysMmuLibInit) (VM_PAGE_SIZE)) != OK))
	goto usrMmuPanic;
#endif /* CPU_FAMILY == SPARC */

#ifdef	_WRS_ARCH_IS_SIMULATOR
    if (mmuArchLibInit (VM_PAGE_SIZE) != OK)
	goto usrMmuPanic;
#endif	/* _WRS_ARCH_IS_SIMULATOR */


#if (CPU_FAMILY==I80X86)
#   if defined(_WRS_CONFIG_LP64)
    if (mmuX86_64LibInit (VM_PAGE_SIZE) != OK)
        goto usrMmuPanic;
        
#   elif ((CPU != PENTIUM) && defined(INCLUDE_MMU_P6_32BIT))

    if (mmuPro32LibInit (VM_PAGE_SIZE) != OK)
        goto usrMmuPanic;

#   elif ((CPU != PENTIUM) && defined(INCLUDE_MMU_P6_36BIT))

    if (mmuPro36LibInit (VM_PAGE_SIZE) != OK)
        goto usrMmuPanic;

#   else

    if (mmuI86LibInit (VM_PAGE_SIZE) != OK)
        goto usrMmuPanic;

#   endif /* defined(_WRS_CONFIG_LP64) */
#endif /* CPU==I80X86 */


#if (CPU_FAMILY==SH)
    if (mmuShLibInit (VM_PAGE_SIZE) != OK)
	goto usrMmuPanic;
#endif /* CPU_FAMILY==SH */

#if ((CPU_FAMILY == PPC) && defined(PPC_MMU_TYPE))
# if	(CPU == PPC85XX)
    if (mmuE500LibInit (PPC_MMU_TYPE, sysStaticTlbDescNumEnt,
                        &sysStaticTlbDesc[0]) != OK)
	goto usrMmuPanic;
# elif	((CPU==PPC601) || (CPU==PPC603) || (CPU==PPCEC603) || (CPU==PPC604) || \
	 (CPU==PPC970))
    if (mmuPpcLibInit (PPC_MMU_TYPE, (int *) MEM_DESC, MEM_DESC_NUM_ENT,
#  ifdef  _WRS_MMU_NO_BAT
			(int *) NULL,
#  else   /* _WRS_MMU_NO_BAT */
			(int *) &sysBatDesc [0],
#  endif  /* _WRS_MMU_NO_BAT */
			USER_HASH_TABLE_ADDRESS, USER_HASH_TABLE_SIZE) != OK)
	goto usrMmuPanic;
# elif	(CPU == PPC860)
    if (mmu800LibInit (PPC_MMU_TYPE) != OK)
	goto usrMmuPanic;
# elif	((CPU == PPC405) || (CPU == PPC405F))
    if (mmu405LibInit (PPC_MMU_TYPE, VM_PAGE_SIZE) != OK)
	goto usrMmuPanic;
# elif	((CPU == PPC440) || (CPU == PPC465))
    if (mmu440LibInit (PPC_MMU_TYPE, sysStaticTlbDescNumEnt,
	    &sysStaticTlbDesc[0], VM_PAGE_SIZE) != OK)
	goto usrMmuPanic;
# elif	(CPU == PPC476)
    if (mmu476LibInit (PPC_MMU_TYPE, sysStaticTlbDescNumEnt,
	    &sysStaticTlbDesc[0], VM_PAGE_SIZE) != OK)
	goto usrMmuPanic;
# else
#  error  "PowerPC MMU Library initialization function unknown for this CPU"
# endif /* CPU */
#endif	/* CPU_FAMILY == PPC && defined(PPC_MMU_TYPE) */

#if (CPU_FAMILY == MIPS)
    if (mmuMipsLibInit ((VIRT_ADDR)AIM_MMU_INIT_MEM_POOL_VIRT_ADRS, 
                        (size_t)AIM_MMU_INIT_MEM_POOL_SIZE) != OK)
	goto usrMmuPanic;
#endif /* CPU_FAMILY == MIPS */
    
#if (CPU == MCF5400)
    if (mmuV4eLibInit () != OK)
	goto usrMmuPanic;
#endif /* CPU == MCF5400 */

#if (CPU_FAMILY == ARM)
    /* BSP selection of MMU library */

    if ((sysMmuLibInit == NULL) || (((* sysMmuLibInit) (VM_PAGE_SIZE)) != OK))
	goto usrMmuPanic;
#endif /* (CPU_FAMILY == ARM) */

#else	/* SW_MMU_ENABLE */

    if (mmuSoftLibInit (VM_PAGE_SIZE, (VIRT_ADDR) LOCAL_MEM_LOCAL_ADRS,
			(VIRT_ADDR) memPoolEndAdrs) != OK)
	goto usrMmuPanic;

# if ((CPU_FAMILY == PPC) && (defined(INCLUDE_CACHE_SUPPORT)) \
      && (USER_D_CACHE_MODE != CACHE_DISABLED) \
      && (CPU != PPC403) && (CPU != PPC440) && (CPU != PPC85XX) && (CPU != PPC476) \
	  && (CPU != PPC465))
    /*
     * The data cache was enabled when usrInit() called cacheEnable(), but for
     * most PPC it will interfere with I/O operations when using mmuSoftLib
     * instead of a real MMU and therefore must be turned off before I/O can
     * safely be set up.  Exceptions are 440/476/85xx (Book E, whose h/w MMU is
     * always on), and 403 (whose cacheability control does not involve the
     * MMU -- VxWorks does not support the 403 MMU even if present).
     */
    cacheDisable(DATA_CACHE);
# endif /* (CPU_FAMILY == PPC) */

#endif	/* SW_MMU_ENABLE */

#ifdef INCLUDE_MMU_BASIC
    if (vmBaseLibInit() != OK)
	goto usrMmuPanic;
#endif

#ifdef INCLUDE_VM_RTP			/* VM support for RTPs */
    if (vmLibInit () != OK)
	goto usrMmuPanic;

#endif	/* INCLUDE_VM_RTP */

#ifdef INCLUDE_VM_INVTBL
    if (vmInvTblLibInit() != OK)
	goto usrMmuPanic;
#endif

#ifdef	INCLUDE_ADR_SPACE_LIB

#ifdef _WRS_CONFIG_USE_MEMDESC
    /*
     * This applies to VxWorks 64-bit only at this time. For RTPs the 
     * overlapped virtual memory feature is compulsory and the base
     * address and size of the code region is fixed per architecture.
     */

    asInitParam.localMemLocalAdrs  = LOCAL_MEM_LOCAL_ADRS;
#else /* _WRS_CONFIG_USE_MEMDESC */
    asInitParam.pPhysMemDesc       = MEM_DESC;
    asInitParam.physMemDescNumEnt  = MEM_DESC_NUM_ENT;
    asInitParam.localMemLocalAdrs  = LOCAL_MEM_LOCAL_ADRS;
    asInitParam.kernelMemTop       = (VIRT_ADDR) memPoolEndAdrs;
    asInitParam.globalVirtMemRgnSize = GLOBAL_VIRT_MEM_REGION_SIZE;
#ifdef INCLUDE_RTP
    asInitParam.rtpRgnBase         = RTP_CODE_REGION_START;
    asInitParam.rtpRgnSize         = RTP_CODE_REGION_SIZE;
    asInitParam.rtpRgnIsOverlapped = RTP_OVERLAPPED_ADDRESS_SPACE;
#else
    asInitParam.rtpRgnBase         = 0;
    asInitParam.rtpRgnSize         = 0;
    asInitParam.rtpRgnIsOverlapped = FALSE;
#endif /* RTP */
#endif /* _WRS_CONFIG_USE_MEMDESC */

    if (adrSpaceLibInit (&asInitParam) != OK)
	goto usrMmuPanic;

#ifdef _WRS_CONFIG_LP64

    /* 
     * For 64-bit VxWorks page optimization is enabled or disabled globally.
     * When enabled, all mappings performed in either kernel or RTP context
     * will be performed automatically with page optimization.
     */

#ifdef INCLUDE_PAGE_SIZE_OPTIMIZATION
    VM_PG_OPTIMIZE = TRUE;
#endif
    
#endif /* _WRS_CONFIG_LP64 */

    if (vmGlobalMapInit (pAsMemDesc, asMemDescNumEnt, TRUE, 
			 MMU_DEFAULT_CACHE_MODE) == NULL)
	goto usrMmuPanic;

#else  /* INCLUDE_ADR_SPACE_LIB */

    if (vmGlobalMapInit (MEM_DESC, MEM_DESC_NUM_ENT, TRUE,
			 MMU_DEFAULT_CACHE_MODE) == NULL)
	goto usrMmuPanic;

#endif  /* INCLUDE_ADR_SPACE_LIB */

    /*
     * Now that the global map has been configured, allow the AIM to
     * import any locked memory regions.  The MIPS architectures sets
     * up wired TLB entries for the kernel address space; this hook
     * allows the AIM to properly set the lock bits in the PTEs to
     * reflect the already-configured hardware state.
     */

    if (_func_aimMmuLockRegionsImport != NULL)
	_func_aimMmuLockRegionsImport ();

#if defined (INCLUDE_SHOW_ROUTINES) || defined (INCLUDE_VM_SHOW)
    vmShowInit ();
#endif /* INCLUDE_SHOW_ROUTINES */

    return;

usrMmuPanic:

    /* save error message with errno in system exception msg area */

    pMsg = pErrorMsg + strlen (pErrorMsg) - 2;
    err  = errno;

    for (ix = 0; ix < 8; ix++)
	{
	*pMsg-- = digits[err & 0xf];
	err >>= 4;
	}

    pMsg = pErrorMsg;

    while ((*(sysExcMsg++) = *(pMsg++)) != EOS)
	;

    /* 
     * Use ED&R to inject an error. In case ED&R is not included or not yet 
     * initialized, the policy stub still will be called.
     */

    EDR_INIT_FATAL_INJECT (NULL, NULL, NULL, NULL, pErrorMsg);

    /* if EDR stub returned somehow... */

    reboot (BOOT_WARM_NO_AUTOBOOT);
    }

#endif	/* __INCusrMmuInitc */
