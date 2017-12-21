#include <unistd.h>
#include <fioLib.h>

#define UART_DEMO_RMU_DBG_ON
/*#undef UART_DEMO_RMU_DBG_ON*/

#ifdef UART_DEMO_RMU_DBG_ON

#define UART_DEMO_RMU_DBG_MSG(fmt,a,b,c,d,e,f)  logMsg(fmt,a,b,c,d,e,f)
#define STATIC  

#else /* UART_DEMO_RMU_DBG_ON*/

#define UART_DEMO_RMU_DBG_MSG(fmt,a,b,c,d,e,f)
#define STATIC static

#endif /* UART_DEMO_RMU_DBG_ON*/


#define MAX_BUFFER_LEN 256
#define OPT_TERMINAL1	(OPT_ECHO | OPT_CRMOD | OPT_TANDEM | \
			 OPT_MON_TRAP | OPT_7_BIT | OPT_ABORT | OPT_RAW)/*OPT_RAW)OPT_LINE*/



/*sp uartDemoWrite,2,0x30000000,9600*/

void uartDemoWrite(int chan,char *pBuffer,UINT32 baudRate)
{	
	int ttyFd,writeNum=0;
	char ttyname[64];
	/*fd_set  writeFs;*/


    sprintf(ttyname,"/tyCo/%d", chan);
    ttyFd = open (ttyname, O_RDWR, 0);/*|O_NONBLOCK*/
	if(ttyFd == ERROR)
	{
		printf("uartDemoOpen:unable to open %s",ttyname);
		//return ERROR;
		return;
	}
	
    (void) ioctl (ttyFd, FIOBAUDRATE, baudRate);
    (void) ioctl (ttyFd, FIOSETOPTIONS, OPT_RAW);
    (void) ioctl(ttyFd,FIOFLUSH,0); 
   
    /*   
	FD_SET(ttyFd, &writeFs); 
    select(ttyFd+1, NULL,&writeFs, NULL, NULL);
	if (FD_ISSET(ttyFd,&writeFs)) 
	*/
    while(1)
    {
    	writeNum = write(ttyFd,pBuffer,1);
    	taskDelay(60);
    }
	/*printf("write 0x%x;;writeNum=%d,len=%d\n", *pBuffer,writeNum,len);*/
	//close(ttyFd);
	//return OK;
}
/*uartDemoWriteS 2,0x20000000,9600*/
/* sp uartDemoWriteS 2,&cc,9600 */
STATUS uartDemoWriteS(int chan,char *pBuffer,UINT32 baudRate)
{	
	int ttyFd,writeNum=0;
	char ttyname[64];
	/*fd_set  writeFs;*/


    sprintf(ttyname,"/tyCo/%d", chan);
    ttyFd = open (ttyname, O_RDWR, 0);/*|O_NONBLOCK*/
	if(ttyFd == ERROR)
	{
		printf("uartDemoOpen:unable to open %s",ttyname);
		return ERROR;
	}
	
    (void) ioctl (ttyFd, FIOBAUDRATE, baudRate);
    (void) ioctl (ttyFd, FIOSETOPTIONS, OPT_RAW);
    (void) ioctl(ttyFd,FIOFLUSH,0); 
   
    /*   
	FD_SET(ttyFd, &writeFs); 
    select(ttyFd+1, NULL,&writeFs, NULL, NULL);
	if (FD_ISSET(ttyFd,&writeFs)) 
	*/
  //  while(1)
    {
    	writeNum = write(ttyFd,pBuffer,1);
    }
	/*printf("write 0x%x;;writeNum=%d,len=%d\n", *pBuffer,writeNum,len);*/
	close(ttyFd);
	return OK;
}
/*sp uartDemoRead,2,&aa,9600*/
	
