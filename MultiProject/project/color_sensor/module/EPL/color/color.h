#ifndef __color_H
#define __color_H

#include "stm32f0xx.h"
void KEY_EXTI_Init(void);
void tcs3200_init(void);
void whitebalance(void);
u16 tcs3200_GREEN(void);
u16 tcs3200_RED(void);
u16 tcs3200_BLUE(void);


#endif

