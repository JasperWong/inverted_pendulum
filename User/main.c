#include "stm32f10x.h"
#include "motor.h"

extern u16 speed;

int main(void)
{	

	MOTOR_GPIO_Configuration();
	TIM2_Configuration();
    speed=25;
    StepMotor(2,BACK,600);
    speed=15;
    StepMotor(2,BACK,600);
    speed=8;
    StepMotor(2,BACK,600);
    speed=100;
    StepMotor(2,FORE,800);  

	while( 1 )
	{
        

	}
}

	


