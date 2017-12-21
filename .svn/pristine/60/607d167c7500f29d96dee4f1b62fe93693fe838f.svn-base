/* config.h - Motorola MVME3100 board configuration header */

/*
 * Copyright (c) 2002-2010, 2012, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 2004-2005 Motorola Inc. All Rights Reserved */

/*
modification history
--------------------
03d,07apr13,d_l  remove INCLUDE_PERFMON to reduce footprint. (WIND00411417)
                 remove build warning for "SM_BASE_ADRS redefined"
                 update BSP version to "6.9/2"
03c,06feb13,d_l  undefine INCLUDE_TIMESTAMP and remove VXB_AUXCLK_DEBUG to 
                 reduce footprint.
                 update BSP version to "6.9/1"
03b,17jan12,mpc  defined CPU_NUM_OF_SM_LOCATION (WIND00325804)
03a,04may10,dhy  modify SM_ANCHOR_ADRS value(WIND00208587)
03a,15apr10,clx  Update BSP version for vxWorks 6.9
02z,15sep09,l_z  Remove unused INCLUDE_PPC_FPU. (WIND00153695)
02y,23dec08,z_l  updated BSP_REV to "/8"
02x,15oct08,y_x  Added virtual vxBus PCI to fix USB issue (WIND00135828)
02w,24apr08,x_f  Added vxBus driver support
02v,25mar08,y_w  updated BSP_REV to "/7"
02u,23jan08,y_w  Fixed MemScope connection error, updated BSP_REV to "/6"
02t,26jul07,agf  inc BSP_REV
02s,20apr07,y_w  Updated rev for VxWorks 6.5, Fixd defects: WIND00038065 and
                 WIND00087536.
02r,11dec06,sam  (WRS) Updated rev for VxWorks 6.4, changed intConfig
                 entry for USB to use Edge (spurrious int fix).
02r,11dec06,jch  (WRS) Updated for VxWorks 6.x compatibility.
02q,23sep05,pjh  IR03 fixes.
02p,07sep05,cak  Modifications to make the file more Tornado Project
                 friendly. 
02o,01sep05,efb  Added support for Wind River USB (requires PNE).
02n,31aug05,cak  TFFS and Flash programming support.
02m,18aug05,pjh  Support for AuxClk.
02l,12aug05,pjh  Support for global and external tick timers.
02k,02aug05,efb  Added INCLUDE_WDOG_TIMER for watchdog timer support.
02j,28jul05,cak  Support for booting via the firmware.
02i,26jul05,efb  Undef'ed INCLUDE_AUX_CLOCK until support is added.
02h,29jun05,efb  Added performance monitor support.
02g,28jun05,cak  Interrupt controller support to allow configuration of
                 interrupt priorities.
02f,21jun05,cak  DS1375 RTC with Alarm support.
02e,02jun05,pjh  Added Quad UART EXAR ST16C554D support.
02d,15may05,efb  Added sATA support.
02c,22apr05,cak  DS1621 Temperature Sensor support.
02b,22apr05,cak  Reenable L2 cache support - Patch 99776: E500 PATCH SPR99776.
02a,12apr05,cak  Removed (#undef'ed) L2 cache support for now until we can
                 determine what is causing the gige blaster/blastee and soak
                 test failures.
01z,01apr05,efb  Set caching to enable L1 instr & data, L2 data only.
01y,25mar05,cak  MPC8540 DMA Support.
01x,07feb05,cak  L2 Cache/SRAM support. 
01w,31jan05,cak  Removed unneeded SCC Serial references.
01v,27jan05,cak  NVRAM Support.
01u,25jan05,cak  Shared memory support. 
01t,06jan05,cak  MV3100 board bring-up, moved some definitions to more
                 appropriate header files.
01s,17dec04,cak  Added some shared memory parameters.
01r,13dec04,cak  PCI/PCI-X Bus Interface support.
01q,18nov04,cak  Moved LOCAL_MEM_SIZE to individual board header files and
                 removed unused DDR SDRAM and FEI ethernet driver 
                 definitions.
01p,09nov04,scb  Added #include "xxx8540B.h".
01o,11oct04,scb  Move flash banks to 0xf0000000.
01n,23aug04,scb  Starting point for "Big Easy".
01m,28oct03,dtr  Support only 32MB for now.
01l,21oct03,dtr  Trim config.h for final release.
01k,02oct03,dtr  Add back in SPE support.
01j,12sep03,dtr  Change some clock defines.
01i,20aug03,dtr  Adding support for TSEC.
01h,14aug03,dtr  Remove undefs for FP and windview.
01g,04aug03,dtr  Modify configuration elements for LBC SDRAM and DDR SDRAM.
01f,29jul03,dtr  Added snoop and disable L2 because of dcbi/dcbst issue.
01e,22jul03,mil  Added flash parameters.
01d,07jul03,mil  Added CPM and SCC.
01c,25jun03,mil  Added prjParams.h for project build.
01b,09oct02,dtr  Added more defines for more complete base for BSP.
01a,03oct02,dtr  Created.
*/

#ifndef __INCconfigh
#define __INCconfigh

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

#define BSP_VERSION        "6.9"       /* vxWorks 6.9 */
#define BSP_REV            "/2"        /* 0 for first revision */

/* PRIMARY INCLUDES */

#include <configAll.h>

/* defines */

#define SYS_MODEL "Motorola MVME3100"  /* only used in validation test */

#define DEFAULT_BOOT_LINE \
    "mottsec(0,0)host:/config/mv3100/vxWorks h=90.0.0.1 e=90.0.0.2 u=vxworks"

