# Makefile - makefile for mv7100 BSP
#
# Copyright (c) 2007-2009 Wind River Systems, Inc.
#
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
#
# Copyright 2004-2005 Motorola, Inc. All Rights Reseerved
#
# modification history
# --------------------
# 01e,23apr09,e_d  replace macro RELEASE with RELEASE_CMD
# 01d,10sep08,y_w  Remove vxBus drivers from BSP directory
# 01c,29apr08,mdg  Made SMP TIPC unincluded by default
# 01b,25apr08,mdg  Added SMP TIPC
# 01a,09sep07,mdg  Created from 8540 Makefile
#
# DESCRIPTION
# This file contains rules for building VxWorks for the template.
#
# INCLUDES
#     makeTarget

CPU              = PPC32
TOOL             = diab
CPU_VARIANT      = _ppc604_745x
TGT_DIR = $(WIND_BASE)/target

EXTRA_INCLUDE=-I$(TGT_DIR)/src/drv -I$(TGT_DIR)/src/hwif -I$(TGT_DIR)/src/hwif/h

include $(TGT_DIR)/h/make/defs.bsp

## Only redefine make definitions below this point, or your definitions will
## be overwritten by the makefile stubs above.  All the board specific
## defines are listed below, but it is preferable to use the BSP that
## matches the hardware.  Runtime checks for some board differences are not
## possible, so the kernel should be configured to the specific board.

TARGET_DIR      = mv7100
VENDOR          = Emerson
BOARD           = MVME7100

# The constants ROM_TEXT_ADRS, ROM_SIZE are defined
# in config.h and Makefile.
# All definitions for these constants must be identical.
# Specify values in hexadecimal, without a leading 0x.

ROM_TEXT_ADRS   = fff00100 # ROM cold entry address
ROM_WARM_ADRS   = fff00108 # ROM warm entry address 
ROM_SIZE        = 00100000 # number of bytes of ROM space (512K)
RAM_LOW_ADRS    = 00100000 # RAM text/data address for vxWorks
RAM_HIGH_ADRS   = 00e00000 # RAM text/data address for bootrom

# ROM_LINK_ADRS defaults to ROM_TEXT_ADRS, but needs to be different
# here due to complications related to the .space 0x100 at the start
# of romInit.s.

ROM_LINK_ADRS   = fff00000

USR_ENTRY       = usrInit

DOC_FILES       = sysCacheLib sysLib

RELEASE_CMD     += bootrom.bin bootrom.hex

MACH_EXTRA      =  sysMpc8641Smc.o sysMotI2c.o sysMpc8641I2c.o sysAUtil.o 

# Shared Memory is not include in SMP libraries by default
# Instead, include SMP safe BSP version.

ifeq (SMP,$(findstring SMP,$(VXBUILD)))

# The following line should be commented out if Shared Memory is not
#  required in a SMP image.

#MACH_EXTRA     += smLib.o smPktLib.o smUtilLib.o smEnd.o smEndShow.o 

# The following two lines should be uncomment if TIPC is to be included
#  in a SMP image
# MACH_EXTRA    += tipc_sm_media.o
# EXTRA_INCLUDE += -I$(TGT_DIR)/h/tipc -I$(TGT_DIR)/src/tipc

endif

BOOT_EXTRA       = sysMpc8641Smc.o sysMpc8641I2c.o sysMotI2cBoot.o sysAUtil.o

## Only redefine make definitions above this point, or the expansion 
## of the makefile target dependencies may be incorrect.

include $(TGT_DIR)/h/make/rules.bsp

