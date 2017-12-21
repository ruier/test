/* 00vxWorks.cdf - VxWorks components */

/*
 * Copyright (c) 1998-2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history     	
-------------------- 
16s,12jul13,pcs  Rename INCLUDE_MEM_CACHE to INCLUDE_MEM_PART_CACHE
16r,05may13,pcs  Add support for SMP-optimized memory allocator.
16r,17apr13,elp  added RUNTIME_CREATION_TIME parameter (WIND00295975)
16q,26mar13,cww  made task create/delete hook table size configurable
                 (WIND00395617)
16r,27mar13,s_z  Add INCLUDE_BOOT_HW_INIT component (WIND00390768)
16q,19mar13,y_y  reordering INCLUDE_SM_OBJ and INCLUDE_ERF init order
                 in usrIosExtraInit (WIND00408192)
16p,06nov12,pee  WIND00110061
16o,19oct12,cww  updated taskCreateHookInit() for SMP hooks
16n,18sep12,l_z  Update SYNOPSIS of INCLUDE_PCI_CFGSHOW. (WIND00108357)
16m,22aug12,mze  WIND00371707 - limit INTRINSICS to those built by VSB tools
16l,28feb12,kk   fix typo in ENABLE_ALL_CPUS (WIND00253761)
16k,18nov11,cww  Ensure sysExcMsg is updated when ED&R is invoked prior to
		 usrRoot (WIND00111567)
		 Moved usrSmpInit() before usrToolsInit() (WIND00309413)
16j,07feb12,l_z  Add INCLUDE_EEPROMDRV for EEPROM driver init
16i,29sep11,sem  WIND00307699 - Move pwr mgmt folder from hardware to os
16h,27sep11,pcs  Added initialization of components INCLUDE_MEM_EDR and
                 INCLUDE_MEM_EDR2 to the InitGroup usrRoot.
16g,20jul11,jlv  WIND00255352, add power policy manager
16f,27jun11,jxu  added PIPE_MAX_OPEN_FDS parameter (WIND00271309).
16e,15jun11,rec  change INCLUDE_POWER_MGMT_CPU_BSP_SUPPORT to
                 INCLUDE_TICKLESS_IDLE
16d,08jun11,zl   added INCLUDE_JITTER_TOOL
16c,03jun11,rec  WIND00255358 add device power management
16b,26may11,pcs  Add components corresponding to the boot sequence hook
                 callout mechanism.
16a,04may11,jpb  Added module & link sym to INCLUDE_STRING_SEARCH.
15z,15jun11,rec  change INCLUDE_POWER_MGMT_CPU_BSP_SUPPORT to
                 INCLUDE_TICKLESS_IDLE
15y,16feb11,v_r  Updated XXX_INTRINSICS synopsis for better clarity
                 (CQ:WIND00253540).
15x,25jan11,vhe  Fixed VSB/VIP compatibility (WIND00251938)
15w,23dec10,d_c  WIND00238183 - Move usrDebugKWriteInit() call after
                 sysHwInit()
15v,10nov10,dlk  Remove backslash-whitespace-newline and similar issues that
                 vxprj warns about.
15w,13jan11,w_x  Initial USB OTG support changes (WIND00211541)
15v,07dec10,ghs  Change USB component group name (WIND00245236)
15u,23nov10,rec  add INCLUDE_CPU_ODG_POLICY_MGMT component
15t,26nov10,ebh  added WRIFT init routine
15s,05jul10,liu  change fppShowInit to aimFppShowInit. (WIND00192339)
15r,13nov10,mze  replace _WRS_VX_SMP with _WRS_CONFIG_SMP
15q,12nov10,gvm  Fix for WIND00239961
15p,22oct10,rlp  Added INCLUDE_SYSDBG_FLAG component
15o,21oct10,jmp  added missing init order rule for INCLUDE_CORE_DUMP_RTP
		 (CQ:WIND00237388).
15r,20oct10,cww  Updated TASK_USER_EXC_STACK_SIZE for LP64
15r,29oct10,cww  added INIT_AFTER constraint for usrDebugKWriteInit
15q,19oct10,ghs  Combine USB class driver initialization into one group
                 (WIND00235953)
15p,18oct10,mze  remove trailing space
15o,08oct10,kk   remove INCLUDE_NBIO_LOG component, no longer used
15q,29sep10,gls  added INCLUDE_EDR_POLICY_HOOKS (WIND00234560)
15n,30sep10,ggz  Add password SHA256 hasing and salt support
15m,29sep10,pee  init order change for INCLUDE_USB_KEYBOARD_INIT
15l,22sep10,gvm  CPLUS component has addditional sources
15l,16sep10,gls  made FUNCPTR_OBJ_MEMALLOC_RTN default to memPartAlignedAlloc
15l,22sep10,gvm  CPLUS component has addditional sources
15k,13sep10,cww  Removed kprintf from usrInit group
15j,06sep10,vhe  Diab now compiles analysis DKMs. WB_RUNTIME_SUPPORT 
                 no longer requires GNU_INTRINSICS (WIND00230168)
15i,18aug10,pee  update init order for INCLUDE_SYSCTL
15i,09aug10,cww  Added INCLUDE_CACHE_COMPACT_DMA_SUPPORT to usrRoot
15h,27jul10,cww  Delayed kprintf initialization for 64-bit
15g,01jul10,cww  Delayed initialization of idle tasks
15f,15jun10,h_k  removed the ifndef conditions from INIT_ORDER in
                 usrIosExtraInit() which were added by mistake.
15e,08jun10,jxu  Removed the components (INCLUDE_PROCFS INCLUDE_TFFS 
                 INCLUDE_TFFS_MOUNT) from LP64 build.
15c,26may10,pee  adding standard List objects
15d,20may10,h_w  Add INCLUDE_AF_MIPC_SOCKETS component.
15c,18may10,pcs  Add support for device memory object.
15b,20may10,jpb  Fixed header file location for pthreadLibInit.
15a,26apr10,rec  CPU performance management update
14v,27apr10,sem  Adjust KERNEL_COMMON_HEAP_INIT_SIZE default value
14u,07apr10,gvm  break the link between icc and WB
14t,01apr10,jpb  Added user-reserved memory to init order.
14s,10mar10,gvm  updates for icc
14r,19feb10,zl   added INCLUDE_POSIX_SHM_API
                 INCLUDE_IO_MISC (WIND00167739)
		 initialization order for mmmap() components
14q,18feb10,rlp  Added INCLUDE_VXDBG component initialization.
14p,17feb10,pch  small footprint
14o,30jan10,h_w  Add INCLUDE_AF_MIPC_SOCKETS component
14n,26jan10,d_c  SMP M-N updates: add INCLUDE_VXCPULIB component.
14m,21jan10,zly  remove "REQUIRES INCLUDE_IO_RTP" from Component
14l,08jan10,s_s  Adding INCLUDE_ENV_VARS in usrRoot Init group after the memory
                 initialization and dropping it from usrKernelExtraInit Init
		 group (defect CQ:WIND00194940)
14k,30dec09,jpb  Remove INCLUDE_TASK_VARS from LP64 build.
14j,16dec09,zl   use objMemAllot() for FUNCPTR_OBJ_MEMALLOC_RTN.
14i,04nov09,pcs  Updated to reflect the name changes to the 64bit heaps.
14h,04nov09,h_k  added INCLUDE_VXBUS_DISK in usrIosExtraInit.
                 (WIND00189354)
14g,08oct09,cww  Added INCLUDE_DEFERRED_INTERRUPT_ENABLE and
                 INCLUDE_SYSHW_INIT_1
14f,07oct09,rlp  Modified INCLUDE_GNU_INTRINSICS dependency (CQ:WIND00183905).
14e,28sep09,cww  Delayed coredump init due to sysctl dependency
14d,25sep09,pcs  Replace memInit2 with memSysHeapCreate
14c,24sep09,jpb  Changed return value of clockLibInit.
14b,25aug09,cww  Added InitGroup usrKernelCreateInit
14a,08sep09,pcs  Updated to add support for multiple heaps for 64bit vxWorks.
13z,11aug09,cww  Updated ordering in usrRoot
13y,01apr09,zl   added INCLUDE_JOB_DEFER component.
13x,27feb09,zl   updated objLibInit() arguments.
13w,10feb09,zl   removed multi-way queue create/delete and qLib components
14s,08sep09,gls  added synopsis to VX_SMP_NUM_CPUS (WIND00160676)
14r,04sep09,s_s  Added INCLUDE_GETOPT component
14q,28aug09,gls  removed minimal semaphore components (WIND00179395)
14p,17aug09,hui  WIND00176466 - Fail to build VIP using max performance VSB
14o,14aug09,jpb  Defect# 161788. Removed INCLUDE_SET.
14n,12aug09,hui  WIND00176503 - workbench cant display FOLDER_HRFS
14m,07aug09,hui  WIND174908 - INCLUDE_IO_REMOVABLE require the wrong component
14l,13jul09,pch  Add support for INCLUDE_TM_TIMING_FRAMEWORK
14k,03jul09,x_s  added component INCLUDE_SYSHW_INIT2.
14j,17jun09,tcr  remove spinlock dependency from WindView
14i,11jun09,h_k  added default baud setting in kwriteSioInit(). (CQ:166577)
14h,08jun09,zly  remove "REQUIRES INCLUDE_IO_RTP" from Component
                 INCLUDE_IO_MISC
14g,02jun09,h_w  Added usrTipcInit to usrRoot if TIPC present (WIND00162024).
14h,13jul09,pch  Add support for INCLUDE_TM_TIMING_FRAMEWORK
14g,22apr09,jpb  Replaced setLib.h with setLibP.h.
14f,21apr09,s_s  Modifying the prototype declaration of the component
		 INCLUDE_DEBUG_KWRITE_USER
14e,21apr09,s_s  Added the PROTOTYPE field for the component
		 INCLUDE_DEBUG_KWRITE_USER. (Defect - WIND00162050)
14d,17apr09,kk   fix CQ:WIND00161840, add spinLockIsrNdInit to
		 INCLUDE_SPINLOCK_ISR_ND LINK_SYMS
14c,15apr09,h_k  cleaned up a compiler warning for kwriteSioInit.
14b,27mar09,s_s  Corrected typo Defect #WIND00160968. Also made the
		 Selection visible in the workbench.
14a,24mar09,gls  added INCLUDE_SPINLOCK_ND_TIMED
13z,18mar09,s_s  Added kernel-write components
13y,03feb09,cww  Add VX_SMP_CPU_EXPLICIT_RESERVE parameter
13x,23jan09,h_k  fixed REQUIRES attribute in INCLUDE_OBJECT_SHOW.
13w,12jan09,s_s  Added INCLUDE_OBJECT_SHOW component
13v,14nov08,zl   added missing multi-line terminator for SYNOPSIS
13u,10nov08,tcr  update INCLUDE_RBUFF to require INCLUDE_SPINLOCK_ISR_ND
                 (WIND00136245)
13t,24oct08,jpb  Defect WIND00136345.  Adding INCLUDE_SHOW_ROUTINES pulled in
                 components INCLUDE_POSIX_TIMER_SHOW and INCLUDE_CLASS_SHOW
                 when class list support not include.
13s,23oct08,zl   added VXEVENTS_OPTIONS configuration parameter.
13r,21oct08,kk   rename spinlockIsrNdLib to spinLockIsrNdLib
13q,20oct08,jpb  Defect 139871.  Added requirement on 
                 INCLUDE_POSIX_TIMER_SHOW.
13p,13oct08,jpb  Added requirement on INCLUDE_TASK_OPEN for
                 INCLUDE_POSIX_PTHREADS.  This is because pthreadCreate calls
                 taskOpen.
13o,23sep08,kab  Fix CQ:133672 - AMP IPI init must follow CPC
13n,25sep08,v_r  Decoupled tip from shell and network (def CQ:WIND00134830).
13m,23sep08,jmp  Added the max number of tasks that can be spied to
                 INCLUDE_SPY component (CQ:WIND00106559).
13l,18sep08,jmp  Added INCLUDE_VXDBG_CPU_CONTROL initialization to usrRoot()
                 (CQ:WIND00134567).
13k,11sep08,v_r	 Added INCLUDE_TIP component (req CQ:WIND00114857).
13j,04sep08,gls  divided INCLUDE_TASK_HOOKS component
13i,29aug08,zl   added TASK_PRIORITY_SET_COMPATIBLE
13h,03sep08,jpb  Removed LAYER from components.
13g,24apr08,tcr  update with eventpoints
13f,14aug08,jpb  Removed INCLUDE_TASK_LIST dependancy on INCLUDE_OBJ_INFO
13e,09jun08,s_z  Reorder initialization of USB Keyboard (Defect WIND00119420)
13d,07jul08,gls  began deprecation of minimal semaphores
13c,27jun08,cwu  insert INCLUDE_SM_OBJ in InitGroup of usrIosExtraInit for 
                 111681
13b,15may08,zl   changed INCLUDE_ENABLE_ALL_CPUS to INCLUDE_SMP_INIT.
13a,06may08,dtr  Add usrMultiosInit routine call if group present in usrRoot.
13a,26mar08,v_r  Added the RTP debug command facility component (req
                 CQ:WIND00004496).
12z,08jan08,dlk  Rename INCLUDE_SPINLOCK_FAST --> INCLUDE_SPINLOCK_ISR_ND.
12y,07jan08,gls  fast semaphores renamed to minimal semaphores
12x,21nov07,gls  added INCLUDE_SEM_FAST_BINARY and INCLUDE_SEM_FAST_MUTEX
	         added INCLUDE_SEM_FAST_BINARY_CREATE
		 added INCLUDE_SEM_FAST_MUTEX_CREATE
12w,11dec07,dlk  Added INCLUDE_SPINLOCK_FAST.
12w,09jan08,pcs  In InitGroup usrRoot, move INCLUDE_PROTECT_TEXT to after
                 INCLUDE_EDR_XXX (WIND00057363).
12v,04oct07,mze  fix for 104829
12u,27sep07,dcc  fixed defect WIND00105766: "The New IO Components Do Not Show
                 Up in WorkBench".
12t,13sep07,jpb  Added whitespace after "Name" in
                 INCLUDE_Q_PRI_BMAP_CREATE_DELETE.  Defect# 104480.
12s,07sep07,jmg  Added VX_SMP_NUM_CPUS.
12r,06sep07,act  fix typo in INCLUDE_IO_FILE_SYSTEM
12q,31aug07,pee  WIND101680. rename rand to stdrand.
12r,31aug07,act  io footprint work (from pcm, modified)
12q,31aug07,mmi  disable spinlock debugging by default
12p,27aug07,kk   fix INCLUDE_TASK_VARS and usage of #ifdef SMP on the
                 component
12p,27aug07,jmg  Added VX_ENABLE_CPU_TIMEOUT
12o,16aug07,pch  remove INCLUDE_TASK_VARS from SMP build
12n,15aug07,dlk  Accomodate relocation of spin lock code.
12m,08aug07,jmg  Added INCLUDE_TASK_ROTATE to FOLDER_KERNEL defect
                 #WIND00100884
12l,20jul07,kk   make ENABLE_ALL_CPUS a param of INCLUDE_KERNEL
12k,17jul07,kk   surround SMP components with ifdef SMP
12j,03jul07,rlp  Removed INCLUDE_BUF_MGR component.
12i,19jun07,gls  added INCLUDE_SEM_EXCHANGE
12h,11may07,jmg  Added INCLUDE_ATOMIC_OPERATORS to INCLUDE_LOGGING
12g,17may07,jlk  removed sfs components
12f,11may07,gls  added INCLUDE_TASK_TOOLS_UTIL
12e,07may07,h_k  added ISR_DEFER_MODE parameter.
12d,02may07,dbt  Enabled TLS support for MKL images.
12c,27apr07,kk   add requires on ATOMIC_OPERATORS for INCLUDE_TASK_CREATE_DELETE
12b,26apr07,pee  add source mode support with SMP
12a,06apr07,zl   added INCLUDE_OBJ_INFO, INCLUDE_CLASS_LIST and 
                 INCLUDE_TASK_LIST components.
11z,06apr07,v_r  Enforced dependency between loader hooks and TLS loader
                 support (req CQ:WIND00045277)
11y,04apr07,mmi  add spinLock API and Components
11x,16mar07,gls  added CFG_PARAMS to INCLUDE_SEM_READ_WRITE
11w,09mar07,pad  Removed a trailing backslash in INCLUDE_ANSI_STDLIB's
                 required component list.
11v,07mar07,gls  added INCLUDE_SEM_READ_WRITE
11u,07mar07,jmg  Added component INCLUDE_ATOMIC_OPERATORS
11t,14feb07,kk   moved INCLUDE_CPC to 10smp.cdf, added ENABLE_ALL_CPUS param
11s,23feb07,h_k  added INCLUDE_ISR_DEFER.
11r,12feb07,jmg  Added poolLibInit to INCLUDE_POOL, removed INCLUDE_WHEN from
                 INCLUDE_POOL_SHOW, fixed typo in INCLUDE_IO_SYSTEM.
11q,30jan07,jmg  Added INCLUDE_TASK_ROTATE (Req WIND00071010). 
11p,30jan07,jmg  Added VX_GLOBAL_NO_STACK_FILL (Req WIND00083667)
11o,24jan07,jmg  In Requiers list of INCLUDE_ANSI_STDIO, INCLUDE_ENV_VARS
                 and INCLUDE_POSIX_DIRLIB changed INCLUDE_MEM_MGR_FULL to
                 INCLUDE_MEM_MGR_BASIC and removed INCLUDE_MEM_MGR_FULL from
                 INCLUDE_ANSI_STDLIB (Req WIND00083666).
11n,23jan07,zhr  Fixed the defect WIND00035897
11m,08jan07,bwa  added usrDshmInit().
11l,08dec06,jpb  Removed layer 1 capability for INCLUDE_GNU_INTRINSICS and
                 INCLUDE_DIAB_INTRINSICS (Defect #82229).  Removed layer 1 
                 capability for INCLUDE_BOOT_LINE_INIT (Defect #114234).
11k,08dec06,jpb  Fixed warning being issued that parameter has bad value.
		 Type needs to be 'BOOL' and not 'bool' (Defect #37172).
11j,02dec06,jpb  Corrected typo error.
11i,07nov06,dbt  Added INCLUDE_TLS component.
11h,25oct06,jmg  Created INCLUDE_POSIX_TIMER_SHOW as part of defect WIND00039293
11g,28sep06,jmg  Changed the type define of objLibInit to (void *)
11f,22sep06,slk  add INCLUDE_VXMEMPROBE_INIT Component
11i,28sep06,jmg  Changed the type define of objLibInit to (void *)
11h,05sep06,tcr  change init order of POSIX_TRACE and POSIX_TRACE_TIMESTAMP
11g,06sep06,jln  Add SELECT_PX_SCHED_POLICY/INCLUDE_POSIX_TRACE to FOLDER_POSIX
11f,24aug06,gls  changed value of MAX_ISR_JOBS (Defect #00059580)
                 added PROTOTYPE for sigeventLibInit() (Defect #00062775)
11e,23aug06,pcm  added INCLUDE_UTF
11d,22aug06,pcm  added INCLUDE_DOSFS_SHOW
11c,25jul06,dcc  Added SFS components.
11b,16aug06,mze  added REQUIRES INCLUDE_WINDVIEW to INCLUDE_WVNETD component
11a,14aug06,tcr  INCLUDE_POSIX_TRACE_TIMESTAMP requires INCLUDE_SYS_TIMESTAMP
09y,10jul06,mmi  add INCLUDE_CPC and update usrRoot initGroup
10z,02aug06,gga  added TIPC WV instrumentation component
10y,01aug06,agf  change init group of INCLUDE_BOOT_LINE_INIT (WIND00056565)
10x,18jul06,md   Add SYNOPSIS for rawfs (WIND00044787)
10w,20jun06,pee  typos.
10v,10jul06,gls  added INIT_RTN sigeventLibInit to INCLUDE_SIGEVENT component
                 added INCLUDE_SIGEVENT to FOLDER_POSIX
10u,29jun06,jln  modified for SCHED_SPORADIC support
10t,15jun06,tcr  INCLUDE_SYS_TIMESTAMP should not require INCLUDE_WINDVIEW
                 Add INCLUDE_POSIX_TRACE
10s,09jun06,pcs  Updated to add obj/class lib support in MKL.
10r,16jun06,zl   added INCLUDE_MEM_MGR_INFO, MEM_PART_DEFAULT_OPTIONS.
10q,10may06,tfo  removed FOLDER_BUNDLES
10p,25apr06,mcm  Fix for SPR#117934 - adding strcasecmp (and strncasecmp)
10o,03apr06,pcm  fixed typo in INCLUDE_FORMATTED_OUT_BASIC (SPR 114558)
10n,31mar06,kk   added REQUIRES to kernel components to ensure clean removal
                 (SPR# 115349)
10m,01Mar06,pee  Fixed multiple declarations of wvLib.o
10l,08feb06,dlk  Replace INCLUDE_NET_STACK_START with its opposite,
		 INCLUDE_NET_INIT_SKIP, for backwards compat. (SPR 117553)
10k,23feb06,jlk  Added INCLUDE_HRFS_ACCESS_TIMESTAMP to FOLDER_IO_SYSTEM folder
10j,10feb06,jlk  Removed INCLUDE_POSIX_GENERAL. Added IOS_POSIX_PSE52_MODE 
                 parameter
10i,09feb06,kk   replace INCLUDE_VX_NATIVE_SCHEDULER with
                 INCLUDE_VX_TRADITIONAL_SCHEDULER
10h,09feb06,gls  added parameter to sigInit()
10g,06feb06,agf  add BSP components to INCLUDE_KERNEL requires
10f,03feb06,jlk  Added INCLUDE_POSIX_GENERAL
10e,03feb06,pcm  add synopsis to INCLUDE_CDROMFS component
                 is not dependent on stack anymore 
10d,19jan06,xli  add the INCLUDE_SOCKLIB to usrRoot init group since
		 INCLUDE_SOCKLIB
10c,18jan06,pad  Changed synopsis of the INCLUDE_POSIX_SCHED and
                 INCLUDE_POSIX_PTHREADS components.
            kk   Improved name string for INCLUDE_WATCHDOG_CREATE_DLETE
	         component (SPR #115952).
10b,17jan06,bpn  Moved symbol table components to 01loader.cdf.
10a,12jan06,act  add component INCLUDE_POSIX_ADVISORY_FILE_LOCKING
09z,30dec05,yvp  Removed INIT_RTN for semaphore components. 
                 INCLUDE_POSIX_SEM requires INCLUDE_SEM_COUNTING.
09y,21dec05,rfr  Added INCLUDE_VRFS
09x,19dec05,pcm  added parameter to INCLUDE_CDROMFS to control default number
                 of sectors to read in a single access
09w,09oct05,dlk  Support INCLUDE_NET_STACK_START.
		 Ensure that INCLUDE_TASK_VARS initialization routine 
                 is called if the component is included (SPR #112537).
09v,17nov05,yvp  SPR 113975: INCLUDE_SIGNALS requires INCLUDE_ANSI_LONGJMP
                 SPR 114233: TASK_UTIL had an incorrect SOURCE path
                 SPR 114234: BOOT_LINE_INIT no longer a an MKL component
                 SPR 114246: ANSI_TIME had incorrect LAYER value (is now 3)
                 SPR 114254: added INCLUDE_ANSI_STRLCPY
                 SPR 114259: incorrect modules list in INCLUDE_NBIO_LOG
                 SPR 114910: GNU_INTRINSICS no longer an MKL component
                 SPR 115086: corrected dependencies for INCLUDE_SET
                 SPR 115088: corrected dependencies for ANSI_ASSERT
                 SPR 115090: corrected dependencies for ENV_VARS
                 SPR 115092: corrected dependencies for INCLUDE_NBIO_LOG
                 SPR 115094: corrected dependencies for INCLUDE_PIPES
                 SPR 115095: corrected dependencies for POSIX_DIRLIB
                 SPR 115202: corrected dependencies for INCLUDE_FORMATTED_*
09u,14oct05,kk   fixed typo in POSIX_DIRLIB component
09t,10oct05,yvp  SPR 112547: INCLUDE_TASK_VARS inits in usrKernelExtraInit
                 SPR 112942: ANSI_STDLIB_NUMBERS doesnt use floating point
                 SPR 113423: new components for Sem/MsgQ event support.
                             corrected dependencies for ANSI_ABORT, LOGGING etc
                 SPR 113654: Made POSIX_PTHREAD_SCHEDULER visible in the IDE
                 SPR 113775: LINK_SYMS missing fro several ANSI components
                 SPR 113809: Added INCLUDE_SIMPLE_BANNER.
09s,07oct05,pcm	 changed INCLUDE_XBD_PARTLIB to INCLUDE_XBD_PART_LIB. SPR 113236
09r,07oct05,h_k  created INCLUDE_FP_TYPE. (SPR #113210)
                 added IO_SYSTEM as a required comp for FORMATTED_IO.
                 added MSG_Q as a required comp for VXEVENTS.
09q,28sep05,yvp  Removed ISR objects from the default build (SPR 112909).
09p,28sep05,kk   fixed dependency on FORMATTED_OUT_BASIC
09o,26sep05,zl   removed folder definition for INCLUDE_MEM_ALLOT.
09n,14sep05,jmt  Add source info for INCLUDE_BOOT_LINE_INIT
09q,15sep05,pcm  added many components to the FOLDER_IO_SYSTEM folder
09p,13sep05,pcm  added INCLUDE_RAM_DISK to usrIosExtraInit()
09o,13sep05,pcm  added INCLUDE_TFFS_MOUNT (SPR# 112219)
09n,13sep05,yvp  Updated ANSI_STDIO, ANSI_TIME, ANSI_STRERROR, POSIX_CLOCKS,
		 TASK_RESTART etc. Added ANSI_LONGJMP, SIGNAL_ARCH etc
09m,14sep05,h_k  moved modules for ANSI_MATH to arch dependent CDF.
                 (SPR #112320)
09l,13sep05,kk   remove OBJ_OWNERSHIP from default (SPR# 110865)
09k,08sep05,dcc  removed INCLUDE_TAPEFS component (SPR# 111788).
09j,08sep05,yvp  Fixed ANSI_STDLIB CDF definition (SPR 112003).
                 Added INIT_RTN for TASK_VARS (SPR 109661).
		 Corrected typo in CDF for INCLUDE_VXEVENTS.
09i,07sep05,h_k  cleaned up BLIB_ARCH dependency. (SPR #111769)
09h,01sep05,yvp  Scalability modifications:
                 - split many INCLUDE_ANSI_xxx routines into seperate components
                 - updated MODULES, REQUIRES & SOURCE for layers 1-3
09g,31aug05,jmt  Move windPwrLib.o from INCLUDE_KERNEL to INCLUDE_KERNEL_ARCH
09f,31aug05,gls  added INCLUDE_TASK_UTIL
09e,30aug05,zl   created selector for memory allocator.
09d,26aug05,pee  Corrected usage of invisible folder
08s,18aug05,act  init XBD_TRANS before DOSFS, not after
08r,18aug05,act  add INCLUDE_XBD_TRANS to usriosExtraInit group
08q,09aug05,dcc  added dosFsCache initialization.
08p,04aug05,aeg  added cdromFsFsmLib.o to INCLUDE_CDROMFS
08o,28jul05,pcm  removed Component INCLUDE_DOSFS as it exists elsewhere
08n,27jul05,jmp  added INCLUDE_CORE_DUMP to usrRoot InitGroup.
08m,15jul05,jlk  fixed a bug where INCLUDE_XBD_BLK_DEV was dropped from 
                 usrIosExtraInit
09c,18aug05,yvp  Added INCLUDE_REBOOT_HOOKS to FOLDER_KERNEL.
09b,16aug05,gls  added objOwnerLib.o to INCLUDE_OBJ_OWNERSHIP component
09a,16aug05,yvp  Added INIT_RTN for Q_XXX_CREATE_DELETE components.
                 Added INCLUDE_FORMATTED_OUT_BASIC.
08z,12aug05,zl   added INCLUDE_MMU_GLOBAL_MAP to usrRoot group; created
		 INCLUDE_MEM_ALLOT component.
08y,10aug05,yvp  Added INCLUDE_CLASS_LIB as an explicit dependency of
                 INCLUDE_OBJ_LIB. Also added it to defaults of FOLDER_KERNEL.
                 Added FOLDER_NOT_VISIBLE for the q*Lib components.
08x,09aug05,pee  Added LAYER to EDR_STUB
08w,06aug05,jln  Added SELECT_KERNEL_SCHEDULER 
08v,04aug05,yvp  Corrected typosin component names.
08u,03aug05,yvp  Added newly created components (sem, msgQ & wd create/delete).
08t,03aug05,pee  Added LAYER information
08s,18jul05,pee  fixed some dependencies.
08r,16jul05,jln  Added INCLUDE_POSIX_PTHREAD_SCHED component
08q,15jul05,jlk  fixed a bug where INCLUDE_XBD_BLK_DEV was dropped from 
                 usrIosExtraInit
08p,09jul05,yvp  Added INCLUDE_SEM_LIB to FOLDER_HIDDEN, excLibP.h to kernel
                 headers, PROTOTYPE for INCLUDE_TASK_CREATE_DELETE. Added more
                 description to synopsis for INCLUDE_ENV_VARS. 
                 Added INCLUDE_HOOKS and INCLUDE_HOOK_SHOW.
08o,08jul05pee   Added SOURCE for a bunch of component
08n,07jul05,yvp  Added INCLUDE_REBOOT_HOOKS to the defaults for FOLDER_KERNEL.
08m,04jul05,pcm  fixed up DEFAULTS list in FOLDER_KERNEL
08l,23jun05,jlk  added INCLUDE_FS_EVENT_UTIL to usrIosExtraInit.
08k,22jun05,yvp  Added components INCLUDE_TASK_CREATE_DELETE and 
                 INCLUDE_TASK_RESTART.
08j,21jun05,pee  removed obsolete INCLUDE_CODETEST (SPR #110121)
08i,15jun05,yvp  Mutex semaphore initalization moved into the usrInit group.
08h,09jun05,yvp  New components INCLUDE_ISR_OBJECTS (no longer default),
                 INCLUDE_SEM_LIB, INCLUDE_REBOOT_HOOKS.
                 INCLUDE_EXC_TASK and INCLUDE_JOB_TASK now decoupled.
08g,03jun05,pcm  added INCLUDE_ERF, INCLUDE_DEVICE_MANAGER, INCLUDE_XBD, and 
                 INCLUDE_FS_MONITOR to usrIosExtraInit
08f,03jun05,pcm  added INCLUDE_DEVICE_MANAGER
08e,02jun05,pcm  added INCLUDE_ERF
08d,01jun05,pcm  added INCLUDE_HRFS_READONLY
08c,26may05,pcm  added FS_MONITOR as RAWFS requirement; removed XBD_BLK_DEV
                 from usrIosExtraInit()
08b,25apr05,yvp  added LINK_SYMS attribute to INCLUDE_VXEVENTS
08a,20apr05,gls  added INCLUDE_OBJ_OWNERSHIP (SPR #106150)
07z,04apr05,svk  Add INCLUDE_STRING_SEARCH component
07y,09mar05,pcm  added INCLUDE_HRFS_FORMAT
07x,08mar05,pcm  added INCLUDE_HRFS and INCLUDE_XBD_BLK_DEV to usrIosExtraInit
07w,22mar05,mcm  Fix for SPR#105016 - removing obselete C++ components.
07v,07mar04,pcm  added INCLUDED_TAPEFS
07u,26feb05,pcs  Changes to reflect different stack overflow and underflow
                 sizes.
07t,07feb05,zl   enable kernel text protection early.
07s,04feb05,pch  SPR 102772: add INCLUDE_PROTECT_INTERRUPT_STACK
07r,18jan05,yvp  Removed INCLUDE_VXEVENTS from the default configuration.
07q,13jan05,tcr  Fix SPR 101137: WindView upload task priority and stacksize
                 should be parameters
07p,15dec04,md   Moved ED&R stub init to be after clock is initialised
07o,11nov04,job  Added usrScInit init group for system call initialization
07n,11nov04,aeg  moved INCLUDE_KERNEL to FOLDER_KERNEL (SPR #104076).
07m,26oct04,fr   added INCLUDE_OBJ_OPEN, INCLUDE_SEM_OPEN, INCLUDE_MSG_Q_OPEN,
		 INCLUDE_TASK_OPEN, and INCLUDE_TIMER_OPEN (SPR 101491)
07l,13oct04,dlk  Added NBIO_LOG.
07k,12oct04,kk   renamed kernelBaseLib.h to private/kernelBaseLibP.h
07j,11oct04,jn   Change name of INCLUDE_ABS_SYMBOLS to INCLUDE_OFFSET_SYMBOLS
07i,08oct04,yvp  Added INCLUDE_SET to FOLDER_UTILITIES (SPR 102389).
07h,01oct04,fr   added component INCLUDE_HANDLE_SHOW (SPR 99750)
07g,30sep04,job  Renamed INCLUDE_RTP_KERNEL to INCLUDE_BASE_KERNEL
07f,30sep04,tam  replaced INCLUDE_EDR_KH by INCLUDE_KERNEL_HARDENING
07e,29sep04,zl   removed MMU_MPU component.
07d,29sep04,jn   Fix problem with symbol table config param 
07c,23sep04,jn   Make INCLUDE_NET_SYM_TBL require INCLUDE_NET_INIT
07b,20sep04,bwa  added usrDsiInit to usrRoot init order.
07a,13sep04,kk   renamed rtpBaseLib components
06z,31aug04,fr	 added component INCLUDE_SEM_INFO (SPR 100038)
06y,06sep04,jmp  Added missing MAX_ISR_JOBS parameter declaration.
		 Fixed typo in usrIosExtraInit InitGroup.
06x,06sep04,dbt  Added JOB_TASK_STACK_SIZE parameter declaration in
                 components.
06w,03sep04,ans  added INCLUDE_JOB_TASK and updated excInit () parameter list
06v,26aug04,md   Added EDR_RECORD_SIZE
06t,08sep04,jyo  Fixing SPR 100119: Removing references to RT11.
06u,23aug04,job  Prototype better POSIX system call scalability.
                 Prototype of no symbol table when INCLUDE_RTP
06t,28jul04,md   removed INCLUDE_EDR_SYMBOLIC, make INCLUDE_EDR_STUB hidden
06s,06jul04,dtr  Adding in AIM configuration and optimization.
06r,30jun04,job  Added prototypes to INCLUDE_POSIX_CLOCKS and INCLUDE_SC_POSIX
		 INCLUDE_SC_POSIX is include when INCLUDE_POSIX_CLOCKS
		 as this component fundamentally relies on the init routine
		 being called.
06q,28jun04,kk   add SIGEVENT to FOLDER_NOT_VISIBLE
06p,24jun04,zl   added INCLUDE_POOL to utilities folder
06o,24jun04,elg  Fix typo (SPR #98363).
06n,22jun04,pad  Made INCLUDE_SC_POSIX included when both INCLUDE_RTP and
                 INCLUDE_POSIX_TIMERS are included (SPR 98349).
06m,21jun04,md   re-arrange ED&R components
06l,11jun04,dcc  added INIT_RTN to INCLUDE_POSIX_CLOCKS (SPR 97847)
06k,09jun04,zl   renamed INCLUDE_MMU_FULL_SHOW to INCLUDE_VM_SHOW.
06j,06may04,lei  added component INCLUDE_MSG_Q_INFO (SPR 81966).
06i,07jun04,elg  Remove call to usrWdbInit init group.
06h,02jun04,bpn  Moved loader component to 01loader.cdf.
06g,12may04,dbt  Added support for WDB Modules & Symbols Synchronization.
06f,29apr04,elg  Include ED&R policies when WDB exception notification is
                 included.
06e,02may04,jkf  fixed SPR#90062
06d,27apr04,tcr  change references to WindView to 'System Viewer'
06c,29apr04,h_k  removed DSP component.
06b,02may04,jkf  fixed SPR#90062
06a,27apr04,tcr  change references to WindView to 'System Viewer'
05z,21apr04,dat  moving selectInit after iosInit. Moved HW_FP, SW_FP, DSP, and
                 SET out of usrIosCoreInit, and into usrRoot. (96540)
05y,01apr04,dat  adding MEMDRV, PTYDRV, TYLIB (93390, 6294)
05x,18mar04,dat  adding components for poolLib, poolShow, and setLib
05w,21apr04,cjj  Added WIND_JOBS_MAX parameter.  Cleaned up history version
		 numbers back to 04d.
05v,06mar04,dat  adding absSymbols to linkSyms, remove underscore (95052)
05u,25feb04,yvp  Added INCLUDE_RTP_APPL to InitGroup usrRoot.
05t,12feb04,dat  chg init order for POSIX_AIO, to IosExtraInit
05s,11feb04,bpn  Moved the shell components to shell CDF files.
05r,28jan04,elg  Move WDB components to 00wdb.cdf.
05q,23jan04,fle  Made INCLUDE_SHOW_ROUTINES belong to the FOLDER_SHOW_ROUTINES
		 folder
05p,16jan04,syy  replaced WVNET component with WVNETD
05o,15dec03,rp   corrected password
05n,08dec03,m_s  added SC_POSIX to FOLDER_POSIX
05m,06dec03,dat  making select support component invisible to users
05l,02dec03,bwa  removed all MsgC/S references
05k,10nov03,kk   added INCLUDE_SHOW_ROUTINES component, clean up inclusion
                 of show routines and model after BSP build.
05j,30oct03,dat  new posix dirLib component, sprs 22056/74198 
05i,22oct03,ans  added INIT_RTN to INCLUDE_POSIX_TIMERS
05h,22oct03,dat  moved SC_xxx comps to 01rtp.cdf. Cleanup of RTP comp
		 attributes Created FOLDER_NOT_VISIBLE as a placeholder for
		 components that are not visible to the user. New RTP_KERNEL,
		 CLASS_LIB components for startup requirements.
05g,22oct03,ans  added INIT_RTN to INCLUDE_POSIX_TIMERS
05f,16oct03,ans  added INCLUDE_SC_POSIX , INCLUDE_SIGEVENT; INCLUDE_POSIX_TIMERS
                 intialized in usrKernelExtraInit.
05e,14oct03,dbs  force INCLUDE_TEXT_PROTECT when EDR KH is included
05d,10oct03,bpn  Moved kernel shell components to 00shell.cdf.
05c,08oct03,tam  moved usrRtpInit before INCLUDE_MMU_BASIC; moved 
		 INCLUDE_OBJ_LIB to usrKernelInit()
05b,06oct03,tcr  change trigger init func; limit number of WV logs
05a,03oct03,dbs  add ED&R KH support
04z,29sep03,pad  Reworked the inclusion logic for INCLUDE_SC_MMAN and
04y,26sep03,dbs  add ED&R policies
04x,25sep03,rp   merging from bas6_itn2_networking to base6_itn2
04w,25sep03,rp   merging from bas6_itn2_networking to base6_itn2
		 INCLUDE_SC_KERNEL. They no longer appear as individually
		 selectable components and are now automatically dragged in
		 when INCLUDE_RTP is selected.
04v,22sep03,bpn  Updated shell components. Added command folder, 
                 INCLUDE_RTP_SHELL_CMD and INCLUDE_DISK_UTIL_SHELL_CMD
		 components. Set default shell line length to 256 characters.
		 Added SHELL_CONFIG parameter. Set vi as the default editing
		 mode.
04u,19sep03,pad  Added Parameters TASK_USER_EXC_STACK_SIZE,
		 TASK_STACK_OVERFLOW_SIZE and TASK_STACK_UNDERFLOW_SIZE. Moved
		 usrRtpInit earlier in the initialization sequence.
04t,12sep03,dbs  move ED&R init as early as possible
04s,10sep03,dbs  add ED&R config params and edrStub component
04r,05sep03,tcr  change initialization of WindView upload
04q,03sep03,elg	 added WDB_END_DEVICE_NAME and WDB_END_DEVICE_UNIT parameters.
04p,21aug03,zl   added INCLUDE_SC_MMAN component.
04o,21aug03,dcc  added INCLUDE_SC_KERNEL component.
04n,19aug03,kk   added INCLUDE_RTP_SHOW to SHOW folder.
04m,27aug03,elg  Add INCLUDE_WDB_RTP component.
04l,18aug03,dbs  rework ED&R show routines
04k,08aug03,dbs  add enhanced ED&R show routines
04j,04aug03,dbs  ensure pmLib has no globals
04i,31jul03,dbs  add separate edrShow component
04h,22jul03,jmp  added INCLUDE_PASSFS_SYM_TBL to usrToolsInit().
04g,22jul03,bpn  Added INCLUDE_SHELL_INTERP_CMD component.
04f,16jul03,dbs  add edrLib initialization
04e,14jul03,dbs  rename phpLib to pmLib
04d,09jul03,dbs  add ED&R components
04c,09jul03,bpn  Updated shellInterpRegister() API.
04b,08jul03,bpn  Updated shell components.
04a,02jul03,md   Added INCLUDE_ROMFS components.
03z,27jun03,bwa  added filename comment; corrected typos;
		 updated following code review
03y,10apr03,elg  Add WDB_COMM_PORT to configure WDB port.
03x,31mar03.bpn  Added a new parameter SHELL_COMPATIBLE to the shell, for 
                 compatibility with vxWorks 5.5.
03w,20mar03.bpn  Changed ledRegister() to ledModeRegister().
03v,17mar03,lcs  Modified ledLib initialization code.
03u,05mar03,bpn  Modified the C interpreter registering name.
03t,18feb03,bpn  Modified the target shell component. Added new components for
                 the interpreter and for the line editing mode. Added new
                 parameters for the shell.
03s,31jan03,tor  Mount filesystems at boot time (spr #69254)
03r,08nov02,dbt  Improved dependencies between WDB components (SPR #79259)
03q,22jun02,gls  reversed order of INCLUDE_MEM_MGR_FULL and 
                 INCLUDE_MEM_MGR_BASIC in usrRoot() (SPR #78999)
03p,29may02,dat  Fix header file for SELECT, 78096
03o,15may02,j_s  make INCLUDE_SPY require INCLUDE_AUX_CLK
03n,08may02,sn   SPR 76322 - pull in demangler if C++ and the symbol table are included
03m,27mar02,jkf  SPR#74716, moved INCLUDE_DISK_UTIL from DOSFS into IO folder
03l,19mar02,sn   SPR 71699 - remove cplusLoad.o from INCLUDE_CPLUS_DEMANGLER
		 (cplusLoad.o and cplusUsr.o are part of the targe loader)
03k,19mar02,j_s  Delist bufLib.o as a module of INCLUDE_WDB, add INCLUDE_BUF_MGR
		 to REQUIRES of INCLUDE_WDB (SPR #73477)
03j,13mar02,rbl  fixing bug where EXC_SHOW component appeared in two folders
03i,12mar02,jkf  SPR#74178, adding CDROMFS to iosextra init group.
03h,11mar02,sn   Wind Foundation classes no longer supported (SPR #73967)
03g,06mar02,sn   Removed INCLUDE_GNU_64BIT (obsoleted by INCLUDE_GNU_INTRINSICS)
03f,04mar02,j_s  fix the typo 'SELECT_SYMTBL_INIT', should be
                 'SELECT_SYM_TBL_INIT'
03e,05feb02,sn   Added SELECT_COMPILER_INTRINSICS
03d,29jan02,g_h  Add INCLUDE_WDB_COMM_VTMD to SELECT_WDB_COMM_TYPE
03c,28jan02,sn   added (); to cplusCtorsLink; added INCLUDE_CTORS_DTORS to InitGroup
03b,25jan02,zl   removed cplusGlob.o from INCLUDE_CTORS_DTORS
03a,21jan02,sn   added INCLUDE_CTORS_DTORS
02z,09dec01,jac  added INCLUDE_CDROMFS component
02y,30nov01,sbs  Synopsis to NUM_FILES about FD_SETSIZE (SPR 9377)
02x,20nov01,bwa  Changed INCLUDE_EVENTS to INCLUDE_VXEVENTS
02w,12nov01,mas  made new components for SM_OBJ, SM_NET, and SM_COMMON
02v,05nov01,gls  added INCLUDE_POSIX_PTHREADS
02u,06nov01,sn   moved toolchain specific C++ stuff to toolchain subdirectories
02t,31oct01,tcr  add Windview network instrumentation component
02s,22oct01,sn   Added catch-all compiler intrinsics components
02r,11oct01,bwa  Corrected MODULE field for VxWorks events.
02q,11oct01,mas  cfg params: added new ones to, and deleted duplicates from,
		 INCLUDE_SM_OBJ (SPR 4547, 62128)
02p,10oct01,fmk  add configlette to INCLUDE_CPLUS and change name of init
		 routine to match
02o,09oct01,bwa  Added VxWorks events module
02n,26sep01,jws  changed default for SM_OBJ_MAX_TRIES to 5000 (SPR 68418)
02m,20sep01,aeg  added the INCLUDE_SELECT_SUPPORT component.
02l,13sep01,pcm  added INCLUDE_EXC_SHOW (SPR 7333), INCLUDE_LSTLIB (SPR 20698)
02k,09may01,ros  fix codetest
02h,17may01,rec  SPR 63492 - cache not enabled from project
02j,10jan01,sn   Specify configlettes for toolchain related/C++ components
		 Changed C++ stdlib defaults to exclude complex and include STL
		 extract core iostreams modules from full iostreams
02i,05dec00,sn   Fix definition of INCLUDE_CPLUS_STL
02h,11oct00,sn   Use modules instead of linksyms for the C++ comps
02k,01dec00,cmf  changed name of INCLUDE_PCI_SHOW to INCLUDE_PCI_CFGSHOW to
                 match T3 naming
02j,24aug00,pai  corrected INCLUDE_PCI_SHOW component fix (SPR #27759).
02i,19may00,pai  added INCLUDE_PCI_SHOW component (SPR #27759).
02h,11oct99,pai  changed software and hardware floating point init order (SPR
                 28624).
02l,16nov00,zl   added DSP related components.
02k,18oct00,rsh  remove obsolete ctOperateroNew.o reference
02j,27jul00,rsh  fix up MODULES define for CodeTEST component to reflect that
                 fact that we are now archiving in the individual .o's instead
                 of a single relatively linked .o (.a)
02i,14jun00,mnd  Changed INCLUDE_CODETEST MODULES from ctLib to libctSwic.a.
02h,20apr00,mnd  Changed CodeTEST include component to ctLib. Removed older
                 modules cttXdr.o, cttOperatorNew.o, cttTargetLib.o, no longer
                 needed. Updating the file for CodeTEST 2.11 integration with
                 Tornado 2.0.
02g,01apr99,cjs  Correct symbol table initialization logic so that removing
                 the symtab init component removes the selection as well
02f,24mar99,cjs  broke apart symbol table initialization so that symbol
		 table can be included w/o initialization 
02e,17mar99,pr   added ataShow routines (SPR #25777)
02d,10mar99,pr   added trgShow and rBuffShow components (SPR #25417)
02c,26feb99,cjs  Fix defaults for SELECT_WDB_COMM_TYPE, SELECT_WDB_MODE 
02b,23feb99,cth  reordering init order in usrIosExtraInit (SPR 24297)
		 added INCLUDE_MEM_SHOW component (SPR 24371)
		 removed extra PROTOTYPE from usrInit
02a,19jan99,pr   deleted INCLUDE_WHEN entry in INCLUDE_SEQ_TIMESTAMP (SPR 23630)
01z,15jan99,dbt  added some help references for WDB components.
01y,24nov98,pr   added dependencies between INCLUDE_WVUPLOAD_TSFSSOCK and 
		 INCLUDE_WDB_TSFS
01x,19nov98,pr   added INCLUDE_WVUPLOAD_TSFSSOCK to windview upload defaults
01w,04nov98,ms   changed some LINK_SYMS to LINK_DATASYMS so SIMHP works.
01v,21oct98,ms   added INCLUDE_CODETEST component
01u,20oct98,ms   added INCLUDE_WHEN for SEQ_TIMESTAMP and RBUFF components
01t,11oct98,ms   cplusUsr.o no longer part of INCLUDE_CPLUS_MANGLER
01s,30sep98,pr   added TRIGGERING component descriptor
01r,30sep98,ms   folded in pr's WINDVIEW component descriptors
01q,15sep98,ms   fixed errors uncovered by new cxrDocValidate
01p,14sep98,ms   removed INCLUDE_EXC_SHOW (it's now part of EXC_HANDLING)
01o,08sep98,sn   removed TRAP_DEBUG parameter
01n,02sep98,sn   Fixed up C++ dependencies
01m,19aug98,ms   Fixed INCLUDE_CPLUS_STRING - append "_" to _cstringi_o
01l,19aug98,sut  Moved BSP components to 00bsp.cdf
01k,19aug98,jmp  fixed typo in INCLUDE_WDB_VIO_LIB SYNOPSIS.
01j,23aug98,ms   INCLUDE_SHELL now REQUIRES SELECT_SYMTBL_INIT.
01i,23aug98,sut  changed some BSP components
01h,14aug98,ms   added proper DEFAULTS to most folders
01g,12aug98,ms   fixed initialization order of WDB agent
01f,07aug98,cjs  fixed a spelling mistake in a component synopsis 
01e,03jun98,ms   lots of cleanup.
01d,03jun98,sn   updated C++ components
01c,03jun98,dbt  completed WDB components
01b,21may98,ms	 SYM_TBL_INIT and SYM_TBL are now only under one folder
01a,09apr98,ms   written
*/

