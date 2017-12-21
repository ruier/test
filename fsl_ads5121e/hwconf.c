/* hwconf.c - hardware configuration support module */

/*
 * Copyright (c) 2007-2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01l,21jan11,yjw  Change CPLD PCI INTA index. (WIND00211229)
01k,10dec10,y_y  fix miss pci macro error. (WIND00245940)
01j,04aug10,liu  clear mot83xxPci.h. (WIND00212294)
01i,15sep09,w_x  rename USB host hwinit routines (WIND00180575)
01h,29jul09,rfm  add graphics support.
01g,09mar09,b_m  add FIFOC and AXE support.
01f,26feb09,l_z  add audio support.
01e,04feb09,w_x  add USB EHCI support.
01d,25dec08,x_f  add PCI and CPLD intCtlr support.
01c,24dec07,b_m  add ppcIntCtrl & IPIC support.
01b,01nov07,b_m  add DMA controller support.
01a,28sep07,b_m  created.
*/

/* includes */

#include <vxWorks.h>
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/hwConf.h>
#include <hwif/vxbus/vxbPlbLib.h>
#include <hwif/vxbus/vxbIntrCtlr.h>
#include <hwif/intCtlr/vxbQuiccIntCtlr.h>
#include <hwif/util/vxbParamSys.h>
#include <busCtlr/m83xxPci.h>
#include "config.h"

/* externs */

IMPORT UINT32 sysGetCsbClock(void);
IMPORT UINT32 sysGetIpsClock(void);

#ifdef DRV_PCIBUS_M83XX
#include <drv/pci/pciAutoConfigLib.h>
IMPORT STATUS sysPciAutoconfigInclude (PCI_SYSTEM *, PCI_LOC *, UINT);
IMPORT UCHAR  sysPciAutoconfigIntrAssign (PCI_SYSTEM *, PCI_LOC *, UCHAR);
#endif /* DRV_PCIBUS_M83XX */

#ifdef INCLUDE_EHCI
IMPORT void fslUsbDrHostHwInit (void);
IMPORT void fslUsbDrHostModeSet (void);
#endif /* INCLUDE_EHCI */

/* device resources */

const struct intrCtlrInputs ppcIntCtlrInputs[] = {
    { 0, "quiccIntCtlr",   0, 0 },
    { 1, "ppcDecTimerDev", 0, 0 },
};

const struct hcfResource ppcIntCtlrResources[] = {
    { "regBase",        HCF_RES_INT,  { (void *)TRUE } },
    { "input",          HCF_RES_ADDR, { (void *)&ppcIntCtlrInputs[0] } },
    { "inputTableSize", HCF_RES_INT,  { (void *)NELEMENTS(ppcIntCtlrInputs) } }
};
#define ppcIntCtlrNum   NELEMENTS(ppcIntCtlrResources)

struct intrCtlrInputs quiccIntCtlrInputs[] = {
    { INUM_FIFOC,    "m512xFifoc",          0, 0 },
    { INUM_FEC,      "motfec",              0, 0 },
#ifdef DRV_TIMER_M5XXX_GPT
    { INUM_GPT0,     "m5xxxGPTimerDev",     0, 0 },
#endif
#ifdef DRV_DMA_M512X
    { INUM_DMA2,     "m512xDma",            0, 0 },
#endif
#if defined(INCLUDE_USB) && defined(INCLUDE_EHCI)
    { INUM_USB_UTMI, "vxbPlbUsbEhci",       0, 0 },
#endif
#ifdef DRV_INTCTLR_ADS5121E_CPLD
    { INUM_IRQ0,     "ads5121eCpldIntCtlr", 0, 0 },
    { INUM_IRQ1,     "ads5121eCpldIntCtlr", 0, 1 },
#endif
#ifdef DRV_FW_FSL_AXE
    { INUM_AXE,      "fslAxe",              0, 0 },
#endif
#ifdef INCLUDE_WINDML
    { INUM_DIU,      "graphics",            0, 0 },
#endif
};

