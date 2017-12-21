#include <stdio.h>
#include <taskLibCommon.h>


#include <hwif/util/vxbParamSys.h>

#include "canDrv.h"





static fCanBufCtrl	fCanRcvBuf[FCAN_CHANNEL_NUM][FCAN_RCV_BUF_NO];
static fCanManagCtrl fCanSendManage[FCAN_CHANNEL_NUM];


static VXB_DEVICE_ID bspFcanDev[2]={NULL,NULL};




static const UINT32 libCanRcvBuf[2][3]=
{
	{(UINT32)fCanRcvBuf[0],sizeof(fCanBufCtrl),FCAN_RCV_BUF_NO},
	{(UINT32)fCanRcvBuf[1],sizeof(fCanBufCtrl),FCAN_RCV_BUF_NO},
};

/**************************************************************/


LOCAL STATUS fCanBufInit(UINT8 channel)
{
	int i;
	
	if(channel>=FCAN_CHANNEL_NUM)	
		return ERROR;

	for(i=0;i<FCAN_RCV_BUF_NO;i++)
	{
		fCanRcvBuf[channel][i].bufNo = i;
		fCanRcvBuf[channel][i].useFlag = 0;
		bzero((void *)&fCanRcvBuf[channel][i].fCanData,sizeof(fCanRcvBuf[channel][i].fCanData));
		if(i<(FCAN_RCV_BUF_NO-1))
			fCanRcvBuf[channel][i].naxt = &fCanRcvBuf[channel][i+1];
		else if(i==(FCAN_RCV_BUF_NO-1))
			fCanRcvBuf[channel][i].naxt = &fCanRcvBuf[channel][0];
	}
	
	fCanSendManage[channel].canSendSem = semBCreate(SEM_Q_FIFO, SEM_FULL);
	if(fCanSendManage[channel].canSendSem==NULL)	
		return ERROR;
	fCanSendManage[channel].canReadSem = semBCreate(SEM_Q_FIFO, SEM_FULL);
	if(fCanSendManage[channel].canReadSem==NULL)
		return ERROR;
	
	return OK;
}




LOCAL STATUS fCanHardWareInit(UINT8 channel)
{
	VXB_DEVICE_ID pDev1;
	VXB_DEVICE_ID pDev2;
	FUNCPTR     handler;
	
	
	pDev1 = vxbInstByNameFind(FCAN_DRV_NAME,channel);	
	handler=vxbDevMethodGet(pDev1, DEVMETHOD_CALL(vxbCanCallbackInstall));
	if(( handler == NULL )||(pDev1 == NULL))
	{
		BSP_CANDRV_TEST("fCanHardWareInit: pDev1 vxbDevMethodGet != OK!!\n",1,2,3,4,5,6);
		return ERROR;
	}
	
	pDev2 = vxbInstByNameFind(FCAN_DRV_NAME,channel);
	handler=vxbDevMethodGet(pDev2, DEVMETHOD_CALL(vxbCanIoctl));
	if(( handler == NULL )||(pDev2 == NULL))
	{
		BSP_CANDRV_TEST("fCanHardWareInit: pDev2 vxbDevMethodGet != OK!!\n",1,2,3,4,5,6);
		return ERROR;
	}
	handler(pDev2,CMD_SETUP_SEND_DISENABLE,0);
	handler(pDev2,CMD_SETUP_RCV_BUF_START_ADDR,libCanRcvBuf[channel][0]);
	handler(pDev2,CMD_SETUP_RCV_BUF_SIZE,libCanRcvBuf[channel][1]);
	handler(pDev2,CMD_SETUP_RCV_BUF_COUNT,libCanRcvBuf[channel][2]);
	handler(pDev2,CMD_SETUP_SEND_ENABLE,0);
	
	BSP_CANDRV_TEST(" bufStartAdd: 0x%x, bufSize: %d, bufCount: %d\n",\
		libCanRcvBuf[channel][0],libCanRcvBuf[channel][1],libCanRcvBuf[channel][2],3,4,5);
	
	if(pDev2 == pDev1)
		bspFcanDev[channel] = pDev2;
	
	return OK;
}


STATUS bspFCanSpawnInit(void)
{
	int i,temp=OK;
	
	for(i=0;i<FCAN_CHANNEL_NUM;i++)
	{	
		temp = fCanBufInit(i);
		if(temp != OK)
			continue;

		temp = fCanHardWareInit(i);
		if(temp != OK)	
			continue;
	}
	
	return OK;
}




