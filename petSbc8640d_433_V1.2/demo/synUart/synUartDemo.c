
/*************************Í·ÎÄ¼þ************************/
#include "vxWorks.h"
#include "bspBridgeDma8641.h"
#include "stdio.h"
#include "math.h"
#include "semLib.h"
#include "taskLib.h"
#include "msgQLib.h"
#include <logLib.h>	
#include <cpuset.h>


#define SYN_UART_DEMO_DBG_ON
#undef SYN_UART_DEMO_DBG_ON

#ifdef SYN_UART_DEMO_DBG_ON
static char sprintBufSynUart[100];
#define DEMO_SYN_UART_DBG_MSG(X0, X1, X2, X3, X4, X5, X6)	{sprintf(sprintBufSynUart,X0, X1, X2, X3, X4, X5, X6);ns16550Pollprintf(sprintBufSynUart);}
#define STATIC 

#else /* SYN_UART_DEMO_DBG_ON */
#define STATIC static
#define DEMO_SYN_UART_DBG_MSG(fmt,a,b,c,d,e,f)
#endif /* SYN_UART_DEMO_DBG_ON */

extern STATUS bspSynUartIntConnect(UINT32 synUartNum,VOIDFUNCPTR routine,UINT32 index);
extern STATUS bspSynUartRcv(UINT32 synUartNum,UINT16 *data,UINT32 *size);
extern STATUS bspSynUartSend(UINT32 synUartNum,UINT16 *data,UINT32 size);

static SEM_ID gBspSynUartRcvSem[SYN_UART_CHAIN_NUM];
UINT8 gRcvStop=0,gSendStop=0;

STATIC void demoSynUartRcvIsr(UINT32 synUartNum)
{
	semGive(gBspSynUartRcvSem[synUartNum]);
	//logMsg("enter demoSynUartRcvIsr\n",1,2,3,4,5,6);
}

STATIC STATUS demoSynUartRcvInit(UINT32 synUartNum)
{
	gBspSynUartRcvSem[synUartNum] = semBCreate (SEM_Q_FIFO, SEM_EMPTY);
	//printf("in here00!\n");
	if(bspSynUartSwitchCPU(synUartNum,sysGetCpuID())==ERROR) return ERROR;
	if(bspSynUartIntDisable(synUartNum,0)==ERROR) return ERROR;
	//printf("in here11!\n");
	if(bspSynUartChainEnable(synUartNum) == ERROR) return ERROR;
	//printf("in here22!\n");
	if(bspSynUartIntConnect(synUartNum,demoSynUartRcvIsr,0) == ERROR) return ERROR;
	//printf("in here33!\n");
	if(bspSynUartIntEnable(synUartNum,0)==ERROR) return ERROR;
	//printf("in here44!\n");
	return OK;	
}

STATUS demoSynUartRcv(UINT32 synUartNum)
{
	UINT32 rcvAddr=0x20000000,rcvSize=0,i;
	UINT16 data[6];
	//printf("in here---!\n");
	taskDelay(30);
	demoSynUartRcvInit(synUartNum);
	//printf("in here!\n");
	while(!gRcvStop)
	{
		//printf("in here55!\n");
		semTake(gBspSynUartRcvSem[synUartNum],WAIT_FOREVER);
		//printf("take sem!\n");
		if(bspSynUartRcv(synUartNum,data,&rcvSize)==ERROR)
		{
			printf("read error!\n");
			return ERROR;
		}
		printf("size %d\n",rcvSize);
		for(i=0;i<rcvSize/2;i++) printf("%d:0x%x\n",i,data[i]);
	}
	return OK;	
}


STATUS demoSynUartSend(UINT32 synUartNum)
{
	UINT16 data[6]={0x55aa,0x0,0xf0f0,0xaa55,0x0f0f,0xf0f0};
	bspSynUartChainEnable(synUartNum);
	while(!gSendStop) 	
	{
		if(bspSynUartSend(synUartNum,data,12)==ERROR) return ERROR;
		taskDelay(20);
	}
	return OK;
}

STATUS demoSynUartSendS(UINT32 synUartNum,UINT32 count)
{
	UINT16 data[6]={0x55aa,0xf0f0,0x0,0xaa55,0x0f0f,0xf0f0};
	//data[0] = data1;
	bspSynUartChainEnable(synUartNum);
	//while(!gSendStop) 	
	{
		if(bspSynUartSend(synUartNum,data,count)==ERROR) return ERROR;
		taskDelay(20);
	}
	return OK;
}


STATIC void demoSynUartDataInit(UINT16* srcAddr, UINT32 size)
{
    int i;
    UINT16 *addr=srcAddr;
    for(i=0; i<(size/2); i++)
        *addr++ = i;
    
}

STATUS demoSynUartLoopBack(UINT32 synUartNum,UINT32 sendSize)
{
	UINT32 sendAddr=0x30000000;
	if(demoSynUartRcvInit(synUartNum)==ERROR) return ERROR;
	demoSynUartDataInit((UINT16*)sendAddr,sendSize);
	bspSynUartChainEnable(synUartNum);
	if(bspSynUartSend(synUartNum,(UINT16*)sendAddr,sendSize)==ERROR) return ERROR;
	return OK;
}


