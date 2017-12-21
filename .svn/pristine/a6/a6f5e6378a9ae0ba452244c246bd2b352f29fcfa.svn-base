/* sysI2cBootSet.c - ROM initialization I2C module of boot */

/*
 * Copyright (c) 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,12oct11,e_d remove redundance code to decrease code size. (WIND00306790)
01a,27jun11,e_d initial creation
*/

/*
DESCRIPTION

This file provides I2c initialization routine for MPC8306 and MPC8309.
The MPC830x board select the reset configuration words source by KA2 micro
ctroller. The KA2 first set the source is Nor Flash and read the PCA9536
(I2C address is 0x41) GPIO pin2. If this pin is not set high, the KA2 will
reset CPU and load reset configuration word from next source.

This routine will be called by romInit and set the GPIO pin2 is outpin and high.

SEE ALSO:
\tb MPC8309KIT User's Guide
*/

/* includes */

#include <vxWorks.h>
#include "config.h"
#include "sysMpc8313I2c.h"

IMPORT void romInit();
IMPORT void romStart();
IMPORT void copyBufferToRam(FAST int startType);

#define I2C_REG_WRITE(addr,value)   \
          *(volatile UINT8 *)(addr) = (UINT8)(value)

#define I2C_REG_READ(addr)          \
         *(volatile UINT8 *)(addr)

/* This macro is define the PCA9536 I2C offset address */

#define PCA9536_I2CADRS       0x82
#define PCA9536_DIRECTION_REG 0x03
#define PCA9536_OUTPIN_REG    0x01

/*******************************************************************************
*
* sysBootDelay - delay function before sysHwInit
*
* This routine is delay function before sysHwInit.
*
* RETURNS: N/A
*
*/
void sysBootDelay(void)
    {
    volatile UINT32 a,b;
    int i = 0;

    for(i = 0; i < 0x1000; i++)
        {
        a++;
        b++;
        a = b;
        }
    }

/*******************************************************************************
*
* sysI2cBootSet - set PCA9356 GPIO pin2
*
* This routine set PCA9356 GPIO pin2 to outpin and high level.
*
* RETURNS: N/A
*
*/
void sysI2cBootSet
    (
    FAST int startType
    )
    {
    volatile UINT8 regTemp = 0;
    int i = 0;
    FUNCPTR run_in_ram;
    unsigned long code_ram_adrs,offset;

#ifndef BOOT_FROM_NAND
    offset=((unsigned long )romStart -(unsigned long )romInit);
    code_ram_adrs = ROM_BASE_ADRS + 0x100 + offset;
#else /* BOOT_FROM_NAND */
    offset=((unsigned long )copyBufferToRam -(unsigned long )romInit);
    code_ram_adrs = NAND_BUFFER_BASE + 0x100 + offset;
#endif /* BOOT_FROM_NAND */

    run_in_ram = (FUNCPTR)code_ram_adrs;

    /* I2C init process */

    I2C_REG_WRITE(CCSBAR + I2C1ICR, 0x20);
    sysBootDelay();

    I2C_REG_WRITE(CCSBAR + I2C1FDR, 0x33);
    sysBootDelay();

    I2C_REG_WRITE(CCSBAR + I2C1DFSRR, 0x10);
    sysBootDelay();

    I2C_REG_WRITE(CCSBAR + I2C1ADR, 0xfe);
    sysBootDelay();

    I2C_REG_WRITE(CCSBAR + I2C1ISR, (regTemp & (~MPC8313_I2C_STATUS_REG_MIF)));
    sysBootDelay();

    I2C_REG_WRITE(CCSBAR + I2C1ICR, 0xa0);
    sysBootDelay();

    I2C_REG_WRITE(CCSBAR + I2C1ICR, 0x80);
    sysBootDelay();

    /* I2C write 0x00 to slave address 0x41 register address is 0x3 */

    for(i = 0; i < 10; i++)
        {
        regTemp = I2C_REG_READ(CCSBAR + I2C1ISR);

        if((regTemp & MPC8313_I2C_STATUS_REG_MBB) == 0x0)
            break;
        }

    I2C_REG_WRITE(CCSBAR + I2C1ICR, MPC8313_I2C_CONTROL_REG_MSTA |
                                    MPC8313_I2C_CONTROL_REG_MTX |
                                    MPC8313_I2C_CONTROL_REG_MEN);
    sysBootDelay();

    I2C_REG_WRITE(CCSBAR + I2C1IDR, PCA9536_I2CADRS);
    sysBootDelay();

    for(i = 0; i < 1000; i++)
        {
        regTemp = I2C_REG_READ(CCSBAR + I2C1ISR);
        if((regTemp & MPC8313_I2C_STATUS_REG_MIF) != 0x0)
            {
            I2C_REG_WRITE(CCSBAR + I2C1ISR, (regTemp &
                                             (~MPC8313_I2C_STATUS_REG_MIF)));
            sysBootDelay();
            break;
            }
        }

    I2C_REG_WRITE(CCSBAR + I2C1IDR, PCA9536_DIRECTION_REG);
    sysBootDelay();

    for(i = 0; i < 1000; i++)
        {
        regTemp = I2C_REG_READ(CCSBAR + I2C1ISR);
        WRS_ASM("sync");
        if((regTemp & MPC8313_I2C_STATUS_REG_MIF) != 0x0)
            {
            I2C_REG_WRITE(CCSBAR + I2C1ISR, (regTemp &
                                             (~MPC8313_I2C_STATUS_REG_MIF)));
            sysBootDelay();
            break;
            }
        }

    I2C_REG_WRITE(CCSBAR + I2C1IDR, 0x00);
    sysBootDelay();

    for(i = 0; i < 1000; i++)
        {
        regTemp = I2C_REG_READ(CCSBAR + I2C1ISR);
        WRS_ASM("sync");
        if((regTemp & MPC8313_I2C_STATUS_REG_MIF) != 0x0)
            {
            I2C_REG_WRITE(CCSBAR + I2C1ISR, (regTemp &
                                             (~MPC8313_I2C_STATUS_REG_MIF)));
            sysBootDelay();
            break;
            }
        }

    I2C_REG_WRITE(CCSBAR + I2C1ICR, 0x80);
    sysBootDelay();

    for(i = 0; i < 100; i++)
        {
        regTemp = I2C_REG_READ(CCSBAR + I2C1ISR);
        if((regTemp & MPC8313_I2C_STATUS_REG_MBB) == 0x0)
            break;
        }

    (*run_in_ram) (startType);
}

