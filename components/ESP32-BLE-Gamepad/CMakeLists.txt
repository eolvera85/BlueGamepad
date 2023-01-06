set(srcs "BleConnectionStatus.cpp" 
        "BleGamepad_main.cpp"
        "BleGamepad.cpp" 
        "BleGamepadConfiguration.cpp" )

set(incde "include")        

set(reqrs "esp-nimble-cpp" "Common")

idf_component_register(SRCS "${srcs}"
             INCLUDE_DIRS "${incde}"
             REQUIRES "${reqrs}")