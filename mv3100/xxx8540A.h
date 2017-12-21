/* xxx8540A.h - Motorola xxx8540 board header file */

/*
 * Copyright (c) 2004-2005, 2007, 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 2004-2005, Motorola Inc. All Rights Reserved */

/*
modification history
--------------------
01s,24apr09,z_l Redefined PCI_IO_BUS_START.
01r,27dec07,y_w Fixed VME address mapping problem (WIND00106506)
01q,25oct05,cak Added ISA_LEGACY_SIZE definition.
01p,01sep05,efb Removed unused decrementer clock define.
01o,01jul05,cak Tempe intEnable and intDisable support.
01n,03jun05,pjh Added Quad UART Support.
01m,23may05,efb Added sATA support.
01l,26apr05,cak Moved the definition for the default core complex bus 
                speed from sysMotI2c.c.
01k,31jan05,cak VPD Support.
01j,25jan05,cak Shared memory support. 
01i,06jan05,cak Moved some clock speed and timer definitions here
                from config.h. 
01h,17dec04,cak Added Tempe (Tsi148) support.
01g,17dec04,cak Added Tempe (Tsi148) support.
01f,10dec04,cak PCI/PCI-X Bus Interface support. 
01e,24nov04,cak Added MPC8540_L2_SRAM_BASE definition and removed 
                unneeded DDR SDRAM register offset definitions.
01d,22nov04,scb Added WHAT_2POWER macro.
01c,18nov04,cak Added ARG_UNUSED macro to remove compiler error.
01b,11oct04,scb Move CCSBAR from 0xfe000000 to 0xe1000000.
01a,25aug04,scb Initial writing.
*/

/*
This file contains macros and defines for I/O addresses, e500
core functions and other utility data.  Near the end of this
file, specific #include's are coded to load definitions unique
to one of the three specific boards suppored by this BSP: The
XMC module, SBC (single board computer), and the PrPMC.
*/

#ifndef __INCxxx8540Ah
#define __INCxxx8540Ah

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

/* includes */

#include "tempe.h"
#include "ppcProc.h"

/* sATA/IDE Support */

#ifdef INCLUDE_ATA
#   include "drv/pcmcia/pccardLib.h"
#   include "drv/hdisk/ataDrv.h"
#endif  /* INCLUDE_ATA */

/* Define Clock Speed and source  */

#define FREQ_33_MHZ      33333333
#define FREQ_66_MHZ      66666666
#define FREQ_100_MHZ     99999999
#define FREQ_133_MHZ    133333333
#define FREQ_266_MHZ    266666666
#define	FREQ_333_MHZ	333333333

#define WDT_RATE_MIN         (sysTimerClkFreq / (1 << 29))
#define WDT_RATE_MAX         (sysTimerClkFreq / (1 << 21))

#define DEFAULT_CCB_SPD        (FREQ_333_MHZ)

/*
 * Compiler Warnings:
 *
 * The following is provided to remove compiler warnings when using the
 * compiler flags:
 * -pedantic
 * -Wall
 * -W
 * -Wmissing-declarations
 * -Wstrict-prototypes
 * -Wmissing-prototypes
 *
 * These compiler warnings were due to the fact that "BSP implemented"
 * routines do not always use all of the function arguments specified
 * in the function declarations contained within vxWorks header files.
 */

#if (defined __GNUC__)
#   define ARG_UNUSED __attribute__((unused))
#else
#   define ARG_UNUSED
#endif

/* Board type descriminator */

#define MPC8540_BOARD_TYPE_XMC		(1)
#define MPC8540_BOARD_TYPE_SBC  	(2)
#define MPC8540_BOARD_TYPE_PRPMC 	(3)

/* BSP configuration error policy */

#define CONTINUE_EXECUTION      0       /* Tolerate VPD/Configuration errors */
#define EXIT_TO_SYSTEM_MONITOR  1       /* Transfer to System Monitor */

/* VPD Support */

