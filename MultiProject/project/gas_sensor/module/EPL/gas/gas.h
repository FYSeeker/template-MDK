#ifndef __GPIO_H
#define __GPIO_H
#include "stm32f0xx.h"

#define Sensor_RCC    RCC_AHBPeriph_GPIOA
#define Sensor_GPIO   GPIOA
#define Sensor_PIN    GPIO_Pin_6
#define R0 280
#define RES_TEST 47
u8 GasVauleGet(void);
#endif 
