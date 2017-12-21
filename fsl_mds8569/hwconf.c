/* hwconf.c - hardware configuration support module */

/*
 * Copyright (c) 2009, 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01d,07jul11,y_c  Support VxWorks native bootrom and SD/MMC card. (WIND00240156)
01c,15oct09,x_z  add support for UCC SIO
01b,10sep09,x_z  update for new QUICC Engine driver and UCC driver
01a,10jul09,x_z  derived from version 01g of fsl_mmds8568/hwconf.c.
*/

/* includes */

#include <vxWorks.h>
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/vxbPlbLib.h>
#include <hwif/vxbus/hwConf.h>
#include <hwif/util/vxbParamSys.h>
#include <hwif/vxbus/vxbIntrCtlr.h>
#include <hwif/intCtlr/vxbEpicIntCtlr.h>
#include <hwif/intCtlr/vxbQeIntCtlr.h>
#include <../src/hwif/h/end/vxbUccEnd.h>
#include <../src/hwif/h/mii/mv88E1x11Phy.h>

#ifdef DRV_PCIBUS_M85XX
#   define MPC85XX_PCIE_MEMORY_OWATTR   (PCI_WINDOW_ENABLE_BIT | \
                                         PCI_OUT_ATTR_RTT_MEM | \
                                         PCI_OUT_ATTR_WTT_MEM)
#   define MPC85XX_PCIE_MEMORYIO_OWATTR (PCI_WINDOW_ENABLE_BIT | \
                                         PCI_OUT_ATTR_RTT_MEM | \
                                         PCI_OUT_ATTR_WTT_MEM)
#   define MPC85XX_PCIE_IO_OWATTR       (PCI_WINDOW_ENABLE_BIT | \
                                         PCI_OUT_ATTR_RTT_IO | \
                                         PCI_OUT_ATTR_WTT_IO)
#   define MPC85XX_PCIE_IWATTR          (PCI_WINDOW_ENABLE_BIT | \
                                         PCI_PREFETCHABLE | \
                                         PCI_IN_ATTR_RTT_LM_READ_SNOOP | \
                                         PCI_IN_ATTR_RTT_LM_WRITE_SNOOP | \
                                         PCI_IN_ATTR_TGI_LM)
#   include <drv/pci/pciAutoConfigLib.h>
#endif

#ifdef DRV_STORAGE_SDHC
#   include <h/storage/vxbSdMmcLib.h>
#   include <h/storage/vxbSdhcStorage.h>
#endif /* DRV_STORAGE_SDHC */

#include <../src/hwif/h/resource/qeIOPort.h>
#include <../src/hwif/h/resource/qeDpramLib.h>

#include "config.h"

/* externs */

#ifdef DRV_PCIBUS_M85XX
IMPORT STATUS sysPciExAutoconfigInclude    (PCI_SYSTEM *, PCI_LOC *, UINT);
IMPORT UCHAR  sysPciExAutoconfigIntrAssign (PCI_SYSTEM *, PCI_LOC *, UCHAR);
#endif

const struct intrCtlrInputs ppcIntCtlrInputs[] = {
    { 0,                  "epic", 0, 0 },
    { 1,                  "m85xxTimerDev", 0, 0 },
    { 3,                  "m85xxTimerDev", 0, 2 },
    { 4,                  "m85xxTimerDev", 0, 1 }
 };

const struct hcfResource ppcIntCtlr0Resources[] = {
    { "regBase",          HCF_RES_INT,  { (void *) TRUE } },
    { "input",            HCF_RES_ADDR, { (void *) &ppcIntCtlrInputs[0] } },
    { "inputTableSize",   HCF_RES_INT,  { (void *) NELEMENTS(ppcIntCtlrInputs) } }
 };
#define ppcIntCtlr0Num NELEMENTS(ppcIntCtlr0Resources)

struct intrCtlrInputs epicInputs[] = {
#ifdef DRV_SIO_NS16550
    { EPIC_DUART_INT_VEC, "ns16550",            0, 0 },
    { EPIC_DUART_INT_VEC, "ns16550",            1, 0 },
#endif /* DRV_SIO_NS16550 */
    { INUM_QE_HIGH,       "quiccEngineIntCtlr", 0, 0 },
    { INUM_QE_LOW,        "quiccEngineIntCtlr", 0, 1 },
#ifdef DRV_STORAGE_SDHC
    { INUM_ESDHC,         "sdhci",              0, 0 }
#endif /* DRV_STORAGE_SDHC */
 };

