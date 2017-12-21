/* config.h - Freescale HPC-II BSP configuration header file */

/*
 * Copyright (c) 2005-2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
 
/* Copyright (c) 2005 Tundra Semiconductor Corp. */

/*
modification history
--------------------
01k,12mar10,rec  CPU performance management update
01j,15apr10,clx  Update BSP version for vxWorks 6.9
01i,16oct09,x_f  Remove RAM_LOW_ADRS and RAM_HIGH_ADRS defines. (WIND00185815)
01h,22apr08,z_l  Add vxBus system clock support
01g,01nov07,h_k  disabled Power Management when cache snoop is enabled.
                 (CQ:108938)
01f,31mar06,dtr  Cleanup.
01e,22mar06,dtr  Enable secondary tsiG device by default.
01d,13mar06,dtr  Make on board tundra ethernet controller default device.
01c,25jan06,avb  Added snooping option
01b,10jan06,dtr  Cleanup for Wind River release.
01a,21jul05,avb  created based on templatePpc BSP
*/

/*
DESCRIPTION
This file contains the configuration parameters for the Freescale HPC2 BSP.
*/

#ifndef __INCconfigh
#define __INCconfigh

#ifdef __cplusplus
extern "C" {
#endif

/* BSP version/revision identification, before configAll.h */

#define BSP_VERSION        "6.9"       /* vxWorks 6.9 */
#define BSP_REV            "/0"        /* 0 for first revision */

#include <configAll.h>        /* Set the VxWorks default configuration */

#define BOOT_DEV_NAME "tsiG0"
#define DEFAULT_BOOT_LINE \
  "tsiG(0,0)host:vxWorks e=10.100.10.41 h=10.64.15.55 g=10.100.10.123 f=0x80 tn=hpc2"

/*    
 * If the FORCE_DEFAULT_BOOT_LINE is defined then the DEFAULT_BOOT_LINE
 * parameters are always used regardless of NVRAM values specified at
 * bootrom time. This is usually used to debug downloaded images without
 * a bootrom present.
 */
 
#undef  FORCE_DEFAULT_BOOT_LINE

/* User include/exclude section */

#define INCLUDE_I2C                     /* i2c.c */
#define INCLUDE_NVRAM                   /* NVRAM driver byteNvRam.c */
#define INCLUDE_FLASH                   /* flashMem.c */
#define INCLUDE_SYS_COPY_TO_FLASH       /* sysCopyToFlash() - sysLib.c */ 

#define INCLUDE_CACHE_SUPPORT           /* cacheLib support */
#define INCLUDE_CACHE_L2                /* include L2 cache support */
#define INCLUDE_PCI                     /* pciConfigLib/pciIntLib */
#define INCLUDE_PCI_AUTOCONF
#define	INCLUDE_SERIAL
#define INCLUDE_END
#define INCLUDE_NETWORK                 /* include networking support */
#define INCLUDE_TSI108_DMA              /* Include Tsi108 DMA support library */

#define INCLUDE_MMU_BASIC               /* bundled MMU support */
#define USER_I_MMU_ENABLE
#define USER_D_MMU_ENABLE
#define SNOOP_ENABLE                    /* enable snooping */

/* Serial port configuration */

#undef  NUM_TTY
#define NUM_TTY             N_SIO_CHANNELS    /* defined in template.h */

#undef  CONSOLE_TTY 
#define CONSOLE_TTY         0 
#undef  WDB_TTY_CHANNEL 
#define WDB_TTY_CHANNEL     1 

/* Memory configuration */

#define LOCAL_MEM_LOCAL_ADRS    0x00000000    /* Physical start of RAM */
#undef  LOCAL_MEM_AUTOSIZE                    /* run-time memory sizing */
#define LOCAL_MEM_SIZE          0x20000000    /* 512M of SDRAM memory */
#define USER_RESERVED_MEM       0x1E000000    /* only 32MB for VxWorks */

/*
 * The constants ROM_TEXT_ADRS and ROM_SIZE
 * are defined in config.h and Makefile.
 * All definitions for these constants must be identical.
 */

#define ROM_BASE_ADRS   0xfff00000                /* base address of ROM */
#define ROM_TEXT_ADRS   (ROM_BASE_ADRS + 0x0100)  /* with PC & SP */
#define ROM_TEXT2_ADRS  (FLASH_ADRS + 0x0100)
#define ROM_WARM_ADRS   (ROM_TEXT2_ADRS + 0x0008) /* warm reboot entry */
#define ROM_SIZE        0x00100000                /* 1MB ROM space */

/*
 * Cache configuration
 *
 * Note that when MMU is enabled, cache modes are controlled by
 * the MMU table entries in sysPhysMemDesc[], not the cache mode
 * macros defined here.
 */

#ifdef INCLUDE_CACHE_SUPPORT

#   define USER_I_CACHE_ENABLE              /* Enable INSTRUCTION CACHE */
#   undef  USER_I_CACHE_MODE
#   define USER_I_CACHE_MODE CACHE_COPYBACK /* select COPYBACK or WRITETHROUGH */

#   define USER_D_CACHE_ENABLE              /* Enable DATA CACHE */
#   undef  USER_D_CACHE_MODE

#   ifdef SNOOP_ENABLE
#       define USER_D_CACHE_MODE (CACHE_COPYBACK | CACHE_SNOOP_ENABLE)

/*
 * Disable power management which are defined in configAll.h.
 * Cache snoop mode can not be supported with the power management on this
 * board. (CQ:108938)
 */

#       undef INCLUDE_CPU_LIGHT_PWR_MGR

#   else
#       define USER_D_CACHE_MODE (CACHE_COPYBACK)
#   endif /* SNOOP_ENABLE */

#else

#   undef  USER_I_CACHE_ENABLE
#   undef  USER_I_CACHE_MODE
#   define USER_I_CACHE_MODE    CACHE_DISABLED
#   undef  USER_D_CACHE_ENABLE
#   undef  USER_D_CACHE_MODE
#   define USER_D_CACHE_MODE    CACHE_DISABLED
#   undef  USER_B_CACHE_ENABLE

#endif

#ifdef INCLUDE_CACHE_L2

#   define L2CACHE_MODE_WRITETHROUGH        0
#   define L2CACHE_MODE_COPYBACK            1
   
/* Set the default L2 cache mode */

#   define L2CACHE_MODE        L2CACHE_MODE_COPYBACK
    
#   define L2CACHE_SIZE        L2CACHE_SIZE_1M      /* 1MB  in sysL2Cache.h */

#endif /* INCLUDE_CACHE_L2  */


/* Network driver configuration */

#ifdef INCLUDE_NETWORK
#   define  INCLUDE_END          /* Enhanced Network Driver (see configNet.h) */
#   define  INCLUDE_TSI108_END   /* include tsi108End driver */
#   define  INCLUDE_SECONDARY_GIGE /* include init of secondary tsi108End */
#endif /* INCLUDE_NETWORK */

#ifdef INCLUDE_PCI

/* Intel 8254x support */

#   ifdef INCLUDE_GEI8254X_END
#       define INCLUDE_PRIMARY_GEI_END
#       undef  INCLUDE_SECONDARY_GEI_END
#       ifndef INCLUDE_END
#           define INCLUDE_END
#       endif  /* INCLUDE_END */                  
#   endif /* INCLUDE_GEI8254X_END */

#endif /* INCLUDE_PCI */

#define INCLUDE_VXBUS
#define INCLUDE_HWMEM_ALLOC
#define INCLUDE_PARAM_SYS
#define INCLUDE_PLB_BUS

#define DRV_TIMER_DEC_PPC
#define INCLUDE_TIMER_SYS
#define HWMEM_POOL_SIZE       50000
#define INCLUDE_VXB_CMDLINE

#include "hpc2.h"    /* Verify user options, specify i/o addr etc */

#ifdef __cplusplus
}
#endif

#endif /* __INCconfigh */
#if defined(PRJ_BUILD)
#include "prjParams.h"
#endif

