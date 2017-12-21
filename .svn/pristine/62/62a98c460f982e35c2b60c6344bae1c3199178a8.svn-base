/* arch.cdf - PPC arch components */

/*
 * Copyright (c) 2003-2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
04k,09apr13,x_s  removed INCLUDE_L2_CACHE & INCLUDE_L3_CACHE and series 
                 Cache Error handler components for e6500.
04j,01mar13,h_k  added func pointers setting at init for EPCI controller
                 driver.
04i,25feb13,h_k  moved INCLUDE_SMP_SCHED_SMT_POLICY and INCLUDE_SMT component
                 definitions from T4240 BSP.
04h,29nov12,x_s  added component INCLUDE_CORE_E6500 to e6500.
04g,23oct12,x_s  added L2&L3 components to e6500.
04f,19sep12,h_k  added Altivec support to e6500.
04e,25jul12,h_k  added e6500 CPU variant support. (WIND00333229)
04d,15may12,jmp  CQID: Fix WIND00324029: Removed WDB_TASK_OPTIONS &
                 WDB_SPAWN_OPTS.
04c,21mar12,h_k  removed INCLUDE_MMU_E500_SHOW. (WIND00340176)
04b,29nov11,h_k  added VX_ALTIVEC_TASK flag to WDB_TASK_OPTIONS for PPC604 and
                 PPC970. (WIND00113300)
04a,10nov11,ers  CQ297094:Adding INCLUDE_MMU_E500_SHOW
03z,08sep11,ers  Adding INCLUDE_MMU_BASIC for WRLOAD.
03y,30apr11,syt  added support for PPC465
03x,09dec10,x_z  added support for PPC476
03x,13nov10,mze  replace _WRS_VX_SMP with _WRS_CONFIG_SMP
03w,25feb10,pmr  SMP M-N updates: Require INCLUDE_VXCPULIB with
                 INCLUDE_KERNEL_ARCH if _WRS_CONFIG_WRHV_GUEST not defined
03v,23feb10,rlg  fix for WIND00201305 provided by matt merged into 6.8.1 by
                 ron SMP M-N updates: Require INCLUDE_VXCPULIB with
                 INCLUDE_KERNEL_ARCH
03u,14dec09,d_c  Defect WIND00192520 Fix WIND00194275
03t,25sep09,pmr  add Altivec non-java option
03t,21sep09,cww  Removed global definition of INCLUDE_DEFERRED_INTERRUPT_ENABLE
03s,13aug09,cww  Updated INCLUDE_DEFERRED_INTERRUPT_ENABLE init location
03s,18dec08,h_k  updated INCLUDE_ANSI_MATH for E500.
03r,22sep08,pch  CQ128808, CQ135295, possibly others: update cache/MMU
		 dependency for 440 & 85xx variants
03q,18sep08,pch  CQ135053: remove ansiMath.o and duplicate support.o from e500v2
03p,26aug08,jpb  Removed LAYER from components.
03o,19mar08,to   added taskALib.o for SMP kernel
03n,19nov07,dtr  Change ev2diab to e500v2diab.
03m,07nov07,dtr  Update ANSI_MATH requires for ev2diab for 85xx.
03l,05oct07,mze  fix for 104829
03k,13sep07,pch  CQ102745: idle-task exception stack protection
03j,20aug07,kk   make optimized atomic operators available only for SMP
03i,09may07,dtr  Support e500 and e200 cache without MMU.
03h,01mar07,jmg  Added INCLUDE_ATOMIC_OPERATORS component
03g,03aug06,pch  SPR 117691: cache/MMU global variable cleanup
03g,11jul06,pcs  Updated to add obj/class lib support in MKL.
03f,30jun06,dtr  Remove space after line continue.
03e,29jun06,dtr  Support new CPU variants.
03d,17mar06,h_k  renamed altivecShow.c to altivecShow.o. (SPR #119213)
03c,23feb06,h_k  removed INCLUDE_IN_CKSUM_ARCH. (SPR #118030)
03b,30nov05,yvp  INCLUDE_SIGNAL_ARCH needs INCLUDE_ANSI_LONGJMP (SPR 115285)
03a,07oct05,h_k  added FP_TYPE as a required comp for ANSI_MATH. (SPR #113210)
		 removed BKL profile.
02z,27sep05,h_k  added INCLUDE_POOL for BKL profile. (SPR #112394)
02y,14sep05,h_k  added arch specific ANSI_MATH. (SPR #112320)
		 and IN_CKSUM.
02x,12sep05,yvp  Added INCLUDE_SIGNAL_ARCH.
02w,07sep05,h_k  changed HIDDEN to FOLDER_NOT_VISIBLE. (SPR #111769)
		 cleaned up BLIB_ARCH dependency.
02v,26aug05,mil  Changed INCLUDE_MMU_BASIC to INCLUDE_MMU_GLOBAL_MAP.
		 Added INCLUDE_OBJ_LIB_ARCH.
02u,23aug05,jmt  Move windPwrLib.o from INCLUDE_KERNEL to INCLUDE_KERNEL_ARCH
02t,12aug05,zl   changed USER_HASH_TABLE_ADDRESS to belong to
		 INCLUDE_MMU_GLOBAL_MAP.
02s,03aug05,pee  Added LAYER information
02r,13Jul05,pee  Added INCLUDE_KERNEL_ARCH and INCLUDE_BLIB_ARCH
01r,13apr05,bpn  Remove use of quote character from INCLUDE_ALTIVEC_MREGS
		 component.
01q,17mar05,asa  added component INCLUDE_ALTIVEC_MREGS
01q,09mar05,pch  adjust for PPC32 builds
01p,26feb05,pcs  Change TASK_STACK_OVERFLOW_SIZE to
		 TASK_USER_EXC_STACK_OVERFLOW_SIZE
01o,02mar05,slk  Fix include problem for INCLUDE_PROTECT_INTERRUPT_STACK
01n,04feb05,pch  SPR 102772: add INCLUDE_PROTECT_INTERRUPT_STACK
01m,17dec04,to   PPC970 also uses USER_HASH_TABLE_* parms
01l,02nov04,pch  add PROTOTYPEs
01k,25oct04,pch  SPR 92598: add excOsmInit (exception stack overflow)
		 add excIntNestLogInit (LOG_FRAMELESS and LOG_DISPATCH support)
01j,24sep04,dtr  Minor mod to altivec and spe components.
01i,25aug04,job  Renamed INCLUDE_LOADER to INCLUDER_LOADER_CODE (SPR#101151)
01h,08jun04,to   SPR#98048: added INCLUDE_CACHE_L2.
01h,09may04,zl   removed references to vxVMI.
01g,15apr04,elg  Remove obsolete WDB components.
01f,12apr04,zl   removed MMU re-definitions and SELECT_MMU.
01e,13apr04,pch  SPR 95355: add INCLUDE_DEFERRED_INTERRUPT_ENABLE
01d,09mar04,tam  increase MMU selection max count to 2
01c,26feb04,mil  Added SPE support for PPC85XX.
01b,31oct03,dat  starting mod history
01a,31oct03,yvp  added INCLUDE_RTP_ARCH modules
*/