#define INCLUDE_MMU_BASIC          /* MMU Support */
#define INCLUDE_L2_CACHE_SRAM      /* L2 Cache & SRAM support */
#define INCLUDE_PRIMARY_TSEC_END   /* Front Panel Three Speed Enet support */
#undef  INCLUDE_SECONDARY_TSEC_END /* Backplane Three Speed Enet support */
#undef  INCLUDE_MOT_FEC_END        /* Backplane fast Ethernet support */
#undef  INCLUDE_ATA                /* sATA support in IDE legacy mode only */
#undef  INCLUDE_QUAD_UART          /* Rear Transition Module Quad UART support*/
#undef  INCLUDE_AUX_CLK            /* Auxilliary clock support */
#undef  INCLUDE_SM_NET             /* Shared Memory Support */ 
#undef  INCLUDE_SM_SEQ_ADDR
#define INCLUDE_SPE                /* Support for SPE 64bit registers */
#undef  INCLUDE_TFFS               /* True Flash File system support */
#undef  INCLUDE_BOOT_IMAGE_PROGRAM /* FLASH Boot Bank Programming Support */
#undef  INCLUDE_ERROR_HANDLING     /* Error handling suite */
#undef  INCLUDE_MPC8540_DMA        /* MPC8540 DMA Support */
#undef  INCLUDE_DS1621             /* Maxim DS1621 Temp Sensor Support */
#undef  INCLUDE_RTC                /* Maxim DS1375 RTC with Alarm Support */
#undef  INCLUDE_WDOG_TIMER         /* Watchdog timer support */
#undef  INCLUDE_MPC8540_TIMERS     /* MPC8540 timer support */
#undef  INCLUDE_TICK_TIMERS        /* External tick timer support */
#undef  INCLUDE_USB                /* Wind River USB support */


/*
 * Firmware RAM & ROM Booting
 *
 * #define'ing FIRMWARE_BOOT allows one to use the MOTLoad firmware
 * command 'execProgram' to begin execution of VxWorks code rather
 * than having the processor, upon powerup, automatically begin
 * executing code from the cold boot location.  This provides
 * flexibility in just where the VxWorks image can reside and
 * still be invoked for execution.
 *
 * The code which is executed via MOTLoad's execProgram command
 * must have been previously loaded into RAM or alternatively
 * placed into flash memory.  The commentary which follows explains
 * this process.
 *
 * To execute a RAM-resident VxWorks "boot" image (rather than
 * a flash resident "boot" image), perform the following:
 *
 * 1. Change #undef FIRMWARE_BOOT to #define FIRMWARE_BOOT.
 * 2. ROM_BASE_ADRS may need to be adjusted.  It is
 *    currently set, for RAM booting, based on the assumption that
 *    there is 512MB of DRAM.  However, this should be adjusted
 *    to the actual amount of DRAM on the board minus 1MB, in order to
 *    leave room for the vxWorks boot image at the top of DRAM.
 *      DRAM - 0x100000  (currently set to 0x1ff00000, if FIRMWARE_BOOT
 *                        is #define'd)
 * 3. Since ROM_TEXT_ADRS, ROM_WARM_ADRS, and ROM_LINK_ADRS are defined 
 *    in both the Makefile (hard-coded) and config.h (based on the value 
 *    of ROM_BASE_ADRS) it is necessary to adjust the values of 
 *    ROM_TEXT_ADRS, ROM_WARM_ADRS, and ROM_LINK_ADRS in the Makefile.  
 *    The values used are dependent upon the value of ROM_BASE_ADRS as 
 *    discussed in #2 above.  For a board with 512MB of DRAM they would 
 *    be 0x1ff00100, 0x1ff00108, and 0x1ff00100 respectively.
 *
 * NOTE: If FIRMWARE_BOOT is defined then USER_RESERVED_MEM will be
 *       defined as 0x100000 (1MB) in order to preserve the boot image
 *       in the top 1 MB of the DRAM.
 *
 * To use the firmware to boot vxWorks from DRAM use the following commands
 * (substitute the appropriate values where necessary).  The values used
 * for the tftp download address and the execP load address are based on a
 * board with 512MB of DRAM:
 *
 * tftpGet -cXXX.XXX.XXX.XXX -fboot.bin -sXXX.XXX.XXX.XXX -d/dev/enetx -a1FF00000
 * execProgram -l1FF00100 -sFFF00
 *
 * A "cold boot" (powerup boot) always causes the image flashed into
 * the boot block to be executed.  The boot block will always be the last
 * megabyte of the flash bank.  There are two boot blocks on the MVME3100,
 * and the boot block switch effectively sets which block is physically
 * mapped to the last megabyte of the flash bank.  The execution address 
 * for cold boot start is 0xfffffffc which is the very end of the boot 
 * block .  We may want to use a flash bank to hold an image for execution 
 * but we also want to protect the "cold boot" image that is present in 
 * the current boot block.  We can use the first megabyte of flash memory 
 * for our executable image and use the MOTLoad "execProgram" command to 
 * execute that image - thus protecting the "cold boot" image which is 
 * located in the boot block.  The explanation which follows describes
 * how to do this.
 *
 * Remember that no matter how the flash bank selection switch is set, the
 * base address (address of first megabyte) of the flash bank is
 * always 0xf8000000.  
 *
 * To execute a vxWorks "boot" image from ROM, perform the following:
 *
 * 1. Change #undef FIRMWARE_BOOT to #define FIRMWARE_BOOT.
 *
 * 2. ROM_BASE_ADRS will need to be set to the start address
 *    of the flash bank. Since we are specifying the start address
 *    (first megabyte) of the flash bank, the image that is resident in
 *    the boot block (cold boot image) will be protected.  So, if your
 *    image to execute is located in the first megabyte of the flash
 *    bank you would set ROM_BASE_ADRS to be 0xf8000000.
 * 3. Since ROM_TEXT_ADRS, ROM_WARM_ADRS, and ROM_LINK_ADRS are defined 
 *    in both the Makefile (hard-coded) and config.h (based on the value 
 *    of ROM_BASE_ADRS) it is necessary to adjust the values of 
 *    ROM_TEXT_ADRS, ROM_WARM_ADRS, and ROM_LINK_ADRS in the Makefile.  
 *    The values used are dependent upon the value of ROM_BASE_ADRS as 
 *    discussed in #2 above.  Based on the example above these values
 *    would be 0xf8000100, 0xf8000108, and 0xf8000100, respectively. 
 * 4. If FIRMWARE_BOOT is defined, USER_RESERVED_MEM will be set to
 *    0x100000 (1MB).  The reason for this is to setup the firmware booting
 *    for DRAM booting by default, rather than ROM booting.  If left as
 *    is, the top 1MB of DRAM will be reserved, which is not necessary when
 *    booting from ROM.  To change this simply change USER_RESERVED_MEM,
 *    located near the end of this file, to 0x0.
 *
 * To use the firmware to boot vxWorks from ROM use the following commands
 * (substitute the appropriate values where necessary).  The execP load address
 *  is dependent upon which flash bank the VxWorks image is located in.
 *
 * tftpGet -cXXX.XXX.XXX.XXX -fboot.bin -sXXX.XXX.XXX.XXX -d/dev/enetx
 * flashProgram -o0 -n100000
 * execProgram -lF8000100 -sFFF00
 */

