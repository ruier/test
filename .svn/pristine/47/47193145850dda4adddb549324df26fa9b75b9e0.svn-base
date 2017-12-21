#include <stdio.h>
#include <string.h>

#include "canDrv.h"

//static fCanBufCtrl	fCanSendBuf[FCAN_CHANNEL_NUM][FCAN_SEND_BUF_NO];
static fCanBufCtrl	fCanReceiveBuf[FCAN_CHANNEL_NUM][FCAN_RCV_BUF_NO];

static SEM_ID sendCompleteSem[FCAN_CHANNEL_NUM];
static SEM_ID rcvFrameSem[FCAN_CHANNEL_NUM];



/************************************************/

LOCAL STATUS sendCompleteCallBack(UINT8 channel)
{	
	if(channel<FCAN_CHANNEL_NUM)
	{	
		semGive(sendCompleteSem[channel]);
		logMsg("sendCompleteCallBack: channel = %d\n",channel,1,2,3,4,5);
	}	
	return OK;
}

LOCAL STATUS rcvFrameCallBack(UINT8 channel)
{
	if(channel<FCAN_CHANNEL_NUM)
	{	
		semGive(rcvFrameSem[channel]);
		logMsg("rcvFrameCallBack: channel = %d\n",channel,1,2,3,4,5);
	}
	return OK;
}


STATUS bspCanTestInit(void)
{
	int i,temp=OK;

	for(i=0;i<FCAN_CHANNEL_NUM;i++)
	{	
		sendCompleteSem[i] = semBCreate (SEM_Q_FIFO, SEM_EMPTY);
		if(sendCompleteSem[i] ==NULL ) return ERROR;
		rcvFrameSem[i] = semCCreate (SEM_Q_FIFO, SEM_EMPTY);	
		if(rcvFrameSem[i] ==NULL ) return ERROR;
	
		temp |= bspCanCallbackSet(i,sendCompleteCallBack,rcvFrameCallBack);
	}
	logMsg("\nbspCanTestInit: temp = 0x%x\n",temp,1,2,3,4,5);
	
	return temp;
}


STATUS bspCanAddrTest(UINT8 channel)
{
	int temp;
	
	if(channel<FCAN_CHANNEL_NUM)
	{	
		temp = bspCanRcvAddrSet(channel,fCanReceiveBuf[channel],FCAN_RCV_BUF_NO);
		logMsg("bspCanAddrTest: setSddr = 0x%x, temp = 0x%x\n",(UINT32)fCanReceiveBuf[channel],temp,2,3,4,5);
	}
	else	temp=ERROR;
	
	return temp;
}
