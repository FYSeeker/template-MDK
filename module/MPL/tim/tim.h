#ifndef _DXL_h_
#define _DXL_h_

#include "stdio.h"
#include "stm32f4xx.h"

void Tim2IntInit(uint16_t arr,uint16_t psc);
void Tim2Irq_Set(void (*func)(void));

#endif

