/* sysFei82557End.c - system configuration module for fei82557End */

/*
********************************************************************************
   This source and object code has been made available to you by IBM on an
   AS-IS basis.

   IT IS PROVIDED WITHOUT WARRANTY OF ANY KIND, INCLUDING THE WARRANTIES OF
   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE OR OF NONINFRINGEMENT
   OF THIRD PARTY RIGHTS.  IN NO EVENT SHALL IBM OR ITS LICENSORS BE LIABLE
   FOR INCIDENTAL, CONSEQUENTIAL OR PUNITIVE DAMAGES.  IBM'S OR ITS LICENSOR'S
   DAMAGES FOR ANY CAUSE OF ACTION, WHETHER IN CONTRACT OR IN TORT, AT LAW OR
   AT EQUITY, SHALL BE LIMITED TO A MAXIMUM OF $1,000 PER LICENSE.  No license
   under IBM patents or patent applications is to be implied by the copyright
   license.

   Any user of this software should understand that neither IBM nor its
   licensors will be responsible for any consequences resulting from the use
   of this software.

   Any person who transfers this source code or any derivative work must
   include the IBM copyright notice, this paragraph, and the preceding two
   paragraphs in the transferred software.

   Any person who transfers this object code or any derivative work must
   include the IBM copyright notice in the transferred software.

   COPYRIGHT   I B M   CORPORATION 2004
   LICENSED MATERIAL  -  PROGRAM PROPERTY OF  I B M"
********************************************************************************
\NOMANUAL
*/

/* Copyright 2004-2005 Wind River Systems, Inc. */

/*
*****************************************************************************
   COPYRIGHT   Applied Micro Circuits Corporation (AMCC) 2004
   LICENSED MATERIAL  -  PROGRAM PROPERTY OF AMCC
   All rights reserved
   AMCC Proprietary

********************************************************************************
\NOMANUAL
*/

/*
modification history
--------------------
01h,30mar05,dr   Remove warnings
01g,07mar05,dr   Merge with latest version from AMCC
01f,26jan05,dr   Removed hardcoded code bypass (#if 1)
01e,28oct04,dr   Fixes for apigen & synch'ed with ocotea version 01i
01d,08sep04,dr   Fixed compiler warnings
01c,07sep04,mdo  Documentation fixes for apigen
01b,08apr04,wdz  added check for address bits and EEPROM size
01a,05apr04,tr   created from ocotea version 01f
*/

/*
********************************************************************************
DESCRIPTION
 This is the WRS-supplied configuration module for the VxWorks
fei82557End (fei) END driver.  It performs the dynamic parameterization
of the fei82557End driver.  This technique of 'just-in-time'
parameterization allows driver parameter values to be declared
as something other than static strings.

INCLUDE FILES:
********************************************************************************
*/
#if (defined(INCLUDE_FEI82557END) && defined (INCLUDE_NETWORK) && defined (INCLUDE_END))

/* includes */

#include "vxWorks.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "end.h"
#include "config.h"

#include "drv/end/fei82557End.h"
#include "etherMultiLib.h"

/* imports */

IMPORT FUNCPTR   feiEndIntConnect;
IMPORT FUNCPTR   feiEndIntDisconnect;
IMPORT END_OBJ * fei82557EndLoad (char *);

/* defines */
#undef  PRO100B_PCI_VENDOR_ID
#define PRO100B_PCI_VENDOR_ID   0x8086  /* PCI vendor ID */
#undef  PRO100B_PCI_DEVICE_ID
#define PRO100B_PCI_DEVICE_ID   0x1030  /* PCI device ID */
#undef  PRO100S_PCI_VENDOR_ID
#define PRO100S_PCI_VENDOR_ID   0x8086  /* PCI vendor ID */
#undef  PRO100S_PCI_DEVICE_ID
#define PRO100S_PCI_DEVICE_ID   0x1229  /* PCI device ID */

#define PRO100_NO_DEVICE    0x0000

/* Intel EtherExpress PRO100B LAN Adapter type */

#define TYPE_PRO100B_PCI          1       /* Intel EtherExpress PRO-100B PCI */
#define TYPE_PRO100S_PCI          2       /* Intel EtherExpress PRO-100S PCI */
#define TYPE_PRO100_PCI_OTHER   254       /* Intel EtherExpress PRO-100S PCI */
#define TYPE_PRO100_NONE    255   /* No board type defined */

/* EEPROM control bits */

#define EE_SK           0x01            /* shift clock */
#define EE_CS           0x02            /* chip select */
#define EE_DI           0x04            /* chip data in */
#define EE_DO           0x08            /* chip data out */

/* EEPROM opcode */

#define EE_CMD_WRITE    0x05            /* WRITE opcode, 101 */
#define EE_CMD_READ     0x06            /* READ  opcode, 110 */
#define EE_CMD_ERASE    0x07            /* ERASE opcode, 111 */

/* EEPROM misc. defines */

#define EE_CMD_BITS     3               /* number of opcode bits */
#define EE_ADDR_BITS    6               /* number of address bits */
#define EE_DATA_BITS    16              /* number of data bits */
#define EE_SIZE         0x40            /* 0x40 words */
#define EE_SIZE_MAX     0x100           /* max size of EEPROM 0x100 WORDS */
#define EE_CHECKSUM     0xbaba          /* checksum */

/* unknown values */

#define UNKNOWN         0xFFFFFFFF

/* init string misc. defines  */
#define NTFDS 0x80  /* default number of transmit descriptor/buffers */
#define NRFDS 0x100  /* default number of receive descriptors */
#define FLAGS 0x00  /* default User flags .Not implemented */
#define BOFFSET 0x00 /* default Offset used to align IP header on word boundary */


