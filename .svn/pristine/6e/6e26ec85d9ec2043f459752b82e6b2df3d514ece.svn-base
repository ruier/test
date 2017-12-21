#include "vxWorks.h"
#include "string.h"
#include "stdlib.h"
#include "taskLib.h"
#include "i2cLib.h"
#include "m5200I2CMaster.h"
#include "stdlib.h"

#define MAX_I2C_BUS              2
#define CPU_B_ID                 1
#define LBC_FPGA_ADDR            0x90000000

#define  CPUA_TEMP_OFFSET_LOW     0x10
#define  CPUA_TEMP_OFFSET_HIGH    0x01
#define  CPUB_TEMP_OFFSET_LOW     0x33
#define  CPUB_TEMP_OFFSET_HIGH    0x30
#define  FPGA3_TEMP_OFFSET        0x6
#define  ADT7483A_SLAVE_ADDR     0x98
#define  CPU_NUMBER_A            1
#define  CPU_NUMBER_B            2

#define  MAX6884_U705_SLAVE_ADDR  0xA0
#define  MAX6884_U709_SLAVE_ADDR  0xA4	  

UINT8 gucI2CBuf[256] = {0};
extern STATUS m5200I2CRead
( 
	M5200_I2C_DRV_CTRL * pDrvCtrl, /* ptr to driver structure */
	UINT8   addr7bit, 			/* Seven bit address */
	UINT8 * data,     			/* ptr to data buffer */
	UINT32  nbytes,   			/* number of bytes to read */
	UINT32  options,
	int     timeout             /* time out in ticks */
);

/* forwards */
extern STATUS m5200I2CRead
( 
	M5200_I2C_DRV_CTRL * pDrvCtrl, /* ptr to driver structure */
	UINT8   addr7bit, 			/* Seven bit address */
	UINT8 * data,     			/* ptr to data buffer */
	UINT32  nbytes,   			/* number of bytes to read */
	UINT32  options,
	int     timeout             /* time out in ticks */
);

extern STATUS m5200I2CWrite
(
	M5200_I2C_DRV_CTRL * pDrvCtrl, /* ptr to driver structure */
	UINT8   addr7bit,			/* Seven bit address */
	UINT8 * data,				/* ptr to data buffer */
	UINT32  nbytes,				/* number of bytes to write */
	UINT32  options,
	int 	timeout             /* timeout in ticks */
);
extern void * m5200I2CDrvLoad(char *);

extern int sysGetCpuID(void);
UINT32    guiWarnThres = 83;
UINT32    guiHaltThres = 100;
UINT32    guiTempLog = 0;

typedef struct tagI2C_CTRL_S
{
    M5200_I2C_DRV_CTRL  *pstI2CDrv;
    UINT32                  uiInitFlag;
}I2C_CTRL_S;


I2C_CTRL_S  gstI2CCtrl[MAX_I2C_BUS];

UINT8 ADT7483AWrite(UINT8 busNo,UINT8 devAddr,UINT8 regAddr,UINT8 data)
{
	UINT8 buf[2];
	
	buf[0] = regAddr;
	buf[1] = data;
	if(0 == gstI2CCtrl[busNo].uiInitFlag)
	{
        return 0xff;
	}
	
	if(ERROR == m5200I2CWrite(gstI2CCtrl[busNo].pstI2CDrv,devAddr,buf,2,0,
	                             0x100000))
	{
		printf("write failed devAddr 0x%x\n",devAddr);
        return 0xff;
	}
	
	return 0;
}

UINT8 ADT7483ARead(UINT8 busNo,UINT8 devAddr,UINT8 regAddr)
{
	UINT8 buf[2];
	
	buf[0] = regAddr;
	buf[1] = 0xff;
	if(0 == gstI2CCtrl[busNo].uiInitFlag)
	{
        return 0xff;
	}
	
	if(ERROR == m5200I2CWrite(gstI2CCtrl[busNo].pstI2CDrv,devAddr,buf,1,0,
	                             0x100000))
	{
        return 0xff;
	}
	
	taskDelay(sysClkRateGet()/60);
	
	if(ERROR == m5200I2CRead(gstI2CCtrl[busNo].pstI2CDrv,devAddr,&buf[1],1,0,
	                            0x100000))
	{
        return 0xff;
	}
	
	return buf[1];
}

