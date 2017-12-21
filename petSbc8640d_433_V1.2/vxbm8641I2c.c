/* wrsample.c - 3rd party VxBus sample driver */

/*
 * Copyright (C) 2007-2008 Wind River Systems, Inc. All rights are reserved.
 * 
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01e,13oct08,h_k  fixed Copyright format.
01d,02sep08,h_k  updated wrsampleResource description.
                 added meta-data.
01c,11aug08,d_l  rewritten, update to vx6.7(WIND00128510).
01b,04aug08,h_k  updated VxBus version.
01a,21aug07,tor  created
*/

/*
DESCRIPTION

This is a sample 3rd party VxBus driver provided by Wind River.
This driver provides a sample of the functionality required to
release a 3rd party VxBus driver into VxWorks 6.7.  

If you want to use this driver, please note that following code should be
added to the hwconf.c in wanted BSP directory.

    const struct hcfResource wrsampleResources[] = {
        { "regBase", HCF_RES_INT,  {(void *)0} },
    };
    #define wrsampleNum NELEMENTS(wrsampleResources)

And following code should be added to the structure hcfDeviceList in
the hwconf.c

        { "wrsample", 0, VXB_BUSID_PLB, 0, wrsampleNum, wrsampleResources },

You can also add the following resources to the wrsampleResources[], which
are optional.

        { "count",   HCF_RES_INT,  {(void *)0} },
        { "output",  HCF_RES_ADDR, {(void *)logMsg} },

This driver supports the {driverControl}() interface between
instances and applications.  There are three supported
commands: WRSAMPLE_LOG_TO_CONSOLE, WRSAMPLE_GET_SAMPLE_FUNCS,
and WRSAMPLE_SET_SAMPLE_FUNCS.

WRSAMPLE_LOG_TO_CONSOLE displays a message on the console for
each instance present on the system.

WRSAMPLE_GET_SAMPLE_FUNCS retrieves a function pointer to the
function, which is called when the WRSAMPLE_LOG_TO_CONSOLE
commnand is issued, the VxBus device ID of the last instance
of wrsample, and the output function used to display the
message, which is logMsg() by default.

WRSAMPLE_SET_SAMPLE_FUNCS changes the output function used to
display an output message.  The default is logMsg(), and if
NULL is specified as the output message, the default of logMsg()
is restored.
*/

/* includes */

#include <vxWorks.h>
#include <stdio.h>			/* for printf() */
#include <logLib.h>			/* for logMsg() */
#include <string.h>			/* for strcmp() */

#include <vxBusLib.h>			/* for VXB_DEVICE_ID */
#include <hwif/vxbus/vxBus.h>
#include <hwif/util/hwMemLib.h>		/* for hwMemAlloc() */
#include <hwif/vxbus/hwConf.h>		/* for devResourceGet() and
					 * hcfDeviceGet()
					 */
#include <driverControl.h>		/* for struct vxbDriverControl */

#include "vxbm8641I2c.h"
#include "sysM8641I2c.h"	/* Mpc85xx I2C Driver Header Module */


#define I2C_DBG_ON
#undef I2C_DBG_ON

#ifdef I2C_DBG_ON
#define I2C_DBG_MSG(fmt,a,b,c,d,e,f)  logMsg(fmt,a,b,c,d,e,f)
#undef LOCAL
#define LOCAL

#else /* I2C_DBG_ON */
#define I2C_DBG_MSG(fmt,a,b,c,d,e,f)
#endif /* I2C_DBG_ON */

/* typedefs */

extern int i2cIoctl(UINT8 ioctlflg,UINT32 address, UINT8 bdata1,UINT8 bdata2);

extern  int i2cWrite(UINT32 baseAddr,UINT8 deviceAddress,	 UINT32  startBlk,UINT8  numBlks,UINT8 * pBuf,UINT8 mode);
extern  int i2cRead(UINT32 baseAddr,UINT8 deviceAddress,	UINT32    startBlk,UINT8   numBlks,UINT8 * pBuf,UINT8 mode);

/* locals */

LOCAL void i2cInstInit(VXB_DEVICE_ID pInst);
LOCAL void i2cInstInit2(VXB_DEVICE_ID pInst);
LOCAL void i2cInstConnect(VXB_DEVICE_ID pInst);

LOCAL struct drvBusFuncs i2cFuncs =
    {
    i2cInstInit,        /* devInstanceInit */
    i2cInstInit2,       /* devInstanceInit2 */
    i2cInstConnect      /* devConnect */
    };

