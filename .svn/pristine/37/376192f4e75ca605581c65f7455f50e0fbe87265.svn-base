
#include <vxWorks.h>
#include <stdio.h>			/* for printf() */
#include <logLib.h>			/* for logMsg() */
#include <string.h>			/* for strcmp() */
#include "vxbGpioDrv.h"

/* define */
#define GPIO_DEBUG
//#undef GPIO_DEBUG

#ifdef GPIO_DEBUG
#define GPIO_MODE_INPUT			1
#define GPIO_MODE_OUTPUT		0
#endif


/**************************************************
 * GPIO 
 * 
 ****************************************************/

static STATUS bspGpioMethodGet(UINT32 methodCmd,VXB_DEVICE_ID *pInst, FUNCPTR *handler)
{
	 VXB_DEVICE_ID pDev;
	 pDev=(VXB_DEVICE_ID)vxbInstByNameFind("gpioDrv",0);
	 if(pDev == NULL)
	 {
		 logMsg("sysGpioMethodGet:vxbDevMethodGetERROR\n",1,2,3,4,5,6);
		 return ERROR;
	 }
	 *pInst = pDev;
	switch(methodCmd)
	{
	case GPIO_MODE_SET:
		*handler = vxbDevMethodGet(pDev, DEVMETHOD_CALL(gpioModeSet));
		break;
	case GPIO_MODE_GET:
		*handler = vxbDevMethodGet(pDev, DEVMETHOD_CALL(gpioModeGet));
		break;
	case GPIO_INTMODE_SET:
		*handler = vxbDevMethodGet(pDev, DEVMETHOD_CALL(gpioIntModeSet));
		break;
	case GPIO_VALUE_SET:
		*handler = vxbDevMethodGet(pDev, DEVMETHOD_CALL(gpioValueSet));
		break;
	case GPIO_VALUE_GET:
		//printf("bspGpioMethodGet:GPIO_VALUE_GET\n");
		*handler = vxbDevMethodGet(pDev, DEVMETHOD_CALL(gpioValueGet));
		break;
	default:
		break;
	}
	return OK;
}

static int bspGpioCtrl(UINT32 methodCmd,struct sGpioInfo gpioInfo)
{
	VXB_DEVICE_ID pInst;
	FUNCPTR handler;
	int status=OK;
	if(bspGpioMethodGet(methodCmd,&pInst, &handler)!=OK) 	return ERROR;
	
	switch(methodCmd)
	{
	case GPIO_MODE_SET:
		handler(pInst,gpioInfo.gpioIndex,gpioInfo.mode);
		status = OK;
		break;
	case GPIO_MODE_GET:
		gpioInfo.mode = handler(pInst,gpioInfo.gpioIndex);
		status = gpioInfo.mode;
		break;
	case GPIO_INTMODE_SET:
		status = handler(pInst,gpioInfo.gpioIndex,gpioInfo.intmode);
		break;
	case GPIO_VALUE_SET:
		status = handler(pInst,gpioInfo.gpioIndex,gpioInfo.value);
		break;
	case GPIO_VALUE_GET:
		//printf("bspGpioCtrl:0x%x\n",gpioInfo.gpioIndex);
		status = handler(pInst,gpioInfo.gpioIndex);
		//gpioInfo.value = handler(pInst,gpioInfo.gpioIndex);
		//status = gpioInfo.value;
		break;
#ifdef DRV_INTCTRL_GPIO
	case GPIO_INT_CONNECT:
		status = vxbIntConnect (pInst, gpioInfo.gpioIndex, (VOIDFUNCPTR)gpioInfo.pIsr, (void*)gpioInfo.gpioIndex);
		break;
	case GPIO_INT_DISCONNECT:
		status = vxbIntDisconnect (pInst, gpioInfo.gpioIndex, (VOIDFUNCPTR)gpioInfo.pIsr, (void*)gpioInfo.gpioIndex);
		break;
	case GPIO_INT_ENABLE:
		status = vxbIntEnable  (pInst, gpioInfo.gpioIndex, (VOIDFUNCPTR)gpioInfo.pIsr, (void*)gpioInfo.gpioIndex);
		if(status == ERROR) printf("vxbIntEnable error!\n");
		break;
	case GPIO_INT_DISABLE:
		status = vxbIntDisable  (pInst, gpioInfo.gpioIndex, (VOIDFUNCPTR)gpioInfo.pIsr, (void*)gpioInfo.gpioIndex);
		break;
#endif
	default:
		break;
	}
return status;
}


STATUS bspGpioModeSet(UINT8 pinIndex,int mode)
{
	struct sGpioInfo gpioInfo;
	
	gpioInfo.gpioIndex = pinIndex;
	gpioInfo.mode = mode;

	if(bspGpioCtrl(GPIO_MODE_SET,gpioInfo) != OK)
	{
		printf("set GPIO%d mode failed\n",pinIndex);
		return ERROR;
	}
	return OK;
}

STATUS bspGpioStatusSet(int pinIndex,BOOL pinValue)
{
	struct sGpioInfo gpioInfo;
	
	gpioInfo.gpioIndex = pinIndex;
	gpioInfo.value = pinValue;
	if(bspGpioCtrl(GPIO_VALUE_SET,gpioInfo) != OK) return ERROR;
	return OK;
}
		
int bspGpioStatusGet(int pinIndex)
{
	struct sGpioInfo gpioInfo;
	int pinValue;
	
	gpioInfo.gpioIndex = pinIndex;
	//printf("pinIndex=%d\n",pinIndex);
	pinValue = bspGpioCtrl(GPIO_VALUE_GET,gpioInfo) ;
	return pinValue;
}

