#include "Joysticks.h"

uint32_t map(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

uint16_t read_joystick_ns_n64(int16_t pot_value, bool reverse_axis)
{
    if (pot_value >= N64_JOYSTICK_DEADZONE_MIN && pot_value <= N64_JOYSTICK_DEADZONE_MAX)
        return ANALOG_CENTER;

    if (reverse_axis)
        return (uint16_t)map(pot_value, N64_JOYSTICK_MIN, N64_JOYSTICK_MAX, ANALOG_MAX, ANALOG_MIN);
    else
        return (uint16_t)map(pot_value, N64_JOYSTICK_MIN, N64_JOYSTICK_MAX, ANALOG_MIN, ANALOG_MAX);
}

uint16_t read_joystick_ns(adc1_channel_t adc_channel, bool reverse_axis)
{
    uint32_t raw = 0;

    for(int i = 0; i < NUMBER_SAMPLES; i++)
    {
        raw += adc1_get_raw(adc_channel);
        vTaskDelay(5);
    }

    raw = raw / NUMBER_SAMPLES;

    if(raw >= ANALOG_DEADZONE_MIN && raw <= ANALOG_DEADZONE_MAX)
        return ANALOG_CENTER;

    return reverse_axis ? (uint16_t)map(raw, ANALOG_MIN, ANALOG_MAX, ANALOG_MAX, ANALOG_MIN) : raw;
}

int16_t read_joystick_n64(int pot_value, bool reverse_axis)
{
    if (pot_value >= N64_JOYSTICK_DEADZONE_MIN && pot_value <= N64_JOYSTICK_DEADZONE_MAX)
        return JOYSTICK_CEN;

    return reverse_axis ? pot_value * -1 : pot_value;
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
        return JOYSTICK_CEN;

    return reverse_axis ? (int16_t)map(raw, ANALOG_MIN, ANALOG_MAX, JOYSTICK_MAX, JOYSTICK_MIN) : raw;
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

    raw = raw / NUMBER_SAMPLES;

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
            return raw;
    }
}