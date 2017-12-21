/* sysGpio.c - P2020DS GPIO driver source file */

/*
 * Copyright (c) 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 *
 */

/*
modification history
--------------------
01a,31may09,syt  written
*/

/*
DESCRIPTION
This is the driver for the Freescale P2020 GPIO controller.  This device
has a total of 16 pins, each of which can be programmed as either an
input or output.  When programmed as an input a pin can be used as an
interrupt source.  Each pin can configed as detect interrupts as active
falling edge or level change.


SEE ALSO:

\tb "QorIQ P2020 Integrated Processor Reference Manual"
*/

/* includes */

#include <vxWorks.h>
#include <stdio.h>
#include <intLib.h>
#include <errnoLib.h>
#include <errno.h>

/* defines */

#define GPIO_0                          0
#define GPIO_1                          1
#define GPIO_2                          2
#define GPIO_3                          3
#define GPIO_4                          4
#define GPIO_5                          5
#define GPIO_6                          6
#define GPIO_7                          7
#define GPIO_8                          8
#define GPIO_9                          9
#define GPIO_10                         10
#define GPIO_11                         11
#define GPIO_12                         12
#define GPIO_13                         13
#define GPIO_14                         14
#define GPIO_15                         15

#define GPIO_PIN_MAX                    15  /* 16 pins (incl. 0) */

#define GPIO_LOW                        0x00
#define GPIO_HIGH                       0x01

#define GPIO_REG_WRITE(regPtr,val)      (*((VUINT32 *)(regPtr)) = (val))
#define GPIO_REG_READ(regPtr,res)       ((res) = *(VUINT32 *)(regPtr))

#define GPIO_CODE_REENTRANT

#ifdef GPIO_CODE_REENTRANT
#   define GPIO_INTLOCK(key)
#   define GPIO_INTUNLOCK(key)
#   define GPIO_KEYDECLARE
#else  /* GPIO_CODE_REENTRANT */
#   define GPIO_INTLOCK(key)            (key = intLock())
#   define GPIO_INTUNLOCK(key)          (intUnlock(key))
#   define GPIO_KEYDECLARE              int intKey
#endif  /* GPIO_CODE_REENTRANT */

/* GPIO pin types */

#define GPIO_OUT                        0x00008000
#define GIPO_OD                         0x00000001
#define GPIO_IN                         0x00000000
#define GPIO_INT                        0x80000000

/* Interrupt styles, these refer to actual values used in reg */

#define GPIO_INT_FAIL_TRIG              0x00010000
#define GPIO_INT_LEVEL_TRIG             0x00020000

/*******************************************************************************
*
* sysGpioLineConfig - configure a GPIO pin
*
* This routine configures a GPIO pin/line number specified in the <lineNo>
* parameter for use as either an input or output, as specified in the
* <style> parameter.
*
* Example:
*   1) Config GPIO_0 as an open drain output Pin, it should call
*
*      sysGpioLineConfig (GPIO_0, (GPIO_OUT | GIPO_OD));
*
*   2) Config GPIO_1 as an external interrupt input Pin, level change
*
*      sysGpioLineConfig (GPIO_1, (GPIO_IN | GPIO_INT | GPIO_INT_LEVEL_TRIG));
*
* RETURNS:
* OK when line is successfully configured, else ERROR if the line number
* is out of range, or an unknown style is requested.
*
* ERRNO
*/

STATUS sysGpioLineConfig
    (
    UINT8  lineNo,
    UINT32 style
    )
    {
    VUINT32 gpioDirReg;
    VUINT32 intMaskReg;
    VUINT32 intCtrlReg;
    VUINT32 gpioOpenDrainReg;

    GPIO_KEYDECLARE;

    if (lineNo > GPIO_PIN_MAX)
        return(ERROR);

    GPIO_REG_READ(GPDIR, gpioDirReg);
    GPIO_REG_READ(GPODR, gpioOpenDrainReg);
    GPIO_REG_READ(GPICR, intCtrlReg);
    GPIO_REG_READ(GPIMR, intMaskReg);

    if (style & GPIO_OUT)
        {
        gpioDirReg |= (0x80000000 >> lineNo);

        if (style & GIPO_OD)
            gpioOpenDrainReg |= (0x80000000 >> lineNo);
        else
            gpioOpenDrainReg &= ~(0x80000000 >> lineNo);
        }
    else
        {
        gpioDirReg &= ~(0x80000000 >> lineNo);

        if (style & GPIO_INT)
            {
            if (style & GPIO_INT_FAIL_TRIG)
                intCtrlReg |= (0x80000000 >> lineNo);
            else if (style & GPIO_INT_LEVEL_TRIG)
                intCtrlReg &= ~(0x80000000 >> lineNo);
            else
                return(ERROR);

            intMaskReg &= ~(0x80000000 >> lineNo);
            }
        }

    GPIO_INTLOCK (GPIO_KEYDECLARE);

    /* Set the new style */

    GPIO_REG_WRITE(GPDIR, gpioDirReg);
    GPIO_REG_WRITE(GPODR, gpioOpenDrainReg);
    GPIO_REG_WRITE(GPICR, intCtrlReg);
    GPIO_REG_WRITE(GPIMR, intMaskReg);

    GPIO_INTUNLOCK (GPIO_KEYDECLARE);

    return(OK);
    }