const SIU_INT_MASK ipicIntMask[] =
    {
    {0x00000000, 0x00000000, 0x00000000},       /* Error */
    {0x00000000, 0x00000000, 0x00000000},       /* PCI */
    {0x00000000, 0x00000000, 0x00000000},       /* PCI DMA */
    {0x00000000, 0x00000000, 0x00000000},       /* PCI MU */
    {0x00000000, 0x00000000, 0x00000000},       /* FEC */
    {0x00000000, 0x00000000, 0x00000000},       /* PATA */
    {0x00000000, 0x00000000, 0x00000000},       /* NFC */
    {0x00000000, 0x00000000, 0x00000000},       /* LPC */
    {0x00000000, 0x00000000, 0x00000000},       /* SDHC */
    {0x00000000, 0x00000000, 0x00000000},       /* I2C1 */
    {0x00000000, 0x00000000, 0x00000000},       /* I2C2 */
    {0x00000000, 0x00000000, 0x00000000},       /* I2C3 */
    {0x00000000, 0x00000000, 0x00000000},       /* MSCAN1 */
    {0x00000000, 0x00000000, 0x00000000},       /* MSCAN2 */
    {0x00000000, 0x00000000, 0x00000000},       /* BDLC */
    {0x00000000, 0x00000000, 0x00000000},       /* GPT0 */
    {0x00000000, 0x00000000, 0x00000000},       /* GPT1 */
    {0x00000000, 0x00000000, 0x00000000},       /* IRQ1 */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* PSC4 */
    {0x00000000, 0x00000000, 0x00000000},       /* PSC5 */
    {0x00000000, 0x00000000, 0x00000000},       /* PSC6 */
    {0x00000000, 0x00000000, 0x00000000},       /* PSC7 */
    {0x00000000, 0x00000000, 0x00000000},       /* PSC8 */
    {0x00000000, 0x00000000, 0x00000000},       /* PSC9 */
    {0x00000000, 0x00000000, 0x00000000},       /* PSC10 */
    {0x00000000, 0x00000000, 0x00000000},       /* PSC11 */
    {0x00000000, 0x00000000, 0x00000000},       /* FIFOC */
    {0x00000000, 0x00000000, 0x00000000},       /* SPDIF */
    {0x00000000, 0x00000000, 0x00000000},       /* AXE */
    {0x00000000, 0x00000000, 0x00000000},       /* USB ULPI */
    {0x00000000, 0x00000000, 0x00000000},       /* USB UTMI */
    {0x00000000, 0x00000000, 0x00000000},       /* SATA */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* IRQ0 */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* DIU */
    {0x00000000, 0x00000000, 0x00000000},       /* DMA2 */
    {0x00000000, 0x00000000, 0x00000000},       /* MBX */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* PSC0 */
    {0x00000000, 0x00000000, 0x00000000},       /* PSC1 */
    {0x00000000, 0x00000000, 0x00000000},       /* PSC2 */
    {0x00000000, 0x00000000, 0x00000000},       /* PSC3 */
    {0x00000000, 0x00000000, 0x00000000},       /* GPT2 */
    {0x00000000, 0x00000000, 0x00000000},       /* GPT3 */
    {0x00000000, 0x00000000, 0x00000000},       /* GPT4 */
    {0x00000000, 0x00000000, 0x00000000},       /* GPT5 */
    {0x00000000, 0x00000000, 0x00000000},       /* GPT6 */
    {0x00000000, 0x00000000, 0x00000000},       /* GPT7 */
    {0x00000000, 0x00000000, 0x00000000},       /* GPIO */
    {0x00000000, 0x00000000, 0x00000000},       /* RTC SEC */
    {0x00000000, 0x00000000, 0x00000000},       /* RTC ALARM */
    {0x00000000, 0x00000000, 0x00000000},       /* DDR */
    {0x00000000, 0x00000000, 0x00000000},       /* SBA */
    {0x00000000, 0x00000000, 0x00000000},       /* PMC */
    {0x00000000, 0x00000000, 0x00000000},       /* USB ULPI WKUP */
    {0x00000000, 0x00000000, 0x00000000},       /* USB UTMI WKUP */
    {0x00000000, 0x00000000, 0x00000000},       /* SATA CMD */
    {0x00000000, 0x00000000, 0x00000000},       /* TEMP 105C */
    {0x00000000, 0x00000000, 0x00000000},       /* IIM */
    {0x00000000, 0x00000000, 0x00000000},       /* PRIOMON */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},       /* Reserved */
    {0x00000000, 0x00000000, 0x00000000}        /* Reserved */
    };