INT_BANK_DESC epicIntDesc[] = {
    { 0,   EPIC_EX_IRQ_NUM,   EPIC_EX_REG_BASE,   EPIC_EX_REG_INTERVAL,   EPIC_EX_INTERRUPT   },
    { 0,   EPIC_IN_IRQ_NUM,   EPIC_IN_REG_BASE,   EPIC_IN_REG_INTERVAL,   EPIC_IN_INTERRUPT   },
    { 0,   EPIC_GT_IRQ_NUM,   EPIC_GT_A_REG_BASE, EPIC_GT_REG_INTERVAL,   EPIC_GT_A_INTERRUPT },
    { 0,   EPIC_GT_IRQ_NUM,   EPIC_GT_B_REG_BASE, EPIC_GT_REG_INTERVAL,   EPIC_GT_B_INTERRUPT },
    { 0,   EPIC_MSG_IRQ_NUM,  EPIC_MSG_REG_BASE,  EPIC_MSG_REG_INTERVAL,  EPIC_MSG_INTERRUPT  },
    { 0,   EPIC_SMSG_IRQ_NUM, EPIC_SMSG_REG_BASE, EPIC_SMSG_REG_INTERVAL, EPIC_SMSG_INTERRUPT },
    { 251, EPIC_IPI_IRQ_NUM,  EPIC_IPI_REG_BASE,  EPIC_IPI_REG_INTERVAL,  EPIC_IPI_INTERRUPT  }
};

const struct hcfResource epic0Resources[] = {
    { "regBase",          HCF_RES_INT,  { (void *) CCSBAR } },
    { "input",            HCF_RES_ADDR, { (void *) &epicInputs[0] } },
    { "inputTableSize",   HCF_RES_INT,  { (void *) NELEMENTS(epicInputs) } },
    { "numCpus",          HCF_RES_INT,  { (void *) 1 } },
#ifdef DRV_PCIBUS_M85XX
    { "exPolar",          HCF_RES_INT,  { (void *) EPIC_INT_ACT_LOW } },
#endif
    { "intDesc_0",        HCF_RES_ADDR, { (void *)&epicIntDesc[0] } },
    { "intDesc_1",        HCF_RES_ADDR, { (void *)&epicIntDesc[1] } },
    { "intDesc_2",        HCF_RES_ADDR, { (void *)&epicIntDesc[2] } },
    { "intDesc_3",        HCF_RES_ADDR, { (void *)&epicIntDesc[3] } },
    { "intDesc_4",        HCF_RES_ADDR, { (void *)&epicIntDesc[4] } },
    { "intDesc_5",        HCF_RES_ADDR, { (void *)&epicIntDesc[5] } },
    { "intDesc_6",        HCF_RES_ADDR, { (void *)&epicIntDesc[6] } },
 };
#define epic0Num NELEMENTS(epic0Resources)

struct hcfResource m85xxTimerResources[] =  {
    { "regBase",          HCF_RES_INT,  { (void *) 0 } },
    { "decMinClkRate",    HCF_RES_INT,  { (void *) SYS_CLK_RATE_MIN } },
    { "decMaxClkRate",    HCF_RES_INT,  { (void *) SYS_CLK_RATE_MAX } },
#ifdef INCLUDE_AUX_CLK
    { "fitMinClkRate",    HCF_RES_INT,  { (void *) AUX_CLK_RATE_MIN } },
    { "fitMaxClkRate",    HCF_RES_INT,  { (void *) AUX_CLK_RATE_MAX } },
#endif
    { "sysClockFreq",    HCF_RES_ADDR,   { (void *) &sysClkFreqGet } }
 };
#define m85xxTimerNum   NELEMENTS(m85xxTimerResources)

const struct hcfResource ns165500Resources[] = {
    { "regBase",          HCF_RES_INT,  { (void *) (CCSBAR + M85XX_UART0_BASE) } },
    { "clkFreq",          HCF_RES_ADDR, { (void *) sysClkFreqGet } },
    { "regInterval",      HCF_RES_INT,  { (void *) 1 } }
 };
#define ns165500Num NELEMENTS(ns165500Resources)

const struct hcfResource ns165501Resources[] = {
    { "regBase",          HCF_RES_INT,  { (void *) (CCSBAR + M85XX_UART1_BASE) } },
    { "clkFreq",          HCF_RES_ADDR, { (void *) sysClkFreqGet } },
    { "regInterval",      HCF_RES_INT,  { (void *) 1 } }
 };
#define ns165501Num NELEMENTS(ns165501Resources)

struct intrCtlrInputs quiccEngineIntCtlrInputs[] = {
    { QE_INUM_UCC1,       "qefcc",         0, 0 },

    /*
     * UCC2 works with UART mode when DRV_SIO_UCC is defined, so replace it with
     * UCC6 for qefcc1.
     */

#ifndef DRV_SIO_UCC
    { QE_INUM_UCC2,       "qefcc",         1, 0 },
#else
    { QE_INUM_UCC2,       "qeUart",        0, 0 },
    { QE_INUM_UCC6,       "qefcc",         1, 0 },
#endif /* DRV_SIO_UCC */
    { QE_INUM_UCC3,       "qefcc",         2, 0 },
    { QE_INUM_UCC4,       "qefcc",         3, 0 }
 };

