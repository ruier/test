/* 05eeprom.cdf - eeprom device component description */

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
01b,09apr12,y_y  added INCLUDE_EEPROMDRV to FOLDER_PERIPHERALS.
01a,07feb12,y_y  written.
*/

Component INCLUDE_EEPROMDRV {
    NAME        EEPROM device
    SYNOPSIS    Attach the EEPROM device to standard I/O 
    INIT_RTN    eepromDrv ();
    MODULES     vxbEeprom.o
    SOURCE      $(WIND_BASE)/target/src/hwif/util
    PROTOTYPE   STATUS eepromDrv (void);
    _CHILDREN   FOLDER_PERIPHERALS
    _INIT_ORDER usrIosExtraInit
}
