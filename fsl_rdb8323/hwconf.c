/* hwconf.c - Hardware configuration support module */

/*
 * Copyright (c) 2006-2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
01h,10dec10,y_y  fix miss pci macro error. (WIND00245940)
01q,04aug10,liu  clear mot83xxPci.h. (WIND00212294)
01f,23sep09,x_z  update quiccEngine configuration.
01e,15jul09,b_m  add QE IO port configuration table.
01d,26jun09,e_d  change timer and interrupt legacy driver to vxBus driver
01c,17mar08,wap  Switch from hEnd qeFcc driver to VxBus UCC driver
01b,26jul07,h_k  added ccsBar and qeIoPorts to quiccEngineResources[].
01a,20dec06,pmr  adapted from mds8360
*/

#include <vxWorks.h>
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/hwConf.h>
#include <hwif/util/vxbParamSys.h>
#include <hwif/vxbus/vxbIntrCtlr.h>
#include "config.h"
#include "sysDuart.h"
#include <../src/hwif/h/mii/icp175cPhy.h>
#include <busCtlr/m83xxPci.h>
#ifdef INCLUDE_QUICC_ENGINE_UTILS
#include <../src/hwif/h/resource/qeIOPort.h>
#endif

/* for now do not allow interruption of interrupt processing for Quicc Engine */

const QE_SIU_INT_MASK qeEnableHighPriInts[QE_IVEC_MAX] =
    {
    {0x00000000, 0x00000000},   /* interrupt 0 error - enable nothing */
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000},
    {0x00000000, 0x00000000}
    };

/* Quicc Engine specific may change for different CPUs */

const UINT32 qeINumToMaskPattern[QE_IVEC_MAX] =
    {
    0x00000000,         /* interrupt number 0: Error */
    0x00008000,         /* interrupt number 1: I2C */
    0x00004000,         /* interrupt number 2: SPI1 */
    0x00002000,         /* interrupt number 3: Risc Timer Table */
    0x00000000,         /* interrupt number 4: */
    0x00000000,         /* interrupt number 5: */
    0x00000000,         /* interrupt number 6:  */
    0x00000000,         /* interrupt number 7:  */
    0x00000000,         /* interrupt number 8:  */
    0x00000000,         /* interrupt number 9:  */
    0x00000040,         /* interrupt number 10: SDMA */
    0x00000020,         /* interrupt number 11: USB */
    0x00000010,         /* interrupt number 12: Timer 1 */
    0x00000008,         /* interrupt number 13: Timer 2 */
    0x00000004,         /* interrupt number 14: Timer 3 */
    0x00000002,         /* interrupt number 15: Timer 4 */
    0x00000000,         /* interrupt number 16: */
    0x00000000,         /* interrupt number 17: */
    0x00000000,         /* interrupt number 18: */
    0x00000000,         /* interrupt number 19: */
    0x10000000,         /* interrupt number 20: VT*/
    0x00000000,         /* interrupt number 21: */
    0x00000000,         /* interrupt number 22: */
    0x00000000,         /* interrupt number 23: */
    0x00000000,         /* interrupt number 24: */
    0x00800000,         /* interrupt number 25: EXT1 */
    0x00400000,         /* interrupt number 26: EXT2 */
    0x00200000,         /* interrupt number 27: EXT3 */
    0x00100000,         /* interrupt number 28: EXT4 */
    0x00000000,         /* interrupt number 29: */
    0x00000000,         /* interrupt number 30: */
    0x00000000,         /* interrupt number 31: */
    0x80000000,         /* interrupt number 32: UCC1 */
    0x40000000,         /* interrupt number 33: UCC2 */
    0x20000000,         /* interrupt number 34: UCC3 */
    0x10000000,         /* interrupt number 35: UCC4 */
    0x00000000,         /* interrupt number 36: reserved */
    0x00000000,         /* interrupt number 37: reserved */
    0x00000000,         /* interrupt number 38: reserved */
    0x00000000,         /* interrupt number 39: reserved */
    0x00800000,         /* interrupt number 40: UCC5 */
    0x00000000,         /* interrupt number 41: reserved */
    0x00000000,         /* interrupt number 42: reserved */
    0x00000000,         /* interrupt number 43: reserved */
    0x00000000,         /* interrupt number 44: reserved */
    0x00000000,         /* interrupt number 45: reserved */
    0x00000000,         /* interrupt number 46: reserved */
    0x00000000,         /* interrupt number 47: reserved */
    0x00000000,         /* interrupt number 48: reserved */
    0x00000000,         /* interrupt number 49: reserved */
    0x00000000,         /* interrupt number 50: reserved */
    0x00000000,         /* interrupt number 51: reserved */
    0x00000000,         /* interrupt number 52: reserved */
    0x00000000,         /* interrupt number 53: reserved */
    0x00000000,         /* interrupt number 54: reserved */
    0x00000000,         /* interrupt number 55: reserved */
    0x00000000,         /* interrupt number 56: reserved */
    0x00000000,         /* interrupt number 57: reserved */
    0x00000000,         /* interrupt number 58: reserved */
    0x00000000,         /* interrupt number 59: reserved */
    0x00000000,         /* interrupt number 60: reserved */
    0x00000000,         /* interrupt number 61: reserved */
    0x00000000,         /* interrupt number 62: reserved */
    0x00000000          /* interrupt number 63: reserved */
    };

