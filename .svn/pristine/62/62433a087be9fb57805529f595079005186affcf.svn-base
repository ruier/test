# Makefile - makefile for bsp/config/mds837x
#
# Copyright (c) 2007-2010 Wind River Systems, Inc.
#
# modification history
# --------------------
# 01e,19dec10,x_z  add support for BMS_HIGH_ENABLE and code clean.
# 01d,10nov09,x_z  modifiy ROM_TEXT_ADRS to protect environment variables for
#                  U-Boot and code clean.
# 01c,08oct08,syt  modified RAM_HIGH_ADRS (WIND00135772)
# 01b,17jul08,x_s  added image bootrom.hex
# 01a,19jan07,dtr  created from ads834x Makefile
#
# DESCRIPTION
# This file contains rules for building VxWorks for the
# Motorola Voyager Application Development Support Board with a MPC 837x
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

TARGET_DIR      = mds837x
VENDOR          = Freescale
BOARD           = mds837x

USR_ENTRY       = usrInit

RELEASE_CMD    += bootrom.bin

## The constants ROM_TEXT_ADRS, ROM_SIZE, and RAM_HIGH_ADRS are defined
## in config.h and Makefile. All definitions for these constants must be
## identical.

# boot from VxWorks bootrom and BMS_HIGH_ENABLE is undefined

ROM_TEXT_ADRS  = fe000100 # ROM entry address
ROM_WARM_ADRS  = fe000108 # ROM warm entry address
ROM_LINK_ADRS  = fe000000

# boot from VxWorks bootrom and BMS_HIGH_ENABLE is defined

#ROM_TEXT_ADRS  = fff00100 # ROM entry address
#ROM_WARM_ADRS  = fff00108 # ROM warm entry address
#ROM_LINK_ADRS  = fff00000

# boot from U-Boot

#ROM_TEXT_ADRS   = fe100100 # ROM entry address
#ROM_WARM_ADRS   = fe100108 # ROM warm entry address
#ROM_LINK_ADRS   = fe100000

ROM_SIZE        = 00100000 # number of bytes of ROM space
RAM_LOW_ADRS    = 00010000 # RAM text/data address
RAM_HIGH_ADRS   = 01000000 # RAM text/data address

# uncomment the following line to enable debugging
#ADDED_CFLAGS +=  -g -O

EXTRA_DEFINE   = -DVXWORKS -DMPC83XX

## Only redefine make definitions above this point, or the expansion of
## makefile target dependencies may be incorrect.

include $(TGT_DIR)/h/make/rules.bsp

romExtras.a: $(EXTRA_OBJS)
	$(AR) crus $@ $(EXTRA_OBJS)

