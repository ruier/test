/* sysMpc8641I2c.h - Low Level mpc8641 I2C Header File */

/*
 * Copyright (c) 2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 2004 Motorola, Inc., All Rights Reserved */

/*
modification history
--------------------
01b,17mar08,mdg Added support for the 8641D.
01a,09nov04,scb Created.
*/

/* 
DESCRIPTION
This file contains the mpc8641 I2C definitions.
*/

#ifndef INCsysMpc8641I2ch
#define INCsysMpc8641I2ch

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

/* Low level function bindings */

#define I2C_CYCLE_START     sys8641I2cStart
#define I2C_CYCLE_STOP      sys8641I2cStop
#define I2C_CYCLE_READ      sys8641I2cRead
#define I2C_CYCLE_WRITE     sys8641I2cWrite
#define I2C_CYCLE_ACKIN     sys8641I2cAckIn
#define I2C_CYCLE_ACKOUT    sys8641I2cAckOut
#define I2C_KNOWN_STATE     sys8641I2cKnownState

/* Device address byte counts */

#define I2C_SINGLE_BYTE_ADDR    (1)
#define I2C_DUAL_BYTE_ADDR      (2)
#define I2C_TRIPLE_BYTE_ADDR    (3)

#ifdef __cplusplus
    }
#endif

#endif /* INCsysMpc8641I2ch */
