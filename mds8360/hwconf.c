/* hwconf.c - Hardware configuration support module */

/*
 * Copyright (c) 2005-2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01r,10dec10,y_y  fix miss pci macro error. (WIND00245940)
01q,04aug10,liu  clear mot83xxPci.h. (WIND00212294)
01p,23sep09,x_z  update quiccEngine configuration.
01o,15jul09,b_m  add QE IO port configuration table.
01n,29jul08,z_l  Code clean up.
01m,18mar08,wap  Switch from hEnd qeFcc driver to VxBus UCC driver
01l,29oct07,h_k  fixed conflict entries for quiccTimerDev. (CQ:101100)
01k,05sep07,dtr  WIND0092393 - Fix GTM masks.
01j,01aug07,h_k  added ccsBar and qeIoPorts to quiccEngineResources[].
01i,14jun07,dtr  Include pci auto library header for PCI_SYSTEM struct.
01h,06jun07,dtr  Support vxBus Interrupt controller drivers.
01g,07feb06,wap  Add VxBus parameter table
01f,04jan06,wap  Correct TSEC setup so interrupts work
01e,26oct05,mdo  renaming header files
01d,15sep05,to   
01c,14sep05,mdo  Add vxb prefix
01b,08aug05,mdo  Change WINDBUS to VXBUS
01a,28jul05,rls  Created.
*/

#include <vxWorks.h>
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/hwConf.h>
#include <hwif/util/vxbParamSys.h>
#include "config.h"
#include <hwif/vxbus/vxbIntrCtlr.h>
#include <drv/pci/pciAutoConfigLib.h>
#include <../src/hwif/h/mii/mv88E1x11Phy.h>
#include <../src/hwif/h/end/vxbUccEnd.h>
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

