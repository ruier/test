

/* includes */

#include <vxWorks.h>
#include <taskLib.h>			/* for taskSpawn() and taskDelay() */
#include <sysLib.h>			/* for sysClkRateGet() */
#include <stdio.h>			/* for printf() */
#include <logLib.h>			/* for logMsg() */
#include <string.h>			/* for bfill() */
#include <vxBusLib.h>			/* for vxbDeviceMethodRun()
					 * and  VXB_DEVICE_ID
					 */

#include <driverControl.h>		/* for struct vxbDriverControl */
#include <hwif/vxbus/vxbMethodDecl.h>	/* for DEVMETHOD_CALL(driverControl) */
#include <vxbm8641SynUart.h>			/* for struct wrsampleDriverControl
					 * and WRSAMPLE_GET_SAMPLE_FUNCS					 */
#include <cpuset.h>


#define BSP_SYN_UART_DBG_ON
#undef BSP_SYN_UART_DBG_ON

#ifdef BSP_SYN_UART_DBG_ON
static char sprintBufUart[100];
#define BSP_SYN_UART_DBG_MSG(X0, X1, X2, X3, X4, X5, X6)	{sprintf(sprintBufUart,X0, X1, X2, X3, X4, X5, X6);ns16550Pollprintf(sprintBufUart);}
#undef STATIC
#define STATIC

#else /* BSP_SYN_UART_DBG_ON */
#define BSP_SYN_UART_DBG_MSG(fmt,a,b,c,d,e,f)
#undef STATIC
#define STATIC static
#endif /* BSP_SYN_UART_DBG_ON */





STATIC STATUS bspSynUartIoCtrl(struct vxbDriverControl ctrl,int synUartNum)
{
	VXB_DEVICE_ID pDev;
	FUNCPTR     handler;
		 
	pDev=(VXB_DEVICE_ID)vxbInstByNameFind("synUart",synUartNum);	
	handler = vxbDevMethodGet(pDev, DEVMETHOD_CALL(driverControl));
	if( handler == NULL )
	{
		printf("bspSynUartIoCtrl:vxbDevMethodGet != OK!!!!\n",1,2,3,4,5,6);
		return ERROR;
	}
	if((*handler)(pDev,&ctrl )==ERROR)
	{	
		printf("bspSynUartIoCtrl:handle != OK!!!!\n",1,2,3,4,5,6);
		return ERROR;
	}
	return OK;	
}

STATIC STATUS bspSynUartCheck(UINT32 synUartNum,UINT32 flag) /*flag-1:rcv 0:send*/
{
	struct vxbDriverControl ctrl;
	SYN_UART_DRV_CTRL_INFO drvCtrlInfo;
	UINT32 data;
	
	ctrl.drvCtrlCmd  = SYN_UART_FUNC_GET;	
	ctrl.driverName  = "synUart";		
	ctrl.drvCtrlInfo = &drvCtrlInfo;
	drvCtrlInfo.value = &data;
	if(bspSynUartIoCtrl(ctrl,synUartNum) ==ERROR)
	{
		printf("bspSynUartCheck:bspSynUartIoCtrl() != OK!!!!\n",1,2,3,4,5,6);
		return ERROR;
	}
	if(data!= flag) return ERROR;
	return OK;	
}

