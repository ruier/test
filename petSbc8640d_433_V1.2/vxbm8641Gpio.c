/* includes */

#include <vxWorks.h>
#include <stdio.h>			/* for printf() */
#include <logLib.h>			/* for logMsg() */
#include <string.h>			/* for strcmp() */

#include <vxBusLib.h>			/* for VXB_DEVICE_ID */
#include <hwif/vxbus/vxBus.h>
#include <hwif/util/hwMemLib.h>		/* for hwMemAlloc() */
#include <hwif/vxbus/hwConf.h>		/* for devResourceGet() and hcfDeviceGet() */
#include <driverControl.h>		/* for struct vxbDriverControl */
#include "vxbm8641Gpio.h"



#define GPIO_DBG_ON
#undef GPIO_DBG_ON
#ifdef GPIO_DBG_ON
#define GPIO_DBG_MSG(fmt,a,b,c,d,e,f)  DPRINTF(fmt,a,b,c,d,e,f)
#undef LOCAL
#define LOCAL
#else 
#define GPIO_DBG_MSG(fmt,a,b,c,d,e,f)
#endif 

#define GPIO_IO_NUM 32

/* locals */

LOCAL void gpioInstInit(VXB_DEVICE_ID pInst);
LOCAL void gpioInstInit2(VXB_DEVICE_ID pInst);
LOCAL void gpioInstConnect(VXB_DEVICE_ID pInst);
LOCAL void gpioIntHandle(VXB_DEVICE_ID pInst);
LOCAL STATUS gpioDriverCtrl(VXB_DEVICE_ID  pInst,  struct vxbDriverControl *pControl);
LOCAL void pulseIntHandle(VXB_DEVICE_ID pInst);

LOCAL struct drvBusFuncs gpioFuncs =
    {
    	gpioInstInit,        /* devInstanceInit */
    	gpioInstInit2,       /* devInstanceInit2 */
    	gpioInstConnect      /* devConnect */
    };

LOCAL device_method_t gpio_methods[] =
    {
    DEVMETHOD(driverControl, gpioDriverCtrl),
    { 0, 0}
    };

LOCAL struct vxbDevRegInfo gpioDevRegistration =
    {
    NULL,               /* pNext */
    VXB_DEVID_DEVICE,   /* devID */
    VXB_BUSID_PLB,      /* busID = PLB */
    VXB_VER_4_0_0,    /* pMethods */
    "gpioDrv",
    &gpioFuncs, 
    &gpio_methods[0],               /* devProbe */
    NULL                /* pParamDefaults */
    };





UINT32 gpioIntCount[32];

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

