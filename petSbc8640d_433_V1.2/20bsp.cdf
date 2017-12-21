/* 20bsp.cdf - BSP component description file */

/*
 * Copyright (c) 2006-2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01d,29oct08,dcc  removed AMP from MP_OPTIONS
01c,11sep08,l_z  Fix bootApp build error 
01b,07sep07,jmg  Added VX_SMP_NUM_CPUS and changed Bsp to wrSbc8641d.
01a,06jun07,x_s  initial created.
*/

Bsp petSbc8640d_433_V1.2 {
    NAME        Wind River SBC8641D board support package
    REQUIRES    INCLUDE_KERNEL    \
                DRV_TIMER_DEC_PPC \
                DRV_SIO_NS16550   \
                DRV_INTCTLR_EPIC  \
                DRV_INTCTLR_PPC
	CPU         PPC32
	MP_OPTIONS  SMP
	ENDIAN      big
	FP          hard
}

/* Network Boot Devices for a BSP
 * The REQUIRES line should be modified for a BSP.
 */
Component       INCLUDE_BOOT_NET_DEVICES {
		    REQUIRES	INCLUDE_ETSEC_VXB_END
}

Parameter RAM_HIGH_ADRS {
        NAME        Bootrom Copy region
        DEFAULT     (INCLUDE_BOOT_APP)::(0x01000000) \
                    0x00E00000
}

Parameter RAM_LOW_ADRS {
        NAME        Runtime kernel load address
        DEFAULT     (INCLUDE_BOOT_RAM_IMAGE)::(0x01200000) \
                    (INCLUDE_BOOT_APP)::(0x00E00000) \
                    0x00100000
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
	DEFAULT		2
}
