# Makefile - makefile for Freescale HPC-II board 
#
# Copyright (c) 2006 Wind River Systems, Inc.
# Copyright 2005 Tundra Semiconductor Corp.
#
# modification history
# --------------------
# 01d,31mar06,dtr  Cleanup.
# 01c,22mar06,dtr  Add extra boot images.
# 01b,10jan06,dtr  cleanup for Wind River release.
# 01a,20jul05,avb  written (based on templatePpc/Makefile)
#
# DESCRIPTION
# This file contains rules for building VxWorks for the HPC-II board.
#
# INCLUDES
#     makeTarget

CPU              = PPC604
TOOL             = diab
CPU_VARIANT      = _745x
TGT_DIR = $(WIND_BASE)/target

include $(TGT_DIR)/h/make/defs.bsp

## Only redefine make definitions below this point, or your definitions will
## be overwritten by the makefile stubs above.

TARGET_DIR	= hpc2-7448
VENDOR		= Tundra
BOARD           = Freescale HPC-II 7448

#
# The constants ROM_TEXT_ADRS, ROM_SIZE, and RAM_HIGH_ADRS are defined
# in config.h and Makefile.
# All definitions for these constants must be identical.
# Specify values in hexadecimal, without a leading 0x.
#

ROM_TEXT_ADRS	= fff00100 # ROM cold entry address
ROM_WARM_ADRS	= fff00108 # ROM warm entry address
ROM_SIZE	= 00100000 # number of bytes of ROM space
RAM_LOW_ADRS	= 00010000 # RAM text/data address for vxWorks
RAM_HIGH_ADRS	= 00400000 # RAM text/data address for bootrom

# ROM_LINK_ADRS defaults to ROM_TEXT_ADRS, but needs to be different
# here due to complications related to the .space 0x100 at the start
# of romInit.s.

ROM_LINK_ADRS   = fff00000 

##
##   altivec processor support
##
##ADDED_CFLAGS +=  -fvec

USR_ENTRY	= usrInit

# MACH_EXTRA is for any additional runtime modules 
MACH_EXTRA      =  tsi108End.o

RELEASE         += bootrom_uncmp.hex

## Only redefine make definitions above this point, or the expansion of 
## makefile target dependencies may be incorrect.

include $(TGT_DIR)/h/make/rules.bsp
