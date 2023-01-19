#include "Controller.h"
#include "GPIOs.h"
#include "Buttons.h"

int16_t controller_type;
uint32_t timeLast = 0;

void nextController()
{
    const char* TAG = "saveController";

    controller_type = controller_type == BLEGAMEPAD ? SWITCH : BLEGAMEPAD;

    //Save next controller
    nvs_handle_t my_handle_write;
    nvs_flash_init();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    nvs_open("storage", NVS_READWRITE, &my_handle_write);
    nvs_set_i16(my_handle_write, "controller_type", controller_type);
    nvs_commit(my_handle_write);
    nvs_close(my_handle_write);

    ESP_LOGI(TAG, "Saved Next Controller Type: %d", controller_type);

    stopLED();
    
    esp_restart();    
}

CONTROLLER_TYPE_E readController()
{
    const char* TAG = "readController";

    nvs_handle_t my_handle_read;
    nvs_flash_init();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    nvs_open("storage", NVS_READWRITE, &my_handle_read);
    nvs_get_i16(my_handle_read, "controller_type", &controller_type);
    nvs_close(my_handle_read);

    ESP_LOGI(TAG, "Read Controller Type: %d", controller_type);

    return (CONTROLLER_TYPE_E)controller_type;
}

void syncController(uint32_t regReadSync)
{
    if (read_button(regReadSync, GPIO_BTN_SYNC))
        nextController();

    if (read_button(regReadSync, GPIO_BTN_START))
    {
        if (timeLast == 0)
            timeLast = xTaskGetTickCount();
        
        if ((xTaskGetTickCount() - timeLast) >= 10000)
                nextController();
    }
    else
        timeLast = 0;
}