LOCAL STATUS fCanSend(int channel, fCanSendRcvBD *pChar, UINT8 mode)
{
	FUNCPTR	handler;
	int temp;
	
//	BSP_CANDRV_DBG_MSG("fCanSend: channel=%d, pChar->0x%x, mode=%d\n",channel,(int)pChar,mode,3,4,5);
	
	if(bspFcanDev[channel]==NULL)
		bspFcanDev[channel] = vxbInstByNameFind(FCAN_DRV_NAME,channel);
	handler=vxbDevMethodGet(bspFcanDev[channel], DEVMETHOD_CALL(vxbCanSendStartUp));
	if( handler == NULL )
	{
		BSP_CANDRV_DBG_MSG("fCanSend: vxbDevMethodGet != OK!!\n",1,2,3,4,5,6);
		return (-3);
	}
	
	temp = (*handler)(bspFcanDev[channel],pChar,mode);
	if(temp != OK)
	{	
		BSP_CANDRV_DBG_MSG("fCanSend: return 0x%x!!, pChar->0x%x\n",temp,(int)pChar,3,4,5,6);
	}	

	return temp;
}



/**********************************************************************/

/*数据采样设置*/
STATUS bspCanSampleBitSet(UINT8 channel,UINT8 clkDiv,UINT8 sjw,UINT8 ts2,UINT8 ts1)
{
	FUNCPTR     handler;
	int temp;
	
	if(channel<FCAN_CHANNEL_NUM)
	{
		if(bspFcanDev[channel]==NULL)
			bspFcanDev[channel] = vxbInstByNameFind(FCAN_DRV_NAME,channel);
		handler=vxbDevMethodGet(bspFcanDev[channel], DEVMETHOD_CALL(vxbCanIoctl));
	
		if( handler == NULL )
		{
			BSP_CANDRV_DBG_MSG("bspCanSampleBitSet: vxbDevMethodGet != OK!!\n",1,2,3,4,5,6);
			return ERROR;
		}
		
		handler(bspFcanDev[channel],CMD_SETUP_SEND_DISENABLE,0);
		temp = ((sjw&0x03)<<7);
		temp |=((ts2&0x07)<<4);
		temp |=(ts1&0x0f);
		handler(bspFcanDev[channel],CMD_SETUP_SAMPLE_TQ,clkDiv);
		handler(bspFcanDev[channel],CMD_SETUP_BIT_TIME,temp);
		handler(bspFcanDev[channel],CMD_SETUP_SEND_ENABLE,0);
	}	
	else
	{
		BSP_CANDRV_DBG_MSG("bspCanSampleBitSet error, channel=%d!\n",channel,1,2,3,4,5);
		return ERROR;
	}
	
	return OK;
}


/* 数据接受过滤ID设置 */
STATUS bspCanFilterIdSet(UINT8 channel,UINT8 chan,UINT8 mode,UINT32 devId)
{
	FCAN_DRV_CTRL *pDrvCtrl;
	
	if(channel>=FCAN_CHANNEL_NUM)	return ERROR;		
	if(bspFcanDev[channel]==NULL)
		bspFcanDev[channel] = vxbInstByNameFind(FCAN_DRV_NAME,channel);
	if(bspFcanDev[channel]==NULL)	return ERROR;	
	if(bspFcanDev[channel]->pDrvCtrl==NULL)		return ERROR;
	
	pDrvCtrl = bspFcanDev[channel]->pDrvCtrl;
	
	fCanFilterIdSetup(pDrvCtrl->regBase,chan,mode,devId);
	
	return OK;
}



