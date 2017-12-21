/* zmiiLib.c - IBM ZMII macro driver */

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

   COPYRIGHT   I B M   CORPORATION 2003
   LICENSED MATERIAL  -  PROGRAM PROPERTY OF  I B M

********************************************************************************
\NOMANUAL
*/

/* Copyright 2006 Wind River Systems, Inc. */

/*
modification history
--------------------
01g,05apr06,mmi  add support functions for drv/end/emacEnd.c generic driver 
01f,02nov04,dr   additional documentation fixes for apigen 
01e,07sep04,mdo  documentation fixes for apigen
01d,11jul02,pch  SPR 77845: fix name collision with PCI drivers
01c,15nov01,pch  handle fewer than 4 EMAC units
01b,27sep01,ajm  moved RMII/SMII/MII mode determination to external callout fnct
01a,05mar01,ajm  created
*/
/*
DESCRIPTION

This driver is for the IBM ZMII macro, (Ethernet Media Access Controller).
This controller is a peripheral available from the IBM Blue Logic ASIC core
library.  The ZMII bridge connects Ethernet media access controllers
(EMACs) to standard physical devices (PHYs).  Media independent interface
(MII) and management data interface (MDI) signals to and from an EMAC are
passed to the ZMII bridge.  The ZMII bridge passes MII and MDI signals through
to the PHY, or formats the MII signals to support the reduced media
independent interface (RMII) or serial media independent interface (SMII).

ZMII provides one of the following off-chip interfaces:  up to two MII, four
RMII, or four SMII.  The interfaces are mutually exclusive.  Initial interface
selection is controlled by pin strapping.

Interface selection is controlled by the Function Enable Register (ZMII0_FER).

RMII and SMII significantly reduce the external signal count required to
support multiple EMAC ports.  The interfaces share two management data
interface (MDI) signals (EMCMDIO and EMCMDCLK).  While the MII requires 16
additional pins for a single interface, RMII requires only seven additional
pins (six data and control and a common clock) and SMII requires only four
additional pins (two data, sync, and a clock).  Configuring the ZMII bridge
for SMII or RMII allows the use of both EMACs using a reduced number of
external pins.
*/


#include "zmiiLib.h"
#include "drv/end/emacEnd.h"

/* imports */
IMPORT UINT sysPlbInLong  (ULONG);
IMPORT void sysPlbOutLong (ULONG address, UINT data);

/* Global -- Function callback to determine which mode (MII/RMII/SMII) is
 *           set by eval board hardware.
 */
ZMII_MODE   (*_func_zmiiReadMode) (VOID) = NULL;

/* Macros for I/O functions */
#ifndef ZMII_REG_WRITE
#define ZMII_REG_WRITE(addr,value)  sysPlbOutLong((addr),(value))
#endif /* ZMII_REG_WRITE */

#ifndef ZMII_REG_READ
#define ZMII_REG_READ(addr) sysPlbInLong((addr))
#endif /* ZMII_REG_READ */

