/* usrWdbCore.c - configuration file for the WDB agent's core facilities */

/*
 * Copyright (c) 1997-2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use of this 
 * software may be licensed only pursuant to the terms of an applicable Wind 
 * River license agreement.
 */

/*
modification history
--------------------
04l,16may12,jmp  CQID: Fix WIND00324029: Created usrWdbTaskModeInit() wrapper
                 which uses coprocsDiscover() when supported to invoke
                 wdbTaskModeInit() with supported coproc options value.
04k,07feb12,rlp  CQ:WIND00331131. Added usrWdbInitDone routine.
04j,06may11,jmp  Updated vxExcHook to no longer take structures as parameters,
		 we now use pointers on structures (CQ:WIND00266088).
04j,01sep11,rlp  CQID: Fix WIND00297470. Did not stop the context that gets an
                 exception if the system is not in debug mode.
04i,11apr11,rlp  Set wdbIsAlwaysEnabled variable before the WDB initialization.
		 Added System Debug mode hook to disable WDB communication
		 interface.
04h,14feb11,rlp  Fixed a build issue with previous modification.
04g,07feb11,rlp  Enabled system mode agent for LP64.
04f,25oct10,rlp  Added the INCLUDE_WDB_ALWAYS_ENABLED support (WIND00227819).
04e,27sep10,rlp  Do not execute exception hook if target server not connected.
04d,06oct10,jmp  Updated to allow WDB inclusion even if
                 INCLUDE_EDR_POLICY_HOOKS component is not included
                 (CQ:WIND00215097).
04c,28jun10,jc0  remove coprocsDiscover(), pCoprocDescList and add them
                 in coproLibP.h header file (WIND00211499). 
04b,28may10,rlp  Removed warnings.
04a,26may10,jmp  Added wdbRtMemInit() to initialize move/scan utilities only
                 when INCLUDE_WDB_MEM component is included.
03z,17may10,rlp  Do not stop RTP in deployed mode.
03y,11may10,rlp  Relied on WDB exc hook to stop the context rather than ED&R.
03x,11may10,jmp  Fixed build issue.
03w,28apr10,jmp  Update WDB to use a shared buffer for temporary operations
		 made by wdbMemRead() & wdbRegsGet().
03v,10mar10,rlp  Complete LP64 adaptation.
03u,05jan10,rlp  Fixed a warning.
03t,10dec09,rlp  Added exception event notification support for LP64.
03s,09sep09,rlp  Partial LP64 adaptation.
03s,21aug09,rlp  Fixed the computation of the target memory size reported to
                 the host (WIND00178188).
03r,15apr08,j_z  Initialize the struct excCtx (Defect #107990).
03q,11jul07,rlp  Modified information reported on exception in system mode.
03p,06jul07,p_b  Added 64bits support
03o,25feb07,jmt  Modify for bootApp support
03n,10jul06,rlp  Replaced VX_CPU_FAMILY by CPU_FAMILY (Defect #37561).
03m,08dec05,dbt  Fixed wdbInstallCommIf() call to provide MTU.
03l,29nov05,dbt  Moved some system mode specific stuff to usrWdbSys.c.
03k,14nov05,dbt  Removed wdbMemRtLibInit() call.
03j,28jul05,bpn  Changed wdbMemChunck buffer to a configurable buffer linked to
                 the INCLUDE_WDB_REG component. Reduced the number of services
		 to 40 as DSA is not supported. Changed conditionnaly build
		 from INCLUDE_TSFS_BOOT to INCLUDE_COPROCESSOR. Removed 
		 obsolete functions.
03i,03mar05,pch  support _WRS_WDB_CPU_TYPE
03h,27sep04,dbt  Updated for merge with usrWdb.c file.
03g,13aug04,dbt  Provide signal number in case of RTP exception.
03f,06jun04,dbt  Change the way to detect if text protection is enabled or
                 not.
03e,27apr04,elg  Modify exception hook wrapper.
03d,31mar04,elg  Remove unused coprocessor routines.
03c,23mar04,elg  Rework way contexts are transmitted in events.
03b,08mar04,elg  Modify WDB_CTX structure.
03a,26feb04,mil  Added SPE support for PPC85XX.
02z,03oct03,elg  Add CPU family and CPU variant.
02y,03oct03,elg  Fix typo error.
02x,08sep03,elg  Do not set page size if MPU is used (fix SPR #90333).
02w,05mar03,jmp  removed SIMNT specific code.
02v,28feb03,elg  Merge file from BSD/OS and define new runtime interface.
02u,05feb03,elg  Add WDB merged protocol.
02t,15feb01,jhw  No longer delete breakpoints in wdbExternEnterHook(), this 
		 is now done on external agent entry (SPR #64291).
02s,18sep00,tpr  replaced vxWorksVersion by runtimeVersion.
02r,14sep00,rbl  fixing use of TOOL and BSP_NAME so that quotes are not
                 necessary on the compile line
02q,05sep00,elg  Add fields toolName and bspShortName (SPR 32724).
02p,28aug00,elg  Change pdIsValid() to pdIdVerify().
02o,08aug00,elg  Add VM_CONTEXT_LOCK() and VM_CONTEXT_UNLOCK().
02n,11jul00,dbt  Reworked context management in system mode.
02m,13jun00,elg  Change current task's context in system mode.
02l,08jun00,dbt  Use BOOT_xxx macros instead of numeric constants for WDB boot
                 line length.
02k,31may00,elp  removed useless SIMNT code.
02j,26may00,elg  Fix bug in vxMemProtect().
02i,22may00,elg  Cancel context switches to kernel.
02h,18may00,dbt  Increased MAX_LEN according modification of BOOT_FILE_LEN.
02g,05apr00,aeg  deleted conditional test for taskCtxSwitch() invocation.
02f,29mar00,elg  Code cleanup.
02e,08mar00,elg  Modify the WDB_RT_INFO structure.
02d,14feb00,elg  Improve vxTaskCtxSwitch().
02c,02feb00,dbt  Remove the use of pdListGet.
02b,31jan00,elg  Fix bug in wdbConfig().
02a,26jan00,elg  Move vxTaskCtxSwitch () from usrWdbTask.c to usrWdbCore.c.
01z,06jan00,dra  PAL update: replace CPU tests.
01y,09nov99,gls  updated to use new vmPageProtect API
                 removed incorrect byte/address calculation in vxMemProtect
01x,09nov99,elp  adapted to VxWorks 6.0 (SIMNT).
01w,22oct99,elg  Add page size info in RT interface.
01v,06oct99,dbt  WDB agent pool is now a static buffer.
01u,27sep99,elg  Add temporary patch in memory protection.
01t,13sep99,bpn  Initialized the communication port variable with the value
                 defined in the ConfigTool (SPR #28606).
01s,24aug99,bpn  Use the new defined hook pointer _func_excWdbHook for the
                 WDB agent instead of the general hook pointer
                 _func_excBaseHook (SPR #8725)
                 The step mode of the task is disabled at the beginning of the
                 hook wrapper (SPR #26050).
01r,07jul99,dbt  allocate buffer for WDB agent memory pool only at the first
                 connection.
01q,21jun99,elg  Add protection domain information.
01p,27apr99,cym  removing wait for simulator IDE signaling
01o,20apr99,tam  pVmTextProtectRtn -> pVmPageProtectRtn
01n,22feb99,cym  increased time before notifying IDE (SPR #23942.)
01m,10jan99,map  allocate memory for wdb pool.
01m,07nov02,dtr  Adding SPE support.
01l,01apr02,jkf  SPR#74251, using bootLib.h macros for array sizes
01k,28feb02,jhw  Modified hasWriteProtect assignment. (SPR 71816).
01j,04oct01,c_c  Added wdbToolName symbol.
01i,06jun01,pch  Fix length of copy to vxBootFile.
01h,09may01,dtr  Addition of wdbTgtHasCoprocessor.
01g,19apr01,kab  Added Altivec support
01f,16nov00,zl   added DSP support.
01e,27apr99,cym  removing wait for simulator IDE signaling
01d,22feb99,cym  increased time before notifying IDE (SPR #23942.)
01c,08jul98,ms   need semTake initialized early if WDB_TASK included
01b,18may98,dbt  integrated latest WDB stuff
01a,21jul97,ms   taken from 01y of usrWdb.c
*/

