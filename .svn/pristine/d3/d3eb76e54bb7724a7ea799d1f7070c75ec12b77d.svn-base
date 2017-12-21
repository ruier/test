/* hwconf.c - Hardware configuration support module */

/*
 * Copyright (c) 2011, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01n,09may12,syt  set IPI interrupt vectors as edge-trigger mode. (WIND00341799) 
01m,29feb12,syt  updated for new SATA driver with INCLUDE_DRV_STORAGE_AHCI.
01l,06feb12,syt  added SDHC and SATA support.
01g,20dec11,x_z  added support for QMAN and on-chip MAC;
                 fixed Code Coverity issues for PCIe.
01f,06dec11,x_s  add AMP support.
01e,28sep11,syt  add flag MPIC_QUIRK_IACK in mpic0Resources to fix
                 MPIC hang issue. (WIND00295774)
01d,30jul11,x_z  fix PCI Express support.
01c,22jul11,b_m  add PCI Express support.
01b,18jul11,b_m  add usb host support.
01a,16jun11,syt  initial creation.
*/

/* includes */

#include <vxWorks.h>
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/hwConf.h>
#include <hwif/util/vxbParamSys.h>
#include "config.h"
#include <hwif/vxbus/vxbIntrCtlr.h>
#include <hwif/intCtlr/vxbMpicIntCtlr.h>

/* externs */

IMPORT UINT32 sysClkFreqGet (void);

#ifdef DRV_PCIBUS_APM_PCIEX

IMPORT STATUS sysPciEx0SerdesInit (void);
IMPORT STATUS sysPciEx1SerdesInit (void);
IMPORT STATUS sysPciEx2SerdesInit (void);

IMPORT UCHAR sysPciEx0AutoCfgIntAssign
    (
    PCI_SYSTEM *    pSys,
    PCI_LOC *       pLoc,
    UCHAR           pin
    );
IMPORT UCHAR sysPciEx1AutoCfgIntAssign
    (
    PCI_SYSTEM *    pSys,
    PCI_LOC *       pLoc,
    UCHAR           pin
    );
IMPORT UCHAR sysPciEx2AutoCfgIntAssign
    (
    PCI_SYSTEM *    pSys,
    PCI_LOC *       pLoc,
    UCHAR           pin
    );
IMPORT STATUS sysPciEx0AutoCfgInclude();
IMPORT STATUS sysPciEx1AutoCfgInclude();
IMPORT STATUS sysPciEx2AutoCfgInclude();

#endif /* DRV_PCIBUS_APM_PCIEX */

#ifdef DRV_STORAGE_SDHC
IMPORT UINT32 sysSdhcClkFreqGet (void);
#endif /* DRV_STORAGE_SDHC */

#ifdef DRV_STORAGE_SDHC
#   include <h/storage/vxbSdMmcLib.h>
#   include <h/storage/vxbSdhcStorage.h>
#endif /* DRV_STORAGE_SDHC */

#if defined(INCLUDE_AMP_CPU) && !defined(INCLUDE_AMP_PPRO_END_DRIVER)
#   undef DRV_VXBEND_APM_PPRO_MAC
#   undef DRV_APM_PPRO_QMAN
#endif /* AMP && !INCLUDE_AMP_PPRO_END_DRIVER */

#ifdef DRV_VXBEND_APM_PPRO_MAC
void sysSocEthClkdivSet (UINT32, UINT32);
#endif /* DRV_VXBEND_APM_PPRO_MAC*/

/* device resources */

const struct intrCtlrInputs ppcIntCtlrInputs[] = {
    { 0,    "mpic",         0,  0 },
    { 5,    "mpic",         0,  1 },
    { 1,    "ppc440Timer",  0,  0 },
    { 3,    "ppc440Timer",  0,  2 },
    { 4,    "ppc440Timer",  0,  1 },
};
const struct hcfResource ppcIntCtlrResources[] = {
    { "regBase",            HCF_RES_INT,  { (void *) TRUE } },
    { "input",              HCF_RES_ADDR, { (void *) &ppcIntCtlrInputs[0] } },
    { "inputTableSize",     HCF_RES_INT,  { (void *) NELEMENTS (ppcIntCtlrInputs) } },
};
#define ppcIntCtlrNum   NELEMENTS(ppcIntCtlrResources)

