/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */  

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "myusart.h"
#include "bsp_systick.h"
#include "mycan.h"
#include "key.h"
#include "bsp_led.h"

/* Private define ------------------------------------------------------------*/


/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	uint8_t i=0,t=0,cnt=0,rxlen=0,x=0;
	uint8_t canbuf[8];
	uint8_t rxbuf[8];

	GPIO_Usart_Init();
	MyCAN_Init();
	Key_Init();
	LED_GPIO_Config();
	
	printf("≤‚ ‘÷–\r\n");
	
	
    while(1)
	{
		if(KEY_Scan()==1)
		{
			for(i=0;i<8;i++)
			{
				canbuf[i] =i + cnt;
			}
//			Can_Send_Msg(canbuf,8);
			MyCAN_Send_Message(canbuf,8);
		}
//		rxlen=Can_Receive_Msg(rxbuf);
		rxlen=MyCAN_Receive_Message(rxbuf);
		
		if(rxlen)
		{
			for(x=0; x<rxlen; x++)
			{
				printf("%x ",rxbuf[x]);
			}
			printf("\r\n");
		}
		t++;
		Systick_Delay_ms(20);
		
		if(t == 20)
		{
			LED_R_TOGGLE;
			t=0;
		}
		
		cnt++;
	}
}





/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
