#ifndef _MYCAN_H
#define _MYCAN_H

#define GPIO_Pin_CAN_TX GPIO_Pin_12
#define GPIO_Pin_CAN_RX GPIO_Pin_11

void MyCAN_Init(void);

void MyCAN_Send_Message(uint8_t *txbuff,uint8_t len);

uint8_t MyCAN_Receive_Message(uint8_t *buf);

u8 Can_Send_Msg(u8* msg,u8 len);

u8 Can_Receive_Msg(u8 *buf);

#endif

