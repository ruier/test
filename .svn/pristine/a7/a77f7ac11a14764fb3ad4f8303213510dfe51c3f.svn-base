#include "blkInc.h"

extern BLK_CTRL_S  gstBlkCtrl;

/*void raidSendReqTest(UINT32 uiClu,UINT32 uiAddr,UINT32 uiByte,UINT32 uiOper)*/
void raidSendReqTest()
{
    FS_REQ_S  stReq;
    UINT32    uiLoop = 0;
    
    memset(&stReq,0,sizeof(FS_REQ_S));
    stReq.uiClu = 1;
    stReq.uiOffset = 0;
    stReq.uiBytes = 132;
    stReq.uiDevId = 0;
    stReq.uiOper = 2;
    stReq.pucBuf = (UINT8*)0x18000000;

    gstBlkCtrl.astDev[0].uiPhyNum = 0;
    gstBlkCtrl.astDev[0].uiDevType = BLK_TYPE_DISK;/*BLK_TYPE_RAID*/
    gstBlkCtrl.astDev[0].stRaid.uiStripSec = BLK_STRIPE_SIZE / 512;
    gstBlkCtrl.astDev[0].stRaid.uiMemNum = 1;
    gstBlkCtrl.astDev[0].stRaid.aucMem[0] = 0;
    /*gstBlkCtrl.astDev[8].stRaid.aucMem[1] = 1;
    gstBlkCtrl.astDev[8].stRaid.aucMem[2] = 2;
    gstBlkCtrl.astDev[8].stRaid.aucMem[3] = 0;*/
    for(uiLoop = 0;uiLoop < 1024;uiLoop++)
    {
        raidSendReq(&stReq);
    }
    
}

void blkReqTest()
{
    UINT32 uiLoop = 0;
    
    BLK_REQ_S *pstReq = NULL;

    pstReq = blkGetReq();
    
    pstReq->uiByte = 132;
    pstReq->uiDevId = 0;
    pstReq->uiPhyId = 0;
    pstReq->uiSecNum = 1;
    pstReq->uiSecOff = 0;
    pstReq->uiSecLo = 1;
    pstReq->uiSecHi = 0;
    pstReq->pucBuf = 0x18000000;
    pstReq->uiOper = 2;

    blkWriteByte(pstReq);
}


void raidAdd()
{
    gstBlkCtrl.astDev[0].uiDevType = BLK_TYPE_RAID;
    gstBlkCtrl.astDev[0].stRaid.uiStripSec = BLK_STRIPE_SIZE / 512;
    gstBlkCtrl.astDev[0].stRaid.uiMemNum = 2;
    gstBlkCtrl.astDev[0].stRaid.aucMem[0] = 2;
    gstBlkCtrl.astDev[0].stRaid.aucMem[1] = 1;
    gstBlkCtrl.astDev[0].stRaid.aucMem[2] = 0;
    gstBlkCtrl.astDev[0].stRaid.aucMem[3] = 0;
}


