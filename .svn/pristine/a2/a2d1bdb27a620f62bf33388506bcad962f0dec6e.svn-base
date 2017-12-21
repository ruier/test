/* config.h - Freescale QorIQ P4080 DS BSP configuration file */

/*
 * Copyright (c) 2008-2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
03d,29jul13,d_l  Update BSP_REV to "6.9/6".
03c,11jan13,ylu  Update macro definitions to support Rev 3 silicon.
03b,04jun12,c_l  added macro definition for errata A-004510 and A-004511.
                 (WIND00352077) (WIND00349563)
03a,25may12,mpc  added workaround for Freescale A-004529. (WIND00350649)
02z,27mar12,h_k  updated BSP_REV for 6.9.2 UP4 release. (WIND00341977)
                 removed optional components.
                 removed redundant definitions and undefinitions.
02y,29feb12,x_s  Remove the component INCLUDE_AIM_MMU_SHOW. (WIND00334904)
02x,28feb11,agf  Update BSP_REV for vxWorks 6.9.2 UP3
02w,07feb12,x_s  Added macro definitions for two workarounds. (WIND00331187)
02v,03nov11,ers  WIND00297094: cache fix for vmPageLock/Unlock.
02u,30sep11,agf  Update BSP_REV for vxWorks 6.9.2
02t,17aug11,ers  Replaced DYNAMIC_WRLOAD with generic wrload support.
02s,21jul11,ers  WIND00262613: fix SERDES errata procedure to not disturb
                 lanes used by Aurora. And updated BSP_REV.
02r,25mar11,pch  cleanup
02q,29jan11,agf  fix CPUn_SPACE macro with parenthesis around input argument
02p,13jan11,agf  WIND00247891: document reserved memory for QMAN & BMAN and
                 remove QMAN_BMAN_RESERVED_MEM define;
                 WIND00246398: restore SYS_CLK_RATE_MIN to 1, root cause was
                 attributable to MIPC_SM and has been fixed;
                 misc. small readability improvements
02o,21dec10,pgh  Disable ROMSTART_BOOT_CLEAR
02n,08sep10,agf  deprecate sysTimerClkFreq in WDT_RATE_MIN/MAX;
                 set SYS_CLK_RATE_MIN to 20, reqd for SMP configuration;
                 clarify FMAN swap comment
01o,04sep10,r_z  Update BSP version for vxWorks 6.9
01n,12mar10,rec  CPU performance management update
02m,22jun10,pmr  make FMAN swap the default
02l,14jun10,wap  Add TGEC MDIO driver
02k,24may10,wap  Add some additional SERDES8 fixes from Freescale
02j,24may10,wap  Add macros for SERDES8 workaround
02i,19may10,wap  Add support for alternate FMAN setup on newer boards
02h,27mar10,pmr  remove old L1 cache parity error configuration
02g,26mar10,wap  Enable 3rd PCI Express bridge on Rev B boards
02f,08mar10,pmr  Rev B support; increase default physical mem size to 4 GB
02e,12jan10,pmr  SMP M-N
02d,11jan10,wap  Add options for secondary SGMII and PCIE configurations
02c,16nov09,pmr  improved 36-bit
02b,12nov09,rab  add TFFS support
02a,29oct09,wap  Add TGEC support
01z,22oct09,pmr  reenable PIXIS for UBOOT, L2, undef HW_FP for sf tools
01y,16oct09,pmr  disable PIXIS for UBOOT
01x,15oct09,wap  Add support for Fman microcode patches, use dummy MDIO driver
                 when using networking with Simics
01w,09oct09,pmr  disable L3 for AMP, EPIC ext. proxy for bootrom
01v,08oct09,rab  Add USB, DMA, SPI, and I2c support
01u,01oct09,wap  Add PCI support
01t,28sep09,pmr  remove USE_36_BIT
01s,24sep09,wap  Add GPIO driver
01r,23sep09,pmr  bump BSP revision
01q,22sep09,pmr  disable power management in bootrom
01p,15sep09,pmr  P4080DS support
01o,28aug09,wap  Add FMAN and DTSEC components
01n,12aug09,pmr  remove L2 SRAM
01m,08jul09,pmr  virtual mapping, 36 bit
01l,10jun09,pmr  extra uarts
01k,22jan09,wap  Add hardware buffer manager support
01j,22jan09,pmr  clock tweaks
01i,22dec08,wap  Remove old LAW driver, add new LAW driver and QMAN components
01h,08dec08,pmr  EPIC external proxy, minor cleanup
01g,10sep08,pmr  BSP name change
01f,03sep08,pmr  modifications for AMP
01e,29aug08,kab  Change _WRS_VX_SMP to _WRS_CONFIG_SMP
01d,20aug08,d_l  break default mac(ENET_DEFAULT) to three bytes.(WIND00121398)
01c,04jun08,dtr  Create FORCE_BOOT_LINE component.
01b,30may08,pmr  AMP support
01a,24apr08,pmr  adapted from ads8572 version 01j
*/

#ifndef	INCconfigh
#define	INCconfigh

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

#define BSP_VERSION     "6.9"
#define BSP_REV         "/6"

/*
 * The following addresses are used for multicore support. They represent
 * reserved memory and should be located where they will not get
 * overwritten by operating system or application software.
 *
 * Changing them will require reflashing the bootrom.
 */

