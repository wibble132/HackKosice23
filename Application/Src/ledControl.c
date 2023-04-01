
#include "ledControl.h"

#include "led12a1.h"


void setLED(LED1202_Object_TypeDef* LED1202Obj, uint8_t x, uint8_t y, uint16_t r, uint16_t g, uint16_t b) {

	uint8_t dev = y;
	uint8_t channelr = x * 3;
	uint8_t channelg = x * 3 + 1;
	uint8_t channelb = x * 3 + 2;

	LED12A1_DigitalDimming(LED1202Obj, &r, channelr, 0, (TypedefEnumDevAddr)(LED_DEVICE1+dev));
	LED12A1_DigitalDimming(LED1202Obj, &g, channelg, 0, (TypedefEnumDevAddr)(LED_DEVICE1+dev));
	LED12A1_DigitalDimming(LED1202Obj, &b, channelb, 0, (TypedefEnumDevAddr)(LED_DEVICE1+dev));
}

void disableAllLED(LED1202_Object_TypeDef* LED1202Obj, uint8_t NumOfDev) {
	for (uint8_t dev = 0;dev<NumOfDev;dev++)
		{
		  LED12A1_ChannelDisable( LED1202Obj , LED_CHANNEL_ALL , (TypedefEnumDevAddr)( LED_DEVICE1 + dev));
		}
}