#undef  FIRMWARE_BOOT

/*
 * Using software FP support. Athough task based 32 bit HW_FP is allowed 
 * no optimised libraries are available. The kernel doesn't execute any hw 
 * fp instructions
 */

#undef  INCLUDE_HW_FP
#define INCLUDE_SW_FP

/* 
 * Define the board type 
 * Possibilities are:
 *  MPC8540_BOARD_TYPE_XMC
 *  MPC8540_BOARD_TYPE_SBC
 *  MPC8540_BOARD_TYPE_PRPMC
 */

#define MPC8540_BOARD_TYPE  MPC8540_BOARD_TYPE_SBC

/* 
 * L1 Cache Support 
 *
 * In order for shared memory to function properly ensure that
 * USER_D_CACHE_ENABLE remains undef'd.
 */

#define USER_D_CACHE_ENABLE
#undef  USER_D_CACHE_MODE
#define USER_D_CACHE_MODE  (CACHE_COPYBACK)

#define USER_I_CACHE_ENABLE
#undef  USER_I_CACHE_MODE
#define USER_I_CACHE_MODE (CACHE_COPYBACK)

/* 
 * L2 Cache Support 
 *
 * The definition INCLUDE_L2_CACHE_SRAM, found above, controls
 * the inclusion of support for the L2 Cache/SRAM.  If #undef'ed
 * the L2 cache/SRAM will be disabled.
 *
 * The L2 Cache/SRAM has six possible configurations, other than 
 * disabling it, they are:
 *
 * (1) L2_CACHE_SRAM_CONFIG_1
 *
 *     Configure all 256 Kbytes as one block of cache.
 *
 * (2) L2_CACHE_SRAM_CONFIG_2
 *
 *     Configure one block (block 1, 128 Kbytes) as cache, block
 *     0 remains unused.
 *
 * (3) L2_CACHE_SRAM_CONFIG_3
 *
 *     Configure one block (block 0) as SRAM, and one block (block 1)
 *     as cache, each 128 Kbytes in size.
 *
 * (4) L2_CACHE_SRAM_CONFIG_4
 * 
 *     Configure all 256 Kbytes as one block of SRAM.
 *
 * (5) L2_CACHE_SRAM_CONFIG_5
 * 
 *     Configure both 128 Kbyte blocks as SRAM, for a total of
 *     256 Kbytes of SRAM.
 *
 * (6) L2_CACHE_SRAM_CONFIG_6
 *
 *     Configure one block (block 0, 128 Kbytes) as SRAM, block
 *     1 remains unused.
 *
 * The BSP default is L2_CACHE_SRAM_CONFIG_1.
 *
 * When any portion of the L2 Cache/SRAM is configured as cache the
 * mode (data, instruction, both) can be specified.  L2_MODE is used
 * to configure the mode.  The default is to allow cacheing of data 
 * and instructions (L2_MODE_INST_AND_DATA).
 * Valid settings also include: L2_MODE_I_ONLY and L2_MODE_D_ONLY.
 */

#define L2_CACHE_SRAM_CONFIG    L2_CACHE_SRAM_CONFIG_4
#define L2_MODE                 L2_MODE_INST_AND_DATA

/* 
 * When trying to connect the target with MemScope, this BSP requires to
 * enable extended 32-bit exception vector in the vxWorks image
 */

#undef  INCLUDE_EXC_EXTENDED_VECTORS

#if defined (INCLUDE_PRIMARY_TSEC_END) || defined (INCLUDE_SECONDARY_TSEC_END)
#   define INCLUDE_END                  /* only END-style driver for FCC */
#endif /* INCLUDE_MOT_TSEC_END */

/*
 * USB EHCI/OHCI Support 
 * 
 * EHCI/OHCI host controller support is included when INCLUDE_USB is defined
 * above. 
 *
 * Wind River's usbTool may also be optionally included below to perform
 * command line initialization and testing of the host controller and USB 
 * devices. If usbTool is included, then automatic initialization of USB 
 * devices will NOT be performed as the kernel starts. 
 *
 * Keyboard and Mouse class drivers are included by default, but may be 
 * undefined if not needed. Note the inclusion of bulk, CBI, and/or 
 * Pegasus drivers require additional parameters to be defined as noted below.
 *
 * See Wind River's USB Host Stack Programmer's Guide and USB Peripheral Stack
 * Programmer's Guide for additional information and descriptions of the
 * available class drivers that may defined. 
 */

#ifdef INCLUDE_USB

/* Optional command line USB control */

#   undef INCLUDE_USBTOOL

/* Add or remove USB Class drivers below */

#   define INCLUDE_USB_MOUSE
#   define INCLUDE_USB_KEYBOARD 
#   define INCLUDE_USB_MS_BULKONLY
#   undef  INCLUDE_USB_MS_CBI
#   undef  INCLUDE_USB_PRINTER
#   undef  INCLUDE_USB_SPEAKER
#   undef  INCLUDE_USB_PEGASUS_END

#   ifdef INCLUDE_USB_MS_CBI 
#       define CBI_DRIVE_NAME  x               /* define this name */
#   endif /* INCLUDE_USB_MS_CBI */