/*
DESCRIPTION
This file contains descriptions for some VxWorks components.
The configuration tool searches for .cdf files in four
places; here, arch/<arch>, config/<bsp>, and the project directory.
This file describes "generic" components which are available
to all CPU architectures and BSPs.
If a new component is released, a new .cdf file should be deposited
in the appropriate directory, rather than modifying existing files.

INTERNAL
add in windview
"Precious" Components (like the kernel!) that can't be deleted.
Folder-precious components (like wdbCore), that can't be deleted
without deleting the whole folder.
*/

List UNAVAILABLE_LIST {
	NAME List of components that are not available
}

List ERROR_LIST {
	NAME List of components that have errors
}

Component INCLUDE_SYS_START {
	NAME		system startup code
	SYNOPSIS	clear BSS and set up the vector table base address.
	INIT_RTN	sysStart (startType);
	CONFIGLETTES	usrStartup.c
	CFG_PARAMS	CLEAR_BSS
	HDR_FILES	intLib.h string.h
}

Component INCLUDE_BOOT_HW_INIT {
	NAME		system bootup hardware init code
	SYNOPSIS	call usrBootHwInit() routine
	INIT_RTN	usrBootHwInit ();
	CONFIGLETTES	usrBootHwInit.c
	HDR_FILES	vxWorks.h
	INCLUDE_WHEN    INCLUDE_SYS_START
}

Component INCLUDE_VXCPULIB {
	NAME		CPU Identification Initialization
	SYNOPSIS 	Enable the initialization of CPU identification routines
	PROTOTYPE       extern void vxCpuLibInit (void);
	INIT_RTN	vxCpuLibInit ();
	_CHILDREN	FOLDER_KERNEL
}

Component INCLUDE_SYSHW_INIT {
	NAME		BSP hardware initialization
	SYNOPSIS	call the BSPs sysHwInit routine during system startup
	INIT_RTN	sysHwInit ();
	HDR_FILES	sysLib.h
}

/* Components to support post-kernel hardware init */

#ifdef _WRS_CONFIG_DELAYED_HW_INIT
Component INCLUDE_DEFERRED_INTERRUPT_ENABLE {
	NAME		Deferred Interrupt Enable
	SYNOPSIS	Enable interrupts at appropriate point in root task
	HDR_FILES	private/intLibP.h
	INIT_RTN	intStartupUnlock ();
	_CHILDREN	FOLDER_NOT_VISIBLE
	INCLUDE_WHEN	INCLUDE_KERNEL
}

Component INCLUDE_SYSHW_INIT_1 {
	NAME		Post-kernel BSP hardware initialization
	SYNOPSIS	call the BSPs sysHwInit1 routine during system startup
	_CHILDREN	FOLDER_NOT_VISIBLE
	HDR_FILES	sysLib.h
	INIT_RTN	sysHwInit1 ();
	INCLUDE_WHEN	INCLUDE_KERNEL
}
#endif

Component INCLUDE_SYSHW_INIT2  {
	NAME		second Phase of BSP hardware initialization
	SYNOPSIS	call the usrSysHwInit2 routine during system startup
	CONFIGLETTES	usrSysHwInit2.c
	INIT_RTN	usrSysHwInit2();
	_CHILDREN	FOLDER_NOT_VISIBLE
}

Component INCLUDE_OFFSET_SYMBOLS {
	NAME		absolute symbol offsets for host tools
	SYNOPSIS	symbol offsets needed by host tools
	SOURCE		$(TGT_DIR)/src/offset 
	MODULES		offsets.o 
	HDR_FILES	private/offsetsP.h
	LINK_SYMS	absSymbols_Common
/* Can't use _WRS_ABS_SYMBOL, because linkSyms doesn't include headers */
}

Component INCLUDE_EDR_STUB {
	NAME		ED&R Stub
	SYNOPSIS	protected error log stub initialization
	INIT_RTN	edrStubInit ();
	CONFIGLETTES	edrStub.c
}

Component INCLUDE_EDR_POLICY_HOOKS {
	NAME		ED&R Policy Hooks
	SYNOPSIS	policy hook routine support for ED&R
	REQUIRES	INCLUDE_EDR_STUB
}

/* Invisible component folder. exists only to suppress warnings */

