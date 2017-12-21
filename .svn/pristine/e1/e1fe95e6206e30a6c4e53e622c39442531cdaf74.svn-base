#include <stdio.h>
#include <vxWorks.h>
#include "vxbm8641i2c.h"


typedef  struct  bspM8641I2cFunc
{
	UINT32  i2cBaseAddr;
	SEM_ID i2cAccessSem;
	int(*i2cWrite)( UINT32 baseAddr,UINT8 deviceAddress,UINT32   startBlk,UINT8   numBlks,UINT8  *pBuf,UINT8 mode);
    int(*i2cRead)(UINT32 baseAddr,UINT8  deviceAddress,UINT32    startBlk,UINT8     numBlks,UINT8  *pBuf,UINT8 mode);	
}S_BSP_M8641_I2C_FUNC;

LOCAL S_BSP_M8641_I2C_FUNC gBspM8641I2cFunc[2]=
{
		{0,NULL,NULL},
		{0,NULL,NULL}
};



LOCAL STATUS bspI2cFuncGet(UINT8 busNo,S_BSP_M8641_I2C_FUNC *pI2cFunc)
{
	struct vxbDriverControl ctrl;
	struct i2cDrvCtrl *pDrvCtrl,drvCtrl;
	VXB_DEVICE_ID pDev;
	FUNCPTR     handler;
	
	
	pDrvCtrl = &drvCtrl;
	
	pDev=(VXB_DEVICE_ID)vxbInstByNameFind("i2c",busNo);
	if(pDev == NULL) 
	{
		printf("I2C vxbInstByNameFind fail\n");
		return ERROR;
	}
	 
	handler = vxbDevMethodGet(pDev, DEVMETHOD_CALL(driverControl));
	if( handler == NULL )
	{
	 printf("vxbDevMethodGet != OK!!!!\n");
	 return (ERROR);
	}
	 
	ctrl.driverName  = "i2c";		
	ctrl.drvCtrlInfo = pDrvCtrl;	
	 
	ctrl.drvCtrlCmd  = I2C_GET_READ_FUNCS;
	if((*handler)(pDev,&ctrl ) != OK)
	{	
		printf("handle != OK!!!!\n");
		return (ERROR);
	}
	else (pI2cFunc->i2cRead) = (pDrvCtrl->i2cRead);
	
	ctrl.drvCtrlCmd  = I2C_GET_WRITE_FUNCS;	/* commnad */	 
	if((*handler)(pDev,&ctrl ) != OK)
	{	
	 printf("handle != OK!!!!\n");
	 return (ERROR);
	}  
	else (pI2cFunc->i2cWrite) = (pDrvCtrl->i2cWrite);
	
	(pI2cFunc->i2cBaseAddr) = (pDrvCtrl->baseAddr);
	return OK;
}



STATUS sysI2cInit()
{
	BOOL flag;
	flag = bspI2cFuncGet(0,&gBspM8641I2cFunc[0]);
	flag = bspI2cFuncGet(1,&gBspM8641I2cFunc[1]);	
	gBspM8641I2cFunc[0].i2cAccessSem = semBCreate (SEM_Q_FIFO, SEM_FULL);
	gBspM8641I2cFunc[1].i2cAccessSem = semBCreate (SEM_Q_FIFO, SEM_FULL);
	return flag;
}



STATUS sysI2cRead(UINT8 busNo,UINT8 deviceAddr,UINT32 regAddr,UINT8 *pData,UINT32 count,UINT8 mode)
{

//	UINT32 i;
	if(semTake(gBspM8641I2cFunc[busNo].i2cAccessSem,0x200) ==ERROR)
	{
		printf("sysI2cRead:semTake fail\n");
		return ERROR;
	}
	//for(i=0;i<count;i++)
	{
		//if((gBspM8641I2cFunc[busNo].i2cRead)(gBspM8641I2cFunc[busNo].i2cBaseAddr,deviceAddr,regAddr+i,1,pData+i,mode)==ERROR)
		if((gBspM8641I2cFunc[busNo].i2cRead)(gBspM8641I2cFunc[busNo].i2cBaseAddr,deviceAddr,regAddr,count,pData,mode)==ERROR) 
		{
			semGive(gBspM8641I2cFunc[busNo].i2cAccessSem);
			return ERROR;
		}
	}
	semGive(gBspM8641I2cFunc[busNo].i2cAccessSem);
	return OK;
}

