#ifndef __dht_H
#define __dht_H 

#include "stm32f0xx.h"


#define CHECK_TIME 28
#define Dht_RCC    RCC_AHBPeriph_GPIOA
#define Dht_GPIO   GPIOA
#define Dht_PIN    GPIO_Pin_6


//IO∑ΩœÚ…Ë÷√

#define Dht_H()    GPIO_SetBits(Dht_GPIO, Dht_PIN)
#define Dht_L()    GPIO_ResetBits(Dht_GPIO, Dht_PIN)
unsigned int Dht11_read(void);
//void mode_output(void );
//void mode_input(void );
void Dht_Init(void);
#endif
