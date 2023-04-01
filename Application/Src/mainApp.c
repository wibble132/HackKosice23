
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

#include "datatypes.h"


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

	Colour white = make_colour(10, 10, 10);

	disableAllLED( &LED1202Obj, NumOfDev );

	for (uint8_t x = 0; x < 4; ++x) {
		for (uint8_t y = 0; y < 4; ++y) {
			/*
			 * Enable all the channels
			 */

			uint8_t dev = y;

			for (int i = 0; i < 3; i++) {
				uint8_t channel = 3*x + i;

				LED12A1_ChannelEnable(
					&LED1202Obj,
					(TypeDefChannel) (LED_CHANNEL_0 << channel),
					(TypedefEnumDevAddr)(LED_DEVICE1 + dev)
				);
			}

			setLED( &LED1202Obj, x, y, white);
			HAL_Delay(1000);
		}
	}

	Colour black = make_colour(0, 0, 0);

	for (uint8_t x = 0; x < 4; ++x) {
		for (uint8_t y = 0; y < 4; ++y) {
			setLED( &LED1202Obj, x, y, black);
			HAL_Delay(1000);
		}
	}

}

//void old_mainLoop() {
//
//  uint16_t digCurrReg = 0;
//  uint8_t dev = 0;
//  uint8_t cnt;
//
//	/*** Turn ON LED one by one ****/
//	for (dev = 0;dev<NumOfDev;dev++)
//	{
//	  LED12A1_ChannelDisable( &LED1202Obj , LED_CHANNEL_ALL , (TypedefEnumDevAddr)( LED_DEVICE1 + dev));
//	}
//
//	for (dev = 0;dev<NumOfDev;dev++)
//	{
//	  for (uint8_t channel = 0;channel<=11;channel++)
//	  {
//		digCurrReg =  4095;
//		LED12A1_ChannelEnable( &LED1202Obj , (TypeDefChannel)(LED_CHANNEL_0<<channel),  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
//		LED12A1_AnalogDimming( &LED1202Obj , LED_CURRENT, channel,  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
//		LED12A1_DigitalDimming( &LED1202Obj ,&digCurrReg,channel,0,  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
//		HAL_Delay(200);
//		LED12A1_ChannelDisable( &LED1202Obj , (TypeDefChannel)(LED_CHANNEL_0<<channel),  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
//	  }
//
//	  HAL_Delay(200);
//	}
//
//	for(cnt = 0;cnt<4;cnt++)
//	{
//
//	/*** Turn ON/OFF All LED at once in Toggling Mode ****/
//	for (dev = 0;dev<NumOfDev;dev++)
//	{
//	  LED12A1_ChannelDisable( &LED1202Obj , LED_CHANNEL_ALL , (TypedefEnumDevAddr)( LED_DEVICE1 + dev));
//	}
//	HAL_Delay(300);
//	for (dev = 0;dev<NumOfDev;dev++)
//	{
//	  for (uint8_t channel = 0;channel<=11;channel++)
//	  {
//		LED12A1_ChannelEnable( &LED1202Obj , (TypeDefChannel)(LED_CHANNEL_0<<channel),  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
//		LED12A1_AnalogDimming( &LED1202Obj , LED_CURRENT, channel,  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
//	  }
//	}
//	HAL_Delay(300);
//	}
//
//	/*** Start LED from minimum intensity to maximum intensity one by one ****/
//	for (dev = 0;dev<NumOfDev;dev++)
//	{
//	  LED12A1_ChannelDisable( &LED1202Obj , LED_CHANNEL_ALL , (TypedefEnumDevAddr)( LED_DEVICE1 + dev));
//	}
//
//	for (dev = 0;dev<NumOfDev;dev++)
//	{
//	  for (uint8_t channel = 0;channel<=11;channel++)
//	  {
//		LED12A1_ChannelEnable( &LED1202Obj , (TypeDefChannel)(LED_CHANNEL_0<<channel),  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
//		LED12A1_AnalogDimming( &LED1202Obj , LED_CURRENT, channel,  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
//		for(uint8_t cnt = 0;cnt<10;cnt++)
//		{
//		  digCurrReg =  400*cnt;
//		  LED12A1_DigitalDimming( &LED1202Obj ,&digCurrReg,channel,0,  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
//		  HAL_Delay(100);
//		}
//	  }
//
//	  HAL_Delay(300);
//	}
//}
