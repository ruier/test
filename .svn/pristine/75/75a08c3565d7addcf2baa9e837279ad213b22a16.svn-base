/* 20bsp.cdf - BSP component description file */

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
01r,08may13,h_k  removed unnecessary bootAppRam specific default LOCAL_MEM_SIZE
                 parameter setting. (WIND00416892)
01q,25feb13,h_k  moved INCLUDE_SMP_SCHED_SMT_POLICY and INCLUDE_SMT component
                 definitions to arch.cdf.
01p,23jan13,wap  Add DTE phy driver dependency to MEMAC (for 10GbE ports)
01o,22jan13,x_s  added dependency rule for USB.
01n,21jan13,x_z  added dependency rules for NVRAM/TFFS.
01m,17jan13,zl   added thread support (SMT) component
01l,16jan13,wap  Add Vitesse VSC3316 driver and dependencies
01k,10jan13,wyt  add REQUIRES INCLUDE_USR_MACADDR
01j,04dec12,h_k  changed the actual HW as the default.
01i,30nov12,x_s  added component INCLUDE_CORE_E6500 by default.
01h,02nov12,x_s  removed L2&L3 CACHE components to ARCH.
01g,01nov12,h_k  updated mem configuration for AMP.
01f,18oct12,syt  added INCLUDE_PCI_BUS dependent components.
01e,09oct12,h_k  added SELECT_QORIQMEMAC_MDIO.
01d,14sep12,h_k  removed TM_ANCHOR_ADRS.
01c,06sep12,h_k  changed the default NIC to memac.
                 added budles for 3 AMPs, 6 AMPs and 12 AMPs configurations.
01b,05sep12,h_k  updated default L2_CACHE_ERROR_INT_ENABLE parameter.
01a,07may12,h_k  created from fsl_p5020_ds 01g and ads8572 01n.
*/

Bsp fsl_t4240_qds {
	CPU		PPC32
	MP_OPTIONS	UP SMP
	ENDIAN		big
	FP		hard soft
	REQUIRES	INCLUDE_KERNEL		\
			INCLUDE_CORE_E6500	\
			DRV_INTCTLR_PPC		\
			DRV_INTCTLR_EPIC	\
			DRV_TIMER_M85XX		\
			DRV_SIO_NS16550
}

/* use openPicTimer as the auxClk */

Component DRV_TIMER_OPENPIC {
	INCLUDE_WHEN	+= INCLUDE_AUX_CLK
}

/* network boot devices for bootapp */

Component INCLUDE_BOOT_NET_DEVICES {
	REQUIRES	DRV_VXBEND_QORIQMEMAC
}

/* enable M command in bootapp */

Component INCLUDE_BOOT_ETH_MAC_HANDLER {
	INCLUDE_WHEN	+= INCLUDE_BOOT_NETWORK
}

/* Use Flash NVRAM for boot line and MAC address */

Component INCLUDE_BOOT_LINE_INIT {
        REQUIRES        += INCLUDE_NVRAM_FLASH
}

Component INCLUDE_USR_MACADDR {
        REQUIRES        += INCLUDE_NVRAM_FLASH
}

/* Use NOR Flash for NVRAM and TFFS */

Component INCLUDE_NVRAM_FLASH {
        REQUIRES        += DRV_VXB_NORFLASH \
                           DRV_VXB_NORFLASH_CMDSET_AMD
}

Component INCLUDE_TFFS_STUB_VXBFLASH {
        REQUIRES        += DRV_VXB_NORFLASH \
                           DRV_VXB_NORFLASH_CMDSET_AMD
}

Component INCLUDE_TFFS {
        REQUIRES        += INCLUDE_TFFS_STUB_VXBFLASH
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
	DEFAULT		(INCLUDE_3AMP_CONFIG)::(SMP_3AMP_NUM_CPUS)	\
			(INCLUDE_6AMP_CONFIG)::(SMP_6AMP_NUM_CPUS)	\
			(INCLUDE_12AMP_CONFIG)::(SMP_12AMP_NUM_CPUS)	\
			(INCLUDE_SMT)::(24) 12
}