/*
This cdf file defines the PPC arch specific component units.
*/

/*
 * This will override parts of INCLUDE_ATOMIC_OPERATORS the
 * component in 00vxworks.cdf
*/

#ifdef _WRS_SMP_SMP
Component INCLUDE_ATOMIC_OPERATORS {
	MODULES		vxAtomicALib.o
	SOURCE 		$(TGT_DIR)/src/arch/ppc
}

/*
 * Specify initialization sequence for generic
 * component, and make it visible/selectable.
 */

Component INCLUDE_PROTECT_IDLE_TASK_STACK {
	_CHILDREN	FOLDER_KERNEL
	_INIT_ORDER	usrRoot
	REQUIRES	INCLUDE_OSM
	INIT_AFTER	INCLUDE_OSM
	INIT_BEFORE	INCLUDE_PROTECT_TASK_STACK
}
#endif	/* SMP */

Component INCLUDE_OSM {
	NAME		Overflow Stopgap Mode
	SYNOPSIS	Handler for Exception Stack Overflow
	PROTOTYPE	void excOsmInit (int, int);
	INIT_RTN	excOsmInit (TASK_USER_EXC_STACK_OVERFLOW_SIZE, VM_PAGE_SIZE);
	_CHILDREN	FOLDER_NOT_VISIBLE
	REQUIRES	INCLUDE_MMU_BASIC
	INCLUDE_WHEN	INCLUDE_PROTECT_TASK_STACK
	_INIT_ORDER	usrRoot
	INIT_AFTER	INCLUDE_MMU_BASIC
	INIT_BEFORE	INCLUDE_PROTECT_TASK_STACK
}

