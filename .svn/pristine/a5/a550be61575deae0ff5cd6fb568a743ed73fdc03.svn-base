/* vxbDtsecMdio.c - driver for dTSEC MDIO port */

/*
 * Copyright (c) 2009-2011, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01i,27feb13,wyt  Fix coverity error. (WIND00401412)
01h,16mar11,b_m  Add BSP hook to enable mdio mux.
01g,26may10,wap  Make sure to initialize the TBIPA setting for DTSEC0
01f,25jan10,wap  Fix GPIO masks in PHY write method too
01e,11jan10,wap  Use proper mask when selecting GPIO pin 0 value
01d,21oct09,wap  Be sure to clear the MIIMCOM register before attempting a
                 read, as Linux sometimes leaves the read bit set after an
                 access
01c,24sep09,wap  Set up GPIO support for FSL P4080DS board
01b,09sep09,wap  Isolate from DTSEC
01a,28jul09,wap  written
*/

/*
DESCRIPTION

This module provides support for accessing the MDIO interface on
the Freescale DTSEC network controllers. It is mainly
used for sharing access to the MDIO port among multiple cores in an
assymetric multiprocessing (AMP) configuration.

Although there are several DTSEC controllers in a single Freescale
P4080 processor, there is only one set of MDIO pins to manage them,
which are tied to the MDIO registers on DTSEC0. This is done to conserve
the use of pins: most Freescale integrated processors contain many
different peripheral functions, such as UARTs, PCI bridges, PCIe
bridges, USB host controllers, and ethernet MACs, and all of these
require pins on the processor package to interface with the outside
world. The MDIO bitbang protocol on MII-compliant PHYs allows up to
32 of them to share a single pair of MDIO pins, so rather than giving
each TSEC port its own pins, Freescale only provides them for DTSEC0.

This complicated management of the PHYs somewhat, even in single
core configurations: in our implementation, we want to allow each DTSEC
to manage its own PHY. In a single core configuration, this requires
allowing the other ports to forward their PHY read and write requests
to DSEC0. In a multi-core AMP configuration, things become even more
complex: if we want to divide up the DTSECs between cores, then only the
core that gets DTSEC0 has access to the MDIO port. This would leave the
core without access to DTSEC0 unable to properly perform link
autonegotiation, which would seriously impair its operation.

In the single core case, the other DTSECs can simply be told to
forward their PHY read/write requests to DTSEC0 using VxBus method
calls. DTSEC0 then arbitrates access to the MDIO port among the
other DTSEC ports and itself. In the AMP case, we can have all DTSEC
ports forward their PHY read/write requests to this driver instead,
and we can arbitrate access not only among the multiple DTSEC ports,
but among multiple cores as well.

To do this, we implement a very simple spinlock mechanism using a
shared memory region and vxTas(). Each core runs its own instance
of the vxbDtsecMdio driver, and both instances are told the address of a
single 32-bit anchor word in a memory region accessible to both cores.
Each core that wants to use the MDIO registers must set the anchor
word to a non-zero value with vxTas(). If the anchor word is already set
(indicating the other core is currently accessing the registers, the
core will taskDelay() for one tick and then try again.

Typically, the miiBusMonitor task will poll each PHY's status register once
every two seconds, also using a taskDelay(). The effect of having the
PHY read and write methods in this module back off when contention is
detected has the effect of desynchronizing the miiBusMonitor tasks running
on each core, such that the two tasks will eventually stop colliding
with each other very frequently.

Note that with the Freescale P4080DS board, there is an additional
complication. The board design incorporates an on-board Vitesse RGMII
PHY, which is typically connected to DTSEC4 (first DTSEC on the second
frame manager), and comes with a riser card in slot 5 with a Vitesse
SGMII PHY, which connects to DTSEC0, 1, 2 and 3 (the four DTSEC ports
on the first frame manager). Unfortunately, the MDIO pins for both PHYs
are not connected directly to the MDIO pins on the P4080: instead, they
are connected through a multiplexer that is manipulated using GPIO pins.
Both the RGMII and SGMII PHYs can not be managed simultaneously, even
though they are set for non-conflicting addresses. To select the RGMII
PHY, GPIO pins 0 and 1 must both be cleared. To select the SGMII PHY,
they must both be set.

Furthermore, it is possible to configure the board with two SGMII
riser cards, thereby making all 8 DTSEC ports available. The PHYs on
both cards use the same MII management addresses however (28, 29, 30
and 31), and again we must use GPIO pins to select which PHY to access.

The available selections on the P4080DS board for the DTSEC MDIO port
are:

\cs
	GPIO PINS [0:1]		Device
        ---------------		------
	0 0			On-board Vitesse RGMII
	0 1			Slot 3 SGMII
	1 0			Slot 4 SGMII
	1 1			Slot 5 SGMII (default)
\ce

However, with Freescale P3041/P5020DS board, Freescale changes the way
that MII management pins are multiplexed. The on-board FPGA (ngPIXIS)
has an register to manipulate the pin mux instead of a set of GPIO pins
that was used on P4080DS.

The available selections on the P3041/P5020DS board for the DTSEC MDIO
port are:

\cs
    EMI1_SEL PINS [0:1:2]   Device
    ---------------------   ------
    0 0 0                   On-board Vitesse RGMII
    1 0 0                   Slot 1 SGMII
    0 1 0                   Slot 2 SGMII
    1 1 0                   Slot 5 SGMII
    0 0 1                   Slot 6 SGMII (default)
    1 0 1                   Slot 7 SGMII
\ce

BOARD LAYOUT
The DTSEC MDIO port is directly integrated into the CPU. All configurations
are jumperless.

EXTERNAL INTERFACE
The driver provides a vxBus external interface. The only exported
routine is the vxbDtsecEndRegister() function, which registers the driver
with VxBus. Since the DTSEC MDIO port is a processor local bus device, each
device instance must be specified in the hwconf.c file in a BSP.
The hwconf entry must specify the following parameters:
 
\is
 
\i <regBase>
Specifies the base address where the controller's CSR registers are mapped
into the host's address space. All register offsets are computed relative
to this address.

\i <fman0Gpio>
Specifies the settings of GPIO pins 0 and 1 that must be used to address
the PHY used for the DTSEC ports on frame manager 0. This can be any
value from 0 to 3.

\i <fman1Gpio>
Specifies the settings of GPIO pins 0 and 1 that must be used to address
the PHY used for the DTSEC ports on frame manager 1. This can be any
value from 0 to 3.

\i <muxEnable>
Specifies the BSP supplied routine that is used to enable the MDIO mux.
The routine takes two paramters: fmanNum as the FMAN number and dtsecNum
as the DTSEC number.

\ie

SEE ALSO: vxBus, miiBus
*/

