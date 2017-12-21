/* i2cLib.c - support library for i2c master and slave drivers */

/* Copyright 1984 - 2003 Wind River Services  */
/* Copyright 1984 - 2003 Wind River Systems, Inc.  */

/*
modification history
--------------------
01d,05mar04,pdr  Fixed warning.
01c,05nov03,e_c	 add comment for documentation.
01b,08apr03,e_c	 enable ISR call for i2cWrite and i2cRead.
01a,12jul01,a_n	 written.
*/
 
/*
DESCRIPTION
This library contains support routines for master and slave i2c drivers.
These routines are common to ALL master and slave drivers.  
Specialized routines should only appear in the drivers themselves.

This framework presents the same mechanism used by the Mux layer with END drivers.
Each I2C driver is initialized using a load fonction passed to the i2cMasterDevLoad
function. 


Slave device drivers will use this master device drivers when specifying 
"<masterName>:<masterUnitNo>" in their load string. 
After that slave device drivers can access the I2C bus with 
the help of i2cRead/i2cWrite functions of the i2cLib.
Also the master will never be used directly by an application but only by slave drivers
through the i2cLib : 

Application
------------------------------------------
I2C Slave driver
------------------------------------------
I2C Lib
------------------------------------------
I2C Master driver
------------------------------------------
I2C MG5200 Controller

An application will use directly slave device drivers APIs.

INCLUDE FILES:
*/

/* includes */

#include "vxWorks.h"
#include "memLib.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "intLib.h"
#include "logLib.h"
#include "errnoLib.h"

#include "drv/i2c/i2cLib.h"

#define I2CSTREQ(A, B) (strcmp(A, B) == 0 ? 1 : 0)
#define S_i2cLib_Master_LOAD_FAILED 0xFFFFAA55
#define S_i2cLib_Slave_LOAD_FAILED 0xFFFFAA56

/* defints */

/* typedefs */

/* globals */
int i2cDebug = 0;

int i2cSemAvailable = 0;

SEM_ID i2cLock = NULL;
LIST i2cMasterList;
LIST i2cSlaveList;
/* locals */

/* forward declarations */

void i2cAccessSemTake
    (
    I2C_MASTER_OBJ* pI2CMaster,
    int tmout
    ) 
    {
    semTake(pI2CMaster->accessSem,tmout);
    }

void i2cAccessSemGive
    (
    I2C_MASTER_OBJ* pI2CMaster
    )
    {
    semGive (pI2CMaster->accessSem);
    }

void i2cMasterFlagsClr
    (
    I2C_MASTER_OBJ* pI2CMaster,
    UINT32 clrBits
    ) 
    {
    pI2CMaster->flags &= ~clrBits;
    }

void i2cMasterFlagsSet
    (
    I2C_MASTER_OBJ* pI2CMaster,
    UINT32 setBits
    )
    {
    pI2CMaster->flags |= (setBits);
    }

int i2cMasterFlagsGet
    (
    I2C_MASTER_OBJ* pI2CMaster
    ) 
    {
    return(pI2CMaster->flags);
    }

char* i2cMasterDevName
    (
    I2C_MASTER_OBJ* pI2CMaster
    ) 
    {
    return(pI2CMaster->i2cDevObject.name);
    }

void i2cSlaveAddrSet
    (
    I2C_SLAVE_OBJ* pI2CSlave,
    UINT32 i2cAddr
    )
    {
    pI2CSlave->i2cAddr = i2cAddr;
    }

UINT32 i2cSlaveAddrGet
    (
    I2C_SLAVE_OBJ* pI2CSlave
    )
    {
    return(pI2CSlave->i2cAddr);
    }

/******************************************************************************
*
* i2cMasterFindByName - find a device using its string name
*
* This routine takes a string name and a unit number and finds the
* I2C Master device that has that name/unit combination.
*
* RETURNS: A pointer to an I2C_MASTER_OBJ or NULL (if the device is not found).
*
*/