Component INCLUDE_DEFERRED_INTERRUPT_ENABLE {
	NAME		Deferred Interrupt Enable
	SYNOPSIS	Enable interrupts at appropriate point in root task
	HDR_FILES	arch/ppc/archPpc.h	\
			arch/ppc/vxPpcLib.h
	PROTOTYPE	void excIntNestLogInit (void); \
			IMPORT _RType taskMsrDefault;	/* from taskArchLib.c */
	INIT_RTN	excIntNestLogInit(); \
			vxMsrSet(vxMsrGet() | taskMsrDefault);
	_CHILDREN	FOLDER_NOT_VISIBLE
	INCLUDE_WHEN	INCLUDE_KERNEL			/* always */
	_INIT_ORDER	usrRoot
	INIT_AFTER	usrKernelCoreInit	\
			INCLUDE_MMU_BASIC
	INIT_BEFORE	INCLUDE_SYSCLK_INIT
}

#if ( !defined PPC403 && !defined _ppc403 )
/*
 * Make generic definition visible in GUI by placing it in a visible folder,
 * and specify initialization sequence.
 */

Component INCLUDE_PROTECT_INTERRUPT_STACK {
	_CHILDREN	FOLDER_KERNEL
	INCLUDE_WHEN	INCLUDE_KERNEL_HARDENING
	_INIT_ORDER	usrRoot
	REQUIRES	INCLUDE_OSM
	INIT_AFTER	INCLUDE_OSM
	INIT_BEFORE	INCLUDE_PROTECT_TASK_STACK
}
#endif	/* !403 */

Component INCLUDE_LOADER_CODE {
	NAME		ELF loader
	SYNOPSIS	ELF loader
	MODULES		loadLib.o loadElfLib.o
	INIT_RTN	loadElfInit ();
	HDR_FILES	loadElfLib.h
}

Component INCLUDE_EXC_EXTENDED_VECTORS {
	NAME		Allow 32-bit branches to handlers
	SYNOPSIS	Extended 32-bit vector branches to handlers
	_CHILDREN	FOLDER_KERNEL
	HDR_FILES	private/excLibP.h
	CFG_PARAMS	EXC_EXTENDED_VECTORS_ENABLED
	INIT_RTN	excExtendedVectors = EXC_EXTENDED_VECTORS_ENABLED;
	_INIT_ORDER	usrInit
	INIT_BEFORE	INCLUDE_EXC_HANDLING
}

/* Component is already defined, just insert the module names for PPC */

Component INCLUDE_RTP_ARCH {
	MODULES		syscallArchLib.o syscallALib.o \
			rtpALib.o rtpSigArchLib.o
}

Component INCLUDE_CACHE_SUPPORT {
	NAME		cache support
	SYNOPSIS	include cache support
	INIT_RTN	cacheLibInit (USER_I_CACHE_MODE, USER_D_CACHE_MODE);
	MODULES		cacheLib.o
	CFG_PARAMS	USER_I_CACHE_MODE USER_D_CACHE_MODE
#if (!defined PPC403 && !defined _ppc403 \
 && !defined PPC405 && !defined _ppc405 \
 && !defined PPC440 && !defined _ppc440 && !defined _ppc440_x5 && !defined _ppc465 \
 && !defined PPC85XX && !defined _ppc85XX && !defined _ppc85XX_e200 \
 && !defined _ppc85XX_e500mc && !defined _ppc85XX_e500v2 \
 && !defined _ppc85XX_e6500 && !defined _ppc476)
	REQUIRES	INCLUDE_MMU_GLOBAL_MAP
#endif	/* !4xx && !85xx */
	HDR_FILES	cacheLib.h
}

Component INCLUDE_CACHE_L2 {
	NAME		L2 cache support
	SYNOPSIS	include L2 cache support
	_CHILDREN	FOLDER_MEMORY
	REQUIRES	INCLUDE_CACHE_SUPPORT
}

Component INCLUDE_CACHE_L2_ENABLE {
	NAME		enable L2 cache
	SYNOPSIS	optionally enable L2 cache
	_CHILDREN	FOLDER_MEMORY
	CFG_PARAMS	USER_L2_CACHE_ENABLE
	REQUIRES	INCLUDE_CACHE_L2
	INCLUDE_WHEN	INCLUDE_CACHE_L2
}

#ifdef _ppc85XX_e6500
Component INCLUDE_CORE_E6500 {
	NAME		core e6500
	SYNOPSIS	include core e6500
	CONFIGLETTES	usrCoreE6500.c
	_CHILDREN	FOLDER_NOT_VISIBLE    
}
#endif /* _ppc85XX_e6500 */