STATUS blkTask(UINT32  uiPhyId,UINT32 uiAddr,UINT32 uiSecNum,UINT32 uiOper)
{
    BLK_REQ_S  *pstReq = NULL;
    UINT8    *pucBuf = NULL;
    UINT32    uiByteNum = 0;
    UINT32    uiWrLen = 0;
    UINT32    uiLoop = 0;
    UINT32    uiClu = 0x18;
    UINT32      beginTimeH = 0;
    UINT32      beginTimeL = 0;
    UINT32      endTimeH = 0;
    UINT32      endTimeL = 0;
    UINT32      tickStart = 0;
    UINT32      tickEnd = 0;
    UINT32      tickSpend = 0;
    float   timeSpend = 0;

    uiByteNum = 512 * uiSecNum;
#if 0    
    pucBuf = malloc((uiByteNum));
    if(NULL == pucBuf)
    {
        FS_LOG(FS_DBG_ERR,"malloc buffer failed\n",0,0,0,0,0,0);
        return ERROR;
    }
    bzero((CHAR*)pucBuf,uiByteNum);
    for(uiLoop = 0;uiLoop < (1*1024*1024);uiLoop++)
    {
        pucBuf[uiLoop] = rand();/**/
    }
#endif    
    printf("test begin\n");
    pstReq = blkGetReq();
    if(NULL == pstReq)
    {
        return ERROR;
    }
    pstReq->uiSecHi = 0;
    pstReq->uiOper = uiOper;
    pstReq->pucBuf = uiAddr;
    pstReq->uiByte = 512 * uiSecNum;
    pstReq->uiSecOff = 0;
    pstReq->uiSecNum = uiSecNum;
    pstReq->uiPhyId = uiPhyId;
    pstReq->uiSecLo = 0x1000;
    
    tickStart = tickGet();
    vxTimeBaseGet(&beginTimeH,&beginTimeL);
    for(uiLoop = 0;uiLoop < 1;uiLoop++)
    {
        if(OK != blkRWSector(pstReq))
        {
            return ERROR;
        }
        semTake(pstReq->stReqSem,WAIT_FOREVER);
    }
    vxTimeBaseGet(&endTimeH,&endTimeL);
    tickEnd = tickGet();
    tickSpend = (tickEnd-tickStart);
#if 0
    free(pucBuf);
    pucBuf = NULL;
    
    timeSpend = (endTimeH -beginTimeH )*4*1024*1024*1024 + (endTimeL-beginTimeL);
    timeSpend = timeSpend*8*2/1000000000;
    

    printf("dma write speed %dMB/s %ds ok\n",(1024*4* 60)/(tickSpend),
           (tickSpend/60));
    printf("dma write speed %fMB/s %f ok\n",(1*4096)/timeSpend,timeSpend);
#endif
	return 0;
}

UINT32 guiBlkSec = 0;

STATUS blkSec(UINT32  uiPhyId,UINT32 uiAddr,UINT32 uiSecNum,UINT32 uiOper)
{
    BLK_REQ_S  *pstReq = NULL;
    UINT32      uiLoop = 0;

    printf("test begin\n");
    pstReq = blkGetReq();
    if(NULL == pstReq)
    {
        return ERROR;
    }
    pstReq->uiSecHi = 0;
    pstReq->uiOper = uiOper;
    pstReq->pucBuf = uiAddr;
    pstReq->uiByte = 512 * uiSecNum;
    pstReq->uiSecOff = 0;
    pstReq->uiSecNum = uiSecNum;
    pstReq->uiPhyId = uiPhyId;
    pstReq->uiSecLo = 0x100000;
   
    /*while(guiBlkSec)*/
    {
        if(OK != blkRWSector(pstReq))
        {
            return ERROR;
        }
        semTake(pstReq->stReqSem,WAIT_FOREVER);
        uiLoop++;
        if(0 == (uiLoop % 1024))
        {
			printf("test is running\n");
        }
        taskDelay(1);
    }
}

void pcieRegTask(void)
{
    UINT32 uiVal = 0;
    
    while(1)
    {
        taskDelay(1);
        uiVal = FSL_READ_REG(0x40100400);
    }
}

