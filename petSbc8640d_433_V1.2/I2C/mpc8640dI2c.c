/* mot8640Di2c.c - I2C Driver Source Module */

/* create by:YZF */

/* includes */

#include <vxWorks.h>       	 		/* vxWorks generics */
#include "config.h"
#include <stdio.h>
#include <string.h>
#include <logLib.h>
#include <stdlib.h>
#include <taskLib.h>
#include <arch/ppc/vxPpcLib.h>

#include "mpc8640dI2c.h"

int I2CDoRepeatStart = 0;

/******************************************************************************
* sysMpc8640dMsDelay - Delay Routing.
*/
void sysMpc8640dMsDelay(UINT delay )
{
    sysMsDelay(delay);
}

/***************************************************************************
*
* i2cInit - initialize I2C device controller
*
*/

STATUS i2cInit()
{
	UINT8 regVal;
	
	regVal=REG8_MPC8640D_I2C1(MPC8640D_I2C_CONTROL_REG);
	regVal=regVal & ((UINT8)~MPC8640D_I2C_CONTROL_REG_MEN) |MPC8640D_I2C_CONTROL_REG_MSTA;
	REG8_MPC8640D_I2C1(MPC8640D_I2C_CONTROL_REG)= regVal;
  
    {
		UINT8 divider ;
		
		divider = 0x30; 
		
		regVal = REG8_MPC8640D_I2C1(MPC8640D_I2C_FREQ_DIV_REG);
		regVal = regVal & ((UINT8)~MPC8640D_I2C_FREQ_DIV_REG_MASK) | divider;
		REG8_MPC8640D_I2C1(MPC8640D_I2C_FREQ_DIV_REG)=regVal;
    
		regVal = REG8_MPC8640D_I2C1(MPC8640D_I2C_ADR_REG);
		regVal = regVal & ((UINT8)~MPC8640D_I2C_ADDRESS_REG_MASK) | 0x1;
		REG8_MPC8640D_I2C1(MPC8640D_I2C_ADR_REG)=regVal;
		
		regVal = REG8_MPC8640D_I2C1(MPC8640D_I2C_CONTROL_REG);
		regVal = regVal | MPC8640D_I2C_CONTROL_REG_MSTA;
		REG8_MPC8640D_I2C1(MPC8640D_I2C_CONTROL_REG)=regVal;
		
		regVal = REG8_MPC8640D_I2C1(MPC8640D_I2C_CONTROL_REG);
		regVal = regVal |  MPC8640D_I2C_CONTROL_REG_MTX;
		REG8_MPC8640D_I2C1(MPC8640D_I2C_CONTROL_REG)=regVal;
		
		regVal = REG8_MPC8640D_I2C1(MPC8640D_I2C_CONTROL_REG);
		regVal = regVal |  MPC8640D_I2C_CONTROL_REG_MIEN;
		REG8_MPC8640D_I2C1(MPC8640D_I2C_CONTROL_REG)=regVal;
		    
		regVal = REG8_MPC8640D_I2C1(MPC8640D_I2C_CONTROL_REG);
		regVal = regVal |  MPC8640D_I2C_CONTROL_REG_MEN;
		REG8_MPC8640D_I2C1(MPC8640D_I2C_CONTROL_REG)=regVal;
		
		regVal = REG8_MPC8640D_I2C1(MPC8640D_I2C_CONTROL_REG);
		 		
		regVal = REG8_MPC8640D_I2C1(MPC8640D_I2C_CONTROL_REG);
		regVal = regVal & ((UINT8)~MPC8640D_I2C_CONTROL_REG_MSTA);
		REG8_MPC8640D_I2C1(MPC8640D_I2C_CONTROL_REG)=regVal;
    }

    return(OK);
}


