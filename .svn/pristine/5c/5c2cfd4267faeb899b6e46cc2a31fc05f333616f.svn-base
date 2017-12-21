/* sysLib.c - Freescale MPC8313ERDB board system-dependent library */

/*
 * Copyright (c) 2007-2010, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01h,10jun13,y_y  disable all timer in sysToMonitor. (WIND00392055)
01g,30apr10,dhy  delete _MMU_UBAT_VP attribute(WIND00162540)
01f,21may09,wap  Work around SDA relocation issue with firmware binary file
01e,07may09,wap  Set up eTSEC interrupt vectors correctly for rev 2 silicon
                 (WIND00154422) use compiled-in Vitesse ethernet switch
                 firmware instead of reading it from flash
01d,24jul08,l_g  replace vxbDelay with local implement
01c,03mar08,z_l  updated to vxWorks6.6
01b,18sep07,l_g  add support of usb target and nand boot
01a,26jun07,l_g  initial creation, based on ads834x
*/

/*
 * DESCRIPTION
 *
 * This library provides board-specific routines.
 *
 */

/* includes */

#include <vxWorks.h>
#include <vme.h>
#include <memLib.h>
#include <cacheLib.h>
#include <sysLib.h>
#include "config.h"
#include <string.h>
#include <intLib.h>
#include <logLib.h>
#include <stdio.h>
#include <taskLib.h>
#include <vxLib.h>
#include <tyLib.h>
#include <arch/ppc/mmu603Lib.h>
#include <arch/ppc/vxPpcLib.h>
#include <private/vmLibP.h>
#include <miiLib.h>

/* globals */

/*
 * sysBatDesc[] is used to initialize the block address translation (BAT)
 * registers within the PowerPC 603/604 MMU.  BAT hits take precedence
 * over Page Table Entry (PTE) hits and are faster.  Overlap of memory
 * coverage by BATs and PTEs is permitted in cases where either the IBATs
 * or the DBATs do not provide the necessary mapping (PTEs apply to both
 * instruction AND data space, without distinction).
 *
 * The primary means of memory control for VxWorks is the MMU PTE support
 * provided by vmLib and cacheLib.  Use of BAT registers will conflict
 * with vmLib support.  User's may use BAT registers for i/o mapping and
 * other purposes but are cautioned that conflicts with cacheing and mapping
 * through vmLib may arise.  Be aware that memory spaces mapped through a BAT
 * are not mapped by a PTE and any vmLib() or cacheLib() operations on such
 * areas will not be effective, nor will they report any error conditions.
 *
 * Note: BAT registers CANNOT be disabled - they are always active.
 * For example, setting them all to zero will yield four identical data
 * and instruction memory spaces starting at local address zero, each 128KB
 * in size, and each set as write-back and cache-enabled.  Hence, the BAT regs
 * MUST be configured carefully.
 *
 * With this in mind, it is recommended that the BAT registers be used
 * to map LARGE memory areas external to the processor if possible.
 * If not possible, map sections of high RAM and/or PROM space where
 * fine grained control of memory access is not needed.  This has the
 * beneficial effects of reducing PTE table size (8 bytes per 4k page)
 * and increasing the speed of access to the largest possible memory space.
 * Use the PTE table only for memory which needs fine grained (4KB pages)
 * control or which is too small to be mapped by the BAT regs.
 *
 * All BATs point to PROM/FLASH memory so that end customer may configure
 * them as required.
 *
 * [Ref: chapter 7, PowerPC Microprocessor Family: The Programming Environments]
 */