LOCAL struct vxbDevRegInfo i2cDevRegistration =
    {
    NULL,               /* pNext */
    VXB_DEVID_DEVICE,   /* devID */
    VXB_BUSID_PLB,      /* busID = PLB */
    VXB_VER_4_2_1,      /* vxbVersion */
    "i2c",         /* drvName */
    &i2cFuncs,     /* pDrvBusFuncs */
    NULL,               /* pMethods */
    NULL,               /* devProbe */
    NULL                /* pParamDefaults */
    };

LOCAL STATUS i2cDriverCtrl
    (
    VXB_DEVICE_ID               instId,
    struct vxbDriverControl *   pControl
    );

/*
 * This sample represents a custom driver class.  Normal VxBus
 * driver methods are not available, but we can use the generic
 * {driverControl}() method to accomplish what we need to.
 * Therefore, we publish only that method.
 *
 * Note that the structures used by the {driverControl}() method
 * must be defined in a per-driver or per-application header file,
 * in addition to the driverControl.h header file.  In this case,
 * the header file is target/3rdparty/windriver/sample/wrsample.h
 * and applications are responsible for ensuring that they include
 * that file when they need to interact with the instance.
 */

LOCAL device_method_t i2c_methods[] =
    {
    DEVMETHOD(driverControl, i2cDriverCtrl),
    { 0, 0}
    };

/* ------------------------------------------------------------ */

/* local defines       */

/* driver functions */

/*******************************************************************************
*
* wrsampleRegister - register wrsample driver
*
* This routine registers the wrsample driver and device recognition
* data with the vxBus subsystem.
*
* RETURNS: N/A
*
* ERRNO
*/

void m8641I2cDrvRegister(void)
    {
    I2C_DBG_MSG("run into m8641I2cDrvRegister\n" ,1,2,3,4,5,6);	
    vxbDevRegister((struct vxbDevRegInfo *)&i2cDevRegistration);
    }

/*******************************************************************************
*
* wrsampleInstInit - initialize wrsample device
*
* This is the wrsample initialization routine.
*
* RETURNS: N/A
*
* ERRNO
*/



LOCAL void i2cInstInit
    (
    VXB_DEVICE_ID pInst
    )
{
    struct i2cDrvCtrl * pDrvCtrl;
    /* to store the HCF device */
    HCF_DEVICE *        pHcf = NULL; 

    I2C_DBG_MSG("run into i2cInstInit\n" ,1,2,3,4,5,6);
    

    if (pInst == NULL)    return;
	
    pDrvCtrl =(struct i2cDrvCtrl *)hwMemAlloc(sizeof (struct i2cDrvCtrl));
    if (pDrvCtrl == NULL)  return;  
	
    /* get the HCF device from vxBus device structure */
    pHcf = hcfDeviceGet (pInst);
    if (pHcf == NULL)     return;

    if (devResourceGet(pHcf,"count", HCF_RES_INT,(void *)&pDrvCtrl->customMethodCount) != OK)
        pDrvCtrl->customMethodCount = 0;

    if (devResourceGet(pHcf,"i2cSlaveAddr", HCF_RES_INT,(void *)&pDrvCtrl->i2cSlaveAddr) != OK)
        pDrvCtrl->i2cSlaveAddr = 0x1;
        
    if (devResourceGet(pHcf,"clkFreq", HCF_RES_INT,(void *)&pDrvCtrl->i2cFdr) != OK)
        pDrvCtrl->i2cFdr = 0x7;

	pInst->pDrvCtrl = pDrvCtrl;


    /* publish methods */
    pInst->pMethods = &i2c_methods[0];

    
 }


LOCAL void i2c_interrupt_handler(UINT32 baseAddr)
{
    UINT8 statusReg;
    statusReg = i2cIoctl(I2C_IOCTL_RD,(UINT32)(baseAddr+MPC85XX_I2C_STATUS_REG), 0, 0);
	printf("abcdefghijklmnopqrst1!!!!!!\n");
	if(( statusReg & MPC85XX_I2C_STATUS_REG_MIF)==0) return;
    i2cIoctl(I2C_IOCTL_RMW_AND,
    		(UINT32)(baseAddr+MPC85XX_I2C_STATUS_REG),
            ((UINT8)~MPC85XX_I2C_STATUS_REG_MIF), 0);
    printf("abcdefghijklmnopqrst4444!!!!!!\n");
	
}

/*******************************************************************************
*
* wrsampleInstInit2 - initialize wrsample device
*
* This is seconde phase initialize routine for VxBus driver. 
*
* RETURNS: N/A
*
* ERRNO
*/