const UINT32 ipicInumToMaskPattern[] =
    {
    0x00000000,         /* Error */
    0x00008000,         /* PCI */
    0x00004000,         /* PCI DMA */
    0x00002000,         /* PCI MU */
    0x00001000,         /* FEC */
    0x00000800,         /* PATA */
    0x00000400,         /* NFC */
    0x00000200,         /* LPC */
    0x00000100,         /* SDHC */
    0x00000080,         /* I2C1 */
    0x00000040,         /* I2C2 */
    0x00000020,         /* I2C3 */
    0x00000010,         /* MSCAN1 */
    0x00000008,         /* MSCAN2 */
    0x00000004,         /* BDLC */
    0x00000002,         /* GPT0 */
    0x00000001,         /* GPT1 */
    0x40000000,         /* IRQ1 */
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
    0x80000000,         /* PSC4 */
    0x40000000,         /* PSC5 */
    0x20000000,         /* PSC6 */
    0x10000000,         /* PSC7 */
    0x08000000,         /* PSC8 */
    0x04000000,         /* PSC9 */
    0x02000000,         /* PSC10 */
    0x01000000,         /* PSC11 */
    0x00800000,         /* FIFOC */
    0x00400000,         /* SPDIF */
    0x00200000,         /* AXE */
    0x00100000,         /* USB ULPI */
    0x00080000,         /* USB UTMI */
    0x00040000,         /* SATA */
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
    0x80000000,         /* DIU */
    0x40000000,         /* DMA2 */
    0x20000000,         /* MBX */
    0x00000000,         /* Reserved */
    0x08000000,         /* PSC0 */
    0x04000000,         /* PSC1 */
    0x02000000,         /* PSC2 */
    0x01000000,         /* PSC3 */
    0x00800000,         /* GPT2 */
    0x00400000,         /* GPT3 */
    0x00200000,         /* GPT4 */
    0x00100000,         /* GPT5 */
    0x00080000,         /* GPT6 */
    0x00040000,         /* GPT7 */
    0x00020000,         /* GPIO */
    0x00010000,         /* RTC SEC */
    0x00008000,         /* RTC ALARM */
    0x00004000,         /* DDR */
    0x00002000,         /* SBA */
    0x00001000,         /* PMC */
    0x00000800,         /* USB ULPI WKUP */
    0x00000400,         /* USB UTMI WKUP */
    0x00000200,         /* SATA CMD */
    0x00000100,         /* TEMP 105C */
    0x00000080,         /* IIM */
    0x00000040,         /* PRIOMON */
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
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000          /* Reserved */
    };

const struct hcfResource quiccIntCtlrResources[] = {
    { "regBase",           HCF_RES_INT,  { (void *)IPIC_BA } },
    { "input",             HCF_RES_ADDR, { (void *)&quiccIntCtlrInputs[0] } },
    { "inputTableSize",    HCF_RES_INT,  { (void *)NELEMENTS(quiccIntCtlrInputs) } },
    { "intMaskTable",      HCF_RES_ADDR, { (void *)&ipicIntMask[0] } },
    { "intMaskTableSize",  HCF_RES_INT,  { (void *)QUICC_IVEC_MAX } },
    { "inumMaskTable",     HCF_RES_ADDR, { (void *)&ipicInumToMaskPattern[0] } },
    { "inumMaskTableSize", HCF_RES_INT,  { (void *)QUICC_IVEC_MAX } }

};
#define quiccIntCtlrNum     NELEMENTS(quiccIntCtlrResources)

