/* config.h - wrSbcPowerQuiccII configuration header file */

/*
 * Copyright (c) 2003-2008, 2010, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
03i,19jul12,clx  Update BSP_REV
03h,15apr10,clx  Update BSP version for vxWorks 6.9
03g,29dec08,syt  Updated BSP_REV.
03f,17oct08,jmt  Fix use of RAM_LOW_ADRS and RAM_HIGH_ADRS
03e,20aug08,d_l  break default mac(ENET_DEFAULT) to three bytes.(WIND00121398)
03d,07nov07,h_k  reduced HWMEM_POOL_SIZE to 50000. (CQ:110392)
03c,15jul07,z_l  Add MOT_FCC_NUNITS to support 3 ethernet devices,update
                 version (WIND00092444)
03b,11jun07,vik   added INCLUDE_VXB_CMDLINE macro
03a,26feb07,h_k  removed undef INCLUDE_TFFS.
02z,01feb07,wap  Change INCLUDE_CPM to INCLUDE_VXB_CPM
02y,11dec06,rec  vxWorks 6.5 mods
02x,24oct06,jmt  Modify to use INCLUDE_BOOT_ETH_MAC_HANDLER
02w,21sep06,jmt  Add back the RAM_HIGH and RAM_LOW defines. Update version.
02v,06sep06,jmt  Remove INCLUDE_USER_APPL code, RAM_HIGH_ADRS and RAM_LOW_ADRS
02u,21aug06,wap  Add VxBus support
02t,11may06,jmt  Add SYS_MODEL definition to define base sysModel string
02s,02feb06,agf  update revision
02r,03sep05,mil  Removed references to VXLAYER.
02q,25jul05,mil  Added defines for BKL and KSL.
02p,13jun05,pcm  removed INCLUDE_DOSFS
02o,27jul05,md   Added ED&R support for minimal kernel
02n,15jul05,mil  Temporarily adding layer definitions in VXLAYER.
02m,12jul05,mil  Added minimal kernel support.
02l,17dec04,ckj  WLAN support for Atheros AR521X
02k,10nov04,j_b  project support for boot Flash and TFFS flash selection (SPRs
                 103800,103790)
02j,24sep04,mdo  Documentation fixes for apigen
02i,30aug04,j_b  remove INCLUDE_NETWORK dependencies (SPR #99747)
02h,29jul04,rec  MAC address scheme ('M') (SPR #93020)
02g,30jul04,md   use default PM_RESERVED_MEM
02f,24jun04,mdo  SPR #98438
02e,08jun04,mdo  Change MMU configuration - dependency is elsewhere.
02d,17may04,j_b  PM_RESERVED_MEM is conditional on INCLUDE_EDR_PM
02c,06apr04,j_b  Port to vxWorks 6.0
02b,23feb04,j_b  update version & revision count
02a,19feb04,j_b  merge power management
01z,08dec03,agf  update version & revision count
01y,19nov03,j_b  fix 32-bit PCI IO space definitions
01x,06oct03,j_b  add TFFS Flash selection
01w,15aug03,j_b  PC Card and wireless LAN support
01v,08aug03,j_b  64MB Flash support
01u,08aug03,scm  remove references to GEI for release...
01t,07aug03,j_b  TFFS support
01s,05aug03,scm  update to support basic PCI-to-PCI bridging...
01r,16jul03,j_b  add RTC support; remove vware
01q,11jun03,j_b  add CONSOLE_TTY; store only 4 IP and MAC addrs; cleanup
01p,30may03,j_b  update version for EAR
01o,29may03,j_b  undefine fei END by default
01n,29may03,j_b  code review cleanup
01m,29may03,scm  turn on PCI by default...
01l,22may03,jb3  Remove ATM references.
01k,20may03,scm  add seperate fei selector...
01j,09may03,scm  add fei support...
01i,02may03,j_b  reverse CPU ID values
01h,30apr03,j_b  removed HARDCODED_FREQ_PARAMS, added OSCILLATOR_FREQ
01g,28apr03,scm  update PCI interrupts...
01f,25apr03,scm  update pci...
01e,24apr03,jb3  Add space in NVRAM for IP adrs and define myAppInit for multi
                 net support.
01d,24apr03,j_b  add CPU selection support
01c,22apr03,scm  add i8259A PIC support...
01b,16apr03,j_b  add USE_LEDS_FOR_BOOT_DEBUG option
01a,01apr03,j_b  adapted from wrSbc8260Atm (ver 01f)
*/

