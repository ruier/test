/* vxbProphetM8640dMdio.c - driver for TSEC MDIO port */



/*
modification history
--------------------

01a,15mar2012,hdz  written
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

#include <hwif/vxbus/vxbPlbLib.h>
#include <hwif/vxbus/vxbPlbLib.h>
#include <hwif/vxbus/hwConf.h>
#include <../src/hwif/h/mii/miiBus.h>
#include <../src/hwif/h/vxbus/vxbAccess.h>
#include <../src/hwif/h/hEnd/hEnd.h>

#include <vxbProphetM8640dMdio.h>
#include <../src/hwif/h/end/vxbEtsecEnd.h>

/* VxBus methods */

LOCAL void      m8640dMdioInstInit (VXB_DEVICE_ID);
LOCAL void      m8640dMdioInstInit2 (VXB_DEVICE_ID);
LOCAL void      m8640dMdioInstConnect (VXB_DEVICE_ID);
LOCAL STATUS    m8640dMdioInstUnlink (VXB_DEVICE_ID, void *);

/* miiBus methods */

LOCAL STATUS    m8640dMdioPhyRead (VXB_DEVICE_ID, UINT8, UINT8, UINT16 *);
LOCAL STATUS    m8640dMdioPhyWrite (VXB_DEVICE_ID, UINT8, UINT8, UINT16);

LOCAL struct drvBusFuncs m8640dMdioFuncs =
    {
    m8640dMdioInstInit,        /* devInstanceInit */
    m8640dMdioInstInit2,       /* devInstanceInit2 */
    m8640dMdioInstConnect      /* devConnect */
    };

LOCAL struct vxbDeviceMethod m8640dMdioMethods[] =
   {
   DEVMETHOD(miiRead,           m8640dMdioPhyRead),
   DEVMETHOD(miiWrite,          m8640dMdioPhyWrite),
   DEVMETHOD(vxbDrvUnlink,  	m8640dMdioInstUnlink),
   { 0, 0 }
   };   

LOCAL struct vxbPlbRegister m8640dMdioDevPlbRegistration =
    {
        {
        NULL,			/* pNext */
        VXB_DEVID_DEVICE,	/* devID */
        VXB_BUSID_PLB,		/* busID = PLB */
        VXB_VER_4_0_0,	/* vxbVersion */
        "prophetM8640dMdio",		/* drvName */
        &m8640dMdioFuncs,		/* pDrvBusFuncs */
        m8640dMdioMethods,		/* pMethods */
        NULL,			/* devProbe */
        NULL,			/* pParamDefaults */
        },
    };

/*****************************************************************************
*
* m8640dMdioRegister - register with the VxBus subsystem
*
* This routine registers the tsecMdio driver with VxBus as a
* child of the PLB bus type.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void vxbProphetM8640dMdioRegister(void)
    {
    vxbDevRegister ((struct vxbDevRegInfo *)&m8640dMdioDevPlbRegistration);
    return;
    }

/*****************************************************************************
*
* m8640dMdioInstInit - VxBus instInit handler
*
* This function implements the VxBus instInit handler for an tsecMdio
* device instance. The only things done here are to select a unit
* number for the device and enable the hardware SMI interface.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void m8640dMdioInstInit
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
* m8640dMdioInstInit2 - VxBus instInit2 handler
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

LOCAL void m8640dMdioInstInit2
    (
    VXB_DEVICE_ID pDev
    )
    {
	M8640D_MDIO_DRV_CTRL *pDrvCtrl;
    struct hcfDevice *pHcf;

    pDrvCtrl = malloc (sizeof(M8640D_MDIO_DRV_CTRL));
    bzero ((char *)pDrvCtrl, sizeof(M8640D_MDIO_DRV_CTRL));
    pDev->pDrvCtrl = pDrvCtrl;
    pDrvCtrl->m8640dMdioDev = pDev;

    pDrvCtrl->m8640dMdioMiiSem = semMCreate (SEM_Q_PRIORITY|
            SEM_DELETE_SAFE|SEM_INVERSION_SAFE);

    pHcf = (struct hcfDevice *)pDev->pBusSpecificDevInfo;

    /* Make sure to set the MII management clock. */
  //  printf("pDev->pRegBase[0]=0x%x\n",(UINT32)pDev->pRegBase[0]);
    M8640_MDIO_CSR_WRITE_4(pDev, ETSEC_MIIMCFG, ETSEC_MIIMCFG_MCS_28);

    return;
    }

/*****************************************************************************
*
* m8640dMdioInstConnect -  VxBus instConnect handler
*
* This function implements the VxBus instConnect handler for an TM
* device instance.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void m8640dMdioInstConnect
    (
    VXB_DEVICE_ID pDev
    )
    { 
    return;
    }

/*****************************************************************************
*
* m8640dMdioInstUnlink -  VxBus unlink handler
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

LOCAL STATUS m8640dMdioInstUnlink
    (
    VXB_DEVICE_ID pDev,
    void * unused
    )
    { 
    return (OK);
    }

/*****************************************************************************
*
* m8640dMdioPhyRead - miiBus miiRead method
*
* This function implements an miiRead() method that allows callers
* to access PHYs connected to the MDIO interface.
*
* RETURNS: ERROR if read times out, otherwise OK
*
* ERRNO: N/A
*/