const struct intrCtlrInputs m512xFifocInputs[] = {
    { 3, "m5xxxPscSio", 0, 0 },
    { 4, "m5xxxPscSio", 1, 0 },
    { 5, "audio",       0, 0 },
};

const struct hcfResource m512xFifocResources[] = {
    { "regBase",        HCF_RES_INT,  { (void *)FIFOC_BA } },
    { "input",          HCF_RES_ADDR, { (void *)&m512xFifocInputs[0] } },
    { "inputTableSize", HCF_RES_INT,  { (void *)NELEMENTS(m512xFifocInputs) } }
};
#define m512xFifocNum   NELEMENTS(m512xFifocResources)

const struct hcfResource ppcDecTimerResources[] = {
    { "regBase",      HCF_RES_INT,  { (void *)0 } },
    { "minClkRate",   HCF_RES_INT,  { (void *)SYS_CLK_RATE_MIN } },
    { "maxClkRate",   HCF_RES_INT,  { (void *)SYS_CLK_RATE_MAX } },
    { "decClockFreq", HCF_RES_ADDR, { (void *)sysGetCsbClock } },
};
#define ppcDecTimerNum  NELEMENTS(ppcDecTimerResources)

const struct hcfResource m5xxxPsc0Resources[] = {
    { "regBase",  HCF_RES_INT,  { (void *)UART0_BA } },
    { "clkFreq",  HCF_RES_ADDR, { (void *)sysGetIpsClock } },
    { "fifoMode", HCF_RES_INT,  { (void *)TRUE } },
    { "pscNum",   HCF_RES_INT,  { (void *)3 } },
};
#define m5xxxPsc0Num    NELEMENTS(m5xxxPsc0Resources)

const struct hcfResource m5xxxPsc1Resources[] = {
    { "regBase",  HCF_RES_INT,  { (void *)UART1_BA } },
    { "clkFreq",  HCF_RES_ADDR, { (void *)sysGetIpsClock } },
    { "fifoMode", HCF_RES_INT,  { (void *)TRUE } },
    { "pscNum",   HCF_RES_INT,  { (void *)4 } },
};
#define m5xxxPsc1Num    NELEMENTS(m5xxxPsc1Resources)

const struct hcfResource m512xFecResources[] = {
    { "regBase",   HCF_RES_INT,    { (void *)FEC_BA } },
    { "txFixup",   HCF_RES_INT,    { (void *)1 } },
    { "clkFreq",   HCF_RES_ADDR,   { (void *)sysGetIpsClock } },
    { "phyAddr",   HCF_RES_INT,    { (void *)1 } },
    { "miiIfName", HCF_RES_STRING, { (void *)"motfec" } },
    { "miiIfUnit", HCF_RES_INT,    { (void *)0 } },
};
#define m512xFecNum NELEMENTS(m512xFecResources)

#ifdef DRV_TIMER_M5XXX_GPT
const struct hcfResource m5xxxGPTimerResources[] = {
    { "regBase",    HCF_RES_INT,  { (void *)GPT_BA(0) } },
    { "clkRateMin", HCF_RES_INT,  { (void *)AUX_CLK_RATE_MIN } },
    { "clkRateMax", HCF_RES_INT,  { (void *)AUX_CLK_RATE_MAX } },
    { "clkFreq",    HCF_RES_ADDR, { (void *)sysGetIpsClock } },
};
#define m5xxxGPTimerNum NELEMENTS(m5xxxGPTimerResources)
#endif /* DRV_TIMER_M5XXX_GPT */

#ifdef DRV_DMA_M512X
const struct hcfResource m512xDmaResources[] = {
    { "regBase",    HCF_RES_INT,  { (void *)DMA_BA } },
};
#define m512xDmaNum NELEMENTS(m512xDmaResources)
#endif /* DRV_DMA_M512X */

