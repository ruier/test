/* ads827x/config.h - Motorola ADS827x-PCI board configuration header */

/*
 * Copyright (c) 2003-2010, 2012, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01v,30may13,d_l  Uprev the BSP_REV to "6.9/2"
01u,10jun12,l_z  Remove RAM_LOW/HIGH_ADRS. (WIND00348963)
01t,15apr10,clx  Update BSP version for vxWorks 6.9
01s,10jun09,wqi  Update revision.
01r,20aug08,d_l  break default mac(ENET_DEFAULT) to three bytes.(WIND00121398)
01q,25apr08,l_g  add vxBus support
01p,21mmi07,mmi  remove legacy power management define
01o,22feb07,rec  WIND00071187 WIND00088466
01n,16feb07,rec  vxWorks 6.5 update
01m,10aug06,dtr  Update rev.
01l,29mar06,dtr  Remove SNOOP_ENABLE as default as breaks FCC for bootloader
                 SPR#119614.
01k,21sep05,dtr  Include TIMESTAMP and AUX_CLK by default.
01j,08sep05,dtr  Add support for MAC address storage.
01i,05apr05,dtr  Add support for SNOOP.
01h,30mar05,dtr  Update revision.
01g,13jun05,pcm  removed INCLUDE_DOSFS
01f,12oct04,dtr  Revision change.
01e,19feb04,dtr  Fix HW RST CONF word.
01d,28jan04,dtr  Modify FLASH defines.
01c,08jan04,dtr  Adding INCLUDE_SECURITY_ENGINE.
01b,06jan04,dtr  Change PCI_MEM_MASK_SIZE to match LOCAL_MEM_SIZE.
01a,18dec03,dtr  adapted from ads826x config.h
*/

/*
This file contains the configuration parameters for the
Motorola MPC8272 ADS board.
*/

#ifndef INCconfigh
#define INCconfigh

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/* 
 BSP version/revision identification, should be placed 
 before #include "configAll.h".
*/

#define BSP_VERSION        "6.9"       /* vxWorks 6.9 */
#define BSP_REV            "/2"        /* 0 for first revision */
#define BSP_NAME           ads827x

#define SYS_MODEL "Motorola MPC82"

/* Define Clock Speed and source  */

#define FREQ_33_MHZ        33000000
#define FREQ_40_MHZ        40000000
#define FREQ_66_MHZ        66000000
#define FREQ_100_MHZ       100000000
/*
 * This define must be set to the value of the resonant oscillator
 * inserted in position U16 of the ADS8272 board.  Choose from above
 * list.
 */
#define OSCILLATOR_FREQ    FREQ_100_MHZ
 
#define DIV_FACT_1     0
#define DIV_FACT_16    1

#define BRGCLK_DIV_FACTOR    DIV_FACT_16

#define M8260_BRGC_DIVISOR    BRGCLK_DIV_FACTOR

#include "configAll.h"

#define DEFAULT_BOOT_LINE \
"motfcc(0,0)host:target/config/ads827x/vxWorks h=92.0.0.1 e=92.0.0.2 u=vxworks pw=vxworks tn=ads827x"

#define INCLUDE_MMU_BASIC

#ifdef  INCLUDE_MMU_BASIC
#   define USER_I_MMU_ENABLE
#   define USER_D_MMU_ENABLE
#endif

#define INCLUDE_CACHE_SUPPORT

#ifdef  INCLUDE_CACHE_SUPPORT
#   define USER_D_CACHE_ENABLE
/* Does nothing about copyback vs writethrough in h/w, must use sysPhysMemDesc */
#   undef  USER_D_CACHE_MODE
/* Suggest use SNOOP for vxWorks image if MMU turned on 
 * (CACHE_COPYBACK | CACHE_SNOOP_ENABLE) 
 */
#   define USER_D_CACHE_MODE  (CACHE_COPYBACK)

#   define USER_I_CACHE_ENABLE
#   undef  USER_I_CACHE_MODE
#   define USER_I_CACHE_MODE  CACHE_COPYBACK
#endif

/* Number of TTY definition */

#undef    NUM_TTY
#define   NUM_TTY        N_SIO_CHANNELS    /* defined in ads827x.h */


/* optional TrueFFS support */

#ifdef INCLUDE_TFFS
#    define INCLUDE_SHOW_ROUTINES /* show routines for system facilities*/
#endif


/* clock rates */

#define SYS_CLK_RATE_MIN    1    /* minimum system clock rate */
#define SYS_CLK_RATE_MAX    8000    /* maximum system clock rate */
#define AUX_CLK_RATE_MIN    1    /* minimum auxiliary clock rate */
#define AUX_CLK_RATE_MAX    8000    /* maximum auxiliary clock rate */