I2C_MASTER_OBJ* i2cMasterFindByName
    (
    char* pName,			/* device name to search for */
    int unit
    )
    {
    BOOL found = FALSE;
    I2C_MASTER_TBL_ROW* pNode;
    I2C_MASTER_OBJ* pMaster;
    
    for (pNode = (I2C_MASTER_TBL_ROW *)lstFirst(&i2cMasterList); pNode != NULL; 
	pNode = (I2C_MASTER_TBL_ROW *)lstNext(&pNode->node))
	{
	if (I2CSTREQ(pNode->name, pName))
	    {
            found = TRUE;
            break;
	    }
	}

    if (found)
        {
        for (pMaster = (I2C_MASTER_OBJ *)lstFirst(&pNode->units); pMaster != NULL; 
             pMaster = (I2C_MASTER_OBJ *)lstNext(&pMaster->node))
            {
            if (pMaster->i2cDevObject.unit == unit)
                {
                return (pMaster);
                }
            }
        }    

    return (NULL);

    }

/******************************************************************************
*
* i2cSlaveFindByName - find a device using its string name
*
* This routine takes a string name and a unit number and finds the
* I2C Slave device that has that name/unit combination.
*
* RETURNS: A pointer to an I2C_SLAVE_OBJ or NULL (if the device is not found).
*
*/

I2C_SLAVE_OBJ* i2cSlaveFindByName
    (
    char* pName,			/* device name to search for */
    int unit
    )
    {
    BOOL found = FALSE;
    I2C_SLAVE_TBL_ROW* pNode;
    I2C_SLAVE_OBJ* pSlave;
    
    for (pNode = (I2C_SLAVE_TBL_ROW *)lstFirst(&i2cSlaveList); pNode != NULL; 
	pNode = (I2C_SLAVE_TBL_ROW *)lstNext(&pNode->node))
	{
	if (I2CSTREQ(pNode->name, pName))
	    {
            found = TRUE;
            break;
	    }
	}

    if (found)
        {
        for (pSlave = (I2C_SLAVE_OBJ *)lstFirst(&pNode->units); pSlave != NULL; 
             pSlave = (I2C_SLAVE_OBJ *)lstNext(&pSlave->node))
            {
            if (pSlave->i2cDevObject.unit == unit)
                {
                return (pSlave);
                }
            }
        }    

    return (NULL);

    }

/*******************************************************************************
*
* i2CLibInit - initialize global state for i2cLib 
*
* This routine initializes all global state for i2cLib 
*
* RETURNS: OK or ERROR.
*/

STATUS i2cLibInit (void)
    {
    STATUS status = OK;

    if (i2cDebug)
        logMsg ("start of muxLibInit\n", 0, 0, 0, 0, 0, 0);

    if (i2cLock != NULL)
        return (OK);
    
    i2cLock = semBCreate(SEM_Q_PRIORITY, SEM_FULL);
    if (i2cLock == NULL)
        return (ERROR);

    /* initialize i2c Master list */

    lstInit (&i2cMasterList);
    
    if (i2cDebug)
        logMsg ("End of muxLibInit\n", 0, 0, 0, 0, 0, 0);

    return (status);
    }


/******************************************************************************
* 
* i2cMasterDevLoad - load an i2c Master device driver, if required 
* 
* The i2cMasterDevLoad() routine loads a i2c master interface driver.  Internally, 
* this routine calls the specified i2cDrvLoad() to initialize the software state 
* of the device.  After the device is initialized, i2cMasterDevStart() must be called 
* to start the device. 
*    <unit> Expects the unit number of the device. 
*    <i2cDrvLoad> Expects a pointer to the driver's i2DrvLoad() entry point.
*    <pInitString> Expects a pointer to an initialization string, 
*      typically a colon-delimited list of options. The i2cMasterDevLoad() 
*		routine passes this along blindly to the i2cDrvLoad() function.
*      
* 
* 
* 
* RETURNS: A pointer to the new device or NULL if an error occurred.
*
* ERRNO: S_i2cLib_Master_LOAD_FAILED
*/