#   ifdef INCLUDE_USB_PEGASUS_END 
#       define PEGASUS_DESTINATION_ADDRESS x   /* define this address */
#       define PEGASUS_IP_ADDRESS x            /* define this address */
#       define PEGASUS_NET_MASK x              /* define this mask */
#       define PEGASUS_TARGET_NAME x           /* define this name */
#   endif /* INCLUDE_USB_PEGASUS_END */

#   define INCLUDE_HRFS
#   define INCLUDE_DOSFS

#endif /* INCLUDE_USB */

/* 
 * ATA Support 
 *
 * Only legacy mode ATA support is provided with this BSP.  The following
 * must be defined for the xbd interface to recognize dosFs formatted 
 * drives.  These are defined here also to support bootrom ATA builds.
 */

#ifdef INCLUDE_ATA
#   define INCLUDE_DOSFS
#   define INCLUDE_CBIO
#   define INCLUDE_RAWFS
#endif /* INCLUDE_ATA */

/* TrueFFS Support */

#ifdef INCLUDE_TFFS
#   define INCLUDE_DOSFS
#   define INCLUDE_TFFS_MOUNT
#endif /* INCLUDE_TFFS */

#ifdef INCLUDE_DOSFS
#   undef  INCLUDE_DOSFS_SHOW
#   define INCLUDE_DISK_UTIL       /* File system and disk utilities */
#   define INCLUDE_DISK_PART
#   define INCLUDE_DEVICE_MANAGER
#   define INCLUDE_DOSFS_DIR_FIXED /* DOS File system old directory format handler */
#   define INCLUDE_DOSFS_CHKDSK    /* DOS file system consistency checker */
#   define INCLUDE_DOSFS_DIR_VFAT  /* DOS file system VFAT directory handler */
#   define INCLUDE_DOSFS_CACHE
#   define INCLUDE_DOSFS_FAT       /* DOS file system volume formatter */
#   define INCLUDE_DOSFS_FMT
#   define INCLUDE_DOSFS_MAIN
#   define INCLUDE_ERF
#   define INCLUDE_FS_EVENT_UTIL
#   define INCLUDE_FS_MONITOR
#   define INCLUDE_XBD             /* Extended block device */
#   define INCLUDE_XBD_BLK_DEV     /* XBD-block device wrapper */
#   define INCLUDE_XBD_PART_LIB    /* XBD Partition Library */
#   define INCLUDE_XBD_TRANS
#endif /* INCLUDE_DOSFS */

/*
 * Shared Memory Support
 *
 * The following must be #define'd the same on all nodes.
 */

#define SM_MEM_ADRS             0x00010000      /* Start of actual SM region */
#define SM_MEM_SIZE             0x00020000
#define SM_OBJ_MEM_ADRS         (SM_MEM_ADRS+SM_MEM_SIZE) /* SM Objects pool */
#define SM_OBJ_MEM_SIZE         0x00010000
#define SM_OFF_BOARD            FALSE 
#define CPU_NUM_OF_SM_LOCATION  0

/*
 * Changing SM_OFF_BOARD to TRUE also requires changing  SM_ANCHOR_ADRS and
 * SM_MEM_ADRS appropriately, as the following #define's do:
 */

#undef  SM_ANCHOR_ADRS
#undef  SM_BASE_ADRS
#define SM_BASE_ADRS  ((sysProcNumGet() == CPU_NUM_OF_SM_LOCATION) ? \
           ((char*)LOCAL_MEM_LOCAL_ADRS) : ((char*)VME_A32_MSTR_LOCAL))

#define SM_ANCHOR_ADRS  ((char*) (SM_BASE_ADRS + SM_ANCHOR_OFFSET))

#define SM_INT_TYPE SM_INT_MAILBOX_1    /* or SM_INT_MAILBOX_1 */

#if defined(INCLUDE_SM_NET)
#   define INCLUDE_SMEND
#   define INCLUDE_SM_COMMON
#   define INCLUDE_SM_NET_SHOW
#   define INCLUDE_NET_DRV
#   define INCLUDE_NET_LIB
#   ifdef  STANDALONE
#       define STANDALONE_NET
#   endif  /* STANDALONE */
#endif /* defined(INCLUDE_SM_NET) */

/*
 * There are two methods of "hardware assist" available for
 * implementing the sysBusTas() function.  They are "VME bus locking"
 * and "VME bus RMW".  Which of these two methods is used is goverened
 * by the defined/undefined state of ANY_BRDS_IN_CHASSIS_NOT_RMW.
 * Having this symbol "#undef"ed will cause "VME bus RMW" to be used.
 * This is the required setting for Rev 1 of the Tempe (Tsi148 PCI-VME) 
 * Chip.  When Rev 2 of the Tempe chip is available, then you will have 
 * the option of "#define"ing ANY_BRDS_IN_CHASSIS_NOT_RMW to force the
 * use of VME bus locking to assist sysBusTas().
 */

#undef ANY_BRDS_IN_CHASSIS_NOT_RMW      /* required for Rev 1 of Tsi148 */

#undef VME_TAS_HW_ASSIST

#ifdef ANY_BRDS_IN_CHASSIS_NOT_RMW
#   define VME_TAS_HW_ASSIST    VME_BUS_LOCK
#else
#   define VME_TAS_HW_ASSIST    VME_BUS_RMW
#endif

/*
 * AuxClk Support
 *
 * There are a total of four 32-bit global timers on the MPC8540.  One of the
 * timers is used to implement the VxWorks AuxClk.  The following definition
 * specifies which of the four (0 through 3) is used to implement the AuxClk
 * and is user-configurable.
 */

#define MPC8540_AUXCLK_TIMER    0

#ifdef INCLUDE_MOT_FEC_END
#   ifndef INCLUDE_END
#       define INCLUDE_END              /* only END-style driver for FEC */
#   endif /* INCLUDE_END */
#endif /* INCLUDE_MOT_FEC_END */

