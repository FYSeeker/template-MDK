#ifndef __touch_H
#define __touch_H
#include "stm32f0xx.h"

#define Sensor_RCC    RCC_AHBPeriph_GPIOA
#define Sensor_GPIO   GPIOA
#define Sensor_PIN    GPIO_Pin_6

void TouchInit(void);
u8 TouchStatusGet(void);
#endif 
