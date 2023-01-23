#include "BleGamepad_main.h"

BleGamepad bleGamepad("BlueGamepad", "EOLVERA85", 100);

uint32_t regread_button = 0;
uint32_t regread_dpad = 0;
int count_x = 0;
int count_y = 0;

signed char current_dpad = DPAD_CENTERED;
signed char previous_dpad = DPAD_CENTERED;


joystick_64_s joystick64 = 
{
    "Joystick L", JOYSTICK_CEN, JOYSTICK_CEN, JOYSTICK_CEN, JOYSTICK_CEN
};

buttons_s buttons[NUM_PHYSICAL_BTNS] = 
{
    { "Circle", GPIO_BTN_CIRCLE, BTN_NUM_A, false, false },
    { "Cross", GPIO_BTN_CROSS, BTN_NUM_B, false, false },
    { "Triangle", GPIO_BTN_TRIANGLE, BTN_NUM_X, false, false },    
    { "Square", GPIO_BTN_SQUARE, BTN_NUM_Y, false, false },    
    
    { "L1", GPIO_BTN_L1, BTN_NUM_L1, false, false },
    { "R1", GPIO_BTN_R1, BTN_NUM_R1, false, false },

    { "L2", GPIO_BTN_L2, BTN_NUM_L2, false, false },
    { "R2", GPIO_BTN_R2, BTN_NUM_R2, false, false },

    { "Select", GPIO_BTN_SELECT, BTN_NUM_SELECT, false, false },
    { "Start", GPIO_BTN_START, BTN_NUM_START, false, false },
    
    { "L3", GPIO_BTN_L3, BTN_NUM_L3, false, false },
    { "R3", GPIO_BTN_R3, BTN_NUM_R3, false, false },

    { "Capture", GPIO_BTN_CAPTURE, BTN_NUM_C, false, false },
    { "Home", GPIO_BTN_HOME, BTN_NUM_Z, false, false }
};

buttons64_s buttons64[NUM_PHYSICAL_BTNS_64] = 
{
    { "A", GPIO_BTN_CIRCLE, BTN_NUM_A, false, false, true },
    { "B", GPIO_BTN_CROSS, BTN_NUM_X, false, false, true },
    { "Start", GPIO_BTN_START, BTN_NUM_START, false, false, true },    
    { "Z", GPIO_BTN_L2, BTN_NUM_L2, false, false, true },
    { "L", GPIO_BTN_L1, BTN_NUM_L1, false, false, true },
    { "R", GPIO_BTN_R1, BTN_NUM_R1, false, false, true },
    { "C-Up", GPIO_BTN_SQUARE, BTN_NUM_C, false, false, REPORT_C_UP_AS_BUTTON },
    { "C-Down", GPIO_BTN_R2, BTN_NUM_B, false, false, REPORT_C_DOWN_AS_BUTTON },
    { "C-Left", GPIO_BTN_TRIANGLE, BTN_NUM_Y, false, false, REPORT_C_LEFT_AS_BUTTON },  
    { "C-Right", GPIO_BTN_SELECT, BTN_NUM_Z, false, false, REPORT_C_RIGHT_AS_BUTTON }
};

joysticks_s joysticks[NUM_OF_JOYSTICKS] =
{
    { "Joystick L", ADC_STICK_LX, JOYSTICK_CEN, JOYSTICK_CEN, BLE_REVERSE_AXIS_LX, ADC_STICK_LY, JOYSTICK_CEN, JOYSTICK_CEN, BLE_REVERSE_AXIS_LY },
    { "Joystick R", ADC_STICK_RX, JOYSTICK_CEN, JOYSTICK_CEN, BLE_REVERSE_AXIS_RX, ADC_STICK_RY, JOYSTICK_CEN, JOYSTICK_CEN, BLE_REVERSE_AXIS_RY }
};

dpad_s dpads[NUM_OF_DPADS * 4] =
{
    { "Up", GPIO_BTN_DPAD_UP, false },
    { "Down", GPIO_BTN_DPAD_DOWN, false },
    { "Left", GPIO_BTN_DPAD_LEFT, false },
    { "Right", GPIO_BTN_DPAD_RIGHT, false }
};

triggers_s triggers[NUM_OF_TRIGGERS] =
{
    { "Trigger L", ADC_TRIGGER_L, JOYSTICK_CEN, JOYSTICK_CEN, BTN_NUM_L2, REVERSE_TRIGGER_L },
    { "Trigger R", ADC_TRIGGER_R, JOYSTICK_CEN, JOYSTICK_CEN, BTN_NUM_R2, REVERSE_TRIGGER_R }
};

