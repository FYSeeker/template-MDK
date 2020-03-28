#include "sun/sun.h"
#include "stm32f0xx.h"
#include "adc/adc.h"




u8 SunVauleGet(void)
{
  	return ADC_ConvertedValue[2]/16;
}

