#ifndef BUTTONS_H
#define BUTTONS_H

#include "GPIOs.h"
#include "BleGamepad.h"
#include "Parameters.h"

#define BTN_NUM_A       BUTTON_1   
#define BTN_NUM_B       BUTTON_2
#define BTN_NUM_C       BUTTON_3
#define BTN_NUM_X       BUTTON_4
#define BTN_NUM_Y       BUTTON_5
#define BTN_NUM_Z       BUTTON_6
#define BTN_NUM_L1      BUTTON_7
#define BTN_NUM_R1      BUTTON_8
#define BTN_NUM_L2      BUTTON_9
#define BTN_NUM_R2      BUTTON_10
#define BTN_NUM_SELECT  BUTTON_11
#define BTN_NUM_START   BUTTON_12
#define BTN_NUM_MENU    BUTTON_13
#define BTN_NUM_L3      BUTTON_14
#define BTN_NUM_R3      BUTTON_15

#define NUM_OF_BUTTONS      15
#define NUM_PHYSICAL_BTNS   14
#define NUM_OF_DPADS        1
#define NUM_PHYSICAL_BTNS_64   10

// Input pin mask creation
#define GPIO_BITMASK_BUTTONS  (1ULL<<GPIO_BTN_DPAD_UP) | (1ULL<<GPIO_BTN_DPAD_DOWN) | (1ULL<<GPIO_BTN_DPAD_LEFT) | (1ULL<<GPIO_BTN_DPAD_RIGHT) | (1ULL<<GPIO_BTN_SELECT) | (1ULL<<GPIO_BTN_START) | (1ULL<<GPIO_BTN_L1) | (1ULL<<GPIO_BTN_L2) | (1ULL<<GPIO_BTN_L3) | (1ULL<<GPIO_BTN_R3) | (1ULL<<GPIO_BTN_R1) | (1ULL<<GPIO_BTN_R2) | (1ULL<<GPIO_BTN_CIRCLE) | (1ULL<<GPIO_BTN_CROSS) | (1ULL<<GPIO_BTN_TRIANGLE) | (1ULL<<GPIO_BTN_SQUARE) | (1ULL<<GPIO_BTN_HOME) | (1ULL<<GPIO_BTN_CAPTURE) | (1ULL<<GPIO_BTN_SYNC)
#define GPIO_BITMASK_TRIGGERS  (1ULL<<GPIO_BTN_DPAD_UP) | (1ULL<<GPIO_BTN_DPAD_DOWN) | (1ULL<<GPIO_BTN_DPAD_LEFT) | (1ULL<<GPIO_BTN_DPAD_RIGHT) | (1ULL<<GPIO_BTN_SELECT) | (1ULL<<GPIO_BTN_START) | (1ULL<<GPIO_BTN_L1) | (1ULL<<GPIO_BTN_L3) | (1ULL<<GPIO_BTN_R3) | (1ULL<<GPIO_BTN_R1) | (1ULL<<GPIO_BTN_CIRCLE) | (1ULL<<GPIO_BTN_CROSS) | (1ULL<<GPIO_BTN_TRIANGLE) | (1ULL<<GPIO_BTN_SQUARE) | (1ULL<<GPIO_BTN_HOME) | (1ULL<<GPIO_BTN_CAPTURE) | (1ULL<<GPIO_BTN_SYNC)

#define GPIO_BITMASK_N64        (1ULL<<GPIO_ENCODER_QX) | (1ULL<<GPIO_ENCODER_QY)
#define GPIO_BITMASK_N64_INTR   (1ULL<<GPIO_ENCODER_IX) | (1ULL<<GPIO_ENCODER_IY)

#define GPIO_BITMASK_LED   (1ULL<<GPIO_LED_SYNC_SWI) | (1ULL<<GPIO_LED_SYNC_BLE)

struct dpad_s
{
    char name[10];
    gpio_num_t gpio_num_dpad;
    bool state_dpad;
};

struct buttons_s
{
    char name[10];
    gpio_num_t gpio_num;
    uint8_t physical_button;
    bool current_state;
    bool previous_state;
};

struct buttons64_s
{
    char name[10];
    gpio_num_t gpio_num;
    uint8_t physical_button;
    bool current_state;
    bool previous_state;
    bool report_as_button;
};

extern dpad_s dpads[NUM_OF_DPADS * 4];
extern buttons_s buttons[NUM_PHYSICAL_BTNS];
extern buttons64_s buttons64[NUM_PHYSICAL_BTNS_64];

EXTERNC bool read_button(uint32_t bytes, uint8_t bit);
EXTERNC signed char get_dpad(bool up, bool down, bool left, bool right);

#endif