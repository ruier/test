/* sysPccard.c - BSP specific PCCARD support */

/*
Copyright (c) 2003-2005 Wind River Systems, Inc. 
The right to copy, distribute, modify, or otherwise make use 
of this software may be licensed only pursuant to the terms of 
an applicable Wind River license agreement. 
*/

/*
modification history
--------------------
01c,23jun05,mdo  Changing several PCI defines to common PCI defined names.
01b,19aug03,j_b  added dual PC Card interrupt, Intersil Prism END support and
                 ne2k interrupt support (dependent on SPR#90999 fix)
01a,14jun03,scm  written.
*/

/*
DESCRIPTION

Support for PC Card libraries and drivers.

INCLUDE FILES: yentaSockServ.h
*/

/* includes */

#include <vxWorks.h>

#include <drv/pccard/csLib.h>
#include <drv/pccard/ssLib.h>
#include <drv/pccard/yentaSockServ.h>
#include <drv/pci/pciConfigLib.h>
#include <drv/pci/pciIntLib.h>

#ifndef PCI2DRAM_BASE_ADRS
#define PCI2DRAM_BASE_ADRS              0x00000000
#endif /* PCI2DRAM_BASE_ADRS */

STATUS sysCardbusIntConnect (VOIDFUNCPTR * vector, VOIDFUNCPTR routine, int parameter);
STATUS sysCardbusIntDisconnect (VOIDFUNCPTR * vector, VOIDFUNCPTR routine, int parameter);

/* include enablers */
#ifdef INCLUDE_PCCARD_ATA
#define SYS_ATA_IO_OFFSET               (0)
#define SYS_ATA_IRQ_TO_INUM(irq)        (irq)

#define SYS_ATA_INT_ENABLE              pciDeviceIntEnable
#define SYS_ATA_INT_DISABLE             pciDeviceIntDisable

#define SYS_ATA_INT_CONNECT             sysCardbusIntConnect
#define SYS_ATA_INT_DISCONNECT          sysCardbusIntDisconnect

#include "pccard/enablers/sysAta.c"
#endif /* INCLUDE_PCCARD_ATA */

#ifdef INCLUDE_PCCARD_SERIAL
#define SYS_SERIAL_BIND
#define SYS_SERIAL_IO_OFFSET            (0)
#define SYS_SERIAL_IRQ_TO_INUM(irq)     (irq)

#define SYS_SERIAL_INT_CONNECT          sysCardbusIntConnect
#define SYS_SERIAL_INT_DISCONNECT       sysCardbusIntDisconnect

#define SYS_SERIAL_INT_ENABLE           pciDeviceIntEnable
#define SYS_SERIAL_INT_DISABLE          pciDeviceIntDisable

#define SYS_SERIAL_INT_FREE(irq)
#include "pccard/enablers/sysSerial.c"
#endif /* INCLUDE_PCCARD_SERIAL */

#ifdef INCLUDE_PCCARD_ELT_3C589_END
#undef SYS_ELT_3C589_BIND
#define SYS_ELT_3C589_IO_OFFSET         (0)
#define SYS_ELT_3C589_IRQ_TO_INUM(irq)  (irq)

#define SYS_ELT_3C589_INT_CONNECT       sysCardbusIntConnect
#define SYS_ELT_3C589_INT_DISCONNECT    sysCardbusIntDisconnect

#define SYS_ELT_3C589_INT_ENABLE        pciDeviceIntEnable
#define SYS_ELT_3C589_INT_DISABLE       pciDeviceIntDisable

#include "pccard/enablers/sysElt3c589End.c"
#endif /* INCLUDE_PCCARD_ELT_3C589_END */

#ifdef INCLUDE_PCCARD_NE2K_END
#define SYS_NE2K_BYTE_ACCESS            (FALSE)
#define SYS_NE2K_IO_OFFSET              (0)
#define SYS_NE2K_IRQ_TO_INUM(irq)       (irq)

#define SYS_NE2K_INT_CONNECT            sysCardbusIntConnect
#define SYS_NE2K_INT_DISCONNECT         sysCardbusIntDisconnect

#define SYS_NE2K_INT_ENABLE             pciDeviceIntEnable
#define SYS_NE2K_INT_DISABLE            pciDeviceIntDisable

