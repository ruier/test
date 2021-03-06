/* config.h - Motorola MVME5500 board configuration header */

/*
 * Copyright 2002-2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 1996-2003 Motorola, Inc. */

/*
modification history
--------------------
02t,12mar10,rec  CPU performance management update
02s,15apr10,clx  Update BSP version for vxWorks 6.9
02r,24jun09,e_d  Update BSP REV to "/7"
02q,13oct08,y_w  Fixed the TFFS issue. (WIND00135153)
02p,21may08,x_f  Added macro FORCE_DEFAULT_CPU_BUS_SPD
02o,06dec07,y_w  Update BSP REV to "/6"
02n,17oct07,pch  CQ100009: handle MPC745x errata #35
02m,15oct07,y_w  Fix the network problem after loading image.(WIND00107507)
02l,22sep07,y_w  Update BSP REV to "/5", fix Flash write slow problem. (#76030)
02k,12jul07,vik  added macro INCLUDE_VXB_CMDLINE
02j,29jan07,rlg  updates for NOR Conversion
02i,21sep06,sup  fix for defect WIND00066405 removed compiler warnings
02h,19sep06,jb3  Update BSP_REV for TFFS build error and un-write protect.
02g,08aug06,sup  included the vxBus components
02f,07aug06,sup  remove all the include components related to vxBus
02e,16jul06,sup  Include components for PPC Dec Timer vxus complaint added
02d,28apr06,jmt  Add SYS_MODEL definition to define base sysModel string
02c,14jun05,pcm  removed INCLUDE_DOSFS
02c,30aug04,j_b  remove INCLUDE_NETWORK dependencies (SPR #99747)
02c,30jul04,md   use default PM_RESERVED_MEM 
02b,24jun04,mdo  SPR #98438
02a,18may04,jln  BSP revision change; add PM_RESERVED_MEM;
                 fix SPR 96420;
01z,13feb04,nrv  adding tffs support
01y,17nov03,jln  Update BSP REV to "/1"
01x,09oct03,scb  Update BSP REV to "/0.4".
01w,06oct03,scb  #define IDMA_SNOOP_ON by default
01v,03sep03,cak  Added L3 Cache Private Memory support.
01u,25jul03,scb  Update BSP REV to "/0.3".
01t,24jul03,scb  Correct comment on INCLUDE_GEI_END
01s,17jul03,scb  define USER_D_CACHE_MODE (CACHE_COPYBACK | CACHE_SNOOP_ENABLE)
01r,08jul03,scb  L2 and L3 confured for data only - as per Wind River support.
01q,24jun03,scb  Remove warning about Altivec support, it works on T2.2.1
01p,17jun03,yyz  Combine definition of IDMA burst limit with IDMA_SNOOP_ON.
01o,02jun03,scb  DMA tuning: Enable 'block transfer' for VME DMA, add
                 IDMA burst limit configurable and snooping defines.
01n,28may03,scb  Enable L2 and L3 cache by default.
01m,09apr03,yyz  Added #define STANDALONE_NET for shared memory in standalone
                 mode. Fixed comments for INCLUDE_SM_NET.
01l,27mar03,scb  A change in plans, turn on ECC by default.
01k,10mar03,cak  IPMC SCSI and serial support - required moving PCI 16-bit
                 I/O from PCI bus 1.0 to PCI bus 0.0.
01j,28feb02,scb  Update rev level from 1.2/0.1 to 1.2/0.2 for shared mem patch.
01i,28feb02,scb  #undef INCLUDE_ECC (default setting)
01h,20feb03,scb  Change default to enable ECC.
01g,20feb03,scb  Disable only data cache (not instruction cache) for
                 shared memory support.
01f,18feb03,scb  Fix botched commentary on memory map.
01e,10feb03,scb  Minor tweek for shared memory support.
01d,05dec02,scb  Warning about no shared memory support (0.1 release).
01c,19nov02,scb  Preliminary shared memory support.
01b,12nov02,scb  VME/Universe support.
01a,01oct02,scb  Adapted from hxeb100 base (ver 01e).
*/

/*
DESCRIPTION
This file contains the configuration parameters for the MVME5500
Single Board Computer.  This file must not contain any "dependencies"
(ie. #ifdef's or #ifndef's).
*/

#ifndef __INCconfigh
#define __INCconfigh

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

/* The following defines must precede configAll.h */

/* BSP version/revision identification */

#define BSP_VERSION        "6.9"       /* vxWorks 6.9 */
#define BSP_REV            "/0"        /* 0 for first revision */

/* Base model string. sysModel adds processor details */

#define SYS_MODEL       "Motorola "

/* PRIMARY INCLUDES */

#include "configAll.h"

/* defines */

#define INCLUDE_VXBUS
#define INCLUDE_PARAM_SYS
#define INCLUDE_HWMEM_ALLOC
#define INCLUDE_PLB_BUS
#define HWMEM_POOL_SIZE 50000
#define INCLUDE_TIMER_SYS
#define DRV_TIMER_DEC_PPC

#define INCLUDE_VXB_CMDLINE
#define DEFAULT_BOOT_LINE \
    "gei(0,0)host:/tornado/mv5500/vxWorks h=90.0.0.1 e=90.0.0.2 u=vxworks"
#define WRONG_CPU_MSG "A PPC604 VxWorks image cannot run on a PPC603!\n"

/*
 * Default board configurations
 *
 * If a supported feature is desired,
 *         change to: #define
 * If a feature is not desired or not supported
 *         change to: #undef
 *
 * NOTE: Not all functionality is supported on all boards
 */

#define INCLUDE_MMU_BASIC
#define INCLUDE_CACHE_L2        /* L2 cache support */
#define INCLUDE_CACHE_L3    /* L3 cache support */
#undef  INCLUDE_CACHE_L3_PM /* L3 cache Private Memory support */
#define INCLUDE_ECC     /* System Memory ECC Support */
#undef  INCLUDE_AUX_CLK         /* MV64260 aux clock support */
#undef  INCLUDE_MV64260_DMA     /* MV64260 DMA */
#undef  INCLUDE_VME_DMA     /* Universe (VME) DMA */
#define INCLUDE_GEI_END     /* Intel 82544 gigabit ethernet */
#undef  INCLUDE_WANCOM_END  /* GT-64260 integrated ethernet driver */
#undef  INCLUDE_IPMC            /* IPMC761 & IPMC712 Support */