struct hcfResource ppc440TimerResources[] =  {
    { "regBase",            HCF_RES_INT,  { (void *)0 } },
    { "decMinClkRate",      HCF_RES_INT,  { (void *)SYS_CLK_RATE_MIN } },
    { "decMaxClkRate",      HCF_RES_INT,  { (void *)SYS_CLK_RATE_MAX } },
    { "fitMinClkRate",      HCF_RES_INT,  { (void *)AUX_CLK_RATE_MIN } },
    { "fitMaxClkRate",      HCF_RES_INT,  { (void *)AUX_CLK_RATE_MAX } },
    { "wdtMinClkRate",      HCF_RES_INT,  { (void *)WDT_RATE_MIN     } },
    { "wdtMaxClkRate",      HCF_RES_INT,  { (void *)WDT_RATE_MAX     } },
    { "clkFreq",            HCF_RES_ADDR, { (void *)&sysClkFreqGet   } }
};
#define ppc440TimerNum  NELEMENTS(ppc440TimerResources)

#if defined(INCLUDE_AMP_UART_0) || !defined(INCLUDE_AMP_CPU)
const struct hcfResource ns1655x0Resources[] = {
    { "regBase",            HCF_RES_INT,  { (void *) APM_UART0_BASE } },
    { "clkFreq",            HCF_RES_INT,  { (void *) SYS_REFCLK } },
    { "regInterval",        HCF_RES_INT,  { (void *) 4 } },
};
#define ns1655x0Num NELEMENTS(ns1655x0Resources)
#endif /* INCLUDE_UART_0 || !INCLUDE_AMP_CPU */

#if defined(INCLUDE_AMP_UART_1) || !defined(INCLUDE_AMP_CPU)
const struct hcfResource ns1655x1Resources[] = {
    { "regBase",            HCF_RES_INT,  { (void *) APM_UART1_BASE } },
    { "clkFreq",            HCF_RES_INT,  { (void *) SYS_REFCLK } },
    { "regInterval",        HCF_RES_INT,  { (void *) 4 } },
};
#define ns1655x1Num NELEMENTS(ns1655x1Resources)

#   if  defined(INCLUDE_AMP_UART_0) || !defined(INCLUDE_AMP_CPU)
#       define uart1Unit   1
#   else
#       define uart1Unit   0
#   endif  /* INCLUDE_AMP_UART_0 || !INCLUDE_AMP_CPU */

#endif /* INCLUDE_AMP_UART_1 || !INCLUDE_AMP_CPU */

