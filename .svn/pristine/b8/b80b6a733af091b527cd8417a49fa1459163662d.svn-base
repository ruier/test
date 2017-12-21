/* 40uart.cdf - Component configuration file */

/*
 * Copyright (c) 2005-2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
02d,11may12,l_z  add PROTOTYPE for DRV_SIO_OCTEON
02c,22aug11,c_t  add AMIO parameter.
02b,17jun11,rab  Add vxbZynq7kSio.o
02a,26oct10,j_z  add Cert Hypervisor AMIO SIO.
01z,15jul10,d_c  Remove vxbOcteonSio.c as a configlette - it is now built with
                 target libraries
01y,28jun10,pch  Crown Beach small footprint
01x,03mar10,pch  Octeon small footprint
01w,09jul09,x_s  add support for small footprint.
01v,19jun09,d_l  add INCLUDE_ISR_DEFER for DRV_SIO_DAVINCI.
01u,19oct08,jrp  WIND00136163
01t,19apr08,x_s  add DRV_SIO_AT91.
01s,08jul08,d_l  add DRV_SIO_DAVINCI.
01r,06mar08,l_z  add DRV_SIO_IMX.
01q,18apr08,b_m  add DRV_SIO_M5XXX_PSC.
01p,22aug07,h_k  removed DEFAULTS.
01o,24may07,h_k  changed ns16550sio and coldFireSio binary release.
01n,04may07,jmt  Add DRV_SIO_OCTEON
01m,27apr07,h_k  added DRV_SIO_PRIMECELL.
01l,07feb07,h_k  added dependency of INCLUDE_ISR_DEFER to DRV_SIO_NS16550 and
                 DRV_SIO_SB1.
01k,12feb07,wap  Coldfire SIO doesn't depend on PCI
01j,28sep06,pmr  add DRV_SIO_SB1
01i,30aug06,pmr  added DRV_SIO_IXP400
01h,25aug06,h_k  moved DRV_SIO_SHSCIF to sh/20bsp.cdf.
01g,01aug06,h_k  removed CONFIGLETTES from DRV_SIO_SHSCIF.
01f,29jun06,dtr  removed PCI BUS dependency from DRV_SIO_NS16550.
01e,22jun06,h_k  added DRV_SIO_SHSCIF.
01d,20apr06,pdg  added coldFire serial driver support
01c,11jan06,mdo  Change INCLUDE_NS16550_SIO to DRV_SIO_NS16550
01b,03jan06,mdo  Don't automatically include when we define INCLUDE_VXBUS
01a,22nov05,mdo  break away from main vxBus cdf

*/


/********************************************************************************
* ns16550 SIO for vxBus
*
*/
Component DRV_SIO_NS16550 {
    NAME            UART support for ns16550-compatible devices
    SYNOPSIS        ns16550 SIO support
    MODULES         vxbNs16550Sio.o
    SOURCE          ../../../src/hwif/sio
    INIT_RTN        ns16550SioRegister();
    PROTOTYPE       void ns16550SioRegister (void);
    _INIT_ORDER     hardWareInterFaceBusInit
#ifdef _WRS_CONFIG_VXBUS_BASIC
    REQUIRES        INCLUDE_VXBUS \
                    INCLUDE_PLB_BUS \
                    INCLUDE_SIO_UTILS
#else	/* _WRS_CONFIG_VXBUS_BASIC */
    REQUIRES        INCLUDE_VXBUS \
                    INCLUDE_PLB_BUS \
                    INCLUDE_SIO_UTILS \
                    INCLUDE_ISR_DEFER
#endif /* _WRS_CONFIG_VXBUS_BASIC */
    _CHILDREN       FOLDER_DRIVERS
}

/********************************************************************************
* coldFire SIO for vxBus
*
*/
Component DRV_SIO_COLDFIRE {
    NAME            UART support for coldFire
    SYNOPSIS        coldFire SIO support
    MODULES         vxbColdFireSio.o
    SOURCE          ../../../src/hwif/sio
    INIT_RTN        coldFireSioRegister();
    PROTOTYPE       void coldFireSioRegister (void);
    _INIT_ORDER     hardWareInterFaceBusInit
    INIT_AFTER      INCLUDE_PCI_BUS
    REQUIRES        INCLUDE_VXBUS \
                    INCLUDE_PLB_BUS \
                    INCLUDE_PCI_BUS \
                    INCLUDE_SIO_UTILS
    _CHILDREN       FOLDER_DRIVERS
}

