/* lcdLib.c - Lcd driver for amcc_taishan board */

/*
 * Copyright (c) 2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* FAT-FTL Lite Software Development Kit
 * Copyright (C) M-Systems Ltd. 1995-2001       */

/*
modification history
--------------------
01a,01mar07,x_s  Created to support amcc_taishan board lcd driver.
*/

/*
DESCRIPTION
This file implement the lcd driver.
The name of each function in this group begins with "lcd" to represent
"Device Driver Services."
NOMANUAL
*/
#include "stdio.h"   
#include "ibmEvb440gx.h"


#define LCD_DELAY_NORMAL_US 100 
#define LCD_DELAY_NORMAL_MS 2 
#define LCD_CMD_ADDR  ((volatile char *)(CFG_EBC2_LCM_BASE))
#define LCD_DATA_ADDR ((volatile char *)(CFG_EBC2_LCM_BASE+1))
#define LCD_BLK_CTRL  ((volatile char *)(CFG_EBC1_FPGA_BASE+0x2))
UINT32  gLcdInitFlag = 0;
char * amccLogo = "   amcc_taishan     evalboard  ";

static char addrFlag = 0x80;

/***************************************************************************
*
* lcdBlCtrl - write value to the LCD controller
*
* This routine write value to the LCD controller.
*
* RETURNS: N/A
*
* ERRNO
*/
static void lcdBlCtrl
	(
	char val
	)
	{
	char cpld_val;
	cpld_val = *LCD_BLK_CTRL;
	*LCD_BLK_CTRL = val | cpld_val;
	}

/***************************************************************************
*
* lcdPutChar - output a single char to console.
*
* This routine output a single char to console.
*
* RETURNS: N/A
*
* ERRNO
*/
static void lcdPutChar
	(
	char val
	)
	{
	int i = 100;
	char addr;
	
	while (i--)
		{
		if((*LCD_CMD_ADDR & 0x80) != 0x80) 
		{
			sysUsDelay(LCD_DELAY_NORMAL_US);
			break;
		}
		sysUsDelay(LCD_DELAY_NORMAL_US);
		}
	
	if (*LCD_CMD_ADDR & 0x80)
		{
		return;
		}
	addr = *LCD_CMD_ADDR;
	sysUsDelay(LCD_DELAY_NORMAL_US);
	
	if ((addr != 0) && (addr % 0x10 == 0))
		{
		addrFlag ^= 0x40;
		*LCD_CMD_ADDR = addrFlag;
		}
	sysUsDelay(LCD_DELAY_NORMAL_US);
	*LCD_DATA_ADDR = val;
	sysUsDelay(LCD_DELAY_NORMAL_US);
	
	}

/***************************************************************************
*
* lcdPutLogo - output the logo information to console.
*
* This routine output the logo information to console. by call function 
* 
* lcdPutChar().
*
* RETURNS: N/A.
*
* ERRNO
*/		
static void lcdPutLogo(void)
	{
	int i = 100;
	char * p = amccLogo;
	
	while (i--)
		{
		if((*LCD_CMD_ADDR & 0x80) != 0x80) 
			{
			sysUsDelay(LCD_DELAY_NORMAL_US);
			break;
			}
		sysUsDelay(LCD_DELAY_NORMAL_US);
		}
	
	if(*LCD_CMD_ADDR & 0x80)
		{
		return;
		}
	
	*LCD_CMD_ADDR = 0x80;
	
	while (*p)
		lcdPutChar(*p++);

	}

/***************************************************************************
*
* lcdInit - initialize the lcd and display logo of the board
*
* This routine initialize the lcd and display logo information.
*
* RETURNS: N/A
*
* ERRNO
*/
void lcdInit(void)
	{	
	if (gLcdInitFlag == 0)
		{	
		sysUsDelay(100*1000); 
		
		*LCD_CMD_ADDR = 0x38;
		sysUsDelay(LCD_DELAY_NORMAL_US);
		
		*LCD_CMD_ADDR = 0x0f;
		sysUsDelay(LCD_DELAY_NORMAL_US);
	
		*LCD_CMD_ADDR = 0x01;
		sysUsDelay(LCD_DELAY_NORMAL_MS*1000);
		
		*LCD_CMD_ADDR = 0x06;
		sysUsDelay(LCD_DELAY_NORMAL_US);
		
		lcdBlCtrl(0x02);
		lcdPutLogo();

		gLcdInitFlag = 1;
		}
	}

