/* 40vxbPiixStorage.cdf - Component Definition file for 
   Intel PIIX sata controller */
                                                                                
/*
 * Copyright (c) 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
                                                                                
/*
modification history
--------------------
01a,11sep11,e_d  derived from 40vxbIntelIchStorage.cdf/01c.
*/

Component       INCLUDE_DRV_STORAGE_PIIX {
    NAME        Intel PIIX SATA Controller
    SYNOPSI     Supports 82801 Intel Serial/Parallel ATA controllers
    MODULES     vxbPiixStorage.o
    _CHILDREN   FOLDER_DRIVERS
    _INIT_ORDER hardWareInterFaceBusInit
    PROTOTYPE   void vxbIntelIchStorageRegister(void);
    INIT_RTN    vxbPiixStorageRegister();
    REQUIRES    INCLUDE_PCI_BUS \
                INCLUDE_VXBUS \
                INCLUDE_FS_MONITOR \
                INCLUDE_DMA_SYS
    INIT_AFTER  INCLUDE_PCI_BUS
}

#ifdef _WRS_CONFIG_LP64
Component       INCLUDE_DRV_STORAGE_PIIX {
    REQUIRES    += INCLUDE_CACHE_DMA32_LIB
}
#endif /* _WRS_CONFIG_LP64 */