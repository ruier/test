/* 00bsp.cdf - BSP component description file */

/*
 * Copyright (c) 1998-2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute or otherwise make use of this software
 * may be licensed only pursuant to the terms of an applicable Wind River
 * license agreement.
 */

/*
modification history
--------------------
03w,10jun13,y_y  remove the wrapper of the INCLUDE_TIMESTAMP64. (WIND00392055)
03v,31may13,y_y  update INCLUDE_TIMESTAMP64 only for LP64. (WIND00414497)
03u,09apr13,x_s  correct SYNOPSIS description of the component INCLUDE_NVRAM_FLASH.
03t,08jan13,x_z  added NVRAM to Flash mapiing library.
03s,07feb12,l_z  add I2C RTC support
03r,03oct11,sem  WIND00307699 - Restore sleep mode component definitions
03q,29sep11,sem  WIND00307699 - Move pwr mgmt folder from hardware to os
03p,18aug11,dee  WIND00278138 - add 64-bit timestamp support
03o,07jul11,j_l  WIND00255353 PwrMgmt: VxWorks Tickless operating mode for IA
03n,15jun11,rec  change INCLUDE_POWER_MGMT_CPU_BSP_SUPPORT to
                 INCLUDE_TICKLESS_IDLE
03m,29nov10,jpb  Defect 00242483.  Added user-reserved memory to memory
                 folder.
03m,12nov10,mze  _WRS_VX_SMP is now obsolete replaced with _WRS_CONFIG_SMP
03j,07oct10,rec  correct component INCLUDE_POWER_MGMT_CPU_BSP_SUPPORT
03l,24sep10,zl   enable MMU page optimization by default for 64-bit.
03k,02jul10,zl   page optimization support for 64-bit VxWorks.
03j,01jun10,jpb  Updated INCLUDE_MEMORY_CONFIG
03j,11jun10,slk  Added parameters for LMM support
03i,05apr10,jpb  Fixed user-reserved memory error and renamed clear memory
                 flag.
03h,30mar10,jpb  Added support for user-reserved memory.
03g,10feb10,zl   disabled SW_MMU_ENABLE for 64-bit VxWorks.
03f,15may09,zl   removed vmData.o from INCLUDE_MMU_GLOBAL_MAP.
03e,30sep08,kab  WIND00133329: Fix AMP default CPU selection
03d,26aug08,jpb  Removed LAYER from components.
03c,15oct07,zl   disable SW_MMU_ENABLE for SMP.
03b,04oct07,mze  fix for 104829
03a,16aug07,pch  change _WRS_VX_SMP to SMP
02z,02aug07,h_k  made SCSI components invisible in SMP
02y,22jan07,pch  add SMP time base synchronization
02x,20jun06,pee  typos
02w,28may06,jmt  Add NV_RAM_SIZE_WRITEABLE Parameter
02w,16may06,pee  fixed parameter names. Fix WIND00045161
02v,30nov05,pgh  Add default aux clock default rate parameter.
02u,24aug05,h_k  added layer info for Power Manage. added SIO_POLL_CONSOLE
                 parameter. removed cache components for source build.
02t,15aug05,h_k  added LAYER info and new components for source build.
02s,12aug05,zl   added INCLUDE_MMU_GLOBAL_MAP, added layer and source info 
		 for VM library components.
02r,03aug05,pee  Added LAYER information
02q,03jun05,pcm  removed INCLUDE_ATA as it is in 10bsp.cdf
02p,08jul05,pee  added SOURCE for CACHE
02o,25feb05,h_k  cleaned up AIM MMU definitions.
02n,27aug04,rec  SPR 98758 - power management component
02m,17sep04,sru  vmTextProtect() -> usrTextProtect()
02l,29sep04,zl   removed MMU_MPU component.
02k,18aug04,dtr  Make AIM conditional for arch and cpu.
02j,03aug04,md   moved PM_RESERVED_MEM to ED&R PM component
02i,04aug04,dtr  Fix description for AIM components.
02h,06jul04,dtr  Adding in AIM configuration and optimization.
02g,09jun04,zl   removed vxVMI component
02f,28apr04,h_k  removed DSP component.
02e,12apr04,zl   software MMU simulation support
02d,06apr04,yp   adding parameters to usrMmuInit
02c,01mar04,tam  Temp for for MMU Selection.
02b,22oct03,dat  removed NET_BSD components. moved TFFS stuff out, new
		 INCLUDE_BSP_MACROS.  This is a placeholder for BSP
		 custom macros to be defined.
02a,08oct03,tam  fixed dependencies with INCLUDE_MMU_FULL
01z,25sep03,rp   merging from bas6_itn2_networking to base6_itn2
01y,11sep03,dbs  add support for PM_RESERVED_MEM
01x,29may03,rec  Power Management, add INCLUDE_POWER_MGMT_CPU_BSP_SUPPORT
01w,21jun02,jkf  SPR#74185, removed STACK_ADRS and STACK_RESIDENT params 
01v,20jun02,h_k  fixed Component INCLUDE_DSP (SPR #78892)
01u,02apr02,gls  added MMU params to INCLUDE_MMU components (SPR #74158)
01t,05mar02,yp   added CFI drver for AMD devices to TrueFFS
01s,13nov01,nrv  changed TFFS support by creating FOLDER_TFFS
01r,09nov01,brk  spelling mistake (SPR 63637)
01q,09oct01,rae  merge from truestack ver 01n base o1m (SPR #67023)
01p,17may01,rec   SPR 63492 - cache not enabled from project make
01o,07dec00,pai  Merged in WRN fixes for project builds in the 'Barney'
                 release.
01n,06dec00,spm  renamed usrConfigIf.c for project tool build fix
01m,16nov00,zl   added DSP component.
01l,16nov00,cmf  added parameters to BSP Memory Configuration Component for
                 SPR 31071
01k,22apr99,pr	 added dependency on DOSFS for ATA
01j,10mar99,yh	 chg'd name to INCLUDE_USR_ENTRIES (SPR 25331)
01i,26feb99,dat  added FEI driver to list, (SPR 23818)
01h,25feb99,cth  added CONSOLE_BAUD_RATE param to INCLUDE_PC_CONSOLE(SPR 24862)
01g,23feb99,cth  added CONSOLE_BAUD_RATE param to INCLUDE_SIO (SPR 24862)
01f,07jan99,cjs  fixed spelling error in folder description 
01e,15sep98,ms   fixed some errors uncovered by new cxrDocValidate.
01d,19aug98,sut  Moved BSP components from 00vxWorks.cdf
01c,18aug98,ann  Removed LOOPBACK dependancy on BSD
01b,18aug98,sut  fixed the SYSCLK_INIT
01a,23jul98,sut  written
*/