const QE_SIU_INT_MASK qeEnableHighPriInts[QE_IVEC_MAX] = {
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 }
 };

const UINT32 qeINumToMaskPattern[QE_IVEC_MAX] = {
    0x00000000,          /* interrupt number 0: Error */
    0x00008000,          /* interrupt number 1: I2C */
    0x00004000,          /* interrupt number 2: SPI */
    0x00002000,          /* interrupt number 3: Risc Timer Table */
    0x00000000,          /* interrupt number 4: */
    0x00000000,          /* interrupt number 5: */
    0x00000000,          /* interrupt number 6:  */
    0x00000000,          /* interrupt number 7:  */
    0x00000000,          /* interrupt number 8:  */
    0x00000000,          /* interrupt number 9:  */
    0x00000040,          /* interrupt number 10: SDMA */
    0x00000020,          /* interrupt number 11: USB */
    0x00000010,          /* interrupt number 12: Timer 1 */
    0x00000008,          /* interrupt number 13: Timer 2 */
    0x00000004,          /* interrupt number 14: Timer 3 */
    0x00000002,          /* interrupt number 15: Timer 4 */
    0x00000000,          /* interrupt number 16: */
    0x00000000,          /* interrupt number 17: */
    0x00000000,          /* interrupt number 18: */
    0x00000000,          /* interrupt number 19: */
    0x10000000,          /* interrupt number 20: VT*/
    0x00000000,          /* interrupt number 21: */
    0x00000000,          /* interrupt number 22: */
    0x00000000,          /* interrupt number 23: */
    0x00000000,          /* interrupt number 24: */
    0x00800000,          /* interrupt number 25: EXT1 */
    0x00400000,          /* interrupt number 26: EXT2 */
    0x00200000,          /* interrupt number 27: EXT3 */
    0x00100000,          /* interrupt number 28: EXT4 */
    0x00000000,          /* interrupt number 29: */
    0x00000000,          /* interrupt number 30: */
    0x00000000,          /* interrupt number 31: */
    0x80000000,          /* interrupt number 32: UCC1 */
    0x40000000,          /* interrupt number 33: UCC2 */
    0x20000000,          /* interrupt number 34: UCC3 */
    0x10000000,          /* interrupt number 35: UCC4 */
    0x08000000,          /* interrupt number 36: MCC1 */
    0x00000000,          /* interrupt number 37: */
    0x00000000,          /* interrupt number 38: */
    0x00000000,          /* interrupt number 39: */
    0x00800000,          /* interrupt number 40: UCC5 */
    0x00400000,          /* interrupt number 41: UCC6 */
    0x00200000,          /* interrupt number 42: UCC7 */
    0x00100000,          /* interrupt number 43: UCC8 */
    0x00000000,          /* interrupt number 44: */
    0x00000000,          /* interrupt number 45: */
    0x00000000,          /* interrupt number 46: */
    0x00000000,          /* interrupt number 47: */
    0x00000000,          /* interrupt number 48: */
    0x00000000,          /* interrupt number 49: */
    0x00000000,          /* interrupt number 50: */
    0x00000000,          /* interrupt number 51: */
    0x00000000,          /* interrupt number 52: */
    0x00000000,          /* interrupt number 53: */
    0x00000000,          /* interrupt number 54: */
    0x00000000,          /* interrupt number 55: */
    0x00000000,          /* interrupt number 56: reserved */
    0x00000000,          /* interrupt number 57: reserved */
    0x00000000,          /* interrupt number 58: reserved */
    0x00000000,          /* interrupt number 59: reserved */
    0x00000000,          /* interrupt number 60: reserved */
    0x00000000,          /* interrupt number 61: reserved */
    0x00000000,          /* interrupt number 62: reserved */
    0x00000000           /* interrupt number 63: reserved */
 };

const struct hcfResource quiccEngineIntCtlr0Resources[] = {
    { "regBase",           HCF_RES_INT,  { (void *) QUICC_ENGINE_BA } },
    { "input",             HCF_RES_ADDR, { (void *) &quiccEngineIntCtlrInputs[0] } },
    { "inputTableSize",    HCF_RES_INT,  { (void *) NELEMENTS(quiccEngineIntCtlrInputs) } },
    { "intMaskTable",      HCF_RES_ADDR, { (void *) &qeEnableHighPriInts[0] } },
    { "intMaskTableSize",  HCF_RES_INT,  { (void *) QE_IVEC_MAX } },
    { "inumMaskTable",     HCF_RES_ADDR, { (void *) &qeINumToMaskPattern[0] } },
    { "inumMaskTableSize", HCF_RES_INT,  { (void *) QE_IVEC_MAX } }
 };
