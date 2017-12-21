/* wrSbc834x/config.h - Wind River SBC834x board configuration header */

/*
 * Copyright (c) 2005-2010, 2012, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01z,14jun13,d_l  Update BSP_REV to "6.9/2"
01y,19jul12,clx  Update BSP_REV
01x,15apr10,clx  Update BSP version for vxWorks 6.9
01w,18jan10,yjw  Increase version of BSP to 2.0/10.
01v,01sep09,mry  Fix WIND00179216
01u,06jul09,z_l  undefine INCLUDE_PCI by default. (WIND00088032)
01t,14apr09,x_z  update revision.(WIND00160963)
01s,03dec08,syt  remove RAM_LOW_ADRS and RAM_HIGH_ADRS. (WIND00139819)
01r,20aug08,d_l  break default mac(ENET_DEFAULT) to three bytes.(WIND00121398)
01q,07aug08,d_l  correct SYS_CLK_FREQ to 264Mhz.(WIND00127502)
01p,16jul08,l_z  updated BSP_REV to "/8" for VxWorks 6.7
01o,13jun08,wap  Switch to new VxBus TSEC driver
01n,17sep07,h_k  removed INCLUDE_PCI_BUS_SHOW. (CQ:105028)
01m,07sep07,ami  INCLUDE_M834X_USB macro removed
01l,15aug07,mmi  remove DEFAULT_POWER_MGT_MODE
01n,09aug07,agf  inc rev
01m,09jul07,pdg  added vxbus support
01l,12jan07,b_m  WIND00065649, add SYS_MODEL define.
01k,31jan07,d_z  Increase BSP version to 2.0/6.
01j,08dec06,rec  vxWorks 6.5 update
01i,21nov06,jmt  move MAX_MAC_DEVS from usrNet.h to config.h
01h,13sep06,tfo  WLAN support for Atheros AR521X
01g,30aug06,dtr  Increment rev of BSP.
01f,08mar06,kds  updated USB and PCI support
01e,12aug05,pcm  removed INCLUDE_DOSFS from INCLUDE_TFFS auto-include list
01d,21jul05,j_b  add PCI support for gei (manual configuration)
01c,11jul05,j_b  increase LOCAL_MEM_SIZE2 for 128MB of local bus SDRAM
01b,21jun05,j_b  add NV RAM and MAC address mgmt support
01a,06jun05,kds  adapted from ads834x (rev 01e)
*/

/*
This file contains the configuration parameters for the
Wind River SBC8349 board.
*/

#ifndef INCconfigh
#define INCconfigh

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/* BSP version/revision identification, should be placed
 * before #include "configAll.h"
 */

#define BSP_VERSION     "6.9"       /* vxWorks 6.9 */
#define BSP_REV         "/2"        /* 0 for first revision */
#define BSP_NAME        wrSbc834x

#define SYS_MODEL    	"Wind River SBC834"

#include "configAll.h"

/* Define Clock Speed and source  */

#define FREQ_33_MHZ      33000000
#define FREQ_40_MHZ      40000000
#define FREQ_66_MHZ      66000000
#define FREQ_100_MHZ     100000000
#define FREQ_264_MHZ     264000000
#define FREQ_333_MHZ     333000000

/*
 * This define must be set to the value SYSCLK freq on the wrSbc834x board.
 * Choose from above list.
 */
#define OSCILLATOR_FREQ FREQ_66_MHZ
#undef REV1_0_SILICON
#define REV1_1_SILICON
#undef REV2_0_SILICON

/*
 * Since oscillator frequency is 66.000Mhz, the default values assume CSB is 
 * 264Mhz based on HRCW setting in boot image
 */

#define SYS_CLK_FREQ            FREQ_264_MHZ
#define DEC_CLK_TO_INC          4               /* # bus clks per increment */
#define DEC_CLOCK_FREQ          SYS_CLK_FREQ    /* Set to system default */
#define TIMESTAMP_HZ            SYS_CLK_FREQ

#define TPR         0x2000
#define LSRT_VALUE  0x20

/* Memory addresses */

