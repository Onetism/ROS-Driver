#ifndef __TIME_H
#define __TIME_H

#include "sys.h"

//extern bool spped_update;

typedef enum {
	TIME1 = 0,
	TIME2 = 1,
	TIME_END = 2
}Time_TypeDef; 

class TIME{
	public:
		TIME(Time_TypeDef _time,uint32_t _arr,uint32_t _psc);
		void init();
	private:
		Time_TypeDef m_time;
		uint32_t m_u32arr;
		uint32_t m_u32psc;
};

#define TIMEn	2

#define TIME1_TIM         	TIM1
#define TIME1_TIM_CLK     	RCC_APB2Periph_TIM1
#define TIME1_IRQn			TIM1_UP_IRQn

#define TIME2_TIM         	TIM4
#define TIME2_TIM_CLK     	RCC_APB1Periph_TIM4
#define TIME2_IRQn			TIM4_IRQn

#endif