/* FEI driver access routines */

/* typedefs */

typedef struct feiResource              /* FEI_RESOURCE */
    {
    UINT32      membaseCsr;             /* Base Address Register 0 */
    UINT32      iobaseCsr;              /* Base Address Register 1 */
    UINT32      membaseFlash;           /* Base Address Register 2 */
    char        irq;                    /* Interrupt Request Level */
    UINT32      configType;             /* type of configuration - unused */
    UINT32      boardType;              /* type of LAN board this unit is */
    UINT32      pciBus;                 /* PCI Bus number */
    UINT32      pciDevice;              /* PCI Device number */
    UINT32      pciFunc;                /* PCI Function number */
    UINT32      eeprom[EE_SIZE_MAX];    /* Ethernet Address of this unit */
    INT32       timeout;                /* timeout for the self-test */
    INT32       str[6];                 /* storage for the self-test result */
    volatile INT32 *pResults;           /* pointer to the self-test result */
    UINT        memLength;              /* required memory size */
    UINT        initialStateMask;       /* mask parameter to vmStateSet */
    UINT        initialState;           /* state parameter to vmStateSet */
    } FEI_RESOURCE;


/* locals */
LOCAL STATUS sys557IntEnable(int unit);
LOCAL STATUS sys557IntDisable(int unit);
LOCAL UINT32 sys557LocalToPciBusAdrs (int unit, UINT32 adrs);
LOCAL UINT32 sys557PciBusToLocalAdrs (int unit, UINT32 adrs);
LOCAL UINT32 feiUnits;                  /* number of FEIs we found */

LOCAL FEI_RESOURCE feiResources [FEI_MAX_UNITS] =
{
#if (FEI_MAX_UNITS > 2)
#if (FEI_MAX_UNITS > 3)
#if (FEI_MAX_UNITS > 4)
#warning feiResources[]: Maximum of 4 FEI devices supported
#else
    {(UINT32) UNKNOWN, (UINT32) UNKNOWN, (UINT32) UNKNOWN, (char) UNKNOWN, 
     (UINT32) UNKNOWN, (UINT32) UNKNOWN, (UINT32) UNKNOWN, (UINT32) UNKNOWN, 
     (UINT32) UNKNOWN, {(UINT32) UNKNOWN}, (INT32) UNKNOWN, {(INT32) UNKNOWN}, 
     NULL, (UINT) UNKNOWN, (UINT) UNKNOWN, (UINT) UNKNOWN},
#endif /* FEI_MAX_UNITS > 4 */
    {(UINT32) UNKNOWN, (UINT32) UNKNOWN, (UINT32) UNKNOWN, (char) UNKNOWN, 
     (UINT32) UNKNOWN, (UINT32) UNKNOWN, (UINT32) UNKNOWN, (UINT32) UNKNOWN, 
     (UINT32) UNKNOWN, {(UINT32) UNKNOWN}, (INT32) UNKNOWN, {(INT32) UNKNOWN}, 
     NULL, (UINT) UNKNOWN, (UINT) UNKNOWN, (UINT) UNKNOWN},
#endif /* FEI_MAX_UNITS > 3 */
    {(UINT32) UNKNOWN, (UINT32) UNKNOWN, (UINT32) UNKNOWN, (char) UNKNOWN, 
     (UINT32) UNKNOWN, (UINT32) UNKNOWN, (UINT32) UNKNOWN, (UINT32) UNKNOWN, 
     (UINT32) UNKNOWN, {(UINT32) UNKNOWN}, (INT32) UNKNOWN, {(INT32) UNKNOWN}, 
     NULL, (UINT) UNKNOWN, (UINT) UNKNOWN, (UINT) UNKNOWN},
#endif /* FEI_MAX_UNITS > 2 */
    {(UINT32) UNKNOWN, (UINT32) UNKNOWN, (UINT32) UNKNOWN, (char) UNKNOWN, 
     (UINT32) UNKNOWN, (UINT32) UNKNOWN, (UINT32) UNKNOWN, (UINT32) UNKNOWN, 
     (UINT32) UNKNOWN, {(UINT32) UNKNOWN}, (INT32) UNKNOWN, {(INT32) UNKNOWN}, 
     NULL, (UINT) UNKNOWN, (UINT) UNKNOWN, (UINT) UNKNOWN},
};

enum phy_chips
    {
    NonSuchPhy=0, I82553AB, I82553C, I82503,
    DP83840, S80C240, S80C24, I82555, DP83840A=10, UndefinedPhy
    };

LOCAL char isDeviceFound;

LOCAL UINT eeSize[FEI_MAX_UNITS];
LOCAL UINT addrBits[FEI_MAX_UNITS];

/* forward declarations */

LOCAL void     sys557eepromSize (int unit);
LOCAL UINT16   sys557eepromRead (int unit, int loc);
#if FALSE
/* The following routines are not being used and are therefore commented out */
LOCAL void     sys557eepromWrite (int unit, int loc, UINT16 data);
LOCAL void     sys557eepromWriteBits (int unit, UINT16 data, int bitlen);
LOCAL void     sys557eepromChecksumSet (int unit);
#endif  /* FALSE */
LOCAL UINT16   sys557mdioRead   (int unit, int phyId, int location);
LOCAL UINT16   sys557mdioWrite  (int unit, int phyId, int location, int value);
LOCAL int      sys557IntAck     (int unit);

/***************************************************************************
*
* sysDelay - delay for a moment
*
* This routine delays for a moment.
*
* RETURNS: N/A
*
* ERRNO
*
* \NOMANUAL
*/

void sysDelay
    (
    void
    )
    {
    sysUsDelay(60); /* 60 microseconds */
    }


