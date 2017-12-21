/* 40vxbApmPProQman.cdf - APM PacketPro Queue Manager driver file */

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
01a,01jun11,x_z  Created.
*/

Component DRV_APM_PPRO_QMAN {
    NAME        APM PacketPro Queue Manager driver
    SYNOPSIS    APM PacketPro Queue Manager driver
    MODULES     vxbApmPProQman.o
    SOURCE      $(WIND_BASE)/target/src/hwif/resource
    _CHILDREN   FOLDER_DRIVERS
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_RTN    vxbApmPProQmanRegister ();
    PROTOTYPE   void vxbApmPProQmanRegister (void);
    REQUIRES    INCLUDE_VXBUS
    INIT_AFTER  INCLUDE_PLB_BUS
}