
/* vxbGpioDrv.c - gpio device  VxBus driver */


/* includes */
#include <vxWorks.h>

#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/hwConf.h>
#include <../src/hwif/h/vxbus/vxbAccess.h>
#include <hwif/util/hwMemLib.h>
#include <../src/hwif/intCtlr/vxbIntCtlrLib.h>
#include "vxbGpioDrv.h"
#include "logLib.h"
/* debug */


#define GPIODRV_DEBUG_MSG(level,fmt,a,b,c,d,e,f) 

/* typedefs */
LOCAL void gpioDrvInstInit(VXB_DEVICE_ID pInst);
LOCAL void gpioDrvInstInit2(VXB_DEVICE_ID pInst);
LOCAL void gpioDrvInstConnect(VXB_DEVICE_ID pInst);

LOCAL void vxbGpioModeSet(
    		VXB_DEVICE_ID pIntCtlr,
    		int gpioIndex , /*  gpioIndex	: 0~15*/
    		int mode	/* mode : 0----input;
								  1----output*/
    		) ;
LOCAL BOOL vxbGpioModeGet(
    		VXB_DEVICE_ID pIntCtlr,
    		int gpioIndex  /*  gpioIndex	: 0~15*/
    		) ;
LOCAL STATUS vxbGpioIntModeSet(
    		VXB_DEVICE_ID pIntCtlr,
    		int gpioIndex , /*  gpioIndex	: 0~15*/
    		int intMode	/* intMode : 0----上升沿;1----下降沿*/
    		) ;
LOCAL STATUS vxbGpioValueSet(
		VXB_DEVICE_ID pIntCtlr,
		int gpioIndex ,		 /*  gpioIndex	: 0~15*/
		BOOL	value		 /*	 value: 0  or 1 */
		);
LOCAL BOOL vxbGpioValueGet(
		VXB_DEVICE_ID pIntCtlr,
		int gpioIndex 		 /*  gpioIndex	: 0~15*/
		);
LOCAL STATUS vxbGpioCleanInt(
		VXB_DEVICE_ID pIntCtlr,
		int gpioIndex 	 /*  gpioIndex	: 0~15*/
		);
/*
 * The driver publishes driver methods.  The following driver
 * methods are for serial devices.  Change these as appropriate
 * for your device.
 */
DEVMETHOD_DEF(gpioModeSet, "gpio mode set:0-input,1-output");
DEVMETHOD_DEF(gpioModeGet, "return gpio mode:0-input,1-output");
DEVMETHOD_DEF(gpioIntModeSet, "gpio intMode set:0-positive edge,1-negative edge");
DEVMETHOD_DEF(gpioValueSet, "set outPut pin value");
DEVMETHOD_DEF(gpioValueGet, "get gpio pin value");
//DEVMETHOD_DEF(gpioIntClean, "clean gpio int");

LOCAL device_method_t gpioDrv_methods[] =
{
 //   DEVMETHOD(driverControl, gpioDriverCtrl),
    DEVMETHOD(gpioModeSet, vxbGpioModeSet),
    DEVMETHOD(gpioModeGet, vxbGpioModeGet),
    DEVMETHOD(gpioIntModeSet, vxbGpioIntModeSet),  
    DEVMETHOD(gpioValueSet, vxbGpioValueSet), 
    DEVMETHOD(gpioValueGet, vxbGpioValueGet), 
   // DEVMETHOD(gpioIntClean, vxbGpioCleanInt), 
    { 0, 0}
};

LOCAL struct drvBusFuncs gpioDrvFuncs =
    {
    	gpioDrvInstInit,        /* devInstanceInit */
    	gpioDrvInstInit2,       /* devInstanceInit2 */
    	gpioDrvInstConnect      /* devConnect */
    };