/* This can be modified as per user manual for chip to prioritize devices */

const SIU_INT_MASK quiccIntMask[QUICC_IVEC_MAX] =
    {
    {0x00000000, 0x00000000, 0x00000000},   /* interrupt 0 error - enable nothing */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0xFF000020, 0x30000000, 0x00000000},   /* UART1 */
    {0xFF000020, 0x30000000, 0x00000000},   /* UART2 */
    {0xFF000000, 0x00000000, 0x00000000},   /* SEC   */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0xFF000020, 0x30000000, 0x00000000},   /* I2C */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0xFF000020, 0x30000000, 0x00000000},   /* IRQ1 */
    {0xFF000020, 0x30000000, 0x00000000},   /* IRQ2 */
    {0xFF000020, 0x30000000, 0x00000000},   /* IRQ3 */
    {0xFF000020, 0x30000000, 0x00000000},   /* IRQ4 */
    {0xFF000020, 0x30000000, 0x00000000},   /* IRQ5 */
    {0xFF000020, 0x30000000, 0x00000000},   /* IRQ6 */
    {0xFF000020, 0x30000000, 0x00000000},   /* IRQ7 **23**/
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* QE HIGH */
    {0x00000000, 0x00000000, 0x00000000},   /* QE LOW */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved*/
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* IRQ0 ***48**/
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0xFF000020, 0x30000000, 0x00000000},   /* RTC SEC */
    {0xFF000020, 0x30000000, 0x00000000},   /* PIT */
    {0xFF000020, 0x00000000, 0x00000000},   /* PCI1 */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0xFF000020, 0x30000000, 0x00000000},   /* RTC ALR */
    {0xFF000020, 0x30000000, 0x00000000},   /* MU */
    {0xFF000020, 0x30000000, 0x00000000},   /* SBA */
    {0xFF000020, 0x30000000, 0x00000000},   /* DMA */
    {0xFF000020, 0x30000000, 0x00000000},   /* GTM4 */
    {0xFF000020, 0x30000000, 0x00000000},   /* GTM8 73*/
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0xFF000020, 0x30000000, 0x00000000},   /* DDR */
    {0xFF000020, 0x30000000, 0x00000000},   /* LBC */
    {0xFF000020, 0x30000000, 0x00000000},   /* GTM2 */
    {0xFF000020, 0x30000000, 0x00000000},   /* GTM6 */
    {0xFF000020, 0x30000000, 0x00000000},   /* PMC */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0xFF000020, 0x30000000, 0x00000000},   /* GTM3 */
    {0xFF000020, 0x30000000, 0x00000000},   /* GTM7 */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0xFF000020, 0x30000000, 0x00000000},   /* GTM1 */
    {0xFF000020, 0x30000000, 0x00000000},   /* GTM5 */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},   /* Reserved */
    {0x00000000, 0x00000000, 0x00000000}    /* Reserved */
    };

/* Specific to CPU  */

