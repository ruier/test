/* 40vxbOcteonEnd.cdf - Component configuration file for Octeon END drivers */
  
/*
 * Copyright 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,11oct10,d_c  written

DESCRIPTION

This file contains component definitions for the Cavium Octeon END
drivers and associated MDIO and PHY drivers.

There are two END drivers defined here: 1) the generic Octeon END
driver (INCLUDE_OCTEON_VXB_END), which supports three different types
of PHY interfaces: RGMII, SGMII, and SPI4000 and 2) a management
interface driver (DRV_VXBEND_OCTEON_MGMT), which supports RGMII only,
via the Cavium MIX interface. In addition, the required MDIO driver
components are defined.

*/

/*****************************************************************************
* END Drivers
*/

/* 
 * Generic Octeon END driver. This requires that at least one of the
 * three "Helper" drivers is also included: INCLUDE_OCTEON_RGMII,
 * INCLUDE_OCTEON_SGMII, or INCLUDE_OCTEON_SPI4000.
 */

Component INCLUDE_OCTEON_VXB_END {
    NAME            Octeon VxBus Enhanced Network Driver
    SYNOPSIS        Octeon VxBus IPNET-native END Driver
    _CHILDREN       FOLDER_DRIVERS
    _INIT_ORDER     hardWareInterFaceBusInit
    PROTOTYPE       void octEndRegister(void);
    INIT_RTN        octEndRegister();
    REQUIRES        INCLUDE_PLB_BUS     \
                    INCLUDE_GENERICPHY  \
                    INCLUDE_MII_BUS     \
                    INCLUDE_PARAM_SYS   \
		    INCLUDE_END2
    INIT_AFTER      INCLUDE_PLB_BUS
}

/* RGMII/MIX-based management driver. */

Component DRV_VXBEND_OCTEON_MGMT {
    NAME        Octeon MII Ethernet VxBus Enhanced Network Driver
    SYNOPSIS    Octeon MII Ethernet Enhanced Network Driver
    REQUIRES    INCLUDE_PLB_BUS     \
                INCLUDE_GENERICPHY  \
                INCLUDE_MII_BUS     \
                INCLUDE_PARAM_SYS
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    vxbOcteonMgmtEndRegister();
    PROTOTYPE   void vxbOcteonMgmtEndRegister(void);
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_AFTER  INCLUDE_PLB_BUS
}

/*****************************************************************************
* "Helper" drivers for generic Octeon END driver (INCLUDE_OCTEON_VXB_END).
* These helper drivers are actually pulled in via a reference to
* a table of function pointers in hwconf.c. This table is also passed
* to the generic END driver through a vxBus resource ("funcsX", where
* X is the GMX interface number associated with the PHY).
*/

/* RGMII helper driver for generic END driver (INCLUDE_OCTEON_VXB_END).
 */

Component INCLUDE_OCTEON_RGMII {
    NAME            RGMII interface support for Octeon
    SYNOPSIS        Adds RGMII interface support to the Octeon \
END driver
    _CHILDREN       FOLDER_DRIVERS
    REQUIRES        INCLUDE_OCTEON_VXB_END \
                    INCLUDE_OCTEON_MDIO
}

/* SGMII helper driver for generic END driver (INCLUDE_OCTEON_VXB_END).
 */

Component INCLUDE_OCTEON_SGMII {
    NAME            SGMII interface support for Octeon
    SYNOPSIS        Adds SGMII interface support to the Octeon \
END driver
    _CHILDREN       FOLDER_DRIVERS
    REQUIRES        INCLUDE_OCTEON_VXB_END \
                    INCLUDE_OCTEON_MDIO
}

/* SPI4000 helper driver for generic END driver (INCLUDE_OCTEON_VXB_END).
 */

Component INCLUDE_OCTEON_SPI4000 {
    NAME            SPI4000 extension board support for Octeon
    SYNOPSIS        Adds support for SPI4000 ethernet ports to the Octeon \
END driver.
    _CHILDREN       FOLDER_DRIVERS
    REQUIRES        INCLUDE_OCTEON_VXB_END \
                    INCLUDE_SPI4000_MDIO \
                    INCLUDE_SPI4000_PHY
}

/* SPI4000 PHY driver for generic END driver (INCLUDE_OCTEON_VXB_END).
 */

Component INCLUDE_SPI4000_PHY {
    NAME            VxBus PHY support for SPI4000
    SYNOPSIS        Adds a SPI4000-specific PHY driver for the Marvell \
88E1141 PHYs on the SPI4000, handling IXF1010 speed/duplex limitations
    PROTOTYPE       void spi4000PhyRegister (void);

    _CHILDREN       FOLDER_DRIVERS
    HDR_FILES       ../src/hwif/h/mii/mv88E1x11Phy.h
    REQUIRES        INCLUDE_OCTEON_SPI4000 \
                    INCLUDE_MII_BUS
    INIT_RTN        spi4000PhyRegister();
    _INIT_ORDER     hardWareInterFaceBusInit
    INIT_AFTER      INCLUDE_MII_BUS
}

/*****************************************************************************
* MDIO drivers 
*/

/* INCLUDE_OCTEON_MDIO: MDIO via Octeon SMI interface. This is used
 * when enabling the RGMII or SGMII PHY interfaces.
 */

Component    INCLUDE_OCTEON_MDIO {
    NAME            Octeon VxBus MDIO
    SYNOPSIS        MDIO via Octeon SMI interface
    PROTOTYPE       void octeonMdioRegister(void);
    _CHILDREN       FOLDER_DRIVERS
    _INIT_ORDER     hardWareInterFaceBusInit
    INIT_RTN        octeonMdioRegister();
    INIT_AFTER      INCLUDE_PLB_BUS
}

/* INCLUDE_SPI4000_MDIO: MDIO via IFX1010 (requires Octeon SPI4000 
 * extension board).
 */

Component    INCLUDE_SPI4000_MDIO {
    NAME            Octeon VxBus SPI4000 MDIO driver
    SYNOPSIS        MDIO support for SPI4000 board via IXF1010 MAC
    PROTOTYPE       void spi4000MdioRegister(void);
    _CHILDREN       FOLDER_DRIVERS
    _INIT_ORDER     hardWareInterFaceBusInit
    INIT_RTN        spi4000MdioRegister();
    INIT_AFTER      INCLUDE_PLB_BUS
    REQUIRES        INCLUDE_OCTEON_SPI4000
}
