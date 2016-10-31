#include "stm32f10x.h"
#ifndef __MOTOR_H
#define __MOTOR_H

#define StepMotor0_GPIO_PORT 		GPIOB										/* GPIO�˿� */
#define StepMotor0_GPIO_CLK 	  RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define StepMotor0_STP_GPIO_PIN		  GPIO_Pin_0			        /* ���ӵ�SCLʱ���ߵ�GPIO */
#define StepMotor0_DIR_GPIO_PIN		  GPIO_Pin_1			        /* ���ӵ�SCLʱ���ߵ�GPIO */
#define StepMotor0_EN_GPIO_PIN		  GPIO_Pin_2			        /* ���ӵ�SCLʱ���ߵ�GPIO */

#define StepMotor1_GPIO_PORT 		GPIOB										/* GPIO�˿� */
#define StepMotor1_GPIO_CLK 	  RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define StepMotor1_STP_GPIO_PIN		  GPIO_Pin_3			        /* ���ӵ�SCLʱ���ߵ�GPIO */
#define StepMotor1_DIR_GPIO_PIN		  GPIO_Pin_4			        /* ���ӵ�SCLʱ���ߵ�GPIO */
#define StepMotor1_EN_GPIO_PIN		  GPIO_Pin_5			        /* ���ӵ�SCLʱ���ߵ�GPIO */

#define StepMotor2_GPIO_PORT 		GPIOB										/* GPIO�˿� */
#define StepMotor2_GPIO_CLK 	  RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define StepMotor2_STP_GPIO_PIN		  GPIO_Pin_8			        /* ���ӵ�SCLʱ���ߵ�GPIO */
#define StepMotor2_DIR_GPIO_PIN		  GPIO_Pin_13			        /* ���ӵ�SCLʱ���ߵ�GPIO */
#define StepMotor2_EN_GPIO_PIN		  GPIO_Pin_12			        /* ���ӵ�SCLʱ���ߵ�GPIO */

#define StepMotor3_GPIO_PORT 		 GPIOB										/* GPIO�˿� */
#define StepMotor3_GPIO_CLK 	  RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define StepMotor3_STP_GPIO_PIN		  GPIO_Pin_9			        /* ���ӵ�SCLʱ���ߵ�GPIO */
#define StepMotor3_DIR_GPIO_PIN		  GPIO_Pin_10			        /* ���ӵ�SCLʱ���ߵ�GPIO */
#define StepMotor3_EN_GPIO_PIN		  GPIO_Pin_11			        /* ���ӵ�SCLʱ���ߵ�GPIO */

#define StepMotor4_GPIO_PORT 		GPIOB										/* GPIO�˿� */
#define StepMotor4_GPIO_CLK 	  RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define StepMotor4_STP_GPIO_PIN		  GPIO_Pin_12			        /* ���ӵ�SCLʱ���ߵ�GPIO */
#define StepMotor4_DIR_GPIO_PIN		  GPIO_Pin_13			        /* ���ӵ�SCLʱ���ߵ�GPIO */
#define StepMotor4_EN_GPIO_PIN		  GPIO_Pin_14			        /* ���ӵ�SCLʱ���ߵ�GPIO */

#define StepMotor5_GPIO_PORT 		GPIOC										/* GPIO�˿� */
#define StepMotor5_GPIO_CLK 	  RCC_APB2Periph_GPIOC		/* GPIO�˿�ʱ�� */
#define StepMotor5_STP_GPIO_PIN		  GPIO_Pin_0			        /* ���ӵ�SCLʱ���ߵ�GPIO */
#define StepMotor5_DIR_GPIO_PIN		  GPIO_Pin_1			        /* ���ӵ�SCLʱ���ߵ�GPIO */
#define StepMotor5_EN_GPIO_PIN		  GPIO_Pin_2			        /* ���ӵ�SCLʱ���ߵ�GPIO */

#define StepMotor6_GPIO_PORT 		GPIOC										/* GPIO�˿� */
#define StepMotor6_GPIO_CLK 	  RCC_APB2Periph_GPIOC		/* GPIO�˿�ʱ�� */
#define StepMotor6_STP_GPIO_PIN		  GPIO_Pin_3			        /* ���ӵ�SCLʱ���ߵ�GPIO */
#define StepMotor6_DIR_GPIO_PIN		  GPIO_Pin_4			        /* ���ӵ�SCLʱ���ߵ�GPIO */
#define StepMotor6_EN_GPIO_PIN		  GPIO_Pin_5			        /* ���ӵ�SCLʱ���ߵ�GPIO */

#define StepMotor7_GPIO_PORT 		GPIOC										/* GPIO�˿� */
#define StepMotor7_GPIO_CLK 	  RCC_APB2Periph_GPIOC		/* GPIO�˿�ʱ�� */
#define StepMotor7_STP_GPIO_PIN		  GPIO_Pin_6			        /* ���ӵ�SCLʱ���ߵ�GPIO */
#define StepMotor7_DIR_GPIO_PIN		  GPIO_Pin_7			        /* ���ӵ�SCLʱ���ߵ�GPIO */
#define StepMotor7_EN_GPIO_PIN		  GPIO_Pin_8			        /* ���ӵ�SCLʱ���ߵ�GPIO */