I2C_MASTER_OBJ * i2cMasterDevLoad
    (
    int unit,                   /* unit number of device */
    FUNCPTR i2cDrvLoad,	/* load function of the driver  */
    char* pInitString		/* init string for this driver  */
    )
    {
    I2C_MASTER_OBJ* pNew;
    I2C_MASTER_TBL_ROW* pNode;
    char initString [I2C_INIT_STR_MAX];
    char devName [I2C_NAME_MAX];
    BOOL found = FALSE;

    

    if (i2cDebug)
        logMsg ("Start of i2cMasterDevLoad\n", 0, 0, 0, 0, 0, 0);

    bzero ( (char *)initString, I2C_INIT_STR_MAX);
    bzero ( (char *)devName, I2C_NAME_MAX);

    /* Let's mutually exclude here, wouldn't you say? */
    semTake (i2cLock, WAIT_FOREVER);
    
    /*
     * Loading a device is a two pass algorithm.
     *
     * This is Pass 1.
     *
     * In the first pass we as the device what its name is.  If that name
     * exists in our table then we add the new node to the end of the
     * already existing list.  If not then we create a new row in the
     * table, and place the new node as the zero'th (0) element in the node's
     * list.
     */

    if (i2cDrvLoad ( (char *)devName) != 0)
        {
        goto i2cMasterLoadErr;
        }

    if (i2cMasterFindByName ( (char *)devName, unit) != NULL)
        {
        goto i2cMasterLoadErr;
        }

    for (pNode = (I2C_MASTER_TBL_ROW *)lstFirst(&i2cMasterList); pNode != NULL; 
	pNode = (I2C_MASTER_TBL_ROW *)lstNext(&pNode->node))
	{
	if (I2CSTREQ (pNode->name, (char *)devName))
            {
            found = TRUE;
            break;
            }
	}

    /*  If there is no row for this device then add it. */
    
    if (!found)
        {
        pNode = (I2C_MASTER_TBL_ROW *) malloc(sizeof(I2C_MASTER_TBL_ROW));
        if (pNode == NULL)
            {
            goto i2cMasterLoadErr;
            }
        bzero ((char *)pNode, sizeof(I2C_MASTER_TBL_ROW));
        strncpy(pNode->name, devName, I2C_NAME_MAX - 1);
		pNode->name [I2C_NAME_MAX - 1] = EOS;
        lstAdd(&i2cMasterList, &pNode->node);
        }

    /*
     * This is Pass 2.
     *
     * Now that we can determine a unique number we assign that number, to
     * the device and actually load it.
     */
    
    sprintf ( (char *)initString, "%d:%s", unit, pInitString);

    pNew = (I2C_MASTER_OBJ *)i2cDrvLoad ( (char *)initString);

    if (pNew == NULL)
        {
        goto i2cMasterLoadErr;
        }

    lstAdd(&pNode->units, &pNew->node);

    semGive (i2cLock);

    return (pNew);

    i2cMasterLoadErr:
    errnoSet (S_i2cLib_Master_LOAD_FAILED);
    semGive (i2cLock);
    return (NULL);
    }

/******************************************************************************
* 
* i2cSlaveDevLoad - load an i2c Slave device driver, if required 
* 
* The i2cSlaveDevLoad() routine loads a i2c slave interface driver.  Internally, 
* this routine calls the specified i2cDrvLoad() to initialize the software state 
* of the device.  After the device is initialized, i2cSlaveDevStart() must be called 
* to start the device. 
* <unit> 15
* Expects the unit number of the device. 
* <i2cDrvLoad> 
* Expects a pointer to the driver's i2DrvLoad() entry point. 
* <pInitString> 
* Expects a pointer to an initialization string, typically a colon-delimited
* list of options.  The i2cSlaveDevLoad() routine passes this along blindly to 
* the i2cDrvLoad() function.
* 
* RETURNS: A pointer to the new device or NULL if an error occurred.
*
* ERRNO: S_i2cLib_Slave_LOAD_FAILED
*/

