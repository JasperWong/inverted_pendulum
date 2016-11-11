#include "stm32f10x.h"
#include "PID.h"
#include "Encoder.h"
#include "step.h"
#include "usart.h"

extern uint8_t SendBuff[SENDBUFF_SIZE];

PID PID_Controller_angle;
PID PID_Controller_velocity;

PID *APID = &PID_Controller_angle;				// ������ PID
PID *VPID = &PID_Controller_velocity;				// �ٶȻ� PID

float Kp = 2.1;   
float Ki = 0.534;
float Kd = 0;

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

//void PID_Init(void)
//{
//	APID->Kp = 9.6;
//	APID->Ki = 0.04;
//	APID->Kd = 0;
//	APID->Expect = 512;
//} 

void PID_Init(void)
{
	APID->Kp = Kp;
	APID->Ki = Ki;
	APID->Kd = Kd;
	APID->Expect = 510;
} 

float Abs( float value )
{
	if( value < 0.0 )
	{
		value *= -1;
	}
	return value;
}

void TIM3_IRQHandler(void)
{
	u8 dir;
	static u8 dead_flag;
	static int arr;
	u16 angle;

	if ( TIM_GetITStatus(TIM3 , TIM_IT_Update) != RESET ) 
	{
		PID_Init();
		APID->Current = Encoder_GetData(0);					    						//��ȡ�ڸ�����  ������
		angle = (int)APID->Current / 5;
		if(APID->Current > 410 && APID->Current < 614)
		{
			
			SendBuff[8] = (angle / 100) + '0';
			angle %= 100;
			SendBuff[9] = (angle / 10) + '0';
			SendBuff[10] = (angle % 10) +'0';
			
			SendBuff[21] = (uint8_t)Kp + '0';
			SendBuff[22] = ((uint8_t)((Kp*10)) % 10) + '0';
			SendBuff[23] = ((uint8_t)((Kp*100)) % 10) + '0';
			
			SendBuff[34] = (uint8_t)Ki + '0';
			SendBuff[35] = ((uint8_t)(Ki*10)) % 10 + '0';
			SendBuff[36] = ((uint8_t)(Ki*100)) % 10 + '0';
			
			SendBuff[47] = (uint8_t)Kd + '0';
			SendBuff[48] = ((uint8_t)(Kd*10)) % 10 + '0';
			SendBuff[49] = ((uint8_t)(Kd*100)) % 10 + '0';
			
			USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
			
			APID->Error = APID->Expect - APID->Current;			    						//ƫ��
			
			if(APID->Error > 0) dir = FORE;
				else dir = BACK;
//			if(APID->Error >= -1.0 && APID->Error <= 1.0) APID->Error = 0;
			APID->sError += APID->Error;													//����
			
//			if(APID->sError > 512)  APID->sError = 512;									//�����޷�
//			if(APID->sError < -512) APID->sError = -512;
			
//			if(APID->Error > 0 && APID->sError < 0) APID->sError = 0;
//			if(APID->Error < 0 && APID->sError > 0) APID->sError = 0;
			
			APID->dError = APID->Error - APID->lError;										//΢��
			APID->lError = APID->Error;																			
			
			APID->Pwm = APID->Kp * APID->Error + APID->Ki * APID->sError + APID->Kd * APID->dError;				
	
			arr = (int)APID->Pwm;

			if(arr < 0) arr *= -1;
			
			if((APID->Current > (APID->Expect - 1)) && (APID->Current < (APID->Expect + 1))) 
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
			USART_DMACmd(USART1, USART_DMAReq_Tx, DISABLE);  
		}
	    TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Update); //TIM_FLAG_Update
	} 
}
	