Parameter SMP_3AMP_NUM_CPUS {
	NAME		Number of CPUs available for 3 AMP configuration
	TYPE		UINT
	DEFAULT		(INCLUDE_SMT)::(8) 4
}

Parameter SMP_6AMP_NUM_CPUS {
	NAME		Number of CPUs available for 6 AMP configuration
	TYPE		UINT
	DEFAULT		(INCLUDE_SMT)::(4) 2
}

Parameter SMP_12AMP_NUM_CPUS {
	NAME		Number of CPUs available for 12 AMP configuration
	TYPE		UINT
	DEFAULT		(INCLUDE_SMT)::(2) 1
}

/* AMP components */

Bundle BUNDLE_AMP_PRI {
	COMPONENTS	+= INCLUDE_AMP_CPU_00		\
			   INCLUDE_MIPC_SHOW		\
			   SELECT_AMP_NUMBER_CONFIG
}

Bundle BUNDLE_AMP_SEC {
	COMPONENTS	+= INCLUDE_AMP_CPU_04		\
			   SELECT_AMP_NUMBER_CONFIG
}

Bundle BUNDLE_AMP_TER {
	NAME		AMP Tertiary OS bundle
	SYNOPSIS	Kernel components required to build an AMP tertiary OS that can be loaded via wrload from a primary OS in a multicore AMP configuration.  The resulting tertiary OS image uses MIPC to interact with the primary OS and includes WDB over MIPC support and MIPC serial device support.
	COMPONENTS	INCLUDE_WDB_COMM_MIPC		\
			INCLUDE_WRLOAD_IMAGE_BUILD	\
			INCLUDE_MSD			\
			INCLUDE_AMP_CPU_08		\
			SELECT_AMP_NUMBER_CONFIG
}

Bundle BUNDLE_AMP_PRI_OF_06 {
	NAME		AMP Primary OS of 6AMPs bundle
	SYNOPSIS	Kernel components required to build an AMP primary OS of 6 AMPs that can load and interact with AMP secondary OSs in a multicore processor used in an AMP configuration.  The primary OS provides wrload to load secondary OSs and uses MIPC to communciate with the secondary OSs.  It also provides WDB proxy support so that Workbench tools can connect through the primary OS to the secondary OSs.  In addition, MIPC serial devices are provided to enable the tip command to be used to connect to the secondary OSs.
	COMPONENTS	INCLUDE_MSD			\
			INCLUDE_WDB_PROXY_MIPC		\
			INCLUDE_WRLOAD			\
			INCLUDE_AMP_CPU_00		\
			INCLUDE_MIPC_SHOW		\
			INCLUDE_6AMP_CONFIG
}

Bundle BUNDLE_AMP_SEC_OF_06 {
	NAME		AMP Secondary OS of 6AMPs bundle
	SYNOPSIS	Kernel components required to build an AMP secondary OS of 6 AMPs that can be loaded via wrload from a primary OS in a multicore AMP configuration.  The resulting secondary OS image uses MIPC to interact with the primary OS and includes WDB over MIPC support and MIPC serial device support.
	COMPONENTS	INCLUDE_WDB_COMM_MIPC		\
			INCLUDE_WRLOAD_IMAGE_BUILD	\
			INCLUDE_MSD			\
			INCLUDE_AMP_CPU_02		\
			INCLUDE_6AMP_CONFIG
}

Bundle BUNDLE_AMP_TER_OF_06 {
	NAME		AMP Tertiary OS of 6AMPs bundle
	SYNOPSIS	Kernel components required to build an AMP tertiary OS of 6 AMPs that can be loaded via wrload from a primary OS in a multicore AMP configuration.  The resulting tertiary OS image uses MIPC to interact with the primary OS and includes WDB over MIPC support and MIPC serial device support.
	COMPONENTS	INCLUDE_WDB_COMM_MIPC		\
			INCLUDE_WRLOAD_IMAGE_BUILD	\
			INCLUDE_MSD			\
			INCLUDE_AMP_CPU_04		\
			INCLUDE_6AMP_CONFIG
}

