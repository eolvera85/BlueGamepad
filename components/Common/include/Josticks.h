#ifndef JOSTICKS_H
#define JOSTICKS_H

#include "GPIOs.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

//ENUMS
enum JOSTICK_TYPE_E { PRO_J = 1, N64_J = 2 };
enum TRIGGER_TYPE_E { DIGITAL = 0, ANALOG = 1 };

// Max, Min  joystick potentiometers
#define ANALOG_MIN              0
#define ANALOG_MAX              4095
#define ANALOG_CENTER           (ANALOG_MAX / 2)
#define ANALOG_DEADZONE_FAC     25 // Factor
#define ANALOG_DEADZONE_MIN     (ANALOG_CENTER - ANALOG_DEADZONE_FAC)    
#define ANALOG_DEADZONE_MAX     (ANALOG_CENTER + ANALOG_DEADZONE_FAC)

#define ANALOG_DEADZONE_TRIGGER     1500 // ANALOG_DEADZONE_FAC

#define JOYSTICK_MIN                0 
#define JOYSTICK_MAX                32767

#define N64_JOSTICK_MAX             39
#define N64_JOSTICK_MIN             -39
#define N64_JOSTICK_DEADZONE_MAX    4
#define N64_JOSTICK_DEADZONE_MIN    -4

#define NUMBER_SAMPLES          1
#define NUM_OF_JOSTICKS         2
#define NUM_OF_TRIGGERS         2

// ************ From here change values to configure the controls ************
// Begin Parameters Controller Type (Change *Jostick PRO/N64 - Buttons L2 R2  )
#define JOSTICK_TYPE            N64_J       // VALUES: Enum JOSTICK_TYPE_E

// Josticks Switch
#define RBS_REVERSE_AXIS_LX     false
#define RBS_REVERSE_AXIS_LY     true
#define RBS_REVERSE_AXIS_RX     true
#define RBS_REVERSE_AXIS_RY     false
// Josticks Switch N64
#define RBS_N64_REVERSE_AXIS_LX     false
#define RBS_N64_REVERSE_AXIS_LY     false

// Josticks BleGamepad
#define BLE_REVERSE_AXIS_LX     false
#define BLE_REVERSE_AXIS_LY     false
#define BLE_REVERSE_AXIS_RX     true
#define BLE_REVERSE_AXIS_RY     true
// Josticks BleGamepad N64
#define BLE_N64_REVERSE_AXIS_LX     false
#define BLE_N64_REVERSE_AXIS_LY     true

// Triggers
#define TRIGGER_TYPE            DIGITAL      // VALUES: Enum TRIGGER_TYPE_E
#define REPORT_TRIGGERS_AS      DIGITAL     // VALUES: Enum TRIGGER_TYPE_E
#define REVERSE_TRIGGER_L       true
#define REVERSE_TRIGGER_R       true
// End Parameters
// *******************************************************************************

struct josticks_s
{
    char name[10];
    adc1_channel_t adc_channel_axis_x;
    int16_t current_state_axis_x;
    int16_t previous_state_axis_x;
    bool reverse_axis_x;
    adc1_channel_t adc_channel_axis_y;
    int16_t current_state_axis_y;
    int16_t previous_state_axis_y;
    bool reverse_axis_y;
};

struct triggers_s
{
    char name[10];
    adc1_channel_t adc_channel;
    int16_t current_state_trigger;
    int16_t previous_state_trigger;
    uint8_t physical_button;
    bool reverse_trigger;
};

struct jostick_64_s
{
    char name[10];
    int16_t current_state_axis_x;
    int16_t previous_state_axis_x;
    bool reverse_axis_x;
    int16_t current_state_axis_y;
    int16_t previous_state_axis_y;
    bool reverse_axis_y;    
};

extern jostick_64_s jostick64;
extern josticks_s josticks[NUM_OF_JOSTICKS];
extern triggers_s triggers[NUM_OF_TRIGGERS];

uint16_t read_jostick_retroblue(adc1_channel_t adc_channel, bool reverse_axis);
uint16_t read_jostick_n64_retroblue(int pot_value, bool reverse_axis);
int16_t read_joystick(adc1_channel_t adc_channel, bool reverse_axis);
int16_t read_joystick_n64(int pot_value, bool reverse_axis);
bool read_trigger_button(adc1_channel_t adc_channel, bool reverse_trigger);
int16_t read_trigger(adc1_channel_t adc_channel, bool reverse_trigger);

#endif