Component INCLUDE_SIGNAL_ARCH {
	NAME		architecture dependent code for signals
	SYNOPSIS	architecture dependent code for signals
	SOURCE		$(TGT_DIR)/src/arch/ppc
	MODULES		sigCtxALib.o sigCtxLib.o
	REQUIRES	INCLUDE_KERNEL \
			INCLUDE_ANSI_LONGJMP
	INCLUDE_WHEN	INCLUDE_SIGNALS
	_CHILDREN	FOLDER_NOT_VISIBLE
}

Parameter USER_L2_CACHE_ENABLE {
	NAME		enable L2 cache
	TYPE		exists
	DEFAULT		TRUE
}

Parameter EXC_EXTENDED_VECTORS_ENABLED {
	NAME		Enables Extended 32-bit exception vector
	TYPE		bool
#ifdef _WRS_CONFIG_WRHV_GUEST
	DEFAULT 	FALSE
#else /* _WRS_CONFIG_WRHV_GUEST */
	DEFAULT 	TRUE
#endif /* _WRS_CONFIG_WRHV_GUEST */
}

Parameter USER_I_MMU_ENABLE {
	NAME		enable instruction MMU
	TYPE		exists
#if ( defined PPC405  || defined PPC440 || \
      defined _ppc405 || defined _ppc440 || defined _ppc440_x5 )
	DEFAULT 	FALSE
#else	/* 405 || 440 */
	DEFAULT 	TRUE
#endif	/* 405 || 440 */
	_CFG_PARAMS	INCLUDE_MMU_GLOBAL_MAP
}

Parameter STACK_SAVE {
	NAME		Stack size
	SYNOPSIS	Amount of stack to reserve (bytes) for rom code
	DEFAULT		0x1000
}

Parameter INT_LOCK_LEVEL {
	NAME		INT lock level
	SYNOPSIS	Interrupt lock level
	DEFAULT		0x0
}

Parameter ROOT_STACK_SIZE {
	NAME		Root stack size
	SYNOPSIS	Root task stack size (bytes)
	DEFAULT		24000
}

Parameter SHELL_STACK_SIZE {
	NAME		Shell stack size
	SYNOPSIS	Shell stack size (bytes)
	DEFAULT		20000
}

Parameter WDB_STACK_SIZE {
	NAME		WDB Stack size
	SYNOPSIS	WDB Stack size (bytes)
	DEFAULT		0x2000
}

Parameter ISR_STACK_SIZE {
	NAME		ISR stack size
	SYNOPSIS	ISR Stack size (bytes)
	DEFAULT		5000
}

Parameter VEC_BASE_ADRS {
	NAME		Vector base address
	SYNOPSIS	Vector base address
	DEFAULT		((char *) 0x0 )
}

Parameter ROM_WARM_ADRS {
	NAME		ROM warm boot address
	SYNOPSIS	ROM warm boot address
	DEFAULT		(ROM_TEXT_ADRS + 4)
}

#if ( defined PPC604 || defined _ppc604 || defined PPC970 \
     || defined _ppc970 || defined _ppc604_745x || defined _ppc85XX_e6500 )
Component INCLUDE_ALTIVEC {
	NAME		AltiVec runtime support
	SYNOPSIS	AltiVec register save/restore routines
	_CHILDREN	FOLDER_PERIPHERALS
	HDR_FILES	altivecLib.h
	CONFIGLETTES	SaveRestVR.c usrAltivec.c
	CFG_PARAMS	ALTIVEC_NON_JAVA_MODE
	REQUIRES	INCLUDE_COPROCESSOR
}

Component INCLUDE_ALTIVEC_SHOW {
	NAME		AltiVec show routine
	SYNOPSIS	task AltiVec registers
	MODULES		altivecShow.o
	_CHILDREN	FOLDER_SHOW_ROUTINES
	INCLUDE_WHEN	INCLUDE_ALTIVEC \
			INCLUDE_SHOW_ROUTINES
	REQUIRES	INCLUDE_ALTIVEC \
			INCLUDE_COPROCESSOR_SHOW
}