Folder	FOLDER_NOT_VISIBLE {
	NAME		invisible, unremovable components
	CHILDREN	INCLUDE_SYS_START \
			INCLUDE_BOOT_HW_INIT \
			INCLUDE_SYSHW_INIT \
			INCLUDE_SYSHW_INIT2 \
			INCLUDE_BASE_KERNEL \
			INCLUDE_SELECT_SUPPORT	\
			INCLUDE_OFFSET_SYMBOLS \
			INCLUDE_CLASS_LIB \
			INCLUDE_CLASS_LIST \
			INCLUDE_EDR_STUB \
			INCLUDE_PROCFS   \
			INCLUDE_SEM_LIB  \
                        INCLUDE_CBIO      \
                        INCLUDE_DISK_PART \
			INCLUDE_CHECKSUM \
			INCLUDE_TASK_SWAP_HOOKS \
			INCLUDE_TLS_LOADER_SUPPORT
	DEFAULTS	INCLUDE_SYS_START \
	                INCLUDE_BOOT_HW_INIT \
			INCLUDE_SYSHW_INIT \
			INCLUDE_SYSHW_INIT2 \
			INCLUDE_BASE_KERNEL \
			INCLUDE_OFFSET_SYMBOLS \
			INCLUDE_CLASS_LIB \
			INCLUDE_EDR_STUB  \
			INCLUDE_SEM_LIB   \
			INCLUDE_TASK_SWAP_HOOKS
}

Folder	FOLDER_ROOT {
	NAME		all components
	CHILDREN	FOLDER_APPLICATION	\
			FOLDER_TOOLS		\
			FOLDER_NETWORK		\
			FOLDER_CPLUS		\
			FOLDER_OS		\
			FOLDER_OBSOLETE		\
			FOLDER_HARDWARE \
                        FOLDER_SSI
	DEFAULTS	FOLDER_TOOLS			\
			FOLDER_NETWORK			\
			FOLDER_OS			\
			FOLDER_HARDWARE
}

Folder FOLDER_OS {
	NAME		operating system components
	CHILDREN	FOLDER_IO_SYSTEM	\
			FOLDER_KERNEL		\
			FOLDER_EDR		\
			FOLDER_ANSI		\
			FOLDER_POSIX		\
			FOLDER_UTILITIES	\
			FOLDER_PWR_MGMT
	DEFAULTS	FOLDER_IO_SYSTEM	\
			FOLDER_KERNEL		\
			FOLDER_EDR		\
			FOLDER_ANSI		\
			FOLDER_UTILITIES
}

Folder	FOLDER_OBSOLETE {
	NAME		obsolete components
	SYNOPSIS	will be removed next release
	CHILDREN	INCLUDE_TYCODRV_5_2
}

Component INCLUDE_TYCODRV_5_2 {
	NAME		5.2 serial drivers
	SYNOPSIS	replaced by sio drivers
	CONFIGLETTES	usrSerialOld.c
	INIT_RTN	usrSerialOldInit ();
	EXCLUDES	INCLUDE_SIO
	CFG_PARAMS	NUM_TTY			\
			CONSOLE_TTY		\
			CONSOLE_BAUD_RATE
	HDR_FILES	sysLib.h stdio.h ioLib.h
}

Folder	FOLDER_ANSI {
	NAME		ANSI C components (libc)
	SYNOPSIS	ANSI libraries
	CHILDREN	INCLUDE_ANSI_ASSERT	\
			INCLUDE_ANSI_CTYPE	\
			INCLUDE_ANSI_LOCALE	\
			INCLUDE_ANSI_MATH	\
			INCLUDE_ANSI_STDIO	\
			INCLUDE_ANSI_STDLIB	\
			INCLUDE_ANSI_STRING	\
			INCLUDE_ANSI_STRDUP     \
			INCLUDE_ANSI_STRERROR   \
			INCLUDE_ANSI_TIME	\
			INCLUDE_ANSI_STDIO_EXTRA
	DEFAULTS	INCLUDE_ANSI_ASSERT	\
			INCLUDE_ANSI_CTYPE	\
			INCLUDE_ANSI_MATH	\
			INCLUDE_ANSI_STDIO	\
			INCLUDE_ANSI_STDLIB	\
			INCLUDE_ANSI_STRING	\
			INCLUDE_ANSI_TIME	\
			INCLUDE_ANSI_STRDUP	\
			INCLUDE_ANSI_STRERROR
}

Component	INCLUDE_ANSI_STDIO_EXTRA {
	NAME		ANSI stdio extensions
	SYNOPSIS	WRS routines getw, putw, and setbuffer
	LINK_SYMS	getw putw setbuffer
}

Component INCLUDE_ANSI_ASSERT {
	NAME		ANSI assert
	SYNOPSIS	ANSI assert function
	LINK_SYMS	__assert
	HELP		ansiAssert
	SOURCE		$(TGT_DIR)/src/libc/assert 
	MODULES		assert.o
	REQUIRES	INCLUDE_ANSI_STDIO \
			INCLUDE_ANSI_ABS   \
			INCLUDE_ANSI_ABORT \
			INCLUDE_FORMATTED_IO
}

Component INCLUDE_ANSI_CTYPE {
	NAME		ANSI ctype
	SYNOPSIS	ANSI ctype functions
	SOURCE		$(TGT_DIR)/src/libc/ctype 
	MODULES		__ctype_tab.o isalnum.o isalpha.o iscntrl.o isdigit.o \
			isgraph.o islower.o isprint.o ispunct.o isspace.o \
			isupper.o isxdigit.o tolower.o toupper.o
	LINK_SYMS	__ctype isalnum isalpha iscntrl isdigit isgraph \
			islower isprint ispunct isspace isupper isxdigit \
			tolower toupper
	HELP		ansiCtype
}

Component INCLUDE_ANSI_LOCALE {
	NAME		ANSI locale
	SYNOPSIS	ANSI locale functions
	SOURCE		$(TGT_DIR)/src/libc/locale
	MODULES		localeconv.o setlocale.o
	LINK_SYMS	localeconv setlocale
	HELP		ansiLocale
	REQUIRES	INCLUDE_ANSI_STRCMP
}

Component INCLUDE_ANSI_LONGJMP {
	NAME		ANSI longjmp
	SYNOPSIS	ANSI longjmp
	SOURCE		$(TGT_DIR)/src/libc/setjmp
	MODULES		longjmp.o
	LINK_SYMS	longjmp
	REQUIRES	INCLUDE_SIGNAL_ARCH
}

Component INCLUDE_ANSI_MATH {
	NAME		ANSI math
	SYNOPSIS	ANSI mathematical functions
	LINK_SYMS	acos asin atan atan2 ceil cos cosh exp fabs floor      \
			fmod frexp ldexp log log10 modf pow sin sinh sqrt      \
			tan tanh
	HELP		ansiMath
}

Component INCLUDE_ANSI_STDIO {
	NAME		ANSI stdio
	SOURCE		$(TGT_DIR)/src/libc/stdio 
 	MODULES		clearerr.o fclose.o feof.o ferror.o fflush.o fgetc.o  \
			fgetpos.o fgets.o fopen.o fprintf.o fputc.o fputs.o   \
			fread.o freopen.o fscanf.o fseek.o fsetpos.o ftell.o  \
			fwrite.o getc.o getchar.o gets.o perror.o putc.o      \
			putchar.o puts.o rewind.o scanf.o setbuf.o setvbuf.o  \
			tmpfile.o tmpnam.o ungetc.o vfprintf.o fdopen.o       \
			fileno.o flags.o makebuf.o rget.o stdio.o fvwrite.o   \
			wbuf.o refill.o wsetup.o
	LINK_SYMS	clearerr fclose feof ferror fflush fgetc fgetpos      \
			fgets fopen fprintf fputc fputs fread freopen fscanf  \
			fseek fsetpos ftell fwrite getc getchar gets perror   \
			putc putchar puts rewind scanf setbuf setvbuf tmpfile \
			tmpnam ungetc vfprintf fdopen fileno 
	HELP		ansiStdio
	REQUIRES	INCLUDE_KERNEL      	\
			INCLUDE_IO_BASIC   	\
			INCLUDE_ANSI_STRING 	\
			INCLUDE_ANSI_STRLEN 	\
			INCLUDE_STDIO		\
			INCLUDE_FORMATTED_IO	\
			INCLUDE_ANSI_STRERROR	\
			INCLUDE_ANSI_LONGJMP    \
			INCLUDE_MEM_MGR_BASIC
}

Component INCLUDE_ANSI_STDLIB {
	NAME		ANSI stdlib
	SOURCE		$(TGT_DIR)/src/libc/stdlib
	MODULES		atexit.o multibyte.o system.o
	LINK_SYMS	atexit mblen system
	REQUIRES	INCLUDE_ANSI_ABS            \
			INCLUDE_ANSI_ABORT          \
			INCLUDE_ANSI_BSEARCH        \
			INCLUDE_ANSI_STRCPY         \
			INCLUDE_ANSI_ATOF           \
			INCLUDE_ANSI_STRTOD         \
			INCLUDE_ANSI_STDLIB_NUMBERS \
			INCLUDE_TASK_CREATE_DELETE  \
			INCLUDE_ENV_VARS            \
			INCLUDE_MEM_MGR_BASIC
	HELP		ansiStdlib
}

Component INCLUDE_ANSI_ABORT {
	NAME		ANSI abort
	SYNOPSIS	ANSI abort function
	LINK_SYMS	abort
	SOURCE		$(TGT_DIR)/src/libc/stdlib 
	MODULES		abort.o
	REQUIRES	INCLUDE_TASK_CREATE_DELETE  \
			INCLUDE_SIGNALS
}

Component INCLUDE_ANSI_BSEARCH {
	NAME		ANSI stdlib bsearch function
	SYNOPSIS	ANSI stdlib bsearch function
	LINK_SYMS	bsearch
	SOURCE		$(TGT_DIR)/src/libc/stdlib 
	MODULES		bsearch.o
}

Component INCLUDE_ANSI_STRTOD {
	NAME		ANSI strtod function
	SYNOPSIS	ANSI strtod function
	LINK_SYMS	strtod
	SOURCE		$(TGT_DIR)/src/libc/stdlib 
	MODULES		strtod.o
}

Component INCLUDE_ANSI_ATOF {
	NAME		ANSI atof function
	SYNOPSIS	ANSI atof function
	LINK_SYMS	atof
	SOURCE		$(TGT_DIR)/src/libc/stdlib 
	MODULES		atof.o
	REQUIRES	INCLUDE_ANSI_STRTOD
}

Component INCLUDE_ANSI_STDLIB_NUMBERS {
	NAME		ANSI stdlib string to number conversion
	SYNOPSIS	ANSI stdlib - string - number conversion functions
	SOURCE		$(TGT_DIR)/src/libc/stdlib 
	MODULES		atoi.o atol.o strtol.o strtoul.o \
			div.o ldiv.o labs.o qsort.o stdrand.o 
	LINK_SYMS	atoi atol strtol strtoul div ldiv    \
			labs qsort rand 
}

Component INCLUDE_ANSI_STRING {
	NAME		ANSI string
	SYNOPSIS	ANSI string manipulation functions
	SOURCE		$(TGT_DIR)/src/libc/string
	REQUIRES	INCLUDE_BLIB              \
			INCLUDE_ANSI_MEMCHR       \
			INCLUDE_ANSI_MEMCPY       \
			INCLUDE_ANSI_MEMSET       \
			INCLUDE_ANSI_MEMCMP       \
			INCLUDE_ANSI_MEMMOVE      \
			INCLUDE_ANSI_STRCAT       \
			INCLUDE_ANSI_STRNCAT      \
			INCLUDE_ANSI_STRCMP       \
			INCLUDE_ANSI_STRNCMP      \
			INCLUDE_ANSI_STRCASECMP   \
			INCLUDE_ANSI_STRNCASECMP  \
			INCLUDE_ANSI_STRCPY       \
			INCLUDE_ANSI_STRNCPY      \
			INCLUDE_ANSI_STRLCPY      \
			INCLUDE_ANSI_STRLEN
	MODULES		strchr.o strcoll.o strcspn.o strstr.o strtok.o    \
			strpbrk.o strrchr.o strspn.o strtok_r.o strxfrm.o \
			xstate.o
	LINK_SYMS	strchr strcoll strcspn strstr strtok strpbrk strrchr \
			strspn strtok_r strxfrm 
	HELP		ansiString
}

Component INCLUDE_ANSI_TIME {
	NAME		ANSI time
	SYNOPSIS	ANSI time related functions
	SOURCE		$(TGT_DIR)/src/libc/time
	MODULES		asctime.o clock.o ctime.o difftime.o gmtime.o \
			localtime.o mktime.o strftime.o time.o locTime.o
	LINK_SYMS	asctime clock ctime difftime gmtime localtime \
			mktime strftime time
	REQUIRES	INCLUDE_ANSI_STDLIB \
			INCLUDE_ANSI_STRING \
			INCLUDE_POSIX_CLOCKS
	HELP		ansiTime
}

Component INCLUDE_ANSI_ABS {
	NAME		ANSI abs function
	SYNOPSIS	ANSI abs function
	SOURCE		$(TGT_DIR)/src/libc/stdlib 
	LINK_SYMS	abs
	MODULES		abs.o
}

Component INCLUDE_ANSI_MEMCHR {
	NAME		ANSI memchr function
	SYNOPSIS	ANSI memchr function
	SOURCE		$(TGT_DIR)/src/libc/string 
	LINK_SYMS	memchr
	MODULES		memchr.o
}

Component INCLUDE_ANSI_MEMCPY {
	NAME		ANSI memcpy function
	SYNOPSIS	ANSI memcpy function
	SOURCE		$(TGT_DIR)/src/libc/string 
	LINK_SYMS	memcpy
	MODULES		memcpy.o
	REQUIRES	INCLUDE_BLIB
}

Component INCLUDE_ANSI_MEMSET {
	NAME		ANSI memset function
	SYNOPSIS	ANSI memset function
	SOURCE		$(TGT_DIR)/src/libc/string 
	LINK_SYMS	memset
	MODULES		memset.o
	REQUIRES	INCLUDE_BLIB
}

Component INCLUDE_ANSI_MEMCMP {
	NAME		ANSI memcmp function
	SYNOPSIS	ANSI memcmp function
	SOURCE		$(TGT_DIR)/src/libc/string 
	LINK_SYMS	memcmp
	MODULES		memcmp.o
}

Component INCLUDE_ANSI_MEMMOVE {
	NAME		ANSI memmove function
	SYNOPSIS	ANSI memmove function
	SOURCE		$(TGT_DIR)/src/libc/string 
	LINK_SYMS	memmove
	MODULES		memmove.o
}

Component INCLUDE_ANSI_STRCAT {
	NAME		ANSI string cat function
	SYNOPSIS	ANSI string cat function
	SOURCE		$(TGT_DIR)/src/libc/string 
	LINK_SYMS	strcat
	MODULES		strcat.o
}

Component INCLUDE_ANSI_STRNCAT {
	NAME		ANSI string cat function
	SYNOPSIS	ANSI string cat function
	SOURCE		$(TGT_DIR)/src/libc/string 
	LINK_SYMS	strncat
	MODULES		strncat.o
}

Component INCLUDE_ANSI_STRCMP {
	NAME		ANSI string compare function
	SYNOPSIS	ANSI string compare function
	SOURCE		$(TGT_DIR)/src/libc/string 
	LINK_SYMS	strcmp
	MODULES		strcmp.o
}

Component INCLUDE_ANSI_STRNCMP {
	NAME		ANSI string compare function - first N characters
	SYNOPSIS	ANSI string compare function - first N characters
	SOURCE		$(TGT_DIR)/src/libc/string 
	LINK_SYMS	strncmp
	MODULES		strncmp.o
}

Component INCLUDE_ANSI_STRCASECMP {
	NAME		ANSI extension - case insensitive string compare function
	SYNOPSIS	ANSI extension - case insensitive string compare function
	SOURCE		$(TGT_DIR)/src/libc/string 
	LINK_SYMS	strcasecmp
	MODULES		strcasecmp.o
}

Component INCLUDE_ANSI_STRNCASECMP {
	NAME		ANSI extension - case insensitive string compare function - first N characters
	SYNOPSIS	ANSI extension - case insensitive string compare function - first N characters
	SOURCE		$(TGT_DIR)/src/libc/string 
	LINK_SYMS	strncasecmp
	MODULES		strncasecmp.o
}

Component INCLUDE_ANSI_STRCPY {
	NAME		ANSI string copy function
	SYNOPSIS	ANSI string copy function
	SOURCE		$(TGT_DIR)/src/libc/string 
	LINK_SYMS	strcpy
	MODULES		strcpy.o
}

Component INCLUDE_ANSI_STRNCPY {
	NAME		ANSI string copy function - first N characters
	SYNOPSIS	ANSI string copy function - first N characters
	SOURCE		$(TGT_DIR)/src/libc/string 
	LINK_SYMS	strncpy
	MODULES		strncpy.o
}

Component INCLUDE_ANSI_STRLCPY {
	NAME		ANSI strlcpy function
	SYNOPSIS	ANSI strlcpy function
	SOURCE		$(TGT_DIR)/src/libc/string 
	LINK_SYMS	strlcpy
	MODULES		strlcpy.o
}

Component INCLUDE_ANSI_STRLEN {
	NAME		ANSI string length function
	SYNOPSIS	ANSI string length function
	SOURCE		$(TGT_DIR)/src/libc/string 
	LINK_SYMS	strlen
	MODULES		strlen.o
}

Component INCLUDE_ANSI_STRDUP {
	NAME		ANSI string duplication function
	SYNOPSIS	ANSI string duplication function
	SOURCE		$(TGT_DIR)/src/libc/string 
	LINK_SYMS	strdup
	MODULES		strdup.o
	REQUIRES	INCLUDE_MEM_MGR_BASIC
}

Component INCLUDE_ANSI_STRERROR {
	NAME		ANSI errno to error string conversion function
	SYNOPSIS	ANSI errno to error string conversion function
	SOURCE		$(TGT_DIR)/src/libc/string 
	LINK_SYMS	strerror
	MODULES		strerror.o
	REQUIRES	INCLUDE_ANSI_STRCPY  \
			INCLUDE_ANSI_STRNCPY \
			INCLUDE_FORMATTED_OUT_BASIC
}

Component INCLUDE_BLIB {
	NAME		buffer manipulation function library
	SYNOPSIS	buffer manipulation function library
	SOURCE		{ $(TGT_DIR)/src/util bLib.o }
	REQUIRES	INCLUDE_ANSI_STRLEN
	_CHILDREN	FOLDER_NOT_VISIBLE
}

Component INCLUDE_BMPLIB {
	NAME            bit first get function
	SYNOPSIS        bit first get function
	SOURCE          $(TGT_DIR)/src/util 
	MODULES		bmpLib.o
	REQUIRES	INCLUDE_KERNEL
	_CHILDREN	FOLDER_NOT_VISIBLE
}

Folder	FOLDER_CPLUS {
	NAME		C++ components
	SYNOPSIS	C++ class libraries
	CHILDREN	INCLUDE_CTORS_DTORS 	\
                        INCLUDE_CPLUS		\
			INCLUDE_CPLUS_LANG	\
			FOLDER_CPLUS_STDLIB
	DEFAULTS	INCLUDE_CPLUS           \
			INCLUDE_CPLUS_LANG
}

Selection SELECT_COMPILER_INTRINSICS {
	NAME		Downloadable kernel modules compiler support routines
	SYNOPSIS        Compiler support routines that may be needed to \
			support dynamically downloaded object modules \
			generated with the various toolchains. Note that \
			this applies to C code only, as C++ binaries are \
			not compatible between toolchains.
	COUNT		1-
	CHILDREN	INCLUDE_GNU_INTRINSICS \
			INCLUDE_DIAB_INTRINSICS \
			INCLUDE_ICC_INTRINSICS \
			INCLUDE_NO_INTRINSICS
}

Component INCLUDE_GNU_INTRINSICS {
	NAME		GNU compiler support routines
        SYNOPSIS        Compiler support routines that may be needed by \
                        dynamically downloaded object modules generated \
                        with the GNU toolchain. Note that this applies to C \
			code only, as C++ binaries are not compatible between \
			toolchains.
        CONFIGLETTES    intrinsics.c
#ifndef _WRS_CONFIG_SUPPORTS_GNU_INTRINSICS
        _CHILDREN += UNAVAILABLE_LIST
        STATUS_DETAILS Support for GNU compiler was not built in the VSB
#endif
}

Component INCLUDE_DIAB_INTRINSICS {
	NAME		Diab compiler support routines
        SYNOPSIS        Compiler support routines that may be needed by \
                        dynamically downloaded object modules generated \
                        with the Diab toolchain. Note that this applies to C \
			code only, as C++ binaries are not compatible between \
			toolchains.
        CONFIGLETTES    intrinsics.c
#ifndef _WRS_CONFIG_SUPPORTS_DIAB_INTRINSICS
        _CHILDREN += UNAVAILABLE_LIST
        STATUS_DETAILS Support for DIAB compiler was not built in the VSB
#endif
}

Component INCLUDE_ICC_INTRINSICS {
	NAME		Intel compiler support routines
        SYNOPSIS        Compiler support routines that may be needed by \
                        dynamically downloaded object modules generated \
                        with the Intel toolchain. Note that this applies to C \
			code only, as C++ binaries are not compatible between \
			toolchains.
        CONFIGLETTES    intrinsics.c
	REQUIRES	INCLUDE_GNU_INTRINSICS
#ifndef _WRS_CONFIG_SUPPORTS_ICC_INTRINSICS
        _CHILDREN += UNAVAILABLE_LIST
        STATUS_DETAILS Support for ICC compiler was not built in the VSB
#endif
}

Component INCLUDE_NO_INTRINSICS {
	NAME		No intrinsics support
	SYNOPSIS	Only include compiler support routines that are required to build kernel
	EXCLUDES	INCLUDE_GNU_INTRINSICS INCLUDE_DIAB_INTRINSICS INCLUDE_ICC_INTRINSICS
}

Component INCLUDE_CPLUS {
	NAME		C++ core runtime
	SYNOPSIS        Basic support for C++ applications
	SOURCE		$(TGT_DIR)/src/cplus/rts
	INIT_RTN	usrCplusLibInit ();
	HDR_FILES	private/cplusLibP.h 
	MODULES		cplusCore.o \
			cplusInit.o \
			cplusLib.o \
			cxa_atexit.o
	CONFIGLETTES	usrCplus.c
        REQUIRES        INCLUDE_CTORS_DTORS
}

Component INCLUDE_CPLUS_LANG {
	NAME		C++ compiler support routines
	SYNOPSIS	Support for exception handling, new, delete etc.
}

Component INCLUDE_CTORS_DTORS {
	NAME		run static initializers
        SYNOPSIS        run compiler generated initialization functions at system startup
	SOURCE		$(TGT_DIR)/src/cplus/rts
	INIT_RTN	cplusCtorsLink ();
	HDR_FILES	cplusLib.h 
	MODULES		cplusXtors.o
}

Folder FOLDER_CPLUS_STDLIB {
	NAME		standard library
	SYNOPSIS	C++ Standard Library
	CHILDREN	INCLUDE_CPLUS_IOSTREAMS
	DEFAULTS	INCLUDE_CPLUS_IOSTREAMS 
}

Folder	FOLDER_POSIX {
	NAME		POSIX components
	CHILDREN	INCLUDE_POSIX_ADVISORY_FILE_LOCKING \
			INCLUDE_POSIX_AIO	\
			INCLUDE_POSIX_AIO_SYSDRV \
			INCLUDE_POSIX_FTRUNC	\
			INCLUDE_POSIX_MEM	\
			INCLUDE_POSIX_MQ	\
			INCLUDE_POSIX_PTHREADS  \
			INCLUDE_POSIX_SCHED	\
			INCLUDE_POSIX_SEM	\
			INCLUDE_POSIX_SIGNALS	\
			INCLUDE_POSIX_TIMERS	\
			INCLUDE_POSIX_DIRLIB	\
			INCLUDE_POSIX_CLOCKS	\
			INCLUDE_SIGEVENT
	DEFAULTS	INCLUDE_POSIX_ADVISORY_FILE_LOCKING \
			INCLUDE_POSIX_AIO			\
			INCLUDE_POSIX_AIO_SYSDRV		\
			INCLUDE_POSIX_FTRUNC			\
			INCLUDE_POSIX_MQ			\
			INCLUDE_POSIX_SEM			\
			INCLUDE_POSIX_SIGNALS			\
			INCLUDE_POSIX_TIMERS			\
			INCLUDE_POSIX_CLOCKS			\
			INCLUDE_POSIX_DIRLIB			\
			INCLUDE_POSIX_PTHREADS
}

Component INCLUDE_POSIX_ADVISORY_FILE_LOCKING {
	NAME		POSIX advisory file locking
	SYNOPSIS	provides advisory file locking in POSIX file systems
	CONFIGLETTES	usrPosixAdvLock.c
	INIT_RTN	usrPosixAdvLockInit ();
}

Component INCLUDE_POSIX_AIO {
	NAME		POSIX asynchronous IO
	MODULES		aioPxLib.o
	INIT_RTN	aioPxLibInit (MAX_LIO_CALLS);
	CFG_PARAMS	MAX_LIO_CALLS
	HDR_FILES	aio.h
	REQUIRES	INCLUDE_PIPES
	INIT_AFTER	INCLUDE_PIPES
}

Component INCLUDE_POSIX_AIO_SYSDRV {
	NAME		POSIX AIO driver
	SYNOPSIS	provides AIO functionality to non-AIO drivers
	MODULES		aioSysDrv.o
	INIT_RTN	aioSysInit (MAX_AIO_SYS_TASKS, \
				AIO_TASK_PRIORITY, AIO_TASK_STACK_SIZE);
	CFG_PARAMS	MAX_AIO_SYS_TASKS \
			AIO_TASK_PRIORITY \
			AIO_TASK_STACK_SIZE
	HDR_FILES	aioSysDrv.h
}

Component INCLUDE_POSIX_FTRUNC {
	NAME		POSIX ftruncate
	MODULES		ftruncate.o
	LINK_SYMS	ftruncate
	HDR_FILES	unistd.h
}

Component INCLUDE_POSIX_MEM {
	NAME		POSIX mman
	SYNOPSIS	null-wrappers for POSIX memory paging routines
	MODULES		mmanPxLib.o
	LINK_SYMS	mlockall
	HDR_FILES	sys/mman.h
}

Component INCLUDE_POSIX_MQ {
	NAME		POSIX message queues
	MODULES		mqPxLib.o
	INIT_RTN	mqPxLibInit (MQ_HASH_SIZE);
	CFG_PARAMS	MQ_HASH_SIZE
	HDR_FILES	private/mqPxLibP.h
	REQUIRES	INCLUDE_OBJ_OPEN
}

Component INCLUDE_POSIX_PTHREADS {
	NAME		POSIX threads
	SYNOPSIS	Support for POSIX threads in the kernel
	MODULES		pthreadLib.o _pthreadLib.o
	INIT_RTN	pthreadLibInit ();	
	HDR_FILES	private/pthreadLibP.h
	REQUIRES	INCLUDE_TASK_OPEN
}

