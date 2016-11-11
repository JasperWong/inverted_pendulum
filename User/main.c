/**
  ******************************************************************************
  * @file    main.c
  * @author  vino&&jasper 
  * @version V1.0
  * @date    2016--10-15
  * @brief   �������\ģ������\����������д
  ******************************************************************************
  ******************************************************************************
  */ 

#include "stm32f10x.h"
#include "motor.h"
#include "state.h"
#include "bsp_exti.h"
#include "Encoder.h"
#include "usart.h"
#include "delay.h"
#include "step.h"
#include "PID.h"       

FUNCTION CurrentFunction;

//float MoterAngleCurrent=0;
//float PoleAngleCurrent=0;

extern u16 speed;
extern u16 speed_step;

void QuickAround_Function1_2();
void EncoderMoitor();

int main(void)
{	
    delay_init();
    USARTx_Config();
	USART1_DMA_Config();
	TIM3_Configuration();
	MOTOR_GPIO_Configuration();
    Step_Init();
	EXTI_Pxy_Config();
	TIM2_Configuration();
    Encoder_Init();
    Encoder_SetZero();
	PID_Init();
    /*�ɹ��ĵ�һ����*/
//    speed=25;
//    StepMotor(BACK,600);
//    speed=15;
//    StepMotor(BACK,600);
//    speed=8;
//    StepMotor(BACK,600);
//    speed=100;
//    StepMotor(FORE,800);  
    /*�ɹ��ĵ�һ����*/
    
//    speed=50;
//    StepMotor(2,BACK,6400);
//    speed=65535;
//    StepMotor(2,FORE,1);
    
//    speed_step=50;
//    StepByPwm(BACK,12);
//    delay_ms(500);
//    StepStop();

	StepMotor_EN(ON);
	TIM_Cmd(TIM3, ENABLE);
	
	while(1)
	{
		switch(CurrentFunction.fun_state)
	    {       
			case STATE_NONE:

			break;
	      
            case STATE_FUNCTION_1:

            break;
            
            case STATE_FUNCTION_2:

            break;
            
            case STATE_FUNCTION_3:

            break;
            
            case STATE_FUNCTION_4:

            break;
            
            case STATE_FUNCTION_5:

            break;
            
            case STATE_FUNCTION_6:

            break;
            
            case STATE_FUNCTION_7:
            
           
            break;
        
            default:
                printf("else");
            break;
        }
    }
}

	
void QuickAround_Function1_2()
{
    speed=25;
    StepMotor(BACK,600);
    speed=15;
    StepMotor(BACK,600);
    speed=8;
    StepMotor(BACK,600);
    speed=100;
    StepMotor(FORE,800);
}

void EncoderMoitor()
{
    /*������1Ϊ�������,������0Ϊҡ��,�Ƕ�˳ʱ�����*/
    printf("motor:%f",Encoder_GetAngle(1)); 
    printf("\n");
    printf("pole:%f",Encoder_GetAngle(0)); 
    printf("\n");
    printf("\n");
    delay_nms(500000);  
}