const UINT32 quiccInumToMaskPattern[QUICC_IVEC_MAX] =
    {
    0x00000000,         /* interrupt number 0: Error */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000080,         /* UART1 */
    0x00000040,         /* UART2 */
    0x00000020,         /* SEC   */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000004,         /* I2C */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x40000000,         /* IRQ1 */
    0x20000000,         /* IRQ2 */
    0x10000000,         /* IRQ3 */
    0x08000000,         /* IRQ4 */
    0x04000000,         /* IRQ5 */
    0x02000000,         /* IRQ6 */
    0x01000000,         /* IRQ7 **23**/
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x80000000,         /* QE HIGH  */
    0x40000000,         /* QE LOW   */
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
    0x80000000,         /* IRQ0 ***48**/
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
    0x20000000,         /* PCI1 */
    0x00000000,         /* Reserved */
    0x08000000,         /* RTC ALR */
    0x04000000,         /* MU */
    0x02000000,         /* SBA */
    0x01000000,         /* DMA */
    0x00800000,         /* GTM4 */
    0x00400000,         /* GTM8 */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00080000,         /* DDR */
    0x00040000,         /* LBC */
    0x00020000,         /* GTM2 */
    0x00010000,         /* GTM6 */
    0x00008000,         /* PMC */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000800,         /* GTM3 */
    0x00000400,         /* GTM7 */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000020,         /* GTM1 */
    0x00000010,         /* GTM5 */
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

/* vxBus resource files for onboard UARTS */

IMPORT UINT32 sysUartDivisorGet (UINT32, UINT32);

#ifdef INCLUDE_PCI_BUS
IMPORT STATUS sysPciAutoconfigInclude ();
IMPORT UCHAR sysPciAutoconfigIntrAssign (PCI_SYSTEM *, PCI_LOC *, UCHAR);

const struct hcfResource m83xxPciResources[] = {
    { "regBase",          HCF_RES_INT,  { (void *)(CCSBAR + M83XXPCI0_REG_BASE)} },
    { "mem32Addr",        HCF_RES_ADDR, { (void *)PCI_MEM_ADRS } },
    { "mem32Size",        HCF_RES_INT,  { (void *)PCI_MEM_SIZE } },
    { "memIo32Addr",      HCF_RES_ADDR, { (void *)PCI_MEMIO_ADRS } },
    { "memIo32Size",      HCF_RES_INT,  { (void *)PCI_MEMIO_SIZE } },
    { "io32Addr",         HCF_RES_ADDR, { (void *)PCI_IO_ADRS } },
    { "io32Size",         HCF_RES_INT,  { (void *)PCI_IO_SIZE } },
    { "cacheSize",        HCF_RES_INT,  { (void *)(_CACHE_ALIGN_SIZE / 4) } },
    { "maxLatAllSet",     HCF_RES_INT,  { (void *)PCI_LAT_TIMER } },
    { "autoIntRouteSet",  HCF_RES_INT,  { (void *)TRUE } },
    { "includeFuncSet",   HCF_RES_ADDR, { (void *)sysPciAutoconfigInclude } },
    { "intAssignFuncSet", HCF_RES_ADDR, {(void *)sysPciAutoconfigIntrAssign }},

    /* non vxbPciAutoConfig() values */

    { "pimmrBase",         HCF_RES_ADDR, { (void *)PCI_BRIDGE_PIMMR_BASE_ADRS } },
    { "lclMemAddr",        HCF_RES_ADDR, { (void *)LOCAL_MEM_LOCAL_ADRS } },
    { "lclMemMapSize",     HCF_RES_INT,  { (void *)(512 * 1024 * 1024) } },
    { "mstrMemBus",        HCF_RES_ADDR, { (void *)PCI_MSTR_MEM_BUS } },
    { "maxBusSet",         HCF_RES_INT,  { (void *)3 } },
    { "owAttrMem",         HCF_RES_INT,  { (void *)(M83XXPCI_OB_WINDOW_ENABLE_BIT |
                                                    M83XXPCI_PREFETCHABLE         |
                                                    M83XXPCI_OB_ATTR_WS_128M) } },
    { "owAttrMemIo",       HCF_RES_INT,  { (void *)(M83XXPCI_OB_WINDOW_ENABLE_BIT |
                                                    M83XXPCI_OB_ATTR_WS_64M) } },
    { "owAttrIo",          HCF_RES_INT,  { (void *)(M83XXPCI_OB_WINDOW_ENABLE_BIT |
                                                    M83XXPCI_OB_ATTR_IO_BIT       |
                                                    M83XXPCI_OB_ATTR_WS_64M) } },
    { "iwAttr",            HCF_RES_INT,  { (void *)(M83XXPCI_IB_WINDOW_ENABLE_BIT |
                                                    M83XXPCI_PREFETCHABLE          |
                                                    M83XXPCI_IB_ATTR_RTT_READ_SNOOP  |
                                                    M83XXPCI_IB_ATTR_RTT_WRITE_SNOOP |
                                                    M83XXPCI_IB_ATTR_IWS_256M) } },
    { "lawbar",            HCF_RES_INT,  { (void *)(M83XXPCI_LAWBAR0)}},
    { "lawar",             HCF_RES_INT,  { (void *)(M83XXPCI_LAWAR0)}},
    { "lawarAttr",         HCF_RES_INT,  { (void *)(M83XXPCI_LAWAR_ENABLE |
                                                    M83XXPCI_LAWAR_SIZE_256MB)}},
    { "pciConfigMechanism",HCF_RES_INT,  { (void *)(0)}},
    { "autoConfig",        HCF_RES_INT,  { (void *)(TRUE)}}
};

#define m83xxPciNum NELEMENTS(m83xxPciResources)
#endif /* INCLUDE_PCI_BUS */

const struct hcfResource ns1655x1Resources[] =
    {
        { "regBase",     HCF_RES_INT,  {(void *)COM1_ADR} },
        { "regInterval", HCF_RES_INT,  {(void *)DUART_REG_ADDR_INTERVAL} },
        { "divsorCalc",  HCF_RES_ADDR, {(void *)&sysUartDivisorGet} },
        { "clkFreq",     HCF_RES_ADDR, {(void *)&sysClkFreqGet} }
    };

#define ns1655x1Num NELEMENTS(ns1655x1Resources)

const struct hcfResource ns1655x2Resources[] =
    {
        { "regBase",     HCF_RES_INT,  {(void *)COM2_ADR} },
        { "regInterval", HCF_RES_INT,  {(void *)DUART_REG_ADDR_INTERVAL} },
        { "divsorCalc",  HCF_RES_ADDR, {(void *)&sysUartDivisorGet} },
        { "clkFreq",     HCF_RES_ADDR, {(void *)&sysClkFreqGet} }
    };

#define ns1655x2Num NELEMENTS(ns1655x2Resources)

/*
 * The RDB8323 board comes with UCC2 and UCC3 wired to an ICplus
 * IP175C 5 port switch controller instead of standalone PHYs. The
 * IP175C has 5 PHYs for direct ethernet media connections and 2
 * MII ports for ethernet MAC connections. The device is configured
 * so that PHY ports 0-3 and MII port 0 are ganged together in a 5 port
 * switch configuration, with the MII0 port connected directly to
 * UCC2. PHY port 4 and MII port 1 are directly linked, with the MII1
 * port connected to UCC3. This makes it appear as though UCC3 is
 * a standalone interface with a single PHY, while UCC3 is connected
 * to one port of a 5 port switch, with the 4 remaining ports available
 * as jacks on the front panel. This provides a total of 5 external
 * cable ports on the board. The whole switch chip has a single MDIO
 * management port which allows access to the registers of all 5
 * media PHYs and the 2 direct MAC links, for a total of 7 virtual
 * PHY devices, numbered 0 through 6 on the MII management bus. The
 * switch controller itself can be programmed through three additional
 * management addresses (29, 30 and 31).
 *
 * The layout for the PHY connections is as follows:
 *
 * MII address		MAC			device
 * -----------		---			------
 * 0			UCC2			PHY port 0
 * 1			UCC2			PHY port 1
 * 2			UCC2			PHY port 2
 * 3			UCC2			PHY port 3
 * 4			UCC3			PHY port 4
 * 5			UCC2			MII port 0
 * 6			UCC3			MII port 1
 * 29			N/A			Switch control 0
 * 30			N/A			Switch control 1
 * 31			N/A			Switch control 2
 *
 * The miiBus configuration for the UCC3 port is simple: it has just one
 * physical media connection (PHY4), so we connect it to MII address 4. This
 * allows the UCC driver to sense link change events when someone unplugs
 * and replugs the cable on port 4.
 *
 * For UCC2, things are a little more complex: rather than having a
 * one-to-one PHY/MAC correlation, it's actually one-to-many: there is
 * one MAC with many physical switch ports. Given this, it doesn't really
 * make sense to connect any of the physical media PHYs to the UCC driver
 * for UCC2, because at best we will only sense link events when someone
 * unplugs or replugs a cable on one of the four external switch ports.
 *
 * Rather than worry about the physical media ports, we instead set
 * the PHY address of UCC2 to MII port 0 (MII address 5). This allows
 * the UCC to negotiate a 100Mbps full duplex link with the switch.
 *
 * Once side effect of this is that the link on UCC2 is always up, and
 * the speed is always 100Mbps full duplex, even if there are no cables
 * connected to any of the 4 switch ports.
 *
 * UCC4 is wired to an empty header, which is meant to host an option
 * DSL module.
 */

#ifdef INCLUDE_UCC_MDIO
LOCAL const struct hcfResource mdio0Resources[] = {
    { "regBase", HCF_RES_INT, { (void *)(CCSBAR + QUICC_ENGINE_BA + UCC3_BASE) } },
};

#define mdio0Num NELEMENTS(mdio0Resources)

#endif /* INCLUDE_UCC_MDIO */

#ifdef INCLUDE_ICP175CPHY

LOCAL const struct hcfResource icp0Resources[] = {
    { "regBase",   HCF_RES_INT,    { (void *)(0) } },
    { "miiIfName", HCF_RES_STRING, { (void *)"uccMdio" } },
    { "miiIfUnit", HCF_RES_INT,    { (void *)0 } },
};

#define icp0Num NELEMENTS(icp0Resources)

#endif /* INCLUDE_ICP175CPHY */

#ifdef QUICC_ENGINE
#ifdef INCLUDE_UCC_VXB_END
const struct hcfResource qefcc0Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *)(CCSBAR + QUICC_ENGINE_BA + UCC3_BASE) } },
    { "uccNum",     HCF_RES_INT,    { (void *)3 } },
    { "phyAddr",    HCF_RES_INT,    { (void *)ICP_PORT4_ADDR } },
    { "miiIfName",  HCF_RES_STRING, { (void *)"uccMdio" } },
    { "miiIfUnit",  HCF_RES_INT,    { (void *)0 } },
};

