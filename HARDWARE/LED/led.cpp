#include "led.h"
#include "sys.h"

LED::LED(bool led_status)
{
	m_bled_status = led_status;
}

void LED::init()
{
	RCC->APB2ENR |= 1<<2;
	GPIOA->CRH &= 0xFFF0FFFF;
	GPIOA->CRH |= 0x00030000;
	if(m_bled_status == true)
		GPIOA->ODR |= 1<<12;
	else
		GPIOA->BRR |= 1<<12;
}

void LED::on_off(bool status)
{
	if(status == true)
	{
		GPIOA->ODR |= 1<<12;
		m_bled_status = true;
	}
	else
	{
		m_bled_status = false;
		GPIOA->BRR |= 1<<12;
	}
}

void LED::revese()
{
	if(m_bled_status == true)
	{
		GPIOA->BRR |= 1<<12;
		m_bled_status = false;
	}
	else
	{
		GPIOA->ODR |= 1<<12;
		m_bled_status = true;
	}
}
