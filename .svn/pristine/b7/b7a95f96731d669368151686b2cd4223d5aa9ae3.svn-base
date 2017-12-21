/* iicLib.c - IIC routines */

/*
********************************************************************************
   This source and object code has been made available to you by IBM on an
   AS-IS basis.

   IT IS PROVIDED WITHOUT WARRANTY OF ANY KIND, INCLUDING THE WARRANTIES OF
   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE OR OF NONINFRINGEMENT
   OF THIRD PARTY RIGHTS.  IN NO EVENT SHALL IBM OR ITS LICENSORS BE LIABLE
   FOR INCIDENTAL, CONSEQUENTIAL OR PUNITIVE DAMAGES.  IBMÆS OR ITS LICENSORÆS
   DAMAGES FOR ANY CAUSE OF ACTION, WHETHER IN CONTRACT OR IN TORT, AT LAW OR
   AT EQUITY, SHALL BE LIMITED TO A MAXIMUM OF $1,000 PER LICENSE.  No license
   under IBM patents or patent applications is to be implied by the copyright
   license.

   Any user of this software should understand that neither IBM nor its
   licensors will be responsible for any consequences resulting from the use
   of this software.

   Any person who transfers this source code or any derivative work must
   include the IBM copyright notice, this paragraph, and the preceding two
   paragraphs in the transferred software.

   Any person who transfers this object code or any derivative work must
   include the IBM copyright notice in the transferred software.

   COPYRIGHT   I B M   CORPORATION 2003
   LICENSED MATERIAL  -  PROGRAM PROPERTY OF  I B M"

********************************************************************************
\NOMANUAL
*/

/* Copyright 2004, 2006 Wind River Systems, Inc. */

/*
modification history
--------------------
012,08mar06,dr    Updated for AMCC440EP bsp.
01d,08sep04,dr    Additional documentation fixes for apigen
01c,07sep04,mdo   Documentation fixes for apigen
01b,03nov03,tr    add iicInit and minor changes
01a,15dec02,saw   created
*/

/*
DESCRIPTION
This library contains IIC read/write routines.
*/

/* includes */

#include "stdio.h"
#include "config.h"
#include "sysLib.h"
#include "time.h"

/* Return code */
 #define IIC_OK                    0
 #define IIC_BUSY                  1
 #define IIC_ERR                   2
 #define IIC_INV_OP                3
 #define IIC_DATA_ERR              4
 #define IIC_INIT_FAILED           8
 #define IIC_UNKNOWN_STATE         9
 #define IIC_INCOMPLETE_TRANSFER   0xA
 #define IIC_INVALID_BUS_STATE     0xB
 #define IIC_TRANSFER_ABORT        0xC
 #define IIC_INVALID_DATA_LENGTH   0xD
 #define IIC_STATUS_ERROR          0xE
 #define IIC_TIMEOUT               0xF

/* miscellaneous */
 #define IIC_PAGE_SIZE             0x10  

#ifndef IIC_FIFO_DEPTH
#define IIC_FIFO_DEPTH          4
#endif

LOCAL STATUS waitXferDone (UINT32 sec, UINT32 usec, UINT32 iicBaseAddr); 


/***************************************************************************
* iicInit - Init IIC device
*
* Init IIC device
*
* RETURNS: OK or ERROR
* 
* ERRNO
*/
STATUS iicInit
    (
    unsigned int iicBaseAddr,
    SYS_INFO *sysInfo
    )
    {
    unsigned char iicBusState;
    unsigned int  rc=0;
    int           iicClkDivisor=0;
    unsigned long opbSpeed;

    opbSpeed = sysInfo->freqPLB / (sysInfo->pllOpbDiv * 1000000);

    iicBusState = (sysInByte(iicBaseAddr + IIC_EXTSTS) & IIC_EXTSTS_BCS_MASK);
    switch (iicBusState) {
        case IIC_BUS_FREE:
            break; /* rc already set to the OK value */
        case IIC_IN_ERROR1_STATE:
        case IIC_IN_ERROR2_STATE:
        case IIC_SLAVE_SELECTED:  /* This state should not occur as IIC ctrl is never slave */
        case IIC_SLAVE_TRANSFER:  /* This state should not occur as IIC ctrl is never slave */
            rc = IIC_INVALID_BUS_STATE;
            sysOutByte(iicBaseAddr + IIC_CNTL,     0x81); /* Halt master transfer */
            sysOutByte(iicBaseAddr + IIC_MDCNTL , 0x40); /* Flush master data buffer */
            sysOutByte(iicBaseAddr + IIC_CNTL   , 0x10);
            break;
        case IIC_BUS_BUSY:
        case MASTER_XFER_STATE:
            rc = IIC_BUSY;
            break;
        case IIC_STATE_UNKNOWN:
            sysOutByte(iicBaseAddr + IIC_MDCNTL,  0xc2);
            rc = IIC_UNKNOWN_STATE;
            break;
        default: /* All possible IIC bus states have been processed by preceding "case" */
            break;
    }

    if (opbSpeed <= 70) iicClkDivisor=0x6;
    if (opbSpeed <= 60) iicClkDivisor=0x5;
    if (opbSpeed <= 50) iicClkDivisor=0x4;
    if (opbSpeed <= 40) iicClkDivisor=0x3;
    if (opbSpeed <= 30) iicClkDivisor=0x2;
    if (opbSpeed == 20) iicClkDivisor=0x1;
    if (iicClkDivisor == 0)
        return ERROR;

    if (rc== IIC_OK) 
        {

        sysOutByte(iicBaseAddr + IIC_LMADR,       0x00);     /* the dev_addr will be set in the write or read */
        sysOutByte(iicBaseAddr + IIC_STS,         0x08);     /* clear status */
        sysOutByte(iicBaseAddr + IIC_EXTSTS,      0x8f);     /* clear extended status */
        sysOutByte(iicBaseAddr + IIC_CLKDIV,iicClkDivisor);
        sysOutByte(iicBaseAddr + IIC_INTRMSK,        0);
        sysOutByte(iicBaseAddr + IIC_CNTL,           0);
        sysOutByte(iicBaseAddr + IIC_XTCNTLSS,    0xf0);
        sysOutByte(iicBaseAddr + IIC_MDCNTL,      0xc2);     /* 100khz setting*/
                                                          /* 4 bytes + start/stop x OPB clock to flush */
        }
    return (rc);
    }



