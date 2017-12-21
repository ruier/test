#include "mot85xxPci.h"
#include "m8640Reg.h"

IMPORT STATUS	shellPromptFmtDftSet (const char * interp,const char * promptFmt);
IMPORT UINT32 sysClkFreqGet(void);

IMPORT STATUS sysI2cInit();
IMPORT STATUS bspLct2991Init();
IMPORT void bspSetOverThres();

static SEM_ID semFpgaTimerAccess = NULL;

int sysGetCpuID(void)
{
	int reg; 
	reg = *(int*)(CCSBAR + 0xe0020);
	reg &=0xf;
	return reg;
}

void bspRs485Sw(UINT8 cpuId)
{
	
	if(cpuId == 0)*(UINT32*)(LOCAL_BUS_ADDR_CS3+0x014) = 0x0;
	else if(cpuId == 1)*(UINT32*)(LOCAL_BUS_ADDR_CS3+0x0014) = 0x1;
}


STATUS bspTempRead(int ch,UINT8 *data)
{
	if((ch>7)||(ch<0))
	{
		return ERROR;
	}
	*data = *(UINT8 *)(LOCAL_BUS_ADDR_CS3+0x0104 + ch*4);
	return OK;
}


STATUS bspOverTempRead(int ch,UINT8 *data)
{
	int offset = 0x0;
	
	switch(ch)
	{
	case 0:
		offset = 0x0b;
		break;
	case 1:
		offset = 0x17;
		break;
	case 2:
		offset = 0x1b;
		break;
	case 3:
		offset = 0x1f;
		break;
	default:
		return ERROR;
		break;
	}
	
	*data = *(UINT8 *)((LOCAL_BUS_ADDR_CS3+0x100) + offset); 
	return OK;
}

STATUS bspAlertTempRead(int ch,UINT8 *data)
{
	if((ch>7)||(ch<0)) return ERROR;
	*data = *(UINT8 *)((LOCAL_BUS_ADDR_CS3+0x105) + ch*4);
	return OK;
}



void bspTempShow(UINT32 time)
{
	UINT32 i,j;
	UINT8 temp[7];
	
	printf("-------------------------------------------------------------------------------------------\n");
	printf("ITEM            ALERT THRESHOLD\n");
	printf("-------------------------------------------------------------------------------------------\n");
	for(j=0;j<7;j++) bspAlertTempRead(j,&temp[j]);
	printf("LOCAL1          %d\n",temp[0]);
	printf("CPU A           %d\n",temp[1]);
	printf("CPU B           %d\n",temp[2]);
	printf("PEX8648         %d\n",temp[3]);
	printf("TSI1848         %d\n",temp[4]);
	printf("FPGA           	%d\n",temp[5]);
	printf("LOCAL2          %d\n",temp[6]);
	
	printf("-------------------------------------------------------------------------------------------\n");
	
	printf("-------------------------------------------------------------------------------------------\n");
	printf("INDEX  LOCAL1  CPUA   CPUB   8648   1848   FPGA   LOCAL2\n");		  
	printf("-------------------------------------------------------------------------------------------\n");
	for(i=0;i<time;i++)
	{
		for(j=0;j<7;j++) bspTempRead(j,&temp[j]);
		printf("%d:      %d      %d     %d     %d     %d     %d    %d\n",i,temp[0],temp[1],temp[2],temp[3],temp[4],temp[5],temp[6]);
		taskDelay(240);
	}	
}


STATUS bspVoltageRead(int ch,UINT16 *data )
{
	UINT16 value=0;
	if((ch>6)||(ch<0))	return ERROR;
	value = *(UINT8 *)((LOCAL_BUS_ADDR_CS3+0x120)+ch*2);
	value <<=8;
	value |= *(UINT8 *)((LOCAL_BUS_ADDR_CS3+0x120)+ch*2+1);
	*data =0;
	*data = ((value>>8)&0xff)*100;
	*data += (value&0xff);
	return OK;
}

