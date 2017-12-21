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

//#include "mpc8640dI2c.h"

#define MAX6689_DEVICE_ID 0x34   //(0x1a<<1)

IMPORT STATUS sysI2cRead(UINT8 busNo,UINT8 deviceAddr,UINT8 regAddr,UINT8 *pData,UINT32 count);
IMPORT STATUS sysI2cWrite(UINT8 busNo,UINT8 deviceAddr,UINT8 regAddr,UINT8 data);
IMPORT double bspLct2991getTemp(UINT8 devAddr);




STATUS bspMax6689ReadId()
{
	UINT8 regVal;
	
	sysI2cRead(0,MAX6689_DEVICE_ID,0x0a,&regVal,1);
	
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


float bspMax6689GetTemp(UINT8 channel)
{
	UINT16 retVal = 0;
	UINT8 regVal;
	
	if(channel==0)
	{
		sysI2cRead(0,MAX6689_DEVICE_ID,0x07,(UINT8 *)&regVal,1);	
		retVal |= (regVal<<3);
		sysI2cRead(0,MAX6689_DEVICE_ID,0x09,(UINT8 *)&regVal,1);
		retVal |= (regVal>>5);
		retVal=retVal*0.125;
	}
	else if((channel>0)&&(channel<7))
	{
		sysI2cRead(0,MAX6689_DEVICE_ID,channel,(UINT8 *)&regVal,1);
		retVal=regVal;
	}
	else
	{
		printf("channel is illegal,please input again!\n");
		return ERROR;
	}
				
	return retVal;
}

STATUS bspMax6689SetAlertThres(UINT8 channel,UINT8 thresVal)
{
	switch(channel)
	{
		case 0:
			sysI2cWrite(0,MAX6689_DEVICE_ID,0x17,thresVal);
			break;
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
			sysI2cWrite(0,MAX6689_DEVICE_ID,0x10+channel,thresVal);
			break;
		default:
			printf("channel is illegal,please input again!\n");
			return ERROR;
	}
	
	return OK;
}

UINT8 bspMax6689GetAlertThres(UINT8 channel)
{
	UINT8 regVal;
	
	switch(channel)
	{
		case 0:
			sysI2cRead(0,MAX6689_DEVICE_ID,0x17,(UINT8 *)&regVal,1);
			break;
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
			sysI2cRead(0,MAX6689_DEVICE_ID,(0x10+channel),(UINT8 *)&regVal,1);
			break;
			
		default:
			printf("channel is illegal,please input again!\n");
			return 0xFF;
	}
	
	return regVal;
}

STATUS bspMax6689SetOverThres(UINT8 channel,UINT8 thresVal)
{
	switch(channel)
	{
		case 1:
			sysI2cWrite(0,MAX6689_DEVICE_ID,0x21,thresVal);
			break;

		case 4:
			sysI2cWrite(0,MAX6689_DEVICE_ID,0x24,thresVal);
			break;
			
		case 5:
			sysI2cWrite(0,MAX6689_DEVICE_ID,0x25,thresVal);
			break;
			
		case 6:
			sysI2cWrite(0,MAX6689_DEVICE_ID,0x26,thresVal);
			break;
			
		default:
			printf("channel is illegal,please input again!\n");
			return ERROR;
	}	
	
	return OK;
}

UINT8 bspMax6689GetOverThres(UINT8 channel)
{
	UINT8 regVal;
	
	switch(channel)
	{
		case 1:
			sysI2cRead(0,MAX6689_DEVICE_ID,0x21,&regVal,1);
			break;
		case 4:
			sysI2cRead(0,MAX6689_DEVICE_ID,0x24,&regVal,1);
			break;
		case 5:
			sysI2cRead(0,MAX6689_DEVICE_ID,0x25,&regVal,1);
			break;
			
		case 6:
			sysI2cRead(0,MAX6689_DEVICE_ID,0x26,&regVal,1);
			break;
			
		default:
			printf("channel is illegal,please input again!\n");
			return 0xFF;
	}	
	
	return OK;	
}



void bspTempShow(UINT32 maxIndex)
{
	int i;
	double temp1,temp2,temp3;
	

		
	printf("-------------------------------------------------------------------------------------------\n");
	printf("ITEM            ALERT THRESHOLD\n");
	printf("-------------------------------------------------------------------------------------------\n");
	
	printf("CPU A           %d\n",bspMax6689GetAlertThres(4));
	printf("CPU B           %d\n",bspMax6689GetAlertThres(2));
	printf("CPU C           %d\n",bspMax6689GetAlertThres(1));
	printf("CPU D           %d\n",bspMax6689GetAlertThres(3));
	printf("FPGA            %d\n",bspMax6689GetAlertThres(5));
	printf("LOCAL           %d\n",bspMax6689GetAlertThres(0));
	
	printf("-------------------------------------------------------------------------------------------\n");
	
	temp1=bspLct2991getTemp(0x90);
	//printf("temp is %.4f.\n",temp1);
	temp2=bspLct2991getTemp(0x92);
	//printf("temp is %.4f.\n",temp2);
	temp3=bspLct2991getTemp(0x94);
	//printf("temp is %.4f.\n",temp3);
	
	printf("\n\n");
	
	printf("-------------------------------------------------------------------------------------------\n");
	printf("Index  CPU A  CPU B  CPU C  CPU D  FPGA  LOCAL1   LOCAL2   LOCAL3    LOCAL4\n");		  
	printf("-------------------------------------------------------------------------------------------\n");
	
	for(i=0;i<maxIndex;i++)
	{
		temp1=bspLct2991getTemp(0x90);
		temp2=bspLct2991getTemp(0x92);
		temp3=bspLct2991getTemp(0x94);
		printf("%2d      %.0f     %.0f     %.0f     %.0f     %.0f    %.0f      %.0f      %.0f      %.0f\n",
				i, bspMax6689GetTemp(4),bspMax6689GetTemp(2),bspMax6689GetTemp(1),bspMax6689GetTemp(3),bspMax6689GetTemp(5),bspMax6689GetTemp(0),temp1,temp2,temp3);
		
		sysMsDelay(1000);
	}	
}

LOCAL UINT8 alertThres[6] = { 70,80,80,87,80,100};  //local CPUC CPUB CPUD CPUA FPGA
void bspSetOverThres()
{
	UINT8 i;
	for(i=0;i<6;i++) bspMax6689SetAlertThres(i,alertThres[i]);
}
