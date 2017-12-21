/* includes */

#include <vxWorks.h>
#include <taskLib.h>			/* for taskSpawn() and taskDelay() */
#include <sysLib.h>			/* for sysClkRateGet() */
#include <stdio.h>			/* for printf() */
#include <logLib.h>			/* for logMsg() */
#include <string.h>			/* for bfill() */
#include <vxBusLib.h>			/* for vxbDeviceMethodRun()
					 * and  VXB_DEVICE_ID
					 */

#include <driverControl.h>		/* for struct vxbDriverControl */
#include <hwif/vxbus/vxbMethodDecl.h>	/* for DEVMETHOD_CALL(driverControl) */
#include <vxbm8641I2c.h>			/* for struct wrsampleDriverControl
					 * and WRSAMPLE_GET_SAMPLE_FUNCS					 */
#include <cpuset.h>


#define MAX6884_DBG_ON
#undef MAX6884_DBG_ON

#ifdef MAX6884_DBG_ON
#define MAX6884_DBG_MSG(fmt,a,b,c,d,e,f)  logMsg(fmt,a,b,c,d,e,f)
#undef LOCAL
#define LOCAL

#else /* MAX6884_DBG_ON */
#define MAX6884_DBG_MSG(fmt,a,b,c,d,e,f)
#endif /* MAX6884_DBG_ON */


#define  IN1_UV_THRESHOLD 0
#define  IN2_UV_THRESHOLD 1
#define  IN3_UV_THRESHOLD 2
#define  IN4_UV_THRESHOLD 3
#define  IN5_UV_THRESHOLD 4
#define  IN6_UV_THRESHOLD 5
#define  IN1_OV_THRESHOLD 6
#define  IN2_OV_THRESHOLD 7
#define  IN3_OV_THRESHOLD 8
#define  IN4_OV_THRESHOLD 9
#define  IN5_OV_THRESHOLD 10
#define  IN6_OV_THRESHOLD 11

#define max6884ShowEeprom 		0
#define max6884ShowReg    		1
#define max6884ShowThreshold 	2
#define max6884ClearEeprom 		3
#define max6884UpdateEeprom 	4
#define max6884ShowVoltage  	5
#define max6884UpdateReg    	6


#define deviceAddrMax6884 0xa4



struct reg_data{
	UINT8 reg;
	UINT8 data;
	};

struct reg_data pEepromUpdate[20] = {
	/*设置第一、第二限值*********************/
	{0x80,40},/*IN1*/
	{0x81,45},/*IN2*/
	{0x82,50},/*IN3*/
	{0x83,65},/*IN4*/
	{0x84,188},/*IN5*/
	{0x85,107},/*IN6*/
	{0x86,50},
	{0x87,55},
	{0x88,60},
	{0x89,79},
	{0x8a,212},
	{0x8b,123},
	/*****************************************/
	{0x8e,0x3f},/*IN1~IN6启动过压检测*/
	{0x8f,0x1f},/*IN1~IN5精档，IN6粗档*/
	{0x90,0x00},/*普通阻值模式*/
	{0x91,0xfc},/*复位中断：选通IN1~IN6初级欠压报警*/
	{0x92,0x01},/*复位中断：关WDO；复位信号25μs响应，低电平有效*/
	{0x93,0xfc},/*过欠压中断:选通IN1~IN6初级欠压报警*/
	{0x94,0x01},/*过欠压中断:复位信号25μs响应，低电平有效*/
	{0x95,0x01},/*禁止看门狗*/
};



struct reg_data pRegUpdate[20] = {
	{0x00,40},/*IN1*/
	{0x01,45},/*IN2*/
	{0x02,50},/*IN3*/
	{0x03,65},/*IN4*/
	{0x04,188},/*IN5*/
	{0x05,107},/*IN6*/
	{0x06,50},
	{0x07,55},
	{0x08,60},
	{0x09,79},
	{0x0a,212},
	{0x0b,123},
	/*****************************************/
	{0x0e,0x3f},/*IN1~IN6启动过压检测*/
	{0x0f,0x1f},/*IN1~IN5精档，IN6粗档*/
	{0x10,0x00},/*普通阻值模式*/
	{0x11,0xfc},/*复位中断：选通IN1~IN6初级欠压报警*/
	{0x12,0x01},/*复位中断：关WDO；复位信号25μs响应，低电平有效*/
	{0x13,0xfc},/*过欠压中断:选通IN1~IN6初级欠压报警*/
	{0x14,0x01},/*过欠压中断:复位信号25μs响应，低电平有效*/
	{0x15,0x01},/*禁止看门狗*/
};