Bundle BUNDLE_AMP_QUA_OF_06 {
	NAME		AMP Quaternary OS of 6AMPs bundle
	SYNOPSIS	Kernel components required to build an AMP quaternary OS of 6 AMPs that can be loaded via wrload from a primary OS in a multicore AMP configuration.  The resulting quaternary OS image uses MIPC to interact with the primary OS and includes WDB over MIPC support and MIPC serial device support.
	COMPONENTS	INCLUDE_WDB_COMM_MIPC		\
			INCLUDE_WRLOAD_IMAGE_BUILD	\
			INCLUDE_MSD			\
			INCLUDE_AMP_CPU_06		\
			INCLUDE_6AMP_CONFIG
}

Bundle BUNDLE_AMP_QUI_OF_06 {
	NAME		AMP Quinary OS of 6AMPs bundle
	SYNOPSIS	Kernel components required to build an AMP quinary OS of 6 AMPs that can be loaded via wrload from a primary OS in a multicore AMP configuration.  The resulting quinary OS image uses MIPC to interact with the primary OS and includes WDB over MIPC support and MIPC serial device support.
	COMPONENTS	INCLUDE_WDB_COMM_MIPC		\
			INCLUDE_WRLOAD_IMAGE_BUILD	\
			INCLUDE_MSD			\
			INCLUDE_AMP_CPU_08		\
			INCLUDE_6AMP_CONFIG
}

Bundle BUNDLE_AMP_SEN_OF_06 {
	NAME		AMP Senary OS of 6AMPs bundle
	SYNOPSIS	Kernel components required to build an AMP senary OS of 6 AMPs that can be loaded via wrload from a primary OS in a multicore AMP configuration.  The resulting senary OS image uses MIPC to interact with the primary OS and includes WDB over MIPC support and MIPC serial device support.
	COMPONENTS	INCLUDE_WDB_COMM_MIPC		\
			INCLUDE_WRLOAD_IMAGE_BUILD	\
			INCLUDE_MSD			\
			INCLUDE_AMP_CPU_10		\
			INCLUDE_6AMP_CONFIG
}

Bundle BUNDLE_AMP_PRI_OF_12 {
	NAME		AMP Primary OS of 12AMPs bundle
	SYNOPSIS	Kernel components required to build an AMP primary OS of 12 AMPs that can load and interact with AMP secondary OSs in a multicore processor used in an AMP configuration.  The primary OS provides wrload to load secondary OSs and uses MIPC to communciate with the secondary OSs.  It also provides WDB proxy support so that Workbench tools can connect through the primary OS to the secondary OSs.  In addition, MIPC serial devices are provided to enable the tip command to be used to connect to the secondary OSs.
	COMPONENTS	INCLUDE_MSD			\
			INCLUDE_WDB_PROXY_MIPC		\
			INCLUDE_WRLOAD			\
			INCLUDE_AMP_CPU_00		\
			INCLUDE_12AMP_CONFIG		\
			INCLUDE_MIPC_SHOW
}

Bundle BUNDLE_AMP_SEC_OF_12 {
	NAME		AMP Secondary OS of 12AMPs bundle
	SYNOPSIS	Kernel components required to build an AMP secondary OS of 12 AMPs that can be loaded via wrload from a primary OS in a multicore AMP configuration.  The resulting secondary OS image uses MIPC to interact with the primary OS and includes WDB over MIPC support and MIPC serial device support.
	COMPONENTS	INCLUDE_WDB_COMM_MIPC		\
			INCLUDE_WRLOAD_IMAGE_BUILD	\
			INCLUDE_MSD			\
			INCLUDE_12AMP_CONFIG		\
			INCLUDE_AMP_CPU_01
}

