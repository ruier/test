/* 40vxbXlpGmacEnd.cdf - Component configuration file */
                                                                                
/*
 * Copyright 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
                                                                                
/*
modification history
--------------------
01a,14mar11,x_f  written
*/

Component   DRV_VXBEND_XLPGMAC {
    NAME        NetLogic Micro GMAC VxBus Enhanced Network Driver
    SYNOPSIS    NetLogic Micro GMAC VxBus END driver
    MODULES     vxbXlpGmacEnd.o
    SOURCE      $(WIND_BASE)/target/src/hwif/end
    _CHILDREN   FOLDER_DRIVERS
    _INIT_ORDER hardWareInterFaceBusInit
    PROTOTYPE   void xlpGmacRegister(void);
    INIT_RTN    xlpGmacRegister();
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_PCI_BUS \
                INCLUDE_GENERICPHY \
                INCLUDE_PARAM_SYS
    INIT_AFTER  INCLUDE_PCI_BUS
}

