#ifndef  __INCLUDES_H__
#define  __INCLUDES_H__

#include "os_cfg_app.h"
#include "bsp_led.h"
#include "bsp_servo.h"
#include "bsp_motor.h"
#include "bsp_adc.h"
#include "bsp_digitalsensor.h"
#include "bsp_ultrasnio.h"
#include "bsp_touch.h"
#include "bsp_fsmc_sram.h"
#include "bsp_spi_nrf.h"
#include "bsp_music.h"
#include "bsp_mpu6050.h"

/* 提供给其他C文件调用的函数 */
void BSP_Init(void);
void BSP_Tick_Init (void);

#endif /* __INCLUDES_H__ */



