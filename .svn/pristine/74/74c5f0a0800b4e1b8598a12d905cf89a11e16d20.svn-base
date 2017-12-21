/* 40vxbDm9000aEnd.cdf - DM9000a Ethernet End Driver */
                                                                                
/*
 * Copyright 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
                                                                                
/*
modification history
--------------------
01a,05jan09,d_l  written
*/

Component   DRV_VXBEND_DM9000A {
    NAME        DM9000a VxBus Enhanced Network Driver
    SYNOPSIS    DM9000a VxBus END driver
    _CHILDREN   FOLDER_DRIVERS
    _INIT_ORDER hardWareInterFaceBusInit
    PROTOTYPE   void dm9000aRegister(void);
    INIT_RTN    dm9000aRegister();
    REQUIRES    INCLUDE_PLB_BUS \
                INCLUDE_GENERICPHY \
                INCLUDE_PARAM_SYS
    INIT_AFTER  INCLUDE_PLB_BUS
}