struct intrCtlrInputs mpicInputs[] = {
#ifdef ns1655x0Num
    { UART0_INT,            "ns16550",    0,  0 },
#endif /* ns1655x0Num */

#ifdef ns1655x1Num
    { UART1_INT,            "ns16550",    uart1Unit,  0 },
#endif /* ns1655x1Num */

#ifdef INCLUDE_USB
    { USB1_OHCI_INT,        "vxbPlbUsbOhci",  0,  0 },
    { USB1_EHCI_INT,        "vxbPlbUsbEhci",  0,  0 },
    { USB2_OHCI_INT,        "vxbPlbUsbOhci",  1,  0 },
    { USB2_EHCI_INT,        "vxbPlbUsbEhci",  1,  0 },
#endif /* INCLUDE_USB */
#ifdef DRV_APM_PPRO_QMAN
    { QM_ERR_INT,           "apmQman",        0,  0 },
    { QM_DEQMBOX0_INT,      "apmQman",        0,  1 },
    { QM_DEQMBOX1_INT,      "apmQman",        0,  2 },
    { QM_DEQMBOX2_INT,      "apmQman",        0,  3 },
    { QM_DEQMBOX3_INT,      "apmQman",        0,  4 },
    { QM_DEQMBOX4_INT,      "apmQman",        0,  5 },
    { QM_DEQMBOX5_INT,      "apmQman",        0,  6 },
    { QM_DEQMBOX6_INT,      "apmQman",        0,  7 },
    { QM_DEQMBOX7_INT,      "apmQman",        0,  8 },
    { QM_DEQMBOX8_INT,      "apmQman",        0,  9 },
    { QM_DEQMBOX9_INT,      "apmQman",        0,  10 },
    { QM_DEQMBOX10_INT,     "apmQman",        0,  11 },
    { QM_DEQMBOX11_INT,     "apmQman",        0,  12 },
    { QM_DEQMBOX12_INT,     "apmQman",        0,  13 },
    { QM_DEQMBOX13_INT,     "apmQman",        0,  14 },
    { QM_DEQMBOX14_INT,     "apmQman",        0,  15 },
    { QM_DEQMBOX15_INT,     "apmQman",        0,  16 },
#endif /* DRV_APM_PPRO_QMAN */
#ifdef DRV_VXBEND_APM_PPRO_MAC
    { ETH_CSR_INT,          "apmMac",         0,  0 },
    { ETH_MAC_INT,          "apmMac",         0,  1 },
    { ETH_QMI_INT,          "apmMac",         0,  2 },
#endif /* DRV_VXBEND_APM_PPRO_MAC */
#ifdef DRV_PCIBUS_APM_PCIEX
    { PCIEX0_INT_A,         "legacy",     0,  PCIEX0_INT_A },
    { PCIEX0_INT_B,         "legacy",     0,  PCIEX0_INT_B },
    { PCIEX0_INT_C,         "legacy",     0,  PCIEX0_INT_C },
    { PCIEX0_INT_D,         "legacy",     0,  PCIEX0_INT_D },
    { PCIEX1_INT_A,         "legacy",     0,  PCIEX1_INT_A },
    { PCIEX1_INT_B,         "legacy",     0,  PCIEX1_INT_B },
    { PCIEX1_INT_C,         "legacy",     0,  PCIEX1_INT_C },
    { PCIEX1_INT_D,         "legacy",     0,  PCIEX1_INT_D },
    { PCIEX2_INT_A,         "legacy",     0,  PCIEX2_INT_A },
    { PCIEX2_INT_B,         "legacy",     0,  PCIEX2_INT_B },
    { PCIEX2_INT_C,         "legacy",     0,  PCIEX2_INT_C },
    { PCIEX2_INT_D,         "legacy",     0,  PCIEX2_INT_D },
#endif /* DRV_PCIBUS_APM_PCIEX */
#ifdef DRV_STORAGE_SDHC
    { SDIO_INT,             "sdhci",          0,  0 },
    { SDIO_INT,             "sdhci",          1,  0 },
#endif /* DRV_STORAGE_SDHC */
#ifdef INCLUDE_DRV_STORAGE_AHCI
    { SATA_INT,             "ahciSata",       0,  0 },
#endif /* INCLUDE_DRV_STORAGE_AHCI */
};

struct intrCtlrPriority mpicPriority[] = {
    { UART0_INT,            10 },
    { UART1_INT,            10 },
#ifdef INCLUDE_USB
    { USB1_OHCI_INT,        10 },
    { USB1_EHCI_INT,        10 },
    { USB2_OHCI_INT,        10 },
    { USB2_EHCI_INT,        10 },
#endif /* INCLUDE_USB */
#ifdef DRV_APM_PPRO_QMAN
    { QM_ERR_INT,           10 },
    { QM_DEQMBOX0_INT,      10 },
    { QM_DEQMBOX1_INT,      10 },
    { QM_DEQMBOX2_INT,      10 },
    { QM_DEQMBOX3_INT,      10 },
    { QM_DEQMBOX4_INT,      10 },
    { QM_DEQMBOX5_INT,      10 },
    { QM_DEQMBOX6_INT,      10 },
    { QM_DEQMBOX7_INT,      10 },
    { QM_DEQMBOX8_INT,      10 },
    { QM_DEQMBOX9_INT,      10 },
    { QM_DEQMBOX10_INT,     10 },
    { QM_DEQMBOX11_INT,     10 },
    { QM_DEQMBOX12_INT,     10 },
    { QM_DEQMBOX13_INT,     10 },
    { QM_DEQMBOX14_INT,     10 },
    { QM_DEQMBOX15_INT,     10 },
#endif /* DRV_APM_PPRO_QMAN */
#ifdef DRV_VXBEND_APM_PPRO_MAC
    { ETH_CSR_INT,          10 },
    { ETH_MAC_INT,          10 },
    { ETH_QMI_INT,          10 },
#endif /* DRV_VXBEND_APM_PPRO_MAC */
#ifdef DRV_PCIBUS_APM_PCIEX
    { PCIEX0_INT_A,         10 },
    { PCIEX0_INT_B,         10 },
    { PCIEX0_INT_C,         10 },
    { PCIEX0_INT_D,         10 },
    { PCIEX1_INT_A,         10 },
    { PCIEX1_INT_B,         10 },
    { PCIEX1_INT_C,         10 },
    { PCIEX1_INT_D,         10 },
    { PCIEX2_INT_A,         10 },
    { PCIEX2_INT_B,         10 },
    { PCIEX2_INT_C,         10 },
    { PCIEX2_INT_D,         10 },
#endif /* DRV_PCIBUS_APM_PCIEX */
#ifdef DRV_STORAGE_SDHC
    { SDIO_INT,             10 },
#endif /* DRV_STORAGE_SDHC */
#ifdef INCLUDE_DRV_STORAGE_AHCI
    { SATA_INT,             10 },
#endif /* INCLUDE_DRV_STORAGE_AHCI */
};