#ifdef  INCLUDE_CACHE_L3
/*
 * The following must be #defined if sysL3CacheDisable() will
 * be called on a processor affected by MPC7457 errata #35:
 * Disabling L3 can trigger erroneous L3 Tag Parity Error.
 * The published workaround consists of disabling the L3 cache,
 * the L3 external clocks, and parity checking -- all in the
 * same cycle -- while keeping the L3CR[IONLY] and L3CR[DONLY]
 * bits set.
 */
 
#define _MPC7457_ERRATA_35  /* include w/a for MPC7457 erratum #35 */
#endif  /* INCLUDE_CACHE_L3 */

/*
 * Disable power management which are defined in configAll.h.
 * Cache snoop mode can not be supported with the power management.
 * see WIND00108938 for details.
 */
 
#undef  INCLUDE_CPU_LIGHT_PWR_MGR

/*
 * Setting USER_D_CACHE_MODE as shown below accomplishes, among other
 * things:
 *
 * CACHE_COPYBACK
 * cacheDmaMalloc() will perform allocations in memory and leave
 * this memory cached after the allocation.  This is necessary for
 * high performance PCI devices such as the gigabit ethernet.  If
 * CACHE_COPYBACK back is not included in USER_D_CACHE_MODE, then
 * cacheDmaMalloc() will allocate buffers and then change the
 * caching attribute of the allocated memory to not cached.  This
 * can have adverse affects on performance for devices such as the
 * gigabit ethernet which use cacheDmaMalloc().
 *
 * CACHE_SNOOP_ENABLE
 * Setting CACHE_SNOOP_ENABLE will cause Wind River drivers (such
 * as gigabit ethernet) to rely totally  on hardware to do snooping
 * rather than requiring the driver itself to perform explicit cache
 * invalidates and cache flushes.  Relying on hardware for this
 * operation will enhance* performance for devices such as the
 * gigabit ethernet.
 */

#undef  USER_D_CACHE_MODE
#define USER_D_CACHE_MODE       (CACHE_COPYBACK | CACHE_SNOOP_ENABLE)

/* L3 Cache Private Memory Support */

#ifdef INCLUDE_CACHE_L3_PM
#   define INCLUDE_CACHE_L3_PM_BASE 0xF1200000
#endif /* INCLUDE_CACHE_L3_PM */

/*
 * If any slave board in the chassis cannot generate a VMEbus
 * RMW, or the master board cannot translate an incoming VMEbus
 * RMW into an atomic operation, define ANY_BRDS_IN_CHASSIS_NOT_RMW
 * else
 * All slave boards are generating a VMEbus RMW, and the master
 * board can translate an incoming VMEbus RMW into an atomic
 * operation, so undef ANY_BRDS_IN_CHASSIS_NOT_RMW.
 *
 * Note that the MVME5500 cannot translate an incoming VMEbus
 * RMW cycle into an atomic operation due to a limitation of the
 * GT-64260 host bridge.
 */

#define ANY_BRDS_IN_CHASSIS_NOT_RMW

#undef  INCLUDE_ALTIVEC         /* Support for Altivec coprocessor */
#undef  INCLUDE_RTC             /* Real-time clock support */
#undef  INCLUDE_FAILSAFE        /* Failsafe (watchdog) timer support */
#undef  INCLUDE_DPM             /* Processor Dynamic Power Management */

#ifdef INCLUDE_IPMC
#   define INCLUDE_PCX7307_SIO  /* Additional 16550-compatible COM Ports */
#   define INCLUDE_Z85230_SIO   /* Z8536/X85230 synchronous COM Ports */
#   define INCLUDE_SCSI         /* SCSI Support */
#endif /* INCLUDE_IPMC */

/*            
 * The default cpu bus speed of mv5500 is 133MHz.
 * If FORCE_DEFAULT_CPU_BUS_SPD is undefined, it will caculated the bus rate 
 * in sysCpuBusSpd(). It will take more time in booting procedure. 
 */ 

#define FORCE_DEFAULT_CPU_BUS_SPD

/*
 * MV64260 System Memory Controller Support
 *
 * The MV64260's system memory controller (SDRAM) registers
 * contain a number of fields that can be configured for system
 * optimization.  The following definitions are used to configure
 * these fields.  To enable/disable or change these default settings
 * simply modify these definitions.  Only those fields described below
 * can be modified.
 *
 * SDRAM Configuration Register:
 * Bit[14] Enable Virtual Banks Interleaving
 * Bit[15] Enable Physical Banks Interleaving
 * Bits[31:26] Read Buffer Assignment per each interface
 *
 * By default we will enable physical bank interleaving, and virtual bank
 * interleaving, and will set the CPU interface to use read buffer 1 while
 * all other interfaces use read buffer 0.
 *
 * By default this will be set to 0x04000000.
 *
 * SDRAM Address Control Register:
 * Bits[3:0] SDRAM Address Select
 *
 * By default this will be set to 0x2.
 *
 * SDRAM Interface Crossbar Control (Low) Register:
 * Bits[3:0]   Slice 0 device controller "pizza" arbiter
 * Bits[7:4]   Slice 1
 * Bits[11:8]  Slice 2
 * Bits[15:12] Slice 3
 * Bits[19:16] Slice 4
 * Bits[23:20] Slice 5
 * Bits[27:24] Slice 6
 * Bits[31:28] Slice 7
 *
 * By default this will be set to 0x00765432.
 *
 * SDRAM Interface Crossbar Control (High) Register:
 * Bits[3:0]   Slice 8
 * Bits[7:4]   Slice 9
 * Bits[11:8]  Slice 10
 * Bits[15:12] Slice 11
 * Bits[19:16] Slice 12
 * Bits[23:20] Slice 13
 * Bits[27:24] Slice 14
 * Bits[31:28] Slice 15
 *
 * By default this will be set to 0x00765432.
 *
 * SDRAM Interface Crossbar Timeout Register:
 * Bits[7:0] Crossbar Arbiter Timeout Preset Value
 * Bit[16]   Crossbar Arbiter Timer Enable
 *
 * By default this will be set to 0x000100FF.
 *
 * SDRAM Banks Parameters Registers:
 * (There is one register for each bank - 0, 1, 2, and 3.)
 * Bit[16] Keep virtual bank0 pages open
 * Bit[17] Keep virtual bank1 pages open
 * Bit[18] Keep virtual bank2 pages open
 * Bit[19] Keep virtual bank3 pages open
 *
 * By default this will be set to 0x000F0000 to keep each virtual
 * page open at the end of an access.
 */

