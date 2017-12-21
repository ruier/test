/* 20bsp.cdf - BSP component description file */

/*
 * Copyright (c) 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,22dec10,y_y  created
*/

Bsp fsl_p1021_mds {
	  CPU  		    PPC32
	  MP_OPTIONS	UP SMP
	  ENDIAN		  big
    FP          e500v2 soft
    REQUIRES    INCLUDE_KERNEL        \
                DRV_INTCTLR_EPIC      \
                DRV_INTCTLR_PPC       \
                DRV_TIMER_M85XX       \
                INCLUDE_TIMER_SYS     \
                DRV_INTCTLR_QE        \
                DRV_SIO_NS16550       \
                INCLUDE_QUICC_ENGINE_UTILS
}

Component       INCLUDE_ETSEC_VXB_END {
    REQUIRES += INCLUDE_MV88E1X11PHY  \
                INCLUDE_TSEC_MDIO     \
                INCLUDE_MII_BUS
}

Component       INCLUDE_UCC_VXB_END  {
    REQUIRES += INCLUDE_GENERICPHY    \
                INCLUDE_MII_BUS       \
                INCLUDE_UCC_MDIO
}


/* use openPicTimer as the auxClk */

Component       DRV_TIMER_OPENPIC {
                INCLUDE_WHEN += INCLUDE_AUX_CLK
}

/* Freescale I2C Bus Support */

Component       INCLUDE_I2C {

    NAME        I2C bus controller module
    SYNOPSIS    Freescale I2C bus controller driver
    _CHILDREN   FOLDER_DRIVERS
}

/* network boot devices for bootapp */

Component       INCLUDE_BOOT_NET_DEVICES {
    REQUIRES += INCLUDE_ETSEC_VXB_END \
                INCLUDE_UCC_VXB_END
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
    NAME        Number of CPUs available to be enabled for VxWorks SMP
    TYPE        UINT
    DEFAULT     2
}

/* enable M command in bootapp */

Component INCLUDE_BOOT_ETH_MAC_HANDLER {
    INCLUDE_WHEN += INCLUDE_BOOT_NETWORK
}

/* AMP components */

Bundle BUNDLE_AMP_PRI {
    COMPONENTS	+= INCLUDE_AMP
}

Bundle BUNDLE_AMP_SEC {
	  COMPONENTS	+= INCLUDE_AMP
}

Component INCLUDE_AMP {
	  NAME		    Base AMP Support
	  REQUIRES  	INCLUDE_AMP_CPU \
			          INCLUDE_VXIPI \
			          INCLUDE_VXCPULIB
    _CHILDREN 	FOLDER_BSP_CONFIG
}

/* Required component for AMP */

Component INCLUDE_AMP_CPU {
    NAME        AMP CPU Support
    SYNOPSIS    Per-cpu configuration for AMP
    REQUIRES    SELECT_AMP_CPU_NUMBER
    _CHILDREN   FOLDER_BSP_CONFIG
}

/*
 * The default component definitions for core 0 & 1 are empty.
 * Users may choose to uniquely configure the kernels and dedicate
 * device resources to each core by adding components to the REQUIRES
 * field of the INCLUDE_AMP_CPU_xx components below.
 */

Component INCLUDE_AMP_CPU_01  {
    NAME        AMP kernel configured for core 01
}

Component INCLUDE_AMP_CPU_00  {
    NAME        AMP kernel configured for core 00
}

/* Memory definitions */

Parameter RAM_HIGH_ADRS {
          NAME      Bootrom Copy region
          DEFAULT   (INCLUDE_BOOT_RAM_IMAGE)::(0x02200000)  \
                    (INCLUDE_BOOT_APP)::(0x01f00000)        \
                    (INCLUDE_AMP_CPU_00)::(0x01c00000)      \
                    (INCLUDE_AMP_CPU_01)::(0x12000000)      \
                    (0x01c00000)
}

Parameter RAM_LOW_ADRS {
         NAME       Runtime kernel load address
         DEFAULT    (INCLUDE_BOOT_RAM_IMAGE)::(0x00a00000)  \
                    (INCLUDE_BOOT_APP)::(0x00600000)        \
                    (INCLUDE_AMP_CPU_00)::(0x00200000)      \
                    (INCLUDE_AMP_CPU_01)::(0x11000000)      \
                    (0x00200000)

}

Parameter LOCAL_MEM_LOCAL_ADRS {
        NAME        system memory start address
        DEFAULT     (INCLUDE_AMP_CPU_00)::(0x00000000)      \
                    (INCLUDE_AMP_CPU_01)::(0x10000000)      \
                    (0x00000000)
}

Parameter LOCAL_MEM_SIZE {
        NAME        system memory size
        DEFAULT     (INCLUDE_AMP_CPU_00)::(0x10000000)      \
                    (INCLUDE_AMP_CPU_01)::(0x10000000)      \
                    (0x20000000)
}

/*
 * This parameter is used for tsec_mdio, should be set
 * 0x4400 + core1's LOCAL_MEM_LOCAL_ADRS value.
 */

Parameter TM_ANCHOR_ADRS {
        NAME        MDIO anchor adrs
        _CFG_PARAMS INCLUDE_TSEC_MDIO
        DEFAULT     (0x4400 + 0x10000000)
}

/*
 * This component set mv88e1x11phy mode of each ETSEC.
 */

Component INCLUDE_MV88E1X11PHY {     
       CFG_PARAMS   MV88E1X11PHY0_MODE              \
                    MV88E1X11PHY1_MODE              \
                    MV88E1X11PHY2_MODE              \
                    CONFIG_ETSEC3_IN_SGMII  
}

Parameter MV88E1X11PHY0_MODE {
       NAME         Marvell 88E1x11 mode
       DEFAULT      MV88E_MODE_RGMII
}

Parameter MV88E1X11PHY1_MODE {
       NAME         Marvell 88E1x11 mode
       DEFAULT      MV88E_MODE_SGMII_NOCLK
}

Parameter MV88E1X11PHY2_MODE {
       NAME         Marvell 88E1x11 mode
       DEFAULT      MV88E_MODE_RGMII
}

Parameter MV88E_MODE_RGMII {
       NAME         The mode parameter of Marvell 88E1x11
       DEFAULT      0x2
}

Parameter MV88E_MODE_SGMII_NOCLK {
       NAME         The mode parameter of Marvell 88E1x11
       DEFAULT      0x5
}

/* ETSEC3 in RGMII mode by default */

Parameter CONFIG_ETSEC3_IN_SGMII  {
    NAME            Enable ETSEC3 in SGMII mode
    TYPE            BOOL
    DEFAULT         FALSE
}

