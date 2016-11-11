#include "stm32f10x.h"
#ifndef __PID_H
#define __PID_H

#define PWM_MAX 500
#define PWM_MIN 0

#define MICROSTEP 		MicrostepMode_Sixteenth		

#define Frequecy_To_Speed(x)	((x) << MICROSTEP)	

typedef struct 
{
	float Kp;		                  //��������
	float Ki;		                  //���ֳ���
	float Kd;		          		  //΢�ֳ���
	float Expect;		              //����ֵ
	float Current;		              //ʵ��ֵ
	float lError;		              //�ϴ����
	float pError;		              //��ǰ���
	float sError;		              //�����
	float dError;	                  //΢��
	float Error;			          //ƫ��
	float Pwm;		                  //PID���pwm
}PID;

typedef enum
{
  MicrostepMode_Full = 0x0,
  MicrostepMode_Half = 0x1,
  MicrostepMode_Quar = 0x2,
  MicrostepMode_Eighth = 0x3,
  MicrostepMode_Sixteenth = 0x4,
  MicrostepMode_ThirtySecond = 0x5
}MicrostepMode_TypeDef;

void PID_Init(void);
void TIM3_Configuration(void);
void PID_Cal(void);

#endif
