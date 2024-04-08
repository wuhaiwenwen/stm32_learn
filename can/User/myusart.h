#ifndef _MYUSART_H
#define _MYUSART_H

#include "stdio.h"

void GPIO_Usart_Init(void);
void MyUsart_SendByte(uint8_t data);
void MyUsart_SendArray(uint8_t *arr,uint8_t count);


#endif