void bspVoltageShow(UINT32 time)
{
	UINT8 i,j;
	UINT16 voltage[6];
	printf("INDEX   V1.0   V0.95   V1.75   V1.2   V12   V1.05\n");
	
	for(j=0;j<time;j++)
	{
		for(i=0;i<6;i++) bspVoltageRead(i,&voltage[i]);
		printf("%d:   %.2fv   %.2fv   %.2fv   %.2fv   %.2fv   %.2fv\n",j,voltage[0]/100.0,voltage[1]/100.0,voltage[2]/100.0,voltage[3]/100.0,voltage[4]/100.0,voltage[5]/100.0);
		taskDelay(180);
	}
}

STATUS bspCurrentRead(int ch,UINT16 *data )
{
	UINT16 value=0;
	if((ch>6)||(ch<0))	return ERROR;
	value = *(UINT8 *)((LOCAL_BUS_ADDR_CS3+0x140)+ch*2);
	value <<=8;
	value |= *(UINT8 *)((LOCAL_BUS_ADDR_CS3+0x140)+ch*2+1);
	*data =0;
	*data = ((value>>8)&0xff)*100;
	*data += (value&0xff);
	return OK;
}

void bspCurrentShow(UINT32 time)
{
	UINT8 i,j;
	UINT16 current[6];
	printf("INDEX   V1.0   V0.95   V1.75   V1.2   V12   V1.05\n");
	
	for(j=0;j<time;j++)
	{
		for(i=0;i<6;i++) bspCurrentRead(i,&current[i]);
		printf("%d:   %.2fA   %.2fA   %.2fA   %.2fA   %.2fA   %.2fA\n",j,current[0]/100.0,current[1]/100.0,current[2]/100.0,current[3]/100.0,current[4]/100.0,current[5]/100.0);
		taskDelay(180);
	}
}


void bspShellPromptInit (void)
{
    /* Change the default Shell Prompt */

	switch( sysGetCpuID())
	{
	case 0:
		shellPromptFmtDftSet ("C", "A->");
		break;
	case 1:
		shellPromptFmtDftSet ("C", "B->");
		break;
	default:
		break;
	}

}



int sysSlotNumGet(void)
{
	unsigned int slotId,slotId1,check;
	int i;

	slotId = (*(volatile int*)(LOCAL_BUS_ADDR_CS3+0x10)) & 0x3f;
	slotId = 0x3f-slotId; 		  /* neg */
	check = 0;
	slotId1 = slotId;
	for(i = 0; i < 6; i++)
	{
		if ((slotId1&0x1)==0x1)
			check ^=  0x1;
		slotId1 >>= 1;
	}
	if (check != 0x1)   slotId=0xff;
	else slotId = slotId&0x1f;
	return (int)slotId;

}


int sysGetChassisID(void)
{
	int value;
	value = (*(volatile int*)(LOCAL_BUS_ADDR_CS3+0x04)) & 0x0f;
	return (value);
}

int sysGetSlotID(void)
{
	unsigned int slotId,slotId1,check;
	int i;

	slotId = (*(volatile int*)(LOCAL_BUS_ADDR_CS3+0x10)) & 0x3f;
	slotId = 0x3f-slotId; 		  /* neg */
	check = 0;
	slotId1 = slotId;
	for(i = 0; i < 6; i++)
	{
		  if ((slotId1&0x1)==0x1)
			   check ^=  0x1;
		  slotId1 >>= 1;
	}
	if (check != 0x1)   slotId=0xff;
	else slotId = slotId&0x1f;
	return (int)slotId;

}


void bspCpuReset(UINT8 cpuId)
{
	
	if( cpuId == 0) 	*(UINT32*)(LOCAL_BUS_ADDR_CS3+0x6c)=0x5a5a5a5a;
	else *(UINT32*)(LOCAL_BUS_ADDR_CS3+0x70)=0x5a5a5a5a;
}

void bspBoardReset()
{
	*(UINT32*)(LOCAL_BUS_ADDR_CS3+0x74)=0x5a5a5a5a;
}

void bspSystermReset()
{
	*(UINT32*)(LOCAL_BUS_ADDR_CS3+0x78)=0x5a5a5a5a;
}