#define MV64260_SDRAM_CFG_DFLT                  0x04000000
#define MV64260_SDRAM_ADDR_CTRL_DFLT            0x00000002
#define MV64260_SDRAM_IF_XBAR_CTRL_LO_DFLT      0x00765432
#define MV64260_SDRAM_IF_XBAR_CTRL_HI_DFLT      0x00765432
#define MV64260_SDRAM_IF_XBAR_TMOUT_DFLT        0x000100FF
#define MV64260_SDRAM_BANK0_PARAMS_DFLT         0x000F0000
#define MV64260_SDRAM_BANK1_PARAMS_DFLT         0x000F0000
#define MV64260_SDRAM_BANK2_PARAMS_DFLT         0x000F0000
#define MV64260_SDRAM_BANK3_PARAMS_DFLT         0x000F0000

/*
 * IPMC SCSI Support
 *
 *     INCLUDE_SCSI2 : Necessary for SCSI2 library.
 * INCLUDE_DISK_UTIL : Necessary for complete file system support.
 * INCLUDE_SCSI_BOOT : Enables booting from a SCSI device.
 *  SCSI_AUTO_CONFIG : Enables an automatic scan of the SCSI bus at
 *                     startup.
 *  SCSI_WIDE_ENABLE : Enable wide SCSI (16-bit data transfers).
 *   SYS_SCSI_CONFIG : Enables you to declare a SCSI peripheral
 *                     configuration, and is meant as a substitute for
 *                     SCSI_AUTO_CONFIG.  You must also edit
 *                     sysScsiConfig in sysScsi.c, so that it reflects
 *                     the actual configuration of your SCSI bus.
 *   INCLUDE_SYM_895 : Provides support for the Symbios/LSI Logic
 *                     SYM53C895A PCI to Ultra2 SCSI controller.
 *       ULTRA2_SCSI : Configures the SYM53C895A for Ultra2 SCSI support.
 */

#ifdef INCLUDE_SCSI
#   define INCLUDE_SCSI2        /* Use SCSI2 library, not SCSI1 */
#   define INCLUDE_DISK_UTIL    /* DOS Utilities */
#   define INCLUDE_SCSI_BOOT    /* include ability to boot from SCSI */
#   define SCSI_AUTO_CONFIG     /* scan bus for devices on startup */
#   define SCSI_WIDE_ENABLE     /* enable wide SCSI, 16-bit data xfrs */
#   undef  SYS_SCSI_CONFIG       /* call sysScsiConfig in sysScsi.c */
#   define INCLUDE_SYM_895      /* Symbios 895 support */
#   define ULTRA2_SCSI          /* Ultra2 SCSI */
#endif /* INCLUDE_SCSI */

