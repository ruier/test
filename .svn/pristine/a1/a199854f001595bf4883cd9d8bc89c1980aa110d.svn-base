/* 40vxbSp25SpiFlash.cdf - Component configuration file */

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
01a,14sep12,y_y  written.
*/

Component DRV_SPIFLASH_SP25 {
    NAME            Spansion S25XX serials SPI Flash vxBus Driver
    SYNOPSIS        This enables access to most modern SPI flash chips, series 
                    supported include Atmel AT25, Spansion S25, ST M25P, EN25
                    and Winbond W25X.  Other chips are supported as well.  See 
                    the driver source for the current list. 
                    Note that the Atmel AT45 series, need an entirely 
                    different driver (DRV_SPIFLASH_AT45).
    MODULES         vxbSp25SpiFlash.o
    INIT_RTN        vxbSp25SpiFlashRegister();
    PROTOTYPE       void vxbSp25SpiFlashRegister (void);
    _INIT_ORDER     hardWareInterFaceBusInit
    INIT_AFTER      INCLUDE_SPI_BUS
    REQUIRES        INCLUDE_VXBUS       \
                    INCLUDE_PLB_BUS     \
                    INCLUDE_SPI_BUS
    _CHILDREN       FOLDER_DRIVERS
}