Component INCLUDE_POSIX_SCHED {
	NAME		POSIX process scheduling
	SYNOPSIS	Kernel-only emulation of process scheduling API - no scheduler
	MODULES		schedPxLib.o
	LINK_SYMS	sched_setparam
	HDR_FILES	sched.h
}

Component INCLUDE_POSIX_SEM {
	NAME		POSIX semaphores
	MODULES		semPxLib.o
	INIT_RTN	semPxLibInit ();
	HDR_FILES	classLib.h private/semPxLibP.h
	REQUIRES	INCLUDE_OBJ_OPEN \
			INCLUDE_SEM_COUNTING_CREATE
}

Component INCLUDE_POSIX_SIGNALS {
	NAME		POSIX signals
	SYNOPSIS	queued signals
	INIT_RTN	sigqueueInit (NUM_SIGNAL_QUEUES);
	CFG_PARAMS	NUM_SIGNAL_QUEUES
	HDR_FILES	private/sigLibP.h
}

Component INCLUDE_SIGEVENT {
	NAME		sigevent notification library
	SYNOPSIS	Kernel signal event support.  Required for SIGEV_THREAD support in RTP.
	MODULES		sigeventLib.o
	PROTOTYPE       extern int sigeventLibInit (void);
	INIT_RTN	sigeventLibInit ();
	HDR_FILES	sigevent.h
	REQUIRES	INCLUDE_SIGNALS
	_CHILDREN	FOLDER_POSIX
}

Component INCLUDE_POSIX_TIMERS {
	NAME		POSIX timers
	MODULES		timerLib.o
	INIT_RTN	timerLibInit ();
	LINK_SYMS	nanosleep
	HDR_FILES	time.h private/timerLibP.h
        REQUIRES        INCLUDE_SIGEVENT INCLUDE_SIGNALS INCLUDE_POSIX_CLOCKS
}

Component INCLUDE_POSIX_CLOCKS {
        NAME            POSIX clocks
        SOURCE          $(TGT_DIR)/src/os
        MODULES         clockLib.o
        PROTOTYPE       extern STATUS clockLibInit (void);
        INIT_RTN        clockLibInit ();
        REQUIRES        INCLUDE_KERNEL   \
                        INCLUDE_FORMATTED_OUT_BASIC
}

Component INCLUDE_POSIX_DIRLIB {
	NAME		POSIX directory utilities
	SOURCE		$(TGT_DIR)/src/os/io
	MODULES		dirLib.o
	LINK_SYMS	opendir
	REQUIRES	INCLUDE_IO_SYSTEM \
			INCLUDE_MEM_MGR_BASIC
}

Component INCLUDE_POSIX_TIMER_SHOW {
	NAME		Posix timer show component 
	SYNOPSIS	This is a utility that will allow the user to display a snap shot of the selected timer. Requires INCLUDE_POSIX_TIMERS
	MODULES         timerShow.o
	INIT_RTN	timerShowInit();
	HDR_FILES	time.h
	INCLUDE_WHEN	INCLUDE_OBJ_INFO \
			INCLUDE_SHOW_ROUTINES
	REQUIRES	INCLUDE_POSIX_TIMERS \
			INCLUDE_OBJ_INFO
}

Component INCLUDE_RTP_DEBUG_COMMAND {
        NAME            RTP debug command facility
        SYNOPSIS        Allows to send commands to perform operations in an RTP context
        INIT_RTN        rtpDbgCmdLibInit ();
        HDR_FILES       private/rtpDbgCmdLibP.h
        _CHILDREN       FOLDER_NOT_VISIBLE
        INCLUDE_WHEN    INCLUDE_RTP INCLUDE_SHELL
        REQUIRES        INCLUDE_RTP
}

/* default show routine are class and task show routines */

Component INCLUDE_SHOW_ROUTINES {
	NAME		show routine component
	SYNOPSIS	enable show routines for configured features
}

Folder FOLDER_SHOW_ROUTINES {
	NAME		show routines
	SYNOPSIS	enable display of components states
	CHILDREN	INCLUDE_CLASS_SHOW	\
			INCLUDE_MEM_SHOW	\
			INCLUDE_VM_SHOW		\
			INCLUDE_MSG_Q_SHOW	\
			INCLUDE_POSIX_AIO_SHOW	\
			INCLUDE_POSIX_MQ_SHOW	\
			INCLUDE_POSIX_SEM_SHOW	\
			INCLUDE_POSIX_TIMER_SHOW\
			INCLUDE_SEM_SHOW	\
			INCLUDE_STDIO_SHOW	\
			INCLUDE_TASK_HOOKS_SHOW	\
			INCLUDE_TASK_SHOW	\
			INCLUDE_WATCHDOGS_SHOW	\
			INCLUDE_TRIGGER_SHOW	\
			INCLUDE_RBUFF_SHOW	\
                        INCLUDE_DATACOLLECTOR_SHOW \
			INCLUDE_ATA_SHOW        \
			INCLUDE_PCI_CFGSHOW	\
			INCLUDE_HW_FP_SHOW	\
			INCLUDE_MAC_SHOW	\
			INCLUDE_EXC_SHOW	\
			INCLUDE_EDR_SHOW        \
			INCLUDE_ISR_SHOW	\
			INCLUDE_POOL_SHOW	\
			INCLUDE_HANDLE_SHOW	\
			INCLUDE_OBJECT_SHOW	\
			INCLUDE_SHOW_ROUTINES
	DEFAULTS	INCLUDE_SHOW_ROUTINES
}

Component INCLUDE_ATA_SHOW {
	NAME		ata show routine
	SYNOPSIS	ata information
	MODULES		ataShow.o
	INIT_RTN	ataShowInit ();
	HDR_FILES	drv/hdisk/ataDrv.h drv/pcmcia/pccardLib.h 
	INCLUDE_WHEN	INCLUDE_ATA \
			INCLUDE_SHOW_ROUTINES
}

Component INCLUDE_PCI_CFGSHOW {
	NAME		pci show routines
	SYNOPSIS	Non VxBus pci information from pciConfigShow routines
	CONFIGLETTES	../../../src/drv/pci/pciConfigShow.c
	INIT_RTN	pciConfigShowInit ();
	HDR_FILES	drv/pci/pciConfigLib.h	\
			drv/pci/pciConfigShow.h	\
			drv/pci/pciHeaderDefs.h
	INCLUDE_WHEN	INCLUDE_PCI_CFG \
			INCLUDE_SHOW_ROUTINES
}

Component INCLUDE_HW_FP_SHOW {
	NAME		floating point show routine
	SYNOPSIS	task floating point registers
	MODULES		aimFppShow.o
	INIT_RTN	aimFppShowInit ();
	HDR_FILES	fppLib.h
	INCLUDE_WHEN	INCLUDE_HW_FP \
			INCLUDE_SHOW_ROUTINES
}

Component INCLUDE_MSG_Q_SHOW {
	NAME		message queue show routine
	SOURCE		$(TGT_DIR)/src/wind
	MODULES		msgQShow.o
	INIT_RTN	msgQShowInit ();
	REQUIRES        INCLUDE_MSG_Q_INFO
	INCLUDE_WHEN	INCLUDE_MSG_Q \
			INCLUDE_SHOW_ROUTINES
}

Component INCLUDE_SEM_SHOW {
	NAME		semaphore show routine
	MODULES		semShow.o
	INIT_RTN	semShowInit ();
	REQUIRES	INCLUDE_SEM_INFO
	INCLUDE_WHEN	INCLUDE_SEM_BINARY \
			INCLUDE_SHOW_ROUTINES
}

Component INCLUDE_STDIO_SHOW {
	NAME		stdio show routine
	MODULES		stdioShow.o
	INIT_RTN	stdioShowInit ();
	HDR_FILES	stdio.h
	INCLUDE_WHEN	INCLUDE_STDIO \
			INCLUDE_SHOW_ROUTINES
}

Component INCLUDE_TASK_HOOKS_SHOW {
	NAME		task hook show routine
	MODULES		taskHookShow.o
	INIT_RTN	taskHookShowInit ();
	HDR_FILES	taskHookLib.h
	INCLUDE_WHEN	INCLUDE_TASK_HOOKS \
			INCLUDE_SHOW_ROUTINES
}

Component INCLUDE_WATCHDOGS_SHOW {
	NAME		watchdog timer show routine
	MODULES		wdShow.o
	INIT_RTN	wdShowInit ();
	INCLUDE_WHEN	INCLUDE_WATCHDOGS \
			INCLUDE_SHOW_ROUTINES
}

Component INCLUDE_TASK_SHOW {
	NAME		task show routine
	MODULES		taskShow.o
	INIT_RTN	taskShowInit ();
	REQUIRES	INCLUDE_TASK_LIST
}

Component INCLUDE_MEM_SHOW {
	NAME		memory show routine
	MODULES		memShow.o
	INIT_RTN	memShowInit ();
	REQUIRES	INCLUDE_MEM_MGR_BASIC \
			INCLUDE_MEM_MGR_INFO
	INCLUDE_WHEN	INCLUDE_MEM_MGR_BASIC \
			INCLUDE_SHOW_ROUTINES
}

Component INCLUDE_CLASS_SHOW {
	NAME		class show routine
	MODULES		classShow.o
	INIT_RTN	classShowInit ();
	REQUIRES	INCLUDE_CLASS_LIST
	INCLUDE_WHEN	INCLUDE_CLASS_LIST \
			INCLUDE_SHOW_ROUTINES
}

Component INCLUDE_POSIX_AIO_SHOW {
	NAME		asynchronous IO show routine
	MODULES		aioPxShow.o
	LINK_SYMS	aioShow
	INCLUDE_WHEN	INCLUDE_POSIX_AIO \
			INCLUDE_SHOW_ROUTINES
}

Component INCLUDE_POSIX_MQ_SHOW {
	NAME		POSIX message queue show routine
	MODULES		mqPxShow.o
	INIT_RTN	mqPxShowInit ();
	HDR_FILES	mqPxShow.h
	INCLUDE_WHEN	INCLUDE_POSIX_MQ \
			INCLUDE_SHOW_ROUTINES
}

Component INCLUDE_POSIX_SEM_SHOW {
	NAME		POSIX semaphore show routine
	MODULES		semPxShow.o
	INIT_RTN	semPxShowInit ();
	HDR_FILES	semPxShow.h
	INCLUDE_WHEN	INCLUDE_POSIX_SEM \
			INCLUDE_SHOW_ROUTINES
}

Component INCLUDE_TRIGGER_SHOW {
	NAME		trigger show routine
	MODULES		trgShow.o
	INIT_RTN	trgShowInit ();
        HDR_FILES       trgLib.h private/trgLibP.h
	REQUIRES	INCLUDE_TRIGGERING
	INCLUDE_WHEN	INCLUDE_TRIGGERING \
			INCLUDE_SHOW_ROUTINES
}

Component INCLUDE_RBUFF_SHOW {
	NAME		rBuff show routine
	MODULES		rBuffShow.o
	INIT_RTN	rBuffShowInit ();
        HDR_FILES       rBuffLib.h
	REQUIRES	INCLUDE_RBUFF
	INCLUDE_WHEN	INCLUDE_RBUFF \
			INCLUDE_SHOW_ROUTINES
}

Component INCLUDE_EDR_SHOW {
	NAME		ED&R show routines
	MODULES		edrShow.o
	INIT_RTN	edrShowInit ();
        HDR_FILES       edrLib.h
	REQUIRES	INCLUDE_EDR_ERRLOG
	INCLUDE_WHEN	INCLUDE_EDR_ERRLOG \
			INCLUDE_SHOW_ROUTINES
}

Component INCLUDE_ISR_SHOW {
	NAME            isr object show routines
	SYNOPSIS        ISR object show routines
	PROTOTYPE       STATUS isrShowInit (void);
	INIT_RTN        isrShowInit ();
	MODULES         isrShow.o
}

Component INCLUDE_HANDLE_SHOW {
	NAME		handle show routines
	SYNOPSIS	handle show routines
	MODULES		handleShow.o
	INCLUDE_WHEN	INCLUDE_SHOW_ROUTINES
}

Component INCLUDE_OBJECT_SHOW {
	NAME            object show routines
	SYNOPSIS        object show routines
	MODULES         objShow.o
	INIT_RTN        objShowInit ();
	REQUIRES        INCLUDE_CLASS_LIST      \
			INCLUDE_OBJ_LIB
	INCLUDE_WHEN    INCLUDE_CLASS_LIST      \
			INCLUDE_OBJ_LIB         \
			INCLUDE_SHOW_ROUTINES
}

Folder	FOLDER_TOOLS {
	NAME		development tool components
	SYNOPSIS	development tools
	CHILDREN	FOLDER_WDB		\
			FOLDER_SHOW_ROUTINES	\
			INCLUDE_SYSDBG_FLAG	\
			INCLUDE_SPY		\
			INCLUDE_TIP		\
			INCLUDE_TIMEX		\
			INCLUDE_VXDBG		\
			INCLUDE_TRIGGERING	\
			FOLDER_WINDVIEW		\
			SELECT_COMPILER_INTRINSICS \
			FOLDER_KERNEL_DEBUG
	DEFAULTS	FOLDER_WDB
}

Component INCLUDE_SPY {
	NAME		spy
	SYNOPSIS	utility to measure per-task CPU utilization
	MODULES		spyLib.o
	INIT_RTN	spyLibInit (SPY_TASKS_MAX);
	REQUIRES	INCLUDE_AUX_CLK
	HDR_FILES	spyLib.h
	CFG_PARAMS	SPY_TASKS_MAX
}

Parameter SPY_TASKS_MAX {
	NAME		Number of tasks to spy
	SYNOPSIS	Maximun number of taks that can be spied
	TYPE		int
	DEFAULT		200
}

Component INCLUDE_JITTER_TOOL {
	NAME		jitter measurement tool
	SYNOPSIS	utility to measure task and interrupt response jitter
	MODULES		jitterLib.o
	INIT_RTN	jitterLibInit ();
	PROTOTYPE	STATUS jitterLibInit (void);
	REQUIRES	INCLUDE_AUX_CLK INCLUDE_SYS_TIMESTAMP
	_CHILDREN	FOLDER_NOT_VISIBLE
}

Component INCLUDE_TIMEX {
	NAME		timex
	SYNOPSIS	utility to measure function execution time
	SOURCE		$(TGT_DIR)/src/ostool
	MODULES		timexLib.o
	INIT_RTN	timexInit ();
	HDR_FILES	timexLib.h
}

Component INCLUDE_TIP {
        NAME            tip serial line connection utility
        SYNOPSIS        interactive utility to connect to and manage multiple serial lines
	CONFIGLETTES    usrTip.c
        MODULES         tipLib.o
        INIT_RTN        usrTipInit (TIP_CONFIG_STRING, TIP_ESCAPE_CHARACTER);
        HDR_FILES       private/tipLibP.h
	LINK_SYMS       tipStart
        INCLUDE_WHEN    INCLUDE_SHELL
        CFG_PARAMS      TIP_CONFIG_STRING\
			TIP_ESCAPE_CHARACTER
        HELP            tip
}

Parameter TIP_CONFIG_STRING {
        NAME            Tip global configuration string
        SYNOPSIS        Used to define connection lines attributes
        TYPE            string
        DEFAULT         ""
}

Parameter TIP_ESCAPE_CHARACTER {
        NAME            Tip escape character
        SYNOPSIS        Character toggling command mode
        TYPE            string
        DEFAULT         "~"
}

Folder  FOLDER_APPLICATION {
        NAME            application components
        CHILDREN        INCLUDE_USER_APPL \
                        INCLUDE_USER_PRE_KERNEL_APPL_INIT \
                        INCLUDE_USER_POST_KERNEL_APPL_INIT \
                        INCLUDE_USER_PRE_NETWORK_APPL_INIT
        DEFAULTS        INCLUDE_USER_APPL
}

Component INCLUDE_USER_APPL {
	NAME		application initialization
	SYNOPSIS	call usrAppInit() (in your usrAppInit.c project file) after startup.
	INIT_RTN	usrAppInit ();
}

Component INCLUDE_USER_PRE_KERNEL_APPL_INIT {
        NAME            pre-kernel application initialization
        SYNOPSIS        call usrPreKernelAppInit() (in your usrAppInit.c project file) 
        INIT_RTN        usrPreKernelAppInit ();
}

Component INCLUDE_USER_POST_KERNEL_APPL_INIT {
        NAME            post-kernel application initialization
        SYNOPSIS        call usrPostKernelAppInit() (in your usrAppInit.c project file) 
        INIT_RTN        usrPostKernelAppInit ();
}

Component INCLUDE_USER_PRE_NETWORK_APPL_INIT {
        NAME            pre-network application initialization
        SYNOPSIS        call usrPreNetworkAppInit() (in your usrAppInit.c project file) 
        INIT_RTN        usrPreNetworkAppInit ();
}

Component INCLUDE_SIMPLE_BANNER {
	NAME		simple banner containing VxWorks version & creation date
	SYNOPSIS	simple banner on console after the system initializes
	CONFIGLETTES	simpleBanner.c
	INIT_RTN	simpleBanner ();
	REQUIRES	INCLUDE_FORMATTED_OUT_BASIC
}

Folder	FOLDER_KERNEL {
	NAME		kernel components
	CHILDREN	INCLUDE_KERNEL		\
			INCLUDE_BOOT_LINE_INIT	\
			INCLUDE_EXC_HANDLING	\
			INCLUDE_ISR_OBJECTS     \
			SELECT_MEM_ALLOC	\
			INCLUDE_MEM_MGR_FULL	\
			INCLUDE_MEM_MGR_INFO	\
			INCLUDE_OBJ_LIB		\
			INCLUDE_OBJ_INFO	\
			INCLUDE_OBJ_OWNERSHIP	\
			INCLUDE_VXMEMPROBE_INIT \
			INCLUDE_VXEVENTS	\
			INCLUDE_SEM_BINARY	\
			INCLUDE_SEM_MUTEX	\
			INCLUDE_SEM_COUNTING	\
			INCLUDE_SEM_READ_WRITE	\
			INCLUDE_SEM_DELETE      \
			INCLUDE_SEM_EXCHANGE    \
			INCLUDE_SEM_BINARY_CREATE		\
			INCLUDE_SEM_MUTEX_CREATE		\
			INCLUDE_SEM_COUNTING_CREATE		\
			INCLUDE_SEM_READ_WRITE_CREATE		\
			INCLUDE_SEM_INFO	\
			INCLUDE_SIGNALS		\
			INCLUDE_MSG_Q		\
			INCLUDE_MSG_Q_CREATE_DELETE 	\
			INCLUDE_MSG_Q_INFO		\
			INCLUDE_WATCHDOGS		\
			INCLUDE_WATCHDOGS_CREATE_DELETE	\
			INCLUDE_TASK_HOOKS         	\
			INCLUDE_TASK_CREATE_HOOKS	\
			INCLUDE_TASK_SWITCH_HOOKS	\
			INCLUDE_TASK_INFO	\
			INCLUDE_TASK_LIST	\
			INCLUDE_TASK_ROTATE	\
			INCLUDE_TASK_UTIL	\
			INCLUDE_ENV_VARS	\
			INCLUDE_JOB_TASK        \
			SELECT_KERNEL_SCHEDULER \
			INCLUDE_REBOOT_HOOKS    \
			INCLUDE_EXC_TASK	\
			INCLUDE_TLS
	DEFAULTS	INCLUDE_KERNEL			\
			INCLUDE_EXC_HANDLING		\
			INCLUDE_MEM_MGR_FULL		\
			INCLUDE_MEM_MGR_INFO		\
			INCLUDE_OBJ_LIB			\
			INCLUDE_SEM_BINARY		\
			INCLUDE_SEM_MUTEX		\
			INCLUDE_SEM_COUNTING		\
			INCLUDE_SEM_BINARY_CREATE	\
			INCLUDE_SEM_COUNTING_CREATE	\
			INCLUDE_SEM_MUTEX_CREATE	\
			INCLUDE_SEM_DELETE		\
			INCLUDE_SIGNALS			\
			INCLUDE_MSG_Q			\
			INCLUDE_MSG_Q_CREATE_DELETE	\
			INCLUDE_TASK_HOOKS		\
			INCLUDE_TASK_UTIL		\
			INCLUDE_WATCHDOGS		\
			INCLUDE_WATCHDOGS_CREATE_DELETE	\
			INCLUDE_ENV_VARS		\
			INCLUDE_EXC_TASK		\
			INCLUDE_JOB_TASK		\
			INCLUDE_TASK_CREATE_DELETE	\
			INCLUDE_TASK_RESTART		\
			INCLUDE_HOOKS			\
			INCLUDE_REBOOT_HOOKS		\
			SELECT_KERNEL_SCHEDULER		\
			INCLUDE_VXMEMPROBE_INIT
}

Component INCLUDE_KERNEL {
	NAME		kernel
	SYNOPSIS	context switch and interrupt handling (DO NOT REMOVE).
	SOURCE		{$(TGT_DIR)/src/wind \
				{kernelLib.o taskLib.o windLib.o \
				schedLib.o workQLib.o tickLib.o }} \
			{$(TGT_DIR)/src/os \
				{funcBind.o rebootLib.o intLib.o \
				ffsLib.o errnoLib.o }}
	CONFIGLETTES	usrKernel.c usrKernelStack.c
	INIT_RTN	usrKernelInit (VX_GLOBAL_NO_STACK_FILL);
	CFG_PARAMS	ROOT_STACK_SIZE		\
			ISR_STACK_SIZE		\
			INT_LOCK_LEVEL		\
                        TASK_USER_EXC_STACK_SIZE        \
                        TASK_USER_EXC_STACK_OVERFLOW_SIZE       \
                        TASK_USER_EXEC_STACK_OVERFLOW_SIZE      \
                        TASK_USER_EXEC_STACK_UNDERFLOW_SIZE     \
                        TASK_KERNEL_EXEC_STACK_OVERFLOW_SIZE    \
                        TASK_KERNEL_EXEC_STACK_UNDERFLOW_SIZE   \
			TASK_PRIORITY_SET_COMPATIBLE		\
			WIND_JOBS_MAX				\
			VX_GLOBAL_NO_STACK_FILL			\
			RUNTIME_CREATION_TIME			
	REQUIRES	INCLUDE_BSP_MACROS	\
			INCLUDE_DLL 		\
			INCLUDE_MEMORY_CONFIG	\
			INCLUDE_KERNEL_ARCH 	\
			INCLUDE_EXC_HANDLING 	\
			INCLUDE_FORMATTED_OUT_BASIC	\
			SELECT_KERNEL_SCHEDULER \
			INCLUDE_Q_FIFO 		\
			INCLUDE_Q_PRI_LIST 	\
			INCLUDE_Q_PRI_BMAP 	\
			INCLUDE_Q_PRI_DELTA	\
			INCLUDE_OFFSET_SYMBOLS	\
			INCLUDE_EDR_STUB	\
			INCLUDE_ANSI_STRCPY	\
			INCLUDE_SYSHW_INIT	\
			INCLUDE_SYS_START   \
			INCLUDE_BOOT_HW_INIT \
			INCLUDE_SYSHW_INIT2
	HDR_FILES	sysLib.h private/kernelLibP.h private/workQLibP.h \
			private/qLibP.h taskLib.h usrConfig.h	\
			private/isrLibP.h private/excLibP.h
}

Component INCLUDE_BASE_KERNEL {
	NAME		early kernel object support
	SOURCE		$(TGT_DIR)/src/os
	SYNOPSIS	required component DO NOT REMOVE.
	MODULES		kernelBaseLib.o
	INIT_RTN	kernelBaseInit ();
	REQUIRES	INCLUDE_KERNEL    \
			INCLUDE_OBJ_LIB   \
			INCLUDE_SEM_MUTEX
	HDR_FILES	private/kernelBaseLibP.h
}

Component INCLUDE_TASK_CREATE_DELETE {
	NAME		dynamic creation and deletion of tasks
	SYNOPSIS	Capability to dynamically instantiate and delete tasks
	SOURCE		$(TGT_DIR)/src/wind
	MODULES		taskCreateLib.o
	INIT_RTN	taskCreateLibInit ();
	PROTOTYPE       void taskCreateLibInit (void);
	REQUIRES	INCLUDE_KERNEL        \
			SELECT_MEM_ALLOC		\
			INCLUDE_OBJ_LIB       \
			INCLUDE_ANSI_STRCPY   \
			INCLUDE_ANSI_STRLEN   \
			INCLUDE_FORMATTED_OUT_BASIC \
			INCLUDE_ATOMIC_OPERATORS
}

Component INCLUDE_TASK_RESTART {
	NAME		Ability to restart/reset tasks
	SYNOPSIS	Ability to restart/reset tasks
	SOURCE		$(TGT_DIR)/src/wind
	MODULES		taskRestartLib.o
	LINK_SYMS	taskRestart
	REQUIRES	INCLUDE_KERNEL        \
			INCLUDE_OBJ_LIB       \
			INCLUDE_ANSI_STRCPY   \
			INCLUDE_ANSI_STRLEN   \
			INCLUDE_TASK_CREATE_DELETE
}

Component INCLUDE_REBOOT_HOOKS {
	NAME		Support for reboot hooks
	SOURCE		$(TGT_DIR)/src/os
	MODULES		rebootHookLib.o
	CFG_PARAMS	MAX_REBOOT_HOOKS
	REQUIRES	INCLUDE_HOOKS
}

Folder	FOLDER_EDR {
	NAME		ED&R components
	CHILDREN	INCLUDE_EDR_PM		\
			INCLUDE_EDR_SYSDBG_FLAG \
			INCLUDE_EDR_ERRLOG      \
			INCLUDE_EDR_POLICY_HOOKS
	DEFAULTS	INCLUDE_EDR_POLICY_HOOKS
}

Component INCLUDE_ENV_VARS {
	NAME		unix compatible environment variables
	SOURCE		$(TGT_DIR)/src/os
	MODULES		envLib.o
	INIT_RTN	envLibInit (ENV_VAR_USE_HOOKS);
	CFG_PARAMS	ENV_VAR_USE_HOOKS
	HDR_FILES	envLib.h
	REQUIRES	INCLUDE_KERNEL                \
			INCLUDE_LOGGING               \
			INCLUDE_TASK_CREATE_HOOKS     \
			INCLUDE_ANSI_STRCPY           \
			INCLUDE_ANSI_STRLEN           \
			INCLUDE_ANSI_STRNCMP          \
			INCLUDE_MEM_MGR_BASIC
}

Component INCLUDE_EXC_TASK {
        NAME            exception task
        SYNOPSIS        interrupt-level job facility
	SOURCE		$(TGT_DIR)/src/os
        MODULES         excLib.o 
	REQUIRES	INCLUDE_KERNEL       \
			INCLUDE_SEM_BINARY
	CFG_PARAMS	MAX_ISR_JOBS
        INIT_RTN        excInit (MAX_ISR_JOBS);
        HDR_FILES       excLib.h
}