/*
DESCRIPTION
This file contains descriptions for some BSP components.
The configuration tool searches for .cdf files in four
places; here, arch/<arch>, config/<bsp>, and the project directory.
This file describes "generic" components which are available
to all BSPs.
If a new component is released, a new .cdf file should be deposited
in the appropriate directory, rather than modifying existing files.
*/


/* Generic BSP configuration parameters */

/*
 * Because this is processed before 00vxWorks.cdf, be very careful to not
 * refer to components outside this file.
 */

Folder	FOLDER_HARDWARE {
	NAME		hardware
	SYNOPSIS	Hardware components
	CHILDREN	FOLDER_MEMORY		\
			FOLDER_BUSES		\
			FOLDER_PERIPHERALS	\
			FOLDER_BSP_CONFIG
	DEFAULTS	FOLDER_MEMORY		\
			FOLDER_BSP_CONFIG
}

Folder FOLDER_MEMORY {
	NAME		memory
	SYNOPSIS	Memory components
	CHILDREN	INCLUDE_MEMORY_CONFIG	\
			FOLDER_MMU		\
			INCLUDE_CACHE_SUPPORT	\
			INCLUDE_CACHE_ENABLE    \
			INCLUDE_USER_RESERVED_MEMORY
}

Folder FOLDER_MMU {
	NAME		MMU
	CHILDREN	INCLUDE_PROTECT_TEXT		\
			INCLUDE_PROTECT_VEC_TABLE	\
			INCLUDE_MMU_BASIC		\
			INCLUDE_MMU_GLOBAL_MAP		\
			FOLDER_AIM_CONFIG
	DEFAULTS	INCLUDE_MMU_BASIC		\
			INCLUDE_MMU_GLOBAL_MAP
}

Folder FOLDER_BUSES {
	NAME		buses
	SYNOPSIS	Bus components
#ifdef	_WRS_CONFIG_SMP
	CHILDREN	INCLUDE_PCI_PARAMS	\
			INCLUDE_VME_PARAMS	\
			INCLUDE_PCMCIA
#else	/* ! _WRS_CONFIG_SMP */
	CHILDREN	INCLUDE_PCI_PARAMS	\
			INCLUDE_VME_PARAMS	\
			INCLUDE_PCMCIA		\
			FOLDER_SCSI
#endif	/* _WRS_CONFIG_SMP */
}

#ifndef	_WRS_CONFIG_SMP
Folder	FOLDER_SCSI {
	NAME		scsi
	SYNOPSIS	SCSI components
	CHILDREN	INCLUDE_SCSI		\
			SELECT_SCSI_VERSION
}
#endif	/* !_WRS_CONFIG_SMP */

Folder	FOLDER_PWR_MGMT {
	NAME		power management
	SYNOPSIS	Power Management component
	CHILDREN	INCLUDE_TICKLESS_IDLE \
			SELECT_POWER_MGMT
	DEFAULTS	INCLUDE_TICKLESS_IDLE \
			SELECT_POWER_MGMT
}

Folder FOLDER_PERIPHERALS {
	NAME		peripherals
	SYNOPSIS	Peripheral components
	CHILDREN	INCLUDE_LPT	\
			INCLUDE_FD	\
			FOLDER_HD	\
			FOLDER_CLOCK	\
			FOLDER_SERIAL	\
			FOLDER_FPP
}

Folder	FOLDER_HD {
	NAME		hard disks
	SYNOPSIS	Hard disk components
	CHILDREN	INCLUDE_IDE	\
			INCLUDE_ATA
}

Folder	FOLDER_CLOCK {
	NAME		clocks
	SYNOPSIS	Clock component
	CHILDREN	INCLUDE_SYSCLK_INIT	\
			INCLUDE_AUX_CLK		\
			INCLUDE_TIMESTAMP	\
			INCLUDE_TIMER_RTC	\
			INCLUDE_TIMESTAMP64
}

Folder	FOLDER_SERIAL {
	NAME		serial
	SYNOPSIS	serial
	CHILDREN	INCLUDE_SIO		\
			INCLUDE_SIO_POLL	\
			INCLUDE_PC_CONSOLE
}

Folder	FOLDER_BSP_CONFIG {
	NAME		BSP configuration variants
	CHILDREN	INCLUDE_BSP_MACROS
	DEFAULTS	INCLUDE_BSP_MACROS
}

Selection SELECT_AMP_CPU_NUMBER {
	NAME		vxWorks AMP core to build
	COUNT		1-1
	REQUIRES	INCLUDE_AMP_CPU
	INCLUDE_WHEN	INCLUDE_AMP_CPU
	_CHILDREN	FOLDER_BSP_CONFIG
	DEFAULTS	INCLUDE_AMP_CPU_00
	CHILDREN	INCLUDE_AMP_CPU_01 \
			INCLUDE_AMP_CPU_00
	}