/*
This file contains the configuration parameters for the BSP.
*/
#ifndef	__INCconfigh
#define	__INCconfigh

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*                                                                  */
/* BSP version/revision identification, should be placed before the */
/* the #include "configAll.h"                                       */

#define BSP_VERSION        "6.9"       /* vxWorks 6.9 */
#define BSP_REV            "/1"        /* 0 for first revision */

/* Model string. sysModel returns this value */
#define SYS_MODEL       "Wind River SBC PowerQUICC II - MPC82xx PowerQUICC II"

/* Now include the global configuration information.                */

#include <configAll.h>

/* Enable VxBus support by default */

#define INCLUDE_VXBUS
#define INCLUDE_PARAM_SYS
#define INCLUDE_HWMEM_ALLOC
#define HWMEM_POOL_SIZE	50000
#define INCLUDE_PLB_BUS
#define INCLUDE_MII_BUS
#define INCLUDE_MDIO
#define INCLUDE_BCM52XXPHY
#define INCLUDE_FCC_VXB_END
#define INCLUDE_SCC_VXB_END
#define INCLUDE_VXB_CPM
#define INCLUDE_DMA_SYS
#define INCLUDE_VXB_CMDLINE

/*
 * Input Frequency configuration: FREQ_66MHZ, FREQ_83MHZ or FREQ_100MHZ
 * OSCILLATOR_FREQ is target-dependent and must be set correctly to
 * determine CPU, CPM, PCI bus, SCC clock and Baud Rate Generator frequencies.
 */
#define  FREQ_66MHZ          66000000
#define  FREQ_83MHZ          83000000
#define  FREQ_100MHZ        100000000

#define OSCILLATOR_FREQ     FREQ_66MHZ     /* 60x Bus frequency as well */

/*
 * CPU Identification - read by user switches or defined here
 * Determines maximum available FCCs and PCI capability
 *
 * The CPU IDs below match user switch settings:
 * Switch Bank SW5, Switches 3,2 and 1 (Open/Off = 1, Closed/On = 0).
 * Example: CPU_ID_8265 is determined by SW5-3 = Off, SW5-2 = On, SW5-1 = On
 */
#define CPU_ID_8250               0x00
#define CPU_ID_8255               0x01
#define CPU_ID_8260               0x02
#define CPU_ID_8264               0x03
#define CPU_ID_8265               0x04
#define CPU_ID_8266               0x05
#define CPU_ID_8270               0x06
#define CPU_ID_8280               0x07

#define MAX_CPU_ID                CPU_ID_8280

/* CPU Identification Mask, when CPU ID is set by user switches */
#define CPU_ID_MASK               0x07

#undef HARDCODED_CPU_ID  /* undef to use User Dip Switches, see target.nr */

#ifdef HARDCODED_CPU_ID
#  define CPU_ID              CPU_ID_8265
#endif /* HARDCODED_CPU_ID */


/* Memory configuration */

/*
 * The constants ROM_TEXT_ADRS, ROM_SIZE, and RAM_HIGH_ADRS are defined
 * in config.h and Makefile. All definitions for these constants must be
 * identical.  (can be fine-tuned for various configurations)
 */

#define ROM_SIZE             0x00200000        /* 2M ROM space               */
#define ROM_BASE_ADRS        0xfff00000        /* base address of ROM        */
#define ROM_TEXT_ADRS        ROM_BASE_ADRS + 0x100
#define LOCAL_MEM_LOCAL_ADRS 0x00000000        /* Base of RAM                */
#define LOCAL_MEM_SIZE       0x10000000        /* 256 Mbyte memory           */
#define USER_RESERVED_MEM    0x0E000000        /* user reserved memory size  */
#define ROM_WARM_ADRS	     (ROM_TEXT_ADRS+8)  /* warm reboot entry */

/* Flash Types */

#define ON_BOARD_FLASH    1
#define SIMM_FLASH        2

/* Boot Flash selection */
#define BOOT_FLASH           ON_BOARD_FLASH /* Specify CS0 connection */

/* Optional TrueFFS support */

