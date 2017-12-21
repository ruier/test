/* iicLib.c - IIC routines */

/*
********************************************************************************
   This source and object code has been made available to you by IBM on an
   AS-IS basis.

   IT IS PROVIDED WITHOUT WARRANTY OF ANY KIND, INCLUDING THE WARRANTIES OF
   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE OR OF NONINFRINGEMENT
   OF THIRD PARTY RIGHTS.  IN NO EVENT SHALL IBM OR ITS LICENSORS BE LIABLE
   FOR INCIDENTAL, CONSEQUENTIAL OR PUNITIVE DAMAGES.  IBMÆS OR ITS LICENSORÆS
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

/*
 * Copyright (c) 2002, 2004, 2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
#include "copyright_wrs.h"

/*
modification history
--------------------
01d,25mar07,x_s  Add IIC init routines
01c,30aug04,mdo  Documentation fixes for apigen
01b,12may04,j_b  fix compiler warnings
01a,15dec02,saw  created
*/

/*
DESCRIPTION
This library contains IIC read/write routines.
*/

/* includes */

#include "ibmEvb440gx.h"

#define IIC_OK		0
#define IIC_NOK		1
#define IIC_NOK_LA	2		/* Lost arbitration */
#define IIC_NOK_ICT	3		/* Incomplete transfer */
#define IIC_NOK_XFRA	4		/* Transfer aborted */
#define IIC_NOK_DATA	5		/* No data in buffer */
#define IIC_NOK_TOUT	6		/* Transfer timeout */

UCHAR bootStrapBuf[16] =
	{
		0x86,
		0x78,
		0xc1,
		0xa6,
		0x09,
		0x67,
		0x04,
		0x63,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00
	};

/***************************************************************************
*
* sysiicOutByte - write value to io space
*
* This routine write value to io space by call function sysOutByte.
*
* RETURNS : N/A
*
* ERRNO
*/
static void sysiicOutByte
	(
	ULONG address, 
	UCHAR data
	)
	{
	ULONG add;
	
	add = IIC0_BASE + address;
	sysOutByte(add, data);
	}

/***************************************************************************
*
* sysiicInByte - read value from io space
*
* This routine read value from io space by call from sysInByte
*
* RETURNS : N/A
*
* ERRNO
*/
static UCHAR sysiicInByte
	(
	ULONG address
	)
	{
	ULONG add;
	
	add = IIC0_BASE + address;
	return (sysInByte(add));
	}

/***************************************************************************
*
* iicBusReset - reset the iic bus
*
* This routine reset the iic bus by call function sysiicOutByte.
*
* RETURNS : N/A
*
* ERRNO
*/
static void iicBusReset (void)
	{
	int i, status;

	sysiicOutByte (IIC_STS, 0x0A);

	sysiicOutByte (IIC_EXTSTS, 0x8F);
	EIEIO;
	
	i = 10;
	do 
	{
		status = sysiicInByte (IIC_STS);
		taskDelay (1);			
		i--;
	} while ((status & IIC_STS_PT) && (i > 0));

	status = sysiicInByte (IIC_XTCNTLSS);
	sysiicOutByte (IIC_XTCNTLSS, (status | IIC_XTCNTLSS_SRST));
	EIEIO;

	sysiicOutByte (IIC_DIRECTCNTL, 0xC);
	for (i = 0; i < 10; i++) 
		{
		if ((sysiicInByte (IIC_DIRECTCNTL) & 0x3) != 0x3) 
			{

			sysiicOutByte (IIC_DIRECTCNTL, 0x8);
			taskDelay (1);		
			sysiicOutByte (IIC_DIRECTCNTL, 0xC);	
			taskDelay (1);		
			}	 
		else 
			{
			break;
			}
		}
	
	sysiicOutByte (IIC_DIRECTCNTL, 0x4);
	taskDelay (1);				

	sysiicOutByte (IIC_DIRECTCNTL, 0xC);
	taskDelay (1);				

	sysiicOutByte (IIC_XTCNTLSS, (status & ~IIC_XTCNTLSS_SRST));
	taskDelay (1);				
	}	

