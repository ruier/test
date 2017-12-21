/* mv7100.h - Motorola MVME7100 board header file */

/*
 * Copyright (c) 2008-2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01i,05may11,swu set VME_IN0_CFG_PARAMS as disabled by default. (WIND00261050)
01h,26apr10,e_d modified the CCSR_OR2_PROG  value. (WIND00185681)
01g,09jan09,e_d add #define MP_MAGIC_RBV
01f,08feb09,e_d change AUX_CLK_RATE_MAX from 600000 to 50000
01e,08feb09,e_d change NV_RAM_SIZE from 64k to 4k
01d,07nov08,y_w fixed comments of NVRAM section.
01c,28sep08,y_w modify for TFFS support.
01b,24apr08,mdg Changed VME_IN0_CFG_PARAMS to include TIPC memory.
01a,17mar08,mdg Created from 8540 header files Initial writing.
*/

/*
DESCRIPTION
This file contains macros and defines for I/O addresses, e600
core functions and other utility data.  Near the end of this
file, specific #include's are coded to load definitions unique
to one of the three specific boards suppored by this BSP: The
XMC module, SBC (single board computer), and the PrPMC.
*/

#ifndef __INCmv7100h
#define __INCmv7100h

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

/* includes */
#include "mpc8641.h"
#include "tempe.h"
#include "ppcProc.h"

/* Define Clock Speed and source  */

#define FREQ_33_MHZ     33330000
#define FREQ_40_MHZ     40000000
#define FREQ_50_MHZ     50000000
#define FREQ_66_MHZ     66670000
#define FREQ_83_MHZ     83330000
#define FREQ_100_MHZ    100000000
#define FREQ_133_MHZ    133330000
#define FREQ_266_MHZ    266660000
#define FREQ_200_MHZ    200000000
#define FREQ_333_MHZ    333330000
#define FREQ_533_MHZ    533330000
#define FREQ_400_MHZ    400000000
#define FREQ_600_MHZ    600000000

#define WDT_RATE_MIN         (sysTimerClkFreq / (1 << 29))
#define WDT_RATE_MAX         (sysTimerClkFreq / (1 << 21))

#define DEFAULT_CCB_SPD      (FREQ_600_MHZ)

#define QUART_REF_FREQ       1843200

/*
 * The following should be defined for REV A prototypes, and undef'ed for
 * newer boards
 */

#undef MVME7100_REV_A

/* Timer defines */

#define MPC8641_RTC_FREQ     1000000

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
#endif /* __GNUC__ */

/* BSP configuration error policy */

#define CONTINUE_EXECUTION      0       /* Tolerate VPD/Configuration errors */
#define EXIT_TO_SYSTEM_MONITOR  1       /* Transfer to System Monitor */

/* VPD Support */

#define DEFAULT_PRODUCT_ID  "Unknown"
#define DEFAULT_CPU_ID      "Unknown"
#define VPD_BRD_OFFSET      0   /* offset into brd's eeprom for vpd data */
#define VPD_PKT_LIMIT       25  /* Max number of packets expected */
#define VPD_EEPROM_SIZE     512 /* size of VPD serial EEPROM */
#define VPD_BYTE_ADDR_SIZE  2

/*
 * TBD:
 * The following #defines were leftovers from the "ads85xx.h" file.
 * We should clean this section up later.
 */

#ifdef  _ASMLANGUAGE
#define CAST(x)
#else /* _ASMLANGUAGE */
typedef volatile UINT32 VUINT32; /* volatile unsigned word */
#define CAST(x) (x)
#endif  /* _ASMLANGUAGE */

#define DELTA(a,b)                 (abs((int)a - (int)b))

/*
 * Space for the SMP-reboot detection structure.  This should go in
 * memory with the same volatility as DRAM -- i.e. it will survive
 * a reset via the PIXIS but not a power-cycle.  It would be nice
 * for it to be accessible without setting up the DRAM controller,
 * but absent knowledge of any such space on the board we'll just
 * put it in DRAM (and therefore it can't be tested until after the
 * DRAM controller has been set up).
 */

#define RBV_ADRS    (RAM_LOW_ADRS - 16)

/*
 * "magic numbers" used to validate the rebootVec.  The definitions
 * are arbitrary, but should not be "common" values like all zeros,
 * all ones, 0xeeeeeeee, 0x55aa55aa, 0xdeadbeef, etc.
 */

#define _WRS_BOOT_MAGIC_1    0x31415927
#define _WRS_BOOT_MAGIC_2    0x27182818
#define _WRS_BOOT_MAGIC_3    0x05772157

/* should not be a possible address for sysInit */

#define MP_MAGIC_RBV        0xfeedfeed

/*
 * Structure written at RBV_ADRS by SMP sysToMonitor when rebooting
 * with multiple processors running, examined by romInit to detect
 * an SMP warm boot.  The assembly version is defined for all builds,
 * because a non-SMP bootrom should work with an SMP vxWorks.
 */

#ifdef _ASMLANGUAGE
#define OFF_RBV_STARTTYPE   0
#define OFF_RBV_MAGIC1      4
#define OFF_RBV_MAGIC2      8
#define OFF_RBV_MAGIC3      12
#else   /* _ASMLANGUAGE */
# ifdef _WRS_CONFIG_SMP
struct rebootVec
    {
    int startType;
    volatile UINT32 magic1;
    volatile UINT32 magic2;
    volatile UINT32 magic3;
    };
# endif /* _WRS_CONFIG_SMP */
#endif  /* _ASMLANGUAGE */

/* OpenPic timer vector support */

#define OPENPIC_TIMERA0_INT_VEC 0x3c
#define OPENPIC_TIMERA1_INT_VEC 0x3d
#define OPENPIC_TIMERA2_INT_VEC 0x3e
#define OPENPIC_TIMERA3_INT_VEC 0x3f
#define OPENPIC_TIMERB0_INT_VEC 0x40
#define OPENPIC_TIMERB1_INT_VEC 0x41
#define OPENPIC_TIMERB2_INT_VEC 0x42
#define OPENPIC_TIMERB3_INT_VEC 0x43

/* Serial Support */

#ifdef INCLUDE_QUAD_UART
#   define N_SIO_CHANNELS   5       /* No. serial I/O channels */
#else
#   define N_SIO_CHANNELS   1       /* No. serial I/O channels */
#endif /* INCLUDE_QUAD_UART */

/* Translation macro */

#define TRANSLATE(x,y,z)\
        ((UINT)(x) - (UINT)(y) + (UINT)(z))

/* Base Address of Memory Mapped Registers */

#define CCSBAR      0xF1000000
#define CCSBAR_BUS  (CCSBAR)

#define CPU1CHECK   ((sysMsscr0Get() & 0x20) >> 5)

#define IS_DRAM_ADDRESS(addr) (((int)addr >= LOCAL_MEM_LOCAL_ADRS) && \
                               ((UINT32)addr < (UINT32)sysPhysMemTop()))

/* VME related */

#define VME_MEM_LOCAL_START 0x90000000
#define VME_MEM_BUS_START   (VME_MEM_LOCAL_START)

#define VME_A32_MSTR_LOCAL  VME_MEM_LOCAL_START
#define VME_A32_MSTR_BUS    (0x08000000)
#define VME_A32_MSTR_END    (VME_A32_MSTR_LOCAL + VME_A32_MSTR_SIZE)

#define VME_A24_MSTR_LOCAL  (VME_A32_MSTR_END)
#define VME_A24_MSTR_BUS    (0x00000000)
#define VME_A24_MSTR_END    (VME_A24_MSTR_LOCAL + VME_A24_MSTR_SIZE)

#define VME_A16_MSTR_LOCAL  (VME_A24_MSTR_END)
#define VME_A16_MSTR_BUS    (0x00000000)
#define VME_A16_MSTR_END    (VME_A16_MSTR_LOCAL + VME_A16_MSTR_SIZE)

/* Make sure VME_LOCAL_MEM_END is rounded to nearest 0x0001000 boundary */

#define VME_MEM_LOCAL_END   (VME_A16_MSTR_END)

/*
 * VME_MEM_SIZE defines the maximum extent of the VME space.  It must
 * be greater than or equal to the ranged defined by VME_MEM_LOCAL_START
 * and VME_MEM_LOCAL_END.  We can increase VME_A32 space by increasing
 * VME_A32_MSTR_SIZE for example and this will push up the value of
 * VME_MEM_LOCAL_END but we better not define more space in this extent
 * than is represented by VME_MEM_SIZE.  The space defined by VME_MEM_SIZE
 * will be encoded into a MPC8641 Outbound Window and thus the
 * additional constraint on VME_MEM_SIZE is that it must be a power of 2.
 */

#define VME_MEM_SIZE       (0x20000000)   /* Must be power of 2 */

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
#define VME_MBOX0_OFFSET        (TEMPE_GCSR_MBOX0 + 3)

/*
 * Allocation of VME space to outbound windows, first set up the out0
 * window to point to the CRG space to allow access to (among other
 * things) other board's mailboxes - necessary for VxWorks shared memory.
 */

#define VME_OUT0_START          (VME_CRG_MSTR_LOCAL)
#define VME_OUT0_SIZE           (VME_CRG_MSTR_SIZE)
#define VME_OUT0_BUS            (VME_CRG_MSTR_BUS)

/* Now out1 will point to some node zero system DRAM */