#define qefcc0Num NELEMENTS(qefcc0Resources)

const struct hcfResource qefcc1Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *)(CCSBAR + QUICC_ENGINE_BA + UCC2_BASE) } },
    { "uccNum",     HCF_RES_INT,    { (void *)2 } },
    { "phyAddr",    HCF_RES_INT,    { (void *)ICP_MII0_ADDR } },
    { "miiIfName",  HCF_RES_STRING, { (void *)"icpPhy" } },
    { "miiIfUnit",  HCF_RES_INT,    { (void *)0 } },
};

#define qefcc1Num NELEMENTS(qefcc1Resources)

const struct hcfResource qefcc2Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *)(CCSBAR + QUICC_ENGINE_BA + UCC4_BASE) } },
    { "uccNum",     HCF_RES_INT,    { (void *)4 } },
    { "phyAddr",    HCF_RES_INT,    { (void *)10 } }, /* dummy value */
    { "miiIfName",  HCF_RES_STRING, { (void *)"uccMdio" } },
    { "miiIfUnit",  HCF_RES_INT,    { (void *)0 } },
};

#define qefcc2Num NELEMENTS(qefcc2Resources)

#endif /* INCLUDE_UCC_VXB_END */

#ifdef INCLUDE_QUICC_ENGINE_UTILS