const UINT32        qeINumToMaskPattern[QE_IVEC_MAX] = 
    {
    0x00000000,         /* interrupt number 0: Error */
    0x00008000,         /* interrupt number 1: I2C */
    0x00004000,         /* interrupt number 2: SPI */
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
    0x08000000,         /* interrupt number 36: MCC1 */
    0x00000000,         /* interrupt number 37: */
    0x00000000,         /* interrupt number 38: */
    0x00000000,         /* interrupt number 39: */
    0x00800000,         /* interrupt number 40: UCC5 */
    0x00400000,         /* interrupt number 41: UCC6 */
    0x00200000,         /* interrupt number 42: UCC7 */
    0x00100000,         /* interrupt number 43: UCC8 */
    0x00000000,         /* interrupt number 44: */
    0x00000000,         /* interrupt number 45: */
    0x00000000,         /* interrupt number 46: */
    0x00000000,         /* interrupt number 47: */
    0x00000000,         /* interrupt number 48: */
    0x00000000,         /* interrupt number 49: */
    0x00000000,         /* interrupt number 50: */
    0x00000000,         /* interrupt number 51: */
    0x00000000,         /* interrupt number 52: */
    0x00000000,         /* interrupt number 53: */
    0x00000000,         /* interrupt number 54: */
    0x00000000,         /* interrupt number 55: */
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
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
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
    {0xFF000020, 0x30000000, 0x00000000},         /* IRQ4 */
    {0xFF000020, 0x30000000, 0x00000000},         /* IRQ5 */
    {0xFF000020, 0x30000000, 0x00000000},         /* IRQ6 */
    {0xFF000020, 0x30000000, 0x00000000},         /* IRQ7 **23**/
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
#ifndef QUICC_ENGINE
    {0x00000000, 0x00000000, 0x00000000},         /* TSEC1 Tx */
    {0x00000000, 0x00000000, 0x00000000},         /* TSEC1 Rx */
    {0x00000000, 0x00000000, 0x00000000},         /* TSEC1 Err */
    {0x00000000, 0x00000000, 0x00000000},         /* TSEC2 Tx */
    {0x00000000, 0x00000000, 0x00000000},         /* TSEC2 Rx */
    {0x00000000, 0x00000000, 0x00000000},         /* TSEC2 Err */
#else
    {0x00000000, 0x00000000, 0x00000000},         /* QE HOGH */
    {0x00000000, 0x00000000, 0x00000000},         /* QE LOW */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved*/
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
#endif
    {0xFC000020, 0x30000000, 0x00000000},         /* USB2 DR */
    {0xFC000020, 0x30000000, 0x00000000},         /* USB2 MPH */
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
#ifndef QUICC_ENGINE
    {0xFF000020, 0x00000000, 0x00000000},         /* PCI2 */
#else
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
#endif
    {0xFF000020, 0x30000000, 0x00000000},         /* RTC ALR */
    {0xFF000020, 0x30000000, 0x00000000},         /* MU */
    {0xFF000020, 0x30000000, 0x00000000},         /* SBA */
    {0xFF000020, 0x30000000, 0x00000000},         /* DMA */
    {0xFF000020, 0x30000000, 0x00000000},         /* GTM4 */
    {0xFF000020, 0x30000000, 0x00000000},         /* GTM8 */
    {0xFF000020, 0x30000000, 0x00000000},         /* CPIO1 */
    {0xFF000020, 0x30000000, 0x00000000},         /* GPIO2 */
    {0xFF000020, 0x30000000, 0x00000000},         /* DDR */
    {0xFF000020, 0x30000000, 0x00000000},         /* LBC */
    {0xFF000020, 0x30000000, 0x00000000},         /* GTM2 */
    {0xFF000020, 0x30000000, 0x00000000},         /* GTM6 */
    {0xFF000020, 0x30000000, 0x00000000},         /* PMC */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0xFF000020, 0x30000000, 0x00000000},         /* GTM3 */
    {0xFF000020, 0x30000000, 0x00000000},         /* GTM7 */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0xFF000020, 0x30000000, 0x00000000},         /* GTM1 */
    {0xFF000020, 0x30000000, 0x00000000},         /* GTM5 */
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
    0x00000004,         /* I2C1 */
    0x00000002,         /* I2C2 */
    0x00000001,         /* SPI */
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
    0x80000000,         /* TSEC1 Tx */
    0x40000000,         /* TSEC1 Rx */
    0x20000000,         /* TSEC1 Err */
    0x10000000,         /* TSEC2 Tx */
    0x08000000,         /* TSEC2 Rx */
    0x04000000,         /* TSEC2 Err */
    0x02000000,         /* USB2 DR */
    0x01000000,         /* USB2 MPH */
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
    0x10000000,         /* PCI2 */
    0x08000000,         /* RTC ALR */
    0x04000000,         /* MU */
    0x02000000,         /* SBA */
    0x01000000,         /* DMA */
    0x00800000,         /* GTM4 */
    0x00400000,         /* GTM8 */
    0x00200000,         /* CPIO1 */
    0x00100000,         /* GPIO2 */
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

#ifdef INCLUDE_PCI_BUS
IMPORT STATUS sysPciAutoconfigInclude();
IMPORT UCHAR sysPciAutoconfigIntrAssign(PCI_SYSTEM *, PCI_LOC *, UCHAR);

