#include "GPIOs.h"
#include "Buttons.h"
#include "Joysticks.h"

uint64_t PIN_BIT_MASK;

void setup_gpios()
{
    PIN_BIT_MASK = GPIO_BITMASK_BUTTONS;

    if (JOYSTICK_TYPE == PRO_J || TRIGGER_TYPE == ANALOG)
        adc1_config_width(ADC_WIDTH_BIT_12); //ADC_WIDTH_BIT_DEFAULT - ADC_WIDTH_BIT_12


    if(TRIGGER_TYPE == ANALOG)
    {
        adc1_config_channel_atten(ADC_TRIGGER_L, ADC_ATTEN_DB_11);
        adc1_config_channel_atten(ADC_TRIGGER_R, ADC_ATTEN_DB_11); 

        PIN_BIT_MASK = GPIO_BITMASK_TRIGGERS;       
    }

    if (JOYSTICK_TYPE == PRO_J)
    {
        adc1_config_channel_atten(ADC_STICK_LX, ADC_ATTEN_DB_11);
        adc1_config_channel_atten(ADC_STICK_LY, ADC_ATTEN_DB_11);
        adc1_config_channel_atten(ADC_STICK_RX, ADC_ATTEN_DB_11);
        adc1_config_channel_atten(ADC_STICK_RY, ADC_ATTEN_DB_11);
    }
    else //N64 Type
    {
        PIN_BIT_MASK = PIN_BIT_MASK | GPIO_BITMASK_N64;

        gpio_config_t io_conf_n64 = {};
        io_conf_n64.mode = GPIO_MODE_INPUT;
        io_conf_n64.pin_bit_mask = GPIO_BITMASK_N64_INTR;
        io_conf_n64.pull_down_en = GPIO_PULLDOWN_DISABLE;
        io_conf_n64.pull_up_en = GPIO_PULLUP_ENABLE;
        io_conf_n64.intr_type = GPIO_INTR_ANYEDGE; //GPIO_INTR_LOW_LEVEL //GPIO_INTR_ANYEDGE;
        gpio_config(&io_conf_n64);
    }
      
    //Buttons GND
    gpio_config_t io_conf = {};
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = PIN_BIT_MASK;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&io_conf);


    //LED 
    gpio_config_t io_conf_led = {};
    io_conf_led.mode = GPIO_MODE_OUTPUT;
    io_conf_led.pin_bit_mask = GPIO_BITMASK_LED;
    io_conf_led.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf_led.pull_up_en = GPIO_PULLUP_DISABLE;
    io_conf_led.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&io_conf_led);    
}