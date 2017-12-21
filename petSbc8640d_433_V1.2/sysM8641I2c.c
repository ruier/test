/* sysMotI2c.c - I2C Driver Source Module */

/* includes */

#include <vxWorks.h>		/* vxWorks generics */
#include "config.h"
#include <ioLib.h>			/* input/output generics */
#include <blkIo.h>			/* block input/output specifics */
#include <semLib.h>			/* semaphore operations */
#include <cacheLib.h>			/* cache control */
#include <intLib.h>			/* interrupt control */
#include <semLib.h>
#include <tickLib.h>

#include "sysM8641I2c.h"	/* Mpc85xx I2C Driver Header Module */
#include <stdio.h>
#include <logLib.h>
#include <stdlib.h>

/* defines */
#define I2C_DRIVER_DEBUG
#undef I2C_DRIVER_DEBUG

#ifdef I2C_DRIVER_DEBUG
#define I2C_DBG_MSG(fmt,a,b,c,d,e,f)  logMsg(fmt,a,b,c,d,e,f)


#undef LOCAL
#define LOCAL

#else /* I2C_DRIVER_DEBUG1 */
#define I2C_DBG_MSG(fmt,a,b,c,d,e,f)
#endif /* I2C_DRIVER_DEBUG1 */


/* externals */
extern  void sysMsDelay(UINT delay)  ;                 /* length of time in MS to delay */
/* locals */
int i2cIoctl (UINT8 ioctlflg,UINT32 address,UINT8 bdata1,UINT8 bdata2) ;
int i2cRead(UINT32 baseAddr,UINT8 deviceAddress,UINT32 startBlk,UINT8 numBlks,UINT8 *pBuf,UINT8 mode);
int i2cWrite(UINT32 baseAddr,UINT8 deviceAddress,UINT32 startBlk,UINT8 numBlks,UINT8 *pBuf,UINT8 mode);

LOCAL void sysMpc85xxMsDelay(UINT mSeconds);

LOCAL int i2cCycleMpc85xxWrite (UINT32, UINT8);
LOCAL int i2cCycleMpc85xxStart (UINT32);
LOCAL int i2cCycleMpc85xxStop (UINT32);
LOCAL int i2cCycleMpc85xxRead (UINT32,UINT8*,UINT8,UINT8);
LOCAL int i2cCycleMpc85xxAckIn (UINT32);
LOCAL int i2cCycleMpc85xxKnownState (UINT32);
LOCAL UINT8 i2cInByte (UINT32);
LOCAL void i2cOutByte (UINT32,UINT8);


SEM_ID gSemI2c=NULL;
/* i2cIoctl - perform I2C read/write operations*/

int i2cIoctl
(
    UINT8 ioctlflg,  /* input/output control flag */
		      /* 0, write */
		      /* 1, read */
		      /* 2, read/modify/write (ORing) */
		      /* 3, read/modify/write (ANDing) */
		      /* 4, read/modify/write (AND/ORing) */
    UINT32  address,	  /* address of device register to be operated upon */
    UINT8   bdata1,	  /* data item 1 for read/write operation */
    UINT8   bdata2	  /* data item 2 for read/write operation */
)
{
	UINT8 u8temp;
    I2C_DBG_MSG("i2cIoctl: adrs - 0x%x.\n", address,2,3,4,5,6);
    sysMpc85xxMsDelay(1);
    if ( ioctlflg == I2C_IOCTL_WR )	/* write */
	{
    	i2cOutByte(address, bdata1);
   	}
    else if ( ioctlflg == I2C_IOCTL_RD ) /* read */
	{
    	bdata1 = i2cInByte(address);
	}
    else if ( ioctlflg == I2C_IOCTL_RMW_OR ) /* ORing */
	{
    	u8temp = i2cInByte(address);
    	u8temp |= bdata1;
    	sysMpc85xxMsDelay(1);

    	i2cOutByte(address, u8temp);
	}
    else if ( ioctlflg == I2C_IOCTL_RMW_AND ) /* ANDing */
	{
    	u8temp = i2cInByte(address);
    	u8temp &= bdata1;
    	sysMpc85xxMsDelay(1);
    	i2cOutByte(address, u8temp);
	}
    else if ( ioctlflg == I2C_IOCTL_RMW_AND_OR ) /* AND/ORing */
	{
    	u8temp = i2cInByte(address);
    	u8temp &= bdata1;
    	u8temp |= bdata2;
    	sysMpc85xxMsDelay(1);
    	i2cOutByte(address, u8temp);
	}
    sysMpc85xxMsDelay(1);
    return(bdata1);
}


