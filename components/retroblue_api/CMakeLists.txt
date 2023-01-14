set(srcs "retroblue_api.cpp" "retroblue_backend.cpp" 
        "retroblue_settings.cpp" "rbc_switch_spi.cpp" 
        "rbc_switch_reports.cpp" "rbc_switch_input.cpp" 
        "rbc_switch_core.cpp" "rbc_switch_controller.cpp" 
        "rbc_switch_comms.cpp" "retroblue_main.cpp")

set(incde "include") 

set(reqrs "Common")

idf_component_register(SRCS "${srcs}"
             INCLUDE_DIRS "${incde}"
             REQUIRES "${reqrs}")