const struct hcfResource m83xxPciResources[] = {
    { "regBase",          HCF_RES_INT, { (void *)(CCSBAR + M83XXPCI0_REG_BASE)} },
    { "mem32Addr",        HCF_RES_ADDR,{ (void *)PCI_MEM_ADRS } },
    { "mem32Size",        HCF_RES_INT, { (void *)PCI_MEM_SIZE } },
    { "memIo32Addr",      HCF_RES_ADDR,{ (void *)PCI_MEMIO_ADRS } },
    { "memIo32Size",      HCF_RES_INT, { (void *)PCI_MEMIO_SIZE } },
    { "io32Addr",         HCF_RES_ADDR,{ (void *)PCI_IO_ADRS } },
    { "io32Size",         HCF_RES_INT, { (void *)PCI_IO_SIZE } },
    { "cacheSize",        HCF_RES_INT, { (void *)(_CACHE_ALIGN_SIZE / 4) } },
    { "maxLatAllSet",     HCF_RES_INT, { (void *)PCI_LAT_TIMER } },
    { "autoIntRouteSet",  HCF_RES_INT, { (void *)TRUE } },
    { "includeFuncSet",   HCF_RES_ADDR,{ (void *)sysPciAutoconfigInclude } },
    { "intAssignFuncSet", HCF_RES_ADDR,{ (void *)sysPciAutoconfigIntrAssign }},
    /* non vxbPciAutoConfig() values */
    { "pimmrBase",        HCF_RES_ADDR,{ (void *)PCI_BRIDGE_PIMMR_BASE_ADRS } },
    { "lclMemAddr",       HCF_RES_ADDR,{ (void *)LOCAL_MEM_LOCAL_ADRS } },
    { "lclMemMapSize",    HCF_RES_INT, { (void *)(512 * 1024 * 1024) } },
    { "mstrMemBus",       HCF_RES_ADDR,{ (void *)PCI_MSTR_MEM_BUS } },
    { "maxBusSet",        HCF_RES_INT, { (void *)3 } },
    { "owAttrMem",        HCF_RES_INT, { (void *)(M83XXPCI_OB_WINDOW_ENABLE_BIT | 
                                                  M83XXPCI_PREFETCHABLE         |  
                                                  M83XXPCI_OB_ATTR_WS_128M) } },
    { "owAttrMemIo",      HCF_RES_INT, { (void *)(M83XXPCI_OB_WINDOW_ENABLE_BIT | 
                                                  M83XXPCI_OB_ATTR_WS_64M) } },
    { "owAttrIo",         HCF_RES_INT, { (void *)(M83XXPCI_OB_WINDOW_ENABLE_BIT | 
                                                  M83XXPCI_OB_ATTR_IO_BIT       |  
                                                  M83XXPCI_OB_ATTR_WS_64M) } },
    { "iwAttr",           HCF_RES_INT, { (void *)(M83XXPCI_IB_WINDOW_ENABLE_BIT | 
                                                  M83XXPCI_PREFETCHABLE         |  
                                                  M83XXPCI_IB_ATTR_RTT_READ_SNOOP  | 
                                                  M83XXPCI_IB_ATTR_RTT_WRITE_SNOOP | 
                                                  M83XXPCI_IB_ATTR_IWS_256M) } },
    { "lawbar",          HCF_RES_INT,  { (void *)(M83XXPCI_LAWBAR0)}},
    { "lawar",           HCF_RES_INT,  { (void *)(M83XXPCI_LAWAR0)}},
    { "lawarAttr",       HCF_RES_INT,  { (void *)(M83XXPCI_LAWAR_ENABLE |
                                                  M83XXPCI_LAWAR_SIZE_256MB)}},
    { "pciConfigMechanism",HCF_RES_INT, { (void *)(0)}},
    { "autoConfig",      HCF_RES_INT, { (void *)(TRUE)}}
};

#define m83xxPciNum NELEMENTS(m83xxPciResources)
#endif /* INCLUDE_PCI_BUS */

/* vxBus resource files for onboard UARTS */

const struct hcfResource ns1655x1Resources[] =
    {
        { "regBase",     HCF_RES_INT,  {(void *)COM1_ADR} },
        { "irq",         HCF_RES_INT,  {(void *)INUM_UART1} },
        { "regInterval", HCF_RES_INT,  {(void *)DUART_REG_ADDR_INTERVAL} },
        { "irqLevel",    HCF_RES_INT,  {(void *)INUM_UART1} },
        { "clkFreq",     HCF_RES_ADDR, {(void *)&sysClkFreqGet} }

    };

#define ns1655x1Num NELEMENTS(ns1655x1Resources)