/*
 * PCI and PCI Auto-Configuration Support
 *
 * PCI_MSTR_IO_SIZE, PCI_MSTR_MEMIO_SIZE and PCI_MSTR_MEM_SIZE
 * control the sizes of the available PCI address spaces. The windows 
 * defined by these parameters must be large enough to accommodate all 
 * of the PCI memory and I/O space requests found during PCI autoconfiguration.
 * If they are not, some devices will not be autoconfigured.
 *
 * NOTE: Since PCI auto-configuration is performed by the bootroms,
 * changing any of these values requires the creation of new bootroms.
 *
 * CPU Space                                   PCI (|) or VME (}) Space
 * (default values)
 *
 * LOCAL_MEM_LOCAL_ADRS =        -----------------  0x00000000
 *              (0x00000000)    |      DRAM       |
 *                              :                 :
 *                              :                 :
 * VME_A32_MSTR_LOCAL =          -----------------  VME_A32_MSTR_BUS
 *              (0x80000000)    |                 } (0x08000000)
 *                              | VME A32 space   } .
 *                              |    256MB        } . Tempe outbound window 1
 *                              | (0x10000000)    } .
 *              (0x87ffffff)    |                 } (0x0fffffff)
 *                              |.................}
 *                              | Unused A32      }
 *              (0x8fff0000)    |.................} (0xfb000000)
 *                              |                 } .
 *                              |                 } . Tempe outbound window 0
 *                              |                 } .
 *              (0x8fffffff)    |                 } (0xfb00ffff)
 * VME_A24_MSTR_LOCAL =          -----------------
 * VME_A32_MSTR_LOCAL +         |                 } (0x00000000)
 * VME_A32_MSTR_SIZE =          | VME A24 space   } .
 *              (0x90000000)    |    16MB         } . Tempe outbound window 2
 *                              | (0x01000000)    } .
 *              (0x90ffffff)    |                 } (0x00ffffff)
 * VME_A16_MSTR_LOCAL =          -----------------
 * VME_A24_MSTR_LOCAL +         |                 } (0x00000000)
 * VME_A24_MSTR_SIZE =          | VME A16 space   } .
 *              (0x91000000)    |    64KB         } . Tempe outbound window 3
 *                              | (0x00010000)    } .
 *              (0x910fffff)    |                 } (0x0000ffff)
 *                               -----------------
 *                              :                 :
 *                              :  Not used       :
 *                              :  239 MB         :
 *                              : (0x0ef00000)    :
 *                              : Available for   :
 *                              : expansion of    :
 *                              : VME space       :
 * PCI_MSTR_MEMIO_LOCAL =        ----------------- PCI_MSTR_MEMIO_BUS =
 *              (0xa0000000)    |                 |  PCI_MSTR_MEMIO_LOCAL =
 *                              | NonPrefetch PCI |  (0xa0000000)
 *                              | Memory space    |
 *                              |      8MB        |
 *                              | (0x00800000)    |
 *                              |                 |
 * PCI_MSTR_MEM_LOCAL =          ----------------- PCI_MSTR_MEM_BUS =
 * PCI_MSTR_MEMIO_LOCAL +       |                 |  PCI_MSTR_MEM_LOCAL
 * PCI_MSTR_MEMIO_SIZE =        | Prefetch PCI    |  (0xa0800000)
 *              (0xa0800000)    | Memory space    |
 *                              |    8MB          |
 *                              | (0x00800000)    |
 * ISA_MSTR_IO_LOCAL =           ----------------- ISA_MSTR_IO_BUS =
 * PCI_MSTR_MEM_LOCAL +         |                 |   (0x00000000)
 * PCI_MSTR_MEM_SIZE            |  I/O space      |
 *              (0xa1000000)    |    64KB         |
 *                              | (0x00010000)    |
 * PCI_MSTR_IO_LOCAL =           ----------------- PCI_MSTR_IO_BUS =
 * ISA_MSTR_IO_LOCAL +          |                 |   (0x00010000)
 * ISA_MSTR_IO_SIZE =           |   I/O space     |
 *              (0xa1010000)    |   8MB - 64KB    |
 *                              | (0x007f0000)    |
 *              (0xa1800000)     ----------------- (0x01000000)
 *                              :                 :
 *                              : Not used,       :
 *                              : Available for   :
 *                              : expansion of    :
 *                              : PCI spaces      :
 *                              :                 :
 *                              :                 :
 * CCSBAR =                      -----------------
 *              (0xe1000000)    | MPC8540         :
 *                              | internal regs.  :
 *                              |    64K          :
 *                              | (0x00100000)    :
 *              (0xe1100000)     -----------------
 *                              :     Unused      :
 * MPC8540_L2_SRAM_BASE          -----------------
 *            = (0xe1100000)    : Integrated SRAM :
 *                              :     256K        :
 *                              : (0x00040000)    :
 *              (0xe1140000)     -----------------
 *                              :     Unused      :
 * LOCAL_BOARD_ACCESS_ADRS       -----------------
 *              (0xe2000000)    :   MVME3100      :
 *                              : Control/Status  :
 *                              :   Registers     :
 *                              :   Quad UARTs    :
 *                              :  Tick Timers    :
 *                              :  (0x01000000)   :
 *              (0xe3000000)     -----------------
 *                              :     Unused      :
 *                              :                 :
 *                              :                 :
 * Flash Bank (dynamic) =        -----------------
 *              (0xf8000000)    |   Flash Bank    :
 *              (0xFc000000)    |  32MB - 128MB   :
 *              (0xfe000000)    |                 :
 *                              |                 :
 *                              |                 :
 *              (0xffffffff)     -----------------
 *
 * Notes on inbound translations from VME bus:
 *
 * Tempe inbound window 0 in support of shared-memory:
 *
 * On the processor node compiled with SM_OFF_BOARD set to FALSE, an
 * inbound window is opened using Tempe's inbound window 0 to map the
 * shared-memory anchor and shared-memory pool.
 *
 * The inbound access is shown here (output from sysTempeInWinShow(0)):
 *
 * Inbound window 0: ENABLED,  Size = 00000000_00040000
 *
 * raw: 00000000 08000000 00000000 0803fff0 ffffffff f8000000 80000faf
 * VME Base:  00000000_08000000    PCI Base:  00000000_00000000
 * VME Limit: 00000000_0803ffff    PCI Limit: 00000000_0003ffff
 *   Attributes:
 *      Read-ahead threshold - when FIFO competely empty
 *      Virtual FIFO size    - 64 bytes
 *      2eSST Mode           - 160 MB/s
 *      Cycle response       - 2eSSTB (Two Edge Source Synchronous Broadcast)
 *                             2eSST (Two Edge Source Synchronous nonBroadcast)
 *                             2eVME (Two Edge VMEbus)
 *                             MBLT (Multiplexed Block Transfer)
 *                             BLT (Block Transfer)
 *      Address space        - A32
 *      VME AM response      - Supervisor, nonSupervisor, Program, Data
 *
 * Here's a recap:
 *
 * If this is not the anchor node (SM_OFF_BOARD == TRUE)
 * then
 *     No A32 inbound mappings are programmed for access to DRAM.
 *     Note that all nodes must have inbound mapping to reach
 *     their mailbox's so shared memory doorbells can
 *     be hit by any other node.  This is taken care of by the
 *     RAI register programming on Universe equipped boards and
 *     the CRG programming by Tempe equipped boards.
 * else
 *     (this is the anchor node ...)
 *     A single inbound mapping through Tempe window 0 is configured,
 *     mapping DRAM address 0x00000000 through 0x0003ffff to VME
 *     bus address 0x08000000 through 0x0803ffff.
 *
 */

