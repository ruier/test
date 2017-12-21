#if 0
#ifndef COMPILE_BOOTROM

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
#endif