/*
DESCRIPTION

This library configures the WDB agent's core facilities.
Agent initialization is a separate step.
*/

#ifdef	_WRS_CONFIG_LP64
#include <adrSpaceLib.h>
#endif	/* _WRS_CONFIG_LP64 */

/* defines */

#define	NUM_MBUFS		5
#define MAX_LEN			BOOT_LINE_SIZE
#define WDB_MAX_SERVICES        40      /* max # of agent services */
#define WDB_NUM_CL_BLKS         5

#ifndef _WRS_CONFIG_LP64
#define WDB_POOL_BASE		((char *) (FREE_RAM_ADRS))
#endif	/* _WRS_CONFIG_LP64 */

#if (!defined(BOOTCONFIG) && !defined(BOOTAPP))
#undef	INCLUDE_TSFS_BOOT	/* TSFS boot is only supported with bootrom */
#endif	/* BOOTCONFIG */

/* externals */

extern STATUS	wdbCommDevInit (WDB_COMM_IF * pCommIf, char ** ppWdbInBuf,
				      char ** ppWdbOutBuf);

#ifdef	INCLUDE_COPROCESSOR
#include <private/coprocLibP.h>
#endif	/* INCLUDE_COPROCESSOR */

/* globals */

uint_t		wdbCommMtu;
int		wdbNumMemRegions = 0;	/* number of extra memory regions */
WDB_MEM_REGION * pWdbMemRegions = NULL;	/* array of regions */
void		(*wdbBpSysEnterHook) (void);
void		(*wdbBpSysExitHook) (void);
BOOL		(*wdbTaskRestartHook) (EXC_INFO * pExcInfo) = NULL;
WDB_RT_IF	wdbRtIf;		/* XXX - move to wdbLib.c */
bool_t		wdbIsInitialized = FALSE;