/***************************************************************************
*
* iicRead - read data from an IIC device
*
* Read data from an IIC device. 
*
* RETURNS: OK on Success
*          ERROR otherwise
*
* ERRNO: N/A
*/

STATUS iicRead
    (
    UINT32  iicBaseAddr,          /* IIC controller base address  */
    UCHAR   devAddr,              /* Device address               */
    UINT32  count,                /* number of bytes to read      */
    UCHAR * data                  /* destination for data         */
    )
    {
    UCHAR           reg;             /* register                 */
    UCHAR           transferCnt;     /* Num of Bytes in transfer */

    /* Clear status. Flush Master Data Buffer. Set device address. */

    sysOutByte(iicBaseAddr + IIC_STS, (IIC_STS_SCMP | IIC_STS_IRQA));
    sysOutByte(iicBaseAddr + IIC_EXTSTS, (   IIC_EXTSTS_IRQP
                                            | IIC_EXTSTS_IRQD
                                            | IIC_EXTSTS_LA
                                            | IIC_EXTSTS_ERR_MASK));

    /* wait until device is ready */ 

    while ((sysInByte(iicBaseAddr + IIC_STS) & IIC_STS_PT) != 0);

    /* set command */

    sysOutByte(iicBaseAddr + IIC_MDCNTL,
               sysInByte(iicBaseAddr + IIC_MDCNTL) | IIC_MDCNTL_FMDB);
               
    /* set device address */               

    sysOutByte(iicBaseAddr + IIC_LMADR, devAddr);

   /* Start transfer */

    while(count != 0)
        {

        /* clear register */

        reg = 0x00;

        /* check count as we can only transfer 4 bytes at a time */

        if (count/IIC_FIFO_DEPTH != 0)
            {
            transferCnt = IIC_FIFO_DEPTH;
            count -= IIC_FIFO_DEPTH;
            }
        else
            {
            transferCnt = count % IIC_FIFO_DEPTH;
            count = 0;
            }

        /* Chain transfers if count <> 0. Last transfer not chained */

        if (count != 0)
            reg |= IIC_CNTL_CHT;

        switch (transferCnt)
            {
            case 1: reg |= IIC_CNTL_TCT_1;
                    break;
            case 2: reg |= IIC_CNTL_TCT_2;
                    break;
            case 3: reg |= IIC_CNTL_TCT_3;
                    break;
            case 4: reg |= IIC_CNTL_TCT_4;
                    break;
            }

        /* Set read bit and start transfer */

        sysOutByte(iicBaseAddr + IIC_CNTL, (reg | IIC_CNTL_RW | IIC_CNTL_PT));
    
        /* wait for device */

        if ( waitXferDone (1,10,iicBaseAddr) != OK )
            return (ERROR);

        /* Read in data from fifo */

        while (transferCnt > 0)
            {
            *data = sysInByte (iicBaseAddr + IIC_MDBUF);
            transferCnt--;
            data++;

            }

        } /* end while count */

    return (OK);
    }