#define quiccEngineIntCtlr0Num NELEMENTS(quiccEngineIntCtlr0Resources)

const QE_IOPORT_TABLE qeIoTable[] = {

    /* QE_MUX_MDC */

    { 2, 31, 1, 0, 1 }, /* QE_MUX_MDC               */

    /* QE_MUX_MDIO */

    { 2, 30, 3, 0, 2 }, /* QE_MUX_MDIO              */

    /* UCC_1_RGMII */

    { 2, 11, 2, 0, 1 }, /* CLK12 */
    { 0, 0,  1, 0, 3 }, /* ENET1_TXD0_SER1_TXD0      */
    { 0, 1,  1, 0, 3 }, /* ENET1_TXD1_SER1_TXD1      */
    { 0, 2,  1, 0, 1 }, /* ENET1_TXD2_SER1_TXD2      */
    { 0, 3,  1, 0, 2 }, /* ENET1_TXD3_SER1_TXD3      */
    { 0, 6,  2, 0, 3 }, /* ENET1_RXD0_SER1_RXD0      */
    { 0, 7,  2, 0, 1 }, /* ENET1_RXD1_SER1_RXD1      */
    { 0, 8,  2, 0, 2 }, /* ENET1_RXD2_SER1_RXD2      */
    { 0, 9,  2, 0, 2 }, /* ENET1_RXD3_SER1_RXD3      */
    { 0, 4,  1, 0, 2 }, /* ENET1_TX_EN_SER1_RTS_B    */
    { 0, 12, 2, 0, 3 }, /* ENET1_RX_DV_SER1_CTS_B    */
    { 2, 8,  2, 0, 1 }, /* ENET1_GRXCLK              */
    { 2, 20, 1, 0, 2 }, /* ENET1_GTXCLK              */

#ifdef DRV_SIO_UCC
    /* QE UART */

    { 0, 19, 1, 0, 2 }, /* QEUART_TX            */
    { 1, 17, 2, 0, 3 }, /* QEUART_RX            */

    /* pins for UCC_2_RGMII must be clean for QE UART */

    { 0, 14, 0, 0, 0 }, /* ENET2_TXD0_SER2_TXD0   */
    { 0, 15, 0, 0, 0 }, /* ENET2_TXD1_SER2_TXD1   */
    { 0, 16, 0, 0, 0 }, /* ENET2_TXD2_SER2_TXD2   */
    { 0, 17, 0, 0, 0 }, /* ENET2_TXD3_SER2_TXD3   */
    { 0, 20, 0, 0, 0 }, /* ENET2_RXD0_SER2_RXD0   */
    { 0, 21, 0, 0, 0 }, /* ENET2_RXD1_SER2_RXD1   */
    { 0, 22, 0, 0, 0 }, /* ENET2_RXD2_SER2_RXD2   */
    { 0, 23, 0, 0, 0 }, /* ENET2_RXD3_SER2_RXD3   */
    { 0, 18, 0, 0, 0 }, /* ENET2_TX_EN_SER2_RTS_B */
    { 0, 26, 0, 0, 0 }, /* ENET2_RX_DV_SER2_CTS_B */
#else
    /* UCC_2_RGMII */

    { 2, 16, 2, 0, 3 }, /* CLK17 */
    { 0, 14, 1, 0, 2 }, /* ENET2_TXD0_SER2_TXD0      */
    { 0, 15, 1, 0, 2 }, /* ENET2_TXD1_SER2_TXD1      */
    { 0, 16, 1, 0, 1 }, /* ENET2_TXD2_SER2_TXD2      */
    { 0, 17, 1, 0, 1 }, /* ENET2_TXD3_SER2_TXD3      */
    { 0, 20, 2, 0, 2 }, /* ENET2_RXD0_SER2_RXD0      */
    { 0, 21, 2, 0, 1 }, /* ENET2_RXD1_SER2_RXD1      */
    { 0, 22, 2, 0, 1 }, /* ENET2_RXD2_SER2_RXD2      */
    { 0, 23, 2, 0, 1 }, /* ENET2_RXD3_SER2_RXD3      */
    { 0, 18, 1, 0, 2 }, /* ENET2_TX_EN_SER2_RTS_B    */
    { 0, 26, 2, 0, 3 }, /* ENET2_RX_DV_SER2_CTS_B    */
    { 2, 3,  2, 0, 1 }, /* ENET2_GRXCLK              */
    { 2, 2,  1, 0, 2 }, /* ENET2_GTXCLK              */
#endif /* DRV_SIO_UCC */

    /* UART1 is muxed with QE PortF bit [9-12].*/

    { 5, 12, 2, 0, 3 }, /* UART1_SIN */
    { 5, 9,  1, 0, 3 }, /* UART1_SOUT */
    { 5, 10, 2, 0, 3 }, /* UART1_CTS_B */
    { 5, 11, 1, 0, 2 }, /* UART1_RTS_B */

};