VXB_DEVICE_ID bspGpioIntInstFind()
{
	VXB_DEVICE_ID pInst = NULL;
	
	pInst=(VXB_DEVICE_ID)vxbInstByNameFind ("gpioDrv",0);
	//printf("pInst = 0x%x\n",pInst);
	if(pInst==NULL) printf ("Get pInst failed\n");
	return pInst;
}

/**************************************************
 * GPIO INT
 * 
 ****************************************************/

#ifdef DRV_INTCTRL_GPIO
STATUS bspGpioIntEnable(int intPin,VOIDFUNCPTR gpioIntIsr)
{
	struct sGpioInfo gpioInfo;
	if(gpioIntIsr == NULL) 
	{
		printf("gpioIntIsr=NULL\n");
		return ERROR;
	}
	gpioInfo.gpioIndex = intPin;
	gpioInfo.arg = (void*)intPin;
	gpioInfo.pIsr = (VOIDFUNCPTR)gpioIntIsr;
	if(bspGpioCtrl(GPIO_INT_ENABLE,gpioInfo)!=OK)
	{
		printf("GPIO_INT_ENABLE failed!\n");
		return ERROR;	
	}
	return OK;
}

STATUS bspGpioIntDisable(int intPin,VOIDFUNCPTR gpioIntIsr)
{
	struct sGpioInfo gpioInfo;
	gpioInfo.gpioIndex = intPin;
	gpioInfo.arg = (void*)intPin;
	gpioInfo.pIsr = (VOIDFUNCPTR)gpioIntIsr;
	if(bspGpioCtrl(GPIO_INT_DISABLE,gpioInfo)!=OK)
	{
		printf("GPIO_INT_DISABLE failed!\n");
		return ERROR;	
	}
	return OK;
}

STATUS bspGpioIntConnect(int intPin,VOIDFUNCPTR gpioIntIsr)
{
	struct sGpioInfo gpioInfo;

	gpioInfo.gpioIndex = intPin;
	gpioInfo.arg = (void*)intPin;
	
	gpioInfo.pIsr = (VOIDFUNCPTR)gpioIntIsr;
	if(bspGpioCtrl(GPIO_INT_CONNECT,gpioInfo)!=OK)
	{
		printf("GPIO_INT_CONNECT failed!\n");
		return ERROR;	
	}
	return OK;
}

STATUS bspGpioIntModeSet(int intPin,int mode)
{
	struct sGpioInfo gpioInfo;

	gpioInfo.gpioIndex = intPin;
	gpioInfo.intmode = mode;	/* intMode : 0----ÉÏÉýÑØ;1----ÏÂ½µÑØ*/
	if(bspGpioCtrl(GPIO_INTMODE_SET,gpioInfo)!=OK)
	{
		printf("GPIO_INTMODE_SET failed!\n");
		return ERROR;	
	}
	return OK;
}

STATUS bspGpioIntClean(int intPin)
{	 
	VXB_DEVICE_ID pDev;
	FUNCPTR  handler;
	
	pDev=(VXB_DEVICE_ID)vxbInstByNameFind("gpioIntCtrl",0);
	
	if(pDev == NULL)
	{
	 logMsg("bspGpioIntClean:vxbDevMethodGetERROR\n",1,2,3,4,5,6);
	 return ERROR;
	}

	handler = vxbDevMethodGet(pDev, DEVMETHOD_CALL(gpioIntClear));
	return(handler(pDev,intPin));
}

#endif


#ifdef GPIO_DEBUG
void gpioTest(UINT8 gpioIn,UINT8 gpioOut,BOOL outStatus)
{
	int value;
	bspGpioModeSet(gpioIn,GPIO_MODE_INPUT);
	bspGpioModeSet(gpioOut,GPIO_MODE_OUTPUT);
	//printf("set gpio%d : IN  gpio%d : OUT\n",gpioIn,gpioOut);
	value = bspGpioStatusGet(gpioIn);
	//printf("gpio%d : IN %d\n",gpioIn,value);
	//printf("gpio%d : OUT %d\n",gpioOut,outStatus);
	bspGpioStatusSet(gpioOut,outStatus);
	
	value = bspGpioStatusGet(gpioIn);
	printf("gpio%d : IN %d\n",gpioIn,value);
}

void gpioTestAll(BOOL outStatus)
{
	int i=0;
	for(i=0; i<4; i++)
	{
		gpioTest(i*2,15-(i*2),0);
		gpioTest(i*2+1,14-(i*2),1);
	}
}


LOCAL void gpioIntHandle(int index)
{
	//bspGpioIntClean(index);
	logMsg("%d enter INT\n",index,2,3,4,5,6);
}

void gpioIntTest(UINT8 gpioIn,UINT8 gpioOut,UINT8 mode)
{
	bspGpioModeSet(gpioIn,GPIO_MODE_INPUT);
	bspGpioModeSet(gpioOut,GPIO_MODE_OUTPUT);

	bspGpioIntModeSet(gpioIn,mode);
	bspGpioIntConnect(gpioIn,gpioIntHandle);
	bspGpioIntClean(gpioIn);
	bspGpioIntEnable(gpioIn,gpioIntHandle);
}




void gpioTrigger(UINT8 gpioOut,UINT8 mode)
{
	bspGpioModeSet(gpioOut,GPIO_MODE_OUTPUT);
	if(mode)
	{
		bspGpioStatusSet(gpioOut,1);
		bspGpioStatusSet(gpioOut,0);
	}
	else
	{
		bspGpioStatusSet(gpioOut,0);
		bspGpioStatusSet(gpioOut,1);
	}
}
#endif