/*
 * PCI, PCI Auto-Configuration and VME Support
 *
 * PCIx_MSTR_IO_SIZE, PCIx_MSTR_MEMIO_SIZE and PCIx_MSTR_MEM_SIZE
 * control the sizes of the available   PCI address spaces.  ('x' is
 * '0' for bus 0.0 and '1' for bus 1.0) The windows defined by these
 * parameters must be large enough to accommodate all of the PCI
 * memory and I/O space requests found during PCI autoconfiguration.
 * If they are not, some devices will not be autoconfigured.
 * Other configurable sizes are ISA_MSTR_IO_SIZE and the sizes which
 * affect VME space: VME_A16_MSTR_SIZE, VME_A24_MSTR_SIZE, and
 * VME_A32_MSTR_SIZE.
 *
 * Be aware that these values alter entries in the sysPhysMemDesc[]
 * array (in sysLib.c) and affect the size of the resulting MMU tables
 * at the rate of 128:1.  For each 128K of address space, a 1K piece
 * of RAM will be used for MMU tables.  If the region is very large,
 * modifications to sysLib.c can be made to use BAT (Block Address
 * Translation) registers instead of MMU page tables to map the
 * memory.
 *
 * NOTE: Since PCI auto-configuration is performed by the bootroms,
 * changing any of these values requires the creation of new bootroms.
 *
 * The memory map below reflects the view if the delivered default
 * values for the various size parameters and for PCI_MEM_LOCAL_START
 * are used.
 *
 * CPU Space                               PCI (|) or VME (}) Space
 * (default values)
 *
 *
 *              (0x00000000)       +-----------------+
 *                                 :                 :
 *                                 :  System Memory  :
 *                                 :   (up to 2GB)   :
 *                                 :                 :
 *                                 :                 :
 * PCI1_MSTR_MEMIO_LOCAL =          ----------------- PCI1_MSTR_MEMIO_BUS =
 *              (0x80000000)       |  Bus 1.0        |  PCI1_MSTR_MEMIO_LOCAL =
 *                                 | NonPrefetch PCI |  (0x80000000)
 *                                 | Memory space    |
 *                                 |      16MB       |
 *                                 | (0x01000000)    |
 *                                 |                 |
 * PCI1_MSTR_MEM_LOCAL =            ----------------- PCI1_MSTR_MEM_BUS =
 * PCI1_MSTR_MEMIO_LOCAL +         |  Bus 1.0        |  PCI1_MSTR_MEM_LOCAL
 * PCI1_MSTR_MEMIO_SIZE =          | Prefetch PCI    |  (0x81000000)
 *              (0x81000000)       | Memory space    |
 *                                 |    8MB          |
 *                                 | (0x00800000)    |
 * ISA_MSTR_IO_LOCAL =              ----------------- ISA_MSTR_IO_BUS
 * PCI1_MSTR_MEM_LOCAL +           |  Bus 0.0 16-bit |  (0x00000000)
 * PCI1_MSTR_MEM_SIZE =            | I/O space       |
 *              (0x81800000)       |    64KB         |
 *                                 | (0x00010000)    |
 * PCI0_MSTR_IO_LOCAL =             ----------------- PCI0_MSTR_MEM_BUS =
 * ISA_MSTR_IO_LOCAL +             |  Bus 0.0 32-bit |  PCI0_MSTR_MEM_LOCAL
 * ISA_MSTR_IO_SIZE =              | I/O space       |  (0x00010000)
 *              (0x81810000)       | 8MB - 64KB      |
 *                                 | (0x007f0000)    |
 * PCI1_MSTR_IO_LOCAL =             ----------------- PCI1_MSTR_IO_BUS =
 * PCI0_MSTR_IO_LOCAL +            |  Bus 1.0 32-bit |   (0x00800000)
 * PCI0_MSTR_IO_SIZE =             | I/O space       |
 *              (0x82000000)       |    8MB          |
 *                                 | (0x00800000)    |
 * PCI0_MSTR_MEM_LOCAL =            ----------------- PCI0_MSTR_MEM_BUS =
 * PCI1_MSTR_IO_LOCAL +            |  Bus 0.0 32-bit |   (0x82800000)
 * PCI1_MSTR_IO_SIZE =             | Prefetch PCI    |
 *              (0x82800000)       | Memory space    |
 *                                 |    8MB          |
 *                                 | (0x00800000)    |
 * PCI0_MSTR_MEMIO_LOCAL =          ----------------- PCI0_MSTR_MEMIO_BUS =
 * PCI0_MSTR_MEM_LOCAL +           |  Bus 0.0        |   (0x83000000)
 * PCI0_MSTR_MEM_SIZE =            | NonPrefetch PCI |
 *              (0x83000000)       | Memory Space    |
 *                                 |    8MB          |
 *                                 | (0x00800000)    |
 *              (0x83800000)        ----------------- (0x83800000)
 *                                 :                 :
 *                                 : Not used,       :
 *                                 : Available for   :
 *                                 : expansion of    :
 *                                 : PCI spaces and  :
 *                                 : VME spaces      :
 *                                 :                 :
 *                                 :                 :
 * VME_A32_MSTR_LOCAL =             -----------------
 *              (0xe7f00000)       |                 } Translated to VME bus
 *       LSI1.bs =     (0xe7f00000)|                 } (0x08000000) LSI1.bs
 *                                 | VME A32 space   }
 *                                 |     128MB       }
 *       LSI1.bd - 1 = (0xe7f3ffff)| (0x08000000)    } (0x0803ffff) LSI1.bd -1
 *       LSI0.bs =     (0xe7f40000)| LSI0 and LSI1   } (0xfb000000) LSI0.bs
 *                                 | outbound maps   }
 *       LSI0.bd - 1 = (0xe7f4ffff)| through here    } (0xfb00ffff) LSI0.bd - 1
 *                     (0xe7f50000)|                 } (0xfb010000)
 *                                  .................
 *                                 :   Unused A32    :
 *                                  -----------------
 * VME_A24_MSTR_LOCAL =            |                 }
 *               (0xeff00000)      | VME A24 space   } Translated:
 *       LSI2.bs =     (0xeff00000)|     16MB        } (0xff000000) LSI2.bs
 *                                 | (0x01000000)    }
 *       LSI2.bd - 1 = (0xf0efffff)| LSI2 maps       } (0xffffffff) LSI2.bd - 1
 *                     (0xf0f00000)| through here    }
 * VME_A16_MSTR_LOCAL =             -----------------
 *                (0xf0f00000)     |                 }
 *                                 | VME A16 space   } Translated:
 *       LSI3.bs =     (0xf0f00000)|     64KB        } (0xffff0000) LSI3.bs
 *                                 | (0x00010000)    }
 *       LSI3.bd - 1 = (0xf0f0ffff)| LSI3 maps       } (0xffffffff) LSI3.bd - 1
 *                     (0xf0f10000)| through here    }
 *                                  -----------------
 *                                 :     Unused      :
 * MV64260_REG_BASE =               -----------------
 *                    (0xf1000000) | MV64260         :
 *                                 | internal regs.  :
 *                                 |    64K          :
 *                                 | (0x00010000)    :
 *                    (0xf1010000)  -----------------
 *                                 :     Unused      :
 * L3_CACHE_PRIVATE_MEMORY_BASE =   -----------------
 *                    (0xf1200000) :   L3 Cache      :
 *                                 : Private Memory  :
 *                                 :   (OPTIONAL)    :
 *                                 :  1MB - 2MB      :
 *                    (0xf1400000)  -----------------
 *                                 :     Unused      :
 *                                 :                 :
 *                                 :                 :
 * MV64260_DEVCS1_BASE_ADDR =       -----------------
 *                    (0xf1d00000) | Device bank     :
 *                                 |    1MB          :
 *                                 | (0x00100000)    :
 *                                 |                 :
 *                    (0xf1e00000)  -----------------
 *
 *
 *                              . . . . . . . . . .
 *
 * If booting from flash bank A (soldered):
 * ----------------------------------------
 *
 *                    (0xf1e00000)  -----------------
 *                                 : Not used        :
 * MV64260_FLSH_BANK_B_ADDR_       :      2MB        :
 * BOOT_A =                        :                 :
 *                    (0xf2000000)  -----------------
 *                                 | Flash bank B    :
 *            (8MB)   (0xf2800000) | 8MB, 16MB, or   :
 *            (16MB)  (0xf3000000) | 32MB            :
 *                                 |                 :
 *            (32MB)  (0xf4000000) |                 :
 *                                  -----------------
 *                                 : Not used        :
 *                                 :     64MB        :
 *                                 :                 :
 *                                 :                 :
 *       MV64260_FLSH_BANK_A_ADDR =   -----------------
 *                    (0xf8000000) | Flash bank A    :
 *                                 |     128MB       :
 *                                 |                 :
 *                                 |                 :
 *                                 |                 :
 *                    (0xffffffff)  -----------------
 *
 *                              . . . . . . . . . .
 *
 * If booting from flash bank B (socketed):
 * ----------------------------------------
 *
 *                    (0xf1e00000)  -----------------
 *                                 : Not used        :
 *                                 :     98MB        :
 *                                 :                 :
 *      MV64260_FLSH_BANK_A_ADDR = :                 :
 *                    (0xf8000000)  -----------------
 *                                 | Flash bank A    :
 *            (8MB)   (0xf8800000) | 8MB, 16MB,      :
 *            (16MB)  (0xf9000000) | 32MB or 64MB    :
 *                                 |                 :
 *            (32MB)  (0xfa000000) |                 :
 *                                 |                 :
 *                                 |                 :
 *                                 |                 :
 *                                 |                 :
 *            (64MB)  (0xfc000000)  -----------------
 *                                 : Not used        :
 *                                 :        56MB     :
 *                                 :                 :
 *                                 :                 :
 *       MV64260_FLSH_BANK_B_ADDR_  -----------------
 *       BOOT_B (0xfe000000)       | Flash bank B    :
 *                                 |      32MB       :
 *                                 |                 :
 *                    (0xffffffff)  -----------------
 *
 * Notes in inbound translations from VME bus:
 *
 *
 * VSI1, VSI4, VSI5:
 * Inbound access (VME slave window) from VME A32 space.
 * Inbound mapping is performed only if SM_OFF_BOARD is #define as
 * FALSE, indicating that this node hosts the shared-memory anchor
 * and memory pool.
 *
 * In case all slave boards in the chassis are capable of generating a
 * VMEbus RMW, and the master is capable of translating an incoming
 * RMW into an atomic operation, the A32 incoming access range is
 * divided into 3 separate windows.  Note that all BSPs in the
 * chassis will be built with #undef ANY_BRDS_IN_CHASSIS_NOT_RMW.
 *
 * The first window, governed by VSI1, will allow normal access to the
 * start of local memory up to the Shared Memory Region.
 *
 * The VMEbus range is:         The translated local DRAM range is:
 *
 * VME_A32_MSTR_BUS
 * (0x08000000)                         (0x00000000)
 *
 *     to                                   to
 *
 * VME_A32_MSTR_BUS +
 * SM_MEM_ADRS
 * (0x08010000)                         (0x00010000)
 *
 * The second window, governed by VSI4 will allow Read-Modify-Write
 * (RMW) access to the Standard VxWorks and VxMP's Shared Memory
 * Region.
 *
 *
 * The VMEbus range is:         The translated local DRAM range is:
 *
 * VME_A32_MSTR_BUS +                   (0x00010000)
 * SM_MEM_ADRS
 * (0x08010000)
 *
 *     to                                   to
 *
 * VME_A32_MSTR_BUS +
 * SM_MEM_ADRS +
 * SM_MEM_SIZE +
 * SM_OBJ_MEM_SIZE
 * (0x08040000)                         (0x00040000)
 *
 * If the any slave board is incapable of generating a VMEbus RMW, then
 * all boards will be configured with #define ANY_BRDS_IN_CHASSIS_NOT_RMW.
 * and the total of DRAM will be mapped through VSI1.
 *
 * The VMEbus range is:         The translated local DRAM range is:
 *
 * VME_A32_MSTR_BUS
 * (0x08000000)                         (0x00000000)
 *
 *      to                                  to
 *
 * (0x08000000 + size of                (top of DRAM)
 *  DRAM)
 *
 * The third window, governed by VSI5 will allow normal access to the
 * local memory starting after the Shared Memory Region up to the end
 * of physical memory.
 *
 * Here's a recap:
 *
 * If this is not the anchor node (SM_OFF_BOARD == TRUE)
 * then
 *     No A32 inbound mappings involving VSI1, VSI4, or VSI5 are
 *     applied.  Note that all nodes must have inbound mapping
 *     to reach their mailbox's so shared memory doorbells can
 *     be hit by any other node.  This is taken care of by the
 *     RAI register programming.
 * else
 *     (this is the anchor node ...)
 *     If at least one of the nodes in the chassis is not capable
 *     read-modify-write cycle generation on VME bus
 *     (ANY_BRDS_IN_CHASSIS_NOT_RMW is #define'd)
 *     then
 *         VMEbus read-modify-write generation will be used by NONE of the
 *         boards in the chassis.  All of DRAM will be mapped through
 *         inbound VSI1.  VSI5 and VSI5 do not apply.
 *     else
 *         (all boards in chassis capable of RMW cycles)
 *         The following mapping is applied:
 *             VSI1 maps memory from zero to start of shared memory
 *                  region.
 *             VSI4 maps memory from where VSI1 leaves off and covers
 *                  the standard VxWorks and VxMP's Shared Memory
 *                  region.
 *             VSI5 maps memory from where VSI4 leaves off and covers
 *                  the remaining memory to the top of DRAM.
 *
 *
 * PCI Mapped Memory consists of non-prefetchable and prefetchable.
 * Each bus has its own non-prefetchable and prefetchable memory
 * spaces.
 *
 * The maximum amount of non-prefetchable PCI memory is defined by
 * the user as PCI0_MSTR_MEMIO_SIZE and PCI1_MSTR_MEMIO_SIZE.
 *
 * The maximum amount of prefetchable PCI memory is defined by the
 * user as PCI0_MSTR_MEM_SIZE and PCI1_MSTR_MEM_SIZE.
 *
 * PCI Mapped I/O consists of 16 and 32 bit I/O. Only bus 0.0 contains
 * 16 bit I/O space.
 *
 * The amount of 16 bit I/O, for bus 0.0, is configured by
 * ISA_MSTR_IO_SIZE and defaults to 64KB in size. This area begins at
 * 0x0 on the PCI I/O bus (0.0), and ISA_MSTR_IO_LOCAL on
 * the processor bus.
 *
 * The amount of 32 bit I/O is configured by PCI0_MSTR_IO_SIZE and
 * PCI1_MSTR_IO_SIZE.
 *
 * PCI_MEM_LOCAL_START:
 *
 * Local (CPU referenced) base address of all PCI space.  This must satisfy
 * the following conditions:
 *
 * 1) It must have a value greater than the highest DRAM address.
 * 2) It must be a multiple of 0x00100000 to satisfy host bridge address
 *       decoders.
 *
 * The space beginning with this address and ending with the base address
 * of the host bridge registers (MV64260_REG_BASE = 0xf1000000) is used
 * for local access to PCI space (the bottom of this range) and for local
 * access of VME space (the top of this range).
 */