/*******************************************************************************
* zmiiSetInterface - Set ZMII logic for correct MII/RMII/SMII connection.
*
* Bootstrap pins determine power-up mode, but these values are not
* accessible by software.
*
* All ethernets must use the same mode or be disabled.
* Different speeds are selectable, however.
*
* Only one MDI interface can be active at a time.  This routine will switch
* MDI interface to the last emac that was called.
*
* If moved to do more than init, this code should be guarded by a semaphore
* DO NOT OVERWRITE PREVIOUSLY SAVED VALUES
*
* RETURNS: 0 if successful, -1 on error
* Note:  ZMII must enable EMAC before a reset is given, otherwise macro never
*        comes out of reset.
*
* ERRNO
*/
STATUS zmiiSetInterface
  (
  UINT32 emacbaseAdrs
  )
  {
  unsigned long zmii_fer;
  ZMII_MODE mode;
  static int first = 1;


  if (_func_zmiiReadMode)             /* If Mode callout function defined    */
    mode = (_func_zmiiReadMode)();    /*  call it -- Implementation Specific */
  else
    mode = ZMII_RMII_MODE;            /* Default to RMII */

  if (first)
    {
    first = 0;
    zmii_fer = 0;       /* Will overwrite any other mode for other EMACs */
    }
  else
    zmii_fer = ZMII_REG_READ(ZMII0_FER);  /* All EMACs modes must match */

  switch (emacbaseAdrs)
    {
#ifdef  EMAC0_BASE
    case EMAC0_BASE:
       /*
       * EMAC0 (do not touch other EMACs)
       */
       switch (mode)
         {
         case ZMII_MII_MODE:
           if (zmii_fer & (ZMII_FER_RMII0 | ZMII_FER_SMII0 |
                           ZMII_FER_RMII1 | ZMII_FER_SMII1 | ZMII_FER_MII1 |
                           ZMII_FER_RMII2 | ZMII_FER_SMII2 |
                           ZMII_FER_RMII3 | ZMII_FER_SMII3 ))

             return ERROR;
           ZMII_REG_WRITE(ZMII0_FER, (zmii_fer & ~ZMII_FER_EMAC0_MASK) |
                                  ZMII_FER_MDI0 | ZMII_FER_MII0);
           ZMII_REG_WRITE(ZMII0_SSR, 0x0);
           break;
         case ZMII_SMII_MODE:
           if (zmii_fer & (ZMII_FER_RMII0 | ZMII_FER_MII0  |
                           ZMII_FER_RMII1 | ZMII_FER_MII1  |
                           ZMII_FER_RMII2 | ZMII_FER_MII2  |
                           ZMII_FER_RMII3 | ZMII_FER_MII3))
             return ERROR;
           ZMII_REG_WRITE(ZMII0_FER, (zmii_fer & ~ZMII_FER_EMAC0_MASK) |
                                  ZMII_FER_MDI0 | ZMII_FER_SMII0);
           ZMII_REG_WRITE(ZMII0_SSR, 0x0);
           break;
         case ZMII_RMII_MODE:
           if (zmii_fer & (ZMII_FER_SMII0 | ZMII_FER_MII0  |
                           ZMII_FER_SMII1 | ZMII_FER_MII1  |
                           ZMII_FER_SMII2 | ZMII_FER_MII2  |
                           ZMII_FER_SMII3 | ZMII_FER_MII3))
             return ERROR;
           ZMII_REG_WRITE(ZMII0_FER, (zmii_fer & ~ZMII_FER_EMAC0_MASK) |
                                  ZMII_FER_MDI0 |ZMII_FER_RMII0);

           break;
         default:
           return ERROR;
         }
       break;
#endif  /* EMAC0_BASE */
#ifdef  EMAC1_BASE
    case EMAC1_BASE:
       /*
       * EMAC1 (do not touch other EMACs)
       */
       switch (mode)
         {
         case ZMII_MII_MODE:
           if (zmii_fer & (ZMII_FER_RMII0 | ZMII_FER_SMII0 | ZMII_FER_MII0 |
                           ZMII_FER_RMII1 | ZMII_FER_SMII1 |
                           ZMII_FER_RMII2 | ZMII_FER_SMII2 |
                           ZMII_FER_RMII3 | ZMII_FER_SMII3 ))

             return ERROR;
           ZMII_REG_WRITE(ZMII0_FER, (zmii_fer & ~ZMII_FER_EMAC1_MASK) |
                                  ZMII_FER_MDI1 | ZMII_FER_MII1);
           ZMII_REG_WRITE(ZMII0_SSR, 0x0);
           break;
         case ZMII_SMII_MODE:
           if (zmii_fer & (ZMII_FER_RMII0 | ZMII_FER_MII0  |
                           ZMII_FER_RMII1 | ZMII_FER_MII1  |
                           ZMII_FER_RMII2 | ZMII_FER_MII2  |
                           ZMII_FER_RMII3 | ZMII_FER_MII3))
             return ERROR;
           ZMII_REG_WRITE(ZMII0_FER, (zmii_fer & ~ZMII_FER_EMAC1_MASK) |
                                  ZMII_FER_MDI1 | ZMII_FER_SMII1);
           ZMII_REG_WRITE(ZMII0_SSR, 0x0);
           break;
         case ZMII_RMII_MODE:
           if (zmii_fer & (ZMII_FER_SMII0 | ZMII_FER_MII0  |
                           ZMII_FER_SMII1 | ZMII_FER_MII1  |
                           ZMII_FER_SMII2 | ZMII_FER_MII2  |
                           ZMII_FER_SMII3 | ZMII_FER_MII3))
             return ERROR;
           ZMII_REG_WRITE(ZMII0_FER, (zmii_fer & ~ZMII_FER_EMAC1_MASK) |
                                  ZMII_FER_MDI1 |ZMII_FER_RMII1);

           break;
         default:
           return ERROR;
         }
       break;
#endif  /* EMAC1_BASE */
#ifdef  EMAC2_BASE
    case EMAC2_BASE:
       /*
       * EMAC2 (do not touch other EMACs)
       */
       switch (mode)
         {
         case ZMII_MII_MODE:
           if (zmii_fer & (ZMII_FER_RMII0 | ZMII_FER_SMII0 |
                           ZMII_FER_RMII1 | ZMII_FER_SMII1 |
                           ZMII_FER_RMII2 | ZMII_FER_SMII2 |
                           ZMII_FER_RMII3 | ZMII_FER_SMII3 | ZMII_FER_MII3))

             return ERROR;
           ZMII_REG_WRITE(ZMII0_FER, (zmii_fer & ~ZMII_FER_EMAC2_MASK) |
                                  ZMII_FER_MDI2 | ZMII_FER_MII2);
           ZMII_REG_WRITE(ZMII0_SSR, 0x0);
           break;
         case ZMII_SMII_MODE:
           if (zmii_fer & (ZMII_FER_RMII0 | ZMII_FER_MII0  |
                           ZMII_FER_RMII1 | ZMII_FER_MII1  |
                           ZMII_FER_RMII2 | ZMII_FER_MII2  |
                           ZMII_FER_RMII3 | ZMII_FER_MII3))
             return ERROR;
           ZMII_REG_WRITE(ZMII0_FER, (zmii_fer & ~ZMII_FER_EMAC2_MASK) |
                                  ZMII_FER_MDI2 | ZMII_FER_SMII2);
           ZMII_REG_WRITE(ZMII0_SSR, 0x0);
           break;
         case ZMII_RMII_MODE:
           if (zmii_fer & (ZMII_FER_SMII0 | ZMII_FER_MII0  |
                           ZMII_FER_SMII1 | ZMII_FER_MII1  |
                           ZMII_FER_SMII2 | ZMII_FER_MII2  |
                           ZMII_FER_SMII3 | ZMII_FER_MII3))
             return ERROR;
           ZMII_REG_WRITE(ZMII0_FER, (zmii_fer & ~ZMII_FER_EMAC2_MASK) |
                                  ZMII_FER_MDI2 |ZMII_FER_RMII2);

           break;
         default:
           return ERROR;
         }
       break;
#endif  /* EMAC2_BASE */
#ifdef  EMAC3_BASE
    case EMAC3_BASE:
       /*
       * EMAC3 (do not touch other EMACs)
       */
       switch (mode)
         {
         case ZMII_MII_MODE:
           if (zmii_fer & (ZMII_FER_RMII0 | ZMII_FER_SMII0 |
                           ZMII_FER_RMII1 | ZMII_FER_SMII1 |
                           ZMII_FER_RMII2 | ZMII_FER_SMII2 | ZMII_FER_MII2 |
                           ZMII_FER_RMII3 | ZMII_FER_SMII3 ))

             return ERROR;
           ZMII_REG_WRITE(ZMII0_FER, (zmii_fer & ~ZMII_FER_EMAC3_MASK) |
                                  ZMII_FER_MDI3 | ZMII_FER_MII3);
           ZMII_REG_WRITE(ZMII0_SSR, 0x0);
           break;
         case ZMII_SMII_MODE:
           if (zmii_fer & (ZMII_FER_RMII0 | ZMII_FER_MII0  |
                           ZMII_FER_RMII1 | ZMII_FER_MII1  |
                           ZMII_FER_RMII2 | ZMII_FER_MII2  |
                           ZMII_FER_RMII3 | ZMII_FER_MII3))
             return ERROR;
           ZMII_REG_WRITE(ZMII0_FER, (zmii_fer & ~ZMII_FER_EMAC3_MASK) |
                                  ZMII_FER_MDI3 | ZMII_FER_SMII3);
           ZMII_REG_WRITE(ZMII0_SSR, 0x0);
           break;
         case ZMII_RMII_MODE:
           if (zmii_fer & (ZMII_FER_SMII0 | ZMII_FER_MII0  |
                           ZMII_FER_SMII1 | ZMII_FER_MII1  |
                           ZMII_FER_SMII2 | ZMII_FER_MII2  |
                           ZMII_FER_SMII3 | ZMII_FER_MII3))
             return ERROR;
           ZMII_REG_WRITE(ZMII0_FER, (zmii_fer & ~ZMII_FER_EMAC3_MASK) |
                                  ZMII_FER_MDI3 |ZMII_FER_RMII3);

           break;
         default:
           return ERROR;
         }
       break;
#endif  /* EMAC3_BASE */
    default:
       return ERROR;
    }
  return OK;
  }


