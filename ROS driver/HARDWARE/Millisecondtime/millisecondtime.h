#ifndef _MILLISECONDTIMER_H_
#define _MILLISECONDTIMER_H_

#ifdef __cplusplus
extern "C" {
#endif
	
#include "stm32f10x.h"

extern u8 UI_send_status;
extern u8 speed_update;
extern u8 battery_update;
	
void initia_millisecondtime(void);
void delay_millis(uint32_t millis_);
uint32_t get_millis(void);
void millis_reset(void);

#endif
#ifdef __cplusplus
}
#endif



