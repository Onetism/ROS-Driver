#ifndef __ENCODER_H
#define __ENCODER_H

#include "sys.h"

extern u8 TIM3_update;

#define COUNTS_PER_REV  1560

typedef enum {
	ENCODER1 = 0,
	ENCODER2 = 1,
	ENCODER_END = 2
}Encoder_TypeDef; 

#define ENCODERn                    2

#define ENCODER1_A_PIN         GPIO_Pin_6
#define ENCODER1_B_PIN         GPIO_Pin_7
#define ENCODER1_GPIO_PORT     GPIOA
#define ENCODER1_GPIO_CLK      RCC_APB2Periph_GPIOA

#define ENCODER2_A_PIN         GPIO_Pin_0
#define ENCODER2_B_PIN         GPIO_Pin_1
#define ENCODER2_GPIO_PORT     GPIOA
#define ENCODER2_GPIO_CLK      RCC_APB2Periph_GPIOA

#define ENCODER1_TIM           TIM3
#define ENCODER1_TIM_CLK       RCC_APB1Periph_TIM3
#define ENCODER1_IRQn			TIM3_IRQn

#define ENCODER2_TIM           TIM5
#define ENCODER2_TIM_CLK       RCC_APB1Periph_TIM5
#define ENCODER2_IRQn			TIM5_IRQn


class ENCODER{
	public:
		ENCODER(Encoder_TypeDef _encoder, uint32_t _arr, uint32_t _psc, uint32_t _counts_per_rev);
		void init();
		int32_t read();
		float get_PRM();
		int32_t get_position();
		void set_pos(int32_t pos);
		
//		int32_t position;
		int32_t last_time;
		int32_t last_time_diff;
	
	private:
		Encoder_TypeDef encoder;
		uint32_t arr;
		uint32_t psc;
		uint32_t counts_per_rev;
		int32_t position;
		unsigned long prev_update_time_;
		long prev_encoder_ticks_;
};

#endif