Component INCLUDE_JOB_TASK {
        NAME            job task
        SYNOPSIS        task level work deferral
	SOURCE		$(TGT_DIR)/src/os
        MODULES         jobTaskLib.o
        INIT_RTN        jobTaskLibInit (JOB_TASK_STACK_SIZE);
	CFG_PARAMS	JOB_TASK_STACK_SIZE
        HDR_FILES       private/jobLibP.h
	REQUIRES	INCLUDE_KERNEL               \
			INCLUDE_OBJ_LIB              \
			INCLUDE_SEM_MUTEX            \
			INCLUDE_ANSI_STRNCPY         \
			INCLUDE_TASK_CREATE_DELETE   \
			INCLUDE_FORMATTED_OUT_BASIC 
}

Component INCLUDE_JOB_DEFER {
        NAME            job deferral
        SYNOPSIS        abstracted work deferral
        MODULES         jobLib.o
        LINK_SYMS       jobAdd
        HDR_FILES       private/jobLibP.h
	REQUIRES	INCLUDE_EXC_TASK
	_CHILDREN	FOLDER_NOT_VISIBLE
}

Component INCLUDE_ISR_DEFER {
        NAME            ISR deferral
        SYNOPSIS        ISR deferral facility
	SOURCE		$(TGT_DIR)/src/hwif/util
        MODULES         isrDeferLib.o 
	REQUIRES	INCLUDE_KERNEL			\
			INCLUDE_ANSI_STRCPY		\
			INCLUDE_LSTLIB			\
			INCLUDE_SEM_BINARY_CREATE	\
			INCLUDE_SEM_BINARY		\
			SELECT_MEM_ALLOC		\
			INCLUDE_TASK_CREATE_DELETE	\
			INCLUDE_VXBUS
        INIT_RTN        usrIsrDeferInit ();
        HDR_FILES       isrDeferLib.h
	CONFIGLETTES	usrIsrDefer.c
	CFG_PARAMS	ISR_DEFER_MODE
}

Parameter ISR_DEFER_MODE {
	NAME		ISR deferral mode
	SYNOPSIS	interrupt service routines deferral handling global mode
	TYPE		int
	DEFAULT		ISR_DEFER_MODE_PER_CPU
}

Component INCLUDE_ISR_OBJECTS {
        NAME            Interrupt Service Routine Object Library
        SYNOPSIS        Objects describing interrupt service routines
	SOURCE		$(TGT_DIR)/src/wind
        MODULES         isrLib.o
	HDR_FILES	private/isrLibP.h
	REQUIRES	INCLUDE_KERNEL               \
			INCLUDE_OBJ_LIB              \
			INCLUDE_OBJ_INFO	     \
			INCLUDE_ANSI_STRNCPY
}

Parameter FUNCPTR_OBJ_MEMALLOC_RTN {
        NAME            Function pointer to the objMemAllocRtn
        SYNOPSIS        Function pointer to the objMemAllocRtn
        TYPE            void *
        DEFAULT         (INCLUDE_MEM_ALLOT)::(objMemAllot) memPartAlignedAlloc
}

Parameter FUNCPTR_OBJ_MEMFREE_RTN {
        NAME            Function pointer to the objMemFreeRtn
        SYNOPSIS        Function pointer to the objMemFreeRtn
        TYPE            FUNCPTR
        DEFAULT         (INCLUDE_MEM_ALLOT)::(NULL) memPartFree
}

Parameter OBJ_MEM_POOL_ID {
        NAME            object memory pool Id
        SYNOPSIS        object memory pool Id
        TYPE            void *
        DEFAULT         (INCLUDE_MEM_ALLOT)::(NULL) memSysPartId
}

Component INCLUDE_OBJ_LIB {
	NAME		object management
	SOURCE		$(TGT_DIR)/src/os
	MODULES		objLib.o 
	REQUIRES	INCLUDE_ANSI_STRNCPY \
                        INCLUDE_ANSI_STRCMP  \
                        INCLUDE_ANSI_STRLEN  \
                        INCLUDE_CLASS_LIB    \
			INCLUDE_KERNEL       \
			INCLUDE_SEM_MUTEX
	INIT_RTN	objLibInit ((OBJ_ALLOC_FUNC)FUNCPTR_OBJ_MEMALLOC_RTN, \
				    (OBJ_FREE_FUNC)FUNCPTR_OBJ_MEMFREE_RTN,   \
				    OBJ_MEM_POOL_ID, 			      \
				    OBJ_LIBRARY_OPTIONS);
	CFG_PARAMS	FUNCPTR_OBJ_MEMALLOC_RTN    \
	                FUNCPTR_OBJ_MEMFREE_RTN     \
                       	OBJ_MEM_POOL_ID             \
	                OBJ_LIBRARY_OPTIONS
	HDR_FILES	objLib.h
}

Component INCLUDE_OBJ_INFO {
	NAME		object information
	SYNOPSIS	object management routines that requires lookup in a \
			list of objects, such as the objNameToId() routine.
	SOURCE		$(TGT_DIR)/src/os
	MODULES		objInfo.o 
	REQUIRES	INCLUDE_OBJ_LIB				\
			INCLUDE_CLASS_LIST
	INIT_RTN	objInfoInit ();
	PROTOTYPE       STATUS objInfoInit (void);
}

Component INCLUDE_OBJ_OWNERSHIP {
        NAME            object management ownership
	MODULES		objOwnerLib.o
        INIT_RTN        objOwnershipInit ();
        HDR_FILES       objLib.h
        INCLUDE_WHEN    INCLUDE_RTP
	REQUIRES	INCLUDE_OBJ_LIB
}

Component INCLUDE_CLASS_LIB {
	NAME		class management
	SOURCE		$(TGT_DIR)/src/os
	SYNOPSIS	object class management
	MODULES		classLib.o           \
			handleLib.o
	INIT_RTN	classLibInit ();
	HDR_FILES	classLib.h
	REQUIRES	INCLUDE_KERNEL       \
			INCLUDE_SEM_MUTEX    \
			SELECT_MEM_ALLOC
}

Component INCLUDE_CLASS_LIST {
	NAME		class list management
	SOURCE		$(TGT_DIR)/src/os
	SYNOPSIS	object class list management
	MODULES		classListLib.o
	INIT_RTN	classListLibInit ();
	REQUIRES	INCLUDE_CLASS_LIB
}

Component INCLUDE_VXEVENTS {
	NAME		VxWorks events
	SOURCE		$(TGT_DIR)/src/wind
	MODULES		eventLib.o 
	INIT_RTN	eventLibInit (VXEVENTS_OPTIONS);
	REQUIRES	INCLUDE_KERNEL \
			INCLUDE_Q_FIFO 
	HDR_FILES	eventLib.h
	CFG_PARAMS	VXEVENTS_OPTIONS
}

Parameter VXEVENTS_OPTIONS {
	NAME		VxWorks events configuration options
	SYNOPSIS	VxWorks events configuration options. For the \
			list of supported options and more detailed \
			description see the eventLib reference guide.
	TYPE		UINT
	DEFAULT		EVENTS_NONINTERRUPTIBLE
}

Component INCLUDE_MSG_Q {
	NAME		message queues
	SOURCE		$(TGT_DIR)/src/wind
	MODULES		msgQLib.o
	INIT_RTN	msgQLibInit ();
	HDR_FILES	msgQLib.h
	REQUIRES	INCLUDE_KERNEL       \
			INCLUDE_Q_FIFO       \
			INCLUDE_Q_PRI_LIST   \
			INCLUDE_SLL          \
			INCLUDE_TASK_CREATE_HOOKS
}

Component INCLUDE_MSG_Q_CREATE_DELETE {
	NAME		message queue creation and deletion library
	SOURCE		$(TGT_DIR)/src/wind
	MODULES		msgQCreateLib.o
	REQUIRES	INCLUDE_MSG_Q \
			INCLUDE_OBJ_LIB
	INIT_RTN	msgQCreateLibInit ();
	HDR_FILES	msgQLib.h
}

Component INCLUDE_MSG_Q_EVENTS {
	NAME		send/receive events from message queue operations
	SOURCE		$(TGT_DIR)/src/wind
	MODULES		msgQEvLib.o
	LINK_SYMS	msgQEvStart
	INCLUDE_WHEN	INCLUDE_VXEVENTS \
			INCLUDE_MSG_Q
	REQUIRES	INCLUDE_VXEVENTS \
			INCLUDE_MSG_Q
	_CHILDREN	FOLDER_NOT_VISIBLE
}

Component INCLUDE_MSG_Q_INFO {
	NAME		message queue info routines
	SOURCE		$(TGT_DIR)/src/wind
	MODULES		msgQInfo.o
	REQUIRES	INCLUDE_MSG_Q 
}

Component INCLUDE_SEM_LIB {
	NAME		semaphore support infrastructure
	SOURCE		$(TGT_DIR)/src/wind
	MODULES		semLib.o 
	INIT_RTN	semLibInit ();
	HDR_FILES	semLib.h
	CONFIGLETTES	usrSemLib.c
	REQUIRES	INCLUDE_KERNEL \
			INCLUDE_Q_FIFO 
	_CHILDREN	FOLDER_NOT_VISIBLE
}

Component INCLUDE_SEM_DELETE {
	NAME		semaphore deletion routines
	SOURCE		$(TGT_DIR)/src/wind
	MODULES		semDeleteLib.o 
	INIT_RTN	semDeleteLibInit ();
	HDR_FILES	semLib.h
	REQUIRES	INCLUDE_SEM_LIB \
			INCLUDE_OBJ_LIB
}

Component INCLUDE_SEM_BINARY {
	NAME		binary semaphores
	SOURCE		$(TGT_DIR)/src/wind
	MODULES		semBLib.o
	HDR_FILES	semLib.h
	REQUIRES	INCLUDE_SEM_LIB
}

Component INCLUDE_SEM_MUTEX {
	NAME		mutex semaphores
	SOURCE		$(TGT_DIR)/src/wind
	MODULES		semMLib.o
	HDR_FILES	semLib.h
	REQUIRES	INCLUDE_SEM_LIB
}

Component INCLUDE_SEM_COUNTING {
	NAME		counting semaphores
	SOURCE		$(TGT_DIR)/src/wind
	MODULES		semCLib.o
	HDR_FILES	semLib.h
	REQUIRES	INCLUDE_SEM_LIB
}

Component INCLUDE_SEM_READ_WRITE {
	NAME		reader/writer semaphores
	SOURCE		$(TGT_DIR)/src/wind
	MODULES		semRWLib.o windSemRWLib.o
	HDR_FILES	semLib.h
	REQUIRES	INCLUDE_SEM_LIB
	CFG_PARAMS	SEM_RW_MAX_CONCURRENT_READERS
}

Component INCLUDE_SEM_BINARY_CREATE {
	NAME		binary semaphore creation routine
	SOURCE		$(TGT_DIR)/src/wind
	MODULES		semBCreateLib.o
	HDR_FILES	semLib.h
	REQUIRES	INCLUDE_SEM_BINARY \
			INCLUDE_OBJ_LIB
	PROTOTYPE       void semBCreateLibInit (void);
}

Component INCLUDE_SEM_COUNTING_CREATE {
	NAME		counting semaphore creation routine
	SOURCE		$(TGT_DIR)/src/wind
	MODULES		semCCreateLib.o
	HDR_FILES	semLib.h
	REQUIRES	INCLUDE_SEM_COUNTING \
			INCLUDE_OBJ_LIB
	PROTOTYPE       void semCCreateLibInit (void);
}

Component INCLUDE_SEM_READ_WRITE_CREATE {
	NAME		reader/writer semaphore creation routine
	SOURCE		$(TGT_DIR)/src/wind
	MODULES		semRWCreateLib.o
	HDR_FILES	semLib.h
	REQUIRES	INCLUDE_SEM_READ_WRITE \
			INCLUDE_OBJ_LIB
}

Component INCLUDE_SEM_MUTEX_CREATE {
	NAME		mutex semaphore creation routine
	SOURCE		$(TGT_DIR)/src/wind
	MODULES		semMCreateLib.o
	HDR_FILES	semLib.h
	REQUIRES	INCLUDE_SEM_MUTEX \
			INCLUDE_OBJ_LIB
	PROTOTYPE       void semMCreateLibInit (void);
}

Component INCLUDE_SEM_EVENTS {
	NAME		send/receive events from semaphore operations
	SOURCE		$(TGT_DIR)/src/wind
	MODULES		semEvLib.o
	LINK_SYMS	semEvStart
	INCLUDE_WHEN	INCLUDE_VXEVENTS \
			INCLUDE_SEM_LIB
	REQUIRES	INCLUDE_VXEVENTS \
			INCLUDE_SEM_LIB
	_CHILDREN	FOLDER_NOT_VISIBLE
}

Component INCLUDE_SEM_INFO {
	NAME		semaphore info routines
	SOURCE		$(TGT_DIR)/src/wind
	MODULES		semInfo.o
	LINK_SYMS	semInfo
	REQUIRES	INCLUDE_SEM_LIB
}

Component INCLUDE_SEM_EXCHANGE {
	NAME		semaphore exchange routine
	SOURCE		$(TGT_DIR)/src/wind
	MODULES		semExchange.o
	HDR_FILES	semLib.h
	LINK_SYMS	semExchange
	REQUIRES	INCLUDE_SEM_LIB
	INCLUDE_WHEN	INCLUDE_RTP
}

Component INCLUDE_TASK_INFO {
	NAME		task info routines
	SOURCE		$(TGT_DIR)/src/wind
	MODULES		taskInfo.o
	LINK_SYMS	taskOptionsGet
	REQUIRES	INCLUDE_KERNEL
}

Component INCLUDE_TASK_LIST {
	NAME		task list management
	SYNOPSIS	task utility routines that depend on lookup in a task \
			list, taskIdListGet() and taskNameToId()
	SOURCE		$(TGT_DIR)/src/wind
	MODULES		taskListLib.o
	LINK_SYMS	taskIdListGet
	REQUIRES	INCLUDE_KERNEL
}

Component INCLUDE_TASK_HOOKS {
	NAME		task hooks
	SYNOPSIS	user callouts on task creation/deletion/context switch
	HDR_FILES	taskHookLib.h
	REQUIRES	INCLUDE_KERNEL \
			INCLUDE_HOOKS  \
			INCLUDE_TASK_CREATE_HOOKS \
			INCLUDE_TASK_SWAP_HOOKS   \
			INCLUDE_TASK_SWITCH_HOOKS
}

Component INCLUDE_TASK_CREATE_HOOKS {
	NAME		task create hooks
	SYNOPSIS	user callouts on task creation/deletion
	SOURCE		$(TGT_DIR)/src/os
	MODULES		taskCreateHookLib.o
	INIT_RTN	taskCreateHookInit ();
	HDR_FILES	taskHookLib.h
	REQUIRES	INCLUDE_KERNEL \
			INCLUDE_HOOKS
	CFG_PARAMS	VX_MAX_TASK_CREATE_RTNS
}

Parameter VX_MAX_TASK_CREATE_RTNS {
	NAME		max task create/delete hooks
	SYNOPSIS	Maximum size of the task create/delete hook tables
	TYPE		UINT
	DEFAULT		20
}

Component INCLUDE_TASK_SWAP_HOOKS {
	NAME		task swap hooks
	SYNOPSIS	user callouts on task context switch
	SOURCE		$(TGT_DIR)/src/os
	MODULES		taskSwapHookLib.o
	LINK_SYMS	taskSwapHookAdd
	HDR_FILES	taskHookLib.h
	REQUIRES	INCLUDE_KERNEL \
			INCLUDE_HOOKS
	INCLUDE_WHEN	INCLUDE_TASK_HOOKS
}

Component INCLUDE_TASK_SWITCH_HOOKS {
	NAME		task switch hooks
	SYNOPSIS	user callouts on task context switch
	SOURCE		$(TGT_DIR)/src/os
	MODULES		taskSwitchHookLib.o
	LINK_SYMS	taskSwitchHookAdd
	HDR_FILES	taskHookLib.h
	REQUIRES	INCLUDE_KERNEL \
			INCLUDE_HOOKS
	INCLUDE_WHEN	INCLUDE_TASK_HOOKS
}

Component INCLUDE_TASK_ROTATE {
	NAME		task rotate routines
	SYNOPSIS	rotate the task ready queue for a given priority
	SOURCE		$(TGT_DIR)/src/wind
	MODULES		taskRotate.o windRotate.o
	LINK_SYMS	taskRotate windRotate
	HDR_FILES	taskLib.h
	REQUIRES	INCLUDE_VX_TRADITIONAL_SCHEDULER
}

#ifdef _WRS_CONFIG_SMP
Component INCLUDE_TASK_TOOLS_UTIL {
	NAME		task tools utility routines
	SOURCE		$(TGT_DIR)/src/wind
	MODULES		taskToolsUtilLib.o
	HDR_FILES	private/taskToolsUtilLibP.h
	LINK_SYMS	taskInfoByCpuGet
	REQUIRES	INCLUDE_KERNEL
	_CHILDREN	FOLDER_NOT_VISIBLE
}
#endif

Component INCLUDE_TASK_UTIL {
	NAME		task utility routines
	SYNOPSIS	programmatic interface for modifying task information
	SOURCE		$(TGT_DIR)/src/wind
	MODULES		taskUtilLib.o
	LINK_SYMS	taskSpareNumAllot
	HDR_FILES	taskUtilLib.h
	REQUIRES	INCLUDE_KERNEL
}

#if (!defined _WRS_CONFIG_SMP) && (!defined _WRS_CONFIG_LP64)
Component INCLUDE_TASK_VARS {
	NAME		task variables
	SYNOPSIS	allow global variables to be made private to a task
	SOURCE		$(TGT_DIR)/src/os
	MODULES		taskVarLib.o
	INIT_RTN	taskVarInit ();
	HDR_FILES	taskVarLib.h
	REQUIRES	INCLUDE_KERNEL            \
			INCLUDE_SEM_DELETE        \
			INCLUDE_SEM_MUTEX         \
			INCLUDE_TASK_CREATE_HOOKS \
			INCLUDE_TASK_SWITCH_HOOKS \
			INCLUDE_MEM_MGR_BASIC
	_CHILDREN	FOLDER_KERNEL
	_DEFAULTS	FOLDER_KERNEL
}
#endif	/* !_WRS_CONFIG_SMP && !_WRS_CONFIG_LP64 */

Component INCLUDE_WATCHDOGS {
	NAME		watchdog timers
	SOURCE		$(TGT_DIR)/src/wind
	MODULES		wdLib.o
	INIT_RTN	wdLibInit ();
	HDR_FILES	wdLib.h
	REQUIRES 	INCLUDE_KERNEL
}

Component INCLUDE_WATCHDOGS_CREATE_DELETE {
	NAME		watchdog timers creation and deletion library
	SOURCE		$(TGT_DIR)/src/wind
	MODULES		wdCreateLib.o
	REQUIRES	INCLUDE_WATCHDOGS INCLUDE_OBJ_LIB
	INIT_RTN	wdCreateLibInit ();
	HDR_FILES	wdLib.h
}

Component INCLUDE_HOOKS {
	NAME		hook function table support
	SYNOPSIS	allow adding/removing of hook functions
	SOURCE		$(TGT_DIR)/src/util
	MODULES		hookLib.o
	HDR_FILES	hookLib.h
	LINK_SYMS	hookAddToTail
}

Component INCLUDE_HOOK_SHOW {
	NAME		hook function table show support
	SYNOPSIS	show component for hook function tables
	SOURCE		$(TGT_DIR)/src/util
	MODULES		hookShow.o
	REQUIRES	INCLUDE_HOOKS
	LINK_SYMS	hookShow
	INCLUDE_WHEN	INCLUDE_HOOKS \
			INCLUDE_SHOW_ROUTINES
}

Selection SELECT_MEM_ALLOC {
        NAME            memory allocator selection
        SYNOPSIS        memory allocator selection
        COUNT           1-1
        CHILDREN        INCLUDE_MEM_ALLOT \
                        INCLUDE_MEM_MGR_BASIC
        DEFAULTS        INCLUDE_MEM_ALLOT
}

Component INCLUDE_MEM_ALLOT {
	NAME		memory allot allocator
	SYNOPSIS	Primitive memory allocator - without free
	SOURCE		$(TGT_DIR)/src/os/mm 
	MODULES		memAllotLib.o
	INIT_RTN	memAllotLibInit (pMemPoolStart, memPoolSize);
	REQUIRES	INCLUDE_KERNEL
}

Component INCLUDE_MEM_MGR_BASIC {
	NAME		basic memory allocator
	SYNOPSIS	core memory partition manager
	SOURCE		$(TGT_DIR)/src/os/mm
	MODULES		memPartLib.o
	INIT_RTN	memPartLibInit (pMemPoolStart, memPoolSize);
	HDR_FILES	memLib.h
#ifndef _WRS_CONFIG_LP64
	REQUIRES	INCLUDE_SLL		\
			INCLUDE_AVL_UINT_TREES	\
			INCLUDE_SEM_MUTEX	\
			INCLUDE_SEM_DELETE	\
			INCLUDE_OBJ_LIB		\
			INCLUDE_ANSI_STRCPY	\
			INCLUDE_ANSI_STRLEN	\
			INCLUDE_FORMATTED_OUT_BASIC
#else
        REQUIRES        INCLUDE_SLL             \
                        INCLUDE_AVL_UINT_TREES  \
                        INCLUDE_SEM_MUTEX       \
                        INCLUDE_SEM_DELETE      \
                        INCLUDE_OBJ_LIB         \
                        INCLUDE_ANSI_STRCPY     \
                        INCLUDE_ANSI_STRLEN     \
                        INCLUDE_KERNEL_PROXIMITY_HEAP \
                        INCLUDE_KERNEL_COMMON_HEAP    \
                        INCLUDE_FORMATTED_OUT_BASIC
#endif
}

Component INCLUDE_MEM_MGR_FULL {
	NAME		full featured memory allocator
	SOURCE		$(TGT_DIR)/src/os/mm
	MODULES		memLib.o
	INIT_RTN	memInit (pMemPoolStart, memPoolSize, \
				 MEM_PART_DEFAULT_OPTIONS);
	REQUIRES	INCLUDE_MEM_MGR_BASIC
	CFG_PARAMS	MEM_PART_DEFAULT_OPTIONS
}

Parameter MEM_PART_DEFAULT_OPTIONS {
	NAME		default memory partition options
	SYNOPSIS	default memory partition options when the full featured \
			memory allocator is present
	TYPE		UINT
	DEFAULT		(MEM_ALLOC_ERROR_LOG_FLAG      | \
			 MEM_ALLOC_ERROR_EDR_WARN_FLAG | \
                         MEM_BLOCK_ERROR_LOG_FLAG      | \
                         MEM_BLOCK_ERROR_EDR_WARN_FLAG | \
                         MEM_BLOCK_ERROR_SUSPEND_FLAG  | \
			 MEM_BLOCK_CHECK)
}

Component INCLUDE_MEM_MGR_INFO {
	NAME		memory allocator info routines
	SOURCE		$(TGT_DIR)/src/os/mm
	MODULES		memInfo.o
	INIT_RTN	memInfoInit ();
	REQUIRES	INCLUDE_MEM_MGR_BASIC
}

#ifdef _WRS_CONFIG_LP64
Component INCLUDE_KERNEL_PROXIMITY_HEAP {
        NAME            Kernel proximity heap for 64bit systems.
        SOURCE          $(TGT_DIR)/src/os/mm
        MODULES         memLib.o
        INIT_RTN        kProxHeapInit (pMemPoolStart, memPoolSize);
        REQUIRES        INCLUDE_MEM_MGR_BASIC
	CFG_PARAMS	KERNEL_PROXIMITY_HEAP_SIZE
}

Parameter KERNEL_PROXIMITY_HEAP_SIZE {
        NAME            kernel proximity heap size
        SYNOPSIS        kernel proximity heap size
        TYPE            ULONG
        DEFAULT         0x10000000
}

Component INCLUDE_KERNEL_COMMON_HEAP {
        NAME            kernel common heap for 64bit systems.
        SOURCE          $(TGT_DIR)/src/os/mm
        MODULES         memLib.o
        INIT_RTN        kCommonHeapInit (KERNEL_COMMON_HEAP_INIT_SIZE, KERNEL_COMMON_HEAP_INCR_SIZE);
        REQUIRES        INCLUDE_MEM_MGR_BASIC
        CFG_PARAMS      KERNEL_COMMON_HEAP_INIT_SIZE \
                        KERNEL_COMMON_HEAP_INCR_SIZE
}

Parameter KERNEL_COMMON_HEAP_INIT_SIZE {
        NAME            kernel common heap initial size 
        SYNOPSIS        kernel common heap initial size
        TYPE            ULONG
        DEFAULT         0x20000000
}

Parameter KERNEL_COMMON_HEAP_INCR_SIZE {
        NAME            kernel common heap increment size
        SYNOPSIS        kernel common heap increment size. size = 0\
                        implies no autogrowth.
        TYPE            ULONG
        DEFAULT         0x200000
}

#endif

Component INCLUDE_SIGNALS {
	NAME		signals
	SOURCE		$(TGT_DIR)/src/os
	MODULES		sigLib.o
	INIT_RTN	sigInit (POSIX_SIGNAL_MODE);
	HDR_FILES	sigLib.h
	REQUIRES	INCLUDE_KERNEL                 \
			INCLUDE_EXC_TASK               \
			INCLUDE_Q_FIFO                 \
			INCLUDE_TASK_CREATE_HOOKS      \
			INCLUDE_MEM_MGR_BASIC	       \
			INCLUDE_SIGNAL_ARCH            \
			INCLUDE_ANSI_LONGJMP
 	CFG_PARAMS	POSIX_SIGNAL_MODE
}

Parameter POSIX_SIGNAL_MODE {
	NAME		POSIX signal mode
	SYNOPSIS	If TRUE then signals sent on fault are POSIX conformant
	TYPE		BOOL
	DEFAULT		(INCLUDE_RTP_POSIX_PSE52)::(TRUE) FALSE
}

Component INCLUDE_EDR_PM {
	NAME		persistent memory
	SOURCE		$(TGT_DIR)/src/os
	SYNOPSIS	reboot-safe protected memory region manager
	MODULES		pmLib.o
	CONFIGLETTES	usrPmInit.c
	INIT_RTN	usrPmInit ();
	HDR_FILES	pmLib.h
	CFG_PARAMS	EDR_PM_ARENA \
			PM_RESERVED_MEM
	REQUIRES	INCLUDE_KERNEL          \
			INCLUDE_CHECKSUM        \
			INCLUDE_ANSI_STRING     \
			INCLUDE_FORMATTED_OUT_BASIC
}

Component INCLUDE_EDR_ERRLOG {
	NAME		persistent error log
	SOURCE		$(TGT_DIR)/src/os
	SYNOPSIS	reboot-safe protected error log
	MODULES		edrLib.o
	CONFIGLETTES	usrEdrInit.c
	INIT_RTN	usrEdrInit ();
	HDR_FILES	edrLib.h
	CFG_PARAMS	EDR_ERRLOG_SIZE \
			EDR_RECORD_SIZE
	REQUIRES	INCLUDE_KERNEL          \
			INCLUDE_EDR_PM          \
			INCLUDE_CPLUS_DEMANGLER \
			INCLUDE_DLL             \
			INCLUDE_HOOKS           \
			INCLUDE_ANSI_STRING     \
			INCLUDE_ANSI_STRLEN     \
			INCLUDE_SEM_MUTEX       \
			INCLUDE_FORMATTED_OUT_BASIC
}

