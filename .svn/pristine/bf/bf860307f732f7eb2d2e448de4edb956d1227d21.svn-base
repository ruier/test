#include <vxWorks.h>
#include <stdio.h>
#include <string.h>
#include <logLib.h>			/* for logMsg() */
#include <vxBusLib.h>	
#include <hwif/vxbus/vxBus.h>
#include <taskLibCommon.h>
#include <intLib.h>
#include <tickLib.h>
#include <driverControl.h>		/* for struct vxbDriverControl */
#include <hwif/vxbus/hwConf.h>		/* for devResourceGet() and hcfDeviceGet() */
#include <hwif/util/hwMemLib.h>		/* for hwMemAlloc() */


#include "canDrv.h"


extern int ns16550Pollprintf(char* pchar);
/*************************************************/

LOCAL void fCanInstInit(VXB_DEVICE_ID pInst);
LOCAL void fCanInstInit2(VXB_DEVICE_ID pInst);
LOCAL void fCanInstConnect(VXB_DEVICE_ID pInst);
LOCAL void fCanIntHandle(VXB_DEVICE_ID   pInst);


//IMPORT  void 	intUnlock (int );
//IMPORT  int 	intLock (void);



/*****************************************************/





/*****************************************************/
/* structure to store the driver functions for vxBus */
LOCAL struct drvBusFuncs fCanDrvFuncs =
{
    fCanInstInit,        /* devInstanceInit */
    fCanInstInit2,       /* devInstanceInit2 */
    fCanInstConnect      /* devConnect */
};



/* structure used for registration with vxbus */
LOCAL struct vxbDevRegInfo fCanDevRegistration =
{
    NULL,                       /* pNext */
    VXB_DEVID_DEVICE,           /* devID */
    VXB_BUSID_PLB,              /* busID = PLB */
    VXB_VER_4_2_1,
    FCAN_DRV_NAME,          /* drvName */
    &fCanDrvFuncs,          /* pDrvBusFuncs */
    NULL,                       /* pMethods */
    NULL,                       /* devProbe */
    NULL                        /* pParamDefaults */
};


/**********************************************/
extern int sysGetCpuID(void);
void fCanDevRegister (void)
{
	if(sysGetCpuID() == 0)
	{
		BSP_CANDRV_TEST("come to fCanDevRegister...\n",1,2,3,4,5,6);
		vxbDevRegister ((struct vxbDevRegInfo *)&fCanDevRegistration);
	}
}



LOCAL void fCanInstInit(VXB_DEVICE_ID pInst)
{
	FCAN_DRV_CTRL * pDrvCtrl = NULL;
    HCF_DEVICE * pHcf = NULL;
    
    BSP_CANDRV_TEST("come to fCanInstInit...\n",1,2,3,4,5,6);

    if(pInst == NULL)    return;
    
    pDrvCtrl =(FCAN_DRV_CTRL * )hwMemAlloc(sizeof (*pDrvCtrl));
    if(pDrvCtrl == NULL)	return;
    
    pHcf = hcfDeviceGet (pInst);
    if (pHcf == NULL)     return;
    
    if(devResourceGet(pHcf,"regBase", HCF_RES_INT,(void *)&pDrvCtrl->regBase)!= OK)
    {	 	
    	pDrvCtrl->regBase = FCAN_REGBASE;
    }	
    
    if(devResourceGet(pHcf,"memBaseAddr", HCF_RES_INT,(void *)&pDrvCtrl->memBaseAddr)!= OK)
    {	     	
    	pDrvCtrl->memBaseAddr = FCAN_MEMBASE;
    }	
    if(devResourceGet(pHcf,"canDeviceId", HCF_RES_INT,(void *)&pDrvCtrl->deviceId)!= OK)
    {  	
    	pDrvCtrl->deviceId = FCAN_DEV_ID;
    }
    
//    if(devResourceGet(pHcf,"canClkDiv", HCF_RES_INT,(void *)&pDrvCtrl->fCanClkDiv)!= OK)
//    {  	
//    	pDrvCtrl->fCanClkDiv = FCAN_CLK_DIV;
//    }	
    
    pInst->pDrvCtrl = pDrvCtrl;
    pInst->pMethods = NULL;
    
}