void vxbGpioDrvRegister(void)
    {
    if(sysGetCpuID() == 0)
    {
    GPIO_DBG_MSG("run into m8641GpioDrvRegister\n" ,1,2,3,4,5,6);	
    vxbDevRegister((struct vxbDevRegInfo *)&gpioDevRegistration);
    }
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



LOCAL void gpioInstInit
    (
    VXB_DEVICE_ID pInst
    )
{
    GPIO_DRV_CTRL * pDrvCtrl;
    /* to store the HCF device */
    HCF_DEVICE *        pHcf = NULL; 

    GPIO_DBG_MSG("run into gpioInstInit\n" ,1,2,3,4,5,6);
    

    if (pInst == NULL)    return;
	
    pDrvCtrl =(GPIO_DRV_CTRL * )hwMemAlloc(sizeof (*pDrvCtrl));
    if (pDrvCtrl == NULL)  return;  
	
    /* get the HCF device from vxBus device structure */
    pHcf = hcfDeviceGet (pInst);
    if (pHcf == NULL)     return;
    
    if (devResourceGet(pHcf,"regBase", HCF_RES_INT,(void *)&pDrvCtrl->regBase) != OK)
        pDrvCtrl->regBase = 0; 
    if (devResourceGet(pHcf,"ioNum", HCF_RES_INT,(void *)&pDrvCtrl->ioNum) != OK)
        pDrvCtrl->ioNum = 0; 
    GPIO_DBG_MSG("run into gpioInstInit--%d\n" ,pDrvCtrl->ioNum,2,3,4,5,6);
	pInst->pDrvCtrl = pDrvCtrl;
    /* publish methods */
    pInst->pMethods = &gpio_methods[0];
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

LOCAL void gpioInstInit2
    (
    VXB_DEVICE_ID pInst
    )
    {
    UINT32 i;
    GPIO_DRV_CTRL *pDrvCtrl;
    
    GPIO_DBG_MSG("run into gpioInstInit2\n" ,1,2,3,4,5,6);
    pDrvCtrl=pInst->pDrvCtrl;
   
    for(i=0;i<(pDrvCtrl->ioNum);i++)
    {
    	pDrvCtrl->gpioIsrPrio[i].cause=i;
    	pDrvCtrl->gpioIsrPrio[i].intPrio=100;
    	pDrvCtrl->gpioIsrEntry[i].userISR = NULL;
    	pDrvCtrl->gpioIsrEntry[i].prio=100;
    	
    	gpioIntCount[i]=0;
    }
    
    
    *(UINT32*)(pDrvCtrl->regBase+GPIO_INT_FLAG_REG) = 0;
    
	if(sysGetCpuID()==0) *(UINT32*)(pDrvCtrl->regBase+GPIO_INT_MASK_SET_REG_A)=0xffffffff;
	else *(UINT32*)(pDrvCtrl->regBase+GPIO_INT_MASK_SET_REG_B)=0xffffffff;
    
    pDrvCtrl->ioNum = GPIO_IO_NUM;
    pDrvCtrl->gpioAccessSem = semBCreate (SEM_Q_FIFO, SEM_FULL);
	vxbIntConnect (pInst, 0, (VOIDFUNCPTR)gpioIntHandle,pInst);
	vxbIntEnable (pInst, 0,(VOIDFUNCPTR) gpioIntHandle,pInst);
	
	*(UINT32*)(pDrvCtrl->regBase+PULSE_INT_FLAG_REG)=1;
	*(UINT32*)(pDrvCtrl->regBase+PULSE_INT_MASK_SET_REG)=1; /*disable*/
	vxbIntConnect (pInst, 1, (VOIDFUNCPTR)pulseIntHandle,pInst);
	vxbIntEnable (pInst, 1,(VOIDFUNCPTR) pulseIntHandle,pInst);
	
	
	if(sysGetCpuID() == 0) *(UINT32*)(pDrvCtrl->regBase+GPIO_INT_ENABLE_A)=1;
	else  *(UINT32*)(pDrvCtrl->regBase+GPIO_INT_ENABLE_B)=1;

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

LOCAL void gpioInstConnect
    (
    VXB_DEVICE_ID pInst
    )
    {
	/*GPIO_DBG_MSG("gpioInstConnect() called\n", 1,2,3,4,5,6);*/
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

LOCAL STATUS gpioDriverCtrl
    (
    VXB_DEVICE_ID               pInst,
    struct vxbDriverControl *   pControl
    )
    {
	
	GPIO_DRV_CTRL *pDrvCtrl;
   	
   	GPIO_DRV_CTRL_INFO *pDrvCtrlInfo;
   	UINT32 regBase,i,j;
   	UINT32 cause=0,value=0,dirValue=0,causeTemp;
   	
    /* check whether this is for us */
    pDrvCtrl =(GPIO_DRV_CTRL *)pInst->pDrvCtrl;
    pDrvCtrlInfo=pControl->drvCtrlInfo;
    if ( pInst == NULL || pControl == NULL )   return(ERROR);
    if ( strcmp(pControl->driverName, "gpio" ) != 0 ) return(ERROR);
    if(semTake(pDrvCtrl->gpioAccessSem,0x100)==ERROR)/*WAIT_FOREVER*/ 
    {
    	printf("a process is using the IO now!!\n");
    	 return(ERROR);	
    }
    regBase=pDrvCtrl->regBase;
    cause= pDrvCtrlInfo->cause;
    dirValue= *(UINT32*)(regBase+GPIO_DIR_SET_REG);
    if(cause>(pDrvCtrl->ioNum)) goto error;
	if((cause<8)||((cause>15)&(cause<24))) dirValue=(dirValue>>((cause/8)*8))&0x01;
	else dirValue= (dirValue >>cause)&0x01;
    switch (pControl->drvCtrlCmd)
    {
    
		case GPIO_WRITE:
		{
			if(dirValue!=0)
			{
				printf("the pin is  \"INPUT\"\n");
				goto error;
			}
			else 
			{
				value= (*pDrvCtrlInfo->value)&0x01;
				GPIO_DBG_MSG("GPIO_WRITE:%d(IO%d)\n",value,cause,3,4,5,6);
				if(value) *(UINT32*)(regBase+GPIO_OUTPUT_DATA_REG) |= (1<<cause);
				else 	*(UINT32*)(regBase+GPIO_OUTPUT_DATA_REG) &= (~(1<<cause));	
			}
			break;	
		}
		case GPIO_READ:
		{
			if(dirValue == 0)
			{
				printf("the pin is  \"OUTPUT\"\n");
				goto error;
			}
			else
			{
				value=((*(UINT32*)(regBase+GPIO_INPUT_DATA_REG))>>cause)&0x01;
				GPIO_DBG_MSG("GPIO_READ:%d(IO%d)\n",value,cause,3,4,5,6);
				*pDrvCtrlInfo->value=value;	
			}
			break;	
		}
		case GPIO_DIR_SET:
		{
			value= (*pDrvCtrlInfo->value)&0x01;
			GPIO_DBG_MSG("GPIO_DIR_SET:%d(IO%d)\n",value,cause,3,4,5,6);
			if((cause<8)||((cause>15)&(cause<24))) causeTemp= (cause/8)*8;
			else causeTemp = cause;
			if(value) 	*(UINT32*)(regBase+GPIO_DIR_SET_REG) |= (1<<causeTemp);
			else 		*(UINT32*)(regBase+GPIO_DIR_SET_REG) &= (~(1<<causeTemp));	
			break;	
		}


		default:
			return ERROR;
			break;
    }
    
    semGive(pDrvCtrl->gpioAccessSem);
    return OK;
error:
	semGive(pDrvCtrl->gpioAccessSem);
	return ERROR;
    }


extern int sysGetCpuID(void);

LOCAL void gpioIntHandle(VXB_DEVICE_ID pInst)
{
	UINT32 intFlag,regBase,intMask,dirValue;
	UINT8 i,ioNum,temp/*,j*/;
    GPIO_DRV_CTRL *pDrvCtrl;
    volatile UINT32 clearInt=0;
    
  //  *(UINT32*)(regBase+GPIO_INT_ENABLE) &=(~(1<<0)); /*disable int*/
    pDrvCtrl=(GPIO_DRV_CTRL*)pInst->pDrvCtrl;
    regBase = pDrvCtrl->regBase;
    ioNum = pDrvCtrl->ioNum;
	intFlag =*(UINT32*)(regBase+GPIO_INT_FLAG_REG);
	dirValue =*(UINT32*)(regBase+GPIO_DIR_SET_REG);
	if(sysGetCpuID()==0) intMask =*(UINT32*)(regBase+GPIO_INT_MASK_SET_REG_A);
	else intMask =*(UINT32*)(regBase+GPIO_INT_MASK_SET_REG_B);

	//logMsg("intMask=0x%x,intFlag=0x%x\n",intMask,intFlag,3,4,5,6);
	for(i=0;i<ioNum;i++)
	{
		if(((intFlag>>i)&0x01)&& (!((intMask>>i)&0x01)))
		{
			gpioIntCount[i]+=1;
			
			
			
			if((i<8)||((i>15)&&(i<25))) temp=(i/8)*8;
			else temp=i;
			if((dirValue>>temp)&0x01) 
			{				
				clearInt=0;
				clearInt |=(1<<i);
				*(UINT32*)(regBase+GPIO_INT_FLAG_REG)=clearInt;
	

				clearInt=0;
				clearInt |=(1<<i);
				*(UINT32*)(regBase+GPIO_INT_FLAG_REG)=clearInt;
				
				clearInt=0;
				clearInt |=(1<<i);
				*(UINT32*)(regBase+GPIO_INT_FLAG_REG)=clearInt;
				
				clearInt=0;
				clearInt |=(1<<i);
				*(UINT32*)(regBase+GPIO_INT_FLAG_REG)=clearInt;
				
				logMsg("i=0x%x\n",i,2,3,4,5,6);

				if(pDrvCtrl->gpioIsrEntry[i].userISR!=NULL) (pDrvCtrl->gpioIsrEntry[i].userISR)(i);
			}
		}
	}
	//*(UINT32*)(regBase+GPIO_INT_ENABLE) |=((1<<0)); /*enable int*/
}

LOCAL void pulseIntHandle(VXB_DEVICE_ID pInst)
{
	GPIO_DRV_CTRL *pDrvCtrl;
	//logMsg("enter pulseIntHandle\n",1,2,3,4,5,6);
	pDrvCtrl=(GPIO_DRV_CTRL*)pInst->pDrvCtrl;
	*(UINT32*)(pDrvCtrl->regBase+PULSE_INT_MASK_SET_REG)=1;
	*(UINT32*)(pDrvCtrl->regBase+PULSE_INT_FLAG_REG)=1;
	if(pDrvCtrl->pulseIntCallBack!=NULL) pDrvCtrl->pulseIntCallBack();
	*(UINT32*)(pDrvCtrl->regBase+PULSE_INT_MASK_SET_REG)=0;
	*(UINT32*)(pDrvCtrl->regBase+PULSE_INT_MASK_SET_REG)=0;
	*(UINT32*)(pDrvCtrl->regBase+PULSE_INT_MASK_SET_REG)=0;
	//logMsg("exit pulseIntHandle\n",*(UINT32*)(pDrvCtrl->regBase+PULSE_INT_MASK_SET_REG),2,3,4,5,6);
}