#ifdef DRV_INTCTLR_ADS5121E_CPLD
const struct intrCtlrInputs cpldIntCtlrInputs[] = {

    /* pin,             driver,     unit,   index */

    { PCI_INTA_SLOT1,   "legacy",   0,      128 },
    { PCI_INTA_SLOT2,   "legacy",   0,      129 },
    { PCI_INTA_SLOT3,   "legacy",   0,      130 },
};

const struct hcfResource cpldIntCtlrResources[] = {
    { "regBase",        HCF_RES_INT,  { (void *)CPLD_BASE_ADRS } },
    { "input",          HCF_RES_ADDR, { (void *)&cpldIntCtlrInputs[0] } },
    { "inputTableSize", HCF_RES_INT,  { (void *)NELEMENTS(cpldIntCtlrInputs) } },
};
#define cpldIntCtlrNum NELEMENTS(cpldIntCtlrResources)
#endif /* DRV_INTCTLR_ADS5121E_CPLD */

#ifdef DRV_PCIBUS_M83XX
const struct hcfResource m83xxPci0Resources[] = {
    { "regBase",            HCF_RES_INT,  { (void *)(IMMRBAR | PCI_REG_BASE) } },
    { "mem32Addr",          HCF_RES_ADDR, { (void *)PCI_MEM_ADRS } },
    { "mem32Size",          HCF_RES_INT,  { (void *)PCI_MEM_SIZE } },
    { "memIo32Addr",        HCF_RES_ADDR, { (void *)PCI_MEMIO_ADRS } },
    { "memIo32Size",        HCF_RES_INT,  { (void *)PCI_MEMIO_SIZE } },
    { "io32Addr",           HCF_RES_ADDR, { (void *)PCI_IO_ADRS } },
    { "io32Size",           HCF_RES_INT,  { (void *)PCI_IO_SIZE } },
    { "io16Addr",           HCF_RES_ADDR, { (void *)PCI_IO_ADRS } },
    { "io16Size",           HCF_RES_INT,  { (void *)PCI_IO_SIZE } },
    { "fbbEnable",          HCF_RES_INT,  { (void *)TRUE } },
    { "cacheSize",          HCF_RES_INT,  { (void *)(_CACHE_ALIGN_SIZE / 4) } },
    { "maxLatAllSet",       HCF_RES_INT,  { (void *)0x80 } },
    { "autoIntRouteSet",    HCF_RES_INT,  { (void *)TRUE } },
    { "includeFuncSet",     HCF_RES_ADDR, { (void *)sysPciAutoconfigInclude } },
    { "intAssignFuncSet",   HCF_RES_ADDR, { (void *)sysPciAutoconfigIntrAssign } },
    { "maxBusSet",          HCF_RES_INT,  { (void *)3 } },
    { "pciConfigMechanism", HCF_RES_INT,  { (void *)0 } },

    /* non vxbPciAutoConfig() values */

    { "pimmrBase",     HCF_RES_ADDR, { (void *)PCI_BRIDGE_PIMMR_BASE_ADRS } },
    { "lclMemAddr",    HCF_RES_ADDR, { (void *)LOCAL_MEM_LOCAL_ADRS } },
    { "lclMemMapSize", HCF_RES_INT,  { (void *)(512 * 1024 * 1024) } },
    { "mstrMemBus",    HCF_RES_ADDR, { (void *)PCI_MSTR_MEM_BUS } },
    { "owAttrMem",     HCF_RES_INT,  { (void *)(M83XXPCI_OB_WINDOW_ENABLE_BIT |
                                                M83XXPCI_PREFETCHABLE         |
                                                M83XXPCI_OB_ATTR_WS_128M) } },
    { "owAttrMemIo",   HCF_RES_INT,  { (void *)(M83XXPCI_OB_WINDOW_ENABLE_BIT |
                                                M83XXPCI_OB_ATTR_WS_64M) } },
    { "owAttrIo",      HCF_RES_INT,  { (void *)(M83XXPCI_OB_WINDOW_ENABLE_BIT |
                                                M83XXPCI_OB_ATTR_IO_BIT       |
                                                M83XXPCI_OB_ATTR_WS_64M) } },
    { "iwAttr",        HCF_RES_INT,  { (void *)(M83XXPCI_IB_WINDOW_ENABLE_BIT |
                                                M83XXPCI_PREFETCHABLE         |
                                                M83XXPCI_IB_ATTR_RTT_READ_SNOOP  |
                                                M83XXPCI_IB_ATTR_RTT_WRITE_SNOOP |
                                                M83XXPCI_IB_ATTR_IWS_512M) } },
    { "lawbar",        HCF_RES_INT,  { (void *)(M83XX_PCILAWBARn(IMMRBAR, 0)) } },
    { "lawar",         HCF_RES_INT,  { (void *)(M83XX_PCILAWARn(IMMRBAR, 0)) } },
    { "lawarAttr",     HCF_RES_INT,  { (void *)(M83XXPCI_LAWAR_ENABLE |
                                                M83XXPCI_LAWAR_SIZE_256MB) } },
    { "autoConfig",    HCF_RES_INT,  { (void *)TRUE } },
};
#define m83xxPci0Num NELEMENTS(m83xxPci0Resources)
#endif /* DRV_PCIBUS_M83XX  */