I2C_SLAVE_OBJ * i2cSlaveDevLoad
    (
    int unit,                   /* unit number of device */
    FUNCPTR i2cDrvLoad,	/* load function of the driver  */
    char* pInitString		/* init string for this driver  */
    )
    {
    I2C_SLAVE_OBJ* pNew;
    I2C_SLAVE_TBL_ROW* pNode;
    char initString [I2C_INIT_STR_MAX];
    char devName [I2C_NAME_MAX];
    BOOL found = FALSE;
	int nbytes = 0;

 

    if (i2cDebug)
        printf ("Start of i2cSlaveDevLoad\n");

    bzero ( (char *)initString, I2C_INIT_STR_MAX);
    bzero ( (char *)devName, I2C_NAME_MAX);

    /* Let's mutually exclude here, wouldn't you say? */
    semTake (i2cLock, WAIT_FOREVER);
    
    /*
     * Loading a device is a two pass algorithm.
     *
     * This is Pass 1.
     *
     * In the first pass we as the device what its name is.  If that name
     * exists in our table then we add the new node to the end of the
     * already existing list.  If not then we create a new row in the
     * table, and place the new node as the zero'th (0) element in the node's
     * list.
     */

    if (i2cDrvLoad ( (char *)devName) != 0)
        {
        goto i2cSlaveLoadErr;
        }

    if (i2cSlaveFindByName ( (char *)devName, unit) != NULL)
        {
        goto i2cSlaveLoadErr;
        }

    for (pNode = (I2C_SLAVE_TBL_ROW *)lstFirst(&i2cSlaveList); pNode != NULL; 
	pNode = (I2C_SLAVE_TBL_ROW *)lstNext(&pNode->node))
	{
	if (I2CSTREQ (pNode->name, (char *)devName))
            {
            found = TRUE;
            break;
            }
	}

    /*  If there is no row for this device then add it. */
    
    if (!found)
        {
        pNode = (I2C_SLAVE_TBL_ROW*) malloc(sizeof(I2C_SLAVE_TBL_ROW));
        if (pNode == NULL)
            {
            goto i2cSlaveLoadErr;
            }
        bzero ((char *)pNode, sizeof(I2C_SLAVE_TBL_ROW));
        strncpy(pNode->name, devName, I2C_NAME_MAX - 1);
		pNode->name [I2C_NAME_MAX - 1] = EOS;
        lstAdd(&i2cSlaveList, &pNode->node);
        }

    /*
     * This is Pass 2.
     *
     * Now that we can determine a unique number we assign that number, to
     * the device and actually load it.
     */
    
    nbytes = sprintf ( (char *)initString, "%d:%s", unit, pInitString);
    
	if (i2cDebug)
        printf ("i2cSlaveDevLoad : before i2cDrvLoad initString = %s, nbytes = %d\n", initString, nbytes);

    pNew = (I2C_SLAVE_OBJ *)i2cDrvLoad ( (char *)initString);

    if (pNew == NULL)
        {
        goto i2cSlaveLoadErr;
        }

    lstAdd(&pNode->units, &pNew->node);

    semGive (i2cLock);

    return (pNew);

    i2cSlaveLoadErr:
    errnoSet (S_i2cLib_Slave_LOAD_FAILED);
    semGive (i2cLock);
    return (NULL);
    }

/*******************************************************************************
*
* i2cMasterObjInit - initialize an I2C_MASTER_OBJ structure
*
* This routine initializes an I2C_MASTER_OBJ structure and fills it with data from 
* the argument list.  It also creates and initializes semaphores.  
*
* RETURNS: OK or ERROR.
*/

