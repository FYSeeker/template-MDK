## 下位机主板主要功能
下述`DXL`协议指`Dynamixel`的[Protocol 1.0](http://emanual.robotis.com/docs/en/dxl/protocol1/)协议。
- 给机器人和外接传感器供电
- 实现`DXL`协议，管理机器人舵机
- 实现板载`DXL`协议、舵机`DXL`协议、传感器`DXL`协议与上位机串口之间的数据交互
- 获取板载传感器数据
#### 供电
下位机主板使用12V直流电源供电，板载五个`DXL`协议接口，可外挂至少253个设备，接口包括通信和供电功能。接口的供电功能可通过程序控制开关，使用`DXL`协议控制的相关代码如下。
```
void DXL_SetPower(uint8_t status)
{
  if(status == 1)
  {
    DXL_POWER_H();
    DXL.power = 1;
  }
  else
  {
    DXL_POWER_L();
    DXL.power = 0;
  }
}
```
上位机通过设置地址24的值开启和关闭下位机`DXL`接口电源。
```
#define	P_DYNAMIXEL_POWER               24
#define DATA_DYNAMIXEL_POWER            gbpControlTable[P_DYNAMIXEL_POWER]
DXL_SetPower(DATA_DYNAMIXEL_POWER);
```
#### 和上位机串口之间的数据交互
下位机主板与舵机、外挂传感器在`DXL`协议上是平等的，都属于总线上的一个设备。但下位机主板有一个重要功能，就是将`DXL`协议上的单总线半双工通信，和上位机串口双线全双工通信之间进行数据转换。
下位机主板接收到上位机发送数据后会直接转发到`DXL`总线上，如果数据包和自己有关则会执行相关处理。收到`DXL`总线上的数据后也会直接转发给上位机，如果数据包和自己有关则会执行相关处理。

---
## 下位机板载传感器定义
下位机会周期性的获取板载传感器的数据，存放到`DXL`协议使用的数组寄存器中；设置数组寄存器中相应位置也会控制板载执行器。获取传感器数据由`sensor.c`中的`SensorGet()`函数实现，控制板载执行器由`sensor.c`中的`SensorSet()`函数实现。数组寄存器相关代码如下：
```
#define ROM_CONTROL_TABLE_LEN   24
#define RAM_CONTROL_TABLE_LEN   56
#define CONTROL_TABLE_LEN       (ROM_CONTROL_TABLE_LEN+RAM_CONTROL_TABLE_LEN)

volatile uint8_t gbpControlTable[CONTROL_TABLE_LEN+1];
```
#### 姿态传感器
姿态传感器包括陀螺仪和地磁，总共九轴`uint16_t`类型的原始数据。相关代码如下：
```
#define	P_GYRO_X                38
//#define   -   39
#define	P_GYRO_Y                40
//#define   -   41
#define	P_GYRO_Z                42
//#define   -   43
#define	P_ACC_X                 44
//#define   -   45
#define	P_ACC_Y                 46
//#define   -   47
#define	P_ACC_Z                 48
//#define   -   49
#define	P_PRESENT_VOLTAGE       50
#define	P_LEFT_MIC              51
//#define   -   52
#define	P_ADC_CH2               53
//#define   -   54
#define	P_ADC_CH3               55
//#define   -   56
```
```
#define DATA_GYRO_X                   HALFWORD_CAST(gbpControlTable[P_GYRO_X])
#define DATA_GYRO_Y                   HALFWORD_CAST(gbpControlTable[P_GYRO_Y])
#define DATA_GYRO_Z                   HALFWORD_CAST(gbpControlTable[P_GYRO_Z])
#define DATA_ACCEL_X                  HALFWORD_CAST(gbpControlTable[P_ACC_X])
#define DATA_ACCEL_Y                  HALFWORD_CAST(gbpControlTable[P_ACC_Y])
#define DATA_ACCEL_Z                  HALFWORD_CAST(gbpControlTable[P_ACC_Z])
#define DATA_MAGNETIC_X               HALFWORD_CAST(gbpControlTable[P_LEFT_MIC])
#define DATA_MAGNETIC_Y               HALFWORD_CAST(gbpControlTable[P_ADC_CH2])
#define DATA_MAGNETIC_Z               HALFWORD_CAST(gbpControlTable[P_ADC_CH3])
```
```
void SensorGetImu()
{
  ImuData_t imuData = {0};
  
  IMU_GetData(&imuData);
  
  DATA_GYRO_X = imuData.gray.x;
  DATA_GYRO_Y = imuData.gray.y;
  DATA_GYRO_Z = imuData.gray.z;
  DATA_ACCEL_X = imuData.accel.x;
  DATA_ACCEL_Y = imuData.accel.y;
  DATA_ACCEL_Z = imuData.accel.z;
  DATA_MAGNETIC_X = imuData.magnetic.x;
  DATA_MAGNETIC_Y = imuData.magnetic.y;
  DATA_MAGNETIC_Z = imuData.magnetic.z;
}
```
#### RGB灯
rgb灯由数据寄存器的两个8位组成的16位数控制，由低到高每3位控制一个灯，总共使用15位控制5个灯。对于单个灯，3位中的第1位为0时红灯灭，3位中的第1位为1时红灯亮；3位中的第2位为0时绿灯灭，3位中的第2位为1时绿灯亮；3位中的第3位为0时蓝灯灭，3位中的第3位为1时蓝灯亮（001：红灯亮,111:全亮）。相关代码如下：
```
#define	P_LED_HEAD                      26
//#define   -   27
```
```
#define DATA_RGB_STATUS                 HALFWORD_CAST(gbpControlTable[P_LED_HEAD])
```
#### 按键
下位机板载5个用户按键，程序会周期性更新按键状态到数组寄存器，相关代码如下：
```
#define	P_BUTTON                        30
//#define   -   31
//#define   -   32
//#define   -   33
//#define   -   34
```
```
#define DATA_KEY1_STATUS                gbpControlTable[P_BUTTON]
#define DATA_KEY2_STATUS                gbpControlTable[P_BUTTON+1]
#define DATA_KEY3_STATUS                gbpControlTable[P_BUTTON+2]
#define DATA_KEY4_STATUS                gbpControlTable[P_BUTTON+3]
#define DATA_KEY5_STATUS                gbpControlTable[P_BUTTON+4]
```
```
void SensorGetKey()
{
  uint8_t keyStatus = 0;
  
  keyStatus = KeyStatusAllGet();
  
  DATA_KEY1_STATUS = (keyStatus>>0)&(0x01);
  DATA_KEY2_STATUS = (keyStatus>>1)&(0x01);
  DATA_KEY3_STATUS = (keyStatus>>2)&(0x01);
  DATA_KEY4_STATUS = (keyStatus>>3)&(0x01);
  DATA_KEY5_STATUS = (keyStatus>>4)&(0x01);
}
```