#ifdef INCLUDE_EHCI
const struct hcfResource vxbPlbUsbEhciDevResources[] = {
    { "regBase",  HCF_RES_INT,  { (void *)(USB0_BA + 0x100) } },
    { "ehciInit", HCF_RES_ADDR, { (void *)fslUsbDrHostHwInit } },
};
#define vxbPlbUsbEhciDevNum NELEMENTS(vxbPlbUsbEhciDevResources)
#endif /* INCLUDE_EHCI */

#ifdef INCLUDE_WINDML
const struct hcfResource m512xVgaAudioResources[] = {
    { "regBase",    HCF_RES_INT, { (void *)PCI_IO_ADRS } },
    { "pciMemBase", HCF_RES_INT, { (void *)PCI_MEM_ADRS } },
    { "barMask",    HCF_RES_INT, { (void *)0xffffffff } },
    { "barMap",     HCF_RES_INT, { (void *)0 } },
};
#define m512xVgaAudioNum    NELEMENTS(m512xVgaAudioResources)

const struct hcfResource m512xAudioResources[] = {
    { "regBase", HCF_RES_INT, { (void *)AUDIO_BA } },
    { "pscNum",  HCF_RES_INT, { (void *)5 } }
};
#define m512xAudioNum       NELEMENTS(m512xAudioResources)

const struct hcfResource m512xDiuResources[] = {
	{ "immrBase", HCF_RES_INT,  { (void *)IMMRBAR } },
	{ "csbFreq",  HCF_RES_ADDR, { (void*)sysGetCsbClock} },
};
#define m512xDiuNum         NELEMENTS(m512xDiuResources)
#endif /* INCLUDE_WINDML */

#ifdef DRV_FW_FSL_AXE
struct hcfResource fslAxeResources[] = {
    { "regBase",       HCF_RES_INT,    { (void *)AXE_BA } },
    { "rsvdDramBase",  HCF_RES_INT,    { (void *)AXE_RSVD_DRAM_BASE } },
    { "rsvdDramSize",  HCF_RES_INT,    { (void *)AXE_RSVD_DRAM_SIZE } },
    { "useSram",       HCF_RES_INT,    { (void *)TRUE } },
    { "sramBase",      HCF_RES_INT,    { (void *)SRAM_BASE_ADRS } },
    { "sramSize",      HCF_RES_INT,    { (void *)SRAM_SIZE } },
    { "schedulerPath", HCF_RES_STRING, { (void *)"scheduler.bin" } },
};
#define fslAxeNum   NELEMENTS(fslAxeResources)
#endif /* DRV_FW_FSL_AXE */

