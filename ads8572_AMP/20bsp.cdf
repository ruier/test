/* 20bsp.cdf - BSP component description file */

/*
 * Copyright (c) 2007-2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01o,29oct08,dcc  removed MP_OPTIONS
01n,09oct08,jlk  Undoing WIND00136978
01m,08oct08,jlk  WIND00136978 - Added INCLUDE_WRLOAD_IMAGE_BUILD to 
		 INCLUDE_AMP_CPU_01 component
01l,30sep08,kab  Fix default AMP CPU selection
01k,26sep08,dtr  UPdate for MSD components.
01j,05sep08,dtr  Move MSD parameters to 22comp_mipc.cdf.
01i,05sep08,pee  changing FP order so that VSB get e500v2 by default
01h,04sep08,dtr  Apply defaults for CPU0 if no CPU defined.
01g,02sep08,dtr  Add CPU number select mechanism.
01g,01sep08,ebh  Move all MIPC/MSD/MCB components to 22comp_mipc.cdf
01f,15aug08,dtr  Add MSD support.
01e,01jul08,dtr  Update PCI dependencies.
01d,24apr08,dtr  Remove BOOT_ETH_MAC_HANDLER from requires.
01c,11apr08,dtr  Mods to support bootapp build.
01b,23jan08,dtr  Revert to standard soft float build.
01a,19dec07,dtr  created
*/

Bsp ads8572_AMP {
	CPU		PPC32
	ENDIAN		big
	FP		e500v2 soft
	REQUIRES	DRV_INTCTLR_EPIC \
			DRV_INTCTLR_PPC \
			DRV_TIMER_M85XX \
			INCLUDE_TIMER_SYS \
			INCLUDE_ETSEC_VXB_END \
			INCLUDE_DMA_SYS \
			INCLUDE_MII_BUS \
			INCLUDE_GENERICPHY \
			INCLUDE_TSEC_MDIO \
			INCLUDE_AMP_CPU \
			INCLUDE_KERNEL
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
    	DEFAULT  	(INCLUDE_AMP_CPU_00)::(0x01000000) \
        	     	(INCLUDE_AMP_CPU_01)::(0x22000000) \
			(0x01000000)
}

Parameter RAM_LOW_ADRS {
	NAME     Runtime kernel load address
	DEFAULT  	(INCLUDE_AMP_CPU_00)::(0x00100000) \
			(INCLUDE_AMP_CPU_01)::(0x21000000) \
			(0x00100000)
}

Parameter LOCAL_MEM_LOCAL_ADRS {
    	NAME     system memory start address
    	DEFAULT  	(INCLUDE_AMP_CPU_00)::(0x00000000) \
             		(INCLUDE_AMP_CPU_01)::(0x20000000) \
			(0x00000000)
}

Parameter LOCAL_MEM_SIZE {
    	NAME     system memory size
    	DEFAULT  	(INCLUDE_AMP_CPU_00)::(0x20000000) \
             		(INCLUDE_AMP_CPU_01)::(0x20000000) \
			(0x20000000)
}

Parameter  RAM_LOW_ADRS_CPU0 {
	NAME     Ram low address specifically for cpu0
	DEFAULT  	0x00100000
}

Parameter TM_ANCHOR_ADRS {
    	NAME     MDIO anchor adrs
    	DEFAULT  	(0x4400 + 0x20000000)
}

Component INCLUDE_PCI_BUS {
    	REQUIRES  	INCLUDE_PCI_BUS_AUTOCONF \
         		DRV_PCIBUS_M85XX
}
