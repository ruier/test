/* sysAdt7461TempSensor.c - ADT7461 Temperature Sensor Library */

/*
 * Copyright (c) 2008-2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 2004-2005 Motorola, Inc. All Rights Reserved */

/*
modification history
--------------------
01d,20jul09,d_l  fix undefined symbol sysAdt7461Show.
01c,05mar09,mdg  Added monitor task to offload interrupts so that
                  I2C calls are not made in interrupt context.
01b,02mar09,mdg  Modified I2C calls to use sysMotI2C calls so that
                  they are AMP and SMP safe.
01a,17mar08,mdg  Ported from 01a, mv3100/sysDs1621TempSensor.
*/

/*
DESCRIPTION

This file contains the library routines for the ADT7461 Digital
Thermometer and Thermostat.  This driver is not (in and of itself)
intended to be safe for multi-threaded environments.  That support is
intended for the application software that overlays this driver.

The ADT7461 measures temperatures from 0C to +127C.
Data is read from/written via a 2-wire serial interface
(I2C).  The temperature may be read from two different physical
locations, the ADT7461 itself (on the board) and a remote location
(in this case the temperature of the CPU).  The readings are
provided in a 8-bit unsigned number.

The ADT7461 can be programmed to perform continuous consecutive
conversions (continuous-conversion mode) or to perform single
conversions on command (one-shot mode).  This is user-selectable.
Note that the ADT7461 always powers up in the continuous conversion
state.

The ADT7461 can also operate in two different modes: thermostat
and thermometer.  In thermometer mode the ADT7461's interrupts
will be disabled, and TH and TL will be set to their
absolute maximum and minimum, respectively.  In this mode
application software can poll the chip for current temperature
using the supplied interface routines.

In thermostat mode the ADT7461's interrupts will be enabled
and TH and TL will be set to user-specified values.
When the ADT7461's temperature meets or exceeds the value stored
in the high register trip register (TH), the output becomes active
and will stay active until the temperature falls below the
temperature stored in the low temperature trigger register (TL).
For this reason the interrupt handler for the ADT7461 will disable
the interrupt and then call a user-specified interrupt handler,
if an interrupt handler has been connected.  The interrupts will
not be enabled again until the ADT7461's enable routine is called.
This will allow the user's application code to poll the chip to
obtain the temperature, perhaps until it falls below the value
contained in TL, and then reenable the ADT7461 once it does.  If
the interrupt were not disabled the chip would continue to signal
an interrupt until the temperature drops below TL.

The ADT7461 contains the following registers:

Configuration Register
Temperature Register
Temperature High Register
Temperature Low Register
CPU Temperature Register
CPU Temperature High Register
CPU Temperature Low Register
Status Register

This library provides the following routines:

STATUS sysAdt7461Show (void)

This routine will display to current board temperature, the value of
the Temperature High register, the value of the Temperature Low register,
the current CPU temperature, the value of the CPU temperature Low register,
the value of the CPU temperature high register,and a few configuration
options of the ADT7461.  INCLUDE_SHOW_ROUTINES must be defined in order
to use this routine.

STATUS sysAdt7461Connect (FUNCPTR)

This routine can be called to connect a user-specified interrupt handler
to the ADT7461's interrupt.  sysAdt7461Connect() allows only one routine
to be connected and multiple calls to sysAdt7461Connect result in
only the last call being the effective one.

STATUS sysAdt7461THRegWrite (UINT8)

This routine can be used to write to the Temperature High register.

STATUS sysAdt7461THRegRead (UINT8 *)

This routine can be used to read from the Temperature High register.

STATUS sysAdt7461CpuTHRegWrite (UINT8)

This routine can be used to write to the CPU Temperature High register.

STATUS sysAdt7461CpuTHRegRead (UINT8 *)

This routine can be used to read from the CPU Temperature High register.

STATUS sysAdt7461TemperatureHighShow (void)

This routine can be used to display temperature programmed in the
Temperature High register.

STATUS sysAdt7461CpuTemperatureHighShow (void)

This routine can be used to display temperature programmed in the
CPU Temperature High register.

STATUS sysAdt7461TLRegWrite (UINT8)

This routine can be used to write to the Temperature Low register.

STATUS sysAdt7461TLRegRead (UINT8 *)

This routine can be used to read from the Temperature Low register.

STATUS sysAdt7461CpuTLRegWrite (UINT8)

This routine can be used to write to the Cpu Temperature Low register.

STATUS sysAdt7461CpuTLRegRead (UINT8 *)

This routine can be used to read from the Cpu Temperature Low register.

STATUS sysAdt7461TemperatureLowShow (void)

This routine can be used to display temperature programmed in the
Temperature Low register.

STATUS sysAdt7461CpuTemperatureLowShow (void)

This routine can be used to display temperature programmed in the
Cpu Temperature Low register.

STATUS sysAdt7461ConfigRegRead (UINT8 *)

This routine can be used to read from the Configuration register.

STATUS sysAdt7461ConfigRegWrite (UINT8)

This routine can be used to write to the Configuration register.

STATUS sysAdt7461StatusRegRead (UINT8 *)

This routine can be used to read from the Status register.

STATUS sysAdt7461TemperatureGet (UINT8 *temp)

This routine can be used to obtain the temperature of the board.

STATUS sysAdt7461CpuTemperatureGet (UINT8 *temp)

This routine can be used to obtain the temperature of the CPU.

STATUS sysAdt7461TemperatureShow (void)

This routine is used to display the temperature of the board.

STATUS sysAdt7461CpuTemperatureShow (void)

This routine is used to display the temperature of the CPU.

STATUS sysAdt7461Config (UINT32, UINT32, UINT8, UINT8, UINT8, UINT8)

This routine is used to configure the ADT7461.  The parameters required for
this routine are: the conversion mode to be used (ADT7461_CONV_MODE_CONT or
ADT7461_CONV_MODE_1SHOT), the operating mode (ADT7461_MODE_THERMOSTAT or
ADT7461_MODE_THERMOMETER), and the values for Temperature High,Temperature Low,
CPU Temperature High, and CPU Temperature Low.

This routine must be called before enabling the ADT7461.  Once the ADT7461 has
been enabled it is possible to reconfigure without explicitly calling the
sysAdt7461Disable() routine.  However, the routine will call
sysAdt7461Disable() in order to temporarily disable the ADT7461 while it
reconfigures it.  After which it will call sysAdt7461Enable to reenable it.

STATUS sysAdt7461Enable (void)

This routine is used to enable (ie. start) the ADT7461 once it has been
configured by calling sysAdt7461Config() above.

STATUS sysAdt7461Disable (void)

This routine is used to disable (ie. stop) the ADT7461.
*/

