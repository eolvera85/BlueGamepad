#include "hoja_main.h"

uint32_t regread_low = 0;
int16_t countx = 0;
int16_t county = 0;
int16_t pot_x = 0;
int16_t pot_y = 0;
uint16_t joystick64_x = JOYSTICK_CEN;
uint16_t joystick64_y = JOYSTICK_CEN;

static const char *TAG = "hoja_main";

void handleEncoderX(void * pvParameters)
{
    if (gpio_get_level(GPIO_ENCODER_IX) == gpio_get_level(GPIO_ENCODER_QX))
        countx++;
    else
        countx--;
    
    if (countx > N64_JOYSTICK_MAX)
        countx = N64_JOYSTICK_MAX;

    if (countx < N64_JOYSTICK_MIN)
        countx = N64_JOYSTICK_MIN;
}

void handleEncoderY(void * pvParameters)
{
    if (gpio_get_level(GPIO_ENCODER_IY) == gpio_get_level(GPIO_ENCODER_QY))
        county++;
    else
        county--;

    if (county > N64_JOYSTICK_MAX)
        county = N64_JOYSTICK_MAX;

    if (county < N64_JOYSTICK_MIN)
        county = N64_JOYSTICK_MIN;
}

void local_button_cb(hoja_button_data_s *button_data)
{
    regread_low = REG_READ(GPIO_IN_REG);

    button_data->dpad_up        |= !util_getbit(regread_low, GPIO_BTN_DPAD_UP);
    button_data->dpad_down      |= !util_getbit(regread_low, GPIO_BTN_DPAD_DOWN);
    button_data->dpad_left      |= !util_getbit(regread_low, GPIO_BTN_DPAD_LEFT)  && util_getbit(regread_low, GPIO_BTN_START);      //Only LEFT
    button_data->dpad_right     |= !util_getbit(regread_low, GPIO_BTN_DPAD_RIGHT) && util_getbit(regread_low, GPIO_BTN_START);      //Only RIGHT
    
    button_data->button_right   |= !util_getbit(regread_low, GPIO_BTN_CIRCLE) && util_getbit(regread_low, GPIO_BTN_START);          //Only A
    button_data->button_down    |= !util_getbit(regread_low, GPIO_BTN_CROSS) && util_getbit(regread_low, GPIO_BTN_START);           //Only B
    button_data->button_up      |= !util_getbit(regread_low, GPIO_BTN_TRIANGLE);                                                    // **N64 C-LEFT
    button_data->button_left    |= !util_getbit(regread_low, GPIO_BTN_SQUARE);  

    button_data->trigger_l      |= (!util_getbit(regread_low, GPIO_BTN_L1) && util_getbit(regread_low, GPIO_BTN_START)) ||          // Only L
                                    (!util_getbit(regread_low, GPIO_BTN_START) && !util_getbit(regread_low, GPIO_BTN_CROSS));       // L = (+) + B To NES 
    button_data->trigger_r      |= (!util_getbit(regread_low, GPIO_BTN_R1) && util_getbit(regread_low, GPIO_BTN_START)) ||          // Only R
                                    (!util_getbit(regread_low, GPIO_BTN_START) && !util_getbit(regread_low, GPIO_BTN_CIRCLE));      // R = (+) + A To NES

    if (TRIGGER_TYPE == DIGITAL)
    {
        button_data->trigger_zl     |= !gpio_get_level(GPIO_BTN_L2) ||                                                              // Only ZL **N64 ZL (Z)
                                        (!util_getbit(regread_low, GPIO_BTN_START) && !util_getbit(regread_low, GPIO_BTN_L1));      // ZL = (+) + L To SNES
        button_data->trigger_zr     |= (JOYSTICK_TYPE == N64_J) ? !gpio_get_level(GPIO_BTN_R2) :                                    // **N64 C-DOWN
                                        !gpio_get_level(GPIO_BTN_R2) ||                                                             // Only ZR
                                        (!util_getbit(regread_low, GPIO_BTN_START) && !util_getbit(regread_low, GPIO_BTN_R1));      // ZR = (+) + R To SNES/N64
    }
    else
    {
        button_data->trigger_zl     |= read_trigger_button(ADC_TRIGGER_L, REVERSE_TRIGGER_L) ||                                     // Only ZL
                                        (!util_getbit(regread_low, GPIO_BTN_START) && !util_getbit(regread_low, GPIO_BTN_L1));      // ZL = (+) + L To SNES/N64
        button_data->trigger_zr     |= read_trigger_button(ADC_TRIGGER_R, REVERSE_TRIGGER_R) ||                                     // Only ZR
                                        (!util_getbit(regread_low, GPIO_BTN_START) && !util_getbit(regread_low, GPIO_BTN_R1));      // ZR = (+) + R To SNES/N64
    }

    button_data->button_select  |= !util_getbit(regread_low, GPIO_BTN_SELECT);                                                      // **N64 C-RIGHT
    button_data->button_start   |= !util_getbit(regread_low, GPIO_BTN_START) && 
                                    util_getbit(regread_low, GPIO_BTN_CIRCLE) && 
                                    util_getbit(regread_low, GPIO_BTN_CROSS) && 
                                    util_getbit(regread_low, GPIO_BTN_L1) && 
                                    util_getbit(regread_low, GPIO_BTN_R1) &&
                                    util_getbit(regread_low, GPIO_BTN_DPAD_LEFT) &&
                                    util_getbit(regread_low, GPIO_BTN_DPAD_RIGHT);                                                  // Only Start

    button_data->button_capture |= !util_getbit(regread_low, GPIO_BTN_CAPTURE) ||                                                   // Only Capture
                                    (!util_getbit(regread_low, GPIO_BTN_START) && !util_getbit(regread_low, GPIO_BTN_DPAD_LEFT));   // Capture = (+) + LEFT
    button_data->button_home    |= !util_getbit(regread_low, GPIO_BTN_HOME) ||                                                      // Only Home
                                    (!util_getbit(regread_low, GPIO_BTN_START) && !util_getbit(regread_low, GPIO_BTN_DPAD_RIGHT));  // Home = (+) + RIGHT

    button_data->button_stick_left  |= JOYSTICK_TYPE != N64_J ? !util_getbit(regread_low, GPIO_BTN_L3) : 
                                        (!util_getbit(regread_low, GPIO_BTN_START) && !util_getbit(regread_low, GPIO_BTN_R1));      // **N64 ZR
    button_data->button_stick_right |= JOYSTICK_TYPE != N64_J ? !util_getbit(regread_low, GPIO_BTN_R3) : false;                 

    regread_low = 0;
}