UINT32 sysBatDesc [2 * (_MMU_NUM_IBAT + _MMU_NUM_DBAT + _MMU_NUM_EXTRA_IBAT + _MMU_NUM_EXTRA_DBAT)] =
    {
    /* I BAT 0 */

    ((FLASH_BASE_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_8M |
    _MMU_UBAT_VS),
    ((FLASH_BASE_ADRS & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
    _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),

    /* I BAT 1 */

    ((LOCAL_MEM_LOCAL_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_128M |
    _MMU_UBAT_VS),
    ((LOCAL_MEM_LOCAL_ADRS & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
    _MMU_LBAT_MEM_COHERENT),

    /* I BAT 2 */

    0, 0,

    /* I BAT 3 */

    0, 0,

    /* D BAT 0 */

    ((CS_LED & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_128K |
     _MMU_UBAT_VS),
    ((CS_LED & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
     _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),

    /* D BAT 1 */

    ((FLASH_BASE_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_8M |
     _MMU_UBAT_VS),
    ((FLASH_BASE_ADRS & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
     _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),
#ifdef INCLUDE_PCI
    /* D BAT 2 - PCI 1 */

    ((PCI1_LAW_BASE & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_256M |
     _MMU_UBAT_VS),
    ((PCI1_LAW_BASE & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
     _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),

    /* D BAT 3 - PCI 2 */

    ((PCI2_LAW_BASE & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_256M |
     _MMU_UBAT_VS),
    ((PCI2_LAW_BASE & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
      _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),
#else
    /* D BAT 2 - PCI 1 */

    0, 0,

    /* D BAT 3 - PCI 2 */

    0, 0,
#endif
    /*
     * These entries are for the I/D BATs (4-7)
     * They should be defined in the following order.
     * IBAT4U,IBAT4L,IBAT5U,IBAT5L,IBAT6U,IBAT6L,IBAT7U,IBAT7L,
     * DBAT4U,DBAT4L,DBAT5U,DBAT5L,DBAT6U,DBAT6L,DBAT7U,DBAT7L,
     */

    /* I BAT 4 */

    0, 0,

    /* I BAT 5 */

    0, 0,

    /* I BAT 6 */

    0, 0,

    /* I BAT 7 */

    0, 0,

    /* D BAT 4 */

    0, 0,

    /* D BAT 5 */

    0, 0,

    /* D BAT 6 */

    0, 0,

    /* D BAT 7 */

    0, 0

    };

/*
 * mmuPcpBatInitMPC7x5 initializes the standard 4 (0-3) I/D BATs &
 * the additional 4 (4-7) I/D BATs present on the MPC74[45]5 & MPC83xx.
 */

/* MPC7x5 and 83xx BAT Support */

IMPORT void mmuPpcBatInitMPC7x5(UINT32 *pSysBatDesc);

IMPORT FUNCPTR _pSysBatInitFunc;

/*
 * sysPhysMemDesc[] is used to initialize the Page Table Entry (PTE) array
 * used by the MMU to translate addresses with single page (4k) granularity.
 * PTE memory space should not, in general, overlap BAT memory space but
 * may be allowed if only Data or Instruction access is mapped via BAT.
 *
 * Address translations for local RAM, memory mapped PCI bus, the Board Control and
 * Status registers, the MPC834X Internal Memory Map, and local FLASH RAM are set here.
 *
 * PTEs are held, strangely enough, in a Page Table.  Page Table sizes are
 * integer powers of two based on amount of memory to be mapped and a
 * minimum size of 64 kbytes.  The MINIMUM recommended Page Table sizes
 * for 32-bit PowerPCs are:
 *
 * Total mapped memory        Page Table size
 * -------------------        ---------------
 *        8 Meg                 64 K
 *       16 Meg                128 K
 *       32 Meg                256 K
 *       64 Meg                512 K
 *      128 Meg                  1 Meg
 *          .                    .
 *          .                    .
 *          .                    .
 *
 * [Ref: chapter 7, PowerPC Microprocessor Family: The Programming Environments]
 *
 */

PHYS_MEM_DESC sysPhysMemDesc [] =
    {

    {
    (VIRT_ADDR) LOCAL_MEM_LOCAL_ADRS,
    (PHYS_ADDR) LOCAL_MEM_LOCAL_ADRS,
    RAM_LOW_ADRS,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
    MMU_ATTR_CACHE_GUARDED
    },

    {
    (VIRT_ADDR) LOCAL_MEM_LOCAL_ADRS + RAM_LOW_ADRS,
    (PHYS_ADDR) LOCAL_MEM_LOCAL_ADRS + RAM_LOW_ADRS,
    LOCAL_MEM_SIZE - RAM_LOW_ADRS,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_DEFAULT |
    MMU_ATTR_CACHE_COHERENCY
    },

    /* MPC8313 Register Memory Map */

    {
    (VIRT_ADDR) CCSBAR,
    (PHYS_ADDR) CCSBAR,
    CCSBAR_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
    MMU_ATTR_CACHE_GUARDED
    },

    /* VSC7385 */

    {
    (VIRT_ADDR) CS_VSC7385,
    (PHYS_ADDR) CS_VSC7385,
    CS_VSC7385_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
    MMU_ATTR_CACHE_GUARDED
    },

    /* NAND controller */

    {
    (VIRT_ADDR) NAND_CONTROLLER_BASE,
    (PHYS_ADDR) NAND_CONTROLLER_BASE,
    NAND_CONTROLLER_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
    MMU_ATTR_CACHE_GUARDED
    }

    };

int sysPhysMemDescNumEnt = NELEMENTS (sysPhysMemDesc);

int   sysBus        = BUS;            /* system bus type (VME_BUS, etc) */
int   sysCpu        = CPU;            /* system CPU type (PPC603) */
char *sysBootLine   = BOOT_LINE_ADRS; /* address of boot line */
char *sysExcMsg     = EXC_MSG_ADRS;   /* catastrophic message area */
int   sysProcNum;                     /* processor number of this CPU */
BOOL  sysVmeEnable  = FALSE;          /* by default no VME */

/* forward declarations */

uint32_t sysDecGet(void);
UINT32 sysClkFreqGet();
void sysUsDelay(UINT32 delay);

#ifdef INCLUDE_FLASH
#   include "sysFlash.c"
#   include "nvRamToFlash.c"
#else
#   include <mem/nullNvRam.c>
#endif

#ifdef INCLUDE_VXBUS
    IMPORT void hardWareInterFaceInit();
    UINT32 sysClkFreqGet();
#   include <vxBusLib.h>
#   include <hwif/vxbus/vxBus.h>
#   include "hwconf.c"
#   ifdef INCLUDE_SIO_UTILS
        IMPORT void sysSerialConnectAll(void);
#   endif
    IMPORT void (*_vxb_delayRtn)(void);
#endif

#include "sysNet.c"

#ifdef INCLUDE_RTC
#   include "sysMotI2c.c"
#   include "sysMpc8313I2c.c"
#   include "sysRtc.c"
#endif

#include "sysNandFlash.c"

void sysLedInit(void);

#define ZERO                0
#define SYS_MODEL           "Freescale MPC8313E"
#define SYS_MODEL_UNKNOWN   "Not E300 C3 CORE - device unknown"

/* Set ACR to park on PCI bus */

#define M83XX_ACR_PARKM_PCI 0x1

/* Clock configuration */

typedef struct
    {
    UINT32 spmf;
    UINT32 corepll;
    UINT32 clkin;
    UINT32 csbfreq;
    UINT32 corefreq;
    } CLK_TABLE;

LOCAL CLK_TABLE clkConfTable[] =
    {
    {0x2 ,0x02,66 ,133 ,133},
    {0x3 ,0x22,66 ,200 ,200},
    {0x4 ,0x42,66 ,266 ,266},
    {0x2 ,0x04,66 ,133 ,266},
    {0x3 ,0x24,66 ,200 ,400},
    {0x5, 0x04,33 ,166 ,333},  /* RevA3 board */
    {0}
    };

/* variant to calculate and save system clock configuration */

UINT32 csb      = 0;
UINT32 coreFreq = 0;
UINT32 lbiu     = 0;
UINT32 ddr      = 0;
UINT32 spmf     = 0;
UINT32 clkDiv   = 0;
UINT32 corePll  = 0;

IMPORT UINT8 _binary_vsc2bin_start[];
IMPORT UINT8 _binary_vsc2bin_end[];

/*******************************************************************************
*
* vsc7385Init - initialize VSC7385 L2 switch
*
* This routine load firmware routine to VSC7385.
*
* RETURNS: OK, or ERROR.
*/

STATUS vsc7385Init()
    {
    UINT32 size;
    UINT32 i;
    volatile UINT32 value;
    int retVal                  = OK;
    volatile UINT32 * RESET     = (volatile UINT32 *)(CS_VSC7385 + 0x1c050);
    volatile UINT32 * CTRL      = (volatile UINT32 *)(CS_VSC7385 + 0x1c040);
    volatile UINT32 * ADDR      = (volatile UINT32 *)(CS_VSC7385 + 0x1c044);
    volatile UINT32 * DATA      = (volatile UINT32 *)(CS_VSC7385 + 0x1c048);
    volatile UINT32 * REG_MAP   = (volatile UINT32 *)(CS_VSC7385 + 0x1c070);
    volatile UINT8  * VSC7385_image;

    size = (UINT32)&_binary_vsc2bin_end - (UINT32)&_binary_vsc2bin_start;
    VSC7385_image = _binary_vsc2bin_start;

    *RESET = 3;
    sysUsDelay(200);

    value = (
         (1 << 7) |
         (1 << 3) |
         (1 << 2) |
         (1 << 1) |
         (0 << 0)
         );
    *CTRL = value;
    sysUsDelay(20);

    *REG_MAP = 1;
    sysUsDelay(20);

    *ADDR = 0x0;
    sysUsDelay(20);

    for (i = 0; i < size; i++)
        {
        *DATA = VSC7385_image[i];
        sysUsDelay(20);
        }

    *ADDR = 0x0;

    for (i = 0; i < size; i++)
        {
        sysUsDelay(20);
        value = *DATA;
        if (value != VSC7385_image[i])
            {
            retVal = ERROR;
            break;
            }
        }

    value = (
         (0 << 7) |
         (1 << 3) |
         (0 << 2) |
         (1 << 1) |
         (1 << 0)
         );
    *CTRL = value;

    return (retVal);

    }

/*******************************************************************************
*
* sysModel - return the model name of the CPU board
*
* This routine returns the model name of the CPU board.
*
* RETURNS: A pointer to the string.
*/

char * sysModel (void)
    {
    char * model;

    if ((vxPvrGet() & 0xffff0000) == PVR_E300C3)
        {
        UINT32 temp;

        temp = vxSvrGet();

        switch(temp & 0xffff0000)
            {
            case SVR_MPC8313E_0:
            case SVR_MPC8313E_1:
                model = SYS_MODEL;
                break;
            default:
                model = "Freescale E300C3 - Unknown version";
                break;
            }
        }
    else
        {
        model = SYS_MODEL_UNKNOWN;
        }

    return(model);

    }

/*******************************************************************************
*
* sysBspRev - return the bsp version with the revision eg 2.0/<x>
*
* This function returns a pointer to a bsp version with the revision.
* for eg. 2.0/<x>. BSP_REV defined in config.h is concatenated to
* BSP_VERSION and returned.
*
* RETURNS: A pointer to the BSP version/revision string.
*/

char * sysBspRev (void)
    {
    return (BSP_VERSION BSP_REV);
    }

/*******************************************************************************
*
* sysHwMemInit - initialize and configure system memory.
*
* This routine is called before sysHwInit(). It performs memory auto-sizing
* and updates the system's physical regions table, `sysPhysRgnTbl'. It may
* include the code to do runtime configuration of extra memory controllers.
*
* NOTE: This routine should not be called directly by the user application.  It
* cannot be used to initialize interrupt vectors.
*
* RETURNS: N/A
*/

void sysHwMemInit (void)
    {

    /* Call sysPhysMemTop() to do memory autosizing if available */

    sysPhysMemDesc[1].len = (UINT)(sysPhysMemTop () -  RAM_LOW_ADRS);
    }

/*******************************************************************************
*
* DelayForVxBus - replace the vxbDelay for vxBus
*
* This routine replace the vxbDelay for vxBus
*
* RETURNS: N/A
*/

LOCAL void DelayForVxBus
    (
    UINT32 usec
    )
    {
    int i, k;
    volatile int a, b;

    for (i = 0; i < usec; i++)
        {
        for(k = 0; k < 0x100; k++)
            {
            a = 0;
            b = a;
            }
        }

    return;
    }


/*******************************************************************************
*
* sysHwInit - initialize the system hardware
*
* This routine initializes features of board. It sets up the control registers,
* initializes various devices if they are present.
*
* NOTE: This routine should not be called directly by the user.
*
* RETURNS: N/A
*/

void sysHwInit (void)
    {
#ifdef INCLUDE_VXBUS
    UINT32 chipRev;
    HCF_DEVICE * pDev;
    struct hcfResource * pRes;
    struct intrCtlrInputs * pInputs;
    int i;
#endif

    /* This is a workaround for errata PCI9 */

#ifdef INCLUDE_PCI
    *M83XX_ACR(CCSBAR) |= M83XX_ACR_PARKM_PCI;  /* Park on PCI */
#else
    *M83XX_ACR(CCSBAR) &= ~M83XX_ACR_PARKM_PCI; /* Park PPC core */
#endif

    /* set pointer to BAT initialization functions */

    _pSysBatInitFunc = (FUNCPTR) mmuPpcBatInitMPC7x5;

    /* Initialize Machine check pin */

    vxHid0Set(0x80000000);
    vxMsrSet(vxMsrGet() | 0x00000002);

    /* HID2 [IFEB / IFEC / EBQS / EBPX] See MPC8349 for details */
    /* This will make use of under the hood architecture features */

    vxHid2Set(vxHid2Get() | 0x04e00000);

    /* Set TSEC/CSB 1:1 clk ratio */

    *M83XX_SCCR(CCSBAR) &= 0x7fffffff ;

    *M83XX_SCCR(CCSBAR) |= 0x70000000 ;

    /* Enable the e300 core timebase */

    *M83XX_SPCR(CCSBAR) |= M83XX_SPCR_TBEN;

    (void)sysClkFreqGet();

    (void)vsc7385Init();

#ifdef INCLUDE_VXBUS
    /*
     * The Rev A versions of the rdb8313 board use rev 1 of
     * the MPC8313E silicon, whereas the Rev B (and later?)
     * versions use rev 2 of the silicon. The rev 2 chip
     * corrects a defect from rev 1 where the interrupts for
     * the eTSEC ports were swapped around.
     *
     * The following workaround allows us to support both
     * chips with a single binary. We assume the rev 1 mapping,
     * but we check the silicon revision here, and if the
     * chip is rev 2 or later, we correct the entries in the
     * VxBus device list. Note that this assumes that the
     * layout of the quiccIntCtlrInputs[], motEtsecHEnd0Resources[]
     * and motEtsecHEnd1Resources[] arrays in hwconf.c will not
     * be changed. If for some reason the fields in these
     * structures are re-arranged, the following code will
     * need to be updated to match.
     */

    chipRev = (vxSvrGet() & 0x000000F0) >> 4;
    if (chipRev >= 2)
        {
        pInputs = quiccIntCtlrInputs;
        pInputs[2].inputPin = INUM_REV2_TSEC1_TX;
        pInputs[3].inputPin = INUM_REV2_TSEC1_RX;
        pInputs[4].inputPin = INUM_REV2_TSEC1_ERR;
        pInputs[5].inputPin = INUM_REV2_TSEC2_TX;
        pInputs[6].inputPin = INUM_REV2_TSEC2_RX;
        pInputs[7].inputPin = INUM_REV2_TSEC2_ERR;
        for (i = 0; i < hcfDeviceNum; i++)
            {
            pDev = &hcfDeviceList[i];
            if (!strcmp (pDev->devName, "motetsec") &&
                pDev->devUnit == 0)
                {
                pRes = (struct hcfResource *)pDev->pResource;
                pRes[1].u.integer = INUM_REV2_TSEC1_TX;
                pRes[2].u.integer = INUM_REV2_TSEC1_TX;
                pRes[3].u.integer = INUM_REV2_TSEC1_RX;
                pRes[4].u.integer = INUM_REV2_TSEC1_RX;
                pRes[5].u.integer = INUM_REV2_TSEC1_ERR;
                pRes[6].u.integer = INUM_REV2_TSEC1_ERR;
                }
            if (!strcmp (pDev->devName, "motetsec") &&
                pDev->devUnit == 1)
                {
                pRes = (struct hcfResource *)pDev->pResource;
                pRes[1].u.integer = INUM_REV2_TSEC2_TX;
                pRes[2].u.integer = INUM_REV2_TSEC2_TX;
                pRes[3].u.integer = INUM_REV2_TSEC2_RX;
                pRes[4].u.integer = INUM_REV2_TSEC2_RX;
                pRes[5].u.integer = INUM_REV2_TSEC2_ERR;
                pRes[6].u.integer = INUM_REV2_TSEC2_ERR;
                }
            }
        }

    hardWareInterFaceInit();
    _vxb_delayRtn = (void *)DelayForVxBus;
#endif /* INCLUDE_VXBUS */

    /*
     * put correct memory size in sysPhysMemDesc[1].len for
     * size of local memory
     */

    sysHwMemInit();

    CACHE_PIPE_FLUSH();

    /* Initialize I2C */

#ifdef INCLUDE_RTC
    i2cDrvInit(0,0);
#endif

#ifdef  FORCE_DEFAULT_BOOT_LINE
    strncpy (sysBootLine, DEFAULT_BOOT_LINE, strlen(DEFAULT_BOOT_LINE) + 1);
#endif /* FORCE_DEFAULT_BOOT_LINE */
    }

/*******************************************************************************
*
* sysGetPeripheralBase - provide CCSRBAR from security engine drivers
*
* This routine provides CCSRBAR address from security engine drivers.
*
* RETURNS: CCSBAR
*
* ERRNO
*/

UINT32 sysGetPeripheralBase(void)
    {
    return (CCSBAR);
    }

/*******************************************************************************
*
* sysClkFreqGet - calculate clock
*
* This routine calculate and save clock configuration
*
* RETURNS: CSB clock frequenty
*
* ERRNO N/A
*/

UINT32 sysClkFreqGet(void)
    {
    int     loop;
    UINT32  tempVal;

    /* Read dip switches */

    tempVal = *M83XX_SPMR(CCSBAR);
    lbiu    = M83XX_SPMR_LBIU_VAL(tempVal);
    ddr     = M83XX_SPMR_DDR_VAL(tempVal);
    spmf    = M83XX_SPMR_SPMF_VAL(tempVal);
    clkDiv  = M83XX_SPMR_CLK_DIV(tempVal);
    corePll = M83XX_SPMR_CLK_PLL(tempVal);

    /* Assume 66MHz oscillator clk */

    for (loop = 0; clkConfTable[loop].spmf != 0; loop++)
        {
        if(spmf == clkConfTable[loop].spmf)
            {
            if (corePll == clkConfTable[loop].corepll)
                {
                coreFreq = clkConfTable[loop].corefreq;
                csb = clkConfTable[loop].csbfreq;
                break;
                }
            }
        }

    /* If loop drops out the bottom then use default BSP value for CSB */

    if (clkConfTable[loop].spmf == 0)
        {
        csb = SYS_CLK_FREQ;
        coreFreq = (csb * (corePll & 3)) / 2;
        }
    else
        {
        csb = csb * 1000000;
        coreFreq = coreFreq * 1000000;
        }

    return (csb);
    }

/*******************************************************************************
*
* sysPhysMemTop - get the address of the top of physical memory
*
* This routine returns the address of the first missing byte of memory,
* which indicates the top of memory.
*
* Determine installed memory by reading memory control registers
* and calculating if one or 2 chip selects are used for SDRAM.
* Use the address mask and valid bit to determine each bank size.
*
* RETURNS: The address of the top of physical memory.
*
* SEE ALSO: sysMemTop()
*/

char * sysPhysMemTop (void)
    {
    LOCAL char * physTop = NULL;

    physTop = (char *)(LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE);

    return (physTop) ;
    }

/*******************************************************************************
*
* sysMemTop - get the address of the top of VxWorks memory
*
* This routine returns a pointer to the first byte of memory not
* controlled or used by VxWorks.
*
* The user can reserve memory space by defining the macro USER_RESERVED_MEM
* in config.h.  This routine returns the address of the reserved memory
* area.  The value of USER_RESERVED_MEM is in bytes.
*
* RETURNS: The address of the top of VxWorks memory.
*/

char * sysMemTop (void)
    {
    LOCAL char * memTop = NULL;

    if (memTop == NULL)
        {
        memTop = sysPhysMemTop () - USER_RESERVED_MEM;

#ifdef INCLUDE_EDR_PM

        /* account for ED&R persistent memory */

        memTop = memTop - PM_RESERVED_MEM;
#endif
        }

    return memTop;
    }

/*******************************************************************************
*
* sysToMonitor - transfer control to the ROM monitor
*
* This routine transfers control to the ROM monitor.  Normally, it is called
* only by reboot()--which services ^X--and bus errors at interrupt level.
* However, in some circumstances, the user may wish to introduce a
* <startType> to enable special boot ROM facilities.
*
* RETURNS: Does not return.
*/

STATUS sysToMonitor
    (
    int startType    /* parameter passed to ROM to tell it how to boot */
    )
    {
    FUNCPTR pRom = (FUNCPTR) (ROM_TEXT_ADRS + 8);    /* Warm reboot */

    intLock();

    cacheDisable(INSTRUCTION_CACHE);
    cacheDisable(DATA_CACHE);

    sysClkDisable();

#ifdef INCLUDE_AUX_CLK
    sysAuxClkDisable();
#endif

#ifdef INCLUDE_TIMESTAMP
    sysTimestampDisable();
#endif

#if defined(INCLUDE_USB)

    /* Make sure the USB controller is stopped. */

    *M83XX_USBCMD(CCSBAR) &= LONGSWAP(~0x1) ;
#endif

    vxMsrSet (0);

    (*pRom) (startType);    /* jump to bootrom entry point */

    return (OK);    /* in case we ever continue from ROM monitor */
    }

/*******************************************************************************
*
* sysHwInit2 - additional system configuration and initialization
*
* This routine connects system interrupts and does any additional
* configuration necessary.
*
* RETURNS: N/A
*/

void sysHwInit2 (void)
    {
    LOCAL BOOL configured = FALSE;

    if (!configured)
        {
#ifdef  INCLUDE_VXBUS
        vxbDevInit();
#endif /* INCLUDE_VXBUS */

#ifdef INCLUDE_SIO_UTILS
        sysSerialConnectAll();
#endif /* INCLUDE_SIO_UTILS */

        configured = TRUE;

#ifdef  INCLUDE_VXBUS
        taskSpawn("devConnect",0,0,10000,vxbDevConnect,0,0,0,0,0,0,0,0,0,0);
#endif /* INCLUDE_VXBUS */
        }
    }

/*******************************************************************************
*
* sysUsDelay - delay for at least x microseconds.
*
* Use the timebase register, running at csb, to delay at least
* the requested number of microseconds.
*
* RETURNS: N/A.
*
* ERRNO
*
* \NOMANUAL
*
* \INTERNAL
*
* Do not call before sysHwInit determines csb.
*/

void sysUsDelay
    (
    UINT32 delay
    )
    {
    UINT32      hi;         /* current timebase upper 32 bits */
    UINT32      lo;         /* current timebase lower 32 bits */
    UINT32      ticks;      /* number of ticks needed for delay */
    FAST UINT32 target_hi;  /* target timebase upper 32 bits */
    FAST UINT32 target_lo;  /* target timebase lower 32 bits */

    /* get start time */

    vxTimeBaseGet(&hi, &lo);

    /*
     * Calculate number of ticks for desired elapsed time. Note that the time
     * base register is incremented once every four bus clock cycles
     */

    ticks = delay * ((csb + 1000000) / 1000000 / 4);

    /* set target hi and lo, and account for rollover */

    target_lo = lo + ticks;                     /* may cause rollover */
    target_hi = hi + (target_lo < lo ? 1 : 0);  /* account for rollover */

    /* read timebase until target reached */

    do
        {
        vxTimeBaseGet(&hi, &lo);
        }
    while (hi < target_hi || (hi == target_hi && lo < target_lo));

    return;
    }

/*******************************************************************************
*
* sysMsDelay - delay for the specified amount of time (MS)
*
* This routine will delay for the specified amount of time by calling sysUsDelay
* with (delay*1000).
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void sysMsDelay
    (
    uint32_t        delay     /* length of time in MS to delay */
    )
    {
    sysUsDelay ( (UINT32) delay * 1000 );
    }

/*******************************************************************************
*
* sysDelay - Fixed 1ms delay. Just calls sysMsDelay
*
* sysDelay(void)
*
* RETURNS : NONE
*
*/

void sysDelay (void)
    {
    sysMsDelay (1);
    }

/*******************************************************************************
*
* sysProcNumGet - get the processor number
*
* This routine returns the processor number for the CPU board, which is
* set with sysProcNumSet().
*
* RETURNS: The processor number for the CPU board.
*
* SEE ALSO: sysProcNumSet()
*/

int sysProcNumGet (void)
    {
    return (sysProcNum);
    }

/*******************************************************************************
*
* sysProcNumSet - set the processor number
*
* This routine sets the processor number for the CPU board.  Processor numbers
* should be unique on a single backplane.
*
* Not applicable for the busless 8260Ads.
*
* RETURNS: N/A
*
* SEE ALSO: sysProcNumGet()
*
*/

void sysProcNumSet
    (
    int     procNum            /* processor number */
    )
    {
    sysProcNum = procNum;
    }

/*******************************************************************************
*
* sysLocalToBusAdrs - convert a local address to a bus address
*
* This routine gets the VMEbus address that accesses a specified local
* memory address.
*
* Not applicable for the 8260Ads
*
* RETURNS: ERROR, always.
*
* SEE ALSO: sysBusToLocalAdrs()
*/

STATUS sysLocalToBusAdrs
    (
    int     adrsSpace,  /* bus address space where busAdrs resides */
    char *  localAdrs,  /* local address to convert */
    char ** pBusAdrs    /* where to return bus address */
    )
    {
    return (ERROR);
    }

/*******************************************************************************
*
* sysBusToLocalAdrs - convert a bus address to a local address
*
* This routine gets the local address that accesses a specified VMEbus
* physical memory address.
*
* Not applicable for the 8260Ads
*
* RETURNS: ERROR, always.
*
* SEE ALSO: sysLocalToBusAdrs()
*/

STATUS sysBusToLocalAdrs
    (
    int      adrsSpace,   /* bus address space where busAdrs resides */
    char *   busAdrs,     /* bus address to convert */
    char **  pLocalAdrs   /* where to return local address */
    )
    {
    return (ERROR);
    }

/*******************************************************************************
*
* sysBusTas - test and set a location across the bus
*
* This routine does an atomic test-and-set operation across the backplane.
*
* Not applicable for the 8260Ads.
*
* RETURNS: FALSE, always.
*
* SEE ALSO: vxTas()
*/

BOOL sysBusTas
    (
    char *    adrs        /* address to be tested-and-set */
    )
    {
    return (FALSE);
    }

/*******************************************************************************
*
* sysBusClearTas - test and clear
*
* This routine is a null function.
*
* RETURNS: N/A
*/

void sysBusClearTas
    (
    volatile char * address    /* address to be tested-and-cleared */
    )
    {
    }

/*******************************************************************************
*
* sysLedInit - turn off all LEDs
*
* sysLedInit(void)
*
* RETURNS: N/A
*
*/

void sysLedInit(void)
    {
    volatile unsigned char *ptr = (volatile unsigned char *)CS_LED;

    /* turn off all LEDs */

    *ptr = 0xff;
    }

/*******************************************************************************
*
* sysTurnOnLED - Turn on LED
*
* This routine will turn on the LED
*
* RETURNS: N/A
*/
void sysTurnOnLED
    (
    unsigned char LED    /* bitmap of LED to turn on */
    )
    {
    volatile unsigned char *ptr = (volatile unsigned char *)CS_LED;

    *ptr = LED;

    }

#ifdef INCLUDE_PCI_BUS

LOCAL UCHAR sysPciIntRoute [NUM_PCI_SLOTS][4] =
    {
    {PCI_XINT4_LVL, PCI_XINT1_LVL, PCI_XINT2_LVL, PCI_XINT3_LVL}
    };

/*******************************************************************************
*
* sysPci1AutoconfigInclude - PCI autoconfig support routine
*
* RETURNS: OK or ERROR for the MPC106 or WINBOND devices.
*/

STATUS sysPci1AutoconfigInclude
    (
    PCI_SYSTEM * pSys,          /* PCI_SYSTEM structure pointer */
    PCI_LOC    * pLoc,          /* pointer to function */
    UINT       devVend          /* deviceID/vendorID of device */
    )
    {

    /* Don't want to auto configure the bridge */

    if ((pLoc->bus > 0) ||
        (pLoc->bus == 0 && pLoc->device == 0 && pLoc->function == 0))
        return(ERROR);

    return OK; /* Autoconfigure all devices */
    }


/*******************************************************************************
*
* sysPci1AutoconfigIntrAssign - PCI autoconfig support routine
*
* RETURNS: PCI interrupt line number given pin mask
*/

UCHAR sysPci1AutoconfigIntrAssign
    (
    PCI_SYSTEM * pSys,                  /* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,                     /* pointer to function */
    UCHAR pin                           /* contents of int pin register */
    )
    {
    UCHAR tmpChar = 0xff;

    /*
     * Ensure this is a resonable value for bus zero.
     * If OK, return INT level, else we return 0xff.
     */

    if((pin >= 1) && (pin < 5))
        {
        tmpChar = INUM_IRQ4 - sysPciIntRoute [0][pin - 1];
        }

    /* return the value to be assigned to the pin */

    return (tmpChar);

    }

#endif /* INCLUDE_PCI_BUS */

#ifdef INCLUDE_SIO_UTILS
#   define BSP_SERIAL_CHAN_GET  bspSerialChanGet
#else /* INCLUDE_SIO_UTILS */
#   define BSP_SERIAL_CHAN_GET  sysSerialChanGet
#endif /* INCLUDE_SIO_UTILS */

/*******************************************************************************
*
* BSP_SERIAL_CHAN_GET - get the serial channel
*
* This routine is called by the vxbus sio driver.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

SIO_CHAN *BSP_SERIAL_CHAN_GET
    (
    int channel
    )
    {
    return ((SIO_CHAN *)ERROR);
    }

#if defined(INCLUDE_USB) && defined(INCLUDE_EHCI)

/******************************************************************************
*
* sysM837xEhciInit - intializes the on-chip EHCI
*
* This routine initializes the USB MPH clock, configures the MPH pins and
* enables the ports.
*
* RETURNS: N/A
*
* ERRNO: none
*
*\NOMANUAL
*/

void sysM8313EhciInit()
    {

    /* USB/CSB 1:3 clk ratio */

    *M83XX_SCCR(CCSBAR)    |= 0x00300000;

    /* config phy */

    *M83XX_CONTROL(CCSBAR)  = 0x00000280;

    while( (*M83XX_CONTROL(CCSBAR) & 0x00020000) == 0 )
        ;

    /* host mode */

    *M83XX_USBMODE(CCSBAR) |= LONGSWAP(0x03) ;

    /* Make sure USBDR pins are available */

    *M83XX_SICRL(CCSBAR)    = 0x00000800 ;

    /* 256 MB */

    *M83XX_SNOOP1(CCSBAR)   = 0x0000001b;

    /* Ports Configuration */

    *M83XX_PORTSC1(CCSBAR)  = 0x00000010 ;

    /* Recommended values if not setting repeat mode */

    *M83XX_BURSTSIZE(CCSBAR) = LONGSWAP(0x00001010)  ;

    /* rd prefetch 32byte base on burst size 8 x 4 */

    *M83XX_SI_CTRL(CCSBAR)  = 0x0000000c;

    /* Recommended prio and age cnt threshold */

    *M83XX_PRI_CTRL(CCSBAR) = 0x00000001;
    *M83XX_AGE_CNT_THRESH(CCSBAR) = 0x00000040 ;

    /* enable phy */

    *M83XX_CONTROL(CCSBAR) |= 0x00000004;

    }

/*******************************************************************************
*
* usbOpAfterReset - operation after usb ehcd controller reset
*
* set MPC837x usb DR controller to host mode after reset.
*
*/

void usbOpAfterReset()
    {
    *M83XX_USBMODE(CCSBAR) |= LONGSWAP(0x03) ;
    }

#endif /* INCLUDE_USB && ... */
