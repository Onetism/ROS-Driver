#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"

class LED{
	public:
		LED(bool led_status);
		void init();
		void on_off(bool status);
		void revese();
	private:
		bool m_bled_status;
};

#endif
