/* 40vxbIntelEdma.cdf - Component configuration file */
                                                                                
/*
 * Copyright (c) 2007-2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
                                                                                
/*
modification history
--------------------
01b,20aug08,j_z  rename INCLUDE_DRV_INTEL_EDMA to DRV_DMA_INTEL_EDMA
                 add MODULES,PROTOTYPE
01a,10Oct07,d_z  created
*/

Component   DRV_DMA_INTEL_EDMA {
        NAME        Intel Enhanced DMA VxBus Driver
        SYNOPSIS    Intel Enhanced DMA VxBus Driver
        MODULES     vxbIntelEdma.o
	_CHILDREN   FOLDER_DRIVERS
        _INIT_ORDER hardWareInterFaceBusInit
        INIT_RTN    vxbIntelEdmaRegister();
	PROTOTYPE   void vxbIntelEdmaRegister(void);
        REQUIRES    INCLUDE_PLB_BUS \
                    INCLUDE_PCI_BUS \
                    INCLUDE_DMA_SYS
        INIT_AFTER  INCLUDE_PCI_BUS
}