LOCAL void fCanInstInit2(VXB_DEVICE_ID pInst)
{
	FCAN_DRV_CTRL * pDrvCtrl = NULL;
	
	BSP_CANDRV_TEST("come to fCanInstInit2...\n",1,2,3,4,5,6);
	pDrvCtrl = pInst->pDrvCtrl;
	
	if(pDrvCtrl == NULL)
	{
		return;

	}
		
	pDrvCtrl->accessSem = semMCreate(SEM_Q_PRIORITY|SEM_DELETE_SAFE|SEM_INVERSION_SAFE);
	
	vxbIntConnect (pInst, 0, (VOIDFUNCPTR)fCanIntHandle,pInst);
	vxbIntEnable (pInst, 0,(VOIDFUNCPTR)fCanIntHandle,pInst);
	
	FCAN_MCU_INT_ENABLE(pDrvCtrl->regBase) = 0; 
}



LOCAL void fCanInstConnect(VXB_DEVICE_ID pInst)
{
//	BSP_CANDRV_TEST("come to fCanInstConnect...\n",1,2,3,4,5,6);
}

//
//LOCAL STATUS fCanCallbackInstall(VXB_DEVICE_ID pInst, STATUS (*sendCallBack)(), STATUS (*rcvCallBack)())
//{
//	FCAN_DRV_CTRL *pDrvCtrl;
//	
//	pDrvCtrl = pInst->pDrvCtrl;
//	
//	if(pInst==NULL||pDrvCtrl==NULL)
//	{
//		BSP_CANDRV_TEST("fCanCallbackInstall: get pDrvCtrl error!!\n",1,2,3,4,5,6);
//		return ERROR;
//	}	
//	pDrvCtrl->sendCallback = sendCallBack;
//	pDrvCtrl->rcvCallback = rcvCallBack;
//		 
//	return OK;
//}


/* 中断处理函数 */
LOCAL void fCanIntHandle(VXB_DEVICE_ID   pInst)
{
//	int intTemp = intLock();
	FCAN_DRV_CTRL *pDrvCtrl;
	UINT8 status;
	UINT8 *dataBuf = NULL;
	UINT8 i = 0;
	UINT8 length;
	pDrvCtrl = pInst->pDrvCtrl;
	
	
	//stop interrupt
	FCAN_MCU_INT_ENABLE(pDrvCtrl->regBase) = 1;
	//clear interrupt status 
	FCAN_MCU_INT_STATUS(pDrvCtrl->regBase) = 1;
	
	//status = FCAN_STATUS_REG(pDrvCtrl->memBaseAddr + 0x1a);
	//logMsg("in int status 0x%x\n",status,2,3,4,5,6);
	
	//get bus0 data
	status = FCAN_STATUS_REG(pDrvCtrl->memBaseAddr);
	if(status & 0x4)
	{
		FCAN_STATUS_REG(pDrvCtrl->memBaseAddr) &= ~(0x4);
		pDrvCtrl->rcvDataFrame[0].dataEnable = 0;
		status = FCAN_STATUS_REG(pDrvCtrl->memBaseAddr);
		while(status & 0x8)
		{
			status = FCAN_STATUS_REG(pDrvCtrl->memBaseAddr);	
		}
		FCAN_STATUS_REG(pDrvCtrl->memBaseAddr) |= 0x8;
		pDrvCtrl->rcvDataFrame[0].ID =  (UINT32)FCAN_RXID_REG(pDrvCtrl->memBaseAddr);
		pDrvCtrl->rcvDataFrame[0].len =  (UINT32)FCAN_RXLEN_REG(pDrvCtrl->memBaseAddr);
		dataBuf = (UINT8*)FCAN_RXBUF_REG(pDrvCtrl->memBaseAddr);
		length = pDrvCtrl->rcvDataFrame[0].len;
		for(i=0; i<length; i++)
		{
			pDrvCtrl->rcvDataFrame[0].data[i] = dataBuf[i]; 
		}
		FCAN_STATUS_REG(pDrvCtrl->memBaseAddr) &= ~(0x8);
		pDrvCtrl->rcvDataFrame[0].dataEnable = 1;
	}
	
	//get bus1 data
	status = FCAN_STATUS_REG(pDrvCtrl->memBaseAddr + 0x1a);
//	logMsg("in int status 0x%x\n",status,2,3,4,5,6);
	if(status & 0x4)
	{
//		logMsg("in int\n",1,2,3,4,5,6);
		FCAN_STATUS_REG(pDrvCtrl->memBaseAddr + 0x1a) &= ~(0x4);
		pDrvCtrl->rcvDataFrame[1].dataEnable = 0;
		status = FCAN_STATUS_REG(pDrvCtrl->memBaseAddr + 0x1a);
		while(status & 0x8)
		{
			status = FCAN_STATUS_REG(pDrvCtrl->memBaseAddr + 0x1a);	
		}
		FCAN_STATUS_REG(pDrvCtrl->memBaseAddr + 0x1a) |= 0x8;
		pDrvCtrl->rcvDataFrame[1].ID =  (UINT32)FCAN_RXID_REG(pDrvCtrl->memBaseAddr + 0x1a);
		pDrvCtrl->rcvDataFrame[1].len =  (UINT32)FCAN_RXLEN_REG(pDrvCtrl->memBaseAddr + 0x1a);
		dataBuf = (UINT8*)FCAN_RXBUF_REG(pDrvCtrl->memBaseAddr + 0x1a);
		length = pDrvCtrl->rcvDataFrame[1].len;
		for(i=0; i<length; i++)
		{
			pDrvCtrl->rcvDataFrame[1].data[i] = dataBuf[i]; 
			//logMsg("%d  dataBuf = %d \n",pDrvCtrl->rcvDataFrame[1].data[i],dataBuf[i],2,3,4,5);
			
		}
		FCAN_STATUS_REG(pDrvCtrl->memBaseAddr + 0x1a) &= ~(0x8);
		pDrvCtrl->rcvDataFrame[1].dataEnable = 1;
	}
	
	
	//enable interrupt
	FCAN_MCU_INT_ENABLE(pDrvCtrl->regBase) = 0;
	
}



