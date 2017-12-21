#ifndef _vxbGpioIntCtrlH
#define _vxbGpioIntCtrlH


#ifdef DRV_INTCTRL_GPIO
#define GPIO_INT_VEC0	0
#define GPIO_INT_VEC1	1
#define GPIO_INT_VEC2	2
#define GPIO_INT_VEC3	3
#define GPIO_INT_VEC4	4
#define GPIO_INT_VEC5	5
#define GPIO_INT_VEC6	6
#define GPIO_INT_VEC7	7
#define	GPIO_INT_VEC8	8
#define GPIO_INT_VEC9	9
#define GPIO_INT_VEC10	10
#define GPIO_INT_VEC11	11
#define GPIO_INT_VEC12	12
#define GPIO_INT_VEC13	13
#define GPIO_INT_VEC14	14
#endif


#ifdef DRV_INTCTRL_I2C
#define GPIO_INT_I2C0_VEC0	0
#define GPIO_INT_I2C0_VEC1	1
#define GPIO_INT_I2C0_VEC2	2

#define GPIO_INT_I2C1_VEC0	3
#define GPIO_INT_I2C1_VEC1	4
#define GPIO_INT_I2C1_VEC2	5


#endif


#ifdef DRV_INTCTRL_FPGA_TIMER
#define FPGA_TIMER_INT_VEC0	0
#endif


#define		GPIO_INT_ENABLE_A		 0x4
#define		GPIO_INT_ENABLE_B		 0xc
#define 	GPIO_INT_STATUS_A	 0x00
#define     GPIO_INT_STATUS_B	 0x08



typedef struct vxbGpioIntCtlrDrvCtrl
    {
    VXB_DEVICE_ID         	pInst;
    //UINT32  				maxIvec;
    UINT32					maxGpioInt;
    void *	handle;
    struct intCtlrHwConf  	isrHandle; 
    void 					*regAccessHandle;

   } VXB_GPIOINTCTLR_DRV_CTRL;


#endif
