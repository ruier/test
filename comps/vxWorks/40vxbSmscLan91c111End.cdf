/* 40vxbSmscLan911c111End.cdf - Component configuration file */
                                                                                
/*
 * Copyright 2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
                                                                                
/*
modification history
--------------------
01a,18feb08,l_g  written

*/

Component INCLUDE_SMSCLAN91C111_VXB_END {
     NAME SMSC    LAN91C111 VxBus Enhanced Network Driver
     SYNOPSIS     SMSC LAN91C111 VxBus END driver
     _CHILDREN    FOLDER_DRIVERS
     _INIT_ORDER  hardWareInterFaceBusInit
     PROTOTYPE    void smscRegister(void);
     INIT_RTN     smscRegister();
     REQUIRES     INCLUDE_PLB_BUS \
     INCLUDE_MII_BUS \
     INCLUDE_GENERICPHY \
     INCLUDE_PARAM_SYS
     INIT_AFTER   INCLUDE_PLB_BUS
}

