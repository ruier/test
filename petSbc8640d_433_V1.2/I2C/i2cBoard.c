/*i2cBoard.c - 835 Driver Source Module */

/* create by:YZF 2012.11.30 */

/* includes */

#include <vxWorks.h>       	 		
#include "config.h"
#include <stdio.h>
#include <string.h>
#include <logLib.h>
#include <stdlib.h>
#include <taskLib.h>
#include <arch/ppc/vxPpcLib.h>

#include "i2cBoard.h"


void testBoardTemp(UINT32 maxIndex)
{
	int i;
	double temp1,temp2,temp3;
	
	i2cInit();
	initLct2991();
		
	printf("-------------------------------------------------------------------------------------------\n");
	printf("ITEM            ALERT THRESHOLD\n");
	printf("-------------------------------------------------------------------------------------------\n");
	
	printf("CPU A           %d\n",getAlertThres(4));
	printf("CPU B           %d\n",getAlertThres(2));
	printf("CPU C           %d\n",getAlertThres(1));
	printf("CPU D           %d\n",getAlertThres(3));
	printf("FPGA            %d\n",getAlertThres(5));
	printf("LOCAL           %d\n",getAlertThres(0));
	
	printf("-------------------------------------------------------------------------------------------\n");
	
	temp1=getLocalTemp(0x48);
	printf("temp is %.4f.\n",temp1);
	temp2=getLocalTemp(0x49);
	printf("temp is %.4f.\n",temp2);
	temp3=getLocalTemp(0x4a);
	printf("temp is %.4f.\n",temp3);
	
	printf("\n\n");
	
	printf("-------------------------------------------------------------------------------------------\n");
	printf("Index  CPU A  CPU B  CPU C  CPU D  FPGA  LOCAL1   LOCAL2   LOCAL3    LOCAL4\n");		  
	printf("-------------------------------------------------------------------------------------------\n");
	
	for(i=0;i<maxIndex;i++)
	{
		temp1=getLocalTemp(0x48);
		temp2=getLocalTemp(0x49);
		temp3=getLocalTemp(0x4a);
		printf("%2d      %.0f     %.0f     %.0f     %.0f     %.0f    %.0f      %.0f      %.0f      %.0f\n",
				i, getChannelTemp(4),getChannelTemp(2),getChannelTemp(1),getChannelTemp(3),getChannelTemp(5),getChannelTemp(0),temp1,temp2,temp3);
		
		sysMsDelay(1000);
	}	
}

void testBoardVoltage(UINT32 maxIndex)
{
	int i;
	double vol1,vol2,vol3,vol4,vol5,vol6,vol7,vol8;
	
	i2cInit();
	initLct2991();
	
	printf("----------------------------------------------------------------------------------------------\n");
	printf("Index    VCC0.95     VCC1.2    VCC1.05    VCC2.5    VCC3.3    VCC1.8     VCC1.0     VCC12\n");		  
	printf("----------------------------------------------------------------------------------------------\n");
		
	for(i=0;i<maxIndex;i++)
	{	
		
		vol1=getBoardVol(1);
		vol2=getBoardVol(2);		
		vol3=getBoardVol(3);
		vol4=getBoardVol(4);
		vol5=getBoardVol(5);
		vol6=getBoardVol(6);
		vol7=getBoardVol(7);
		vol8=getBoardVol(8);
							
		printf("%2d      %.4f     %.4f     %.4f     %.4f     %.4f    %.4f    %.4f    %.4f\n",
						i, vol1,vol2,vol3,vol4,vol5,vol6,vol7,vol8);
		
		sysMsDelay(1000);
	}
}

void testBoardCurrent(UINT32 maxIndex)
{
	int i;
	double curr1,curr2,curr3,curr4,curr5,curr6,curr7;
	
	i2cInit();
	initLct2991();
	
	printf("-------------------------------------------------------------------------------\n");
	printf("Index    I0.95    I1.2    I1.05    I2.5    I3.3    I1.8     I1.0\n");		  
	printf("-------------------------------------------------------------------------------\n");
		
	for(i=0;i<maxIndex;i++)
	{			
		curr1=getBoardCurr(1);
		curr2=getBoardCurr(2);
		curr3=getBoardCurr(3);
		curr4=getBoardCurr(4);
		curr5=getBoardCurr(5);
		curr6=getBoardCurr(6);
		curr7=getBoardCurr(7);
							
		printf("%d      %.2f     %.2f     %.2f     %.2f     %.2f    %.2f    %.2f\n",
						i, curr1,curr2,curr3,curr4,curr5,curr6,curr7);
		
		sysMsDelay(1000);
	}	
}

void testTotalPower(UINT32 maxIndex)
{
	int i;
	double totalPower;
	
	i2cInit();
	initLct2991();
	
	printf("-------------------------------------------------------------------------------\n");
	printf("Index  TotalPower\n");		  
	printf("-------------------------------------------------------------------------------\n");
	
	for(i=0;i<maxIndex;i++)
	{
		totalPower=getTotalPower();
	
		printf("%d     %.4f.\n",i,totalPower);
		
		sysMsDelay(1000);
	}
	
}



