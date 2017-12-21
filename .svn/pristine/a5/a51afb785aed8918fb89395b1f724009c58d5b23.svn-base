/*ltc2991.c - ltc2991 Driver Source Module */

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


STATUS initLct2991()
{
	i2cRegWrite(0x48,0x1,0xF8);	
	i2cRegWrite(0x49,0x1,0xF8);	
	i2cRegWrite(0x4a,0x1,0x38);
	
	i2cRegWrite(0x48,0x8,0x18);
	i2cRegWrite(0x49,0x8,0x18);	
	i2cRegWrite(0x4a,0x8,0x18);
		
	sysMsDelay(1000);
			
	return OK;
}

double getLocalTemp(UINT8 devAddr)
{
	
	double tempVal;
	UINT32 highVal;
	UINT32 lowVal;
	UINT32 temp;
	
	highVal=i2cRegRead(devAddr,0x1A);
	lowVal=i2cRegRead(devAddr,0x1B);
	
	if((highVal&0x10)==0x00)
	{
		temp= ((highVal&0x1f)<<8 |lowVal);
		tempVal= temp * 0.0625;
				
		return tempVal;
	}
	else
	{
		temp= ((highVal&0x1f)<<8 | lowVal);
		temp=(~temp)&0x1fff;
		tempVal= (temp+1)*(-0.0625); 
				
		return tempVal;
	}	
}

IMPORT void i2cInit();
void showChannelVol()
{
	double voltageVal=0;
	UINT32 highVal;
	UINT32 lowVal;
	UINT32 temp;
	
	i2cInit();
	initLct2991();
	
	i2cRegWrite(0x48,0x6,0x00);
	i2cRegWrite(0x48,0x7,0x00);
	
	i2cRegWrite(0x49,0x6,0x00);
	i2cRegWrite(0x49,0x7,0x00);
	
	i2cRegWrite(0x4a,0x6,0x00);
	
	sysMsDelay(100);
	
	highVal=i2cRegRead(0x48,0x0A);
	lowVal=i2cRegRead(0x48,0x0B);
	temp= ((highVal & 0x3f)<<8 |lowVal);
	voltageVal= 305.18 * temp /1000000;
	
	printf("U649 1 Channel Voltage is %.4f.\n",voltageVal);
	
	highVal=i2cRegRead(0x48,0x0C);
	lowVal=i2cRegRead(0x48,0x0D);
	temp= ((highVal & 0x3f)<<8 |lowVal);
	voltageVal= 305.18 * temp /1000000;
	
	printf("U649 2 Channel Voltage is %.4f.\n",voltageVal);
	
	highVal=i2cRegRead(0x48,0x0E);
	lowVal=i2cRegRead(0x48,0x0F);
	temp= ((highVal & 0x3f)<<8 |lowVal);
	voltageVal= 305.18 * temp /1000000;
	
	printf("U649 3 Channel Voltage is %.4f.\n",voltageVal);
	
	highVal=i2cRegRead(0x48,0x10);
	lowVal=i2cRegRead(0x48,0x11);
	temp= ((highVal & 0x3f)<<8 |lowVal);
	voltageVal= 305.18 * temp /1000000;
	
	printf("U649 4 Channel Voltage is %.4f.\n",voltageVal);
	
	
	highVal=i2cRegRead(0x48,0x12);
	lowVal=i2cRegRead(0x48,0x13);
	temp= ((highVal & 0x3f)<<8 |lowVal);
	voltageVal= 305.18 * temp /1000000;
	
	printf("U649 5 Channel Voltage is %.4f.\n",voltageVal);
	
	highVal=i2cRegRead(0x48,0x14);
	lowVal=i2cRegRead(0x48,0x15);
	temp= ((highVal & 0x3f)<<8 |lowVal);
	voltageVal= 305.18 * temp /1000000;
	
	printf("U649 6 Channel Voltage is %.4f.\n",voltageVal);
	
	highVal=i2cRegRead(0x48,0x16);
	lowVal=i2cRegRead(0x48,0x17);
	temp= ((highVal & 0x3f)<<8 |lowVal);
	voltageVal= 305.18 * temp /1000000;
	
	printf("U649 7 Channel Voltage is %.4f.\n",voltageVal);
	
	highVal=i2cRegRead(0x48,0x18);
	lowVal=i2cRegRead(0x48,0x19);
	temp= ((highVal & 0x3f)<<8 |lowVal);
	voltageVal= 305.18 * temp /1000000;
	
	printf("U649 8 Channel Voltage is %.4f.\n",voltageVal);	
}

