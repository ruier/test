
#define RBR     0x00    /* receiver buffer register */
#define THR     0x00    /* transmit holding register */
#define DLL     0x00    /* divisor latch */
#define IER     0x01    /* interrupt enable register */
#define DLM     0x01    /* divisor latch(MS) */
#define IIR     0x02    /* interrupt identification register */
#define FCR     0x02    /* FIFO control register */
#define LCR     0x03    /* line control register */
#define MCR     0x04    /* modem control register */
#define LSR     0x05    /* line status register */
#define MSR     0x06    /* modem status register */
#define SCR     0x07    /* scratch register */

#define DUART_BASE_ADDR 0xe1000600
#define REG_INTERVAL  4

UINT8 uartTestRecStop=0;


extern UINT32 sysUartClkFreqGet();
void uartTestInit1(UINT8 num)
{
    /* set baud */
	UINT32 regBase;
	regBase=DUART_BASE_ADDR+0x100*num;
	
    *(char*)(regBase+LCR*REG_INTERVAL)=0x83;
    printf("0x%x\n",bspRs485ClkFreqGet()/(8*9600));
	*(char*)(regBase+DLL*REG_INTERVAL)= (bspRs485ClkFreqGet()/(8*9600))&0xff;
	*(char*)(regBase+DLM*REG_INTERVAL)= ((bspRs485ClkFreqGet()/(8*9600))&0xff00)>>8;
    printf("0x%x\n",(char*)(regBase+LCR*REG_INTERVAL));
	*(char*)(regBase+LCR*REG_INTERVAL)=0x03;
}


int uartTestOutputchar
    (
    char            outChar ,/* char to send */
    UINT8 num
    )
    {
    UINT8 pollStatus;
	UINT32 regBase;
	regBase=DUART_BASE_ADDR+0x100*num;
	
    pollStatus = *(char*)(regBase+LSR*REG_INTERVAL);
    printf("0x%x=0x%x\n",regBase+LSR*REG_INTERVAL,pollStatus);
    /* is the transmitter ready to accept a character? */

    if ((pollStatus & 0x20) == 0x00)
	{
	    return(-1);
	}
    *(char*)(regBase+THR*REG_INTERVAL) = outChar;
 
    return(0);
    
}

void uartTestSend(UINT8 data,UINT8 num)
{
	UINT32 regBase;
	regBase=DUART_BASE_ADDR+0x100*num;
	printf("0x%x\n",DUART_BASE_ADDR+0x100*num);
	*(char*)(regBase)=data;
	 
}

int uartTestprintf(char* pchar,UINT8 num)
{
    while(*(pchar)!='\0')
    {
        if (*pchar == '\n')
        {
            while (uartTestOutputchar('\r',num)!=0);
			while (uartTestOutputchar('\n',num)!=0);
       	}
		else
			while (uartTestOutputchar(*pchar,num)!=0);
	    pchar++; 
    }
   

	return 0;
}


int uartTestRev(UINT8 num)
{
	UINT8 pollStatus;
	char inChar;
	UINT32 regBase;
	regBase=DUART_BASE_ADDR+0x100*num;
	
	 /* is the transmitter ready to accept a character? */   
	 while(!uartTestRecStop) 
	 {
		 pollStatus = *(char*)(regBase+LSR*REG_INTERVAL);
		 if ((pollStatus & 0x01) != 0x00)
		 {
			 /*printf("pollStatus = 0x%x\n",pollStatus);*/
			 inChar=*(char*)(regBase+RBR*REG_INTERVAL);
			 printf("uartTestRev:0x%x\n",inChar);
		 }
	 }
	    return(0);
}

#if 0

int testUartSend(UINT8 num)
{
	UINT32 regBase;
	UINT8 data=0;
	regBase=DUART_BASE_ADDR+0x100*num;
	while(1)
	{
		*(char*)(regBase)=data;
		data++;
		taskDelay(60);
	}
}
#endif