#include <vxWorks.h>
#include <vxBusLib.h>
#include <semLib.h>
#include <stdlib.h>
#include <string.h>

#include <netLib.h>
#include <netBufLib.h>
#include <end.h>
#define END_MACROS
#include <endLib.h>

#include <hwif/vxbus/vxBus.h>
#include <hwif/util/vxbParamSys.h>

#include <hwif/vxbus/vxbPlbLib.h>
#include <hwif/vxbus/vxbPlbLib.h>
#include <hwif/vxbus/hwConf.h>
#include <../src/hwif/h/mii/miiBus.h>
#include <../src/hwif/h/vxbus/vxbAccess.h>
#include <../src/hwif/h/hEnd/hEnd.h>

#include <../src/hwif/h/resource/vxbQorIQBmanPortal.h>
#include <../src/hwif/h/resource/vxbQorIQQmanPortal.h>
#include "vxbDtsecEnd.h"
#include <../src/hwif/h/resource/vxbFslGpio.h>
#include "vxbDtsecMdio.h"
/* VxBus methods */

LOCAL void      dtmInstInit (VXB_DEVICE_ID);
LOCAL void      dtmInstInit2 (VXB_DEVICE_ID);
LOCAL void      dtmInstConnect (VXB_DEVICE_ID);
LOCAL STATUS    dtmInstUnlink (VXB_DEVICE_ID, void *);

