# Makefile - LSI ACP34XX Evaluation ELPASO board
#
# Copyright 2010 Wind River Systems, Inc.
#
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
#
# modification history
# --------------------
# 01c,16dec10,x_z  added support for hard float point.
# 01b,18oct10,x_z  code clean.
# 01a,26jan10,x_s  written.
#
# DESCRIPTION
# This file contains rules for building VxWorks for the LSI ACP34XX ELPASO
# Evaluation board.
# This Makefile generates a 1024 KB bootrom image.
#
# INCLUDES
#     makeTarget
#*/

CPU              = PPC32
TOOL             = diab
CPU_VARIANT      = _ppc476

TGT_DIR=$(WIND_BASE)/target
include $(TGT_DIR)/h/make/defs.bsp

## Only redefine make definitions below this point, or your definitions will
## be overwritten by the makefile stubs above.

TARGET_DIR       = lsi_acp34xx_elpaso
VENDOR           = LSI
BOARD            = ELPASO

USR_ENTRY        = usrInit

RELEASE_CMD      += bootrom.bin

#
# The constants ROM_TEXT_ADRS, ROM_SIZE, and RAM_HIGH_ADRS are defined
# in config.h, MakeSkel, Makefile, and Makefile.*
# All definitions for these constants must be identical.
#

# boot from U-Boot

ROM_TEXT_ADRS   = 3ff00000 # ROM entry address
ROM_WARM_ADRS   = 3ff00004 # ROM warm entry address
ROM_SIZE        = 00100000 # number of bytes of ROM space
RAM_LOW_ADRS    = 00100000 # RAM text/data address
RAM_HIGH_ADRS   = 01000000 # RAM text/data address

DATA_SEG_ALIGN  = 00100000 # Align data section for TLB locking

## Only redefine make definitions above this point, or the expansion of
## makefile target dependencies may be incorrect.

include $(TGT_DIR)/h/make/rules.bsp
