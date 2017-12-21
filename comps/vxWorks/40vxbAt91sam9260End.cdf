/* 40vxbAt91sam9260End.cdf - Component configuration file */

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
01b,28oct08,pgh  Remove extra back-slash
01a,07apr08,x_s  written.
*/

Component DRV_VXBEND_AT91EMAC {
    NAME            AT91 EMAC Enhanced Network Driver
    SYNOPSIS        AT91 EMAC Enhanced Network Driver
    HDR_FILES       ../src/hwif/h/end/vxbAt91sam9260End.h
    _CHILDREN       FOLDER_DRIVERS
    _INIT_ORDER     hardWareInterFaceBusInit
    INIT_RTN        at91EmacRegister();
    REQUIRES        INCLUDE_PLB_BUS \
                    INCLUDE_PARAM_SYS \
                    INCLUDE_MII_BUS
    INIT_AFTER      INCLUDE_PLB_BUS
}
