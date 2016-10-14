/*********************************************************************************************
程序名：			步进电机驱动源文件
编写人：			耀祖
编写时间：			2015年6月21日
硬件支持：		MCU:STM32F103
接口说明：　	脉冲频率输出使用PWM，步数检测使用了Encoder的库
修改日志：		　
　　NO.1-							
*********************************************************************************************
说明：里面的 Speed 变量都是指 脉冲频率
*********************************************************************************************/
#include "stepper_motor_control.h"
#include "usart.h"
//#include "Encoder.h"
#include "delay.h"
#include "timer.h"

// S型加速曲线调整
// #define INCREASE_FACT	10000		// 加速常数，用于确定S型变加速的时间段长短，越高则加速度越小，变加速过程越小
// #define VARIES_ACCE		5 		// 变加速量，为S型变加速的值，越大则加速变化越快
// #define ACCE_FACTION	2			// 加速因子，软件将变加速量分开几份，真正变加速值为 VARIES_ACCE/ACCE_FACTION

// #define WAITONESTEP 	1		// 每15Hz频率等1格步长（编码器）

//-------------------------临界不丢步------------------------//
// #define INCREASE_FACT	7		// 加速常数，用于确定S型变加速的时间段长短，越高则加速度越小，变加速过程越小
// #define VARIES_ACCE		20 		// 变加速量，为S型变加速的值，越大则加速变化越快
// #define ACCE_FACTION	1			// 加速因子，软件将变加速量分开几份，真正变加速值为 VARIES_ACCE/ACCE_FACTION

#define INCREASE_FACT	7		// 加速常数，用于确定S型变加速的时间段长短，越高则加速度越小，变加速过程越小
#define VARIES_ACCE		10 		// 变加速量，为S型变加速的值，越大则加速变化越快
#define ACCE_FACTION	1			// 加速因子，软件将变加速量分开几份，真正变加速值为 VARIES_ACCE/ACCE_FACTION

static unsigned int Current_Speed;
static unsigned int Current_Direct;

