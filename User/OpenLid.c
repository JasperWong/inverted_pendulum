#include "OpenLid.h"



void Servo_GPIO_Config(void)												 //开盖舵机IO口初始化
{
 	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		                     // 复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
} 

void Servo_TIM3_Config(u16 per,u16 psc)								     	 //开盖舵机定时器初始化
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 

	TIM_TimeBaseStructure.TIM_Period = per;                                 //当定时器从0计数到per，即为per次，为一个定时周期
    TIM_TimeBaseStructure.TIM_Prescaler = psc;	                            //设置预分频
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	            //设置时钟分频系数：不分频(这里用不到)
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;             //向上计数模式
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	                    //配置为PWM模式1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;               //当定时器计数值小于CCR1_Val时为高电平
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);	                            //使能通道1
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM3, ENABLE);			                            // 使能TIM3重载寄存器ARR
    TIM_Cmd(TIM3, ENABLE);                                                  //使能定时器3	
   
}

void OpenLid_Config(void)
{

	Servo_GPIO_Config();
	Servo_TIM3_Config(199,0);										    	//定时器3初始化，pwm频率为50Hz ，周期可改
	TIM_PrescalerConfig(TIM3,7200,TIM_PSCReloadMode_Immediate);				//设定频率50Hz，即定时20ms
	TIM_SetCompare1(TIM3,15);												//舵机初始角度为-90度
}