VOID haltCPU(INT32 cpuNum)
{
    UINT32  uiRegVal = 0;
    /*
     *halt cpu
    */
    uiRegVal = *(UINT32*)(LBC_FPGA_ADDR + 0x8);

    printf("cpu number %d temperature overtop",cpuNum);
    
    /*
     *halt all cpu 
    */
    if(CPU_B_ID == cpuNum)
    {
        *(UINT32*)(LBC_FPGA_ADDR + 0x8) = uiRegVal & 0xd;
    }
    else
    {
        *(UINT32*)(LBC_FPGA_ADDR + 0x8) = uiRegVal & 0xe;
    }
}

STATUS haltAll(void)
{
    UINT32  uiRegVal = 0;
    
    uiRegVal = *(UINT32*)(LBC_FPGA_ADDR + 0x8);
    *(UINT32*)(LBC_FPGA_ADDR + 0x8) = (uiRegVal & 0);
    
    return OK;
}


void cpuTempWarn(INT32 cpuTemp,INT32 cpuTempLast,INT32 cpuNum)
{
    if((0xff == cpuTemp) || (0xff == cpuTempLast))
    {
        return;
    }
    
    if((((cpuTemp-cpuTempLast) > 6) && ((cpuTemp-cpuTempLast)>0)))
    {
        printf("warning cpu heat up to high this %dC last %dC increase %dC \n"
			   ,cpuTemp,cpuTempLast,cpuTemp-cpuTempLast);
    }
			
	if(cpuTemp >= guiHaltThres)
	{
		printf("\nCPU temp is too high halt CPU\n");
		taskDelay(sysClkRateGet()/12);
		haltCPU(cpuNum);
	}
	if(cpuTemp >= guiWarnThres)
	{
		printf("warn cpu temp is high %d\n",cpuTemp);
	}
}

void fpgaTempWarn(INT32 cpuTemp,INT32 cpuTempLast)
{
    if((0xff == cpuTemp) || (0xff == cpuTempLast))
    {
        return;
    }
    
    if((((cpuTemp-cpuTempLast) > 6) && ((cpuTemp-cpuTempLast)>0)))
    {
        printf("warning FPGA heat up to high this %dC last %dC increase %dC \n"
			   ,cpuTemp,cpuTempLast,cpuTemp-cpuTempLast);
    }
			
	if(cpuTemp >= guiHaltThres)
	{
		printf("\FPGA temp is too high halt CPU\n");
		taskDelay(sysClkRateGet()/12);
		haltAll();
	}
	if(cpuTemp >= guiWarnThres)
	{
		printf("FPGA cpu temp is high %d\n",cpuTemp);
	}
}



STATUS i2cIsInited(UINT32 uiBusNo)
{
    if(gstI2CCtrl[uiBusNo].uiInitFlag == 0)
    {
        printf("I2C moudle is not init\n");
        return ERROR;
    }

    return OK;
}

VOID sysVoltIncInit()
{
	UINT8 readInternal;
	
	ADT7483AWrite(0,MAX6884_U709_SLAVE_ADDR, 0xf,0x3f);

	taskDelay(sysClkRateGet() / 50);
	readInternal = ADT7483ARead(0, MAX6884_U709_SLAVE_ADDR, 0x16);
	readInternal &= 0x7f;
	/*ÄÚ²¿ADC*/
	ADT7483AWrite(0,MAX6884_U709_SLAVE_ADDR, 0x16,readInternal);
	/*
	taskDelay(sysClkRateGet() / 50);

	ADT7483AWrite(0,MAX6884_U709_SLAVE_ADDR,0xf,0x3f);	*/
}

