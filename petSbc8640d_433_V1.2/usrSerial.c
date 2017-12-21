/* usrSerial.c - serial device configuration file */

/* Copyright 1984-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
01d,30apr04,dat  fix itos code warnings, remove unnecessary casts (96786)
01c,24feb99,pr   added control for PC_CONSOLE in IO initialization (SPR#23075)
01b,11oct97,ms   use itos instead of sprintf to break dependecy on fioLib
01a,09oct97,ms   taken from usrConfig.c
*/

/*
DESCRIPTION

User configurable serial device intialization.
*/

#define	TY_NAME_BASE	"/tyCo/"
extern int ns16550Pollprintf(char* pchar);
int fd_uart2;
/******************************************************************************
*
* itos - given an integer, return a string representing it.
*/

static char * itos (unsigned int val)
    {
    static char str [20];
    char * pChar;

    if (val == 0)
	return "0";

    pChar = & str[19];
    *pChar = '\0';

    while (val != 0)
	{
	*--pChar = '0' + (val % 10);
	val /= 10;
	}

    return pChar;
    }

/******************************************************************************
*
* usrSerialInit - initialize the serial ports
*/

STATUS usrSerialInit (void)
    {
    char tyName [20];
    unsigned int ix;
    
    ttyDrv();					/* install console driver */

    if ((NUM_TTY > 0))
	{
	for (ix = 0; ix < NUM_TTY; ix++)	/* create serial devices */
	    {
#if     (defined(INCLUDE_WDB) && defined(INCLUDE_WDB_COMM_SERIAL))
	    if (ix == WDB_TTY_CHANNEL)		/* don't use WDBs channel */
		continue;
#endif
	    tyName[0] = '\0';
	    strcat (tyName, TY_NAME_BASE);
	    strcat (tyName, itos (ix));
	    ttyDevCreate (tyName, sysSerialChanGet(ix), 512, 512);

#if  (!(defined(INCLUDE_PC_CONSOLE)))
	    
#ifndef SHARE_UART
	    if ((ix == CONSOLE_TTY))		/* init the tty console */
		{
		consoleFd = open (tyName, O_RDWR, 0);
		ioctl (consoleFd, FIOBAUDRATE, CONSOLE_BAUD_RATE);
		ioctl (consoleFd, FIOSETOPTIONS, OPT_TERMINAL);
		}
#else
	   // if((sysGetCpuID()==0)&&(ix == CONSOLE_TTY))		/* init the tty console */
	    if(sysGetCpuID()==0)
	    {
	    	if(ix == CONSOLE_TTY)
	    	{
//	  		ns16550Pollprintf("cpuA  init tty console\n");
	    		consoleFd = open (tyName, O_RDWR, 0);
	    		ioctl (consoleFd, FIOBAUDRATE, CONSOLE_BAUD_RATE);
	    		ioctl (consoleFd, FIOSETOPTIONS, OPT_TERMINAL);
	    	
	    	}
	    	else if(ix==2)
	    	{
	    		fd_uart2  = open (tyName, O_RDWR, 0);
	    		(void) ioctl (fd_uart2, FIOBAUDRATE, CONSOLE_BAUD_RATE);
	    		(void) ioctl (fd_uart2, FIOSETOPTIONS, OPT_TERMINAL);  
	    		
	    	    (void) ioctl (fd_uart2, FIOSETOPTIONS, OPT_RAW);
	    	    (void) ioctl(fd_uart2,FIOFLUSH,0); 
	    	    
	    	//	close(fd);
	    	}
	    		
	    }
	    if((sysGetCpuID()!=0)&&(ix == 2))		/* init the tty console */
	    {
//	    	ns16550Pollprintf("cpuB/C/D  init tty console\n");
	    	consoleFd = open (tyName, O_RDWR, 0);
	    	(void) ioctl (consoleFd, FIOBAUDRATE, CONSOLE_BAUD_RATE);
	    	(void) ioctl (consoleFd, FIOSETOPTIONS, OPT_TERMINAL);	    	
	    }	    
#endif
#endif /* INCLUDE_PC_CONSOLE */

	    }
	}

    ioGlobalStdSet (STD_IN,  consoleFd);
    ioGlobalStdSet (STD_OUT, consoleFd);
    ioGlobalStdSet (STD_ERR, consoleFd);
    
    return (OK);
    }
