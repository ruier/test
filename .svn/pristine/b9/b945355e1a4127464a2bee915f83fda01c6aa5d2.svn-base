#ifndef __PPC_API_H__
#define __PPC_API_H__

#include <stdio.h>
#include <vxWorks.h>
#include <vsbConfig.h>
#include <vme.h>
#include <memLib.h>
#include <cacheLib.h>
#include <sysLib.h>
#include <string.h>
#include <intLib.h>
#include <logLib.h>
#include <taskLib.h>
#include <vxLib.h>
#include <tyLib.h>
#include <vmLib.h>
#include <arch/ppc/mmu603Lib.h>
#include <arch/ppc/vxPpcLib.h>
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/util/vxbDmaLib.h>
#include <vxbTimerLib.h>
#include <private/cpcLibP.h>
#include <logLib.h>
#include <cpuset.h>
#include <vxCpuLib.h>
#include <assert.h>
#include <semLib.h>


#define DWORD  UINT32
#define WORD   UINT16


#define  I2C_BUS_LOCAL  0   /* I2C bus 0 */
#define  MAX_I2C_BUS    2   /* I2C bus number*/



#define WRITE_REG16(addr,data)      (*(volatile UINT16 *)(addr) = data)  
#define READ_REG16(addr)            (*(volatile UINT16 *)(addr))

#define WRITE_REG32(addr,data)       (*(volatile UINT32 *)(addr) = data)  
#define READ_REG32(addr)             (*(volatile UINT32 *)(addr))

#define FSL_WRITE_REG(addr,data)     (*(volatile UINT32 *)(addr) = data)  
#define FSL_READ_REG(addr)           (*(volatile UINT32 *)(addr))

#define M8641_WRITE_REG(addr,data)   (*(volatile UINT32 *)(addr) = data)  
#define M8641_READ_REG(addr)         (*(volatile UINT32 *)(addr))

#define M8641_READ32_REG(addr)       (*(volatile UINT32 *)(addr))
#define M8641_WRITE32_REG(addr,data) (*(volatile UINT32 *)(addr) = data)




#define UART_PSMR_TPM_ODD      0x0000   /*odd parity mode (Tx)*/
#define UART_PSMR_TPM_EVEN     0x0002   /*even parity mode (Tx)*/

#define UART_PSMR_RPM_ODD      0x0000   /*odd parity mode (Rx)*/
#define UART_PSMR_RPM_EVEN     0x0008   /*even parity mode (Rx)*/

#define UART_PSMR_PEN          0x0010   /*parity enable */
#define UART_PSMR_PDEN         0x0000   /*parity disable */

#define UART_PSMR_CL_5BIT      0x0000   /*5 bit character length */
#define UART_PSMR_CL_6BIT      0x1000   /*6 bit character length */
#define UART_PSMR_CL_7BIT      0x2000   /*7 bit character length */
#define UART_PSMR_CL_8BIT      0x3000   /*8 bit character length */

#define UART_PSMR_TWO_STOP     0x4000   /*2 bit stop length */
#define UART_PSMR_ONE_STOP     0x0000   /*1 bit stop length */

#define UART_PSMR_FLC          0x8000   /*flow control */

#define CPUA_WDG_REG  (M8641_CS1_BASE + 0x1c)
#define CPUB_WDG_REG  (M8641_CS1_BASE + 0x20)

#define TIMER_MODE_CONTINUE      0
#define TIMER_MODE_ONCE          1
#define TIMER_MODE_SEM_CONTINUE  2
#define TIMER_MODE_SEM_ONCE      3

extern SEM_ID sysTimerSemId1;
extern SEM_ID sysTimerSemId2;
extern SEM_ID sysTimerSemId3;
extern SEM_ID sysTimerSemId4;

extern SEM_ID semPrintSem;