#define CPUn_SPACE_BASE_ADRS 	0x00100000   /* RAM_LOW_ADRS */
#define CPUn_SPACE_SIZE		0x00010000   /* must be 2^exp */
#define CPUn_SPACE_SIZE_SHIFT	16	     /* used by romInit.s */
#define CPUn_SPACE(n)	        ((CPUn_SPACE_BASE_ADRS - ( CPUn_SPACE_SIZE * (n) )))
#define CPUn_FUNC_START_ADR(n)  (CPUn_SPACE(n) + 0)
#define CPUn_STACK_START_ADR(n) (CPUn_SPACE(n) + 4)
#define CPUn_INIT_START_ADR(n)  (CPUn_SPACE(n) + 8)
#define CPUn_LOCAL_MEM_ERPN(n)  (CPUn_SPACE(n) + 12)
#define CPUn_LOCAL_MEM_RPN(n)   (CPUn_SPACE(n) + 16)

/* NOTE: This BSP now supports the Rev. B boards by default now.
 * ROM images built with this default setting WILL NOT FUNCTION on the
 * older Rev. A boards.  Be sure to identify your board before building
 * and flashing a ROM image.
 *
 * To build a ROM image for Rev. A boards, uncomment the following line.
 */

/* #define P4080DS_REVA */

/*
 * Errata: A-003474 (Internal DDR calibration circuit is not supported)
 * This workaround is implemented in bootrom (romInit.s).
 * The errata has been fixed on Rev 3 silicon. Nothing will be done even if 
 * FSL_ERRATA_CCF_A004510 is defined.
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
 * The errata has been fixed on Rev 3 silicon. Nothing will be done even if 
 * FSL_ERRATA_CCF_A004510 is defined for Rev 3.0 or later.
 */

#define FSL_ERRATA_CCF_A004510

/*
 * Errata: A-004511 (Transactions may be incorrectly dropped from the wait
 * queue in the CoreNet coherency fabric), This workaround is implemented
 * in bootrom(romInit.s) and vxWorks image(sysLib.c).
 * The errata has been fixed on Rev 3 silicon. Nothing will be done even if 
 * FSL_ERRATA_CCF_A004511 is defined for Rev 3.0 or later.
*/

#define FSL_ERRATA_CCF_A004511

/*
 * NOTE: after the first two board revisions, Freescale changed the Fman
 * configuration in the reset control word. Originally, FMAN1 was set
 * for SGMII mode and FMAN2 was set for RGMII. With rev 2 silicon,
 * the SERDES protocol option for that configuration was removed, and
 * a new setup was chosen that has the opposite configuration: FMAN1 is
 * set for RGMII and FMAN2 is set for SGMII. The same number of DTSEC
 * ports are provided, but where physical ports DTSEC1, 2, 3, 4 and 5
 * were used before, the new configuration uses DTSEC2, 4, 5, 6 and 7.
 *
 * The two layouts require different setups in hwconf. To assist users,
 * both setups are defined in hwconf and the macro FMAN_SWAP will select
 * between them at compile time.
 *
 * Support for rev 2 silicon has been made the BSP's default configuration.
 * Users with rev 1 hardware must 'undef' the FMAN_SWAP macro.
 */

#define FMAN_SWAP

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
 *       The errata has been fixed on Rev 3 silicon.
 */

#undef FREESCALE_ERRATA_A004529_WORKAROUND_ENABLE

/*
 * For the SERDES8 errata workarounds, we need to specify which lanes
 * in Serdes banks 2 and 3 will be disabled via software. We want to leave
 * at least lane B in bank 3 off because with the expected protcol selection
 * of 0x10, if we leave it on, it will stop the RGMII DTSEC port on
 * FMAN 1 from working.
 */

#define P4080_ERRATUM_SERDES8_BANK2 0
#define P4080_ERRATUM_SERDES8_BANK3 0xF

/* ECC support is now standard on Rev A. and Rev B. boards */
#define INCLUDE_DDR_ECC

/* Define Clock Speed and source  */

#define	FREQ_10_MHZ	 10000000
#define	FREQ_33_MHZ	 33333300
#define	FREQ_40_MHZ	 40000000
#define	FREQ_50_MHZ	 50000000
#define	FREQ_66_MHZ	 66666600
#define	FREQ_75_MHZ	 75000000
#define FREQ_83_MHZ      83333300
#define	FREQ_90_MHZ	 90000000
#define	FREQ_100_MHZ	100000000
#define	FREQ_111_MHZ	111000000
#define	FREQ_125_MHZ	125000000
#define	FREQ_133_MHZ	133333300
#define	FREQ_166_MHZ	166666600
#define	FREQ_266_MHZ	266666000
#define	FREQ_400_MHZ	400000000
#define	FREQ_333_MHZ	333333000
#define	FREQ_533_MHZ	533333000
#define	FREQ_500_MHZ	500000000
#define	FREQ_600_MHZ	600000000

#define	OSCILLATOR_FREQ	FREQ_100_MHZ

#define PLAT_RATIO_800_MHZ 20
#define PLAT_RATIO_533_MHZ 16
#define PLAT_RATIO_400_MHZ 12
#define PLAT_RATIO_333_MHZ 8

/* Frequency applied to the  "RTC" pin */
#define	HPCN_RTC_FREQ	14318000

#define DEFAULT_SYSCLKFREQ (FREQ_500_MHZ / 2)

#undef FORCE_DEFAULT_FREQ /* Use to force freq used with DUART/Timers etc */

/*
 * By default, this BSP is now built for real hardware.  To revert to building
 * for Simics, uncomment the following line.
 *
#define SIMICS
 *
 */

