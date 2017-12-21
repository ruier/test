/* 40hEndInit.cdf - Component configuration file */
                                                                                
/*
 * Copyright (c) 2005-2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
01b,23apr08,h_k  converted to source build. (CQ:105277)
01a,06jul05,tor  written

*/

Component       INCLUDE_HEND_PARAM_SYS {
        NAME            Hierarchical END Driver Parameter System
        SYNOPSIS        hEnd utility
	MODULES		hEndParamSys.o
	SOURCE		$(WIND_BASE)/target/src/hwif/hEnd
        _CHILDREN       FOLDER_DRIVERS
        REQUIRES        INCLUDE_VXBUS
}

Component       INCLUDE_HEND_NPT {
        NAME            Hierarchical END Driver configured as NPT
        SYNOPSIS        hEnd configuration
        _CHILDREN       FOLDER_DRIVERS
}