Bundle BUNDLE_AMP_TER_OF_12 {
	NAME		AMP Tertiary OS of 12AMPs bundle
	SYNOPSIS	Kernel components required to build an AMP tertiary OS of 12 AMPs that can be loaded via wrload from a primary OS in a multicore AMP configuration.  The resulting tertiary OS image uses MIPC to interact with the primary OS and includes WDB over MIPC support and MIPC serial device support.
	COMPONENTS	INCLUDE_WDB_COMM_MIPC		\
			INCLUDE_WRLOAD_IMAGE_BUILD	\
			INCLUDE_MSD			\
			INCLUDE_12AMP_CONFIG		\
			INCLUDE_AMP_CPU_02
}

Bundle BUNDLE_AMP_QUA_OF_12 {
	NAME		AMP Quaternary OS of 12AMPs bundle
	SYNOPSIS	Kernel components required to build an AMP quaternary OS of 12 AMPs that can be loaded via wrload from a primary OS in a multicore AMP configuration.  The resulting quaternary OS image uses MIPC to interact with the primary OS and includes WDB over MIPC support and MIPC serial device support.
	COMPONENTS	INCLUDE_WDB_COMM_MIPC		\
			INCLUDE_WRLOAD_IMAGE_BUILD	\
			INCLUDE_MSD			\
			INCLUDE_12AMP_CONFIG		\
			INCLUDE_AMP_CPU_03
}

Bundle BUNDLE_AMP_QUI_OF_12 {
	NAME		AMP Quinary OS of 12AMPs bundle
	SYNOPSIS	Kernel components required to build an AMP quinary OS of 12 AMPs that can be loaded via wrload from a primary OS in a multicore AMP configuration.  The resulting quinary OS image uses MIPC to interact with the primary OS and includes WDB over MIPC support and MIPC serial device support.
	COMPONENTS	INCLUDE_WDB_COMM_MIPC		\
			INCLUDE_WRLOAD_IMAGE_BUILD	\
			INCLUDE_MSD			\
			INCLUDE_12AMP_CONFIG		\
			INCLUDE_AMP_CPU_04
}

Bundle BUNDLE_AMP_SEN_OF_12 {
	NAME		AMP Senary OS of 12AMPs bundle
	SYNOPSIS	Kernel components required to build an AMP senary OS of 12 AMPs that can be loaded via wrload from a primary OS in a multicore AMP configuration.  The resulting senary OS image uses MIPC to interact with the primary OS and includes WDB over MIPC support and MIPC serial device support.
	COMPONENTS	INCLUDE_WDB_COMM_MIPC		\
			INCLUDE_WRLOAD_IMAGE_BUILD	\
			INCLUDE_MSD			\
			INCLUDE_12AMP_CONFIG		\
			INCLUDE_AMP_CPU_05
}

Bundle BUNDLE_AMP_SEP_OF_12 {
	NAME		AMP Septenary OS of 12AMPs bundle
	SYNOPSIS	Kernel components required to build an AMP septenary OS of 12 AMPs that can be loaded via wrload from a primary OS in a multicore AMP configuration.  The resulting septenary OS image uses MIPC to interact with the primary OS and includes WDB over MIPC support and MIPC serial device support.
	COMPONENTS	INCLUDE_WDB_COMM_MIPC		\
			INCLUDE_WRLOAD_IMAGE_BUILD	\
			INCLUDE_MSD			\
			INCLUDE_12AMP_CONFIG		\
			INCLUDE_AMP_CPU_06
}

Bundle BUNDLE_AMP_OCT_OF_12 {
	NAME		AMP Octonary OS of 12AMPs bundle
	SYNOPSIS	Kernel components required to build an AMP octonary OS of 12 AMPs that can be loaded via wrload from a primary OS in a multicore AMP configuration.  The resulting octonary OS image uses MIPC to interact with the primary OS and includes WDB over MIPC support and MIPC serial device support.
	COMPONENTS	INCLUDE_WDB_COMM_MIPC		\
			INCLUDE_WRLOAD_IMAGE_BUILD	\
			INCLUDE_MSD			\
			INCLUDE_12AMP_CONFIG		\
			INCLUDE_AMP_CPU_07
}

