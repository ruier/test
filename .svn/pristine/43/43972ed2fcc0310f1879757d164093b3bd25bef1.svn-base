/* 40vxbXlrFmn.cdf - xlr FMN file */

/*
 * Copyright (c) 2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
01a,29sep07,l_z  created
*/

Component   INCLUDE_XLR_FMN {
    NAME        xlr FMN driver
    SYNOPSIS    xlr FMN driver
    MODULES     vxbXlrFmn.o
    SOURCE      $(WIND_BASE)/target/src/hwif/resource
    _CHILDREN   FOLDER_DRIVERS
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_RTN    xlrFmnRegister();
    PROTOTYPE   void xlrFmnRegister(void);
    REQUIRES    INCLUDE_PLB_BUS \
            INCLUDE_PARAM_SYS
    INIT_AFTER  INCLUDE_PLB_BUS

}
