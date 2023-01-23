#include "LED.h"

TaskHandle_t BlinkHandle = NULL;
gpio_num_t gpio_led;

void led_task(void * pvParameters)
{
    while(true) 
    {
        gpio_set_level(gpio_led, 1);
        vTaskDelay(30);
        gpio_set_level(gpio_led, 0);
        vTaskDelay(30);
    }
}

void initFlashBleGamepad()
{
    gpio_set_level(GPIO_LED_SYNC, 0);

    gpio_led = GPIO_LED_SYNC_BLE;
    xTaskCreatePinnedToCore(led_task, "led_task", 1024, NULL, 5, &BlinkHandle, 1);
}

void initFlashSwitch()
{
    gpio_set_level(GPIO_LED_SYNC_BLE, 0);

    gpio_led = GPIO_LED_SYNC;
    xTaskCreatePinnedToCore(led_task, "led_task", 1024, NULL, 5, &BlinkHandle, 1);
}

void stopLED()
{
    gpio_set_level(gpio_led, 0);
}

void setLed()
{
    if (gpio_led == GPIO_LED_SYNC)
        vTaskDelay(1000);
    
    if (BlinkHandle != NULL)
        vTaskDelete(BlinkHandle);

    BlinkHandle = NULL;
    gpio_set_level(gpio_led, 1);
}
