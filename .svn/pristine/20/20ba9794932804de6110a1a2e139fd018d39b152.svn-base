# Makefile - makefile for Freescale p2020ds board
#
# Copyright (c) 2009 Wind River Systems, Inc.
#
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
#
# modification history
# --------------------
# 01a,07may09,syt  derived from ads8572/Makefile/01a
#
# DESCRIPTION
# This file contains rules for building VxWorks for the
# Freescale P2020DS reference board
#
# INCLUDES
#     makeTarget
#

CPU              = PPC32
#optional soft-float kernel

TOOL            = sfdiab
#TOOL             = e500v2gun

CPU_VARIANT      = _ppc85XX_e500v2

TGT_DIR = $(WIND_BASE)/target
SRC_DIR = $(TGT_DIR)/config/common_lib

include $(TGT_DIR)/h/make/defs.bsp

## Only redefine make definitions below this point, or your definitions will
## be overwritten by the makefile stubs above.

TARGET_DIR      = fsl_p2020_ds_vpx6
VENDOR          = Prophet
BOARD           = P2020DS

RELEASE         += bootrom.bin


USRCONFIG = usrConfig.c
#BOOTINIT        = bootInit.c

EXTRA_INCLUDE  += -I$(SRC_DIR)
EXTRA_INCLUDE  += -I$(SRC_DIR)/drv_inc
EXTRA_INCLUDE  += -I$(SRC_DIR)/drv_src

EXTRA_INCLUDE  += -I$(TGT_DIR)/config/$(TARGET_DIR)/board_spec_inc
EXTRA_INCLUDE  += -I$(TGT_DIR)/config/$(TARGET_DIR)/wind_code

ifeq ($(MAKECMDGOALS),bootrom.bin)
EXTRA_DEFINE += -DCOMPILE_BOOTROM
endif

ifeq ($(MAKECMDGOALS),bootrom)
EXTRA_DEFINE += -DCOMPILE_BOOTROM
endif

ROM_TEXT_ADRS   = fff00100 # ROM entry address
ROM_SIZE        = 00100000 # number of bytes of ROM space
ROM_WARM_ADRS   = fff00108 # ROM warm entry address
ROM_LINK_ADRS   = fff00000
RAM_LOW_ADRS    = 00100000 # RAM text/data address for vxWorks ?
RAM_HIGH_ADRS   = 01000000 # RAM text/data address for bootrom ?

LIB_EXTRA += common_lib/libP2020DSPPC32e500v2diabvx.a

VMA_START       = 0x0

## Only redefine make definitions above this point, or the expansion of
## makefile target dependencies may be incorrect.

include $(TGT_DIR)/h/make/rules.bsp
