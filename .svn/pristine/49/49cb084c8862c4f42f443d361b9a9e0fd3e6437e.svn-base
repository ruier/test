# Makefile - makefile for bsp/config/fsl_mpc8308_rdb
#
#
# Copyright (c) 2010 Wind River Systems, Inc.
#
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
#
# modification history
# --------------------
# 01a,16jun10,syt  initial creation, based on fsl_rdb8313
#
# DESCRIPTION
# This file contains rules for building VxWorks for the
# Freescale MPC8308_RDB reference board
#
# INCLUDES
#     makeTarget
#*/

CPU              = PPC32
CPU_VARIANT      = _ppc603_83xx
TOOL             = diab 

TGT_DIR = $(WIND_BASE)/target
include $(TGT_DIR)/h/make/defs.bsp

## Only redefine make definitions below this point, or your definitions will
## be overwritten by the makefile stubs above.

TARGET_DIR   = fsl_mpc8308_rdb
VENDOR       = Freescale
BOARD        = MPC8308_RDB

USR_ENTRY    = usrInit

LIB_EXTRA    =

RELEASE      += bootrom.bin

## The constants ROM_TEXT_ADRS, ROM_SIZE, and RAM_HIGH_ADRS are defined
## in config.h and Makefile. All definitions for these constants must be
## identical.

ROM_TEXT_ADRS    = fe000100 # ROM entry address
ROM_SIZE         = 00190000 # number of bytes of ROM space
ROM_WARM_ADRS    = fe000108 # ROM warm entry address 
ROM_LINK_ADRS    = fe000000

RAM_LOW_ADRS     = 00010000 # RAM text/data address
RAM_HIGH_ADRS    = 00c00000 # RAM text/data address

vsc2bin.obj:
	objcopyppc -I binary -O elf32-powerpc-vxworks \
		-N _binary_vsc2bin_size \
		-B powerpc:603 vsc2bin vsc2bin.obj

# uncomment the following line to enable debugging
#ADDED_CFLAGS +=  -g 
EXTRA_DEFINE   = -DVXWORKS -DMPC83XX

MACH_EXTRA	   += vsc2bin.obj

EXTRA_MODULES  =

## Only redefine make definitions above this point, or the expansion of 
## makefile target dependencies may be incorrect.

include $(TGT_DIR)/h/make/rules.bsp 