STATUS sysI2cWrite(UINT8 busNo,UINT8 deviceAddr,UINT32 regAddr,UINT8 *pData,UINT32 count,UINT8 mode)
{
	
	if(semTake(gBspM8641I2cFunc[busNo].i2cAccessSem,0x200) ==ERROR)
	{
		printf("sysI2cRead:semTake fail\n");
		return ERROR;
	}
	if((gBspM8641I2cFunc[busNo].i2cWrite)(gBspM8641I2cFunc[busNo].i2cBaseAddr,deviceAddr,regAddr,count,pData,mode) == ERROR)
	{
		semGive(gBspM8641I2cFunc[busNo].i2cAccessSem);
		return ERROR;
	}
	semGive(gBspM8641I2cFunc[busNo].i2cAccessSem);
	return OK;
}


STATUS rioSwPortModeSet(UINT8 channelNo,UINT8 mode) /*0: x4 1:othet*/
{
	UINT32 data=0;
	UINT32 regAddr = 0xf20200;
	UINT32 modeCode = 0;
	
	
	if((channelNo>3)) 
	{
		printf("channelNo = %d is error\n",channelNo);
		return ERROR;
	}
	if(mode != 0)
	{
		switch(channelNo)
		{
			case 0:
			case 1:
				modeCode = 0x3;
				break;
			case 2:
			case 3:
				modeCode = 0x1;
				break;
			default: 
				printf("channelNo = %d is error\n",channelNo);
				return ERROR;
		}
	}
	if(sysI2cRead(0,0xff,regAddr,(UINT8*)&data,4,1) == ERROR) return ERROR;
	printf("read 0x%x = 0x%x\n",regAddr,data);
	data &=(~(0x3<<(channelNo*2)));
	data |= modeCode;
	printf("write 0x%x = 0x%x\n",regAddr,data);
	if(sysI2cWrite(0,0xff,regAddr,(UINT8*)&data,4,1) == ERROR) return ERROR;
	return OK;
}

static UINT8 gQuadNum[4][5] = 
{
	{0,12,16,4,8},
	{1,13,17,5,9},
	{2,14,14,6,10},
	{3,15,15,7,11},
};

static UINT8 gPllNum[10]={0,4,8,1,5,9,2,3,7,11};
static UINT8 gLanNum[10][4]=
{
	{0,1,2,3},
	{16,17,18,19},
	{32,33,34,35},
	{4,5,6,7},
	{20,21,22,23},
	{36,37,38,39},
	{8,9,10,11},
	{12,13,14,15},
	{28,29,30,31},
	{44,45,46,47}
};

static UINT8 gPortNum[10][3]=
{
	{0,12,16},
	{4,4,4},
	{8,8,8},
	{1,13,17},
	{5,5,5},
	{9,9,9},
	{2,14,14},
	{3,15,15},
	{7,7,7},
	{11,11,11}
};
static UINT8 gPortLane[18][4]=
{
	{0,1,0,0},  //0
	{4,5,4,5},  //1
	{8,9,8,9},  //2
	{12,13,12,13},//3
	{16,17,18,19}, // 4
	{20,21,22,23}, //5
	{24,25,26,27}, //6
	{28,29,30,31}, //7
	{32,33,34,35}, //8
	{36,37,38,39}, //9
	{40,41,42,43}, //10
	{44,45,46,47}, //11
	{2,2,2,2},  //12
	{6,6,6,6},  //13
	{10,11,10,11},//14
	{3,3,3,3},  //16
	{7,7,7,7}  //17
};