/*
 * Define default size for WDB shared buffer.
 * We must make sure that WDB_SHARED_BUFFER_SIZE is a multiple of the size
 * of a long. This is required to respect alignment to ULONG for wdbMemRead().
 */

#define WDB_SHARED_BUFFER_SIZE	\
		((long) (WDB_MTU) & ~((long) sizeof(long) - 1))

#ifdef	INCLUDE_WDB_PRE_KERNEL_INIT
ULONG wdbStaticSharedBuffer [WDB_SHARED_BUFFER_SIZE/sizeof(ULONG)];
#endif	/* INCLUDE_WDB_PRE_KERNEL_INIT */

char * wdbSharedBuffer;					/* WDB shared buffer */
UINT32	wdbSharedBufferSize = WDB_SHARED_BUFFER_SIZE;	/* Shared buffer size */

#ifdef _WRS_CONFIG_LP64
#ifdef	INCLUDE_WDB_PRE_KERNEL_INIT
ULONG	wdbPoolBase [WDB_POOL_SIZE/sizeof(ULONG)];
#else	/* INCLUDE_WDB_PRE_KERNEL_INIT */
void *	wdbPoolBase = NULL;
#endif	/* INCLUDE_WDB_PRE_KERNEL_INIT */
#endif	/* _WRS_CONFIG_LP64 */

/* This two macros transform TOOL define into the string "TOOL" */

#define MKSTR(MACRO) MKSTR_FIRST_PASS(MACRO)
#define MKSTR_FIRST_PASS(MACRO) #MACRO

/* 
 * This symbol "wdbToolName" is used by the tgtsvr to retrieve the name of the
 * tool used to build vxWorks run-time. DO NOT REMOVE !!!
 */

#ifdef	TOOL
const char wdbToolName[]	= MKSTR(TOOL);
#else	/* TOOL */
const char wdbToolName[]	= "Unknown";
#endif	/* TOOL */

/* locals */

static char		vxBootFile [MAX_LEN];

static WDB_SVC		wdbSvcArray [WDB_MAX_SERVICES];
static uint_t		wdbSvcArraySize = WDB_MAX_SERVICES;
static struct mbuf	mbufs [NUM_MBUFS];
static CL_BLK		wdbClBlks [WDB_NUM_CL_BLKS];

static void		(*vxExcHook)
				(
				WDB_CTX *	pStoppedCtx,
				WDB_CTX *	pExcCtx,
				UINT32		vec,
				UINT32		sigNum,
				void *		pEsf,
				WDB_IU_REGS *	pRegs,
				BOOL		systemSuspend
				);

/* forward static declarations */

static void	wdbRtIfInit (void);
static STATUS	wdbCommIfInit (void);
#ifdef	INCLUDE_COPROCESSOR
static UINT32	wdbCoprocMaxSizeGet (void);
static size_t	wdbCoprocMaxAlignGet (void);
#endif	/* INCLUDE_COPROCESSOR */

/******************************************************************************
*
* usrWdbInitDone - acknowledge the WDB agent initialization.
*
* This routine acknowledges the WDB agent initialization.
*
* RETURNS : N/A
*
* NOMANUAL
*/

void usrWdbInitDone (void)
    {
    wdbIsReady = TRUE;
    }
    
/******************************************************************************
*
* wdbConfig - configure and initialize the WDB agent.
*
* This routine configures and initializes the WDB agent.
*
* RETURNS :
* OK or ERROR if the communication link cannot be initialized.
*
* NOMANUAL
*/