#define sysPrintf(fmt,arg...) \
            do{\
              semTake(semPrintSem,WAIT_FOREVER);\
              printf(fmt,##arg);\
              semGive(semPrintSem); \
            }while(0)
			
#define SysPrintf sysPrintf
#define Printf_Ex sysPrintf

typedef struct tagRTC_DATE_TIME_S
{   
	UINT32	msec;     /*0~1000*/
	UINT32   second;   /*从0点开始累积一整天*/
	UINT32   minute;   /*0-59*/
	UINT32   hour;     /* 0-23*/
	UINT32   date;     /*1-31*/
	UINT32   month;    /*1-12*/
	UINT32   year;     /*00-99*/
} RTC_DATE_TIME;

typedef struct _VER_AND_DATE_
{
	UINT16 Major_Ver:8;/* 1-9*/
	UINT16 Minor_Ver:8;/* 1-9*/
    UINT16 year:7;     /* 0-99*/
    UINT16 month:4;    /*1-12*/
    UINT16 day:5;      /*1-31*/
} VER_AND_DATE;

typedef UINT32 (*USR_TIMER_ISR_F)(UINT32 id,UINT32 arg1,UINT32 arg2,UINT32 arg3);

extern DWORD  sysUartInit (DWORD uartCtlPort, DWORD uartModeSel);
extern DWORD  sysUartOpen (DWORD uartCtlPort, DWORD uartModeSel);

#define SysUartInit sysUartInit
#define SysUartOpen sysUartOpen

extern DWORD  sysTimerOpen (DWORD id, DWORD mode);
extern DWORD  sysTimerClose (DWORD id);
extern DWORD  usrTimerDelay (DWORD id, DWORD delayTime,VOIDFUNCPTR usrISR,
                             DWORD arg1,DWORD arg2, DWORD arg3);

#define SysTimerOpen  sysTimerOpen
#define SysTimerClose sysTimerClose
#define UsrTimerDelay usrTimerDelay


extern STATUS  sysRtcInit (void);
extern STATUS  sysRtcSet( RTC_DATE_TIME *tm);
extern STATUS  sysRtcGet( RTC_DATE_TIME *tm);
extern void   sysRtcShutdown(void);

#define SysRtcInit     sysRtcInit
#define SysRtcSet      sysRtcSet
#define SysRtcGet      sysRtcGet
#define SysRtcShutdown sysRtcShutdown


extern void vxTimeBaseSet (DWORD  pTbu, DWORD  pTbl);
extern void vxTimeBaseGet (DWORD * pTbu, DWORD * pTbl);
extern void sysPositionGet(UINT8 *pCaseIdx, UINT8 *pSlotIdx, UINT8 *pCpuIdx);

#define SysPositionGet sysPositionGet

extern DWORD sysWatchDogInit (DWORD ms);
extern void  sysWatchDogCounterStop (void);
extern void  sysWatchDogCounterReload (void);

#define SysWatchDogInit          sysWatchDogInit
#define SysWatchDogCounterStop   sysWatchDogCounterStop
#define SysWatchDogCounterReload sysWatchDogCounterReload


extern STATUS  intOpenInit(UINT8 intNum,UINT8 intMode,VOIDFUNCPTR intIsr,DWORD intArg);
extern void    intMaskEnable(UINT8 intNum);
extern void    intMaskDisable(UINT8 intNum);

#define IntOpenInit          intOpenInit
#define IntMaskEnable        intMaskEnable
#define IntMaskDisable       intMaskDisable

extern DWORD sysNetIpSet  (DWORD netCtlPort, UINT8 *ipAddr, DWORD ipMaskAddr);
extern DWORD sysNetMacSet (DWORD netCtlPort, UINT8 *ipMacAddr);
extern DWORD sysNetMacGet (DWORD netCtlPort, UINT8 *ipMacAddr);

#define SysNetIpSet        sysNetIpSet
#define SysNetMacSet       sysNetMacSet
#define SysNetMacGet       sysNetMacGet


extern DWORD tffsInit(void);
extern DWORD usrTffsConfig(DWORD diskNum, DWORD attr, UINT8* devName);
extern void  sysTffsFormat(void);

extern void  sysOSProgram(DWORD len,UINT8 *data);
extern void  sysRomFSProgram(DWORD len,UINT8 *data);
extern void  sysUserData(DWORD ID,DWORD len,UINT8 *data);
extern UINT8* getUserData(DWORD ID);

#define SysOSProgram      sysOSProgram
#define SysRomFSProgram   sysRomFSProgram
#define SysUserData       sysUserData
#define GetUserData       getUserData


extern void sysGetBootVersionAndDate(VER_AND_DATE *p);
extern void sysGetOsVersionAndDate(VER_AND_DATE *p);

#define SysGetBootVersionAndDate   sysGetBootVersionAndDate
#define SysGetOsVersionAndDate     sysGetOsVersionAndDate

#define SYS_FAULT_CPU         (1 << 0)
#define SYS_FAULT_FPGA        (1 << 1)
#define SYS_FAULT_DDR         (1 << 2)
#define SYS_FAULT_FLASH       (1 << 3)
#define SYS_FAULT_APP         (1 << 4)
#define SYS_FAULT_RS232_1     (1 << 5)
#define SYS_FAULT_RS232_2     (1 << 6)
#define SYS_FAULT_RS422_1     (1 << 7)
#define SYS_FAULT_RS422_2     (1 << 8)
#define SYS_FAULT_RS422_3     (1 << 9)
#define SYS_FAULT_RS422_4     (1 << 10)
#define SYS_FAULT_ETH_1       (1 << 11)
#define SYS_FAULT_ETH_2       (1 << 12)
#define SYS_FAULT_SRIO        (1 << 13)
#define SYS_FAULT_28V         (1 << 14)
#define SYS_FAULT_5V          (1 << 15)
#define SYS_FAULT_3_3V        (1 << 16)
#define SYS_FAULT_RTC         (1 << 17)

#define EPIC_VEC_EXT_IRQ0     0
#define EPIC_MODE_LOW_LEV     1
#define EPIC_MODE_HIGH_LEV    2
#define EPIC_MODE_LOW_EDG     3
#define EPIC_MODE_HIGH_EDG    4
#define EPIC_REG(x)           (CCSBAR + 0x50000 + x*0x20)
#define EPIC_FPGA_STATUS      (0xe2000000 + 0x38)
#define EPIC_FPGA_MASK        (0xe2000000 + 0x3c)

extern WORD sysGetCPUBoardFactory(void);
extern DWORD sysGetCPUBoardBitCode(void);
extern WORD sysGetAGState(void);

#define SysGetCPUBoardFactory     sysGetCPUBoardFactory
#define SysGetCPUBoardBitCode     sysGetCPUBoardBitCode
#define SysGetAGState             sysGetAGState


extern DWORD sysDwordRead(DWORD addr);
extern void  sysDwordWrite(DWORD addr, DWORD val);

#define SysDwordRead  sysDwordRead
#define SysDwordWrite sysDwordWrite

extern DWORD rioSysInit(UINT32 vuiPortID, void (*dbHook)(UINT8 srcID,UINT8 dstID, WORD data), 
            void (*msgHook) (UINT8 mailBox,UINT8 * msgPtr, WORD len),int flag);
extern DWORD rioDmaTransfer(UINT32 vuiPortID, UINT8 chan, DWORD srcAddr, DWORD dstAddr, DWORD dataSize, 
                     DWORD phyNextDescPtr);
extern VOID rioSendDoorbell(UINT32 vuiPortID,UINT8 targetID,UINT16 DBData);
extern void rioSendMsg(UINT32 vuiPortID,UINT8 mailBox, UINT8 dstID, UINT8 *msgPtr, WORD len);
extern UINT32 rioMapOutb(UINT32 vuiPortID, UINT32 localAddr, UINT32 srioAddr, UINT32 size, UINT32 dstID, UINT32 flags);
extern DWORD rioMapInb(UINT32 vuiPortID, DWORD lStart, DWORD rStart, DWORD size, DWORD flags);
extern DWORD rioDeviceNum(UINT32 vuiPortID, DWORD p[]);
extern DWORD rioSearchDevice(UINT32 vuiPortID, UINT32 dstID);
extern DWORD rioDmaNread(UINT32 vuiPortID, UINT8 chan,DWORD localAddr,DWORD  rioAddr, DWORD byteCnt);
extern DWORD rioDmaNwrite(UINT32 vuiPortID, UINT8 chan,DWORD localAddr,DWORD rioAddr, DWORD byteCnt);
extern DWORD rioSetBaseDeviceId(UINT32 vuiPortID,WORD dstPortID,UINT8 hopCount,DWORD newDevID);
extern DWORD rioGetBaseDeviceId (UINT8 localPort, DWORD *devID);
extern DWORD testnwrite(UINT32 vuiPortID, UINT32 dstAddr, UINT32 size);
extern DWORD testnread(UINT32 vuiPortID, DWORD srcAddr,DWORD dstAddr, DWORD size);
extern void  rioDeviceNumPrint();
extern DWORD ddrDma(UINT8 chan,char  *pcSrcAddr,char *pcDestAddr,UINT32 uiSize);


#define M8641_WRITE_REG(addr,data)    (*(volatile UINT32 *)(addr) = data)  
#define M8641_READ_REG(addr)          (*(volatile UINT32 *)(addr))


extern STATUS hostLoadFile(char *pucFileName);
extern INT8 sysGetCPUTemp(UINT8 vucBusNo);
extern INT8 sysGetFPGATemp(UINT8 vucBusNo);

extern STATUS fpgaFileLoad(char *pucFileName,UINT32 uiPart);

#endif