Bundle BUNDLE_AMP_NON_OF_12 {
	NAME		AMP Nonary OS of 12AMPs bundle
	SYNOPSIS	Kernel components required to build an AMP nonary OS of 12 AMPs that can be loaded via wrload from a primary OS in a multicore AMP configuration.  The resulting noenary OS image uses MIPC to interact with the primary OS and includes WDB over MIPC support and MIPC serial device support.
	COMPONENTS	INCLUDE_WDB_COMM_MIPC		\
			INCLUDE_WRLOAD_IMAGE_BUILD	\
			INCLUDE_MSD			\
			INCLUDE_12AMP_CONFIG		\
			INCLUDE_AMP_CPU_08
}

Bundle BUNDLE_AMP_DEN_OF_12 {
	NAME		AMP Senary OS of 12AMPs bundle
	SYNOPSIS	Kernel components required to build an AMP senary OS of 12 AMPs that can be loaded via wrload from a primary OS in a multicore AMP configuration.  The resulting senary OS image uses MIPC to interact with the primary OS and includes WDB over MIPC support and MIPC serial device support.
	COMPONENTS	INCLUDE_WDB_COMM_MIPC		\
			INCLUDE_WRLOAD_IMAGE_BUILD	\
			INCLUDE_MSD			\
			INCLUDE_12AMP_CONFIG		\
			INCLUDE_AMP_CPU_09
}

Bundle BUNDLE_AMP_ELE_OF_12 {
	NAME		AMP Eleventh OS of 12AMPs bundle
	SYNOPSIS	Kernel components required to build an AMP eleventh OS of 12 AMPs that can be loaded via wrload from a primary OS in a multicore AMP configuration.  The resulting eleventh OS image uses MIPC to interact with the primary OS and includes WDB over MIPC support and MIPC serial device support.
	COMPONENTS	INCLUDE_WDB_COMM_MIPC		\
			INCLUDE_WRLOAD_IMAGE_BUILD	\
			INCLUDE_MSD			\
			INCLUDE_12AMP_CONFIG		\
			INCLUDE_AMP_CPU_10
}

Bundle BUNDLE_AMP_DUO_OF_12 {
	NAME		AMP Duodenary OS of 12AMPs bundle
	SYNOPSIS	Kernel components required to build an AMP duodenary OS of 12 AMPs that can be loaded via wrload from a primary OS in a multicore AMP configuration.  The resulting duodenary OS image uses MIPC to interact with the primary OS and includes WDB over MIPC support and MIPC serial device support.
	COMPONENTS	INCLUDE_WDB_COMM_MIPC		\
			INCLUDE_WRLOAD_IMAGE_BUILD	\
			INCLUDE_MSD			\
			INCLUDE_12AMP_CONFIG		\
			INCLUDE_AMP_CPU_11
}

Component INCLUDE_AMP {
	NAME		Base AMP Support
	REQUIRES	INCLUDE_VXIPI		\
			INCLUDE_VXCPULIB	\
			SELECT_AMP_CPU_NUMBER
	_CHILDREN	FOLDER_BSP_CONFIG
}

Selection SELECT_AMP_NUMBER_CONFIG {
	NAME		Total AMP number configuration
	COUNT		1-1
	REQUIRES	INCLUDE_AMP
	_CHILDREN	FOLDER_BSP_CONFIG
	DEFAULTS	INCLUDE_3AMP_CONFIG
	CHILDREN	INCLUDE_3AMP_CONFIG	\
			INCLUDE_6AMP_CONFIG	\
			INCLUDE_12AMP_CONFIG
}

Component INCLUDE_3AMP_CONFIG {
	NAME		Up to three AMPs Support Example Configuration
	CFG_PARAMS	SMP_3AMP_NUM_CPUS
}

Component INCLUDE_6AMP_CONFIG {
	NAME		Up to six AMPs Support Example Configuration
	CFG_PARAMS	SMP_6AMP_NUM_CPUS
}

