#ifndef _vxbm8641SynUart_h
#define _vxbm8641SynUart_h

/* includes */

#include <vxWorks.h>
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include <isrDeferLib.h>

/* typedefs */

#define VXB_VER_4_2_1		0x00040201



#define SYN_UART_CHAIN_NUM  5

#define SYN_UART_BAUD_SET        1
#define SYN_UART_RCV_INT_SETUP   2
#define SYN_UART_WRITE           3
#define SYN_UART_READ          	 4
#define SYN_UART_STATUS_GET      5
#define SYN_UART_FUNC_GET		 6
#define SYN_UART_RCV_INT_MASK	 7
#define SYN_UART_EMPTY_INT_MASK	 8
#define SYN_UART_EMPTY_INT_SETUP 9
#define SYN_UART_CHAIN_ENABLE   10
#define SYN_UART_CHAIN_DISABLE  11
#define SYN_UART_BAUD_GET       12
#define SYN_UART_SWITCH_CPU     13



#define RCV_INT_F 0
#define EMPTY_INT_F 1

typedef void (*P_INT_USR_ISR)(UINT32);

typedef struct _synUartDrvCtrl
{
	SEM_ID synUartAccessSem;
	UINT32 unitNumber;
	UINT32 regBase;
	UINT32 dirFlag;    /*1:RCV 0£ºSEND*/
	P_INT_USR_ISR userISR[2];
}SYN_UART_DRV_CTRL;



typedef struct _synUartDrvCtrlInfo
{
	UINT32* value;/*IO value 0/1*/
	VOIDFUNCPTR intCallBack;
}SYN_UART_DRV_CTRL_INFO;






#define SYN_UART_REG_INTERVAL 0x100

#define SYN_UART_BAUD_REG(x) 		(0x310+x*SYN_UART_REG_INTERVAL)
#define SYN_UART_INT_FLAG_REG(x)  	(0x30c+x*SYN_UART_REG_INTERVAL)
#define SYN_UART_DATA_REG(x)        (0x300+x*SYN_UART_REG_INTERVAL)
#define SYN_UART_STATUS_REG(x)      (0x308+x*SYN_UART_REG_INTERVAL)
#define SYN_UART_ENABLE_REG(x)      (0x304+x*SYN_UART_REG_INTERVAL)
#define SYN_UART_INT_ENABLE_REG(x)      (0x314+x*SYN_UART_REG_INTERVAL)
#define SYN_UART_SWITCH_CPU_REG(x)      (0x18+x*4)

#endif
