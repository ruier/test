/* romSdramInit.c - amcc440ep DDR SDRAM initialization */

/*
*****************************************************************************
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
   LICENSED MATERIAL  -  PROGRAM PROPERTY OF  I B M"


********************************************************************************
\NOMANUAL
*/

/* Copyright 2006 Wind River Systems, Inc.  All rights reserved. */

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
01a,10mar06,mmi  written based on bamboo romSdramInit.c version 01h

*/

/*
DESCRIPTION

This file contains functions to initialize the on-chip PPC440EP DDR
SDRAM Controller as applied to the amcc440ep reference board.

Since the only memory facility available to these routines is the stack
(initialized to the Data cache), this module is written to
avoid use of any data or BSS memory.

*/

/* includes */

#include "vxWorks.h"
#include "config.h"
#include "ppc440EP.h"
#include "sdramDcr.h"
#include "sysLib.h"

/* defines */

/* board configuration read support */

/* externals */

IMPORT void sdramDcrSet (int registerAdrs, UINT32 data);
IMPORT UINT32 sdramDcrGet (int registerAdrs);
IMPORT void sdramResetDelay (void);


/*****************************************************************************
*
* romSdramInit - Initializes the 440EP SDRAM controller
*
* Configures SDRAM memory banks for fixed SDRAM device. Autoconfiguration not
* supported 
*
* RETURNS: void
*
* ERRNO
*/
void romSdramInit(void)
{
  UINT32  temp;

  /* set upper 4 bits of the PLB base addr */

  sdramDcrSet(SDRAM0_UABBA, 0x00000000);

  /* Set the PLB slave interface options  */

  sdramDcrSet(SDRAM0_SLIO,0x00000000);

  /* Set the Device Options register */

  sdramDcrSet(SDRAM0_DEVOPT,0x00000000);

  sdramDcrSet(SDRAM0_CLKTR, SDRAM_CLKTR_CLKP_90DEG);

  /* Set the SDRAM Write Data/DM/DQS Clock Timing Reg */

  sdramDcrSet(SDRAM0_WDDCTR, SDRAM_WDDCTR_WRCP_90DEG);

  /* Clear cfg register */

  sdramDcrSet (SDRAM0_CFG0, 0x00000000);

  /* bank 0 starts at zero  */

  sdramDcrSet (SDRAM0_B0CR, SDRAM_BXCR_SDSZ_128 | SDRAM_BXCR_SDAM_3 | 
                            SDRAM_BXCR_SDBE);
  /* bank 1: base address 0x08000000 */

  sdramDcrSet (SDRAM0_B1CR, 0x08000000 | SDRAM_BXCR_SDSZ_128 | 
                            SDRAM_BXCR_SDAM_3 );

  sdramDcrSet (SDRAM0_TR0, SDRAM_TR0_SDWD_1_CLK | SDRAM_TR0_SDCL_2_5_CLK |
                           SDRAM_TR0_SDPA_3_CLK | SDRAM_TR0_SDCP_4_CLK | 
                           SDRAM_TR0_SDLD_2_CLK   |   SDRAM_TR0_SDRA_10_CLK |
                           SDRAM_TR0_SDRD_3_CLK );    

  sdramDcrSet (SDRAM0_TR1, SDRAM_TR1_RDSS_TR2 | SDRAM_TR1_RDSL_STAGE3 |
                           SDRAM_TR1_RDCD_RCD_1_2 | SDRAM_TR1_RDCT_ENCODE (11));

  sdramDcrSet (SDRAM0_RTR, SDRAM_RTR_RINT_ENCODE (0x408));

  sdramDcrSet (SDRAM0_CFG1, 0x00000000);

  sdramDcrSet (SDRAM0_CFG0, SDRAM_CFG0_PMUD);

  sdramResetDelay();

  sdramDcrSet (SDRAM0_CFG0, SDRAM_CFG0_DCEN | SDRAM_CFG0_PMUD);

  for (;;) {
	temp = sdramDcrGet (SDRAM0_MCSTS);
	if (temp & SDRAM_MCSTS_MRSC)
		break;
	}

}

