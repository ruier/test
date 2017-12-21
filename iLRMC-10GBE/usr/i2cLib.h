/* i2cLib.h - Structure for the i2cLib API*/

/* Copyright 1984 - 2002 Wind River Services. */
/* Copyright 1984 - 2002, 2010 Wind River Systems, Inc. */

/*
modification history
--------------------
01b,29apr10,pad  Moved extern C statement after include statements.
01a,18jul01,a_n	 written.

*/
 
/*
DESCRIPTION

This file defines all the structures uses by master and slave i2c device
drivers.
*/

#ifndef __INCi2cLibh
#define __INCi2cLibh

#include "vxWorks.h"
#include "semLib.h"
#include "lstLib.h"	/* List library for linked structures. */

#ifdef __cplusplus
extern "C" {
#endif


#define I2C_INIT_STR_MAX 255
#define I2C_NAME_MAX 20          /* The maximum length of a device name */
#define I2C_DESC_MAX 80         /* The maximum length of device desription. */
#define ENO_I2C_MASTER 0xFFFFA5A5

/* options passed to the master read/write routines */
#define I2C_OPT_NO_STOP 0x00000001

/* flags */
#define I2C_SLAVE_FLG_FAST_MODE 			0x00000001 /* common 400kbs xfer rate */
#define I2C_SLAVE_FLG_STANDARD_MODE_ONLY 	0x00000002 /* old 100kbs xfer*/
#define I2C_SLAVE_FLG_HS_MODE 				0x00000004 /* 3.4 Mbs xfer */
#define I2C_SLAVE_FLG_7_BIT_ADDR 			0x80000000 /* normal 7 bit addressing */
#define I2C_SLAVE_FLG_10_BIT_ADDR 			0x40000000 /* normal 7 bit addressing */

#define I2C_MASTER_FLG_FAST_MODE 			0x00000001 /* common 400kbs xfer rate */
#define I2C_MASTER_FLG_STANDARD_MODE_ONLY 	0x00000002 /* old 100kbs xfer*/
#define I2C_MASTER_FLG_HS_MODE 				0x00000004 /* 3.4 Mbs xfer */

/*
 *
 * I2C_DEV_OBJ - device specific control object
 *
 * The I2C_DEV_OBJ structure is the glue linking generic
 * master and slave I2C device structure (I2C_MASTER_OBJ & I2C_SLAVE_OBJ)
 * with the device specific data object referenced by pDevice.
 */

typedef struct i2c_dev_obj 
    {
    char name[I2C_NAME_MAX];
    int unit;
    char description[I2C_DESC_MAX];
    void* pDevice;		/* Pointer back to the device data. */
    } I2C_DEV_OBJ;
	

/*
 *
 * i2cMasterObject - the basic end object that i2c masters derives from
 *
 * This data structure defines a device independant amount of state
 * that is maintained by all drivers/devices.
 * Each specific i2c master interface device derives from this object
 * first and then incorporates it's own data structures after it.
 *
 */
typedef struct i2c_master_object
    {
    NODE node;
    I2C_DEV_OBJ i2cDevObject;		/* Root of the device heirarchy. */
    BOOL attached;		/* Indicates unit is attached. */
    SEM_ID accessSem;		/* write semaphore. */
    UINT32 flags;			/* Various flags. */
    struct i2c_master_funcs *pFuncTable;	/* Function table. */
	LIST attachedSlaves;     /* slave i2c devices attached */
    } I2C_MASTER_OBJ;

    
/*
 *
 * i2cSlaveObject - the basic end object that i2c slaves derives from
 *
 * This data structure defines a device independant amount of state
 * that is maintained by all drivers/devices.
 * Each specific i2c slave device derives from this object
 * first and then incorporates it's own data structures after it.
 *
 */
typedef struct i2c_slave_object
    {
    NODE node;
    I2C_DEV_OBJ i2cDevObject;		/* Root of the device heirarchy. */
    UINT32 flags;			/* Various flags. */
    UINT32 i2cAddr;			/* I2C Slave address. */
	I2C_MASTER_OBJ * i2cMaster;
    } I2C_SLAVE_OBJ;
/*
 * 
 * I2C_MASTER_FUNCS - driver function table
 *
 * This is a table that is created, one per driver, to hold all the
 * function pointers for that driver.  In this way we can have only one
 * instance to this structer, but one pointer per netDevice structure.
 */
#if 0
typedef struct i2c_master_funcs
    {
    STATUS (*write) (I2C_MASTER_OBJ *, UINT8, UINT8*, UINT32, UINT32, int);	   /* Driver's write func. */
    STATUS (*read) (I2C_MASTER_OBJ *, UINT8, UINT8*, UINT32, UINT32, int);	   /* Driver's read func. */
    } I2C_MASTER_FUNCS; 
#else
typedef struct i2c_master_funcs
    {
    FUNCPTR write;
    FUNCPTR read;
    } I2C_MASTER_FUNCS; 
#endif

/* This structure is a row in our run time i2c master table. 
 * Each row contains all the i2c master units started on a particular named device.
 * I.e. all devices that are MPC8240 i2c masters will be in the list
 * off of the row with the name "mpc8240_i2c".
 */

typedef struct i2c_master_tbl_row
    {
    NODE node;                  /* Needed by our list processing library. */
    char name[I2C_NAME_MAX];	/* Name of the devices in row. (mpc8240_i2c, etc.) */
    LIST units;                 /* List of devices, i.e. 0, 1, etc..  */
    } I2C_MASTER_TBL_ROW;

/* This is the structure that is used by the BSP to build up a table
 * of I2C Master interface devices to be started at boot time.
 */
typedef struct i2c_master_tbl_entry
    {
    int unit;                                /* This device's unit # */
    I2C_MASTER_OBJ* (*i2cLoadFunc) (char*, void*);         /* The Load function. */
    char* i2cLoadString;                     /* The load string. */
    BOOL processed;                          /* Has this been processed? */
    } I2C_MASTER_TBL_ENTRY;
    
#define I2C_MASTER_TBL_END NULL

/* This structure is a row in our run time i2c slave table. 
 * Each row contains all the i2c slave units started on a particular named device.
 * I.e. all devices that are eeprom i2c masters will be in the list
 * off of the row with the name "eeprom_i2c".
 */

typedef struct i2c_slave_tbl_row
    {
    NODE node;                  /* Needed by our list processing library. */
    char name[I2C_NAME_MAX];	/* Name of the devices in row. (mpc8240_i2c, etc.) */
    LIST units;                 /* List of devices, i.e. 0, 1, etc..  */
    } I2C_SLAVE_TBL_ROW;

/* This is the structure that is used by the BSP to build up a table
 * of I2C Slave devices to be started at boot time.
 */
typedef struct i2c_slave_tbl_entry
    {
    int unit;                                /* This device's unit # */
    I2C_SLAVE_OBJ* (*i2cLoadFunc) (char*, void*);         /* The Load function. */
    char* i2cLoadString;                     /* The load string. */
    BOOL processed;                          /* Has this been processed? */
    } I2C_SLAVE_TBL_ENTRY;
    
#define I2C_SLAVE_TBL_END NULL 
   
void i2cAccessSemTake
    (
    I2C_MASTER_OBJ* pI2CMaster,
    int tmout
    );

void i2cAccessSemGive
    (
    I2C_MASTER_OBJ* pI2CMaster
    );

void i2cMasterFlagsClr
    (
    I2C_MASTER_OBJ* pI2CMaster,
    UINT32 clrBits
    ); 

void i2cMasterFlagsSet
    (
    I2C_MASTER_OBJ* pI2CMaster,
    UINT32 setBits
    );

int i2cMasterFlagsGet
    (
    I2C_MASTER_OBJ* pI2CMaster
    );

char* i2cMasterDevName
    (
    I2C_MASTER_OBJ* pI2CMaster
    ); 

void i2cSlaveAddrSet
    (
    I2C_SLAVE_OBJ* pI2CSlave,
    UINT32 i2cAddr
    );

UINT32 i2cSlaveAddrGet
    (
    I2C_SLAVE_OBJ* pI2CSlave
    );

I2C_MASTER_OBJ* i2cMasterFindByName
    (
    char* pName,			/* device name to search for */
    int unit
    );

I2C_SLAVE_OBJ* i2cSlaveFindByName
    (
    char* pName,			/* device name to search for */
    int unit
    );

STATUS i2cLibInit (void);

I2C_MASTER_OBJ * i2cMasterDevLoad
    (
    int unit,                   /* unit number of device */
    FUNCPTR i2cDrvLoad,	/* load function of the driver  */
    char* pInitString		/* init string for this driver  */
    );

I2C_SLAVE_OBJ * i2cSlaveDevLoad
    (
    int unit,                   /* unit number of device */
    FUNCPTR i2cDrvLoad,	/* load function of the driver  */
    char* pInitString		/* init string for this driver  */
    );

STATUS i2cMasterObjInit
    (
    I2C_MASTER_OBJ *	pI2CMasterObj,	/* object to be initialized */
    char *	pMasterBaseName,	/* device base name, for example, "mpc8240_i2c" */
    int		masterUnitNo,		/* unit number */
    I2C_MASTER_FUNCS * pMasterFuncTable,	/* master device functions */
    char*   pMasterDescription,
    UINT32  masterFlags
    );

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
    );

STATUS i2cMasterObjFlagSet
    (
    I2C_MASTER_OBJ * pI2CMaster,
    UINT32	flags
    );

STATUS i2cWrite
    (
    I2C_SLAVE_OBJ * pI2CSlave, 	/* i2c slave device structure */
    UINT8 * pBuff,	/* data to be written */
    UINT32 nBytes,	/* number of bytes to write */
    UINT32 options,	/* transaction options */
	int timeout     /* timeout in ticks */
    );

STATUS i2cRead
    (
    I2C_SLAVE_OBJ * pI2CSlave, 	/* i2c slave device structure */
    UINT8 * pBuff,	/* data buffer to read into */
    UINT32 nBytes,	/* number of bytes to read */
    UINT32 options,	/* transaction options */
	int timeout     /* timeout in ticks */
    );

#ifdef __cplusplus
}
#endif

#endif /* __INCi2cLibh */