STATUS bspSynUartBaudSet(UINT32 synUartNum,UINT32 baudRate)
{
	struct vxbDriverControl ctrl;
	SYN_UART_DRV_CTRL_INFO drvCtrlInfo;
	UINT32 data = 0;
	
	if(baudRate>3) baudRate =3;
	
	
	data= 1<<baudRate;
	ctrl.drvCtrlCmd  = SYN_UART_BAUD_SET;	
	ctrl.driverName  = "synUart";		
	ctrl.drvCtrlInfo = &drvCtrlInfo;
	drvCtrlInfo.value = &data;
	if(bspSynUartIoCtrl(ctrl,synUartNum) ==ERROR)
	{
		printf("bspSynUartCheck:bspSynUartIoCtrl() != OK!!!!\n",1,2,3,4,5,6);
		return ERROR;
	}
	return OK;		
}
STATUS bspSynUartSwitchCPU(UINT32 synUartNum,UINT32 cpuId)
{
	struct vxbDriverControl ctrl;
	SYN_UART_DRV_CTRL_INFO drvCtrlInfo;
	UINT32 data = 0;
	//UINT32 cpuId = 0;
	//cpuId = sysGetCpuID();


	
	ctrl.drvCtrlCmd  = SYN_UART_SWITCH_CPU;	
	ctrl.driverName  = "synUart";		
	ctrl.drvCtrlInfo = &drvCtrlInfo;
	drvCtrlInfo.value = &cpuId;

	if(bspSynUartIoCtrl(ctrl,synUartNum) ==ERROR)
	{
		printf("bspSynUartCheck:bspSynUartIoCtrl() != OK!!!!\n",1,2,3,4,5,6);
		return ERROR;
	}
	
	return OK;		
}
STATUS bspSynUartBaudGet(UINT32 synUartNum,UINT32 *baudRate)
{
	struct vxbDriverControl ctrl;	
	SYN_UART_DRV_CTRL_INFO drvCtrlInfo;
	UINT32 i,data;
	
	ctrl.drvCtrlCmd  = SYN_UART_BAUD_GET;	
	ctrl.driverName  = "synUart";		
	ctrl.drvCtrlInfo = &drvCtrlInfo;
	drvCtrlInfo.value = &data;
	if(bspSynUartIoCtrl(ctrl,synUartNum) ==ERROR)
	{
		printf("bspSynUartCheck:bspSynUartIoCtrl() != OK!!!!\n",1,2,3,4,5,6);
		return ERROR;
	}
	data &= 0xf;
	for(i=0;i<4;i++)
	{
		if((data>>i)&0x01)
		{
			*baudRate=i;
			return OK;
		}
	}
	return OK;		
}


STATIC STATUS bspSynUartStatusGet(UINT32 synUartNum,UINT32 *pData)
{
	struct vxbDriverControl ctrl;
	SYN_UART_DRV_CTRL_INFO drvCtrlInfo;
	ctrl.drvCtrlCmd  = SYN_UART_STATUS_GET;	
	ctrl.driverName  = "synUart";		
	ctrl.drvCtrlInfo = &drvCtrlInfo;
	drvCtrlInfo.value=pData;
	if(bspSynUartIoCtrl(ctrl,synUartNum) ==ERROR)
	{
		printf("bspSynUartStatusGet:bspSynUartIoCtrl() != OK!!!!\n",1,2,3,4,5,6);
		return ERROR;
	}
	return OK;
}


STATIC STATUS bspSynUartChainMaskSet(UINT32 synUartNum,UINT32 value)
{
	
	struct vxbDriverControl ctrl;
	UINT32 data;
	SYN_UART_DRV_CTRL_INFO   drvCtrlInfo;
	data = value&0x01;

	if(data==1) ctrl.drvCtrlCmd  = SYN_UART_CHAIN_ENABLE;	
	else  ctrl.drvCtrlCmd  = SYN_UART_CHAIN_DISABLE;	
	ctrl.driverName  = "synUart";		
	ctrl.drvCtrlInfo = &drvCtrlInfo;
	//BSP_ROCKETIO_DBG_MSG("rocketIoIntMaskSet\n",1,2,3,4,5,6);
	if(bspSynUartIoCtrl(ctrl,synUartNum) ==ERROR)
	{
		printf("bspSynUartIntMaskSet:bspSynUartIoCtrl() != OK!!!!\n",1,2,3,4,5,6);
		return ERROR;
	}
	return OK;
	
}


STATIC STATUS bspSynUartIntMaskSet(UINT32 synUartNum,UINT32 value,UINT32 index)
{
	
	struct vxbDriverControl ctrl;
	UINT32 data;
	SYN_UART_DRV_CTRL_INFO   drvCtrlInfo;
	data = value&0x01;

	if(index==RCV_INT_F) ctrl.drvCtrlCmd  = SYN_UART_RCV_INT_MASK;	
	else if(index==EMPTY_INT_F) ctrl.drvCtrlCmd  = SYN_UART_EMPTY_INT_MASK;	
	ctrl.driverName  = "synUart";		
	ctrl.drvCtrlInfo = &drvCtrlInfo;
	drvCtrlInfo.value=&data;
	//BSP_ROCKETIO_DBG_MSG("rocketIoIntMaskSet\n",1,2,3,4,5,6);
	if(bspSynUartIoCtrl(ctrl,synUartNum) ==ERROR)
	{
		printf("bspSynUartIntMaskSet:bspSynUartIoCtrl() != OK!!!!\n",1,2,3,4,5,6);
		return ERROR;
	}
	return OK;
	
}