LOCAL void i2cInstInit2
    (
    VXB_DEVICE_ID pInst
    )
    {
    UINT32 baseAddr;
    struct i2cDrvCtrl *pI2cDrvCtrl;

    baseAddr=(UINT32 )pInst->pRegBase[0];
    pI2cDrvCtrl=pInst->pDrvCtrl;

	I2C_DBG_MSG("run into i2cInstInit2\n" ,1,2,3,4,5,6);
    	    /* disable the I2C module, set the device to Master Mode  */
   	i2cIoctl(I2C_IOCTL_RMW_AND_OR,
	     (UINT32)(baseAddr + MPC85XX_I2C_CONTROL_REG),
	     ((UINT8)~MPC85XX_I2C_CONTROL_REG_MEN),
	     MPC85XX_I2C_CONTROL_REG_MSTA);
	/* initialize and enable the I2C interface */
	
	/*set I2CFDR*/
	i2cIoctl(I2C_IOCTL_RMW_AND_OR,
	     (UINT32)(baseAddr + MPC85XX_I2C_FREQ_DIV_REG),
	     ((UINT8)~MPC85XX_I2C_FREQ_DIV_REG_MASK),
	    (UINT8) pI2cDrvCtrl->i2cFdr);
	
	 /* set the slave address */
   	 i2cIoctl(I2C_IOCTL_RMW_AND_OR,
	     (UINT32)(baseAddr +MPC85XX_I2C_ADR_REG),
	     ((UINT8)~MPC85XX_I2C_ADDRESS_REG_MASK),  (UINT8)(pI2cDrvCtrl->i2cSlaveAddr)<<1);


		
    	/* enable the interface */
   	 i2cIoctl(I2C_IOCTL_RMW_OR,
	     (UINT32)(baseAddr+MPC85XX_I2C_CONTROL_REG),
	     MPC85XX_I2C_CONTROL_REG_MEN  , 0);/*|MPC85XX_I2C_CONTROL_REG_MIEN*/

     /*set the device to slave mode.  This is required for clearing a BUS BUSY lockup condition.*/

	i2cIoctl(I2C_IOCTL_RMW_AND,
	     (UINT32)(baseAddr +MPC85XX_I2C_CONTROL_REG),
	     ((UINT8)~MPC85XX_I2C_CONTROL_REG_MSTA), 0);
			
	I2C_DBG_MSG("vxbIntConnect\n" ,1,2,3,4,5,6);
	vxbIntConnect (pInst, 0, (VOIDFUNCPTR)i2c_interrupt_handler,(void *) baseAddr);
	vxbIntEnable (pInst, 0,(VOIDFUNCPTR) i2c_interrupt_handler,(void *)  baseAddr);
    }



/*******************************************************************************
*
* wrsampleInstConnect - VxBus connect phase routine for wrsample driver
*
* This is connect phase routine.
*
* RETURNS: N/A
*
* ERRNO: not set
*/

LOCAL void i2cInstConnect
    (
    VXB_DEVICE_ID pInst
    )
    {
    I2C_DBG_MSG("i2cInstConnect() called\n", 1,2,3,4,5,6);
    }


/*******************************************************************************
*
* i2cDriverCtrl - handle {driverControl}() method calls
*
* This routine handles driverControl method call from application layer.
*
* RETURNS: ERROR or OK
*
* ERRNO: not set
*/

LOCAL STATUS i2cDriverCtrl
    (
    VXB_DEVICE_ID               instId,
    struct vxbDriverControl *   pControl
    )
    {

   	struct i2cDrvCtrl * pDrvCtrl;

   	
    I2C_DBG_MSG("run into i2cDriverCtrl\n" ,1,2,3,4,5,6);
    /* check whether this is for us */
    if ( instId == NULL || pControl == NULL )
        return(ERROR);
    if ( strcmp(pControl->driverName, "i2c" ) != 0 )
        return(ERROR);
    I2C_DBG_MSG("run into i2cDriverCtrl11\n" ,1,2,3,4,5,6);
    /* get pDrvCtrl from VxBus instance */
    pDrvCtrl =(struct i2cDrvCtrl *)instId->pDrvCtrl;
    pDrvCtrl = pControl->drvCtrlInfo;

	/*pDrvCtrl->pInst = instId;*/

    /* switch on command to run */

    switch (pControl->drvCtrlCmd)
        {
        case I2C_GET_WRITE_FUNCS:
             /* retrieve functions */
            pDrvCtrl->i2cWrite = i2cWrite;
            pDrvCtrl->baseAddr=(UINT32)instId->pRegBase[0];
            break;

        case I2C_GET_READ_FUNCS:
            /* retrieve functions */		
            pDrvCtrl->i2cRead = i2cRead;
            pDrvCtrl->baseAddr=(UINT32)instId->pRegBase[0];
            break;

        default:
            return(ERROR);
        }

    return(OK);
    }