UINT32 sysGetVolt(UINT32 vuiMAX6884SlaveAddr, UINT8 ucChan)
{
	UINT8   cpuVoltLo = 0;
	UINT8   cpuVoltHi = 0;
	UINT8   uiAddr = 0;
	UINT8   busNo = 0;
	UINT32  cpuVolt = 0;


	/*
	 * volt = (X + 5) * 0.01
	*/
	if(OK != i2cIsInited(busNo))
	{
	    return 0xff;
	}

	uiAddr = 0x18 + 2 * ucChan;
	cpuVoltHi = ADT7483ARead(busNo, vuiMAX6884SlaveAddr, uiAddr);
	
	taskDelay(sysClkRateGet() / 10);

	uiAddr = 0x19 + 2 * ucChan;
	cpuVoltLo = ADT7483ARead(busNo, vuiMAX6884SlaveAddr, uiAddr) & 0x3;
	/*logMsg("cpuVoltHi 0x%x cpuVoltLo %x \n",cpuVoltHi,cpuVoltLo,0,0,0,0);*/
	
	cpuVolt = cpuVoltHi;
	cpuVolt = (cpuVolt << 2) | (cpuVoltLo);
	
	/*logMsg("cpuVolt %x \n",cpuVolt,0,0,0,0,0);*/
	
	cpuVolt = 2*(cpuVolt * 366)/100;/*convert to mv*/

	return cpuVolt;
}

UINT8 sysGetCpuTemp(void)
{
	UINT8  cpuTempLow = 0;
	UINT8  cpuTempHigh = 0;
	UINT8  busNo = 0;
	
	if(gstI2CCtrl[busNo].uiInitFlag == 0)
	{
		printf("temp moudle is not init\n");
		return 0xff;
	}
	
	cpuTempLow = ADT7483ARead(busNo,ADT7483A_SLAVE_ADDR,CPUB_TEMP_OFFSET_LOW);
	if(0xff == cpuTempLow)
	{
		return 0xff;
	}
	
	cpuTempHigh = ADT7483ARead(busNo,ADT7483A_SLAVE_ADDR,CPUB_TEMP_OFFSET_HIGH);
	if(0xff == cpuTempHigh)
	{
		return 0xff;
	}
	
	return cpuTempHigh;
}

UINT8 sysGetFPGATemp(void)
{
    UINT8  cpuTempLow = 0;
    UINT8  cpuTempHigh = 0;
    UINT8  busNo = 0;
    
    if(gstI2CCtrl[busNo].uiInitFlag == 0)
    {
        printf("temp moudle is not init\n");
        return 0xff;
    }
    
    cpuTempLow = ADT7483ARead(busNo,ADT7483A_SLAVE_ADDR,CPUA_TEMP_OFFSET_LOW);
    if(0xff == cpuTempLow)
    {
        return 0xff;
    }
    
    cpuTempHigh = ADT7483ARead(busNo,ADT7483A_SLAVE_ADDR,CPUA_TEMP_OFFSET_HIGH);
    if(0xff == cpuTempHigh)
    {
        return 0xff;
    }
    
    return cpuTempHigh;
}

STATUS tempModuleInit(UINT8 busNo)
{
	char buf[64] = {0};
	
	if(0x0 == busNo)
	{
		strcpy(buf,"0:0xe0118000");
	}
	else if(0x1 == busNo)
	{
		strcpy(buf,"0:0xe0118100");
	}
	else if(0x2 == busNo)
	{
		strcpy(buf,"0:0xe0119000");
	}
	else
	{
		strcpy(buf,"0:0xe0119100");
	}
	
	gstI2CCtrl[busNo].pstI2CDrv = (M5200_I2C_DRV_CTRL*)m5200I2CDrvLoad(buf);
	if(ERROR == gstI2CCtrl[busNo].pstI2CDrv)
	{
		return ERROR;
	}
		
	gstI2CCtrl[busNo].uiInitFlag = 1;
	
	return OK;
}