/*接收数据缓存设置*/
STATUS bspCanRcvAddrSet(UINT8 channel,void* recAddr,int rcvBufNum)
{
	FUNCPTR     handler;
	FCAN_DRV_CTRL *pDrvCtrl;
	fCanBufCtrl *bufPtr;
	int i;
	
	if((rcvBufNum<1)||(rcvBufNum>10000))
	{
		BSP_CANDRV_DBG_MSG("bspCanRcvAddrSet: rcvBufNum = %d!\n",rcvBufNum,2,3,4,5,6);
		return ERROR;
	}	
		
	if(channel<FCAN_CHANNEL_NUM)
	{
		if(bspFcanDev[channel]==NULL)
			bspFcanDev[channel] = vxbInstByNameFind(FCAN_DRV_NAME,channel);
		handler=vxbDevMethodGet(bspFcanDev[channel], DEVMETHOD_CALL(vxbCanIoctl));
		
		if( handler == NULL )
		{
			BSP_CANDRV_DBG_MSG("bspCanRcvAddrSet: vxbDevMethodGet != OK!!\n",1,2,3,4,5,6);
			return ERROR;
		}
		
		for(i=0;i<rcvBufNum;i++)
		{
			bufPtr = (((fCanBufCtrl *)recAddr)+i);	
			bufPtr->bufNo = i;
			bufPtr->useFlag = 0;
			bzero((void *)&bufPtr->fCanData,sizeof(bufPtr->fCanData));
			if(i<(rcvBufNum-1))
				bufPtr->naxt = (bufPtr+1);
			else if(i==(rcvBufNum-1))
				bufPtr->naxt = (fCanBufCtrl *)recAddr;
		}
		pDrvCtrl = bspFcanDev[channel]->pDrvCtrl;
		if(pDrvCtrl==NULL)	
		{
			BSP_CANDRV_DBG_MSG("bspCanRcvAddrSet ERROR: pDrvCtrl = NULL!\n",1,2,3,4,5,6);
			return ERROR;
		}	
		
		handler(bspFcanDev[channel],CMD_SETUP_SEND_DISENABLE,0);	
		pDrvCtrl->rcvScptr = 0;
		pDrvCtrl->readScptr = 0;
		pDrvCtrl->readPingP = 0;
		handler(bspFcanDev[channel],CMD_SETUP_RCV_BUF_START_ADDR,recAddr);
		handler(bspFcanDev[channel],CMD_SETUP_RCV_BUF_COUNT,rcvBufNum);
		handler(bspFcanDev[channel],CMD_SETUP_RCV_BUF_SIZE,sizeof(fCanBufCtrl));
		handler(bspFcanDev[channel],CMD_SETUP_SEND_ENABLE,0);
	}	
	else
	{
		BSP_CANDRV_DBG_MSG("bspCanRcvAddrSet error, channel=%d!\n",channel,1,2,3,4,5);
		return ERROR;
	}	
	
	return OK;
}


/*注册 中断通知回调函数*/
STATUS bspCanCallbackSet(UINT8 channel,FUNCPTR sendCallBack,FUNCPTR rcvCallBack)
{
	FUNCPTR	handler;
	
	if(channel<FCAN_CHANNEL_NUM)
	{	
		if(bspFcanDev[channel]==NULL)
			bspFcanDev[channel] = vxbInstByNameFind(FCAN_DRV_NAME,channel);
		handler=vxbDevMethodGet(bspFcanDev[channel], DEVMETHOD_CALL(vxbCanCallbackInstall));
		if( handler == NULL )
		{
			BSP_CANDRV_DBG_MSG("bspCanCallbackSet: vxbDevMethodGet != OK!!\n",1,2,3,4,5,6);
			return ERROR;
		}	
		handler(bspFcanDev[channel],sendCallBack,rcvCallBack);
	}	
	else 
	{	
		BSP_CANDRV_DBG_MSG("bspCanRcvCallbackSet error, channel=%d!\n",channel,1,2,3,4,5);
		return ERROR;
	}
	
	return OK;
}


