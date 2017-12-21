/* 40vxbIntelAhciStorage.cdf - Component Definition file for 
   Intel AHCI sata controller */
                                                                                
/*
 * Copyright (c) 2008-2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01d,10jun10,j_l  Merge in patches.
01c,25jan10,rtc  Change prototype for ahciShowInit() to remove build warning 
01b,04nov09,h_k  added a dependency to INCLUDE_VXBUS_DISK. (WIND00189354)
01a,16may08,mdg  written

*/

Component       INCLUDE_DRV_STORAGE_INTEL_AHCI {
    NAME             Intel AHCI SATA Controller
    SYNOPSIS         Supports Ahci controllers
    MODULES	     vxbIntelAhciStorage.o
    _CHILDREN        FOLDER_DRIVERS
    _INIT_ORDER      hardWareInterFaceBusInit
    PROTOTYPE	     void vxbIntelAhciStorageRegister(void);
    INIT_RTN         vxbIntelAhciStorageRegister();
    CFG_PARAMS       AHCI_SEM_TIMEOUT_DEF	\
                     AHCI_WDG_TIMEOUT_DEF	\
                     AHCI_SVC_TASK_COUNT
    REQUIRES         INCLUDE_PCI_BUS		\
                     INCLUDE_VXBUS_DISK		\
		     INCLUDE_FS_MONITOR
    INIT_AFTER       INCLUDE_PCI_BUS
}

Component       INCLUDE_DRV_STORAGE_INTEL_AHCI_SHOW {
    NAME             Intel AHCI SATA Controller Show Routines
    SYNOPSIS         Supports 82801 Intel Serial/Parallel ATA controllers
    MODULES	     vxbIntelAhciStorageShow.o
    _CHILDREN        FOLDER_DRIVERS
    _INIT_ORDER      hardWareInterFaceBusInit
    PROTOTYPE	     void ahciShowInit(void);
    INIT_RTN         ahciShowInit();
    REQUIRES         INCLUDE_DRV_STORAGE_INTEL_AHCI
    INIT_AFTER       INCLUDE_DRV_STORAGE_INTEL_AHCI
}

Parameter AHCI_SEM_TIMEOUT_DEF  {
    NAME              AHCI Semaphore Timeout
    SYNOPSIS          AHCI Driver Semaphore Timeout in Ticks
    TYPE              uint
    DEFAULT           (5*sysClkRateGet())
}

Parameter AHCI_WDG_TIMEOUT_DEF  {
    NAME              AHCI Watchdog Timeout
    SYNOPSIS          AHCI Driver Watchdog Timeout in Ticks
    TYPE              uint
    DEFAULT           (sysClkRateGet()/2)
}

Parameter AHCI_SVC_TASK_COUNT  {
    NAME              AHCI Service Tasks Per Drive
    SYNOPSIS          Number of Service Tasks per Drive (for NCQ support)
    TYPE              uint
    DEFAULT           1
}