/* END interrupt control routines */
IMPORT FUNCPTR ne2000EndIntConnect;
IMPORT FUNCPTR ne2000EndIntDisconnect;
IMPORT FUNCPTR ne2000EndIntEnable;
IMPORT FUNCPTR ne2000EndIntDisable;

#include "pccard/enablers/sysNe2kEnd.c"
#endif /* INCLUDE_PCCARD_NE2K_END */

#ifdef INCLUDE_PCCARD_EL_3C575_END
#define SYS_EL_3C575_BIND
#define SYS_EL_3C575_PCI_IO_OFFSET      (0)
#define SYS_EL_3C575_IRQ_TO_INUM(irq)   (irq)

#define SYS_EL_3C575_INT_CONNECT        sysCardbusIntConnect
#define SYS_EL_3C575_INT_DISCONNECT     sysCardbusIntDisconnect

#define SYS_EL_3C575_INT_ENABLE         pciDeviceIntEnable
#define SYS_EL_3C575_INT_DISABLE        pciDeviceIntDisable

#include "pccard/enablers/sysEl3c575End.c"
#endif /* INCLUDE_EL_3C575_END */

#ifdef INCLUDE_PCCARD_NETGEAR_END
#define SYS_NETGEAR_BIND
#define SYS_NETGEAR_PCI_IO_OFFSET       (0)
#define SYS_NETGEAR_IRQ_TO_INUM(irq)    (irq)

#define SYS_NETGEAR_INT_CONNECT         sysCardbusIntConnect
#define SYS_NETGEAR_INT_DISCONNECT      sysCardbusIntDisconnect

#define SYS_NETGEAR_INT_ENABLE          pciDeviceIntEnable
#define SYS_NETGEAR_INT_DISABLE         pciDeviceIntDisable

#include "pccard/enablers/sysNetgear.c"
#endif /* INCLUDE_PCCARD_NETGEAR_END */

#ifdef INCLUDE_PCCARD_INTPR_END
#define WLAN_DEFAULT_BYTE_ORDER         _LITTLE_ENDIAN

#define SYS_INTPR_IO_OFFSET             (0)
#define SYS_INTPR_IRQ_TO_INUM(irq)      (irq)

#define SYS_INTPR_INT_CONNECT           sysCardbusIntConnect
#define SYS_INTPR_INT_DISCONNECT        sysCardbusIntDisconnect

#define SYS_INTPR_INT_ENABLE            pciDeviceIntEnable
#define SYS_INTPR_INT_DISABLE           pciDeviceIntDisable

#include "pccard/enablers/sysIntPrEnd.c"
#endif /* INCLUDE_PCCARD_INTPR_END */

#if (defined(INCLUDE_PCCARD_ATA) || defined (INCLUDE_PCCARD_ELT_3C589_END) || \
     defined (INCLUDE_PCCARD_NE2K_END))
#include "sysUtil.c"
#endif

/* defines */

/* globals */

#ifdef INCLUDE_PCCARD_ATA

#include "drv/hdisk/ataDrv.h"

/*
 * The first member in the ATA_TYPE struct has a dual purpose.
 *   1) If cylinders == 0, the device location is not probed at startup.
 *   2) If cylinders |= 0, the device location is probed, and if a device
 *      is found, the driver will fill in the first 3 member of the struct
 *      with number of cylinders, number of heads, and sectors per track.
 *
 * The last 2 members of the struct are static and should not be changed.
 *
 * The ATA_TYPE struct has the following layout:
 *   int cylinders;
 *   int heads;
 *   int sectorsTrack;
 *   int bytesSector;
 *   int precomp;
 *
 * NOTE: If configType == ATA_GEO_FORCE, the user needs to fill in
 * values for cylinders, heads, and sectorsTrack.
 */

ATA_TYPE ataTypes [ATA_MAX_CTRLS][ATA_MAX_DRIVES] =
    {
    {
    {1, 0, 0, 512, 0xFF}, /* controller 0, drive 0 */
    },
    {
    {1, 0, 0, 512, 0xFF}, /* controller 1, drive 0 */
    }
    };

ATA_RESOURCE ataResources[ATA_MAX_CTRLS];
#endif /* INCLUDE_ATA */

LOCAL YENTA_DRIVER pci1420SSDriver;

LOCAL UINT32 pci142016BitIoResource[] =
        {
            PCI_IO_BASE,  /* Base I/O */
            0x00001000    /* 4K Bytes */
        };

