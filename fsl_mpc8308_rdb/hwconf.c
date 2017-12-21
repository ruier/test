/* hwconf.c - Hardware configuration support module */

/*
 * Copyright (c) 2010, 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,20may11,sye  rename DRV_STORAGE_FSL_SDHC to DRV_STORAGE_SDHC. (WIND00240151)
01a,16jun10,syt  derived from version 01b of fsl_rdb8313/hwconf.c
*/

#include <vxWorks.h>
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/hwConf.h>
#include <hwif/util/vxbParamSys.h>
#include "config.h"
#include <hwif/vxbus/vxbIntrCtlr.h>
#include <drv/pci/pciAutoConfigLib.h>

#ifdef DRV_STORAGE_SDHC
#   include <h/storage/vxbSdMmcLib.h>
#   include <h/storage/vxbSdhcStorage.h>
#endif /* DRV_STORAGE_SDHC */

#if (defined(INCLUDE_EHCI) || defined(INCLUDE_FSL_TCD))
IMPORT void     sysEhciInit (void);
IMPORT void     usbOpAfterReset ();
IMPORT UINT32   vxbSwap32 (UINT32);
#endif /* INCLUDE_EHCI || INCLUDE_FSL_TCD */

#ifdef DRV_STORAGE_SDHC
IMPORT UINT32   sysSdhcClkFreqGet (void);
#endif /* DRV_STORAGE_SDHC */

#ifdef DRV_PCIBUS_M83XX_EX
IMPORT STATUS   sysPciExAutoconfigInclude ();
IMPORT UCHAR    sysPciExAutoconfigIntrAssign (PCI_SYSTEM *,PCI_LOC *,UCHAR);
#endif /* DRV_PCIBUS_M83XX_EX */

const SIU_INT_MASK quiccIntMask[QUICC_IVEC_MAX] =
    {
    {0x00000000, 0x00000000, 0x00000000},         /* Error(no interrupt) */
    {0x00000000, 0x00000000, 0x00000000},         /* PEX CNT */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0xFF000020, 0x30000000, 0x00000000},         /* UART1 */
    {0xFF000020, 0x30000000, 0x00000000},         /* UART2 */
    {0xFF000000, 0x00000000, 0x00000000},         /* SEC   */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0xFF000020, 0x30000000, 0x00000000},         /* I2C1 */
    {0xFF000020, 0x30000000, 0x00000000},         /* I2C2 */
    {0xFF000020, 0x30000000, 0x00000000},         /* SPI */
    {0xFF000020, 0x30000000, 0x00000000},         /* IRQ1 */
    {0xFF000020, 0x30000000, 0x00000000},         /* IRQ2 */
    {0xFF000020, 0x30000000, 0x00000000},         /* IRQ3 */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved **23**/
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* TSEC1 Tx */
    {0x00000000, 0x00000000, 0x00000000},         /* TSEC1 Rx */
    {0x00000000, 0x00000000, 0x00000000},         /* TSEC1 Err */
    {0x00000000, 0x00000000, 0x00000000},         /* TSEC2 Tx */
    {0x00000000, 0x00000000, 0x00000000},         /* TSEC2 Rx */
    {0x00000000, 0x00000000, 0x00000000},         /* TSEC2 Err */
    {0xFC000020, 0x30000000, 0x00000000},         /* USB DR */
    {0x00000020, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* IRQ0 ***48**/
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0xFF000020, 0x30000000, 0x00000000},         /* RTC SEC */
    {0xFF000020, 0x30000000, 0x00000000},         /* PIT */
    {0xFF000020, 0x00000000, 0x00000000},         /* PCI1 */
    {0xFF000020, 0x00000000, 0x00000000},         /* PCI2 */
    {0xFF000020, 0x30000000, 0x00000000},         /* RTC ALR */
    {0xFF000020, 0x30000000, 0x00000000},         /* MU */
    {0xFF000020, 0x30000000, 0x00000000},         /* SBA */
    {0xFF000020, 0x30000000, 0x00000000},         /* DMA */
    {0xFF000020, 0x30000000, 0x00000000},         /* GTM4 */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0xFF000020, 0x30000000, 0x00000000},         /* CPIO1 */
    {0xFF000020, 0x30000000, 0x00000000},         /* GPIO2 */
    {0xFF000020, 0x30000000, 0x00000000},         /* DDR */
    {0xFF000020, 0x30000000, 0x00000000},         /* LBC */
    {0xFF000020, 0x30000000, 0x00000000},         /* GTM2 */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0xFF000020, 0x30000000, 0x00000000},         /* PMC */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0xFF000020, 0x30000000, 0x00000000},         /* GTM3 */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0xFF000020, 0x30000000, 0x00000000},         /* GTM1 */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000}          /* Reserved */
    };