struct reg_data pEepromShow[21] = {/*作为EEPROM观察内存*/
	{0x80,0x0},
	{0x81,0x0},
	{0x82,0x0},
	{0x83,0x0},
	{0x84,0x0},
	{0x85,0x0},
	{0x86,0x0},
	{0x87,0x0},
	{0x88,0x0},
	{0x89,0x0},
	{0x8a,0x0},
	{0x8b,0x0},
	{0x8e,0x0},
	{0x8f,0x0},
	{0x90,0x0},
	{0x91,0x0},
	{0x92,0x0},
	{0x93,0x0},
	{0x94,0x0},
	{0x95,0x0},
	{0x96,0x0}/* 7:extern reference-1; 6:NC; 5:VCC powered-1; 4:NC; 3:configuration unlcoked-0; 2:NC; 1: -enable write-0; 0:enable write-0 */
};

struct reg_data pRegShow[24] = {/*作为EEPROM观察内存*/
	{0x00,0x0},
	{0x01,0x0},
	{0x02,0x0},
	{0x03,0x0},
	{0x04,0x0},
	{0x05,0x0},
	{0x06,0x0},
	{0x07,0x0},
	{0x08,0x0},
	{0x09,0x0},
	{0x0a,0x0},
	{0x0b,0x0},/*12*/
	
	{0x0e,0x0},
	{0x0f,0x0},
	{0x10,0x0},
	{0x11,0x0},
	{0x12,0x0},
	{0x13,0x0},
	{0x14,0x0},
	{0x15,0x0},
	{0x16,0x0},/*9*/
	
	{0x28,0x0},/*初级电压监测故障标志*/
	{0x29,0x0},/*次级电压监测故障标志*/
	{0x2a,0x0} /*RESET#，UV#/OV#，及WDO#故障标志*/
};


STATUS max6884ShowAllThreshold(struct i2cDrvCtrl *pI2cdrvCtrl,UINT8 deviceAddress);
STATUS max6884ShowAllVoltage(struct i2cDrvCtrl *pI2cdrvCtrl,UINT8 deviceAddress);
STATUS  max6884SetThreshold(struct i2cDrvCtrl *pI2cdrvCtrl,UINT8 deviceAddress,UINT8 whichThreshold);

#if 1
//extern VXB_DEVICE_ID vxbInstByNameFind(char * instName,int unit);

STATUS max6884MethodGet(struct i2cDrvCtrl *pI2cdrvCtrl ,int num )
{
	 struct i2cDrvCtrl *pDrvCtrl=pI2cdrvCtrl; 
	 struct vxbDriverControl ctrl;	
	 VXB_DEVICE_ID pDev;
	 FUNCPTR     handler;
	 
	 ctrl.driverName  = "i2c";		
	 ctrl.drvCtrlCmd  = I2C_GET_READ_FUNCS;	
	 ctrl.drvCtrlInfo = pDrvCtrl;	
	
	 if((num != 0)&&(num != 1))  logMsg("the arg of fuction :num is fault !!!!!\n",1,2,3,4,5,6);
	 
	 /*find Inst ID*/
	 pDev=vxbInstByNameFind("i2c",num);

	 /* find handler */
	 handler = vxbDevMethodGet(pDev, DEVMETHOD_CALL(driverControl));
	 if( handler == NULL )
	 {
		 logMsg("max6884MethodGet:vxbDevMethodGet != OK!!!!\n",1,2,3,4,5,6);
		 return (ERROR);
	 }
	 
	 if((*handler)(pDev,&ctrl ) != OK)
	 {	
		 logMsg("max6884MethodGet:handle != OK!!!!\n",1,2,3,4,5,6);
		 return (ERROR);
	 }
	
	 ctrl.drvCtrlCmd  = I2C_GET_WRITE_FUNCS;	/* commnad */	 
	 if((*handler)(pDev,&ctrl ) != OK)
	 {	
		 logMsg("max6884MethodGet:handle != OK!!!!\n",1,2,3,4,5,6);
		 return (ERROR);
	 }  
	 /*logMsg("pDrvCtrl:baseAddr=0x%x\n",pDrvCtrl->baseAddr,2,3,4,5,6);*/
	 
	 return (OK);
}

#else

