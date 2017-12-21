/* 40vxbFslAxe.cdf - Freescale AXE Driver */

/*
 * Copyright (c) 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,09mar09,b_m  written.
*/

Component DRV_FW_FSL_AXE {
    NAME        Freescale AXE driver
    SYNOPSIS    Freescale AXE driver
    MODULES     vxbFslAxe.o
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_PLB_BUS
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    fslAxeRegister();
    PROTOTYPE   void fslAxeRegister(void);
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_AFTER  INCLUDE_PLB_BUS
}