/* includes */

#include "sysMpc8641I2c.h"

/* defines */

#define ADT7461_READ_TEMP_CMD       0x00
#define ADT7461_READ_CPU_TEMP_CMD   0x01
#define ADT7461_READ_STATUS_CMD     0x02
#define ADT7461_READ_CONFIG_CMD     0x03
#define ADT7451_READ_RATE_CMD       0x04
#define ADT7461_READ_TH_CMD         0x05
#define ADT7461_READ_TL_CMD         0x06
#define ADT7461_READ_CPU_TH_CMD     0x07
#define ADT7461_READ_CPU_TL_CMD     0x08
#define ADT7461_WRITE_CONFIG_CMD    0x09
#define ADT7451_WRITE_RATE_CMD      0x0A
#define ADT7461_WRITE_TH_CMD        0x0B
#define ADT7461_WRITE_TL_CMD        0x0C
#define ADT7461_WRITE_CPU_TH_CMD    0x0D
#define ADT7461_WRITE_CPU_TL_CMD    0x0E
#define ADT7461_WRITE_ONE_SHOT_CMD  0x0F

#define ADT7461_1SHOT_MASK          (1 << 6)
#define ADT7461_DONE_MASK           (1 << 0)
#define ADT7461_MODE_THERMOSTAT     0
#define ADT7461_MODE_THERMOMETER    1
#define ADT7461_CONV_MODE_CONT      0
#define ADT7461_CONV_MODE_1SHOT     1

#define ADT7461_TASK_PRIORITY       1
#define ADT7461_STACK_SIZE          4096

#define ADT7461_DEBUG

/* typedefs */

/* globals */

/* locals */

LOCAL BOOL isAdt7461Initialized = FALSE;
LOCAL SEM_ID sysAdt7461Sem;
LOCAL FUNCPTR sysAdt7461Routine = NULL;
LOCAL int sysAdt7461RoutineArg = 0x0;
LOCAL BOOL sysAdt7461Mode = ADT7461_MODE_THERMOSTAT;
LOCAL BOOL isAdt7461Running = FALSE;
#ifdef ADT7461_DEBUG
    UINT32 intCounter = 0;
#endif

/* forward declarations */

#ifdef INCLUDE_SHOW_ROUTINES
    STATUS sysAdt7461Show (void);
#endif /* INCLUDE_SHOW_ROUTINES */
STATUS sysAdt7461Connect (FUNCPTR, int);
#ifdef ADT7461_DEBUG
    LOCAL void sysAdt7461UsrInt (int);
#endif /* ADT7461_DEBUG */
LOCAL void   sysAdt7461Int (void);
LOCAL void   sysAdt7461IntDisable (void);
LOCAL void   sysAdt7461IntEnable (void);
LOCAL void   sysAdt7461IntConnect (void);
LOCAL STATUS sysAdt7461RegWrite (UCHAR, UINT8);
LOCAL STATUS sysAdt7461RegRead (UCHAR, UINT8 *);
STATUS sysAdt7461THRegWrite (UINT8);
STATUS sysAdt7461THRegRead (UINT8 *);
STATUS sysAdt7461TemperatureHighShow (void);
STATUS sysAdt7461TLRegWrite (UINT8);
STATUS sysAdt7461TLRegRead (UINT8 *);
STATUS sysAdt7461TemperatureLowShow (void);
STATUS sysAdt7461ConfigRegRead (UINT8 *);
STATUS sysAdt7461ConfigRegWrite (UINT8);
STATUS sysAdt7461StatusRegRead (UINT8 *);
LOCAL STATUS sysAdt7461TempRegsRead (UCHAR, UINT8 *);
STATUS sysAdt7461TemperatureGet (UINT8 *);
STATUS sysAdt7461CpuTemperatureGet (UINT8 *);
STATUS sysAdt7461TemperatureShow (void);
STATUS sysAdt7461CpuTemperatureShow (void);
STATUS sysAdt7461CpuTemperatureLowShow (void);
STATUS sysAdt7461CpuTemperatureHighShow (void);
LOCAL void sysAdt7461Init (void);
STATUS sysAdt7461Config (UINT32, UINT32, UINT8, UINT8, UINT8, UINT8);
STATUS sysAdt7461Enable (void);
STATUS sysAdt7461Disable (void);
#ifdef ADT7461_DEBUG
    STATUS sysAdt7461Test (void);
