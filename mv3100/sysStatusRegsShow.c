/* sysStatusRegsShow.c - "Show" routines for board level status registers */

/* Copyright 2005 Motorola, Inc. All Rights Reserved */

/*
modification history
--------------------
01a,10aug10,efb Initial writing.
*/

/*
DESCRIPTION
This file contains the show routines for the board system status registers,
and memory device SPD. These routines are included only when 
INCLUDE_SHOW_ROUTINES is defined in config.h.

*/

/* includes */

#include "vxWorks.h"
#include "logLib.h"
#include "mpc8540Smc.h"
#include "sdramSpd.h"

/* defines */

/* typedefs */

/* globals */

/* locals */

/* forward declarations */

/* externals */

IMPORT STATUS sysMpc8540GetSpdData (UINT32, UINT32, UINT32, UCHAR *);
IMPORT UINT32 sysPciBusASpdGet (void);
IMPORT UINT32 sysPciBusBSpdGet (void);
IMPORT UINT32 sysPciBusCSpdGet (void);

/******************************************************************************
*
* sysStatusRegShow - Show the system status register.
*
* This function displays to the console the system status register and 
* interprets the bit values.
*
* RETURNS: NA
*/

void sysStatusRegShow (void)
    {
    UINT8 regVal = *(UINT8 *)BRD_SYSTEM_STATUS_REG;

    printf ("System Status Register @ 0x%x = 0x%x\r\n", 
    (UINT8 *)BRD_SYSTEM_STATUS_REG, regVal);

    printf ("    [1:0] Board Type = ");
    if ((regVal & BRD_SYSTEM_STATUS_BRD_TYPE_MASK) ==
        BRD_SYSTEM_STATUS_VME_BRD_TYPE) 
        {
        printf ("VME SBC\r\n");
        }
    else
        {
        printf ("PrPMC\r\n");
        }

    if ((regVal & BRD_SYSTEM_STATUS_SAFE_START_MASK) == 0)
        {
        printf ("    [4]   Firmware uses programmed NVRAM settings.\r\n");
        }
    else
        {
        printf ("    [4]   Firmware uses safe ENV settings.\r\n");
        }
        
    printf ("\r\n");
    }

/******************************************************************************
*
* sysStatusIndicatorRegShow - Show the system status indicator register.
*
* This function displays to the console the system status indicator register 
* and interprets the bit values.
*
* RETURNS: NA
*/

void sysStatusIndicatorRegShow (void)
    {
    UINT8 regVal = *(UINT8 *)BRD_SYSTEM_STATUS_INDICATOR_REG;

    printf ("System Status Indicator Register @ 0x%x = 0x%x\r\n", 
            (UINT8 *)BRD_SYSTEM_STATUS_INDICATOR_REG, regVal);

    printf ("    [0] Board Fail LED is ");

    if ((regVal & BRD_SYSTEM_STATUS_INDICATOR_BRD_FAIL_MASK) == 0) 
        {
        printf ("OFF.\r\n");
        }
    else
        {
        printf ("ON.\r\n");
        }

    printf ("    [1] User 1 LED is ");

    if ((regVal & BRD_SYSTEM_STATUS_INDICATOR_USR1_LED_MASK) == 0) 
        {
        printf ("OFF.\r\n");
        }
    else
        {
        printf ("ON.\r\n");
        }

    printf ("    [2] User 2 LED is ");

    if ((regVal & BRD_SYSTEM_STATUS_INDICATOR_USR2_LED_MASK) == 0) 
        {
        printf ("OFF.\r\n");
        }
    else
        {
        printf ("ON.\r\n");
        }

    printf ("    [3] User 3 LED is ");

    if ((regVal & BRD_SYSTEM_STATUS_INDICATOR_USR3_LED_MASK) == 0) 
        {
        printf ("OFF.\r\n");
        }
    else
        {
        printf ("ON.\r\n");
        }
        
    printf ("\r\n");
    }

/******************************************************************************
*
* sysFlashStatusRegShow - Show the system FLASH status register.
*
* This function displays to the console the system FLASH status register 
* and interprets the bit values.
*
* RETURNS: NA
*/

