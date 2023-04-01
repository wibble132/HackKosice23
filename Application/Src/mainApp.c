
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

#include "iks01a3_motion_sensors.h"
#include "iks01a3_conf.h"

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
	task3();
}

void task3() {

	const int HEIGHT = 6;
	const int WIDTH = 4;

	const int h = 3 * (HEIGHT + 1);
	const int str_bsh[21][4] = {
		{0, 0, 0, 0},
		/*
		 * Character b
		 */
		{1, 0, 0, 0},
		{1, 0, 0, 0},
		{1, 0, 0, 0},
		{1, 1, 1, 0},
		{1, 0, 0, 1},
		{1, 1, 1, 0},

		{0, 0, 0, 0},
		/*
		 * Character S
		 */
		{0, 1, 1, 0},
		{1, 0, 0, 1},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{1, 0, 0, 1},
		{0, 1, 1, 0},

		{0, 0, 0, 0},
		/*
		 * Character h
		 */
		{1, 0, 0, 0},
		{1, 0, 0, 1},
		{1, 0, 0, 0},
		{1, 1, 1, 0},
		{1, 0, 0, 1},
		{1, 0, 0, 1}
	};



	// enable the motion sensor
	IKS01A3_MOTION_SENSOR_Init(IKS01A3_LSM6DSO_0, MOTION_GYRO);
	IKS01A3_MOTION_SENSOR_Enable(IKS01A3_LSM6DSO_0, MOTION_GYRO);

	int curr = 0; // where we are

	while(true) {
		IKS01A3_MOTION_SENSOR_Axes_t angular;
		IKS01A3_MOTION_SENSOR_GetAxes(IKS01A3_LSM6DSO_0, MOTION_GYRO, &angular);

		double gyro_x = angular.x / 1e4; // normalise the gyro data


		// Cut off gyro_x at the bounds
		if(gyro_x > 100)
			gyro_x = 100;
		if(gyro_x < -100)
			gyro_x = -100;

		// Set threshold for tilt
		const double my_threshold = 30 * 100 / 180; // 30 degree threshold

		if(gyro_x > my_threshold) {
			curr = (curr + 1) % h;
		}
		else if(gyro_x < -my_threshold) {
			curr = (curr - 1 + h) % h;
		}

		// display the letters on the screen
		for(int x = 0; x < WIDTH; x++) {
			for(int y = 0; y < WIDTH; y++) {
				int my_x = (x + curr) % h;
				enableLED(&LED1202Obj, my_x, y);
				Colour my_col;

				if(str_bsh[my_x][y] == 1) {
					// We colour this white
					my_col = make_colour(50, 50, 50);
				}

				if(0 <= my_x && my_x <= WIDTH) {
					my_col = make_colour(14 - 2 * my_x, 2 * my_x, 0);
				}
				else if(WIDTH + 1 <= my_x && my_x <= 2 * (WIDTH+1)) {
					my_col = make_colour(0, 28 - 2 * my_x, 2 * my_x - 14);
				}
				else if(2 * (WIDTH+1) <= my_x && my_x < 3 * (WIDTH+1)) {
					my_col = make_colour(2 * my_x - 28, 0, 42 - 2 * my_x);
				}

				setLED(&LED1202Obj, x, y, my_col);
			}
		}
	}
}

void task2() {

	// initialise LEDs as all red
	disableAllLED( &LED1202Obj, NumOfDev );
	const int letters_buffer[27] =  {0, 0, 0, 0, 7, 9, 7, 1, 1, 1, 0, 6, 9, 4, 2, 9, 6, 0, 9, 9, 7, 1, 1, 1, 0, 0, 0};
//	const bool device3_buffer[27] = {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0};
//	const bool device2_buffer[27] = {0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0};
//	const bool device1_buffer[27] = {0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0};
//	const bool device0_buffer[27] = {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0};

	bool enabled_flag = false;

	const Colour bright_white = make_colour(50, 50, 50);
//	const Colour black = make_colour(0, 0, 0);

	while (1) {
		uint8_t t = 3;
		while (t < 27) {
			for (uint8_t x = 0; x < 4; ++x) {
				for (uint8_t y = 0; y < 4; ++y) {
					if (!enabled_flag) {
						enableLED(&LED1202Obj, x, y);
//						LED12A1_ChannelEnable( &LED1202Obj, (TypeDefChannel) (LED_CHANNEL_0<<(3*x)), (TypedefEnumDevAddr)(LED_DEVICE1 + y));
//						LED12A1_ChannelEnable( &LED1202Obj, (TypeDefChannel) (LED_CHANNEL_0<<(3*x+1)), (TypedefEnumDevAddr)(LED_DEVICE1 + y));
//						LED12A1_ChannelEnable( &LED1202Obj, (TypeDefChannel) (LED_CHANNEL_0<<(3*x+2)), (TypedefEnumDevAddr)(LED_DEVICE1 + y));
					}
					if ((letters_buffer[t - y] >> x) & 1) {
						setLED(&LED1202Obj, x, y, bright_white);
					}
					else {
						Colour myCol;
						uint8_t t_off = (t - y) % 24;
						if (t_off < 8) {
							myCol = make_colour(8 - t_off, t_off, 0);
						}
						else if (t_off < 16) {
							myCol = make_colour(0, 16 - t_off, t_off - 8);
						}
						else {
							myCol = make_colour(t_off - 16, 0, 24 - t_off);
						}
						setLED(&LED1202Obj, x, y, myCol);
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


void task1() {

	while(1) {
		uint8_t t = 0;
		Colour myCol;

		while (t < 30) { // (10, 0, 0) -> (0, 10, 0)
			if (t < 10) {
				myCol = make_colour(10-t, t, 0);
			}
			else if (t < 20) {
				myCol = make_colour(0, 20-t, t-10);
			}
			else {
				myCol = make_colour(t-20, 0, 30-t);
			}
			for (uint8_t x = 0; x < 4; ++x) {
				for (uint8_t y = 0; y < 4; ++y) {
					enableLED(&LED1202Obj, x, y);
					setLED(&LED1202Obj, x, y, myCol);
				}
			}
			HAL_Delay(20);
			t++;
		}
	}
//	for (uint8_t x = 0; x < 4; ++x) {
//		for (uint8_t y = 0; y < 4; ++y) {
//			setLED( &LED1202Obj, x, y, black);
//			HAL_Delay(1000);
//		}
//	}
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
