#ifndef __TIM_H
#define __TIM_H
#include "stm32f0xx.h"

#define TIMLongOut  TIM1->CNT>10000
#define gas_vaule 26
#define sensor_id 106

void Tim3PwmInit(void);
void TIM2_Int_Init(u16 arr,u16 psc);
void TIM14_Int_Init(u16 arr,u16 psc);
void TIM3_USER_Config(void);
void TIM1Init(void);
void OTstart(void);
void OTdisen(void);
void ParameterInit(void);
void PWM_Init(void);
#endif
