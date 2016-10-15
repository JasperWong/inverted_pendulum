#include "stm32f10x.h"
#ifndef ENCODER_H
#define ENCODER_H

typedef struct EncState
{
	unsigned int angle[2];
	signed int speed[2];
	float real_angle[2];
}EncoderState;

extern EncoderState State_Quantity;
extern EncoderState State_Quantity_2;

void Encoder_Init(void);
unsigned int Encoder_Read(int select);
void Encoder_SetZero(void);
unsigned int Encoder_GetData(int select);
float Encoder_GetAngle(int select);
signed int Encoder_GetSpeed(int select);
unsigned int Encoder_GetFIRData(int select);
#endif
