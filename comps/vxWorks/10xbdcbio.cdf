/* 10xbdcbio.cdf - XBD CBIO Device Wrapper  */

/*
 * Copyright 2006, 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
02l,02jun10,jxu  removed INCLUDE_XBD_CBIO_DEV from LP64 build.
01a,13oct06,jlk  written.
*/

/*
DESCRIPTION
This file contains the description for the XBD/CBIO device wrapper.
*/
#ifndef _WRS_CONFIG_LP64
Component INCLUDE_XBD_CBIO_DEV
    {
    NAME           XBD CBIO Device
    SYNOPSIS       XBD CBIO Device Wrapper
    LINK_SYMS      xbdCbioDevCreate
    REQUIRES       INCLUDE_XBD INCLUDE_CBIO
    }
#endif
