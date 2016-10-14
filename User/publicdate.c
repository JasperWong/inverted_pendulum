#include "publicdate.h"
void delay_us(u32 nus)									                                        //微秒级延时
{
	 u32 temp;
	 SysTick->LOAD=nus*9;
	 SysTick->VAL=0x00;
	 SysTick->CTRL=0x01;
	 do
	 {
	  	temp=SysTick->CTRL;
	 }
	 while((temp&0x01)&&!(temp&(1<<16)));
	 SysTick->CTRL=0x00;
	 SysTick->VAL=0x00;
}
													                                       	    //毫秒级延时
void delay_ms(u16 nms)
{
 	u32 temp;
	SysTick->LOAD=nms*9000;
	SysTick->VAL=0X00;
	SysTick->CTRL=0X01;
	do
	{
	 temp=SysTick->CTRL;
	}
	while((temp&0x01)&&!(temp&(1<<16)));
	SysTick->CTRL=0X00;
	SysTick->VAL=0x00;
}
														                                   
void USART1_Config(void)
{																							   //串口1初始化
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);		   //打开GPIOA和串口1的时钟
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;										   //复用推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);    

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;											   
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;								   //浮空输入
		GPIO_Init(GPIOA, &GPIO_InitStructure);
			
		USART_InitStructure.USART_BaudRate = 115200;										   //波特率15200
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;							   //8位数据位
		USART_InitStructure.USART_StopBits = USART_StopBits_1;								   //1个停止位
		USART_InitStructure.USART_Parity = USART_Parity_No ;								   //无奇偶校验
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		   //硬件流控制失能
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;						   //发送接收使能
		USART_Init(USART1, &USART_InitStructure); 
		USART_Cmd(USART1, ENABLE);															   //串口1使能
}


int fputc(int ch, FILE *f)			                                                           //重定向c库函数printf到USART1 
{
		USART_SendData(USART1, (uint8_t) ch);
		
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);		
	
		return (ch);
}