STATUS wdbConfig (void)
    {
#ifndef	INCLUDE_WDB_PRE_KERNEL_INIT
    size_t wdbSharedBufferAlignment = sizeof (long);
#ifdef	INCLUDE_COPROCESSOR
    /*
     * Kernel is initialized and it does support coprocessors: We must
     * determine the size of the biggest coprocessor register set. This size
     * will then be used to allocate the WDB shared buffer (Minimal size of
     * shared buffer will be WDB_MTU).
     */

    UINT32 wdbCoprocMaxSize = wdbCoprocMaxSizeGet();
    size_t wdbCoprocMaxAlignment = wdbCoprocMaxAlignGet ();

    if (wdbCoprocMaxSize > WDB_SHARED_BUFFER_SIZE)
	wdbSharedBufferSize = wdbCoprocMaxSize;

    if (wdbCoprocMaxAlignment > wdbSharedBufferAlignment)
	wdbSharedBufferAlignment = wdbCoprocMaxAlignment;
#endif	/* INCLUDE_COPROCESSOR */

    /*
     * The shared buffer must be allocated using KMEM_ALIGNED_ALLOC() to also
     * work fine with memAllotLib (when _WRS_CONFIG_MEM_ALLOT is defined).
     */

    if ((wdbSharedBuffer = KMEM_ALIGNED_ALLOC (wdbSharedBufferSize,
					wdbSharedBufferAlignment)) == NULL)
	{
	if (_func_printErr != NULL)
	    _func_printErr ("wdbConfig: Error allocating WDB shared buffer\n");
	return (ERROR);
	}

#ifdef	_WRS_CONFIG_LP64
    if ((wdbPoolBase = KMEM_ALIGNED_ALLOC (WDB_POOL_SIZE,
    						sizeof (ULONG))) == NULL)
    	{
	if (_func_printErr != NULL)
	    _func_printErr ("wdbConfig: Error allocating WDB memory pool\n");
	return (ERROR);
	}
#endif	/* _WRS_CONFIG_LP64 */
#else	/* INCLUDE_WDB_PRE_KERNEL_INIT */

    /*
     * In pre-kernel initialization mode, WDB system mode uses a static buffer
     * sized using WDB_MTU. If this buffer is too small to store a requested
     * coprocessor register set, wdbRegGet() will return WDB_ERR_RT_ERROR.
     */

    wdbSharedBuffer = (char *)wdbStaticSharedBuffer;
#endif	/* INCLUDE_WDB_PRE_KERNEL_INIT */

#ifdef	INCLUDE_WDB_ALWAYS_ENABLED
    wdbIsAlwaysEnabled = TRUE;    
#else	/* INCLUDE_WDB_ALWAYS_ENABLED */
    wdbIsAlwaysEnabled = FALSE;    
#endif	/* INCLUDE_WDB_ALWAYS_ENABLED */

    if (sysDebugModeSetHookAdd (wdbCommIfEnable) == ERROR)
	{
	if (_func_printErr != NULL)
	    _func_printErr ("wdbConfig: error adding System Debug Mode hook\n");
	return (ERROR);
	}

    /* Initialize the agents interface function pointers */

    wdbRtIfInit   ();		/* run-time interface functions */

    if (wdbCommIfInit () == ERROR)	/* communication interface functions */
	{
	if (_func_printErr != NULL)
	    _func_printErr ("wdbConfig: error configuring WDB communication interface\n");
	return (ERROR);
	}

    /* Install some required agent services */

    wdbSvcLibInit (wdbSvcArray, wdbSvcArraySize);
    wdbConnectLibInit ();	/* required agent service */
    wdbTgtInfoLibInit ();	/* required agent service */
    wdbMemCoreLibInit ();	/* required agent service */

    wdbIsInitialized = TRUE;	/* mark WDB as initialized */

    return (OK);
    }

/*******************************************************************************
*
* usrWdbTaskModeInit - configure and initialize the WDB task mode agent
*
* This routine configures and initializes the WDB task mode agent with
* support for enabled coprocessors.
*
* RETURNS: ERROR if WDB task initialization fails,
*	   OK otherwise.
*
* NOMANUAL
*/

STATUS usrWdbTaskModeInit
    (
    UINT32	spawnStackSize,	/* stack size of tasks created by WDB */
    UINT32	taskPriority,	/* WDB task priority */
    UINT32	taskOptions,	/* WDB task options */
    UINT32	stackSize,	/* WDB task stack size */
    UINT32	maxRestarts,	/* WDB task maximum restarts */
    UINT32	restartTime	/* WDB task restart delay */
    )
    {
#ifdef	INCLUDE_COPROCESSOR
    UINT32	ix;
    UINT32	descriptorMasks[COPROCS_MAX];

    for (ix = 0; ix < COPROCS_MAX; ++ix)
	descriptorMasks[ix] = 0;

    /*
     * Use coprocsDiscover() to complete wdbTaskOptions with supported
     * coproc options.
     */

    coprocsDiscover (descriptorMasks);

    for (ix = 0; ix < COPROCS_MAX && descriptorMasks[ix] != 0; ++ix)
	taskOptions |= descriptorMasks[ix];
#endif	/* INCLUDE_COPROCESSOR */

    return (wdbTaskModeInit (spawnStackSize, taskPriority, taskOptions,
			     stackSize, maxRestarts, restartTime));
    }

#ifdef	INCLUDE_COPROCESSOR
/******************************************************************************
* 
* wdbTgtHasCoprocessor - check supported coprocessors
*
* This function's result contains bit fields for each 
* co-processor. Coprocessors include floating-point, Altivec (PPC),
* SPE (PPC), and DSP (SH).
*
* RETURNS: a bit mask with supported coprocessors.
*
* NOMANUAL
*/ 

static UINT32 wdbTgtHasCoprocessor (void)
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

/*******************************************************************************
*
* wdbCoprocMaxSizeGet - determine the size of the biggest coproc register set
*
* This routine parses the coprocessor descriptor list to determine the size of
* the biggest coprocessor register set.
*
* RETURNS: Size of the biggest coproc register set
*
* ERRNO: N/A
*
*/

static UINT32 wdbCoprocMaxSizeGet (void)
    {
    COPROC_DESC *	pDesc;		/* current coprocessor descriptor */
    UINT32		maxCtxSize = 0;	/* Max coprocessor context size */

    for (pDesc = pCoprocDescList; pDesc != NULL; pDesc = pDesc->next)
	{
	if (pDesc->ctxSize > maxCtxSize)
	    maxCtxSize = pDesc->ctxSize;
	}

    return (maxCtxSize);
    }

/*******************************************************************************
*
* wdbCoprocMaxAlignGet - determine the biggest coproc context alignment
*
* This routine parses the coprocessor descriptor list to determine the biggest
* coprocessor context memory alignment.
*
* RETURNS: Size of the biggest coproc register set alignment.
*
* ERRNO: N/A
*
*/

