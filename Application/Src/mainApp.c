
#include "mainApp.h"
#include "ledControl.h"

#include "stm32g0xx_hal.h"
#include <callbacks.h>
#include <stdbool.h>
#include "led12a1.h"
#include <bridge.h>
#include <callbacks.h>
#include <stdbool.h>
#include "led1202.h"
#include "led12a1.h"



uint8_t GuiMode = 0;
uint8_t DevAddrArray[10];
uint8_t NumOfDev = 0;
extern  uint8_t bridge_IN_buffer[];
extern  uint8_t bridge_OUT_buffer[];
extern volatile uint8_t command_triggered;
extern   LED1202_Object_TypeDef LED1202Obj;

#define LED_CURRENT 1

void setup() {

  /*Wait Time assuring a properly steady state condition */
  HAL_Delay(100);

  interface_mode = UART_MODE;	/*Set a known condition */

  /*Clear Buffer*/
  ClearBuffers();

  /*LED12A1 Initialization*/
  LED12A1_Init();

  /*Scan for Number of available LED1202 ICs*/
  NumOfDev =  LED12A1_ScanAvailDevice( &LED1202Obj ,DevAddrArray);

  /*Enable available LED1202 ICs*/
  for (uint8_t dev = 0;dev<NumOfDev;dev++)
  {
	LED12A1_DeviceEnable(&LED1202Obj , (TypedefEnumDevAddr)(LED_DEVICE1 + dev));
	LED12A1_ChannelDisable( &LED1202Obj , LED_CHANNEL_ALL , (TypedefEnumDevAddr)( LED_DEVICE1 + dev));
  }
}

void mainLoop() {

	// initialise LEDs as all red
	disableAllLED( &LED1202Obj, NumOfDev );
	const int letters_buffer[27] = {0, 0, 0, 0, 14, 9,14, 8, 8, 8, 0, 6, 9, 2, 4, 9, 6, 0, 9, 9,14, 8, 8, 8, 0, 0, 0};
//	const bool device3_buffer[27] = {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0};
//	const bool device2_buffer[27] = {0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0};
//	const bool device1_buffer[27] = {0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0};
//	const bool device0_buffer[27] = {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0};

	bool enabled_flag = false;
	while (1) {
		uint8_t t = 3;
		while (t < 27) {
			for (uint8_t x = 0; x < 4; ++x) {
				for (uint8_t y = 0; y < 4; ++y) {
					if (!enabled_flag) {
						LED12A1_ChannelEnable( &LED1202Obj, (TypeDefChannel) (LED_CHANNEL_0<<(3*x)), (TypedefEnumDevAddr)(LED_DEVICE1 + y));
						LED12A1_ChannelEnable( &LED1202Obj, (TypeDefChannel) (LED_CHANNEL_0<<(3*x+1)), (TypedefEnumDevAddr)(LED_DEVICE1 + y));
						LED12A1_ChannelEnable( &LED1202Obj, (TypeDefChannel) (LED_CHANNEL_0<<(3*x+2)), (TypedefEnumDevAddr)(LED_DEVICE1 + y));
					}
					if ((letters_buffer[t - y] >> x) & 1) {
						setLED(&LED1202Obj, x, y, 50, 50, 50);
					}
					else {
						uint8_t r = 0;
						uint8_t g = 0;
						uint8_t b = 0;
						if (t - 3 < 8) {
							r = 11 - t;
							g = t - 3;
							b = 0;
						}
						else if (t - 3 < 16) {
							r = 0;
							g = 19 - t;
							b = t - 11;
						}
						else {
							r = t - 19;
							g = 0;
							b = 27 - t;
						}
						setLED(&LED1202Obj, x, y, r, g, b);
					}
				}
			}
			if (!enabled_flag) {
				enabled_flag = true;
			}
			HAL_Delay(200);
			t++;
		}
	}
}