#ifdef SIMICS
#define FORCE_DEFAULT_FREQ /* must be defined */
#undef OSCILLATOR_FREQ
#undef DEFAULT_SYSCLKFREQ
#define SIMICS_CLK_DIV	4
#define OSCILLATOR_FREQ (FREQ_166_MHZ / SIMICS_CLK_DIV)
#define DEFAULT_SYSCLKFREQ (FREQ_333_MHZ * 4 / SIMICS_CLK_DIV)
#endif /* SIMICS */

/*
 * Support for booting from UBOOT (or Linux).  By default, this is disabled.
 * To enable, uncomment the following line.
#define UBOOT
 */

#include <vsbConfig.h>
#include <configAll.h>

#ifndef _ASMLANGUAGE
#include <hwif/intCtlr/vxbEpicIntCtlr.h>
#endif

#include "p4080.h"

#undef CONSOLE_BAUD_RATE
#define CONSOLE_BAUD_RATE 115200

/* Extra uarts are available in simics.  See hwconf.c */
#ifdef SIMICS
#define USE_EXTRA_DUARTS
#endif /* SIMICS */

/* DMA support */
 
#undef  DRV_DMA_FSL

/* SPI support */

#undef  DRV_SPI_S25FLXX

#ifdef DRV_SPI_S25FLXX
#   define INCLUDE_FSL_SPI
#endif  /* DRV_SPI_S25FLXX */

/* I2C support */

#undef  INCLUDE_I2C

/* Common Includes for VXBUS PCI and DTSEC */
#define INCLUDE_VXBUS

#ifdef INCLUDE_VXBUS
#define INCLUDE_VXB_CMDLINE
#define INCLUDE_HWMEM_ALLOC
#define INCLUDE_PARAM_SYS

#define INCLUDE_PLB_BUS

#ifdef INCLUDE_PCI_BUS
#define INCLUDE_PCI_BUS_AUTOCONF
#define DRV_PCIBUS_QORIQ_PCIEX

/*
 * On some boards, I/O slot 3 is configured for SRIO mode, however
 * it can be changed to PCIe mode by updating the RCW, allowing
 * a total of 3 PCIe slots. To use this configuration, un-comment
 * the SECONDARY_PCIE_BRIDGE macro below.
 *

#define SECONDARY_PCIE_BRIDGE

 */

/*
 * The Rev B board has slot 3 set as a x4 PCIe link
 * Later rev boards use a different SERDES setup where slot 3
 * is used for SGMII.
 */

#if !defined(P4080DS_REVA) && !defined(FMAN_SWAP)
#define SECONDARY_PCIE_BRIDGE
#endif	/* !P4080DS_REVA && !FMAN_SWAP */

#endif	/* INCLUDE_PCI_BUS */

#define DRV_RESOURCE_QORIQLAW
#define DRV_RESOURCE_QORIQBMAN
#define DRV_RESOURCE_QORIQQMAN
#define DRV_RESOURCE_QORIQFMAN

#define	INCLUDE_INTCTLR_LIB
#define	DRV_INTCTLR_PPC
#define	DRV_INTCTLR_EPIC
#define	DRV_TIMER_OPENPIC
#define	INCLUDE_INTCTLR_DYNAMIC_LIB

#define DRV_TIMER_M85XX
#define INCLUDE_TIMER_SYS

#define DRV_SIO_NS16550
#define INCLUDE_SIO_UTILS

/*
 * The Simics P4080 simulation supports both ETSEC
 * and DTSEC ethernet. For the time being, we still
 * default to ETSEC, since using DTSEC requires updated
 * Simics initialization scripts. To change, disable
 * INCLUDE_ETSEC_VXB_END below and enable DRV_VXBEND_DTSEC
 * instead
 */

#ifdef SIMICS
#undef DRV_VXBEND_DTSEC
#define INCLUDE_ETSEC_VXB_END
#else /* SIMICS */
#define DRV_VXBEND_DTSEC
#endif /* SIMICS */

#define INCLUDE_DMA_SYS

#ifdef DRV_VXBEND_DTSEC

/*
 * A microcode patch is required for proper support of
 * soft parser rules, independent mode and coarse
 * classification.
 */

#define INCLUDE_P4080_FMAN_UCODE

/*
 * Simics doesn't emulate the MDIO or PHY logic for
 * the DTSEC ports, so we must use the dummy MDIO driver
 * to convince the DTSEC driver there's actually
 * a link present. For real hardware, we use the
 * DTSEC MDIO driver and GPIO driver.
 */

#ifdef SIMICS
#define INCLUDE_DUMMY_MDIO
#else
#define DRV_MII_DTSEC_MDIO
#define DRV_RESOURCE_FSLGPIO
#endif

/*
 * Out of the box, the P4080DS board supports 4 DTSEC ports,
 * four on an SGMII riser card, and one via an on-board RGMII PHY.
 * It's possible however to add a second SGMII rider, allowing all
 * 8 DTSECs to be active at once. (In this configuration, the RGMII
 * PHY is disabled.) To use this configuration, un-comment the
 * SECONDARY_SGMII_RISER macro below.

#define SECONDARY_SGMII_RISER

 */

#endif

#define INCLUDE_MII_BUS
#define INCLUDE_GENERICPHY
#define INCLUDE_END

