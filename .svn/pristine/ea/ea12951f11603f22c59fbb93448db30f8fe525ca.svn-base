# Makefile - makefile for Freescale p1022ds board
#
# Copyright (c) 2010 Wind River Systems, Inc.
#
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
#
# modification history
# --------------------
# 01a,17aug10,e_d  derived from fsl_p2020_ds/Makefile
#
# DESCRIPTION
# This file contains rules for building VxWorks for the
# Freescale P1022DS reference board
#
# INCLUDES
#     makeTarget
#

CPU              = PPC32
#optional soft-float kernel
#TOOL            = sfdiab
TOOL             = e500v2diab
CPU_VARIANT      = _ppc85XX_e500v2

TGT_DIR = $(WIND_BASE)/target

include $(TGT_DIR)/h/make/defs.bsp

## Only redefine make definitions below this point, or your definitions will
## be overwritten by the makefile stubs above.

TARGET_DIR      = fsl_p1022_ds
VENDOR          = Freescale
BOARD           = P1022DS

RELEASE         += bootrom.bin

# Boot from SPI flash bootrom

#ROM_TEXT_ADRS  = 7fe00500 # ROM entry address
#ROM_SIZE       = 00100000 # number of bytes of ROM space
#ROM_WARM_ADRS  = 7fe00508 # ROM warm entry address
#ROM_LINK_ADRS  = 7fe00000

# Boot from NOR flash bootrom

ROM_TEXT_ADRS   = fff00100 # ROM entry address
ROM_SIZE        = 00100000 # number of bytes of ROM space
ROM_WARM_ADRS   = fff00108 # ROM warm entry address
ROM_LINK_ADRS   = fff00000

RAM_LOW_ADRS    = 00100000 # RAM text/data address
RAM_HIGH_ADRS   = 01000000 # RAM text/data address

VMA_START       = 0x0

## Only redefine make definitions above this point, or the expansion of
## makefile target dependencies may be incorrect.

include $(TGT_DIR)/h/make/rules.bsp
