
#ifndef LEDCONTROL_H
#define LEDCONTROL_H

#include "led1202.h"

#include "datatypes.h"

void setLED(LED1202_Object_TypeDef* LED1202Obj, uint8_t x, uint8_t y, Colour col);

void disableAllLED(LED1202_Object_TypeDef* LED1202Obj, uint8_t NumOfDev);

#endif