/*
 * The P4080 also has two 10 gigabit ethernet controllers.
 * To use them, one needs one (or more) XAUI riser cards,
 * which are not included by default with the P4080DS board.
 * Consequently, the TGEC driver is not on by default either.
 *
 * When it is enabled, the TGEC driver works in conjunction
 * with the DTEPHY driver for link management with miiBus.
 * The DRV_MII_TGEC_MDIO driver is also required for fiber
 * optic media support.
 *
#define DRV_VXBEND_TGEC
 */

#ifdef DRV_VXBEND_TGEC
#define DRV_MII_DTEPHY
#define DRV_MII_TGEC_MDIO
#endif

/*
 * To select fiber media mode on one of the TGEC ports, uncomment
 * one of the macros below. This requires the TGEC_MDIO driver.
 *
#define TGEC0_FIBER
#define TGEC1_FIBER
 */

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

#ifdef _WRS_CONFIG_SMP
#define NET_DAEMONS_CPU_AFFINITY TRUE
#endif /* WRS_VX_SMP */

/*
 * The EPIC External Proxy Facility is required when VxWorks is loaded
 * from WR Linux, as the latter sets this mode.  Defining this macro
 * will result in the EPIC driver using a BSP-defined method, sysEprGet(),
 * to obtain the interrupt vector from the External Proxy Register (EPR).
 * If this macro is undefined, the EPIC driver will set up Mixed Mode like
 * other Freescale BSPs (if run from CPU0) or assume Mixed Mode is configured
 * (if run from CPU1-7).  In Mixed Mode, the interrupt vector is obtained
 * by reading the EPIC register IACK.  The two modes are not compatible.
 */

/* Note: this should be undefined when building VxWorks bootroms for hardware.
 * This is a temporary measure to address an issue with external proxy mode
 * on hardware.  Simics works well with external proxy mode enabled.o
 *
 * When building for use in an Linux-VxWorks AMP system, this must be defined.
 */
#if defined (SIMICS) || defined (UBOOT)
#define EPIC_EXTERNAL_PROXY
#endif

#endif /* INCLUDE_VXBUS */

/*
 * When using AMP, the TSEC_MDIO driver is required if we want to
 * use the on-board ETSEC ethernet ports on both cores. This is because
 * there is only one MDIO port, and it has to be shared between the cores.
 * A TSEC_MDIO instance on one core will cooperate with its twin on the
 * other code to insure that MDIO accesses will not overlap.
 */

/* other useful includes  
#define INCLUDE_VXBUS_SHOW
#define INCLUDE_IFCONFIG
#define INCLUDE_PING
#define INCLUDE_SHOW_ROUTINES
#define INCLUDE_PCI_BUS_SHOW
#define INCLUDE_IPATTACH
#define INCLUDE_SHELL
*/

#define MAX_MAC_DEVS 	4 		/* two network devices (dtsec, tgec) */

/* default mac address */

#define ENET_DEFAULT0 WR_ENET0
#define ENET_DEFAULT1 WR_ENET1
#define ENET_DEFAULT2 WR_ENET2

#ifdef DRV_VXBEND_DTSEC

#define DEFAULT_BOOT_LINE \
"dtsec(0,0)host:vxWorks h=192.168.0.1 e=192.168.0.2 u=vxworks pw=vxworks f=0x0"

/* the following is used for simics */
#undef DEFAULT_BOOT_LINE
#define DEFAULT_BOOT_LINE \
"dtsec(0,0)host:vxWorks h=10.10.0.10 e=10.10.0.80 u=vxworks pw=vxworks f=0x0"

#else

#define DEFAULT_BOOT_LINE \
"motetsec(0,0)host:vxWorks h=192.168.0.1 e=192.168.0.2 u=vxworks pw=vxworks f=0x0"

/* the following is used for simics */
#undef DEFAULT_BOOT_LINE
#define DEFAULT_BOOT_LINE \
"motetsec(0,0)host:vxWorks h=10.10.0.10 e=10.10.0.80 u=vxworks pw=vxworks f=0x0"

#endif

#define INCLUDE_END
/* MMU and CACHE */

#define INCLUDE_MMU_BASIC
#define USER_I_MMU_ENABLE
#define USER_D_MMU_ENABLE

#define CACHE_LIBRARY_MODE     (CACHE_COPYBACK | CACHE_SNOOP_ENABLE)
#define CAM_DRAM_CACHE_MODE    _MMU_TLB_ATTR_M
#define TLB_CACHE_MODE         VM_STATE_CACHEABLE | VM_STATE_MEM_COHERENCY

#undef  USER_D_CACHE_MODE
#define USER_D_CACHE_MODE  (CACHE_LIBRARY_MODE)
#undef  USER_I_CACHE_MODE
#define USER_I_CACHE_MODE (CACHE_LIBRARY_MODE)

#define INCLUDE_L2_CACHE

#define INCLUDE_L3_CACHE        /* disabled for P4080DS_REVA */
#define L3_WRITE_THRU_MODE  0   /* 1 => write thru, 0 => write back */

#ifdef P4080DS_REVA
#define L3_DISABLED
#endif

#define INCLUDE_BRANCH_PREDICTION

#ifdef INCLUDE_NFS
/* Default NFS parameters - constants may be changed here, variables
 * may be changed in usrConfig.c at the point where NFS is included.
 */
#define NFS_USER_ID             2001            /* dummy nfs user id */
#define NFS_GROUP_ID            100             /* dummy nfs user group id */
#define NFS_MAXPATH             255             /* max. file path length */
#endif /* INCLUDE_NFS */