/***************************************************************************
*
* sysFei82557EndLoad - load fei82557 (fei) device.
*
* This routine loads the fei device with initial parameters.
*
* RETURNS: pointer to END object or ERROR.
*
* ERRNO
*
* SEE ALSO: fei82557EndLoad()
*/

END_OBJ * sysFei82557EndLoad
    (
    char *  pParamStr,  /* ptr to initialization parameter string */
    void *  unused      /* unused optional argument */
    )
    {
    /*
     * The fei82557End driver END_LOAD_STRING should be:
     * "<memBase>:<memSize>:<nTfds>:<nRfds>:<flags>:<offset>"
     */

    char *  cp;
    char    paramStr [END_INIT_STR_MAX];   /* from end.h */
    END_OBJ *   pEnd;
    char * pMemBase;
    ULONG memSize,rfdSize,rbdSize,cfdSize,clusterSize;

    if ( !isDeviceFound )
        {
        pEnd = 0;
        return(pEnd);
        }

    if ( strlen (pParamStr) == 0 )
        {
        /*
         * muxDevLoad() calls us twice.  If the string is
         * zero length, then this is the first time through
         * this routine, so we just return.
         */

        pEnd = fei82557EndLoad (pParamStr);
        }
    else
        {
        /*
         * On the second pass though here, we actually create
         * the initialization parameter string on the fly.
         * Note that we will be handed our unit number on the
         * second pass through and we need to preserve that information.
         * So we use the unit number handed from the input string.
         */

        cp = strcpy (paramStr, pParamStr); /* cp points to paramStr */

        /* Now, we advance cp, by finding the end the string */

        cp += strlen (paramStr);

        /* finish off the initialization parameter string */

        /* Compute memory size needed */
        rfdSize = ((NRFDS + 1) * RFD_DESC_SIZE);
        rbdSize = ((NRFDS + 1) * RBD_SIZE);
        cfdSize = (NTFDS * (CFD_SIZE + CLUSTER_SIZE));
        clusterSize = (((NRFDS + 1) * 2) * CLUSTER_SIZE);

        memSize = rfdSize + rbdSize + cfdSize + clusterSize;

        pMemBase = cacheDmaMalloc(memSize);
        sprintf (cp,"0x%x:0x%x:0x%x:0x%x:0x%x:%d",
                 (UINT32)pMemBase,(UINT32)memSize,NTFDS,NRFDS,FLAGS,BOFFSET);

        if ( (pEnd = fei82557EndLoad (paramStr)) == NULL )
            {
            taskDelay(sysClkRateGet()*1);
            }
        }

    return(pEnd);
    }

/***************************************************************************
*
* sys557PciInit - prepare LAN adapter for 82557 initialization
*
* This routine find out the PCI device, and map its memory and IO address.
* It must be done prior to initializing the 82557, sys557Init().  Also
* must be done prior to MMU initialization, usrMmuInit().
*
* RETURNS: N/A
*
* ERRNO
*/

void sys557PciInit
    (
    void
    )
    {
    FEI_RESOURCE * pReso; /* pointer to fei information */
    int     pciBus;       /* pci bus number */
    int     pciDevice;    /* pci device number */
    int     pciFunc;      /* pci device function number */
    int     unit;         /* current fei unit number for specific dev type */
    UINT32  membaseCsr;   /* base address of device memory space */
    UINT32  iobaseCsr;    /* base address of device io space */
    UINT32  membaseFlash; /* base address of device flash space */
    char    irq;          /* device pci irq */
    int     pciDevIdList[] =
    {
    PRO100B_PCI_DEVICE_ID,      /* Pro100B */
    PRO100S_PCI_DEVICE_ID,      /* Pro100S */
    PRO100_NO_DEVICE /* terminator -- DO NOT REMOVE */
    };
    int *   pPciDevId;

    isDeviceFound = 0;

    /*
     * NOTE: unit is the PCI index number of the cards
     * matching the current DeviceID.  feiUnits is the
     * fei unit number matching the current card.
     */

    /* find all fei devices */
    unit = 0;       /* none found yet */
    pPciDevId = &pciDevIdList[0]; /* first DeviceID */

    for ( feiUnits = 0 ; feiUnits < FEI_MAX_UNITS; feiUnits++ )
        {

    /* loop through supported device types */
        while ( ( *pPciDevId != PRO100_NO_DEVICE ) &&
        ( pciFindDevice (PRO100B_PCI_VENDOR_ID,
                               *pPciDevId, unit,
                               &pciBus, &pciDevice, &pciFunc) )
                != OK )
            {
        /* go to next DeviceID in list, and retry */
        pPciDevId++;

        /*
         * pciFindDevice() needs to look for the first
         * instance (index=0) of the new pciDevID
         */
        unit = 0;
        }

    /*
     * if we got here, we either found a device or reached
     * the end of pciDevIdList[].  If we found a device,
     * *pPciDevId contains the DeviceID for the current board.
     */

    /* if no more DeviceIDs to check */
    if ( *pPciDevId == PRO100_NO_DEVICE )
        break;

        isDeviceFound = 1;

        /* record basic pci info */

        pReso            = &feiResources [feiUnits];
        pReso->pciBus    = pciBus;
        pReso->pciDevice = pciDevice;
        pReso->pciFunc   = pciFunc;
        pReso->boardType = TYPE_PRO100_NONE;

        /* ask device its memory base address and IO base address */

        pciConfigInLong (pReso->pciBus, pReso->pciDevice, pReso->pciFunc,
                         PCI_CFG_BASE_ADDRESS_0, &membaseCsr);
        pciConfigInLong (pReso->pciBus, pReso->pciDevice, pReso->pciFunc,
                         PCI_CFG_BASE_ADDRESS_1, &iobaseCsr);
        pciConfigInLong (pReso->pciBus, pReso->pciDevice, pReso->pciFunc,
                         PCI_CFG_BASE_ADDRESS_2, &membaseFlash);
        pciConfigInByte (pReso->pciBus, pReso->pciDevice, pReso->pciFunc,
                         PCI_CFG_DEV_INT_LINE, &irq);

        membaseCsr   &= PCI_MEMBASE_MASK;
        iobaseCsr    &= PCI_IOBASE_MASK;
        membaseFlash &= PCI_MEMBASE_MASK;

        /* over write the resource table with read value */

        pReso->membaseCsr   = membaseCsr;
        pReso->iobaseCsr    = iobaseCsr;
        pReso->membaseFlash = membaseFlash;
        pReso->irq          = irq;

        /* enable mapped memory and IO addresses */

        pciConfigOutWord (pReso->pciBus, pReso->pciDevice, pReso->pciFunc,
                          PCI_CFG_COMMAND, PCI_CMD_IO_ENABLE |
                          PCI_CMD_MEM_ENABLE | PCI_CMD_MASTER_ENABLE);

        unit++;
        }

    /* specify the interrupt connect/disconnect routines to be used */

    feiEndIntConnect = (FUNCPTR) intConnect;
    feiEndIntDisconnect = NULL;

    }

