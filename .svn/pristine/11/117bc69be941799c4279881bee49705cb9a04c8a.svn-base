/* 40vxbComcertoGemacEnd.cdf - Mindspeed Comcerto 100 GEMAC END Driver */
                                                                                
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
01a,11jun08,b_m  written
*/

Component   DRV_VXBEND_COMCERTO_GEMAC {
    NAME        Mindspeed Comcerto 100 GEMAC VxBus Enhanced Network Driver
    SYNOPSIS    Mindspeed Comcerto 100 GEMAC VxBus Enhanced Network Driver
    MODULES     vxbComcertoGemacEnd.o
    REQUIRES    INCLUDE_PLB_BUS \
                INCLUDE_PARAM_SYS
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    comcertoGemacRegister();
    PROTOTYPE   void comcertoGemacRegister(void);
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_AFTER  INCLUDE_PLB_BUS
}
