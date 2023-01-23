#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "esp_system.h"
#include "esp_log.h"

#include "nvs.h"
#include "nvs_flash.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "Parameters.h"
#include "LED.h"

void syncController(uint32_t regReadSync);
CONTROLLER_TYPE_E readController();

#endif