const struct hcfDevice hcfDeviceList[] = {
    { "ppcIntCtlr",     0, VXB_BUSID_PLB, 0, ppcIntCtlrNum,   ppcIntCtlrResources },
    { "quiccIntCtlr",   0, VXB_BUSID_PLB, 0, quiccIntCtlrNum, quiccIntCtlrResources },
    { "m512xFifoc",     0, VXB_BUSID_PLB, 0, m512xFifocNum,   m512xFifocResources },
    { "ppcDecTimerDev", 0, VXB_BUSID_PLB, 0, ppcDecTimerNum,  ppcDecTimerResources },
    { "m5xxxPscSio",    0, VXB_BUSID_PLB, 0, m5xxxPsc0Num,    m5xxxPsc0Resources },
    { "m5xxxPscSio",    1, VXB_BUSID_PLB, 0, m5xxxPsc1Num,    m5xxxPsc1Resources },
    { "motfec",         0, VXB_BUSID_PLB, 0, m512xFecNum,     m512xFecResources },
#ifdef DRV_TIMER_M5XXX_GPT
    { "m5xxxGPTimerDev",0, VXB_BUSID_PLB, 0, m5xxxGPTimerNum, m5xxxGPTimerResources },
#endif /* DRV_TIMER_M5XXX_GPT */
#ifdef DRV_DMA_M512X
    { "m512xDma",       0, VXB_BUSID_PLB, 0, m512xDmaNum,     m512xDmaResources },
#endif /* DRV_DMA_M512X */
#ifdef DRV_INTCTLR_ADS5121E_CPLD
    { "ads5121eCpldIntCtlr", 0, VXB_BUSID_PLB, 0, cpldIntCtlrNum, cpldIntCtlrResources },
#endif /* DRV_INTCTLR_ADS5121E_CPLD */
#ifdef DRV_PCIBUS_M83XX
    { "m83xxPci",            0, VXB_BUSID_PLB, 0, m83xxPci0Num,   m83xxPci0Resources },
#endif /* DRV_PCIBUS_M83XX */
#ifdef INCLUDE_EHCI
    { "vxbPlbUsbEhci", 0, VXB_BUSID_PLB, 0, vxbPlbUsbEhciDevNum, vxbPlbUsbEhciDevResources },
#endif /* INCLUDE_EHCI */
#ifdef INCLUDE_WINDML
    { "vga-audio", 0, VXB_BUSID_PLB, 0, m512xVgaAudioNum, m512xVgaAudioResources },
    { "audio",     0, VXB_BUSID_PLB, 0, m512xAudioNum ,   m512xAudioResources },
	{ "graphics",  0, VXB_BUSID_PLB, 0, m512xDiuNum,      m512xDiuResources },
#endif /* INLCUDE_WINDML */
#ifdef DRV_FW_FSL_AXE
    { "fslAxe", 0, VXB_BUSID_PLB, 0, fslAxeNum, fslAxeResources },
#endif /* DRV_FW_FSL_AXE */
#ifdef INCLUDE_SER_EMULATOR
    { "usbTargSerSio", 2, VXB_BUSID_PLB, 0, 0, NULL },
#endif /* INCLUDE_SER_EMULATOR */
};
const int hcfDeviceNum = NELEMENTS(hcfDeviceList);


VXB_INST_PARAM_OVERRIDE sysInstParamTable[] = {
#ifdef INCLUDE_EHCI
    { "vxbPlbUsbEhci", 0, "hasEmbeddedTT",   VXB_PARAM_INT32,   { (void *)TRUE } },
    { "vxbPlbUsbEhci", 0, "fixupPortNumber", VXB_PARAM_INT32,   { (void *)FALSE } },
    { "vxbPlbUsbEhci", 0, "postResetHook",   VXB_PARAM_FUNCPTR, { (void *)fslUsbDrHostModeSet } },
    { "vxbPlbUsbEhci", 0, "descBigEndian",   VXB_PARAM_INT32,   { (void *)TRUE } },
    /* regBigEndian may be overriden to FALSE in fsl5121eUsbEndianProbe */
    { "vxbPlbUsbEhci", 0, "regBigEndian",    VXB_PARAM_INT32,   { (void *)TRUE } },
#endif /* INCLUDE_EHCI */
    { NULL, 0, NULL, VXB_PARAM_END_OF_LIST, { (void *)0 } }
};
