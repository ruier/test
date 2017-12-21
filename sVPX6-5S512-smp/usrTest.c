#ifndef COMPILE_BOOTROM
#include <../h/pthread.h>
INT32 testTei()
{
    netServer(0);
}


INT32 testSrio()
{
    INT32  uiVal = 0;
    INT32  uiSpeed = 0;
    UINT32 uiPortNum = 0;
    
  
    logMsg("srio test start:\n",0,0,0,0,0,0);
    if(OK == rioSysInit(0,0,1))
    {
         rioSystemDisplay();

         uiPortNum = 0x5;
         uiSpeed = rioGetPortSpeed(0,0xff,0,uiPortNum);
         switch(uiSpeed)
         {
             case RIO_PORT_SPEED125:
                    logMsg("PPC->1848 : 1.25 G \n", 0, 0, 0, 0, 0, 0);
             break;
             
             case RIO_PORT_SPEED25:
                    logMsg("PPC->1848 : 2.5 G \n", 0, 0, 0, 0, 0, 0);
             break;
                    
             case RIO_PORT_SPEED3125:
                    logMsg("PPC->1848 : 3.125 G \n", 0, 0, 0, 0, 0, 0);
             break;
                    
             case RIO_PORT_SPEED5:
                    logMsg("PPC->1848 : 5 G \n", 0, 0, 0, 0, 0, 0);
             break;
                   
             case RIO_PORT_SPEED625:
                  logMsg("PPC->1848 : 6.25 G \n", 0, 0, 0, 0, 0, 0);
             break;

             default:
                  logMsg("PPC->1848 : error !\n", 0, 0, 0, 0, 0, 0);   
         }
         
         uiPortNum = 0xa;
         uiSpeed = rioGetPortSpeed(0,0xff,0,uiPortNum);
		
         switch(uiSpeed)
         {
             case RIO_PORT_SPEED125:
                    logMsg("PPC->K7 : 1.25 G \n", 0, 0, 0, 0, 0, 0);
             break;
             
             case RIO_PORT_SPEED25:
                    /*logMsg("PPC->K7 : 2.5 G \n", 0, 0, 0, 0, 0, 0);*/
					logMsg("PPC->K7 : 3.125 G \n", 0, 0, 0, 0, 0, 0);
             break;
                    
             case RIO_PORT_SPEED3125:
                    logMsg("PPC->K7 : 3.125 G \n", 0, 0, 0, 0, 0, 0);
             break;
                    
             case RIO_PORT_SPEED5:
                    logMsg("PPC->K7 : 5 G \n", 0, 0, 0, 0, 0, 0);
             break;
                   
             case RIO_PORT_SPEED625:
                  logMsg("PPC->K7 : 6.25 G \n", 0, 0, 0, 0, 0, 0);
             break;

             default:
                  logMsg("PPC->K7 : error !\n", 0, 0, 0, 0, 0, 0);   
         }
        logMsg("srio test successfully,check data right !\n",0,0,0,0,0,0);
    }
    else
    {
        logMsg("srio test fail !\n",0,0,0,0,0,0);
    }     
}


INT32 testTempAndVolt()
{
    UINT32 uiFlag = 0;
    INT32  reValue = 0;

    logMsg("TempAndVolt test start:\n",0,0,0,0,0,0);
    
    reValue = sysGetCpuTemp();
    logMsg("cpu temp : %d C\n", reValue, 0, 0, 0, 0, 0);
    
    /*reValue = sysGetFPGATemp();
    logMsg("fpga temp : %d C\n", reValue, 0, 0, 0, 0, 0);*/
    
    for(uiFlag = 0; uiFlag < 6; uiFlag++)
    {
        reValue = sysGetVolt(0XA4,uiFlag);
        logMsg("volt%d : %d mv \n", uiFlag, reValue, 0, 0, 0, 0);
    }

    
    return OK;
}


