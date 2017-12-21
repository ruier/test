/* 40vxbStmSpear13xxGmacEnd.cdf -STM SPEAr 13xx GMAC END Driver */
                                                                                
/*
 * Copyright (c) 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
                                                                                
/*
modification history
--------------------
01a,27dec10,clx  create
*/

Component   DRV_VXBEND_STMSPEAR13XX_GEMAC {
    NAME        STM SPEAr13xx GMAC VxBus Enhanced Network Driver
    SYNOPSIS    STM SPEAr13xx GMAC VxBus Enhanced Network Driver
    MODULES     vxbStmSpear13xxGmacEnd.o
    REQUIRES    INCLUDE_PLB_BUS   \
                INCLUDE_PARAM_SYS \
                INCLUDE_MII_BUS   \
                INCLUDE_GENERICPHY
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    stmSpear13xxGmacRegister();
    PROTOTYPE   void stmSpear13xxGmacRegister(void);
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_AFTER  INCLUDE_PLB_BUS
}