#define VME_DRAM_ACCESS_SIZE    (0x00100000)
#define VME_OUT1_START          (VME_A32_MSTR_LOCAL)
#define VME_OUT1_SIZE           (VME_DRAM_ACCESS_SIZE)
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
#    undef  VME_IN0_CFG_PARAMS
#ifdef INCLUDE_AMP
#    define VME_IN0_CFG_PARAMS       \
            TRUE,                    \
            0,                       \
            VME_A32_MSTR_BUS,        \
            0,                       \
            (0x00010000 +            \
             SM_MEM_SIZE +           \
             SM_OBJ_MEM_SIZE +       \
             SM_TIPC_SIZE),          \
            0,                       \
            0x0,                     \
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
#else
#    define VME_IN0_CFG_PARAMS       \
            TRUE,                    \
            0,                       \
            VME_A32_MSTR_BUS,        \
            0,                       \
            (0x00010000 +            \
             SM_MEM_SIZE +           \
             SM_OBJ_MEM_SIZE +       \
             SM_TIPC_SIZE),          \
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
#endif /* INCLUDE_AMP */
#endif /* SM_OFF_BOARD == FALSE */

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

#define TEMPE_CBAU_VAL   0    /* Upper 64-bits zero */
#define TEMPE_CBAL_VAL   (VME_CRG_MSTR_BUS+(sysProcNumGet()*VME_CRG_SLV_SIZE))
#define TEMPE_CSRAT_VAL  0x00000AF     /* Enable for A32 and all other access */

/* 64-bit address support (used on Tempe and maybe other places as well) */

#define MAKE64(high32,low32) (((UINT64)high32<<32)+((UINT64)low32))
#define UNMAKE64(val64,high32,low32) { high32 = (val64>>32) & 0xffffffff; \
                                     low32  = (val64) & 0xffffffff; }

/* Define Tempe interrupt numbers (assiciated with intConnect()) */

#define TEMPE_INT_NUM_MIN       (0x60)

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
 * into the MPC8641 interrupt handler.  The INT_USR_EXTRA component
 * can be initialized inside of "config.h" to refer to any special
 * intEnable()/intDisable() code that the user wants to invoke for
 * the user reserved interrupt vector numbers.
 */

#ifndef _ASMLANGUAGE
    STATUS sysTempeIntEnable (int intLevel);
    STATUS sysTempeIntDisable (int intLevel);
    IMPORT UINT32 sysMsscr0Get();
    IMPORT int sysSerialConsoleTty();
#endif /* _ASMLANGUAGE */

#define INT_UTIL_EXTRA  ,{TEMPE_INT_NUM_MIN,TEMPE_INT_NUM_MAX, \
                          sysTempeIntEnable,sysTempeIntDisable} \
                          INT_USR_EXTRA

/*
 * Define the minimum and maximum interrupt vector numbers available for user
 * applications.  Note: See INT_USR_EXTRA in "config.h".
 */

#define USR_INT_NUM_MIN         0xc0
#define USR_INT_NUM_MAX         0xcf

/* Support for VME bus locking */

#define LOCK_TIMEOUT            10
#define DECREMENTER_PRESCALE    4
#define USECS_PER_SECOND        1000000

/* Support for sysBusTas() RMW cycle */

#define VME_RMW_ENABLE_BITS     0x80000000
#define VME_RMW_COMPARE_BITS    0x00000000
#define VME_RMW_SWAP_TO_SET     0x80000000

/*
 * Support for determining if we're ROM based or not.  _sysInit
 * saves the startType parameter at location ROM_BASED_FLAG.
 */

#define PCI_AUTOCONFIG_FLAG_OFFSET (0x4c00)
#define PCI_AUTOCONFIG_FLAG        (*(UCHAR *)(LOCAL_MEM_LOCAL_ADRS + \
                                     PCI_AUTOCONFIG_FLAG_OFFSET))
#define PCI_AUTOCONFIG_DONE        (PCI_AUTOCONFIG_FLAG != 0)

/*
 * Support for determining which of the MPC8641's PCI interfaces to access
 * based on the PCI bus number specified.  sysPciAutoConfig() called from
 * sysHwInit() saves the highest numbered bus connected to Bus interface 0.0
 * and this bus number, plus 1, is equivalent to bus 0 on Bus interface 1.0.
 */

#define PCI_BUS_ALIAS_OFFSET       (PCI_AUTOCONFIG_FLAG_OFFSET + 4)
#define PCI_BUS_ALIAS              (*(UINT *)(LOCAL_MEM_LOCAL_ADRS + \
                                     PCI_BUS_ALIAS_OFFSET))

/* During Boot ROM, no spin locks are needed as there is only one core */

#define BOOT_ROM_FLAG_OFFSET       (PCI_BUS_ALIAS_OFFSET + 4)
#define BOOT_ROM_FLAG              (*(UINT *)(LOCAL_MEM_LOCAL_ADRS + \
                                     BOOT_ROM_FLAG_OFFSET))
#define BOOT_ROM_IN_PROCESS        (BOOT_ROM_FLAG == 0)

/* Define the host bridge device number */

#define PCI_HOST_BRIDGE_BUS_NUM         (0)
#define PCI_HOST_BRIDGE_DEV_NUM         (0)
#define PCI_HOST_BRIDGE_FUNC_NUM        (0)

/* Tempe Support */

#define TEMPE_PCI_BUS           5         /* PCI bus of Tempe PCI-VME bridge */
#define TEMPE_PCI_DEV           2         /* Device number of Tempe */
#define TEMPE_PCI_FCN           0         /* Function number of Tempe */
#define TEMPE_BRIDGE_PCI_BUS    4         /* PCI bus of Tempe leg of PCIE switch */
#define TEMPE_BRIDGE_PCI_DEV    0         /* Device number of Tempe */
#define TEMPE_BRIDGE_PCI_FCN    0         /* Function number of Tempe */
#define TEMPE_SWITCH_PCI_BUS    2         /* PCI bus of Tempe leg of PCIE switch */
#define TEMPE_SWITCH_PCI_DEV    1         /* Device number of Tempe */
#define TEMPE_SWITCH_PCI_FCN    0         /* Function number of Tempe */
#define PCIE0_SWITCH_PCI_BUS    1         /* PCI bus of PCIE switch */
#define PCIE0_SWITCH_PCI_DEV    0         /* Device number of Tempe */
#define PCIE0_SWITCH_PCI_FCN    0         /* Function number of Tempe */

/* PCI header rleated */

#define PCI_CLINE_SZ    (_CACHE_ALIGN_SIZE/4)   /* Cache Ln Size = 32 bytes */
#define PCI_LAT_TIMER   0xff    /* Latency Timer Value = 255 PCI clocks */

/*
 * Define the PCI regions:
 * MEMIO...      Nonprefetchable memory
 * MEM...        Prefetchable memory
 * ISA...        16-bit I/O
 * IO...         32-bit I/O
 */

/* Local (view from cpu) base addresses of PCI regions */

#define PCI0_MEM_LOCAL_START    (0x80000000)

#define PCI0_MSTR_MEMIO_LOCAL   (PCI0_MEM_LOCAL_START)
#define PCI0_MEMIO_LOCAL_END    (PCI0_MSTR_MEMIO_LOCAL + PCI0_MSTR_MEMIO_SIZE)

#define PCI0_MSTR_MEM_LOCAL     (PCI0_MEMIO_LOCAL_END)
#define PCI0_MEM_LOCAL_END      (PCI0_MSTR_MEM_LOCAL + PCI0_MSTR_MEM_SIZE -1)

#define PCI0_MEM_LOCAL_MAX      (0x90000000 - 1)    /* Max top of PCI */
#define PCI0_VME_MEM_LOCAL_END  (0xc0000000 - 1)    /* Max top of PCI + VME */
#define PCI0_VME_MEM_LOCAL_SIZE (PCI0_VME_MEM_LOCAL_END -  \
                                PCI0_MEM_LOCAL_START + 1)

#define PCI0_TEMPE_MEM_SIZE     0x100000
#define PCI0_TEMPE_MEM          ((PCI0_VME_MEM_LOCAL_END + 1) - \
                                PCI0_TEMPE_MEM_SIZE)
#define PCI0_TEMPE_BRIDGE_MEM   (PCI0_TEMPE_MEM - PCI0_TEMPE_MEM_SIZE)

#define PCI0_IO_LOCAL_START     (0xf0000000)

#define ISA0_MSTR_IO_LOCAL      (PCI0_IO_LOCAL_START)
#define ISA0_IO_LOCAL_END       (ISA0_MSTR_IO_LOCAL + ISA0_MSTR_IO_SIZE)

#define PCI0_MSTR_IO_LOCAL      (ISA0_IO_LOCAL_END)
#define PCI0_IO_LOCAL_END       (PCI0_MSTR_IO_LOCAL + PCI0_MSTR_IO_SIZE - 1)

#define PCI0_IO_LOCAL_MAX       (0xf0800000 - 1)    /* Max top of PCI IO */
#define PCI0_IO_SIZE            (ISA0_MSTR_IO_SIZE + PCI0_MSTR_IO_SIZE)
#define PCI0_IO_LOCAL_SIZE      (PCI0_IO_LOCAL_MAX -  \
                                PCI0_IO_LOCAL_START + 1)

#define PCI1_MEM_LOCAL_START    (0xc0000000)

#define PCI1_MSTR_MEMIO_LOCAL   (PCI1_MEM_LOCAL_START)
#define PCI1_MEMIO_LOCAL_END    (PCI1_MSTR_MEMIO_LOCAL + PCI1_MSTR_MEMIO_SIZE)

