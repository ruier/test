# Makefile - Makefile for Freescale lite5200b BSP
#
# Copyright (c) 2004-2008 Wind River Systems, Inc.
#
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
#
# modification history
# --------------------
# 01o,06oct08,b_m  add bootrom.bin
# 01n,26aug08,l_g  remove the comments about RAM_HIGH_ADRS
# 01m,21apr08,b_m  change CPU to PPC603 for building libBestComm.a
# 01l,21aug07,b_m  update CPU & CPU_VARIANT.
# 01k,17jan07,b_m  Remove some non-standard libraries.
# 01j,03feb06,pkr  add support for Lite5200b
# 01i,24jan05,pkr  default gnu, change Motorola into Freescale,
#                  private bootConfig.c for datalight,
#                  force build of libs to gnu, add usb
# 01h,14apr05,j_b  fix TARGET_DIR (SPR 108020)
# 01g,23mar05,j_b  fix BestComm lib build rule for project creation failure
# 01f,02mar05,j_b  add libBestComm.a build rule
# 01e,24feb05,j_b  removed MPC5200Drv and added BestComm lib.
# 01d,30dec04,k_p  removed BSP's High version macros.
# 01c,23dec04,k_p  default TOOL to diab.
# 01b,04mar04,bjn  Moved m5200Sdma.c to MPC5200Drv build
#                  libMPC5200Drv.a is no longer in the BSP directory.
#                  Changed default build to IS_ICECUBE and BOOT_HIGH = 1
# 01a,04feb04,bjn  Support ICECUBE_OLD memory option
#                  Support for Tornado Project builds
#
# DESCRIPTION
# -----------
# This file contains rules for building VxWorks for the
# Freescale Lite5200B board.
#
# TARGETS
# -------
# This section briefly lists the available build targets.
#
# INCLUDES
#     makeTarget
#

CPU             = PPC32
CPU_VARIANT     = _ppc603
TOOL            = diab

TGT_DIR = $(WIND_BASE)/target
include $(TGT_DIR)/h/make/defs.bsp

## Only redefine make definitions below this point, or your definitions will
## be overwritten by the makefile stubs above.

TARGET_DIR      = lite5200b
VENDOR          = Freescale
BOARD           = Lite5200B

RELEASE_CMD    += bootrom.bin

USR_ENTRY       = usrInit

## The constants ROM_TEXT_ADRS and ROM_SIZE are defined in config.h, Makefile.
## All definitions for these constants must be identical.

## ROM_SIZE (number of bytes of ROM space)
ROM_SIZE        = 01000000 # 16MB

## ROM_TEXT_ADRS: ROM entry address
##

ROM_TEXT_ADRS   = FFF00100 # vxWorks bootrom
#ROM_TEXT_ADRS  = FF800100 # U-Boot jump
#ROM_TEXT_ADRS  = 02000000 # U-Boot load

## RAM_HIGH_ADRS: RAM text/data address.
##                Used for copying bootrom images to.

RAM_HIGH_ADRS   = 01D00000

## RAM_LOW_ADRS: RAM text/data address.
##               The address at which to load VxWorks.

RAM_LOW_ADRS    = 00100000

## LIBBESTCOMM: The library is stored in the bestComm directory

BESTCOMM_DIR     = $(TGT_DIR)/config/lite5200b/bestComm
BESTCOMM_VARIANT = image_rtos1

LIBBESTCOMM     = $(BESTCOMM_DIR)/libBestComm.a

$(TGT_DIR)/config/lite5200b/bestComm/libBestComm.a:
	$(MAKE) -C $(BESTCOMM_DIR) CPU=PPC603 TOOL=$(TOOL) all

## Extra defines required for lite5200b

EXTRA_INCLUDE   += -I$(BESTCOMM_DIR)/code_dma/$(BESTCOMM_VARIANT)
EXTRA_INCLUDE   += -I$(BESTCOMM_DIR)/capi
EXTRA_INCLUDE   += -I$(BESTCOMM_DIR)
EXTRA_DEFINE    += -Dinline=__inline__

LD_LINK_PATH_EXTRA +=

RELEASE_PRE     += $(LIBBESTCOMM)

## For both the VxWorks image and boot loader

LIB_EXTRA       += $(LIBBESTCOMM)

## For the VxWorks image only

EXTRA_MODULES   +=

## For the boot loader only

MACH_EXTRA      +=

## Only redefine make definitions above this point, or the expansion of
## makefile target dependencies may be incorrect.

include $(TGT_DIR)/h/make/rules.bsp