struct intrCtlrTrigger mpicTrigger[] = {
    { PCIEX0_INT_A,         VXB_INTR_TRIG_LEVEL | VXB_INTR_TRIG_POS },
    { PCIEX0_INT_B,         VXB_INTR_TRIG_LEVEL | VXB_INTR_TRIG_POS },
    { PCIEX0_INT_C,         VXB_INTR_TRIG_LEVEL | VXB_INTR_TRIG_POS },
    { PCIEX0_INT_D,         VXB_INTR_TRIG_LEVEL | VXB_INTR_TRIG_POS },
    { PCIEX1_INT_A,         VXB_INTR_TRIG_LEVEL | VXB_INTR_TRIG_POS },
    { PCIEX1_INT_B,         VXB_INTR_TRIG_LEVEL | VXB_INTR_TRIG_POS },
    { PCIEX1_INT_C,         VXB_INTR_TRIG_LEVEL | VXB_INTR_TRIG_POS },
    { PCIEX1_INT_D,         VXB_INTR_TRIG_LEVEL | VXB_INTR_TRIG_POS },
    { PCIEX2_INT_A,         VXB_INTR_TRIG_LEVEL | VXB_INTR_TRIG_POS },
    { PCIEX2_INT_B,         VXB_INTR_TRIG_LEVEL | VXB_INTR_TRIG_POS },
    { PCIEX2_INT_C,         VXB_INTR_TRIG_LEVEL | VXB_INTR_TRIG_POS },
    { PCIEX2_INT_D,         VXB_INTR_TRIG_LEVEL | VXB_INTR_TRIG_POS },
    { MPIC_IPI0_VEC,        VXB_INTR_TRIG_EDGE },
    { MPIC_IPI1_VEC,        VXB_INTR_TRIG_EDGE },
    { MPIC_IPI2_VEC,        VXB_INTR_TRIG_EDGE },
    { MPIC_IPI3_VEC,        VXB_INTR_TRIG_EDGE },
};

const struct hcfResource mpic0Resources[] = {
    { VXB_REG_BASE,         HCF_RES_INT,  { (void *) APM_MPIC_BASE } },
    { "input",              HCF_RES_ADDR, { (void *) &mpicInputs[0] } },
    { "inputTableSize",     HCF_RES_INT,  { (void *) NELEMENTS(mpicInputs) } },
    { "priority",           HCF_RES_ADDR, { (void *) &mpicPriority[0] } },
    { "priorityTableSize",  HCF_RES_INT,  { (void *) NELEMENTS(mpicPriority) } },
    { "trigger",            HCF_RES_ADDR, { (void *) &mpicTrigger[0] } },
    { "triggerTableSize",   HCF_RES_INT,  { (void *) NELEMENTS(mpicTrigger) } },
    { "flags",              HCF_RES_INT,  { (void *) (MPIC_REG_MEM      | \
                                                      MPIC_QUIRK_WHOAMI | \
                                                      MPIC_QUIRK_NO_REG | \
                                                      MPIC_QUIRK_IACK) } },
};
#define mpic0Num NELEMENTS(mpic0Resources)

#ifdef DRV_APM_PPRO_QMAN
LOCAL const struct hcfResource apmQman0Resources[] = {
    { "regBase",        HCF_RES_INT,    { (void *) (APM_QMAN_CSR_BASE) } },
    { "regBase1",       HCF_RES_INT,    { (void *) (APM_QMAN_HBF_BASE) } },
    { "mboxBasePhyHi",  HCF_RES_INT,    { (void *) (APM_QMAN_MBOX_PHY_BASE_HI) } },
    { "mboxBasePhyLo",  HCF_RES_INT,    { (void *) (APM_QMAN_MBOX_PHY_BASE) } },
    { "mboxBase",       HCF_RES_INT,    { (void *) (APM_QMAN_MBOX_BASE) } },
    { "l2SnoopEn",      HCF_RES_INT,    { (void *) 1 } }
    };
