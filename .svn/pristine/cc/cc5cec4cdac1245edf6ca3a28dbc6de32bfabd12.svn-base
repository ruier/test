/* 20bsp.cdf - BSP component description file */

/*
 * Copyright (c) 2007-2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
02b,29jul13,d_l  Remove default_rom/default_romResident support. (WIND00427646)
02a,28jan13,ylu  Added eSPI Controller component for REV 2/3 silicon.
01z,06apr12,h_k  added dependency from non core0 AMP to
                 INCLUDE_WRLOAD_IMAGE_BUILD.
                 removed INCLUDE_SYSTEM_GLOBALS. (WIND00341977)
01y,22sep11,ers  CQ253685:Move SYSTEM_GLOBALS for wrload SMP support.
01x,09aug11,ers  Replace DYNAMIC_WRLOAD with generic wrload support.
01w,27jul11,ers  CQ280006:INCLUDE_BOOT_RAM_IMAGE image fails. Restore 
                 RAM_LOW_ADRS to fix MMU exception. PROFILE_BOOTAPP now removes 
                 INCLUDE_MMU_BASIC to use TLB1's PHYS_MEM_START mapping.
01v,06jun11,pgh  Fix bootapp RAM image address space.
01u,25mar11,pch  CQ259341: INCLUDE_PCI_BUS requires DRV_PCIBUS_QORIQ_PCIEX,
		 not DRV_PCIBUS_M85XX
01t,07mar11,pch  INCLUDE_AMP_CPU REQUIRES INCLUDE_AMP
01s,13jan11,agf  CQ247891 restore LOCAL_MEM_SIZE definition now that QMAN and
		 BMAN have dedicated PTEs
01r,16dec10,pgh  CQ247891 change LOCAL_MEM_SIZE definition (work-around only)
01q,11nov10,pch  CQ238230 bootapp ROM build
01p,24aug10,kab  WIND000212298 - bootapp build
01o,26apr10,pmr  cache error handling options
01n,01mar10,pmr  WIND00201922: fix LOCAL_MEM_SIZE for non-AMP
01m,18feb10,mze  mod to MP_OPTIONS
01l,05jan10,pmr  SMP M-N, AMP
01k,13nov09,pmr  BSP name change
01j,28sep09,pmr  add LOCAL_MEM_ERPN
01i,10sep08,pmr  BSP name change
01h,30may08,pmr  removed AMP option
01g,24apr08,pmr  adapted for sim8578
01f,17jan08,dtr  Remove e500v2 option .
01e,19dec07,dtr  Support for AMP.
01d,28nov07,dtr  Add e500v2 FP type.
01c,20sep07,dtr  Fix for 8572.
01b,19jul07,ebh  Add AMP advertising option
01a,19jul07,pee  created
*/

Bsp fsl_p4080_ds {
	CPU		PPC32
	MP_OPTIONS	UP SMP
	ENDIAN		big
	FP		hard soft
	BUILD_SPECS	default_romCompress
	REQUIRES	INCLUDE_KERNEL \
			INCLUDE_VXBUS \
			DRV_TIMER_M85XX \
			INCLUDE_TIMER_SYS \
			INCLUDE_SIO_UTILS \
			DRV_INTCTLR_PPC \
			DRV_INTCTLR_EPIC \
			DRV_SIO_NS16550 \
			DRV_RESOURCE_FSLGPIO \
			DRV_RESOURCE_QORIQBMAN \
			DRV_RESOURCE_QORIQFMAN \
			DRV_RESOURCE_QORIQQMAN \
			DRV_RESOURCE_QORIQLAW
}

/* network boot devices for bootapp */

Component DRV_VXBEND_DTSEC {
	REQUIRES +=   DRV_MII_DTSEC_MDIO
}

Component INCLUDE_BOOT_NET_DEVICES {
	REQUIRES	DRV_VXBEND_DTSEC
}

/* enable M command in bootapp */

Component INCLUDE_BOOT_ETH_MAC_HANDLER {
	INCLUDE_WHEN += INCLUDE_BOOT_NETWORK
}

/* Local component dependencies, to match config.h */

