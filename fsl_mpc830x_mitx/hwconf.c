/* hwconf.c - Hardware configuration support module */

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
01b,20jul11,e_d  add new parameter for qe driver.(WIND00288732)
01a,30jun11,e_d  derived from version 01a of fsl_mpc8308_rdb/hwconf.c
*/

#include <vxWorks.h>
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/hwConf.h>
#include <hwif/util/vxbParamSys.h>
#include "config.h"
#include <hwif/vxbus/vxbIntrCtlr.h>
#include <drv/pci/pciAutoConfigLib.h>

#ifdef DRV_INTCTLR_QE
#   include <../src/hwif/h/end/vxbUccEnd.h>
#   include <hwif/intCtlr/vxbQeIntCtlr.h>
#   include <../src/hwif/h/resource/qeIOPort.h>
#   include <../src/hwif/h/resource/qeDpramLib.h>
#endif /* DRV_INTCTLR_QE */

#if (defined(INCLUDE_EHCI) || defined(INCLUDE_FSL_TCD))
IMPORT void     sysEhciInit (void);
IMPORT void     usbOpAfterReset ();
IMPORT UINT32   vxbSwap32 (UINT32);
#endif  /* INCLUDE_EHCI || INCLUDE_FSL_TCD */

#ifdef DRV_STORAGE_SDHC
#   include <h/storage/vxbSdMmcLib.h>
#   include <h/storage/vxbSdhcStorage.h>
IMPORT UINT32   sysSdhcClkFreqGet (void);
#endif  /* DRV_STORAGE_SDHC */

#ifdef DRV_PCIBUS_M83XX
IMPORT STATUS   sysPciAutoconfigInclude ();
IMPORT UCHAR    sysPciAutoconfigIntrAssign (PCI_SYSTEM *,PCI_LOC *,UCHAR);
#endif /* DRV_PCIBUS_M83XX_EX */

#ifdef DRV_INTCTLR_QE
IMPORT UINT8 _binary_iram_mpc8306_r1_bin_start[];
#   define  QE_MICROCODE_ADDR    _binary_iram_mpc8306_r1_bin_start
#endif /* DRV_INTCTLR_QE */

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
    {0x00000000, 0x00000000, 0x00000000},         /* UART2 */
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
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* QE High  */
    {0x00000000, 0x00000000, 0x00000000},         /* QE Low   */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0xFC000020, 0x30000000, 0x00000000},         /* USB DR   */
    {0x00000020, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* IRQ0 ** **/
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
    {0xFF000020, 0x30000000, 0x00000000},         /* RTC SEC  */
    {0xFF000020, 0x30000000, 0x00000000},         /* PIT */
#ifdef MPC8309_BOARD
    {0xFF000020, 0x30000000, 0x00000000},         /* PCI1 */
#else /* MPC8309_BOARD */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
#endif /* MPC8309_BOARD */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0xFF000020, 0x30000000, 0x00000000},         /* RTC ALR */
    {0xFF000020, 0x30000000, 0x00000000},         /* MU */
    {0xFF000020, 0x30000000, 0x00000000},         /* SBA */
    {0xFF000020, 0x30000000, 0x00000000},         /* DMA */
    {0xFF000020, 0x30000000, 0x00000000},         /* GTM4 */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0xFF000020, 0x30000000, 0x00000000},         /* QE Port  */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
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
    0x00000000,         /* PEX CNT */
    0x00000000,         /* Reserved */
    0x00002000,         /* DMAC */
    0x00000000,         /* MSIR1 */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000080,         /* UART1 */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* ETSEC1 1588 TIMER */
    0x00000000,         /* ETSEC2 1588 TIMER */
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
    0x80000000,         /* QE High  */
    0x40000000,         /* QE Low   */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
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
#ifdef MPC8309_BOARD
    0x00000000,         /* PIT */
    0X00000000,         /* PCI */
#else /* MPC8309_BOARD */
    0x40000000,         /* PIT */
    0x00000000,         /* Reserved */
