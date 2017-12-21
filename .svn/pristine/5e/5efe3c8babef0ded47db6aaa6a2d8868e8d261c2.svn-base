/* config.h - Wind River wrSbcP4080 BSP configuration file */

/*
 * Copyright (c) 2010-2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01s,22feb13,d_l  uprev BSP_REV to "6.9/7"
01r,21sep12,l_z  Replace INCLUDE_TIMER_STUB with INCLUDE_TIMER_EARLY_VXB_DELAY.
                 (WIND00377568)
01q,04jun12,c_l  added macro definition for errata A-004510 and A-004511. 
                 (WIND00352077) (WIND00349563)
01p,25may12,mpc  added workaround for Freescale A-004529. (WIND00350649)
01o,09apr12,h_k  updated BSP_REV for 6.9.2 UP4 release. (WIND00341977)
                 removed optional components.
                 removed redundant definitions and undefinitions.
01n,08mar12,y_y  add vxBus I2C support.
01m,29feb12,x_s  Remove the component INCLUDE_AIM_MMU_SHOW. (WIND00334904)
01l,28feb12,agf  update BSP_REV for UP3
01k,07feb12,x_s  Added macro definitions for two workarounds. (WIND00331187)
01j,09nov11,ers  WIND00297094: cache fix for vmPageLock/Unlock.
01i,23sep11,ers  Replaced DYNAMIC_WRLOAD with generic wrload. Updated BSP_REV.
01h,31aug11,wqi  fix SERDES errata procedure to not disturb
                 lanes used by Aurora. (WIND00298017)
01g,31aug11.swu  added macro OPTIMIZE_I2C for I2C. (WIND00290411)
01f,27jul11,e_d  updated BSP version
01e,21feb11,e_d  updated BSP version for vxWorks 6.9 and updated
                 codes from fsl_p4080_ds confing.h version 02o
01d,28dec10,e_d  modified SYS_CLK_RATE_MIN value.(WIND00248407)
01c,28dec10,e_d  modified LOCAL_MEM_SIZE value.(WIND00248406)
01b,25nov10,e_d  code clean
01a,21oct10,wap  created from fsl_p4080_ds config.h version 02n
*/

#ifndef __INCconfigh
#define __INCconfigh

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

#define BSP_VERSION     "6.9"
#define BSP_REV         "/7"

/*
 * The following addresses are used for multicore support. They represent
 * reserved memory and should be located where they will not get
 * overwritten by operating system or application software.
 *
 * Changing them will require reflashing the bootrom.
 */

#define CPUn_SPACE_BASE_ADRS    0x00100000 - 0x00010000 /* RAM_LOW_ADRS - 64KB of stack */
#define CPUn_SPACE_SIZE         0x00000400              /* must be 2^n */
#define CPUn_SPACE_SIZE_SHIFT   10                      /* used by romInit.s */
#define CPUn_SPACE(n)           ((CPUn_SPACE_BASE_ADRS - ( CPUn_SPACE_SIZE * (n) )))
#define CPUn_FUNC_START_ADR(n)  (CPUn_SPACE(n) + 0)
#define CPUn_STACK_START_ADR(n) (CPUn_SPACE(n) + 4)
#define CPUn_INIT_START_ADR(n)  (CPUn_SPACE(n) + 8)
#define CPUn_LOCAL_MEM_ERPN(n)  (CPUn_SPACE(n) + 12)
#define CPUn_LOCAL_MEM_RPN(n)   (CPUn_SPACE(n) + 16)

/*
 * Errata: A-003474 (Internal DDR calibration circuit is not supported) 
 * This workaround is implemented in bootrom (romInit.s).
 */

#define FSL_ERRATA_DDR_A003474

/*
 * Errata: A-003999 (Running Floating Point instructions requires special 
 * initialization), This workaround is implemented in boortom(romInit.s) 
 * and vxWorks image(sysLib.c).
 */

#define FSL_ERRATA_FPU_A003999

/*
 * Errata: A-004510 (Transactions can be lost in the CoreNet coherency fabric),
 * This workaround is implemented in bootrom(romInit.s) and vxWorks
 * image(sysLib.c).
 */

#define FSL_ERRATA_CCF_A004510

/*
 * Errata: A-004511 (Transactions may be incorrectly dropped from the wait
 * queue in the CoreNet coherency fabric), This workaround is implemented
 * in bootrom(romInit.s) and vxWorks image(sysLib.c).
 */