Component INCLUDE_12AMP_CONFIG {
	NAME		Up to twelve AMPs Support Example Configuration
	CFG_PARAMS	SMP_12AMP_NUM_CPUS
}

Component INCLUDE_AMP_CPU_00  {
	NAME		AMP kernel configured for core 00
	REQUIRES	INCLUDE_AMP
	CFG_PARAMS	AMP_CPU_NUMBER
}

Component INCLUDE_AMP_CPU_01 {
	NAME		AMP kernel configured for core 01
	REQUIRES	INCLUDE_AMP
	CFG_PARAMS	AMP_CPU_NUMBER
}

Component INCLUDE_AMP_CPU_02 {
	NAME		AMP kernel configured for core 02
	REQUIRES	INCLUDE_AMP
	_CHILDREN	SELECT_AMP_CPU_NUMBER
	CFG_PARAMS	AMP_CPU_NUMBER
}

Component INCLUDE_AMP_CPU_03 {
	NAME		AMP kernel configured for core 03
	REQUIRES	INCLUDE_AMP
	_CHILDREN	SELECT_AMP_CPU_NUMBER
	CFG_PARAMS	AMP_CPU_NUMBER
}

Component INCLUDE_AMP_CPU_04 {
	NAME		AMP kernel configured for core 04
	REQUIRES	INCLUDE_AMP
	_CHILDREN	SELECT_AMP_CPU_NUMBER
	CFG_PARAMS	AMP_CPU_NUMBER
}

Component INCLUDE_AMP_CPU_05 {
	NAME		AMP kernel configured for core 05
	REQUIRES	INCLUDE_AMP
	_CHILDREN	SELECT_AMP_CPU_NUMBER
	CFG_PARAMS	AMP_CPU_NUMBER
}

Component INCLUDE_AMP_CPU_06 {
	NAME		AMP kernel configured for core 06
	REQUIRES	INCLUDE_AMP
	_CHILDREN	SELECT_AMP_CPU_NUMBER
	CFG_PARAMS	AMP_CPU_NUMBER
}

Component INCLUDE_AMP_CPU_07 {
	NAME		AMP kernel configured for core 07
	REQUIRES	INCLUDE_AMP
	_CHILDREN	SELECT_AMP_CPU_NUMBER
	CFG_PARAMS	AMP_CPU_NUMBER
}

Component INCLUDE_AMP_CPU_08 {
	NAME		AMP kernel configured for core 08
	REQUIRES	INCLUDE_AMP
	_CHILDREN	SELECT_AMP_CPU_NUMBER
	CFG_PARAMS	AMP_CPU_NUMBER
}

Component INCLUDE_AMP_CPU_09 {
	NAME		AMP kernel configured for core 09
	REQUIRES	INCLUDE_AMP
	_CHILDREN	SELECT_AMP_CPU_NUMBER
	CFG_PARAMS	AMP_CPU_NUMBER
}

Component INCLUDE_AMP_CPU_10 {
	NAME		AMP kernel configured for core 10
	REQUIRES	INCLUDE_AMP
	_CHILDREN	SELECT_AMP_CPU_NUMBER
	CFG_PARAMS	AMP_CPU_NUMBER
}

Component INCLUDE_AMP_CPU_11 {
	NAME		AMP kernel configured for core 11
	REQUIRES	INCLUDE_AMP
	_CHILDREN	SELECT_AMP_CPU_NUMBER
	CFG_PARAMS	AMP_CPU_NUMBER
}

Parameter AMP_CPU_NUMBER {
	NAME		AMP CPU number
	DEFAULT		(INCLUDE_AMP_CPU_11)::(22)	\
			(INCLUDE_AMP_CPU_10)::(20)	\
			(INCLUDE_AMP_CPU_09)::(18)	\
			(INCLUDE_AMP_CPU_08)::(16)	\
			(INCLUDE_AMP_CPU_07)::(14)	\
			(INCLUDE_AMP_CPU_06)::(12)	\
			(INCLUDE_AMP_CPU_05)::(10)	\
			(INCLUDE_AMP_CPU_04)::(8)	\
			(INCLUDE_AMP_CPU_03)::(6)	\
			(INCLUDE_AMP_CPU_02)::(4)	\
			(INCLUDE_AMP_CPU_01)::(2)	\
			0
}