/******************************************************************************
*
* zmiiSetSpeed - set the ZMII_SSR register for proper EMAC clocking
*
* Speed selection is required for RMII mode, optional for SMII mode,
* and ignored for MII mode.
*
* This function resets a mask for the given EMACs speed bit, and then sets
* the mask bit if the speed parameter is 100Mbps.
*
* RETURNS: N/A
*
* ERRNO
*/
void  zmiiSetSpeed
    (
    UINT32 emacbaseAdrs, 
    UINT speed
    )
    {
    UINT32 zmii_ssr;

    zmii_ssr = ZMII_REG_READ(ZMII0_SSR);
    switch (emacbaseAdrs)
      {
#ifdef  EMAC0_BASE
      case EMAC0_BASE:
        zmii_ssr = zmii_ssr & ~ZMII_SSR_EMAC0_MASK;
        if (speed == EMAC_SPEED_100Mbps)
          zmii_ssr |= ZMII_SSR_SP0_100Mbps;
        break;
#endif  /* EMAC0_BASE */
#ifdef  EMAC1_BASE
      case EMAC1_BASE:
        zmii_ssr = zmii_ssr & ~ZMII_SSR_EMAC1_MASK;
        if (speed == EMAC_SPEED_100Mbps)
          zmii_ssr |= ZMII_SSR_SP1_100Mbps;
        break;
#endif  /* EMAC1_BASE */
#ifdef  EMAC2_BASE
      case EMAC2_BASE:
        zmii_ssr = zmii_ssr & ~ZMII_SSR_EMAC2_MASK;
        if (speed == EMAC_SPEED_100Mbps)
          zmii_ssr |= ZMII_SSR_SP2_100Mbps;
        break;
#endif  /* EMAC2_BASE */
#ifdef  EMAC3_BASE
      case EMAC3_BASE:
        zmii_ssr = zmii_ssr & ~ZMII_SSR_EMAC3_MASK;
        if (speed == EMAC_SPEED_100Mbps)
          zmii_ssr |= ZMII_SSR_SP3_100Mbps;
        break;
#endif  /* EMAC3_BASE */
      default:
        for(;;);
      }
    ZMII_REG_WRITE(ZMII0_SSR, zmii_ssr);
}