#define FSL_ERRATA_CCF_A004511

/*
 * Freescale AN4065 Rev A specifies a SerDes fix for Rev 2.0 silicon.
 * SERDES_FIX_CPU specifies the core that executes the errata work-around.
 * Users may choose to change the SERDES_FIX_CPU to a core other than 0, 
 * however this has not been verified by Wind River. SerDes lanes in banks
 * 2 & 3 are powered off during the work-around with implicit impact to any
 * secondary CPU's that may be trying to use the lanes in those banks. If
 * future silicon rev's resolve this errata undef this define.
 */

#define SERDES_FIX_CPU 0

/*
 * Workarounds for Freescale A-004529(Phantom Branch triggered by floating point
 * instruction can cause core to stall until interrupted):
 *
 * (1)Ensure that an interrupt can always be taken when executing floating point
 *    code and provide a source for such an interrupt on a periodic basis. Do 
 *    not execute any floating-point instructions when interrupts are blocked.
 * (2)Disable the BTB (BUCSR[BPEN]=0) when MSR[FP] is set to 1 and enable the 
 *    BTB when MSR[FP] is set to 0.
 *
 * Note: If you make sure your code meet workaround 1, you can undefine 
 *       FREESCALE_ERRATA_A004529_WORKAROUND_ENABLE for performance. Otherwise, you 
 *       should define it. But, this will cause performance drop about 50%.
 *       The issue is fixed in silicon Rev 3.0. So, nothing will be done even
 *       if FREESCALE_ERRATA_A004529_WORKAROUND_ENABLE is defined for Rev 3.0 or later.
 *       For Rev 1.0 devices are not supported by Freescale, they are not
 *       production devices. If you use Rev 1.0, please contact Freescale
 *       marketing team.
 */

#undef FREESCALE_ERRATA_A004529_WORKAROUND_ENABLE

#include <vsbConfig.h>
#include <configAll.h>

#ifndef _ASMLANGUAGE
#   include <hwif/intCtlr/vxbEpicIntCtlr.h>
#endif /* _ASMLANGUAGE */

#include "wrSbcP4080.h"

#undef INCLUDE_DDR_ECC

/* Common Includes for VXBUS PCI and DTSEC */

#define INCLUDE_VXBUS

#ifdef INCLUDE_VXBUS
#   define INCLUDE_VXB_CMDLINE
#   define INCLUDE_HWMEM_ALLOC
#   define INCLUDE_PARAM_SYS

#   define INCLUDE_PLB_BUS
#   define INCLUDE_END

#   ifdef INCLUDE_PCI_BUS
#      define INCLUDE_PCI_BUS_AUTOCONF
#      define DRV_PCIBUS_QORIQ_PCIEX
#   endif /* INCLUDE_PCI_BUS */

#   define DRV_RESOURCE_QORIQLAW
#   define DRV_RESOURCE_QORIQBMAN
#   define DRV_RESOURCE_QORIQQMAN
#   define DRV_RESOURCE_QORIQFMAN

#   define INCLUDE_INTCTLR_LIB
#   define DRV_INTCTLR_PPC
#   define DRV_INTCTLR_EPIC
#   define DRV_TIMER_OPENPIC
#   define INCLUDE_INTCTLR_DYNAMIC_LIB

#   define DRV_TIMER_M85XX
#   define INCLUDE_TIMER_SYS
#   define INCLUDE_TIMER_EARLY_VXB_DELAY

#   define DRV_SIO_NS16550
#   define INCLUDE_SIO_UTILS

#   define DRV_VXBEND_DTSEC
#   define INCLUDE_DMA_SYS

#   ifdef DRV_VXBEND_DTSEC

/*
 * A microcode patch is required for proper support of
 * soft parser rules, independent mode and coarse
 * classification.
 */

#      define INCLUDE_P4080_FMAN_UCODE

#      define DRV_MII_DTSEC_MDIO
#      define DRV_RESOURCE_FSLGPIO

/*
 * The wrSbcP4080 board uses SFP modules for some of its
 * DTSEC ports. These must be managed via I2C.
 */

#      define DRV_MII_I2C_MDIO
#      define INCLUDE_I2C_BUS

/*
 * The wrSbcP4080's SFP module need the Marvell 88E1111 driver
 * for proper link management.
 */

#      define INCLUDE_MV88E1X11PHY
#endif /* DRV_VXBEND_DTSEC */

#   define INCLUDE_END

