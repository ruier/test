/* testHotPlag.c - test hot plag,test routine.*/

/*
 * Copyright (c) 2012- Prophet Electronic,Inc.
 * 
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Prophet license agreement. 
 */

/*
modification history
-----------------------
12/09/2012,sjy Created the file.
 */

/*
 * DESCRIPTION
 * Initialize the RapidIO Switch of CPS1848.
 * 
 */

/* includes */

#include <vxworks.h>

void rioScanTask()
{
	while(1)
	{
		rioScanDevice();
		taskDelay(3600);
	}
}

void testRoutine()
{
	int cpuID;
	int slotNum;
	
	cpuID = sysGetCpuID();
	slotNum = sysGetSlotNum();
	if((cpuID == 0)&&(slotNum == 1))
	{
		taskSpawn("tRioScan",150,0,0x100000,(FUNCPTR)rioScanTask,1,2,3,4,5,6,7,8,9,10);
	}
	
}