LOCAL STATUS m8640dMdioPhyRead
    (
    VXB_DEVICE_ID pDev,
    UINT8 phyAddr,
    UINT8 regAddr,
    UINT16 *dataVal
    )
    {
	M8640D_MDIO_DRV_CTRL * pDrvCtrl;
    STATUS rval = ERROR;
    UINT8 p;
    int i;

    pDrvCtrl = pDev->pDrvCtrl;

   semTake (pDrvCtrl->m8640dMdioMiiSem, WAIT_FOREVER);
    

    p = phyAddr;
    if (p > 0x1F) p -= 32;
    //printf("p=0x%x\n",p);
    M8640_MDIO_CSR_WRITE_4(pDev, ETSEC_MIIMADD, regAddr | (p << 8));
    M8640_MDIO_CSR_WRITE_4(pDev, ETSEC_MIIMCOM, ETSEC_MIIMCOM_READ);

    /*
     * Perform an isync here to make sure the writes above occur
     * before the reads below. Normally each M8640_MDIO_CSR_WRITE_4() performs
     * an eieio, but on some processors, if the MMU is disabled.
     * eieio is a no-op. There are cases where we run with the
     * MMU off, but the instruction cache on (such as in the
     * bootrom), so we need the additional isync instruction to
     * ensure synchronization.
     */

    WRS_ASM("isync");

    for (i = 0; i < M8640DMDIO_TIMEOUT; i++)
        {
        if ((M8640_MDIO_CSR_READ_4(pDev, ETSEC_MIIMIND) &
            (ETSEC_MIIMIND_BUSY|ETSEC_MIIMIND_NOT_VALID)) == 0)
            break;
        }

    if (i == M8640DMDIO_TIMEOUT)
        *dataVal = 0xFFFF;
    else
        {
    	// printf("not M8640DMDIO_TIMEOUT\n");
        *dataVal = M8640_MDIO_CSR_READ_4(pDev, ETSEC_MIIMSTAT) & 0xFFFF;
        rval = OK;
        }

    M8640_MDIO_CSR_WRITE_4(pDev, ETSEC_MIIMCOM, 0);

    
	//printf("m8640dMdioPhyRead phyAddr=0x%x,regAddr=0x%x,dataValue=0x%x,pDev->pRegBase[0]=0x%x\n",phyAddr,regAddr,*dataVal,(UINT32)pDev->pRegBase[0]);
    semGive (pDrvCtrl->m8640dMdioMiiSem);

    return (rval);
    }

/*****************************************************************************
*
* m8640dMdioPhyWrite - miiBus miiRead method
*
* This function implements an miiWrite() method that allows callers
* to access PHYs connected to the MDIO interface.
*
* RETURNS: ERROR if write times out, otherwise OK
*
* ERRNO: N/A
*/

LOCAL STATUS m8640dMdioPhyWrite
    (
    VXB_DEVICE_ID pDev,
    UINT8 phyAddr,
    UINT8 regAddr,
    UINT16 dataVal
    )
    {
	M8640D_MDIO_DRV_CTRL * pDrvCtrl;
    UINT8 p;
    STATUS rval;
    int i;

    pDrvCtrl = pDev->pDrvCtrl;

    semTake (pDrvCtrl->m8640dMdioMiiSem, WAIT_FOREVER);


    p = phyAddr;
    if (p > 0x1F)  p -= 32;

    M8640_MDIO_CSR_WRITE_4(pDev, ETSEC_MIIMADD, regAddr | (p << 8));
    M8640_MDIO_CSR_WRITE_4(pDev, ETSEC_MIIMCON, dataVal);

    /*
     * Perform an isync here to make sure the writes above occur
     * before the reads below. Normally each M8640_MDIO_CSR_WRITE_4() performs
     * an eieio, but on some processors, if the MMU is disabled.
     * eieio is a no-op. There are cases where we run with the
     * MMU off, but the instruction cache on (such as in the
     * bootrom), so we need the additional isync instruction to
     * ensure synchronization.
     */

    WRS_ASM("isync");

    for (i = 0; i < M8640DMDIO_TIMEOUT; i++)
        {
        if (!(M8640_MDIO_CSR_READ_4(pDev, ETSEC_MIIMIND) & ETSEC_MIIMIND_BUSY))
            break;
        }


    semGive (pDrvCtrl->m8640dMdioMiiSem);

    if (i == M8640DMDIO_TIMEOUT)
        rval = ERROR;
    else
        rval = OK;

    return (rval);
    }