#   define INCLUDE_MII_BUS
#   define INCLUDE_GENERICPHY

/*
 * The P4080 also has two 10 gigabit ethernet controllers.
 * With the wrSbcP4080 board, one 10GbE port (TGEC1) can be used
 * if an optional XFP media module is provided.
 */

#   define DRV_VXBEND_TGEC
#   define DRV_MII_DTEPHY


#define HWMEM_POOL_SIZE 100000

/*
 * This is required for SMP mode. When tNet0 is scheduled, we don't
 * have any way of predicting which core it will be running on when
 * it wakes up. But the frame manager is programmed to enqueue frames
 * to a portal belonging to a specific CPU (when a frame queue is
 * created, it's typically targeted at a particular CPU's portal).
 * We need to avoid the case where tNet0 and some other task both
 * try to dequeue from the same portal while running on different
 * cores in an SMP configuration, since then there would be
 * overlapping accesses to the same portal. To do this, we must
 * enforce the rule that if a task wants to dequeue a frame
 * descriptor from a given portal, it has to be running on the
 * CPU corresponding to that portal. To do that, we have to make
 * sure tNet0 always runs on the CPU specified as the target of
 * the DTSEC driver's frame queues, which in an UP and SMP
 * configuration defaults to 0.
 */

#   ifdef _WRS_CONFIG_SMP
#       define NET_DAEMONS_CPU_AFFINITY TRUE
#   endif /* WRS_VX_SMP */

#endif /* INCLUDE_VXBUS */

#define MAX_MAC_DEVS          5       /* two network devices (dtsec, tgec) */

/* default mac address */

#define ENET_DEFAULT0         WR_ENET0
#define ENET_DEFAULT1         WR_ENET1
#define ENET_DEFAULT2         WR_ENET2

#define DEFAULT_BOOT_LINE \
"dtsec(0,0)host:vxWorks h=192.168.0.1 e=192.168.0.2 u=vxworks pw=vxworks f=0x0"

/* MMU and CACHE */

#define INCLUDE_MMU_BASIC
#define USER_I_MMU_ENABLE
#define USER_D_MMU_ENABLE

#define CACHE_LIBRARY_MODE    (CACHE_COPYBACK | CACHE_SNOOP_ENABLE)
#define CAM_DRAM_CACHE_MODE   _MMU_TLB_ATTR_M
#define TLB_CACHE_MODE        (VM_STATE_CACHEABLE | VM_STATE_MEM_COHERENCY)

#undef  USER_D_CACHE_MODE
#define USER_D_CACHE_MODE     (CACHE_LIBRARY_MODE)
#undef  USER_I_CACHE_MODE
#define USER_I_CACHE_MODE     (CACHE_LIBRARY_MODE)

#define INCLUDE_L2_CACHE

#define INCLUDE_L3_CACHE
#define L3_WRITE_THRU_MODE  0   /* 1 => write thru, 0 => write back */

#define INCLUDE_BRANCH_PREDICTION

#undef  DRV_DMA_FSL
#undef  INCLUDE_FSL_SPI

/* I2C support */

#ifdef INCLUDE_I2C_BUS
#   define DRV_I2CBUS_FSL
#   define DRV_I2C_EEPROM
#   define INCLUDE_EEPROMDRV
#endif /* INCLUDE_I2C_BUS */

/* Serial channel and TTY */

#undef  NUM_TTY
#define NUM_TTY 1

#undef  CONSOLE_BAUD_RATE
#define CONSOLE_BAUD_RATE    115200

/* Clock rates */

#define SYS_CLK_RATE_MIN     1      /* minimum system clock rate */
#define SYS_CLK_RATE_MAX     8000   /* maximum system clock rate */
#define AUX_CLK_RATE_MIN     1      /* minimum auxiliary clock rate */
#define AUX_CLK_RATE_MAX     8000   /* maximum auxiliary clock rate */
#define WDT_RATE_MIN         ((sysClkFreqGet() >> 3) / (1 << 29))
#define WDT_RATE_MAX         ((sysClkFreqGet() >> 3) / (1 << 21))

/* TrueFFS flash support */

#ifdef  INCLUDE_TFFS
#   define FLASH_TFFS_SIZE    0x00800000
#   define FLASH_WINDOW_SIZE  0x00800000
#   define INCLUDE_TL_FTL
#   define INCLUDE_TFFS_MOUNT
#   define INCLUDE_DOSFS
#endif  /* INCLUDE_TFFS */

/* USB EHCI */

