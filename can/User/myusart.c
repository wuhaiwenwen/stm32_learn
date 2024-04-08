#include "stm32f10x.h"
#include "myusart.h"

void GPIO_Usart_Init(void)
{
	GPIO_InitTypeDef GPIO_Usart_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	GPIO_Usart_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Usart_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_Usart_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_Usart_InitStructure);
	
	GPIO_Usart_InitStructure.GPIO_Mode=GPIO_Mode_IPD;
	GPIO_Usart_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_Usart_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_Usart_InitStructure);
	
	USART_InitStructure.USART_BaudRate=115200;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Tx;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);

	USART_Cmd(USART1, ENABLE);
}

void MyUsart_SendByte(uint8_t data)
{
	USART_SendData(USART1, data);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET);
}

void MyUsart_SendArray(uint8_t *arr,uint8_t count)
{
	uint8_t i;
	for(i=0;i<count;i++)
	{
		MyUsart_SendByte(arr[i]);
	}

}

int fputc(int ch,FILE *f)
{
	MyUsart_SendByte(ch);
	return ch;
}

int fgetc(FILE *f)
{
	while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)==RESET);
	return (int)USART_ReceiveData(USART1);
}

