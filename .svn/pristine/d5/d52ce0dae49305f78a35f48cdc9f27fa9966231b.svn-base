/* usrCoreDump.c - configuration file for the Core Dump facility */

/*
 * Copyright (c) 2005-2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute or otherwise make use of this software
 * may be licensed only pursuant to the terms of an applicable Wind River
 * license agreement. No license to Wind River intellectual property rights
 * is granted herein. All rights not licensed by Wind River are reserved
 * by Wind River.
 */

/*
modification history
--------------------
02j,20apr12,jmp  CQID: Fix WIND00346084 : Updated coreDumpCksum() call.
02i,12apr11,jmp  Added coreDumpMemPartRegister() calls to register system
                 partition and proximity heap partition for free blocks
		 filtering.
02h,30mar11,jmp  Fixed build warning.
02g,18mar11,jmp  Updated user reserved memory filtering for LP64.
02f,08mar11,jmp  Remorked usrCoreDumpRtInfoGet() to no longer call
                 adrSpaceInfoGet() during core dump generation.
02e,02feb10,jmp  Reworked physical memory handling for LP64 support.
02d,16nov10,jmp  Added CORE_DUMP_STACK_SIZE validation vs the size of the core
		 dump memory pool pages (CQ:WIND00240273).
02c,04jun10,jmp  Added a parameter to control the minimal block size for the
                 free block filtering mechanism.
02b,11dec09,jmp  Partial LP64 code adaptation.
02a,21aug09,rlp  Fixed the computation of the target memory size reported to
                 the host (WIND00178199).
01z,15jul09,jmp  Added INCLUDE_CORE_DUMP_RTP_FS support.
01y,16jul08,jmp  Allow to select different compression methods for Kernel and
		 RTP Core Dump.
01x,10jul08,jmp  Resolved Kernel and RTP Core Dump coexistence issue.
		 Made usrCoreDumpRtpInit() local.
01w,22may08,jmp  Added usrCoreDumpRtpInit (REQ CQ:WIND00005075).
		 Resolved some dependencies issues.
01v,14apr08,jmp  Cleaned-up core dump device init (REQ CQ:WIND00005075).
01u,21feb08,jmp  Reserved one core dump hook for core dump generator.
01t,06jul07,p_b  Added 64bits support
01s,18jun07,jmp  Updated coreDumpMemPoolLibInit() call to pass the
                 CORE_DUMP_MEM_POOL_PROTECTION_ENABLE parameter.
01r,11jun07,dbt  Fixed core dump memory pool page size usage.
01q,23oct06,jmp  added core dump memory pool initialization.
		 updated to follow core dump filtering enhancements.
		 moved static arrays to memory pool.
01p,11jul06,jmp  fixed number of system filters (Defect #54853).
		 fixed GNU build warning (Defect #30434).
01o,10jul06,rlp  Replaced VX_CPU_FAMILY by CPU_FAMILY (Defect #37561).
01n,22jun06,jmp  added support for INCLUDE_CORE_DUMP_UTIL component,
		 and for INCLUDE_CORE_DUMP_STREAM_DEV.
01m,13mar06,jmp  added specific handling for MIPS targets configured with
		 INCLUDE_MAPPED_KERNEL (SPR #118953).
01l,16feb06,jmp  fixed coredump compression algorithm selection (SPR #117856).
01k,02dec05,jmp  added RLE compression support.
01j,24nov05,jmp  added CORE_DUMP_SKIP_USER_RESERVED_MEM handling.
		 excluded core dump stack from core dump.
01i,07sep05,jmp  Added alternate core dump stack support.
		 Provide the ability to install core dump filters before
		 usrCoreDumpInit() call.
01i,07sep05,dbt  Updated TGT_INFO structure; Added ability to skip text
                 section.
01h,16aug05,dbt  Removed ED&R hack. Fill device size field in case of memory
                 storage.
01g,10aug05,dbt  Fixed post-mortem area usage.
01f,05aug05,jmp  modified sysCoreDumpDevInit() call to pass a pointer to a
                 SYS_CORE_DUMP_DESC structure.
01e,04aug05,jmp  added sysCoreDumpDevInit() declaration.
01d,03aug05,dbt  Added support for core dump create hooks & coprocessor
                 support.
01c,01aug05,jmp  added memory filtering facility.
		 updated following coreDumpDevInit() change.
01b,27jul05,jmp  added core dump compression level.
01a,01jul05,jmp  written.
*/

/*
DESCRIPTION

This file contains the code to configure the core dump facility.
*/

/* includes */

#include <coreDumpLib.h>
#include <private/coreDumpLibP.h>
#include <pmLib.h>

/* defines */

/* This two macros transform TOOL define into the string "TOOL" */

#define MKSTR(MACRO) MKSTR_FIRST_PASS(MACRO)
#define MKSTR_FIRST_PASS(MACRO) #MACRO

/*
 * Reserve some filters for internal use:
 *	- Peristent Memory filter
 *	- Text section filter
 *	- USER_RESERVED_MEM filter
 *	- Core Dump Memory Pool
 */

#define CORE_DUMP_MEM_FILTER_SYSTEM_COUNT	100

/*
 * Reserve a core dump creation hook slot for internal use. It is reserved for
 * usrCoreDumpCreateHook() which is used to save coprocessors, to install
 * filters for the biggest free blocks, ...
 */

