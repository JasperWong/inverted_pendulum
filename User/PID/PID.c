#include "stm32f10x.h"
#include "PID.h"
#include "Encoder.h"
#include "step.h"
#include "usart.h"

extern uint8_t SendBuff[SENDBUFF_SIZE];

PID PID_Controller_angle;
PID PID_Controller_velocity;

PID *APID = &PID_Controller_angle;				// 倒立环 PID
PID *VPID = &PID_Controller_velocity;				// 速度环 PID

float Kp = 2.1;   
float Ki = 0.534;
float Kd = 0;

/*
 * TIM_Period / Auto Reload Register(ARR) = 1000   TIM_Prescaler--71 
 * 中断周期为 = 1/(72MHZ /72) * 5000 = 5ms
 *
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIM_Period(ARR) --> 中断 且TIMxCNT重置为0重新计数 
 */
void TIM3_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = 5000; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler = 71; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
// 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

//	//中断优先级NVIC设置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
	TIM_Cmd(TIM3, DISABLE);  //使能TIMx																		
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
		APID->Current = Encoder_GetData(0);					    						//获取摆杆数据  经处理
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
			
			APID->Error = APID->Expect - APID->Current;			    						//偏差
			
			if(APID->Error > 0) dir = FORE;
				else dir = BACK;
//			if(APID->Error >= -1.0 && APID->Error <= 1.0) APID->Error = 0;
			APID->sError += APID->Error;													//积分
			
//			if(APID->sError > 512)  APID->sError = 512;									//积分限幅
//			if(APID->sError < -512) APID->sError = -512;
			
//			if(APID->Error > 0 && APID->sError < 0) APID->sError = 0;
//			if(APID->Error < 0 && APID->sError > 0) APID->sError = 0;
			
			APID->dError = APID->Error - APID->lError;										//微分
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
			MotorFrequecyOut(arr,dir);  												//位置式PID输出
			
		}
		else																			//重置PID参数
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
	