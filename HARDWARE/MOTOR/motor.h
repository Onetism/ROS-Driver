#ifndef __MOTOR_H
#define __MOTOR_H

#include "sys.h"

typedef enum {
	MOTOR1 = 0,
	MOTOR2 = 1,
	MOTOR_END = 4
}Motor_TypeDef; 

//四个电机驱动
#define MOTORn							2

#define MOTOR1_PWMA_PIN         GPIO_Pin_6
#define MOTOR1_PWMA_PORT        GPIOC
#define MOTOR1_PWMA_CLK         RCC_APB2Periph_GPIOC

#define MOTOR1_PWMB_PIN         GPIO_Pin_7
#define MOTOR1_PWMB_PORT        GPIOC
#define MOTOR1_PWMB_CLK         RCC_APB2Periph_GPIOC

#define MOTOR1_PWM_TIM         	TIM8
#define MOTOR1_PWM_TIM_CLK     	RCC_APB2Periph_TIM8

#define MOTOR2_PWMA_PIN         GPIO_Pin_8
#define MOTOR2_PWMA_PORT        GPIOC
#define MOTOR2_PWMA_CLK         RCC_APB2Periph_GPIOC

#define MOTOR2_PWMB_PIN         GPIO_Pin_9
#define MOTOR2_PWMB_PORT        GPIOC
#define MOTOR2_PWMB_CLK         RCC_APB2Periph_GPIOC

#define MOTOR2_PWM_TIM         	TIM8
#define MOTOR2_PWM_TIM_CLK     	RCC_APB2Periph_TIM8

//#define MOTOR3_PWMA_PIN         GPIO_Pin_8
//#define MOTOR3_PWMA_PORT        GPIOA
//#define MOTOR3_PWMA_CLK         RCC_APB2Periph_GPIOA

//#define MOTOR3_PWMB_PIN         GPIO_Pin_9
//#define MOTOR3_PWMB_PORT        GPIOA
//#define MOTOR3_PWMB_CLK         RCC_APB2Periph_GPIOA

//#define MOTOR3_PWM_TIM         	TIM1
//#define MOTOR3_PWM_TIM_CLK     	RCC_APB2Periph_TIM1

//#define MOTOR4_PWMA_PIN         GPIO_Pin_10
//#define MOTOR4_PWMA_PORT        GPIOA
//#define MOTOR4_PWMA_CLK         RCC_APB2Periph_GPIOA

//#define MOTOR4_PWMB_PIN         GPIO_Pin_11
//#define MOTOR4_PWMB_PORT        GPIOA
//#define MOTOR4_PWMB_CLK         RCC_APB2Periph_GPIOA

//#define MOTOR4_PWM_TIM         	TIM1
//#define MOTOR4_PWM_TIM_CLK     	RCC_APB2Periph_TIM1



#define constrain(amt,low,high) \
	((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

class MOTOR{
	public:
		MOTOR(Motor_TypeDef _motor,uint32_t _arr,uint32_t _psc); 
		void init();
		void setPWM(int pwm);
	private:
		Motor_TypeDef m_motor;
		uint32_t m_u32arr;
		uint32_t m_u32psc;
		void motor_pwm_init();
};

#endif