/* Serial channel and TTY */
#  undef  NUM_TTY
#  define NUM_TTY 1

/* Clock rates */

#define	SYS_CLK_RATE_MIN	1 	/* minimum system clock rate */
#define	SYS_CLK_RATE_MAX	8000	/* maximum system clock rate */
#define	AUX_CLK_RATE_MIN	1	/* minimum auxiliary clock rate */
#define	AUX_CLK_RATE_MAX	8000	/* maximum auxiliary clock rate */
#define WDT_RATE_MIN		((sysClkFreqGet() >> 3) / (1 << 29))
#define WDT_RATE_MAX		((sysClkFreqGet() >> 3) / (1 << 21))

/* eSDHC */

#ifdef DRV_STORAGE_SDHC
#   define INCLUDE_DOSFS
#endif  /* DRV_STORAGE_SDHC */

/* TrueFFS flash support */

#ifdef  INCLUDE_TFFS

/* board specific flash configuration needed by MTD driver */

#define S29GL_FLASH_BIT      1           /* 16-bit */
#define S29GL_FLASH_NUM      1           /* only one chip */
#define S29GL_FLASH_WIDTH    2           /* 16-bit */
#define S29GL_SECTOR_SIZE    0x00020000  /* 128K per sector */
#define S29GL_FLASH_SIZE     0x00800000  /* only 8MB used for TrueFFS */
#define S29GL_FLASH_OP_DELAY

#define INCLUDE_TL_FTL
#define INCLUDE_TFFS_MOUNT
#define INCLUDE_DOSFS

#endif  /* INCLUDE_TFFS */

/* USB EHCI */

/* USB not defined by default
#define INCLUDE_USB
*/

#ifdef INCLUDE_USB
#define INCLUDE_USB_INIT
#define INCLUDE_EHCI
#define INCLUDE_EHCI_INIT
#define INCLUDE_USB_MS_BULKONLY
#define INCLUDE_USB_MS_BULKONLY_INIT
#define INCLUDE_DOSFS
#endif  /* INCLUDE_USB */

/*  Define for FS */

#ifdef INCLUDE_DOSFS

#define INCLUDE_ERF
#define INCLUDE_DEVICE_MANAGER
#define INCLUDE_FS_EVENT_UTIL
#define INCLUDE_FS_MONITOR
#define INCLUDE_XBD
#define INCLUDE_XBD_BLK_DEV
#define INCLUDE_XBD_TRANS
#define INCLUDE_DOSFS_DIR_FIXED
#define INCLUDE_DOSFS_DIR_VFAT
#define INCLUDE_DOSFS_FAT
#define INCLUDE_DOSFS_FMT
#define INCLUDE_DOSFS_CHKDSK
#define INCLUDE_DOSFS_MAIN
#define INCLUDE_DISK_UTIL
#define INCLUDE_DOSFS_CACHE
#   ifdef  BOOTAPP
#      define INCLUDE_BOOT_FILESYSTEMS
#   endif  /* BOOTAPP */

#endif  /* INCLUDE_DOSFS */

/* 	  	 
 * Although the support for point to point has been added it is untested 	  	 
 * with Serial RIO. Similar was tested on parallel RIO 	  	 
 */ 	  	 
  	  	 
#ifdef INCLUDE_RAPIDIO_BUS 	  	 

/* RapidIO information */ 	  	 

#define RAPIDIO_REG_BASE 0xc0000 	  	 
#define RAPIDIO_BASE   (CCSBAR + RAPIDIO_REG_BASE) 	  	 
#define RAPIDIO_ADRS   0xc0000000 	  	 
#define RAPIDIO_SIZE   0x10000000 	  	 
#define RAPIDIO_BUS_ADRS 0x80000000 	  	 
#define RAPIDIO_BUS_SIZE 0x10000000 	  	 
#endif  /* INCLUDE_RAPIDIO_BUS */

#ifdef  INCLUDE_PCI_BUS

/*  p4080ds supports dual PCIe controllers */

/*

CPU Addr                                  PCI Addr ( PCI1 or PCI2)
PCI_LOCAL_MEM_BUS        ------------------------- PCI_MSTR_MEM_BUS
                         -                       -
                         -                       -
PCI_LOCAL_MEM_BUS +      ------------------------- PCI_MSTR_MEM_BUS +
PCI_LOCAL_MEM_SIZE       -                       - PCI_MSTR_MEM_SIZE
                         -                       -
                         -                       ---- PCI Bridge (for PCI1 only)
                         -                       -     configuration regs
                         -                       -
CPU_PCI_MEM_ADRS (PCIEX1) ------------------------ PCI_MEM_ADRS
CPU_PCI_MEM_ADRS2(PCIEX2) -                      - PCI_MEM_ADRS2
CPU_PCI_MEM_ADRS3(PCIEX3) ------------------------ PCI_MEM_ADRS3
                         -                       -
CPU_PCI_MEMIO_ADRS       ------------------------- PCI_MEMIO_ADRS
CPU_PCI_MEMIO_ADRS2      -                       - PCI_MEMIO_ADRS2
CPU_PCI_MEMIO_ADRS3      -                       - PCI_MEMIO_ADRS3
                         -                       -
CPU_PCI_IO_ADRS (PCIEX1) ------------------------- PCI_IO_ADRS
CPU_PCI_IO_ADRS2(PCIEX2) -                       - PCI_IO_ADRS2
CPU_PCI_IO_ADRS3(PCIEX3) -                       - PCI_IO_ADRS3
                         -                       -
CPU_PCI_IO_ADRS (PCIEX1)+------------------------- PCI_IO_ADRS +
CPU_PCI_IO_ADRS2(PCIEX2)+                          PCI_IO_ADRS2 +
CPU_PCI_IO_ADRS2(PCIEX3)                           PCI_IO_ADRS3
CPU_PCI_IO_SIZE          -                       - PCI_IO_SIZE
                         -                       -
*/