#define INCLUDE_TIMESTAMP
#define INCLUDE_AUX_CLK
/*
 * DRAM refresh frequency - This macro defines the DRAM refresh frequency.
 * e.i: A DRAM with 1024 rows to refresh in 16ms:
 * DRAM_REFRESH_FREQ = 1024/ 16E-3 = 64E3 hz
 */

#define DRAM_REFRESH_FREQ    64000    /* 64 kHz */

#define M8260_SCCR_PCI_MODE_BIT 0x80

/* add on-chip drivers */
#define INCLUDE_SECURITY_ENGINE

#define INCLUDE_PCI        /* include PCI library support */

#ifdef  INCLUDE_PCI
#define INCLUDE_PCI_AUTOCONF

/*
CPU Addr                    PCI Addr
PCI_LOCAL_MEM_BUS    ------------------------- PCI_MSTR_MEM_BUS
            -                -
                  -                -
PCI_LOCAL_MEM_BUS +    ------------------------- PCI_MSTR_MEM_BUS +
PCI_LOCAL_MEM_SIZE    ----IMMR                  - PCI_MSTR_MEM_SIZE
                  -                -
                   -                -----PIMMR
                   -                -
                   -                -
CPU_PCI_MEM_ADRS    ------------------------- PCI_MEM_ADRS
                       -            -
                  -            -
CPU_PCI_MEMIO_ADRS    ------------------------- PCI_MEMIO_ADRS
                      -            -
                   -                   -
CPU_PCI_IO_ADRS        ------------------------- PCI_IO_ADRS
                   -                   -
                   -                  -
CPU_PCI_IO_ADRS +    ------------------------- PCI_IO_ADRS +
CPU_PCI_IO_SIZE        -                 - PCI_IO_SIZE
                   -                 -
                   -                 -
                   -                 -
                   -------------------------- 4GBytes
*/



/* for custom sysPciAutoConfig.c */


/* PCI based addresses */
#define PCI_MEM_ADRS          0x50000000
#define PCI_MEM_SIZE          0x01000000         /*16MB */
#define PCI_MEM_SIZE_MASK     PCI_SIZE_MASK_16M /* This should match PCI_MEM_SIZE */
#define PCI_MEMIO_ADRS        0x60000000
#define PCI_MEMIO_SIZE        0x01000000         /*16MB */
#define PCI_MEMIO_SIZE_MASK   PCI_SIZE_MASK_16M    /* This should match PCI_MEMIO_SIZE */
#define PCI_IO_ADRS           0x70000000
#define PCI_IO_SIZE           0x00100000    /* IO Space is not available */


/* CPU based addresses */
#define CPU_PCI_MEM_ADRS           0x80000000
#define CPU_PCI_MEM_SIZE           PCI_MEM_SIZE
#define CPU_PCI_MEMIO_ADRS         0x90000000
#define CPU_PCI_MEMIO_SIZE         PCI_MEMIO_SIZE
#define CPU_PCI_IO_ADRS            0xa0000000
#define CPU_PCI_IO_SIZE            PCI_IO_SIZE


/* CPU from PCI bus */
#define PCI_MSTR_MEM_BUS                0x00000000
#define PCI_MSTR_MEM_SIZE               PCI_LOCAL_MEM_SIZE
#define PCI_BRIDGE_PIMMR_BASE_ADRS      0x40000000


/* CPU Address that is visible from PCI */
#define PCI_LOCAL_MEM_BUS     LOCAL_MEM_LOCAL_ADRS
#define PCI_LOCAL_MEM_SIZE    LOCAL_MEM_SIZE

/* This should match LOCAL_MEM_SIZE - if not statically defined buffers
   need to be used for PCI devices */
#define PCI_MSTR_MEM_SIZE_MASK    PCI_SIZE_MASK_64M

#ifndef PCI_CFG_TYPE
#    ifdef INCLUDE_PCI_AUTOCONF
#        define PCI_CFG_TYPE PCI_CFG_AUTO
#    else
#        define PCI_CFG_TYPE PCI_CFG_FORCE
#    endif /* INCLUDE_PCI_AUTOCONF */
#endif /* PCI_CFG_TYPE */

#endif /* INCLUDE_PCI */

/*
 * MAC Address configuration
 * The board doesn't have hardwired MAC addresses, so they must be initialized
 * and stored in NvRAM.
 */

#define MAC_ADRS_LEN    6   /* 6 bytes in MAC address */
#define MAX_MAC_ADRS    2   /* number of MAC addresses to save in EEPROM */

#define ETHERNET_MAC_HANDLER        /* bootrom 'M' command sets MAC address */