const struct hcfResource ns1655x2Resources[] =
    {
        { "regBase",     HCF_RES_INT,  {(void *)COM2_ADR} },
        { "irq",         HCF_RES_INT,  {(void *)INUM_UART2} },
        { "regInterval", HCF_RES_INT,  {(void *)DUART_REG_ADDR_INTERVAL} },
        { "irqLevel",    HCF_RES_INT,  {(void *)INUM_UART2} },
        { "clkFreq",     HCF_RES_ADDR, {(void *)&sysClkFreqGet} }

    };
#define ns1655x2Num NELEMENTS(ns1655x2Resources)

#ifdef QUICC_ENGINE
#ifdef INCLUDE_UCC_VXB_END

/*
 * On the 8360mds board, PHYs are physically wired according
 * to the following table:
 *
 *              Data port pins  Management port pins    MII address
 *              --------------  --------------------    -----------
 * PHY0:        UCC0            UCC0                    0
 * PHY1:        UCC1            UCC0                    1
 *
 * UCC0 is programmed to me the MDIO master (via the clock general
 * clock routing register). The VxBus UCC driver allows us to specify
 * the name of the device that provides PHY management support.
 * This device is "qefcc0" in both cases. We also specify the
 * "phyAddr" resource for each device, which is 0 for TSEC0
 * and 1 for TSEC1. We also specify that the UCCs are configured
 * for RGMII mode.
 */

const struct hcfResource qefcc0Resources[] = {
    { "regBase",        HCF_RES_INT,    { (void *)(CCSBAR + QUICC_ENGINE_BA + UCC1_BASE) } },
    { "intr0",          HCF_RES_INT,    { (void *)(QE_INUM_UCC1) } },
    { "intr0Level",     HCF_RES_INT,    { (void *)(QE_INUM_UCC1) } },
    { "uccNum",         HCF_RES_INT,    { (void *)1 } },
    { "phyAddr",        HCF_RES_INT,    { (void *)0 } },
    { "miiIfName",      HCF_RES_STRING, { (void *)"qefcc" } },
    { "miiIfUnit",      HCF_RES_INT,    { (void *)0 } },
    { "uccMedia",       HCF_RES_INT,    { (void *)UCC_MEDIA_RGMII } }
};
#define qefcc0Num NELEMENTS(qefcc0Resources)

const struct hcfResource qefcc1Resources[] = {
    { "regBase",        HCF_RES_INT,    { (void *)(CCSBAR + QUICC_ENGINE_BA + UCC2_BASE) } },
    { "intr0",          HCF_RES_INT,    { (void *)(QE_INUM_UCC2) } },
    { "intr0Level",     HCF_RES_INT,    { (void *)(QE_INUM_UCC2) } },
    { "uccNum",         HCF_RES_INT,    { (void *)2 } },
    { "phyAddr",        HCF_RES_INT,    { (void *)1 } },
    { "miiIfName",      HCF_RES_STRING, { (void *)"qefcc" } },
    { "miiIfUnit",      HCF_RES_INT,    { (void *)0 } },
    { "uccMedia",       HCF_RES_INT,    { (void *)UCC_MEDIA_RGMII } }

};
#define qefcc1Num NELEMENTS(qefcc1Resources)
#endif /* INCLUDE_UCC_VXB_END */

#ifdef INCLUDE_QUICC_ENGINE_UTILS

