#ifndef _vxbm8641gpio_h
#define _vxbm8641gpio_h

/* includes */

#include <vxWorks.h>
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>

/* typedefs */

#define VXB_VER_4_2_1		0x00040201

#define GPIO_MAX_NUM 32

#define GPIO_WRITE         	1
#define GPIO_READ          	2
#define GPIO_DIR_SET      	 	3
#define GPIO_INT_SETUP     	4
#if 0
#define GPIO_INT_MASK_A    	5
#define GPIO_INT_MASK_B    	6
#define GPIO_INT_LEVEL_SET    	7
#define PULSE_INT_SETUP   		8
#define PULSE_INT_MASK   		9
#endif
#define GPIO_INT_CLEAR			10

typedef struct _gpioIsrprio
{
	UINT8 cause;/*IO NUM (0-31)*/
	int intPrio;
}GPIO_ISR_PRIO;


typedef struct _gpioIsrEntry {
    VOIDFUNCPTR userISR;   /* A user ISR pointer.*/
    int         arg;       /* An argument to interrupt ISR. */
    int         prio;      /* An interrupt priority. */
} GPIO_ISR_ENTRY;


typedef struct _gpioDrvCtrl
{
	SEM_ID gpioAccessSem;
	UINT32 regBase;
	UINT32 ioNum;
	GPIO_ISR_ENTRY gpioIsrEntry[GPIO_MAX_NUM];
	GPIO_ISR_PRIO gpioIsrPrio[GPIO_MAX_NUM];
	VOIDFUNCPTR pulseIntCallBack;
}GPIO_DRV_CTRL;



typedef struct _gpioDrvCtrlInfo
{
	UINT8 cause;/*IO NUM (0-31)*/
	UINT32 *value;/*IO value 0/1*/
	VOIDFUNCPTR intCallBack;
	int intParameter;
	int intPrio;
	
}GPIO_DRV_CTRL_INFO;


#define GPIO_INT_MASK_SET_REG_A 0x0
#define GPIO_INT_MASK_SET_REG_B 0x4
#define GPIO_INT_FLAG_REG 0x8
#define GPIO_INT_LEVEL_REG 0xc
#define GPIO_DIR_SET_REG 0x10
#define GPIO_OUTPUT_DATA_REG 0x14
#define GPIO_INPUT_DATA_REG 0x18
#define GPIO_INT_ENABLE_A 0x1c
#define GPIO_INT_ENABLE_B 0x20

#define PULSE_INT_MASK_SET_REG 0x100
#define PULSE_INT_FLAG_REG 0x104


#endif