/* Specific to CPU  */

const UINT32    quiccInumToMaskPattern[QUICC_IVEC_MAX] =
    {
    0x00000000,         /* interrupt number 0: Error */
    0x00008000,         /* PEX CNT */
    0x00000000,         /* Reserved */
    0x00002000,         /* DMAC */
    0x00001000,         /* MSIR1 */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000080,         /* UART1 */
    0x00000040,         /* UART2 */
    0x00000000,         /* Reserved */
    0x00000010,         /* ETSEC1 1588 TIMER */
    0x00000008,         /* ETSEC2 1588 TIMER */
    0x00000004,         /* I2C1 */
    0x00000002,         /* I2C2 */
    0x00000001,         /* SPI */
    0x40000000,         /* IRQ1 */
    0x20000000,         /* IRQ2 */
    0x10000000,         /* IRQ3 */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved**/
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x80000000,         /* TSEC1 Tx */
    0x40000000,         /* TSEC1 Rx */
    0x20000000,         /* TSEC1 Err */
    0x10000000,         /* TSEC2 Tx */
    0x08000000,         /* TSEC2 Rx */
    0x04000000,         /* TSEC2 Err */
    0x02000000,         /* USB DR */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00200000,         /* eSDHC */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x80000000,         /* IRQ0 */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x80000000,         /* RTC SEC */
    0x40000000,         /* PIT */
    0x00000000,         /* Reserved */
    0x10000000,         /* MSIR0 */
    0x08000000,         /* RTC ALR */
    0x00000000,         /* Reserved */
    0x02000000,         /* SBA */
    0x00000000,         /* Reserved */
    0x00800000,         /* GTM4 */
    0x00000000,         /* Reserved */
    0x00200000,         /* GPIO */
    0x00000000,         /* Reserved */
    0x00080000,         /* DDR */
    0x00040000,         /* LBC */
    0x00020000,         /* GTM2 */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00004000,         /* MSIR2 */
    0x00002000,         /* MSIR3 */
    0x00000000,         /* Reserved */
    0x00000800,         /* GTM3 */
    0x00000000,         /* Reserved */
    0x00000200,         /* MSIR4 */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000020,         /* GTM1 */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000002,         /* DMAC_ERR */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000          /* Reserved */
    };

const struct intrCtlrInputs ppcIntCtlrInputs[] =
    {
    { 0, "quiccIntCtlr",    0, 0 },
    { 1, "ppcDecTimerDev",  0, 0 },
    };

const struct hcfResource ppcIntCtlr0Resources[] =
    {
    { VXB_REG_BASE,     HCF_RES_INT,  {(void *)TRUE } },
    { "input",          HCF_RES_ADDR, {(void *)&ppcIntCtlrInputs[0] } },
    { "inputTableSize", HCF_RES_INT,  {(void *)NELEMENTS(ppcIntCtlrInputs) } },
    };
#define ppcIntCtlr0Num  NELEMENTS (ppcIntCtlr0Resources)

