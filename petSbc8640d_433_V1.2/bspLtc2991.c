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

IMPORT STATUS sysI2cRead(UINT8 busNo,UINT8 deviceAddr,UINT8 regAddr,UINT8 *pData,UINT32 count);
IMPORT STATUS sysI2cWrite(UINT8 busNo,UINT8 deviceAddr,UINT8 regAddr,UINT8 data);


STATUS bspLct2991Init()
{
	//UINT8 value;
	sysI2cWrite(0,0x90,0x1,0xF8);	 //0x90<<1
	//sysI2cRead(0,0x90,0x1,(UINT8*)&value,1);
	//*(UINT32*)0x20000000=0;
	//*(UINT8*)0x20000000=value;
	sysI2cWrite(0,0x92,0x1,0xF8);
	sysI2cWrite(0,0x94,0x1,0x38);
	
	sysI2cWrite(0,0x90,0x8,0x18);
	sysI2cWrite(0,0x92,0x8,0x18);	
	sysI2cWrite(0,0x94,0x8,0x18);
		
	sysMsDelay(1000);
			
	return OK;
}

double bspLct2991getTemp(UINT8 devAddr)
{
	
	double tempVal;
	UINT8 highVal;
	UINT8 lowVal;
	UINT32 temp;
	
	sysI2cRead(0,devAddr,0x1A,(UINT8*)&highVal,1);
	sysI2cRead(0,devAddr,0x1B,(UINT8*)&lowVal,1);
	
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



double bspLct2991GetVol(UINT8 index)
{
	double voltageVal=0;
	UINT8 highVal;
	UINT8 lowVal;
	UINT32 temp;
	
	double vol1,vol2,vol3;
	
	sysI2cWrite(0,0x90,0x6,0x00);
	sysI2cWrite(0,0x90,0x7,0x00);
	
	sysI2cWrite(0,0x92,0x6,0x00);
	sysI2cWrite(0,0x92,0x7,0x00);
	
	sysI2cWrite(0,0x94,0x6,0x00);
	
	sysMsDelay(100);
		
	switch(index)
	{
	case 1:
		sysI2cRead(0,0x90,0x0C,(UINT8*)&highVal,1);
		sysI2cRead(0,0x90,0x0D,(UINT8*)&lowVal,1);
		temp= ((highVal & 0x3f)<<8 |lowVal);
		vol1= 305.18 * temp /1000000;
		
		sysI2cRead(0,0x90,0x10,(UINT8*)&highVal,1);
		sysI2cRead(0,0x90,0x11,(UINT8*)&lowVal,1);
		temp= ((highVal & 0x3f)<<8 |lowVal);
		vol2= 305.18 * temp /1000000;
				
		sysI2cRead(0,0x90,0x14,(UINT8*)&highVal,1);
		sysI2cRead(0,0x90,0x15,(UINT8*)&lowVal,1);
		temp= ((highVal & 0x3f)<<8 |lowVal);
		vol3= 305.18 * temp /1000000;
		
		voltageVal=(vol1+vol2+vol3)/3;
		
		break;
		
	case 2:
		sysI2cRead(0,0x90,0x18,(UINT8*)&highVal,1);
		sysI2cRead(0,0x90,0x19,(UINT8*)&lowVal,1);
		temp= ((highVal & 0x3f)<<8 |lowVal);
		voltageVal= 305.18 * temp /1000000;
		
		break;
		
	case 3:
		sysI2cRead(0,0x92,0x0C,(UINT8*)&highVal,1);
		sysI2cRead(0,0x92,0x0D,(UINT8*)&lowVal,1);
		temp= ((highVal & 0x3f)<<8 |lowVal);
		voltageVal= 305.18 * temp /1000000;
		break;
		
	case 4:
		sysI2cRead(0,0x92,0x10,(UINT8*)&highVal,1);
		sysI2cRead(0,0x92,0x11,(UINT8*)&lowVal,1);
		temp= ((highVal & 0x3f)<<8 |lowVal);
		voltageVal= 305.18 * temp /1000000;
		break;
		
	case 5:
		sysI2cRead(0,0x92,0x14,(UINT8*)&highVal,1);
		sysI2cRead(0,0x92,0x15,(UINT8*)&lowVal,1);
		temp= ((highVal & 0x3f)<<8 |lowVal);
		voltageVal= 305.18 * temp /1000000;
		break;
		
	case 6:
		sysI2cRead(0,0x92,0x18,(UINT8*)&highVal,1);
		sysI2cRead(0,0x92,0x19,(UINT8*)&lowVal,1);
		temp= ((highVal & 0x3f)<<8 |lowVal);
		voltageVal= 305.18 * temp /1000000;
		break;
		
	case 7:
		sysI2cRead(0,0x94,0x0C,(UINT8*)&highVal,1);
		sysI2cRead(0,0x94,0x0D,(UINT8*)&lowVal,1);
		temp= ((highVal & 0x3f)<<8 |lowVal);
		voltageVal= 305.18 * temp /1000000;
		
		break;
	case 8:
		sysI2cRead(0,0x94,0x10,(UINT8*)&highVal,1);
		sysI2cRead(0,0x94,0x11,(UINT8*)&lowVal,1);
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

double bspLct2991GetCurr(UINT8 index)
{
	double current=0;

	UINT8 highVal;
	UINT8 lowVal;
	UINT32 temp;
	double cur1,cur2,cur3;
	
	sysI2cWrite(0,0x90,0x6,0x11);
	sysI2cWrite(0,0x90,0x7,0x11);
	
	sysI2cWrite(0,0x92,0x6,0x11);
	sysI2cWrite(0,0x92,0x7,0x11);
	
	sysI2cWrite(0,0x94,0x6,0x01);
	
	sysMsDelay(100);
	
	
	switch(index)
	{
	case 1:
		sysI2cRead(0,0x90,0x0C,(UINT8*)&highVal,1);
		sysI2cRead(0,0x90,0x0D,(UINT8*)&lowVal,1);
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
						
		sysI2cRead(0,0x90,0x10,(UINT8*)&highVal,1);
		sysI2cRead(0,0x90,0x11,(UINT8*)&lowVal,1);
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
						
		sysI2cRead(0,0x90,0x14,(UINT8*)&highVal,1);
		sysI2cRead(0,0x90,0x15,(UINT8*)&lowVal,1);
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
		sysI2cRead(0,0x90,0x18,(UINT8*)&highVal,1);
		sysI2cRead(0,0x90,0x19,(UINT8*)&lowVal,1);
		
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
		sysI2cRead(0,0x92,0x0C,(UINT8*)&highVal,1);
		sysI2cRead(0,0x92,0x0D,(UINT8*)&lowVal,1);
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
		sysI2cRead(0,0x92,0x10,(UINT8*)&highVal,1);
		sysI2cRead(0,0x92,0x11,(UINT8*)&lowVal,1);
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
		sysI2cRead(0,0x92,0x14,(UINT8*)&highVal,1);
		sysI2cRead(0,0x92,0x15,(UINT8*)&lowVal,1);
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
		sysI2cRead(0,0x92,0x18,(UINT8*)&highVal,1);
		sysI2cRead(0,0x92,0x19,(UINT8*)&lowVal,1);
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
		
		sysI2cRead(0,0x4A,0x0C,(UINT8*)&highVal,1);
		sysI2cRead(0,0x4A,0x0D,(UINT8*)&lowVal,1);
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

double bspLct2991GetTotalPower()
{
	double totalPower=0;
	double vol;
	double curr;

	int i;
	
	for(i=1;i<8;i++)
	{
		vol=bspLct2991GetVol(i);
		curr=bspLct2991GetCurr(i);
		
		totalPower=totalPower+vol*curr;
		
		sysMsDelay(10);
	}
		
	return totalPower;
	
}



void bspShowVoltage(UINT32 maxIndex)
{
	int i;
	double vol1,vol2,vol3,vol4,vol5,vol6,vol7,vol8;
	
	
	printf("----------------------------------------------------------------------------------------------\n");
	printf("Index    VCC0.95     VCC1.2    VCC1.05    VCC2.5    VCC3.3    VCC1.8     VCC1.0     VCC12\n");		  
	printf("----------------------------------------------------------------------------------------------\n");
		
	for(i=0;i<maxIndex;i++)
	{	
		
		vol1=bspLct2991GetVol(1);
		vol2=bspLct2991GetVol(2);		
		vol3=bspLct2991GetVol(3);
		vol4=bspLct2991GetVol(4);
		vol5=bspLct2991GetVol(5);
		vol6=bspLct2991GetVol(6);
		vol7=bspLct2991GetVol(7);
		vol8=bspLct2991GetVol(8);
							
		printf("%2d      %.4f     %.4f     %.4f     %.4f     %.4f    %.4f    %.4f    %.4f\n",
						i, vol1,vol2,vol3,vol4,vol5,vol6,vol7,vol8);
		
		sysMsDelay(1000);
	}
}

void  bspShowCurrent(UINT32 maxIndex)
{
	int i;
	double curr1,curr2,curr3,curr4,curr5,curr6,curr7;
	

	
	printf("-------------------------------------------------------------------------------\n");
	printf("Index    I0.95    I1.2    I1.05    I2.5    I3.3    I1.8     I1.0\n");		  
	printf("-------------------------------------------------------------------------------\n");
		
	for(i=0;i<maxIndex;i++)
	{			
		curr1=bspLct2991GetCurr(1);
		curr2=bspLct2991GetCurr(2);
		curr3=bspLct2991GetCurr(3);
		curr4=bspLct2991GetCurr(4);
		curr5=bspLct2991GetCurr(5);
		curr6=bspLct2991GetCurr(6);
		curr7=bspLct2991GetCurr(7);
							
		printf("%d      %.2f     %.2f     %.2f     %.2f     %.2f    %.2f    %.2f\n",
						i, curr1,curr2,curr3,curr4,curr5,curr6,curr7);
		
		sysMsDelay(1000);
	}	
}

void bspShowTotalPower(UINT32 maxIndex)
{
	int i;
	double totalPower;
	
	
	printf("-------------------------------------------------------------------------------\n");
	printf("Index  TotalPower\n");		  
	printf("-------------------------------------------------------------------------------\n");
	
	for(i=0;i<maxIndex;i++)
	{
		totalPower=bspLct2991GetTotalPower();
	
		printf("%d     %.4f.\n",i,totalPower);
		
		sysMsDelay(1000);
	}
	
}
	