#ifdef INCLUDE_TFFS
#  define INCLUDE_DISK_UTIL
#  define INCLUDE_MTD_USR
   /* TFFS Flash selection: SIMM_FLASH or ON_BOARD_FLASH */
#  define TFFS_FLASH  SIMM_FLASH
#endif /* INCLUDE_TFFS */

/* Serial port configuration */

#define INCLUDE_TTY_DEV      /* Include TTY Dev for those that use command line building */
#undef	NUM_TTY
#define	NUM_TTY		     N_SIO_CHANNELS

#undef	 CONSOLE_TTY
#define CONSOLE_TTY	     0  /* 0 = SMC1/COM1, 1 = SMC2/COM2 */
#define SIO_POLL_CONSOLE     0  /* 0 = SMC1/COM1, 1 = SMC2/COM2 */

/* Timer configuration */

#define  INCLUDE_AUX_CLK
#define  INCLUDE_TIMESTAMP

/* Cache configuration  */

#define INCLUDE_CACHE_SUPPORT
#define USER_I_CACHE_ENABLE
#undef  USER_I_CACHE_MODE
#define USER_I_CACHE_MODE    CACHE_COPYBACK /* select COPYBACK or WRITETHROUGH */

#define USER_D_CACHE_ENABLE
#undef  USER_D_CACHE_MODE
#define USER_D_CACHE_MODE    CACHE_COPYBACK /* select COPYBACK or WRITETHROUGH */

/* MMU configuration */

#define INCLUDE_MMU_BASIC
#undef  INCLUDE_MMU_FULL
#define USER_I_MMU_ENABLE
#define USER_D_MMU_ENABLE

/* LED configuration */

#define INCLUDE_SYSLED  /* Hex LED support */

/* Boot Tracking with LED */

#ifdef INCLUDE_SYSLED
#  define USE_LEDS_FOR_BOOT_DEBUG
#endif /* INCLUDE_SYSLED */

/* IP Address configuration */

#define IP_ADRS_LEN	    24	/* bytes in IP address */
#define MAX_IP_ADRS      4	/* IP addresses to save in EEPROM */

/* MAC Address configuration */

#define MAC_ADRS_LEN	6	/* 6 bytes in MAC address */
#define MAX_MAC_ADRS    4   /* MAC addresses to save in EEPROM */

#define WR_ENET0  	    0x00  /* WR specific portion of MAC (MSB->LSB) */
#define WR_ENET1  	    0xA0
#define WR_ENET2  	    0x1E

#define CUST_ENET3_0    0xA0  /* Customer portion of MAC address */
#define CUST_ENET3_1    0xA1
#define CUST_ENET3_2    0xA2
#define CUST_ENET3_3    0xA3
#define CUST_ENET4 	    0xAA
#define CUST_ENET5 	    0xA0

/*
 * The Ethernet hardware address that is used with FCC or SCC is
 *
 *       00:A0:1E:nn:nn:nn
 *
 * where the first three bytes are defined below, and last three bytes are
 * user specified.
 */

#define ENET_DEFAULT0 0x00
#define ENET_DEFAULT1 0xA0
#define ENET_DEFAULT2 0x1E

/* Non-volatile memory storage configuration */

#define SMART_EEPROM_WRITE     /* skip EEPROM write if already the value */
#define INCLUDE_EEPROM_LOCKING /* lock eeprom via software protection 	 */

/* enable 'M' command */
#define ETHERNET_MAC_HANDLER
#define MAX_MAC_DEVS 2 /* two network devices (fcc, scc) */
#ifdef BOOTAPP
#define INCLUDE_BOOT_ETH_MAC_HANDLER
#endif

/* Number of FCC ports */

#define MOT_FCC_NUNITS       3

#define EEPROM_DELAY         50

/* Non-volatile memory configuration */

#define NV_RAM_SIZE          EEPROM_SIZE /* 8K bytes non-volatile memory */
#define NV_RAM_ADRS          EEPROM_ADRS
#define NV_RAM_INTRVL        1

#define RETRY_NVWRITE_COUNT  1000 /* abort loop after iterations */

#undef  NV_BOOT_OFFSET	     /* room for IP and MAC addresses  */

/* Define offsets back from boot line for sysNvRamGet();  */