#endif /* ADT7461_DEBUG */

/* externals */

IMPORT STATUS sysMotI2cRead (UINT32 devAddr, UINT32 devOffset,
                                  UINT32 devAddrBytes, UCHAR *spdData,
                                  UINT32 numBytes);
IMPORT STATUS sysMotI2cWrite (UINT32 devAddr, UINT32 devOffset,
                                  UINT32 devAddrBytes, UINT8 *pBfr,
                                  UINT32 numBytes);

#ifdef INCLUDE_SHOW_ROUTINES

/*******************************************************************************
*
* sysAdt7461Show - show routine for the ADT7461
*
* This routine will display the current registers settings and temperature
* readings from the ADT7461.
*
* RETURNS: OK if successful, ERROR otherwise.
*/

STATUS sysAdt7461Show (void)
    {
    UINT8 configReg = 0;

    if (sysAdt7461TemperatureShow() == ERROR)
        {
        printf("DB: sysAdt7461Show - sysAdt7461TemperatureShow ERROR\n\r");
        return (ERROR);
        }
    if (sysAdt7461TemperatureLowShow() == ERROR)
        {
        printf("DB: sysAdt7461Show - sysAdt7461TemperatureLowShow ERROR\n\r");
        return (ERROR);
        }
    if (sysAdt7461TemperatureHighShow() == ERROR)
        {
        printf("DB: sysAdt7461Show - sysAdt7461TemperatureHighShow ERROR\n\r");
        return (ERROR);
        }

    if (sysAdt7461CpuTemperatureShow() == ERROR)
        {
        printf("DB: sysAdt7461Show - sysAdt7461CpuTemperatureShow ERROR\n\r");
        return (ERROR);
        }
    if (sysAdt7461CpuTemperatureLowShow() == ERROR)
        {
        printf("DB: sysAdt7461Show - sysAdt7461CpuTemperatureLowShow ERROR\n\r");
        return (ERROR);
        }
    if (sysAdt7461CpuTemperatureHighShow() == ERROR)
        {
        printf("DB: sysAdt7461Show - sysAdt7461CpuTemperatureHighShow ERROR\n\r");
        return (ERROR);
        }

    if (sysAdt7461ConfigRegRead(&configReg) == ERROR)
        {
        printf("DB: sysAdt7461Show - sysAdt7461ConfigRegRead ERROR\n\r");
        return (ERROR);
        }

    if (configReg & ADT7461_1SHOT_MASK)
        printf("ADT7461 is configured for 1-shot conversion.\n\r");
    else
        printf("ADT7461 is configured for continuous conversion.\n\r");

    return (OK);
    }
#endif /* INCLUDE_SHOW_ROUTINES */

/*******************************************************************************
*
* sysAdt7461Connect - connect a user-defined interrupt handler
*
* This routine connects a user specifed interrupt handler routine to be
* called by the ADT7461's interrupt handler.
*
* RETURNS: OK always.
*/

STATUS sysAdt7461Connect
    (
    FUNCPTR routine,    /* user-defined routine */
    int arg     /* argument */
    )
    {
    sysAdt7461Routine = routine;
    sysAdt7461RoutineArg = arg;
    return (OK);
    }

#ifdef ADT7461_DEBUG

/*******************************************************************************
*
* sysAdt7461UsrInt - test interrupt handler for the ADT7461 temperature sensor
*
* This routine is a test interrupt handler for the ADT7461 temperature sensor.
*
* RETURNS: N/A
*/

LOCAL void sysAdt7461UsrInt
    (
    int arg /* argument - not used */
    )
    {
    intCounter += 1;
    }
#endif /* ADT7461_DEBUG */

/*******************************************************************************
*
* sysAdt7461Int - interrupt handler for the ADT7461 temperature sensor
*
* This routine is the interrupt handler for the ADT7461 temperature sensor.
*
* RETURNS: N/A
*/

LOCAL void sysAdt7461Int (void)
    {
    if (((*(UCHAR *)(BRD_INTERRUPT_REG_2)) &
          (BRD_INTERRUPT_REG_2_TEMP | BRD_INTERRUPT_REG_2_TEMP_MASK)) ==
         BRD_INTERRUPT_REG_2_TEMP)
        {
        semGive(sysAdt7461Sem);

        /* Disable interrupts until the monitor tasks clear it */

        sysAdt7461IntDisable();
        }
    }

/*******************************************************************************
*
* sysAdt7461IntDisable - disable ADT7461 interrupts
*
* This routine will disable ADT7461 interrupts.
*
* RETURNS: N/A
*/

LOCAL void sysAdt7461IntDisable (void)
    {
    UCHAR temp = 0;

    /* set TSTAT_MASK bit in system status register 2 */

    *(UCHAR *)(BRD_INTERRUPT_REG_2) |= BRD_INTERRUPT_REG_2_TEMP_MASK;

    /* read it back to push the value to the register */

    temp = *(UCHAR *)(BRD_INTERRUPT_REG_2);

    }