#define LOCAL_MEM_SIZE          0x10000000      /* 256 Mbytes available */
#define LOCAL_MEM_LOCAL_ADRS    0x00000000      /* Base of RAM */
#define INCLUDE_LBC_SDRAM
#define INCLUDE_DDR_SDRAM
#define INCLUDE_SECONDARY_DRAM
/* NOTE this should match the LAWAR SIZE in romInit for the chosen SDRAM */
#define LOCAL_MEM_SIZE2             0x8000000       /* 128 Mbytes available */
#define LOCAL_MEM_LOCAL_ADRS2       0x10000000      /* Base of RAM */
#define LBC_SDRAM_LOCAL_SIZE_MASK   0xfc000000
#define LBC_SDRAM_LOCAL_ADRS        LOCAL_MEM_LOCAL_ADRS2
#define LBC_SDRAM_LOCAL_SIZE        LOCAL_MEM_SIZE2
#define DDR_SDRAM_LOCAL_ADRS        LOCAL_MEM_LOCAL_ADRS
#define DDR_SDRAM_LOCAL_SIZE        LOCAL_MEM_SIZE
#define DDR_SDRAM_LOCAL_ADRS_END   (DDR_SDRAM_LOCAL_ADRS + DDR_SDRAM_LOCAL_SIZE)

/*
 * The constants ROM_TEXT_ADRS, ROM_SIZE are defined
 * in config.h, MakeSkel, Makefile, and Makefile.*
 * All definitions for these constants must be identical.
 */

#define ROM_BASE_ADRS   0xff800000      /* base address of ROM */
#define ROM_TEXT_ADRS   0xfff00100
#define ROM_SIZE        0x100000         /* ROM space */
#define ROM_WARM_ADRS   (ROM_TEXT_ADRS+8) /* warm reboot entry */


#define USER_RESERVED_MEM    0x00000000 /* user reserved memory size */

/* define only one of the following processors */
#define  MPC8349
#undef  MPC8347 /* no secondary PCI controller */

#define DEFAULT_BOOT_LINE \
"mottsec(0,0)host:target/config/wrSbc834x/vxWorks h=92.0.0.1 e=92.0.0.2 u=vxworks pw=vxworks tn=wrSbc834x"

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
#   define USER_D_CACHE_MODE  CACHE_COPYBACK | CACHE_SNOOP_ENABLE
#   define USER_I_CACHE_ENABLE
#   undef  USER_I_CACHE_MODE
#   define USER_I_CACHE_MODE  CACHE_COPYBACK
#endif

/* Number of TTY definition */

#undef  NUM_TTY
#define NUM_TTY     N_SIO_CHANNELS  /* defined in wrSbc834x.h */

#undef   CONSOLE_TTY
#define CONSOLE_TTY  0   /* 0 = UART1/COM1, 1 = UART2/COM2 */


/* Optional timestamp support */

#undef INCLUDE_TIMESTAMP
#define INCLUDE_AUX_CLK

#undef INCLUDE_DMA

/* optional TrueFFS support */

#undef  INCLUDE_TFFS

#ifdef INCLUDE_TFFS
#endif  /* INCLUDE_TFFS */


/* clock rates */

#define SYS_CLK_RATE_MIN    1   /* minimum system clock rate */
#define SYS_CLK_RATE_MAX    8000    /* maximum system clock rate */
#define AUX_CLK_RATE_MIN    1   /* minimum auxiliary clock rate */
#define AUX_CLK_RATE_MAX    8000    /* maximum auxiliary clock rate */


/* add on-chip drivers */
#define INCLUDE_SECURITY_ENGINE  /* only support if chip has E ie 8349E */


#undef INCLUDE_PIB_SUPPORT      /* include PIB IO board support */
#define INCLUDE_PCI              /* include PCI library support */

#ifdef  INCLUDE_PCI
#  ifndef REV1_0_SILICON
#    define INCLUDE_PCI_AUTOCONF
#  else
#    undef INCLUDE_PCI_AUTOCONF
#endif /* INCLUDE_PCI */

/*
CPU Addr                    PCI Addr
PCI_LOCAL_MEM_BUS   ------------------------- PCI_MSTR_MEM_BUS
            -               -
                -               -
PCI_LOCAL_MEM_BUS + ------------------------- PCI_MSTR_MEM_BUS +
PCI_LOCAL_MEM_SIZE  ----IMMR                - PCI_MSTR_MEM_SIZE
                -               -
                -               -----PIMMR
                -               -
                -               -
CPU_PCI_MEM_ADRS    ------------------------- PCI_MEM_ADRS
                    -           -
                -           -
CPU_PCI_MEMIO_ADRS  ------------------------- PCI_MEMIO_ADRS
                    -           -
                -               -
CPU_PCI_IO_ADRS     ------------------------- PCI_IO_ADRS
                -               -
                -               -
CPU_PCI_IO_ADRS +   ------------------------- PCI_IO_ADRS +
CPU_PCI_IO_SIZE     -               - PCI_IO_SIZE
                -               -
                -               -
                -               -
                -------------------------- 4GBytes
*/