#endif /* MPC8309_BOARD */
    0x00000000,         /* MSIR0 */
    0x08000000,         /* RTC ALR */
    0x00000000,         /* Reserved */
    0x02000000,         /* SBA */
    0x00000000,         /* Reserved */
    0x00800000,         /* GTM4 */
    0x00000000,         /* Reserved */
    0x00200000,         /* QE Port */
    0x00000000,         /* Reserved */
    0x00080000,         /* DDR */
    0x00040000,         /* LBC */
    0x00020000,         /* GTM2 */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* MSIR2 */
    0x00000000,         /* MSIR3 */
    0x00000000,         /* Reserved */
    0x00000800,         /* GTM3 */
    0x00000000,         /* Reserved */
    0x00000000,         /* MSIR4 */
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
    0x00008000,          /* interrupt number 1: SPI2 */
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
    0x00000000,          /* interrupt number 41: */
    0x00200000,          /* interrupt number 42: UCC7 */
    0x00000000,          /* interrupt number 43: */
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

const QE_IOPORT_TABLE qeIoTable[] = {
    { 0, 0, 0, 0, 0 },
};

#define QE_EV_THREAD_NUM_830X 14
#define QE_IV_THREAD_NUM_830X 12

LOCAL QE_THREAD_DESC qeThreads[QE_EV_THREAD_NUM_830X + QE_IV_THREAD_NUM_830X] = {

    /* External Virtual Threads for 830x or Virtual Threads for others */

    { 0x88, TRUE },
    { 0x89, TRUE },
    { 0x98, TRUE },
    { 0x99, TRUE },
    { 0xa8, TRUE },
    { 0xa9, TRUE },
    { 0xb8, TRUE },
    { 0xb9, TRUE },
    { 0xc8, TRUE },
    { 0xc9, TRUE },
    { 0xd8, TRUE },
    { 0xd9, TRUE },
    { 0xe8, TRUE },
    { 0xe9, TRUE },

    /* Internal Virtual Threads without interrupt capabilities for 830x */

    { 0x54, TRUE },
    { 0x55, TRUE },
    { 0x5c, TRUE },
    { 0x5d, TRUE },
    { 0x64, TRUE },
    { 0x65, TRUE },
    { 0x6c, TRUE },
    { 0x6d, TRUE },
    { 0x74, TRUE },
    { 0x75, TRUE },
    { 0x7c, TRUE },
    { 0x7d, TRUE }
};

struct intrCtlrInputs quiccEngineIntCtlrInputs[] = {
    { QE_IC_INTR_UCC_1,      "qefcc",         0, 0 },
    { QE_IC_INTR_UCC_2,      "qefcc",         1, 0 },
    { QE_IC_INTR_UCC_3,      "qefcc",         2, 0 },
};
const struct hcfResource quiccEngineResources[] = {
    { "regBase",        HCF_RES_INT,  { (void *)(QUICC_ENGINE_BASE) } },
    { "muRamBase",      HCF_RES_INT,  { (void *)(QE_MURAM_POOL_BA)}},
    { "muRamPoolSize",  HCF_RES_INT,  { (void *) 0x4000 } },
    { "firmwareAdrs",   HCF_RES_INT,  { (void *) QE_MICROCODE_ADDR } },
    { "qeThreadTbl",    HCF_RES_ADDR, { (void *) &qeThreads[0] } },
    { "qeEvThreadNum",  HCF_RES_INT,  { (void *) QE_EV_THREAD_NUM_830X } },
    { "qeIvThreadNum",  HCF_RES_INT,  { (void *) QE_IV_THREAD_NUM_830X } },
    { "qeIoTable",      HCF_RES_ADDR, { (void *)&qeIoTable[0] } },
    { "qeIoTableSize",  HCF_RES_INT,  { (void *) 0 } }
};
#define quiccEngine0Num NELEMENTS(quiccEngineResources)