/*******************************************************************************
*
* sysAdt7461IntEnable - enable ADT7461 interrupts
*
* This routine will enable ADT7461 interrupts.
*
* RETURNS: N/A
*/

LOCAL void sysAdt7461IntEnable (void)
    {
    UCHAR temp = 0;

    /* clear TSTAT_MASK bit in system status register 2 */

    *(UCHAR *)(BRD_INTERRUPT_REG_2) &= ~BRD_INTERRUPT_REG_2_TEMP_MASK;

    /* read it back to ensure the value is written to the register */

    temp = *(UCHAR *)(BRD_INTERRUPT_REG_2);

    }

/*******************************************************************************
*
* sysAdt7461IntConnect - connect ADT7461 interrupts
*
* This routine will connect ADT7461 interrupts.
*
* RETURNS: N/A
*/

LOCAL void sysAdt7461IntConnect (void)
    {
    intConnect (INUM_TO_IVEC(TEMP_INT_VEC), (VOIDFUNCPTR)sysAdt7461Int, 0);
    intEnable (TEMP_INT_VEC);
    }

/*******************************************************************************
*
* sysAdt7461RegWrite - routine to write to ADT7461 registers
*
* This routine is provided to write to the ADT7461 registers that are only
* one byte in size.
*
* RETURNS: OK if write succeeded, ERROR otherwise.
*/

LOCAL STATUS sysAdt7461RegWrite
    (
    UCHAR command,  /* ADT7461 Command */
    UINT8 data      /* data to be written */
    )
    {
    return(sysMotI2cWrite(ADT7461_TEMP_SENSOR_I2C_ADDR,
                          command, 1, &data, 1));
    }

/*******************************************************************************
*
* sysAdt7461RegRead - routine to read from the ADT7461 registers
*
* This routine is provided to read from the ADT7461 registers which are one
* byte in size.
*
* RETURNS: OK if successful, ERROR otherwise.
*/
LOCAL STATUS sysAdt7461RegRead
    (
    UCHAR command,  /* ADT7461 command */
    UINT8 * buffer  /* buffer for data that is read */
    )

    {
    return(sysMotI2cRead(ADT7461_TEMP_SENSOR_I2C_ADDR,
                          command, 1, buffer, 1));
    }

/*******************************************************************************
*
* sysAdt7461ConversionStart - start ADT7461 temperature conversion
*
* This routine will start the ADT7461 temperature conversion in one shot mode
*
* RETURNS: OK if conversion started, ERROR otherwise
*/

STATUS sysAdt7461ConversionStart (void)
    {
    return (sysAdt7461RegWrite(ADT7461_WRITE_ONE_SHOT_CMD, 0));
    }

/*******************************************************************************
*
* sysAdt7461THRegWrite - write to the TH register
*
* This routine will write a given value to the TH (Temperature High) register.
*
* RETURNS: OK if successful, ERROR otherwise
*/

STATUS sysAdt7461THRegWrite
    (
    UINT8 temp  /* data to be written */
    )
    {
    return (sysAdt7461RegWrite(ADT7461_WRITE_TH_CMD, temp));
    }

/*******************************************************************************
*
* sysAdt7461THRegRead - read from the TH register
*
* This routine will read from the TH (Temperature High) register.
*
* RETURNS: OK if successful, ERROR otherwise
*/

STATUS sysAdt7461THRegRead
    (
    UINT8 * temp    /* read data */
    )
    {
    return (sysAdt7461RegRead(ADT7461_READ_TH_CMD, temp));
    }

/*******************************************************************************
*
* sysAdt7461TemperatureHighShow - display contents of TH register
*
* This routine will display the (interpreted) contents of the Temperature
* High register.
*
* RETURNS: OK if successful, ERROR otherwise
*/

STATUS sysAdt7461TemperatureHighShow (void)
    {
    UINT8 temp = 0;

    if (sysAdt7461THRegRead(&temp) == ERROR)
        {
        printf("DB: sysAdt7461TemperatureHighShow - sysAdt7461THRegRead ERROR\n\r");
        return (ERROR);
        }

    printf("Temperature High = %d\n\r", temp);

    return (OK);
    }

/*******************************************************************************
*
* sysAdt7461TLRegWrite - write to the TL register
*
* This routine will write a given value to the TL (Temperature Low) register.
*
* RETURNS: OK if successful, ERROR otherwise
*/

STATUS sysAdt7461TLRegWrite
    (
    UINT8 temp  /* data to be written */
    )
    {
    return (sysAdt7461RegWrite(ADT7461_WRITE_TL_CMD, temp));
    }

/*******************************************************************************
*
* sysAdt7461TLRegRead - read from the TL register
*
* This routine will read from the TL (Temperature Low) register.
*
* RETURNS: OK if successful, ERROR otherwise
*/

STATUS sysAdt7461TLRegRead
    (
    UINT8 * temp    /* read data */
    )
    {
    return (sysAdt7461RegRead(ADT7461_READ_TL_CMD, temp));
    }

/*******************************************************************************
*
* sysAdt7461TemperatureLowShow - display the contents of the TL register
*
* This routine will display the (interepreted) contents of the temperature
* low register.
*
* RETURNS: OK if successful, ERROR otherwise
*/