Component INCLUDE_TFFS {
	REQUIRES +=	INCLUDE_TL_FTL INCLUDE_TFFS_MOUNT
}

/*
 * VX_SMP_NUM_CPUS is a SMP parameter only and only available for SMP
 * builds. Due to a limitation of the project tool at the time this
 * parameter is created where the tool can not recognize the ifdef SMP
 * selection, this parameter is set up such that _CFG_PARAMS is not
 * specified here. In the 00vxWorks.cdf file, where the parameter
 * VX_SMP_NUM_CPUS is defined, the _CFG_PARAMS is specified only for
 * VxWorks SMP. Hence the redefining of VX_SMP_NUM_CPUS here should only
 * override the value and not the rest of the properties. And for UP, the
 * parameter is ignored since the parameter is not tied to any component
 * (_CFG_PARAMS is not specified).
 */

Parameter VX_SMP_NUM_CPUS {
	NAME		Number of CPUs available to be enabled for VxWorks SMP
	TYPE		UINT
	DEFAULT		(INCLUDE_AMP)::(2) \
			8
}

/* AMP components */

Bundle BUNDLE_AMP_PRI {
	COMPONENTS	+= INCLUDE_AMP
}

Bundle BUNDLE_AMP_SEC {
	COMPONENTS	+= INCLUDE_AMP
}


/* 
 * The IPVR is set to LOCAL_MEM_LOCAL_ADRS, which creates an implicit 
 * requirement that RAM_LOW_ADRS be within the PPC branch distance to 
 * LOCAL_MEM_LOCAL_ADRS. Since that wouldn't always give enough space to 
 * download the run-time vxWorks image, LOCAL_MEM_LOCAL_ADRS is moved up in 
 * memory and the INCLUDE_MMU_BASIC component is removed from the default 
 * PROFILE_BOOTAPP. The resulting bootApp image can then execute with the IS/DS
 * bits of the MSR clear and use the global, physical memory mapping in the 
 * sysStaticTlb table for accessing memory.
 */

Profile PROFILE_BOOTAPP {
        COMPONENTS      -= INCLUDE_MMU_BASIC
}


Component INCLUDE_AMP {
	NAME		Base AMP Support
	REQUIRES	INCLUDE_AMP_CPU \
			INCLUDE_VXIPI \
			INCLUDE_VXCPULIB
	_CHILDREN	FOLDER_BSP_CONFIG
}

Component INCLUDE_AMP_CPU {
	NAME		AMP CPU Support
	SYNOPSIS	Per-cpu configuration for AMP
	REQUIRES	SELECT_AMP_CPU_NUMBER \
			INCLUDE_AMP
	_CHILDREN	FOLDER_BSP_CONFIG
}

/*
 * Override the default (empty) component definitions with custom options
 */
Component INCLUDE_AMP_CPU_07  {
	NAME		AMP kernel configured for core 07
	REQUIRES	INCLUDE_AMP_CPU		\
			INCLUDE_WRLOAD_IMAGE_BUILD
	_CHILDREN	SELECT_AMP_CPU_NUMBER
	CFG_PARAMS	AMP_CPU_NUMBER
}

Component INCLUDE_AMP_CPU_06  {
	NAME		AMP kernel configured for core 06
	REQUIRES	INCLUDE_AMP_CPU		\
			INCLUDE_WRLOAD_IMAGE_BUILD
	_CHILDREN	SELECT_AMP_CPU_NUMBER
	CFG_PARAMS	AMP_CPU_NUMBER
}

Component INCLUDE_AMP_CPU_05  {
	NAME		AMP kernel configured for core 05
	REQUIRES	INCLUDE_AMP_CPU		\
			INCLUDE_WRLOAD_IMAGE_BUILD
	_CHILDREN	SELECT_AMP_CPU_NUMBER
	CFG_PARAMS	AMP_CPU_NUMBER
}

Component INCLUDE_AMP_CPU_04  {
	NAME		AMP kernel configured for core 04
	REQUIRES	INCLUDE_AMP_CPU		\
			INCLUDE_WRLOAD_IMAGE_BUILD
	_CHILDREN	SELECT_AMP_CPU_NUMBER
	CFG_PARAMS	AMP_CPU_NUMBER
}

