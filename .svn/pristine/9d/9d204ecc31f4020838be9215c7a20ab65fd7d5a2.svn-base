#include <stdio.h>
#include <vxWorks.h>
#include <cpuset.h>
#include "taskLib.h"

#include "bsp.h"
#include "logLib.h"

UINT32 testLedFlashStop=0;
UINT32 testLedControlStop=0;

void testLedFlash(UINT8 time)
{
	while(!testLedFlashStop)
	{
		sysLedSet(sysGetCpuID(),1); /*led off*/
		taskDelay(time);
		sysLedSet(sysGetCpuID(),0); /*led on*/
		taskDelay(time);
	}
}

STATUS testLedFlashTask()
{
	testLedFlashStop = 0;
	taskSpawn ("tTestLedFlash", 200, 0, 20000,(FUNCPTR)testLedFlash, 180,0,0,0,0,0,0,0,0,0);
}


void testLed(UINT8 mode,UINT8 time)
{
	if(mode == 0)
	{
		sysLedSet(0,1);
		sysLedSet(1,1);
		while(!testLedControlStop)
		{
			sysLedSet(1,1); /*led on*/
			sysLedSet(0,0); /*led on*/
			taskDelay(time);
			sysLedSet(0,1); /*led on*/
			sysLedSet(1,0); /*led on*/
			taskDelay(time);
		}
	}
	else
	{
		sysLedSet(2,1);
		sysLedSet(3,1);
		sysLedSet(7,1);
		while(!testLedControlStop)
		{
			sysLedSet(7,1); /*led on*/
			sysLedSet(2,0); /*led on*/
			taskDelay(time);
			sysLedSet(2,1); /*led on*/
			sysLedSet(3,0); /*led on*/
			taskDelay(time);
			sysLedSet(3,1); /*led on*/
			sysLedSet(7,0); /*led on*/
			taskDelay(time);
		}
		
	}
	
	
	
}
STATUS testLedTask(UINT32 delayTime)
{
	testLedControlStop = 0;
	taskSpawn ("tTestLed0", 200, 0, 20000,(FUNCPTR)testLed,0,delayTime,0,0,0,0,0,0,0,0);
	taskSpawn ("tTestLed1", 200, 0, 20000,(FUNCPTR)testLed,1,delayTime,0,0,0,0,0,0,0,0);
}

#if 0

void testLedControl(UINT8 time)
{

	while(!testLedControlStop)
	{
		sysLedSet(7,1);
		sysLedSet(0,0); /*led on*/
		taskDelay(time);
		sysLedSet(0,1); /*led on*/
		sysLedSet(1,0); /*led on*/
		taskDelay(time);
		sysLedSet(1,1); /*led on*/
		sysLedSet(2,0); /*led on*/
		taskDelay(time);
		sysLedSet(2,1); /*led on*/
		sysLedSet(3,0); /*led on*/
		taskDelay(time);
		sysLedSet(3,1); /*led on*/
		sysLedSet(7,0); /*led on*/
		taskDelay(time);
	}
}

STATUS testLedControlTask()
{
	taskSpawn ("tTestLedFlash", 200, 0, 20000,(FUNCPTR)testLedControl,10,0,0,0,0,0,0,0,0,0);
}
#endif