Parameter RAM_HIGH_ADRS {
	NAME		VxWorks bootrom load address
	DEFAULT		(INCLUDE_BOOT_RAM_IMAGE)::(0x01800000)	\
			(INCLUDE_BOOT_APP)::(0x01E00000)	\
			(INCLUDE_AMP_CPU_11)::(0x31000000)	\
			(INCLUDE_AMP_CPU_10)::(0x21000000)	\
			(INCLUDE_AMP_CPU_09)::(0x11000000)	\
			(INCLUDE_AMP_CPU_08)::(0x41000000)	\
			(INCLUDE_AMP_CPU_07)::(0x31000000)	\
			(INCLUDE_AMP_CPU_06)::(0x21000000)	\
			(INCLUDE_AMP_CPU_05)::(0x11000000)	\
			(INCLUDE_AMP_CPU_04)::(0x41000000)	\
			(INCLUDE_AMP_CPU_03)::(0x31000000)	\
			(INCLUDE_AMP_CPU_02)::(0x21000000)	\
			(INCLUDE_AMP_CPU_01)::(0x11000000)	\
			0x01000000
}

Parameter RAM_LOW_ADRS {
	NAME		Runtime kernel load address
	DEFAULT		(INCLUDE_BOOT_RAM_IMAGE)::(0x01000000)	\
			(INCLUDE_BOOT_APP)::(0x01800000)	\
			(INCLUDE_AMP_CPU_11)::(0x30100000)	\
			(INCLUDE_AMP_CPU_10)::(0x20100000)	\
			(INCLUDE_AMP_CPU_09)::(0x10100000)	\
			(INCLUDE_AMP_CPU_08)::(0x40100000)	\
			(INCLUDE_AMP_CPU_07)::(0x30100000)	\
			(INCLUDE_AMP_CPU_06)::(0x20100000)	\
			(INCLUDE_AMP_CPU_05)::(0x10100000)	\
			(INCLUDE_AMP_CPU_04)::(0x40100000)	\
			(INCLUDE_AMP_CPU_03)::(0x30100000)	\
			(INCLUDE_AMP_CPU_02)::(0x20100000)	\
			(INCLUDE_AMP_CPU_01)::(0x10100000)	\
			0x00100000
}

Parameter LOCAL_MEM_LOCAL_ADRS {
	NAME		Runtime kernel lower memory address
	DEFAULT		(INCLUDE_AMP_CPU_11)::(0x30000000)	\
			(INCLUDE_AMP_CPU_10)::(0x20000000)	\
			(INCLUDE_AMP_CPU_09)::(0x10000000)	\
			(INCLUDE_AMP_CPU_08)::(0x40000000)	\
			(INCLUDE_AMP_CPU_07)::(0x30000000)	\
			(INCLUDE_AMP_CPU_06)::(0x20000000)	\
			(INCLUDE_AMP_CPU_05)::(0x10000000)	\
			(INCLUDE_AMP_CPU_04)::(0x40000000)	\
			(INCLUDE_AMP_CPU_03)::(0x30000000)	\
			(INCLUDE_AMP_CPU_02)::(0x20000000)	\
			(INCLUDE_AMP_CPU_01)::(0x10000000)	\
			0x00000000
}

Parameter LOCAL_MEM_SIZE {
	NAME		system memory size
	DEFAULT		(INCLUDE_3AMP_CONFIG)::(0x20000000)	\
			(INCLUDE_6AMP_CONFIG)::(0x20000000)	\
			(INCLUDE_AMP)::(0x10000000)		\
			QMAN_MEM_BASE
}