Component INCLUDE_AMP_CPU_03  {
	NAME		AMP kernel configured for core 03
	REQUIRES	INCLUDE_AMP_CPU		\
			INCLUDE_WRLOAD_IMAGE_BUILD
	_CHILDREN	SELECT_AMP_CPU_NUMBER
	CFG_PARAMS	AMP_CPU_NUMBER
}

Component INCLUDE_AMP_CPU_02  {
	NAME		AMP kernel configured for core 02
	REQUIRES	INCLUDE_AMP_CPU		\
			INCLUDE_WRLOAD_IMAGE_BUILD
	_CHILDREN	SELECT_AMP_CPU_NUMBER
	CFG_PARAMS	AMP_CPU_NUMBER
}

Component INCLUDE_AMP_CPU_01  {
	REQUIRES	INCLUDE_AMP_CPU		\
			INCLUDE_WRLOAD_IMAGE_BUILD
	CFG_PARAMS	AMP_CPU_NUMBER
}

Component INCLUDE_AMP_CPU_00  {
	REQUIRES	INCLUDE_AMP_CPU
	CFG_PARAMS	AMP_CPU_NUMBER
}

/*
 * WARNING: Care must be taken setting the memory definitions
 * for the P4080DS as there are multiple reserved spaces that
 * must be consistent between the ROM-able and the downloadable
 * kernels. Changing the below memory addresses may require
 * corresponding updates to config.h and/or reflashing of the
 * bootrom. Refer to the target.ref for details.
 */

Parameter RAM_HIGH_ADRS {
	NAME        vxWorks bootrom load address
	DEFAULT	    (INCLUDE_BOOT_RAM_IMAGE)::(0x11000000) \
		    (INCLUDE_BOOT_APP)::(0x02000000) \
		    (INCLUDE_AMP_CPU_07)::(0x1d000000) \
		    (INCLUDE_AMP_CPU_06)::(0x1b000000) \
		    (INCLUDE_AMP_CPU_05)::(0x19000000) \
		    (INCLUDE_AMP_CPU_04)::(0x17000000) \
		    (INCLUDE_AMP_CPU_03)::(0x15000000) \
		    (INCLUDE_AMP_CPU_02)::(0x13000000) \
		    (INCLUDE_AMP_CPU_01)::(0x11000000) \
		    (INCLUDE_AMP_CPU_00)::(0x01000000) \
		    0x01000000
}

Parameter RAM_LOW_ADRS {
	NAME        Runtime kernel load address
	DEFAULT     (INCLUDE_BOOT_RAM_IMAGE)::(0x10010000) \
		    (INCLUDE_BOOT_APP)::(0x01000000) \
		    (INCLUDE_AMP_CPU_07)::(0x1c010000) \
		    (INCLUDE_AMP_CPU_06)::(0x1a010000) \
		    (INCLUDE_AMP_CPU_05)::(0x18010000) \
		    (INCLUDE_AMP_CPU_04)::(0x16010000) \
		    (INCLUDE_AMP_CPU_03)::(0x14010000) \
		    (INCLUDE_AMP_CPU_02)::(0x12010000) \
		    (INCLUDE_AMP_CPU_01)::(0x10010000) \
		    (INCLUDE_AMP_CPU_00)::(0x00100000) \
		    0x00100000
}

Parameter LOCAL_MEM_LOCAL_ADRS {
	NAME        Runtime kernel lower memory address
	DEFAULT     (INCLUDE_BOOT_RAM_IMAGE)::(0x10000000) \
		    (INCLUDE_BOOT_APP)::(0x00000000) \
		    (INCLUDE_AMP_CPU_07)::(0x1c000000) \
		    (INCLUDE_AMP_CPU_06)::(0x1a000000) \
		    (INCLUDE_AMP_CPU_05)::(0x18000000) \
		    (INCLUDE_AMP_CPU_04)::(0x16000000) \
		    (INCLUDE_AMP_CPU_03)::(0x14000000) \
		    (INCLUDE_AMP_CPU_02)::(0x12000000) \
		    (INCLUDE_AMP_CPU_01)::(0x10000000) \
		    (INCLUDE_AMP_CPU_00)::(0x00000000) \
		    0x00000000
}