static STATUS bspFpgaTimerInit()
{
	semFpgaTimerAccess = semBCreate(SEM_Q_FIFO, SEM_FULL);
	if(semFpgaTimerAccess == NULL) return ERROR;
	return OK;	
}


void bspInit()
{
	if(bspFpgaTimerInit() == ERROR) printf("bspFpgaTimerInit error\n");
#ifdef DRV_M8641_IIC
	if(sysI2cInit() == ERROR) printf("system I2C Init Fail\n");
#endif
}


void read8(UINT32 addr,UINT32 count)
{
	UINT32 i;
	for(i=0;i<count;i++) printf("0x%x:0x%x\n",addr+i,*(UINT8*)(addr+i));
	
}




STATUS sysGetTime(UINT32 *timeH,UINT32 *timeL)
{
	UINT8 cpuId;
	cpuId = sysGetCpuID();
	if(cpuId>3) return ERROR;
#if 0
	//if(semTake(semFpgaTimerAccess,0x200) == ERROR) 
	if(semTake(semFpgaTimerAccess,WAIT_FOREVER) == ERROR) 
		{
		//printf("semTake error!!\n");
		return ERROR;
		}
#endif
	*timeL=*(UINT32 *)(LOCAL_BUS_ADDR_CS3+0x30+cpuId*8);
	*timeH =*(UINT32 *)(LOCAL_BUS_ADDR_CS3+0x30+cpuId*8+4);
	//semGive(semFpgaTimerAccess);
	return OK;
}


UINT32 sysFpgaTimerClkFreqGet()
{
	UINT32 sysClkFreq,lbClkRatio;
	sysClkFreq = sysClkFreqGet();
	lbClkRatio = *(UINT32*)(CCSBAR+0x50d4);
	lbClkRatio = ((lbClkRatio&0xf)>>1)<<2;
	return(sysClkFreq/lbClkRatio);
	
}



float sysSpendTimeGet(UINT32 startTimeH,UINT32 startTimeL,UINT32 endTimeH,UINT32 endTimeL)
{
	float spendTime;
	UINT32 clkFreq;
	clkFreq = sysFpgaTimerClkFreqGet();
	//printf("startTimeH=0x%x,startTimeL=0x%x,endTimeH=0x%x,endTimeL=0x%x,clkFreq=%d\n",startTimeH,startTimeL,endTimeH,endTimeL,clkFreq);
	if(endTimeL<startTimeL) 
		spendTime = (endTimeL+0xffffffff-startTimeL) + (endTimeH-1-startTimeH)*4.0*1024*1024*1024;
	else  spendTime = (endTimeL-startTimeL) + (endTimeH-startTimeH)*4.0*1024*1024*1024;
	
	spendTime = spendTime/clkFreq;
	
	
	//printf("startTimeH=0x%x,startTimeL=0x%x,endTimeH=0x%x,endTimeL=0x%x,clkFreq=%d  %f\n",startTimeH,startTimeL,endTimeH,endTimeL,clkFreq,spendTime);
	return spendTime;
	
}

#if 1
static STATUS demoFpgaTimerTest(UINT32 delayTime,UINT32 time)
{
	UINT32 fpgaTimerClkFreq,i;
	UINT32 startTimeH,startTimeL,endTimeH,endTimeL;
	UINT32 startTimeH1,startTimeL1,endTimeH1,endTimeL1;
	float timespend,timespend1;
	
	
	fpgaTimerClkFreq = sysFpgaTimerClkFreqGet();
	
	for(i=0;i<time;i++)
	{
		sysGetTime(&startTimeH,&startTimeL);
		vxTimeBaseGet(&startTimeH1,&startTimeL1);
		taskDelay(delayTime);
		sysGetTime(&endTimeH,&endTimeL);
		vxTimeBaseGet(&endTimeH1,&endTimeL1);
		
		timespend = sysSpendTimeGet(startTimeH,startTimeL,endTimeH,endTimeL);
		
		if(endTimeL1<startTimeL1) timespend1 = (endTimeL1+0xffffffff-startTimeL1) + (endTimeH1-1-startTimeH1)*4.0*1024*1024*1024;
		else  timespend1 = (endTimeL1-startTimeL1) + (endTimeH1-startTimeH1)*4.0*1024*1024*1024;
	
		timespend1 = timespend1*8/1000000000.0;
		printf("timespend = %.12f s, timespend1 = %.12f s\n",timespend,timespend1);
	}
	return OK;
}
#ifndef COMPILE_BOOTROM
STATUS  fpgaTimerTest(UINT32 delayTime,UINT32 time)
{

	int tid;
	cpuset_t affinity;

	tid = taskCreate ("tTestSmp0", 200, VX_FP_TASK, 0x200000,(FUNCPTR)demoFpgaTimerTest,
			delayTime, time, 0, 0,0, 0, 0, 0, 0, 0); 
	if (tid == NULL) return (ERROR);
		
	
	CPUSET_ZERO (affinity);
	CPUSET_SET (affinity, 0);
	if (taskCpuAffinitySet (tid, affinity) == ERROR)
	{
		/* Either CPUs are not enabled or we are in UP mode */
		taskDelete (tid);
		return (ERROR);
	}
	

	taskActivate (tid);
	return OK;
}
#endif
#endif