#define PCI1_MSTR_MEM_LOCAL     (PCI1_MEMIO_LOCAL_END)
#define PCI1_MEM_LOCAL_END      (PCI1_MSTR_MEM_LOCAL + PCI1_MSTR_MEM_SIZE - 1)

#define PCI1_MEM_LOCAL_MAX      (0xe0000000 - 1)    /* Max top of PCI */
#define PCI1_MEM_LOCAL_SIZE     (PCI1_MEM_LOCAL_START - PCI1_MEM_LOCAL_MAX + 1)


#define PCI1_IO_LOCAL_START     (0xf0800000)

#define ISA1_MSTR_IO_LOCAL      (PCI1_IO_LOCAL_START)
#define ISA1_IO_LOCAL_END       (ISA1_MSTR_IO_LOCAL + ISA1_MSTR_IO_SIZE)

#define PCI1_MSTR_IO_LOCAL      (ISA1_IO_LOCAL_END)
#define PCI1_IO_LOCAL_END       (PCI1_MSTR_IO_LOCAL + PCI1_MSTR_IO_SIZE - 1)

#define PCI1_IO_LOCAL_MAX       (0xf1000000 - 1)    /* Max top of PCI IO */
#define PCI1_IO_SIZE            (ISA1_MSTR_IO_SIZE + PCI1_MSTR_IO_SIZE)
#define PCI1_IO_LOCAL_SIZE      (PCI1_IO_LOCAL_MAX -  \
                                PCI1_IO_LOCAL_START + 1)

/* Bus (view from PCI bus) base addresses of PCI regions */

#define PCI0_MEM_BUS_START      (PCI0_MSTR_MEMIO_LOCAL)
#define PCI0_MSTR_MEMIO_BUS     (PCI0_MEM_BUS_START)
#define PCI0_MSTR_MEM_BUS       (PCI0_MSTR_MEM_LOCAL)

#define PCI0_IO_BUS_START       (0xf0000000)
#define ISA0_MSTR_IO_BUS        (PCI0_IO_BUS_START)
#define PCI0_MSTR_IO_BUS        (TRANSLATE(PCI0_MSTR_IO_LOCAL,\
                                ISA0_MSTR_IO_LOCAL,\
                                ISA0_MSTR_IO_BUS))

#define PCI1_MEM_BUS_START      (PCI1_MSTR_MEMIO_LOCAL)
#define PCI1_MSTR_MEMIO_BUS     (PCI1_MEM_BUS_START)
#define PCI1_MSTR_MEM_BUS       (PCI1_MSTR_MEM_LOCAL)

#define PCI1_IO_BUS_START       (0xf0800000)
#define ISA1_MSTR_IO_BUS        (PCI1_IO_BUS_START)
#define PCI1_MSTR_IO_BUS        (TRANSLATE(PCI1_MSTR_IO_LOCAL,\
                                ISA1_MSTR_IO_LOCAL,\
                                ISA1_MSTR_IO_BUS))

#define ISA_LEGACY_SIZE         0x00004000

/*
 * Compiler Errors:
 *
 * Signal a compile time error if:
 *
 * 1. The VME local memory space extends beyond the beginning of the PCI
 *    memory space.
 *
 * 2. The PCI local I/O space extends beyond the beginning of the
 *    MPC8641 internal register space.
 */

#if (VME_MEM_LOCAL_END > PCI1_MEM_LOCAL_START)
# error local VME memory space extends into local PCI memory space
#endif

#if (PCI0_IO_LOCAL_END > PCI0_IO_LOCAL_MAX)
# error PCI0 I/O space extends beyond PCI0 limit
#endif

#if (PCI0_MEM_LOCAL_END > PCI0_MEM_LOCAL_MAX)
# error PCI0 I/O space extends beyond PCI0 limit
#endif

#if (PCI1_IO_LOCAL_END > PCI1_IO_LOCAL_MAX)
# error PCI0 I/O space extends beyond PCI0 limit
#endif

#if (PCI1_MEM_LOCAL_END > PCI1_MEM_LOCAL_MAX)
# error PCI0 I/O space extends beyond PCI0 limit
#endif

/* Common I/O synchronizing instructions */

#ifndef EIEIO_SYNC
#   define EIEIO_SYNC  WRS_ASM("eieio;sync")
#endif  /* EIEIO_SYNC */

#ifndef EIEIO
#   define EIEIO       WRS_ASM("eieio")
#endif  /* EIEIO */

#ifndef SYNC
#   define SYNC        WRS_ASM("sync")
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
 * Define board specific addresses and field programming values.
 * There are three different register domains which much be programmed
 * in order to access an area such a flash or IO space.  These
 * domains are:
 *
 * MMU access -   This will be facilitated by programming TLB1 entries
 *            to cover the appropriate address range.  TLB1 entries
 *            are programmed initially inside of "romInit.s" and
 *            again after the kernel comes up, using the "sysLib.c"
 *            resident table sysStaticTlbDesc[].
 *
 * Local access - This is covered programming of the appropriate LAWBARn
 *                LAWARn register set.  Along with an address range, these
 *            registers specify the associated target interface.  This
 *            programming is performed inside of "romInit.s"
 *
 * Chip select  - This is governed by BRn and ORn register programming.
 *            This programming is also done inside of "romInit.s".
 *
 * All three of these register domains must be set in harmony with each
 * other to properly access flash or IO space.  The assumptions concerning
 * flash are:
 *
 * 1.  There may be up to two different banks of flash, the 'boot' flash
 *     and the 'alternate' flash.  Two flash banks are on the SBC
 *     (Single board computer) version of Big Easy.
 *
 * 2.  The boot and alternate flash banks occupy one contiguous region
 *     of memory and can thus be covered by a single TLB1 address range,
 *     and local access register set.  Separate flash banks require
 *     separate chip selects (and thus two sets of BRn/ORn registers
 *     must be programmed).  Note the we may actually have two separate
 *     TLB1 entries covering the same address range, one with a TS
 *     (Translation Space) bit set to zero, and one with TS set to 1.
 *     This will allow both the bootrom (TS = 0) and the kernel (TS = 1)
 *     to access this space.
 *
 * Note: FLASH_WINDOW_SIZE is used by sysTffs.c
 */

#define FLASH_BASE_ADRS     0xf8000000
#define FLASH_MMU_TLB_SZ    _MMU_TLB_SZ_128M
#define FLASH_LAWAR_SZ      MPC8641_PCI_SIZE_ENCODE(FLASH_WINDOW_SIZE)
#define FLASH_WINDOW_SIZE   0x08000000
#define FLASH_BOOT_ADRS     0xf8000000
#define FLASH_BOOT_SZ       0x08000000
#define FLASH_ALT_ADRS      0xf0000000
#define FLASH_ALT_SZ        0x08000000

#define LOCAL_BOARD_ACCESS_ADRS         0xf2000000
#define LOCAL_BOARD_ACCESS_SIZE         0x00040000
#define LOCAL_BOARD_ACCESS_MMU_TBL_SZ   _MMU_TLB_SZ_256K
#define LOCAL_BOARD_ACCESS_LAWAR_SZ\
                               MPC8641_PCI_SIZE_ENCODE(LOCAL_BOARD_ACCESS_SIZE)

#define SBC_MRAM_ADRS           0xf2400000
#define SBC_MRAM_SZ             0x80000
#define SBC_MRAM_LAWAR_SZ       MPC8641_PCI_SIZE_ENCODE(SBC_MRAM_SZ)

/* The following describe the inside of the "local board access" space */

#define SBC_CTRL_REG_ADRS       (LOCAL_BOARD_ACCESS_ADRS)
#define SBC_CTRL_REG_SZ         0x10000
#define SBC_QUAD_UART_ADRS      (SBC_CTRL_REG_ADRS + 0x10000)
#define SBC_QUAD_UART_SZ        0x10000
#define SBC_32BIT_TIMER_ADRS    (SBC_CTRL_REG_ADRS + 0x20000)
#define SBC_32BIT_TIMER_SZ      0x10000
#define SBC_NAND_FLASH_ADRS     (SBC_CTRL_REG_ADRS + 0x30000)
#define SBC_NAND_FLASH_SZ       0x10000

/*
 * Define programming values for the 8 sets of local access window
 *  registers (LAWBARn/LAWARn)
 */

/* Access to flash (covers chip selects 0 and 1 (BR0/OR0 and BR1/OR1)) */

#define CCSR_LAWBAR0_PROG (CCSR_LAWBAR_BASE_ADDR_ENCODE(FLASH_BASE_ADRS))
#define CCSR_LAWAR0_PROG  (CCSR_LAWAR_EN_VAL_ENABLED | \
                           CCSR_LAWAR_TGTIF_VAL_LBC | \
                           FLASH_LAWAR_SZ)

/* Access to local DRAM */

#define CCSR_LAWBAR1_PROG  \
    (CCSR_LAWBAR_BASE_ADDR_ENCODE(LOCAL_MEM_LOCAL_ADRS))
#define CCSR_LAWAR1_PROG  (CCSR_LAWAR_EN_VAL_ENABLED | \
                           CCSR_LAWAR_TGTIF_VAL_DDRSDRAM | \
                           MPC8641_PCI_SIZE_ENCODE(0x40000000))

/*
 * Access to local board devices (covers chip selects 2, 4, 5 and 6)
 * (BR2/OR2, BR4/OR4, BR5/OR5, and BR6/OR6))
 */