/* i2cRead - read blocks from I2C(NOW it is test to read a byte*/

LOCAL int i2cCycleMpc85xxReStart(UINT32 baseAddr);
int i2cRead
(
    UINT32 baseAddr,
    UINT8  deviceAddress,	/* Device's I2C bus address */
    UINT32 startBlk,	/* starting block to read or register to read */
    UINT8 numBlks,	/* number of blocks to read or single/double byte register */
    UINT8 *pBuf,				/* pointer to buffer to receive data */
    UINT8 mode
)
{
    UINT8 statusVariable=0,i;		
    int localStatus=OK;

    if(baseAddr == 0)
    {
		logMsg("baseAddr is inval!!.\n", 1,2,3,4,5,6);
        return ERROR ;
	}
    if ( !numBlks ) return(ERROR);
    
    if(gSemI2c==NULL) gSemI2c = semBCreate(SEM_Q_FIFO, SEM_FULL);
    semTake(gSemI2c,WAIT_FOREVER);
    i2cCycleMpc85xxStop(baseAddr);
   
	if ( i2cCycleMpc85xxKnownState(baseAddr) )
	{
		statusVariable = I2C_ERROR_KNOWN_STATE;
	    goto errorEnd ;
	}
	
	if ( i2cCycleMpc85xxStart(baseAddr) )
	{
	    statusVariable = I2C_ERROR_CYCLE_START;
	    goto errorEnd ;
	}
	
	/* device address */
	if ( i2cCycleMpc85xxWrite(baseAddr,deviceAddress&0xfe) )
	{
	    statusVariable = I2C_ERROR_CYCLE_WRITE;
	    goto errorEnd ;
	}
	if ( i2cCycleMpc85xxAckIn(baseAddr) )
	{
	    statusVariable = I2C_ERROR_CYCLE_ACKIN;
	    goto errorEnd ;
	}
	
	if(mode ==0)
	{
		/* address register pointer */
		if ( i2cCycleMpc85xxWrite(baseAddr,startBlk&0xff) )
		{
			statusVariable = I2C_ERROR_CYCLE_WRITE;
			goto errorEnd ;
		}
		if ( i2cCycleMpc85xxAckIn(baseAddr) )
		{
			statusVariable = I2C_ERROR_CYCLE_ACKIN;
			goto errorEnd ;
		}
	}
	else
	{
		/* address register pointer */
		if ( i2cCycleMpc85xxWrite(baseAddr,(startBlk>>18)&0x3f) )
		{
			statusVariable = I2C_ERROR_CYCLE_WRITE;
			goto errorEnd ;
		}
		if ( i2cCycleMpc85xxAckIn(baseAddr) )
		{
			statusVariable = I2C_ERROR_CYCLE_ACKIN;
			goto errorEnd ;
		}
		if ( i2cCycleMpc85xxWrite(baseAddr,(startBlk>>10)&0xff) )
		{
			statusVariable = I2C_ERROR_CYCLE_WRITE;
			goto errorEnd ;
		}
		if ( i2cCycleMpc85xxAckIn(baseAddr) )
		{
			statusVariable = I2C_ERROR_CYCLE_ACKIN;
			goto errorEnd ;
		}
		if ( i2cCycleMpc85xxWrite(baseAddr,(startBlk>>2)&0xff) )
		{
			statusVariable = I2C_ERROR_CYCLE_WRITE;
			goto errorEnd ;
		}
		if ( i2cCycleMpc85xxAckIn(baseAddr) )
		{
			statusVariable = I2C_ERROR_CYCLE_ACKIN;
			goto errorEnd ;
		}
		
	}
#if 0
	if ( i2cCycleMpc85xxStop(baseAddr) )
	{
	    statusVariable = I2C_ERROR_CYCLE_STOP;
	    goto errorEnd ;
	}

	sysMpc85xxMsDelay (1);

	if ( i2cCycleMpc85xxStart(baseAddr) )
	{
	    statusVariable = I2C_ERROR_CYCLE_START;
	    goto errorEnd ;
	}
#else
	sysMpc85xxMsDelay (1);
	if ( i2cCycleMpc85xxReStart(baseAddr) )
	{
	    statusVariable = I2C_ERROR_CYCLE_START;
	    goto errorEnd ;
	}
	sysMpc85xxMsDelay (1);
#endif
	
	/* device address */
	if ( i2cCycleMpc85xxWrite(baseAddr,deviceAddress&0xfe|0x1) )
	{
	    statusVariable = I2C_ERROR_CYCLE_WRITE;
	    goto errorEnd ;
	}
	if ( i2cCycleMpc85xxAckIn(baseAddr) )
	{
	    statusVariable = I2C_ERROR_CYCLE_ACKIN;
	    goto errorEnd ;
	}
	
	for(i=0;i<numBlks;i++)
	{
		UINT8 txak=1;
		if(i>(numBlks-2)) txak=0; /*do not sent ack*/
	    if ( i2cCycleMpc85xxRead(baseAddr,pBuf+i,txak,i))
	    {
	    	statusVariable = I2C_ERROR_CYCLE_READ;
	    	goto errorEnd ;
	    }
	}
	
	if ( i2cCycleMpc85xxStop(baseAddr) )
	{
		statusVariable = I2C_ERROR_CYCLE_STOP;
		goto errorEnd ;
	}
	
errorEnd:
    /* Leave the I2C bus in a known state. */
	i2cCycleMpc85xxKnownState(baseAddr);
    /* Return the appropriate status. */
    if ( statusVariable != 0 )
	{
    	logMsg("ERR!statusVariable- 0x%x\n",statusVariable,2,3,4,5,6);
    	localStatus = ERROR ;
	}
    else  
    	localStatus	= OK ;
    
    semGive(gSemI2c);
    return(localStatus);
}


