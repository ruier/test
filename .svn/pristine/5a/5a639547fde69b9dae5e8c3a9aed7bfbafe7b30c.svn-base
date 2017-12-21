/* 40vxbLsiMdio.cdf - Component configuration file */

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
01a,22may10,x_z  written
*/

Component INCLUDE_LSI_MDIO {
    NAME        LSI MDIO driver
    SYNOPSIS    LSI MDIO driver
    _CHILDREN   FOLDER_DRIVERS
    _INIT_ORDER hardWareInterFaceBusInit
    HDR_FILES   ../src/hwif/h/mii/vxbLsiMdio.h
    INIT_RTN    lsiMdioRegister();
    REQUIRES    INCLUDE_MII_BUS
    INIT_AFTER  INCLUDE_MII_BUS
}