/***************************************************************************
*
* iicWrite - write data to an IIC device
*
* Write data to an IIC device.
*
* RETURNS: OK on Success
*          ERROR otherwise
*
* ERRNO: N/A
*/
STATUS iicWrite
    (
    UINT32  iicBaseAddr,          /* IIC controller base address  */
    UCHAR   devAddr,              /* Device address               */
    UCHAR   devSubaddr,           /* Offset into device           */
    UINT32  count,                /* number of bytes to write     */
    UCHAR * data                  /* data source location         */
    )
    {
    UCHAR           reg;             /* register                 */
    UCHAR           transferCnt;     /* Num of Bytes in transfer */
    BOOL            firstPass;       /* fisrt pass flag          */
    
    INT32           numBytes = count;  /* number of bytes in this cycle */
   
    
    /* Do one page at a time. */
    
    while (numBytes >= 0) 
        {

        /* Figure out how many bytes to transfer for this page */
        
        if (numBytes < IIC_PAGE_SIZE )
            count = numBytes;
        else
            count = IIC_PAGE_SIZE ;

        /*
         * Clear status. Flush Master Data Buffer. Set device address.
         * Set device subaddress, which is the start byte to write. Since
         * the device subaddress is programmed in the master data
         * buffer, we need to up the input count by 1.
         */

        sysOutByte(iicBaseAddr + IIC_STS, (IIC_STS_SCMP | IIC_STS_IRQA));
        
        /* wait until device is ready */ 
        
        while ((sysInByte(iicBaseAddr + IIC_STS) & IIC_STS_PT) != 0);
        
        /* set command */
        
        sysOutByte(iicBaseAddr + IIC_MDCNTL,
                  sysInByte(iicBaseAddr + IIC_MDCNTL) | IIC_MDCNTL_FMDB);

        /* set device address */
        
        sysOutByte(iicBaseAddr + IIC_LMADR, devAddr);

        /* set offset */
        
        sysOutByte(iicBaseAddr + IIC_MDBUF, devSubaddr);

        
        count++;

        /* initialize first pass */
        
        firstPass = TRUE;

        /* Start transfer */
        
        while(count != 0)
            {
        
            /* clear register */
        
            reg = 0x00;

            /* check count as we can only transfer 4 bytes at a time */
        
            if (count/IIC_FIFO_DEPTH != 0)
                {
                transferCnt = IIC_FIFO_DEPTH;
                count -= IIC_FIFO_DEPTH;
                }
            else
                {
                transferCnt = count % IIC_FIFO_DEPTH;
                count = 0;
                }

            /* 
             * If count != 0, then chain transfers. Last transfer should not
             * be chained.                                                   
             */
      
            if (count != 0)
                reg |= IIC_CNTL_CHT;

            switch (transferCnt)
                {
                case 1: reg |= IIC_CNTL_TCT_1;
                        break;
                case 2: reg |= IIC_CNTL_TCT_2;
                        break;
                case 3: reg |= IIC_CNTL_TCT_3;
                        break;
                case 4: reg |= IIC_CNTL_TCT_4;
                        break;
                }      

            /*
             * Write the data. If first transfer, ie i=0, reduce transfer count
             * by one since dev_subaddr was already written to the MDBUF.
             */
            
            if ( firstPass )
                {
                transferCnt--;
                firstPass = FALSE;
                }

            while (transferCnt > 0)
                {
        
                /* write data byte to Data fifo */
        
                sysOutByte(iicBaseAddr + IIC_MDBUF, *data);

                /* adjust counters and pointers */
        
                transferCnt--;
                data++;
                devSubaddr++;
                }

            /* Initiate transfer */
        
            sysOutByte(iicBaseAddr + IIC_CNTL, (reg | IIC_CNTL_PT));

            /* wait for device */

            if ( waitXferDone(1,10,iicBaseAddr) != OK )
                return (ERROR);
                
            } /* end while count  */

            /* page complete. adjust numBytes */

            numBytes -= IIC_PAGE_SIZE ;
        }

        return(OK);
    }

/***************************************************************************
*
* waitXferdone - Wait until transfer is complete
*
* Wait until transfer is complete
*
* RETURNS: OK on Success
*          ERROR on timeout
*
* ERRNO: N/A
*/
LOCAL STATUS waitXferDone
    (
    UINT32 sec,                     /* number of seconds total */
    UINT32 usec,                    /* number of uSec per loop */
    UINT32 iicBaseAddr              /* Controller base address */
    )
    {
    struct timespec delayAmt;       /* Delay                 */
    UINT32  loops;                  /* number of loops to do */
    UINT32  reg;                    /* register              */

    /* calculate the number of loops we must do */
    
    loops = sec * (1000000/usec);
     
    while (loops--)
        {
        /* setup and call nanosleep */

        delayAmt.tv_sec = 0;
        delayAmt.tv_nsec = usec * 1000;
        nanosleep(&delayAmt, NULL);  

        /* poll completion bit */

        reg = sysInByte(iicBaseAddr+IIC_STS);  

        /* exit when complete */

        if ((reg & IIC_STS_PT) == 0)
            return (OK);

        }
    return (ERROR);
    }


