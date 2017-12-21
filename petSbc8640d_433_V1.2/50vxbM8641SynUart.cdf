				
Component DRV_M8641_SYN_UART {
        NAME           Synchronous Uart VxBus driver
        SYNOPSIS       Synchronous Uart VxBus driver provided by prophet
      
        PROTOTYPE       void m8641SynUartDrvRegister(void);
        INIT_RTN        m8641SynUartDrvRegister();
         _CHILDREN      FOLDER_DRIVERS
        _INIT_ORDER     hardWareInterFaceBusInit
        REQUIRES        INCLUDE_VXBUS \
                        INCLUDE_PLB_BUS
         INIT_AFTER      INCLUDE_PLB_BUS
}