struct intrCtlrInputs quiccIntCtlrInputs[] =
    {
    { INUM_UART1,       "ns16550",          0,  0 },
    { INUM_UART2,       "ns16550",          1,  0 },
    { INUM_TSEC1_TX,    "motetsec",         0,  0 },
    { INUM_TSEC1_RX,    "motetsec",         0,  1 },
    { INUM_TSEC1_ERR,   "motetsec",         0,  2 },
    { INUM_TSEC2_TX,    "motetsec",         1,  0 },
    { INUM_TSEC2_RX,    "motetsec",         1,  1 },
    { INUM_TSEC2_ERR,   "motetsec",         1,  2 },

#ifdef DRV_TIMER_QUICC_PPC
    { INUM_GTM2,        "quiccTimerDev",    0,  0 },
    { INUM_GTM4,        "quiccTimerDev",    1,  0 },
#endif /* DRV_TIMER_QUICC_PPC */

#ifdef INCLUDE_EHCI
    { INUM_USB_DR,      "vxbPlbUsbEhci",    0,  0 },
#endif /* INCLUDE_EHCI */

#ifdef DRV_STORAGE_SDHC
    { INUM_SDHC,        "sdhci",            0,  0 },
#endif /* DRV_STORAGE_SDHC */

#ifdef DRV_DMA_M512X
    { INUM_DMAC,        "m512xDma",         0,  0 },
#endif /* DRV_DMA_M512X */
    };

const struct hcfResource quiccIntCtlr0Resources[] =
    {
    { VXB_REG_BASE,       HCF_RES_INT,  { (void *)IPIC_BASE } },
    { "input",            HCF_RES_ADDR, { (void *)&quiccIntCtlrInputs[0] } },
    { "inputTableSize",   HCF_RES_INT,  { (void *)NELEMENTS(quiccIntCtlrInputs) } },
    { "intMaskTable",     HCF_RES_ADDR, { (void *)&quiccIntMask[0] } },
    { "intMaskTableSize", HCF_RES_INT,  { (void *) QUICC_IVEC_MAX } },
    { "inumMaskTable",    HCF_RES_ADDR, { (void *)&quiccInumToMaskPattern[0] } },
    { "inumMaskTableSize",HCF_RES_INT,  { (void *) QUICC_IVEC_MAX } }
    };
#define quiccIntCtlr0Num    NELEMENTS(quiccIntCtlr0Resources)

#ifdef INCLUDE_DUMMY_MDIO
const struct hcfResource dummyMdio0Resources[] =
    {
    { "regBase",        HCF_RES_INT,    { (void *)0 } },
    { "fullDuplex",     HCF_RES_INT,    { (void *)1 } },
    { "speed",          HCF_RES_INT,    { (void *)1000 } }
    };
#define dummyMdio0Num       NELEMENTS(dummyMdio0Resources)
#endif /* INCLUDE_DUMMY_MDIO */

const struct hcfResource ppcDecTimerDevResources[] =
    {
    { VXB_REG_BASE,     HCF_RES_INT,    { (void *)0 } },
    { "decClockFreq",   HCF_RES_ADDR,   { (void *)sysClkFreqGet } },
    { "minClkRate",     HCF_RES_INT,    { (void *)SYS_CLK_RATE_MIN } },
    { "maxClkRate",     HCF_RES_INT,    { (void *)SYS_CLK_RATE_MAX } }
    };
#define ppcDecTimerDevNum   NELEMENTS(ppcDecTimerDevResources)

/* vxBus resource files for onboard UARTS */

const struct hcfResource ns1655x0Resources[] = {
    { "regBase",        HCF_RES_INT,    { (void *)UART0_BASE } },
    { "clkFreq",        HCF_RES_ADDR,   { (void *)sysClkFreqGet } },
    { "regInterval",    HCF_RES_INT,    { (void *)DUART_REG_ADDR_INTERVAL } }
};
#define ns1655x0Num         NELEMENTS(ns1655x0Resources)

const struct hcfResource ns1655x1Resources[] = {
    { "regBase",        HCF_RES_INT,    { (void *)UART1_BASE } },
    { "clkFreq",        HCF_RES_ADDR,   { (void *)sysClkFreqGet } },
    { "regInterval",    HCF_RES_INT,    { (void *)DUART_REG_ADDR_INTERVAL } }
};
#define ns1655x1Num         NELEMENTS(ns1655x1Resources)

