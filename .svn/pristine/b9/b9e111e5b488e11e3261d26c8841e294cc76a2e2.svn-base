/* 20bsp.cdf - BSP component description file */

/*
 * Copyright (c) 2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01e,29oct08,dcc  removed MP_OPTIONS
01d,30sep08,kab  Fix default AMP CPU selection
01c,05sep08,dtr  Move MSD parameters to 22comp_mipc.cdf.
01b,04sep08,dtr  Use CPU number selection. Defaults are CPU0
01a,18aug08,dtr  Created from wrSbc8641d/20bsp.cdf/01b
*/

Bsp wrSbc8641d_AMP {
	NAME        Wind River SBC8641D board support package
	REQUIRES    INCLUDE_KERNEL \
		    DRV_SIO_NS16550 \
		    INCLUDE_AMP_CPU
	CPU         PPC32
	ENDIAN      big
	FP          hard
}

/* Network Boot Devices for a BSP
 * The REQUIRES line should be modified for a BSP.
 */
Component       INCLUDE_BOOT_NET_DEVICES {
	REQUIRES	INCLUDE_ETSEC_VXB_END
}

/*
 * Required component for AMP.
 */
Component INCLUDE_AMP_CPU {
	NAME		AMP CPU Support
	SYNOPSIS	Per-cpu configuration for AMP
	REQUIRES	SELECT_AMP_CPU_NUMBER
	_CHILDREN	FOLDER_BSP_CONFIG
}

/*
 * Override the default (empty) component definitions with custom options
 */
Component INCLUDE_AMP_CPU_01  {
	NAME		AMP kernel configured for core 01
}

Component INCLUDE_AMP_CPU_00  { 
	NAME		AMP kernel configured for core 00
}

/* Memory definitions */

Parameter RAM_HIGH_ADRS {
    	NAME     Bootrom Copy region
    	DEFAULT  	(INCLUDE_AMP_CPU_00)::(0x00e00000) \
             		(INCLUDE_AMP_CPU_01)::(0x00e00000) \
			(0x00e00000)
}

Parameter RAM_LOW_ADRS {
    	NAME     Runtime kernel load address
    	DEFAULT  	(INCLUDE_AMP_CPU_00)::(0x00100000) \
             		(INCLUDE_AMP_CPU_01)::(0x00100000) \
			(0x00100000)
}

Parameter LOCAL_MEM_LOCAL_ADRS {
    	NAME     system memory start address
    	DEFAULT  	(INCLUDE_AMP_CPU_00)::(0x00000000) \
             		(INCLUDE_AMP_CPU_01)::(0x00000000) \
			(0x00000000)
}

Parameter LOCAL_MEM_SIZE {
    	NAME     system memory size
    	DEFAULT  	(INCLUDE_AMP_CPU_00)::(0x10000000) \
             		(INCLUDE_AMP_CPU_01)::(0x10000000) \
			(0x10000000
}

Parameter  RAM_LOW_ADRS_CPU0 {
	NAME     Ram low address specifically for cpu0
    	DEFAULT  	0x00100000
}

Parameter TM_ANCHOR_ADRS {
    	NAME     MDIO anchor adrs
    	DEFAULT  	(0x4400 + 0x10000000)
}

Component INCLUDE_PCI_BUS {
    	REQUIRES  	INCLUDE_PCI_BUS_AUTOCONF \
              		DRV_PCIBUS_M85XX
}