void encoder_pot_x(void * pvParameters)
{
    if (gpio_get_level(GPIO_ENCODER_IX) == gpio_get_level(GPIO_ENCODER_QX))
        count_x++;
    else
        count_x--;
    
    if (count_x > N64_JOYSTICK_MAX)
        count_x = N64_JOYSTICK_MAX;

    if (count_x < N64_JOYSTICK_MIN)
        count_x = N64_JOYSTICK_MIN;        

    //vTaskDelay(5);
}

void encoder_pot_y(void * pvParameters)
{
    if (gpio_get_level(GPIO_ENCODER_IY) == gpio_get_level(GPIO_ENCODER_QY))
        count_y++;
    else
        count_y--;

    if (count_y > N64_JOYSTICK_MAX)
        count_y = N64_JOYSTICK_MAX;

    if (count_y < N64_JOYSTICK_MIN)
        count_y = N64_JOYSTICK_MIN;        

    //vTaskDelay(5);
}

void triggers_task()
{
    const char* TAG = "triggers_task";

    for (int i = 0; i < NUM_OF_TRIGGERS; i++)
    {
        triggers[i].current_state_trigger = read_trigger(triggers[i].adc_channel, triggers[i].reverse_trigger);

        if (triggers[i].current_state_trigger != triggers[i].previous_state_trigger)
        {
            ESP_LOGI(TAG, "%s: %d", triggers[i].name, triggers[i].current_state_trigger);

            if (REPORT_TRIGGERS_AS == DIGITAL)
            {
                if (triggers[i].current_state_trigger)
                {
                    ESP_LOGI(TAG, "Press Button: %s", triggers[i].name);
                    bleGamepad.press(triggers[i].physical_button);
                }
                else
                {
                    ESP_LOGI(TAG, "Release Button: %s", triggers[i].name);
                    bleGamepad.release(triggers[i].physical_button);                    
                }
            }
            else
            {
                if (i == 0)
                    bleGamepad.setLeftTrigger(triggers[i].current_state_trigger);
                else
                    bleGamepad.setRightTrigger(triggers[i].current_state_trigger);
            }
            
            bleGamepad.sendReport();
            triggers[i].previous_state_trigger = triggers[i].current_state_trigger;
        }
    }
}

void dpad_task(void * pvParameters)
{
    while (true)
    {
        regread_dpad = REG_READ(GPIO_IN_REG) & PIN_BIT_MASK;

        if (bleGamepad.isConnected())
        {
            dpads[0].state_dpad = read_button(regread_dpad, dpads[0].gpio_num_dpad);
            dpads[1].state_dpad = read_button(regread_dpad, dpads[1].gpio_num_dpad);
            dpads[2].state_dpad = read_button(regread_dpad, dpads[2].gpio_num_dpad);
            dpads[3].state_dpad = read_button(regread_dpad, dpads[3].gpio_num_dpad);

            current_dpad = get_dpad(dpads[0].state_dpad, dpads[1].state_dpad,
                                    dpads[2].state_dpad, dpads[3].state_dpad);

            if(current_dpad != previous_dpad)
            {
                bleGamepad.setHat(current_dpad);
                bleGamepad.sendReport();

                previous_dpad = current_dpad;
            }
        }

        vTaskDelay(5);
    }
}

void button_task(void * pvParameters)
{
    const char* TAG = "button_task";

    while (true)
    {
        regread_button = REG_READ(GPIO_IN_REG) & PIN_BIT_MASK;

        if (bleGamepad.isConnected())
        {
            for (int i = 0; i < NUM_PHYSICAL_BTNS; i++)
            {
                if ((buttons[i].gpio_num == GPIO_NUM_32 || buttons[i].gpio_num == GPIO_NUM_33) && TRIGGER_TYPE == DIGITAL)
                    buttons[i].current_state = !gpio_get_level((gpio_num_t)buttons[i].gpio_num);
                else
                    buttons[i].current_state = read_button(regread_button, buttons[i].gpio_num);

                if (buttons[i].current_state != buttons[i].previous_state)
                {
                    if (buttons[i].current_state)
                    {
                        ESP_LOGI(TAG, "Press Button: %s", buttons[i].name);
                        bleGamepad.press(buttons[i].physical_button);
                    }
                    else
                    {
                        ESP_LOGI(TAG, "Release Button: %s", buttons[i].name);
                        bleGamepad.release(buttons[i].physical_button);
                    }

                    bleGamepad.sendReport();
                    buttons[i].previous_state = buttons[i].current_state;
                }
            }
        }
        
        syncController(regread_button);

        vTaskDelay(5);
    }
}