LOCAL struct vxbDevRegInfo gpioDrvDevRegistration =
    {
    (struct vxbDevRegInfo *)NULL, /* pNext */
    VXB_DEVID_DEVICE,     /* devID */
    VXB_BUSID_PLB,        /* busID = PLB */
    VXB_VER_4_0_0,        /* busVer */
    "gpioDrv",           /* drvName */
    &gpioDrvFuncs,       /* pDrvBusFuncs */
    &gpioDrv_methods[0], /* pMethods */
    NULL                  /* devProbe */
    };

typedef struct gpioDrvCtrl
    {
    VXB_DEVICE_ID	pInst;
    void *	regBase;
    void *	handle;
    struct hcfDevice * pHcf;
    spinlockIsr_t spinlockIsr;	/* ISR-callable spinlock */
    } GPIODRV_DRV_CTRL;

/* ------------------------------------------------------------ */

/* local defines       */



/* static forward declarations */

/******************************************************************************
*
* gpioDrvRegister - register gpio driver
*
*  We use a two-function method here, to delay registration of the driver
*  during debug.  This should be left in place when the driver is released.
*
*/

void vxbGpioDrvRegister(void)
    {
    vxbDevRegister((struct vxbDevRegInfo *)&gpioDrvDevRegistration);
    }


/******************************************************************************
*
* gpioDrvInstInit - initialize gpio device
*
* This is the gpio initialization routine.
*
* NOTE:
*
* This routine is called early during system initialization, and
* *MUST NOT* make calls to OS facilities such as memory allocation
* and I/O.
*
* RETURNS: N/A
*
* ERRNO
*/

LOCAL void gpioDrvInstInit
    (
    VXB_DEVICE_ID pInst
    )
    {
    GPIODRV_DRV_CTRL * pDrvCtrl;
    struct hcfDevice *  pHcf;

    /* allocate pDrvCtrl */

    pDrvCtrl = (GPIODRV_DRV_CTRL *)hwMemAlloc(sizeof(*pDrvCtrl));

    if (pDrvCtrl == NULL)
        return;

    vxbRegMap (pInst, 0, &pDrvCtrl->handle);
    pDrvCtrl->regBase = pInst->pRegBase[0];

    pDrvCtrl->pInst = pInst;

    /* extract BSP information */

    pHcf = (struct hcfDevice *)hcfDeviceGet(pInst);
    pDrvCtrl->pHcf = pHcf;

    pInst->pDrvCtrl = pDrvCtrl;

    pInst->pMethods = &gpioDrv_methods[0];

    }

/******************************************************************************
*
* gpioDrvInstInit2 - initialize gpio device
*
*/

LOCAL void gpioDrvInstInit2
    (
    VXB_DEVICE_ID pInst
    )
    {
	
	GPIODRV_DRV_CTRL * pDrvCtrl = pInst->pDrvCtrl;
   	
}



/******************************************************************************
*
* gpioDrvInstConnect - connect gpio device to I/O system
*
* Nothing to do here. 
* 
* RETURNS: N/A
*
* ERRNO
*/

LOCAL void gpioDrvInstConnect
    (
    VXB_DEVICE_ID pInst
    )
    {
	GPIODRV_DEBUG_MSG(1,"gpioDrvInstConnect(): called\n",1,2,3,4,5,6);
    }


/*******************************************************************************
*
* vxbGpioModeSet - set GPIO mode control
*
*/
LOCAL void vxbGpioModeSet(
		VXB_DEVICE_ID pIntCtlr,
		int gpioIndex , /*  gpioIndex	: 0~15*/
		int mode	/* mode : 0----input;1----output*/
		)  
{
	 struct gpioDrvCtrl * pDrvCtrl = pIntCtlr->pDrvCtrl;
	 char  *regBase = pIntCtlr->pRegBase[0];
	// UINT32 data;
	 //printf("vxbGpioModeSet:gpio%d 0x%x = %d\n",gpioIndex,((UINT8*)regBase + GPIO_CTRL_REG+((gpioIndex/4)<<2)+(3-(gpioIndex & 0x03))),mode&0x01);
	 vxbWrite8((void*)pDrvCtrl->handle,(UINT8*)((UINT8*)regBase + GPIO_CTRL_REG+((gpioIndex/4)<<2)+(3-(gpioIndex & 0x03))),mode&0x01);

}