LOCAL UINT32 pci142016BitMemResource[] =
        {
            PCI_MEM_BASE, /* Base Mem */
            0x00040000    /* 256 KB   */
        };

/***********************************************************************
*
* sysCardbusIntConnect - Socket interrupt connection
*
* This routine connects an interrupt handler to a Cardbus
* interrupt. Also used for the socket status change interrupt.
*
* RETURNS: OK or ERROR
*/

STATUS sysCardbusIntConnect
    (
    VOIDFUNCPTR * vector,
    VOIDFUNCPTR routine,
    int parameter
    )
    {
    return pciDeviceIntConnect((int)vector, (VOIDFUNCPTR *)routine, parameter);
    }

/***********************************************************************
*
* sysCardbusIntDisconnect - Disconnect socket interrupt
*
* This routine disconnects an interrupt handler from a Cardbus
* interrupt. Also used for the socket status change interrupt.
*
* RETURNS: OK or ERROR
*
* ERRNO
*/

STATUS sysCardbusIntDisconnect
    (
    VOIDFUNCPTR * vector,
    VOIDFUNCPTR routine,
    int parameter
    )
    {
    return pciDeviceIntDisconnect2((int)vector, (VOIDFUNCPTR *)routine, parameter);
    }

/***********************************************************************
*
* sysCardbusIntEnable - Socket interrupt enable
*
* This routine enables the Cardbus interrupt. Also used for the socket
* status change interrupt.
*
* RETURNS: OK or ERROR
*
* ERRNO
*/

STATUS sysCardbusIntEnable
    (
    int intNum /* Interrupt to Enable */
    )
    {
    pciDeviceIntEnable (intNum);
    return OK;
    }

/***********************************************************************
*
* sysPccardMemOffset - Return PC Card memory space offset
*
* Returns the offset needed to access PC Card memory space. Card
* Services will add this offset before accessing PC Card attribute
* memory. When the socket controller is PCI based, this will normally
* return the offset of PCI non-prefetchable memory space.
*
* This returns 0 as there is a 1-to-1 mapping of PCI space on this
* platform.
*
* RETURNS: 0, always
*
* ERRNO
*/

int sysPccardMemOffset (void)
    {
    return 0;
    }

/***********************************************************************
*
* sys16BitIoPoolAdd - Add 16-bit IO space to resource pool
*
* This is called by Card Services to add memory to the 16-bit IO space
* pool.
*
* RETURNS: N/A
*
* ERRNO
*/

void sys16BitIoPoolAdd
    (
    CS_POOL_ID poolId
    )
    {
    int i;

    for (i = 0; i < NELEMENTS (pci142016BitIoResource); i += 2)
        {
        csPoolBaseSet (poolId, pci142016BitIoResource[i]);
        csPoolAdd (poolId, 0, pci142016BitIoResource[i + 1]);
        }
    }

/***********************************************************************
*
* sys16BitMemPoolAdd - Add 16-bit memory space to resource pool
*
* This is called by Card Services to add memory to the 16-bit memory
* space pool.
*
* RETURNS: N/A
*
* ERRNO
*/

void sys16BitMemPoolAdd
    (
    CS_POOL_ID poolId
    )
    {
    int i;

    for (i = 0; i < NELEMENTS (pci142016BitMemResource); i += 2)
        {
        csPoolBaseSet (poolId, pci142016BitMemResource[i]);
        csPoolAdd (poolId, 0, pci142016BitMemResource[i + 1]);
        }
    }

/**************************************************************************
*
* sysPci1420BridgePreConfigInit - Reserve PC Card memory
*
* This routine is called by the PCI auto-configuration process when it
* encounters a bridge. It reserves the memory needed for PC Card
* 16-bit devices.
*
* RETURNS: OK/ERROR
*
* ERRNO
*/