INT32 testDDRandSSD()
{
    INT32 reValue = 0;
    
    P5020_READ_REG(0x7ffffff0);
    logMsg("DDR: 2GB\n",0,0,0,0,0,0);
	reValue = fsGetVolCapTest("/vol");
    if(reValue != 0)
    {
        logMsg("SSD: %d MB\n",reValue,0,0,0,0,0);     
    }
    
}
void cpuTaskPrint(UINT32 uiCpuID)
{
	printf("cpu id %d \n",uiCpuID);
}
STATUS affinitySetExample (UINT32 uiCpuID)
{
    cpuset_t   affinity;
    INT32      tid = 0;
    UINT8      aucBuf[16] = {0};

    sprintf(aucBuf,"%s%d","taskCPU",uiCpuID);
    /* Create the task but only activate it after setting its affinity */
    tid = taskCreate (aucBuf, 100, 0, 5000, cpuTaskPrint,uiCpuID, 0, 0, 0,
                      0, 0, 0, 0, 0, 0);
    if (tid == NULL)
    {
        return (ERROR);
    }
    
    /*
     * Clear the affinity CPU set and set index for CPU 1 
    */
    CPUSET_ZERO(affinity);
    CPUSET_SET(affinity,uiCpuID);
    
    if (taskCpuAffinitySet (tid, affinity) == ERROR)
    {
        /*

         * Either CPUs are not enabled or we are in UP mode 
        */
        taskDelete (tid);
        return (ERROR);
    }
    
    /*
     *Now let the task run on CPU 1
    */
    taskActivate (tid);
    return (OK);
}
STATUS  affinitySet(UINT32 uiCpuID,UINT32 uiTaskID)
{
    cpuset_t   affinity;
	
    CPUSET_ZERO(affinity);
    CPUSET_SET(affinity,uiCpuID);
    if (taskCpuAffinitySet (uiTaskID, affinity) == ERROR)
    {
        /*
         * Either CPUs are not enabled or we are in UP mode 
        */
        taskDelete (uiTaskID);
        return (ERROR);
    }
    
    /*
     *Now let the task run on CPU 1
    */
    taskActivate (uiTaskID);
    return (OK);

}
	
STATUS affinityUnSet(UINT32 uiTaskID)
{
    cpuset_t affinity;
    
    CPUSET_ZERO (affinity);
    return taskCpuAffinitySet (uiTaskID,affinity);
}

STATUS affinityGet(UINT32 uiTaskID)
{
    cpuset_t affinity = 0;
    
    if(OK != taskCpuAffinityGet(uiTaskID,&affinity))
    {
        return ERROR;
    }
    
    if(0x00 == affinity)
    {
        printf("task not binding to cpu %d \n",affinity);
    }
    else if(0x01 == affinity)
    {
        printf("task binding to cpu0 %d \n",affinity);
    }
    else
    {
        printf("task binding to cpu1 %d \n",affinity);
    }
    
    return OK;
}


STATUS dualCorePrintf(VOID)
{
#if 1
    affinitySetExample(0);
    affinitySetExample(1);
    taskDelay(sysClkRateGet());
#else 
	int loop = 0;
	while(loop < 5)
	{
		printf("    this message is print by core 0\n");
		printf("    this message is print by core 1\n");
		loop++;
	}
#endif	
}


void testRTC()
{
    UINT8 uiLoop = 0;
	RTC_DATE_TIME tmpTime;

	tmpTime.year = 16;  
	tmpTime.month = 5;
	tmpTime.date = 24;   
	tmpTime.day = 2;
	tmpTime.hour = 11;
	tmpTime.minute = 26;
	tmpTime.second = 27;
	tmpTime.mSec = 21;


	sysRtcInit();
	taskDelay(10);
	sysRtcSet(&tmpTime);	
	printf("Set RTC Successful \n");

    for(uiLoop = 0; uiLoop < 5; uiLoop++)
    {
        memset(&tmpTime, 0, sizeof(RTC_DATE_TIME));
	    sysRtcGet(&tmpTime);
        printf("get RTC Successful \n");
	    printf(" %02dcentury, 20%02d-%02d-%02d, week%d %02d:%02d:%02d \n", tmpTime.mSec, tmpTime.year, tmpTime.month,
		tmpTime.date, tmpTime.day, tmpTime.hour, tmpTime.minute, tmpTime.second);
		taskDelay(sysClkRateGet()+15);
	}
}