Component INCLUDE_AMP_CPU_01  {
	NAME		AMP kernel configured for core 01
	}

Component INCLUDE_AMP_CPU_00  { 
	NAME		AMP kernel configured for core 00
	}

Folder  FOLDER_MMU_OPTIMIZE {
	NAME		MMU optimization
	SYNOPSIS	optionally enable MMU optimization
	CHILDREN	INCLUDE_LOCK_TEXT_SECTION \
			INCLUDE_PAGE_SIZE_OPTIMIZATION
}

Component INCLUDE_BSP_MACROS {
	NAME		General BSP macros
	SYNOPSIS	Placeholder component for general BSP macros
	CFG_PARAMS	DEFAULT_BOOT_LINE
}

Component INCLUDE_MEMORY_CONFIG {
	NAME		BSP Memory Configuration
	SYNOPSIS	Memory configuration parameter for BSP
	CFG_PARAMS      LOCAL_MEM_LOCAL_ADRS	\
			LOCAL_MEM_SIZE		\
			LOCAL_MEM_AUTOSIZE	\
			LOCAL_MEM_PHYS_ADRS	\
			LOCAL_UNMAPPED_BASE_ADRS	\
			NV_RAM_SIZE		\
			NV_BOOT_OFFSET		\
			VEC_BASE_ADRS		\
			EXC_MSG_OFFSET		\
			EXC_MSG_ADRS		\
			BOOT_LINE_SIZE		\
			BOOT_LINE_ADRS		\
			BOOT_LINE_OFFSET	\
			RESERVED		\
			FREE_RAM_ADRS		\
			ROM_WARM_ADRS		\
			STACK_SAVE		\
			RAM_HIGH_ADRS		\
			RAM_LOW_ADRS		\
			ROM_BASE_ADRS		\
			ROM_TEXT_ADRS		\
			ROM_SIZE
}

#ifndef _WRS_CONFIG_LP64
Component INCLUDE_MEMORY_CONFIG {
        CFG_PARAMS +=   USER_RESERVED_MEM
}
#endif /* !_WRS_CONFIG_LP64 */

Component INCLUDE_CACHE_SUPPORT {
	NAME		cache support
	SYNOPSIS	include cache support
	INIT_RTN	cacheLibInit (USER_I_CACHE_MODE, USER_D_CACHE_MODE);
	SOURCE		$(WIND_BASE)/target/src/os/mm
	MODULES		cacheLib.o
	CFG_PARAMS	USER_I_CACHE_MODE USER_D_CACHE_MODE
	HDR_FILES	cacheLib.h
}

Component INCLUDE_CACHE_ENABLE {
	NAME		enable caches
	SYNOPSIS	optionally enable caches
	CONFIGLETTES	usrCache.c
	INIT_RTN	usrCacheEnable ();
	CFG_PARAMS	USER_I_CACHE_ENABLE USER_D_CACHE_ENABLE
	REQUIRES	INCLUDE_CACHE_SUPPORT
	INCLUDE_WHEN	INCLUDE_CACHE_SUPPORT
	HDR_FILES	cacheLib.h
}

Component INCLUDE_CPU_SLEEP_MODE_OFF {
	NAME		Idle Power Management Off Operating Mode
	SYNOPSIS	Spin when in idle.
	REQUIRES	SELECT_POWER_MGMT
}

Component INCLUDE_CPU_SLEEP_MODE_SHORT {
	NAME		Idle Power Management Short Operating Mode
	SYNOPSIS	Service all timer interrupts and idle-off.
	REQUIRES	SELECT_POWER_MGMT
}

Component INCLUDE_CPU_SLEEP_MODE_LONG {
	NAME		Idle Power Management tickless 
	SYNOPSIS	Reduces clock timer interrupts and idle-off.
	REQUIRES	SELECT_POWER_MGMT
}

Component INCLUDE_TICKLESS_IDLE {
	NAME		Tickless-idle component 
	SYNOPSIS	Reduce timer interrupts when in idle 
        INIT_RTN        sysOneShotInit (); sysCpuPwrEnable ();
        PROTOTYPE       void sysOneShotInit (void); extern void sysCpuPwrEnable (void);
	REQUIRES	SELECT_POWER_MGMT
}

Component INCLUDE_PROTECT_TEXT {
	NAME		write-protect program text
	CONFIGLETTES	usrTextProtect.c
	INIT_RTN	usrTextProtect ();
	REQUIRES	INCLUDE_MMU_BASIC
	HDR_FILES	vmLib.h
}

Component INCLUDE_PROTECT_VEC_TABLE {
	NAME		write-protect vector table
	INIT_RTN	intVecTableWriteProtect ();
	REQUIRES	INCLUDE_MMU_BASIC
	HDR_FILES	intLib.h
}

Component INCLUDE_AIM_MMU_MEM_POOL_CONFIG {
	NAME		Page table and region table memory pool configuration
	REQUIRES	INCLUDE_MMU_BASIC INCLUDE_AIM_MMU_CONFIG
	CFG_PARAMS	AIM_MMU_INIT_PT_NUM \
			AIM_MMU_INIT_PT_INCR \
			AIM_MMU_INIT_RT_NUM \
			AIM_MMU_INIT_RT_INCR
}

Component INCLUDE_AIM_MMU_PT_PROTECTION {
	NAME		write-protect page tables
	REQUIRES	INCLUDE_MMU_BASIC INCLUDE_AIM_MMU_CONFIG
}