static size_t wdbCoprocMaxAlignGet (void)
    {
    COPROC_DESC *	pDesc;		/* current coprocessor descriptor */
    size_t		maxCtxAlign = 0;/* Max coprocessor context alignment */

    for (pDesc = pCoprocDescList; pDesc != NULL; pDesc = pDesc->next)
	{
	if (pDesc->ctxAlign > maxCtxAlign)
	    maxCtxAlign = pDesc->ctxAlign;
	}

    return (maxCtxAlign);
    }
#endif	/* INCLUDE_COPROCESSOR */

/******************************************************************************
*
* wdbRtInfoGet - get info on the VxWorks run time system.
*
* This routine fills the runtime info structure.
*
* RETURNS: a status
*
* NOMANUAL
*/

static UINT32 wdbRtInfoGet
    (
    WDB_RT_INFO *	pRtInfo
    )
    {
#ifdef _WRS_CONFIG_LP64
    ADR_SPACE_INFO adrsSpaceInfo;
#endif

    /* XXX - temporary modification: waiting for the creation of vxWorksName */
    pRtInfo->rtName		= runtimeName;
    pRtInfo->rtVersion		= runtimeVersion;
    pRtInfo->cpuFamily		= CPU_FAMILY;
#ifdef	_WRS_WDB_CPU_TYPE
    pRtInfo->cpuType		= _WRS_WDB_CPU_TYPE;
#else	/* _WRS_WDB_CPU_TYPE */
    pRtInfo->cpuType		= CPU;
#endif	/* _WRS_WDB_CPU_TYPE */
#ifdef	CPU_VARIANT
    pRtInfo->cpuVariant		= MKSTR (CPU_VARIANT);
#else	/* CPU_VARIANT */
    pRtInfo->cpuVariant		= NULL;
#endif	/* CPU_VARIANT */
#ifdef	TOOL
    pRtInfo->toolName		= MKSTR(TOOL);
#else	/* TOOL */
    pRtInfo->toolName		= NULL;
#endif	/* TOOL */
#ifdef	BSP_NAME
    pRtInfo->bspShortName	= MKSTR(BSP_NAME);
#else	/* BSP_NAME */
    pRtInfo->bspShortName	= NULL;
#endif	/* BSP_NAME */
#ifdef	INCLUDE_COPROCESSOR
    pRtInfo->hasCoprocessor	= wdbTgtHasCoprocessor ();
#else	/* INCLUDE_COPROCESSOR */
    pRtInfo->hasCoprocessor	= 0;
#endif	/* INCLUDE_COPROCESSOR */
#ifdef INCLUDE_PROTECT_TEXT
    pRtInfo->hasWriteProtect	= vmLibInfo.protectTextSegs;
#else	/* INCLUDE_PROTECT_TEXT */
    pRtInfo->hasWriteProtect	= FALSE;
#endif	/* INCLUDE_PROTECT_TEXT */
    pRtInfo->pageSize   	= (TGT_INT_T) VM_PAGE_SIZE_GET();
    pRtInfo->endian		= _BYTE_ORDER;
    pRtInfo->bspName		= sysModel();
    pRtInfo->bootline	= vxBootFile;

#ifndef _WRS_CONFIG_LP64
#ifdef	HITACHI_SH_KERNEL_ON_SDRAM
    pRtInfo->memBase	= (TGT_ADDR_T) (FREE_RAM_ADRS);
    pRtInfo->memSize	= (unsigned long) sysMemTop () -
    					(unsigned long) FREE_RAM_ADRS;
#else	/* HITACHI_SH_KERNEL_ON_SDRAM */
    pRtInfo->memBase	= (TGT_ADDR_T)(LOCAL_MEM_LOCAL_ADRS);
    pRtInfo->memSize	= (unsigned long) sysMemTop() -
    					(unsigned long) LOCAL_MEM_LOCAL_ADRS;
#endif	/* HITACHI_SH_KERNEL_ON_SDRAM */
#else	/* _WRS_CONFIG_LP64 */
    /*
     * In 64-bit, RAM may consist of a number of discontiguous blocks, so a
     * single base address is not sufficient to describe it. Moreover, this
     * field is no longer used by the VxWorks 64-bit target server.
     */

    pRtInfo->memBase	= (TGT_ADDR_T) -1;

    /*
     * In pre-kernel initialization mode, the address space library is not
     * initialized yet.
     */
#ifndef	INCLUDE_WDB_PRE_KERNEL_INIT
    if (adrSpaceInfoGet (&adrsSpaceInfo) == OK)
	pRtInfo->memSize = adrsSpaceInfo.physTotalPages *
					adrsSpaceInfo.physAllocUnit;
    else
#endif	/* INCLUDE_WDB_PRE_KERNEL_INIT */
	pRtInfo->memSize = 0;
#endif	/* _WRS_CONFIG_LP64 */
    pRtInfo->numRegions	= wdbNumMemRegions;
    pRtInfo->memRegion	= pWdbMemRegions;

#ifndef _WRS_CONFIG_LP64
    pRtInfo->hostPoolBase = (TGT_ADDR_T) WDB_POOL_BASE;
#else	/* _WRS_CONFIG_LP64 */
    pRtInfo->hostPoolBase = (TGT_ADDR_T) wdbPoolBase;
#endif	/* _WRS_CONFIG_LP64 */
    pRtInfo->hostPoolSize = WDB_POOL_SIZE;
    pRtInfo->numLongs	  = sizeof (WDB_VXWORKS_INFO) / 4;
    pRtInfo->rtSpecific.vxWorksInfo.pdList = 0;

    return (WDB_OK);
    }

