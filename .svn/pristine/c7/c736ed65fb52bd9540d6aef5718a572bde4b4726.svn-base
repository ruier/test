# Makefile - makefile for Wind River guestOS virtual board wrVbP2020rdb BSP
#
# Copyright (c) 2011 Wind River Systems, Inc. */
#
#  The right to copy, distribute, modify or otherwise make use
#  of this software may be licensed only pursuant to the terms
#  of an applicable Wind River license agreement.
# 
# modification history
# --------------------
# 01a,15feb11,agf  created based on wrVb8548 rev 01a
#
# DESCRIPTION
# This file contains rules for building VxWorks for the
# WRS Virtual Board BSP for P2020RDB.
#
# INCLUDES
#     makeTarget
#*/

CPU	        = PPC32
CPU_VARIANT     = _ppc85XX_e500v2
TOOL	        = sfdiab

TGT_DIR = $(WIND_BASE)/target

include $(TGT_DIR)/h/make/defs.bsp

## Only redefine make definitions below this point, or your definitions will
## be overwritten by the makefile stubs above.


TARGET_DIR	= wrVbP2020rdb 
VENDOR		= WRS
BOARD		= WRVBP2020RDB

## The constants ROM_TEXT_ADRS, ROM_SIZE, and RAM_HIGH_ADRS are defined
## in config.h and Makefile. All definitions for these constants must be
## identical.

ROM_TEXT_ADRS    = fff00100 # ROM entry address
ROM_SIZE         = 00080000 # number of bytes of ROM space
ROM_WARM_ADRS    = fff00108 # ROM warm entry address 
ROM_LINK_ADRS	 = fff00000

RAM_LOW_ADRS     = 00010000 # RAM text/data address
RAM_HIGH_ADRS    = 00100000 # RAM text/data address

DATA_SEG_ALIGN   = 00001000 # page align data section

MACH_EXTRA      =
BOOT_EXTRA      = 
LIB_EXTRA       =

VMA_START       = 0x0
USR_ENTRY       = usrInit

RELEASE_CMD     = 
RELEASE_PRJ     =
RELEASE_PRE     =
RELEASE_POST    =
RELEASE         =

docs man : EXTRA_DEFINE += -DWRVB_CMDLINE_TARGET_SUPPORTED

ifeq ($(BSP2PRJ), TRUE)
EXTRA_DEFINE += -DWRVB_CMDLINE_TARGET_SUPPORTED
endif

## Only redefine make definitions above this point, or the expansion of 
## makefile target dependencies may be incorrect.

include $(TGT_DIR)/h/make/rules.bsp