/* i2cWrite - read blocks from I2C;NOW it is only test to write one byte*/

int i2cWrite
(
	UINT32 baseAddr,
	UINT8  deviceAddress,	/* Device's I2C bus address */
	UINT32 startBlk,	/* starting block to read or register to read */
	UINT8 numBlks,	/* number of blocks to read or single/double byte register */
	UINT8 *pBuf	,		/* pointer to buffer of send data */
	UINT8 mode
)
{
    UINT8 statusVariable=0,*pWriteData,i;	
	int localStatus = OK ;
	if(baseAddr == 0)
    {
	    logMsg("i2cWrite:baseAddr is inval!!.\n", 1,2,3,4,5,6);
        return ERROR ;
	}
    if ( !numBlks )return(ERROR);
   
/* Initialize pointer to caller's write data buffer. */
    if(gSemI2c==NULL) gSemI2c = semBCreate(SEM_Q_FIFO, SEM_FULL);
    semTake(gSemI2c,WAIT_FOREVER);
	pWriteData = pBuf;
	
	i2cCycleMpc85xxStop(baseAddr);
	//printf("1\n");
	if ( i2cCycleMpc85xxKnownState(baseAddr) )
	{
	    statusVariable = I2C_ERROR_KNOWN_STATE;
	    goto errorEnd ;
	}
	//printf("2\n");
	if ( i2cCycleMpc85xxStart(baseAddr) )
	{
	    statusVariable = I2C_ERROR_CYCLE_START;
	    goto errorEnd ;
	}
	//printf("3\n");
	/* device address */
	if ( i2cCycleMpc85xxWrite(baseAddr,deviceAddress&0xfe))
	{
	    statusVariable = I2C_ERROR_CYCLE_WRITE;
	    goto errorEnd ;
	}
	//printf("4\n");
	if ( i2cCycleMpc85xxAckIn(baseAddr) )
	{
	    statusVariable = I2C_ERROR_CYCLE_ACKIN;
	    goto errorEnd ;
	}
	//printf("5\n");
	/* address register pointer */
	if(mode == 0)
	{
		if ( i2cCycleMpc85xxWrite(baseAddr,(UINT8)(startBlk&0xff)) )
		{
		    statusVariable = I2C_ERROR_CYCLE_WRITE;
		    goto errorEnd ;
		}
		if ( i2cCycleMpc85xxAckIn(baseAddr) )
		{
			statusVariable = I2C_ERROR_CYCLE_ACKIN;
			goto errorEnd ;
		}
	}
	else
	{
		/* address register pointer */
		if ( i2cCycleMpc85xxWrite(baseAddr,(startBlk>>18)&0x3f) )
		{
			statusVariable = I2C_ERROR_CYCLE_WRITE;
			goto errorEnd ;
		}
		sysMpc85xxMsDelay(1);
		if ( i2cCycleMpc85xxAckIn(baseAddr) )
		{
			statusVariable = I2C_ERROR_CYCLE_ACKIN;
			goto errorEnd ;
		}
		if ( i2cCycleMpc85xxWrite(baseAddr,(startBlk>>10)&0xff) )
		{
			statusVariable = I2C_ERROR_CYCLE_WRITE;
			goto errorEnd ;
		}
		sysMpc85xxMsDelay(1);
		if ( i2cCycleMpc85xxAckIn(baseAddr) )
		{
			statusVariable = I2C_ERROR_CYCLE_ACKIN;
			goto errorEnd ;
		}
		if ( i2cCycleMpc85xxWrite(baseAddr,(startBlk>>2)&0xff) )
		{
			statusVariable = I2C_ERROR_CYCLE_WRITE;
			goto errorEnd ;
		}
		sysMpc85xxMsDelay(1);
		if ( i2cCycleMpc85xxAckIn(baseAddr) )
		{
			statusVariable = I2C_ERROR_CYCLE_ACKIN;
			goto errorEnd ;
		}		
	}
	sysMpc85xxMsDelay(10);
//	printf("6\n");

	
	for(i=0;i<numBlks;i++)
	{
		//printf("%d:0x%x\n",i,*(pWriteData+i));
		sysMpc85xxMsDelay(10);
		if ( i2cCycleMpc85xxWrite(baseAddr,*(pWriteData+i)) )
		{
			statusVariable = I2C_ERROR_CYCLE_WRITE ;
			goto errorEnd ;
		}
		if ( i2cCycleMpc85xxAckIn(baseAddr))
		{
			statusVariable = I2C_ERROR_CYCLE_ACKIN;
			goto errorEnd ;
		}
		sysMpc85xxMsDelay(10);
		
	}
	
	if ( i2cCycleMpc85xxStop(baseAddr))
	{
	    statusVariable = I2C_ERROR_CYCLE_STOP;
	    goto errorEnd ;
	}

errorEnd:
    /* Leave the I2C bus in a known state. */
    i2cCycleMpc85xxKnownState(baseAddr);
   /* Return the appropriate status. */
    if (statusVariable != 0 )
	{
    	logMsg("statusVariable- 0x%x\n",statusVariable,2,3,4,5,6);
    	localStatus = ERROR ;
	}
    else 
    	localStatus	= OK ;
    semGive(gSemI2c);
    return localStatus ;
}