/******************************************************************************
* 
*/
int i2cCycleMpc8640dStart()  
{
	unsigned int timeOutCount;
	
	UINT8 regVal = 0;
	
	
	if( I2CDoRepeatStart == 1)
	{
	    regVal=REG8_MPC8640D_I2C1(MPC8640D_I2C_CONTROL_REG);      
	    regVal=regVal|MPC8640D_I2C_CONTROL_REG_MSTA|MPC8640D_I2C_CONTROL_REG_MTX|MPC8640D_I2C_CONTROL_REG_RSTA;    
	    REG8_MPC8640D_I2C1(MPC8640D_I2C_CONTROL_REG)=regVal;
	    
	    I2CDoRepeatStart = 0;
	    
	    return(0);
	}
	
	for ( timeOutCount = 100; timeOutCount; timeOutCount-- )
	{
		regVal=REG8_MPC8640D_I2C1(MPC8640D_I2C_STATUS_REG);
		
        if ( !(regVal & MPC8640D_I2C_STATUS_REG_MBB) )
        {
            break;
        }
        
        sysMpc8640dMsDelay(1);      
	}
	
    if ( !timeOutCount )
    {
        return(-1); 
    }
        
    regVal=REG8_MPC8640D_I2C1(MPC8640D_I2C_CONTROL_REG);      
    regVal=regVal|MPC8640D_I2C_CONTROL_REG_MSTA|MPC8640D_I2C_CONTROL_REG_MTX;    
    REG8_MPC8640D_I2C1(MPC8640D_I2C_CONTROL_REG)=regVal;
    
    I2CDoRepeatStart = 1;
    
    return(0);
}


/******************************************************************************
* 
* i2cCycleMpc8640dStop - perform I2C "stop" cycle
* 
*/
int i2cCycleMpc8640dStop() 
{
	UINT8 regVal = 0;
	
	sysMpc8640dMsDelay(1); 
	
    regVal=REG8_MPC8640D_I2C1(MPC8640D_I2C_CONTROL_REG);      
    regVal=regVal&((UINT8)~(MPC8640D_I2C_CONTROL_REG_MSTA |
					MPC8640D_I2C_CONTROL_REG_MTX  |  MPC8640D_I2C_CONTROL_REG_TXAK));  
    REG8_MPC8640D_I2C1(MPC8640D_I2C_CONTROL_REG)=regVal;
    
    
    I2CDoRepeatStart = 0;
    	
    return(0);

}


/******************************************************************************
* 
* i2cCycleMpc8640dRead - perform I2C "read" cycle
* 

*/
int i2cCycleMpc8640dRead
    (
    unsigned char *pReadDataBuf,    	/* pointer to read data buffer */
    int ack    							/* 0 = don't ack, 1 = ack */
    )
{
	volatile UINT8 regVal = 0;
    
    if(!ack)
    {
        regVal=REG8_MPC8640D_I2C1(MPC8640D_I2C_CONTROL_REG);       
        regVal=regVal&(UINT8)(~MPC8640D_I2C_CONTROL_REG_MTX)|MPC8640D_I2C_CONTROL_REG_TXAK;
        REG8_MPC8640D_I2C1(MPC8640D_I2C_CONTROL_REG)=regVal;    	
    	
    }
    else
    {
        regVal=REG8_MPC8640D_I2C1(MPC8640D_I2C_CONTROL_REG);       
        regVal=regVal&((UINT8)~(MPC8640D_I2C_CONTROL_REG_MTX|MPC8640D_I2C_CONTROL_REG_TXAK));
        REG8_MPC8640D_I2C1(MPC8640D_I2C_CONTROL_REG)=regVal;
    }
    
    sysMpc8640dMsDelay(1);
	
    regVal=REG8_MPC8640D_I2C1(MPC8640D_I2C_DATA_REG); 
    
    sysMpc8640dMsDelay(1);   
    
    regVal=REG8_MPC8640D_I2C1(MPC8640D_I2C_DATA_REG); 
    
    *pReadDataBuf =regVal;
    
    return(0);

}


/******************************************************************************
* 
* i2cCycleMpc8640dWrite - perform I2C "write" cycle
* 
*/
int i2cCycleMpc8640dWrite
    (
    unsigned char   writeData   						/* character to write */
    )
{
	
	REG8_MPC8640D_I2C1(MPC8640D_I2C_DATA_REG)=writeData;
	
    return(0);
}


/******************************************************************************
* 
* i2cCycleMpc8640dAckIn - perform I2C "acknowledge-in" cycle
* 
* This function's purpose is to perform an I2C acknowledge-in cycle.
* 
* RETURNS:
*   zero        = operation successful
*   non-zero    = operation failed
*
* ERRNO: N/A
*/
int i2cCycleMpc8640dAckIn ( )
{
	UINT8 regVal = 0;
	unsigned int timeOutCount;
	
    for ( timeOutCount = 10000; timeOutCount; timeOutCount-- )
    {
    	regVal=REG8_MPC8640D_I2C1(MPC8640D_I2C_STATUS_REG);
    			
    	if ( regVal & MPC8640D_I2C_STATUS_REG_MIF )
    	{
    		regVal = REG8_MPC8640D_I2C1(MPC8640D_I2C_STATUS_REG);
    		regVal = regVal & ((UINT8)~MPC8640D_I2C_STATUS_REG_MIF);
    		REG8_MPC8640D_I2C1(MPC8640D_I2C_STATUS_REG)=regVal;
    		
    		break;
    	}  
    }
        
    if ( !timeOutCount )
    {
        return(-1);
    }
    
    return(0);

}


