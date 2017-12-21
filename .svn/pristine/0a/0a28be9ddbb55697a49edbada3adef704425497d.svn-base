/* 20bsp.cdf - BSP component description file */

/*
 * Copyright (c) 2010-2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01g,12apr12,y_y  Replaced INCLUDE_TSEC_MDIO with INCLUDE_AMP in 
                 TM_ANCHOR_ADRS. (WIND00344884)
01f,09apr12,h_k  added dependency from non core0 AMP to
                 INCLUDE_WRLOAD_IMAGE_BUILD.
                 removed INCLUDE_SYSTEM_GLOBALS. (WIND00341977)
01e,22sep11,ers  Replaced DYNAMIC_WRLOAD with generic wrload. Relocated 
                 SYSTEM_GLOBALS.
01d,24aug11,swu  Fix build warnings for BOOTAPP. (WIND00292325)
01c,03mar11,e_d  added SMP support.
01b,28dec10,e_d  modified RAM_HIGH_ADRS value.(WIND00248407)
01a,25nov10,e_d  created from fsl_p4080_ds 20bsp.cdf version 01o
*/

Bsp wrSbcP4080 {
  CPU         PPC32
  MP_OPTIONS  UP SMP
  ENDIAN      big
  FP          hard soft
  REQUIRES    INCLUDE_KERNEL \
              INCLUDE_VXBUS \
              DRV_TIMER_M85XX \
              INCLUDE_TIMER_SYS \
              INCLUDE_SIO_UTILS \
              INCLUDE_AIM_MMU_BASIC \
              INCLUDE_AIM_MMU_MEM_POOL_CONFIG \
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
  REQUIRES += DRV_MII_DTSEC_MDIO
}

Component INCLUDE_BOOT_NET_DEVICES {
  REQUIRES  DRV_VXBEND_DTSEC
}

/* enable M command in bootapp */

Component INCLUDE_BOOT_ETH_MAC_HANDLER {
  INCLUDE_WHEN += INCLUDE_BOOT_NETWORK
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
  NAME      Number of CPUs available to be enabled for VxWorks SMP
  TYPE      UINT
  DEFAULT   (INCLUDE_AMP)::(2) \
            8
}

/* AMP components */

Bundle BUNDLE_AMP_PRI {
  COMPONENTS  += INCLUDE_AMP
}

Bundle BUNDLE_AMP_SEC {
  COMPONENTS  += INCLUDE_AMP
}

Component INCLUDE_AMP {
  NAME      Base AMP Support
  REQUIRES  INCLUDE_AMP_CPU \
            INCLUDE_VXIPI \
            INCLUDE_VXCPULIB
  _CHILDREN FOLDER_BSP_CONFIG
}

Component INCLUDE_AMP_CPU {
  NAME      AMP CPU Support
  SYNOPSIS  Per-cpu configuration for AMP
  REQUIRES  SELECT_AMP_CPU_NUMBER
  _CHILDREN FOLDER_BSP_CONFIG
}

/*
 * Override the default (empty) component definitions with custom options
 */

Component INCLUDE_AMP_CPU_07  {
  NAME  AMP kernel configured for core 07
	REQUIRES	INCLUDE_AMP_CPU		\
			INCLUDE_WRLOAD_IMAGE_BUILD
  _CHILDREN SELECT_AMP_CPU_NUMBER
  CFG_PARAMS  AMP_CPU_NUMBER
}

Component INCLUDE_AMP_CPU_06  {
  NAME  AMP kernel configured for core 06
	REQUIRES	INCLUDE_AMP_CPU		\
			INCLUDE_WRLOAD_IMAGE_BUILD
 _CHILDREN SELECT_AMP_CPU_NUMBER
  CFG_PARAMS  AMP_CPU_NUMBER
}

Component INCLUDE_AMP_CPU_05  {
  NAME  AMP kernel configured for core 05
	REQUIRES	INCLUDE_AMP_CPU		\
			INCLUDE_WRLOAD_IMAGE_BUILD
  _CHILDREN SELECT_AMP_CPU_NUMBER
  CFG_PARAMS  AMP_CPU_NUMBER
}

Component INCLUDE_AMP_CPU_04  {
  NAME  AMP kernel configured for core 04
	REQUIRES	INCLUDE_AMP_CPU		\
			INCLUDE_WRLOAD_IMAGE_BUILD
  _CHILDREN SELECT_AMP_CPU_NUMBER
  CFG_PARAMS  AMP_CPU_NUMBER
}

Component INCLUDE_AMP_CPU_03  {
  NAME  AMP kernel configured for core 03
	REQUIRES	INCLUDE_AMP_CPU		\
			INCLUDE_WRLOAD_IMAGE_BUILD
  _CHILDREN SELECT_AMP_CPU_NUMBER
  CFG_PARAMS  AMP_CPU_NUMBER
}

Component INCLUDE_AMP_CPU_02  {
  NAME  AMP kernel configured for core 02
	REQUIRES	INCLUDE_AMP_CPU		\
			INCLUDE_WRLOAD_IMAGE_BUILD
  _CHILDREN SELECT_AMP_CPU_NUMBER
  CFG_PARAMS  AMP_CPU_NUMBER
}

Component INCLUDE_AMP_CPU_01  {
	REQUIRES	INCLUDE_AMP_CPU		\
			INCLUDE_WRLOAD_IMAGE_BUILD
  CFG_PARAMS  AMP_CPU_NUMBER
}

Component INCLUDE_AMP_CPU_00  {
	REQUIRES	INCLUDE_AMP_CPU
  CFG_PARAMS  AMP_CPU_NUMBER
}

