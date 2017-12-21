
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
    UINT8   century;  /*0-39*/
} RTC_DATE_TIME;

/*#define P5020_RTC_NVRAM_ADDR     SYS_OR1_BASE_ADRS*/
#define P5020_RTC_NVRAM_ADDR     SYS_OR4_BASE_ADRS

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

#define RTC_FPGA_CTREG      	(P5020_RTC_NVRAM_ADDR + 0xB0)
#define RTC_FPGA_WRITE_LOWREG   (P5020_RTC_NVRAM_ADDR + 0xB4)
#define RTC_FPGA_WRITE_HIREG    (P5020_RTC_NVRAM_ADDR + 0xB8)
#define RTC_FPGA_READ_LOWREG    (P5020_RTC_NVRAM_ADDR + 0xBC)
#define RTC_FPGA_READ_HIREG    	(P5020_RTC_NVRAM_ADDR + 0xC0)
#define RTC_FPGA_BUSY_REG    	(P5020_RTC_NVRAM_ADDR + 0xC4)
#define RTC_FPGA_LED_CTREG    (P5020_RTC_NVRAM_ADDR + 0xc8)

#define RTC_FPGA_CTRREG        	(P5020_RTC_NVRAM_ADDR + 0xB0)

#define CPU_WDG_REG            (SYS_OR4_BASE_ADRS + 0x1c)


#define		   RM_LED_FLAG_CLOSE		  0x0
#define		   RM_LED_FLAG_OPEN		  	  0x1
#define		   RM_LED_FLAG_BLINK		  0x2

#define RM_M8548_WRITE_REG(addr,data)    (*(volatile UINT32 *)(addr) = data)  
#define RM_M8548_READ_REG(addr)          (*(volatile UINT32 *)(addr))
extern INT32 sysGetFpgaTime(RTC_DATE_TIME *tm);
extern INT32 sysSetFpgaTime(RTC_DATE_TIME *tm);




#endif