Component INCLUDE_SYSDBG_FLAG {

	NAME		system debug flag
	SYNOPSIS	a flag indicating the system is in 'debug' mode
	SOURCE		$(TGT_DIR)/src/debug
	MODULES		sysDbgLib.o
	INIT_RTN	sysDebugModeInit ();
	HDR_FILES	private/sysDbgLibP.h
}

Component INCLUDE_EDR_SYSDBG_FLAG {
	NAME		ED&R system debug flag
	SYNOPSIS	a flag indicating the ED&R system is in 'debug' mode
	SOURCE		$(TGT_DIR)/src/os
	MODULES		edrSysDbgLib.o
	HDR_FILES	edrLib.h
	REQUIRES	INCLUDE_SYSDBG_FLAG
}

Component INCLUDE_SC_POSIX {
	NAME		POSIX system call handlers
	SYNOPSIS	system call handlers for POSIX based calls
	PROTOTYPE       extern void usrPosixScInit (void);
	CONFIGLETTES    usrPosixSc.c
	INIT_RTN	usrPosixScInit ();
	INCLUDE_WHEN	INCLUDE_RTP 
	REQUIRES	INCLUDE_RTP
	_CHILDREN	FOLDER_NOT_VISIBLE
}

Component INCLUDE_PX_SCHED_DEF_POLICIES {
        NAME            POSIX scheduling policies SCHED_FIFO/SCHED_RR/SCHED_OTHER support in RTPs
        SYNOPSIS        Provide support for POSIX SCHED_FIFO, SCHED_RR, \
			and SCHED_OTHER policies in RTPs. \
			Selecting this component will \
			 drag in INCLUDE_POSIX_PTHREAD_SCHEDULER component.
        MODULES         qPxPriBMapLib.o
        REQUIRES        INCLUDE_POSIX_PTHREAD_SCHEDULER
        }

Component INCLUDE_PX_SCHED_SPORADIC_POLICY {
        NAME            POSIX scheduling support including SCHED_SPORADIC policy in RTPs
        SYNOPSIS        Provide support for POSIX SCHED_FIFO, SCHED_RR, \
			SCHED_OTHER, and SCHED_SPORADIC policies in RTPs. \
			Selecting this component \
			  will drag in INCLUDE_POSIX_PTHREAD_SCHEDULER component.
        MODULES         qPxSSPriBMapLib.o
        INIT_RTN        pxSSThreadLibInit ();
        HDR_FILES       private/taskPxLibP.h 
        REQUIRES        INCLUDE_POSIX_PTHREAD_SCHEDULER
        }

Selection SELECT_PX_SCHED_POLICIES {
        NAME            POSIX scheduling policy selection 
        SYNOPSIS        select POSIX scheduling policy support in RTPs
        COUNT           1-1
        CHILDREN        INCLUDE_PX_SCHED_DEF_POLICIES \
                        INCLUDE_PX_SCHED_SPORADIC_POLICY
        DEFAULTS        INCLUDE_PX_SCHED_DEF_POLICIES
	_CHILDREN	FOLDER_POSIX
	}

Component INCLUDE_POSIX_PTHREAD_SCHEDULER {
        NAME            POSIX thread scheduler in RTPs
        SYNOPSIS        POSIX thread scheduling behavior in RTPs
        MODULES         taskPxLib.o 
        INIT_RTN        taskPxLibInit ();
        EXCLUDES	INCLUDE_TASK_ROTATE
        HDR_FILES       private/taskPxLibP.h
        CFG_PARAMS      POSIX_PTHREAD_RR_TIMESLICE
	REQUIRES        INCLUDE_RTP \
			INCLUDE_Q_PRI_BMAP \
			SELECT_PX_SCHED_POLICIES
        }

Component INCLUDE_CUSTOM_SCHEDULER {
        NAME            user-specified kernel scheduler
        SYNOPSIS        user-specified kernel scheduler
        CONFIGLETTES    usrCustomScheduler.c
        INIT_RTN        usrCustomSchedulerInit ();
        EXCLUDES	INCLUDE_TASK_ROTATE
	}

Component INCLUDE_VX_NATIVE_SCHEDULER {
        NAME            VxWorks traditional scheduler (obsoleted - replaced by INCLUDE_VX_TRADITIONAL_SCHEDULER)
        SYNOPSIS        (Obsoleted) This component is replaced by INCLUDE_VX_TRADITIONAL_SCHEDULER.
        CFG_PARAMS      VX_NATIVE_SCHED_CONSTANT_RDY_Q
	REQUIRES	INCLUDE_VX_TRADITIONAL_SCHEDULER
        }

Component INCLUDE_VX_TRADITIONAL_SCHEDULER {
        NAME            VxWorks traditional kernel scheduler
        SYNOPSIS        use VxWorks traditional kernel scheduler
        CFG_PARAMS      VX_TRAD_SCHED_CONSTANT_RDY_Q
	REQUIRES	INCLUDE_Q_PRI_BMAP
        }

Selection SELECT_KERNEL_SCHEDULER {
        NAME            kernel scheduler selection
        SYNOPSIS        select a kernel scheduler
        COUNT           1-1
        CHILDREN        INCLUDE_VX_TRADITIONAL_SCHEDULER \
                        INCLUDE_POSIX_PTHREAD_SCHEDULER \
                        INCLUDE_CUSTOM_SCHEDULER
        DEFAULTS        INCLUDE_VX_TRADITIONAL_SCHEDULER
}

Parameter VX_NATIVE_SCHED_CONSTANT_RDY_Q {
        NAME            constant-time kernel ready queue (obsoleted)
        SYNOPSIS        (Obsoleted) This parameter is replaced by VX_TRAD_SCHED_CONSTANT_RDY_Q.
        TYPE            BOOL
        DEFAULT         VX_TRAD_SCHED_CONSTANT_RDY_Q
}

Parameter VX_TRAD_SCHED_CONSTANT_RDY_Q {
        NAME            constant-time kernel ready queue
        TYPE            BOOL
        DEFAULT         TRUE
}

Parameter POSIX_PTHREAD_RR_TIMESLICE {
        NAME            user POSIX thread time slice in ticks
        TYPE            uint
        DEFAULT         10
}

Component INCLUDE_KERNEL_HARDENING {
	NAME		kernel hardening features
	SYNOPSIS	applies protection to certain kernel components (text segment write protection, write protection of the exception vector table, task stack overflow and underflow detection, non-executable task stacks, NULL pointer dereference detection for kernel tasks)
	_CHILDREN	FOLDER_KERNEL
	REQUIRES	INCLUDE_PROTECT_TEXT \
			INCLUDE_PROTECT_VEC_TABLE \
			INCLUDE_PROTECT_TASK_STACK \
			INCLUDE_TASK_STACK_NO_EXEC
}

Folder	FOLDER_IO_SYSTEM {
	NAME		IO system components
	CHILDREN	INCLUDE_IO_SYSTEM	\
			FOLDER_IO_SUBSYSTEM	\
			FOLDER_HRFS             \
			INCLUDE_STDIO		\
			INCLUDE_FLOATING_POINT	\
			INCLUDE_FORMATTED_IO	\
			INCLUDE_FORMATTED_OUT_BASIC	\
			INCLUDE_LOGGING		\
			INCLUDE_PIPES		\
			INCLUDE_SELECT		\
                        INCLUDE_ERF             \
                        INCLUDE_FS_EVENT_UTIL   \
                        INCLUDE_FS_MONITOR      \
                        INCLUDE_DEVICE_MANAGER  \
                        INCLUDE_XBD             \
                        INCLUDE_XBD_BLK_DEV     \
                        INCLUDE_XBD_RAMDRV      \
                        INCLUDE_XBD_PART_LIB    \
			INCLUDE_CDROMFS		\
			INCLUDE_DOSFS		\
			INCLUDE_DISK_UTIL	\
			INCLUDE_RAWFS		\
			INCLUDE_RAM_DISK        \
			INCLUDE_RAMDRV		\
			INCLUDE_MEMDRV		\
			INCLUDE_PTYDRV		\
			INCLUDE_TYLIB		\
			INCLUDE_ISR_DEFER	\
			INCLUDE_ROMFS		\
			INCLUDE_VRFS		\
                        INCLUDE_UTF             \
			INCLUDE_TTY_DEV
	DEFAULTS	INCLUDE_IO_SYSTEM		\
			INCLUDE_STDIO			\
			INCLUDE_FLOATING_POINT		\
			INCLUDE_FORMATTED_IO		\
			INCLUDE_FORMATTED_OUT_BASIC	\
			INCLUDE_LOGGING			\
			INCLUDE_PIPES			\
			INCLUDE_SELECT			\
			INCLUDE_TTY_DEV
}

Component INCLUDE_TTY_DEV {
	NAME		terminal driver
	SOURCE		$(TGT_DIR)/src/os/io
	MODULES		ttyDrv.o
	INIT_RTN	ttyDrv ();
	REQUIRES	INCLUDE_IO_BASIC        \
			INCLUDE_TYLIB           \
			INCLUDE_MEM_MGR_BASIC
	HDR_FILES	ttyLib.h
}

Component INCLUDE_UTF {
        NAME            Unicode Character Set
        SYNOPSIS        Unicode Character Set
        MODULES         utfLib.o
        HDR_FILES       utfLib.h
        INIT_RTN        utfLibInit();
}

Component INCLUDE_CDROMFS {
	NAME		CDROM (ISO9960 High Sierra) filesystem
	SYNOPSIS	CDROM (ISO9960 High Sierra) filesystem
	MODULES		cdromFsLib.o cdromFsFsmLib.o
	INIT_RTN	cdromFsInit (CDROM_COM_BUF_SIZE);
	HDR_FILES	cdromFsLib.h
        CFG_PARAMS      CDROM_COM_BUF_SIZE
	REQUIRES	INCLUDE_IO_FILE_SYSTEM
}

Parameter CDROM_COM_BUF_SIZE {
    NAME        CDROM Common Buffer Size
    SYNOPSIS    CDROM Common Buffer Size (Minimum value is 3)
    TYPE        UINT32
    DEFAULT     3
    }

Component INCLUDE_FP_TYPE {
	NAME		floating point type
	SYNOPSIS	type of floating point
	SOURCE		$(TGT_DIR)/src/math
	MODULES		fpType.o
	_CHILDREN	FOLDER_NOT_VISIBLE
}
	
Component INCLUDE_FLOATING_POINT {
	NAME		fpp formatting for printf
	SYNOPSIS	allow printf and others to format floats correctly
	SOURCE		$(TGT_DIR)/src/math
	MODULES		floatLib.o
	INIT_RTN	floatInit ();
	HDR_FILES	math.h
	REQUIRES	INCLUDE_FORMATTED_IO \
			INCLUDE_ANSI_STRCPY  \
			INCLUDE_ANSI_MATH    \
			INCLUDE_FP_TYPE
}

Component INCLUDE_FORMATTED_IO {
	NAME		formatted IO
	SYNOPSIS	formatting for printf, scanf, etc.
	SOURCE		$(TGT_DIR)/src/math
	REQUIRES	INCLUDE_FORMATTED_OUT_BASIC \
			INCLUDE_ANSI_MEMCHR         \
			INCLUDE_IO_BASIC
	MODULES		fioLib.o
	INIT_RTN	fioLibInit ();
	HDR_FILES	fioLib.h
}

Component INCLUDE_FORMATTED_OUT_BASIC {
	NAME            formatted IO output routines
	SYNOPSIS        formatting for output routines - scaled down
	SOURCE          $(TGT_DIR)/src/math
	REQUIRES        INCLUDE_KERNEL		\
			INCLUDE_BLIB		\
			INCLUDE_ANSI_MEMCHR	\
			INCLUDE_ANSI_STRLEN
	MODULES         fioBaseLib.o
	INIT_RTN        fioBaseLibInit ();
	HDR_FILES       fioLib.h
}

Component INCLUDE_POSIX_FS {
	NAME		Posix APIs for file systems
	SOURCE		$(TGT_DIR)/src/posix
	MODULES		fsPxLib.o
	REQUIRES	INCLUDE_IO_SYSTEM
	INCLUDE_WHEN	INCLUDE_IO_SYSTEM
	_CHILDREN	FOLDER_NOT_VISIBLE
}

Parameter IOS_POSIX_PSE52_MODE {
        NAME            POSIX PSE52 mode for VxWorks I/O subsystem
        SYNOPSIS        Enables POSIX PSE52 behavior for the VxWorks I/O subsystem
        TYPE            BOOL
        DEFAULT         (INCLUDE_RTP_POSIX_PSE52)::(TRUE) FALSE
}

Component INCLUDE_IO_BASIC {
        NAME            Basic IO System 
	SYNOPSIS	Basic IO system component
        SOURCE          $(TGT_DIR)/src/os/io
	REQUIRES	INCLUDE_KERNEL           \
			INCLUDE_OBJ_LIB		 \
			INCLUDE_SEM_INFO	 \
			INCLUDE_TASK_INFO	 \
			INCLUDE_LSTLIB		 \
			INCLUDE_DLL              \
			INCLUDE_POOL		 \
			INCLUDE_SEM_MUTEX_CREATE \
			INCLUDE_ANSI_CTYPE	 \
			INCLUDE_ANSI_STRLEN	 \
			INCLUDE_ANSI_STRCAT	 \
			INCLUDE_ANSI_STRNCAT	 \
			INCLUDE_ANSI_STRNCMP     \
			INCLUDE_MEM_MGR_BASIC
        MODULES         ioLib.o iosLib.o iosDrvLib.o pathCatLib.o
	INIT_RTN	iosInit (NUM_DRIVERS, NUM_FILES, "/null");
	CFG_PARAMS	NUM_DRIVERS NUM_FILES
	HDR_FILES	iosLib.h
}

Component INCLUDE_IO_FILE_SYSTEM {
        NAME            File System IO
	SYNOPSIS	File System IO component
        SOURCE          $(TGT_DIR)/src/os/io
        REQUIRES        INCLUDE_IO_BASIC \
			INCLUDE_IO_REMOVABLE
	INCLUDE_WHEN	INCLUDE_IO_SYSTEM
        MODULES         fsIoLib.o iosPathLib.o pathLib.o
        INIT_RTN        iosPathLibInit ();
        HDR_FILES       iosLib.h
}

Component INCLUDE_IO_REMOVABLE {
        NAME            Removable IO
	SYNOPSIS	Removable IO component
        SOURCE          $(TGT_DIR)/src/os/io
	REQUIRES        INCLUDE_IO_BASIC
	INCLUDE_WHEN	INCLUDE_IO_SYSTEM
        MODULES         iosRmvLib.o
        INIT_RTN        iosRmvLibInit ();
}

Component INCLUDE_IO_POSIX {
        NAME            POSIX IO
	SYNOPSIS	POSIX IO component
        SOURCE          $(TGT_DIR)/src/os/io
        REQUIRES        INCLUDE_IO_BASIC       \
                        INCLUDE_IO_FILE_SYSTEM \
                        INCLUDE_IO_REMOVABLE
	INCLUDE_WHEN	INCLUDE_IO_SYSTEM
        MODULES         ioPxLib.o iosPxLib.o
        INIT_RTN        iosPxLibInit (IOS_POSIX_PSE52_MODE);
	CFG_PARAMS	IOS_POSIX_PSE52_MODE
        HDR_FILES       iosLib.h
}

Component INCLUDE_IO_RTP {
        NAME            RTP IO
	SYNOPSIS	RTP IO component
        SOURCE          $(TGT_DIR)/src/os/io
        REQUIRES        INCLUDE_IO_BASIC       \
                        INCLUDE_IO_FILE_SYSTEM \
                        INCLUDE_IO_REMOVABLE   \
                        INCLUDE_IO_POSIX
	INCLUDE_WHEN	INCLUDE_IO_SYSTEM
        MODULES         iosRtpLib.o
        INIT_RTN        iosRtpLibInit ();
        HDR_FILES       iosLib.h
}

Component INCLUDE_IO_MISC {
        NAME            Miscellaneous IO
	SYNOPSIS	Miscellaneous IO component
        SOURCE          $(TGT_DIR)/src/os/io
	INCLUDE_WHEN	INCLUDE_IO_SYSTEM
        MODULES         iosMiscLib.o
        HDR_FILES       iosLib.h
}

Component INCLUDE_IO_SYSTEM {
	NAME		IO system
	SYNOPSIS	IO system component
	SOURCE		$(TGT_DIR)/src/os/io
	REQUIRES	INCLUDE_IO_BASIC
}

Folder FOLDER_IO_SUBSYSTEM
	{
	NAME		IO Subsystem Components
	SYNOPSIS	IO System related components
	_CHILDREN	FOLDER_IO_SYSTEM
	CHILDREN	INCLUDE_IO_BASIC \
			INCLUDE_IO_FILE_SYSTEM \
			INCLUDE_IO_REMOVABLE \
			INCLUDE_IO_POSIX \
			INCLUDE_IO_RTP \
			INCLUDE_IO_MISC
	}

Component INCLUDE_LOGGING {
	NAME		message logging
	SOURCE		$(TGT_DIR)/src/os
	MODULES		logLib.o
	INIT_RTN	logInit (consoleFd, MAX_LOG_MSGS);
	CFG_PARAMS	MAX_LOG_MSGS
	HDR_FILES	logLib.h
	REQUIRES	INCLUDE_MSG_Q              \
			INCLUDE_ANSI_STDIO         \
			INCLUDE_SEM_MUTEX          \
			INCLUDE_ANSI_STRNCPY       \
			INCLUDE_TASK_CREATE_DELETE \
			INCLUDE_MSG_Q_CREATE_DELETE\
			INCLUDE_ATOMIC_OPERATORS
}

Component INCLUDE_PIPES {
	NAME		pipes
	SOURCE		$(TGT_DIR)/src/os/io
	MODULES		pipeDrv.o
	INIT_RTN	pipeDrv (PIPE_MAX_OPEN_FDS);
	CFG_PARAMS      PIPE_MAX_OPEN_FDS	
	HDR_FILES	pipeDrv.h
	REQUIRES        INCLUDE_KERNEL            \
			INCLUDE_MSG_Q_INFO        \
			INCLUDE_SEM_INFO          \
			INCLUDE_IO_BASIC          \
			INCLUDE_MEM_MGR_BASIC     \
			INCLUDE_SELECT            \
			INCLUDE_MSG_Q_CREATE_DELETE
}

Parameter PIPE_MAX_OPEN_FDS {
	NAME            maximum open fds for each pipe device 
        SYNOPSIS        maximum open fds for each pipe device
	TYPE            uint 
        DEFAULT         16 
}

Component INCLUDE_RAMDRV {
	NAME		RAM disk driver
	SYNOPSIS	allows a filesystem to be put on top of RAM
	MODULES		ramDrv.o
	INIT_RTN	ramDrv ();
	HDR_FILES	ramDrv.h
}

Component INCLUDE_MEMDRV {
	NAME		MEM disk driver
	SYNOPSIS	allows a filesystem to be put on top of memory
	MODULES		memDrv.o
	INIT_RTN	memDrv ();
	HDR_FILES	memDrv.h
}

Component INCLUDE_PTYDRV {
	NAME		Pseudo terminal driver
	SYNOPSIS	Allows communication between processes
	MODULES		ptyDrv.o
	INIT_RTN	ptyDrv ();
	HDR_FILES	ptyDrv.h
}

Component INCLUDE_TYLIB {
	NAME		terminal driver support
	SYNOPSIS	Allows communication between processes
	REQUIRES	INCLUDE_BLIB		\
			INCLUDE_SEM_BINARY        \
			INCLUDE_SEM_MUTEX         \
			INCLUDE_RNG_BUF
	SOURCE		$(TGT_DIR)/src/os/io
	MODULES		tyLib.o
	INIT_RTN	tyLibInit (TYLIB_XOFF_PCNT, TYLIB_XON_PCNT, \
				TYLIB_WRT_THRESHLD);
	HDR_FILES	tyLib.h
	CFG_PARAMS	TYLIB_XOFF_PCNT \
			TYLIB_XON_PCNT \
			TYLIB_WRT_THRESHLD
}

Parameter TYLIB_XOFF_PCNT {
	NAME		XOFF buffer threshold
	SYNOPSIS	Send XOFF when rcv buff exceeds this level (%).
	TYPE		uint
	DEFAULT		85
}

Parameter TYLIB_XON_PCNT {
	NAME		XON buffer threshold
	SYNOPSIS	Send XON when rcv buff drops below this level (%).
	TYPE		uint
	DEFAULT		50
}

Parameter TYLIB_WRT_THRESHLD {
	NAME		Write buffer threshold
	SYNOPSIS	Wakeup tty writer tasks when tx buffer has this much available space (chars).
	TYPE		uint
	DEFAULT		50
}

Component INCLUDE_ROMFS {
	NAME		ROMFS file system
	SYNOPSIS	Read-only memory based file system
	SOURCE		$(TGT_DIR)/src/fs/romfs
	CONFIGLETTES	usrRomfs.c
	MODULES		romfs.o romfsLib.o romfsDrv.o
	INIT_RTN	usrRomfsConfig ();
	HDR_FILES	romfsLib.h
	REQUIRES	INCLUDE_IO_FILE_SYSTEM    \
			INCLUDE_MEM_MGR_BASIC     \
			INCLUDE_SEM_MUTEX         \
			INCLUDE_FORMATTED_OUT_BASIC
}

Component INCLUDE_RAWFS {
	NAME		raw filesystem
	SYNOPSIS	Raw block device file system interface
	MODULES		rawFsLib.o
	INIT_RTN	rawFsInit (NUM_RAWFS_FILES);
	CFG_PARAMS	NUM_RAWFS_FILES
	HDR_FILES	rawFsLib.h
    	REQUIRES	INCLUDE_FS_MONITOR INCLUDE_FS_EVENT_UTIL
}
#ifndef _WRS_CONFIG_LP64
Component INCLUDE_PROCFS {
	NAME		PROCFS file system
	SYNOPSIS	process based file system
	CONFIGLETTES	usrProcfs.c
	MODULES		procfsLib.o procfs.o procfsIO.o
	INIT_RTN	usrProcfsConfig ();
	REQUIRES	INCLUDE_IO_FILE_SYSTEM
}
#endif
Component INCLUDE_ERF {
    NAME                Event Reporting Framework
    SYNOPSIS            Event Reporting Framework
    MODULES             erfLib.o
    HDR_FILES           drv/erf/erfLib.h
    INIT_RTN            erfLibInit (ERF_MAX_USR_CATEGORIES, ERF_MAX_USR_TYPES);
    CFG_PARAMS          ERF_MAX_USR_CATEGORIES     \
                        ERF_MAX_USR_TYPES
    }

Component INCLUDE_DEVICE_MANAGER {
    NAME                Device Manager
    SYNOPSIS            Device Manager
    MODULES             device.o
    HDR_FILES           drv/manager/device.h
    INIT_RTN            devInit (DEVICE_MANAGER_MAX_NUM_DEVICES);
    CFG_PARAMS          DEVICE_MANAGER_MAX_NUM_DEVICES
    INCLUDE_WHEN        INCLUDE_XBD
    }

/* Select is really one component with two different init routines */

Component INCLUDE_SELECT {
	NAME		select
	SOURCE		$(TGT_DIR)/src/os/io
	MODULES		selectLib.o
	INIT_RTN	selectInit (NUM_FILES);
	HDR_FILES	selectLib.h
	REQUIRES	INCLUDE_IO_BASIC	       \
			INCLUDE_SEM_BINARY	       \
			INCLUDE_SEM_MUTEX              \
			INCLUDE_EXC_TASK	       \
			INCLUDE_TASK_CREATE_HOOKS      \
			INCLUDE_LSTLIB                 \
			INCLUDE_MEM_MGR_BASIC          \
			INCLUDE_OBJ_LIB                \
			INCLUDE_RNG_BUF                \
			INCLUDE_SELECT_SUPPORT
	INCLUDE_WHEN	INCLUDE_IO_SYSTEM
	INIT_BEFORE	INCLUDE_SELECT_SUPPORT
	HELP		selectLib
}

/*
 * This is the second init routine for select. This is because
 * the select delete hook must be called before the RPC delete hook when
 * a task is deleted.
 * The task create hook is installed as part of selectInit() above.
 */

Component INCLUDE_SELECT_SUPPORT {
	NAME		selectInit, part 2, install task delete hook
	INIT_RTN	selTaskDeleteHookAdd ();
	REQUIRES	INCLUDE_SELECT
	HDR_FILES	selectLib.h
}

Component INCLUDE_STDIO {
	NAME		stdio
	SYNOPSIS	buffered IO library
	SOURCE		$(TGT_DIR)/src/libc/stdio
	MODULES		stdioLib.o
	INIT_RTN	stdioInit ();
	HDR_FILES	stdio.h
	REQUIRES	INCLUDE_ANSI_STDIO      \
			INCLUDE_MEM_MGR_BASIC   \
			INCLUDE_TASK_CREATE_HOOKS
}

Folder	FOLDER_UTILITIES {
	NAME		utility components
	CHILDREN	INCLUDE_HASH		\
			INCLUDE_DLL		\
			INCLUDE_RNG_BUF		\
			INCLUDE_LSTLIB		\
			INCLUDE_POOL            \
			INCLUDE_STRING_SEARCH  
}

Component INCLUDE_DISK_UTIL {
        NAME            File System and Disk Utilities
        SYNOPSIS        Target-resident File System utilities (cd, copy, ll, mkdir, etc)
	SOURCE		$(TGT_DIR)/src/usr
        MODULES         usrFsLib.o
        HDR_FILES       usrLib.h
	LINK_SYMS	ioHelp
	REQUIRES	INCLUDE_IO_FILE_SYSTEM  \
			INCLUDE_ANSI_STDIO      \
			INCLUDE_ANSI_STDLIB     \
			INCLUDE_ANSI_STRCMP     \
			INCLUDE_ANSI_STRLEN     \
			INCLUDE_ANSI_STRNCPY    \
			INCLUDE_ANSI_TIME       \
			INCLUDE_MEM_MGR_BASIC   \
			INCLUDE_POSIX_DIRLIB	\
			INCLUDE_FORMATTED_OUT_BASIC
        }

Component INCLUDE_HASH {
	NAME		hash library
	SOURCE		$(TGT_DIR)/src/os
	MODULES		hashLib.o
	INIT_RTN	hashLibInit ();
	HDR_FILES	hashLib.h
	REQUIRES	INCLUDE_MEM_MGR_BASIC \
			INCLUDE_SLL           \
			INCLUDE_ANSI_STRCMP
}

Component INCLUDE_DLL {
	NAME		doubly linked lists
	SOURCE		$(TGT_DIR)/src/util
	MODULES		dllLib.o
	LINK_SYMS	dllInit
}

