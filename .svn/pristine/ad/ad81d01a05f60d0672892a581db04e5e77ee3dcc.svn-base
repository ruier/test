/* symTbl.c - standalone symbol tables wrapper */

/* CREATED BY /view/ot.26_share.2013091112/wind/vxworks-6.9/host/resource/hutils/tcl/makeSymTbl.tcl
 *  WITH ARGS ppc tmp.o symTbl.c
 *         ON Wed Sep 11 17:20:18 EDT 2013
 */

#include "vxWorks.h"
#include "symbol.h"

IMPORT int MD5_Final ();
IMPORT int MD5_Init ();
IMPORT int MD5_Transform ();
IMPORT int MD5_Update ();
IMPORT int MD5_version;
IMPORT int QorIQLawAlloc_desc;
IMPORT int QorIQLawFind_desc;
IMPORT int QorIQLawFree_desc;
IMPORT int QorIQdcbf ();
IMPORT int QorIQdcbi ();
IMPORT int QorIQdcbt ();
IMPORT int QorIQdcbzl ();
IMPORT int QorIQlwsync ();
IMPORT int SHA1_Final ();
IMPORT int SHA1_Init ();
IMPORT int SHA1_Transform ();
IMPORT int SHA1_Update ();
IMPORT int SHA1_version;
IMPORT int SetNewSeedtoRandomGenerator ();
IMPORT int ZBUF_GETSEG ();
IMPORT int _Randseed;
IMPORT int ___x_diab_sfpddtoll_o;
IMPORT int ___x_diab_sfpdlltod_o;
IMPORT int ___x_diab_sfpdnorm_o;
IMPORT int ___x_diab_sxpasr64_o;
IMPORT int ___x_diab_sxpdiv64_o;
IMPORT int ___x_diab_sxplsl64_o;
IMPORT int ___x_diab_sxplsr64_o;
IMPORT int ___x_diab_sxprem64_o;
IMPORT int ___x_diab_va_arg_o;
IMPORT int __asr64 ();
IMPORT int __assert ();
IMPORT int __clocale;
IMPORT int __costate;
IMPORT int __cplusLoadObjFiles;
IMPORT int __cplusLoad_o;
IMPORT int __cplusUsr_o;
IMPORT int __cplusXtors_o;
IMPORT int __ctype;
IMPORT int __ctype_tolower;
IMPORT int __ctype_toupper;
IMPORT int __daysSinceEpoch ();
IMPORT int __div64 ();
IMPORT int __dtoll ();
IMPORT int __errno ();
IMPORT int __getDstInfo ();
IMPORT int __getDstInfoSub ();
IMPORT int __getTime ();
IMPORT int __getZoneInfo ();
IMPORT int __julday ();
IMPORT int __lltod ();
IMPORT int __locale;
IMPORT int __loctime;
IMPORT int __lsl64 ();
IMPORT int __lsr64 ();
IMPORT int __netlinkref_uipc_mbufutil1;
IMPORT int __rem64 ();
IMPORT int __rta_longjmp;
IMPORT int __saverest_o;
IMPORT int __sclose ();
IMPORT int __sflags ();
IMPORT int __sflush ();
IMPORT int __sfvwrite ();
IMPORT int __smakebuf ();
IMPORT int __sread ();
IMPORT int __srefill ();
IMPORT int __srget ();
IMPORT int __sseek ();
IMPORT int __stderr ();
IMPORT int __stdin ();
IMPORT int __stdout ();
IMPORT int __strxfrm ();
IMPORT int __swbuf ();
IMPORT int __swrite ();
IMPORT int __swsetup ();
IMPORT int __taskVarAdd ();
IMPORT int __taskVarDelete ();
IMPORT int __taskVarGet ();
IMPORT int __taskVarSet ();
IMPORT int __udiv64 ();
IMPORT int __urem64 ();
IMPORT int __va_arg ();
IMPORT int _applLog ();
IMPORT int _archHelp_msg;
IMPORT int _archIORead16 ();
IMPORT int _archIORead32 ();
IMPORT int _archIORead8 ();
IMPORT int _archIOWrite16 ();
IMPORT int _archIOWrite32 ();
IMPORT int _archIOWrite8 ();
IMPORT int _binary_fsl_fman_ucode_P3_P4_P5_101_8_bin_end;
IMPORT int _binary_fsl_fman_ucode_P3_P4_P5_101_8_bin_start;
IMPORT int _binary_fsl_fman_ucode_P4080_106_2_4_bin_end;
IMPORT int _binary_fsl_fman_ucode_P4080_106_2_4_bin_start;
IMPORT int _binary_fsl_fman_ucode_p4080_92_2_bin_end;
IMPORT int _binary_fsl_fman_ucode_p4080_92_2_bin_start;
IMPORT int _clockRealtime;
IMPORT int _d_dtoll ();
IMPORT int _d_lltod ();
IMPORT int _dbgArchInit ();
IMPORT int _dbgBrkDisplayHard ();
IMPORT int _dbgDsmInstRtn;
IMPORT int _dbgFuncCallCheck ();
IMPORT int _dbgInfoPCGet ();
IMPORT int _dbgInstSizeGet ();
IMPORT int _dbgRetAdrsGet ();
IMPORT int _dbgTaskPCGet ();
IMPORT int _dbgTaskPCSet ();
IMPORT int _endM2Packet_1213 ();
IMPORT int _endM2Packet_2233 ();
IMPORT int _func_aimMmuLockRegionsImport;
IMPORT int _func_cacheArchDisableFromMmu;
IMPORT int _func_cacheArchEnable;
IMPORT int _func_cacheFuncsSet;
IMPORT int _func_classListAdd;
IMPORT int _func_classListInit;
IMPORT int _func_classListLock;
IMPORT int _func_classListRemove;
IMPORT int _func_classListTblInit;
IMPORT int _func_classListUnlock;
IMPORT int _func_consoleOut;
IMPORT int _func_coprocShow;
IMPORT int _func_coprocTaskRegsShow;
IMPORT int _func_cpuPwrIdleEnter;
IMPORT int _func_cpuPwrIntEnterHook;
IMPORT int _func_cpuPwrIntExitHook;
IMPORT int _func_cpuPwrLoadContext;
IMPORT int _func_cpuPwrPriorityRaise;
IMPORT int _func_dosFsFmtRtn;
IMPORT int _func_end2VxBusConnect;
IMPORT int _func_end8023AddressForm;
IMPORT int _func_endEtherAddressForm;
IMPORT int _func_endEtherPacketAddrGet;
IMPORT int _func_endEtherPacketDataGet;
IMPORT int _func_envGet;
IMPORT int _func_eventReceive;
IMPORT int _func_eventRsrcSend;
IMPORT int _func_eventRsrcShow;
IMPORT int _func_eventSend;
IMPORT int _func_eventTaskShow;
IMPORT int _func_eventTerminate;
IMPORT int _func_excBaseHook;
IMPORT int _func_excInfoShow;
IMPORT int _func_excIntHook;
IMPORT int _func_excJobAdd;
IMPORT int _func_excOsm;
IMPORT int _func_excPanicHook;
IMPORT int _func_excTrapRtn;
IMPORT int _func_exit;
IMPORT int _func_fclose;
IMPORT int _func_fcntl_dup;
IMPORT int _func_fileDoesNotExist;
IMPORT int _func_fioFltFormatRtn;
IMPORT int _func_fioFltScanRtn;
IMPORT int _func_ftpLs;
IMPORT int _func_ftpTransientFatal;
IMPORT int _func_ftruncMmanHook;
IMPORT int _func_handleAlloc;
IMPORT int _func_handleFreeAll;
IMPORT int _func_intConnectRtn;
IMPORT int _func_intDisableRtn;
IMPORT int _func_intDisconnectRtn;
IMPORT int _func_intEnableRtn;
IMPORT int _func_intLevelSetRtn;
IMPORT int _func_intVecBaseGetRtn;
IMPORT int _func_intVecBaseSetRtn;
IMPORT int _func_intVecGetRtn;
IMPORT int _func_intVecSetRtn;
IMPORT int _func_ioGlobalStdGet;
IMPORT int _func_ioPxCreateOrOpen;
IMPORT int _func_ioTaskStdSet;
IMPORT int _func_iosPathFdEntryIoctl;
IMPORT int _func_iosPosixFdEntryIoctl;
IMPORT int _func_iosRtpIoTableSizeGet;
IMPORT int _func_iosRtpIoTableSizeSet;
IMPORT int _func_isrConnect;
IMPORT int _func_isrCreate;
IMPORT int _func_isrDelete;
IMPORT int _func_isrDisconnect;
IMPORT int _func_isrDispatcher;
IMPORT int _func_isrInfo;
IMPORT int _func_jobTaskWorkAdd;
IMPORT int _func_kernelModuleListIdGet;
IMPORT int _func_kernelRoundRobinHook;
IMPORT int _func_kprintf;
IMPORT int _func_kwrite;
IMPORT int _func_lf_advlock;
IMPORT int _func_logMsg;
IMPORT int _func_loginPrompt2;
IMPORT int _func_loginStringGet;
IMPORT int _func_loginStringSet;
IMPORT int _func_m2PollStatsIfPoll;
IMPORT int _func_memPartCacheAlloc;
IMPORT int _func_memPartCacheFree;
IMPORT int _func_memPartCacheInfoGet;
IMPORT int _func_memPartRealloc;
IMPORT int _func_memPartReallocInternal;
IMPORT int _func_memalign;
IMPORT int _func_mmuCacheFlushLoadAdrsGet;
IMPORT int _func_moduleIdFigure;
IMPORT int _func_moduleSegInfoGet;
IMPORT int _func_mux;
IMPORT int _func_mux_l2;
IMPORT int _func_netAttrDeReg;
IMPORT int _func_netAttrReg;
IMPORT int _func_netChDirByName;
IMPORT int _func_netDaemonIxToQId;
IMPORT int _func_netDaemonQIdToIx;
IMPORT int _func_netLsByName;
IMPORT int _func_netPoolShow;
IMPORT int _func_objCoreRelease;
IMPORT int _func_objHandleToObjId;
IMPORT int _func_objNameVerifiedSet;
IMPORT int _func_objOwnerListAdd;
IMPORT int _func_objOwnerListRemove;
IMPORT int _func_objOwnerReclaim;
IMPORT int _func_objOwnerSetBaseInternal;
IMPORT int _func_objOwnerSetInternal;
IMPORT int _func_objRtpObjEachHook;
IMPORT int _func_ownerListLock;
IMPORT int _func_ownerListUnlock;
IMPORT int _func_printErr;
IMPORT int _func_printExcPrintHook;
IMPORT int _func_pthread_testandset_canceltype;
IMPORT int _func_qPriBMapNativeResort;
IMPORT int _func_remCurIdGet;
IMPORT int _func_remCurIdSet;
IMPORT int _func_rtpDelete;
IMPORT int _func_rtpDeleteHookAdd;
IMPORT int _func_rtpDeleteHookDelete;
IMPORT int _func_rtpHandleIdGet;
IMPORT int _func_rtpIdVerify;
IMPORT int _func_rtpInitCompleteHookAdd;
IMPORT int _func_rtpInitCompleteHookDelete;
IMPORT int _func_rtpPause;
IMPORT int _func_rtpPostCreateHookAdd;
IMPORT int _func_rtpPostCreateHookDelete;
IMPORT int _func_rtpPreCreateHookAdd;
IMPORT int _func_rtpPreCreateHookDelete;
IMPORT int _func_rtpSdUnmapAll;
IMPORT int _func_rtpShlDeleteAll;
IMPORT int _func_rtpShlInfoGet;
IMPORT int _func_rtpShlSymbolsRegister;
IMPORT int _func_rtpShlSymbolsUnregister;
IMPORT int _func_rtpShow;
IMPORT int _func_rtpSigExcKill;
IMPORT int _func_rtpSigPendDestroy;
IMPORT int _func_rtpSigPendInit;
IMPORT int _func_rtpSigPendKill;
IMPORT int _func_rtpSigaction;
IMPORT int _func_rtpSigpending;
IMPORT int _func_rtpSigprocmask;
IMPORT int _func_rtpSigqueue;
IMPORT int _func_rtpSigsuspend;
IMPORT int _func_rtpSigtimedwait;
IMPORT int _func_rtpSpawn;
IMPORT int _func_rtpSymTblIdGet;
IMPORT int _func_rtpTaskKill;
IMPORT int _func_rtpTaskSigBlock;
IMPORT int _func_rtpTaskSigCheck;
IMPORT int _func_rtpTaskSigPendKill;
IMPORT int _func_rtpTaskSigqueue;
IMPORT int _func_rtpVarAdd;
IMPORT int _func_rtpVerifiedLock;
IMPORT int _func_selPtyAdd;
IMPORT int _func_selPtyDelete;
IMPORT int _func_selPtySlaveAdd;
IMPORT int _func_selPtySlaveDelete;
IMPORT int _func_selTyAdd;
IMPORT int _func_selTyDelete;
IMPORT int _func_selWakeup;
IMPORT int _func_selWakeupAll;
IMPORT int _func_selWakeupListInit;
IMPORT int _func_selWakeupListTerm;
IMPORT int _func_semExchange;
IMPORT int _func_semRTake;
IMPORT int _func_semWTake;
IMPORT int _func_shellDbgCallPrint;
IMPORT int _func_shellDbgStackTrace;
IMPORT int _func_shellExcPrint;
IMPORT int _func_sigExcKill;
IMPORT int _func_sigTimeoutRecalc;
IMPORT int _func_sigeventInit;
IMPORT int _func_sigfillset;
IMPORT int _func_signalMmanHook;
IMPORT int _func_sigprocmask;
IMPORT int _func_spy;
IMPORT int _func_spy2;
IMPORT int _func_spyClkStart;
IMPORT int _func_spyClkStop;
IMPORT int _func_spyReport;
IMPORT int _func_spyReport2;
IMPORT int _func_spyStop;
IMPORT int _func_spyTask;
IMPORT int _func_symEach;
IMPORT int _func_symEachCall;
IMPORT int _func_symFind;
IMPORT int _func_symFindSymbol;
IMPORT int _func_symNameGet;
IMPORT int _func_symRegister;
IMPORT int _func_symTblShutdown;
IMPORT int _func_symTypeGet;
IMPORT int _func_symValueGet;
IMPORT int _func_sysDoze;
IMPORT int _func_taskCreateHookAdd;
IMPORT int _func_taskDeleteHookAdd;
IMPORT int _func_taskMemCtxSwitch;
IMPORT int _func_taskStackAlloc;
IMPORT int _func_taskStackFree;
IMPORT int _func_tipStart;
IMPORT int _func_unldByModuleId;
IMPORT int _func_valloc;
IMPORT int _func_virtExtraMapInfoGet;
IMPORT int _func_virtSegInfoGet;
IMPORT int _func_vmBaseFuncsSet;
IMPORT int _func_vmInvPageMap;
IMPORT int _func_vmInvUnMap;
IMPORT int _func_vxEpicIpiCtrlInitIpiEmit;
IMPORT int _func_vxMemProbeHook;
IMPORT int _func_vxSvrGet;
IMPORT int _func_vxbDelayCommon;
IMPORT int _func_vxbIntConnect;
IMPORT int _func_vxbIntDisable;
IMPORT int _func_vxbIntDisconnect;
IMPORT int _func_vxbIntDynaConnect;
IMPORT int _func_vxbIntDynaCtlrInputInit;
IMPORT int _func_vxbIntDynaVecAlloc;
IMPORT int _func_vxbIntDynaVecDevMultiProgram;
IMPORT int _func_vxbIntDynaVecDisable;
IMPORT int _func_vxbIntDynaVecEnable;
IMPORT int _func_vxbIntDynaVecErase;
IMPORT int _func_vxbIntDynaVecGet;
IMPORT int _func_vxbIntDynaVecOwnerFind;
IMPORT int _func_vxbIntDynaVecProgram;
IMPORT int _func_vxbIntEnable;
IMPORT int _func_vxbPciDevCfgRead;
IMPORT int _func_vxbPciDevCfgWrite;
IMPORT int _func_vxbSysEpicIntHandler;
IMPORT int _func_vxbUserHookDevInit;
IMPORT int _func_vxdbgBpDelete;
IMPORT int _func_vxdbgCtxCont;
IMPORT int _func_vxdbgCtxStop;
IMPORT int _func_vxdbgMemMove;
IMPORT int _func_vxdbgMemRead;
IMPORT int _func_vxdbgMemScan;
IMPORT int _func_vxdbgMemWrite;
IMPORT int _func_vxdbgRtpCont;
IMPORT int _func_vxdbgRtpIdVerify;
IMPORT int _func_vxdbgRtpStop;
IMPORT int _func_vxdbgTrap;
IMPORT int _func_wdbTrap;
IMPORT int _func_windSemRWDelete;
IMPORT int _func_windTickAnnounceHook;
IMPORT int _func_workQDbgTextDump;
IMPORT int _hostAdd ();
IMPORT int _hostDelete ();
IMPORT int _hostGetByAddr ();
IMPORT int _hostGetByName ();
IMPORT int _hostTblSearchByName2 ();
IMPORT int _ipcom_gethostbyaddr_r;
IMPORT int _ipcom_gethostbyname_r;
IMPORT int _ipdhcpc_callback_hook;
IMPORT int _linkMemReqClAlign;
IMPORT int _linkMemReqMlinkAlign;
IMPORT int _muxProtosPerEndInc;
IMPORT int _muxProtosPerEndStart;
IMPORT int _netMemReqDefault ();
IMPORT int _netSysctl;
IMPORT int _netSysctlCli;
IMPORT int _pLinkPoolFuncTbl;
IMPORT int _pNetBufCollect;
IMPORT int _pNetDpool;
IMPORT int _pNetPoolFuncAlignTbl;
IMPORT int _pNetPoolFuncTbl;
IMPORT int _pNetSysPool;
IMPORT int _pSigQueueFreeHead;
IMPORT int _pSysL2CacheDisable;
IMPORT int _pSysL2CacheEnable;
IMPORT int _pSysL2CacheFlush;
IMPORT int _pSysL2CacheInvFunc;
IMPORT int _pSysL3CacheFlushDisableFunc;
IMPORT int _pSysL3CacheInvalEnableFunc;
IMPORT int _panicHook;
IMPORT int _ppcAllocationQuantumSize;
IMPORT int _ppcCacheLineSize;
IMPORT int _ppcStackAlignSize;
IMPORT int _procNumWasSet;
IMPORT int _restfpr_14_l ();
IMPORT int _restfpr_15_l ();
IMPORT int _restfpr_16_l ();
IMPORT int _restfpr_17_l ();
IMPORT int _restfpr_18_l ();
IMPORT int _restfpr_19_l ();
IMPORT int _restfpr_20_l ();
IMPORT int _restfpr_21_l ();
IMPORT int _restfpr_22_l ();
IMPORT int _restfpr_23_l ();
IMPORT int _restfpr_24_l ();
IMPORT int _restfpr_25_l ();
IMPORT int _restfpr_26_l ();
IMPORT int _restfpr_27_l ();
IMPORT int _restfpr_28_l ();
IMPORT int _restfpr_29_l ();
IMPORT int _restfpr_30_l ();
IMPORT int _restfpr_31_l ();
IMPORT int _restgpr_14 ();
IMPORT int _restgpr_14_l ();
IMPORT int _restgpr_15 ();
IMPORT int _restgpr_15_l ();
IMPORT int _restgpr_16 ();
IMPORT int _restgpr_16_l ();
IMPORT int _restgpr_17 ();
IMPORT int _restgpr_17_l ();
IMPORT int _restgpr_18 ();
IMPORT int _restgpr_18_l ();
IMPORT int _restgpr_19 ();
IMPORT int _restgpr_19_l ();
IMPORT int _restgpr_20 ();
IMPORT int _restgpr_20_l ();
IMPORT int _restgpr_21 ();
IMPORT int _restgpr_21_l ();
IMPORT int _restgpr_22 ();
IMPORT int _restgpr_22_l ();
IMPORT int _restgpr_23 ();
IMPORT int _restgpr_23_l ();
IMPORT int _restgpr_24 ();
IMPORT int _restgpr_24_l ();
IMPORT int _restgpr_25 ();
IMPORT int _restgpr_25_l ();
IMPORT int _restgpr_26 ();
IMPORT int _restgpr_26_l ();
IMPORT int _restgpr_27 ();
IMPORT int _restgpr_27_l ();
IMPORT int _restgpr_28 ();
IMPORT int _restgpr_28_l ();
IMPORT int _restgpr_29 ();
IMPORT int _restgpr_29_l ();
IMPORT int _restgpr_30 ();
IMPORT int _restgpr_30_l ();
IMPORT int _restgpr_31 ();
IMPORT int _restgpr_31_l ();
IMPORT int _savefpr_14_l ();
IMPORT int _savefpr_15_l ();
IMPORT int _savefpr_16_l ();
IMPORT int _savefpr_17_l ();
IMPORT int _savefpr_18_l ();
IMPORT int _savefpr_19_l ();
IMPORT int _savefpr_20_l ();
IMPORT int _savefpr_21_l ();
IMPORT int _savefpr_22_l ();
IMPORT int _savefpr_23_l ();
IMPORT int _savefpr_24_l ();
IMPORT int _savefpr_25_l ();
IMPORT int _savefpr_26_l ();
IMPORT int _savefpr_27_l ();
IMPORT int _savefpr_28_l ();
IMPORT int _savefpr_29_l ();
IMPORT int _savefpr_30_l ();
IMPORT int _savefpr_31_l ();
IMPORT int _savegpr_14 ();
IMPORT int _savegpr_14_l ();
IMPORT int _savegpr_15 ();
IMPORT int _savegpr_15_l ();
IMPORT int _savegpr_16 ();
IMPORT int _savegpr_16_l ();
IMPORT int _savegpr_17 ();
IMPORT int _savegpr_17_l ();
IMPORT int _savegpr_18 ();
IMPORT int _savegpr_18_l ();
IMPORT int _savegpr_19 ();
IMPORT int _savegpr_19_l ();
IMPORT int _savegpr_20 ();
IMPORT int _savegpr_20_l ();
IMPORT int _savegpr_21 ();
IMPORT int _savegpr_21_l ();
IMPORT int _savegpr_22 ();
IMPORT int _savegpr_22_l ();
IMPORT int _savegpr_23 ();
IMPORT int _savegpr_23_l ();
IMPORT int _savegpr_24 ();
IMPORT int _savegpr_24_l ();
IMPORT int _savegpr_25 ();
IMPORT int _savegpr_25_l ();
IMPORT int _savegpr_26 ();
IMPORT int _savegpr_26_l ();
IMPORT int _savegpr_27 ();
IMPORT int _savegpr_27_l ();
IMPORT int _savegpr_28 ();
IMPORT int _savegpr_28_l ();
IMPORT int _savegpr_29 ();
IMPORT int _savegpr_29_l ();
IMPORT int _savegpr_30 ();
IMPORT int _savegpr_30_l ();
IMPORT int _savegpr_31 ();
IMPORT int _savegpr_31_l ();
IMPORT int _setjmpSetup ();
IMPORT int _sigCtxLoad ();
IMPORT int _sigCtxRtnValSet ();
IMPORT int _sigCtxSave ();
IMPORT int _sigCtxSetup ();
IMPORT int _sigCtxStackEnd ();
IMPORT int _sigfaulttable;
IMPORT int _sysInit ();
IMPORT int _taskSuspend ();
IMPORT int _vmpByte ();
IMPORT int _vmpByteEnd ();
IMPORT int _vmpByteRead ();
IMPORT int _vmpByteWrite ();
IMPORT int _vmpShort ();
IMPORT int _vmpShortEnd ();
IMPORT int _vmpShortRead ();
IMPORT int _vmpShortWrite ();
IMPORT int _vmpWord ();
IMPORT int _vmpWordEnd ();
IMPORT int _vmpWordRead ();
IMPORT int _vmpWordWrite ();
IMPORT int _vxb_clkConnectRtn;
IMPORT int _vxb_clkDisableRtn;
IMPORT int _vxb_clkEnableRtn;
IMPORT int _vxb_clkRateGetRtn;
IMPORT int _vxb_clkRateSetRtn;
IMPORT int _vxb_delayRtn;
IMPORT int _vxb_msDelayRtn;
IMPORT int _vxb_usDelayRtn;
IMPORT int _vxmux_pNullPoolFuncTbl;
IMPORT int _vxworks_asctime_r ();
IMPORT int _vxworks_ctime_r ();
IMPORT int _vxworks_gmtime_r ();
IMPORT int _vxworks_localtime_r ();
IMPORT int _zbufCreate_nolock ();
IMPORT int _zbufDeleteEmpty ();
IMPORT int _zbufSetSeg ();
IMPORT int _zbuf_getseg_nolock ();
IMPORT int _zbuf_getsegptr_nolock ();
IMPORT int abort ();
IMPORT int abs ();
IMPORT int absSymbols_Common ();
IMPORT int accept ();
IMPORT int access ();
IMPORT int acos ();
IMPORT int aimCacheInit ();
IMPORT int aimFppLibInit ();
IMPORT int aimMmuBaseLibInit ();
IMPORT int aimMmuContextTbl;
IMPORT int aimMmuLibInit ();
IMPORT int aimMmuNullPageTbl;
IMPORT int aimMmuNullRegionTbl;
IMPORT int allocChunkEndGuardSize;
IMPORT int applLoggerInit ();
IMPORT int applLoggerStop ();
IMPORT int applUtilInstInit ();
IMPORT int applUtilLogSem;
IMPORT int asctime ();
IMPORT int asctime_r ();
IMPORT int asin ();
IMPORT int assertFiles;
IMPORT int atan ();
IMPORT int atan2 ();
IMPORT int atexit ();
IMPORT int atof ();
IMPORT int atoi ();
IMPORT int atol ();
IMPORT int attrib ();
IMPORT int avlUintDelete ();
IMPORT int avlUintInsert ();
IMPORT int avlUintMaximumGet ();
IMPORT int avlUintMinimumGet ();
IMPORT int avlUintPredecessorGet ();
IMPORT int avlUintSearch ();
IMPORT int avlUintSuccessorGet ();
IMPORT int avlUintTreeWalk ();
IMPORT int b ();
IMPORT int baudRateGenClk;
IMPORT int bcmp ();
IMPORT int bcopy ();
IMPORT int bcopyBytes ();
IMPORT int bcopyLongs ();
IMPORT int bcopyWords ();
IMPORT int bd ();
IMPORT int bdall ();
IMPORT int bfill ();
IMPORT int bfillBytes ();
IMPORT int bh ();
IMPORT int bi ();
IMPORT int bind ();
IMPORT int bindresvport ();
IMPORT int bindresvportCommon ();
IMPORT int bindresvport_af ();
IMPORT int binvert ();
IMPORT int bman0Resources;
IMPORT int bmanRegister ();
IMPORT int bootBpAnchorExtract ();
IMPORT int bootChange ();
IMPORT int bootLeaseExtract ();
IMPORT int bootNetmaskExtract ();
IMPORT int bootParamsErrorPrint ();
IMPORT int bootParamsPrompt ();
IMPORT int bootParamsShow ();
IMPORT int bootScanNum ();
IMPORT int bootStringToStruct ();
IMPORT int bootStringToStructAdd ();
IMPORT int bootStructToString ();
IMPORT int bportalAcquire ();
IMPORT int bportalAlloc ();
IMPORT int bportalCountGet ();
IMPORT int bportalCreate ();
IMPORT int bportalDestroy ();
IMPORT int bportalFree ();
IMPORT int bportalIntConnect ();
IMPORT int bportalIntDisable ();
IMPORT int bportalIntDisconnect ();
IMPORT int bportalIntEnable ();
IMPORT int bportalLocalAlloc ();
IMPORT int bportalLocalFree ();
IMPORT int bportalLocalMultiAlloc ();
IMPORT int bportalLocalMultiFree ();
IMPORT int bportalMultiAlloc ();
IMPORT int bportalMultiFree ();
IMPORT int bportalQuery ();
IMPORT int bportalRelease ();
IMPORT int bportalSafeAlloc ();
IMPORT int bportalSafeFree ();
IMPORT int bportalSafeMultiAlloc ();
IMPORT int bportalSafeMultiFree ();
IMPORT int bsearch ();
IMPORT int bspSerialChanGet ();
IMPORT int bswap ();
IMPORT int busCtlrDevCtlr_desc;
IMPORT int busDevShow_desc;
IMPORT int bzero ();
IMPORT int c ();
IMPORT int cacheAddrAlign;
IMPORT int cacheAimE500ClearD ();
IMPORT int cacheAimE500DisableDFromMmu ();
IMPORT int cacheAimE500DisableD_Impl ();
IMPORT int cacheAimE500DisableIFromMmu ();
IMPORT int cacheAimE500DisableI_Impl ();
IMPORT int cacheAimE500EnableD_Impl ();
IMPORT int cacheAimE500EnableI_Impl ();
IMPORT int cacheAimE500FlushAllD ();
IMPORT int cacheAimE500FlushD ();
IMPORT int cacheAimE500InvalidateD ();
IMPORT int cacheAimE500InvalidateI ();
IMPORT int cacheAimE500PipeFlush ();
IMPORT int cacheAimE500TextUpdate ();
IMPORT int cacheAimE500dCachePEShutdown ();
IMPORT int cacheAimInfo;
IMPORT int cacheArchDmaFree ();
IMPORT int cacheArchDmaMalloc ();
IMPORT int cacheArchEnable ();
IMPORT int cacheArchLibInit ();
IMPORT int cacheClear ();
IMPORT int cacheDToEnable;
IMPORT int cacheDataEnabled;
IMPORT int cacheDataMode;
IMPORT int cacheDisable ();
IMPORT int cacheDma32Free ();
IMPORT int cacheDma32Malloc ();
IMPORT int cacheDmaFree ();
IMPORT int cacheDmaFreeRtn;
IMPORT int cacheDmaFuncs;
IMPORT int cacheDmaMalloc ();
IMPORT int cacheDmaMallocRtn;
IMPORT int cacheDrvFlush ();
IMPORT int cacheDrvInvalidate ();
IMPORT int cacheDrvPhysToVirt ();
IMPORT int cacheDrvVirtToPhys ();
IMPORT int cacheEnable ();
IMPORT int cacheFlush ();
IMPORT int cacheForeignClear ();
IMPORT int cacheForeignFlush ();
IMPORT int cacheForeignInvalidate ();
IMPORT int cacheFuncsSet ();
IMPORT int cacheIToEnable;
IMPORT int cacheInvalidate ();
IMPORT int cacheLib;
IMPORT int cacheLibInit ();
IMPORT int cacheLineMask;
IMPORT int cacheLineSize;
IMPORT int cacheLock ();
IMPORT int cacheMmuAvailable;
IMPORT int cacheNullFuncs;
IMPORT int cachePipeFlush ();
IMPORT int cachePpcReadOrigin;
IMPORT int cacheTextLocalUpdate ();
IMPORT int cacheTextUpdate ();
IMPORT int cacheTypeChk ();
IMPORT int cacheUnlock ();
IMPORT int cacheUserFuncs;
IMPORT int calloc ();
IMPORT int catInfo;
IMPORT int ccRatioTable;
IMPORT int ccs1RatioTable;
IMPORT int ccs2RatioTable;
IMPORT int cd ();
IMPORT int ceil ();
IMPORT int cfree ();
IMPORT int changeFpReg ();
IMPORT int changeLogLevel ();
IMPORT int changeReg ();
IMPORT int chdir ();
IMPORT int checkStack ();
IMPORT int chipErrataCpu29Print ();
IMPORT int chkdsk ();
IMPORT int chmod ();
IMPORT int classCreateConnect ();
IMPORT int classDestroyConnect ();
IMPORT int classIdTable;
IMPORT int classInit ();
IMPORT int classLibInit ();
IMPORT int classListLibInit ();
IMPORT int classMemPartIdSet ();
IMPORT int classShow ();
IMPORT int classShowConnect ();
IMPORT int classShowInit ();
IMPORT int clearerr ();
IMPORT int clock ();
IMPORT int clockLibInit ();
IMPORT int clock_getres ();
IMPORT int clock_gettime ();
IMPORT int clock_gettime_wd ();
IMPORT int clock_nanosleep ();
IMPORT int clock_setres ();
IMPORT int clock_settime ();
IMPORT int close ();
IMPORT int closedir ();
IMPORT int commit ();
IMPORT int completionPathComplete ();
IMPORT int completionPathListGet ();
IMPORT int completionPathShow ();
IMPORT int completionSymComplete ();
IMPORT int completionSymListGet ();
IMPORT int completionSymShow ();
IMPORT int connect ();
IMPORT int connectWithTimeout ();
IMPORT int consoleFd;
IMPORT int consoleName;
IMPORT int coprocCtxReplicate ();
IMPORT int coprocCtxStorageSizeGet ();
IMPORT int coprocDescriptorsInit ();
IMPORT int coprocDisable ();
IMPORT int coprocEnable ();
IMPORT int coprocLibInit ();
IMPORT int coprocMRegs ();
IMPORT int coprocRegListShow ();
IMPORT int coprocSave ();
IMPORT int coprocShow ();
IMPORT int coprocShowInit ();
IMPORT int coprocShowOnDebug ();
IMPORT int coprocTaskCreateHook ();
IMPORT int coprocTaskDeleteHook ();
IMPORT int coprocTaskRegsGet ();
IMPORT int coprocTaskRegsSet ();
IMPORT int coprocTaskRegsShow ();
IMPORT int coprocTaskSwapHook ();
IMPORT int coprocsDiscover ();
IMPORT int copy ();
IMPORT int copyStreams ();
IMPORT int copysign ();
IMPORT int cos ();
IMPORT int cosh ();
IMPORT int cp ();
IMPORT int cplusCallCtors ();
IMPORT int cplusCallDtors ();
IMPORT int cplusCtors ();
IMPORT int cplusCtorsLink ();
IMPORT int cplusDemangle ();
IMPORT int cplusDemangle2 ();
IMPORT int cplusDemangle2Func;
IMPORT int cplusDemangleFunc;
IMPORT int cplusDemangleToBuffer ();
IMPORT int cplusDemangleToBufferFunc;
IMPORT int cplusDemanglerMode;
IMPORT int cplusDemanglerSet ();
IMPORT int cplusDemanglerStyle;
IMPORT int cplusDemanglerStyleInit ();
IMPORT int cplusDemanglerStyleSet ();
IMPORT int cplusDtors ();
IMPORT int cplusDtorsLink ();
IMPORT int cplusLoadCtorsCall ();
IMPORT int cplusLoadFixup ();
IMPORT int cplusMangledSymCheck ();
IMPORT int cplusMangledSymGet ();
IMPORT int cplusMatchMangled ();
IMPORT int cplusMatchMangledListGet ();
IMPORT int cplusUnloadFixup ();
IMPORT int cplusUserAsk ();
IMPORT int cplusXtorGet ();
IMPORT int cplusXtorSet ();
IMPORT int cplusXtorStrategy;
IMPORT int cpuPwrLightMgrInit ();
IMPORT int cpuPwrMgrEnable ();
IMPORT int cpuPwrMgrIsEnabled ();
IMPORT int creat ();
IMPORT char * creationDate;
IMPORT int cret ();
IMPORT int ctime ();
IMPORT int ctime_r ();
IMPORT int ctypeFiles;
IMPORT int d ();
IMPORT int dCachePushParityErrorMsg;
IMPORT int dbgBpEpCoreRtn ();
IMPORT int dbgBpListPrint ();
IMPORT int dbgCallPrint ();
IMPORT int dbgDisassemble ();
IMPORT int dbgDynPrintfCommonRtn ();
IMPORT int dbgHelp ();
IMPORT int dbgInit ();
IMPORT int dbgPrintDsp;
IMPORT int dbgPrintFpp;
IMPORT int dbgPrintSimd;
IMPORT int dbgStackTrace ();
IMPORT int dbgTaskTraceCoreRtn ();
IMPORT int ddrRatioTable;
IMPORT int defaultDrv;
IMPORT int devResourceGet ();
IMPORT int devResourceIntrGet ();
IMPORT int devs ();
IMPORT int difftime ();
IMPORT int dirList ();
IMPORT int disableBranchPrediction ();
IMPORT int diskFormat ();
IMPORT int diskInit ();
IMPORT int div ();
IMPORT int div_r ();
IMPORT int dllAdd ();
IMPORT int dllCount ();
IMPORT int dllEach ();
IMPORT int dllGet ();
IMPORT int dllInit ();
IMPORT int dllInsert ();
IMPORT int dllRemove ();
IMPORT int dllTerminate ();
IMPORT int dosfsDiskFormat ();
IMPORT int dprintf ();
IMPORT int dprintfVerboseLvl;
IMPORT int drem ();
IMPORT int drvTable;
IMPORT int dsmFindByInst ();
IMPORT int dsmFindByName ();
IMPORT int dsmInst ();
IMPORT int dtmRegister ();
IMPORT int dtsec0Resources;
IMPORT int dtsec1Resources;
IMPORT int dtsec2Resources;
IMPORT int dtsec3Resources;
IMPORT int dtsec4Resources;
IMPORT int dtsecMdio0Resources;
IMPORT int dummyErrno;
IMPORT int dup ();
IMPORT int dup2 ();
IMPORT int e ();
IMPORT int edrErrorInjectStub ();
IMPORT int edrErrorPolicyHookRemove ();
IMPORT int edrFlagsGet ();
IMPORT int edrInitFatalPolicyHandler ();
IMPORT int edrInterruptFatalPolicyHandler ();
IMPORT int edrIsDebugMode ();
IMPORT int edrKernelFatalPolicyHandler ();
IMPORT int edrPolicyHandlerHookAdd ();
IMPORT int edrPolicyHandlerHookGet ();
IMPORT int edrRtpFatalPolicyHandler ();
IMPORT int edrStubInit ();
IMPORT int edrSystemDebugModeGet ();
IMPORT int edrSystemDebugModeInit ();
IMPORT int edrSystemDebugModeSet ();
IMPORT int enablePteProtection;
IMPORT int end8023AddressForm ();
IMPORT int endDevName ();
IMPORT int endDevTbl;
IMPORT int endEtherAddressForm ();
IMPORT int endEtherCrc32BeGet ();
IMPORT int endEtherCrc32LeGet ();
IMPORT int endEtherHdrInit ();
IMPORT int endEtherPacketAddrGet ();
IMPORT int endEtherPacketDataGet ();
IMPORT int endFindByName ();
IMPORT int endFirstUnitFind ();
IMPORT int endFlagsClr ();
IMPORT int endFlagsGet ();
IMPORT int endFlagsSet ();
IMPORT int endLibInit ();
IMPORT int endList;
IMPORT int endM2Free ();
IMPORT int endM2Init ();
IMPORT int endM2Ioctl ();
IMPORT int endM2Packet;
IMPORT int endMibIfInit ();
IMPORT int endMultiLstCnt ();
IMPORT int endMultiLstFirst ();
IMPORT int endMultiLstNext ();
IMPORT int endObjFlagSet ();
IMPORT int endObjInit ();
IMPORT int endObjectUnload ();
IMPORT int endPollStatsInit ();
IMPORT int endPoolClSize;
IMPORT int endPoolCreate ();
IMPORT int endPoolDestroy ();
IMPORT int endPoolJumboClSize;
IMPORT int endPoolJumboCreate ();
IMPORT int endRcvRtnCall ();
IMPORT int endTxSemGive ();
IMPORT int endTxSemTake ();
IMPORT int envGet ();
IMPORT int envLibInit ();
IMPORT int envLibSem;
IMPORT int envLibSemId;
IMPORT int envPrivateCreate ();
IMPORT int envPrivateDestroy ();
IMPORT int envShow ();
IMPORT int epic0Resources;
IMPORT int epicCpu;
IMPORT int epicInputs;
IMPORT int epicPriority;
IMPORT int epicXBar;
IMPORT int errno;
IMPORT int errnoGet ();
IMPORT int errnoOfTaskGet ();
IMPORT int errnoOfTaskSet ();
IMPORT int errnoSet ();
IMPORT int etherMultiAdd ();
IMPORT int etherMultiDel ();
IMPORT int etherMultiGet ();
IMPORT int etherbroadcastaddr;
IMPORT int eventClear ();
IMPORT int eventInit ();
IMPORT int eventLibInit ();
IMPORT int eventPointStubLibInit ();
IMPORT int eventPointSwHandle ();
IMPORT int eventPointSwStubConnect ();
IMPORT int eventReceive ();
IMPORT int eventRsrcSend ();
IMPORT int eventSend ();
IMPORT int eventStart ();
IMPORT int eventTerminate ();
IMPORT int evtAction;
IMPORT int exDesc;
IMPORT int excConnect ();
IMPORT int excCrtConnect ();
IMPORT int excCrtEnt ();
IMPORT int excCrtExit ();
IMPORT int excDecrHandle ();
IMPORT int excEPSet ();
IMPORT int excEnt ();
IMPORT int excExcHandle ();
IMPORT int excExcepHook;
IMPORT int excExit ();
IMPORT int excExtendedVectors;
IMPORT int excHookAdd ();
IMPORT int excInit ();
IMPORT int excIntConnect ();
IMPORT int excIntCrtConnect ();
IMPORT int excIntHandle ();
IMPORT int excIntNestLogInit ();
IMPORT int excIvorInit ();
IMPORT int excJobAdd ();
IMPORT int excMchkConnect ();
IMPORT int excMchkEnt ();
IMPORT int excMchkExit ();
IMPORT int excShowInit ();
IMPORT int excVecBaseAltAdrs;
IMPORT int excVecBaseGet ();
IMPORT int excVecBaseSet ();
IMPORT int excVecGet ();
IMPORT int excVecInit ();
IMPORT int excVecSet ();
IMPORT int exit ();
IMPORT int exp ();
IMPORT int exp__E ();
IMPORT int expm1 ();
IMPORT int fabs ();
IMPORT int fchmod ();
IMPORT int fclose ();
IMPORT int fcntl ();
IMPORT int fcntl_dup ();
IMPORT int fdClass;
IMPORT int fdInvalidDrv;
IMPORT int fdatasync ();
IMPORT int fdopen ();
IMPORT int fdprintf ();
IMPORT int feof ();
IMPORT int ferror ();
IMPORT int fflush ();
IMPORT int ffs32Lsb ();
IMPORT int ffs32Msb ();
IMPORT int ffs64Lsb ();
IMPORT int ffs64Msb ();
IMPORT int ffsLsb ();
IMPORT int ffsMsb ();
IMPORT int fgetc ();
IMPORT int fgetpos ();
IMPORT int fgets ();
IMPORT int fieldSzIncludeSign;
IMPORT int fileno ();
IMPORT int finite ();
IMPORT int fioBaseLibInit ();
IMPORT int fioBufPrint ();
IMPORT int fioBufPut ();
IMPORT int fioFltInstall ();
IMPORT int fioFormatV ();
IMPORT int fioLibInit ();
IMPORT int fioRdString ();
IMPORT int fioRead ();
IMPORT int fioScanV ();
IMPORT int fioSnBufPut ();
IMPORT int firstStaticDesc;
IMPORT int flashDelayAdj;
IMPORT int flashDelayIncr;
IMPORT int flashDelayShift;
IMPORT int floatInit ();
IMPORT int floor ();
IMPORT int fman0Resources;
IMPORT int fman1Resources;
IMPORT int fmanBmiPoolInfoGet ();
IMPORT int fmanBmiPoolInfoSet ();
IMPORT int fmanBmiPortConfig ();
IMPORT int fmanBmiPortHalt ();
IMPORT int fmanCcConfigAdd ();
IMPORT int fmanCcConfigFlush ();
IMPORT int fmanCcPortConfig ();
IMPORT int fmanCcPortHalt ();
IMPORT int fmanKgHash ();
IMPORT int fmanKgPartConfRead ();
IMPORT int fmanKgPartConfWrite ();
IMPORT int fmanKgPlanRead ();
IMPORT int fmanKgPlanWrite ();
IMPORT int fmanKgPortConfig ();
IMPORT int fmanKgPortHalt ();
IMPORT int fmanKgSchemeRead ();
IMPORT int fmanKgSchemeWrite ();
IMPORT int fmanPlPortConfig ();
IMPORT int fmanPlPortHalt ();
IMPORT int fmanPlPramFlush ();
IMPORT int fmanPlProfileRead ();
IMPORT int fmanPlProfileWrite ();
IMPORT int fmanPlRatesCalc ();
IMPORT int fmanPrPortConfig ();
IMPORT int fmanPrPortHalt ();
IMPORT int fmanPrPortTpidGet ();
IMPORT int fmanPrPortTpidSet ();
IMPORT int fmanPrSpCodeFlush ();
IMPORT int fmanPrSpFmcProgLoad ();
IMPORT int fmanPrSpPortConfig ();
IMPORT int fmanQmiPortConfig ();
IMPORT int fmanQmiPortHalt ();
IMPORT int fmanRegister ();
IMPORT int fmod ();
IMPORT int fopen ();
IMPORT int fpCtlRegName;
IMPORT int fpExcMsgTbl;
IMPORT int fpRegName;
IMPORT int fpTypeGet ();
IMPORT int fpathconf ();
IMPORT int fppCtxCreate ();
IMPORT int fppCtxDelete ();
IMPORT int fppCtxShow ();
IMPORT int fppDisable ();
IMPORT int fppEnable ();
IMPORT int fppMRegs ();
IMPORT int fppProbe ();
IMPORT int fppRegListShow ();
IMPORT int fppRestore ();
IMPORT int fppSave ();
IMPORT int fppTaskRegsCFmt;
IMPORT int fppTaskRegsDFmt;
IMPORT int fppTaskRegsGet ();
IMPORT int fppTaskRegsSet ();
IMPORT int fprintf ();
IMPORT int fputc ();
IMPORT int fputs ();
IMPORT int fread ();
IMPORT int free ();
IMPORT int freopen ();
IMPORT int frexp ();
IMPORT int fscanf ();
IMPORT int fseek ();
IMPORT int fsetpos ();
IMPORT int fslGpioRegister ();
IMPORT int fstat ();
IMPORT int fstatfs ();
IMPORT int fstatfs64 ();
IMPORT int fsync ();
IMPORT int ftell ();
IMPORT int ftpCommand ();
IMPORT int ftpCommandEnhanced ();
IMPORT int ftpDataConnGet ();
IMPORT int ftpDataConnInit ();
IMPORT int ftpDataConnInitPassiveMode ();
IMPORT int ftpHookup ();
IMPORT int ftpLibDebugOptionsSet ();
IMPORT int ftpLibInit ();
IMPORT int ftpLogin ();
IMPORT int ftpLs ();
IMPORT int ftpReplyGet ();
IMPORT int ftpReplyGetEnhanced ();
IMPORT int ftpReplyTimeout;
IMPORT int ftpTransientConfigGet ();
IMPORT int ftpTransientConfigSet ();
IMPORT int ftpTransientFatalInstall ();
IMPORT int ftpVerbose;
IMPORT int ftpXfer ();
IMPORT int ftplDebug;
IMPORT int ftplPasvModeDisable;
IMPORT int ftplTransientMaxRetryCount;
IMPORT int ftplTransientRetryInterval;
IMPORT int fwrite ();
IMPORT int genPhyDevRegistration;
IMPORT int genPhyRegister ();
IMPORT int genericLayeredExcHdlrInstall ();
IMPORT int getSymNames ();
IMPORT int getc ();
IMPORT int getchar ();
IMPORT int getcwd ();
IMPORT int getenv ();
IMPORT int gethostname ();
IMPORT int getlhostbyaddr ();
IMPORT int getlhostbyname ();
IMPORT int getpeername ();
IMPORT int getpid ();
IMPORT int gets ();
IMPORT int getsockname ();
IMPORT int getsockopt ();
IMPORT int gettimeofday ();
IMPORT int getw ();
IMPORT int getwd ();
IMPORT int globalNoStackFill;
IMPORT int gmtime ();
IMPORT int gmtime_r ();
IMPORT int gpio0Resources;
IMPORT int gpioInputRead ();
IMPORT int gpioInputSelect ();
IMPORT int gpioIntConnect ();
IMPORT int gpioIntDisconnect ();
IMPORT int gpioOutputSelect ();
IMPORT int gtaDesc;
IMPORT int gtbDesc;
IMPORT int h ();
IMPORT int handleContextGet ();
IMPORT int handleContextGetBase ();
IMPORT int handleContextSet ();
IMPORT int handleInit ();
IMPORT int handleShow ();
IMPORT int handleShowConnect ();
IMPORT int handleTerminate ();
IMPORT int handleTypeGet ();
IMPORT int handleVerify ();
IMPORT int hardWareInterFaceBusInit ();
IMPORT int hardWareInterFaceInit ();
IMPORT int hashFuncIterScale ();
IMPORT int hashFuncModulo ();
IMPORT int hashFuncMultiply ();
IMPORT int hashKeyCmp ();
IMPORT int hashKeyStrCmp ();
IMPORT int hashLibInit ();
IMPORT int hashTblCreate ();
IMPORT int hashTblDelete ();
IMPORT int hashTblDestroy ();
IMPORT int hashTblEach ();
IMPORT int hashTblFind ();
IMPORT int hashTblInit ();
IMPORT int hashTblPut ();
IMPORT int hashTblRemove ();
IMPORT int hashTblTerminate ();
IMPORT int hcfDeviceGet ();
IMPORT int hcfDeviceList;
IMPORT int hcfDeviceNum;
IMPORT int hdprintf ();
IMPORT int help ();
IMPORT int hookAddToHead ();
IMPORT int hookAddToTail ();
IMPORT int hookDelete ();
IMPORT int hookFind ();
IMPORT int hookShow ();
IMPORT int hookTblAddToHead ();
IMPORT int hookTblAddToTail ();
IMPORT int hookTblDelete ();
IMPORT int hookTblFind ();
IMPORT int hookTblInit ();
IMPORT int hookTblShow ();
IMPORT int hookTblUpdatersUnpend ();
IMPORT int hostAdd ();
IMPORT int hostDelete ();
IMPORT int hostGetByAddr ();
IMPORT int hostGetByName ();
IMPORT int hostInitFlag;
IMPORT int hostList;
IMPORT int hostListSem;
IMPORT int hostTblInit ();
IMPORT int hostTblSearchByAddr ();
IMPORT int hostTblSearchByAddr2 ();
IMPORT int hostTblSearchByName ();
IMPORT int hostTblSearchByName2 ();
IMPORT int hostTblSetup ();
IMPORT int hostentAlloc ();
IMPORT int hostentFree ();
IMPORT int hostnameSetup ();
IMPORT int hrfsDiskFormat ();
IMPORT int hrfsFmtRtn;
IMPORT int hwMemAlloc ();
IMPORT int hwMemFree ();
IMPORT int hwMemLibInit ();
IMPORT int hwMemPool;
IMPORT int hwMemPoolCreate ();
IMPORT int i ();
IMPORT int iam ();
IMPORT int inDesc;
IMPORT int inFullVxWorksImage;
IMPORT int incrNumPageTables;
IMPORT int incrNumRegionTables;
IMPORT int index ();
IMPORT int inet_addr ();
IMPORT int inet_aton ();
IMPORT int inet_ntoa ();
IMPORT int initApplLogger ();
IMPORT int initNumPageTables;
IMPORT int initNumRegionTables;
IMPORT int initStaticTlbNum;
IMPORT int instParamModify_desc;
IMPORT int intArchConnect ();
IMPORT int intArchDisconnect ();
IMPORT int intCnt;
IMPORT int intConnect ();
IMPORT int intContext ();
IMPORT int intCount ();
IMPORT int intCpuLock ();
IMPORT int intCpuMicroLock ();
IMPORT int intCpuMicroUnlock ();
IMPORT int intCpuUnlock ();
IMPORT int intCrtEnt ();
IMPORT int intCrtExit ();
IMPORT int intCtlrChainISR ();
IMPORT int intCtlrHwConfGet ();
IMPORT int intCtlrISRAdd ();
IMPORT int intCtlrISRDisable ();
IMPORT int intCtlrISREnable ();
IMPORT int intCtlrISRRemove ();
IMPORT int intCtlrPinFind ();
IMPORT int intCtlrStrayISR ();
IMPORT int intCtlrTableArgGet ();
IMPORT int intCtlrTableCreate ();
IMPORT int intCtlrTableFlagsGet ();
IMPORT int intCtlrTableFlagsSet ();
IMPORT int intCtlrTableIsrGet ();
IMPORT int intCtlrTableUserSet ();
IMPORT int intDisable ();
IMPORT int intDisconnect ();
IMPORT int intEnable ();
IMPORT int intEnt ();
IMPORT int intExit ();
IMPORT int intLevelSet ();
IMPORT int intLock ();
IMPORT int intLockLevelGet ();
IMPORT int intLockLevelSet ();
IMPORT int intRegsLock ();
IMPORT int intRegsUnlock ();
IMPORT int intRestrict ();
IMPORT int intUnlock ();
IMPORT int intVecBaseGet ();
IMPORT int intVecBaseSet ();
IMPORT int intVecGet ();
IMPORT int intVecSet ();
IMPORT int intVecTableWriteProtect ();
IMPORT int ioDefPath;
IMPORT int ioDefPathCat ();
IMPORT int ioDefPathGet ();
IMPORT int ioDefPathSet ();
IMPORT int ioDefPathValidate ();
IMPORT int ioFullFileNameGet ();
IMPORT int ioGlobalStdGet ();
IMPORT int ioGlobalStdSet ();
IMPORT int ioHelp ();
IMPORT int ioMaxLinkLevels;
IMPORT int ioPxCreateOrOpen ();
IMPORT int ioTaskStdGet ();
IMPORT int ioTaskStdSet ();
IMPORT int ioctl ();
IMPORT int iosClose ();
IMPORT int iosCreate ();
IMPORT int iosDelete ();
IMPORT int iosDevAdd ();
IMPORT int iosDevCheck ();
IMPORT int iosDevDelCallback ();
IMPORT int iosDevDelDrv ();
IMPORT int iosDevDelete ();
IMPORT int iosDevFind ();
IMPORT int iosDevMatch ();
IMPORT int iosDevReplace ();
IMPORT int iosDevReplaceExt ();
IMPORT int iosDevResume ();
IMPORT int iosDevShow ();
IMPORT int iosDevSuspend ();
IMPORT int iosDrvInit ();
IMPORT int iosDrvInstall ();
IMPORT int iosDrvIoctl ();
IMPORT int iosDrvIoctlMemValSet ();
IMPORT int iosDrvRemove ();
IMPORT int iosDrvShow ();
IMPORT int iosDvList;
IMPORT int iosFdDevFind ();
IMPORT int iosFdDrvValue ();
IMPORT int iosFdEntryGet ();
IMPORT int iosFdEntryIoctl ();
IMPORT int iosFdEntryPool;
IMPORT int iosFdEntryReturn ();
IMPORT int iosFdEntrySet ();
IMPORT int iosFdFree ();
IMPORT int iosFdMap ();
IMPORT int iosFdMaxFiles ();
IMPORT int iosFdNew ();
IMPORT int iosFdSet ();
IMPORT int iosFdShow ();
IMPORT int iosFdTable;
IMPORT int iosFdTableLock ();
IMPORT int iosFdTableUnlock ();
IMPORT int iosFdValue ();
IMPORT int iosInit ();
IMPORT int iosIoctl ();
IMPORT int iosIoctlInternal ();
IMPORT int iosLibInitialized;
IMPORT int iosLock ();
IMPORT int iosMaxDrivers;
IMPORT int iosMaxFiles;
IMPORT int iosNextDevGet ();
IMPORT int iosOpen ();
IMPORT int iosPathFdEntryIoctl ();
IMPORT int iosPathLibInit ();
IMPORT int iosPse52Mode;
IMPORT int iosPxLibInit ();
IMPORT int iosPxLibInitialized;
IMPORT int iosRead ();
IMPORT int iosRmvFdEntrySet ();
IMPORT int iosRmvLibInit ();
IMPORT int iosRtpDefPathGet ();
IMPORT int iosRtpDefPathSet ();
IMPORT int iosRtpDevCloseOrInvalidate ();
IMPORT int iosRtpFdEntryMap ();
IMPORT int iosRtpFdInvalid ();
IMPORT int iosRtpFdReserve ();
IMPORT int iosRtpFdReserveCntrl ();
IMPORT int iosRtpFdSetup ();
IMPORT int iosRtpFdShow ();
IMPORT int iosRtpFdTableGet ();
IMPORT int iosRtpFdTableSizeGet ();
IMPORT int iosRtpFdTblEntryGet ();
IMPORT int iosRtpFdUnmap2 ();
IMPORT int iosRtpIoTableSizeGet ();
IMPORT int iosRtpIoTableSizeSet ();
IMPORT int iosRtpLibInit ();
IMPORT int iosRtpTableSizeSet ();
IMPORT int iosShowInit ();
IMPORT int iosTaskCwdGet ();
IMPORT int iosTaskCwdSet ();
IMPORT int iosUnlock ();
IMPORT int iosWrite ();
IMPORT int ip_inaddr_any;
IMPORT int ipcomNetTask ();
IMPORT int ipcom_accept ();
IMPORT int ipcom_accept_usr ();
IMPORT int ipcom_asctime_r_vxworks ();
IMPORT int ipcom_atomic_add ();
IMPORT int ipcom_atomic_add_and_return ();
IMPORT int ipcom_atomic_dec ();
IMPORT int ipcom_atomic_get ();
IMPORT int ipcom_atomic_inc ();
IMPORT int ipcom_atomic_ptr_cas ();
IMPORT int ipcom_atomic_set ();
IMPORT int ipcom_atomic_sub ();
IMPORT int ipcom_atomic_sub_and_return ();
IMPORT int ipcom_bind ();
IMPORT int ipcom_bind_usr ();
IMPORT int ipcom_block_create ();
IMPORT int ipcom_block_delete ();
IMPORT int ipcom_block_post ();
IMPORT int ipcom_block_reader_create ();
IMPORT int ipcom_block_wait ();
IMPORT int ipcom_cache_line_size ();
IMPORT int ipcom_calloc ();
IMPORT int ipcom_chdir ();
IMPORT int ipcom_clearenv ();
IMPORT int ipcom_closedir ();
IMPORT int ipcom_closelog ();
IMPORT int ipcom_cmd_if_caps_to_str ();
IMPORT int ipcom_cmd_if_flags_to_str ();
IMPORT int ipcom_cmd_key_to_str ();
IMPORT int ipcom_cmd_sockaddr_to_str ();
IMPORT int ipcom_cmd_str_to_key ();
IMPORT int ipcom_cmd_str_to_key2 ();
IMPORT int ipcom_cmsg_nxthdr ();
IMPORT int ipcom_conf_max_link_hdr_size;
IMPORT int ipcom_conf_pkt_pool_buffer_alignment;
IMPORT int ipcom_conf_pkt_pool_buffer_extra_space;
IMPORT int ipcom_configure ();
IMPORT int ipcom_configure_reserved_cpus ();
IMPORT int ipcom_connect ();
IMPORT int ipcom_connect_usr ();
IMPORT int ipcom_cpu_is_online ();
IMPORT int ipcom_cpu_reserved_alloc ();
IMPORT int ipcom_cpu_reserved_free ();
IMPORT int ipcom_create ();
IMPORT int ipcom_ctime_r_vxworks ();
IMPORT int ipcom_dns;
IMPORT int ipcom_dns_init_ok;
IMPORT int ipcom_drv_eth_create ();
IMPORT int ipcom_drv_eth_exit ();
IMPORT int ipcom_drv_eth_filter_add_mcast_addr ();
IMPORT int ipcom_drv_eth_filter_exit ();
IMPORT int ipcom_drv_eth_filter_init ();
IMPORT int ipcom_drv_eth_filter_remove_mcast_addr ();
IMPORT int ipcom_drv_eth_init ();
IMPORT int ipcom_drv_eth_ip_attach ();
IMPORT int ipcom_errno_get ();
IMPORT int ipcom_errno_set ();
IMPORT int ipcom_fclose ();
IMPORT int ipcom_fdopen ();
IMPORT int ipcom_feof ();
IMPORT int ipcom_ferror ();
IMPORT int ipcom_fflush ();
IMPORT int ipcom_fileclose ();
IMPORT int ipcom_fileno ();
IMPORT int ipcom_fileopen ();
IMPORT int ipcom_fileread ();
IMPORT int ipcom_filewrite ();
IMPORT int ipcom_fopen ();
IMPORT int ipcom_forwarder_ipsec_sa_cache_size;
IMPORT int ipcom_forwarder_ipsec_single_output_queue;
IMPORT int ipcom_fread ();
IMPORT int ipcom_free ();
IMPORT int ipcom_freeaddrinfo ();
IMPORT int ipcom_freeaddrinfo2 ();
IMPORT int ipcom_fseek ();
IMPORT int ipcom_fstat ();
IMPORT int ipcom_ftell ();
IMPORT int ipcom_fwrite ();
IMPORT int ipcom_get_cpuid ();
IMPORT int ipcom_get_ip_if_name ();
IMPORT int ipcom_get_start_shell_ptr ();
IMPORT int ipcom_get_true_cpuid ();
IMPORT int ipcom_getaddrinfo ();
IMPORT int ipcom_getaddrinfo2 ();
IMPORT int ipcom_getcwd ();
IMPORT int ipcom_getenv ();
IMPORT int ipcom_getenv_as_int ();
IMPORT int ipcom_gethostbyaddr ();
IMPORT int ipcom_gethostbyaddr_r ();
IMPORT int ipcom_gethostbyaddrlocal ();
IMPORT int ipcom_gethostbyname ();
IMPORT int ipcom_gethostbyname_r ();
IMPORT int ipcom_gethostbynamelocal ();
IMPORT int ipcom_getlogmask_on ();
IMPORT int ipcom_getnameinfo ();
IMPORT int ipcom_getnameinfo2 ();
IMPORT int ipcom_getopt ();
IMPORT int ipcom_getopt_clear ();
IMPORT int ipcom_getopt_clear_r ();
IMPORT int ipcom_getopt_data;
IMPORT int ipcom_getopt_r ();
IMPORT int ipcom_getpeername ();
IMPORT int ipcom_getpeername_usr ();
IMPORT int ipcom_getpid ();
IMPORT int ipcom_getprotobyname ();
IMPORT int ipcom_getprotobynumber ();
IMPORT int ipcom_getservbyname ();
IMPORT int ipcom_getservbyport ();
IMPORT int ipcom_getsockaddrbyaddr ();
IMPORT int ipcom_getsockaddrbyaddrname ();
IMPORT int ipcom_getsockname ();
IMPORT int ipcom_getsockname_usr ();
IMPORT int ipcom_getsockopt ();
IMPORT int ipcom_getsockopt_usr ();
IMPORT int ipcom_gettimeofday ();
IMPORT int ipcom_h_errno;
IMPORT int ipcom_hash_add ();
IMPORT int ipcom_hash_delete ();
IMPORT int ipcom_hash_for_each ();
IMPORT int ipcom_hash_get ();
IMPORT int ipcom_hash_new ();
IMPORT int ipcom_hash_remove ();
IMPORT int ipcom_hash_update ();
IMPORT int ipcom_heap_sort ();
IMPORT int ipcom_if_attach ();
IMPORT int ipcom_if_attach_and_lock ();
IMPORT int ipcom_if_changelinkaddr ();
IMPORT int ipcom_if_changemtu ();
IMPORT int ipcom_if_changename ();
IMPORT int ipcom_if_detach ();
IMPORT int ipcom_if_free ();
IMPORT int ipcom_if_freenameindex ();
IMPORT int ipcom_if_indextoname ();
IMPORT int ipcom_if_malloc ();
IMPORT int ipcom_if_nameindex ();
IMPORT int ipcom_if_nametoindex ();
IMPORT int ipcom_if_nametonetif ();
IMPORT int ipcom_ima_stack_task ();
IMPORT int ipcom_in_checksum ();
IMPORT int ipcom_in_checksum_finish ();
IMPORT int ipcom_in_checksum_memcpy ();
IMPORT int ipcom_in_checksum_memcpy_asm ();
IMPORT int ipcom_in_checksum_pkt ();
IMPORT int ipcom_in_checksum_update ();
IMPORT int ipcom_in_checksum_update2_asm ();
IMPORT int ipcom_in_checksum_update_pkt ();
IMPORT int ipcom_inet_addr ();
IMPORT int ipcom_inet_aton ();
IMPORT int ipcom_inet_ntoa ();
IMPORT int ipcom_inet_ntop ();
IMPORT int ipcom_inet_pton ();
IMPORT int ipcom_init_config_vars ();
IMPORT int ipcom_initd ();
IMPORT int ipcom_interrupt_disable ();
IMPORT int ipcom_interrupt_enable ();
IMPORT int ipcom_ipc_close ();
IMPORT int ipcom_ipc_free ();
IMPORT int ipcom_ipc_install ();
IMPORT int ipcom_ipc_isopen ();
IMPORT int ipcom_ipc_malloc ();
IMPORT int ipcom_ipc_open ();
IMPORT int ipcom_ipc_receive ();
IMPORT int ipcom_ipc_send ();
IMPORT int ipcom_ipc_size ();
IMPORT int ipcom_ipc_uninstall ();
IMPORT int ipcom_ipd_datamsg ();
IMPORT int ipcom_ipd_exit ();
IMPORT int ipcom_ipd_init ();
IMPORT int ipcom_ipd_input ();
IMPORT int ipcom_ipd_input_process ();
IMPORT int ipcom_ipd_kill ();
IMPORT int ipcom_ipd_products;
IMPORT int ipcom_ipd_read ();
IMPORT int ipcom_ipd_reconfigure ();
IMPORT int ipcom_ipd_send ();
IMPORT int ipcom_ipd_sendmsg ();
IMPORT int ipcom_ipd_start ();
IMPORT int ipcom_ipproto_name ();
IMPORT int ipcom_is_pid_valid ();
IMPORT int ipcom_is_stack_task ();
IMPORT int ipcom_job_queue_init ();
IMPORT int ipcom_job_queue_pipe_schedule ();
IMPORT int ipcom_job_queue_schedule ();
IMPORT int ipcom_job_queue_schedule_singleton ();
IMPORT int ipcom_job_queue_schedule_singleton_delayed ();
IMPORT int ipcom_job_queue_singleton_delete ();
IMPORT int ipcom_job_queue_singleton_new ();
IMPORT int ipcom_key_db_file_root;
IMPORT int ipcom_list_first ();
IMPORT int ipcom_list_init ();
IMPORT int ipcom_list_insert_after ();
IMPORT int ipcom_list_insert_before ();
IMPORT int ipcom_list_insert_first ();
IMPORT int ipcom_list_insert_last ();
IMPORT int ipcom_list_last ();
IMPORT int ipcom_list_next ();
IMPORT int ipcom_list_prev ();
IMPORT int ipcom_list_remove ();
IMPORT int ipcom_list_unlink_head ();
IMPORT int ipcom_listen ();
IMPORT int ipcom_listen_usr ();
IMPORT int ipcom_lseek ();
IMPORT int ipcom_malloc ();
IMPORT int ipcom_mask_to_prefixlen ();
IMPORT int ipcom_mb ();
IMPORT int ipcom_mcmd_ifconfig ();
IMPORT int ipcom_mcmd_route ();
IMPORT int ipcom_memcpy_asm ();
IMPORT int ipcom_memory_pool_new ();
IMPORT int ipcom_microtime ();
IMPORT int ipcom_millisleep ();
IMPORT int ipcom_mipc_addr2offset ();
IMPORT int ipcom_mipc_offset2addr ();
IMPORT int ipcom_mkdir ();
IMPORT int ipcom_mutex_create ();
IMPORT int ipcom_mutex_delete ();
IMPORT int ipcom_mutex_lock ();
IMPORT int ipcom_mutex_unlock ();
IMPORT int ipcom_null_pool_create;
IMPORT int ipcom_num_configured_cpus ();
IMPORT int ipcom_num_online_cpus ();
IMPORT int ipcom_observer_notify ();
IMPORT int ipcom_once ();
IMPORT int ipcom_once_mutex;
IMPORT int ipcom_opendir ();
IMPORT int ipcom_openlog ();
IMPORT int ipcom_parse_argstr ();
IMPORT int ipcom_pipe_attr_init ();
IMPORT int ipcom_pipe_free ();
IMPORT int ipcom_pipe_generic_create ();
IMPORT int ipcom_pipe_job_queue_create ();
IMPORT int ipcom_pipe_lockless_create ();
IMPORT int ipcom_pipe_new ();
IMPORT int ipcom_pipe_register_type ();
IMPORT int ipcom_pipe_tv_zero;
IMPORT int ipcom_pkt_alloc_info ();
IMPORT int ipcom_pkt_copy_info ();
IMPORT int ipcom_pkt_create ();
IMPORT int ipcom_pkt_create_info ();
IMPORT int ipcom_pkt_free ();
IMPORT int ipcom_pkt_get_info ();
IMPORT int ipcom_pkt_get_info_and_size ();
IMPORT int ipcom_pkt_get_info_safe ();
IMPORT int ipcom_pkt_hdrspace ();
IMPORT int ipcom_pkt_input ();
IMPORT int ipcom_pkt_input_queue ();
IMPORT int ipcom_pkt_make_linear ();
IMPORT int ipcom_pkt_malloc ();
IMPORT int ipcom_pkt_output_done ();
IMPORT int ipcom_pkt_set_info ();
IMPORT int ipcom_pkt_sg_get_data ();
IMPORT int ipcom_pkt_trim_head ();
IMPORT int ipcom_pkt_trim_tail ();
IMPORT int ipcom_pktbuf_free ();
IMPORT int ipcom_port;
IMPORT int ipcom_port_pkt_hdr_extra_size;
IMPORT int ipcom_pqueue_delete ();
IMPORT int ipcom_pqueue_for_each ();
IMPORT int ipcom_pqueue_get ();
IMPORT int ipcom_pqueue_get_next ();
IMPORT int ipcom_pqueue_insert ();
IMPORT int ipcom_pqueue_new ();
IMPORT int ipcom_pqueue_nop_store_index ();
IMPORT int ipcom_pqueue_remove ();
IMPORT int ipcom_pqueue_remove_next ();
IMPORT int ipcom_pqueue_size ();
IMPORT int ipcom_preempt_disable ();
IMPORT int ipcom_preempt_enable ();
IMPORT int ipcom_priority_map;
IMPORT int ipcom_proc_acreate ();
IMPORT int ipcom_proc_attr_init ();
IMPORT int ipcom_proc_cpu_affinity_clr ();
IMPORT int ipcom_proc_cpu_affinity_get ();
IMPORT int ipcom_proc_cpu_affinity_set ();
IMPORT int ipcom_proc_cpu_affinity_set_all ();
IMPORT int ipcom_proc_create ();
IMPORT int ipcom_proc_exit ();
IMPORT int ipcom_proc_find ();
IMPORT int ipcom_proc_free ();
IMPORT int ipcom_proc_getprio ();
IMPORT int ipcom_proc_init ();
IMPORT int ipcom_proc_malloc ();
IMPORT int ipcom_proc_self ();
IMPORT int ipcom_proc_setprio ();
IMPORT int ipcom_proc_vr_get ();
IMPORT int ipcom_proc_vr_set ();
IMPORT int ipcom_proc_yield ();
IMPORT int ipcom_rand ();
IMPORT int ipcom_random ();
IMPORT int ipcom_random_init ();
IMPORT int ipcom_random_seed_state ();
IMPORT int ipcom_readdir ();
IMPORT int ipcom_realloc ();
IMPORT int ipcom_recv ();
IMPORT int ipcom_recv_usr ();
IMPORT int ipcom_recvfrom ();
IMPORT int ipcom_recvfrom_usr ();
IMPORT int ipcom_recvmsg ();
IMPORT int ipcom_recvmsg_usr ();
IMPORT int ipcom_register_dev_name_mapping ();
IMPORT int ipcom_rename ();
IMPORT int ipcom_rewind ();
IMPORT int ipcom_rmb ();
IMPORT int ipcom_rmdir ();
IMPORT int ipcom_route_add ();
IMPORT int ipcom_route_first_entry ();
IMPORT int ipcom_route_free_table ();
IMPORT int ipcom_route_init_subtree_mask ();
IMPORT int ipcom_route_key_cmp ();
IMPORT int ipcom_route_lookup ();
IMPORT int ipcom_route_new_table ();
IMPORT int ipcom_route_next_entry ();
IMPORT int ipcom_route_remove ();
IMPORT int ipcom_route_walk_tree ();
IMPORT int ipcom_route_walk_tree_backwards ();
IMPORT int ipcom_rtp_tcb_index_get_fd ();
IMPORT int ipcom_run_cmd ();
IMPORT int ipcom_sem_create ();
IMPORT int ipcom_sem_delete ();
IMPORT int ipcom_sem_flush ();
IMPORT int ipcom_sem_getvalue ();
IMPORT int ipcom_sem_interrupt_flush ();
IMPORT int ipcom_sem_interrupt_post ();
IMPORT int ipcom_sem_post ();
IMPORT int ipcom_sem_wait ();
IMPORT int ipcom_send ();
IMPORT int ipcom_send_usr ();
IMPORT int ipcom_sendmsg ();
IMPORT int ipcom_sendmsg_usr ();
IMPORT int ipcom_sendto ();
IMPORT int ipcom_sendto_usr ();
IMPORT int ipcom_set_add ();
IMPORT int ipcom_set_clone ();
IMPORT int ipcom_set_contains ();
IMPORT int ipcom_set_delete ();
IMPORT int ipcom_set_equal ();
IMPORT int ipcom_set_for_each ();
IMPORT int ipcom_set_intersection ();
IMPORT int ipcom_set_new ();
IMPORT int ipcom_set_remove ();
IMPORT int ipcom_set_remove_all ();
IMPORT int ipcom_set_start_shell_ptr ();
IMPORT int ipcom_set_subtract ();
IMPORT int ipcom_set_union ();
IMPORT int ipcom_setenv ();
IMPORT int ipcom_setenv_as_int ();
IMPORT int ipcom_setlogfile ();
IMPORT int ipcom_setlogmask_on ();
IMPORT int ipcom_setlogudp ();
IMPORT int ipcom_setsockopt ();
IMPORT int ipcom_setsockopt_usr ();
IMPORT int ipcom_settimeofday ();
IMPORT int ipcom_shell_add_cmd ();
IMPORT int ipcom_shell_cmd_head;
IMPORT int ipcom_shell_find_cmd ();
IMPORT int ipcom_shell_find_cmd_hook;
IMPORT int ipcom_shell_fread ();
IMPORT int ipcom_shell_fwrite ();
IMPORT int ipcom_shell_remove_cmd ();
IMPORT int ipcom_shutdown ();
IMPORT int ipcom_shutdown_usr ();
IMPORT int ipcom_slab_add ();
IMPORT int ipcom_slab_alloc_from ();
IMPORT int ipcom_slab_alloc_try ();
IMPORT int ipcom_slab_free ();
IMPORT int ipcom_slab_get_objects_per_slab ();
IMPORT int ipcom_slab_memory_pool_list;
IMPORT int ipcom_slab_new ();
IMPORT int ipcom_slab_once;
IMPORT int ipcom_sleep ();
IMPORT int ipcom_sockaddr_to_prefixlen ();
IMPORT int ipcom_socket ();
IMPORT int ipcom_socket_usr ();
IMPORT int ipcom_socketclose ();
IMPORT int ipcom_socketclose_usr ();
IMPORT int ipcom_socketioctl ();
IMPORT int ipcom_socketioctl_usr ();
IMPORT int ipcom_socketread ();
IMPORT int ipcom_socketread_usr ();
IMPORT int ipcom_socketselect ();
IMPORT int ipcom_socketselect_usr ();
IMPORT int ipcom_socketwrite ();
IMPORT int ipcom_socketwrite_usr ();
IMPORT int ipcom_socketwritev ();
IMPORT int ipcom_socketwritev_usr ();
IMPORT int ipcom_spinlock_create ();
IMPORT int ipcom_spinlock_delete ();
IMPORT int ipcom_spinlock_lock ();
IMPORT int ipcom_spinlock_unlock ();
IMPORT int ipcom_srand ();
IMPORT int ipcom_srandom ();
IMPORT int ipcom_stackd_init ();
IMPORT int ipcom_start ();
IMPORT int ipcom_start_shell ();
IMPORT int ipcom_start_shell_native ();
IMPORT int ipcom_start_shell_ptr;
IMPORT int ipcom_stat ();
IMPORT int ipcom_stderr ();
IMPORT int ipcom_stdin ();
IMPORT int ipcom_stdio_set_echo ();
IMPORT int ipcom_stdout ();
IMPORT int ipcom_str_in_list ();
IMPORT int ipcom_strcasecmp ();
IMPORT int ipcom_strerror ();
IMPORT int ipcom_strerror_r ();
IMPORT int ipcom_strncasecmp ();
IMPORT int ipcom_sys_free;
IMPORT int ipcom_sys_malloc;
IMPORT int ipcom_sysctl ();
IMPORT int ipcom_syslog ();
IMPORT int ipcom_syslog_facility_names;
IMPORT int ipcom_syslog_printf ();
IMPORT int ipcom_syslog_priority_names;
IMPORT int ipcom_syslogd_init ();
IMPORT int ipcom_sysvar_add_observer ();
IMPORT int ipcom_sysvar_ext_setv ();
IMPORT int ipcom_sysvar_for_each ();
IMPORT int ipcom_sysvar_get ();
IMPORT int ipcom_sysvar_get0 ();
IMPORT int ipcom_sysvar_get_as_int ();
IMPORT int ipcom_sysvar_get_as_int0 ();
IMPORT int ipcom_sysvar_get_as_int_vr ();
IMPORT int ipcom_sysvar_get_descriptive_int0 ();
IMPORT int ipcom_sysvar_get_descriptive_int_vr ();
IMPORT int ipcom_sysvar_getvr ();
IMPORT int ipcom_sysvar_remove_observer ();
IMPORT int ipcom_sysvar_set ();
IMPORT int ipcom_sysvar_set0 ();
IMPORT int ipcom_sysvar_set_tree ();
IMPORT int ipcom_sysvar_setv ();
IMPORT int ipcom_sysvar_setvr ();
IMPORT int ipcom_sysvar_unset ();
IMPORT int ipcom_tmo2_cancel ();
IMPORT int ipcom_tmo2_delete ();
IMPORT int ipcom_tmo2_get ();
IMPORT int ipcom_tmo2_new ();
IMPORT int ipcom_tmo2_request ();
IMPORT int ipcom_tmo2_request_timeval ();
IMPORT int ipcom_tmo2_reset ();
IMPORT int ipcom_tmo2_select ();
IMPORT int ipcom_tmo_cancel ();
IMPORT int ipcom_tmo_get ();
IMPORT int ipcom_tmo_request ();
IMPORT int ipcom_tmo_reset ();
IMPORT int ipcom_unlink ();
IMPORT int ipcom_unsetenv ();
IMPORT int ipcom_usr_create ();
IMPORT int ipcom_version;
IMPORT int ipcom_vsyslog ();
IMPORT int ipcom_vxshell_add_cmd ();
IMPORT int ipcom_vxshell_add_cmd_hook;
IMPORT int ipcom_vxworks_boot_devname ();
IMPORT int ipcom_vxworks_loginStringGet ();
IMPORT int ipcom_vxworks_pkt_pool_hdr_ctor ();
IMPORT int ipcom_vxworks_proc_free ();
IMPORT int ipcom_vxworks_rtp_del_hook_add ();
IMPORT int ipcom_vxworks_task_del_hook ();
IMPORT int ipcom_windnet_socketwritev ();
IMPORT int ipcom_windnet_socklib_init ();
IMPORT int ipcom_windnet_socklib_sock_size ();
IMPORT int ipcom_windnet_socklib_socket_free ();
IMPORT int ipcom_windnet_socklib_socket_init ();
IMPORT int ipcom_wmb ();
IMPORT int ipcom_wrapper_funcs;
IMPORT int ipcom_wv_event_2 ();
IMPORT int ipcom_wv_marker_1 ();
IMPORT int ipcom_wv_marker_2 ();
IMPORT int ipiDesc;
IMPORT int ipmcrypto_MD5_Final ();
IMPORT int ipmcrypto_MD5_Init ();
IMPORT int ipmcrypto_MD5_Update ();
IMPORT int ipmcrypto_SHA1_Final ();
IMPORT int ipmcrypto_SHA1_Init ();
IMPORT int ipmcrypto_SHA1_Update ();
IMPORT int ipmcrypto_md5_calculate ();
IMPORT int ipnet;
IMPORT int ipnet_arp_ioctl ();
IMPORT int ipnet_arp_request ();
IMPORT int ipnet_bool_map;
IMPORT int ipnet_cmd_ifconfig_hook;
IMPORT int ipnet_cmd_qc_hook;
IMPORT int ipnet_cmd_route_hook;
IMPORT int ipnet_conf_boot_cmd;
IMPORT int ipnet_conf_cache_bufsiz;
IMPORT int ipnet_conf_inq_max;
IMPORT int ipnet_conf_interfaces;
IMPORT int ipnet_conf_ip4_min_mtu;
IMPORT int ipnet_conf_ip6_min_mtu;
IMPORT int ipnet_conf_link_layer;
IMPORT int ipnet_conf_max_dgram_frag_list_len;
IMPORT int ipnet_conf_max_reassembly_list_len;
IMPORT int ipnet_conf_max_sockets;
IMPORT int ipnet_conf_memory_limit;
IMPORT int ipnet_conf_reassembly_timeout;
IMPORT int ipnet_conf_update ();
IMPORT int ipnet_configd_start ();
IMPORT int ipnet_configure ();
IMPORT int ipnet_create ();
IMPORT int ipnet_create_reassembled_packet ();
IMPORT int ipnet_ctrl ();
IMPORT int ipnet_ctrl_sig ();
IMPORT int ipnet_do_close ();
IMPORT int ipnet_dst_cache;
IMPORT int ipnet_dst_cache_blackhole ();
IMPORT int ipnet_dst_cache_blackhole_flow_spec ();
IMPORT int ipnet_dst_cache_flush ();
IMPORT int ipnet_dst_cache_flush_where_neigh_is ();
IMPORT int ipnet_dst_cache_foreach ();
IMPORT int ipnet_dst_cache_init ();
IMPORT int ipnet_dst_cache_new ();
IMPORT int ipnet_dst_cache_select_best_rt ();
IMPORT int ipnet_dst_cache_set_path_mtu ();
IMPORT int ipnet_dst_cache_set_rx_handler ();
IMPORT int ipnet_dst_cache_set_tx_handler ();
IMPORT int ipnet_eth_add_hdr ();
IMPORT int ipnet_eth_addr_broadcast;
IMPORT int ipnet_eth_addr_broadcast_storage;
IMPORT int ipnet_eth_if_init ();
IMPORT int ipnet_eth_update_mib2_lastchange ();
IMPORT int ipnet_flags;
IMPORT int ipnet_flow_spec_domain ();
IMPORT int ipnet_flow_spec_from_info ();
IMPORT int ipnet_flow_spec_from_sock ();
IMPORT int ipnet_frag_release_peer_info ();
IMPORT int ipnet_frag_set_peer_info ();
IMPORT int ipnet_frag_to_peer_info ();
IMPORT int ipnet_fragment_packet ();
IMPORT int ipnet_get_sock ();
IMPORT int ipnet_has_priv ();
IMPORT int ipnet_icmp4_param_init ();
IMPORT int ipnet_icmp4_send ();
IMPORT int ipnet_icmp4_send_host_unreachable ();
IMPORT int ipnet_icmp4_send_port_unreachable ();
IMPORT int ipnet_if_can_detach ();
IMPORT int ipnet_if_clean ();
IMPORT int ipnet_if_clean_ppp_peer ();
IMPORT int ipnet_if_clean_snd_queue ();
IMPORT int ipnet_if_detach ();
IMPORT int ipnet_if_drv_ioctl ();
IMPORT int ipnet_if_drv_output ();
IMPORT int ipnet_if_free ();
IMPORT int ipnet_if_get_index_array ();
IMPORT int ipnet_if_indextonetif ();
IMPORT int ipnet_if_init ();
IMPORT int ipnet_if_init_ppp_peer ();
IMPORT int ipnet_if_link_ioctl ();
IMPORT int ipnet_if_nametonetif ();
IMPORT int ipnet_if_output ();
IMPORT int ipnet_if_set_ipv4_ppp_peer ();
IMPORT int ipnet_if_to_sockaddr_dl ();
IMPORT int ipnet_if_update_mib2_ifLastChange ();
IMPORT int ipnet_igmpv3_report_change ();
IMPORT int ipnet_increase_hdr_space ();
IMPORT int ipnet_ioctl_move_if_rt_to_vr ();
IMPORT int ipnet_ip4_acd_conflict ();
IMPORT int ipnet_ip4_add_addr ();
IMPORT int ipnet_ip4_add_addr2 ();
IMPORT int ipnet_ip4_add_route_table ();
IMPORT int ipnet_ip4_addr_to_sockaddr ();
IMPORT int ipnet_ip4_change_addr_mask ();
IMPORT int ipnet_ip4_configure_route_table ();
IMPORT int ipnet_ip4_dst_cache_get ();
IMPORT int ipnet_ip4_dst_cache_rx_ctor ();
IMPORT int ipnet_ip4_dst_unreachable ();
IMPORT int ipnet_ip4_dst_unreachable_filter ();
IMPORT int ipnet_ip4_flow_spec_from_info ();
IMPORT int ipnet_ip4_flow_spec_from_pkt ();
IMPORT int ipnet_ip4_fragment_timeout_peer ();
IMPORT int ipnet_ip4_get_addr_entry ();
IMPORT int ipnet_ip4_get_addr_type ();
IMPORT int ipnet_ip4_get_addr_type2 ();
IMPORT int ipnet_ip4_get_mss ();
IMPORT int ipnet_ip4_get_src_addr ();
IMPORT int ipnet_ip4_init ();
IMPORT int ipnet_ip4_input ();
IMPORT int ipnet_ip4_insert_addr_cache ();
IMPORT int ipnet_ip4_is_part_of_same_pkt ();
IMPORT int ipnet_ip4_kioevent ();
IMPORT int ipnet_ip4_netif_get_subbrd ();
IMPORT int ipnet_ip4_pkt_with_iphdr_tx ();
IMPORT int ipnet_ip4_reg_transport_layer ();
IMPORT int ipnet_ip4_remove_addr ();
IMPORT int ipnet_ip4_remove_addr_cache ();
IMPORT int ipnet_ip4_rfc1256_mode_update ();
IMPORT int ipnet_ip4_rx ();
IMPORT int ipnet_ip4_sendto ();
IMPORT int ipnet_ip4_transport_rx ();
IMPORT int ipnet_ip4_tx ();
IMPORT int ipnet_is_loopback ();
IMPORT int ipnet_is_sock_exceptional ();
IMPORT int ipnet_is_sock_readable ();
IMPORT int ipnet_is_sock_writable ();
IMPORT int ipnet_is_stack_task ();
IMPORT int ipnet_kioevent ();
IMPORT int ipnet_loop_pkt_tx ();
IMPORT int ipnet_loopback_attach ();
IMPORT int ipnet_loopback_get_netif ();
IMPORT int ipnet_loopback_if_init ();
IMPORT int ipnet_mcast_build_if_filter ();
IMPORT int ipnet_mcast_build_source_change_report ();
IMPORT int ipnet_mcast_clear ();
IMPORT int ipnet_mcast_free ();
IMPORT int ipnet_mcast_init ();
IMPORT int ipnet_mcast_report_finish ();
IMPORT int ipnet_mcast_schedule_membership_report ();
IMPORT int ipnet_mcast_time_to_msec ();
IMPORT int ipnet_msec_now ();
IMPORT int ipnet_neigh_dad ();
IMPORT int ipnet_neigh_flush ();
IMPORT int ipnet_neigh_flush_all ();
IMPORT int ipnet_neigh_foreach ();
IMPORT int ipnet_neigh_get ();
IMPORT int ipnet_neigh_init ();
IMPORT int ipnet_neigh_init_addr_observer ();
IMPORT int ipnet_neigh_invalidate ();
IMPORT int ipnet_neigh_is_probing ();
IMPORT int ipnet_neigh_netif_discard ();
IMPORT int ipnet_neigh_probe ();
IMPORT int ipnet_neigh_release ();
IMPORT int ipnet_neigh_set_blackhole ();
IMPORT int ipnet_neigh_set_external ();
IMPORT int ipnet_neigh_set_gateway ();
IMPORT int ipnet_neigh_set_lifetime ();
IMPORT int ipnet_neigh_set_lladdr ();
IMPORT int ipnet_neigh_set_local ();
IMPORT int ipnet_neigh_set_perm ();
IMPORT int ipnet_neigh_set_publ ();
IMPORT int ipnet_neigh_set_report ();
IMPORT int ipnet_neigh_set_silent ();
IMPORT int ipnet_neigh_set_state ();
IMPORT int ipnet_netif_change_flags ();
IMPORT int ipnet_netif_is_ifproxy ();
IMPORT int ipnet_netif_neigh_create ();
IMPORT int ipnet_next_ephemeral_port ();
IMPORT int ipnet_noeth_if_init ();
IMPORT int ipnet_packet_getsockopt ();
IMPORT int ipnet_packet_input ();
IMPORT int ipnet_packet_setsockopt ();
IMPORT int ipnet_pcap_ioctl ();
IMPORT int ipnet_peer_info_addref ();
IMPORT int ipnet_peer_info_get ();
IMPORT int ipnet_peer_info_init ();
IMPORT int ipnet_peer_info_release ();
IMPORT int ipnet_peer_info_timeout_cancel ();
IMPORT int ipnet_peer_info_timeout_schedule ();
IMPORT int ipnet_pipe_create ();
IMPORT int ipnet_pkt_clone ();
IMPORT int ipnet_pkt_get_maxlen ();
IMPORT int ipnet_pkt_input ();
IMPORT int ipnet_pkt_pool_drain ();
IMPORT int ipnet_pkt_pool_hdr_count_hint ();
IMPORT int ipnet_pkt_pool_hdr_ctor_func;
IMPORT int ipnet_pkt_pool_init ();
IMPORT int ipnet_pkt_pool_slab_desc;
IMPORT int ipnet_pkt_queue_delete ();
IMPORT int ipnet_pkt_queue_fifo_template ();
IMPORT int ipnet_pkt_queue_init ();
IMPORT int ipnet_pkt_queue_new ();
IMPORT int ipnet_ppp_if_init ();
IMPORT int ipnet_queue_received_packet ();
IMPORT int ipnet_raw_input ();
IMPORT int ipnet_reassembly ();
IMPORT int ipnet_register_if_attach_handler ();
IMPORT int ipnet_release_sock ();
IMPORT int ipnet_resume_stack ();
IMPORT int ipnet_route_add ();
IMPORT int ipnet_route_add_table ();
IMPORT int ipnet_route_apply_mask ();
IMPORT int ipnet_route_cache_invalidate ();
IMPORT int ipnet_route_create_mask ();
IMPORT int ipnet_route_delete ();
IMPORT int ipnet_route_delete2 ();
IMPORT int ipnet_route_delete_name ();
IMPORT int ipnet_route_delete_table ();
IMPORT int ipnet_route_delete_vr ();
IMPORT int ipnet_route_first_entry ();
IMPORT int ipnet_route_for_each_data_init ();
IMPORT int ipnet_route_get_name ();
IMPORT int ipnet_route_get_rtab ();
IMPORT int ipnet_route_get_table_ids ();
IMPORT int ipnet_route_has_changed ();
IMPORT int ipnet_route_has_expired ();
IMPORT int ipnet_route_ip4_is_mcast_or_bcast ();
IMPORT int ipnet_route_is_virtual_router_valid ();
IMPORT int ipnet_route_key_cmp ();
IMPORT int ipnet_route_key_to_sockaddr ();
IMPORT int ipnet_route_lock ();
IMPORT int ipnet_route_lookup ();
IMPORT int ipnet_route_lookup_ecmp ();
IMPORT int ipnet_route_next_entry ();
IMPORT int ipnet_route_notify_func ();
IMPORT int ipnet_route_policy_get_rule_chain ();
IMPORT int ipnet_route_policy_ioctl ();
IMPORT int ipnet_route_raw_lookup ();
IMPORT int ipnet_route_raw_lookup2 ();
IMPORT int ipnet_route_remove_all ();
IMPORT int ipnet_route_remove_all_cb_for_each_entry ();
IMPORT int ipnet_route_remove_dynamic_rt_with_gw ();
IMPORT int ipnet_route_rotate_gateways ();
IMPORT int ipnet_route_set_lifetime ();
IMPORT int ipnet_route_set_name ();
IMPORT int ipnet_route_set_rtab ();
IMPORT int ipnet_route_sockaddr_to_key ();
IMPORT int ipnet_route_unlock ();
IMPORT int ipnet_route_vr_and_table_from_name ();
IMPORT int ipnet_route_walk_tree ();
IMPORT int ipnet_routesock_addr_add ();
IMPORT int ipnet_routesock_addr_conflict ();
IMPORT int ipnet_routesock_addr_delete ();
IMPORT int ipnet_routesock_if_announce ();
IMPORT int ipnet_routesock_if_change ();
IMPORT int ipnet_routesock_neigh_resolve ();
IMPORT int ipnet_routesock_rt_add ();
IMPORT int ipnet_routesock_rt_change ();
IMPORT int ipnet_routesock_rt_delete ();
IMPORT int ipnet_routesock_rt_miss ();
IMPORT int ipnet_routesock_rt_redirect ();
IMPORT int ipnet_sec_now ();
IMPORT int ipnet_sig_free ();
IMPORT int ipnet_sock_alloc_private_data ();
IMPORT int ipnet_sock_bind ();
IMPORT int ipnet_sock_bind_addr_removed ();
IMPORT int ipnet_sock_bind_to_port ();
IMPORT int ipnet_sock_change_addr_entry ();
IMPORT int ipnet_sock_data_avail ();
IMPORT int ipnet_sock_drop_all_multicast_memberships ();
IMPORT int ipnet_sock_free ();
IMPORT int ipnet_sock_get ();
IMPORT int ipnet_sock_get_bound_port_size ();
IMPORT int ipnet_sock_get_ops_handle ();
IMPORT int ipnet_sock_get_route ();
IMPORT int ipnet_sock_has_waiting_writers ();
IMPORT int ipnet_sock_init ();
IMPORT int ipnet_sock_ip4_get_mcast_data ();
IMPORT int ipnet_sock_ip4_get_ops ();
IMPORT int ipnet_sock_ip4_lookup ();
IMPORT int ipnet_sock_ip4_mcast_delete_data ();
IMPORT int ipnet_sock_ip4_register ();
IMPORT int ipnet_sock_join_router_alert_chain ();
IMPORT int ipnet_sock_leave_router_alert_chain ();
IMPORT int ipnet_sock_next ();
IMPORT int ipnet_sock_ops;
IMPORT int ipnet_sock_packet_lookup ();
IMPORT int ipnet_sock_packet_register ();
IMPORT int ipnet_sock_pkt_drain ();
IMPORT int ipnet_sock_register_ops ();
IMPORT int ipnet_sock_route_register ();
IMPORT int ipnet_sock_router_alert_pkt ();
IMPORT int ipnet_sock_set_reachable ();
IMPORT int ipnet_sock_set_unreachable ();
IMPORT int ipnet_sock_tcp_register ();
IMPORT int ipnet_sock_udp_register ();
IMPORT int ipnet_sock_unbind ();
IMPORT int ipnet_sock_unconnect ();
IMPORT int ipnet_sock_update_reuse ();
IMPORT int ipnet_sockopt_ip4_addr_to_netif ();
IMPORT int ipnet_softirq_init ();
IMPORT int ipnet_softirq_schedule ();
IMPORT int ipnet_start ();
IMPORT int ipnet_stats_tcp_established ();
IMPORT int ipnet_suspend_stack ();
IMPORT int ipnet_sys_accept ();
IMPORT int ipnet_sys_bind ();
IMPORT int ipnet_sys_checktcp ();
IMPORT int ipnet_sys_connect ();
IMPORT int ipnet_sys_get_ancillary_data ();
IMPORT int ipnet_sys_getname ();
IMPORT int ipnet_sys_getsockopt ();
IMPORT int ipnet_sys_if_attach ();
IMPORT int ipnet_sys_if_changelinkaddr ();
IMPORT int ipnet_sys_if_changemtu ();
IMPORT int ipnet_sys_if_changename ();
IMPORT int ipnet_sys_if_detach ();
IMPORT int ipnet_sys_if_free ();
IMPORT int ipnet_sys_if_indexname ();
IMPORT int ipnet_sys_if_malloc ();
IMPORT int ipnet_sys_if_nametonetif ();
IMPORT int ipnet_sys_listen ();
IMPORT int ipnet_sys_poll ();
IMPORT int ipnet_sys_poll_async ();
IMPORT int ipnet_sys_savesignal ();
IMPORT int ipnet_sys_sendmsg ();
IMPORT int ipnet_sys_setsockopt ();
IMPORT int ipnet_sys_shutdown ();
IMPORT int ipnet_sys_socket ();
IMPORT int ipnet_sys_socketclose ();
IMPORT int ipnet_sys_socketioctl ();
IMPORT int ipnet_sys_sysctl ();
IMPORT int ipnet_sysctl_create_ifinfo ();
IMPORT int ipnet_sysctl_if_add_addrs_one ();
IMPORT int ipnet_sysvar_get_as_int_vr ();
IMPORT int ipnet_sysvar_netif_get_as_int ();
IMPORT int ipnet_sysvar_netif_get_as_int_ex ();
IMPORT int ipnet_timeout_cancel ();
IMPORT int ipnet_timeout_msec_until ();
IMPORT int ipnet_timeout_schedule ();
IMPORT int ipnet_timeval_to_msec ();
IMPORT int ipnet_try_free_mem ();
IMPORT int ipnet_usr_sock_get_ops ();
IMPORT int ipnet_usr_sock_get_pipe ();
IMPORT int ipnet_usr_sock_get_response_pipe ();
IMPORT int ipnet_usr_sock_get_route_cache_id ();
IMPORT int ipnet_usr_sock_init ();
IMPORT int ipnet_usr_sock_init_sig ();
IMPORT int ipnet_usr_sock_recvmsg ();
IMPORT int ipnet_usr_sock_wait_until_writable ();
IMPORT int ipnet_version ();
IMPORT int ipnet_wrapper_funcs;
IMPORT int ipppp_example_action_cb ();
IMPORT int iptcp;
IMPORT int iptcp_accept ();
IMPORT int iptcp_ao_rtnetlink_init ();
IMPORT int iptcp_change_state_global ();
IMPORT int iptcp_close ();
IMPORT int iptcp_conf_default_backlog;
IMPORT int iptcp_configure ();
IMPORT int iptcp_connect ();
IMPORT int iptcp_create ();
IMPORT int iptcp_drop_connection ();
IMPORT int iptcp_get_rto ();
IMPORT int iptcp_get_state ();
IMPORT int iptcp_getsockopt ();
IMPORT int iptcp_icmp4_report ();
IMPORT int iptcp_ioctl ();
IMPORT int iptcp_listen ();
IMPORT int iptcp_pkt_drain ();
IMPORT int iptcp_send ();
IMPORT int iptcp_send_reset ();
IMPORT int iptcp_setsockopt ();
IMPORT int iptcp_shutdown ();
IMPORT int iptcp_sock_free ();
IMPORT int iptcp_sock_init ();
IMPORT int iptcp_start ();
IMPORT int iptcp_tx_list;
IMPORT int iptcp_usr_recv ();
IMPORT int iptcp_version ();
IMPORT int iptcp_window_update ();
IMPORT int isalnum ();
IMPORT int isalpha ();
IMPORT int isatty ();
IMPORT int iscntrl ();
IMPORT int isdigit ();
IMPORT int isgraph ();
IMPORT int islower ();
IMPORT int isprint ();
IMPORT int ispunct ();
IMPORT int isrArchDecCreate ();
IMPORT int isrArchDecDispatcher ();
IMPORT int isrDeferJobAdd ();
IMPORT int isrDeferLibInit ();
IMPORT int isrDeferQueueGet ();
IMPORT int isrIdCurrent;
IMPORT int isrJobPool;
IMPORT int isrRerouteNotify ();
IMPORT int isspace ();
IMPORT int isupper ();
IMPORT int isxdigit ();
IMPORT int jobAdd ();
IMPORT int jobQueueCreate ();
IMPORT int jobQueueInit ();
IMPORT int jobQueueLibInit ();
IMPORT int jobQueuePost ();
IMPORT int jobQueuePriorityMask ();
IMPORT int jobQueueProcess ();
IMPORT int jobQueueProcessFunc;
IMPORT int jobQueueStdJobsAlloc ();
IMPORT int jobQueueStdPoolInit ();
IMPORT int jobQueueStdPost ();
IMPORT int jobQueueTask ();
IMPORT int jobTaskLibInit ();
IMPORT int jobTaskWorkAdd ();
IMPORT int kernelBaseInit ();
IMPORT int kernelCpuEnable ();
IMPORT int kernelId;
IMPORT int kernelIdGet ();
IMPORT int kernelInit ();
IMPORT int kernelIsCpuIdle ();
IMPORT int kernelIsIdle;
IMPORT int kernelIsSystemIdle ();
IMPORT int kernelModuleListId;
IMPORT int kernelObjClassId;
IMPORT int kernelRoundRobinInstall ();
IMPORT int kernelState;
IMPORT int kernelTimeSlice ();
IMPORT int kernelTimeSliceGet ();
IMPORT int kernelVersion ();
IMPORT int kill ();
IMPORT int l ();
IMPORT int labs ();
IMPORT int law0Resources;
IMPORT int lawRegister ();
IMPORT int ld ();
IMPORT int ldCommonMatchAll;
IMPORT int ldexp ();
IMPORT int ldiv ();
IMPORT int ldiv_r ();
IMPORT int ledBeep ();
IMPORT int ledBwdFind ();
IMPORT int ledCDelete ();
IMPORT int ledChange ();
IMPORT int ledClose ();
IMPORT int ledComplete ();
IMPORT int ledCompletionSet ();
IMPORT int ledControl ();
IMPORT int ledDfltComplete ();
IMPORT int ledFwdFind ();
IMPORT int ledHistCurrentNumGet ();
IMPORT int ledHistFind ();
IMPORT int ledHistNextGet ();
IMPORT int ledHistNumGet ();
IMPORT int ledHistPrevGet ();
IMPORT int ledHistResize ();
IMPORT int ledHistoryAdd ();
IMPORT int ledHistoryClone ();
IMPORT int ledHistoryCreate ();
IMPORT int ledHistoryFree ();
IMPORT int ledHistoryLineAllocate ();
IMPORT int ledHistorySet ();
IMPORT int ledInactivityDelaySet ();
IMPORT int ledLibInit ();
IMPORT int ledModeDefaultNameGet ();
IMPORT int ledModeNameGet ();
IMPORT int ledModeRegister ();
IMPORT int ledModeSet ();
IMPORT int ledNWrite ();
IMPORT int ledOpen ();
IMPORT int ledPreempt ();
IMPORT int ledPreemptSet ();
IMPORT int ledRead ();
IMPORT int ledRead2 ();
IMPORT int ledRedraw ();
IMPORT int ledReplace ();
IMPORT int ledSearch ();
IMPORT int link ();
IMPORT int linkBufPoolInit ();
IMPORT int linkedCtorsInitialized;
IMPORT int listen ();
IMPORT int lkAddr ();
IMPORT int lkAddrInternal ();
IMPORT int lkup ();
IMPORT int ll ();
IMPORT int llr ();
IMPORT int loadCommonSymbolProcess ();
IMPORT int loadDefinedSymbolProcess ();
IMPORT int loadLibInit ();
IMPORT int loadModule ();
IMPORT int loadModuleAt ();
IMPORT int loadModuleInfoFromFd ();
IMPORT int loadModuleInfoFromFilenameOpen ();
IMPORT int loadModuleInfoInit ();
IMPORT int loadModuleInfoRelease ();
IMPORT int loadModuleSeek ();
IMPORT int loadModuleStringRead ();
IMPORT int loadModuleValueRead ();
IMPORT int loadSectionsInstall ();
IMPORT int loadSegInfoDescAllocate ();
IMPORT int loadSegInfoDescFree ();
IMPORT int loadSegmentsAllocate ();
IMPORT int loadShareTgtMemAlign ();
IMPORT int loadShareTgtMemFree ();
IMPORT int loadShareTgtMemSet ();
IMPORT int loadSysSymTblLoad ();
IMPORT int loadUndefinedSymbolProcess ();
IMPORT int loaderCacheBufferSize;
IMPORT int localToGlobalOffset;
IMPORT int localeFiles;
IMPORT int localeconv ();
IMPORT int localtime ();
IMPORT int localtime_r ();
IMPORT int log ();
IMPORT int log10 ();
IMPORT int logDecorationChange ();
IMPORT int logFd;
IMPORT int logFdAdd ();
IMPORT int logFdDelete ();
IMPORT int logFdFromRlogin;
IMPORT int logFdSem;
IMPORT int logFdSet ();
IMPORT int logInit ();
IMPORT int logLevelChange ();
IMPORT int logMsg ();
IMPORT int logShow ();
IMPORT int logTask ();
IMPORT int logTaskId;
IMPORT int logTaskOptions;
IMPORT int logTaskPriority;
IMPORT int logTaskStackSize;
IMPORT int logb ();
IMPORT int loggerInfoShow ();
IMPORT int loginHostname;
IMPORT int loginPassword;
IMPORT int loginUsername;
IMPORT int logout ();
IMPORT int longjmp ();
IMPORT int ls ();
IMPORT int lseek ();
IMPORT int lsr ();
IMPORT int lstAdd ();
IMPORT int lstConcat ();
IMPORT int lstCount ();
IMPORT int lstDelete ();
IMPORT int lstExtract ();
IMPORT int lstFind ();
IMPORT int lstFirst ();
IMPORT int lstFree ();
IMPORT int lstFree2 ();
IMPORT int lstGet ();
IMPORT int lstInit ();
IMPORT int lstInsert ();
IMPORT int lstLast ();
IMPORT int lstLibInit ();
IMPORT int lstNStep ();
IMPORT int lstNext ();
IMPORT int lstNth ();
IMPORT int lstPrevious ();
IMPORT int m ();
IMPORT int m2If64BitCounters;
IMPORT int m85xxTimerDrvRegister ();
IMPORT int m85xxTimerResources;
IMPORT int mRegs ();
IMPORT int m_prepend ();
IMPORT int malloc ();
IMPORT int mathFiles;
IMPORT int max_hdr;
IMPORT int max_linkhdr;
IMPORT int max_protohdr;
IMPORT int mblen ();
IMPORT int mbstowcs ();
IMPORT int mbtowc ();
IMPORT int mchErrorL1DParityCnt;
IMPORT int mchErrorL1IParityCnt;
IMPORT int mchErrorTotalCnt;
IMPORT int md5_block_data_order ();
IMPORT int md5_block_host_order ();
IMPORT int memAddToPool ();
IMPORT int memBlockSizeGet ();
IMPORT int memDefaultAlignment;
IMPORT int memFindMax ();
IMPORT int memInfoGet ();
IMPORT int memInfoInit ();
IMPORT int memInit ();
IMPORT int memMsgBlockError;
IMPORT int memMsgBlockTooBig;
IMPORT int memOptionsGet ();
IMPORT int memOptionsSet ();
IMPORT int memPartAddToPool ();
IMPORT int memPartAlignedAlloc ();
IMPORT int memPartAlloc ();
IMPORT int memPartAllocErrorRtn;
IMPORT int memPartAllocN ();
IMPORT int memPartBlockErrorRtn;
IMPORT int memPartBlockIsValid ();
IMPORT int memPartBlockValidate ();
IMPORT int memPartClassId;
IMPORT int memPartCreate ();
IMPORT int memPartCreateHookSet ();
IMPORT int memPartDelete ();
IMPORT int memPartDeleteFree ();
IMPORT int memPartEdrErrorInject ();
IMPORT int memPartFindMax ();
IMPORT int memPartFree ();
IMPORT int memPartFreeListWalk ();
IMPORT int memPartFreeN ();
IMPORT int memPartHooksInstall ();
IMPORT int memPartInfoGet ();
IMPORT int memPartInit ();
IMPORT int memPartInsertFree ();
IMPORT int memPartLibInit ();
IMPORT int memPartOptionsDefault;
IMPORT int memPartOptionsGet ();
IMPORT int memPartOptionsSet ();
IMPORT int memPartRealloc ();
IMPORT int memPartReallocInternal ();
IMPORT int memPartShow ();
IMPORT int memPartVerifiedLock ();
IMPORT int memPoolEndAdrs;
IMPORT int memShow ();
IMPORT int memShowInit ();
IMPORT int memSysPartId;
IMPORT int memSysPartition;
IMPORT int memalign ();
IMPORT int memchr ();
IMPORT int memcmp ();
IMPORT int memcpy ();
IMPORT int memmove ();
IMPORT int memoryDump ();
IMPORT int memoryModify ();
IMPORT int memset ();
IMPORT int mib2ErrorAdd ();
IMPORT int mib2Init ();
IMPORT int miiBusCreate ();
IMPORT int miiBusDelete ();
IMPORT int miiBusGet ();
IMPORT int miiBusIdleErrorCheck ();
IMPORT int miiBusListAdd ();
IMPORT int miiBusListDel ();
IMPORT int miiBusMediaAdd ();
IMPORT int miiBusMediaDefaultSet ();
IMPORT int miiBusMediaDel ();
IMPORT int miiBusMediaListGet ();
IMPORT int miiBusMediaUpdate ();
IMPORT int miiBusModeGet ();
IMPORT int miiBusModeSet ();
IMPORT int miiBusRead ();
IMPORT int miiBusRegister ();
IMPORT int miiBusWrite ();
IMPORT int miiMediaUpdate_desc;
IMPORT int miiModeGet_desc;
IMPORT int miiModeSet_desc;
IMPORT int miiRead_desc;
IMPORT int miiWrite_desc;
IMPORT int mkdir ();
IMPORT int mktime ();
IMPORT int mmuArchCopySize;
IMPORT int mmuArchLibAttrs;
IMPORT int mmuArchLibFuncs;
IMPORT int mmuCacheStateTransTbl;
IMPORT int mmuE500FlushPte ();
IMPORT int mmuE500LibInit ();
IMPORT int mmuE500MaxPhysBitsGet ();
IMPORT int mmuE500PteUpdate ();
IMPORT int mmuE500StaticTlbArray;
IMPORT int mmuE500TlbDynamicInvalidate ();
IMPORT int mmuE500TlbStaticInit ();
IMPORT int mmuE500TlbStaticInvalidate ();
IMPORT int mmuGlobalTransTbl;
IMPORT int mmuInvalidState;
IMPORT int mmuLibFuncs;
IMPORT int mmuMaskTransTbl;
IMPORT int mmuPpcADisable ();
IMPORT int mmuPpcAEnable ();
IMPORT int mmuPpcBcopy ();
IMPORT int mmuPpcDEnabled;
IMPORT int mmuPpcDataTlbMissHandler ();
IMPORT int mmuPpcE500mcTlbie ();
IMPORT int mmuPpcHasBeenEnabled;
IMPORT int mmuPpcIEnabled;
IMPORT int mmuPpcInstTlbMissHandler ();
IMPORT int mmuPpcPidGet ();
IMPORT int mmuPpcPidSet ();
IMPORT int mmuPpcSearchPidGet ();
IMPORT int mmuPpcSearchPidSet ();
IMPORT int mmuPpcSelected;
IMPORT int mmuPpcTlbMissHandlerEnd ();
IMPORT int mmuPpcTlbReadEntryWord0 ();
IMPORT int mmuPpcTlbReadEntryWord1 ();
IMPORT int mmuPpcTlbReadEntryWord2 ();
IMPORT int mmuPpcTlbReadEntryWord3 ();
IMPORT int mmuPpcTlbWriteExecute ();
IMPORT int mmuProtStateTransTbl;
IMPORT int mmuRegionTblMask ();
IMPORT int mmuValidStateTransTbl;
IMPORT int modf ();
IMPORT int moduleCheck ();
IMPORT int moduleCreate ();
IMPORT int moduleCreateHookAdd ();
IMPORT int moduleCreateHookDelete ();
IMPORT int moduleDelete ();
IMPORT int moduleDescDestroy ();
IMPORT int moduleEach ();
IMPORT int moduleFindByGroup ();
IMPORT int moduleFindByName ();
IMPORT int moduleFindByNameAndPath ();
IMPORT int moduleFirstSectionGet ();
IMPORT int moduleFlagsGet ();
IMPORT int moduleFlagsSet ();
IMPORT int moduleIdFigure ();
IMPORT int moduleIdListGet ();
IMPORT int moduleIdVerify ();
IMPORT int moduleInfoGet ();
IMPORT int moduleInit ();
IMPORT int moduleLibInit ();
IMPORT int moduleNameGet ();
IMPORT int moduleNextSectionGet ();
IMPORT int moduleSectionChecksumSet ();
IMPORT int moduleSectionDescFree ();
IMPORT int moduleSectionDescGet ();
IMPORT int moduleSectionRegister ();
IMPORT int moduleSegAdd ();
IMPORT int moduleSegEach ();
IMPORT int moduleSegFirst ();
IMPORT int moduleSegGet ();
IMPORT int moduleSegInfoGet ();
IMPORT int moduleSegNext ();
IMPORT int moduleShow ();
IMPORT int moduleShowInit ();
IMPORT int moduleUndefSymAdd ();
IMPORT int msgDesc;
IMPORT int msgQClassId;
IMPORT int msgQCreate ();
IMPORT int msgQCreateLibInit ();
IMPORT int msgQDelete ();
IMPORT int msgQDestroy ();
IMPORT int msgQEvStart ();
IMPORT int msgQEvStop ();
IMPORT int msgQInfoGet ();
IMPORT int msgQInit ();
IMPORT int msgQInitialize ();
IMPORT int msgQLibInit ();
IMPORT int msgQLibInstalled;
IMPORT int msgQNumMsgs ();
IMPORT int msgQReceive ();
IMPORT int msgQSend ();
IMPORT int msgQShow ();
IMPORT int msgQShowInit ();
IMPORT int msgQSmNumMsgsRtn;
IMPORT int msgQSmReceiveRtn;
IMPORT int msgQSmSendRtn;
IMPORT int msgQTerminate ();
IMPORT int mutexOptionsHostLib;
IMPORT int mutexOptionsIosLib;
IMPORT int mutexOptionsLogLib;
IMPORT int mutexOptionsNetDrv;
IMPORT int mutexOptionsSelectLib;
IMPORT int mutexOptionsSymLib;
IMPORT int mutexOptionsTyLib;
IMPORT int mux2Bind ();
IMPORT int mux2LibInit ();
IMPORT int mux2OverEndInit ();
IMPORT int mux2OverEndOutputFilterChainDrops;
IMPORT int mux2PollReceive ();
IMPORT int mux2PollSend ();
IMPORT int mux2Send ();
IMPORT int muxAddressForm ();
IMPORT int muxBind ();
IMPORT int muxBindCommon ();
IMPORT int muxBindStyles;
IMPORT int muxCommonInit ();
IMPORT int muxDebug;
IMPORT int muxDevAcquire ();
IMPORT int muxDevConnect_desc;
IMPORT int muxDevExists ();
IMPORT int muxDevLoad ();
IMPORT int muxDevRelease ();
IMPORT int muxDevStart ();
IMPORT int muxDevStop ();
IMPORT int muxDevStopAll ();
IMPORT int muxDevUnload ();
IMPORT int muxDevUnloadAsync ();
IMPORT int muxEndJobQDo ();
IMPORT int muxEndJobQDoJob ();
IMPORT int muxEndListGet ();
IMPORT int muxEndQnumGet ();
IMPORT int muxEndRxJobQueue ();
IMPORT int muxEndStyles;
IMPORT int muxError ();
IMPORT int muxErrorPost ();
IMPORT int muxErrorSkip ();
IMPORT int muxIfTypeGet ();
IMPORT int muxIoctl ();
IMPORT int muxIpcomPktFreeMblk ();
IMPORT int muxIpcomPktToMblkTx ();
IMPORT int muxL2PortDetachRtn;
IMPORT int muxLibInit ();
IMPORT int muxLinkDownNotify ();
IMPORT int muxLinkHeaderCreate ();
IMPORT int muxLinkUpNotify ();
IMPORT int muxLock;
IMPORT int muxMCastAddrAdd ();
IMPORT int muxMCastAddrDel ();
IMPORT int muxMCastAddrGet ();
IMPORT int muxMblkToIpcomPktMallocDrops;
IMPORT int muxMblkToIpcomPktTx ();
IMPORT int muxMblkToIpcomPktTxChainDrops;
IMPORT int muxPacketAddrGet ();
IMPORT int muxPacketDataGet ();
IMPORT int muxPollReceive ();
IMPORT int muxPollSend ();
IMPORT int muxProtoInfoGet ();
IMPORT int muxProtoListGet ();
IMPORT int muxReceive ();
IMPORT int muxRestoreIpcomPktFromMblkTx ();
IMPORT int muxSend ();
IMPORT int muxShow ();
IMPORT int muxTkBind ();
IMPORT int muxTkCookieGet ();
IMPORT int muxTkDrvCheck ();
IMPORT int muxTkLibInit ();
IMPORT int muxTkOverEndInit ();
IMPORT int muxTkPollReceive ();
IMPORT int muxTkPollSend ();
IMPORT int muxTkSend ();
IMPORT int muxTxRestart ();
IMPORT int muxUnbind ();
IMPORT int mv ();
IMPORT int nae_log ();
IMPORT int namelessPrefix;
IMPORT int netBufClOffset;
IMPORT int netBufLibInit ();
IMPORT int netBufLibInitialize ();
IMPORT int netBufLock;
IMPORT int netBufPoolInit ();
IMPORT int netClBlkFree ();
IMPORT int netClBlkGet ();
IMPORT int netClBlkJoin ();
IMPORT int netClFree ();
IMPORT int netClPoolIdGet ();
IMPORT int netClusterGet ();
IMPORT int netDaemonQ ();
IMPORT int netDaemonQnum ();
IMPORT int netDaemonsCount ();
IMPORT int netDaemonsStart ();
IMPORT int netDevCreate ();
IMPORT int netDevCreate2 ();
IMPORT int netDrv ();
IMPORT int netDrvDebugLevelSet ();
IMPORT int netDrvFileDoesNotExist ();
IMPORT int netDrvFileDoesNotExistInstall ();
IMPORT int netErrnoSet ();
IMPORT int netHelp ();
IMPORT int netJobAdd ();
IMPORT int netJobNum;
IMPORT int netJobQueueId;
IMPORT int netLibInitialize ();
IMPORT int netLsByName ();
IMPORT int netMblkChainDup ();
IMPORT int netMblkClChainFree ();
IMPORT int netMblkClFree ();
IMPORT int netMblkClGet ();
IMPORT int netMblkClJoin ();
IMPORT int netMblkDup ();
IMPORT int netMblkFree ();
IMPORT int netMblkFromBufCopy ();
IMPORT int netMblkGet ();
IMPORT int netMblkOffsetToBufCopy ();
IMPORT int netMblkToBufCopy ();
IMPORT int netPoolCreate ();
IMPORT int netPoolCreateValidate ();
IMPORT int netPoolDelete ();
IMPORT int netPoolIdGet ();
IMPORT int netPoolIdGetWork ();
IMPORT int netPoolInit ();
IMPORT int netPoolInitWrapper ();
IMPORT int netPoolKheapInit ();
IMPORT int netPoolNameGet ();
IMPORT int netPoolRelPri;
IMPORT int netPoolRelease ();
IMPORT int netTaskId;
IMPORT int netTaskNamePrefix;
IMPORT int netTaskOptions;
IMPORT int netTaskPriority;
IMPORT int netTaskStackSize;
IMPORT int netTupleFree ();
IMPORT int netTupleGet ();
IMPORT int netVersionString;
IMPORT int networkinit ();
IMPORT int noDev ();
IMPORT int ns16550SioRegister ();
IMPORT int ns1655x1Resources;
IMPORT int ns1655x2Resources;
IMPORT int ns1655x3Resources;
IMPORT int ns1655x4Resources;
IMPORT int numLogFds;
IMPORT int numberStaticAvail;
IMPORT int objAlloc ();
IMPORT int objAllocExtra ();
IMPORT int objArchLibInit ();
IMPORT int objClassIdGet ();
IMPORT int objClassTypeGet ();
IMPORT int objClassTypeShow ();
IMPORT int objClassTypeString;
IMPORT int objContextGet ();
IMPORT int objContextSet ();
IMPORT int objCoreInit ();
IMPORT int objCoreInvalidate ();
IMPORT int objCoreReclaim ();
IMPORT int objCoreTerminate ();
IMPORT int objCoreUnlink ();
IMPORT int objEach ();
IMPORT int objFree ();
IMPORT int objGenericVerify ();
IMPORT int objHandleShow ();
IMPORT int objHandleTblShow ();
IMPORT int objInfo ();
IMPORT int objInfoInit ();
IMPORT int objLibInit ();
IMPORT int objMemAlloc ();
IMPORT int objMemFree ();
IMPORT int objNameGet ();
IMPORT int objNameLenGet ();
IMPORT int objNamePtrGet ();
IMPORT int objNamePtrSet ();
IMPORT int objNameSet ();
IMPORT int objNameToId ();
IMPORT int objOwnerGet ();
IMPORT int objOwnerLibInstalled;
IMPORT int objOwnerSet ();
IMPORT int objOwnerSetBase ();
IMPORT int objRoutineNameGet ();
IMPORT int objShow ();
IMPORT int objShowAll ();
IMPORT int objShowInit ();
IMPORT int objVerify ();
IMPORT int objVerifyError ();
IMPORT int objVerifyMagicCompare ();
IMPORT int objVerifyMagicRead ();
IMPORT int objVerifyTypeCompare ();
IMPORT int objVerifyTypeRead ();
IMPORT int objectShow ();
IMPORT int open ();
IMPORT int openPicTimerDevAresources;
IMPORT int openPicTimerDevBresources;
IMPORT int openPicTimerDrvRegister ();
IMPORT int opendir ();
IMPORT int oprintf ();
IMPORT int pBusHead;
IMPORT int pBusListHead;
IMPORT int pClkCookie;
IMPORT int pClkTimer;
IMPORT int pCoprocDescList;
IMPORT int pDbgClnt;
IMPORT int pDriverListHead;
IMPORT int pHead;
IMPORT int pIosDevCloseRtn;
IMPORT int pIosFdEntrySetRtn;
IMPORT int pIosFdInvalidRtn;
IMPORT int pIosRemovableDevDrv;
IMPORT int pIosSuspendedDevDrv;
IMPORT int pJobPool;
IMPORT int pLoadRoutine;
IMPORT int pLoaderHooksCall;
IMPORT int pM2TrapRtn;
IMPORT int pM2TrapRtnArg;
IMPORT int pMibRtn;
IMPORT int pNetPoolHead;
IMPORT int pNullFd;
IMPORT int pPlbBus;
IMPORT int pPlbDev;
IMPORT int pPlbDevControl;
IMPORT int pRebootHookTbl;
IMPORT int pReservedAtomics;
IMPORT int pRootMemStart;
IMPORT int pSockIoctlMemVal;
IMPORT int pSysClkName;
IMPORT int pSysPlbMethods;
IMPORT int pTaskLastDspTcb;
IMPORT int pTaskLastFpTcb;
IMPORT int pUnixIoctlMemVal;
IMPORT int pVxbDelayCookie;
IMPORT int pVxbDelayTimer;
IMPORT int pVxbEpicIsrHandle;
IMPORT int pVxbM85xxTimerInstance;
IMPORT int pVxbPpcIntCtlrDrvCtrl;
IMPORT int pVxbSpinLockGive;
IMPORT int pVxbSpinLockTake;
IMPORT int pWvNetEvtMap;
IMPORT int panic ();
IMPORT int panicSuspend;
IMPORT int pathBuild ();
IMPORT int pathCat ();
IMPORT int pathCondense ();
IMPORT int pathLastName ();
IMPORT int pathLastNamePtr ();
IMPORT int pathParse ();
IMPORT int pathSplit ();
IMPORT int pathconf ();
IMPORT int pause ();
IMPORT int pc ();
IMPORT int period ();
IMPORT int periodRun ();
IMPORT int perror ();
IMPORT int pipeDevCreate ();
IMPORT int pipeDevDelete ();
IMPORT int pipeDrv ();
IMPORT int pipeMsgQOptions;
IMPORT int platRatioTable;
IMPORT int plbInit1 ();
IMPORT int plbIntrGet ();
IMPORT int plbIntrSet ();
IMPORT int plbRegister ();
IMPORT int poolBlockAdd ();
IMPORT int poolCreate ();
IMPORT int poolDelete ();
IMPORT int poolFindItem ();
IMPORT int poolFreeCount ();
IMPORT int poolIdListGet ();
IMPORT int poolIncrementGet ();
IMPORT int poolIncrementSet ();
IMPORT int poolInitialize ();
IMPORT int poolItemGet ();
IMPORT int poolItemReturn ();
IMPORT int poolLibInit ();
IMPORT int poolListGbl;
IMPORT int poolShow ();
IMPORT int poolTotalCount ();
IMPORT int poolUnusedBlocksFree ();
IMPORT int posixSignalMode;
IMPORT int pow ();
IMPORT int ppGlobalEnviron;
IMPORT int ppcBrCompute ();
IMPORT int ppcBrExtract ();
IMPORT int ppcE500CACHE_ALIGN_SIZE;
IMPORT int ppcE500DCACHE_LINE_NUM;
IMPORT int ppcE500ICACHE_LINE_NUM;
IMPORT int ppcE500Mch ();
IMPORT int ppcE500MchEnd ();
IMPORT int ppcIntCtlr0Resources;
IMPORT int ppcIntCtlrInputs;
IMPORT int ppcIntCtlrRegister ();
IMPORT int ppcVectorFit;
IMPORT int ppcVectorSysTimer;
IMPORT int ppcVectorWdt;
IMPORT int printErr ();
IMPORT int printErrno ();
IMPORT int printExc ();
IMPORT int printLogo ();
IMPORT int printf ();
IMPORT int priv_targetName;
IMPORT int ptyDevCreate ();
IMPORT int ptyDevRemove ();
IMPORT int ptyDrv ();
IMPORT int putc ();
IMPORT int putchar ();
IMPORT int putenv ();
IMPORT int puts ();
IMPORT int putw ();
IMPORT int pwd ();
IMPORT int qFifoClass;
IMPORT int qFifoClassId;
IMPORT int qFifoEach ();
IMPORT int qFifoGet ();
IMPORT int qFifoInfo ();
IMPORT int qFifoInit ();
IMPORT int qFifoNext ();
IMPORT int qFifoPut ();
IMPORT int qFifoRemove ();
IMPORT int qFifoResort ();
IMPORT int qFifoRestore ();
IMPORT int qPriBMapClassId;
IMPORT int qPriBMapEach ();
IMPORT int qPriBMapGet ();
IMPORT int qPriBMapInfo ();
IMPORT int qPriBMapInit ();
IMPORT int qPriBMapKey ();
IMPORT int qPriBMapNext ();
IMPORT int qPriBMapPut ();
IMPORT int qPriBMapRemove ();
IMPORT int qPriBMapResort ();
IMPORT int qPriBMapRestore ();
IMPORT int qPriBMapSet ();
IMPORT int qPriDeltaAdvance ();
IMPORT int qPriDeltaClassId;
IMPORT int qPriDeltaEach ();
IMPORT int qPriDeltaGet ();
IMPORT int qPriDeltaGetExpired ();
IMPORT int qPriDeltaInfo ();
IMPORT int qPriDeltaInit ();
IMPORT int qPriDeltaKey ();
IMPORT int qPriDeltaNext ();
IMPORT int qPriDeltaPut ();
IMPORT int qPriDeltaRemove ();
IMPORT int qPriDeltaResort ();
IMPORT int qPriListClassId;
IMPORT int qPriListEach ();
IMPORT int qPriListFromTailClassId;
IMPORT int qPriListGet ();
IMPORT int qPriListInfo ();
IMPORT int qPriListInit ();
IMPORT int qPriListKey ();
IMPORT int qPriListNext ();
IMPORT int qPriListPut ();
IMPORT int qPriListPutFromTail ();
IMPORT int qPriListRemove ();
IMPORT int qPriListResort ();
IMPORT int qTradPriBMapClassId;
IMPORT int qTradPriBMapSetCompatible ();
IMPORT int qTradPriListClassId;
IMPORT int qTradPriListSetCompatible ();
IMPORT int qman0Resources;
IMPORT int qmanRegister ();
IMPORT int qportalAcquire ();
IMPORT int qportalCStateQuery ();
IMPORT int qportalCalloutAdd ();
IMPORT int qportalCalloutDelete ();
IMPORT int qportalCgInit ();
IMPORT int qportalCgModify ();
IMPORT int qportalCgQuery ();
IMPORT int qportalCgTestWrite ();
IMPORT int qportalCreate ();
IMPORT int qportalDcaDequeue ();
IMPORT int qportalDcaEnqueue ();
IMPORT int qportalDcaRelease ();
IMPORT int qportalDequeue ();
IMPORT int qportalDequeueSourceSelect ();
IMPORT int qportalDestroy ();
IMPORT int qportalEnqueue ();
IMPORT int qportalFqFullInit ();
IMPORT int qportalFqInit ();
IMPORT int qportalFqNpQuery ();
IMPORT int qportalFqQuery ();
IMPORT int qportalFqShutdown ();
IMPORT int qportalFqTdCalc ();
IMPORT int qportalFullDequeue ();
IMPORT int qportalFullEnqueue ();
IMPORT int qportalIntConnect ();
IMPORT int qportalIntDisable ();
IMPORT int qportalIntDisconnect ();
IMPORT int qportalIntEnable ();
IMPORT int qportalIntHandlingDisable ();
IMPORT int qportalIntHandlingEnable ();
IMPORT int qportalIntHandlingSelect ();
IMPORT int qportalLocalDequeue ();
IMPORT int qportalLocalEnqueue ();
IMPORT int qportalMrCalloutAdd ();
IMPORT int qportalMrCalloutDelete ();
IMPORT int qportalMrDequeue ();
IMPORT int qportalMrDequeueFull ();
IMPORT int qportalMrLocalDequeue ();
IMPORT int qportalOrpDequeue ();
IMPORT int qportalOrpEnqueue ();
IMPORT int qportalOrpLocalDequeue ();
IMPORT int qportalOrpLocalEnqueue ();
IMPORT int qportalOrpMrDequeue ();
IMPORT int qportalOrpSafeDequeue ();
IMPORT int qportalOrpSafeEnqueue ();
IMPORT int qportalRelease ();
IMPORT int qportalSafeDequeue ();
IMPORT int qportalSafeEnqueue ();
IMPORT int qportalSafeMrDequeue ();
IMPORT int qportalSet ();
IMPORT int qsort ();
IMPORT int raise ();
IMPORT int rand ();
IMPORT int rcmd ();
IMPORT int rcmd_af ();
IMPORT int read ();
IMPORT int readdir ();
IMPORT int readdir_r ();
IMPORT int readv ();
IMPORT int readyQBMap;
IMPORT int readyQHead;
IMPORT int realloc ();
IMPORT int reboot ();
IMPORT int rebootHookAdd ();
IMPORT int rebootHookDelete ();
IMPORT int rebootHookTbl;
IMPORT int rebootHookTblSize;
IMPORT int recv ();
IMPORT int recvfrom ();
IMPORT int recvmsg ();
IMPORT int reld ();
IMPORT int remCurIdGet ();
IMPORT int remCurIdSet ();
IMPORT int remLastResvPort;
IMPORT int remLibInit ();
IMPORT int remNumConnRetrials;
IMPORT int remPasswd;
IMPORT int remStdErrSetupTimeout;
IMPORT int remUser;
IMPORT int remove ();
IMPORT int rename ();
IMPORT int repeat ();
IMPORT int repeatRun ();
IMPORT int reschedule ();
IMPORT int restartTaskName;
IMPORT int restartTaskOptions;
IMPORT int restartTaskPriority;
IMPORT int restartTaskStackSize;
IMPORT int retran_noprs;
IMPORT int retran_notmr;
IMPORT int rewind ();
IMPORT int rewinddir ();
IMPORT int rindex ();
IMPORT int rm ();
IMPORT int rmdir ();
IMPORT int rngBufGet ();
IMPORT int rngBufPut ();
IMPORT int rngCreate ();
IMPORT int rngDelete ();
IMPORT int rngFlush ();
IMPORT int rngFreeBytes ();
IMPORT int rngIsEmpty ();
IMPORT int rngIsFull ();
IMPORT int rngMoveAhead ();
IMPORT int rngNBytes ();
IMPORT int rngPutAhead ();
IMPORT int romfsImage;
IMPORT int romfsImageEnd;
IMPORT int rootMemNBytes;
IMPORT int rootTaskId;
IMPORT int roundRobinHookInstalled;
IMPORT int roundRobinOn;
IMPORT int roundRobinSlice;
IMPORT int rresvport ();
IMPORT int rresvportCommon ();
IMPORT int rresvport_af ();
IMPORT int rtpDeleteMsg;
IMPORT int rtpSigDeleteMsg;
IMPORT int rtpSigStopMsg;
IMPORT int rtpStopMsg;
IMPORT char * runtimeName;
IMPORT char * runtimeVersion;
IMPORT char * runtimeVersionFull;
IMPORT int s ();
IMPORT int saveExcMsg ();
IMPORT int scalb ();
IMPORT int scanCharSet ();
IMPORT int scanField ();
IMPORT int scanf ();
IMPORT int selNodeAdd ();
IMPORT int selNodeDelete ();
IMPORT int selTaskDeleteHookAdd ();
IMPORT int selWakeup ();
IMPORT int selWakeupAll ();
IMPORT int selWakeupListInit ();
IMPORT int selWakeupListLen ();
IMPORT int selWakeupListTerm ();
IMPORT int selWakeupType ();
IMPORT int select ();
IMPORT int selectInit ();
IMPORT int semBCreate ();
IMPORT int semBGive ();
IMPORT int semBGiveDefer ();
IMPORT int semBGiveNoLock ();
IMPORT int semBGiveScalable ();
IMPORT int semBInit ();
IMPORT int semBInitialize ();
IMPORT int semBTake ();
IMPORT int semBTakeNoLock ();
IMPORT int semBTakeScalable ();
IMPORT int semCCreate ();
IMPORT int semCGive ();
IMPORT int semCGiveDefer ();
IMPORT int semCInit ();
IMPORT int semCInitialize ();
IMPORT int semCTake ();
IMPORT int semClass;
IMPORT int semClassId;
IMPORT int semCreateTbl;
IMPORT int semDelete ();
IMPORT int semDeleteLibInit ();
IMPORT int semDestroy ();
IMPORT int semEvIsFreeTbl;
IMPORT int semEvStart ();
IMPORT int semEvStop ();
IMPORT int semFlush ();
IMPORT int semFlushDefer ();
IMPORT int semFlushDeferTbl;
IMPORT int semFlushTbl;
IMPORT int semGive ();
IMPORT int semGiveDefer ();
IMPORT int semGiveDeferTbl;
IMPORT int semGiveNoLockTbl;
IMPORT int semGiveTbl;
IMPORT int semInfo ();
IMPORT int semInfoGet ();
IMPORT int semIntRestrict ();
IMPORT int semInvalid ();
IMPORT int semLibInit ();
IMPORT int semLocalIdVerify ();
IMPORT int semMCreate ();
IMPORT int semMGive ();
IMPORT int semMGiveForce ();
IMPORT int semMGiveKernWork ();
IMPORT int semMGiveNoLock ();
IMPORT int semMGiveScalable ();
IMPORT int semMInit ();
IMPORT int semMInitialize ();
IMPORT int semMPendQPut ();
IMPORT int semMTake ();
IMPORT int semMTakeByProxy ();
IMPORT int semMTakeNoLock ();
IMPORT int semMTakeScalable ();
IMPORT int semQFlush ();
IMPORT int semQFlushDefer ();
IMPORT int semQInit ();
IMPORT int semRTake ();
IMPORT int semRWCreate ();
IMPORT int semRWGive ();
IMPORT int semRWGiveForce ();
IMPORT int semRWInitialize ();
IMPORT int semRWLibMaxReaders;
IMPORT int semShow ();
IMPORT int semShowInit ();
IMPORT int semTake ();
IMPORT int semTakeNoLockTbl;
IMPORT int semTakeTbl;
IMPORT int semTerminate ();
IMPORT int semWTake ();
IMPORT int send ();
IMPORT int send_full;
IMPORT int sendmsg ();
IMPORT int sendto ();
IMPORT int setbuf ();
IMPORT int setbuffer ();
IMPORT int sethostname ();
IMPORT int setjmp ();
IMPORT int setlinebuf ();
IMPORT int setlocale ();
IMPORT int setsockopt ();
IMPORT int settimeofday ();
IMPORT int setvbuf ();
IMPORT int sfpDoubleNormalize ();
IMPORT int sfpDoubleNormalize2 ();
IMPORT int shConfig ();
IMPORT int sha1_block_data_order ();
IMPORT int sha1_block_host_order ();
IMPORT int shellAbort ();
IMPORT int shellAbortHandledByShell ();
IMPORT int shellActivate ();
IMPORT int shellBackgroundInit ();
IMPORT int shellBackgroundRead ();
IMPORT int shellCompatibleCheck ();
IMPORT int shellCompatibleSet ();
IMPORT int shellCompletionPathnameGet ();
IMPORT int shellConfigDefaultGet ();
IMPORT int shellConfigDefaultSet ();
IMPORT int shellConfigDefaultValueGet ();
IMPORT int shellConfigDefaultValueSet ();
IMPORT int shellConfigDefaultValueUnset ();
IMPORT int shellConfigGet ();
IMPORT int shellConfigLibInit ();
IMPORT int shellConfigSet ();
IMPORT int shellConfigValueGet ();
IMPORT int shellConfigValueSet ();
IMPORT int shellConfigValueUnset ();
IMPORT int shellConsoleInit ();
IMPORT int shellConsoleTaskId;
IMPORT int shellContextFinalize ();
IMPORT int shellContextInit ();
IMPORT int shellContextListLock ();
IMPORT int shellContextListUnlock ();
IMPORT int shellDataAdd ();
IMPORT int shellDataAllRemove ();
IMPORT int shellDataFirst ();
IMPORT int shellDataFromNameAdd ();
IMPORT int shellDataFromNameGet ();
IMPORT int shellDataGet ();
IMPORT int shellDataNext ();
IMPORT int shellDataRemove ();
IMPORT int shellErrnoGet ();
IMPORT int shellErrnoSet ();
IMPORT int shellExec ();
IMPORT int shellExitWrapper ();
IMPORT int shellFirst ();
IMPORT int shellFromNameGet ();
IMPORT int shellFromTaskGet ();
IMPORT int shellFromTaskParentContextGet ();
IMPORT int shellGenericCreate ();
IMPORT int shellGenericInit ();
IMPORT int shellHistory ();
IMPORT int shellIdVerify ();
IMPORT int shellInOutBgndClose ();
IMPORT int shellInOutBgndGet ();
IMPORT int shellInOutBgndIoClose ();
IMPORT int shellInOutBgndOpen ();
IMPORT int shellInOutBgndRegister ();
IMPORT int shellInOutBlockingBlock ();
IMPORT int shellInOutBlockingBlockedDataGet ();
IMPORT int shellInOutBlockingClose ();
IMPORT int shellInOutBlockingDataSet ();
IMPORT int shellInOutBlockingGet ();
IMPORT int shellInOutBlockingOnDataClose ();
IMPORT int shellInOutBlockingOpen ();
IMPORT int shellInOutBlockingUnblock ();
IMPORT int shellInOutGet ();
IMPORT int shellInOutInitialGet ();
IMPORT int shellInOutInputHookAdd ();
IMPORT int shellInOutInputHookDelete ();
IMPORT int shellInOutInputHookSet ();
IMPORT int shellInOutInputHookUnset ();
IMPORT int shellInOutInputUniqueCheck ();
IMPORT int shellInOutMasterPtyGet ();
IMPORT int shellInOutRedirectClose ();
IMPORT int shellInOutRedirectOpen ();
IMPORT int shellInOutRedirectRestore ();
IMPORT int shellInOutRedirectScript ();
IMPORT int shellInOutRedirectSet ();
IMPORT int shellInOutSet ();
IMPORT int shellInOutSlavePtyGet ();
IMPORT int shellInOutUse ();
IMPORT int shellInternalArgStackAllocate ();
IMPORT int shellInternalArgumentEval ();
IMPORT int shellInternalDataSymbolGet ();
IMPORT int shellInternalDecValueDisplay ();
IMPORT int shellInternalFuncDoubleArgAdd ();
IMPORT int shellInternalFuncFloatArgAdd ();
IMPORT int shellInternalFuncLLongArgAdd ();
IMPORT int shellInternalFuncLongArgAdd ();
IMPORT int shellInternalFunctionCall ();
IMPORT int shellInternalHexValueDisplay ();
IMPORT int shellInternalKernelSymbolCreate ();
IMPORT int shellInternalOptFree ();
IMPORT int shellInternalOptGet ();
IMPORT int shellInternalStrSpaceTokenize ();
IMPORT int shellInternalStrToChar ();
IMPORT int shellInternalStrToFloat ();
IMPORT int shellInternalStrToInt ();
IMPORT int shellInternalStrToInt2 ();
IMPORT int shellInternalStrToInt32 ();
IMPORT int shellInternalStrToLong ();
IMPORT int shellInternalStrTokenize ();
IMPORT int shellInternalStrUnquote ();
IMPORT int shellInternalSymTblFromRtpGet ();
IMPORT int shellInternalSymTblFromTaskGet ();
IMPORT int shellInternalSymbolDisplay ();
IMPORT int shellInternalSymbolGet ();
IMPORT int shellInternalSymbolsResolve ();
IMPORT int shellInternalTaskNameMatch ();
IMPORT int shellInternalTextSymbolGet ();
IMPORT int shellInternalValueDisplay ();
IMPORT int shellInterpByNameFind ();
IMPORT int shellInterpCInit ();
IMPORT int shellInterpC_create_buffer ();
IMPORT int shellInterpC_delete_buffer ();
IMPORT int shellInterpC_flush_buffer ();
IMPORT int shellInterpC_init_buffer ();
IMPORT int shellInterpC_load_buffer_state ();
IMPORT int shellInterpC_scan_buffer ();
IMPORT int shellInterpC_scan_bytes ();
IMPORT int shellInterpC_scan_string ();
IMPORT int shellInterpC_switch_to_buffer ();
IMPORT int shellInterpCin;
IMPORT int shellInterpCleng;
IMPORT int shellInterpClex ();
IMPORT int shellInterpCout;
IMPORT int shellInterpCparse ();
IMPORT int shellInterpCrestart ();
IMPORT int shellInterpCtext;
IMPORT int shellInterpCtxCreate ();
IMPORT int shellInterpCtxGet ();
IMPORT int shellInterpDefaultNameGet ();
IMPORT int shellInterpDelete ();
IMPORT int shellInterpEvaluate ();
IMPORT int shellInterpLibInit ();
IMPORT int shellInterpNameGet ();
IMPORT int shellInterpPromptDftSet ();
IMPORT int shellInterpPromptGet ();
IMPORT int shellInterpPromptSet ();
IMPORT int shellInterpRegister ();
IMPORT int shellInterpRestart ();
IMPORT int shellInterpSet ();
IMPORT int shellInterpSwitch ();
IMPORT int shellLibInit ();
IMPORT int shellLineLenGet ();
IMPORT int shellLock ();
IMPORT int shellLoginInstall ();
IMPORT int shellLogout ();
IMPORT int shellLogout2 ();
IMPORT int shellLogoutInstall ();
IMPORT int shellMaxSessionsSet ();
IMPORT int shellMemAllRegFree ();
IMPORT int shellMemCalloc ();
IMPORT int shellMemFree ();
IMPORT int shellMemLibInit ();
IMPORT int shellMemMalloc ();
IMPORT int shellMemRealloc ();
IMPORT int shellMemRegAllFree ();
IMPORT int shellMemRegFree ();
IMPORT int shellMemRegMalloc ();
IMPORT int shellMemRegister ();
IMPORT int shellMemStrdup ();
IMPORT int shellMemStringAdd ();
IMPORT int shellMemStringFree ();
IMPORT int shellMemStringIsAllocated ();
IMPORT int shellMemStringShow ();
IMPORT int shellMemUnregister ();
IMPORT int shellNext ();
IMPORT int shellOutputLock ();
IMPORT int shellOutputUnlock ();
IMPORT int shellParserControl ();
IMPORT int shellPrint ();
IMPORT int shellPrintErr ();
IMPORT int shellPromptFmtDftSet ();
IMPORT int shellPromptFmtSet ();
IMPORT int shellPromptFmtStrAdd ();
IMPORT int shellPromptLibInit ();
IMPORT int shellPromptPrint ();
IMPORT int shellPromptSet ();
IMPORT int shellRemoteConfigStr;
IMPORT int shellResourceReleaseHookAdd ();
IMPORT int shellRestart ();
IMPORT int shellScriptAbort ();
IMPORT int shellScriptNoAbort ();
IMPORT int shellTask ();
IMPORT int shellTaskDefaultBaseNameGet ();
IMPORT int shellTaskDefaultBaseNameSet ();
IMPORT int shellTaskDefaultOptionsGet ();
IMPORT int shellTaskDefaultOptionsSet ();
IMPORT int shellTaskDefaultPriorityGet ();
IMPORT int shellTaskDefaultPrioritySet ();
IMPORT int shellTaskDefaultStackSizeGet ();
IMPORT int shellTaskDefaultStackSizeSet ();
IMPORT int shellTaskGet ();
IMPORT int shellTaskIdDefault ();
IMPORT int shellTerminate ();
IMPORT int shellWorkingMemCtxAttach ();
IMPORT int shellWorkingMemCtxAttachedShow ();
IMPORT int shellWorkingMemCtxCurrentGet ();
IMPORT int shellWorkingMemCtxCurrentSet ();
IMPORT int shellWorkingMemCtxDetach ();
IMPORT int shellWorkingMemCtxDuplicate ();
IMPORT int shellWorkingMemCtxGet ();
IMPORT int shellWorkingMemCtxIdConvert ();
IMPORT int shellWorkingMemCtxLastGet ();
IMPORT int shellWorkingMemCtxRtnSet ();
IMPORT int show ();
IMPORT int showLoggerInfo ();
IMPORT int shutdown ();
IMPORT int sigEvtRtn;
IMPORT int sigInit ();
IMPORT int sigPendDestroy ();
IMPORT int sigPendInit ();
IMPORT int sigPendKill ();
IMPORT int sigaction ();
IMPORT int sigaddset ();
IMPORT int sigblock ();
IMPORT int sigdelset ();
IMPORT int sigemptyset ();
IMPORT int sigfillset ();
IMPORT int sigismember ();
IMPORT int signal ();
IMPORT int sigpending ();
IMPORT int sigprocmask ();
IMPORT int sigqueue ();
IMPORT int sigqueueInit ();
IMPORT int sigreturn ();
IMPORT int sigsetjmp ();
IMPORT int sigsetmask ();
IMPORT int sigsuspend ();
IMPORT int sigtimedwait ();
IMPORT int sigvec ();
IMPORT int sigwait ();
IMPORT int sigwaitinfo ();
IMPORT int sin ();
IMPORT int sinh ();
IMPORT int sioChanConnect_desc;
IMPORT int sioChanGet_desc;
IMPORT int sioEnable_desc;
IMPORT int sioIntEnable_desc;
IMPORT int sioNextChannelNumberAssign ();
IMPORT int sizeNodesAlloc ();
IMPORT int sllCount ();
IMPORT SL_NODE *sllEach ();
IMPORT SL_NODE *sllGet ();
IMPORT int sllInit ();
IMPORT SL_NODE *sllPrevious ();
IMPORT void sllPutAtHead ();
IMPORT void sllPutAtTail ();
IMPORT void sllRemove ();
IMPORT int sllTerminate ();
IMPORT int smObjPoolMinusOne;
IMPORT int smObjTaskDeleteFailRtn;
IMPORT int smObjTcbFreeFailRtn;
IMPORT int smObjTcbFreeRtn;
IMPORT int smsgDesc;
IMPORT int snoopEnable;
IMPORT int snprintf ();
IMPORT int so ();
IMPORT int sockInfo ();
IMPORT int sockLibAdd ();
IMPORT int sockLibInit ();
IMPORT int sockLibMap;
IMPORT int sockShow ();
IMPORT int socket ();
IMPORT int socketDevHdr;
IMPORT int sp ();
IMPORT int spTaskOptions;
IMPORT int spTaskPriority;
IMPORT int spTaskStackSize;
IMPORT int spinLockIsrGive ();
IMPORT int spinLockIsrInit ();
IMPORT int spinLockIsrTake ();
IMPORT int spinLockRestrict ();
IMPORT int spinLockTaskGive ();
IMPORT int spinLockTaskInit ();
IMPORT int spinLockTaskTake ();
IMPORT int sprintf ();
IMPORT int spy ();
IMPORT int spyClkStart ();
IMPORT int spyClkStop ();
IMPORT int spyHelp ();
IMPORT int spyReport ();
IMPORT int spyStop ();
IMPORT int spyTask ();
IMPORT int sqrt ();
IMPORT int srand ();
IMPORT int sscanf ();
IMPORT int standAloneSymTbl;
IMPORT int stat ();
IMPORT int statSymTbl;
IMPORT int statfs ();
IMPORT int statfs64 ();
IMPORT int staticPTEArray;
IMPORT int stdioFiles;
IMPORT int stdioFp ();
IMPORT int stdioFpCreate ();
IMPORT int stdioFpDestroy ();
IMPORT int stdioInit ();
IMPORT int stdioShow ();
IMPORT int stdioShowInit ();
IMPORT int stdlibFiles;
IMPORT int stopApplLogger ();
IMPORT int strFree ();
IMPORT int strcasecmp ();
IMPORT int strcat ();
IMPORT int strchr ();
IMPORT int strcmp ();
IMPORT int strcoll ();
IMPORT int strcpy ();
IMPORT int strcspn ();
IMPORT int strdup ();
IMPORT int strerror ();
IMPORT int strerrorIf ();
IMPORT int strerror_r ();
IMPORT int strftime ();
IMPORT int stringFiles;
IMPORT int strlcpy ();
IMPORT int strlen ();
IMPORT int strncat ();
IMPORT int strncmp ();
IMPORT int strncpy ();
IMPORT int strpbrk ();
IMPORT int strrchr ();
IMPORT int strspn ();
IMPORT int strstr ();
IMPORT int strtod ();
IMPORT int strtok ();
IMPORT int strtok_r ();
IMPORT int strtol ();
IMPORT int strtoul ();
IMPORT int strxfrm ();
IMPORT int substrcmp ();
IMPORT int swab ();
IMPORT int symAdd ();
IMPORT int symByCNameFind ();
IMPORT int symByValueAndTypeFind ();
IMPORT int symByValueFind ();
IMPORT int symEach ();
IMPORT int symEachCall ();
IMPORT int symFind ();
IMPORT int symFindByName ();
IMPORT int symFindByNameAndType ();
IMPORT int symFindByValue ();
IMPORT int symFindByValueAndType ();
IMPORT int symFindSymbol ();
IMPORT int symFree ();
IMPORT int symGroupDefault;
IMPORT int symLibInit ();
IMPORT int symListGet ();
IMPORT int symLkupPgSz;
IMPORT int symNameGet ();
IMPORT int symRegister ();
IMPORT int symRemove ();
IMPORT int symShow ();
IMPORT int symShowInit ();
IMPORT int symTblAdd ();
IMPORT int symTblCreate ();
IMPORT int symTblDelete ();
IMPORT int symTblRemove ();
IMPORT int symTblShutdown ();
IMPORT int symTblSymSetRemove ();
IMPORT int symTypeGet ();
IMPORT int symTypeToStringConvert ();
IMPORT int symValueGet ();
IMPORT int syncLoadRtn;
IMPORT int syncSymAddRtn;
IMPORT int syncSymRemoveRtn;
IMPORT int sysAdaEnable;
IMPORT int sysBoardReset ();
IMPORT int sysBootFile;
IMPORT int sysBootHost;
IMPORT int sysBootLine;
IMPORT int sysBootParams;
IMPORT int sysBspDevFilter_desc;
IMPORT int sysBspRev ();
IMPORT int sysBus;
IMPORT int sysBusClear ();
IMPORT int sysBusIntAck ();
IMPORT int sysBusTas ();
IMPORT int sysBusToLocalAdrs ();
IMPORT int sysCacheEnable ();
IMPORT int sysCacheFlush ();
IMPORT int sysClkConnect ();
IMPORT int sysClkDevUnitNo;
IMPORT int sysClkDisable ();
IMPORT int sysClkEnable ();
IMPORT int sysClkFreqGet ();
IMPORT int sysClkHandleGet ();
IMPORT int sysClkRateGet ();
IMPORT int sysClkRateSet ();
IMPORT int sysClkTimerNo;
IMPORT int sysCpcRegGet ();
IMPORT int sysCpcRegSet ();
IMPORT int sysCplusEnable;
IMPORT int sysCpu;
IMPORT int sysCpu1LoopCount;
IMPORT int sysCpuIdGet ();
IMPORT int sysCpusetReset ();
IMPORT int sysDebugModeGet ();
IMPORT int sysDebugModeInit ();
IMPORT int sysDebugModeSet ();
IMPORT int sysDebugModeSetHookAdd ();
IMPORT int sysDebugModeSetHookDelete ();
IMPORT int sysDelay ();
IMPORT int sysDoze ();
IMPORT int sysDozeCpu ();
IMPORT int sysEprGet ();
IMPORT int sysExcMsg;
IMPORT int sysFlags;
IMPORT int sysFlashDataPoll ();
IMPORT int sysFlashDelay ();
IMPORT int sysFlashErase ();
IMPORT int sysFlashGet ();
IMPORT int sysFlashSet ();
IMPORT int sysFlashTypeGet ();
IMPORT int sysFlashWrite ();
IMPORT int sysFmanClkFreqGet_desc;
IMPORT int sysFslErrataCcfA004510AndA004511 ();
IMPORT int sysFslErrataFpuA003999 ();
IMPORT int sysGetPeripheralBase ();
IMPORT int sysHwErrorString;
IMPORT int sysHwInit ();
IMPORT int sysHwInit2 ();
IMPORT int sysHwMemInit ();
IMPORT int sysICacheLock ();
IMPORT int sysICacheUnlock ();
IMPORT int sysInByte ();
IMPORT int sysInLong ();
IMPORT int sysInWord ();
IMPORT int sysInit ();
IMPORT int sysInstParamTable;
IMPORT int sysIntConnect ();
IMPORT int sysIntDisable ();
IMPORT int sysIntEnable ();
IMPORT int sysL2CacheInit ();
IMPORT int sysL3ExtWriteBufferAlloc ();
IMPORT int sysLocalToBusAdrs ();
IMPORT int sysLpidSet ();
IMPORT int sysMacIndex2Dev ();
IMPORT int sysMacIndex2Unit ();
IMPORT int sysMacOffsetGet ();
IMPORT int sysMailboxConnect ();
IMPORT int sysMailboxDisable ();
IMPORT int sysMailboxEnable ();
IMPORT int sysMemTop ();
IMPORT int sysModel ();
IMPORT int sysMotTsecEnetAddrGet ();
IMPORT int sysMsDelay ();
IMPORT int sysNetDevName;
IMPORT int sysNetMacAddrGet ();
IMPORT int sysNetMacAddrSet ();
IMPORT int sysNetMacNVRamAddrGet ();
IMPORT int sysNvRamGet ();
IMPORT int sysNvRamSet ();
IMPORT int sysOutByte ();
IMPORT int sysOutLong ();
IMPORT int sysOutWord ();
IMPORT int sysPCGet ();
IMPORT int sysPciInByte ();
IMPORT int sysPciInLong ();
IMPORT int sysPciInWord ();
IMPORT int sysPciOutByte ();
IMPORT int sysPciOutLong ();
IMPORT int sysPciOutWord ();
IMPORT int sysPciRead32 ();
IMPORT int sysPciWrite32 ();
IMPORT int sysPhysMemDesc;
IMPORT int sysPhysMemDescNumEnt;
IMPORT int sysPhysMemTop ();
IMPORT int sysPhysToVirt_desc;
IMPORT int sysPicClkFreqGet ();
IMPORT int sysProcNum;
IMPORT int sysProcNumGet ();
IMPORT int sysProcNumSet ();
IMPORT int sysSectorErase ();
IMPORT int sysSelfReset ();
IMPORT int sysSerdesRcwActual;
IMPORT int sysSerialChanConnect ();
IMPORT int sysSerialChanGet ();
IMPORT int sysSerialConnectAll ();
IMPORT int sysStartType;
IMPORT int sysStaticTlbDesc;
IMPORT int sysStaticTlbDescNumEnt;
IMPORT int sysSymTbl;
IMPORT int sysTas ();
IMPORT int sysTextProtect;
IMPORT int sysTimeBaseLGet ();
IMPORT int sysToMonitor ();
IMPORT int sysUsDelay ();
IMPORT int sysVirtToPhys_desc;
IMPORT int sysVmContext;
IMPORT int sysVmeEnable;
IMPORT int system ();
IMPORT int systemSecurityIsEnabled;
IMPORT int tExcTaskExcStk;
IMPORT int tExcTaskStk;
IMPORT int tExcTaskTcb;
IMPORT int tan ();
IMPORT int tanh ();
IMPORT int taskActivate ();
IMPORT int taskArgsGet ();
IMPORT int taskArgsSet ();
IMPORT int taskBpHook;
IMPORT int taskBpHookSet ();
IMPORT int taskClassId;
IMPORT int taskCont ();
IMPORT int taskCpuAffinityGet ();
IMPORT int taskCpuAffinitySet ();
IMPORT int taskCpuLock ();
IMPORT int taskCpuUnlock ();
IMPORT int taskCpuUnlockNoResched ();
IMPORT int taskCreat ();
IMPORT int taskCreate ();
IMPORT int taskCreateHookAdd ();
IMPORT int taskCreateHookDelete ();
IMPORT int taskCreateHookInit ();
IMPORT int taskCreateHookShow ();
IMPORT int taskCreateHooks;
IMPORT int taskCreateHooksArrayA;
IMPORT int taskCreateHooksArrayB;
IMPORT int taskCreateLibInit ();
IMPORT int taskCreateWithGuard ();
IMPORT int taskDelay ();
IMPORT int taskDelete ();
IMPORT int taskDeleteForce ();
IMPORT int taskDeleteHookAdd ();
IMPORT int taskDeleteHookDelete ();
IMPORT int taskDeleteHookShow ();
IMPORT int taskDeleteHooks;
IMPORT int taskDeleteHooksArrayA;
IMPORT int taskDeleteHooksArrayB;
IMPORT int taskDestroy ();
IMPORT int taskEach ();
IMPORT int taskExit ();
IMPORT int taskHookShowInit ();
IMPORT int taskIdCurrent;
IMPORT int taskIdDefault ();
IMPORT int taskIdFigure ();
IMPORT int taskIdListGet ();
IMPORT int taskIdListSort ();
IMPORT int taskIdSelf ();
IMPORT int taskIdVerify ();
IMPORT int taskInfoGet ();
IMPORT int taskInit ();
IMPORT int taskInitExcStk ();
IMPORT int taskInitialize ();
IMPORT int taskIsDelayed ();
IMPORT int taskIsPended ();
IMPORT int taskIsReady ();
IMPORT int taskIsStopped ();
IMPORT int taskIsSuspended ();
IMPORT int taskKerExcStackSize;
IMPORT int taskKerExcStkOverflowSize;
IMPORT int taskKerExecStkOverflowSize;
IMPORT int taskKerExecStkUnderflowSize;
IMPORT int taskLibInit ();
IMPORT int taskLock ();
IMPORT int taskMsrDefault;
IMPORT int taskMsrSet ();
IMPORT int taskName ();
IMPORT int taskNameToId ();
IMPORT int taskOptionsGet ();
IMPORT int taskOptionsSet ();
IMPORT int taskOptionsString ();
IMPORT int taskPriNormalGet ();
IMPORT int taskPriorityGet ();
IMPORT int taskPrioritySet ();
IMPORT int taskRegName;
IMPORT int taskRegsGet ();
IMPORT int taskRegsInit ();
IMPORT int taskRegsModify ();
IMPORT int taskRegsSet ();
IMPORT int taskRegsShow ();
IMPORT int taskRegsShowOutput ();
IMPORT int taskReservedFieldGet ();
IMPORT int taskReservedFieldSet ();
IMPORT int taskReservedNumAllot ();
IMPORT int taskReset ();
IMPORT int taskRestart ();
IMPORT int taskResume ();
IMPORT int taskRtnValueSet ();
IMPORT int taskSafe ();
IMPORT int taskSchedInfoGet ();
IMPORT int taskShow ();
IMPORT int taskShowInit ();
IMPORT int taskSpareFieldGet ();
IMPORT int taskSpareFieldSet ();
IMPORT int taskSpareNumAllot ();
IMPORT int taskSpawn ();
IMPORT int taskStackAllot ();
IMPORT int taskStackSizeGet ();
IMPORT int taskStatusString ();
IMPORT int taskStop ();
IMPORT int taskStopForce ();
IMPORT int taskStopMsg;
IMPORT int taskSuspend ();
IMPORT int taskSwapHookAdd ();
IMPORT int taskSwapHookAttach ();
IMPORT int taskSwapHookDelete ();
IMPORT int taskSwapHookDetach ();
IMPORT int taskSwapHookShow ();
IMPORT int taskSwapReference;
IMPORT int taskSwapTable;
IMPORT int taskSwitchHookAdd ();
IMPORT int taskSwitchHookDelete ();
IMPORT int taskSwitchHookShow ();
IMPORT int taskSwitchTable;
IMPORT int taskTcb ();
IMPORT int taskTerminate ();
IMPORT int taskUndelay ();
IMPORT int taskUnlock ();
IMPORT int taskUnsafe ();
IMPORT int taskUnsafeInternal ();
IMPORT int taskUserMmuCtxSet ();
IMPORT int taskUsrExcStackSize;
IMPORT int taskUsrExcStkOverflowSize;
IMPORT int taskUsrExecStkOverflowSize;
IMPORT int taskUsrExecStkUnderflowSize;
IMPORT int taskVarAdd ();
IMPORT int taskVarDelete ();
IMPORT int taskVarGet ();
IMPORT int taskVarInfo ();
IMPORT int taskVarInit ();
IMPORT int taskVarSet ();
IMPORT int taskWait ();
IMPORT int taskWaitShow ();
IMPORT int taskWaitShowCoreRtn ();
IMPORT int tcbReserveBitmap;
IMPORT int tcbSpareBitmap;
IMPORT int tcp_tmr_string;
IMPORT int td ();
IMPORT int tgec0Resources;
IMPORT int tgec1Resources;
IMPORT int tgecMdio0Resources;
IMPORT int ti ();
IMPORT int tick64Get ();
IMPORT int tick64Set ();
IMPORT int tickAnnounce ();
IMPORT int tickAnnounceHookAdd ();
IMPORT int tickGet ();
IMPORT int tickQHead;
IMPORT int tickSet ();
IMPORT int time ();
IMPORT int timeFiles;
IMPORT int timex ();
IMPORT int timexClear ();
IMPORT int timexFunc ();
IMPORT int timexHelp ();
IMPORT int timexInit ();
IMPORT int timexN ();
IMPORT int timexPost ();
IMPORT int timexPre ();
IMPORT int timexShow ();
IMPORT int tip ();
IMPORT int tipConfigString;
IMPORT int tipEscapeChar;
IMPORT int tipLibInit ();
IMPORT int tipStart ();
IMPORT int tmpfile ();
IMPORT int tmpnam ();
IMPORT int tolower ();
IMPORT int totalStaticDesc;
IMPORT int toupper ();
IMPORT int tr ();
IMPORT int trcDefaultArgs;
IMPORT int trcLibFuncs;
IMPORT int trcScanDepth;
IMPORT int trcStack ();
IMPORT int trcStackCtxTrace ();
IMPORT int trcStackTrace ();
IMPORT int ts ();
IMPORT int tt ();
IMPORT int ttyDevCreate ();
IMPORT int ttyDrv ();
IMPORT int tw ();
IMPORT int tyAbortFuncGet ();
IMPORT int tyAbortFuncSet ();
IMPORT int tyAbortGet ();
IMPORT int tyAbortSet ();
IMPORT int tyBackspaceChar;
IMPORT int tyBackspaceSet ();
IMPORT int tyDeleteLineChar;
IMPORT int tyDeleteLineSet ();
IMPORT int tyDevInit ();
IMPORT int tyDevRemove ();
IMPORT int tyDevTerminate ();
IMPORT int tyEOFGet ();
IMPORT int tyEOFSet ();
IMPORT int tyEofChar;
IMPORT int tyIRd ();
IMPORT int tyITx ();
IMPORT int tyIoctl ();
IMPORT int tyLibInit ();
IMPORT int tyMonitorTrapSet ();
IMPORT int tyRead ();
IMPORT int tyWrite ();
IMPORT int tyXoffHookSet ();
IMPORT int ungetc ();
IMPORT int unld ();
IMPORT int unlink ();
IMPORT int unmount ();
IMPORT int usrAimMmuConfig ();
IMPORT int usrBanner ();
IMPORT int usrBootLineCrack ();
IMPORT int usrBootLineGet ();
IMPORT int usrBootLineInit ();
IMPORT int usrBootLineParse ();
IMPORT int usrBreakpointInit ();
IMPORT int usrBreakpointSet ();
IMPORT int usrCacheEnable ();
IMPORT int usrCheckNetBootConfig ();
IMPORT int usrClock ();
IMPORT int usrExtraModules;
IMPORT int usrFtpInit ();
IMPORT int usrInit ();
IMPORT int usrIsrDeferInit ();
IMPORT int usrKernelCoreInit ();
IMPORT int usrKernelCorePreInit ();
IMPORT int usrKernelCoreShowInit ();
IMPORT int usrKernelCreateInit ();
IMPORT int usrKernelInit ();
IMPORT int usrLibInit ();
IMPORT int usrMmuInit ();
IMPORT int usrModuleLoad ();
IMPORT int usrNetAddrInit ();
IMPORT int usrNetAppInit ();
IMPORT int usrNetApplUtilInit ();
IMPORT int usrNetBoot ();
IMPORT int usrNetDaemonInit ();
IMPORT int usrNetDevNameGet ();
IMPORT int usrNetEndLibInit ();
IMPORT int usrNetHostAdd ();
IMPORT int usrNetHostSetup ();
IMPORT int usrNetInit ();
IMPORT int usrNetProtoInit ();
IMPORT int usrNetRemoteCreate ();
IMPORT int usrNetRemoteInit ();
IMPORT int usrNetSocketInit ();
IMPORT int usrNetUtilsInit ();
IMPORT int usrNetmaskGet ();
IMPORT int usrNetworkExtraModules;
IMPORT int usrRoot ();
IMPORT int usrShell ();
IMPORT int usrStandaloneInit ();
IMPORT int usrSysSymTblInit ();
IMPORT int usrTipInit ();
IMPORT int usrVxbTimerSysInit ();
IMPORT int usrVxdbgInit ();
IMPORT int uswab ();
IMPORT int utime ();
IMPORT int valloc ();
IMPORT int vecOffRelocMatch ();
IMPORT int vecOffRelocMatchRev ();
IMPORT int version ();
IMPORT int vfdprintf ();
IMPORT int vfprintf ();
IMPORT int viLedLibInit ();
IMPORT int vmAttrShow ();
IMPORT int vmAttrStringGet ();
IMPORT int vmAttrToArchConvert ();
IMPORT int vmAttrToIndepConvert ();
IMPORT int vmBaseLibInit ();
IMPORT int vmBasePageSizeGet ();
IMPORT int vmBaseStateSet ();
IMPORT int vmBufferWrite ();
IMPORT int vmContextClassId;
IMPORT int vmContextHeaderPrint ();
IMPORT int vmContextRangeShow ();
IMPORT int vmContextShow ();
IMPORT int vmContigBlockEach ();
IMPORT int vmCurrentGet ();
IMPORT int vmGlobalMapInit ();
IMPORT int vmInfoGet ();
IMPORT int vmKernelContextIdGet ();
IMPORT int vmLibInfo;
IMPORT int vmMap ();
IMPORT int vmMaxPhysBitsGet ();
IMPORT int vmOptimizedSizeGet ();
IMPORT int vmPageLock ();
IMPORT int vmPageMap ();
IMPORT int vmPageOptimize ();
IMPORT int vmPageSizeGet ();
IMPORT int vmPageUnlock ();
IMPORT int vmPageUnmap ();
IMPORT int vmPgMap ();
IMPORT int vmPgUnMap ();
IMPORT int vmPhysTranslate ();
IMPORT int vmPteSizeGet ();
IMPORT int vmShowInit ();
IMPORT int vmStateGet ();
IMPORT int vmStateSet ();
IMPORT int vmTextProtect ();
IMPORT int vmTranslate ();
IMPORT int vmpxx ();
IMPORT int voprintf ();
IMPORT int vprintf ();
IMPORT int vsnprintf ();
IMPORT int vsprintf ();
IMPORT int vxAbsTicks;
IMPORT int vxAtomic32Add ();
IMPORT int vxAtomic32And ();
IMPORT int vxAtomic32Cas ();
IMPORT int vxAtomic32Clear ();
IMPORT int vxAtomic32Dec ();
IMPORT int vxAtomic32Get ();
IMPORT int vxAtomic32Inc ();
IMPORT int vxAtomic32Nand ();
IMPORT int vxAtomic32Or ();
IMPORT int vxAtomic32Set ();
IMPORT int vxAtomic32Sub ();
IMPORT int vxAtomic32Xor ();
IMPORT int vxAtomic64Add ();
IMPORT int vxAtomic64And ();
IMPORT int vxAtomic64Cas ();
IMPORT int vxAtomic64Clear ();
IMPORT int vxAtomic64Dec ();
IMPORT int vxAtomic64Get ();
IMPORT int vxAtomic64Inc ();
IMPORT int vxAtomic64Nand ();
IMPORT int vxAtomic64Or ();
IMPORT int vxAtomic64Set ();
IMPORT int vxAtomic64Sub ();
IMPORT int vxAtomic64Xor ();
IMPORT int vxAtomicAdd ();
IMPORT int vxAtomicAnd ();
IMPORT int vxAtomicCas ();
IMPORT int vxAtomicClear ();
IMPORT int vxAtomicDec ();
IMPORT int vxAtomicGet ();
IMPORT int vxAtomicInc ();
IMPORT int vxAtomicNand ();
IMPORT int vxAtomicOr ();
IMPORT int vxAtomicSet ();
IMPORT int vxAtomicSub ();
IMPORT int vxAtomicXor ();
IMPORT int vxBaseCpuPhysIndex;
IMPORT int vxBucsrGet ();
IMPORT int vxBusIncluded;
IMPORT int vxC1StateSet ();
IMPORT int vxCas ();
IMPORT int vxCpuConfigured;
IMPORT int vxCpuConfiguredGet ();
IMPORT int vxCpuEnabled;
IMPORT int vxCpuEnabledGet ();
IMPORT int vxCpuIdGet ();
IMPORT int vxCpuIdToPhysIndex ();
IMPORT int vxCpuIndexGet ();
IMPORT int vxCpuLibInit ();
IMPORT int vxCpuPhysIndexGet ();
IMPORT int vxCpuPhysIndexToId ();
IMPORT int vxCpuReserve ();
IMPORT int vxCpuReservedGet ();
IMPORT int vxCpuSetReserved;
IMPORT int vxCpuSetReservedAvail;
IMPORT int vxCpuUnreserve ();
IMPORT int vxCsrr0Get ();
IMPORT int vxCsrr0Set ();
IMPORT int vxCsrr1Get ();
IMPORT int vxCsrr1Set ();
IMPORT int vxDac1Get ();
IMPORT int vxDac1Set ();
IMPORT int vxDac2Get ();
IMPORT int vxDac2Set ();
IMPORT int vxDbcr0Get ();
IMPORT int vxDbcr0Set ();
IMPORT int vxDbcr1Get ();
IMPORT int vxDbcr1Set ();
IMPORT int vxDbcr2Get ();
IMPORT int vxDbcr2Set ();
IMPORT int vxDbsrClear ();
IMPORT int vxDbsrGet ();
IMPORT int vxDearGet ();
IMPORT int vxDearSet ();
IMPORT int vxDecGet ();
IMPORT int vxDecIntAck ();
IMPORT int vxDecIntEnable ();
IMPORT int vxDecSet ();
IMPORT int vxDecarSet ();
IMPORT int vxDisableBP ();
IMPORT int vxEieio ();
IMPORT int vxEnableBP ();
IMPORT int vxEventPendQ;
IMPORT int vxFirstBit ();
IMPORT int vxFirstRBit ();
IMPORT int vxFitIntAck ();
IMPORT int vxFitIntDisable ();
IMPORT int vxFitIntEnable ();
IMPORT int vxFpscrGet ();
IMPORT int vxFpscrSet ();
IMPORT int vxHid0Get ();
IMPORT int vxHid0Set ();
IMPORT int vxIOArchProbe ();
IMPORT int vxIac1Get ();
IMPORT int vxIac1Set ();
IMPORT int vxIac2Get ();
IMPORT int vxIac2Set ();
IMPORT int vxIntStackBase;
IMPORT int vxIntStackEnd;
IMPORT int vxIntStackOverflowSize;
IMPORT int vxIntStackUnderflowSize;
IMPORT int vxIpiControlGet_desc;
IMPORT int vxIvor1Set ();
IMPORT int vxIvprGet ();
IMPORT int vxIvprSet ();
IMPORT int vxL1CFG0Get ();
IMPORT int vxL1CFG1Get ();
IMPORT int vxL1CSR0Get ();
IMPORT int vxL1CSR0Set ();
IMPORT int vxL1CSR1Get ();
IMPORT int vxL1CSR1Set ();
IMPORT int vxL1CSR2Get ();
IMPORT int vxL1CSR2Set ();
IMPORT int vxL2CFG0Get ();
IMPORT int vxL2CSR0Get ();
IMPORT int vxL2CSR0Set ();
IMPORT int vxL2ERRCTLGet ();
IMPORT int vxL2ERRCTLSet ();
IMPORT int vxL2ERRDETGet ();
IMPORT int vxL2ERRDETSet ();
IMPORT int vxL2ERRDISGet ();
IMPORT int vxL2ERRDISSet ();
IMPORT int vxL2ERRINJCTLGet ();
IMPORT int vxL2ERRINJCTLSet ();
IMPORT int vxL2ERRINTENGet ();
IMPORT int vxL2ERRINTENSet ();
IMPORT int vxMcsrr0Get ();
IMPORT int vxMcsrr0Set ();
IMPORT int vxMcsrr1Get ();
IMPORT int vxMcsrr1Set ();
IMPORT int vxMemArchProbe ();
IMPORT int vxMemArchProbeInit ();
IMPORT int vxMemProbe ();
IMPORT int vxMemProbeInit ();
IMPORT int vxMemProbeSup ();
IMPORT int vxMsrGet ();
IMPORT int vxMsrSet ();
IMPORT int vxPirGet ();
IMPORT int vxPirSet ();
IMPORT int vxPpcDecTimerMaxCnt;
IMPORT int vxPpcIntMask;
IMPORT int vxPvrGet ();
IMPORT int vxSdaInit ();
IMPORT int vxSdaSet ();
IMPORT int vxSrr0Get ();
IMPORT int vxSrr0Set ();
IMPORT int vxSrr1Get ();
IMPORT int vxSrr1Set ();
IMPORT int vxSvrGet ();
IMPORT int vxTas ();
IMPORT int vxTaskEntry ();
IMPORT int vxTcrGet ();
IMPORT int vxTcrSet ();
IMPORT int vxTimeBaseGet ();
IMPORT int vxTimeBaseSet ();
IMPORT int vxTsrGet ();
IMPORT int vxTsrSet ();
IMPORT char * vxWorksVersion;
IMPORT const unsigned int vxWorksVersionMaint;
IMPORT const unsigned int vxWorksVersionMajor;
IMPORT const unsigned int vxWorksVersionMinor;
IMPORT int vxWorksVersionSvcPk;
IMPORT int vxbBusAnnounce ();
IMPORT int vxbBusListLock;
IMPORT int vxbBusTypeRegister ();
IMPORT int vxbBusTypeString ();
IMPORT int vxbBusTypeUnregister ();
IMPORT int vxbDelay ();
IMPORT int vxbDelayLibInit ();
IMPORT int vxbDelayTimerFreeRunning;
IMPORT int vxbDevConnect ();
IMPORT int vxbDevConnectInternal ();
IMPORT int vxbDevInit ();
IMPORT int vxbDevInitInternal ();
IMPORT int vxbDevIntCapabCheck_desc;
IMPORT int vxbDevIterate ();
IMPORT int vxbDevMethodGet ();
IMPORT int vxbDevMethodRun ();
IMPORT int vxbDevParent ();
IMPORT int vxbDevPath ();
IMPORT int vxbDevRegMap_desc;
IMPORT int vxbDevRegister ();
IMPORT int vxbDevRemovalAnnounce ();
IMPORT int vxbDevStructAlloc ();
IMPORT int vxbDevStructFree ();
IMPORT int vxbDevStructListLock;
IMPORT int vxbDeviceAnnounce ();
IMPORT int vxbDeviceDriverRelease ();
IMPORT int vxbDeviceMethodRun ();
IMPORT int vxbDmaBufArchFlush;
IMPORT int vxbDmaBufArchInvalidate;
IMPORT int vxbDmaBufBspAlloc;
IMPORT int vxbDmaBufBspFree;
IMPORT int vxbDmaBufFlush ();
IMPORT int vxbDmaBufFragFill ();
IMPORT int vxbDmaBufInit ();
IMPORT int vxbDmaBufInvalidate ();
IMPORT int vxbDmaBufMapArchFlush;
IMPORT int vxbDmaBufMapArchInvalidate;
IMPORT int vxbDmaBufMapCreate ();
IMPORT int vxbDmaBufMapCreate_desc;
IMPORT int vxbDmaBufMapDestroy ();
IMPORT int vxbDmaBufMapDestroy_desc;
IMPORT int vxbDmaBufMapFlush ();
IMPORT int vxbDmaBufMapInvalidate ();
IMPORT int vxbDmaBufMapIoVecLoad ();
IMPORT int vxbDmaBufMapLoad ();
IMPORT int vxbDmaBufMapMblkLoad ();
IMPORT int vxbDmaBufMapMemAlloc_desc;
IMPORT int vxbDmaBufMapMemFree_desc;
IMPORT int vxbDmaBufMapSync ();
IMPORT int vxbDmaBufMapUnload ();
IMPORT int vxbDmaBufMemAlloc ();
IMPORT int vxbDmaBufMemFree ();
IMPORT int vxbDmaBufSync ();
IMPORT int vxbDmaBufTagCreate ();
IMPORT int vxbDmaBufTagDestroy ();
IMPORT int vxbDmaBufTagGet_desc;
IMPORT int vxbDmaBufTagParentGet ();
IMPORT int vxbDmaChanAlloc ();
IMPORT int vxbDmaChanFree ();
IMPORT int vxbDmaLibInit ();
IMPORT int vxbDmaResDedicatedGet_desc;
IMPORT int vxbDmaResourceGet_desc;
IMPORT int vxbDmaResourceRelease_desc;
IMPORT int vxbDriverUnregister ();
IMPORT int vxbDrvRescan ();
IMPORT int vxbDrvUnlink_desc;
IMPORT int vxbDrvVerCheck ();
IMPORT int vxbDtsecEndRegister ();
IMPORT int vxbEpicEPRGet;
IMPORT int vxbEpicIntAckReg;
IMPORT int vxbEpicIntCtlrRegister ();
IMPORT int vxbEpicIpiDpatchReg0;
IMPORT int vxbEpicIpiGen ();
IMPORT int vxbEpicSharedMsgHandler ();
IMPORT int vxbEpicSpuriousVector;
IMPORT int vxbGlobalListsLock;
IMPORT int vxbInit ();
IMPORT int vxbInstByNameFind ();
IMPORT int vxbInstParamByIndexGet ();
IMPORT int vxbInstParamByNameGet ();
IMPORT int vxbInstParamSet ();
IMPORT int vxbInstUnitGet ();
IMPORT int vxbInstUnitSet ();
IMPORT int vxbInstUsrOptionGet ();
IMPORT int vxbIntAcknowledge ();
IMPORT int vxbIntAlloc ();
IMPORT int vxbIntConnect ();
IMPORT int vxbIntCtlrAck_desc;
IMPORT int vxbIntCtlrAlloc_desc;
IMPORT int vxbIntCtlrConnect_desc;
IMPORT int vxbIntCtlrCpuAvail_desc;
IMPORT int vxbIntCtlrCpuDisable_desc;
IMPORT int vxbIntCtlrDisable_desc;
IMPORT int vxbIntCtlrDisconnect_desc;
IMPORT int vxbIntCtlrEnable_desc;
IMPORT int vxbIntCtlrFree_desc;
IMPORT int vxbIntDisable ();
IMPORT int vxbIntDisconnect ();
IMPORT int vxbIntDynaConnect ();
IMPORT int vxbIntDynaCtlrInit ();
IMPORT int vxbIntDynaVecAlloc ();
IMPORT int vxbIntDynaVecAlloc_desc;
IMPORT int vxbIntDynaVecConnect_desc;
IMPORT int vxbIntDynaVecDevMultiProgram ();
IMPORT int vxbIntDynaVecDevMultiProgram_desc;
IMPORT int vxbIntDynaVecDisable_desc;
IMPORT int vxbIntDynaVecEnable_desc;
IMPORT int vxbIntDynaVecErase ();
IMPORT int vxbIntDynaVecErase_desc;
IMPORT int vxbIntDynaVecGet_desc;
IMPORT int vxbIntDynaVecMultiConnect ();
IMPORT int vxbIntDynaVecMultiConnect_desc;
IMPORT int vxbIntDynaVecOwnerFind ();
IMPORT int vxbIntDynaVecOwnerFind_desc;
IMPORT int vxbIntDynaVecProgram_desc;
IMPORT int vxbIntEnable ();
IMPORT int vxbIntFree ();
IMPORT int vxbIntVectorGet ();
IMPORT int vxbLibError ();
IMPORT int vxbLibInit ();
IMPORT int vxbLockDelete ();
IMPORT int vxbLockGive ();
IMPORT int vxbLockInit ();
IMPORT int vxbLockTake ();
IMPORT int vxbM85xxTimerLock;
IMPORT int vxbMsDelay ();
IMPORT int vxbMsiConnect ();
IMPORT int vxbNextUnitGet ();
IMPORT int vxbPpcIntCtlrISR_CRTL ();
IMPORT int vxbPpcIntCtlrISR_FIT ();
IMPORT int vxbPpcIntCtlrISR_INTR ();
IMPORT int vxbPpcIntCtlrISR_SYSTIMER ();
IMPORT int vxbPpcIntCtlrISR_WD ();
IMPORT int vxbRead16 ();
IMPORT int vxbRead32 ();
IMPORT int vxbRead64 ();
IMPORT int vxbRead8 ();
IMPORT int vxbRegMap ();
IMPORT int vxbRegUnmap ();
IMPORT int vxbResourceFind ();
IMPORT int vxbSerialChanGet ();
IMPORT int vxbSpecialRead16 ();
IMPORT int vxbSpecialRead32 ();
IMPORT int vxbSpecialRead8 ();
IMPORT int vxbSpecialWrite16 ();
IMPORT int vxbSpecialWrite32 ();
IMPORT int vxbSpecialWrite8 ();
IMPORT int vxbSubDevAction ();
IMPORT int vxbSwap16 ();
IMPORT int vxbSwap32 ();
IMPORT int vxbSwap64 ();
IMPORT int vxbSysClkConnect ();
IMPORT int vxbSysClkDisable ();
IMPORT int vxbSysClkEnable ();
IMPORT int vxbSysClkLibInit ();
IMPORT int vxbSysClkRateGet ();
IMPORT int vxbSysClkRateSet ();
IMPORT int vxbSysEpicIntHandler ();
IMPORT int vxbTimerAlloc ();
IMPORT int vxbTimerFeaturesGet ();
IMPORT int vxbTimerFuncGet_desc;
IMPORT int vxbTimerNumberGet_desc;
IMPORT int vxbTimerRelease ();
IMPORT int vxbUsDelay ();
IMPORT int vxbWrite16 ();
IMPORT int vxbWrite32 ();
IMPORT int vxbWrite64 ();
IMPORT int vxbWrite8 ();
IMPORT int vxdbgBpAdd ();
IMPORT int vxdbgBpAddrCheck ();
IMPORT int vxdbgBpAllDisable ();
IMPORT int vxdbgBpDelete ();
IMPORT int vxdbgBpFind ();
IMPORT int vxdbgBpList;
IMPORT int vxdbgBpMsgDrop ();
IMPORT int vxdbgBpMsgGet ();
IMPORT int vxdbgBpMsgPost ();
IMPORT int vxdbgBpMsgQCreate ();
IMPORT int vxdbgBpMsgQHdlrIsRunning ();
IMPORT int vxdbgClntRegister ();
IMPORT int vxdbgClntUnregister ();
IMPORT int vxdbgCont ();
IMPORT int vxdbgEnable ();
IMPORT int vxdbgEventIdAlloc ();
IMPORT int vxdbgEventIdFree ();
IMPORT int vxdbgEventInject ();
IMPORT int vxdbgEventLibInit ();
IMPORT int vxdbgHookAdd ();
IMPORT int vxdbgHookInvoke ();
IMPORT int vxdbgHooksInit ();
IMPORT int vxdbgInstrCmp ();
IMPORT int vxdbgInternalEventInject ();
IMPORT int vxdbgLibInit ();
IMPORT int vxdbgListDelete ();
IMPORT int vxdbgListInsert ();
IMPORT int vxdbgLockUnbreakable;
IMPORT int vxdbgMemAccess ();
IMPORT int vxdbgMemCtxSwitch ();
IMPORT int vxdbgMemMove ();
IMPORT int vxdbgMemRead ();
IMPORT int vxdbgMemScan ();
IMPORT int vxdbgMemWrite ();
IMPORT int vxdbgRunCtrlHookInvoke ();
IMPORT int vxdbgRunCtrlLibInit ();
IMPORT int vxdbgSafeUnbreakable;
IMPORT int vxdbgStep ();
IMPORT int vxdbgStop ();
IMPORT int vxdbgTaskCont ();
IMPORT int vxdbgTaskHookInit ();
IMPORT int vxdbgTaskLibInit ();
IMPORT int vxdbgTaskStep ();
IMPORT int vxdbgTaskStop ();
IMPORT int vxdbgUnbreakableOld;
IMPORT int vxfs_ramdisk_init_hook;
IMPORT int vxmux_mux_mblk_init ();
IMPORT int vxmux_null_buf_init ();
IMPORT int vxmux_null_pool;
IMPORT int w ();
IMPORT int wcstombs ();
IMPORT int wctomb ();
IMPORT int wdCancel ();
IMPORT int wdClassId;
IMPORT int wdCreate ();
IMPORT int wdCreateLibInit ();
IMPORT int wdDelete ();
IMPORT int wdDestroy ();
IMPORT int wdInit ();
IMPORT int wdInitialize ();
IMPORT int wdLibInit ();
IMPORT int wdShow ();
IMPORT int wdShowInit ();
IMPORT int wdStart ();
IMPORT int wdTerminate ();
IMPORT int wdTick ();
IMPORT int wdbDbgArchInit ();
IMPORT int wdbDbgDac1Get ();
IMPORT int wdbDbgDac1Set ();
IMPORT int wdbDbgDac2Get ();
IMPORT int wdbDbgDac2Set ();
IMPORT int wdbDbgDbcr0Get ();
IMPORT int wdbDbgDbcr0Set ();
IMPORT int wdbDbgDbcr1Get ();
IMPORT int wdbDbgDbcr1Set ();
IMPORT int wdbDbgDbcr2Get ();
IMPORT int wdbDbgDbcr2Set ();
IMPORT int wdbDbgDbsrGet ();
IMPORT int wdbDbgDbsrSet ();
IMPORT int wdbDbgGetNpc ();
IMPORT int wdbDbgHwAddrCheck ();
IMPORT int wdbDbgHwBpFind ();
IMPORT int wdbDbgHwBpHandle ();
IMPORT int wdbDbgHwBpSet ();
IMPORT int wdbDbgHwBpStub ();
IMPORT int wdbDbgIac1Get ();
IMPORT int wdbDbgIac1Set ();
IMPORT int wdbDbgIac2Get ();
IMPORT int wdbDbgIac2Set ();
IMPORT int wdbDbgRegsClear ();
IMPORT int wdbDbgRegsSet ();
IMPORT int wdbDbgSysCallCheck ();
IMPORT int wdbDbgTraceModeClear ();
IMPORT int wdbDbgTraceModeSet ();
IMPORT int wdbDbgTrap ();
IMPORT int whoami ();
IMPORT int windCont ();
IMPORT int windDelay ();
IMPORT int windDelete ();
IMPORT int windExit ();
IMPORT int windIntStackSet ();
IMPORT int windLoadContext ();
IMPORT int windNullReturn ();
IMPORT int windPendQFlush ();
IMPORT int windPendQGet ();
IMPORT int windPendQPut ();
IMPORT int windPendQRemove ();
IMPORT int windPendQTerminate ();
IMPORT int windPpcLoadRegSet ();
IMPORT int windPpcLoadVolatileRegSet ();
IMPORT int windPriNormalSet ();
IMPORT int windPrioritySet ();
IMPORT int windReadyQPut ();
IMPORT int windReadyQRemove ();
IMPORT int windResume ();
IMPORT int windSemDelete ();
IMPORT int windSemRWDelete ();
IMPORT int windStop ();
IMPORT int windStopClear ();
IMPORT int windSuspend ();
IMPORT int windTickAnnounce ();
IMPORT int windTickAnnounceN ();
IMPORT int windTickWaitGet ();
IMPORT int windUndelay ();
IMPORT int windWdCancel ();
IMPORT int windWdStart ();
IMPORT int workQAdd0 ();
IMPORT int workQAdd1 ();
IMPORT int workQAdd2 ();
IMPORT int workQDoWork ();
IMPORT int workQInit ();
IMPORT int workQIsEmpty;
IMPORT int workQIx;
IMPORT int workQPanic ();
IMPORT int workQPanicHook;
IMPORT int workQWorkInProgress;
IMPORT int write ();
IMPORT int writev ();
IMPORT int xattrib ();
IMPORT int xcopy ();
IMPORT int xdelete ();
IMPORT int zbufCreate ();
IMPORT int zbufCut ();
IMPORT int zbufDelete ();
IMPORT int zbufDup ();
IMPORT int zbufExtractCopy ();
IMPORT int zbufInsert ();
IMPORT int zbufInsertBuf ();
IMPORT int zbufInsertBuf64 ();
IMPORT int zbufInsertCopy ();
IMPORT int zbufLength ();
IMPORT int zbufLibInit ();
IMPORT int zbufSegData ();
IMPORT int zbufSegFind ();
IMPORT int zbufSegLength ();
IMPORT int zbufSegNext ();
IMPORT int zbufSegPrev ();
IMPORT int zbufSockBufSend ();
IMPORT int zbufSockBufSend64 ();
IMPORT int zbufSockBufSendto ();
IMPORT int zbufSockBufSendto64 ();
IMPORT int zbufSockLibInit ();
IMPORT int zbufSockRecv ();
IMPORT int zbufSockRecvfrom ();
IMPORT int zbufSockSend ();
IMPORT int zbufSockSendto ();
IMPORT int zbufSplit ();
IMPORT int zbuf_free ();

SYMBOL standTbl [4888] =
    {
        {{NULL}, "MD5_Final", (char*) MD5_Final, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "MD5_Init", (char*) MD5_Init, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "MD5_Transform", (char*) MD5_Transform, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "MD5_Update", (char*) MD5_Update, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "MD5_version", (char*) &MD5_version, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "QorIQLawAlloc_desc", (char*) &QorIQLawAlloc_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "QorIQLawFind_desc", (char*) &QorIQLawFind_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "QorIQLawFree_desc", (char*) &QorIQLawFree_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "QorIQdcbf", (char*) QorIQdcbf, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "QorIQdcbi", (char*) QorIQdcbi, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "QorIQdcbt", (char*) QorIQdcbt, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "QorIQdcbzl", (char*) QorIQdcbzl, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "QorIQlwsync", (char*) QorIQlwsync, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "SHA1_Final", (char*) SHA1_Final, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "SHA1_Init", (char*) SHA1_Init, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "SHA1_Transform", (char*) SHA1_Transform, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "SHA1_Update", (char*) SHA1_Update, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "SHA1_version", (char*) &SHA1_version, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "SetNewSeedtoRandomGenerator", (char*) SetNewSeedtoRandomGenerator, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ZBUF_GETSEG", (char*) ZBUF_GETSEG, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_Randseed", (char*) &_Randseed, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "___x_diab_sfpddtoll_o", (char*) &___x_diab_sfpddtoll_o, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "___x_diab_sfpdlltod_o", (char*) &___x_diab_sfpdlltod_o, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "___x_diab_sfpdnorm_o", (char*) &___x_diab_sfpdnorm_o, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "___x_diab_sxpasr64_o", (char*) &___x_diab_sxpasr64_o, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "___x_diab_sxpdiv64_o", (char*) &___x_diab_sxpdiv64_o, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "___x_diab_sxplsl64_o", (char*) &___x_diab_sxplsl64_o, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "___x_diab_sxplsr64_o", (char*) &___x_diab_sxplsr64_o, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "___x_diab_sxprem64_o", (char*) &___x_diab_sxprem64_o, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "___x_diab_va_arg_o", (char*) &___x_diab_va_arg_o, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "__asr64", (char*) __asr64, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "__assert", (char*) __assert, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "__clocale", (char*) &__clocale, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "__costate", (char*) &__costate, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "__cplusLoadObjFiles", (char*) &__cplusLoadObjFiles, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "__cplusLoad_o", (char*) &__cplusLoad_o, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "__cplusUsr_o", (char*) &__cplusUsr_o, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "__cplusXtors_o", (char*) &__cplusXtors_o, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "__ctype", (char*) &__ctype, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "__ctype_tolower", (char*) &__ctype_tolower, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "__ctype_toupper", (char*) &__ctype_toupper, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "__daysSinceEpoch", (char*) __daysSinceEpoch, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "__div64", (char*) __div64, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "__dtoll", (char*) __dtoll, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "__errno", (char*) __errno, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "__getDstInfo", (char*) __getDstInfo, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "__getDstInfoSub", (char*) __getDstInfoSub, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "__getTime", (char*) __getTime, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "__getZoneInfo", (char*) __getZoneInfo, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "__julday", (char*) __julday, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "__lltod", (char*) __lltod, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "__locale", (char*) &__locale, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "__loctime", (char*) &__loctime, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "__lsl64", (char*) __lsl64, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "__lsr64", (char*) __lsr64, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "__netlinkref_uipc_mbufutil1", (char*) &__netlinkref_uipc_mbufutil1, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "__rem64", (char*) __rem64, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "__rta_longjmp", (char*) &__rta_longjmp, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "__saverest_o", (char*) &__saverest_o, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "__sclose", (char*) __sclose, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "__sflags", (char*) __sflags, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "__sflush", (char*) __sflush, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "__sfvwrite", (char*) __sfvwrite, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "__smakebuf", (char*) __smakebuf, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "__sread", (char*) __sread, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "__srefill", (char*) __srefill, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "__srget", (char*) __srget, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "__sseek", (char*) __sseek, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "__stderr", (char*) __stderr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "__stdin", (char*) __stdin, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "__stdout", (char*) __stdout, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "__strxfrm", (char*) __strxfrm, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "__swbuf", (char*) __swbuf, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "__swrite", (char*) __swrite, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "__swsetup", (char*) __swsetup, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "__taskVarAdd", (char*) __taskVarAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "__taskVarDelete", (char*) __taskVarDelete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "__taskVarGet", (char*) __taskVarGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "__taskVarSet", (char*) __taskVarSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "__udiv64", (char*) __udiv64, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "__urem64", (char*) __urem64, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "__va_arg", (char*) __va_arg, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_applLog", (char*) _applLog, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_archHelp_msg", (char*) &_archHelp_msg, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_archIORead16", (char*) _archIORead16, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_archIORead32", (char*) _archIORead32, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_archIORead8", (char*) _archIORead8, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_archIOWrite16", (char*) _archIOWrite16, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_archIOWrite32", (char*) _archIOWrite32, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_archIOWrite8", (char*) _archIOWrite8, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_binary_fsl_fman_ucode_P3_P4_P5_101_8_bin_end", (char*) &_binary_fsl_fman_ucode_P3_P4_P5_101_8_bin_end, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_binary_fsl_fman_ucode_P3_P4_P5_101_8_bin_start", (char*) &_binary_fsl_fman_ucode_P3_P4_P5_101_8_bin_start, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_binary_fsl_fman_ucode_P4080_106_2_4_bin_end", (char*) &_binary_fsl_fman_ucode_P4080_106_2_4_bin_end, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_binary_fsl_fman_ucode_P4080_106_2_4_bin_start", (char*) &_binary_fsl_fman_ucode_P4080_106_2_4_bin_start, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_binary_fsl_fman_ucode_p4080_92_2_bin_end", (char*) &_binary_fsl_fman_ucode_p4080_92_2_bin_end, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_binary_fsl_fman_ucode_p4080_92_2_bin_start", (char*) &_binary_fsl_fman_ucode_p4080_92_2_bin_start, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_clockRealtime", (char*) &_clockRealtime, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_d_dtoll", (char*) _d_dtoll, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_d_lltod", (char*) _d_lltod, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_dbgArchInit", (char*) _dbgArchInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_dbgBrkDisplayHard", (char*) _dbgBrkDisplayHard, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_dbgDsmInstRtn", (char*) &_dbgDsmInstRtn, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_dbgFuncCallCheck", (char*) _dbgFuncCallCheck, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_dbgInfoPCGet", (char*) _dbgInfoPCGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_dbgInstSizeGet", (char*) _dbgInstSizeGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_dbgRetAdrsGet", (char*) _dbgRetAdrsGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_dbgTaskPCGet", (char*) _dbgTaskPCGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_dbgTaskPCSet", (char*) _dbgTaskPCSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_endM2Packet_1213", (char*) _endM2Packet_1213, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_endM2Packet_2233", (char*) _endM2Packet_2233, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_func_aimMmuLockRegionsImport", (char*) &_func_aimMmuLockRegionsImport, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_cacheArchDisableFromMmu", (char*) &_func_cacheArchDisableFromMmu, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_cacheArchEnable", (char*) &_func_cacheArchEnable, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_cacheFuncsSet", (char*) &_func_cacheFuncsSet, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_classListAdd", (char*) &_func_classListAdd, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_classListInit", (char*) &_func_classListInit, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_classListLock", (char*) &_func_classListLock, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_classListRemove", (char*) &_func_classListRemove, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_classListTblInit", (char*) &_func_classListTblInit, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_classListUnlock", (char*) &_func_classListUnlock, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_consoleOut", (char*) &_func_consoleOut, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_coprocShow", (char*) &_func_coprocShow, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_coprocTaskRegsShow", (char*) &_func_coprocTaskRegsShow, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_cpuPwrIdleEnter", (char*) &_func_cpuPwrIdleEnter, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_cpuPwrIntEnterHook", (char*) &_func_cpuPwrIntEnterHook, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_cpuPwrIntExitHook", (char*) &_func_cpuPwrIntExitHook, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_cpuPwrLoadContext", (char*) &_func_cpuPwrLoadContext, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_cpuPwrPriorityRaise", (char*) &_func_cpuPwrPriorityRaise, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_dosFsFmtRtn", (char*) &_func_dosFsFmtRtn, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_end2VxBusConnect", (char*) &_func_end2VxBusConnect, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_end8023AddressForm", (char*) &_func_end8023AddressForm, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_endEtherAddressForm", (char*) &_func_endEtherAddressForm, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_endEtherPacketAddrGet", (char*) &_func_endEtherPacketAddrGet, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_endEtherPacketDataGet", (char*) &_func_endEtherPacketDataGet, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_envGet", (char*) &_func_envGet, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_eventReceive", (char*) &_func_eventReceive, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_eventRsrcSend", (char*) &_func_eventRsrcSend, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_eventRsrcShow", (char*) &_func_eventRsrcShow, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_eventSend", (char*) &_func_eventSend, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_eventTaskShow", (char*) &_func_eventTaskShow, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_eventTerminate", (char*) &_func_eventTerminate, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_excBaseHook", (char*) &_func_excBaseHook, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_excInfoShow", (char*) &_func_excInfoShow, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_excIntHook", (char*) &_func_excIntHook, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_excJobAdd", (char*) &_func_excJobAdd, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_excOsm", (char*) &_func_excOsm, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_excPanicHook", (char*) &_func_excPanicHook, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_excTrapRtn", (char*) &_func_excTrapRtn, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_exit", (char*) &_func_exit, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_fclose", (char*) &_func_fclose, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_fcntl_dup", (char*) &_func_fcntl_dup, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_fileDoesNotExist", (char*) &_func_fileDoesNotExist, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_fioFltFormatRtn", (char*) &_func_fioFltFormatRtn, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_fioFltScanRtn", (char*) &_func_fioFltScanRtn, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_ftpLs", (char*) &_func_ftpLs, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_ftpTransientFatal", (char*) &_func_ftpTransientFatal, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_ftruncMmanHook", (char*) &_func_ftruncMmanHook, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_handleAlloc", (char*) &_func_handleAlloc, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_handleFreeAll", (char*) &_func_handleFreeAll, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_intConnectRtn", (char*) &_func_intConnectRtn, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_intDisableRtn", (char*) &_func_intDisableRtn, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_intDisconnectRtn", (char*) &_func_intDisconnectRtn, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_intEnableRtn", (char*) &_func_intEnableRtn, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_intLevelSetRtn", (char*) &_func_intLevelSetRtn, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_intVecBaseGetRtn", (char*) &_func_intVecBaseGetRtn, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_intVecBaseSetRtn", (char*) &_func_intVecBaseSetRtn, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_intVecGetRtn", (char*) &_func_intVecGetRtn, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_intVecSetRtn", (char*) &_func_intVecSetRtn, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_ioGlobalStdGet", (char*) &_func_ioGlobalStdGet, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_ioPxCreateOrOpen", (char*) &_func_ioPxCreateOrOpen, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_ioTaskStdSet", (char*) &_func_ioTaskStdSet, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_iosPathFdEntryIoctl", (char*) &_func_iosPathFdEntryIoctl, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_iosPosixFdEntryIoctl", (char*) &_func_iosPosixFdEntryIoctl, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_iosRtpIoTableSizeGet", (char*) &_func_iosRtpIoTableSizeGet, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_iosRtpIoTableSizeSet", (char*) &_func_iosRtpIoTableSizeSet, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_isrConnect", (char*) &_func_isrConnect, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_isrCreate", (char*) &_func_isrCreate, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_isrDelete", (char*) &_func_isrDelete, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_isrDisconnect", (char*) &_func_isrDisconnect, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_isrDispatcher", (char*) &_func_isrDispatcher, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_isrInfo", (char*) &_func_isrInfo, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_jobTaskWorkAdd", (char*) &_func_jobTaskWorkAdd, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_kernelModuleListIdGet", (char*) &_func_kernelModuleListIdGet, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_kernelRoundRobinHook", (char*) &_func_kernelRoundRobinHook, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_kprintf", (char*) &_func_kprintf, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_kwrite", (char*) &_func_kwrite, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_lf_advlock", (char*) &_func_lf_advlock, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_logMsg", (char*) &_func_logMsg, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_loginPrompt2", (char*) &_func_loginPrompt2, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_loginStringGet", (char*) &_func_loginStringGet, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_loginStringSet", (char*) &_func_loginStringSet, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_m2PollStatsIfPoll", (char*) &_func_m2PollStatsIfPoll, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_memPartCacheAlloc", (char*) &_func_memPartCacheAlloc, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_memPartCacheFree", (char*) &_func_memPartCacheFree, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_memPartCacheInfoGet", (char*) &_func_memPartCacheInfoGet, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_memPartRealloc", (char*) &_func_memPartRealloc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_memPartReallocInternal", (char*) &_func_memPartReallocInternal, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_memalign", (char*) &_func_memalign, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_mmuCacheFlushLoadAdrsGet", (char*) &_func_mmuCacheFlushLoadAdrsGet, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_moduleIdFigure", (char*) &_func_moduleIdFigure, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_moduleSegInfoGet", (char*) &_func_moduleSegInfoGet, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_mux", (char*) &_func_mux, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_mux_l2", (char*) &_func_mux_l2, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_netAttrDeReg", (char*) &_func_netAttrDeReg, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_netAttrReg", (char*) &_func_netAttrReg, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_netChDirByName", (char*) &_func_netChDirByName, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_netDaemonIxToQId", (char*) &_func_netDaemonIxToQId, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_netDaemonQIdToIx", (char*) &_func_netDaemonQIdToIx, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_netLsByName", (char*) &_func_netLsByName, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_netPoolShow", (char*) &_func_netPoolShow, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_objCoreRelease", (char*) &_func_objCoreRelease, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_objHandleToObjId", (char*) &_func_objHandleToObjId, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_objNameVerifiedSet", (char*) &_func_objNameVerifiedSet, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_objOwnerListAdd", (char*) &_func_objOwnerListAdd, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_objOwnerListRemove", (char*) &_func_objOwnerListRemove, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_objOwnerReclaim", (char*) &_func_objOwnerReclaim, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_objOwnerSetBaseInternal", (char*) &_func_objOwnerSetBaseInternal, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_objOwnerSetInternal", (char*) &_func_objOwnerSetInternal, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_objRtpObjEachHook", (char*) &_func_objRtpObjEachHook, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_ownerListLock", (char*) &_func_ownerListLock, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_ownerListUnlock", (char*) &_func_ownerListUnlock, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_printErr", (char*) &_func_printErr, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_printExcPrintHook", (char*) &_func_printExcPrintHook, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_pthread_testandset_canceltype", (char*) &_func_pthread_testandset_canceltype, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_qPriBMapNativeResort", (char*) &_func_qPriBMapNativeResort, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_remCurIdGet", (char*) &_func_remCurIdGet, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_remCurIdSet", (char*) &_func_remCurIdSet, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_rtpDelete", (char*) &_func_rtpDelete, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_rtpDeleteHookAdd", (char*) &_func_rtpDeleteHookAdd, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_rtpDeleteHookDelete", (char*) &_func_rtpDeleteHookDelete, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_rtpHandleIdGet", (char*) &_func_rtpHandleIdGet, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_rtpIdVerify", (char*) &_func_rtpIdVerify, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_rtpInitCompleteHookAdd", (char*) &_func_rtpInitCompleteHookAdd, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_rtpInitCompleteHookDelete", (char*) &_func_rtpInitCompleteHookDelete, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_rtpPause", (char*) &_func_rtpPause, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_rtpPostCreateHookAdd", (char*) &_func_rtpPostCreateHookAdd, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_rtpPostCreateHookDelete", (char*) &_func_rtpPostCreateHookDelete, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_rtpPreCreateHookAdd", (char*) &_func_rtpPreCreateHookAdd, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_rtpPreCreateHookDelete", (char*) &_func_rtpPreCreateHookDelete, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_rtpSdUnmapAll", (char*) &_func_rtpSdUnmapAll, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_rtpShlDeleteAll", (char*) &_func_rtpShlDeleteAll, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_rtpShlInfoGet", (char*) &_func_rtpShlInfoGet, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_rtpShlSymbolsRegister", (char*) &_func_rtpShlSymbolsRegister, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_rtpShlSymbolsUnregister", (char*) &_func_rtpShlSymbolsUnregister, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_rtpShow", (char*) &_func_rtpShow, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_rtpSigExcKill", (char*) &_func_rtpSigExcKill, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_rtpSigPendDestroy", (char*) &_func_rtpSigPendDestroy, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_rtpSigPendInit", (char*) &_func_rtpSigPendInit, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_rtpSigPendKill", (char*) &_func_rtpSigPendKill, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_rtpSigaction", (char*) &_func_rtpSigaction, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_rtpSigpending", (char*) &_func_rtpSigpending, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_rtpSigprocmask", (char*) &_func_rtpSigprocmask, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_rtpSigqueue", (char*) &_func_rtpSigqueue, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_rtpSigsuspend", (char*) &_func_rtpSigsuspend, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_rtpSigtimedwait", (char*) &_func_rtpSigtimedwait, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_rtpSpawn", (char*) &_func_rtpSpawn, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_rtpSymTblIdGet", (char*) &_func_rtpSymTblIdGet, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_rtpTaskKill", (char*) &_func_rtpTaskKill, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_rtpTaskSigBlock", (char*) &_func_rtpTaskSigBlock, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_rtpTaskSigCheck", (char*) &_func_rtpTaskSigCheck, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_rtpTaskSigPendKill", (char*) &_func_rtpTaskSigPendKill, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_rtpTaskSigqueue", (char*) &_func_rtpTaskSigqueue, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_rtpVarAdd", (char*) &_func_rtpVarAdd, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_rtpVerifiedLock", (char*) &_func_rtpVerifiedLock, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_selPtyAdd", (char*) &_func_selPtyAdd, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_selPtyDelete", (char*) &_func_selPtyDelete, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_selPtySlaveAdd", (char*) &_func_selPtySlaveAdd, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_selPtySlaveDelete", (char*) &_func_selPtySlaveDelete, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_selTyAdd", (char*) &_func_selTyAdd, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_selTyDelete", (char*) &_func_selTyDelete, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_selWakeup", (char*) &_func_selWakeup, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_selWakeupAll", (char*) &_func_selWakeupAll, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_selWakeupListInit", (char*) &_func_selWakeupListInit, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_selWakeupListTerm", (char*) &_func_selWakeupListTerm, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_semExchange", (char*) &_func_semExchange, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_semRTake", (char*) &_func_semRTake, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_semWTake", (char*) &_func_semWTake, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_shellDbgCallPrint", (char*) &_func_shellDbgCallPrint, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_shellDbgStackTrace", (char*) &_func_shellDbgStackTrace, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_shellExcPrint", (char*) &_func_shellExcPrint, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_sigExcKill", (char*) &_func_sigExcKill, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_sigTimeoutRecalc", (char*) &_func_sigTimeoutRecalc, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_sigeventInit", (char*) &_func_sigeventInit, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_sigfillset", (char*) &_func_sigfillset, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_signalMmanHook", (char*) &_func_signalMmanHook, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_sigprocmask", (char*) &_func_sigprocmask, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_spy", (char*) &_func_spy, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_spy2", (char*) &_func_spy2, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_spyClkStart", (char*) &_func_spyClkStart, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_spyClkStop", (char*) &_func_spyClkStop, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_spyReport", (char*) &_func_spyReport, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_spyReport2", (char*) &_func_spyReport2, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_spyStop", (char*) &_func_spyStop, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_spyTask", (char*) &_func_spyTask, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_symEach", (char*) &_func_symEach, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_symEachCall", (char*) &_func_symEachCall, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_symFind", (char*) &_func_symFind, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_symFindSymbol", (char*) &_func_symFindSymbol, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_symNameGet", (char*) &_func_symNameGet, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_symRegister", (char*) &_func_symRegister, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_symTblShutdown", (char*) &_func_symTblShutdown, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_symTypeGet", (char*) &_func_symTypeGet, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_symValueGet", (char*) &_func_symValueGet, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_sysDoze", (char*) &_func_sysDoze, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_taskCreateHookAdd", (char*) &_func_taskCreateHookAdd, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_taskDeleteHookAdd", (char*) &_func_taskDeleteHookAdd, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_taskMemCtxSwitch", (char*) &_func_taskMemCtxSwitch, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_taskStackAlloc", (char*) &_func_taskStackAlloc, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_taskStackFree", (char*) &_func_taskStackFree, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_tipStart", (char*) &_func_tipStart, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_unldByModuleId", (char*) &_func_unldByModuleId, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_valloc", (char*) &_func_valloc, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_virtExtraMapInfoGet", (char*) &_func_virtExtraMapInfoGet, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_virtSegInfoGet", (char*) &_func_virtSegInfoGet, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_vmBaseFuncsSet", (char*) &_func_vmBaseFuncsSet, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_vmInvPageMap", (char*) &_func_vmInvPageMap, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_vmInvUnMap", (char*) &_func_vmInvUnMap, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_vxEpicIpiCtrlInitIpiEmit", (char*) &_func_vxEpicIpiCtrlInitIpiEmit, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_vxMemProbeHook", (char*) &_func_vxMemProbeHook, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_vxSvrGet", (char*) &_func_vxSvrGet, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_vxbDelayCommon", (char*) &_func_vxbDelayCommon, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_vxbIntConnect", (char*) &_func_vxbIntConnect, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_vxbIntDisable", (char*) &_func_vxbIntDisable, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_vxbIntDisconnect", (char*) &_func_vxbIntDisconnect, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_vxbIntDynaConnect", (char*) &_func_vxbIntDynaConnect, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_vxbIntDynaCtlrInputInit", (char*) &_func_vxbIntDynaCtlrInputInit, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_vxbIntDynaVecAlloc", (char*) &_func_vxbIntDynaVecAlloc, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_vxbIntDynaVecDevMultiProgram", (char*) &_func_vxbIntDynaVecDevMultiProgram, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_vxbIntDynaVecDisable", (char*) &_func_vxbIntDynaVecDisable, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_vxbIntDynaVecEnable", (char*) &_func_vxbIntDynaVecEnable, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_vxbIntDynaVecErase", (char*) &_func_vxbIntDynaVecErase, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_vxbIntDynaVecGet", (char*) &_func_vxbIntDynaVecGet, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_vxbIntDynaVecOwnerFind", (char*) &_func_vxbIntDynaVecOwnerFind, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_vxbIntDynaVecProgram", (char*) &_func_vxbIntDynaVecProgram, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_vxbIntEnable", (char*) &_func_vxbIntEnable, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_vxbPciDevCfgRead", (char*) &_func_vxbPciDevCfgRead, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_vxbPciDevCfgWrite", (char*) &_func_vxbPciDevCfgWrite, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_vxbSysEpicIntHandler", (char*) &_func_vxbSysEpicIntHandler, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_vxbUserHookDevInit", (char*) &_func_vxbUserHookDevInit, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_vxdbgBpDelete", (char*) &_func_vxdbgBpDelete, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_vxdbgCtxCont", (char*) &_func_vxdbgCtxCont, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_vxdbgCtxStop", (char*) &_func_vxdbgCtxStop, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_vxdbgMemMove", (char*) &_func_vxdbgMemMove, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_vxdbgMemRead", (char*) &_func_vxdbgMemRead, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_vxdbgMemScan", (char*) &_func_vxdbgMemScan, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_vxdbgMemWrite", (char*) &_func_vxdbgMemWrite, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_vxdbgRtpCont", (char*) &_func_vxdbgRtpCont, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_vxdbgRtpIdVerify", (char*) &_func_vxdbgRtpIdVerify, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_vxdbgRtpStop", (char*) &_func_vxdbgRtpStop, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_vxdbgTrap", (char*) &_func_vxdbgTrap, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_wdbTrap", (char*) &_func_wdbTrap, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_windSemRWDelete", (char*) &_func_windSemRWDelete, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_func_windTickAnnounceHook", (char*) &_func_windTickAnnounceHook, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_func_workQDbgTextDump", (char*) &_func_workQDbgTextDump, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_hostAdd", (char*) _hostAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_hostDelete", (char*) _hostDelete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_hostGetByAddr", (char*) _hostGetByAddr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_hostGetByName", (char*) _hostGetByName, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_hostTblSearchByName2", (char*) _hostTblSearchByName2, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_ipcom_gethostbyaddr_r", (char*) &_ipcom_gethostbyaddr_r, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_ipcom_gethostbyname_r", (char*) &_ipcom_gethostbyname_r, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_ipdhcpc_callback_hook", (char*) &_ipdhcpc_callback_hook, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_linkMemReqClAlign", (char*) &_linkMemReqClAlign, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_linkMemReqMlinkAlign", (char*) &_linkMemReqMlinkAlign, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_muxProtosPerEndInc", (char*) &_muxProtosPerEndInc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_muxProtosPerEndStart", (char*) &_muxProtosPerEndStart, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_netMemReqDefault", (char*) _netMemReqDefault, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_netSysctl", (char*) &_netSysctl, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_netSysctlCli", (char*) &_netSysctlCli, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_pLinkPoolFuncTbl", (char*) &_pLinkPoolFuncTbl, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_pNetBufCollect", (char*) &_pNetBufCollect, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_pNetDpool", (char*) &_pNetDpool, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_pNetPoolFuncAlignTbl", (char*) &_pNetPoolFuncAlignTbl, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_pNetPoolFuncTbl", (char*) &_pNetPoolFuncTbl, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_pNetSysPool", (char*) &_pNetSysPool, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_pSigQueueFreeHead", (char*) &_pSigQueueFreeHead, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "_pSysL2CacheDisable", (char*) &_pSysL2CacheDisable, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_pSysL2CacheEnable", (char*) &_pSysL2CacheEnable, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_pSysL2CacheFlush", (char*) &_pSysL2CacheFlush, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_pSysL2CacheInvFunc", (char*) &_pSysL2CacheInvFunc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_pSysL3CacheFlushDisableFunc", (char*) &_pSysL3CacheFlushDisableFunc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_pSysL3CacheInvalEnableFunc", (char*) &_pSysL3CacheInvalEnableFunc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_panicHook", (char*) &_panicHook, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_ppcAllocationQuantumSize", (char*) &_ppcAllocationQuantumSize, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_ppcCacheLineSize", (char*) &_ppcCacheLineSize, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_ppcStackAlignSize", (char*) &_ppcStackAlignSize, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_procNumWasSet", (char*) &_procNumWasSet, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_restfpr_14_l", (char*) _restfpr_14_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restfpr_15_l", (char*) _restfpr_15_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restfpr_16_l", (char*) _restfpr_16_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restfpr_17_l", (char*) _restfpr_17_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restfpr_18_l", (char*) _restfpr_18_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restfpr_19_l", (char*) _restfpr_19_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restfpr_20_l", (char*) _restfpr_20_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restfpr_21_l", (char*) _restfpr_21_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restfpr_22_l", (char*) _restfpr_22_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restfpr_23_l", (char*) _restfpr_23_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restfpr_24_l", (char*) _restfpr_24_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restfpr_25_l", (char*) _restfpr_25_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restfpr_26_l", (char*) _restfpr_26_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restfpr_27_l", (char*) _restfpr_27_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restfpr_28_l", (char*) _restfpr_28_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restfpr_29_l", (char*) _restfpr_29_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restfpr_30_l", (char*) _restfpr_30_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restfpr_31_l", (char*) _restfpr_31_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restgpr_14", (char*) _restgpr_14, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restgpr_14_l", (char*) _restgpr_14_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restgpr_15", (char*) _restgpr_15, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restgpr_15_l", (char*) _restgpr_15_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restgpr_16", (char*) _restgpr_16, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restgpr_16_l", (char*) _restgpr_16_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restgpr_17", (char*) _restgpr_17, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restgpr_17_l", (char*) _restgpr_17_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restgpr_18", (char*) _restgpr_18, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restgpr_18_l", (char*) _restgpr_18_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restgpr_19", (char*) _restgpr_19, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restgpr_19_l", (char*) _restgpr_19_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restgpr_20", (char*) _restgpr_20, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restgpr_20_l", (char*) _restgpr_20_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restgpr_21", (char*) _restgpr_21, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restgpr_21_l", (char*) _restgpr_21_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restgpr_22", (char*) _restgpr_22, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restgpr_22_l", (char*) _restgpr_22_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restgpr_23", (char*) _restgpr_23, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restgpr_23_l", (char*) _restgpr_23_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restgpr_24", (char*) _restgpr_24, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restgpr_24_l", (char*) _restgpr_24_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restgpr_25", (char*) _restgpr_25, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restgpr_25_l", (char*) _restgpr_25_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restgpr_26", (char*) _restgpr_26, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restgpr_26_l", (char*) _restgpr_26_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restgpr_27", (char*) _restgpr_27, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restgpr_27_l", (char*) _restgpr_27_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restgpr_28", (char*) _restgpr_28, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restgpr_28_l", (char*) _restgpr_28_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restgpr_29", (char*) _restgpr_29, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restgpr_29_l", (char*) _restgpr_29_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restgpr_30", (char*) _restgpr_30, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restgpr_30_l", (char*) _restgpr_30_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restgpr_31", (char*) _restgpr_31, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_restgpr_31_l", (char*) _restgpr_31_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savefpr_14_l", (char*) _savefpr_14_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savefpr_15_l", (char*) _savefpr_15_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savefpr_16_l", (char*) _savefpr_16_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savefpr_17_l", (char*) _savefpr_17_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savefpr_18_l", (char*) _savefpr_18_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savefpr_19_l", (char*) _savefpr_19_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savefpr_20_l", (char*) _savefpr_20_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savefpr_21_l", (char*) _savefpr_21_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savefpr_22_l", (char*) _savefpr_22_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savefpr_23_l", (char*) _savefpr_23_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savefpr_24_l", (char*) _savefpr_24_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savefpr_25_l", (char*) _savefpr_25_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savefpr_26_l", (char*) _savefpr_26_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savefpr_27_l", (char*) _savefpr_27_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savefpr_28_l", (char*) _savefpr_28_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savefpr_29_l", (char*) _savefpr_29_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savefpr_30_l", (char*) _savefpr_30_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savefpr_31_l", (char*) _savefpr_31_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savegpr_14", (char*) _savegpr_14, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savegpr_14_l", (char*) _savegpr_14_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savegpr_15", (char*) _savegpr_15, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savegpr_15_l", (char*) _savegpr_15_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savegpr_16", (char*) _savegpr_16, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savegpr_16_l", (char*) _savegpr_16_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savegpr_17", (char*) _savegpr_17, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savegpr_17_l", (char*) _savegpr_17_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savegpr_18", (char*) _savegpr_18, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savegpr_18_l", (char*) _savegpr_18_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savegpr_19", (char*) _savegpr_19, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savegpr_19_l", (char*) _savegpr_19_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savegpr_20", (char*) _savegpr_20, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savegpr_20_l", (char*) _savegpr_20_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savegpr_21", (char*) _savegpr_21, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savegpr_21_l", (char*) _savegpr_21_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savegpr_22", (char*) _savegpr_22, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savegpr_22_l", (char*) _savegpr_22_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savegpr_23", (char*) _savegpr_23, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savegpr_23_l", (char*) _savegpr_23_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savegpr_24", (char*) _savegpr_24, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savegpr_24_l", (char*) _savegpr_24_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savegpr_25", (char*) _savegpr_25, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savegpr_25_l", (char*) _savegpr_25_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savegpr_26", (char*) _savegpr_26, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savegpr_26_l", (char*) _savegpr_26_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savegpr_27", (char*) _savegpr_27, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savegpr_27_l", (char*) _savegpr_27_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savegpr_28", (char*) _savegpr_28, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savegpr_28_l", (char*) _savegpr_28_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savegpr_29", (char*) _savegpr_29, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savegpr_29_l", (char*) _savegpr_29_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savegpr_30", (char*) _savegpr_30, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savegpr_30_l", (char*) _savegpr_30_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savegpr_31", (char*) _savegpr_31, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_savegpr_31_l", (char*) _savegpr_31_l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_setjmpSetup", (char*) _setjmpSetup, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_sigCtxLoad", (char*) _sigCtxLoad, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_sigCtxRtnValSet", (char*) _sigCtxRtnValSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_sigCtxSave", (char*) _sigCtxSave, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_sigCtxSetup", (char*) _sigCtxSetup, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_sigCtxStackEnd", (char*) _sigCtxStackEnd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_sigfaulttable", (char*) &_sigfaulttable, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_sysInit", (char*) _sysInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_taskSuspend", (char*) _taskSuspend, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_vmpByte", (char*) _vmpByte, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_vmpByteEnd", (char*) _vmpByteEnd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_vmpByteRead", (char*) _vmpByteRead, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_vmpByteWrite", (char*) _vmpByteWrite, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_vmpShort", (char*) _vmpShort, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_vmpShortEnd", (char*) _vmpShortEnd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_vmpShortRead", (char*) _vmpShortRead, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_vmpShortWrite", (char*) _vmpShortWrite, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_vmpWord", (char*) _vmpWord, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_vmpWordEnd", (char*) _vmpWordEnd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_vmpWordRead", (char*) _vmpWordRead, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_vmpWordWrite", (char*) _vmpWordWrite, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_vx_offset_COMMON_SYM_pSymbol", (char*) 0x00000004, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_COPROC_DESC_ctxAlign", (char*) 0x00000008, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_COPROC_DESC_ctxSize", (char*) 0x00000004, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_COPROC_DESC_mask", (char*) 0x00000048, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_COPROC_DESC_next", (char*) 0x00000000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_COPROC_DESC_pCtxGetRtn", (char*) 0x00000024, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_COPROC_DESC_pLastEntry", (char*) 0x00000044, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_COPROC_TBL_ENTRY_pCtx", (char*) 0x00000000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_COPROC_TBL_ENTRY_pDescriptor", (char*) 0x00000004, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_COPROC_TBL_ENTRY_pTask", (char*) 0x00000008, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_COPROC_TBL_ENTRY_size", (char*) 0x0000000c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_DEV_HDR_drvNum", (char*) 0x0000001c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_DEV_HDR_name", (char*) 0x00000008, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_DL_LIST_head", (char*) 0x00000000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_DL_NODE_next", (char*) 0x00000000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_DRV_ENTRY_de_close", (char*) 0x0000000c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_DRV_ENTRY_de_create", (char*) 0x00000000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_DRV_ENTRY_de_delete", (char*) 0x00000004, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_DRV_ENTRY_de_inuse", (char*) 0x00000020, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_DRV_ENTRY_de_ioctl", (char*) 0x00000018, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_DRV_ENTRY_de_open", (char*) 0x00000008, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_DRV_ENTRY_de_read", (char*) 0x00000010, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_DRV_ENTRY_de_write", (char*) 0x00000014, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_ESFPPC_dear", (char*) 0x0000000c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_ESFPPC_esr", (char*) 0x00000010, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_ESFPPC_iarr", (char*) 0x00000014, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_ESFPPC_regSet", (char*) 0x00000018, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_ESFPPC_vecOffset", (char*) 0x00000006, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_FD_ENTRY_pDevHdr", (char*) 0x00000044, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_FD_ENTRY_refCnt", (char*) 0x00000050, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_FD_ENTRY_value", (char*) 0x00000048, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_HANDLE_attributes", (char*) 0x00000008, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_HANDLE_context", (char*) 0x0000000c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_HANDLE_contextType", (char*) 0x0000000b, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_HANDLE_magic", (char*) 0x00000000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_HANDLE_objSize", (char*) 0x00000010, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_HANDLE_safeCnt", (char*) 0x00000004, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_HANDLE_type", (char*) 0x0000000a, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_HASH_TBL_elements", (char*) 0x00000020, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_HASH_TBL_pHashTbl", (char*) 0x00000018, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_HOOK_TBL_hookTableA", (char*) 0x0000008c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_HOOK_TBL_maxEntries", (char*) 0x00000070, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_JOB_arg1", (char*) 0x00000004, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_JOB_arg2", (char*) 0x00000008, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_JOB_function", (char*) 0x00000000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_JOB_isrTag", (char*) 0x0000000c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_KERNEL_RTP_CMN_pWdbInfo", (char*) 0x00000010, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_KERNEL_RTP_CMN_pgMgrId", (char*) 0x00000004, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_KERNEL_RTP_CMN_sharedDataList", (char*) 0x00000008, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_KERNEL_RTP_CMN_vmContextId", (char*) 0x00000000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_MEM_PART_STATS_maxBlockSizeFree", (char*) 0x00000008, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_MEM_PART_STATS_maxBytesAlloc", (char*) 0x00000014, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_MEM_PART_STATS_numBlocksAlloc", (char*) 0x00000010, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_MEM_PART_STATS_numBlocksFree", (char*) 0x00000004, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_MEM_PART_STATS_numBytesAlloc", (char*) 0x0000000c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_MEM_PART_STATS_numBytesFree", (char*) 0x00000000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_MEM_SECTION_HDR_size", (char*) 0x00000004, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_MODLIST_DESC_moduleList", (char*) 0x00000010, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_MODULE_commTotalSize", (char*) 0x00000540, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_MODULE_commonSymList", (char*) 0x00000554, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_MODULE_ctors", (char*) 0x00000538, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_MODULE_dtors", (char*) 0x0000053c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_MODULE_flags", (char*) 0x0000051c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_MODULE_format", (char*) 0x00000530, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_MODULE_group", (char*) 0x00000534, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_MODULE_nameWithPath", (char*) 0x00000018, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_MODULE_sectionList", (char*) 0x00000520, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_MODULE_segmentList", (char*) 0x00000528, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_MODULE_swapNeeded", (char*) 0x00000544, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_MODULE_undefSymCount", (char*) 0x00000548, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_MODULE_undefSymList", (char*) 0x0000054c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_MSG_NODE_message", (char*) 0x0000000c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_MSG_NODE_msgLength", (char*) 0x00000004, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_MSG_Q_HEAD_count", (char*) 0x00000008, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_MSG_Q_HEAD_list", (char*) 0x00000000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_MSG_Q_HEAD_pendQ", (char*) 0x0000000c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_MSG_Q_events", (char*) 0x0000008c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_MSG_Q_freeQ", (char*) 0x00000060, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_MSG_Q_maxMsgLength", (char*) 0x00000080, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_MSG_Q_maxMsgs", (char*) 0x0000007c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_MSG_Q_msgQ", (char*) 0x00000044, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_MSG_Q_options", (char*) 0x000000a8, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_MSG_Q_recvTimeouts", (char*) 0x00000088, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_MSG_Q_sendTimeouts", (char*) 0x00000084, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_OBJ_CLASS_createRtn", (char*) 0x0000001c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_OBJ_CLASS_destroyRtn", (char*) 0x00000020, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_OBJ_CLASS_handle", (char*) 0x00000000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_OBJ_CLASS_objClassType", (char*) 0x00000010, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_OBJ_CLASS_objMemAllocRtn", (char*) 0x00000028, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_OBJ_CLASS_objMemFreeRtn", (char*) 0x0000002c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_OBJ_CLASS_objPartId", (char*) 0x00000030, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_OBJ_CLASS_objPrivList", (char*) 0x00000034, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_OBJ_CLASS_objPubList", (char*) 0x0000003c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_OBJ_CLASS_objSize", (char*) 0x00000018, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_OBJ_CLASS_options", (char*) 0x00000014, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_OBJ_CLASS_pClassSem", (char*) 0x00000044, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_OBJ_CLASS_showRtn", (char*) 0x00000024, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_PAGE_MGR_OBJ_mmapList", (char*) 0x00000010, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_PAGE_MGR_OBJ_options", (char*) 0x00000018, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_PAGE_MGR_OBJ_physPgPoolId", (char*) 0x00000004, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_PAGE_MGR_OBJ_virtPgPoolId", (char*) 0x00000000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_PAGE_MGR_OBJ_vmContextId", (char*) 0x0000000c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_PARTITION_avlSize", (char*) 0x00000044, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_PARTITION_cumBlocksAllocated", (char*) 0x00000100, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_PARTITION_cumWordsAllocated", (char*) 0x00000108, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_PARTITION_curBlocksAllocated", (char*) 0x000000f4, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_PARTITION_curBlocksAllocatedInternal", (char*) 0x00000114, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_PARTITION_curBlocksFreed", (char*) 0x000000ec, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_PARTITION_curWordsAllocated", (char*) 0x000000f8, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_PARTITION_curWordsAllocatedInternal", (char*) 0x00000118, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_PARTITION_curWordsFreed", (char*) 0x000000f0, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_PARTITION_freeSizeNodeNb", (char*) 0x00000050, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_PARTITION_maxWordsAllocated", (char*) 0x00000110, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_PARTITION_options", (char*) 0x000000cc, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_PARTITION_sectionHdrLst", (char*) 0x000000d0, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_PARTITION_sem", (char*) 0x00000058, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_PARTITION_totalWords", (char*) 0x000000c8, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_POSIX_MSG_Q_ATTR_mq_curmsgs", (char*) 0x0000000c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_POSIX_MSG_Q_ATTR_mq_flags", (char*) 0x00000008, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_POSIX_MSG_Q_ATTR_mq_maxmsg", (char*) 0x00000000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_POSIX_MSG_Q_ATTR_mq_msgsize", (char*) 0x00000004, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_POSIX_MSG_Q_DES_f_data", (char*) 0x00000000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_POSIX_MSG_Q_DES_f_flag", (char*) 0x00000004, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_POSIX_MSG_Q_msgq_attr", (char*) 0x00000114, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_POSIX_MSG_Q_msgq_cond_data", (char*) 0x00000054, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_POSIX_MSG_Q_msgq_cond_read", (char*) 0x00000044, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_PSEMAPHORE_objCore", (char*) 0x00000000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_PSEMAPHORE_pSelf", (char*) 0x00000048, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_PSEMAPHORE_wSemId", (char*) 0x00000044, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_Q_FIFO_G_HEAD_pFifoQ", (char*) 0x00000008, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_RBUFF_BUFF_TYPE_dataEnd", (char*) 0x0000000c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_RBUFF_BUFF_TYPE_dataLen", (char*) 0x00000010, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_RBUFF_BUFF_TYPE_dataStart", (char*) 0x00000008, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_RBUFF_BUFF_TYPE_next", (char*) 0x00000000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_RBUFF_BUFF_TYPE_prev", (char*) 0x00000018, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_RBUFF_BUFF_TYPE_spaceAvail", (char*) 0x00000004, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_RBUFF_BUFF_TYPE_uncommitedRead", (char*) 0x00000014, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_RBUFF_INFO_TYPE_buffSize", (char*) 0x00000008, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_RBUFF_INFO_TYPE_bytesPeak", (char*) 0x00000040, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_RBUFF_INFO_TYPE_bytesRead", (char*) 0x0000003c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_RBUFF_INFO_TYPE_bytesWritten", (char*) 0x00000038, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_RBUFF_INFO_TYPE_currBuffs", (char*) 0x0000000c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_RBUFF_INFO_TYPE_dataContent", (char*) 0x00000024, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_RBUFF_INFO_TYPE_emptyBuffs", (char*) 0x00000020, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_RBUFF_INFO_TYPE_maxBuffs", (char*) 0x00000018, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_RBUFF_INFO_TYPE_maxBuffsActual", (char*) 0x0000001c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_RBUFF_INFO_TYPE_minBuffs", (char*) 0x00000014, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_RBUFF_INFO_TYPE_options", (char*) 0x00000004, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_RBUFF_INFO_TYPE_readsSinceReset", (char*) 0x0000002c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_RBUFF_INFO_TYPE_srcPart", (char*) 0x00000000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_RBUFF_INFO_TYPE_threshold", (char*) 0x00000010, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_RBUFF_INFO_TYPE_timesExtended", (char*) 0x00000030, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_RBUFF_INFO_TYPE_timesXThreshold", (char*) 0x00000034, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_RBUFF_INFO_TYPE_writesSinceReset", (char*) 0x00000028, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_RBUFF_TYPE_buffDesc", (char*) 0x00000000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_RBUFF_TYPE_buffRead", (char*) 0x000000b0, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_RBUFF_TYPE_buffWrite", (char*) 0x000000b4, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_RBUFF_TYPE_dataRead", (char*) 0x000000b8, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_RBUFF_TYPE_dataWrite", (char*) 0x000000bc, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_RBUFF_TYPE_info", (char*) 0x000000cc, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_REG_SET_cr", (char*) 0x00000090, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_REG_SET_msr", (char*) 0x00000080, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_REG_SET_pc", (char*) 0x0000008c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_REG_SET_size", (char*) 0x000000a4, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_REG_SET_sp", (char*) 0x00000004, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SECTION_DESC_address", (char*) 0x0000001c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SECTION_DESC_checksum", (char*) 0x00000030, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SECTION_DESC_flags", (char*) 0x00000028, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SECTION_DESC_name", (char*) 0x00000018, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SECTION_DESC_sectionNode", (char*) 0x00000010, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SECTION_DESC_size", (char*) 0x00000020, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SECTION_DESC_type", (char*) 0x00000024, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SEGMENT_address", (char*) 0x00000008, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SEGMENT_flags", (char*) 0x00000014, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SEGMENT_segmentNode", (char*) 0x00000000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SEGMENT_size", (char*) 0x0000000c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SEGMENT_type", (char*) 0x00000010, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SEMAPHORE_count", (char*) 0x0000005c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SEMAPHORE_events", (char*) 0x00000060, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SEMAPHORE_events_options", (char*) 0x00000068, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SEMAPHORE_events_registered", (char*) 0x00000064, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SEMAPHORE_options", (char*) 0x00000045, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SEMAPHORE_priInheritFlag", (char*) 0x00000048, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SEMAPHORE_qHead", (char*) 0x0000004c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SEMAPHORE_recurse", (char*) 0x00000046, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SEMAPHORE_semType", (char*) 0x00000044, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SL_LIST_head", (char*) 0x00000000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SL_LIST_size", (char*) 0x00000008, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SL_NODE_next", (char*) 0x00000000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SM_BLOCK_HDR_nWords", (char*) 0x00000004, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SM_FREE_BLOCK_node", (char*) 0x00000010, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SM_MSG_NODE_message", (char*) 0x0000000c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SM_MSG_NODE_msgLength", (char*) 0x00000008, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SM_MSG_Q_freeQ", (char*) 0x00000048, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SM_MSG_Q_freeQSem", (char*) 0x0000002c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SM_MSG_Q_maxMsgLength", (char*) 0x00000058, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SM_MSG_Q_maxMsgs", (char*) 0x00000054, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SM_MSG_Q_msgQ", (char*) 0x00000024, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SM_MSG_Q_msgQSem", (char*) 0x00000008, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SM_MSG_Q_options", (char*) 0x00000050, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SM_MSG_Q_recvTimeouts", (char*) 0x00000060, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SM_MSG_Q_sendTimeouts", (char*) 0x0000005c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SM_OBJ_MEM_HDR_curNumMsgQ", (char*) 0x000001d0, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SM_OBJ_MEM_HDR_curNumName", (char*) 0x000001dc, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SM_OBJ_MEM_HDR_curNumPart", (char*) 0x000001d8, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SM_OBJ_MEM_HDR_curNumSemB", (char*) 0x000001c8, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SM_OBJ_MEM_HDR_curNumSemC", (char*) 0x000001cc, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SM_OBJ_MEM_HDR_curNumTask", (char*) 0x000001d4, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SM_OBJ_MEM_HDR_maxMemParts", (char*) 0x000001c0, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SM_OBJ_MEM_HDR_maxMsgQueues", (char*) 0x000001b8, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SM_OBJ_MEM_HDR_maxNames", (char*) 0x000001c4, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SM_OBJ_MEM_HDR_maxSems", (char*) 0x000001b4, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SM_OBJ_MEM_HDR_maxTasks", (char*) 0x000001bc, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SM_OBJ_MEM_HDR_nameDtb", (char*) 0x00000184, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SM_OBJ_MEM_HDR_objCpuTbl", (char*) 0x000001b0, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SM_OBJ_MEM_HDR_smMsgQPart", (char*) 0x000000b8, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SM_OBJ_MEM_HDR_smNamePart", (char*) 0x00000074, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SM_OBJ_MEM_HDR_smPartPart", (char*) 0x000000fc, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SM_OBJ_MEM_HDR_smSemPart", (char*) 0x00000030, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SM_OBJ_MEM_HDR_smSysPart", (char*) 0x00000140, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SM_OBJ_MEM_HDR_smTcbPart", (char*) 0x0000000c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SM_OBJ_TCB_localTcb", (char*) 0x00000008, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SM_OBJ_TCB_ownerCpu", (char*) 0x0000000c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SM_OBJ_objType", (char*) 0x00000004, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SM_PARTITION_cumBlocksAllocated", (char*) 0x0000003c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SM_PARTITION_cumWordsAllocated", (char*) 0x00000040, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SM_PARTITION_curBlocksAllocated", (char*) 0x00000034, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SM_PARTITION_curWordsAllocated", (char*) 0x00000038, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SM_PARTITION_freeList", (char*) 0x00000008, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SM_PARTITION_totalWords", (char*) 0x00000028, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SM_SEMAPHORE_count", (char*) 0x00000014, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SM_SEMAPHORE_flag", (char*) 0x00000014, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SM_SEMAPHORE_lock", (char*) 0x00000008, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SM_SEMAPHORE_objType", (char*) 0x00000004, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SM_SEMAPHORE_smPendQ", (char*) 0x0000000c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SM_SEMAPHORE_verify", (char*) 0x00000000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SYMBOL_group", (char*) 0x00000010, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SYMBOL_name", (char*) 0x00000004, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SYMBOL_symRef", (char*) 0x0000000c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SYMBOL_type", (char*) 0x00000012, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SYMBOL_value", (char*) 0x00000008, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SYMTAB_nameHashId", (char*) 0x00000010, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SYSCALL_ENTRY_STATE_args", (char*) 0x00000000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SYSCALL_ENTRY_STATE_pUStack", (char*) 0x00000030, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SYSCALL_ENTRY_STATE_pc", (char*) 0x00000038, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SYSCALL_ENTRY_STATE_retAddrReg", (char*) 0x0000002c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SYSCALL_ENTRY_STATE_scn", (char*) 0x00000020, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_SYSCALL_ENTRY_STATE_size", (char*) 0x00000058, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_TRIGGER_actionArg", (char*) 0x00000078, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_TRIGGER_actionDef", (char*) 0x0000007c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_TRIGGER_actionFunc", (char*) 0x00000074, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_TRIGGER_actionType", (char*) 0x00000070, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_TRIGGER_chain", (char*) 0x00000058, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_TRIGGER_condEx1", (char*) 0x00000064, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_TRIGGER_condEx2", (char*) 0x0000006c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_TRIGGER_condOp", (char*) 0x00000068, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_TRIGGER_condType", (char*) 0x00000060, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_TRIGGER_conditional", (char*) 0x0000005c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_TRIGGER_contextId", (char*) 0x00000050, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_TRIGGER_contextType", (char*) 0x0000004c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_TRIGGER_disable", (char*) 0x00000048, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_TRIGGER_eventId", (char*) 0x00000044, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_TRIGGER_hitCnt", (char*) 0x00000080, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_TRIGGER_initialState", (char*) 0x00000084, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_TRIGGER_objCore", (char*) 0x00000000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_TRIGGER_objId", (char*) 0x00000054, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_TRIGGER_pDataCollector", (char*) 0x00000088, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_TRIGGER_status", (char*) 0x00000046, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_UNCOMPCB_dropped", (char*) 0x00000022, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_UNCOMPCB_hiwatermark", (char*) 0x00000020, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_UNCOMPCB_node", (char*) 0x00000000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_UNCOMPCB_pLastExp", (char*) 0x00000018, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_UNCOMPCB_pLastReg", (char*) 0x00000014, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_UNCOMPCB_pPeerSocket", (char*) 0x0000000c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_UNCOMPCB_pRecvPacket", (char*) 0x0000001c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_UNCOMPCB_pSelfSocket", (char*) 0x00000008, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_UNCOMPCB_selfName", (char*) 0x00000012, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_UNCOMPCB_selfState", (char*) 0x00000010, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_UNDEF_SYM_name", (char*) 0x00000004, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_VM_CONTEXT_mmuTransTbl", (char*) 0x00000044, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_VM_LIB_INFO_pVmCtxSwitchRtn", (char*) 0x00000034, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_VX_altivecTask", (char*) 0x02000000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_VX_coproc1Task", (char*) 0x01000000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_VX_coproc2Task", (char*) 0x02000000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_VX_coproc3Task", (char*) 0x04000000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_VX_coproc4Task", (char*) 0x08000000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_VX_coproc5Task", (char*) 0x10000000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_VX_coproc6Task", (char*) 0x20000000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_VX_coproc7Task", (char*) 0x40000000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_VX_coproc8Task", (char*) 0x80000000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_VX_fpTask", (char*) 0x01000000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_VX_speTask", (char*) 0x04000000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WDOG_deferStartCnt", (char*) 0x0000005e, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WDOG_status", (char*) 0x0000005c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WDOG_timeout", (char*) 0x0000004c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WDOG_wdParameter", (char*) 0x00000058, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WDOG_wdRoutine", (char*) 0x00000054, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_ISR_arg", (char*) 0x0000005c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_ISR_count", (char*) 0x00000048, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_ISR_cpuTime", (char*) 0x00000050, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_ISR_handlerRtn", (char*) 0x00000058, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_ISR_isrTag", (char*) 0x00000044, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_ISR_objCore", (char*) 0x00000000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_ISR_options", (char*) 0x00000060, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_ISR_serviceCount", (char*) 0x0000004c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_OBJ_accessCnt", (char*) 0x00000042, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_OBJ_classNode", (char*) 0x00000020, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_OBJ_handle", (char*) 0x00000000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_OBJ_name", (char*) 0x00000030, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_OBJ_objHandleList", (char*) 0x00000038, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_OBJ_ownerId", (char*) 0x00000028, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_OBJ_ownerList", (char*) 0x00000010, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_OBJ_ownerNode", (char*) 0x00000018, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_OBJ_ownerRtpId", (char*) 0x0000002c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_OBJ_pObjClass", (char*) 0x00000034, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_OBJ_refCnt", (char*) 0x00000040, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_RTP_binaryInfo", (char*) 0x00000084, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_RTP_cpuTimeInfo", (char*) 0x00000d68, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_RTP_defPath", (char*) 0x000000d4, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_RTP_defPathLen", (char*) 0x000000d8, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_RTP_entrAddr", (char*) 0x00000078, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_RTP_fdTable", (char*) 0x000000cc, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_RTP_fdTableSize", (char*) 0x000000d0, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_RTP_initTaskId", (char*) 0x0000007c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_RTP_kernelRtpCmn", (char*) 0x00000044, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_RTP_lockTaskId", (char*) 0x00000d54, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_RTP_options", (char*) 0x00000074, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_RTP_pArgv", (char*) 0x00000068, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_RTP_pEnv", (char*) 0x0000006c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_RTP_pPathName", (char*) 0x00000064, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_RTP_pSchedInfo", (char*) 0x00000d78, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_RTP_pSigQFreeHead", (char*) 0x00000920, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_RTP_parentRtpId", (char*) 0x00000934, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_RTP_rtpNode", (char*) 0x00000058, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_RTP_semId", (char*) 0x00000060, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_RTP_sigReturnRtn", (char*) 0x000000e0, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_RTP_signalInfo", (char*) 0x000000e8, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_RTP_sigwaitQ", (char*) 0x00000924, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_RTP_status", (char*) 0x00000070, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_RTP_symTabId", (char*) 0x00000080, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_RTP_taskCnt", (char*) 0x000000b4, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_RTP_taskExitRtn", (char*) 0x000000dc, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_SD_attr", (char*) 0x000000bc, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_SD_clientCount", (char*) 0x000000d4, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_SD_objCore", (char*) 0x00000000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_SD_options", (char*) 0x000000b8, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_SD_physAdrs", (char*) 0x000000c8, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_SD_sem", (char*) 0x00000048, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_SD_size", (char*) 0x000000c0, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_SD_virtAdrs", (char*) 0x000000d0, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_SOCKET_so_fd", (char*) 0x00000044, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_SOCKET_so_linger", (char*) 0x00000008, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_SOCKET_so_options", (char*) 0x00000006, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_SOCKET_so_pcb", (char*) 0x0000000c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_SOCKET_so_qlen", (char*) 0x00000030, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_SOCKET_so_qlimit", (char*) 0x00000034, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_SOCKET_so_state", (char*) 0x0000000a, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_SOCKET_so_type", (char*) 0x00000004, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_TCB_cpuTimeInfo", (char*) 0x00000168, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_TCB_entry", (char*) 0x000000c4, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_TCB_errorStatus", (char*) 0x000000d4, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_TCB_events", (char*) 0x00000110, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_TCB_excCnt", (char*) 0x00000070, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_TCB_excInfo", (char*) 0x000001bc, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_TCB_lockCnt", (char*) 0x00000080, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_TCB_objCore", (char*) 0x00000000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_TCB_options", (char*) 0x000000a0, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_TCB_pCoprocTbl", (char*) 0x000000f4, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_TCB_pDbgInfo", (char*) 0x00000138, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_TCB_pEdrInfo", (char*) 0x000000d0, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_TCB_pExcRegSet", (char*) 0x00000130, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_TCB_pExcStackBase", (char*) 0x00000064, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_TCB_pExcStackEnd", (char*) 0x00000068, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_TCB_pExcStackStart", (char*) 0x0000006c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_TCB_pPendQ", (char*) 0x000000a8, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_TCB_pPriMutex", (char*) 0x00000090, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_TCB_pPwrMgmtPState", (char*) 0x00000178, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_TCB_pSchedInfo", (char*) 0x0000017c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_TCB_pStackBase", (char*) 0x000000c8, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_TCB_pStackEnd", (char*) 0x000000cc, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_TCB_pTlsDesc", (char*) 0x00000298, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_TCB_pUTcb", (char*) 0x0000009c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_TCB_priDrop", (char*) 0x0000008c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_TCB_priInheritCnt", (char*) 0x00000088, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_TCB_priMutexCnt", (char*) 0x00000084, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_TCB_priNormal", (char*) 0x0000007c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_TCB_priority", (char*) 0x00000078, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_TCB_qNode", (char*) 0x00000054, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_TCB_regs", (char*) 0x000001ec, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_TCB_rtpId", (char*) 0x00000098, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_TCB_safeCnt", (char*) 0x000000b0, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_TCB_safetyQHead", (char*) 0x000000b4, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_TCB_status", (char*) 0x00000074, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_TCB_swapInMask", (char*) 0x000000ac, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_TCB_swapOutMask", (char*) 0x000000ae, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_TCB_taskTicks", (char*) 0x000000e4, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_TCB_timeout", (char*) 0x0000004c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WIND_TCB_windSmpInfo", (char*) 0x00000184, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WORK_Q_IX_bitmask", (char*) 0x00000004, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WORK_Q_IX_read", (char*) 0x00000000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WORK_Q_IX_write", (char*) 0x00000008, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WV_HASH_NODE_eventSize", (char*) 0x00000004, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WV_HASH_NODE_key", (char*) 0x00000008, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WV_HASH_NODE_next", (char*) 0x0000000c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WV_HASH_NODE_pEvent", (char*) 0x00000000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WV_HASH_TBL_size", (char*) 0x00000000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WV_HASH_TBL_tbl", (char*) 0x00000004, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WV_LOG_LIST_hdr_magic", (char*) 0x00000010, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WV_LOG_LIST_hdr_memPart", (char*) 0x00000008, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WV_LOG_LIST_hdr_numLogs", (char*) 0x0000000c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WV_LOG_LIST_hdr_wvLogListHead", (char*) 0x00000000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WV_LOG_configEventSize", (char*) 0x00000010, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WV_LOG_memPart", (char*) 0x00000004, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WV_LOG_node", (char*) 0x00000000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WV_LOG_pConfigEvent", (char*) 0x0000000c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WV_LOG_pEvtBuffer", (char*) 0x00000008, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_WV_LOG_pHashTbl", (char*) 0x00000014, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_hiddenModule", (char*) 0x00000010, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_loadAllSymbols", (char*) 0x0000000c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_loadCommonHeap", (char*) 0x00000800, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_loadCommonMatchAll", (char*) 0x00000400, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_loadCommonMatchNone", (char*) 0x00000100, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_loadCommonMatchUser", (char*) 0x00000200, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_loadCplusXtorAuto", (char*) 0x00001000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_loadCplusXtorManual", (char*) 0x00002000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_loadForce", (char*) 0x00000080, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_loadFullyLinked", (char*) 0x00000020, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_loadGlobalSymbols", (char*) 0x00000008, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_loadLocalSymbols", (char*) 0x00000004, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_loadNoDownload", (char*) 0x00000040, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_loadNoSymbols", (char*) 0x00000002, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_loadWeakMatchAll", (char*) 0x00020000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_loadWeakMatchNone", (char*) 0x00010000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_sizeof_FD_ENTRY", (char*) 0x00000060, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_sizeof_MSG_NODE", (char*) 0x0000000c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_sizeof_MSG_Q", (char*) 0x000000b0, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_sizeof_SEMAPHORE", (char*) 0x00000070, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_sizeof_SEM_RW_EXT", (char*) 0x00000018, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_sizeof_SEM_RW_LIST_ENTRY", (char*) 0x0000000c, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_sizeof_WDOG", (char*) 0x00000060, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_sizeof_WIND_OBJ", (char*) 0x00000044, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_syscallEntryBaseOffset", (char*) 0x00000000, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_unloadCplusXtorAuto", (char*) 0x00000020, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_unloadCplusXtorManual", (char*) 0x00000040, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vx_offset_unloadForce", (char*) 0x00000002, 0, 0, SYM_GLOBAL | SYM_ABS},
        {{NULL}, "_vxb_clkConnectRtn", (char*) &_vxb_clkConnectRtn, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_vxb_clkDisableRtn", (char*) &_vxb_clkDisableRtn, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_vxb_clkEnableRtn", (char*) &_vxb_clkEnableRtn, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_vxb_clkRateGetRtn", (char*) &_vxb_clkRateGetRtn, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_vxb_clkRateSetRtn", (char*) &_vxb_clkRateSetRtn, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_vxb_delayRtn", (char*) &_vxb_delayRtn, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_vxb_msDelayRtn", (char*) &_vxb_msDelayRtn, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_vxb_usDelayRtn", (char*) &_vxb_usDelayRtn, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_vxmux_pNullPoolFuncTbl", (char*) &_vxmux_pNullPoolFuncTbl, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "_vxworks_asctime_r", (char*) _vxworks_asctime_r, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_vxworks_ctime_r", (char*) _vxworks_ctime_r, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_vxworks_gmtime_r", (char*) _vxworks_gmtime_r, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_vxworks_localtime_r", (char*) _vxworks_localtime_r, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_zbufCreate_nolock", (char*) _zbufCreate_nolock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_zbufDeleteEmpty", (char*) _zbufDeleteEmpty, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_zbufSetSeg", (char*) _zbufSetSeg, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_zbuf_getseg_nolock", (char*) _zbuf_getseg_nolock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "_zbuf_getsegptr_nolock", (char*) _zbuf_getsegptr_nolock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "abort", (char*) abort, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "abs", (char*) abs, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "absSymbols_Common", (char*) absSymbols_Common, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "accept", (char*) accept, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "access", (char*) access, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "acos", (char*) acos, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "aimCacheInit", (char*) aimCacheInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "aimFppLibInit", (char*) aimFppLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "aimMmuBaseLibInit", (char*) aimMmuBaseLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "aimMmuContextTbl", (char*) &aimMmuContextTbl, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "aimMmuLibInit", (char*) aimMmuLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "aimMmuNullPageTbl", (char*) &aimMmuNullPageTbl, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "aimMmuNullRegionTbl", (char*) &aimMmuNullRegionTbl, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "allocChunkEndGuardSize", (char*) &allocChunkEndGuardSize, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "applLoggerInit", (char*) applLoggerInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "applLoggerStop", (char*) applLoggerStop, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "applUtilInstInit", (char*) applUtilInstInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "applUtilLogSem", (char*) &applUtilLogSem, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "asctime", (char*) asctime, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "asctime_r", (char*) asctime_r, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "asin", (char*) asin, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "assertFiles", (char*) &assertFiles, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "atan", (char*) atan, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "atan2", (char*) atan2, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "atexit", (char*) atexit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "atof", (char*) atof, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "atoi", (char*) atoi, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "atol", (char*) atol, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "attrib", (char*) attrib, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "avlUintDelete", (char*) avlUintDelete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "avlUintInsert", (char*) avlUintInsert, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "avlUintMaximumGet", (char*) avlUintMaximumGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "avlUintMinimumGet", (char*) avlUintMinimumGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "avlUintPredecessorGet", (char*) avlUintPredecessorGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "avlUintSearch", (char*) avlUintSearch, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "avlUintSuccessorGet", (char*) avlUintSuccessorGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "avlUintTreeWalk", (char*) avlUintTreeWalk, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "b", (char*) b, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "baudRateGenClk", (char*) &baudRateGenClk, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "bcmp", (char*) bcmp, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bcopy", (char*) bcopy, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bcopyBytes", (char*) bcopyBytes, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bcopyLongs", (char*) bcopyLongs, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bcopyWords", (char*) bcopyWords, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bd", (char*) bd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bdall", (char*) bdall, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bfill", (char*) bfill, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bfillBytes", (char*) bfillBytes, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bh", (char*) bh, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bi", (char*) bi, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bind", (char*) bind, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bindresvport", (char*) bindresvport, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bindresvportCommon", (char*) bindresvportCommon, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bindresvport_af", (char*) bindresvport_af, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "binvert", (char*) binvert, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bman0Resources", (char*) &bman0Resources, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "bmanRegister", (char*) bmanRegister, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bootBpAnchorExtract", (char*) bootBpAnchorExtract, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bootChange", (char*) bootChange, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bootLeaseExtract", (char*) bootLeaseExtract, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bootNetmaskExtract", (char*) bootNetmaskExtract, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bootParamsErrorPrint", (char*) bootParamsErrorPrint, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bootParamsPrompt", (char*) bootParamsPrompt, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bootParamsShow", (char*) bootParamsShow, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bootScanNum", (char*) bootScanNum, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bootStringToStruct", (char*) bootStringToStruct, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bootStringToStructAdd", (char*) bootStringToStructAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bootStructToString", (char*) bootStructToString, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bportalAcquire", (char*) bportalAcquire, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bportalAlloc", (char*) bportalAlloc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bportalCountGet", (char*) bportalCountGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bportalCreate", (char*) bportalCreate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bportalDestroy", (char*) bportalDestroy, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bportalFree", (char*) bportalFree, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bportalIntConnect", (char*) bportalIntConnect, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bportalIntDisable", (char*) bportalIntDisable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bportalIntDisconnect", (char*) bportalIntDisconnect, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bportalIntEnable", (char*) bportalIntEnable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bportalLocalAlloc", (char*) bportalLocalAlloc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bportalLocalFree", (char*) bportalLocalFree, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bportalLocalMultiAlloc", (char*) bportalLocalMultiAlloc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bportalLocalMultiFree", (char*) bportalLocalMultiFree, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bportalMultiAlloc", (char*) bportalMultiAlloc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bportalMultiFree", (char*) bportalMultiFree, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bportalQuery", (char*) bportalQuery, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bportalRelease", (char*) bportalRelease, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bportalSafeAlloc", (char*) bportalSafeAlloc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bportalSafeFree", (char*) bportalSafeFree, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bportalSafeMultiAlloc", (char*) bportalSafeMultiAlloc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bportalSafeMultiFree", (char*) bportalSafeMultiFree, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bsearch", (char*) bsearch, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bspSerialChanGet", (char*) bspSerialChanGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "bswap", (char*) bswap, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "busCtlrDevCtlr_desc", (char*) &busCtlrDevCtlr_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "busDevShow_desc", (char*) &busDevShow_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "bzero", (char*) bzero, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "c", (char*) c, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cacheAddrAlign", (char*) &cacheAddrAlign, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "cacheAimE500ClearD", (char*) cacheAimE500ClearD, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cacheAimE500DisableDFromMmu", (char*) cacheAimE500DisableDFromMmu, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cacheAimE500DisableD_Impl", (char*) cacheAimE500DisableD_Impl, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cacheAimE500DisableIFromMmu", (char*) cacheAimE500DisableIFromMmu, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cacheAimE500DisableI_Impl", (char*) cacheAimE500DisableI_Impl, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cacheAimE500EnableD_Impl", (char*) cacheAimE500EnableD_Impl, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cacheAimE500EnableI_Impl", (char*) cacheAimE500EnableI_Impl, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cacheAimE500FlushAllD", (char*) cacheAimE500FlushAllD, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cacheAimE500FlushD", (char*) cacheAimE500FlushD, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cacheAimE500InvalidateD", (char*) cacheAimE500InvalidateD, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cacheAimE500InvalidateI", (char*) cacheAimE500InvalidateI, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cacheAimE500PipeFlush", (char*) cacheAimE500PipeFlush, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cacheAimE500TextUpdate", (char*) cacheAimE500TextUpdate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cacheAimE500dCachePEShutdown", (char*) cacheAimE500dCachePEShutdown, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cacheAimInfo", (char*) &cacheAimInfo, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "cacheArchDmaFree", (char*) cacheArchDmaFree, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cacheArchDmaMalloc", (char*) cacheArchDmaMalloc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cacheArchEnable", (char*) cacheArchEnable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cacheArchLibInit", (char*) cacheArchLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cacheClear", (char*) cacheClear, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cacheDToEnable", (char*) &cacheDToEnable, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "cacheDataEnabled", (char*) &cacheDataEnabled, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "cacheDataMode", (char*) &cacheDataMode, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "cacheDisable", (char*) cacheDisable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cacheDma32Free", (char*) cacheDma32Free, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cacheDma32Malloc", (char*) cacheDma32Malloc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cacheDmaFree", (char*) cacheDmaFree, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cacheDmaFreeRtn", (char*) &cacheDmaFreeRtn, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "cacheDmaFuncs", (char*) &cacheDmaFuncs, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "cacheDmaMalloc", (char*) cacheDmaMalloc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cacheDmaMallocRtn", (char*) &cacheDmaMallocRtn, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "cacheDrvFlush", (char*) cacheDrvFlush, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cacheDrvInvalidate", (char*) cacheDrvInvalidate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cacheDrvPhysToVirt", (char*) cacheDrvPhysToVirt, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cacheDrvVirtToPhys", (char*) cacheDrvVirtToPhys, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cacheEnable", (char*) cacheEnable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cacheFlush", (char*) cacheFlush, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cacheForeignClear", (char*) cacheForeignClear, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cacheForeignFlush", (char*) cacheForeignFlush, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cacheForeignInvalidate", (char*) cacheForeignInvalidate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cacheFuncsSet", (char*) cacheFuncsSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cacheIToEnable", (char*) &cacheIToEnable, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "cacheInvalidate", (char*) cacheInvalidate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cacheLib", (char*) &cacheLib, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "cacheLibInit", (char*) cacheLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cacheLineMask", (char*) &cacheLineMask, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "cacheLineSize", (char*) &cacheLineSize, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "cacheLock", (char*) cacheLock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cacheMmuAvailable", (char*) &cacheMmuAvailable, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "cacheNullFuncs", (char*) &cacheNullFuncs, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "cachePipeFlush", (char*) cachePipeFlush, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cachePpcReadOrigin", (char*) &cachePpcReadOrigin, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "cacheTextLocalUpdate", (char*) cacheTextLocalUpdate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cacheTextUpdate", (char*) cacheTextUpdate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cacheTypeChk", (char*) cacheTypeChk, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cacheUnlock", (char*) cacheUnlock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cacheUserFuncs", (char*) &cacheUserFuncs, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "calloc", (char*) calloc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "catInfo", (char*) &catInfo, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ccRatioTable", (char*) &ccRatioTable, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ccs1RatioTable", (char*) &ccs1RatioTable, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ccs2RatioTable", (char*) &ccs2RatioTable, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "cd", (char*) cd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ceil", (char*) ceil, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cfree", (char*) cfree, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "changeFpReg", (char*) changeFpReg, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "changeLogLevel", (char*) changeLogLevel, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "changeReg", (char*) changeReg, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "chdir", (char*) chdir, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "checkStack", (char*) checkStack, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "chipErrataCpu29Print", (char*) chipErrataCpu29Print, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "chkdsk", (char*) chkdsk, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "chmod", (char*) chmod, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "classCreateConnect", (char*) classCreateConnect, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "classDestroyConnect", (char*) classDestroyConnect, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "classIdTable", (char*) &classIdTable, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "classInit", (char*) classInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "classLibInit", (char*) classLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "classListLibInit", (char*) classListLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "classMemPartIdSet", (char*) classMemPartIdSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "classShow", (char*) classShow, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "classShowConnect", (char*) classShowConnect, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "classShowInit", (char*) classShowInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "clearerr", (char*) clearerr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "clock", (char*) clock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "clockLibInit", (char*) clockLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "clock_getres", (char*) clock_getres, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "clock_gettime", (char*) clock_gettime, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "clock_gettime_wd", (char*) clock_gettime_wd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "clock_nanosleep", (char*) clock_nanosleep, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "clock_setres", (char*) clock_setres, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "clock_settime", (char*) clock_settime, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "close", (char*) close, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "closedir", (char*) closedir, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "commit", (char*) commit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "completionPathComplete", (char*) completionPathComplete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "completionPathListGet", (char*) completionPathListGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "completionPathShow", (char*) completionPathShow, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "completionSymComplete", (char*) completionSymComplete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "completionSymListGet", (char*) completionSymListGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "completionSymShow", (char*) completionSymShow, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "connect", (char*) connect, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "connectWithTimeout", (char*) connectWithTimeout, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "consoleFd", (char*) &consoleFd, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "consoleName", (char*) &consoleName, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "coprocCtxReplicate", (char*) coprocCtxReplicate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "coprocCtxStorageSizeGet", (char*) coprocCtxStorageSizeGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "coprocDescriptorsInit", (char*) coprocDescriptorsInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "coprocDisable", (char*) coprocDisable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "coprocEnable", (char*) coprocEnable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "coprocLibInit", (char*) coprocLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "coprocMRegs", (char*) coprocMRegs, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "coprocRegListShow", (char*) coprocRegListShow, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "coprocSave", (char*) coprocSave, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "coprocShow", (char*) coprocShow, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "coprocShowInit", (char*) coprocShowInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "coprocShowOnDebug", (char*) coprocShowOnDebug, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "coprocTaskCreateHook", (char*) coprocTaskCreateHook, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "coprocTaskDeleteHook", (char*) coprocTaskDeleteHook, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "coprocTaskRegsGet", (char*) coprocTaskRegsGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "coprocTaskRegsSet", (char*) coprocTaskRegsSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "coprocTaskRegsShow", (char*) coprocTaskRegsShow, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "coprocTaskSwapHook", (char*) coprocTaskSwapHook, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "coprocsDiscover", (char*) coprocsDiscover, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "copy", (char*) copy, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "copyStreams", (char*) copyStreams, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "copysign", (char*) copysign, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cos", (char*) cos, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cosh", (char*) cosh, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cp", (char*) cp, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cplusCallCtors", (char*) cplusCallCtors, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cplusCallDtors", (char*) cplusCallDtors, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cplusCtors", (char*) cplusCtors, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cplusCtorsLink", (char*) cplusCtorsLink, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cplusDemangle", (char*) cplusDemangle, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cplusDemangle2", (char*) cplusDemangle2, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cplusDemangle2Func", (char*) &cplusDemangle2Func, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "cplusDemangleFunc", (char*) &cplusDemangleFunc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "cplusDemangleToBuffer", (char*) cplusDemangleToBuffer, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cplusDemangleToBufferFunc", (char*) &cplusDemangleToBufferFunc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "cplusDemanglerMode", (char*) &cplusDemanglerMode, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "cplusDemanglerSet", (char*) cplusDemanglerSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cplusDemanglerStyle", (char*) &cplusDemanglerStyle, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "cplusDemanglerStyleInit", (char*) cplusDemanglerStyleInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cplusDemanglerStyleSet", (char*) cplusDemanglerStyleSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cplusDtors", (char*) cplusDtors, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cplusDtorsLink", (char*) cplusDtorsLink, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cplusLoadCtorsCall", (char*) cplusLoadCtorsCall, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cplusLoadFixup", (char*) cplusLoadFixup, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cplusMangledSymCheck", (char*) cplusMangledSymCheck, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cplusMangledSymGet", (char*) cplusMangledSymGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cplusMatchMangled", (char*) cplusMatchMangled, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cplusMatchMangledListGet", (char*) cplusMatchMangledListGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cplusUnloadFixup", (char*) cplusUnloadFixup, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cplusUserAsk", (char*) cplusUserAsk, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cplusXtorGet", (char*) cplusXtorGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cplusXtorSet", (char*) cplusXtorSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cplusXtorStrategy", (char*) &cplusXtorStrategy, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "cpuPwrLightMgrInit", (char*) cpuPwrLightMgrInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cpuPwrMgrEnable", (char*) cpuPwrMgrEnable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "cpuPwrMgrIsEnabled", (char*) cpuPwrMgrIsEnabled, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "creat", (char*) creat, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "creationDate", (char*) &creationDate, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "cret", (char*) cret, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ctime", (char*) ctime, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ctime_r", (char*) ctime_r, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ctypeFiles", (char*) &ctypeFiles, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "d", (char*) d, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "dCachePushParityErrorMsg", (char*) &dCachePushParityErrorMsg, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "dbgBpEpCoreRtn", (char*) dbgBpEpCoreRtn, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "dbgBpListPrint", (char*) dbgBpListPrint, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "dbgCallPrint", (char*) dbgCallPrint, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "dbgDisassemble", (char*) dbgDisassemble, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "dbgDynPrintfCommonRtn", (char*) dbgDynPrintfCommonRtn, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "dbgHelp", (char*) dbgHelp, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "dbgInit", (char*) dbgInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "dbgPrintDsp", (char*) &dbgPrintDsp, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "dbgPrintFpp", (char*) &dbgPrintFpp, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "dbgPrintSimd", (char*) &dbgPrintSimd, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "dbgStackTrace", (char*) dbgStackTrace, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "dbgTaskTraceCoreRtn", (char*) dbgTaskTraceCoreRtn, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ddrRatioTable", (char*) &ddrRatioTable, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "defaultDrv", (char*) &defaultDrv, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "devResourceGet", (char*) devResourceGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "devResourceIntrGet", (char*) devResourceIntrGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "devs", (char*) devs, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "difftime", (char*) difftime, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "dirList", (char*) dirList, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "disableBranchPrediction", (char*) disableBranchPrediction, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "diskFormat", (char*) diskFormat, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "diskInit", (char*) diskInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "div", (char*) div, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "div_r", (char*) div_r, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "dllAdd", (char*) dllAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "dllCount", (char*) dllCount, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "dllEach", (char*) dllEach, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "dllGet", (char*) dllGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "dllInit", (char*) dllInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "dllInsert", (char*) dllInsert, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "dllRemove", (char*) dllRemove, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "dllTerminate", (char*) dllTerminate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "dosfsDiskFormat", (char*) dosfsDiskFormat, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "dprintf", (char*) dprintf, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "dprintfVerboseLvl", (char*) &dprintfVerboseLvl, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "drem", (char*) drem, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "drvTable", (char*) &drvTable, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "dsmFindByInst", (char*) dsmFindByInst, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "dsmFindByName", (char*) dsmFindByName, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "dsmInst", (char*) dsmInst, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "dtmRegister", (char*) dtmRegister, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "dtsec0Resources", (char*) &dtsec0Resources, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "dtsec1Resources", (char*) &dtsec1Resources, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "dtsec2Resources", (char*) &dtsec2Resources, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "dtsec3Resources", (char*) &dtsec3Resources, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "dtsec4Resources", (char*) &dtsec4Resources, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "dtsecMdio0Resources", (char*) &dtsecMdio0Resources, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "dummyErrno", (char*) &dummyErrno, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "dup", (char*) dup, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "dup2", (char*) dup2, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "e", (char*) e, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "edrErrorInjectStub", (char*) edrErrorInjectStub, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "edrErrorPolicyHookRemove", (char*) edrErrorPolicyHookRemove, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "edrFlagsGet", (char*) edrFlagsGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "edrInitFatalPolicyHandler", (char*) edrInitFatalPolicyHandler, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "edrInterruptFatalPolicyHandler", (char*) edrInterruptFatalPolicyHandler, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "edrIsDebugMode", (char*) edrIsDebugMode, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "edrKernelFatalPolicyHandler", (char*) edrKernelFatalPolicyHandler, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "edrPolicyHandlerHookAdd", (char*) edrPolicyHandlerHookAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "edrPolicyHandlerHookGet", (char*) edrPolicyHandlerHookGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "edrRtpFatalPolicyHandler", (char*) edrRtpFatalPolicyHandler, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "edrStubInit", (char*) edrStubInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "edrSystemDebugModeGet", (char*) edrSystemDebugModeGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "edrSystemDebugModeInit", (char*) edrSystemDebugModeInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "edrSystemDebugModeSet", (char*) edrSystemDebugModeSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "enablePteProtection", (char*) &enablePteProtection, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "end8023AddressForm", (char*) end8023AddressForm, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "endDevName", (char*) endDevName, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "endDevTbl", (char*) &endDevTbl, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "endEtherAddressForm", (char*) endEtherAddressForm, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "endEtherCrc32BeGet", (char*) endEtherCrc32BeGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "endEtherCrc32LeGet", (char*) endEtherCrc32LeGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "endEtherHdrInit", (char*) endEtherHdrInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "endEtherPacketAddrGet", (char*) endEtherPacketAddrGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "endEtherPacketDataGet", (char*) endEtherPacketDataGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "endFindByName", (char*) endFindByName, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "endFirstUnitFind", (char*) endFirstUnitFind, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "endFlagsClr", (char*) endFlagsClr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "endFlagsGet", (char*) endFlagsGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "endFlagsSet", (char*) endFlagsSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "endLibInit", (char*) endLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "endList", (char*) &endList, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "endM2Free", (char*) endM2Free, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "endM2Init", (char*) endM2Init, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "endM2Ioctl", (char*) endM2Ioctl, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "endM2Packet", (char*) &endM2Packet, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "endMibIfInit", (char*) endMibIfInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "endMultiLstCnt", (char*) endMultiLstCnt, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "endMultiLstFirst", (char*) endMultiLstFirst, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "endMultiLstNext", (char*) endMultiLstNext, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "endObjFlagSet", (char*) endObjFlagSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "endObjInit", (char*) endObjInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "endObjectUnload", (char*) endObjectUnload, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "endPollStatsInit", (char*) endPollStatsInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "endPoolClSize", (char*) &endPoolClSize, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "endPoolCreate", (char*) endPoolCreate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "endPoolDestroy", (char*) endPoolDestroy, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "endPoolJumboClSize", (char*) &endPoolJumboClSize, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "endPoolJumboCreate", (char*) endPoolJumboCreate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "endRcvRtnCall", (char*) endRcvRtnCall, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "endTxSemGive", (char*) endTxSemGive, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "endTxSemTake", (char*) endTxSemTake, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "envGet", (char*) envGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "envLibInit", (char*) envLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "envLibSem", (char*) &envLibSem, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "envLibSemId", (char*) &envLibSemId, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "envPrivateCreate", (char*) envPrivateCreate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "envPrivateDestroy", (char*) envPrivateDestroy, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "envShow", (char*) envShow, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "epic0Resources", (char*) &epic0Resources, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "epicCpu", (char*) &epicCpu, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "epicInputs", (char*) &epicInputs, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "epicPriority", (char*) &epicPriority, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "epicXBar", (char*) &epicXBar, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "errno", (char*) &errno, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "errnoGet", (char*) errnoGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "errnoOfTaskGet", (char*) errnoOfTaskGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "errnoOfTaskSet", (char*) errnoOfTaskSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "errnoSet", (char*) errnoSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "etherMultiAdd", (char*) etherMultiAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "etherMultiDel", (char*) etherMultiDel, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "etherMultiGet", (char*) etherMultiGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "etherbroadcastaddr", (char*) &etherbroadcastaddr, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "eventClear", (char*) eventClear, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "eventInit", (char*) eventInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "eventLibInit", (char*) eventLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "eventPointStubLibInit", (char*) eventPointStubLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "eventPointSwHandle", (char*) eventPointSwHandle, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "eventPointSwStubConnect", (char*) eventPointSwStubConnect, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "eventReceive", (char*) eventReceive, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "eventRsrcSend", (char*) eventRsrcSend, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "eventSend", (char*) eventSend, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "eventStart", (char*) eventStart, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "eventTerminate", (char*) eventTerminate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "evtAction", (char*) &evtAction, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "exDesc", (char*) &exDesc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "excConnect", (char*) excConnect, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "excCrtConnect", (char*) excCrtConnect, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "excCrtEnt", (char*) excCrtEnt, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "excCrtExit", (char*) excCrtExit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "excDecrHandle", (char*) excDecrHandle, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "excEPSet", (char*) excEPSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "excEnt", (char*) excEnt, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "excExcHandle", (char*) excExcHandle, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "excExcepHook", (char*) &excExcepHook, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "excExit", (char*) excExit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "excExtendedVectors", (char*) &excExtendedVectors, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "excHookAdd", (char*) excHookAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "excInit", (char*) excInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "excIntConnect", (char*) excIntConnect, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "excIntCrtConnect", (char*) excIntCrtConnect, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "excIntHandle", (char*) excIntHandle, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "excIntNestLogInit", (char*) excIntNestLogInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "excIvorInit", (char*) excIvorInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "excJobAdd", (char*) excJobAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "excMchkConnect", (char*) excMchkConnect, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "excMchkEnt", (char*) excMchkEnt, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "excMchkExit", (char*) excMchkExit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "excShowInit", (char*) excShowInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "excVecBaseAltAdrs", (char*) &excVecBaseAltAdrs, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "excVecBaseGet", (char*) excVecBaseGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "excVecBaseSet", (char*) excVecBaseSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "excVecGet", (char*) excVecGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "excVecInit", (char*) excVecInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "excVecSet", (char*) excVecSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "exit", (char*) exit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "exp", (char*) exp, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "exp__E", (char*) exp__E, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "expm1", (char*) expm1, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fabs", (char*) fabs, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fchmod", (char*) fchmod, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fclose", (char*) fclose, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fcntl", (char*) fcntl, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fcntl_dup", (char*) fcntl_dup, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fdClass", (char*) &fdClass, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "fdInvalidDrv", (char*) &fdInvalidDrv, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "fdatasync", (char*) fdatasync, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fdopen", (char*) fdopen, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fdprintf", (char*) fdprintf, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "feof", (char*) feof, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ferror", (char*) ferror, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fflush", (char*) fflush, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ffs32Lsb", (char*) ffs32Lsb, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ffs32Msb", (char*) ffs32Msb, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ffs64Lsb", (char*) ffs64Lsb, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ffs64Msb", (char*) ffs64Msb, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ffsLsb", (char*) ffsLsb, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ffsMsb", (char*) ffsMsb, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fgetc", (char*) fgetc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fgetpos", (char*) fgetpos, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fgets", (char*) fgets, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fieldSzIncludeSign", (char*) &fieldSzIncludeSign, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "fileno", (char*) fileno, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "finite", (char*) finite, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fioBaseLibInit", (char*) fioBaseLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fioBufPrint", (char*) fioBufPrint, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fioBufPut", (char*) fioBufPut, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fioFltInstall", (char*) fioFltInstall, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fioFormatV", (char*) fioFormatV, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fioLibInit", (char*) fioLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fioRdString", (char*) fioRdString, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fioRead", (char*) fioRead, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fioScanV", (char*) fioScanV, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fioSnBufPut", (char*) fioSnBufPut, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "firstStaticDesc", (char*) &firstStaticDesc, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "flashDelayAdj", (char*) &flashDelayAdj, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "flashDelayIncr", (char*) &flashDelayIncr, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "flashDelayShift", (char*) &flashDelayShift, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "floatInit", (char*) floatInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "floor", (char*) floor, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fman0Resources", (char*) &fman0Resources, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "fman1Resources", (char*) &fman1Resources, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "fmanBmiPoolInfoGet", (char*) fmanBmiPoolInfoGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fmanBmiPoolInfoSet", (char*) fmanBmiPoolInfoSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fmanBmiPortConfig", (char*) fmanBmiPortConfig, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fmanBmiPortHalt", (char*) fmanBmiPortHalt, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fmanCcConfigAdd", (char*) fmanCcConfigAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fmanCcConfigFlush", (char*) fmanCcConfigFlush, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fmanCcPortConfig", (char*) fmanCcPortConfig, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fmanCcPortHalt", (char*) fmanCcPortHalt, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fmanKgHash", (char*) fmanKgHash, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fmanKgPartConfRead", (char*) fmanKgPartConfRead, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fmanKgPartConfWrite", (char*) fmanKgPartConfWrite, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fmanKgPlanRead", (char*) fmanKgPlanRead, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fmanKgPlanWrite", (char*) fmanKgPlanWrite, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fmanKgPortConfig", (char*) fmanKgPortConfig, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fmanKgPortHalt", (char*) fmanKgPortHalt, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fmanKgSchemeRead", (char*) fmanKgSchemeRead, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fmanKgSchemeWrite", (char*) fmanKgSchemeWrite, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fmanPlPortConfig", (char*) fmanPlPortConfig, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fmanPlPortHalt", (char*) fmanPlPortHalt, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fmanPlPramFlush", (char*) fmanPlPramFlush, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fmanPlProfileRead", (char*) fmanPlProfileRead, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fmanPlProfileWrite", (char*) fmanPlProfileWrite, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fmanPlRatesCalc", (char*) fmanPlRatesCalc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fmanPrPortConfig", (char*) fmanPrPortConfig, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fmanPrPortHalt", (char*) fmanPrPortHalt, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fmanPrPortTpidGet", (char*) fmanPrPortTpidGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fmanPrPortTpidSet", (char*) fmanPrPortTpidSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fmanPrSpCodeFlush", (char*) fmanPrSpCodeFlush, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fmanPrSpFmcProgLoad", (char*) fmanPrSpFmcProgLoad, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fmanPrSpPortConfig", (char*) fmanPrSpPortConfig, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fmanQmiPortConfig", (char*) fmanQmiPortConfig, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fmanQmiPortHalt", (char*) fmanQmiPortHalt, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fmanRegister", (char*) fmanRegister, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fmod", (char*) fmod, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fopen", (char*) fopen, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fpCtlRegName", (char*) &fpCtlRegName, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "fpExcMsgTbl", (char*) &fpExcMsgTbl, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "fpRegName", (char*) &fpRegName, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "fpTypeGet", (char*) fpTypeGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fpathconf", (char*) fpathconf, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fppCtxCreate", (char*) fppCtxCreate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fppCtxDelete", (char*) fppCtxDelete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fppCtxShow", (char*) fppCtxShow, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fppDisable", (char*) fppDisable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fppEnable", (char*) fppEnable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fppMRegs", (char*) fppMRegs, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fppProbe", (char*) fppProbe, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fppRegListShow", (char*) fppRegListShow, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fppRestore", (char*) fppRestore, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fppSave", (char*) fppSave, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fppTaskRegsCFmt", (char*) &fppTaskRegsCFmt, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "fppTaskRegsDFmt", (char*) &fppTaskRegsDFmt, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "fppTaskRegsGet", (char*) fppTaskRegsGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fppTaskRegsSet", (char*) fppTaskRegsSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fprintf", (char*) fprintf, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fputc", (char*) fputc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fputs", (char*) fputs, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fread", (char*) fread, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "free", (char*) free, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "freopen", (char*) freopen, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "frexp", (char*) frexp, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fscanf", (char*) fscanf, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fseek", (char*) fseek, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fsetpos", (char*) fsetpos, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fslGpioRegister", (char*) fslGpioRegister, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fstat", (char*) fstat, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fstatfs", (char*) fstatfs, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fstatfs64", (char*) fstatfs64, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "fsync", (char*) fsync, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ftell", (char*) ftell, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ftpCommand", (char*) ftpCommand, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ftpCommandEnhanced", (char*) ftpCommandEnhanced, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ftpDataConnGet", (char*) ftpDataConnGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ftpDataConnInit", (char*) ftpDataConnInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ftpDataConnInitPassiveMode", (char*) ftpDataConnInitPassiveMode, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ftpHookup", (char*) ftpHookup, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ftpLibDebugOptionsSet", (char*) ftpLibDebugOptionsSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ftpLibInit", (char*) ftpLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ftpLogin", (char*) ftpLogin, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ftpLs", (char*) ftpLs, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ftpReplyGet", (char*) ftpReplyGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ftpReplyGetEnhanced", (char*) ftpReplyGetEnhanced, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ftpReplyTimeout", (char*) &ftpReplyTimeout, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ftpTransientConfigGet", (char*) ftpTransientConfigGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ftpTransientConfigSet", (char*) ftpTransientConfigSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ftpTransientFatalInstall", (char*) ftpTransientFatalInstall, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ftpVerbose", (char*) &ftpVerbose, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ftpXfer", (char*) ftpXfer, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ftplDebug", (char*) &ftplDebug, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ftplPasvModeDisable", (char*) &ftplPasvModeDisable, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ftplTransientMaxRetryCount", (char*) &ftplTransientMaxRetryCount, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ftplTransientRetryInterval", (char*) &ftplTransientRetryInterval, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "fwrite", (char*) fwrite, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "genPhyDevRegistration", (char*) &genPhyDevRegistration, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "genPhyRegister", (char*) genPhyRegister, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "genericLayeredExcHdlrInstall", (char*) genericLayeredExcHdlrInstall, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "getSymNames", (char*) getSymNames, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "getc", (char*) getc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "getchar", (char*) getchar, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "getcwd", (char*) getcwd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "getenv", (char*) getenv, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "gethostname", (char*) gethostname, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "getlhostbyaddr", (char*) getlhostbyaddr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "getlhostbyname", (char*) getlhostbyname, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "getpeername", (char*) getpeername, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "getpid", (char*) getpid, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "gets", (char*) gets, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "getsockname", (char*) getsockname, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "getsockopt", (char*) getsockopt, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "gettimeofday", (char*) gettimeofday, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "getw", (char*) getw, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "getwd", (char*) getwd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "globalNoStackFill", (char*) &globalNoStackFill, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "gmtime", (char*) gmtime, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "gmtime_r", (char*) gmtime_r, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "gpio0Resources", (char*) &gpio0Resources, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "gpioInputRead", (char*) gpioInputRead, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "gpioInputSelect", (char*) gpioInputSelect, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "gpioIntConnect", (char*) gpioIntConnect, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "gpioIntDisconnect", (char*) gpioIntDisconnect, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "gpioOutputSelect", (char*) gpioOutputSelect, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "gtaDesc", (char*) &gtaDesc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "gtbDesc", (char*) &gtbDesc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "h", (char*) h, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "handleContextGet", (char*) handleContextGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "handleContextGetBase", (char*) handleContextGetBase, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "handleContextSet", (char*) handleContextSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "handleInit", (char*) handleInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "handleShow", (char*) handleShow, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "handleShowConnect", (char*) handleShowConnect, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "handleTerminate", (char*) handleTerminate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "handleTypeGet", (char*) handleTypeGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "handleVerify", (char*) handleVerify, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "hardWareInterFaceBusInit", (char*) hardWareInterFaceBusInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "hardWareInterFaceInit", (char*) hardWareInterFaceInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "hashFuncIterScale", (char*) hashFuncIterScale, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "hashFuncModulo", (char*) hashFuncModulo, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "hashFuncMultiply", (char*) hashFuncMultiply, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "hashKeyCmp", (char*) hashKeyCmp, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "hashKeyStrCmp", (char*) hashKeyStrCmp, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "hashLibInit", (char*) hashLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "hashTblCreate", (char*) hashTblCreate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "hashTblDelete", (char*) hashTblDelete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "hashTblDestroy", (char*) hashTblDestroy, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "hashTblEach", (char*) hashTblEach, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "hashTblFind", (char*) hashTblFind, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "hashTblInit", (char*) hashTblInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "hashTblPut", (char*) hashTblPut, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "hashTblRemove", (char*) hashTblRemove, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "hashTblTerminate", (char*) hashTblTerminate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "hcfDeviceGet", (char*) hcfDeviceGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "hcfDeviceList", (char*) &hcfDeviceList, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "hcfDeviceNum", (char*) &hcfDeviceNum, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "hdprintf", (char*) hdprintf, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "help", (char*) help, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "hookAddToHead", (char*) hookAddToHead, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "hookAddToTail", (char*) hookAddToTail, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "hookDelete", (char*) hookDelete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "hookFind", (char*) hookFind, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "hookShow", (char*) hookShow, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "hookTblAddToHead", (char*) hookTblAddToHead, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "hookTblAddToTail", (char*) hookTblAddToTail, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "hookTblDelete", (char*) hookTblDelete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "hookTblFind", (char*) hookTblFind, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "hookTblInit", (char*) hookTblInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "hookTblShow", (char*) hookTblShow, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "hookTblUpdatersUnpend", (char*) hookTblUpdatersUnpend, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "hostAdd", (char*) hostAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "hostDelete", (char*) hostDelete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "hostGetByAddr", (char*) hostGetByAddr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "hostGetByName", (char*) hostGetByName, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "hostInitFlag", (char*) &hostInitFlag, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "hostList", (char*) &hostList, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "hostListSem", (char*) &hostListSem, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "hostTblInit", (char*) hostTblInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "hostTblSearchByAddr", (char*) hostTblSearchByAddr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "hostTblSearchByAddr2", (char*) hostTblSearchByAddr2, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "hostTblSearchByName", (char*) hostTblSearchByName, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "hostTblSearchByName2", (char*) hostTblSearchByName2, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "hostTblSetup", (char*) hostTblSetup, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "hostentAlloc", (char*) hostentAlloc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "hostentFree", (char*) hostentFree, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "hostnameSetup", (char*) hostnameSetup, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "hrfsDiskFormat", (char*) hrfsDiskFormat, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "hrfsFmtRtn", (char*) &hrfsFmtRtn, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "hwMemAlloc", (char*) hwMemAlloc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "hwMemFree", (char*) hwMemFree, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "hwMemLibInit", (char*) hwMemLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "hwMemPool", (char*) &hwMemPool, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "hwMemPoolCreate", (char*) hwMemPoolCreate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "i", (char*) i, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iam", (char*) iam, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "inDesc", (char*) &inDesc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "inFullVxWorksImage", (char*) &inFullVxWorksImage, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "incrNumPageTables", (char*) &incrNumPageTables, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "incrNumRegionTables", (char*) &incrNumRegionTables, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "index", (char*) index, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "inet_addr", (char*) inet_addr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "inet_aton", (char*) inet_aton, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "inet_ntoa", (char*) inet_ntoa, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "initApplLogger", (char*) initApplLogger, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "initNumPageTables", (char*) &initNumPageTables, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "initNumRegionTables", (char*) &initNumRegionTables, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "initStaticTlbNum", (char*) &initStaticTlbNum, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "instParamModify_desc", (char*) &instParamModify_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "intArchConnect", (char*) intArchConnect, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "intArchDisconnect", (char*) intArchDisconnect, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "intCnt", (char*) &intCnt, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "intConnect", (char*) intConnect, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "intContext", (char*) intContext, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "intCount", (char*) intCount, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "intCpuLock", (char*) intCpuLock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "intCpuMicroLock", (char*) intCpuMicroLock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "intCpuMicroUnlock", (char*) intCpuMicroUnlock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "intCpuUnlock", (char*) intCpuUnlock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "intCrtEnt", (char*) intCrtEnt, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "intCrtExit", (char*) intCrtExit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "intCtlrChainISR", (char*) intCtlrChainISR, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "intCtlrHwConfGet", (char*) intCtlrHwConfGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "intCtlrISRAdd", (char*) intCtlrISRAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "intCtlrISRDisable", (char*) intCtlrISRDisable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "intCtlrISREnable", (char*) intCtlrISREnable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "intCtlrISRRemove", (char*) intCtlrISRRemove, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "intCtlrPinFind", (char*) intCtlrPinFind, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "intCtlrStrayISR", (char*) intCtlrStrayISR, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "intCtlrTableArgGet", (char*) intCtlrTableArgGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "intCtlrTableCreate", (char*) intCtlrTableCreate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "intCtlrTableFlagsGet", (char*) intCtlrTableFlagsGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "intCtlrTableFlagsSet", (char*) intCtlrTableFlagsSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "intCtlrTableIsrGet", (char*) intCtlrTableIsrGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "intCtlrTableUserSet", (char*) intCtlrTableUserSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "intDisable", (char*) intDisable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "intDisconnect", (char*) intDisconnect, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "intEnable", (char*) intEnable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "intEnt", (char*) intEnt, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "intExit", (char*) intExit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "intLevelSet", (char*) intLevelSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "intLock", (char*) intLock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "intLockLevelGet", (char*) intLockLevelGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "intLockLevelSet", (char*) intLockLevelSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "intRegsLock", (char*) intRegsLock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "intRegsUnlock", (char*) intRegsUnlock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "intRestrict", (char*) intRestrict, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "intUnlock", (char*) intUnlock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "intVecBaseGet", (char*) intVecBaseGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "intVecBaseSet", (char*) intVecBaseSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "intVecGet", (char*) intVecGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "intVecSet", (char*) intVecSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "intVecTableWriteProtect", (char*) intVecTableWriteProtect, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ioDefPath", (char*) &ioDefPath, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "ioDefPathCat", (char*) ioDefPathCat, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ioDefPathGet", (char*) ioDefPathGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ioDefPathSet", (char*) ioDefPathSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ioDefPathValidate", (char*) ioDefPathValidate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ioFullFileNameGet", (char*) ioFullFileNameGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ioGlobalStdGet", (char*) ioGlobalStdGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ioGlobalStdSet", (char*) ioGlobalStdSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ioHelp", (char*) ioHelp, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ioMaxLinkLevels", (char*) &ioMaxLinkLevels, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ioPxCreateOrOpen", (char*) ioPxCreateOrOpen, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ioTaskStdGet", (char*) ioTaskStdGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ioTaskStdSet", (char*) ioTaskStdSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ioctl", (char*) ioctl, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosClose", (char*) iosClose, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosCreate", (char*) iosCreate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosDelete", (char*) iosDelete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosDevAdd", (char*) iosDevAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosDevCheck", (char*) iosDevCheck, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosDevDelCallback", (char*) iosDevDelCallback, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosDevDelDrv", (char*) iosDevDelDrv, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosDevDelete", (char*) iosDevDelete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosDevFind", (char*) iosDevFind, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosDevMatch", (char*) iosDevMatch, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosDevReplace", (char*) iosDevReplace, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosDevReplaceExt", (char*) iosDevReplaceExt, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosDevResume", (char*) iosDevResume, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosDevShow", (char*) iosDevShow, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosDevSuspend", (char*) iosDevSuspend, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosDrvInit", (char*) iosDrvInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosDrvInstall", (char*) iosDrvInstall, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosDrvIoctl", (char*) iosDrvIoctl, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosDrvIoctlMemValSet", (char*) iosDrvIoctlMemValSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosDrvRemove", (char*) iosDrvRemove, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosDrvShow", (char*) iosDrvShow, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosDvList", (char*) &iosDvList, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "iosFdDevFind", (char*) iosFdDevFind, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosFdDrvValue", (char*) iosFdDrvValue, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosFdEntryGet", (char*) iosFdEntryGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosFdEntryIoctl", (char*) iosFdEntryIoctl, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosFdEntryPool", (char*) &iosFdEntryPool, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "iosFdEntryReturn", (char*) iosFdEntryReturn, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosFdEntrySet", (char*) iosFdEntrySet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosFdFree", (char*) iosFdFree, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosFdMap", (char*) iosFdMap, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosFdMaxFiles", (char*) iosFdMaxFiles, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosFdNew", (char*) iosFdNew, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosFdSet", (char*) iosFdSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosFdShow", (char*) iosFdShow, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosFdTable", (char*) &iosFdTable, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "iosFdTableLock", (char*) iosFdTableLock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosFdTableUnlock", (char*) iosFdTableUnlock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosFdValue", (char*) iosFdValue, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosInit", (char*) iosInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosIoctl", (char*) iosIoctl, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosIoctlInternal", (char*) iosIoctlInternal, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosLibInitialized", (char*) &iosLibInitialized, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "iosLock", (char*) iosLock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosMaxDrivers", (char*) &iosMaxDrivers, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "iosMaxFiles", (char*) &iosMaxFiles, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "iosNextDevGet", (char*) iosNextDevGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosOpen", (char*) iosOpen, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosPathFdEntryIoctl", (char*) iosPathFdEntryIoctl, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosPathLibInit", (char*) iosPathLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosPse52Mode", (char*) &iosPse52Mode, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "iosPxLibInit", (char*) iosPxLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosPxLibInitialized", (char*) &iosPxLibInitialized, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "iosRead", (char*) iosRead, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosRmvFdEntrySet", (char*) iosRmvFdEntrySet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosRmvLibInit", (char*) iosRmvLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosRtpDefPathGet", (char*) iosRtpDefPathGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosRtpDefPathSet", (char*) iosRtpDefPathSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosRtpDevCloseOrInvalidate", (char*) iosRtpDevCloseOrInvalidate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosRtpFdEntryMap", (char*) iosRtpFdEntryMap, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosRtpFdInvalid", (char*) iosRtpFdInvalid, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosRtpFdReserve", (char*) iosRtpFdReserve, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosRtpFdReserveCntrl", (char*) iosRtpFdReserveCntrl, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosRtpFdSetup", (char*) iosRtpFdSetup, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosRtpFdShow", (char*) iosRtpFdShow, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosRtpFdTableGet", (char*) iosRtpFdTableGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosRtpFdTableSizeGet", (char*) iosRtpFdTableSizeGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosRtpFdTblEntryGet", (char*) iosRtpFdTblEntryGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosRtpFdUnmap2", (char*) iosRtpFdUnmap2, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosRtpIoTableSizeGet", (char*) iosRtpIoTableSizeGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosRtpIoTableSizeSet", (char*) iosRtpIoTableSizeSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosRtpLibInit", (char*) iosRtpLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosRtpTableSizeSet", (char*) iosRtpTableSizeSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosShowInit", (char*) iosShowInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosTaskCwdGet", (char*) iosTaskCwdGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosTaskCwdSet", (char*) iosTaskCwdSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosUnlock", (char*) iosUnlock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iosWrite", (char*) iosWrite, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ip_inaddr_any", (char*) &ip_inaddr_any, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ipcomNetTask", (char*) ipcomNetTask, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_accept", (char*) ipcom_accept, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_accept_usr", (char*) ipcom_accept_usr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_asctime_r_vxworks", (char*) ipcom_asctime_r_vxworks, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_atomic_add", (char*) ipcom_atomic_add, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_atomic_add_and_return", (char*) ipcom_atomic_add_and_return, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_atomic_dec", (char*) ipcom_atomic_dec, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_atomic_get", (char*) ipcom_atomic_get, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_atomic_inc", (char*) ipcom_atomic_inc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_atomic_ptr_cas", (char*) ipcom_atomic_ptr_cas, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_atomic_set", (char*) ipcom_atomic_set, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_atomic_sub", (char*) ipcom_atomic_sub, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_atomic_sub_and_return", (char*) ipcom_atomic_sub_and_return, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_bind", (char*) ipcom_bind, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_bind_usr", (char*) ipcom_bind_usr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_block_create", (char*) ipcom_block_create, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_block_delete", (char*) ipcom_block_delete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_block_post", (char*) ipcom_block_post, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_block_reader_create", (char*) ipcom_block_reader_create, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_block_wait", (char*) ipcom_block_wait, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_cache_line_size", (char*) ipcom_cache_line_size, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_calloc", (char*) ipcom_calloc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_chdir", (char*) ipcom_chdir, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_clearenv", (char*) ipcom_clearenv, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_closedir", (char*) ipcom_closedir, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_closelog", (char*) ipcom_closelog, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_cmd_if_caps_to_str", (char*) ipcom_cmd_if_caps_to_str, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_cmd_if_flags_to_str", (char*) ipcom_cmd_if_flags_to_str, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_cmd_key_to_str", (char*) ipcom_cmd_key_to_str, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_cmd_sockaddr_to_str", (char*) ipcom_cmd_sockaddr_to_str, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_cmd_str_to_key", (char*) ipcom_cmd_str_to_key, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_cmd_str_to_key2", (char*) ipcom_cmd_str_to_key2, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_cmsg_nxthdr", (char*) ipcom_cmsg_nxthdr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_conf_max_link_hdr_size", (char*) &ipcom_conf_max_link_hdr_size, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "ipcom_conf_pkt_pool_buffer_alignment", (char*) &ipcom_conf_pkt_pool_buffer_alignment, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ipcom_conf_pkt_pool_buffer_extra_space", (char*) &ipcom_conf_pkt_pool_buffer_extra_space, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ipcom_configure", (char*) ipcom_configure, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_configure_reserved_cpus", (char*) ipcom_configure_reserved_cpus, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_connect", (char*) ipcom_connect, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_connect_usr", (char*) ipcom_connect_usr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_cpu_is_online", (char*) ipcom_cpu_is_online, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_cpu_reserved_alloc", (char*) ipcom_cpu_reserved_alloc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_cpu_reserved_free", (char*) ipcom_cpu_reserved_free, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_create", (char*) ipcom_create, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_ctime_r_vxworks", (char*) ipcom_ctime_r_vxworks, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_dns", (char*) &ipcom_dns, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "ipcom_dns_init_ok", (char*) &ipcom_dns_init_ok, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ipcom_drv_eth_create", (char*) ipcom_drv_eth_create, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_drv_eth_exit", (char*) ipcom_drv_eth_exit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_drv_eth_filter_add_mcast_addr", (char*) ipcom_drv_eth_filter_add_mcast_addr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_drv_eth_filter_exit", (char*) ipcom_drv_eth_filter_exit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_drv_eth_filter_init", (char*) ipcom_drv_eth_filter_init, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_drv_eth_filter_remove_mcast_addr", (char*) ipcom_drv_eth_filter_remove_mcast_addr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_drv_eth_init", (char*) ipcom_drv_eth_init, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_drv_eth_ip_attach", (char*) ipcom_drv_eth_ip_attach, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_errno_get", (char*) ipcom_errno_get, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_errno_set", (char*) ipcom_errno_set, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_fclose", (char*) ipcom_fclose, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_fdopen", (char*) ipcom_fdopen, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_feof", (char*) ipcom_feof, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_ferror", (char*) ipcom_ferror, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_fflush", (char*) ipcom_fflush, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_fileclose", (char*) ipcom_fileclose, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_fileno", (char*) ipcom_fileno, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_fileopen", (char*) ipcom_fileopen, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_fileread", (char*) ipcom_fileread, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_filewrite", (char*) ipcom_filewrite, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_fopen", (char*) ipcom_fopen, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_forwarder_ipsec_sa_cache_size", (char*) &ipcom_forwarder_ipsec_sa_cache_size, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ipcom_forwarder_ipsec_single_output_queue", (char*) &ipcom_forwarder_ipsec_single_output_queue, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ipcom_fread", (char*) ipcom_fread, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_free", (char*) ipcom_free, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_freeaddrinfo", (char*) ipcom_freeaddrinfo, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_freeaddrinfo2", (char*) ipcom_freeaddrinfo2, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_fseek", (char*) ipcom_fseek, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_fstat", (char*) ipcom_fstat, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_ftell", (char*) ipcom_ftell, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_fwrite", (char*) ipcom_fwrite, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_get_cpuid", (char*) ipcom_get_cpuid, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_get_ip_if_name", (char*) ipcom_get_ip_if_name, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_get_start_shell_ptr", (char*) ipcom_get_start_shell_ptr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_get_true_cpuid", (char*) ipcom_get_true_cpuid, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_getaddrinfo", (char*) ipcom_getaddrinfo, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_getaddrinfo2", (char*) ipcom_getaddrinfo2, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_getcwd", (char*) ipcom_getcwd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_getenv", (char*) ipcom_getenv, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_getenv_as_int", (char*) ipcom_getenv_as_int, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_gethostbyaddr", (char*) ipcom_gethostbyaddr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_gethostbyaddr_r", (char*) ipcom_gethostbyaddr_r, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_gethostbyaddrlocal", (char*) ipcom_gethostbyaddrlocal, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_gethostbyname", (char*) ipcom_gethostbyname, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_gethostbyname_r", (char*) ipcom_gethostbyname_r, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_gethostbynamelocal", (char*) ipcom_gethostbynamelocal, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_getlogmask_on", (char*) ipcom_getlogmask_on, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_getnameinfo", (char*) ipcom_getnameinfo, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_getnameinfo2", (char*) ipcom_getnameinfo2, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_getopt", (char*) ipcom_getopt, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_getopt_clear", (char*) ipcom_getopt_clear, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_getopt_clear_r", (char*) ipcom_getopt_clear_r, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_getopt_data", (char*) &ipcom_getopt_data, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "ipcom_getopt_r", (char*) ipcom_getopt_r, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_getpeername", (char*) ipcom_getpeername, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_getpeername_usr", (char*) ipcom_getpeername_usr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_getpid", (char*) ipcom_getpid, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_getprotobyname", (char*) ipcom_getprotobyname, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_getprotobynumber", (char*) ipcom_getprotobynumber, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_getservbyname", (char*) ipcom_getservbyname, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_getservbyport", (char*) ipcom_getservbyport, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_getsockaddrbyaddr", (char*) ipcom_getsockaddrbyaddr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_getsockaddrbyaddrname", (char*) ipcom_getsockaddrbyaddrname, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_getsockname", (char*) ipcom_getsockname, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_getsockname_usr", (char*) ipcom_getsockname_usr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_getsockopt", (char*) ipcom_getsockopt, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_getsockopt_usr", (char*) ipcom_getsockopt_usr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_gettimeofday", (char*) ipcom_gettimeofday, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_h_errno", (char*) &ipcom_h_errno, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "ipcom_hash_add", (char*) ipcom_hash_add, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_hash_delete", (char*) ipcom_hash_delete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_hash_for_each", (char*) ipcom_hash_for_each, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_hash_get", (char*) ipcom_hash_get, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_hash_new", (char*) ipcom_hash_new, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_hash_remove", (char*) ipcom_hash_remove, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_hash_update", (char*) ipcom_hash_update, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_heap_sort", (char*) ipcom_heap_sort, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_if_attach", (char*) ipcom_if_attach, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_if_attach_and_lock", (char*) ipcom_if_attach_and_lock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_if_changelinkaddr", (char*) ipcom_if_changelinkaddr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_if_changemtu", (char*) ipcom_if_changemtu, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_if_changename", (char*) ipcom_if_changename, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_if_detach", (char*) ipcom_if_detach, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_if_free", (char*) ipcom_if_free, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_if_freenameindex", (char*) ipcom_if_freenameindex, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_if_indextoname", (char*) ipcom_if_indextoname, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_if_malloc", (char*) ipcom_if_malloc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_if_nameindex", (char*) ipcom_if_nameindex, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_if_nametoindex", (char*) ipcom_if_nametoindex, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_if_nametonetif", (char*) ipcom_if_nametonetif, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_ima_stack_task", (char*) ipcom_ima_stack_task, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_in_checksum", (char*) ipcom_in_checksum, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_in_checksum_finish", (char*) ipcom_in_checksum_finish, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_in_checksum_memcpy", (char*) ipcom_in_checksum_memcpy, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_in_checksum_memcpy_asm", (char*) ipcom_in_checksum_memcpy_asm, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_in_checksum_pkt", (char*) ipcom_in_checksum_pkt, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_in_checksum_update", (char*) ipcom_in_checksum_update, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_in_checksum_update2_asm", (char*) ipcom_in_checksum_update2_asm, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_in_checksum_update_pkt", (char*) ipcom_in_checksum_update_pkt, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_inet_addr", (char*) ipcom_inet_addr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_inet_aton", (char*) ipcom_inet_aton, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_inet_ntoa", (char*) ipcom_inet_ntoa, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_inet_ntop", (char*) ipcom_inet_ntop, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_inet_pton", (char*) ipcom_inet_pton, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_init_config_vars", (char*) ipcom_init_config_vars, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_initd", (char*) ipcom_initd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_interrupt_disable", (char*) ipcom_interrupt_disable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_interrupt_enable", (char*) ipcom_interrupt_enable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_ipc_close", (char*) ipcom_ipc_close, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_ipc_free", (char*) ipcom_ipc_free, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_ipc_install", (char*) ipcom_ipc_install, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_ipc_isopen", (char*) ipcom_ipc_isopen, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_ipc_malloc", (char*) ipcom_ipc_malloc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_ipc_open", (char*) ipcom_ipc_open, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_ipc_receive", (char*) ipcom_ipc_receive, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_ipc_send", (char*) ipcom_ipc_send, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_ipc_size", (char*) ipcom_ipc_size, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_ipc_uninstall", (char*) ipcom_ipc_uninstall, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_ipd_datamsg", (char*) ipcom_ipd_datamsg, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_ipd_exit", (char*) ipcom_ipd_exit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_ipd_init", (char*) ipcom_ipd_init, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_ipd_input", (char*) ipcom_ipd_input, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_ipd_input_process", (char*) ipcom_ipd_input_process, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_ipd_kill", (char*) ipcom_ipd_kill, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_ipd_products", (char*) &ipcom_ipd_products, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ipcom_ipd_read", (char*) ipcom_ipd_read, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_ipd_reconfigure", (char*) ipcom_ipd_reconfigure, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_ipd_send", (char*) ipcom_ipd_send, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_ipd_sendmsg", (char*) ipcom_ipd_sendmsg, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_ipd_start", (char*) ipcom_ipd_start, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_ipproto_name", (char*) ipcom_ipproto_name, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_is_pid_valid", (char*) ipcom_is_pid_valid, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_is_stack_task", (char*) ipcom_is_stack_task, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_job_queue_init", (char*) ipcom_job_queue_init, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_job_queue_pipe_schedule", (char*) ipcom_job_queue_pipe_schedule, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_job_queue_schedule", (char*) ipcom_job_queue_schedule, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_job_queue_schedule_singleton", (char*) ipcom_job_queue_schedule_singleton, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_job_queue_schedule_singleton_delayed", (char*) ipcom_job_queue_schedule_singleton_delayed, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_job_queue_singleton_delete", (char*) ipcom_job_queue_singleton_delete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_job_queue_singleton_new", (char*) ipcom_job_queue_singleton_new, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_key_db_file_root", (char*) &ipcom_key_db_file_root, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ipcom_list_first", (char*) ipcom_list_first, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_list_init", (char*) ipcom_list_init, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_list_insert_after", (char*) ipcom_list_insert_after, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_list_insert_before", (char*) ipcom_list_insert_before, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_list_insert_first", (char*) ipcom_list_insert_first, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_list_insert_last", (char*) ipcom_list_insert_last, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_list_last", (char*) ipcom_list_last, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_list_next", (char*) ipcom_list_next, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_list_prev", (char*) ipcom_list_prev, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_list_remove", (char*) ipcom_list_remove, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_list_unlink_head", (char*) ipcom_list_unlink_head, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_listen", (char*) ipcom_listen, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_listen_usr", (char*) ipcom_listen_usr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_lseek", (char*) ipcom_lseek, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_malloc", (char*) ipcom_malloc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_mask_to_prefixlen", (char*) ipcom_mask_to_prefixlen, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_mb", (char*) ipcom_mb, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_mcmd_ifconfig", (char*) ipcom_mcmd_ifconfig, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_mcmd_route", (char*) ipcom_mcmd_route, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_memcpy_asm", (char*) ipcom_memcpy_asm, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_memory_pool_new", (char*) ipcom_memory_pool_new, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_microtime", (char*) ipcom_microtime, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_millisleep", (char*) ipcom_millisleep, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_mipc_addr2offset", (char*) ipcom_mipc_addr2offset, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_mipc_offset2addr", (char*) ipcom_mipc_offset2addr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_mkdir", (char*) ipcom_mkdir, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_mutex_create", (char*) ipcom_mutex_create, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_mutex_delete", (char*) ipcom_mutex_delete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_mutex_lock", (char*) ipcom_mutex_lock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_mutex_unlock", (char*) ipcom_mutex_unlock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_null_pool_create", (char*) &ipcom_null_pool_create, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ipcom_num_configured_cpus", (char*) ipcom_num_configured_cpus, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_num_online_cpus", (char*) ipcom_num_online_cpus, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_observer_notify", (char*) ipcom_observer_notify, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_once", (char*) ipcom_once, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_once_mutex", (char*) &ipcom_once_mutex, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "ipcom_opendir", (char*) ipcom_opendir, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_openlog", (char*) ipcom_openlog, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_parse_argstr", (char*) ipcom_parse_argstr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_pipe_attr_init", (char*) ipcom_pipe_attr_init, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_pipe_free", (char*) ipcom_pipe_free, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_pipe_generic_create", (char*) ipcom_pipe_generic_create, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_pipe_job_queue_create", (char*) ipcom_pipe_job_queue_create, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_pipe_lockless_create", (char*) ipcom_pipe_lockless_create, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_pipe_new", (char*) ipcom_pipe_new, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_pipe_register_type", (char*) ipcom_pipe_register_type, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_pipe_tv_zero", (char*) &ipcom_pipe_tv_zero, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ipcom_pkt_alloc_info", (char*) ipcom_pkt_alloc_info, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_pkt_copy_info", (char*) ipcom_pkt_copy_info, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_pkt_create", (char*) ipcom_pkt_create, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_pkt_create_info", (char*) ipcom_pkt_create_info, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_pkt_free", (char*) ipcom_pkt_free, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_pkt_get_info", (char*) ipcom_pkt_get_info, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_pkt_get_info_and_size", (char*) ipcom_pkt_get_info_and_size, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_pkt_get_info_safe", (char*) ipcom_pkt_get_info_safe, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_pkt_hdrspace", (char*) ipcom_pkt_hdrspace, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_pkt_input", (char*) ipcom_pkt_input, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_pkt_input_queue", (char*) ipcom_pkt_input_queue, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_pkt_make_linear", (char*) ipcom_pkt_make_linear, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_pkt_malloc", (char*) ipcom_pkt_malloc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_pkt_output_done", (char*) ipcom_pkt_output_done, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_pkt_set_info", (char*) ipcom_pkt_set_info, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_pkt_sg_get_data", (char*) ipcom_pkt_sg_get_data, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_pkt_trim_head", (char*) ipcom_pkt_trim_head, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_pkt_trim_tail", (char*) ipcom_pkt_trim_tail, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_pktbuf_free", (char*) ipcom_pktbuf_free, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_port", (char*) &ipcom_port, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ipcom_port_pkt_hdr_extra_size", (char*) &ipcom_port_pkt_hdr_extra_size, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ipcom_pqueue_delete", (char*) ipcom_pqueue_delete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_pqueue_for_each", (char*) ipcom_pqueue_for_each, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_pqueue_get", (char*) ipcom_pqueue_get, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_pqueue_get_next", (char*) ipcom_pqueue_get_next, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_pqueue_insert", (char*) ipcom_pqueue_insert, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_pqueue_new", (char*) ipcom_pqueue_new, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_pqueue_nop_store_index", (char*) ipcom_pqueue_nop_store_index, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_pqueue_remove", (char*) ipcom_pqueue_remove, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_pqueue_remove_next", (char*) ipcom_pqueue_remove_next, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_pqueue_size", (char*) ipcom_pqueue_size, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_preempt_disable", (char*) ipcom_preempt_disable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_preempt_enable", (char*) ipcom_preempt_enable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_priority_map", (char*) &ipcom_priority_map, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ipcom_proc_acreate", (char*) ipcom_proc_acreate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_proc_attr_init", (char*) ipcom_proc_attr_init, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_proc_cpu_affinity_clr", (char*) ipcom_proc_cpu_affinity_clr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_proc_cpu_affinity_get", (char*) ipcom_proc_cpu_affinity_get, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_proc_cpu_affinity_set", (char*) ipcom_proc_cpu_affinity_set, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_proc_cpu_affinity_set_all", (char*) ipcom_proc_cpu_affinity_set_all, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_proc_create", (char*) ipcom_proc_create, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_proc_exit", (char*) ipcom_proc_exit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_proc_find", (char*) ipcom_proc_find, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_proc_free", (char*) ipcom_proc_free, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_proc_getprio", (char*) ipcom_proc_getprio, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_proc_init", (char*) ipcom_proc_init, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_proc_malloc", (char*) ipcom_proc_malloc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_proc_self", (char*) ipcom_proc_self, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_proc_setprio", (char*) ipcom_proc_setprio, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_proc_vr_get", (char*) ipcom_proc_vr_get, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_proc_vr_set", (char*) ipcom_proc_vr_set, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_proc_yield", (char*) ipcom_proc_yield, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_rand", (char*) ipcom_rand, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_random", (char*) ipcom_random, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_random_init", (char*) ipcom_random_init, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_random_seed_state", (char*) ipcom_random_seed_state, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_readdir", (char*) ipcom_readdir, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_realloc", (char*) ipcom_realloc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_recv", (char*) ipcom_recv, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_recv_usr", (char*) ipcom_recv_usr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_recvfrom", (char*) ipcom_recvfrom, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_recvfrom_usr", (char*) ipcom_recvfrom_usr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_recvmsg", (char*) ipcom_recvmsg, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_recvmsg_usr", (char*) ipcom_recvmsg_usr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_register_dev_name_mapping", (char*) ipcom_register_dev_name_mapping, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_rename", (char*) ipcom_rename, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_rewind", (char*) ipcom_rewind, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_rmb", (char*) ipcom_rmb, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_rmdir", (char*) ipcom_rmdir, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_route_add", (char*) ipcom_route_add, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_route_first_entry", (char*) ipcom_route_first_entry, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_route_free_table", (char*) ipcom_route_free_table, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_route_init_subtree_mask", (char*) ipcom_route_init_subtree_mask, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_route_key_cmp", (char*) ipcom_route_key_cmp, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_route_lookup", (char*) ipcom_route_lookup, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_route_new_table", (char*) ipcom_route_new_table, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_route_next_entry", (char*) ipcom_route_next_entry, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_route_remove", (char*) ipcom_route_remove, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_route_walk_tree", (char*) ipcom_route_walk_tree, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_route_walk_tree_backwards", (char*) ipcom_route_walk_tree_backwards, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_rtp_tcb_index_get_fd", (char*) ipcom_rtp_tcb_index_get_fd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_run_cmd", (char*) ipcom_run_cmd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_sem_create", (char*) ipcom_sem_create, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_sem_delete", (char*) ipcom_sem_delete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_sem_flush", (char*) ipcom_sem_flush, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_sem_getvalue", (char*) ipcom_sem_getvalue, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_sem_interrupt_flush", (char*) ipcom_sem_interrupt_flush, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_sem_interrupt_post", (char*) ipcom_sem_interrupt_post, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_sem_post", (char*) ipcom_sem_post, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_sem_wait", (char*) ipcom_sem_wait, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_send", (char*) ipcom_send, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_send_usr", (char*) ipcom_send_usr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_sendmsg", (char*) ipcom_sendmsg, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_sendmsg_usr", (char*) ipcom_sendmsg_usr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_sendto", (char*) ipcom_sendto, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_sendto_usr", (char*) ipcom_sendto_usr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_set_add", (char*) ipcom_set_add, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_set_clone", (char*) ipcom_set_clone, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_set_contains", (char*) ipcom_set_contains, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_set_delete", (char*) ipcom_set_delete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_set_equal", (char*) ipcom_set_equal, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_set_for_each", (char*) ipcom_set_for_each, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_set_intersection", (char*) ipcom_set_intersection, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_set_new", (char*) ipcom_set_new, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_set_remove", (char*) ipcom_set_remove, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_set_remove_all", (char*) ipcom_set_remove_all, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_set_start_shell_ptr", (char*) ipcom_set_start_shell_ptr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_set_subtract", (char*) ipcom_set_subtract, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_set_union", (char*) ipcom_set_union, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_setenv", (char*) ipcom_setenv, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_setenv_as_int", (char*) ipcom_setenv_as_int, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_setlogfile", (char*) ipcom_setlogfile, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_setlogmask_on", (char*) ipcom_setlogmask_on, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_setlogudp", (char*) ipcom_setlogudp, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_setsockopt", (char*) ipcom_setsockopt, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_setsockopt_usr", (char*) ipcom_setsockopt_usr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_settimeofday", (char*) ipcom_settimeofday, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_shell_add_cmd", (char*) ipcom_shell_add_cmd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_shell_cmd_head", (char*) &ipcom_shell_cmd_head, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "ipcom_shell_find_cmd", (char*) ipcom_shell_find_cmd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_shell_find_cmd_hook", (char*) &ipcom_shell_find_cmd_hook, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ipcom_shell_fread", (char*) ipcom_shell_fread, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_shell_fwrite", (char*) ipcom_shell_fwrite, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_shell_remove_cmd", (char*) ipcom_shell_remove_cmd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_shutdown", (char*) ipcom_shutdown, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_shutdown_usr", (char*) ipcom_shutdown_usr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_slab_add", (char*) ipcom_slab_add, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_slab_alloc_from", (char*) ipcom_slab_alloc_from, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_slab_alloc_try", (char*) ipcom_slab_alloc_try, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_slab_free", (char*) ipcom_slab_free, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_slab_get_objects_per_slab", (char*) ipcom_slab_get_objects_per_slab, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_slab_memory_pool_list", (char*) &ipcom_slab_memory_pool_list, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "ipcom_slab_new", (char*) ipcom_slab_new, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_slab_once", (char*) &ipcom_slab_once, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ipcom_sleep", (char*) ipcom_sleep, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_sockaddr_to_prefixlen", (char*) ipcom_sockaddr_to_prefixlen, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_socket", (char*) ipcom_socket, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_socket_usr", (char*) ipcom_socket_usr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_socketclose", (char*) ipcom_socketclose, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_socketclose_usr", (char*) ipcom_socketclose_usr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_socketioctl", (char*) ipcom_socketioctl, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_socketioctl_usr", (char*) ipcom_socketioctl_usr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_socketread", (char*) ipcom_socketread, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_socketread_usr", (char*) ipcom_socketread_usr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_socketselect", (char*) ipcom_socketselect, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_socketselect_usr", (char*) ipcom_socketselect_usr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_socketwrite", (char*) ipcom_socketwrite, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_socketwrite_usr", (char*) ipcom_socketwrite_usr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_socketwritev", (char*) ipcom_socketwritev, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_socketwritev_usr", (char*) ipcom_socketwritev_usr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_spinlock_create", (char*) ipcom_spinlock_create, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_spinlock_delete", (char*) ipcom_spinlock_delete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_spinlock_lock", (char*) ipcom_spinlock_lock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_spinlock_unlock", (char*) ipcom_spinlock_unlock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_srand", (char*) ipcom_srand, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_srandom", (char*) ipcom_srandom, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_stackd_init", (char*) ipcom_stackd_init, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_start", (char*) ipcom_start, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_start_shell", (char*) ipcom_start_shell, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_start_shell_native", (char*) ipcom_start_shell_native, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_start_shell_ptr", (char*) &ipcom_start_shell_ptr, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ipcom_stat", (char*) ipcom_stat, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_stderr", (char*) ipcom_stderr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_stdin", (char*) ipcom_stdin, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_stdio_set_echo", (char*) ipcom_stdio_set_echo, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_stdout", (char*) ipcom_stdout, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_str_in_list", (char*) ipcom_str_in_list, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_strcasecmp", (char*) ipcom_strcasecmp, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_strerror", (char*) ipcom_strerror, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_strerror_r", (char*) ipcom_strerror_r, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_strncasecmp", (char*) ipcom_strncasecmp, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_sys_free", (char*) &ipcom_sys_free, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ipcom_sys_malloc", (char*) &ipcom_sys_malloc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ipcom_sysctl", (char*) ipcom_sysctl, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_syslog", (char*) ipcom_syslog, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_syslog_facility_names", (char*) &ipcom_syslog_facility_names, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ipcom_syslog_printf", (char*) ipcom_syslog_printf, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_syslog_priority_names", (char*) &ipcom_syslog_priority_names, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ipcom_syslogd_init", (char*) ipcom_syslogd_init, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_sysvar_add_observer", (char*) ipcom_sysvar_add_observer, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_sysvar_ext_setv", (char*) ipcom_sysvar_ext_setv, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_sysvar_for_each", (char*) ipcom_sysvar_for_each, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_sysvar_get", (char*) ipcom_sysvar_get, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_sysvar_get0", (char*) ipcom_sysvar_get0, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_sysvar_get_as_int", (char*) ipcom_sysvar_get_as_int, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_sysvar_get_as_int0", (char*) ipcom_sysvar_get_as_int0, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_sysvar_get_as_int_vr", (char*) ipcom_sysvar_get_as_int_vr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_sysvar_get_descriptive_int0", (char*) ipcom_sysvar_get_descriptive_int0, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_sysvar_get_descriptive_int_vr", (char*) ipcom_sysvar_get_descriptive_int_vr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_sysvar_getvr", (char*) ipcom_sysvar_getvr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_sysvar_remove_observer", (char*) ipcom_sysvar_remove_observer, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_sysvar_set", (char*) ipcom_sysvar_set, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_sysvar_set0", (char*) ipcom_sysvar_set0, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_sysvar_set_tree", (char*) ipcom_sysvar_set_tree, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_sysvar_setv", (char*) ipcom_sysvar_setv, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_sysvar_setvr", (char*) ipcom_sysvar_setvr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_sysvar_unset", (char*) ipcom_sysvar_unset, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_tmo2_cancel", (char*) ipcom_tmo2_cancel, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_tmo2_delete", (char*) ipcom_tmo2_delete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_tmo2_get", (char*) ipcom_tmo2_get, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_tmo2_new", (char*) ipcom_tmo2_new, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_tmo2_request", (char*) ipcom_tmo2_request, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_tmo2_request_timeval", (char*) ipcom_tmo2_request_timeval, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_tmo2_reset", (char*) ipcom_tmo2_reset, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_tmo2_select", (char*) ipcom_tmo2_select, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_tmo_cancel", (char*) ipcom_tmo_cancel, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_tmo_get", (char*) ipcom_tmo_get, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_tmo_request", (char*) ipcom_tmo_request, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_tmo_reset", (char*) ipcom_tmo_reset, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_unlink", (char*) ipcom_unlink, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_unsetenv", (char*) ipcom_unsetenv, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_usr_create", (char*) ipcom_usr_create, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_version", (char*) &ipcom_version, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ipcom_vsyslog", (char*) ipcom_vsyslog, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_vxshell_add_cmd", (char*) ipcom_vxshell_add_cmd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_vxshell_add_cmd_hook", (char*) &ipcom_vxshell_add_cmd_hook, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ipcom_vxworks_boot_devname", (char*) ipcom_vxworks_boot_devname, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_vxworks_loginStringGet", (char*) ipcom_vxworks_loginStringGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_vxworks_pkt_pool_hdr_ctor", (char*) ipcom_vxworks_pkt_pool_hdr_ctor, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_vxworks_proc_free", (char*) ipcom_vxworks_proc_free, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_vxworks_rtp_del_hook_add", (char*) ipcom_vxworks_rtp_del_hook_add, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_vxworks_task_del_hook", (char*) ipcom_vxworks_task_del_hook, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_windnet_socketwritev", (char*) ipcom_windnet_socketwritev, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_windnet_socklib_init", (char*) ipcom_windnet_socklib_init, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_windnet_socklib_sock_size", (char*) ipcom_windnet_socklib_sock_size, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_windnet_socklib_socket_free", (char*) ipcom_windnet_socklib_socket_free, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_windnet_socklib_socket_init", (char*) ipcom_windnet_socklib_socket_init, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_wmb", (char*) ipcom_wmb, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_wrapper_funcs", (char*) &ipcom_wrapper_funcs, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ipcom_wv_event_2", (char*) ipcom_wv_event_2, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_wv_marker_1", (char*) ipcom_wv_marker_1, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipcom_wv_marker_2", (char*) ipcom_wv_marker_2, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipiDesc", (char*) &ipiDesc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ipmcrypto_MD5_Final", (char*) ipmcrypto_MD5_Final, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipmcrypto_MD5_Init", (char*) ipmcrypto_MD5_Init, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipmcrypto_MD5_Update", (char*) ipmcrypto_MD5_Update, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipmcrypto_SHA1_Final", (char*) ipmcrypto_SHA1_Final, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipmcrypto_SHA1_Init", (char*) ipmcrypto_SHA1_Init, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipmcrypto_SHA1_Update", (char*) ipmcrypto_SHA1_Update, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipmcrypto_md5_calculate", (char*) ipmcrypto_md5_calculate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet", (char*) &ipnet, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ipnet_arp_ioctl", (char*) ipnet_arp_ioctl, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_arp_request", (char*) ipnet_arp_request, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_bool_map", (char*) &ipnet_bool_map, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ipnet_cmd_ifconfig_hook", (char*) &ipnet_cmd_ifconfig_hook, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ipnet_cmd_qc_hook", (char*) &ipnet_cmd_qc_hook, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ipnet_cmd_route_hook", (char*) &ipnet_cmd_route_hook, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ipnet_conf_boot_cmd", (char*) &ipnet_conf_boot_cmd, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ipnet_conf_cache_bufsiz", (char*) &ipnet_conf_cache_bufsiz, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ipnet_conf_inq_max", (char*) &ipnet_conf_inq_max, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ipnet_conf_interfaces", (char*) &ipnet_conf_interfaces, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ipnet_conf_ip4_min_mtu", (char*) &ipnet_conf_ip4_min_mtu, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ipnet_conf_ip6_min_mtu", (char*) &ipnet_conf_ip6_min_mtu, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ipnet_conf_link_layer", (char*) &ipnet_conf_link_layer, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ipnet_conf_max_dgram_frag_list_len", (char*) &ipnet_conf_max_dgram_frag_list_len, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ipnet_conf_max_reassembly_list_len", (char*) &ipnet_conf_max_reassembly_list_len, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ipnet_conf_max_sockets", (char*) &ipnet_conf_max_sockets, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ipnet_conf_memory_limit", (char*) &ipnet_conf_memory_limit, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ipnet_conf_reassembly_timeout", (char*) &ipnet_conf_reassembly_timeout, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ipnet_conf_update", (char*) ipnet_conf_update, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_configd_start", (char*) ipnet_configd_start, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_configure", (char*) ipnet_configure, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_create", (char*) ipnet_create, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_create_reassembled_packet", (char*) ipnet_create_reassembled_packet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_ctrl", (char*) ipnet_ctrl, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_ctrl_sig", (char*) ipnet_ctrl_sig, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_do_close", (char*) ipnet_do_close, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_dst_cache", (char*) &ipnet_dst_cache, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "ipnet_dst_cache_blackhole", (char*) ipnet_dst_cache_blackhole, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_dst_cache_blackhole_flow_spec", (char*) ipnet_dst_cache_blackhole_flow_spec, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_dst_cache_flush", (char*) ipnet_dst_cache_flush, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_dst_cache_flush_where_neigh_is", (char*) ipnet_dst_cache_flush_where_neigh_is, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_dst_cache_foreach", (char*) ipnet_dst_cache_foreach, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_dst_cache_init", (char*) ipnet_dst_cache_init, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_dst_cache_new", (char*) ipnet_dst_cache_new, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_dst_cache_select_best_rt", (char*) ipnet_dst_cache_select_best_rt, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_dst_cache_set_path_mtu", (char*) ipnet_dst_cache_set_path_mtu, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_dst_cache_set_rx_handler", (char*) ipnet_dst_cache_set_rx_handler, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_dst_cache_set_tx_handler", (char*) ipnet_dst_cache_set_tx_handler, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_eth_add_hdr", (char*) ipnet_eth_add_hdr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_eth_addr_broadcast", (char*) &ipnet_eth_addr_broadcast, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ipnet_eth_addr_broadcast_storage", (char*) &ipnet_eth_addr_broadcast_storage, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ipnet_eth_if_init", (char*) ipnet_eth_if_init, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_eth_update_mib2_lastchange", (char*) ipnet_eth_update_mib2_lastchange, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_flags", (char*) &ipnet_flags, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ipnet_flow_spec_domain", (char*) ipnet_flow_spec_domain, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_flow_spec_from_info", (char*) ipnet_flow_spec_from_info, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_flow_spec_from_sock", (char*) ipnet_flow_spec_from_sock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_frag_release_peer_info", (char*) ipnet_frag_release_peer_info, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_frag_set_peer_info", (char*) ipnet_frag_set_peer_info, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_frag_to_peer_info", (char*) ipnet_frag_to_peer_info, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_fragment_packet", (char*) ipnet_fragment_packet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_get_sock", (char*) ipnet_get_sock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_has_priv", (char*) ipnet_has_priv, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_icmp4_param_init", (char*) ipnet_icmp4_param_init, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_icmp4_send", (char*) ipnet_icmp4_send, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_icmp4_send_host_unreachable", (char*) ipnet_icmp4_send_host_unreachable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_icmp4_send_port_unreachable", (char*) ipnet_icmp4_send_port_unreachable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_if_can_detach", (char*) ipnet_if_can_detach, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_if_clean", (char*) ipnet_if_clean, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_if_clean_ppp_peer", (char*) ipnet_if_clean_ppp_peer, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_if_clean_snd_queue", (char*) ipnet_if_clean_snd_queue, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_if_detach", (char*) ipnet_if_detach, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_if_drv_ioctl", (char*) ipnet_if_drv_ioctl, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_if_drv_output", (char*) ipnet_if_drv_output, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_if_free", (char*) ipnet_if_free, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_if_get_index_array", (char*) ipnet_if_get_index_array, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_if_indextonetif", (char*) ipnet_if_indextonetif, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_if_init", (char*) ipnet_if_init, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_if_init_ppp_peer", (char*) ipnet_if_init_ppp_peer, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_if_link_ioctl", (char*) ipnet_if_link_ioctl, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_if_nametonetif", (char*) ipnet_if_nametonetif, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_if_output", (char*) ipnet_if_output, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_if_set_ipv4_ppp_peer", (char*) ipnet_if_set_ipv4_ppp_peer, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_if_to_sockaddr_dl", (char*) ipnet_if_to_sockaddr_dl, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_if_update_mib2_ifLastChange", (char*) ipnet_if_update_mib2_ifLastChange, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_igmpv3_report_change", (char*) ipnet_igmpv3_report_change, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_increase_hdr_space", (char*) ipnet_increase_hdr_space, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_ioctl_move_if_rt_to_vr", (char*) ipnet_ioctl_move_if_rt_to_vr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_ip4_acd_conflict", (char*) ipnet_ip4_acd_conflict, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_ip4_add_addr", (char*) ipnet_ip4_add_addr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_ip4_add_addr2", (char*) ipnet_ip4_add_addr2, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_ip4_add_route_table", (char*) ipnet_ip4_add_route_table, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_ip4_addr_to_sockaddr", (char*) ipnet_ip4_addr_to_sockaddr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_ip4_change_addr_mask", (char*) ipnet_ip4_change_addr_mask, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_ip4_configure_route_table", (char*) ipnet_ip4_configure_route_table, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_ip4_dst_cache_get", (char*) ipnet_ip4_dst_cache_get, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_ip4_dst_cache_rx_ctor", (char*) ipnet_ip4_dst_cache_rx_ctor, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_ip4_dst_unreachable", (char*) ipnet_ip4_dst_unreachable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_ip4_dst_unreachable_filter", (char*) ipnet_ip4_dst_unreachable_filter, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_ip4_flow_spec_from_info", (char*) ipnet_ip4_flow_spec_from_info, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_ip4_flow_spec_from_pkt", (char*) ipnet_ip4_flow_spec_from_pkt, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_ip4_fragment_timeout_peer", (char*) ipnet_ip4_fragment_timeout_peer, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_ip4_get_addr_entry", (char*) ipnet_ip4_get_addr_entry, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_ip4_get_addr_type", (char*) ipnet_ip4_get_addr_type, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_ip4_get_addr_type2", (char*) ipnet_ip4_get_addr_type2, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_ip4_get_mss", (char*) ipnet_ip4_get_mss, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_ip4_get_src_addr", (char*) ipnet_ip4_get_src_addr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_ip4_init", (char*) ipnet_ip4_init, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_ip4_input", (char*) ipnet_ip4_input, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_ip4_insert_addr_cache", (char*) ipnet_ip4_insert_addr_cache, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_ip4_is_part_of_same_pkt", (char*) ipnet_ip4_is_part_of_same_pkt, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_ip4_kioevent", (char*) ipnet_ip4_kioevent, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_ip4_netif_get_subbrd", (char*) ipnet_ip4_netif_get_subbrd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_ip4_pkt_with_iphdr_tx", (char*) ipnet_ip4_pkt_with_iphdr_tx, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_ip4_reg_transport_layer", (char*) ipnet_ip4_reg_transport_layer, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_ip4_remove_addr", (char*) ipnet_ip4_remove_addr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_ip4_remove_addr_cache", (char*) ipnet_ip4_remove_addr_cache, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_ip4_rfc1256_mode_update", (char*) ipnet_ip4_rfc1256_mode_update, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_ip4_rx", (char*) ipnet_ip4_rx, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_ip4_sendto", (char*) ipnet_ip4_sendto, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_ip4_transport_rx", (char*) ipnet_ip4_transport_rx, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_ip4_tx", (char*) ipnet_ip4_tx, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_is_loopback", (char*) ipnet_is_loopback, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_is_sock_exceptional", (char*) ipnet_is_sock_exceptional, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_is_sock_readable", (char*) ipnet_is_sock_readable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_is_sock_writable", (char*) ipnet_is_sock_writable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_is_stack_task", (char*) ipnet_is_stack_task, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_kioevent", (char*) ipnet_kioevent, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_loop_pkt_tx", (char*) ipnet_loop_pkt_tx, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_loopback_attach", (char*) ipnet_loopback_attach, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_loopback_get_netif", (char*) ipnet_loopback_get_netif, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_loopback_if_init", (char*) ipnet_loopback_if_init, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_mcast_build_if_filter", (char*) ipnet_mcast_build_if_filter, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_mcast_build_source_change_report", (char*) ipnet_mcast_build_source_change_report, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_mcast_clear", (char*) ipnet_mcast_clear, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_mcast_free", (char*) ipnet_mcast_free, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_mcast_init", (char*) ipnet_mcast_init, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_mcast_report_finish", (char*) ipnet_mcast_report_finish, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_mcast_schedule_membership_report", (char*) ipnet_mcast_schedule_membership_report, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_mcast_time_to_msec", (char*) ipnet_mcast_time_to_msec, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_msec_now", (char*) ipnet_msec_now, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_neigh_dad", (char*) ipnet_neigh_dad, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_neigh_flush", (char*) ipnet_neigh_flush, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_neigh_flush_all", (char*) ipnet_neigh_flush_all, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_neigh_foreach", (char*) ipnet_neigh_foreach, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_neigh_get", (char*) ipnet_neigh_get, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_neigh_init", (char*) ipnet_neigh_init, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_neigh_init_addr_observer", (char*) ipnet_neigh_init_addr_observer, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_neigh_invalidate", (char*) ipnet_neigh_invalidate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_neigh_is_probing", (char*) ipnet_neigh_is_probing, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_neigh_netif_discard", (char*) ipnet_neigh_netif_discard, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_neigh_probe", (char*) ipnet_neigh_probe, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_neigh_release", (char*) ipnet_neigh_release, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_neigh_set_blackhole", (char*) ipnet_neigh_set_blackhole, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_neigh_set_external", (char*) ipnet_neigh_set_external, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_neigh_set_gateway", (char*) ipnet_neigh_set_gateway, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_neigh_set_lifetime", (char*) ipnet_neigh_set_lifetime, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_neigh_set_lladdr", (char*) ipnet_neigh_set_lladdr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_neigh_set_local", (char*) ipnet_neigh_set_local, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_neigh_set_perm", (char*) ipnet_neigh_set_perm, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_neigh_set_publ", (char*) ipnet_neigh_set_publ, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_neigh_set_report", (char*) ipnet_neigh_set_report, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_neigh_set_silent", (char*) ipnet_neigh_set_silent, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_neigh_set_state", (char*) ipnet_neigh_set_state, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_netif_change_flags", (char*) ipnet_netif_change_flags, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_netif_is_ifproxy", (char*) ipnet_netif_is_ifproxy, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_netif_neigh_create", (char*) ipnet_netif_neigh_create, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_next_ephemeral_port", (char*) ipnet_next_ephemeral_port, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_noeth_if_init", (char*) ipnet_noeth_if_init, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_packet_getsockopt", (char*) ipnet_packet_getsockopt, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_packet_input", (char*) ipnet_packet_input, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_packet_setsockopt", (char*) ipnet_packet_setsockopt, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_pcap_ioctl", (char*) ipnet_pcap_ioctl, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_peer_info_addref", (char*) ipnet_peer_info_addref, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_peer_info_get", (char*) ipnet_peer_info_get, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_peer_info_init", (char*) ipnet_peer_info_init, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_peer_info_release", (char*) ipnet_peer_info_release, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_peer_info_timeout_cancel", (char*) ipnet_peer_info_timeout_cancel, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_peer_info_timeout_schedule", (char*) ipnet_peer_info_timeout_schedule, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_pipe_create", (char*) ipnet_pipe_create, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_pkt_clone", (char*) ipnet_pkt_clone, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_pkt_get_maxlen", (char*) ipnet_pkt_get_maxlen, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_pkt_input", (char*) ipnet_pkt_input, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_pkt_pool_drain", (char*) ipnet_pkt_pool_drain, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_pkt_pool_hdr_count_hint", (char*) ipnet_pkt_pool_hdr_count_hint, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_pkt_pool_hdr_ctor_func", (char*) &ipnet_pkt_pool_hdr_ctor_func, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ipnet_pkt_pool_init", (char*) ipnet_pkt_pool_init, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_pkt_pool_slab_desc", (char*) &ipnet_pkt_pool_slab_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ipnet_pkt_queue_delete", (char*) ipnet_pkt_queue_delete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_pkt_queue_fifo_template", (char*) ipnet_pkt_queue_fifo_template, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_pkt_queue_init", (char*) ipnet_pkt_queue_init, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_pkt_queue_new", (char*) ipnet_pkt_queue_new, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_ppp_if_init", (char*) ipnet_ppp_if_init, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_queue_received_packet", (char*) ipnet_queue_received_packet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_raw_input", (char*) ipnet_raw_input, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_reassembly", (char*) ipnet_reassembly, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_register_if_attach_handler", (char*) ipnet_register_if_attach_handler, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_release_sock", (char*) ipnet_release_sock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_resume_stack", (char*) ipnet_resume_stack, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_route_add", (char*) ipnet_route_add, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_route_add_table", (char*) ipnet_route_add_table, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_route_apply_mask", (char*) ipnet_route_apply_mask, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_route_cache_invalidate", (char*) ipnet_route_cache_invalidate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_route_create_mask", (char*) ipnet_route_create_mask, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_route_delete", (char*) ipnet_route_delete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_route_delete2", (char*) ipnet_route_delete2, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_route_delete_name", (char*) ipnet_route_delete_name, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_route_delete_table", (char*) ipnet_route_delete_table, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_route_delete_vr", (char*) ipnet_route_delete_vr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_route_first_entry", (char*) ipnet_route_first_entry, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_route_for_each_data_init", (char*) ipnet_route_for_each_data_init, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_route_get_name", (char*) ipnet_route_get_name, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_route_get_rtab", (char*) ipnet_route_get_rtab, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_route_get_table_ids", (char*) ipnet_route_get_table_ids, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_route_has_changed", (char*) ipnet_route_has_changed, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_route_has_expired", (char*) ipnet_route_has_expired, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_route_ip4_is_mcast_or_bcast", (char*) ipnet_route_ip4_is_mcast_or_bcast, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_route_is_virtual_router_valid", (char*) ipnet_route_is_virtual_router_valid, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_route_key_cmp", (char*) ipnet_route_key_cmp, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_route_key_to_sockaddr", (char*) ipnet_route_key_to_sockaddr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_route_lock", (char*) ipnet_route_lock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_route_lookup", (char*) ipnet_route_lookup, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_route_lookup_ecmp", (char*) ipnet_route_lookup_ecmp, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_route_next_entry", (char*) ipnet_route_next_entry, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_route_notify_func", (char*) ipnet_route_notify_func, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_route_policy_get_rule_chain", (char*) ipnet_route_policy_get_rule_chain, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_route_policy_ioctl", (char*) ipnet_route_policy_ioctl, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_route_raw_lookup", (char*) ipnet_route_raw_lookup, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_route_raw_lookup2", (char*) ipnet_route_raw_lookup2, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_route_remove_all", (char*) ipnet_route_remove_all, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_route_remove_all_cb_for_each_entry", (char*) ipnet_route_remove_all_cb_for_each_entry, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_route_remove_dynamic_rt_with_gw", (char*) ipnet_route_remove_dynamic_rt_with_gw, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_route_rotate_gateways", (char*) ipnet_route_rotate_gateways, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_route_set_lifetime", (char*) ipnet_route_set_lifetime, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_route_set_name", (char*) ipnet_route_set_name, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_route_set_rtab", (char*) ipnet_route_set_rtab, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_route_sockaddr_to_key", (char*) ipnet_route_sockaddr_to_key, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_route_unlock", (char*) ipnet_route_unlock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_route_vr_and_table_from_name", (char*) ipnet_route_vr_and_table_from_name, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_route_walk_tree", (char*) ipnet_route_walk_tree, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_routesock_addr_add", (char*) ipnet_routesock_addr_add, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_routesock_addr_conflict", (char*) ipnet_routesock_addr_conflict, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_routesock_addr_delete", (char*) ipnet_routesock_addr_delete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_routesock_if_announce", (char*) ipnet_routesock_if_announce, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_routesock_if_change", (char*) ipnet_routesock_if_change, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_routesock_neigh_resolve", (char*) ipnet_routesock_neigh_resolve, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_routesock_rt_add", (char*) ipnet_routesock_rt_add, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_routesock_rt_change", (char*) ipnet_routesock_rt_change, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_routesock_rt_delete", (char*) ipnet_routesock_rt_delete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_routesock_rt_miss", (char*) ipnet_routesock_rt_miss, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_routesock_rt_redirect", (char*) ipnet_routesock_rt_redirect, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sec_now", (char*) ipnet_sec_now, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sig_free", (char*) ipnet_sig_free, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sock_alloc_private_data", (char*) ipnet_sock_alloc_private_data, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sock_bind", (char*) ipnet_sock_bind, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sock_bind_addr_removed", (char*) ipnet_sock_bind_addr_removed, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sock_bind_to_port", (char*) ipnet_sock_bind_to_port, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sock_change_addr_entry", (char*) ipnet_sock_change_addr_entry, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sock_data_avail", (char*) ipnet_sock_data_avail, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sock_drop_all_multicast_memberships", (char*) ipnet_sock_drop_all_multicast_memberships, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sock_free", (char*) ipnet_sock_free, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sock_get", (char*) ipnet_sock_get, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sock_get_bound_port_size", (char*) ipnet_sock_get_bound_port_size, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sock_get_ops_handle", (char*) ipnet_sock_get_ops_handle, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sock_get_route", (char*) ipnet_sock_get_route, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sock_has_waiting_writers", (char*) ipnet_sock_has_waiting_writers, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sock_init", (char*) ipnet_sock_init, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sock_ip4_get_mcast_data", (char*) ipnet_sock_ip4_get_mcast_data, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sock_ip4_get_ops", (char*) ipnet_sock_ip4_get_ops, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sock_ip4_lookup", (char*) ipnet_sock_ip4_lookup, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sock_ip4_mcast_delete_data", (char*) ipnet_sock_ip4_mcast_delete_data, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sock_ip4_register", (char*) ipnet_sock_ip4_register, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sock_join_router_alert_chain", (char*) ipnet_sock_join_router_alert_chain, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sock_leave_router_alert_chain", (char*) ipnet_sock_leave_router_alert_chain, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sock_next", (char*) ipnet_sock_next, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sock_ops", (char*) &ipnet_sock_ops, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ipnet_sock_packet_lookup", (char*) ipnet_sock_packet_lookup, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sock_packet_register", (char*) ipnet_sock_packet_register, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sock_pkt_drain", (char*) ipnet_sock_pkt_drain, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sock_register_ops", (char*) ipnet_sock_register_ops, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sock_route_register", (char*) ipnet_sock_route_register, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sock_router_alert_pkt", (char*) ipnet_sock_router_alert_pkt, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sock_set_reachable", (char*) ipnet_sock_set_reachable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sock_set_unreachable", (char*) ipnet_sock_set_unreachable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sock_tcp_register", (char*) ipnet_sock_tcp_register, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sock_udp_register", (char*) ipnet_sock_udp_register, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sock_unbind", (char*) ipnet_sock_unbind, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sock_unconnect", (char*) ipnet_sock_unconnect, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sock_update_reuse", (char*) ipnet_sock_update_reuse, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sockopt_ip4_addr_to_netif", (char*) ipnet_sockopt_ip4_addr_to_netif, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_softirq_init", (char*) ipnet_softirq_init, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_softirq_schedule", (char*) ipnet_softirq_schedule, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_start", (char*) ipnet_start, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_stats_tcp_established", (char*) ipnet_stats_tcp_established, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_suspend_stack", (char*) ipnet_suspend_stack, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sys_accept", (char*) ipnet_sys_accept, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sys_bind", (char*) ipnet_sys_bind, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sys_checktcp", (char*) ipnet_sys_checktcp, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sys_connect", (char*) ipnet_sys_connect, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sys_get_ancillary_data", (char*) ipnet_sys_get_ancillary_data, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sys_getname", (char*) ipnet_sys_getname, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sys_getsockopt", (char*) ipnet_sys_getsockopt, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sys_if_attach", (char*) ipnet_sys_if_attach, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sys_if_changelinkaddr", (char*) ipnet_sys_if_changelinkaddr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sys_if_changemtu", (char*) ipnet_sys_if_changemtu, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sys_if_changename", (char*) ipnet_sys_if_changename, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sys_if_detach", (char*) ipnet_sys_if_detach, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sys_if_free", (char*) ipnet_sys_if_free, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sys_if_indexname", (char*) ipnet_sys_if_indexname, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sys_if_malloc", (char*) ipnet_sys_if_malloc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sys_if_nametonetif", (char*) ipnet_sys_if_nametonetif, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sys_listen", (char*) ipnet_sys_listen, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sys_poll", (char*) ipnet_sys_poll, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sys_poll_async", (char*) ipnet_sys_poll_async, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sys_savesignal", (char*) ipnet_sys_savesignal, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sys_sendmsg", (char*) ipnet_sys_sendmsg, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sys_setsockopt", (char*) ipnet_sys_setsockopt, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sys_shutdown", (char*) ipnet_sys_shutdown, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sys_socket", (char*) ipnet_sys_socket, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sys_socketclose", (char*) ipnet_sys_socketclose, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sys_socketioctl", (char*) ipnet_sys_socketioctl, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sys_sysctl", (char*) ipnet_sys_sysctl, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sysctl_create_ifinfo", (char*) ipnet_sysctl_create_ifinfo, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sysctl_if_add_addrs_one", (char*) ipnet_sysctl_if_add_addrs_one, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sysvar_get_as_int_vr", (char*) ipnet_sysvar_get_as_int_vr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sysvar_netif_get_as_int", (char*) ipnet_sysvar_netif_get_as_int, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_sysvar_netif_get_as_int_ex", (char*) ipnet_sysvar_netif_get_as_int_ex, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_timeout_cancel", (char*) ipnet_timeout_cancel, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_timeout_msec_until", (char*) ipnet_timeout_msec_until, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_timeout_schedule", (char*) ipnet_timeout_schedule, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_timeval_to_msec", (char*) ipnet_timeval_to_msec, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_try_free_mem", (char*) ipnet_try_free_mem, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_usr_sock_get_ops", (char*) ipnet_usr_sock_get_ops, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_usr_sock_get_pipe", (char*) ipnet_usr_sock_get_pipe, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_usr_sock_get_response_pipe", (char*) ipnet_usr_sock_get_response_pipe, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_usr_sock_get_route_cache_id", (char*) ipnet_usr_sock_get_route_cache_id, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_usr_sock_init", (char*) ipnet_usr_sock_init, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_usr_sock_init_sig", (char*) ipnet_usr_sock_init_sig, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_usr_sock_recvmsg", (char*) ipnet_usr_sock_recvmsg, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_usr_sock_wait_until_writable", (char*) ipnet_usr_sock_wait_until_writable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_version", (char*) ipnet_version, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ipnet_wrapper_funcs", (char*) &ipnet_wrapper_funcs, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ipppp_example_action_cb", (char*) ipppp_example_action_cb, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iptcp", (char*) &iptcp, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "iptcp_accept", (char*) iptcp_accept, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iptcp_ao_rtnetlink_init", (char*) iptcp_ao_rtnetlink_init, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iptcp_change_state_global", (char*) iptcp_change_state_global, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iptcp_close", (char*) iptcp_close, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iptcp_conf_default_backlog", (char*) &iptcp_conf_default_backlog, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "iptcp_configure", (char*) iptcp_configure, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iptcp_connect", (char*) iptcp_connect, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iptcp_create", (char*) iptcp_create, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iptcp_drop_connection", (char*) iptcp_drop_connection, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iptcp_get_rto", (char*) iptcp_get_rto, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iptcp_get_state", (char*) iptcp_get_state, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iptcp_getsockopt", (char*) iptcp_getsockopt, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iptcp_icmp4_report", (char*) iptcp_icmp4_report, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iptcp_ioctl", (char*) iptcp_ioctl, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iptcp_listen", (char*) iptcp_listen, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iptcp_pkt_drain", (char*) iptcp_pkt_drain, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iptcp_send", (char*) iptcp_send, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iptcp_send_reset", (char*) iptcp_send_reset, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iptcp_setsockopt", (char*) iptcp_setsockopt, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iptcp_shutdown", (char*) iptcp_shutdown, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iptcp_sock_free", (char*) iptcp_sock_free, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iptcp_sock_init", (char*) iptcp_sock_init, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iptcp_start", (char*) iptcp_start, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iptcp_tx_list", (char*) &iptcp_tx_list, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "iptcp_usr_recv", (char*) iptcp_usr_recv, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iptcp_version", (char*) iptcp_version, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iptcp_window_update", (char*) iptcp_window_update, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "isalnum", (char*) isalnum, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "isalpha", (char*) isalpha, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "isatty", (char*) isatty, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "iscntrl", (char*) iscntrl, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "isdigit", (char*) isdigit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "isgraph", (char*) isgraph, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "islower", (char*) islower, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "isprint", (char*) isprint, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ispunct", (char*) ispunct, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "isrArchDecCreate", (char*) isrArchDecCreate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "isrArchDecDispatcher", (char*) isrArchDecDispatcher, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "isrDeferJobAdd", (char*) isrDeferJobAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "isrDeferLibInit", (char*) isrDeferLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "isrDeferQueueGet", (char*) isrDeferQueueGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "isrIdCurrent", (char*) &isrIdCurrent, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "isrJobPool", (char*) &isrJobPool, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "isrRerouteNotify", (char*) isrRerouteNotify, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "isspace", (char*) isspace, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "isupper", (char*) isupper, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "isxdigit", (char*) isxdigit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "jobAdd", (char*) jobAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "jobQueueCreate", (char*) jobQueueCreate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "jobQueueInit", (char*) jobQueueInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "jobQueueLibInit", (char*) jobQueueLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "jobQueuePost", (char*) jobQueuePost, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "jobQueuePriorityMask", (char*) jobQueuePriorityMask, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "jobQueueProcess", (char*) jobQueueProcess, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "jobQueueProcessFunc", (char*) &jobQueueProcessFunc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "jobQueueStdJobsAlloc", (char*) jobQueueStdJobsAlloc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "jobQueueStdPoolInit", (char*) jobQueueStdPoolInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "jobQueueStdPost", (char*) jobQueueStdPost, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "jobQueueTask", (char*) jobQueueTask, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "jobTaskLibInit", (char*) jobTaskLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "jobTaskWorkAdd", (char*) jobTaskWorkAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "kernelBaseInit", (char*) kernelBaseInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "kernelCpuEnable", (char*) kernelCpuEnable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "kernelId", (char*) &kernelId, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "kernelIdGet", (char*) kernelIdGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "kernelInit", (char*) kernelInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "kernelIsCpuIdle", (char*) kernelIsCpuIdle, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "kernelIsIdle", (char*) &kernelIsIdle, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "kernelIsSystemIdle", (char*) kernelIsSystemIdle, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "kernelModuleListId", (char*) &kernelModuleListId, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "kernelObjClassId", (char*) &kernelObjClassId, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "kernelRoundRobinInstall", (char*) kernelRoundRobinInstall, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "kernelState", (char*) &kernelState, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "kernelTimeSlice", (char*) kernelTimeSlice, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "kernelTimeSliceGet", (char*) kernelTimeSliceGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "kernelVersion", (char*) kernelVersion, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "kill", (char*) kill, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "l", (char*) l, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "labs", (char*) labs, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "law0Resources", (char*) &law0Resources, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "lawRegister", (char*) lawRegister, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ld", (char*) ld, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ldCommonMatchAll", (char*) &ldCommonMatchAll, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ldexp", (char*) ldexp, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ldiv", (char*) ldiv, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ldiv_r", (char*) ldiv_r, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ledBeep", (char*) ledBeep, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ledBwdFind", (char*) ledBwdFind, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ledCDelete", (char*) ledCDelete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ledChange", (char*) ledChange, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ledClose", (char*) ledClose, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ledComplete", (char*) ledComplete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ledCompletionSet", (char*) ledCompletionSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ledControl", (char*) ledControl, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ledDfltComplete", (char*) ledDfltComplete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ledFwdFind", (char*) ledFwdFind, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ledHistCurrentNumGet", (char*) ledHistCurrentNumGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ledHistFind", (char*) ledHistFind, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ledHistNextGet", (char*) ledHistNextGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ledHistNumGet", (char*) ledHistNumGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ledHistPrevGet", (char*) ledHistPrevGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ledHistResize", (char*) ledHistResize, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ledHistoryAdd", (char*) ledHistoryAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ledHistoryClone", (char*) ledHistoryClone, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ledHistoryCreate", (char*) ledHistoryCreate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ledHistoryFree", (char*) ledHistoryFree, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ledHistoryLineAllocate", (char*) ledHistoryLineAllocate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ledHistorySet", (char*) ledHistorySet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ledInactivityDelaySet", (char*) ledInactivityDelaySet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ledLibInit", (char*) ledLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ledModeDefaultNameGet", (char*) ledModeDefaultNameGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ledModeNameGet", (char*) ledModeNameGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ledModeRegister", (char*) ledModeRegister, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ledModeSet", (char*) ledModeSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ledNWrite", (char*) ledNWrite, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ledOpen", (char*) ledOpen, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ledPreempt", (char*) ledPreempt, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ledPreemptSet", (char*) ledPreemptSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ledRead", (char*) ledRead, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ledRead2", (char*) ledRead2, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ledRedraw", (char*) ledRedraw, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ledReplace", (char*) ledReplace, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ledSearch", (char*) ledSearch, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "link", (char*) link, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "linkBufPoolInit", (char*) linkBufPoolInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "linkedCtorsInitialized", (char*) &linkedCtorsInitialized, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "listen", (char*) listen, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "lkAddr", (char*) lkAddr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "lkAddrInternal", (char*) lkAddrInternal, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "lkup", (char*) lkup, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ll", (char*) ll, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "llr", (char*) llr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "loadCommonSymbolProcess", (char*) loadCommonSymbolProcess, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "loadDefinedSymbolProcess", (char*) loadDefinedSymbolProcess, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "loadLibInit", (char*) loadLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "loadModule", (char*) loadModule, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "loadModuleAt", (char*) loadModuleAt, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "loadModuleInfoFromFd", (char*) loadModuleInfoFromFd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "loadModuleInfoFromFilenameOpen", (char*) loadModuleInfoFromFilenameOpen, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "loadModuleInfoInit", (char*) loadModuleInfoInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "loadModuleInfoRelease", (char*) loadModuleInfoRelease, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "loadModuleSeek", (char*) loadModuleSeek, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "loadModuleStringRead", (char*) loadModuleStringRead, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "loadModuleValueRead", (char*) loadModuleValueRead, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "loadSectionsInstall", (char*) loadSectionsInstall, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "loadSegInfoDescAllocate", (char*) loadSegInfoDescAllocate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "loadSegInfoDescFree", (char*) loadSegInfoDescFree, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "loadSegmentsAllocate", (char*) loadSegmentsAllocate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "loadShareTgtMemAlign", (char*) loadShareTgtMemAlign, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "loadShareTgtMemFree", (char*) loadShareTgtMemFree, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "loadShareTgtMemSet", (char*) loadShareTgtMemSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "loadSysSymTblLoad", (char*) loadSysSymTblLoad, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "loadUndefinedSymbolProcess", (char*) loadUndefinedSymbolProcess, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "loaderCacheBufferSize", (char*) &loaderCacheBufferSize, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "localToGlobalOffset", (char*) &localToGlobalOffset, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "localeFiles", (char*) &localeFiles, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "localeconv", (char*) localeconv, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "localtime", (char*) localtime, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "localtime_r", (char*) localtime_r, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "log", (char*) log, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "log10", (char*) log10, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "logDecorationChange", (char*) logDecorationChange, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "logFd", (char*) &logFd, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "logFdAdd", (char*) logFdAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "logFdDelete", (char*) logFdDelete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "logFdFromRlogin", (char*) &logFdFromRlogin, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "logFdSem", (char*) &logFdSem, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "logFdSet", (char*) logFdSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "logInit", (char*) logInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "logLevelChange", (char*) logLevelChange, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "logMsg", (char*) logMsg, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "logShow", (char*) logShow, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "logTask", (char*) logTask, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "logTaskId", (char*) &logTaskId, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "logTaskOptions", (char*) &logTaskOptions, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "logTaskPriority", (char*) &logTaskPriority, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "logTaskStackSize", (char*) &logTaskStackSize, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "logb", (char*) logb, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "loggerInfoShow", (char*) loggerInfoShow, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "loginHostname", (char*) &loginHostname, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "loginPassword", (char*) &loginPassword, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "loginUsername", (char*) &loginUsername, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "logout", (char*) logout, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "longjmp", (char*) longjmp, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ls", (char*) ls, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "lseek", (char*) lseek, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "lsr", (char*) lsr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "lstAdd", (char*) lstAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "lstConcat", (char*) lstConcat, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "lstCount", (char*) lstCount, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "lstDelete", (char*) lstDelete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "lstExtract", (char*) lstExtract, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "lstFind", (char*) lstFind, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "lstFirst", (char*) lstFirst, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "lstFree", (char*) lstFree, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "lstFree2", (char*) lstFree2, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "lstGet", (char*) lstGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "lstInit", (char*) lstInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "lstInsert", (char*) lstInsert, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "lstLast", (char*) lstLast, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "lstLibInit", (char*) lstLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "lstNStep", (char*) lstNStep, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "lstNext", (char*) lstNext, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "lstNth", (char*) lstNth, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "lstPrevious", (char*) lstPrevious, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "m", (char*) m, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "m2If64BitCounters", (char*) &m2If64BitCounters, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "m85xxTimerDrvRegister", (char*) m85xxTimerDrvRegister, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "m85xxTimerResources", (char*) &m85xxTimerResources, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "mRegs", (char*) mRegs, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "m_prepend", (char*) m_prepend, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "malloc", (char*) malloc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "mathFiles", (char*) &mathFiles, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "max_hdr", (char*) &max_hdr, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "max_linkhdr", (char*) &max_linkhdr, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "max_protohdr", (char*) &max_protohdr, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "mblen", (char*) mblen, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "mbstowcs", (char*) mbstowcs, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "mbtowc", (char*) mbtowc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "mchErrorL1DParityCnt", (char*) &mchErrorL1DParityCnt, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "mchErrorL1IParityCnt", (char*) &mchErrorL1IParityCnt, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "mchErrorTotalCnt", (char*) &mchErrorTotalCnt, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "md5_block_data_order", (char*) md5_block_data_order, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "md5_block_host_order", (char*) md5_block_host_order, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "memAddToPool", (char*) memAddToPool, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "memBlockSizeGet", (char*) memBlockSizeGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "memDefaultAlignment", (char*) &memDefaultAlignment, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "memFindMax", (char*) memFindMax, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "memInfoGet", (char*) memInfoGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "memInfoInit", (char*) memInfoInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "memInit", (char*) memInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "memMsgBlockError", (char*) &memMsgBlockError, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "memMsgBlockTooBig", (char*) &memMsgBlockTooBig, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "memOptionsGet", (char*) memOptionsGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "memOptionsSet", (char*) memOptionsSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "memPartAddToPool", (char*) memPartAddToPool, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "memPartAlignedAlloc", (char*) memPartAlignedAlloc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "memPartAlloc", (char*) memPartAlloc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "memPartAllocErrorRtn", (char*) &memPartAllocErrorRtn, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "memPartAllocN", (char*) memPartAllocN, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "memPartBlockErrorRtn", (char*) &memPartBlockErrorRtn, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "memPartBlockIsValid", (char*) memPartBlockIsValid, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "memPartBlockValidate", (char*) memPartBlockValidate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "memPartClassId", (char*) &memPartClassId, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "memPartCreate", (char*) memPartCreate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "memPartCreateHookSet", (char*) memPartCreateHookSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "memPartDelete", (char*) memPartDelete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "memPartDeleteFree", (char*) memPartDeleteFree, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "memPartEdrErrorInject", (char*) memPartEdrErrorInject, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "memPartFindMax", (char*) memPartFindMax, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "memPartFree", (char*) memPartFree, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "memPartFreeListWalk", (char*) memPartFreeListWalk, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "memPartFreeN", (char*) memPartFreeN, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "memPartHooksInstall", (char*) memPartHooksInstall, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "memPartInfoGet", (char*) memPartInfoGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "memPartInit", (char*) memPartInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "memPartInsertFree", (char*) memPartInsertFree, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "memPartLibInit", (char*) memPartLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "memPartOptionsDefault", (char*) &memPartOptionsDefault, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "memPartOptionsGet", (char*) memPartOptionsGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "memPartOptionsSet", (char*) memPartOptionsSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "memPartRealloc", (char*) memPartRealloc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "memPartReallocInternal", (char*) memPartReallocInternal, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "memPartShow", (char*) memPartShow, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "memPartVerifiedLock", (char*) memPartVerifiedLock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "memPoolEndAdrs", (char*) &memPoolEndAdrs, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "memShow", (char*) memShow, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "memShowInit", (char*) memShowInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "memSysPartId", (char*) &memSysPartId, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "memSysPartition", (char*) &memSysPartition, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "memalign", (char*) memalign, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "memchr", (char*) memchr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "memcmp", (char*) memcmp, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "memcpy", (char*) memcpy, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "memmove", (char*) memmove, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "memoryDump", (char*) memoryDump, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "memoryModify", (char*) memoryModify, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "memset", (char*) memset, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "mib2ErrorAdd", (char*) mib2ErrorAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "mib2Init", (char*) mib2Init, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "miiBusCreate", (char*) miiBusCreate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "miiBusDelete", (char*) miiBusDelete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "miiBusGet", (char*) miiBusGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "miiBusIdleErrorCheck", (char*) miiBusIdleErrorCheck, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "miiBusListAdd", (char*) miiBusListAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "miiBusListDel", (char*) miiBusListDel, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "miiBusMediaAdd", (char*) miiBusMediaAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "miiBusMediaDefaultSet", (char*) miiBusMediaDefaultSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "miiBusMediaDel", (char*) miiBusMediaDel, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "miiBusMediaListGet", (char*) miiBusMediaListGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "miiBusMediaUpdate", (char*) miiBusMediaUpdate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "miiBusModeGet", (char*) miiBusModeGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "miiBusModeSet", (char*) miiBusModeSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "miiBusRead", (char*) miiBusRead, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "miiBusRegister", (char*) miiBusRegister, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "miiBusWrite", (char*) miiBusWrite, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "miiMediaUpdate_desc", (char*) &miiMediaUpdate_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "miiModeGet_desc", (char*) &miiModeGet_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "miiModeSet_desc", (char*) &miiModeSet_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "miiRead_desc", (char*) &miiRead_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "miiWrite_desc", (char*) &miiWrite_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "mkdir", (char*) mkdir, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "mktime", (char*) mktime, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "mmuArchCopySize", (char*) &mmuArchCopySize, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "mmuArchLibAttrs", (char*) &mmuArchLibAttrs, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "mmuArchLibFuncs", (char*) &mmuArchLibFuncs, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "mmuCacheStateTransTbl", (char*) &mmuCacheStateTransTbl, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "mmuE500FlushPte", (char*) mmuE500FlushPte, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "mmuE500LibInit", (char*) mmuE500LibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "mmuE500MaxPhysBitsGet", (char*) mmuE500MaxPhysBitsGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "mmuE500PteUpdate", (char*) mmuE500PteUpdate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "mmuE500StaticTlbArray", (char*) &mmuE500StaticTlbArray, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "mmuE500TlbDynamicInvalidate", (char*) mmuE500TlbDynamicInvalidate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "mmuE500TlbStaticInit", (char*) mmuE500TlbStaticInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "mmuE500TlbStaticInvalidate", (char*) mmuE500TlbStaticInvalidate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "mmuGlobalTransTbl", (char*) &mmuGlobalTransTbl, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "mmuInvalidState", (char*) &mmuInvalidState, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "mmuLibFuncs", (char*) &mmuLibFuncs, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "mmuMaskTransTbl", (char*) &mmuMaskTransTbl, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "mmuPpcADisable", (char*) mmuPpcADisable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "mmuPpcAEnable", (char*) mmuPpcAEnable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "mmuPpcBcopy", (char*) mmuPpcBcopy, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "mmuPpcDEnabled", (char*) &mmuPpcDEnabled, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "mmuPpcDataTlbMissHandler", (char*) mmuPpcDataTlbMissHandler, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "mmuPpcE500mcTlbie", (char*) mmuPpcE500mcTlbie, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "mmuPpcHasBeenEnabled", (char*) &mmuPpcHasBeenEnabled, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "mmuPpcIEnabled", (char*) &mmuPpcIEnabled, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "mmuPpcInstTlbMissHandler", (char*) mmuPpcInstTlbMissHandler, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "mmuPpcPidGet", (char*) mmuPpcPidGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "mmuPpcPidSet", (char*) mmuPpcPidSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "mmuPpcSearchPidGet", (char*) mmuPpcSearchPidGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "mmuPpcSearchPidSet", (char*) mmuPpcSearchPidSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "mmuPpcSelected", (char*) &mmuPpcSelected, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "mmuPpcTlbMissHandlerEnd", (char*) mmuPpcTlbMissHandlerEnd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "mmuPpcTlbReadEntryWord0", (char*) mmuPpcTlbReadEntryWord0, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "mmuPpcTlbReadEntryWord1", (char*) mmuPpcTlbReadEntryWord1, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "mmuPpcTlbReadEntryWord2", (char*) mmuPpcTlbReadEntryWord2, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "mmuPpcTlbReadEntryWord3", (char*) mmuPpcTlbReadEntryWord3, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "mmuPpcTlbWriteExecute", (char*) mmuPpcTlbWriteExecute, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "mmuProtStateTransTbl", (char*) &mmuProtStateTransTbl, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "mmuRegionTblMask", (char*) mmuRegionTblMask, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "mmuValidStateTransTbl", (char*) &mmuValidStateTransTbl, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "modf", (char*) modf, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "moduleCheck", (char*) moduleCheck, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "moduleCreate", (char*) moduleCreate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "moduleCreateHookAdd", (char*) moduleCreateHookAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "moduleCreateHookDelete", (char*) moduleCreateHookDelete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "moduleDelete", (char*) moduleDelete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "moduleDescDestroy", (char*) moduleDescDestroy, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "moduleEach", (char*) moduleEach, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "moduleFindByGroup", (char*) moduleFindByGroup, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "moduleFindByName", (char*) moduleFindByName, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "moduleFindByNameAndPath", (char*) moduleFindByNameAndPath, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "moduleFirstSectionGet", (char*) moduleFirstSectionGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "moduleFlagsGet", (char*) moduleFlagsGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "moduleFlagsSet", (char*) moduleFlagsSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "moduleIdFigure", (char*) moduleIdFigure, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "moduleIdListGet", (char*) moduleIdListGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "moduleIdVerify", (char*) moduleIdVerify, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "moduleInfoGet", (char*) moduleInfoGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "moduleInit", (char*) moduleInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "moduleLibInit", (char*) moduleLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "moduleNameGet", (char*) moduleNameGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "moduleNextSectionGet", (char*) moduleNextSectionGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "moduleSectionChecksumSet", (char*) moduleSectionChecksumSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "moduleSectionDescFree", (char*) moduleSectionDescFree, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "moduleSectionDescGet", (char*) moduleSectionDescGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "moduleSectionRegister", (char*) moduleSectionRegister, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "moduleSegAdd", (char*) moduleSegAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "moduleSegEach", (char*) moduleSegEach, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "moduleSegFirst", (char*) moduleSegFirst, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "moduleSegGet", (char*) moduleSegGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "moduleSegInfoGet", (char*) moduleSegInfoGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "moduleSegNext", (char*) moduleSegNext, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "moduleShow", (char*) moduleShow, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "moduleShowInit", (char*) moduleShowInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "moduleUndefSymAdd", (char*) moduleUndefSymAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "msgDesc", (char*) &msgDesc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "msgQClassId", (char*) &msgQClassId, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "msgQCreate", (char*) msgQCreate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "msgQCreateLibInit", (char*) msgQCreateLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "msgQDelete", (char*) msgQDelete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "msgQDestroy", (char*) msgQDestroy, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "msgQEvStart", (char*) msgQEvStart, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "msgQEvStop", (char*) msgQEvStop, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "msgQInfoGet", (char*) msgQInfoGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "msgQInit", (char*) msgQInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "msgQInitialize", (char*) msgQInitialize, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "msgQLibInit", (char*) msgQLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "msgQLibInstalled", (char*) &msgQLibInstalled, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "msgQNumMsgs", (char*) msgQNumMsgs, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "msgQReceive", (char*) msgQReceive, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "msgQSend", (char*) msgQSend, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "msgQShow", (char*) msgQShow, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "msgQShowInit", (char*) msgQShowInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "msgQSmNumMsgsRtn", (char*) &msgQSmNumMsgsRtn, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "msgQSmReceiveRtn", (char*) &msgQSmReceiveRtn, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "msgQSmSendRtn", (char*) &msgQSmSendRtn, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "msgQTerminate", (char*) msgQTerminate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "mutexOptionsHostLib", (char*) &mutexOptionsHostLib, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "mutexOptionsIosLib", (char*) &mutexOptionsIosLib, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "mutexOptionsLogLib", (char*) &mutexOptionsLogLib, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "mutexOptionsNetDrv", (char*) &mutexOptionsNetDrv, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "mutexOptionsSelectLib", (char*) &mutexOptionsSelectLib, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "mutexOptionsSymLib", (char*) &mutexOptionsSymLib, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "mutexOptionsTyLib", (char*) &mutexOptionsTyLib, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "mux2Bind", (char*) mux2Bind, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "mux2LibInit", (char*) mux2LibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "mux2OverEndInit", (char*) mux2OverEndInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "mux2OverEndOutputFilterChainDrops", (char*) &mux2OverEndOutputFilterChainDrops, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "mux2PollReceive", (char*) mux2PollReceive, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "mux2PollSend", (char*) mux2PollSend, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "mux2Send", (char*) mux2Send, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxAddressForm", (char*) muxAddressForm, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxBind", (char*) muxBind, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxBindCommon", (char*) muxBindCommon, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxBindStyles", (char*) &muxBindStyles, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "muxCommonInit", (char*) muxCommonInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxDebug", (char*) &muxDebug, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "muxDevAcquire", (char*) muxDevAcquire, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxDevConnect_desc", (char*) &muxDevConnect_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "muxDevExists", (char*) muxDevExists, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxDevLoad", (char*) muxDevLoad, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxDevRelease", (char*) muxDevRelease, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxDevStart", (char*) muxDevStart, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxDevStop", (char*) muxDevStop, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxDevStopAll", (char*) muxDevStopAll, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxDevUnload", (char*) muxDevUnload, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxDevUnloadAsync", (char*) muxDevUnloadAsync, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxEndJobQDo", (char*) muxEndJobQDo, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxEndJobQDoJob", (char*) muxEndJobQDoJob, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxEndListGet", (char*) muxEndListGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxEndQnumGet", (char*) muxEndQnumGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxEndRxJobQueue", (char*) muxEndRxJobQueue, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxEndStyles", (char*) &muxEndStyles, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "muxError", (char*) muxError, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxErrorPost", (char*) muxErrorPost, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxErrorSkip", (char*) muxErrorSkip, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxIfTypeGet", (char*) muxIfTypeGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxIoctl", (char*) muxIoctl, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxIpcomPktFreeMblk", (char*) muxIpcomPktFreeMblk, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxIpcomPktToMblkTx", (char*) muxIpcomPktToMblkTx, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxL2PortDetachRtn", (char*) &muxL2PortDetachRtn, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "muxLibInit", (char*) muxLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxLinkDownNotify", (char*) muxLinkDownNotify, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxLinkHeaderCreate", (char*) muxLinkHeaderCreate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxLinkUpNotify", (char*) muxLinkUpNotify, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxLock", (char*) &muxLock, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "muxMCastAddrAdd", (char*) muxMCastAddrAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxMCastAddrDel", (char*) muxMCastAddrDel, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxMCastAddrGet", (char*) muxMCastAddrGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxMblkToIpcomPktMallocDrops", (char*) &muxMblkToIpcomPktMallocDrops, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "muxMblkToIpcomPktTx", (char*) muxMblkToIpcomPktTx, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxMblkToIpcomPktTxChainDrops", (char*) &muxMblkToIpcomPktTxChainDrops, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "muxPacketAddrGet", (char*) muxPacketAddrGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxPacketDataGet", (char*) muxPacketDataGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxPollReceive", (char*) muxPollReceive, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxPollSend", (char*) muxPollSend, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxProtoInfoGet", (char*) muxProtoInfoGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxProtoListGet", (char*) muxProtoListGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxReceive", (char*) muxReceive, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxRestoreIpcomPktFromMblkTx", (char*) muxRestoreIpcomPktFromMblkTx, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxSend", (char*) muxSend, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxShow", (char*) muxShow, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxTkBind", (char*) muxTkBind, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxTkCookieGet", (char*) muxTkCookieGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxTkDrvCheck", (char*) muxTkDrvCheck, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxTkLibInit", (char*) muxTkLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxTkOverEndInit", (char*) muxTkOverEndInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxTkPollReceive", (char*) muxTkPollReceive, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxTkPollSend", (char*) muxTkPollSend, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxTkSend", (char*) muxTkSend, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxTxRestart", (char*) muxTxRestart, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "muxUnbind", (char*) muxUnbind, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "mv", (char*) mv, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "nae_log", (char*) nae_log, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "namelessPrefix", (char*) &namelessPrefix, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "netBufClOffset", (char*) &netBufClOffset, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "netBufLibInit", (char*) netBufLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "netBufLibInitialize", (char*) netBufLibInitialize, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "netBufLock", (char*) &netBufLock, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "netBufPoolInit", (char*) netBufPoolInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "netClBlkFree", (char*) netClBlkFree, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "netClBlkGet", (char*) netClBlkGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "netClBlkJoin", (char*) netClBlkJoin, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "netClFree", (char*) netClFree, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "netClPoolIdGet", (char*) netClPoolIdGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "netClusterGet", (char*) netClusterGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "netDaemonQ", (char*) netDaemonQ, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "netDaemonQnum", (char*) netDaemonQnum, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "netDaemonsCount", (char*) netDaemonsCount, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "netDaemonsStart", (char*) netDaemonsStart, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "netDevCreate", (char*) netDevCreate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "netDevCreate2", (char*) netDevCreate2, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "netDrv", (char*) netDrv, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "netDrvDebugLevelSet", (char*) netDrvDebugLevelSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "netDrvFileDoesNotExist", (char*) netDrvFileDoesNotExist, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "netDrvFileDoesNotExistInstall", (char*) netDrvFileDoesNotExistInstall, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "netErrnoSet", (char*) netErrnoSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "netHelp", (char*) netHelp, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "netJobAdd", (char*) netJobAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "netJobNum", (char*) &netJobNum, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "netJobQueueId", (char*) &netJobQueueId, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "netLibInitialize", (char*) netLibInitialize, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "netLsByName", (char*) netLsByName, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "netMblkChainDup", (char*) netMblkChainDup, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "netMblkClChainFree", (char*) netMblkClChainFree, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "netMblkClFree", (char*) netMblkClFree, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "netMblkClGet", (char*) netMblkClGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "netMblkClJoin", (char*) netMblkClJoin, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "netMblkDup", (char*) netMblkDup, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "netMblkFree", (char*) netMblkFree, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "netMblkFromBufCopy", (char*) netMblkFromBufCopy, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "netMblkGet", (char*) netMblkGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "netMblkOffsetToBufCopy", (char*) netMblkOffsetToBufCopy, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "netMblkToBufCopy", (char*) netMblkToBufCopy, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "netPoolCreate", (char*) netPoolCreate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "netPoolCreateValidate", (char*) netPoolCreateValidate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "netPoolDelete", (char*) netPoolDelete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "netPoolIdGet", (char*) netPoolIdGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "netPoolIdGetWork", (char*) netPoolIdGetWork, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "netPoolInit", (char*) netPoolInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "netPoolInitWrapper", (char*) netPoolInitWrapper, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "netPoolKheapInit", (char*) netPoolKheapInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "netPoolNameGet", (char*) netPoolNameGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "netPoolRelPri", (char*) &netPoolRelPri, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "netPoolRelease", (char*) netPoolRelease, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "netTaskId", (char*) &netTaskId, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "netTaskNamePrefix", (char*) &netTaskNamePrefix, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "netTaskOptions", (char*) &netTaskOptions, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "netTaskPriority", (char*) &netTaskPriority, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "netTaskStackSize", (char*) &netTaskStackSize, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "netTupleFree", (char*) netTupleFree, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "netTupleGet", (char*) netTupleGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "netVersionString", (char*) &netVersionString, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "networkinit", (char*) networkinit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "noDev", (char*) noDev, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ns16550SioRegister", (char*) ns16550SioRegister, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ns1655x1Resources", (char*) &ns1655x1Resources, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ns1655x2Resources", (char*) &ns1655x2Resources, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ns1655x3Resources", (char*) &ns1655x3Resources, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ns1655x4Resources", (char*) &ns1655x4Resources, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "numLogFds", (char*) &numLogFds, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "numberStaticAvail", (char*) &numberStaticAvail, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "objAlloc", (char*) objAlloc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "objAllocExtra", (char*) objAllocExtra, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "objArchLibInit", (char*) objArchLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "objClassIdGet", (char*) objClassIdGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "objClassTypeGet", (char*) objClassTypeGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "objClassTypeShow", (char*) objClassTypeShow, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "objClassTypeString", (char*) &objClassTypeString, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "objContextGet", (char*) objContextGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "objContextSet", (char*) objContextSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "objCoreInit", (char*) objCoreInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "objCoreInvalidate", (char*) objCoreInvalidate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "objCoreReclaim", (char*) objCoreReclaim, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "objCoreTerminate", (char*) objCoreTerminate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "objCoreUnlink", (char*) objCoreUnlink, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "objEach", (char*) objEach, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "objFree", (char*) objFree, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "objGenericVerify", (char*) objGenericVerify, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "objHandleShow", (char*) objHandleShow, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "objHandleTblShow", (char*) objHandleTblShow, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "objInfo", (char*) objInfo, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "objInfoInit", (char*) objInfoInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "objLibInit", (char*) objLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "objMemAlloc", (char*) objMemAlloc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "objMemFree", (char*) objMemFree, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "objNameGet", (char*) objNameGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "objNameLenGet", (char*) objNameLenGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "objNamePtrGet", (char*) objNamePtrGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "objNamePtrSet", (char*) objNamePtrSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "objNameSet", (char*) objNameSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "objNameToId", (char*) objNameToId, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "objOwnerGet", (char*) objOwnerGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "objOwnerLibInstalled", (char*) &objOwnerLibInstalled, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "objOwnerSet", (char*) objOwnerSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "objOwnerSetBase", (char*) objOwnerSetBase, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "objRoutineNameGet", (char*) objRoutineNameGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "objShow", (char*) objShow, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "objShowAll", (char*) objShowAll, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "objShowInit", (char*) objShowInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "objVerify", (char*) objVerify, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "objVerifyError", (char*) objVerifyError, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "objVerifyMagicCompare", (char*) objVerifyMagicCompare, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "objVerifyMagicRead", (char*) objVerifyMagicRead, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "objVerifyTypeCompare", (char*) objVerifyTypeCompare, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "objVerifyTypeRead", (char*) objVerifyTypeRead, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "objectShow", (char*) objectShow, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "open", (char*) open, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "openPicTimerDevAresources", (char*) &openPicTimerDevAresources, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "openPicTimerDevBresources", (char*) &openPicTimerDevBresources, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "openPicTimerDrvRegister", (char*) openPicTimerDrvRegister, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "opendir", (char*) opendir, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "oprintf", (char*) oprintf, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "pBusHead", (char*) &pBusHead, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "pBusListHead", (char*) &pBusListHead, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "pClkCookie", (char*) &pClkCookie, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "pClkTimer", (char*) &pClkTimer, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "pCoprocDescList", (char*) &pCoprocDescList, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "pDbgClnt", (char*) &pDbgClnt, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "pDriverListHead", (char*) &pDriverListHead, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "pHead", (char*) &pHead, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "pIosDevCloseRtn", (char*) &pIosDevCloseRtn, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "pIosFdEntrySetRtn", (char*) &pIosFdEntrySetRtn, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "pIosFdInvalidRtn", (char*) &pIosFdInvalidRtn, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "pIosRemovableDevDrv", (char*) &pIosRemovableDevDrv, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "pIosSuspendedDevDrv", (char*) &pIosSuspendedDevDrv, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "pJobPool", (char*) &pJobPool, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "pLoadRoutine", (char*) &pLoadRoutine, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "pLoaderHooksCall", (char*) &pLoaderHooksCall, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "pM2TrapRtn", (char*) &pM2TrapRtn, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "pM2TrapRtnArg", (char*) &pM2TrapRtnArg, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "pMibRtn", (char*) &pMibRtn, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "pNetPoolHead", (char*) &pNetPoolHead, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "pNullFd", (char*) &pNullFd, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "pPlbBus", (char*) &pPlbBus, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "pPlbDev", (char*) &pPlbDev, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "pPlbDevControl", (char*) &pPlbDevControl, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "pRebootHookTbl", (char*) &pRebootHookTbl, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "pReservedAtomics", (char*) &pReservedAtomics, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "pRootMemStart", (char*) &pRootMemStart, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "pSockIoctlMemVal", (char*) &pSockIoctlMemVal, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "pSysClkName", (char*) &pSysClkName, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "pSysPlbMethods", (char*) &pSysPlbMethods, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "pTaskLastDspTcb", (char*) &pTaskLastDspTcb, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "pTaskLastFpTcb", (char*) &pTaskLastFpTcb, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "pUnixIoctlMemVal", (char*) &pUnixIoctlMemVal, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "pVxbDelayCookie", (char*) &pVxbDelayCookie, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "pVxbDelayTimer", (char*) &pVxbDelayTimer, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "pVxbEpicIsrHandle", (char*) &pVxbEpicIsrHandle, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "pVxbM85xxTimerInstance", (char*) &pVxbM85xxTimerInstance, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "pVxbPpcIntCtlrDrvCtrl", (char*) &pVxbPpcIntCtlrDrvCtrl, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "pVxbSpinLockGive", (char*) &pVxbSpinLockGive, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "pVxbSpinLockTake", (char*) &pVxbSpinLockTake, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "pWvNetEvtMap", (char*) &pWvNetEvtMap, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "panic", (char*) panic, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "panicSuspend", (char*) &panicSuspend, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "pathBuild", (char*) pathBuild, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "pathCat", (char*) pathCat, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "pathCondense", (char*) pathCondense, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "pathLastName", (char*) pathLastName, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "pathLastNamePtr", (char*) pathLastNamePtr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "pathParse", (char*) pathParse, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "pathSplit", (char*) pathSplit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "pathconf", (char*) pathconf, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "pause", (char*) pause, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "pc", (char*) pc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "period", (char*) period, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "periodRun", (char*) periodRun, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "perror", (char*) perror, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "pipeDevCreate", (char*) pipeDevCreate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "pipeDevDelete", (char*) pipeDevDelete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "pipeDrv", (char*) pipeDrv, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "pipeMsgQOptions", (char*) &pipeMsgQOptions, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "platRatioTable", (char*) &platRatioTable, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "plbInit1", (char*) plbInit1, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "plbIntrGet", (char*) plbIntrGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "plbIntrSet", (char*) plbIntrSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "plbRegister", (char*) plbRegister, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "poolBlockAdd", (char*) poolBlockAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "poolCreate", (char*) poolCreate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "poolDelete", (char*) poolDelete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "poolFindItem", (char*) poolFindItem, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "poolFreeCount", (char*) poolFreeCount, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "poolIdListGet", (char*) poolIdListGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "poolIncrementGet", (char*) poolIncrementGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "poolIncrementSet", (char*) poolIncrementSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "poolInitialize", (char*) poolInitialize, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "poolItemGet", (char*) poolItemGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "poolItemReturn", (char*) poolItemReturn, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "poolLibInit", (char*) poolLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "poolListGbl", (char*) &poolListGbl, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "poolShow", (char*) poolShow, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "poolTotalCount", (char*) poolTotalCount, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "poolUnusedBlocksFree", (char*) poolUnusedBlocksFree, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "posixSignalMode", (char*) &posixSignalMode, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "pow", (char*) pow, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ppGlobalEnviron", (char*) &ppGlobalEnviron, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "ppcBrCompute", (char*) ppcBrCompute, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ppcBrExtract", (char*) ppcBrExtract, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ppcE500CACHE_ALIGN_SIZE", (char*) &ppcE500CACHE_ALIGN_SIZE, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ppcE500DCACHE_LINE_NUM", (char*) &ppcE500DCACHE_LINE_NUM, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ppcE500ICACHE_LINE_NUM", (char*) &ppcE500ICACHE_LINE_NUM, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ppcE500Mch", (char*) ppcE500Mch, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ppcE500MchEnd", (char*) ppcE500MchEnd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ppcIntCtlr0Resources", (char*) &ppcIntCtlr0Resources, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ppcIntCtlrInputs", (char*) &ppcIntCtlrInputs, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ppcIntCtlrRegister", (char*) ppcIntCtlrRegister, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ppcVectorFit", (char*) &ppcVectorFit, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ppcVectorSysTimer", (char*) &ppcVectorSysTimer, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ppcVectorWdt", (char*) &ppcVectorWdt, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "printErr", (char*) printErr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "printErrno", (char*) printErrno, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "printExc", (char*) printExc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "printLogo", (char*) printLogo, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "printf", (char*) printf, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "priv_targetName", (char*) &priv_targetName, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "ptyDevCreate", (char*) ptyDevCreate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ptyDevRemove", (char*) ptyDevRemove, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ptyDrv", (char*) ptyDrv, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "putc", (char*) putc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "putchar", (char*) putchar, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "putenv", (char*) putenv, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "puts", (char*) puts, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "putw", (char*) putw, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "pwd", (char*) pwd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qFifoClass", (char*) &qFifoClass, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "qFifoClassId", (char*) &qFifoClassId, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "qFifoEach", (char*) qFifoEach, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qFifoGet", (char*) qFifoGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qFifoInfo", (char*) qFifoInfo, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qFifoInit", (char*) qFifoInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qFifoNext", (char*) qFifoNext, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qFifoPut", (char*) qFifoPut, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qFifoRemove", (char*) qFifoRemove, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qFifoResort", (char*) qFifoResort, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qFifoRestore", (char*) qFifoRestore, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qPriBMapClassId", (char*) &qPriBMapClassId, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "qPriBMapEach", (char*) qPriBMapEach, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qPriBMapGet", (char*) qPriBMapGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qPriBMapInfo", (char*) qPriBMapInfo, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qPriBMapInit", (char*) qPriBMapInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qPriBMapKey", (char*) qPriBMapKey, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qPriBMapNext", (char*) qPriBMapNext, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qPriBMapPut", (char*) qPriBMapPut, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qPriBMapRemove", (char*) qPriBMapRemove, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qPriBMapResort", (char*) qPriBMapResort, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qPriBMapRestore", (char*) qPriBMapRestore, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qPriBMapSet", (char*) qPriBMapSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qPriDeltaAdvance", (char*) qPriDeltaAdvance, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qPriDeltaClassId", (char*) &qPriDeltaClassId, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "qPriDeltaEach", (char*) qPriDeltaEach, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qPriDeltaGet", (char*) qPriDeltaGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qPriDeltaGetExpired", (char*) qPriDeltaGetExpired, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qPriDeltaInfo", (char*) qPriDeltaInfo, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qPriDeltaInit", (char*) qPriDeltaInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qPriDeltaKey", (char*) qPriDeltaKey, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qPriDeltaNext", (char*) qPriDeltaNext, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qPriDeltaPut", (char*) qPriDeltaPut, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qPriDeltaRemove", (char*) qPriDeltaRemove, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qPriDeltaResort", (char*) qPriDeltaResort, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qPriListClassId", (char*) &qPriListClassId, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "qPriListEach", (char*) qPriListEach, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qPriListFromTailClassId", (char*) &qPriListFromTailClassId, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "qPriListGet", (char*) qPriListGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qPriListInfo", (char*) qPriListInfo, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qPriListInit", (char*) qPriListInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qPriListKey", (char*) qPriListKey, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qPriListNext", (char*) qPriListNext, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qPriListPut", (char*) qPriListPut, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qPriListPutFromTail", (char*) qPriListPutFromTail, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qPriListRemove", (char*) qPriListRemove, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qPriListResort", (char*) qPriListResort, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qTradPriBMapClassId", (char*) &qTradPriBMapClassId, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "qTradPriBMapSetCompatible", (char*) qTradPriBMapSetCompatible, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qTradPriListClassId", (char*) &qTradPriListClassId, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "qTradPriListSetCompatible", (char*) qTradPriListSetCompatible, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qman0Resources", (char*) &qman0Resources, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "qmanRegister", (char*) qmanRegister, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalAcquire", (char*) qportalAcquire, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalCStateQuery", (char*) qportalCStateQuery, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalCalloutAdd", (char*) qportalCalloutAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalCalloutDelete", (char*) qportalCalloutDelete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalCgInit", (char*) qportalCgInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalCgModify", (char*) qportalCgModify, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalCgQuery", (char*) qportalCgQuery, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalCgTestWrite", (char*) qportalCgTestWrite, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalCreate", (char*) qportalCreate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalDcaDequeue", (char*) qportalDcaDequeue, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalDcaEnqueue", (char*) qportalDcaEnqueue, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalDcaRelease", (char*) qportalDcaRelease, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalDequeue", (char*) qportalDequeue, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalDequeueSourceSelect", (char*) qportalDequeueSourceSelect, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalDestroy", (char*) qportalDestroy, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalEnqueue", (char*) qportalEnqueue, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalFqFullInit", (char*) qportalFqFullInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalFqInit", (char*) qportalFqInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalFqNpQuery", (char*) qportalFqNpQuery, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalFqQuery", (char*) qportalFqQuery, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalFqShutdown", (char*) qportalFqShutdown, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalFqTdCalc", (char*) qportalFqTdCalc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalFullDequeue", (char*) qportalFullDequeue, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalFullEnqueue", (char*) qportalFullEnqueue, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalIntConnect", (char*) qportalIntConnect, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalIntDisable", (char*) qportalIntDisable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalIntDisconnect", (char*) qportalIntDisconnect, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalIntEnable", (char*) qportalIntEnable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalIntHandlingDisable", (char*) qportalIntHandlingDisable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalIntHandlingEnable", (char*) qportalIntHandlingEnable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalIntHandlingSelect", (char*) qportalIntHandlingSelect, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalLocalDequeue", (char*) qportalLocalDequeue, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalLocalEnqueue", (char*) qportalLocalEnqueue, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalMrCalloutAdd", (char*) qportalMrCalloutAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalMrCalloutDelete", (char*) qportalMrCalloutDelete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalMrDequeue", (char*) qportalMrDequeue, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalMrDequeueFull", (char*) qportalMrDequeueFull, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalMrLocalDequeue", (char*) qportalMrLocalDequeue, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalOrpDequeue", (char*) qportalOrpDequeue, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalOrpEnqueue", (char*) qportalOrpEnqueue, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalOrpLocalDequeue", (char*) qportalOrpLocalDequeue, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalOrpLocalEnqueue", (char*) qportalOrpLocalEnqueue, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalOrpMrDequeue", (char*) qportalOrpMrDequeue, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalOrpSafeDequeue", (char*) qportalOrpSafeDequeue, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalOrpSafeEnqueue", (char*) qportalOrpSafeEnqueue, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalRelease", (char*) qportalRelease, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalSafeDequeue", (char*) qportalSafeDequeue, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalSafeEnqueue", (char*) qportalSafeEnqueue, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalSafeMrDequeue", (char*) qportalSafeMrDequeue, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qportalSet", (char*) qportalSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "qsort", (char*) qsort, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "raise", (char*) raise, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "rand", (char*) rand, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "rcmd", (char*) rcmd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "rcmd_af", (char*) rcmd_af, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "read", (char*) read, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "readdir", (char*) readdir, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "readdir_r", (char*) readdir_r, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "readv", (char*) readv, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "readyQBMap", (char*) &readyQBMap, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "readyQHead", (char*) &readyQHead, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "realloc", (char*) realloc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "reboot", (char*) reboot, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "rebootHookAdd", (char*) rebootHookAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "rebootHookDelete", (char*) rebootHookDelete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "rebootHookTbl", (char*) &rebootHookTbl, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "rebootHookTblSize", (char*) &rebootHookTblSize, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "recv", (char*) recv, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "recvfrom", (char*) recvfrom, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "recvmsg", (char*) recvmsg, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "reld", (char*) reld, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "remCurIdGet", (char*) remCurIdGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "remCurIdSet", (char*) remCurIdSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "remLastResvPort", (char*) &remLastResvPort, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "remLibInit", (char*) remLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "remNumConnRetrials", (char*) &remNumConnRetrials, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "remPasswd", (char*) &remPasswd, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "remStdErrSetupTimeout", (char*) &remStdErrSetupTimeout, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "remUser", (char*) &remUser, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "remove", (char*) remove, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "rename", (char*) rename, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "repeat", (char*) repeat, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "repeatRun", (char*) repeatRun, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "reschedule", (char*) reschedule, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "restartTaskName", (char*) &restartTaskName, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "restartTaskOptions", (char*) &restartTaskOptions, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "restartTaskPriority", (char*) &restartTaskPriority, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "restartTaskStackSize", (char*) &restartTaskStackSize, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "retran_noprs", (char*) &retran_noprs, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "retran_notmr", (char*) &retran_notmr, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "rewind", (char*) rewind, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "rewinddir", (char*) rewinddir, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "rindex", (char*) rindex, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "rm", (char*) rm, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "rmdir", (char*) rmdir, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "rngBufGet", (char*) rngBufGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "rngBufPut", (char*) rngBufPut, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "rngCreate", (char*) rngCreate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "rngDelete", (char*) rngDelete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "rngFlush", (char*) rngFlush, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "rngFreeBytes", (char*) rngFreeBytes, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "rngIsEmpty", (char*) rngIsEmpty, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "rngIsFull", (char*) rngIsFull, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "rngMoveAhead", (char*) rngMoveAhead, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "rngNBytes", (char*) rngNBytes, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "rngPutAhead", (char*) rngPutAhead, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "romfsImage", (char*) &romfsImage, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "romfsImageEnd", (char*) &romfsImageEnd, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "rootMemNBytes", (char*) &rootMemNBytes, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "rootTaskId", (char*) &rootTaskId, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "roundRobinHookInstalled", (char*) &roundRobinHookInstalled, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "roundRobinOn", (char*) &roundRobinOn, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "roundRobinSlice", (char*) &roundRobinSlice, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "rresvport", (char*) rresvport, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "rresvportCommon", (char*) rresvportCommon, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "rresvport_af", (char*) rresvport_af, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "rtpDeleteMsg", (char*) &rtpDeleteMsg, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "rtpSigDeleteMsg", (char*) &rtpSigDeleteMsg, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "rtpSigStopMsg", (char*) &rtpSigStopMsg, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "rtpStopMsg", (char*) &rtpStopMsg, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "runtimeName", (char*) &runtimeName, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "runtimeVersion", (char*) &runtimeVersion, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "runtimeVersionFull", (char*) &runtimeVersionFull, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "s", (char*) s, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "saveExcMsg", (char*) saveExcMsg, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "scalb", (char*) scalb, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "scanCharSet", (char*) scanCharSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "scanField", (char*) scanField, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "scanf", (char*) scanf, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "selNodeAdd", (char*) selNodeAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "selNodeDelete", (char*) selNodeDelete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "selTaskDeleteHookAdd", (char*) selTaskDeleteHookAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "selWakeup", (char*) selWakeup, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "selWakeupAll", (char*) selWakeupAll, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "selWakeupListInit", (char*) selWakeupListInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "selWakeupListLen", (char*) selWakeupListLen, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "selWakeupListTerm", (char*) selWakeupListTerm, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "selWakeupType", (char*) selWakeupType, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "select", (char*) select, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "selectInit", (char*) selectInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semBCreate", (char*) semBCreate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semBGive", (char*) semBGive, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semBGiveDefer", (char*) semBGiveDefer, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semBGiveNoLock", (char*) semBGiveNoLock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semBGiveScalable", (char*) semBGiveScalable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semBInit", (char*) semBInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semBInitialize", (char*) semBInitialize, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semBTake", (char*) semBTake, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semBTakeNoLock", (char*) semBTakeNoLock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semBTakeScalable", (char*) semBTakeScalable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semCCreate", (char*) semCCreate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semCGive", (char*) semCGive, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semCGiveDefer", (char*) semCGiveDefer, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semCInit", (char*) semCInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semCInitialize", (char*) semCInitialize, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semCTake", (char*) semCTake, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semClass", (char*) &semClass, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "semClassId", (char*) &semClassId, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "semCreateTbl", (char*) &semCreateTbl, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "semDelete", (char*) semDelete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semDeleteLibInit", (char*) semDeleteLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semDestroy", (char*) semDestroy, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semEvIsFreeTbl", (char*) &semEvIsFreeTbl, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "semEvStart", (char*) semEvStart, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semEvStop", (char*) semEvStop, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semFlush", (char*) semFlush, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semFlushDefer", (char*) semFlushDefer, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semFlushDeferTbl", (char*) &semFlushDeferTbl, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "semFlushTbl", (char*) &semFlushTbl, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "semGive", (char*) semGive, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semGiveDefer", (char*) semGiveDefer, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semGiveDeferTbl", (char*) &semGiveDeferTbl, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "semGiveNoLockTbl", (char*) &semGiveNoLockTbl, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "semGiveTbl", (char*) &semGiveTbl, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "semInfo", (char*) semInfo, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semInfoGet", (char*) semInfoGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semIntRestrict", (char*) semIntRestrict, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semInvalid", (char*) semInvalid, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semLibInit", (char*) semLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semLocalIdVerify", (char*) semLocalIdVerify, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semMCreate", (char*) semMCreate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semMGive", (char*) semMGive, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semMGiveForce", (char*) semMGiveForce, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semMGiveKernWork", (char*) semMGiveKernWork, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semMGiveNoLock", (char*) semMGiveNoLock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semMGiveScalable", (char*) semMGiveScalable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semMInit", (char*) semMInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semMInitialize", (char*) semMInitialize, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semMPendQPut", (char*) semMPendQPut, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semMTake", (char*) semMTake, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semMTakeByProxy", (char*) semMTakeByProxy, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semMTakeNoLock", (char*) semMTakeNoLock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semMTakeScalable", (char*) semMTakeScalable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semQFlush", (char*) semQFlush, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semQFlushDefer", (char*) semQFlushDefer, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semQInit", (char*) semQInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semRTake", (char*) semRTake, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semRWCreate", (char*) semRWCreate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semRWGive", (char*) semRWGive, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semRWGiveForce", (char*) semRWGiveForce, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semRWInitialize", (char*) semRWInitialize, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semRWLibMaxReaders", (char*) &semRWLibMaxReaders, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "semShow", (char*) semShow, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semShowInit", (char*) semShowInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semTake", (char*) semTake, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semTakeNoLockTbl", (char*) &semTakeNoLockTbl, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "semTakeTbl", (char*) &semTakeTbl, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "semTerminate", (char*) semTerminate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "semWTake", (char*) semWTake, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "send", (char*) send, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "send_full", (char*) &send_full, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "sendmsg", (char*) sendmsg, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sendto", (char*) sendto, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "setbuf", (char*) setbuf, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "setbuffer", (char*) setbuffer, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sethostname", (char*) sethostname, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "setjmp", (char*) setjmp, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "setlinebuf", (char*) setlinebuf, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "setlocale", (char*) setlocale, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "setsockopt", (char*) setsockopt, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "settimeofday", (char*) settimeofday, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "setvbuf", (char*) setvbuf, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sfpDoubleNormalize", (char*) sfpDoubleNormalize, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sfpDoubleNormalize2", (char*) sfpDoubleNormalize2, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shConfig", (char*) shConfig, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sha1_block_data_order", (char*) sha1_block_data_order, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sha1_block_host_order", (char*) sha1_block_host_order, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellAbort", (char*) shellAbort, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellAbortHandledByShell", (char*) shellAbortHandledByShell, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellActivate", (char*) shellActivate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellBackgroundInit", (char*) shellBackgroundInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellBackgroundRead", (char*) shellBackgroundRead, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellCompatibleCheck", (char*) shellCompatibleCheck, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellCompatibleSet", (char*) shellCompatibleSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellCompletionPathnameGet", (char*) shellCompletionPathnameGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellConfigDefaultGet", (char*) shellConfigDefaultGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellConfigDefaultSet", (char*) shellConfigDefaultSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellConfigDefaultValueGet", (char*) shellConfigDefaultValueGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellConfigDefaultValueSet", (char*) shellConfigDefaultValueSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellConfigDefaultValueUnset", (char*) shellConfigDefaultValueUnset, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellConfigGet", (char*) shellConfigGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellConfigLibInit", (char*) shellConfigLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellConfigSet", (char*) shellConfigSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellConfigValueGet", (char*) shellConfigValueGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellConfigValueSet", (char*) shellConfigValueSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellConfigValueUnset", (char*) shellConfigValueUnset, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellConsoleInit", (char*) shellConsoleInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellConsoleTaskId", (char*) &shellConsoleTaskId, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "shellContextFinalize", (char*) shellContextFinalize, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellContextInit", (char*) shellContextInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellContextListLock", (char*) shellContextListLock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellContextListUnlock", (char*) shellContextListUnlock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellDataAdd", (char*) shellDataAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellDataAllRemove", (char*) shellDataAllRemove, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellDataFirst", (char*) shellDataFirst, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellDataFromNameAdd", (char*) shellDataFromNameAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellDataFromNameGet", (char*) shellDataFromNameGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellDataGet", (char*) shellDataGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellDataNext", (char*) shellDataNext, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellDataRemove", (char*) shellDataRemove, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellErrnoGet", (char*) shellErrnoGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellErrnoSet", (char*) shellErrnoSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellExec", (char*) shellExec, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellExitWrapper", (char*) shellExitWrapper, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellFirst", (char*) shellFirst, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellFromNameGet", (char*) shellFromNameGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellFromTaskGet", (char*) shellFromTaskGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellFromTaskParentContextGet", (char*) shellFromTaskParentContextGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellGenericCreate", (char*) shellGenericCreate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellGenericInit", (char*) shellGenericInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellHistory", (char*) shellHistory, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellIdVerify", (char*) shellIdVerify, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInOutBgndClose", (char*) shellInOutBgndClose, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInOutBgndGet", (char*) shellInOutBgndGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInOutBgndIoClose", (char*) shellInOutBgndIoClose, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInOutBgndOpen", (char*) shellInOutBgndOpen, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInOutBgndRegister", (char*) shellInOutBgndRegister, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInOutBlockingBlock", (char*) shellInOutBlockingBlock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInOutBlockingBlockedDataGet", (char*) shellInOutBlockingBlockedDataGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInOutBlockingClose", (char*) shellInOutBlockingClose, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInOutBlockingDataSet", (char*) shellInOutBlockingDataSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInOutBlockingGet", (char*) shellInOutBlockingGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInOutBlockingOnDataClose", (char*) shellInOutBlockingOnDataClose, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInOutBlockingOpen", (char*) shellInOutBlockingOpen, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInOutBlockingUnblock", (char*) shellInOutBlockingUnblock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInOutGet", (char*) shellInOutGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInOutInitialGet", (char*) shellInOutInitialGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInOutInputHookAdd", (char*) shellInOutInputHookAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInOutInputHookDelete", (char*) shellInOutInputHookDelete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInOutInputHookSet", (char*) shellInOutInputHookSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInOutInputHookUnset", (char*) shellInOutInputHookUnset, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInOutInputUniqueCheck", (char*) shellInOutInputUniqueCheck, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInOutMasterPtyGet", (char*) shellInOutMasterPtyGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInOutRedirectClose", (char*) shellInOutRedirectClose, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInOutRedirectOpen", (char*) shellInOutRedirectOpen, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInOutRedirectRestore", (char*) shellInOutRedirectRestore, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInOutRedirectScript", (char*) shellInOutRedirectScript, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInOutRedirectSet", (char*) shellInOutRedirectSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInOutSet", (char*) shellInOutSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInOutSlavePtyGet", (char*) shellInOutSlavePtyGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInOutUse", (char*) shellInOutUse, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInternalArgStackAllocate", (char*) shellInternalArgStackAllocate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInternalArgumentEval", (char*) shellInternalArgumentEval, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInternalDataSymbolGet", (char*) shellInternalDataSymbolGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInternalDecValueDisplay", (char*) shellInternalDecValueDisplay, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInternalFuncDoubleArgAdd", (char*) shellInternalFuncDoubleArgAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInternalFuncFloatArgAdd", (char*) shellInternalFuncFloatArgAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInternalFuncLLongArgAdd", (char*) shellInternalFuncLLongArgAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInternalFuncLongArgAdd", (char*) shellInternalFuncLongArgAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInternalFunctionCall", (char*) shellInternalFunctionCall, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInternalHexValueDisplay", (char*) shellInternalHexValueDisplay, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInternalKernelSymbolCreate", (char*) shellInternalKernelSymbolCreate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInternalOptFree", (char*) shellInternalOptFree, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInternalOptGet", (char*) shellInternalOptGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInternalStrSpaceTokenize", (char*) shellInternalStrSpaceTokenize, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInternalStrToChar", (char*) shellInternalStrToChar, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInternalStrToFloat", (char*) shellInternalStrToFloat, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInternalStrToInt", (char*) shellInternalStrToInt, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInternalStrToInt2", (char*) shellInternalStrToInt2, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInternalStrToInt32", (char*) shellInternalStrToInt32, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInternalStrToLong", (char*) shellInternalStrToLong, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInternalStrTokenize", (char*) shellInternalStrTokenize, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInternalStrUnquote", (char*) shellInternalStrUnquote, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInternalSymTblFromRtpGet", (char*) shellInternalSymTblFromRtpGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInternalSymTblFromTaskGet", (char*) shellInternalSymTblFromTaskGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInternalSymbolDisplay", (char*) shellInternalSymbolDisplay, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInternalSymbolGet", (char*) shellInternalSymbolGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInternalSymbolsResolve", (char*) shellInternalSymbolsResolve, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInternalTaskNameMatch", (char*) shellInternalTaskNameMatch, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInternalTextSymbolGet", (char*) shellInternalTextSymbolGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInternalValueDisplay", (char*) shellInternalValueDisplay, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInterpByNameFind", (char*) shellInterpByNameFind, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInterpCInit", (char*) shellInterpCInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInterpC_create_buffer", (char*) shellInterpC_create_buffer, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInterpC_delete_buffer", (char*) shellInterpC_delete_buffer, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInterpC_flush_buffer", (char*) shellInterpC_flush_buffer, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInterpC_init_buffer", (char*) shellInterpC_init_buffer, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInterpC_load_buffer_state", (char*) shellInterpC_load_buffer_state, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInterpC_scan_buffer", (char*) shellInterpC_scan_buffer, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInterpC_scan_bytes", (char*) shellInterpC_scan_bytes, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInterpC_scan_string", (char*) shellInterpC_scan_string, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInterpC_switch_to_buffer", (char*) shellInterpC_switch_to_buffer, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInterpCin", (char*) &shellInterpCin, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "shellInterpCleng", (char*) &shellInterpCleng, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "shellInterpClex", (char*) shellInterpClex, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInterpCout", (char*) &shellInterpCout, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "shellInterpCparse", (char*) shellInterpCparse, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInterpCrestart", (char*) shellInterpCrestart, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInterpCtext", (char*) &shellInterpCtext, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "shellInterpCtxCreate", (char*) shellInterpCtxCreate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInterpCtxGet", (char*) shellInterpCtxGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInterpDefaultNameGet", (char*) shellInterpDefaultNameGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInterpDelete", (char*) shellInterpDelete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInterpEvaluate", (char*) shellInterpEvaluate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInterpLibInit", (char*) shellInterpLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInterpNameGet", (char*) shellInterpNameGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInterpPromptDftSet", (char*) shellInterpPromptDftSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInterpPromptGet", (char*) shellInterpPromptGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInterpPromptSet", (char*) shellInterpPromptSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInterpRegister", (char*) shellInterpRegister, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInterpRestart", (char*) shellInterpRestart, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInterpSet", (char*) shellInterpSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellInterpSwitch", (char*) shellInterpSwitch, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellLibInit", (char*) shellLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellLineLenGet", (char*) shellLineLenGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellLock", (char*) shellLock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellLoginInstall", (char*) shellLoginInstall, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellLogout", (char*) shellLogout, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellLogout2", (char*) shellLogout2, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellLogoutInstall", (char*) shellLogoutInstall, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellMaxSessionsSet", (char*) shellMaxSessionsSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellMemAllRegFree", (char*) shellMemAllRegFree, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellMemCalloc", (char*) shellMemCalloc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellMemFree", (char*) shellMemFree, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellMemLibInit", (char*) shellMemLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellMemMalloc", (char*) shellMemMalloc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellMemRealloc", (char*) shellMemRealloc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellMemRegAllFree", (char*) shellMemRegAllFree, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellMemRegFree", (char*) shellMemRegFree, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellMemRegMalloc", (char*) shellMemRegMalloc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellMemRegister", (char*) shellMemRegister, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellMemStrdup", (char*) shellMemStrdup, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellMemStringAdd", (char*) shellMemStringAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellMemStringFree", (char*) shellMemStringFree, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellMemStringIsAllocated", (char*) shellMemStringIsAllocated, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellMemStringShow", (char*) shellMemStringShow, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellMemUnregister", (char*) shellMemUnregister, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellNext", (char*) shellNext, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellOutputLock", (char*) shellOutputLock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellOutputUnlock", (char*) shellOutputUnlock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellParserControl", (char*) shellParserControl, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellPrint", (char*) shellPrint, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellPrintErr", (char*) shellPrintErr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellPromptFmtDftSet", (char*) shellPromptFmtDftSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellPromptFmtSet", (char*) shellPromptFmtSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellPromptFmtStrAdd", (char*) shellPromptFmtStrAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellPromptLibInit", (char*) shellPromptLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellPromptPrint", (char*) shellPromptPrint, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellPromptSet", (char*) shellPromptSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellRemoteConfigStr", (char*) &shellRemoteConfigStr, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "shellResourceReleaseHookAdd", (char*) shellResourceReleaseHookAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellRestart", (char*) shellRestart, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellScriptAbort", (char*) shellScriptAbort, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellScriptNoAbort", (char*) shellScriptNoAbort, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellTask", (char*) shellTask, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellTaskDefaultBaseNameGet", (char*) shellTaskDefaultBaseNameGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellTaskDefaultBaseNameSet", (char*) shellTaskDefaultBaseNameSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellTaskDefaultOptionsGet", (char*) shellTaskDefaultOptionsGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellTaskDefaultOptionsSet", (char*) shellTaskDefaultOptionsSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellTaskDefaultPriorityGet", (char*) shellTaskDefaultPriorityGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellTaskDefaultPrioritySet", (char*) shellTaskDefaultPrioritySet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellTaskDefaultStackSizeGet", (char*) shellTaskDefaultStackSizeGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellTaskDefaultStackSizeSet", (char*) shellTaskDefaultStackSizeSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellTaskGet", (char*) shellTaskGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellTaskIdDefault", (char*) shellTaskIdDefault, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellTerminate", (char*) shellTerminate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellWorkingMemCtxAttach", (char*) shellWorkingMemCtxAttach, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellWorkingMemCtxAttachedShow", (char*) shellWorkingMemCtxAttachedShow, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellWorkingMemCtxCurrentGet", (char*) shellWorkingMemCtxCurrentGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellWorkingMemCtxCurrentSet", (char*) shellWorkingMemCtxCurrentSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellWorkingMemCtxDetach", (char*) shellWorkingMemCtxDetach, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellWorkingMemCtxDuplicate", (char*) shellWorkingMemCtxDuplicate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellWorkingMemCtxGet", (char*) shellWorkingMemCtxGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellWorkingMemCtxIdConvert", (char*) shellWorkingMemCtxIdConvert, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellWorkingMemCtxLastGet", (char*) shellWorkingMemCtxLastGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shellWorkingMemCtxRtnSet", (char*) shellWorkingMemCtxRtnSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "show", (char*) show, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "showLoggerInfo", (char*) showLoggerInfo, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "shutdown", (char*) shutdown, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sigEvtRtn", (char*) &sigEvtRtn, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "sigInit", (char*) sigInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sigPendDestroy", (char*) sigPendDestroy, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sigPendInit", (char*) sigPendInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sigPendKill", (char*) sigPendKill, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sigaction", (char*) sigaction, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sigaddset", (char*) sigaddset, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sigblock", (char*) sigblock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sigdelset", (char*) sigdelset, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sigemptyset", (char*) sigemptyset, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sigfillset", (char*) sigfillset, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sigismember", (char*) sigismember, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "signal", (char*) signal, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sigpending", (char*) sigpending, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sigprocmask", (char*) sigprocmask, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sigqueue", (char*) sigqueue, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sigqueueInit", (char*) sigqueueInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sigreturn", (char*) sigreturn, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sigsetjmp", (char*) sigsetjmp, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sigsetmask", (char*) sigsetmask, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sigsuspend", (char*) sigsuspend, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sigtimedwait", (char*) sigtimedwait, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sigvec", (char*) sigvec, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sigwait", (char*) sigwait, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sigwaitinfo", (char*) sigwaitinfo, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sin", (char*) sin, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sinh", (char*) sinh, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sioChanConnect_desc", (char*) &sioChanConnect_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "sioChanGet_desc", (char*) &sioChanGet_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "sioEnable_desc", (char*) &sioEnable_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "sioIntEnable_desc", (char*) &sioIntEnable_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "sioNextChannelNumberAssign", (char*) sioNextChannelNumberAssign, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sizeNodesAlloc", (char*) sizeNodesAlloc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sllCount", (char*) sllCount, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sllEach", (char*) sllEach, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sllGet", (char*) sllGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sllInit", (char*) sllInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sllPrevious", (char*) sllPrevious, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sllPutAtHead", (char*) sllPutAtHead, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sllPutAtTail", (char*) sllPutAtTail, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sllRemove", (char*) sllRemove, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sllTerminate", (char*) sllTerminate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "smObjPoolMinusOne", (char*) &smObjPoolMinusOne, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "smObjTaskDeleteFailRtn", (char*) &smObjTaskDeleteFailRtn, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "smObjTcbFreeFailRtn", (char*) &smObjTcbFreeFailRtn, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "smObjTcbFreeRtn", (char*) &smObjTcbFreeRtn, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "smsgDesc", (char*) &smsgDesc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "snoopEnable", (char*) &snoopEnable, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "snprintf", (char*) snprintf, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "so", (char*) so, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sockInfo", (char*) sockInfo, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sockLibAdd", (char*) sockLibAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sockLibInit", (char*) sockLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sockLibMap", (char*) &sockLibMap, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "sockShow", (char*) sockShow, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "socket", (char*) socket, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "socketDevHdr", (char*) &socketDevHdr, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "sp", (char*) sp, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "spTaskOptions", (char*) &spTaskOptions, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "spTaskPriority", (char*) &spTaskPriority, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "spTaskStackSize", (char*) &spTaskStackSize, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "spinLockIsrGive", (char*) spinLockIsrGive, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "spinLockIsrInit", (char*) spinLockIsrInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "spinLockIsrTake", (char*) spinLockIsrTake, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "spinLockRestrict", (char*) spinLockRestrict, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "spinLockTaskGive", (char*) spinLockTaskGive, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "spinLockTaskInit", (char*) spinLockTaskInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "spinLockTaskTake", (char*) spinLockTaskTake, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sprintf", (char*) sprintf, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "spy", (char*) spy, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "spyClkStart", (char*) spyClkStart, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "spyClkStop", (char*) spyClkStop, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "spyHelp", (char*) spyHelp, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "spyReport", (char*) spyReport, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "spyStop", (char*) spyStop, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "spyTask", (char*) spyTask, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sqrt", (char*) sqrt, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "srand", (char*) srand, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sscanf", (char*) sscanf, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "standAloneSymTbl", (char*) &standAloneSymTbl, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "stat", (char*) stat, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "statSymTbl", (char*) &statSymTbl, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "statfs", (char*) statfs, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "statfs64", (char*) statfs64, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "staticPTEArray", (char*) &staticPTEArray, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "stdioFiles", (char*) &stdioFiles, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "stdioFp", (char*) stdioFp, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "stdioFpCreate", (char*) stdioFpCreate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "stdioFpDestroy", (char*) stdioFpDestroy, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "stdioInit", (char*) stdioInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "stdioShow", (char*) stdioShow, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "stdioShowInit", (char*) stdioShowInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "stdlibFiles", (char*) &stdlibFiles, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "stopApplLogger", (char*) stopApplLogger, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "strFree", (char*) strFree, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "strcasecmp", (char*) strcasecmp, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "strcat", (char*) strcat, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "strchr", (char*) strchr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "strcmp", (char*) strcmp, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "strcoll", (char*) strcoll, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "strcpy", (char*) strcpy, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "strcspn", (char*) strcspn, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "strdup", (char*) strdup, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "strerror", (char*) strerror, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "strerrorIf", (char*) strerrorIf, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "strerror_r", (char*) strerror_r, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "strftime", (char*) strftime, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "stringFiles", (char*) &stringFiles, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "strlcpy", (char*) strlcpy, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "strlen", (char*) strlen, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "strncat", (char*) strncat, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "strncmp", (char*) strncmp, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "strncpy", (char*) strncpy, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "strpbrk", (char*) strpbrk, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "strrchr", (char*) strrchr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "strspn", (char*) strspn, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "strstr", (char*) strstr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "strtod", (char*) strtod, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "strtok", (char*) strtok, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "strtok_r", (char*) strtok_r, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "strtol", (char*) strtol, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "strtoul", (char*) strtoul, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "strxfrm", (char*) strxfrm, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "substrcmp", (char*) substrcmp, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "swab", (char*) swab, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "symAdd", (char*) symAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "symByCNameFind", (char*) symByCNameFind, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "symByValueAndTypeFind", (char*) symByValueAndTypeFind, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "symByValueFind", (char*) symByValueFind, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "symEach", (char*) symEach, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "symEachCall", (char*) symEachCall, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "symFind", (char*) symFind, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "symFindByName", (char*) symFindByName, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "symFindByNameAndType", (char*) symFindByNameAndType, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "symFindByValue", (char*) symFindByValue, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "symFindByValueAndType", (char*) symFindByValueAndType, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "symFindSymbol", (char*) symFindSymbol, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "symFree", (char*) symFree, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "symGroupDefault", (char*) &symGroupDefault, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "symLibInit", (char*) symLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "symListGet", (char*) symListGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "symLkupPgSz", (char*) &symLkupPgSz, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "symNameGet", (char*) symNameGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "symRegister", (char*) symRegister, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "symRemove", (char*) symRemove, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "symShow", (char*) symShow, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "symShowInit", (char*) symShowInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "symTblAdd", (char*) symTblAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "symTblCreate", (char*) symTblCreate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "symTblDelete", (char*) symTblDelete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "symTblRemove", (char*) symTblRemove, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "symTblShutdown", (char*) symTblShutdown, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "symTblSymSetRemove", (char*) symTblSymSetRemove, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "symTypeGet", (char*) symTypeGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "symTypeToStringConvert", (char*) symTypeToStringConvert, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "symValueGet", (char*) symValueGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "syncLoadRtn", (char*) &syncLoadRtn, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "syncSymAddRtn", (char*) &syncSymAddRtn, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "syncSymRemoveRtn", (char*) &syncSymRemoveRtn, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "sysAdaEnable", (char*) &sysAdaEnable, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "sysBoardReset", (char*) sysBoardReset, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysBootFile", (char*) &sysBootFile, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "sysBootHost", (char*) &sysBootHost, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "sysBootLine", (char*) &sysBootLine, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "sysBootParams", (char*) &sysBootParams, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "sysBspDevFilter_desc", (char*) &sysBspDevFilter_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "sysBspRev", (char*) sysBspRev, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysBus", (char*) &sysBus, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "sysBusClear", (char*) sysBusClear, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysBusIntAck", (char*) sysBusIntAck, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysBusTas", (char*) sysBusTas, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysBusToLocalAdrs", (char*) sysBusToLocalAdrs, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysCacheEnable", (char*) sysCacheEnable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysCacheFlush", (char*) sysCacheFlush, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysClkConnect", (char*) sysClkConnect, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysClkDevUnitNo", (char*) &sysClkDevUnitNo, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "sysClkDisable", (char*) sysClkDisable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysClkEnable", (char*) sysClkEnable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysClkFreqGet", (char*) sysClkFreqGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysClkHandleGet", (char*) sysClkHandleGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysClkRateGet", (char*) sysClkRateGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysClkRateSet", (char*) sysClkRateSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysClkTimerNo", (char*) &sysClkTimerNo, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "sysCpcRegGet", (char*) sysCpcRegGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysCpcRegSet", (char*) sysCpcRegSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysCplusEnable", (char*) &sysCplusEnable, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "sysCpu", (char*) &sysCpu, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "sysCpu1LoopCount", (char*) &sysCpu1LoopCount, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "sysCpuIdGet", (char*) sysCpuIdGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysCpusetReset", (char*) sysCpusetReset, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysDebugModeGet", (char*) sysDebugModeGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysDebugModeInit", (char*) sysDebugModeInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysDebugModeSet", (char*) sysDebugModeSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysDebugModeSetHookAdd", (char*) sysDebugModeSetHookAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysDebugModeSetHookDelete", (char*) sysDebugModeSetHookDelete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysDelay", (char*) sysDelay, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysDoze", (char*) sysDoze, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysDozeCpu", (char*) sysDozeCpu, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysEprGet", (char*) sysEprGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysExcMsg", (char*) &sysExcMsg, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "sysFlags", (char*) &sysFlags, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "sysFlashDataPoll", (char*) sysFlashDataPoll, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysFlashDelay", (char*) sysFlashDelay, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysFlashErase", (char*) sysFlashErase, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysFlashGet", (char*) sysFlashGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysFlashSet", (char*) sysFlashSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysFlashTypeGet", (char*) sysFlashTypeGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysFlashWrite", (char*) sysFlashWrite, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysFmanClkFreqGet_desc", (char*) &sysFmanClkFreqGet_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "sysFslErrataCcfA004510AndA004511", (char*) sysFslErrataCcfA004510AndA004511, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysFslErrataFpuA003999", (char*) sysFslErrataFpuA003999, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysGetPeripheralBase", (char*) sysGetPeripheralBase, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysHwErrorString", (char*) &sysHwErrorString, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "sysHwInit", (char*) sysHwInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysHwInit2", (char*) sysHwInit2, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysHwMemInit", (char*) sysHwMemInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysICacheLock", (char*) sysICacheLock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysICacheUnlock", (char*) sysICacheUnlock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysInByte", (char*) sysInByte, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysInLong", (char*) sysInLong, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysInWord", (char*) sysInWord, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysInit", (char*) sysInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysInstParamTable", (char*) &sysInstParamTable, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "sysIntConnect", (char*) sysIntConnect, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysIntDisable", (char*) sysIntDisable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysIntEnable", (char*) sysIntEnable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysL2CacheInit", (char*) sysL2CacheInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysL3ExtWriteBufferAlloc", (char*) sysL3ExtWriteBufferAlloc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysLocalToBusAdrs", (char*) sysLocalToBusAdrs, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysLpidSet", (char*) sysLpidSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysMacIndex2Dev", (char*) sysMacIndex2Dev, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysMacIndex2Unit", (char*) sysMacIndex2Unit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysMacOffsetGet", (char*) sysMacOffsetGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysMailboxConnect", (char*) sysMailboxConnect, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysMailboxDisable", (char*) sysMailboxDisable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysMailboxEnable", (char*) sysMailboxEnable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysMemTop", (char*) sysMemTop, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysModel", (char*) sysModel, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysMotTsecEnetAddrGet", (char*) sysMotTsecEnetAddrGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysMsDelay", (char*) sysMsDelay, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysNetDevName", (char*) &sysNetDevName, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "sysNetMacAddrGet", (char*) sysNetMacAddrGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysNetMacAddrSet", (char*) sysNetMacAddrSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysNetMacNVRamAddrGet", (char*) sysNetMacNVRamAddrGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysNvRamGet", (char*) sysNvRamGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysNvRamSet", (char*) sysNvRamSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysOutByte", (char*) sysOutByte, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysOutLong", (char*) sysOutLong, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysOutWord", (char*) sysOutWord, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysPCGet", (char*) sysPCGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysPciInByte", (char*) sysPciInByte, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysPciInLong", (char*) sysPciInLong, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysPciInWord", (char*) sysPciInWord, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysPciOutByte", (char*) sysPciOutByte, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysPciOutLong", (char*) sysPciOutLong, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysPciOutWord", (char*) sysPciOutWord, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysPciRead32", (char*) sysPciRead32, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysPciWrite32", (char*) sysPciWrite32, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysPhysMemDesc", (char*) &sysPhysMemDesc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "sysPhysMemDescNumEnt", (char*) &sysPhysMemDescNumEnt, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "sysPhysMemTop", (char*) sysPhysMemTop, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysPhysToVirt_desc", (char*) &sysPhysToVirt_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "sysPicClkFreqGet", (char*) sysPicClkFreqGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysProcNum", (char*) &sysProcNum, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "sysProcNumGet", (char*) sysProcNumGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysProcNumSet", (char*) sysProcNumSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysSectorErase", (char*) sysSectorErase, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysSelfReset", (char*) sysSelfReset, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysSerdesRcwActual", (char*) &sysSerdesRcwActual, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "sysSerialChanConnect", (char*) sysSerialChanConnect, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysSerialChanGet", (char*) sysSerialChanGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysSerialConnectAll", (char*) sysSerialConnectAll, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysStartType", (char*) &sysStartType, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "sysStaticTlbDesc", (char*) &sysStaticTlbDesc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "sysStaticTlbDescNumEnt", (char*) &sysStaticTlbDescNumEnt, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "sysSymTbl", (char*) &sysSymTbl, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "sysTas", (char*) sysTas, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysTextProtect", (char*) &sysTextProtect, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "sysTimeBaseLGet", (char*) sysTimeBaseLGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysToMonitor", (char*) sysToMonitor, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysUsDelay", (char*) sysUsDelay, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "sysVirtToPhys_desc", (char*) &sysVirtToPhys_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "sysVmContext", (char*) &sysVmContext, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "sysVmeEnable", (char*) &sysVmeEnable, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "system", (char*) system, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "systemSecurityIsEnabled", (char*) &systemSecurityIsEnabled, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "tExcTaskExcStk", (char*) &tExcTaskExcStk, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "tExcTaskStk", (char*) &tExcTaskStk, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "tExcTaskTcb", (char*) &tExcTaskTcb, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "tan", (char*) tan, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "tanh", (char*) tanh, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskActivate", (char*) taskActivate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskArgsGet", (char*) taskArgsGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskArgsSet", (char*) taskArgsSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskBpHook", (char*) &taskBpHook, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "taskBpHookSet", (char*) taskBpHookSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskClassId", (char*) &taskClassId, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "taskCont", (char*) taskCont, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskCpuAffinityGet", (char*) taskCpuAffinityGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskCpuAffinitySet", (char*) taskCpuAffinitySet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskCpuLock", (char*) taskCpuLock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskCpuUnlock", (char*) taskCpuUnlock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskCpuUnlockNoResched", (char*) taskCpuUnlockNoResched, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskCreat", (char*) taskCreat, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskCreate", (char*) taskCreate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskCreateHookAdd", (char*) taskCreateHookAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskCreateHookDelete", (char*) taskCreateHookDelete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskCreateHookInit", (char*) taskCreateHookInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskCreateHookShow", (char*) taskCreateHookShow, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskCreateHooks", (char*) &taskCreateHooks, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "taskCreateHooksArrayA", (char*) &taskCreateHooksArrayA, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "taskCreateHooksArrayB", (char*) &taskCreateHooksArrayB, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "taskCreateLibInit", (char*) taskCreateLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskCreateWithGuard", (char*) taskCreateWithGuard, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskDelay", (char*) taskDelay, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskDelete", (char*) taskDelete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskDeleteForce", (char*) taskDeleteForce, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskDeleteHookAdd", (char*) taskDeleteHookAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskDeleteHookDelete", (char*) taskDeleteHookDelete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskDeleteHookShow", (char*) taskDeleteHookShow, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskDeleteHooks", (char*) &taskDeleteHooks, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "taskDeleteHooksArrayA", (char*) &taskDeleteHooksArrayA, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "taskDeleteHooksArrayB", (char*) &taskDeleteHooksArrayB, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "taskDestroy", (char*) taskDestroy, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskEach", (char*) taskEach, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskExit", (char*) taskExit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskHookShowInit", (char*) taskHookShowInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskIdCurrent", (char*) &taskIdCurrent, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "taskIdDefault", (char*) taskIdDefault, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskIdFigure", (char*) taskIdFigure, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskIdListGet", (char*) taskIdListGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskIdListSort", (char*) taskIdListSort, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskIdSelf", (char*) taskIdSelf, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskIdVerify", (char*) taskIdVerify, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskInfoGet", (char*) taskInfoGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskInit", (char*) taskInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskInitExcStk", (char*) taskInitExcStk, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskInitialize", (char*) taskInitialize, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskIsDelayed", (char*) taskIsDelayed, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskIsPended", (char*) taskIsPended, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskIsReady", (char*) taskIsReady, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskIsStopped", (char*) taskIsStopped, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskIsSuspended", (char*) taskIsSuspended, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskKerExcStackSize", (char*) &taskKerExcStackSize, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "taskKerExcStkOverflowSize", (char*) &taskKerExcStkOverflowSize, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "taskKerExecStkOverflowSize", (char*) &taskKerExecStkOverflowSize, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "taskKerExecStkUnderflowSize", (char*) &taskKerExecStkUnderflowSize, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "taskLibInit", (char*) taskLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskLock", (char*) taskLock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskMsrDefault", (char*) &taskMsrDefault, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "taskMsrSet", (char*) taskMsrSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskName", (char*) taskName, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskNameToId", (char*) taskNameToId, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskOptionsGet", (char*) taskOptionsGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskOptionsSet", (char*) taskOptionsSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskOptionsString", (char*) taskOptionsString, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskPriNormalGet", (char*) taskPriNormalGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskPriorityGet", (char*) taskPriorityGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskPrioritySet", (char*) taskPrioritySet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskRegName", (char*) &taskRegName, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "taskRegsGet", (char*) taskRegsGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskRegsInit", (char*) taskRegsInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskRegsModify", (char*) taskRegsModify, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskRegsSet", (char*) taskRegsSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskRegsShow", (char*) taskRegsShow, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskRegsShowOutput", (char*) taskRegsShowOutput, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskReservedFieldGet", (char*) taskReservedFieldGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskReservedFieldSet", (char*) taskReservedFieldSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskReservedNumAllot", (char*) taskReservedNumAllot, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskReset", (char*) taskReset, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskRestart", (char*) taskRestart, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskResume", (char*) taskResume, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskRtnValueSet", (char*) taskRtnValueSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskSafe", (char*) taskSafe, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskSchedInfoGet", (char*) taskSchedInfoGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskShow", (char*) taskShow, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskShowInit", (char*) taskShowInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskSpareFieldGet", (char*) taskSpareFieldGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskSpareFieldSet", (char*) taskSpareFieldSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskSpareNumAllot", (char*) taskSpareNumAllot, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskSpawn", (char*) taskSpawn, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskStackAllot", (char*) taskStackAllot, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskStackSizeGet", (char*) taskStackSizeGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskStatusString", (char*) taskStatusString, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskStop", (char*) taskStop, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskStopForce", (char*) taskStopForce, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskStopMsg", (char*) &taskStopMsg, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "taskSuspend", (char*) taskSuspend, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskSwapHookAdd", (char*) taskSwapHookAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskSwapHookAttach", (char*) taskSwapHookAttach, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskSwapHookDelete", (char*) taskSwapHookDelete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskSwapHookDetach", (char*) taskSwapHookDetach, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskSwapHookShow", (char*) taskSwapHookShow, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskSwapReference", (char*) &taskSwapReference, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "taskSwapTable", (char*) &taskSwapTable, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "taskSwitchHookAdd", (char*) taskSwitchHookAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskSwitchHookDelete", (char*) taskSwitchHookDelete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskSwitchHookShow", (char*) taskSwitchHookShow, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskSwitchTable", (char*) &taskSwitchTable, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "taskTcb", (char*) taskTcb, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskTerminate", (char*) taskTerminate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskUndelay", (char*) taskUndelay, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskUnlock", (char*) taskUnlock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskUnsafe", (char*) taskUnsafe, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskUnsafeInternal", (char*) taskUnsafeInternal, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskUserMmuCtxSet", (char*) taskUserMmuCtxSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskUsrExcStackSize", (char*) &taskUsrExcStackSize, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "taskUsrExcStkOverflowSize", (char*) &taskUsrExcStkOverflowSize, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "taskUsrExecStkOverflowSize", (char*) &taskUsrExecStkOverflowSize, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "taskUsrExecStkUnderflowSize", (char*) &taskUsrExecStkUnderflowSize, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "taskVarAdd", (char*) taskVarAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskVarDelete", (char*) taskVarDelete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskVarGet", (char*) taskVarGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskVarInfo", (char*) taskVarInfo, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskVarInit", (char*) taskVarInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskVarSet", (char*) taskVarSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskWait", (char*) taskWait, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskWaitShow", (char*) taskWaitShow, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "taskWaitShowCoreRtn", (char*) taskWaitShowCoreRtn, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "tcbReserveBitmap", (char*) &tcbReserveBitmap, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "tcbSpareBitmap", (char*) &tcbSpareBitmap, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "tcp_tmr_string", (char*) &tcp_tmr_string, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "td", (char*) td, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "tgec0Resources", (char*) &tgec0Resources, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "tgec1Resources", (char*) &tgec1Resources, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "tgecMdio0Resources", (char*) &tgecMdio0Resources, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "ti", (char*) ti, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "tick64Get", (char*) tick64Get, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "tick64Set", (char*) tick64Set, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "tickAnnounce", (char*) tickAnnounce, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "tickAnnounceHookAdd", (char*) tickAnnounceHookAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "tickGet", (char*) tickGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "tickQHead", (char*) &tickQHead, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "tickSet", (char*) tickSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "time", (char*) time, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "timeFiles", (char*) &timeFiles, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "timex", (char*) timex, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "timexClear", (char*) timexClear, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "timexFunc", (char*) timexFunc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "timexHelp", (char*) timexHelp, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "timexInit", (char*) timexInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "timexN", (char*) timexN, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "timexPost", (char*) timexPost, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "timexPre", (char*) timexPre, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "timexShow", (char*) timexShow, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "tip", (char*) tip, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "tipConfigString", (char*) &tipConfigString, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "tipEscapeChar", (char*) &tipEscapeChar, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "tipLibInit", (char*) tipLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "tipStart", (char*) tipStart, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "tmpfile", (char*) tmpfile, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "tmpnam", (char*) tmpnam, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "tolower", (char*) tolower, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "totalStaticDesc", (char*) &totalStaticDesc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "toupper", (char*) toupper, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "tr", (char*) tr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "trcDefaultArgs", (char*) &trcDefaultArgs, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "trcLibFuncs", (char*) &trcLibFuncs, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "trcScanDepth", (char*) &trcScanDepth, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "trcStack", (char*) trcStack, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "trcStackCtxTrace", (char*) trcStackCtxTrace, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "trcStackTrace", (char*) trcStackTrace, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ts", (char*) ts, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "tt", (char*) tt, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ttyDevCreate", (char*) ttyDevCreate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ttyDrv", (char*) ttyDrv, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "tw", (char*) tw, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "tyAbortFuncGet", (char*) tyAbortFuncGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "tyAbortFuncSet", (char*) tyAbortFuncSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "tyAbortGet", (char*) tyAbortGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "tyAbortSet", (char*) tyAbortSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "tyBackspaceChar", (char*) &tyBackspaceChar, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "tyBackspaceSet", (char*) tyBackspaceSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "tyDeleteLineChar", (char*) &tyDeleteLineChar, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "tyDeleteLineSet", (char*) tyDeleteLineSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "tyDevInit", (char*) tyDevInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "tyDevRemove", (char*) tyDevRemove, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "tyDevTerminate", (char*) tyDevTerminate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "tyEOFGet", (char*) tyEOFGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "tyEOFSet", (char*) tyEOFSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "tyEofChar", (char*) &tyEofChar, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "tyIRd", (char*) tyIRd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "tyITx", (char*) tyITx, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "tyIoctl", (char*) tyIoctl, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "tyLibInit", (char*) tyLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "tyMonitorTrapSet", (char*) tyMonitorTrapSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "tyRead", (char*) tyRead, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "tyWrite", (char*) tyWrite, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "tyXoffHookSet", (char*) tyXoffHookSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "ungetc", (char*) ungetc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "unld", (char*) unld, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "unlink", (char*) unlink, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "unmount", (char*) unmount, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "usrAimMmuConfig", (char*) usrAimMmuConfig, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "usrBanner", (char*) usrBanner, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "usrBootLineCrack", (char*) usrBootLineCrack, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "usrBootLineGet", (char*) usrBootLineGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "usrBootLineInit", (char*) usrBootLineInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "usrBootLineParse", (char*) usrBootLineParse, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "usrBreakpointInit", (char*) usrBreakpointInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "usrBreakpointSet", (char*) usrBreakpointSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "usrCacheEnable", (char*) usrCacheEnable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "usrCheckNetBootConfig", (char*) usrCheckNetBootConfig, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "usrClock", (char*) usrClock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "usrExtraModules", (char*) &usrExtraModules, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "usrFtpInit", (char*) usrFtpInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "usrInit", (char*) usrInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "usrIsrDeferInit", (char*) usrIsrDeferInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "usrKernelCoreInit", (char*) usrKernelCoreInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "usrKernelCorePreInit", (char*) usrKernelCorePreInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "usrKernelCoreShowInit", (char*) usrKernelCoreShowInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "usrKernelCreateInit", (char*) usrKernelCreateInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "usrKernelInit", (char*) usrKernelInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "usrLibInit", (char*) usrLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "usrMmuInit", (char*) usrMmuInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "usrModuleLoad", (char*) usrModuleLoad, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "usrNetAddrInit", (char*) usrNetAddrInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "usrNetAppInit", (char*) usrNetAppInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "usrNetApplUtilInit", (char*) usrNetApplUtilInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "usrNetBoot", (char*) usrNetBoot, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "usrNetDaemonInit", (char*) usrNetDaemonInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "usrNetDevNameGet", (char*) usrNetDevNameGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "usrNetEndLibInit", (char*) usrNetEndLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "usrNetHostAdd", (char*) usrNetHostAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "usrNetHostSetup", (char*) usrNetHostSetup, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "usrNetInit", (char*) usrNetInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "usrNetProtoInit", (char*) usrNetProtoInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "usrNetRemoteCreate", (char*) usrNetRemoteCreate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "usrNetRemoteInit", (char*) usrNetRemoteInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "usrNetSocketInit", (char*) usrNetSocketInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "usrNetUtilsInit", (char*) usrNetUtilsInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "usrNetmaskGet", (char*) usrNetmaskGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "usrNetworkExtraModules", (char*) &usrNetworkExtraModules, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "usrRoot", (char*) usrRoot, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "usrShell", (char*) usrShell, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "usrStandaloneInit", (char*) usrStandaloneInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "usrSysSymTblInit", (char*) usrSysSymTblInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "usrTipInit", (char*) usrTipInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "usrVxbTimerSysInit", (char*) usrVxbTimerSysInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "usrVxdbgInit", (char*) usrVxdbgInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "uswab", (char*) uswab, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "utime", (char*) utime, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "valloc", (char*) valloc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vecOffRelocMatch", (char*) vecOffRelocMatch, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vecOffRelocMatchRev", (char*) vecOffRelocMatchRev, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "version", (char*) version, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vfdprintf", (char*) vfdprintf, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vfprintf", (char*) vfprintf, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "viLedLibInit", (char*) viLedLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vmAttrShow", (char*) vmAttrShow, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vmAttrStringGet", (char*) vmAttrStringGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vmAttrToArchConvert", (char*) vmAttrToArchConvert, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vmAttrToIndepConvert", (char*) vmAttrToIndepConvert, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vmBaseLibInit", (char*) vmBaseLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vmBasePageSizeGet", (char*) vmBasePageSizeGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vmBaseStateSet", (char*) vmBaseStateSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vmBufferWrite", (char*) vmBufferWrite, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vmContextClassId", (char*) &vmContextClassId, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vmContextHeaderPrint", (char*) vmContextHeaderPrint, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vmContextRangeShow", (char*) vmContextRangeShow, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vmContextShow", (char*) vmContextShow, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vmContigBlockEach", (char*) vmContigBlockEach, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vmCurrentGet", (char*) vmCurrentGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vmGlobalMapInit", (char*) vmGlobalMapInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vmInfoGet", (char*) vmInfoGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vmKernelContextIdGet", (char*) vmKernelContextIdGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vmLibInfo", (char*) &vmLibInfo, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vmMap", (char*) vmMap, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vmMaxPhysBitsGet", (char*) vmMaxPhysBitsGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vmOptimizedSizeGet", (char*) vmOptimizedSizeGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vmPageLock", (char*) vmPageLock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vmPageMap", (char*) vmPageMap, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vmPageOptimize", (char*) vmPageOptimize, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vmPageSizeGet", (char*) vmPageSizeGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vmPageUnlock", (char*) vmPageUnlock, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vmPageUnmap", (char*) vmPageUnmap, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vmPgMap", (char*) vmPgMap, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vmPgUnMap", (char*) vmPgUnMap, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vmPhysTranslate", (char*) vmPhysTranslate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vmPteSizeGet", (char*) vmPteSizeGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vmShowInit", (char*) vmShowInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vmStateGet", (char*) vmStateGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vmStateSet", (char*) vmStateSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vmTextProtect", (char*) vmTextProtect, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vmTranslate", (char*) vmTranslate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vmpxx", (char*) vmpxx, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "voprintf", (char*) voprintf, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vprintf", (char*) vprintf, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vsnprintf", (char*) vsnprintf, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vsprintf", (char*) vsprintf, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxAbsTicks", (char*) &vxAbsTicks, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "vxAtomic32Add", (char*) vxAtomic32Add, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxAtomic32And", (char*) vxAtomic32And, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxAtomic32Cas", (char*) vxAtomic32Cas, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxAtomic32Clear", (char*) vxAtomic32Clear, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxAtomic32Dec", (char*) vxAtomic32Dec, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxAtomic32Get", (char*) vxAtomic32Get, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxAtomic32Inc", (char*) vxAtomic32Inc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxAtomic32Nand", (char*) vxAtomic32Nand, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxAtomic32Or", (char*) vxAtomic32Or, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxAtomic32Set", (char*) vxAtomic32Set, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxAtomic32Sub", (char*) vxAtomic32Sub, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxAtomic32Xor", (char*) vxAtomic32Xor, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxAtomic64Add", (char*) vxAtomic64Add, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxAtomic64And", (char*) vxAtomic64And, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxAtomic64Cas", (char*) vxAtomic64Cas, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxAtomic64Clear", (char*) vxAtomic64Clear, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxAtomic64Dec", (char*) vxAtomic64Dec, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxAtomic64Get", (char*) vxAtomic64Get, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxAtomic64Inc", (char*) vxAtomic64Inc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxAtomic64Nand", (char*) vxAtomic64Nand, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxAtomic64Or", (char*) vxAtomic64Or, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxAtomic64Set", (char*) vxAtomic64Set, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxAtomic64Sub", (char*) vxAtomic64Sub, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxAtomic64Xor", (char*) vxAtomic64Xor, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxAtomicAdd", (char*) vxAtomicAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxAtomicAnd", (char*) vxAtomicAnd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxAtomicCas", (char*) vxAtomicCas, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxAtomicClear", (char*) vxAtomicClear, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxAtomicDec", (char*) vxAtomicDec, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxAtomicGet", (char*) vxAtomicGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxAtomicInc", (char*) vxAtomicInc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxAtomicNand", (char*) vxAtomicNand, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxAtomicOr", (char*) vxAtomicOr, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxAtomicSet", (char*) vxAtomicSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxAtomicSub", (char*) vxAtomicSub, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxAtomicXor", (char*) vxAtomicXor, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxBaseCpuPhysIndex", (char*) &vxBaseCpuPhysIndex, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxBucsrGet", (char*) vxBucsrGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxBusIncluded", (char*) &vxBusIncluded, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxC1StateSet", (char*) vxC1StateSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxCas", (char*) vxCas, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxCpuConfigured", (char*) &vxCpuConfigured, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxCpuConfiguredGet", (char*) vxCpuConfiguredGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxCpuEnabled", (char*) &vxCpuEnabled, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxCpuEnabledGet", (char*) vxCpuEnabledGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxCpuIdGet", (char*) vxCpuIdGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxCpuIdToPhysIndex", (char*) vxCpuIdToPhysIndex, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxCpuIndexGet", (char*) vxCpuIndexGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxCpuLibInit", (char*) vxCpuLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxCpuPhysIndexGet", (char*) vxCpuPhysIndexGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxCpuPhysIndexToId", (char*) vxCpuPhysIndexToId, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxCpuReserve", (char*) vxCpuReserve, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxCpuReservedGet", (char*) vxCpuReservedGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxCpuSetReserved", (char*) &vxCpuSetReserved, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxCpuSetReservedAvail", (char*) &vxCpuSetReservedAvail, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "vxCpuUnreserve", (char*) vxCpuUnreserve, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxCsrr0Get", (char*) vxCsrr0Get, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxCsrr0Set", (char*) vxCsrr0Set, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxCsrr1Get", (char*) vxCsrr1Get, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxCsrr1Set", (char*) vxCsrr1Set, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxDac1Get", (char*) vxDac1Get, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxDac1Set", (char*) vxDac1Set, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxDac2Get", (char*) vxDac2Get, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxDac2Set", (char*) vxDac2Set, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxDbcr0Get", (char*) vxDbcr0Get, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxDbcr0Set", (char*) vxDbcr0Set, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxDbcr1Get", (char*) vxDbcr1Get, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxDbcr1Set", (char*) vxDbcr1Set, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxDbcr2Get", (char*) vxDbcr2Get, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxDbcr2Set", (char*) vxDbcr2Set, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxDbsrClear", (char*) vxDbsrClear, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxDbsrGet", (char*) vxDbsrGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxDearGet", (char*) vxDearGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxDearSet", (char*) vxDearSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxDecGet", (char*) vxDecGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxDecIntAck", (char*) vxDecIntAck, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxDecIntEnable", (char*) vxDecIntEnable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxDecSet", (char*) vxDecSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxDecarSet", (char*) vxDecarSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxDisableBP", (char*) vxDisableBP, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxEieio", (char*) vxEieio, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxEnableBP", (char*) vxEnableBP, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxEventPendQ", (char*) &vxEventPendQ, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxFirstBit", (char*) vxFirstBit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxFirstRBit", (char*) vxFirstRBit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxFitIntAck", (char*) vxFitIntAck, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxFitIntDisable", (char*) vxFitIntDisable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxFitIntEnable", (char*) vxFitIntEnable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxFpscrGet", (char*) vxFpscrGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxFpscrSet", (char*) vxFpscrSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxHid0Get", (char*) vxHid0Get, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxHid0Set", (char*) vxHid0Set, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxIOArchProbe", (char*) vxIOArchProbe, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxIac1Get", (char*) vxIac1Get, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxIac1Set", (char*) vxIac1Set, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxIac2Get", (char*) vxIac2Get, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxIac2Set", (char*) vxIac2Set, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxIntStackBase", (char*) &vxIntStackBase, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "vxIntStackEnd", (char*) &vxIntStackEnd, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "vxIntStackOverflowSize", (char*) &vxIntStackOverflowSize, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxIntStackUnderflowSize", (char*) &vxIntStackUnderflowSize, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxIpiControlGet_desc", (char*) &vxIpiControlGet_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxIvor1Set", (char*) vxIvor1Set, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxIvprGet", (char*) vxIvprGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxIvprSet", (char*) vxIvprSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxL1CFG0Get", (char*) vxL1CFG0Get, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxL1CFG1Get", (char*) vxL1CFG1Get, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxL1CSR0Get", (char*) vxL1CSR0Get, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxL1CSR0Set", (char*) vxL1CSR0Set, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxL1CSR1Get", (char*) vxL1CSR1Get, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxL1CSR1Set", (char*) vxL1CSR1Set, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxL1CSR2Get", (char*) vxL1CSR2Get, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxL1CSR2Set", (char*) vxL1CSR2Set, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxL2CFG0Get", (char*) vxL2CFG0Get, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxL2CSR0Get", (char*) vxL2CSR0Get, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxL2CSR0Set", (char*) vxL2CSR0Set, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxL2ERRCTLGet", (char*) vxL2ERRCTLGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxL2ERRCTLSet", (char*) vxL2ERRCTLSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxL2ERRDETGet", (char*) vxL2ERRDETGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxL2ERRDETSet", (char*) vxL2ERRDETSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxL2ERRDISGet", (char*) vxL2ERRDISGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxL2ERRDISSet", (char*) vxL2ERRDISSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxL2ERRINJCTLGet", (char*) vxL2ERRINJCTLGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxL2ERRINJCTLSet", (char*) vxL2ERRINJCTLSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxL2ERRINTENGet", (char*) vxL2ERRINTENGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxL2ERRINTENSet", (char*) vxL2ERRINTENSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxMcsrr0Get", (char*) vxMcsrr0Get, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxMcsrr0Set", (char*) vxMcsrr0Set, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxMcsrr1Get", (char*) vxMcsrr1Get, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxMcsrr1Set", (char*) vxMcsrr1Set, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxMemArchProbe", (char*) vxMemArchProbe, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxMemArchProbeInit", (char*) vxMemArchProbeInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxMemProbe", (char*) vxMemProbe, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxMemProbeInit", (char*) vxMemProbeInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxMemProbeSup", (char*) vxMemProbeSup, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxMsrGet", (char*) vxMsrGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxMsrSet", (char*) vxMsrSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxPirGet", (char*) vxPirGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxPirSet", (char*) vxPirSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxPpcDecTimerMaxCnt", (char*) &vxPpcDecTimerMaxCnt, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxPpcIntMask", (char*) &vxPpcIntMask, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxPvrGet", (char*) vxPvrGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxSdaInit", (char*) vxSdaInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxSdaSet", (char*) vxSdaSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxSrr0Get", (char*) vxSrr0Get, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxSrr0Set", (char*) vxSrr0Set, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxSrr1Get", (char*) vxSrr1Get, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxSrr1Set", (char*) vxSrr1Set, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxSvrGet", (char*) vxSvrGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxTas", (char*) vxTas, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxTaskEntry", (char*) vxTaskEntry, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxTcrGet", (char*) vxTcrGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxTcrSet", (char*) vxTcrSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxTimeBaseGet", (char*) vxTimeBaseGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxTimeBaseSet", (char*) vxTimeBaseSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxTsrGet", (char*) vxTsrGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxTsrSet", (char*) vxTsrSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxWorksVersion", (char*) &vxWorksVersion, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxWorksVersionMaint", (char*) &vxWorksVersionMaint, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxWorksVersionMajor", (char*) &vxWorksVersionMajor, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxWorksVersionMinor", (char*) &vxWorksVersionMinor, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxWorksVersionSvcPk", (char*) &vxWorksVersionSvcPk, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxbBusAnnounce", (char*) vxbBusAnnounce, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbBusListLock", (char*) &vxbBusListLock, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxbBusTypeRegister", (char*) vxbBusTypeRegister, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbBusTypeString", (char*) vxbBusTypeString, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbBusTypeUnregister", (char*) vxbBusTypeUnregister, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbDelay", (char*) vxbDelay, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbDelayLibInit", (char*) vxbDelayLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbDelayTimerFreeRunning", (char*) &vxbDelayTimerFreeRunning, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxbDevConnect", (char*) vxbDevConnect, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbDevConnectInternal", (char*) vxbDevConnectInternal, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbDevInit", (char*) vxbDevInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbDevInitInternal", (char*) vxbDevInitInternal, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbDevIntCapabCheck_desc", (char*) &vxbDevIntCapabCheck_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxbDevIterate", (char*) vxbDevIterate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbDevMethodGet", (char*) vxbDevMethodGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbDevMethodRun", (char*) vxbDevMethodRun, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbDevParent", (char*) vxbDevParent, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbDevPath", (char*) vxbDevPath, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbDevRegMap_desc", (char*) &vxbDevRegMap_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxbDevRegister", (char*) vxbDevRegister, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbDevRemovalAnnounce", (char*) vxbDevRemovalAnnounce, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbDevStructAlloc", (char*) vxbDevStructAlloc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbDevStructFree", (char*) vxbDevStructFree, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbDevStructListLock", (char*) &vxbDevStructListLock, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxbDeviceAnnounce", (char*) vxbDeviceAnnounce, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbDeviceDriverRelease", (char*) vxbDeviceDriverRelease, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbDeviceMethodRun", (char*) vxbDeviceMethodRun, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbDmaBufArchFlush", (char*) &vxbDmaBufArchFlush, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxbDmaBufArchInvalidate", (char*) &vxbDmaBufArchInvalidate, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxbDmaBufBspAlloc", (char*) &vxbDmaBufBspAlloc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxbDmaBufBspFree", (char*) &vxbDmaBufBspFree, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxbDmaBufFlush", (char*) vxbDmaBufFlush, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbDmaBufFragFill", (char*) vxbDmaBufFragFill, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbDmaBufInit", (char*) vxbDmaBufInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbDmaBufInvalidate", (char*) vxbDmaBufInvalidate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbDmaBufMapArchFlush", (char*) &vxbDmaBufMapArchFlush, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxbDmaBufMapArchInvalidate", (char*) &vxbDmaBufMapArchInvalidate, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxbDmaBufMapCreate", (char*) vxbDmaBufMapCreate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbDmaBufMapCreate_desc", (char*) &vxbDmaBufMapCreate_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxbDmaBufMapDestroy", (char*) vxbDmaBufMapDestroy, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbDmaBufMapDestroy_desc", (char*) &vxbDmaBufMapDestroy_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxbDmaBufMapFlush", (char*) vxbDmaBufMapFlush, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbDmaBufMapInvalidate", (char*) vxbDmaBufMapInvalidate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbDmaBufMapIoVecLoad", (char*) vxbDmaBufMapIoVecLoad, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbDmaBufMapLoad", (char*) vxbDmaBufMapLoad, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbDmaBufMapMblkLoad", (char*) vxbDmaBufMapMblkLoad, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbDmaBufMapMemAlloc_desc", (char*) &vxbDmaBufMapMemAlloc_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxbDmaBufMapMemFree_desc", (char*) &vxbDmaBufMapMemFree_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxbDmaBufMapSync", (char*) vxbDmaBufMapSync, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbDmaBufMapUnload", (char*) vxbDmaBufMapUnload, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbDmaBufMemAlloc", (char*) vxbDmaBufMemAlloc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbDmaBufMemFree", (char*) vxbDmaBufMemFree, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbDmaBufSync", (char*) vxbDmaBufSync, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbDmaBufTagCreate", (char*) vxbDmaBufTagCreate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbDmaBufTagDestroy", (char*) vxbDmaBufTagDestroy, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbDmaBufTagGet_desc", (char*) &vxbDmaBufTagGet_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxbDmaBufTagParentGet", (char*) vxbDmaBufTagParentGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbDmaChanAlloc", (char*) vxbDmaChanAlloc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbDmaChanFree", (char*) vxbDmaChanFree, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbDmaLibInit", (char*) vxbDmaLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbDmaResDedicatedGet_desc", (char*) &vxbDmaResDedicatedGet_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxbDmaResourceGet_desc", (char*) &vxbDmaResourceGet_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxbDmaResourceRelease_desc", (char*) &vxbDmaResourceRelease_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxbDriverUnregister", (char*) vxbDriverUnregister, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbDrvRescan", (char*) vxbDrvRescan, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbDrvUnlink_desc", (char*) &vxbDrvUnlink_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxbDrvVerCheck", (char*) vxbDrvVerCheck, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbDtsecEndRegister", (char*) vxbDtsecEndRegister, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbEpicEPRGet", (char*) &vxbEpicEPRGet, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "vxbEpicIntAckReg", (char*) &vxbEpicIntAckReg, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "vxbEpicIntCtlrRegister", (char*) vxbEpicIntCtlrRegister, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbEpicIpiDpatchReg0", (char*) &vxbEpicIpiDpatchReg0, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "vxbEpicIpiGen", (char*) vxbEpicIpiGen, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbEpicSharedMsgHandler", (char*) vxbEpicSharedMsgHandler, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbEpicSpuriousVector", (char*) &vxbEpicSpuriousVector, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxbGlobalListsLock", (char*) &vxbGlobalListsLock, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxbInit", (char*) vxbInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbInstByNameFind", (char*) vxbInstByNameFind, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbInstParamByIndexGet", (char*) vxbInstParamByIndexGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbInstParamByNameGet", (char*) vxbInstParamByNameGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbInstParamSet", (char*) vxbInstParamSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbInstUnitGet", (char*) vxbInstUnitGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbInstUnitSet", (char*) vxbInstUnitSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbInstUsrOptionGet", (char*) vxbInstUsrOptionGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbIntAcknowledge", (char*) vxbIntAcknowledge, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbIntAlloc", (char*) vxbIntAlloc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbIntConnect", (char*) vxbIntConnect, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbIntCtlrAck_desc", (char*) &vxbIntCtlrAck_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxbIntCtlrAlloc_desc", (char*) &vxbIntCtlrAlloc_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxbIntCtlrConnect_desc", (char*) &vxbIntCtlrConnect_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxbIntCtlrCpuAvail_desc", (char*) &vxbIntCtlrCpuAvail_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxbIntCtlrCpuDisable_desc", (char*) &vxbIntCtlrCpuDisable_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxbIntCtlrDisable_desc", (char*) &vxbIntCtlrDisable_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxbIntCtlrDisconnect_desc", (char*) &vxbIntCtlrDisconnect_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxbIntCtlrEnable_desc", (char*) &vxbIntCtlrEnable_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxbIntCtlrFree_desc", (char*) &vxbIntCtlrFree_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxbIntDisable", (char*) vxbIntDisable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbIntDisconnect", (char*) vxbIntDisconnect, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbIntDynaConnect", (char*) vxbIntDynaConnect, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbIntDynaCtlrInit", (char*) vxbIntDynaCtlrInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbIntDynaVecAlloc", (char*) vxbIntDynaVecAlloc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbIntDynaVecAlloc_desc", (char*) &vxbIntDynaVecAlloc_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxbIntDynaVecConnect_desc", (char*) &vxbIntDynaVecConnect_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxbIntDynaVecDevMultiProgram", (char*) vxbIntDynaVecDevMultiProgram, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbIntDynaVecDevMultiProgram_desc", (char*) &vxbIntDynaVecDevMultiProgram_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxbIntDynaVecDisable_desc", (char*) &vxbIntDynaVecDisable_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxbIntDynaVecEnable_desc", (char*) &vxbIntDynaVecEnable_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxbIntDynaVecErase", (char*) vxbIntDynaVecErase, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbIntDynaVecErase_desc", (char*) &vxbIntDynaVecErase_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxbIntDynaVecGet_desc", (char*) &vxbIntDynaVecGet_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxbIntDynaVecMultiConnect", (char*) vxbIntDynaVecMultiConnect, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbIntDynaVecMultiConnect_desc", (char*) &vxbIntDynaVecMultiConnect_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxbIntDynaVecOwnerFind", (char*) vxbIntDynaVecOwnerFind, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbIntDynaVecOwnerFind_desc", (char*) &vxbIntDynaVecOwnerFind_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxbIntDynaVecProgram_desc", (char*) &vxbIntDynaVecProgram_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxbIntEnable", (char*) vxbIntEnable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbIntFree", (char*) vxbIntFree, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbIntVectorGet", (char*) vxbIntVectorGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbLibError", (char*) vxbLibError, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbLibInit", (char*) vxbLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbLockDelete", (char*) vxbLockDelete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbLockGive", (char*) vxbLockGive, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbLockInit", (char*) vxbLockInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbLockTake", (char*) vxbLockTake, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbM85xxTimerLock", (char*) &vxbM85xxTimerLock, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxbMsDelay", (char*) vxbMsDelay, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbMsiConnect", (char*) vxbMsiConnect, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbNextUnitGet", (char*) vxbNextUnitGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbPpcIntCtlrISR_CRTL", (char*) vxbPpcIntCtlrISR_CRTL, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbPpcIntCtlrISR_FIT", (char*) vxbPpcIntCtlrISR_FIT, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbPpcIntCtlrISR_INTR", (char*) vxbPpcIntCtlrISR_INTR, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbPpcIntCtlrISR_SYSTIMER", (char*) vxbPpcIntCtlrISR_SYSTIMER, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbPpcIntCtlrISR_WD", (char*) vxbPpcIntCtlrISR_WD, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbRead16", (char*) vxbRead16, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbRead32", (char*) vxbRead32, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbRead64", (char*) vxbRead64, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbRead8", (char*) vxbRead8, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbRegMap", (char*) vxbRegMap, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbRegUnmap", (char*) vxbRegUnmap, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbResourceFind", (char*) vxbResourceFind, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbSerialChanGet", (char*) vxbSerialChanGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbSpecialRead16", (char*) vxbSpecialRead16, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbSpecialRead32", (char*) vxbSpecialRead32, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbSpecialRead8", (char*) vxbSpecialRead8, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbSpecialWrite16", (char*) vxbSpecialWrite16, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbSpecialWrite32", (char*) vxbSpecialWrite32, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbSpecialWrite8", (char*) vxbSpecialWrite8, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbSubDevAction", (char*) vxbSubDevAction, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbSwap16", (char*) vxbSwap16, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbSwap32", (char*) vxbSwap32, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbSwap64", (char*) vxbSwap64, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbSysClkConnect", (char*) vxbSysClkConnect, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbSysClkDisable", (char*) vxbSysClkDisable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbSysClkEnable", (char*) vxbSysClkEnable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbSysClkLibInit", (char*) vxbSysClkLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbSysClkRateGet", (char*) vxbSysClkRateGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbSysClkRateSet", (char*) vxbSysClkRateSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbSysEpicIntHandler", (char*) vxbSysEpicIntHandler, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbTimerAlloc", (char*) vxbTimerAlloc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbTimerFeaturesGet", (char*) vxbTimerFeaturesGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbTimerFuncGet_desc", (char*) &vxbTimerFuncGet_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxbTimerNumberGet_desc", (char*) &vxbTimerNumberGet_desc, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxbTimerRelease", (char*) vxbTimerRelease, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbUsDelay", (char*) vxbUsDelay, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbWrite16", (char*) vxbWrite16, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbWrite32", (char*) vxbWrite32, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbWrite64", (char*) vxbWrite64, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxbWrite8", (char*) vxbWrite8, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxdbgBpAdd", (char*) vxdbgBpAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxdbgBpAddrCheck", (char*) vxdbgBpAddrCheck, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxdbgBpAllDisable", (char*) vxdbgBpAllDisable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxdbgBpDelete", (char*) vxdbgBpDelete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxdbgBpFind", (char*) vxdbgBpFind, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxdbgBpList", (char*) &vxdbgBpList, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "vxdbgBpMsgDrop", (char*) vxdbgBpMsgDrop, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxdbgBpMsgGet", (char*) vxdbgBpMsgGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxdbgBpMsgPost", (char*) vxdbgBpMsgPost, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxdbgBpMsgQCreate", (char*) vxdbgBpMsgQCreate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxdbgBpMsgQHdlrIsRunning", (char*) vxdbgBpMsgQHdlrIsRunning, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxdbgClntRegister", (char*) vxdbgClntRegister, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxdbgClntUnregister", (char*) vxdbgClntUnregister, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxdbgCont", (char*) vxdbgCont, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxdbgEnable", (char*) vxdbgEnable, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxdbgEventIdAlloc", (char*) vxdbgEventIdAlloc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxdbgEventIdFree", (char*) vxdbgEventIdFree, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxdbgEventInject", (char*) vxdbgEventInject, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxdbgEventLibInit", (char*) vxdbgEventLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxdbgHookAdd", (char*) vxdbgHookAdd, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxdbgHookInvoke", (char*) vxdbgHookInvoke, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxdbgHooksInit", (char*) vxdbgHooksInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxdbgInstrCmp", (char*) vxdbgInstrCmp, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxdbgInternalEventInject", (char*) vxdbgInternalEventInject, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxdbgLibInit", (char*) vxdbgLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxdbgListDelete", (char*) vxdbgListDelete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxdbgListInsert", (char*) vxdbgListInsert, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxdbgLockUnbreakable", (char*) &vxdbgLockUnbreakable, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "vxdbgMemAccess", (char*) vxdbgMemAccess, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxdbgMemCtxSwitch", (char*) vxdbgMemCtxSwitch, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxdbgMemMove", (char*) vxdbgMemMove, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxdbgMemRead", (char*) vxdbgMemRead, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxdbgMemScan", (char*) vxdbgMemScan, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxdbgMemWrite", (char*) vxdbgMemWrite, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxdbgRunCtrlHookInvoke", (char*) vxdbgRunCtrlHookInvoke, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxdbgRunCtrlLibInit", (char*) vxdbgRunCtrlLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxdbgSafeUnbreakable", (char*) &vxdbgSafeUnbreakable, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "vxdbgStep", (char*) vxdbgStep, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxdbgStop", (char*) vxdbgStop, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxdbgTaskCont", (char*) vxdbgTaskCont, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxdbgTaskHookInit", (char*) vxdbgTaskHookInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxdbgTaskLibInit", (char*) vxdbgTaskLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxdbgTaskStep", (char*) vxdbgTaskStep, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxdbgTaskStop", (char*) vxdbgTaskStop, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxdbgUnbreakableOld", (char*) &vxdbgUnbreakableOld, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "vxfs_ramdisk_init_hook", (char*) &vxfs_ramdisk_init_hook, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "vxmux_mux_mblk_init", (char*) vxmux_mux_mblk_init, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxmux_null_buf_init", (char*) vxmux_null_buf_init, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "vxmux_null_pool", (char*) &vxmux_null_pool, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "w", (char*) w, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "wcstombs", (char*) wcstombs, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "wctomb", (char*) wctomb, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "wdCancel", (char*) wdCancel, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "wdClassId", (char*) &wdClassId, 0, 0,  SYM_GLOBAL | SYM_DATA},
        {{NULL}, "wdCreate", (char*) wdCreate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "wdCreateLibInit", (char*) wdCreateLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "wdDelete", (char*) wdDelete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "wdDestroy", (char*) wdDestroy, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "wdInit", (char*) wdInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "wdInitialize", (char*) wdInitialize, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "wdLibInit", (char*) wdLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "wdShow", (char*) wdShow, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "wdShowInit", (char*) wdShowInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "wdStart", (char*) wdStart, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "wdTerminate", (char*) wdTerminate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "wdTick", (char*) wdTick, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "wdbDbgArchInit", (char*) wdbDbgArchInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "wdbDbgDac1Get", (char*) wdbDbgDac1Get, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "wdbDbgDac1Set", (char*) wdbDbgDac1Set, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "wdbDbgDac2Get", (char*) wdbDbgDac2Get, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "wdbDbgDac2Set", (char*) wdbDbgDac2Set, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "wdbDbgDbcr0Get", (char*) wdbDbgDbcr0Get, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "wdbDbgDbcr0Set", (char*) wdbDbgDbcr0Set, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "wdbDbgDbcr1Get", (char*) wdbDbgDbcr1Get, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "wdbDbgDbcr1Set", (char*) wdbDbgDbcr1Set, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "wdbDbgDbcr2Get", (char*) wdbDbgDbcr2Get, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "wdbDbgDbcr2Set", (char*) wdbDbgDbcr2Set, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "wdbDbgDbsrGet", (char*) wdbDbgDbsrGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "wdbDbgDbsrSet", (char*) wdbDbgDbsrSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "wdbDbgGetNpc", (char*) wdbDbgGetNpc, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "wdbDbgHwAddrCheck", (char*) wdbDbgHwAddrCheck, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "wdbDbgHwBpFind", (char*) wdbDbgHwBpFind, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "wdbDbgHwBpHandle", (char*) wdbDbgHwBpHandle, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "wdbDbgHwBpSet", (char*) wdbDbgHwBpSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "wdbDbgHwBpStub", (char*) wdbDbgHwBpStub, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "wdbDbgIac1Get", (char*) wdbDbgIac1Get, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "wdbDbgIac1Set", (char*) wdbDbgIac1Set, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "wdbDbgIac2Get", (char*) wdbDbgIac2Get, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "wdbDbgIac2Set", (char*) wdbDbgIac2Set, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "wdbDbgRegsClear", (char*) wdbDbgRegsClear, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "wdbDbgRegsSet", (char*) wdbDbgRegsSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "wdbDbgSysCallCheck", (char*) wdbDbgSysCallCheck, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "wdbDbgTraceModeClear", (char*) wdbDbgTraceModeClear, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "wdbDbgTraceModeSet", (char*) wdbDbgTraceModeSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "wdbDbgTrap", (char*) wdbDbgTrap, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "whoami", (char*) whoami, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "windCont", (char*) windCont, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "windDelay", (char*) windDelay, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "windDelete", (char*) windDelete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "windExit", (char*) windExit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "windIntStackSet", (char*) windIntStackSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "windLoadContext", (char*) windLoadContext, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "windNullReturn", (char*) windNullReturn, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "windPendQFlush", (char*) windPendQFlush, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "windPendQGet", (char*) windPendQGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "windPendQPut", (char*) windPendQPut, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "windPendQRemove", (char*) windPendQRemove, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "windPendQTerminate", (char*) windPendQTerminate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "windPpcLoadRegSet", (char*) windPpcLoadRegSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "windPpcLoadVolatileRegSet", (char*) windPpcLoadVolatileRegSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "windPriNormalSet", (char*) windPriNormalSet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "windPrioritySet", (char*) windPrioritySet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "windReadyQPut", (char*) windReadyQPut, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "windReadyQRemove", (char*) windReadyQRemove, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "windResume", (char*) windResume, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "windSemDelete", (char*) windSemDelete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "windSemRWDelete", (char*) windSemRWDelete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "windStop", (char*) windStop, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "windStopClear", (char*) windStopClear, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "windSuspend", (char*) windSuspend, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "windTickAnnounce", (char*) windTickAnnounce, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "windTickAnnounceN", (char*) windTickAnnounceN, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "windTickWaitGet", (char*) windTickWaitGet, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "windUndelay", (char*) windUndelay, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "windWdCancel", (char*) windWdCancel, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "windWdStart", (char*) windWdStart, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "workQAdd0", (char*) workQAdd0, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "workQAdd1", (char*) workQAdd1, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "workQAdd2", (char*) workQAdd2, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "workQDoWork", (char*) workQDoWork, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "workQInit", (char*) workQInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "workQIsEmpty", (char*) &workQIsEmpty, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "workQIx", (char*) &workQIx, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "workQPanic", (char*) workQPanic, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "workQPanicHook", (char*) &workQPanicHook, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "workQWorkInProgress", (char*) &workQWorkInProgress, 0, 0,  SYM_GLOBAL | SYM_BSS},
        {{NULL}, "write", (char*) write, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "writev", (char*) writev, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "xattrib", (char*) xattrib, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "xcopy", (char*) xcopy, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "xdelete", (char*) xdelete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "zbufCreate", (char*) zbufCreate, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "zbufCut", (char*) zbufCut, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "zbufDelete", (char*) zbufDelete, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "zbufDup", (char*) zbufDup, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "zbufExtractCopy", (char*) zbufExtractCopy, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "zbufInsert", (char*) zbufInsert, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "zbufInsertBuf", (char*) zbufInsertBuf, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "zbufInsertBuf64", (char*) zbufInsertBuf64, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "zbufInsertCopy", (char*) zbufInsertCopy, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "zbufLength", (char*) zbufLength, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "zbufLibInit", (char*) zbufLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "zbufSegData", (char*) zbufSegData, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "zbufSegFind", (char*) zbufSegFind, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "zbufSegLength", (char*) zbufSegLength, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "zbufSegNext", (char*) zbufSegNext, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "zbufSegPrev", (char*) zbufSegPrev, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "zbufSockBufSend", (char*) zbufSockBufSend, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "zbufSockBufSend64", (char*) zbufSockBufSend64, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "zbufSockBufSendto", (char*) zbufSockBufSendto, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "zbufSockBufSendto64", (char*) zbufSockBufSendto64, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "zbufSockLibInit", (char*) zbufSockLibInit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "zbufSockRecv", (char*) zbufSockRecv, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "zbufSockRecvfrom", (char*) zbufSockRecvfrom, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "zbufSockSend", (char*) zbufSockSend, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "zbufSockSendto", (char*) zbufSockSendto, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "zbufSplit", (char*) zbufSplit, 0, 0,  SYM_GLOBAL | SYM_TEXT},
        {{NULL}, "zbuf_free", (char*) zbuf_free, 0, 0,  SYM_GLOBAL | SYM_TEXT},
    };


ULONG standTblSize = 4888;