STATUS sysAdt7461TemperatureLowShow (void)
    {
    UINT8 temp = 0;

    if (sysAdt7461TLRegRead(&temp) == ERROR)
        {
        printf("DB: sysAdt7461TemperatureLowShow - sysAdt7461TLRegRead ERROR\n\r");
        return (ERROR);
        }

    printf("Temperature Low = %d\n\r", temp);

    return (OK);
    }

/*******************************************************************************
*
* sysAdt7461CpuTHRegWrite - write to the CPU TH register
*
* This routine will write a given value to the CPU TH (Temperature High)
*  register.
*
* RETURNS: OK if successful, ERROR otherwise
*/

STATUS sysAdt7461CpuTHRegWrite
    (
    UINT8 temp  /* data to be written */
    )
    {
    return (sysAdt7461RegWrite(ADT7461_WRITE_CPU_TH_CMD, temp));
    }

/*******************************************************************************
*
* sysAdt7461CpuTHRegRead - read from the CPU TH register
*
* This routine will read from the CPU TH (Temperature High) register.
*
* RETURNS: OK if successful, ERROR otherwise
*/

STATUS sysAdt7461CpuTHRegRead
    (
    UINT8 * temp    /* read data */
    )
    {
    return (sysAdt7461RegRead(ADT7461_READ_CPU_TH_CMD, temp));
    }

/*******************************************************************************
*
* sysAdt7461CpuTemperatureHighShow - display contents of CPU TH register
*
* This routine will display the (interpreted) contents of the CPU Temperature
* High register.
*
* RETURNS: OK if successful, ERROR otherwise
*/

STATUS sysAdt7461CpuTemperatureHighShow (void)
    {
    UINT8 temp = 0;

    if (sysAdt7461CpuTHRegRead(&temp) == ERROR)
        {
        printf("DB: sysAdt7461CpuTemperatureHighShow"
               " - sysAdt7461CPUTHRegRead ERROR\n\r");
        return (ERROR);
        }

    printf("CPU Temperature High = %d\n\r", temp);

    return (OK);
    }

/*******************************************************************************
*
* sysAdt7461CpuTLRegWrite - write to the CPU TL register
*
* This routine will write a given value to the CPU TL (Temperature Low)
*  register.
*
* RETURNS: OK if successful, ERROR otherwise
*/

STATUS sysAdt7461CpuTLRegWrite
    (
    UINT8 temp  /* data to be written */
    )
    {
    return (sysAdt7461RegWrite(ADT7461_WRITE_CPU_TL_CMD, temp));
    }

/*******************************************************************************
*
* sysAdt7461CpuTLRegRead - read from the CPU TL register
*
* This routine will read from the TCPU TL (Temperature Low) register.
*
* RETURNS: OK if successful, ERROR otherwise
*/

STATUS sysAdt7461CpuTLRegRead
    (
    UINT8 * temp    /* read data */
    )
    {
    return (sysAdt7461RegRead(ADT7461_READ_CPU_TL_CMD, temp));
    }

/*******************************************************************************
*
* sysAdt7461CpuTemperatureLowShow - display the contents of the CPU TL register
*
* This routine will display the (interepreted) contents of the CPU temperature
* low register.
*
* RETURNS: OK if successful, ERROR otherwise
*/

STATUS sysAdt7461CpuTemperatureLowShow (void)
    {
    UINT8 temp = 0;

    if (sysAdt7461CpuTLRegRead(&temp) == ERROR)
        {
        printf("DB: sysAdt7461CpuTemperatureLowShow "
               "- sysAdt7461CpuTLRegRead ERROR\n\r");
        return (ERROR);
        }

    printf("CPU Temperature Low = %d\n\r", temp);

    return (OK);
    }

/*******************************************************************************
*
* sysAdt7461ConfigRegRead - read from the config register
*
* This routine will read from the Configuration register.
*
* RETURNS: OK if successful, ERROR otherwise
*/

STATUS sysAdt7461ConfigRegRead (UINT8 *config)
    {
    return (sysAdt7461RegRead(ADT7461_READ_CONFIG_CMD,config));
    }

/*******************************************************************************
*
* sysAdt7461ConfigRegWrite - write to the config register
*
* This routine will write a given value to the configuration register.
*
* RETURNS: OK if successful, ERROR otherwise
*/

STATUS sysAdt7461ConfigRegWrite
    (
    UINT8 data  /* data to be written to config register */
    )
    {
    return (sysAdt7461RegWrite(ADT7461_WRITE_CONFIG_CMD, data));
    }

/*******************************************************************************
*
* sysAdt7461RateRegRead - read from the converstion rate register
*
* This routine will read from the converstion rate register.
*
* RETURNS: OK if successful, ERROR otherwise
*/

STATUS sysAdt7461RateRegRead (UINT8 *config)
    {
    return (sysAdt7461RegRead(ADT7451_READ_RATE_CMD,config));
    }

/*******************************************************************************
*
* sysAdt7461ConfigRegWrite - write to the converstion rate register
*
* This routine will write a given value to the converstion rate register.
*
* RETURNS: OK if successful, ERROR otherwise
*/

STATUS sysAdt7461RateRegWrite
    (
    UINT8 data  /* data to be written to rate register */
    )
    {
    return (sysAdt7461RegWrite(ADT7451_WRITE_RATE_CMD, data));
    }

/*******************************************************************************
*
* sysAdt7461StatusRegRead - read from the status register
*
* This routine will read from the status register.
*
* RETURNS: value of status register if successful, 0 otherwise
*/