Component INCLUDE_LOCK_TEXT_SECTION {
	NAME		Lock program text into TLB
	SYNOPSIS	Lock page table entries for kernel text segment into \
			statically managed TLB entries. The number of static \
			TLB entries is limited, therefore it is not always \
			possible to lock the entire text segment. Not every \
			architecture and CPU family supports this feature. \
			See the respective Architecture Supplement for details.
	REQUIRES	INCLUDE_MMU_OPTIMIZE
}

Component INCLUDE_PAGE_SIZE_OPTIMIZATION {
	NAME		Optimize page tables
	SYNOPSIS	Enable page size optimization. On 32-bit VxWorks the \
			kernel context is optimized at the end of the \
			initialization sequence. On 64-bit VxWorks all \
			mappings (kernel and RTP) are performed automatically \
			with page size optimization when this component is \
			included. Not every architecture and CPU family \
			supports this feature. See the respective \
			Architecture Supplement for details.
#ifdef _WRS_CONFIG_LP64
	INCLUDE_WHEN	INCLUDE_MMU_BASIC
#else
	REQUIRES	INCLUDE_MMU_OPTIMIZE
#endif
}

Component INCLUDE_MMU_OPTIMIZE {
	NAME		Optimize configlette
	CONFIGLETTES	usrMmuOptimize.c
	INIT_RTN	usrMmuOptimize ();
	REQUIRES	INCLUDE_MMU_BASIC
	_CHILDREN	FOLDER_NOT_VISIBLE
	HDR_FILES	vmLib.h
}
	
Component INCLUDE_AIM_MMU_CONFIG {
	NAME		AIM MMU configlette
	CONFIGLETTES	usrAimMmu.c
	INIT_RTN	usrAimMmuConfig ();
	REQUIRES	INCLUDE_MMU_BASIC
}

/* 
 * For VxWorks SMP and 64-bit VxWorks the MMU must be enabled; the SW_MMU_ENABLE 
 * parameter is not suppported. 
 */

Component INCLUDE_MMU_BASIC {
	NAME		basic MMU
	SYNOPSIS	basic MMU component
	SOURCE		$(TGT_DIR)/src/os/mm
	MODULES		vmBaseLib.o
	HDR_FILES	rebootLib.h private/vmLibP.h fioLib.h sysLib.h
#if (defined (_WRS_CONFIG_SMP) || defined (_WRS_CONFIG_LP64))
	_REQUIRES	INCLUDE_KERNEL
#else
	CFG_PARAMS	SW_MMU_ENABLE
#endif
	REQUIRES	INCLUDE_MMU_GLOBAL_MAP
}

Component INCLUDE_MMU_GLOBAL_MAP {
	NAME		MMU global map
	SYNOPSIS	MMU global map support
	SOURCE		$(TGT_DIR)/src/os/mm
	CONFIGLETTES	usrMmuInit.c
	MODULES		vmGlobalMap.o
        INIT_RTN        usrMmuInit ((VIRT_ADDR) pMemPoolStart, memPoolSize);
	HDR_FILES	rebootLib.h private/vmLibP.h fioLib.h sysLib.h
	CFG_PARAMS	VM_PAGE_SIZE		\
			USER_D_MMU_ENABLE	\
			USER_I_MMU_ENABLE
}

Component INCLUDE_PCI_PARAMS {
	NAME		PCI configuration
	SYNOPSIS	System assumes BSP supports PCI
	CFG_PARAMS	PCI_MSTR_MEM_SIZE	\
			PCI_MSTR_MEM_BUS	\
			PCI_MSTR_MEM_LOCAL	\
			PCI_MSTR_IO_SIZE	\
			PCI_MSTR_IO_BUS		\
			PCI_MSTR_IO_LOCAL	\
			PCI_MSTR_MEMIO_SIZE	\
			PCI_MSTR_MEMIO_BUS	\
			PCI_MSTR_MEMIO_LOCAL	\
			PCI_SLV_MEM_SIZE	\
			PCI_SLV_MEM_BUS		\
			PCI_SLV_MEM_LOCAL	\
			PCI_SLV_IO_SIZE		\
			PCI_SLV_IO_BUS		\
			PCI_SLV_IO_LOCAL	\
			PCI_SLV_MEMIO_SIZE	\
			PCI_SLV_MEMIO_BUS	\
			PCI_SLV_MEMIO_LOCAL
}

Component INCLUDE_VME_PARAMS {
	NAME		VME configuration
	SYNOPSIS	Systems assumes BSP supports VME
	CFG_PARAMS	VME_A16_MSTR_SIZE	\
			VME_A16_MSTR_BUS	\
			VME_A16_MSTR_LOCAL	\
			VME_A24_MSTR_SIZE	\
			VME_A24_MSTR_BUS	\
			VME_A24_MSTR_LOCAL	\
			VME_A32_MSTR_SIZE	\
			VME_A32_MSTR_BUS	\
			VME_A32_MSTR_LOCAL	\
			VME_A16_SLV_SIZE	\
			VME_A16_SLV_BUS		\
			VME_A16_SLV_LOCAL	\
			VME_A24_SLV_SIZE	\
			VME_A24_SLV_BUS		\
			VME_A24_SLV_LOCAL	\
			VME_A32_SLV_SIZE	\
			VME_A32_SLV_BUS		\
			VME_A32_SLV_LOCAL
}

Component INCLUDE_PCMCIA {
	NAME		PCMCIA
	SYNOPSIS	PCMCIA components
	CONFIGLETTES	usrPcmcia.c
	INIT_RTN	pcmciaInit ();
	HDR_FILES	drv/pcmcia/pcmciaLib.h
}