/* miiBus methods */

LOCAL STATUS    dtmPhyRead (VXB_DEVICE_ID, VXB_DEVICE_ID, UINT8, UINT8, UINT16 *);
LOCAL STATUS    dtmPhyWrite (VXB_DEVICE_ID, VXB_DEVICE_ID, UINT8, UINT8, UINT16);

LOCAL struct drvBusFuncs dtmFuncs =
    {
    dtmInstInit,        /* devInstanceInit */
    dtmInstInit2,       /* devInstanceInit2 */
    dtmInstConnect      /* devConnect */
    };

LOCAL struct vxbDeviceMethod dtmMethods[] =
   {
   DEVMETHOD(miiRead,           dtmPhyRead),
   DEVMETHOD(miiWrite,          dtmPhyWrite),
   DEVMETHOD(vxbDrvUnlink,  	dtmInstUnlink),
   { 0, 0 }
   };   

LOCAL struct vxbPlbRegister dtmDevPlbRegistration =
    {
        {
        NULL,			/* pNext */
        VXB_DEVID_DEVICE,	/* devID */
        VXB_BUSID_PLB,		/* busID = PLB */
        VXB_VER_4_0_0,		/* vxbVersion */
        "dtsecMdio",		/* drvName */
        &dtmFuncs,		/* pDrvBusFuncs */
        dtmMethods,		/* pMethods */
        NULL,			/* devProbe */
        NULL,			/* pParamDefaults */
        },
    };

/*****************************************************************************
*
* dtmRegister - register with the VxBus subsystem
*
* This routine registers the vxbDtsecMdio driver with VxBus as a
* child of the PLB bus type.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void dtmRegister(void)
    {
    vxbDevRegister ((struct vxbDevRegInfo *)&dtmDevPlbRegistration);
    return;
    }

/*****************************************************************************
*
* dtmInstInit - VxBus instInit handler
*
* This function implements the VxBus instInit handler for an vxbDtsecMdio
* device instance. The only things done here are to select a unit
* number for the device and enable the hardware SMI interface.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void dtmInstInit
    (
    VXB_DEVICE_ID pDev
    )
    {
    struct hcfDevice *pHcf;

    /* Always use the unit number allocated to us in the hwconf file. */

    pHcf = (struct hcfDevice *)pDev->pBusSpecificDevInfo;
    vxbInstUnitSet (pDev, pHcf->devUnit);
	
    return;
    }

