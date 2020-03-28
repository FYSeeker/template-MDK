#ifndef __FAN_H
#define __FAN_H
#include "stm32f0xx.h"

#define NO 0X00 
#define FORWARD 0X01 
#define BACK 0X02 

void Run_Moto(u8 data);
#endif