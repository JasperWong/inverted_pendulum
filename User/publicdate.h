#ifndef __PUBLICDATE_H
#define __PUBLICDATE_H
#include "stm32f10x.h"
#include "stdio.h"
void delay_us(u32 nus);
void delay_ms(u16 nms);
void USART1_Config(void);
int fputc(int ch, FILE *f);
#endif