STATUS sysGetBootLock()
{
	int cpuId = sysGetCpuID();
	
	*(UINT8*)(BOOT_LOCK_REG) = (cpuId+1);
	//printf("lock=0x%x\n",*(UINT32*)(BOOT_LOCK_REG));
	if((*(UINT8*)(BOOT_LOCK_REG)) == (cpuId+1)) return OK;
	else return ERROR;
}

STATUS sysFreeBootLock()
{
	int cpuId = sysGetCpuID();
	if(*(UINT8*)(BOOT_LOCK_REG) == 0) return OK;
	else
	{
		*(UINT8*)(BOOT_LOCK_REG) = (cpuId+1);
		//printf("lock1=0x%x\n",*(UINT32*)(BOOT_LOCK_REG));
		if((*(UINT8*)(BOOT_LOCK_REG)) == 0)  return OK;
		else return ERROR;	
	}
}



void write8(UINT32 addr,UINT8 data)
{
	*(UINT8*)addr = data;
}


UINT32 bspRs485ClkFreqGet
(
void
)
{
	return 200000000;
}


UINT32 bspRs485DivisorGet
    (
    UINT32 xtal,
    UINT32 baud
    )
    {
    return (xtal / (8 * baud));
    }


#if 1
extern void vxbMsDelay(int	delayTime);
static void rs485Time(BOOL bit)
{
	UINT32 *pAddr=(UINT8*)(LOCAL_BUS_ADDR_CS3+0xa08);

	if(bit == 1) 
	{
		*pAddr = 1;     
		vxbMsDelay(5);
		*pAddr = 0;
		vxbMsDelay(5);
	}
	else
	{
		*pAddr = 1;     
		vxbMsDelay(2);
		*pAddr = 0;
		vxbMsDelay(8);
	}
	
}


static void rs485Position()
{
	UINT32 *pAddr=(UINT32*)(LOCAL_BUS_ADDR_CS3+0xa08);
	*pAddr = 1;     
	vxbMsDelay(8);
	*pAddr = 0;
	vxbMsDelay(2);
	
}


