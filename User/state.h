#ifndef __STATE_H
#define	__STATE_H

#include "stm32f10x.h"
#include "stdint.h"

typedef struct 
{
	enum state
	{
		STATE_FUNCTION_0,
		STATE_FUNCTION_1,
		STATE_FUNCTION_2,
		STATE_FUNCTION_3,
		STATE_FUNCTION_4,
		STATE_FUNCTION_5,
		STATE_FUNCTION_6,
		
	};
	uint8_t fun_state;
}FUNCTION;



#endif /* __STATE_H */