INT32 testWatchDog()
{
    /*初始化看门狗，将喂狗时间设置为5秒*/
    sysWatchDogInit(5000);
    
    logMsg("stop reload watchdog,wait for ppc reset !\n",0,0,0,0,0,0);
}

INT32 ddrTest(char	*pcSrcAddr,char *pcDestAddr,UINT32 num)
{
	UINT32 uiSendLen = 0,startTime = 0;
	
	startTime = tickGet();
	while(1)
	{
		ddrDma(0,pcSrcAddr,pcDestAddr,num * 0x100000);
		uiSendLen += num;
		if ((tickGet()-startTime) > 2*sysClkRateGet())
		{
			logMsg("s speed M/S %d\n",uiSendLen,0,0,0,0,0);
			uiSendLen = 0;
			startTime = tickGet();
		}
	}
	

}
UINT32 recTest(char *name ,UINT32 num,UINT32 addr,UINT32 uiFlag)
{
	UINT32	uiWriteLen = 0;

	UINT32	uiCurTime = 0,endTime = 0;
	UINT32	uiCurAddr = 0;
	UINT32  rmRecSpeed = 0,fd = 0;
	UINT32	writeLen = 0;
	char	filePath[96];
	UINT32	uiCunt = 0;
	UINT32    uiMinSpeed = 0xFFFF;
    UINT32    uiMaxSpeed = 0;
	UINT32    uiAddr = addr;
	VOID     *pvBuf = NULL;

	sprintf(filePath,"/vol/%s",name);
	fd = open(filePath,0x209,0);
	if(fd <= 0)
	{
		printf("open file failed \n");
		return ERROR;
	}
	writeLen = num*0x100000;
	if(0 == uiFlag)
	{
		pvBuf = malloc(writeLen);
		if(NULL == pvBuf)
		{
			printf("malloc buffer failed\n");
			return -1;
		}
		uiAddr =  (UINT32)pvBuf;
	}
	logMsg("file path %s cur addr 0x%x,need write len 0x%x,fd 0x%x \n",
							filePath,uiCurAddr,writeLen,fd,0,0);
	uiCurTime = tickGet();
	printf("start tick %d \n",uiCurTime);
	while(1)
	{
		uiWriteLen= read(fd, (char*)(uiAddr),writeLen);
		if(uiWriteLen <= 0)
		{
			printf("write data error %d \n",uiWriteLen);
			printf("min speed %dMB/s max speed %dMB/s\n",uiMinSpeed,
										uiMaxSpeed);
			close(fd);
			formatvol("/vol");
			
			fd = open(filePath,0x209,0);
			if(fd <= 0)
			{
				printf("open file failed \n");
				return ERROR;
			}
			
			continue;
		}
		rmRecSpeed += uiWriteLen/0x100000;
		endTime = tickGet();
		if((endTime - uiCurTime) > 5*sysClkRateGet())
		{
			rmRecSpeed = rmRecSpeed/5;
			uiMinSpeed = MIN(uiMinSpeed,rmRecSpeed);
            uiMaxSpeed = MAX(uiMaxSpeed,rmRecSpeed);
            printf(" speed %d MB/s\n",rmRecSpeed);
			uiCurTime = endTime;
			rmRecSpeed = 0;
		}
	}
	printf("min speed %dMB/s max speed %dMB/s\n",uiMinSpeed,uiMaxSpeed);
	close(fd);
}

#endif

/*eipc test */
int mh;

static INT32 chandle[8] = {0};

