#include "joypad.h"

uint8_t Joypad::get_buttons()
{
    return buttons_;
}

uint8_t Joypad::get_dpad()
{
    return dpad_;
}

uint8_t Joypad::get_selection()
{
    return selection_;
}

void Joypad::set_buttons(uint8_t value)
{
    buttons_ = value;
}

void Joypad::set_dpad(uint8_t value)
{
    dpad_ = value;
}

void Joypad::set_selection(uint8_t value)
{
    selection_ &= 0x0f;
    selection_ |= (value);
}