#ifdef INCLUDE_USB
#   define INCLUDE_USB_INIT
#   define INCLUDE_EHCI
#   define INCLUDE_EHCI_INIT
#   define INCLUDE_USB_MS_BULKONLY
#   define INCLUDE_USB_MS_BULKONLY_INIT
#   define INCLUDE_DOSFS
#endif  /* INCLUDE_USB */

/*  Define for FS */

#ifdef INCLUDE_DOSFS
#   define INCLUDE_ERF
#   define INCLUDE_DEVICE_MANAGER
#   define INCLUDE_FS_EVENT_UTIL
#   define INCLUDE_FS_MONITOR
#   define INCLUDE_XBD
#   define INCLUDE_XBD_BLK_DEV
#   define INCLUDE_XBD_TRANS
#   define INCLUDE_DOSFS_DIR_FIXED
#   define INCLUDE_DOSFS_DIR_VFAT
#   define INCLUDE_DOSFS_FAT
#   define INCLUDE_DOSFS_FMT
#   define INCLUDE_DOSFS_CHKDSK
#   define INCLUDE_DOSFS_MAIN
#endif  /* INCLUDE_DOSFS */

/* LBC CS0 - flash 0 - default for bootrom */

#define FLASH_BASE_ADRS                 0xff000000
#define FLASH_ADRS_MASK                 0xff000000
#define MAIN_FLASH_SIZE                 0x01000000

#define INCLUDE_LCD
#define INCLUDE_SYSLED

#if (defined(INCLUDE_LCD) || defined (INCLUDE_SYSLED))
#   define INCLUDE_CPLD_IO
#endif /* INCLUDE_LCD */

#ifdef INCLUDE_CPLD_IO
#   define CPLD_IO_LOCAL_ADRS           CPLD_BASE
#   define CPLD_IO_LOCAL_SIZE_MASK      0xFFFF0000
#   define CPLD_IO_SIZE                 CPLD_SIZE
#endif /* INCLUDE_CPLD_IO */

/* FLASH configuration */

#if ((defined INCLUDE_FLASH) || (defined INCLUDE_TFFS))
#   define FLASH_ADRS                   0xFF000000
#   define FLASH_SIZE                   0x1000000
#   define FLASH_WIDTH                  2
#   define FLASH_CHIP_WIDTH             2
#   define FLASH_SECTOR_SIZE            0x00020000
#   define FLASH_SECTOR_MASK            0xFFFE0000

#   define NV_RAM_ADRS                  (ROM_TEXT_ADRS - FLASH_SECTOR_SIZE)
#   define NV_RAM_SIZE                  FLASH_SECTOR_SIZE
#   define NV_MAC_ADRS_OFFSET           0x200
#endif /* INCLUDE_FLASH */

/*
 * By default, we stored the bootline in NVRAM,
 * So we should define INCLUDE_NV_RAM in config.h.
 */

#define INCLUDE_NV_RAM

#ifdef  INCLUDE_NV_RAM
#   undef  NV_RAM_ADRS
#   define NV_RAM_ADRS                  EEPROM_BASE
#   undef  NV_RAM_SIZE
#   define NV_RAM_SIZE                  EEPROM_SIZE
#   define NV_RAM_SIZE_MASK             EEPROM_SIZE_MASK
#   define NV_RAM_INTRVL                1
#   undef  NV_BOOT_OFFSET
#   define NV_BOOT_OFFSET               0
#   define NV_MAC_ADRS_OFFSET           0x200
#endif  /* INCLUDE_NV_RAM */

/* Memory addresses */

#define PHYS_MEM_START                  0x00000000

/*
 * Address of QMAN and BMAN private memory.
 * The QMAN driver will allocate 2MB starting from QMAN_MEM_BASE.
 * The BMAN driver will allocate 1MB starting from BMAN_MEM_BASE.
 * Changing the MEM_BASE's will require reflashing the bootrom.
 * This memory is reserved and should not be accessed by software.
 *
 * The QMAN and BMAN are passed the address of their private memory
 * _once_ at hw init. Once passed and initialized, the private memory
 * cannot be relocated. This has the implication that downloadable
 * kernels must know and respect whatever location the bootrom has
 * used to place the private memory. For this reason Wind River chose
 * to hard-code the private memory to a fixed location, as this could
 * be most easily supported in a reference BSP which must implement
 * all possible configurations.
 *
 * vxWorks has multiple methods for allocating reserved memory. For
 * this BSP we have chosen to reserve the memory by hard-coding it
 * at a place immediately above LOCAL_MEM_SIZE. Another method would
 * have been to place it below RAM_LOW_ADRS. Users without Wind River's
 * legacy bootrom considerations should feel free to relocate and
 * reserve the private space however best suits their application.
 *
 */