/********************************************************************************
* ixp400 SIO for vxBus
*
*/
Component DRV_SIO_IXP400 {
    NAME            UART support for ixp400
    SYNOPSIS        ixp400 SIO support
    MODULES         vxbIxp400Sio.o
    SOURCE          ../../../src/hwif/sio
    INIT_RTN        ixp400SioRegister();
    PROTOTYPE       void ixp400SioRegister (void);
    _INIT_ORDER     hardWareInterFaceBusInit
    INIT_AFTER      INCLUDE_PLB_BUS
    REQUIRES        INCLUDE_VXBUS \
                    INCLUDE_PLB_BUS \
                    INCLUDE_SIO_UTILS
    _CHILDREN       FOLDER_DRIVERS
}

/********************************************************************************
* sb1 SIO for vxBus
*
*/
Component DRV_SIO_SB1 {
    NAME            UART support for sb1
    SYNOPSIS        Broadcom SiByte SIO support
    MODULES	    vxbSb1DuartSio.o
    SOURCE          ../../../src/hwif/sio
    INIT_RTN        vxbSb1DuartSioRegister();
    PROTOTYPE       void vxbSb1DuartSioRegister (void);
    _INIT_ORDER     hardWareInterFaceBusInit
    INIT_AFTER      INCLUDE_PLB_BUS
    REQUIRES        INCLUDE_VXBUS \
                    INCLUDE_PLB_BUS \
                    INCLUDE_SIO_UTILS \
                    INCLUDE_ISR_DEFER
    _CHILDREN       FOLDER_DRIVERS
}

/********************************************************************************
* ARM PrimeCell SIO for vxBus
*
*/
Component DRV_SIO_PRIMECELL {
    NAME            UART support for PrimeCell
    SYNOPSIS        ARM PrimeCell SIO support
    MODULES	    vxbPrimeCellSio.o
    SOURCE          ../../../src/hwif/sio
    INIT_RTN        vxbPrimeCellSioRegister();
    PROTOTYPE       void vxbPrimeCellSioRegister (void);
    _INIT_ORDER     hardWareInterFaceBusInit
    INIT_AFTER      INCLUDE_PLB_BUS
    REQUIRES        INCLUDE_VXBUS \
                    INCLUDE_PLB_BUS \
                    INCLUDE_SIO_UTILS \
                    INCLUDE_ISR_DEFER
    _CHILDREN       FOLDER_DRIVERS
}

/********************************************************************************
* Octeon SIO for vxBus
*
*/
Component DRV_SIO_OCTEON {
    NAME            UART support for Cavium Octeon
    SYNOPSIS        Octeon SIO support
    INIT_RTN        octeonSioRegister();
    PROTOTYPE       void octeonSioRegister (void);
    _INIT_ORDER     hardWareInterFaceBusInit
#ifdef _WRS_CONFIG_VXBUS_BASIC
    REQUIRES        INCLUDE_VXBUS \
                    INCLUDE_PLB_BUS \
                    INCLUDE_SIO_UTILS
#else	/* _WRS_CONFIG_VXBUS_BASIC */
    REQUIRES        INCLUDE_VXBUS \
                    INCLUDE_PLB_BUS \
                    INCLUDE_SIO_UTILS \
                    INCLUDE_ISR_DEFER 
#endif /* _WRS_CONFIG_VXBUS_BASIC */
    _CHILDREN       FOLDER_DRIVERS
}

/********************************************************************************
* MPC512x/52xx PSC SIO for vxBus
*
*/

Component DRV_SIO_M5XXX_PSC {
    NAME            UART support for MPC5XXX PSC
    SYNOPSIS        MPC5xxx PSC SIO support
    MODULES         vxbM5xxxPscSio.o
    SOURCE          ../../../src/hwif/sio
    INIT_RTN        m5xxxPscSioRegister();
    PROTOTYPE       void m5xxxPscSioRegister (void);
    _INIT_ORDER     hardWareInterFaceBusInit
    REQUIRES        INCLUDE_VXBUS \
                    INCLUDE_PLB_BUS \
                    INCLUDE_SIO_UTILS
    _CHILDREN       FOLDER_DRIVERS
}