/******************************************************************************
*
* wdbRtConnectInfoGet - get connection info on the VxWorks run time system.
*
* This routine fills the runtime connection info structure.
*
* RETURNS: N/A
*
* NOMANUAL
*/

static UINT32 wdbRtConnectInfoGet
    (
    WDB_CONNECT_INFO *       pRtConnectInfo
    )
    {
    /* first get agent information */

    wdbInfoGet (&pRtConnectInfo->agentInfo);

    /* fill connection information fields */

    pRtConnectInfo->rtName		= runtimeName;
    pRtConnectInfo->tgtAddrSize		= sizeof (TGT_ADDR_T);
    pRtConnectInfo->tgtLongSize		= sizeof (TGT_LONG_T);
    pRtConnectInfo->tgtIntSize		= sizeof (TGT_INT_T);

    return (WDB_OK);
    }

/******************************************************************************
*
* vxReboot - reboot the system.
*
* This routine is the runtime interface for target rebooting.
*
* RETURNS: N/A
*
* NOMANUAL
*/

static UINT32 vxReboot (void)
    {
    reboot (0);
    return (WDB_OK);
    }

/******************************************************************************
*
* vxExcHookWrapper - exception hook wrapper
*
* This hook is called at exception time.
*
* RETURNS: TRUE or FALSE
*/