#if 0
{
    
    UINT32      uiLoop = 0;
    UINT32      tickStart = 0;
    UINT32      tickEnd = 0;
    UINT32      tickSpend = 0;
    UINT32      tickLast = 0;
    UINT32      tickCur = 0;
    UINT8       *pcBuf = NULL;

    pstReq = blkGetReq();
    if(NULL == pstReq)
    {
        return ERROR;
    }
    pstReq->uiSecHi = 0;
    pstReq->uiOper = uiOper;
    pstReq->pucBuf = (UINT8*)uiAddr;
    pstReq->uiByte = 512 * uiSecNum;
    pstReq->uiSecOff = 0;
    pstReq->uiSecNum = uiSecNum;
    pstReq->uiPhyId = uiPhyId;
    pstReq->uiSecLo = 0;
    printf("start test phy %d addr 0x%x oper %d\n",uiPhyId,uiAddr,uiOper);
    pcBuf = (UINT8*)uiAddr;
    for(uiLoop = 0;uiLoop < 512 * uiSecNum;uiLoop++)
    {
         pcBuf[uiLoop] = rand();
    }
    
    tickStart = tickGet();
    tickCur = tickStart;
    for(uiLoop = 0;uiLoop < 1024*24;uiLoop++)
    {
        pstReq->uiSecLo = pstReq->uiSecLo + uiSecNum;
        /*if((pstReq->uiSecLo + uiSecNum) >= 0x0020facf)
        {
            printf("test disk over sec 0x%x\n",pstReq->uiSecLo);
            break;
        }*/
        if(OK != blkRWSector(pstReq))
        {
            return ERROR;
        }
        
        if(0 == (uiLoop % 260))
        {
            tickLast = tickGet();
            printf("testing disk sec 0x%x tick %d speed %dMB/s\n",pstReq->uiSecLo,
                    (tickLast - tickCur),(260 * 60)/(tickLast - tickCur));
            tickCur = tickLast;
        }
        semTake(pstReq->stReqSem,WAIT_FOREVER);
    }
    tickEnd = tickGet();
    tickSpend = (tickEnd-tickStart);

    printf("dma write speed %dMB/s %ds ok\n",(1024*24* 60)/(tickSpend),
           (tickSpend/60));
    
    return OK;
}
#endif

STATUS blk3Task(UINT32 uiAddr,UINT32 uiOper,UINT32 vuiTestTime)
{
    BLK_REQ_S  *apstReq[3] = NULL;
    UINT32      uiLoop = 0;
    UINT32      uiReq = 0;
    UINT32      tickStart = 0;
    UINT32      tickEnd = 0;
    UINT32      tickSpend = 0;
    UINT32      uiMinSpeed = 0xFFFF;
    UINT32      uiMaxSpeed = 0;
    UINT32      uiTimes = 0;
    UINT32      tickSpeed = 0;
    UINT32      uiLen = 0;

    for(uiLoop = 0;uiLoop < 3;uiLoop++)
    {   
        apstReq[uiLoop] = blkGetReq();
        if(NULL == apstReq[uiLoop])
        {
            return ERROR;
        }
        apstReq[uiLoop]->uiSecHi = 0;
        apstReq[uiLoop]->uiOper = uiOper;
        apstReq[uiLoop]->pucBuf = (UINT8*)uiAddr;
        apstReq[uiLoop]->uiByte = 1024 * 1024;
        apstReq[uiLoop]->uiSecOff = 0;
        apstReq[uiLoop]->uiSecNum = 2048;
        apstReq[uiLoop]->uiPhyId = uiLoop;
        apstReq[uiLoop]->uiSecLo = 1024;

    }
    
    tickStart = tickGet();
    while(uiTimes < vuiTestTime)
    {
        for(uiReq = 0;uiReq < 3;uiReq++)
        {
            apstReq[uiReq]->uiSecLo = apstReq[uiReq]->uiSecLo + 2048;
            if(OK != blkRWSector(apstReq[uiReq]))
            {
                return ERROR;
            }
        }
        
        semTake(apstReq[0]->stReqSem,WAIT_FOREVER);
        semTake(apstReq[1]->stReqSem,WAIT_FOREVER);
        semTake(apstReq[2]->stReqSem,WAIT_FOREVER);

        uiLen = 3 + uiLen;		
        tickEnd = tickGet();
        if((tickEnd-tickStart) > 60)
        {
        	tickSpeed = (uiLen * 60)/(tickEnd-tickStart);
        	uiMinSpeed = MIN(uiMinSpeed,tickSpeed);
            uiMaxSpeed = MAX(uiMaxSpeed,tickSpeed);
			printf("--tick dma write speed %dMB/s %d tick ok\n",tickSpeed,
                    (tickEnd-tickStart));
            tickStart = tickGet();
            uiLen = 0;
            uiTimes++;
        }
        
    }
    
    printf("test time %dS min speed %dMB/s max speed %dMB/s\n",vuiTestTime,
           uiMinSpeed,uiMaxSpeed);
    
    return OK;
}