void button_task64(void * pvParameters)
{
    const char* TAG = "button_task64";

    while (true)
    {
        regread_button = REG_READ(GPIO_IN_REG) & PIN_BIT_MASK;

        if (bleGamepad.isConnected())
        {
            for (int i = 0; i < NUM_PHYSICAL_BTNS_64; i++)
            {
                if (!buttons64[i].report_as_button)
                    continue;

                buttons64[i].current_state = read_button(regread_button, buttons64[i].gpio_num);

                if (buttons64[i].current_state != buttons64[i].previous_state)
                {
                    if (buttons64[i].current_state)
                    {
                        ESP_LOGI(TAG, "Press Button: %s", buttons64[i].name);                        
                        bleGamepad.press(buttons64[i].physical_button);
                    }
                    else
                    {
                        ESP_LOGI(TAG, "Release Button: %s", buttons64[i].name);
                        bleGamepad.release(buttons64[i].physical_button);
                    }
                       
                    bleGamepad.sendReport();
                    buttons64[i].previous_state = buttons64[i].current_state;
                }
            }
        }
        
        syncController(regread_button);

        vTaskDelay(5);
    }
}

void joystick_task(void * pvParameters)
{
    const char* TAG = "joystick_task";

    while(true)
    {
        if (bleGamepad.isConnected())
        {
            if (TRIGGER_TYPE == ANALOG)
                triggers_task();

            for (int i = 0; i < NUM_OF_JOYSTICKS; i++)
            {
                joysticks[i].current_state_axis_x = read_joystick(joysticks[i].adc_channel_axis_x, joysticks[i].reverse_axis_x);
                joysticks[i].current_state_axis_y = read_joystick(joysticks[i].adc_channel_axis_y, joysticks[i].reverse_axis_y);

                if(joysticks[i].current_state_axis_x != joysticks[i].previous_state_axis_x || 
                    joysticks[i].current_state_axis_y != joysticks[i].previous_state_axis_y)
                {
                    ESP_LOGI(TAG, "%sX: %d", joysticks[i].name, joysticks[i].current_state_axis_x);
                    ESP_LOGI(TAG, "%sY: %d", joysticks[i].name, joysticks[i].current_state_axis_y);

                    if (i == 0)
                        bleGamepad.setLeftThumb(joysticks[i].current_state_axis_x, joysticks[i].current_state_axis_y);
                    else
                    {
                        if (PRIMARY_CONTROLLER == N64)
                            bleGamepad.setRightThumb(JOYSTICK_CEN, JOYSTICK_CEN);
                        else
                            bleGamepad.setRightThumb(joysticks[i].current_state_axis_x, joysticks[i].current_state_axis_y);
                    }

                    bleGamepad.sendReport();

                    joysticks[i].previous_state_axis_x = joysticks[i].current_state_axis_x;
                    joysticks[i].previous_state_axis_y = joysticks[i].current_state_axis_y;
                }
            }
        }

        vTaskDelay(5);
    }
}

void button_c_task64(void * pvParameters)
{
    const char* TAG = "button_c_task64";

    int16_t current_axis_ry;
    int16_t current_axis_rx;
    int16_t previous_axis_ry;
    int16_t previous_axis_rx;
    uint32_t regread_button_c;

    previous_axis_ry = JOYSTICK_CEN;
    previous_axis_rx = JOYSTICK_CEN;

    while(true)
    {
        if (bleGamepad.isConnected())
        {
            regread_button_c = REG_READ(GPIO_IN_REG) & PIN_BIT_MASK;
            current_axis_ry = JOYSTICK_CEN;
            current_axis_rx = JOYSTICK_CEN;

            for (int i = 0; i < NUM_PHYSICAL_BTNS_64; i++)
            {
                if (buttons64[i].report_as_button)
                    continue;

                buttons64[i].current_state = read_button(regread_button_c, buttons64[i].gpio_num);

                if (buttons64[i].current_state)
                {
                    if (strcmp(buttons64[i].name, "C-Up") == 0)
                        current_axis_ry = JOYSTICK_MIN;
            
                    if (strcmp(buttons64[i].name, "C-Down") == 0)
                        current_axis_ry = JOYSTICK_MAX;

                    if (strcmp(buttons64[i].name, "C-Left") == 0)
                        current_axis_rx = JOYSTICK_MIN;

                    if (strcmp(buttons64[i].name, "C-Right") == 0)
                        current_axis_rx = JOYSTICK_MAX;
                }
            }

            if (current_axis_rx != previous_axis_rx || current_axis_ry != previous_axis_ry)
            {
                ESP_LOGI(TAG, "Joystick RX: %d ", current_axis_rx);
                ESP_LOGI(TAG, "Joystick RY: %d ", current_axis_ry);

                bleGamepad.setRightThumb(current_axis_rx, current_axis_ry);
                bleGamepad.sendReport();

                previous_axis_rx = current_axis_rx;
                previous_axis_ry = current_axis_ry;
            }
        }

        vTaskDelay(5);
    }
}

