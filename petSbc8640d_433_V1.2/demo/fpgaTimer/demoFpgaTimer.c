
#include <stdio.h>
#include <vxWorks.h>


STATUS demoFpgaTimerTest(UINT32 delayTime,UINT32 time)
{
	UINT32 fpgaTimerClkFreq,i;
	UINT32 startTimeH,startTimeL,endTimeH,endTimeL;
	UINT32 startTimeH1,startTimeL1,endTimeH1,endTimeL1;
	float timespend,timespend1;
	
	
	fpgaTimerClkFreq = sysFpgaTimerClkFreqGet();
//	printf("fpgaTimerClkFreq=%d\n",fpgaTimerClkFreq);
	for(i=0;i<time;i++)
	{
		sysGetTime(&startTimeH,&startTimeL);
		vxTimeBaseGet(&startTimeH1,&startTimeL1);
		taskDelay(delayTime);
		sysGetTime(&endTimeH,&endTimeL);
		vxTimeBaseGet(&endTimeH1,&endTimeL1);
		
		timespend = sysSpendTimeGet(startTimeH,startTimeL,endTimeH,endTimeL);
		timespend1 = sysSpendTimeGet(startTimeH1,startTimeL1,endTimeH1,endTimeL1);
		
		
		timespend = timespend/fpgaTimerClkFreq;
		printf("timespend = %f s, timespend1 = %f s\n",timespend,timespend1);
	}
	return OK;
}
