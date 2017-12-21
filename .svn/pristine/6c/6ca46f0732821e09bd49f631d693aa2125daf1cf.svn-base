/* 40vxbAltSocFpgaDwEnd.cdf - Altera Dw EMAC Enhanced Network Driver */

/*
 * Copyright (c) 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,13mar13,swu  created
*/

Component INCLUDE_ALT_SOC_GEN5_DW_END {
    NAME        Altera Dw EMAC Enhanced Network Driver
    SYNOPSIS    This component adds support of driver for Designware EMAC IP in Altera SoC FPGA.
    _CHILDREN   FOLDER_DRIVERS
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_RTN    vxbAltSocGen5DwEndRegister();
    MODULES     vxbAltSocGen5DwEnd.o
    REQUIRES    INCLUDE_PLB_BUS   \
                INCLUDE_PARAM_SYS \
                INCLUDE_MII_BUS
    PROTOTYPE   void vxbAltSocGen5DwEndRegister(void);
    INIT_AFTER  INCLUDE_PLB_BUS 
}