const QE_IOPORT_TABLE qeIoTable[] = {
    /* ENET2/UCC2 */

    { QE_PORTD, 0x15, 2, 0, 1 },    /* RX_CLK (CLK16) */
    { QE_PORTD, 0x17, 2, 0, 1 },    /* TX_CLK (CLK3) */
    { QE_PORTA, 0x12, 1, 0, 1 },    /* TXD0 */
    { QE_PORTA, 0x13, 1, 0, 1 },    /* TXD1 */
    { QE_PORTA, 0x14, 1, 0, 1 },    /* TXD2 */
    { QE_PORTA, 0x15, 1, 0, 1 },    /* TXD3 */
    { QE_PORTA, 0x16, 2, 0, 1 },    /* RXD0 */
    { QE_PORTA, 0x17, 2, 0, 1 },    /* RXD1 */
    { QE_PORTA, 0x18, 2, 0, 1 },    /* RXD2 */
    { QE_PORTA, 0x19, 2, 0, 1 },    /* RXD3 */
    { QE_PORTA, 0x1A, 2, 0, 1 },    /* RX_ER */
    { QE_PORTA, 0x1B, 1, 0, 1 },    /* TX_ER */
    { QE_PORTA, 0x1C, 2, 0, 1 },    /* RX_DV */
    { QE_PORTA, 0x1D, 2, 0, 1 },    /* COL */
    { QE_PORTA, 0x1E, 1, 0, 1 },    /* TX_EN */
    { QE_PORTA, 0x1F, 2, 0, 1 },    /* CRS */
    { QE_PORTA, 0x0D, 2, 0, 1 },    /* RX_CLK (CLK9) */
    { QE_PORTD, 0x18, 2, 0, 1 },    /* TX_CLK (CLK10) */
    { QE_PORTB, 0x00, 1, 0, 1 },    /* TXD0 */
    { QE_PORTB, 0x01, 1, 0, 1 },    /* TXD1 */
    { QE_PORTB, 0x02, 1, 0, 1 },    /* TXD2 */
    { QE_PORTB, 0x03, 1, 0, 1 },    /* TXD3 */
    { QE_PORTB, 0x04, 2, 0, 1 },    /* RXD0 */
    { QE_PORTB, 0x05, 2, 0, 1 },    /* RXD1 */
    { QE_PORTB, 0x06, 2, 0, 1 },    /* RXD2 */
    { QE_PORTB, 0x07, 2, 0, 1 },    /* RXD3 */
    { QE_PORTB, 0x08, 2, 0, 1 },    /* RX_ER */
    { QE_PORTB, 0x09, 1, 0, 1 },    /* TX_ER */
    { QE_PORTB, 0x0A, 2, 0, 1 },    /* RX_DV */
    { QE_PORTB, 0x0B, 2, 0, 1 },    /* COL */
    { QE_PORTB, 0x0C, 1, 0, 1 },    /* TX_EN */
    { QE_PORTB, 0x0D, 2, 0, 1 },    /* CRS */
    { QE_PORTD, 0x1F, 2, 0, 1 },    /* RX_CLK (CLK7) */
    { QE_PORTD, 0x06, 2, 0, 1 },    /* TX_CLK (CLK8) */

    { QE_PORTB, 0x12, 1, 0, 1 },    /* TXD0 */
    { QE_PORTB, 0x13, 1, 0, 1 },    /* TXD1 */
    { QE_PORTB, 0x14, 1, 0, 1 },    /* TXD2 */
    { QE_PORTB, 0x15, 1, 0, 1 },    /* TXD3 */
    { QE_PORTB, 0x16, 2, 0, 1 },    /* RXD0 */
    { QE_PORTB, 0x17, 2, 0, 1 },    /* RXD1 */
    { QE_PORTB, 0x18, 2, 0, 1 },    /* RXD2 */
    { QE_PORTB, 0x19, 2, 0, 1 },    /* RXD3 */
    { QE_PORTB, 0x1A, 2, 0, 1 },    /* RX_ER */
    { QE_PORTB, 0x1B, 1, 0, 1 },    /* TX_ER */
    { QE_PORTB, 0x1C, 2, 0, 1 },    /* RX_DV */
    { QE_PORTB, 0x1D, 2, 0, 1 },    /* COL */
    { QE_PORTB, 0x1E, 1, 0, 1 },    /* TX_EN */
    { QE_PORTB, 0x1F, 2, 0, 1 },    /* CRS */

    /* MDIO */

    { QE_PORTD, 0x04, 3, 0, 2 },    /* MDIO */
    { QE_PORTD, 0x05, 1, 0, 2 },    /* MDC */
};