#define CCSR_LAWBAR2_PROG  \
        (CCSR_LAWBAR_BASE_ADDR_ENCODE(LOCAL_BOARD_ACCESS_ADRS))
#define CCSR_LAWAR2_PROG  (CCSR_LAWAR_EN_VAL_ENABLED | \
                           CCSR_LAWAR_TGTIF_VAL_LBC | \
                           LOCAL_BOARD_ACCESS_LAWAR_SZ)

/*
 * Now let's cover PCI space.  We must configure one local access
 * window for each of the configured outbound PCI windows.  There
 * will be a total of four windows configured for PCI space:
 *
 * PCI0_MEM_LOCAL
 * PCI1_MEM_LOCAL
 * PCI0_IO_LOCAL
 * PCI1_IO_LOCAL
 */

#define CCSR_LAWBAR3_PROG  \
    (CCSR_LAWBAR_BASE_ADDR_ENCODE(PCI0_MEM_LOCAL_START))
#define CCSR_LAWAR3_PROG  (CCSR_LAWAR_EN_VAL_ENABLED | \
                           CCSR_LAWAR_TGTIF_VAL_PCIE0 | \
                           MPC8641_PCI_SIZE_ENCODE(PCI0_VME_MEM_LOCAL_SIZE))

#define CCSR_LAWBAR4_PROG \
    (CCSR_LAWBAR_BASE_ADDR_ENCODE(PCI0_IO_LOCAL_START))
#define CCSR_LAWAR4_PROG (CCSR_LAWAR_EN_VAL_ENABLED | \
                          CCSR_LAWAR_TGTIF_VAL_PCIE0 | \
                          MPC8641_PCI_SIZE_ENCODE(PCI0_IO_LOCAL_SIZE))

#define CCSR_LAWBAR5_PROG \
    (CCSR_LAWBAR_BASE_ADDR_ENCODE(PCI1_MEM_LOCAL_START))
#define CCSR_LAWAR5_PROG (CCSR_LAWAR_EN_VAL_ENABLED | \
                          CCSR_LAWAR_TGTIF_VAL_PCIE1 | \
                           MPC8641_PCI_SIZE_ENCODE(PCI1_MEM_LOCAL_SIZE))

#define CCSR_LAWBAR6_PROG  \
    (CCSR_LAWBAR_BASE_ADDR_ENCODE(PCI1_IO_LOCAL_START))
#define CCSR_LAWAR6_PROG  (CCSR_LAWAR_EN_VAL_ENABLED | \
                           CCSR_LAWAR_TGTIF_VAL_PCIE1 | \
                           MPC8641_PCI_SIZE_ENCODE(PCI1_IO_LOCAL_SIZE))

/*
 * Access to MRAM device (covers chip selects 3)
 * (BR3/OR3)
 */

#define CCSR_LAWBAR7_PROG  \
        (CCSR_LAWBAR_BASE_ADDR_ENCODE(SBC_MRAM_ADRS))
#define CCSR_LAWAR7_PROG  (CCSR_LAWAR_EN_VAL_ENABLED | \
                           CCSR_LAWAR_TGTIF_VAL_LBC | \
                           SBC_MRAM_LAWAR_SZ)

/* Define information for chip select programming (BRn/ORn) */

/*
 * ----- Chip select 0
 *
 * BR0 to be set as follows (flash boot access):
 *   bit 0       = 1    ->  V: valid
 *   bit 2:3     = 00   ->  ATOM: Not used for atomic operations
 *   bit 5:7     = 000  ->  MSEL: Machine select = GPCM
 *   bit 8       = 0    ->  WP: Read and write accesses allowed
 *   bit 10:9    = 00   ->  DECC: Err check disable, parity generated
 *   bit 12:11   = 11   ->  PS: Port size = 32-bit
 *   bit 31:16   = xx   ->  BA: FLASH_BOOT_ADRS (base address)
 */

/*
 * Proto Rev A has only a 16 bit wide flash bus.  Newer boards are
 * the proper 32 bit width
 */

#ifdef MVME7100_REV_A
#define CCSR_BR0_PROG   (FLASH_BOOT_ADRS | 0x1001)
#else
#define CCSR_BR0_PROG   (FLASH_BOOT_ADRS | 0x1801)
#endif /* MVME7100_REV_A */

/*
 * OR0 to be set as follows:
 *   bit 0       = 1    -> EAD: Extra bus clock cycles are added (LALE
 *                           is asserted for the number of bus clock
 *                           cycles specified by LCRR[EADC].
 *   bit 1       = 1    -> EHTR: 8 idle clock cycles are inserted.
 *   bit 2       = 1    -> TRLX: relaxed timing
 *   bit 3       = 0    -> SETA: Access teminated internaly by memory
 *                           controller.
 *   bit 7:4     = 1111 -> SCY: 15 bus clock cycle wait states.
 *   bit 8       = 1    -> XACS: Address to chip select setup
 *                           is extended.
 *   bit 10:9    = 11   -> ACS: LCSn# is output a half bus clock cycle
 *                           after the address lines.
 *   bit 11      = 1    -> CSNT: LCSn# and LWE# are negated earlier
 *                           depending on the value of LCRR[CLKDIV].
 *   bit 12      = 0    -> BCTLD: LBCTL is asserted upon access to
 *                           the current memory bank.
 *   bit 31:16   = xxxx -> AM: GPCM address mask.
 *
 */

#define CCSR_OR0_PROG   (CCSR_ORn_AM_VAL(FLASH_BOOT_SZ) | 0x6FF7)

/*
 * ----- Chip select 1 - not used
 *
 */

#define CCSR_BR1_PROG   (CCSR_BRn_INVALID)
#define CCSR_OR1_PROG   (CCSR_ORn_INVALID)

/*
 * ----- Chip select 2
 *
 * BR2 to be set as follows (NAND Flash):
 * bit  0     = 1    ->  V: valid
 * bits 2:3   = 00b  ->  ATOM: Not used for atomic operations
 * bits 5:7   = 000b ->  MSEL: Machine select = GPCM
 * bit  8     = 0    ->  WP: Read and write accesses allowed
 * bits 10:9  = 00b  ->  DECC: Err check disable, parity generated
 * bits 12:11 = 01b  ->  PS: Port size = 8-bit
 * bits 31:16 = xx   ->  BA: SBC_NAND_FLASH_ADRS (base address)
 */

#define CCSR_BR2_PROG   (SBC_NAND_FLASH_ADRS | 0x0801)

/*
 * OR2 to be set as follows:
 *   bit 0       = 1    -> EAD: Extra bus clock cycles are added (LALE
 *                           is asserted for the number of bus clock
 *                           cycles specified by LCRR[EADC].
 *   bit 1       = 1    -> EHTR: 8 idle clock cycles are inserted.
 *   bit 2       = 1    -> TRLX: relaxed timing
 *   bit 3       = 0    -> SETA: Access teminated internaly by memory
 *                           controller.
 *   bit 7:4     = 1111 -> SCY: 15 bus clock cycle wait states.
 *   bit 8       = 1    -> XACS: Address to chip select setup
 *                           is extended.
 *   bit 10:9    = 11   -> ACS: LCSn# is output a half bus clock cycle
 *                           after the address lines.
 *   bit 11      = 1    -> CSNT: LCSn# and LWE# are negated earlier
 *                           depending on the value of LCRR[CLKDIV].
 *   bit 12      = 0    -> BCTLD: LBCTL is asserted upon access to
 *                           the current memory bank.
 *   bit 31:16   = xxxx -> AM: GPCM address mask.
 */

#define CCSR_OR2_PROG   (CCSR_ORn_AM_VAL(SBC_NAND_FLASH_SZ) | 0x6834)

/*
 * ----- Chip select 3
 *
 * BR3 to be set as follows (MRAM):
 * bit  0     = 1    ->  V: valid
 * bits 2:3   = 00b  ->  ATOM: Not used for atomic operations
 * bits 5:7   = 000b ->  MSEL: Machine select = GPCM
 * bit  8     = 0    ->  WP: Read and write accesses allowed
 * bits 10:9  = 00b  ->  DECC: Err check disable, parity generated
 * bits 12:11 = 10b  ->  PS: Port size = 16-bit
 * bits 31:16 = xx   ->  BA: SBC_MRAM_ADRS (base address)
 */

#define CCSR_BR3_PROG   (SBC_MRAM_ADRS | 0x1001)

/*
 * OR3 to be set as follows:
 *   bit 0       = 1    -> EAD: Extra bus clock cycles are added (LALE
 *                           is asserted for the number of bus clock
 *                           cycles specified by LCRR[EADC].
 *   bit 1       = 1    -> EHTR: 8 idle clock cycles are inserted.
 *   bit 2       = 1    -> TRLX: relaxed timing
 *   bit 3       = 0    -> SETA: Access teminated internaly by memory
 *                           controller.
 *   bit 7:4     = 1111 -> SCY: 15 bus clock cycle wait states.
 *   bit 8       = 1    -> XACS: Address to chip select setup
 *                           is extended.
 *   bit 10:9    = 11   -> ACS: LCSn# is output a half bus clock cycle
 *                           after the address lines.
 *   bit 11      = 1    -> CSNT: LCSn# and LWE# are negated earlier
 *                           depending on the value of LCRR[CLKDIV].
 *   bit 12      = 0    -> BCTLD: LBCTL is asserted upon access to
 *                           the current memory bank.
 *   bit 31:16   = xxxx -> AM: GPCM address mask.
 *
 */

