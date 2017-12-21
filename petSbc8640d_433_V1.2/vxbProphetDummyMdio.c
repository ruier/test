/* vxbProphetDummyMdio.c - driver for dummy MII MDIO ports */



/*
modification history
--------------------

01a,15mar2012,hdz  written
*/

/*
DESCRIPTION

This module implements a dummy MDIO driver in case the phy chip can not be 
accessed through MDIO. For instance, the MAC is connected with a L2 switch, we
can't get the status of switch's port(or it's not necessary to get it), then 
this driver can be used as a dummy MDIO vxBus device to provide mac the 
connection mode and link status.

EXTERNAL INTERFACE
The driver provides a vxBus external interface. The only exported
routine is the etsecRegister() function, which registers the driver
with VxBus. Since the dummy MDIO is a processor local bus device, each device
instance must be specified in the hwconf.c file in a BSP. The hwconf entry 
must specify the following parameters:
parameters:
 
\is

\i <regBase>
Specifies a fake base address.
 
\i <fullDuplex>
Specifies the duplex mode of this interface. The optional value are 0 & 1.

\i <speed>
Specifies the speed of this interface. The optional value are 10, 100 & 1000.

\ie

An example hwconf entry is shown below:

\cs
const struct hcfResource dummyMdio0Resources[] = {
    { "regBase",    HCF_RES_INT, { (void *)0 } },
    { "fullDuplex", HCF_RES_INT, { (void *)1 } },
    { "speed",      HCF_RES_INT, { (void *)1000 } }
};
#define dummyMdio0Num NELEMENTS(dummyMdio0Resources)
\ce

*/

#include <vxWorks.h>
#include <vxBusLib.h>
#include <semLib.h>
#include <stdlib.h>
#include <string.h>

#include <hwif/vxbus/vxBus.h>
#include <hwif/util/hwMemLib.h>

#include <hwif/vxbus/vxbPlbLib.h>
#include <hwif/vxbus/hwConf.h>
#include <../src/hwif/h/vxbus/vxbAccess.h>
#include <../src/hwif/h/mii/miiBus.h>

#include <vxbProphetDummyMdio.h>

/* VxBus methods */

LOCAL void   dummyMdioInstInit (VXB_DEVICE_ID);
LOCAL void   dummyMdioInstInit2 (VXB_DEVICE_ID);
LOCAL void   dummyMdioInstConnect (VXB_DEVICE_ID);
LOCAL STATUS dummyMdioInstUnlink (VXB_DEVICE_ID, void *);

/* miiBus methods */

LOCAL STATUS dummyMdioPhyRead (VXB_DEVICE_ID, UINT8, UINT8, UINT16 *);
LOCAL STATUS dummyMdioPhyWrite (VXB_DEVICE_ID, UINT8, UINT8, UINT16);
LOCAL STATUS dummyMdioLinkUpdate (VXB_DEVICE_ID);

LOCAL struct drvBusFuncs dummyMdioFuncs =
    {
    dummyMdioInstInit,        /* devInstanceInit */
    dummyMdioInstInit2,       /* devInstanceInit2 */
    dummyMdioInstConnect      /* devConnect */
    };

LOCAL struct vxbDeviceMethod dummyMdioMethods[] =
   {
   DEVMETHOD(miiRead,           dummyMdioPhyRead),
   DEVMETHOD(miiWrite,          dummyMdioPhyWrite),
   DEVMETHOD(miiMediaUpdate,    dummyMdioLinkUpdate),
   DEVMETHOD(vxbDrvUnlink,      dummyMdioInstUnlink),
   { 0, 0 }
   };   

LOCAL struct vxbPlbRegister dummyMdioDevPlbRegistration =
    {
    {
    NULL,                /* pNext */
    VXB_DEVID_DEVICE,    /* devID */
    VXB_BUSID_PLB,       /* busID = PLB */
    VXB_VER_4_0_0,       /* vxbVersion */
    "prophetDummyMdio",         /* drvName */
    &dummyMdioFuncs,     /* pDrvBusFuncs */
    dummyMdioMethods,    /* pMethods */
    NULL,                /* devProbe */
    NULL,                /* pParamDefaults */
    },
    };

