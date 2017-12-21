/* 40vxbNorFlash.cdf - Component Definition file for NOR Flash driver */

/*
 * Copyright (c) 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,05jan12,x_z  written.
*/

Component DRV_VXB_NORFLASH {
    NAME        Nor Flash VxBus Driver
    SYNOPSIS    Nor Flash VxBus Driver
    MODULES     vxbNorFlash.o
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    vxbNorFlashRegister();
    PROTOTYPE   void vxbNorFlashRegister(void);
    _INIT_ORDER hardWareInterFaceBusInit
    REQUIRES    INCLUDE_PLB_BUS     \
                INCLUDE_PARAM_SYS
    INIT_AFTER  INCLUDE_PLB_BUS
}

Component DRV_VXB_NORFLASH_CMDSET_AMD {
    NAME        AMD/Spansion Nor Flash command set library
    SYNOPSIS    AMD/Spansion Nor Flash command set library
    MODULES     vxbNorFlashAmdCmdSetLib.o
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    vxbNorFlashAmdCmdSetRegister();
    PROTOTYPE   void vxbNorFlashAmdCmdSetRegister(void);
    _INIT_ORDER hardWareInterFaceBusInit
    REQUIRES    DRV_VXB_NORFLASH
    INIT_AFTER  DRV_VXB_NORFLASH
}

Component DRV_VXB_NORFLASH_CMDSET_ITL {
    NAME        Intel Nor Flash command set library
    SYNOPSIS    Intel Nor Flash command set library
    MODULES     vxbNorFlashItlCmdSetLib.o
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    vxbNorFlashItlCmdSetRegister();
    PROTOTYPE   void vxbNorFlashItlCmdSetRegister(void);
    _INIT_ORDER hardWareInterFaceBusInit
    REQUIRES    DRV_VXB_NORFLASH
    INIT_AFTER  DRV_VXB_NORFLASH
}