/***************************************************************************
*
* iicInit - the iic controller initialization
*
* This routine init the iic controller. It is finished by the routine 
* sysiicOutByte().
*
* RETURNS : N/A
*
* ERRNO
*/
void iicInit 
	(
	int speed
	)
	{
	SYS_INFO    sysInfo;
	unsigned long freqOPB;
	int val, divisor;

	iicBusReset();

	sysiicOutByte (IIC_LMADR, 0);

	sysiicOutByte (IIC_HMADR, 0);

	sysiicOutByte (IIC_LSADR, 0);

	sysiicOutByte (IIC_HSADR, 0);

	sysInfoGet (&sysInfo);
	freqOPB = sysInfo.freqPLB / sysInfo.pllOpbDiv;

	divisor = (freqOPB - 1) / 10000000;
	if (divisor == 0)
		divisor = 1;
	sysiicOutByte (IIC_CLKDIV, divisor);

	sysiicOutByte (IIC_INTRMSK, 0);

	sysiicOutByte (IIC_XFRCNT, 0);

	sysiicOutByte (IIC_XTCNTLSS, 0xF0);

	sysiicOutByte (IIC_MDCNTL, IIC_MDCNTL_FSDB | IIC_MDCNTL_FMDB);
	EIEIO;


	val = sysiicInByte(IIC_MDCNTL);
	EIEIO;


	val |= IIC_MDCNTL_EUBS|IIC_MDCNTL_HSCL;
	if (speed >= 400000)
		{
		val |= IIC_MDCNTL_FSM;
		}
	
	sysiicOutByte (IIC_MDCNTL, val);

	sysiicOutByte (IIC_CNTL, 0x00);
	EIEIO;

	}
/***************************************************************************
*
* iicTransfer - construct data to transfer to the eeprom
*
* This routine will transfer up to 4 bytes in one pass
* on the loop. It only does sysiicOutByte(lbz) to the buffer when it
* is possible to do out16(lhz) transfers.
*
* RETURNS : OK OR ERROR
*
* ERRNO
*/
static int iicTransfer
	(
	UCHAR cmd_type,
	UCHAR chip,
	UCHAR addr[],
	UCHAR addr_len,
	UCHAR data[],
	UCHAR data_len 
	)
	{
	UCHAR* ptr;
	int reading;
	int tran,cnt;
	int result;
	int status;
	int i;
	char creg;

	if (data == 0 || data_len == 0)
		{
		return IIC_NOK;
		}
	
	if (addr && addr_len)
		{
		ptr = addr;
		cnt = addr_len;
		reading = 0;
		}
	else
		{
		ptr = data;
		cnt = data_len;
		reading = cmd_type;
		}

	sysiicOutByte (IIC_STS,IIC_STS_SCMP);
	
	i=10;
	do 
	{
		status = sysiicInByte (IIC_STS);
		EIEIO;
		i--;
	} while ((status & IIC_STS_PT) && (i>0));

	if (status & IIC_STS_PT) 
		{
		result = IIC_NOK_TOUT;
		return(result);
		}
	
	sysiicOutByte(IIC_MDCNTL, ((sysiicInByte(IIC_MDCNTL))|IIC_MDCNTL_FMDB|IIC_MDCNTL_FSDB));
	
	sysiicOutByte(IIC_HMADR,0);
	sysiicOutByte(IIC_LMADR, chip);
	EIEIO;

	tran = 0;
	result = IIC_OK;
	creg = 0;

	while ( tran != cnt && (result == IIC_OK)) 
		{
		int  bc,j;

		creg |= IIC_CNTL_PT;

		bc = (cnt - tran) > 4 ? 4 : cnt - tran;
		creg |= (bc-1)<<4;

		if ((!cmd_type && (ptr == addr)) || ((tran+bc) != cnt))
			creg |= IIC_CNTL_CHT;

		if (reading)
			creg |= IIC_CNTL_READ;
		else 
			{
			for (j=0; j<bc; j++) 
				{

				sysiicOutByte (IIC_MDBUF,ptr[tran+j]);
				EIEIO;
				}
			}
		sysiicOutByte (IIC_CNTL, creg);
		EIEIO;

		i=2*5*8;
		do 
		{
	
			status = sysiicInByte(IIC_STS);
			EIEIO;
			taskDelay (1);
			i--;
		} while ((status & IIC_STS_PT) && !(status & IIC_STS_ERR) && (i>0));

		if (status & IIC_STS_ERR) 
			{
			result = IIC_NOK;
			status = sysiicInByte (IIC_EXTSTS);

			if (status & IIC_EXTSTS_LA)
				result = IIC_NOK_LA;

			if (status & IIC_EXTSTS_ICT)
				result = IIC_NOK_ICT;
	
			if (status & IIC_EXTSTS_XFRA)
				result = IIC_NOK_XFRA;
			} 
		else if ( status & IIC_STS_PT) 
			{
			result = IIC_NOK_TOUT;
			}

		if ((reading) && (result == IIC_OK)) 
			{
	
			if (status & IIC_STS_MDBS) 
				{
				taskDelay (1);
				for(j=0;j<bc;j++) 
					{
					ptr[tran+j] = sysiicInByte(IIC_MDBUF);
					EIEIO;
					}
				} 
			else
				result = IIC_NOK_DATA;
			}
		creg = 0;
		tran+=bc;
		if (ptr == addr && tran == cnt)
			{
			ptr = data;
			cnt = data_len;
			tran = 0;
			reading = cmd_type;
			if (reading)
				creg = IIC_CNTL_RPST;
			}
		}
	
	return (result);
	}