/***************************************************************************
*
* sys557Init - prepare LAN adapter for 82557 initialization
*
* This routine is expected to perform any adapter-specific or target-specific
* initialization that must be done prior to initializing the 82557.
*
* The 82557 driver calls this routine from the driver attach routine before
* any other routines in this library.
*
* This routine returns the interrupt level the <pIntLvl> parameter.
*
* RETURNS: OK or ERROR if the adapter could not be prepared for initialization.
*
* ERRNO
*/

STATUS sys557Init
    (
    int unit,                   /* unit number */
    FEI_BOARD_INFO *pBoard      /* board information for the end driver */
    )
    {
    FEI_RESOURCE *pReso;    /* resource ptr, initialized by sys557PciInit */
    UINT16 sum = 0;         /* checksum of eeprom on fei board */
    int ix;                 /* general index */
    int iy;                 /* general index */
    UINT16 value;           /* test return value */
    UINT16 pciDevId;         /* PCI DeviceID */

    if ( unit >= feiUnits )
        return(ERROR);

    /* figure out which fei resource we are working with */

    pReso = &feiResources [unit];

    /* set up the board */

    if (
     (pReso->boardType != TYPE_PRO100B_PCI) ||
     (pReso->boardType != TYPE_PRO100S_PCI) ||
     (pReso->boardType != TYPE_PRO100_PCI_OTHER)
       )   /* only setup once */
        {
        /* read the configuration in EEPROM */

        sys557eepromSize (unit); /*get the number of address bits and the EEPROM size for this unit*/

        for (ix = 0; ix < eeSize[unit]; ix++)
            {
            value = sys557eepromRead (unit, ix);
            pReso->eeprom[ix] = value;
            sum += value;
            }

            /*
             * Verify checksum correct. This will fail if part is not correctly
             * mapped and initialized as per sys557PciInit.
             */

        if ( sum != EE_CHECKSUM )
            printf ("i82557(%d): Invalid EEPROM checksum %#4.4x\n", unit, sum);

        /* DP83840-specific setup */

        if ( ((pReso->eeprom[6]>>8) & 0x3f) == DP83840 )
            {
            int reg23 = sys557mdioRead (unit, pReso->eeprom[6] & 0x1f, 23);
            sys557mdioWrite (unit, pReso->eeprom[6] & 0x1f, 23, reg23 | 0x0420);
            }

        /* perform a system self-test */

        pReso->timeout = 16000;             /* Timeout for self-test */
        pReso->pResults  = (volatile int *) cacheDmaMalloc(sizeof(pReso->str));
        pReso->pResults[0] = 0;
        pReso->pResults[1] = -1;
        sysOutLong((ULONG)PCI_MEMIO2LOCAL(pReso->membaseCsr + SCB_PORT),
        (ULONG)((int)pReso->pResults | 1));

        do
            {
            sysDelay ();        /* delay for one IO READ cycle */
            }
        while ( (pReso->pResults[1] == -1) && (--pReso->timeout >= 0) );

        /* find device ID */
        pciConfigInWord (pReso->pciBus, pReso->pciDevice, pReso->pciFunc,
        PCI_CFG_DEVICE_ID, &pciDevId);

        /* set board type accordingly */
        if ( pciDevId == PRO100B_PCI_DEVICE_ID )
            pReso->boardType = TYPE_PRO100B_PCI;
        else if ( pciDevId == PRO100S_PCI_DEVICE_ID )
            pReso->boardType = TYPE_PRO100S_PCI;
        else
        /* This should never happen */
        pReso->boardType = TYPE_PRO100_PCI_OTHER;
        }

    /* complete initialization of the board information structure */

    pBoard->vector   = pReso->irq;
    pBoard->baseAddr = pReso->membaseCsr;

    for ( ix = 0, iy = 0; ix < 3; ix++ )
        {
        pBoard->enetAddr[iy++] = pReso->eeprom[ix] & 0xff;
        pBoard->enetAddr[iy++] = (pReso->eeprom[ix] >> 8) & 0xff;
        }

    pBoard->intEnable     = sys557IntEnable;
    pBoard->intDisable    = sys557IntDisable;
    pBoard->intAck        = sys557IntAck;
    pBoard->sysLocalToBus = sys557LocalToPciBusAdrs;
    pBoard->sysBusToLocal = sys557PciBusToLocalAdrs;

    #ifdef FEI_10MB
    pBoard->phySpeed      = NULL;
    pBoard->phyDpx        = NULL;
    #endif

    return(OK);
    }