double getBoardVol(UINT8 index)
{
	double voltageVal=0;
	UINT32 highVal;
	UINT32 lowVal;
	UINT32 temp;
	
	double vol1,vol2,vol3;
	
	i2cRegWrite(0x48,0x6,0x00);
	i2cRegWrite(0x48,0x7,0x00);
	
	i2cRegWrite(0x49,0x6,0x00);
	i2cRegWrite(0x49,0x7,0x00);
	
	i2cRegWrite(0x4a,0x6,0x00);
	
	sysMsDelay(100);
		
	switch(index)
	{
	case 1:
		highVal=i2cRegRead(0x48,0x0C);
		lowVal=i2cRegRead(0x48,0x0D);
		printf("highVal = 0x%x,lowVal=0x%x\n",highVal,lowVal);
		temp= ((highVal & 0x3f)<<8 |lowVal);
		vol1= 305.18 * temp /1000000;
		
		highVal=i2cRegRead(0x48,0x10);
		lowVal=i2cRegRead(0x48,0x11);
		printf("highVal = 0x%x,lowVal=0x%x\n",highVal,lowVal);
		temp= ((highVal & 0x3f)<<8 |lowVal);
		vol2= 305.18 * temp /1000000;
				
		highVal=i2cRegRead(0x48,0x14);
		lowVal=i2cRegRead(0x48,0x15);
		printf("highVal = 0x%x,lowVal=0x%x\n",highVal,lowVal);
		temp= ((highVal & 0x3f)<<8 |lowVal);
		vol3= 305.18 * temp /1000000;
		
		voltageVal=(vol1+vol2+vol3)/3;
		
		break;
		
	case 2:
		highVal=i2cRegRead(0x48,0x18);
		lowVal=i2cRegRead(0x48,0x19);
		temp= ((highVal & 0x3f)<<8 |lowVal);
		voltageVal= 305.18 * temp /1000000;
		
		break;
		
	case 3:
		highVal=i2cRegRead(0x49,0x0C);
		lowVal=i2cRegRead(0x49,0x0D);
		temp= ((highVal & 0x3f)<<8 |lowVal);
		voltageVal= 305.18 * temp /1000000;
		break;
		
	case 4:
		highVal=i2cRegRead(0x49,0x10);
		lowVal=i2cRegRead(0x49,0x11);
		temp= ((highVal & 0x3f)<<8 |lowVal);
		voltageVal= 305.18 * temp /1000000;
		break;
		
	case 5:
		highVal=i2cRegRead(0x49,0x14);
		lowVal=i2cRegRead(0x49,0x15);
		temp= ((highVal & 0x3f)<<8 |lowVal);
		voltageVal= 305.18 * temp /1000000;
		break;
		
	case 6:
		highVal=i2cRegRead(0x49,0x18);
		lowVal=i2cRegRead(0x49,0x19);
		temp= ((highVal & 0x3f)<<8 |lowVal);
		voltageVal= 305.18 * temp /1000000;
		break;
		
	case 7:
		highVal=i2cRegRead(0x4a,0x0C);
		lowVal=i2cRegRead(0x4a,0x0D);
		temp= ((highVal & 0x3f)<<8 |lowVal);
		voltageVal= 305.18 * temp /1000000;
		
		break;
	case 8:
		highVal=i2cRegRead(0x4a,0x10);
		lowVal=i2cRegRead(0x4a,0x11);
		temp= ((highVal & 0x3f)<<8 |lowVal);
		voltageVal= 305.18 * temp /1000000;
		voltageVal=voltageVal*4.01;
		
		break;
	case 9:
		break;
	default:
		printf("Input index ERROR!\n");
		return ERROR;
	}
		
	return voltageVal;
}

