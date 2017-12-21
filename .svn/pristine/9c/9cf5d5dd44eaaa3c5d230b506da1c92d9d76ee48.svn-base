# Makefile - makefile for IBM750CL Evaluation board 
#
# Copyright (c) 2005-2008 Wind River Systems, Inc.
#
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
#
# Copyright (c) 2005-2006 Tundra Semiconductor Corp.
#
# modification history
# --------------------
# 01e,17jul08,y_w  Removed bootrom_uncmp.hex from the release list
# 01d,07nov07,y_w  Removed Tsi108End.o from MACH_EXTRA, put it into sysLib.c.
#                  Added complete support for booting from big flash.
# 01c,22jul07,y_w  Changed BOARD name to IBM750CL
# 01b,13jul06,avb  Cleanup
# 01a,20jul05,avb  written (based on templatePpc/Makefile)
#
# DESCRIPTION
# This file contains rules for building VxWorks for Tsi109/Tsi110evb board.
#
# INCLUDES
#     makeTarget

CPU         = PPC32
CPU_VARIANT = _ppc604
TOOL        = diab
TGT_DIR     = $(WIND_BASE)/target

include $(TGT_DIR)/h/make/defs.bsp

## Only redefine make definitions below this point, or your definitions will
## be overwritten by the makefile stubs above.

TARGET_DIR  = ibm_evb750cl
VENDOR      = IBM
BOARD       = IBM PPC750CL Evaluation Board

#
# The constants ROM_TEXT_ADRS, ROM_SIZE, and RAM_HIGH_ADRS are defined
# in config.h and Makefile.
# All definitions for these constants must be identical.
# Specify values in hexadecimal, without a leading 0x.
#

#BOOT FROM BIG FLASH
#EXTRA_DEFINE  += -DBOOT_FROM_BIGFLASH
#ROM_TEXT_ADRS	= fe000100 # ROM cold entry address
#ROM_WARM_ADRS	= fe000108 # ROM warm entry address

#BOOT FROM SMALL FLASH
ROM_TEXT_ADRS	= fff00100 # ROM cold entry address
ROM_WARM_ADRS	= fff00108 # ROM warm entry address

ROM_SIZE       	= 00100000 # number of bytes of ROM space
RAM_LOW_ADRS	= 00200000 # RAM text/data address for vxWorks
RAM_HIGH_ADRS	= 00600000 # RAM text/data address for bootrom

# ROM_LINK_ADRS defaults to ROM_TEXT_ADRS, but needs to be different
# here due to complications related to the .space 0x100 at the start
# of romInit.s.

#BOOT FROM BIG FLASH
#ROM_LINK_ADRS  = fe000000 

#BOOT FROM SMALL FLASH
ROM_LINK_ADRS   = fff00000 

USR_ENTRY	= usrInit

# Release define

RELEASE_CMD    += bootrom.bin

# MACH_EXTRA is for any additional runtime modules 
MACH_EXTRA      =  

## Only redefine make definitions above this point, or the expansion of 
## makefile target dependencies may be incorrect.

include $(TGT_DIR)/h/make/rules.bsp
