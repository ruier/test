/* 10vxbNonVolLib.cdf - NvRam Component configuration file */ 

/*
 * Copyright (c) 2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute or otherwise make use of this software
 * may be licensed only pursuant to the terms of an applicable
 *  Wind River license agreement.
 */

/*
modification history
-------------------- 
01a,18jul07,tor created
*/

Component       INCLUDE_NON_VOLATILE_RAM {
        NAME            Non Volatile Memory Library
        SYNOPSIS        Non Volatile Memory Library
        INIT_RTN        vxbNonVolLibInit();
	REQUIRES	INCLUDE_VXBUS
}