#define DEFAULT_PRODUCT_ID	"Unknown"
#define DEFAULT_CPU_ID		"Unknown"
#define VPD_BRD_OFFSET		0   /* offset into brd's eeprom for vpd data */
#define VPD_PKT_LIMIT		25  /* Max number of packets expected */
#define VPD_EEPROM_SIZE		512 /* size of VPD serial EEPROM */
#define VPD_BYTE_ADDR_SIZE	2

/* L2 SRAM Base Address */

#define MPC8540_L2_SRAM_BASE            0xE1100000

/* 
 * TBD:
 * The following #defines were leftovers from the "ads85xx.h" file.
 * We should clean this section up later.
 */

#ifdef  _ASMLANGUAGE
#define CAST(x)
#else /* _ASMLANGUAGE */
typedef volatile UCHAR  VCHAR;   /* shorthand for volatile UCHAR */
typedef volatile UINT32 VUINT32; /* volatile unsigned word */
typedef volatile UINT16 VUINT16; /* volatile unsigned halfword */
typedef volatile UINT8  VUINT8;  /* volatile unsigned byte */
#define CAST(x) (x)
#endif  /* _ASMLANGUAGE */

/* Serial Support */

#ifdef INCLUDE_QUAD_UART
#   define N_SIO_CHANNELS	5       /* No. serial I/O channels */
#else
#   define N_SIO_CHANNELS	1       /* No. serial I/O channels */
#endif /* INCLUDE_QUAD_UART */

/* Translation macro */

#define TRANSLATE(x,y,z)\
        ((UINT)(x) - (UINT)(y) + (UINT)(z))

/* Base Address of Memory Mapped Registers */

#define CCSBAR		0xe1000000
#define CCSBAR_BUS	(CCSBAR)

#define IS_DRAM_ADDRESS(addr) (((int)addr >= LOCAL_MEM_LOCAL_ADRS) && \
                               ((UINT32)addr < (UINT32)sysPhysMemTop()))

/* VME related */

#define VME_MEM_LOCAL_START	0x80000000
#define VME_MEM_BUS_START	(VME_MEM_LOCAL_START)

#define VME_A32_MSTR_LOCAL	VME_MEM_LOCAL_START
#define VME_A32_MSTR_BUS	(0x08000000)
#define VME_A32_MSTR_END	(VME_A32_MSTR_LOCAL + VME_A32_MSTR_SIZE)

#define VME_A24_MSTR_LOCAL	(VME_A32_MSTR_END)
#define VME_A24_MSTR_BUS	(0x00000000)
#define VME_A24_MSTR_END	(VME_A24_MSTR_LOCAL + VME_A24_MSTR_SIZE)

#define VME_A16_MSTR_LOCAL	(VME_A24_MSTR_END)
#define VME_A16_MSTR_BUS	(0x00000000)
#define VME_A16_MSTR_END	(VME_A16_MSTR_LOCAL + VME_A16_MSTR_SIZE)

/* Make sure VME_LOCAL_MEM_END is rounded to nearest 0x0001000 boundary */

#define VME_MEM_LOCAL_END	(VME_A16_MSTR_END)

/* 
 * VME_MEM_SIZE defines the maximum extent of the VME space.  It must
 * be greater than or equal to the ranged defined by VME_MEM_LOCAL_START
 * and VME_MEM_LOCAL_END.  We can increase VME_A32 space by increasing
 * VME_A32_MSTR_SIZE for example and this will push up the value of 
 * VME_MEM_LOCAL_END but we better not define more space in this extent
 * than is represented by VME_MEM_SIZE.  The space defined by VME_MEM_SIZE
 * will be encoded into a MPC8540 Outbound Window and thus the 
 * additional constraint on VME_MEM_SIZE is that it must be a power of 2. 
 */

#define VME_MEM_SIZE 		(0x20000000)	/* Must be power of 2 */

#define IS_VME_ADDR_MOD(a) ((a == VME_AM_EXT_SUP_PGM) || \
                            (a == VME_AM_EXT_SUP_DATA) || \
                            (a == VME_AM_EXT_USR_PGM) || \
                            (a == VME_AM_EXT_USR_DATA) || \
                            (a == VME_AM_STD_SUP_PGM) || \
                            (a == VME_AM_STD_SUP_DATA) || \
                            (a == VME_AM_STD_USR_PGM) || \
                            (a == VME_AM_STD_USR_DATA) || \
                            (a == VME_AM_SUP_SHORT_IO) || \
                            (a == VME_AM_USR_SHORT_IO))

