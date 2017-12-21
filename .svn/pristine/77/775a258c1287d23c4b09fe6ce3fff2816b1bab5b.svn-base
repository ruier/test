/* 40vxbM512xFifoc.cdf - MPC512x PSC Centralized FIFO Controller Driver */

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
01a,26feb09,b_m  created
*/

Component DRV_MF_M512X_FIFOC {
    NAME        MPC512x PSC Centralized FIFO Controller Driver
    SYNOPSIS    MPC512x PSC Centralized FIFO Controller Driver
    MODULES     vxbM512xFifoc.o
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_PLB_BUS \
                INCLUDE_INTCTLR_LIB
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    m512xFifocRegister();
    PROTOTYPE   void m512xFifocRegister(void);
    INIT_AFTER  INCLUDE_PLB_BUS
    _INIT_ORDER hardWareInterFaceBusInit
}