const QE_IOPORT_TABLE qeIoTable[] = {
    /* UCC1 Ethernet */
    
    { QE_PORTA,  1, 3, 0, 2 },  /* MDIO */
    { QE_PORTA,  2, 1, 0, 1 },  /* MDC */

    { QE_PORTA,  0, 2, 0, 1 },  /* RX_CLK */
    { QE_PORTA,  3, 1, 0, 1 },  /* TxD0 */
    { QE_PORTA,  4, 1, 0, 1 },  /* TxD1 */
    { QE_PORTA,  5, 1, 0, 1 },  /* TxD2 */
    { QE_PORTA,  6, 1, 0, 1 },  /* TxD3 */
    { QE_PORTA,  7, 1, 0, 1 },  /* TX_EN */
    { QE_PORTA,  8, 1, 0, 1 },  /* TX_ER */
    { QE_PORTA,  9, 2, 0, 1 },  /* RxD0 */
    { QE_PORTA, 10, 2, 0, 1 },  /* RxD1 */
    { QE_PORTA, 11, 2, 0, 1 },  /* RxD2 */
    { QE_PORTA, 12, 2, 0, 1 },  /* RxD3 */
    { QE_PORTA, 13, 2, 0, 1 },  /* RxD4 */
    { QE_PORTA, 14, 2, 0, 1 },  /* CRS */
    { QE_PORTA, 15, 2, 0, 1 },  /* RX_DV */
    { QE_PORTA, 16, 2, 0, 1 },  /* RX_ER */

    { QE_PORTB,  0, 2, 0, 2 },  /* RxD6 */
    { QE_PORTB,  1, 2, 0, 2 },  /* RxD5 */
    { QE_PORTB,  4, 2, 0, 2 },  /* RxD7 */
    { QE_PORTB,  6, 1, 0, 3 },  /* TxD4 */
    { QE_PORTB,  7, 1, 0, 1 },  /* TxD5 */
    { QE_PORTB,  9, 1, 0, 2 },  /* TxD6 */
    { QE_PORTB, 10, 1, 0, 2 },  /* TxD7 */
    { QE_PORTC,  8, 2, 0, 1 },  /* GTX125 - CLK9 */
    { QE_PORTC,  9, 1, 0, 3 },  /* GTX_CLK - CLK10 */
    
    /* UCC2 Ethernet */
    
    { QE_PORTA, 17, 1, 0, 1 },  /* TxD0 */
    { QE_PORTA, 18, 1, 0, 1 },  /* TxD1 */
    { QE_PORTA, 19, 1, 0, 1 },  /* TxD2 */
    { QE_PORTA, 20, 1, 0, 1 },  /* TxD3 */
    { QE_PORTA, 21, 1, 0, 1 },  /* TX_EN */
    { QE_PORTA, 22, 1, 0, 1 },  /* TX_ER */
    { QE_PORTA, 23, 2, 0, 1 },  /* RxD0 */
    { QE_PORTA, 24, 2, 0, 1 },  /* RxD1 */
    { QE_PORTA, 25, 2, 0, 1 },  /* RxD2 */
    { QE_PORTA, 26, 2, 0, 1 },  /* RxD3 */
    { QE_PORTA, 27, 2, 0, 1 },  /* RxD4 */
    { QE_PORTA, 28, 2, 0, 1 },  /* CRS */
    { QE_PORTA, 29, 2, 0, 1 },  /* RX_DV */
    { QE_PORTA, 30, 2, 0, 1 },  /* RX_ER */

    { QE_PORTB,  2, 1, 0, 1 },  /* TxD4 */
    { QE_PORTB,  3, 1, 0, 2 },  /* TxD5 */
    { QE_PORTB,  5, 1, 0, 3 },  /* TxD6 */
    { QE_PORTB,  8, 1, 0, 3 },  /* TxD7 */
    { QE_PORTB, 11, 2, 0, 2 },  /* RxD7 */
    { QE_PORTB, 12, 2, 0, 2 },  /* RxD5 */
    { QE_PORTB, 13, 2, 0, 3 },  /* RxD6 */

    { QE_PORTC,  2, 1, 0, 2 },  /* GTX CLK */
    { QE_PORTC,  3, 2, 0, 1 },  /* 125M CLK4*/

    { QE_PORTA, 31, 2, 0, 1 },  /* RX_CLK */

#ifdef INCLUDE_PCI_BUS

    { QE_PORTC,  5, 2, 0, 1 },
    { QE_PORTF,  0, 1, 0, 2 },
    { QE_PORTF,  1, 2, 0, 3 },
    { QE_PORTF,  2, 1, 0, 1 },
    { QE_PORTF,  3, 2, 0, 2 },
    { QE_PORTF,  4, 2, 0, 3 },
    { QE_PORTF,  5, 1, 0, 3 },
    { QE_PORTF,  6, 1, 0, 3 },
    { QE_PORTF,  7, 3, 0, 3 },
    { QE_PORTF,  8, 3, 0, 3 }, 
    { QE_PORTF,  9, 3, 0, 3 }, 
    { QE_PORTF, 10, 3, 0, 3 },
    { QE_PORTF, 11, 3, 0, 3 },
    { QE_PORTF, 12, 3, 0, 3 },
    { QE_PORTF, 13, 3, 0, 3 },
    { QE_PORTF, 14, 3, 0, 3 },
    { QE_PORTF, 15, 3, 0, 3 },
    { QE_PORTF, 16, 3, 0, 3 },
    { QE_PORTF, 17, 2, 0, 3 },
    { QE_PORTF, 18, 3, 0, 3 },
    { QE_PORTF, 19, 3, 0, 3 },
    { QE_PORTF, 20, 3, 0, 3 },
    { QE_PORTF, 21, 2, 0, 3 },
    { QE_PORTF, 22, 2, 0, 3 },
    { QE_PORTF, 23, 3, 0, 3 },
    { QE_PORTF, 24, 1, 0, 3 },
    { QE_PORTF, 25, 1, 0, 3 },
    { QE_PORTF, 26, 1, 0, 3 },
    { QE_PORTF, 27, 1, 0, 3 },
    { QE_PORTF, 28, 1, 0, 3 },
    { QE_PORTF, 29, 1, 0, 3 },

    { QE_PORTG,  0, 3, 0, 3 },
    { QE_PORTG,  1, 3, 0, 3 },
    { QE_PORTG,  2, 3, 0, 3 },
    { QE_PORTG,  3, 3, 0, 3 },
    { QE_PORTG,  4, 3, 0, 3 },
    { QE_PORTG,  5, 3, 0, 3 },
    { QE_PORTG,  6, 3, 0, 3 },
    { QE_PORTG,  7, 3, 0, 3 },
    { QE_PORTG,  8, 3, 0, 3 },
    { QE_PORTG,  9, 3, 0, 3 },
    { QE_PORTG, 10, 3, 0, 3 },
    { QE_PORTG, 11, 3, 0, 3 },
    { QE_PORTG, 12, 3, 0, 3 },
    { QE_PORTG, 13, 3, 0, 3 },
    { QE_PORTG, 14, 3, 0, 3 },
    { QE_PORTG, 15, 3, 0, 3 },
    { QE_PORTG, 16, 3, 0, 3 },
    { QE_PORTG, 17, 3, 0, 3 },
    { QE_PORTG, 18, 3, 0, 3 },
    { QE_PORTG, 19, 3, 0, 3 },
    { QE_PORTG, 20, 3, 0, 3 },
    { QE_PORTG, 21, 3, 0, 3 },
    { QE_PORTG, 22, 3, 0, 3 },
    { QE_PORTG, 23, 3, 0, 3 },
    { QE_PORTG, 24, 3, 0, 3 },
    { QE_PORTG, 25, 3, 0, 3 },
    { QE_PORTG, 26, 3, 0, 3 },
    { QE_PORTG, 27, 3, 0, 3 },
    { QE_PORTG, 28, 3, 0, 3 },
    { QE_PORTG, 29, 3, 0, 3 },
    { QE_PORTG, 30, 3, 0, 3 },
    { QE_PORTG, 31, 3, 0, 3 },

#endif /* INCLUDE_PCI_BUS */
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
    { QE_INUM_UCC2,    "qefcc", 1, 0 },
    { QE_INUM_UCC1,    "qefcc", 0, 0 },
};