/* 
 * Note that VME_CRG_SLV_SIZE is the size of my CRG register group.
 * This group will be mapped onto the VME bus.  VME_CRG_MSTR_SIZE is
 * the size of 16 consecutive CRG groups.  This size will be used to
 * map an outbound window so we can have access to up to 16 different
 * boards's CRG groups (and hence their mailboxes) - allowing VxWorks
 * shared memory to work with mailbox interrupts.
 */

#define VME_CRG_SLV_SIZE        (0x1000)
#define VME_CRG_MSTR_SIZE       (16 * VME_CRG_SLV_SIZE)
#define VME_CRG_MSTR_LOCAL      (VME_A32_MSTR_LOCAL + \
				 VME_A32_MSTR_SIZE - \
				 VME_CRG_MSTR_SIZE)
#define VME_CRG_MSTR_BUS        (0xfb000000)
#define VME_MBOX0_OFFSET	(TEMPE_GCSR_MBOX0 + 3)

/*
 * Allocation of VME space to outbound windows, first set up the out0
 * window to point to the CRG space to allow access to (among other
 * things) other board's mailboxes - necessary for VxWorks shared memory.
 */

#define VME_OUT0_START		(VME_CRG_MSTR_LOCAL)
#define VME_OUT0_SIZE		(VME_CRG_MSTR_SIZE)
#define VME_OUT0_BUS            (VME_CRG_MSTR_BUS)

/* Now out1 will point to some node zero system DRAM */

#define VME_DRAM_ACCESS_SIZE	(0x00100000)
#define VME_OUT1_START		(VME_A32_MSTR_LOCAL)
#define VME_OUT1_SIZE		(VME_DRAM_ACCESS_SIZE)
#define VME_OUT1_BUS            (VME_A32_MSTR_BUS)

/* Finish up with A24 space (out2) and A16 space (out3) */

#define VME_OUT2_START          (VME_A24_MSTR_LOCAL)
#define VME_OUT2_SIZE           (VME_A24_MSTR_SIZE)
#define VME_OUT2_BUS            (VME_A24_MSTR_BUS)

#define VME_OUT3_START          (VME_A16_MSTR_LOCAL)
#define VME_OUT3_SIZE           (VME_A16_MSTR_SIZE)
#define VME_OUT3_BUS            (VME_A16_MSTR_BUS)

#define VME_OUT0_CFG_PARAMS \
                TRUE,                   /* Window enabled */ \
                0, VME_OUT0_START,      /* Local start addrs (upper = 0) */ \
                0, VME_OUT0_SIZE,       /* Size (upper = 0) */ \
                0, VME_OUT0_BUS,        /* VME bus addr (upper = 0) */ \
                0,                      /* 2eSST broadcast select */ \
                0,                      /* Unused */ \
                FALSE,                  /* Read prefetch enable state */ \
                VME_RD_PREFETCH_2_CACHE_LINES, \
                VME_SST160,             /* 2esst xfer rate */ \
                VME_SCT_OUT,            /* transfer mode */ \
                VME_D32,                /* VME data bus width */ \
                FALSE,                  /* nonsupervisor access */ \
                FALSE,                  /* Not pgm but instead data access */ \
                VME_MODE_A32            /* transfer mode */

#define VME_OUT1_CFG_PARAMS \
                TRUE,                   /* Window enabled */ \
                0, VME_OUT1_START,      /* Local start addrs (upper = 0) */ \
                0, VME_OUT1_SIZE,       /* Size (upper = 0) */ \
                0, VME_OUT1_BUS,        /* VME bus addr (upper = 0) */ \
                0,                      /* 2eSST broadcast select */ \
                0,                      /* Unused */ \
                TRUE,                   /* Read prefetch enable state */ \
                VME_RD_PREFETCH_2_CACHE_LINES, \
                VME_SST160,             /* 2esst xfer rate */ \
                VME_MBLT_OUT,           /* transfer mode */ \
                VME_D32,                /* VME data bus width */ \
                TRUE,                   /* supervisor access */ \
                FALSE,                  /* Not pgm but instead data access */ \
                VME_MODE_A32            /* transfer mode */

