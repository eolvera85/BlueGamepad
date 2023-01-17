#ifndef GPIOS_H
#define GPIOS_H

#include "driver/adc.h"
#include "driver/gpio.h"
#include "Parameters.h"

// Jostick N64 type
#define GPIO_ENCODER_IX         GPIO_NUM_35
#define GPIO_ENCODER_QX         GPIO_NUM_34
#define GPIO_ENCODER_IY         GPIO_NUM_32
#define GPIO_ENCODER_QY         GPIO_NUM_33

// ADC channels for analog stick input
#define ADC_STICK_LX        ADC1_CHANNEL_0  // VP
#define ADC_STICK_LY        ADC1_CHANNEL_3  // VN
#define ADC_STICK_RX        ADC1_CHANNEL_6  // D34
#define ADC_STICK_RY        ADC1_CHANNEL_7  // D35

#define ADC_TRIGGER_L       ADC1_CHANNEL_4  // D32 ZL - Trigger
#define ADC_TRIGGER_R       ADC1_CHANNEL_5  // D33 ZR - Trigger

#if JOYSTICK_TYPE == N64_J
    #define GPIO_BTN_L2         GPIO_NUM_21     // D21 %N64=Z  
    #define GPIO_BTN_R2         GPIO_NUM_3      // RX0 %N64=C-DOWN

    #define GPIO_BTN_L3         GPIO_NUM_0      // 
    #define GPIO_BTN_R3         GPIO_NUM_0      // 
#else
    #define GPIO_BTN_L2         GPIO_NUM_32     // D32  ZL
    #define GPIO_BTN_R2         GPIO_NUM_33     // D33  ZR

    #define GPIO_BTN_L3         GPIO_NUM_21     // D21  Stick L3
    #define GPIO_BTN_R3         GPIO_NUM_3      // RX0  Stick R3
#endif

#define GPIO_BTN_DPAD_UP    GPIO_NUM_25     // D25
#define GPIO_BTN_DPAD_DOWN  GPIO_NUM_26     // D26
#define GPIO_BTN_DPAD_LEFT  GPIO_NUM_27     // D27
#define GPIO_BTN_DPAD_RIGHT GPIO_NUM_14     // D14

#define GPIO_BTN_SELECT     GPIO_NUM_12     // D12   MINUS %N64=C-RIGHT
#define GPIO_BTN_START      GPIO_NUM_13     // D13   PLUS %N64=START

#define GPIO_BTN_L1         GPIO_NUM_23     // D23   L %N64=L
#define GPIO_BTN_R1         GPIO_NUM_22     // D22   R %N64=R


#define GPIO_BTN_CROSS      GPIO_NUM_19     // D19   B %N64=B
#define GPIO_BTN_CIRCLE     GPIO_NUM_18     // D18   A %N64=A 
#define GPIO_BTN_TRIANGLE   GPIO_NUM_5      // D5    X %N64=C-LEFT % 
#define GPIO_BTN_SQUARE     GPIO_NUM_17     // TX2   Y %N64=C-UP % 

#define GPIO_BTN_HOME       GPIO_NUM_16     // RX2
#define GPIO_BTN_CAPTURE    GPIO_NUM_4      // D4

// Pins for Sync & Battery
#define ADC_BATTERY_LEVEL   ADC2_CHANNEL_2  // D2

#define GPIO_LED_SYNC_BLE   GPIO_NUM_0      // D0
#define GPIO_LED_SYNC       GPIO_NUM_1      // TX0
#define GPIO_BTN_SYNC       GPIO_NUM_15     // D15

extern uint64_t PIN_BIT_MASK;

void setup_gpios();

#endif