STATUS rioSwPortCfg(UINT8 quad,UINT8 mode) /*quad:{0:0,1,2,3},{1:0,1,2,3},{2:0,1},{3:0,1}*/
{
	UINT32 writeData=0,readData = 0;
	UINT32 regAddr = 0xF20200;
	int i;
	
	if((quad > 1)&&(mode > 1))
	{
		printf("Argument Error!\n");
		return OK;
	}
	if(sysI2cRead(0,0xff,regAddr,(UINT8*)&readData,4,1) == ERROR) return ERROR;
	printf("Read data is %x  %x\n",writeData,readData);
	
	writeData = readData &(~(0x3<<(quad*2)));
	writeData |= mode << (quad*2);
	if(sysI2cWrite(0,0xff,regAddr,(UINT8*)&writeData,4,1) == ERROR) return ERROR;
	if(sysI2cRead(0,0xff,regAddr,(UINT8*)&readData,4,1) == ERROR) return ERROR;
	printf("Read data is %x  %x\n",writeData,readData);
	
//	if(quad == 2)
//	{
//		regAddr = 0x15c + 0x20*gQuadNum[2][0];
//		if(sysI2cRead(0,0xff,regAddr,(UINT8*)&readData,4,1) == ERROR) return ERROR;
//		writeData = readData &0xfcffffff;
//		writeData |= 0x02000000;
//		if(sysI2cWrite(0,0xff,regAddr,(UINT8*)&writeData,4,1) == ERROR) return ERROR;
//		if(sysI2cRead(0,0xff,regAddr,(UINT8*)&readData,4,1) == ERROR) return ERROR;
//		printf("new Read data is %x  %x\n",writeData,readData);
//	}
//	for(i=0; i<5; i++)
//	{
//		if((gQuadNum[quad][i] ==2)||(gQuadNum[quad][i] ==11)||(gQuadNum[quad][i] ==7)||(gQuadNum[quad][i] ==17))
//		{
//			continue;
//		}
//		regAddr = 0x15c + 0x20*gQuadNum[quad][i];
//		if(sysI2cRead(0,0xff,regAddr,(UINT8*)&readData,4,1) == ERROR) return ERROR;
//		writeData = readData &0xff7fffff;
//		writeData |= 0x00800000;
//		if(sysI2cWrite(0,0xff,regAddr,(UINT8*)&writeData,4,1) == ERROR) return ERROR;
//		if(sysI2cRead(0,0xff,regAddr,(UINT8*)&readData,4,1) == ERROR) return ERROR;
//		printf("port %d new Read data is %x  %x\n",gQuadNum[quad][i],writeData,readData);
//	}
	
}