const struct hcfResource quiccEngineIntCtlr0Resources[] = {
    { VXB_REG_BASE,        HCF_RES_INT,  {(void *)(CCSBAR + QUICC_ENGINE_BA) } },
    { "input",             HCF_RES_ADDR, {(void *)&quiccEngineIntCtlrInputs[0] } },
    { "inputTableSize",    HCF_RES_INT,  {(void *)NELEMENTS(quiccEngineIntCtlrInputs) } },
    { "intMaskTable",      HCF_RES_ADDR, {(void *)&qeEnableHighPriInts[0]} },
    { "intMaskTableSize",  HCF_RES_INT,  {(void *) QE_IVEC_MAX} },
    { "inumMaskTable",     HCF_RES_ADDR, {(void *)&qeINumToMaskPattern[0]} },
    { "inumMaskTableSize", HCF_RES_INT,  {(void *) QE_IVEC_MAX} }
};
#define quiccEngineIntCtlr0Num NELEMENTS(quiccEngineIntCtlr0Resources)

#endif /* QUICC_ENGINE */

const struct intrCtlrInputs ppcIntCtlrInputs[] = {
    { 0, "quiccIntCtlr", 0, 0 },
    { 1, "ppcDecTimerDev", 0, 0 },
};


const struct hcfResource ppcIntCtlr0Resources[] = {
    { VXB_REG_BASE,     HCF_RES_INT,  {(void *)TRUE} },
    { "input",          HCF_RES_ADDR, {(void *)&ppcIntCtlrInputs[0] } },
    { "inputTableSize", HCF_RES_INT,  {(void *)NELEMENTS(ppcIntCtlrInputs) } },
};
#define ppcIntCtlr0Num NELEMENTS(ppcIntCtlr0Resources)