#define VME_OUT2_CFG_PARAMS \
                TRUE,                   /* Window enabled */ \
                0, VME_OUT2_START,      /* Local start addrs (upper = 0) */ \
                0, VME_OUT2_SIZE,       /* Size (upper = 0) */ \
                0, VME_OUT2_BUS,        /* VME bus addr (upper = 0) */ \
                0,                      /* 2eSST broadcast select */ \
                0,                      /* Unused */ \
                TRUE,                   /* Read prefetch enable state */ \
                VME_RD_PREFETCH_2_CACHE_LINES, \
                VME_SST160,             /* 2esst xfer rate */ \
                VME_MBLT_OUT,           /* transfer mode */ \
                VME_D32,                /* VME data bus width */ \
                FALSE,                  /* nonsupervisor access */ \
                FALSE,                  /* Not pgm but instead data access */ \
                VME_MODE_A24            /* transfer mode */\

#define VME_OUT3_CFG_PARAMS \
                TRUE,                   /* Window enabled */ \
                0, VME_OUT3_START,      /* Local start addrs (upper = 0) */ \
                0, VME_OUT3_SIZE,       /* Size (upper = 0) */ \
                0, VME_OUT3_BUS,        /* VME bus addr (upper = 0) */ \
                0,                      /* 2eSST broadcast select */ \
                0,                      /* Unused */ \
                TRUE,                   /* Read prefetch enable state */ \
                VME_RD_PREFETCH_2_CACHE_LINES, \
                VME_SST160,             /* 2esst xfer rate */ \
                VME_MBLT_OUT,           /* transfer mode */ \
                VME_D32,                /* VME data bus width */ \
                FALSE,                  /* nonsupervisor access */ \
                FALSE,                  /* Not pgm but instead data access */ \
                VME_MODE_A16            /* transfer mode */

#define VME_OUT4_CFG_PARAMS     VME_OUTx_CFG_DISABLED
#define VME_OUT5_CFG_PARAMS     VME_OUTx_CFG_DISABLED
#define VME_OUT6_CFG_PARAMS     VME_OUTx_CFG_DISABLED
#define VME_OUT7_CFG_PARAMS     VME_OUTx_CFG_DISABLED

#define VME_OUT_WINDOW_CFG_PARAMS \
                VME_OUT0_CFG_PARAMS, \
                VME_OUT1_CFG_PARAMS, \
                VME_OUT2_CFG_PARAMS, \
                VME_OUT3_CFG_PARAMS, \
                VME_OUT4_CFG_PARAMS, \
                VME_OUT5_CFG_PARAMS, \
                VME_OUT6_CFG_PARAMS, \
                VME_OUT7_CFG_PARAMS

#define VME_IN0_CFG_PARAMS      VME_INx_CFG_DISABLED
#define VME_IN1_CFG_PARAMS      VME_INx_CFG_DISABLED
#define VME_IN2_CFG_PARAMS      VME_INx_CFG_DISABLED
#define VME_IN3_CFG_PARAMS      VME_INx_CFG_DISABLED
#define VME_IN4_CFG_PARAMS      VME_INx_CFG_DISABLED
#define VME_IN5_CFG_PARAMS      VME_INx_CFG_DISABLED
#define VME_IN6_CFG_PARAMS      VME_INx_CFG_DISABLED
#define VME_IN7_CFG_PARAMS      VME_INx_CFG_DISABLED

/* 
 * Make shared memory adjustment for inbound window mapping.  This only
 * applies if SM_OFF_BOARD is set to FALSE, meaning that we are the node
 * which is hosting the shared memory pool.  In that case, we must make
 * sure that we have an inbound window mapped to allow access to our
 * shared memory pool from the VME bus, instead of having no inbound
 * windows programmed as would normally be the case if SM_OFF_BOARD
 * is set to FALSE.
 */

