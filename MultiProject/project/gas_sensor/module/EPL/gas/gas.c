#include "gas/gas.h"
#include "stm32f0xx.h"
#include "adc/adc.h"
#include "math.h"
u8 ppm=0;    
float MP530_vol=0;

u8 GasVauleGet(void)
{

	float MP530_res=0;
  MP530_vol=((float)ADC_ConvertedValue[2]/4096)*3.3;
  MP530_res=(RES_TEST/MP530_vol)*(5-MP530_vol);
	ppm=exp(-2.5*log10(MP530_res/R0));
  return ppm;
}

