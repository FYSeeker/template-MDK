#ifndef __COMMUNICCATION_H
#define	__COMMUNICCATION_H

#include "stm32f0xx.h"
#include "system/delay/delay.h"

#define rx_buf_len 128	
//#define tx_buf_len 128
#define ElemType u8


#define MotoFlashHead  0x08003ccc//���flash�洢ͷ
#define MotoId  MotoFlashEEPROM[ID]
#define TorqueSwitchFlag  MotoFlashEEPROM[TorqueSwitch]
//�ڴ���Ʊ�EEPROM
//enum {
//VersionH      = 0x03,//����汾H
//VersionL      = 0x04,//����汾L
//MotoIdctp     = 0x05,//ID
//BaudRate      = 0x06,//������
//RtDelayTime   = 0x07,//������ʱ
//RTStaLVL      = 0x08,//Ӧ��״̬�ȼ�
//AngleMinH     = 0x09,//��С�Ƕ�����H
//AngleMinL     = 0x0A,//��С�Ƕ�����L
//AngleMaxH     = 0x0B,//���Ƕ�����H
//AngleMaxL     = 0x0C,//���Ƕ�����L
//TemperatureMax= 0x0D,//�¶�����
//VoltageMax    = 0x0E,//��ѹ����
//VoltageMin		= 0x0F,//��ѹ����
//TorqueMaxH    = 0x10,//���Ť��H
//TorqueMaxL    = 0x11,//���Ť��L
//SpeedReg			= 0x12,//�ٶȵ���
//Unloading			= 0x13,//ж������
//MedianRegH    = 0x14,//��λ����H
//MedianRegL    = 0x15,//��λ����L
//ElectricityH  = 0x16,//������ֵH
//ElectricityL  = 0x17,//������ֵL
//PWMoutMinH    = 0x18,//��СPWMH
//PWMoutMinL    = 0x19,//��СPWML
//ADCDenL       = 0x1A,//˳ʱ�벻������
//ADCDenR       = 0x1B,//��ʱ�벻������
//LEDAlarm      = 0x1C,//LED����
//TorqueDenAlarm= 0x1D,//����Ť�ؾ���
//};

//enum {
//TorqueSwitch     = 0x28,//Ť�ؿ���
//LEDSwitch        = 0x29,//LED����
//TargetIocationH  = 0x2A,//Ŀ��λ��H
//TargetIocationL  = 0x2B,//Ŀ��λ��L
//ElapsedTimeH     = 0x2C,//����ʱ��H
//ElapsedTimeL     = 0x2D,//����ʱ��L
//CurrentElectricH = 0x2E,//��ǰ����H
//CurrentElectricL = 0x2f,//��ǰ����L
//Shackles         = 0x30,//����־
//CurrentPositionH = 0x38,//��ǰλ��H
//CurrentPositionL = 0x39,//��ǰλ��L
//RunningSpeedH    = 0x3A,//�����ٶ�H
//RunningSpeedL    = 0x3B,//�����ٶ�L
//CurrentLoadH		 = 0x3C,//��ǰ����H
//CurrentLoadL     = 0x3D,//��ǰ����L
//CurrentVoltage   = 0x3E,//��ǰ��ѹ
//CurrentTemperature=0x3F,//��ǰ�¶�
//RegWRI           = 0x40,//REGwriFlag
//};

//Control Table of EEPROM Area
enum {
	Model_NumberL				= 0,
	Model_NumberH				= 1,
	Firmware_Version		= 2,
	ID									= 3,
	Baud_Rate						= 4,
	Return_Delay_Time		= 5,
	CW_Angle_LimitL			= 6,
	CW_Angle_LimitH			= 7,
	CCW_Angle_LimitL		= 8,
	CCW_Angle_LimitH		= 9,
	Temperature_Limit		= 11,
	Min_Voltage_Limit		= 12,
	Max_Voltage_Limit		= 13,
	Max_TorqueL					= 14,
	Max_TorqueH					= 15,
	Status_Return_Level	= 16,
	Alarm_LED						= 17,
	Shutdown						= 18,
	Multi_Turn_OffsetL	= 20,
	Multi_Turn_OffsetH  = 21,
	Resolution_Divider	= 22,
};
//Control Table of RAM Area
enum {
	Torque_Enable				= 24,
	LED									= 25,
	D_Gain							= 26,
	I_Gain							= 27,
	P_Gain							= 28,
	Goal_PositionL			= 30,
	Goal_PositionH			= 31,
	Moving_SpeedL				= 32,
	Moving_SpeedH				= 33,
	Torque_LimitL				= 34,
	Torque_LimitH				= 35,
	Present_PositionL		= 36,
	Present_PositionH		= 37,
	Present_SpeedL		  = 38,
	Present_SpeedH 		  = 39,
	Present_LoadL			  = 40,
	Present_LoadH				= 41,
	Present_Voltage			= 42,
	Present_Temperature	= 43,
	Registered					= 44,
	Moving							= 46,
	Lock								= 47,
	PunchL							= 48,
	PunchH							= 49,
	Realtime_TickL			= 50,
	Realtime_TickH			= 51,
	Goal_Acceleration		= 73,
	
	pressure_valueA_L			= 90,
	pressure_valueA_H			= 91,
	pressure_valueB_L			= 92,
	pressure_valueB_H			= 93,
	pressure_valueC_L			= 94,
	pressure_valueC_H			= 95,
	pressure_valueD_L			= 96,
	pressure_valueD_H			= 97,
};


enum {
PingCmd         = 0x01,//Ping
ReadCmd					= 0x02,//��ȡ���Ʊ�
WriteCmd        = 0x03,//д����Ʊ�
RegWRICmd				= 0X04,//�첽д
Action					= 0x05,//ִ���첽д
SyncWRCMD				= 0x83,//
BulkReadCMD     = 0X92,
};  



typedef struct {
    int         size;   /* maximum number of elements           */
    int         start;  /* index of oldest element              */
    int         end;    /* index at which to write new element  */
	  int         count;    /* index at which to write new element  */
    ElemType   *elems;  /* vector of elements                   */
} CircularBuffer;



//extern u8 uart1_cmd_rev;
//extern u8 uart1_cmd_buffer;


extern CircularBuffer rxbuf;
//extern CircularBuffer txbuf;
//extern CircularBuffer VOEbuff;//USART1���ڽ��ջ�����
//extern CircularBuffer VOE2buff;//USART2���ڽ��ջ�����

extern u8 rxbuf_st[rx_buf_len]; 
extern u8 txbuf_st[rx_buf_len];
//extern u8 VOEbuff_st[rx_buf_len];//�������ڻ���������
//extern u8 VOE2buff_st[rx_buf_len];//�������ڻ���������

void rx_buf_init(void);

//cb func
void cbInit(CircularBuffer *cb, int size,u8* state);
int cbIsFull(CircularBuffer *cb);
int cbIsEmpty(CircularBuffer *cb);
int cbCount(CircularBuffer *cb);
int cbIncr(CircularBuffer *cb, int p);
void cbWrite(CircularBuffer *cb, ElemType *elem);
void cbRead(CircularBuffer *cb, ElemType *elem);

void txrx_buf_init(void);

#endif
