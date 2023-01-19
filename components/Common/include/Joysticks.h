#ifndef JOYSTICKS_H
#define JOYSTICKS_H

#include "Parameters.h"
#include "GPIOs.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

// Max, Min  joystick potentiometers
#define ANALOG_MIN              0
#define ANALOG_MAX              4095
#define ANALOG_CENTER           2048
#define ANALOG_DEADZONE_FAC     25 // Factor
#define ANALOG_DEADZONE_MIN     (ANALOG_CENTER - ANALOG_DEADZONE_FAC)    
#define ANALOG_DEADZONE_MAX     (ANALOG_CENTER + ANALOG_DEADZONE_FAC)

#define ANALOG_DEADZONE_TRIGGER     1500 // ANALOG_DEADZONE_FAC

#if JOYSTICK_TYPE == N64_J
    #define JOYSTICK_MIN                -39 
    #define JOYSTICK_MAX                39
    #define JOYSTICK_CEN                0
#else
    #define JOYSTICK_MIN                0
    #define JOYSTICK_MAX                4095
    #define JOYSTICK_CEN                2048
#endif

#define N64_JOYSTICK_MAX             39
#define N64_JOYSTICK_MIN             -39
#define N64_JOYSTICK_DEADZONE_MAX    4
#define N64_JOYSTICK_DEADZONE_MIN    -4

#define NUMBER_SAMPLES          1
#define NUM_OF_JOYSTICKS        2
#define NUM_OF_TRIGGERS         2


struct joysticks_s
{
    char name[12];
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
    char name[12];
    adc1_channel_t adc_channel;
    int16_t current_state_trigger;
    int16_t previous_state_trigger;
    uint8_t physical_button;
    bool reverse_trigger;
};

struct joystick_64_s
{
    char name[12];
    int16_t current_state_pot_x;
    int16_t previous_state_pot_x;
    int16_t current_state_pot_y;
    int16_t previous_state_pot_y;
};

extern struct joystick_64_s joystick64;
extern struct joysticks_s joysticks[NUM_OF_JOYSTICKS];
extern struct triggers_s triggers[NUM_OF_TRIGGERS];

EXTERNC uint16_t read_joystick_ns(adc1_channel_t adc_channel, bool reverse_axis);
EXTERNC uint16_t read_joystick_ns_n64(int16_t pot_value, bool reverse_axis);
EXTERNC int16_t read_joystick(adc1_channel_t adc_channel, bool reverse_axis);
EXTERNC int16_t read_joystick_n64(int pot_value, bool reverse_axis);
EXTERNC bool read_trigger_button(adc1_channel_t adc_channel, bool reverse_trigger);
EXTERNC int16_t read_trigger(adc1_channel_t adc_channel, bool reverse_trigger);

#endif