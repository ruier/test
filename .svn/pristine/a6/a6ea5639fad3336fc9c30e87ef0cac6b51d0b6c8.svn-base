
#ifndef __USR_API_H__
#define __USR_API_H__

#define TIMER_MODE_CONTINUE      0
#define TIMER_MODE_ONCE          1
#define TIMER_MODE_SEM_CONTINUE  2
#define TIMER_MODE_SEM_ONCE      3


#define TIMER_FLAG_UNINIT      0
#define TIMER_FLAG_INITED      1
#define TIMER_FLAG_OPEN        2
#define TIMER_FLAG_CLOSE       3


#define TIMER_TFRRB          (CCSBAR + 0x420f0)
#define TIMER_GTBCRB(i)      (CCSBAR + 0x42110 + i * 0x40)
#define TIMER_GTVPRB(i)      (CCSBAR + 0x42120 + i * 0x40)
#define TIMER_GTBDRB(i)      (CCSBAR + 0x42130 + i * 0x40)
#define TIMER_TCRB           (CCSBAR + 0x42300)
#define TIMER_CNT             4


typedef struct tagM8641_TIMER_S
{
    UINT32            uiFlag;
    UINT32            uiMode;
    VOIDFUNCPTR       pfISR;
    UINT32            uiArg1;
    UINT32            uiArg2;
    UINT32            uiArg3;
    SEM_ID            semLock;
} M8641_TIMER_S;

typedef struct tagMSG_BUF
    {
    void  * buf;
    UINT32  len;
    void  * next;
    } MSG_TRAN_BUF_S;

typedef struct tagRIO_MSG_INT_INF_S
{
	MSG_TRAN_BUF_S	msgIntInf;
	SEM_ID			msgSemId;

}RIO_MSG_INT_INF_S;

#define P5020_WRITE_REG(addr,data)    (*(volatile UINT32 *)(addr) = data)  
#define P5020_READ_REG(addr)          (*(volatile UINT32 *)(addr))

#define P5020_WRITE_REG_8BIT(addr, data)  (*(volatile UINT8 *)(addr) = data)
#define P5020_READ_REG_8BIT(addr)	      (*(volatile UINT8 *)(addr))

#define P5020_WRITE_REG_16BIT(addr, data)  (*(volatile UINT16 *)(addr) = data)
#define P5020_READ_REG_16BIT(addr)	      (*(volatile UINT16 *)(addr))

extern UINT32 sysClkFreqGet (void);

typedef struct tagRTC_DATE_TIME_S
{   
    UINT8   year;     /*00-99*/
    UINT8   month;    /*1-12*/
    UINT8   date;     /*1-31*/
    UINT8   day;      /*ÐÇÆÚ¼¸*/
    UINT8   hour;     /* 0-23*/
    UINT8   minute;   /*0-59*/
    UINT8   second;   /*0-59*/
    UINT32   mSec;    /*0-1000*/
} RTC_DATE_TIME;
#undef NVRAM_RTC

#define P5020_RTC_NVRAM_ADDR     SYS_OR1_BASE_ADRS

#define RTC_OSCILLATOR          (P5020_RTC_NVRAM_ADDR + 0x1fff8)
#define RTC_RW_CONTROL          (P5020_RTC_NVRAM_ADDR + 0x1fff0)

#define RTC_TIME_YEAR           (P5020_RTC_NVRAM_ADDR + 0x1ffff)
#define RTC_TIME_MONTH          (P5020_RTC_NVRAM_ADDR + 0x1fffe)
#define RTC_TIME_DATE           (P5020_RTC_NVRAM_ADDR + 0x1fffd)
#define RTC_TIME_DAY            (P5020_RTC_NVRAM_ADDR + 0x1fffc)
#define RTC_TIME_HOUR           (P5020_RTC_NVRAM_ADDR + 0x1fffb)
#define RTC_TIME_MINUTES        (P5020_RTC_NVRAM_ADDR + 0x1fffa)
#define RTC_TIME_SECONDS        (P5020_RTC_NVRAM_ADDR + 0x1fff9)
#define RTC_TIME_CENTURY        (P5020_RTC_NVRAM_ADDR + 0x1fff1)
/***************fpga rtc reg********************/
#define P5020_RTC_FPGA_ADDR     SYS_OR4_BASE_ADRS

#define RTC_SET_REG_1  (P5020_RTC_FPGA_ADDR + 0x30)
#define RTC_SET_REG_2  (P5020_RTC_FPGA_ADDR + 0x28)

#define RTC_GET_REG_1  (P5020_RTC_FPGA_ADDR + 0x34)
#define RTC_GET_REG_2  (P5020_RTC_FPGA_ADDR + 0x2c)

/****************** end ********************/
#define CPU_WDG_REG            (SYS_OR4_BASE_ADRS + 0x1c)


#define		   RM_LED_FLAG_CLOSE		  0x0
#define		   RM_LED_FLAG_OPEN		  	  0x1
#define		   RM_LED_FLAG_BLINK		  0x2

#define RM_M8548_WRITE_REG(addr,data)    (*(volatile UINT32 *)(addr) = data)  
#define RM_M8548_READ_REG(addr)          (*(volatile UINT32 *)(addr))

extern UINT32 rioMapOutb(UINT32 localAddr, UINT32 srioAddr, UINT32 size,
                  UINT32 dstID, UINT32 flags);
extern UINT32 rioMapInb(UINT32 localAddr, UINT32 srioAddr, UINT32 size, UINT32 flags);

extern INT32 rioDmaNread(UINT8 chan,char  *pcSrcAddr,char *pcDestAddr,UINT32 uiSize);
extern INT32 rioDmaNwrite(UINT8 chan,char	*pcSrcAddr,char *pcDestAddr,UINT32 uiSize);
extern STATUS rioSendMsg(UINT16 localPort,UINT16 mailBox,UINT16 dstID,UINT8 *msgPtr,UINT32 len);
extern int rioRecMsg(UINT8 *msgPtr, UINT32 len, UINT16 *srcid, UINT32 timeout);
extern int sysCPUIdGet(void);
extern int sysRioIdGet(void);
extern UINT8 getSlotNum(void);
extern UINT8 getChasisNum(void);
#endif


