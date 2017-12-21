/* 40vxblsiMacEnd.cdf - LSI MAC END Driver */

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
01a,02apr10,x_z  written.
*/

Component INCLUDE_LSI_MAC_VXB_END {
    NAME        LSI MAC VxBus Enhanced Network Driver
    SYNOPSIS    LSI MAC VxBus Enhanced Network Driver
    MODULES     vxbLsiMacEnd.o
    REQUIRES    INCLUDE_PLB_BUS     \
                INCLUDE_PARAM_SYS   \
                INCLUDE_DMA_SYS
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    lsiMacVxbEndRegister();
    PROTOTYPE   void lsiMacVxbEndRegister(void);
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_AFTER  INCLUDE_PLB_BUS
}