#define PCI_MEM_LOCAL_START     0x80000000  /* Bottom of PCI memory space */

/*
 * Bus 0.0 PCI Space Definitions - must be set to power of 2, in
 * the case of ISA_MSTR_IO_SIZE and PCI0_MSTR_IO_SIZE, their sum
 * must be a power of 2
 */

#define PCI0_MSTR_MEMIO_SIZE    0x00800000  /* 8 MB */
#define PCI0_MSTR_MEM_SIZE      0x00800000  /* 8 MB */
#define ISA_MSTR_IO_SIZE        0x00010000  /* 64 kbytes */
#define PCI0_MSTR_IO_SIZE       0x007f0000  /* 8 MB - 64 KB */

/* Bus 1.0 PCI Space Definitions - must be set to power of 2 */

#define PCI1_MSTR_MEMIO_SIZE    0x01000000  /* 16 MB */
#define PCI1_MSTR_MEM_SIZE      0x00800000  /* 8 MB */
#define PCI1_MSTR_IO_SIZE       0x00800000  /* 8 MB */

/*
 * VME Memory Mapping size Definitions  - note:  See "mv5500A.h" for commentary
 * regarding PCI-VME and VME-PCI mappings.
 */

#define VME_A32_MSTR_SIZE       0x08000000      /* 128 MB */
#define VME_A24_MSTR_SIZE       0x01000000      /* 16 MB */
#define VME_A16_MSTR_SIZE       0x00010000  /* 64k */