#ifdef DRV_TIMER_QUICC_PPC
const struct hcfResource generalPurposeTimerResources0[] =
    {
    { "regBase",        HCF_RES_INT,  { (void *)(GTM_BASE) } },
    { "regBase1",       HCF_RES_INT,  { (void *)(GTM_BASE + 0x10) } },
    { "clkFrequency",   HCF_RES_INT,  { (void *)SYS_CLK_FREQ } },
    { "clkRateMin",     HCF_RES_INT,  { (void *)AUX_CLK_RATE_MIN } },
    { "clkRateMax",     HCF_RES_INT,  { (void *)AUX_CLK_RATE_MAX } },
    };
#define generalPurposeTimerNum0   NELEMENTS(generalPurposeTimerResources0)

const struct hcfResource generalPurposeTimerResources1[] =
    {
    { "regBase",        HCF_RES_INT,  { (void *)(GTM_BASE + 0x04) } },
    { "regBase1",       HCF_RES_INT,  { (void *)(GTM_BASE + 0x20) } },
    { "clkFrequency",   HCF_RES_INT,  { (void *)SYS_CLK_FREQ } },
    { "clkRateMin",     HCF_RES_INT,  { (void *)AUX_CLK_RATE_MIN } },
    { "clkRateMax",     HCF_RES_INT,  { (void *)AUX_CLK_RATE_MAX } },
    };

#define generalPurposeTimerNum1   NELEMENTS(generalPurposeTimerResources1)
#endif /* DRV_TIMER_QUICC_PPC */

#ifdef DRV_PCIBUS_M83XX_EX
const struct hcfResource m83xxPciExResources[] = {
    { "regBase",          HCF_RES_INT,  { (void *)PCIEX_BASE } },
    { "cfgBase",          HCF_RES_ADDR, { (void *)PCIEX_CFG_BASE } },
    { "mem32Addr",        HCF_RES_ADDR, { (void *)PCIEX_MEM_ADRS } },
    { "mem32Size",        HCF_RES_INT,  { (void *)PCIEX_MEM_SIZE } },
    { "memIo32Addr",      HCF_RES_ADDR, { (void *)PCIEX_MEMIO_ADRS } },
    { "memIo32Size",      HCF_RES_INT,  { (void *)PCIEX_MEMIO_SIZE } },
    { "io32Addr",         HCF_RES_ADDR, { (void *)PCIEX_IO_ADRS } },
    { "io32Size",         HCF_RES_INT,  { (void *)PCIEX_IO_SIZE } },
    { "io16Addr",         HCF_RES_ADDR, { (void *)PCIEX_IO_ADRS } },
    { "io16Size",         HCF_RES_INT,  { (void *)PCIEX_IO_SIZE } },
    { "fbbEnable",        HCF_RES_INT,  { (void *)TRUE } },
    { "cacheSize",        HCF_RES_INT,  { (void *)(_CACHE_ALIGN_SIZE / 4) } },
    { "maxLatAllSet",     HCF_RES_INT,  { (void *)PCI_LAT_TIMER } },
    { "autoIntRouteSet",  HCF_RES_INT,  { (void *)TRUE } },
    { "includeFuncSet",   HCF_RES_ADDR, { (void *)sysPciExAutoconfigInclude } },
    { "intAssignFuncSet", HCF_RES_ADDR, { (void *)sysPciExAutoconfigIntrAssign } },
    { "maxBusSet",        HCF_RES_INT,  { (void *)1 } },
    { "pciConfigMechanism",HCF_RES_INT, { (void *)(0) } },

    /* non vxbPciAutoConfig() values */

    { "pimmrBase",        HCF_RES_ADDR, { (void *)PCI_BRIDGE_PIMMR_BASE_ADRS } },
    { "lclMemAddr",       HCF_RES_ADDR, { (void *)LOCAL_MEM_LOCAL_ADRS } },
    { "lclMemMapSize",    HCF_RES_INT,  { (void *)LOCAL_MEM_SIZE } },
    { "mstrMemBus",       HCF_RES_ADDR, { (void *)PCI_MSTR_MEM_BUS } },
    { "autoConfig",       HCF_RES_INT,  { (void *)TRUE } },
    { "lawbar",           HCF_RES_INT,  { (void *)PCIEXP1LAWBAR } },
    { "lawar",            HCF_RES_INT,  { (void *)PCIEXP1LAWAR } },
    { "lawarAttr",        HCF_RES_INT,  { (void *)(PCI_LAWAR_ENABLE |
                                                   PCI_LAWAR_SIZE_512MB) } },
    { "pciExpressHost",   HCF_RES_INT,  { (void *)TRUE } },
    { "msiEnable",        HCF_RES_INT,  { (void *)FALSE } },
    { "msiAddress",       HCF_RES_INT,  { (void *)0 } },
    { "msiData",          HCF_RES_INT,  { (void *)0 } }
};
#   define m83xxPciExNum NELEMENTS(m83xxPciExResources)
#endif /* DRV_PCIBUS_M83XX_EX */