int create_read_port(int memnum,int index,char *name)
{
	int ret;
	ret = eipc_open(name,MASTER,memnum,index);
	if(ret == -1)
	{
		logMsg("open port [%s] failed,current coreid [%d]\r\n",INCORENAME,0,0,0,0,0);
		eipc_errno_get(1);
	}
	else 
		logMsg("open port [%s] successed,current coreid [%d]\r\n",INCORENAME,0,0,0,0,0);
	return ret;
}
int create_write_port(int memnum,int index)
{
	int ret;
	ret = eipc_open(INCORENAME1,SLAVE,memnum,index);
	if(ret == -1)
	{
		logMsg("open port [%s] failed,current coreid [%d]\r\n",INCORENAME,0,0,0,0,0);
		eipc_errno_get(1);
	}
	else 
		logMsg("open port [%s] successed,current coreid [%d]\r\n",INCORENAME,0,0,0,0,0);
	return ret;
}
UINT32 rmEipcSetInc(UINT32* pcBuf,UINT32 uiLen)
{
	UINT32 uiLoop = 0;
	
	for(uiLoop = 0;uiLoop < (uiLen/4);uiLoop++)
	{
		pcBuf[uiLoop] = uiLoop;
	}
}

void tovxworks(int memnum, char msgtype,
	char linktype,int Num,int size,int indexid,INT32 flag)
{

	int i;
	int ret;
	char *sndbuf = NULL;
	UINT32 *rcvbuf = NULL;
	UINT32 *buf;
	UINT32 uiLoop = 0,uiCnt = 0 ;
	rcvbuf = (UINT32 *)0x10000000;
	if(flag == 0)
	{
	
		mh = create_read_port(memnum,0,INCORENAME);
	}
	else
	{
	
		mh = create_write_port(memnum,1);
	}

	logMsg("before eipc_hunt\r\n",0,0,0,0,0,0);
	eipc_hunt(mh,-1);
	logMsg("after eipc_hunt\r\n",0,0,0,0,0,0);
	eipc_print_attr(mh);

	buf = memalign(0x2000000, 0x2000000);
	logMsg("buf 0x%x \n",buf,0,0,0,0,0);
	memset((void *)buf, 0, 0x2000000);
	
	rioMapInb(buf,0xA0000000,0x2000000,0);
	if(flag == 1)
	{
		rmEipcSetInc(buf,0x2000000);
		/*memset(buf,12,0x2000000);*/
		rioMapOutb (0xa0000000,0xf4000000,0x1000000,49,0);
	}
	for(i=0;i<Num;i++)
	{
		struct	in_addr remoteip;
		
		/*eipc_barrier(mh,0,10);*/
		/*memset(sndbuf,1+i,size);*/
		if(flag == 0)
		{
		
		ret = eipc_recv(mh,msgtype,linktype,
				buf,/*0xA0000000*/ size, 100, (int *)&remoteip,0);
		}
		else
		{
			ret =  eipc_send(mh,msgtype, 
					linktype,buf,0xa0000000,size, 100, 0);
		}

		if(ret == -1)
		{
			logMsg("eipc_recv failed,current rioid = %d\r\n",0,0,0,0,0,0);
			eipc_errno_get(1);			
		}
		else
		{
			if(flag == 0)
			{
				for(uiLoop = 0;uiLoop < (ret/4);uiLoop ++)
				{
					if(AHCI_CPU_LE32(buf[uiLoop]) != uiCnt)
					{
						logMsg("data is error right : 0x%x curr data 0x%x cur data 0x%x\n",
								uiCnt,AHCI_CPU_LE32(rcvbuf[uiLoop]),rcvbuf[uiLoop],0,0,0);
						return ;
					}
				}
				uiCnt ++;
			}
			if(linktype == NET)
					logMsg("[%d] there are %u bytes receved from [%s],buf[1000] = %d\r\n",
					i,ret,inet_ntoa(remoteip),rcvbuf[1000],0,0);
				else
					logMsg("[%d] there are %u bytes receved from [%d],buf[1000] = %d\r\n",
					i,ret,*((int *)&remoteip),rcvbuf[1000],0,0);
		}
		
	}


	
	if(eipc_close(mh,10)==-1)
		logMsg("there are error happened when closed !\r\n",0,0,0,0,0,0);
		
}