IMPORT UINT32 _binary_fsl_qe_ucode_8569_10_bin_start[];

const struct hcfResource quiccEngineResources[] = {
    { "regBase",           HCF_RES_INT,  { (void *) QUICC_ENGINE_BA } },
    { "regBase1",          HCF_RES_INT,  { (void *) QE_IO_PORTS } },
    { "muRamBase",         HCF_RES_INT,  { (void *) QE_MURAM_BA } },
    { "muRamPoolSize",     HCF_RES_INT,  { (void *) QE_MURAM_SIZE } },
    { "firmwareAdrs",      HCF_RES_INT,  { (void *) _binary_fsl_qe_ucode_8569_10_bin_start } },
    { "qeIoTable",         HCF_RES_ADDR, { (void *) &qeIoTable[0] } },
    { "qeIoTableSize",     HCF_RES_INT,  { (void *) NELEMENTS(qeIoTable) } }	
 };
#define quiccEngine0Num NELEMENTS(quiccEngineResources)

#ifdef DRV_SIO_UCC
struct hcfResource uccSioDev0Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *) UCC2_BA } },
    { "regBase1",   HCF_RES_INT,    { (void *) QUICC_ENGINE_BA } },
    { "uccNum",     HCF_RES_INT,    { (void *) 2 } },
    { "qeFreq",     HCF_RES_ADDR,   { (void *) sysQeFreqGet } },
    { "rxBrgNum",   HCF_RES_INT,    { (void *) 9 } },
    { "txBrgNum",   HCF_RES_INT,    { (void *) 9 } }
};
#define uccSio0Num NELEMENTS(uccSioDev0Resources)
#endif /* DRV_SIO_UCC */

/*
 * On the Freescale MDS8569 board, the 6 Ethernet interfaces are supported -
 * UCC1/UCC2/UCC3/UCC4/UCC6/UCC8, but only the 4 ones can be supported by the
 * processor at the same time. UCC6 and UCC8 share the SERDES Lane E and F with
 * the SRIOs, and the SW8 must be 'configured to enbale SGMII mode for Lane E
 * and F.
 *
 * PHYs are physically wired according to the following table:
 *
 * Data port pins       MII address     TBI address
 * --------------       -----------     -----------
 * PHY0: UCC1           7               N/A
 * PHY1: UCC2           1               N/A
 * PHY2: UCC3           2               N/A
 * PHY3: UCC4           3               N/A
 * PHY4: UCC6           4               30
 * PHY5: UCC8           6               30
 *
 * 1. All the 6 PHYs have their management pins connected to UCC1.
 *
 * 2. The UCC1/UCC2/UCC3/UCC4 is used as qefcc0/qefcc1/qefcc2/qefcc3 by default.
 *
 * 3. The UCC2 works with UART mode when DRV_SIO_UCC is defined, so replace it
 * with UCC6. In other words, the UCC6 will be used as qefcc1 and the UCC2 will
 * be used qeUart.
 *
 * 4. UCC6 and UCC8 work with SGMII mode. They share the SERDES Lane E and F
 * with the SRIOs, and the SW8 must be configured to enbale SGMII mode for Lane
 * E and F.For them, "uccMedia" must be UCC_MEDIA_SGMII. And the mode of
 * corresponding external phy (MV88E1111)must be MV88E_MODE_SGMII_NOCLK. And the
 * corresponding interruption must be connected to it.
 *
 * 5. The TBI block of UCC6 and UCC8 is used to connect to the external phy, and
 * the TBI block can be only controlled by the SMI of the UCC which it belongs
 * to, even if that UCC does not have an external MDIO connection. In other
 * words, the TBI phy of UCC6 must be accessed by the SMI of UCC6, and the TBI
 * phy of UCC8 must be accessed by the SMI of UCC8. So the TBI phy address for
 * UCC6/UCC8 can be configured to any valid phy address because the TBI phy is
 * the only phy attached to the SMI of UCC6/UCC8. The TBI phy address can be
 * overrided by the following resource item:
 *
 * { "tbiAddr",               HCF_RES_INT,    { (void *) 18 } }
 */

const struct hcfResource qefcc0Resources[] = {
    { "regBase",               HCF_RES_INT,    { (void *) UCC1_BA } },
    { "uccNum",                HCF_RES_INT,    { (void *) 1 } },
    { "phyAddr",               HCF_RES_INT,    { (void *) 7 } },
    { "uccMedia",              HCF_RES_INT,    { (void *) UCC_MEDIA_RGMII } },
    { "miiIfName",             HCF_RES_STRING, { (void *) "qefcc" } },
    { "miiIfUnit",             HCF_RES_INT,    { (void *) 0 } }
 };