/* VME support */

#define VME_A64_MSTR_SIZE       (0x00000000)      /* 0 MB */
#define VME_A32_MSTR_SIZE       (0x10000000)      /* 256 MB */
#define VME_A24_MSTR_SIZE       (0x01000000)      /* 16 MB */
#define VME_A16_MSTR_SIZE       (0x00010000)      /* 64 KB */

/* PCI and PCI Auto-Configuration Support */

#define PCI_MSTR_MEMIO_SIZE     (0x00800000)
#define PCI_MSTR_MEM_SIZE       (0x00800000)
#define ISA_MSTR_IO_SIZE        (0x00010000)
#define PCI_MSTR_IO_SIZE        (0x007f0000)

/* Memory addresses */

#define LOCAL_MEM_LOCAL_ADRS    0x00000000      /* Base of RAM */

/*
 * The constants ROM_TEXT_ADRS, ROM_SIZE are defined in config.h and Makefile.
 * All definitions for these constants must be identical.
 */

#ifndef FIRMWARE_BOOT
#   define ROM_BASE_ADRS        0xfff00000
#   define USER_RESERVED_MEM    0x00000000
#else                           
#   define ROM_BASE_ADRS        0x1ff00000
#   define USER_RESERVED_MEM    0x00100000
#endif

#define ROM_TEXT_ADRS           (ROM_BASE_ADRS + 0x100)
#define ROM_WARM_ADRS           (ROM_BASE_ADRS + 0x108)
#define ROM_LINK_ADRS           (ROM_TEXT_ADRS)
#define ROM_SIZE                0x00100000

#define INCLUDE_VXBUS
#define INCLUDE_VXB_CMDLINE
#undef  INCLUDE_SHOW_ROUTINES
#undef  INCLUDE_VXBUS_SHOW
#define DRV_PCI_VIRTUAL
#define INCLUDE_PLB_BUS
#define INCLUDE_PARAM_SYS
#define INCLUDE_HWMEM_ALLOC
#define HWMEM_POOL_SIZE         50000

#define DRV_SIO_NS16550
#define INCLUDE_SIO_UTILS

#define DRV_TIMER_M85XX
#define INCLUDE_TIMER_SYS

/*
 * MPC8540 Interrupt Configuration
 *
 * The MPC8540 interrupt controller controls a number of interrupt
 * sources.  The sources are:
 *
 * 12 external or off-chip sources (vectors 0 - 11)
 * 23 internal or on-chip sources (vectors 12 - 34)
 * 4 global timer sources (vectors 35 - 38)
 * 4 message register sources (vectors 39 - 42)
 * 4 inter-processor sources (vectors 43 - 46)
 *
 * The table below is used to configure the priority for each of the
 * interrupt sources, in the order listed above, and to configure the 
 * polarity and sense for external interrupt sources.
 *
 * Each row in the table represents one interrupt source.  The first
 * entry in each row is the register offset for that particular 
 * interrupt source's vector/priority register.  The second entry covers
 * the interrupt source's configuration values for priority, polarity,
 * and sense.  The third entry is the interrupt source's vector
 * number.  The priority is the only parameter that is meant to be
 * user-configurable.  The priority can have a value between 0 and 15, 
 * 0 is the lowest and 15 is the highest.  Caution: a priority of 0 
 * will disable interrupts from that source.  The last (or fourth) entry
 * is used to specify whether the interrupt is to be configured as a 
 * critical interrupt or not.
 */

