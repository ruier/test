/* config.h - Wind River Virtual Board P4080 configuration file */

/*
 * Copyright (c) 2009-2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01j,02feb12,x_s  Remove the component INCLUDE_AIM_MMU_SHOW. (WIND00334904)
01i,25may11,agf  update CPUn_SPACE macros to new design
01h,15mar11,kab  Add tgec ethernet support
01g,15oct10,kab  Update version for release
01f,23jul10,wap  Make FMAN_SWAP the default (WIND00223129)
01e,12jun10,rab  Add support for SMP guest os
01d,25may10,wap  Add support for alternate FMAN setup on newer boards
01c,23dec09,wap  Add networking support
01b,18dec09,to   increase NUM_TTY from 1 to 2.
01a,13nov09,to   created
*/

#ifndef	INCconfigh
#define	INCconfigh

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/* BSP version/revision identification, before configAll.h */

#define BSP_VERSION	"6.9"
#define BSP_REV		"/1"

/* Base model string. sysModel adds processor details */

#ifndef _WRS_CONFIG_SMP
#define SYS_MODEL	"Wind River Virtual Board P4080"
#else
#define SYS_MODEL	"Wind River Virtual Board P4080 SMP"
#endif

#include <vsbConfig.h>
#include <configAll.h>
#include "wrVbP4080.h"


/*
 * The following addresses are used for multicore support. They represent
 * reserved memory and should be located where they will not get
 * overwritten by operating system or application software.
 *
 * Changing them will require reflashing the bootrom.
 */

#define CPUn_SPACE_BASE_ADRS    0x00010000   /* must be CPU0's RAM_LOW_ADRS */
#define CPUn_SPACE_SIZE         0x00001000   /* must be power of 2^n */
#define CPUn_SPACE_SIZE_SHIFT   12           /* used by romInit.s */
#define CPUn_SPACE(n)           ((CPUn_SPACE_BASE_ADRS - ( CPUn_SPACE_SIZE * (n) )))
#define CPUn_FUNC_START_ADR(n)  (CPUn_SPACE(n) + 0)
#define CPUn_STACK_START_ADR(n) (CPUn_SPACE(n) + 4)
#define CPUn_INIT_START_ADR(n)  (CPUn_SPACE(n) + 8)
#define CPUn_LOCAL_MEM_ERPN(n)  (CPUn_SPACE(n) + 12)


/*
 * NOTE: after the first two board revsions, Freescale changed the Fman
 * configuration in the reset control word. Originally, FMAN1 was set
 * for SGMII mode and FMAN2 was set for RGMII. With rev 2 silicon,
 * the SERDES protocol option for that configuration was removed, and
 * a new setup was chosen that has the opposite configuration: FMAN1 is
 * set for RGMII and FMAN2 is set for SGMII. The same number of DTSEC
 * ports are provided, but where physical ports DTSEC1, 2, 3, 4 and 5
 * we used before, the new configuration uses DTSEC2, 4, 5, 6 and 7.
 * A different hwconf setup is needed to accomodate this new layout,
 * which can be enabled by #defining FMAN_SWAP below. All boards with
 * rev 2 silicon will use this layout, which means this will become the
 * default, and the original setup will be deprecated.
 */

#define FMAN_SWAP


/* memory configuration */

/*
 * The constants ROM_TEXT_ADRS, ROM_SIZE, and RAM_HIGH_ADRS
 * are defined in config.h and Makefile.
 * All definitions for these constants must be identical.
 */

#define ROM_BASE_ADRS		0xFFF00000
#define ROM_TEXT_ADRS		0xFFF00100
#define ROM_SIZE		0x00100000

#define LOCAL_MEM_LOCAL_ADRS	0x00000000
#define LOCAL_MEM_SIZE		0x01000000	/* 16MB */
#define LOCAL_MEM_AUTOSIZE      /* let hypervisor tell us how much memory */
#define USER_RESERVED_MEM	0x00000000


/* Define Clock Speed and source  */