STATUS max6884MethodGet(struct i2cDrvCtrl *pI2cdrvCtrl )
{
	 struct i2cDrvCtrl *pDrvCtrl=pI2cdrvCtrl; 
	 struct vxbDriverControl ctrl;	
	 ctrl.driverName  = "i2c";		
	 ctrl.drvCtrlCmd  = I2C_GET_READ_FUNCS;	
	 ctrl.drvCtrlInfo = pDrvCtrl;	
	 if (vxbDevMethodRun (DEVMETHOD_CALL(driverControl), &ctrl) != OK)
	 {	
		 logMsg("i2cTest:vxbDevMethodRun != OK!!!!\n",1,2,3,4,5,6);
		 return (ERROR);
	 }
	 ctrl.drvCtrlCmd  = I2C_GET_WRITE_FUNCS;	/* commnad */
	 if (vxbDevMethodRun (DEVMETHOD_CALL(driverControl), &ctrl) != OK)
	 {	
		 logMsg("i2cTest:vxbDevMethodRun != OK!!!!\n",1,2,3,4,5,6);
		 return (ERROR);
	 }  
	 return (OK);
}
	  
#endif


STATUS max6884Init(struct i2cDrvCtrl *pI2cdrvCtrl,UINT8 deviceAddress)
{
	struct i2cDrvCtrl *pDrvCtrl=pI2cdrvCtrl; 
	UINT32 baseAddr;
	
	baseAddr=pDrvCtrl->baseAddr;	
	 
	 
	MAX6884_DBG_MSG("max6884Init final\n",1,2,3,4,5,6);
	return OK;
	error:
		logMsg("max6884Init:fail!!\n",1,2,3,4,5,6);
		return ERROR;
}




STATUS max6884Handle(struct i2cDrvCtrl *pI2cdrvCtrl,UINT8 deviceAddress,UINT8 cmd)
{
	struct i2cDrvCtrl *pDrvCtrl=pI2cdrvCtrl; 
	UINT32 baseAddr;
	UINT8 i,data;
	
	baseAddr=pDrvCtrl->baseAddr;
	if(baseAddr == 0) return ERROR;
	switch(cmd)
	{
			case max6884ShowEeprom:
				{
					for(i=0;i<21;i++)
					{
						if(pDrvCtrl->i2cRead(baseAddr,deviceAddress,pEepromShow[i].reg,1,&pEepromShow[i].data) == ERROR) goto error;
						printf("reg[0x%x]:0x%x\n",pEepromShow[i].reg,pEepromShow[i].data);
						pEepromShow[i].data=0;
					}
					break;
				}

			case max6884ShowReg:
				{
					for(i=0;i<24;i++)
					{
						if(pDrvCtrl->i2cRead(baseAddr,deviceAddress,pRegShow[i].reg,1,&pRegShow[i].data) == ERROR) goto error;
						printf("reg[0x%x]:0x%x\n",pRegShow[i].reg,pRegShow[i].data);
						pRegShow[i].data=0;
					}
					break;
				}
			case max6884ClearEeprom:
				{
					for(i=0;i<20;i++)
					{
						data=0;
						if(pDrvCtrl->i2cWrite(baseAddr,deviceAddress,pEepromUpdate[i].reg,1,(UINT8*)&data) == ERROR) goto error;
					}
					break;
				}
			case max6884UpdateEeprom:
				{
					for(i=0;i<20;i++)
						if(pDrvCtrl->i2cWrite(baseAddr,deviceAddress,pEepromUpdate[i].reg,1,&pEepromUpdate[i].data) == ERROR) goto error;
					break;
				}
			case max6884UpdateReg:
				{
					for(i=0;i<20;i++)
						if(pDrvCtrl->i2cWrite(baseAddr,deviceAddress,pRegUpdate[i].reg,1,&pRegUpdate[i].data) == ERROR) goto error;
					break;
				}
				
			case max6884ShowVoltage:
				{
						if(max6884ShowAllVoltage(pDrvCtrl,deviceAddress)==ERROR) goto error;
						break;
				}
			case max6884ShowThreshold:
				{
						if(max6884ShowAllThreshold(pDrvCtrl,deviceAddress)==ERROR) goto error;
						break;
				}
			default:
				goto error;
	}
	return OK;
	error:
		logMsg("max6884Handle:fail!!\n",1,2,3,4,5,6);
		return ERROR;
}
		