#define INTERRUPT_CONFIG_TABLE \
static UINT32 intConfig [][4] = \
    { \
        { CCSR_EIVPR0, EPIC_PRIORITY_3 | EPIC_INT_ACT_LOW | \
          EPIC_SENSE_LVL, EPIC_VEC_EXT_IRQ0, EPIC_CRIT_INT_FALSE }, \
        { CCSR_EIVPR1, EPIC_PRIORITY_3 | EPIC_INT_ACT_LOW | \
          EPIC_SENSE_LVL, EPIC_VEC_EXT_IRQ1, EPIC_CRIT_INT_FALSE }, \
        { CCSR_EIVPR2, EPIC_PRIORITY_3 | EPIC_INT_ACT_LOW | \
          EPIC_SENSE_LVL, EPIC_VEC_EXT_IRQ2, EPIC_CRIT_INT_FALSE }, \
        { CCSR_EIVPR3, EPIC_PRIORITY_3 | EPIC_INT_ACT_LOW | \
          EPIC_SENSE_LVL, EPIC_VEC_EXT_IRQ3, EPIC_CRIT_INT_FALSE }, \
        { CCSR_EIVPR4, EPIC_PRIORITY_3 | EPIC_INT_ACT_LOW | \
          EPIC_SENSE_LVL, EPIC_VEC_EXT_IRQ4, EPIC_CRIT_INT_FALSE }, \
        { CCSR_EIVPR5, EPIC_PRIORITY_3 | EPIC_INT_ACT_LOW | \
          EPIC_SENSE_LVL, EPIC_VEC_EXT_IRQ5, EPIC_CRIT_INT_FALSE }, \
        { CCSR_EIVPR6, EPIC_PRIORITY_3 | EPIC_INT_ACT_LOW | \
          EPIC_SENSE_EDG, EPIC_VEC_EXT_IRQ6, EPIC_CRIT_INT_FALSE }, \
        { CCSR_EIVPR7, EPIC_PRIORITY_3 | EPIC_INT_ACT_LOW | \
          EPIC_SENSE_LVL, EPIC_VEC_EXT_IRQ7, EPIC_CRIT_INT_FALSE }, \
        { CCSR_EIVPR8, EPIC_PRIORITY_3 | EPIC_INT_ACT_LOW | \
          EPIC_SENSE_LVL, EPIC_VEC_EXT_IRQ8, EPIC_CRIT_INT_FALSE }, \
        { CCSR_EIVPR9, EPIC_PRIORITY_3 | EPIC_INT_ACT_LOW | \
          EPIC_SENSE_LVL, EPIC_VEC_EXT_IRQ9, EPIC_CRIT_INT_FALSE }, \
        { CCSR_EIVPR10, EPIC_PRIORITY_3 | EPIC_INT_ACT_LOW | \
          EPIC_SENSE_LVL, EPIC_VEC_EXT_IRQ10, EPIC_CRIT_INT_FALSE }, \
        { CCSR_EIVPR11, EPIC_PRIORITY_3 | EPIC_INT_ACT_LOW | \
          EPIC_SENSE_LVL, EPIC_VEC_EXT_IRQ11, EPIC_CRIT_INT_FALSE }, \
        { CCSR_IIVPR0, EPIC_PRIORITY_3 | EPIC_INT_ACT_HIGH, \
          EPIC_L2CACHE_INT_VEC, EPIC_CRIT_INT_FALSE }, \
        { CCSR_IIVPR1, EPIC_PRIORITY_3 | EPIC_INT_ACT_HIGH, \
          EPIC_ECM_INT_VEC, EPIC_CRIT_INT_FALSE }, \
        { CCSR_IIVPR2, EPIC_PRIORITY_3 | EPIC_INT_ACT_HIGH, \
          EPIC_DDR_INT_VEC, EPIC_CRIT_INT_FALSE }, \
        { CCSR_IIVPR3, EPIC_PRIORITY_3 | EPIC_INT_ACT_HIGH, \
          EPIC_LBC_INT_VEC, EPIC_CRIT_INT_FALSE }, \
        { CCSR_IIVPR4, EPIC_PRIORITY_3 | EPIC_INT_ACT_HIGH, \
          EPIC_DMA0_INT_VEC, EPIC_CRIT_INT_FALSE }, \
        { CCSR_IIVPR5, EPIC_PRIORITY_3 | EPIC_INT_ACT_HIGH, \
          EPIC_DMA1_INT_VEC, EPIC_CRIT_INT_FALSE }, \
        { CCSR_IIVPR6, EPIC_PRIORITY_3 | EPIC_INT_ACT_HIGH, \
          EPIC_DMA2_INT_VEC, EPIC_CRIT_INT_FALSE }, \
        { CCSR_IIVPR7, EPIC_PRIORITY_3 | EPIC_INT_ACT_HIGH, \
          EPIC_DMA3_INT_VEC, EPIC_CRIT_INT_FALSE }, \
        { CCSR_IIVPR8, EPIC_PRIORITY_3 | EPIC_INT_ACT_HIGH, \
          EPIC_PCI_INT_VEC, EPIC_CRIT_INT_FALSE }, \
        { CCSR_IIVPR9, EPIC_PRIORITY_3 | EPIC_INT_ACT_HIGH, \
          EPIC_RIOWRERR_INT_VEC, EPIC_CRIT_INT_FALSE }, \
        { CCSR_IIVPR10, EPIC_PRIORITY_3 | EPIC_INT_ACT_HIGH, \
          EPIC_RIODBELL_INT_VEC, EPIC_CRIT_INT_FALSE }, \
        { CCSR_IIVPR11, EPIC_PRIORITY_3 | EPIC_INT_ACT_HIGH, \
          EPIC_RIOMSGOUT_INT_VEC, EPIC_CRIT_INT_FALSE }, \
        { CCSR_IIVPR12, EPIC_PRIORITY_3 | EPIC_INT_ACT_HIGH, \
          EPIC_RIOMSGIN_INT_VEC, EPIC_CRIT_INT_FALSE }, \
        { CCSR_IIVPR13, EPIC_PRIORITY_3 | EPIC_INT_ACT_HIGH, \
          EPIC_TSEC1TX_INT_VEC, EPIC_CRIT_INT_FALSE }, \
        { CCSR_IIVPR14, EPIC_PRIORITY_3 | EPIC_INT_ACT_HIGH, \
          EPIC_TSEC1RX_INT_VEC, EPIC_CRIT_INT_FALSE }, \
        { CCSR_IIVPR18, EPIC_PRIORITY_3 | EPIC_INT_ACT_HIGH, \
          EPIC_TSEC1ERR_INT_VEC, EPIC_CRIT_INT_FALSE }, \
        { CCSR_IIVPR19, EPIC_PRIORITY_3 | EPIC_INT_ACT_HIGH, \
          EPIC_TSEC2TX_INT_VEC, EPIC_CRIT_INT_FALSE }, \
        { CCSR_IIVPR20, EPIC_PRIORITY_3 | EPIC_INT_ACT_HIGH, \
          EPIC_TSEC2RX_INT_VEC, EPIC_CRIT_INT_FALSE }, \
        { CCSR_IIVPR24, EPIC_PRIORITY_3 | EPIC_INT_ACT_HIGH, \
          EPIC_TSEC2ERR_INT_VEC, EPIC_CRIT_INT_FALSE }, \
        { CCSR_IIVPR25, EPIC_PRIORITY_3 | EPIC_INT_ACT_HIGH, \
          EPIC_FEC_INT_VEC, EPIC_CRIT_INT_FALSE }, \
        { CCSR_IIVPR26, EPIC_PRIORITY_3 | EPIC_INT_ACT_HIGH, \
          EPIC_DUART_INT_VEC, EPIC_CRIT_INT_FALSE }, \
        { CCSR_IIVPR27, EPIC_PRIORITY_3 | EPIC_INT_ACT_HIGH, \
          EPIC_I2C_INT_VEC, EPIC_CRIT_INT_FALSE }, \
        { CCSR_IIVPR28, EPIC_PRIORITY_3 | EPIC_INT_ACT_HIGH, \
          EPIC_PERFMON_INT_VEC, EPIC_CRIT_INT_FALSE }, \
        { CCSR_GLBL_GTVPR0, EPIC_PRIORITY_3, EPIC_VEC_GT_IRQ0, \
          EPIC_CRIT_INT_NONE }, \
        { CCSR_GLBL_GTVPR1, EPIC_PRIORITY_3, EPIC_VEC_GT_IRQ1, \
          EPIC_CRIT_INT_NONE }, \
        { CCSR_GLBL_GTVPR2, EPIC_PRIORITY_3, EPIC_VEC_GT_IRQ2, \
          EPIC_CRIT_INT_NONE }, \
        { CCSR_GLBL_GTVPR3, EPIC_PRIORITY_3, EPIC_VEC_GT_IRQ3, \
          EPIC_CRIT_INT_NONE }, \
        { CCSR_MIVPR0, EPIC_PRIORITY_3, EPIC_VEC_MSG_IRQ0, \
          EPIC_CRIT_INT_FALSE }, \
        { CCSR_MIVPR1, EPIC_PRIORITY_3, EPIC_VEC_MSG_IRQ1, \
          EPIC_CRIT_INT_FALSE }, \
        { CCSR_MIVPR2, EPIC_PRIORITY_3, EPIC_VEC_MSG_IRQ2, \
          EPIC_CRIT_INT_FALSE }, \
        { CCSR_MIVPR3, EPIC_PRIORITY_3, EPIC_VEC_MSG_IRQ3, \
          EPIC_CRIT_INT_FALSE }, \
        { CCSR_GLBL_IPIVPR0, EPIC_PRIORITY_3, EPIC_VEC_IPI_IRQ0, \
          EPIC_CRIT_INT_NONE }, \
        { CCSR_GLBL_IPIVPR1, EPIC_PRIORITY_3, EPIC_VEC_IPI_IRQ1, \
          EPIC_CRIT_INT_NONE }, \
        { CCSR_GLBL_IPIVPR2, EPIC_PRIORITY_3, EPIC_VEC_IPI_IRQ2, \
          EPIC_CRIT_INT_NONE }, \
        { CCSR_GLBL_IPIVPR3, EPIC_PRIORITY_3, EPIC_VEC_IPI_IRQ3, \
          EPIC_CRIT_INT_NONE }, \
        { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}, /* end of array */ \
    };