/* for custom sysPciAutoConfig.c */


/* PCI based addresses */
#  define PCI_MEM_ADRS        0x80000000
#  define PCI_MEM_SIZE        0x10000000      /* 256MB */
#  define PCI_MEM_SIZE_MASK   PCI_SIZE_256MB  /* Mask to match PCI_MEM_SIZE */
#  define PCI_MEMIO_ADRS      0x90000000
#  define PCI_MEMIO_SIZE      0x10000000      /* 256MB */
#  define PCI_MEMIO_SIZE_MASK PCI_SIZE_256MB  /* Mask to match PCI_MEMIO_SIZE */
#  define PCI_IO_ADRS         0xa0000000
#  define PCI_IO_SIZE         0x10000000      /* 256MB */


/* CPU based addresses */
#  define CPU_PCI_MEM_ADRS        0x80000000
#  define CPU_PCI_MEM_SIZE        PCI_MEM_SIZE
#  define CPU_PCI_MEMIO_ADRS      0x90000000
#  define CPU_PCI_MEMIO_SIZE      PCI_MEMIO_SIZE
#  define CPU_PCI_IO_ADRS         0xa0000000
#  define CPU_PCI_IO_SIZE         PCI_IO_SIZE


/* CPU from PCI bus */
#  define PCI_MSTR_MEM_BUS        0x00000000
#  define PCI_MSTR_MEM_SIZE       PCI_LOCAL_MEM_SIZE
#  define PCI_BRIDGE_PIMMR_BASE_ADRS      0x40000000


/* CPU Address that is visible from PCI */
#  define PCI_LOCAL_MEM_BUS   LOCAL_MEM_LOCAL_ADRS
#  define PCI_LOCAL_MEM_SIZE  LOCAL_MEM_SIZE
/* This should at least match size of LOCAL_MEM_SIZE */
#  define PCI_LOCAL_MEM_SIZE_MASK PCI_SIZE_256MB

#ifndef PCI_CFG_TYPE
#   ifdef INCLUDE_PCI_AUTOCONF
#       define PCI_CFG_TYPE PCI_CFG_AUTO
#   else
#       define PCI_CFG_TYPE PCI_CFG_FORCE
#   endif /* INCLUDE_PCI_AUTOCONF */
#endif /* PCI_CFG_TYPE */

#endif /* INCLUDE_PCI */


/* add necessary drivers */

#undef  INCLUDE_AR521X_END      /* Atheros AR521X WLAN Support */


#define INCLUDE_VXBUS

#ifdef INCLUDE_VXBUS
#   define INCLUDE_VXB_CMDLINE
#   define INCLUDE_HWMEM_ALLOC
#   define HWMEM_POOL_SIZE 50000
#   define INCLUDE_PLB_BUS
#   define INCLUDE_PARAM_SYS
#   define INCLUDE_MII_BUS
#   define INCLUDE_TSEC_VXB_END
#   define INCLUDE_GENERICPHY
#   ifdef INCLUDE_PCI
#       define INCLUDE_PCI_BUS
#       define DRV_PCIBUS_M83XX
#       define INCLUDE_PCI_BUS_AUTOCONF
#       define INCLUDE_PCI_OLD_CONFIG_ROUTINES
#   endif /* INCLUDE_PCI */
#endif /* INCLUDE_VXBUS */

#define INCLUDE_END  

/*
 * Support for Intel PRO/1000 gigabit ethernet
 * (Optional, off by default.)
 *
#define INCLUDE_GEI825XX_VXB_END
 */

#define INCLUDE_FLASH


#ifdef INCLUDE_FLASH
#  define SYS_FLASH_TYPE        FLASH_28F640J3A    /* flash device type */
#  define FLASH_WIDTH           2
#  define FLASH_CHIP_WIDTH      1
#  define FLASH_SEGMENT_SIZE    0x10000
#  define FLASH_ADRS            0xfffe0000
#  define FLASH_SIZE            FLASH_SEGMENT_SIZE
#  define FLASH_SIZE_WRITEABLE  FLASH_SEGMENT_SIZE
#  define FLASH_WIDTH_SPECIAL_2
#  undef FLASH_NO_OVERLAY
#endif

#define FLASH_BASE_ADRS     0xff800000
#define FLASH_WINDOW_SIZE   0x00400000

/* MAC Address configuration */

#define MAC_ADRS_LEN    6   /* 6 bytes in MAC address */
#define MAX_MAC_ADRS    2   /* number of MAC addresses to save in EEPROM */