/******************************************************************************
*
* zmiiSetMDI - set the ZMII_FER[MDI] interface for proper PHY communication
*
* Note:  Only one EMAC MDI interface can be hooked up at a given time.  All
*        others will be reset.
*
* RETURNS: N/A
*
* ERRNO
*/
void zmiiSetMDI
  (
  UINT32 emacbaseAdrs
  )
  {

  UINT32 fer;

  /* Get original FER to not change other EMAC settings */
  fer = ZMII_REG_READ(ZMII0_FER);

  /*
  * Must set proper ZMII0_FER[MDI] for correct EMAC (0-3)
  * before we can communicate with PHY.
  * Make sure not to corrupt the rest of the ZMII settings
  * from previous instance call
  */
  if (emacbaseAdrs == EMAC0_BASE)
    ZMII_REG_WRITE(ZMII0_FER,ZMII_FER_MDI0 | (fer & ~ZMII_FER_MDI1 &
                                                ~ZMII_FER_MDI2 & ~ZMII_FER_MDI3));
#ifdef  EMAC1_BASE
  else if (emacbaseAdrs == EMAC1_BASE)
    ZMII_REG_WRITE(ZMII0_FER,ZMII_FER_MDI1 | (fer & ~ZMII_FER_MDI0 &
                                                ~ZMII_FER_MDI2 & ~ZMII_FER_MDI3));
#endif  /* EMAC1_BASE */
#ifdef  EMAC2_BASE
  else if (emacbaseAdrs == EMAC2_BASE)
    ZMII_REG_WRITE(ZMII0_FER,ZMII_FER_MDI2 | (fer & ~ZMII_FER_MDI0 &
                                                ~ZMII_FER_MDI1 & ~ZMII_FER_MDI3));
#endif  /* EMAC2_BASE */
#ifdef  EMAC3_BASE
  else if (emacbaseAdrs == EMAC3_BASE)
    ZMII_REG_WRITE(ZMII0_FER,ZMII_FER_MDI3 | (fer & ~ZMII_FER_MDI0 &
                                                  ~ZMII_FER_MDI1 & ~ZMII_FER_MDI2));
#endif  /* EMAC3_BASE */
}

/******************************************************************
* SetInterfaceByDrv - zmii interface initialization
*
* This function is called by the emac device driver to initialize
* the zmii bridge.
*
* RETURN: OK or ERROR
*/

STATUS zmiiSetInterfaceByDrv
     (
     EMAC_DRV_CTRL * pDrvCtrl
     )
     {
     return (zmiiSetInterface (pDrvCtrl->baseAdrs));
     }

/******************************************************************
* zmiiSetMDIByDrv - setup zmii's MDI 
*
* This function is called by the emac device driver to set up MDI 
* for zmii bridge.
*
* RETURN: N/A
*/

void zmiiSetMDIByDrv
     (
     EMAC_DRV_CTRL * pDrvCtrl
     )
     {
     zmiiSetMDI (pDrvCtrl->baseAdrs);
     }

/******************************************************************
* zmiiSetSpeedByDrv - configure speed in the zmii bridge 
*
* This function is called by the emac device driver to configure speed 
* for the zmii bridge.
*
* RETURN: N/A
*/

void zmiiSetSpeedByDrv
     (
     EMAC_DRV_CTRL * pDrvCtrl
     )
     {
     zmiiSetSpeed (pDrvCtrl->baseAdrs, pDrvCtrl->phySpeed);
     }

