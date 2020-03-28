#ifndef __ADC_H
#define	__ADC_H


#include "stm32f0xx.h"

#define ADC1_DR_Address                0x40012440
extern __IO uint16_t ADC_ConvertedValue[4];

void ADC1_Init(void);


#endif /* __ADC_H */