void sysFlashStatusRegShow (void)
    {
    UINT8 regVal = *(UINT8 *)BRD_FLASH_CONTROL_REG;

    printf ("System Flash Status Register @ 0x%x = 0x%x\r\n", 
            (UINT8 *)BRD_FLASH_CONTROL_REG, regVal);

    printf ("    [0] FLASH is ");
    if ((regVal & BRD_FLASH_CONTROL_FLASH_RDY_MASK) != 0) 
        {
        printf ("ready.\r\n");
        }
    else
        {
        printf ("not ready.\r\n");
        }

    printf ("    [1] FLASH boot block is ");
    if ((regVal & BRD_FLASH_CONTROL_BOOT_BLOCK_SEL_MASK) == 0) 
        {
        printf ("A.\r\n");
        }
    else
        {
        printf ("B.\r\n");
        }

    printf ("    [2] FLASH hardware write protect is ");
    if ((regVal & BRD_FLASH_CONTROL_HARDWARE_WP_MASK) != 0) 
        {
        printf ("enabled.\r\n");
        }
    else
        {
        printf ("not enabled.\r\n");
        }

    printf ("    [3] FLASH software write protect is ");
    if ((regVal & BRD_FLASH_CONTROL_SOFTWARE_WP_MASK) != 0) 
        {
        printf ("enabled.\r\n");
        }
    else
        {
        printf ("not enabled.\r\n");
        }

    printf ("    [4] FLASH memory map is ");
    if ((regVal & BRD_FLASH_CONTROL_MAP_SEL_MASK) == 0) 
        {
        printf ("controlled by the boot block select switch.\r\n");
        }
    else
        {
        printf ("fixed to boot block A only.\r\n");
        }

    printf ("\r\n");
    }

/******************************************************************************
*
* sysPciBusStatusRegShowA - Show the system status register for PCI bus A
*
* This function displays to the console the system status register for
* PCI bus A and interprets the bit values.
*
* RETURNS: NA
*/

void sysPciBusStatusRegShowA (void)
    {
    UINT8 regVal = *(UINT8 *)BRD_PCI_BUS_A_STAT_REG;
    UINT32 speed = sysPciBusASpdGet (); 

    printf ("PCI Bus A Status Register @ 0x%x = 0x%x\r\n", 
            (UINT8 *)BRD_PCI_BUS_A_STAT_REG, regVal);

    printf ("    [1:0] PCI A bus speed is %d Hz.\r\n", speed);

    printf ("    [2]   PCI A bus mode is ");
    if ((regVal & BRD_PCI_BUS_A_STAT_PCIX_MASK) != 0) 
        {
        printf ("PCI-X.\r\n");
        }
    else
        {
        printf ("PCI.\r\n");
        }

    printf ("    [3]   PCI A bus width is ");
    if ((regVal & BRD_PCI_BUS_A_STAT_64B_MASK) != 0) 
        {
        printf ("64 bits.\r\n");
        }
    else
        {
        printf ("32 bits.\r\n");
        }
    printf("\r\n");
    }

/******************************************************************************
*
* sysPciBusStatusRegShowB - Show the system status register for PCI bus B
*
* This function displays to the console the system status register for
* PCI bus B and interprets the bit values.
*
* RETURNS: NA
*/

void sysPciBusStatusRegShowB (void)
    {
    UINT8 regVal = *(UINT8 *)BRD_PCI_BUS_B_STAT_REG;
    UINT32 speed = sysPciBusBSpdGet (); 

    printf ("PCI Bus B Status Register @ 0x%x = 0x%x\r\n", 
            (UINT8 *)BRD_PCI_BUS_B_STAT_REG, regVal);

    printf ("    [1:0] PCI B bus speed is %d Hz.\r\n", speed);

    printf ("    [2]   PCI B bus mode is ");
    if ((regVal & BRD_PCI_BUS_B_STAT_PCIX_MASK) != 0) 
        {
        printf ("PCI-X.\r\n");
        }
    else
        {
        printf ("PCI.\r\n");
        }

    printf ("    [3]   PCI B bus width is ");
    if ((regVal & BRD_PCI_BUS_B_STAT_64B_MASK) != 0) 
        {
        printf ("64 bits.\r\n");
        }
    else
        {
        printf ("32 bits.\r\n");
        }

    printf ("    [4]   PMC site 1 device is " );
    if ((regVal & BRD_PCI_BUS_B_STAT_ERDY1_MASK) != 0) 
        {
        printf ("ready for enumeration.\r\n");
        }
    else
        {
        printf ("not ready.\r\n");
        }

    printf ("    [5]   PMC site 2 device is " );
    if ((regVal & BRD_PCI_BUS_B_STAT_ERDY2_MASK) != 0) 
        {
        printf ("ready for enumeration.\r\n");
        }
    else
        {
        printf ("not ready.\r\n");
        }

    if ((regVal & BRD_PCI_BUS_B_STAT_5VIO_MASK) != 0) 
        {
        printf ("    [6]   PMC bus is configured for " );
        printf ("5.0V VIO.\r\n");
        }

    if ((regVal & BRD_PCI_BUS_B_STAT_3VIO_MASK) != 0) 
        {
        printf ("    [7]   PMC bus is configured for " );
        printf ("3.3V VIO.\r\n");
        }

    printf("\r\n");
    }

/******************************************************************************
*
* sysPciBusStatusRegShowC - Show the system status register for PCI bus C
*
* This function displays to the console the system status register for
* PCI bus C and interprets the bit values.
*
* RETURNS: NA
*/

