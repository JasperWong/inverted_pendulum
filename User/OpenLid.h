#ifndef __OPENLID_H
#define __OPENLID_H
#include "stm32f10x.h"

void Servo_GPIO_Config(void);
void Servo_TIM3_Config(u16 per,u16 psc);
void OpenLid_Config(void);
#endif
