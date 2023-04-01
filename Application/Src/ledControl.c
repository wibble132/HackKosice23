
#include "ledControl.h"

#include "led12a1.h"

#include "datatypes.h"

void enableLED(LED1202_Object_TypeDef *obj, uint8_t x, uint8_t y) {
	/*
	 * Utility function to enable an LED.
	 */
	uint8_t dev = x;

	for (uint8_t i = 0; i < 3; i++) {
		uint8_t channel = 3*(3-y) + i;

		LED12A1_ChannelEnable(
			obj,
			(TypeDefChannel) (LED_CHANNEL_0 << channel),
			(TypedefEnumDevAddr)(LED_DEVICE1 + dev)
		);
	}

}

void setLED(LED1202_Object_TypeDef *LED1202Obj, uint8_t x, uint8_t y, Colour col) {
	/*
	 * Utility function to set the colour of an LED.
	 */

	uint8_t dev = y;
	uint16_t colours[3] = {
		col.Red,
		col.Green,
		col.Blue
	};

	for (uint8_t i=0; i<3; i++) {
		uint8_t channel_col = 3*(3-x) + i;

		LED12A1_DigitalDimming(
			LED1202Obj,
			&colours[i],
			channel_col,
			0, // Pattern num, but we don't know what this is yet
			(TypedefEnumDevAddr)(LED_DEVICE1 + dev)
		);
	}
}

void disableAllLED(LED1202_Object_TypeDef* LED1202Obj, uint8_t NumOfDev) {
	for (uint8_t dev = 0;dev<NumOfDev;dev++)
		{
		  LED12A1_ChannelDisable( LED1202Obj , LED_CHANNEL_ALL , (TypedefEnumDevAddr)( LED_DEVICE1 + dev));
		}
}
