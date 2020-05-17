#include "motor.h"

GPIO_TypeDef* MOTOR_PWMA_PORT[MOTORn] = {MOTOR1_PWMA_PORT, MOTOR2_PWMA_PORT};
GPIO_TypeDef* MOTOR_PWMB_PORT[MOTORn] = {MOTOR1_PWMB_PORT, MOTOR2_PWMB_PORT};
TIM_TypeDef*  MOTOR_PWM_TIM[MOTORn] = {MOTOR1_PWM_TIM, MOTOR2_PWM_TIM};
const uint32_t  MOTOR_PWMA_PORT_CLK[MOTORn] = {MOTOR1_PWMA_CLK, MOTOR2_PWMA_CLK};
const uint32_t  MOTOR_PWMB_PORT_CLK[MOTORn] = {MOTOR1_PWMB_CLK, MOTOR2_PWMB_CLK};
const uint32_t  MOTOR_PWM_TIM_CLK[MOTORn] = {MOTOR1_PWM_TIM_CLK, MOTOR2_PWM_TIM_CLK};
const uint16_t  MOTOR_PWMA_PIN[MOTORn] = {MOTOR1_PWMA_PIN, MOTOR2_PWMA_PIN};
const uint16_t  MOTOR_PWMB_PIN[MOTORn] = {MOTOR1_PWMB_PIN, MOTOR2_PWMB_PIN};

MOTOR::MOTOR(Motor_TypeDef _motor, uint32_t _arr, uint32_t _psc)
{
	m_motor = _motor;
	m_u32arr = _arr;
	m_u32psc = _psc;
}

void MOTOR::init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(MOTOR_PWMA_PORT_CLK[this->m_motor] 
						| MOTOR_PWMB_PORT_CLK[this->m_motor], ENABLE);
	
	GPIO_InitStructure.GPIO_Pin     = MOTOR_PWMA_PIN[this->m_motor];
	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_Init(MOTOR_PWMA_PORT[this->m_motor], &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin     = MOTOR_PWMB_PIN[this->m_motor];
	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_Init(MOTOR_PWMB_PORT[this->m_motor], &GPIO_InitStructure);
	
	motor_pwm_init();
}

void MOTOR::motor_pwm_init()
{
	TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd(MOTOR_PWM_TIM_CLK[this->m_motor], ENABLE);

	TIM_BaseInitStructure.TIM_Period                = this->m_u32arr;
	TIM_BaseInitStructure.TIM_Prescaler             = this->m_u32psc;
	TIM_BaseInitStructure.TIM_ClockDivision         = TIM_CKD_DIV1;
	TIM_BaseInitStructure.TIM_CounterMode           = TIM_CounterMode_Up;
	TIM_BaseInitStructure.TIM_RepetitionCounter     = 0;

	TIM_TimeBaseInit(MOTOR_PWM_TIM[this->m_motor], &TIM_BaseInitStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 

	if(this->m_motor == MOTOR1){
		TIM_OC1Init(MOTOR_PWM_TIM[this->m_motor], &TIM_OCInitStructure);
		TIM_OC1PreloadConfig(MOTOR_PWM_TIM[this->m_motor], TIM_OCPreload_Enable);
		TIM_OC2Init(MOTOR_PWM_TIM[this->m_motor], &TIM_OCInitStructure);
		TIM_OC2PreloadConfig(MOTOR_PWM_TIM[this->m_motor], TIM_OCPreload_Enable);
	}

	if(this->m_motor == MOTOR2) {
		TIM_OC3Init(MOTOR_PWM_TIM[this->m_motor], &TIM_OCInitStructure);
		TIM_OC3PreloadConfig(MOTOR_PWM_TIM[this->m_motor], TIM_OCPreload_Enable);
		TIM_OC4Init(MOTOR_PWM_TIM[this->m_motor], &TIM_OCInitStructure);
		TIM_OC4PreloadConfig(MOTOR_PWM_TIM[this->m_motor], TIM_OCPreload_Enable);
	}
	TIM_ARRPreloadConfig(MOTOR_PWM_TIM[this->m_motor], ENABLE);

	TIM_CtrlPWMOutputs(MOTOR_PWM_TIM[this->m_motor], ENABLE);
	TIM_Cmd(MOTOR_PWM_TIM[this->m_motor], ENABLE);		
}

void MOTOR::setPWM(int pwm)
{
	if(this->m_motor == MOTOR1)
	{
		if(pwm > 0)
		{
			TIM_SetCompare1(MOTOR_PWM_TIM[this->m_motor], 7200);
			TIM_SetCompare2(MOTOR_PWM_TIM[this->m_motor], 7200-pwm);
		}else
		{
			TIM_SetCompare1(MOTOR_PWM_TIM[this->m_motor], 7200+pwm);
			TIM_SetCompare2(MOTOR_PWM_TIM[this->m_motor], 7200);
		}
	}
	else if(this->m_motor == MOTOR2)
	{
		if(pwm > 0)
		{
			TIM_SetCompare3(MOTOR_PWM_TIM[this->m_motor], 7200-pwm);
			TIM_SetCompare4(MOTOR_PWM_TIM[this->m_motor], 7200);
		}else
		{
			TIM_SetCompare3(MOTOR_PWM_TIM[this->m_motor], 7200);
			TIM_SetCompare4(MOTOR_PWM_TIM[this->m_motor], 7200+pwm);
		}	
	}
//	else if(this->m_motor == MOTOR3)
//	{
//		if(pwm > 0)
//		{
//			TIM_SetCompare1(MOTOR_PWM_TIM[this->m_motor], 7200);
//			TIM_SetCompare2(MOTOR_PWM_TIM[this->m_motor], 7200-pwm);
//		}else
//		{
//			TIM_SetCompare1(MOTOR_PWM_TIM[this->m_motor], 7200+pwm);
//			TIM_SetCompare2(MOTOR_PWM_TIM[this->m_motor], 7200);
//		}	
//	}
//	else if(this->m_motor == MOTOR4)
//	{
//		if(pwm > 0)
//		{
//			TIM_SetCompare3(MOTOR_PWM_TIM[this->m_motor], 7200);
//			TIM_SetCompare4(MOTOR_PWM_TIM[this->m_motor], 7200-pwm);
//		}else
//		{
//			TIM_SetCompare3(MOTOR_PWM_TIM[this->m_motor], 7200+pwm);
//			TIM_SetCompare4(MOTOR_PWM_TIM[this->m_motor], 7200);
//		}	
//	}	
	
}