/***************************************************************************
*
* sys557IntEnable - enable 82557 interrupts
*
* This routine enables 8255x interrupts.  This may involve operations on
* interrupt control hardware.
*
* RETURNS: OK or ERROR for invalid arguments.
*
* ERRNO
*/

LOCAL STATUS sys557IntEnable
    (
    int unit   /* unit number */
    )
    {
    return intEnable(feiResources[unit].irq);
    }

/*******************************************************************************
*
* sys557IntDisable - disable 82557 interrupts
*
* This routine disables 8255x interrupts.  This may involve operations on
* interrupt control hardware.
*
* RETURNS: OK or ERROR for invalid arguments.
*
* ERRNO
*/

LOCAL STATUS sys557IntDisable
    (
    int unit   /* unit number */
    )
    {
    return intDisable(feiResources[unit].irq);
    }

/***************************************************************************
*
* sys557IntAck - acknowledge an 82557 interrupt
*
* This routine performs any 82557 interrupt acknowledge that may be
* required.  This typically involves an operation to some interrupt
* control hardware.
*
* This routine gets called from the 82557 driver's interrupt handler.
*
* This routine assumes that the PCI configuration information has already
* been setup.
*
* RETURNS: OK, or ERROR if the interrupt could not be acknowledged.
*
* ERRNO
*/

LOCAL STATUS sys557IntAck
    (
    int unit            /* unit number */
    )
    {
    FEI_RESOURCE *pReso = &feiResources [unit];

    switch ( pReso->boardType )
        {
        case TYPE_PRO100B_PCI:          /* handle PRO100B LAN Adapter */
        case TYPE_PRO100S_PCI:          /* handle PRO100S LAN Adapter */
        case TYPE_PRO100_PCI_OTHER: /* handle PRO100? LAN Adapter */
            /* no addition work necessary for the PRO100B/PRO100S */
            break;

        default:
            return(ERROR);
        }

    return(OK);
    }

/*******************************************************************************
*
* sys557eepromSize - get the size of the 82557 PCI ethernet device EEPROM
* and adjust eeSize and addrBits accordingly
*
* RETURNS: N/A.
*
* ERRNO
*
* \NOMANUAL
*/
LOCAL void sys557eepromSize
    (
    int      unit           /* END driver unit number */
    )
    {
    UINT32   iobase = (UINT32)PCI_IO2LOCAL(feiResources[unit].iobaseCsr);

    UINT16   retval = 0;
    UINT16   dataval;
    int      ix;

    volatile UINT16 dummy=1;

    /* enable EEPROM */

    sysOutWord (iobase + SCB_EEPROM, EE_CS);

    /* write the READ opcode */

    for (ix = EE_CMD_BITS - 1; ix >= 0; ix--)
        {
        dataval = (EE_CMD_READ & (1 << ix)) ? EE_DI : 0;
        sysOutWord (iobase + SCB_EEPROM, EE_CS | dataval);
        sysDelay ();        /* delay for one IO READ cycle */
        sysOutWord (iobase + SCB_EEPROM, EE_CS | dataval | EE_SK);
        sysDelay ();        /* delay for one IO READ cycle */
        sysOutWord (iobase + SCB_EEPROM, EE_CS | dataval);
        sysDelay ();        /* delay for one IO READ cycle */
        }

    /* write the location, the number of iterations gives the address size */

    addrBits[unit] = 0;
    while (TRUE)
        {
        dataval = 0; /*using 0 should be just fine here*/
        sysOutWord (iobase + SCB_EEPROM, EE_CS | dataval);
        sysDelay ();        /* delay for one IO READ cycle */
        sysOutWord (iobase + SCB_EEPROM, EE_CS | dataval | EE_SK);
        sysDelay ();        /* delay for one IO READ cycle */
        sysOutWord (iobase + SCB_EEPROM, EE_CS | dataval);
        sysDelay ();        /* delay for one IO READ cycle */
        dummy = sysInWord (iobase + SCB_EEPROM);
        addrBits[unit]++;
        if ((dummy & EE_DO) == 0)
          break;
        if (addrBits[unit] > 8)
          break; /*test for address bits is not working*/
        }

    switch (addrBits[unit])
        {
        case (6):
             eeSize[unit] = 0x40;
             break;
        case (8):
             eeSize[unit] = 0x100;
             break;
        default:
             printf("fei%d: EEPROM address size could not be detected; using default values.\n",
             unit);
             addrBits[unit] = 6;
             eeSize[unit] = 0x40;
             break;
        }


    /*dummy read the data */

    for (ix = EE_DATA_BITS - 1; ix >= 0; ix--)
        {
        sysOutWord (iobase + SCB_EEPROM, EE_CS | EE_SK);

        sysDelay ();        /* delay for one IO READ cycle */

        retval = (retval << 1) |
                 ((sysInWord (iobase + SCB_EEPROM) & EE_DO) ? 1 : 0);

        sysOutWord (iobase + SCB_EEPROM, EE_CS);

        sysDelay ();        /* delay for one IO READ cycle */
        }

    /* disable EEPROM */

    sysOutWord (iobase + SCB_EEPROM, 0x00);

    }