#define NV_BOOT_OFFSET       ((IP_ADRS_LEN * MAX_IP_ADRS) + (MAC_ADRS_LEN * MAX_MAC_ADRS))
#define NV_IP_ADRS_OFFSET    -((IP_ADRS_LEN * MAX_IP_ADRS) + (MAC_ADRS_LEN * MAX_MAC_ADRS))
#define NV_MAC_ADRS_OFFSET   -(MAC_ADRS_LEN * MAX_MAC_ADRS)

/* Network driver configuration */

#define INCLUDE_END       /* Enhanced Network Driver (see configNet.h) */

/* Enhanced Network Driver Types */

#define NO_NETWORK           0  /* No network interfaces supported */
#define SCC_FCC_END          1  /* Both on-board network interfaces supported */
#define SCC_END              2  /* Only SCC supported on-board */
#define FCC_END              3  /* Only FCC supported on-board */
#define FEI_END              4  /* No on-board network interfaces supported */

#ifdef INCLUDE_END
#  define MAX_SCC_CHANNELS  1       /* max SCC number for Ethernet channels */

/* Specify Boot Device from Network Driver Types above */
#  define BOOT_DEVICE          SCC_FCC_END

#else /* INCLUDE_END */
#  define BOOT_DEVICE          NO_NETWORK

#endif /* INCLUDE_END */

/* Network interface support and default Boot Device */
#if (BOOT_DEVICE == FCC_END)
  #define INCLUDE_MOTFCCEND    /* Include Motorola FCC Fast Ethernet controller */
  #undef  INCLUDE_MOTSCCEND    /* Exclude Motorola SCC Ethernet controller */
  #define BOOT_DEV_NAME        "motfcc"
  #undef  WDB_COMM_TYPE
  #define WDB_COMM_TYPE WDB_COMM_END
#elif (BOOT_DEVICE == SCC_END)
  #undef  INCLUDE_MOTFCCEND    /* Exclude Motorola FCC Fast Ethernet controller */
  #define INCLUDE_MOTSCCEND    /* Include Motorola SCC Ethernet controller */
  #define BOOT_DEV_NAME        "motscc"
  #undef  WDB_COMM_TYPE
  #define WDB_COMM_TYPE WDB_COMM_END
#elif (BOOT_DEVICE == SCC_FCC_END)
  #define BOOT_DEV_NAME        "motfcc"
  #define INCLUDE_MOTFCCEND    /* Include Motorola FCC Fast Ethernet controller */
  #define INCLUDE_MOTSCCEND    /* Include Motorola SCC Ethernet controller */
  #undef  WDB_COMM_TYPE
  #define WDB_COMM_TYPE WDB_COMM_END
#elif (BOOT_DEVICE == FEI_END)
  #undef  INCLUDE_MOTFCCEND
  #undef  INCLUDE_MOTSCCEND
  #define BOOT_DEV_NAME        "fei"
  #undef  WDB_COMM_TYPE
  #define WDB_COMM_TYPE WDB_COMM_END
#elif (BOOT_DEVICE == NO_NETWORK)
  #undef  INCLUDE_MOTFCCEND
  #undef  INCLUDE_MOTSCCEND
  #undef  INCLUDE_FEI_END
  #define BOOT_DEV_NAME        "none"
#else
  #error BOOT_DEV_NAME needs to be defined in config.h
#endif /* (BOOT_DEVICE == SCC_FCC_END) */

#ifdef INCLUDE_FCC_VXB_END
#undef INCLUDE_MOTFCCEND
#endif

#ifdef INCLUDE_SCC_VXB_END
#undef INCLUDE_MOTSCCEND
#endif


/* Secondary Interrupt Controller i8259A PIC, Required for PCI support */
#define INCLUDE_i8259A

#ifdef INCLUDE_i8259A

/** PIC **/
#  define PIC1_BASE_ADR              0x22008000
#  define PIC1_IRQ_BASE              0x00
#  define INT_VEC_EXT_IRQ0           PIC1_IRQ_BASE
#  define PIC_REG_ADDR_INTERVAL      1

/*
 * Vector number is not flexible, obtained by:
 *   vectorNo(PIC_IRQn) = vectorNo(PIC_IRQ_BASE) + n
 */

#  define NUMBER_OF_PIC_IRQS           8