void StepperMotor_Init( MicrostepMode_TypeDef mode )
{
    //-----------------Motor Control Pin--------------//
	// 初始化结构体 GPIO_InitStructure
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	//------------------------------------------------//
	
	//-----------------Step Mode Pin----------------//
	// 设置GPIOA0、PA2、PA4为推挽输出，最大输出频率为50MHz
	// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_5;
	// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	// GPIO_Init(GPIOA, &GPIO_InitStructure);
	//----------------------------------------------//
	
	// MicrostepMode_Set(mode);
	
	EN = 0;
	
//	FPGA_74595_Init();
	TIM4_PWM_Init(7199, 10000);
	
	printf("Stepper Motor init complete!\n");
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

// 细分设置
void MicrostepMode_Set( MicrostepMode_TypeDef mode )
{
	Mode0 = mode&0x1;
	Mode1 = mode&0x2 >> 1;
	Mode2 = mode&0x4 >> 2;
}

// 获取当前电机速度
unsigned int MotorGetCurrentSpeed(void)
{
	return Current_Speed;
}

// 获取当前电机方向
unsigned int MotorGetCurrentDirect(void)
{
	return Current_Direct;
}

// 更新当前电机速度
void MotorUpdateSpeed(unsigned int current)
{
	Current_Speed = current;
}

// 更新当前电机方向
void MotorUpdateDirect(unsigned int current)
{
	Current_Direct = current;
}

static unsigned char disable_flag = 1;
// 输出控制脉冲频率
void MotorFrequecyOut( unsigned int freq )
{
	u32 temp;
	u16 autoload;
	u16 psc;
	double value;
	
	if( MAX_OUTPUT-1 <= freq )
	{
		TIM_PrescalerConfig(TIM4, 50, TIM_PSCReloadMode_Immediate);
		TIM_SetAutoreload(TIM4, 225);
		TIM_SetCompare3(TIM4, 112);
		if(disable_flag)
		{
			TIM_Cmd(TIM4, ENABLE);
			disable_flag = 0;
		}
	}
	else if( freq < MIN_OUTPUT )
	{
		TIM_Cmd(TIM4, DISABLE);
//		TIM_Cmd(TIM5, DISABLE);
		disable_flag = 1;
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
		if(disable_flag)
		{
			TIM_Cmd(TIM4, ENABLE);
			disable_flag = 0;
		}
	}
//	printf("%d ", Frequecy_To_Speed(freq));
}

// 电机控制
void MotorControl( unsigned int Speed, unsigned int Dir )
{
	unsigned int current;
	unsigned int current_dir;
	current = MotorGetCurrentSpeed();
	// current_dir = MotorGetCurrentDirect();
	DIR = Dir;
	if( current < Speed )
	{
		MotorSpeedUp( current, Speed );
	}
	else if( Speed < current )
	{
		MotorSpeedDown( current, Speed );
	}
	MotorFrequecyOut( Speed );
	MotorUpdateSpeed( Speed );
	MotorUpdateDirect( Dir );
}

void MotorControl_2( unsigned int Speed, unsigned int Dir )
{
	unsigned int current;
	unsigned int current_dir;
	current = MotorGetCurrentSpeed();
	// current_dir = MotorGetCurrentDirect();
	DIR = Dir;
	if( current < Speed )
	{
		//MotorSpeedUp_2( current, Speed );
	}
	else if( Speed < current )
	{
		MotorFrequecyOut( Speed );
		//MotorSpeedDown_2( current, Speed );
	}
	MotorFrequecyOut( Speed );
	MotorUpdateSpeed( Speed );
	MotorUpdateDirect( Dir );
}

/*
// 阶梯升速法调速
void MotorSpeedUp( unsigned int current, unsigned int destinct )
{
	int i;
	unsigned int delta_freq = destinct - current;
	int n = delta_freq / FQ;
	
	for(i = 0; i < n; i++)
	{
		current += FQ;
		if( current > destinct )
		{
			current = current-FQ;
			break;
		}
		MotorFrequecyOut( current );
		/// if( destinct > 100 )
		// MotorWaitForResponse( FQ );
		MotorWaitForResponse( current );
	}
	MotorFrequecyOut( destinct );
	// MotorWaitForResponse( destinct-current );
	MotorWaitForResponse( destinct );
}

// 阶梯升速法调速
void MotorSpeedDown( unsigned int current, unsigned int destinct )
{
	int i;
	unsigned int delta_freq = current - destinct;
	int n = delta_freq / FQ;
	
	for(i = 0; i < n; i++)
	{
		if( current < destinct+FQ )
		{
			break;
		}
		current -= FQ;
		MotorFrequecyOut( current );
		// MotorWaitForResponse( FQ );
		MotorWaitForResponse( current );
	}
	MotorFrequecyOut( destinct );
	// MotorWaitForResponse( destinct-current );
	MotorWaitForResponse( destinct );
}
*/


// 电机S型加速
void MotorSpeedUp( unsigned int current, unsigned int destinct )
{
	unsigned int i = 0, flag = 0;
	unsigned int t_vary = current+(destinct-current)/INCREASE_FACT;
	unsigned int t_decre = 0;
	unsigned int step_speed = 1;
	unsigned int start_freq = FQ;
	
	if( current < start_freq )
	{
		current = start_freq;
	}
	while( current < destinct )
	{
		current += step_speed;
		if( current < t_vary )
		{
			if( !(i % ACCE_FACTION) )
			{
				step_speed+=VARIES_ACCE;
			}
		}
		else
		{
			if( flag == 0 )
			{
				t_decre = destinct-((i*i)>>1);
				flag = 1;
			}
			else if( t_decre < current )
			{
				if( !(i % ACCE_FACTION) )
				{
					step_speed-=VARIES_ACCE;
				}
			}
		}
		MotorFrequecyOut( current );
		// delay_ms(1);
		MotorWaitForResponse( current );
		// MotorWaitForResponse( step_speed );
		i++;
		// printf("%d ",current);
	}
	MotorFrequecyOut( destinct );
	// MotorWaitForResponse( step_speed );
	MotorWaitForResponse( destinct );
}

// 电机S型减速
void MotorSpeedDown( unsigned int current, unsigned int destinct )
{
	unsigned int i = 0, flag = 0;
	unsigned int t_vary = current-(current-destinct)/INCREASE_FACT;
	unsigned int t_decre = 0;
	unsigned int step_speed = 1;
	unsigned int start_freq = FQ;
	
	if( destinct < start_freq )
	{
		destinct = start_freq;
	}
	while( destinct < current )
	{
		if( current <= step_speed )
		{
			MotorFrequecyOut( 0 );
			return;
		}
		current -= step_speed;
		if( t_vary < current )
		{
			if( !(i % ACCE_FACTION) )
			{
				step_speed+=VARIES_ACCE;
			}
		}
		else
		{
			if( flag == 0 )
			{
				t_decre = destinct+((i*i)>>1);
				flag = 1;
			}
			else if( current < t_decre )
			{
				if( step_speed <= VARIES_ACCE )
				{
					MotorFrequecyOut( destinct );
					return;
				}
				if( !(i % ACCE_FACTION) )
				{
					step_speed-=VARIES_ACCE;
				}
			}
		}
		MotorFrequecyOut( current );
		// delay_ms(1);
		MotorWaitForResponse( current );
		// MotorWaitForResponse( step_speed );
		i++;
	//	printf("%d ",current);
	}
	MotorFrequecyOut( destinct );
	// MotorWaitForResponse( step_speed );
	MotorWaitForResponse( destinct );
}


// 频率变化时等待步进电机响应所需步数
// 可用Encoder_GetAngle()来提高移植性
void MotorWaitForResponse( unsigned int delta_speed )
{
	unsigned int wait = delta_speed / WAITONESTEP;
	unsigned int origin = Encoder_GetData(1);			
	unsigned int data = origin;
	unsigned int change = 0;
	
	if(wait > 512)
	{
		return;
	}
	while(change < wait)
	{
		origin = data;
		data = Encoder_GetData(1);
		if(data >= origin)
		{
			if((data - origin) >= 512)
			{
				change += (1024 - data + origin);
			}
			else
			{
				change += (data - origin);
			}
		}
		else
		{
			if((origin - data) >= 512)
			{
				change += (1024 - origin + data);
			}
			else
			{
				change += (origin - data);
			}
		}
	}
}

void MotorStop(void)
{
//	EN = 1;
//	FPGA_74595_SendData(0);
//	FPGA_74595_Clear();
//	FPGA_74595_SendData(0);
	TIM_Cmd(TIM4, DISABLE);
	disable_flag =1;
}



void MotorSpeedUp_3( unsigned int current, unsigned int destinct )
{
	int i;
	unsigned int delta_freq = destinct - current;
	unsigned int fq = 4;
	int n = delta_freq / fq;
	
	for(i = 0; i < n; i++)
	{
		current += fq;
		if( current > destinct )
		{
			current = current-fq;
			break;
		}
		MotorFrequecyOut( current );
		delay_ms(1);
		/// if( destinct > 100 )
		// MotorWaitForResponse( fq );
		// MotorWaitForResponse( current );
	}
	MotorFrequecyOut( destinct );
	// MotorWaitForResponse( destinct-current );
	// MotorWaitForResponse( destinct );
}

void MotorSpeedDown_3( unsigned int current, unsigned int destinct )
{
	int i;
	unsigned int delta_freq = current - destinct;
	unsigned int fq = 4;
	int n = delta_freq / fq;
	
	for(i = 0; i < n; i++)
	{
		if( current < destinct+fq )
		{
			break;
		}
		current -= fq;
		MotorFrequecyOut( current );
		// MotorWaitForResponse( fq );
		// MotorWaitForResponse( current );
	}
	MotorFrequecyOut( destinct );
	// MotorWaitForResponse( destinct-current );
	// MotorWaitForResponse( destinct );
}

