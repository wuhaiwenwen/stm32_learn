#include "stm32f10x.h"
#include "mycan.h"

void MyCAN_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	CAN_InitTypeDef CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_CAN_TX;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA ,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_CAN_RX;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA ,&GPIO_InitStructure);
	
	CAN_InitStructure.CAN_ABOM=DISABLE;    //软件自动离线管理
	CAN_InitStructure.CAN_AWUM=DISABLE;    //睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
	CAN_InitStructure.CAN_TXFP=DISABLE;    //优先级由报文标识符决定
	CAN_InitStructure.CAN_TTCM=DISABLE;	   //非时间触发通信模式
	CAN_InitStructure.CAN_NART=ENABLE;     //禁止报文自动传送 
	CAN_InitStructure.CAN_RFLM=DISABLE;    //报文不锁定,新的覆盖旧的
	
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq; //重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位
	CAN_InitStructure.CAN_BS1=CAN_BS1_9tq; //Tbs1=tbs1+1个时间单位 1-16TQ
	CAN_InitStructure.CAN_BS2=CAN_BS1_8tq; //Tbs2=tbs2+1个时间单位C 1-8TQ
	CAN_InitStructure.CAN_Mode=CAN_Mode_LoopBack; //模式设置： mode:0,普通模式;1,回环模式;
	CAN_InitStructure.CAN_Prescaler=0;       //分频系数(Fdiv)为brp+1	
	CAN_Init(CAN1, &CAN_InitStructure);
	
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;
	CAN_FilterInitStructure.CAN_FilterIdHigh=0;
	CAN_FilterInitStructure.CAN_FilterIdLow=0;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0;
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;//
	CAN_FilterInitStructure.CAN_FilterNumber=0;
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
	CAN_FilterInit(&CAN_FilterInitStructure);
}

u8 Can_Send_Msg(u8* msg,u8 len)
{	
  u8 mbox;
  u16 i=0;
  CanTxMsg TxMessage;
  TxMessage.StdId=0x12;					 // 标准标识符 
  TxMessage.ExtId=0x12;				   // 设置扩展标示符 
  TxMessage.IDE=CAN_Id_Standard; // 标准帧
  TxMessage.RTR=CAN_RTR_Data;		 // 数据帧
  TxMessage.DLC=len;						// 要发送的数据长度
  for(i=0;i<len;i++)
  TxMessage.Data[i]=msg[i];			          
  mbox= CAN_Transmit(CAN1, &TxMessage);   
  i=0;
  while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//等待发送结束
  if(i>=0XFFF)return 1;
  return 0;		
}

void MyCAN_Send_Message(uint8_t *txbuff,uint8_t len)
{
	uint16_t i=0;
	uint8_t m_box;
	
	CanTxMsg  CanTxStrtcture;
	
	CanTxStrtcture.DLC=len;					// 要发送的数据长度
	CanTxStrtcture.ExtId=0x12;        		// 设置扩展标示符 
	CanTxStrtcture.IDE=CAN_Id_Standard;     // 标准帧
	CanTxStrtcture.RTR=CAN_RTR_Data;        // 数据帧
	CanTxStrtcture.StdId=0x12;        		// 标准标识符 
	
	for(i=0;i<len;i++)
	{
		CanTxStrtcture.Data[i]=txbuff[i];	
	}

	m_box=CAN_Transmit(CAN1, &CanTxStrtcture);
	
	while(CAN_TransmitStatus(CAN1,m_box)==CAN_TxStatus_Failed);
}

//can口接收数据查询
//buf:数据缓存区;	 
//返回值:0,无数据被收到;
//		 其他,接收的数据长度;
u8 Can_Receive_Msg(u8 *buf)
{		   		   
 	u32 i;
	CanRxMsg RxMessage;
    if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;		//没有接收到数据,直接退出 
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//读取数据	
    for(i=0;i<8;i++)
    buf[i]=RxMessage.Data[i];  
	return RxMessage.DLC;	
}

uint8_t MyCAN_Receive_Message(uint8_t *buf)
{
	uint16_t i;
	CanRxMsg CanRxStrtcture;
	if(CAN_MessagePending(CAN1, CAN_FIFO0)==0)
	{
		return 0;	
	}
	
	CAN_Receive(CAN1, CAN_FIFO0, &CanRxStrtcture);
	
	for(i=0;i<CanRxStrtcture.DLC;i++)
	{
		buf[i] = CanRxStrtcture.Data[i];
	}
	return CanRxStrtcture.DLC;	
}

