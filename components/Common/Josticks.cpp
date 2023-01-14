#include "Josticks.h"

uint32_t map(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

uint16_t read_jostick_n64_retroblue(int pot_value, bool reverse_axis)
{
    if (pot_value >= N64_JOSTICK_DEADZONE_MIN && pot_value <= N64_JOSTICK_DEADZONE_MAX)
        return (uint16_t)ANALOG_CENTER;

    if (reverse_axis)
        return (uint16_t)map(pot_value, N64_JOSTICK_MIN, N64_JOSTICK_MAX, ANALOG_MAX, ANALOG_MIN);
    else
        return (uint16_t)map(pot_value, N64_JOSTICK_MIN, N64_JOSTICK_MAX, ANALOG_MIN, ANALOG_MAX);
}

uint16_t read_jostick_retroblue(adc1_channel_t adc_channel, bool reverse_axis)
{
    if (reverse_axis)
        return (uint16_t)map(adc1_get_raw(adc_channel), ANALOG_MIN, ANALOG_MAX, ANALOG_MAX, ANALOG_MIN);
    else
        return (uint16_t)map(adc1_get_raw(adc_channel), ANALOG_MIN, ANALOG_MAX, ANALOG_MIN, ANALOG_MAX);
}

int16_t read_joystick_n64(int pot_value, bool reverse_axis)
{
    if (pot_value >= N64_JOSTICK_DEADZONE_MIN && pot_value <= N64_JOSTICK_DEADZONE_MAX)
        return JOYSTICK_CEN; //pot_value = 0;

    if (reverse_axis)
        return (int16_t)map(pot_value, N64_JOSTICK_MIN, N64_JOSTICK_MAX, JOYSTICK_MAX, JOYSTICK_MIN);
    else
        return (int16_t)map(pot_value, N64_JOSTICK_MIN, N64_JOSTICK_MAX, JOYSTICK_MIN, JOYSTICK_MAX);
}

int16_t read_joystick(adc1_channel_t adc_channel, bool reverse_axis)
{
    uint32_t raw = 0;

    for(int i = 0; i < NUMBER_SAMPLES; i++)
    {
        raw += adc1_get_raw(adc_channel);
        vTaskDelay(5);
    }

    raw = raw / NUMBER_SAMPLES;

    if(raw >= ANALOG_DEADZONE_MIN && raw <= ANALOG_DEADZONE_MAX)
        raw = ANALOG_CENTER;

    if (reverse_axis)
        return (int16_t)map(raw, ANALOG_MIN, ANALOG_MAX, JOYSTICK_MAX, JOYSTICK_MIN);
    else
        return (int16_t)map(raw, ANALOG_MIN, ANALOG_MAX, JOYSTICK_MIN, JOYSTICK_MAX);
}

bool read_trigger_button(adc1_channel_t adc_channel, bool reverse_trigger)
{
    return (bool)read_trigger(adc_channel, reverse_trigger);
}

int16_t read_trigger(adc1_channel_t adc_channel, bool reverse_trigger)
{
    uint32_t raw = 0;

    for(int i = 0; i < NUMBER_SAMPLES; i++)
    {
        raw += adc1_get_raw(adc_channel);
        vTaskDelay(5);
    }

    if (reverse_trigger)
    {
        if (raw >= (ANALOG_MAX - ANALOG_DEADZONE_TRIGGER) && raw <= ANALOG_MAX)
            return JOYSTICK_MIN;
        else
            return (int16_t)map(raw, ANALOG_MIN, ANALOG_MAX, JOYSTICK_MAX, JOYSTICK_MIN);
    }
    else
    {
        if (raw > -1 && raw <= ANALOG_DEADZONE_TRIGGER)
            return JOYSTICK_MIN;
        else
            return (int16_t)map(raw, ANALOG_MIN, ANALOG_MAX, JOYSTICK_MIN, JOYSTICK_MAX);
    }
}