/*BSP发送接口函数*/
STATUS bspCanSend(UINT8 channel,UINT32 destId,UINT8 *pChar,int len,UINT8 mode)
{
	FCAN_DRV_CTRL *pDrvCtrl;
	VXB_DEVICE_ID pDev = NULL;
	UINT32 i = 0,length = 0;
	UINT8 status;
	UINT8 *dataBuf = NULL;

	pDev=(VXB_DEVICE_ID)vxbInstByNameFind("ppcFpgaCan",0);
	
	pDrvCtrl = pDev->pDrvCtrl;
	if(len > 8)
	{
		length = 8;
	}
	else
	{
		length = len;
	}
	
	switch(channel)
	{
	case 0:	
		//check busy bit
	//	status = FCAN_STATUS_REG(pDrvCtrl->memBaseAddr);
		while(status & 0x2)
		{
			status = FCAN_STATUS_REG(pDrvCtrl->memBaseAddr);
		}
		FCAN_STATUS_REG(pDrvCtrl->memBaseAddr) |= (UINT8)0x2;
		
		FCAN_TXID_REG(pDrvCtrl->memBaseAddr) = (UINT8) destId;
		FCAN_TXLEN_REG(pDrvCtrl->memBaseAddr) = (UINT8) length;
	
		dataBuf = (UINT8 *)FCAN_TXBUF_REG(pDrvCtrl->memBaseAddr);
		
		for(i=0; i<length; i++)
		{
			dataBuf[i] = pChar[i]; 
		}
		
		//clear busy bit
		FCAN_STATUS_REG(pDrvCtrl->memBaseAddr) &= (UINT8)(~(0x2));
		//set data frame enable
		FCAN_STATUS_REG(pDrvCtrl->memBaseAddr) |= (UINT8)(0x1);
		
		//send int to mcu
		FCAN_TO_MCU_INT_REG(pDrvCtrl->regBase) = 1;
		break;
	case 1:
		//check busy bit
		status = FCAN_STATUS_REG(pDrvCtrl->memBaseAddr + 0x1a);
		while(status & 0x2)
		{
			status = FCAN_STATUS_REG(pDrvCtrl->memBaseAddr + 0x1a);
		}
		FCAN_STATUS_REG(pDrvCtrl->memBaseAddr + 0x1a) |= (UINT8)0x2;
		
		FCAN_TXID_REG(pDrvCtrl->memBaseAddr + 0x1a) = (UINT8) destId;
		FCAN_TXLEN_REG(pDrvCtrl->memBaseAddr + 0x1a) = (UINT8) length;
	
		dataBuf = (UINT8 *)FCAN_TXBUF_REG(pDrvCtrl->memBaseAddr + 0x1a);
		for(i=0; i<length; i++)
		{
			dataBuf[i] = pChar[i]; 
		}
		//clear busy bit
		FCAN_STATUS_REG(pDrvCtrl->memBaseAddr + 0x1a) &= (UINT8)(~(0x2));
		
		//set data frame enable
		FCAN_STATUS_REG(pDrvCtrl->memBaseAddr + 0x1a) |= (UINT8)(0x1);
		
		//send int to mcu
		FCAN_TO_MCU_INT_REG(pDrvCtrl->regBase) = 1;
		break;
	default:
		printf("channel error,please check it!\n");
		return ERROR;
		break;
	}
	return OK;
}