/*****************************************************************************
*
* dtmInstInit2 - VxBus instInit2 handler
*
* This function implements the VxBus instInit2 handler for a TM
* device instance. Once we reach this stage of initialization, it's
* safe for us to allocate memory, so we can create our pDrvCtrl
* structure. We also create a semaphore used to guard access to the
* MDIO registers.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void dtmInstInit2
    (
    VXB_DEVICE_ID pDev
    )
    {
    TM_DRV_CTRL *pDrvCtrl;
    struct hcfDevice *pHcf;
    int gpio;
    FUNCPTR muxEnable;
    STATUS r;

    pDrvCtrl = malloc (sizeof(TM_DRV_CTRL));
    if (pDrvCtrl == NULL)
        return;
    bzero ((char *)pDrvCtrl, sizeof(TM_DRV_CTRL));
    pDev->pDrvCtrl = pDrvCtrl;
    pDrvCtrl->dtmDev = pDev;

    pDrvCtrl->dtmMiiSem = semMCreate (SEM_Q_PRIORITY|
        SEM_DELETE_SAFE|SEM_INVERSION_SAFE);

    pHcf = (struct hcfDevice *)pDev->pBusSpecificDevInfo;
    /* 
     * resourceDesc {
     * The sharedMem resource specifies the location of the
     * shared memory anchor.  This must be set correctly for
     * AMP systems. }
     */
    devResourceGet (pHcf, "sharedMem", HCF_RES_INT,
        (void *)&pDrvCtrl->dtmShared);

    /* Set default values for P4080DS board. */

    pDrvCtrl->dtmFm0Gpio = 0x3;
    pDrvCtrl->dtmFm1Gpio = 0x0;

    /* 
     * resourceDesc {
     * The fman0Gpio resource specifies the settings for GPIO
     * pins 0 and 1 needed to address the PHY connected to the
     * DTSEC ports on frame manager 0. }
     */
    r = devResourceGet (pHcf, "fman0Gpio", HCF_RES_INT, (void *)&gpio);
    if (r == OK)
        pDrvCtrl->dtmFm0Gpio = gpio;

    /* 
     * resourceDesc {
     * The fman1Gpio resource specifies the settings for GPIO
     * pins 0 and 1 needed to address the PHY connected to the
     * DTSEC ports on frame manager 1. }
     */
    r = devResourceGet (pHcf, "fman1Gpio", HCF_RES_INT, (void *)&gpio);
    if (r == OK)
        pDrvCtrl->dtmFm1Gpio = gpio;

    /* 
     * resourceDesc {
     * The muxEnable resource specifies the BSP supplied routine
     * that is used to enable the MDIO mux. The routine takes two
     * paramters: fmanNum as the FMAN number and dtsecNum as the
     * DTSEC number. }
     */
    r = devResourceGet (pHcf, "muxEnable", HCF_RES_ADDR, (void *)&muxEnable);
    if (r == OK)
        pDrvCtrl->dtmMuxEnable = muxEnable;

    /* No shared memory anchor configured, use a dummy value. */

    if (pDrvCtrl->dtmShared == NULL)
        pDrvCtrl->dtmShared = &pDrvCtrl->dtmDummy;

    /* Make sure to set the MII management clock. */

    CSR_WRITE_4(pDev, DTM_MIIMCFG, DTM_MIIMCFG_MCS_28);

    /* Find the GPIO device instance. */

    pDrvCtrl->dtmGpioDev = vxbInstByNameFind ("fslGpio", 0);

    return;
    }

/*****************************************************************************
*
* dtmInstConnect -  VxBus instConnect handler
*
* This function implements the VxBus instConnect handler for an TM
* device instance.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void dtmInstConnect
    (
    VXB_DEVICE_ID pDev
    )
    { 
    return;
    }

/*****************************************************************************
*
* dtmInstUnlink -  VxBus unlink handler
*
* This function shuts down an TM device instance in response to an
* unlink event from VxBus. This may occur if our VxBus instance has
* been terminated, or if the TM driver has been unloaded. When an
* unlink event occurs, we release all the resources allocated
* during instance creation.
*
* RETURNS: OK if device was successfully destroyed, otherwise ERROR
*
* ERRNO: N/A
*/

LOCAL STATUS dtmInstUnlink
    (
    VXB_DEVICE_ID pDev,
    void * unused
    )
    { 
    TM_DRV_CTRL * pDrvCtrl;

    pDrvCtrl = pDev->pDrvCtrl;

    semDelete (pDrvCtrl->dtmMiiSem);

    free (pDrvCtrl);
    pDev->pDrvCtrl = NULL;

    /* Goodbye cruel world. */

    return (OK);
    }

/*****************************************************************************
*
* dtmPhyRead - miiBus miiRead method
*
* This function implements an miiRead() method that allows callers
* to access PHYs connected to the MDIO interface.
*
* RETURNS: ERROR if read times out, otherwise OK
*
* ERRNO: N/A
*/

