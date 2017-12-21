/* 01visionTmdDriver.cdf - Wind River visionTMD component description file */

/*
 * Copyright (c) 2001, 2005, 2007 Wind River Systems, Inc.  
 *
 * The right to copy, distribute, modify or otherwise make use  of this 
 * software may be licensed only pursuant to the terms of an applicable Wind 
 * River license agreement.
 */

/*
modification history
--------------------
01e,11apr11,l_c  - fix WIND00158473
01d,06dec07,rlp  Removed obsolete product references (CQ:WIND00112534).
01c,27sep05,bpn  Added missing WDB_MTU parameter defined in 00wdb.cdf
                 (SPR#112365).
01b,30nov01,g_h  Cleaning for T2.2
01a,30may01,g_h  written

*/

/*
DESCRIPTION
This file contains descriptions for the TM Driver.
The configuration tool searches for .cdf files in four
places; here, arch/<arch>, config/<bsp>, and the project directory.
*/

Component  INCLUDE_WDB_COMM_VTMD {

    NAME         WDB visionTMD connection
    SYNOPSIS     Transparent Mode Driver connection using Wind River ICE SX or Wind River ICE 2
    CONFIGLETTES wdbVisionDriver.c
    CFG_PARAMS   TMD_DEFAULT_POLL_DELAY \
                 WDB_COMM_VTMD \
                 WDB_MTU
    MACRO_NEST	 INCLUDE_WDB
    HDR_FILES    wdb/wdbUdpLib.h wdb/wdbLib.h wdb/wdbLibP.h drv/wdb/vision/wdbVisionPktDrv.h
    _CHILDREN    SELECT_WDB_COMM_TYPE
    REQUIRES     INCLUDE_WDB
    HELP         tgtsvr wdbVisionTMDPktDrv WDB
}

Parameter TMD_DEFAULT_POLL_DELAY {

    SYNOPSIS     clock tick interval to poll for data
    DEFAULT      2
    TYPE         uint
}

Parameter WDB_COMM_VTMD {

    SYNOPSIS     WDB Comm Device Number (used for switch/case)
    DEFAULT      8
    TYPE         uint
}