void sysPci1420BridgePreConfigInit
    (
    PCI_AUTO_CONFIG_OPTS * pSystem,
    PCI_LOC * pPciLoc,
    UINT dev_vend
    )
    {
    UINT32 sizeAdj;
    UINT32 alignedBase;
    UINT8 subClass;
    UINT32 pccard16BitMemSize = pci142016BitMemResource[1];
    UINT32 pccard16BitIoSize = pci142016BitIoResource[1];

    pciConfigInByte (pPciLoc->bus, pPciLoc->device, pPciLoc->function,
                     PCI_CFG_SUBCLASS, &subClass);

    if (subClass != PCI_SUBCLASS_CARDBUS_BRIDGE)
        return;

    if (pPciLoc->function != 0)
        return;

    /* Reserve memory for PCCARD memory */
    pciAutoAddrAlign(pSystem->pciMemIo32,
                     (pSystem->pciMemIo32 + pSystem->pciMemIo32Size),
                     pccard16BitMemSize,
                     &alignedBase);

    PCI_AUTO_DEBUG_MSG("16-bit mem orig[0x%08x] new[0x%08x] adj[0x%08x]\n",
            (pSystem->pciMemIo32),
            alignedBase,
            (alignedBase - (pSystem->pciMemIo32)),
            0,
            0,
            0
            );

    sizeAdj = alignedBase - (pSystem->pciMemIo32) + pccard16BitMemSize;

    (pSystem->pciMemIo32Used) += sizeAdj;
    (pSystem->pciMemIo32Size) -= sizeAdj;
    (pSystem->pciMemIo32) += sizeAdj;

    pci142016BitMemResource[0] = alignedBase;
    pci142016BitMemResource[1] = pccard16BitMemSize;

    /* Reserve memory for PCCARD IO */
    pciAutoAddrAlign(pSystem->pciIo16,
                     (pSystem->pciIo16 + pSystem->pciIo16Size),
                     pccard16BitIoSize,
                     &alignedBase);

	PCI_AUTO_DEBUG_MSG("I/O 16 Base orig[0x%08x] new[0x%08x] adj[0x%08x]\n",
            (pSystem->pciIo16),
            alignedBase,
            (alignedBase - (pSystem->pciIo16)),
            0,
            0,
            0
            );

    sizeAdj = alignedBase - (pSystem->pciIo16) + pccard16BitIoSize;

    (pSystem->pciIo16Used) += sizeAdj;
    (pSystem->pciIo16Size) -= sizeAdj;
    (pSystem->pciIo16) += sizeAdj;

    pci142016BitIoResource[0] = alignedBase;
    pci142016BitIoResource[1] = pccard16BitIoSize;
    }

/**************************************************************************
*
* sysPci1420HwInit - PCI1420 initialization
*
* This routine finds out the PCI device, maps its memory and IO address.
* It must be done prior to initializing the driver.
*
* RETURNS: OK/ERROR
*
* ERRNO
*/