# define CCSR_OR3_PROG  (CCSR_ORn_AM_VAL(SBC_MRAM_SZ) | 0x6FF7)

/*
 * ----- Chip select 4
 *
 * BR4 to be set as follows (Control Registers):
 * bit  0     = 1    ->  V: valid
 * bits 2:3   = 00b  ->  ATOM: Not used for atomic operations
 * bits 5:7   = 000b ->  MSEL: Machine select = GPCM
 * bit  8     = 0    ->  WP: Read and write accesses allowed
 * bits 10:9  = 00b  ->  DECC: Err check disable, parity generated
 * bits 12:11 = 11b  ->  PS: Port size = 32-bit
 * bits 31:16 = xx   ->  BA: SBC_CTRL_REG_ADRS (base address)
 */

#define CCSR_BR4_PROG   (SBC_CTRL_REG_ADRS | 0x1801)

/*
 * OR4 to be set as follows:
 *   bit 0       = 1    -> EAD: Extra bus clock cycles are added (LALE
 *                           is asserted for the number of bus clock
 *                           cycles specified by LCRR[EADC].
 *   bit 1       = 1    -> EHTR: 8 idle clock cycles are inserted.
 *   bit 2       = 2    -> TRLX: relaxed timing
 *   bit 3       = 0    -> SETA: Access teminated internaly by memory
 *                           controller.
 *   bit 7:4     = 1111 -> SCY: 15 bus clock cycle wait states.
 *   bit 8       = 1    -> XACS: Address to chip select setup
 *                           is extended.
 *   bit 10:9    = 11   -> ACS: LCSn# is output a half bus clock cycle
 *                           after the address lines.
 *   bit 11      = 1    -> CSNT: LCSn# and LWE# are negated earlier
 *                           depending on the value of LCRR[CLKDIV].
 *   bit 12      = 0    -> BCTLD: LBCTL is asserted upon access to
 *                           the current memory bank.
 *   bit 31:16   = xxxx -> AM: GPCM address mask.
 */

#define CCSR_OR4_PROG   (CCSR_ORn_AM_VAL(SBC_CTRL_REG_SZ) | 0x6FF7)

#define CCSR_BR5_PROG   (SBC_QUAD_UART_ADRS | 0x0801)

/*
 * ----- Chip select 5
 *
 * BR4 to be set as follows (Quad UART):
 * bit  0     = 1    ->  V: valid
 * bits 2:3   = 00b  ->  ATOM: Not used for atomic operations
 * bits 5:7   = 000b ->  MSEL: Machine select = GPCM
 * bit  8     = 0    ->  WP: Read and write accesses allowed
 * bits 10:9  = 00b  ->  DECC: Err check disable, parity generated
 * bits 12:11 = 01b  ->  PS: Port size = 8-bit
 * bits 31:16 = xx   ->  BA: SBC_QUAD_UART_ADRS (base address)
 */

#define CCSR_OR5_PROG   (CCSR_ORn_AM_VAL(SBC_QUAD_UART_SZ) | 0x6FF7)

/*
 * OR5 to be set as follows:
 *   bit 0       = 1    -> EAD: Extra bus clock cycles are added (LALE
 *                           is asserted for the number of bus clock
 *                           cycles specified by LCRR[EADC].
 *   bit 1       = 1    -> EHTR: 8 idle clock cycles are inserted.
 *   bit 2       = 2    -> TRLX: relaxed timing
 *   bit 3       = 0    -> SETA: Access teminated internaly by memory
 *                           controller.
 *   bit 7:4     = 1111 -> SCY: 15 bus clock cycle wait states.
 *   bit 8       = 1    -> XACS: Address to chip select setup
 *                           is extended.
 *   bit 10:9    = 11   -> ACS: LCSn# is output a half bus clock cycle
 *                           after the address lines.
 *   bit 11      = 1    -> CSNT: LCSn# and LWE# are negated earlier
 *                           depending on the value of LCRR[CLKDIV].
 *   bit 12      = 0    -> BCTLD: LBCTL is asserted upon access to
 *                           the current memory bank.
 *   bit 31:16   = xxxx -> AM: GPCM address mask.
 */

#define CCSR_BR6_PROG   (SBC_32BIT_TIMER_ADRS | 0x1801)

/*
 * ----- Chip select 6
 *
 * BR4 to be set as follows (32-bit timers):
 * bit  0     = 1    ->  V: valid
 * bits 2:3   = 00b  ->  ATOM: Not used for atomic operations
 * bits 5:7   = 000b ->  MSEL: Machine select = GPCM
 * bit  8     = 0    ->  WP: Read and write accesses allowed
 * bits 10:9  = 00b  ->  DECC: Err check disable, parity generated
 * bits 12:11 = 11b  ->  PS: Port size = 32-bit
 * bits 31:16 = xx   ->  BA: SBC_32BIT_TIMER_ADRS (base address)
 */

#define CCSR_OR6_PROG   (CCSR_ORn_AM_VAL(SBC_32BIT_TIMER_SZ) | 0x6FF7)

/*
 * OR5 to be set as follows:
 *   bit 0       = 1    -> EAD: Extra bus clock cycles are added (LALE
 *                           is asserted for the number of bus clock
 *                           cycles specified by LCRR[EADC].
 *   bit 1       = 1    -> EHTR: 8 idle clock cycles are inserted.
 *   bit 2       = 2    -> TRLX: relaxed timing
 *   bit 3       = 0    -> SETA: Access teminated internaly by memory
 *                           controller.
 *   bit 7:4     = 1111 -> SCY: 15 bus clock cycle wait states.
 *   bit 8       = 1    -> XACS: Address to chip select setup
 *                           is extended.
 *   bit 10:9    = 11   -> ACS: LCSn# is output a half bus clock cycle
 *                           after the address lines.
 *   bit 11      = 1    -> CSNT: LCSn# and LWE# are negated earlier
 *                           depending on the value of LCRR[CLKDIV].
 *   bit 12      = 0    -> BCTLD: LBCTL is asserted upon access to
 *                           the current memory bank.
 *   bit 31:16   = xxxx -> AM: GPCM address mask.
 */

/* The rest are invalid */

#define CCSR_BR7_PROG   (CCSR_BRn_INVALID)
#define CCSR_OR7_PROG   (CCSR_ORn_INVALID)

/* Now for some board related registers */

#define BRD_SYSTEM_STATUS_REG           (LOCAL_BOARD_ACCESS_ADRS + 0x0)
#define BRD_SYSTEM_CONTROL_REG          (LOCAL_BOARD_ACCESS_ADRS + 0x1)
#define BRD_SYSTEM_STATUS_INDICATOR_REG (LOCAL_BOARD_ACCESS_ADRS + 0x2)
#define BRD_FLASH_CONTROL_REG           (LOCAL_BOARD_ACCESS_ADRS + 0x3)
#define BRD_INTERRUPT_REG_1             (LOCAL_BOARD_ACCESS_ADRS + 0x4)
#define BRD_INTERRUPT_REG_2             (LOCAL_BOARD_ACCESS_ADRS + 0x5)
#define BRD_PRESENCE_DETECT_REG         (LOCAL_BOARD_ACCESS_ADRS + 0x6)
#define BRD_WATCH_DOG_LOAD_REG          (LOCAL_BOARD_ACCESS_ADRS + 0x20)
#define BRD_WATCH_DOG_CTRL_REG          (LOCAL_BOARD_ACCESS_ADRS + 0x24)
#define BRD_WATCH_DOG_RES_REG           (LOCAL_BOARD_ACCESS_ADRS + 0x25)
#define BRD_WATCH_DOG_COUNT_REG         (LOCAL_BOARD_ACCESS_ADRS + 0x26)
#define BRD_PLD_REVISION_REG            (LOCAL_BOARD_ACCESS_ADRS + 0x30)
#define BRD_PLD_DATE_CODE_REG           (LOCAL_BOARD_ACCESS_ADRS + 0x34)
#define BRD_TEST_1_REG                  (LOCAL_BOARD_ACCESS_ADRS + 0x38)
#define BRD_TEST_2_REG                  (LOCAL_BOARD_ACCESS_ADRS + 0x3C)


#define BRD_INTERRUPT_REG_1_PHY1_MASK       0x01
#define BRD_INTERRUPT_REG_1_PHY2_MASK       0x02
#define BRD_INTERRUPT_REG_1_PHY3_MASK       0x04
#define BRD_INTERRUPT_REG_1_PHY4_MASK       0x08
#define BRD_INTERRUPT_REG_2_ABORT           0x01
#define BRD_INTERRUPT_REG_2_TEMP            0x02
#define BRD_INTERRUPT_REG_2_RTC             0x04
#define BRD_INTERRUPT_REG_2_ABORT_MASK      0x10
#define BRD_INTERRUPT_REG_2_TEMP_MASK       0x20
#define BRD_INTERRUPT_REG_2_RTC_MASK        0x40
#define BRD_PRESENCE_DETECT_REG_ERDY2_MASK  0x20
#define BRD_PRESENCE_DETECT_REG_ERDY1_MASK  0x10
#define BRD_WATCH_DOG_LOAD_PATTERN          0xDB
#define BRD_WATCH_DOG_ENABLE_MASK           0x80
#define BRD_WATCH_DOG_SYS_RST_MASK          0x40

/* MPC8641 DUART Support */

#define COM0_BASE_ADRS      ((ULONG) CCSBAR + (ULONG) CCSR_URBR0)

/* Rear Transition Module Quad UART Support */