LOCAL int i2cCycleMpc85xxReStart(UINT32 baseAddr)
{
    if (baseAddr == 0)  return (ERROR);
	i2cIoctl(I2C_IOCTL_RMW_OR,
			(UINT32)(baseAddr+MPC85XX_I2C_CONTROL_REG),
			(MPC85XX_I2C_CONTROL_REG_RSTA),0);
    return(OK);
}


/* i2cCycleMpc85xxStart - perform I2C "start" cycle*/

LOCAL int i2cCycleMpc85xxStart(UINT32 baseAddr)
{
    UINT8 timeOut=30,statusReg = 0;   
  //  ULONG tick;
  
    if (baseAddr == 0)  return (ERROR);

   // tick=tickGet();

    while(timeOut)
	{
        statusReg = i2cIoctl(I2C_IOCTL_RD,(UINT32)(baseAddr+MPC85XX_I2C_STATUS_REG), 0, 0);
        if ( !(statusReg & MPC85XX_I2C_STATUS_REG_MBB))   break;
       // if (tickGet() != tick)
        {
        	//tick = tickGet();
        	timeOut--;
        }
	}
    if ( !timeOut)  return(ERROR);

    /*
     * since this is the first time through, generate a START(MSTA) and
     * place the I2C interface into a master transmitter mode(MTX).
     */
    i2cIoctl(I2C_IOCTL_RMW_OR,
            (UINT32)(baseAddr+MPC85XX_I2C_CONTROL_REG),
            (MPC85XX_I2C_CONTROL_REG_MTX |
             MPC85XX_I2C_CONTROL_REG_MSTA),0);
    return(OK);
}