void eipcstart(INT32 flag,int Num)
{
		eipc_init(0);
#if 1/*onttomulti beyond node*/
	/*测试片间的eipc_scatter函数*/
	tovxworks(2,BLK_DATA,RAPIDIO,100,5*1024*1024,1,flag);
	
#endif
}
typedef struct tagRMEIPC_DATE_TIME_S
{
    UINT32 Year;
    UINT8  Month;
    UINT8  Day;
    UINT8  Hour;
    UINT8  Minute;
    UINT8  Second;
}RMEIPC_DATE_TIME_S;

VOID setDefaultEipcTimes(void)
{
	RMEIPC_DATE_TIME_S		recdTimes;

    recdTimes.Year = 2016;
    recdTimes.Month = 1;
    recdTimes.Day = 1;
    recdTimes.Hour =0;
    recdTimes.Minute = 0;
    recdTimes.Second = 0; 
	/*设置RTC*/
	setRtc(16,recdTimes.Month,recdTimes.Day,0,
		recdTimes.Hour,recdTimes.Minute,
		recdTimes.Second,0);
	
   	printf("rec time:%04d-%02d-%02d,%02d:%02d:%02d\n",
			recdTimes.Year, recdTimes.Month,
			recdTimes.Day,recdTimes.Hour, 
			recdTimes.Minute,recdTimes.Second);
	
}

INT32 IntervalTime(RTC_DATE_TIME *curTmpTime,RTC_DATE_TIME *lastTmpTime)
{
	INT32 intervalTime = 0;
	
	/*假设时间间隔最大到一个小时*/
	if(curTmpTime->hour > lastTmpTime->hour)/*时间间隔超过了一个小时*/
	{
		intervalTime = ((curTmpTime->hour - lastTmpTime->hour -1)*60*60*1000 +
						(((59 - lastTmpTime->minute) + curTmpTime->minute))*60*1000 + 
						(((59 - lastTmpTime->second) +curTmpTime->second))*1000 +
						((1000 - lastTmpTime->mSec) + curTmpTime->mSec));
	}
	else
	{
		/*时间间隔相差分钟*/
		if(curTmpTime->minute > lastTmpTime->minute)
	 	{
		 	intervalTime = ((curTmpTime->minute - lastTmpTime->minute -1)*60*1000 +
						((59 - lastTmpTime->second) + curTmpTime->second)*1000 +
						((1000 - lastTmpTime->mSec) + curTmpTime->mSec));
	 	}
		else
		{
			if(curTmpTime->second > lastTmpTime->second)
			{
				intervalTime = ((curTmpTime->second - lastTmpTime->second -1)*1000 +
								(1000 - lastTmpTime->mSec) + curTmpTime->mSec);
			}
			else
			{
				intervalTime = (curTmpTime->mSec - lastTmpTime->mSec);
			}
		}
	}
	return intervalTime;
}