#define COM1_BASE_ADRS      (SBC_QUAD_UART_ADRS + 0x1000)
#define COM2_BASE_ADRS      (SBC_QUAD_UART_ADRS + 0x2000)
#define COM3_BASE_ADRS      (SBC_QUAD_UART_ADRS + 0x3000)
#define COM4_BASE_ADRS      (SBC_QUAD_UART_ADRS + 0x4000)

#define N_UART_CHANNELS     4   /* No. serial I/O channels */

/* External Tick timers */

#define BRD_PRESCALER_REG           (LOCAL_BOARD_ACCESS_ADRS + 0x20000)

#define BRD_TICK_TIMER_1_CTRL_REG   (LOCAL_BOARD_ACCESS_ADRS + 0x20010)
#define BRD_TICK_TIMER_2_CTRL_REG   (LOCAL_BOARD_ACCESS_ADRS + 0x20020)
#define BRD_TICK_TIMER_3_CTRL_REG   (LOCAL_BOARD_ACCESS_ADRS + 0x20030)
#define BRD_TICK_TIMER_4_CTRL_REG   (LOCAL_BOARD_ACCESS_ADRS + 0x20040)
#define BRD_TICK_TIMER_1_CMPR_REG   (LOCAL_BOARD_ACCESS_ADRS + 0x20014)
#define BRD_TICK_TIMER_2_CMPR_REG   (LOCAL_BOARD_ACCESS_ADRS + 0x20024)
#define BRD_TICK_TIMER_3_CMPR_REG   (LOCAL_BOARD_ACCESS_ADRS + 0x20034)
#define BRD_TICK_TIMER_4_CMPR_REG   (LOCAL_BOARD_ACCESS_ADRS + 0x20044)
#define BRD_TICK_TIMER_1_CNTR_REG   (LOCAL_BOARD_ACCESS_ADRS + 0x20018)
#define BRD_TICK_TIMER_2_CNTR_REG   (LOCAL_BOARD_ACCESS_ADRS + 0x20028)
#define BRD_TICK_TIMER_3_CNTR_REG   (LOCAL_BOARD_ACCESS_ADRS + 0x20038)
#define BRD_TICK_TIMER_4_CNTR_REG   (LOCAL_BOARD_ACCESS_ADRS + 0x20048)

#define TMR_CNTR0_INT_LVL           EXT_TIMER_INT_VEC
#define TMR_CNTR0_INT_VEC           (EXT_TIMER_INT_VEC + 0x00)

/* OK, here are some bit and mask definitions for the board registers */

#define BRD_SYSTEM_CONTROL_TSTAT_BIT    (0)
#define BRD_SYSTEM_CONTROL_TSTAT_MASK   (1 << BRD_SYSTEM_CONTROL_TSTAT_BIT)

#define BRD_SYSTEM_CONTROL_EEPROMWP_BIT  (1)
#define BRD_SYSTEM_CONTROL_EEPROMWP_MASK (1 << BRD_SYSTEM_CONTROL_EEPROMWP_BIT)

#define BRD_SYSTEM_CONTROL_FPHY_MASK_BIT (2)
#define BRD_SYSTEM_CONTROL_FPHY_MASK_MASK \
        (1 << BRD_SYSTEM_CONTROL_FPHY_MASK_BIT)

#define BRD_SYSTEM_CONTROL_RESET_BIT    (5)
#define BRD_SYSTEM_CONTROL_RESET_MASK   (7 << BRD_SYSTEM_CONTROL_RESET_BIT)
#define BRD_SYSTEM_CONTROL_RESET_VAL    0xA0

#define BRD_SYSTEM_STATUS_BRD_TYPE_BIT  (0)
#define BRD_SYSTEM_STATUS_BRD_TYPE_MASK  \
        (0x03 << BRD_SYSTEM_STATUS_BRD_TYPE_BIT)
#define BRD_SYSTEM_STATUS_VME_BRD_TYPE    0
#define BRD_SYSTEM_STATUS_PRPMC_BRD_TYPE  1

#define BRD_SYSTEM_STATUS_SAFE_START_BIT  (4)
#define BRD_SYSTEM_STATUS_SAFE_START_MASK  \
        (1 << BRD_SYSTEM_STATUS_SAFE_START_BIT)

#define BRD_SYSTEM_STATUS_INDICATOR_USR1R_LED_BIT (0)
#define BRD_SYSTEM_STATUS_INDICATOR_USR1R_LED_MASK \
        (1 << BRD_SYSTEM_STATUS_INDICATOR_USR1R_LED_BIT)

#define BRD_SYSTEM_STATUS_INDICATOR_USR1Y_LED_BIT (1)
#define BRD_SYSTEM_STATUS_INDICATOR_USR1Y_LED_MASK \
        (1 << BRD_SYSTEM_STATUS_INDICATOR_USR1Y_LED_BIT)

#define BRD_SYSTEM_STATUS_INDICATOR_USR2_LED_BIT (2)
#define BRD_SYSTEM_STATUS_INDICATOR_USR2_LED_MASK \
        (1 << BRD_SYSTEM_STATUS_INDICATOR_USR2_LED_BIT)

#define BRD_SYSTEM_STATUS_INDICATOR_USR3_LED_BIT (3)
#define BRD_SYSTEM_STATUS_INDICATOR_USR3_LED_MASK \
        (1 << BRD_SYSTEM_STATUS_INDICATOR_USR3_LED_BIT)

#define BRD_SYSTEM_STATUS_INDICATOR_USR_LED_MASK  0x0F

#define BRD_FLASH_CONTROL_FLASH_RDY_BIT  (0)
#define BRD_FLASH_CONTROL_FLASH_RDY_MASK  \
        (1 << BRD_FLASH_CONTROL_FLASH_RDY_BIT)

#define BRD_FLASH_CONTROL_BOOT_BLOCK_SEL_BIT  (1)
#define BRD_FLASH_CONTROL_BOOT_BLOCK_SEL_MASK  \
        (1 << BRD_FLASH_CONTROL_BOOT_BLOCK_SEL_BIT)

#define BRD_FLASH_CONTROL_HARDWARE_WP_BIT  (2)
#define BRD_FLASH_CONTROL_HARDWARE_WP_MASK  \
        (1 << BRD_FLASH_CONTROL_HARDWARE_WP_BIT)

#define BRD_FLASH_CONTROL_SOFTWARE_WP_BIT  (3)
#define BRD_FLASH_CONTROL_SOFTWARE_WP_MASK  \
        (1 << BRD_FLASH_CONTROL_SOFTWARE_WP_BIT)

#define BRD_FLASH_CONTROL_MAP_SEL_BIT  (4)
#define BRD_FLASH_CONTROL_MAP_SEL_MASK  \
        (1 << BRD_FLASH_CONTROL_MAP_SEL_BIT)

#define BRD_PCI_BUS_A_STAT_SPD_BIT  (0)
#define BRD_PCI_BUS_A_STAT_SPD_MASK  \
        (0x03 << BRD_PCI_BUS_A_STAT_SPD_BIT)

#define BRD_PCI_BUS_A_STAT_PCIX_BIT  (2)
#define BRD_PCI_BUS_A_STAT_PCIX_MASK  \
        (1 << BRD_PCI_BUS_A_STAT_PCIX_BIT)

#define BRD_PCI_BUS_A_STAT_64B_BIT  (3)
#define BRD_PCI_BUS_A_STAT_64B_MASK  \
        (1 << BRD_PCI_BUS_A_STAT_64B_BIT)

#define BRD_PCI_BUS_B_STAT_SPD_BIT  (0)
#define BRD_PCI_BUS_B_STAT_SPD_MASK  \
        (0x03 << BRD_PCI_BUS_B_STAT_SPD_BIT)

#define BRD_PCI_BUS_B_STAT_PCIX_BIT  (2)
#define BRD_PCI_BUS_B_STAT_PCIX_MASK  \
        (1 << BRD_PCI_BUS_B_STAT_PCIX_BIT)

#define BRD_PCI_BUS_B_STAT_64B_BIT  (3)
#define BRD_PCI_BUS_B_STAT_64B_MASK  \
        (1 << BRD_PCI_BUS_B_STAT_64B_BIT)

#define BRD_PCI_BUS_B_STAT_ERDY1_BIT  (4)
#define BRD_PCI_BUS_B_STAT_ERDY1_MASK  \
        (1 << BRD_PCI_BUS_B_STAT_ERDY1_BIT)

#define BRD_PCI_BUS_B_STAT_ERDY2_BIT  (5)
#define BRD_PCI_BUS_B_STAT_ERDY2_MASK  \
        (1 << BRD_PCI_BUS_B_STAT_ERDY2_BIT)

#define BRD_PCI_BUS_B_STAT_5VIO_BIT  (6)
#define BRD_PCI_BUS_B_STAT_5VIO_MASK  \
        (1 << BRD_PCI_BUS_B_STAT_5VIO_BIT)

#define BRD_PCI_BUS_B_STAT_3VIO_BIT  (7)
#define BRD_PCI_BUS_B_STAT_3VIO_MASK  \
        (1 << BRD_PCI_BUS_B_STAT_3VIO_BIT)

#define BRD_PCI_BUS_C_STAT_SPD_BIT  (0)
#define BRD_PCI_BUS_C_STAT_SPD_MASK  \
        (0x03 << BRD_PCI_BUS_C_STAT_SPD_BIT)