void rs485TimerTest(UINT32 time)
{
	UINT8 second=21;
	UINT8 minute=57;
	UINT8 hour=12;
	UINT32 date=230;
	UINT32 year=2013;
	UINT8 data,i,j;
	
	
	*(UINT32*)(LOCAL_BUS_ADDR_CS3+0xa04)=1;
	vxbMsDelay(1);
	while(time--)
	{
		/********P0********/
		rs485Position();
		/********Pr********/
		rs485Position();
		/********second********/
		//printf("second =%d\n",second);
		for(i=0;i<7;i++)
		{
			if(i == 0) data = second%10;
			else if(i ==4) 
			{
				data = second/10;
				rs485Time((data>>(i%4))&0x01);	
			}
			//printf("data =%d,i=%d\n",data,i%4);
			rs485Time((data>>i%4)&0x01);	
		}
		/********P1********/
		rs485Position();
		/********minute	********/
		for(i=0;i<8;i++)
		{
			if(i == 0) data = minute%10;
			else if(i ==4)  
			{
				data = minute/10;
				rs485Time((data>>i)&0x01);	
			}
			rs485Time((data>>(i%4))&0x01);
		}
		/********P2********/
		rs485Position();
		/********hour	********/
		for(i=0;i<8;i++)
		{
			if(i == 0) data = hour%10;
			else if(i ==4) 
			{
				data = hour/10;
				rs485Time((data>>i)&0x01);	
			}
			rs485Time((data>>(i%4))&0x01);	
		}
		/********P3********/
		rs485Position();
		/********date	********/
		for(i=0;i<8;i++)
		{
			if(i == 0) data = date%10;
			else if(i ==4) 
			{
				data = (date%100)/10;
				rs485Time((data>>i)&0x01);
			}
			rs485Time((data>>(i%4))&0x01);	
		}
		/********P4********/
		rs485Position();
		/********date	********/
		data = date/100;
		rs485Time((data>>0)&0x01);
		rs485Time((data>>1)&0x01);
		rs485Time((data>>1)&0x01);
		if(second%2)
		{
			rs485Time(0);
			rs485Time(0);
			data = year%10;
			for(i=0;i<4;i++) rs485Time((data>>i)&0x01);			
		}
		else 		
		{
			rs485Time(1);
			rs485Time(1);
			data = (year%100)/10;
			for(i=0;i<4;i++) rs485Time((data>>i)&0x01);			
		}
		for(j=0;j<5;j++)
		{
			/********P5 ~ P9********/
			rs485Position();
			/********date	********/
			data=second+j;
			for(i=0;i<8;i++)
			{
				if(i == 0) data = date%10;
				else if(i ==4) 
				{
					data = (date%100)/10;
					rs485Time((data>>i)&0x01);
				}
				rs485Time((data>>(i%4))&0x01);	
			}
		}
		second ++;
		if(second >=60) 
		{
			second =0;
			minute ++;
		}
		if(minute >=60) 
		{
			minute =0;
			hour ++;
		}
	}
}
#endif

typedef struct
{
	UINT32 mSecond;
	UINT8 second;
	UINT8 minute;
	UINT8 hour;
	UINT8 day;
	UINT8 year;	
}sBspTime;

void bspTimeRead(sBspTime *pTime)
{
	UINT32 mSecond,second,minute,hour,day,year;	
    
	
	*(UINT32 *)(LOCAL_BUS_ADDR_CS3+0xa0c) = 1;
	
	mSecond = (*(UINT32*)(LOCAL_BUS_ADDR_CS3+0xa40))&0x00ffffff;
//	pTime->mSecond = mSecond/100000;
	pTime->mSecond = mSecond;
	
	second = *(UINT32*)(LOCAL_BUS_ADDR_CS3+0xa10);
	pTime->second = (second&0xf)+((second>>4)&0xf)*10;

	
	minute = *(UINT32*)(LOCAL_BUS_ADDR_CS3+0xa14);
	minute = (minute&0xf)+((minute>>4)&0xf)*10;
	pTime->minute = minute;
	
	hour = *(UINT32*)(LOCAL_BUS_ADDR_CS3+0xa18);
	hour = (hour&0xf)+((hour>>4)&0xf)*10;
	pTime->hour = hour;
	
	day = *(UINT32*)(LOCAL_BUS_ADDR_CS3+0xa1c);
	day = (day&0xf)+((day>>4)&0xf)*10+((day>>8)&0xf)*100;
	pTime->day = day;
	
	year = *(UINT32*)(LOCAL_BUS_ADDR_CS3+0xa20);
	year = (year&0xf)+((year>>4)&0xf)*10;
	pTime->year = year;
	
	logMsg("%4d - %d  %2d:%2d:%02d - %d\n",pTime->year,pTime->day,pTime->hour,pTime->minute,pTime->second,pTime->mSecond);

}


void bspTimeReadTest()
{
	UINT8 timeReadStop = 0;
	while(!timeReadStop)
	{
		bspTimeRead((sBspTime *)0x20000000);
		taskDelay(20);
	}	
}