#define qefcc0Num   NELEMENTS(qefcc0Resources)

/*
 * UCC2 works with UART mode when DRV_SIO_UCC is defined, so replace it with
 * UCC6 for qefcc1.
 */

#ifdef  DRV_SIO_UCC
const struct hcfResource qefcc1Resources[] = {
    { "regBase",               HCF_RES_INT,    { (void *) UCC6_BA } },
    { "uccNum",                HCF_RES_INT,    { (void *) 6 } },
    { "uccMedia",              HCF_RES_INT,    { (void *) UCC_MEDIA_SGMII } },
    { "phyAddr",               HCF_RES_INT,    { (void *) 4 } },
    { "miiIfName",             HCF_RES_STRING, { (void *) "qefcc" } },
    { "miiIfUnit",             HCF_RES_INT,    { (void *) 0 } }
 };
#else
const struct hcfResource qefcc1Resources[] = {
    { "regBase",               HCF_RES_INT,    { (void *) UCC2_BA } },
    { "uccNum",                HCF_RES_INT,    { (void *) 2 } },
    { "phyAddr",               HCF_RES_INT,    { (void *) 1 } },
    { "uccMedia",              HCF_RES_INT,    { (void *) UCC_MEDIA_RGMII } },
    { "miiIfName",             HCF_RES_STRING, { (void *) "qefcc" } },
    { "miiIfUnit",             HCF_RES_INT,    { (void *) 0 } }
 };
#endif /* DRV_SIO_UCC */

#define qefcc1Num   NELEMENTS(qefcc1Resources)

const struct hcfResource qefcc2Resources[] = {
    { "regBase",               HCF_RES_INT,    { (void *) UCC3_BA } },
    { "uccNum",                HCF_RES_INT,    { (void *) 3 } },
    { "uccMedia",              HCF_RES_INT,    { (void *) UCC_MEDIA_RGMII } },
    { "phyAddr",               HCF_RES_INT,    { (void *) 2 } },
    { "miiIfName",             HCF_RES_STRING, { (void *) "qefcc" } },
    { "miiIfUnit",             HCF_RES_INT,    { (void *) 0 } }
 };
#define qefcc2Num   NELEMENTS(qefcc2Resources)

const struct hcfResource qefcc3Resources[] = {
    { "regBase",               HCF_RES_INT,    { (void *) UCC4_BA } },
    { "uccNum",                HCF_RES_INT,    { (void *) 4 } },
    { "uccMedia",              HCF_RES_INT,    { (void *) UCC_MEDIA_RGMII } },
    { "phyAddr",               HCF_RES_INT,    { (void *) 3 } },
    { "miiIfName",             HCF_RES_STRING, { (void *) "qefcc" } },
    { "miiIfUnit",             HCF_RES_INT,    { (void *) 0 } }
 };
#define qefcc3Num   NELEMENTS(qefcc3Resources)

#ifdef DRV_RESOURCE_M85XXCCSR

const struct hcfResource m85xxLawBarResources[] = {
    { "regBase",               HCF_RES_INT,    { (void *)CCSBAR } },
    { "lawBarNum",             HCF_RES_INT,    { (void *)M85XX_LAW_NUM } },
    { "LAWBAR0",               HCF_RES_STRING, { "reserved" } },
    { "LAWBAR1",               HCF_RES_STRING, { "reserved" } },
    { "LAWBAR2",               HCF_RES_STRING, { "reserved" } },
    { "LAWBAR3",               HCF_RES_STRING, { "m85xxPci" } },
    { "LAWBAR4",               HCF_RES_STRING, { "reserved" } },
    { "LAWBAR5",               HCF_RES_STRING, { "reserved" } },
    { "LAWBAR6",               HCF_RES_STRING, { "reserved" } },
    { "LAWBAR7",               HCF_RES_STRING, { "reserved" } },
    { "LAWBAR8",               HCF_RES_STRING, { "reserved" } },
    { "LAWBAR9",               HCF_RES_STRING, { "reserved" } },
    { "LAWAR_ENABLE",          HCF_RES_INT,    { (void *)LAWAR_ENABLE } },
    { "LAWBAR_ADRS_SHIFT",     HCF_RES_INT,    { (void *)LAWBAR_ADRS_SHIFT } },
 };
#define m85xxLawBarNum  NELEMENTS(m85xxLawBarResources)

#endif /* DRV_RESOURCE_M85XXCCSR */

#ifdef DRV_PCIBUS_M85XX