STATUS sysAdt7461StatusRegRead (UINT8 *status)
    {
    return (sysAdt7461RegRead(ADT7461_READ_STATUS_CMD,status));
    }

/*******************************************************************************
*
* sysAdt7461TempRegsRead - retrieve temperature data from the ADT7461
*
* This routine retrieves temperature data from the specified ADT7461
* temperature register which is two bytes in size.
*
* RETURNS: OK if temperature data retrieved successfully, ERROR otherwise
*/

LOCAL STATUS sysAdt7461TempRegsRead
    (
    UCHAR command,  /* ADT7461 command */
    UINT8 * temp    /* Read data */
    )
    {
    UINT32 timeOut;
    UINT8 configReg;
    UINT8 statusReg;

    if (command == ADT7461_READ_TEMP_CMD ||
        command == ADT7461_READ_CPU_TEMP_CMD)
        {
        if (sysAdt7461ConfigRegRead(&configReg) == ERROR)
            {
            printf("DB: sysAdt7461TempRegsRead - sysAdt7461ConfigRegRead ERROR\n\r");
            return (ERROR);
            }

        if (configReg & ADT7461_1SHOT_MASK)
            {

            /* one-shot mode, initiate conversion for accurate reading */

            if (sysAdt7461ConversionStart() == ERROR)
                {
                printf("DB: sysAdt7461TempRegsRead - sysAdt7461ConversionStart ERROR\n\r");
                return (ERROR);
                }

            /* poll until conversion is complete */

            for (timeOut = 20; timeOut > 0; timeOut--)
                {
                if (sysAdt7461StatusRegRead(&statusReg) == ERROR)
                    {
                    printf("DB: sysAdt7461TempRegsRead - sysAdt7461StatusRegRead ERROR\n\r");
                    return (ERROR);
                    }
                if (!(statusReg & ADT7461_DONE_MASK))
                    break;
                taskDelay(1);
                }
            if (!timeOut)
                return (ERROR);
            }
        }

    if (sysAdt7461RegRead(command,temp) == ERROR)
        {
        printf("DB: sysAdt7461TempRegsRead - sysAdt7461RegRead ERROR\n\r");
        return (ERROR);
        }

    return (OK);
    }

/*******************************************************************************
*
* sysAdt7461TemperatureGet - retrieve current temperature
*
* This routine will retrieve the current temperature with the highest
* precision possible.
*
* RETURNS: OK if temperature retrieved successfully, ERROR otherwise.
*/

STATUS sysAdt7461TemperatureGet
    (
    UINT8 *temp
    )
    {

    if (sysAdt7461TempRegsRead(ADT7461_READ_TEMP_CMD, temp) == ERROR)
        {
        printf("DB: sysAdt7461TemperatureGet - sysAdt7461TempRegsRead ERROR\n\r");
        return (ERROR);
        }

    return (OK);
    }

/*******************************************************************************
*
* sysAdt7461CpuTemperatureGet - retrieve current CPU temperature
*
* This routine will retrieve the current CPU temperature with the highest
* precision possible.
*
* RETURNS: OK if temperature retrieved successfully, ERROR otherwise.
*/

STATUS sysAdt7461CpuTemperatureGet
    (
    UINT8 *temp
    )
    {

    if (sysAdt7461TempRegsRead(ADT7461_READ_CPU_TEMP_CMD, temp) == ERROR)
        {
        printf("DB: sysAdt7461CpuTemperatureGet - sysAdt7461TempRegsRead ERROR\n\r");
        return (ERROR);
        }

    return (OK);
    }

/*******************************************************************************
*
* sysAdt7461TemperatureShow - display current ADT7461 temperature
*
* This routine will display the current temperature as measured by the ADT7461.
*
* RETURNS: OK if successful, ERROR otherwise.
*/

STATUS sysAdt7461TemperatureShow (void)
    {
    UINT8 temp;

    if (sysAdt7461TemperatureGet(&temp) == ERROR)
        {
        printf("DB: sysAdt7461TemperatureShow - sysAdt7461TemperatureGet ERROR\n\r");
        return (ERROR);
        }

    printf("Board Temperature = %d\n\r", temp);

    return (OK);
    }

/*******************************************************************************
*
* sysAdt7461CpuTemperatureShow - display current CPU  temperature
*
* This routine will display the current CPU temperature as measured by the
*  ADT7461.
*
* RETURNS: OK if successful, ERROR otherwise.
*/

STATUS sysAdt7461CpuTemperatureShow (void)
    {
    UINT8 temp;

    if (sysAdt7461CpuTemperatureGet(&temp) == ERROR)
        {
        printf("DB: sysAdt7461CpuTemperatureShow "
               "- sysAdt7461CpuTemperatureGet ERROR\n\r");
        return (ERROR);
        }

    printf("CPU Temperature = %d\n\r", temp);

    return (OK);
    }

/*******************************************************************************
*
* sysAdt7461Mon - monitor task routine to process alarms in task context
*
* This routine is the routine ran by the tAdtMon task.
*
* RETURNS: N/A
*/

