#ifndef RBC_SWITCH_CORE_H
#define RBC_SWITCH_CORE_H

// Include any necessary includes from RetroBlue backend
#include "rbc_err.h"
#include "retroblue_backend.h"

// Library for handling commands and sub-commands
#include "rbc_switch_comms.h"

// Libaray for handling input report
// generation
#include "rbc_switch_reports.h"

// Library for handling controller input
// buttons, sticks, sixaxis
#include "rbc_switch_input.h"

// Library for handling controller
// configuration and loaded controller
// data.
#include "rbc_switch_controller.h"

// Library for handling SPI read commands
#include "rbc_switch_spi.h"

// Library for interpreting vibration data
// and translating it for the RetroBlue API
//#include "rbc_switch_vibration.h"

/* Define controller global elements */
#define NS_FW_PRIMARY       0x03
#define NS_FW_SECONDARY     0x80

#define NS_COLOR_SET        0x02

/* Define color defaults */
#define NS_COLOR_BLACK      0x00

extern uint8_t ns_snes_controller_type;

// Parameters for bluetooth function
struct nscore_param_s
{
    esp_hidd_app_param_t app_param;
    esp_hidd_qos_param_t both_qos;
    SemaphoreHandle_t ns_semaphore;
};

// Instance of NS Core parameters
extern struct nscore_param_s ns_core_param;

// HID descriptor instances
extern uint8_t hid_descriptor_ns_core[102];
extern uint8_t hid_descriptor_ns_core_len;

// Handler for vTask for report mode changes.
extern TaskHandle_t ns_ReportModeHandle;
// Hander for the vTask for keeping a pulse.
//TaskHandle_t ns_BlankReportsHandle;

// Start the Nintendo Switch core and enter pairing mode.
rb_err_t rbc_core_ns_start();

rb_err_t rbc_core_savepairing(uint8_t* host_addr);

#endif