/*
 * PCI based addresses
 * Note: the LAW_BASE values have been chosen to coincide
 * with those used by U-Boot on the P4080DS board.
 */

#define PCIEX1_LAW_BASE             0x80000000       /* PCI LAW window */
#define PCIEX1_LAW_SIZE             0x10000000       /* PCI LAW size   */

#define PCIEX1_MEM_SIZE             0x04000000        /* 64 MB */
#define PCIEX1_MEMIO_SIZE           0x04000000        /* 64 MB */
#define PCIEX1_IO_SIZE              0x04000000        /* 64 MB */
#define PCIEX1_IO32_SIZE            0x04000000        /* 64 MB */

#define PCIEX1_MEM_ADRS             (PCIEX1_LAW_BASE)
#define PCIEX1_MEMIO_ADRS           (PCIEX1_MEM_ADRS   + PCIEX1_MEM_SIZE)
#define PCIEX1_IO_ADRS              (PCIEX1_MEMIO_ADRS + PCIEX1_MEMIO_SIZE)
#define PCIEX1_IO32_ADRS            (PCIEX1_IO_ADRS    + PCIEX1_IO_SIZE)

#define PCIEX2_LAW_BASE             0xA0000000       /* PCI LAW window */
#define PCIEX2_LAW_SIZE             0x10000000       /* PCI LAW size   */

#define PCIEX2_MEM_SIZE             0x04000000        /* 64 MB */
#define PCIEX2_MEMIO_SIZE           0x04000000        /* 64 MB */
#define PCIEX2_IO_SIZE              0x04000000        /* 64 MB */
#define PCIEX2_IO32_SIZE            0x04000000        /* 64 MB */

#define PCIEX2_MEM_ADRS            (PCIEX2_LAW_BASE)
#define PCIEX2_MEMIO_ADRS          (PCIEX2_MEM_ADRS   + PCIEX2_MEM_SIZE)
#define PCIEX2_IO_ADRS             (PCIEX2_MEMIO_ADRS + PCIEX2_MEMIO_SIZE)
#define PCIEX2_IO32_ADRS           (PCIEX2_IO_ADRS    + PCIEX2_IO_SIZE)

#define PCIEX3_LAW_BASE             0xC0000000       /* PCI LAW window */
#define PCIEX3_LAW_SIZE             0x10000000       /* PCI LAW size   */

#define PCIEX3_MEM_SIZE             0x04000000        /* 64 MB */
#define PCIEX3_MEMIO_SIZE           0x04000000        /* 64 MB */
#define PCIEX3_IO_SIZE              0x04000000        /* 64 MB */
#define PCIEX3_IO32_SIZE            0x04000000        /* 64 MB */

#define PCIEX3_MEM_ADRS             (PCIEX3_LAW_BASE)
#define PCIEX3_MEMIO_ADRS           (PCIEX3_MEM_ADRS   + PCIEX3_MEM_SIZE)
#define PCIEX3_IO_ADRS              (PCIEX3_MEMIO_ADRS + PCIEX3_MEMIO_SIZE)
#define PCIEX3_IO32_ADRS            (PCIEX3_IO_ADRS    + PCIEX3_IO_SIZE)

#define PCI_MMU_TLB_SZ		_MMU_TLB_SZ_256M

#endif /* INCLUDE_PCI_BUS */

#define FLASH_WINDOW_SIZE               0x00800000

/* Flash Driver pulled from cds85xx BSP */
#define CDS85XX_FLASH_RESERVED_SIZE     0x00110000

/* LBC CS0 - flash 0 - default for bootrom */

#define FLASH_BASE_ADRS                 0xff000000
#define FLASH_ADRS_MASK                 0xff000000
#define MAIN_FLASH_SIZE                 0x01000000

/* LBC CS3 - nvram, cadmus, ATM phy */
#if !defined (SIMICS)
#define INCLUDE_LBC_CS3
#endif /* SIMICS */

#ifdef INCLUDE_LBC_CS3
#define LBC_CS3_LOCAL_ADRS       PIXIS_BASE
#define LBC_CS3_LOCAL_SIZE_MASK  0xfff00000
#define LBC_CS3_SIZE             PIXIS_SIZE
#endif

#if !defined (SIMICS) && !defined (UBOOT)
#define INCLUDE_FLASH
#endif /* SIMICS / UBOOT */

#ifdef INCLUDE_FLASH

/*
 * 29LV640D flash is not supported in flashMem.c directly but is similar
 * in procedures with 29LV160B
 */
#define S29GLXX
#define FLASH_29LV640D	FLASH_29LV160B

#undef  SYS_FLASH_TYPE
#define SYS_FLASH_TYPE		FLASH_29LV640D
#undef  FLASH_WIDTH
#define FLASH_WIDTH		2
#undef  FLASH_CHIP_WIDTH
#define FLASH_CHIP_WIDTH	2

#define NV_RAM_ADRS             0
#define FLASH_SEGMENT_SIZE    0x10000
#define FLASH_ADRS            0xffef0000

