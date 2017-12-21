# Makefile - makefile for bsp/config/fsl_mpc830x_mitx
#
#
# Copyright (c) 2011 Wind River Systems, Inc.
#
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
#
# modification history
# --------------------
# 01a,30jun11,e_d  initial creation, based on fsl_rdb8308
#
# DESCRIPTION
# This file contains rules for building VxWorks for the
# Freescale MPC830x mitx reference board
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

TARGET_DIR   = fsl_mpc830x_mitx
VENDOR       = Freescale
BOARD        = MPC830X_MITX

USR_ENTRY    = usrInit

LIB_EXTRA    =

## boot from NAND flash BootRom, should used the following macro

#ROM_EXTRA    = bootInit_nand.o sysI2cBootSet.o
#BOOT_EXTRA   = bootInit_nand.o sysI2cBootSet.o

## boot from NOR flash BootRom, should used the following macro

BOOT_EXTRA    = sysI2cBootSet.o
ROM_EXTRA     = sysI2cBootSet.o

RELEASE       += bootrom.bin

## The constants ROM_TEXT_ADRS, ROM_SIZE, and RAM_HIGH_ADRS are defined
## in config.h and Makefile. All definitions for these constants must be
## identical.

## boot from NOR flash BootRom, should used the following macro

ROM_TEXT_ADRS    = fe000100 # ROM entry address
ROM_SIZE         = 00100000 # number of bytes of ROM space
ROM_WARM_ADRS    = fe000108 # ROM warm entry address 
ROM_LINK_ADRS    = fe000000

## boot from 8309 NAND flash BootRom, should used the following macro
## Note by default, ROM_SIZE is set 1MB. If image size is more than 
## 1MB size, should increase ROM_SIZE to right size and modify 
## ROM_TEXT_ADRS ROM_WARM_ADRS ROM_LINK_ADRS to right value. For example
## ROM_SIZE is set to 00200000. ROM_TEXT_ADRS = 0fe00100 ROM_WARM_ADRS = 0fe00108
## ROM_LINK_ADRS = 0fe00000

#ROM_TEXT_ADRS    = 0ff00100 # ROM entry address
#ROM_SIZE         = 00100000 # number of bytes of ROM space
#ROM_WARM_ADRS    = 0ff00108 # ROM warm entry address 
#ROM_LINK_ADRS    = 0ff00000

## boot from 8306 NAND flash BootRom, should used the following macro

#ROM_TEXT_ADRS    = 07f00100 # ROM entry address
#ROM_SIZE         = 00100000 # number of bytes of ROM space
#ROM_WARM_ADRS    = 07f00108 # ROM warm entry address 
#ROM_LINK_ADRS    = 07f00000

RAM_LOW_ADRS     = 00010000 # RAM text/data address
RAM_HIGH_ADRS    = 00c00000 # RAM text/data address

iram_mpc8306_r1.obj:
	objcopyppc -I binary -O elf32-powerpc-vxworks \
		-N _binary_irammpc8306r1_size \
		-B powerpc:603 iram_mpc8306_r1.bin iram_mpc8306_r1.obj

MACH_EXTRA  += iram_mpc8306_r1.obj

# uncomment the following line to enable debugging
#ADDED_CFLAGS +=  -g 
EXTRA_DEFINE   = -DVXWORKS -DMPC83XX

EXTRA_MODULES  =

## Only redefine make definitions above this point, or the expansion of 
## makefile target dependencies may be incorrect.

include $(TGT_DIR)/h/make/rules.bsp 