#define QMAN_BMAN_RESERVED_MEM          0x00300000
#define QMAN_MEM_BASE                   PHYS_MEM_SIZE - QMAN_BMAN_RESERVED_MEM
#define BMAN_MEM_BASE                   QMAN_MEM_BASE + 0x00200000

#define ROM_TEXT_ADRS                   0xFFF00100
#define ROM_BASE_ADRS                   0xFFF00000
#define ROM_SIZE                        0x00100000

/*
 * LOCAL_MEM_SIZE only can be defined in config.h when commandline build.
 * So we add CDF_OVERRIDE for rmove this define when prj build.
 */

#ifndef CDF_OVERRIDE
#   define LOCAL_MEM_SIZE               PHYS_MEM_SIZE - QMAN_BMAN_RESERVED_MEM
#   define LOCAL_MEM_LOCAL_ADRS         PHYS_MEM_START
#endif /* CDF_OVERRIDE */

#define DDR_SDRAM_LOCAL_ADRS            PHYS_MEM_START
#define DDR_SDRAM_LAWAR_SIZE            LAWAR_SIZE_2GB
#define DDR_SDRAM_LOCAL_END             (PHYS_MEM_START + 0x3fffffff)
#define DDR_SDRAM_LOCAL_ADRS2           (PHYS_MEM_START + 0x40000000)
#define DDR_SDRAM_LOCAL_END2            (PHYS_MEM_START + 0x7fffffff)
#define PHYS_MEM_SIZE                   0x80000000

#ifdef IPBASIC

/* remove power management for bootrom */

#   undef INCLUDE_CPU_PWR_MGMT
#endif /* IPBASIC */

/* XXX this may be removed eventually */

#define LOCAL_MEM_LOCAL_OFFSET          0

/*
 * NEW 36-BIT CONFIGURATION MACROS.
 *
 * LOCAL_MEM_ERPN specifies the upper 4 bits of the physical address of RAM
 * used by this image.  It cannot be set to anything other than 0 for CPU0.
 * It should be configured using the project facility or vxprj.
 *
 * DDR_SDRAM_LOCAL_ADRS_H specifies the upper 4 bits of the physical address
 * of the first bank of RAM.  This bank is what is used by CPU0 and any CPUs
 * configured with a LOCAL_MEM_ERPN of 0.
 *
 * DDR_SDRAM_LOCAL_ADRS2_H specifies the upper 4 bits of the physical address
 * of the second bank of RAM.  This bank can be used by any CPU configured
 * with a LOCAL_MEM_ERPN equal to DDR_SDRAM_LOCAL_ADRS2_H.
 *
 */

#ifndef PRJ_BUILD
#   define LOCAL_MEM_ERPN               0
#endif /* PRJ_BUILD */

#define DDR_SDRAM_LOCAL_ADRS_H          0
#define DDR_SDRAM_LOCAL_ADRS2_H         2

#undef  VEC_BASE_ADRS
#define VEC_BASE_ADRS                   LOCAL_MEM_LOCAL_ADRS

#ifdef INCLUDE_MMU_BASIC

#   define INCLUDE_AIM_MMU_CONFIG
#   define INCLUDE_AIM_MMU_MEM_POOL_CONFIG

/*
 * The following parameters are to configure initial memory usage for
 * page table and region tables and in event of running out the increment
 * of memory allocation and is specified as a number of MMU pages (4KByte
 * blocks).
 */

#   define AIM_MMU_INIT_PT_NUM          0x40
#   define AIM_MMU_INIT_PT_INCR         0x20
#   define AIM_MMU_INIT_RT_NUM          0x10
#   define AIM_MMU_INIT_RT_INCR         0x10

#endif /* INCLUDE_MMU_BASIC */

/*
* Do not clear memory in romStart() since it is not necessary,
* and slows boot time.
*/

#undef  ROMSTART_BOOT_CLEAR

/* user reserved memory, see sysMemTop() */

#define USER_RESERVED_MEM               0x00000000

#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif  /* __INCconfigh */

#if defined(PRJ_BUILD)
    #include "prjParams.h"
#endif /* PRJ_BUILD */
