
#ifndef _bsp_H_
#define _bsp_H_

//#include "gpio.h"

struct gpioInfo
{
	int gpioIndex;
	int mode;
	BOOL value;
	int intmode;
	VOIDFUNCPTR pIsr;
	void * arg;
};

extern 	unsigned long 	inet_addr (const char *);
extern  UINT32	vxDecGet (void); 


extern STATUS sysGetTime(UINT32 *timeH,UINT32 *timeL);
extern float sysSpendTimeGet(UINT32 startTimeH,UINT32 startTimeL,UINT32 endTimeH,UINT32 endTimeL);

extern STATUS	taskDelay 	(int ticks);
extern void	vxTimeBaseGet (UINT32 * pTbu, UINT32 * pTbl);
extern int sysGetCpuID(void);
extern STATUS sysLedSet(UINT8 ledNo,UINT8 status) ;


extern void bspGpioIntClean(int intPin);
extern STATUS bspGpioModeSet(UINT8 pinIndex,int mode);
extern STATUS bspGpioStatusSet(UINT8 pinIndex,int pinValue);		
extern int bspGpioStatusGet(UINT8 pinIndex);
//IMPORT VXB_DEVICE_ID bspGpioIntInstFind();
//extern STATUS bspGpioIntEnable(int intPin,VOIDFUNCPTR gpioIntIsr);
//extern STATUS bspGpioIntDisable(UINT8 intPin,VOIDFUNCPTR gpioIntIsr);
//extern STATUS bspGpioIntConnect(UINT8 intPin,VOIDFUNCPTR gpioIntIsr);
//extern STATUS bspGpioIntModeSet(UINT8 intPin,int mode);
extern STATUS sysI2cWrite(UINT8 busNo,UINT8 deviceAddr,UINT8 regAddr,UINT8 data);
extern STATUS sysI2cRead(UINT8 busNo,UINT8 deviceAddr,UINT8 regAddr,UINT8 *pData,UINT32 count);
extern STATUS sysI2cRegWrite(UINT8 iicUnit,UINT8 regOffset,UINT8 regValue);
extern STATUS sysI2cRegRead(UINT8 iicUnit,UINT8 regOffset,UINT8 *regValue);
extern FILE * ffopen(const char * file,const char * mode);
extern int ffclose(FAST FILE * fp ) ;
extern int ffwrite(const void * buf,size_t size, size_t count,FILE * fp);


#endif
