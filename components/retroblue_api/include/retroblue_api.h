#ifndef RETROBLUE_API_H
#define RETROBLUE_API_H

#include "retroblue_backend.h"
#include "retroblue_settings.h"
#include "rbc_switch_core.h"

extern SemaphoreHandle_t xSemaphore;

/* Functions relating to the API itself */

// Initialize the API.
// This loads any settings saved and initializes flash storage.
rb_err_t rb_api_init();

// Starts the controller. You MUST set the core before running this.
rb_err_t rb_api_startController();

// Vars for callbacks
typedef void (*input_update_callback) (void);
extern input_update_callback rb_button_cb;
extern input_update_callback rb_stick_cb;

// Function to register callback function for setting buttons
// This is called by the core at an appropriate rate to help
// avoid weird timing issues.
rb_err_t rb_register_button_callback(input_update_callback func);
// Function to register callback function for setting sticks
rb_err_t rb_register_stick_callback(input_update_callback func);

#endif