/* PIC1 PCI interrupt mapping */
#  define PIC1_PC_INTA_IRQ   PIC1_IRQ_BASE        /* PCMCIA Card Bus 0 */
#  define PIC1_PC_INTB_IRQ   (PIC1_IRQ_BASE + 1)  /* PCMCIA Card Bus 1 */
#  define PIC1_185_IRQ       (PIC1_IRQ_BASE + 2)  /* Security Processor -NOT supported */
#  define PIC1_ATM_IRQ       (PIC1_IRQ_BASE + 3)  /* ATM Interface -NOT Supported */
#  define PIC1_INTA_IRQ      (PIC1_IRQ_BASE + 4)  /* INT_A, PMC Device (shared) */
#  define PIC1_INTB_IRQ      (PIC1_IRQ_BASE + 5)  /* INT_B... */
#  define PIC1_INTC_IRQ      (PIC1_IRQ_BASE + 6)  /* INT_C, IBM21P100BGC PCI/PCI-x Bridge, if used */
#  define PIC1_INTD_IRQ      (PIC1_IRQ_BASE + 7)  /* INT_D... */

/*
 * include PCI library support
 * since PCI dependent on i8259A, it is within the confines of the i8259A wrapper
 */
#  define INCLUDE_PCI

#  ifdef INCLUDE_PCI

#    define INT_NUM_IRQ6         0x18        /* IRQ6: PCI vector number */
#    define IRQ6_SIPNR_H_MASK    0x00400000  /* clear IRQ6 by writing 1 */

/*
 * Defining INCLUDE_PCI_AUTOCONF, causes the VxWorks bootroms to use
 * the pciAutoConfigLib to enumerate PCI devices. This is
 * the recommended default setting. Undef'ing
 * INCLUDE_PCI_AUTOCONF, causes the BIOS PCI configuration
 * settings to be used by VxWorks.
 */

#    define INCLUDE_PCI_AUTOCONF

#    undef PQII_EXTND_PCI_MASTER   /* configure PCI cards on extended backplane */

#    undef PCI_DEBUG
#    undef PCI_AUTO_DEBUG

#    ifdef PCI_DEBUG
#      define INCLUDE_LOGGING
#      define INCLUDE_LOG_STARTUP
#      undef MAX_LOG_MSGS
#      define MAX_LOG_MSGS   100
#    endif  /* PCI_DEBUG */

#    undef  INCLUDE_FEI_END
#    ifdef  INCLUDE_FEI_END
#      define SYS_FEI_UNITS  1  /* max fei units supported */
#    endif  /* INCLUDE_FEI_END */

#    define DELTA(a,b)            (abs((int)a - (int)b))

/*
*******************************************
* Power Quicc II PCI Configuration Defines
*******************************************
*/
/*
CPU Addr                                         PCI Addr
PCI_LOCAL_MEM_BUS     -------------------------- PCI_MSTR_MEM_BUS
                      -                        -
                      -                        -
PCI_LOCAL_MEM_BUS +   -------------------------- PCI_MSTR_MEM_BUS +
PCI_LOCAL_MEM_SIZE    ----IMMR                 - PCI_MSTR_MEM_SIZE
                      -                        -
                      -                    -----PIMMR
                      -                        -
                      -                        -
CPU_PCI_MEM_ADRS      -------------------------- PCI_MEM_BASE
                      -                        -
                      -                        -
CPU_PCI_MEMIO_ADRS    -------------------------- PCI_MEMIO_ADRS
                      -                        -
                      -                        -
CPU_PCI_IO_ADRS       -------------------------- PCI_IO_BASE
                      -                        -
                      -                        -
CPU_PCI_IO_ADRS +     -------------------------- PCI_IO_BASE +
CPU_PCI_IO_SIZE       -                        - PCI_IO_SIZE
                      -                        -
                      -                        -
                      -                        -
                      -------------------------- 4GBytes
*/

/* PCI based addresses */
#    define PCI_MEM_BASE             0x90000000  /* PCI Auto Config Memory Start Address UINT32 */
#    define PCI_MEM_SIZE             0x04000000  /* 64M - PCI Auto Config Memory Size    UINT32 */

#    define PCI_IO_BASE              0xB0000000  /* PCI Auto Config IO Start Address     UINT32 */
#    define PCI_IO_SIZE              0x00100000  /* 1M - PCI Auto Config IO Size         UINT32 */