/* enable 'M' command in bootrom */
#define ETHERNET_MAC_HANDLER
#define MAX_MAC_DEVS 1  /* one network device */
#ifdef BOOTAPP
#define INCLUDE_BOOT_ETH_MAC_HANDLER
#endif


/*
 * The Ethernet hardware address is of the form
 *
 *       00:A0:1E:nn:nn:nn
 *
 * where the first three bytes are defined below, and last three bytes are
 * user specified.
 */

#define WR_ENET0        0x00  /* WR specific portion of MAC (MSB->LSB) */
#define WR_ENET1        0xA0
#define WR_ENET2        0x1E

#define CUST_ENET3      0x11  /* Customer portion of MAC address */
#define CUST_ENET4      0x12
#define CUST_ENET5_0    0x00
#define CUST_ENET5_1    0x00

/* default mac address */

#define ENET_DEFAULT0   WR_ENET0
#define ENET_DEFAULT1   WR_ENET1
#define ENET_DEFAULT2   WR_ENET2

/* Non-volatile memory configuration */

#define NV_RAM_SIZE          EEPROM_SIZE /* 8K bytes non-volatile memory */
#define NV_RAM_ADRS          EEPROM_BASE_ADRS
#define NV_RAM_INTRVL        1

#if (NV_RAM_SIZE != NONE)
#  define SMART_EEPROM_WRITE     /* skip EEPROM write if value isn't changing */
#  define INCLUDE_EEPROM_LOCKING /* lock eeprom via software protection    */

#  undef  NV_BOOT_OFFSET       /* room for MAC addresses  */

  /* Define offsets for non-volatile data  */

#  define NV_BOOT_OFFSET       0  /* boot line parameters */
#  define NV_MAC_ADRS_OFFSET   (BOOT_LINE_SIZE + 1)  /* MAC addresses */
/* next available area */
#  define NV_USER_OFFSET    (NV_MAC_ADRS_OFFSET + (MAC_ADRS_LEN * MAX_MAC_ADRS))
#endif  /* NV_RAM_SIZE != NONE */

/* Hard Reset Configuration Words */

/* See MPC8349E hardware specification for supported clock freq */
/* spmf 1:5 ie 4*66Mhz == 266Mhz CSB*/
#define HRCW_LOW_BYTE0  0x04
/* corepll ratio 400Mhz  */
#define HRCW_LOW_BYTE1  0x23
/* Must be cleared*/
#define HRCW_LOW_BYTE2  0x00
/* Must be cleared*/
#define HRCW_LOW_BYTE3  0x00


/* Pci host,2* 32 bit buses,arbiters disabled */
#define HRCW_HIGH_BYTE0  HRCW_HIGH_PCI_HOST | HRCW_HIGH_PCI1_ARB | HRCW_HIGH_PCI64
/* Rom Location Flash 16 bit. Watch dog disabled  */
#define HRCW_HIGH_BYTE1  ROMLOC_GPCM_16BIT
/* GMII */
#define HRCW_HIGH_BYTE2  HRCW_HIGH_TSEC12M_GMII /* Both TSECs use GMII */
/* Big Endian */
#define HRCW_HIGH_BYTE3  0x00

#define INCLUDE_SYSLED

#undef  INCLUDE_BSP_VTS           /* For BSP validation */
#ifdef  INCLUDE_BSP_VTS

#define INCLUDE_SHELL
#define INCLUDE_LOADER
#define INCLUDE_UNLOADER
#define INCLUDE_SYM_TBL
#define INCLUDE_NET_SHOW
#define INCLUDE_RLOGIN
#define INCLUDE_TELNET
#define INCLUDE_NET_SYM_TBL
#define INCLUDE_PING
#define INCLUDE_SHOW_ROUTINES
#define INCLUDE_DISK_UTIL
#define INCLUDE_ICMP_SHOW
#define INCLUDE_TCP_SHOW
#define INCLUDE_UDP_SHOW
#define INCLUDE_IGMP_SHOW
#define INCLUDE_NET_IF_SHOW     /* Interface show routines */
#define INCLUDE_NET_SHOW
#define INCLUDE_DISK_UTIL
#define INCLUDE_IFCONFIG
#define INCLUDE_IFLIB
#define INCLUDE_ROUTECMD        /* routec command-line utility */

#endif /* INCLUDE_BSP_VTS */

#include "wrSbc834x.h"          /* include the wrSbc834x params */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* INCconfigh */
#if defined(PRJ_BUILD)
    #include "prjParams.h"
#endif

