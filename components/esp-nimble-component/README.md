# esp-nimble-component
Updated esp-nimble fork to use as a separate ESP32 component in ESP-IDF v3.2 and v3.3.  

# Purpose:
This repo is provided to allow ESP-IDF v3.2 & v3.3 users to use NimBLE for their BLE projects instead of bluedroid.  

## Arduino as a component Note:
If you are using IDF 3.3, which already includes NimBLE, and also using Arduino as a component you will need this to use NimBLE as well.  
This is because enabling NimBLE in menuconfig will disable bluedroid and will result in compilation  
errors that require disabling many Arduino libraries.  
This repo solves that by using NimBLE as a component in the project folder.  

# Using:
Download or clone this repo into your `project/components` folder and run `menuconfig`.  
Configure settings in `main menu -> NimBLE Options`.  
   
**DO NOT** enable NimBLE in `Component config -> Bluetooth` if using v3.3.2 and Arduino (see note above).
Bluetooth and Bluedroid both have to be enabled though.
   
A C++ library is [available here](https://github.com/h2zero/esp-nimble-cpp) for use with NimBLE that is (mostly) compatible  
with the original cpp_utils and esp32-Arduino BLE library.  
   
# Current tracking

This repo is currently using espressif/esp-nimble @f4ae049
