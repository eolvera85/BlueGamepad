#ifndef LED_H
#define LED_H

#include "Parameters.h"
#include "GPIOs.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"


void initFlashBleGamepad();
void initFlashSwitch();
void setLed();
void stopLED();

#endif