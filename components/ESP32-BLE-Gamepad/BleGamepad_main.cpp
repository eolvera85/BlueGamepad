#include "BleGamepad_main.h"

BleGamepad bleGamepad("BlueGamepad", "EOLVERA85", 100);

uint32_t regread_button = 0;
uint32_t regread_dpad = 0;
volatile int count_x = 0;
volatile int count_y = 0;

signed char current_dpad = DPAD_CENTERED;
signed char previous_dpad = DPAD_CENTERED;


jostick_64_s jostick64 = 
{
    "Jostick L", 0, 0, BLE_N64_REVERSE_AXIS_LX, 0, 0, BLE_REVERSE_AXIS_LY
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

josticks_s josticks[NUM_OF_JOSTICKS] =
{
    { "Jostick L", ADC_STICK_LX, 0, 0, BLE_REVERSE_AXIS_LX, ADC_STICK_LY, 0, 0, BLE_REVERSE_AXIS_LY },
    { "Jostick R", ADC_STICK_RX, 0, 0, BLE_REVERSE_AXIS_RX, ADC_STICK_RY, 0, 0, BLE_REVERSE_AXIS_RY }
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
    { "Trigger L", ADC_TRIGGER_L, 0, 0, BTN_NUM_L2, REVERSE_TRIGGER_L },
    { "Trigger R", ADC_TRIGGER_R, 0, 0, BTN_NUM_R2, REVERSE_TRIGGER_R }
};

void encoder_pot_x(void * pvParameters)
{
    if (gpio_get_level(GPIO_ENCODER_IX) == gpio_get_level(GPIO_ENCODER_QX))
        count_x++;
    else
        count_x--;
    
    if (count_x > N64_JOSTICK_MAX)
        count_x = N64_JOSTICK_MAX;

    if (count_x < N64_JOSTICK_MIN)
        count_x = N64_JOSTICK_MIN;        

    //vTaskDelay(5);
}

void encoder_pot_y(void * pvParameters)
{
    if (gpio_get_level(GPIO_ENCODER_IY) == gpio_get_level(GPIO_ENCODER_QY))
        count_y++;
    else
        count_y--;

    if (count_y > N64_JOSTICK_MAX)
        count_y = N64_JOSTICK_MAX;

    if (count_y < N64_JOSTICK_MIN)
        count_y = N64_JOSTICK_MIN;        

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

void joystick_task(void * pvParameters)
{
    const char* TAG = "joystick_task";

    while(true)
    {
        if (bleGamepad.isConnected())
        {
            if (TRIGGER_TYPE == ANALOG)
                triggers_task();

            for (int i = 0; i < NUM_OF_JOSTICKS; i++)
            {
                josticks[i].current_state_axis_x = read_joystick(josticks[i].adc_channel_axis_x, josticks[i].reverse_axis_x);
                josticks[i].current_state_axis_y = read_joystick(josticks[i].adc_channel_axis_y, josticks[i].reverse_axis_y);

                if(josticks[i].current_state_axis_x != josticks[i].previous_state_axis_x || 
                    josticks[i].current_state_axis_y != josticks[i].previous_state_axis_y)
                {
                    ESP_LOGI(TAG, "%sX: %d", josticks[i].name, josticks[i].current_state_axis_x);
                    ESP_LOGI(TAG, "%sY: %d", josticks[i].name, josticks[i].current_state_axis_y);

                    //bleGamepad.setX(josticks[i].current_state_axis_x);    // Left X
                    //bleGamepad.setY(josticks[i].current_state_axis_y);    // Left Y
                    //bleGamepad.setY(josticks[i].current_state_axis_x);    // Right X
                    //bleGamepad.setRZ(josticks[i].current_state_axis_y);   // Right Y

                    if (i == 0)
                        bleGamepad.setLeftThumb(josticks[i].current_state_axis_x, josticks[i].current_state_axis_y);
                    else
                        bleGamepad.setRightThumb(josticks[i].current_state_axis_x, josticks[i].current_state_axis_y);

                    bleGamepad.sendReport();

                    josticks[i].previous_state_axis_x = josticks[i].current_state_axis_x;
                    josticks[i].previous_state_axis_y = josticks[i].current_state_axis_y;
                }
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

            jostick64.current_state_axis_x = read_joystick_n64(pot_x, BLE_N64_REVERSE_AXIS_LX);
            jostick64.current_state_axis_y = read_joystick_n64(pot_y, BLE_N64_REVERSE_AXIS_LY);

            if(jostick64.current_state_axis_x != jostick64.previous_state_axis_x || 
                jostick64.current_state_axis_y != jostick64.previous_state_axis_y)
            {
                ESP_LOGI(TAG, "%sX: %d / N64:%d", jostick64.name, jostick64.current_state_axis_x, pot_x);
                ESP_LOGI(TAG, "%sY: %d / N64:%d", jostick64.name, jostick64.current_state_axis_y, pot_y);

                bleGamepad.setLeftThumb(jostick64.current_state_axis_x, jostick64.current_state_axis_y);
                bleGamepad.sendReport();

                jostick64.previous_state_axis_x = jostick64.current_state_axis_x;
                jostick64.previous_state_axis_y = jostick64.current_state_axis_y;
            }
        }

        vTaskDelay(5);
    }
}

void blegamepad_init()
{
    bool include_jostick_left;
    bool include_jostick_right;
    bool include_triggers;

    include_jostick_left = (PRIMARY_CONTROLLER == PRO || PRIMARY_CONTROLLER == N64);
    include_jostick_right = PRIMARY_CONTROLLER == PRO;
    include_triggers = false; //TRIGGER_TYPE == ANALOG;

    setup_gpios();

    BleGamepadConfiguration bleGamepadConfig;
    bleGamepadConfig.setAutoReport(false);
    bleGamepadConfig.setAxesMin(JOYSTICK_MIN);
    bleGamepadConfig.setAxesMax(JOYSTICK_MAX);
    bleGamepadConfig.setButtonCount(NUM_OF_BUTTONS);
    bleGamepadConfig.setHatSwitchCount(NUM_OF_DPADS);
    bleGamepadConfig.setIncludeXAxis(include_jostick_left);     // Left Jostick X
    bleGamepadConfig.setIncludeYAxis(include_jostick_left);     // Left Jostick Y
    bleGamepadConfig.setIncludeZAxis(include_jostick_right);    // Right Jostick X
    bleGamepadConfig.setIncludeRzAxis(include_jostick_right);   // Right Jostick Y
    bleGamepadConfig.setIncludeRxAxis(include_triggers);        // Left Trigger
    bleGamepadConfig.setIncludeRyAxis(include_triggers);        // Right Trigger
    
    bleGamepad.begin(&bleGamepadConfig);

    if (JOSTICK_TYPE == PRO_J)
        //xTaskCreate(joystick_task, "joystick_task", 4096, NULL, 1, NULL);
        xTaskCreatePinnedToCore(joystick_task, "joystick_task", 4096, NULL, 1, NULL, 0);
    else
    {
        gpio_install_isr_service(0);
        ESP_ERROR_CHECK(gpio_isr_handler_add(GPIO_ENCODER_IX, encoder_pot_x, NULL));
        ESP_ERROR_CHECK(gpio_isr_handler_add(GPIO_ENCODER_IY, encoder_pot_y, NULL));

        //xTaskCreate(joystick_n64_task, "joystick_n64_task", 4096, NULL, 1, NULL);
        xTaskCreatePinnedToCore(joystick_n64_task, "joystick_n64_task", 4096, NULL, 1, NULL, 0);
    }

    //xTaskCreate(dpad_task, "dpad_task", 4096, NULL, 1, NULL);
    //xTaskCreate(button_task, "button_task", 4096, NULL, 1, NULL);
    xTaskCreatePinnedToCore(dpad_task, "dpad_task", 4096, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(button_task, "button_task", 4096, NULL, 1, NULL, 0);
}
