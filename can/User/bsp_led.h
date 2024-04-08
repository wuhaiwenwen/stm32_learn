#ifndef    _BSP_LED_H
#define    _BSP_LED_H

#define LED_RCC_GPIO_R  RCC_APB2Periph_GPIOB
#define LED_RCC_GPIO_G  RCC_APB2Periph_GPIOE

#define LED_GPIO_R  GPIOB
#define LED_GPIO_G  GPIOE

#define LED_GPIO_Pin  GPIO_Pin_5

#define ON  1
#define OFF 0

#define LED_R(a)  if(a) GPIO_ResetBits(LED_GPIO_R, LED_GPIO_Pin); else GPIO_SetBits(LED_GPIO_R, LED_GPIO_Pin);
#define LED_G(b)  if(b) GPIO_ResetBits(LED_GPIO_G, LED_GPIO_Pin); else GPIO_SetBits(LED_GPIO_G, LED_GPIO_Pin);

#define LED_R_TOGGLE {LED_GPIO_R->ODR^=LED_GPIO_Pin;}
#define LED_G_TOGGLE {LED_GPIO_G->ODR^=LED_GPIO_Pin;}


void LED_GPIO_Config(void);

#endif   /*_BSP_LED_H*/