LOCAL void sysAdt7461Mon(void)
    {
    UINT8 statusReg = 0;

    FOREVER
        {
        semTake(sysAdt7461Sem, WAIT_FOREVER);

        if (sysAdt7461StatusRegRead(&statusReg) == ERROR)
            return;

        if (statusReg & 0x7C)
            {

            /* we've confirmed that we are the source of the interrupt, THF=1 */

            sysAdt7461Disable();

            /* call user-specified interrupt handler */

            if (sysAdt7461Routine != NULL)
                {
                (*sysAdt7461Routine)(sysAdt7461RoutineArg);
                }
            }
        }
    }

/*******************************************************************************
*
* sysAdt7461Init - initialization routine for the ADT7461
*
* This routine is the initialization routine for the ADT7461.
*
* RETURNS: N/A
*/

LOCAL void sysAdt7461Init (void)
    {
    if (!isAdt7461Initialized)
        {
        sysAdt7461Sem = semBCreate(SEM_Q_PRIORITY, SEM_EMPTY);

        /* connect interrupt service routine to system interrupt table */

        taskSpawn("tAdtMon",ADT7461_TASK_PRIORITY,0,
                  ADT7461_STACK_SIZE,(FUNCPTR) sysAdt7461Mon,
                  0,0,0,0,0,0,0,0,0,0);

#ifdef INCLUDE_AMP
        if (!CPU1CHECK)
            {
#endif /* INCLUDE_AMP */
            sysAdt7461IntConnect();
#ifdef INCLUDE_AMP
            }
#endif /* INCLUDE_AMP */
        isAdt7461Initialized = TRUE;
        }
    }

/*******************************************************************************
*
* sysAdt7461Config - configure the ADT7461
*
* This routine will configure the ADT7461 based on user input.
*
* RETURNS: OK if successful, ERROR otherwise.
*/

STATUS sysAdt7461Config
    (
    UINT32 conversion,          /* conversion mode */
    UINT32 mode,                /* mode, thermostat vs thermometer */
    UINT8 adt7461TempLow,       /* temp low config data */
    UINT8 adt7461TempHigh,      /* temp high config data */
    UINT8 adt7461CpuTempLow,    /* CPU temp low config data */
    UINT8 adt7461CpuTempHigh    /* CPU temp high config data */
    )
    {
    UINT8 configReg = 0;

    if ((conversion != ADT7461_CONV_MODE_CONT) &&
        (conversion != ADT7461_CONV_MODE_1SHOT))
        return (ERROR);

    if ((mode != ADT7461_MODE_THERMOSTAT) &&
        (mode != ADT7461_MODE_THERMOMETER))
        return (ERROR);

    if (isAdt7461Running)
        sysAdt7461Disable();

    /* call the ADT7461 initialization routine */

    sysAdt7461Init();

    /* conversion mode */

    sysAdt7461ConfigRegRead(&configReg);
    configReg &= ~(ADT7461_1SHOT_MASK);
    if (conversion)
        configReg |= ADT7461_1SHOT_MASK;
    sysAdt7461ConfigRegWrite(configReg);

    /* mode, thermostat vs. thermometer */

    sysAdt7461Mode = mode;

    if (sysAdt7461Mode == ADT7461_MODE_THERMOSTAT)
        {

        /* temperature high */
        sysAdt7461THRegWrite(adt7461TempHigh);

        /* temperature low */
        sysAdt7461TLRegWrite(adt7461TempLow);

        /* CPU temperature high */
        sysAdt7461CpuTHRegWrite(adt7461CpuTempHigh);

        /* CPU temperature low */
        sysAdt7461CpuTLRegWrite(adt7461CpuTempLow);
        }
    else
        {
        /* temperature high */
        sysAdt7461THRegWrite(127);

        /* temperature low */
        sysAdt7461TLRegWrite(0);

        /* CPU temperature high */
        sysAdt7461CpuTHRegWrite(127);

        /* CPU temperature low */
        sysAdt7461CpuTLRegWrite(0);
        }

    if (!isAdt7461Running)
        sysAdt7461Enable();

    return (OK);
    }

/*******************************************************************************
*
* sysAdt7461Enable - "start" the ADT7461
*
* This routine will "start" the ADT7461 by enabling interrupts and starting
* the temperature conversion.
*
* RETURNS: OK if started alright, ERROR if already "running".
*/

STATUS sysAdt7461Enable (void)
    {
    UINT8 configReg = 0;

    if (!isAdt7461Running)
        {

        if (sysAdt7461ConfigRegRead(&configReg) == ERROR)
            {
            return(ERROR);
            }

        if (configReg & ADT7461_1SHOT_MASK)
            {
            if (sysAdt7461ConversionStart() == ERROR)
                return (ERROR);
            }

        isAdt7461Running = TRUE;

        if (sysAdt7461Mode == ADT7461_MODE_THERMOSTAT)
#ifdef INCLUDE_AMP
            if (!CPU1CHECK)
                {
#endif /* INCLUDE_AMP */
                sysAdt7461IntEnable();  /* enable interrupts */
#ifdef INCLUDE_AMP
                }
#endif /* INCLUDE_AMP */
        }
    return (OK);    /* already running */
    }

/*******************************************************************************
*
* sysAdt7461Disable - "stop" the ADT7461
*
* This routine will "stop" the ADT7461 by disabling interrupts and stopping
* the temperature conversion.
*
* RETURNS: OK if stopped alright, ERROR if not already "running".
*/