#define FORE  1
#define BACK  0

#define StepMotor0_DIR(a)	if (a)	\
					GPIO_SetBits(StepMotor0_GPIO_PORT,StepMotor0_DIR_GPIO_PIN);\
					else		\
					GPIO_ResetBits(StepMotor0_GPIO_PORT,StepMotor0_DIR_GPIO_PIN)
					
#define StepMotor1_DIR(a)	if (a)	\
					GPIO_SetBits(StepMotor1_GPIO_PORT,StepMotor1_DIR_GPIO_PIN);\
					else		\
					GPIO_ResetBits(StepMotor1_GPIO_PORT,StepMotor1_DIR_GPIO_PIN)
					
#define StepMotor2_DIR(a)	if (a)	\
					GPIO_SetBits(StepMotor2_GPIO_PORT,StepMotor2_DIR_GPIO_PIN);\
					else		\
					GPIO_ResetBits(StepMotor2_GPIO_PORT,StepMotor2_DIR_GPIO_PIN)
					
#define StepMotor3_DIR(a)	if (a)	\
					GPIO_SetBits(StepMotor3_GPIO_PORT,StepMotor3_DIR_GPIO_PIN);\
					else		\
					GPIO_ResetBits(StepMotor3_GPIO_PORT,StepMotor3_DIR_GPIO_PIN)
					
#define StepMotor4_DIR(a)	if (a)	\
					GPIO_SetBits(StepMotor4_GPIO_PORT,StepMotor4_DIR_GPIO_PIN);\
					else		\
					GPIO_ResetBits(StepMotor4_GPIO_PORT,StepMotor4_DIR_GPIO_PIN)

#define StepMotor5_DIR(a)	if (a)	\
					GPIO_SetBits(StepMotor5_GPIO_PORT,StepMotor5_DIR_GPIO_PIN);\
					else		\
					GPIO_ResetBits(StepMotor5_GPIO_PORT,StepMotor5_DIR_GPIO_PIN)
					
#define StepMotor6_DIR(a)	if (a)	\
					GPIO_SetBits(StepMotor6_GPIO_PORT,StepMotor6_DIR_GPIO_PIN);\
					else		\
					GPIO_ResetBits(StepMotor6_GPIO_PORT,StepMotor6_DIR_GPIO_PIN)
					
#define ON  0
#define OFF 1
					
#define StepMotor0_EN(a)	if (a)	\
					GPIO_SetBits(StepMotor0_GPIO_PORT,StepMotor0_EN_GPIO_PIN);\
					else		\
					GPIO_ResetBits(StepMotor0_GPIO_PORT,StepMotor0_EN_GPIO_PIN)
					
#define StepMotor1_EN(a)	if (a)	\
					GPIO_SetBits(StepMotor1_GPIO_PORT,StepMotor1_EN_GPIO_PIN);\
					else		\
					GPIO_ResetBits(StepMotor1_GPIO_PORT,StepMotor1_EN_GPIO_PIN)
					
#define StepMotor2_EN(a)	if (a)	\
					GPIO_SetBits(StepMotor2_GPIO_PORT,StepMotor2_EN_GPIO_PIN);\
					else		\
					GPIO_ResetBits(StepMotor2_GPIO_PORT,StepMotor2_EN_GPIO_PIN)
					
#define StepMotor3_EN(a)	if (a)	\
					GPIO_SetBits(StepMotor3_GPIO_PORT,StepMotor3_EN_GPIO_PIN);\
					else		\
					GPIO_ResetBits(StepMotor3_GPIO_PORT,StepMotor3_EN_GPIO_PIN)
					
#define StepMotor4_EN(a)	if (a)	\
					GPIO_SetBits(StepMotor4_GPIO_PORT,StepMotor4_EN_GPIO_PIN);\
					else		\
					GPIO_ResetBits(StepMotor4_GPIO_PORT,StepMotor4_EN_GPIO_PIN)

#define StepMotor5_EN(a)	if (a)	\
					GPIO_SetBits(StepMotor5_GPIO_PORT,StepMotor5_EN_GPIO_PIN);\
					else		\
					GPIO_ResetBits(StepMotor5_GPIO_PORT,StepMotor5_EN_GPIO_PIN)
					
#define StepMotor6_EN(a)	if (a)	\
					GPIO_SetBits(StepMotor6_GPIO_PORT,StepMotor6_EN_GPIO_PIN);\
					else		\
					GPIO_ResetBits(StepMotor6_GPIO_PORT,StepMotor6_EN_GPIO_PIN)
					
void TIM2_Configuration(void);
void delay_nms(u32 n);
void MOTOR_GPIO_Configuration(void);
void StepMotor(char direction,u32 step);



#endif