#define USR_INT_ENABLE  NULL    /* Replace with valid intEnable fcn ptr */
#define USR_INT_DISABLE NULL    /* Replace with valid intDisable fcn ptr */

#define INT_USR_EXTRA ,{ USR_INT_NUM_MIN,USR_INT_NUM_MAX, \
                         USR_INT_ENABLE, USR_INT_DISABLE }

/*
 * Define the priority list for the Tempe interrupt support.  List items
 * correspond to bit postions in the interrupt enable register.  The
 * first item in the list is the first one scanned when a Tempe interrupt
 * occurs.
 */

#define TEMPE_INT_PRIORITIES \
        TEMPE_INT_NUM_IRQ1_BIT,     TEMPE_INT_NUM_IRQ2_BIT, \
        TEMPE_INT_NUM_IRQ3_BIT,     TEMPE_INT_NUM_IRQ4_BIT, \
        TEMPE_INT_NUM_IRQ5_BIT,     TEMPE_INT_NUM_IRQ6_BIT, \
        TEMPE_INT_NUM_IRQ7_BIT,     TEMPE_INT_NUM_ACFL_BIT, \
        TEMPE_INT_NUM_SYSFL_BIT,    TEMPE_INT_NUM_IACK_BIT, \
        TEMPE_INT_NUM_VIEEN_BIT,    TEMPE_INT_NUM_VERR_BIT, \
        TEMPE_INT_NUM_PERR_BIT,     TEMPE_INT_NUM_MB0_BIT,  \
        TEMPE_INT_NUM_MB1_BIT,      TEMPE_INT_NUM_MB2_BIT,  \
        TEMPE_INT_NUM_MB3_BIT,      TEMPE_INT_NUM_LM0_BIT,  \
        TEMPE_INT_NUM_LM1_BIT,      TEMPE_INT_NUM_LM2_BIT,  \
        TEMPE_INT_NUM_LM3_BIT,      TEMPE_INT_NUM_DMA0_BIT, \
        TEMPE_INT_NUM_DMA1_BIT,     0xff    /* end of list */

#include "xxx8540A.h"   /* board specific definitions */

#ifdef __cplusplus
    }
#endif /* __cplusplus */

#if defined(PRJ_BUILD)
#   include "prjParams.h"
#endif /* PRJ_BUILD */

#include "xxx8540B.h"	/* definition dependencies */

#endif  /* INCconfigh */
