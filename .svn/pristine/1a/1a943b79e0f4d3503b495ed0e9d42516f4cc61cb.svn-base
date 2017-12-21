# Makefile - make rules for target/config/amcc440ep AMCC PPC440EP board
#
# Copyright 2006, 2012 Wind River Systems, Inc.
#
# modification history
# --------------------
# 01d,30mar11,y_c  Added ROM_EXTRA definition. (WIND00201051)
# 01c,02mar07,rec  WIND00089420 - remove ipnet makefile workaround
# 01b,31oct06,rec  vxWorks 6.5 update
# 01a,10mar06,mmi  created based on bamboo version 01d.
#
# DESCRIPTION
# This file contains rules for building VxWorks for the
# AMCC PPC440EP board.
# This Makefile generates a 1024 KB bootrom image.
#
#
# INCLUDES
#     makeTarget
#*/

CPU              = PPC32
CPU_VARIANT	= _ppc440_x5
TOOL             = diab

TGT_DIR=$(WIND_BASE)/target
include $(TGT_DIR)/h/make/defs.bsp

## Only redefine make definitions below this point, or your definitions will
## be overwritten by the makefile stubs above.


TARGET_DIR       = amcc440ep
VENDOR           = AMCC
BOARD            = AMCC440EP 

USR_ENTRY        = usrInit

LIB_EXTRA	 = romExtras.a

#
# The constants ROM_TEXT_ADRS, ROM_SIZE, and RAM_HIGH_ADRS are defined
# in config.h, MakeSkel, Makefile, and Makefile.*
# All definitions for these constants must be identical.
#

ROM_TEXT_ADRS      = FFF00000 # ROM entry address
ROM_SIZE           = 00100000 # number of bytes of ROM space

RAM_LOW_ADRS       = 00010000
RAM_HIGH_ADRS      = 00C00000

## Only redefine make definitions above this point, or the expansion of
## makefile target dependencies may be incorrect.

include $(TGT_DIR)/h/make/rules.bsp


# Additional objects used by romInit

EXTRA_OBJS = romSdramInit.o gpio.o

ROM_EXTRA  = romSdramInit.o gpio.o


romExtras.a: $(EXTRA_OBJS)
	$(AR) crus $@ $(EXTRA_OBJS)