STATUS sysPci1420HwInit (void)
    {
    int pciBus;
    int pciDevice;
    int socket;
    int instance = 0;
    UINT8 irq[YENTA_MAX_SOCKETS], temp2;
    UINT32 temp1, i, j;

    while (pciFindClass(YENTA_PCI_CLASS_CARDBUS_BRIDGE, instance,
                        &pciBus, &pciDevice, &socket) != ERROR)
        {
        /* read ExCA base */
        pciConfigInLong(pciBus, pciDevice, socket, PCI_CFG_BASE_ADDRESS_0,
                        &pci1420SSDriver.baseAddr[socket]);

        pci1420SSDriver.baseAddr[socket] &= PCI_MEMBASE_MASK;

        /* Enable IO access, memory access, and bus mastering */
        pciConfigOutWord (pciBus, pciDevice, socket, PCI_CFG_COMMAND,
                          PCI_CMD_IO_ENABLE  |
                          PCI_CMD_MEM_ENABLE |
                          PCI_CMD_MASTER_ENABLE);

        instance++;
        }

    /* write to the multi-function routing register */
    pciConfigOutLong (pciBus, pciDevice, 0,
                      MULTIFUNC_REG,
                      0x00C01D22);

    for (i=0; i < instance; i++)
        {
        /* set power switch clock to be generated internally */
        pciConfigInLong (pciBus, pciDevice, i,
                         SYSTEM_CTRL_REG,
                         &temp1);

        temp1 |= P2CCLK;

        pciConfigOutLong (pciBus, pciDevice, i,
                          SYSTEM_CTRL_REG,
                          temp1);

        /* we need to wait after we start the clock */
        for (j=0; j<0x1FFFFF; j++);

        /* set interrupt signaling mode */
        pciConfigInByte (pciBus, pciDevice, i,
                         DEVICE_CTRL_REG,
                         &temp2);

        temp2 &= PARALLEL_PCI_MASK;

        pciConfigOutByte (pciBus, pciDevice, i,
                          DEVICE_CTRL_REG,
                          temp2);

        /* Read back autoconfigured IRQ */

        pciConfigInByte (pciBus, pciDevice, i, PCI_CFG_BRG_INT_LINE, &irq[i]);
        }


    /* Initialize driver structure */

    pci1420SSDriver.bus = pciBus;
    pci1420SSDriver.device = pciDevice;
    pci1420SSDriver.numSockets = instance;

    pci1420SSDriver.flags = YENTA_FLAGS_POLL_CSC;

    /* set type to prevent socket interrupt sharing */
    pci1420SSDriver.type = YENTA_TYPE_PCI142X;
    pci1420SSDriver.typeInit = NULL;

    pci1420SSDriver.sysCardbusIntConnect = sysCardbusIntConnect;
    pci1420SSDriver.sysCardbusIntDisconnect = sysCardbusIntDisconnect;
    pci1420SSDriver.sysCardbusIntEnable = sysCardbusIntEnable;

    pci1420SSDriver.sysExcaIntConnect = sysCardbusIntConnect;
    pci1420SSDriver.sysExcaIntDisconnect = sysCardbusIntDisconnect;

    pci1420SSDriver.cscIntVector[0] = INUM_TO_IVEC (irq[0]);
    pci1420SSDriver.cscIntLevel[0] = irq[0];
    pci1420SSDriver.cscIntVector[1] = INUM_TO_IVEC (irq[1]);
    pci1420SSDriver.cscIntLevel[1] = irq[1];

    pci1420SSDriver.excaIntVector[0] = INUM_TO_IVEC (PIC1_PC_INTA_IRQ);
    pci1420SSDriver.excaIntLevel[0] = PIC1_PC_INTA_IRQ;

    pci1420SSDriver.excaIntVector[1] = INUM_TO_IVEC (PIC1_PC_INTB_IRQ);
    pci1420SSDriver.excaIntLevel[1] = PIC1_PC_INTB_IRQ;

#ifdef INCLUDE_PCCARD_NE2K_END
    ne2000EndIntConnect = (FUNCPTR) SYS_NE2K_INT_CONNECT;
    ne2000EndIntDisconnect = (FUNCPTR) SYS_NE2K_INT_DISCONNECT;
    ne2000EndIntEnable = (FUNCPTR) SYS_NE2K_INT_ENABLE;
    ne2000EndIntDisable = (FUNCPTR) SYS_NE2K_INT_DISABLE;
#endif

    return OK;
    }

/***********************************************************************
*
* sysPccardInit - Initialize PCCARD services and enablers
*
* This routine initializes the socket controller and PCCARD
* enablers. It is called from csInit.
*
* RETURNS: OK or ERROR
*
* ERRNO
*/

STATUS sysPccardInit (void)
    {

    sysPci1420HwInit ();

    if (yentaInit (&pci1420SSDriver) == ERROR)
        return ERROR;

    /* Register enablers */

#ifdef INCLUDE_PCCARD_ATA

    /* PCMCIA on both controllers */

    ataResources[0].ctrlType = ATA_PCMCIA;
    ataResources[1].ctrlType = ATA_PCMCIA;

    sysAtaInit ();
#endif /* INCLUDE_PCCARD_ATA */

#ifdef INCLUDE_PCCARD_SERIAL
    sysSerialInit ();
#endif /* INCLUDE_PCCARD_SERIAL */

#ifdef INCLUDE_PCCARD_ELT_3C589_END
    sysElt3c589Init ();
#endif /* INCLUDE_PCCARD_ELT_3C589_END */

#ifdef INCLUDE_PCCARD_NE2K_END
    sysNe2kInit ();
#endif /* INCLUDE_PCCARD_NE2k_END */

#ifdef INCLUDE_PCCARD_EL_3C575_END
    sysEl3c575Init ();
#endif /* INCLUDE_PCCARD_EL_3C575_END */

#ifdef INCLUDE_PCCARD_NETGEAR_END
/*     sysNetgearInit (); */
#endif /* INCLUDE_PCCARD_NETGEAR_END */

#ifdef INCLUDE_PCCARD_INTPR_END
    sysIntPrInit ();
#endif /* INCLUDE_PCCARD_INTPR_END */

    return OK;
    }