STATUS i2cMasterObjInit
    (
    I2C_MASTER_OBJ *	pI2CMasterObj,	/* object to be initialized */
    char *	pMasterBaseName,	/* device base name, for example, "mpc8240_i2c" */
    int		masterUnitNo,		/* unit number */
    I2C_MASTER_FUNCS * pMasterFuncTable,	/* master device functions */
    char*   pMasterDescription,
    UINT32  masterFlags
    )
    {
    /* Create the write semaphore. */

    pI2CMasterObj->accessSem = semMCreate ( SEM_Q_PRIORITY |
                                  SEM_DELETE_SAFE |
                                  SEM_INVERSION_SAFE);

    if (pI2CMasterObj->accessSem == NULL)
        {
        return (ERROR);
        }

    /* Install data and functions into the node. */

    pI2CMasterObj->flags = masterFlags;

    /* Check and control the length of the name string. */
    if (strlen(pMasterBaseName) > sizeof(pI2CMasterObj->i2cDevObject.name))
        pMasterBaseName[sizeof(pI2CMasterObj->i2cDevObject.name - 1)] = EOS;

    strcpy (pI2CMasterObj->i2cDevObject.name, pMasterBaseName);

    /* Check and control the length of the description string. */
    if (strlen(pMasterDescription) > sizeof(pI2CMasterObj->i2cDevObject.description))
        pMasterDescription[sizeof(pI2CMasterObj->i2cDevObject.description - 1)] = EOS;
    strcpy (pI2CMasterObj->i2cDevObject.description, pMasterDescription);

    pI2CMasterObj->i2cDevObject.unit = masterUnitNo;
    
    pI2CMasterObj->pFuncTable = pMasterFuncTable;

    return OK;
    }

/*******************************************************************************
*
* i2cSlaveObjInit - initialize an I2C_SLAVE_OBJ structure
*
* This routine initializes an I2C_SLAVE_OBJ structure and fills it with data from 
* the argument list.  It also creates and initializes semaphores.  
*
* RETURNS: OK or ERROR.
*/

STATUS i2cSlaveObjInit
    (
    I2C_SLAVE_OBJ *	pI2CSlaveObj,	/* object to be initialized */
    char *	pSlaveBaseName,	/* device base name, for example, "mpc8240_i2c" */
    int		slaveUnitNo,		/* unit number */
    char*   pSlaveDescription,
    UINT32  slaveFlags,
    UINT32  slaveI2CAddr,
    char *	pMasterBaseName,	/* device base name, for example, "mpc8240_i2c" */
    int		masterUnitNo		/* unit number */
    )
    {
	I2C_MASTER_OBJ * pI2CMaster = NULL;

	pI2CMaster = i2cMasterFindByName(pMasterBaseName, masterUnitNo);
	if(pI2CMaster == NULL)
		return(ERROR);

    /* Check and control the length of the name string. */
    if (strlen(pSlaveBaseName) > sizeof(pI2CSlaveObj->i2cDevObject.name))
        pSlaveBaseName[sizeof(pI2CSlaveObj->i2cDevObject.name - 1)] = EOS;

    strcpy (pI2CSlaveObj->i2cDevObject.name, pSlaveBaseName);

    /* Check and control the length of the description string. */
    if (strlen(pSlaveDescription) > sizeof(pI2CSlaveObj->i2cDevObject.description))
        pSlaveDescription[sizeof(pI2CSlaveObj->i2cDevObject.description - 1)] = EOS;
    strcpy (pI2CSlaveObj->i2cDevObject.description, pSlaveDescription);

    pI2CSlaveObj->i2cDevObject.unit = slaveUnitNo;
    pI2CSlaveObj->i2cAddr = slaveI2CAddr;
    pI2CSlaveObj->flags = slaveFlags;
    pI2CSlaveObj->i2cMaster = pI2CMaster;
    

    return OK;
    }

