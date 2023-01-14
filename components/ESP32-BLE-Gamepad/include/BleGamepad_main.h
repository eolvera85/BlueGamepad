#ifndef BLEGAMEPAD_MAIN_H
#define BLEGAMEPAD_MAIN_H


#include "GPIOs.h"
#include "Buttons.h"
#include "Josticks.h"
#include "Controller.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "soc/gpio_reg.h"

#include "BleGamepad.h"
#include "BleGamepadConfiguration.h"

extern BleGamepad bleGamepad;

void blegamepad_init();

#endif

