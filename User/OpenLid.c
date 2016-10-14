#include "OpenLid.h"



void Servo_GPIO_Config(void)												 //���Ƕ��IO�ڳ�ʼ��
{
 	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		                     // �����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
} 

void Servo_TIM3_Config(u16 per,u16 psc)								     	 //���Ƕ����ʱ����ʼ��
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 

	TIM_TimeBaseStructure.TIM_Period = per;                                 //����ʱ����0������per����Ϊper�Σ�Ϊһ����ʱ����
    TIM_TimeBaseStructure.TIM_Prescaler = psc;	                            //����Ԥ��Ƶ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	            //����ʱ�ӷ�Ƶϵ��������Ƶ(�����ò���)
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;             //���ϼ���ģʽ
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	                    //����ΪPWMģʽ1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;               //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);	                            //ʹ��ͨ��1
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM3, ENABLE);			                            // ʹ��TIM3���ؼĴ���ARR
    TIM_Cmd(TIM3, ENABLE);                                                  //ʹ�ܶ�ʱ��3	
   
}

void OpenLid_Config(void)
{

	Servo_GPIO_Config();
	Servo_TIM3_Config(199,0);										    	//��ʱ��3��ʼ����pwmƵ��Ϊ50Hz �����ڿɸ�
	TIM_PrescalerConfig(TIM3,7200,TIM_PSCReloadMode_Immediate);				//�趨Ƶ��50Hz������ʱ20ms
	TIM_SetCompare1(TIM3,15);												//�����ʼ�Ƕ�Ϊ-90��
}