INT32 eipcSendTest(int memnum, char msgtype,
	char linktype,int Num,int size)
{
	int i,uiCnt = 0;
	int ret,readLen = 0;
	UINT32 *buf =NULL;
	UINT32 uiLoop = 0, uiSendLen = 0;
	INT32	eipcHandler = -1,time = 0;
	RTC_DATE_TIME tmpTimetest[2];
	INT32	 curIntTime[2] = 0;
	INT32     fd = -1;

	fd = open("/vol/test",0,0);
	if(fd < 0)
	{
		logMsg("open file failed \n",0,0,0,0,0,0);
		return ERROR;
	}
	eipcHandler = create_write_port(memnum,1);
	
	logMsg("before eipc_hunt\r\n",0,0,0,0,0,0);
	eipc_hunt(eipcHandler,-1);
	logMsg("after eipc_hunt\r\n",0,0,0,0,0,0);
	eipc_print_attr(eipcHandler);

	buf = memalign(0x2000000, 0x2000000);
	logMsg("buf 0x%x \n",buf,0,0,0,0,0);
	memset((void *)buf, 0, 0x2000000);
	
	rmEipcSetInc(buf,0x2000000);
	
	rioMapOutb (0xa0000000,0xf4000000,0x1000000,49,0);

	time = tickGet();
	while(TRUE)
	{
	
		sysFpgaRtcGet(&tmpTimetest[0]);/*获取当前时间*/
		readLen = read(fd,buf,0x500000);
		while(readLen == 0)
		{
			readLen = read(fd,buf,0x500000);
		}
		sysFpgaRtcGet(&tmpTimetest[1]);/*获取当前时间*/
		curIntTime[0] = IntervalTime(&tmpTimetest[1],&tmpTimetest[0]);
		
		sysFpgaRtcGet(&tmpTimetest[0]);/*获取当前时间*/
		ret =  eipc_send(eipcHandler,msgtype, 
			linktype,(buf),0xa0000000,readLen, 100, 0);
		if(ret == -1)
		{
			logMsg("eipc_recv failed,current rioid = %d\r\n",0,0,0,0,0,0);
			eipc_errno_get(1);			
		}

		sysFpgaRtcGet(&tmpTimetest[1]);/*获取当前时间*/
		curIntTime[1] = IntervalTime(&tmpTimetest[1],&tmpTimetest[0]);
		uiCnt ++;
		if(uiCnt == 1000)
		{
			logMsg("send time %d ms read time %d ms \n",
				curIntTime[0],curIntTime[1],0,0,0,0);
			uiCnt= 0;
		}
		uiSendLen += ret;
		if((tickGet()-time) > 5*sysClkRateGet())
		{
			logMsg("send speed %d M/S \n",
				uiSendLen/(5*0x100000),0,0,0,0,0);
			uiSendLen = 0;
			time = tickGet();
		}
	}


}

pthread_mutex_t test_mutex;
INT32 eipcRecTest(int memnum, char msgtype,
		char linktype,int Num,int size)
{
	int i;
	int ret;
	char *sndbuf = NULL;
	UINT32 *buf,*rcvBuf = NULL;
	UINT32 uiLoop = 0,uiCnt = 0 ;
	INT32	eipcHandler = -1;
	RTC_DATE_TIME tmpTimetest[2];
	INT32    curIntTime[2] = 0;
	UINT32 uiStartTime = 0;
	
	eipcHandler = rmcreate_read_port(memnum,0);

	logMsg("before eipc_hunt\r\n",0,0,0,0,0,0);
	eipc_hunt(eipcHandler,-1);
	logMsg("after eipc_hunt\r\n",0,0,0,0,0,0);
	eipc_print_attr(eipcHandler);

	buf = memalign(0x2000000, 0x2000000);
	logMsg("buf 0x%x \n",buf,0,0,0,0,0);
	memset((void *)buf, 0, 0x2000000);
	if(RAPIDIO == linktype)
	{
		rioMapInb(buf,0xA0000000,0x2000000,0);
		rcvBuf = 0xA0000000;
	}else
	{
		rcvBuf = buf;
	}
	struct	in_addr remoteip;

	while(TRUE)
	{
		sysFpgaRtcGet(&tmpTimetest[0]);/*获取当前时间*/
		if(MSG_DATA == msgtype)
		{
			ret = eipc_recv(eipcHandler,msgtype,linktype,
				buf,size, 100, (int *)&remoteip,0);
		}
		else
		{
			ret = eipc_recv(eipcHandler,msgtype,linktype,
					rcvBuf,size, 100, (int *)&remoteip,0);
		}
		if(ret == -1)
		{
			logMsg("eipc_recv failed,current rioid = %d\r\n",0,0,0,0,0,0);
			eipc_errno_get(1);			
		}
		else
		{	
			sysFpgaRtcGet(&tmpTimetest[1]);/*获取当前时间*/
			curIntTime[0] = rmIntervalTime(&tmpTimetest[1],&tmpTimetest[0]);
			uiCnt ++;
			if(uiCnt == 1000)
			{
				logMsg("receive  time %d ms \n",curIntTime[0],0,0,0,0,0);
				uiCnt= 0;
			}
			if(MSG_DATA == msgtype)
			{
				logMsg("receive msg time %d ms \n",curIntTime[0],0,0,0,0,0);
			}
		}
	}
	free(buf);
}