STATUS bspSynUartIntConnect(UINT32 synUartNum,VOIDFUNCPTR routine,UINT32 index)
{
	struct vxbDriverControl ctrl;
	SYN_UART_DRV_CTRL_INFO drvCtrlInfo;
	if(index==RCV_INT_F) ctrl.drvCtrlCmd  = SYN_UART_RCV_INT_SETUP;	
	else if(index==EMPTY_INT_F) ctrl.drvCtrlCmd  = SYN_UART_EMPTY_INT_SETUP;
	ctrl.driverName  = "synUart";		
	ctrl.drvCtrlInfo = &drvCtrlInfo;
	drvCtrlInfo.intCallBack =routine;	
	if(bspSynUartIoCtrl(ctrl,synUartNum) ==ERROR)
	{
		printf("bspSynUartIntConnect:bspSynUartIoCtrl() != OK!!!!\n",1,2,3,4,5,6);
		return ERROR;
	}
	return OK;
}



STATUS bspSynUartChainEnable(UINT32 synUartNum)
{
	if(bspSynUartChainMaskSet(synUartNum,1)==ERROR) return ERROR;
	return OK;
}

STATUS bspSynUartChainDisable(UINT32 synUartNum)
{
	if(bspSynUartChainMaskSet(synUartNum,0)==ERROR) return ERROR;
	return OK;
}


STATUS bspSynUartIntEnable(UINT32 synUartNum,UINT32 index)
{
	if(bspSynUartIntMaskSet(synUartNum,0,index)==ERROR) return ERROR;
	return OK;
}

STATUS bspSynUartIntDisable(UINT32 synUartNum,UINT32 index)
{
	if(bspSynUartIntMaskSet(synUartNum,1,index)==ERROR) return ERROR;
	return OK;
}


STATUS bspSynUartRcv(UINT32 synUartNum,UINT16 *pData,UINT32 *size)
{
	struct vxbDriverControl ctrl;
	SYN_UART_DRV_CTRL_INFO drvCtrlInfo;
	UINT32 i,rcvSize=0;
	UINT32 data;
	
	if(bspSynUartCheck(synUartNum,1)==ERROR) return ERROR;
	if(bspSynUartStatusGet(synUartNum,&rcvSize) ==ERROR) return ERROR;
	rcvSize >>= 16;
	rcvSize &= 0xff;
	printf("rcvSize=0x%x\n",rcvSize*2);
	ctrl.drvCtrlCmd  = SYN_UART_READ;	
	ctrl.driverName  = "synUart";		
	ctrl.drvCtrlInfo = &drvCtrlInfo;
	drvCtrlInfo.value = &data;
	for(i=0;i<rcvSize;i++)
	{		
		if(bspSynUartIoCtrl(ctrl,synUartNum) ==ERROR)
		{
			printf("synUartSendStart:bspSynUartIoCtrl() != OK!!!!\n",1,2,3,4,5,6);
			return ERROR;
		}
		(*pData++) = data &0xffff;
	}
	*size = rcvSize<<1;
	return OK;
}


STATUS bspSynUartSend(UINT32 synUartNum,UINT16 *Data,UINT32 size)
{
	struct vxbDriverControl ctrl;
	SYN_UART_DRV_CTRL_INFO drvCtrlInfo;
	UINT32 i,data;
	UINT16 *pData=Data;
	
	if(bspSynUartCheck(synUartNum,0)==ERROR) 
	{
		printf("this chain is not support send!!\n");
		return ERROR;
	}
	ctrl.drvCtrlCmd  = SYN_UART_WRITE;	
	ctrl.driverName  = "synUart";		
	ctrl.drvCtrlInfo = &drvCtrlInfo;
	for(i=0;i<(size/2-1);i++)
	{
	
		data = *(pData++) & 0xffff;
		//printf("bspSynUartSend-%d:0x%x\n",i,data);
		drvCtrlInfo.value =&data;
		if(bspSynUartIoCtrl(ctrl,synUartNum) ==ERROR)
		{
			printf("synUartSendStart:bspSynUartIoCtrl() != OK!!!!\n",1,2,3,4,5,6);
			return ERROR;
		}
	}
	data = (*pData & 0xffff)|(1<<16);
	//printf("bspSynUartSend-%d:0x%x\n",i,data);
	drvCtrlInfo.value =&data;
	if(bspSynUartIoCtrl(ctrl,synUartNum) ==ERROR)
	{
		printf("bspSynUartSend:bspSynUartIoCtrl() != OK!!!!\n",1,2,3,4,5,6);
		return ERROR;
	}
	return OK;
}



