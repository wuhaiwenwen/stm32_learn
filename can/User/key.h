#ifndef _KEY_H
#define _KEY_H

#define GPIO_Pin_KEY0 GPIO_Pin_4
#define GPIO_Pin_KEY1 GPIO_Pin_3

void Key_Init(void);

uint8_t KEY_Scan(void);


#endif


