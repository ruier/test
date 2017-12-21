/* max6689.c - max6689 Driver Source Module */

/* create by:YZF */

/* includes */

#include <vxWorks.h>       	 		
#include "config.h"
#include <stdio.h>
#include <string.h>
#include <logLib.h>
#include <stdlib.h>
#include <taskLib.h>
#include <arch/ppc/vxPpcLib.h>

#include "mpc8640dI2c.h"


STATUS rdMax6689Id()
{
	UINT8 regVal;
	
	regVal=i2cRegRead(0x1a,0x0A);
	
	if(regVal== 0x4d)
	{
		printf("The Max6689's ID is 0x%x.Access OK!\n",regVal);
		return OK;
	}
	else
	{
		printf("The Max6689 access failure!\n");
		return ERROR;
	}	
}


float getChannelTemp(UINT8 channel)
{
	UINT16 retVal;
	UINT8 regVal;
	
	if(channel==0)
	{
		regVal=i2cRegRead(0x1a,0x07);
		retVal=regVal<<3;
		regVal=i2cRegRead(0x1a,0x09);
		retVal=((UINT16)retVal)|(regVal>>5);
		retVal=retVal*0.125;
	}
	else if((channel>0)&&(channel<7))
	{
		regVal=i2cRegRead(0x1a,channel);
		
		retVal=regVal;
	}
	else
	{
		printf("channel is illegal,please input again!\n");
		return ERROR;
	}
				
	return retVal;
}

STATUS setAlertThres(UINT8 channel,UINT8 thresVal)
{
	switch(channel)
	{
		case 0:
			i2cRegWrite(0x1a,0x17,thresVal);
			break;
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
			i2cRegWrite(0x1a,0x10+channel,thresVal);
			break;
		default:
			printf("channel is illegal,please input again!\n");
			return ERROR;
	}
	
	return OK;
}

UINT8 getAlertThres(UINT8 channel)
{
	UINT regVal;
	
	switch(channel)
	{
		case 0:
			regVal=i2cRegRead(0x1a,0x17);
			break;
			
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
			regVal=i2cRegRead(0x1a,0x10+channel);
			break;
			
		default:
			printf("channel is illegal,please input again!\n");
			return 0xFF;
	}
	
	return regVal;
}

STATUS setOverThres(UINT8 channel,UINT8 thresVal)
{
	switch(channel)
	{
		case 1:
			i2cRegWrite(0x1a,0x21,thresVal);
			break;

		case 4:
			i2cRegWrite(0x1a,0x24,thresVal);
			break;
			
		case 5:
			i2cRegWrite(0x1a,0x25,thresVal);
			break;
			
		case 6:
			i2cRegWrite(0x1a,0x26,thresVal);
			break;
			
		default:
			printf("channel is illegal,please input again!\n");
			return ERROR;
	}	
	
	return OK;
}

UINT8 getOverThres(UINT8 channel)
{
	UINT8 regVal;
	
	switch(channel)
	{
		case 1:
			regVal=i2cRegRead(0x1a,0x21);
			break;

		case 4:
			regVal=i2cRegRead(0x1a,0x24);
			break;
			
		case 5:
			regVal=i2cRegRead(0x1a,0x25);
			break;
			
		case 6:
			regVal=i2cRegRead(0x1a,0x26);
			break;
			
		default:
			printf("channel is illegal,please input again!\n");
			return 0xFF;
	}	
	
	return OK;	
}


