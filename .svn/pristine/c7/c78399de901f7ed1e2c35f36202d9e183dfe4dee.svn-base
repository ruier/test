/* 40vxbLpc24xxEnd.cdf - LPC24XX EMAC END Driver */
                                                                                
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
01a,22dec08,x_s  written.
*/

Component DRV_VXBEND_LPC24XXEMAC {
    NAME        LPC24XX EMAC VxBus Enhanced Network Driver
    SYNOPSIS    LPC24XX EMAC VxBus Enhanced Network Driver
    MODULES     vxbLpc24xxEnd.o
    REQUIRES    INCLUDE_PLB_BUS \
                INCLUDE_PARAM_SYS
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    lpc24xxEndRegister();
    PROTOTYPE   void lpc24xxEndRegister(void);
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_AFTER  INCLUDE_PLB_BUS
}
