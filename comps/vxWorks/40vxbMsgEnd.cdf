/* 40vxbMsgEnd.cdf - Virtual Message END Device driver */

/*
 * Copyright (c) 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,22oct12,j_z  created
*/

Component       INCLUDE_MSG_NET {
        NAME            message Virtual END device driver
        SYNOPSIS        message Virtual END device driver
        _CHILDREN       FOLDER_DRIVERS
        PROTOTYPE       void msgEndRegister (void);
        MODULES         vxbMsgEnd.o
        INIT_RTN        msgEndRegister ();
        REQUIRES        INCLUDE_PLB_BUS
        _INIT_ORDER     hardWareInterFaceBusInit
}