const struct hcfResource m85xxPci0Resources[] = {
    { "regBase",               HCF_RES_INT,    { (void *) (CCSBAR + PCIEX_BASE_ADRS) } },
    { "mem32Addr",             HCF_RES_ADDR,   { (void *) PCIEX_MEM_ADRS } },
    { "mem32Size",             HCF_RES_INT,    { (void *) PCIEX_MEM_SIZE } },
    { "memIo32Addr",           HCF_RES_ADDR,   { (void *) PCIEX_MEMIO_ADRS } },
    { "memIo32Size",           HCF_RES_INT,    { (void *) PCIEX_MEMIO_SIZE } },
    { "io32Addr",              HCF_RES_ADDR,   { (void *) PCIEX_IO_ADRS } },
    { "io32Size",              HCF_RES_INT,    { (void *) PCIEX_IO_SIZE } },
    { "io16Addr",              HCF_RES_ADDR,   { (void *) PCIEX_IO_ADRS } },
    { "io16Size",              HCF_RES_INT,    { (void *) PCIEX_IO_SIZE } },
    { "fbbEnable",             HCF_RES_INT,    { (void *) TRUE } },
    { "cacheSize",             HCF_RES_INT,    { (void *) (_CACHE_ALIGN_SIZE / 4) } },
    { "maxLatAllSet",          HCF_RES_INT,    { (void *) PCI_LAT_TIMER } },
    { "autoIntRouteSet",       HCF_RES_INT,    { (void *) TRUE } },
    { "includeFuncSet",        HCF_RES_ADDR,   { (void *) sysPciExAutoconfigInclude } },
    { "intAssignFuncSet",      HCF_RES_ADDR,   { (void *) sysPciExAutoconfigIntrAssign } },

    /* non-vxbPciAutoConfig() values */

    { "pimmrBase",             HCF_RES_ADDR,   { (void *) PCI_BRIDGE_PIMMR_BASE_ADRS } },
    { "lclMemAddr",            HCF_RES_ADDR,   { (void *) LOCAL_MEM_LOCAL_ADRS } },
    { "lclMemMapSize",         HCF_RES_INT,    { (void *) LOCAL_MEM_SIZE } },
    { "mstrMemBus",            HCF_RES_ADDR,   { (void *) PCI_MSTR_MEM_BUS } },
    { "maxBusSet",             HCF_RES_INT,    { (void *) NUM_PCIEX_SUB_BUS } },

    /* LAWBAR details */

    { "tgtIf",                 HCF_RES_INT,    { (void *) LAWAR_TGTIF_PCIEX } },

    /* Window Attributes */

    { "owAttrMem",             HCF_RES_INT,    { (void *) MPC85XX_PCIE_MEMORY_OWATTR } },
    { "owAttrMemIo",           HCF_RES_INT,    { (void *) MPC85XX_PCIE_MEMORYIO_OWATTR } },
    { "owAttrIo",              HCF_RES_INT,    { (void *) MPC85XX_PCIE_IO_OWATTR } },
    { "iwAttr",                HCF_RES_INT,    { (void *) MPC85XX_PCIE_IWATTR } },
    { "singleLawBar",          HCF_RES_INT,    { (void *) TRUE } },
    { "singleLawBarSize",      HCF_RES_INT,    { (void *) PCIEX_LAW_SIZE } },

    { "pciConfigMechanism",    HCF_RES_INT,    { (void *) 0 } },
    { "autoConfig",            HCF_RES_INT,    { (void *) TRUE } },
    { "pciExpressHost",        HCF_RES_INT,    { (void *) TRUE } },

#ifdef INCLUDE_INTCTLR_DYNAMIC_LIB
    { "msiEnable",             HCF_RES_INT,    { (void *) (TRUE) } },
    { "dynamicInterrupts",     HCF_RES_INT,    { (void *) (TRUE) } },
    { "msiDataWorkaround",     HCF_RES_INT,    { (void *) (TRUE) } },
    { "msiDataWorkaroundAddr", HCF_RES_INT,    { (void *) (0x80000000) } }
#else
    { "msiEnable",             HCF_RES_INT,    { (void *) (FALSE) } },
    { "dynamicInterrupts",     HCF_RES_INT,    { (void *) (FALSE) } },
    { "msiDataWorkaround",     HCF_RES_INT,    { (void *) (FALSE) } }
#endif
 };
#define m85xxPci0Num    NELEMENTS(m85xxPci0Resources)

#endif /* DRV_PCIBUS_M85XX */

#ifdef DRV_STORAGE_SDHC
IMPORT UINT32 sysSdhcClkFreqGet (void);

