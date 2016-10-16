#include "stm32f10x.h"
#include "motor.h"

u32 delay_time;
u16 speed = 4;

/********************************************
函数名:	 TIM2_NVIC_Configuration 
功能:		 TIM2定时器中断初始化
输入参数:	 无
返回参数:   无  
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
函数名:	 TIM2_Configuration 
功能:		 TIM2定时器初始化
输入参数:	 无
返回参数:   无
********************************************/
void TIM2_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
    TIM_DeInit(TIM2);
    TIM_TimeBaseStructure.TIM_Period=1;					
    TIM_TimeBaseStructure.TIM_Prescaler= 71;					//1ms刷新一次
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	TIM2_NVIC_Configuration();

	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE);		
    TIM_Cmd(TIM2, DISABLE);										//先关闭定时器													  
}

/********************************************
函数名:	 delay_nms
功能:		 定时器延时n毫秒
输入参数:	 延时时间
返回参数:   无
********************************************/
void delay_nms(u32 n)
{
    //1-打开定时器1,打开中断,设定定时时间
    TIM_Cmd(TIM2, ENABLE);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	delay_time=n;
	//2-延时等待
	while(delay_time>0);
    //3-关闭定时器，关闭中断
	TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE);
    TIM_Cmd(TIM2, DISABLE);		
}

/********************************************
函数名:	 MOTOR_GPIO_Configuration 
功能:		 四相步进电机IO口初始化
输入参数:	 无
返回参数:   无
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


	