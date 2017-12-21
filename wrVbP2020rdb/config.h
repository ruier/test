/* config.h - Wind River Virtual Board P2020rdb configuration file */

/*
 * Copyright (c) 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,05apr11,agf  undef PCI and FLASH by default
01a,15feb11,agf  created
*/

#ifndef	INCconfigh
#define	INCconfigh

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* BSP version/revision identification, before configAll.h */

#define BSP_VERSION	"6.9"
#define BSP_REV		"/0"

/* Base model string. sysModel adds processor details */

#define SYS_MODEL	"Wind River Virtual Board P2020-RDB"

#include <vsbConfig.h>
#include <configAll.h>
#include "wrVbP2020rdb.h"


/* Default device driver components included for convenience */

#undef  INCLUDE_PCI_BUS
#define INCLUDE_ETSEC_VXB_END
#define INCLUDE_GENERICPHY
#define DRV_SIO_NS16550


/* Network and MAC configuration */

#define MAX_MAC_DEVS            3       /* three network devices (etsec) */


/* flash configuration */

#undef  INCLUDE_FLASH
#ifdef  INCLUDE_FLASH
#if 0
/* Hypervisor & guest BSP do not yet support entire flash chip */
#   define FLASH_ADRS           0xFF000000   /* Spansion S29GL128P        */
#   define FLASH_SIZE           0x01000000   /* 16MB device               */
#else
/* For now the sector for NVRAM parameters is made available as read-only */
#   define FLASH_ADRS           0xFFEE0000
#   define FLASH_SIZE           0x00020000
#endif

#   define NV_RAM_ADRS          0xFFEE0000   /* sector imm. below bootrom */
#   define NV_RAM_SIZE          0x00020000   /* 128KB sectors             */
#   define NV_BOOT_OFFSET       0
#   define NV_MAC_ADRS_OFFSET   0x200

#else
#   define NV_RAM_SIZE          NONE
#endif /* INCLUDE_FLASH */


/* console configuration */

#define TTY_UART1               0
#define TTY_UART2               1

#undef  CONSOLE_TTY
#define CONSOLE_TTY             sysConsoleTty

#undef  CONSOLE_BAUD_RATE
#define CONSOLE_BAUD_RATE       sysConsoleBaud

#undef  NUM_TTY
#define NUM_TTY                 2


/* cache configuration */
#define INCLUDE_CACHE_SUPPORT
#define USER_D_CACHE_ENABLE
#undef  USER_D_CACHE_MODE
#define USER_D_CACHE_MODE        (CACHE_COPYBACK | CACHE_SNOOP_ENABLE)
#define USER_I_CACHE_ENABLE
#undef  USER_I_CACHE_MODE
#define USER_I_CACHE_MODE        (CACHE_COPYBACK | CACHE_SNOOP_ENABLE)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* INCconfigh */

#if defined(PRJ_BUILD)
    #include "prjParams.h"
#else
#if !defined(WRVB_CMDLINE_TARGET_SUPPORTED)
#error This is a Virtual BSP. It must be built with the Wind River \
       project facility (vxprj or Workbench). In addition, it must be linked \
       with a VSB that supports Power PC Guest OS.
#endif /* !WRVB_CMDLINE_TARGET_SUPPORTED */
#endif /* PRJ_BUILD */

/* Finally, check the VBI version and notify user if necessary */
#include <wrhv/vbiVersion.h>
#if (VBI_VERSION_MAJOR != 2)
#error This BSP was developed for the Virtual Board Interface (VBI) version 2. \
       The VBI version used in this vxWorks installation appears to be otherwise.
#endif /* (VBI_VERSION_MAJOR != 2) */