#define CORE_DUMP_HOOK_SYSTEM_COUNT		1

/*
 * Define the minimal block size for the free block filtering mechanism.
 * Free blocks smaller than CORE_DUMP_FREE_BLOCK_MIN_SIZE won't be filtered.
 */

#define CORE_DUMP_FREE_BLOCK_MIN_SIZE		1024

#define	PRINT_ERR \
    if (_func_printErr != NULL) _func_printErr

/* externals */

extern STATUS   sysCoreDumpRawDevInit      (CORE_DUMP_RAW_DEV_DESC * pDesc);
extern char	wrs_kernel_text_start [];
extern char	wrs_kernel_text_end [];

/* globals */

/* locals */

#ifndef _WRS_CONFIG_USE_MEMDESC
LOCAL PHYS_ADDR	coreDumpSysMemBaseAdrs;		/* Physical memory base addr */
LOCAL size_t	coreDumpSysPhysMemSize;		/* Physical memory size */
#endif	/* !_WRS_CONFIG_USE_MEMDESC */

/* forward static declarations */

LOCAL STATUS	usrCoreDumpCommonInit	(void);
#ifdef INCLUDE_CORE_DUMP
LOCAL STATUS	usrCoreDumpKernelInit	(void);
LOCAL STATUS	usrCoreDumpCreateHook	(void);
LOCAL STATUS	usrCoreDumpMemFilterSysInstall (void);
#endif /* INCLUDE_CORE_DUMP */
LOCAL void	usrCoreDumpRtInfoGet	(TGT_INFO_NOTE * pRtInfo);
#ifdef INCLUDE_CORE_DUMP_RTP
LOCAL STATUS	usrCoreDumpRtpInit	(void);
#endif	/* INCLUDE_CORE_DUMP_RTP */
#ifdef INCLUDE_CORE_DUMP_MEM
LOCAL STATUS	usrCoreDumpMemDevInit	(void);
#endif /* INCLUDE_CORE_DUMP_MEM */
#ifdef INCLUDE_CORE_DUMP_RAW_DEV
LOCAL STATUS	usrCoreDumpRawDevInit	(void);
#endif	/* INCLUDE_CORE_DUMP_RAW_DEV */
LOCAL BOOL	usrCoreDumpPhysMemCheck (PHYS_ADDR paddr, size_t size,
					 PHYS_ADDR * pBlockPaddr,
					 size_t * pBlockSize);
LOCAL STATUS	usrCoreDumpPhysMemDump	(FUNCPTR pDumpRtn, void * pKernelIf,
					 UINT32 state, UINT32 alignment);
LOCAL BOOL	usrCoreDumpPhysMemTest	(PHYS_ADDR physicalAddr, size_t len,
					 PHYS_ADDR paddr, size_t size,
					 PHYS_ADDR * pBlockPaddr,
					 size_t * pBlockSize);

#ifdef	_WRS_CONFIG_LP64
LOCAL ULONG adrSpacePhysSize;	/* Address space physical memory size */
#endif	/* _WRS_CONFIG_LP64 */

/*******************************************************************************
*
* usrCoreDumpInit - Core Dump initialization
*
* Entry point for Kernel and RTP core dump initialization.
*
* RETURNS: OK, or ERROR if initialization failed.
*
* ERRNO: N/A
*
*/