void cpuTempMonitor()
{    
	INT32 fpgaTempLast = 0;
	UINT8 fpgaTemp = 0,cpuTemp = 0;
	UINT8 configBit = 0;
	UINT32 volt = 0,uiLoop;

	/*sysVoltIncInit();*/

	fpgaTemp = sysGetFPGATemp();

    while(1)
	{
		fpgaTempLast = fpgaTemp;
		taskDelay(sysClkRateGet()*5);
		fpgaTemp = sysGetFPGATemp();
		cpuTemp = sysGetCpuTemp();
		if(guiTempLog)
		{
            logMsg("fpga temp is   %d,cpu temp  is    %d \n",fpgaTemp,cpuTemp,0,0,0,0);
		}
		#if 0
		for(uiLoop = 0;uiLoop <= 6;uiLoop++)
		{
			volt = sysGetVolt(MAX6884_U709_SLAVE_ADDR, uiLoop);
			if(guiTempLog)
			{
	            logMsg("IN%d voltal  is    %d \n",volt,0,0,0,0,0);
			}			
		}
		#endif
		*(UINT8*)0x90000074 = fpgaTemp;
		/*fpgaTempWarn(fpgaTemp,fpgaTempLast);	*/
	}    
}

STATUS tempMonitorInit()
{
    char   buf[64] = {0};

	guiWarnThres = 0x5a;
	guiHaltThres = 0x64; 
	taskSpawn("tempMon",105,0,0x10000,cpuTempMonitor,0,0,0,0,0,0,0,0,0,0);

	return OK;
}

void tempLog(UINT32 uiFlag)
{
    if(uiFlag)
    {
        guiTempLog = 1;
    }
    else
    {
        guiTempLog = 0;
    }
}

VOID tempSetWarn(UINT32 uiWarn,UINT32 uiHalt)
{
    if(uiWarn <= uiHalt)
    {
        printf("halt temperature should larger than warnning\n");
        return ;
    }
    guiWarnThres = uiWarn;
    guiHaltThres = uiHalt;
}

UINT32 testI2cMasterReadCheckReg(UINT32 uiDevAddr,UINT32 uiRegNo)
{
    UINT32 uiVal = 0;
    
    uiVal = ADT7483ARead(0,uiDevAddr,uiRegNo);

    return uiVal;
}

STATUS i2cWriteBuf(UINT8 busNo,UINT8 devAddr,UINT8 *data,UINT32 nbytes)
{
	STATUS iRet = 0;
	
	if(busNo > 1)
	{
		printf("bus number %d error\n",busNo);
		return ERROR;
	}

	iRet = m5200I2CWrite(gstI2CCtrl[busNo].pstI2CDrv,devAddr,data,nbytes,0,
	                             0x100000);

	return iRet;
}


STATUS  i2cWriteBurst(UINT8 devAddr,UINT8 bytes,UINT8 data1,UINT8 data2,UINT8 data3,
		              UINT8 data4,UINT8 data5,UINT8 data6,UINT8 data7,UINT8 data8,
		              UINT8 data9,UINT8 data10,UINT8 data11,UINT8 data12,UINT8 data13,
		              UINT8 data14,UINT8 data15,UINT8 data16,UINT8 data17,UINT8 data18,
		              UINT8 data19,UINT8 data20,UINT8 data21,UINT8 data22,UINT8 data23,
		              UINT8 data24)
{
	STATUS iRet = OK;

	gucI2CBuf[0] = data1;
	gucI2CBuf[1] = data2;
	gucI2CBuf[2] = data3;
	gucI2CBuf[3] = data4;
	gucI2CBuf[4] = data5;
	gucI2CBuf[5] = data6;
	gucI2CBuf[6] = data7;
	gucI2CBuf[7] = data8;
    gucI2CBuf[8] = data9;
    gucI2CBuf[9] = data10;
    gucI2CBuf[10] = data11;
    gucI2CBuf[11] = data12;
    gucI2CBuf[12] = data13;
    gucI2CBuf[13] = data14;
    gucI2CBuf[14] = data15;
    gucI2CBuf[15] = data16;
    gucI2CBuf[16] = data17;
    gucI2CBuf[17] = data18;
    gucI2CBuf[18] = data19;
    gucI2CBuf[19] = data20;
    gucI2CBuf[20] = data21;
    gucI2CBuf[21] = data22;
    gucI2CBuf[22] = data23;
    gucI2CBuf[23] = data24;
	
	iRet = i2cWriteBuf(0,devAddr,&gucI2CBuf[0],bytes);
	
	return iRet;
}