void local_analog_cb(hoja_analog_data_s *analog_data)
{
    analog_data->ls_x = read_joystick_ns(ADC_STICK_LX, RBS_REVERSE_AXIS_LX);
    analog_data->ls_y = read_joystick_ns(ADC_STICK_LY, RBS_REVERSE_AXIS_LY);
    analog_data->rs_x = PRIMARY_CONTROLLER == N64 ? ANALOG_CENTER : read_joystick_ns(ADC_STICK_RX, RBS_REVERSE_AXIS_RX);
    analog_data->rs_y = PRIMARY_CONTROLLER == N64 ? ANALOG_CENTER : read_joystick_ns(ADC_STICK_RY, RBS_REVERSE_AXIS_RY);

}

void local_analog_cb_n64(hoja_analog_data_s *analog_data)
{
    pot_x = countx;
    pot_y = county;

    joystick64_x = read_joystick_ns_n64(pot_x, RBS_N64_REVERSE_AXIS_LX);
    joystick64_y = read_joystick_ns_n64(pot_y, RBS_N64_REVERSE_AXIS_LY);

    ESP_LOGI(TAG, "%sX: %d / N64:%d", "Joystick L", joystick64_x, pot_x);
    ESP_LOGI(TAG, "%sY: %d / N64:%d", "Joystick L", joystick64_y, pot_y);

    analog_data->ls_x = joystick64_x;
    analog_data->ls_y = joystick64_y;
    analog_data->rs_x = ANALOG_CENTER;
    analog_data->rs_y = ANALOG_CENTER;
}

void local_event_cb(hoja_event_type_t type, uint8_t evt, uint8_t param)
{
    const char* TAG = "local_event_cb";
    uint32_t regEvent = 0;

    if (type == HOJA_EVT_BT && evt == HEVT_BT_DISCONNECT)
    {
        hoja_set_core(HOJA_CORE_NS);
        
        while (true)
        {
            regEvent = regread_low == 0 ? REG_READ(GPIO_IN_REG) : regread_low;

            if (!util_getbit(regread_low, GPIO_BTN_START))
            {
                while(hoja_start_core() != HOJA_OK)
                    vTaskDelay(100 / portTICK_PERIOD_MS);

                break;
            }
        }
    }
}

void sync_button_task(void * pvParameters)
{
    uint32_t regSync = 0;

    while (true)
    {
        regSync = regread_low == 0 ? REG_READ(GPIO_IN_REG) & PIN_BIT_MASK : regread_low;

        syncController(regSync);

        vTaskDelay(5);
    }
}

void hoja_switch_init()
{
    setup_gpios();
    initFlashSwitch();

    xTaskCreatePinnedToCore(sync_button_task, "sync_button_task", 2048, NULL, 0, NULL, 0);

    hoja_register_button_callback(local_button_cb);

    if (JOYSTICK_TYPE == PRO_J)
        hoja_register_analog_callback(local_analog_cb);
    else
    {
        gpio_install_isr_service(0);
        gpio_isr_handler_add(GPIO_ENCODER_IX, handleEncoderX, NULL);
        gpio_isr_handler_add(GPIO_ENCODER_IY, handleEncoderY, NULL);

        hoja_register_analog_callback(local_analog_cb_n64);
    }

    hoja_register_event_callback(local_event_cb);

    if (hoja_init() != HOJA_OK)
        ESP_LOGE(TAG, "Failed to initialize HOJA.");
    else
    {
        hoja_set_core(HOJA_CORE_NS);

        while(hoja_start_core() != HOJA_OK)
            vTaskDelay(100 / portTICK_PERIOD_MS);

        setLed();
    }
}