#if (SM_OFF_BOARD == FALSE)
#   undef  VME_IN0_CFG_PARAMS
#   define VME_IN0_CFG_PARAMS            \
                TRUE,                    \
                0,                       \
                VME_A32_MSTR_BUS,        \
                0,                       \
                (SM_MEM_ADRS +           \
                 SM_MEM_SIZE +           \
                 SM_OBJ_MEM_SIZE),       \
                0,                       \
                LOCAL_MEM_LOCAL_ADRS,    \
                VME_RD_THRESHOLD_EMPTY,  \
                VME_VFS_FIFO_64,         \
                VME_SST160,              \
                (VME_BLT_IN |            \
                 VME_MBLT_IN |           \
                 VME_2eVME_IN |          \
                 VME_2eSST_IN |          \
                 VME_2eSST_BCAST_IN ),   \
                 VME_SPACE_A32,          \
                 TRUE,                   \
                 TRUE,                   \
                 TRUE,                   \
                 TRUE,                   \
                 0
#endif


#   undef  VME_IN1_CFG_PARAMS
#   define VME_IN1_CFG_PARAMS            \
				TRUE,                    \
				0,                       \
				VME_A32_MSTR_BUS + 0x100000,        \
				0,                       \
				(SM_MEM_ADRS +           \
				 SM_MEM_SIZE +           \
				 SM_OBJ_MEM_SIZE),       \
				0,                       \
				LOCAL_MEM_LOCAL_ADRS + 0x100000,    \
				VME_RD_THRESHOLD_EMPTY,  \
				VME_VFS_FIFO_64,         \
				VME_SST160,              \
				(VME_BLT_IN |            \
				 VME_MBLT_IN |           \
				 VME_2eVME_IN |          \
				 VME_2eSST_IN |          \
				 VME_2eSST_BCAST_IN ),   \
				 VME_SPACE_A32,          \
				 TRUE,                   \
				 TRUE,                   \
				 TRUE,                   \
				 TRUE,                   \
				 0

/* 
 * Tempe CRG access values - these define a 64-bit access address and
 * attribute bit field for accessing my Tempe CSR register set from the
 * VME bus.  We're only dealing with a 32-bit address here so the "upper"
 * register (TEMPE_CBAU_VAL) is zero, the "lower" register (TEMPE_CBAL_VAL) 
 * contains the 3 high-order significant digits of the 32-bit address.  The 
 * "attribute" register (TEMPE_CSRAT_VAL) contains the attribute bits which
 * allow for the following accesses to this space: data, program, 
 * nonpriviledged, supervisor, A32 - and "enabled".
 */

#define TEMPE_CBAU_VAL	0		/* Upper 64-bits zero */
#define TEMPE_CBAL_VAL  (VME_CRG_MSTR_BUS+(sysProcNumGet()*VME_CRG_SLV_SIZE))
#define TEMPE_CSRAT_VAL 0x00000AF     /* Enable for A32 and all other access */

/* 64-bit address support (used on Tempe and maybe other places as well) */

#define MAKE64(high32,low32) (((UINT64)high32<<32)+((UINT64)low32))
#define UNMAKE64(val64,high32,low32) { high32 = (val64>>32) & 0xffffffff; \
				       low32  = (val64) & 0xffffffff; }

/* Define Tempe interrupt numbers (assiciated with intConnect()) */

#define TEMPE_INT_NUM_MIN	(0x60)