#define apmQman0Num NELEMENTS(apmQman0Resources)
#endif /* DRV_APM_PPRO_QMAN */

#ifdef DRV_VXBEND_APM_PPRO_MAC
struct hcfResource apmMac0Resources[]= {
    { "regBase",        HCF_RES_INT,    { (void *) (APM_ETH_BASE) } },
    { "regBase1",       HCF_RES_INT,    { (void *) (APM_CLE_BASE) } },
    { "miiPort",        HCF_RES_INT,    { (void *) 0 } },
    { "errIntEn",       HCF_RES_INT,    { (void *) 1 } },
    { "clkdivSet",      HCF_RES_ADDR,   { (void *) &sysSocEthClkdivSet } },
    { "phyAddr0",       HCF_RES_INT,    { (void *) 1 } },
    { "phyAddr1",       HCF_RES_INT,    { (void *) 2 } }
    };
#define apmMac0Num NELEMENTS(apmMac0Resources)

#endif /* DRV_VXBEND_APM_PPRO_MAC */


#ifdef DRV_PCIBUS_APM_PCIEX

const struct hcfResource apmPciEx0Resources[] = {
    { "regBase",          HCF_RES_INT,  { (void *) APM_PCIEX0_CSR_BASE } },
    { "regBase1",         HCF_RES_INT,  { (void *) APM_PCIEX0_CFG_BASE } },
    { "memPhyAddrHi",     HCF_RES_INT,  { (void *) PCIEX0_MEM_PHY_ADRS_HI } },
    { "mem32Addr",        HCF_RES_ADDR, { (void *) PCIEX0_MEM_ADRS } },
    { "mem32Size",        HCF_RES_INT,  { (void *) PCIEX0_MEM_SIZE } },
    { "memIo32Addr",      HCF_RES_ADDR, { (void *) PCIEX0_MEMIO_ADRS } },
    { "memIo32Size",      HCF_RES_INT,  { (void *) PCIEX0_MEMIO_SIZE } },
    { "io16Addr",         HCF_RES_ADDR, { (void *) PCIEX0_IO_ADRS } },
    { "io16Size",         HCF_RES_INT,  { (void *) PCIEX0_IO_SIZE } },
    { "serdesInit",       HCF_RES_ADDR, { (void *) sysPciEx0SerdesInit } },
    { "maxBusSet",        HCF_RES_INT,  { (void *) 15 } },
    { "autoConfig",       HCF_RES_INT,  { (void *) 1 } },
    { "includeFuncSet",   HCF_RES_ADDR, { (void *) sysPciEx0AutoCfgInclude } },
    { "intAssignFuncSet", HCF_RES_ADDR, { (void *) sysPciEx0AutoCfgIntAssign } }
};
#define apmPciEx0Num NELEMENTS(apmPciEx0Resources)

const struct hcfResource apmPciEx1Resources[] = {
    { "regBase",          HCF_RES_INT,  { (void *) APM_PCIEX1_CSR_BASE } },
    { "regBase1",         HCF_RES_INT,  { (void *) APM_PCIEX1_CFG_BASE } },
    { "memPhyAddrHi",     HCF_RES_INT,  { (void *) PCIEX1_MEM_PHY_ADRS_HI } },
    { "mem32Addr",        HCF_RES_ADDR, { (void *) PCIEX1_MEM_ADRS } },
    { "mem32Size",        HCF_RES_INT,  { (void *) PCIEX1_MEM_SIZE } },
    { "memIo32Addr",      HCF_RES_ADDR, { (void *) PCIEX1_MEMIO_ADRS } },
    { "memIo32Size",      HCF_RES_INT,  { (void *) PCIEX1_MEMIO_SIZE } },
    { "io16Addr",         HCF_RES_ADDR, { (void *) PCIEX1_IO_ADRS } },
    { "io16Size",         HCF_RES_INT,  { (void *) PCIEX1_IO_SIZE } },
    { "serdesInit",       HCF_RES_ADDR, { (void *) sysPciEx1SerdesInit } },
    { "maxBusSet",        HCF_RES_INT,  { (void *) 15 } },
    { "autoConfig",       HCF_RES_INT,  { (void *) 1 } },
    { "includeFuncSet",   HCF_RES_ADDR, { (void *) sysPciEx1AutoCfgInclude } },
    { "intAssignFuncSet", HCF_RES_ADDR, { (void *) sysPciEx1AutoCfgIntAssign } }
};
#define apmPciEx1Num NELEMENTS(apmPciEx1Resources)