void uartDemoRead(int chan,char *pBuffer,UINT32 baudRate)
{	
	int ttyFd,readNum;
	char ttyname[64];
	fd_set  readFs;
	sprintf(ttyname,"/tyCo/%d", chan);
	ttyFd = open (ttyname, O_RDWR|O_NOCTTY|O_NONBLOCK, 0); /*O_NONBLOCK no active*/
	if(ttyFd == ERROR)
	{
		printf("unable to open device\n");
		//return ERROR;
		return;
	}
	(void) ioctl (ttyFd, FIOSETOPTIONS, OPT_RAW);
	(void) ioctl(ttyFd,FIOFLUSH,0); /*«Âø’ ‰»Î ‰≥ˆª∫≥Â*/
	(void) ioctl (ttyFd, FIOBAUDRATE, baudRate);
	FD_SET(ttyFd, &readFs); 
	select(ttyFd+1, &readFs,NULL, NULL, NULL);
	while(1)
	{
		readNum = read(ttyFd,pBuffer,1);	
		printf("read 0x%x;;readNum=%d\n", *pBuffer,readNum);
		if(readNum==0) 
		{
			printf("read error!!\n");
			/*break;*/
		}
	}
//	close(ttyFd);
//	return readNum;
}

STATUS uartDemoInit(int chan,UINT32 baudRate)
{
	int ttyFd;
	char ttyname[64];

	sprintf(ttyname,"/tyCo/%d\n", chan);
	ttyFd = open (ttyname, O_RDWR, 0);
	if(ttyFd == ERROR)
	{
		printf("unable to open device\n");
		return ERROR;
	}
	(void) ioctl (ttyFd, FIOBAUDRATE, baudRate);
	(void) ioctl (ttyFd, FIOSETOPTIONS, OPT_RAW);
	(void) ioctl(ttyFd,FIOFLUSH,0); /*«Âø’ ‰»Î ‰≥ˆª∫≥Â*/
	close(ttyFd);
	
	return OK;
}




void uartRegReadAll(UINT32 baseAddr)
{
	UINT8 data,i,j;
	for(j=0;j<2;j++)
	{
		if(j==0) *(UINT8*)(baseAddr+3) &=(~(1<<7));
		else *(UINT8*)(baseAddr+3) |=(1<<7);
		for(i=0;i<8;i++)
		{
			data=*(UINT8*)(baseAddr+i);
			printf("%d:0x%x\n",i,data);
		}	
	}
}

static int gFd;

void uartTestR(UINT32 addr)
{
	UINT8 readNum;
	while(1)
	{
		readNum = read(gFd,(char *)addr,1);	
		printf("read 0x%x;;readNum=%d\n", *(UINT8*)addr,readNum);
		if(readNum==0) 
		{
			printf("read error!!\n");
			/*break;*/
		}
	}
	
}


void uartTestS(UINT32 addr)
{
	//UINT8 readNum;
	UINT8 data=0;
	while(1)
	{
		write(gFd,&data,1);
		data++;
	}
	
}

STATUS uartTestInit(UINT32 baudRate)
{
	//int ttyFd;
	//char ttyname[64];


	gFd = open ("/tyCo/2", O_RDWR, 0);
	if(gFd == ERROR)
	{
		printf("unable to open device\n");
		return ERROR;
	}
	(void) ioctl (gFd, FIOBAUDRATE, baudRate);
	(void) ioctl (gFd, FIOSETOPTIONS, OPT_RAW);
	(void) ioctl(gFd,FIOFLUSH,0); /*«Âø’ ‰»Î ‰≥ˆª∫≥Â*/
	
	return OK;
	
}



#if 0

void test(int chan)
{
	UINT8 data=0x5a;
	printf("chan=0x%x\n",chan);
	while(1) uartDemoWrite(chan,&data,1,9600);
}

UINT8 readStop=0;
STATUS readTest(int chan)
{	
	INT8 aa;
	printf("chan=0x%x\n",chan);
	while(1)
		{
		printf("befor read\n");
		uartDemoRead(chan,&aa,9600);
		printf("aa:0x%x\n",aa);
		
		}
}

#endif