#ifdef  INCLUDE_MV64260_DMA     /* MV64260 DMA */

    /*
     * IDMA_SNOOP_ON is #define'd by default:
     * IDMA snooping is enabled by default to ensure cache coherency
     * on DMA transfers.  It is important to have IDMA snooping defined
     * if the DMA regions are in cached memory.  If caching is turned
     * off for these memory regions then IDMA snooping can also be turned
     * off.  Doing so would result in faster DMA transfers at the
     * expense of dealing with uncached memory.
     */

#   define   IDMA_SNOOP_ON

    /*
     * Burst limit choices are 8, 16, 32, 64 or 128 bytes.  The higher the
     * burst limit, the faster the transfer.  DMA buffer address should be
     * aligned to burst limit granularity for best performance.
     *
     * Since snooping is working on cache line basis, any access to SDRAM
     * interface unit that requires snoop must not cross the cache line
     * boundary.  Therefore, if snooping is turned on, the IDMA burst limit
     * must not exceed 32 bytes.
     */

#   ifdef   IDMA_SNOOP_ON
#       define  IDMA_DST_BURST_LIMIT IDMA_CTRL_DBURST_LIMIT_32B
#       define  IDMA_SRC_BURST_LIMIT IDMA_CTRL_SBURST_LIMIT_32B
#   else
#       define  IDMA_DST_BURST_LIMIT IDMA_CTRL_DBURST_LIMIT_128B
#       define  IDMA_SRC_BURST_LIMIT IDMA_CTRL_SBURST_LIMIT_128B
#   endif   /* IDMA_SNOOP_ON */

#endif  /* INCLUDE_MV64260_DMA */

#ifdef  INCLUDE_VME_DMA
/* Default DMA operating parameters used by sysVmeDmaInit() */

/*
 * VMEbus data transfer type
 * - a combination of VMEbus Max. Datawidth (VDM) and
 *   VMEbus Cycle Type (VCT).
 *
 *  choices:
 *      (DCTL_VDW_8  | DCTL_VCT_SINGLE)
 *      (DCTL_VDW_16 | DCTL_VCT_SINGLE)
 *      (DCTL_VDW_32 | DCTL_VCT_SINGLE)
 *      (DCTL_VDW_64 | DCTL_VCT_SINGLE)
 *      (DCTL_VDW_32 | DCTL_VCT_BLK)    ** BLT  **
 *      (DCTL_VDW_64 | DCTL_VCT_BLK)    ** MBLT **
 */

#   define VME_DMA_XFER_TYPE    (DCTL_VDW_64 | DCTL_VCT_BLK)

/*
 * VMEbus Address Space type
 *  choices:
 *      DCTL_VAS_A16
 *      DCTL_VAS_A24
 *      DCTL_VAS_A32
 */

#   define VME_DMA_ADDR_SPACE   DCTL_VAS_A32

/*
 * Program/Data Address Modifier (AM) Code
 *  choices:
 *      DCTL_PGM_DATA
 *      DCTL_PGM_PRGM
 */

#   define VME_DMA_DATA_TYPE    DCTL_PGM_DATA

/*
 * Supervisor/User Address Modifier (AM) Code
 *  choices:
 *      DCTL_SUPER_USER
 *      DCTL_SUPER_SUP
 */

#   define VME_DMA_USER_TYPE    DCTL_SUPER_SUP

/*
 * VMEbus Aligned DMA Transfer Count (in bytes)
 * - Max. data transferred before giving up the VMEbus
 *  choices:
 *      DGCS_VON_DONE
 *      DGCS_VON_256
 *      DGCS_VON_512
 *      DGCS_VON_1024
 *      DGCS_VON_2048
 *      DGCS_VON_4096
 *      DGCS_VON_8192
 *      DGCS_VON_16384
 */

#   define VME_DMA_MAX_BURST        DGCS_VON_DONE

/*
 * Minimum period the DMA is off the VMEbus between tenures (in microseconds)
 *  choices:
 *      DGCS_VOFF_0
 *      DGCS_VOFF_16
 *      DGCS_VOFF_32
 *      DGCS_VOFF_64
 *      DGCS_VOFF_128
 *      DGCS_VOFF_256
 *      DGCS_VOFF_512
 *      DGCS_VOFF_1024
 */

#   define VME_DMA_MIN_TIME_OFF_BUS DGCS_VOFF_0

#endif  /* INCLUDE_VME_DMA */