const struct hcfResource apmPciEx2Resources[] = {
    { "regBase",          HCF_RES_INT,  { (void *) APM_PCIEX2_CSR_BASE } },
    { "regBase1",         HCF_RES_INT,  { (void *) APM_PCIEX2_CFG_BASE } },
    { "memPhyAddrHi",     HCF_RES_INT,  { (void *) PCIEX2_MEM_PHY_ADRS_HI } },
    { "mem32Addr",        HCF_RES_ADDR, { (void *) PCIEX2_MEM_ADRS } },
    { "mem32Size",        HCF_RES_INT,  { (void *) PCIEX2_MEM_SIZE } },
    { "memIo32Addr",      HCF_RES_ADDR, { (void *) PCIEX2_MEMIO_ADRS } },
    { "memIo32Size",      HCF_RES_INT,  { (void *) PCIEX2_MEMIO_SIZE } },
    { "io16Addr",         HCF_RES_ADDR, { (void *) PCIEX2_IO_ADRS } },
    { "io16Size",         HCF_RES_INT,  { (void *) PCIEX2_IO_SIZE } },
    { "serdesInit",       HCF_RES_ADDR, { (void *) sysPciEx2SerdesInit } },
    { "maxBusSet",        HCF_RES_INT,  { (void *) 15 } },
    { "autoConfig",       HCF_RES_INT,  { (void *) 1 } },
    { "includeFuncSet",   HCF_RES_ADDR, { (void *) sysPciEx2AutoCfgInclude } },
    { "intAssignFuncSet", HCF_RES_ADDR, { (void *) sysPciEx2AutoCfgIntAssign } }
};
#define apmPciEx2Num NELEMENTS(apmPciEx2Resources)

#endif

#ifdef DRV_STORAGE_SDHC

struct hcfResource sdhc0Resources[] = {
    { "regBase",          HCF_RES_INT,  { (void *)APM_SDHC0_BASE } },
    { "clkFreq",          HCF_RES_ADDR, { (void *)sysSdhcClkFreqGet } },
    { "dmaMode",          HCF_RES_INT,  { (void *)SDHC_DMA_MODE_SDMA } },
    { "polling",          HCF_RES_INT,  { (void *)0 } },
    { "flags" ,           HCF_RES_INT,  { (void *)SDHC_HOST_CTRL_APM } },
};
#define sdhc0Num  NELEMENTS(sdhc0Resources)

struct hcfResource sdhc1Resources[] = {
    { "regBase",          HCF_RES_INT,  { (void *)APM_SDHC1_BASE } },
    { "clkFreq",          HCF_RES_ADDR, { (void *)sysSdhcClkFreqGet } },
    { "dmaMode",          HCF_RES_INT,  { (void *)SDHC_DMA_MODE_SDMA } },
    { "polling",          HCF_RES_INT,  { (void *)0 } },
    { "flags" ,           HCF_RES_INT,  { (void *)SDHC_HOST_CTRL_APM } },
};
#define sdhc1Num  NELEMENTS(sdhc1Resources)
#endif  /* DRV_STORAGE_SDHC */

#ifdef INCLUDE_DRV_STORAGE_AHCI

const struct hcfResource vxbApm86xxxSataResources[] = {
    { "regBase",          HCF_RES_INT,  { (void *)(APM_SATA_BASE) } },
    };
#define apmSataNum NELEMENTS(vxbApm86xxxSataResources)

#endif /* INCLUDE_DRV_STORAGE_AHCI */

#ifdef INCLUDE_USB
const struct hcfResource apmOhci0Resources[]= {
    { "regBase",          HCF_RES_INT,  { (void *) APM_OHCI1_BASE } },
    { "dataSwap",         HCF_RES_ADDR, { (void *) vxbSwap32 } },
};
#define apmOhci0Num NELEMENTS(apmOhci0Resources)

