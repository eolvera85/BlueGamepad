#include "Buttons.h"

bool read_button(uint32_t bytes, uint8_t bit)
{
    return !((bytes >> bit) & 0x1);
}

signed char get_dpad(bool up, bool down, bool left, bool right)
{
    if (up && right)
        return DPAD_UP_RIGHT;

    if (up && left)
        return DPAD_UP_LEFT;

    if (down && right)
        return DPAD_DOWN_RIGHT;

    if (down && left)
        return DPAD_DOWN_LEFT;

    if (up)
        return DPAD_UP;

    if (right)
        return DPAD_RIGHT;

    if (down)
        return DPAD_DOWN;

    if (left)
        return DPAD_LEFT;

    return DPAD_CENTERED;
}