#define TEMPE_INT_NUM_IRQ1      (TEMPE_INT_NUM_MIN + 0)
#define TEMPE_INT_NUM_IRQ2      (TEMPE_INT_NUM_MIN + 1)
#define TEMPE_INT_NUM_IRQ3      (TEMPE_INT_NUM_MIN + 2)
#define TEMPE_INT_NUM_IRQ4      (TEMPE_INT_NUM_MIN + 3)
#define TEMPE_INT_NUM_IRQ5      (TEMPE_INT_NUM_MIN + 4)
#define TEMPE_INT_NUM_IRQ6      (TEMPE_INT_NUM_MIN + 5)
#define TEMPE_INT_NUM_IRQ7      (TEMPE_INT_NUM_MIN + 6)
#define TEMPE_INT_NUM_ACFL      (TEMPE_INT_NUM_MIN + 7)
#define TEMPE_INT_NUM_SYSFL     (TEMPE_INT_NUM_MIN + 8)
#define TEMPE_INT_NUM_IACK      (TEMPE_INT_NUM_MIN + 9)
#define TEMPE_INT_NUM_VIEEN     (TEMPE_INT_NUM_MIN + 10)
#define TEMPE_INT_NUM_VERR      (TEMPE_INT_NUM_MIN + 11)
#define TEMPE_INT_NUM_PERR      (TEMPE_INT_NUM_MIN + 12)
#define TEMPE_INT_NUM_MB0       (TEMPE_INT_NUM_MIN + 13)
#define TEMPE_INT_NUM_MB1       (TEMPE_INT_NUM_MIN + 14)
#define TEMPE_INT_NUM_MB2       (TEMPE_INT_NUM_MIN + 15)
#define TEMPE_INT_NUM_MB3       (TEMPE_INT_NUM_MIN + 16)
#define TEMPE_INT_NUM_LM0       (TEMPE_INT_NUM_MIN + 17)
#define TEMPE_INT_NUM_LM1       (TEMPE_INT_NUM_MIN + 18)
#define TEMPE_INT_NUM_LM2       (TEMPE_INT_NUM_MIN + 19)
#define TEMPE_INT_NUM_LM3       (TEMPE_INT_NUM_MIN + 20)
#define TEMPE_INT_NUM_DMA0      (TEMPE_INT_NUM_MIN + 21)
#define TEMPE_INT_NUM_DMA1      (TEMPE_INT_NUM_MIN + 22)

#define TEMPE_INT_NUM_MAX       (TEMPE_INT_NUM_DMA1)

/*
 * Define the entry into INT_UTIL_DESC intUtil [] which allows us
 * to hook Tempe (Tsi148) interrupt management and ISA interrupt management
 * into the MPC8540 interrupt handler.  The INT_USR_EXTRA component
 * can be initialized inside of "config.h" to refer to any special
 * intEnable()/intDisable() code that the user wants to invoke for
 * the user reserved interrupt vector numbers.
 */

#ifndef _ASMLANGUAGE
    STATUS sysTempeIntEnable (int intLevel);
    STATUS sysTempeIntDisable (int intLevel);
#endif

#define INT_UTIL_EXTRA  ,{TEMPE_INT_NUM_MIN,TEMPE_INT_NUM_MAX, \
                          sysTempeIntEnable,sysTempeIntDisable} \
                          INT_USR_EXTRA

/*
 * Define the minimum and maximum interrupt vector numbers available for user
 * applications.  Note: See INT_USR_EXTRA in "config.h".
 */

#define USR_INT_NUM_MIN         0xc0
#define USR_INT_NUM_MAX         0xff

/* Support for VME bus locking */

#define LOCK_TIMEOUT		10
#define DECREMENTER_PRESCALE	4
#define USECS_PER_SECOND	1000000

/* Support for sysBusTas() RMW cycle */

#define	VME_RMW_ENABLE_BITS		0x80000000
#define VME_RMW_COMPARE_BITS		0x00000000
#define VME_RMW_SWAP_TO_SET		0x80000000

/* PCI related */

#define INCLUDE_PCI             /* always include pci */
#define INCLUDE_PCI_AUTOCONF    /* always include pci auto config*/

/*
 * Support for determining if we're ROM based or not.  _sysInit
 * saves the startType parameter at location ROM_BASED_FLAG.
 */

#define PCI_AUTOCONFIG_FLAG_OFFSET (0x4c00)
#define PCI_AUTOCONFIG_FLAG        (*(UCHAR *)(LOCAL_MEM_LOCAL_ADRS + \
                                     PCI_AUTOCONFIG_FLAG_OFFSET))
