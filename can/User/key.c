#include "stm32f10x.h"
#include "key.h"

void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_KEY0 | GPIO_Pin_KEY1;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

uint8_t KEY_Scan(void)
{
	if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_KEY0)==0)
	{
		while(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_KEY0)==0)
		{
			return 1;
		}
		
	}else if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_KEY1)==0)
	{
		while(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_KEY1)==0)
		{
			return 2;
		}
		
	}else return 0;
}