STATUS usrCoreDumpInit (void)
    {
    static STATUS usrCoreDumpInitStatus = 1;	/* usrCoreDumpInit status */

    /*
     * First test usrCoreDumpInit status: 1 means that usrCoreDumpInit()
     * has never been called.
     */

    if ((usrCoreDumpInitStatus == OK) ||  (usrCoreDumpInitStatus == ERROR))
	{
	/*
	 * usrCoreDumpInit() has already been alled, return result of last
	 * call.
	 * NOTE: usrCoreDumpInit() can be call twice since the Kernel and the
	 * RTP Core Dump components do use the same initialization routine, we
	 * must make sure that it is called only once.
	 */

	return (usrCoreDumpInitStatus);
	}

    if (usrCoreDumpCommonInit () == ERROR)
	{
	usrCoreDumpInitStatus = ERROR;
	goto usrCoreDumpInitEnd;
	}

#ifdef INCLUDE_CORE_DUMP
    if (usrCoreDumpKernelInit () == ERROR)
	{
	usrCoreDumpInitStatus = ERROR;
	goto usrCoreDumpInitEnd;
	}
#endif	/* INCLUDE_CORE_DUMP */

#ifdef INCLUDE_CORE_DUMP_RTP
    if (usrCoreDumpRtpInit () == ERROR)
	{
	usrCoreDumpInitStatus = ERROR;
	goto usrCoreDumpInitEnd;
	}
#endif	/* INCLUDE_CORE_DUMP_RTP */

#ifdef INCLUDE_CORE_DUMP_MEM
    if (usrCoreDumpMemDevInit () == ERROR)
	{
	usrCoreDumpInitStatus = ERROR;
	goto usrCoreDumpInitEnd;
	}
#endif	/* INCLUDE_CORE_DUMP_MEM */

#ifdef INCLUDE_CORE_DUMP_RAW_DEV

    /* INCLUDE_CORE_DUMP_RAW_DEV is deprecated: generate a warning */

    #warning "INCLUDE_CORE_DUMP_RAW_DEV component is deprecated! \
		Please use INCLUDE_CORE_DUMP_USER_DEV instead."

    if (usrCoreDumpRawDevInit () == ERROR)
	{
	usrCoreDumpInitStatus = ERROR;
	goto usrCoreDumpInitEnd;
	}
#endif	/* INCLUDE_CORE_DUMP_RAW_DEV */

#if !defined(PRJ_BUILD)
    /*
     * For BSP build (only), we must invoke the init routine for the user
     * provided device driver.
     * The init routine must be specified using CORE_DUMP_USER_DEV_INIT_RTN
     */

#ifdef INCLUDE_CORE_DUMP_USER_DEV
    if (CORE_DUMP_USER_DEV_INIT_RTN () == ERROR)
	{
	usrCoreDumpInitStatus = ERROR;
	goto usrCoreDumpInitEnd;
	}
#endif	/* INCLUDE_CORE_DUMP_USER_DEV */
#endif	/* !PRJ_BUILD */

#ifdef INCLUDE_CORE_DUMP_UTIL
    if (coreDumpUtilLibInit () == ERROR)
	{
	usrCoreDumpInitStatus = ERROR;
	goto usrCoreDumpInitEnd;
	}
#endif	/* INCLUDE_CORE_DUMP_UTIL */

#ifdef INCLUDE_CORE_DUMP_SHOW
    if (coreDumpShowInit () == ERROR)
	{
	usrCoreDumpInitStatus = ERROR;
	goto usrCoreDumpInitEnd;
	}
#endif	/* INCLUDE_CORE_DUMP_SHOW */

#if (CORE_DUMP_MEM_POOL_PROTECTION_ENABLE == TRUE)
    if (coreDumpMemPoolProtectEnable (TRUE) == ERROR)
	{
	usrCoreDumpInitStatus = ERROR;
	goto usrCoreDumpInitEnd;
	}
#endif	/* CORE_DUMP_MEM_POOL_PROTECTION_ENABLE == TRUE */

    usrCoreDumpInitStatus = OK;		/* Initialization successful */

usrCoreDumpInitEnd:
    return (usrCoreDumpInitStatus);
    }

/*******************************************************************************
*
* usrCoreDumpCommonInit - common initialization for RTP and Kernel Core Dump
*
* This routine contains the initialization code common between RTP and
* Kernel Core Dump facilities.
*
* RETURNS: OK, or ERROR if initialization failed.
*
* ERRNO: N/A
*
*/

LOCAL STATUS usrCoreDumpCommonInit (void)
    {
#ifdef _WRS_CONFIG_LP64
    ADR_SPACE_INFO adrsSpaceInfo;
#endif
    UINT32	coreDumpMemPoolPgSize;

#ifdef _WRS_CONFIG_LP64
    /*
     * In 64-bit, RAM may consist of a number of discontiguous blocks.
     * Compute the physical memory size from information returned by
     * adrSpaceInfoGet().
     * NOTE: Since adrSpaceInfoGet() is performing system calls, it must never
     * been called during core dump generation.
     */

    if (adrSpaceInfoGet (&adrsSpaceInfo) == ERROR)
	adrSpacePhysSize = 0;
    else
	adrSpacePhysSize = adrsSpaceInfo.physTotalPages *
					adrsSpaceInfo.physAllocUnit;
#endif	/* _WRS_CONFIG_LP64 */

#if (CORE_DUMP_MEM_POOL_PROTECTION_ENABLE == TRUE)
    if (VM_INSTALLED)
    	{
	/*
	 * The core dump memory pool page size must be aligned on a Virtual
	 * Memory page size if core dump memory pool protection is enabled.
	 */

	coreDumpMemPoolPgSize = ROUND_UP (CORE_DUMP_MEM_POOL_PAGE_SIZE,
					  VM_PAGE_SIZE_GET ());
	}
    else
#endif	/* (CORE_DUMP_MEM_POOL_PROTECTION_ENABLE == TRUE) */
	coreDumpMemPoolPgSize = CORE_DUMP_MEM_POOL_PAGE_SIZE;

    /* initialize core dump memory pool library for dynamic allocation */

    if (coreDumpMemPoolLibInit (NULL, coreDumpMemPoolPgSize,
				CORE_DUMP_MEM_POOL_PROTECTION_ENABLE) == ERROR)
	return (ERROR);

    return (OK);
    }

#ifdef INCLUDE_CORE_DUMP_MEM
/*******************************************************************************
*
* usrCoreDumpMemDevInit - initialize persistent memory RAW device driver
*
* This routine initializes the kernel core dump facility to use the Persistent
* Memory as Core Dump RAW device.
*
* RETURNS: OK, or ERROR if failed to initialize PM core dump support.
*
* ERRNO: N/A
*
* NOMANUAL
*/

LOCAL STATUS usrCoreDumpMemDevInit (void)
    {
    CORE_DUMP_RAW_DEV_DESC	coreDumpRawDevDesc;

    /*
     * Persistent memory must be initialized before calling
     * coreDumpMemDevInit() since by default CORE_DUMP_REGION_SIZE relies
     * on PM library initialization.
     */

    if (pmInit (NULL) == ERROR)
	{
	PRINT_ERR ("Failed to initialize Post Mortem area for core dump.\n");
	return (ERROR);
	}

    /* Initialize Core Dump Persistent Memory device */

    if (coreDumpMemDevInit (&coreDumpRawDevDesc,
			    CORE_DUMP_REGION_SIZE) == ERROR)
	{
	PRINT_ERR ("Failed to create core dump memory storage "
		   "(size = %#lx)\n", CORE_DUMP_REGION_SIZE);
	return (ERROR);
	}
    
    return (OK);
    }