double getBoardCurr(UINT8 index)
{
	double current=0;

	UINT32 highVal;
	UINT32 lowVal;
	UINT32 temp;
	double cur1,cur2,cur3;
	
	i2cRegWrite(0x48,0x6,0x11);
	i2cRegWrite(0x48,0x7,0x11);
	
	i2cRegWrite(0x49,0x6,0x11);
	i2cRegWrite(0x49,0x7,0x11);
	
	i2cRegWrite(0x4a,0x6,0x01);
	
	sysMsDelay(100);
	
	
	switch(index)
	{
	case 1:
		highVal=i2cRegRead(0x48,0x0C);
		lowVal=i2cRegRead(0x48,0x0D);
		printf("highVal = 0x%x,lowVal=0x%x\n",highVal,lowVal);
		if((highVal & 0x40)==0x00)
		{
			temp=((highVal & 0x3f)<<8)|lowVal;
			cur1 = temp * 19.0735/500;
		}
		else
		{
			temp=((highVal & 0x3f)<<8)|lowVal;
			temp=(~temp)&0x3fff;
			cur1 = (temp+1)*(-19.0735)/500; 
		}
						
		highVal=i2cRegRead(0x48,0x10);
		lowVal=i2cRegRead(0x48,0x11);
		printf("highVal = 0x%x,lowVal=0x%x\n",highVal,lowVal);
		if((highVal & 0x40)==0x00)
		{
			temp=((highVal & 0x3f)<<8)|lowVal;
			cur2 = temp * 19.0735/500;
		}
		else
		{
			temp=((highVal & 0x3f)<<8)|lowVal;
			temp=(~temp)&0x3fff;
			cur2 = (temp+1)*(-19.0735)/500; 
		}		
						
		highVal=i2cRegRead(0x48,0x14);
		lowVal=i2cRegRead(0x48,0x15);
		printf("highVal = 0x%x,lowVal=0x%x\n",highVal,lowVal);
		if((highVal & 0x40)==0x00)
		{
			temp=((highVal & 0x3f)<<8)|lowVal;
			cur3 = temp * 19.0735/500;
		}
		else
		{
			temp=((highVal & 0x3f)<<8)|lowVal;
			temp=(~temp)&0x3fff;
			cur3 = (temp+1)*(-19.0735)/500; 
		}		
			
		current =cur1+cur2+cur3;
	
		break;
		
	case 2:
		highVal=i2cRegRead(0x48,0x18);
		lowVal=i2cRegRead(0x48,0x19);
		
		if((highVal & 0x40)==0x00)
		{
			temp=((highVal & 0x3f)<<8)|lowVal;
			current = temp * 19.0735/1000;
		}
		else
		{
			temp=((highVal & 0x3f)<<8)|lowVal;
			temp=(~temp)&0x3fff;
			current = (temp+1)*(-19.0735)/1000; 
		}
				
		break;
		
	case 3:
		highVal=i2cRegRead(0x49,0x0C);
		lowVal=i2cRegRead(0x49,0x0D);
		if((highVal & 0x40)==0x00)
		{
			temp=((highVal & 0x3f)<<8)|lowVal;
			current = temp * 19.0735/1000;
		}
		else
		{
			temp=((highVal & 0x3f)<<8)|lowVal;
			temp=(~temp)&0x3fff;
			current = (temp+1)*(-19.0735)/1000; 
		}
				
		break;
		
	case 4:
		highVal=i2cRegRead(0x49,0x10);
		lowVal=i2cRegRead(0x49,0x11);
		if((highVal & 0x40)==0x00)
		{
			temp=((highVal & 0x3f)<<8)|lowVal;
			current = temp * 19.0735/3000;
		}
		else
		{
			temp=((highVal & 0x3f)<<8)|lowVal;
			temp=(~temp)&0x3fff;
			current = (temp+1)*(-19.0735)/3000; 
		}
				
		break;
		
	case 5:
		highVal=i2cRegRead(0x49,0x14);
		lowVal=i2cRegRead(0x49,0x15);
		if((highVal & 0x40)==0x00)
		{
			temp=((highVal & 0x3f)<<8)|lowVal;
			current = temp * 19.0735/3000;
		}
		else
		{
			temp=((highVal & 0x3f)<<8)|lowVal;
			temp=(~temp)&0x3fff;
			current = (temp+1)*(-19.0735)/3000; 
		}
			
		break;
		
	case 6:
		highVal=i2cRegRead(0x49,0x18);
		lowVal=i2cRegRead(0x49,0x19);
		if((highVal & 0x40)==0x00)
		{
			temp=((highVal & 0x3f)<<8)|lowVal;
			current = temp * 19.0735/1000;
		}
		else
		{
			temp=((highVal & 0x3f)<<8)|lowVal;
			temp=(~temp)&0x3fff;
			current = (temp+1)*(-19.0735)/1000; 
		}
			
		break;
		
	case 7:
		
		highVal=i2cRegRead(0x4A,0x0C);
		lowVal=i2cRegRead(0x4A,0x0D);
		if((highVal & 0x40)==0x00)
		{
			temp=((highVal & 0x3f)<<8)|lowVal;
			current = temp * 19.0735/1000;
		}
		else
		{
			temp=((highVal & 0x3f)<<8)|lowVal;
			temp=(~temp)&0x3fff;
			current = (temp+1)*(-19.0735)/1000; 
		}
			
		break;
		
	case 8:
		
		break;
		
	default:
		printf("Input index ERROR!\n");
		return ERROR;
	}	
	
	if(current<0)
	{
		current=-current;
	}
		
	return current;
}

double getTotalPower()
{
	double totalPower=0;
	double vol;
	double curr;

	int i;
	
	for(i=1;i<8;i++)
	{
		vol=getBoardVol(i);
		curr=getBoardCurr(i);
		
		totalPower=totalPower+vol*curr;
		
		sysMsDelay(10);
	}
		
	return totalPower;
	
}


	
