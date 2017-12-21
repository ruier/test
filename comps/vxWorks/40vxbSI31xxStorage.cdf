/* 40vxbSI31xxStorage.cdf - Component Definition file for 
   Silicon Image 3124/3132 Sata controller */
                                                                                
/* Copyright (c) 2007 Wind River Systems, Inc. */
                                                                                
/*
modification history
--------------------
01b,15sep08,pmr  fix typo in SYNOPSIS (WIND00130404)
01a,01mar07,dee  written

*/

Component       DRV_STORAGE_SI31XX {
    NAME             Silicon Image 31xx SATA Controller
    SYNOPSIS         Supports 3124 and 3132 Silicon Image Serial ATA controllers
    MODULES	     vxbSI31xxStorage.o
    _CHILDREN        FOLDER_DRIVERS
    _INIT_ORDER      hardWareInterFaceBusInit
    PROTOTYPE	     void vxbSI31xxStorageRegister(void);
    INIT_RTN         vxbSI31xxStorageRegister();
    REQUIRES         INCLUDE_PCI_BUS \
                     INCLUDE_VXBUS
    INIT_AFTER       INCLUDE_PCI_BUS
}