Component INCLUDE_ALTIVEC_MREGS {
	NAME		altivec registers modification
	SYNOPSIS	Provides ability to modify altivec registers from the kernel shell.
	MODULES		altivecRegsLib.o
	INCLUDE_WHEN	INCLUDE_ALTIVEC_SHOW \
			INCLUDE_SHELL
	REQUIRES	INCLUDE_ALTIVEC_SHOW \
			INCLUDE_SHELL
	_CHILDREN 	FOLDER_NOT_VISIBLE
}

Parameter ALTIVEC_NON_JAVA_MODE {
	NAME		Altivec non-java mode
	TYPE		exists
	DEFAULT		FALSE
}
#endif	/* 604 || 970 || e6500 */

#if ( defined PPC603 || defined PPC604 || defined _ppc603 || defined _ppc604 \
   || defined PPC970 || defined _ppc970 || defined _ppc603_83xx \
   || defined _ppc603_g2le || defined _ppc604_745x )
Parameter USER_HASH_TABLE_ADDRESS {
	NAME		MMU Hash Table location
	TYPE		void *
	DEFAULT		NULL
	_CFG_PARAMS	INCLUDE_MMU_GLOBAL_MAP
}

Parameter USER_HASH_TABLE_SIZE {
	NAME		MMU Hash Table size in bytes
	TYPE		uint
	DEFAULT		0x0
	_CFG_PARAMS	INCLUDE_MMU_GLOBAL_MAP
}
#endif	/* 60x || 970 */

#if ( defined PPC85XX || defined _ppc85XX || defined _ppc85XX_e500v2 )
Component INCLUDE_SPE {
	NAME		Spe runtime support
	SYNOPSIS	Spe register save/restore routines
	_CHILDREN	FOLDER_PERIPHERALS
	HDR_FILES	speLib.h
	REQUIRES	INCLUDE_COPROCESSOR
}

Component INCLUDE_SPE_SHOW {
	NAME		Spe show routine
	SYNOPSIS	task Spe registers
	_CHILDREN	FOLDER_SHOW_ROUTINES
	INCLUDE_WHEN	INCLUDE_SPE \
			INCLUDE_SHOW_ROUTINES
	REQUIRES	INCLUDE_SPE \
			INCLUDE_COPROCESSOR
}
#endif	/* 85XX */

Component INCLUDE_KERNEL_ARCH {
	NAME		Architecture Specific Kernel Components
#ifdef	_WRS_CONFIG_SMP
	SOURCE		{$(TGT_DIR)/src/arch/ppc \
			{intALib.o intArchLib.o windALib.o excALib.o \
			taskArchLib.o intLockALib.o vxLib.o vxALib.o \
			isrArchDecLib.o ffsALib.o ppcArchVars.o \
			ppcBrCompute.o taskALib.o }} \
			{$(TGT_DIR)/src/tool/common saverest.o } \
			{$(TGT_DIR)/src/wind windPwrLib.o }
#else	/* ! _WRS_CONFIG_SMP */
	SOURCE 		{$(TGT_DIR)/src/arch/ppc \
			{intALib.o intArchLib.o windALib.o excALib.o \
			taskArchLib.o intLockALib.o vxLib.o vxALib.o \
			isrArchDecLib.o ffsALib.o ppcArchVars.o \
			ppcBrCompute.o }} \
			{$(TGT_DIR)/src/tool/common saverest.o } \
			{$(TGT_DIR)/src/wind windPwrLib.o }
#endif	/* _WRS_CONFIG_SMP */
#ifndef _WRS_CONFIG_WRHV_GUEST
        REQUIRES        INCLUDE_VXCPULIB
#endif /* _WRS_CONFIG_WRHV_GUEST */
	_CHILDREN	FOLDER_NOT_VISIBLE
}

Component INCLUDE_BLIB_ARCH {
	NAME		Architecture specific blib implementation
	SOURCE		{$(TGT_DIR)/src/arch/ppc bALib.o }
	REQUIRES	INCLUDE_BLIB
	INCLUDE_WHEN	INCLUDE_BLIB
	_CHILDREN	FOLDER_NOT_VISIBLE
}

Component INCLUDE_OBJ_LIB_ARCH {
	NAME		object management
	SYNOPSIS	architecture dependent portion of OBJ_LIB
	SOURCE		{$(TGT_DIR)/src/arch/ppc {objArchLib.o objALib.o}}
	REQUIRES	INCLUDE_OBJ_LIB
	INCLUDE_WHEN	INCLUDE_OBJ_LIB
	_CHILDREN	FOLDER_NOT_VISIBLE
}

