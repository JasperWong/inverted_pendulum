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

FUNCTION CurrentFunction;

extern u16 speed;

int main(void)
{	
    USARTx_Config();
    
    MOTOR_GPIO_Configuration();
	EXTI_Pxy_Config();
	TIM2_Configuration();
    Encoder_Init();
//    speed=25;
//    StepMotor(2,BACK,600);
//    speed=15;
//    StepMotor(2,BACK,600);
//    speed=8;
//    StepMotor(2,BACK,600);
//    speed=100;
//    StepMotor(2,FORE,800);  
    
//    speed=50;
//    StepMotor(2,BACK,6400);
//    speed=65535;
//    StepMotor(2,FORE,1);
//    Encoder_SetZero();
	while(1)  
	{
        
//        printf("motor:%d",Encoder_Read(1)); 
//        printf("\n");
//        printf("pole:%d",Encoder_Read(0)); 
//        printf("\n");
//        printf("\n");
//        delay_nms(500000);
        switch(CurrentFunction.fun_state)
		{
			case STATE_FUNCTION_0:
				printf("7");
			break;
			
			case STATE_FUNCTION_1:
				printf("1");
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
			
			default:
				printf("else");
			break;
		}
	}
	
}

	