/*******************************************************************************
*
* i2cMasterObjFlagSet - set the `flags' member of an I2C_MASTER_OBJ structure
*
* As input, this routine expects a pointer to an I2C_MASTER_OBJ structure 
* (the <pI2CMaster> parameter) and a flags value (the <flags> parameter).
* This routine sets the 'flags' member of the I2C_MASTER_OBJ structure
* to the value of the <flags> parameter. 
*
* Because this routine assumes that the driver interface is now up,  
* this routine also sets the 'attached' member of the referenced I2C_MASTER_OBJ
* structure to TRUE. 
*
* RETURNS: OK
*/

STATUS i2cMasterObjFlagSet
    (
    I2C_MASTER_OBJ * pI2CMaster,
    UINT32	flags
    )
    {
    pI2CMaster->attached = TRUE;
    pI2CMaster->flags = flags;

    return OK;
    }


/*****************************************************************************
* 
* i2cWrite - write to an i2c slave device via a master i2c interface
*
* This routine writes to an i2c slave device via a master i2c interface 
*
* <pCookie>
* Expects the pointer returned from i2cMasterBind().  This pointer 
* identifies the binding between the i2c master and slave devices.
* <pBuff>
* Expects a pointer to the buffer that contains the data you want 
* to send.  
* <nBytes>
* Expects the number of bytes you want to send. Not including the address bytes. 
* <options>
* Expects a bitmap of i2c options like I2C_OPT_NO_STOP 
*
* RETURNS: OK, or ENO_I2C_BIND if <pCookie> does not represent a valid binding,
*          or ERROR if the i2c master driver's i2cMasterWrite() routine fails.
*
*/

STATUS i2cWrite
    (
    I2C_SLAVE_OBJ * pI2CSlave, 	/* i2c slave device structure */
    UINT8 * pBuff,	/* data to be written */
    UINT32 nBytes,	/* number of bytes to write */
    UINT32 options,	/* transaction options */
	int timeout     /* timeout in ticks */
    )
    {
    UINT8 addr7bit;
    UINT8 addr10bitMsb;
    UINT8 * pBuff10Bit = NULL;

    int error = ENO_I2C_MASTER;

    I2C_MASTER_OBJ* pI2CMaster;

    pI2CMaster = pI2CSlave->i2cMaster;

    if (i2cDebug)
        printf ("i2cWrite : pI2CSlave = 0x%x, pI2CMaster = 0x%x\n", (int) pI2CSlave, (int) pI2CMaster);


    if (pI2CMaster == NULL)
	{
	error = ENO_I2C_MASTER;
	}
    else if (pBuff != NULL) 
	{
	/* get exclusive use of the master */
		if ( !intContext() )
		{
			semTake(pI2CMaster->accessSem,WAIT_FOREVER);
		}

		if(pI2CSlave->flags & I2C_SLAVE_FLG_10_BIT_ADDR)
		{
   			/* 10 bit addr is composed of all ones in the 
				* upper nibble, the upper 2 bits of the 10 bit
				* address in bit position 2:1, and the read/write
				* bit setting at bit 0 */

			addr10bitMsb = (UINT8) ((0xF0 | (pI2CSlave->i2cAddr>>7)) &  0xFE);

			/* get memory one more byte larger then input buffer 
			* in order to prefix the buffer with the LSB of the 10 bit addr */

			pBuff10Bit = malloc(nBytes + 1);
			if(pBuff10Bit == NULL)
			{
				/* we are done using the master, release the resource */
				if ( !intContext() )
				{
					semGive(pI2CMaster->accessSem);
				}

				return(ERROR);
			}

			/* set the first byte of the payload to be the LSB of the
			 * 10 bit addr */
			pBuff10Bit[0] = (UINT8) (pI2CSlave->i2cAddr & 0xFF);
			bcopy(pBuff, (pBuff10Bit + 1), nBytes);

			error = pI2CMaster->pFuncTable->write (pI2CMaster, addr10bitMsb, 
							   pBuff10Bit, nBytes + 1, options, timeout);

			free(pBuff10Bit);
		}
		else
		{
			addr7bit = (pI2CSlave->i2cAddr<<1) & 0xFE;
			error = pI2CMaster->pFuncTable->write (pI2CMaster, addr7bit, 
							   pBuff, nBytes, options, timeout);
		}

		/* we are done using the master, release the resource */
		if ( !intContext() )
		{
			semGive(pI2CMaster->accessSem);
		}

	}
	

    return (error);
    }