#define MAX_MAC_DEVS    2 /* two network devices (fcc, scc) */
#ifdef BOOTAPP
#define INCLUDE_BOOT_ETH_MAC_HANDLER
#endif

#define WR_ENET0          0x00  /* WR specific portion of MAC (MSB->LSB) */
#define WR_ENET1          0xA0
#define WR_ENET2          0x1E

#define CUST_ENET3_0      0xA0  /* Customer portion of MAC address */
#define CUST_ENET3_1      0xA1
#define CUST_ENET4        0xAA
#define CUST_ENET5        0xA0

/* default mac address */

#define ENET_DEFAULT0 WR_ENET0
#define ENET_DEFAULT1 WR_ENET1
#define ENET_DEFAULT2 WR_ENET2

#undef INCLUDE_PCI_DMA
#undef INCLUDE_PCI_ERROR_HANDLING
#undef PCI_BRIDGE_READ_ERRATA_WORKAROUND

#ifndef INCLUDE_PCI
#  undef INCLUDE_PCI_DMA
#  undef INCLUDE_PCI_ERROR_HANDLING
#  undef INCLUDE_FEI_END
#  undef PCI_BRIDGE_READ_ERRATA_WORKAROUND
#endif

#define INCLUDE_FLASH

#ifdef INCLUDE_FLASH
#define FLASH_ADRS_8M            0xff800000    /* base address of 8Meg flash */
#define FLASH_ADRS_16M           0xff000000    /* base address of 16Meg flash */
#define FLASH_ADRS_32M           0xfe000000    /* base address of 32Meg flash */
#define FLASH_BASE_ADRS          FLASH_ADRS_8M  /* this board has 8Meg flash */
#define FLASH_ADRS               0xffec0000
#define FLASH_BLOCK_SIZE         0x40000
#define FLASH_BOOTLINE_OFFSET    0x0
#define FLASH_WIDTH              4
#define FLASH_SIZE               0x800000 /* 8 Mbytes total flash size */
#define FLASH_SIZE_WRITEABLE     4096        /* reasonable write size */
#define SYS_FLASH_TYPE           FLASH_28F016    /* Intel flash device type */
#endif

/* Memory addresses */

#define LOCAL_MEM_SIZE          0x04000000        /* 64 Mbyte memory available */
#define LOCAL_MEM_LOCAL_ADRS    0x00000000        /* Base of RAM */



/*
 * The constants ROM_TEXT_ADRS, ROM_SIZE, and RAM_HIGH_ADRS are defined
 * in config.h, MakeSkel, Makefile, and Makefile.*
 * All definitions for these constants must be identical.
 */

#define ROM_BASE_ADRS   0xfff00000        /* base address of ROM */
#define ROM_TEXT_ADRS   ROM_BASE_ADRS + 0x100
#define ROM_SIZE        0x100000         /* ROM space */
#define ROM_WARM_ADRS   (ROM_TEXT_ADRS+8) /* warm reboot entry */

#define USER_RESERVED_MEM    0x00000000    /* user reserved memory size */

/* Hard Reset Configuration Words */

#define HRCW_BYTE_0    0x0C    /* 32 bit boot port, MSR(IP) = 1 */
#define HRCW_BYTE_1    0x72    /* IMMR[0-14] = 0x0F00_0000, no L2 cache signals*/
#define HRCW_BYTE_2    0x36    /* bit[20:21] LBPC = 01, set local bus as PCI bus */
#define HRCW_BYTE_3    0x5A    /* bit[26] ALD_EN = 1 */

/* VxBus support */

#define INCLUDE_VXBUS
#define INCLUDE_VXB_CMDLINE
#define INCLUDE_PLB_BUS
#define INCLUDE_PARAM_SYS
#define INCLUDE_DMA_SYS
#define INCLUDE_HWMEM_ALLOC
#define HWMEM_POOL_SIZE   50000
#define INCLUDE_FCC_VXB_END
#define INCLUDE_VXB_CPM
#define INCLUDE_DM9161PHY
#define INCLUDE_MDIO
#define INCLUDE_MII_BUS

/*
 * The hard reset configuration word (above) defines the initial location
 * of the Internal Memory Map.  If you want to change this address during
 * bsp boot time (to match the memory map defined by the Motorola ADS8266-PCI
 * documentation) define the IMMAP_REMAP macro below.  The actual value
 * for the INTERNAL_MEM_MAP_ADDR is then defined in ads828x.h
 */
#undef IMMAP_REMAP

#include "ads827x.h"            /* include the ads826x params */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif    /* INCconfigh */
#if defined(PRJ_BUILD)
    #include "prjParams.h"
#endif