INT32 eipcMsgRecTest(int memnum, char msgtype,
		char linktype,int Num,int size,INT32	eipcHandler)
{
	int i;
	int ret,len =0 ;
	char *sndbuf = NULL;
	UINT32 *buf;
	UINT32 uiLoop = 0,uiCnt = 0 ,uiMsgCnt = 0;
	RTC_DATE_TIME tmpTimetest[2];
	INT32    curIntTime[2] = 0;
	UINT32 uiStartTime = 0;
	INT32 fd = -1;

	buf = memalign(0x2000000, 0x2000000);
	logMsg("buf 0x%x \n",buf,0,0,0,0,0);
	memset((void *)buf, 0, 0x2000000);
	
	int remoteip;
	
	/*logMsg("before eipc_hunt\r\n",0,0,0,0,0,0);
	eipc_hunt(eipcHandler,-1);
	logMsg("after eipc_hunt\r\n",0,0,0,0,0,0);
	eipc_print_attr(eipcHandler);*/

	while(TRUE)
	{
		sysFpgaRtcGet(&tmpTimetest[0]);/*获取当前时间*/

		ret = eipc_recv(eipcHandler,msgtype,linktype,
			buf,size, 100, (int *)&remoteip,0);

		if(ret == -1)
		{
			logMsg("eipc_recv failed,current rioid = %d\r\n",0,0,0,0,0,0);
			eipc_errno_get(1);			
		}
		else
		{	
			uiMsgCnt ++;
			sysFpgaRtcGet(&tmpTimetest[1]);/*获取当前时间*/
			curIntTime[0] = IntervalTime(&tmpTimetest[1],&tmpTimetest[0]);

			logMsg("***cnt %d receive msg time %d ms Write %d ms \n",
							uiMsgCnt,(curIntTime[0]/1000),curIntTime[1],0,0,0);

		#if 0
			if(linktype == NET)
					logMsg("[%d] there are %u bytes receved from [%s],buf[1000] = %d\r\n",
					i,ret,inet_ntoa(remoteip),buf[1000],0,0);
				else
					logMsg("[%d] there are %u bytes receved from [%d],buf[1000] = %d\r\n",
					i,ret,*((int *)&remoteip),buf[1000],0,0);
		#endif
		}
	}
	
}

void taskStart(INT32 flag,int Num)
{
	INT32 uiTaskID;

	
	
	uiTaskID = taskCreate("start",101,0,0x100000,
		eipcstart,flag,Num,0,0,0,0,0,0,0,0);
	if(uiTaskID == ERROR)
	{
		logMsg("recCmdRecv creat task error ! \n",0,0,0,0,0,0);
		return ERROR;
	}
	affinitySet(1,uiTaskID);
}



INT32 taskSendTest()
{
	INT32 uiTaskID;
	
	uiTaskID = taskCreate("send",101,0,0x100000,
		eipcSendTest,2,BLK_DATA,RAPIDIO,100,5*1024*1024,0,0,0,0,0);
	if(uiTaskID == ERROR)
	{
		logMsg("recCmdRecv creat task error ! \n",0,0,0,0,0,0);
		return ERROR;
	}
	affinitySet(1,uiTaskID);

}