STATUS rioPortReset(UINT8 port)
{
	UINT32 writeData=0,readData = 0;
	UINT32 regAddr;
	
	regAddr = 0x1044 +0x40*port;
	writeData=0x00000000;
	
	if(sysI2cWrite(0,0xff,regAddr,(UINT8*)&writeData,4,1) == ERROR)
	{
		return ERROR;
	}
	regAddr = 0x106c +0x40*port;
	writeData=0x00000000;
	if(sysI2cWrite(0,0xff,regAddr,(UINT8*)&writeData,4,1) == ERROR)
	{
		return ERROR;
	}
	printf("1\n");
	regAddr = 0xFF8010 +0x100*gPortLane[port][0];
	writeData=0x00000000;
	if(sysI2cWrite(0,0xff,regAddr,(UINT8*)&writeData,4,1) == ERROR)
	{
		return ERROR;
	}
	printf("2\n");
	regAddr = 0xFF8010 +0x100*gPortLane[port][1];
	writeData=0x00000000;
	if(sysI2cWrite(0,0xff,regAddr,(UINT8*)&writeData,4,1) == ERROR)
	{
		return ERROR;
	}
	printf("3\n");
	regAddr = 0xFF8010 +0x100*gPortLane[port][2];
	writeData=0x00000000;
	if(sysI2cWrite(0,0xff,regAddr,(UINT8*)&writeData,4,1) == ERROR)
	{
		return ERROR;
	}
	printf("4\n");
	regAddr = 0xFF8010 +0x100*gPortLane[port][3];
	writeData=0x00000000;
	if(sysI2cWrite(0,0xff,regAddr,(UINT8*)&writeData,4,1) == ERROR)
	{
		return ERROR;
	}
	printf("5\n");
	regAddr = 0x15c +0x20*port;
	if(sysI2cRead(0,0xff,regAddr,(UINT8*)&readData,4,1) == ERROR)
	{
		return ERROR;
	}
	printf("6\n");
	writeData=readData&0xfffffff1;
	if(sysI2cWrite(0,0xff,regAddr,(UINT8*)&writeData,4,1) == ERROR)
	{
		return ERROR;
	}
	regAddr = 0x148+0x20*port;
	writeData=0x80000000;
	if(sysI2cWrite(0,0xff,regAddr,(UINT8*)&writeData,4,1) == ERROR)
	{
		return ERROR;
	}
	regAddr = 0x140+0x20*port;
	writeData=0x00000003;
	if(sysI2cWrite(0,0xff,regAddr,(UINT8*)&writeData,4,1) == ERROR)
	{
		return ERROR;
	}
	
	regAddr = 0x144+0x20*port;

	if(sysI2cRead(0,0xff,regAddr,(UINT8*)&readData,4,1) == ERROR)
	{
		return ERROR;
	}
	printf("8\n");
	regAddr = 0xf20300;
	writeData=0x80000000|(1<<port);
	if(sysI2cWrite(0,0xff,regAddr,(UINT8*)&writeData,4,1) == ERROR)
	{
		return ERROR;
	}
	
	regAddr = 0x140+0x20*port;
	writeData=0x00000004;
	if(sysI2cWrite(0,0xff,regAddr,(UINT8*)&writeData,4,1) == ERROR)
	{
		return ERROR;
	}
}

STATUS rioSwSpeedSet(UINT8 channelNo,UINT8 mode)/*3:reserved 4:3.125,5:6.25,0:1.25,1:2.5,2:5.0*/
{
	UINT32 writeData=0,readData=0,pllMode = 0;
	UINT32 regAddr;
	UINT8 i;
	UINT8 pllNum = gPllNum[channelNo] ,lanNum;
	

	
	if((pllMode&0x01) == 1)
	{
		regAddr = 0xff0000 + 0x10*pllNum;
		writeData = mode/3;
		if(sysI2cWrite(0,0xff,regAddr,(UINT8*)&writeData,4,1) == ERROR) return ERROR;
		printf("I2c Write:%d 0x%x = 0x%x\n",pllNum,regAddr,writeData);
		if(sysI2cRead(0,0xff,regAddr,(UINT8*)&readData,4,1) == ERROR) return ERROR;
		printf("I2c Read:%d 0x%x = 0x%x\n",pllNum,regAddr,readData);
	}
	
	for(i=0;i<4;i++)
	{
		lanNum = gLanNum[channelNo][i];
		regAddr = 0xff8000 + 0x100*lanNum;
		if(sysI2cRead(0,0xff,regAddr,(UINT8*)&readData,4,1) == ERROR) return ERROR;
	//	printf("I2c Read: 0x%x = 0x%x\n",regAddr,readData);
		writeData = readData &0xfffffffe;
		if(sysI2cWrite(0,0xff,regAddr,(UINT8*)&writeData,4,1) == ERROR) return ERROR;
	//	printf("I2c Write: 0x%x = 0x%x\n",regAddr,writeData);
		writeData = readData &(~(0xf<<1));
		writeData |=(/*(1<<0)|*/((mode%3)<<1)|((mode%3)<<3));
		if(sysI2cWrite(0,0xff,regAddr,(UINT8*)&writeData,4,1) == ERROR) return ERROR;
	//	printf("I2c Write: 0x%x = 0x%x\n",regAddr,writeData);
	}
	
	if((pllMode&0x01) == 0)
	{
		regAddr = 0xff0000 + 0x10*pllNum;
		writeData = mode/3;
		if(sysI2cWrite(0,0xff,regAddr,(UINT8*)&writeData,4,1) == ERROR) return ERROR;
	//	printf("I2c Write: 0x%x = 0x%x\n",regAddr,writeData);
	}
	
	regAddr = 0xff0000 + 0x10*pllNum;
	if(sysI2cRead(0,0xff,regAddr,(UINT8*)&pllMode,4,1) == ERROR) return ERROR;
	
	if(pllMode ==0)	printf("Pll %d select 5.0/2.5/1.25Gb\n",pllNum);
	else printf("Pll %d select 6.25/3.125Gb\n",pllNum);
	
	return OK;
}