#define FLASH_SIZE            0x10000
#define FLASH_SIZE_WRITEABLE	FLASH_SIZE	/* write all of flash */
#define NV_RAM_SIZE           FLASH_SIZE
#undef FLASH_NO_OVERLAY
#undef  NV_BOOT_OFFSET
#define NV_BOOT_OFFSET        0

#else /* INCLUDE_FLASH */

#define NV_RAM_SIZE 0

#endif

#ifdef INCLUDE_SM_COMMON
#define INCLUDE_SMEND
#define INCLUDE_SM_NET
#define INCLUDE_NET_DRV
#define INCLUDE_SM_SEQ_ADDR

#undef IPFORWARDING_CFG
#define IPFORWARDING_CFG 1
#endif /* defined(INCLUDE_SM_NET) */

#ifndef PRJ_BUILD
    #ifdef INCLUDE_TIPC
        #ifndef _WRS_CONFIG_SMP
            #define INCLUDE_TIPC_MEDIA_SM           /* shared memory media */
        #endif
        #define INCLUDE_TIPC_CONFIG_STR         /* use static config string */
        #define INCLUDE_TIPC_CONFIG_HOOK_BOOT   /* call boot params config routine */
        #define INCLUDE_TIPC_MEMPOOL            /* include TIPC memory buffers */
        #define INCLUDE_TIPC_SOCKET             /* include TIPC socket API */
    #endif
#endif
/*
 * Changing SM_OFF_BOARD to TRUE also requires changing
 * SM_ANCHOR_ADRS and SM_MEM_ADRS appropriately.
 */

#define SM_OFF_BOARD    TRUE

#undef SM_ANCHOR_OFFSET
#define SM_ANCHOR_OFFSET 0x4100
#undef SM_ANCHOR_ADRS
#define SM_ANCHOR_ADRS   SM_ANCHOR_OFFSET
#undef SM_MEM_ADRS
#define SM_MEM_ADRS   0x00010000

#define SM_MEM_SIZE		0x00070000
#define SM_OBJ_MEM_ADRS		(SM_MEM_ADRS+SM_MEM_SIZE) /* SM Objects pool */
#define SM_OBJ_MEM_SIZE		0x00040000

#define SM_INT_ARG1   (EPIC_VEC_IPI_IRQ0) /*EPIC_VEC_IPI_IRQ0*/
#define SM_INT_ARG2   (EPIC_VEC_IPI_IRQ0) /*EPIC_VEC_IPI_IRQ0*/
#define SM_INT_ARG3   1

#define SM_TIPC_ADRS_DEFAULT    ((int) SM_OBJ_MEM_ADRS + SM_OBJ_MEM_SIZE)
#define SM_TIPC_ADRS            SM_TIPC_ADRS_DEFAULT
#define SM_TIPC_SIZE_DEFAULT    0x00020000
#define SM_TIPC_SIZE            SM_TIPC_SIZE_DEFAULT /* SM TIPC pool size  */

#undef SM_TAS_TYPE

/* Default is not to include shared memory in bootrom for dual core
 * SM_TAS_HARD will break if you do so as it requires data cache enabled.
 */
#define SM_TAS_TYPE SM_TAS_HARD
#define SM_INT_TYPE SM_INT_BUS  /* or SM_INT_NONE */

/* Memory addresses */

#define INCLUDE_DDR_SDRAM

#define PHYS_MEM_START          0x00000000
/* only 1 GB and 2 GB have been tested */
#define PHYS_MEM_SIZE_TOTAL     0xffffffff 	/* 4 GB is default */

/*
 * Address of QMAN and BMAN private memory.
 * The QMAN driver will allocate 2MB starting from QMAN_MEM_BASE.
 * The BMAN driver will allocate 1MB starting from BMAN_MEM_BASE.
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
 */

#define QMAN_MEM_BASE		0x3FD00000
#define BMAN_MEM_BASE		(QMAN_MEM_BASE + 0x00200000)

#ifdef NO_BOOTROM_LEGACY
#undef  QMAN_MEM_BASE
#undef  BMAN_MEM_BASE
#define QMAN_MEM_BASE	(PHYS_MEM_START + PHYS_MEM_SIZE - 0x00300000)
#define BMAN_MEM_BASE	(QMAN_MEM_BASE + 0x00200000)
#endif

/*
 * LOCAL_MEM_SIZE can not be defined in config.h since it will override
 * any CDF defines for the same variable.  The DEFAULT_LOCAL_MEM_SIZE will
 * be defined here and used if vxprj does not set the LOCAL_MEM_SIZE
 * define.
 *
 * DEFAULT_LOCAL_MEM_SIZE will stop right below the QMAN & BMAN's
 * reserved memory, though this method depends on LOCAL_MEM_LOCAL_ADRS = 0x0.
 * This is a reasonable requirement as kernels that reference
 * DEFAULT_LOCAL_MEM_SIZE are necessarily UP images.
 */

#define DEFAULT_LOCAL_MEM_SIZE	   QMAN_MEM_BASE

#define DDR_SDRAM_LOCAL_ADRS       PHYS_MEM_START

