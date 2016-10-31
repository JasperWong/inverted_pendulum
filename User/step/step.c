#include "step.h"
#include "PID.h"

u16 speed_step=4;

float MotorAngleCurrent;
float PoleAngleCurrent;
void Step_GPIO_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(StepMotor_GPIO_CLK,ENABLE);
    GPIO_InitStructure.GPIO_Pin = StepMotor_DIR_GPIO_PIN|StepMotor_EN_GPIO_PIN|StepMotor_STP_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(StepMotor_GPIO_PORT , &GPIO_InitStructure);	
}

void StepByPwm(u8 direction)
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
    delay_ms(300);
    StepMotor_EN(OFF);
}

void StepAngle(u8 direction,float angle)
{
    static float angleTemp=0;
    MotorAngleCurrent=Encoder_GetAngle(1);
    
    if(direction==FORE)
    {
        StepByPwm(FORE);
        while(angleTemp<=angle)
        {
            angleTemp=Encoder_GetAngle(1)-MotorAngleCurrent;
            if(angleTemp<0)
            {
                angleTemp=-angleTemp;
            }
            printf("motor:%f",Encoder_GetAngle(1)); 
            
            printf("\n");  
        }
        StepStop();
        printf("motor:%f",Encoder_GetAngle(1)); 
        printf("\n");
    }        
       
}
static void TIM4_GPIO_Config(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* 设置TIM3CLK 为 72MHZ */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE); 

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // 复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/*PB8 PWM*/
void TIM4_PWM_Init(u16 arr,u16 psc)
{  

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

   //初始化TIM4
	TIM_TimeBaseStructure.TIM_Period = arr; 											//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 											//设置用来作为TIMx时钟频率除数的预分频值 
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;										
	TIM_OCInitStructure.TIM_Pulse = arr/2;												//占空比 50%
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 										//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  						//TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 									//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM4 Channel3 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 									//选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;						 //比较输出使能
		
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 							//输出极性:TIM输出比较极性高
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  											//根据T指定的参数初始化外设TIM4 OC3

	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  									//使能TIM4在CCR3上的预装载寄存器
	TIM_ARRPreloadConfig(TIM4, ENABLE);													//使能重装载计时器
	TIM_Cmd(TIM4, ENABLE);  															//使能TIM4

}

#define ACCURACY 1
double newSqrt(double n)
{
	double low, high, mid, tmp;
	if (n > 1)
	{
		low = 1;
		high = n;
	} 
	else 
	{
		low = n;
		high = 1;
	}

	while (low <= high) 
	{
		mid = (low + high) / 2.000;
		tmp = mid * mid;
		if (tmp - n <= ACCURACY && tmp -n >= ACCURACY * -1) 
		{
			return mid;
		} 
		else if (tmp > n) 
		{
			high = mid;
		} 
		else 
		{
			low = mid;
		}
	}
	return -1.000;
}

void MotorFrequecyOut( unsigned int freq ,u8 dir)
{
	u32 temp;
	u16 autoload;
	u16 psc;
	double value;
	
	StepMotor_EN(ON);
    StepMotor_DIR(dir);
	
	if( freq > PWM_MAX )
	{
		TIM_PrescalerConfig(TIM4, 71, TIM_PSCReloadMode_Immediate);
		TIM_SetAutoreload(TIM4, 80);
		TIM_SetCompare3(TIM4, 40);
		TIM_Cmd(TIM4, ENABLE);
	}
	else if( freq < PWM_MIN )
	{
		
//		TIM_PrescalerConfig(TIM4, 71, TIM_PSCReloadMode_Immediate);
//		TIM_SetAutoreload(TIM4, 200);
//		TIM_SetCompare3(TIM4, 100);
//		TIM_Cmd(TIM4, ENABLE);
		
		TIM_Cmd(TIM4, DISABLE);
	}
	else
	{
		temp = (72000000/Frequecy_To_Speed(freq));   
		value = newSqrt(temp);
		if(2*value > 2*((unsigned int)value))
		{
			psc = (unsigned int)value;
			autoload = (unsigned int)value+1;
		}
		else
		{
			psc = (unsigned int)value;
			autoload = (unsigned int)value;
		}
		TIM_PrescalerConfig(TIM4, psc, TIM_PSCReloadMode_Immediate);
		TIM_SetAutoreload(TIM4, autoload);
		TIM_SetCompare3(TIM4, autoload/2);
		TIM_Cmd(TIM4, ENABLE);
//		if(disable_flag)
//		{
//			TIM_Cmd(TIM4, ENABLE);
//			disable_flag = 0;
//		}
	}
//	printf(" arr %d psc %d  auto%d\r\n\r\n",freq,psc,autoload);
//	printf("%d ", Frequecy_To_Speed(freq));
}

//void MotorFrequecyOut( unsigned int freq ,u8 dir)
//{
//	u32 temp;
//	u16 autoload;
//	u16 psc;
//	double value;
//	
//    StepMotor_DIR(dir);
//	
//	if( freq > PWM_MAX )
//	{
//		TIM_PrescalerConfig(TIM4, 71, TIM_PSCReloadMode_Immediate);
//		TIM_SetAutoreload(TIM4, 1000);
//		TIM_SetCompare3(TIM4, 500);

//	}
//	else if( freq < PWM_MIN )
//	{
//		TIM_Cmd(TIM4, DISABLE);
//	}
//	else
//	{
//		TIM_PrescalerConfig(TIM4, 71, TIM_PSCReloadMode_Immediate);
//		TIM_SetAutoreload(TIM4, freq);
//		TIM_SetCompare3(TIM4, freq/2);
//		TIM_Cmd(TIM4, ENABLE);
//	}

//}

void Step_Init()
{
    TIM4_GPIO_Config();
    TIM4_PWM_Init(1,71);
}