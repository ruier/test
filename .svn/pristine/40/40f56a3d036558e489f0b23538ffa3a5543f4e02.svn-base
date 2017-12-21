
CPU             = PPC32
TOOL            = e500v2diab
#TOOL            = e500v2gnu


TGT_DIR = $(WIND_BASE)/target

include $(TGT_DIR)/h/make/defs.bsp

TARGET_DIR      = fsl_p2020_ds_vpx6
VENDOR          = Prophet
BOARD           = P2020DS


BSP_DIR = $(TGT_DIR)/config/$(TARGET_DIR)
SRC_DIR = $(TGT_DIR)/config/common_lib
VPX6-SP2020OBJDIR   = $(BSP_DIR)$(DIRCHAR)common_lib$(DIRCHAR)obj$(BOARD)$(CPU)$(TOOL)vx
VPX6-SP2020LIB = $(BSP_DIR)$(DIRCHAR)common_lib$(DIRCHAR)lib$(BOARD)$(CPU)$(TOOL)vx.a

ADDED_CFLAGS    += -g2

#include $(TGT_DIR)/h/make/rules.bsp
EXTRA_INCLUDE  += -I$(BSP_DIR)
EXTRA_INCLUDE  += -I$(SRC_DIR)/drv_inc
EXTRA_INCLUDE  += -I$(SRC_DIR)/drv_src
EXTRA_INCLUDE  += -I$(SRC_DIR)/tools

EXTRA_INCLUDE  += -I$(TGT_DIR)/h/ -I$(TGT_DIR)/src/ -I$(WIND_HOME)/components

P2020_LIB:
#SRIO				
	$(MAKE) $(SRC_DIR)/drvApi.o 
	$(AR) rv $(VPX6-SP2020LIB) $(SRC_DIR)/drvApi.o
	$(MV) $(SRC_DIR)/drvApi.o $(VPX6-SP2020OBJDIR)	

#TSEC				
#	$(MAKE) $(SRC_DIR)/vxbEtsecEnd.o 
#	$(AR) rv $(VPX6-SP2020LIB) $(SRC_DIR)/vxbEtsecEnd.o
#	$(MV) $(SRC_DIR)/vxbEtsecEnd.o $(VPX6-SP2020OBJDIR)		

clean:
	rm -rf *.a obj*



	