# BlueGamepad
Emulate a gamepad with ESP32 compatible to PC/Android/Switch

This project merges two source codes to form a single firmware to be compatible with both Nintendo Switch and PC/Android. To switch between Nintendo Switch mode and PC/Android mode can be done by two options:

- Pin 15 (Button to GND)
- Holding the (+) / START button for 10 seconds

### Default GPIOs
| GPIO | Button |
| ----------------- | ----- |
| 3 | Stick Button Right / R3 |
| 4 | Capture |
| 5 | X |
| 12 | ( - ) / SELECT |
| 13 | ( + ) / START |
| 14 | Dpad Right |
| 15 | Sync | 
| 16 | Home | 
| 17 | Y |
| 18 | A |
| 19 | B |
| 21 | Stick Button Left / L3 |
| 22 | R / R1 |
| 23 | L / L1  |
| 25 | Dpad Up |
| 26 | Dpad Down |
| 27 | Dpad Left |
| 32 | ZL / L2 |
| 33 | ZR / R2 |
| 34 | Right Stick X Axis |
| 35 | Right Stick Y Axis |
| 36 | Left Stick X Axis |
| 39 | Left Stick Y Axis |

### Soft-buttons:
To complement the miss of some buttons in the switch controller, virtual buttons are defined

| Emulated button | Obtained by pressing |
| ----------------- | ----------------- |
| L | (+) + B |
| R | (+) + A |
| ZL | (+) + L |
| ZR | (+) + R | 
| Capture | (+) + Dpad Left |
| Home | (+) + Dpad Right |

Resources used:
- https://github.com/HandHeldLegend/HOJA-LIB-ESP32
- https://github.com/lemmingDev/ESP32-BLE-Gamepad
- https://github.com/h2zero/esp-nimble-cpp
- https://github.com/h2zero/esp-nimble-component