/*****************************************************************************
* 
* i2cRead - read from an i2c slave device via a master i2c interface
*
* This routine reads from an i2c slave device via a master i2c interface 
*
* <pCookie>
* Expects the pointer returned from i2cMasterBind().  This pointer 
* identifies the binding between the i2c master and slave devices.
* <pBuff>
* Expects a pointer to the buffer that contains the data you want 
* to read into.  
* <nBytes>
* Expects the number of bytes you want to read. Not including the address bytes. 
* <options>
* Expects a bitmap of i2c options like I2C_OPT_NO_ACK 
*
* RETURNS: OK, or ENO_I2C_BIND if <pCookie> does not represent a valid binding,
*          or ERROR if the i2c master driver's i2cMasterRead() routine fails.
*
*/

STATUS i2cRead
    (
    I2C_SLAVE_OBJ * pI2CSlave, 	/* i2c slave device structure */
    UINT8 * pBuff,	/* data buffer to read into */
    UINT32 nBytes,	/* number of bytes to read */
    UINT32 options,	/* transaction options */
	int timeout     /* timeout in ticks */
    )
    {  
    UINT8 addr7bit;
    UINT8 addr10bitMsb;
    UINT8 pBuff10Bit[1];

    I2C_MASTER_OBJ* pI2CMaster;
    int error = ENO_I2C_MASTER;

    pI2CMaster = pI2CSlave->i2cMaster;

    if (i2cDebug)
        logMsg ("i2cRead : pI2CSlave = 0x%x, pI2CMaster = 0x%x\n", (int) pI2CSlave, (int) pI2CMaster, 0, 0, 0, 0);

    if (pI2CMaster == NULL)
	{
	error = ENO_I2C_MASTER;
	}
    else if (pBuff != NULL) 
	{
		/* get exclusive use of the master */
		if ( !intContext() )
		{
			semTake(pI2CMaster->accessSem,WAIT_FOREVER);
		}

		if(pI2CSlave->flags & I2C_SLAVE_FLG_10_BIT_ADDR)
		{
			/* 10 bit addr is composed of all ones in the 
			* upper nibble, the upper 2 bits of the 10 bit
			* address in bit position 2:1, and the read/write
			* bit setting at bit 0 */

			/* we first do a dummy write operation to set the 
			 * full 10 bit address, then we follow it will a
			 * read operation */

			addr10bitMsb = (UINT8) ((0xF0 | (pI2CSlave->i2cAddr>>7)) &  0xFE);
	
			/* set the single byte payload to be the LSB of the
			 * 10 bit addr */
			pBuff10Bit[0] = (UINT8) (pI2CSlave->i2cAddr & 0xFF);

			error = pI2CMaster->pFuncTable->write (pI2CMaster, addr10bitMsb, 
					       pBuff10Bit, 1, options, timeout);

			if(error != OK)
			{
				/* we are done using the master, release the resource */
				if ( !intContext() )
				{
					semGive(pI2CMaster->accessSem);
				}

				return(error);
			}

			/* now switch to a read operation */
			addr10bitMsb |= 0x01;

			error = pI2CMaster->pFuncTable->read (pI2CMaster, addr10bitMsb, 
					       pBuff, nBytes, options, timeout);

		}
		else
		{
			addr7bit = ((pI2CSlave->i2cAddr & 0x7F)<<1) | 0x01;
			error = pI2CMaster->pFuncTable->read (pI2CMaster, addr7bit, 
						     pBuff, nBytes, options, timeout);
		}

		/* we are done using the master, release the resource */
		if ( !intContext() )
		{
			semGive(pI2CMaster->accessSem);
		}
	}

    return (error);
    }