#if 0
STATUS rs485t(UINT32 baudRate)
{
	INT32 serialID,serialID2 ;
	UINT8 uBufTemp[300];
	int i/*,j*/;
	serialID = open("/tyCo/2",O_RDWR,0);
	serialID2 = open("/tyCo/3",O_RDWR,0);

	sprintf("\n Serial Open %x %x",serialID,serialID2);
	
		
	(void) ioctl (serialID, FIOBAUDRATE, baudRate);
	(void) ioctl (serialID, FIOSETOPTIONS, OPT_RAW);
	(void) ioctl(serialID,FIOFLUSH,0); 
	
	(void) ioctl (serialID2, FIOBAUDRATE, baudRate);
	(void) ioctl (serialID2, FIOSETOPTIONS, OPT_RAW);
	(void) ioctl(serialID2,FIOFLUSH,0); 	

	for(i=0;i<20;i++)
		uBufTemp[i] = 0x55+i;

while(1)
	{
		write(serialID,&uBufTemp[0],4);
		taskDelay(1);
//		write(serialID2,&uBufTemp[0],4);		
//		sprintf("S");
	}

}
STATUS rs485r(UINT32 baudRate)
{
	INT32 serialID,serialID2 ;
	UINT8 uBufTemp[300];
	int i/*,j*/;
	UINT32 ulLength;
	
	UINT32 ulInsDataLength ;

	ulInsDataLength = 16;


	serialID = open("/tyCo/2",O_RDWR,0);
	serialID2 = open("/tyCo/3",O_RDWR,0);

	printf("\n Serial Open %x %x",serialID,serialID2);

	ioctl(serialID,FIOBAUDRATE,baudRate);
	ioctl(serialID,FIOFLUSH,0);	
	ioctl(serialID,FIOSETOPTIONS,OPT_RAW);

	ioctl(serialID2,FIOBAUDRATE,baudRate);
	ioctl(serialID2,FIOFLUSH,0);	
	ioctl(serialID2,FIOSETOPTIONS,OPT_RAW);	
	

while(1)
	{
		ulLength = fioRead(serialID2,&uBufTemp[0],ulInsDataLength);
		if(1){			printf("\nINSDataIni ");						
					for(i = 0;i<ulLength;i++)
				{
							printf("<%x>",uBufTemp[i]);
				}
			}
	}
}
STATUS rs485_ins(void)
{
	INT32 serialID=0,serialID2=0 ;
	UINT8 uBufTemp[300];
	int i/*,j*/;
	UINT32 ulLength;
	
	UINT32 ulInsDataLength ;

	ulInsDataLength = 86;

	serialID2 = open("/tyCo/3",O_RDWR,0);

	printf("\n Serial Open %x %x",serialID,serialID2);


	ioctl(serialID2,FIOBAUDRATE,1000000);
	ioctl(serialID2,FIOFLUSH,0);	
	ioctl(serialID2,FIOSETOPTIONS,OPT_RAW);	
	

while(1)
	{
		ulLength = fioRead(serialID2,&uBufTemp[0],ulInsDataLength);
		if(1){			printf("\nINS:");						
					for(i = 0;i<ulLength;i++)
				{
							printf("%x|",uBufTemp[i]);
				}
					printf("\n");
			}
	}
}

STATUS rs485_gps(void)
{
	INT32 serialID/*,serialID2*/ ;
	UINT8 uBufTemp[300];
	int i/*,j*/;
	UINT32 ulLength;
	
	UINT32 ulInsDataLength ;

	ulInsDataLength = 78;

	serialID = open("/tyCo/2",O_RDWR,0);

	printf("\n Serial Open %x ",serialID);


	ioctl(serialID,FIOBAUDRATE,1000000);
	ioctl(serialID,FIOFLUSH,0);	
	ioctl(serialID,FIOSETOPTIONS,OPT_RAW);	
	

while(1)
	{
		ulLength = fioRead(serialID,&uBufTemp[0],ulInsDataLength);
		if(1){			printf("\nGPSDataIn ");						
					for(i = 0;i<ulLength;i++)
				{
							printf("%c",uBufTemp[i]);
				}
					printf("\n");
			}
	}
}
STATUS rs485t_w(void)
{
	int i;
	*(UINT16*)0xe100020c=0x8300;
	*(UINT16*)0xe1000204=0x0000;
	*(UINT16*)0xe1000200=0x0c00;
	*(UINT16*)0xe100020c=0x0300;
	
while(1)
	{
	for(i=0x55;i<0xaa;i++)
		{
			*(UINT8*)0xe1000200=i;
			taskDelay(1);
			printf("S");
		}
	}
}
#endif