struct intrCtlrInputs quiccIntCtlrInputs[] = {
    { INUM_UART2,    "ns16550", 1, 0 },
    { INUM_UART1,    "ns16550", 0, 0 },
    { INUM_QE_HIGH,  "quiccEngineIntCtlr", 0, 0 },
    { INUM_QE_LOW,   "quiccEngineIntCtlr", 0, 1 }
};

const struct hcfResource quiccIntCtlr0Resources[] = {
    { VXB_REG_BASE,        HCF_RES_INT,  {(void *)(CCSBAR + SIC_REG_BA) } },
    { "input",             HCF_RES_ADDR, {(void *)&quiccIntCtlrInputs[0] } },
    { "inputTableSize",    HCF_RES_INT,  {(void *)NELEMENTS(quiccIntCtlrInputs) }},
    { "intMaskTable",      HCF_RES_ADDR, {(void *)&quiccIntMask[0]} },
    { "intMaskTableSize",  HCF_RES_INT,  {(void *) QUICC_IVEC_MAX} },
    { "inumMaskTable",     HCF_RES_ADDR, {(void *)&quiccInumToMaskPattern[0]} },
    { "inumMaskTableSize", HCF_RES_INT,  {(void *) QUICC_IVEC_MAX} }

};
#define quiccIntCtlr0Num NELEMENTS(quiccIntCtlr0Resources)

const struct hcfResource ppcDecTimerDevResources[] =
    {
    {VXB_REG_BASE,   HCF_RES_INT,  {(void *)0}},
    {"decClockFreq", HCF_RES_ADDR, {(void *)sysClkFreqGet}},
    {"minClkRate",   HCF_RES_INT,  {(void *)SYS_CLK_RATE_MIN}},
    {"maxClkRate",   HCF_RES_INT,  {(void *)SYS_CLK_RATE_MAX}}
    };
#define ppcDecTimerDevNum NELEMENTS(ppcDecTimerDevResources)

const struct hcfResource generalPurposeTimerResources12[] =  {
    {"regBase",     HCF_RES_INT, {(void *)(CCSBAR | QUICC_GTM1_BASE)}},
    {"irq",         HCF_RES_INT, {(void *)(INUM_GTM2)} },
    {"irqLevel",    HCF_RES_INT, {(void *)(INUM_GTM2)} },
    {"regBase1",    HCF_RES_INT, {(void *)(CCSBAR | QUICC_GTM1_BASE | 0x10)}},
    {"clkRateMin",  HCF_RES_INT, {(void *)AUX_CLK_RATE_MIN}},
    {"clkRateMax",  HCF_RES_INT, {(void *)AUX_CLK_RATE_MAX}},
    {"clkFrequency",HCF_RES_INT, {(void *)TIMESTAMP_HZ}}
};