#ifndef	_WRS_CONFIG_SMP
Component INCLUDE_SCSI {
	NAME		scsi
	SYNOPSIS	SCSI components
	CONFIGLETTES	usrScsi.c
	MODULES		scsiLib.o
	INIT_RTN	usrScsiConfig ();
	HDR_FILES	sysLib.h stdio.h scsiLib.h

}

Selection SELECT_SCSI_VERSION {
	NAME		SCSI version
	COUNT		1-1
	CHILDREN	INCLUDE_SCSI1	\
			INCLUDE_SCSI2
	DEFAULTS	INCLUDE_SCSI2
}

Component INCLUDE_SCSI1 {
	NAME		SCSI 1
	SYNOPSIS	SCSI 1 components	
	MODULES		scsi1Lib.o
}

Component INCLUDE_SCSI2 {
	NAME		SCSI 2
	SYNOPSIS	SCSI 2 components
	MODULES		scsi2Lib.o
	MACRO_NEST	INCLUDE_SCSI
}
#endif	/* !_WRS_CONFIG_SMP */

Component INCLUDE_LPT {
	NAME		parallel port
	SYNOPSIS	Parallel port components
	MODULES		lptDrv.o
	INIT_RTN	lptDrv (LPT_CHANNELS, &lptResources[0]);
	HDR_FILES	drv/parallel/lptDrv.h
	CFG_PARAMS	LPT_CHANNELS
}

Component INCLUDE_FD {
	NAME		floppy drive (NEC 765)
	SYNOPSIS	NEC 765 floppy drive component
	MODULES		nec765Fd.o
	CONFIGLETTES	usrFd.c
	INIT_RTN	fdDrv (FD_INT_VEC, FD_INT_LVL);
	CFG_PARAMS	FD_INT_VEC FD_INT_LVL
	HDR_FILES	drv/fdisk/nec765Fd.h
}

Component INCLUDE_IDE {
	NAME		IDE hard drive
	SYNOPSIS	IDE hard drive component
	MODULES		ideDrv.o
	INIT_RTN	ideDrv (IDE_INT_VEC, IDE_INT_LVL, IDE_CONFIG);
	CFG_PARAMS	IDE_INT_VEC IDE_INT_LVL IDE_CONFIG
	HDR_FILES	drv/hdisk/ideDrv.h
}

Component INCLUDE_SYSCLK_INIT {
	NAME		System clock
	SYNOPSIS	System clock component
	CONFIGLETTES	sysClkInit.c
	HDR_FILES	tickLib.h
	INIT_RTN	sysClkInit ();
	CFG_PARAMS	SYS_CLK_RATE		\
			SYS_CLK_RATE_MIN	\
			SYS_CLK_RATE_MAX
}

Component INCLUDE_AUX_CLK {
	NAME		AUX clock
	SYNOPSIS	AUX clock component
	CFG_PARAMS	AUX_CLK_RATE_MIN	\
			AUX_CLK_RATE_MAX
}

Component INCLUDE_TIMER_RTC {
	NAME		RTC clock
	SYNOPSIS	RTC clock component
}

Component INCLUDE_TIMESTAMP {
	NAME		high resolution timestamping
	SYNOPSIS	BSP high resolution timestamp driver
	HDR_FILES	drv/timer/timerDev.h
#ifdef	_WRS_CONFIG_SMP
	INIT_RTN	sysSmpTimeBaseInit(0, 0);
	INIT_AFTER	INCLUDE_MMU_OPTIMIZE
	INIT_BEFORE	INCLUDE_USER_APPL
#endif	/* _WRS_CONFIG_SMP */
}

Component INCLUDE_TIMESTAMP64 {
	NAME		64-bit resolution timestamping
	SYNOPSIS	64-bit resolution timestamp driver
	REQUIRES	INCLUDE_TIMESTAMP
}	

Component INCLUDE_SIO {
	NAME		SIO
	SYNOPSIS	SIO component
	CONFIGLETTES	usrSerial.c
	INIT_RTN	usrSerialInit ();
	EXCLUDES	INCLUDE_TYCODRV_5_2
	CFG_PARAMS	NUM_TTY	CONSOLE_TTY CONSOLE_BAUD_RATE
	HDR_FILES	sysLib.h ttyLib.h stdio.h string.h ioLib.h
	REQUIRES	INCLUDE_TTY_DEV
}

Component INCLUDE_SIO_POLL {
	NAME		SIO_POLL
	SYNOPSIS	Polling mode only SIO component
	EXCLUDES	INCLUDE_TYCODRV_5_2
	CFG_PARAMS	NUM_TTY	SIO_POLL_CONSOLE CONSOLE_BAUD_RATE
}

Component INCLUDE_PC_CONSOLE {
	NAME		PC console
	SYNOPSIS	PC console component
	CONFIGLETTES	usrPcConsole.c
	INIT_RTN	usrPcConsoleInit ();
	HDR_FILES	sysLib.h drv/serial/pcConsole.h stdio.h
	CFG_PARAMS	N_VIRTUAL_CONSOLES PC_CONSOLE CONSOLE_BAUD_RATE
}

Folder FOLDER_FPP {
	NAME		floating point libraries
	CHILDREN	INCLUDE_SW_FP INCLUDE_HW_FP
}

Component INCLUDE_SW_FP {
	NAME		software fpp support
	MODULES		mathSoftLib.o
	INIT_RTN	mathSoftInit ();
	HDR_FILES	math.h
}