/******************************************************************************
* 
* i2cCycleMpc8640dAckOut - perform I2C "acknowledge-out" cycle
* 
*/
int i2cCycleMpc8640dAckOut 
    (
    int unit
    )
{
    return(0);
}


/******************************************************************************
* 
* i2cCycleMpc8640dKnownState - initialize the I2C bus to a known state
* 
*/

int i2cCycleMpc8640dKnownState ()
{
    STATUS 		status;
    UINT   		timeOutCount;
    UINT8   	regVal;

    status = OK;
    
    for ( timeOutCount = 1000; timeOutCount; timeOutCount-- )
    {       
		regVal=REG8_MPC8640D_I2C1(MPC8640D_I2C_STATUS_REG);
		
        if ( !(regVal & MPC8640D_I2C_STATUS_REG_MBB) )
        {
        	status = OK;
            break;
        }
        
       	if ( !(timeOutCount % 50) )
        {
            status = i2cInit();
            
            if ( status == OK )
                break;
            else
                return ERROR ;
        }
    }


    if ( !timeOutCount )
        status = ERROR;

    return(status);
    
}


/******************************************************************************
* 
* i2cCycleMpc8640dDelay - perform interface's I2C delay routine 
* 
*/
void i2cCycleMpc8640dDelay
	(
    int mSeconds    /* time to delay in milliseconds */
    )
{
	sysMsDelay(mSeconds );

 }


unsigned int i2cAddressMunge(unsigned int deviceAddress)
{
    return((deviceAddress<<1)& 0xfe) ;
}

/** DEBUG FUNCTIONS **/


STATUS i2cRegReadVal(UINT32 deviceAddress,UINT8 registerAddress ,UINT8 * pData)
{	
	i2cCycleMpc8640dStop(); 
	
	if(i2cCycleMpc8640dKnownState())
	{
		return ERROR;
	}
	
	if(i2cCycleMpc8640dStart())
	{
		return ERROR;
	}
	
    /* device address - write, R/W bit = 0  */
    if (i2cCycleMpc8640dWrite(i2cAddressMunge(deviceAddress)))
    {
		return ERROR;
    }
    
    if ( i2cCycleMpc8640dAckIn() )
    {
		return ERROR;
    }
    
    /* command byte (register #) */
    if ( i2cCycleMpc8640dWrite(registerAddress ) )
    {
		return ERROR;
    }
    
    if ( i2cCycleMpc8640dAckIn() )
    {
		return ERROR;
    }
    
	if(i2cCycleMpc8640dStart())
	{
		return ERROR;
	}
	
    /* device address - read, R/W bit = 1 */
    if ( i2cCycleMpc8640dWrite(i2cAddressMunge(deviceAddress)|0x1) )
    {
		return ERROR;
    }
    
    if ( i2cCycleMpc8640dAckIn() )
    {
		return ERROR;
    }
    
	if ( i2cCycleMpc8640dRead((unsigned char *)pData,0) )
	{
		return ERROR;
	}
		
	if (i2cCycleMpc8640dStop())
	{
		return ERROR;
	}
	
	i2cCycleMpc8640dKnownState();
		
	return OK;
}

UINT8 i2cRegRead(UINT32 deviceAddress,UINT8 registerAddress)
{
	UINT8 regVal;
	
	i2cRegReadVal(deviceAddress,registerAddress,&regVal);
	
	return regVal;
}



STATUS i2cRegWrite(UINT32 deviceAddress, UINT8 registerAddress, UINT8 data)
{	
	i2cCycleMpc8640dStop(); 
	
	if(i2cCycleMpc8640dKnownState())
	{
		return ERROR;
	}
	
	if(i2cCycleMpc8640dStart())
	{
		return ERROR;
	}
	
    /* device address - write, R/W bit = 0  */
    if (i2cCycleMpc8640dWrite(i2cAddressMunge(deviceAddress)))
    {
		return ERROR;
    }
    
    if ( i2cCycleMpc8640dAckIn() )
    {
		return ERROR;
    }
    
    /* command byte (register #) */
    if ( i2cCycleMpc8640dWrite(registerAddress ) )
    {
		return ERROR;
    }
    
    if ( i2cCycleMpc8640dAckIn() )
    {
		return ERROR;
    }
    
	/* write data */
	if ( i2cCycleMpc8640dWrite(data) )
	{
		return ERROR;
	}
		
	if ( i2cCycleMpc8640dAckIn() )
	{
		return ERROR;
	}
		
	if ( i2cCycleMpc8640dStop() )
	{
		return ERROR;
	}
	
	i2cCycleMpc8640dDelay(10);
	
	i2cCycleMpc8640dKnownState();
	
	return OK;
}