#define generalPurposeTimerNum12       NELEMENTS(generalPurposeTimerResources12)

const struct hcfResource generalPurposeTimerResources34[] =  {
    {"regBase",     HCF_RES_INT, {(void *)(CCSBAR | QUICC_GTM1_BASE | 0x04)}},
    {"irq",         HCF_RES_INT, {(void *)(INUM_GTM4)} },
    {"irqLevel",    HCF_RES_INT, {(void *)(INUM_GTM4)} },
    {"regBase1",    HCF_RES_INT, {(void *)(CCSBAR | QUICC_GTM1_BASE | 0x20)}},
    {"clkRateMin",  HCF_RES_INT, {(void *)AUX_CLK_RATE_MIN}},
    {"clkRateMax",  HCF_RES_INT, {(void *)AUX_CLK_RATE_MAX}},
    {"clkFrequency",HCF_RES_INT, {(void *)TIMESTAMP_HZ}}
};

#define generalPurposeTimerNum34       NELEMENTS(generalPurposeTimerResources34)

const struct hcfDevice hcfDeviceList[] = {
    { "quiccIntCtlr",       0, VXB_BUSID_PLB, 0, quiccIntCtlr0Num, quiccIntCtlr0Resources },
    { "ppcIntCtlr",         0, VXB_BUSID_PLB, 0, ppcIntCtlr0Num, ppcIntCtlr0Resources },
    { "ppcDecTimerDev",     0, VXB_BUSID_PLB, 0, ppcDecTimerDevNum,   ppcDecTimerDevResources },
    { "ns16550",            0, VXB_BUSID_PLB, 0, ns1655x1Num, ns1655x1Resources },
    { "ns16550",            1, VXB_BUSID_PLB, 0, ns1655x2Num, ns1655x2Resources },

#ifdef QUICC_ENGINE
    { "quiccEngine",        0, VXB_BUSID_PLB, 0, quiccEngine0Num, quiccEngineResources},
    { "quiccEngineIntCtlr", 0, VXB_BUSID_PLB, 0, quiccEngineIntCtlr0Num, quiccEngineIntCtlr0Resources },

#ifdef INCLUDE_UCC_VXB_END
    { "qefcc",              0, VXB_BUSID_PLB, 0, qefcc0Num, qefcc0Resources },
    { "qefcc",              1, VXB_BUSID_PLB, 0, qefcc1Num, qefcc1Resources },
#endif /* INCLUDE_UCC_VXB_END */
#endif /* QUICC_ENGINE */

#ifdef DRV_TIMER_QUICC_PPC
    {"quiccTimerDev",       0, VXB_BUSID_PLB, 0, generalPurposeTimerNum12, generalPurposeTimerResources12 },
    {"quiccTimerDev",       1, VXB_BUSID_PLB, 0, generalPurposeTimerNum34, generalPurposeTimerResources34 },
#endif /* DRV_TIMER_QUICC_PPC */     

#ifdef INCLUDE_PCI_BUS
    { "m83xxPci",           0, VXB_BUSID_PLB, 0, m83xxPciNum, m83xxPciResources }
#endif /* INCLUDE_PCI_BUS */
    };

const int hcfDeviceNum = NELEMENTS(hcfDeviceList);

VXB_INST_PARAM_OVERRIDE sysInstParamTable[] =
    {
    /*
     * The UCCs on the 8360MDS board are configured for RGMII mode, so we
     * must set the PHYs for RGMII mode too.
     */
    { "mv88E1x11Phy", 0, "mode", VXB_PARAM_INT32, {(void *)MV88E_MODE_RGMII} },
    { "mv88E1x11Phy", 1, "mode", VXB_PARAM_INT32, {(void *)MV88E_MODE_RGMII} },
    { NULL, 0, NULL, VXB_PARAM_END_OF_LIST, {(void *)0} }
    };