STATUS rioSwPllPortReset(UINT8 channelNo)
{
	UINT32 writeData=0,i,readData;
	UINT32 regAddr = 0xf20300;
	
	for(i=0;i<3;i++) writeData |=(1<<gPortNum[channelNo][i]) ;
	writeData |=((1<<(gPllNum[channelNo]+18))|(1<<31));
	if(sysI2cWrite(0,0xff,regAddr,(UINT8*)&writeData,4,1) == ERROR) return ERROR;
	if(sysI2cRead(0,0xff,regAddr,(UINT8*)&readData,4,1) == ERROR) return ERROR;
	//printf("I2c Read: 0x%x = 0x%x\n",regAddr,readData);
	return OK;
}

//STATUS rioSwQuadPortReset(UINT8 quad)
//{
//	UINT32 writeData=0,i,readData;
//	UINT32 regAddr = 0xf20300;
//
//	for(i=0;i<5;i++) writeData |=(1<<gQuadNum[quad][i]) ;
//	writeData |=(1<<31);
//	if(sysI2cWrite(0,0xff,regAddr,(UINT8*)&writeData,4,1) == ERROR) return ERROR;
//	if(sysI2cRead(0,0xff,regAddr,(UINT8*)&readData,4,1) == ERROR) return ERROR;
////	for(i=0;i<5;i++)
////	{
////		rioPortReset(gQuadNum[quad][i]);
////		taskDelay(1);
////	}
//	//printf("I2c Read: 0x%x = 0x%x\n",regAddr,readData);
//	return OK;
//}

STATUS rioSwQuadPortReset(UINT8 quad)
{
	UINT32 writeData=0,i,readData;
	UINT32 regAddr = 0xf20300;
	UINT32 cmdValue = 0x3;

	for(i=0;i<5;i++)
	{
		if(sysI2cWrite(0,0xff,0x140+0x20*gQuadNum[quad][i],(UINT8*)&cmdValue,4,1)== ERROR) return ERROR;
		if(sysI2cRead(0,0xff,0x140+0x20*gQuadNum[quad][i],(UINT8*)&readData,4,1) == ERROR) return ERROR;
		printf("I2c Read: 0x%x = 0x%x\n",0x140+0x20*gQuadNum[quad][i],readData);
		writeData |=(1<<gQuadNum[quad][i]) ;
	}
	writeData |=(1<<31);
	if(sysI2cWrite(0,0xff,regAddr,(UINT8*)&writeData,4,1) == ERROR) return ERROR;
	if(sysI2cRead(0,0xff,regAddr,(UINT8*)&readData,4,1) == ERROR) return ERROR;
	
	cmdValue = 0x4;
	for(i=0;i<5;i++)
	{
		if(sysI2cWrite(0,0xff,0x140+0x20*gQuadNum[quad][i],(UINT8*)&cmdValue,4,1)== ERROR) return ERROR;
		if(sysI2cRead(0,0xff,0x140+0x20*gQuadNum[quad][i],(UINT8*)&readData,4,1) == ERROR) return ERROR;
//		printf("I2c Read: 0x%x = 0x%x\n",0x140+0x20*gQuadNum[quad][i],readData);
		taskDelay(1);
	}
//	for(i=0;i<5;i++)
//	{
//		rioPortReset(gQuadNum[quad][i]);
//		taskDelay(1);
//	}
	//printf("I2c Read: 0x%x = 0x%x\n",regAddr,readData);
	return OK;
}