#endif	/* INCLUDE_CORE_DUMP_MEM */

#ifdef INCLUDE_CORE_DUMP_RAW_DEV
/*******************************************************************************
*
* usrCoreDumpRawDevInit - initialize user provided RAW device driver
*
* This routine initializes the core dump raw device support to use the
* user provided RAW device driver.
*
* NOTE: INCLUDE_CORE_DUMP_RAW_DEV component is deprecated, it is now
* recommended to use INCLUDE_CORE_DUMP_USER_DEV instead. Building a
* project with INCLUDE_CORE_DUMP_RAW_DEV will generate a build warning.
*
* RETURNS: OK, or ERROR if failed to initialize user provided RAW device driver.
*
* ERRNO: N/A
*
* NOMANUAL
*/

LOCAL STATUS usrCoreDumpRawDevInit (void)
    {
    CORE_DUMP_RAW_DEV_DESC	coreDumpRawDevDesc;

    /* Initialized all the fields of the device descriptor to NULL */

    memset (&coreDumpRawDevDesc, 0, sizeof (CORE_DUMP_RAW_DEV_DESC));

    /* Call the user proviced initialization routine */

    if (sysCoreDumpRawDevInit (&coreDumpRawDevDesc) == ERROR)
	return (ERROR);

    /* initialize core dump raw device support to use user provided driver */

    if (coreDumpRawDevInit (&coreDumpRawDevDesc) == ERROR)
	return (ERROR);

    return (OK);
    }
#endif	/* INCLUDE_CORE_DUMP_RAW_DEV */

#ifdef INCLUDE_CORE_DUMP
/*******************************************************************************
*
* usrCoreDumpKernelInit - initialize the kernel core dump support
*
* This routine initializes the kernel core dump facility.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
*/

LOCAL STATUS usrCoreDumpKernelInit (void)
    {
#ifndef _WRS_CONFIG_USE_MEMDESC
#if (defined (INCLUDE_MAPPED_KERNEL) && (CPU_FAMILY==MIPS))
    /*
     * On MIPS architecture when INCLUDE_MAPPED_KERNEL component is included,
     * LOCAL_MEM_LOCAL_ADRS must be considered as a virtual address, which
     * means that it has to be translated to a physical address.
     * For more details, refer to the "Interface Variations" and to the
     * "Architecture Considerations" sections of the MIPS Architecture
     * Supplement Guide.
     */

    if (vmTranslate (NULL, (VIRT_ADDR)LOCAL_MEM_LOCAL_ADRS,
		     &coreDumpSysMemBaseAdrs) == ERROR)
	return (ERROR);
#else
    coreDumpSysMemBaseAdrs = (PHYS_ADDR) LOCAL_MEM_LOCAL_ADRS;
#endif
    coreDumpSysPhysMemSize = (size_t)(sysPhysMemTop() - LOCAL_MEM_LOCAL_ADRS);
#endif	/* !_WRS_CONFIG_USE_MEMDESC */

    /*
     * Verify CORE_DUMP_STACK_SIZE validity Vs CORE_DUMP_MEM_POOL_PAGE_SIZE
     *
     * The core dump stack is allocated from the core dump memory pool,
     * this pool is composed of one or several pages which size are defined
     * by CORE_DUMP_MEM_POOL_PAGE_SIZE. This means that it is not possible
     * to allocate buffers that are bigger than CORE_DUMP_MEM_POOL_PAGE_SIZE
     * (less a few bytes for page header).
     * Moreover, as handling a unique pool page is optimum for core dump
     * generator, we impose the following rule which means that the core
     * dump stack size must be lower than the half of the core dump pool
     * page size.
     */

#if (CORE_DUMP_STACK_SIZE > (CORE_DUMP_MEM_POOL_PAGE_SIZE / 2))
#error "CORE_DUMP_STACK_SIZE must be lower than CORE_DUMP_MEM_POOL_PAGE_SIZE/2"
#endif

    /* initialize core dump core support */

    if (coreDumpInit (CORE_DUMP_STACK_SIZE,
		      (CORE_DUMP_RT_INFO_GET) usrCoreDumpRtInfoGet,
		      usrCoreDumpPhysMemCheck,
		      usrCoreDumpPhysMemDump,
		      CORE_DUMP_CKSUM_ENABLE) == ERROR)
	return (ERROR);

    /* initialize core dump memory filtering library */

    if (coreDumpMemFilterLibInit (CORE_DUMP_MEM_FILTER_MAX,
				  CORE_DUMP_MEM_FILTER_SYSTEM_COUNT) == ERROR)
	return (ERROR);

    /* Initialize core dump create hook facility */

    if (coreDumpHookInit (CORE_DUMP_MAX_HOOKS +
			  CORE_DUMP_HOOK_SYSTEM_COUNT) == ERROR)
	return (ERROR);

    /* initialize core dump compression facility */

#ifdef INCLUDE_CORE_DUMP_COMPRESS
#if ((CORE_DUMP_COMPRESSION_LEVEL < 1 ) || (CORE_DUMP_COMPRESSION_LEVEL > 9))
#error "Invalid value for CORE_DUMP_COMPRESSION_LEVEL parameter"
#endif
    if (coreDumpZLibInit (CORE_DUMP_DEVICE_KERNEL,
			  CORE_DUMP_COMPRESSION_LEVEL) == ERROR)
	return (ERROR);
#elif defined(INCLUDE_CORE_DUMP_COMPRESS_RLE)
    if (coreDumpRleInit (CORE_DUMP_DEVICE_KERNEL) == ERROR)
	return (ERROR);
#endif	/* !INCLUDE_CORE_DUMP_COMPRESS && !INCLUDE_CORE_DUMP_COMPRESS_RLE */

    /* Install core dump create hook */

    if (coreDumpCreateHookAdd (usrCoreDumpCreateHook) == ERROR)
    	return (ERROR);

    /* Core dump system filters installation. */

    if (usrCoreDumpMemFilterSysInstall () == ERROR)
	return (ERROR);

    /*
     * Register system and proximity heap memory partitions; the core dump
     * generator will walk through those partitions to install filters for
     * the biggest free blocks.
     */

    if (coreDumpMemPartRegister (memSysPartId) == ERROR)
	return (ERROR);

#ifdef	_WRS_CONFIG_LP64
    if (coreDumpMemPartRegister (kProxHeapPartId) == ERROR)
	return (ERROR);
#endif	/* _WRS_CONFIG_LP64 */

    return (OK);
    }