/*
 * Cache Support
 *
 * The symbol INCLUDE_CACHE_SUPPORT (in "configAll.h") is #defined by
 * default and causes L1 cache support (including enabling of the cache)
 * to be provided.  In the event that no L1 cache enabling is desired
 * include a #undef INCLUDE_CACHE_SUPPORT here. However, note that if
 * INCLUDE_CACHE_SUPPORT is not defined (L1 cache not enabled) then
 * L2 cache will not be enabled as well, regardless of the state of the
 * INCLUDE_CACHE_L2 define.  This is because the L2 cache cannot be
 * enabled if the L1 cache is not enabled.  The same holds true for the
 * L3 cache and the INCLUDE_CACHE_L3 and INCLUDE_CACHE_L3_PM
 * defines.  If INCLUDE_CACHE_SUPPORT is not defined (L1 cache not
 * enabled) then the L3 cache cannot be configured, as cache or as
 * private memory.
 *
 * L2 & L3 Cache Support
 *
 * The following features of the L2 cache can be configured (these are
 * relevant only if INCLUDE_CACHE_L2 is defined):
 *
 * (1) Number of prefetch engines enabled
 * The number of prefetch engines enabled can range from none (disabled)
 * to 3.  This is controlled with the MSSCR0_L2_PREFETCH_ENGINES definition.
 * The default number of prefetch engines is 1 (MSSCR0_L2PFE_ONE). Valid
 * settings include: MSSCR0_L2PFE_NONE, MSSCR0_L2PFE_TWO, and
 * MSSCR0_L2PFE_THREE.
 *
 * (2) Enable/Disable L2 Data Parity Checking
 * L2 data parity checking can be enabled/disabled with the
 * ENABLE_L2_DATA_PARITY definition.  Parity checking is enabled by
 * default.
 *
 * (3) Mode
 * The L2 cache can be configured for Instruction-only mode, Data-only
 * mode, or both (instructions and data) with the L2_MODE definition.
 * The default is to allow caching of data only (L2_MODE_D_ONLY).
 * Valid settings also include: L2_MODE_I_ONLY and L2_MODE_INST_AND_DATA.
 *
 * (4) L2 Replacement Algorithm
 * The L2 cache replacement algorithm can be configured for Default replacement
 * algorithm or Secondary replacement algorithm.  The default is
 * to use the default replacement algorithm (L2_REP_ALG_DEFAULT).  To use
 * the secondary algorithm set L2_REP_ALG to L2_REP_ALG_SECONDARY.
 *
 * The following features of the L3 Cache can be configured (these are
 * relevant only if INCLUDE_CACHE_L3 is defined):
 *
 * (1) Mode
 * The L3 cache can be configured for Instruction-only mode, Data-only
 * mode, or both (instructions and data) with the L3_MODE definition.
 * The default is to allow caching of data only (L3_MODE_D_ONLY).
 * Valid settings also include: L3_MODE_I_ONLY and L3_MODE_INST_AND_DATA.
 *
 * (2) L3 Replacement Algorithm
 * The L3 cache replacement algorithm can be configured for Default replacement
 * algorithm or Secondary replacement algorithm.  The default is
 * to use the default replacement algorithm (L3_REP_ALG_DEFAULT).  To use
 * the secondary algorithm set L3_REP_ALG to L3_REP_ALG_SECONDARY.
 */

#define ENABLE_L2_DATA_PARITY
#define MSSCR0_L2_PREFETCH_ENGINES   MSSCR0_L2PFE_ONE
#define L2_MODE                      L2_MODE_D_ONLY
#define L2_REP_ALG                   L2_REP_ALG_DEFAULT
#define L3_MODE                      L3_MODE_D_ONLY
#define L3_REP_ALG                   L3_REP_ALG_DEFAULT

/*
 * Timestamp Support
 *
 * Timestamp option not included by default:  #define to include it
 */

#undef  INCLUDE_TIMESTAMP

/* Optional TrueFFS support */

#undef INCLUDE_TFFS

#ifdef INCLUDE_TFFS
#   define INCLUDE_TFFS_MOUNT
#   define INCLUDE_FS_EVENT_UTIL   /* File system event utility */
#   define INCLUDE_FS_MONITOR      /* File system monitor */
#   define INCLUDE_ERF             /* Event reporting framework */
#   define INCLUDE_DEVICE_MANAGER  /* Device Manager */
#   define INCLUDE_XBD             /* Extended block device */
#   define INCLUDE_XBD_TRANS
#   define INCLUDE_XBD_BLK_DEV     /* XBD-block device wrapper */
#   define INCLUDE_DOSFS_MAIN
#   define INCLUDE_DOSFS_FMT
#   define INCLUDE_DOSFS_FAT
#   define INCLUDE_DOSFS_DIR_VFAT
#   define INCLUDE_DOSFS_DIR_FIXED
#endif /* INCLUDE_TFFS */

/*
 * Network Support
 * Include the enhanced network driver.
 */

#define  INCLUDE_END /* Enhanced Network Driver (see configNet.h) */

/*
 * System Memory (DRAM) Support
 *
 * By default, the available DRAM memory is sized at bootup (LOCAL_MEM_AUTOSIZE
 * is defined).  If auto-sizing is not selected, make certain that
 * LOCAL_MEM_SIZE is set to the actual amount of memory on the board.
 * By default, it is set to the minimum memory configuration: 32 MB.
 * Failure to do so can cause unpredictable system behavior!
 */

#define LOCAL_MEM_AUTOSIZE                  /* undef for fixed size */
#define LOCAL_MEM_LOCAL_ADRS    0x00000000  /* fixed at zero */
#define LOCAL_MEM_SIZE          0x02000000  /* Default: Min memory: 32MB */

/* user reserved memory, see sysMemTop() */

#define USER_RESERVED_MEM   (0) /* number of reserved bytes */

/*
 * NOTE: the following are #define'd in two separate places.  Make sure
 * the definitions in both places agree:
 *
 *   ROM_TEXT_ADRS    #define'd in "config.h" and "Makefile".
 *   ROM_WARM_ADRS    #define'd in "config.h" and "Makefile".
 *   RAM_LOW_ADRS     #define'd in "config.h" and "Makefile".
 *   RAM_HIGH_ADRS    #define'd in "config.h" and "Makefile".
 *
 * ROM_WARM_ADRS is the boot ROM warm entry address which is set to 4 bytes
 * beyond the cold boot entry point ROM_TEXT_ADRS.
 */

#define ROM_TEXT_ADRS   (ROM_BASE_ADRS + 0x100)
#define ROM_WARM_ADRS   (ROM_TEXT_ADRS + 0x004)

#define ROM_SIZE        0x00100000      /* 1 Meg ROM space */

/* Shared memory support */

/*
 * INCLUDE_SM_NET and INCLUDE_SM_SEQ_ADDR are the shared memory backplane
 * driver and the auto address setup which are excluded in configAll.h.
 * To include them, uncomment the following lines:
 *
 * #define INCLUDE_SM_NET
 * #define INCLUDE_SM_SEQ_ADDR
 */

#if defined(INCLUDE_SM_NET)
#   define INCLUDE_SM_COMMON
#   define INCLUDE_SM_OBJ 
#   define INCLUDE_SM_COMMON
#   define INCLUDE_BSD
#   define INCLUDE_SM_NET_SHOW
#   define INCLUDE_NET_DRV
#   define INCLUDE_NET_LIB
#   define INCLUDE_SMEND 
#   undef  PCI_DMA_SNOOP_BUS_0_ON
#   undef  IPFORWARDING_CFG
#   define IPFORWARDING_CFG  TRUE /* ipForwardCfg is TRUE or FALSE depending */
                                  /* on whether the stack was built as a host */
                                  /* or a router. Replace ipForwardCfg with */ 
                                  /* TRUE or FALSE to change the default */
                                  /* configuration. */