/***************************************************************************
*
* iicRead - read value from eeprom
*
* This routine read value from eeprom space by call iicTransfer function
*
* RETURNS : OK OR ERROR
*
* ERRNO
*/
int iicRead 
	(
	UCHAR chip, 
	UINT addr, 
	int alen, 
	UCHAR * buffer, 
	int len
	)
	{
	UCHAR xaddr[4];

	if ( alen > 4 ) 
		{
		printf ("iicRead: addr len %d not supported\n", alen);
		return 1;
		}

	if ( alen > 0 ) 
		{
		xaddr[0] = (addr >> 24) & 0xFF;
		xaddr[1] = (addr >> 16) & 0xFF;
		xaddr[2] = (addr >> 8) & 0xFF;
		xaddr[3] = addr & 0xFF;
		}

	return iicTransfer( 1, chip<<1, &xaddr[4-alen], alen, buffer, len );
	}

/***************************************************************************
*
* iicWrite - write eeprom 
*
* This routine write eeprom by call iicTransfer.
*
* RETURNS : OK or ERROR
*
* ERRNO
*/
int iicWrite 
	(
	UCHAR chip, 
	UINT addr, 
	int alen, 
	UCHAR * buffer, 
	int len
	)
	{
	UCHAR xaddr[4];

	if (alen > 4) 
		{
		printf ("iicWrite: addr len %d not supported\n", alen);
		return 1;

		}
	if (alen > 0) 
		{
		xaddr[0] = (addr >> 24) & 0xFF;
		xaddr[1] = (addr >> 16) & 0xFF;
		xaddr[2] = (addr >> 8) & 0xFF;
		xaddr[3] = addr & 0xFF;
		}


	return iicTransfer( 0, chip<<1, &xaddr[4-alen], alen, buffer, len );
	}


/***************************************************************************
*
* updateBootEeprom - update eeprom by call iicWrite function.
*
* This routine update the eeprom value, It call the iicWrite function.
*
* RETURNS : OK OR ERROR
*
* ERRNO
*/
int updateBootEeprom(void)
	{
	unsigned long len = 0x10;
	unsigned char chip = BOOTSTRAP_IIC_ADDR;
	unsigned char* pbuf = bootStrapBuf;
	int ii,jj;

	for(ii = 0; ii< len; ii++)
		{
		if(iicWrite(chip,ii,1,&pbuf[ii],1) != 0)
			{
			printf("iicWrite failed\n");
			return ERROR;
			}
		
		for( jj=0;jj<10;jj++)
			{
			sysUsDelay(1000);
			}	
		}
	
	return OK;
	}