Parameter LOCAL_MEM_ERPN {
	NAME		Runtime kernel physical address, upper 8 bits
	DEFAULT		(INCLUDE_AMP_CPU_11)::(0x02)	\
			(INCLUDE_AMP_CPU_10)::(0x02)	\
			(INCLUDE_AMP_CPU_09)::(0x02)	\
			(INCLUDE_AMP_CPU_08)::(0x02)	\
			(INCLUDE_AMP_CPU_07)::(0x01)	\
			(INCLUDE_AMP_CPU_06)::(0x01)	\
			(INCLUDE_AMP_CPU_05)::(0x01)	\
			(INCLUDE_AMP_CPU_04)::(0x01)	\
			(INCLUDE_AMP_CPU_03)::(0x00)	\
			(INCLUDE_AMP_CPU_02)::(0x00)	\
			(INCLUDE_AMP_CPU_01)::(0x00)	\
			0x00
	_CFG_PARAMS	INCLUDE_MEMORY_CONFIG
}

Parameter WRLOAD_IMAGE_BUILD_PHYS_BIAS {
	DEFAULT		(INCLUDE_AMP_CPU_11)::(0x0200000000)	\
			(INCLUDE_AMP_CPU_10)::(0x0200000000)	\
			(INCLUDE_AMP_CPU_09)::(0x0200000000)	\
			(INCLUDE_AMP_CPU_08)::(0x0200000000)	\
			(INCLUDE_AMP_CPU_07)::(0x0100000000)	\
			(INCLUDE_AMP_CPU_06)::(0x0100000000)	\
			(INCLUDE_AMP_CPU_05)::(0x0100000000)	\
			(INCLUDE_AMP_CPU_04)::(0x0100000000)	\
			(INCLUDE_AMP_CPU_03)::(0x0000000000)	\
			(INCLUDE_AMP_CPU_02)::(0x0000000000)	\
			(INCLUDE_AMP_CPU_01)::(0x0000000000)	\
			0x0000000000
}

/* driver components */

Selection SELECT_QORIQMEMAC_MDIO {
	NAME		MDIO selection for QorIQ MEMAC
	COUNT		1-1
	REQUIRES	DRV_VXBEND_QORIQMEMAC
	_CHILDREN	FOLDER_BSP_CONFIG
	DEFAULTS	DRV_MII_QORIQMEMAC_MDIO
	CHILDREN	DRV_MII_QORIQMEMAC_MDIO		\
			INCLUDE_DUMMY_MDIO
}

Component DRV_VXBEND_QORIQMEMAC {
	REQUIRES	+= SELECT_QORIQMEMAC_MDIO	\
			   INCLUDE_USR_MACADDR		\
			   INCLUDE_GENERICPHY		\
			   DRV_RESOURCE_QORIQBMAN	\
			   DRV_RESOURCE_QORIQFMAN	\
			   DRV_RESOURCE_QORIQQMAN	\
			   DRV_RESOURCE_QORIQLAW	\
			   DRV_I2CBUS_FSL		\
			   DRV_I2C_GENERIC_DEV		\
			   DRV_I2C_VSC3316		\
			   DRV_MII_DTEPHY
}

Component INCLUDE_VIRTUAL_SM {
	NAME		virtual SM region
	_CHILDREN	FOLDER_BSP_CONFIG
	CFG_PARAMS	VIRTUAL_SM_BASE		\
			VIRTUAL_SM_BASE_PHYS	\
			VIRTUAL_SM_SIZE
}

Parameter VIRTUAL_SM_BASE {
	NAME		shared memory virtual address
	DEFAULT		0x78000000
}

Parameter VIRTUAL_SM_BASE_PHYS {
	NAME		shared memory physical address
	DEFAULT		0x78000000
}

Parameter VIRTUAL_SM_SIZE {
	NAME		shared memory size
	DEFAULT		0x01000000
}

Component INCLUDE_PCI_BUS {
    REQUIRES        INCLUDE_PCI_BUS_AUTOCONF \
                    DRV_PCIBUS_QORIQ_PCIEX
}

Component INCLUDE_USB {
	REQUIRES	+= INCLUDE_USB_PCI_STUB_CUSTOM
}