/* CPU based addresses */
#    define CPU_PCI_MEM_ADRS         PCI_MEM_BASE
#    define CPU_PCI_MEM_SIZE         PCI_MEM_SIZE /* (PCI_MEM_SIZE / 2) */

#    define CPU_PCI_NO_PRE_MEM_ADRS  0x00000000  /* (PCI_MEM_BASE + CPU_PCI_MEM_SIZE) */
#    define CPU_PCI_NO_PRE_MEM_SIZE  0x00000000  /* (PCI_MEM_SIZE / 2) */

#    define CPU_PCI_IO32_ADRS        PCI_IO_BASE
#    define CPU_PCI_IO32_SIZE        PCI_IO_SIZE

#    define CPU_PCI_IO16_ADRS        0x00000000
#    define CPU_PCI_IO16_SIZE        0x00000000

#    define PCI_IO_END               (PCI_IO_BASE + PCI_IO_SIZE - 1)
#    define PCI_MEM_END              (PCI_MEM_BASE + PCI_MEM_SIZE - 1)

/* CPU from PCI bus */
#    define PCI_MSTR_MEM_BUS         0x00000000
#    define PCI_MSTR_MEM_SIZE        0x10000000

/* CPU Address that is visible from PCI */
#    define PCI_LOCAL_MEM_BUS        0x00000000
#    define PCI_LOCAL_MEM_SIZE       PCI_MSTR_MEM_SIZE

/* PCI defines begin */
#    define BUS                      0              /* bus-less board       */
#    define BUS_TYPE                 BUS_TYPE_PCI   /* board bus interface  */

#    define CLASS_OFFSET             0xB
#    define CLASS_WIDTH              0x1
#    define BRIDGE_CLASS_TYPE        0x6

#    undef  PCI_LAT_TIMER
#    define PCI_LAT_TIMER            0x40  /* latency timer value, 64 PCI clocks */

#    undef  PCI_MAX_BUS
#    define PCI_MAX_BUS              2

#    define M8260_SCCR_PCI_MODE_BIT  0x80

/* ID Select Line */
#    define PQII_MPC8265_ID          0x00  /* if agent mode were to be supported ID select is 0x13 */
#    define PQII_PMC_ID              0x10
#    define PQII_CARD_BUS_ID         0x11
#    define PQII_IBM_BRIDGE_ID       0x12

#    ifdef PQII_EXTND_PCI_MASTER
#      define PQII_EXT_SLOT_1        0x0C
#      define PQII_EXT_SLOT_2        0x0D
#      define PQII_EXT_SLOT_3        0x0E
#    endif  /* PQII_EXTND_PCI_MASTER */

#    define PCI_DEV_ID_8265          0x18C01057  /* Vendor & Dev Id for MPC8265ADS-PCI board */
#    define PCI_DEV_ID_TIPCI1420     0xAC51104C  /* Vendor & Dev Id for TI PCI1420 PCI-PCMCIA controller */
#    define PCI_DEV_ID_IBM21P100     0x01A71014  /* Vendor & Dev Id for IBM21P100BGC PCI/PCI-x Bridge */

#    define PCI_ID_I82557            0x10308086  /* Id for Intel 82557 */
#    define PCI_ID_I82559            0x12298086  /* Id for Intel 82559 */
#    define PCI_ID_I82559ER          0x12098086  /* Id for Intel 82559 ER */

#    define PCI_ID_I82543_1          0x10018086  /* Id for Intel 82543 Copper */
#    define PCI_ID_I82543_2          0x10048086  /* Id for Intel 82543 Fiber */

#    define PCI_ID_I82544_1          0x10088086  /* Id for Intel 82544 Copper */
#    define PCI_ID_I82544_2          0x10098086  /* Id for Intel 82544 Fiber */
#    define PCI_ID_I82544_3          0x100C8086  /* Id for Intel 82544 Copper */

/* PC Card support */
#    undef INCLUDE_PCCARD

#    ifdef INCLUDE_PCCARD
#      define INCLUDE_PCCARD_SHOW

#      define PC_WINPAGE_REG         0x840

#      define SYSTEM_CTRL_REG        0x80
#      define MULTIFUNC_REG          0x8c
#      define DEVICE_CTRL_REG        0x92

#      define P2CCLK                 0x08000000
#      define INTRTIE                0x20000000

#      define PARALLEL_PCI_MASK      0xF9