const struct hcfResource apmEhci0Resources[]= {
    { "regBase",          HCF_RES_INT,  { (void *) APM_EHCI1_BASE } },
};
#define apmEhci0Num NELEMENTS(apmEhci0Resources)

const struct hcfResource apmOhci1Resources[]= {
    { "regBase",          HCF_RES_INT,  { (void *) APM_OHCI2_BASE } },
    { "dataSwap",         HCF_RES_ADDR, { (void *) vxbSwap32 } },
};
#define apmOhci1Num NELEMENTS(apmOhci1Resources)

const struct hcfResource apmEhci1Resources[]= {
    { "regBase",          HCF_RES_INT,  { (void *) APM_EHCI2_BASE } },
};
#define apmEhci1Num NELEMENTS(apmEhci1Resources)
#endif /* INCLUDE_USB */

const struct hcfDevice hcfDeviceList[] = {
    { "ppcIntCtlr",     0,  VXB_BUSID_PLB,  0,  ppcIntCtlrNum,  ppcIntCtlrResources },
    { "mpic",           0,  VXB_BUSID_PLB,  0,  mpic0Num,       mpic0Resources},
    { "ppc440Timer",    0,  VXB_BUSID_PLB,  0,  ppc440TimerNum, ppc440TimerResources},

#ifdef ns1655x0Num
    { "ns16550",        0,  VXB_BUSID_PLB,  0,  ns1655x0Num,    ns1655x0Resources },
#endif /* ns1655x0Num */

#ifdef ns1655x1Num
    { "ns16550",        uart1Unit,  VXB_BUSID_PLB,  0,  ns1655x1Num,    ns1655x1Resources },
#endif /* ns1655x1Num */

#ifdef DRV_APM_PPRO_QMAN
    { "apmQman",        0,  VXB_BUSID_PLB,  0,  apmQman0Num,    apmQman0Resources},
#endif /* DRV_APM_PPRO_QMAN */

#ifdef DRV_VXBEND_APM_PPRO_MAC
    { "apmMac",         0,  VXB_BUSID_PLB,  0,  apmMac0Num,     apmMac0Resources},
#endif /* DRV_VXBEND_APM_PPRO_MAC */

#ifdef DRV_PCIBUS_APM_PCIEX
#   if (PCIEX_PORT0_ENABLE == TRUE)
    { "apmPciEx",       0,  VXB_BUSID_PLB,  0,  apmPciEx0Num,   apmPciEx0Resources },
#   endif /* PCIEX_PORT0_ENABLE */
    { "apmPciEx",       1,  VXB_BUSID_PLB,  0,  apmPciEx1Num,   apmPciEx1Resources },
    { "apmPciEx",       2,  VXB_BUSID_PLB,  0,  apmPciEx2Num,   apmPciEx2Resources },
#endif
#ifdef DRV_STORAGE_SDHC
    { "sdhci",          0, VXB_BUSID_PLB,   0,  sdhc0Num,       sdhc0Resources },
    { "sdhci",          1, VXB_BUSID_PLB,   0,  sdhc1Num,       sdhc1Resources },
#endif /* DRV_STORAGE_SDHC */
#ifdef INCLUDE_DRV_STORAGE_AHCI
    { "ahciSata",       0, VXB_BUSID_PLB,   0,  apmSataNum,     vxbApm86xxxSataResources },
#endif /* INCLUDE_DRV_STORAGE_AHCI */

#ifdef INCLUDE_USB
    { "vxbPlbUsbOhci",  0,  VXB_BUSID_PLB,  0,  apmOhci0Num,    apmOhci0Resources },
    { "vxbPlbUsbOhci",  1,  VXB_BUSID_PLB,  0,  apmOhci1Num,    apmOhci1Resources },
    { "vxbPlbUsbEhci",  0,  VXB_BUSID_PLB,  0,  apmEhci0Num,    apmEhci0Resources },
    { "vxbPlbUsbEhci",  1,  VXB_BUSID_PLB,  0,  apmEhci1Num,    apmEhci1Resources },
#endif /* INCLUDE_USB */

};
const int hcfDeviceNum = NELEMENTS(hcfDeviceList);

VXB_INST_PARAM_OVERRIDE sysInstParamTable[] = {
    { NULL, 0, NULL, VXB_PARAM_END_OF_LIST, { (void *)0 } }
};