const struct hcfResource quiccEngineResources[] = {
    { "regBase",        HCF_RES_INT,  { (void *)(CCSBAR + QUICC_ENGINE_BA) } },
    { "regBase1",       HCF_RES_INT,  { (void *)(CCSBAR + QE_IO_PORTS) } },
    { "muRamBase",      HCF_RES_INT,  { (void *)(CCSBAR + QUICC_ENGINE_BA + QE_MURAM_BA) } },
    { "muRamPoolSize",  HCF_RES_INT,  { (void *)QE_MURAM_SIZE } },
    { "qeIoTable",      HCF_RES_ADDR, { (void *)&qeIoTable[0] } },
    { "qeIoTableSize",  HCF_RES_INT,  { (void *)NELEMENTS(qeIoTable) } }
};

#define quiccEngine0Num NELEMENTS(quiccEngineResources)

#endif /* INCLUDE_QUICC_ENGINE_UTILS */

struct intrCtlrInputs quiccEngineIntCtlrInputs[] = {
    { QE_INUM_UCC3,     "qefcc", 0, 0 },
    { QE_INUM_UCC2,     "qefcc", 1, 0 },
    { QE_INUM_UCC4,     "qefcc", 2, 0 },
};

const struct hcfResource quiccEngineIntCtlr0Resources[] = {
    { VXB_REG_BASE,         HCF_RES_INT,  { (void *)(CCSBAR + QUICC_ENGINE_BA) } },
    { "input",              HCF_RES_ADDR, { (void *)&quiccEngineIntCtlrInputs[0] } },
    { "inputTableSize",     HCF_RES_INT,  { (void *)NELEMENTS(quiccEngineIntCtlrInputs) } },
    { "intMaskTable",       HCF_RES_ADDR, { (void *)&qeEnableHighPriInts[0] } },
    { "intMaskTableSize",   HCF_RES_INT,  { (void *) QE_IVEC_MAX } },
    { "inumMaskTable",      HCF_RES_ADDR, { (void *)&qeINumToMaskPattern[0] } },
    { "inumMaskTableSize",  HCF_RES_INT,  { (void *) QE_IVEC_MAX } }
};