#define BRD_PCI_BUS_C_STAT_PCIX_BIT  (2)
#define BRD_PCI_BUS_C_STAT_PCIX_MASK  \
        (1 << BRD_PCI_BUS_C_STAT_PCIX_BIT)

#define BRD_PCI_BUS_C_STAT_64B_BIT  (3)
#define BRD_PCI_BUS_C_STAT_64B_MASK  \
        (1 << BRD_PCI_BUS_C_STAT_64B_BIT)

#define BRD_PRESENCE_DETECT_PMC1_BIT  (0)
#define BRD_PRESENCE_DETECT_PMC1_MASK  \
        (1 << BRD_PRESENCE_DETECT_PMC1_BIT)

#define BRD_PRESENCE_DETECT_PMC2_BIT  (1)
#define BRD_PRESENCE_DETECT_PMC2_MASK  \
        (1 << BRD_PRESENCE_DETECT_PMC2_BIT)

#define BRD_PRESENCE_DETECT_PMC_SPAN_BIT  (2)
#define BRD_PRESENCE_DETECT_PMC_SPAN_MASK  \
        (1 << BRD_PRESENCE_DETECT_PMC_SPAN_BIT)

/* PHY Register definitions */

#define BRGPHY_AUXSTS           0x19    /* AUX status */
#define BRGPHY_AUXSTS_ACOMP     0x8000  /* autoneg complete */
#define BRGPHY_AUXSTS_AN_ACK    0x4000  /* autoneg complete ack */
#define BRGPHY_AUXSTS_AN_ACK_D  0x2000  /* autoneg complete ack detect */
#define BRGPHY_AUXSTS_AN_NPW    0x1000  /* autoneg next page wait */
#define BRGPHY_AUXSTS_AN_RES    0x0700  /* AN HDC */
#define BRGPHY_AUXSTS_PDF       0x0080  /* Parallel detect. fault */
#define BRGPHY_AUXSTS_RF        0x0040  /* remote fault */
#define BRGPHY_AUXSTS_ANP_R     0x0020  /* AN page received */
#define BRGPHY_AUXSTS_LP_ANAB   0x0010  /* LP AN ability */
#define BRGPHY_AUXSTS_LP_NPAB   0x0008  /* LP Next page ability */
#define BRGPHY_AUXSTS_LINK      0x0004  /* Link status */
#define BRGPHY_AUXSTS_PRR       0x0002  /* Pause resolution-RX */
#define BRGPHY_AUXSTS_PRT       0x0001  /* Pause resolution-TX */

#define BRGPHY_RES_1000FD       0x0700  /* 1000baseT full duplex */
#define BRGPHY_RES_1000HD       0x0600  /* 1000baseT half duplex */
#define BRGPHY_RES_100FD        0x0500  /* 100baseT full duplex */
#define BRGPHY_RES_100T4        0x0400  /* 100baseT4 */
#define BRGPHY_RES_100HD        0x0300  /* 100baseT half duplex */
#define BRGPHY_RES_10FD         0x0200  /* 10baseT full duplex */
#define BRGPHY_RES_10HD         0x0100  /* 10baseT half duplex */

#define BRGPHY_ISR              0x1A    /* interrupt status */
#define BRGPHY_ISR_PSERR        0x4000  /* Pair swap error */
#define BRGPHY_ISR_MDXI_SC      0x2000  /* MDIX Status Change */
#define BRGPHY_ISR_HCT          0x1000  /* counter above 32K */
#define BRGPHY_ISR_LCT          0x0800  /* all counter below 128 */
#define BRGPHY_ISR_AN_PR        0x0400  /* Autoneg page received */
#define BRGPHY_ISR_NO_HDCL      0x0200  /* No HCD Link */
#define BRGPHY_ISR_NO_HDC       0x0100  /* No HCD */
#define BRGPHY_ISR_USHDC        0x0080  /* Negotiated Unsupported HCD */
#define BRGPHY_ISR_SCR_S_ERR    0x0040  /* Scrambler sync error */
#define BRGPHY_ISR_RRS_CHG      0x0020  /* Remote RX status change */
#define BRGPHY_ISR_LRS_CHG      0x0010  /* Local RX status change */
#define BRGPHY_ISR_DUP_CHG      0x0008  /* Duplex mode change */
#define BRGPHY_ISR_LSP_CHG      0x0004  /* Link speed changed */
#define BRGPHY_ISR_LNK_CHG      0x0002  /* Link status change */
#define BRGPHY_ISR_CRCERR       0x0001  /* CEC error */

#define BRGPHY_IMR              0x1B    /* interrupt mask */
#define BRGPHY_IMR_PSERR        0x4000  /* Pair swap error */
#define BRGPHY_IMR_MDXI_SC      0x2000  /* MDIX Status Change */
#define BRGPHY_IMR_HCT          0x1000  /* counter above 32K */
#define BRGPHY_IMR_LCT          0x0800  /* all counter below 128 */
#define BRGPHY_IMR_AN_PR        0x0400  /* Autoneg page received */
#define BRGPHY_IMR_NO_HDCL      0x0200  /* No HCD Link */
#define BRGPHY_IMR_NO_HDC       0x0100  /* No HCD */
#define BRGPHY_IMR_USHDC        0x0080  /* Negotiated Unsupported HCD */
#define BRGPHY_IMR_SCR_S_ERR    0x0040  /* Scrambler sync error */
#define BRGPHY_IMR_RRS_CHG      0x0020  /* Remote RX status change */
#define BRGPHY_IMR_LRS_CHG      0x0010  /* Local RX status change */
#define BRGPHY_IMR_DUP_CHG      0x0008  /* Duplex mode change */
#define BRGPHY_IMR_LSP_CHG      0x0004  /* Link speed changed */
#define BRGPHY_IMR_LNK_CHG      0x0002  /* Link status change */
#define BRGPHY_IMR_CRCERR       0x0001  /* CEC error */

/* TFFS Support */

#define NUM_TFFS_VOLS   1

/*
 * The following definitions are provided to support the automatic
 * mounting of the TFFS drive at kernel boot.  sysApplInit() is
 * defined in sysLib.c.
 */

#undef INCLUDE_USER_APPL

/* I2C device address etc. */

#define ADT7461_TEMP_SENSOR_I2C_ADDR    (0x98)
#define SPD1_EEPROM_I2C_ADDR            (0xa0)
#define SPD2_EEPROM_I2C_ADDR            (0xa2)
#define USR_BRD_EEPROM1_I2C_ADDR        (0xa4)
#define USR_BRD_EEPROM2_I2C_ADDR        (0xa6)
#define VPD_BRD_EEPROM_I2C_ADDR         (0xa8)
#define VPD_RTM_EEPROM_I2C_ADDR         (0xaa)
#define DS1375_RTC_I2C_ADDR             (0xd0)

#define VPD_BYTE_ADDR_SZ        2

#define VALID_I2C_WRT_ADDR      USR_BRD_EEPROM1_I2C_ADDR, \
                                USR_BRD_EEPROM2_I2C_ADDR, \
                                VPD_BRD_EEPROM_I2C_ADDR, \
                                ADT7461_TEMP_SENSOR_I2C_ADDR, \
                                DS1375_RTC_I2C_ADDR, \
                                0xffffffff

#define I2C_WRITE_ENABLE \
    { \
    UINT8 reg08; \
    reg08 = *(volatile UINT8 *)BRD_SYSTEM_CONTROL_REG; \
    reg08 &= ~BRD_SYSTEM_CONTROL_EEPROMWP_MASK; \
    *(volatile UINT8 *)(BRD_SYSTEM_CONTROL_REG) = reg08; \
    reg08 = *(volatile UINT8 *)BRD_SYSTEM_CONTROL_REG; \
    }

/*
 * Non-volatile (NVRAM) defines.
 *
 * The MVME7100 board does not have a true NVRAM chip.  This
 * functionality is implemented via an 64KB EEPROM.  This serial EEPROM is
 * specifically for user configuration data storage.
 *
 * Of the storage area available to VxWorks, the first 0x100 (256 bytes)
 * are used to store the VxWorks boot parameters.
 */

#undef  NV_BOOT_OFFSET
#define NV_BOOT_OFFSET          0           /* From start */
#define NV_RAM_SIZE             0x1000     /*  4KB Total */
#define NV_RAM_INTRVL           1
#define NV_RAM_I2C_ADDR         USR_BRD_EEPROM1_I2C_ADDR
#define NV_RAM_READ(x)          sysNvRead (x,NV_RAM_I2C_ADDR)
#define NV_RAM_WRITE(x,y)       sysNvWrite (x,y,NV_RAM_I2C_ADDR)

/* DDR SDRAM Memory Controller Configuration Parameters */

#define DDR_SDRAM_CS_0_MODE     DDR_SDRAM_PAGE_MODE
#define DDR_SDRAM_CS_1_MODE     DDR_SDRAM_PAGE_MODE
#define DDR_SDRAM_CS_2_MODE     DDR_SDRAM_PAGE_MODE
#define DDR_SDRAM_CS_3_MODE     DDR_SDRAM_PAGE_MODE
#define DDR_SDRAM_BSTOPRE_VAL        0x100
#define MPC8641_DDR_SDRAM_WRREC_VAL  3
#define MPC8641_DDR_SDRAM_WRTORD_VAL 1
#define DDR_SDRAM_WR_DATA_DELAY_VAL  0x3
#define DDR_SDRAM_ACSM_VAL           0
#define DDR_SDRAM_CPO_VAL            0x0
#define DDR_SDRAM_SDMODE_VAL         0x0062
#define DDR_SDRAM_ESDMODE_VAL        0x0000
#define DDR_SDRAM_2T_EN_VAL          1
#define DDR_SDRAM_NCAP_VAL           0
#define DDR_SDRAM_DYN_PWR_VAL        0
#define DDR_SDRAM_SREN_VAL           1
#define DDR_SDRAM_SBET_VAL           0xFF
#define INCLUDE_ECC
#undef  LOCAL_MEM_AUTOSIZE

