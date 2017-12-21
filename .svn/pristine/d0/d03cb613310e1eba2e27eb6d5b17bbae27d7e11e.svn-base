/* usrUsbTargPciInit.c - Initialization of USB Target Controller PCI Interfaces*/

/*
 * Copyright (c) 2004-2005, 2007, 2010, 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01x,23feb11,w_x  Fix duplicates by usrDepend.c and usrUsbInit.c (WIND00247788) 
01v,21sep10,sem  Add new IA cpus. Remove pentium2/3
01u,30jun10,ghs  Fix WIND00214934
01t,10jun10,ghs  Temp fix WIND00214934
01s,18jul07,jrp  fixing header dependencies
01r,09apr05,ami  Interrupt Lines changes for malta4kc.
01q,26oct04,hch  Change the usage of pci functions to bus abstraction layer
                 functions
01p,15oct04,ami  Apigen Changes
01o,11oct04,ami  Apigen Changes
01n,29sep04,ami  Changes for MTX604 on NET2280
01m,29sep04,ami  Changes for MIPS on NET2280
01l,20sep04,ami  NET2280 tested for High Speed
01k,17sep04,hch  merge for Netchip instrumentation
01j,16sep04,hch  Removed MS7751SE01 conditional address mapping
01i,12aug04,hch  added include for usbIsp1582.h
01h,09aug04,hch  moved pciAssignResources to sysIsp1582PciDisconnect
01g,03aug04,hch  change the archtecture reference to SH7750 to BSP reference
                 to MS7751SE01
01f,02aug04,hch  changed CPU reference to PPC604 to BSP reference to MTX
                 fixed compiler warning
01e,29jul04,pdg  Diab warnings fixed
01d,23jul04,ami  Apigen Changes
01c,23jul04,ami  Coding Convention Changes
01b,19jul04,hch  created the file element
01a,14jul04,ami  PCI Interfaces for ISP 1582 TC
*/

/*
DESCRIPTION

This configlette initializes a Philips ISP 1582 & NetChip NET2280 Peripheral
Controller PCI Interface.

INCLUDE: usb/usbPlatform.h usb/pciConstants.h, usb/usbPcilib.h
         drv/usb/target/usbIsp1582.h usrUsbTargInit.c
*/

#ifndef __INCusrUsbTargPciInitc
#define __INCusrUsbTargPciInitc

/* includes */

#include "usb/usbPlatform.h"
#include "usb/pciConstants.h"
#include "usb/usbPciLib.h"           /* PCI bus functions */
#include "drv/usb/target/usbIsp1582.h"
#include "usrUsbTargInit.c"

/* defines */

/* To hold the vendor Id and the device id for Isp 1582 */
#define ISP1582_VENDOR_ID       0x10B5
#define ISP1582_DEVICE_ID       0x5406

#define ISP1582_ADDRESS_PORT    0
#define ISP1582_DATA_PORT       2
#ifndef ISP1582_MODE_REG
    #define ISP1582_MODE_REG    0x0c
#endif

#if (CPU_FAMILY == I80X86)
#define USB_PCI_MEM_SIZE        0x1000
#endif

/* To hold the vendor id and the device id for NET2280 */
#define NET2280_VENDOR_ID       0x17CC
#define NET2280_DEVICE_ID       0x2280

/* globals */

#ifdef INCLUDE_NET2280

UINT32  BADDR_NET2280 [3];
UINT16  IRQ_NET2280;

#endif

/* forward declaration */

#ifdef INCLUDE_NET2280

void sysNET2280PciInit (void);

#endif

IMPORT BOOL usbTargFlag;

/*******************************************************************************
*
* targMapControllerMemToSysMem - maps PCI controler memory
*
* This function is used to map the PCI controller memory to the system memory.
* When this function is called, the <pAddress> parameter specifies the address
* to be mapped. On successful return from this function, this parameter holds
* the mapped address.
*
* RETURNS: the mapped address
*
* ERRNO: none
*
*\NOMANUAL
*/

void targMapControllerMemToSysMem
    (
    UINT32 * pAddress
    )

    {
    *pAddress += usbPciMemioOffset();
    }

void usrUsbTargPciInit (void)
    {
#ifdef INCLUDE_NET2280
    sysNET2280PciInit ();
#endif

    }