/*******************************************************************************
*
* usrCoreDumpMemFilterSysInstall - install system core dump filters
*
* This routine installs the core dump filters used by the core dump facility
* itself.
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A
*
*/

LOCAL STATUS usrCoreDumpMemFilterSysInstall (void)
    {
    /* install filter(s) for core dump memory pool page(s) */

    if (coreDumpMemPoolFiltersInstall () == ERROR)
	return (ERROR);

#if	(CORE_DUMP_SKIP_TEXT_SECTION == TRUE)
    /* Filter VxWorks text section */

    if (coreDumpMemFilterInstall  (wrs_kernel_text_start,
				   wrs_kernel_text_end - wrs_kernel_text_start,
				   CORE_DUMP_SYSTEM_FILTER) == ERROR)
	return (ERROR);
#endif	/* CORE_DUMP_SKIP_TEXT_SECTION == TRUE */

#if	(CORE_DUMP_SKIP_USER_RESERVED_MEM == TRUE)
    /* Filter User Reserved Memory */

#ifndef _WRS_CONFIG_LP64
#if (USER_RESERVED_MEM != 0)
    if (coreDumpMemFilterInstall  (sysMemTop(), USER_RESERVED_MEM,
				   CORE_DUMP_SYSTEM_FILTER) == ERROR)
	return (ERROR);
#endif	/* USER_RESERVED_MEM != 0 */
#else	/* _WRS_CONFIG_LP64 */
#ifdef	INCLUDE_USER_RESERVED_MEMORY
    {
    char * userReservedAddr;
    size_t userReservedSize = 0;

    userReservedGet (&userReservedAddr, &userReservedSize);

    if (userReservedSize != 0)
	{
	if (coreDumpMemFilterInstall  (userReservedAddr, userReservedSize,
				       CORE_DUMP_SYSTEM_FILTER) == ERROR)
	    return (ERROR);
	}
    }
#endif	/* INCLUDE_USER_RESERVED_MEMORY */
#endif	/* _WRS_CONFIG_LP64 */
#endif	/* CORE_DUMP_SKIP_USER_RESERVED_MEM == TRUE */

    return (OK);
    }

/*******************************************************************************
*
* usrCoreDumpCreateHook - core dump create hook
*
* This routine is called every time a core dump is created
*
* RETURNS: OK or ERROR
*
*/

LOCAL STATUS usrCoreDumpCreateHook (void)
    {
#ifdef	INCLUDE_COPROCESSOR
    /* Save all coprocessors to tasks TCB */

    coprocSave (0);
#endif	/* INCLUDE_COPROCESSOR */

    /* Install filters for the biggest free blocks */

    if (coreDumpFreeBlockFilterAdd (CORE_DUMP_FREE_BLOCK_MIN_SIZE) == ERROR)
	return (ERROR);

    return (OK);
    }
#endif	/* INCLUDE_CORE_DUMP */

#ifdef	INCLUDE_COPROCESSOR
/******************************************************************************
* 
* coreDumpTgtHasCoprocessor - check supported coprocessors
*
* This function's result contains bit fields for each 
* co-processor. Coprocessors include floating-point, Altivec (PPC),
* SPE (PPC), and DSP (SH).
*
* RETURNS: a bit mask with supported coprocessors.
*
* NOMANUAL
*/ 

LOCAL UINT32 coreDumpTgtHasCoprocessor (void)
    {
    UINT32	result = 0;
    UINT32	descriptorMasks [COPROCS_MAX];
    int		ix;

    for (ix = 0; ix < COPROCS_MAX; ++ix)	/* reset descriptor masks */
    	descriptorMasks [ix] = 0;

    coprocsDiscover (descriptorMasks);		/* probe coprocessors */

    /* do an union of all descriptor masks */

    for (ix = 0; ix < COPROCS_MAX && descriptorMasks [ix] != 0; ++ix)
    	result += descriptorMasks [ix];

    return result;
    }
