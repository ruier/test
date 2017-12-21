#ifndef __SYS_USR_H__ 

#define __SYS_USR_H__

#include <semLib.h>


#ifndef COMPILE_BOOTROM
#define  INCLUDE_PCI_BUS
#define  INCLUDE_PCI_AUTOCONF
#define  INCLUDE_DISK_UTIL
#define  INCLUDE_IPFTPS
#define  INCLUDE_IFCONFIG
#define  INCLUDE_FSL_DMA
#define  INCLUDE_FILE_SYSTEM
#define  INCLUDE_FSL_HOST_LOAD
#endif




#define INCLUDE_NETWRS_ROUTELIB
#ifdef  INCLUDE_TFFS
#define INCLUDE_TFFS_DOSFS
#define FLASH_TFFS_ADRS             0xf8000000
#define INCLUDE_TFFS_SHOW
#undef INCLUDE_TFFS_MOUNT
#undef  INCLUDE_FS_EVENT_UTIL  
#undef  INCLUDE_FS_MONITOR   
#undef  INCLUDE_ERF            
#undef  INCLUDE_DEVICE_MANAGER 
#define INCLUDE_XBD        
#endif



#define EPIC_VEC_EXT_IRQ0     0
#define EPIC_MODE_LOW_LEV     1
#define EPIC_MODE_HIGH_LEV    2
#define EPIC_MODE_LOW_EDG     3
#define EPIC_MODE_HIGH_EDG    4
#define EPIC_REG(x)           (CCSBAR + 0x50000 + x*0x20)

#if 0
/* PCI support is available just not defined by default */
#ifdef INCLUDE_PCI_BUS
#define DRV_PCIBUS_M85XX
#define DRV_RESOURCE_M85XXCCSR
#define INCLUDE_PCI_BUS_AUTOCONF
#define INCLUDE_PCI_OLD_CONFIG_ROUTINES
#define INCLUDE_PCI_BUS_SHOW

#define EPIC_EX_DFT_POLAR	EPIC_INT_ACT_LOW

#define VXB_EPICINTCTLR_DBG_ON


#define PCI_MMU_TLB_SZ           _MMU_TLB_SZ_256M

#if (PCI_MEM_SIZE == 0x04000000)
#define PCI_MEM_SIZE_MASK        PCI_ATTR_WS_64M
#endif

#if (PCI_MEMIO_SIZE == 0x04000000)
#define PCI_MEMIO_SIZE_MASK      PCI_ATTR_WS_64M
#endif

#if (PCI_IO_SIZE == 0x04000000)
#define PCI_IO_SIZE_MASK         PCI_ATTR_WS_64M
#endif

#define PCI_BRIDGE_PIMMR_BASE_ADRS  CCSBAR

/* just 1:1 mapping */

/* PCI 1 */

#define CPU_PCI_MEM_ADRS          PCI_MEM_ADRS
#define CPU_PCI_MEMIO_ADRS        PCI_MEMIO_ADRS
#define CPU_PCI_IO_ADRS           PCI_IO_ADRS

/* PCI 2 */

#define CPU_PCI_MEM_ADRS2         PCI_MEM_ADRS2
#define CPU_PCI_MEMIO_ADRS2       PCI_MEMIO_ADRS2
#define CPU_PCI_IO_ADRS2          PCI_IO_ADRS2

/* PCI Express */

#define CPU_PCI_MEM_ADRS3         PCI_MEM_ADRS3
#define CPU_PCI_MEMIO_ADRS3       PCI_MEMIO_ADRS3
#define CPU_PCI_IO_ADRS3          PCI_IO_ADRS3

#define CPU_PCI_MEM_SIZE          PCI_MEM_SIZE
#define CPU_PCI_MEMIO_SIZE        PCI_MEMIO_SIZE
#define CPU_PCI_IO_SIZE           PCI_IO_SIZE

/* CPU from PCI bus */

#define PCI_MSTR_MEM_BUS           0x00000000
#define PCI_MSTR_MEM_SIZE          0x40000000 /* 1G */

/* CPU Address that is visible from PCI */

#define PCI_LOCAL_MEM_BUS         0x00000000
#define PCI_LOCAL_MEM_SIZE        PCI_MSTR_MEM_SIZE
#define PCI_LOCAL_MEM_SIZE_MASK   PCI_ATTR_WS_1G

#ifndef PCI_CFG_TYPE
#   ifdef INCLUDE_PCI_AUTOCONF
#      define PCI_CFG_TYPE PCI_CFG_AUTO
#   else
#      define PCI_CFG_TYPE PCI_CFG_FORCE
#   endif /* INCLUDE_PCI_AUTOCONF */
#endif /* PCI_CFG_TYPE */
#endif

#define FLASH_BOOT_OFFSET              0xffe00000
#define FSL_BOOT_ADDR                  0xfff00000
#define FSL_BLK_SIZE                   0x20000
#define FLASH_PROG_PRINT               1
#define FLASH_ERASE_PRINT              2
#define FSL_MAX_BOOT_LEN               (1*1024*1024)
#define FLASH_SECTOR_BYTE              0x10000
#endif



#define NET_STRING_LEN        128

#define FSL_WRITE_REG(addr,data)    (*(volatile UINT32 *)(addr) = data)  
#define FSL_READ_REG(addr)          (*(volatile UINT32 *)(addr))

#define FSL_WRITE_BYTE(addr,data)   (*(volatile UINT8 *)(addr) = data)  
#define FSL_READ_BYTE(addr)         (*(volatile UINT8 *)(addr))

#define TIMER_GTBCR0                (CCSBAR + 0x41110)
#define TIMER_TCRB                  (CCSBAR + 0x41300)





#endif

