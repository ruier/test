/* 40vxbIntelGcu.cdf - Component configuration file */
                                                                                
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
01c,25nov08,j_z  remove "\" after INCLUDE_PCI_BUS.
01b,20aug08,j_z  rename INCLUDE_DRV_INTEL_GCU to DRV_MII_INTEL_GCU
                 add MODULES,PROTOTYPE
01a,15Oct07,d_z  created
*/

Component   DRV_MII_INTEL_GCU {
        NAME        Intel VxBus general configuration unit Driver
        SYNOPSIS    Intel VxBus general configuration unit Driver
        MODULES     vxbIntelGcu.o
	_CHILDREN   FOLDER_DRIVERS
        _INIT_ORDER hardWareInterFaceBusInit
        INIT_RTN    vxbIntelGcuRegister();
	PROTOTYPE   void vxbIntelGcuRegister(void);
        REQUIRES    INCLUDE_PLB_BUS \
                    INCLUDE_PCI_BUS
        INIT_AFTER  INCLUDE_PCI_BUS
}