void task1_mainLoop() {
	uint8_t t = 0;
	while (t < 10) {
		for (uint8_t x = 0; x < 4; ++x) {
			for (uint8_t y = 0; y < 4; ++y) {
				LED12A1_ChannelEnable( &LED1202Obj, (TypeDefChannel) (LED_CHANNEL_0<<(3*x)), (TypedefEnumDevAddr)(LED_DEVICE1 + y));
				LED12A1_ChannelEnable( &LED1202Obj, (TypeDefChannel) (LED_CHANNEL_0<<(3*x+1)), (TypedefEnumDevAddr)(LED_DEVICE1 + y));
				LED12A1_ChannelEnable( &LED1202Obj, (TypeDefChannel) (LED_CHANNEL_0<<(3*x+2)), (TypedefEnumDevAddr)(LED_DEVICE1 + y));
				setLED(&LED1202Obj, x, y, 10 - t, t, 0);
			}
		}
		HAL_Delay(20);
		t++;
	}
	while (t < 20) {
		for (uint8_t x = 0; x < 4; ++x) {
			for (uint8_t y = 0; y < 4; ++y) {
				LED12A1_ChannelEnable( &LED1202Obj, (TypeDefChannel) (LED_CHANNEL_0<<(3*x)), (TypedefEnumDevAddr)(LED_DEVICE1 + y));
				LED12A1_ChannelEnable( &LED1202Obj, (TypeDefChannel) (LED_CHANNEL_0<<(3*x+1)), (TypedefEnumDevAddr)(LED_DEVICE1 + y));
				LED12A1_ChannelEnable( &LED1202Obj, (TypeDefChannel) (LED_CHANNEL_0<<(3*x+2)), (TypedefEnumDevAddr)(LED_DEVICE1 + y));
				setLED(&LED1202Obj, x, y, 0, 20 - t, t - 10);
			}
		}
		HAL_Delay(20);
		t++;
	}
	while (t < 30) {
		for (uint8_t x = 0; x < 4; ++x) {
			for (uint8_t y = 0; y < 4; ++y) {
				LED12A1_ChannelEnable( &LED1202Obj, (TypeDefChannel) (LED_CHANNEL_0<<(3*x)), (TypedefEnumDevAddr)(LED_DEVICE1 + y));
				LED12A1_ChannelEnable( &LED1202Obj, (TypeDefChannel) (LED_CHANNEL_0<<(3*x+1)), (TypedefEnumDevAddr)(LED_DEVICE1 + y));
				LED12A1_ChannelEnable( &LED1202Obj, (TypeDefChannel) (LED_CHANNEL_0<<(3*x+2)), (TypedefEnumDevAddr)(LED_DEVICE1 + y));
				setLED(&LED1202Obj, x, y, t - 20, 0, 30 - t);
			}
		}
		HAL_Delay(20);
		t++;
	}
	for (uint8_t x = 0; x < 4; ++x) {
		for (uint8_t y = 0; y < 4; ++y) {
			setLED( &LED1202Obj, x, y, 0, 0, 0);
			HAL_Delay(1000);
		}
	}
}

void old_mainLoop() {

  uint16_t digCurrReg = 0;
  uint8_t dev = 0;
  uint8_t cnt;

	/*** Turn ON LED one by one ****/
	for (dev = 0;dev<NumOfDev;dev++)
	{
	  LED12A1_ChannelDisable( &LED1202Obj , LED_CHANNEL_ALL , (TypedefEnumDevAddr)( LED_DEVICE1 + dev));
	}

	for (dev = 0;dev<NumOfDev;dev++)
	{
	  for (uint8_t channel = 0;channel<=11;channel++)
	  {
		digCurrReg =  4095;
		LED12A1_ChannelEnable( &LED1202Obj , (TypeDefChannel)(LED_CHANNEL_0<<channel),  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
		LED12A1_AnalogDimming( &LED1202Obj , LED_CURRENT, channel,  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
		LED12A1_DigitalDimming( &LED1202Obj ,&digCurrReg,channel,0,  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
		HAL_Delay(200);
		LED12A1_ChannelDisable( &LED1202Obj , (TypeDefChannel)(LED_CHANNEL_0<<channel),  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
	  }

	  HAL_Delay(200);
	}

	for(cnt = 0;cnt<4;cnt++)
	{

	/*** Turn ON/OFF All LED at once in Toggling Mode ****/
	for (dev = 0;dev<NumOfDev;dev++)
	{
	  LED12A1_ChannelDisable( &LED1202Obj , LED_CHANNEL_ALL , (TypedefEnumDevAddr)( LED_DEVICE1 + dev));
	}
	HAL_Delay(300);
	for (dev = 0;dev<NumOfDev;dev++)
	{
	  for (uint8_t channel = 0;channel<=11;channel++)
	  {
		LED12A1_ChannelEnable( &LED1202Obj , (TypeDefChannel)(LED_CHANNEL_0<<channel),  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
		LED12A1_AnalogDimming( &LED1202Obj , LED_CURRENT, channel,  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
	  }
	}
	HAL_Delay(300);
	}

	/*** Start LED from minimum intensity to maximum intensity one by one ****/
	for (dev = 0;dev<NumOfDev;dev++)
	{
	  LED12A1_ChannelDisable( &LED1202Obj , LED_CHANNEL_ALL , (TypedefEnumDevAddr)( LED_DEVICE1 + dev));
	}

	for (dev = 0;dev<NumOfDev;dev++)
	{
	  for (uint8_t channel = 0;channel<=11;channel++)
	  {
		LED12A1_ChannelEnable( &LED1202Obj , (TypeDefChannel)(LED_CHANNEL_0<<channel),  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
		LED12A1_AnalogDimming( &LED1202Obj , LED_CURRENT, channel,  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
		for(uint8_t cnt = 0;cnt<10;cnt++)
		{
		  digCurrReg =  400*cnt;
		  LED12A1_DigitalDimming( &LED1202Obj ,&digCurrReg,channel,0,  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
		  HAL_Delay(100);
		}
	  }

	  HAL_Delay(300);
	}
}
