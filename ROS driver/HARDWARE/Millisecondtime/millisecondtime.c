#include "millisecondtime.h"

volatile uint32_t _counter;
u8 UI_send_status = 0;
u8 speed_update = 0;
u8 battery_update = 0;


void initia_millisecondtime(void) 
{
	_counter = 0;
	SysTick_Config(SystemCoreClock / 1000);
}

void delay_millis(uint32_t millis) 
{
	uint32_t target;

	target = _counter + millis;
	while(_counter < target);
} 

void SysTick_Handler(void) 
{
	_counter++;
	if(_counter%50==0)
		speed_update = 1;
	if(_counter%10000==0)
		battery_update = 1;
}

uint32_t get_millis(void) 
{
	return _counter;
}

void millis_reset(void) 
{
	_counter = 0;
}