#ifdef INCLUDE_PHILIPS1582
/*******************************************************************************
*
* sysIsp1582PciInit - to configure the PCI interface for ISP 1582
*
* This function is used to configure the PCI interface for ISP 1582. It
* obtains the PCI Configuration Header for the ISP 1582 and provides with
* the base address and the irq number.
*
* RETURNS: N/A
*
* ERRNO:
*  none
*/

void  sysIsp1582PciInit
    (
    long    * pBaseaddr,            /* base address */
    long    * pIrq                  /* irq number */
    )
    {
    UINT16  data16;
    int     PCIBusNumber;           /* PCI bus number */
    int     PCIDeviceNumber;        /* PCI device number */
    int     PCIFunctionNumber;      /* PCI function number */
    UINT32  bStatus ;               /* status */
    int     nDeviceIndex=0;         /* device index */
    UINT32  baseaddr1;              /* base address */
    UINT32  temp = 0;
    PCI_CFG_HEADER pciCfgHdr;       /* configuration header */

    /* Find the device */

    bStatus = USB_PCI_FIND_DEVICE(ISP1582_VENDOR_ID, ISP1582_DEVICE_ID,
                                  nDeviceIndex, &PCIBusNumber, &PCIDeviceNumber,
                                  &PCIFunctionNumber);

    /* Check whether the isp1582 Controller was found */

    if (bStatus != OK )
        {

        /* No ISP1592 Device found */

        printf (" pciFindDevice returned error \n ");
        return ;
        }

#if  defined(CPU_940T) || defined(CPU_920T)
        /* Assign the PCI resources */
    pciAssignResources(PCIBusNumber,
                       PCIDeviceNumber,
                       PCIFunctionNumber);
#endif

    data16 = USB_PCI_CFG_WORD_GET (PCIBusNumber, PCIDeviceNumber,
                                   PCIFunctionNumber, PCI_CFG_COMMAND);

    /* Enable the IO access */

    USB_PCI_CFG_WORD_OUT(PCIBusNumber, PCIDeviceNumber, PCIFunctionNumber,
                         PCI_CFG_COMMAND, (data16 | PCI_CMD_IO_ENABLE |
                         PCI_CMD_MASTER_ENABLE));

    /* Get the configuration header */

    usbPciConfigHeaderGet (PCIBusNumber, PCIDeviceNumber, PCIFunctionNumber,
                           &pciCfgHdr);

    /*
     * As it is I/O mapped, the least significant bit which
     * is set will be 1. So mask out the least significant bit.
     * Copy the value to the pBaseaddr pointer passed.
     */

     *pBaseaddr =  pciCfgHdr.baseReg[2] & ~0x01;

    /* Copy the interrupt line value to the pIrq pointer passed */

#ifdef _WRS_MIPS_4KC

    *pIrq = pciCfgHdr.intLine - INT_NUM_IRQ0;

#else
    *pIrq = pciCfgHdr.intLine;

#endif

#ifdef MTX
    *pIrq = pciCfgHdr.intLine - EXT_INTERRUPT_BASE;
#endif

    baseaddr1 = pciCfgHdr.baseReg[1] & ~0x01;

    /* Wait State */

    temp = USB_PCI_DWORD_IN (baseaddr1 + 0x18);
    temp &= ~0x3C;
    temp |= 2 << 2;
    USB_PCI_DWORD_OUT (baseaddr1 + 0x18 , temp);

    /* Added for PLX to route Local Inerrupt to PCI Interrupt*/

    temp = USB_PCI_DWORD_IN (baseaddr1 + 0x68);
    temp |= 0x900;
    USB_PCI_DWORD_OUT (baseaddr1 + 0x68, temp);
    temp = USB_PCI_DWORD_IN (baseaddr1 );
    USB_PCI_DWORD_OUT (baseaddr1, temp | 1);

    return;
    }

/*******************************************************************************
*
* sysIsp1582PciDisconnect - to reset ISP 1582 on warm- reboot
*
* This funtion is used to reset ISP 1582 on warm reboot. It also re-configures
* the PCI interface.
*
* RETURNS: N/A
*
* ERRNO:
*  none
*
* \NOMANUAL
*/

