/* 40vxbXlnxTemacEnd.cdf - Xlnx Temac End driver */
                                                                                
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
01b,17nov10,e_d  replace INCLUDE_TEMAC_VXB_END with DRV_VXBEND_XLNX_TEMAC
01a,05jan10,e_d  written

*/

Component	DRV_VXBEND_XLNX_TEMAC {
  NAME            vxbTemacEnd VxBus Enhanced Network Driver
  SYNOPSIS        temac vxBus END driver
  _CHILDREN       FOLDER_DRIVERS
  HDR_FILES       ../src/hwif/h/end/vxbXlnxTemacEnd.h
  _INIT_ORDER     hardWareInterFaceBusInit
  INIT_RTN        xlnxTemacRegister();
  NIT_AFTER       DRV_DMA_XLNX 
  REQUIRES        INCLUDE_PLB_BUS     \
                  INCLUDE_PARAM_SYS   \
                  INCLUDE_DMA_SYS
}