/* PC Cards supported */
#      undef INCLUDE_PCCARD_ATA
#      undef INCLUDE_PCCARD_SERIAL
#      define INCLUDE_PCCARD_ELT_3C589_END
#      define INCLUDE_PCCARD_NE2K_END
#      undef INCLUDE_PCCARD_EL_3C575_END
#      undef INCLUDE_PCCARD_NETGEAR_END

#      ifdef INCLUDE_PCCARD_ATA
#        define INCLUDE_ATA
#        define INCLUDE_DISK_UTIL
#      endif  /* INCLUDE_PCCARD_ATA */

/* Wireless LAN PC Card support - must have WindNet 802.11b DDK */
#      undef INCLUDE_WLAN_END
#      undef INCLUDE_PCCARD_INTPR_END

#      undef INCLUDE_PCI_CFGSHOW

#      undef INCLUDE_SHOW_ROUTINES
#      undef INCLUDE_PING
#      undef INCLUDE_NET_SHOW
#    endif  /* INCLUDE_PCCARD */

/* Atheros AR521X WLAN Support */
#    undef INCLUDE_AR521X_END

#  endif  /* INCLUDE_PCI */
#endif  /* INCLUDE_i8259A */

/* Real-Time Clock Support */
#define M48T59_RTC_SRAM    /* RTC with alarm, watchdog and SRAM */

/* Include hardware header file */

#include "wrSbcPowerQuiccII.h"


/* Debugging configuration */

/*
 * If the FORCE_DEFAULT_BOOT_LINE is defined then the DEFAULT_BOOT_LINE
 * parameters are always used regardless of NVRAM values specified at
 * bootrom time. See target.nr for details. This is usually used to debug
 * WR downloaded images without a bootrom present.
 */

#undef  FORCE_DEFAULT_BOOT_LINE

/* Transparent Mode Driver for WDB connection via Wind River emulator */
#undef  INCLUDE_WDB_COMM_VTMD
#ifdef  INCLUDE_WDB_COMM_VTMD
#  undef FORCE_DEFAULT_BOOT_LINE /* When using TMD this macro should be undefined */
#undef  WDB_COMM_TYPE
#define WDB_COMM_TYPE WDB_COMM_VTMD
#endif  /* INCLUDE_WDB_COMM_VTMD */

/* Stuff to be excluded if FORCE_DEFAULT_BOOT_LINE defined */

#ifdef  FORCE_DEFAULT_BOOT_LINE
#  undef  NV_RAM_SIZE
#  define NV_RAM_SIZE		  NONE
#endif  /* FORCE_DEFAULT_BOOT_LINE */

/* Boot line configuration */

#ifdef BOOT_DEV_NAME
#  define DEFAULT_BOOT_LINE    BOOT_DEV_NAME \
                             "(0,0)wrSbcPowerQuiccII:vxWorks " \
                             "e=24.42.124.92 " \
                             "h=24.42.124.94 " \
                             "g=0.0.0.0 " \
                             "u=vx pw=vx " \
                             "f=0x00 tn=wrSbcPowerQuiccII"

#endif /* BOOT_DEV_NAME */

/*
 * STANDALONE_NET must be defined for network debug with
 * standalone vxWorks
 */
#undef STANDALONE_NET

/* define for bspVal testing */
#undef BSPVAL_TEST

#ifdef BSPVAL_TEST
#  define INCLUDE_DEBUG
#  define INCLUDE_SHELL
#  define INCLUDE_SHOW_ROUTINES
#  define INCLUDE_NET_SHOW
#  define INCLUDE_NET_SYM_TBL
#  define INCLUDE_LOADER
#  define INCLUDE_PING
#  define INCLUDE_POWER_MGMT_CPU_BSP_SUPPORT
#endif /* BSPVAL_TEST */


#ifdef __cplusplus
}
#endif

#endif	/* __INCconfigh */

#if defined(PRJ_BUILD)
#include "prjParams.h"
#endif

#ifndef RAM_DST_ADRS         /* default uncompress dest. */
#ifndef RAM_HIGH_ADRS
#  define RAM_DST_ADRS       0x01C00000        /* RAM address for ROM boot   */
#else
#  define RAM_DST_ADRS       RAM_HIGH_ADRS
#endif  /* RAM_HIGH_ADRS */
#endif  /* RAM_DST_ADRS */

