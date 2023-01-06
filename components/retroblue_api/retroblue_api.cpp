#include "retroblue_api.h"

SemaphoreHandle_t xSemaphore;

rb_err_t rb_api_init()
{
    const char* TAG = "rb_api_init";
	esp_err_t ret;

    xSemaphore = xSemaphoreCreateMutex();

    // Initialize NVS
    ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    if (!rb_settings_init())
    {
        ESP_LOGE(TAG, "Settings failed to load.");
        return RB_FAIL;
    }

    ESP_LOGI(TAG, "Retroblue API initialized.");

    return RB_OK;
}

rb_err_t rb_api_startController()
{
    const char* TAG = "rb_api_startController";

    ESP_LOGI(TAG, "Attempting Nintendo Switch Core start...");

    rb_api_init();
    rbc_core_ns_start();

    return RB_OK;
}

// Pointer to hold callback
input_update_callback rb_button_cb;
input_update_callback rb_stick_cb;

// Function to register callback function for setting buttons
rb_err_t rb_register_button_callback(input_update_callback func)
{
    if (func == NULL) return RB_FAIL;

    rb_button_cb = func;

    return RB_OK;
}

// Function to register callback function for setting sticks
rb_err_t rb_register_stick_callback(input_update_callback func)
{
    if (func == NULL) return RB_FAIL;

    rb_stick_cb = func;

    return RB_OK;
}