const struct hcfResource quiccEngineIntCtlr0Resources[] = {
    { "regBase",           HCF_RES_INT,  { (void *) (QUICC_ENGINE_BASE) } },
    { "input",             HCF_RES_ADDR, { (void *) &quiccEngineIntCtlrInputs[0] } },
    { "inputTableSize",    HCF_RES_INT,  { (void *) NELEMENTS(quiccEngineIntCtlrInputs) } },
    { "intMaskTable",      HCF_RES_ADDR, { (void *) &qeEnableHighPriInts[0] } },
    { "intMaskTableSize",  HCF_RES_INT,  { (void *) QE_IVEC_MAX } },
    { "inumMaskTable",     HCF_RES_ADDR, { (void *) &qeINumToMaskPattern[0] } },
    { "inumMaskTableSize", HCF_RES_INT,  { (void *) QE_IVEC_MAX } }
 };
#define quiccEngineIntCtlr0Num NELEMENTS(quiccEngineIntCtlr0Resources)

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
    { INUM_UART1,       "ns16550",            0,  0 },
    { INUM_UART1,       "ns16550",            1,  0 },
#ifdef DRV_TIMER_QUICC_PPC
    { INUM_GTM2,        "quiccTimerDev",      0,  0 },
    { INUM_GTM4,        "quiccTimerDev",      1,  0 },
#endif  /* DRV_TIMER_QUICC_PPC */

#ifdef INCLUDE_EHCI
    { INUM_USB_DR,      "vxbPlbUsbEhci",      0,  0 },
#endif  /* INCLUDE_EHCI */

#ifdef DRV_STORAGE_SDHC
    { INUM_SDHC,        "sdhci",              0,  0 },
#endif  /* DRV_STORAGE_SDHC */

#ifdef DRV_DMA_M512X
    { INUM_DMAC,        "m512xDma",           0,  0 },
#endif  /* DRV_DMA_M512X */

#ifdef DRV_INTCTLR_QE
    { INUM_QE_HIGH,     "quiccEngineIntCtlr", 0,  0 },
    { INUM_QE_LOW,      "quiccEngineIntCtlr", 0,  1 }
#endif /* DRV_INTCTLR_QE */
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

const struct hcfResource dummyMdio0Resources[] =
    {
    { "regBase",         HCF_RES_INT,   { (void *)0 } },
    { "fullDuplex",      HCF_RES_INT,   { (void *)1 } },
    { "speed",           HCF_RES_INT,   { (void *)100 } }
    };
#define dummyMdio0Num       NELEMENTS(dummyMdio0Resources)

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
#endif  /* DRV_TIMER_QUICC_PPC */