void  sysIsp1582PciDisconnect()
    {
    UINT16          data16;
    int             PCIBusNumber;       /* PCI bus number */
    int             PCIDeviceNumber;    /* device number */
    int             PCIFunctionNumber;  /* PCI function number */
    UINT32          bStatus ;           /* status */
    UINT32          nDeviceIndex=0;     /* devive index */
    UINT32          baseaddr;           /* base address */
    PCI_CFG_HEADER  pciCfgHdr;          /* pci configureation header */

    /* Find the device */

    bStatus = USB_PCI_FIND_DEVICE(ISP1582_VENDOR_ID, ISP1582_DEVICE_ID,
                                  nDeviceIndex, &PCIBusNumber, &PCIDeviceNumber,
                                  &PCIFunctionNumber);

    /* Check whether the isp1582 Controller was found */

    if (bStatus != OK )
        {

        /* No ISP1592 Device found */

        printf (" pciFindDevice returned error \n ");
        return ;
        }

#if  defined(CPU_940T) || defined(CPU_920T)
    /* Assign the PCI resources */
    pciAssignResources(PCIBusNumber,
                       PCIDeviceNumber,
                       PCIFunctionNumber);
#endif

    data16 = USB_PCI_CFG_WORD_GET (PCIBusNumber, PCIDeviceNumber, PCIFunctionNumber, PCI_CFG_COMMAND);

    /* Enable the IO access */

    USB_PCI_CFG_WORD_OUT(PCIBusNumber, PCIDeviceNumber, PCIFunctionNumber,
                         PCI_CFG_COMMAND,
                         (data16 | PCI_CMD_IO_ENABLE | PCI_CMD_MASTER_ENABLE));

    /* Get the configuration header */

    usbPciConfigHeaderGet (PCIBusNumber, PCIDeviceNumber, PCIFunctionNumber,
                           &pciCfgHdr);

    /*
     * As it is I/O mapped, the least significant bit which
     * is set will be 1. So mask out the least significant bit.
     * Copy the value to the pBaseaddr pointer passed.
     */

    baseaddr =  pciCfgHdr.baseReg[2] & ~0x01;

    /*
     * Issue a soft reset to initialize the registers
     * This is done by writing 1 to bit 4 followed by 0
     */

    USB_PCI_WORD_OUT (baseaddr + ISP1582_ADDRESS_PORT ,ISP1582_MODE_REG);
    USB_PCI_WORD_OUT (baseaddr + ISP1582_DATA_PORT , 0x10);

    USB_PCI_WORD_OUT (baseaddr + ISP1582_ADDRESS_PORT ,ISP1582_MODE_REG);
    USB_PCI_WORD_OUT (baseaddr + ISP1582_DATA_PORT , 0);

    return;
    }

#endif

#ifdef INCLUDE_NET2280

/*******************************************************************************
*
* sysNET2280PciInit - to map the PCI base addresses to memory
*
* This function maps the PCI Base Addresses of NET2280 to system memory
* RETURNS: N/A
*
* ERRNO:
*  none
*/

void  sysNET2280PciInit (void)
    {

#if (CPU_FAMILY == I80X86)

    UINT16          data16;
    int             PCIBusNumber;       /* PCI bus number */
    int             PCIDeviceNumber;    /* PCI device number */
    int             PCIFunctionNumber;  /* PCI function number */
    UINT32          bStatus ;           /* status */
    int             nDeviceIndex=0;     /* device index */
    PCI_CFG_HEADER  pciCfgHdr;          /* configuration header */
    UINT8           i = 0;

    /* Find the device */

    bStatus = USB_PCI_FIND_DEVICE(NET2280_VENDOR_ID, NET2280_DEVICE_ID,
                                  nDeviceIndex, &PCIBusNumber, &PCIDeviceNumber,
                                  &PCIFunctionNumber);

    /* Check whether the NET2280 Controller was found */

    if (bStatus != OK )
        {
        /* No NET2280 Device found */

        printf (" pciFindDevice returned error \n ");
        return ;
        }

    data16 = USB_PCI_CFG_WORD_GET (PCIBusNumber, PCIDeviceNumber,
                                   PCIFunctionNumber, PCI_CFG_COMMAND);

    /* Enable the memory access */

    USB_PCI_CFG_WORD_OUT(PCIBusNumber, PCIDeviceNumber, PCIFunctionNumber,
                         PCI_CFG_COMMAND, (data16 | PCI_CMD_MEM_ENABLE |
                         PCI_CMD_MASTER_ENABLE));

    /* Get the configuration header */

    usbPciConfigHeaderGet (PCIBusNumber, PCIDeviceNumber, PCIFunctionNumber,
                           &pciCfgHdr);

    /* Map all the address to the system memory */

    for (i = 0; i < 3; i++)
        {

        if (sysMmuMapAdd((void *)(pciCfgHdr.baseReg[i] & PCI_MEMBASE_MASK),
            USB_PCI_MEM_SIZE, VM_STATE_MASK_FOR_ALL, VM_STATE_FOR_PCI)
            == ERROR)
            {
            printf("Error in mapping the memory\n");
            return;
            }
         }
#endif

    return;
    }