/*BSP发送接口函数*/
STATUS bspCanSend(UINT8 channel,UINT32 destId,UINT8 *pChar,int len,UINT8 mode)
{
	fCanSendRcvBD canSendData;
	int val;
	
	if((pChar==NULL)||(len<0)||(channel>=FCAN_CHANNEL_NUM)||len>FCAN_RCV_MAX_LEN)
	{
		logMsg("bspCanSend parameter error, channel = %d, pChar->0x%x, len=%d!\n",\
				channel,(int)pChar,len,3,4,5);
		return (-2);
	}	
	
	if(fCanSendManage[channel].canSendSem!=NULL)
		semTake(fCanSendManage[channel].canSendSem,WAIT_FOREVER);	
	
	canSendData.fCanDlc = FCAN_TXDLC_LEN(len);
	memcpy(&canSendData.fCanDhigh,pChar,len);
	
	if(destId&FCAN_EXTEN_ID)
	{
		canSendData.fCanId = FCAN_EXTEN_IDH_MASK(destId);
		canSendData.fCanId |= (FCAN_TXRXID_SRTR|FCAN_TXRXID_IDEN);
		canSendData.fCanId |= FCAN_EXTEN_IDL_MASK(destId);
		if(destId&FCAN_REMOTE_MASK)
			canSendData.fCanId |= FCAN_TXRXID_RTR;
	}	
	else
	{
		canSendData.fCanId = FCAN_TXRXID_IDH(destId);
		if(destId&FCAN_REMOTE_MASK)
			canSendData.fCanId |= FCAN_TXRXID_SRTR;
	}	
	logMsg("0x%x-0x%x-0x%x-0x%x\n",canSendData.fCanId,canSendData.fCanDlc,canSendData.fCanDhigh,canSendData.fCanDlow,5,6);
	
	val = fCanSend(channel,&canSendData,mode);
		
	if(fCanSendManage[channel].canSendSem != NULL)
		semGive(fCanSendManage[channel].canSendSem);
	
	BSP_CANDRV_DBG_MSG("bspCanSend complete, canSendData->0x%x, return 0x%x\n", \
			(int)&canSendData,val,2,3,4,5);

	return val;
}


/*BSP读取数据接口函数*/
STATUS bspCanRead(UINT8 channel,UINT32 *pChar,int len)
{
	fCanSendRcvBD canReadData;
	FUNCPTR	handler;
	int temp;
	UINT32 val;
	

	if((pChar==NULL)||(len<FCAN_RCV_HAND_LEN)||(channel>FCAN_CHANNEL_NUM))
	{
		logMsg("bspCanRead parameter error, channel = %d, pChar->0x%x, len=%d!\n",channel,(int)pChar,len,3,4,5);
		return (-2);
	}	
	
	if(fCanSendManage[channel].canReadSem != NULL)
		semTake(fCanSendManage[channel].canReadSem, WAIT_FOREVER);

	if(bspFcanDev[channel]==NULL)
		bspFcanDev[channel] = vxbInstByNameFind(FCAN_DRV_NAME,channel);
	handler = vxbDevMethodGet(bspFcanDev[channel], DEVMETHOD_CALL(vxbCanRcvRead));
	if( handler == NULL )
	{
		BSP_CANDRV_DBG_MSG("bspCanRead: vxbDevMethodGet != OK!!\n",1,2,3,4,5,6);
		return (-3);
	}
	
	temp = (*handler)(bspFcanDev[channel],&canReadData);	
	if(temp==OK)
	{
		logMsg("temp =oK,id=0x%x,fCanDlc=0x%x\n",canReadData.fCanId,canReadData.fCanDlc,3,4,5,6);
		if(canReadData.fCanId&FCAN_TXRXID_IDEN)
		{
			val = FCAN_EXTEN_IDH_D(canReadData.fCanId);
			val |= FCAN_EXTEN_IDL_D(canReadData.fCanId);
			if(canReadData.fCanId&FCAN_TXRXID_RTR)
				val |= FCAN_REMOTE_MASK;
			pChar[0] = (FCAN_EXTEN_ID|val);	
		}	
		else
		{
			val = FCAN_STANDARD_ID_D(canReadData.fCanId);
			if(canReadData.fCanId&FCAN_TXRXID_SRTR)
				val |= FCAN_REMOTE_MASK;
			pChar[0] = val;
		}
		
		pChar[1] = canReadData.fCanDlc;
		if(pChar[1]<=FCAN_RCV_MAX_LEN)
		{
			val = ((pChar[1]+7)/4);
			if(len > val)
				memcpy(&pChar[2],&canReadData.fCanDhigh,pChar[1]);
			else if(len>FCAN_RCV_HAND_LEN)
				memcpy(&pChar[2],&canReadData.fCanDhigh,4);
			temp = pChar[1];
		}
		else BSP_CANDRV_DBG_MSG("bspCanRead error: len = %d !\n",pChar[1],1,2,3,4,5);
	}
#if 0
	if(temp<0)
	{	
		BSP_CANDRV_DBG_MSG("bspCanRead: return 0x%x!!, pChar->0x%x\n",temp,(int)pChar,3,4,5,6);
	}
#endif		
	if(fCanSendManage[channel].canReadSem != NULL)
		semGive(fCanSendManage[channel].canReadSem);
	
	return temp;
}