#ifdef SIMICS
#define DDR_SDRAM_LAWAR_SIZE  	   LAWAR_SIZE_1GB
#define DDR_SDRAM_LOCAL_END	   (PHYS_MEM_START + 0x1fffffff)
#define DDR_SDRAM_LOCAL_ADRSB      (PHYS_MEM_START + 0x20000000)
#define DDR_SDRAM_LOCAL_ENDB	   (PHYS_MEM_START + 0x3fffffff)
#define DDR_SDRAM_LOCAL_ADRS2      (PHYS_MEM_START + 0x40000000)
#define DDR_SDRAM_LOCAL_END2	   (PHYS_MEM_START + 0x5fffffff)
#define DDR_SDRAM_LOCAL_ADRS2B     (PHYS_MEM_START + 0x60000000)
#define DDR_SDRAM_LOCAL_END2B	   (PHYS_MEM_START + 0x7fffffff)
#define PHYS_MEM_SIZE		   0x40000000
#else /* SIMICS */
# if (PHYS_MEM_SIZE_TOTAL == 0x40000000)
#  define DDR_SDRAM_LAWAR_SIZE     LAWAR_SIZE_512MB
#  define DDR_SDRAM_LOCAL_END	   (PHYS_MEM_START + 0x1fffffff)
#  define DDR_SDRAM_LOCAL_ADRS2    (PHYS_MEM_START + 0x20000000)
#  define DDR_SDRAM_LOCAL_END2	   (PHYS_MEM_START + 0x3fffffff)
#  define PHYS_MEM_SIZE		   0x20000000
# elif (PHYS_MEM_SIZE_TOTAL == 0x80000000)
#  define DDR_SDRAM_LAWAR_SIZE     LAWAR_SIZE_1GB
#  define DDR_SDRAM_LOCAL_END	   (PHYS_MEM_START + 0x3fffffff)
#  define DDR_SDRAM_LOCAL_ADRS2    (PHYS_MEM_START + 0x40000000)
#  define DDR_SDRAM_LOCAL_END2	   (PHYS_MEM_START + 0x7fffffff)
#  define PHYS_MEM_SIZE		   0x40000000
# else /* assume full 4 GB */
#  define DDR_SDRAM_LAWAR_SIZE     LAWAR_SIZE_2GB
#  define DDR_SDRAM_LOCAL_END	   (PHYS_MEM_START + 0x7fffffff)
#  define DDR_SDRAM_LOCAL_ADRS2    (PHYS_MEM_START + 0x80000000)
#  define DDR_SDRAM_LOCAL_END2	   (PHYS_MEM_START + 0xffffffff)
#  define PHYS_MEM_SIZE		   0x40000000
# endif
#endif /* SIMICS */

#ifdef IPBASIC
/* remove power management for bootrom */
#undef INCLUDE_CPU_PWR_MGMT
#endif /* IPBASIC */

/* XXX this may be removed eventually */
#define LOCAL_MEM_LOCAL_OFFSET 0

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
#define LOCAL_MEM_ERPN		0
#endif

#define DDR_SDRAM_LOCAL_ADRS_H	   0
#define DDR_SDRAM_LOCAL_ADRS2_H	   2

/* if upper memory half is in 36-bit address range, redefine the lower
 * 32-bits to match the lower half.  For example,
 * 1 GB at 0x000000000 - 0x03fffffff
 * 1 GB at 0x200000000 - 0x23fffffff
 *    (rather than 1 GB at 0x240000000 - 0x27fffffff)
 *
 * This example is the default configuration.
 */
#if DDR_SDRAM_LOCAL_ADRS2_H
#undef DDR_SDRAM_LOCAL_ADRS2
#define DDR_SDRAM_LOCAL_ADRS2   DDR_SDRAM_LOCAL_ADRS
#undef DDR_SDRAM_LOCAL_END2
#define DDR_SDRAM_LOCAL_END2    DDR_SDRAM_LOCAL_END
# ifdef SIMICS
# undef DDR_SDRAM_LOCAL_ADRS2B
# define DDR_SDRAM_LOCAL_ADRS2B   DDR_SDRAM_LOCAL_ADRSB
# undef DDR_SDRAM_LOCAL_END2B
# define DDR_SDRAM_LOCAL_END2B    DDR_SDRAM_LOCAL_ENDB
# endif /* SIMICS */
#endif /* DDR_SDRAM_LOCAL_ADRS2_H */

#undef  VEC_BASE_ADRS
#define VEC_BASE_ADRS	 	   LOCAL_MEM_LOCAL_ADRS

#ifdef INCLUDE_MMU_BASIC

#define INCLUDE_AIM_MMU_CONFIG
#define INCLUDE_AIM_MMU_MEM_POOL_CONFIG

/*
 * The following parameters are to configure initial memory usage for
 * page table and region tables and in event of running out the increment
 * of memory allocation and is specified as a number of MMU pages (4KByte
 * blocks).
 */
#define AIM_MMU_INIT_PT_NUM 0x40
#define AIM_MMU_INIT_PT_INCR 0x20
#define AIM_MMU_INIT_RT_NUM 0x10
#define AIM_MMU_INIT_RT_INCR 0x10

#endif /* INCLUDE_MMU_BASIC */

#define ROM_TEXT_ADRS 0xFFF00100
#define ROM_BASE_ADRS 0xFFF00000
#define ROM_SIZE      0x00100000
/*
* Do not clear memory in romStart() since it is not necessary,
* and slows boot time.
*/
#undef ROMSTART_BOOT_CLEAR

/* user reserved memory, see sysMemTop() */

#define USER_RESERVED_MEM 0x00000000

#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif  /* INCconfigh */

#if defined(PRJ_BUILD)
    #include "prjParams.h"
#endif /* PRJ_BUILD */
