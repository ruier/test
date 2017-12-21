# Makefile - makefile for bsp/config/fsl_rdb8315
#
# Copyright (c) 2008-2009 Wind River Systems, Inc.
#
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
#
# modification history
# --------------------
# 01b,11dec09,x_z  Add support for booting from NOR flash BootRom.
# 01a,05may08,z_l  Created.
#
# DESCRIPTION
# This file contains rules for building VxWorks for the
# Freescale MPC8315ERDB Board with a PowerPC 8315
#
# INCLUDES
#     makeTarget
#

CPU              = PPC32
CPU_VARIANT      = _ppc603_83xx
TOOL             = diab

TGT_DIR = $(WIND_BASE)/target
include $(TGT_DIR)/h/make/defs.bsp

## Only redefine make definitions below this point, or your definitions will
## be overwritten by the makefile stubs above.

TARGET_DIR      = fsl_rdb8315
VENDOR          = Freescale
BOARD           = MPC8315ERDB

USR_ENTRY       = usrInit

RELEASE_CMD    += bootrom.bin

## The constants ROM_TEXT_ADRS and ROM_SIZE are defined in config.h and
## Makefile. All definitions for these constants must be identical.

# boot from NOR flash BootRom

ROM_TEXT_ADRS   = fe000100 # ROM entry address
ROM_SIZE        = 00100000 # number of bytes of ROM space
ROM_WARM_ADRS   = fe000108 # ROM warm entry address
ROM_LINK_ADRS   = fe000000

# boot from U-Boot

#ROM_TEXT_ADRS  = fe090100 # ROM entry address
#ROM_SIZE       = 00100000 # number of bytes of ROM space
#ROM_WARM_ADRS  = fe090108 # ROM warm entry address
#ROM_LINK_ADRS  = fe090000

RAM_LOW_ADRS    = 00010000 # RAM text/data address
RAM_HIGH_ADRS   = 00C00000 # RAM text/data address

# uncomment the following line to enable debugging
# ADDED_CFLAGS += -g

EXTRA_DEFINE   = -DVXWORKS -DMPC83XX

## Only redefine make definitions above this point, or the expansion of
## makefile target dependencies may be incorrect.

include $(TGT_DIR)/h/make/rules.bsp
