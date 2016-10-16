#include "stm32f10x.h"
#include "motor.h"

u32 delay_time;
u16 speed = 4;

/********************************************
������:	 TIM2_NVIC_Configuration 
����:		 TIM2��ʱ���жϳ�ʼ��
�������:	 ��
���ز���:   ��  
********************************************/
void TIM2_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  													
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/********************************************
������:	 TIM2_Configuration 
����:		 TIM2��ʱ����ʼ��
�������:	 ��
���ز���:   ��
********************************************/
void TIM2_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
    TIM_DeInit(TIM2);
    TIM_TimeBaseStructure.TIM_Period=1;					
    TIM_TimeBaseStructure.TIM_Prescaler= 71;					//1msˢ��һ��
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	TIM2_NVIC_Configuration();

	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE);		
    TIM_Cmd(TIM2, DISABLE);										//�ȹرն�ʱ��													  
}

/********************************************
������:	 delay_nms
����:		 ��ʱ����ʱn����
�������:	 ��ʱʱ��
���ز���:   ��
********************************************/
void delay_nms(u32 n)
{
    //1-�򿪶�ʱ��1,���ж�,�趨��ʱʱ��
    TIM_Cmd(TIM2, ENABLE);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	delay_time=n;
	//2-��ʱ�ȴ�
	while(delay_time>0);
    //3-�رն�ʱ�����ر��ж�
	TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE);
    TIM_Cmd(TIM2, DISABLE);		
}

/********************************************
������:	 MOTOR_GPIO_Configuration 
����:		 ���ಽ�����IO�ڳ�ʼ��
�������:	 ��
���ز���:   ��
********************************************/
void MOTOR_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(StepMotor0_GPIO_CLK,ENABLE);
	GPIO_DeInit(StepMotor0_GPIO_PORT);
    GPIO_InitStructure.GPIO_Pin = StepMotor2_STP_GPIO_PIN | StepMotor2_DIR_GPIO_PIN | StepMotor2_EN_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(StepMotor0_GPIO_PORT , &GPIO_InitStructure);	
	
//	RCC_APB2PeriphClockCmd(StepMotor6_GPIO_CLK,ENABLE);
//	GPIO_DeInit(StepMotor6_GPIO_PORT);
//    GPIO_InitStructure.GPIO_Pin = StepMotor5_STP_GPIO_PIN | StepMotor5_DIR_GPIO_PIN | StepMotor5_EN_GPIO_PIN \
//															| StepMotor6_STP_GPIO_PIN | StepMotor6_DIR_GPIO_PIN | StepMotor6_EN_GPIO_PIN ;
	GPIO_Init(StepMotor6_GPIO_PORT , &GPIO_InitStructure);
    StepMotor2_EN(OFF);
}

void StepMotor(char direction,u32 step)
{
    StepMotor2_EN(ON);
    StepMotor2_DIR(direction);
    for(;step;step--)
    {
            GPIO_SetBits(StepMotor2_GPIO_PORT,StepMotor2_STP_GPIO_PIN	);
            delay_nms(speed);
            GPIO_ResetBits(StepMotor2_GPIO_PORT,StepMotor2_STP_GPIO_PIN	);
            delay_nms(speed);
    }
    StepMotor2_EN(OFF);
								
}
void StepMotor_Go(char motor_num,char direction)
{
		;
}


	