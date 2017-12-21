/* romSdramInit.c - amcc_taishan DDR SDRAM initialization */

/*
******************************************************************************
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

   COPYRIGHT   I B M   CORPORATION 2002
   LICENSED MATERIAL  -  PROGRAM PROPERTY OF  I B M"

********************************************************************************
\NOMANUAL
*/

/*
 * Copyright (c) 2002-2004, 2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01f,02mar07,x_s  Fix support for boards with MICRON RAM chips
01e,30aug04,mdo  Documentation fixes for apigen
01d,19may04,dtr  Fix diab compiler warnings.
01c,12aug03,jtp  Formal code inspection changes: macro moves, clocking updates.
01b,18jul03,jtp  V850 code clarified
01a,15dec02,saw  written based on 440GP v01a
*/

/*
DESCRIPTION

This file contains functions to initialize the on-chip PPC440GX DDR
SDRAM Controller as applied to the IBM reference board.

This routine makes use of the IIC0 device.

This is relocatable code.  The initial bootrom code is typically loaded
at a different address than it was linked.  The only memory facility
available to these routines is the stack, initialized to the PPC440's
onchip SRAM.  For these reasons, all code in this module must be
position-independent, make sparing use only of auto (stack) variables,
and must avoid use of data and BSS memory and static variables.
Finally, compiler output must be inspected to make sure that no jump
tables with absolute branches have been generated; switch statements and
if-else if-... statements are the leading culprits.

INCLUDE FILES:

*/

/* includes */

#include "vxWorks.h"
#include "config.h"
#include "ppc440gx.h"
#include "sdramDcr.h"

/* defines */

/*
 * The first version of the amcc_taishan board does not support the V850, so
 * avoid programming it.
 */

#undef PROGRAM_V850

#define SPIN     { \
                 volatile int temp= 0; \
         for(;;temp++); \
                 }

#define DDR_SDRAM0_SIZE		0x10000000
#define DDR_BXCR_MAXBANKS	4    

#define	DDR_SDRAM0_CFG0		0x06200000
#define	DDR_SDRAM0_CFG1		0x00000000
#define	DDR_SDRAM0_RTR		0x04E00000
#define	DDR_SDRAM0_TR0		0xC10A401A
#define	DDR_SDRAM0_B0CR		0x000c4001	
#define	DDR_SDRAM0_CLKTR	0x40000000
#define	DDR_SDRAM0_TR1		0x80800819
/* externals */

IMPORT void sdramDcrSet (int registerAdrs, UINT32 data);
IMPORT UINT32   sdramDcrGet (int registerAdrs);
IMPORT void sdramResetDelay (void);
IMPORT UINT32   sysCprInLong(UINT32);
IMPORT UINT32   sysClkPeriodGet(UINT32);

IMPORT void sdrDcrSet (int registerAdrs, UINT32 data);
IMPORT UINT32   sdrDcrGet (int registerAdrs);


/*****************************************************************************
*
* romSdramInit - Initializes the SDRAM controller
*
* Configures SDRAM memory banks for fixed SDRAM device. If auto memory
* configuration is selected at compile time, romSdramInit reads the
* SDRAM EEPROM via the IIC bus and then configures the SDRAM memory
* banks appropriately.
*
* RETURNS: N/A
*
* ERRNO
*/
void romSdramInit(void)
    {
    UINT32 temp;
    UINT32 bxcr_num; 
	
	sdrDcrSet(SDR0_SRST, SDR0_SRST_DMC);
	sdrDcrSet(SDR0_SRST, 0x00000000);
	
  
    /* set upper 4 bits of the PLB base addr */
    sdramDcrSet(SDRAM0_UABBA,
    sdramDcrGet(SDRAM0_UABBA) & ~SDRAM_UABBA_ADDR);

    /* Set the PLB slave interface options  */
    sdramDcrSet(SDRAM0_SLIO,
    sdramDcrGet(SDRAM0_SLIO) &
        ~(SDRAM_SLIO_RD_RE | SDRAM_SLIO_WR_RE | SDRAM_SLIO_RARW));

    /* Set the Device Options register */
    sdramDcrSet(SDRAM0_DEVOPT,
    sdramDcrGet(SDRAM0_DEVOPT) & ~(SDRAM_DEVOPT_DLL | SDRAM_DEVOPT_DS));
	

    /* Set the SDRAM Write Data/DM/DQS Clock Timing Reg */
    temp = sdramDcrGet(SDRAM0_WDDCTR) &
    ~(SDRAM_WDDCTR_WRCP | SDRAM_WDDCTR_DCD);
    temp |= (SDRAM_WDDCTR_WRCP_0DEG | SDRAM_WDDCTR_DCD_ENCODE(0));
    sdramDcrSet(SDRAM0_WDDCTR, temp);
 
 
	/* program 440GP SDRAM controller options (SDRAM0_CFG0) */
	sdramDcrSet(SDRAM0_CFG0, DDR_SDRAM0_CFG0);

	/* program 440GP SDRAM controller options (SDRAM0_CFG1) */
	sdramDcrSet(SDRAM0_CFG1, DDR_SDRAM0_CFG1);
	
 	
	/* program SDRAM refresh register (SDRAM0_RTR) */
	sdramDcrSet(SDRAM0_RTR, DDR_SDRAM0_RTR);
	
	/* program SDRAM Timing Register 0 (SDRAM0_TR0) */
	sdramDcrSet(SDRAM0_TR0, DDR_SDRAM0_TR0);

		/* program the BxCR registers */
	for(bxcr_num=0; bxcr_num < DDR_BXCR_MAXBANKS; bxcr_num++)
		{
		sdramDcrSet(SDRAM0_B0CR + (bxcr_num << 2), 0x00000000);
		}
	
	sdramDcrSet(SDRAM0_B0CR,DDR_SDRAM0_B0CR);
 
	/* program SDRAM Clock Timing Register (SDRAM0_CLKTR) */
	sdramDcrSet(SDRAM0_CLKTR, DDR_SDRAM0_CLKTR);
	
  	sdramResetDelay();


	/* enable the memory controller */
	sdramDcrSet(SDRAM0_CFG0, sdramDcrGet(SDRAM0_CFG0)| 0x80000000);
 	/* program SDRAM Timing Register 1, adding some delays */
	sdramDcrSet(SDRAM0_TR1, DDR_SDRAM0_TR1);
    }
 
