#ifndef _BATTERY_H_
#define _BATTERY_H_ 

#include "sys.h"

#define ADC1_DR_ADDRESS         ((u32)0x4001244C)
#define BATTERY_PIN        		GPIO_Pin_1
#define BATTERY_GPIO_PORT      GPIOC
#define BATTERY_GPIO_CLK       RCC_APB2Periph_GPIOC
#define BATTERY_ADC_CLK        RCC_APB2Periph_ADC1
#define BATTERY_DMA_CLK        RCC_AHBPeriph_DMA1

class Battery {
public:
	Battery(float _threshold, float _volt_min, float _volt_max);
	void init();
	void filter();
	float get_volt();
	float get_battery_notifier();
	bool get_battery_low();

private:
	float threshold;
	float volt_min;
	float volt_max;
	__IO u16 ADC_ConvertedValue[1];
	float volt_filter;
};

#endif // _BATTERY_H_
