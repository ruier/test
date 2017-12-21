# Makefile - makefile for bsp/config/fsl_rdb8323
#
# Copyright (c) 2006-2007, 2009-2010 Wind River Systems, Inc.
#
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
#
# modification history
# --------------------
# 01d,15oct10,kab  Rename BSP from rdb8323
# 01c,29jun09,e_d  replace macro RELEASE with RELEASE_CMD
# 01b,12jul07,vik  removed cmdLine.c build
# 01a,07nov06,pmr  created from mds8323 Makefile
#
# DESCRIPTION
# This file contains rules for building VxWorks for the
# Freescale RDB board with a PowerPC 8323
#

CPU              = PPC32
CPU_VARIANT      = _ppc603_83xx
TOOL             = sfdiab

TGT_DIR = $(WIND_BASE)/target
include $(TGT_DIR)/h/make/defs.bsp

## Only redefine make definitions below this point, or your definitions will
## be overwritten by the makefile stubs above.

TARGET_DIR	= fsl_rdb8323
VENDOR		= Motorola
BOARD		= rdb8323

USR_ENTRY   = usrInit

RELEASE_CMD		+= bootrom.bin

## The constants ROM_TEXT_ADRS, ROM_SIZE, and RAM_HIGH_ADRS are defined
## in config.h and Makefile. All definitions for these constants must be
## identical.

ROM_TEXT_ADRS    = fe000100 # ROM entry address
ROM_SIZE         = 00100000 # number of bytes of ROM space
ROM_WARM_ADRS    = fe000108 # ROM warm entry address 
ROM_LINK_ADRS	   = fe000000

RAM_LOW_ADRS     = 00010000 # RAM text/data address
RAM_HIGH_ADRS    = 00e00000 # RAM text/data address

# uncomment the following line to enable debugging
#ADDED_CFLAGS +=  -g -O
EXTRA_DEFINE  += -DVXWORKS -DMPC83XX

MACH_EXTRA      = 

## Only redefine make definitions above this point, or the expansion of 
## makefile target dependencies may be incorrect.

include $(TGT_DIR)/h/make/rules.bsp