/*******************************************************************************
*
* sys2NET2280PciInit - to configure the PCI interface for NET2280
*
* This function is used to configure the PCI interface for NET2280. It
* obtains the PCI Configuration Header for the NET2280 and provides with
* the base addresses and the irq number.
*
* 3 PCI Base Addresses are obtained:
* \ss
* PCI BASE ADDRESS [0] - for configuration regsiters
* PCI BASE ADDRESS [1] - for 8051 microcontroller
* PCI BASE ADDRESS [2] - for accessing the FIFO buffers directly
* \se
*
* RETURNS: N/A
*
* ERRNO:
*  none
*/

void  sys2NET2280PciInit (void)
    {
    UINT16          data16;
    int             PCIBusNumber;       /* PCI bus number */
    int             PCIDeviceNumber;    /* PCI device number */
    int             PCIFunctionNumber;  /* PCI function number */
    UINT32          bStatus;            /* status */
    int             nDeviceIndex=0;     /* device index */
    PCI_CFG_HEADER  pciCfgHdr;          /* configuration header */
    UINT8           i = 0;

    /* Find the device */

    bStatus = USB_PCI_FIND_DEVICE(NET2280_VENDOR_ID, NET2280_DEVICE_ID,
                                  nDeviceIndex, &PCIBusNumber, &PCIDeviceNumber,
                                  &PCIFunctionNumber);

    /* Check whether the NET2280 Controller was found */

    if (bStatus != OK )
        {

        /* No NET2280 Device found */

        printf (" pciFindDevice returned error \n ");
        return ;
        }

#if  defined(CPU_940T) || defined(CPU_920T)
        /* Assign the PCI resources */
    pciAssignResources(PCIBusNumber,
                       PCIDeviceNumber,
                       PCIFunctionNumber);
#endif

    data16 = USB_PCI_CFG_WORD_GET (PCIBusNumber, PCIDeviceNumber,
                                   PCIFunctionNumber, PCI_CFG_COMMAND);

    /* Enable the memory access */

    USB_PCI_CFG_WORD_OUT(PCIBusNumber, PCIDeviceNumber, PCIFunctionNumber,
                         PCI_CFG_COMMAND, (data16 | PCI_CMD_MEM_ENABLE |
                         PCI_CMD_MASTER_ENABLE));

    /* Get the configuration header */

    usbPciConfigHeaderGet (PCIBusNumber, PCIDeviceNumber, PCIFunctionNumber,
                           &pciCfgHdr);

    for (i = 0; i < 3; i++)
        {
        BADDR_NET2280[i] = pciCfgHdr.baseReg[i];
        targMapControllerMemToSysMem (&BADDR_NET2280[i]);
        }

#ifdef _WRS_MIPS_4KC

    IRQ_NET2280 = pciCfgHdr.intLine - INT_NUM_IRQ0;

    USB_PCI_CFG_BYTE_OUT(PCI_BUS_LOCAL,
                         nDeviceIndex,
                         PCIFunctionNumber,
                         PCI_CFG_DEV_INT_LINE,
                         IRQ_NET2280);
#else
    IRQ_NET2280 = pciCfgHdr.intLine;
#endif

#ifdef MTX
    IRQ_NET2280 = pciCfgHdr.intLine - EXT_INTERRUPT_BASE;
#endif

    return;
    }
#endif

#endif /* __INCusrUsbTargPciInitc */
