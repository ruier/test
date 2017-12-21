/* 40vxbIeee1394Ohci.cdf - Component Definition file for IEEE1394 OHCI controller */
                                                                                
/*
 * Copyright (c) 2011, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,11dec12,j_x  integration.
01a,29oct12,add  intial version.
*/


Folder    FOLDER_IEEE1394
    {
    NAME            IEEE1394
    _CHILDREN       FOLDER_BUSES
    SYNOPSIS        IEEE1394 Standard for a High Performance Serial Bus
    CHILDREN        INCLUDE_IEEE1394_OHCI         \
                    INCLUDE_IEEE1394_IODRV
    }
    
Component INCLUDE_IEEE1394_OHCI
    {
    NAME        IEEE1394 OHCI Controller Driver
    SYNOPSIS    Supports IEEE1394 OHCI Controllers
    MODULES     ieee1394Card.o ieee1394ConRom.o ieee1394IoDrv.o ieee1394Ohci.o ieee1394OhciAsyncMgr.o \
                ieee1394OhciBusMgr.o ieee1394OhciIsoMgr.o ieee1394Util.o
    _CHILDREN   FOLDER_IEEE1394
    _INIT_ORDER hardWareInterFaceBusInit
    PROTOTYPE   void vxbFwOhciDrvRegister(void);
    INIT_RTN    vxbFwOhciDrvRegister();
    REQUIRES    INCLUDE_PCI_BUS      \
                INCLUDE_ISR_DEFER    \
                INCLUDE_POSIX_FTRUNC \
                INCLUDE_POSIX_SHM    \
                INCLUDE_POSIX_SHM_API \
                INCLUDE_MMAP \
                INCLUDE_INTCTLR_DYNAMIC_LIB \
                INCLUDE_SHL 
    INIT_AFTER  INCLUDE_PCI_BUS
    }

#ifdef _WRS_CONFIG_LP64
Component INCLUDE_IEEE1394_OHCI 
    {
    REQUIRES += INCLUDE_CACHE_DMA32_LIB
    }
#endif

Component INCLUDE_IEEE1394_IODRV
    {
    NAME        IEEE1394 I/O Driver
    SYNOPSIS    IEEE1394 I/O Driver
    _CHILDREN   FOLDER_IEEE1394
    _INIT_ORDER usrRoot
    PROTOTYPE   STATUS fwIoDrvInstall(VOID);
    INIT_RTN    fwIoDrvInstall();
    REQUIRES    INCLUDE_IEEE1394_OHCI
    }