/*******************************************************************************
*
* sysGpioLineGet - get the value of a GPIO pin
*
* This routine returns the value of a specified GPIO <lineNo> pin to the
* location specified in the <value> parameter.
*
* RETURNS:
* OK when a pin value is successfully retrieved, else ERROR if the
* specified pin number is out of range.
*
* ERRNO
*/

STATUS sysGpioLineGet
    (
    UINT32 * gpioData,
    UINT8    lineNo
    )
    {

    if (lineNo > GPIO_PIN_MAX)
        return (ERROR);

    GPIO_REG_READ(GPDAT, *gpioData);

    *gpioData = (*gpioData >> (31 - lineNo)) & 0x01;

    return (OK);
    }

/*******************************************************************************
*
* sysGpioLineSet - set the value of a GPIO pin configured for output
*
* This routine sets the <value> of a specified GPIO <lineNo> pin number.
*
* RETURNS:
* OK when pin is successfully set, else ERROR if the specified pin number
* is out of range.
*
* ERRNO
*/

STATUS sysGpioLineSet
    (
    UINT8   lineNo,
    UINT8   value
    )
    {
    UINT32  gpioData;

    if (lineNo > GPIO_PIN_MAX)
        return (ERROR);

    GPIO_REG_READ(GPDAT, gpioData);

    if (value == GPIO_HIGH)
       {
       gpioData |= (0x80000000 >> lineNo);
       }
    else if (value == GPIO_LOW)
       {
       gpioData &= ~(0x80000000 >> lineNo);
       }

    GPIO_REG_WRITE(GPDAT, gpioData);

    return (OK);
    }

/*******************************************************************************
*
* sysGpioIntStatusGet - get a GPIO (interrupt) pin status
*
* Given GPIO pin number <lineNo>, this routine returns its status to the
* location specified in the <status> parameter.
*
* RETURNS:
* OK when status is successfully retrieved, else ERROR if the specified
* pin number is out of range.
*
* ERRNO
*/

STATUS sysGpioIntStatusGet
    (
    UINT8  lineNo,
    BOOL * status
    )
    {
    volatile UINT32 intStatusReg;

    if (lineNo > GPIO_PIN_MAX)
        return (ERROR);

    if (status == NULL)
        return (ERROR);

    GPIO_REG_READ(GPIER, intStatusReg);

    *status = ((intStatusReg & (0x80000000 >> lineNo)) != 0);

    return (OK);
    }

/*******************************************************************************
*
* sysGpioIntStatusClear - clear a GPIO (interrupt) pin status
*
* This routine returns the GPIO pin status.
*
* RETURNS:
* OK if status cleared, else ERROR if the pin number passed is out of range.
*
* ERRNO
*/

STATUS sysGpioIntStatusClear
    (
    UINT8 lineNo
    )
    {
    UINT32 intStatusClr;

    if (lineNo > GPIO_PIN_MAX)
        return(ERROR);

    intStatusClr = (0x80000000 >> lineNo);

    /* Write 1 to clear */

    GPIO_REG_WRITE(GPIER, intStatusClr);

    return(OK);
    }

/*******************************************************************************
*
* sysGpioReset - reset the GPIO controller
*
* This routine resets the pins of the GPIO controller to known default settings.
* The settings (defined in p2020Gpio.h) modify how the pins are programmed
* and what value they are set at. It is intended that this function be called
* on a warm reset of the P2020 device.
*
* RETURNS: N/A
*
* ERRNO
*/

void sysGpioReset (void)
    {
    GPIO_KEYDECLARE;

    GPIO_INTLOCK (GPIO_KEYDECLARE);

    GPIO_REG_WRITE(GPDIR, 0x00);
    GPIO_REG_WRITE(GPODR, 0x00);
    GPIO_REG_WRITE(GPDAT, 0x00);
    GPIO_REG_WRITE(GPIMR, 0x00);
    GPIO_REG_WRITE(GPIER, 0xffff0000);
    GPIO_REG_WRITE(GPICR, 0x00);

    GPIO_INTUNLOCK (GPIO_KEYDECLARE);

    return;
    }

/*******************************************************************************
*
* sysGpioDirGet - get the direction of a GPIO pin number
*
* This routine returns the direction, 0x00 or 0x00, for
* a specified GPIO <lineNo> to the location specified in the <gpioDir>
* parameter.
*
* RETURNS:
* OK when the direction is returned, else ERROR if the specified pin
* number is out of range.
*
* ERRNO
*/

STATUS sysGpioDirGet
    (
    UINT8   lineNo,
    UINT8 * gpioDir
    )
    {
    VUINT32 gpioDirReg;
    UINT32  shifter;

    if (lineNo > GPIO_PIN_MAX)
        {
        return (ERROR);
        }

    GPIO_REG_READ(GPDIR, gpioDirReg);

    shifter = (0x80000000 >> lineNo);

    if (shifter & gpioDirReg)
        {*gpioDir = 0x01;}
    else
        {*gpioDir = 0x00;}

    return (OK);
    }