Component INCLUDE_SLL {
	NAME		single linked lists
	SOURCE		$(TGT_DIR)/src/util
	MODULES		sllLib.o
	LINK_SYMS	sllInit
}

Component INCLUDE_LSTLIB {
	NAME		linked list library
	REQUIRES	INCLUDE_ANSI_ABS    \
			SELECT_MEM_ALLOC
	SOURCE		$(TGT_DIR)/src/util
	MODULES		lstLib.o
	LINK_SYMS	lstInit
	HDR_FILES	lstLib.h
}

Component INCLUDE_RNG_BUF {
	NAME		ring buffers
	SOURCE		$(TGT_DIR)/src/util
	MODULES		rngLib.o
	LINK_SYMS	rngCreate
	REQUIRES	INCLUDE_BLIB \
			SELECT_MEM_ALLOC
}

Component INCLUDE_STRING_SEARCH {
	NAME		string search library
        SYNOPSIS        Efficient string search utilities
	MODULES		memmem.o strSearchLib.o
	LINK_SYMS	memmem memimem fastStrSearch
}

Component INCLUDE_EXC_HANDLING {
	NAME		exception handling
	SOURCE		$(TGT_DIR)/src/arch/$(ARCH_DIR)
	MODULES		excArchLib.o
	INIT_RTN	excVecInit ();
	HDR_FILES	excLib.h
	REQUIRES	INCLUDE_KERNEL 
}

Component INCLUDE_EXC_SHOW {
	NAME		exception show routines
	SOURCE		$(TGT_DIR)/src/arch/$(ARCH_DIR)
	MODULES		excArchShow.o
	INIT_RTN	excShowInit ();
	HDR_FILES	excLib.h
	INCLUDE_WHEN	INCLUDE_EXC_HANDLING \
			INCLUDE_SHOW_ROUTINES
}

Component INCLUDE_VM_SHOW {
	NAME		VM library show routine
	MODULES		vmShow.o
	INIT_RTN	vmShowInit ();
	HDR_FILES	vmLib.h
	INCLUDE_WHEN	INCLUDE_MMU_BASIC \
			INCLUDE_SHOW_ROUTINES
}

Component INCLUDE_BOOT_LINE_INIT
        {
        NAME            read the bootline
        SYNOPSIS        parse some boot device configuration info 
	SOURCE		$(TGT_DIR)/config/comps/src
        CONFIGLETTES    net/coreip/usrBootLine.c
        INIT_RTN        usrBootLineParse (BOOT_LINE_ADRS);
	HDR_FILES	sysLib.h private/funcBindP.h
        }

Component INCLUDE_ATOMIC_OPERATORS {
	NAME		atomic operators support
	SYNOPSIS	Atomic operators, such as vxAtomicAdd() and vxAtomicInc() support
	SOURCE 		$(TGT_DIR)/src/os
	MODULES		vxAtomicLib.o			
	_CHILDREN	FOLDER_KERNEL
	LINK_SYMS	vxAtomicAdd
	HDR_FILES	vxAtomicLib.h
}

Folder	FOLDER_WINDVIEW {
	NAME		System Viewer components
	CHILDREN	INCLUDE_WINDVIEW 	\
			INCLUDE_WINDVIEW_CLASS 	\
                        INCLUDE_WVNETD          \
			SELECT_TIMESTAMP 	\
                        SELECT_WVUPLOAD 	\
                        SELECT_WV_BUFFER
	DEFAULTS	INCLUDE_WINDVIEW 	\
			INCLUDE_WINDVIEW_CLASS 	\
			SELECT_TIMESTAMP 	\
                        SELECT_WV_BUFFER	\
			SELECT_WVUPLOAD	
}

Selection	SELECT_WVUPLOAD {
	NAME		upload path(s)
	SYNOPSIS	choose among TSFS, NFS and/or TCP/IP
	COUNT		1-
	CHILDREN	INCLUDE_WVUPLOAD_FILE	\
			INCLUDE_WVUPLOAD_SOCK	\
			INCLUDE_WVUPLOAD_TSFSSOCK 
	DEFAULTS	INCLUDE_WVUPLOAD_FILE   \
	        	INCLUDE_WVUPLOAD_TSFSSOCK 
}

Component INCLUDE_WVUPLOAD_FILE {
	NAME		file upload path initialization 
	SYNOPSIS	initialize path for the upload to file 
	SOURCE		$(TGT_DIR)/src/event
	MODULES		wvFileUploadPathLib.o 
	INIT_RTN	wvFileUploadPathLibInit ();
	HDR_FILES	private/wvUploadPathP.h private/wvFileUploadPathLibP.h
	REQUIRES	INCLUDE_WINDVIEW
}

Component INCLUDE_WVUPLOAD_SOCK {
	NAME		TCP/IP socket upload path initialization 
	SYNOPSIS	initialize path for the upload through TCP/IP sockets 
	MODULES		wvSockUploadPathLib.o 
	INIT_RTN	wvSockUploadPathLibInit ();
	HDR_FILES	private/wvUploadPathP.h private/wvSockUploadPathLibP.h
	REQUIRES	INCLUDE_WINDVIEW
}

Component INCLUDE_WVUPLOAD_TSFSSOCK {
	NAME		TSFS upload path initialization 
	SYNOPSIS	initialize path for the upload through TSFS socket 
	SOURCE		$(TGT_DIR)/src/event
	MODULES		wvTsfsUploadPathLib.o 
	INIT_RTN	wvTsfsUploadPathLibInit ();
	HDR_FILES	private/wvUploadPathP.h private/wvTsfsUploadPathLibP.h
	REQUIRES	INCLUDE_WINDVIEW INCLUDE_WDB_TSFS
	HELP		tgtsvr wdbTsfsDrvDrv WDB
}

Selection	SELECT_TIMESTAMP {
	NAME		select timestamping
	COUNT		1-1
	CHILDREN	INCLUDE_SYS_TIMESTAMP	\
			INCLUDE_USER_TIMESTAMP	\
			INCLUDE_SEQ_TIMESTAMP
	DEFAULTS	INCLUDE_SEQ_TIMESTAMP
}

Component INCLUDE_SYS_TIMESTAMP {
	NAME		system-defined timestamping
	SYNOPSIS	bsp specific timestamp routines 
#ifdef _WRS_CONFIG_SV_INSTRUMENTATION
	INIT_RTN	wvTmrRegister ((UINTFUNCPTR) sysTimestamp,	\
				(UINTFUNCPTR) sysTimestampLock,	        \
				(FUNCPTR) sysTimestampEnable,	\
				(FUNCPTR) sysTimestampDisable,	\
				(FUNCPTR) sysTimestampConnect,	\
				(UINTFUNCPTR) sysTimestampPeriod,	\
				(UINTFUNCPTR) sysTimestampFreq);
#endif 
	HDR_FILES	wvTmrLib.h sysLib.h
	REQUIRES	INCLUDE_TIMESTAMP
}

Component INCLUDE_USER_TIMESTAMP {
	NAME		user-defined timestamping
	SYNOPSIS	user-defined timestamp routines
#ifdef _WRS_CONFIG_SV_INSTRUMENTATION
	INIT_RTN	wvTmrRegister ((UINTFUNCPTR) USER_TIMESTAMP,	\
				(UINTFUNCPTR) USER_TIMESTAMPLOCK,	\
				(FUNCPTR) USER_TIMEENABLE,		\
				(FUNCPTR) USER_TIMEDISABLE,		\
				(FUNCPTR) USER_TIMECONNECT,		\
				(UINTFUNCPTR) USER_TIMEPERIOD,		\
				(UINTFUNCPTR) USER_TIMEFREQ);
#endif
	CFG_PARAMS	USER_TIMESTAMP		\
			USER_TIMESTAMPLOCK	\
			USER_TIMEENABLE		\
			USER_TIMEDISABLE	\
			USER_TIMECONNECT	\
			USER_TIMEPERIOD		\
			USER_TIMEFREQ
	HDR_FILES	wvTmrLib.h
}

Component INCLUDE_SEQ_TIMESTAMP {
	NAME		sequential timestamping
	SYNOPSIS	no timestamping
	MODULES		seqDrv.o
#ifdef _WRS_CONFIG_SV_INSTRUMENTATION
	INIT_RTN	wvTmrRegister ((UINTFUNCPTR) seqStamp,	\
				(UINTFUNCPTR) seqStampLock,	\
				(FUNCPTR) seqEnable,	\
				(FUNCPTR) seqDisable,	\
				(FUNCPTR) seqConnect,	\
				(UINTFUNCPTR) seqPeriod,	\
				(UINTFUNCPTR) seqFreq);
#endif
	HDR_FILES	wvTmrLib.h private/seqDrvP.h
}

Component INCLUDE_WINDVIEW {
	NAME		System Viewer library
	SYNOPSIS	initialize and control event logging
	SOURCE		$(TGT_DIR)/src/event
	CONFIGLETTES	usrWindview.c
	MODULES		evtLogLib.o 
	INIT_RTN	windviewConfig ();
	CFG_PARAMS	WV_DEFAULT_BUF_MIN	\
			WV_DEFAULT_BUF_MAX	\
			WV_DEFAULT_BUF_SIZE	\
			WV_DEFAULT_BUF_THRESH	\
			WV_DEFAULT_BUF_OPTIONS	\
        	        WV_MAX_LOGS             \
                        WV_UPLOAD_STACKSIZE     \
                        WV_UPLOAD_PRIORITY
	HDR_FILES	wvLib.h rBuffLib.h
	REQUIRES	SELECT_TIMESTAMP \
			SELECT_WVUPLOAD  \
			SELECT_WV_BUFFER \
			INCLUDE_WINDVIEW_CLASS \
                        INCLUDE_SV_DATACOLLECTOR
}

Component INCLUDE_SV_DATACOLLECTOR {
	NAME		System Viewer data collector library
	SYNOPSIS	Data collectors for System Viewer
	INIT_RTN	svDataCollectorLibInit ();
	MODULES		svDataCollectorLib.o
        REQUIRES        INCLUDE_DATACOLLECTOR
	HDR_FILES	private/svDataCollectorLibP.h
}

Component INCLUDE_DATACOLLECTOR {
	NAME		Generic data collector library
	SYNOPSIS	Data collectors for tools
	INIT_RTN	dataCollectorLibInit ();
	MODULES		dataCollectorLib.o
        REQUIRES        INCLUDE_EVENTPOINT
	HDR_FILES	dataCollectorLib.h
}

Component INCLUDE_DATACOLLECTOR_SHOW {
	NAME		data collector show routines
	SYNOPSIS	data collector information
	MODULES		dataCollectorShow.o
	INIT_RTN	dataCollectorShowInit ();
	HDR_FILES	dataCollectorShow.h
        REQUIRES        INCLUDE_DATACOLLECTOR
}

Component INCLUDE_EVENTPOINT {
        NAME            eventpoints library
        SYNOPSIS        eventpoints instrumentation
        INIT_RTN        eventPointLibInit ();
	PROTOTYPE	STATUS eventPointLibInit (void);
        MODULES         eventPointLib.o
	HDR_FILES	private/eventPointLibP.h
        REQUIRES        INCLUDE_EVENTPOINT_STUB
}

Component INCLUDE_EVENTPOINT_STUB {
        NAME            eventpoint stub library
        SYNOPSIS        stub eventpoint implementation
        INIT_RTN        eventPointStubLibInit ();
	PROTOTYPE	STATUS eventPointStubLibInit (void);
        MODULES         eventPointStubLib.o
	HDR_FILES	private/eventDefsP.h \
			private/eventPointStubLibP.h
}

Selection SELECT_WV_BUFFER {
	NAME		System Viewer buffer management library
	COUNT		1-1
	CHILDREN	INCLUDE_RBUFF INCLUDE_WV_BUFF_USER
	DEFAULTS	INCLUDE_RBUFF
}

Component INCLUDE_WINDVIEW_CLASS {
	NAME		System Viewer class instrumentation
	SYNOPSIS	low-level kernel instrumentation needed by System Viewer
	INIT_RTN	wvLibInit ();
	MODULES		wvLib.o
}

Component INCLUDE_POSIX_TRACE {
	NAME		POSIX TRACE
	SYNOPSIS	POSIX trace in RTP.
	MODULES		pxTraceStreamLib.o
        INIT_RTN        pxTraceStreamLibInit ();
	HDR_FILES	private/pxTraceStreamLibP.h
	_CHILDREN	FOLDER_POSIX
	REQUIRES	INCLUDE_RTP \
			INCLUDE_POSIX_CLOCKS	\
			INCLUDE_POSIX_TRACE_TIMESTAMP
}

Component INCLUDE_POSIX_TRACE_TIMESTAMP {
        NAME            POSIX trace timestamp
        MODULES         wvTmrLib.o
        INIT_RTN        traceTmrRegister ((UINTFUNCPTR) sysTimestamp,	\
				(FUNCPTR) sysTimestampEnable,	\
				(FUNCPTR) sysTimestampDisable,	\
				(FUNCPTR) sysTimestampConnect,	\
				(UINTFUNCPTR) sysTimestampPeriod,	\
				(UINTFUNCPTR) sysTimestampFreq);
        REQUIRES        INCLUDE_SYS_TIMESTAMP
        HDR_FILES       wvTmrLib.h
}

Component INCLUDE_RBUFF {
	NAME		rBuff library
	SYNOPSIS	System Viewer ring of buffers for event logging 
	MODULES		rBuffLib.o 
	INIT_RTN	rBuffLibInit ();
        CFG_PARAMS      WV_RBUFF_MGR_PRIORITY
	HDR_FILES	rBuffLib.h
	INCLUDE_WHEN	INCLUDE_WINDVIEW
}

Parameter WV_RBUFF_MGR_PRIORITY {
	NAME		rBuff manager priority
	SYNOPSIS	Initial priority of rBuff manager task for System Viewer
	TYPE		int
	DEFAULT		100
}

Component INCLUDE_WV_BUFF_USER {
	NAME		user-defined System Viewer buffer manager
	SYNOPSIS	user defined library for managing System Viewer buffers
	INIT_RTN	USER_WV_BUFF_INIT
	CFG_PARAMS	USER_WV_BUFF_INIT
}

Component INCLUDE_WVNETD {
        NAME            network instrumentation
        SYNOPSIS        System Viewer instrumentation for network
        MODULES         wvNetDLib.o
        REQUIRES	INCLUDE_WINDVIEW
        INIT_RTN        wvNetDInit ();
        HDR_FILES       wvNetDLib.h
}

Component INCLUDE_TRIGGERING {
        NAME            triggering library
        SYNOPSIS        triggering for system and user events
	SOURCE		$(TGT_DIR)/src/event
        MODULES         trgLib.o
        INIT_RTN        trgLibInit ();
        HDR_FILES       trgLib.h private/trgLibP.h
        REQUIRES        INCLUDE_TRIG_DATACOLLECTOR
}

Component INCLUDE_TRIG_DATACOLLECTOR {
	NAME		Triggering data collector library
	SYNOPSIS	Data collectors for Triggering
	INIT_RTN	trgDataCollectorLibInit ();
	MODULES		trgDataCollectorLib.o
        REQUIRES        INCLUDE_DATACOLLECTOR
}

Component INCLUDE_POOL {
        NAME            pool allocation library
        SYNOPSIS        memory pools of fixed size items
	SOURCE		$(TGT_DIR)/src/util
        MODULES         poolLib.o
	INIT_RTN	poolLibInit();
        HDR_FILES       private/poolLibP.h
	REQUIRES	INCLUDE_KERNEL      \
			INCLUDE_LSTLIB      \
			INCLUDE_SEM_MUTEX   \
			INCLUDE_MEM_MGR_BASIC
}

Component INCLUDE_POOL_SHOW {
        NAME            pool allocation show routines
        SYNOPSIS        pool allocation show routines
        MODULES         poolShow.o
	LINK_SYMS	poolShow
	REQUIRES	INCLUDE_POOL
}

Component INCLUDE_SLL_CREATE_DELETE {
	NAME		dynamic creation and destruction of single linked list
	SOURCE		{ $(TGT_DIR)/src/util sllCreateLib.o }
	REQUIRES	INCLUDE_MEM_MGR_BASIC INCLUDE_SLL
	_CHILDREN	FOLDER_NOT_VISIBLE
}

Component INCLUDE_DLL_CREATE_DELETE {
	NAME		dynamic creation and destruction of double linked list
	SOURCE		{ $(TGT_DIR)/src/util dllCreateLib.o }
	REQUIRES	INCLUDE_MEM_MGR_BASIC INCLUDE_DLL
	_CHILDREN	FOLDER_NOT_VISIBLE
}

Component INCLUDE_Q_FIFO {
	NAME		Fifo queue library
	SOURCE		$(TGT_DIR)/src/util 
	MODULES		qFifoLib.o
	REQUIRES	INCLUDE_DLL
	_CHILDREN	FOLDER_NOT_VISIBLE
}

Component INCLUDE_Q_PRI_LIST {
	NAME		 queue library
	SOURCE		$(TGT_DIR)/src/util 
	MODULES		qPriListLib.o
	REQUIRES	INCLUDE_DLL   \
			INCLUDE_KERNEL
	_CHILDREN	FOLDER_NOT_VISIBLE
}

Component INCLUDE_Q_PRI_BMAP {
	NAME 		Bit-mapped priority queue management library
	SOURCE		$(TGT_DIR)/src/util 
	MODULES		qPriBMapLib.o
	REQUIRES	INCLUDE_DLL
	_CHILDREN	FOLDER_NOT_VISIBLE
}

Component INCLUDE_Q_PRI_DELTA {
	NAME 		Priority queue with relative priority sorting library
	SOURCE		$(TGT_DIR)/src/util 
	MODULES		qPriDeltaLib.o
	REQUIRES	INCLUDE_KERNEL \
			INCLUDE_DLL
	_CHILDREN	FOLDER_NOT_VISIBLE
}

Component INCLUDE_AVL_TREES {
	NAME		AVL tree library
	SYNOPSIS	AVL tree library with generic key
	SOURCE		$(TGT_DIR)/src/util 
	MODULES		avlLib.o
	_CHILDREN	FOLDER_NOT_VISIBLE
}

Component INCLUDE_AVL_UINT_TREES {
	NAME		UAVL tree library
	SYNOPSIS	AVL tree library with unsigned integer key
	SOURCE		$(TGT_DIR)/src/util 
	MODULES		avlUintLib.o
	_CHILDREN	FOLDER_NOT_VISIBLE
}

Component INCLUDE_CHECKSUM {
        NAME            checksum function
        SYNOPSIS        checksum function
	SOURCE		$(TGT_DIR)/src/util
        MODULES         cksumLib.o
        LINK_SYMS	checksum
        PROTOTYPE	u_short checksum (u_short *, int);
}

/*
 * Definition is generic, but not necessarily available on all arch.  Those
 * which support it will specify the INIT_ORDER, and override _CHILDREN to
 * make it visible/selectable.  The configlette, usrKernelStack.c, is listed
 * above under INCLUDE_KERNEL because it is always required (even on arch
 * which do not support the capability, where it is a no-op).
 */
Component INCLUDE_PROTECT_INTERRUPT_STACK {
	NAME		enable guard pages for interrupt stack
	SYNOPSIS	insert underflow and overflow guard pages on interrupt stack
	_CHILDREN	FOLDER_NOT_VISIBLE
	CFG_PARAMS	INTERRUPT_STACK_OVERFLOW_SIZE		\
			INTERRUPT_STACK_UNDERFLOW_SIZE
	REQUIRES	INCLUDE_MMU_BASIC
	PROTOTYPE	void usrKernelIntStkProtect(void);
	INIT_RTN	usrKernelIntStkProtect();
}

Parameter INTERRUPT_STACK_OVERFLOW_SIZE {
	NAME		Interrupt stack overflow protection size
	SYNOPSIS	This defines the size (in bytes) of the protected area above the interrupt stack.
	TYPE		uint
	DEFAULT		4096
}

Parameter INTERRUPT_STACK_UNDERFLOW_SIZE {
	NAME		Interrupt stack underflow protection size
	SYNOPSIS	This defines the size (in bytes) of the protected area below the interrupt stack.
	TYPE		uint
	DEFAULT		4096
}

/* initialize vxMemProbe support in exc Handler hook */

Component INCLUDE_VXMEMPROBE_INIT {
	NAME            vxMemProbe initializer for exception handler support
	SYNOPSIS        Initialize vxMemProbe exception handler support
	SOURCE          $(TGT_DIR)/src/os
	MODULES         vxMemProbeLib.o
	INIT_RTN        vxMemProbeInit ();
	HDR_FILES       vxLib.h private/vxMemProbeLibP.h
	REQUIRES        INCLUDE_EXC_HANDLING
}

/* Thread Local Storage (__thread variables) support */

Component INCLUDE_TLS {
	NAME		__thread variables support
	SYNOPSIS	__thread variables support
	SOURCE          $(TGT_DIR)/src/tool/common/tls
	MODULES		tlsLib.o
	HDR_FILES	private/tlsLibP.h
	REQUIRES	INCLUDE_TASK_CREATE_HOOKS
	INIT_BEFORE	INCLUDE_KERNEL
}

Component INCLUDE_TLS_LOADER_SUPPORT {
	NAME		__thread variables loader support
	SYNOPSIS	__thread variables loader support
	INIT_RTN        tlsLoadLibInit ();
	HDR_FILES	private/tlsLibP.h
	MODULES		tlsLoadLib.o
	_INIT_ORDER	usrToolsInit
	INIT_AFTER      usrLoaderInit
	INCLUDE_WHEN	INCLUDE_TLS \
			INCLUDE_LOADER
}

Component INCLUDE_GETOPT {
        NAME            getopt function
        SYNOPSIS        decodes arguments passed in argc/argv vector
        SOURCE          $(TGT_DIR)/src/util
        MODULES         getopt.o
        HDR_FILES       unistd.h
        LINK_SYMS       getoptInit
        REQUIRES        INCLUDE_ANSI_STRING
        INCLUDE_WHEN    INCLUDE_SYSCTL_CLI
        _CHILDREN       FOLDER_UTILITIES
}

/* INITIALIZATION ORDER */

InitGroup usrInit {
	PROTOTYPE	void usrInit (int startType)
	SYNOPSIS	pre-kernel initialization
	INIT_ORDER	INCLUDE_SYS_START	\
			INCLUDE_BOOT_HW_INIT \
			INCLUDE_CACHE_SUPPORT	\
                        INCLUDE_USER_PRE_KERNEL_APPL_INIT       \
			INCLUDE_EXC_SHOW	\
			INCLUDE_EXC_HANDLING	\
			INCLUDE_VXCPULIB        \
			INCLUDE_SYSHW_INIT	\
			INCLUDE_CACHE_ENABLE	\
			INCLUDE_OBJ_OWNERSHIP	\
			INCLUDE_OBJ_INFO	\
			INCLUDE_OBJ_LIB		\
			INCLUDE_VXMEMPROBE_INIT \
			INCLUDE_WINDVIEW_CLASS	\
			INCLUDE_CLASS_LIST	\
			INCLUDE_SEM_LIB 	\
			INCLUDE_SEM_MUTEX 	\
			INCLUDE_SEM_MUTEX_CREATE \
			INCLUDE_CLASS_LIB	 \
			INCLUDE_BASE_KERNEL	 \
			INCLUDE_TASK_CREATE_HOOKS       	\
			INCLUDE_CUSTOM_SCHEDULER \
			INCLUDE_TLS		 \
			INCLUDE_SYSDBG_FLAG	\
			INCLUDE_KERNEL
}

InitGroup usrToolsInit {
	INIT_RTN	usrToolsInit ();
	SYNOPSIS	software development tools
	INIT_ORDER	INCLUDE_SPY			\
			INCLUDE_JITTER_TOOL		\
			INCLUDE_TIMEX			\
                        INCLUDE_EVENTPOINT_STUB \
                        INCLUDE_EVENTPOINT \
                        INCLUDE_DATACOLLECTOR \
                        INCLUDE_SV_DATACOLLECTOR \
			INCLUDE_TRIGGERING	\
			INCLUDE_VXDBG		\
			usrWindviewInit		\
                        INCLUDE_POSIX_TRACE_TIMESTAMP     \
                        INCLUDE_POSIX_TRACE     \
			INCLUDE_TLS_LOADER_SUPPORT \
			INCLUDE_RTP_DEBUG_COMMAND \
			INCLUDE_TIP \
			usrShowInit
}

InitGroup usrWindviewInit {
	INIT_RTN	usrWindviewInit ();
	INIT_ORDER	INCLUDE_WINDVIEW	\
			INCLUDE_SYS_TIMESTAMP	\
			INCLUDE_USER_TIMESTAMP	\
			INCLUDE_SEQ_TIMESTAMP	\
			INCLUDE_RBUFF		\
			INCLUDE_WV_BUFF_USER	\
			INCLUDE_WDB_TSFS 	\
			INCLUDE_WVUPLOAD_SOCK	\
			INCLUDE_WVUPLOAD_TSFSSOCK\
			INCLUDE_WVUPLOAD_FILE   \
                        INCLUDE_WVNETD		\
			INCLUDE_WVTIPC
}

InitGroup usrShowInit {
	INIT_RTN	usrShowInit ();
	SYNOPSIS	enable object show routines
	INIT_ORDER	INCLUDE_TASK_SHOW \
			INCLUDE_CLASS_SHOW \
                        INCLUDE_ISR_SHOW \
			INCLUDE_MEM_SHOW \
			INCLUDE_TASK_HOOKS_SHOW \
			INCLUDE_SEM_SHOW \
			INCLUDE_MSG_Q_SHOW \
			INCLUDE_WATCHDOGS_SHOW \
			INCLUDE_VM_SHOW \
			INCLUDE_POSIX_MQ_SHOW \
			INCLUDE_POSIX_SEM_SHOW \
			INCLUDE_HW_FP_SHOW \
			INCLUDE_ATA_SHOW \
			INCLUDE_TRIGGER_SHOW \
			INCLUDE_RBUFF_SHOW \
			INCLUDE_STDIO_SHOW \
			INCLUDE_EDR_SHOW \
			INCLUDE_HANDLE_SHOW\
			INCLUDE_POSIX_TIMER_SHOW \
                        INCLUDE_DATACOLLECTOR_SHOW
}

InitGroup usrKernelCreateInit {
	INIT_RTN	usrKernelCreateInit ();
	SYNOPSIS	object creation routines
	INIT_ORDER	INCLUDE_SEM_DELETE			\
			INCLUDE_TASK_CREATE_DELETE		\
			INCLUDE_TASK_STACK_ALLOC		\
			INCLUDE_MSG_Q_CREATE_DELETE		\
			INCLUDE_WATCHDOGS_CREATE_DELETE
}

InitGroup usrRtpInit {
	INIT_RTN	usrRtpInit ();
	SYNOPSIS	RTP init routines
	INIT_ORDER	INCLUDE_RTP_HOOKS			\
			INCLUDE_RTP				\
			INCLUDE_SHARED_DATA			\
			INCLUDE_SHL
}