struct hcfResource sdhcResources[] =  {
    { "regBase",      HCF_RES_INT,    { (void *)SDHC_BASE } },
    { "clkFreq",      HCF_RES_ADDR,   { (void *)sysSdhcClkFreqGet } },
    { "dmaMode",      HCF_RES_INT,    { (void *)0 } },
    { "polling",      HCF_RES_INT,    { (void *)0 } },
    { "flags"  ,      HCF_RES_INT,    { (void *)(SDHC_PIO_NEED_DELAY | SDHC_HW_SNOOP |
                                                 SDHC_FIFO_ENDIANESS_REVERSE |
                                                 SDHC_HOST_VER_REVERSE |
                                                 SDHC_HOST_CTRL_FREESCALE) } },
};
#define sdhcNum  NELEMENTS(sdhcResources)
#endif /* DRV_STORAGE_SDHC */

const struct hcfDevice hcfDeviceList[] = {
    { "epic",                  0, VXB_BUSID_PLB, 0, epic0Num,               epic0Resources },
    { "ppcIntCtlr",            0, VXB_BUSID_PLB, 0, ppcIntCtlr0Num,         ppcIntCtlr0Resources },

    { "m85xxTimerDev",         0, VXB_BUSID_PLB, 0, m85xxTimerNum,          m85xxTimerResources },

    /* DUART is mutually exclusive with QE UART */

#ifdef DRV_SIO_NS16550
    { "ns16550",               0, VXB_BUSID_PLB, 0, ns165500Num,            ns165500Resources },
    { "ns16550",               1, VXB_BUSID_PLB, 0, ns165501Num,            ns165501Resources },
#endif  /* DRV_SIO_NS16550 */

    { "quiccEngineIntCtlr",    0, VXB_BUSID_PLB, 0, quiccEngineIntCtlr0Num, quiccEngineIntCtlr0Resources },
    { "quiccEngine",           0, VXB_BUSID_PLB, 0, quiccEngine0Num,        quiccEngineResources },

    /* QE UART must be initialized after quiccEngine */

#ifdef DRV_SIO_UCC
    { "qeUart",                 0, VXB_BUSID_PLB, 0, uccSio0Num,            uccSioDev0Resources },
#endif /* DRV_SIO_UCC */

#ifdef DRV_RESOURCE_M85XXCCSR
    { "m85xxCCSR",             0, VXB_BUSID_PLB, 0, m85xxLawBarNum,         m85xxLawBarResources },
#endif /* DRV_RESOURCE_M85XXCCSR */

#ifdef DRV_PCIBUS_M85XX
    { "m85xxPci",              0, VXB_BUSID_PLB, 0, m85xxPci0Num,           m85xxPci0Resources },
#endif /* DRV_PCIBUS_M85XX */

    { "qefcc",                 0, VXB_BUSID_PLB, 0, qefcc0Num,              qefcc0Resources },
    { "qefcc",                 1, VXB_BUSID_PLB, 0, qefcc1Num,              qefcc1Resources },
    { "qefcc",                 2, VXB_BUSID_PLB, 0, qefcc2Num,              qefcc2Resources },
    { "qefcc",                 3, VXB_BUSID_PLB, 0, qefcc3Num,              qefcc3Resources },
#ifdef DRV_STORAGE_SDHC
    { "sdhci",                 0, VXB_BUSID_PLB, 0, sdhcNum,                sdhcResources },
#endif /* DRV_STORAGE_SDHC */
 };
const int hcfDeviceNum = NELEMENTS(hcfDeviceList);

/*
 * If the Marvell 88E1111 PHY is working in SGMII mode, the corresponding 'mode'
 * parameter should be changed to MV88E_MODE_SGMII_NOCLK.
 */

VXB_INST_PARAM_OVERRIDE sysInstParamTable[] = {
    { "mv88E1x11Phy",          0, "mode", VXB_PARAM_INT32, { (void *) MV88E_MODE_RGMII } },

    /*
     * UCC2 works with UART mode when DRV_SIO_UCC is defined, so replace it with
     * UCC6 and chaneg the mode to MV88E_MODE_SGMII_NOCLK.
     */

#ifdef DRV_SIO_UCC
    { "mv88E1x11Phy",          1, "mode", VXB_PARAM_INT32, { (void *) MV88E_MODE_SGMII_NOCLK } },
#else
    { "mv88E1x11Phy",          1, "mode", VXB_PARAM_INT32, { (void *) MV88E_MODE_RGMII } },
#endif /* DRV_SIO_UCC */
    { "mv88E1x11Phy",          2, "mode", VXB_PARAM_INT32, { (void *) MV88E_MODE_RGMII } },
    { "mv88E1x11Phy",          3, "mode", VXB_PARAM_INT32, { (void *) MV88E_MODE_RGMII } },
    { NULL, 0, NULL, VXB_PARAM_END_OF_LIST, { (void *)0 } }
 };
