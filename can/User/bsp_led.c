#include "stm32f10x.h"
#include "bsp_led.h"

void LED_GPIO_Config(void){
	
	GPIO_InitTypeDef LED_GPIO_Structinit;
	
	RCC_APB2PeriphClockCmd(LED_RCC_GPIO_R, ENABLE);//RCC_APB2Periph_GPIOB
	RCC_APB2PeriphClockCmd(LED_RCC_GPIO_G, ENABLE);//RCC_APB2Periph_GPIOE
	
	
	LED_GPIO_Structinit.GPIO_Mode=GPIO_Mode_Out_PP;
	LED_GPIO_Structinit.GPIO_Pin=LED_GPIO_Pin;
	LED_GPIO_Structinit.GPIO_Speed=GPIO_Speed_10MHz;
	
	GPIO_Init(LED_GPIO_R, &LED_GPIO_Structinit);		
	GPIO_Init(LED_GPIO_G, &LED_GPIO_Structinit);
	
	GPIO_WriteBit(LED_GPIO_R, LED_GPIO_Pin, Bit_SET);
	GPIO_WriteBit(LED_GPIO_G, LED_GPIO_Pin, Bit_SET);
}