INT32 taskRcvTest(char msgtype,char linktype,int size)
{
	INT32 uiTaskID;

	rmsetDefaultEipcTimes();
	uiTaskID = taskCreate("receive",102,0,0x100000,
			eipcRecTest,2,msgtype,linktype,100,size,0,0,0,0,0);
	if(uiTaskID == ERROR)
	{
		logMsg("recCmdRecv creat task error ! \n",0,0,0,0,0,0);
		return ERROR;
	}
	affinitySet(1,uiTaskID);

}
INT32 taskMsgTest()
{
	INT32 uiTaskID;
	/*setDefaultEipcTimes();
	eipc_init(0);*/
	INT32	eipcHandler = 0;

	eipcHandler = create_read_port(2,0,INCORENAMEMSG);

	uiTaskID = taskCreate("msgreceive",101,0,0x100000,
		eipcMsgRecTest,2,MSG_DATA,RAPIDIO,100,1024,eipcHandler,0,0,0,0);
	if(uiTaskID == ERROR)
	{
		logMsg("recCmdRecv creat task error ! \n",0,0,0,0,0,0);
		return ERROR;
	}
	affinitySet(0,uiTaskID);

}


SEM_ID		rmRecSem;

INT32 eipcMsgTest()
{
	INT32 ret = -1,sendLen ;
	UINT16 srcid = 50;
	UINT32 uiCnt =0;
	char *buf,testBuf[256];

	
	buf = memalign(0x2000000, 0x2000000);
	logMsg("buf 0x%x \n",buf,0,0,0,0,0);
	memset((void *)buf, 0, 0x2000000);
	
	rioMapInb(buf,0xA0000000,0x2000000,0);
	rmRecSem = semBCreate(SEM_Q_PRIORITY,SEM_EMPTY);
	memset(testBuf,0,256);
	
	while(TRUE)
	{
		ret = rioRecMsg((UINT8 *)buf,
						0x100, &srcid,WAIT_FOREVER);
		if(ret < 0)
		{
			logMsg("receive msg error \n",0,0,0,0,0,0);
		}
		if(memcmp(testBuf,buf,256) !=0 )
		{
			logMsg("receive buf %d, cur buf %d \n",buf[0],testBuf[0],0,0,0,0);
			break;
		}
		semGive(rmRecSem);
		/*sendLen = rioSendMsg(0,0,srcid,0x10000000,ret);
		if(sendLen < 0)
		{
			logMsg("send msg error \n",0,0,0,0,0,0);
		}
		*/
		uiCnt ++;
		if(uiCnt == 256)
		{
			logMsg("send msg cnt %d  \n",uiCnt,0,0,0,0,0);
			uiCnt =0;
		}
		
	}
}
INT32 eipcMsgTestPrin()
{
	UINT32 uiCnt =0;
	INT32 sendLen = 0;
	
	while(TRUE)
	{
		semTake(rmRecSem,WAIT_FOREVER);
		sendLen = rioSendMsg(0,0,50,0x10000000,strlen("hello"));
		if(sendLen < 0)
		{
			logMsg("send msg error \n",0,0,0,0,0,0);
		}
		if(uiCnt == 3000)
		{
			logMsg("send msg cnt %d  \n",uiCnt,0,0,0,0,0);
			uiCnt =0;
		}
		uiCnt ++;
	}
}

INT32 eipcMsgTestTask()
{
	INT32 uiTaskID;
	setDefaultEipcTimes();

	uiTaskID = taskCreate("eipcMsgTest",101,0,0x100000,
		eipcMsgTest,0,0,0,0,0,0,0,0,0,0);
	if(uiTaskID == ERROR)
	{
		logMsg("recCmdRecv creat task error ! \n",0,0,0,0,0,0);
		return ERROR;
	}
	affinitySet(0,uiTaskID);

}
INT32 eipcMsgTestPrinTask()
{
	INT32 uiTaskID;
	setDefaultEipcTimes();

	uiTaskID = taskCreate("eipcMsgTestPrin",101,0,0x100000,
		eipcMsgTestPrin,0,0,0,0,0,0,0,0,0,0);
	if(uiTaskID == ERROR)
	{
		logMsg("recCmdRecv creat task error ! \n",0,0,0,0,0,0);
		return ERROR;
	}
	affinitySet(0,uiTaskID);

}


