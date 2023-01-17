#ifndef JOSTICKS_H
#define JOSTICKS_H

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
#define NUM_OF_JOSTICKS         2
#define NUM_OF_TRIGGERS         2


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
    int16_t current_state_pot_x;
    int16_t previous_state_pot_x;
    int16_t current_state_pot_y;
    int16_t previous_state_pot_y;
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