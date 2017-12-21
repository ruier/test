/* 40vxbIntelIchStorage.cdf - Component Definition file for 
   Intel ICH [s]ata controller */
                                                                                
/* Copyright (c) 2007-2008,2010 Wind River Systems, Inc. */
                                                                                
/*
modification history
--------------------
01c,16sep10,j_l  Include DMA32 only when needed (Fix for WIND00230745)
01b,15sep08,pmr  fix typo in SYNOPSIS (WIND00130404)
01a,22aug07,pmr  written

*/

Component       INCLUDE_DRV_STORAGE_INTEL_ICH {
    NAME             Intel ICH SATA Controller
    SYNOPSIS         Supports 82801 Intel Serial/Parallel ATA controllers
    MODULES	     vxbIntelIchStorage.o
    _CHILDREN        FOLDER_DRIVERS
    _INIT_ORDER      hardWareInterFaceBusInit
    PROTOTYPE	     void vxbIntelIchStorageRegister(void);
    INIT_RTN         vxbIntelIchStorageRegister();
    REQUIRES         INCLUDE_PCI_BUS \
                     INCLUDE_VXBUS \
                     INCLUDE_FS_MONITOR \
                     INCLUDE_DMA_SYS
    INIT_AFTER       INCLUDE_PCI_BUS
}

#ifdef	_WRS_CONFIG_LP64
Component       INCLUDE_DRV_STORAGE_INTEL_ICH {
    REQUIRES         += INCLUDE_CACHE_DMA32_LIB
}
#endif	/* _WRS_CONFIG_LP64 */

Component       INCLUDE_DRV_STORAGE_INTEL_ICH_SHOW {
    NAME             Intel ICH SATA Controller Show Routines
    SYNOPSIS         Supports 82801 Intel Serial/Parallel ATA controllers
    MODULES	     vxbIntelIchStorageShow.o
    _CHILDREN        FOLDER_DRIVERS
    _INIT_ORDER      hardWareInterFaceBusInit
    PROTOTYPE	     STATUS ichAtaShowInit(void);
    INIT_RTN         ichAtaShowInit();
    REQUIRES         INCLUDE_DRV_STORAGE_INTEL_ICH
    INIT_AFTER       INCLUDE_DRV_STORAGE_INTEL_ICH
}
