/* 20bsp.cdf - BSP component description file */

/*
 * Copyright (c) 2006-2008, 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01i,14jan10,pch  CQ195058: add SDRAM sizing and setup settings
01h,23feb09,x_z  add M command support for bootapp.(WIND00046372)
01g,16oct08,j_z  change RAM_HIGH_ADRS,RAM_LOW_ADRS,add INCLUDE_GENERICPHY(WIND00128640)
01f,25sep08,pmr  sysTimerClkFreq prototype for aux clock
01e,23jun08,d_c  Auto-exclude tests for unsupported components
01d,19oct07,slk  fix defect 106444: BoardSupportPackagesBootApp Test build
                 failures
01c,22feb07,jmt  Defect 88750 - Fix problem with network boot device CDF defines
01b,04dec06,rlg  update for NOR port
01a,16jul06,pee  created

*/

Bsp cds85xx {
	CPU PPC32
	REQUIRES	INCLUDE_KERNEL   \
			DRV_SIO_NS16550
	FP soft
}

Component       INCLUDE_TSEC_VXB_END {
       REQUIRES +=     INCLUDE_GENERICPHY  
} 

/*
 * Network Boot Devices for a BSP
 * The REQUIRES line should be modified for a BSP.
 */
 
Component       INCLUDE_BOOT_NET_DEVICES {
       REQUIRES    INCLUDE_TSEC_VXB_END
}

/* Define INCLUDE_BOOT_ETH_MAC_HANDLER when BootApp Networking is enabled */

Component       INCLUDE_BOOT_ETH_MAC_HANDLER {
       INCLUDE_WHEN INCLUDE_BOOT_NETWORK
}

Parameter RAM_HIGH_ADRS {
       NAME         Bootrom Copy region
       DEFAULT      (INCLUDE_BOOT_RAM_IMAGE)::(0x02000000) \
                    (INCLUDE_BOOT_APP)::(0x01400000) \
                    0x00e00000
}

Parameter RAM_LOW_ADRS {
       NAME         Runtime kernel load address
       DEFAULT      (INCLUDE_BOOT_RAM_IMAGE)::(0x01800000) \
                    (INCLUDE_BOOT_APP)::(0x00e00000) \
                    0x00100000
}

Component 	INCLUDE_AUX_CLK {
	PROTOTYPE	IMPORT UINT32 sysTimerClkFreq;
}
/*
 * SDRAM sizing and setup based on user-specified DIMM dimensions.
 *
 * This BSP does not support LOCAL_MEM_AUTOSIZE, and
 * LOCAL_MEM_SIZE is a derived value (not a parameter).
 *
 * There can be only one DIMM, because the cds85xx board has only
 * one DIMM socket.  This guarantees that the SDRAM size will be a
 * power of 2, and simplifies matters considerably.
 *
 * Ideally we would have a way to directly specify the set of acceptable
 * values for parameters such as DDR_SDRAM_ROW_ADRS_BITS, but at the time
 * this is being added the only configuration mechanism that allows choices
 * to be made from a list is the Selection, whose members are Components.
 * The selected components are converted to the corresponding parameter
 * values at the end of config.h (after its inclusion of prjParams.h).
 */

Component INCLUDE_MEMORY_CONFIG {
	CFG_PARAMS   -= LOCAL_MEM_SIZE LOCAL_MEM_AUTOSIZE
	REQUIRES     += SELECT_DDR_SDRAM_ROW_ADRS_BITS \
			SELECT_DDR_SDRAM_COL_ADRS_BITS \
			SELECT_DDR_SDRAM_PHYS_BANK_BITS \
			SELECT_DDR_SDRAM_CAS_LATENCY
}

Selection SELECT_DDR_SDRAM_ROW_ADRS_BITS {
	NAME	    Row address bits used by the installed DIMM
	SYNOPSIS    This setting, along with SELECT_DDR_SDRAM_COL_ADRS_BITS and SELECT_DDR_SDRAM_PHYS_BANK_BITS, determines the SDRAM size.
	_CHILDREN   FOLDER_MEMORY
	COUNT	    1-1
	DEFAULTS    INCLUDE_CS_ROW_BITS_13
	CHILDREN    INCLUDE_CS_ROW_BITS_12 INCLUDE_CS_ROW_BITS_13 \
		    INCLUDE_CS_ROW_BITS_14
}

Selection SELECT_DDR_SDRAM_COL_ADRS_BITS {
	NAME	    Column address bits used by the installed DIMM
	SYNOPSIS    This setting, along with SELECT_DDR_SDRAM_ROW_ADRS_BITS and SELECT_DDR_SDRAM_PHYS_BANK_BITS, determines the SDRAM size.
	_CHILDREN   FOLDER_MEMORY
	COUNT	    1-1
	DEFAULTS    INCLUDE_CS_COL_BITS_10
	CHILDREN    INCLUDE_CS_COL_BITS_8 INCLUDE_CS_COL_BITS_9 \
		    INCLUDE_CS_COL_BITS_10 INCLUDE_CS_COL_BITS_11
}

