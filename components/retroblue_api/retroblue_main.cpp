#include "retroblue_main.h"
#include "Josticks.h"

uint32_t regread = 0;
int countx = 0;
int county = 0;
int pot_x = 0;
int pot_y = 0;
uint16_t jostick64_x = 2048;
uint16_t jostick64_y = 2048;
const char* TAG = "joystick_n64_task";

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

void button_task()
{
    regread = REG_READ(GPIO_IN_REG) & PIN_BIT_MASK;

    g_button_data.d_up      = read_button(regread, GPIO_BTN_DPAD_UP);
    g_button_data.d_down    = read_button(regread, GPIO_BTN_DPAD_DOWN);
    g_button_data.d_left    = read_button(regread, GPIO_BTN_DPAD_LEFT) && !read_button(regread, GPIO_BTN_START);    //Only LEFT
    g_button_data.d_right   = read_button(regread, GPIO_BTN_DPAD_RIGHT) && !read_button(regread, GPIO_BTN_START);   //Only RIGHT

    g_button_data.b_right   = read_button(regread, GPIO_BTN_CIRCLE) && !read_button(regread, GPIO_BTN_START);       //Only A
    g_button_data.b_down    = read_button(regread, GPIO_BTN_CROSS) && !read_button(regread, GPIO_BTN_START);        //Only B
    g_button_data.b_up      = read_button(regread, GPIO_BTN_TRIANGLE);                                              // **N64 C-LEFT
    g_button_data.b_left    = read_button(regread, GPIO_BTN_SQUARE);                                                // **N64 C-UP

    g_button_data.t_l       = (read_button(regread, GPIO_BTN_L1) && !read_button(regread, GPIO_BTN_START)) ||       // Only L
                            (read_button(regread, GPIO_BTN_START) && read_button(regread, GPIO_BTN_CROSS));         // L = (+) + B To NES 
    g_button_data.t_r       = (read_button(regread, GPIO_BTN_R1) && !read_button(regread, GPIO_BTN_START)) ||       // Only R
                            (read_button(regread, GPIO_BTN_START) && read_button(regread, GPIO_BTN_CIRCLE));        // R = (+) + A To NES


    if (TRIGGER_TYPE == DIGITAL)
    {
        g_button_data.t_zl      = !gpio_get_level(GPIO_BTN_L2) ||                                                   // Only ZL **N64 ZL (Z)
                                (read_button(regread, GPIO_BTN_START) && read_button(regread, GPIO_BTN_L1));        // ZL = (+) + L To SNES
        g_button_data.t_zr      = (JOYSTICK_TYPE == N64_J) ? !gpio_get_level(GPIO_BTN_R2) :                          // **N64 C-DOWN
                                !gpio_get_level(GPIO_BTN_R2) ||                                                     // Only ZR
                                (read_button(regread, GPIO_BTN_START) && read_button(regread, GPIO_BTN_R1));        // ZR = (+) + R To SNES/N64
    }
    else
    {
        g_button_data.t_zl      = read_trigger_button(ADC_TRIGGER_L, REVERSE_TRIGGER_L) ||                          // Only ZL
                                (read_button(regread, GPIO_BTN_START) && read_button(regread, GPIO_BTN_L1));        // ZL = (+) + L To SNES/N64
        g_button_data.t_zr      = read_trigger_button(ADC_TRIGGER_R, REVERSE_TRIGGER_R) ||                          // Only ZR
                                (read_button(regread, GPIO_BTN_START) && read_button(regread, GPIO_BTN_R1));        // ZR = (+) + R To SNES/N64
    }

    g_button_data.b_select  = read_button(regread, GPIO_BTN_SELECT);                                                // **N64 C-RIGHT
    g_button_data.b_start   = read_button(regread, GPIO_BTN_START) && 
                            !read_button(regread, GPIO_BTN_CIRCLE) && 
                            !read_button(regread, GPIO_BTN_CROSS) && 
                            !read_button(regread, GPIO_BTN_L1) && 
                            !read_button(regread, GPIO_BTN_R1) &&
                            !read_button(regread, GPIO_BTN_DPAD_LEFT) &&
                            !read_button(regread, GPIO_BTN_DPAD_RIGHT);                                             // Only Start

    g_button_data.b_capture = read_button(regread, GPIO_BTN_CAPTURE) ||                                             // Only Capture
                            (read_button(regread, GPIO_BTN_START) && read_button(regread, GPIO_BTN_DPAD_LEFT));     // Capture = (+) + LEFT
    g_button_data.b_home    = read_button(regread, GPIO_BTN_HOME) ||                                                // Only Home
                            (read_button(regread, GPIO_BTN_START) && read_button(regread, GPIO_BTN_DPAD_RIGHT));    // Home = (+) + RIGHT

    g_button_data.sb_left   = JOYSTICK_TYPE != N64_J ? read_button(regread, GPIO_BTN_L3) : 
                            (read_button(regread, GPIO_BTN_START) && read_button(regread, GPIO_BTN_R1));            // **N64 ZR
    g_button_data.sb_right  = JOYSTICK_TYPE != N64_J ? read_button(regread, GPIO_BTN_R3) : false;                 

    regread = 0;
}

void stick_task()
{
    
    g_stick_data.lsx = read_jostick_retroblue(ADC_STICK_LX, RBS_REVERSE_AXIS_LX);
    g_stick_data.lsy = read_jostick_retroblue(ADC_STICK_LY, RBS_REVERSE_AXIS_LY);
    g_stick_data.rsx = read_jostick_retroblue(ADC_STICK_RX, RBS_REVERSE_AXIS_RX);
    g_stick_data.rsy = read_jostick_retroblue(ADC_STICK_RY, RBS_REVERSE_AXIS_RY);

    return;
}

void stick_task_n64()
{
    pot_x = countx;
    pot_y = county;

    jostick64_x = read_jostick_n64_retroblue(pot_x, RBS_N64_REVERSE_AXIS_LX);
    jostick64_y = read_jostick_n64_retroblue(pot_y, RBS_N64_REVERSE_AXIS_LY);

    ESP_LOGI(TAG, "%sX: %d / N64:%d", "Jostick L", jostick64_x, pot_x);
    ESP_LOGI(TAG, "%sY: %d / N64:%d", "Jostick L", jostick64_y, pot_y);

    g_stick_data.lsx = jostick64_x;
    g_stick_data.lsy = jostick64_y;
    g_stick_data.rsx = 2048;
    g_stick_data.rsy = 2048;

    return;
}

void sync_button_task(void * pvParameters)
{
    uint32_t regSync;

    while (true)
    {
        regSync = regread == 0 ? REG_READ(GPIO_IN_REG) & PIN_BIT_MASK : regread;

        syncController(regSync);

        vTaskDelay(5);
    }
}

void retroblue_init()
{
    setup_gpios();

    xTaskCreatePinnedToCore(sync_button_task, "sync_button_task", 2048, NULL, 0, NULL, 0);

    rb_register_button_callback(button_task);

    if (JOYSTICK_TYPE == PRO_J)
        rb_register_stick_callback(stick_task);
    else
    {
        gpio_install_isr_service(0);
        gpio_isr_handler_add(GPIO_ENCODER_IX, handleEncoderX, NULL);
        gpio_isr_handler_add(GPIO_ENCODER_IY, handleEncoderY, NULL);

        rb_register_stick_callback(stick_task_n64);
    }

    rb_api_startController();
}