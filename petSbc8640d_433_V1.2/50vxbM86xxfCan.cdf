				
Component DRV_M86XX_FCAN {
        NAME            mpc86xx FPGA_CAN VxBus driver
        SYNOPSIS        mpc86xx FPGA_CAN VxBus driver provided by prophet
      
        PROTOTYPE       void fCanDevRegister(void);
        INIT_RTN        fCanDevRegister();
         _CHILDREN      FOLDER_DRIVERS
        _INIT_ORDER     hardWareInterFaceBusInit
        REQUIRES        INCLUDE_VXBUS \
                        INCLUDE_PLB_BUS
        INIT_AFTER      INCLUDE_PLB_BUS
        								INCLUDE_PCI_BUS
}