static int vxExcHookWrapper
    (
    int				excType,	/* exception type */
    void *			pInfo,		/* information on exception */
    BOOL			systemSuspend	/* suspend system ? */
    )
    {
    int				vecNum;		/* vector number */
    char *			pEsf;		/* exception frame */
    WDB_IU_REGS *		pRegs;		/* register set */
    EXC_INFO *			pExcInfo;	/* exception information */
    WDB_CTX			stoppedCtx;	/* stopped context */
    TGT_LONG_T			stoppedArgs [WDB_CTX_MAX_ARGS];
    WDB_CTX			excCtx;		/* context that got exception */
    TGT_LONG_T			excArgs [WDB_CTX_MAX_ARGS];
    EDR_INTERRUPT_INFO *	pIntInfo;	/* information on interrupt */
    EDR_INIT_INFO *		pInitInfo;	/* information on init */
    EDR_TASK_INFO *		pTaskInfo;	/* information on task */
    EDR_RTP_INFO *		pRtpInfo;	/* information on RTP */
    BOOL			systemExc;	/* system exception */
    ULONG			ctxToStop;	/* context must be stopped */
    UINT32			sigNum = 0;	/* signal number */

    /* no information means we are not in an exception */
    
    if ((pInfo == NULL) || !wdbTargetIsConnected())
    	return FALSE;	/* do not notify */

    /* initialize the struct */

	excCtx.contextType      = WDB_CTX_SYSTEM;
	excCtx.numArgs          = 0;

	/*
     * Fill context information: excCtx got exception, stoppedCtx has been
     * stopped by exception.
     */

    /* get room to save context information */

    excCtx.args		= excArgs;
    stoppedCtx.args	= stoppedArgs;

    /* by default this is not a system exception */

    systemExc = FALSE;

    /* by default the context is not stopped */

    ctxToStop = 0;

    switch (excType)
    	{
	case EDR_FACILITY_KERNEL:
	    pTaskInfo = (EDR_TASK_INFO *) pInfo;

	    /* check it is an exception */

	    if (!pTaskInfo->isException)
	    	return FALSE;

	    if (pTaskInfo->taskId == 0)
	    	pTaskInfo->taskId = (TASK_ID) taskIdCurrent;
		
	    /*
	     * If exception occured in kernel space, task is stopped,
	     * fill stoppedCtx with task information.
	     */

	    stoppedCtx.contextType	= WDB_CTX_TASK;
	    stoppedCtx.numArgs		= 2;
	    WDB_CTX_TASK_INFO_SET (stoppedCtx.args, pTaskInfo->taskId,
	    			   taskTcb (pTaskInfo->taskId)->rtpId);

	    excCtx.contextType	= WDB_CTX_TASK;
	    excCtx.numArgs	= 2;
	    WDB_CTX_TASK_INFO_SET (excCtx.args, pTaskInfo->taskId,
	    			   taskTcb (pTaskInfo->taskId)->rtpId);

	    /* task must be stopped */

	    ctxToStop = (ULONG) pTaskInfo->taskId;

	    vecNum	= pTaskInfo->vector;
	    pEsf	= pTaskInfo->pEsf;
	    pRegs	= pTaskInfo->pRegs;
	    pExcInfo	= pTaskInfo->pExcInfo;
	    break;

	case EDR_FACILITY_RTP:
	    pRtpInfo = (EDR_RTP_INFO *) pInfo;

	    /* check it is an exception */

	    if (!pRtpInfo->isException)
	    	return FALSE;

	    if (pRtpInfo->rtpId == NULL)
	    	pRtpInfo->rtpId = taskIdCurrent->rtpId;

	    /*
	     * If exception occured in user space, RTP is stopped,
	     * fill stoppedCtx with RTP information,
	     * fill excCtx with task information.
	     */

	    stoppedCtx.contextType	= WDB_CTX_RTP;
	    stoppedCtx.numArgs		= 1;
	    WDB_CTX_RTP_INFO_SET (stoppedCtx.args, pRtpInfo->rtpId);

	    excCtx.contextType	= WDB_CTX_TASK;
	    excCtx.numArgs	= 2;
	    WDB_CTX_TASK_INFO_SET (excCtx.args, pRtpInfo->taskId,
	    			   pRtpInfo->rtpId);

	    /* RTP must be stopped */

		ctxToStop = (ULONG) pRtpInfo->rtpId;

	    vecNum	= pRtpInfo->vector;
	    pEsf	= pRtpInfo->pEsf;
	    pRegs	= pRtpInfo->pRegs;
	    sigNum	= WTERMSIG(pRtpInfo->status);
	    pExcInfo	= 0;	/* only used to restart WDB task */
	    break;

	case EDR_FACILITY_INIT:
	    pInitInfo = (EDR_INIT_INFO *) pInfo;

	    /* check it is an exception */

	    if (!pInitInfo->isException)
	    	return FALSE;

	    /* if exception occurred in system, the whole system is stopped */

	    stoppedCtx.contextType	= WDB_CTX_SYSTEM;
	    stoppedCtx.numArgs		= 0;
	    WDB_CTX_SYS_INFO_SET (stoppedCtx.args);

	    /* this is a system exception */

	    systemExc = TRUE;
	    
	    vecNum	= pInitInfo->vector;
	    pEsf	= pInitInfo->pEsf;
	    pRegs	= pInitInfo->pRegs;
	    pExcInfo	= 0;	/* only used to restart WDB task */
	    break;

	case EDR_FACILITY_INTERRUPT:
	    pIntInfo = (EDR_INTERRUPT_INFO *) pInfo;

	    /* check it is an exception */

	    if (!pIntInfo->isException) 
	    	return FALSE;

	    /* if exception occurred in system, the whole system is stopped */

	    stoppedCtx.contextType	= WDB_CTX_SYSTEM;
	    stoppedCtx.numArgs		= 0;
	    WDB_CTX_SYS_INFO_SET (stoppedCtx.args);

	    /* this is a system exception */

	    systemExc = TRUE;

	    vecNum	= pIntInfo->vector;
	    pEsf	= pIntInfo->pEsf;
	    pRegs	= pIntInfo->pRegs;
	    pExcInfo	= 0;	/* only used to restart WDB task */
	    break;

	default:
	    return FALSE;
	}

    /*
     * If we intend to stop the whole system, override previous information
     * to tell system is stopped.
     */

    if (WDB_IS_NOW_EXTERNAL () || systemSuspend)
	{
	/* in system mode, the whole system is stopped */

	stoppedCtx.contextType	= WDB_CTX_SYSTEM;
	stoppedCtx.numArgs	= 0;
	WDB_CTX_SYS_INFO_SET (stoppedCtx.args);

	/*
	 * In system mode, the agent reports the index of the CPU which
	 * got the exception.
 	 */

 	excCtx.contextType	= WDB_CTX_SYSTEM;
	excCtx.numArgs		= 2;
	WDB_CTX_SYS_INFO_SET (excCtx.args);

	ctxToStop = 0;
	}

    /*
     * In case of a fatal error system, use taskIdCurrent to
     * have information on task that got exception. If taskIdCurrent
     * is NULL, tell the whole system got exception.
     */

    if (systemExc)
    	{
	if (taskIdCurrent == NULL)
	    {
	    excCtx.contextType	= WDB_CTX_SYSTEM;
	    excCtx.numArgs	= 0;
	    WDB_CTX_SYS_INFO_SET (excCtx.args);
	    }
	else
	    {
	    excCtx.contextType	= WDB_CTX_TASK;
	    excCtx.numArgs	= 2;
	    WDB_CTX_TASK_INFO_SET (excCtx.args, taskIdCurrent,
	    			  taskIdCurrent->rtpId);
	    }
	}

    /* call WDB service to send event to tools */

    (*vxExcHook) (&stoppedCtx, &excCtx, vecNum,
    			sigNum, (void *) pEsf, pRegs, systemSuspend);

    /* if WDB task got exception, restart it */

    if ((pExcInfo != 0) && (wdbTaskRestartHook != NULL))
    	{
	if (wdbTaskRestartHook (pExcInfo))
	    return (TRUE);
	}

    /* If we are not in debug mode, do not the stop the context */

    if (!sysDebugModeGet ())
    return (FALSE);

    /* Stop the context that received the exception */

    if (ctxToStop != 0)
	{
#ifdef	INCLUDE_VXDBG_RUNCTRL
	VXDBG_CTX	vxdbgCtx;

	if (excType == EDR_FACILITY_KERNEL)
	    vxdbgCtx.ctxType = VXDBG_CTX_TASK;
	else
	    vxdbgCtx.ctxType = VXDBG_CTX_RTP;

        vxdbgCtx.ctxId = (VXDBG_CTX_ID) ctxToStop;

	vxdbgStop (pWdbClnt, &vxdbgCtx, TRUE);
#else	/* INCLUDE_VXDBG_RUNCTRL*/
	if (excType == EDR_FACILITY_KERNEL)
	    taskStopForce ((TASK_ID) ctxToStop);
#ifdef INCLUDE_RTP
	else
	    rtpTaskAllStop ((RTP_ID) ctxToStop);
#endif	/* INCLUDE_RTP */
#endif	/* INCLUDE_VXDBG_RUNCTRL*/
	}

    return (TRUE);
    }