/* i2cCycleMpc85xxStop - perform I2C "stop" cycle*/

LOCAL int i2cCycleMpc85xxStop(UINT32  baseAddr)
{
    if (baseAddr == 0) return (ERROR);    
        /*
     * turn off MSTA bit(which will generate a STOP bus cycle)
     * turn off MTX bit(which places the MPC85XX interface into receive mode
     * turn off TXAK bit(which allows 9th clock cycle acknowledges)
     */
    sysMpc85xxMsDelay(1);
    i2cIoctl(I2C_IOCTL_RMW_AND,
            (UINT32)(baseAddr+MPC85XX_I2C_CONTROL_REG),
            ((UINT8)~(MPC85XX_I2C_CONTROL_REG_MTX  |
               MPC85XX_I2C_CONTROL_REG_MSTA |
               MPC85XX_I2C_CONTROL_REG_TXAK)), 0);
    return(OK);
}


/* i2cCycleMpc85xxRead - perform I2C "read" cycle*/

LOCAL int i2cCycleMpc85xxRead
(
    UINT32 baseAddr,
    UINT8 *pReadDataBuf,    /* pointer to read data buffer */
    UINT8 ack    ,/* 0 = don't ack, 1 = ack */
    UINT8 mode
)
{
    UINT8 readData = 0;
    if (baseAddr == 0)return (ERROR) ;
    
    /*
     * place the I2C interface into receive mode(MTX=0) and set the interface
     * to NOT acknowledge(TXAK=1) the incoming data on the 9th clock cycle.
     * this is required when doing random reads of a I2C device.
     */
    if (!ack)
    {
        /* Don't send master ack. */
        i2cIoctl(I2C_IOCTL_RMW_AND_OR,
            (UINT32)(baseAddr+MPC85XX_I2C_CONTROL_REG),
            ((UINT8)~MPC85XX_I2C_CONTROL_REG_MTX),
              MPC85XX_I2C_CONTROL_REG_TXAK);
	}
    else
	{
        /* Send master ack. */
        i2cIoctl(I2C_IOCTL_RMW_AND_OR,
            (UINT32)(baseAddr+MPC85XX_I2C_CONTROL_REG),
            ((UINT8)~(MPC85XX_I2C_CONTROL_REG_MTX|MPC85XX_I2C_CONTROL_REG_TXAK)),0);
	}
    sysMpc85xxMsDelay(1);

    /* do a "dummy read".  this latches the data off the bus. */
    if(mode == 0) i2cIoctl(I2C_IOCTL_RD,(UINT32)(baseAddr+MPC85XX_I2C_DATA_REG),0, 0);

    /* now do the actual read, make this one count */
    sysMpc85xxMsDelay(1);
    readData = i2cIoctl(I2C_IOCTL_RD,(UINT32)(baseAddr+MPC85XX_I2C_DATA_REG),0, 0);
    *pReadDataBuf = readData;
    return(OK);
}