#ifdef DRV_PCIBUS_M83XX
const struct hcfResource m83xxPciResources[] = {
    { "regBase",          HCF_RES_INT,  { (void *)PCI_BASE } },
    { "mem32Addr",        HCF_RES_ADDR, { (void *)PCI_MEM_ADRS } },
    { "mem32Size",        HCF_RES_INT,  { (void *)PCI_MEM_SIZE } },
    { "memIo32Addr",      HCF_RES_ADDR, { (void *)PCI_MEMIO_ADRS } },
    { "memIo32Size",      HCF_RES_INT,  { (void *)PCI_MEMIO_SIZE } },
    { "io32Addr",         HCF_RES_ADDR, { (void *)PCI_IO_ADRS } },
    { "io32Size",         HCF_RES_INT,  { (void *)PCI_IO_SIZE } },
    { "io16Addr",         HCF_RES_ADDR, { (void *)PCI_IO_ADRS } },
    { "io16Size",         HCF_RES_INT,  { (void *)PCI_IO_SIZE } },
    { "fbbEnable",        HCF_RES_INT,  { (void *)TRUE } },
    { "cacheSize",        HCF_RES_INT,  { (void *)(_CACHE_ALIGN_SIZE / 4) } },
    { "maxLatAllSet",     HCF_RES_INT,  { (void *)PCI_LAT_TIMER } },
    { "autoIntRouteSet",  HCF_RES_INT,  { (void *)TRUE } },
    { "includeFuncSet",   HCF_RES_ADDR, { (void *)sysPciAutoconfigInclude } },
    { "intAssignFuncSet", HCF_RES_ADDR, { (void *)sysPciAutoconfigIntrAssign } },
    { "maxBusSet",        HCF_RES_INT,  { (void *)0 } },
    { "pciConfigMechanism",HCF_RES_INT, { (void *)(0) } },

    /* non vxbPciAutoConfig() values */

    { "pimmrBase",        HCF_RES_ADDR, { (void *)PCI_PIMMR_BASE_ADRS } },
    { "lclMemAddr",       HCF_RES_ADDR, { (void *)LOCAL_MEM_LOCAL_ADRS } },
    { "lclMemMapSize",    HCF_RES_INT,  { (void *)(256 * 1024 * 1024) } },
    { "mstrMemBus",       HCF_RES_ADDR, { (void *)PCI_MSTR_MEM_BUS } },
    { "autoConfig",       HCF_RES_INT,  { (void *)TRUE } },
    { "lawbar",           HCF_RES_INT,  { (void *)(M83XX_PCILAWBARn(CCSBAR,0))}},
    { "lawar",            HCF_RES_INT,  { (void *)(M83XX_PCILAWARn(CCSBAR,0))}},
    { "lawarAttr",        HCF_RES_INT,  { (void *)(PCI_LAWAR_ENABLE |
                                                   PCI_LAWAR_SIZE_256MB)}},
};
#   define m83xxPciNum NELEMENTS(m83xxPciResources)
#endif /* DRV_PCIBUS_M83XX_EX */

#ifdef  INCLUDE_UCC_MDIO
const struct hcfResource uccMdioResources[] = {
    { "regBase",       HCF_RES_INT,   { (void *)(QUICC_ENGINE_BASE + UCC1_BASE) } },
};
#define uccMdioNum NELEMENTS(uccMdioResources)
#endif /* INCLUDE_UCC_MDIO */

/*
 * On the Freescale MPC8306/9 board, We have three UEC net port
 * PHYs are physically wired according to the following table:
 *
 * Data port pins       MII address     mode
 * --------------       -----------     -----------
 * PHY0: UCC1           0x11               MII
 * PHY1: UCC2           dummy phy          MII
 * PHY2: UCC3           0x12               MII
 *
 * 1. All the 3 PHYs have their management pins connected to UCC1.
 */

#ifdef  INCLUDE_UCC_VXB_END
const struct hcfResource qefcc0Resources[] = {
    { "regBase",   HCF_RES_INT,    { (void *) (QUICC_ENGINE_BASE + UCC1_BASE) } },
    { "uccNum",    HCF_RES_INT,    { (void *) 1 } },
    { "phyAddr",   HCF_RES_INT,    { (void *) 0x11 } },
    { "uccMedia",  HCF_RES_INT,    { (void *) UCC_MEDIA_MII_GMII } },
    { "miiIfName", HCF_RES_STRING, { (void *) "uccMdio" } },
    { "miiIfUnit", HCF_RES_INT,    { (void *) 0 } }
 };
#define qefcc0Num   NELEMENTS(qefcc0Resources)

const struct hcfResource qefcc1Resources[] = {
    { "regBase",   HCF_RES_INT,    { (void *) (QUICC_ENGINE_BASE + UCC2_BASE) } },
    { "uccNum",    HCF_RES_INT,    { (void *) 2 } },
    { "phyAddr",   HCF_RES_INT,    { (void *) 0x13 } },
    { "uccMedia",  HCF_RES_INT,    { (void *) UCC_MEDIA_MII_GMII } },
    { "miiIfName", HCF_RES_STRING, { (void *) "dummyMdio" } },
    { "miiIfUnit", HCF_RES_INT,    { (void *) 0 } }
 };