InitGroup usrKernelCoreInit {
	INIT_RTN	usrKernelCoreInit ();
	SYNOPSIS	core kernel facilities
	INIT_ORDER	INCLUDE_VXEVENTS			\
			INCLUDE_SEM_BINARY 			\
			INCLUDE_SEM_COUNTING 			\
			INCLUDE_SEM_READ_WRITE  		\
			INCLUDE_SEM_OPEN			\
			INCLUDE_MSG_Q 				\
			INCLUDE_MSG_Q_OPEN			\
			INCLUDE_WATCHDOGS 			\
			INCLUDE_TASK_OPEN			\
			INCLUDE_WDB_TASK_HOOKS			\
			INCLUDE_VXDBG_HOOK
}

InitGroup usrKernelExtraInit {
	INIT_RTN	usrKernelExtraInit ();
	SYNOPSIS	extended kernel facilities
	INIT_ORDER	INCLUDE_HASH		\
                        INCLUDE_TASK_VARS       \
			INCLUDE_SIGNALS 	\
			INCLUDE_SIGEVENT 	\
			INCLUDE_POSIX_MQ 	\
			INCLUDE_POSIX_PTHREADS 	\
			INCLUDE_POSIX_SEM 	\
			INCLUDE_POSIX_SIGNALS 	\
			INCLUDE_POSIX_CLOCKS    \
                        INCLUDE_POSIX_TIMERS    \
			INCLUDE_TIMER_OPEN	\
			INCLUDE_PROTECT_VEC_TABLE
}

InitGroup usrIosCoreInit {
	INIT_RTN	usrIosCoreInit ();
	SYNOPSIS	core I/O system
	INIT_ORDER	INCLUDE_IO_BASIC       \
                        INCLUDE_IO_FILE_SYSTEM \
                        INCLUDE_IO_REMOVABLE   \
                        INCLUDE_IO_POSIX       \
                        INCLUDE_IO_RTP         \
                        INCLUDE_IO_MISC        \
			INCLUDE_SELECT \
			INCLUDE_ISR_DEFER \
			INCLUDE_TYLIB \
			INCLUDE_TTY_DEV \
			INCLUDE_TYCODRV_5_2 \
			INCLUDE_SIO \
			INCLUDE_PC_CONSOLE
	}

InitGroup usrIosExtraInit {
	INIT_RTN	usrIosExtraInit ();
	SYNOPSIS	extended I/O system
	INIT_ORDER	INCLUDE_JOB_TASK \
			INCLUDE_EXC_TASK \
			INCLUDE_ERF \
			INCLUDE_SM_OBJ \
			INCLUDE_LOGGING \
			INCLUDE_PIPES \
			INCLUDE_POSIX_ADVISORY_FILE_LOCKING \
			INCLUDE_POSIX_AIO 	\
			INCLUDE_POSIX_AIO_SYSDRV  \
			INCLUDE_STDIO \
			INCLUDE_FORMATTED_IO \
			INCLUDE_FLOATING_POINT \
			INCLUDE_DEVICE_MANAGER \
			INCLUDE_XBD \
			INCLUDE_XBD_BLK_DEV \
			INCLUDE_FS_MONITOR \
			INCLUDE_FS_EVENT_UTIL \
			INCLUDE_XBD_TRANS \
			INCLUDE_CDROMFS \
			INCLUDE_DOSFS_MAIN \
			INCLUDE_DOSFS_CACHE \
                        INCLUDE_DOSFS_SHOW \
			INCLUDE_HRFS \
			INCLUDE_HRFS_READONLY	\
			INCLUDE_RAWFS \
			INCLUDE_HRFS_FORMAT	\
                        INCLUDE_UTF \
			INCLUDE_RAMDRV \
			INCLUDE_MEMDRV \
			INCLUDE_PTYDRV \
			INCLUDE_ROMFS \
			INCLUDE_PROCFS \
			INCLUDE_SCSI \
			INCLUDE_SCSI_DOSFS \
			INCLUDE_RAM_DISK   \
			INCLUDE_VXBUS_DISK \
			INCLUDE_FD \
			INCLUDE_IDE \
			INCLUDE_ATA \
			INCLUDE_LPT \
			INCLUDE_PCMCIA      \
			INCLUDE_TFFS        \
			INCLUDE_TFFS_MOUNT  \
			INCLUDE_EEPROMDRV   \
			INCLUDE_VRFS        \
			INCLUDE_BOOT_LINE_INIT
}

InitGroup usrRoot {
	PROTOTYPE	void usrRoot (char *pMemPoolStart, unsigned memPoolSize)
	SYNOPSIS	entry point for post-kernel initialization
	INIT_ORDER	INCLUDE_TM_TIMING_FRAMEWORK	\
			usrKernelCoreInit	\
			INCLUDE_MEM_ALLOT			\
			INCLUDE_POOL				\
			INCLUDE_MEM_EDR				\
			INCLUDE_MEM_MGR_FULL			\
			INCLUDE_MEM_MGR_BASIC			\
			INCLUDE_KERNEL_PROXIMITY_HEAP		\
			INCLUDE_MEM_PART_CACHE			\
			INCLUDE_MEM_EDR2			\
			INCLUDE_AIM_MMU_CONFIG  		\
			INCLUDE_PG_POOL_LIB			\
			INCLUDE_PG_POOL_VIRT_LIB		\
			INCLUDE_PG_POOL_PHYS_LIB		\
			INCLUDE_MMU_BASIC 			\
			INCLUDE_TM_TIMING_FRAMEWORK_PRE_MMU_INIT \
			INCLUDE_MMU_GLOBAL_MAP 			\
			INCLUDE_TM_TIMING_FRAMEWORK_POST_MMU_INIT \
			INCLUDE_KERNEL_COMMON_HEAP		\
			usrKernelCreateInit			\
                        INCLUDE_USER_POST_KERNEL_APPL_INIT      \
			INCLUDE_MEM_MGR_INFO			\
                        INCLUDE_USER_RESERVED_MEMORY		\
			INCLUDE_ENV_VARS 	                \
			INCLUDE_EDR_PM				\
			INCLUDE_EDR_ERRLOG			\
			INCLUDE_EDR_STUB			\
			INCLUDE_CACHE_COMPACT_DMA_SUPPORT	\
			INCLUDE_PROTECT_TEXT 			\
			INCLUDE_PROTECT_TASK_STACK		\
			INCLUDE_TASK_STACK_NO_EXEC		\
			INCLUDE_SYSHW_INIT_1			\
			INCLUDE_IDLE_TASKS      		\
			INCLUDE_DEFERRED_INTERRUPT_ENABLE	\
			INCLUDE_SYSHW_INIT2			\
			INCLUDE_SYSCLK_INIT			\
			INCLUDE_CPC				\
                        usrDshmInit             		\
                        usrMultiosInit          		\
			INCLUDE_VXDBG_CPU_CONTROL		\
			INCLUDE_SW_FP 				\
			INCLUDE_HW_FP 				\
			INCLUDE_PG_MGR_BASIC			\
			INCLUDE_PG_MGR_LIB			\
			INCLUDE_SYSCTL				\
			usrRtpInit				\
			INCLUDE_CORE_DUMP			\
			INCLUDE_CORE_DUMP_RTP			\
			INCLUDE_POSIX_PTHREAD_SCHEDULER 	\
			INCLUDE_PX_SCHED_SPORADIC_POLICY 	\
			usrIosCoreInit				\
			INCLUDE_MMAP				\
			INCLUDE_POSIX_MAPPED_FILES		\
			INCLUDE_POSIX_SHM			\
			INCLUDE_POSIX_SHM_API			\
			INCLUDE_DEVMEM  			\
			usrKernelExtraInit			\
			usrIosExtraInit				\
                        INCLUDE_SOCKLIB         		\
			INCLUDE_AF_MIPC_SOCKETS			\
                        INCLUDE_CNS         			\
                        INCLUDE_SSI         			\
                        INCLUDE_DSI_DYNAMIC_INIT     		\
                        INCLUDE_USER_PRE_NETWORK_APPL_INIT      \
			usrNetworkInit				\
			INCLUDE_AF_MIPC_SOCKETS	\
			usrTipcInit			        \
                        INCLUDE_DSI_STATIC_INIT     		\
			INCLUDE_SELECT_SUPPORT			\
                        INCLUDE_ACPI                            \
                        INCLUDE_ACPI_SHOW                       \
                        INCLUDE_ACPI_CPU_CONFIG                 \
                        INCLUDE_CPU_PWR_ARCH                    \
                        INCLUDE_DEV_PWR_MGMT                    \
                        INCLUDE_CPU_TURBO                       \
                        INCLUDE_CPU_PERFORMANCE_MGMT            \
                        INCLUDE_CPU_ODG_POLICY_MGMT             \
                        INCLUDE_PWR_POLICY_MGR                  \
			INCLUDE_CPU_LIGHT_PWR_MGR 		\
                        INCLUDE_TICKLESS_IDLE                   \
                        INCLUDE_CTORS_DTORS     		\
			INCLUDE_CPLUS				\
			INCLUDE_CPLUS_DEMANGLER			\
			INCLUDE_HTTP				\
			usrScInit               		\
			INCLUDE_USB_HOST_CLASS_INIT     	\
			INCLUDE_USB_TGT_FUNC_INIT       	\
			INCLUDE_SMP_INIT			\
			usrToolsInit				\
			usrWriftInit				\
			INCLUDE_MMU_OPTIMIZE 			\
			INCLUDE_SIMPLE_BANNER   		\
			INCLUDE_TM_TIMING_FRAMEWORK_END	\
			INCLUDE_USER_APPL
}

InitGroup usrScInit {
    INIT_RTN        usrScInit ();
    SYNOPSIS        The system call initialization sequence
    INIT_ORDER      INCLUDE_SC_POSIX
}

Parameter TASK_USER_EXC_STACK_SIZE {
	NAME		User task exception stack size
	SYNOPSIS	This defines the size (in bytes) of the exception stack for the user tasks.
	TYPE		uint
#ifdef _WRS_CONFIG_LP64
	DEFAULT		16384
#else
	DEFAULT		8192
#endif
}

Parameter TASK_USER_EXC_STACK_OVERFLOW_SIZE {
        NAME            User task exception stack overflow protection size
        SYNOPSIS        This defines the size (in bytes) of the protected area above the task stacks.
        TYPE            uint
        DEFAULT         4096	
}
                                                                                                                      
Parameter TASK_USER_EXEC_STACK_OVERFLOW_SIZE {
        NAME            User task execution stack overflow protection size
        SYNOPSIS        This defines the size (in bytes) of the protected area above the task stacks.
        TYPE            uint
        DEFAULT         4096
}
                                                                                                                      
Parameter TASK_USER_EXEC_STACK_UNDERFLOW_SIZE {
        NAME            User task execution stack underflow protection size
        SYNOPSIS        This defines the size (in bytes) of the protected area below the task stacks.
        TYPE            uint
        DEFAULT         4096
}
                                                                                                                      
Parameter TASK_KERNEL_EXEC_STACK_OVERFLOW_SIZE {
        NAME            Kernel task execution stack overflow protection size
        SYNOPSIS        This defines the size (in bytes) of the protected area above the task stacks.
        TYPE            uint
        DEFAULT         4096
}
                                                                                                                      
Parameter TASK_KERNEL_EXEC_STACK_UNDERFLOW_SIZE {
        NAME            Kernel task execution stack underflow protection size
        SYNOPSIS        This defines the size (in bytes) of the protected area below the task stacks.
        TYPE            uint
        DEFAULT         4096
}

Parameter TASK_PRIORITY_SET_COMPATIBLE {
	NAME		Enable compatible mode for task priority setting
	SYNOPSIS	This configuration parameter can be used to enable \
			the backward compatible (but incorrect) way of \
			handling task priority changes: when set to TRUE, \
			a running task may be preemted when its priority \
			is lowered to a level at which other ready tasks \
			are waiting. This configuration option will be \
			removed in a future release.
	TYPE		BOOL
	DEFAULT		FALSE
}

Parameter INT_LOCK_LEVEL {
	NAME		INT lock level
	SYNOPSIS	Interrupt lock level
}

Parameter ROOT_STACK_SIZE {
	NAME		root stack size
	SYNOPSIS	Root task stack size (bytes)
}

Parameter ISR_STACK_SIZE {
	NAME		ISR stack size
	SYNOPSIS	ISR Stack size (bytes)
}

Parameter WIND_JOBS_MAX  {
        NAME            work queue size
        SYNOPSIS        WIND work queue size (# of entries).  Must be a power of 2. \
			A value below 64 may cause work queue panics.
        TYPE            UINT32
        DEFAULT         64
}
          
Parameter SEM_RW_MAX_CONCURRENT_READERS {
        NAME            Maximum number of concurrent RW semaphore readers
        SYNOPSIS        Maximum number of concurrent RW semaphore readers
        TYPE            UINT32
        DEFAULT         32
}

Parameter MAX_ISR_JOBS {
        NAME            Maximum number of jobs posted from ISR level
        SYNOPSIS        Maximum number of jobs posted from ISR level
        TYPE            UINT32
        DEFAULT         16
}

Parameter JOB_TASK_STACK_SIZE {
        NAME            Job Task stack size
        SYNOPSIS        Job Task stack size
        TYPE            UINT32
        DEFAULT         8000
}

Parameter USER_I_MMU_ENABLE {
	NAME		instruction MMU
	SYNOPSIS	Enable instruction MMU
	TYPE		exists
	DEFAULT		TRUE
}

Parameter USER_D_MMU_ENABLE {
	NAME		data MMU
	SYNOPSIS	Enable data MMU
	TYPE		exists
	DEFAULT		TRUE
}

Parameter NUM_RAWFS_FILES {
	NAME		max # open RAWFS files
	TYPE		uint
	DEFAULT		5
}

Parameter NUM_DOSFS_FILES {
	NAME		max # open DOSFS files
	TYPE		uint
	DEFAULT		20
}

Parameter ERF_MAX_USR_CATEGORIES {
        NAME           max # of ERF user categories
        TYPE           UINT16
        DEFAULT        16
}

Parameter ERF_MAX_USR_TYPES {
        NAME           max # of ERF user types
        TYPE           UINT16
        DEFAULT        16
}

Parameter DEVICE_MANAGER_MAX_NUM_DEVICES {
        NAME           max # of devices
        TYPE           uint16_t
        DEFAULT        255
}

Parameter MAX_LOG_MSGS {
	NAME		max # queued messages
	TYPE		uint
	DEFAULT		50
}

Parameter NUM_DRIVERS {
	NAME		max # drivers
	TYPE		uint
	DEFAULT		20
}

Parameter NUM_FILES {
	SYNOPSIS	Limit to FD_SETSIZE (2048) when using selectLib
	NAME		max # open files in the system
	TYPE		uint
	DEFAULT		50
}

Parameter CLEAR_BSS {
	NAME		clear BSS at system startup (recommended)
	TYPE		exists
	DEFAULT		TRUE
}

Parameter MAX_LIO_CALLS {
	NAME		max outstanding lio calls, 0=default
	TYPE		uint
	DEFAULT		0
}

Parameter MAX_AIO_SYS_TASKS {
	NAME		max aio system tasks, 0 = default
	TYPE		uint
	DEFAULT		0
}

Parameter AIO_TASK_PRIORITY {
	NAME		aio system tasks priority, 0 = default
	TYPE		uint
	DEFAULT		0
}

Parameter AIO_TASK_STACK_SIZE {
	NAME		aio tasks stack size, 0 = default
	TYPE		uint
	DEFAULT		0
}

Parameter MQ_HASH_SIZE {
	NAME		POSIX message queue hash table size, 0 = default
	TYPE		uint
	DEFAULT		0
}

Parameter NUM_SIGNAL_QUEUES {
	NAME		POSIX queued signal count
	TYPE		uint
	DEFAULT		16
}

Parameter LOGIN_USER_NAME {
	NAME		rlogin/telnet user name
	DEFAULT		"target"
}

Parameter LOGIN_PASSWORD {
	NAME		rlogin/telnet encrypted password
	DEFAULT		"m4qJ/O/Iam+2AdBmwD7+cav+W6HABSdMF2yQyK+rIQA="
}

Parameter  LOGIN_PASSWORD_SALT {
        NAME            rlogin/telnet encrypted password salt
        DEFAULT         "BFqADK/VLEk="
}

Parameter ENV_VAR_USE_HOOKS {
	NAME		install environment variable task create/delete hooks
	TYPE		bool
	DEFAULT		TRUE
}

Parameter USER_WV_BUFF_INIT {
	NAME		routine to initialize user-defined buffer manager
}

Parameter USER_TIMESTAMP {
	NAME	        Timestamp routine
	SYNOPSIS	user defined timestamp routine address
}

Parameter USER_TIMESTAMPLOCK {
	NAME	        Timestamp with intLock 
	SYNOPSIS	same as Timestamp but with interrupts locked
}

Parameter USER_TIMEENABLE {
	NAME	        enable Timestamp routine
	SYNOPSIS	enable a timestamp timer interrupt
}

Parameter USER_TIMEDISABLE {
	NAME	        disable Timestamp routine
	SYNOPSIS	disable a timestamp timer interrupt
}

Parameter USER_TIMECONNECT {
	NAME	        connect Timestamp routine to interrupt
	SYNOPSIS	connect a user routine to a timestamp timer interrupt
}

Parameter USER_TIMEPERIOD {
	NAME	        get the period of timestamp 
	SYNOPSIS	get the period of a timestamp timer
}

Parameter USER_TIMEFREQ {
	NAME		get frequency of timestamp
	SYNOPSIS	get a timestamp timer clock frequency
}

Parameter WV_DEFAULT_BUF_MIN {
	NAME		min number of buffers 
	TYPE		int
	DEFAULT		4
}

Parameter WV_DEFAULT_BUF_MAX {
	NAME		max number of buffers 
	TYPE		int
	DEFAULT		10
}

Parameter WV_DEFAULT_BUF_SIZE {
	NAME		size of each buffer 
	TYPE		int
	DEFAULT		0x8000
}

Parameter WV_DEFAULT_BUF_THRESH {
	NAME		buffer threshold
	TYPE		int
	DEFAULT		0x4000
}

Parameter WV_DEFAULT_BUF_OPTIONS {
	NAME		buffer options
	TYPE		int
	DEFAULT		0x0
}

Parameter WV_MAX_LOGS {
	NAME		Maximum number of Wind View logs
	SYNOPSIS	Maximum number of Wind View logs on target at any time
	TYPE		int
	DEFAULT		5
}

Parameter WV_UPLOAD_STACKSIZE {
	NAME		Stack allocated to System Viewer upload task
	SYNOPSIS	Stack allocated to System Viewer upload task
	TYPE		int
	DEFAULT		5000
}

Parameter WV_UPLOAD_PRIORITY {
	NAME		System Viewer upload task priority
	SYNOPSIS	Initial priority of System viewer upload task
	TYPE		int
	DEFAULT		150
}

Parameter OBJ_LIBRARY_OPTIONS {
	NAME            Default option for object library
        SYNOPSIS        global options for object library
	TYPE            int
        DEFAULT         0
}

Parameter EDR_PM_ARENA {
	NAME		ED&R PM Arena Definition
	DEFAULT		pmDefaultArena
}

Parameter EDR_ERRLOG_SIZE {
	NAME		ED&R Error Log Size (in bytes)
	DEFAULT		(pmFreeSpace (EDR_PM_ARENA) / 2)
}

Parameter EDR_RECORD_SIZE {
	NAME		Size of each ED&R record (in bytes)
	DEFAULT		4096
}

Parameter PM_RESERVED_MEM {
	NAME		Persistent Memory Region Size (in bytes)
	DEFAULT		(6 * VM_PAGE_SIZE)
}

Parameter MAX_REBOOT_HOOKS {
	NAME		Maximum number of reboot hooks
	TYPE		int
	DEFAULT		5
}

Parameter VX_GLOBAL_NO_STACK_FILL {
	NAME		Disable stack fill on all task
	TYPE		BOOL
	DEFAULT		FALSE
}

#ifdef _WRS_CONFIG_SMP
Parameter VX_SMP_NUM_CPUS {
	NAME		Number of CPUs to be enabled for SMP
	SYNOPSIS	This value must be greater than zero.
	TYPE		UINT
	DEFAULT		2
	_CFG_PARAMS	INCLUDE_KERNEL
}

Parameter ENABLE_ALL_CPUS {
	NAME		Enable all configured CPUs for SMP
	TYPE		BOOL
	DEFAULT		TRUE
	_CFG_PARAMS	INCLUDE_KERNEL
}

Parameter VX_ENABLE_CPU_TIMEOUT {
	NAME		Timeout for enabling additional CPUs in SMP (in seconds)
	TYPE		int
	DEFAULT		5
	_CFG_PARAMS	INCLUDE_KERNEL
}

Parameter VX_SMP_CPU_EXPLICIT_RESERVE {
	NAME	    	SMP CPUs to be excluded from indiscriminate reservation
	SYNOPSIS	The CPUs specified in this parameter are excluded from \
			the indiscriminate reservation pool.  They are not, \
			however, prevented from being explicitly reserved.
	TYPE		string
	DEFAULT		""
	_CFG_PARAMS	INCLUDE_KERNEL
}

#endif

/* spinLock library API */

Api API_SPINLOCK {
	NAME 		spinLock libraries API
	SYNOPSIS 	spinLock libraries API selection
	_CHILDREN 	FOLDER_KERNEL
	_REQUIRES 	INCLUDE_KERNEL
	DEFAULTS 	INCLUDE_SPINLOCK
}

/* spinLock component that offers debugging code at runtime */

#ifdef _WRS_CONFIG_SMP
Component INCLUDE_SPINLOCK_DEBUG {
	NAME 		spinLock with debugging 
	SYNOPSIS 	spinLock library with debugging capability
	SOURCE		$(TGT_DIR)/src/os/lock
	MODULES		spinLockSmpDbgLib.o
	PROVIDES 	API_SPINLOCK
	ARCHIVE 	libsplDbg.a
}
#endif

/* spinLock component without debugging */

Component INCLUDE_SPINLOCK {
	NAME 		spinLock
	SYNOPSIS 	spinLock libraries without debugging capability
	SOURCE		$(TGT_DIR)/src/os/lock
#ifdef _WRS_CONFIG_SMP
	MODULES		spinLockSmpLib.o
#else
	MODULES		spinLockUpLib.o
#endif
	PROVIDES 	API_SPINLOCK
	ARCHIVE 	libspl.a
}

Component INCLUDE_SPINLOCK_ISR_ND {
	NAME 		Fast, non-deterministic ISR callable spin locks
	SYNOPSIS 	Fast but non-fair ISR spin lock implementation
	SOURCE		$(TGT_DIR)/src/os/lock
	MODULES		spinLockIsrNdLib.o
	LINK_SYMS	spinLockIsrNdTake spinLockIsrNdInit
	_CHILDREN	FOLDER_KERNEL
}

Component INCLUDE_SPINLOCK_ND_TIMED {
	NAME 		AMP/SMP safe, ISR callable, time limited spin locks
	SYNOPSIS 	MP safe, ISR callable, time limited spin locks
	SOURCE		$(TGT_DIR)/src/os/lock
	MODULES		spinLockNdTimedLib.o
	LINK_SYMS	spinLockNdTimedTake
	_CHILDREN	FOLDER_KERNEL
}

Folder FOLDER_KERNEL_DEBUG {
	NAME            Kernel-write components
	CHILDREN        INCLUDE_DEBUG_KPRINTF \
			INCLUDE_DEBUG_KPUTS   \
			SELECT_DEBUG_KWRITE
	DEFAULT         INCLUDE_DEBUG_KPRINTF \
			INCLUDE_DEBUG_KPUTS
}

Selection SELECT_DEBUG_KWRITE {
	NAME            kernel-write facility components
	SYNOPSIS        provides the output routine for kernel-write facility
	COUNT           1-1
	CHILDREN        INCLUDE_DEBUG_KWRITE_SIO \
			INCLUDE_DEBUG_KWRITE_USER
	DEFAULTS        INCLUDE_DEBUG_KWRITE_SIO
}

Component INCLUDE_DEBUG_KPUTS {
	NAME            kputs component
	SYNOPSIS        unformatted kernel print facility
	MODULES         kputsLib.o
	LINK_SYMS       kputs
	REQUIRES        SELECT_DEBUG_KWRITE
}

Component INCLUDE_DEBUG_KPRINTF {
	NAME            kprintf component
	SYNOPSIS        formatted kernel print facility
	MODULES         kprintfLib.o
	HDR_FILES	stdio.h private/kwriteLibP.h
	INIT_RTN	_func_kprintf = kprintf;
	REQUIRES        SELECT_DEBUG_KWRITE
}

Component INCLUDE_DEBUG_KWRITE_SIO {
	NAME            kwrite sio component
	SYNOPSIS        polled mode sio routine for kprintf and kputs
	MODULES         kwriteSioLib.o
	PROTOTYPE	extern void kwriteSioInit (int numTty, int baud);
        INIT_RTN        kwriteSioInit (CONSOLE_TTY, CONSOLE_BAUD_RATE);
        CFG_PARAMS      CONSOLE_TTY 		\
			CONSOLE_BAUD_RATE
	REQUIRES	INCLUDE_SIO_POLL
}

Component INCLUDE_DEBUG_KWRITE_USER {
	NAME            kwrite component for user routine
	SYNOPSIS        user provided output routine for kprintf and kputs
	CONFIGLETTES    usrDebugKwrite.c
        PROTOTYPE       STATUS DEBUG_KWRITE_USR_RTN(char * pBuf, size_t len);
	INIT_RTN        usrKwriteInit (DEBUG_KWRITE_USR_RTN);
	CFG_PARAMS      DEBUG_KWRITE_USR_RTN
}

Parameter DEBUG_KWRITE_USR_RTN {
	NAME            configuration parameter for usrKwriteInit routine
	SYNOPSIS        Configuration parameter provided by the user
	TYPE            void *
	DEFAULT         NULL
}

InitGroup usrDebugKWriteInit {
	INIT_RTN        usrDebugKWriteInit ();
	SYNOPSIS        Kernel printf initialization
	INIT_ORDER      INCLUDE_DEBUG_KWRITE_SIO	\
			INCLUDE_DEBUG_KWRITE_USER	\
			INCLUDE_DEBUG_KPRINTF
#ifdef _WRS_CONFIG_DELAYED_HW_INIT
	_INIT_ORDER	usrRoot
	INIT_AFTER	INCLUDE_SYSHW_INIT_1
	INIT_BEFORE	INCLUDE_IDLE_TASKS
#else
	_INIT_ORDER	usrInit
	INIT_AFTER	INCLUDE_SYSHW_INIT
	INIT_BEFORE	INCLUDE_CACHE_ENABLE
#endif
}

Parameter RUNTIME_CREATION_TIME {
	NAME            Runtime creation time 
	SYNOPSIS        Runtime identification string. The value will be \
			embedded into the final VxWorks image, allowing to \
			uniquely identify the resultant image. Defaults to \
			the date and time of the build. 
	TYPE            string
	DEFAULT         __DATE__" "__TIME__
}