/*******************************************************************************
*
* sys557eepromRead - read a word from 82557 PCI ethernet device EEPROM
*
* Read a word from 82557 PCI ethernet device EEPROM.
*
* RETURNS: The EEPROM data word read in.
*
* ERRNO
*
* \NOMANUAL
*/
LOCAL UINT16 sys557eepromRead
    (
    int unit,           /* END driver unit number */
    int location        /* address of word to read */
    )
    {

    UINT32   iobase = (UINT32)PCI_IO2LOCAL(feiResources[unit].iobaseCsr);

    UINT16   retval = 0;
    UINT16   dataval;
    int      ix;

    volatile UINT16 dummy;

    /* enable EEPROM */

    sysOutWord (iobase + SCB_EEPROM, EE_CS);

    /* write the READ opcode */

    for (ix = EE_CMD_BITS - 1; ix >= 0; ix--)
        {
        dataval = (EE_CMD_READ & (1 << ix)) ? EE_DI : 0;
        sysOutWord (iobase + SCB_EEPROM, EE_CS | dataval);
        sysDelay ();        /* delay for one IO READ cycle */
        sysOutWord (iobase + SCB_EEPROM, EE_CS | dataval | EE_SK);
        sysDelay ();        /* delay for one IO READ cycle */
        sysOutWord (iobase + SCB_EEPROM, EE_CS | dataval);
        sysDelay ();    /* delay for one IO READ cycle */
        }

    /* write the location */

    for ( ix = addrBits[unit] - 1; ix >= 0; ix--)
        {
        dataval = (location & (1 << ix)) ? EE_DI : 0;
        sysOutWord (iobase + SCB_EEPROM, EE_CS | dataval);
        sysDelay ();        /* delay for one IO READ cycle */
        sysOutWord (iobase + SCB_EEPROM, EE_CS | dataval | EE_SK);
        sysDelay ();        /* delay for one IO READ cycle */
        sysOutWord (iobase + SCB_EEPROM, EE_CS | dataval);
        sysDelay ();        /* delay for one IO READ cycle */
        dummy = sysInWord (iobase + SCB_EEPROM);
        }

    if ((dummy & EE_DO) == 0)
        {
        ;  /* dummy read */
        }

    /* read the data */

    for ( ix = EE_DATA_BITS - 1; ix >= 0; ix-- )
        {
        sysOutWord (iobase + SCB_EEPROM, EE_CS | EE_SK);

        sysDelay ();        /* delay for one IO READ cycle */

        retval = (retval << 1) |
                 ((sysInWord (iobase + SCB_EEPROM) & EE_DO) ? 1 : 0);

        sysOutWord (iobase + SCB_EEPROM, EE_CS);

        sysDelay ();        /* delay for one IO READ cycle */
        }

    /* disable EEPROM */

    sysOutWord (iobase + SCB_EEPROM, 0x00);

    return(retval);
    }

#if FALSE

/* The following routine is not being used and is therefore commented out */

/******************************************************************************
*
* sys557eepromWrite - write a word to 82557 PCI ethernet device EEPROM
*
* Write a word to 82557 PCI ethernet device EEPROM.
*
* RETURNS: N/A
*
* ERRNO
*
* \NOMANUAL
*/
LOCAL void sys557eepromWrite
    (
    int     unit,           /* END driver unit number */
    int     location,       /* address of word to write */
    UINT16  data            /* the data to write */
    )
    {
    UINT32  iobase = (UINT32)PCI_IO2LOCAL(feiResources[unit].iobaseCsr);

    int i;
    int j;

    /* Enable the EEPROM for writing */

    sysOutWord (iobase + SCB_EEPROM, EE_CS);

    sys557eepromWriteBits (unit, 0x4, EE_CMD_BITS);
    sys557eepromWriteBits (unit, 0x03 << (addrBits[unit] - 2), addrBits[unit]);

    sysOutWord (iobase + SCB_EEPROM, 0);
    sysDelay ();

    /* (1) Activate EEPROM by writing '1' to the EECS bit */

    sysOutWord (iobase + SCB_EEPROM, EE_CS);

    /* (2) Write the WRITE Opcode */

    sys557eepromWriteBits (unit, EE_CMD_WRITE, EE_CMD_BITS);

    /* (3) Write the Address field */

    sys557eepromWriteBits (unit, location, addrBits[unit]);

    /* (4) Write the data field */

    sys557eepromWriteBits (unit, data, EE_DATA_BITS);

    /* (5) Deactivate the EEPROM */

    sysOutWord (iobase + SCB_EEPROM, 0);
    sysDelay ();

    /* delay */

    sysOutWord (iobase + SCB_EEPROM, EE_CS);
    sysDelay ();

    for (i = 0; i < 1000; ++i)
        {
        if (sysInWord (iobase + SCB_EEPROM) & FEI_EEDO)
            break;

        for (j = 0; j < 50; ++j)
            sysDelay ();
        }

    sysOutWord (iobase + SCB_EEPROM, 0);
    sysDelay ();

    /* Disable the write access */

    sysOutWord (iobase + SCB_EEPROM, EE_CS);

    sys557eepromWriteBits (unit, 0x4, EE_CMD_BITS);
    sys557eepromWriteBits (unit, 0, addrBits[unit]);

    sysOutWord (iobase + SCB_EEPROM, 0);
    sysDelay ();
    }

#endif /* FALSE */

#if FALSE

/* The following routine is not being used and is therefore commented out */

