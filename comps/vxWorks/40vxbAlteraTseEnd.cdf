/* 40vxbAlteraTseEnd.cdf - Altera Triple Speed Ethernet END Driver */
                                                                                
/*
 * Copyright (c) 2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
                                                                                
/*
modification history
--------------------
01a,21nov08,l_z  created
*/

Component   DRV_VXBEND_ALTERA_TSE {
    NAME        Altera Triple Speed Ethernet VxBus Enhanced Network Driver
    SYNOPSIS    Altera Triple Speed Ethernet Enhanced Network Driver
    MODULES     vxbAlteraTseEnd.o
    REQUIRES    INCLUDE_PLB_BUS   \
                INCLUDE_PARAM_SYS 
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    alteraTseRegister ();
    PROTOTYPE   void alteraTseRegister (void);
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_AFTER  INCLUDE_PLB_BUS
}
