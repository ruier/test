/* 40vxbSpiEeprom.cdf - SPI interface EEPROM component configuration file */

/*
 * Copyright (c) 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,03oct12,y_y written
*/

Component   DRV_SPI_EEPROM {
    NAME        SPI EEPROM vxBus driver
    SYNOPSIS    spi EEPROM vxBus driver
                This enables access to MicroChip 25XX series EEPROM chips.
                Other chips which similar in procedures with one item in 
                current list are supported as well. More info see the 
                driver source for the current list.
    MODULES     vxbSpiEeprom.o
    _CHILDREN   FOLDER_DRIVERS
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_RTN    vxbSpiEepromRegister ();
    PROTOTYPE   void vxbSpiEepromRegister(void);
    REQUIRES    INCLUDE_PLB_BUS \
                INCLUDE_SPI_BUS 
    INIT_AFTER  INCLUDE_SPI_BUS
}

