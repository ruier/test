#include "vxWorks.h"
#include "bspBridgeDma8641.h"
#include "taskLib.h"
#include "stdio.h"
#include "bootFlash.h"

#define DMA_OPTIONS     MPC8641_DMA_DEF_OPTIONS
/* Attribute when source address is local */

#define lOCAL_SRC_ATTRIB_NOSNOOP      (DMA8641_SATR_SREADTTYPE_RD_NOSNOOP)
#define lOCAL_SRC_ATTRIB_SNOOP      (DMA8641_SATR_SREADTTYPE_RD_SNOOP)

#define lOCAL_DST_ATTRIB_NOSNOOP       (DMA8641_DATR_DWRITETTYPE_WR_NOSNOOP)
#define lOCAL_DST_ATTRIB_SNOOP       (DMA8641_DATR_DWRITETTYPE_WR_SNOOP)

#define RAPIDIO_SRC_ATTRIB      (DMA8641_SATR_SBPATMU_BYPASS|DMA8641_SATR_STFLOWLVL_HIGH|DMA8641_SATR_STRANSIT_SRIO|DMA8641_SATR_SREADTTYPE_ATMU_NRD|DMA8641_SATR_SRIO_ID(1))
#define RAPIDIO_DST_ATTRIB      (DMA8641_DATR_DBPATMU_BYPASS|DMA8641_DATR_DTFLOWLVL_HIGH|DMA8641_DATR_DTRANSIT_SRIO|DMA8641_DATR_DWRITETTYPE_ATMU_SWRITE|DMA8641_DATR_SRIO_ID(1))

STATUS sysGetTime(UINT32 *timeH,UINT32 *timeL);

int dmaFinished = 0;

void dmaIsr(void)
{
	dmaFinished = 1;
}
#if 0
void ddrDma(unsigned int src,unsigned int dst,unsigned int size)
{   	
	dmaFinished = 0;
	bspBridgeDmaDirectXfer(0,DMA_OPTIONS|(0x1<<9),                                /* channel options                            */
	(UINT64)((UINT32)src), 0, 0, 0x40000, /* source parameters                          */
	(UINT64)((UINT32)dst), 0, 0, 0x40000, /* destination parameters                     */
	 size,                              /* transfer size                              */
	(VOIDFUNCPTR)dmaIsr,               /* user routine called at the end of transfer */
	(void*)0);
	while(dmaFinished==0);
}
#endif

void ddrTrans(unsigned int src,unsigned int dst,unsigned int size)
{
	unsigned int i;
	for (i=0;i<size;i=i+4)
		{
		*(unsigned int*)(dst+i)=*(unsigned int*)(src+i);
		}
}
void readvxWorks(char * filename,UINT32 fileAddr)
{
	int fd,len,fileLen=0/*,cpuId*/;
	struct stat	   fileInfo;
	//cpuId = sysGetCpuID();
	if (stat (filename, &fileInfo) != OK)
	{
	    printErr("get file info failed\n");
	}
	else 
	{
		fileLen=fileInfo.st_size;	
	    fileLen=fileInfo.st_size;
	}
	fd=open (filename, O_RDWR, 0);	
	len=read(fd,(char*)fileAddr,fileLen);
	//printf("readvxWorks1 len=0x%x\n",len);
	if(((*(UINT8*)(FL_BASE_ADDR+EP_FLAG_OFFSET+1))&0xff) == 0x5a)
	{
		*(UINT32*)(FL_BASE_ADDR+CPUB_REG_BASE+FLLEN_OFFSET) = len;
		//printf("readvxWorks len=0x%x\n",len);
	}
		
	if(fd!=0) close(fd);
}

void filenameSet(char *string,UINT32 fnLenOffset, UINT32 fnOffset)
{
	int strLen;	

	*(UINT32*)fnLenOffset=0;
	
	strLen=strlen(string);	
	*(UINT32*)fnLenOffset=strLen;
	 while (strLen--)
	 {
	  char data;
	  data = *string; 
	  *(UCHAR*)((int)fnOffset)=data;
	  string++, fnOffset++;
	   }
	 *(UCHAR*)((int)fnOffset)=EOS;
}
void filenameGet(int offset)
{
	int strLen;
	char *string;
	strLen=*(UINT32*)0x10000000;
	string=(char*)offset;
	printf("string = %s,strLen=%d\n",string,strLen);
}
void queryTask()
{	
	static int	initialized_b=0;		/* Do this ONCE */
	char* bootFl;
	UINT beginTimeH,beginTimeL,endTimeH,endTimeL;
	float timeSpend;
	

	sysGetTime(&beginTimeH,&beginTimeL);
	if(((*(UINT32*)BOOTROM_TFFS_SYNC_REG) & 0x00FF0000) != 0x5a) printf("waiting for the bootfile name of EP ...\n");
	while(((*(UINT32*)BOOTROM_TFFS_SYNC_REG) & 0x00FF0000) != 0x005a0000)
	{

		if(((*(UINT8*)(FL_BASE_ADDR+EP_FLAG_OFFSET+1))&0xff) == 0x5a)
		{				
			if (!initialized_b)
			{
				initialized_b = TRUE;
				bootFl=(char*)(FL_BASE_ADDR+CPUB_REG_BASE+FLNAME_OFFSET);
				printf("B bootFile: %s\n",bootFl);
				readvxWorks(bootFl,FL_BASE_ADDR+FL_CPUB_BASE+FL_OFFSET);
				//*(UINT32*)(FL_BASE_ADDR+EP_FLAG_OFFSET)=*(UINT32*)(FL_BASE_ADDR+EP_FLAG_OFFSET)&0xfffffffe;
				*(UINT8*)(FL_BASE_ADDR+EP_FLAG_OFFSET+1) = 0;
				*(UINT8*)(BOOTROM_TFFS_SYNC_REG+1) = 0x5a;
				//printf("read cpuB bootFile: %s complete\n",bootFl);
			}
		}
		//printf("read 0x%x = 0x%x\n",BOOTROM_TFFS_SYNC_REG,*(UINT32*)BOOTROM_TFFS_SYNC_REG);
		sysGetTime(&endTimeH,&endTimeL);
		timeSpend = (endTimeH -beginTimeH )*4.0*1024*1024*1024 + (endTimeL-beginTimeL);
		timeSpend = timeSpend*40/1000000000.0;
		if(timeSpend>20)
		{
			UINT32 tffsSync=0;
			tffsSync = *(UINT32*)(BOOTROM_TFFS_SYNC_REG);
			if(((tffsSync>>8)&0xff) != 0x5a) printf("timeout:can't get bootfile name of CPU B\n");
			break;
		}
	}	
}

