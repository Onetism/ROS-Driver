#include "time.h"

TIM_TypeDef*  TIME_TIM[TIMEn] = {TIME1_TIM, TIME2_TIM};
const uint32_t  TIME_TIM_CLK[TIMEn] = {TIME1_TIM_CLK, TIME2_TIM_CLK};
const uint8_t TIME_IRQn[TIMEn] = {TIME1_IRQn,TIME2_IRQn};

//bool spped_update = false;

TIME::TIME(Time_TypeDef _time,uint32_t _arr,uint32_t _psc)
{
	 m_time = _time;
	 m_u32arr = _arr;
	 m_u32psc = _psc;	
}

void TIME::init()
{
   TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	if(this->m_time == TIME1)
	{
		 RCC_APB2PeriphClockCmd(TIME_TIM_CLK[this->m_time] , ENABLE);
	}
	else
	{
		RCC_APB1PeriphClockCmd(TIME_TIM_CLK[this->m_time] , ENABLE);
	}
  
   TIM_DeInit(TIME_TIM[this->m_time]);
	NVIC_InitTypeDef NVIC_InitStructure;

   /* 自动重装载寄存器周期的值(计数值) */
   TIM_TimeBaseStructure.TIM_Period=this->m_u32arr;  
   TIM_TimeBaseStructure.TIM_Prescaler=this->m_u32psc; 
	
   TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;	
   TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
   TIM_TimeBaseInit(TIME_TIM[this->m_time], &TIM_TimeBaseStructure);
	
   TIM_ClearFlag(TIME_TIM[this->m_time], TIM_FLAG_Update);
   TIM_ITConfig(TIME_TIM[this->m_time],TIM_IT_Update,ENABLE);
   
	NVIC_InitStructure.NVIC_IRQChannel = TIME_IRQn[this->m_time];  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = this->m_time+2;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	
   TIM_Cmd(TIME_TIM[this->m_time], ENABLE);	
}