#define qefcc1Num   NELEMENTS(qefcc1Resources)

const struct hcfResource qefcc2Resources[] = {
    { "regBase",   HCF_RES_INT,    { (void *) (QUICC_ENGINE_BASE + UCC3_BASE) } },
    { "uccNum",    HCF_RES_INT,    { (void *) 3 } },
    { "phyAddr",   HCF_RES_INT,    { (void *) 0x12 } },
    { "uccMedia",  HCF_RES_INT,    { (void *) UCC_MEDIA_MII_GMII } },
    { "miiIfName", HCF_RES_STRING, { (void *) "uccMdio" } },
    { "miiIfUnit", HCF_RES_INT,    { (void *) 0 } }
 };
#define qefcc2Num   NELEMENTS(qefcc2Resources)
#endif /* INCLUDE_UCC_VXB_END */

#ifdef INCLUDE_EHCI
const struct hcfResource vxbPlbUsbEhciDevResources[]=
    {
    {"regBase",     HCF_RES_INT,   { (void *)M83XX_CAPLENGTH } },
    {"ehciInit",    HCF_RES_ADDR,  { (void *)sysEhciInit } },
    {"dataSwap",    HCF_RES_ADDR,  { (void *)vxbSwap32}},
    };
#define vxbPlbUsbEhciDevNum NELEMENTS(vxbPlbUsbEhciDevResources)
#endif  /* INCLUDE_EHCI */

#ifdef DRV_STORAGE_SDHC
struct hcfResource fslSdhcResources[] =  {
    { "regBase",    HCF_RES_INT,   { (void *)SDHC_BASE } },
    { "clkFreq",    HCF_RES_ADDR,  { (void *)sysSdhcClkFreqGet } },
    { "dmaMode",    HCF_RES_INT,   { (void *)0 } },
    { "polling",    HCF_RES_INT,   { (void *)0 } },
    { "flags" ,     HCF_RES_INT,   { (void *)(SDHC_PIO_NEED_DELAY |
                                              SDHC_HW_SNOOP |
                                              SDHC_FIFO_ENDIANESS_REVERSE |
                                              SDHC_HOST_VER_REVERSE |
                                              SDHC_HOST_CTRL_FREESCALE) } },
};
#define fslSdhcNum  NELEMENTS(fslSdhcResources)
#endif  /* DRV_STORAGE_SDHC */

#ifdef DRV_DMA_M512X
const struct hcfResource m512xDmaResources[] = {
    { "regBase",    HCF_RES_INT,    { (void *)DMAC_BASE } },
    };
#define m512xDmaNum NELEMENTS(m512xDmaResources)
#endif  /* DRV_DMA_M512X */

#ifdef DRV_RESOURCE_FSLGPIO
const struct hcfResource gpio0Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *)(GPIO1_BASE) } },
};
#define gpio0Num NELEMENTS(gpio0Resources)
const struct hcfResource gpio1Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *)(GPIO2_BASE) } },
};
#define gpio1Num NELEMENTS(gpio1Resources)
#endif /* DRV_RESOURCE_FSLGPIO */