Component INCLUDE_HW_FP {
	NAME		hardware fpp support
	MODULES		mathHardLib.o
	INIT_RTN	mathHardInit ();
	HDR_FILES	math.h
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

Parameter LOCAL_MEM_LOCAL_ADRS {
	NAME		local memory address
	SYNOPSIS	The start of the on-board memory area
	DEFAULT		0x0
}

Parameter LOCAL_MEM_SIZE {
	NAME		local memory size
	SYNOPSIS	Fixed (static) memory size
	DEFAULT		0x00400000
}

Parameter LOCAL_MEM_AUTOSIZE {
	NAME		local memory Autosize
	SYNOPSIS	Run-time (dynamic) sizing
	TYPE		exists
	DEFAULT		FALSE
}

Parameter LOCAL_MEM_PHYS_ADRS {
	NAME            local memory physical base address
	SYNOPSIS        Base physical address for mapped base virtual address
	DEFAULT         0x20000000
}

Parameter LOCAL_UNMAPPED_BASE_ADRS {
	NAME            local memory unmapped base address
	SYNOPSIS        Base address for start of unmapped memory
	DEFAULT         0x82000000
}

Component INCLUDE_USER_RESERVED_MEMORY {
        NAME            user-reserved memory.
	SYNOPSIS	user-reserved memory
        SOURCE          $(TGT_DIR)/src/os/mm
	HDR_FILES	private/userReservedMemP.h
        MODULES         userReservedMem.o
        INIT_RTN        userReservedMemInit (USER_RESERVED_MEM, \
                                       CLEAR_USER_RESERVED_MEMORY_ON_COLD_BOOT);
#ifdef _WRS_CONFIG_LP64
        CFG_PARAMS	USER_RESERVED_MEM CLEAR_USER_RESERVED_MEMORY_ON_COLD_BOOT
#else
        CFG_PARAMS      CLEAR_USER_RESERVED_MEMORY_ON_COLD_BOOT
#endif /* _WRS_CONFIG_LP64 */
}

Parameter CLEAR_USER_RESERVED_MEMORY_ON_COLD_BOOT {
        NAME            clear user-reserved memory on reboot
        SYNOPSIS        Clears user-reserved memory on cold reboot
        DEFAULT         TRUE
}

Parameter USER_RESERVED_MEM {
	NAME		user reserved Memory
	SYNOPSIS	Reserved memory size ( bytes)
	DEFAULT		0x0
}

Parameter NV_RAM_SIZE {
	NAME		NVRAM size
	SYNOPSIS	NVRAM size (bytes)
	DEFAULT		0x100
}

Parameter NV_BOOT_OFFSET {
	NAME		NVRAM boot offset
	SYNOPSIS	NVRAM boot offset
	DEFAULT		0x0
}

Parameter NV_RAM_SIZE_WRITEABLE {
	NAME		NVRAM Writeable size
	SYNOPSIS	NVRAM Writeable size (bytes)
	DEFAULT		NV_RAM_SIZE
}

Component INCLUDE_NVRAM_FLASH {
    NAME            NVRAM to Flash mapping component
    SYNOPSIS        Use FLASH memory as non-volatile RAM
    _CHILDREN       FOLDER_PERIPHERALS
    CONFIGLETTES    usrFlashNvRam.c
}

Parameter VEC_BASE_ADRS {
	NAME		Vector base address
	SYNOPSIS	Vector base address
	DEFAULT		(LOCAL_MEM_LOCAL_ADRS + 0x0)
}

Parameter EXC_MSG_OFFSET {
	NAME		exception message offset
	SYNOPSIS	Exception message offset
	DEFAULT		0x800
}

Parameter EXC_MSG_ADRS {
	NAME		exception message address
	SYNOPSIS	Exception message address
	DEFAULT		(LOCAL_MEM_LOCAL_ADRS + EXC_MSG_OFFSET)
}

Parameter BOOT_LINE_SIZE {
	NAME		boot line size (bytes)
	SYNOPSIS	Boot line size (bytes)
	DEFAULT		0x100
}

Parameter BOOT_LINE_ADRS {
	NAME		Boot line address
	SYNOPSIS	Boot line address
	DEFAULT		(LOCAL_MEM_LOCAL_ADRS + BOOT_LINE_OFFSET)
}

Parameter BOOT_LINE_OFFSET {
	NAME		boot line offset
	SYNOPSIS	Boot line offset
	DEFAULT		0x700
}

Parameter DEFAULT_BOOT_LINE {
	NAME		default boot line
	SYNOPSIS	Default boot line string
	TYPE		string
	DEFAULT		"ei(0,0)host:/usr/vw/config/bsp/vxWorks \
                         h=90.0.0.3 e=90.0.0.50 u=target"
}

Parameter RESERVED {
	NAME		reserved
	SYNOPSIS	Reserved
	DEFAULT		0x400
}

Parameter FREE_RAM_ADRS {
	NAME		free RAM address
	SYNOPSIS	Free RAM starting address
	DEFAULT		(end)
}

Parameter ROM_WARM_ADRS {
	NAME		ROM warm boot address
	SYNOPSIS	ROM warm boot address
}

Parameter STACK_SAVE {
	NAME		stack size
	SYNOPSIS	Amount of stack to reserve (bytes) for rom code
	DEFAULT		0x40
}

Parameter USER_D_CACHE_MODE {
	NAME		data cache mode
	DEFAULT		CACHE_WRITETHROUGH
}

Parameter USER_I_CACHE_MODE {
	NAME		instruction cache mode
	DEFAULT		CACHE_WRITETHROUGH
}

Parameter USER_I_CACHE_ENABLE {
	NAME		enable instruction cache
	TYPE		exists
	DEFAULT		TRUE
}

Parameter USER_D_CACHE_ENABLE {
	NAME		enable data cache
	TYPE		exists
	DEFAULT		TRUE
}

Parameter PCI_MSTR_MEM_SIZE {
	NAME		PCI Master memory size
	SYNOPSIS	PCI Master window memory size (bytes)
	DEFAULT		(0x0)
}

Parameter PCI_MSTR_MEM_BUS {
	NAME		PCI Master memory bus address
	SYNOPSIS	PCI Master window memory bus address
	DEFAULT		(0x0)
}

Parameter PCI_MSTR_MEM_LOCAL {
	NAME		PCI Master memory local address
	SYNOPSIS	PCI Master window memory local address
	DEFAULT		(0x0)
}

Parameter PCI_MSTR_IO_SIZE {
	NAME		PCI Master IO size
	SYNOPSIS	PCI Master window IO size (bytes)
	DEFAULT		(0x0)
}

Parameter PCI_MSTR_IO_BUS {
	NAME		PCI Master IO bus address
	SYNOPSIS	PCI Master window IO bus address
	DEFAULT		(0x0)
}

Parameter PCI_MSTR_IO_LOCAL {
	NAME		PCI Master IO local address
	SYNOPSIS	PCI Master window IO local address
	DEFAULT		(0x0)
}

Parameter PCI_MSTR_MEMIO_SIZE {
	NAME		PCI Master non-prefetch memory size
	SYNOPSIS	PCI Master window non-prefetch memory size (bytes)
	DEFAULT		(0x0)
}

Parameter PCI_MSTR_MEMIO_BUS {
	NAME		PCI Master non-prefetch memory bus address
	SYNOPSIS	PCI Master window non-prefetch memory bus address
	DEFAULT		(0x0)
}

Parameter PCI_MSTR_MEMIO_LOCAL {
	NAME		PCI Master non-prefetch memory local address
	SYNOPSIS	PCI Master window non-prefetch memory local address
	DEFAULT		(0x0)
}

Parameter PCI_SLV_MEM_SIZE {
	NAME		PCI Slave memory size
	SYNOPSIS	PCI Slave window memory size (bytes)
	DEFAULT		(0x0)
}

Parameter PCI_SLV_MEM_BUS {
	NAME		PCI Slave memory bus address
	SYNOPSIS	PCI Slave window memory bus address
	DEFAULT		(0x0)
}

Parameter PCI_SLV_MEM_LOCAL {
	NAME		PCI Slave memory local address
	SYNOPSIS	PCI Slave memory window local address
	DEFAULT		(0x0)
}

Parameter PCI_SLV_IO_SIZE {
	NAME		PCI Slave IO size
	SYNOPSIS	PCI Slave window IO size (bytes)
	DEFAULT		(0x0)
}

Parameter PCI_SLV_IO_BUS {
	NAME		PCI Slave IO bus address
	SYNOPSIS	PCI Slave window IO bus address
	DEFAULT		(0x0)
}

Parameter PCI_SLV_IO_LOCAL {
	NAME		PCI Slave IO local address
	SYNOPSIS	PCI Slave window IO local address
	DEFAULT		(0x0)
}

Parameter PCI_SLV_MEMIO_SIZE {
	NAME		PCI Slave non-prefetch memory size
	SYNOPSIS	PCI Slave non-prefetch memory size (bytes)
	DEFAULT		(0x0)
}

Parameter PCI_SLV_MEMIO_BUS {
	NAME		PCI Slave non-prefetch memory bus address
	SYNOPSIS	PCI Slave window non-prefetch memory bus address
	DEFAULT		(0x0)
}

Parameter PCI_SLV_MEMIO_LOCAL {
	NAME		PCI Slave non-prefetch memory local address
	SYNOPSIS	PCI Slave window non-prefetch memory local address
	DEFAULT		(0x0)
}

Parameter VME_A16_MSTR_SIZE {
	NAME		VME A16 Master size
	SYNOPSIS	VME A16 Master window size (bytes)
	DEFAULT		(0x0)
}

Parameter VME_A16_MSTR_BUS {
	NAME		VME A16 Master bus address
	SYNOPSIS	VME A16 Master window bus address
	DEFAULT		(0x0)
}

Parameter VME_A16_MSTR_LOCAL {
	NAME		VME A16 Master local address
	SYNOPSIS	VME A16 Master window local address
	DEFAULT		(0x0)
}

Parameter VME_A24_MSTR_SIZE {
	NAME		VME A24 Master size
	SYNOPSIS	VME A24 Master window size (bytes)
	DEFAULT		(0x0)
}

Parameter VME_A24_MSTR_BUS {
	NAME		VME A24 Master bus address
	SYNOPSIS	VME A24 Master window bus address
	DEFAULT		(0x0)
}

Parameter VME_A24_MSTR_LOCAL {
	NAME		VME A24 Master local address
	SYNOPSIS	VME A24 Master window local address
	DEFAULT		(0x0)
}

Parameter VME_A32_MSTR_SIZE {
	NAME		VME A32 Master size
	SYNOPSIS	VME A32 Master window size (bytes)
	DEFAULT		(0x0)
}

Parameter VME_A32_MSTR_BUS {
	NAME		VME A32 Master bus address
	SYNOPSIS	VME A32 Master window bus address
	DEFAULT		(0x0)
}

Parameter VME_A32_MSTR_LOCAL {
	NAME		VME A32 Master local address
	SYNOPSIS	VME A32 Master window local address
	DEFAULT		(0x0)
}

Parameter VME_A16_SLV_SIZE {
	NAME		VME A16 Slave size
	SYNOPSIS	VME A16 Slave window size (bytes)
	DEFAULT		(0x0)
}

Parameter VME_A16_SLV_BUS {
	NAME		VME A16 Slave bus address
	SYNOPSIS	VME A16 Slave window bus address
	DEFAULT		(0x0)
}

Parameter VME_A16_SLV_LOCAL {
	NAME		VME A16 Slave local address
	SYNOPSIS	VME A16 Slave window local address
	DEFAULT		(0x0)
}

Parameter VME_A24_SLV_SIZE {
	NAME		VME A24 Slave size
	SYNOPSIS	VME A24 Slave window size (bytes)
	DEFAULT		(0x0)
}

Parameter VME_A24_SLV_BUS {
	NAME		VME A24 Slave bus address
	SYNOPSIS	VME A24 Slave window bus address
	DEFAULT		(0x0)
}

Parameter VME_A24_SLV_LOCAL {
	NAME		VME A24 Slave local address
	SYNOPSIS	VME A246 Slave window local address
	DEFAULT		(0x0)
}

Parameter VME_A32_SLV_SIZE {
	NAME		VME A32 Slave size
	SYNOPSIS	VME A32 Slave size (bytes)
	DEFAULT		(0x0)
}

Parameter VME_A32_SLV_BUS {
	NAME		VME A32 Slave bus address
	SYNOPSIS	VME A32 Slave window bus address
	DEFAULT		(0x0)
}

Parameter VME_A32_SLV_LOCAL {
	NAME		VME A32 Slave local address
	SYNOPSIS	VME A32 Slave window local address
	DEFAULT		(0x0)
}

Parameter LPT_CHANNELS {
	NAME		number of channels	
	SYNOPSIS	Number of parallel port channels
	TYPE		uint
}

Parameter FD_INT_VEC {
	NAME		interrupt vector
	SYNOPSIS	floppy drive interrupt vector
	TYPE		uint
}

Parameter FD_INT_LVL {
	NAME		interrupt level
	SYNOPSIS	Floppy drive interrupt level
	TYPE		uint
}

Parameter IDE_INT_VEC {
	NAME		interrupt vector
	SYNOPSIS	IDE interrupt vector
	TYPE		uint
}

Parameter IDE_INT_LVL {
	NAME		interrupt level
	SYNOPSIS	IDE interrupt level
	TYPE		uint
}

Parameter IDE_CONFIG {
	NAME		manual configuration
	SYNOPSIS	Manual configuration
	TYPE		bool
}

Parameter SYS_CLK_RATE_MIN {
	NAME		SYS clock rate min
	SYNOPSIS	Minimum system clock rate
	DEFAULT		(0x1)
}

Parameter SYS_CLK_RATE_MAX {
	NAME		SYS clock rate max
	SYNOPSIS	Maximum system clock rate
	DEFAULT		(1000)
}

Parameter SYS_CLK_RATE {
	NAME		# ticks per second
	TYPE		uint
	DEFAULT		60
}

Parameter AUX_CLK_RATE_MIN {
	NAME		AUX clock rate min
	SYNOPSIS	Minimum AUX clock rate
	DEFAULT		(0x1)
}

Parameter AUX_CLK_RATE_MAX {
	NAME		AUX clock rate max
	SYNOPSIS	Maximum AUX clock rate
	DEFAULT		(1000)
}

Parameter AUX_CLK_RATE {
	NAME		AUX clock rate default
	SYNOPSIS	Default AUX clock rate
	DEFAULT		(100)
}

Parameter NUM_TTY {
	NAME		number of serial ports
	TYPE		uint
	DEFAULT		2
}

Parameter CONSOLE_TTY {
	NAME		console serial port
	TYPE		uint
	DEFAULT		0
}

Parameter N_VIRTUAL_CONSOLES {
	DEFAULT		1
	TYPE		uint
}

Parameter PC_CONSOLE {
	DEFAULT		0
	TYPE		uint
}

Parameter CONSOLE_BAUD_RATE {
	NAME		baud rate of console port
	TYPE		uint
	DEFAULT		9600
}

Parameter RAM_LOW_ADRS {
	NAME		RAM low Address
	SYNOPSIS	The address at which to load VxWorks
	DEFAULT		0x01000
}

Parameter RAM_HIGH_ADRS {
	NAME		RAM high Address
	SYNOPSIS	The destination address used for copying \
			boot ROM image to RAM
	DEFAULT		0x020000
}

Parameter ROM_TEXT_ADRS {
	NAME		ROM text address
	SYNOPSIS	ROM text address
}

Parameter ROM_BASE_ADRS {
	NAME		ROM base address
	SYNOPSIS	ROM base address
}

Parameter ROM_SIZE {
	NAME		ROM size
	SYNOPSIS	ROM size
}

Parameter VM_PAGE_SIZE {
	NAME		VM page size
	SYNOPSIS	Virtual memory page size (bytes)
	DEFAULT		8192
}

#if (!defined (_WRS_CONFIG_SMP) && !defined (_WRS_CONFIG_LP64))
Parameter SW_MMU_ENABLE {
	NAME		Software MMU simulation
	SYNOPSIS	Enable software MMU simulation
	DEFAULT		FALSE
}
#endif

Parameter AIM_MMU_INIT_PT_NUM {
	NAME		AIM MMU page table pool init size
	SYNOPSIS	Sets the size of the mem pool for page tables
	DEFAULT		0x40
}

Parameter AIM_MMU_INIT_PT_INCR {
	NAME		AIM MMU page table pool init size
	SYNOPSIS	Sets the size of the increment when need more memory
	DEFAULT		0x20
}

Parameter AIM_MMU_INIT_RT_NUM {
	NAME		AIM MMU region table pool init size
	SYNOPSIS	Sets the size of the mem pool for region tables
	DEFAULT		0x10
}

Parameter AIM_MMU_INIT_RT_INCR {
	NAME		AIM MMU region table pool increment size
	SYNOPSIS	Sets the size of the increment when need more memory
	DEFAULT		0x10
}

Parameter SIO_POLL_CONSOLE {
	NAME		console polling only serial port 
	SYNOPSIS	Sets the polling only console port
	TYPE		uint
	DEFAULT		0
}
