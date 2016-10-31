#include "stm32f10x.h"
#include "PID.h"
#include "Encoder.h"
#include "step.h"
#include "usart.h"

PID PID_Controller_angle;
PID PID_Controller_velocity;

PID *APID = &PID_Controller_angle;				// ������ PID
PID *VPID = &PID_Controller_velocity;				// �ٶȻ� PID

/*
 * TIM_Period / Auto Reload Register(ARR) = 1000   TIM_Prescaler--71 
 * �ж�����Ϊ = 1/(72MHZ /72) * 5000 = 5ms
 *
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIM_Period(ARR) --> �ж� ��TIMxCNT����Ϊ0���¼��� 
 */
void TIM3_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = 5000; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler = 71; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
// 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

//	//�ж����ȼ�NVIC����
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
	TIM_Cmd(TIM3, DISABLE);  //ʹ��TIMx																		
}

void PID_Init(void)
{
	APID->Kp = 4.7;
	APID->Ki = 0.0850;
	APID->Kd = 19.9;
	APID->Expect = 512;
} 

float Abs( float value )
{
	if( value < 0.0 )
	{
		value *= -1;
	}
	return value;
}

//void TIM3_IRQHandler(void)
//{
//	u8 dir;
//	static u8 i;
//	static int arr;
//	static float pSpeed;
//	if ( TIM_GetITStatus(TIM3 , TIM_IT_Update) != RESET ) 
//	{
//		
//		APID->Current = Encoder_GetData(0);					    						//��ȡ�ڸ�����  ������
//		
//		APID->Error = APID->Expect - APID->Current;			    						//ƫ��
//		if(APID->Error > 0) dir = FORE;
//			else dir = BACK;
////		if(APID->Error >= -1.0 && APID->Error <= 1.0) APID->Error = 0;
//		APID->sError += APID->Error;													//����
//		if(APID->Error > 170 || APID->Error < -170)										//���ڵ�����120������PID
//		{
//			APID->Error = 0;
//			APID->sError = 0;
//			APID->dError = 0;
//		}
//		APID->dError = APID->Error - APID->lError;										//΢��
//		APID->lError = APID->Error;																			
//		
//		APID->Pwm += APID->Kp * APID->Error + APID->Ki * APID->sError + APID->Kd * APID->dError;
//		if(i++ >= 5)																						
//		{
//			APID->Pwm /= 6;
//			arr = (int)APID->Pwm;
//			APID->Pwm = 0;
//			i = 0;
//			
//			if(arr < 0) arr *= -1;
//			
////			if(arr > 0)																	//ȷ���������
////			{
////				dir = FORE;
////			}
////			else
////			{
////				dir = BACK;
////				arr *= -1;
////			}
//			
//			MotorFrequecyOut(arr,dir);  												//λ��ʽPID���

//		}
//				
//		printf("%d  %f  %f  %f  %f\r\n",arr,APID->Error,APID->sError,APID->dError,APID->Pwm);
//	    TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Update); //TIM_FLAG_Update
//	} 
//}

void TIM3_IRQHandler(void)
{
	u8 dir;
	static u8 dead_flag;
	static int arr;

	if ( TIM_GetITStatus(TIM3 , TIM_IT_Update) != RESET ) 
	{
		APID->Current = Encoder_GetData(0);					    						//��ȡ�ڸ�����  ������
		
		if(APID->Current > 390 && APID->Current < 634)
		{
//			printf("sumError %f",APID->sError);
//			printf("add 1,0,%d",(int)(APID->Current*255/1024));
//			Usart_SendByte(USART1, 0xFF);
//			Usart_SendByte(USART1, 0xFF);
//			Usart_SendByte(USART1, 0xFF);
			
			APID->Error = APID->Expect - APID->Current;			    						//ƫ��
			if(APID->Error > 0) dir = FORE;
				else dir = BACK;
//			if(APID->Error >= -1.0 && APID->Error <= 1.0) APID->Error = 0;
			APID->sError += APID->Error;													//����
			
			if(APID->sError > 2050)  APID->sError = 2050;									//�����޷�
			if(APID->sError < -2050) APID->sError = -2050;
			
			if(APID->Error > 0 && APID->sError < 0) APID->sError = 0;
			if(APID->Error < 0 && APID->sError > 0) APID->sError = 0;
			
			APID->dError = APID->Error - APID->lError;										//΢��
			APID->lError = APID->Error;																			
			
			APID->Pwm = APID->Kp * APID->Error + APID->Ki * APID->sError + APID->Kd * APID->dError;
	
			arr = (int)APID->Pwm;
	
//			if(arr > 0) dir = FORE;
//			if(arr < 0) {arr *= -1; dir = BACK;}
			if(arr < 0) arr *= -1;
			
			if(APID->Current > (APID->Expect -1) && APID->Current < (APID->Expect +1)) 
			{
				arr=arr*3/5;
				dead_flag = 1;
//				APID->sError = 0;
//				return;
			}
			else
			{
				if(dead_flag)
				{
					dead_flag = 0;
					arr = arr*4/5;
				}
			}
//			printf("%d\r\n",arr);
			MotorFrequecyOut(arr,dir);  												//λ��ʽPID���
			
		}
		else																			//����PID����
		{			
			
			APID->Error = 0;
			APID->sError = 0;
			APID->dError = 0;
			TIM_Cmd(TIM4, DISABLE);
			StepMotor_EN(OFF);
			
		}
	    TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Update); //TIM_FLAG_Update
	} 
}

//void PID_Cal(void)
//{
//	u8 dir;
//	static int arr;
//	static float pSpeed;
//	
//	APID->Current = Encoder_GetData(0);					    						//��ȡ�ڸ�����  ������
//	if(APID->Current > 341 && APID->Current < 682)
//	{
//		
//		APID->Error = APID->Expect - APID->Current;			    						//ƫ��
//		if(APID->Error > 0) dir = FORE;
//			else dir = BACK;
////		if(APID->Error >= -1.0 && APID->Error <= 1.0) APID->Error = 0;
//		APID->sError += APID->Error;													//����
//		
//		APID->dError = APID->Error - APID->lError;										//΢��
//		APID->lError = APID->Error;																			
//		
//		APID->Pwm  ced = APID->Kp * APID->Error + APID->Ki * APID->sError + APID->Kd * APID->dError;

//		arr = (int)APID->Pwm;

//		if(arr < 0) arr *= -1;
//		
//		MotorFrequecyOut(arr,dir);  												//λ��ʽPID���
//	
//		printf("%d  %f  %f  %f  %f\r\n",arr,APID->Error,APID->sError,APID->dError,APID->Pwm);
//	}
//	else																			//����PID����
//	{									
//		APID->Error = 0;
//		APID->sError = 0;
//		APID->dError = 0;
//		StepMotor_EN(OFF);
//	}
//}
	