#if defined (INCLUDE_ETSEC_VXB_END) || defined (INCLUDE_ETSEC_VXB_END2)
const struct hcfResource motEtsecHEnd0Resources[] =
    {
    { "regBase",    HCF_RES_INT,    { (void *)ETSEC1_BASE } },
    { "phyAddr",    HCF_RES_INT,    { (void *)2 }},
    { "miiIfName",  HCF_RES_STRING, { (void *)"motetsec" } },
    { "miiIfUnit",  HCF_RES_INT,    { (void *)0 } }
    };
#define etsecHEnd0Num       NELEMENTS(motEtsecHEnd0Resources)

const struct hcfResource motEtsecHEnd1Resources[] =
    {
    { "regBase",    HCF_RES_INT,    { (void *)ETSEC2_BASE } },
    { "phyAddr",    HCF_RES_INT,    { (void *)0 }},
    { "miiIfName",  HCF_RES_STRING, { (void *)"dummyMdio" } },
    { "miiIfUnit",  HCF_RES_INT,    { (void *)0 } }
    };
#define etsecHEnd1Num       NELEMENTS(motEtsecHEnd1Resources)
#endif /* INCLUDE_ETSEC_VXB_END || INCLUDE_ETSEC_VXB_END2 */

#ifdef INCLUDE_EHCI
const struct hcfResource vxbPlbUsbEhciDevResources[]=
    {
    {"regBase",     HCF_RES_INT,    { (void *)M83XX_CAPLENGTH } },
    {"ehciInit",    HCF_RES_ADDR,   { (void *)sysEhciInit } },
    {"dataSwap",    HCF_RES_ADDR,   { (void *)vxbSwap32}},
    };
#define vxbPlbUsbEhciDevNum NELEMENTS(vxbPlbUsbEhciDevResources)
#endif /* INCLUDE_EHCI */

#ifdef DRV_STORAGE_SDHC
struct hcfResource sdhcResources[] =  {
    { "regBase",    HCF_RES_INT,    { (void *)SDHC_BASE } },
    { "clkFreq",    HCF_RES_ADDR,   { (void *)sysSdhcClkFreqGet } },
    { "dmaMode",    HCF_RES_INT,    { (void *)FALSE } },
    { "polling",    HCF_RES_INT,    { (void *)FALSE } },
    { "flags"  ,    HCF_RES_INT,    { (void *)(SDHC_PIO_NEED_DELAY | 
                                               SDHC_HW_SNOOP |
                                               SDHC_FIFO_ENDIANESS_REVERSE |
                                               SDHC_HOST_VER_REVERSE |
                                               SDHC_HOST_CTRL_FREESCALE) } },
};
#define sdhcNum  NELEMENTS(sdhcResources)
#endif /* DRV_STORAGE_SDHC */

