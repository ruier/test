/* 20bsp.cdf - BSP component description file */

/*
 * Copyright (c) 2006-2009, 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01i,16jan10,x_s  Add SDRAM sizing and setup settings.
01h,29dec09,y_y  Fix cds8548 VIP image with PROFILE_BOOTAPP boot failed. (WIND00193103)
01g,29sep08,pgh  Fix PROFILE_BOOTAPP build problems.
01f,05sep08,pee  changing FP order so that VSB get e500v2 by default
01e,07dec07,dtr  Update for E500v2 hard float support.
01d,07aug07,dtr  Update for rapidIo configurations.
01c,22feb07,jmt  Defect 88750 - Fix problem with network boot device CDF
                 defines
01b,04dec06,rlg  update for NOR port
01a,16jul06,pee  created

*/

Bsp cds8548 {
	CPU         PPC32
	REQUIRES	INCLUDE_KERNEL \
                INCLUDE_VXBUS \
                DRV_TIMER_M85XX \
                DRV_SIO_NS16550 \
                INCLUDE_INTCTLR_LIB \
                DRV_INTCTLR_EPIC \
                DRV_INTCTLR_PPC
	FP          e500v2 soft
}

/* Network Boot Devices for a BSP
 * The REQUIRES line should be modified for a BSP.
 */
Component       INCLUDE_BOOT_NET_DEVICES {
    REQUIRES    INCLUDE_ETSEC_VXB_END \
                INCLUDE_VSC82XXPHY
}

Parameter RAM_HIGH_ADRS {
       NAME         Bootrom Copy region
       DEFAULT      (INCLUDE_BOOT_RAM_IMAGE)::(0x02000000) \
                    (INCLUDE_BOOT_APP)::(0x01400000) \
                    0x01000000
}

Parameter RAM_LOW_ADRS {
       NAME         Runtime kernel load address
       DEFAULT      (INCLUDE_BOOT_RAM_IMAGE)::(0x01800000) \
                    (INCLUDE_BOOT_APP)::(0x01000000) \
                    0x00100000
}

Component INCLUDE_RAPIDIO_BUS {
       REQUIRES   += INCLUDE_M85XX_RAPIDIO \
                  INCLUDE_M85XX_CPU DRV_RESOURCE_M85XXCCSR \
                  RAPIDIO_BUS_STATIC_TABLE \
                  VXBUS_TABLE_CONFIG
}

Component INCLUDE_SM_COMMON {
       REQUIRES   +=  INCLUDE_M85XX_RIO_SM_CFG INCLUDE_VXBUS_SM_SUPPORT \
                  INCLUDE_VXBUS_SM_SUPPORT INCLUDE_SMEND INCLUDE_SM_NET \
                  INCLUDE_IPATTACH INCLUDE_IFCONFIG \
                  INCLUDE_IPWRAP_IPPROTO
}

/*
 * SDRAM sizing and setup based on user-specified DIMM dimensions.
 *
 * This BSP does not support LOCAL_MEM_AUTOSIZE, and
 * LOCAL_MEM_SIZE is a derived value (not a parameter).
 *
 * There can be only one DIMM, because the cds8548 board has only
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
			SELECT_DDR_SDRAM_LOGI_BANK_BITS
}

Selection SELECT_DDR_SDRAM_ROW_ADRS_BITS {
	NAME	    Row address bits used by the installed DIMM
	SYNOPSIS    This setting, along with SELECT_DDR_SDRAM_COL_ADRS_BITS and SELECT_DDR_SDRAM_PHYS_BANK_BITS and SELECT_DDR_SDRAM_LOGI_BANK_BITS, determines the SDRAM size.
	_CHILDREN   FOLDER_MEMORY
	COUNT	    1-1
	DEFAULTS    INCLUDE_CS_ROW_BITS_13
	CHILDREN    INCLUDE_CS_ROW_BITS_12 INCLUDE_CS_ROW_BITS_13 \
		    INCLUDE_CS_ROW_BITS_14 INCLUDE_CS_ROW_BITS_15 \
		    INCLUDE_CS_ROW_BITS_16
}

Selection SELECT_DDR_SDRAM_COL_ADRS_BITS {
	NAME	    Column address bits used by the installed DIMM
	SYNOPSIS    This setting, along with SELECT_DDR_SDRAM_ROW_ADRS_BITS and SELECT_DDR_SDRAM_PHYS_BANK_BITS and SELECT_DDR_SDRAM_LOGI_BANK_BITS, determines the SDRAM size.
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

Selection SELECT_DDR_SDRAM_LOGI_BANK_BITS {
	NAME	    Address bits used to select a logical bank on the installed DIMM
	SYNOPSIS    This setting, along with SELECT_DDR_SDRAM_ROW_ADRS_BITS and SELECT_DDR_SDRAM_PHYS_BANK_BITS and SELECT_DDR_SDRAM_COL_ADRS_BITS, determines the SDRAM size.
	_CHILDREN   FOLDER_MEMORY
	COUNT	    1-1
	DEFAULTS    INCLUDE_CS_LOGI_BANK_BITS_2
	CHILDREN    INCLUDE_CS_LOGI_BANK_BITS_2 INCLUDE_CS_LOGI_BANK_BITS_3       
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

Component INCLUDE_CS_ROW_BITS_15 {
	NAME	    DIMM uses 15 row address bits
	SYNOPSIS    DIMM uses 15 row address bits
}

Component INCLUDE_CS_ROW_BITS_16 {
	NAME	    DIMM uses 16 row address bits
	SYNOPSIS    DIMM uses 16 row address bits
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

Component INCLUDE_CS_LOGI_BANK_BITS_2 {
	NAME	    DIMM uses 2 logical bank
	SYNOPSIS    DIMM uses 2 logical bank
}

Component INCLUDE_CS_LOGI_BANK_BITS_3 {
	NAME	    DIMM uses 3 logical bank
	SYNOPSIS    DIMM uses 3 logical bank
}
/* end of SDRAM sizing and setup */
