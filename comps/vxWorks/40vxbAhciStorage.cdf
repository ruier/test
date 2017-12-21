/* 40vxbAhciStorage.cdf - Component Definition file for AHCI sata controller */
                                                                                
/*
 * Copyright (c) 2011, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,21mar12,sye  Remove unnecessary parameters. (WIND00289836)
01a,11sep11,e_d  derived from 40vxbIntelAhciStorage.cdf/01d.
*/

Component       INCLUDE_DRV_STORAGE_AHCI {
    NAME        AHCI SATA Controller
    SYNOPSIS    Supports Ahci controllers
    MODULES     vxbAhciStorage.o
    _CHILDREN   FOLDER_DRIVERS
    _INIT_ORDER hardWareInterFaceBusInit
    PROTOTYPE   void vxbAhciStorageRegister(void);
    INIT_RTN    vxbAhciStorageRegister();
    REQUIRES    INCLUDE_PCI_BUS      \
                INCLUDE_VXBUS_DISK   \
                INCLUDE_FS_MONITOR
    INIT_AFTER  INCLUDE_PCI_BUS
}
