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
	
	CAN_InitStructure.CAN_ABOM=DISABLE;    //����Զ����߹���
	CAN_InitStructure.CAN_AWUM=DISABLE;    //˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)
	CAN_InitStructure.CAN_TXFP=DISABLE;    //���ȼ��ɱ��ı�ʶ������
	CAN_InitStructure.CAN_TTCM=DISABLE;	   //��ʱ�䴥��ͨ��ģʽ
	CAN_InitStructure.CAN_NART=ENABLE;     //��ֹ�����Զ����� 
	CAN_InitStructure.CAN_RFLM=DISABLE;    //���Ĳ�����,�µĸ��Ǿɵ�
	
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq; //����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ
	CAN_InitStructure.CAN_BS1=CAN_BS1_9tq; //Tbs1=tbs1+1��ʱ�䵥λ 1-16TQ
	CAN_InitStructure.CAN_BS2=CAN_BS1_8tq; //Tbs2=tbs2+1��ʱ�䵥λC 1-8TQ
	CAN_InitStructure.CAN_Mode=CAN_Mode_LoopBack; //ģʽ���ã� mode:0,��ͨģʽ;1,�ػ�ģʽ;
	CAN_InitStructure.CAN_Prescaler=0;       //��Ƶϵ��(Fdiv)Ϊbrp+1	
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
  TxMessage.StdId=0x12;					 // ��׼��ʶ�� 
  TxMessage.ExtId=0x12;				   // ������չ��ʾ�� 
  TxMessage.IDE=CAN_Id_Standard; // ��׼֡
  TxMessage.RTR=CAN_RTR_Data;		 // ����֡
  TxMessage.DLC=len;						// Ҫ���͵����ݳ���
  for(i=0;i<len;i++)
  TxMessage.Data[i]=msg[i];			          
  mbox= CAN_Transmit(CAN1, &TxMessage);   
  i=0;
  while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//�ȴ����ͽ���
  if(i>=0XFFF)return 1;
  return 0;		
}

void MyCAN_Send_Message(uint8_t *txbuff,uint8_t len)
{
	uint16_t i=0;
	uint8_t m_box;
	
	CanTxMsg  CanTxStrtcture;
	
	CanTxStrtcture.DLC=len;					// Ҫ���͵����ݳ���
	CanTxStrtcture.ExtId=0x12;        		// ������չ��ʾ�� 
	CanTxStrtcture.IDE=CAN_Id_Standard;     // ��׼֡
	CanTxStrtcture.RTR=CAN_RTR_Data;        // ����֡
	CanTxStrtcture.StdId=0x12;        		// ��׼��ʶ�� 
	
	for(i=0;i<len;i++)
	{
		CanTxStrtcture.Data[i]=txbuff[i];	
	}

	m_box=CAN_Transmit(CAN1, &CanTxStrtcture);
	
	while(CAN_TransmitStatus(CAN1,m_box)==CAN_TxStatus_Failed);
}

//can�ڽ������ݲ�ѯ
//buf:���ݻ�����;	 
//����ֵ:0,�����ݱ��յ�;
//		 ����,���յ����ݳ���;
u8 Can_Receive_Msg(u8 *buf)
{		   		   
 	u32 i;
	CanRxMsg RxMessage;
    if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;		//û�н��յ�����,ֱ���˳� 
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//��ȡ����	
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

