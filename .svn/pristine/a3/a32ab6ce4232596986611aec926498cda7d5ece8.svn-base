/* 40vxbFslSataStorage.cdf - Component Definition file for 
   Freescale Sata controller */
                                                                                
/* Copyright (c) 2008 Wind River Systems, Inc. */
                                                                                
/*
modification history
--------------------
01b,15sep08,pmr  fix typo in SYNOPSIS (WIND00130404)
01a,20feb08,x_s  written
*/

Component       DRV_STORAGE_FSLSATA {
    NAME             FSL SATA Controller
    SYNOPSIS         FSL Serial ATA controllers
    MODULES	     vxbFslSataStorage.o
    _CHILDREN        FOLDER_DRIVERS
    _INIT_ORDER      hardWareInterFaceBusInit
    PROTOTYPE	     void vxbFslSataStorageRegister(void);
    INIT_RTN         vxbFslSataStorageRegister();
    REQUIRES         INCLUDE_VXBUS \
		     INCLUDE_PLB_BUS
   }