LOCAL STATUS dtmPhyRead
    (
    VXB_DEVICE_ID pDev,
    VXB_DEVICE_ID pParent,
    UINT8 phyAddr,
    UINT8 regAddr,
    UINT16 * dataVal
    )
    {
    DTSEC_DRV_CTRL * pParentCtrl;
    TM_DRV_CTRL * pDrvCtrl;
    STATUS rval = ERROR;
    UINT32 tbipa;
    UINT8 pin0, pin1;
    UINT8 p;
    int i;

    pDrvCtrl = pDev->pDrvCtrl;
    /*logMsg("num 0x%x,name %s,flag 0x%x bus id 0x%x regAddr 0x%x phyAddr 0x%x \n",
		pDev->unitNumber,pDev->pName,pDev->flags,pDev->busID,regAddr,phyAddr);*/
    if(/*1 == pDev->unitNumber*/phyAddr != 0x23)
    {
    switch (regAddr)
    {
    
        case 0x0:
			*dataVal = 0x1000;
			break;
		case 0x1:
			*dataVal = 0x796d;
			break;
		case 0x2:
			*dataVal = 0x141;
			break;
		case 0x3:
			*dataVal = 0xcd4;
			break;
		case 0x4:
			*dataVal = 0xde1;
			break;
		case 0x5:
			*dataVal = 0x0081;
			break;
		case 0x6:
			*dataVal = 0x6;
			break;
        case 0x7:
			*dataVal = 0x2801;
			break;
        case 0x8:
			*dataVal = 0x4876;
			break;
		case 0x9:
			*dataVal = 0x0300;
			break;
		case 0xa:
			*dataVal = 0x7c00;
			break;
		case 0x10:
		    *dataVal = 0x78;
			break;
		case 0x11:
		    *dataVal = 0xac4c;/*1000M:0xac4c 100M: 0x4d00*/
			break;	
		case 0x14:
		    *dataVal = 0xce2;
			break;
		case 0x1b:
		    *dataVal = 0x848b;
			break; 
		default:
			*dataVal = 0xffff;
			break;
    	}
		rval = OK;

		return 0;
    }
   
    
    semTake (pDrvCtrl->dtmMiiSem, WAIT_FOREVER);

    for (i = 0; i < 10; i++)
        {
        if (vxTas ((BOOL *)pDrvCtrl->dtmShared))
            break;
        taskDelay (1);
        }

    /*
     * Make sure DTSEC0's TBIPA register is programmed correctly
     * so that it doesn't overlap one of the external PHYs.
     */

    tbipa = CSR_READ_4(pDev, DTSEC_TBIPA - 0x1000);
    if (tbipa == 0x00 || tbipa == 0x1F)
        CSR_WRITE_4(pDev, DTSEC_TBIPA - 0x1000, DTSEC_TBI_ADDR);

    p = phyAddr;
    if (p > 0x1F)
        p -= 32;

    pParentCtrl = pParent->pDrvCtrl;

    if (pParentCtrl->dtsecFmanUnit == 0)
        {
        pin0 = pDrvCtrl->dtmFm0Gpio & 0x1; 
        pin1 = pDrvCtrl->dtmFm0Gpio >> 1; 
        }
    else
        {
        pin0 = pDrvCtrl->dtmFm1Gpio & 0x1; 
        pin1 = pDrvCtrl->dtmFm1Gpio >> 1; 
        }

    if (pDrvCtrl->dtmGpioDev != NULL)
        {
        gpioOutputSelect (pDrvCtrl->dtmGpioDev, 0, 0, pin0);
        gpioOutputSelect (pDrvCtrl->dtmGpioDev, 1, 0, pin1);
        }

    if (pDrvCtrl->dtmMuxEnable != NULL)
        pDrvCtrl->dtmMuxEnable(pParentCtrl->dtsecFmanUnit,
                               pParentCtrl->dtsecNum);

    CSR_WRITE_4(pDev, DTM_MIIMCOM, 0);
    CSR_WRITE_4(pDev, DTM_MIIMADD, regAddr | (p << 8));
    CSR_WRITE_4(pDev, DTM_MIIMCOM, DTM_MIIMCOM_READ);

    /*
     * Perform an isync here to make sure the writes above occur
     * before the reads below. Normally each CSR_WRITE_4() performs
     * an eieio, but on some processors, if the MMU is disabled.
     * eieio is a no-op. There are cases where we run with the
     * MMU off, but the instruction cache on (such as in the
     * bootrom), so we need the additional isync instruction to
     * ensure synchronization.
     */

    WRS_ASM("isync");

    for (i = 0; i < DTM_TIMEOUT; i++)
        {
        if ((CSR_READ_4(pDev, DTM_MIIMIND) &
            (DTM_MIIMIND_BUSY|DTM_MIIMIND_NOT_VALID)) == 0)
            break;
        }

    if (i == DTM_TIMEOUT)
        *dataVal = 0xFFFF;
    else
        {
        *dataVal = CSR_READ_4(pDev, DTM_MIIMSTAT) & 0xFFFF;
        rval = OK;
        }

    CSR_WRITE_4(pDev, DTM_MIIMCOM, 0);

    *pDrvCtrl->dtmShared = 0;

    semGive (pDrvCtrl->dtmMiiSem);

    return (rval);
    }

