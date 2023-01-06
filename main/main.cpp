#include "main.h"

extern "C" void app_main()
{
    CONTROLLER_TYPE_E controller_type;

    controller_type = readController();

    if (controller_type == BLEGAMEPAD)
        blegamepad_init();
    else 
        retroblue_init();
}
