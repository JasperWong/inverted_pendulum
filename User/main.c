/**
  ******************************************************************************
  * @file    main.c
  * @author  vino&&jasper 
  * @version V1.0
  * @date    2016--10-15
  * @brief   基本框架\模块整合
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

FUNCTION CurrentFunction;

extern u16 speed;
extern u16 speed_step;

int main(void)
{	
    delay_init();
    USARTx_Config();
    MOTOR_GPIO_Configuration();
	EXTI_Pxy_Config();
	TIM2_Configuration();
    Encoder_Init();
    Step_Init(1);
    /*成功的第一二步*/
//    speed=25;
//    StepMotor(BACK,600);
//    speed=15;
//    StepMotor(BACK,600);
//    speed=8;
//    StepMotor(BACK,600);
//    speed=100;
//    StepMotor(FORE,800);  
    /*成功的第一二步*/
    
//    speed=50;
//    StepMotor(2,BACK,6400);
//    speed=65535;
//    StepMotor(2,FORE,1);
    Encoder_SetZero();
    speed_step=10;
    StepByPwm(BACK,12);
    delay_ms(500);
    StepStop();
	while(1)  
	{
        switch(CurrentFunction.fun_state)
		{           
			case STATE_FUNCTION_1:
                
            
            
            
                printf("motor:%f",Encoder_GetAngle(1)); 
                printf("\n");
                printf("pole:%f",Encoder_GetAngle(0)); 
                printf("\n");
                printf("\n");
                delay_nms(500000);
              
			break;
			
			case STATE_FUNCTION_2:
				printf("3");
			break;
			
			case STATE_FUNCTION_3:
				printf("4");
			break;
			
			case STATE_FUNCTION_4:
				printf("5");
			break;
			
			case STATE_FUNCTION_5:
				printf("6");
			break;
			
			case STATE_FUNCTION_6:
				printf("7");
			break;
            
            case STATE_FUNCTION_0:
            
           
			break;

			default:
				printf("else");
			break;
		}
	}
	
}

	


