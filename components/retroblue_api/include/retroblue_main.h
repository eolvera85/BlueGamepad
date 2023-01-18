#ifndef RETROBLUE_MAIN_H
#define RETROBLUE_MAIN_H

#include "driver/adc.h"
#include "esp_system.h"
#include "sdkconfig.h"
#include "soc/gpio_reg.h"

#include "GPIOs.h"
#include "Buttons.h"
#include "Joysticks.h"
#include "Controller.h"
#include "LED.h"

#include "retroblue_api.h"

void retroblue_init();

#endif