/*******************************************************************************
*
* vxbGpioModeGet - set GPIO mode control
*
*return value : 0 —— Input ；1 —— OutPut
*/
 LOCAL BOOL vxbGpioModeGet(
		VXB_DEVICE_ID pIntCtlr,
		int gpioIndex  /*  gpioIndex	: 0~15*/
		)  
{
	 struct gpioDrvCtrl * pDrvCtrl = pIntCtlr->pDrvCtrl;
	 char  *regBase = pIntCtlr->pRegBase[0];
	 UINT32 data;
	 BOOL gpioMode;
	 data = vxbRead8((void*)pDrvCtrl->handle,(UINT8*)((char*)regBase + GPIO_CTRL_REG+((gpioIndex/4)<<2)+(3-(gpioIndex & 0x03)))) ;
	 gpioMode = data & 0x01;
	 return gpioMode;
}
/*******************************************************************************
*
* gpioIntSet - set GPIO int  control
*  0----上升沿有效;1----下降沿有效
*
*/
LOCAL STATUS vxbGpioIntModeSet(
		VXB_DEVICE_ID pIntCtlr,
		int gpioIndex , /*  gpioIndex	: 0~15*/
		int intMode		/* intMode : 0----上升沿;1----下降沿*/
		)  
{
	 struct gpioDrvCtrl * pDrvCtrl = pIntCtlr->pDrvCtrl;
	 char  *regBase = pIntCtlr->pRegBase[0];
	// UINT32 data;
	 
	 if( vxbGpioModeGet(pIntCtlr,gpioIndex) != 1)   /*  0——output		1——input */
	 {
		 printf("set int mode failed:gpio%d is as outPut pin\n",gpioIndex);
		 return ERROR;
	 }
	 
	 vxbWrite8((void*)pDrvCtrl->handle,(UINT8*)((UINT8*)regBase + GPIO_INT_CTRL+((gpioIndex/4)<<2)+(3-(gpioIndex & 0x03))),intMode&0x01);	
	 return OK;
}


LOCAL STATUS vxbGpioValueSet(
		VXB_DEVICE_ID pIntCtlr,
		int gpioIndex ,		 /*  gpioIndex	: 0~15*/
		BOOL	value		 /*	 value: 0  or 1 */
		)
{
	struct gpioDrvCtrl * pDrvCtrl = pIntCtlr->pDrvCtrl;
	char  *regBase = pIntCtlr->pRegBase[0];
	BOOL gpioMode;	/*	0 —— OutPut ；1 ——InPut */
	//UINT32 data ;
	gpioMode = vxbGpioModeGet(pIntCtlr, gpioIndex);
	if( gpioMode == 1)   return ERROR;
	else		vxbWrite8((void*)pDrvCtrl->handle,(UINT8*)((UINT8*)regBase + GPIO_STATUS+((gpioIndex/4)<<2)+(3-(gpioIndex & 0x03))),value&0x01);
	return OK;	
}

LOCAL BOOL vxbGpioValueGet(
		VXB_DEVICE_ID pIntCtlr,
		int gpioIndex 	 /*  gpioIndex	: 0~15*/
		)
{
	struct gpioDrvCtrl * pDrvCtrl = pIntCtlr->pDrvCtrl;
	char  *regBase = pIntCtlr->pRegBase[0];
	UINT32 data;	
	BOOL   value;
	
	 data = vxbRead8((void*)pDrvCtrl->handle,(UINT8*)((UINT8*)regBase + GPIO_STATUS+((gpioIndex/4)<<2)+(3-(gpioIndex & 0x03))));
	 //printf("addr=0x%x\n",(UINT8*)regBase + GPIO_STATUS+((gpioIndex/4)<<2)+(3-(gpioIndex & 0x03)));
	 value = data & 0x01;
	 return value;
}