#define quiccEngineIntCtlr0Num NELEMENTS(quiccEngineIntCtlr0Resources)

#endif /* QUICC_ENGINE */

const struct intrCtlrInputs ppcIntCtlrInputs[] = {
    { 0, "quiccIntCtlr",    0, 0 },
    { 1, "ppcDecTimerDev",  0, 0 },
};

const struct hcfResource ppcIntCtlr0Resources[] = {
    { VXB_REG_BASE,         HCF_RES_INT,  { (void *)TRUE } },
    { "input",              HCF_RES_ADDR, { (void *)&ppcIntCtlrInputs[0] } },
    { "inputTableSize",     HCF_RES_INT,  { (void *)NELEMENTS(ppcIntCtlrInputs) } },
};

#define ppcIntCtlr0Num NELEMENTS(ppcIntCtlr0Resources)

struct intrCtlrInputs quiccIntCtlrInputs[] = {
    { INUM_UART1,    "ns16550",             0, 0 },
    { INUM_UART2,    "ns16550",             1, 0 },
    { INUM_QE_HIGH,  "quiccEngineIntCtlr",  0, 0 },
    { INUM_QE_LOW,   "quiccEngineIntCtlr",  0, 1 }
};

const struct hcfResource quiccIntCtlr0Resources[] = {
    { VXB_REG_BASE,         HCF_RES_INT,  { (void *)(CCSBAR + SIC_REG_BA) } },
    { "input",              HCF_RES_ADDR, { (void *)&quiccIntCtlrInputs[0] } },
    { "inputTableSize",     HCF_RES_INT,  { (void *)NELEMENTS(quiccIntCtlrInputs) } },
    { "intMaskTable",       HCF_RES_ADDR, { (void *)&quiccIntMask[0] } },
    { "intMaskTableSize",   HCF_RES_INT,  { (void *) QUICC_IVEC_MAX } },
    { "inumMaskTable",      HCF_RES_ADDR, { (void *)&quiccInumToMaskPattern[0] } },
    { "inumMaskTableSize",  HCF_RES_INT,  { (void *) QUICC_IVEC_MAX } }
};

#define quiccIntCtlr0Num NELEMENTS(quiccIntCtlr0Resources)

const struct hcfResource ppcDecTimerDevResources[] = {
    { VXB_REG_BASE,         HCF_RES_INT,  { (void *)0 } },
    { "decClockFreq",       HCF_RES_ADDR, { (void *)sysClkFreqGet } },
    { "minClkRate",         HCF_RES_INT,  { (void *)SYS_CLK_RATE_MIN } },
    { "maxClkRate",         HCF_RES_INT,  { (void *)SYS_CLK_RATE_MAX } }
};

#define ppcDecTimerDevNum NELEMENTS(ppcDecTimerDevResources)