#endif	/* INCLUDE_COPROCESSOR */

/******************************************************************************
*
* usrCoreDumpRtInfoGet - get info on the VxWorks run time system.
*
* This routine fills the runtime info structure.
* 
* NOTE: Since this routine is called during core dump generation, it must
*       not perform any system calls.
*
* RETURNS: N/A
*
* NOMANUAL
*/

LOCAL void usrCoreDumpRtInfoGet
    (
    TGT_INFO_NOTE *	pRtInfo
    )
    {
    strncpy (pRtInfo->rtName, runtimeName, CORE_DUMP_RT_NAME_LEN);
    pRtInfo->rtName [CORE_DUMP_RT_NAME_LEN - 1] = EOS;
    strncpy (pRtInfo->rtVersion, runtimeVersion, CORE_DUMP_RT_VER_LEN);
    pRtInfo->rtName [CORE_DUMP_RT_VER_LEN - 1] = EOS;
    pRtInfo->cpuFamily		= CPU_FAMILY;
#ifdef  _WRS_WDB_CPU_TYPE
    pRtInfo->cpuType		= _WRS_WDB_CPU_TYPE;
#else   /* _WRS_WDB_CPU_TYPE */
    pRtInfo->cpuType		= CPU;
#endif	/* _WRS_WDB_CPU_TYPE */
#ifdef	CPU_VARIANT
    strncpy (pRtInfo->cpuVariant, MKSTR (CPU_VARIANT),
    					CORE_DUMP_CPU_VARIANT_LEN);
    pRtInfo->cpuVariant [CORE_DUMP_CPU_VARIANT_LEN - 1] = EOS;
#else	/* CPU_VARIANT */
    pRtInfo->cpuVariant [0]	= EOS;
#endif	/* CPU_VARIANT */
#ifdef	TOOL
    strncpy (pRtInfo->toolName, MKSTR (TOOL),
    					CORE_DUMP_TOOL_NAME_LEN);
    pRtInfo->toolName [CORE_DUMP_TOOL_NAME_LEN - 1] = EOS;
#else	/* TOOL */
    pRtInfo->toolName [0]	= EOS;
#endif	/* TOOL */
#ifdef	BSP_NAME
    strncpy (pRtInfo->bspShortName, MKSTR (BSP_NAME),
    					CORE_DUMP_BSP_SHORT_NAME_LEN);
    pRtInfo->bspShortName [CORE_DUMP_BSP_SHORT_NAME_LEN - 1] = EOS;
#else	/* BSP_NAME */
    pRtInfo->bspShortName [EOS]	= EOS;
#endif	/* BSP_NAME */
#ifdef	INCLUDE_COPROCESSOR
    pRtInfo->hasCoprocessor	= coreDumpTgtHasCoprocessor ();
#else	/* INCLUDE_COPROCESSOR */
    pRtInfo->hasCoprocessor	= 0;
#endif	/* INCLUDE_COPROCESSOR */
#ifdef INCLUDE_PROTECT_TEXT
    pRtInfo->hasWriteProtect	= vmLibInfo.protectTextSegs;
#else	/* INCLUDE_PROTECT_TEXT */
    pRtInfo->hasWriteProtect	= FALSE;
#endif	/* INCLUDE_PROTECT_TEXT */
    pRtInfo->pageSize   	= VM_PAGE_SIZE_GET();
    pRtInfo->endian		= _BYTE_ORDER;
    strncpy (pRtInfo->bspName, sysModel (), CORE_DUMP_BSP_NAME_LEN);

#ifndef _WRS_CONFIG_LP64
#ifdef	HITACHI_SH_KERNEL_ON_SDRAM
    pRtInfo->memBase	= (ULONG) (FREE_RAM_ADRS);
    pRtInfo->memSize	= (unsigned long) sysMemTop () -
    					(unsigned long) FREE_RAM_ADRS;
#else	/* HITACHI_SH_KERNEL_ON_SDRAM */
    pRtInfo->memBase	= (ULONG)(LOCAL_MEM_LOCAL_ADRS);
    pRtInfo->memSize	= (unsigned long) sysMemTop() -
    					(unsigned long) LOCAL_MEM_LOCAL_ADRS;
#endif	/* HITACHI_SH_KERNEL_ON_SDRAM */
#else
    /*
     * In 64-bit, RAM may consist of a number of discontiguous blocks, so a
     * single base address is not sufficient to describe it. Moreover, this
     * field is no longer used by the VxWorks 64-bit target server.
     */

    pRtInfo->memBase	= (ULONG) -1;
    pRtInfo->memSize	= adrSpacePhysSize;	/* computed info returned */
						/* by adrSpaceInfoGet() */
#endif	/* _WRS_CONFIG_LP64 */

#if	(CORE_DUMP_SKIP_TEXT_SECTION == TRUE)
    pRtInfo->textScnCksum = coreDumpCksum (wrs_kernel_text_start,
					   wrs_kernel_text_end -
							wrs_kernel_text_start,
					   0, NULL, FALSE);
#else	/* CORE_DUMP_SKIP_TEXT_SECTION == FALSE */
    pRtInfo->textScnCksum = 0;
#endif	/* CORE_DUMP_SKIP_TEXT_SECTION == TRUE */
    }