const struct hcfDevice hcfDeviceList[] =
    {
    { "ppcIntCtlr",         0, VXB_BUSID_PLB, 0, ppcIntCtlr0Num,          ppcIntCtlr0Resources },
    { "quiccIntCtlr",       0, VXB_BUSID_PLB, 0, quiccIntCtlr0Num,        quiccIntCtlr0Resources },

#if defined(INCLUDE_QUICC_ENGINE_UTILS) || defined(DRV_INTCTLR_QE)
    { "quiccEngineIntCtlr", 0, VXB_BUSID_PLB, 0, quiccEngineIntCtlr0Num,  quiccEngineIntCtlr0Resources },
    { "quiccEngine",        0, VXB_BUSID_PLB, 0, quiccEngine0Num,         quiccEngineResources },
#endif /* INCLUDE_QUICC_ENGINE_UTILS || DRV_INTCTLR_QE */

    { "ppcDecTimerDev",     0, VXB_BUSID_PLB, 0, ppcDecTimerDevNum,       ppcDecTimerDevResources },

#ifdef DRV_TIMER_QUICC_PPC
    { "quiccTimerDev",      0, VXB_BUSID_PLB, 0, generalPurposeTimerNum0, generalPurposeTimerResources0 },
    { "quiccTimerDev",      1, VXB_BUSID_PLB, 0, generalPurposeTimerNum1, generalPurposeTimerResources1 },
#endif  /* DRV_TIMER_QUICC_PPC */

      { "ns16550",          0, VXB_BUSID_PLB, 0, ns1655x0Num,             ns1655x0Resources },
      { "ns16550",          1, VXB_BUSID_PLB, 0, ns1655x1Num,             ns1655x1Resources },

#ifdef INCLUDE_UCC_MDIO
    { "uccMdio",            0, VXB_BUSID_PLB, 0, uccMdioNum,             uccMdioResources },
#endif /* INCLUDE_UCC_MDIO */

#ifdef INCLUDE_DUMMY_MDIO
    { "dummyMdio",          0, VXB_BUSID_PLB, 0, dummyMdio0Num,          dummyMdio0Resources },
#endif /* INCLUDE_DUMMY_MDIO */

#ifdef INCLUDE_UCC_VXB_END
     { "qefcc",             0, VXB_BUSID_PLB, 0, qefcc0Num,              qefcc0Resources },
     { "qefcc",             1, VXB_BUSID_PLB, 0, qefcc1Num,              qefcc1Resources },
     { "qefcc",             2, VXB_BUSID_PLB, 0, qefcc2Num,              qefcc2Resources },
#endif /* INCLUDE_UCC_VXB_END */

#ifdef DRV_PCIBUS_M83XX
    { "m83xxPci",           0, VXB_BUSID_PLB, 0, m83xxPciNum,            m83xxPciResources },
#endif  /* DRV_PCIBUS_M83XX_EX */

#ifdef INCLUDE_EHCI
    { "vxbPlbUsbEhci",      0, VXB_BUSID_PLB, 0, vxbPlbUsbEhciDevNum,    vxbPlbUsbEhciDevResources },
#endif  /* INCLUDE_USB */

#ifdef DRV_RESOURCE_FSLGPIO
    { "fslGpio",            0, VXB_BUSID_PLB, 0, gpio0Num,               gpio0Resources },
    { "fslGpio",            1, VXB_BUSID_PLB, 0, gpio1Num,               gpio1Resources },
#endif /* DRV_RESOURCE_FSLGPIO */

#ifdef DRV_STORAGE_SDHC
    { "sdhci",              0, VXB_BUSID_PLB, 0, fslSdhcNum,             fslSdhcResources },
#endif  /* DRV_STORAGE_SDHC */

#ifdef DRV_DMA_M512X
    { "m512xDma",           0, VXB_BUSID_PLB, 0, m512xDmaNum,            m512xDmaResources },
#endif  /* DRV_DMA_M512X */
    };

const int hcfDeviceNum = NELEMENTS(hcfDeviceList);

VXB_INST_PARAM_OVERRIDE sysInstParamTable[] =
    {
#ifdef INCLUDE_EHCI
    { "vxbPlbUsbEhci", 0, "hasEmbeddedTT",   VXB_PARAM_INT32,       {(void *)TRUE} },
    { "vxbPlbUsbEhci", 0, "fixupPortNumber", VXB_PARAM_INT32,       {(void *)FALSE} },
    { "vxbPlbUsbEhci", 0, "postResetHook",   VXB_PARAM_FUNCPTR,     {(void *)usbOpAfterReset} },
#endif  /* INCLUDE_EHCI */
    { NULL,            0, NULL,              VXB_PARAM_END_OF_LIST, {(void *)0} }
    };