const struct hcfResource generalPurposeTimerResources12[] = {
    { "regBase",            HCF_RES_INT,  { (void *)(CCSBAR | QUICC_GTM1_BASE) } },
    { "irq",                HCF_RES_INT,  { (void *)(INUM_GTM2) } },
    { "irqLevel",           HCF_RES_INT,  { (void *)(INUM_GTM2) } },
    { "regBase1",           HCF_RES_INT,  { (void *)(CCSBAR | QUICC_GTM1_BASE | 0x10) } },
    { "clkRateMin",         HCF_RES_INT,  { (void *)AUX_CLK_RATE_MIN } },
    { "clkRateMax",         HCF_RES_INT,  { (void *)AUX_CLK_RATE_MAX } },
    { "clkFrequency",       HCF_RES_INT,  { (void *)TIMESTAMP_HZ } }
};

#define generalPurposeTimerNum12 NELEMENTS(generalPurposeTimerResources12)

const struct hcfResource generalPurposeTimerResources34[] = {
    { "regBase",            HCF_RES_INT, { (void *)(CCSBAR | QUICC_GTM1_BASE | 0x04) } },
    { "irq",                HCF_RES_INT, { (void *)(INUM_GTM4) } },
    { "irqLevel",           HCF_RES_INT, { (void *)(INUM_GTM4) } },
    { "regBase1",           HCF_RES_INT, { (void *)(CCSBAR | QUICC_GTM1_BASE | 0x20) } },
    { "clkRateMin",         HCF_RES_INT, { (void *)AUX_CLK_RATE_MIN } },
    { "clkRateMax",         HCF_RES_INT, { (void *)AUX_CLK_RATE_MAX } },
    { "clkFrequency",       HCF_RES_INT, { (void *)TIMESTAMP_HZ } }
};

#define generalPurposeTimerNum34 NELEMENTS(generalPurposeTimerResources34)

const struct hcfDevice hcfDeviceList[] = {
    { "quiccIntCtlr",       0, VXB_BUSID_PLB, 0, quiccIntCtlr0Num,  quiccIntCtlr0Resources },
    { "ppcIntCtlr",         0, VXB_BUSID_PLB, 0, ppcIntCtlr0Num,    ppcIntCtlr0Resources },
#ifdef INCLUDE_UCC_MDIO
    { "uccMdio", 0, VXB_BUSID_PLB, 0, mdio0Num, mdio0Resources },
#endif /* INCLUDE_UCC_MDIO */
#ifdef INCLUDE_ICP175CPHY

    /*
     * Note: icpPhy must be initialized after uccMdio.
     */

    { "icpPhy", 0, VXB_BUSID_PLB, 0, icp0Num, icp0Resources },
#endif /* INCLUDE_ICP175CPHY */
#ifdef INCLUDE_UCC_VXB_END
    { "qefcc",0,VXB_BUSID_PLB, 0, qefcc0Num, qefcc0Resources },
    { "qefcc",1,VXB_BUSID_PLB, 0, qefcc1Num, qefcc1Resources },
    { "qefcc",2,VXB_BUSID_PLB, 0, qefcc2Num, qefcc2Resources },
#endif /* INCLUDE_UCC_VXB_END */
    { "ppcDecTimerDev",     0, VXB_BUSID_PLB, 0, ppcDecTimerDevNum, ppcDecTimerDevResources },
    { "quiccTimerDev",      0, VXB_BUSID_PLB, 0, generalPurposeTimerNum12,  generalPurposeTimerResources12 },
    { "quiccTimerDev",      1, VXB_BUSID_PLB, 0, generalPurposeTimerNum34,  generalPurposeTimerResources34},
#ifdef QUICC_ENGINE
    { "quiccEngineIntCtlr", 0, VXB_BUSID_PLB, 0, quiccEngineIntCtlr0Num,    quiccEngineIntCtlr0Resources },
    { "quiccEngine",        0, VXB_BUSID_PLB, 0, quiccEngine0Num,   quiccEngineResources},
#endif /* QUICC_ENGINE */
#ifdef INCLUDE_PCI_BUS
    { "m83xxPci",   0, VXB_BUSID_PLB, 0, m83xxPciNum, m83xxPciResources },
#endif /* INCLUDE_PCI_BUS */
    { "ns16550",    0, VXB_BUSID_PLB, 0, ns1655x1Num, ns1655x1Resources },
    { "ns16550",    1, VXB_BUSID_PLB, 0, ns1655x2Num, ns1655x2Resources }
};

const int hcfDeviceNum = NELEMENTS(hcfDeviceList);

VXB_INST_PARAM_OVERRIDE sysInstParamTable[] = {
    { NULL, 0, NULL, VXB_PARAM_END_OF_LIST, {(void *)0} }
};
