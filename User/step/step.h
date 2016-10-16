#ifndef STEP_H
#define STEP_H

#include "stm32f10x.h"
#include "Encoder.h"

#define FORE  0
#define BACK  1
#define ON  0
#define OFF 1
#define StepMotor_GPIO_PORT 		GPIOB							/* GPIO�˿� */
#define StepMotor_GPIO_CLK 	  RCC_APB2Periph_GPIOB		            /* GPIO�˿�ʱ�� */
#define StepMotor_STP_GPIO_PIN		  GPIO_Pin_8			        /* ʱ����GPIO */
#define StepMotor_DIR_GPIO_PIN		  GPIO_Pin_13			        /* ����GPIO */
#define StepMotor_EN_GPIO_PIN		  GPIO_Pin_12			        /* ʹ��GPIO */
#define StepMotor_EN(a)	if (a)	\
					GPIO_SetBits(StepMotor_GPIO_PORT,StepMotor_EN_GPIO_PIN);\
					else		\
					GPIO_ResetBits(StepMotor_GPIO_PORT,StepMotor_EN_GPIO_PIN)
#define StepMotor_DIR(a)	if (a)	\
                            GPIO_SetBits(StepMotor_GPIO_PORT,StepMotor_DIR_GPIO_PIN);\
                            else		\
                            GPIO_ResetBits(StepMotor_GPIO_PORT,StepMotor_DIR_GPIO_PIN)

static void TIM4_GPIO_Config(void);
void TIM4_PWM_Init(u16 arr,u16 psc);
void Step_Init(u16 speed);
void StepByPwm(u8 direction,u32 step);
void StepStop();












#endif