Parameter LOCAL_MEM_SIZE {
	NAME	     system memory size
	DEFAULT     (INCLUDE_BOOT_RAM_IMAGE)::(0x02000000) \
		    (INCLUDE_BOOT_APP)::(0x20000000) \
		    (INCLUDE_AMP_CPU_07)::(0x02000000) \
		    (INCLUDE_AMP_CPU_06)::(0x02000000) \
		    (INCLUDE_AMP_CPU_05)::(0x02000000) \
		    (INCLUDE_AMP_CPU_04)::(0x02000000) \
		    (INCLUDE_AMP_CPU_03)::(0x02000000) \
		    (INCLUDE_AMP_CPU_02)::(0x02000000) \
		    (INCLUDE_AMP_CPU_01)::(0x02000000) \
		    (INCLUDE_AMP_CPU_00)::(0x10000000) \
		    0x20000000
}

Parameter AMP_CPU_NUMBER {
	NAME	AMP CPU number
	DEFAULT     (INCLUDE_AMP_CPU_07)::(7) \
		    (INCLUDE_AMP_CPU_06)::(6) \
		    (INCLUDE_AMP_CPU_05)::(5) \
		    (INCLUDE_AMP_CPU_04)::(4) \
		    (INCLUDE_AMP_CPU_03)::(3) \
		    (INCLUDE_AMP_CPU_02)::(2) \
		    (INCLUDE_AMP_CPU_01)::(1) \
		    (INCLUDE_AMP_CPU_00)::(0) \
		    0
}

Parameter LOCAL_MEM_ERPN {
	NAME        	Runtime kernel physical address, upper 4 bits
	DEFAULT     	0x0
	_CFG_PARAMS	INCLUDE_MEMORY_CONFIG
}

Component INCLUDE_LOGICAL_PARTITION {
	NAME		Logical partition support
	CFG_PARAMS	LOGICAL_PARTITION_ID
	INCLUDE_WHEN	INCLUDE_AMP
	_CHILDREN	FOLDER_BSP_CONFIG
}

Parameter LOGICAL_PARTITION_ID {
	NAME        	Logical partition ID for this OS instance
	DEFAULT     	(INCLUDE_AMP_CPU_07)::(17) \
		    	(INCLUDE_AMP_CPU_06)::(16) \
		    	(INCLUDE_AMP_CPU_05)::(15) \
		    	(INCLUDE_AMP_CPU_04)::(14) \
		    	(INCLUDE_AMP_CPU_03)::(13) \
		    	(INCLUDE_AMP_CPU_02)::(12) \
		    	(INCLUDE_AMP_CPU_01)::(11) \
		    	(INCLUDE_AMP_CPU_00)::(1) \
		    	0
}

/* driver components */

Parameter TM_ANCHOR_ADRS {
	NAME   	  	MDIO anchor adrs
	_CFG_PARAMS	INCLUDE_TSEC_MDIO
	DEFAULT  	(0x4400 + 0x20000000)
}

Parameter XTSEC0_CORE_NUM {
	NAME   	  	tsec0 CPU assignment
	_CFG_PARAMS	INCLUDE_ETSEC_VXB_END
	DEFAULT  	0
}

Parameter XTSEC1_CORE_NUM {
	NAME   	  	tsec1 CPU assignment
	_CFG_PARAMS	INCLUDE_ETSEC_VXB_END
	DEFAULT  	1
}

Parameter XTSEC2_CORE_NUM {
	NAME   	  	tsec2 CPU assignment
	_CFG_PARAMS	INCLUDE_ETSEC_VXB_END
	DEFAULT  	2
}

Parameter XTSEC3_CORE_NUM {
	NAME   	  	tsec3 CPU assignment
	_CFG_PARAMS	INCLUDE_ETSEC_VXB_END
	DEFAULT  	3
}

Component INCLUDE_PCI_BUS {
	REQUIRES  	INCLUDE_PCI_BUS_AUTOCONF \
			DRV_PCIBUS_QORIQ_PCIEX
}

