#ifndef STEPPER_MOTOR_CONTROL_H
#define STEPPER_MOTOR_CONTROL_H

#include "sys.h"
#include "stm32f10x_tim.h"

//-----------------Motor Freq Limit--------------//
#define MAX_OUTPUT		400
#define MIN_OUTPUT		0
// #define FQ				36		//启动频率/突跳频率 
// #define FQ				10		//启动频率/突跳频率 
#define FQ				10		//启动频率/突跳频率 

// #define WAITONESTEP 	4		// 每15Hz频率等1格步长（编码器）
#define WAITONESTEP 	50		// 每15Hz频率等1格步长（编码器）
//---------------Motor Direct Control------------//
#define MOTOR_CLOCKWISE			1
#define MOTOR_CONTRAROTATE		0
//-----------------Motor Control Pin--------------//
#define DIR PCout(12)	// 方向控制
//#define STP PBout(8)	// 直连输出口
#define EN	PDout(3)	// 低电平使能
//-----------------Step Mode Pin----------------//
#define Mode0 PAout(1)
#define Mode1 PAout(3)
#define Mode2 PAout(5)

#define MICROSTEP 		MicrostepMode_Sixteenth		// 采用16细分
#define MICROFREQ 		( 1/(1<<MICROSTEP) )		// 细分分频系数
//-----------------DDS As Controller--------------//
//#define FWORD_RESOLUTION		4							// FPGA的ROM表最小分辨率，代表频率控制字1单位改变的频率，单位为Hz
//#define Frequecy_To_Fword(x)	( ((x) << MICROSTEP) >> 2 )	//	用于将频率转换成频率控制字，与 FWORD_RESOLUTION 和 MICROSTEP 相关
//#define Frequecy_To_Fword(x)	( (x << MICROSTEP) >> 2 )	//	用于将频率转换成频率控制字，与 FWORD_RESOLUTION 和 MICROSTEP 相关
//------------------------------------------------//
#define Frequecy_To_Speed(x)	((x) << MICROSTEP)	//	用于将频率转换成频率控制字，与 FWORD_RESOLUTION 和 MICROSTEP 相关
//#define Frequecy_To_Speed(x)	(x)

// 无细分下：
// 1Hz = 0.3rpm
// 816Hz优良上限 288Hz优良底限

// 细分
typedef enum
{
  MicrostepMode_Full = 0x0,
  MicrostepMode_Half = 0x1,
  MicrostepMode_Quar = 0x2,
  MicrostepMode_Eighth = 0x3,
  MicrostepMode_Sixteenth = 0x4,
  MicrostepMode_ThirtySecond = 0x5
}MicrostepMode_TypeDef;

unsigned int MotorGetCurrentSpeed(void);
unsigned int MotorGetCurrentDirect(void);
void MotorUpdateSpeed(unsigned int current);
void MotorUpdateDirect(unsigned int current);
void StepperMotor_Init( MicrostepMode_TypeDef mode );
void MotorFrequecyOut( unsigned int freq );
void MicrostepMode_Set( MicrostepMode_TypeDef mode );
void MotorControl( unsigned int Fout, unsigned int Dir );
void MotorSpeedUp( unsigned int current, unsigned int destinct );
void MotorSpeedDown( unsigned int current, unsigned int destinct );
void MotorWaitForResponse( unsigned int delta_speed );
void MotorStop(void);

void MotorControl_2( unsigned int Speed, unsigned int Dir );
void MotorSpeedUp_3( unsigned int current, unsigned int destinct );

#endif
