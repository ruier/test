
/******************************************************/
/*本文件用于PPC BSP 与  库文件中 函数、定义等的关联
 *不同BSP，本文件中内容可能会有不同
 */


#include "p2020.h"
#include "config.h"
#include "sysCfg.h"



#define BOARD_ID_LEN            25   /*VPX6-SP2020.1:123456-001, 多保留1个byte用于\0*/

/*nvram 中板号的存放位置*/
#define NV_BOARD_ID_OFFSET    0x200     /*notice dongyang 注意此值要确认*/


#define FLASH_SECTOR_BYTE     0x20000

#define NET_STRING_LEN    128

#define M8641_BLK_SIZE          0x20000


#define FPGA_VER_LEN    16      /* 单位是字节*/




#define WRITE_REG32(addr,data)    (*(volatile UINT32 *)(addr) = data)  
#define READ_REG32(addr)          (*(volatile UINT32 *)(addr))

#define WRITE_REG16(addr,data)    (*(volatile UINT16 *)(addr) = data)  
#define READ_REG16(addr)          (*(volatile UINT16 *)(addr))





/****************此处的宏定义均为该项目不用, 但公共库编译时所需的**********/
#define FLASH_MODE_OFFSET       0x300       /*地面、控制模式切换*/

#define CPU_A_ID      0
#define CPU_B_ID      1

#define MCU_VER_LEN     32      /* 该单位是字节*/
#define PPC_V6_VER_LEN  32      /* 该单位是双字节*/
#define FPGA_VER_LEN    16      /* 单位是字节*/

#define FLASH2_BASE_ADRS         0x90000000    
#define FLASH2_TOTAL_SIZE        0x08000000

#define M8641_CS1_BASE    0xe0000000
#define M8641_CS3_BASE    0xe2000000


#define RTC_SET_REG_1  (M8641_CS1_BASE + 0x30)
#define RTC_SET_REG_2  (M8641_CS1_BASE + 0x28)

#define RTC_GET_REG_1  (M8641_CS1_BASE + 0x34)
#define RTC_GET_REG_2  (M8641_CS1_BASE + 0x2c)


#define M8641_FPGA_ADDR          0x90000000     /* 存储FPGA的Flash 的首地址*/
#define M8641_FPGA_LEN           (16*1024*1024) /* 8 个part 每个part 大小为16M*/
#define M8641_FPGA_ERASE_LEN     (15*1024*1024) /* FPGA 镜像数据大小的最大值*/
#define M8641_FPGA_LEN_BYTE      4
#define M8641_FPGA_LEN_OFFSET    0

#define FLASH_BOOT_OFFSET       0xffe00000      /*NVRAM的起始地址*/

#define M8641_USER_ADDR          0xfe000000
#define M8641_USER_LEN           (2*1024*1024) /*8 part 2M*/
#define M8641_USER_PART          8

#define M8641_APP_ADDR           0xff000000
#define M8641_APP_LEN           (10*1024*1024)

#define M8641_MAX_OS_LEN       (4*1024*1024)
#define M8641_OS_ADDR           0xffa00000
#define M8641_OS_SIZE_OFFSET    0xc
#define M8641_MAX_BOOT_LEN     (1*1024*1024)

/*new add*/
#define M8641_OS_ADDR_ADD        0xff500000  /* 存储vxWorks的镜像*/
#define M8641_OS_ADDR_FLAG	     0xff900000  /* 存储镜像选择标识*/
#define M8641_BOOT_ADDR          0xfff00000  /* bootrom的基地址*/



#define M8641_BOOT_ADDR         0xfff00000
#define FLASH_PROG_PRINT         1
#define FLASH_ERASE_PRINT        2
#define FlASH_PRINT_HALF         3

/********************************************************************************/


#define _WRS_VX_SMP


#ifdef INCLUDE_TFFS
#define INCLUDE_WAMDMTD_LIB
#endif


#define INCLUDE_422_LIB

#define INCLUDE_429_LIB




#define INCLUDE_FPGA_LIB
#define INCLUDE_RTC_LIB
#define INCLUDE_FPGA_TIMER_LIB
#define INCLUDE_RESET_LIB



#define  INCLUDE_I2C_LIB
#define  INCLUDE_TEMP_LIB
#define  INCLUDE_VOLT_LIB
#define  INCLUDE_IDTCLK_LIB


#define INCLUDE_CAN_LIB


#if 1
#define INCLUDE_SDCARD_LIB

#define INCLUDE_UART_LIB

#define INCLUDE_RIO_LIB

#define INCLUDE_DMA_LIB
#endif