/* i2cCycleMpc85xxWrite - perform I2C "write" cycle*/

LOCAL int i2cCycleMpc85xxWrite
(
    UINT32 baseAddr,
    UINT8   writeData   /* character to write */
)
{
    if (baseAddr == 0) return ERROR ;
    /*
     * write the requested data to the data register, which will cause
     * it to be transmitted on the I2C bus.
     */
    i2cIoctl(I2C_IOCTL_WR,(UINT32)(baseAddr+MPC85XX_I2C_DATA_REG),writeData, 0);
    return(OK);
}

/* i2cCycleMpc85xxAckIn - perform I2C "acknowledge-in" cycle*/

LOCAL int i2cCycleMpc85xxAckIn(UINT32 baseAddr)
{
    UINT8 statusReg = 0,timeOut=20;
    //ULONG tick;
      
    
    if (baseAddr == 0) return ERROR ;
    
    /*
     * wait until an *internal* device interrupt has been generated, then
     * clear it.  if it is not received, return with an error.
     * we are polling, so NO processor interrupt is generated.
     */
    //tick=tickGet();
    while(timeOut)
    {
        statusReg = i2cIoctl(I2C_IOCTL_RD,(UINT32)(baseAddr+MPC85XX_I2C_STATUS_REG), 0, 0);
        if ( statusReg & MPC85XX_I2C_STATUS_REG_MIF )
        {
            i2cIoctl(I2C_IOCTL_RMW_AND,
                    (UINT32)(baseAddr+MPC85XX_I2C_STATUS_REG),
                    ((UINT8)~MPC85XX_I2C_STATUS_REG_MIF), 0);
            break;
        }
       // if (tickGet() != tick)
        {
        	//tick = tickGet();
        	timeOut--;
        }
	}
    if ( !timeOut )return(ERROR);
    return(OK);
}


/* i2cCycleMpc85xxKnownState - initialize the I2C bus to a known state*/

LOCAL int i2cCycleMpc85xxKnownState(UINT32 baseAddr)
{
    STATUS status=OK;
    UINT8   timeOut=200,statusReg;
   /*ULONG tick;*/

    if (baseAddr == 0) return (ERROR) ;

    /*
     * wait until the I2C bus is free.  if it doesn't become free
     * within a *reasonable* amount of time, exit with an error.
     */
   // tick=tickGet();
    while(timeOut)
    {
        statusReg = i2cIoctl(I2C_IOCTL_RD,(UINT32)(baseAddr+MPC85XX_I2C_STATUS_REG), 0, 0);
        if ( !(statusReg & MPC85XX_I2C_STATUS_REG_MBB) )
        {
            status = OK;
            break;
        }
       // if (tickGet() != tick)
        {
        	//tick = tickGet();
        	timeOut--;
        }
    }

        /*
         * re-initialize the I2C if the BUS BUSY does not clear
         * after trying half the *reasonable* amount of reads of the
         * status register.
         */
	i2cIoctl(I2C_IOCTL_RMW_AND,
	(UINT32)(baseAddr+MPC85XX_I2C_CONTROL_REG),
			((UINT8)~MPC85XX_I2C_CONTROL_REG_MSTA), 0);

    if ( !timeOut)    status = ERROR;
    return(status);
 }


/* i2cCycleMpc85xxDelay - perform interface's I2C delay routine*/

LOCAL void sysMpc85xxMsDelay(UINT mSeconds)
{
	sysMsDelay(mSeconds);
}


/* i2cInByte - reads a byte from I2C Regs*/

LOCAL UINT8 i2cInByte(UINT32 dataPtr)
{
    UINT8 *ptr = (UINT8*)dataPtr ;
    return(*ptr);
}



/* i2cOutByte - writes a byte to an I2C register*/

LOCAL void i2cOutByte(UINT32 dataPtr,UINT8 data)
{
    UINT8 * ptr = (UINT8*)dataPtr ;
    *ptr = data ;
}

