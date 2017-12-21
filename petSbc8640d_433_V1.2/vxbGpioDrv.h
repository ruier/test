#ifndef _vxbGpioDrvH
#define _vxbGpioDrvH


   
#define 	GPIO_CTRL_REG		 0xCC
#define		GPIO_STATUS			 0xBC
#define		GPIO_INT_CTRL		 0xDC


//#define 	GPIO_IVEC_MAX		15
//#define 	GPIO_MAX_ALL_IRQS   15

#define GPIO_MODE_SET		0
#define GPIO_MODE_GET		1
#define GPIO_INTMODE_SET	2
#define GPIO_VALUE_SET		3
#define GPIO_VALUE_GET		4
#define GPIO_INT_CONNECT	5
#define GPIO_INT_DISCONNECT	6
#define GPIO_INT_ENABLE		7
#define GPIO_INT_DISABLE	8

struct sGpioInfo
{
	int gpioIndex;
	int mode;
	BOOL value;
	int intmode;
	VOIDFUNCPTR pIsr;
	void * arg;
};




#endif