void sysPciBusStatusRegShowC (void)
    {
    UINT8 regVal = *(UINT8 *)BRD_PCI_BUS_C_STAT_REG;
    UINT32 speed = sysPciBusCSpdGet (); 

    printf ("PCI Bus C Status Register @ 0x%x = 0x%x\r\n", 
            (UINT8 *)BRD_PCI_BUS_C_STAT_REG, regVal);

    printf ("    [1:0] PCI C bus speed is %d Hz.\r\n", speed);

    printf ("    [2]   PCI C bus mode is ");
    if ((regVal & BRD_PCI_BUS_C_STAT_PCIX_MASK) != 0) 
        {
        printf ("PCI-X.\r\n");
        }
    else
        {
        printf ("PCI.\r\n");
        }

    printf ("    [3]   PCI C bus width is ");
    if ((regVal & BRD_PCI_BUS_C_STAT_64B_MASK) != 0) 
        {
        printf ("64 bits.\r\n");
        }
    else
        {
        printf ("32 bits.\r\n");
        }

    printf("\r\n");
    }

/******************************************************************************
*
* sysPresenceDetectRegShow - Show the system presence detect register
*
* This function displays to the console the system presence detect 
* register and interprets the bit values.
*
* RETURNS: NA
*/

void sysPresenceDetectRegShow (void)
    {
    UINT8 regVal = *(UINT8 *)BRD_PRESENCE_DETECT_REG;

    printf ("Presence Detect Register @ 0x%x = 0x%x\r\n", 
            (UINT8 *)BRD_PRESENCE_DETECT_REG, regVal);

    if (regVal == 0)
        {
        printf ("    No PMC devices present.\r\n");
        }

    if ((regVal & BRD_PRESENCE_DETECT_PMC1_MASK) != 0) 
        {
        printf ("    PMC device present at site 1.\r\n");
        }

    if ((regVal & BRD_PRESENCE_DETECT_PMC2_MASK) != 0) 
        {
        printf ("    PMC device present at site 2.\r\n");
        }

    if ((regVal & BRD_PRESENCE_DETECT_PMC_SPAN_MASK) != 0) 
        {
        printf ("    PMC span is installed.\r\n");
        }

    printf("\r\n");
    }

/******************************************************************************
*
* sysPldInfoShow - Show the system PLD revision and date code
*
* This function displays to the console the system  PLD revision and date
* code information.
*
* RETURNS: NA
*/

void sysPldInfoShow (void)
    {
    printf ("PLD Revision Register @ 0x%x = 0x%x\r\n", 
            (UINT8 *)BRD_PLD_REVISION_REG, 
            *(UINT8 *)BRD_PLD_REVISION_REG);

    printf ("PLD Date Code Register @ 0x%x = 0x%x\r\n", 
            (UINT8 *)BRD_PLD_DATE_CODE_REG, 
            *(UINT8 *)BRD_PLD_DATE_CODE_REG);

    printf("\r\n");
    }

/******************************************************************************
*
* sysStatusRegShowAll - Show all of the board system status registers  
*
* This function displays to the console all of the board system status 
* registers and interprets the bit values.
*
* RETURNS: NA
*/

void sysStatusRegShowAll (void)
    {
    sysStatusRegShow ();
    sysStatusIndicatorRegShow ();
    sysFlashStatusRegShow ();
    sysPciBusStatusRegShowA ();
    sysPciBusStatusRegShowB ();
    sysPciBusStatusRegShowC ();
    sysPresenceDetectRegShow ();
    sysPldInfoShow ();
    }

/******************************************************************************
*
* sysMpc8540SpdShow - Display the SPD values.
*
* This function reads & displays the SPD values for the memory devices on
* the board. There is only one SPD memory device at SPD_EEPROM_I2C_ADDR.
*
* RETURNS: OK if SPD is read successfully, ERROR otherwise.
*/

STATUS sysMpc8540SpdShow ()
    {
    UINT32 spdByte;                        /* SPD byte index ctr */
    UCHAR  spdData[SPD_SIZE];              /* SPD data */
    STATUS status = OK;

    status = sysMpc8540GetSpdData (SPD_EEPROM_I2C_ADDR, 0, SPD_SIZE, spdData);
  
    if (status == OK)
        {
        printf("I2C Address: %X\n\r", SPD_EEPROM_I2C_ADDR);
        printf("---------------\r\n");
        for (spdByte = 0; spdByte < SPD_SIZE; spdByte++)
            {
            printf("Byte %d = 0x%02X\n\r", spdByte, spdData[spdByte]);
            }
         }
    else
         {
         printf("No SPD device detected at %X\n\r", SPD_EEPROM_I2C_ADDR);
         }

    return (status);
    }