/*****************************************************************************
*
* dtmPhyWrite - miiBus miiRead method
*
* This function implements an miiWrite() method that allows callers
* to access PHYs connected to the MDIO interface.
*
* RETURNS: ERROR if write times out, otherwise OK
*
* ERRNO: N/A
*/

LOCAL STATUS dtmPhyWrite
    (
    VXB_DEVICE_ID pDev,
    VXB_DEVICE_ID pParent,
    UINT8 phyAddr,
    UINT8 regAddr,
    UINT16 dataVal
    )
    {
    DTSEC_DRV_CTRL * pParentCtrl;
    TM_DRV_CTRL * pDrvCtrl;
    UINT32 tbipa;
    UINT8 pin0, pin1;
    UINT8 p;
    STATUS rval;
    int i;

    pDrvCtrl = pDev->pDrvCtrl;

    semTake (pDrvCtrl->dtmMiiSem, WAIT_FOREVER);

    for (i = 0; i < 10; i++)
        {
        if (vxTas ((BOOL *)pDrvCtrl->dtmShared))
            break;
        taskDelay (1);
        }

    /*
     * Make sure DTSEC0's TBIPA register is programmed correctly
     * so that it doesn't overlap one of the external PHYs.
     */

    tbipa = CSR_READ_4(pDev, DTSEC_TBIPA - 0x1000);
    if (tbipa == 0x00 || tbipa == 0x1F)
        CSR_WRITE_4(pDev, DTSEC_TBIPA - 0x1000, DTSEC_TBI_ADDR);

    p = phyAddr;
    if (p > 0x1F)
        p -= 32;

    pParentCtrl = pParent->pDrvCtrl;

    if (pParentCtrl->dtsecFmanUnit == 0)
        {
        pin0 = pDrvCtrl->dtmFm0Gpio & 0x1; 
        pin1 = pDrvCtrl->dtmFm0Gpio >> 1; 
        }
    else
        {
        pin0 = pDrvCtrl->dtmFm1Gpio & 0x1; 
        pin1 = pDrvCtrl->dtmFm1Gpio >> 1; 
        }

    if (pDrvCtrl->dtmGpioDev != NULL)
        {
        gpioOutputSelect (pDrvCtrl->dtmGpioDev, 0, 0, pin0);
        gpioOutputSelect (pDrvCtrl->dtmGpioDev, 1, 0, pin1);
        }

    if (pDrvCtrl->dtmMuxEnable != NULL)
        pDrvCtrl->dtmMuxEnable(pParentCtrl->dtsecFmanUnit,
                               pParentCtrl->dtsecNum);

    CSR_WRITE_4(pDev, DTM_MIIMADD, regAddr | (p << 8));
    CSR_WRITE_4(pDev, DTM_MIIMCON, dataVal);

    /*
     * Perform an isync here to make sure the writes above occur
     * before the reads below. Normally each CSR_WRITE_4() performs
     * an eieio, but on some processors, if the MMU is disabled.
     * eieio is a no-op. There are cases where we run with the
     * MMU off, but the instruction cache on (such as in the
     * bootrom), so we need the additional isync instruction to
     * ensure synchronization.
     */

    WRS_ASM("isync");

    for (i = 0; i < DTM_TIMEOUT; i++)
        {
        if (!(CSR_READ_4(pDev, DTM_MIIMIND) & DTM_MIIMIND_BUSY))
            break;
        }

    *pDrvCtrl->dtmShared = 0;

    semGive (pDrvCtrl->dtmMiiSem);

    if (i == DTM_TIMEOUT)
        rval = ERROR;
    else
        rval = OK;

    return (rval);
    }
