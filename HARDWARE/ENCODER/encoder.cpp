#include "encoder.h"
#include "millisecondtime.h"
#include "usart.h"

u8 TIM3_update = 0;
int32_t ecoder1_irqn_num = 0;
int32_t ecoder2_irqn_num = 0;

GPIO_TypeDef*  ENCODER_PORT[ENCODERn] = {ENCODER1_GPIO_PORT, ENCODER2_GPIO_PORT};
TIM_TypeDef*   ENCODER_TIM[ENCODERn] = {ENCODER1_TIM, ENCODER2_TIM};
const uint16_t ENCODER_A_PIN[ENCODERn] = {ENCODER1_A_PIN, ENCODER2_A_PIN};
const uint16_t ENCODER_B_PIN[ENCODERn] = {ENCODER1_B_PIN, ENCODER2_B_PIN};
const uint32_t ENCODER_PORT_CLK[ENCODERn] = {ENCODER1_GPIO_CLK, ENCODER2_GPIO_CLK};
const uint32_t ENCODER_TIM_CLK[ENCODERn] = {ENCODER1_TIM_CLK, ENCODER2_TIM_CLK};
const uint8_t TIME_IRQn[ENCODERn] = {ENCODER1_IRQn,ENCODER2_IRQn};


ENCODER::ENCODER(Encoder_TypeDef _encoder, uint32_t _arr, uint32_t _psc, uint32_t _counts_per_rev)
{
	encoder = _encoder;
	arr = _arr;
	psc = _psc;
	counts_per_rev = _counts_per_rev;

	position = 0;
	last_time = 0;
	last_time_diff = 0;
}

void ENCODER::init()
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(ENCODER_PORT_CLK[this->encoder], ENABLE);
	GPIO_InitStructure.GPIO_Pin = ENCODER_A_PIN[this->encoder] | ENCODER_B_PIN[this->encoder];
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(ENCODER_PORT[this->encoder], &GPIO_InitStructure);

	RCC_APB1PeriphClockCmd(ENCODER_TIM_CLK[this->encoder], ENABLE);
	TIM_DeInit(ENCODER_TIM[this->encoder]);  
	TIM_TimeBaseInit(ENCODER_TIM[this->encoder], &TIM_TimeBaseStructure); 

	TIM_TimeBaseStructure.TIM_Period = this->arr; 
	TIM_TimeBaseStructure.TIM_Prescaler = this->psc;     //设置预分频：  
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1;  //设置时钟分频系数：不分频 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式   

	TIM_TimeBaseInit(ENCODER_TIM[this->encoder], &TIM_TimeBaseStructure);

	if(this->encoder == ENCODER1){
		TIM_EncoderInterfaceConfig(ENCODER_TIM[this->encoder], TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Falling);
	} else {
		TIM_EncoderInterfaceConfig(ENCODER_TIM[this->encoder], TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Rising);
	}

	TIM_ICStructInit(&TIM_ICInitStructure); 
	TIM_ICInitStructure.TIM_ICFilter = 6; 
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
	TIM_ICInit(ENCODER_TIM[this->encoder], &TIM_ICInitStructure);
	
	TIM_ClearFlag(ENCODER_TIM[this->encoder], TIM_FLAG_Update); 
	TIM_ITConfig(ENCODER_TIM[this->encoder], TIM_IT_Update, ENABLE); 
	TIM_SetCounter(ENCODER_TIM[this->encoder], 0);

	NVIC_InitStructure.NVIC_IRQChannel = TIME_IRQn[this->encoder];  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = this->encoder+2;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	
	TIM_Cmd(ENCODER_TIM[this->encoder], ENABLE); 
}

int32_t ENCODER::get_position()
{
	return position;
}

int32_t ENCODER::read()
{
	int32_t timer_value=0;;
	if(this->encoder == ENCODER1)
		timer_value = TIM_GetCounter(ENCODER_TIM[this->encoder])+ecoder1_irqn_num*65536;
	else 
		timer_value = TIM_GetCounter(ENCODER_TIM[this->encoder])+ecoder2_irqn_num*65536;
	last_time_diff = timer_value - last_time;
	last_time = timer_value;
	if(this->encoder == ENCODER1)
	{
		position += last_time_diff;
	}
	else if(this->encoder == ENCODER2)
	{
		position += last_time_diff;
	}
	
	return position;
}

float ENCODER::get_PRM()
{
		long encoder_ticks = read();
		//this function calculates the motor's RPM based on encoder ticks and delta time
		unsigned long current_time = get_millis();
		unsigned long dt = current_time - prev_update_time_;

		//convert the time from milliseconds to minutes
		double dtm = (double)dt / 60000;
		double delta_ticks = encoder_ticks - prev_encoder_ticks_;

		//calculate wheel's speed 
		prev_update_time_ = current_time;
		prev_encoder_ticks_ = encoder_ticks;
		
		return (delta_ticks / counts_per_rev) / dtm;
}

void ENCODER::set_pos(int32_t pos)
{
	position = pos;
}

#ifdef __cplusplus
 extern "C" {
#endif 
	 
void TIM3_IRQHandler(void)
{ 		    		  			    
	if(TIM3->SR&0X0001)//溢出中断
	{    
		TIM3_update = 1;
		if(TIM3->CNT<0x7fff)
			ecoder1_irqn_num++; //正转
		else
			ecoder1_irqn_num--;	//反转
	}				   
	TIM3->SR&=~(1<<0);//清除中断标志位 	    
}

void TIM5_IRQHandler(void)
{ 		    		  			    
	if(TIM5->SR&0X0001)//溢出中断
	{    	
		if(TIM5->CNT<0x7fff)
			ecoder2_irqn_num++; //正转
		else
			ecoder2_irqn_num--;	//反转		
	}				   
	TIM5->SR&=~(1<<0);//清除中断标志位 	    
}
#ifdef __cplusplus
}
#endif
