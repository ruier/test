/* 40SpS25flxxSpiFlash.cdf - Spansion S25FLXX flash component configuration file */

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
01a,30aug21,y_y  written.
*/

/*
DESCRIPTION
This file contains components for the Spansion S25FLXX serial flash.
*/

/* Spansion SPI flash driver */

Component DRV_SPI_S25FLXX
{
    NAME            Spansion SPI Flash Driver
    SYNOPSIS        Spansion S25FLXX serial flash driver
    REQUIRES        INCLUDE_FSL_SPI
    _CHILDREN       FOLDER_DRIVERS
}