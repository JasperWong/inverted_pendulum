/**
  ******************************************************************************
  * @file    main.c
  * @author  vino&&jasper 
  * @version V1.0
  * @date    2016--10-15
  * @brief   基本框架\模块整合\基础函数编写
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
	TIM3_Configuration();
	MOTOR_GPIO_Configuration();
    Step_Init();
	EXTI_Pxy_Config();
	TIM2_Configuration();
    Encoder_Init();
    Encoder_SetZero();
	PID_Init();
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
    
//    speed_step=50;
//    StepByPwm(BACK,12);
//    delay_ms(500);
//    StepStop();

	StepMotor_EN(ON);
	TIM_Cmd(TIM3, ENABLE);

//	while(1)
//	{
//		switch(CurrentFunction.fun_state)
//	    {       
//			case STATE_NONE:
//				TIM_Cmd(TIM3, DISABLE);
//				StepMotor_EN(OFF);
//			break;
//	      
//            case STATE_FUNCTION_1:
////				MOTOR_GPIO_Configuration();
//				QuickAround_Function1_2();
//				CurrentFunction.fun_state = STATE_NONE;
//            break;
//            
//            case STATE_FUNCTION_2:
////				MOTOR_GPIO_Configuration();
//                QuickAround_Function1_2();
//            	CurrentFunction.fun_state = STATE_NONE;
//            break;
//            
//            case STATE_FUNCTION_3:
//				StepMotor_EN(ON);
//				TIM_Cmd(TIM3, ENABLE);
////                PID_Cal();  
//            break;
//            
//            case STATE_FUNCTION_4:
//                
//            break;
//            
//            case STATE_FUNCTION_5:
//                printf("5");
//            break;
//            
//            case STATE_FUNCTION_6:
//                printf("6");
//            break;
//            
//            case STATE_FUNCTION_7:
//            
//           
//            break;
//        
//            default:
//                printf("else");
//            break;
//        }
//    }
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
    /*编码器1为步进电机,编码器0为摇杆,角度顺时针递增*/
    printf("motor:%f",Encoder_GetAngle(1)); 
    printf("\n");
    printf("pole:%f",Encoder_GetAngle(0)); 
    printf("\n");
    printf("\n");
    delay_nms(500000);  
}