Component INCLUDE_ANSI_MATH {
	NAME		ANSI math
	SYNOPSIS	ANSI mathematical functions
#if (defined diab) || (defined gnu)
	SOURCE		{$(TGT_DIR)/src/math cabs.o} \
			{$(TGT_DIR)/src/libc/math \
			{support.o frexp.o ldexp.o modf.o \
			acosh.o asincos.o asinh.o atanh.o ceil.o cosh.o \
			exp__E.o \
			expm1.o floor.o fmod.o log1p.o log__L.o sinh.o \
			tan.o tanh.o fabs.o}} \
			{$(TGT_DIR)/src/arch/ppc/math/motorolaLib \
			{atan.o pow.o sqrt.o trig.o}}
#endif
#if (defined e500v2diab) || (defined e500v2gnu)
	SOURCE		{$(TGT_DIR)/src/math cabs.o} \
			{$(TGT_DIR)/src/libc/math \
			{frexp.o ldexp.o modf.o \
			ceil.o exp__E.o expm1.o floor.o fabs.o \
			fmod.o log1p.o log__L.o support.o asincos.o asinh.o \
			cosh.o \
			sinh.o tan.o tanh.o}} \
			{$(TGT_DIR)/src/arch/ppc/math/fslE500ObjLib \
			{elf8wrapper-atan.o elf8wrapper-exp.o \
			elf8wrapper-log.o elf8wrapper-pio2.o \
			elf8wrapper-pow.o elf8wrapper-sin-cos.o \
			elf8wrapper-sqrt.o}}
#endif
#if (defined sfdiab) || (defined sfgnu)
	SOURCE		{$(TGT_DIR)/src/math cabs.o} \
			{$(TGT_DIR)/src/libc/math support.o} \
			{$(TGT_DIR)/src/arch/ppc/math/usSoftwareLib \
			{arc32.o ceil32.o dp32.o dpcmp.o exp32.o floor32.o \
			fp32.o \
			fparc32.o fpceil32.o fpcmp.o fpexp32.o fpfloo32.o \
			fphyp32.o fplog32.o fpmod32.o fppow32.o fpsqrt32.o \
			fptrig32.o func32.o hyp32.o log32.o mod32.o pow32.o \
			sqrt32.o trig32.o gccMathLib.o dccMathLib.o}}
#endif
	LINK_SYMS	acos asin atan atan2 ceil cos cosh exp fabs floor \
			fmod frexp ldexp log log10 modf pow sin sinh sqrt \
			tan tanh
#if (defined diab) || (defined gnu)
	REQUIRES	INCLUDE_HW_FP	\
			INCLUDE_FP_TYPE
#endif
#if (defined e500v2diab) || (defined e500v2gnu)	
	REQUIRES	INCLUDE_SPE	\
			INCLUDE_FP_TYPE
#endif
#if (defined sfdiab) || (defined sfgnu)
	REQUIRES	INCLUDE_SW_FP	\
			INCLUDE_FP_TYPE
#endif	/* diab || gnu */
	HELP		ansiMath
}

Component INCLUDE_WRLOAD {
	REQUIRES	+= INCLUDE_MMU_BASIC
}

#if (defined _ppc85XX_e6500) && (defined _WRS_CONFIG_SMP)
/*
 * enable SMT scheduling policy by overriding the MODULES definition.
 * The scheduling policy requires SMT support; both are available only
 * for SMP builds, and both are included for default SMP configurations.
 *
 * Note: we cannot make INCLUDE_SMT required for INCLUDE_SMP_SCHED_SMT_POLICY
 * because by doing so the removal of INCLUDE_SMT would result of the removal
 * of most kernel components.
 */

Component INCLUDE_SMP_SCHED_SMT_POLICY {
        MODULES         readyQDeltaSmtLib.o
}

Api API_SMP_SCHED_POLICIES {
        DEFAULTS        INCLUDE_SMP_SCHED_SMT_POLICY
}

Component INCLUDE_SMT {
        NAME            Threading support
        SYNOPSIS        Simultaneous Multi-Threading support
        _CHILDREN       FOLDER_BSP_CONFIG
        INCLUDE_WHEN    INCLUDE_SMP_INIT
}

Component DRV_INTCTLR_EPIC {
	CONFIGLETTES	hwif/vxbEpicIntCtlr.dc
	INIT_RTN	vxbEpicIntCtlrPreInit ();
	PROTOTYPE	void vxbEpicIntCtlrPreInit (void);
}
#endif	/* _ppc85XX_e6500 && _WRS_CONFIG_SMP */
