/* 40mkboot.cdf - Generic mkboot module */

/*
 * Copyright (c) 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
01a,19mar12,wyt  created.
*/

Component INCLUDE_MKBOOT {
  NAME         General mkboot module for Intel Architecture 
  SYNOPSIS     This component creates bootable disks for Intel Architecture. \
               It supplies functions to install IA MBR and FAT PBR codes.
  _CHILDREN    FOLDER_HARDWARE
  MODULES      mkboot.o
  INIT_RTN     mkbootInit();
  PROTOTYPE    void mkbootInit(void);
  _INIT_ORDER  hardWareInterFaceBusInit
  INIT_AFTER   INCLUDE_PLB_BUS
  REQUIRES     INCLUDE_DOSFS \
               INCLUDE_FS_MONITOR \
               INCLUDE_DEVICE_MANAGER \
               INCLUDE_XBD
}