void joystick_n64_task(void * pvParameters)
{
    const char* TAG = "joystick_n64_task";
    int pot_x;
    int pot_y;

    while(true)
    {
        if (bleGamepad.isConnected())
        {
            if (TRIGGER_TYPE == ANALOG)
                triggers_task();

            pot_x = count_x;
            pot_y = count_y;

            joystick64.current_state_pot_x = read_joystick_n64(pot_x, BLE_N64_REVERSE_AXIS_LX);
            joystick64.current_state_pot_y = read_joystick_n64(pot_y, BLE_N64_REVERSE_AXIS_LY);

            if(joystick64.current_state_pot_x != joystick64.previous_state_pot_x || 
                joystick64.current_state_pot_y != joystick64.previous_state_pot_y)
            {
                ESP_LOGI(TAG, "%sX: %d / N64:%d", joystick64.name, joystick64.current_state_pot_x, pot_x);
                ESP_LOGI(TAG, "%sY: %d / N64:%d", joystick64.name, joystick64.current_state_pot_y, pot_y);

                bleGamepad.setLeftThumb(joystick64.current_state_pot_x, joystick64.current_state_pot_y);
                bleGamepad.sendReport();

                joystick64.previous_state_pot_x = joystick64.current_state_pot_x;
                joystick64.previous_state_pot_y = joystick64.current_state_pot_y;
            }
        }

        vTaskDelay(5);
    }
}

void blegamepad_init()
{
    bool include_joystick_left;
    bool include_joystick_right;
    bool include_triggers;

    include_joystick_left = (PRIMARY_CONTROLLER == PRO || PRIMARY_CONTROLLER == N64);
    include_joystick_right = (PRIMARY_CONTROLLER == PRO || PRIMARY_CONTROLLER == N64);
    include_triggers = false; //TRIGGER_TYPE == ANALOG;

    setup_gpios();
    initFlashBleGamepad();

    BleGamepadConfiguration bleGamepadConfig;
    bleGamepadConfig.setAutoReport(false);
    bleGamepadConfig.setAxesMin(JOYSTICK_MIN);
    bleGamepadConfig.setAxesMax(JOYSTICK_MAX);
    bleGamepadConfig.setButtonCount(NUM_OF_BUTTONS);
    bleGamepadConfig.setHatSwitchCount(NUM_OF_DPADS);
    bleGamepadConfig.setIncludeXAxis(include_joystick_left);     // Left Joystick X
    bleGamepadConfig.setIncludeYAxis(include_joystick_left);     // Left Joystick Y
    bleGamepadConfig.setIncludeZAxis(include_joystick_right);    // Right Joystick X
    bleGamepadConfig.setIncludeRzAxis(include_joystick_right);   // Right Joystick Y
    bleGamepadConfig.setIncludeRxAxis(include_triggers);        // Left Trigger
    bleGamepadConfig.setIncludeRyAxis(include_triggers);        // Right Trigger
    
    bleGamepad.begin(&bleGamepadConfig);

    if (JOYSTICK_TYPE == PRO_J)
    {
        xTaskCreatePinnedToCore(joystick_task, "joystick_task", 4096, NULL, 1, NULL, 0);

        if (PRIMARY_CONTROLLER == N64)
            xTaskCreatePinnedToCore(button_task64, "button_task64", 4096, NULL, 1, NULL, 0);
        else
            xTaskCreatePinnedToCore(button_task, "button_task", 4096, NULL, 1, NULL, 0);
    }
    else
    {
        gpio_install_isr_service(0);
        gpio_isr_handler_add(GPIO_ENCODER_IX, encoder_pot_x, NULL);
        gpio_isr_handler_add(GPIO_ENCODER_IY, encoder_pot_y, NULL);

        xTaskCreatePinnedToCore(joystick_n64_task, "joystick_n64_task", 4096, NULL, 1, NULL, 0);
        xTaskCreatePinnedToCore(button_task64, "button_task64", 4096, NULL, 1, NULL, 0);

        if (!REPORT_C_RIGHT_AS_BUTTON || !REPORT_C_LEFT_AS_BUTTON || !REPORT_C_UP_AS_BUTTON || !REPORT_C_DOWN_AS_BUTTON)
            xTaskCreatePinnedToCore(button_c_task64, "button_c_task64", 4096, NULL, 1, NULL, 0);
    }

    xTaskCreatePinnedToCore(dpad_task, "dpad_task", 4096, NULL, 1, NULL, 0);

    while (true)
    {
        if (bleGamepad.isConnected())
        {
            setLed();
            break;
        }
    }
}