/* shared memory components */

Component INCLUDE_VIRTUAL_SM {
	NAME         	virtual SM region
	_CHILDREN	FOLDER_BSP_CONFIG
	CFG_PARAMS	VIRTUAL_SM_BASE \
			VIRTUAL_SM_BASE_PHYS \
			VIRTUAL_SM_SIZE
}

Parameter VIRTUAL_SM_BASE {
	NAME         	shared memory virtual address
	DEFAULT		(PHYS_MEM_START + PHYS_MEM_SIZE)
}

Parameter VIRTUAL_SM_BASE_PHYS {
	NAME         	shared memory physical address
	DEFAULT		0x20000000
}

Parameter VIRTUAL_SM_SIZE {
	NAME         	shared memory size
	DEFAULT		0x01000000
}

/* Cache error handling components */

Component INCLUDE_CACHE_ERROR_HANDLER {
	NAME		L1/L2/L3 cache error handler
	_CHILDREN	FOLDER_BSP_CONFIG
}

Component INCLUDE_L1_CACHE_ERROR_RECOVERY {
	NAME		L1 cache error recovery
	REQUIRES	INCLUDE_CACHE_ERROR_HANDLER
	INCLUDE_WHEN	INCLUDE_CACHE_ERROR_HANDLER
	CFG_PARAMS	L1_CACHE_ERROR_INJECTION
	_CHILDREN	FOLDER_BSP_CONFIG
}

Parameter L1_CACHE_ERROR_INJECTION {
	NAME		include L1 cache error injection code
	TYPE		exists
	DEFAULT		FALSE
}

Component INCLUDE_L2_CACHE_ERROR_RECOVERY {
	NAME		L2 cache error recovery
	REQUIRES	INCLUDE_CACHE_ERROR_HANDLER
	CFG_PARAMS	L2_CACHE_ERROR_INJECTION \
			L2_CACHE_ERROR_INT_ENABLE \
			L2_CACHE_ERROR_L2CTHRESH
	_CHILDREN	FOLDER_BSP_CONFIG
}

Parameter L2_CACHE_ERROR_INJECTION {
	NAME		include L2 cache error injection code
	TYPE		exists
	DEFAULT		FALSE
}

Parameter L2_CACHE_ERROR_INT_ENABLE {
	NAME		L2 cache error interrupt enable setting
	DEFAULT		0x9e
}

Parameter L2_CACHE_ERROR_L2CTHRESH {
	NAME		L2 cache error threshold setting (L2ERRCTL)
	DEFAULT		0x0
}

Component INCLUDE_L3_CACHE_ERROR_RECOVERY {
	NAME		L3 cache error recovery
	REQUIRES	INCLUDE_CACHE_ERROR_HANDLER
	CFG_PARAMS	L3_CACHE_ERROR_INJECTION \
			L3_CACHE_ERROR_INT_ENABLE \
			L3_CACHE_ERROR_CPCTHRESH
	_CHILDREN	FOLDER_BSP_CONFIG
}

Parameter L3_CACHE_ERROR_INJECTION {
	NAME		include L3 cache error injection code
	TYPE		exists
	DEFAULT		FALSE
}

Parameter L3_CACHE_ERROR_INT_ENABLE {
	NAME		L3 cache error interrupt enable setting
	DEFAULT		0xec
}

Parameter L3_CACHE_ERROR_CPCTHRESH {
	NAME		L3 cache error threshold setting (CPCERRCTL)
	DEFAULT		0x0
}

/* Freescale eSPI Controller Driver */

Component INCLUDE_FSL_SPI {
    NAME        SPI Bus Controller Driver
    SYNOPSIS    Freescale eSPI Controller Driver
    _CHILDREN   FOLDER_DRIVERS
}

/* Spansion SPI flash driver */

Component DRV_SPI_S25FLXX {
    NAME        Spansion SPI Flash Driver
    SYNOPSIS    Spansion S25FLXX serial flash driver
    REQUIRES    INCLUDE_FSL_SPI
    _CHILDREN   FOLDER_DRIVERS
}