/********************************************************************************
* IMX SIO for vxBus
*
*/
Component DRV_SIO_IMX {
    NAME            UART support for i.MX Series CPUs
    SYNOPSIS        i.MX Series CPUs SIO support
    MODULES         vxbImxSio.o
    SOURCE          ../../../src/hwif/sio
    INIT_RTN        vxbImxSioRegister();
    PROTOTYPE       void vxbImxSioRegister (void);
    _INIT_ORDER     hardWareInterFaceBusInit
    INIT_AFTER      INCLUDE_PLB_BUS
    REQUIRES        INCLUDE_VXBUS \
                    INCLUDE_PLB_BUS \
                    INCLUDE_SIO_UTILS
    _CHILDREN       FOLDER_DRIVERS
}

/*******************************************************************************
* TI Davinci SIO for vxBus
*
*/
Component DRV_SIO_DAVINCI {
    NAME            UART support for TI Davinci
    SYNOPSIS        TI Davinci SIO support
    MODULES         vxbDavinciSio.o
    SOURCE          ../../../src/hwif/sio
    INIT_RTN        dv16550SioRegister();
    PROTOTYPE       void dv16550SioRegister (void);
    _INIT_ORDER     hardWareInterFaceBusInit
    INIT_AFTER      INCLUDE_PLB_BUS
#ifndef _WRS_CONFIG_VXBUS_BASIC
    REQUIRES        INCLUDE_VXBUS \
                    INCLUDE_PLB_BUS \
                    INCLUDE_SIO_UTILS \
                    INCLUDE_ISR_DEFER 
#else
    REQUIRES        INCLUDE_VXBUS \
                    INCLUDE_PLB_BUS \
                    INCLUDE_SIO_UTILS
#endif /* !_WRS_CONFIG_VXBUS_BASIC */
    _CHILDREN       FOLDER_DRIVERS
}

/*******************************************************************************
* AT91 SIO for vxBus
*
*/
Component DRV_SIO_AT91 {
    NAME            UART support for TI at91
    SYNOPSIS        At91 SIO support
    MODULES         vxbAt91sam9260Sio.o
    SOURCE          ../../../src/hwif/sio
    INIT_RTN        vxbAt91SioRegister();
    PROTOTYPE       void vxbAt91SioRegister (void);
    _INIT_ORDER     hardWareInterFaceBusInit
    INIT_AFTER      INCLUDE_PLB_BUS
    REQUIRES        INCLUDE_VXBUS \
                    INCLUDE_PLB_BUS \
                    INCLUDE_SIO_UTILS 
    _CHILDREN       FOLDER_DRIVERS
}

/*******************************************************************************
* Zynq-7000 SIO for vxBus
*
*/

Component DRV_SIO_ZYNQ7K {
    NAME            UART support for Xilinx Zynq-7000 
    SYNOPSIS        Zynq-7000 SIO support
    MODULES         vxbZynq7kSio.o
    SOURCE          ../../../src/hwif/sio
    INIT_RTN        vxbZynqSioRegister();
    PROTOTYPE       void vxbZynqSioRegister (void);
    _INIT_ORDER     hardWareInterFaceBusInit
    INIT_AFTER      INCLUDE_PLB_BUS
    REQUIRES        INCLUDE_VXBUS \
                    INCLUDE_PLB_BUS \
                    INCLUDE_SIO_UTILS 
    _CHILDREN       FOLDER_DRIVERS
}

#if defined(_WRS_CONFIG_WRHV_GUEST_CERT) ||\
    defined(_WRS_CONFIG_WRHV_GUEST_CERT_DEBUG)

/*******************************************************************************
* Cert Hypervisor AMIO SIO for vxBus
*
*/

Parameter AMIO_SYSTEM_DEVICE  {
    NAME              AMIO system device name
    SYNOPSIS          system device name configured in xml for AMIO client 
    TYPE              char *
    DEFAULT           "amioClientVB1"
}

Component DRV_SIO_AMIO {
    NAME            AMIO SIO Driver
    SYNOPSIS        AMIO SIO Driver
    MODULES         vxbVbAmioSio.o
    SOURCE          ../../../src/hwif/sio
    INIT_RTN        amioSioRegister();
    PROTOTYPE       void amioSioRegister (void);
    _INIT_ORDER     hardWareInterFaceBusInit
    INIT_AFTER      INCLUDE_PLB_BUS
    REQUIRES        INCLUDE_VXBUS   \
                    INCLUDE_PLB_BUS \
                    INCLUDE_SIO_UTILS
    CFG_PARAMS      AMIO_SYSTEM_DEVICE
    _CHILDREN       FOLDER_DRIVERS
}
#endif / *_WRS_CONFIG_WRHV_GUEST_CERT_DEBUG */