Selection SELECT_DDR_SDRAM_PHYS_BANK_BITS {
	NAME	    Address bits used to select a physical bank on the installed DIMM
	SYNOPSIS    Set to 0 for an 8-byte-wide DIMM, or 1 for a 16-byte-wide DIMM.  This setting, along with SELECT_DDR_SDRAM_ROW_ADRS_BITS and SELECT_DDR_SDRAM_COL_ADRS_BITS, determines the SDRAM size.
	_CHILDREN   FOLDER_MEMORY
	COUNT	    1-1
	DEFAULTS    INCLUDE_CS_PHYS_BANK_BITS_0
	CHILDREN    INCLUDE_CS_PHYS_BANK_BITS_0 INCLUDE_CS_PHYS_BANK_BITS_1
}

Selection SELECT_DDR_SDRAM_CAS_LATENCY {
	NAME	    DIMM CAS latency
	SYNOPSIS    DIMM "CAS latency" or "CL" value
	_CHILDREN   FOLDER_MEMORY
	COUNT	    1-1
	DEFAULTS    INCLUDE_CASLAT_2_PT_5
	CHILDREN    INCLUDE_CASLAT_1_PT_0 INCLUDE_CASLAT_1_PT_5 \
		    INCLUDE_CASLAT_2_PT_0 INCLUDE_CASLAT_2_PT_5 \
		    INCLUDE_CASLAT_3_PT_0 INCLUDE_CASLAT_3_PT_5 \
		    INCLUDE_CASLAT_4_PT_0
}

Component INCLUDE_CS_ROW_BITS_12 {
	NAME	    DIMM uses 12 row address bits
	SYNOPSIS    DIMM uses 12 row address bits
}

Component INCLUDE_CS_ROW_BITS_13 {
	NAME	    DIMM uses 13 row address bits
	SYNOPSIS    DIMM uses 13 row address bits
}

Component INCLUDE_CS_ROW_BITS_14 {
	NAME	    DIMM uses 14 row address bits
	SYNOPSIS    DIMM uses 14 row address bits
}

Component INCLUDE_CS_COL_BITS_8 {
	NAME	    DIMM uses 8 column address bits
	SYNOPSIS    DIMM uses 8 column address bits
}

Component INCLUDE_CS_COL_BITS_9 {
	NAME	    DIMM uses 9 column address bits
	SYNOPSIS    DIMM uses 9 column address bits
}

Component INCLUDE_CS_COL_BITS_10 {
	NAME	    DIMM uses 10 column address bits
	SYNOPSIS    DIMM uses 10 column address bits
}

Component INCLUDE_CS_COL_BITS_11 {
	NAME	    DIMM uses 11 column address bits
	SYNOPSIS    DIMM uses 11 column address bits
}

Component INCLUDE_CS_PHYS_BANK_BITS_0 {
	NAME	    DIMM is 8 bytes wide
	SYNOPSIS    DIMM is 8 bytes wide
}

Component INCLUDE_CS_PHYS_BANK_BITS_1 {
	NAME	    DIMM is 16 bytes wide
	SYNOPSIS    DIMM is 16 bytes wide
}

Component INCLUDE_CASLAT_1_PT_0 {
	NAME	    CL 1.0
	SYNOPSIS    CAS latency 1.0
}

Component INCLUDE_CASLAT_1_PT_5 {
	NAME	    CL 1.5
	SYNOPSIS    CAS latency 1.5
}

Component INCLUDE_CASLAT_2_PT_0 {
	NAME	    CL 2.0
	SYNOPSIS    CAS latency 2.0
}

Component INCLUDE_CASLAT_2_PT_5 {
	NAME	    CL 2.5
	SYNOPSIS    CAS latency 2.5
}

Component INCLUDE_CASLAT_3_PT_0 {
	NAME	    CL 3.0
	SYNOPSIS    CAS latency 3.0
}

Component INCLUDE_CASLAT_3_PT_5 {
	NAME	    CL 3.5
	SYNOPSIS    CAS latency 3.5
}

Component INCLUDE_CASLAT_4_PT_0 {
	NAME	    CL 4.0
	SYNOPSIS    CAS latency 4.0
}

/* end of SDRAM sizing and setup */

/*
 * Force unsupported components to be unavailable.
 *
 * The following component definition(s) forces the named component(s)
 * to be "unavailable" as far as the project build facility (vxprj) is
 * concerned. The required component (COMPONENT_NOT_SUPPORTED) should
 * never be defined, and hence the named component(s) will never be
 * available. This little trick is used by the BSPVTS build scripts
 * (suiteBuild, bspBuildTest.tcl) to automatically exclude test modules
 * that are not applicable to a BSP because the BSP does not support a
 * given component and/or test module. If and when support is added,
 * the following definition(s) should be removed.
 */

Component               INCLUDE_TM_VXBPCIACCESS {
        REQUIRES        COMPONENT_NOT_SUPPORTED
}

Component               INCLUDE_TM_VXBVIRTACCESS {
        REQUIRES        COMPONENT_NOT_SUPPORTED
}

Component               INCLUDE_TIMER_SYS {
        REQUIRES        COMPONENT_NOT_SUPPORTED
}