STATUS sysAdt7461Disable (void)
    {

    if (isAdt7461Running)
        {
        if (sysAdt7461Mode == ADT7461_MODE_THERMOSTAT)
            sysAdt7461IntDisable();

        isAdt7461Running = FALSE;
        }
    return (OK);
    }

#ifdef ADT7461_DEBUG
/*******************************************************************************
*
* sysAdt7461Test - test routine for ADT7461 library
*
* This is a test routine for the ADT7461 library.
*
* RETURNS: OK if successful, ERROR otherwise
*/

STATUS sysAdt7461Test (void)
    {
    UINT8 adt7461TempLow;
    UINT8 adt7461TempHigh;
    UINT8 adt7461CpuTempLow;
    UINT8 adt7461CpuTempHigh;
    UINT8 configReg;
    UINT32 counter = 0;

    printf("ADT7461 Test: Starting Test...\n\r");

    /* connect "test" interrupt handler */

    sysAdt7461Connect((FUNCPTR)sysAdt7461UsrInt,0x0);

    /* test the conversion modes */

    adt7461TempLow = 0;
    adt7461TempHigh = 127;
    adt7461CpuTempLow = 0;
    adt7461CpuTempHigh = 127;
    if (sysAdt7461Config(ADT7461_CONV_MODE_CONT, ADT7461_MODE_THERMOSTAT,
                         adt7461TempLow, adt7461TempHigh,
                         adt7461CpuTempLow, adt7461CpuTempHigh) == ERROR)
        return (ERROR);
    if (sysAdt7461Enable() == ERROR)
        return (ERROR);
    if (sysAdt7461ConfigRegRead(&configReg) == ERROR)
        return (ERROR);
    if (configReg & ADT7461_1SHOT_MASK)
        return (ERROR);
    else
        printf("ADT7461 Test: Continuous mode PASSED\n\r");

#ifdef INCLUDE_SHOW_ROUTINES
    if (sysAdt7461Show() == ERROR)
        {
        printf("Show routine returned ERROR\n\r");
        return (ERROR);
        }
#endif /* INCLUDE_SHOW_ROUTINES */

    sysAdt7461Disable();

    if (sysAdt7461Config(ADT7461_CONV_MODE_1SHOT, ADT7461_MODE_THERMOSTAT,
                         adt7461TempLow, adt7461TempHigh,
                         adt7461CpuTempLow, adt7461CpuTempHigh) == ERROR)
        {
        printf("Config returned ERROR\n\r");
        return (ERROR);
        }
    if (sysAdt7461Enable() == ERROR)
        {
            printf("Enable returned ERROR\n\r");
            return (ERROR);
        }
    if (sysAdt7461ConfigRegRead(&configReg) == ERROR)
        return (ERROR);
    if (configReg & ADT7461_1SHOT_MASK)
        printf("ADT7461 Test: 1-Shot mode PASSED\n\r");
    else
        {
        printf("1-Shot bit is not set\n\r");
        return (ERROR);
        }

#ifdef INCLUDE_SHOW_ROUTINES
    if (sysAdt7461Show() == ERROR)
        {
        printf("Show routine returned ERROR\n\r");
        return (ERROR);
        }
#endif /* INCLUDE_SHOW_ROUTINES */

    sysAdt7461Disable();

    /*
     * Test our ability to reconfigure the ADT7461 even while it is
     * running and test the interrupts.
     */

    if (sysAdt7461Config(ADT7461_CONV_MODE_CONT, ADT7461_MODE_THERMOMETER,
                         adt7461TempLow, adt7461TempHigh,
                         adt7461CpuTempLow, adt7461CpuTempHigh) == ERROR)
        {
        printf("Config returned ERROR\n\r");
        return (ERROR);
        }
    if (sysAdt7461Enable() == ERROR)
        {
        printf("Enable returned ERROR\n\r");
        return (ERROR);
        }
#ifdef INCLUDE_SHOW_ROUTINES
    if (sysAdt7461Show() == ERROR)
        {
        printf("Show routine returned ERROR\n\r");
        return (ERROR);
        }
#endif

    adt7461TempLow = 0;
    adt7461TempHigh = 25;
    adt7461CpuTempLow = 0;
    adt7461CpuTempHigh = 25;
    counter = intCounter;
    printf("Counter = %d\n\r", counter);
    if (sysAdt7461Config(ADT7461_CONV_MODE_CONT, ADT7461_MODE_THERMOSTAT,
                         adt7461TempLow, adt7461TempHigh,
                         adt7461CpuTempLow, adt7461CpuTempHigh) == ERROR)
        {
        printf("Config returned ERROR\n\r");
        return (ERROR);
        }
    taskDelay(sysClkRateGet()); /* Let monitor task run */
    printf("intCounter = %d\n\r", intCounter);

    if (intCounter > counter)
        {
        printf("ADT7461 Test: Ability to reconfigure while running PASSED\n\r");
        printf("ADT7461 Test: Interrupts PASSED\n\r");
        }
    else
        {
        printf("Counter did not increment\n\r");
        return (ERROR);
        }

    /* Test show routines */

    printf("Visually Verify test routine output!\n\r");
#ifdef INCLUDE_SHOW_ROUTINES
    if (sysAdt7461Show() == ERROR)
        {
        printf("Show routine returned ERROR\n\r");
        return (ERROR);
        }
#endif

    printf("ADT7461 Test: Test COMPLETE\n\r");
    return (OK);
    }
#endif /* ADT7461_DEBUG */
