/* sysMpc8540I2c.h - Low Level mpc8540 I2C Header File */

/* Copyright 2004 Motorola, Inc., All Rights Reserved */

/*
modification history
--------------------
01a,09nov04,scb Created.
*/

/* 
DESCRIPTION
This file contains the mpc8540 I2C definitions.
*/

#ifndef INCsysMpc8540I2ch
#define INCsysMpc8540I2ch

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

/* Low level function bindings */

#define I2C_CYCLE_START		sys8540I2cStart
#define I2C_CYCLE_STOP		sys8540I2cStop
#define I2C_CYCLE_READ		sys8540I2cRead
#define I2C_CYCLE_WRITE		sys8540I2cWrite
#define I2C_CYCLE_ACKIN		sys8540I2cAckIn
#define I2C_CYCLE_ACKOUT	sys8540I2cAckOut
#define I2C_KNOWN_STATE		sys8540I2cKnownState

/* Device address byte counts */

#define I2C_SINGLE_BYTE_ADDR	(1)
#define I2C_DUAL_BYTE_ADDR	(2)
#define I2C_TRIPLE_BYTE_ADDR	(3)

#ifdef __cplusplus
    }
#endif

#endif /* INCsysMpc8540I2ch */