#ifdef INCLUDE_CORE_DUMP_RTP
/*******************************************************************************
*
* usrCoreDumpRtpInit - initialize the RTP core dump support
*
* This routine initializes the core dump facility.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
*/

LOCAL STATUS usrCoreDumpRtpInit (void)
    {
    /* initialize RTP core dump library */

    if (coreDumpRtpInit ((CORE_DUMP_RT_INFO_GET) usrCoreDumpRtInfoGet) == ERROR)
	return (ERROR);

    /* initialize core dump compression facility */

#ifdef INCLUDE_CORE_DUMP_RTP_COMPRESS_ZLIB
#if ((CORE_DUMP_RTP_ZLIB_COMPRESSION_LEVEL < 1 ) || (CORE_DUMP_RTP_ZLIB_COMPRESSION_LEVEL > 9))
#error "Invalid value for CORE_DUMP_RTP_ZLIB_COMPRESSION_LEVEL parameter"
#endif
    if (coreDumpZLibInit (CORE_DUMP_DEVICE_RTP,
			  CORE_DUMP_RTP_ZLIB_COMPRESSION_LEVEL) == ERROR)
	return (ERROR);
#elif defined(INCLUDE_CORE_DUMP_RTP_COMPRESS_RLE)
    if (coreDumpRleInit (CORE_DUMP_DEVICE_RTP) == ERROR)
	return (ERROR);
#endif	/* !INCLUDE_CORE_DUMP_RTP_COMPRESS_ZLIB && !INCLUDE_CORE_DUMP_RTP_COMPRESS_RLE */

#ifdef	INCLUDE_CORE_DUMP_RTP_FS
    if (coreDumpRtpFsDevInit (CORE_DUMP_RTP_FS_PATH, TRUE) == ERROR)
	return (ERROR);
#endif	/* INCLUDE_CORE_DUMP_RTP_FS */

    return (OK);
    }
#endif	/* INCLUDE_CORE_DUMP_RTP */

/*******************************************************************************
*
* usrCoreDumpPhysMemCheck - check if given memory block is physical memory
*
* This routine checks if the given memory block described by <paddr> and
* <size> is a block of physical memory or not.
*
* If current configuration defines _WRS_CONFIG_USE_MEMDESC, then this
* routine parses the physical memory descriptor array using sysMemDescGet()
* to test if given block is part of kernel RAM (MEM_DESC_RAM), user RAM
* (MEM_DESC_USER_RESERVED_RAM), or persistent memory RAM (MEM_DESC_PM_RAM).
* NOTE: The I/O RAM (MEM_DESC_DMA32_RAM) is by default voluntary excluded
* from core dump generation (This memory can be dumped using coreDumpMemDump()).
*
* If current configuration does not define _WRS_CONFIG_USE_MEMDESC, then this
* routine checks if given memory block is located between LOCAL_MEM_LOCAL_ADRS
* and sysPhysMemTop().
*
* In any cases, if the given memory block is partially outside of VxWorks
* physical memory, this routine returns via <pBlockPaddr> & <pBlockSize> the
* adjusted start address and size of the memory block. This is necessary
* because of vmContigBlockEach() routine behavior: The each routine can
* be called with contiguous blocks containing I/O memory (file mapping,
* registers, ...).
*
* NOTE: Since this routine is called during core dump generation, it must
*       not perform any system calls.
*
* RETURNS: TRUE if given memory block is physical memory, FALSE otherwise.
*
* ERRNO: N/A
*
*/

LOCAL BOOL usrCoreDumpPhysMemCheck
    (
    PHYS_ADDR	paddr,		/* Address of memory block to check */
    size_t	size,		/* Size of memory block to check */
    PHYS_ADDR * pBlockPaddr,	/* Address of found physical memory block */
    size_t *	pBlockSize	/* Size of found physical memory block */
    )
#ifdef _WRS_CONFIG_USE_MEMDESC
    {
    int typeIdx = 0;
    int memDescIdx = 0;
    PHYS_MEM_DESC memDesc;
    MEM_DESC_TYPE memDescType[] =	{MEM_DESC_RAM,
					 MEM_DESC_USER_RESERVED_RAM,
					 MEM_DESC_PM_RAM,
					 0};

    while (memDescType[typeIdx] != 0)
	{
	while (sysMemDescGet (memDescType[typeIdx], memDescIdx++,
			      &memDesc) != ERROR)
	    {
	    if (usrCoreDumpPhysMemTest (memDesc.physicalAddr,
					memDesc.len,
					paddr, size, pBlockPaddr,
					pBlockSize) == TRUE)
		return (TRUE);
	    }
	
	typeIdx++;
	memDescIdx = 0;
	}
 
    /*
     * This given block is entirely outside of VxWorks Physical memory:
     * It must be ignored.
     */

    return (FALSE);
    }
#else	/* !_WRS_CONFIG_USE_MEMDESC */
    {
    return (usrCoreDumpPhysMemTest (coreDumpSysMemBaseAdrs,
				    coreDumpSysPhysMemSize,
				    paddr, size,
				    pBlockPaddr, pBlockSize));
    }
#endif	/* _WRS_CONFIG_USE_MEMDESC */

