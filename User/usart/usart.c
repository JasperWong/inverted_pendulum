/**
  ******************************************************************************
  * @file    bsp_usart1.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   usart应用bsp
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 iSO STM32 开发板 
  * 论坛    :http://www.chuxue123.com
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "usart.h"


uint8_t SendBuff[SENDBUFF_SIZE] = {		'a','d','d',' ','1',',','0',',','x','y','z',0xff,0xff,0xff,
										'n','0','.','v','a','l','=','x','y','z',0xff,0xff,0xff, 
										'n','1','.','v','a','l','=','x','y','z',0xff,0xff,0xff,
										'n','2','.','v','a','l','=','x','y','z',0xff,0xff,0xff
};

/// 配置USART1接收中断
//static void NVIC_Configuration(void)
//{
//	NVIC_InitTypeDef NVIC_InitStructure; 
//	/* Configure the NVIC Preemption Priority Bits */  
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
//	
//	/* Enable the USARTy Interrupt */
//	NVIC_InitStructure.NVIC_IRQChannel = macUSART_IRQ;	 
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//}


 /**
  * @brief  USART1 GPIO 配置,工作模式配置。115200 8-N-1
  * @param  无
  * @retval 无
  */
void USARTx_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/* config USART1 clock */
	macUSART_APBxClock_FUN(macUSART_CLK, ENABLE);
	macUSART_GPIO_APBxClock_FUN(macUSART_GPIO_CLK, ENABLE);
	
	/* USART1 GPIO config */
	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin =  macUSART_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(macUSART_TX_PORT, &GPIO_InitStructure);    
	/* Configure USART1 Rx (PA.10) as input floating */
	GPIO_InitStructure.GPIO_Pin = macUSART_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(macUSART_RX_PORT, &GPIO_InitStructure);
	
	/* USART1 mode config */
	USART_InitStructure.USART_BaudRate = macUSART_BAUD_RATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(macUSARTx, &USART_InitStructure);
	
	/* 使能串口1接收中断 */
//	NVIC_Configuration();
//	USART_ITConfig(macUSARTx, USART_IT_RXNE, ENABLE);
	
	USART_Cmd(macUSARTx, ENABLE);
}

void USART1_DMA_Config(void)
{
		DMA_InitTypeDef DMA_InitStructure;
	
		/*开启DMA时钟*/
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	
		//NVIC_Config();	   			//配置DMA中断

		/*设置DMA源：串口数据寄存器地址*/
		DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_DR_Base;	   

		/*内存地址(要传输的变量的指针)*/
		DMA_InitStructure.DMA_MemoryBaseAddr = (u32)SendBuff;

		/*方向：从内存到外设*/		
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;	

		/*传输大小DMA_BufferSize=SENDBUFF_SIZE*/	
		DMA_InitStructure.DMA_BufferSize = SENDBUFF_SIZE;

		/*外设地址不增*/	    
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 

		/*内存地址自增*/
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	

		/*外设数据单位*/	
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;

		/*内存数据单位 8bit*/
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 

		/*DMA模式：不断循环*/
//		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	 

		/*优先级：中*/	
		DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;  

		/*禁止内存到内存的传输	*/
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

		/*配置DMA1的4通道*/		   
		DMA_Init(DMA1_Channel4, &DMA_InitStructure); 	   
		
		/*使能DMA*/
		DMA_Cmd (DMA1_Channel4,ENABLE);					
		//DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE);  //配置DMA发送完成后产生中断
}

/// 重定向c库函数printf到USART1
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到USART1 */
		USART_SendData(macUSARTx, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(macUSARTx, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

/// 重定向c库函数scanf到USART1
int fgetc(FILE *f)
{
		/* 等待串口1输入数据 */
		while (USART_GetFlagStatus(macUSARTx, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(macUSARTx);
}

void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* ?????????USART1 */
	USART_SendData(pUSARTx,ch);
		
	/* ?????? */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

void Usart_SendStr( USART_TypeDef * pUSARTx, uint8_t *str )
{
	unsigned int k=0;
    do 
    {
        Usart_SendByte( pUSARTx, *(str + k) );
        k++;
    }while(*(str + k) != '\0');
}

/*****************  ?????????? **********************/
void Usart_SendStr_length( USART_TypeDef * pUSARTx, uint8_t *str,uint32_t strlen )
{
	unsigned int k=0;
    do 
    {
        Usart_SendByte( pUSARTx, *(str + k) );
        k++;
    } while(k < strlen);
}
/*********************************************END OF FILE**********************/