#define FREQ_10_MHZ      10000000
#define FREQ_33_MHZ      33333300
#define FREQ_40_MHZ      40000000
#define FREQ_50_MHZ      50000000
#define FREQ_66_MHZ      66666600
#define FREQ_75_MHZ      75000000
#define FREQ_83_MHZ      83333300
#define FREQ_90_MHZ      90000000
#define FREQ_100_MHZ    100000000
#define FREQ_111_MHZ    111000000
#define FREQ_125_MHZ    125000000
#define FREQ_133_MHZ    133333300
#define FREQ_166_MHZ    166666600
#define FREQ_266_MHZ    266666000
#define FREQ_400_MHZ    400000000
#define FREQ_333_MHZ    333333000
#define FREQ_533_MHZ    533333000
#define FREQ_500_MHZ    500000000
#define FREQ_600_MHZ    600000000

#define OSCILLATOR_FREQ FREQ_100_MHZ

/* Frequency applied to the  "RTC" pin */
#define HPCN_RTC_FREQ   14318000

/* possibly this is not supported check your DDR undefined for safety */
#undef INCLUDE_DDR_ECC
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
#define SIMICS_CLK_DIV  4
#define OSCILLATOR_FREQ (FREQ_166_MHZ / SIMICS_CLK_DIV)
#define DEFAULT_SYSCLKFREQ (FREQ_333_MHZ * 4 / SIMICS_CLK_DIV)
#endif /* SIMICS */


/* VxBus configuration */

#define INCLUDE_VXBUS
#define INCLUDE_VXBUS_INIT
#define INCLUDE_VXBUS_ACTIVATE
#define INCLUDE_PARAM_SYS
#define INCLUDE_PLB_BUS
#define INCLUDE_HWMEM_ALLOC
#define HWMEM_POOL_SIZE 100000

#define INCLUDE_INTCTLR_LIB
#define DRV_INTCTLR_VB
#define DRV_INTCTLR_PPC
#define DRV_TIMER_VB
#define SYSCLK_TIMER_NAME "vxbVbTimerDev"
#define DRV_SIO_NS16550

/* DPAA and networking configuration */

#define DRV_RESOURCE_QORIQLAW
#define DRV_RESOURCE_QORIQQMAN
#define DRV_RESOURCE_QORIQBMAN
#define DRV_RESOURCE_QORIQFMAN
#define DRV_VXBEND_DTSEC
#define INCLUDE_MII_BUS
#define INCLUDE_GENERICPHY

/* Default control plane specification */

#ifndef CONTROL_PLANE_CORE
#define CONTROL_PLANE_CORE -1
#endif

/* MAC address configuration */

#define MAX_MAC_DEVS    4               /* two network devices (dtsec, tgec) */

/* default mac address */

#define ENET_DEFAULT0 WR_ENET0
#define ENET_DEFAULT1 WR_ENET1
#define ENET_DEFAULT2 WR_ENET2

/* flash configuration */

#define INCLUDE_FLASH
#ifdef  INCLUDE_FLASH
#define FLASH_ADRS		0xD0007000
#define FLASH_SIZE		0x10000
#define NV_RAM_SIZE		FLASH_SIZE
#define NV_BOOT_OFFSET		0
#define NV_MAC_ADRS_OFFSET	0x200
#else   /* INCLUDE_FLASH */
#define NV_RAM_SIZE NONE
#endif /* INCLUDE_FLASH */


/* console configuration */

#define TTY_UART1 0
#define TTY_UART2 1

#undef  CONSOLE_TTY
#define CONSOLE_TTY sysConsoleTty

#undef  CONSOLE_BAUD_RATE
#define CONSOLE_BAUD_RATE sysConsoleBaud

#undef  NUM_TTY
#define NUM_TTY 2


/* AUX clock configuration */

#undef INCLUDE_AUX_CLK
#define AUX_CLK_RATE_MIN    1
#define AUX_CLK_RATE_MAX    50000


/* cache configuration */

#define INCLUDE_CACHE_SUPPORT
#define USER_D_CACHE_ENABLE
#undef  USER_D_CACHE_MODE
#define USER_D_CACHE_MODE (CACHE_COPYBACK)
#define USER_I_CACHE_ENABLE
#undef  USER_I_CACHE_MODE
#define USER_I_CACHE_MODE (CACHE_COPYBACK)


/* MMU configuration */

#define INCLUDE_MMU_BASIC
#ifdef  INCLUDE_MMU_BASIC

#define USER_I_MMU_ENABLE
#define USER_D_MMU_ENABLE

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

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* INCconfigh */

#if defined(PRJ_BUILD)
    #include "prjParams.h"
#endif /* PRJ_BUILD */