#define PCI_AUTOCONFIG_DONE        (PCI_AUTOCONFIG_FLAG != 0)

/* Define the host bridge device number */

#define PCI_HOST_BRIDGE_BUS_NUM         (0)
#define PCI_HOST_BRIDGE_DEV_NUM         (0)
#define PCI_HOST_BRIDGE_FUNC_NUM	(0)

/* PCI header rleated */

#define PCI_CLINE_SZ    (_CACHE_ALIGN_SIZE/4)   /* Cache Ln Size = 32 bytes */
#define PCI_LAT_TIMER   0xff    /* Latency Timer Value = 255 PCI clocks */

/*
 * Define the PCI regions:
 * ...MEMIO...		Nonprefetchable memory
 * ...MEM...		Prefetchable memory
 * ISA...		16-bit I/O
 * ...IO...		32-bit I/O
 */

/* Local (view from cpu) base addresses of PCI regions */

#define PCI_MEM_LOCAL_START	(0xa0000000)

#define PCI_MSTR_MEMIO_LOCAL	(PCI_MEM_LOCAL_START)
#define PCI_MEMIO_LOCAL_END	(PCI_MSTR_MEMIO_LOCAL + PCI_MSTR_MEMIO_SIZE)

#define PCI_MSTR_MEM_LOCAL	(PCI_MEMIO_LOCAL_END)
#define PCI_MEM_LOCAL_END	(PCI_MSTR_MEM_LOCAL + PCI_MSTR_MEM_SIZE)

#define PCI_IO_LOCAL_START	(PCI_MEM_LOCAL_END)

#define ISA_MSTR_IO_LOCAL	(PCI_IO_LOCAL_START)
#define ISA_IO_LOCAL_END	(ISA_MSTR_IO_LOCAL + ISA_MSTR_IO_SIZE)

#define PCI_MSTR_IO_LOCAL	(ISA_IO_LOCAL_END)
#define PCI_IO_LOCAL_END	(PCI_MSTR_IO_LOCAL + PCI_MSTR_IO_SIZE)

#define PCI_IO_SIZE		(ISA_MSTR_IO_SIZE + PCI_MSTR_IO_SIZE)

#define PCI_MEM_LOCAL_MAX	(0xe0000000 - 1)	/* Max top of PCI */

/* Bus (view from PCI bus) base addresses of PCI regions */

#define PCI_MEM_BUS_START		(PCI_MSTR_MEMIO_LOCAL)
#define PCI_MSTR_MEMIO_BUS		(PCI_MEM_BUS_START)
#define PCI_MSTR_MEM_BUS		(PCI_MSTR_MEM_LOCAL)

#define PCI_IO_BUS_START		(PCI_MSTR_IO_LOCAL)
#define ISA_MSTR_IO_BUS			(PCI_IO_BUS_START)
#define PCI_MSTR_IO_BUS			(TRANSLATE(PCI_MSTR_IO_LOCAL,\
					 ISA_MSTR_IO_LOCAL,\
					 ISA_MSTR_IO_BUS))

#define ISA_LEGACY_SIZE          0x00004000

/*
 * Compiler Errors:
 *
 * Signal a compile time error if:
 *
 * 1. The VME local memory space extends beyond the beginning of the PCI
 *    memory space.
 *
 * 2. The PCI local I/O space extends beyond the beginning of the
 *    MPC8540 internal register space.
 */

#if (VME_MEM_LOCAL_END > PCI_MEM_LOCAL_START)
# error local VME memory space extends into local PCI memory space
#endif

#if (PCI_IO_LOCAL_END > CCSBAR)
# error PCI I/O space extends beyond MPC8540 internal register base
#endif

/* Common I/O synchronizing instructions */

#ifndef EIEIO_SYNC
#   define EIEIO_SYNC  WRS_ASM("eieio;sync")
#endif  /* EIEIO_SYNC */

#ifndef EIEIO
#   define EIEIO   WRS_ASM("eieio")
#endif  /* EIEIO */

#ifndef SYNC
#   define SYNC   WRS_ASM("sync")
#endif  /* SYNC */