Parameter RAM_HIGH_ADRS {
  NAME        vxWorks bootrom load address
  DEFAULT     (INCLUDE_BOOT_APP)::(0x02000000) \
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
  DEFAULT     (INCLUDE_BOOT_APP)::(0x01800000) \
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
  DEFAULT     (INCLUDE_BOOT_APP)::(0x00000000) \
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
  NAME       system memory size
  DEFAULT     (INCLUDE_BOOT_APP)::((0x80000000 - QMAN_BMAN_RESERVED_MEM)) \
              (INCLUDE_AMP_CPU_07)::(0x02000000) \
              (INCLUDE_AMP_CPU_06)::(0x02000000) \
              (INCLUDE_AMP_CPU_05)::(0x02000000) \
              (INCLUDE_AMP_CPU_04)::(0x02000000) \
              (INCLUDE_AMP_CPU_03)::(0x02000000) \
              (INCLUDE_AMP_CPU_02)::(0x02000000) \
              (INCLUDE_AMP_CPU_01)::(0x02000000) \
              (INCLUDE_AMP_CPU_00)::(0x10000000) \
              (0x80000000 - QMAN_BMAN_RESERVED_MEM)
}

Parameter AMP_CPU_NUMBER {
  NAME  AMP CPU number
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
  NAME          Runtime kernel physical address, upper 4 bits
  DEFAULT       0x0
  _CFG_PARAMS   INCLUDE_MEMORY_CONFIG
}

Component INCLUDE_LOGICAL_PARTITION {
  NAME          Logical partition support
  CFG_PARAMS    LOGICAL_PARTITION_ID
  INCLUDE_WHEN  INCLUDE_AMP
  _CHILDREN     FOLDER_BSP_CONFIG
}

Parameter LOGICAL_PARTITION_ID {
  NAME          Logical partition ID for this OS instance
  DEFAULT       (INCLUDE_AMP_CPU_07)::(17) \
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
  NAME        MDIO anchor adrs
  _CFG_PARAMS INCLUDE_AMP
  DEFAULT     (0x4400 + 0x20000000)
}

Component INCLUDE_PCI_BUS {
  REQUIRES    INCLUDE_PCI_BUS_AUTOCONF \
              DRV_PCIBUS_QORIQ_PCIEX
}

/* shared memory components */

Component INCLUDE_VIRTUAL_SM {
  NAME          virtual SM region
  _CHILDREN     FOLDER_BSP_CONFIG
  CFG_PARAMS    VIRTUAL_SM_BASE \
                VIRTUAL_SM_BASE_PHYS \
                VIRTUAL_SM_SIZE
}

Parameter VIRTUAL_SM_BASE {
  NAME          shared memory virtual address
  DEFAULT      (PHYS_MEM_START + PHYS_MEM_SIZE)
}

Parameter VIRTUAL_SM_BASE_PHYS {
  NAME          shared memory physical address
  DEFAULT       0x20000000
}

Parameter VIRTUAL_SM_SIZE {
  NAME          shared memory size
  DEFAULT       0x01000000
}

/* Cache error handling components */

Component INCLUDE_CACHE_ERROR_HANDLER {
  NAME         L1/L2/L3 cache error handler
  _CHILDREN FOLDER_BSP_CONFIG
}

Component INCLUDE_L1_CACHE_ERROR_RECOVERY {
  NAME          L1 cache error recovery
  REQUIRES      INCLUDE_CACHE_ERROR_HANDLER
  INCLUDE_WHEN  INCLUDE_CACHE_ERROR_HANDLER
  CFG_PARAMS    L1_CACHE_ERROR_INJECTION
  _CHILDREN FOLDER_BSP_CONFIG
}

Parameter L1_CACHE_ERROR_INJECTION {
  NAME    include L1 cache error injection code
  TYPE    exists
  DEFAULT FALSE
}

Component INCLUDE_L2_CACHE_ERROR_RECOVERY {
  NAME        L2 cache error recovery
  REQUIRES    INCLUDE_CACHE_ERROR_HANDLER
  CFG_PARAMS  L2_CACHE_ERROR_INJECTION \
              L2_CACHE_ERROR_INT_ENABLE \
              L2_CACHE_ERROR_L2CTHRESH
  _CHILDREN FOLDER_BSP_CONFIG
}

Parameter L2_CACHE_ERROR_INJECTION {
  NAME    include L2 cache error injection code
  TYPE    exists
  DEFAULT FALSE
}

Parameter L2_CACHE_ERROR_INT_ENABLE {
  NAME    L2 cache error interrupt enable setting
  DEFAULT 0x9e
}

Parameter L2_CACHE_ERROR_L2CTHRESH {
  NAME    L2 cache error threshold setting (L2ERRCTL)
  DEFAULT 0x0
}

Component INCLUDE_L3_CACHE_ERROR_RECOVERY {
  NAME        L3 cache error recovery
  REQUIRES    INCLUDE_CACHE_ERROR_HANDLER
  CFG_PARAMS  L3_CACHE_ERROR_INJECTION \
              L3_CACHE_ERROR_INT_ENABLE \
              L3_CACHE_ERROR_CPCTHRESH
  _CHILDREN   FOLDER_BSP_CONFIG
}

Parameter L3_CACHE_ERROR_INJECTION {
  NAME    include L3 cache error injection code
  TYPE    exists
  DEFAULT FALSE
}

Parameter L3_CACHE_ERROR_INT_ENABLE {
  NAME    L3 cache error interrupt enable setting
  DEFAULT 0xec
}

Parameter L3_CACHE_ERROR_CPCTHRESH {
  NAME    L3 cache error threshold setting (CPCERRCTL)
  DEFAULT 0x0
}