/******************************************************************************
*
* vxExcHookAdd - add an exception hook
*
* This routine adds an exception hook.
*
* RETURNS: OK
*
* NOMANUAL
*/

static UINT32 vxExcHookAdd
    (
    void (*hook)
    		(
		WDB_CTX *	pStoppedCtx,
		WDB_CTX	*	pExcCtx,
		UINT32		vec,
		UINT32		sigNum,
		void *		pEsf,
		WDB_IU_REGS *	pRegs,
		BOOL		systemSuspend
		)
    )
    {
    vxExcHook = hook;

#ifdef	INCLUDE_EDR_POLICY_HOOKS
    edrPolicyHandlerHookAdd (vxExcHookWrapper);
#endif	/* INCLUDE_EDR_POLICY_HOOKS */

    return (OK);
    }

/******************************************************************************
*
* wdbRtIfInit - Initialize pointers to the VxWorks routines.
*
* This routine initializes the WDB runtime interface.
*
* RETURNS: N/A
*
* NOMANUAL
*/

static void wdbRtIfInit (void)
    {
    int 	ix = 0;

    bzero ((char *)&wdbRtIf, sizeof (WDB_RT_IF));

    wdbRtIf.rtInfoGet		= wdbRtInfoGet;
    wdbRtIf.rtConnectInfoGet	= wdbRtConnectInfoGet;
    wdbRtIf.reboot		= vxReboot;
    wdbRtIf.excHookAdd		= vxExcHookAdd;

    wdbRtLibInit (&wdbRtIf);		/* initialize core routines */

#ifdef	INCLUDE_WDB_MEM
    wdbRtMemInit (&wdbRtIf);		/* initialize move/scan utilities */
#endif	/* INCLUDE_WDB_MEM */

#if	ARM_HAS_MPU
    /*
     * If a MPU is used instead of a MMU, set page size to 0 to devalidate
     * memory protection. This happens only on ARM.
     */

    wdbRtIf.pageSize   	= 0;
#endif	/* ARM_HAS_MPU */

    /* first check if boot line is empty (eg : no network) */

    if (*sysBootLine != EOS)
	{
	for (ix = 0; ix < MAX_LEN; ix ++)
	    {
	    if (*(sysBootLine + ix) == ')')
		{
		ix++;
		break;
		}
	    }

	/* Copy the bootline following the ')' to vxBootFile */
	strncpy (vxBootFile, sysBootLine + ix, sizeof (vxBootFile));

	/* Truncate vxBootFile at the first space */
	for (ix = 0; ix < MAX_LEN - 1; ix ++)
	    {
	    if (*(vxBootFile + ix) == ' ')
		break;
	    }
	}

    *(vxBootFile + ix) = '\0';

    wdbInstallRtIf (&wdbRtIf);
    }

/******************************************************************************
*
* wdbCommIfInit - Initialize the agent's communction interface
*
* RETURNS : OK or error if we can't initialize the communication interface.
*
* NOMANUAL
*/

static STATUS wdbCommIfInit (void)
    {
    static WDB_XPORT	wdbXport;
    static WDB_COMM_IF	wdbCommIf;
    char *		pWdbInBuf;
    char *		pWdbOutBuf;

    wdbPort = (UINT16) WDB_COMM_PORT;	/* set the communication port to the */
    					/* value defined in the project tool */

    wdbMbufInit (mbufs, NUM_MBUFS, wdbClBlks, WDB_NUM_CL_BLKS);

    /* initialize the agent communication interface device */

    memset (&wdbCommIf, 0, sizeof (wdbCommIf));

    if (wdbCommDevInit (&wdbCommIf, &pWdbInBuf, &pWdbOutBuf) == ERROR)
	return (ERROR);
    wdbCommIf.commId = &wdbCommIf;

    /*
     * Install the agents communication interface and RPC transport handle.
     * Currently only one agent will be active at a time, so both
     * agents can share the same communication interface and XPORT handle.
     */

    wdbRpcXportInit  (&wdbXport, &wdbCommIf, pWdbInBuf, pWdbOutBuf, wdbCommMtu);
    wdbInstallCommIf (&wdbCommIf, &wdbXport, wdbCommMtu);

    return (OK);
    }