/*******************************************************************************
*
* dummyMdioRegister - register with the VxBus subsystem
*
* This routine registers the dummy mdio driver with VxBus as a
* child of the PLB bus type.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void vxbProphetDummyMdioRegister(void)
    {
    vxbDevRegister((struct vxbDevRegInfo *)&dummyMdioDevPlbRegistration);
    return;
    }

/*******************************************************************************
*
* dummyMdioInstInit - VxBus instInit handler
*
* This function implements the VxBus instInit handler for an dummy mdio
* device instance. The only thing done here is to select a unit
* number for the device.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void dummyMdioInstInit
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

/*******************************************************************************
*
* dummyMdioInstInit2 - VxBus instInit2 handler
*
* This function implements the VxBus instInit2 handler for a dummy mdio
* device instance. Once we reach this stage of initialization, it's
* safe for us to allocate memory, so we can create our pDrvCtrl
* structure and do some initial setup.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void dummyMdioInstInit2
    (
    VXB_DEVICE_ID pDev
    )
    {    
    PROPHET_DUMMY_MDIO_DRV_CTRL *pDrvCtrl;
    struct hcfDevice *pHcf;

    pDrvCtrl = malloc (sizeof(PROPHET_DUMMY_MDIO_DRV_CTRL));
    bzero ((char *)pDrvCtrl, sizeof(PROPHET_DUMMY_MDIO_DRV_CTRL));
    pDev->pDrvCtrl = pDrvCtrl;
    pDrvCtrl->dummyDev = pDev;

    pHcf = (struct hcfDevice *)pDev->pBusSpecificDevInfo;

    /*
     * resourceDesc {
     * The fullDuplex resource specifies the duplex
     * mode of this interface.  The optional value
     * are 0 & 1. }
     */

    devResourceGet (pHcf, "fullDuplex", HCF_RES_INT,
                    (void *)&pDrvCtrl->fullDuplex);

    /*
     * resourceDesc {
     * The speed resource specifies the speed of
     * this interface.  The optional value are
     * 10, 100 & 1000. }
     */

    devResourceGet (pHcf, "speed", HCF_RES_INT,
                    (void *)&pDrvCtrl->speed);

    return;
    }

/*******************************************************************************
*
* dummyMdioInstConnect - VxBus instConnect handler
*
* Fake function.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void dummyMdioInstConnect
    (
    VXB_DEVICE_ID pDev
    )
    { 
    return;
    }

/*******************************************************************************
*
* dummyMdioInstUnlink -  VxBus unlink handler
*
* Fake function.
*
* RETURNS: OK
*
* ERRNO: N/A
*/

LOCAL STATUS dummyMdioInstUnlink
    (
    VXB_DEVICE_ID pDev,
    void * unused
    )
    { 
    return (OK);
    }

/*******************************************************************************
*
* dummyMdioPhyRead - miiBus miiRead method
*
* This function implements miiRead() method.
*
* RETURNS: OK
*
* ERRNO: N/A
*/

LOCAL STATUS dummyMdioPhyRead
(
    VXB_DEVICE_ID pDev,
    UINT8 phyAddr,
    UINT8 regAddr,
    UINT16 *dataVal
)
{
    PROPHET_DUMMY_MDIO_DRV_CTRL * pDrvCtrl;    

    pDrvCtrl = pDev->pDrvCtrl;  

    switch (regAddr)
	{
		case MII_CTRL_REG:
			 *dataVal = 0;
			 if(pDrvCtrl->fullDuplex == 1) 	*dataVal |= MII_CR_FDX;
			 if(pDrvCtrl->speed == 1000) 	*dataVal |= (MII_CR_100 | MII_CR_1000);
			 else if(pDrvCtrl->speed == 100) *dataVal |= MII_CR_100;
			 break;
		case MII_STAT_REG:
			 *dataVal = MII_SR_LINK_STATUS | MII_SR_AUTO_NEG;
			 break;
		case MII_PHY_ID1_REG:
			 *dataVal = 0;
			 break;
		case MII_PHY_ID2_REG:
			 *dataVal = 0;
			 break;
		case MII_MASSLA_STAT_REG:
			 *dataVal = MII_MASSLA_STAT_LOCAL_RCV | MII_MASSLA_STAT_REMOTE_RCV;
			 break;
		default:
			 *dataVal = 0xFFFF;
			 //printf("dummyMdioPhyRead:phyAddr=0x%x,regAddr=0x%x,dataVal=0x%x\n",phyAddr,regAddr,*dataVal);
			 return (ERROR);                
	}
    //printf("dummyMdioPhyRead:phyAddr=0x%x,regAddr=0x%x,dataVal=0x%x\n",phyAddr,regAddr,*dataVal);
    return (OK);
}

/*******************************************************************************
*
* dummyMdioPhyWrite - miiBus miiWrite method
*
* This function implements miiWrite() method. Fake function.
*
* RETURNS: OK
*
* ERRNO: N/A
*/

LOCAL STATUS dummyMdioPhyWrite
    (
    VXB_DEVICE_ID pDev,
    UINT8 phyAddr,
    UINT8 regAddr,
    UINT16 dataVal
    )
    {
    return (OK);
    }

/*******************************************************************************
*
* dummyMdioLinkUpdate - miiBus miiLinkUpdate method
*
* This function implements miiLinkUpdate() method. Fake function.
*
* RETURNS: OK
*
* ERRNO: N/A
*/

LOCAL STATUS dummyMdioLinkUpdate
    (
    VXB_DEVICE_ID pDev
    )
    {
    return (OK);
    }