void blkTest(UINT32  vuiPhyId,UINT32 uiAddr,UINT32 uiSecNum,UINT32 uiOper,UINT32 vuiTestTime)
{
    BLK_REQ_S  *pstReq = NULL;
    UINT8    *pucBuf = NULL;
    UINT32    uiByteNum = 0;
    UINT32    uiWrLen = 0;
    UINT32    uiLoop = 0;
    UINT32    uiClu = 0x18;
    UINT32    uiMinSpeed = 0xFFFF;
    UINT32    uiMaxSpeed = 0;
    UINT32    uiTimes = 0;
    UINT32    tickStart = 0;
    UINT32    tickEnd = 0;
    UINT32    tickSpeed = 0;
    UINT32    uiLen = 0;
    float     timeSpend = 0;
	UINT32 	  uiSecLo = 0x1000;

	if(0 == vuiTestTime)
	{
		vuiTestTime = 0;
	}
    uiByteNum = 512 * uiSecNum;
    printf("test begin\n");
    pstReq = blkGetReq();
    if(NULL == pstReq)
    {
        return ERROR;
    }
    pstReq->uiSecHi = 0;
    pstReq->uiOper = uiOper;
    pstReq->pucBuf = uiAddr;
    pstReq->uiByte = 512 * uiSecNum;
    pstReq->uiSecOff = 0;
    pstReq->uiSecNum = uiSecNum;
    pstReq->uiPhyId = vuiPhyId;
    pstReq->uiSecLo = uiSecLo;
    uiSecLo = uiSecLo + uiSecNum;
	for(uiLoop = 0;uiLoop < 512 * uiSecNum;uiLoop++)
    {
         pstReq->pucBuf[uiLoop] = rand();
    }
    tickStart = tickGet();
    while(uiTimes < vuiTestTime)
    {
        if(OK != blkRWSector(pstReq))
        {
            return ERROR;
        }
        semTake(pstReq->stReqSem,WAIT_FOREVER);
		uiLen = (512 * uiSecNum) + uiLen;		
        tickEnd = tickGet();
        if((tickEnd-tickStart) > 60)
        {
			uiLen = uiLen/0x100000;
        	tickSpeed = (uiLen * 60)/(tickEnd-tickStart);
        	uiMinSpeed = MIN(uiMinSpeed,tickSpeed);
            uiMaxSpeed = MAX(uiMaxSpeed,tickSpeed);
			printf("--tick dma write speed %dMB/s %d tick ok\n",tickSpeed,
                    (tickEnd-tickStart));
            tickStart = tickGet();
            uiLen = 0;
            uiTimes++;
        }
		uiSecLo = uiSecLo + uiSecNum;
		if(uiSecLo > 0x3B000000)
		{
			uiSecLo = 0x1000;
			logMsg("seclo 0x%x \n",uiSecLo,0,0,0,0,0);
		}
		pstReq->uiSecLo = uiSecLo;
    }
    printf("test time %dS min speed %dMB/s max speed %dMB/s\n",vuiTestTime,
           uiMinSpeed,uiMaxSpeed);
    
    
    return 0;
}
    

#if 0
{
    taskSpawn("task1",100,0,0x10000,(FUNCPTR)blkSec,0,uiAddr,uiOper,0,0,0,0,0,0,0);
    taskSpawn("task2",80,0,0x10000,(FUNCPTR)pcieRegTask,0,0,0,0,0,0,0,0,0,0);
    /*taskSpawn("task0",100,0,0x10000,(FUNCPTR)blkReqTest,0,0,0,0,0,0,0,0,0,0);
    taskSpawn("task1",110,0,0x10000,(FUNCPTR)blkReqTest,0,0,0,0,0,0,0,0,0,0);*/
}
#endif

