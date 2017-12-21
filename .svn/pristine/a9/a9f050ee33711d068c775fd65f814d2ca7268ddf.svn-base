/* 40vxbM512xFecEnd.cdf - Freescale MPC512x FEC Enhanced Network Driver */

/*
 * Copyright (c) 2007-2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,20aug08,b_m  convert to library build.
01a,18sep07,b_m  written.
*/

Component   INCLUDE_M512XFEC_VXB_END {
    NAME        Freescale MPC512x FEC Enhanced Network Driver
    SYNOPSIS    Freescale MPC512x FEC Enhanced Network Driver
    MODULES     vxbM512xFecEnd.o
    REQUIRES    INCLUDE_PLB_BUS \
                INCLUDE_MII_BUS \
                INCLUDE_PARAM_SYS \
                INCLUDE_DMA_SYS    
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    m512xFecRegister();
    PROTOTYPE   void m512xFecRegister(void);
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_AFTER  INCLUDE_PLB_BUS
}
