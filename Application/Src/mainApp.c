
#include "mainApp.h"

#include "stm32g0xx_hal.h"
#include <callbacks.h>
#include <stdbool.h>
#include "led12a1.h"
#include <bridge.h>
#include <callbacks.h>
#include <stdbool.h>
#include "led1202.h"
#include "led12a1.h"


void mainApp_setup() {

#ifndef GUI_MODE
  uint16_t  digCurrReg = 0;
  uint8_t dev = 0;
  uint8_t cnt;
#endif

  __HAL_TIM_CLEAR_IT(&htim2,TIM_SR_UIF);
  __HAL_TIM_CLEAR_IT(&htim3,TIM_SR_UIF);

  /*Wait Time assuring a properly steady state condition */
  HAL_Delay(100);

  interface_mode = UART_MODE;	/*Set a known condition */

  /*Clear Buffer*/
  ClearBuffers();

  /*LED12A1 Initialization*/
  LED12A1_Init();

  /*Scan for Number of available LED1202 ICs*/
  NumOfDev =  LED12A1_ScanAvailDevice( &LED1202Obj ,DevAddrArray);
#ifdef GUI_MODE
  HAL_UART_Receive_IT(&huart2,&bridge_OUT_buffer[0],64);
#else
  /*Enable available LED1202 ICs*/
  for (uint8_t dev = 0;dev<NumOfDev;dev++)
  {
	LED12A1_DeviceEnable(&LED1202Obj , (TypedefEnumDevAddr)(LED_DEVICE1 + dev));
	LED12A1_ChannelDisable( &LED1202Obj , LED_CHANNEL_ALL , (TypedefEnumDevAddr)( LED_DEVICE1 + dev));

  }
#endif

}

void mainApp_mainLoop() {
#ifdef GUI_MODE
    WaitForCommand();
#else

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
		LED12A1_AnalogDimming( &LED1202Obj , MAX_CH_CURRENT, channel,  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
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
		LED12A1_AnalogDimming( &LED1202Obj , MAX_CH_CURRENT, channel,  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
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
		LED12A1_AnalogDimming( &LED1202Obj , MAX_CH_CURRENT, channel,  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
		for(uint8_t cnt = 0;cnt<10;cnt++)
		{
		  digCurrReg =  400*cnt;
		  LED12A1_DigitalDimming( &LED1202Obj ,&digCurrReg,channel,0,  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
		  HAL_Delay(100);
		}
	  }

	  HAL_Delay(300);
	}
	#endif
}
