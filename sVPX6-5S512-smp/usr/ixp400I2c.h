/* ixp400I2c.h - Intel IXP400 I2c header file */

/*
Copyright (c) 2002, 2004-2005 Wind River Systems, Inc.  All rights reserved.
Certain portions of these files may be copyright (c) 2002-2005 Intel
Corporation.
*/

/*
modification history
--------------------
01c,28feb05,pai  Picked up Intel mods to init functions.
01b,20dec04,pai  Updated copyright notice.
01a,05jun02,jb  initial version...
*/

#ifndef __INCixp400I2ch 
#define __INCixp400I2ch


#ifdef __cplusplus
extern "C" {
#endif

#define IXP400_I2C_READ_FLAG 0x1	/* Read from slave */
#define IXP400_I2C_WRITE_MSK 0xFE	/* Write to slave */
#define IXP400_I2C_ACK_RTY 5	/* Acknowledge Receive retry count */


/* Function Declarations */

STATUS ixp400I2CStart ();
void ixp400I2CStop ();
void ixp400I2CAckSend ();
STATUS ixp400I2CAckReceive ();
void ixp400I2CByteTransmit (unsigned char dataByte);
void ixp400I2CByteReceive (unsigned char *dataByte);
STATUS ixp400I2CWriteTransfer (UINT8 devAddr, UINT8 *buffer, UINT32 num, UINT8 offset);
STATUS ixp400I2CReadTransfer (UINT8 devAddr, UINT8 *buffer, UINT32 num, UINT8 offset);

#ifdef INCLUDE_I2C_HW_CTRL
UINT32	ixp400I2CInit (void);
#endif 

#ifdef __cplusplus
}
#endif

#endif /* __INCixp400I2ch */ 
