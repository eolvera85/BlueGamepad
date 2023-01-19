#ifndef PARAMETERS_H
#define PARAMETERS_H

#ifdef __cplusplus
    #define EXTERNC extern "C"
#else
    #define EXTERNC
#endif

//ENUMS
enum JOYSTICK_TYPE_E { PRO_J = 1, N64_J = 2 };
enum TRIGGER_TYPE_E { DIGITAL = 0, ANALOG = 1 };

enum CONTROLLER_TYPE_E { SWITCH = 0, BLEGAMEPAD = 1 };
enum PRIMARY_CONTROLLER_E { PRO = 0x03, NES = 0x09, SNES = 0x0B, N64 = 0x0C };

// ************ From here change values to configure the controls ************
// Begin Parameters Controller Type (Change *Joystick PRO/N64)

#define PRIMARY_CONTROLLER      N64             // Values: Enum -> PRIMARY_CONTROLLER_E
#define JOYSTICK_TYPE           N64_J           // VALUES: Enum -> JOYSTICK_TYPE_E

//NO CHANGE
#if PRIMARY_CONTROLLER == N64
    #define SECONDARY_CONTROLLER    0x11
#else
    #define SECONDARY_CONTROLLER    0x02
#endif

// Joysticks Switch
#define RBS_REVERSE_AXIS_LX     false
#define RBS_REVERSE_AXIS_LY     true
#define RBS_REVERSE_AXIS_RX     true
#define RBS_REVERSE_AXIS_RY     false
// Joysticks Switch N64
#define RBS_N64_REVERSE_AXIS_LX     false
#define RBS_N64_REVERSE_AXIS_LY     false

// Joysticks BleGamepad
#define BLE_REVERSE_AXIS_LX     false
#define BLE_REVERSE_AXIS_LY     false
#define BLE_REVERSE_AXIS_RX     true
#define BLE_REVERSE_AXIS_RY     true
// Joysticks BleGamepad N64
#define BLE_N64_REVERSE_AXIS_LX     false
#define BLE_N64_REVERSE_AXIS_LY     true

// Triggers
#define TRIGGER_TYPE            DIGITAL      // VALUES: Enum -> TRIGGER_TYPE_E
#define REPORT_TRIGGERS_AS      DIGITAL     // VALUES: Enum -> TRIGGER_TYPE_E
#define REVERSE_TRIGGER_L       true
#define REVERSE_TRIGGER_R       true

// CONTROLLER N64 ON BLUERETRO (WHY? ARE REPORT AS JOYSTICK R)
#define REPORT_C_RIGHT_AS_BUTTON    true
#define REPORT_C_LEFT_AS_BUTTON     true
#define REPORT_C_UP_AS_BUTTON       true
#define REPORT_C_DOWN_AS_BUTTON     true

// End Parameters
// *******************************************************************************

#endif
