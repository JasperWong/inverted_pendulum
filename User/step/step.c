#include "step.h"

u16 speed_step=4;
void Step_GPIO_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(StepMotor_GPIO_CLK,ENABLE);
    GPIO_InitStructure.GPIO_Pin = StepMotor_DIR_GPIO_PIN|StepMotor_EN_GPIO_PIN|StepMotor_STP_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(StepMotor_GPIO_PORT , &GPIO_InitStructure);	
}

void StepByPwm(u8 direction,u32 step)
{
    StepMotor_EN(ON);
    StepMotor_DIR(direction);
//    TIM_Cmd(TIM4, ENABLE); 
    TIM4_PWM_Init(1,1);
//    StepMotor_EN(OFF); 
}

void StepStop()
{
    TIM_Cmd(TIM4, DISABLE); 
    StepMotor_EN(OFF);
}
static void TIM4_GPIO_Config(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* ����TIM3CLK Ϊ 72MHZ */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE); 

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // �����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/*PB8 PWM*/
void TIM4_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//ʹ�ܶ�ʱ��3ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
 
   //���ø�����Ϊ�����������,���TIM4 CH3��PWM���岨��	GPIOB.8
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //TIM_CH3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
 
   //��ʼ��TIM4
//	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
//	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
    TIM_TimeBaseStructure.TIM_Period = speed_step-1; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =71; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
//    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_OCInitStructure.TIM_Pulse = (speed_step+1)/2;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM4 Channel3 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM4 OC3

	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM4��CCR3�ϵ�Ԥװ�ؼĴ���
	TIM_ARRPreloadConfig(TIM4, DISABLE);
	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM4
}

void Step_Init(u16 speed)
{
    TIM4_GPIO_Config();
    TIM4_PWM_Init(1,2);
}