STATUS i2cRegReadWordVal(UINT32 deviceAddress,UINT8 registerAddress ,UINT16 * pData)
{

	UINT8 byteVal[2];
	UINT8 *pByteVal=&byteVal[0];
	
	i2cCycleMpc8640dStop();
		
	if(i2cCycleMpc8640dKnownState())
	{
		return ERROR;
	}
	
	if(i2cCycleMpc8640dStart())
	{
		return ERROR;
	}
	
    /* device address - write, R/W bit = 0  */
    if (i2cCycleMpc8640dWrite(i2cAddressMunge(deviceAddress)))
    {
		return ERROR;
    }
    
    if ( i2cCycleMpc8640dAckIn() )
    {
		return ERROR;
    }
    
    /* command byte (register #) */
    if ( i2cCycleMpc8640dWrite(registerAddress ) )
    {
		return ERROR;
    }
    
    if ( i2cCycleMpc8640dAckIn() )
    {
		return ERROR;
    }
    
	if(i2cCycleMpc8640dStart())
	{
		return ERROR;
	}
	
    /* device address - read, R/W bit = 1 */
    if ( i2cCycleMpc8640dWrite(i2cAddressMunge(deviceAddress)|0x1) )
    {
		return ERROR;
    }
    
    if ( i2cCycleMpc8640dAckIn() )
    {
		return ERROR;
    }
    
	if ( i2cCycleMpc8640dRead((unsigned char *)pByteVal,1) )
	{
		return ERROR;
	}
	
	if ( i2cCycleMpc8640dRead((unsigned char *)(pByteVal+1),0) )
	{
		return ERROR;
	}
	
	
	if (i2cCycleMpc8640dStop())
	{
		return ERROR;
	}
		
	i2cCycleMpc8640dKnownState();
		
	*pData=(byteVal[1]<<8)|byteVal[0];
			
	return OK;
}

UINT16 i2cRegReadWord(UINT32 deviceAddress,UINT8 registerAddress)
{
	UINT16 regVal;
	
	i2cRegReadWordVal(deviceAddress,registerAddress,&regVal);
	
	return regVal;
}

STATUS i2cRegWriteWord(UINT32 deviceAddress, UINT8 registerAddress, UINT16 data)
{

	UINT8 byteVal[2];
	
	byteVal[0]=(UINT8)(data&0x00FF);		/*low byte*/
	byteVal[1]=(UINT8)((data&0xFF00)>>8);	/*high byte*/
	
	i2cCycleMpc8640dStop(); 
	
	if(i2cCycleMpc8640dKnownState())
	{
		return ERROR;
	}
	
	if(i2cCycleMpc8640dStart())
	{
		return ERROR;
	}
	
    /* device address - write, R/W bit = 0  */
    if (i2cCycleMpc8640dWrite(i2cAddressMunge(deviceAddress)))
    {
		return ERROR;
    }
    
    if ( i2cCycleMpc8640dAckIn() )
    {
		return ERROR;
    }
    
    /* command byte (register #) */
    if ( i2cCycleMpc8640dWrite(registerAddress ) )
    {
		return ERROR;
    }
    
    if ( i2cCycleMpc8640dAckIn() )
    {
		return ERROR;
    }
    
	/* write data low */
	if ( i2cCycleMpc8640dWrite(byteVal[0]) )
	{
		return ERROR;
	}
		
	if ( i2cCycleMpc8640dAckIn() )
	{
		return ERROR;
	}
	
	/* write data high */
	if ( i2cCycleMpc8640dWrite(byteVal[1]) )
	{
		return ERROR;
	}
		
	if ( i2cCycleMpc8640dAckIn() )
	{
		return ERROR;
	}
		
	if ( i2cCycleMpc8640dStop() )
	{
		return ERROR;
	}
	
	i2cCycleMpc8640dDelay(10);
	
	i2cCycleMpc8640dKnownState();
	
	return OK;
}



