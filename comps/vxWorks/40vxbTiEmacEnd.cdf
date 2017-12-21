/* 40vxbTiEmacEnd.cdf - Component configuration file */

/*
 * Copyright (c) 2007-2008, 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01c,11oct10,hcl  change emac driver file name.
01b,03jul08,d_l  changed component name.
01a,07dec07,b_m  written.
*/

Component   DRV_VXBEND_TI_EMAC {
    NAME            TI EMAC Enhanced Network Driver
    SYNOPSIS        TI EMAC Enhanced Network Driver
    HDR_FILES       ../src/hwif/h/end/vxbTiEmacEnd.h
    _CHILDREN       FOLDER_DRIVERS
    _INIT_ORDER     hardWareInterFaceBusInit
    INIT_RTN        tiEmacRegister();
    REQUIRES        INCLUDE_PLB_BUS \
                    INCLUDE_PARAM_SYS
    INIT_AFTER      INCLUDE_PLB_BUS
}