/* memory configuration */

#ifdef _WRS_VX_AMP
#   define LOCAL_MEM_SIZE          0x10000000      /* 256 M */
#else /* _WRS_VX_AMP */
#   define LOCAL_MEM_SIZE          0x20000000      /* 512 M */
#endif /* _WRS_VX_AMP */

/* External Interrupt Sources */

#define EPIC_CCSROFF            0x40000                  /* EUMBBAR of EPIC  */
#define EPIC_TM_CTRL            (EPIC_CCSROFF + 0x01300) /* Timer Control */
#define EPIC_TM0_BASE_COUNT_REG (EPIC_CCSROFF + 0x01110) /* Gbl TM0 Base Count*/
#define EPIC_TM0_CUR_COUNT_REG  (EPIC_CCSROFF + 0x01100) /* Gbl TM0 Cur. Count*/

/* 12 External Interrupts */

#define EPIC_EXT_IRQ0_INT_NUM       0
#define EPIC_EXT_IRQ0_INT_VEC       (EPIC_EXT_IRQ0_INT_NUM + EPIC_VEC_EXT_IRQ0)
#define EPIC_EXT_IRQ1_INT_NUM       1
#define EPIC_EXT_IRQ1_INT_VEC       (EPIC_EXT_IRQ1_INT_NUM + EPIC_VEC_EXT_IRQ0)
#define EPIC_EXT_IRQ2_INT_NUM       2
#define EPIC_EXT_IRQ2_INT_VEC       (EPIC_EXT_IRQ2_INT_NUM + EPIC_VEC_EXT_IRQ0)
#define EPIC_EXT_IRQ3_INT_NUM       3
#define EPIC_EXT_IRQ3_INT_VEC       (EPIC_EXT_IRQ3_INT_NUM + EPIC_VEC_EXT_IRQ0)
#define EPIC_EXT_IRQ4_INT_NUM       4
#define EPIC_EXT_IRQ4_INT_VEC       (EPIC_EXT_IRQ4_INT_NUM + EPIC_VEC_EXT_IRQ0)
#define EPIC_EXT_IRQ5_INT_NUM       5
#define EPIC_EXT_IRQ5_INT_VEC       (EPIC_EXT_IRQ5_INT_NUM + EPIC_VEC_EXT_IRQ0)
#define EPIC_EXT_IRQ6_INT_NUM       6
#define EPIC_EXT_IRQ6_INT_VEC       (EPIC_EXT_IRQ6_INT_NUM + EPIC_VEC_EXT_IRQ0)
#define EPIC_EXT_IRQ7_INT_NUM       7
#define EPIC_EXT_IRQ7_INT_VEC       (EPIC_EXT_IRQ7_INT_NUM + EPIC_VEC_EXT_IRQ0)
#define EPIC_EXT_IRQ8_INT_NUM       8
#define EPIC_EXT_IRQ8_INT_VEC       (EPIC_EXT_IRQ8_INT_NUM + EPIC_VEC_EXT_IRQ0)
#define EPIC_EXT_IRQ9_INT_NUM       9
#define EPIC_EXT_IRQ9_INT_VEC       (EPIC_EXT_IRQ9_INT_NUM + EPIC_VEC_EXT_IRQ0)
#define EPIC_EXT_IRQ10_INT_NUM      10
#define EPIC_EXT_IRQ10_INT_VEC      (EPIC_EXT_IRQ10_INT_NUM + EPIC_VEC_EXT_IRQ0)
#define EPIC_EXT_IRQ11_INT_NUM      11
#define EPIC_EXT_IRQ11_INT_VEC      (EPIC_EXT_IRQ11_INT_NUM + EPIC_VEC_EXT_IRQ0)


#define PCIE1_INT0_VEC      (EPIC_EXT_IRQ0_INT_VEC)
#define PCIE1_INT1_VEC      (EPIC_EXT_IRQ1_INT_VEC)
#define PCIE1_INT2_VEC      (EPIC_EXT_IRQ2_INT_VEC)
#define PCIE1_INT3_VEC      (EPIC_EXT_IRQ3_INT_VEC)
#define PCIE2_INT0_VEC      (EPIC_EXT_IRQ4_INT_VEC)
#define PCIE2_INT1_VEC      (EPIC_EXT_IRQ5_INT_VEC)
#define PCIE2_INT2_VEC      (EPIC_EXT_IRQ6_INT_VEC)
#define PCIE2_INT3_VEC      (EPIC_EXT_IRQ7_INT_VEC)
#define NAND_FLASH__INT_VEC (EPIC_EXT_IRQ8_INT_VEC)
#define DS1375_INT_VEC      (EPIC_EXT_IRQ9_INT_VEC)
#define TEMP_INT_VEC        (EPIC_EXT_IRQ9_INT_VEC)
#define ABORT_INT_VEC       (EPIC_EXT_IRQ9_INT_VEC)
#define ETH_PHY_INT_VEC     (EPIC_EXT_IRQ10_INT_VEC)
#define QUART_INT_VEC       (EPIC_EXT_IRQ11_INT_VEC)
#define EXT_TIMER_INT_VEC   (EPIC_EXT_IRQ11_INT_VEC)

/*
 * PCI interrupt assignments
 *
 */

#define INT_LINES \
    { \
        { PCIE1_INT0_VEC,PCIE1_INT1_VEC,PCIE1_INT2_VEC,PCIE1_INT3_VEC }, \
        { PCIE1_INT1_VEC,PCIE1_INT2_VEC,PCIE1_INT3_VEC,PCIE1_INT0_VEC }, \
        { PCIE1_INT2_VEC,PCIE1_INT3_VEC,PCIE1_INT0_VEC,PCIE1_INT1_VEC }, \
        { PCIE1_INT3_VEC,PCIE1_INT0_VEC,PCIE1_INT1_VEC,PCIE1_INT2_VEC }, \
        { PCIE2_INT0_VEC,PCIE2_INT1_VEC,PCIE2_INT2_VEC,PCIE2_INT3_VEC }, \
        { PCIE2_INT1_VEC,PCIE2_INT2_VEC,PCIE2_INT3_VEC,PCIE2_INT0_VEC }, \
        { PCIE2_INT2_VEC,PCIE2_INT3_VEC,PCIE2_INT0_VEC,PCIE2_INT1_VEC }, \
        { PCIE2_INT3_VEC,PCIE2_INT0_VEC,PCIE2_INT1_VEC,PCIE2_INT2_VEC }, \
    }

#define EEBACR_STREAM_DIS      0x0000000b
#define EEBPCR_CPU_ENB_HIGHEST 0x01000002

/* Tsi148 (Tempe) PCI ID */

#define PCI_ID_TEMPE           0x014810e3  /* Id for Tempe VME-PCI Brdg */

/* USB */

#define PCI_ID_USB             0x00351033   /* USB controller device/vendor ID */
#define PCI_CONFIG_EXT2_OFFSET 0xE4
#define HIGH_CLOCK_SPD_SEL     0x20

/* VxWorks AuxClock Support */

#ifdef INCLUDE_AUX_CLK                  /* auxClk depends on global timers */
#   define INCLUDE_MPC8641_TIMERS
#endif /* INCLUDE_AUX_CLK */

/* clock rates */

#define SYS_CLK_RATE_MIN        1       /* minimum system clock rate */
#define SYS_CLK_RATE_MAX        8000    /* maximum system clock rate */
#define AUX_CLK_RATE_MIN        1       /* minimum auxiliary clock rate */
#define AUX_CLK_RATE_MAX        50000  /* maximum auxiliary clock rate */

/* Boot Image Programming Support */

#ifdef INCLUDE_BOOT_IMAGE_PROGRAM
#   define STANDALONE_NET
#endif /* INCLUDE_BOOT_IMAGE_PROGRAM */

/*
 * sysPhysMemDesc[] dummy entries:
 * these create space for updating sysPhysMemDesc table at a later stage
 * mainly to provide plug and play
 */

#define DUMMY_PHYS_ADDR         -1
#define DUMMY_VIRT_ADDR         -1
#define DUMMY_LENGTH            -1
#define DUMMY_INIT_STATE_MASK   -1
#define DUMMY_INIT_STATE        -1

#define DUMMY_MMU_ENTRY     {               \
                            (VIRT_ADDR) DUMMY_VIRT_ADDR,    \
                            (PHYS_ADDR) DUMMY_PHYS_ADDR,    \
                            (UINT32)DUMMY_LENGTH,       \
                            (UINT32)DUMMY_INIT_STATE_MASK,  \
                            (UINT32)DUMMY_INIT_STATE        \
                            }

/* Vital Product Data */

#ifdef VPD_ERRORS_NONFATAL
#   define  DEFAULT_BSP_ERROR_BEHAVIOR  CONTINUE_EXECUTION
#else /* VPD_ERRORS_NONFATAL */
#   define  DEFAULT_BSP_ERROR_BEHAVIOR  EXIT_TO_SYSTEM_MONITOR
#endif /* VPD_ERRORS_NONFATAL */


#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif /* __INCmv7100h */