/*******************************************************************************
*
* sys557eepromWriteBits - write bits to 82557 PCI ethernet device EEPROM
*
* This routine writes a specified <data> item of <bitlen> length-in-bits
* to the serial EEPROM of an Intel 8255x device associated with an
* END driver <unit> number.
*
* RETURNS: N/A
*
* ERRNO
*
* \NOMANUAL
*/
LOCAL void sys557eepromWriteBits
    (
    int     unit,           /* END driver unit number */
    UINT16  data,           /* the data to write */
    int     bitlen          /* the data length in bits */
    )
    {
    UINT32  iobase = (UINT32)PCI_IO2LOCAL(feiResources[unit].iobaseCsr);
    UINT16  reg;
    UINT16  mask;

    /* write the data, MSB first */

    for (mask = 1 << (bitlen - 1); mask; mask >>= 1)
        {
        /* if mask and data then set the EEDI bit on */

        reg = ((data & mask) ? (EE_CS | EE_DI) : (EE_CS));/*wdz: replaced FEI_EECS and FEI_EEDI by EE_CS and EE_DI*/

        /* (a) write the bit to the EEDI bit */

        sysOutWord (iobase + SCB_EEPROM, reg);
        sysDelay();

        /* (b) write a '1' to the EESK bit then wait the minimum SK high time */

        sysOutWord (iobase + SCB_EEPROM, reg | FEI_EESK);
        sysDelay();

        /* (c) write a '0' to the EESK bit then wait the minimum SK low time */

        sysOutWord (iobase + SCB_EEPROM, reg);
        sysDelay();
        }
    }

#endif /* FALSE */

#if FALSE

/* The following routine is not being used and is therefore commented out */

/*******************************************************************************
*
* sys557eepromChecksumSet - set an Intel 825xx EEPROM checksum
*
* This routine computes and writes the checksum for the serial EEPROM
* on an Intel 82557, 82558, 82559, or 82562 device associated with the
* specified END driver <unit> number.
*
* RETURNS: N/A
*
* ERRNO
*
* \NOMANUAL
*/
LOCAL void sys557eepromChecksumSet
    (
    int    unit           /* END driver unit number */
    )
    {
    int    ix;
    UINT16 sum = 0;

    for (ix = 0; ix < eeSize[unit] - 1; ix++)  /*wdz: changed EE_SIZE to eeSize*/
        {
        sum += sys557eepromRead (unit, ix);
        }

    sys557eepromWrite (unit, eeSize[unit] - 1, (EE_CHECKSUM - sum));  /*wdz: changed EE_SIZE to eeSize*/
    }

#endif /* FALSE */

/***************************************************************************
*
* sys557mdioRead - read MDIO
*
* This routine reads from the MDIO.
*
* RETURNS: read value
*
* ERRNO
*/

LOCAL UINT16 sys557mdioRead
    (
    int unit,           /* unit number */
    int phyId,          /* PHY ID */
    int location        /* location to read */
    )
    {
    UINT32 membase = (UINT32)PCI_MEMIO2LOCAL(feiResources[unit].membaseCsr);
    int timeout   = 64*4;       /* <64 usec. to complete, typ 27 ticks */
    int val;

    sysOutLong (membase + SCB_MDI, 0x08000000 | (location<<16) | (phyId<<21));

    do
        {
        sysDelay ();    /* delay for one IO READ cycle */
        val = sysInLong (membase + SCB_MDI);
        if ( --timeout < 0 )
            printf ("sys557mdioRead() timed out with val = %8.8x.\n", val);
        } while ( ! (val & 0x10000000) );

    return(val & 0xffff);
    }

/***************************************************************************
*
* sys557mdioWrite - write MDIO
*
* This routine writes to the MDIO.
*
* RETURNS: write value
*
* ERRNO
*/

LOCAL UINT16 sys557mdioWrite
    (
    int unit,           /* unit number */
    int phyId,          /* PHY ID */
    int location,       /* location to write */
    int value           /* value to write */
    )
    {
    UINT32 membase = (UINT32)PCI_MEMIO2LOCAL(feiResources[unit].membaseCsr);
    int timeout   = 64*4;       /* <64 usec. to complete, typ 27 ticks */
    int val;

    sysOutLong (membase + SCB_MDI,
                0x04000000 | (location<<16) | (phyId<<21) | value);
    do
        {
        sysDelay ();    /* delay for one IO READ cycle */
        val = sysInLong (membase + SCB_MDI);
        if ( --timeout < 0 )
            printf ("sys557mdioWrite() timed out with val = %8.8x.\n", val);
        } while ( ! (val & 0x10000000) );

    return(val & 0xffff);
    }


/***************************************************************************
*
* sys557LocalToPciBusAdrs - convert a local address to a bus address
*
* This routine returns a PCIbus address for the LOCAL bus address.
*
* RETURNS: PCIbus address
*
* ERRNO
*/

LOCAL UINT32 sys557LocalToPciBusAdrs
    (
    int     unit,
    UINT32      adrs    /* Local Address */
    )
    {
    adrs = (UINT32)LOCAL2PCI_MEM(adrs);
    return(adrs);
    }

/***************************************************************************
*
* sys557PciBusToLocalAdrs - convert a bus address to a local address
*
* This routine returns a local address that is used to access the PCIbus.
* The bus address that is passed into this routine is the PCIbus address
* as it would be seen on the local bus.
*
* RETURNS: local bus address
*
* ERRNO
*/

LOCAL UINT32 sys557PciBusToLocalAdrs
    (
    int     unit,
    UINT32  adrs    /* PCI Address */
    )
    {
    adrs = (UINT32)PCI_MEMIO2LOCAL(adrs);
    return(adrs);
    }

#ifdef FEI_RESOURCE_SHOW
/***************************************************************************
*
* sys557ResoShow - Display saved resource configuration info
*
* This routine displays the resource information about each
* device probed after PCI configuration was completed.
*
* ARGUMENTS:
* sel: select Nth device only (0 = entire list)
* lvl: verbose level (0 = summary only)
*
* Note: the sel argument will show the Nth instance, rather than
* the index.  This is one more than the index.  E.g. to show device
* fei0, use sel=1.  To show device fei1, use sel=2.  Etc.
*
* RETURNS: N/A
*
* ERRNO
*/