STATUS max6884ShowStatus(struct i2cDrvCtrl *pI2cdrvCtrl,UINT8 deviceAddress,UINT8 data[3])
{
	struct i2cDrvCtrl *pDrvCtrl=pI2cdrvCtrl; 
	UINT32 baseAddr;
	
	baseAddr=pDrvCtrl->baseAddr;
	if(baseAddr == 0) return ERROR;
		
	if(pDrvCtrl->i2cRead(baseAddr,deviceAddress,0x28,1,&data[0]) == ERROR) goto error;
	if(pDrvCtrl->i2cRead(baseAddr,deviceAddress,0x29,1,&data[1]) == ERROR) goto error;
	if(pDrvCtrl->i2cRead(baseAddr,deviceAddress,0x2a,1,&data[2]) == ERROR) goto error;
	printf(" max6884Status is  1:0x%x   2:0x%x   3:0x%x\n",data[0],data[1],data[2]);
	return OK;
error:
	logMsg("max6884ShowStatus FAIL!!!\n",1,2,3,4,5,6);
	return ERROR;
	
}		
		

	
	
STATUS max6884ShowAllThreshold(struct i2cDrvCtrl *pI2cdrvCtrl,UINT8 deviceAddress)
{

	struct i2cDrvCtrl *pDrvCtrl=pI2cdrvCtrl; 
	UINT32 baseAddr;
	UINT8 i,data[2];
	
	baseAddr=pDrvCtrl->baseAddr;
	if(baseAddr == 0) return ERROR;
	printf(" NO    UV    OV \n");
	for(i=1;i<7;i++)
	{
		if(pDrvCtrl->i2cRead(baseAddr,deviceAddress,i-1,1,&data[0]) == ERROR) goto error;
		if(pDrvCtrl->i2cRead(baseAddr,deviceAddress,i-1+6,1,&data[1]) == ERROR) goto error;
		if(i==6) printf(" IN%d:%5d%5d\n",i,data[0]*20+1000,data[1]*20+1000);
		else printf(" IN%d:%5d%5d\n",i,data[0]*10+500,data[1]*10+500);
	}
	printf("\r\n");
	return OK;
	error:
		return ERROR;
}
STATUS max6884ShowAllVoltage(struct i2cDrvCtrl *pI2cdrvCtrl,UINT8 deviceAddress)
{
	struct i2cDrvCtrl *pDrvCtrl=pI2cdrvCtrl; 
	UINT32 baseAddr;
	UINT8 i,data[2];
	UINT16 temp=0;
	
	baseAddr=pDrvCtrl->baseAddr;
	if(baseAddr == 0) return ERROR;
	/*printf("max6884ShowAllVoltage--baseAddr=0x%x\n",baseAddr);*/
	for(i=1;i<7;i++)
	{
		data[0]=data[1]=0;
		if(pDrvCtrl->i2cRead(baseAddr,deviceAddress,i*2+0x16,1,&data[0]) == ERROR) goto error;
		if(pDrvCtrl->i2cRead(baseAddr,deviceAddress,i*2+1+0x16,1,&data[1]) == ERROR) goto error;
		
		temp=0;
		temp |= data[0];
		temp <<=2;
		temp |=(data[1]&0x03);
			
		if(i==6) printf(" IN%d:%5.2f\n",i,temp*7.32);
		else printf(" IN%d:%5.2f\n",i,temp*3.66);
	}
	printf("\r\n");
	return OK;
	error:
		return ERROR;
	
}


void max6884Test(void)
{
	struct i2cDrvCtrl *pI2cdrvCtrl;
	if(max6884MethodGet(pI2cdrvCtrl,1)==ERROR) goto error;

#if 0
	
	max6884Handle(pI2cdrvCtrl,deviceAddrMax6884,max6884ShowReg);
	max6884Handle(pI2cdrvCtrl,deviceAddrMax6884,max6884UpdateReg);
	
	max6884Handle(pI2cdrvCtrl,deviceAddrMax6884,max6884ShowReg);

	max6884Handle(pI2cdrvCtrl,deviceAddrMax6884,max6884ShowEeprom);
	max6884Handle(pI2cdrvCtrl,deviceAddrMax6884,max6884UpdateEeprom);

	max6884Handle(pI2cdrvCtrl,deviceAddrMax6884,max6884ShowEeprom);

#endif

	if(max6884ShowAllThreshold(pI2cdrvCtrl,deviceAddrMax6884)==ERROR) goto error;

	if(max6884ShowAllVoltage(pI2cdrvCtrl,deviceAddrMax6884)==ERROR) goto error;
	
	return;
	error:
		logMsg("max6884Test fail!\n",1,2,3,4,5,6);
}
