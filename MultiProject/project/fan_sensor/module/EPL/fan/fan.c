#include "fan.h"
#include "system/delay/delay.h"

static u8 Run_Direction=NO;


static u16 Pwm_Num[5]={800,950,1100,1250,1400};

void Run_Forward(u16 pwmnum)
{
  Run_Direction=FORWARD;
  TIM_SetCompare1(TIM3,pwmnum); 
  TIM_SetCompare2(TIM3,0);  
}

void Run_Back(u16 pwmnum)
{
  Run_Direction=BACK;
  TIM_SetCompare1(TIM3,0); 
  TIM_SetCompare2(TIM3,pwmnum);  
}

void Run_Stop()
{

  Run_Direction=NO;
  TIM_SetCompare1(TIM3,0); 
  TIM_SetCompare2(TIM3,0);  
}

void Run_Brake()
{
  TIM_SetCompare1(TIM3,1200); 
  TIM_SetCompare2(TIM3,1200);  
}

u16 Pwm_Transfer(u8 pwm_data)
{
 return Pwm_Num[pwm_data];
}

static u8 Data_Run_Direction=NO;
static u8 Data_Run=0;
static u8 Data_Pre=0x00;

void Run_Moto(u8 data)
{
	if(data==Data_Pre)   return;
	Data_Pre=data;
  Data_Run_Direction=data>>4;
  Data_Run=data&0x0F;
  if(((Data_Run_Direction!=1)&&(Data_Run_Direction!=2))||(Data_Run>4))  
    {Run_Stop();return;}
	if((Run_Direction!=NO)&&(Data_Run_Direction!=Run_Direction))
	{Run_Brake();delay_ms(1);}
  if(Data_Run_Direction==FORWARD)                                 
    Run_Forward(Pwm_Transfer(Data_Run));
  else if(Data_Run_Direction==BACK)                                 
    Run_Back(Pwm_Transfer(Data_Run));
    
}