/*
 * BITVAL returns the value (1 or 0) of a bit at a particular postion 
 * within a word. 
 */

#define BITVAL(word,bitpos)  ((word >> bitpos) & 1)

/*
 * WHAT_2POWER returns a weighted sum of the bit values within a 32-bit
 * word.  The weighting factor is the bit position within the word, with
 * "positions" being counted, starting at 0, with the lowest significant
 * bit.  It is called WHAT_2POWER because when given a number which has
 * only one bit set (and is thus a power of 2), it returns the exponent
 * value which must be applied to 2 in order to produce the number.
 * e.g WHAT_2POWER(0x00800000) returns a computed value of 23 since
 * 2^23 = 0x00800000.  This macro can be applied to numbers other than
 * powers of two but I can see no useful result that would be produced
 * with such use.
 */

#define WHAT_2POWER(x)  (\
        (BITVAL(x,0)*0) + (BITVAL(x,1)*1) + (BITVAL(x,2)*2) + \
        (BITVAL(x,3)*3) + (BITVAL(x,4)*4) + (BITVAL(x,5)*5) + \
        (BITVAL(x,6)*6) + (BITVAL(x,7)*7) + (BITVAL(x,8)*8) + \
        (BITVAL(x,9)*9) + (BITVAL(x,10)*10) + (BITVAL(x,11)*11) + \
        (BITVAL(x,12)*12) + (BITVAL(x,13)*13) + (BITVAL(x,14)*14) + \
        (BITVAL(x,15)*15) + (BITVAL(x,16)*16) + (BITVAL(x,17)*17) + \
        (BITVAL(x,18)*18) + (BITVAL(x,19)*19) + (BITVAL(x,20)*20) + \
        (BITVAL(x,21)*21) + (BITVAL(x,22)*22) + (BITVAL(x,23)*23) + \
        (BITVAL(x,24)*24) + (BITVAL(x,25)*25) + (BITVAL(x,26)*26) + \
        (BITVAL(x,27)*27) + (BITVAL(x,28)*28) + (BITVAL(x,29)*29) + \
        (BITVAL(x,30)*30) + (BITVAL(x,31)*31) \
                        )

/*
 * Shared memory related parameters and
 * Software Test and Set Support:
 *
 * Note: This BSP requires a modified software Test and Set algorithm.
 * SM_TAS_TYPE is set to SM_TAS_HARD despite the lack of a hardware TAS
 * mechanism to force the use of a BSP-specific software TAS algorithm.
 * Shared Memory Interrupt Type.
 * Interrupt this target with a 1-byte write mailbox.
 * VME_A32 space, address based on procNum, value is SIG1_INTR_SET.
 */

#undef SM_TAS_TYPE
#define SM_TAS_TYPE     SM_TAS_HARD

#define SM_INT_ARG1     VME_AM_EXT_SUP_DATA
#define SM_INT_ARG2     (VME_CRG_MSTR_BUS + VME_MBOX0_OFFSET + \
                         (sysProcNumGet() * VME_CRG_SLV_SIZE))
#define SIG1_INTR_SET   0x80    /* Shared mem Mailbox interrupt "write" value */
#define SM_INT_ARG3     SIG1_INTR_SET

#define SM_BASE         (LOCAL_MEM_LOCAL_ADRS + SM_MEM_ADRS)
#define SM_LIMIT        (SM_BASE + SM_MEM_SIZE)

/*
 * This BSP covers three separate board types, the XMC, SBC (Single
 * board computer), and the PrPMC.  Here is where we selectively
 * include the proper header file for board specific definitions as
 * they applie to one of the above supported boards.
 */

#if (MPC8540_BOARD_TYPE == MPC8540_BOARD_TYPE_XMC)
#   include "xmc8540A.h"
#elif (MPC8540_BOARD_TYPE == MPC8540_BOARD_TYPE_SBC)
#   include "sbc8540A.h"
#elif (MPC8540_BOARD_TYPE == MPC8540_BOARD_TYPE_PRPMC)
# 

/* TBD - prpmc8540A.h included here*/

#endif

#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif /* __INCxxx8540Ah */
