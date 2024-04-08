#include "bsp_systick.h"
#include "core_cm3.h"

#if 0
static __INLINE uint32_t SysTick_Config(uint32_t ticks)
{ 
	//�ж�ticks��ֵ�Ƿ�ϸ�
	if (ticks > SysTick_LOAD_RELOAD_Msk)  return (1);           
   
	//������װ�ؼ�ʱ���ĳ�ʼֵ	
	SysTick->LOAD  = (ticks & SysTick_LOAD_RELOAD_Msk) - 1;     
	
	//�����ж����ȼ�Ϊ15�����
	NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1); 
	//���ü�������ֵΪ0
	SysTick->VAL   = 0;	
	
	//����systickΪ72M
	//ʹ���ж�
	//ʹ��systick
	SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | 
				   SysTick_CTRL_TICKINT_Msk   | 
                   SysTick_CTRL_ENABLE_Msk;                    
	return (0);                                              
	}                                 

#endif
void Systick_Delay_ms(uint32_t ms)
{
	uint32_t i;
	SysTick_Config(72000); //1ms
	for(i=0;i<ms;i++){
		while( !((SysTick->CTRL)&(1<<16)) );
	}
	SysTick->CTRL &=~ SysTick_CTRL_ENABLE_Msk;
}

void Systick_Delay_us(uint32_t us)
{
	uint32_t i;
	SysTick_Config(72); //1us
	for(i=0;i<us;i++){
		while( !((SysTick->CTRL)&(1<<16)) );
	}
	SysTick->CTRL &=~ SysTick_CTRL_ENABLE_Msk;
}