void sys557ResoShow
    (
    int sel, 
    int lvl
    )
    {
    int unitNo;
    FEI_RESOURCE * pReso; /* pointer to fei information */

    for ( unitNo = 1 ; unitNo <= feiUnits ; unitNo++ )
        {
        if ( sel == 0 || unitNo == sel )
            {
            pReso = & feiResources [unitNo - 1];

            printf("FEI Unit %d [%d,%d,%d]:\n", unitNo - 1,
                   pReso->pciBus,
                   pReso->pciDevice,
                   pReso->pciFunc);

            printf("\tmembaseCsr   @ 0x%08x\n", pReso->membaseCsr);
            printf("\tiobaseCsr    @ 0x%08x\n", pReso->iobaseCsr);
            printf("\tmembaseFlash @ 0x%08x\n", pReso->membaseFlash);
            printf("\tIRQ          @ 0x%08x\n", pReso->irq);

            if ( lvl > 0 )
                {
                printf("\tconfigType = %d\n", pReso->configType);
                printf("\tboardType = %s\n",
                       (pReso->boardType == TYPE_PRO100B_PCI) ? "Pro 100 B" :
                       (pReso->boardType == TYPE_PRO100S_PCI) ? "Pro 100 S" :
                       (pReso->boardType == TYPE_PRO100_PCI_OTHER) ? "Pro100 ?" :
                       "Unknown");
                printf("\ttimeout = %d\n", pReso->timeout);
                printf("\tmemLength = %d\n", pReso->memLength);
                printf("\tinitialStateMask = 0x%08x\n", pReso->initialStateMask);
                printf("\tinitialState = 0x%08x\n", pReso->initialState);
                printf("\tstatus results[] @ 0x%08x = \n\t\t0x%08x %08x %08x\n"
                       "\t\t0x%08x %08x %08x\n",
                       (int)(&pReso->str[0]),
                       pReso->str[0], pReso->str[1], pReso->str[2],
                       pReso->str[3], pReso->str[4], pReso->str[5]);
                printf("\tpResults = 0x%08x\n", (int)(pReso->pResults));
                printf("\teeprom[] = \n"
                       "\t\t0x%04x %04x %04x %04x 0x%04x %04x %04x %04x\n"
                       "\t\t0x%04x %04x %04x %04x 0x%04x %04x %04x %04x\n",
                       pReso->eeprom[0x00], pReso->eeprom[0x01],
                       pReso->eeprom[0x02], pReso->eeprom[0x03],
                       pReso->eeprom[0x04], pReso->eeprom[0x05],
                       pReso->eeprom[0x06], pReso->eeprom[0x07],
                       pReso->eeprom[0x08], pReso->eeprom[0x09],
                       pReso->eeprom[0x0a], pReso->eeprom[0x0b],
                       pReso->eeprom[0x0c], pReso->eeprom[0x0d],
                       pReso->eeprom[0x0e], pReso->eeprom[0x0f]);
                printf("\t\t0x%04x %04x %04x %04x 0x%04x %04x %04x %04x\n"
                       "\t\t0x%04x %04x %04x %04x 0x%04x %04x %04x %04x\n",
                       pReso->eeprom[0x10], pReso->eeprom[0x11],
                       pReso->eeprom[0x12], pReso->eeprom[0x13],
                       pReso->eeprom[0x14], pReso->eeprom[0x15],
                       pReso->eeprom[0x16], pReso->eeprom[0x17],
                       pReso->eeprom[0x18], pReso->eeprom[0x19],
                       pReso->eeprom[0x1a], pReso->eeprom[0x1b],
                       pReso->eeprom[0x1c], pReso->eeprom[0x1d],
                       pReso->eeprom[0x1e], pReso->eeprom[0x1f]);
                printf("\t\t0x%04x %04x %04x %04x 0x%04x %04x %04x %04x\n"
                       "\t\t0x%04x %04x %04x %04x 0x%04x %04x %04x %04x\n",
                       pReso->eeprom[0x20], pReso->eeprom[0x21],
                       pReso->eeprom[0x22], pReso->eeprom[0x23],
                       pReso->eeprom[0x24], pReso->eeprom[0x25],
                       pReso->eeprom[0x26], pReso->eeprom[0x27],
                       pReso->eeprom[0x28], pReso->eeprom[0x29],
                       pReso->eeprom[0x2a], pReso->eeprom[0x2b],
                       pReso->eeprom[0x2c], pReso->eeprom[0x2d],
                       pReso->eeprom[0x2e], pReso->eeprom[0x2f]);
                printf("\t\t0x%04x %04x %04x %04x 0x%04x %04x %04x %04x\n"
                       "\t\t0x%04x %04x %04x %04x 0x%04x %04x %04x %04x\n",
                       pReso->eeprom[0x30], pReso->eeprom[0x31],
                       pReso->eeprom[0x32], pReso->eeprom[0x33],
                       pReso->eeprom[0x34], pReso->eeprom[0x35],
                       pReso->eeprom[0x36], pReso->eeprom[0x37],
                       pReso->eeprom[0x38], pReso->eeprom[0x39],
                       pReso->eeprom[0x3a], pReso->eeprom[0x3b],
                       pReso->eeprom[0x3c], pReso->eeprom[0x3d],
                       pReso->eeprom[0x3e], pReso->eeprom[0x3f]);
                }
            }
        }
    }
#endif /* FEI_RESOURCE_SHOW */

#endif /* defined (INCLUDE_END) && defined (INCLUDE_FEI82557END) */

