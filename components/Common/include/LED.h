#ifndef LED_H
#define LED_H

#include "Parameters.h"
#include "GPIOs.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"


EXTERNC void initFlashBleGamepad();
EXTERNC void initFlashSwitch();
EXTERNC void setLed();
EXTERNC void stopLED();

#endif