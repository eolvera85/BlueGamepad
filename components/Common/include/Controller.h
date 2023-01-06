#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "esp_system.h"
#include "esp_log.h"

#include "nvs.h"
#include "nvs_flash.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

enum CONTROLLER_TYPE_E { RETROBLUE = 0, BLEGAMEPAD = 1 };
enum PRIMARY_CONTROLLER_E { PRO = 0x03, NES = 0x09, SNES = 0x0B, N64 = 0x0C };
enum SECONDARY_CONTROLLER_E { PROCON = 0x02, N64CLA = 0x11 };

// Parameters Type Switch Controller
#define PRIMARY_CONTROLLER      N64         // Values: Enum -> PRIMARY_CONTROLLER_E
#define SECONDARY_CONTROLLER    N64CLA      // Values: Enum -> SECONDARY_CONTROLLER_E
// End Parameters

void syncController(uint32_t regReadSync);
CONTROLLER_TYPE_E readController();

#endif