/*******************************************************************************
*
* usrCoreDumpPhysMemTest - test if given memory block is physical memory
*
* This routine tests if the given memory block described by <paddr> and
* <size> is part of the physical memory block described by <physicalAddr>
* and <len>.
*
* NOTE: If the given memory block is partially outside of given VxWorks
* physical memory block, this routine returns via <pBlockPaddr> & <pBlockSize>
* the adjusted start address and size of the memory block. This is necessary
* because of vmContigBlockEach() routine behavior: The each routine can be
* called with contiguous blocks containing I/O memory (file mapping, registers,
* ...).
*
* NOTE: Since this routine is called during core dump generation, it must
*       not perform any system calls.
*
* RETURNS: TRUE if given memory block is physical memory, FALSE otherwise.
*
* ERRNO: N/A
*
*/

LOCAL BOOL usrCoreDumpPhysMemTest
    (
    PHYS_ADDR	physicalAddr,	/* block physical address */
    size_t	len,		/* block length */
    PHYS_ADDR	paddr,		/* Address of memory block to check */
    size_t	size,		/* Size of memory block to check */
    PHYS_ADDR * pBlockPaddr,	/* Address of found physical memory block */
    size_t *	pBlockSize	/* Size of found physical memory block */
    )
    {
    *pBlockSize = size;
    *pBlockPaddr = paddr;

    /* If memory segment is entirely outside VxWorks physical memory: skip it */

    if (((paddr + size) <= physicalAddr) || (paddr >= (physicalAddr + len)))
	return (FALSE);	/* Skip that memory segment */

    /*
     * If memory segment is entirely inside VxWorks physical memory:
     * It must be dumped.
     */

    if ((paddr >= physicalAddr) && ((paddr + size) <= (physicalAddr + len)))
	return (TRUE);

    /*
     * If memory segment is partialy outside of VxWorks physical memory,
     * then we must adjust segment address and/or size to only dump the area
     * located in VxWorks memory.
     */

    if ((paddr + size) > (physicalAddr + len))
	{
	/*
	 * The higher part of this block is outside of VxWorks physical memory,
	 * adjust <size> to only dump the region that is located inside of
	 * VxWorks physical memory.
	 */

	*pBlockSize = (physicalAddr + len) - paddr;
	return (TRUE);
	}

    if (paddr < physicalAddr)
	{
	/*
	 * The lower part of this block is outside of VxWorks physical memory,
	 * adjust paddr, vaddr and size to only dump the region that is located
	 * inside of VxWorks physical memory.
	 */
	
	*pBlockSize -= (physicalAddr - paddr);
	*pBlockPaddr = physicalAddr;
	return (TRUE);
	}

    return (FALSE);
    }

/*******************************************************************************
*
* usrCoreDumpPhysMemDump - Dump physical memory
*
* This routine is used by the core dump generator to dump VxWorks memory when
* virtual memory is not installed (VM_INSTALLED = FALSE).
* This routine supports both the legacy model and the sysMemDescGet() model
* for describing address space.
*
* NOTE: Since this routine is called during core dump generation, it must
*       not perform any system calls.
*
* RETURNS: OK, or ERROR if dump routine failed.
*
* ERRNO: N/A
*
*/

LOCAL STATUS usrCoreDumpPhysMemDump
    (
    FUNCPTR	pDumpRtn,	/* Dump routine */
    void *	pKernelIf,	/* Kernel core dump interface */
    UINT32	state,		/* Memory segment state flags */
    UINT32	alignment	/* Memory segment alignment */
    )
    {
#ifdef _WRS_CONFIG_USE_MEMDESC
    /*
     * Use sysMemDescGet() to determine the physical memory blocks to dump
     * into core dump.
     * By default, we dump the kernel RAM, the User Reserved Memory, and the
     * Persistent Memory (We only exclude the DMA RAM (MEM_DESC_DMA32_RAM).
     */

    int typeIdx = 0;
    int memDescIdx = 0;
    PHYS_MEM_DESC memDesc;
    MEM_DESC_TYPE memDescType[] =	{MEM_DESC_RAM,
					 MEM_DESC_USER_RESERVED_RAM,
					 MEM_DESC_PM_RAM,
					 0};

    while (memDescType[typeIdx] != 0)
	{
	while (sysMemDescGet (memDescType[typeIdx], memDescIdx++,
			      &memDesc) != ERROR)
	    {
	    /* Dump this memory region to the core dump */

	    if (pDumpRtn (pKernelIf, memDesc.physicalAddr, memDesc.len,
			  memDesc.physicalAddr, memDesc.physicalAddr,
			  state, alignment) == ERROR)
		return (ERROR);
	    }
	
	typeIdx++;
	memDescIdx = 0;
	}
 
    return (OK);
#else	/* !_WRS_CONFIG_USE_MEMDESC */
    /*
     * Use legacy model to determine the physical memory block to dump
     * into core dump (Use coreDumpSysMemBaseAdrs / coreDumpSysPhysMemSize
     * which have been computed earlier using LOCAL_MEM_LOCAL_ADRS and
     * sysPhysMemTop().
     */

    return (pDumpRtn (pKernelIf, coreDumpSysMemBaseAdrs, coreDumpSysPhysMemSize,
		      coreDumpSysMemBaseAdrs, coreDumpSysMemBaseAdrs,
		      state, alignment));
#endif	/* _WRS_CONFIG_USE_MEMDESC */
    }