#ifdef DRV_DMA_M512X
const struct hcfResource m512xDmaResources[] = {
    { "regBase",    HCF_RES_INT,    { (void *)DMAC_BASE } },
    };
#define m512xDmaNum NELEMENTS(m512xDmaResources)
#endif /* DRV_DMA_M512X */

const struct hcfDevice hcfDeviceList[] =
    {
    { "quiccIntCtlr",   0, VXB_BUSID_PLB, 0, quiccIntCtlr0Num,      quiccIntCtlr0Resources },
    { "ppcIntCtlr",     0, VXB_BUSID_PLB, 0, ppcIntCtlr0Num,        ppcIntCtlr0Resources },

#ifdef INCLUDE_DUMMY_MDIO
    { "dummyMdio",      0, VXB_BUSID_PLB, 0, dummyMdio0Num,         dummyMdio0Resources },
#endif /* INCLUDE_DUMMY_MDIO */

#if defined (INCLUDE_ETSEC_VXB_END) || defined (INCLUDE_ETSEC_VXB_END2)
    { "motetsec",       0, VXB_BUSID_PLB, 0, etsecHEnd0Num,         motEtsecHEnd0Resources },
    { "motetsec",       1, VXB_BUSID_PLB, 0, etsecHEnd1Num,         motEtsecHEnd1Resources },
#endif /* INCLUDE_ETSEC_VXB_END || INCLUDE_ETSEC_VXB_END2 */

    { "ppcDecTimerDev", 0, VXB_BUSID_PLB, 0, ppcDecTimerDevNum,     ppcDecTimerDevResources },

#ifdef DRV_TIMER_QUICC_PPC
    { "quiccTimerDev",  0, VXB_BUSID_PLB, 0, generalPurposeTimerNum0, generalPurposeTimerResources0 },
    { "quiccTimerDev",  1, VXB_BUSID_PLB, 0, generalPurposeTimerNum1, generalPurposeTimerResources1 },
#endif /* DRV_TIMER_QUICC_PPC */

    { "ns16550",        0, VXB_BUSID_PLB, 0, ns1655x0Num,           ns1655x0Resources },
    { "ns16550",        1, VXB_BUSID_PLB, 0, ns1655x1Num,           ns1655x1Resources },

#ifdef DRV_PCIBUS_M83XX_EX
    { "m83xxPciEx",     0, VXB_BUSID_PLB, 0, m83xxPciExNum,         m83xxPciExResources },
#endif /* DRV_PCIBUS_M83XX_EX */

#ifdef INCLUDE_EHCI
    { "vxbPlbUsbEhci",  0, VXB_BUSID_PLB, 0, vxbPlbUsbEhciDevNum,   vxbPlbUsbEhciDevResources },
#endif /* INCLUDE_USB */

#ifdef DRV_STORAGE_SDHC
    { "sdhci",          0, VXB_BUSID_PLB, 0, sdhcNum,               sdhcResources },
#endif /* DRV_STORAGE_SDHC */

#ifdef DRV_DMA_M512X
    { "m512xDma",       0, VXB_BUSID_PLB, 0, m512xDmaNum,           m512xDmaResources },
#endif /* DRV_DMA_M512X */
    };

const int hcfDeviceNum = NELEMENTS(hcfDeviceList);

VXB_INST_PARAM_OVERRIDE sysInstParamTable[] =
    {
#ifdef INCLUDE_EHCI
    { "vxbPlbUsbEhci", 0, "hasEmbeddedTT",   VXB_PARAM_INT32,       {(void *)TRUE} },
    { "vxbPlbUsbEhci", 0, "fixupPortNumber", VXB_PARAM_INT32,       {(void *)FALSE} },
    { "vxbPlbUsbEhci", 0, "postResetHook",   VXB_PARAM_FUNCPTR,     {(void *)usbOpAfterReset} },
#endif /* INCLUDE_EHCI */
    { NULL,            0, NULL,              VXB_PARAM_END_OF_LIST, {(void *)0} }
    };
