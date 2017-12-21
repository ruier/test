
#include "sysCfg.h"


#include "board_spec_src/mac/macApi.c"
#include "board_spec_src/phy/phyApi.c"



#ifndef COMPILE_BOOTROM


#include "board_spec_src/srio/rioSrc.c"

/*****************************************************************************
 函 数 名  : i2cDevCfg
 功能描述  : I2C设备较多,且每个初始化均需延时,单独起一个任务进行管理
 输入参数  : 
 输出参数  : 
 返回值    :
*****************************************************************************/
void i2cDevCfg()
{
	
	i2cModuleInit(I2C_BUS_LOCAL);
	taskDelay(sysClkRateGet() / 5);

	/*初始化电压监控*/
	sysVoltInit();
	taskDelay(sysClkRateGet() / 5);

	/*初始化 IDT时钟芯片*/
#if 1
	idtSetClk10MIn10MOut();
#else
	idtSetCLK1ToQ456();
#endif
	taskDelay(sysClkRateGet() / 5);

	/*初始化温度监控*/
	tempMonitorInit();
}




/*****************************************************************************
 函 数 名  : sysBSPInit
 功能描述  : 上电自启动的任务
 输入参数  : 
 输出参数  : 
 返回值    :
*****************************************************************************/
VOID sysBSPInit()
{
#ifndef  COMPILE_BOOTROM
	taskSpawn("i2cDevCfg", 100,0,0x10000,(FUNCPTR)i2cDevCfg, 0,0,0,0,0,0,0,0,0,0);  
#endif

	sysPrintInit();

	sysGetVer();
}

#endif