#   undef  WDB_COMM_TYPE   
#   define WDB_COMM_TYPE    WDB_COMM_END 
                            /* on whether the stack was built as a host */
                            /* or a router. Replace ipForwardCfg with */
                            /* TRUE or FALSE to change the default */
                            /* configuration. */
#   undef  WDB_COMM_TYPE   
#   define WDB_COMM_TYPE    WDB_COMM_END
#endif /* defined(INCLUDE_SM_NET) */

/*
 * Changing SM_OFF_BOARD to TRUE also requires changing
 * SM_ANCHOR_ADRS and SM_MEM_ADRS appropriately.
 */

#define SM_OFF_BOARD    FALSE

#undef  SM_ANCHOR_ADRS
#define SM_ANCHOR_ADRS  ((sysProcNumGet() == 0) ? \
    ((char*) (LOCAL_MEM_LOCAL_ADRS + SM_ANCHOR_OFFSET)) : \
    ((char*) (VME_A32_MSTR_LOCAL + SM_ANCHOR_OFFSET)))

#define SM_INT_TYPE             SM_INT_MAILBOX_1         /* or SM_INT_NONE */

/* The following must be #define'd the same on all nodes. */

#define SM_MEM_ADRS             0x00010000      /* Start of actual SM region */
#define SM_MEM_SIZE             0x00020000
#define SM_OBJ_MEM_ADRS         (SM_MEM_ADRS+SM_MEM_SIZE) /* SM Objects pool */
#define SM_OBJ_MEM_SIZE         0x00010000

/* BSP-specific includes */

/*
 * Main cause interrupt priority table defines the order in which bits
 * in the two "interrupt main cause" registers (low and high) are checked
 * when an interrupt occurs.  The table is organized with paired-entries.
 * The first component of the pair is the low or high cause register
 * designation, the second component of the pair is the bit number within
 * that register.  The ordering of the table specifies the order in which
 * bits are checked in the cause registers when an interrupt occurs.
 * Entries at the beginning of the table are checked first so to give an
 * interrupt cause bit a higher software priority, put it near the beginning
 * of the list.
 */

#define ICI_MAIN_INT_PRIORITIES           \
        ICI_MICL_INT_NUM_0,  ICI_MICL_INT_NUM_1,  \
        ICI_MICL_INT_NUM_2,  ICI_MICL_INT_NUM_3,  \
        ICI_MICL_INT_NUM_4,  ICI_MICL_INT_NUM_5,  \
        ICI_MICL_INT_NUM_6,  ICI_MICL_INT_NUM_7,  \
        ICI_MICL_INT_NUM_8,  ICI_MICL_INT_NUM_9,  \
        ICI_MICL_INT_NUM_10, ICI_MICL_INT_NUM_11, \
        ICI_MICL_INT_NUM_12, ICI_MICL_INT_NUM_13, \
        ICI_MICL_INT_NUM_14, ICI_MICL_INT_NUM_15, \
        ICI_MICL_INT_NUM_16, ICI_MICL_INT_NUM_17, \
        ICI_MICL_INT_NUM_18, ICI_MICL_INT_NUM_19, \
        ICI_MICL_INT_NUM_20, ICI_MICL_INT_NUM_21, \
        ICI_MICL_INT_NUM_22, ICI_MICL_INT_NUM_23, \
        ICI_MICL_INT_NUM_24, ICI_MICL_INT_NUM_25, \
        ICI_MICL_INT_NUM_26, ICI_MICL_INT_NUM_27, \
        ICI_MICL_INT_NUM_28, ICI_MICL_INT_NUM_29, \
        ICI_MICL_INT_NUM_30, ICI_MICL_INT_NUM_31, \
        ICI_MICH_INT_NUM_0,  ICI_MICH_INT_NUM_1,  \
        ICI_MICH_INT_NUM_2,  ICI_MICH_INT_NUM_3,  \
        ICI_MICH_INT_NUM_4,  ICI_MICH_INT_NUM_5,  \
        ICI_MICH_INT_NUM_6,  ICI_MICH_INT_NUM_7,  \
        ICI_MICH_INT_NUM_8,  ICI_MICH_INT_NUM_9,  \
        ICI_MICH_INT_NUM_10, ICI_MICH_INT_NUM_11, \
        ICI_MICH_INT_NUM_12, ICI_MICH_INT_NUM_13, \
        ICI_MICH_INT_NUM_14, ICI_MICH_INT_NUM_15, \
        ICI_MICH_INT_NUM_16, ICI_MICH_INT_NUM_17, \
        ICI_MICH_INT_NUM_18, ICI_MICH_INT_NUM_19, \
        ICI_MICH_INT_NUM_20, ICI_MICH_INT_NUM_21, \
        ICI_MICH_INT_NUM_22, ICI_MICH_INT_NUM_23, \
        ICI_MICH_INT_NUM_24, ICI_MICH_INT_NUM_25, \
        ICI_MICH_INT_NUM_26, ICI_MICH_INT_NUM_27, \
        ICI_MICH_INT_NUM_28, ICI_MICH_INT_NUM_29, \
        ICI_MICH_INT_NUM_30, ICI_MICH_INT_NUM_31, \
        0xff
/*
 * In a similar manner to the "main interrupt priorities", below
 * is a list representing the GPP interrupt priorities.  Each element in the
 * list is a GPP interrupt number.  Each number represents a bit
 * position in the GPP interrupt cause register.  When a GPP interrupt
 * occurs (via bits 24, 25, or 26 of the main interrupt cause register),
 * the GPP interrupt handler will examine the GPP local cause bits in
 * the order specified by the following list.  By rearranging the
 * ordering of bits below the priority in which GPP interrupts are
 * checked can be modified.
 */

#define GPP_LOCAL_INT_PRIORITIES \
        0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7, \
        0x8,0x9,0xa,0xb,0xc,0xd,0xe,0xf, \
        0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17, \
        0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f, \
        0xff        /* end of list */

#include "mv5500A.h"    /* board specific definitions */

#ifdef __cplusplus
    }
#endif /* __cplusplus */

#if defined(PRJ_BUILD)
    #include "prjParams.h"
#endif /* PRJ_BUILD */

#include "mv5500B.h"    /* definition dependencies */

#endif  /* __INCconfigh */

