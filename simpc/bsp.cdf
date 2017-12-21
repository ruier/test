/* bsp.cdf - BSP-specific component descriptor file */

/*
 * Copyright (c) 1998,2002,2004,2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */

/*
modification history
--------------------
01d,29jul10,elp  LP64 support.
01c,02sep04,dbt  Added RAM_LOW_ADRS & RAM_HIGH_ADRS definitions.
01b,13dec02,jmp  added multiple interface support (SPR #73834).
01a,30Sep98,ms   written
*/

/*
DESCRIPTION

This file contains BSP-specific changes to the generic component
descriptor files.
*/

Component INCLUDE_END {
	INCLUDE_WHEN	INCLUDE_NET_INIT
}

Parameter IP_MAX_UNITS {
        DEFAULT         8
}

Parameter RAM_HIGH_ADRS {
	DEFAULT		0xffffffff
}

Parameter RAM_LOW_ADRS {
#ifdef _WRS_CONFIG_LP64
	DEFAULT		0x180010000
#else
	DEFAULT		0x10010000
#endif
}
