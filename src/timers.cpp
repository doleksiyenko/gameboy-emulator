#include "timers.h"

void Timers::increment_cycle_counter()
{
    t_cycle_counter += 1;
}

void Timers::increment_tima()
{
    if (tima_ + 1 > 0xff) {
        // request timer interrupt
    }
}

uint8_t Timers::read_div()
{
    return div_;
}
uint8_t Timers::read_tima()
{
    return tima_;
}
uint8_t Timers::read_tma()
{
    return tma_;
}
uint8_t Timers::read_tac()
{
    return tac_;
}