/**********************************************/

/*BSP读取数据接口函数*/
STATUS bspCanRead(UINT8 channel,UINT8 *pChar,int len)
{

	FCAN_DRV_CTRL *pDrvCtrl;
	VXB_DEVICE_ID pDev = NULL;
	UINT32 i = 0,length = 0;

	pDev=(VXB_DEVICE_ID)vxbInstByNameFind("ppcFpgaCan",0);
	
	pDrvCtrl = pDev->pDrvCtrl;
	if(channel > 1)
	{
		printf("Channel error,please check it!\n");
		return ERROR;
	}
	
	if(pDrvCtrl->rcvDataFrame[channel].dataEnable == 1)
	{
		pChar[0] = (char)pDrvCtrl->rcvDataFrame[channel].ID;
		pChar[1] = (char)pDrvCtrl->rcvDataFrame[channel].len;
		if(len > pDrvCtrl->rcvDataFrame[channel].len)
		{
			length = pDrvCtrl->rcvDataFrame[channel].len;
		}
		else
		{
			length = len;
		}
		for(i=0; i<length; i++)
		{
			pChar[i+2] = pDrvCtrl->rcvDataFrame[channel].data[i];
			//printf("%d \n",pDrvCtrl->rcvDataFrame[channel].data[i]);
			//logMsg("%d %d \n",pDrvCtrl->rcvDataFrame[1].data[i],pChar[i+2],2,3,4,5);
		}
		//printf("%d %d\n",pChar[2],pChar[3]);
		return OK;
	}
	else
	{
		return ERROR;
	}
}

/*BSP初始化CAN 接口函数*/
STATUS bspCanInit(UINT8 channel,UINT8 devID,UINT8 baudrate)
{
	FCAN_DRV_CTRL *pDrvCtrl;
	VXB_DEVICE_ID pDev = NULL;

	pDev=(VXB_DEVICE_ID)vxbInstByNameFind("ppcFpgaCan",0);
	
	pDrvCtrl = pDev->pDrvCtrl;
	
	//set can int enable bit
	FCAN_MCU_INT_ENABLE(pDrvCtrl->regBase) = 0;
	
	switch(channel)
	{
	case 0:
		FCAN_BAUDRATE_REG(pDrvCtrl->memBaseAddr) = baudrate;
		FCAN_ID_REG(pDrvCtrl->memBaseAddr) = devID;
		//set baudrate enable bit
		FCAN_STATUS_REG(pDrvCtrl->memBaseAddr) |= (UINT8)0x10;
//		FCAN_STATUS_REG(pDrvCtrl->memBaseAddr) = (UINT8)0x10;
		//printf("set 0x%x to 0x%x\n",pDrvCtrl->memBaseAddr,0x10);
//		*(UINT8 *)0xe1000100 = (UINT8*)0x10;
//		*(UINT8 *)0xe1000101 = (UINT8*)0x10;
		//send int to mcu
		FCAN_TO_MCU_INT_REG(pDrvCtrl->regBase) = 1;
		break;
	case 1:
		FCAN_BAUDRATE_REG(pDrvCtrl->memBaseAddr + 0x1a) = baudrate;
		FCAN_ID_REG(pDrvCtrl->memBaseAddr + 0x1a) = devID;
		//set baudrate enable bit
		FCAN_STATUS_REG(pDrvCtrl->memBaseAddr + 0x1a) |= 0x10;
		//send int to mcu
		FCAN_TO_MCU_INT_REG(pDrvCtrl->regBase) = 1;
		break;
	default:
		return ERROR;
		break;
	}
	return OK;

}
